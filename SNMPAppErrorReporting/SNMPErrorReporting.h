// SNMPErrorReporting.h : main header file for the SNMPERRORREPORTING DLL
//

#if !defined(AFX_SNMPERRORREPORTING_H__DA2B92AA_592F_11D3_BBCE_00C04F79B6F0__INCLUDED_)
#define AFX_SNMPERRORREPORTING_H__DA2B92AA_592F_11D3_BBCE_00C04F79B6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#define C_CALLING	extern "C"

/////////////////////////////////////////////////////////////////////////////
// CSNMPErrorReportingApp
// See SNMPErrorReporting.cpp for the implementation of this class
//

class CSNMPErrorReportingApp : public CWinApp
{
public:
	CSNMPErrorReportingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSNMPErrorReportingApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSNMPErrorReportingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNMPERRORREPORTING_H__DA2B92AA_592F_11D3_BBCE_00C04F79B6F0__INCLUDED_)
