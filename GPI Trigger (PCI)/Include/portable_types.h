///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Common
// File Name   : portable_types.h
// File Type   : C++ Header
// Description : Standard Types
// Author      : Rick Miles
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   15/03/96       Draft
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PORTABLE_TYPES_H
#define _PORTABLE_TYPES_H

// Define character types
typedef char  c;
#ifdef __AFXWIN_H__
typedef WCHAR wc;
typedef TCHAR tc;
#endif  // __AFXWIN_H_

// Define integer types (machine dependent types)
typedef int            i;
typedef unsigned short u;

// Define fixed bitlength types
typedef unsigned char  b;  // 8 bits
typedef unsigned short w;  // 16 bits
typedef short          s;
typedef unsigned long  m;  // 32 bits
typedef long           l;

// Define floating point types
typedef float  f;  // 6 decimal places
typedef double d;  // 15 decimal places

// Define boolean types
#ifndef __AFXWIN_H__
typedef int BOOL;
#define TRUE  1
#define FALSE 0
#endif  // __AFXWIN_H__
typedef BOOL z;  // TRUE/FALSE only

// Define null index values
static const m NULL_INDEX_M = 0xFFFFFFFF;
static const w NULL_INDEX_W = 0xFFFF;
static const b NULL_INDEX_B = 0xFF;

/*
#if _MSC_VER < 1100
typedef BOOL bool
#endif // _MSC_VER >= 1000
*/

#endif  // _PORTABLE_TYPES_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
