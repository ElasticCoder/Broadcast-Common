///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itwq.cpp
// File Type   : C++ Source
// Description : Inter Thread Communications Write Queue Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itwq.h>
#include <g02_timeout_control.h>

#include <g01_event_logging.h>
#include "itq_info.h"
#include "comms_event_codes.h"

#ifdef _DEBUG
#include "itc_heap_info.h"
#endif _DEBUG

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

C_itwq::C_itwq()
: C_itq()
{
} // C_itwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itwq::C_itwq(C_itc *pc_itc_p)
: C_itq(pc_itc_p)
{
} // C_itwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itwq constructor initialising object
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itwq::C_itwq(const tc	*pc_queue_name_tcp,
			   C_itc	*pc_itc_p)
: C_itq(pc_itc_p)
{
	init(pc_queue_name_tcp, INFINITE);
} // C_itwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itwq constructor initialising object
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itwq::C_itwq(
			   w		p_thread_num_w,
			   w		p_thread_inst_w,
			   C_itc	*pc_itc_p)
: C_itq(pc_itc_p)
{
	C_g00_app_name		app_name;
	C_g00_thread_name	thread_name;
	tc					thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];

//
// Get the current application name
//
	g00_app_name(app_name);
//
// Build the name of the desired thread
//
	thread_name.name_thread(
		app_name,
		p_thread_num_w,
		p_thread_inst_w);

	thread_name.thread_name(thread_name_tca);

	(void) init(thread_name_tca, INFINITE);
} // C_itwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_iwrq destructor
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

C_itwq::~C_itwq()
{
	disconnect();
} // ~C_itwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : Writes a message to the queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 07/05/96
///////////////////////////////////////////////////////////////////////////////

T_itwq_write_status C_itwq::write(
		C_itq_q_msg	p_msg,
		const DWORD	pc_dwTimeout)
{
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_g03_lock_status		lock_status;
	T_itq_info_put_status	put_status;
	T_itwq_write_status		ret_val;
	DWORD					dwWaitStatus;
	w						event_code_w = 0;

	ret_val = WQ_ERROR;

	if (m_itc_p != NULL &&
		m_q_info_p != NULL &&
		m_q_info_p->queue_status() == Q_OK)
	{
#ifdef _DEBUG
//
// Record the id of the message that the buffer is being sent in
// and update the status of the buffer
//
		if (p_msg.m_data_p != NULL)
		{
			S_heap_head_sentinel *head_sentinel_p;

//
// Check that allocated buffer has a valid head sentinel
//
			head_sentinel_p = ((S_heap_head_sentinel *) p_msg.m_data_p) - 1;

			if (head_sentinel_p->m_magic_num1_m == HEAP_SENTINEL_M &&
				head_sentinel_p->m_magic_num2_m == ~HEAP_SENTINEL_M)
			{
				_tcscpy(head_sentinel_p->m_read_q_tca,
						m_q_info_p->m_queue_name_tca);
				head_sentinel_p->m_msg_id_m		= p_msg.m_id_m;

				if (m_q_info_p->windows_queue() == FALSE)
				{
					head_sentinel_p->m_status		= ITC_DATA_SENT;
				}
				else
				{
					head_sentinel_p->m_status		= ITC_DATA_POSTED;
				}
			}
			else
			{
				CString		temp;

				temp.Format(_TEXT("Msg ID = WM_USER + %d"),
							p_msg.m_id_m - WM_USER);

				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 0,
					CString(_TEXT("C_itwq::write: corrupt data sentinel")),
					temp,
					G01_SE_RAISED);
			}
		}
#endif _DEBUG
		if (m_q_info_p->windows_queue() == FALSE)
		{
//
// Obtain permission to put message on queue
//
			dwWaitStatus = ::WaitForSingleObject(
								m_write_semaphore_h,
								timeout.time_left());

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
					put_status = m_q_info_p->put_msg(p_msg);

					if (put_status == ITQ_PUT_OK)
					{
//
// Update count of messages on queue for the reader
//
						(void) ReleaseSemaphore(
									m_read_semaphore_h,
									1,
									NULL);

						ret_val = WQ_OK;
					}
					else
					{
						CString		temp;

						temp.Format(_TEXT("Msg ID = WM_USER + %d"),
									p_msg.m_id_m - WM_USER);

						g01_log_system_event(
							G01_SE_SOFTWARE_EXCEPTION,
							G01_SE_ERROR,
							G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 1,
							CString(_TEXT("C_itwq::write: failure to put message")),
							temp,
							G01_SE_RAISED);
					}

					m_itc_p->unlock_itc();
					break;
				case LOCK_TIMEOUT:
					ret_val = WQ_TIMEOUT;
					break;
				case LOCK_ERROR:
					ret_val = WQ_ERROR;
					event_code_w = 2;
					break;
				default:
					ret_val = WQ_ERROR;
					event_code_w = 3;
					break;
				} // switch on lock status

				if (ret_val != WQ_OK)
				{
//
// Write not successful, so release write semaphore
//
					(void) ReleaseSemaphore(
								m_write_semaphore_h,
								1,
								NULL);
				}

				if (ret_val == WQ_ERROR)
				{
					CString		temp;

					temp.Format(_TEXT("Msg ID = WM_USER + %d"),
								p_msg.m_id_m - WM_USER);

					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						static_cast<w>(G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + event_code_w),
						CString(_TEXT("C_itwq::write: cannot obtain lock")),
						temp,
						G01_SE_RAISED);
				}
				break;
			case WAIT_TIMEOUT:
				ret_val = WQ_TIMEOUT;
				break;
			default:
				{
				CString		temp;

				temp.Format(_T("Msg ID = WM_USER + %d, Error = %d, ")
							_T("GetLastError() = 0x%08X, Handle = 0x%08X"),
							p_msg.m_id_m - WM_USER,
							dwWaitStatus,
							GetLastError(),
							(DWORD) m_write_semaphore_h);

				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 4,
					CString(_TEXT("C_itwq::write: cannot obtain semaphore")),
					temp,
					G01_SE_RAISED);

				ret_val = WQ_ERROR;
				} // scope
				break;
			} // switch on wait for write semaphore
		} // not windows queue
		else
		{
			z	post_ok_z;

			post_ok_z = PostMessage(
						m_q_info_p->window(),
						p_msg.m_id_m,
						(WPARAM) p_msg.m_msg_data_size_m,
						(LPARAM) p_msg.m_data_p);

			if (post_ok_z != FALSE)
			{
				ret_val = WQ_OK;
			}
			else
			{
				CString		temp;

				temp.Format(_T("Msg ID = WM_USER + %d, ")
							_T("GetLastError() = 0x%08X"),
							p_msg.m_id_m - WM_USER,
							GetLastError());

				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 5,
					CString(_TEXT("C_itwq::write: failure to post to thread")),
					temp,
					G01_SE_RAISED);
			}
		} // windows queue
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
			temp.Format(_TEXT("queue name = %s, state = %d, Msg Id = WM_USER + %d"),
						m_q_info_p->m_queue_name_tca,
						m_q_info_p->queue_status(),
						p_msg.m_id_m - WM_USER);
		}

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 6,
			CString(_TEXT("C_itwq::write: queue in invalid state")),
			temp,
			G01_SE_RAISED);
	}

	if (ret_val != WQ_OK)
	{
//
// If write is unsuccessful then free any message buffer allocated
//
		free_msg_data(p_msg.m_data_p);
	}

	return ret_val;
} // write

///////////////////////////////////////////////////////////////////////////////
// Function Description : Writes a message to the queue, copying message data
//						: into a buffer allocated within
// Author               : Duncan Olesen
// Date Created         : 07/05/96
// Date Last Modified   : 07/05/96
///////////////////////////////////////////////////////////////////////////////

T_itwq_write_status C_itwq::write(
			C_itq_q_msg	p_msg,
			const void	*p_msg_data_p,
			m			p_msg_data_size_m,
			const DWORD	pc_dwTimeout)
{
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_itwq_write_status		ret_val;

	ret_val = WQ_ERROR;
//
// If there is message data then prepare to allocate a buffer and copy the
// data into it
//
	if (p_msg_data_size_m != 0)
	{
		if (p_msg_data_p != NULL)
		{
			p_msg.m_data_p = alloc_msg_data(p_msg_data_size_m);
			p_msg.m_msg_data_size_m = p_msg_data_size_m;
//
// If buffer allocated then copy data and write message
//
			if (p_msg.m_data_p != NULL)
			{
				memcpy(p_msg.m_data_p, p_msg_data_p, p_msg_data_size_m);

				ret_val = write(p_msg, timeout.time_left());
			} // allocated message data buffer
		} // data size non-zero, data pointer non null
		else
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 7,
				CString(_TEXT("C_itwq::write: p_msg_data_p is NULL")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
	} // data size is non-zero
	else
	{
//
// No message data, validate parameters
//
		if (p_msg_data_p != NULL)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 8,
				CString(_TEXT("C_itwq::write: p_msg_data_p is not NULL")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Write message with no data
//
			p_msg.m_data_p = NULL;
			p_msg.m_msg_data_size_m = p_msg_data_size_m;

			ret_val = write(p_msg, pc_dwTimeout);
		}
	}

	return ret_val;
} // write

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itwq::initialise(
				const tc	*pc_queue_name_tcp,
				const DWORD	pc_dwTimeout)
{
	if (m_q_info_p != NULL)
	{
		disconnect();

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 9,
			CString(_TEXT("C_itwq::initialise: re-initialisation not allowed")),
			CString(pc_queue_name_tcp),
			G01_SE_RAISED);
	}
	else
	{
		init(pc_queue_name_tcp, pc_dwTimeout);
	}

	return queue_status();
} // initialise


///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 23/04/97
// Date Last Modified   : 23/04/97
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itwq::initialise(
					C_itc		*p_itc_p,
                    const tc	*pc_queue_name_tcp,
                    const DWORD	pc_dwTimeout)
{
	T_itq_q_status ret_val;

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
				G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 10,
				CString(_TEXT("C_itwq::initialise: re-initialisation not allowed")),
				CString(pc_queue_name_tcp),
				G01_SE_RAISED);
		}
		else
		{
			ret_val = init(pc_queue_name_tcp, pc_dwTimeout);
		}
	}

	return ret_val;
} // initialise


///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itwq::initialise(
			w			p_thread_num_w,
			w			p_thread_inst_w,
			const DWORD	pc_dwTimeout)
{
	C_g00_app_name		app_name;
	C_g00_thread_name	thread_name;
	tc					thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	T_itq_q_status ret_val;

	ret_val = Q_ERROR;
//
// Get the current application name
//
	g00_app_name(app_name);
//
// Build the name of the desired thread
//
	thread_name.name_thread(
		app_name,
		p_thread_num_w,
		p_thread_inst_w);

	thread_name.thread_name(thread_name_tca);

	if (m_q_info_p != NULL)
	{
		disconnect();

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 11,
			CString(_TEXT("C_itwq::initialise: re-initialisation not allowed")),
			CString(thread_name_tca),
			G01_SE_RAISED);
	}
	else
	{
		ret_val = init(thread_name_tca, pc_dwTimeout);
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itwq::initialise(
			C_itc		*p_itc_p,
			w			p_thread_num_w,
			w			p_thread_inst_w,
			const DWORD	pc_dwTimeout)
{
	C_g00_app_name		app_name;
	C_g00_thread_name	thread_name;
	tc					thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	T_itq_q_status ret_val;

	ret_val = Q_ERROR;
//
// Get the current application name
//
	g00_app_name(app_name);
//
// Build the name of the desired thread
//
	thread_name.name_thread(
		app_name,
		p_thread_num_w,
		p_thread_inst_w);

	thread_name.thread_name(thread_name_tca);

	if (m_itc_p == NULL || m_itc_p == p_itc_p)
	{
		m_itc_p = p_itc_p;

		if (m_q_info_p != NULL)
		{
			disconnect();

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 12,
				CString(_TEXT("C_itwq::initialise: re-initialisation not allowed")),
				CString(thread_name_tca),
				G01_SE_RAISED);
		}
		else
		{
			ret_val = init(thread_name_tca, pc_dwTimeout);
		}
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/03/96
// Date Last Modified   : 25/03/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itwq::init(
				const tc	*pc_queue_name_tcp,
				const DWORD	pc_dwTimeout)
{
	T_g03_lock_status		lock_status;
	T_itq_info_wait_status	wait_status;
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_itq_q_status		ret_val;

	ret_val = Q_ERROR;

	if (m_itc_p != NULL)
	{
//
// obtain exclusive access to control object
//
		lock_status = m_itc_p->lock_itc(timeout);

		switch (lock_status)
		{
		case LOCKED:
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

				q_ok_z = TRUE;

				switch (m_q_info_p->queue_status())
				{
				case Q_UNINITIALISED:
					m_q_info_p->set_queue_status(Q_WAITING_FOR_READER);
					break;
				case Q_WAITING_FOR_READER:
					break;
				case Q_OK:
					break;
				case Q_ERROR:
					q_ok_z = FALSE;
					break;
				case Q_FREE:
					q_ok_z = FALSE;
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 13,
						CString(_TEXT("C_itwq::init: found queue in FREE state")),
						CString(pc_queue_name_tcp),
						G01_SE_RAISED);
					break;
				}

				if (!q_ok_z)
				{
//
// Release exclusive access to control object
//
					m_itc_p->unlock_itc();
				}
				else
				{
//
// Wait for the reader to attach to the queue. The lock on the ITC is
// released in order to do this
//
					wait_status = m_q_info_p->
									wait_for_reader(m_itc_p, timeout);

					switch (wait_status)
					{
					case ITQ_INFO_WAIT_OK:
						if (m_q_info_p->windows_queue() == FALSE)
						{
							tc read_q_name_tca[ITQ_QUEUE_NAME_LEN_M + 2];
							tc write_q_name_tca[ITQ_QUEUE_NAME_LEN_M + 2];

							_tcscpy(read_q_name_tca, pc_queue_name_tcp);
							_tcscat(read_q_name_tca, _TEXT("R"));
							_tcscpy(write_q_name_tca, pc_queue_name_tcp);
							_tcscat(write_q_name_tca, _TEXT("W"));

							m_read_semaphore_h = ::OpenSemaphore(
									SEMAPHORE_ALL_ACCESS,
									FALSE,	// no inheritance
									read_q_name_tca);

							DWORD dwReadSemError;

							dwReadSemError = GetLastError();

							m_write_semaphore_h = ::OpenSemaphore(
									SEMAPHORE_ALL_ACCESS,
									FALSE,	// no inheritance
									write_q_name_tca);

							DWORD dwWriteSemError;

							dwWriteSemError = GetLastError();

							if (m_read_semaphore_h == NULL ||
								m_write_semaphore_h == NULL)
							{
								q_ok_z = FALSE;

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

						if (q_ok_z)
						{
							ret_val = m_q_info_p->queue_status();
						}
						break;

					case ITQ_INFO_WAIT_TIMEOUT:
						q_ok_z = FALSE;
						ret_val = Q_INIT_TIMEOUT;
						break;

					default:
						q_ok_z = FALSE;
						break;
					} // switch on wait for reader status
				} // queue status OK
//
// If queue found but could not initialised correctly then unlink from
// the queue info record
//
				if (!q_ok_z)
				{
					m_itc_p->free_queue(m_q_info_p);
				}
			} // queue found
			else
			{
//
// Could not find or create queue
//
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITRQ_CODE_BASE_W + 14,
					CString(_TEXT("C_itwq::init: could not find queue")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);
//
// Release exclusive access to control object
//
				m_itc_p->unlock_itc();
			}
			break;

		case LOCK_TIMEOUT:
			ret_val = Q_INIT_TIMEOUT;
			break;

		default:
//
// Lock not obtained
//
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 15,
					CString(_TEXT("C_itwq::init: lock not obtained")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);

			ret_val = Q_ERROR;
			break;
		} // switch on lock status
	} // control object non-null
	else
	{
//
// Control object is NULL
//
		g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + ITWQ_CODE_BASE_W + 16,
					CString(_TEXT("C_itwq::init: control object is NULL")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);

		ret_val = Q_ERROR;
	} // control object non-null

	return ret_val;
} // init
