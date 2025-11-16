// TimeData.h: interface for the CTimeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEDATA_H__FE645782_620E_11D2_A1C8_00C04F8EBF64__INCLUDED_)
#define AFX_TIMEDATA_H__FE645782_620E_11D2_A1C8_00C04F8EBF64__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Added by SPD
#include <SchepTC.h>

class CTimeData : public CSchepTC
{
public:
	CSchepTC& operator= (const CTimeData &p_rhs);
	BOOL IsValid();
	BOOL VCreate(CString p_sTime, CString p_sDate);
	long GetFrames();
	CTimeData();
	virtual ~CTimeData();
protected:
	BOOL m_zIsValid;
};

#endif // !defined(AFX_TIMEDATA_H__FE645782_620E_11D2_A1C8_00C04F8EBF64__INCLUDED_)
