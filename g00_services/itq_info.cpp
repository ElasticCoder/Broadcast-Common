///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq_info.cpp
// File Type   : C++ Source
// Description : Inter Thread Queue Info Class
// Author      : Duncan Olesen
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

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq_info constructor
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

C_itq_info::C_itq_info()
: m_high_list(), m_low_list()
{
	m_queue_name_tca[0]	 = _TEXT('\0');
	m_queue_status		 = Q_UNINITIALISED;
	m_queue_size_m		 = 0;
// NB m_num_msgs_m is not used by anything
	m_num_msgs_m		 = 0;
	m_next_info_idx_m	 = NULL_INDEX_M;
	m_usage_count_m		 = 0;
	m_reader_connected_z = FALSE;
	m_writer_waiting_z   = FALSE;

	m_message_p       = (S_itq_q_msg_holder *) NULL;
	m_free_list_idx_m = NULL_INDEX_M;
	m_windows_queue_z = FALSE;
	m_window_h		  = NULL;
	m_event_h		  = NULL;
} // C_itq_info 

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq_info destructor
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 27/03/96
///////////////////////////////////////////////////////////////////////////////

C_itq_info::~C_itq_info()
{
	if (m_message_p != (S_itq_q_msg_holder *) NULL)
	{
		delete [] m_message_p;
	}

	if (m_event_h != NULL)
	{
		::CloseHandle(m_event_h);
	}
} // ~C_itq_info 

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the next message from the head of the queue
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

T_itq_info_get_status C_itq_info::get_msg(C_itq_q_msg &p_msg)
{
	T_itq_info_get_status	ret_val;
	z						message_obtained_z;

	ret_val = ITQ_GET_ERROR;

	if (m_message_p != NULL)
	{
//
// Get high priority message
//
		message_obtained_z = m_high_list.remove_msg(
								m_message_p,
								m_free_list_idx_m,
								p_msg);

		if (message_obtained_z == FALSE)
		{
//
// No high priority messages so get low priority message
//
			message_obtained_z = m_low_list.remove_msg(
									m_message_p,
									m_free_list_idx_m,
									p_msg);
		}

		if (message_obtained_z != FALSE)
		{
			m_num_msgs_m--;
			ret_val = ITQ_GET_OK;
		}
	}

	return ret_val;
} // get_msg

///////////////////////////////////////////////////////////////////////////////
// Function Description : Put the message in the priority queue
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 27/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_info_put_status C_itq_info::put_msg(const C_itq_q_msg &p_msg)
{
	T_itq_info_put_status	ret_val;
	z						message_added_z;

	ret_val = ITQ_PUT_ERROR;

	if (m_message_p != NULL)
	{
		if (p_msg.m_priority_b >= ITQ_HIGH_PRIORITY_B)
		{
//
// Add message to high priority queue
//
			message_added_z = m_high_list.add_msg(
								m_message_p,
								m_free_list_idx_m,
								p_msg);
		}
		else
		{
//
// Add message to low priority queue
//
			message_added_z = m_low_list.add_msg(
								m_message_p,
								m_free_list_idx_m,
								p_msg);
		}

		if (message_added_z != FALSE)
		{
			m_num_msgs_m++;
			ret_val = ITQ_PUT_OK;
		}
	}

	return ret_val;
} // put_msg

///////////////////////////////////////////////////////////////////////////////
// Function Description : Destroy queue information
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

void C_itq_info::destroy_queue(C_itc *p_itc_p)
{
//
// Free message array
//
	if (m_message_p != NULL)
	{
//
// Discard all messages on the queue
//
		C_itq_q_msg	msg;

		while (get_msg(msg) == ITQ_GET_OK)
		{
			if (msg.m_data_p != NULL)
			{
				p_itc_p->free_msg_data(msg.m_data_p);
			}
		}

		delete [] m_message_p;
		m_message_p = NULL;
	}
	
	m_windows_queue_z = FALSE;
	m_free_list_idx_m = NULL_INDEX_M;

	C_itq_info::destroy_queue();
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Create queue information. Do nothing if queue
//						: information already exists (reader reconnecting)
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

void C_itq_info::create_queue(
				z		p_windows_queue_z,
				HWND	p_window_h,
				m		p_queue_size_m)
{
	m_windows_queue_z = p_windows_queue_z;

	if (p_windows_queue_z == FALSE)
	{
		if (m_message_p == NULL)
		{
//
// Allocate array for queue
//
			m_message_p = new S_itq_q_msg_holder[p_queue_size_m];

			if (m_message_p != NULL)
			{
				m_queue_size_m = p_queue_size_m;
//
// Set up message holder free list
//
				for (m i_m = 0; i_m < m_queue_size_m - 1; i_m++)
				{
					m_message_p[i_m].m_next_idx_m = i_m + 1;
				}
			
				if (m_queue_size_m > 0)
				{
					m_message_p[m_queue_size_m - 1].
						m_next_idx_m = NULL_INDEX_M;
					m_free_list_idx_m = 0;
				}
				else
				{
					m_free_list_idx_m = NULL_INDEX_M;
				}
				
			}
			else
			{
				m_queue_status = Q_ERROR;
			}
		}
	}
	else
	{
		if (m_window_h != NULL)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 1,
				CString(_TEXT("C_itq_info::create_queue: handle not NULL")),
				CString(m_queue_name_tca),
				G01_SE_RAISED);

			m_queue_status = Q_ERROR;
		}
		else
		{
			m_window_h = p_window_h;
		}
	} // window queue
} // create_queue

///////////////////////////////////////////////////////////////////////////////
// Function Description : Signal that the reader is connected to the queue
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 27/03/96
///////////////////////////////////////////////////////////////////////////////

void C_itq_info::reader_connected()
{
//
// This function will signal an appropriate event if there are any writer
// threads waiting for the queue reader thread to connect
//
	z		set_ok_z;
	z		error_z;

	error_z = FALSE;

	m_reader_connected_z = TRUE;

	if (m_writer_waiting_z == TRUE)
	{
		if (m_event_h != (HANDLE) NULL)
		{
			set_ok_z = ::SetEvent(m_event_h);

			if (set_ok_z != TRUE)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 2,
					CString(_TEXT("C_itq_info::reader_connected: event not signalled")),
					CString(m_queue_name_tca),
					G01_SE_RAISED);

				error_z = TRUE;
			}
		}
		else
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 3,
				CString(_TEXT("C_itq_info::reader_connected: event not created")),
				CString(m_queue_name_tca),
				G01_SE_RAISED);

			error_z = TRUE;
		}
	}

	if (error_z == FALSE)
	{
		m_queue_status	   = Q_OK;
		m_writer_waiting_z = FALSE;
	}
	else
	{
		m_queue_status = Q_ERROR;
	}
} // reader_connected

///////////////////////////////////////////////////////////////////////////////
// Function Description : Signal that the reader is disconnected from the queue
// Author               : Duncan Olesen
// Date Created         : 09/04/96
// Date Last Modified   : 09/04/96
///////////////////////////////////////////////////////////////////////////////

void C_itq_info::reader_disconnected()
{
	m_queue_status		 = Q_WAITING_FOR_READER;
	m_reader_connected_z = FALSE;
	m_window_h			 = NULL;

	if (m_event_h != NULL)
	{
		::ResetEvent(m_event_h);
	}
} // reader_disconnected

///////////////////////////////////////////////////////////////////////////////
// Function Description : Wait for the reader to connect to the queue
// Author               : Duncan Olesen
// Date Created         : 27/03/96
// Date Last Modified   : 27/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_info_wait_status C_itq_info::wait_for_reader(
						C_itc						*p_itc_p,
						const C_g02_timeout_control	&pc_timeout)
{
//
// This function will wait until the reader thread has connected to the queue
// The function will release the lock on the queue
//
	l						wait_status_l;
	T_itq_info_wait_status	ret_val;

	ret_val = ITQ_INFO_WAIT_OK;

	if (p_itc_p == NULL)
	{
		ret_val = ITQ_INFO_WAIT_ERROR;
	}
	else if (m_queue_status == Q_ERROR)
	{
		ret_val = ITQ_INFO_WAIT_ERROR;

		p_itc_p->unlock_itc();
	}
	else if (m_reader_connected_z == FALSE)
	{
		if (m_event_h == NULL)
		{
//
// Event doesn't exist yet
//
			m_event_h = CreateEvent(
							NULL,
							TRUE,	// Manual reset
							FALSE,	// Initial state is non-signalled
							m_queue_name_tca);
		}

		if (m_event_h != (HANDLE) NULL)
		{
			m_writer_waiting_z	= TRUE;
//
// Release lock on control object to allow reader to gain access
//
			p_itc_p->unlock_itc();
//
// wait for reader to signal connection
//
			wait_status_l = ::WaitForSingleObject(
								m_event_h,
								pc_timeout.time_left());

			switch (wait_status_l)
			{
			case WAIT_OBJECT_0:
//
// Reader connected, obtain lock to ensure reader has released lock
//
				T_g03_lock_status		lock_status;

				lock_status = p_itc_p->lock_itc(pc_timeout);

				switch (lock_status)
				{
				case LOCKED:
					p_itc_p->unlock_itc();
					break;

				case LOCK_TIMEOUT:
					ret_val = ITQ_INFO_WAIT_TIMEOUT;
					break;

				default:
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 4,
						CString(_TEXT("C_itq_info::wait_for_reader: lock not re-obtained")),
						CString(m_queue_name_tca),
						G01_SE_RAISED);

					ret_val = ITQ_INFO_WAIT_ERROR;
					break;
				}
				break;

			case WAIT_TIMEOUT:
				ret_val = ITQ_INFO_WAIT_TIMEOUT;
				break;

			default:
//
// Wait for event to be signalled failed
//
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 5,
					CString(_TEXT("C_itq_info::wait_for_reader: event not signalled")),
					CString(m_queue_name_tca),
					G01_SE_RAISED);

				ret_val = ITQ_INFO_WAIT_ERROR;
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
				G01_SE_COMMS_CODE_BASE_W + ITQ_INFO_CODE_BASE_W + 6,
				CString(_TEXT("C_itq_info::wait_for_reader: event not created")),
				CString(m_queue_name_tca),
				G01_SE_RAISED);

			ret_val = ITQ_INFO_WAIT_ERROR;
//
// Release lock on control object
//
			p_itc_p->unlock_itc();
		}
	}
	else
	{
//
// reader already connected
//
		p_itc_p->unlock_itc();
	};

	return ret_val;
} // wait_for_reader

///////////////////////////////////////////////////////////////////////////////
// Function Description : Destroy queue information
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

void C_itq_info::destroy_queue()
{
//
// Reset queue data
//
	m_queue_name_tca[0]  = _TEXT('\0');
	m_queue_status       = Q_FREE;
	m_queue_size_m       = 0;
	m_num_msgs_m         = 0;
	m_reader_connected_z = FALSE;
	m_writer_waiting_z   = FALSE;
	m_window_h			 = NULL;

	if (m_event_h != NULL)
	{
		CloseHandle(m_event_h);
		m_event_h = NULL;
	}
} // destroy_queue