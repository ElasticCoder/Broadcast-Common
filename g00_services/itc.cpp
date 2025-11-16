///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itc.cpp       
// File Type   : C++ Source
// Description : Inter Thread Communications Control Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itc.h>
#include <g01_event_logging.h>
#include "comms_event_codes.h"
#include "itq_info.h"
#include "itc_heap_info.h"

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itc constructor
// Author               : Duncan Olesen
// Date Created         : 22/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

C_itc::C_itc()
: m_access_control((const tc *) NULL)
{
	m_msg_data_heap_h	= NULL;
	m_q_list_idx_m		= NULL_INDEX_M;
	m_q_free_list_idx_m	= NULL_INDEX_M;
	m_q_info_p			= NULL;
	m_itc_status		= ITC_UNINITIALISED;
	m_max_queue_size_m	= 0;
	m_max_queues_m		= 0;
	m_data_allocated_m	= 0;
	m_buffers_allocated_m = 0;
} // C_itc

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itc destructor
// Author               : Duncan Olesen
// Date Created         : 22/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

C_itc::~C_itc()
{
	if (m_q_info_p != NULL)
	{
		delete [] m_q_info_p;
	}
//
// Check for memory leakage
//
	if (m_data_allocated_m != 0)
	{
		CString temp;

		temp.Format(_TEXT("bytes leaked = %u, buffers leaked = %u"),
					m_data_allocated_m,
					m_buffers_allocated_m);

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 1,
			CString(_TEXT("C_itc::~C_itc: memory leak")),
			temp,
			G01_SE_RAISED);

#ifdef _DEBUG
		itc_dump_heap(m_msg_data_heap_h);
#endif _DEBUG
	}

	if (m_msg_data_heap_h != (HANDLE) NULL)
	{
		HeapDestroy(m_msg_data_heap_h);
		m_msg_data_heap_h = NULL;
	}
} // ~C_itc

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itc initialisation
// Author               : Duncan Olesen
// Date Created         : 22/03/96
// Date Last Modified   : 22/03/96
///////////////////////////////////////////////////////////////////////////////

T_itc_status C_itc::initialise(const S_itc_conf &pc_config)
{
	T_g03_lock_status		lock_status;
	C_g02_timeout_control	timeout(INFINITE);
//
// Obtain exclusive access to object
//
	lock_status = lock_itc(timeout);

	if (lock_status == LOCKED)
	{
		if (m_itc_status != ITC_UNINITIALISED)
		{
//
// The ITC has already been initialised
//
			m_itc_status = ITC_ERROR;
		}
		else
		{
			m_itc_status = ITC_OK;
			m_max_queues_m = pc_config.m_max_queues_m;
			m_max_queue_size_m = pc_config.m_max_queue_size_m;
//
// Allocate array of queue information structures
//
			m_q_info_p = new C_itq_info[pc_config.m_max_queues_m];

			if (m_q_info_p == (C_itq_info *) NULL)
			{
				m_itc_status = ITC_ERROR;
			}
			else
			{
//
// Link queue info structs into the free list
//
				for (m i_m = 0; i_m < pc_config.m_max_queues_m - 1; i_m++)
				{
					m_q_info_p[i_m].m_next_info_idx_m = i_m + 1;
					m_q_info_p[i_m].set_queue_status(Q_FREE);
				}
			
				if (pc_config.m_max_queues_m > 0)
				{
					m_q_info_p[pc_config.m_max_queues_m - 1].
						m_next_info_idx_m = NULL_INDEX_M;
					m_q_info_p[pc_config.m_max_queues_m - 1].
						set_queue_status(Q_FREE);
					m_q_free_list_idx_m = 0;
				}
				else
				{
					m_q_free_list_idx_m = NULL_INDEX_M;
				}
			}

			if (m_itc_status == ITC_OK)
			{

//
// Create heap for message data
//
				m_msg_data_heap_h = HeapCreate(0, 0, pc_config.m_heap_size_l);

				if (m_msg_data_heap_h == (HANDLE) NULL)
				{
					m_itc_status = ITC_ERROR;

					delete [] m_q_info_p;
					m_q_info_p = NULL;
					m_q_free_list_idx_m = NULL_INDEX_M;
				}
			}
		} // Not already initialised
//
// Release exclusive lock on control object
//
		unlock_itc();
	}

	return m_itc_status;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate memory for inter thread message
// Author               : Duncan Olesen
// Date Created         : 22/03/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

void *C_itc::alloc_msg_data(l p_msg_size_l)
{
	void					*ret_val_p;
	S_heap_head_sentinel	*head_sentinel_p;
	S_heap_tail_sentinel	*tail_sentinel_p;
	DWORD					dwAllocSize;

	ret_val_p = NULL;

	if (m_msg_data_heap_h == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 2,
			CString(_TEXT("C_itc::alloc_msg_data: heap uninitialised")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (p_msg_size_l > 0)
	{
//
// Allocate space for requested buffer and for sentinel structure
//
		dwAllocSize = p_msg_size_l + sizeof(S_heap_head_sentinel) +
					  sizeof(S_heap_tail_sentinel);

		head_sentinel_p = (S_heap_head_sentinel *) HeapAlloc(
						m_msg_data_heap_h,
						HEAP_ZERO_MEMORY,
						dwAllocSize);

		if (head_sentinel_p != (S_heap_head_sentinel *) NULL)
		{
//
// Buffer successfully allocated, so set up sentinels
//
			m_data_allocated_m += p_msg_size_l;
			m_buffers_allocated_m++;
			head_sentinel_p->m_magic_num1_m = HEAP_SENTINEL_M;
			head_sentinel_p->m_magic_num2_m = ~HEAP_SENTINEL_M;
			head_sentinel_p->m_data_size_m  = p_msg_size_l;
#ifdef _DEBUG
			head_sentinel_p->m_status		= ITC_DATA_ALLOCATED;
			g00_thread_name(head_sentinel_p->m_alloc_thread);
			head_sentinel_p->m_msg_id_m		= 0;
#endif _DEBUG

			tail_sentinel_p = (S_heap_tail_sentinel *)
					(((b*) (head_sentinel_p + 1)) + p_msg_size_l);
			tail_sentinel_p->m_magic_num_m = HEAP_SENTINEL_M;
//
// set return pointer to address after head sentinel
//
			ret_val_p = (void *) (head_sentinel_p + 1);
		}
	}
	else
	{
		CString temp;

		temp.Format(_TEXT("data size = %d"), p_msg_size_l);

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 3,
			CString(_TEXT("C_itc::alloc_msg_data: illegal size")),
			temp,
			G01_SE_RAISED);
	}

	return ret_val_p;
} // alloc_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Release memory for inter thread message
// Author               : Duncan Olesen
// Date Created         : 22/03/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

void C_itc::free_msg_data(void *p_msg_p)
{
	S_heap_head_sentinel *head_sentinel_p;
	S_heap_tail_sentinel *tail_sentinel_p;

	if (m_msg_data_heap_h == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 4,
			CString(_TEXT("C_itc::free_msg_data: heap uninitialised")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (p_msg_p != NULL)
	{
//
// Check that allocated buffer has a valid head sentinel
//
		head_sentinel_p = ((S_heap_head_sentinel *) p_msg_p) - 1;

		if (head_sentinel_p->m_magic_num1_m == HEAP_SENTINEL_M &&
			head_sentinel_p->m_magic_num2_m == ~HEAP_SENTINEL_M)
		{
//
// Head sentinel valid, so update count of allocated bytes
// and free the buffer
//
			m_data_allocated_m -= head_sentinel_p->m_data_size_m;
			m_buffers_allocated_m--;
//
// Check the tail sentinel
//
			tail_sentinel_p = (S_heap_tail_sentinel *)
					(((b *) (head_sentinel_p + 1)) + head_sentinel_p->m_data_size_m);

			if (tail_sentinel_p->m_magic_num_m != HEAP_SENTINEL_M)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 5,
					CString(_TEXT("C_itc::free_msg_data: tail corrupt")),
					CString(_TEXT("")),
					G01_SE_RAISED);
			}
//
// Clear sentinels prior to releasing memory to the heap
//
			head_sentinel_p->m_magic_num1_m = 0;
			head_sentinel_p->m_magic_num2_m = 0;
			tail_sentinel_p->m_magic_num_m  = 0;

			HeapFree(m_msg_data_heap_h, 0, (void *) head_sentinel_p);
		}
		else
		{
//
// Buffer has either been corrupted or was not allocated
// from the inter thread message heap
//
			CString temp;

			temp.Format(_TEXT("buffer address = %p"), p_msg_p);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 6,
				CString(_TEXT("C_itc::free_msg_data: corrupt data")),
				temp,
				G01_SE_RAISED);
		}
	}
} // free_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Find queue information structure
//						: N.B. Assumes lock has already been obtained
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_itq_info *C_itc::find_queue(const tc *pc_queue_name_tcp)
{
	C_itq_info			*ret_val_p;
	z					not_found_z;
	int					string_match;
	m					queue_idx_m;
	C_g02_timeout_control	timeout(INFINITE);

	ret_val_p	= (C_itq_info *) NULL;

	if (pc_queue_name_tcp != NULL &&
		*pc_queue_name_tcp != _TEXT('\0'))
	{
		if (lock_held() == TRUE)
		{
//
// Search for existing queue record
//
			queue_idx_m = m_q_list_idx_m;
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
				m_q_free_list_idx_m != NULL_INDEX_M)
			{
//
// Move record from free list to allocated list
//
				queue_idx_m = m_q_free_list_idx_m;
				m_q_free_list_idx_m = m_q_info_p[queue_idx_m].
							m_next_info_idx_m;
				m_q_info_p[queue_idx_m].m_next_info_idx_m = m_q_list_idx_m;
				m_q_list_idx_m = queue_idx_m;
//
// Initialise queue info
//
				_tcscpy(m_q_info_p[queue_idx_m].m_queue_name_tca,
						pc_queue_name_tcp);
				m_q_info_p[queue_idx_m].m_queue_status  = Q_UNINITIALISED;
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
				G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 7,
				CString(_TEXT("C_itc::find_queue: lock not held")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
	}
	else
	{
//
// Null or blank name
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITC_CODE_BASE_W + 8,
			CString(_TEXT("C_itc::find_queue: blank queue name")),
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

void C_itc::free_queue(C_itq_info *p_queue_info_p)
{
	T_g03_lock_status	lock_status;
	m					queue_idx_m;
	m					prev_idx_m;
	z					not_found_z;
	C_g02_timeout_control	timeout(INFINITE);

	if (p_queue_info_p != NULL)
	{
//
// Obtain exclusive access to object
//
		lock_status = lock_itc(timeout);

		if (lock_status == LOCKED)
		{
			queue_idx_m = p_queue_info_p - m_q_info_p;
//
// If queue belongs to this control object and is not
// already free then decrement usage count
//
			if (queue_idx_m < m_max_queues_m &&
				p_queue_info_p->queue_status() != Q_FREE)
			{
				p_queue_info_p->m_usage_count_m--;

				if (p_queue_info_p->m_usage_count_m == 0)
				{
//
// If no longer in use then return record to the free list
//
					not_found_z	= TRUE;
					prev_idx_m = m_q_list_idx_m;
//
// If record is at head of list then adjust head index
//
					if (queue_idx_m == m_q_list_idx_m)
					{
						m_q_list_idx_m = m_q_info_p[queue_idx_m].
											m_next_info_idx_m;
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
									 m_q_free_list_idx_m;
						m_q_free_list_idx_m = queue_idx_m;

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
					}
				} // no longer in use
			} // in use
//
// Release exclusive access to object
//
			unlock_itc();
		} // obtained lock
	} // pointer not null
} // free_queue
