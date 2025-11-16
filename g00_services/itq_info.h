///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq_info.h       
// File Type   : C++ Header
// Description : Inter Thread Queue Info Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _ITQ_INFO_H
#define _ITQ_INFO_H

#include <itq_types.h>
#include <g02_timeout_control.h>
#include "itq_msg_list.h"

enum T_itq_info_get_status { ITQ_GET_OK, ITQ_GET_ERROR };
enum T_itq_info_put_status { ITQ_PUT_OK, ITQ_PUT_ERROR };

enum T_itq_info_wait_status
{
	ITQ_INFO_WAIT_OK,
	ITQ_INFO_WAIT_TIMEOUT,
	ITQ_INFO_WAIT_ERROR
};

class C_itc;

class C_itq_info
{
public:
	C_itq_info();
	~C_itq_info();
//
// Attributes
//
public:
	m					m_next_info_idx_m;
	tc					m_queue_name_tca[ITQ_QUEUE_NAME_LEN_M + 1];
	T_itq_q_status		m_queue_status;
	m					m_queue_size_m;
	m					m_num_msgs_m;
	z					m_reader_connected_z;
	z					m_writer_waiting_z;
	m					m_usage_count_m;

private:
	S_itq_q_msg_holder	*m_message_p;
	m					m_free_list_idx_m;
	z					m_windows_queue_z;
	HWND				m_window_h; // Queue reader window handle (windows queue)
	HANDLE				m_event_h;

	C_itq_msg_list		m_high_list;
	C_itq_msg_list		m_low_list;
//
// Implementation
//
public:
	inline z				windows_queue() { return m_windows_queue_z; };
	inline HWND				window() { return m_window_h; };
	T_itq_info_get_status	get_msg(C_itq_q_msg &p_msg);
	T_itq_info_put_status	put_msg(const C_itq_q_msg &p_msg);
	void					destroy_queue(C_itc *p_itc_p);
	void					create_queue(
								z		p_windows_queue_z,
								HWND	p_window_h,
								m		p_queue_size_m);
	T_itq_info_wait_status	wait_for_reader(
								C_itc						*p_itc_p,
								const C_g02_timeout_control	&pc_timeout);
	void					reader_connected();
	void					reader_disconnected();
	inline T_itq_q_status
						queue_status() const
							{ return m_queue_status; };
	inline void			set_queue_status(T_itq_q_status p_status)
							{ m_queue_status = p_status; };
	void				destroy_queue();
};

#endif
