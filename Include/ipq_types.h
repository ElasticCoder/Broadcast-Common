///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : ipq_types.h       
// File Type   : C++ header
// Description : Inter process queue type definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IPQ_TYPES_H
#define _IPQ_TYPES_H

#include <portable_types.h>  // Type definitions
#include <g00_process_registry.h>  // ...

const b IPQ_LOW_PRIORITY_B  = 0;
const b IPQ_HIGH_PRIORITY_B = 128;

#define _IPQ_QUEUE_NAME_LEN 63
const m IPQ_QUEUE_NAME_LEN_M = _IPQ_QUEUE_NAME_LEN;

//
// Ensure there is room for application name in the queue name
//
#if (_IPQ_QUEUE_NAME_LEN) < (_MAX_APP_NAME_LEN)
	#error IPQ_QUEUE_NAME_LEN_M < G00_MAX_APP_NAME_LEN_W
#endif

enum T_ipq_q_status
{
    IPQ_OK,
    IPQ_UNINITIALISED,
    IPQ_WAITING_FOR_READER,
    IPQ_ERROR,
    IPQ_FREE,
    IPQ_INIT_TIMEOUT
};

class SERVICES_EXTERNAL C_ipq_q_msg
{
public:
    m              m_id_m;
    void*          m_data_p;
    m              m_msg_data_size_m;
    b              m_priority_b;
    C_g00_app_name m_sender;

	inline C_ipq_q_msg(): m_sender()
	{
        m_id_m	          = 0;
        m_data_p          = NULL;
        m_msg_data_size_m = 0;
        m_priority_b      = IPQ_LOW_PRIORITY_B;
	};
	inline ~C_ipq_q_msg()
    {
    };
};

#endif  // _IPQ_TYPES_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
