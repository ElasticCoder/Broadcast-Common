///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itrq.cpp
// File Type   : C++ Source
// Description : Inter Thread Communications Read Queue Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itrq.h>

#include <g00_process_registry.h>
#include <g01_event_logging.h>
#include <g01_debug_logging.h>
#include "itq_info.h"
#include "comms_event_codes.h"

#ifdef _DEBUG
#include "itc_heap_info.h"
#endif _DEBUG

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

C_itrq::C_itrq()
: C_itq()
{
} // C_itrq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itrq::C_itrq(C_itc *p_itc_p)
: C_itq(p_itc_p)
{
} // C_itrq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor initialising object
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itrq::C_itrq(C_itc	*p_itc_p,
			   z		p_windows_queue_z,
			   HWND		p_window_h,
			   m		p_queue_size_m,
			   const tc	*pc_queue_name_tcp)
: C_itq(p_itc_p)
{
	(void) init(p_windows_queue_z,
		 p_window_h,
		 p_queue_size_m,
		 INFINITE,
		 pc_queue_name_tcp);
} // C_itrq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq destructor
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itrq::~C_itrq()
{
	disconnect();
} // ~C_itrq

///////////////////////////////////////////////////////////////////////////////
// Function Description : Reads a message from the queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itrq_read_status C_itrq::read(
		C_itq_q_msg		&p_msg,
		const DWORD		pc_dwTimeout,
		const HANDLE	*pc_handle_ph,
		w				p_num_handles_w,
		w				*p_signalled_object_pw)
{
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_g03_lock_status		lock_status;
	T_itq_info_get_status	get_status;
	T_itrq_read_status		ret_val;
	DWORD					dwWaitStatus;
	w						event_code_w;
	HANDLE					handle_ha[MAXIMUM_WAIT_OBJECTS];
	z						parameters_ok_z;

	ret_val		 = RQ_ERROR;
	event_code_w = 0;
//
// Validate multiple kernel object parameters
//
	if (p_num_handles_w >= MAXIMUM_WAIT_OBJECTS ||
		(pc_handle_ph == NULL && p_num_handles_w != 0))
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 0,
			CString(_TEXT("C_itrq::read: invalid parameters")),
			CString(_TEXT("")),
			G01_SE_RAISED);

		parameters_ok_z = FALSE;
	}
	else
	{
//
// set up array of handles to wait on
//
		handle_ha[0] = m_read_semaphore_h;	// message on queue

		for (int i=1; i <= p_num_handles_w; i++)
		{
			handle_ha[i] = pc_handle_ph[i - 1];
		}

		parameters_ok_z = TRUE;
	}

	if (parameters_ok_z == TRUE &&
		m_itc_p != NULL &&
		m_q_info_p != NULL &&
		m_q_info_p->queue_status() == Q_OK)
	{
//
// Obtain permission to get message from queue
//
		dwWaitStatus = ::WaitForMultipleObjects(
								(DWORD) (p_num_handles_w + 1),
								handle_ha,
								FALSE,		// don't wait for all objects
								timeout.time_left());
//
// Handle one of the other kernel objects being signalled
//
		if (dwWaitStatus > WAIT_OBJECT_0 &&
			dwWaitStatus <= WAIT_OBJECT_0 + p_num_handles_w)
		{
			ret_val = RQ_OBJECT_SIGNALLED;

			if (p_signalled_object_pw != NULL)
			{
				*p_signalled_object_pw = (w) (dwWaitStatus - WAIT_OBJECT_0 - 1);
			}
		}
//
// Handle one of the other kernel objects (mutex) being abandoned
//
		else if (dwWaitStatus > WAIT_ABANDONED_0 &&
				 dwWaitStatus <= WAIT_ABANDONED_0 + p_num_handles_w)
		{
			ret_val = RQ_WAIT_ABANDONED;

			if (p_signalled_object_pw != NULL)
			{
				*p_signalled_object_pw = (w) (dwWaitStatus - WAIT_ABANDONED_0 - 1);
			}
		}
		else
		{
//
// Handle message to read from queue
//
			switch(dwWaitStatus)
			{
			case WAIT_OBJECT_0:
//
// obtain exclusive access to control object
//
				lock_status = m_itc_p->lock_itc(timeout);

				switch (lock_status)
				{
				case LOCKED:
					get_status = m_q_info_p->get_msg(p_msg);

					if (get_status == ITQ_GET_OK)
					{
#ifdef _DEBUG
//
// Update the status of any message data
//
						if (p_msg.m_data_p != NULL)
						{
							S_heap_head_sentinel *head_sentinel_p;
//
// Check that allocated buffer has a valid head sentinel
//
							head_sentinel_p = ((S_heap_head_sentinel *)
													p_msg.m_data_p) - 1;

							if (head_sentinel_p->m_magic_num1_m ==
								HEAP_SENTINEL_M &&
								head_sentinel_p->m_magic_num2_m ==
								~HEAP_SENTINEL_M)
							{
								head_sentinel_p->m_status = ITC_DATA_READ;
							}
							else
							{
								CString		temp;

								temp.Format(_TEXT("Msg ID = WM_USER + %d"),
											p_msg.m_id_m - WM_USER);

								g01_log_system_event(
									G01_SE_SOFTWARE_EXCEPTION,
									G01_SE_ERROR,
									G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 1,
									CString(_TEXT("C_itrq::read: corrupt data sentinel")),
									temp,
									G01_SE_RAISED);
							}
						}
#endif _DEBUG
//
// Update count of room for other messages on queue for writers
//
						(void) ReleaseSemaphore(
									m_write_semaphore_h,
									1,
									NULL);

						ret_val = RQ_OK;
					}
					else
					{
						g01_log_system_event(
							G01_SE_SOFTWARE_EXCEPTION,
							G01_SE_ERROR,
							G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 2,
							CString(_TEXT("C_itrq::read: failure to get message")),
							CString(_TEXT("")),
							G01_SE_RAISED);
					}

					m_itc_p->unlock_itc();
					break;
				case LOCK_TIMEOUT:
					ret_val = RQ_TIMEOUT;
					break;
				case LOCK_ERROR:
					event_code_w = 3;
					ret_val = RQ_ERROR;
					break;
				default:
					event_code_w = 4;
					ret_val = RQ_ERROR;
					break;
				} // switch on lock status

				if (ret_val != RQ_OK)
				{
//
// Read not successful, so release read semaphore
//
					(void) ReleaseSemaphore(
								m_read_semaphore_h,
								1,
								NULL);
				}

				if (ret_val == RQ_ERROR && event_code_w != 0)
				{
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						static_cast<w>(G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + event_code_w),
						CString(_TEXT("C_itrq::read: cannot obtain lock")),
						CString(_TEXT("")),
						G01_SE_RAISED);
				}
				break;
			case WAIT_TIMEOUT:
				ret_val = RQ_TIMEOUT;
				break;
			default:
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 5,
					CString(_TEXT("C_itrq::read: cannot obtain semaphore")),
					CString(_TEXT("")),
					G01_SE_RAISED);

				ret_val = RQ_ERROR;
				break;
			} // switch on wait for read semaphore
		} // if message to read from queue
	} // queue in OK state
	else
	{
		CString temp;

		if (m_itc_p == NULL)
		{
			temp.Format(_TEXT("m_itc_p == NULL"));
		}
		else if (m_q_info_p == NULL)
		{
			temp.Format(_TEXT("m_q_info_p == NULL"));
		}
		else
		{
			temp.Format(_TEXT("queue name = %s, state = %d"),
						m_q_info_p->m_queue_name_tca,
						m_q_info_p->queue_status());
		}

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 6,
			CString(_TEXT("C_itrq::read: queue in invalid state")),
			temp,
			G01_SE_RAISED);
	}

	return ret_val;
} // read

///////////////////////////////////////////////////////////////////////////////
// Function Description : Reads a message from the queue
// Author               : Duncan Olesen
// Date Created         : 13/05/96
// Date Last Modified   : 13/05/96
///////////////////////////////////////////////////////////////////////////////

T_itrq_read_status C_itrq::read(
		m				&p_msg_id_m,
		void			*p_msg_buffer_p,
		m				p_buffer_size_m,
		m				&p_msg_data_size_m,
		const DWORD		pc_dwTimeout,
		const HANDLE	*pc_handle_ph,
		w				p_num_handles_w,
		w				*p_signalled_object_pw)
{
	T_itrq_read_status		ret_val;
	C_itq_q_msg				msg;
//
// Read the message
//
	ret_val		 = read(msg,
						pc_dwTimeout,
						pc_handle_ph,
						p_num_handles_w,
						p_signalled_object_pw);
//
// If read successful then extract message data
//
	if (ret_val == RQ_OK)
	{
//
// Check that buffer is large enough to hold the message data
//
		if (p_buffer_size_m >= msg.m_msg_data_size_m)
		{
			p_msg_id_m = msg.m_id_m;
			p_msg_data_size_m = 0;

			if (msg.m_msg_data_size_m != 0 &&
				msg.m_data_p != NULL)
			{
//
// If there is any message data, copy it into buffer
//
				memcpy(p_msg_buffer_p, msg.m_data_p, msg.m_msg_data_size_m);
//
// If valid message data pointer and data size, set data size parameter
//
				p_msg_data_size_m = msg.m_msg_data_size_m;
			}
		}
		else
		{
			CString temp;

			temp.Format(_T("message ID = %lu, message size = %lu,")
						_T("buffer size = %lu"),
						msg.m_id_m,
						msg.m_msg_data_size_m,
						p_buffer_size_m);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 7,
				CString(_TEXT("C_itrq::read: message data overflow")),
				temp,
				G01_SE_RAISED);

			ret_val = RQ_DATA_OVERFLOW;
		}
//
// Release memory holding message data
//
		free_msg_data(msg.m_data_p);
	}

	return ret_val;
} // read

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itrq::initialise(
		z			p_windows_queue_z,
		HWND		p_window_h,
		m			p_queue_size_m,
		const DWORD	pc_dwTimeout,
		const tc	*pc_queue_name_tcp)
{
	T_itq_q_status	ret_val;

	ret_val = Q_ERROR;

	if (m_q_info_p != NULL)
	{
		disconnect();

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 8,
			CString(_TEXT("C_itrq::initialise: re-initialisation not allowed")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
		ret_val = init(p_windows_queue_z,
			 p_window_h,
			 p_queue_size_m,
			 pc_dwTimeout,
			 pc_queue_name_tcp);
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itrq::initialise(
		C_itc		*p_itc_p,
		z			p_windows_queue_z,
		HWND		p_window_h,
		m			p_queue_size_m,
		const DWORD	pc_dwTimeout,
		const tc	*pc_queue_name_tcp)
{
	T_itq_q_status	ret_val;

	ret_val = Q_ERROR;

	if (m_itc_p == NULL || m_itc_p == p_itc_p)
	{
		m_itc_p = p_itc_p;

		if (m_q_info_p != NULL)
		{
			disconnect();

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 9,
				CString(_TEXT("C_itrq::initialise: re-initialisation not allowed")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
			ret_val = init(p_windows_queue_z,
				 p_window_h,
				 p_queue_size_m,
				 pc_dwTimeout,
				 pc_queue_name_tcp);
		}
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Disconnect from queue
// Author               : Duncan Olesen
// Date Created         : 15/04/97
// Date Last Modified   : 15/04/97
///////////////////////////////////////////////////////////////////////////////

void C_itrq::disconnect()
{
	if (m_q_info_p != NULL)
	{
		m_q_info_p->reader_disconnected();
	}

	C_itq::disconnect();
} // disconnect

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itrq::init(
		z			p_windows_queue_z,
		HWND		p_window_h,
		m			p_queue_size_m,
		DWORD		p_dwTimeout,
		const tc	*pc_queue_name_tcp)
{
	C_g02_timeout_control	timeout(p_dwTimeout);
	T_g03_lock_status		lock_status;
	tc						thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	T_itq_q_status			ret_val;
	z						ok_z;

	ret_val = Q_ERROR;
	ok_z	= TRUE;

	if (pc_queue_name_tcp == NULL ||
		*pc_queue_name_tcp == _TEXT('\0'))
	{
//
// Get the name of the current thread if no name provided for the queue
//
		g00_thread_name(thread_name_tca);
		pc_queue_name_tcp = thread_name_tca;
	}
	else if (_tcslen(pc_queue_name_tcp) > ITQ_QUEUE_NAME_LEN_M)
	{
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 10,
				CString(_TEXT("C_itrq::init: queue name too long")),
				CString(pc_queue_name_tcp),
				G01_SE_RAISED);

		ok_z = FALSE;
	}

	if (m_itc_p == NULL)
	{
//
// Control object is NULL
//
		g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 11,
					CString(_TEXT("C_itrq::init: control object is NULL")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);

		ok_z = FALSE;
	}

	if (ok_z)
	{
//
// obtain exclusive access to control object
//
		lock_status = m_itc_p->lock_itc(timeout);

		switch (lock_status)
		{
		case LOCKED:
			if (p_queue_size_m > m_itc_p->max_queue_size())
			{
//
// Queue too big
//
				g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 12,
						CString(_TEXT("C_itrq::init: queue too big")),
						CString(_TEXT("")),
						G01_SE_RAISED);
			}
			else
			{
//
// find queue
//
				m_q_info_p = m_itc_p->find_queue(pc_queue_name_tcp);
//
// if queue found then initialise it
//
				if (m_q_info_p != NULL)
				{
					z	q_ok_z;
					tc	read_q_name_tca[ITQ_QUEUE_NAME_LEN_M + 2];
					tc	write_q_name_tca[ITQ_QUEUE_NAME_LEN_M + 2];

					q_ok_z = TRUE;
					_tcscpy(read_q_name_tca, pc_queue_name_tcp);
					_tcscat(read_q_name_tca, _TEXT("R"));
					_tcscpy(write_q_name_tca, pc_queue_name_tcp);
					_tcscat(write_q_name_tca, _TEXT("W"));

					switch (m_q_info_p->queue_status())
					{
					case Q_UNINITIALISED:
					case Q_WAITING_FOR_READER:
						m_q_info_p->set_queue_status(Q_WAITING_FOR_READER);
						m_q_info_p->create_queue(
										p_windows_queue_z,
										p_window_h,
										p_queue_size_m);

						if (p_windows_queue_z == FALSE)
						{
							DWORD	dwReadSemError;
							DWORD	dwWriteSemError;
//
// Create semaphores if not a windows queue
//
							m_read_semaphore_h = CreateSemaphore(
									NULL, // no security
									0,    // no messages to read
									p_queue_size_m, // max count
									read_q_name_tca);
							dwReadSemError = GetLastError();

							m_write_semaphore_h = CreateSemaphore(
									NULL, // no security
									p_queue_size_m, // messages to write
									p_queue_size_m, // max count
									write_q_name_tca);
							dwWriteSemError = GetLastError();

							if (dwReadSemError == ERROR_ALREADY_EXISTS ||
								dwWriteSemError == ERROR_ALREADY_EXISTS)
							{
								q_ok_z = FALSE;
//
// Semaphore(s) already exist
//
								g01_log_system_event(
									G01_SE_SOFTWARE_EXCEPTION,
									G01_SE_ERROR,
									G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 13,
									CString(_TEXT("C_itrq::init: semaphores already exist")),
									CString(pc_queue_name_tcp),
									G01_SE_RAISED);
							}

							if (m_read_semaphore_h == NULL ||
								m_write_semaphore_h == NULL)
							{
								q_ok_z = FALSE;
//
// Could not create semaphore(s)
//
								g01_log_system_event(
									G01_SE_SOFTWARE_EXCEPTION,
									G01_SE_ERROR,
									G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 14,
									CString(_TEXT("C_itrq::init: create semaphore failure")),
									CString(pc_queue_name_tcp),
									G01_SE_RAISED);
							}
//
// If semaphores not set up OK then close them
//
							if (!q_ok_z)
							{
								if (m_read_semaphore_h != NULL)
								{
									CloseHandle(m_read_semaphore_h);
									m_read_semaphore_h = NULL;
								}

								if (m_write_semaphore_h != NULL)
								{
									CloseHandle(m_write_semaphore_h);
									m_write_semaphore_h = NULL;
								}
							}
						}
						break;
					case Q_ERROR:
						q_ok_z = FALSE;
						break;
					case Q_OK:
//
// Only one reader allowed, do not set queue status in error to avoid
// upsetting existing reader
//
						q_ok_z = FALSE;
						break;
					case Q_FREE:
						q_ok_z = FALSE;
						g01_log_system_event(
							G01_SE_SOFTWARE_EXCEPTION,
							G01_SE_ERROR,
							G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 15,
							CString(_TEXT("C_itrq::init: found queue in FREE state")),
							CString(pc_queue_name_tcp),
							G01_SE_RAISED);
						break;
					}

					if (q_ok_z)
					{
						m_q_info_p->reader_connected();
						ret_val = m_q_info_p->queue_status();
					}
				}
				else
				{
//
// Could not find or create queue
//
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 16,
						CString(_TEXT("C_itrq::init: could not create queue")),
						CString(pc_queue_name_tcp),
						G01_SE_RAISED);
				}
			} // queue size ?
//
// Release exclusive access to control object
//
			m_itc_p->unlock_itc();
//
// If queue not initialised successfully then remove link to queue info
//
			if (ret_val == Q_ERROR)
			{
				m_itc_p->free_queue(m_q_info_p);
				m_q_info_p = NULL;
			}
			break;

		case LOCK_TIMEOUT:
			ret_val = Q_INIT_TIMEOUT;
			break;

		case LOCK_ERROR:
//
// Lock not obtained
//
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 17,
					CString(_TEXT("C_itrq::init: lock not obtained")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);
			break;

		default:
//
// Lock not obtained
//
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 18,
					CString(_TEXT("C_itrq::init: unrecognised lock status")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);
			break;
		} // switch on lock status
	} // control object non-null

	return ret_val;
} // init
