///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_datetime.cpp
// File Type   : C++ Source
// Description : Date and Time to millisecond resolution
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>
#include <g02_datetime.h>
#include <g02_datetime_span.h>
#include "g02_systemtime.h"
#include "g02_find_field.h"


//
// Lookup table for ordering of date components according to the
// international date format in use.
//
static const w v_date_lookup_wa[3][3] =
{
	1, 0, 2,		// format = 0: mm/dd/yy
	0, 1, 2,		// format = 1: dd/mm/yy
	2, 1, 0			// format = 2: yy/mm/dd
};


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime()
{
	m_days_m = 0;
	m_time_m = 0;
	m_view_status = VIEW_OUT_OF_DATE;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 19/04/96
// Date Last Modified   : 19/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(
					const w p_year_w,
					const w p_month_w,
					const w p_day_w,
					const w p_hour_w,
					const w p_min_w,
					const w p_sec_w,
					const w p_millisec_w)
{
	set_datetime(
			p_year_w,
			p_month_w,
			p_day_w,
			p_hour_w,
			p_min_w,
			p_sec_w,
			p_millisec_w);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 19/04/96
// Date Last Modified   : 19/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(
					const w p_hour_w,
					const w p_min_w,
					const w p_sec_w,
					const w p_millisec_w)
{
	set_time(
			p_hour_w,
			p_min_w,
			p_sec_w,
			p_millisec_w);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 19/04/96
// Date Last Modified   : 19/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(
					const w p_year_w,
					const w p_month_w,
					const w p_day_w)
{
	set_date(
			p_year_w,
			p_month_w,
			p_day_w);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(
					const m p_days_m,
					const m p_millisecs_m)
{
	m_days_m = p_days_m;
	m_time_m = p_millisecs_m;

	if (p_millisecs_m >= G02_MILLISECS_PER_DAY_M)
	{
		// throw exception ?
		m_time_m = 0;
	}

	m_view_status = VIEW_OUT_OF_DATE;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(const C_g02_datetime &p_datetime)
{
	m_days_m = p_datetime.m_days_m;
	m_time_m = p_datetime.m_time_m;
	m_view_status = p_datetime.m_view_status;
	m_view	 = p_datetime.m_view;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(const CTime &p_datetime)
{
	set_datetime(p_datetime);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(const FILETIME &p_datetime)
{
	set_datetime(p_datetime);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime constructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::C_g02_datetime(const SYSTEMTIME &p_datetime)
{
	set_datetime(p_datetime);
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g02_datetime destructor
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime::~C_g02_datetime()
{
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the datetime into a CTime object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::datetime(CTime &p_time) const
{
	time_t	temp;

	if (g02_datetime_to_time_t(m_days_m, m_time_m, temp) == FALSE)
	{
		// throw exception ?
	}

	p_time = CTime(temp);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the datetime into a FILETIME object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::datetime(FILETIME &p_datetime) const
{
	g02_datetime_to_filetime(m_days_m, m_time_m, p_datetime);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the datetime into a SYSTEMTIME object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::datetime(SYSTEMTIME &p_datetime) const
{
	if (g02_datetime_to_systemtime(m_days_m, m_time_m, p_datetime) == FALSE)
	{
		// throw exception ?
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get date and time into an 8 byte (64 bit) array
//						: in the format used to hold date times in the database
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g02_datetime::datetime(b *p_time_p)
{
//
// Date and time format for database is most significant bit first.
// The intel processor stores a 32 bit word as
//		byte[0]	bits 0-7
//		byte[1] bits 8-15
//		byte[2] bits 16-23
//		byte[3] bits 24-31
//
	b	*read_p;
	b	count_b;
//
// Get bits 32-63 from m_days_m
// set read	pointer to bits 56-63 (4th byte of m_days_m)
//
	read_p = ((b *) &m_days_m) + 3;

	for (count_b = 4; count_b > 0; count_b--)
	{
		*p_time_p++ = *read_p--;
	}
//
// Get bits 0 - 31 from m_time_m
// set read	pointer to bits 24-31 (4th byte of m_time_m)
//
	read_p = ((b *) &m_time_m) + 3;

	for (count_b = 4; count_b > 0; count_b--)
	{
		*p_time_p++ = *read_p--;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the current date and time
// Author               : Duncan Olesen
// Date Created         : 17/04/96
// Date Last Modified   : 17/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime C_g02_datetime::now()
{
	SYSTEMTIME		now;
	C_g02_datetime	ret_val;

	GetLocalTime(&now);
	ret_val.set_datetime(now);

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the time component of C_g02_datetime
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_time(const m p_millisecs_m)
{
	m_time_m = p_millisecs_m;

	if (p_millisecs_m >= G02_MILLISECS_PER_DAY_M)
	{
		// throw exception ?
		m_time_m = 0;
	}

	m_view_status = VIEW_OUT_OF_DATE;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the date component of C_g02_datetime
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_date(const m p_days_m)
{
	m_days_m = p_days_m;
	m_view_status = VIEW_OUT_OF_DATE;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set object to current time
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 12/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_datetime()
{
	*this = C_g02_datetime::now();
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the date and time components of C_g02_datetime
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_datetime(
				const w p_year_w,
				const w p_month_w,
				const w p_day_w,
				const w p_hour_w,
				const w p_min_w,
				const w p_sec_w,
				const w p_millisec_w)
{
	m_view.wYear			= p_year_w;
	m_view.wMonth			= p_month_w;
	m_view.wDay				= p_day_w;
	m_view.wHour			= p_hour_w;
	m_view.wMinute			= p_min_w;
	m_view.wSecond			= p_sec_w;
	m_view.wMilliseconds	= p_millisec_w;

	(void) update();
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the time component of C_g02_datetime
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_time(
				const w p_hour_w,
				const w p_min_w,
				const w p_sec_w,
				const w p_millisec_w)
{
//
// Ensure the view is up to date
//
	if (update_view())
	{
//
// Store time in the view
//
		m_view.wHour			= p_hour_w;
		m_view.wMinute			= p_min_w;
		m_view.wSecond			= p_sec_w;
		m_view.wMilliseconds	= p_millisec_w;
//
// Update the date time from the view
//
		update();
	}
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the date component of C_g02_datetime
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_date(
					const w p_year_w,
					const w p_month_w,
					const w p_day_w)
{
//
// Ensure the view is up to date
//
	if (update_view())
	{
//
// Store time in the view
//
		m_view.wYear			= p_year_w;
		m_view.wMonth			= p_month_w;
		m_view.wDay				= p_day_w;
//
// Update the date time from the view
//
		update();
	}
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time from a CTime object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 18/06/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_datetime(const CTime &p_time)
{
	m_view_status = VIEW_OUT_OF_DATE;

	g02_time_t_to_datetime(p_time.GetTime(), m_days_m, m_time_m);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time from a FILETIME object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_datetime(const FILETIME &p_time)
{
	m_view_status = VIEW_OUT_OF_DATE;

	g02_filetime_to_datetime(p_time, m_days_m, m_time_m);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time from a SYSTEMTIME object
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

void	C_g02_datetime::set_datetime(const SYSTEMTIME &p_time)
{
	m_view_status = VIEW_OUT_OF_DATE;

	if (g02_systemtime_to_datetime(p_time, m_days_m, m_time_m) == FALSE)
	{
		// throw exception ?
		m_days_m = 0;
		m_time_m = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time from an 8 byte (64 bit) array
//						: in the format used to hold date times in the database
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g02_datetime::set_datetime(const b *p_time_p)
{
//
// Date and time format for database is most significant bit first.
// The intel processor stores a 32 bit word as
//		byte[0]	bits 0-7
//		byte[1] bits 8-15
//		byte[2] bits 16-23
//		byte[3] bits 24-31
//
	b	*write_p;
	b	count_b;

	m_view_status = VIEW_OUT_OF_DATE;
//
// Store bits 32-63 in m_days_m
// set write pointer to bits 56-63 (4th byte of m_days_m)
//
	write_p = ((b *) &m_days_m) + 3;

	for (count_b = 4; count_b > 0; count_b--)
	{
		*write_p-- = *p_time_p++;
	}
//
// Get bits 0 - 31 from m_time_m
// set write pointer to bits 24-31 (4th byte of m_time_m)
//
	write_p = ((b *) &m_time_m) + 3;

	for (count_b = 4; count_b > 0; count_b--)
	{
		*write_p-- = *p_time_p++;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the year component of the date time
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::year()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wYear;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the month component of the date time
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::month()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wMonth;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the day component of the date time
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::day()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wDay;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Get the day of week of the date time
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::day_of_week()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wDayOfWeek;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the number of hours in the date time
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::hour()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wHour;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the number of minutes past the hour
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::minute()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wMinute;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the number of seconds past the minute
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::second()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wSecond;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the number of milliseconds past the second
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

w C_g02_datetime::millisec()
{
	w	ret_val_w;
//
// Ensure the view is up to date
//
	if (update_view())
	{
		ret_val_w = m_view.wMilliseconds;
	}
	else
	{
		ret_val_w = 0;
	}

	return ret_val_w;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Assignment operator
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime &C_g02_datetime::operator=(const C_g02_datetime &p_time)
{
	m_time_m			= p_time.m_time_m;
	m_days_m			= p_time.m_days_m;
	m_view_status		= p_time.m_view_status;
	m_view				= p_time.m_view;

	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add date time span (duration) to date time
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime C_g02_datetime::operator+(const C_g02_datetime_span &p_time_span) const
{
	C_g02_datetime	ret_val;

	if (p_time_span.negative() == FALSE)
	{
		m	carry_m;
//
// Add time components and carry
//
		ret_val.m_time_m = m_time_m + p_time_span.time();

		if (ret_val.m_time_m >= G02_MILLISECS_PER_DAY_M)
		{
			ret_val.m_time_m = ret_val.m_time_m - G02_MILLISECS_PER_DAY_M;
			carry_m = 1;
		}
		else
		{
			carry_m = 0;
		}
//
// Add date components with carry
//
		ret_val.m_days_m = m_days_m + carry_m + p_time_span.days();
	}
	else
	{
		ret_val = *this - (-p_time_span);
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract date time span (duration) from date time
// Author               : Duncan Olesen
// Date Created         : 10/04/96
// Date Last Modified   : 10/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime C_g02_datetime::operator-(const C_g02_datetime_span &p_time_span) const
{
	C_g02_datetime	ret_val;

	if (p_time_span.negative() == FALSE)
	{
		m	borrow_m;
		m	span_time_m;
		m	span_date_m;

		span_time_m = p_time_span.time();
		span_date_m = p_time_span.days();
//
// Subtract time components and borrow if necessary
//
		if (span_time_m > m_time_m)
		{
			borrow_m = 1;
			ret_val.m_time_m = (G02_MILLISECS_PER_DAY_M + m_time_m) -
							   span_time_m;
		}
		else
		{
			borrow_m = 0;
			ret_val.m_time_m = m_time_m - span_time_m;
		}
//
// Subtract date components with borrow
//
		if (m_days_m < span_date_m + borrow_m)
		{
//
// Result is before zero date time so set to zero date time
//
			ret_val.m_time_m = 0;
			ret_val.m_days_m = 0;
		}
		else
		{
			ret_val.m_days_m = m_days_m - (span_date_m + borrow_m);
		}
	}
	else
	{
		ret_val = *this + (-p_time_span);
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract date time to result in a date time span
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime_span C_g02_datetime::operator-(const C_g02_datetime &p_time) const
{
	C_g02_datetime_span	ret_val;
	C_g02_datetime_span	work_span;
	C_g02_datetime		work;

	if (*this >= p_time)
	{
		work_span.set_span(p_time.date(), p_time.time(), FALSE);
		work = *this - work_span;
		ret_val.set_span(work.date(), work.time(), FALSE);
	}
	else
	{
		work_span.set_span(date(), time(), FALSE);
		work = p_time - work_span;
		ret_val.set_span(work.date(), work.time(), TRUE);
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Add time span in milliseconds
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime C_g02_datetime::operator+(l p_time_span_l) const
{
	C_g02_datetime		ret_val;
	C_g02_datetime_span	span;

	ret_val = *this;

	span.set_span( m(abs(p_time_span_l)), FALSE );

	if (p_time_span_l < 0)
	{
		ret_val -= span;
	}
	else
	{
		ret_val += span;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Subtract time span in milliseconds
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime C_g02_datetime::operator-(l p_time_span_l) const
{
	return (*this + (-p_time_span_l));
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime &C_g02_datetime::operator+=(const C_g02_datetime_span &p_time_span)
{
	*this = *this + p_time_span;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime &C_g02_datetime::operator-=(const C_g02_datetime_span &p_time_span)
{
	*this = *this - p_time_span;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime &C_g02_datetime::operator+=(l p_time_span_l)
{
	*this = *this + p_time_span_l;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

C_g02_datetime &C_g02_datetime::operator-=(l p_time_span_l)
{
	*this = *this - p_time_span_l;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime::operator==(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_time_m == p_time.m_time_m &&
		m_days_m == p_time.m_days_m)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

BOOL C_g02_datetime::operator!=(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_time_m != p_time.m_time_m ||
		m_days_m != p_time.m_days_m)
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime::operator<(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_days_m < p_time.m_days_m ||
		(m_days_m == p_time.m_days_m &&
		 m_time_m < p_time.m_time_m))
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime::operator>(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_days_m > p_time.m_days_m ||
		(m_days_m == p_time.m_days_m &&
		 m_time_m > p_time.m_time_m))
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime::operator<=(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_days_m < p_time.m_days_m ||
		(m_days_m == p_time.m_days_m &&
		 m_time_m <= p_time.m_time_m))
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : 
// Author               : Duncan Olesen
// Date Created         : 11/04/96
// Date Last Modified   : 11/04/96
///////////////////////////////////////////////////////////////////////////////

BOOL C_g02_datetime::operator>=(const C_g02_datetime &p_time) const
{
	BOOL	ret_val;

	if (m_days_m > p_time.m_days_m ||
		(m_days_m == p_time.m_days_m &&
		 m_time_m >= p_time.m_time_m))
	{
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}

	return ret_val;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return date and time string in specified format
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

CString C_g02_datetime::Format(
							const CString	&p_format)
{
	CString ret_val;
	tc		buffer_tc[17];
	int		idx;
	int		format_len;
	CString Param; 
	CString	Modifier;
	z		fixed_width_z;
	z		error_z;
	m		date_part_ma[3];
	w		date_format_w;
	CString	separator;
	CString	YearForm;
	CString	MonthForm;
	CString	DayForm;

	error_z = FALSE;
	idx = 0;
	format_len = p_format.GetLength();

	if (!update_view())
	{
//
// Could not update view so set loop counter so that no processing is done
//
		idx = format_len; 
	}

	while(idx < format_len)
	{
		if (p_format[idx++]==_TEXT('%'))
		{
			switch(p_format[idx])
			{
			case _TEXT('#'):
				fixed_width_z = FALSE;
				Modifier = _TEXT("");
				idx++;
				break;
			default:
				fixed_width_z = TRUE;
				break;
			}	

			switch(p_format[idx++])
			{
			case _TEXT('%'):
				ret_val+=_TEXT('%');
				break;

			case _TEXT('H'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wHour);
				ret_val+=buffer_tc;
				break;

			case _TEXT('M'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wMinute);
				ret_val+=buffer_tc;
				break;

			case _TEXT('S'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wSecond);
				ret_val+=buffer_tc;	
				break;

			case _TEXT('C'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wMilliseconds / 10);
				ret_val+=buffer_tc;
				break;

			case _TEXT('F'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("03");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wMilliseconds);
				ret_val+=buffer_tc;
				break;

			case _TEXT('x'):
			case _TEXT('X'):
				g02_date_format(date_format_w,
								separator);

				if (p_format[idx - 1] == _TEXT('X'))
				{
					YearForm  = _TEXT("%04u"); // 4 digit year
					date_part_ma[v_date_lookup_wa[date_format_w][2]] =
							m_view.wYear;
				}
				else
				{
					YearForm  = _TEXT("%02u"); // 2 digit year
					date_part_ma[v_date_lookup_wa[date_format_w][2]] =
							m_view.wYear % 100;
				}
				MonthForm = _TEXT("%02u");
				DayForm   = _TEXT("%02u");

				switch(date_format_w)
				{
				case 0:
					Param = MonthForm + separator + DayForm + separator +
							   YearForm;
					break;
				case 2:
					Param = YearForm + separator + MonthForm + separator +
							   DayForm;
					break;
				default:
					Param = DayForm + separator + MonthForm + separator +
							   YearForm;
					date_format_w = 1;
					break;
				}
				
				date_part_ma[v_date_lookup_wa[date_format_w][0]] = m_view.wDay;
				date_part_ma[v_date_lookup_wa[date_format_w][1]] = m_view.wMonth;

				_stprintf(
						buffer_tc,
						Param,
						date_part_ma[0],
						date_part_ma[1],
						date_part_ma[2]);

				ret_val+=buffer_tc;
				break;
			
			case _TEXT('d'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wDay);
				ret_val+=buffer_tc;
				break;
			
			case _TEXT('m'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wMonth);
				ret_val+=buffer_tc;
				break;
							
			case _TEXT('Y'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("04");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wYear);
				ret_val+=buffer_tc;	
				break;
			
			case _TEXT('w'):
				Param = _TEXT("%d");
				_stprintf(buffer_tc,Param,m_view.wDayOfWeek);
				ret_val+=buffer_tc;
				break;
			
			case _TEXT('y'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("02");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,m_view.wYear%100);
				ret_val+=buffer_tc;
				break;

			case _TEXT('l'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("07");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,date());
				ret_val+=buffer_tc;
				break;

			case _TEXT('f'):
				if (fixed_width_z)
				{
					Modifier = _TEXT("07");
				}
				Param = _TEXT("%") + Modifier + _TEXT("d");
				_stprintf(buffer_tc,Param,time());
				ret_val+=buffer_tc;
				break;

			default:
				error_z = TRUE;
				//throw SE_BroadcastTimeInvalidFormat(FATAL, gmrGeneralDetails()); //Bad format character 
				break;
			}
		}
		else
		{
		ret_val+=p_format[idx-1];
		}
	}

	if (error_z)
	{
		CString temp;

		temp.Format(_TEXT("Format = %s"), p_format);

		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G02_CODE_BASE_W + DATETIME_CODE_BASE_W + 1,
				CString(_TEXT("C_g02_datetime::Format: format failed")),
				temp,
				G01_SE_RAISED );
	}

	return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Set date and time from a string in specified format
// Author               : Duncan Olesen
// Date Created         : 12/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////

z C_g02_datetime::Format(
					const CString	&p_format,
					const CString	&p_time)
{

	int pos;						// position in format string

	CString ScanForm;				// holds section of format input
	CString Scanned;   				// holds a string formatted by  sscanf output
	CString ScanModifier;

	CString Time=p_time;			// holds a copy of input parameters 
	CString Format=p_format;		// inputs are const

	tc		fch_tc;					//Holds current formatting character
	m		extract_m;
	m		extract_ma[3];
	z		fixed_width_z;
	z		optional_z;
	z		error_z;
	int		extracts = 0;
	SYSTEMTIME		now;
	w		date_format_w;
	CString	separator;
	CString	YearForm;
	CString	MonthForm;
	CString	DayForm;
//
// Get current time for default values of optional date fields
//
	GetLocalTime(&now);
//
// Ensure view is consistent in case not all components are defined
//
	error_z = !update_view();

	m_view_status = VIEW_OUT_OF_DATE;

	pos = g02_find_field(Format, Time);

	while (pos == 0 &&
		   error_z == FALSE)
	{
//
// Check for fixed width field specification
//
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
				m_view.wHour = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wHour = 0;
				}
				break;
			
			case _TEXT('M'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wMinute = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wMinute = 0;
				}
				break;
			
			case _TEXT('S'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wSecond = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wSecond = 0;
				}
				break;
			
			case _TEXT('C'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wMilliseconds = static_cast<w>(extract_m * 10);
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wMilliseconds = 0;
				}
				break;

			case _TEXT('F'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("03");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wMilliseconds = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wMilliseconds = 0;
				}
				break;

			case _TEXT('X'):
			case _TEXT('x'):
				g02_date_format(date_format_w,
								separator);

				if (fixed_width_z)
				{
					if (fch_tc == _TEXT('X'))
					{
						YearForm  = _TEXT("%04u"); // 4 digit year
					}
					else
					{
						YearForm  = _TEXT("%02u"); // 2 digit year
					}
					MonthForm = _TEXT("%02u");
					DayForm   = _TEXT("%02u");
				}
				else
				{
					YearForm  = _TEXT("%u");
					MonthForm = _TEXT("%u");
					DayForm   = _TEXT("%u");
				}

				switch(date_format_w)
				{
				case 0:
					ScanForm = MonthForm + separator + DayForm + separator +
							   YearForm;
					break;
				case 2:
					ScanForm = YearForm + separator + MonthForm + separator +
							   DayForm;
					break;
				default:
					ScanForm = DayForm + separator + MonthForm + separator +
							   YearForm;
					date_format_w = 1;
					break;
				}

				extracts = _stscanf(Time,
									ScanForm,
									&extract_ma[0],
									&extract_ma[1],
									&extract_ma[2]);

				m_view.wDay	  = (w) extract_ma[v_date_lookup_wa[date_format_w][0]];
				m_view.wMonth = (w) extract_ma[v_date_lookup_wa[date_format_w][1]];
				m_view.wYear  = (w) extract_ma[v_date_lookup_wa[date_format_w][2]];

				if (fch_tc != _TEXT('X'))
				{
//
// Only 2 digits in year so determine the century
//
					WORD	wCurrentCentury;

					wCurrentCentury = static_cast<WORD>((now.wYear / 100) * 100);
//
// Initialise year to be in current century
//
					// Pretty stupid Compiler warning means that the following needs to change for a clean build.
					//m_view.wYear += wCurrentCentury;
					m_view.wYear = static_cast<w>(m_view.wYear + wCurrentCentury);
//
// Test if year should be in previous century
//
					if (m_view.wYear > now.wYear + 50)
					{
						m_view.wYear -= 100;
					}
//
// Test if year should be in next century
//
					if (m_view.wYear < now.wYear - 50)
					{
						m_view.wYear += 100;
					}
				} // Calculate century

				Scanned.Format(ScanForm,
							   extract_ma[0],
							   extract_ma[1],
							   extract_ma[2]);
//
// Set flag to indicate if all values extracted successfully
//
				if (extracts == 3)
				{
					extracts = 1;
				}
				else
				{
					extracts = 0;
				}

				if (optional_z && extracts != 1)
				{
					m_view.wDay		= now.wDay;
					m_view.wMonth	= now.wMonth;
					m_view.wYear	= now.wYear;
				}
				break;

			case _TEXT('d'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wDay = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wDay		= now.wDay;
				}
				break;
			
			case _TEXT('m'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wMonth = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wMonth	= now.wMonth;
				}
				break;
			
			case _TEXT('y'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("02");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wYear	= now.wYear;
				}
				else
				{
//
// Only 2 digits in year so determine the century
//
					WORD	wCurrentCentury;

					wCurrentCentury = static_cast<WORD>((now.wYear / 100) * 100);
//
// Initialise year to be in current century
//
					m_view.wYear = static_cast<w>(wCurrentCentury + extract_m);
//
// Test if year should be in previous century
//
					if (m_view.wYear > now.wYear + 50)
					{
						m_view.wYear -= 100;
					}
//
// Test if year should be in next century
//
					if (m_view.wYear < now.wYear - 50)
					{
						m_view.wYear += 100;
					}
				}
				Scanned.Format(ScanForm,extract_m);
				break;
			
			case _TEXT('Y'):
				if (fixed_width_z)
				{
					ScanModifier = _TEXT("04");
				}
				ScanForm = _TEXT("%") + ScanModifier + _TEXT("u");
				extracts = _stscanf(Time,ScanForm,&extract_m);
				m_view.wYear = (w) extract_m;
				Scanned.Format(ScanForm,extract_m);
				if (optional_z && extracts != 1)
				{
					m_view.wYear	= now.wYear;
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
		error_z = !update();
	}

	return !error_z;
}

void C_g02_datetime::Dump()
{
	CString format=_TEXT("Date %d/%m/%Y Time %H:%M:%S.%F\n");
//	cout << Format(format);
	_tprintf(_TEXT("%s"), Format(format));
}

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : copies view into date and time
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////////	

z C_g02_datetime::update()
{
	z	ret_val_z;

	if (g02_systemtime_to_datetime(
				m_view,
				m_days_m,
				m_time_m) == FALSE)
	{
		m_view_status = VIEW_OUT_OF_DATE;
		ret_val_z = FALSE;
	}
	else
	{
		m_view_status = LOCAL_VIEW;

		ret_val_z = TRUE;
	}

	return ret_val_z;
}

///////////////////////////////////////////////////////////////////////////////////	
// Function Description : copies date and time into view	
// Author               : Duncan Olesen
// Date Created			: 11/04/96
// Date Last Modified   : 15/07/96
///////////////////////////////////////////////////////////////////////////////////	

z C_g02_datetime::update_view()
{
	T_view_status	desired_status;
	z				ret_val_z;

	ret_val_z = TRUE;

	desired_status = LOCAL_VIEW;

	if (m_view_status != desired_status)
	{
		if (g02_datetime_to_systemtime(
				m_days_m,
				m_time_m,
				m_view) == TRUE)
		{
			m_view_status = desired_status;
		}
		else
		{
			ret_val_z = FALSE;
		}
	}

	return ret_val_z;
} // update_view


///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////
