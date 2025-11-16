///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipq_msg_list.h       
// File Type   : C++ Header
// Description : Inter Process Queue Message List Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _IPQ_MSG_LIST_H
#define _IPQ_MSG_LIST_H

#include <g03_types.h>
#include <ipq_types.h>

class C_ipc;

struct S_ipq_msg_holder
{
	m				m_id_m;
	T_g03_memory_id	m_data_addr;
	m				m_msg_data_size_m;
	b				m_priority_b;
	m				m_next_idx_m;
	C_g00_app_name	m_sender;
};

class C_ipq_msg_list
{
public:
//
// No constructors / destructors as objects reside in shared memory
//

// Attributes
private:
	m	m_head_idx_m;
	m	m_tail_idx_m;

// Implementation
public:
	void	init();
	z add_msg(
		S_ipq_msg_holder	*p_holder_array_p,
		m					&p_free_list_idx_m,
		C_ipc				*p_ipc_p,
		const C_ipq_q_msg	&p_msg);
	z remove_msg(
		S_ipq_msg_holder	*p_holder_array_p,
		m					&p_free_list_idx_m,
		C_ipc				*p_ipc_p,
		C_ipq_q_msg			&p_msg);
};

#endif _IPQ_MSG_LIST_H