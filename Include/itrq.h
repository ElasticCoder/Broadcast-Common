///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : itrq.h       
// File Type   : C++ header
// Description : Inter thread communications read queue definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ITRQ_H
#define _ITRQ_H

#include <portable_types.h>  // Type definitions
#include <G00.h>     // Services DLL import/export control
#include <itc.h>     // Inter thread communications control definitions
#include <itq.h>     // Inter thread communications queue definitions

///////////////////////////////////////////////////////////////////////////////
// Define enumerations
///////////////////////////////////////////////////////////////////////////////
enum T_itrq_read_status
{
	RQ_ERROR,			// an error occurred, no message read
	RQ_OK,				// message read successfully
	RQ_TIMEOUT,			// no message read, timeout expired
	RQ_DATA_OVERFLOW,	// message data too large for buffer provided,
						// no message read
	RQ_OBJECT_SIGNALLED,// One of the other kernel objects signalled,
						// no message read
	RQ_WAIT_ABANDONED	// One of the kernel objects was a mutex and was abandoned
};

///////////////////////////////////////////////////////////////////////////////
// Define read queue class
///////////////////////////////////////////////////////////////////////////////
class SERVICES_EXTERNAL C_itrq : public C_itq
{
// Implementation
public:
    C_itrq();
    C_itrq(C_itc* p_itc_p);
	C_itrq(C_itc	*p_itc_p,
		   z		p_windows_queue_z,
		   HWND		p_window_h,
		   m		p_queue_size_m,
		   const tc	*pc_queue_name_tcp = NULL);	// name of queue if not NULL
    ~C_itrq();

	T_itq_q_status initialise(z           p_windows_queue_z,
                              HWND        p_window_h,
                              m			  p_queue_size_m,
                              const DWORD pc_dwTimeout = INFINITE,
							  const tc	  *pc_queue_name_tcp = NULL); // name of queue if not NULL
    T_itq_q_status initialise(C_itc		  *p_itc_p,
                              z           p_windows_queue_z,
                              HWND        p_window_h,
                              m           p_queue_size_m,
                              const DWORD pc_dwTimeout = INFINITE,
							  const tc	  *pc_queue_name_tcp = NULL); // name of queue if not NULL
	virtual void	disconnect();
	T_itrq_read_status read(
			C_itq_q_msg		&p_msg,					// details of message read
			const DWORD		pc_dwTimeout,			// max time to wait on queue
			const HANDLE	*pc_handle_ph = NULL,	// array of other objects to wait on
			w				p_num_handles_w = 0,	// number of other objects
			w				*p_signalled_object_pw = NULL);
												// index of object signalled
	T_itrq_read_status read(
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
	T_itq_q_status init(z			p_windows_queue_z,
                        HWND		p_window_h,
                        m			p_queue_size_m,
                        DWORD		p_dwTimeout,
						const tc	*pc_queue_name_tcp);
};

#endif  // _ITRQ_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
