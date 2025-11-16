// RegData.h: interface for the CRegData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGDATA_H__E7F0E1FE_CDEF_11D1_AAF7_00C04FB1589E__INCLUDED_)
#define AFX_REGDATA_H__E7F0E1FE_CDEF_11D1_AAF7_00C04FB1589E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

const BYTE STR_BUF_SIZE = 127;

// Application configuration keys
#define KEY_PAWS_APP			_T("TX-Suite\\PAWS")
#define KEY_TX_SETTINGS			_T("TX-Suite\\Settings")
#define KEY_TXVIEW				_T("TX-Suite\\TX-VIEW")

// Paws configuration keys
#define KEY_CHANNEL				_T("Channel")
#define KEY_PAWS_SETTINGS		_T("Settings")
#define KEY_REPORT_FILE			_T("Use Report File")
#define KEY_REPORT_LOCATION		_T("Report File Location")
#define KEY_AUTO_NUM			_T("Auto Event Numbering")
#define KEY_PLAYLIST_LOCATION	_T("Default List File Location")
#define KEY_CONFIRM_OVERWRITE	_T("Confirm Overwrite")
#define KEY_COLUMNS				_T("Columns")

// TX-Suite settings key
#define KEY_SETTINGS_ICON		_T("Icon Directory")

class CRegData : public CObject  
{
public:
	static void SetAppKey(const CString& sAppKey);
	
	
	CRegData(const CString&);
	virtual ~CRegData();

	//
	// Overload #1 Integer values
	//
	void			WriteVal(const CString& sKey, DWORD dwValue);
	//
	// Overload #2 String values
	//
	void			WriteVal(const CString& sKey, const CString& sValue);

	DWORD			ReadIntValue(const CString&,DWORD dwDefaultVal = 0);
	CString			ReadStringValue(const CString& sValue);
	CString			ReadStringValueEx(const CString&, const CString&);
	static CString  GetSubKey(const CString&);

private:
	HKEY	m_hKey;
};

#endif // !defined(AFX_REGDATA_H__E7F0E1FE_CDEF_11D1_AAF7_00C04FB1589E__INCLUDED_)
