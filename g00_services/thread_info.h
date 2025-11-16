///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : thread_info.h
// File Type   : C++ Header
// Description : Class for Register of threads
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _THREAD_INFO_H
#define _THREAD_INFO_H

struct S_thread_info
{
public:
	w					m_thread_num_w;
	w					m_thread_inst_w;
	HANDLE				m_handle_h;
};

#endif

