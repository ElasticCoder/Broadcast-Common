/******************************************************************************
 * Project     : PLATO Monitor (PETRA)
 * File        : GridCtrl.cpp
 * Author      : Paul Brain/Paul Tarney
 * Date        : 12/6/96
 *
 * Description : This file contains the grid functions
 * Updates	   :
 *
 *				22/Oct/97 - Andy Hill
 *				Modified the code to have some features required by the
 *              PLATO monitor.
 *				=====================
 *				20/Feb/98 - Andy Hill
 *				Added function (method) "SetCursorRow" to move the highlight
 *				bar. Required for SCHEP TDB Editor
 *				=====================
 *				02/Mar/98 - Andy Hill
 *				Added the functions 
 *				"MouseMove"
 *				"CheckMouse"
 *				"ProcLButtonUp"
 *				So that drag-drop could be implemented in SCHEP TDB Editor.
 *				======================
 *				17/Apr/98 - Andy Hill
 *				Added the functions
 *              "SetRowFlag"
 *				"ClearRowFlag"
 *				To Store a 32 bit word in each 'row' that is then passed to
 *				the caller in the Cell Structure
 *				======================
 *				29/Apr/98 - Andy Hill
 *				Added the function
 *				"SetHighlightBlock"
 *				to set the row ranges
 *              ======================
 *				01/July/98 - Andy Hill
 *				Added Focus Got/Lost events to parent
 *****************************************************************************/



/******************************************************************************
 * Include Files
 *****************************************************************************/
//
// Set up for unicode, if MFC has not already done this
// Not for Windows 95
//
#include "stdafx.h"

#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************
 * Grid Constants
 *****************************************************************************/

#define GRD_EDIT_ID     1   // Cell Edit control identifier
#define IDC_HDR_ID		2	// Header control identifier


/******************************************************************************
 * Grid Variables
 *****************************************************************************/

static HINSTANCE	v_inst_h;

/******************************************************************************
 * Grid Functions
 *****************************************************************************/

/******************************************************************************
 * CGridCtrl() - Routine to initialise the overall CGrdCtrl structure
 *****************************************************************************/

CGridCtrl::CGridCtrl(HWND grid_h)
{
	m_grid_h = grid_h;
	m_parent_h = GetParent(m_grid_h);
	m_pen_h = (HPEN)GetStockObject(BLACK_PEN);

    // Initialise with no panes
	m_paneCol_i = 0;
	m_paneRow_i = 0;
	//Clear any user data
	m_dwUserData = 0;

    // Initialise Scroll Ranges and Positions
	m_colScrlRng_i = 0;
	m_rowScrlRng_i = 0;
	m_colScrlPos_i = 0;
	m_rowScrlPos_i = 0;

	m_keyState_i  = 0;
    m_keysOn_z    = TRUE;
	m_crsrOn_z    = FALSE;
	m_cursorStyle_n = EDGE_SUNKEN;
	m_bUseBlockColouring = FALSE;
	m_cell3D_z	  = FALSE;
	m_crsrCol_i	  = 0;  
	m_crsrRow_i	  = 0;	
	m_colRng0_i	  = 0;
	m_rowRng0_i	  = 0;
	m_colRng1_i	  = 0;
	m_rowRng1_i	  = 0;

	m_numCols_i   = 4;	
	m_numRows_i	  = 50;     // Number of columns

	m_colSizes_ip = NULL;	// array of column sizes (NULL=>divide up client area equally)
	m_rowSizes_ip = NULL;	// array of row sizes (NULL => m_charHt_i+margins+1)

    // find out original scroll bar style of window & use if present
    m_colScrl_h = NULL;
    m_rowScrl_h = NULL;
    if (GetWindowLong(m_grid_h,GWL_STYLE) & WS_HSCROLL)
		m_colScrl_h = m_grid_h;
    if (GetWindowLong(m_grid_h,GWL_STYLE) & WS_VSCROLL)
		m_rowScrl_h = m_grid_h;
    m_colScrlNullStyle_i = GRD_SCRL_DSBL;
	m_rowScrlNullStyle_i = GRD_SCRL_DSBL;
	m_colScrlType_i = SB_HORZ;
	m_rowScrlType_i = SB_VERT;

    m_cellEdit_p = NULL;
    m_editCol_i = -1;
    m_editRow_i = -1;

	// Initialise header variables
	m_hasHeader_z		= FALSE;
	m_headerHeight_i	= 0;
	m_header_h			= NULL;
	m_colNames_tc2p	= NULL;

	SetFont((HFONT)GetStockObject(SYSTEM_FONT));

	m_iLastMouseRow = -1;
	
	m_BackColour	= GetSysColor(COLOR_APPWORKSPACE);
	//RGB(0xff,0xff,0xff);

	CalcLastCell();
} // CGridCtrl

/******************************************************************************
 * CGridCtrl() - Routine to initialise the overall CGrdCtrl structure
 *****************************************************************************/

CGridCtrl::~CGridCtrl()
{
    if (m_colSizes_ip != NULL)
        {
        delete [] m_colSizes_ip;
        }

    if (m_rowSizes_ip != NULL)
	{
        delete [] m_rowSizes_ip;
	}

	FreeColNames();
} // ~CGridCtrl

/******************************************************************************
 * grd_register() - Register window class
 *****************************************************************************/

BOOL grd_register(HINSTANCE inst_h)
{
    WNDCLASSEX  wndclass ;

	v_inst_h = inst_h;

	wndclass.cbSize        = sizeof (wndclass) ;
    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndclass.lpfnWndProc   = grd_winproc;
    wndclass.cbClsExtra    = 0 ;
    wndclass.cbWndExtra    = sizeof(void*); // extra space for class pointer
    wndclass.hInstance     = inst_h;
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
    wndclass.lpszMenuName  = NULL ;
    wndclass.lpszClassName = GRD_CLASSNAME;
	wndclass.hIconSm       = NULL;

    return (BOOL)RegisterClassEx(&wndclass);
} // grd_register

/******************************************************************************
 * grd_winproc() - Window Procedure
 *****************************************************************************/

LRESULT CALLBACK grd_winproc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    // get pointer to class
	CGridCtrl* info_p = (CGridCtrl*)GetWindowLong(hwnd,GRD_DATAIDX);

    switch (iMsg)
	    {
        case WM_CREATE :
            info_p = new CGridCtrl(hwnd);
			SetWindowLong(hwnd,GRD_DATAIDX,(LONG)info_p);
			if (!GetWindowLong(hwnd,GRD_DATAIDX) == (LONG)info_p)
                {
                return -1;
                }
            return 0;

        case WM_DESTROY :
            delete info_p;
            return 0;

		case WM_SETFONT:
            info_p->SetFont((HFONT)wParam);
		    return 0;

        case WM_SIZE :
            info_p->ProcSize(LOWORD(lParam),HIWORD(lParam));
            return 0 ;

        case WM_GRD_GET_CLASS :
            return (LRESULT)info_p;

		case WM_GETDLGCODE:
            return DLGC_WANTALLKEYS;// DLG_WANTARROWS

        case WM_COMMAND:
            info_p->ProcCommand(wParam,lParam);
            return 0;
		case WM_RBUTTONUP:
			return 0;
		case WM_RBUTTONDOWN:
			info_p-> ProcRButtonDown(LOWORD(lParam),HIWORD(lParam));
			return 0;
		case WM_LBUTTONDOWN:
            info_p->ProcLButtonDown(LOWORD(lParam),HIWORD(lParam));
		    return 0;
		case WM_LBUTTONUP:
			info_p->ProcLButtonUp(LOWORD(lParam),HIWORD(lParam));
			return 0;

		case WM_LBUTTONDBLCLK:
            info_p->ProcLButtonDblClick(LOWORD(lParam),HIWORD(lParam));
    		return 0;

        case WM_VSCROLL :
            info_p->ProcVScroll(LOWORD(wParam),HIWORD(wParam));
            return 0 ;

        case WM_HSCROLL :
            info_p->ProcHScroll(LOWORD(wParam),HIWORD(wParam));
            return 0 ;

        case WM_KEYDOWN :
             info_p->ProcKeyDown((int)wParam);
            return 0 ;

        case WM_PAINT :
            info_p->ProcPaint();
            return 0 ;

		case WM_ERASEBKGND:
			info_p-> OnEraseBackground( (HDC) wParam );
			return 1;

        case WM_NOTIFY :
            return info_p->ProcNotify((int)wParam, (NMHDR *)lParam);
          
		case WM_MOUSEMOVE:
			info_p->MouseMove(wParam,lParam);
			return 0;
		case WM_TIMER:
			if ( wParam == CHECK_MOUSE )
			{
				info_p->CheckMouse(  );
			}
			return 0;
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
			info_p -> FocusFunction( iMsg );
			return 0;
			break;
        default :
            return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
        }
} // grd_winproc



//===========================================================================
// Function name	: CGridCtrl::FocusFunction
// Return type		: void 
// Argument         : UINT iMsg
// Description	    : Calls events in the parent for kill/set focus
void CGridCtrl::FocusFunction(UINT iMsg)
{

	switch ( iMsg )
	{
	case WM_KILLFOCUS:
		SendMessage(m_parent_h, WM_GRD_LOSTFOCUS, (WPARAM)m_grid_h,(LPARAM)0);
		break;
	case WM_SETFOCUS:
		SendMessage(m_parent_h, WM_GRD_SETFOCUS, (WPARAM)m_grid_h,(LPARAM)0);
		break;
	default:
		TRACE("Huh!\n");
		break;
	}
	
}

/******************************************************************************
 * ProcSize() - Handles Size Messages
 *****************************************************************************/

void CGridCtrl::ProcSize(int xcoord_i, int ycoord_i)
{
    m_xsize_i = xcoord_i;
    m_ysize_i = ycoord_i;
	CalcScroll();
} // ProcSize

/******************************************************************************
 * ProcLButtonUp() - Handles Left Button Up Messages
 ******************************************************************************/

void CGridCtrl::ProcLButtonUp(int xcoord_i, int ycoord_i)
{
	int col_i = GetColPos(xcoord_i);
	int row_i = GetRowPos(ycoord_i);

	::KillTimer(m_grid_h, CHECK_MOUSE);

    if ((col_i != -1) && (row_i != -1))
    {
        SGrdCellInfo    cell;
        MakeDefaultCell(NULL,col_i,row_i,cell); // set up cell info

        SendMessage(m_parent_h,WM_GRD_LBUTTONUP,(WPARAM)m_grid_h,(LPARAM)&cell);
	}
	m_bMouseKeyDown = FALSE;
}


/******************************************************************************
 * ProcLButtonDown() - Handles Left Button Down Messages
 ******************************************************************************/

void CGridCtrl::ProcLButtonDown(int xcoord_i, int ycoord_i)
{
	m_bMouseKeyDown = TRUE;

    HandleClick(WM_GRD_SGL_CLK,xcoord_i,ycoord_i);
} // ProcLButtonDown

/******************************************************************************
 * ProcRButtonDown() - Handles Right Button Down Messages
 ******************************************************************************/

void CGridCtrl::ProcRButtonDown(int xcoord_i, int ycoord_i)
{
	m_bMouseKeyDown = TRUE;

    HandleClick(WM_GRD_RBUTTON_CLK,xcoord_i,ycoord_i);

}

/******************************************************************************
 * ProcRButtonUp() - Handles Right Button Up Messages
 ******************************************************************************/

void CGridCtrl::ProcRButtonUp(int xcoord_i, int ycoord_i)
{
	xcoord_i = xcoord_i;
	ycoord_i = ycoord_i;
}

/******************************************************************************
 * ProcLButtonDblClick() - Handles Left Button Double Click Messages
 ******************************************************************************/

void CGridCtrl::ProcLButtonDblClick(int xcoord_i, int ycoord_i)
{
    HandleClick(WM_GRD_DBL_CLK,xcoord_i,ycoord_i);
} // ProcLButtonDblClick

//==============================================================================
// Function name	: CheckMouse
// Description	    : Called on timer to check the state of the mouse 
// Return type		: none
void CGridCtrl::CheckMouse( )
{
	int
		iGridRows = m_numRows_i - m_rowScrlRng_i ;
	
	//
	// If the left mouse key is pressed
	//
	if ( m_bMouseKeyDown )
	{
		//
		// if the highlight row is at the bottom of the viewable screen
		//
		if ( m_iRowInView == iGridRows  )
		{
			ProcKeyDown( VK_DOWN ); //Move down
		}
		else
		if ( m_iRowInView == 1)
		{
			ProcKeyDown( VK_UP );	//Move up
		}
	}
}


//===========================================================================
// Function name	: CGridCtrl::MouseMove
// Description	    : 
// Return type		: void 
// Argument         : WPARAM
//						fwKeys = wParam;        // key flags 
// Argument         : LPARAM
//						xPos = LOWORD(lParam);  // horizontal position of cursor 
//						yPos = HIWORD(lParam);  // vertical position of cursor 
//
void CGridCtrl::MouseMove(WPARAM wParam, LPARAM lParam)
{
	 
	m_fwKeys = static_cast<WORD>(wParam);        // key flags 

	int col_i = GetColPos( LOWORD(lParam) );
	int row_i = GetRowPos( HIWORD(lParam) );
    
	if ((col_i != -1) && (row_i != -1))
    {
        SGrdCellInfo    
			cell;

        MakeDefaultCell(NULL, col_i, row_i, cell); // set up cell info
		
		
		//
		// Alert the user IF the row has changed AND a key is pressed.
		//
		if ( (row_i != m_iLastMouseRow) && (m_fwKeys != 0) )
		{
			

			SendMessage(m_parent_h,WM_GRD_MOUSEMOVE,(WPARAM)m_grid_h,(LPARAM)&cell);

			m_iLastMouseRow = row_i;
		}

		if ( (m_fwKeys & MK_LBUTTON ) == MK_LBUTTON )
		{
			//
			// if the left button is pressed then flag it and
			// set the timer to check it periodically
			//
			m_iRowInView = GetViewPortRow( row_i ) + 1; 
			
			m_bMouseKeyDown = TRUE;
			
			::SetTimer(m_grid_h, CHECK_MOUSE, 100, NULL);
		}
		else
		{
			if ( m_bMouseKeyDown )
			{
				//
				// Normally the timer would be stopped by releasing
				// the left key. If the cursor is moved off the grid
				// and the key released, moving back on to the grid
				// would still have the timer running. This check
				// ensures that the states are correctly set.
				//
				::KillTimer(m_grid_h, CHECK_MOUSE);
				m_bMouseKeyDown = FALSE;
				SendMessage(m_parent_h,WM_GRD_MOUSEMOVE,(WPARAM)m_grid_h,(LPARAM)&cell);
			}
		}
	}

}


/******************************************************************************
 * HandleClickRet() - Handles Click Return Messages
 ******************************************************************************/

void CGridCtrl::HandleClick(int type_i, int xcoord_i, int ycoord_i)
{
	int col_i = GetColPos(xcoord_i);
	int row_i = GetRowPos(ycoord_i);

    if ((col_i != -1) && (row_i != -1))
    {
		SGrdCellInfo    cell;
		
		GetKeyStates();

		MakeDefaultCell(NULL,col_i,row_i,cell); // set up cell info

		SetFocus(m_grid_h);

		switch(SendMessage(m_parent_h,type_i,(WPARAM)m_grid_h,(LPARAM)&cell))
		{
			case GRD_CLK_UPDATE_CRSR :
				UpdateCursor(col_i,row_i);
				break;

			case GRD_CLK_EDIT_CELL :
				EditCell(col_i,row_i);
				break;

			default :
				// no action
				break;
		}
    }
} // HandleClick

/******************************************************************************
 * EditCell() - Creates an Edit Control over the given cell
 *****************************************************************************/

void CGridCtrl::EditCell(int col_i, int row_i)
{
    SGrdCellInfo    cell;

    // Set up cell information
    MakeDefaultCell(NULL,col_i,row_i,cell);

	// Send Double Click Message to Window
    if (SendMessage(m_parent_h,WM_GRD_EDIT_CELL,(WPARAM)m_grid_h,(LPARAM)&cell))
        {
        RECT cellRect,textRect;
        CalcCellRect(&cellRect,cell.col_i,cell.row_i,cell.sizeCols_i,cell.sizeRows_i);
        CalcTextRect(&cellRect,&textRect);

        m_editCol_i = cell.col_i;
        m_editRow_i = cell.row_i;

        m_cellEdit_p = new CRichEditCtrl;
        m_cellEdit_p->Create(WS_CHILD | WS_VISIBLE | cell.txtFmt_m,
                             textRect,
                             CWnd::FromHandle(m_grid_h),
                             GRD_EDIT_ID);

        m_cellEdit_p->SetFocus();
        m_cellEdit_p->SetFont(CFont::FromHandle(m_font_h));
        m_cellEdit_p->SetWindowText(cell.text_tcp);
        }
} // EditCell

/******************************************************************************
 * ProcCommand() - Handles Command Messages
 *****************************************************************************/

void CGridCtrl::ProcCommand(WPARAM wParam, LPARAM lParam)
{
    WORD    NotifyCode_w    = HIWORD(wParam);
    WORD    id_w            = LOWORD(wParam);
    TCHAR*  buffer_p        = NULL;
    int     length_i        = 0;
    //ctrl_h         = (HWND)lParam;
	lParam = lParam;
    switch(id_w)
    {
        case GRD_EDIT_ID :
            switch(NotifyCode_w)
                {
                case EN_KILLFOCUS :
					{
                    // Set up cell information
                    SGrdCellInfo cell;
                    MakeDefaultCell(NULL,m_editCol_i,m_editRow_i,cell);
                    length_i = m_cellEdit_p->GetWindowTextLength() + 2;
                    buffer_p = new TCHAR[length_i];
                    m_cellEdit_p->GetWindowText(buffer_p,length_i);
                    cell.text_tcp = buffer_p;
                    SendMessage(m_parent_h,
                                WM_GRD_KILLFOCUS,
                                (WPARAM)m_grid_h,
                                (LPARAM)&cell);
                    delete []buffer_p;
                    delete m_cellEdit_p;
					}
                    break;

                default :
                    break;
                }
            break;

        default :
            break;
    }
} // ProcCommand

/******************************************************************************
 * ProcVScroll() - Handles Vertical Scroll Bar Messages
 *****************************************************************************/

void CGridCtrl::ProcVScroll(int action_i, int pos_i)
{
    int scroll_i = 0;

    SetFocus(m_grid_h);
    switch(action_i)
         {
         case SB_TOP :
              scroll_i = 0 - m_rowScrlPos_i;
              break ;

         case SB_BOTTOM :
              scroll_i = m_numRows_i - m_rowScrlPos_i;
              break ;

         case SB_LINEUP :
              scroll_i = -1 ;
              break ;

         case SB_LINEDOWN :
              scroll_i = 1 ;
              break ;

         case SB_PAGEUP :
              // this will not work with user defined row heights
              scroll_i = min(-1,-GetViewHt()/GetRowHt(0));
              break ;

         case SB_PAGEDOWN :
              // this will not work with user defined row heights
              scroll_i = max(1,GetViewHt()/GetRowHt(0));
              break ;

         case SB_THUMBTRACK :
              scroll_i = pos_i - m_rowScrlPos_i;
              break ;

         default :
              scroll_i = 0 ;
         }

    scroll_i = max(-m_rowScrlPos_i,min(scroll_i,m_rowScrlRng_i - m_rowScrlPos_i));

    if (scroll_i != 0)
        {
        SetRowScrlPos(m_rowScrlPos_i + scroll_i);
        }
} // ProcVScroll

/******************************************************************************
 * ProcHScroll() - Handles Horizontal Scroll Bar Messages
 *****************************************************************************/

void CGridCtrl::ProcHScroll(int action_i, int pos_i)
{
    int scroll_i = 0;

    SetFocus(m_grid_h);
    switch(action_i)
        {
        case SB_LINEUP :
            scroll_i = -1;
            break ;

        case SB_LINEDOWN :
            scroll_i = 1 ;
            break ;

        case SB_PAGEUP :
            scroll_i = -2 ;
            break ;

        case SB_PAGEDOWN :
            scroll_i = 2 ;
            break ;

        case SB_THUMBTRACK :
            scroll_i = pos_i - m_colScrlPos_i ;
            break ;

        default :
            scroll_i = 0 ;
        }
    scroll_i = max (-m_colScrlPos_i,min (scroll_i,m_colScrlRng_i-m_colScrlPos_i));

    if (scroll_i != 0)
        {
        SetColScrlPos(m_colScrlPos_i + scroll_i);
        }
} // ProcHScroll

/******************************************************************************
 * ProcKeyDown() - Handles Keys
 *****************************************************************************/

void CGridCtrl::ProcKeyDown(int key_i)
{
    if (m_keysOn_z) // if allowed to process keystrokes
        {
        switch (key_i)
            {
            case VK_HOME :
			    if (m_crsrRow_i != 0)
                    UpdateCursor(m_crsrCol_i,0);
                SendMessage (m_grid_h, WM_VSCROLL, SB_TOP, 0L);
                break ;

            case VK_END :
		        if (m_crsrRow_i != (m_numRows_i - 1))
  				    UpdateCursor(m_crsrCol_i,m_numRows_i-1);
                SendMessage (m_grid_h, WM_VSCROLL, SB_BOTTOM, 0L) ;
                break ;

            case VK_PRIOR :
                // This will not work with user defined row heights
			    UpdateCursor(m_crsrCol_i,
							 max(0,
								 m_crsrRow_i-(GetViewHt()/GetRowHt(0))));
                SendMessage (m_grid_h, WM_VSCROLL, SB_PAGEUP, 0L) ;
                break ;

            case VK_NEXT :
                // This will not work with user defined row heights
			    UpdateCursor(m_crsrCol_i,
							 min(m_numRows_i - 1,
								 m_crsrRow_i + (GetViewHt()/GetRowHt(0))));
                SendMessage (m_grid_h, WM_VSCROLL, SB_PAGEDOWN, 0L) ;
                break ;

            case VK_UP :
			    if (m_crsrRow_i != 0)
  				    UpdateCursor(m_crsrCol_i,m_crsrRow_i-1);
                if (m_crsrRow_i < m_rowScrlPos_i + m_paneRow_i)
				    SendMessage (m_grid_h, WM_VSCROLL, SB_LINEUP, 0L);
                break ;

            case VK_DOWN :
			    if (m_crsrRow_i != (m_numRows_i-1))
  				    UpdateCursor(m_crsrCol_i,m_crsrRow_i+1);

                // if cannot display the whole of the cell
                if (m_crsrRow_i >= m_lastRow_i)
                    {
                    // how many cells to scroll up to get in next one?
                    int cnt_i = 1;
                    int cell_i = m_rowScrlPos_i + m_paneRow_i;
                    int ht_i = GetRowHt(cell_i) - GetYPos(m_crsrRow_i) +
							   m_ysize_i + 1;
                    while(ht_i < GetRowHt(m_crsrRow_i))
                        {
                        ht_i += GetRowHt(++cell_i);
                        cnt_i++;
                        }

                    // change scroll position by count or max scroll
                    SetRowScrlPos(min(m_rowScrlPos_i + cnt_i,m_rowScrlRng_i));
                    }
                break ;

            case VK_LEFT :
			    if ( m_crsrCol_i )
  				    UpdateCursor(m_crsrCol_i-1,m_crsrRow_i);
                if (m_crsrCol_i < m_colScrlPos_i + m_paneCol_i)
				    SendMessage (m_grid_h, WM_HSCROLL, SB_LINEUP, 0L) ;
                break ;

            case VK_TAB :
				//== 01/July/1998 == Andy Hill ==
				// Added events for lost/got focus, when a Tab received
				//   call lost focus event, note that this does not cause
				//   the grid to lose focus, unless the receiver performs 
				//   some action
				SendMessage(m_parent_h, WM_GRD_LOSTFOCUS, (WPARAM)m_grid_h, MAKELPARAM(VK_TAB, 0));
				break;
            case VK_RIGHT :
			    if ( m_crsrCol_i != (m_numCols_i-1) )
  				    UpdateCursor(m_crsrCol_i+1,m_crsrRow_i);

                // if cannot display the whole of the cell
                if (m_crsrCol_i >= m_lastCol_i)
                    {
                    // how many cells to scroll left to get in next one?
                    int cnt_i = 1;
                    int cell_i = m_colScrlPos_i + m_paneCol_i;
                    int width_i = GetColWd(cell_i) - GetXPos(m_crsrCol_i) + m_xsize_i + 1;
                    while(width_i < GetColWd(m_crsrCol_i))
                        {
                        width_i += GetColWd(++cell_i);
                        cnt_i++;
                        }

                    // change scroll position by count or max scroll
                    SetColScrlPos(min(m_colScrlPos_i + cnt_i,m_colScrlRng_i));
                    }
                break ;
            } // end case
        } // end if keys allowed
} // ProcKeyDown

/******************************************************************************
 * ProcPaint() - paints the grid
 *****************************************************************************/

void CGridCtrl::ProcPaint()
{
    PAINTSTRUCT ps;
	HDC			hdc;
	HRGN		cell_rgn;
	RECT		cell_rect;

	SGrdCellInfo   cell;

	int			xpos_i;			
	int			ypos_i;

    int         col_i;
    int         row_i;

    hdc = BeginPaint (m_grid_h, &ps) ;

    // Loop Backwards Over Visible Rows
    row_i = m_lastRow_i+1;
    ypos_i = GetYPos(row_i);

    while(row_i > 0)
    {
        row_i--;
        ypos_i = GetYPos(row_i);

        // if row is actually in drawing area
        if (((ypos_i + GetRowHt(row_i)) > ps.rcPaint.top) &&
             (ypos_i < ps.rcPaint.bottom))
        {
            // Loop Backwards Over Visible Columns
            col_i = m_lastCol_i+1;
        
            while(col_i > 0)
			{
                col_i--;
                xpos_i = GetXPos(col_i);

                if (((xpos_i + GetColWd(col_i)) > ps.rcPaint.left) && 
                    (xpos_i < ps.rcPaint.right))
                {
                    // Get Parent Cell Information
                    MakeDefaultCell(hdc,col_i,row_i,cell);
                    
					SendMessage(m_parent_h,WM_GRD_GET_INFO,(WPARAM)m_grid_h,(LPARAM)&cell);

			        // Create clipping region
                    CalcCellRect(&cell_rect,
                                 cell.col_i,cell.row_i,cell.sizeCols_i,cell.sizeRows_i);

			        cell_rgn = CreateRectRgn( cell_rect.left,
									          cell_rect.top, 
									          cell_rect.right, 
									          cell_rect.bottom );

					SelectClipRgn(hdc,cell_rgn);
			        DeleteObject(cell_rgn);
					
					m_cell3D_z = cell.cell3D_z;

                    // Draw Text?
			        if (!cell.owner_z)
				    {
                        //
			            // But background into cell whatever to make sure scrolled
                        // cells that have already been drawn are overdrawn. These
                        // overdrawn cells may be multi-row/col and hence may have
                        // some visible part, hence they have to be drawn and then
                        // overdrawn later
                        //
			            HBRUSH 
							newBrush_h,
							oldBrush_h;
						
						if ( m_bUseBlockColouring && InBlock( row_i, col_i ) )
						{
							newBrush_h = CreateSolidBrush( m_clrBackGround );
						}
						else
						{
							newBrush_h = CreateSolidBrush(cell.bkColor_m);
						}
						
						oldBrush_h = (HBRUSH)SelectObject(hdc,newBrush_h);
						
						SelectObject(hdc, GetStockObject(NULL_PEN));

			            Rectangle(hdc,
					              cell_rect.left, 
					              cell_rect.top,  
					              cell_rect.right,
					              cell_rect.bottom);

			            SelectObject(hdc,oldBrush_h);
			            DeleteObject(newBrush_h);

                        //
			            // Put text into cell?
                        //
                        if (_tcslen(cell.text_tcp) != 0)
                        {
                            RECT text_rect;
                            CalcTextRect(&cell_rect,&text_rect);

                            HFONT
								oldFont_h = (HFONT)SelectObject(hdc,cell.font_h);
							COLORREF 
								oldTxtColor_m,
								oldBkColor_m;

							if ( m_bUseBlockColouring && InBlock( row_i, col_i ) )
							{
								oldTxtColor_m = SetTextColor(hdc, m_clrForeGround );
								oldBkColor_m  = SetBkColor  (hdc, m_clrBackGround );
							}
							else
							{
								oldTxtColor_m = SetTextColor(hdc, cell.txtColor_m);
								oldBkColor_m  = SetBkColor  (hdc, cell.bkColor_m );
							}

				            DrawText(hdc, 
						             cell.text_tcp, -1,		// NULL Term
						             &text_rect,
						             cell.txtFmt_m);

                            SelectObject(hdc,oldFont_h);
				            SetTextColor(hdc,oldTxtColor_m);
			                SetBkColor(hdc,oldBkColor_m);
                        }
				    }

                    // Draw cell separators
                    HPEN oldPen_h = (HPEN)SelectObject(hdc,cell.rowPen_h);

    			    MoveToEx(hdc,cell_rect.left-1,cell_rect.bottom-1,NULL);
				    LineTo(hdc,cell_rect.right-1,cell_rect.bottom-1);

                    (HPEN)SelectObject(hdc,cell.colPen_h);
				    LineTo(hdc,cell_rect.right-1,cell_rect.top-1);

                    SelectObject(hdc,oldPen_h);

                    // has cursor range changed?
                    if ((cell.crsrOn_z != m_crsrOn_z) ||
                        (cell.crsrCol_i != m_crsrCol_i) ||
                        (cell.crsrRow_i != m_crsrRow_i) ||
                        (cell.colRng0_i != m_colRng0_i) ||
                        (cell.colRng1_i != m_colRng1_i) ||
                        (cell.rowRng0_i != m_rowRng0_i) ||
                        (cell.rowRng1_i != m_rowRng1_i))
                    {
				        InvalidateCursor();
                        m_crsrOn_z = cell.crsrOn_z;
                        m_crsrCol_i = cell.crsrCol_i;
                        m_crsrRow_i = cell.crsrRow_i;
				        m_colRng0_i = cell.colRng0_i;
				        m_rowRng0_i = cell.rowRng0_i;
				        m_colRng1_i = cell.colRng1_i;
				        m_rowRng1_i = cell.rowRng1_i;
				        InvalidateCursor();
                    }

                    // Cursor On and In Cursor Range?
                    if (m_crsrOn_z &&
                        (col_i >= min(m_colRng0_i,m_colRng1_i)) &&
                        (col_i <= max(m_colRng0_i,m_colRng1_i)) &&
                        (row_i >= min(m_rowRng0_i,m_rowRng1_i)) &&
                        (row_i <= max(m_rowRng0_i,m_rowRng1_i)))
                    {
                        UINT flags = 0;
                        RECT curs_rect = cell_rect;
                        curs_rect.right--;
                        curs_rect.bottom--;
                
                        if (row_i == min(m_rowRng0_i,m_rowRng1_i)) flags |= BF_TOP;
                        if (row_i == max(m_rowRng0_i,m_rowRng1_i)) flags |= BF_BOTTOM;
                        if (col_i == min(m_colRng0_i,m_colRng1_i)) flags |= BF_LEFT;
                        if (col_i == max(m_colRng0_i,m_colRng1_i)) flags |= BF_RIGHT;

						DrawEdge(hdc,&curs_rect,m_cursorStyle_n,flags);

                    }
					
					if ( m_cell3D_z )
					{
						RECT curs_rect = cell_rect;
						
                        curs_rect.right--;
                        curs_rect.bottom--;
						
						DrawEdge(hdc,&curs_rect, EDGE_RAISED, BF_ADJUST|BF_BOTTOMLEFT|BF_BOTTOMRIGHT);
					}

                    // Reset selected clipping region
			        cell_rgn = CreateRectRgn(0,0,m_xsize_i,m_ysize_i);
			        SelectClipRgn(hdc,cell_rgn);
			        DeleteObject(cell_rgn);
                }
			} // End while over x
        }
	} // End while over y

    EndPaint(m_grid_h, &ps) ;
} // ProcPaint

/******************************************************************************
 * ProcSize() - Handles Size Messages
 *****************************************************************************/

LRESULT CGridCtrl::ProcNotify(int ctrlId_i, NMHDR *nmhdr_p)
{
	BOOL	ret_val_z;

	if (ctrlId_i != IDC_HDR_ID ||
		nmhdr_p->code != HDN_BEGINTRACK) 
    {
        ret_val_z = DefWindowProc (m_grid_h, WM_NOTIFY, ctrlId_i, (long)nmhdr_p);
	}
	else
	{
		ret_val_z = TRUE;
    }

	return ret_val_z;
} // ProcNotify




/*****************************************************************************
 * SetGridSize() - sets the size of the grid
 *****************************************************************************/

void CGridCtrl::ConfigureScrlBars(HWND colScrl_h,
                                  HWND rowScrl_h,
                                  int colNullStyle_i,
                                  int rowNullStyle_i)
{
    // set up column scroll bar info
    m_colScrl_h = colScrl_h;
    m_colScrlNullStyle_i = colNullStyle_i;

    if (colScrl_h == m_grid_h)
        m_colScrlType_i = SB_HORZ;  // using grid windows default scrl bars
    else
        m_colScrlType_i = SB_CTL;   // using specific controls

    // set up vertical scroll bar info
    m_rowScrl_h = rowScrl_h;
    m_rowScrlNullStyle_i = rowNullStyle_i;

    if (rowScrl_h == m_grid_h)
        m_rowScrlType_i = SB_VERT;  // using grid windows default scrl bars
    else
        m_rowScrlType_i = SB_CTL;   // using specific controls 

    CalcScroll();
} // ConfigureScrlBars

/*****************************************************************************
 * ConfigureGrid() - sets the size of the grid
 *****************************************************************************/

void CGridCtrl::ConfigureGrid(
					int			numCols_i,
					int			numRows_i,
					const int	*colSizes_ip,
					const int	*rowSizes_ip,
                    int			paneCol_i,
					int			paneRow_i,
					BOOL		hasHdr_z,
					TCHAR *const *colNames_tc2p)
{
	SetColData(numCols_i, colSizes_ip, paneCol_i);
	SetRowData(numRows_i, rowSizes_ip, paneRow_i);
	SetHdrData(hasHdr_z, colNames_tc2p);

    CalcScroll();
    InvalidateRect(m_grid_h,NULL,TRUE);
} // ConfigureGrid

/*****************************************************************************
 * ConfigureCols() - sets the size of columns (similiar to ConfigureGrid)
 *****************************************************************************/

void CGridCtrl::ConfigureCols(int		numCols_i,
                              const int	*colSizes_ip,
                              int		paneCol_i)
{
	SetColData(numCols_i, colSizes_ip, paneCol_i);
	SetHdrData(m_hasHeader_z, NULL);

    CalcScroll();
    InvalidateRect(m_grid_h,NULL,TRUE);
} // ConfigureCols

/*****************************************************************************
 * ConfigureRows() - configures the rows (similiar to ConfigureGrid)
 *****************************************************************************/

void CGridCtrl::ConfigureRows(int		numRows_i,
                              const int	*rowSizes_ip,
                              int		paneRow_i)
{
	SetRowData(numRows_i, rowSizes_ip, paneRow_i);

    CalcScroll();
    InvalidateRect(m_grid_h,NULL,TRUE);
} // ConfigureRows

/*****************************************************************************
 * ConfigureHeader() - configures the Header (similiar to ConfigureGrid)
 *****************************************************************************/

void CGridCtrl::ConfigureHeader(BOOL			hasHeader_z,
							    TCHAR *const	*colNames_tc2p)
{
	SetHdrData(hasHeader_z, colNames_tc2p);

    CalcScroll();
    InvalidateRect(m_grid_h,NULL,TRUE);
} // ConfigureHeader

void CGridCtrl::ConfigureCursor(UINT CursorStyle)
{
	m_cursorStyle_n = CursorStyle;
}



/*****************************************************************************
 * SetColData() - Records Column configuration data
 *****************************************************************************/

void CGridCtrl::SetColData(int			numCols_i,
                           const int	*colSizes_ip,
                           int			paneCol_i)
{
	if (numCols_i != m_numCols_i)
	{
//
// If the number of columns has changed then the header is no longer valid
//
		FreeColNames();
		SetHdrData(FALSE, NULL);
	}

    m_numCols_i = numCols_i;
    m_paneCol_i = paneCol_i;

    // Set Columns Sizes if supplied
    if (colSizes_ip != NULL)
	{
        // free up old column sizes array
        if (m_colSizes_ip != NULL)
		{
			delete [] m_colSizes_ip;
		}

        m_colSizes_ip = new int[m_numCols_i];

		if (m_colSizes_ip != NULL)
		{
			memcpy((void*)m_colSizes_ip,
				   (void*)colSizes_ip,
				   sizeof(int)*m_numCols_i);
        }
	}
} // SetColData

/*****************************************************************************
 * SetRowData() - Records row configuration data
 *****************************************************************************/

void CGridCtrl::SetRowData(int			numRows_i,
                           const int	*rowSizes_ip,
                           int			paneRow_i)
{
    m_numRows_i = numRows_i;
	m_paneRow_i = paneRow_i;

    // Set Row Sizes if supplied
    if (rowSizes_ip != NULL)
	{
        // free up old column sizes array
        if (m_rowSizes_ip != NULL)
		{
			delete [] m_rowSizes_ip;
		}

        m_rowSizes_ip = new int[m_numRows_i];

		if (m_rowSizes_ip != NULL)
		{
			memcpy((void*)m_rowSizes_ip,
				   (void*)rowSizes_ip,
				   sizeof(int)*m_numRows_i);
        }
	}
} // SetRowData

/*****************************************************************************
 * SetHdrData() - Records Header configuration data
 *****************************************************************************/

void CGridCtrl::SetHdrData(BOOL			hasHeader_z,
						   TCHAR *const	*colNames_tc2p)
{
	int		index_i;

	m_hasHeader_z = hasHeader_z;

	if (!hasHeader_z && m_header_h != NULL)
	{
		DestroyWindow(m_header_h);
		m_header_h			= NULL;
		m_headerHeight_i	= 0;
	}

    // Set Column Names if supplied
    if (colNames_tc2p != NULL)
	{
		FreeColNames();

        m_colNames_tc2p = new TCHAR*[m_numCols_i];

		for (index_i = 0; index_i < m_numCols_i; index_i++)
		{
			if (colNames_tc2p[index_i] != NULL)
			{
				m_colNames_tc2p[index_i] = 
						new TCHAR[_tcslen(colNames_tc2p[index_i]) + 1];
				_tcscpy(m_colNames_tc2p[index_i], colNames_tc2p[index_i]);
			}
			else
			{
				m_colNames_tc2p[index_i] = NULL;
			}
		}
	}

	if (hasHeader_z && m_header_h == NULL)
	{
//
// Ensure that the common control DLL is loaded, and then create 
// the header control.
//
	    InitCommonControls();
//
// Create a header for the dialog, it will be moved and resized to sit
// above the grid later
//
		m_header_h = CreateWindowEx(
						0,						// dwExStyle
						WC_HEADER,				// lpClassName
						(LPCTSTR) NULL,			// lpWindowName
						WS_CHILD |
						HDS_BUTTONS |
						HDS_HORZ,				// dwStyle
						0,						// x
						0,						// y
						0,						// nWidth
						0,						// nHeight
						m_grid_h,				// hWndParent
						(HMENU) IDC_HDR_ID,		// hMenu
						v_inst_h,				// hInstance
						(LPVOID) NULL);			// lpParam
	}

    if (m_header_h != NULL)
	{
		HFONT		parent_font_h;
	    RECT		grid_client;
		HD_LAYOUT	hdl;
		WINDOWPOS	wp;
//
// Set the font in the header to that used by the grid's parent
//
		parent_font_h = (HFONT) ::SendMessage(m_parent_h,WM_GETFONT,0,0);

		::SendMessage(
				m_header_h,
				WM_SETFONT,
				(WPARAM) parent_font_h,
				MAKELPARAM(0,0));
// 
// Retrieve the bounding rectangle of the grid's
// client area, and then request size and position values
// from the header control.
//
		GetClientRect(m_grid_h, &grid_client);

		hdl.prc = &grid_client;
		hdl.pwpos = &wp; 

		if (!SendMessage(m_header_h, HDM_LAYOUT, 0, (LPARAM) &hdl))
		{
			DestroyWindow(m_header_h);
			m_header_h			= NULL;
			m_hasHeader_z		= FALSE;
			m_headerHeight_i	= 0;
		}
		else
		{
//
// Record the height of the header
//
			m_headerHeight_i	= wp.cy;
//
// Position header above the grid
//
			SetWindowPos(m_header_h, wp.hwndInsertAfter, wp.x, wp.y,
				wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW);
		} // HDM_LAYOUT
	} // create window (header)
//
// If there column names provided, discard existing names in header and use new ones
//
	if (m_header_h != NULL && m_colNames_tc2p != NULL)
	{
		int	items_i;

		items_i = (int) ::SendMessage(
							m_header_h,
							HDM_GETITEMCOUNT,
							(WPARAM) 0,
							MAKELPARAM(0,0));

		for (; items_i > 0; items_i--)
		{
			(void) ::SendMessage(
							m_header_h,
							HDM_DELETEITEM,
							(WPARAM) 0, // delete from start of header
							MAKELPARAM(0,0));
		}
//
// Insert the column headings in reverse order
//
		int		col_index_i;
		HD_ITEM	hdi; 

		for (col_index_i = m_numCols_i - 1; col_index_i >= 0; col_index_i--)
		{
			hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH; 
			hdi.pszText = m_colNames_tc2p[col_index_i];
			hdi.cxy = GetColWd(col_index_i); 
			hdi.cchTextMax = lstrlen(hdi.pszText); 
			hdi.fmt = HDF_LEFT | HDF_STRING; 

			(void) ::SendMessage(
							m_header_h,
							HDM_INSERTITEM,
							(WPARAM) 0, // insert at start of header
							(LPARAM) &hdi);
		}
	}
} // SetHdrData

/*****************************************************************************
 * FreeColNames() - Delete memory allocated to hold column names
 *****************************************************************************/

void CGridCtrl::FreeColNames()
{
	int	index_i;

	if (m_colNames_tc2p != NULL)
	{
		for (index_i = 0; index_i < m_numCols_i; index_i++)
		{
			if (m_colNames_tc2p[index_i] != NULL)
			{
				delete m_colNames_tc2p[index_i];
			}
		}

		delete [] m_colNames_tc2p;
	}
} // FreeColNames

/*****************************************************************************
 * SetCellMargins() - sets the margins of the grid
 *****************************************************************************/

void CGridCtrl::SetCellMargins(int top_i, int right_i, int bottom_i, int left_i)
{
    mrgTop_i    = top_i;
    mrgRight_i  = right_i;
    mrgBottom_i = bottom_i;
    mrgLeft_i   = left_i;
} // SetCellMargins

/*****************************************************************************
 * SetColScrlPos() - sets the x scroll pos for the grid
 *****************************************************************************/

void CGridCtrl::SetColScrlPos(int scrlpos_i)
{
    if (m_colScrlPos_i != scrlpos_i)
        {
        m_colScrlPos_i = scrlpos_i ;
        CalcLastCell();

        // if scroll bar present then alter it
        if (m_colScrl_h) SetColScrollBar();

        // tell parent window
        ::SendMessage(m_parent_h,
                      WM_GRD_CHG_COL_SCRL,
                      (WPARAM)m_grid_h,
                      (LPARAM)m_colScrlPos_i);

        InvalidateRect(m_grid_h, NULL, TRUE);
        }
} // SetColScrlPos




//===========================================================================
// Function name	: CGridCtrl::SetCursorRow
// Description	    : Moves the highlight cursor to a given row
// Return type		: void 
// Argument         : int iCursorRow
void CGridCtrl::SetCursorRow(int iCursorRow)
{
	int 
		iLastViewRow = GetViewPortRow( m_crsrRow_i ) + 1;
	int
		iRowsInView = NumRowsInView( );
	bool
		bRepos = false;
	//
	// Determine if the NEW row is in the current view
	// If it isn't then the screen needs to be repositioned 
	// around the row where possible.
	//
	if ( iCursorRow < m_crsrRow_i ) //Back
	{
		//if top row of view is >= new row ....
		if ( ( m_crsrRow_i - iLastViewRow ) >= iCursorRow )
		{
			bRepos = true;	
		}
	}
	else
	if ( iCursorRow > m_crsrRow_i ) //Forward
	{
		//
		if ( ( (iRowsInView - iLastViewRow ) + m_crsrRow_i ) < iCursorRow )
		{
			bRepos = true;	
		}
	}

	m_crsrRow_i = iCursorRow;

    SGrdCellInfo    cell;
	
	GetKeyStates( );    
	
	MakeDefaultCell(NULL,m_crsrCol_i,m_crsrRow_i,cell); // set up cell info
	//
	// If the new row is not in view, reposition
	//
	if ( bRepos )
	{
		int
			scroll_i = 0;

		scroll_i = iCursorRow - (iRowsInView / 2) ;

		if ( scroll_i < 0 )
		{
			scroll_i = 0;
		}
		else
		if ( (scroll_i + iRowsInView) > m_numRows_i )
		{
			scroll_i -= (scroll_i + iRowsInView) - m_numRows_i ;
		}

		SetRowScrlPos( scroll_i );
	}

    SetFocus(m_grid_h);

    switch(SendMessage(m_parent_h,WM_GRD_SGL_CLK,(WPARAM)m_grid_h,(LPARAM)&cell))
    {
		case GRD_CLK_UPDATE_CRSR :
			UpdateCursor(m_crsrCol_i,m_crsrRow_i);
			break;

		case GRD_CLK_EDIT_CELL :
			EditCell(m_crsrCol_i, m_crsrRow_i);
			break;

		default :
			// no action
			break;
    }
}

/*****************************************************************************
 * SetRowScrlPos() - sets the y scroll pos for the grid
 *****************************************************************************/

void CGridCtrl::SetRowScrlPos(int scrlpos_i)
{
    if (m_rowScrlPos_i != scrlpos_i)
        {
        m_rowScrlPos_i = scrlpos_i ;
        CalcLastCell();

        // if scroll bar present then alter it
        if (m_rowScrl_h) SetRowScrollBar();

        // tell parent window
        ::SendMessage(m_parent_h,
                      WM_GRD_CHG_ROW_SCRL,
                      (WPARAM)m_grid_h,
                      (LPARAM)m_rowScrlPos_i);
		// invalidate all grid except header
        InvalidateCells(0, 0, m_numCols_i, m_numRows_i);
        }
} // SetRowScrlPos

/*****************************************************************************
 * SetColScrollBar() - gets the y scroll pos for the grid
 * (Added by AJP 25th April 2000)
 *****************************************************************************/

int CGridCtrl::GetRowScrlPos()
{
	return m_rowScrlPos_i;
} // GetRowScrlPos

/*****************************************************************************
 * SetColScrollBar() - sets the x scroll bar for the grid
 *****************************************************************************/

void CGridCtrl::SetColScrollBar()
{
	SCROLLINFO scrlInfo;

	scrlInfo.cbSize = sizeof(SCROLLINFO);
	scrlInfo.fMask	= SIF_RANGE | SIF_POS | SIF_PAGE;
	scrlInfo.nMin	= 0;
	scrlInfo.nMax	= m_numCols_i - 1;
	scrlInfo.nPos	= m_colScrlPos_i;
	scrlInfo.nPage	= m_numCols_i - m_colScrlRng_i;

	if (m_colScrlNullStyle_i == GRD_SCRL_DSBL)
		scrlInfo.fMask |= SIF_DISABLENOSCROLL;

	SetScrollInfo(m_colScrl_h,m_colScrlType_i,&scrlInfo,TRUE);
} // SetColScrollBar

/*****************************************************************************
 * SetRowScrollBar() - sets the y scroll bar for the grid
 *****************************************************************************/

void CGridCtrl::SetRowScrollBar()
{
	SCROLLINFO scrlInfo;

	scrlInfo.cbSize = sizeof(SCROLLINFO);
	scrlInfo.fMask	= SIF_RANGE | SIF_POS | SIF_PAGE;
	scrlInfo.nMin	= 0;
	scrlInfo.nMax	= m_numRows_i - 1;
	scrlInfo.nPos	= m_rowScrlPos_i;
	scrlInfo.nPage	= m_numRows_i - m_rowScrlRng_i;

	if (m_rowScrlNullStyle_i == GRD_SCRL_DSBL)
		scrlInfo.fMask |= SIF_DISABLENOSCROLL;

	SetScrollInfo(m_rowScrl_h,m_rowScrlType_i,&scrlInfo,TRUE);
} // SetRowScrollBar

/*****************************************************************************
 * SetFont() - Set overall grid font, & get pixel sizes
 *****************************************************************************/

void CGridCtrl::SetFont(HFONT font_h)
{
    HDC	        hdc;
	TEXTMETRIC  tm;        
			
	hdc = GetDC(m_grid_h);

	m_font_h = font_h;
	SelectObject(hdc, font_h);

    GetTextMetrics(hdc,&tm) ;
    m_charWd_i    = tm.tmAveCharWidth;
	m_capCharWd_i = (tm.tmPitchAndFamily & 1 ? 3 : 2) * m_charWd_i / 2 ;
    m_charHt_i    = tm.tmHeight + tm.tmExternalLeading ;

	mrgLeft_i   = max(2,m_charWd_i/2);	// Cell margins
	mrgRight_i  = max(2,m_charWd_i/2);
	mrgTop_i    = max(2,m_charHt_i/8);
	mrgBottom_i = max(3,m_charHt_i/8);

    ReleaseDC(m_grid_h,hdc);

    CalcScroll();
    InvalidateRect(m_grid_h,NULL,TRUE);
} // SetFont

/*****************************************************************************
 * SetPen() - Set overall grid pen, for drawing cell separators
 *****************************************************************************/

void CGridCtrl::SetPen(HPEN pen_h)
{
    HDC hdc;
			
	hdc = GetDC(m_grid_h);

	m_pen_h = pen_h;

	SelectObject(hdc,pen_h);

    ReleaseDC(m_grid_h,hdc);

    InvalidateRect(m_grid_h,NULL,TRUE);
} // SetPen

/******************************************************************************
 * GetStrWd() - Determines width of given string in pixels
 *****************************************************************************/

long CGridCtrl::GetStrWd(TCHAR* string_tcp)
{
    SIZE size;

    GetTextExtentPoint32(GetDC(m_grid_h),
                         string_tcp,
                         _tcslen(string_tcp),
                         &size);
    return(size.cx);
} // GetStrWd

/******************************************************************************
 * GetStrHt() - Determines height of given string in pixles
 *****************************************************************************/

long CGridCtrl::GetStrHt(TCHAR* string_tcp)
{
    SIZE size;

    GetTextExtentPoint32(GetDC(m_grid_h),
                         string_tcp,
                         _tcslen(string_tcp),
                         &size);
    return(size.cy);
} // GetStrHt

/******************************************************************************
 * CalcLastCell() - Determines last cell displayed on the grid
 *****************************************************************************/

void CGridCtrl::CalcLastCell()
{
	int	col_i, row_i;
	
    // find last row outside the client area
	for(row_i = 0;((row_i < m_numRows_i) &&
				   (GetYPos(row_i) <= m_ysize_i+1));row_i++);

    // find the last x cell outside the client area
	for(col_i = 0;((col_i < m_numCols_i) && (GetXPos(col_i) <= m_xsize_i+1));col_i++);

	m_lastRow_i = min(m_numRows_i - 1, row_i - 1);
	m_lastCol_i = min(m_numCols_i - 1, col_i - 1);
} // CalcLastCell

/******************************************************************************
 * SetCursorState - Sets the cursor on/off
 *****************************************************************************/

void CGridCtrl::EnableCursor(BOOL crsrState_z)
{
    m_crsrOn_z = crsrState_z;
    UpdateCursor(m_crsrCol_i,m_crsrRow_i);
} // EnableCursor

/******************************************************************************
 * EnableKeys - Sets the cursor on/off
 *****************************************************************************/

void CGridCtrl::EnableKeys(BOOL keyState_z)
{
    m_keysOn_z = keyState_z;
} // EnableKeys

/******************************************************************************
 * UpdateCursor  - Routine to update cursor range
 *****************************************************************************/

void CGridCtrl::UpdateCursor(int col_i, int row_i)
{
    SGrdCellInfo cell;

    MakeDefaultCell(NULL,col_i,row_i,cell);

    GetKeyStates();
	
	if (!(m_keyState_i & GRD_CTRL_MASK))
    {
	    if (m_keyState_i & GRD_SHIFT_MASK)  
    	{
			cell.colRng1_i = cell.crsrCol_i = col_i;
			cell.rowRng1_i = cell.crsrRow_i = row_i;
    	}
    	else // no SHIFT down
		{
			cell.colRng0_i = cell.crsrCol_i = col_i;
			cell.rowRng0_i = cell.crsrRow_i = row_i;
			cell.colRng1_i = cell.crsrCol_i = col_i;
			cell.rowRng1_i = cell.crsrRow_i = row_i;
    	}
		//================================================================
		// AH. 27/04/98
		// Moved the following lines inside the CTRL key test, so that 
		// the parent is not notified in this case.
		//================================================================

		// Inform parent and update cursor if parent changes it
		SendMessage(m_parent_h,WM_GRD_CRSR_CHG,(WPARAM)m_grid_h,(LPARAM)&cell);

		// set visible cursor change flag
		BOOL change_z = ((cell.crsrOn_z != m_crsrOn_z)   ||
						 (cell.colRng0_i != m_colRng0_i) ||
						 (cell.colRng1_i != m_colRng1_i) ||
						 (cell.rowRng0_i != m_rowRng0_i) ||
						 (cell.rowRng1_i != m_rowRng1_i));

		if (change_z) 
		{
			InvalidateCursor();
		}
		
		m_crsrOn_z  = cell.crsrOn_z;
		m_crsrCol_i = cell.crsrCol_i;
		m_crsrRow_i = cell.crsrRow_i;
		m_colRng0_i = cell.colRng0_i;
		m_rowRng0_i = cell.rowRng0_i;
		m_colRng1_i = cell.colRng1_i;
		m_rowRng1_i = cell.rowRng1_i;

		if (change_z) 
		{
			InvalidateCursor();
		}

	}
} // UpdateCursor

/******************************************************************************
 * InvalidateCursor - invalidates the visible current cursor
 *****************************************************************************/

void CGridCtrl::InvalidateCursor()
{
	int	col_i;
	int	row_i;
	int	sizeCols_i;
	int	sizeRows_i;

    if (m_crsrOn_z)
    {
	    col_i = min(m_colRng0_i,m_colRng1_i);
	    row_i = min(m_rowRng0_i,m_rowRng1_i);
	    sizeCols_i = max(m_colRng0_i,m_colRng1_i) - col_i + 1;
        sizeRows_i = max(m_rowRng0_i,m_rowRng1_i) - row_i + 1;
		
    	InvalidateCells(col_i,row_i,sizeCols_i,sizeRows_i);
	}
} // InvalidateCursor

/******************************************************************************
 * InvalidateCells - invalidates the given area of cells
 *****************************************************************************/

void CGridCtrl::InvalidateCells(int col_i, int row_i, int sizeCols_i, int sizeRows_i)
{
	RECT rect;
		
    CalcCellRect(&rect,col_i,row_i,sizeCols_i,sizeRows_i);

	InvalidateRect(m_grid_h,&rect,TRUE);
} // InvalidateCells

/******************************************************************************
 * GetKeyState()- Get key state (CTRL, SHIFT)
 *****************************************************************************/

void CGridCtrl::GetKeyStates()
{
	m_keyState_i = (GetKeyState(VK_SHIFT) & 0x8000) ? GRD_SHIFT_MASK:0;
	m_keyState_i |= (GetKeyState(VK_CONTROL) & 0x8000) ? GRD_CTRL_MASK:0;
} // GetKeyStates


/******************************************************************************
 * CalcScroll() - Setup window scrollbars, after resize, row or column 
 *				- changes
 *****************************************************************************/

void CGridCtrl::CalcScroll()
{
	int  lp_i;				// Row/col loop counter
//
// set y scroll range
//
    // calculate scrollable range
    int scrArea_i = GetViewHt() - RowHtDiff(0,m_paneRow_i) + 1;

    // if last row is larger than the scrollable area
    if (GetRowHt(m_numRows_i-1) > scrArea_i)
        {
    	m_rowScrlRng_i = m_numRows_i - m_paneRow_i - 1;
        }
    else // find out how many rows are 'off the end' of the displayed area
        {
    	for (lp_i = m_numRows_i-1; lp_i >= m_paneRow_i; lp_i--)
	    	if (RowHtDiff(lp_i,m_numRows_i) > scrArea_i ) break;

        // total cells displayable - pane limit
	    m_rowScrlRng_i = lp_i + 1 - m_paneRow_i;
        }

    //Pick Minimum of Current Scroll Position and new scroll range
    m_rowScrlPos_i = min(m_rowScrlPos_i,m_rowScrlRng_i);

    if (m_rowScrl_h != NULL) // if there is a scroll bar
        {
        if (m_rowScrlRng_i != 0)
            {
            switch(m_rowScrlNullStyle_i)
                {
                case GRD_SCRL_HIDE :
                    ShowScrollBar(m_rowScrl_h,m_rowScrlType_i,TRUE);
                    break;

                case GRD_SCRL_DSBL :
                    ShowScrollBar(m_rowScrl_h,m_rowScrlType_i,TRUE);
                    EnableScrollBar(m_rowScrl_h,m_rowScrlType_i,ESB_ENABLE_BOTH);
                    break;

                default:
                    break;
                }
            }
        else
            {
            switch(m_rowScrlNullStyle_i)
                {
                case GRD_SCRL_HIDE :
                    ShowScrollBar(m_rowScrl_h,m_rowScrlType_i,FALSE);
                    break;

                case GRD_SCRL_DSBL :
                    ShowScrollBar(m_rowScrl_h,m_rowScrlType_i,TRUE);
                    EnableScrollBar(m_rowScrl_h,m_rowScrlType_i,ESB_DISABLE_BOTH);
                    break;

                default:
                    break;
                }
            }
		SetRowScrollBar();
        }

    // tell parent window about change in scroll position
    ::SendMessage(m_parent_h,
                  WM_GRD_CHG_ROW_SCRL,
                  (WPARAM)m_grid_h,
                  (LPARAM)m_rowScrlPos_i);

//
// set x scroll range
//
    // calculate scrollable area on screen
    scrArea_i = m_xsize_i - ColWdDiff(0,m_paneCol_i) + 1;

    // if last column is larger than the scrollable area
    if (GetColWd(m_numCols_i-1) > scrArea_i)
        {
        // total cells - pane limit - 1
    	m_colScrlRng_i = m_numCols_i - m_paneCol_i - 1;
        }
    else // find out how many columns are 'off the end' of the displayed area
        {
	    for (lp_i = m_numCols_i-1; lp_i >= m_paneCol_i; lp_i--)
		    if ( ColWdDiff(lp_i,m_numCols_i) > scrArea_i ) break;

        // total cells displayable - pane limit
	    m_colScrlRng_i = lp_i + 1 - m_paneCol_i;
        }

    // Pick Minimum of Current Scroll Position and new scroll range
    m_colScrlPos_i = min(m_colScrlPos_i,m_colScrlRng_i);

	CalcLastCell();


    if (m_colScrl_h != NULL) // if there is a scroll bar
        {
        
		
		if (m_colScrlRng_i != 0)
            {
            switch(m_colScrlNullStyle_i)
                {
                case GRD_SCRL_HIDE :
                    ShowScrollBar(m_colScrl_h,m_colScrlType_i,TRUE);
                    break;

                case GRD_SCRL_DSBL :
                    ShowScrollBar(m_colScrl_h,m_colScrlType_i,TRUE);
                    EnableScrollBar(m_colScrl_h,m_colScrlType_i,ESB_ENABLE_BOTH);
                    break;

                default:
                    break;
                }
            }
        else
            {
            switch(m_colScrlNullStyle_i)
                {
                case GRD_SCRL_HIDE :
                    ShowScrollBar(m_colScrl_h,m_colScrlType_i,FALSE);
                    break;

                case GRD_SCRL_DSBL :
                    ShowScrollBar(m_colScrl_h,m_colScrlType_i,TRUE);
                    EnableScrollBar(m_colScrl_h,m_colScrlType_i,ESB_DISABLE_BOTH);
                    break;

                default:
                    break;
                }
            }
		SetColScrollBar();
        }

    // tell parent window about change in scroll position
    ::SendMessage(m_parent_h,
                  WM_GRD_CHG_COL_SCRL,
                  (WPARAM)m_grid_h,
                  (LPARAM)m_colScrlPos_i);
} // CalcScroll


/******************************************************************************
 * GetRowPos() - determines the row position of a given y coordinate
 *****************************************************************************/

int CGridCtrl::GetRowPos(int ycoord_i) const
{
    int row_i;

    // if the coordinate is in the grid area then find the column
    if (ycoord_i <= (GetYPos(m_numRows_i-1)+GetRowHt(m_numRows_i-1)))
        {
        for(row_i = 0;
            (row_i < m_numRows_i) && (ycoord_i > GetYPos(row_i));
            row_i++);
        row_i--;
        }
    else
        {
        row_i = -1;     // signified outside of grid
        }

	return(row_i);
} // GetRowPos


/******************************************************************************
 * GetColPos() - determines the column position of a given x coordinate
 *****************************************************************************/

int CGridCtrl::GetColPos(int xcoord_i) const
{
    int col_i;

    // if the coordinate is in the grid area then find the column
    if (xcoord_i <= (GetXPos(m_numCols_i-1)+GetColWd(m_numCols_i-1)))
        {
        for(col_i = 0;
            (col_i < m_numCols_i) && (xcoord_i > GetXPos(col_i));
            col_i++);
        col_i--;
        }
    else
        {
        col_i = -1;    // signifies not in grid
        }

	return(col_i);
} // GetColPos


/******************************************************************************
 * RowHtDiff() - returns difference in pixels between start of cell y0 and
 * start of cell y1
 *****************************************************************************/

int CGridCtrl::RowHtDiff(int row0_i, int row1_i) const
{
	int	    row_i;
	int	    result_i = 0;
    BOOL    negate_z = FALSE;

    if (row0_i > row1_i)
        {
        int temp = row0_i;
        row0_i = row1_i;
        row1_i = temp;
        negate_z = TRUE;
        }

	for (row_i = row0_i; row_i < row1_i; row_i++)
		{
		result_i += GetRowHt(row_i);
		}

    if (negate_z) result_i = -result_i;

    return(result_i);
} // RowHtDiff


/******************************************************************************
 * ColWdDiff() - returns difference in pixels between start of col0 and start 
 *             - start of col1
 *****************************************************************************/

int CGridCtrl::ColWdDiff(int col0_i, int col1_i) const
{
	int	 col_i;
	int	 result_i = 0;
    BOOL negate_z = FALSE;
    
    if (col0_i > col1_i)
        {
        int temp = col0_i;
        col0_i = col1_i;
        col1_i = temp;
        negate_z = TRUE;
        }

    for (col_i = col0_i; col_i < col1_i; col_i++)
        {
		result_i += GetColWd(col_i);
        }

    if (negate_z) result_i = -result_i;

    return(result_i);
} // ColWdDiff


/******************************************************************************
 * GetColWd() - returns the width of column in pixels
 *****************************************************************************/

int CGridCtrl::GetColWd(int col_i) const
{
	int	left_i;
	int	right_i;
    int wd_i;

	if (m_colSizes_ip == NULL)
        {
		left_i = (col_i * (m_xsize_i + 1)) / m_numCols_i;
		right_i = (++col_i * (m_xsize_i + 1)) / m_numCols_i;
		wd_i = right_i - left_i;
        }
	else
		wd_i = m_colSizes_ip[col_i];

    return(wd_i);
} // GetColWd


/******************************************************************************
 * GetRowHt() - returns the height of cell y0
 *****************************************************************************/

int CGridCtrl::GetRowHt(int row_i) const
{
    int ht_i;

	if (m_rowSizes_ip == NULL)
		ht_i = (m_charHt_i + 1 + mrgTop_i + mrgBottom_i);
	else
		ht_i = m_rowSizes_ip[row_i];

    return(ht_i);
} // GetRowHt

/******************************************************************************
 * GetRowHt() - returns the height of cell viewing area (excludes the header)
 *****************************************************************************/

int CGridCtrl::GetViewHt() const
{
    return(m_ysize_i - m_headerHeight_i);
} // GetViewHt

/******************************************************************************
 * GetXPos - determines the start position of a given column
 *****************************************************************************/

int CGridCtrl::GetXPos(int col_i) const
{
    int   xpos_i;

    // select x cell outside pane area
    if (col_i < m_paneCol_i)
        {
    	xpos_i = ColWdDiff(0,col_i);
        }
    else // if in pane area
        {
        xpos_i = ColWdDiff(0,m_paneCol_i) + 
                 ColWdDiff(m_colScrlPos_i + m_paneCol_i, col_i);
        }
	return (xpos_i);
} // GetXPos


/******************************************************************************
 * GetYPos - determines the start position of a given row in the client area
 *****************************************************************************/

int CGridCtrl::GetYPos(int row_i) const
{
    int   ypos_i;

    // if y cell outside pane area
    if (row_i < m_paneRow_i)
        {
    	ypos_i = RowHtDiff(0,row_i);
        }
    else // if in pane area
        {
   		ypos_i = RowHtDiff(0,m_paneRow_i) + 
                 RowHtDiff(m_rowScrlPos_i + m_paneRow_i, row_i);
        }

	ypos_i += m_headerHeight_i;

	return (ypos_i);
} // GetYPos

/******************************************************************************
 * MakeDefaultCell - sets up a default cell structure
 *****************************************************************************/

void CGridCtrl::MakeDefaultCell(HDC hdc, int col_i, int row_i, SGrdCellInfo &cell)
{
    // Set up cell information
	cell.col_i	     = col_i;
	cell.row_i	     = row_i;
    cell.hdc         = hdc;
	cell.keyState_i	 = m_keyState_i;		
	cell.crsrOn_z    = m_crsrOn_z;
	cell.cell3D_z    = m_cell3D_z;
	cell.crsrCol_i	 = m_crsrCol_i;    
	cell.crsrRow_i	 = m_crsrRow_i;		
	cell.colRng0_i	 = m_colRng0_i;
	cell.rowRng0_i	 = m_rowRng0_i;
	cell.colRng1_i	 = m_colRng1_i;
	cell.rowRng1_i	 = m_rowRng1_i;

	cell.font_h      = m_font_h;
	cell.colPen_h    = m_pen_h;
    cell.rowPen_h    = m_pen_h;
    
	if (hdc != NULL)
	{
		cell.bkColor_m   = GetBkColor(hdc);
		cell.txtColor_m  = GetTextColor(hdc);
	}

	cell.txtFmt_m    = DT_NOCLIP | DT_EXTERNALLEADING | DT_WORDBREAK;
	cell.owner_z     = FALSE;
	cell.cell3D_z    = FALSE;
	cell.text_tcp    = _T("");
	cell.sizeCols_i  = 1;
	cell.sizeRows_i  = 1;
	cell.RowInView   = GetViewPortRow( row_i );
	cell.dwUserData  = GetUserData( row_i );

	cell.fwKeys		  = m_fwKeys;
} // MakeDefaultCell

/******************************************************************************
 * CalcCellRect - determines bounding rectangle given cell information
 *****************************************************************************/

void CGridCtrl::CalcCellRect(RECT *rect_p,
                             int col_i, int row_i, int sizeCols_i, int sizeRows_i)
{
	rect_p->left = rect_p->right = GetXPos(col_i);
    rect_p->right += ColWdDiff(col_i,col_i + sizeCols_i);

	rect_p->top	= rect_p->bottom = GetYPos(row_i);
    rect_p->bottom += RowHtDiff(row_i,row_i + sizeRows_i);

	if (rect_p->top < m_headerHeight_i)
	{
		rect_p->top = m_headerHeight_i;
	}
} // CalcCellRect

/******************************************************************************
 * CalcTextRect - determines bounding text rectangle given a cell rectangle
 *****************************************************************************/

void CGridCtrl::CalcTextRect(RECT *cellRect_p, RECT *textRect_p)
{
	textRect_p->left   = cellRect_p->left   + mrgLeft_i;
	textRect_p->top    = cellRect_p->top    + mrgTop_i;
	textRect_p->right  = cellRect_p->right  - mrgRight_i;
	textRect_p->bottom = cellRect_p->bottom - mrgBottom_i;
} // CalcTextRect

///////////////////////////////////////////////////////////////////////////////
// Function Description : Adjust the height of the grid to display a whole
//						: number of rows. All rows are assumed to be of the
//						: same height. The position of the top of the grid
//						: remains unchanged
// Author               : Duncan Olesen
// Date Created         : 11/12/96
// Date Last Modified   : 11/12/96
///////////////////////////////////////////////////////////////////////////////

void CGridCtrl::NormaliseHeight(T_NormaliseMode	p_mode)
{
	int		row_height_i;
	RECT	grid_window;
	POINT	window_pos;
	int		num_rows_i;
	int		client_height_i;
	int		new_client_height_i;
	int		new_window_height_i;
	BOOL	bOk;

	row_height_i = GetRowHt(0);

//
// Determine the coordinates of the grid relative
//
	bOk = GetWindowRect(m_grid_h, &grid_window);

	if (bOk)
	{
		if (m_parent_h != NULL)
		{
//
// As the grid is a child window, the coordinates need to be converted from
// screen to client
//
			window_pos.x = grid_window.left;
			window_pos.y = grid_window.top;

			bOk = ScreenToClient(
						m_parent_h,
						&window_pos);

			if (bOk)
			{
				grid_window.right	-= grid_window.left - window_pos.x;
				grid_window.bottom	-= grid_window.top - window_pos.y;
				grid_window.left	 = window_pos.x;
				grid_window.top		 = window_pos.y;
			}
		} // grid is a child
	}
//
// Calculate the nearest number of rows that will fit into the grid and
// shrink or enlarge the grid accordingly
//
	if (bOk)
	{
//
// Calculate the height required to display a whole number of rows
//
		client_height_i = GetViewHt();
		num_rows_i = client_height_i / row_height_i;
		new_client_height_i = num_rows_i * row_height_i;
//
// Adjust the height according to the normalisation mode if necessary
//
		switch (p_mode)
		{
		case NM_NEAREST:
			if (client_height_i - new_client_height_i >
				row_height_i / 2)
			{
				new_client_height_i += row_height_i;
			}
			break;

		case NM_EXPAND:
			if (client_height_i != new_client_height_i)
			{
				new_client_height_i += row_height_i;
			}
			break;

		case NM_SHRINK:
			if (client_height_i != new_client_height_i)
			{
				new_client_height_i -= row_height_i;
			}
			break;
		} // switch on mode
//
// Calculate new height. One is subtracted so that the divider at the
// bottom of the last visible row is not visible
//
		new_window_height_i = grid_window.bottom - grid_window.top + 
							  new_client_height_i - client_height_i - 1;
//
// Resize and repaint the grid
//
		bOk = MoveWindow(
					m_grid_h,
					(int) grid_window.left,
					(int) grid_window.top,
					(int) (grid_window.right - grid_window.left),
					new_window_height_i,
					TRUE);
	}
} // NormaliseHeight

/******************************************************************************/

BOOL CGridCtrl::IsVScrollVisible()
{
	return (m_rowScrlRng_i > 0);
	
}

BOOL CGridCtrl::IsHScrollVisible()
{
	return (m_colScrlRng_i > 0);
}

//=====================================================================
// Function		: GetViewPortRow
// Parameters	: iRow - Absolute Row Number.
// Author		: Andrew Hill
// Description  : Calculates the row number in the viewable area
int CGridCtrl::GetViewPortRow( int iRow )
{
	int 
		iTopY,
		iRowInView;

	iTopY = GetYPos( iRow );

	if (iTopY < m_headerHeight_i)
	{
		iTopY = m_headerHeight_i;
	}

	iRowInView = iTopY / GetRowHt( iRow );

	return iRowInView;
}




//===========================================================================
// Function name	: CGridCtrl::NumRowsInView
// Return type		: int 
// Description	    : Returns the number of rows in the view.
//					  NOTE : Will only work with equal sized rows.
int CGridCtrl::NumRowsInView()
{
	int 
		iRows = GetViewHt() / GetRowHt( 0 );

	return iRows;
}


//===========================================================================
// Function name	: CGridCtrl::SetRowData
// Return type		: void 
// Argument         : int iRow
// Argument         : int iFlag
// Description	    : Stores a flag value for a given row
void CGridCtrl::SetRowFlag(int iRow, int iFlag)
{
	ASSERT( iRow <= m_numRows_i);
	ASSERT( iFlag <= 31 );

	DWORD
		dwData,
		dwNewData = 1 << iFlag;

	if ( m_UserDataMap.Lookup(iRow, dwData) == 0)
	{
		dwData = dwNewData;
	}
	else
	{
		dwData |= dwNewData;
	}

	m_UserDataMap.SetAt(iRow, dwData);

	InvalidateCells(0, iRow, m_numCols_i);
}


//===========================================================================
// Function name	: CGridCtrl::ClearRowData
// Return type		: void 
// Argument         : int iRow
// Argument         : int iFlag
// Description	    : Clears a flag value for a given row
void CGridCtrl::ClearRowFlag(int iRow, int iFlag)
{
	DWORD
		dwData,
		dwNewData = 1 << iFlag;

	if ( m_UserDataMap.Lookup(iRow, dwData) == 0 )
	{
		//No data to clear!
	}
	else
	{
		dwData &= ~dwNewData;

		if ( dwData == 0 )
		{
			m_UserDataMap.RemoveKey( iRow );
		}
		else
		{
			m_UserDataMap.SetAt(iRow, dwData);
		}

	}


	InvalidateCells(0, iRow, m_numCols_i);
}

//===========================================================================
// Function name	:	IsRowFlagged
// Return type		:	boolean true/false
// Argument         :	row number to test
// Description	    :	tests to see if a row actually has data in it
bool CGridCtrl::IsRowFlagged( int iRow )
{
	DWORD
		dwData;
	bool
		bFlagged;

	if ( m_UserDataMap.Lookup(iRow, dwData) == 0 )
	{
		bFlagged = false;
	}
	else
	{
		bFlagged = ( dwData != 0 );
	}
		
	return bFlagged;
}

//===========================================================================
// Function name	: CGridCtrl::GetUserData
// Return type		: DWORD 
// Argument         : int iRow
// Description	    : Fetches the data to pass to caller
DWORD CGridCtrl::GetUserData(int iRow)
{
	DWORD
		dwData = 0;
	
	m_UserDataMap.Lookup(iRow, dwData);

	return dwData;
}


//===========================================================================
// Function name	: CGridCtrl::SetHighlightBlock
// Return type		: void 
// Argument         : int iStart
// Argument         : int iEnd
// Description	    : Sets the number of rows selected 
void CGridCtrl::SetHighlightBlock(int iStart, int iEnd)
{
	m_rowRng0_i = iStart;
	m_rowRng1_i = iEnd;
}

//===============================================================================
// Function name	: CGridCtrl::InBlock
// Return type		: BOOL 		-
// Author           : Andy Hill 06/10/98 1:40:51 pm
// Argument         : int iRow		- Row number
// Argument         : int iCol		- Column number
// Description	    : Returns whether a cell is in the selected block but is 
//					  NOT the cursor row.
BOOL CGridCtrl::InBlock(int iRow, int iCol)
{
	if (m_crsrOn_z &&
		(iCol >= min(m_colRng0_i,m_colRng1_i)) &&
        (iCol <= max(m_colRng0_i,m_colRng1_i)) &&
        (iRow >= min(m_rowRng0_i,m_rowRng1_i)) &&
        (iRow <= max(m_rowRng0_i,m_rowRng1_i)) && 
		(iRow != m_crsrRow_i))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


//===============================================================================
// Function name	: CGridCtrl::CursorColour
// Return type		: void 		-
// Author           : Andy Hill 06/10/98 2:19:26 pm
// Argument         : COLORREF clrBack		-
// Argument         : COLORREF clrFore		-
// Description	    : 
//
void CGridCtrl::CursorColour(COLORREF clrBack, COLORREF clrFore)
{
	m_clrBackGround = clrBack;
	m_clrForeGround = clrFore;
	
	m_bUseBlockColouring = TRUE;
}



//=============================================================================
// Function name	: CGridCtrl::SetGridBkColor
// Return type		: void 
// Argument         : COLORREF clrBack
// Author           : AH
// Description	    : Sets the grid background exposed area
void CGridCtrl::SetGridBkColor(COLORREF clrBack)
{
	m_BackColour = clrBack;
}


//=============================================================================
// Function name	: CGridCtrl::OnEraseBackground
// Return type		: void 
// Argument         : HDC hDC
// Author           : AH
// Description	    : Override for the WM_ERASEBKGND message so that custom 
//						backcolours can be chosen. 
void CGridCtrl::OnEraseBackground( HDC hDC )
{
	//
	// If the scroll bar is not present then erase the background
	//
	if ( !IsVScrollVisible() )
	{

		CDC
			cDC;

		cDC.Attach( hDC );

		// Set brush to desired background color.
		CBrush 
			backBrush( m_BackColour );    
		// Save old brush.
		CBrush* 
			pOldBrush = cDC.SelectObject(&backBrush);    
		CRect 
			rect;

		int
			iGridRows = m_numRows_i - m_rowScrlRng_i ,
			iTop  = iGridRows * GetRowHt( 0 ),
			iBott = GetViewHt( );
		//====================================
		// Co-ordinates of the exposed area
		//====================================
		rect.left   = 0;
		rect.top	= iTop;
		rect.bottom = iBott;
		rect.right  = m_xsize_i; 

		cDC.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    
		cDC.SelectObject(pOldBrush);    

		cDC.Detach( );
	}
}
