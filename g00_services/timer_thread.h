///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : timer_thread.h       
// File Type   : C++ Header
// Description : Timer Service Thread
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////



#ifndef _TIMER_THREAD_H
#define _TIMER_THREAD_H

#include <G02.h>
#include <Message ID Bases.h>
#include "timer_info.h"

UINT g02_timer_main( LPVOID pParam );
//
// Message IDs
//
const m M_G02_TIMER_RQT_M			= TIMER_THREAD + 1;
const m M_G02_CANCEL_TIMER_M		= TIMER_THREAD + 2;
const m M_G02_CANCEL_ALL_TIMERS_M	= TIMER_THREAD + 3;
const m M_G02_STOP_THREAD_M			= TIMER_THREAD + 4;
//
// Timer request message structure
//
struct S_timer_rqt
{
public:
	C_timer_info		*m_timer_info_p;
	C_g00_thread_name	m_thread_name;
};
//
// Cancel Timer message structure
//
struct S_cancel_timer
{
public:
	T_g02_timer_id		m_id;
};
//
// Cancel All Timers message structure
//
struct S_cancel_all_timers
{
public:
	z					m_exclude_infinite_z;
};
//
// Timer thread configuration data class
//
class C_timer_config
{
public:
	C_itc				*m_itc_p;
	m					m_queue_size_m;
	z					m_initialised_z;

	inline C_timer_config() { m_initialised_z = FALSE; }
	inline ~C_timer_config() {};
};

#endif _TIMER_THREAD_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////

