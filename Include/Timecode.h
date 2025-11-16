///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1997; Sony BPE
// All rights Reserved
/////////////////////////////////////////////////////////////////////////////
// Timecode.h: interface for the CTimecode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMECODE_H__08DB6071_F86A_11D0_AC5B_00805F81DE56__INCLUDED_)
#define AFX_TIMECODE_H__08DB6071_F86A_11D0_AC5B_00805F81DE56__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "cps_types.h"


const enum CPS_Serialize_Format 
{	
	CPS_SERIALIZE_FORMAT_1_0	   = 0x0100 // Version 1 0
};


const enum CPS_Timecode_Format 
{	
	CPS_TIMECODE_FORMAT_PAL_SECAM  = 0,	// PAL or SECAM
	CPS_TIMECODE_FORMAT_NTSC_NDF   = 1,	// NTSC (No drop frame)
	CPS_TIMECODE_FORMAT_NTSC_DF    = 2	// NTSC (Drop frame)
};


const m CPS_FRAMES_PER_DAY_PAL_SECAM  = 2160000;	// PAL or SECAM
const m CPS_FRAMES_PER_DAY_NTSC_NDF   = 2592000;	// NTSC (No drop frame)
const m CPS_FRAMES_PER_DAY_NTSC_DF	  = 2589408;	// NTSC (Drop frame)


#define  	TC1DAYN	0x278d00	/*	NTSC non drop frame	*/
#define		TC1DAYD	0x2782e0	/*	NTSC Drop Frame		*/
#define		TC1DAYP	0x20f580	/*	PAL			*/

class CTimecode : public CObject  
{

public:
	// Default serialization for the block
	DECLARE_SERIAL(CTimecode)
	virtual void Serialize(CArchive& ar);

protected:
	// Provide default constructor to allow serialisation, this is 
	// protected to stop it being used generally.
	CTimecode();

public:
	CTimecode(CPS_Timecode_Format p_Format);
	virtual ~CTimecode();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc);
#endif

	CTimecode(const CTimecode &p_Record);
	CTimecode(w p_hours_w, w p_minutes_w, w p_seconds_w, w p_frames_w, CPS_Timecode_Format p_Format);

	CTimecode &operator=(const CTimecode &p_rhs);
	CTimecode &operator=(const m &frames);
	CTimecode &operator+=(m p_frames_m);
	CTimecode &operator-=(m p_frames_m);

	CTimecode operator+(CTimecode &other);
	CTimecode operator-(CTimecode &other);
	z operator>(CTimecode &other)  {return (Get() >  other.Get());}
	z operator<(CTimecode &other)  {return (Get() <  other.Get());}
	z operator>=(CTimecode &other) {return (Get() >= other.Get());}
	z operator<=(CTimecode &other) {return (Get() <= other.Get());}
	z operator==(CTimecode &other) {return (Get() == other.Get());}
	z operator!=(CTimecode &other) {return (Get() != other.Get());}

	z ParseString(const CString &Timecode_Text);
	CString GetString();

	void SetHMSF(w  p_Hours_w,  w  p_Minutes_w,  w  p_Seconds_w,  w  p_Frames_w, CPS_Timecode_Format p_Format);
	void GetHMSF(w* p_Hours_wp, w* p_Minutes_wp, w* p_Seconds_wp, w* p_Frames_wp) const;

	m Get() const {return m_Frames;}

	z SetSeperator(const CString &p_Seperator);
	CString GetSeperator() const {return m_Seperator;}

	z SetFormat(const CPS_Timecode_Format &p_Format);
	CPS_Timecode_Format GetFormat() const {return m_Format;}

	void SetASCII(c *p_data_p, w p_bytes_w, CPS_Timecode_Format p_Format, z p_reverse_z);
	w GetASCII(c *p_data_p, w p_bytes_w, z p_reverse_z) const;

	void SetHex(b *p_data_p, w p_bytes_w, CPS_Timecode_Format p_Format, z p_reverse_z);
	w GetHex(b *p_data_p, w p_bytes_w, z p_reverse_z) const;

	void SetASCII(const CString *p_string_p, w p_bytes_w, w p_offset_w, CPS_Timecode_Format p_Format, z p_reverse_z);
protected:
	m		m_Frames;
	CString m_Seperator;

	CPS_Timecode_Format m_Format;
};

#endif // !defined(AFX_TIMECODE_H__08DB6071_F86A_11D0_AC5B_00805F81DE56__INCLUDED_)



#if 0
//
// Un-used code removed
//
b HexCharToByte(c p_hex_char_c)
{
	if ((p_hex_char_c >= '0')&&(p_hex_char_c <= '9'))
	{
		return (b)(p_hex_char_c - (b)'0');
	}
	else if ((p_hex_char_c >= 'A')&&(p_hex_char_c <= 'F'))
	{
		return (b)((p_hex_char_c - (b)'A') + 10);
	}
	return 0;
}

b HexByteToDecByte(b p_hex_b)
{
	b	dec_b = 0;

	if ((p_hex_b & 0x0F) < 0xA)
	{
		dec_b = (b)(p_hex_b & 0xF);
	}
	if (((p_hex_b >> 4) & 0xF) < 0xA)
	{

		dec_b += (b) (((p_hex_b >> 4) & 0xF) * 10);
	}
	return dec_b;
}



b DecByteToHexByte(b p_dec_b)
{
	return p_dec_b + (6 * (p_dec_b / 10));
}
#endif