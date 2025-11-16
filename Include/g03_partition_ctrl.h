///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g03_partition_ctrl.h
// File Type   : C++ Header
// Description : Heap Partition Control Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _G03_PARTITION_CTRL_H
#define _G03_PARTITION_CTRL_H

#include <g03_types.h>

class C_g03_partition_ctrl
{
public:
//
// No constructors / destructors as objects reside in shared memory
//
// Members
private:
	w	m_partition_num_w;
	m	m_size_of_element_m;
	m	m_num_elements_m;
	m	m_partition_offset_m;
	m	m_highest_used_m;
	m	m_free_index_m;

// Implementation
public:
	//
	// return the number of bytes required for a
	// partition with a given configuration
	//
	static	m	mem_required(
					const S_g03_partition_config
							&p_config);
	//
	// Return the partition number of an allocated element
	//
	static	w	partition_num(void	*p_element_p);
	//
	// Initialise a partition control object
	//
	void		init(
					const S_g03_partition_config
							&p_config,
					w		p_partition_num_w,
					m		p_partition_offset_m);
	//
	// Allocate an element (block of memory) from the
	// partition. The base address is the base address
	// to which the partition offset is applied 
	//
	void		*alloc_element(
					void	*p_base_addr_p);
	//
	// Release an element back to the partition
	//
	void		free_element(
					void	*p_base_addr_p,
					void	*p_element_p);
	//
	// Return the size of the elements in the partition
	//
	inline m	element_size() const { return m_size_of_element_m; };
	//
	// Return the largest number of elements allocated simultaneously
	//
	inline m	high_water_mark() const { return m_highest_used_m; };
private:
	//
	// Return the address of an indexed element
	//
	void		*element(
					void	*p_base_addr_p,
					m		p_index_m) const;
};

#endif _G03_PARTITION_CTRL_H
