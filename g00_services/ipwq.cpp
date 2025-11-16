///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipwq.cpp
// File Type   : C++ Source
// Description : Inter Process Communications Write Queue Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <ipwq.h>
#include <g02_timeout_control.h>

#include <g01_event_logging.h>
#include "ipq_info.h"
#include "comms_event_codes.h"

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipwq::C_ipwq()
: C_ipq()
{
} // C_ipwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itrq constructor not initialising object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipwq::C_ipwq(C_ipc *pc_ipc_p)
: C_ipq(pc_ipc_p)
{
} // C_ipwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipwq constructor initialising object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipwq::C_ipwq(const tc	*pc_queue_name_tcp,
			   C_ipc	*pc_ipc_p)
: C_ipq(pc_ipc_p)
{
	init(pc_queue_name_tcp, INFINITE);
} // C_ipwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipwq constructor initialising object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipwq::C_ipwq(
			T_g00_app_type	p_app_type,
			w				p_app_inst_w,
			C_ipc	*pc_ipc_p)
: C_ipq(pc_ipc_p)
{
	C_g00_app_name	app_name;
	tc				app_name_tca[G00_MAX_APP_NAME_LEN_W + 1];

//
// Build the name of the desired application
//
	app_name.name_app(
		p_app_type,
		p_app_inst_w);

	app_name.app_name(app_name_tca);

	(void) init(app_name_tca, INFINITE);
} // C_ipwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_iwrq destructor
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipwq::~C_ipwq()
{
	disconnect();
} // ~C_ipwq

///////////////////////////////////////////////////////////////////////////////
// Function Description : Writes a message to the queue
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipwq_write_status C_ipwq::write(
		C_ipq_q_msg	p_msg,
		const DWORD	pc_dwTimeout)
{
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_g03_lock_status		lock_status;
	T_ipq_info_put_status	put_status;
	T_ipwq_write_status		ret_val;
	DWORD					dwWaitStatus;
	w						event_code_w = 0;

	ret_val = IPQ_WQ_ERROR;
//
// Set the sender field in the message to the current (calling) application
//
	g00_app_name(p_msg.m_sender);

	if (m_ipc_p != NULL &&
		m_q_info_p != NULL &&
		m_q_info_p->queue_status() == IPQ_OK)
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
			lock_status = m_ipc_p->lock_ipc(timeout);

			switch (lock_status)
			{
			case LOCKED:
				put_status = m_q_info_p->put_msg(m_ipc_p, p_msg);

				if (put_status == IPQ_PUT_OK)
				{
//
// Update count of messages on queue for the reader
//
					(void) ReleaseSemaphore(
								m_read_semaphore_h,
								1,
								NULL);

					ret_val = IPQ_WQ_OK;
				}
				else
				{
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 1,
						CString(_TEXT("C_ipwq::write: failure to put message")),
						CString(_TEXT("")),
						G01_SE_RAISED);
				}

				m_ipc_p->unlock_ipc();
				break;
			case LOCK_TIMEOUT:
				ret_val = IPQ_WQ_TIMEOUT;
				break;
			case LOCK_ERROR:
				ret_val = IPQ_WQ_ERROR;
				event_code_w = 2;
				break;
			default:
				ret_val = IPQ_WQ_ERROR;
				event_code_w = 3;
				break;
			} // switch on lock status

			if (ret_val != IPQ_WQ_OK)
			{
//
// Write not successful, so release write semaphore
//
				(void) ReleaseSemaphore(
							m_write_semaphore_h,
							1,
							NULL);
			}

			if (ret_val == IPQ_WQ_ERROR)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					static_cast<w>(G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + event_code_w),
					CString(_TEXT("C_ipwq::write: cannot obtain lock")),
					CString(_TEXT("")),
					G01_SE_RAISED);
			}
			break;
		case WAIT_TIMEOUT:
			ret_val = IPQ_WQ_TIMEOUT;
			break;
		default:
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 4,
				CString(_TEXT("C_ipwq::write: cannot obtain semaphore")),
				CString(_TEXT("")),
				G01_SE_RAISED);

			ret_val = IPQ_WQ_ERROR;
			break;
		} // switch on wait for write semaphore
	} // queue in OK state
	else
	{
		CString temp;

		if (m_ipc_p == NULL)
		{
			temp.Format(_TEXT("m_ipc_p == NULL"));
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
			G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 5,
			CString(_TEXT("C_ipwq::write: queue in invalid state")),
			temp,
			G01_SE_RAISED);
	}

	if (ret_val != IPQ_WQ_OK)
	{
//
// If write is unsuccessful then free the any buffer allocated
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

T_ipwq_write_status C_ipwq::write(
			C_ipq_q_msg	p_msg,
			const void	*p_msg_data_p,
			m			p_msg_data_size_m,
			const DWORD	pc_dwTimeout)
{
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_ipwq_write_status		ret_val;

	ret_val = IPQ_WQ_ERROR;
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
				G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 6,
				CString(_TEXT("C_ipwq::write: p_msg_data_p is NULL")),
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
				G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 7,
				CString(_TEXT("C_ipwq::write: p_msg_data_p is not NULL")),
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
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_q_status C_ipwq::initialise(
				const tc	*pc_queue_name_tcp,
				const DWORD	pc_dwTimeout)
{
	if (m_q_info_p != NULL)
	{
		disconnect();

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 8,
			CString(_TEXT("C_ipwq::initialise: re-initialisation not allowed")),
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

T_ipq_q_status C_ipwq::initialise(
			C_ipc		*p_ipc_p,
			const tc	*pc_queue_name_tcp,
			const DWORD	pc_dwTimeout)
{
	T_ipq_q_status	ret_val;

	ret_val = IPQ_ERROR;

	if (m_ipc_p == NULL || m_ipc_p == p_ipc_p)
	{
		m_ipc_p = p_ipc_p;

		if (m_q_info_p != NULL)
		{
			disconnect();

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 9,
				CString(_TEXT("C_ipwq::initialise: re-initialisation not allowed")),
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
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_q_status C_ipwq::initialise(
			T_g00_app_type	p_app_type,
			w				p_app_inst_w,
			const DWORD		pc_dwTimeout)
{
	C_g00_app_name	app_name;
	tc				app_name_tca[G00_MAX_APP_NAME_LEN_W + 1];
	T_ipq_q_status	ret_val;

	ret_val = IPQ_ERROR;
//
// Build the name of the desired application
//
	app_name.name_app(
		p_app_type,
		p_app_inst_w);

	app_name.app_name(app_name_tca);

	if (m_q_info_p != NULL)
	{
		disconnect();

		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 10,
			CString(_TEXT("C_ipwq::initialise: re-initialisation not allowed")),
			CString(app_name_tca),
			G01_SE_RAISED);
	}
	else
	{
		ret_val = init(app_name_tca, pc_dwTimeout);
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_q_status C_ipwq::initialise(
			C_ipc			*p_ipc_p,
			T_g00_app_type	p_app_type,
			w				p_app_inst_w,
			const DWORD		pc_dwTimeout)
{
	C_g00_app_name	app_name;
	tc				app_name_tca[G00_MAX_APP_NAME_LEN_W + 1];
	T_ipq_q_status	ret_val;

	ret_val = IPQ_ERROR;
//
// Build the name of the desired application
//
	app_name.name_app(
		p_app_type,
		p_app_inst_w);

	app_name.app_name(app_name_tca);

	if (m_ipc_p == NULL || m_ipc_p == p_ipc_p)
	{
		m_ipc_p = p_ipc_p;

		if (m_q_info_p != NULL)
		{
			disconnect();

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 11,
				CString(_TEXT("C_ipwq::initialise: re-initialisation not allowed")),
				CString(app_name_tca),
				G01_SE_RAISED);
		}
		else
		{
			ret_val = init(app_name_tca, pc_dwTimeout);
		}
	}

	return ret_val;
} // initialise

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise queue
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_q_status C_ipwq::init(
				const tc	*pc_queue_name_tcp,
				const DWORD	pc_dwTimeout)
{
	T_g03_lock_status		lock_status;
	T_ipq_info_wait_status	wait_status;
	C_g02_timeout_control	timeout(pc_dwTimeout);
	T_ipq_q_status			ret_val;

	ret_val = IPQ_ERROR;

	if (m_ipc_p != NULL)
	{
//
// obtain exclusive access to control object
//
		lock_status = m_ipc_p->lock_ipc(timeout);

		switch (lock_status)
		{
		case LOCKED:
//
// find queue
//
			m_q_info_p = m_ipc_p->find_queue(pc_queue_name_tcp);
//
// if queue found then initialise it
//
			if (m_q_info_p != NULL)
			{
				z	q_ok_z;

				q_ok_z = TRUE;

				switch (m_q_info_p->queue_status())
				{
				case IPQ_UNINITIALISED:
					m_q_info_p->set_queue_status(IPQ_WAITING_FOR_READER);
					break;
				case IPQ_WAITING_FOR_READER:
					break;
				case IPQ_OK:
					break;
				case IPQ_ERROR:
					q_ok_z = FALSE;
					break;
				case IPQ_FREE:
					q_ok_z = FALSE;
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 12,
						CString(_TEXT("C_ipwq::init: found queue in FREE state")),
						CString(pc_queue_name_tcp),
						G01_SE_RAISED);
					break;
				}

				if (!q_ok_z)
				{
//
// Release exclusive access to control object
//
					m_ipc_p->unlock_ipc();
				}
				else
				{
//
// Wait for the reader to attach to the queue. The lock on the IPC is
// released in order to do this
//
					wait_status = m_q_info_p->
									wait_for_reader(m_ipc_p, timeout);

					switch (wait_status)
					{
					case IPQ_INFO_WAIT_OK:
						tc read_q_name_tca[IPQ_QUEUE_NAME_LEN_M + 2];
						tc write_q_name_tca[IPQ_QUEUE_NAME_LEN_M + 2];

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

						if (q_ok_z)
						{
							ret_val = m_q_info_p->queue_status();
						}
						break;

					case IPQ_INFO_WAIT_TIMEOUT:
						q_ok_z = FALSE;
						ret_val = IPQ_INIT_TIMEOUT;
						break;

					default:
						q_ok_z = FALSE;
						break;
					} // switch on wait for reader status
				} // queue status != error
//
// If queue found but could not initialised correctly then unlink from
// the queue info record
//
				if (!q_ok_z)
				{
					m_ipc_p->free_queue(m_q_info_p);
					m_q_info_p = NULL;
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
					G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 13,
					CString(_TEXT("C_ipwq::init: could not find queue")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);
//
// Release exclusive access to control object
//
				m_ipc_p->unlock_ipc();
				ret_val = IPQ_ERROR;
			}
			break;

		case LOCK_TIMEOUT:
			ret_val = IPQ_INIT_TIMEOUT;
			break;

		default:
//
// Lock not obtained
//
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 14,
					CString(_TEXT("C_ipwq::init: lock not obtained")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);

			ret_val = IPQ_ERROR;
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
					G01_SE_COMMS_CODE_BASE_W + IPWQ_CODE_BASE_W + 15,
					CString(_TEXT("C_ipwq::init: control object is NULL")),
					CString(pc_queue_name_tcp),
					G01_SE_RAISED);

		ret_val = IPQ_ERROR;
	} // control object non-null

	return ret_val;
} // init
