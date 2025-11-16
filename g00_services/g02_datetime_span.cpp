///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_datetime_span.cpp
// File Type   : C++ Source
// Description : Date and Time Span (duration) Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>
#include <g02_datetime_span.h>
#include "g02_find_field.h"


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span constructor
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::C_g02_datetime_span()
: m_datetime()
{
	m_negative_z = FALSE;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span constructor
// Author               : Duncan Olesen
// Date Created         : 19/04/96
// Date Last Modified   : 19/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::C_g02_datetime_span(
					const m p_days_m,
					const w p_hours_w,
					const w p_mins_w,
					const w p_secs_w,
					const w p_millisecs_w,
					const z	p_negative_z)
{
	set_span(
			p_days_m,
			p_hours_w,
			p_mins_w,
			p_secs_w,
			p_millisecs_w,
			p_negative_z);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span constructor
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::C_g02_datetime_span(const l p_time_span_l)
: m_datetime()
{
	set_span(p_time_span_l);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span constructor
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::C_g02_datetime_span(
				const m p_days_m,
				const m p_millisecs_m,
				const z	p_negative_z)
: m_datetime(p_days_m, p_millisecs_m)
{
	m_negative_z = !(!p_negative_z);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span copy constructor
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::C_g02_datetime_span(
			const C_g02_datetime_span &p_datetime)
: m_datetime(p_datetime.m_datetime)
{
	m_negative_z = p_datetime.m_negative_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime_span destructor
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span::~C_g02_datetime_span()
{
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set datetime span from a number of milliseconds
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime_span::set_span(
				const m &p_millisecs_m,
				const z	p_negative_z)
{
	m	time_m;
	m	days_m;

	days_m = p_millisecs_m / G02_MILLISECS_PER_DAY_M;
	time_m = p_millisecs_m - days_m * G02_MILLISECS_PER_DAY_M;

	m_datetime.set_date(days_m);
	m_datetime.set_time(time_m);
	m_negative_z = !(!p_negative_z);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set datetime span from a number of milliseconds
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime_span::set_span(const l p_time_span_l)
{
	z	sign_z;

	sign_z = (p_time_span_l < 0);

	set_span(m(::abs(p_time_span_l)), sign_z);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set datetime span
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime_span::set_span(
				const m p_days_m,
				const w p_hours_w,
				const w p_mins_w,
				const w p_secs_w,
				const w p_millisecs_w,
				const z	p_negative_z)
{
	m_datetime.set_date(p_days_m);
	m_datetime.set_time(
				p_hours_w,
				p_mins_w,
				p_secs_w,
				p_millisecs_w);

	m_negative_z = !(!p_negative_z);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return non negative value
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::abs() const
{
	C_g02_datetime_span	ret_val;

	ret_val = *this;
	ret_val.m_negative_z = FALSE;

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get duration into an 8 byte (64 bit) array
//						: in the format used to hold durations in the database
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g02_datetime_span::span(b *p_span_p)
{
	m_datetime.datetime(p_span_p);

	if (m_negative_z)
	{
//
// set most significant bit if duration is negative
//
		p_span_p[0] |= 0x80;
	}
	else
	{
//
// Clear most significant bit if duration is positive
//
		p_span_p[0] &= 0x7F;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set duration from an 8 byte (64 bit) array
//						: in the format used to hold durations in the database
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g02_datetime_span::set_span(const b *p_span_p)
{
	b	work_ba[8];
//
// Make a copy of the duration in the working array
//
	memcpy((void *) work_ba, p_span_p, 8);
//
// Set negative flag according to the state of the most significant bit
//
	if ((work_ba[0] & 0x80) != 0)
	{
		m_negative_z = TRUE;
//
// Clear the most significant bit prior to setting the datetime member
//
		work_ba[0] &= 0x7F;
	}
	else
	{
		m_negative_z = FALSE;
	}

	m_datetime.set_datetime(work_ba);
}


///////////////////////////////////////////////////////////////////////////////
// Function Description : Add a datetime span
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator+(
				const C_g02_datetime_span &p_time_span) const
{
	C_g02_datetime_span	ret_val;

	if (m_negative_z == p_time_span.m_negative_z)
	{
//
// If the durations have the same sign then add the datetimes together
//
		ret_val = *this;
		ret_val.m_datetime += p_time_span.abs();
	}
//
// The signs are different.
//
	else if (m_datetime < p_time_span.m_datetime)
	{
		ret_val = p_time_span;
		ret_val.m_datetime -= this->abs();
	}
	else if (m_datetime == p_time_span.m_datetime)
	{
		ret_val.set_span(0, 0, FALSE);
	}
	else
	{
		ret_val = *this;
		ret_val.m_datetime -= p_time_span.abs();

	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract a datetime span
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator-(
				const C_g02_datetime_span &p_time_span) const
{
	return *this + (-p_time_span);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add a time span in milliseconds
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator+(l p_time_span_l) const
{
	return *this + C_g02_datetime_span(p_time_span_l);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract a time span in milliseconds
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator-(l p_time_span_l) const
{
	return *this - C_g02_datetime_span(p_time_span_l);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Change sign of datetime span
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator-() const
{
	C_g02_datetime_span	ret_val;

	ret_val = *this;
	ret_val.m_negative_z = !ret_val.m_negative_z;

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Multiply a time span
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime_span::operator*(const w p_scale_w) const
{
	C_g02_datetime_span	ret_val;
	__int64				work64;
	m					time_m;
	m					carried_days_m;
	m					days_m;
	const m				MAX_INT_M = ~m(0);
//
// Calculate time of day * scale
//
	work64 = __int64(m_datetime.time()) * p_scale_w;
//
// Calculate time of day component of result and number of carried days
//
	carried_days_m = m(work64 / G02_MILLISECS_PER_DAY_M);
	time_m = m(work64 - carried_days_m * G02_MILLISECS_PER_DAY_M);
//
// Calculate days * scale + carry
//
	work64 = __int64(m_datetime.date()) * p_scale_w + carried_days_m;

	if (work64 > MAX_INT_M)
	{
		// throw exception
		days_m = MAX_INT_M;
	}
	else
	{
		days_m = m(work64);
	}

	ret_val.set_span(days_m, time_m, m_negative_z);

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Assignment
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator=(
				const C_g02_datetime_span &p_time_span)
{
	m_datetime   = p_time_span.m_datetime;
	m_negative_z = p_time_span.m_negative_z;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add and assign
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator+=(
				const C_g02_datetime_span &p_time_span)
{
	*this = *this + p_time_span;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract and assign
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator-=(
				const C_g02_datetime_span &p_time_span)
{
	*this = *this - p_time_span;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add and assign
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator+=(l p_time_span_l)
{
	*this = *this + p_time_span_l;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract and assign
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator-=(l p_time_span_l)
{
	*this = *this - p_time_span_l;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Multiply and assign
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span &C_g02_datetime_span::operator*=(const w p_scale_w)
{
	*this = *this * p_scale_w;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Equality
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator==(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	ret_val_z = FALSE;

	if (m_datetime == p_time_span.m_datetime &&
		m_negative_z == p_time_span.m_negative_z)
	{
		ret_val_z = TRUE;
	}

	return ret_val_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Inequality
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator!=(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	ret_val_z = FALSE;

	if (m_datetime != p_time_span.m_datetime ||
		m_negative_z != p_time_span.m_negative_z)
	{
		ret_val_z = TRUE;
	}

	return ret_val_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Less than
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator<(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	if (m_negative_z == p_time_span.m_negative_z)
	{
		ret_val_z = (m_datetime < p_time_span.m_datetime) ^ m_negative_z;
	}
	else
	{
		ret_val_z = m_negative_z;
	}

	return ret_val_z;
};


///////////////////////////////////////////////////////////////////////////////
// Function Description : Greater than
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator>(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	if (m_negative_z == p_time_span.m_negative_z)
	{
		ret_val_z = (m_datetime > p_time_span.m_datetime) ^ m_negative_z;
	}
	else
	{
		ret_val_z = p_time_span.m_negative_z;
	}

	return ret_val_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Less than or equal to
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator<=(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	if (m_negative_z == p_time_span.m_negative_z)
	{
		ret_val_z = (m_datetime < p_time_span.m_datetime) ^ m_negative_z;
		ret_val_z |= (m_datetime == p_time_span.m_datetime);
	}
	else
	{
		ret_val_z = m_negative_z;
	}

	return ret_val_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Greater than or equal to
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime_span::operator>=(
				const C_g02_datetime_span &p_time_span) const
{
	BOOL ret_val_z;

	if (m_negative_z == p_time_span.m_negative_z)
	{
		ret_val_z = (m_datetime > p_time_span.m_datetime) ^ m_negative_z;
		ret_val_z |= (m_datetime == p_time_span.m_datetime);
	}
	else
	{
		ret_val_z = p_time_span.m_negative_z;
	}

	return ret_val_z;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return date and time span string in specified format
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 28/05/96
///////////////////////////////////////////////////////////////////////////////

CString C_g02_datetime_span::Format(const CString &p_format)
{
	CString ret_val;
	tc		buffer_tc[17];
	int		idx;
	int		format_len;
	CString Param; 
	CString Modifier;
	z		fixed_width_z;

	idx = 0;
	format_len = p_format.GetLength();

	while(idx < format_len)
	{
		if (p_format[idx++]==_TEXT('%'))
		{
			switch(p_format[idx])
			{
			case '#':
				fixed_width_z = FALSE;
				Modifier=_TEXT("");
				idx++;
				break;
			default:
				fixed_width_z = TRUE;
				break;
			}	

			switch(p_format[idx++])
			{
			default:
				//throw SE_BroadcastTimeInvalidFormat(FATAL, gmrGeneralDetails()); //Bad format character 

				break;
			case '%':
				ret_val+=_TEXT('%');
				break;

			case _TEXT('H'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,hours());
				ret_val+=buffer_tc;
				break;

			case _TEXT('M'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,minutes());
				ret_val+=buffer_tc;
				break;

			case _TEXT('S'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,seconds());
				ret_val+=buffer_tc;	
				break;

			case _TEXT('C'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,millisecs() / 10);
				ret_val+=buffer_tc;
				break;

			case _TEXT('F'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("03");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,millisecs());
				ret_val+=buffer_tc;
				break;

			case _TEXT('d'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("07");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,days());
				ret_val+=buffer_tc;
				break;
			
			case _TEXT('l'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("07");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,days());
				ret_val+=buffer_tc;
				break;

			case _TEXT('f'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("07");
				}
				Param = _TEXT("%") + Modifier + _TEXT("u");
				_stprintf(buffer_tc,Param,time());
				ret_val+=buffer_tc;
				break;

			case _TEXT('+'):
				if (m_negative_z)
				{
					ret_val+=_TEXT('-');
				}
				else
				{
					ret_val+=_TEXT('+');
				}
				break;

			case _TEXT('-'):
				if (m_negative_z)
				{
					ret_val+=_TEXT('-');
				}
				break;
			}
		}
		else
		{
		ret_val+=p_format[idx-1];
		}
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time span from a string in specified format
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 28/05/96
///////////////////////////////////////////////////////////////////////////////

z C_g02_datetime_span::Format(const CString &p_format, const CString &p_time)
{

	int pos;						// position in format string

	CString ScanForm;				// holds section of format input
	CString Scanned;   				// holds a string formatted by  sscanf output
	CString ScanModifier;

	CString Time=p_time;			// holds a copy of input parameters 
	CString Format=p_format;		// inputs are const

	tc		fch_tc;					//Holds current formatting character
	m		extract_m;
	l		extract_l;
	tc		extract_tc;
	z		fixed_width_z;
	z		optional_z;
	z		error_z;
	int		extracts = 0;

	l		days_l;
	w		hour_w;
	w		minute_w;
	w		second_w;
	w		millisec_w;
	z		negative_z;

	error_z = FALSE;
//
// Ensure view is consistent in case not all components are defined
//
	days_l = m_datetime.date();
	negative_z = m_negative_z;
	hour_w			= m_datetime.hour();
	minute_w		= m_datetime.minute();
	second_w		= m_datetime.second();
	millisec_w		= m_datetime.millisec();

	pos = g02_find_field(Format, Time);

	while (pos == 0 &&
		   error_z == FALSE)
	{
		if ((fch_tc=Format[++pos]) == _TEXT('#'))	 	//current format character = 1 after %
		{
			fch_tc=Format[++pos];				//advance to next character
			fixed_width_z = FALSE;
			ScanModifier = _TEXT("");
		}
		else
		{
			fixed_width_z = TRUE;
		}
//
// Check for optional field specification
//
		if (fch_tc == _TEXT('o'))
		{
			fch_tc=Format[++pos];				//advance to next character
			optional_z = TRUE;
		}
		else
		{
			optional_z = FALSE;
		}

		switch (fch_tc)
		{

		 	case _TEXT('H'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				hour_w = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					hour_w = 0;
				}
				break;
			
			case _TEXT('M'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				minute_w = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					minute_w = 0;
				}
				break;
			
			case _TEXT('S'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				second_w = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					second_w = 0;
				}
				break;
			
			case _TEXT('C'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				millisec_w = static_cast<w>(extract_m * 10);
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					millisec_w = 0;
				}
				break;

			case _TEXT('F'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("03");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				millisec_w = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					millisec_w = 0;
				}
				break;

			case _TEXT('l'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("07");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("d");
				extracts = _stscanf(Time,ScanForm,&extract_l);
				days_l = (l) extract_l;
				Scanned.Format(ScanForm,extract_l);
				break;

			case _TEXT('n'):
				ScanForm += _TEXT("%1c");
				extract_tc = _TEXT('\0');
				optional_z = TRUE;	// %n is always optional
				extracts = _stscanf(Time,ScanForm,&extract_tc);
				if (extract_tc == _TEXT('-'))
				{
					negative_z = TRUE;
					Scanned = _TEXT("-");
				}
				else
				{
					negative_z = FALSE;
					if (extract_tc == _TEXT('+'))
					{
						Scanned = _TEXT("+");
					}
					else
					{
						Scanned = _TEXT("");
					}
				}
				break;

			default:
				error_z = TRUE;
				break;

		} //end switch

		if (extracts == 1 || optional_z)
		{
//
// lop off left of format string not including format char
//
			Format=Format.Mid(pos + 1);
//
// If data extracted
//
			if (extracts == 1)
			{
//
// find position of scanned data
//
				if ((pos=Time.Find(Scanned)) >=0)
				{
//
// lop off characters upto end of scanned data
//				
					Time=Time.Mid(pos + Scanned.GetLength());
				}
				else
				{
//
// scanned data not found in input string
//
					error_z = TRUE;
				}
			}
		} // extracts == 1 or optional field
		else
		{
			error_z = TRUE;
		}

		pos = g02_find_field(Format, Time);
	}

	if (pos != NO_FIELD)
	{
		error_z = TRUE;
	}

	if (!error_z)
	{
		m_negative_z = negative_z;
		m_datetime.set_date((m) days_l);
		m_datetime.set_time(
					hour_w,
					minute_w,
					second_w,
					millisec_w);
	}
	else
	{
/*
		CString temp;

		temp.Format(_TEXT("Format = %s, Time = %s"), p_format, p_time);

		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G02_CODE_BASE_W + DATETIME_SPAN_CODE_BASE_W + 1,
				CString(_TEXT("C_g02_datetime_span::Format: scan failed")),
				temp,
				G01_SE_RAISED );
*/
	}

	return !error_z;
}

void C_g02_datetime_span::Dump()
{
	CString format=_TEXT("Days %+%d, Time %H:%M:%S.%F\n");
	_tprintf(_TEXT("%s"), Format(format));
};

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////

