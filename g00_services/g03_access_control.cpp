///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : access_control.cpp
// File Type   : C++ Source
// Description : Exclusive Access Control
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>
#include <g03_access_control.h>

#include "g03_event_codes.h"

const	m	ACCESS_CONTROL_MAGIC_NUM_M = 0x59AB95CD;

tc *mutex_name(const tc *p_access_control_name_tcp);
tc *memory_name(const tc *p_access_control_name_tcp);

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_access_control default constructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_g03_access_control::C_g03_access_control()
: m_shared_memory()
{
	m_lock_h							= NULL;
	m_local_holder.m_lock_held_z		= FALSE;
	m_local_holder.m_dwLockingThreadId	= 0;
	m_lock_holder_p						= NULL;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_access_control constructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_g03_access_control::C_g03_access_control(const tc *pc_name_tcp)
: m_shared_memory()
{
	m_lock_h							= NULL;
	m_local_holder.m_lock_held_z		= FALSE;
	m_local_holder.m_dwLockingThreadId	= 0;
	m_lock_holder_p						= NULL;
//
// If name is null or of zero length then consider the object to
// be unnamed. Thus the lock holder information is held locally
//
	if (pc_name_tcp == NULL ||
		_tcslen(pc_name_tcp) == 0)
	{
		m_lock_h		= CreateMutex(NULL, FALSE, NULL);
		m_lock_holder_p	= &m_local_holder;
	}
	else
	{
		initialise(pc_name_tcp);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_access_control destructor
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

C_g03_access_control::~C_g03_access_control()
{
	if (m_lock_h != (HANDLE) NULL)
	{
		CloseHandle(m_lock_h);
	}
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise C_g03_access_control object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_access_control::initialise(const tc *pc_name_tcp)
{
	m	*magic_num_mp;
	tc	*mutex_name_tcp;
	tc	*memory_name_tcp;

	mutex_name_tcp  = NULL;
	memory_name_tcp = NULL;
//
// Validate parameter
//
	if (pc_name_tcp == NULL ||
		_tcslen(pc_name_tcp) == 0)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 1,
			CString(_TEXT("C_g03_access_control::initialise: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Get names for mutex and shared memory
//
	mutex_name_tcp = mutex_name(pc_name_tcp);
	memory_name_tcp = memory_name(pc_name_tcp);

	if (mutex_name_tcp == NULL ||
		memory_name_tcp == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 2,
			CString(_TEXT("C_g03_access_control::initialise: name build error")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Validate usage
//
	else if (m_lock_holder_p != NULL)
	{
//
// Invalid usage of the method
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 3,
			CString(_TEXT("C_g03_access_control::initialise: already initiated")),
			CString(pc_name_tcp),
			G01_SE_RAISED);
	}
	else
	{
//
// Create a Mutex to enable mutual exclusion
//
		m_lock_h = CreateMutex(NULL, FALSE, mutex_name_tcp);

		if (m_lock_h == NULL)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 4,
				CString(_TEXT("C_g03_access_control::initialise: no mutex handle")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Create shared memory object
//
			magic_num_mp = (m *) m_shared_memory.configure(
						memory_name_tcp,
						sizeof(m) + sizeof(S_lock_holder));

			if (magic_num_mp == NULL)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 5,
					CString(_TEXT("C_g03_access_control::initialise: no shared memory")),
					CString(_TEXT("")),
					G01_SE_RAISED);

				CloseHandle(m_lock_h);
				m_lock_h = NULL;
			}
			else
			{
				*magic_num_mp = ACCESS_CONTROL_MAGIC_NUM_M;
				m_lock_holder_p = (S_lock_holder *) (magic_num_mp + 1);
				*m_lock_holder_p = m_local_holder;
			}
		}
	}
//
// Free memory used for names
//
	if (mutex_name_tcp != NULL)
	{
		delete [] mutex_name_tcp;
	}

	if (memory_name_tcp != NULL)
	{
		delete [] memory_name_tcp;
	}

	return;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Attach to Initialise C_g03_access_control object
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_access_control::attach(const tc *pc_name_tcp)
{
	m	*magic_num_mp;
	tc	*mutex_name_tcp;
	tc	*memory_name_tcp;

	mutex_name_tcp  = NULL;
	memory_name_tcp = NULL;
//
// Validate parameter
//
	if (pc_name_tcp == NULL ||
		_tcslen(pc_name_tcp) == 0)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 6,
			CString(_TEXT("C_g03_access_control::attach: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Get names for mutex and shared memory
//
	mutex_name_tcp = mutex_name(pc_name_tcp);
	memory_name_tcp = memory_name(pc_name_tcp);

	if (mutex_name_tcp == NULL ||
		memory_name_tcp == NULL)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 7,
			CString(_TEXT("C_g03_access_control::initialise: name build error")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Validate usage
//
	if (m_lock_holder_p != NULL)
	{
//
// Invalid usage of the method
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 8,
			CString(_TEXT("C_g03_access_control::attach: already initiated")),
			CString(pc_name_tcp),
			G01_SE_RAISED);
	}
	else
	{
//
// Create a Mutex to enable mutual exclusion
//
		m_lock_h = CreateMutex(NULL, FALSE, mutex_name_tcp);

		if (m_lock_h == NULL)
		{
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 9,
				CString(_TEXT("C_g03_access_control::attach: no mutex handle")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Attach to shared memory object
//
			m_shared_memory.attach(memory_name_tcp);

			magic_num_mp = (m *) m_shared_memory.address();

			if (magic_num_mp == NULL ||
				*magic_num_mp != ACCESS_CONTROL_MAGIC_NUM_M)
			{
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 10,
					CString(_TEXT("C_g03_access_control::attach: shared memory error")),
					CString(_TEXT("")),
					G01_SE_RAISED);

				CloseHandle(m_lock_h);
				m_lock_h = NULL;
			}
			else
			{
				m_lock_holder_p = (S_lock_holder *) (magic_num_mp + 1);
			}
		}
	}
//
// Free memory used for names
//
	if (mutex_name_tcp != NULL)
	{
		delete [] mutex_name_tcp;
	}

	if (memory_name_tcp != NULL)
	{
		delete [] memory_name_tcp;
	}

	return;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Obtain exclusive access
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

T_g03_lock_status C_g03_access_control::lock(
							const C_g02_timeout_control	&pc_timeout,
							m							p_id_m)
{
	return lock(pc_timeout.time_left(), p_id_m);
} // lock

///////////////////////////////////////////////////////////////////////////////
// Function Description : Obtain exclusive access
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

T_g03_lock_status C_g03_access_control::lock(
							DWORD	p_dwTimeout,
							m		p_id_m)
{
	l					wait_ret_val_l;
	T_g03_lock_status	ret_val;

	ret_val = LOCK_ERROR;

	if (m_lock_h == NULL)
	{
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_INFO,
				G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 11,
				CString(_TEXT("C_g03_access_control::lock: lock not initialised")),
				CString(_TEXT("")),
				G01_SE_RAISED );
	}
	else if (lock_held() == FALSE)
	{
//
// Obtain exclusive access
//
#ifdef _DEBUG
//
// In debug build interrupt an infinite wait after 5 minutes and report
// the thread and lock id of the owner of the lock
//
		if (p_dwTimeout == INFINITE)
		{
			wait_ret_val_l = WaitForSingleObject(m_lock_h, 300000);

			if (wait_ret_val_l == WAIT_TIMEOUT)
			{
				CString temp;

				temp.Format(_TEXT("Lock id = %d, held by %s"),
							m_lock_holder_p->m_lock_id_m,
							m_lock_holder_p->m_locking_thread_name_tca);

				g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_INFO,
						G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 12,
						CString(_TEXT("C_g03_access_control::lock: lock not obtained in 20s")),
						temp,
						G01_SE_RAISED);

				wait_ret_val_l = WaitForSingleObject(m_lock_h, p_dwTimeout);
			}
		}
		else
		{
#endif _DEBUG
		wait_ret_val_l = WaitForSingleObject(m_lock_h, p_dwTimeout);
#ifdef _DEBUG
		}
#endif _DEBUG

		if (wait_ret_val_l == WAIT_OBJECT_0)
		{
			m_lock_holder_p->m_lock_held_z		 = TRUE;
			m_lock_holder_p->m_dwLockingThreadId = GetCurrentThreadId();
			m_lock_holder_p->m_lock_id_m		 = p_id_m;
#ifdef _DEBUG
			g00_thread_name(m_lock_holder_p->m_locking_thread_name_tca);
#endif _DEBUG
			ret_val = LOCKED;
		}
		else if (wait_ret_val_l == WAIT_TIMEOUT)
		{
			ret_val = LOCK_TIMEOUT;
		}
		else
		{
			ret_val = LOCK_ERROR;
		}
	}
	else
	{
//
// Log attempt by a thread to re-obtain a lock
//
		CString temp;

		temp.Format(_TEXT("Current Thread Id = %d"), GetCurrentThreadId());

		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_INFO,
				G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 13,
				CString(_TEXT("C_g03_access_control::lock: lock already held")),
				temp,
				G01_SE_RAISED );
	}

	return ret_val;
} // lock

///////////////////////////////////////////////////////////////////////////////
// Function Description : Release exclusive access
// Author               : Duncan Olesen
// Date Created         : 28/03/96
// Date Last Modified   : 28/03/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_access_control::unlock(m	p_id_m)
{
	if (lock_held())
	{
		if (p_id_m != 0 && p_id_m != m_lock_holder_p->m_lock_id_m)
		{
			CString temp;

			temp.Format(_TEXT("Lock obtained Id = %d, release Id = %d"),
						m_lock_holder_p->m_lock_id_m,
						p_id_m);

			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_WARNING,
					G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 14,
					CString(_TEXT("C_g03_access_control::unlock: id mismatch")),
					temp,
					G01_SE_RAISED );
		}

		m_lock_holder_p->m_dwLockingThreadId = 0;
		m_lock_holder_p->m_lock_held_z		 = FALSE;

		ReleaseMutex(m_lock_h);
	}
	else
	{
//
// Log attempt to release a lock not held by current thread
//
		CString temp;

		if (m_lock_holder_p != NULL)
		{
			temp.Format(_T("Current Thread Id = %d, m_lock_held_z = %d")
						_T(", m_dwLockingThreadId = %d"),
						GetCurrentThreadId(),
						m_lock_holder_p->m_lock_held_z,
						m_lock_holder_p->m_dwLockingThreadId);
		}
		else
		{
			temp.Format(_TEXT("Current Thread Id = %d"),
						GetCurrentThreadId());
		}

		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_INFO,
				G01_SE_G03_CODE_BASE_W + ACCESS_CONTROL_CODE_BASE_W + 15,
				CString(_TEXT("C_g03_access_control::unlock: lock not held")),
				temp,
				G01_SE_RAISED );
	}
} // unlock

///////////////////////////////////////////////////////////////////////////////
// Function Description : Returns TRUE if current thread holds the lock
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 17/04/96
///////////////////////////////////////////////////////////////////////////////

z C_g03_access_control::lock_held() const
{
	z	ret_val_z;

	if (m_lock_holder_p != NULL &&
		m_lock_holder_p->m_lock_held_z &&
		m_lock_holder_p->m_dwLockingThreadId == GetCurrentThreadId())
	{
		ret_val_z = TRUE;
	}
	else
	{
		ret_val_z = FALSE;
	}

	return ret_val_z;
} // lock_held

///////////////////////////////////////////////////////////////////////////////
// Function Description : Returns the name of the mutex
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

tc *mutex_name(const tc *p_access_control_name_tcp)
{
	tc	*ret_val_tcp;
	int	name_length;

	name_length = _tcslen(p_access_control_name_tcp) + 2;

	ret_val_tcp = new tc[name_length];

	if (ret_val_tcp != NULL)
	{
		_tcscpy(ret_val_tcp, p_access_control_name_tcp);
		_tcscat(ret_val_tcp, _TEXT("X"));
	}

	return ret_val_tcp;
} // mutex_name

///////////////////////////////////////////////////////////////////////////////
// Function Description : Returns the name of the shared memory
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

tc *memory_name(const tc *p_access_control_name_tcp)
{
	tc	*ret_val_tcp;
	int	name_length;

	name_length = _tcslen(p_access_control_name_tcp) + 2;

	ret_val_tcp = new tc[name_length];

	if (ret_val_tcp != NULL)
	{
		_tcscpy(ret_val_tcp, p_access_control_name_tcp);
		_tcscat(ret_val_tcp, _TEXT("M"));
	}

	return ret_val_tcp;
} // memory_name
