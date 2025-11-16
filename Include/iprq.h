///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : iprq.h       
// File Type   : C++ header
// Description : Inter process communications read queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IPRQ_H
#define _IPRQ_H

#include <portable_types.h>  // Type definitions
#include <G00.h>     // Services DLL import/export control
#include <ipc.h>     // Inter process communications control definitions
#include <ipq.h>     // Inter process communications queue definitions

enum T_iprq_read_status
{
	IPQ_RQ_ERROR,			// an error occurred, no message read
	IPQ_RQ_OK,				// message read successfully
	IPQ_RQ_TIMEOUT,			// no message read, timeout expired
	IPQ_RQ_DATA_OVERFLOW,	// message data too large for buffer provided,
							// no message read
	IPQ_RQ_OBJECT_SIGNALLED,// One of the other kernel objects signalled,
							// no message read
	IPQ_RQ_WAIT_ABANDONED	// One of the kernel objects was a mutex and was abandoned
};

class SERVICES_EXTERNAL C_iprq : public C_ipq
{
// Implementation
public:
    C_iprq();
    C_iprq(C_ipc* p_ipc_p);
    ~C_iprq();

	T_ipq_q_status initialise(
		const DWORD	pc_dwTimeout = INFINITE,
		const tc	*pc_queue_name_tcp = NULL);	// name of queue if not NULL

	T_ipq_q_status initialise(
		C_ipc		*p_ipc_p,
		const DWORD	pc_dwTimeout = INFINITE,
		const tc	*pc_queue_name_tcp = NULL);	// name of queue if not NULL
	virtual void   disconnect();
	//
	// Test if there are any messages on the queue, return FALSE if so
	//
	z				is_empty() const;
	T_iprq_read_status read(
		C_ipq_q_msg		&p_msg,					// details of message read
		const DWORD		pc_dwTimeout,			// max time to wait on queue
		const HANDLE	*pc_handle_ph = NULL,	// array of other objects to wait on
		w				p_num_handles_w = 0,	// number of other objects
		w				*p_signalled_object_pw = NULL);
												// index of object signalled

	T_iprq_read_status read(
		m				&p_msg_id_m,			// Id of message read
		void			*p_msg_buffer_p,		// buffer to receive message data
		m				p_buffer_size_m,		// size of buffer
		m				&p_msg_data_size_m,		// size of message data read
		const DWORD		pc_dwTimeout,			// max time to wait on queue
		const HANDLE	*pc_handle_ph = NULL,	// array of other objects to wait on
		w				p_num_handles_w = 0,	// number of other objects
		w				*p_signalled_object_pw = NULL);
												// index of object signalled

private:
	T_ipq_q_status	init(
						DWORD		p_dwTimeout,
						const tc	*pc_queue_name_tcp);	// name of queue if not NULL
};

#endif  // _IPRQ_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
