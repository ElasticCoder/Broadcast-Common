///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_systemtime.h
// File Type   : C++ Source
// Description : Convert Date and Time components to and from SYSTEMTIME,
//			   : FILETIME and time_t
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions

#include <G02.h>
#include "g02_systemtime.h"

static TIME_ZONE_INFORMATION	v_tz_info;
static TIME_ZONE_INFORMATION	v_inv_tz_info;
static z						v_tz_uninit_z = TRUE;

const m FT_UNITS_PER_MILLISEC_M	= 10000;
        
///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts _SYSTEMTIME structure to Day and Milli-Second
//                      : components
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 30/05/96
///////////////////////////////////////////////////////////////////////////////////	

z g02_systemtime_to_datetime(
						const SYSTEMTIME	&p_systemtime,
						m					&p_days_m,
						m					&p_time_m)
{
	z			ret_val_z;
	FILETIME	FT;			
	SYSTEMTIME	ST;
	__int64 file_time;

	ret_val_z = TRUE;

//
// Determine milliseconds since start of day
//
	p_time_m = 	(m)p_systemtime.wHour * G02_MILLISECS_PER_HOUR_M +
				(m)p_systemtime.wMinute * G02_MILLISECS_PER_MINUTE_M +
				(m)p_systemtime.wSecond * G02_MILLISECS_PER_SECOND_M +
				(m)p_systemtime.wMilliseconds;
//
// Setup structure to determine days
//
	ST = p_systemtime;
	ST.wHour=0;
	ST.wMinute=0;
	ST.wSecond=0;
	ST.wMilliseconds=0;
//
// Convert date from system time to file time
//
	if (SystemTimeToFileTime(&ST,&FT)==FALSE)
	{
		ret_val_z = FALSE;
	}
	else
	{
//
// Load file time into a 64 bit integer
//
		file_time = FT.dwHighDateTime;
		file_time <<= 32;
		file_time += FT.dwLowDateTime;
//
// Convert file time 100 nanosecond units to days
//
		file_time = file_time / FT_UNITS_PER_MILLISEC_M;
		file_time = file_time / G02_MILLISECS_PER_DAY_M;
		p_days_m = m(file_time);
	}

	return ret_val_z;
	
} // g02_systemtime_to_datetime

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts Day and Milli-Second components to _SYSTEMTIME
//						: structure
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 30/05/96
///////////////////////////////////////////////////////////////////////////////////	

z g02_datetime_to_systemtime(
						const m		&p_days_m,
						const m		&p_time_m,
						SYSTEMTIME	&p_systemtime)
{
	z			ret_val_z;
	FILETIME	FT;
//
// convert datetime to FileTime
//
	g02_datetime_to_filetime(p_days_m, p_time_m, FT);
//
// convert FileTime to system time
//
	ret_val_z = FileTimeToSystemTime(&FT,&p_systemtime);

	return ret_val_z;
} // g02_datetime_to_systemtime

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts FILETIME structure to Day and Milli-Second
//                      : components
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////////	

void g02_filetime_to_datetime(
						const FILETIME	&p_filetime,
						m				&p_days_m,
						m				&p_time_m)
{
	__int64 file_time;
	__int64 days;
//
// Load file time into a 64 bit integer
//
	file_time = p_filetime.dwHighDateTime;
	file_time <<= 32;
	file_time += p_filetime.dwLowDateTime;
//
// Convert file time 100 nanosecond units to milliseconds
//
	file_time = file_time / FT_UNITS_PER_MILLISEC_M;
//
// Calculate number of days
//
	days = file_time / G02_MILLISECS_PER_DAY_M;
	p_days_m = m(days);
//
// Calculate the number of milliseconds since start of day
//
	p_time_m = m(file_time - days * G02_MILLISECS_PER_DAY_M);
} // g02_filetime_to_datetime

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts Day and Milli-Second components to FILETIME
//						: structure
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////////	

void g02_datetime_to_filetime(
						const m		&p_days_m,
						const m		&p_time_m,
						FILETIME	&p_filetime)
{
	__int64		file_time;
//
// Convert days into millisecond units
//
	file_time = p_days_m;
	file_time *= G02_MILLISECS_PER_DAY_M;
//
// Add time component in milliseconds
//
	file_time += p_time_m;
//
// Convert milliseconds into 100 nano second units
//
	file_time *= FT_UNITS_PER_MILLISEC_M;
//
// Setup FILETIME structure
//
	p_filetime.dwLowDateTime = unsigned long (file_time);
	file_time >>= 32;
	p_filetime.dwHighDateTime = unsigned long(file_time);
} // g02_datetime_to_filetime


	
///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts time_t structure to Day and Milli-Second
//                      : components
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////////	

void g02_time_t_to_datetime(
						const time_t	&p_time,
						m				&p_days_m,
						m				&p_time_m)
{
	m	days_since_1970_m;

	days_since_1970_m = m(p_time / G02_SECONDS_PER_DAY_M);
	p_days_m = days_since_1970_m + G02_DAYS_1601_TO_1970_M;

	p_time_m = m((p_time - days_since_1970_m * G02_SECONDS_PER_DAY_M) *
				  G02_MILLISECS_PER_DAY_M);
} // g02_time_t_to_datetime

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts Day and Milli-Second components to time_t
//						: structure
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////////	

z g02_datetime_to_time_t(
						const m		&p_days_m,
						const m		&p_time_m,
						time_t		&p_time)
{
	z	ret_val_z;

	if (p_days_m < G02_DAYS_1601_TO_1970_M)
	{
//
// Date is too early for time_t
//
		p_time = 0;
		ret_val_z = FALSE;
	}
	else
	{
//
// Calculate number seconds in complete days since 1970
//
		p_time = (time_t) ((p_days_m - G02_DAYS_1601_TO_1970_M) *
							G02_SECONDS_PER_DAY_M);
//
// Add time of day in seconds
//
		p_time += (time_t) (p_time_m / G02_MILLISECS_PER_SECOND_M);
		ret_val_z = TRUE;
	}

	return ret_val_z;
} // g02_datetime_to_time_t

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : converts Day and Milli-Second components to time_t
//						: structure
// Author               : Duncan Olesen
// Date Created			: 30/05/96
// Date Last Modified   : 30/05/96
///////////////////////////////////////////////////////////////////////////////////	

z g02_local_systemtime_to_systemtime(
						const SYSTEMTIME	&p_systemtime,
						SYSTEMTIME			&p_utc_time)
{
	z						ret_val_z;
	SYSTEMTIME				local_time;
//
// Copy p_systemtime because SystemTimeToTzSpecificLocalTime
// does not use const
//
	local_time = p_systemtime;

	if (v_tz_uninit_z)
	{
		DWORD		tz_status;

		tz_status = GetTimeZoneInformation(&v_tz_info);

		if (tz_status != 0xFFFFFFFF)
		{
//
// Successfully obtained timezone information.
// In order to convert from local time to UTC we need to invert the
// timezone information and then call SystemTimeToTzSpecificLocalTime
//
			v_inv_tz_info = v_tz_info;
			v_inv_tz_info.Bias			= -v_tz_info.Bias;
			v_inv_tz_info.StandardBias	= -v_tz_info.StandardBias;
			v_inv_tz_info.DaylightBias	= -v_tz_info.DaylightBias;

			v_tz_uninit_z = FALSE;
		}
	}

	ret_val_z = SystemTimeToTzSpecificLocalTime(
					&v_inv_tz_info,	// lpTimeZoneInformation
					&local_time,	// lpUniversalTime
					&p_utc_time);	// lpLocalTime

	return ret_val_z;
} // g02_local_systemtime_to_systemtime

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : Returns the format to be used for dates and the separator
// Author               : Duncan Olesen
// Date Created			: 15/07/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////////	

void g02_date_format(
						w		&p_date_format_w,
						CString	&p_separator)
{
	LONG	open_status;
	LONG	value_status;
	HKEY	format_key;
	const	w	VALUE_STRING_LEN_W = 15;
	tc		value_tca[VALUE_STRING_LEN_W];
	DWORD	buffer_size;

	open_status = RegOpenKeyEx(
					HKEY_CURRENT_USER,	// handle of open key
					_TEXT("Control Panel\\International"),	// name of subkey to open
					0,					// reserved
					KEY_QUERY_VALUE,	// security access mask
					&format_key);		// handle for open key

	if (open_status == ERROR_SUCCESS)
	{
//
// Read iDate - date format
//
		buffer_size = VALUE_STRING_LEN_W * sizeof(tc);

		value_status = RegQueryValueEx(
						format_key,		// handle of key to query
						_TEXT("iDate"),	// name of value to query
						NULL,			// reserved
						NULL,			// buffer for value type
						(b *) value_tca,	// buffer for data
						&buffer_size);	// size of data buffer

		if (value_status == ERROR_SUCCESS)
		{
			if (_stscanf(value_tca, _TEXT("%hd"), &p_date_format_w) != 1)
			{
				p_date_format_w = 1;
			}
		}
		else
		{
//
// Could not get value from the registry so default to 31/12/90 format
//
			p_date_format_w = 1;
		}
//
// Read sDate - date separator
//
		buffer_size = VALUE_STRING_LEN_W * sizeof(tc);

		value_status = RegQueryValueEx(
						format_key,		// handle of key to query
						_TEXT("sDate"),	// name of value to query
						NULL,			// reserved
						NULL,			// buffer for value type
						(b *) value_tca,	// buffer for data
						&buffer_size);	// size of data buffer

		if (value_status == ERROR_SUCCESS)
		{
			p_separator = value_tca;
		}
		else
		{
//
// Could not get value from the registry so default '/'
//
			p_separator = _TEXT("/");
		}
//
// Close Key
//
		RegCloseKey(format_key);
	}
	else
	{
//
// Could not open registry so default to 31/12/90 format, '/' as separator
//
		p_date_format_w = 1;
		p_separator = _TEXT("/");
	}
} // g02_date_format
