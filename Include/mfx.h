////////////////////////////////////////////////////////////////
// MFX Microsoft Foundation Class Library Extensions
// 1993 Written by Paul DiLascia
//
// NOTE: for proper indention, set tab size = 3. 

#ifndef _MFX_H
#define _MFX_H

#include <afxwin.h>			// MFC core and standard components
#include <afxext.h>			// MFC extensions (including VB)
#include <stdlib.h>
#include <string.h>
#include <Timecode.h>

/////////////////
// Useful typedefs
typedef const char* CSTR;

// Shorthand for checking runtime class
#define ISA(classname)	IsKindOf(RUNTIME_CLASS(classname))

//////////////////
// MFX application object.
// Apps should derive their main application object from this.
//
class CMfxApp : public CWinApp {
	DECLARE_DYNAMIC(CMfxApp)
public:
	CMfxApp();

	HCURSOR LoadCursor(LPCSTR name)
		{ return ::LoadCursor(HIWORD(name) ? m_hInstance : NULL, name); }
	HCURSOR LoadCursor(int id)
		{ return CWinApp::LoadCursor(id); }
};

#ifndef _DEBUG
inline CMfxApp* MfxGetApp() { return (CMfxApp*)AfxGetApp(); }
#else
extern CMfxApp* MfxGetApp();
#endif

//////////////////
// Like a normal CRect, but has constructors to get window dimensions.
// Gets either client area, client area in screen coords or window
// in screen coords.
//
enum WHICHRECT { CLIENT, CLIENTSCREEN, WINDOW, WINDOWCLIENT, MDICLIENT };

class CWinRect : public CRect {
public:
   //enum WHICHRECT { CLIENT, CLIENTSCREEN, WINDOW, WINDOWCLIENT, MDICLIENT };
private:
   void Construct(CWnd *pWnd, WHICHRECT which);
public:
   // Constructors and assignment
   CWinRect(CWnd *pWnd, WHICHRECT which=CLIENT) { Construct(pWnd, which); }
   CWinRect(CWnd  &wnd, WHICHRECT which=CLIENT) { Construct(&wnd, which); }
   CWinRect(const CRect& rc)  { *this = rc; }
   CWinRect& operator= (const CRect& rc) 
      { *((CRect*)this) = rc; return *this; }

   void CapturePt(CPoint &pt);
   void Normalize();
};

//////////////////
// Extended string class
//
class CMfxString : public CString {
public:
   // Constructors
   CMfxString()                           { }
   CMfxString(CSTR str) : CString(str)    { }
   CMfxString(CString s): CString(s)      { }

   // Assignment
   const CMfxString& operator=(CSTR str)  
      { CString::operator=(str); return *this; }
   const CMfxString& operator=(CString s) 
      { CString::operator=(s); return *this; }

   // Useful stuff
   int printf(CSTR fmt, ...);
   int MessageBox(int type=MB_OK, CSTR caption=NULL)
      { return ::MessageBox(NULL,(CSTR)(*this),caption,type); }
};


////////////////////////////////////////////////////////////////
// Global objects for mouse, keyboard, cursor, etc.
////////////////////////////////////////////////////////////////

//////////////////
// Global mouse object.  There's only one: TheMouse.
// Use this object to get/set mouse parameters, capture the mouse, etc.
// 
class GMouse {
public:
   CWnd* Capture()         { return CWnd::GetCapture(); }
   CWnd* Capture(CWnd *w)  { return w->SetCapture(); }
   void Release()          { ReleaseCapture(); }
};
extern GMouse TheMouse;

/////////////////
// Global Cursor.
// 
class GCursor {
public:
   HCURSOR operator= (HCURSOR hc)   { return SetCursor(hc); }
   HCURSOR operator= (LPCSTR name)  
      { return *this = MfxGetApp()->LoadCursor(name); }
   HCURSOR operator= (int id)       
      { return *this = MfxGetApp()->LoadCursor(id); }

   void     GetPos(int x, int y)    { SetCursorPos(x, y); }
   CPoint   GetPos()                { CPoint p; GetCursorPos(&p); return p; }
   operator CPoint()                { return GetPos(); }
   CPoint   operator=(CPoint p)     { SetCursorPos(p.x, p.y); return p; }

   void Clip(CWnd *pWnd);
   void Clip(CRect &rect)           { ClipCursor(&rect); }
   void UnClip()                    { ClipCursor(NULL); }
};
extern GCursor TheCursor;

//////////////////
// Global Keyboard object, used to get info about the keyboard.
// 
class GKeyboard {
public:
   int CodePage()                { return GetKBCodePage(); }
   int Type()                    { return GetKeyboardType(0); }
   int SubType()                 { return GetKeyboardType(1); }
   int NumFnKeys()               { return GetKeyboardType(2); }
   int KeyName(LONG lparam, char *buf, int buflen)
      { return GetKeyNameText(lparam, buf, buflen); }
   int State(int vkey)           { return GetKeyState(vkey); }
   void GetState(BYTE* buf)      { GetKeyboardState(buf); }
   void SetState(BYTE* buf)      { SetKeyboardState(buf); }
   BOOL IsArrowKey(int key)      { return VK_LEFT<=key && key<=VK_DOWN; }
};
extern GKeyboard TheKeyboard;

//////////////////
// Class to display a wait cursor (hourglass).
//
class CHourglass {
   CWnd*          m_pCapWnd;     // window to capture mouse, if any
   HCURSOR        m_hSaveCursor; // save cursor handle
public:
   CHourglass(CWnd *pWnd=NULL);
   ~CHourglass();
};

class CMfxTime : public CTimecode
{

public:
	enum TIME_FORMATS  {DDMMYYHHMMSS,DDMMYY,HHMMSS,HHMMSSFF,HHMM};

	static CString TNow(TIME_FORMATS TimeFormat = HHMMSSFF);


};


#endif // _MFX_H
