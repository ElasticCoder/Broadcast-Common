///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g02_datetime_span.h       
// File Type   : C++ Header
// Description : Date and Time Span (duration) Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_DATETIME_SPAN_H
#define _G02_DATETIME_SPAN_H

#include <portable_types.h>  // Type definitions
#include <G00.h>           // Services DLL import/export control
#include <G02.h>           // Time/Timer event definitions
#include <g02_datetime.h>  // ...

///////////////////////////////////////////////////////////////////////////////////
// Class          : C_g02_datetime_span
///////////////////////////////////////////////////////////////////////////////////

class SERVICES_EXTERNAL C_g02_datetime_span
{
public:					
	C_g02_datetime_span();
	C_g02_datetime_span(
					const m p_days_m,
					const w p_hours_w,
					const w p_mins_w,
					const w p_secs_w,
					const w p_millisecs_w,
					const z	p_negative_z = FALSE);
	C_g02_datetime_span(const l p_time_span_l);
	C_g02_datetime_span(
					const m p_days_m,
					const m p_millisecs_m,
					const z	p_negative_z);
	C_g02_datetime_span(
				const C_g02_datetime_span &p_datetime);
	~C_g02_datetime_span();

protected:
	C_g02_datetime	m_datetime;
	z				m_negative_z;

public:
	inline void	set_time(const m p_millisecs_m)
					{ m_datetime.set_time(p_millisecs_m); };
	inline void	set_date(const m p_days_m)
					{ m_datetime.set_date(p_days_m); };
	void	set_span(
					const m &p_millisecs_m,
					const z	p_negative_z);
	void	set_span(const l p_time_span_l);
	inline void	set_sign(const z p_negative_z)
					{ m_negative_z = !(!p_negative_z); };
	void	set_span(
					const m p_days_m,
					const w p_hours_w,
					const w p_mins_w,
					const w p_secs_w,
					const w p_millisecs_w,
					const z	p_negative_z = FALSE);
	inline void	set_span(
					const m p_days_m,
					const m p_millisecs_m,
					const z	p_negative_z)
					{ m_datetime.set_date(p_days_m);
					  m_datetime.set_time(p_millisecs_m);
					  m_negative_z = p_negative_z; };

	inline m days() const { return m_datetime.date(); };
	inline w hours() { return m_datetime.hour(); };
	inline w minutes() { return m_datetime.minute(); };
	inline w seconds()  { return m_datetime.second(); };
	inline w millisecs() { return m_datetime.millisec(); };
	inline z negative() const { return m_negative_z; };
	inline m time() const { return m_datetime.time(); };

	C_g02_datetime_span abs() const;
//
// Get duration into an 8 byte (64 bit) array
// in the format used to hold durations in the database
//
	void	span(b *p_span_p);
//
// Set duration from an 8 byte (64 bit) array
// in the format used to hold durations in the database
//
	void	set_span(const b *p_span_p);

	C_g02_datetime_span operator+(const C_g02_datetime_span &p_time_span) const;
	C_g02_datetime_span operator-(const C_g02_datetime_span &p_time_span) const;
	C_g02_datetime_span operator+(l p_time_span_l) const;
	C_g02_datetime_span operator-(l p_time_span_l) const;
	C_g02_datetime_span operator-() const;
	C_g02_datetime_span operator*(const w p_scale_w) const;

	C_g02_datetime_span &operator=(const C_g02_datetime_span &p_time_span);
	C_g02_datetime_span &operator+=(const C_g02_datetime_span &p_time_span);
	C_g02_datetime_span &operator-=(const C_g02_datetime_span &p_time_span);
	C_g02_datetime_span &operator+=(l p_time_span_l);
	C_g02_datetime_span &operator-=(l p_time_span_l);
	C_g02_datetime_span &operator*=(const w p_scale_w);
	BOOL operator==(const C_g02_datetime_span &p_time_span) const;
	BOOL operator!=(const C_g02_datetime_span &p_time_span) const;
	BOOL operator<(const C_g02_datetime_span &p_time_span) const;
	BOOL operator>(const C_g02_datetime_span &p_time_span) const;
	BOOL operator<=(const C_g02_datetime_span &p_time_span) const;
	BOOL operator>=(const C_g02_datetime_span &p_time_span) const;

//
// Generate a string from the datetime according to a specified format.
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
//	l		Days			7
//
// A special modifier can be used to change from fixed width with leading zeros
// to width determined by the data output with no leading zeros. This modifier
// is '#', e.g. %#H:%M:%#S will output time 09:05:00 as "9:05:0"
//
	CString Format(const CString &p_format);
//
// Set up datetime object from a string in a specified format.
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
//	l		Days			7				0
//
//	n		sign			1				+ (always optional)
//
// A special modifier can be used to change from fixed width with leading zeros
// to width determined by the data output with no leading zeros. This modifier
// is '#', e.g. %#H:%M:%#S will read time "9:05:0" as 09:05:00 but
// the format would fail to scan "9:5:0"
// Another modifier is 'o' meaning that the field and any formatting characters
// preceeding the field are optional. Optional fields have default values as
// indicated above.
//
// The function returns TRUE if the datetime span was set successfully according
// to the specified format, FALSE otherwise
//
	z Format(const CString &p_format, const CString &p_time);

	void Dump();
};	
 

//////////////////////////////////////////////////////////////////////////////////

#endif  // _G02_DATETIME_SPAN_H

///////////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////////
