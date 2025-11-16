///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : G01_event_logging.h
// File Type   : C++ header
// Description : Event logging definitions
// Author      : Duncan Olesen; modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G01_EVENT_LOGGING_H
#define _G01_EVENT_LOGGING_H

#include <G00.h>                // Services DLL import/export control
#include <g02_datetime.h>       // ...
#include <g02_datetime_span.h>  // ...
#include <standard.h>           // Standard definitions
#include <portable_types.h>  // Type definitions

const w G01_SE_STRING_LEN_W   = 80;
const w G01_SE_ADD_INFO_LEN_W = 160;

const w G01_SE_COMMS_CODE_BASE_W = 1000;
const w G01_SE_TIMER_CODE_BASE_W = 2000;
const w G01_SE_G00_CODE_BASE_W	 = 3000;
const w G01_SE_G01_CODE_BASE_W	 = 3200;
const w G01_SE_G02_CODE_BASE_W	 = 3400;
const w G01_SE_G03_CODE_BASE_W	 = 3600;

const w G01_SE_APP_CODE_BASE_W	 = 5000;


enum T_g01_se_type
{
    G01_SE_SOFTWARE_EXCEPTION   =  0,
    G01_SE_SOFTWARE_WARNING     =  1,
    G01_SE_SOFTWARE_INFORMATION =  2
};

enum T_g01_se_severity
{
    G01_SE_PLATFORM_FATAL    = 30,
    G01_SE_APPLICATION_FATAL = 20,
    G01_SE_THREAD_FATAL      = 15,
    G01_SE_ERROR             = 10,
    G01_SE_WARNING           = 5,
    G01_SE_INFO              = 1
};

enum T_g01_se_status
{
    G01_SE_RAISED  = 0,
    G01_SE_CLEARED = 1
};

// Log system event
void SERVICES_EXTERNAL g01_log_system_event(T_g01_se_type     p_type,
                                            T_g01_se_severity p_severity,
                                            w                 p_code_w,
                                            const CString&    p_string,
                                            const CString&    p_add_info,
                                            T_g01_se_status   p_status);

#endif // _G01_EVENT_LOGGING_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////
