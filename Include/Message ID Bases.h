///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : Message ID Bases.h       
// File Type   : C++ header
// Description : This file defines the base message numbers to be used when 
//               communicating within and between applications.
// Author      : Piers Lawson, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MESSAGE_ID_BASES_H
#define _MESSAGE_ID_BASES_H

// Message ID bases for Services threads
const UINT TIMER_THREAD = WM_USER + 10;	// Timer thread

// Message ID bases for Application threads
const UINT APP_THREAD_MSG_BASE = WM_USER + 100;

#endif // _MESSAGE_ID_BASES_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
