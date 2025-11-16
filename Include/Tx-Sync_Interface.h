#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//
// Interface.h - 
//    Declarations of interfaces, IIDs, and CLSID
//    shared by the client and the component.
//

// Windows message IDs
#define WM_NEWONLINEEVENT					WM_USER + 102

// Windows message variables
#define STATUS_OK							0

// COM interfaces
interface IControl : IUnknown
{
	virtual HRESULT pascal Create(HWND pWnd, CString sIPAddress, UINT iPort) = 0;
	virtual HRESULT pascal Start() = 0;
	virtual HRESULT pascal Stop() = 0;
};


interface IY : IUnknown
{
	virtual void pascal Fy() = 0 ;
};

//
// Declaration of GUIDs for interfaces and component.
//   These constants are defined in GUIDs.cpp.
//

extern "C" const IID IID_IControl ;
extern "C" const IID IID_IY ;

extern "C" const CLSID CLSID_NetSyncComponent;


