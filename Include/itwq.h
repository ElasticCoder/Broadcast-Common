///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : itwq.h       
// File Type   : C++ Header
// Description : Inter thread communications write queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ITWQ_H
#define _ITWQ_H

#include <portable_types.h>  // Type definitions
#include <itq.h>    // Inter thread communications queue definitions
#include <itc.h>    // Inter thream communications control definitions

enum T_itwq_write_status
{
    WQ_ERROR,
    WQ_OK,
    WQ_TIMEOUT
};

class SERVICES_EXTERNAL C_itwq : public C_itq
{
public:
    C_itwq();
    C_itwq(C_itc* p_itc_p);
    C_itwq(const tc* pc_queue_name_tcp,
           C_itc*    p_itc_p);
    C_itwq(w      p_thread_num_w,
           w      p_thread_inst_w,
           C_itc* pc_itc_p);
    ~C_itwq();

	T_itq_q_status initialise(const tc    *pc_queue_name_tcp,
                              const DWORD pc_dwTimeout = INFINITE);

    T_itq_q_status initialise(C_itc       *p_itc_p,
                              const tc    *pc_queue_name_tcp,
                              const DWORD pc_dwTimeout = INFINITE);
	
	T_itq_q_status initialise(w			  p_thread_num_w,
                              w			  p_thread_inst_w,
                              const DWORD pc_dwTimeout = INFINITE);
    
	T_itq_q_status initialise(C_itc       *p_itc_p,
                              w           p_thread_num_w,
                              w           p_thread_inst_w,
                              const DWORD pc_dwTimeout = INFINITE);
	
	T_itwq_write_status write(C_itq_q_msg p_msg,
                              const DWORD pc_dwTimeout = INFINITE);
	
	T_itwq_write_status write(C_itq_q_msg p_msg,
                              const void  *p_msg_data_p,
                              m     p_msg_data_size_m,
                              const DWORD pc_dwTimeout = INFINITE);

private:
	T_itq_q_status init(const tc*   pc_queue_name_tcp,
                        const DWORD p_dwTimeout);
};

#endif  // _ITWQ_H

///////////////////////////////////////////////////////////////////////////////
// End of File
///////////////////////////////////////////////////////////////////////////////
