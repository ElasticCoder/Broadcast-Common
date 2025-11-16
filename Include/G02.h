///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : G02.h       
// File Type   : C++ Header
// Description : Time/Timer event defintions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_H
#define _G02_H

#include <portable_types.h>  // Type definitions

// Time/Timer event logging code bases
const w TIMER_CODE_BASE_W           = 0;
const w TIMER_THREAD_CODE_BASE_W    = 100;
const w TIMEOUT_CONTROL_CODE_BASE_W = 200;
const w DATETIME_CODE_BASE_W        = 300;
const w DATETIME_SPAN_CODE_BASE_W   = 400;

const m G02_DAYS_1601_TO_1970_M     = 134774;

const m G02_MILLISECS_PER_SECOND_M  = 1000;
const m G02_MILLISECS_PER_MINUTE_M  = 60000;
const m G02_MILLISECS_PER_HOUR_M    = 60*60000;
const m G02_MILLISECS_PER_DAY_M     = 60*60*24000;

const m G02_SECONDS_PER_MINUTE_M    = 60;
const m G02_SECONDS_PER_HOUR_M      = 60*60;
const m G02_SECONDS_PER_DAY_M       = 60*60*24;

const m G02_MINUTES_PER_DAY_M       = 60*24;
const m G02_MINUTES_PER_HOUR_M      = 60;
const m G02_HOURS_PER_DAY_M         = 24;

#endif  // _G02_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////
