///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : Services
// File Name   : ipc.h       
// File Type   : C++ header
// Description : Inter process communications control definitions
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IPC_H
#define _IPC_H

#include <ipq_types.h>

#include <portable_types.h>		 // Type definitions
#include <g03_heap.h>            // ...
#include <g03_partition_ctrl.h>  // ...
#include <g03_access_control.h>  // ...

class C_ipq_info;

const w IPC_DEFAULT_PARTITIONS_W = 0;
const w IPC_NAME_LEN_W           = 40;

enum T_ipc_status
{
    IPC_OK,
    IPC_ERROR,
    IPC_UNINITIALISED
};

struct S_ipc_conf
{
    tc                            m_ipc_name_tca[IPC_NAME_LEN_W + 1];
    w                             m_num_partitions_w;
    const S_g03_partition_config *m_partition_conf_p;
    m                             m_max_queues_m;
    m                             m_queue_size_m;
};

struct S_ipc_info;

//
// Inter Process Control class
//
// Each process wishing to use inter process communications will
// create an object of this class. The object will give the process
// access to the memory shared with other processes that enables
// inter process communication.
//
// IPC shared memory consists of
//
//	1)	Control Memory
//				header info
//				array of queues
//	2)	Message Data Heap - to enable allocation of memory to hold message data
//	3)	Message Holder Array Heap - each queue requires an array of message 
//					holders, one holder per message on the queue
//
class SERVICES_EXTERNAL C_ipc
{
// Attributes
private:
	// Inter Process Control information object
    struct S_ipc_info
    {
        T_ipc_status m_ipc_status;
        m            m_max_queues_m;
        m            m_queue_size_m;
        m            m_q_list_idx_m;       // Active queue list head pointer
        m            m_q_free_list_idx_m;
	};

    C_g03_access_control m_access_control;  // Exclusive access control
    C_g03_heap           m_message_heap;    // Inter process message buffer heap
	C_g03_heap           m_holder_heap;     // The heap for queued message holders
	C_g03_shared_memory	 m_control_memory;  // Shared memory control information and queue info records
	S_ipc_info          *m_ipc_info_p;      // Shared memory (queue heap) control information pointer
	C_ipq_info          *m_q_info_p;        // Shared memory queue info records pointer

// Implementation
public:
	C_ipc();                          // Construct uninitialised IPC
	C_ipc(const tc *p_ipc_name_tcp);  // Construct initialised IPC
	~C_ipc();                         // Destruct IPC

//
// Methods to control exclusive access to the object
//
	inline T_g03_lock_status lock_ipc(const C_g02_timeout_control& pc_timeout)
    {
        return m_access_control.lock(pc_timeout);
    };
	inline void	unlock_ipc()
    {
        m_access_control.unlock();
    };
	inline z lock_held() const
    {
        return m_access_control.lock_held();
    };
//
// Find a named queue or create one if not found, maintaining usage count
//
	C_ipq_info *find_queue(const tc *pc_queue_name_tcp);
//
// Reduce queue usage count and release the queue if no longer used
//
	void free_queue(C_ipq_info *p_queue_info_p);
//
// Initialise the object and shared memory so that it can be used by this
// and other processes
//
	T_ipc_status initialise(const S_ipc_conf& pc_config);
//
// Attach to initialised IPC shared memory
//
	z attach(const tc *p_ipc_name_tcp);
//
// Allocate/Free memory for inter process message data
//
	inline void *alloc_msg_data(l p_msg_size_l)
    {
        return m_message_heap.alloc(p_msg_size_l);
    };
	inline void free_msg_data(void *p_msg_p)
    {
        m_message_heap.free(p_msg_p);
    };
	inline void *msg_data_addr(T_g03_memory_id p_data_id) const
    {
        return m_message_heap.address(p_data_id);
    };
	inline T_g03_memory_id msg_data_id(void *p_data_p) const
    {
        return m_message_heap.memory_id(p_data_p);
    };
//
// Allocate / Free memory for message holder arrays
//
	T_g03_memory_id	alloc_holders();
	void free_holders(T_g03_memory_id p_holders);
	inline void *holder_address(T_g03_memory_id p_holder) const
    {
        return m_holder_heap.address(p_holder);
    };
//
// Return status of this object and shared memory
//
	T_ipc_status ipc_status() const;
//
// Return the maximum number of messages held in queues
//
	m queue_size() const;
//
// Return High Water Mark for all the partitions in the message heap
// The function returns a pointer to an array of records allocated from the
// default process heap. Each record specifies the size of blocks in the partition
// and the maximum number allocated simultaneously
//
	inline S_g03_partition_config *high_water() const
	{
        return m_message_heap.high_water();
    };
};

#endif // _IPC_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
