///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : g02_timer.h       
// File Type   : C++ header
// Description : Timer services definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G02_TIMER_H
#define _G02_TIMER_H

#include <portable_types.h>  // Type definitions
#include <G00.h>        // Services DLL import/export control
#include <G02.h>        // Time/Timer event definitions
#include <time.h>       // Time definitions
#include <itc.h>        // Inter thread communications control definitions
#include <itq_types.h>  // Inter process queue type definitions
#include <g02_timer_id.h>

const m G02_INFINITE_TIMEOUTS_M  = 0xFFFFFFFF;

//
// Structure of data in timeout message
//
struct T_g02_timeout_msg
{
public:
    void*          m_timeout_data_p;      // Copy of data in original request
    m              m_timeout_data_size_m;
    T_g02_timer_id m_timer_id;
};

///////////////////////////////////////////////////////////////////////////////////
// Initialise Timer service
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL z g02_init_timer(C_itc* p_itc_p,
						           m      p_queue_size_m,
						           DWORD  p_dwTimeout = 10000);  // 10 second default timeout

///////////////////////////////////////////////////////////////////////////////////
// Terminate Timer service
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL void g02_stop_timer();

///////////////////////////////////////////////////////////////////////////////////
// Request Periodic (Multiple) Timeout Message relative to now
// The value returned is an ID to be used to cancel the timeout
// The optional thread num and inst parameters are used to specify an
// alternative thread to receive the timeout messages
// The optional no timeout data parameter can be used to specify that no
// T_g02_timeout_msg data is sent with the timeout message, i.e. just
// the timeout message id is sent. If this is the case no additional
// message data can be specified in p_timeout_msg
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL T_g02_timer_id g02_rqt_multi_timeout(m           p_initial_timeout_m,
                                                       m           p_period_m,
                                                       m           p_num_of_timeouts_m,
                                                       C_itq_q_msg p_timeout_msg,
                                                       w           p_dest_thread_num_w = 0,
                                                       w           p_dest_thread_inst_w = 0,
													   z		   p_no_timeout_data_z = FALSE);

///////////////////////////////////////////////////////////////////////////////////
// Request Periodic (Multiple) Timeout Message with absolute start
// The value returned is an ID to be used to cancel the timeout
// The optional thread num and inst parameters are used to specify an
// alternative thread to receive the timeout messages
// The optional no timeout data parameter can be used to specify that no
// T_g02_timeout_msg data is sent with the timeout message, i.e. just
// the timeout message id is sent. If this is the case no additional
// message data can be specified in p_timeout_msg
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL T_g02_timer_id g02_rqt_multi_timeout(const C_g02_datetime& p_initial_timeout,
                                                       m                     p_period_m,
                                                       m                     p_num_of_timeouts_m,
                                                       C_itq_q_msg           p_timeout_msg,
                                                       w                     p_dest_thread_num_w = 0,
                                                       w                     p_dest_thread_inst_w = 0,
													   z					 p_no_timeout_data_z = FALSE);

///////////////////////////////////////////////////////////////////////////////////
// Request Single Timeout Message relative to now
// The value returned is an ID to be used to cancel the timeout
// The optional thread num and inst parameters are used to specify an
// alternative thread to receive the timeout messages
// The optional no timeout data parameter can be used to specify that no
// T_g02_timeout_msg data is sent with the timeout message, i.e. just
// the timeout message id is sent. If this is the case no additional
// message data can be specified in p_timeout_msg
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL T_g02_timer_id g02_rqt_single_timeout(m           p_initial_timeout_m,
                                                        C_itq_q_msg p_timeout_msg,
                                                        w           p_dest_thread_num_w = 0,
                                                        w           p_dest_thread_inst_w = 0,
														z			p_no_timeout_data_z = FALSE);

///////////////////////////////////////////////////////////////////////////////////
// Request Single absolute Timeout Message
// The value returned is an ID to be used to cancel the timeout
// The optional thread num and inst parameters are used to specify an
// alternative thread to receive the timeout messages
// The optional no timeout data parameter can be used to specify that no
// T_g02_timeout_msg data is sent with the timeout message, i.e. just
// the timeout message id is sent. If this is the case no additional
// message data can be specified in p_timeout_msg
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL T_g02_timer_id g02_rqt_single_timeout(const C_g02_datetime& p_initial_timeout,
                                                        C_itq_q_msg           p_timeout_msg,
                                                        w                     p_dest_thread_num_w = 0,
                                                        w                     p_dest_thread_inst_w = 0,
														z					  p_no_timeout_data_z = FALSE);

///////////////////////////////////////////////////////////////////////////////////
// Cancel Timeout
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL void g02_cancel_timeout(T_g02_timer_id p_timer_id);

///////////////////////////////////////////////////////////////////////////////////
// Cancel All Timeouts or All Timeouts excluding infinite timeouts
// The p_exclude_infinite_z parameter is used to specify whether multi-shot timeouts
// with an infinite number (p_num_of_timeouts_m = INFINITE) are cancelled
// (p_exclude_infinite_z = FALSE) or not (p_exclude_infinite_z = TRUE)
///////////////////////////////////////////////////////////////////////////////////
SERVICES_EXTERNAL void g02_cancel_all_timeouts(z	p_exclude_infinite_z);



#endif  // _G02_TIMER_H

///////////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////////
