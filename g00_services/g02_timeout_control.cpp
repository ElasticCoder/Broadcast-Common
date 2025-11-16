///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_timeout_control.cpp
// File Type   : C++ Source
// Description : Timeout Control
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>
#include <g02_timeout_control.h>
#include <g02_datetime.h>
#include <g02_datetime_span.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_timeout_control constructor
// Author               : Duncan Olesen
// Date Created         : 29/03/96
// Date Last Modified   : 29/03/96
///////////////////////////////////////////////////////////////////////////////

C_g02_timeout_control::C_g02_timeout_control(DWORD p_dwTimeout)
: m_timeout()
{
	reset(p_dwTimeout);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_timeout_control constructor
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 17/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_timeout_control::C_g02_timeout_control(const C_g02_datetime &pc_datetime)
: m_timeout()
{
	reset(pc_datetime);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_timeout_control destructor
// Author               : Duncan Olesen
// Date Created         : 29/03/96
// Date Last Modified   : 29/03/96
///////////////////////////////////////////////////////////////////////////////

C_g02_timeout_control::~C_g02_timeout_control()
{
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : The amount of time left before timeout expires (ms)
// Author               : Duncan Olesen
// Date Created         : 29/03/96
// Date Last Modified   : 08/10/96
///////////////////////////////////////////////////////////////////////////////

DWORD C_g02_timeout_control::time_left() const
{
	DWORD dwRet;

	if (m_wait_forever_z)
	{
		dwRet = INFINITE;
	}
	else
	{
		C_g02_datetime_span	duration;

		duration = m_timeout - C_g02_datetime::now();

		if (duration.negative())
		{
			dwRet = 0;
		}
		else if (duration.days() > 0)
		{
//
// duration until timeout is longer than a day, so return special value
// to indicate that the timeout is incomplete
//
			dwRet = G02_MILLISECS_PER_DAY_M;
		}
		else
		{
			dwRet = duration.time();
		}
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Reset the timeout
// Author               : Duncan Olesen
// Date Created         : 02/04/96
// Date Last Modified   : 08/10/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_timeout_control::reset(DWORD p_dwTimeout)
{
	if (p_dwTimeout == INFINITE)
	{
		m_wait_forever_z = TRUE;
	}
	else
	{
		m_wait_forever_z = FALSE;
//
// Set timeout instant to now + p_dwTimeout milliseconds
//
		m_timeout = C_g02_datetime::now() + m(p_dwTimeout);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Reset the timeout
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 08/10/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_timeout_control::reset(const C_g02_datetime &pc_datetime)
{
//
// This timeout can not be forever
//
	m_wait_forever_z	= FALSE;
	m_timeout			= pc_datetime;
}
