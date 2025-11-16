/******************************************************************************
 * Project     : Grid Control
 * File        : gc_cell_info.h
 * Author      : Duncan Olesen
 * Date        : 20/12/96
 *
 * Description : Grid Control Cell Info Structure
 *****************************************************************************/


#ifndef _GC_CELL_INFO_H
#define _GC_CELL_INFO_H

#include <gclibapi.h>

//
// Defined the number of digits including sign in a DWORD
//
const unsigned char DWORD_DIGITS_B = 11;

// Used by the WM_GRD_GET_INFO message to pass/obtain information about a cell
// from the parent window
struct SGrdCellInfo
{
public:
	SGrdCellInfo();
	~SGrdCellInfo();

    // Users may NOT update the following data
	HDC			hdc;                // device context handle of grid
	DWORD		dwUserData;			// User data
	// Users may update the following data
	int         col_i;              // column of cell
	int	        row_i;              // row of cell

	int			RowInView;			// Row number in viewable region

	HFONT	    font_h;		        // Font to use
	HPEN	    colPen_h;		    // Pen to draw col seperator
	HPEN	    rowPen_h;		    // Pen to draw row seperator
	DWORD       bkColor_m;	        // Cell Background colour
	DWORD       txtColor_m;         // Text color
	DWORD       txtFmt_m;	        // DT_ literals (i.e. centre,right etc.)
	BOOL        owner_z;		    // always TRUE, make false if owner draw
	BOOL		cell3D_z;			// 3D Cell Effect;
	UINT		CursorStyle;		// Type of cursor 
	TCHAR		*text_tcp;		    // Address of text
	int	        sizeCols_i;		    // cell size in number of columns (default 1)
	int	        sizeRows_i;		    // cell size in number of rows (default 1)

	int		    keyState_i;			// GRD_SHIFT_MASK, GRD_CTRL_MASK
	WORD		fwKeys;				// Mouse state
	BOOL	    crsrOn_z;			// cursor off flag
	int		    crsrCol_i;          // column position of cursor
	int		    crsrRow_i;			// row position of cursor
	int		    colRng0_i;			// If SHIFT, only x2,y2 updated 
	int		    rowRng0_i;			// If CTRL, none updated
	int		    colRng1_i;			// No shift, both set to crsr_x, crsr_y
	int		    rowRng1_i;

private:
	TCHAR		number_tca[DWORD_DIGITS_B + 1];

public:
	inline BOOL	cell_in_cursor()
				{ return (crsrOn_z && 
						  col_i >= colRng0_i &&
						  col_i <= colRng1_i &&
						  row_i >= rowRng0_i &&
						  row_i <= rowRng1_i); };
//
// Set cell to hold a text represenation of a number
// Parameters:
//		p_num_l			The number to be displayed in the cell
//		p_format_tcp	printf format string to use to format the number,
//						including the '%' character.
//						The default format is "%ld"
//						The format will be unsuccessful if the resulting
//						string is longer than the number of digits in a
//						DWORD plus a sign (DWORD_DIGITS_B)
// Returns
//		Returns TRUE if number successfully formatted, FALSE otherwise
//
	GCLIBAPI BOOL set_num(
					DWORD			p_num_l,
					const TCHAR	*p_format_tcp = NULL);
};

#endif _GC_CELL_INFO_H