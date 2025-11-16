// GridControl.h: interface for the CGridControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCONTROL_H__C53D191C_118C_11D2_AB2E_00C04FB1589E__INCLUDED_)
#define AFX_GRIDCONTROL_H__C53D191C_118C_11D2_AB2E_00C04FB1589E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <GridCtrl.h>
#include "colourdefs.h"

enum COLSIZE_STYLE { SB_NORMAL, SB_STRETCH };


class CGridControl 
{
public:
	
	

	//Construction/Destruction
					CGridControl( CWnd* pParent, WORD wGridResId );
					~CGridControl();
public:
	//
	// Public Methods
	//
	void			StartCursorPos(int iStartPos);
	void			PaintCursor(SGrdCellInfo* pCell);
	bool			IsEndOfData(){return ((m_iRowsInGrid-1) == m_iCursorPosition);};
	bool			IsStartOfData(){return (m_iRowsInGrid == 0);};

	bool			IsMyMessage( HWND hWnd);
	bool			IsCursor(const SGrdCellInfo * pCell);
	bool			IsLeftButton( SGrdCellInfo* pCell);

	bool			IsHighlight( const SGrdCellInfo* pCell );
	void			Justify(SGrdCellInfo * pCell, DWORD dwJustify = DT_LEFT);
	bool			BlockSelected();
	bool			BlockSelected( int& iStart, int& iEnd );
	void			BlockSelection( bool bOn = true);
	
	void			Disable( bool bInhibit );
	
	CRect			MoveGrid( int iTop = -1);
	void			MoveCursorTo(int iRow);
	CGridCtrl*		Grid();
	

	void			CursorOff(SGrdCellInfo *pCell);
	void			CursorOn(SGrdCellInfo *pCell);
	void			EnableCursor( bool bEnable = true);

	void			ConfigureScrollBar();
	void			ConfigureCols(const int* colWidths, COLSIZE_STYLE Style = SB_NORMAL);
	void			ConfigureGrid();
	void			ConfigureHeader(TCHAR *const	*szHeaderText);
	void			ConfigureCursor(UINT CursorStyle){m_pGridCtrl -> ConfigureCursor(CursorStyle);};
	//=======================================================
	//
	void			ClearRowFlag(int iRow, int iFlag){m_pGridCtrl ->ClearRowFlag(iRow, iFlag);};
	//
	void			SetRowFlag(int iRow, int iFlag){m_pGridCtrl ->SetRowFlag(iRow, iFlag);};
	//
	void			SetHighlightBlock(int iStart, int iEnd){m_pGridCtrl -> SetHighlightBlock(iStart,iEnd);};
	//
	bool			IsRowFlagged(int iRow){return m_pGridCtrl -> IsRowFlagged(iRow);};
	//
	//=======================================================
	void			SetCursorPos( int iCursorPos );
	void			SetCursorColumnRange( int iCols );
	void			SetBlockHighlightColour(COLORREF clrBack, COLORREF clrFore);
	void			SetNumRows( int iRow );
	void			SetNumColumns(int iCols);
	bool			SetCursorRange(SGrdCellInfo* pCell);
	void			SetCellColour(SGrdCellInfo* pCell, COLORREF clrBack, COLORREF clrText = GRD_BLACK);
	void			SetVisibleRows( int iVisRows);
	void			SetCellText(SGrdCellInfo * pCell, const CString& sText);
	void			SetCellTextEx(SGrdCellInfo * pCell, const char* p_szText);

	void			SetColumnWidths(const int* colWidths);
	void			SetBackColour( SGrdCellInfo * pCell, COLORREF clrBack ){pCell -> bkColor_m =  ::GetNearestColor(pCell -> hdc, clrBack);};
	void			SetGridFont(CString sFaceName, int iPointSize);
	void			SetCellStyle(SGrdCellInfo * pCell, bool b3DOn);
	
	void			SetPen(HPEN hPen){m_pGridCtrl -> SetPen(hPen);};
	void			SetFocus() {m_pGridWnd.SetFocus();};

	bool			IsMarked(int& iStart, int& iEnd);
	void			UnmarkBlock(int fBlockFlag, int iStart, int iEnd);
	void			MarkBlock(int fBlockFlag, int iStart, int iEnd);

	CPoint			GridMaxSize( int NumHeaderRows );
	CPoint			GridMinSize( int NumHeaderRows );

	void			MoveGrid(CRect GridRect, BOOL bPaint = TRUE){m_pGridWnd.MoveWindow( GridRect, bPaint );};
	void			MoveGrid(int iLeft, int iTop);
	void			InvalidateScreen();
	void			InvalidateCursor( );
	void			InvalidateRow( int iRow );
	void			InvalidateCells(int iCol, int iRow, int iSizeCols = 1, int iSizeRows = 1){m_pGridCtrl -> InvalidateCells(iCol,iRow,iSizeCols,iSizeRows);};


	HWND			GetSafeHwnd();
	CWnd*			GetSafeWnd();
	int				GetCursorPos();
	int				GetNumRows(){return m_iRowsInGrid;};
	int*			GetColumnWidths(){return m_piHeaderWidths;};
	int				GetGridTextWidth();
	//==============================
	// Get the Row Height
	//==============================
	int				GetDefaultRowHt( ) const {
		return m_pGridCtrl -> GetRowHt( 0 );
	}
	void			CursorBack(COLORREF newClr) {
		m_CursorBack = newClr;
	}
	void			CursorText(COLORREF newClr ) {
		m_CursorText = newClr;
	}
	void			MoveUpOneLine( );
	void			MoveDownOneLine( );
private:
	//==============================
	// Private methods
	//==============================
	int				StartCursorPos();
	//==============================
	// Private Data
	//==============================
	int				m_iStartCursorPos;
	int				m_iMarkCount;
	int				m_iRowsInGrid;
	int				m_iColumnsInGrid;
	int				m_iCursorPosition;
	int				m_iVisibleRows;
	int				m_iStartBlock;
	int				m_iEndBlock;
	int				m_iCursorColumns;
	int				m_iMarkStart;
	int				m_iMarkEnd;
	CGridCtrl*		m_pGridCtrl;
	CWnd			m_pGridWnd;
	CWnd*			m_pParent;
	int*			m_piHeaderWidths;
	bool			m_bBlockSelection;
	bool			m_bInhibit;
	bool			m_bUseCursor;
	CFont			m_gridFont;
	COLORREF		m_CursorBack;
	COLORREF		m_CursorText;
};

#endif // !defined(AFX_GRIDCONTROL_H__C53D191C_118C_11D2_AB2E_00C04FB1589E__INCLUDED_)
