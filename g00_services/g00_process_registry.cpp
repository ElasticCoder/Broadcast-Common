///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : G00_process_registry.cpp
// File Type   : C++ Source
// Description : Register of application and its threads
// Author      : Duncan Olesen, modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>  // Type definitions
#include <g00_process_registry.h>
#include <g01_event_logging.h>
#include <g02_timeout_control.h>
#include "thread_info.h"

#include <afxtempl.h>

// Items in the following list should be supported by the enum in G00_process_registry.h
const tc application_name_tc2a[][G00_MAX_APP_TYPE_LEN_W + 1] =
{
_TEXT("APP_UNKNOWN"),
_TEXT("PLATO"),
_TEXT("DBI"),
_TEXT("KAVO"),
_TEXT("TX_SYNC"),
_TEXT("RCP"),
_TEXT("CATALOG"),
_TEXT("LMS"),
_TEXT("PAWS_LITE"),
_TEXT("ADEC")
};

class C_lock_holder
{
public:
	C_lock_holder()
		{ m_lock_h = NULL; };
	~C_lock_holder()
		{ if (m_lock_h != NULL) CloseHandle(m_lock_h); };

	HANDLE	m_lock_h;
};

static C_g00_app_name		v_crnt_app_name;
static DWORD				v_primary_thread = 0;
static CMap <DWORD, DWORD, S_thread_info, S_thread_info> v_thread_info_map;
static z					v_terminating_z = FALSE;
static C_lock_holder		v_lock;

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_app_name::C_g00_app_name()
{
	name_app(
		P00_UNKNOWN,
		(w) 0);
} // C_g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name copy constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_app_name::C_g00_app_name(const C_g00_app_name	&p_app_name)
{
	name_app(
		p_app_name.m_app_type,
		p_app_name.m_app_inst_w);
} // C_g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_app_name::C_g00_app_name(
		T_g00_app_type	p_app_type,
		w				p_app_inst_w)
{
	name_app(
		p_app_type,
		p_app_inst_w);
} // C_g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name destructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_app_name::~C_g00_app_name()
{
} // ~C_g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name Equality
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g00_app_name::operator==(const C_g00_app_name &pc_app_name) const
{
	BOOL	ret_val;

	if (m_app_type == pc_app_name.m_app_type &&
		m_app_inst_w == pc_app_name.m_app_inst_w)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator==

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_app_name Inequality
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g00_app_name::operator!=(const C_g00_app_name &pc_app_name) const
{
	BOOL	ret_val;

	if (m_app_type != pc_app_name.m_app_type ||
		m_app_inst_w != pc_app_name.m_app_inst_w)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator!=

///////////////////////////////////////////////////////////////////////////////
// Function Description : Sets string containing application instance name
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void C_g00_app_name::app_name(tc *p_app_name_tcp) const
{
//
// Sets up string to contain system unique name derived
// from application name. The string will have a maximum
// length of G00_MAX_APP_NAME_LEN_W + terminating zero.
//
	_stprintf(p_app_name_tcp,
			  _TEXT("%s.%u"),
			  application_name_tc2a[m_app_type],
			  m_app_inst_w);
} // app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_thread_name::C_g00_thread_name()
: C_g00_app_name(P00_UNKNOWN, (w) 0)
{
	m_thread_num_w  = 0;
	m_thread_inst_w = 0;
} // C_g00_thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_thread_name::C_g00_thread_name(
		T_g00_app_type	p_app_type,
		w				p_app_inst_w,
		w				p_thread_num_w,
		w				p_thread_inst_w)
: C_g00_app_name(p_app_type, p_app_inst_w)
{
	m_thread_num_w  = p_thread_num_w;
	m_thread_inst_w = p_thread_inst_w;
} // C_g00_thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name constructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_thread_name::C_g00_thread_name(
		const C_g00_app_name	&p_app_name,
		w						p_thread_num_w,
		w						p_thread_inst_w)
: C_g00_app_name(p_app_name)
{
	m_thread_num_w  = p_thread_num_w;
	m_thread_inst_w = p_thread_inst_w;
} // C_g00_thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name destructor
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

C_g00_thread_name::~C_g00_thread_name()
{
} // ~C_g00_thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name Equality
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g00_thread_name::operator==(const C_g00_thread_name &pc_thread_name) const
{
	BOOL	ret_val;

	if (C_g00_app_name(*this) == C_g00_app_name(pc_thread_name) &&
		m_thread_num_w == pc_thread_name.m_thread_num_w &&
		m_thread_inst_w == pc_thread_name.m_thread_inst_w)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator==

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g00_thread_name Inequality
// Author               : Duncan Olesen
// Date Created         : 16/04/96
// Date Last Modified   : 16/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g00_thread_name::operator!=(const C_g00_thread_name &pc_thread_name) const
{
	BOOL	ret_val;

	if (C_g00_app_name(*this) != C_g00_app_name(pc_thread_name) ||
		m_thread_num_w != pc_thread_name.m_thread_num_w ||
		m_thread_inst_w != pc_thread_name.m_thread_inst_w)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
} // operator!=

///////////////////////////////////////////////////////////////////////////////
// Function Description : Sets string containing thread instance name
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void C_g00_thread_name::thread_name(tc *p_thread_name_tcp) const
{
//
// Sets up string to contain system unique name derived
// from thread name. The string will have a maximum length
// of G00_MAX_THREAD_NAME_LEN + terminating zero.
//
	long	app_name_len_w;

	app_name(p_thread_name_tcp);

	app_name_len_w = _tcslen(p_thread_name_tcp);

	_stprintf(&p_thread_name_tcp[app_name_len_w],
			  _TEXT(":%u.%u"),
			  m_thread_num_w,
			  m_thread_inst_w);
} // thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : Register current platform
// Author               : Duncan Olesen
// Date Created         : 01/04/96
// Date Last Modified   : 01/04/96
///////////////////////////////////////////////////////////////////////////////

void g00_register_platform(const tc* /*pc_platform_tcp*/)
{
} // g00_register_platform

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove current platform from registry
// Author               : Duncan Olesen
// Date Created         : 01/04/96
// Date Last Modified   : 01/04/96
///////////////////////////////////////////////////////////////////////////////

void g00_unregister_platform()
{
} // g00_unregister_platform

///////////////////////////////////////////////////////////////////////////////
// Function Description : Register current application
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_register_app(const C_g00_app_name &p_app_name)
{
	if (v_primary_thread == 0)
	{
		v_crnt_app_name = p_app_name;
//
// Record the id of the current thread as the id of the process'
// primary thread. This is used to ensure that g00_wait_for_child_termination
// is only called by the primary thread
//
		v_primary_thread = GetCurrentThreadId();
//
// Create mutex to enable exclusive access to the process global thread info
//
		v_lock.m_lock_h = CreateMutex(
						NULL,	// lpsa - No security attributes
						FALSE,	// fInitialOwner - created in unsignalled state
						NULL);	// lpszMutexName - none

		if (v_lock.m_lock_h == NULL)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 1,
				CString(_TEXT("g00_register_app")),
				CString(_TEXT("Could not create mutex")),
				G01_SE_RAISED);
		}
//
// Flag that process is not terminating
//
		v_terminating_z = FALSE;
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 2,
			CString(_TEXT("g00_register_app")),
			CString(_TEXT("Application already registered")),
			G01_SE_RAISED);
	}
} // g00_register_app

///////////////////////////////////////////////////////////////////////////////
// Function Description : Register current application
// Author               : Duncan Olesen
// Date Created         : 03/04/96
// Date Last Modified   : 03/04/96
///////////////////////////////////////////////////////////////////////////////

void g00_register_app(
						T_g00_app_type	p_app_type,
						w				p_app_inst_w)
{
	g00_register_app(C_g00_app_name(p_app_type, p_app_inst_w));
} // g00_register_app

///////////////////////////////////////////////////////////////////////////////
// Function Description : Register current thread
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_register_thread(const C_g00_thread_name &p_thread_name)
{
	T_g00_app_type	app_type;
	w				app_inst_w;
	w				thread_num_w;
	w				thread_inst_w;
//
// Get the components of the thread name
//
	p_thread_name.thread_name(
		app_type,
		app_inst_w,
		thread_num_w,
		thread_inst_w);
//
// Register the thread
//
	g00_register_thread(
		thread_num_w,
		thread_inst_w);
} // g00_register_thread

///////////////////////////////////////////////////////////////////////////////
// Function Description : Register current thread
// Author               : Duncan Olesen
// Date Created         : 03/04/96
// Date Last Modified   : 03/04/96
///////////////////////////////////////////////////////////////////////////////

void g00_register_thread(
						w	p_thread_num_w,
						w	p_thread_inst_w)
{
	DWORD			dwWaitStatus;
	DWORD			dwThreadId;
	S_thread_info	thread_info;
	HANDLE			thread_h;

	if (v_lock.m_lock_h != NULL)
	{
//
// Get lock on global data
//
		dwWaitStatus = WaitForSingleObject(v_lock.m_lock_h, INFINITE);

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
//
// Get the id for the thread calling this function
//
			dwThreadId = GetCurrentThreadId();
//
// Obtain a 'real' thread handle rather than the pseudo (thread relative) handle
// returned by GetCurrentThread(). A real handle is required because the handle
// is to be used by another (the primary) thread by g00_wait_for_child_termination
//
			DuplicateHandle(
				GetCurrentProcess(),	// Handle of process that thread
										// psuedo handle is relative to
				GetCurrentThread(),		// Current thread's pseudo handle
				GetCurrentProcess(),	// Handle of process that the new,
										// 'real' handle is relative to
				&thread_h,				// will receive the new handle
				0,						// Ignored because of DUPLICATE_SAME_ACCESS
				FALSE,					// new handle is not inheritable
				DUPLICATE_SAME_ACCESS);	// New handle has same access as pseudo-handle
//
// Store the thread name in appropriate object
//
			thread_info.m_thread_num_w  = p_thread_num_w;
			thread_info.m_thread_inst_w = p_thread_inst_w;
			thread_info.m_handle_h		= thread_h;
//
// Store the thread info in the map (hash table) keyed by the thread id
//
			v_thread_info_map[dwThreadId] = thread_info;
//
// Release lock
//
			ReleaseMutex(v_lock.m_lock_h);
		}
		else
		{
			DWORD	dwError;
			CString temp;

			dwError = GetLastError();

			temp.Format(_TEXT("dwWaitStatus = %ld, GetLastError = X%08x"),
						dwWaitStatus,
						dwError);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 3,
				CString(_TEXT("g00_register_thread: wait error")),
				temp,
				G01_SE_RAISED);
		}
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 4,
			CString(_TEXT("g00_register_thread")),
			CString(_TEXT("NULL lock handle")),
			G01_SE_RAISED);
	}
} // g00_register_thread

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove current app from registry
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_unregister_app()
{
	DWORD			dwWaitStatus;

	if (v_lock.m_lock_h != NULL)
	{
//
// Get lock on global data
//
		dwWaitStatus = WaitForSingleObject(v_lock.m_lock_h, INFINITE);

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
//
// Check that called by primary thread
//
			if (GetCurrentThreadId() != v_primary_thread)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 7,
					CString(_TEXT("g00_unregister_app")),
					CString(_TEXT("Not called by primary thread")),
					G01_SE_RAISED);
//
// Release lock
//
				ReleaseMutex(v_lock.m_lock_h);
			}
			else
			{
				v_primary_thread = 0;
				ReleaseMutex(v_lock.m_lock_h);
				CloseHandle(v_lock.m_lock_h);
				v_lock.m_lock_h = NULL;
			}
		}
		else
		{
			DWORD	dwError;
			CString temp;

			dwError = GetLastError();

			temp.Format(_TEXT("dwWaitStatus = %ld, GetLastError = X%08x"),
						dwWaitStatus,
						dwError);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 5,
				CString(_TEXT("g00_unregister_app: wait error")),
				temp,
				G01_SE_RAISED);
		}
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 6,
			CString(_TEXT("g00_unregister_app")),
			CString(_TEXT("NULL lock handle")),
			G01_SE_RAISED);
	}
} // g00_unregister_app

///////////////////////////////////////////////////////////////////////////////
// Function Description : Remove current thread from registry
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_unregister_thread()
{
	DWORD			dwWaitStatus;
	DWORD			dwThreadId;

	if (v_lock.m_lock_h != NULL)
	{
//
// Get lock on global data
//
		dwWaitStatus = WaitForSingleObject(v_lock.m_lock_h, INFINITE);

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
//
// Get the id for the thread calling this function
//
			dwThreadId = GetCurrentThreadId();
//
// Only update global variables if not waiting for process to terminate or
// this is the primary thread
//
			if (v_terminating_z == FALSE ||
				dwThreadId == v_primary_thread)
			{
//
// Close the handle of the current thread
//
				CloseHandle(v_thread_info_map[dwThreadId].m_handle_h);
//
// Remove the info associated with the current thread from the map
//
				(void) v_thread_info_map.RemoveKey(dwThreadId);
			}
//
// Release lock
//
			ReleaseMutex(v_lock.m_lock_h);
		}
		else
		{
			DWORD	dwError;
			CString temp;

			dwError = GetLastError();

			temp.Format(_TEXT("dwWaitStatus = %ld, GetLastError = X%08x"),
						dwWaitStatus,
						dwError);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 5,
				CString(_TEXT("g00_unregister_thread: wait error")),
				temp,
				G01_SE_RAISED);
		}
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 6,
			CString(_TEXT("g00_unregister_thread")),
			CString(_TEXT("NULL lock handle")),
			G01_SE_RAISED);
	}
} // g00_unregister_thread

///////////////////////////////////////////////////////////////////////////////
// Function Description : Wait for child threads to terminate
// Author               : Duncan Olesen
// Date Created         : 03/06/96
// Date Last Modified   : 03/06/96
///////////////////////////////////////////////////////////////////////////////

void g00_wait_for_child_termination(DWORD	p_dwTimeout)
{
	DWORD			dwWaitStatus;

	if (v_lock.m_lock_h != NULL)
	{
//
// Get lock on global data
//
		dwWaitStatus = WaitForSingleObject(v_lock.m_lock_h, INFINITE);

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
//
// Flag that process is terminating, no further deletions to be made from
// thread info list
//
			v_terminating_z = TRUE;
//
// Release lock
//
			ReleaseMutex(v_lock.m_lock_h);
//
// Check that called by primary thread
//
			if (GetCurrentThreadId() != v_primary_thread)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 7,
					CString(_TEXT("g00_wait_for_child_termination")),
					CString(_TEXT("Not called by primary thread")),
					G01_SE_RAISED);
			}
			else
			{
				int				max_threads;
				w				num_children_w;
				POSITION		map_position;
				DWORD			dwThreadId;
				S_thread_info	thread_info;
				C_g02_timeout_control	timeout(p_dwTimeout);

				max_threads = v_thread_info_map.GetCount();
				num_children_w = 0;
//
// Loop to wait on handles of child threads,
// i.e. do not include the current (primary) thread
//
				map_position = v_thread_info_map.GetStartPosition();

				while (map_position != NULL)
				{
					v_thread_info_map.GetNextAssoc(
										map_position,
										dwThreadId,
										thread_info);

					if (dwThreadId != v_primary_thread)
					{
						DWORD		dwWaitStatus;

						num_children_w++;

						dwWaitStatus = WaitForSingleObject(
											thread_info.m_handle_h,	// hObject
											timeout.time_left());	// dwTimeout

//
// Report error if wait unsuccessful
//
						if (dwWaitStatus == WAIT_TIMEOUT)
						{
							CString temp;

							temp.Format(_T("p_dwTimeout = %ld, Thread Num = %hd, ")
										_T("Thread Inst = %hd"),
										p_dwTimeout,
										thread_info.m_thread_num_w,
										thread_info.m_thread_inst_w);

							g01_log_system_event(
								G01_SE_SOFTWARE_EXCEPTION,
								G01_SE_ERROR,
								G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 8,
								CString(_TEXT("g00_wait_for_child_termination: wait timeout")),
								temp,
								G01_SE_RAISED);
						}
						else if (dwWaitStatus != WAIT_OBJECT_0)
						{
							DWORD	dwError;
							CString temp;

							dwError = GetLastError();

							temp.Format(_T("dwWaitStatus = %ld, GetLastError = X%08x, ")
										_T("Thread Num = %hd, Thread Inst = %hd"),
										dwWaitStatus,
										dwError,
										thread_info.m_thread_num_w,
										thread_info.m_thread_inst_w);

							g01_log_system_event(
								G01_SE_SOFTWARE_EXCEPTION,
								G01_SE_ERROR,
								G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 9,
								CString(_TEXT("g00_wait_for_child_termination: wait error")),
								temp,
								G01_SE_RAISED);
						} // wait error
					} // not primary thread
				} // while threads
//
// Report error if no child threads found
//
				if (num_children_w == 0)
				{
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_ERROR,
						G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 10,
						CString(_TEXT("g00_wait_for_child_termination")),
						CString(_TEXT("No registered children")),
						G01_SE_RAISED);
				}
			}	// called by primary thread
		}
		else
		{
			DWORD	dwError;
			CString temp;

			dwError = GetLastError();

			temp.Format(_TEXT("dwWaitStatus = %ld, GetLastError = X%08x"),
						dwWaitStatus,
						dwError);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 11,
				CString(_TEXT("g00_wait_for_child_termination: wait error")),
				temp,
				G01_SE_RAISED);
		}
	}
	else
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G00_CODE_BASE_W + REGISTRY_CODE_BASE_W + 12,
			CString(_TEXT("g00_wait_for_child_termination")),
			CString(_TEXT("NULL lock handle")),
			G01_SE_RAISED);
	}
} // g00_wait_for_child_termination

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get current application name
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_app_name(C_g00_app_name &p_app_name)
{
	v_crnt_app_name.app_name(p_app_name);
} // g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get current application name string
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_app_name(tc *p_app_name_tcp)
{
	v_crnt_app_name.app_name(p_app_name_tcp);
} // g00_app_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get current thread name
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_thread_name(C_g00_thread_name &p_thread_name)
{
	DWORD			dwThreadId;
	S_thread_info	thread_info;
	z				found_z;
//
// Get the id for the thread calling this function
//
	dwThreadId = GetCurrentThreadId();
	found_z = v_thread_info_map.Lookup(dwThreadId, thread_info);

	if (found_z == TRUE)
	{
		p_thread_name.name_thread(
			v_crnt_app_name,
			thread_info.m_thread_num_w,
			thread_info.m_thread_inst_w);
	}
	else
	{
		p_thread_name.name_thread(
			P00_UNKNOWN,
			(w) 0,
			thread_info.m_thread_num_w,
			thread_info.m_thread_inst_w);
	}
} // g00_thread_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get current thread name string
// Author               : Duncan Olesen
// Date Created         : 26/03/96
// Date Last Modified   : 26/03/96
///////////////////////////////////////////////////////////////////////////////

void g00_thread_name(tc *p_thread_name_tcp)
{
	DWORD				dwThreadId;
	S_thread_info		thread_info;
	C_g00_thread_name	thread_name;
	z					found_z;
//
// Get the id for the thread calling this function
//
	dwThreadId = GetCurrentThreadId();
	found_z = v_thread_info_map.Lookup(dwThreadId, thread_info);

	thread_name.name_thread(
		v_crnt_app_name,
		thread_info.m_thread_num_w,
		thread_info.m_thread_inst_w);

	if (found_z == TRUE)
	{
		thread_name.thread_name(p_thread_name_tcp);
	}
	else
	{
		p_thread_name_tcp[0] = _TEXT('\0');
	}
} // g00_thread_name

