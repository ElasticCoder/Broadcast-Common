///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipq_msg_list.cpp
// File Type   : C++ Source
// Description : Inter Process Queue Message List Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g03_types.h>

#include <ipc.h>
#include "ipq_msg_list.h"

#include <g01_event_logging.h>
#include "comms_event_codes.h"


///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise C_ipq_msg_list object
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq_msg_list::init()
{
	m_head_idx_m = NULL_INDEX_M;
	m_tail_idx_m = NULL_INDEX_M;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add message to list
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

z C_ipq_msg_list::add_msg(
			S_ipq_msg_holder	*p_message_array_p,
			m					&p_free_list_idx_m,
			C_ipc				*p_ipc_p,
			const C_ipq_q_msg	&p_msg)
{
//
// Add message to list.
// Return FALSE if no free message to add to list,
// Return TRUE if message successfully added to list
//
	z	ret_val;
	m	new_msg_idx_m;
	T_g03_memory_id	data_id;

	ret_val = FALSE;
//
// Validate message data address
//
	data_id = p_ipc_p->msg_data_id(p_msg.m_data_p);

	if (p_msg.m_data_p != NULL &&
		data_id == G03_INVALID_ID)
	{
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_COMMS_CODE_BASE_W + IPQ_MSG_LIST_CODE_BASE_W + 1,
				CString(_TEXT("C_ipq_msg_list::add_msg: invalid data")),
				CString(_TEXT("")),
				G01_SE_RAISED);
	}
	else if (p_free_list_idx_m != NULL_INDEX_M &&
			 p_message_array_p != NULL)
	{
//
// get a message holder from the free list
//
		new_msg_idx_m	  = p_free_list_idx_m;
		p_free_list_idx_m = p_message_array_p[new_msg_idx_m].m_next_idx_m;
//
// Put message holder at tail of queue
//
		if (m_tail_idx_m != NULL_INDEX_M)
		{
			p_message_array_p[m_tail_idx_m].m_next_idx_m = new_msg_idx_m;
		}
		else
		{
//
// Set head index if this is the only message in the list
//
			m_head_idx_m = new_msg_idx_m;
		}

		m_tail_idx_m = new_msg_idx_m;
//
// Setup message holder
//
		p_message_array_p[new_msg_idx_m].m_next_idx_m = NULL_INDEX_M;
		p_message_array_p[new_msg_idx_m].m_id_m = p_msg.m_id_m;
		p_message_array_p[new_msg_idx_m].m_data_addr = data_id;
		p_message_array_p[new_msg_idx_m].m_msg_data_size_m =
							p_msg.m_msg_data_size_m;
		p_message_array_p[new_msg_idx_m].m_priority_b =
							p_msg.m_priority_b;
		p_message_array_p[new_msg_idx_m].m_sender =
							p_msg.m_sender;

		ret_val = TRUE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove message from list
// Author               : Duncan Olesen
// Date Created         : 24/04/96
// Date Last Modified   : 24/04/96
///////////////////////////////////////////////////////////////////////////////

z C_ipq_msg_list::remove_msg(
			S_ipq_msg_holder	*p_message_array_p,
			m					&p_free_list_idx_m,
			C_ipc				*p_ipc_p,
			C_ipq_q_msg			&p_msg)
{
//
// Remove message from head of list.
// Return FALSE if no message in list,
// Return TRUE if message removed from list
//
	z	ret_val;
	m	msg_idx_m;

	ret_val = FALSE;

	if (m_head_idx_m != NULL_INDEX_M &&
		p_message_array_p != NULL)
	{
//
// Remove message from head of list
//
		msg_idx_m = m_head_idx_m;

		p_msg.m_id_m = p_message_array_p[msg_idx_m].m_id_m;

		p_msg.m_data_p = p_ipc_p->msg_data_addr(
						p_message_array_p[msg_idx_m].m_data_addr);

		p_msg.m_msg_data_size_m =
						p_message_array_p[msg_idx_m].m_msg_data_size_m;

		p_msg.m_priority_b = p_message_array_p[msg_idx_m].m_priority_b;

		p_msg.m_sender = p_message_array_p[msg_idx_m].m_sender;

		m_head_idx_m = p_message_array_p[msg_idx_m].m_next_idx_m;
//
// Update tail index if no messages left in list
//
		if (m_head_idx_m == NULL_INDEX_M)
		{
			m_tail_idx_m = NULL_INDEX_M;
		}
//
// Return message holder to the free list
//
		p_message_array_p[msg_idx_m].m_next_idx_m = p_free_list_idx_m;
		p_free_list_idx_m = msg_idx_m;

		ret_val = TRUE;
	}

	return ret_val;
};
