///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g02_find_field.h
// File Type   : C++ Header
// Description : Find next date & time field in formatted input string
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_FIND_FIELD_H
#define _G02_DATETIME_H

#define NO_FIELD (-1)
#define DATA_MISMATCH (-2)

int g02_find_field(CString &p_format, CString &p_data);

#endif _G02_FIND_FIELD_H
