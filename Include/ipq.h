//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipq.h       
// File Type   : C++ header
// Description : Inter process communications queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IPQ_H
#define _IPQ_H

#include <portable_types.h>  // Type definitions
#include <ipc.h>    // Inter process communications control definitions

class C_ipq_info;

class SERVICES_EXTERNAL C_ipq
{
// Attributes
protected:
    C_ipc*      m_ipc_p;
    C_ipq_info* m_q_info_p;
    HANDLE      m_read_semaphore_h;
    HANDLE      m_write_semaphore_h;

// Implementation
public:
    C_ipq();
    C_ipq(C_ipc* p_ipc_p);
    ~C_ipq();

    T_ipq_q_status queue_status() const;
	virtual void   queue_name(tc *p_queue_name_tcp) const;
    virtual void*  alloc_msg_data(m p_msg_size_m);
    virtual void   free_msg_data(void* p_msg_p);
	virtual void   disconnect();

private:
//
// Declare the copy constructor and assignment operator
// private in order to prevent misuse of queues.
//
// C_ipq objects and more specifically objects of derived
// classes must be passed by reference to functions rather
// than by value
//
	C_ipq(const C_ipq &p_queue);
	C_ipq &operator=(const C_ipq &p_queue);
};

#endif  // _IPQ_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
