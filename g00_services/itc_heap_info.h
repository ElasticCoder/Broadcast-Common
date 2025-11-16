///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Radio Automation Workstation
// Subsystem   : Communications
// File Name   : itc_heap_info.h
// File Type   : C++ Header
// Description : ITC heap data strcutures
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   24/01/97		Draft
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _ITC_HEAP_INFO_H
#define _ITC_HEAP_INFO_H

const m HEAP_SENTINEL_M = 0xAA55AA55;

#ifdef _DEBUG
#include <g00_process_registry.h>
#endif _DEBUG
//
// Declare enumeration type for status of allocated buffer
//
enum T_ITC_MSG_DATA_STATUS 
{
	ITC_DATA_ALLOCATED,			// Buffer allocated but not sent
	ITC_DATA_SENT,				// Buffer has been written to a queue
	ITC_DATA_READ,				// Buffer has been read from a queue
	ITC_DATA_POSTED				// Buffer has been posted to a windows queue
};
struct S_heap_head_sentinel
{
	m	m_magic_num1_m;
	m	m_data_size_m;
#ifdef _DEBUG
	T_ITC_MSG_DATA_STATUS	m_status;		// Status of buffer
	C_g00_thread_name		m_alloc_thread;	// Thread which allocated buffer
	m						m_msg_id_m;		// ID of first msg buffer written as
	tc						m_read_q_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
											// Name of queue last written to
#endif _DEBUG
	m	m_magic_num2_m;
};

struct S_heap_tail_sentinel
{
	m	m_magic_num_m;
};
//
// Funtion which outputs details of all allocated blocks in the ITC heap
// Housekeeping data is not output
//
#ifdef _DEBUG
void itc_dump_heap(HANDLE		hHeap);
#endif _DEBUG

#endif _ITC_HEAP_INFO_H