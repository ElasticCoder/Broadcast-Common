///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq_msg_list.h       
// File Type   : C++ Header
// Description : Inter Thread Queue Message List Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _ITQ_MSG_LIST_H
#define _ITQ_MSG_LIST_H

#include <itq_types.h>
#include <g02_timeout_control.h>

struct S_itq_q_msg_holder
{
	m			m_next_idx_m;
	C_itq_q_msg m_msg;
};

class C_itq_msg_list
{
public:
	C_itq_msg_list();
	~C_itq_msg_list();

	// Attributes
private:
	m	m_head_idx_m;
	m	m_tail_idx_m;

	// Implementation
public:
	z add_msg(
		S_itq_q_msg_holder *p_message_array_p,
		m					&p_free_list_idx_m,
		const C_itq_q_msg	&p_msg);
	z remove_msg(
		S_itq_q_msg_holder	*p_message_array_p,
		m					&p_free_list_idx_m,
		C_itq_q_msg			&p_msg);
};

#endif _ITQ_MSG_LIST_H
