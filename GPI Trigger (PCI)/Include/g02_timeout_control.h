///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : timeout_control.h       
// File Type   : C++ Header
// Description : Timeout Control
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_TIMEOUT_CONTROL_H
#define _G02_TIMEOUT_CONTROL_H

#include <portable_types.h>  // Type definitions
#include <G00.h>           // Services DLL import/export control
#include <G02.h>           // Time/Timer event definitions
#include <g02_datetime.h>  // ...

class SERVICES_EXTERNAL C_g02_timeout_control
{
public:
	C_g02_timeout_control(DWORD p_dwTimeout);
	C_g02_timeout_control(const C_g02_datetime &pc_datetime);
	~C_g02_timeout_control();

// Attributes
private:
	z				m_wait_forever_z;
	C_g02_datetime	m_timeout;

// Implementation
public:
	DWORD	time_left() const;
	void	reset(DWORD p_dwTimeout);
	void	reset(const C_g02_datetime &pc_datetime);
};

#endif  // _G02_TIMEOUT_CONTROL_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
