///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_find_field.cpp
// File Type   : C++ Source
// Description : Find next date & time field in formatted input string
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include "g02_find_field.h"

///////////////////////////////////////////////////////////////////////////////
// Function Description : Find next field in format string. Strip off preceeding
//						: characters in both strings having matched them against
//						: data string.
//						: Return	0				- field found
//						:			NO_FIELD		- end of format string
//						:			DATA_MISMATCH	- format and data strings
//													  mismatch
// Author               : Duncan Olesen
// Date Created         : 29/05/96
// Date Last Modified   : 29/05/96
///////////////////////////////////////////////////////////////////////////////

int g02_find_field(CString &p_format, CString &p_data)
{
	int	ret_val = 0;
	int	format_length;
	int data_length;
	int fpos;
	int dpos;
	z	finished_z;

	format_length	= p_format.GetLength();
	data_length		= p_data.GetLength();
	fpos			= 0;
	dpos			= 0;
	finished_z		= FALSE;

	while (!finished_z)
	{
//
// Skip whitespace in format string
//
		while (fpos < format_length &&
			   _istspace(p_format[fpos]))
		{
			fpos++;
		}
//
// Skip whitespace in data string
//
		while (dpos < data_length &&
			   _istspace(p_data[dpos]))
		{
			dpos++;
		}
//
// Check if at end of format string
//
		if (fpos == format_length)
		{
			ret_val = NO_FIELD;
			finished_z = TRUE;
		}
		else if (p_format[fpos] == _TEXT('%'))
		{
//
// If character in format string is '%' then we have found the field
//
			ret_val = 0;
			finished_z = TRUE;
		}
		else if (dpos < data_length)
		{
			if (p_format[fpos] == p_data[dpos])
			{
//
// Current characters match so advance to next characters
//
				fpos++;
				dpos++;
			}
			else
			{
//
// current characters do not match
//
				ret_val = DATA_MISMATCH;
				finished_z = TRUE;
			}
		}
		else
		{
//
// At end of data string, so just go to next character in format string
//
			fpos++;
		}
	}
//
// Strip off the characters scanned over
//
	p_format = p_format.Mid(fpos);
	p_data   = p_data.Mid(dpos);

	return ret_val;
}
