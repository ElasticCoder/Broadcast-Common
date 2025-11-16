/****************************************************************
	Ultimate Grid
	Dundas Software Ltd.
	Table Header File	  for version 2.0
	(C)opyright 1994-1995


	Note: This file may not be redistributed.


		DUNUG3220  32 bit class name

		DUNUG1620  16 bit class name

*****************************************************************/

#ifndef _ULTGRD20_H
#define _ULTGRD20_H


#include <windows.h>

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


//*********************** defines **************************
// date styles
#define DATE_CENTURY 	16
#define DATE_DDMMYY   	1
#define DATE_MMDDYY   	2
#define DATE_DDMONYY  	3
#define DATE_MONDDYY  	4
#define DATE_DDMONTHYY  5
#define DATE_MONTHDDYY  6

// window styles
#define TS_HEADINGWINDOW 	0x0010l  //Heading Window Style
#define TS_HEADING3D	 		0x0020l  //heading window with 3D button look
#define TS_SIDEHEADING     0x0040l	//side heading window


//misc. defines
#define TOPHEADING 0
#define SIDEHEADING 1

//edit key notify values
#define KEY_ENTER		1
#define KEY_TAB		2
#define KEY_STAB		3
#define KEY_UP			4
#define KEY_DOWN		5
#define KEY_PGUP		6
#define KEY_PGDOWN	7
#define KEY_ESC		8



//************************* Messages *************************

// messages that retrieve information

#define TB_GETCHARWIDTH WM_USER+131	//returns the width of an average char for
												//the current table font (in pixels)
#define TB_GETCOLWIDTH	WM_USER+128	//returns the widths of a column in pixels
												//wParam=col
#define TB_GETFONT		WM_USER+136	//returns the current font handle (NULL for the sys font)
#define TB_GETHDGHWND	WM_USER+135 //returns the hwnd of the tables heading window
												//wParam: 0=top 1=side
#define TB_GETNUMCOLS	WM_USER+134 //returns the number of cols on the entire table
#define TB_GETNUMROWS	WM_USER+133 //returns the number of rows on the entire table

#undef TB_GETRECT					//undefine because of conflict with C:\MSDEV\include\COMMCTRL.H
#define TB_GETRECT 		WM_USER+130 //gets the rect of a specific row-col
												//lParam =far * GETRECT
#define TB_GETSELROW		WM_USER+115	//returns the currently selected row number
#define TB_GETSELCOL		WM_USER+127	//returns the currently selected col number
#define TB_STRINGWIDTH 	WM_USER+132	//returns the width of a string using the table font
												//in pixels   lParam= char *
#define TB_GETHDC       WM_USER+138 //returns the tables HDC handle and also
												//sets the tables font

//messages that set information

#define TB_FSETCOLWIDTH WM_USER+137 	//sets the width of a column by ave char width
													//wParam=column  lParam=width
#define TB_GOTOCOL		WM_USER+114		//moves to the col specified  wParam=col
#define TB_GOTOROW		WM_USER+113		//moves to the row specified  lParam=row
#define TB_SETCOLWIDTH	WM_USER+112 	//sets the width of a column in pixels
													//wParam=column  lParam=width
#define TB_SETHDGHWND   WM_USER+119 	//gives the table the handle to the heading window
													//wParam: 0-top heading 1-side heading
													//lParam=hwnd
#define TB_SETINTERSPACE WM_USER+118   //sets the spacing between rows
													//wParam= number of pixels
#define TB_SETNUMCOLS	WM_USER+111		//sets the number of colums available
													//wParam=num cols up to 100
#define TB_SETNUMROWS 	WM_USER+110	  	//sets the number of rows available:
													//lParam=num rows up to 2,100,000,000
#define TB_USERRESIZE   WM_USER+116		//turns user column risizing on and off
													// wParam= 1 or 0  (On/Off)
#define TB_SETVLINES		WM_USER+140		//turns vertical lines on or off
													//wParam = TRUE or FALSE
#define TB_GETADDRESS	WM_USER+141		//returns the TABLEINFO address

#define TB_SETHIGHLIGHT	WM_USER+142  	//sets the highlight flag
													//wParam = TRUE or FALSE

#define TB_SETHLINES		WM_USER+143		//turns horizontal lines on/off
													//wParam = TRUE or FALSE
#define TB_SETROWHEIGHT WM_USER+144  	//sets the hieght of a row in pixels
													//wParam = pixels
#define TB_GETSCREENLINES WM_USER+145  //retrieves the number of rows that
													//could be shown on the screen

#define TB_HIGHLIGHTROW	WM_USER+146    //sets the row or cell mode
													//wParam 1-row 0-cell  

#define TB_SET3DHEIGHT	WM_USER+147		//sets the height of the button style
													//headings , wParam=height

#define TB_SETMOUSEMOVE	WM_USER+148   	//turns mouse movement on/off

#define TB_SETTOPROW		WM_USER+149		//sets the top row position
													//lParam=toprow
#define TB_GETTOPROW		WM_USER+150		//returns the top row position

#define TB_SETPAINTMODE	WM_USER+151		//turns painting on/off
													//wParam=TRUE or FALSE

#define TB_SETMARGIN		WM_USER+152		//sets the left and right margins

#define TB_SETLINECOLOR	WM_USER+153		//sets the pen colors for the grid lines
													//wParam  0-vlines 1-hlines
													//lParam  HPEN

#define TB_GETROWHEIGHT	WM_USER+154		//returns the hieght of a row in pixels

#define TB_SETCOLUMNMODE WM_USER+155	//sets the column display mode
													// 0- normal 1-stretch right column

#define TB_DRAGACCEPT	WM_USER+156		//specifies if the grid accepts drag and drop
													//wParam = TRUE/FALSE

//new as of July 10
#define TB_REDRAWROW		WM_USER+157		//redraws the given row lParam=row


#define TB_GETXCELLPOS	WM_USER+158		//returns the current cell click positions
#define TB_GETYCELLPOS	WM_USER+159

#define TB_GETEDITHWND	WM_USER+160		//returns the window handle for the edit control

#define TB_REDRAWCELL	WM_USER+161

#define TB_DRAWRAISEDBORDER	WM_USER+162 //draws a border wParam = hdc
#define TB_DRAWRECESSBORDER	WM_USER+163 //lParam =rect *

#define TB_SETMSGWINDOW			WM_USER+164 //sets the window that recieve the
														//notify messages (parent is default)
// new as of Aug 15
#define TB_ADJUSTNUMROWS		WM_USER+165 //lParam is added to the current number
//new as of Aug 28
#define TB_SETDEFBACKCOLOR   	WM_USER+166 //lParam = COLORREF
#define TB_SETDEFTEXTCOLOR		WM_USER+167
//new as of Sept 26
#define TB_SETVSCROLLMODE		WM_USER+168 //wParam = mode (1 by default)
														// mode 0 is joystick mode
//new as of Oct 16
#define TB_SETMAXCOLS			WM_USER+169 //wParam = new MAXCOLS value
//new as of Nov 9
#define TB_SETTHUMBTRACK		WM_USER+170 //wParam =TRUE or FALSE(default)
//new as of Nov 21
#define TB_SETHSCROLLMODE		WM_USER+171 //wParam = TRUE or FALSE(default)
														//TRUE:scroll only until right col is in view
//new as of Nov 24
#define TB_SETMOUSECAPTURE		WM_USER+172 //wParam = TRUE or FASLE(default)

//new as of Jan 18
#define TB_DESELECTALL			WM_USER+173 //no params

//new as of Jan 30
#define TB_SETMARGINMODE		WM_USER+174	//wParam = 0(default) or 1 dual margins
#define TB_MULTILINEMODE		WM_USER+175 //wParam = 0 (default ) or TRUE

//new as of Feb 16
#define TB_SETBTNDRKCOLOR		WM_USER+176	//lParam = COLORREF
#define TB_SETBTNLITCOLOR		WM_USER+177	//lParam = COLORREF

//*** edit messages ***
#define TB_EDIT_GETHWND	WM_USER+200
#define TB_EDIT_GETTEXT	WM_USER+201
#define TB_EDIT_SETTEXT	WM_USER+202

//notify messages (sent from the table or table heading)
//NOTE: all notify messages use the TABLEINFO structure

#define TBN_COLCHANGE	WM_USER+122	//notifies the parent of a col change
												//lParam= TABLEINFO *
#define TBN_KEYBOARD		WM_USER+124	//returns the value of a key that was pressed
#define TBN_ROWCHANGE	WM_USER+121	//notifies the parent of a row change
												//lParam= TABLEINFO *
#define TBN_ROWSELECTED	WM_USER+123	//notifies the parent of a row selection
												//lParam= TABLEINFO *
#define TBN_WANTINFO		WM_USER+120	//notifies the main window that it must supply
												//the string for the current row
												//lParam= TABLEINFO *
#define TBN_LCLICKED		WM_USER+126 //notifies the main window that the left mouse
												//button was clicked
#define TBN_RCLICKED		WM_USER+127 //notifies the main window that the right mouse
												//button was clicked
#define TBN_DCLICKED		WM_USER+128 //notifies the main window that the left mouse
												//button was double clicked

#define TBN_ENTER			WM_USER+129 //notifies of an ENTER key press

#define TBN_LCLICKED2	WM_USER+130 //notifies the main window that the left mouse
												//button was clicked and the cell did not change

#define TBN_SETFOCUS		WM_USER+131	//sent when the grid gains focus

#define TBN_KILLFOCUS	WM_USER+132 //sent when the grid loses focus

#define TBN_CELLCHANGE	WM_USER+133	//sent whenever the current cell is changed

#define TBN_DRAGDROP		WM_USER+134	//sent whenever a drag and drop is completed

#define TBN_CANRESIZE	WM_USER+135	//sent when a user tries to resize a column
												//TI.WPARAM equals  TRUE or FALSE
#define TBN_COMMAND		WM_USER+136 //sent when the grids child controls send messages
												//TI.wParam = ID of child control
												//TI.lParam = message from child control

//new as of July 24
#define TBN_TOPROWCHANGE	  	WM_USER+137 //sent when the top row changes
#define TBN_LEFTCOLCHANGE	 	WM_USER+138 //sent when the left col changes

//new as of Aug 15
#define TBN_COLUMNSIZED			WM_USER+139 //sent when a column was sized
#define TBN_HITBOTTOM			WM_USER+140
#define TBN_HITTOP				WM_USER+141
//new as of Sept 27
#define TBN_COLSIZING			WM_USER+142	//sent when a col is sizing
//new as of Nov 24
#define TBN_MOUSECAPTURE		WM_USER+143 //sent when mouse capture is enabled

//********** edit notifications **********
#define TBN_EDITKEY		WM_USER+150	//when in edit mode - some of the keystrokes
												//made are passed on by this notification message
#define TBN_EDITCHANGE	WM_USER+151 //when in edit mode - sent whenever the string
												//changes
#define TBN_EDITKILL		WM_USER+152 //when in edit mode - sent when the edit control
												//loses focus
#define TBN_EDITSET		WM_USER+153 //sent when the edit mode starts



//**********  debug interface *******
#define TB_DEBUG			WM_USER+190	//turns debug mode on/off
												//wParam  TRUE/FALSE
#define TBN_DEBUG			WM_USER+192	//sent when the debug has a message



//************************* Structures *************************

typedef struct{

	UINT msg;				//notification being sent

	WPARAM wParam;			//general purpose variable
	LPARAM lParam;			//general purpose variable

	int  col;				//column
	long row;  		     	//row

	COLORREF textcolor; 	//text color
	COLORREF backcolor; 	//background color
	WPARAM 	alignment;	//text alignment extended option
	char far * buf;     	//text string
	int      style;		//extended style option (for check boxes,radio,etc)
	HBITMAP	hbitmap;		//bitmap handle

	int owner;				//owner drawn flag , TRUE or FALSE
	HDC hdc;          	//device context
	RECT rect;           //cell rectangle

   POINT point;			//point member , for mouse clicks

}TABLEINFO;

// this is used when retrieving the co-ords of a cell within the table
typedef struct{
	WPARAM col;				//col to return
	LPARAM  row;			//row to return
	RECT rect;				//structure to return the co-ords in
	int isvisible;			//visibility flag  0-not visible 1- visible
								//                 2-partial visibility
}TABLEGETRECT;


//************************* Functions *************************

int CALLBACK TB_InitTable(HINSTANCE hInstance);
int CALLBACK TB_SetupTable(HWND Tablehwnd,HWND Topheadhwnd,
	HWND Sideheadhwnd,LPARAM rows,int cols,int far* colwidths,int interspace,int usersize);
int CALLBACK TB_ExtendText(TABLEINFO far *ti,WPARAM alignment,COLORREF textcolor,
							COLORREF backcolor);
int CALLBACK TB_BestFit(HWND tblhwnd,int extra,LPARAM count,FARPROC fp);
int CALLBACK TB_RedrawTable(HWND tblhwnd);
int CALLBACK TB_FitToWindow(HWND tblhwnd,int numcols);
int CALLBACK TB_CurrencyToString(char far *String,double d,int Commas,
							char far *Symbol);
int CALLBACK TB_DateToString(char far *String,int Day,int Month,int Year,
							char far *Separator,int Type);
int CALLBACK TB_FitHeadingToTable(HWND tblhwnd,int heading,int space);

int CALLBACK TB_StartEdit(HWND Tblhwnd,char far *text,char far *mask,
							char far *temp,int key);
int CALLBACK TB_StartEditEx(HWND Tblhwnd,char far *text,char far *mask,char far *temp,
							int key,int col,long row);
int CALLBACK TB_EndEdit(HWND Tblhwnd);
int CALLBACK TB_SetEditClass(HWND Tblhwnd,char far *classname);

int CALLBACK TB_DragStart(HWND Tblhwnd,HCURSOR hcursor,char far **draginfo,
	int numitems);

int CALLBACK TB_StartCache(HWND tblhwnd,int size);
int CALLBACK TB_RefreshCache(HWND tblhwnd);

int CALLBACK TB_SetEditClassEx(HWND Tblhwnd,char far *classname,long style);

/**********************************************************
			 In-Line Editing Control
***********************************************************/

// functions
int CALLBACK ED_Init(HINSTANCE hInstance);

// styles
#define DES_WANTENTER	0x0100
#define DES_WANTTAB   	0x0200
#define DES_HIGHLIGHT	0x0400

// messages
#define	DEM_GETTEXT		  	WM_GETTEXT
#define	DEM_SETTEXT      	WM_SETTEXT
#define	DEM_SETTEMPLATE  	WM_USER+100
#define	DEM_GETTEMPLATE  	WM_USER+101
#define	DEM_GETMASK      	WM_USER+102
#define	DEM_SETMASK      	WM_USER+103
#define  DEM_GETKEY		  	WM_USER+104
#define  DEM_SETMARGIN	  	WM_USER+105
#define	DEM_SETTEXTCOLOR  WM_USER+106
#define	DEM_SETBACKCOLOR  WM_USER+107

// Notify messages
#define	DEN_CHANGE 		EN_CHANGE
#define	DEN_SETFOCUS   EN_SETFOCUS
#define	DEN_KILLFOCUS  EN_KILLFOCUS
#define  DEN_KEYPRESSED 100
#define	DEN_MASKWARN   101


#ifdef __cplusplus
}
#endif  /* __cplusplus */




#endif
