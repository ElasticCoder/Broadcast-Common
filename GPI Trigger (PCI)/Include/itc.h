///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : itc.h
// File Type   : C++ header
// Description : Inter thread communications control definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ITC_H
#define _ITC_H

#include <portable_types.h>  // Type definitions
#include <G00.h>                 // Services DLL import/export control
#include <g03_access_control.h>  // ...

class C_itq_info;

const l ITC_SYS_DEFAULT_HEAP_SIZE_L = 0;
const m ITC_NAME_LEN_M              = 63;

enum T_itc_status
{ 
    ITC_OK,
    ITC_ERROR,
    ITC_UNINITIALISED
};

struct S_itc_conf
{
    l m_heap_size_l;
	m m_max_queues_m;
	m m_max_queue_size_m;
};

class SERVICES_EXTERNAL C_itc
{
// Attributes
private:
    HANDLE               m_msg_data_heap_h;
    m                    m_q_list_idx_m;
    m                    m_q_free_list_idx_m;
    C_itq_info*          m_q_info_p;
    T_itc_status         m_itc_status;
    m                    m_max_queue_size_m;
    m                    m_max_queues_m;
    m                    m_data_allocated_m;
    m                    m_buffers_allocated_m;
    C_g03_access_control m_access_control;

// Implementation
public:
    C_itc();
    ~C_itc();

    C_itq_info              *find_queue(const tc *pc_queue_name_tcp);
    void                     free_queue(C_itq_info *p_queue_info_p);
    T_itc_status             initialise(const S_itc_conf& pc_config);
    void                    *alloc_msg_data(l p_msg_size_l);
    void                     free_msg_data(void *p_msg_p);
    inline T_g03_lock_status lock_itc(const C_g02_timeout_control& pc_timeout)
    {
        return m_access_control.lock(pc_timeout);
    };
	inline void              unlock_itc()
    {
        m_access_control.unlock();
    };
	inline z                 lock_held() const
    {
        return m_access_control.lock_held();
    };
	inline T_itc_status	     itc_status() const
    {
        return m_itc_status;
    };
	inline m                 max_queue_size() const
    {
        return m_max_queue_size_m;
    };
};

#endif  // _ITC_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
