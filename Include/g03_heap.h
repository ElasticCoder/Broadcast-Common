///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g03_heap.h       
// File Type   : C++ Header
// Description : Heap Shared By Several Processes Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _G03_SHARED_HEAP_H
#define _G03_SHARED_HEAP_H

#include <g03_types.h>
#include <g03_partition_ctrl.h>
#include <g03_shared_memory.h>
//
// This class implements a heap for dynamic allocation of memory to
// be shared between processes.
// The heap will be stored in a memory mapped (paging) file area. The
// memory area will be mapped into each process at a different address
// so the class cannot use pointers, but must use indices and offsets
// from the start of the mapped area.
//
// A heap consists of a number of partitions.
//
// Each partition contains a number of elements which can be allocated.
// The size of all elements in a partition is the same. Thus to be able
// to allocate different amounts of memory partitions with different
// element sizes must be configured.
//
// The heap has an array of partition control blocks, one control block
// per partition of the heap. The control block holds the offset from
// the start of the heap where its partition is stored.
//

class SERVICES_EXTERNAL C_g03_heap
{
public:
	C_g03_heap();
	C_g03_heap(const tc	*p_heap_name_tcp);
	~C_g03_heap();

// Attributes
private:

	struct S_heap_config
	{
//
// Magic number is used to indicate that the memory holds a heap controller
//
		m		m_magic_num_m;
		w		m_num_partitions_w;
		w		m_usage_count_w;
		m		m_heap_size_m;	// memory needed for heap
		m		m_allocated_m;	// total memory allocated (including internal
								// fragmentation)
		m		m_buffers_m;	// buffers currently allocated
	};

	C_g03_shared_memory		m_shared_memory;
	S_heap_config			*m_heap_config_p;
	C_g03_partition_ctrl	*m_partition_ctrl_p;

// Implementation
public:
//
// Configure the heap
//
	z 			configure(
					const tc	*p_heap_name_tcp,
					w			p_num_partitions_w,
					const S_g03_partition_config
								*p_partition_conf_p);
//
// Attach to a configured heap
//
	void		attach(const tc	*p_heap_name_tcp);
//
// Allocate memory suitable for sharing between processes
//
	void		*alloc(l p_size_l);
//
// Free memory shared between processes
//
	void		free(void *p_mem_p);
//
// Convert address of allocated element into a process
// independent identifier
//
	T_g03_memory_id	memory_id(const void *p_mem_p) const
							{ return m_shared_memory.memory_id(p_mem_p); };
//
// Convert a process independent identifier into an address
//
	void		*address(T_g03_memory_id p_memory_id) const
							{ return m_shared_memory.address(p_memory_id); };
//
// Return High Water Mark for all the partitions in the heap
// The function returns a pointer to an array of records allocated from the
// default process heap. Each record specifies the size of blocks in the partition
// and the maximum number allocated simultaneously
//
	S_g03_partition_config	*high_water() const;

private:
	//
	// return the number of bytes required for a
	// heap with a given configuration
	//
	static m	mem_required(
					w		p_num_partitions_w,
					const S_g03_partition_config
							*p_partition_conf_p);
};

#endif _G03_SHARED_HEAP_H
