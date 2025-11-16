// SCReportError.h: interface for the SCReportError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREPORTERROR_H__DA2B92B1_592F_11D3_BBCE_00C04F79B6F0__INCLUDED_)
#define AFX_SCREPORTERROR_H__DA2B92B1_592F_11D3_BBCE_00C04F79B6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SNMPMIBParameters.h"

#define SZNAMEDPIPESERVER	TEXT("\\\\.\\pipe\\SPDExtensionAgent")

SNMPMIBParameters m_MIBParameters;
/////////////////////////////////////////////////////////////////////////////
// SCReportError

class SCReportError 
{
public:
	SCReportError(); 
	~SCReportError(); 

// SIReportError
private:
	HANDLE	m_hPipe;
	int		m_iInteger;
	char	m_szString[30];
public:
	void SendTrapInfo(int Trap, int value, LPCTSTR AppName);
	void SendTrapInfo(int Trap, int value, LPCTSTR AppName, LPCTSTR DescriptionString);
	BOOL OpenTrapPipe();
	void CloseTrapPipe();

};

SCReportError m_Report;

#endif // !defined(AFX_SCREPORTERROR_H__9A9641FA_D87B_11D2_AB05_00C04FB158A9__INCLUDED_)
