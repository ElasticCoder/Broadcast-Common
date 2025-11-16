///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipwq.h       
// File Type   : C++ header
// Description : Inter process communications write queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IPWQ_H
#define _IPWQ_H

#include <portable_types.h>		   // Type definitions
#include <G00.h>                   // Services DLL import/export control
#include <g00_process_registry.h>  // Application and thread register definitions
#include <ipc.h>                   // Inter process communications control definitions
#include <ipq.h>                   // Inter process communications queue definitions

enum T_ipwq_write_status
{
    IPQ_WQ_ERROR,
    IPQ_WQ_OK,
    IPQ_WQ_TIMEOUT
};

class SERVICES_EXTERNAL C_ipwq : public C_ipq
{
public:
	C_ipwq();
	C_ipwq(C_ipc* p_ipc_p);
	C_ipwq(const tc* pc_queue_name_tcp,
		   C_ipc*    p_ipc_p);
	C_ipwq(T_g00_app_type p_app_type,
		   w              p_app_inst_w,
		   C_ipc*         pc_ipc_p);
	~C_ipwq();

// Implementation
public:
    T_ipq_q_status initialise(const tc*   pc_queue_name_tcp,
                              const DWORD pc_dwTimeout = INFINITE);

    T_ipq_q_status initialise(C_ipc*         p_ipc_p,
                              const tc*		 pc_queue_name_tcp,
                              const DWORD    pc_dwTimeout = INFINITE);

    T_ipq_q_status initialise(T_g00_app_type p_app_type,
                              w              p_app_inst_w,
                              const DWORD    pc_dwTimeout = INFINITE);

    T_ipq_q_status initialise(C_ipc*         p_ipc_p,
                              T_g00_app_type p_app_type,
                              w              p_app_inst_w,
                              const DWORD    pc_dwTimeout = INFINITE);

    T_ipwq_write_status write(C_ipq_q_msg p_msg,
                              const DWORD pc_dwTimeout = INFINITE);

    T_ipwq_write_status write(C_ipq_q_msg p_msg,
                              const void  *p_msg_data_p,
                              m           p_msg_data_size_m,
                              const DWORD pc_dwTimeout = INFINITE);

private:
    T_ipq_q_status init(const tc*   pc_queue_name_tcp,
                        const DWORD p_dwTimeout);
};

#endif  // _IPWQ_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
