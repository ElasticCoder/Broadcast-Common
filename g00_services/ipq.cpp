///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipq.cpp
// File Type   : C++ Source
// Description : Inter Process Communications Queue Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <ipq.h>
#include "ipq_info.h"
#include "comms_event_codes.h"
#include <g01_event_logging.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipq constructor
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipq::C_ipq()
{
	m_ipc_p				= NULL;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;
} // C_ipq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipq constructor
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipq::C_ipq(C_ipc *p_ipc_p)
{
	m_ipc_p				= p_ipc_p;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;
} // C_ipq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipq destructor
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

C_ipq::~C_ipq()
{
	disconnect();
} // ~C_ipq

///////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate inter thread mesage data
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

void *C_ipq::alloc_msg_data(m p_msg_size_m)
{
	void *ret_val_p;

	if (m_ipc_p == (C_ipc *) NULL)
	{
		ret_val_p = (void *) NULL;
	}
	else
	{
		ret_val_p = m_ipc_p->alloc_msg_data(p_msg_size_m);
	}

	return ret_val_p;
} // alloc_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Release memory for inter thread message
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq::free_msg_data(void *p_msg_p)
{
	if (m_ipc_p != (C_ipc *) NULL)
	{
		m_ipc_p->free_msg_data(p_msg_p);
	}
} // free_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return queue status
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

T_ipq_q_status C_ipq::queue_status() const
{
	T_ipq_q_status ret_val;

	if (m_q_info_p == NULL)
	{
		ret_val = IPQ_ERROR;
	}
	else
	{
		ret_val = m_q_info_p->queue_status();
	}

	return ret_val;
} // queue_status


///////////////////////////////////////////////////////////////////////////////
// Function Description : Returns the name of the queue
// Author               : Duncan Olesen
// Date Created         : 27/11/96
// Date Last Modified   : 27/11/96
///////////////////////////////////////////////////////////////////////////////

void C_ipq::queue_name(tc *p_queue_name_tcp) const
{
	if (p_queue_name_tcp == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_COMMS_CODE_BASE_W + ITQ_CODE_BASE_W + 1,
			CString(_TEXT("C_itq::queue_name")),
			CString(_TEXT("pc_queue_name_tcp == NULL")),
			G01_SE_RAISED);
	}
	else if (m_q_info_p != NULL)
	{
		_tcscpy(p_queue_name_tcp, m_q_info_p->m_queue_name_tca);
	}
	else
	{
		p_queue_name_tcp[0] = _TEXT('\0');
	}
} // queue_name


///////////////////////////////////////////////////////////////////////////////
// Function Description : Disconnect from queue
// Author               : Duncan Olesen
// Date Created         : 16/04/97
// Date Last Modified   : 16/04/97
///////////////////////////////////////////////////////////////////////////////

void C_ipq::disconnect()
{
	if (m_ipc_p != NULL)
	{
		m_ipc_p->free_queue(m_q_info_p);
	}

	if (m_read_semaphore_h != NULL)
	{
		CloseHandle(m_read_semaphore_h);
		m_read_semaphore_h = NULL;
	}

	if (m_write_semaphore_h != NULL)
	{
		CloseHandle(m_write_semaphore_h);
		m_write_semaphore_h = NULL;
	}

	m_ipc_p		= NULL;
	m_q_info_p	= NULL;
} // disconnect


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipq copy constructor.
//						: Is private and should not be used
// Author               : Duncan Olesen
// Date Created         : 15/08/96
// Date Last Modified   : 15/08/96
///////////////////////////////////////////////////////////////////////////////

C_ipq::C_ipq(const C_ipq &p_queue)
{
	UNREFERENCED_PARAMETER(p_queue);	// TODO: Why is this parameter not referenced?

	m_ipc_p				= NULL;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;

	g01_log_system_event(
		G01_SE_SOFTWARE_EXCEPTION,
		G01_SE_ERROR,
		G01_SE_COMMS_CODE_BASE_W + IPQ_CODE_BASE_W + 1,
		CString(_TEXT("C_ipq::C_ipq: copy constructor should not be used")),
		CString(_TEXT("")),
		G01_SE_RAISED);
} // C_ipq copy constructor


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ipq assignment operator
//						: Is private and should not be used
// Author               : Duncan Olesen
// Date Created         : 15/08/96
// Date Last Modified   : 15/08/96
///////////////////////////////////////////////////////////////////////////////

C_ipq &C_ipq::operator=(const C_ipq &p_queue)
{
	UNREFERENCED_PARAMETER(p_queue);	// TODO: Why is this parameter not referenced?

	g01_log_system_event(
		G01_SE_SOFTWARE_EXCEPTION,
		G01_SE_ERROR,
		G01_SE_COMMS_CODE_BASE_W + IPQ_CODE_BASE_W + 2,
		CString(_TEXT("C_ipq::C_ipq: assignment operator should not be used")),
		CString(_TEXT("")),
		G01_SE_RAISED);

	return *this;
} // operator =