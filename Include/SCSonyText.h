// SCSonyText.h: interface for the SCSonyText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCSONYTEXT_H__B84B6821_B4FB_11D2_BDB2_00C04FB1589E__INCLUDED_)
#define AFX_SCSONYTEXT_H__B84B6821_B4FB_11D2_BDB2_00C04FB1589E__INCLUDED_

#include "Language.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SCSonyText  
{
public:
	
						SCSonyText();
	virtual				~SCSonyText();
public:
	CString				GetSonyText(UINT nID);
private:
	HMODULE				m_hModLang;
	
	LPFNDLLGETLANGTEXT	m_pfnGetLangString ;
	
	CString				m_StrReturn;
};

#endif // !defined(AFX_SCSONYTEXT_H__B84B6821_B4FB_11D2_BDB2_00C04FB1589E__INCLUDED_)
