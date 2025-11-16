///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : timer_info.h       
// File Type   : C++ Header
// Description : Timer Info class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _TIMER_INFO_H
#define _TIMER_INFO_H

#include <G02.h>
#include <itwq.h>
#include <g02_datetime.h>

//
// Timer info class
//
class C_timer_info
{
public:
	C_timer_info();
	C_timer_info(
		const C_g02_datetime	&p_next_timeout,
		m						p_period_m,
		m						p_remaining_timeouts_m,
		const C_itq_q_msg		&p_timeout_msg,
		z						p_no_timeout_data_z
		);
	~C_timer_info();

public:
	C_itwq				m_write_queue;
	C_g02_datetime		m_next_timeout;
	m					m_period_m;
	m					m_remaining_timeouts_m;
	z					m_no_timeout_data_z;
	m					m_timer_number_m;

private:
	C_itq_q_msg			m_timeout_msg;
	C_timer_info		*m_prev_p;
	C_timer_info		*m_next_p;

// Implementation
public:
	inline C_timer_info	*next() const { return m_next_p; };
	//
	// Remove this object from a linked list.
	// This method relies on there being a dummy object at the
	// start of the list to enable the effective head of the queue
	// to be removed.
	//
	void	unlink(); // probably redundant, can be done by destructor
	//
	// Add this object into a linked list after a specified object
	//
	void	link(C_timer_info &p_previous_p);
	//
	// Send a timeout message
	// This updates the number of remaining timeouts and adjusts
	// the time at which the next timeout is due
	//
	T_itwq_write_status send_timeout();
};

#endif _TIMER_INFO_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////

