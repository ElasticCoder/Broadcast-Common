///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq.cpp
// File Type   : C++ Source
// Description : Inter Thread Communications Queue Class
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <itq.h>
#include "itq_info.h"
#include "comms_event_codes.h"
#include <g01_event_logging.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq constructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_itq::C_itq()
{
	m_itc_p				= NULL;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;
} // C_itq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq constructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_itq::C_itq(C_itc *p_itc_p)
{
	m_itc_p				= p_itc_p;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;
} // C_itq

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq destructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_itq::~C_itq()
{
	disconnect();
} // ~C_itq

///////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate inter thread mesage data
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

void *C_itq::alloc_msg_data(m p_msg_size_m)
{
	void *ret_val_p;

	if (m_itc_p == (C_itc *) NULL)
	{
		ret_val_p = (void *) NULL;
	}
	else
	{
		ret_val_p = m_itc_p->alloc_msg_data(p_msg_size_m);
	}

	return ret_val_p;
} // alloc_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Release memory for inter thread message
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

void C_itq::free_msg_data(void *p_msg_p)
{
	if (m_itc_p != (C_itc *) NULL)
	{
		m_itc_p->free_msg_data(p_msg_p);
	}
} // free_msg_data

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return queue status
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 31/05/96
///////////////////////////////////////////////////////////////////////////////

T_itq_q_status C_itq::queue_status() const
{
	T_itq_q_status ret_val;

	if (m_q_info_p == NULL)
	{
		ret_val = Q_ERROR;
	}
	else
	{
		ret_val = m_q_info_p->queue_status();
	}

	return ret_val;
} // queue_status

///////////////////////////////////////////////////////////////////////////////
// Function Description : Test if there are any messages on the queue
//						: Return FALSE if so
// Author               : Duncan Olesen
// Date Created         : 30/06/97
// Date Last Modified   : 30/06/97
///////////////////////////////////////////////////////////////////////////////

z C_itq::is_empty() const
{
	z				is_empty_z;
	DWORD			dwWaitStatus;

	is_empty_z = TRUE;

	dwWaitStatus = WaitForSingleObject(
								m_read_semaphore_h,
								0);

	if (dwWaitStatus == WAIT_OBJECT_0)
	{
		ReleaseSemaphore(m_read_semaphore_h,
						 (LONG) 1,
						 NULL);

		is_empty_z = FALSE;
	}

	return is_empty_z;
} // is_empty

///////////////////////////////////////////////////////////////////////////////
// Function Description : Returns the name of the queue
// Author               : Duncan Olesen
// Date Created         : 27/11/96
// Date Last Modified   : 27/11/96
///////////////////////////////////////////////////////////////////////////////

void C_itq::queue_name(tc *p_queue_name_tcp) const
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
// Date Created         : 15/04/97
// Date Last Modified   : 15/04/97
///////////////////////////////////////////////////////////////////////////////

void C_itq::disconnect()
{
	if (m_itc_p != NULL)
	{
		m_itc_p->free_queue(m_q_info_p);
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

	m_q_info_p	= NULL;
	m_itc_p		= NULL;
} // disconnect

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq copy constructor.
//						: Is private and should not be used
// Author               : Duncan Olesen
// Date Created         : 15/08/96
// Date Last Modified   : 15/08/96
///////////////////////////////////////////////////////////////////////////////

C_itq::C_itq(const C_itq &p_queue)
{
	UNREFERENCED_PARAMETER(p_queue);	// TODO: Why is this parameter not referenced?

	m_itc_p				= NULL;
	m_q_info_p			= NULL;
	m_read_semaphore_h	= NULL;
	m_write_semaphore_h	= NULL;

	g01_log_system_event(
		G01_SE_SOFTWARE_EXCEPTION,
		G01_SE_ERROR,
		G01_SE_COMMS_CODE_BASE_W + ITQ_CODE_BASE_W + 2,
		CString(_TEXT("C_itq::C_itq: copy constructor should not be used")),
		CString(_TEXT("")),
		G01_SE_RAISED);
} // C_itq copy constructor

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_itq assignment operator
//						: Is private and should not be used
// Author               : Duncan Olesen
// Date Created         : 15/08/96
// Date Last Modified   : 15/08/96
///////////////////////////////////////////////////////////////////////////////

C_itq &C_itq::operator=(const C_itq &p_queue)
{
	UNREFERENCED_PARAMETER(p_queue);	// TODO: Why is this parameter not referenced?

	g01_log_system_event(
		G01_SE_SOFTWARE_EXCEPTION,
		G01_SE_ERROR,
		G01_SE_COMMS_CODE_BASE_W + ITQ_CODE_BASE_W + 3,
		CString(_TEXT("C_itq::C_itq: assignment operator should not be used")),
		CString(_TEXT("")),
		G01_SE_RAISED);

	return *this;
} // operator =