// SCEvent.h: interface for the SCEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCEVENT_H__F9E17BDD_E03F_11D2_BDD8_00C04FB1589E__INCLUDED_)
#define AFX_SCEVENT_H__F9E17BDD_E03F_11D2_BDD8_00C04FB1589E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BOOL SD_MANUAL_RESET	= TRUE;
const BOOL SD_AUTO_RESET	= FALSE;

const BOOL SD_SIGNALLED		= TRUE;
const BOOL SD_UNSIGNALLED	= FALSE;



class SCEvent  
{
public:
				SCEvent(bool bReset =SD_MANUAL_RESET, bool bSignalled = SD_SIGNALLED);
	virtual		~SCEvent();

	void		Set() {
		::SetEvent(m_hEvent);
	};

	void		Reset() {
		::ResetEvent(m_hEvent);
	};

	DWORD		Wait( DWORD dwMilliseconds = INFINITE);

	DWORD		WaitMulti( DWORD dwMilliseconds = INFINITE);
	DWORD		WaitMulti( UINT wWinMessage, DWORD dwMilliseconds, bool& rWinMsg);
	//==============================
	// Returns this classes handle
	//==============================
	HANDLE		EventHandle( ) const {
		return m_hEvent;
	};

private:
	HANDLE		m_hEvent;
};

#endif // !defined(AFX_SCEVENT_H__F9E17BDD_E03F_11D2_BDD8_00C04FB1589E__INCLUDED_)
