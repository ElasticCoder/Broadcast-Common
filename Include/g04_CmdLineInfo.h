///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g04_CmdLineInfo.h
// File Type   : C++ Header
// Description : Command Line Information
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G04_CMDLINEINFO_H
#define _G04_CMDLINEINFO_H

#include <portable_types.h>  // Type definitions
#include <standard.h>
#include <tchar.h>

#include <G00.h>

class SERVICES_EXTERNAL C_g04_CmdLineInfo : public CCommandLineInfo
{
public:
	C_g04_CmdLineInfo();
	~C_g04_CmdLineInfo();

// Attributes
public:
	w	m_instance_w;		// application instance

private:
	z	m_inst_param_z;		// Instance flag encountered and instance
							// parameter not encountered
// Implementation
public:
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

};

#endif _G04_CMDLINEINFO_H
