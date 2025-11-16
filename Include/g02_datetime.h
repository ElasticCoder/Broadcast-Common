///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g02_datetime.h       
// File Type   : C++ Header
// Description : Date and Time Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_DATETIME_H
#define _G02_DATETIME_H

#include <portable_types.h>  // Type definitions
#include <G00.h>    // Services DLL import/export control
#include <G02.h>    // Time/Timer event definitions
#include <time.h>   // Time definitions

///////////////////////////////////////////////////////////////////////////////////
// Class          : C_g02_datetime
///////////////////////////////////////////////////////////////////////////////////
class C_g02_datetime_span; 

class SERVICES_EXTERNAL C_g02_datetime 
{
public:					
    C_g02_datetime();
    C_g02_datetime(const w p_year_w,
                   const w p_month_w,
                   const w p_day_w,
                   const w p_hour_w,
                   const w p_min_w,
                   const w p_sec_w,
                   const w p_millisec_w);
    C_g02_datetime(const w p_hour_w,
                   const w p_min_w,
                   const w p_sec_w,
                   const w p_millisec_w);
    C_g02_datetime(const w p_year_w,
                   const w p_month_w,
                   const w p_day_w);
    C_g02_datetime(const m p_days_m,
                   const m p_millisecs_m);
    C_g02_datetime(const C_g02_datetime &p_datetime);
    C_g02_datetime(const CTime &p_datetime);
    C_g02_datetime(const FILETIME &p_datetime);
    C_g02_datetime(const SYSTEMTIME &p_datetime);
    ~C_g02_datetime();

private:
    enum T_view_status
    {
        VIEW_OUT_OF_DATE,
        LOCAL_VIEW
    };

    m				m_days_m;
    m				m_time_m;
    T_view_status	m_view_status;
    SYSTEMTIME		m_view;

public:
    void datetime(CTime &p_datetime) const;
    void datetime(FILETIME &p_datetime) const;
    void datetime(SYSTEMTIME &p_datetime) const;

// Get date and time into an 8 byte (64 bit) array
// in the format used to hold date times in the database
    void datetime(b *p_time_p);

	static C_g02_datetime now();  // Return current datetime

    void set_time(const m p_millisecs_m);
    void set_date(const m p_days_m);
    void set_datetime();  // Set datetime = now
    void set_datetime(const w p_year_w,
                      const w p_month_w,
                      const w p_day_w,
                      const w p_hour_w,
                      const w p_min_w,
                      const w p_sec_w,
                      const w p_millisec_w);
    void set_time(const w p_hour_w,
                  const w p_min_w,
                  const w p_sec_w,
                  const w p_millisec_w);
    void set_date(const w p_year_w,
                  const w p_month_w,
                  const w p_day_w);
    void set_datetime(const CTime &p_time);
    void set_datetime(const FILETIME &p_time);
    void set_datetime(const SYSTEMTIME &p_time);

// Setup date and time from an 8 byte (64 bit) array
// in the format used to hold date times in the database
    void set_datetime(const b *p_time_p);

    w year();
    w month();
    w day();
    w day_of_week();
    w hour();
    w minute();
    w second();
    w millisec();
    inline m time() const
    {
        return m_time_m;
    };
    inline m date() const
    {
        return m_days_m;
    };

    C_g02_datetime operator+(const C_g02_datetime_span &p_time_span) const;
    C_g02_datetime operator-(const C_g02_datetime_span &p_time_span) const;
    C_g02_datetime_span operator-(const C_g02_datetime &p_time) const;
    C_g02_datetime operator+(l p_time_span_l) const;
    C_g02_datetime operator-(l p_time_span_l) const;

    C_g02_datetime &operator=(const C_g02_datetime &p_time);
    C_g02_datetime &operator+=(const C_g02_datetime_span &p_time_span);
    C_g02_datetime &operator-=(const C_g02_datetime_span &p_time_span);
    C_g02_datetime &operator+=(l p_time_span_l);
    C_g02_datetime &operator-=(l p_time_span_l);
    BOOL operator==(const C_g02_datetime &p_time) const;
    BOOL operator!=(const C_g02_datetime &p_time) const;
    BOOL operator<(const C_g02_datetime &p_time) const;
    BOOL operator>(const C_g02_datetime &p_time) const;
    BOOL operator<=(const C_g02_datetime &p_time) const;
    BOOL operator>=(const C_g02_datetime &p_time) const;

// Generate a string from the datetime according to a specified format.
// The string will be based upon local time.
// The format string is similar to that used for sprintf
// The format characters are as follows:
//	char	component		width
//
//	H		Hours			2
//	M		Minutes			2
//	S		Seconds			2
//	C		Centisecs		2
//	F		Millisecs		3
//
//	x		Date (dd/mm/yy or mm/dd/yy)
//	X		Date (dd/mm/yyyy or mm/dd/yyyy)
//	d		Day				2
//	m		Month			2
//	y		Year			2
//	Y		Year			4
//	w		Day of week		1
//
//	l		Days since 1601	7
//	f		Millisecs since
//			start of day	7
//
// A special modifier can be used to change from fixed width with leading zeros
// to width determined by the data output with no leading zeros. This modifier
// is '#', e.g. %#H:%M:%#S will output time 09:05:00 as "9:05:0"
//
    CString Format(const CString &p_format);

// Set up datetime object from a string in a specified format.
// The data string is read as local time.
// The format string is similar to that used for sscanf
// The format characters are as follows:
//	char	component		width			Default value
//
//	H		Hours			2				0
//	M		Minutes			2				0
//	S		Seconds			2				0
//	C		Centisecs		2				0
//	F		Millisecs		3				0
//
//	x		Date (dd/mm/yy or mm/dd/yy)		today
//	X		Date (dd/mm/yyyy or mm/dd/yyyy)	today
//	d		Day				2				Day component of today
//	m		Month			2				Month component of today
//	y		Year			2				Year component of today
//	Y		Year			4				Year component of today
//
// A special modifier can be used to change from fixed width with leading zeros
// to width determined by the data output with no leading zeros. This modifier
// is '#', e.g. %#H:%M:%#S will read time "9:05:0" as 09:05:00 but
// the format would fail to scan "9:5:0"
// Another modifier is 'o' meaning that the field and any formatting characters
// preceeding the field are optional. Optional fields have default values as
// indicated above.
//
// The function returns TRUE if the datetime was set successfully according to
// the specified format, FALSE otherwise
//
    z Format(const CString &p_format,
             const CString &p_time);

    void Dump();

private:
    z	update();
    z	update_view();
};	
 
#endif  // _G02_DATETIME_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////
