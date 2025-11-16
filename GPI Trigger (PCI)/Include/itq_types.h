///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : itq_types.h       
// File Type   : C++ header
// Description : Inter thread queue type definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ITQ_TYPES_H
#define _ITQ_TYPES_H

#include <portable_types.h>			// Type definitions
#include <g00_process_registry.h>	// Application and thread register definitions

const b ITQ_LOW_PRIORITY_B  = 0;
const b ITQ_HIGH_PRIORITY_B = 128;

#define _ITQ_QUEUE_NAME_LEN 63
const m ITQ_QUEUE_NAME_LEN_M = _ITQ_QUEUE_NAME_LEN;

//
// Ensure there is room for thread name in the queue name
//
#if (_ITQ_QUEUE_NAME_LEN) < (_MAX_THREAD_NAME_LEN_)
	#error ITQ_QUEUE_NAME_LEN_M < G00_MAX_THREAD_NAME_LEN_W
#endif

enum T_itq_q_status
{
	Q_OK,
	Q_UNINITIALISED,
	Q_WAITING_FOR_READER,
	Q_ERROR,
	Q_FREE,
	Q_INIT_TIMEOUT
};

class SERVICES_EXTERNAL C_itq_q_msg
{
public:
    m     m_id_m;
    void* m_data_p;
    m     m_msg_data_size_m;
    b     m_priority_b;

    inline C_itq_q_msg()
    {
        m_id_m            = 0;
        m_data_p          = NULL;
        m_msg_data_size_m = 0;
        m_priority_b      = ITQ_LOW_PRIORITY_B;
    };
    inline ~C_itq_q_msg()
    {
    };
};

#endif  // _ITQ_TYPES_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
