///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : services.cpp
// File Type   : C++ Source
// Description : Defines the initialization routines for the DLL.
// Author      : Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "services.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServicesApp

BEGIN_MESSAGE_MAP(CServicesApp, CWinApp)
	//{{AFX_MSG_MAP(CServicesApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServicesApp construction

CServicesApp::CServicesApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServicesApp object

CServicesApp theApp;
