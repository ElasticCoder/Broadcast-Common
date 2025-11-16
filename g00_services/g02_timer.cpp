///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_timer.cpp
// File Type   : C++ Source
// Description : Timer Service
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itc.h>
#include <itwq.h>
#include <g01_event_logging.h>
#include <memory.h>

#include <g02_timer.h>

#include "timer_thread.h"
#include "timer_info.h"

static C_timer_config	v_timer_config;
static C_itwq			*v_timer_queue_p;
static CRITICAL_SECTION	v_critical_section;
static z				v_critical_section_init_z = FALSE;
static m				v_next_timer_num_m = 0;

z				validate_rqt(
						C_itq_q_msg		p_rqt_msg,
						C_itq_q_msg		&p_msg,
						z				p_no_timeout_data_z);
T_g02_timer_id	send_rqt(
						C_g02_datetime	p_initial_timeout,
						m				p_period_m,
						m				p_num_of_timeouts_m,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z);

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise Timer Service
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 03/06/96
///////////////////////////////////////////////////////////////////////////////

z g02_init_timer(
						C_itc	*p_itc_p,
						m		p_queue_size_m,
						DWORD	p_dwTimeout)
{
	CWinThread		*timer_thread;
	T_itq_q_status	queue_status;
//
// Initialise critical section variable if not already done so
//
	if (!v_critical_section_init_z)
	{
		InitializeCriticalSection(&v_critical_section);
		v_critical_section_init_z = TRUE;
	}
//
// Enter critical section to initialise timer service
//
	EnterCriticalSection(&v_critical_section);
//
// Ensure timer is not initialised more than once
//
	if (!v_timer_config.m_initialised_z)
	{
//
// Validate parameters
//
		if (p_itc_p != NULL && p_queue_size_m > 0)
		{
			v_timer_config.m_itc_p			= p_itc_p;
			v_timer_config.m_queue_size_m	= p_queue_size_m;
//
// Spawn the timer thread
//
			timer_thread = AfxBeginThread(g02_timer_main,
					   (LPVOID) &v_timer_config,
					   THREAD_PRIORITY_HIGHEST,
					   0, 
					   0, 
					   NULL );
//
// Attach to the timer thread's message queue
//
			if (timer_thread != NULL && timer_thread->m_hThread != NULL)
			{
				TRACE(_T("Timer thread id = 0x%x\n"), timer_thread->m_nThreadID);

				v_timer_queue_p = new C_itwq;

				if (v_timer_queue_p != NULL)
				{
					queue_status = v_timer_queue_p->initialise(
									p_itc_p,
									TIMER_THREAD_NUM_W,
									1,
									p_dwTimeout);

					if (queue_status == Q_OK)
					{
						v_timer_config.m_initialised_z = TRUE;
					}
					else
					{
						delete v_timer_queue_p;
						v_timer_queue_p = NULL;
					}
				}
			}
			else
			{
//
// Failed to create thread
//
				CString temp;

				temp.Format(_TEXT("GetLastError = %ld"),
								GetLastError());

				g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_G02_CODE_BASE_W + TIMER_CODE_BASE_W + 1,
						CString(_TEXT("g02_start_timer: failed to create thread")),
						temp,
						G01_SE_RAISED);
			}
		}
	}
//
// Leave critical section
//
	LeaveCriticalSection(&v_critical_section);

	return v_timer_config.m_initialised_z;
} // g02_init_timer

///////////////////////////////////////////////////////////////////////////////
// Function Description : Stop Timer Service
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 17/04/96
///////////////////////////////////////////////////////////////////////////////

void g02_stop_timer()
{
	T_itwq_write_status	write_status;
	C_itq_q_msg			msg;
//
// Enter critical section to stop timer service
//
	EnterCriticalSection(&v_critical_section);

	if (v_timer_config.m_initialised_z)
	{
		msg.m_id_m   = M_G02_STOP_THREAD_M;

		write_status = v_timer_queue_p->write(msg);

		if (write_status != WQ_OK)
		{
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G02_CODE_BASE_W + TIMER_CODE_BASE_W + 2,
					CString(_TEXT("g02_stop_timer: queue write failure")),
					CString(_TEXT("")),
					G01_SE_RAISED);
		}

		delete v_timer_queue_p;
		v_timer_queue_p = NULL;

		v_timer_config.m_initialised_z = FALSE;
	}
//
// Leave critical section
//
	LeaveCriticalSection(&v_critical_section);
} // g02_stop_timer

///////////////////////////////////////////////////////////////////////////////
// Function Description : Request Periodic (Multiple) Timeout Message relative
//						: to now
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id g02_rqt_multi_timeout(
						m				p_initial_timeout_m,
						m				p_period_m,
						m				p_num_of_timeouts_m,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z)
{
	return g02_rqt_multi_timeout(
						C_g02_datetime::now() + p_initial_timeout_m,
						p_period_m,
						p_num_of_timeouts_m,
						p_timeout_msg,
						p_dest_thread_num_w,
						p_dest_thread_inst_w,
						p_no_timeout_data_z);
} // g02_rqt_multi_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Request Periodic (Multiple) Timeout Message with
//						: absolute start
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id g02_rqt_multi_timeout(
						const C_g02_datetime
										&p_initial_timeout,
						m				p_period_m,
						m				p_num_of_timeouts_m,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z)
{
//
// The buffer holding timeout message data will be released
//
	T_g02_timer_id	ret_val;

	ret_val = G02_INVALID_TIMER_ID;

	if (v_timer_config.m_initialised_z)
	{
		z			request_valid_z;
		C_itq_q_msg	timeout_msg;

		request_valid_z = validate_rqt(
							p_timeout_msg,
							timeout_msg,
							p_no_timeout_data_z);

		if (request_valid_z)
		{
//
// Send request to timer thread
//
			ret_val = send_rqt(
						p_initial_timeout,
						p_period_m,
						p_num_of_timeouts_m,
						timeout_msg,
						p_dest_thread_num_w,
						p_dest_thread_inst_w,
						p_no_timeout_data_z);
		} // request valid
	} // timer uninitialised
//
// Free the memory holding the requested timeout message data
//
	if (p_timeout_msg.m_data_p != NULL)
	{
		v_timer_queue_p->free_msg_data(p_timeout_msg.m_data_p);
	}

	return ret_val;
} // g02_rqt_multi_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Request Single Timeout Message relative to now
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id g02_rqt_single_timeout(
						m				p_initial_timeout_m,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z)
{
	return g02_rqt_multi_timeout(
						C_g02_datetime::now() + p_initial_timeout_m,
						m(0),
						m(1),
						p_timeout_msg,
						p_dest_thread_num_w,
						p_dest_thread_inst_w,
						p_no_timeout_data_z);
} // g02_rqt_single_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Request Single Timeout Message with absolute start
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id g02_rqt_single_timeout(
						const C_g02_datetime
										&p_initial_timeout,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z)
{
	return g02_rqt_multi_timeout(
						p_initial_timeout,
						m(0),
						m(1),
						p_timeout_msg,
						p_dest_thread_num_w,
						p_dest_thread_inst_w,
						p_no_timeout_data_z);
} // g02_rqt_single_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Cancel Timeout
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 15/04/96
///////////////////////////////////////////////////////////////////////////////

void g02_cancel_timeout(T_g02_timer_id	p_timer_id)
{
	if (p_timer_id != G02_INVALID_TIMER_ID &&
		v_timer_config.m_initialised_z)
	{
		S_cancel_timer	*msg_p;
//
// Allocate buffer for cancel timer message
//
		msg_p = (S_cancel_timer *) v_timer_queue_p->alloc_msg_data(
									sizeof(S_cancel_timer));

		if (msg_p != NULL)
		{
			T_itwq_write_status	write_status;
			C_itq_q_msg			Q_msg;

			msg_p->m_id = p_timer_id;
//
// Send cancel timer message to timer thread
//
			Q_msg.m_id_m			= M_G02_CANCEL_TIMER_M;
			Q_msg.m_data_p			= (void *) msg_p;
			Q_msg.m_msg_data_size_m = sizeof(S_cancel_timer);

			write_status = v_timer_queue_p->write(Q_msg);

			if (write_status != WQ_OK)
			{
				g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_G02_CODE_BASE_W + TIMER_CODE_BASE_W + 3,
						CString(_TEXT("g02_cancel_timeout: cancel timer queue write failure")),
						CString(_TEXT("")),
						G01_SE_RAISED);

				v_timer_queue_p->free_msg_data(msg_p);
			}
		}
	}
} // g02_cancel_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Cancel All Timeouts or All Timeouts excluding
//						: infinite timeouts
// Author               : Duncan Olesen
// Date Created         : 10/02/97
// Date Last Modified   : 10/02/97
///////////////////////////////////////////////////////////////////////////////

void g02_cancel_all_timeouts(z	p_exclude_infinite_z)
{
	if (v_timer_config.m_initialised_z)
	{
		S_cancel_all_timers	*msg_p;
//
// Allocate buffer for cancel all timers message
//
		msg_p = (S_cancel_all_timers *) v_timer_queue_p->alloc_msg_data(
									sizeof(S_cancel_all_timers));

		if (msg_p != NULL)
		{
			T_itwq_write_status	write_status;
			C_itq_q_msg			Q_msg;

			msg_p->m_exclude_infinite_z = p_exclude_infinite_z;
//
// Send cancel all timers message to timer thread
//
			Q_msg.m_id_m			= M_G02_CANCEL_ALL_TIMERS_M;
			Q_msg.m_data_p			= (void *) msg_p;
			Q_msg.m_msg_data_size_m = sizeof(S_cancel_all_timers);

			write_status = v_timer_queue_p->write(Q_msg);

			if (write_status != WQ_OK)
			{
				g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_G02_CODE_BASE_W + TIMER_CODE_BASE_W + 4,
						CString(_TEXT("g02_cancel_all_timeouts")),
						CString(_TEXT("write failure")),
						G01_SE_RAISED);

				v_timer_queue_p->free_msg_data(msg_p);
			}
		}
	}
} // g02_cancel_all_timeouts

///////////////////////////////////////////////////////////////////////////////
// Function Description : Validate a timer request and creates a copy of the
//						: queue message to be used subsequently in the timeout
//						: message sent to the requester
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 12/03/97
///////////////////////////////////////////////////////////////////////////////

z	validate_rqt(
				C_itq_q_msg		p_rqt_msg,
				C_itq_q_msg		&p_msg,
				z				p_no_timeout_data_z)
{
	z	ret_val_z;

	ret_val_z = FALSE;

	p_msg.m_id_m = p_rqt_msg.m_id_m;
//
// Allocate memory for copy of requested timeout message data
//
	if (p_rqt_msg.m_data_p != NULL)
	{
		if (p_rqt_msg.m_msg_data_size_m != 0 &&
			p_no_timeout_data_z == FALSE)
		{
			p_msg.m_data_p = malloc(p_rqt_msg.m_msg_data_size_m);

			if (p_msg.m_data_p != NULL)
			{
//
// Copy request data into allocated buffer
//
				(void) memcpy(
							p_msg.m_data_p,
							p_rqt_msg.m_data_p,
							(size_t) p_rqt_msg.m_msg_data_size_m);

				p_msg.m_msg_data_size_m = 
							p_rqt_msg.m_msg_data_size_m;

				ret_val_z = TRUE;
			} // buffer allocated
		} // data size non-zero
	} // data in request timeout message
	else
	{
//
// No data in requested timeout message
//
		p_msg.m_data_p = NULL;
		p_msg.m_msg_data_size_m = 0;

		ret_val_z = TRUE;
	} // no data in request timeout message

	return ret_val_z;
}


///////////////////////////////////////////////////////////////////////////////
// Function Description : Send timeout request to timer thread, returns the
//						: timer id allocated to the request
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 12/03/97
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id	send_rqt(
						C_g02_datetime	p_initial_timeout,
						m				p_period_m,
						m				p_num_of_timeouts_m,
						C_itq_q_msg		p_timeout_msg,
						w				p_dest_thread_num_w,
						w				p_dest_thread_inst_w,
						z				p_no_timeout_data_z)
{
	T_g02_timer_id	ret_val;
	C_timer_info	*timer_info_p;
	S_timer_rqt		timer_rqt;

	ret_val = G02_INVALID_TIMER_ID;

//
// Create a timer information object
//
	timer_info_p = new C_timer_info(
				p_initial_timeout,
				p_period_m,
				p_num_of_timeouts_m,
				p_timeout_msg,
				p_no_timeout_data_z);

	if (timer_info_p != NULL)
	{
		T_itwq_write_status	write_status;
		C_itq_q_msg			Q_msg;

		timer_rqt.m_timer_info_p = timer_info_p;
//
// Enter critical section to allocate timer number
//
		EnterCriticalSection(&v_critical_section);

		timer_info_p->m_timer_number_m = ++v_next_timer_num_m;
//
// Leave critical section
//
		LeaveCriticalSection(&v_critical_section);
//
// Set up name of the destination thread
//
		if (p_dest_thread_num_w == 0 &&
			p_dest_thread_inst_w == 0)
		{
//
// Destination unspecified so set to the requesting (current) thread
//
			g00_thread_name(timer_rqt.m_thread_name);
		}
		else
		{
			C_g00_app_name	app_name;

			g00_app_name(app_name);

			timer_rqt.m_thread_name.name_thread(
						app_name,
						p_dest_thread_num_w,
						p_dest_thread_inst_w);
		}
//
// Send request to timer thread
//
		Q_msg.m_id_m			= M_G02_TIMER_RQT_M;

		write_status = v_timer_queue_p->write(
								Q_msg,
								(void *) &timer_rqt,
								sizeof(timer_rqt));

		if (write_status != WQ_OK)
		{
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G02_CODE_BASE_W + TIMER_CODE_BASE_W + 5,
					CString(_TEXT("send_rqt: timer request queue write failure")),
					CString(_TEXT("")),
					G01_SE_RAISED);

			delete timer_info_p;
			timer_info_p = NULL;
		}
		else
		{
//
// Successfully sent request
//
			ret_val.m_number_m		= timer_info_p->m_timer_number_m;
			ret_val.m_timer_info_p	= timer_info_p;
		}
	} // created timer info object
//
// If unsuccessful free any timeout message data memory allocated
//
	if (ret_val == G02_INVALID_TIMER_ID &&
		p_timeout_msg.m_data_p != NULL)
	{
		free(p_timeout_msg.m_data_p);
	}

	return ret_val;
}

//////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////

