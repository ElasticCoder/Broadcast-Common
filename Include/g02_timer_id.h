///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Service
// File Name   : g02_timer_id.h
// File Type   : C++ Header
// Description : Timer ID type
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////



#ifndef _G02_TIMER_ID_H
#define _G02_TIMER_ID_H

#include <G02.h>

class SERVICES_EXTERNAL T_g02_timer_id
{
public:
	T_g02_timer_id();
	~T_g02_timer_id();

	m		m_number_m;
	void	*m_timer_info_p;

// Implementation
public:
	BOOL	operator==(const T_g02_timer_id	&p_rhs) const;
	BOOL	operator!=(const T_g02_timer_id	&p_rhs) const;
};


const T_g02_timer_id	G02_INVALID_TIMER_ID = T_g02_timer_id();


#endif _G02_TIMER_ID_H