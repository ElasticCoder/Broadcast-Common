///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : itq.h       
// File Type   : C++ header
// Description : Inter thread communications queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ITQ_H
#define _ITQ_H

#include <G00.h>        // Services DLL import/export control
#include <itc.h>        // Inter thread communications control definitions
#include <itq_types.h>  // Inter thread queue type definitions

class C_itq_info;

class SERVICES_EXTERNAL C_itq
{
// Attributes
protected:
    C_itc      *m_itc_p;
    C_itq_info *m_q_info_p;
    HANDLE      m_read_semaphore_h;
    HANDLE      m_write_semaphore_h;

// Implementation
public:
    C_itq();
    C_itq(C_itc *p_itc_p);
    ~C_itq();

    T_itq_q_status queue_status() const;
	//
	// Test if there are any messages on the queue, return FALSE if so
	//
	z				is_empty() const;
	// returns name of queue in p_queue_name_tcp
	void			queue_name(tc *p_queue_name_tcp) const;
    virtual void	*alloc_msg_data(m p_msg_size_m);
    virtual void	free_msg_data(void *p_msg_p);
	virtual void	disconnect();

private:
//
// Declare the copy constructor and assignment operator
// private in order to prevent misuse of queues.
//
// C_itq objects and more specifically objects of derived
// classes must be passed by reference to functions rather
// than by value
//
	C_itq(const C_itq &p_queue);
	C_itq &operator=(const C_itq &p_queue);
};

#endif  // _ITQ_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
