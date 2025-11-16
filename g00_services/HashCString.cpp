///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : HashCString.cpp
// File Type   : C++ Source
// Description : CString HasKey function
// Author      : Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <portable_types.h>
#include <G00.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : Hash CString key
///////////////////////////////////////////////////////////////////////////////

SERVICES_EXTERNAL UINT __stdcall HashKey(CString& rKey)
{
// Declare local data
    i       iCurrCharPos;  // Current character position
    m       mHashValue;    // Hash value
    CString HashString;    // Hash string

// Calculate hash value
    HashString = rKey;
    HashString.MakeUpper();
    mHashValue = 0;
    for (iCurrCharPos = HashString.GetLength() - 1; iCurrCharPos >= 0; iCurrCharPos--)
    {
        mHashValue  = mHashValue << 5;
        mHashValue += HashString[iCurrCharPos];
    }

// Return hash value to caller
    return mHashValue;
}

