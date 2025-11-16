///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g04_CmdLineInfo.cpp
// File Type   : C++ Source
// Description : Command Line Information
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>

#include <g04_CmdLineInfo.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : g04_CmdLineInfo constructor
// Author               : Duncan Olesen
// Date Created         : 17/06/96
// Date Last Modified   : 17/06/96
///////////////////////////////////////////////////////////////////////////////

C_g04_CmdLineInfo::C_g04_CmdLineInfo()
{
	m_instance_w	= 0;		// application instance undefined
	m_inst_param_z	= FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : g04_CmdLineInfo destructor
// Author               : Duncan Olesen
// Date Created         : 17/06/96
// Date Last Modified   : 17/06/96
///////////////////////////////////////////////////////////////////////////////

C_g04_CmdLineInfo::~C_g04_CmdLineInfo()
{
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Parse a command line parameter
// Author               : Duncan Olesen
// Date Created         : 17/06/96
// Date Last Modified   : 17/06/96
///////////////////////////////////////////////////////////////////////////////

void C_g04_CmdLineInfo::ParseParam(
							LPCTSTR	lpszParam,
							BOOL	bFlag,
							BOOL	bLast)
{
	z	param_handled_z;
	int	scanned;

	param_handled_z = FALSE;
//
// If previous parameter was the instance flag, then get the instance
// number
//
	if (m_inst_param_z == TRUE)
	{
		m_inst_param_z = FALSE;

		if (!bFlag)
		{
			scanned = _stscanf(lpszParam, _T("%hu"), &m_instance_w);
			param_handled_z = TRUE;
		}
	}
//
// If parameter is the instance flag (/inst)
//
	if (bFlag)
	{
		if (_tcscmp(lpszParam, _T("inst")) == 0) 
		{
			m_inst_param_z  = TRUE;
			param_handled_z = TRUE;
		}
	}

	if (param_handled_z)
	{
//
// If parameter handled then perform standard last parameter
// processing
//
		if (bLast)
		{
			if (m_nShellCommand == FileNew && !m_strFileName.IsEmpty())
				m_nShellCommand = FileOpen;
			m_bShowSplash = !m_bRunEmbedded && !m_bRunAutomated;
		}
	}
	else
	{
//
// Parameter not handled, so get base class method to do so
//
		CCommandLineInfo::ParseParam(lpszParam,
									 bFlag,
									 bLast);
	}
} // ParseParam

