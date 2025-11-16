///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : G00_process_registry.h
// File Type   : C++ header
// Description : Application and thread register definitions
// Author      : Duncan Olesen; modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G00_PROCESS_REGISTRY_H
#define _G00_PROCESS_REGISTRY_H

#include <G00.h>    // Services DLL import/export control
#include <portable_types.h>  // Type definitions

#define _MAX_APP_TYPE_LEN 20
#define _MAX_APP_INST_LEN 5
// App name length includes space for type / instance separator character
#define _MAX_APP_NAME_LEN (_MAX_APP_TYPE_LEN + _MAX_APP_INST_LEN + 1)
#define _MAX_THREAD_TYPE_LEN 5
#define _MAX_THREAD_INST_LEN 5
// Thread name length includes space for type / instance separator characters
#define _MAX_THREAD_NAME_LEN (_MAX_APP_NAME_LEN + 1 +   \
                              _MAX_THREAD_TYPE_LEN +    \
                              _MAX_THREAD_INST_LEN + 1)

const w G00_MAX_APP_TYPE_LEN_W    = _MAX_APP_TYPE_LEN;
const w G00_MAX_APP_INST_LEN_W    = _MAX_APP_INST_LEN;
const w G00_MAX_APP_NAME_LEN_W    = _MAX_APP_NAME_LEN;
const w G00_MAX_THREAD_TYPE_LEN_W = _MAX_THREAD_TYPE_LEN;
const w G00_MAX_THREAD_INST_LEN_W = _MAX_THREAD_INST_LEN;
const w G00_MAX_THREAD_NAME_LEN_W = _MAX_THREAD_NAME_LEN;

// Items in the following list should be supported by app names in G00_process_registry.cpp
enum T_g00_app_type
{
    P00_UNKNOWN		= 0,
    P01_PLATO		= 1,	// Plato App
    P02_DBI			= 2,	// Plato or Kavo DB Interface Process
	P01_KAVO		= 3,	// Kavo App
	P01_TX_SYNC		= 4,	// TX-Sync Process
	P03_RCP			= 5,	// Plato Remote Control Panel
	P04_CATALOG		= 6,	// Cataloguing client
	P05_LMS			= 7,	// LMS Transfer App
	P06_PAWS_LITE	= 8,	// PAWS Lite application
	P01_ADEC		= 9		// ADEC application
};

const w TIMER_THREAD_NUM_W    = 129;

///////////////////////////////////////////////////////////////////////////////
// Define application instance class
///////////////////////////////////////////////////////////////////////////////
class SERVICES_EXTERNAL C_g00_app_name
{
public:
    C_g00_app_name();
    C_g00_app_name(T_g00_app_type p_app_type,
			       w              p_app_inst_w);
    C_g00_app_name(const C_g00_app_name& p_app_name);
    ~C_g00_app_name();

// Attributes
protected:
    T_g00_app_type m_app_type;
    w              m_app_inst_w;

// Implementation
public:
    inline void name_app(T_g00_app_type p_app_type,
                         w              p_app_inst_w)
    {
        m_app_type   = p_app_type;
        m_app_inst_w = p_app_inst_w;
    };
    inline void name_app(const C_g00_app_name& p_app_name)
    {
        *this = p_app_name;
    };
    inline void app_name(T_g00_app_type& p_app_type,
                         w&              p_app_inst_w) const
    {
        p_app_type   = m_app_type;
        p_app_inst_w = m_app_inst_w;
    };
    inline void app_name(C_g00_app_name& p_app_name) const
    {
        p_app_name = *this;
    };

    BOOL operator==(const C_g00_app_name& pc_app_name) const;
    BOOL operator!=(const C_g00_app_name& pc_app_name) const;

//
// Sets up string to contain system unique name derived
// from application name. The string will have a maximum
// length of G00_MAX_APP_NAME_LEN including terminating zero.
//
    void app_name(tc* p_app_name_tcp) const;
};

///////////////////////////////////////////////////////////////////////////////
// Define thread instance class
///////////////////////////////////////////////////////////////////////////////
class SERVICES_EXTERNAL C_g00_thread_name : C_g00_app_name
{
// Attributes
private:
    w m_thread_num_w;
    w m_thread_inst_w;

// Implementation
public:
    C_g00_thread_name();
    C_g00_thread_name(T_g00_app_type p_app_type,
                      w              p_app_inst_w,
                      w              p_thread_num_w,
                      w              p_thread_inst_w);
    C_g00_thread_name(const C_g00_app_name& p_app_name,
                      w                     p_thread_num_w,
                      w                     p_thread_inst_w);
    ~C_g00_thread_name();

    inline void name_thread(T_g00_app_type p_app_type,
                            w              p_app_inst_w,
                            w              p_thread_num_w,
                            w              p_thread_inst_w)
    {
        name_app(p_app_type, p_app_inst_w);
        m_thread_num_w  = p_thread_num_w;
        m_thread_inst_w = p_thread_inst_w;
    };
    inline void name_thread(const C_g00_app_name& p_app_name,
                            w                     p_thread_num_w,
                            w                     p_thread_inst_w)
    {
        name_app(p_app_name);
        m_thread_num_w  = p_thread_num_w;
        m_thread_inst_w = p_thread_inst_w;
    };
    inline void thread_name(T_g00_app_type& p_app_type,
                            w&              p_app_inst_w,
                            w&              p_thread_num_w,
                            w&              p_thread_inst_w) const
    {
        app_name(p_app_type, p_app_inst_w);
        p_thread_num_w  = m_thread_num_w;
        p_thread_inst_w = m_thread_inst_w;
    };
    inline void thread_name(w& p_thread_num_w,
                            w& p_thread_inst_w) const
    {
        p_thread_num_w  = m_thread_num_w;
        p_thread_inst_w = m_thread_inst_w;
    };

    BOOL operator==(const C_g00_thread_name& pc_thread_name) const;
    BOOL operator!=(const C_g00_thread_name& pc_thread_name) const;
//
// Sets up string to contain system unique name derived
// from thread name. The string will have a maximum length
// of G00_MAX_THREAD_NAME_LEN including terminating zero.
//
    void thread_name(tc* p_thread_name_tcp) const;
};

// Register current platform
void SERVICES_EXTERNAL g00_register_platform(const tc* pc_platform_tcp);
void SERVICES_EXTERNAL g00_unregister_platform();

// Register current application
void SERVICES_EXTERNAL g00_register_app(const C_g00_app_name& p_app_name);
void SERVICES_EXTERNAL g00_register_app(T_g00_app_type p_app_type,
                                        w              p_app_inst_w);
void SERVICES_EXTERNAL g00_unregister_app();

// Register current thread
void SERVICES_EXTERNAL g00_register_thread(const C_g00_thread_name& p_thread_name);
void SERVICES_EXTERNAL g00_register_thread(w p_thread_num_w,
                                           w p_thread_inst_w);

// Unregister thread should only be called by threads that do not run for
// the entire life of the application
void SERVICES_EXTERNAL g00_unregister_thread();

// Called by the primary thread to wait for all its registered child threads
// to terminate, default of one minute
void SERVICES_EXTERNAL g00_wait_for_child_termination(DWORD	p_dwTimeout = 60000);

// Get current application name
void SERVICES_EXTERNAL g00_app_name(C_g00_app_name& p_app_name);
void SERVICES_EXTERNAL g00_app_name(tc* p_app_name_tcp);

// Get current thread name
void SERVICES_EXTERNAL g00_thread_name(C_g00_thread_name& p_thread_name);
void SERVICES_EXTERNAL g00_thread_name(tc* p_thread_name_tcp);

#endif  // _G00_PROCESS_REGISTRY_H

///////////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////////
