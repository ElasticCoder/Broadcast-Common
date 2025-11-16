///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_timer_thread.cpp
// File Type   : C++ Source
// Description : Timer Service thread
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itc.h>
#include <itrq.h>
#include <itwq.h>
#include <g00_process_registry.h>
#include <g01_event_logging.h>

#include <g02_timer.h>

#include "timer_thread.h"
#include "timer_info.h"

void start_timer(
			S_timer_rqt				&timer_rqt,
			C_timer_info			&p_timer_list_head,
			C_itc					*p_itc_p);

void cancel_timer(
			S_cancel_timer			&p_cancel_timer,
			const C_timer_info		&p_timer_list_head);

void send_timeout(C_timer_info		&p_timer_list_head);

C_timer_info *preceeding_timer(
				C_timer_info		&p_timer,
				C_timer_info		&p_timer_list_head);

void set_timeout(
			C_g02_timeout_control	&p_next_timeout,
			C_timer_info			&p_timer_list_head);

void cancel_all_timers(
			z					p_exclude_infinite_z,
			const C_timer_info	&p_timer_list_head);

///////////////////////////////////////////////////////////////////////////////
// Function Description : Timer Service thread entry point
// Author               : Duncan Olesen
// Date Created         : 15/04/96
// Date Last Modified   : 08/10/96
///////////////////////////////////////////////////////////////////////////////

UINT g02_timer_main(LPVOID p_config_p)
{
	UINT				ret_val = 0;
	C_timer_config		*timer_config;
	T_itq_q_status		queue_status;
	C_itrq				read_queue;
	C_timer_info		timer_list_head;

	timer_config = (C_timer_config *) p_config_p;
	queue_status = Q_ERROR;
//
// Register thread
//
	g00_register_thread(TIMER_THREAD_NUM_W, w(1));
//
// Create read queue if config provided
//
	if (p_config_p != NULL)
	{
		queue_status = read_queue.initialise(
							timer_config->m_itc_p,
							FALSE,
							NULL,
							timer_config->m_queue_size_m);
	}

	if (queue_status != Q_OK)
	{
//
// Unable to configure timer thread
//
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 1,
				CString(_TEXT("g02_timer_main: terminated abnormally")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		ret_val = 1;
	}
	else
	{
//
// Read queue is OK so loop to process messages
//
		T_itrq_read_status		read_status;
		C_itq_q_msg				msg;
		z						continue_z;
		C_g02_timeout_control	next_timeout(INFINITE);

		continue_z = TRUE;

		while (continue_z)
		{
			DWORD	dwTimeRemaining;
//
// Determine how long to wait on the queue
//
			set_timeout(next_timeout, timer_list_head);
			dwTimeRemaining = next_timeout.time_left();

			if (dwTimeRemaining > 0)
			{
//
// still time to go until next timeout is due so wait on queue
//
				read_status = read_queue.read(
								msg,
								dwTimeRemaining);
			}
			else
			{
//
// No time to wait for a message so simulate a read timeout
//
				read_status = RQ_TIMEOUT;
			}

			switch (read_status)
			{
			case RQ_OK:
				switch (msg.m_id_m)
				{
				case M_G02_TIMER_RQT_M:
//
// Timer request message
//
					{
					S_timer_rqt			*timer_rqt_p;

					timer_rqt_p = (S_timer_rqt *) msg.m_data_p;

					if (timer_rqt_p == NULL)
					{
//
// No timer request message data
//
						g01_log_system_event(
								G01_SE_SOFTWARE_EXCEPTION,
								G01_SE_ERROR,
								G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 2,
								CString(_TEXT("g02_timer_main: no timer rqt data")),
								CString(_TEXT("")),
								G01_SE_RAISED);
					}
					else
					{
//
// Start a timer
//
						start_timer(
								*timer_rqt_p,
								timer_list_head,
								timer_config->m_itc_p);
					}
					} // scope
					break;

				case M_G02_CANCEL_TIMER_M:
//
// Cancel a timer
//
					{
					S_cancel_timer	*cancel_timer_p;

					cancel_timer_p = (S_cancel_timer *) msg.m_data_p;

					if (cancel_timer_p == NULL)
					{
//
// No cancel timer message data
//
						g01_log_system_event(
								G01_SE_SOFTWARE_EXCEPTION,
								G01_SE_ERROR,
								G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 3,
								CString(_TEXT("g02_timer_main: no cancel timer rqt data")),
								CString(_TEXT("")),
								G01_SE_RAISED);
					}
					else
					{
//
// Cancel the timer
//
						cancel_timer(
								*cancel_timer_p,
								timer_list_head);
					}
					} // scope
					break;

				case M_G02_CANCEL_ALL_TIMERS_M:
//
// Cancel all timers
//
					{
					S_cancel_all_timers	*cancel_all_timers_p;

					cancel_all_timers_p = (S_cancel_all_timers *) msg.m_data_p;

					if (cancel_all_timers_p == NULL)
					{
//
// No cancel timer message data
//
						g01_log_system_event(
								G01_SE_SOFTWARE_EXCEPTION,
								G01_SE_ERROR,
								G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 3,
								CString(_TEXT("g02_timer_main")),
								CString(_TEXT("No cancel all timers rqt data")),
								G01_SE_RAISED);
					}
					else
					{
//
// Cancel all timers
//
						cancel_all_timers(
								cancel_all_timers_p->m_exclude_infinite_z,
								timer_list_head);
					}
					} // scope
					break;

				case M_G02_STOP_THREAD_M:
//
// Terminate the timer thread
//
					continue_z = FALSE;
					ret_val = 0;
					break;

				default:
//
// Unrecognised message received
//
					{
					CString temp;

					temp.Format(_TEXT("message id = %d"), msg.m_id_m);

					g01_log_system_event(
							G01_SE_SOFTWARE_EXCEPTION,
							G01_SE_ERROR,
							G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 4,
							CString(_TEXT("g02_timer_main: unrecognised message received")),
							temp,
							G01_SE_RAISED);
					}
					break;
				} // switch on message id
//
// Free any message data
//
				read_queue.free_msg_data(msg.m_data_p);
				msg.m_data_p = NULL;
				break;

			case RQ_TIMEOUT:
//
// Read timed out so it must be time to send the next timeout message,
// unless the timeout was greater than a day.
//
				if (dwTimeRemaining != G02_MILLISECS_PER_DAY_M)
				{
					send_timeout(timer_list_head);
				}
				break;

			default:
//
// Read error, terminate thread
//
				continue_z = FALSE;
				ret_val = 2;
				break;
			} // switch on read_status
		} // loop while continue_z
	}
//
// Clean up timers
//
	cancel_all_timers(FALSE, timer_list_head);
//
// Indicate that the timer thread is not running
//
	timer_config->m_initialised_z = FALSE;

	return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add a timer to the list and update the time until
//						: next timeout if necessary
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

void start_timer(
			S_timer_rqt				&p_timer_rqt,
			C_timer_info			&p_timer_list_head,
			C_itc					*p_itc_p)
{
//	C_g00_thread_name	m_thread_name;
	C_timer_info	*new_timer_p;
//
// Get address of timer info object from the message
//
	new_timer_p = (C_timer_info *) p_timer_rqt.m_timer_info_p;

	if (new_timer_p != NULL)
	{
		C_timer_info	*prev_timer_p;

		prev_timer_p = preceeding_timer(
							*new_timer_p,
							p_timer_list_head);
//
// Link timer into list
//
		new_timer_p->link(*prev_timer_p);
//
// Initialise the queue onto which timeout messages will be written
//
		w	thread_num_w;
		w	thread_inst_w;

		p_timer_rqt.m_thread_name.thread_name(
						thread_num_w,
						thread_inst_w);

		(void) new_timer_p->m_write_queue.initialise(
						p_itc_p,
						thread_num_w,
						thread_inst_w);
	}
	else
	{
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 5,
				CString(_TEXT("start_timer: timer info ptr is NULL")),
				CString(_TEXT("")),
				G01_SE_RAISED);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Cancel a timer, removing it from timer list and
//						: updating time until next timeout if necessary
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

void cancel_timer(
			S_cancel_timer			&p_cancel_timer,
			const C_timer_info		&p_timer_list_head)
{
	C_timer_info	*cancel_timer_info_p;
	m				cancel_timer_number_m;
	C_timer_info	*timer_p;
//
// Get address of timer info object from the message
//
	cancel_timer_info_p		= (C_timer_info *)
								p_cancel_timer.m_id.m_timer_info_p;
	cancel_timer_number_m	= p_cancel_timer.m_id.m_number_m;
//
// Search for the timer in the list, it may have been deleted already
//
	timer_p		 = p_timer_list_head.next();

	while (timer_p != NULL &&
		   (timer_p != cancel_timer_info_p ||
		    timer_p->m_timer_number_m != cancel_timer_number_m))
	{
		timer_p		 = timer_p->next();
	}

	if (timer_p != NULL)
	{
//
// Timer found delete it, this removes it from the list
//
		delete timer_p;
	}
} // cancel_timer

///////////////////////////////////////////////////////////////////////////////
// Function Description : Cancel all timers, possibly excluding infinite ones
// Author               : Duncan Olesen
// Date Created         : 10/02/97
// Date Last Modified   : 10/02/97
///////////////////////////////////////////////////////////////////////////////

void cancel_all_timers(
			z					p_exclude_infinite_z,
			const C_timer_info	&p_timer_list_head)
{
//
// Search for the timer in the list, it may have been deleted already
//
	C_timer_info	*timer_p;
	C_timer_info	*next_timer_p;

	timer_p		 = p_timer_list_head.next();

	while (timer_p != NULL)
	{
		next_timer_p	= timer_p->next();

		if (!p_exclude_infinite_z ||
			timer_p->m_remaining_timeouts_m != G02_INFINITE_TIMEOUTS_M)
		{
//
// Remove the timeout if it is not infinite or infinite timeouts are
// not excluded
//
			delete timer_p;
		}

		timer_p	= next_timer_p;
	} // loop through timers
} // cancel_all_timers

///////////////////////////////////////////////////////////////////////////////
// Function Description : Send the next timeout, adjust the position of the
//						: timer in the list if necessary and update the
//						: time until the next timeout
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

void send_timeout(C_timer_info		&p_timer_list_head)
{
	C_timer_info		*first_timer_p;
	T_itwq_write_status	write_status;
//
// Get First real timer in list
//
	first_timer_p = p_timer_list_head.next();

	write_status = first_timer_p->send_timeout();

	if (write_status != WQ_OK)
	{
		CString		temp;
//
// Failed to write timeout
//
		temp.Format(_TEXT("write status = %d"),
					(int) write_status);

		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G02_CODE_BASE_W + TIMER_THREAD_CODE_BASE_W + 6,
				CString(_TEXT("send_timeout: failure to send timeout")),
				temp,
				G01_SE_RAISED);
	}
//
// If no timeouts remaining for timer, delete the object, which
// removes it from the list
//
	if (first_timer_p->m_remaining_timeouts_m == 0)
	{
		delete first_timer_p;
	}
	else
	{
		C_timer_info	*prev_timer_p;
//
// Remove the timer temporarily from the list
//
		first_timer_p->unlink();
//
// Find the position of what is now the preceeding timer
//
		prev_timer_p = preceeding_timer(
							*first_timer_p,
							p_timer_list_head);
//
// Place the timer in the correct position
//
		first_timer_p->link(*prev_timer_p);
	}
} // send_timeout

///////////////////////////////////////////////////////////////////////////////
// Function Description : Find the timer in the list which is due before
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 16/07/96
///////////////////////////////////////////////////////////////////////////////

C_timer_info *preceeding_timer(
				C_timer_info	&p_timer,
				C_timer_info	&p_timer_list_head)
{
	C_timer_info	*timer_p;
	C_timer_info	*prev_timer_p;
	C_g02_datetime	timeout;
//
// Loop through list of timers to find position
//
	prev_timer_p = &p_timer_list_head;
	timer_p		 = p_timer_list_head.next();
	timeout		 = p_timer.m_next_timeout;
//
// Pass over timers with equal timeout to ensure that identical single 
// timeouts will be generated in the order they were requested
//
	while (timer_p != NULL &&
		   timeout >= timer_p->m_next_timeout)
	{
		prev_timer_p = timer_p;
		timer_p		 = timer_p->next();
	}

	return prev_timer_p;
} // preceeding_timer

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set next timeout to the timeout at the start of the
//						: list or inifinity if there are no timers
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

void set_timeout(
			C_g02_timeout_control	&p_next_timeout,
			C_timer_info			&p_timer_list_head)
{
	C_timer_info		*first_timer_p;

	first_timer_p = p_timer_list_head.next();
		
	if (first_timer_p == NULL)
	{
//
// no timers so reset next timeout to infinity
//
		p_next_timeout.reset(INFINITE);
	}
	else
	{
		p_next_timeout.reset(first_timer_p->m_next_timeout);
	}
} // set_timeout

