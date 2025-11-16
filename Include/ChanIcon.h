// ChanIcon.h: interface for the CChanIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANICON_H__E6FC4691_EBD4_11D1_A34E_00805F8AECB6__INCLUDED_)
#define AFX_CHANICON_H__E6FC4691_EBD4_11D1_A34E_00805F8AECB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CChanIcon  
{
public:
	void SetFileDir(CString p_sFileDir);
	BOOL AddChannel(CString p_sChanID);
	void RemoveAll();
	int GetImagePos(CString p_sChanID);
	CImageList* GetImageList(BOOL bLargeIcon=TRUE);
	HICON GethIcon(CString p_ChanID, BOOL bLargeIcon=TRUE);
	BOOL LoadIcons(CStringArray* p_sChanArray, CString p_sFileDir);
	CChanIcon();
	virtual ~CChanIcon();

protected:
	CString m_sFileDir;
	CImageList m_LargeIcons;
	CImageList m_SmallIcons;
	CStringArray m_ChannelID;
};

#endif // !defined(AFX_CHANICON_H__E6FC4691_EBD4_11D1_A34E_00805F8AECB6__INCLUDED_)
