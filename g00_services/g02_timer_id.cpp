///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_timer.cpp
// File Type   : C++ Source
// Description : Timer ID type
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions

#include <g02_timer.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : T_g02_timer_id constructor
// Author               : Duncan Olesen
// Date Created         : 19/03/97
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id::T_g02_timer_id()
{
	m_number_m = 0;
	m_timer_info_p = NULL;
} // T_g02_timer_id


///////////////////////////////////////////////////////////////////////////////
// Function Description : T_g02_timer_id destructor
// Author               : Duncan Olesen
// Date Created         : 19/03/97
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

T_g02_timer_id::~T_g02_timer_id()
{
} // ~T_g02_timer_id


///////////////////////////////////////////////////////////////////////////////
// Function Description : T_g02_timer_id equality operator
// Author               : Duncan Olesen
// Date Created         : 19/03/97
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

BOOL	T_g02_timer_id::operator==(const T_g02_timer_id	&p_rhs) const
{
	BOOL	ret_val;

	if (m_number_m == p_rhs.m_number_m &&
		m_timer_info_p == p_rhs.m_timer_info_p)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator==


///////////////////////////////////////////////////////////////////////////////
// Function Description : T_g02_timer_id inequality operator
// Author               : Duncan Olesen
// Date Created         : 19/03/97
// Date Last Modified   : 19/03/97
///////////////////////////////////////////////////////////////////////////////

BOOL	T_g02_timer_id::operator!=(const T_g02_timer_id	&p_rhs) const
{
	BOOL	ret_val;

	if (m_number_m != p_rhs.m_number_m ||
		m_timer_info_p != p_rhs.m_timer_info_p)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator!=
