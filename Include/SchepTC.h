///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1999; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : GENERIC Time Handling Class
// Creation    : AH on Tuesday, November 09, 1999
// File Name   : $Workfile: SchepTC.h $
// Revision No : $Revision: 31 $
// Description : 
//
// Last Mod by : $Author: Ajp $
// Check in    : $Date: 9/10/00 15:14 $
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_SCHEPTC_H__2A2E9BF1_9BE5_11D1_AAC2_00C04FB1589E__INCLUDED_)
#define AFX_SCHEPTC_H__2A2E9BF1_9BE5_11D1_AAC2_00C04FB1589E__INCLUDED_

#pragma once

#include "timecode.h"

#include "SDTimeConstants.h"

const b TDB_TCSTRING_LEN	= 8;
const b DISP_TCSTRING_LEN	= 11;

/////////////////////////////////////////////////////////////////////////////
// CSchepTC dialog

class CSchepTC : public CTimecode
{
public:
	//==============================
	// Static helpers
	//==============================
	static long CSchepTC::FramesFromMinutes(int iMinutes) {
		return iMinutes * 60 * 25;
	};
	static long CSchepTC::FramesFromSeconds(long iSeconds) {
		return iSeconds * 25;
	};
	static long CSchepTC::SecondsFromFrames(long lFrames) {
		return lFrames / 25;
	};
	static long CSchepTC::DaysFromFrames(long lFrames) {
		return lFrames / CPS_FRAMES_PER_DAY_PAL_SECAM;
	};

public:
	void SetDate(DATE);
	DATE GetDate(DATE& rDate);
	CString		TimingErrorString( LONG lTimingErr );
	
	static CSchepTC& Now();
	bool		GapGreaterThan( WORD wFrames, CSchepTC TC2 );
	CSchepTC&	Copy( const CSchepTC& TC);
	
	void		SetSign(char cSign) {
		if ( cSign == '+' )
		{
			m_bPositive = TRUE;
		}
		else
		{
			m_bPositive = FALSE;
		}
	};

	CString		ShowTime();
	const CTime& GetDate( ) const;
	void		SetDate(const CTime& Date);
	void		SetDate(int p_iDay, int p_iMonth, int p_iYear);
	long		CalcDiff(const CSchepTC& TC, CString& sDiffText);
	long		CalcDiff(CSchepTC p_Upper);
	bool		CalcSplit(const CSchepTC& Start, const CSchepTC& End) ;
	long		GetDBTimeVal( );
	void		Empty( );
	BOOL		IsEmpty( );

	CString		GetString(enum DATE_TIME_FORMATS Mode);
	CString		GetString( );
	CString		GetNSString();
	CString		FormatDB( );
	
	//======================================
	// Constructors and creation methods
	//======================================
				CSchepTC(  );
				CSchepTC(int iHours, int iMinutes=0, int iSeconds=0, int iFrames=0);
				CSchepTC(const DATE date);
				
	void		Create(long TimeCodeValue);
	void		Create(const CString& sSchepTC);
	BOOL		ParseString(const CString &Timecode_Text);
	//=============
	// Operators
	//=============
	CSchepTC	&operator=(const CTimecode &p_rhs);
	CSchepTC	&operator=(const CSchepTC &p_rhs);

	BOOL		operator <= (const CSchepTC& p_rhs);
	BOOL		operator >= (const CSchepTC& p_rhs);
	BOOL		operator < (const CSchepTC& p_rhs);
	BOOL		operator > (const CSchepTC& p_rhs);
	CSchepTC&	operator+=(m p_frames_m);
	CSchepTC&	operator-=(m p_frames_m);

	BOOL		operator!=(CSchepTC &other);
	BOOL		operator==(CSchepTC &other);
	//==============================
	// Set the text for GAP
	//==============================
	void		SetGapText(const CString& Text) {
		m_GapText = Text;
	};
	//==============================
	// Set the text for overlap
	//==============================
	void		SetOverlapText(const CString& Text) {
		m_OverlapText = Text;
	};
	//==============================
	// Set the text for GAP > day
	//==============================
	void		SetBigGapText(const CString& Text) {
		m_BigGapText = Text;
	};
	//==============================
	// Set the text for Overlap > day
	//==============================
	void		SetBigOverlapText(const CString& Text) {
		m_BigOverlapText = Text;
	};

protected:
	//{{AFX_MSG(CSchepTC)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
private:
	void		InitializeSTC( );

	//
	// Place un-implemented operators here
	//
	CSchepTC&	operator-(const CSchepTC& TC);
	CSchepTC&	operator+(const CSchepTC& TC);


	CString		FormatDiffText(const CString& sBase, DWORD dwDiff );
	long		JapDate(const CTime& Time);
	//
	// Data...
	//
	BOOL		m_bPositive;
	BOOL		m_bEmpty;
	DWORD		m_TDBTCValue;
	CTime		m_Date;
	CString		m_sReadableDateTime;

	CString		m_GapText;
	CString		m_OverlapText;

	CString		m_BigGapText;
	CString		m_BigOverlapText;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHEPTC_H__2A2E9BF1_9BE5_11D1_AAC2_00C04FB1589E__INCLUDED_)
//
// $Header: /Publisuisse/LMSTransfer/Include/SchepTC.h 31    9/10/00 15:14 Ajp $
//
