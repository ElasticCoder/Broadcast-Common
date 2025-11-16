#ifndef __SDTIMECONSTANTS_H__
#define __SDTIMECONSTANTS_H__
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : General Time Constants
// Creation    : AH on Thursday, February 17, 2000
// File Name   : $Workfile: SDTimeConstants.h $
// Revision No : $Revision: 10 $
// Description : 
//
// Last Mod by : $Author: Ah $
// Check in    : $Date: 18/12/00 5:47p $
///////////////////////////////////////////////////////////////////////////////
#pragma once

enum DATE_TIME_FORMATS
{
	DDhMMhYYYY,			// The h represents 'hyphen'(-)
	DAYspDDdMMdYYYY,	// sp=Space d = dot (WED 17.05.2000)
	DDdspMONspYYYY,		// 02. Aug 2000
	DAYspDDdMONdYYYY,	// (WED 17.May.2000)
	DD_MON_YYYY,		// DD MON YYYY (13 MAY 2000)
	DDhMMhYYYY_HHcMIcSS,// DD-MM-YYYY HH:MI:SS
	DDsMMsYYYY,			// The s represents 'slash' (/)
	HHcMMcSS,			// The c represents 'colon' (:)
	HHcMMcSScFF,		// HH:MM:SS:FF
	MMcSScFF,			// MM:SS:FF
	HHcMM,				// HH:MM
	MMcSS,				// MM:SS
	ReadAble,
	NaturalTime
};


enum FRAME_RATES
{
	PAL24FPS = 25,
	NTSC30FPS= 30
};

const int SD_REAL_TIME_LIMIT_UPPER = ((23*60*60) + (59*60) + 60);
const int SD_TIME_LIMIT_UPPER = ((25*60*60) + (59*60) + 60);
const int SD_TIME_LIMIT_LOWER = 0;

const int SD_SECONDS_IN_HOUR	= (60*60);
const int SD_SECONDS_IN_MINUTE  = 60;



//
// $Header: /FCM/FCM Service Module/Software/Common/Include/SDTimeConstants.h 10    18/12/00 5:47p Ah $
//

#endif //__SDTIMECONSTANTS_H__