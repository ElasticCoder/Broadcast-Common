///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : timer_info.cpp
// File Type   : C++ Source
// Description : Timer Info class
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
#include <g02_timer.h>

#include <memory.h>

#include "timer_info.h"


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_timer_info constructor
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

C_timer_info::C_timer_info()
: m_next_timeout(), m_write_queue(), m_timeout_msg()
{
	m_period_m				= 0;
	m_remaining_timeouts_m	= 0;
	m_prev_p				= NULL;
	m_next_p				= NULL;
	m_no_timeout_data_z		= FALSE;
	m_timer_number_m		= 0;
} // C_timer_info

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_timer_info constructor
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

C_timer_info::C_timer_info(
		const C_g02_datetime	&p_next_timeout,
		m						p_period_m,
		m						p_remaining_timeouts_m,
		const C_itq_q_msg		&p_timeout_msg,
		z						p_no_timeout_data_z
		)
: m_next_timeout(p_next_timeout), m_write_queue(), m_timeout_msg(p_timeout_msg)
{
	m_period_m				= p_period_m;
	m_remaining_timeouts_m	= p_remaining_timeouts_m;
	m_prev_p				= NULL;
	m_next_p				= NULL;
	m_no_timeout_data_z		= p_no_timeout_data_z;
	m_timer_number_m		= 0;
} // C_timer_info

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_timer_info destructor
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

C_timer_info::~C_timer_info()
{
//
// Free the timeout message data
//
	if (m_timeout_msg.m_data_p != NULL)
	{
		free(m_timeout_msg.m_data_p);
	}
//
// Unlink the object from the linked list
//
	unlink();
} // ~C_timer_info

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove this object from a linked list.
//						: This method relies on there being a dummy object at
//						: the start of the list to enable the effective head of
//						: the queue to be removed.
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 19/06/96
///////////////////////////////////////////////////////////////////////////////

void C_timer_info::unlink()
{
//
// If this object has a precedent then adjust the precedent
//
	if (m_prev_p != NULL)
	{
		m_prev_p->m_next_p = m_next_p;
	}
//
// If this object has a follower then adjust the follower
//
	if (m_next_p != NULL)
	{
		m_next_p->m_prev_p = m_prev_p;
	}

	m_next_p = NULL;
	m_prev_p = NULL;
} // unlink

//////////////////////////////////////////////////////////////////////////////
// Function Description : Add this object into a linked list after a specified
//						: object
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 18/04/96
///////////////////////////////////////////////////////////////////////////////

void C_timer_info::link(C_timer_info &p_previous)
{
	m_next_p = p_previous.m_next_p;
	m_prev_p = &p_previous;

	p_previous.m_next_p = this;

	if (m_next_p != NULL)
	{
		m_next_p->m_prev_p = this;
	}
} // link

//////////////////////////////////////////////////////////////////////////////
// Function Description : Send a timeout message
//						: This updates the number of remaining timeouts and
//						: adjusts the time at which the next timeout is due
// Author               : Duncan Olesen
// Date Created         : 18/04/96
// Date Last Modified   : 12/03/97
///////////////////////////////////////////////////////////////////////////////

T_itwq_write_status C_timer_info::send_timeout()
{
	T_itwq_write_status	ret_val;
	C_itq_q_msg			Q_msg;
	T_g02_timeout_msg	timeout;

	ret_val = WQ_ERROR;

	if (m_remaining_timeouts_m > 0)
	{
//
// Ensure timeout information is updated even if the current
// timeout fails to be sent
//
		if (m_remaining_timeouts_m != G02_INFINITE_TIMEOUTS_M)
		{
			m_remaining_timeouts_m--;
		}

		m_next_timeout += m_period_m;

		z	error_z;

		error_z = FALSE;

		timeout.m_timer_id.m_timer_info_p = this;
		timeout.m_timer_id.m_number_m = m_timer_number_m;
//
// If there is any timeout message data then create a copy of it
//
		if (m_timeout_msg.m_data_p != NULL)
		{
			timeout.m_timeout_data_p = m_write_queue.alloc_msg_data(
							m_timeout_msg.m_msg_data_size_m);
			timeout.m_timeout_data_size_m =
							m_timeout_msg.m_msg_data_size_m;

			if (timeout.m_timeout_data_p == NULL)
			{
				error_z = TRUE;
			}
			else
			{
//
// Copy request data into allocated buffer
//
				(void) memcpy(
						timeout.m_timeout_data_p,
						m_timeout_msg.m_data_p,
						(size_t) m_timeout_msg.m_msg_data_size_m);
			}
		} // timeout message data
		else
		{
			timeout.m_timeout_data_p		= NULL;
			timeout.m_timeout_data_size_m	= 0;
		}

		if (!error_z)
		{
//
// Allocate buffer for requested timeout data, so send message
//
			Q_msg.m_id_m = m_timeout_msg.m_id_m;
			Q_msg.m_priority_b = m_timeout_msg.m_priority_b;

			if (m_no_timeout_data_z)
			{
				ret_val = m_write_queue.write(
								Q_msg,
								NULL,
								0);
			}
			else
			{
				ret_val = m_write_queue.write(
								Q_msg,
								(void *) &timeout,
								sizeof(timeout));
			}
		} // allocation error ?
//
// If no further timeouts then free any requested timeout data
//
		if (m_remaining_timeouts_m == 0 &&
			m_timeout_msg.m_data_p != NULL)
		{
			free(m_timeout_msg.m_data_p);
			m_timeout_msg.m_data_p = NULL;
		}
	} // timeouts remaining

	return ret_val;
} // send_timeout

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////

