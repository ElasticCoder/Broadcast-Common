///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_systemtime.
// File Type   : C++ Header
// Description : Convert Date and Time components to and from SYSTEMTIME,
//			   : FILETIME and time_t
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <G00.h>
#include <G02.h>

z SERVICES_EXTERNAL g02_systemtime_to_datetime(
						const SYSTEMTIME	&p_systemtime,
						m					&p_days_m,
						m					&p_time_m);

z SERVICES_EXTERNAL g02_datetime_to_systemtime(
						const m		&p_days_m,
						const m		&p_time_m,
						SYSTEMTIME	&p_systemtime);

void SERVICES_EXTERNAL g02_filetime_to_datetime(
						const FILETIME	&p_filetime,
						m				&p_days_m,
						m				&p_time_m);

void SERVICES_EXTERNAL g02_datetime_to_filetime(
						const m		&p_days_m,
						const m		&p_time_m,
						FILETIME	&p_filetime);

void SERVICES_EXTERNAL g02_time_t_to_datetime(
						const time_t	&p_time,
						m				&p_days_m,
						m				&p_time_m);

z SERVICES_EXTERNAL g02_datetime_to_time_t(
						const m		&p_days_m,
						const m		&p_time_m,
						time_t		&p_time);

z SERVICES_EXTERNAL g02_local_systemtime_to_systemtime(
						const SYSTEMTIME	&p_systemtime,
						SYSTEMTIME			&p_utc_time);
//
// g02_date_format returns information on how a date should
// be formatted and the string to use to separate fields
//
// p_date_format_w:
//		0 - 12/31/90
//		1 - 31/12/90
//		2 - 90/12/31
//
void SERVICES_EXTERNAL g02_date_format(
						w		&p_date_format,
						CString	&p_separator);
