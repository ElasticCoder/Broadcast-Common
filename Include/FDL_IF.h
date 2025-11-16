/* ~FileStart
********************************************************************
**  Copyright (C) Sony Broadcast &  Professional Europe 1998
**
**  FILE NAME      : FDL_IF.h
**  PROJECT        : 
**  AUTHOR         : AJP
**  CREATION DATE  : 22/09/98
**  REFERENCE      : None
**
********************************************************************
**  FUNCTIONAL DESCRIPTION:
**
**  This file defines the structures and functions used to
**	interface with the FDL Dynamic Link Library.
**
********************************************************************
**  FUNCTIONS IN THIS FILE:
**
**	None
**
********************************************************************
**  REVISION HISTORY:
**
**	Please refer to the Visual SourceSafe History for the revision
**  history of this file
**
********************************************************************
~FileEnd */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*******************************************************************
**  Include Files:
********************************************************************/

/*******************************************************************
** DEFINITIONS:
********************************************************************/

// Operational configurations
#define OPTION_WRITE_REPORT			0x01	// Write a report file
#define OPTION_CONFIRM_OVERWRITE	0x02	// Confirm before overwriting files
#define OPTION_AUTO_MODE			0x04	// Automatic operation
#define OPTION_AUTO_EVENT_NUM		0x08	// Automatically generate event IDs
#define OPTION_OVERWRITE_REPORTS	0x10	// Overwrite old report files
#define OPTION_OFFLINE_MODE			0x20	// Offline mode
#define OPTION_APPEND_END			0x40	// Append to the end of the schedule

// The parameters passed to an FDL AutoAppend function call
struct FDL_AppendParam
{
	CString m_sChanID;				// The ID of the channel being loaded
	CString m_sFilename;			// The filename of the playlist file
	CString m_sFileTitle;			// The title of the playlist file
	CDatabase* m_pDatabase;			// Pointer to the TDB database
	CString m_sReportDir;			// The report directory
	CString m_sDefaultStartTime;	// The start time if no dates are specified
	int m_iOptions;					// The FDL options used
};

// The status returned by an FDL AutoAppend function call
struct FDL_Return_Status
{
	BOOL m_zOK;						// Was the file loaded successfully?
	int m_iErrorCode;				// The error code
	CString m_sErrorMsg;			// The error message
	int m_iNumWarnings;				// The number of warnings
	int m_iNumErrors;				// The number of errors
	CString m_sReportFile;			// The report file written
};
