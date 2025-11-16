///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : ipq_info.h       
// File Type   : C++ Header
// Description : Inter Process Queue Info Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _IPQ_INFO_H
#define _IPQ_INFO_H

#include <ipq_types.h>
#include <g02_timeout_control.h>
#include <g03_types.h>
#include "ipq_msg_list.h"

enum T_ipq_info_get_status { IPQ_GET_OK, IPQ_GET_ERROR };
enum T_ipq_info_put_status { IPQ_PUT_OK, IPQ_PUT_ERROR };

enum T_ipq_info_wait_status
{
	IPQ_INFO_WAIT_OK,
	IPQ_INFO_WAIT_TIMEOUT,
	IPQ_INFO_WAIT_ERROR
};


class C_ipc;

class C_ipq_info
{
public:
//
// No constructors / destructors as objects reside in shared memory
//
//
// Attributes
//
public:
	m					m_next_info_idx_m;
	tc					m_queue_name_tca[IPQ_QUEUE_NAME_LEN_M + 1];
	T_ipq_q_status		m_queue_status;
	z					m_reader_connected_z;
	z					m_writer_waiting_z;
	m					m_usage_count_m;
private:
//
// Pointer to array of message holders
//
	T_g03_memory_id		m_msg_holders;
	m					m_free_list_idx_m;
//
// High and low priority message lists
//
	C_ipq_msg_list		m_high_list;
	C_ipq_msg_list		m_low_list;
//
// Implementation
//
public:
//
// Pseudo-constructor provided to initialise
// memory already reserved for the object
//
	void					init();
//
// Get the next message with the highest priority
//
	T_ipq_info_get_status	get_msg(
							C_ipc		*p_ipc_p,
							C_ipq_q_msg &p_msg);
//
// Put a message at the tail of the list for the appropriate priority
//
	T_ipq_info_put_status	put_msg(
							C_ipc		*p_ipc_p,
							const C_ipq_q_msg &p_msg);
//
// Clean queue info structure for unused status
//
	void					destroy_queue(C_ipc *p_ipc_p);
//
// Set up a queue info structure for use
//
	void					create_queue(C_ipc	*p_ipc_p);
//
// Wait for the reader to configure the queue
//
	T_ipq_info_wait_status	wait_for_reader(
								C_ipc						*p_ipc_p,
								const C_g02_timeout_control	&pc_timeout);
//
// Signal that the reader is connected to the queue
//
	void					reader_connected();
//
// Signal that the reader is no longer connected to the queue
//
	void					reader_disconnected();
//
// Queue status
//
	inline T_ipq_q_status
						queue_status() const
							{ return m_queue_status; };
	inline void			set_queue_status(T_ipq_q_status p_status)
							{ m_queue_status = p_status; };
};

#endif _IPQ_INFO_H