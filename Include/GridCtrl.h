/******************************************************************************
 * Project     : Deacon Prototype
 * File        : grd_ctrl.h
 * Author      : Paul Brain
 * Date        : 12/6/96
 *
 * Description : Class registration and WinProc and class for custom grid ctrl
 *****************************************************************************/

#ifndef _GRD_CTRL_H
#define _GRD_CTRL_H

#include <gclibapi.h>
#include <gc_cell_info.h>
#include <afxtempl.h>
/******************************************************************************
 * Macros/Literals
 *****************************************************************************/

#define GRD_CLASSNAME   _T("BPE_GRID")     // Class Name

#define GRD_DATAIDX	0		// Index for Set/GetWindowLong

// User Defined Messages - sent to parent window from custom grid
#define WM_GRD_GET_INFO         WM_USER+1001        // Requests cell info from parent
#define WM_GRD_CHG_COL_SCRL     WM_USER+1002        // Change of col scroll position
#define WM_GRD_CHG_ROW_SCRL     WM_USER+1003        // Change of row scroll position
#define WM_GRD_SGL_CLK          WM_USER+1004        // Single Click
#define WM_GRD_DBL_CLK          WM_USER+1005        // Double Click
#define WM_GRD_CRSR_CHG         WM_USER+1006        // Change in Cursor Position
#define WM_GRD_EDIT_CELL        WM_USER+1007        // Edit Cell
#define WM_GRD_KILLFOCUS        WM_USER+1008        // Kill Focus from Edit Control
#define WM_GRD_LBUTTONUP        WM_USER+1009
#define WM_GRD_MOUSEMOVE		WM_USER+1010
#define WM_GRD_LOSTFOCUS		WM_USER+1011		// Kill focus from grid
#define WM_GRD_SETFOCUS			WM_USER+1012		// Set focus to grid
#define WM_GRD_RBUTTON_CLK		WM_USER+1013

//
// Timer ID's
//
#define CHECK_MOUSE				1

// User Defined Messages - sent to custom grid from parent window
#define WM_GRD_GET_CLASS        WM_USER+2000       // Gets pointer to class

// Other Literals
#define GRD_SHIFT_MASK	1
#define GRD_CTRL_MASK	2	// Grid Key Masks

// Styles for scroll bars when the range is zero (i.e. not in use)
#define GRD_SCRL_HIDE   0   // Hide
#define GRD_SCRL_DSBL   1   // Disable
//#define GRD_SCRL_SHOW   2   // Show Always

// Literals returned from Single & Double Click Msgs to determine what
// action to take
#define GRD_CLK_NO_ACTION   0   // No Action to take
#define GRD_CLK_UPDATE_CRSR 1   // Update the cursor
#define GRD_CLK_EDIT_CELL   2   // Edit Cell

/******************************************************************************
 * Structures
 *****************************************************************************/

GCLIBAPI LRESULT CALLBACK grd_winproc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
GCLIBAPI BOOL grd_register(HINSTANCE p_inst_h);

enum T_NormaliseMode
{
	NM_NEAREST,		// expand or shrink according to which is the smaller change
	NM_EXPAND,		// Increase size if a change is necessary
	NM_SHRINK		// Decrease size if a change is necessary
};
//
// Main Grid Control Class
//
class CGridCtrl
	{
public :
	
//
// Public Functions used to configure/control grid
//
    GCLIBAPI void ConfigureGrid(
					int			numCols_i		= 10,
					int			numRows_i		= 10,
					const int	*colSizes_ip	= NULL,
					const int	*rowSizes_ip	= NULL,
                    int			paneCol_i		= 0,
					int			paneRow_i		= 0,
					BOOL		hasHdr_z		= FALSE,
					TCHAR *const *colNames_tc2p	= NULL);

    GCLIBAPI void ConfigureCols(int			numCols_i		= 10,
								const int	*colSizes_ip	= NULL,
								int			paneCol_i		= 0);
	
    GCLIBAPI void ConfigureRows(int			numRows_i		= 10,
                                const int	*rowSizes_ip	= NULL,
                                int			paneRow_i		= 0);

	GCLIBAPI void ConfigureScrlBars(HWND colScrl_h      = NULL,
                                    HWND rowScrl_h      = NULL,
                                    int  colNullStyle_i = GRD_SCRL_HIDE,
                                    int  rowNullStyle_i = GRD_SCRL_HIDE);
	GCLIBAPI BOOL IsHScrollVisible();
	GCLIBAPI BOOL IsVScrollVisible();

	GCLIBAPI void ConfigureHeader(BOOL			hasHeader_z		= FALSE,
								  TCHAR *const	*colNames_tc2p	= NULL);
	GCLIBAPI void ConfigureCursor(UINT CursorStyle);
	GCLIBAPI void CursorColour(COLORREF clrBack, COLORREF clrFore);
	GCLIBAPI void NormaliseHeight(T_NormaliseMode	p_mode = NM_NEAREST);

    GCLIBAPI void SetColScrlPos(int scrlpos_i);     // set col scroll position
    GCLIBAPI void SetRowScrlPos(int scrlpos_i);     // set row scroll position

    GCLIBAPI void SetCellMargins(int top_i, int right_i, int bottom_i, int left_i);

    GCLIBAPI void SetFont(HFONT font_h);            // set font for grid

    GCLIBAPI void SetPen(HPEN pen_h);               // set pen for grid

	GCLIBAPI void SetGridBkColor(COLORREF clrBack);     // set background colour

    GCLIBAPI void EnableCursor(BOOL crsrState_z);   // enables/disables the cursor
    GCLIBAPI void EnableKeys(BOOL keyState_z);      // enables/disables key hits

    GCLIBAPI long GetStrWd(TCHAR* string_tcp);      // gets string width in pixles
    GCLIBAPI long GetStrHt(TCHAR* string_tcp);      // gets string height in pixles

    GCLIBAPI int GetColWd(int col_i) const;         // get wd of col
    GCLIBAPI int GetRowHt(int row_i) const;         // get ht of row

	GCLIBAPI int GetRowScrlPos();					// Gets the row scroll position

    GCLIBAPI void CalcCellRect(RECT *rect_p,
                               int col_i, int row_i,
                               int sizeCols_i = 1, int sizeRows_i = 1);
                                                    // calc bounding rectangle of cell

    GCLIBAPI void CalcTextRect(RECT *cellRect_p, RECT *textRect_p);
                                                    // calc text rect from cell rect

    GCLIBAPI void InvalidateCells(int col_i, int row_i, int sizeCols_i = 1, int sizeRows_i = 1);
                                        // invalidates specified cells area

	GCLIBAPI void SetCursorRow(int iCursorRow);

	GCLIBAPI void ClearRowFlag(int iRow, int iFlag);

	GCLIBAPI void SetRowFlag(int iRow, int iFlag);
	GCLIBAPI bool IsRowFlagged( int iRow );

	GCLIBAPI void SetHighlightBlock(int iStart, int iEnd);
private:
		void OnEraseBackground(HDC hDC);
	BOOL	InBlock( int iRow, int iCol );
	void	FocusFunction( UINT iMsg );
		
//
// Private Class Data
//
	CMap	<int,int,DWORD,DWORD&> m_UserDataMap;
	COLORREF m_clrBackGround;
	COLORREF m_clrForeGround;
	BOOL	m_bUseBlockColouring;
	DWORD	m_dwUserData;
	WORD	m_fwKeys;

	int		m_iLastMouseRow;
	int		m_iRowInView;
	BOOL	m_bMouseKeyDown;
	
    HWND    m_parent_h;         // windows handle of parent of grid
	HWND	m_grid_h;           // windows handle for this grid

	HPEN	m_pen_h;            // pen for drawing grid line
	HFONT	m_font_h;           // grid font

	int		m_charWd_i;         // font character width
	int		m_capCharWd_i;      // font capital character width
	int		m_charHt_i;			// font character height

	int		m_numCols_i;        // Number of rows in grid
	int		m_numRows_i;		// Number of columns in grid

	int		m_xsize_i;          // x size of drawing area
	int		m_ysize_i;          // y size of drawing area

    int     m_paneCol_i;        // pane column start position
    int     m_paneRow_i;        // pane row start position

	int		m_colScrlRng_i;     // column scroll range
	int		m_colScrlPos_i;     // column scroll position
	int		m_lastCol_i;		// last column displayed

	int		m_rowScrlRng_i;     // row scroll range
	int		m_rowScrlPos_i;		// Scroll Ranges and positions
	int		m_lastRow_i;		// last row displayed

	int		m_keyState_i;		// GRD_SHIFT_MASK, GRD_CTRL_MASK
    BOOL    m_keysOn_z;         // keystrokes allowed to ctrl cursor
	BOOL	m_crsrOn_z;	    	// Cursor off flag
	BOOL	m_cell3D_z;			// 3D CellEffect;
	UINT	m_cursorStyle_n;	// cursor style
	
	int		m_crsrCol_i;        // cursor column
	int		m_crsrRow_i;		// cursor row
	int		m_colRng0_i;		// highlighted range of cursor
	int		m_rowRng0_i;
	int		m_colRng1_i;
	int		m_rowRng1_i;

    HWND    m_colScrl_h;        // horizontal scroll bar handle
    int     m_colScrlType_i;    // horizontal scroll bar type
    int     m_colScrlNullStyle_i;  // horizontal scroll bar null style
    HWND    m_rowScrl_h;        // vertical scroll bar handle
    int     m_rowScrlType_i;    // vertical scroll bar type
    int     m_rowScrlNullStyle_i;  // vertical scroll bar null style

	int*	m_colSizes_ip;		// array of column sizes (NULL=>divide up client area equally)
	int*	m_rowSizes_ip;		// array of row sizes (NULL => m_charHt_i+margins+1)

    int     mrgLeft_i;          // cell margin left
    int	    mrgRight_i;         // cell margin right
    int	    mrgTop_i;           // cell margin top
    int	    mrgBottom_i;        // cell margin bottom ( Def = char_x /4 )

    CRichEditCtrl* m_cellEdit_p;// ptr to edit control
    int     m_editCol_i;        // edit column
    int     m_editRow_i;        // edit row

	BOOL	m_hasHeader_z;		// Grid has a header
	int		m_headerHeight_i;	// Height of the header
	HWND	m_header_h;			// Handle to the header control
	TCHAR	**m_colNames_tc2p;	// Array of pointers to column names

	COLORREF m_BackColour;
//
// Contruction / Destruction (private)
//

    CGridCtrl(HWND grid_h);				// constructor
    ~CGridCtrl();                       // destructor

//
// Friend Function that handles windows messages
//
    friend GCLIBAPI LRESULT CALLBACK grd_winproc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//
// Private Functions used to handle windows messages
//
	void ProcRButtonUp(int xcoord_i, int ycoord_i);
	void ProcRButtonDown(int xcoord_i, int ycoord_i);
    void ProcSize(int xcoord_i, int ycoord_i);
    void ProcLButtonDblClick(int xcoord_i, int ycoord_i);
    void ProcLButtonDown(int xcoord_i, int ycoord_i);
	void ProcLButtonUp(int xcoord_i, int ycoord_i);
    void ProcVScroll(int action_i, int pos_i);
	void MouseMove(WPARAM wParam, LPARAM lParam);
    void ProcHScroll(int action_i, int pos_i);
    void ProcKeyDown(int key_i);
    void ProcPaint();
    LRESULT ProcNotify(int ctrlId_i, NMHDR *nmhdr_p);
    void ProcCommand(WPARAM wParam, LPARAM lParam);
	void CheckMouse( );	
	int	 GetViewPortRow( int iRow );

//
// Private Functions used to configure the grid
//
    void SetColData(int			numCols_i,
					const int	*colSizes_ip,
					int			paneCol_i);
	
    void SetRowData(int			numRows_i,
                    const int	*rowSizes_ip,
                    int			paneRow_i);

	void SetHdrData(BOOL			has_header_z,
					TCHAR *const	*colNames_tc2p);

	void FreeColNames();
//
// Private Functions used to control the grid
//
    void EditCell(int col_i, int row_i);
                                        // Creates an edit control over a cell

    void HandleClick(int type_i, int xcoord_i, int ycoord_i);
                                        // Handle a button click of certain type

    void CalcLastCell();	            // Calculate last cell displayed

    void UpdateCursor(int col_i, int row_i);
                                        // Update Cursor Range

    void GetKeyStates();                // get key states

    void CalcScroll();                  // calculate scroll bars positions

	void SetRowScrollBar();				// sets up the row scroll bar
	void SetColScrollBar();				// sets up the row scroll bar

    void InvalidateCursor();            // invalidates the cursor area

    void MakeDefaultCell(HDC hdc, int col_i, int row_i, SGrdCellInfo &cell);
                                        // sets up a default cell structure

    int GetRowPos(int ycoord_i) const;	// convert y-coord to row pos    
    int GetColPos(int xcoord_i) const;	// convert x-coord to col pos

	int GetViewHt() const;				// calculate height of cell view area

    int GetXPos(int col_i) const;		// calculate col start position
    int GetYPos(int row_i) const;		// calculate row start position

    int ColWdDiff(int col0_i,int col1_i) const;   // get wd diff between cols
    int RowHtDiff(int row0_i,int row1_i) const;   // get ht diff between rows
	int		NumRowsInView( );
	DWORD	GetUserData(int iRow);

	
		
};
#endif
