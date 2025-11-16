// GPI Trigger.h : main header file for the GPI TRIGGER DLL
//

#if !defined(AFX_GPITRIGGER_H__1D2FB3D8_2F8C_11D3_A2DC_00C04F8EBF64__INCLUDED_)
#define AFX_GPITRIGGER_H__1D2FB3D8_2F8C_11D3_A2DC_00C04F8EBF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGPITriggerApp
// See GPI Trigger.cpp for the implementation of this class
//

class CGPITriggerApp : public CWinApp
{
public:
	CGPITriggerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPITriggerApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGPITriggerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPITRIGGER_H__1D2FB3D8_2F8C_11D3_A2DC_00C04F8EBF64__INCLUDED_)
