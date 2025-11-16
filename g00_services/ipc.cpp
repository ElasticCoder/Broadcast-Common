///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipc.cpp
// File Type   : C++ Source
// Description : Inter Process Communications Control Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////


#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <ipc.h>
#include "ipq_info.h"

#include <g01_event_logging.h>
#include "comms_event_codes.h"

struct S_resource_names
{
	tc						m_access_control_name_tca[IPC_NAME_LEN_W + 2];
	tc						m_holder_heap_name_tca[IPC_NAME_LEN_W + 2];
	tc						m_message_heap_name_tca[IPC_NAME_LEN_W + 2];
	tc						m_control_memory_name_tca[IPC_NAME_LEN_W + 2];
};

S_resource_names resource_names(const tc *p_ipc_name_tcp);
			
///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipc constructor used by the process which
//						: configures the IPC
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipc::C_ipc()
: m_access_control(),
  m_message_heap(), 
  m_holder_heap(),
  m_control_memory()
{
	m_ipc_info_p = NULL;
	m_q_info_p	 = NULL;
} // C_ipc

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipc constructor
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipc::C_ipc(const tc *p_ipc_name_tcp)
: m_access_control(),
  m_message_heap(), 
  m_holder_heap(),
  m_control_memory()
{
	m_ipc_info_p = NULL;

	attach(p_ipc_name_tcp);
} // C_ipc

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipc destructor
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipc::~C_ipc()
{
	m_ipc_info_p	= NULL;
	m_q_info_p		= NULL;
} // ~C_ipc

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipc initialisation
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipc_status C_ipc::initialise(const S_ipc_conf &pc_config)
{
	T_ipc_status			ipc_status;
	T_g03_lock_status		lock_status;
	C_g02_timeout_control	timeout(INFINITE);
	const w					default_num_partitions_w = 7;
	S_g03_partition_config	default_config_a[default_num_partitions_w];
	S_g03_partition_config	holder_config;
	m						control_memory_size_m;
	z						init_ok_z;
	S_resource_names		names;

	ipc_status = IPC_OK;
//
// Get shared resource names
//
	names = resource_names(pc_config.m_ipc_name_tca);
//
// Define default message heap configuration
//
	default_config_a[0].m_size_of_element_m = 16;
	default_config_a[0].m_num_elements_m	= 100;
	default_config_a[1].m_size_of_element_m = 32;
	default_config_a[1].m_num_elements_m	= 50;
	default_config_a[2].m_size_of_element_m = 64;
	default_config_a[2].m_num_elements_m	= 50;
	default_config_a[3].m_size_of_element_m = 128;
	default_config_a[3].m_num_elements_m	= 30;
	default_config_a[4].m_size_of_element_m = 256;
	default_config_a[4].m_num_elements_m	= 30;
	default_config_a[5].m_size_of_element_m = 512;
	default_config_a[5].m_num_elements_m	= 20;
	default_config_a[6].m_size_of_element_m = 1024;
	default_config_a[6].m_num_elements_m	= 10;
//
// Initialise the access control object
//
	m_access_control.initialise(names.m_access_control_name_tca);
//
// Obtain exclusive access to object
//
	lock_status = lock_ipc(timeout);

	if (lock_status == LOCKED)
	{
//
// Initialise Message Data heap
//
		if (pc_config.m_num_partitions_w == IPC_DEFAULT_PARTITIONS_W)
		{
			init_ok_z = m_message_heap.configure(
							names.m_message_heap_name_tca,
							default_num_partitions_w,
							default_config_a);
		}
		else
		{
			init_ok_z = m_message_heap.configure(
							names.m_message_heap_name_tca,
							pc_config.m_num_partitions_w,
							pc_config.m_partition_conf_p);
		}

		if (!init_ok_z)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 1,
				CString(_TEXT("C_ipc::initialise: failed to init message heap")),
				pc_config.m_ipc_name_tca,
				G01_SE_RAISED);

			ipc_status = IPC_ERROR;
		}
//
// Initialise Message Holder array heap
//
		holder_config.m_size_of_element_m =
				pc_config.m_queue_size_m * sizeof(S_ipq_msg_holder);
		holder_config.m_num_elements_m	= pc_config.m_max_queues_m;

		init_ok_z = m_holder_heap.configure(
						names.m_holder_heap_name_tca,
						1,
						&holder_config);

		if (!init_ok_z)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 2,
				CString(_TEXT("C_ipc::initialise: failed to init holder heap")),
				pc_config.m_ipc_name_tca,
				G01_SE_RAISED);

			ipc_status = IPC_ERROR;
		}
//
// Initialise shared memory for control info and queue info array
//
		control_memory_size_m = sizeof(S_ipc_info) +
					pc_config.m_max_queues_m * sizeof(C_ipq_info);

		m_ipc_info_p = (S_ipc_info *) m_control_memory.configure(
						names.m_control_memory_name_tca,
						control_memory_size_m);

		if (m_ipc_info_p != NULL)
		{
			m_q_info_p	 = (C_ipq_info *) (m_ipc_info_p + 1);
		}
		else
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 3,
				CString(_TEXT("C_ipc::initialise: failed to init control memory")),
				pc_config.m_ipc_name_tca,
				G01_SE_RAISED);

			ipc_status = IPC_ERROR;
			m_q_info_p = NULL;
		}

		if (ipc_status == IPC_OK)
		{
			
			m_ipc_info_p->m_ipc_status   = ipc_status;
			m_ipc_info_p->m_max_queues_m = pc_config.m_max_queues_m;
			m_ipc_info_p->m_queue_size_m = pc_config.m_queue_size_m;
			m_ipc_info_p->m_q_list_idx_m = NULL_INDEX_M;
//
// Link queue info structs into the free list
//
			for (m i_m = 0; i_m < pc_config.m_max_queues_m; i_m++)
			{
				m_q_info_p[i_m].init();
				m_q_info_p[i_m].m_next_info_idx_m = i_m + 1;
				m_q_info_p[i_m].set_queue_status(IPQ_FREE);
			}

			if (pc_config.m_max_queues_m > 0)
			{
				m_q_info_p[pc_config.m_max_queues_m - 1].
					m_next_info_idx_m = NULL_INDEX_M;
				m_ipc_info_p->m_q_free_list_idx_m = 0;
			}
			else
			{
				m_ipc_info_p->m_q_free_list_idx_m = NULL_INDEX_M;
			}
		}
//
// Release exclusive lock on control object
//
		unlock_ipc();
	}
	else
	{
//
// Could not obtain lock
//
		ipc_status = IPC_ERROR;
	}

	return ipc_status;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Attach to initialised IPC shared memory
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

z C_ipc::attach(const tc	*p_ipc_name_tcp)
{
	z	ret_val_z;
	S_resource_names		names;

	ret_val_z = FALSE;
//
// Get shared resource names
//
	names = resource_names(p_ipc_name_tcp);
//
// Attach to shared resources
//
	m_access_control.attach(names.m_access_control_name_tca);
	m_message_heap.attach(names.m_message_heap_name_tca);
	m_holder_heap.attach(names.m_holder_heap_name_tca);
	m_control_memory.attach(names.m_control_memory_name_tca);
//
// Calculate address of shared data in this process
//
	m_ipc_info_p = (S_ipc_info *) m_control_memory.address();

	if (m_ipc_info_p != NULL)
	{
		m_q_info_p	 = (C_ipq_info *) (m_ipc_info_p + 1);
		ret_val_z	 = TRUE;
	}
	else
	{
		m_q_info_p = NULL;
	}

	return ret_val_z;
} // attach

///////////////////////////////////////////////////////////////////////////////
// Function Description : Find queue information structure
//						: N.B. Assumes lock has already been obtained
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipq_info *C_ipc::find_queue(const tc *pc_queue_name_tcp)
{
	C_ipq_info			*ret_val_p;
	z					not_found_z;
	int					string_match;
	m					queue_idx_m;
	C_g02_timeout_control	timeout(INFINITE);

	ret_val_p	= (C_ipq_info *) NULL;

	if (m_ipc_info_p == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 4,
			CString(_TEXT("C_ipc::find_queue: IPC uninitialised")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (pc_queue_name_tcp == NULL ||
			 *pc_queue_name_tcp == _TEXT('\0'))
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 5,
			CString(_TEXT("C_ipc::find_queue: blank queue name")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (lock_held() == TRUE)
	{
//
// Search for existing queue record
//
		queue_idx_m = m_ipc_info_p->m_q_list_idx_m;
		not_found_z	= TRUE;

		while (not_found_z == TRUE && queue_idx_m != NULL_INDEX_M)
		{
			string_match = _tcscmp(
				pc_queue_name_tcp,
				m_q_info_p[queue_idx_m].m_queue_name_tca);

			if (string_match == 0)
			{
				not_found_z = FALSE;
			}
			else
			{
				queue_idx_m = m_q_info_p[queue_idx_m].
						m_next_info_idx_m;
			}
		}
//
// If not found then allocate a new queue
//
		if (not_found_z != FALSE &&
			m_ipc_info_p->m_q_free_list_idx_m != NULL_INDEX_M)
		{
//
// Move record from free list to allocated list
//
			queue_idx_m = m_ipc_info_p->m_q_free_list_idx_m;
			m_ipc_info_p->m_q_free_list_idx_m = m_q_info_p[queue_idx_m].
						m_next_info_idx_m;
			m_q_info_p[queue_idx_m].m_next_info_idx_m =
						m_ipc_info_p->m_q_list_idx_m;
			m_ipc_info_p->m_q_list_idx_m = queue_idx_m;
//
// Initialise queue info
//
			_tcscpy(m_q_info_p[queue_idx_m].m_queue_name_tca,
					pc_queue_name_tcp);
			m_q_info_p[queue_idx_m].m_queue_status  = IPQ_UNINITIALISED;
		}
//
// If queue found or created then update usage count and set return value
//
		if (queue_idx_m != NULL_INDEX_M)
		{
			m_q_info_p[queue_idx_m].m_usage_count_m++;

			ret_val_p = m_q_info_p + queue_idx_m;
		}
	}
	else
	{
//
// Lock not held by thread
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 6,
			CString(_TEXT("C_ipc::find_queue: lock not held")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}

	return ret_val_p;
} // find_queue

///////////////////////////////////////////////////////////////////////////////
// Function Description : Free queue information structure.
//						: N.B. Assumes lock has not already been obtained
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

void C_ipc::free_queue(C_ipq_info *p_queue_info_p)
{
	T_g03_lock_status	lock_status;
	m					queue_idx_m;
	m					prev_idx_m;
	z					not_found_z;
	C_g02_timeout_control	timeout(INFINITE);

	if (m_ipc_info_p == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 7,
			CString(_TEXT("C_ipc::free_queue: IPC uninitialised")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (p_queue_info_p != NULL)
	{
//
// Obtain exclusive access to object
//
		lock_status = lock_ipc(timeout);

		if (lock_status == LOCKED)
		{
			queue_idx_m = p_queue_info_p - m_q_info_p;
//
// If queue belongs to this control object and is not
// already free then decrement usage count
//
			if (queue_idx_m < m_ipc_info_p->m_max_queues_m &&
				p_queue_info_p->queue_status() != IPQ_FREE)
			{
				p_queue_info_p->m_usage_count_m--;

				if (p_queue_info_p->m_usage_count_m == 0)
				{
//
// If no longer in use then return record to the free list
//
					not_found_z	= TRUE;
					prev_idx_m = m_ipc_info_p->m_q_list_idx_m;
//
// If record is at head of list then adjust head index
//
					if (queue_idx_m == m_ipc_info_p->m_q_list_idx_m)
					{
						m_ipc_info_p->m_q_list_idx_m =
							m_q_info_p[queue_idx_m].m_next_info_idx_m;
						not_found_z = FALSE;
					}
					else
					{
//
// Find record in queue list
//
						while (not_found_z == TRUE &&
							   prev_idx_m != NULL_INDEX_M)
						{
							if (m_q_info_p[prev_idx_m].m_next_info_idx_m ==
								queue_idx_m)
							{
								not_found_z = FALSE;
							}
							else
							{
								prev_idx_m = m_q_info_p[prev_idx_m].
												m_next_info_idx_m;
							}
						}

						if (not_found_z == FALSE)
						{
//
// If record found in list then unlink it
//
							m_q_info_p[prev_idx_m].m_next_info_idx_m =
								m_q_info_p[queue_idx_m].m_next_info_idx_m;
						}
					}

					if (not_found_z == FALSE)
					{
//
// If record found then link it into free list
//
						m_q_info_p[queue_idx_m].m_next_info_idx_m =
									 m_ipc_info_p->m_q_free_list_idx_m;
						m_ipc_info_p->m_q_free_list_idx_m = queue_idx_m;

//
// Destroy queue
//
						p_queue_info_p->destroy_queue(this);
					}
					else
					{
//
// Could not free record
//
						CString temp;

						temp.Format(_TEXT("queue name = %s"),
									p_queue_info_p->m_queue_name_tca);

						g01_log_system_event(
							G01_SE_SOFTWARE_EXCEPTION,
							G01_SE_ERROR,
							G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 8,
							CString(_TEXT("C_ipc::free_queue: could not free queue")),
							temp,
							G01_SE_RAISED);
					}
				} // no longer in use
			} // in use
//
// Release exclusive access to object
//
			unlock_ipc();
		} // obtained lock
	} // pointer not null
} // free_queue


///////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate an array of message holders
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_g03_memory_id	C_ipc::alloc_holders()
{
	T_g03_memory_id	ret_val;
	void			*mem_p;

	ret_val = G03_INVALID_ID;

	if (m_ipc_info_p != NULL)
	{
		mem_p = m_holder_heap.alloc((l)
					m_ipc_info_p->m_queue_size_m * sizeof(S_ipq_msg_holder));

		ret_val = m_holder_heap.memory_id(mem_p);
	}

	return ret_val;
} // alloc_holders

///////////////////////////////////////////////////////////////////////////////
// Function Description : Free an array of message holders
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipc::free_holders(T_g03_memory_id p_holders)
{
	void			*mem_p;

	if (m_ipc_info_p != NULL)
	{
		mem_p = m_holder_heap.address(p_holders);

		m_holder_heap.free(mem_p);
	}
} // free_holders

///////////////////////////////////////////////////////////////////////////////
// Function Description : Determine the status of the IPC
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipc_status	C_ipc::ipc_status() const
{
	T_ipc_status	ret_val;

	ret_val = IPC_UNINITIALISED;

	if (m_ipc_info_p != NULL)
	{
		ret_val = m_ipc_info_p->m_ipc_status;
	}

	return ret_val;
} // ipc_status

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the size of queues
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

m C_ipc::queue_size() const
{
	m		ret_val_m;

	ret_val_m = 0;

	if (m_ipc_info_p != NULL)
	{
		ret_val_m = m_ipc_info_p->m_queue_size_m;
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPC_CODE_BASE_W + 9,
			CString(_TEXT("C_ipc::queue_size: IPC uninitialised")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}

	return ret_val_m;
} // queue_size

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the names of the shared resources
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

S_resource_names resource_names(const tc *p_ipc_name_tcp)
{
	S_resource_names	ret_val;
//
// Access control name
//
	_tcscpy(ret_val.m_access_control_name_tca, p_ipc_name_tcp);
	_tcscat(ret_val.m_access_control_name_tca, _TEXT("A"));
//
// Message holder heap name
//
	_tcscpy(ret_val.m_holder_heap_name_tca, p_ipc_name_tcp);
	_tcscat(ret_val.m_holder_heap_name_tca, _TEXT("H"));
//
// Message data heap name
//
	_tcscpy(ret_val.m_message_heap_name_tca, p_ipc_name_tcp);
	_tcscat(ret_val.m_message_heap_name_tca, _TEXT("D"));
//
// Shared control memory name
//
	_tcscpy(ret_val.m_control_memory_name_tca, p_ipc_name_tcp);
	_tcscat(ret_val.m_control_memory_name_tca, _TEXT("C"));

	return ret_val;
} // ::resource_names
