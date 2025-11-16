///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : G01_debug_logging.cpp
// File Type   : C++ source
// Description : Debug logging
// Author      : Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>                // Standard AFX definitions
#include <standard.h>              // Standard definitions
#include <tchar.h>                 // ...
#include <portable_types.h>		   // Type definitions
#include <g00_process_registry.h>  // Application and thread register definitions
#include <g02_datetime.h>          // Date and time class definitions
#include <stdio.h>                 // Standard input/output definitions

///////////////////////////////////////////////////////////////////////////////
// Function Description : Log to the debug log file
// Author               : Rick Miles
///////////////////////////////////////////////////////////////////////////////

void g01_log_debug(const char*    log_string_ca)
{
	HANDLE	log_file_h;

	CString	       timestamp;

	c				log_buffer_ca[512];
	m				bytes_written_m;
	C_g02_datetime	current_time;
//
// Set timestamp to current time
//
	current_time.set_datetime();
	timestamp = current_time.Format(CString(_TEXT("%d/%m/%y %H:%M:%S.%F")));

//
// Create log directory in case it doesn't exist
//
	SECURITY_ATTRIBUTES	SecurityAttributes;

	SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecurityAttributes.lpSecurityDescriptor =NULL; 
    SecurityAttributes.bInheritHandle = TRUE; 

	CreateDirectory(_T("C:\\plato\\log"),
					&SecurityAttributes 	// pointer to a security descriptor 
   );	
	
//
// Create file (with one retry if file handle invalid)
//
	log_file_h = CreateFile(_TEXT("C:\\plato\\log\\debuglog.txt"),
					        GENERIC_WRITE,	        // fdwAccess
					        0,				        // dfwShareMode
					        NULL,			        // lpsa
					        OPEN_ALWAYS,	        // fdwCreate
					        FILE_ATTRIBUTE_NORMAL,  // fdwAttrsAndFlags
					        NULL);
	if (log_file_h == INVALID_HANDLE_VALUE)
	{
        Sleep(1000);
	    log_file_h = CreateFile(_TEXT("C:\\plato\\log\\debuglog.txt"),
					            GENERIC_WRITE,	        // fdwAccess
					            0,				        // dfwShareMode
					            NULL,			        // lpsa
					            OPEN_ALWAYS,	        // fdwCreate
					            FILE_ATTRIBUTE_NORMAL,  // fdwAttrsAndFlags
					            NULL);
    }
//
// Format debug message (if file handle valid)
//
	if (log_file_h != INVALID_HANDLE_VALUE)
	{
		sprintf(	log_buffer_ca,
					"%s %s\r\n",
					timestamp,
					log_string_ca);
//
// Move to end of file
//
		SetFilePointer(log_file_h,
			           0,
			           NULL,
			           FILE_END);
//
// Write event to file
//
		WriteFile(log_file_h,
			      log_buffer_ca,
			      strlen(log_buffer_ca),
			      &bytes_written_m,
			      NULL);
//
// Get file size in case it needs archiving
//
		BY_HANDLE_FILE_INFORMATION		FileInformation; 	// address of structure 
		const m	MAX_ERRLOG_FILESIZE_M	= 200000;

		GetFileInformationByHandle(	log_file_h,
									&FileInformation );

//
// Close file
//
		CloseHandle(log_file_h);

//
// Archive overlarge file
//
		if (FileInformation.nFileSizeLow > MAX_ERRLOG_FILESIZE_M)
		{
			// Purge old archive file
			DeleteFile(	_T("C:\\plato\\log\\debuglog_a.txt"));

			// Rename log file
			MoveFile(	_T("C:\\plato\\log\\debuglog.txt"),
						_T("C:\\plato\\log\\debuglog_a.txt"));
  		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////