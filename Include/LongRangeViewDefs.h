///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Long Range View Structure Definitions
// Creation    : AH on Thursday, February 17, 2000
// File Name   : $Workfile: LongRangeViewDefs.h $
// Revision No : $Revision: 2 $
// Description : 
//
// Last Mod by : $Author: Paj $
// Check in    : $Date: 1/04/01 10:27a $
///////////////////////////////////////////////////////////////////////////////
#ifndef __LONGRANGEVIEWDEFS_H__
#define __LONGRANGEVIEWDEFS_H__
#pragma once


typedef struct LONGRANGE_DATA
{
#if defined (__cplusplus)
	//----------------------------------------
	//Perform C++ type initialisations here...
	//----------------------------------------
	void CoInitialize( )
	{
		ZeroMemory(this, sizeof(LONGRANGE_DATA) );
	}
	
	void CoFree( )
	{
		::SysFreeString( Title );
		::SysFreeString(BroadcastID);
		::SysFreeString(ItemPattern);
	}
#endif

	int		nTitleLen;
	int		nBroadcastIDLen;
	int		nItemPatternLen;
	//
	// Data
	//
	long	PlannedTime;
	long	BookedDuration;
	long	Duration;

	long	PlannedDay;
	long	PlannedMonth;
	long	PlannedYear;
	BSTR BroadcastID;
	long SI_Indent;
	BSTR ItemPattern;

	#if defined (__midl)
	[size_is(nTitleLen + nBroadcastIDLen + nItemPatternLen)]
	#endif
	BSTR		Title;
} LONGRANGE_DATA;


#endif //__LONGRANGEVIEWDEFS_H__

//
// $Header: /TX-Suite/Common/Include/LongRangeViewDefs.h 2     1/04/01 10:27a Paj $
//
