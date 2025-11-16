///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq_msg_list.cpp
// File Type   : C++ Source
// Description : Inter Thread Queue Message List Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include "itq_msg_list.h"

#include <g01_event_logging.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq_msg_list constructor
// Author               : Duncan Olesen
// Date Created         : 02/04/96
// Date Last Modified   : 02/04/96
///////////////////////////////////////////////////////////////////////////////

C_itq_msg_list::C_itq_msg_list()
{
	m_head_idx_m = NULL_INDEX_M;
	m_tail_idx_m = NULL_INDEX_M;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq_msg_list constructor
// Author               : Duncan Olesen
// Date Created         : 02/04/96
// Date Last Modified   : 02/04/96
///////////////////////////////////////////////////////////////////////////////

C_itq_msg_list::~C_itq_msg_list()
{
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add message to list
// Author               : Duncan Olesen
// Date Created         : 02/04/96
// Date Last Modified   : 02/04/96
///////////////////////////////////////////////////////////////////////////////

z C_itq_msg_list::add_msg(
			S_itq_q_msg_holder	*p_message_array_p,
			m					&p_free_list_idx_m,
			const C_itq_q_msg	&p_msg)
{
//
// Add message to list.
// Return FALSE if no free message to add to list,
// Return TRUE if message successfully added to list
//
	z	ret_val;
	m	new_msg_idx_m;

	ret_val = FALSE;

	if (p_free_list_idx_m != NULL_INDEX_M &&
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
		p_message_array_p[new_msg_idx_m].m_msg = p_msg;

		ret_val = TRUE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove message from list
// Author               : Duncan Olesen
// Date Created         : 02/04/96
// Date Last Modified   : 02/04/96
///////////////////////////////////////////////////////////////////////////////

z C_itq_msg_list::remove_msg(
			S_itq_q_msg_holder	*p_message_array_p,
			m					&p_free_list_idx_m,
			C_itq_q_msg			&p_msg)
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
		p_msg = p_message_array_p[msg_idx_m].m_msg;
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
