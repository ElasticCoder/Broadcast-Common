#ifndef __BLOCKEDITORINTERFACE_H__
#define __BLOCKEDITORINTERFACE_H__
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PAWS Block Editor Interface Header
// Creation    : AH on Tuesday, April 18, 2000
// File Name   : $Workfile: BlockEditorInterface.h $
// Revision No : $Revision: 4 $
// Description : 
//
// Last Mod by : $Author: Ah $
// Check in    : $Date: 18/08/00 3:41p $
///////////////////////////////////////////////////////////////////////////////

#pragma once

//-------------------------------------------------------------------------
// This definition is used to control the inclusion/exclusion of the
// definitions in this file.  If #import on the TLB is used the generated
// headers contain duplicates of the definitions that clash.
//-------------------------------------------------------------------------

#if defined ( _IMPORT_DEFINITIONS )
enum FILTER_LEVELS
{
	SD_FILTER_LEVEL_BLOCK,
	SD_FILTER_LEVEL_SPOTS,
	SD_FILTER_LEVEL_PADDING
};
#endif

#if defined (__cplusplus)
typedef struct UpdateData_tag
#else
typedef struct UpdateData
#endif
{
#if defined (__cplusplus)
	void CoInitialize( )
	{
		ZeroMemory(this, sizeof(UpdateData_tag) );
	}
	
	void CoFree( )
	{
		::SysFreeString( m_OutputChannel );
		::SysFreeString( m_BroadcastID );
		::SysFreeString( m_Title );
		::SysFreeString( m_MaterialID );
		::SysFreeString( m_DeviceID );
	}

	void	SIIdent(LONG NewVal)			{m_SIIdent = NewVal;}
	void	Title(LPCSTR rNewVal)			{m_Title = A2BSTR(rNewVal);}
	void	SBIdent(LONG NewVal)			{m_SBIdent = NewVal ;}
	void    FilterLevel(short NewVal)		{m_FilterLevel = NewVal;}
	void    PlannedDuration(LONG NewVal)	{m_PlannedDuration = NewVal;}
	void    PlannedDate(DATE NewVal)		{m_PlannedDate = NewVal;}
	void    PlannedTime(LONG NewVal)		{m_PlannedTime = NewVal;}
	void    SOM(LONG NewVal)				{m_SOM = NewVal;}
	void	Cancelled(bool rNewVal)			{m_bCancelled = ((rNewVal)?VARIANT_TRUE:(VARIANT_FALSE));}

	void    OutputChannel(LPCSTR rNewVal)	{m_OutputChannel = A2BSTR(rNewVal);}
	void    BroadcastID(LPCSTR rNewVal)		{m_BroadcastID = A2BSTR(rNewVal);}
	void    MaterialID(LPCSTR rNewVal)		{m_MaterialID = A2BSTR(rNewVal);}
	void    DeviceID(LPCSTR rNewVal)		{m_DeviceID = A2BSTR(rNewVal);}


#endif
	//---------------------------------------------
	// Data...
	//---------------------------------------------
	LONG			m_SIIdent;
	LONG			m_SBIdent;
	LONG			m_PlannedDuration;
	DATE			m_PlannedDate;
	LONG			m_PlannedTime;
	LONG			m_SOM;
	short			m_FilterLevel;
	VARIANT_BOOL	m_bCancelled;
	//---------------------------------------------
	// Strings...
	//---------------------------------------------
	BSTR			m_OutputChannel;
	BSTR			m_BroadcastID;
	BSTR			m_Title;
	BSTR			m_MaterialID;
	BSTR			m_DeviceID;
} BLOCK_UPDATE_DATA, *PBLOCK_UPDATE_DATA;






//
// $Header: /TX-Suite/Common/Include/BlockEditorInterface.h 4     18/08/00 3:41p Ah $
//

#endif //__BLOCKEDITORINTERFACE_H__