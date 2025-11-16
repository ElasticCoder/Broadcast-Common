///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipq_info.cpp
// File Type   : C++ Source
// Description : Inter Process Queue Info Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <ipc.h>

#include <g01_event_logging.h>
#include "ipq_info.h"
#include "comms_event_codes.h"

void set_event_name(
			const tc	*p_queue_name_tcp,
			tc			*event_name_tcp);

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue information (pseudo-constructor)
// Author               : Duncan Olesen
// Date Created         : 07/11/96
// Date Last Modified   : 07/11/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_info::init()
{
	m_next_info_idx_m	 = NULL_INDEX_M;
	m_queue_name_tca[0]  = _TEXT('\0');
	m_queue_status       = IPQ_FREE;
	m_reader_connected_z = FALSE;
	m_writer_waiting_z   = FALSE;
	m_usage_count_m		 = 0;

	m_msg_holders		 = G03_INVALID_ID;
	m_free_list_idx_m	 = NULL_INDEX_M;

	m_high_list.init();
	m_low_list.init();
} // init

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the next message from the head of the queue
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_info_get_status C_ipq_info::get_msg(
						C_ipc		*p_ipc_p,
						C_ipq_q_msg &p_msg)
{
	T_ipq_info_get_status	ret_val;
	S_ipq_msg_holder		*holders_p;
	z						message_obtained_z;

	ret_val = IPQ_GET_ERROR;
//
// Convert message holder array memory id into an address in this process
//
	holders_p = (S_ipq_msg_holder *) p_ipc_p->holder_address(m_msg_holders);

	if (holders_p != NULL)
	{
//
// Get high priority message
//
		message_obtained_z = m_high_list.remove_msg(
								holders_p,
								m_free_list_idx_m,
								p_ipc_p,
								p_msg);

		if (message_obtained_z == FALSE)
		{
//
// No high priority messages so get low priority message
//
			message_obtained_z = m_low_list.remove_msg(
									holders_p,
									m_free_list_idx_m,
									p_ipc_p,
									p_msg);
		}

		if (message_obtained_z != FALSE)
		{
			ret_val = IPQ_GET_OK;
		}
	}

	return ret_val;
} // get_msg

///////////////////////////////////////////////////////////////////////////////
// Function Description : Put the message in the priority queue
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_info_put_status C_ipq_info::put_msg(
						C_ipc				*p_ipc_p,
						const C_ipq_q_msg	&p_msg)
{
	T_ipq_info_put_status	ret_val;
	S_ipq_msg_holder		*holders_p;
	z						message_added_z;

	ret_val = IPQ_PUT_ERROR;
//
// Convert message holder array memory id into an address in this process
//
	holders_p = (S_ipq_msg_holder *) p_ipc_p->holder_address(m_msg_holders);

	if (holders_p != NULL)
	{
		if (p_msg.m_priority_b >= IPQ_HIGH_PRIORITY_B)
		{
//
// Add message to high priority queue
//
			message_added_z = m_high_list.add_msg(
								holders_p,
								m_free_list_idx_m,
								p_ipc_p,
								p_msg);
		}
		else
		{
//
// Add message to low priority queue
//
			message_added_z = m_low_list.add_msg(
								holders_p,
								m_free_list_idx_m,
								p_ipc_p,
								p_msg);
		}

		if (message_added_z != FALSE)
		{
			ret_val = IPQ_PUT_OK;
		}
	}

	return ret_val;
} // put_msg

///////////////////////////////////////////////////////////////////////////////
// Function Description : Destroy queue information
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_info::destroy_queue(C_ipc *p_ipc_p)
{
	void	*holder_address_p;
//
// Convert message holder array memory id into an address in this process
//
	holder_address_p = p_ipc_p->holder_address(m_msg_holders);
//
// Free message array
//
	if (holder_address_p != NULL)
	{
//
// Discard all messages on the queue
//
		C_ipq_q_msg	msg;

		while (get_msg(p_ipc_p, msg) == IPQ_GET_OK)
		{
			if (msg.m_data_p != NULL)
			{
				p_ipc_p->free_msg_data(msg.m_data_p);
			}
		}

		p_ipc_p->free_holders(m_msg_holders);
		m_msg_holders = G03_INVALID_ID;
	}
	
	m_free_list_idx_m	 = NULL_INDEX_M;
	m_queue_name_tca[0]  = _TEXT('\0');
	m_queue_status       = IPQ_FREE;
	m_reader_connected_z = FALSE;
	m_writer_waiting_z   = FALSE;
} // destroy_queue

///////////////////////////////////////////////////////////////////////////////
// Function Description : Create queue information. Do nothing if queue
//						: information already exists (reader reconnecting)
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_info::create_queue(C_ipc	*p_ipc_p)
{
	S_ipq_msg_holder	*holders_p;
	m					queue_size_m;

	if (m_msg_holders == G03_INVALID_ID)
	{
		m_free_list_idx_m = NULL_INDEX_M;
//
// Allocate array for queue
//
		m_msg_holders = p_ipc_p->alloc_holders();
//
// Convert message holder array memory id into an address in this process
//
		holders_p = (S_ipq_msg_holder *) p_ipc_p->holder_address(m_msg_holders);

		if (holders_p != NULL)
		{
//
// Set up message holder free list
//
			queue_size_m = p_ipc_p->queue_size();

			for (m i_m = 0; i_m < queue_size_m - 1; i_m++)
			{
				holders_p[i_m].m_next_idx_m = i_m + 1;
				m_high_list.init();
				m_low_list.init();
			}
		
			if (queue_size_m > 0)
			{
				holders_p[queue_size_m - 1].
					m_next_idx_m = NULL_INDEX_M;
				m_free_list_idx_m = 0;
			}
		}
		else
		{
			m_queue_status = IPQ_ERROR;
		}
	} // message holders do not yet exist
} // create_queue

///////////////////////////////////////////////////////////////////////////////
// Function Description : Signal that the reader is connected to the queue
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_info::reader_connected()
{
//
// This function will signal an appropriate event if there are any writer
// threads waiting for the queue reader thread to connect
//
	z		set_ok_z;
	z		error_z;
	tc		event_name_tca[IPQ_QUEUE_NAME_LEN_M + 2];
	HANDLE	event_h;

	error_z = FALSE;

	m_reader_connected_z = TRUE;

	if (m_writer_waiting_z == TRUE)
	{
//
// Open a handle to the reader waiting event
//
		set_event_name(m_queue_name_tca, event_name_tca);

		event_h = OpenEvent(
					EVENT_MODIFY_STATE,	// can set event
					FALSE,				// not inheritable
					event_name_tca);	// Event name

		if (event_h != (HANDLE) NULL)
		{
			set_ok_z = ::SetEvent(event_h);

			if (set_ok_z != TRUE)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + IPQ_INFO_CODE_BASE_W + 1,
					CString(_TEXT("C_ipq_info::reader_connected: event not signalled")),
					CString(m_queue_name_tca),
					G01_SE_RAISED);

				error_z = TRUE;
			}
//
// Close event handle
//
			CloseHandle(event_h);
		}
		else
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPQ_INFO_CODE_BASE_W + 2,
				CString(_TEXT("C_ipq_info::reader_connected: event not created")),
				CString(m_queue_name_tca),
				G01_SE_RAISED);

			error_z = TRUE;
		}
	}

	if (error_z == FALSE)
	{
		m_queue_status	   = IPQ_OK;
		m_writer_waiting_z = FALSE;
	}
	else
	{
		m_queue_status = IPQ_ERROR;
	}
} // reader_connected

///////////////////////////////////////////////////////////////////////////////
// Function Description : Signal that the reader is disconnected from the queue
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_info::reader_disconnected()
{
	m_queue_status		 = IPQ_WAITING_FOR_READER;
	m_reader_connected_z = FALSE;
} // reader_disconnected

///////////////////////////////////////////////////////////////////////////////
// Function Description : Wait for the reader to connect to the queue
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_info_wait_status C_ipq_info::wait_for_reader(
						C_ipc						*p_ipc_p,
						const C_g02_timeout_control	&pc_timeout)
{
//
// This function will wait until the reader thread has connected to the queue
// The function will release the lock on the queue
//
	l						wait_status_l;
	T_ipq_info_wait_status	ret_val;
	tc		event_name_tca[IPQ_QUEUE_NAME_LEN_M + 2];
	HANDLE					event_h;

	ret_val = IPQ_INFO_WAIT_OK;

	if (p_ipc_p == NULL)
	{
		ret_val = IPQ_INFO_WAIT_ERROR;
	}
	else if (m_queue_status == IPQ_ERROR)
	{
		ret_val = IPQ_INFO_WAIT_ERROR;

		p_ipc_p->unlock_ipc();
	}
	else if (m_reader_connected_z == FALSE)
	{
		set_event_name(m_queue_name_tca, event_name_tca);

		if (m_writer_waiting_z == FALSE)
		{
//
// Event doesn't exist yet so create a handle to the
// writer waiting for reader event
//
//
			event_h = CreateEvent(
							NULL,
							TRUE,	// Manual reset
							FALSE,	// Initial state is non-signalled
							event_name_tca);
		}
		else
		{
//
// Open a handle to the writer waiting for reader event
//
			event_h = OpenEvent(
						SYNCHRONIZE,		// can wait on event
						FALSE,				// not inheritable
						event_name_tca);	// Event name
		}

		if (event_h != (HANDLE) NULL)
		{
			m_writer_waiting_z	= TRUE;
//
// Release lock on control object to allow reader to gain access
//
			p_ipc_p->unlock_ipc();
//
// wait for reader to signal connection
//
			wait_status_l = ::WaitForSingleObject(
								event_h,
								pc_timeout.time_left());
//
// Close event handle
//
			CloseHandle(event_h);

			switch (wait_status_l)
			{
			case WAIT_OBJECT_0:
//
// Reader connected, obtain lock to ensure reader has released lock
//
				T_g03_lock_status		lock_status;

				lock_status = p_ipc_p->lock_ipc(pc_timeout);

				switch (lock_status)
				{
				case LOCKED:
					p_ipc_p->unlock_ipc();
					break;

				case LOCK_TIMEOUT:
					ret_val = IPQ_INFO_WAIT_TIMEOUT;
					break;

				default:
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + IPQ_INFO_CODE_BASE_W + 3,
						CString(_TEXT("C_ipq_info::wait_for_reader: lock not re-obtained")),
						CString(m_queue_name_tca),
						G01_SE_RAISED);

					ret_val = IPQ_INFO_WAIT_ERROR;
					break;
				}
				break;

			case WAIT_TIMEOUT:
				ret_val = IPQ_INFO_WAIT_TIMEOUT;
				break;

			default:
//
// Wait for event to be signalled failed
//
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + IPQ_INFO_CODE_BASE_W + 4,
					CString(_TEXT("C_ipq_info::wait_for_reader: event not signalled")),
					CString(m_queue_name_tca),
					G01_SE_RAISED);

				ret_val = IPQ_INFO_WAIT_ERROR;
				break;
			} // switch on re-obtain lock status
		}
		else
		{
//
// Failed to create event
//
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPQ_INFO_CODE_BASE_W + 5,
				CString(_TEXT("C_ipq_info::wait_for_reader: event not created")),
				CString(m_queue_name_tca),
				G01_SE_RAISED);

			ret_val = IPQ_INFO_WAIT_ERROR;
//
// Release lock on control object
//
			p_ipc_p->unlock_ipc();
		}
	}
	else
	{
//
// reader already connected
//
		p_ipc_p->unlock_ipc();
	};

	return ret_val;
} // wait_for_reader

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set up name of reader connected event
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

void set_event_name(
			const tc	*p_queue_name_tcp,
			tc			*event_name_tcp)
{
	_tcscpy(event_name_tcp, p_queue_name_tcp);
	_tcscat(event_name_tcp, _TEXT("E"));

	return;
} // ::set_event_name
