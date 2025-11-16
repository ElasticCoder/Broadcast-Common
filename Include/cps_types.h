///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1997; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Cassette Preparation System
// File Name   : cps_types.h
// File Type   : C++ Header
// Description : Standard Types
// Author      : Piers Lawson
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   08/07/97		Draft
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _CPS_TYPES_H
#define _CPS_TYPES_H

// Character types

typedef char  c;
typedef WCHAR wc;
typedef TCHAR tc;

// Integer types (machine dependant types)

typedef int i;
typedef int n;

// Fixed bitlength types

typedef unsigned char  b;    // 8 bit
typedef short          s;
typedef unsigned short w;    // 16 bit
typedef long           l;
typedef unsigned long  m;    // 32 bit

// Misc

typedef float  f;            // 6 Dec places
typedef double d;            // 15 Dec places

typedef BOOL   z;            // TRUE/FALSE only


#endif
