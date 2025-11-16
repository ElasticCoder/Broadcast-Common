///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : G01_event_logging.cpp
// File Type   : C++ source
// Description : Event logging
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>                // Standard AFX definitions
#include <standard.h>              // Standard definitions
#include <tchar.h>                 // ...
#include <portable_types.h>		   // Type definitions
#include <g00_process_registry.h>  // Application and thread register definitions
#include <g01_event_logging.h>     // Event logging definitions
#include <g02_datetime.h>          // Date and time class definitions
#include <stdio.h>                 // Standard input/output definitions

//
// Define data to enable exclusive access to the log file
// N.B. these are only needed while the logs are held in files rather
//	    than in the database
//
static HANDLE	v_file_access_mutex_h = NULL;	// 
static const tc v_file_access_mutex_name_tca[] = _TEXT("g01_file_access");

static const tc v_directory_name_tca[] = _TEXT("log");
static const tc v_file_name_tca[] = _TEXT("log\\errorlog.txt");
static const tc v_backup_file_name_tca[] = _TEXT("log\\errorlog_a.txt");

///////////////////////////////////////////////////////////////////////////////
// Function Description : Obtain exclusive access to the log files
// Author               : Duncan Olesen
// Date Created         : 16/01/97
// Date Last Modified   : 16/01/97
///////////////////////////////////////////////////////////////////////////////

z g01_lock_file()
{
	z	locked_z;

	locked_z = FALSE;
//
// Obtain handle to mutex if not already done so
//
	if (v_file_access_mutex_h == NULL)
	{
		v_file_access_mutex_h = CreateMutex(
									NULL,		// Security attributes
									FALSE,		// Initial Owner
									v_file_access_mutex_name_tca); // mutex name
	}

	if (v_file_access_mutex_h != NULL)
	{
		DWORD	dwWaitStatus;

		dwWaitStatus = WaitForSingleObject(
							v_file_access_mutex_h,	// hObject
							(DWORD) 1000);			// dwTimeout

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
			locked_z = TRUE;
		}
	}

	return locked_z;
} // g01_lock_file

///////////////////////////////////////////////////////////////////////////////
// Function Description : Send a system event to the local platform logger
// Author               : Duncan Olesen
// Date Created         : 01/04/96
// Date Last Modified   : 01/04/96
///////////////////////////////////////////////////////////////////////////////

void g01_log_system_event(
	T_g01_se_type		p_type,
	T_g01_se_severity	p_severity,
	w					p_code_w,
	const CString		&p_string,
	const CString		&p_add_info,
	T_g01_se_status		p_status)
{
	HANDLE		log_file_h;
	tc			event_buffer_tca[1024];
	tc			type_tca[40];
	tc			severity_tca[40];
	tc			status_tca[40];
	tc			thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	m			bytes_written_m;
	C_g02_datetime	current_time;
	CString		timestamp;
//
// Set timestamp to current time
//
	current_time.set_datetime();
	timestamp = current_time.Format(CString(_TEXT("%d/%m/%y %H:%M:%S.%F")));

	switch (p_type)
	{
	case G01_SE_SOFTWARE_EXCEPTION:
		_tcscpy(type_tca, _TEXT("G01_SE_SOFTWARE_EXCEPTION"));
		break;
	case G01_SE_SOFTWARE_WARNING:
		_tcscpy(type_tca, _TEXT("G01_SE_SOFTWARE_WARNING"));
		break;
	case G01_SE_SOFTWARE_INFORMATION:
		_tcscpy(type_tca, _TEXT("G01_SE_SOFTWARE_INFORMATION"));
		break;
	default:
		_tcscpy(type_tca, _TEXT("UNRECOGNISED"));
		break;
	};

	switch (p_severity)
	{
	case G01_SE_PLATFORM_FATAL:
		_tcscpy(severity_tca, _TEXT("G01_SE_PLATFORM_FATAL"));
		break;
	case G01_SE_APPLICATION_FATAL:
		_tcscpy(severity_tca, _TEXT("G01_SE_APPLICATION_FATAL"));
		break;
	case G01_SE_THREAD_FATAL:
		_tcscpy(severity_tca, _TEXT("G01_SE_THREAD_FATAL"));
		break;
	case G01_SE_ERROR:
		_tcscpy(severity_tca, _TEXT("G01_SE_ERROR"));
		break;
	case G01_SE_WARNING:
		_tcscpy(severity_tca, _TEXT("G01_SE_WARNING"));
		break;
	case G01_SE_INFO:
		_tcscpy(severity_tca, _TEXT("G01_SE_INFO"));
		break;
	default:
		_tcscpy(severity_tca, _TEXT("UNRECOGNISED"));
		break;
	};

	switch (p_status)
	{
	case G01_SE_RAISED:
		_tcscpy(status_tca, _TEXT("G01_SE_RAISED"));
		break;
	case G01_SE_CLEARED:
		_tcscpy(status_tca, _TEXT("G01_SE_CLEARED"));
		break;
	default:
		_tcscpy(status_tca, _TEXT("UNRECOGNISED"));
		break;
	};

	g00_thread_name(thread_name_tca);

	_stprintf(
		event_buffer_tca,
		_TEXT("%s %-39s %-39s\r\n%05d %s\r\n%s\r\n%s\r\n%s\r\n\r\n"),
		timestamp,
		type_tca,
		severity_tca,
		p_code_w,
		thread_name_tca,
		p_string,
		p_add_info,
		status_tca);
//
// Create log directory in case it doesn't exist
//
	SECURITY_ATTRIBUTES	SecurityAttributes;

	SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecurityAttributes.lpSecurityDescriptor =NULL; 
    SecurityAttributes.bInheritHandle = TRUE; 

	CreateDirectory(v_directory_name_tca,
					&SecurityAttributes); 	// pointer to a security descriptor 
//
// Obtain exclusive access to the log file
//
	if (g01_lock_file())
	{
		log_file_h = CreateFile(
						v_file_name_tca,
						GENERIC_WRITE,	// fdwAccess
						0,				// dfwShareMode
						NULL,			// lpsa
						OPEN_ALWAYS,	// fdwCreate
						FILE_ATTRIBUTE_NORMAL, // fdwAttrsAndFlags
						NULL);

		if (log_file_h != INVALID_HANDLE_VALUE)
		{
//
// Move to end of file
//
			SetFilePointer(
				log_file_h,
				0,
				NULL,
				FILE_END);
//
// Write event to file
//
			WriteFile(
				log_file_h,
				event_buffer_tca,
				_tcslen(event_buffer_tca) * sizeof(tc),
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
				DeleteFile(v_backup_file_name_tca);

				// Rename log file
				MoveFile(v_file_name_tca,
						 v_backup_file_name_tca);
  			}
		}
//
// Unlock file
//
		ReleaseMutex(v_file_access_mutex_h);
	} // file locked
}


///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////