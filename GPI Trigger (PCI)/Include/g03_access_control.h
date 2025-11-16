///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : access_control.h       
// File Type   : C++ header
// Description : Exclusive access control definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G03_ACCESS_CONTROL_H
#define _G03_ACCESS_CONTROL_H

#include <portable_types.h>		  // Type definitions
#include <G00.h>                  // Services DLL import/export control
#include <g02_timeout_control.h>  // ...
#include <g03_shared_memory.h>    // ...

#ifdef _DEBUG
//
// Include g00_process_registry.h for debugging unreleased locks
//
#include <g00_process_registry.h>
#endif _DEBUG

const m G03_ACCESS_CONTROL_NAME_LEN_M = 63;

enum T_g03_lock_status
{
	LOCKED,
	LOCK_TIMEOUT,
	LOCK_ERROR
};

struct S_g03_lock_holder;

class SERVICES_EXTERNAL C_g03_access_control
{
public:
	C_g03_access_control();
	C_g03_access_control(const tc *pc_name_tcp);
	~C_g03_access_control();

// Attributes
private:
	HANDLE	m_lock_h;

	struct S_lock_holder
	{
		z		m_lock_held_z;
		DWORD	m_dwLockingThreadId;
		m		m_lock_id_m;
#ifdef _DEBUG
		tc		m_locking_thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
#endif _DEBUG
	};

//
// The identity of the lock holder is held in member variables if
// the access control object is unnamed.
// The identity of the lock holder is held in shared memory if the
// access control object is named
//
	z					m_shared_lock_info_z;
	C_g03_shared_memory	m_shared_memory;
	S_lock_holder		*m_lock_holder_p;
	S_lock_holder		m_local_holder;

// Implementation
public:
	void				initialise(const tc *pc_name_tcp);
	void				attach(const tc *pc_name_tcp);
//
// Obtain lock with timeout.
// The optional id is for debugging purposes to identify
// where an application is not releasing a lock
//
	T_g03_lock_status	lock(const C_g02_timeout_control &pc_timeout,
							 m							 p_id_m = 0);
	T_g03_lock_status	lock(DWORD p_dwTimeout,
							 m							 p_id_m = 0);
//
// Release lock. If p_id_m != 0 then it should match the id used when
// the lock was obtained
//
	void				unlock(m	p_id_m = 0);
	z					lock_held() const; // TRUE if current thread holds lock
};

#endif  // _G03_ACCESS_CONTROL_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
