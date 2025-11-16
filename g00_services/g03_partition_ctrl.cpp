///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g03_partition_ctrl.cpp
// File Type   : C++ Source
// Description : Heap Partition Control Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   22/04/96		Draft
// 0.2	 23/08/96		Fix m_size_of_element_m error
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>

#include <g03_partition_ctrl.h>

#include "g03_event_codes.h"

const m NULL_IDX_M = 0xFFFFFFFF;
const m FREE_GUARD_NUM_M = 0xAA55AA55;
const w ALLOC_GUARD_NUM_W = 0x55AA;
const m TAIL_GUARD_NUM_M = 0xA5A55A5A;

struct S_alloc_sentinel
{
	w	m_magic_num_w;
	w	m_partition_num_w;
};

struct S_free_sentinel
{
	m	m_magic_num_m;
	m	m_next_idx_m;
};

struct S_tail_sentinel
{
	m	m_magic_num_m;
};

//
// Return the actual amount of memory needed to hold a element of a partition
// including sentinels
//
m true_size(m	p_element_size_m);
//
// Return size of an element rounded up to the required multiple
//
m rounded_size(m	p_element_size_m);

///////////////////////////////////////////////////////////////////////////////
// Function Description : return the number of bytes required for a partition
//						: with a given configuration
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

m C_g03_partition_ctrl::mem_required(
						const S_g03_partition_config
								&p_config)
{
	m	ret_val_m;

	ret_val_m = p_config.m_num_elements_m * 
					true_size(rounded_size(p_config.m_size_of_element_m));

	return ret_val_m;
} // mem_required

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the partition number of an allocated element,
//						: G03_INVALID_PARTITION_W is returned if the element
//						: is not allocated or has been corrupted
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 18/06/96
///////////////////////////////////////////////////////////////////////////////

w C_g03_partition_ctrl::partition_num(void	*p_element_p)
{
	w	ret_val_w;
	S_alloc_sentinel	*alloc_sentinel_p;

	alloc_sentinel_p =  ((S_alloc_sentinel *) p_element_p) - 1;

	if (alloc_sentinel_p->m_magic_num_w != ALLOC_GUARD_NUM_W)
	{
		ret_val_w = G03_INVALID_PARTITION_W;
	}
	else
	{
		ret_val_w = alloc_sentinel_p->m_partition_num_w;
	}

	return ret_val_w;
} // partition_num

///////////////////////////////////////////////////////////////////////////////
// Function Description : Initialise a partition control object
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_partition_ctrl::init(
						const S_g03_partition_config
								&p_config,
						w		p_partition_num_w,
						m		p_partition_offset_m)
{
	m_partition_num_w		= p_partition_num_w;
//
// Round element size up to a multiple of 4 bytes
//
	m_size_of_element_m		= rounded_size(p_config.m_size_of_element_m);
	m_num_elements_m		= p_config.m_num_elements_m;
	m_partition_offset_m	= p_partition_offset_m;
	m_highest_used_m		= 0;
	m_free_index_m			= NULL_IDX_M;
} // init

//////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate an element (block of memory) from the
//						: partition. The base address is the base address
//						: to which the partition offset is applied
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 18/06/96
///////////////////////////////////////////////////////////////////////////////

void *C_g03_partition_ctrl::alloc_element(void	*p_base_addr_p)
{
	void				*ret_val_p;
	S_free_sentinel		*free_sentinel_p;
	S_alloc_sentinel	*alloc_sentinel_p;
	S_tail_sentinel		*tail_sentinel_p;
	m					true_size_m;

	ret_val_p = NULL;
	true_size_m = true_size(m_size_of_element_m);
//
// If the free list is empty get the next unused element
//
	if (m_free_index_m == NULL_IDX_M && m_highest_used_m < m_num_elements_m)
	{
		m_free_index_m = m_highest_used_m;

		free_sentinel_p = (S_free_sentinel *)
						element(p_base_addr_p, m_free_index_m);
		tail_sentinel_p = ((S_tail_sentinel *) (((b*) free_sentinel_p) +
												 true_size_m)) - 1;

		free_sentinel_p->m_magic_num_m = FREE_GUARD_NUM_M;
		free_sentinel_p->m_next_idx_m  = NULL_IDX_M;
		tail_sentinel_p->m_magic_num_m = TAIL_GUARD_NUM_M;

		m_highest_used_m++;
	}
//
// If there is an element to allocate then allocate it
//
	if (m_free_index_m != NULL_IDX_M)
	{
		free_sentinel_p = (S_free_sentinel *)
						element(p_base_addr_p, m_free_index_m);
		tail_sentinel_p = ((S_tail_sentinel *) (((b*) free_sentinel_p) +
												 true_size_m)) - 1;
//
// Check that the sentinel has not been corrupted
//
		if (free_sentinel_p->m_magic_num_m != FREE_GUARD_NUM_M ||
			tail_sentinel_p->m_magic_num_m != TAIL_GUARD_NUM_M)
		{
//
// Corrupt element, log error
//
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_PLATFORM_FATAL,
				G01_SE_G03_CODE_BASE_W + PARTITION_CODE_BASE_W + 1,
				CString(_TEXT("C_g03_partition_ctrl::alloc_element: corruption")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Remove element from the free list
//
			m_free_index_m = free_sentinel_p->m_next_idx_m;
//
// Allocated element follows allocated sentinel
//
			alloc_sentinel_p = (S_alloc_sentinel *) free_sentinel_p;
			alloc_sentinel_p->m_magic_num_w = ALLOC_GUARD_NUM_W;
			alloc_sentinel_p->m_partition_num_w = m_partition_num_w;
			tail_sentinel_p->m_magic_num_m = TAIL_GUARD_NUM_M;

			ret_val_p = (void *) (alloc_sentinel_p + 1);
		}
	}

	return ret_val_p;
} // alloc_element

//////////////////////////////////////////////////////////////////////////////
// Function Description : Release an element back to the partition
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_partition_ctrl::free_element(
						void	*p_base_addr_p,
						void	*p_element_p)
{
	S_free_sentinel	*free_sentinel_p;
	S_tail_sentinel	*tail_sentinel_p;
	m				index_m;
	m				element_offset_m;
	b				*partition_bp;
	m				true_size_m;

	true_size_m = true_size(m_size_of_element_m);
//
// Calculate address of partition
//
	partition_bp = (b *)p_base_addr_p + m_partition_offset_m;
//
// Calculate address of the element's sentinels.
// The alloc sentinel will have been tested for corruption already
//
	free_sentinel_p = (S_free_sentinel *)
						(((S_alloc_sentinel *) p_element_p) - 1);
	tail_sentinel_p  =  ((S_tail_sentinel *) (((b*) free_sentinel_p) +
												true_size_m)) - 1;
//
// Check the tail sentinel
//
	if (tail_sentinel_p->m_magic_num_m != TAIL_GUARD_NUM_M)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_PLATFORM_FATAL,
			G01_SE_G03_CODE_BASE_W + PARTITION_CODE_BASE_W + 2,
			CString(_TEXT("C_g03_partition_ctrl::free_element: tail corrupt")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Calculate index of element
//
	element_offset_m = ((b *) free_sentinel_p) - partition_bp;

	index_m = element_offset_m / true_size_m;
//
// Validate element address
//
	if (index_m >= m_num_elements_m ||
		(b *) free_sentinel_p != partition_bp + index_m * true_size_m)
	{
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_PLATFORM_FATAL,
			G01_SE_G03_CODE_BASE_W + PARTITION_CODE_BASE_W + 3,
			CString(_TEXT("C_g03_partition_ctrl::free_element: bad address")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
//
// Element OK so return it to the free list
//
		free_sentinel_p->m_magic_num_m = FREE_GUARD_NUM_M;
		free_sentinel_p->m_next_idx_m  = m_free_index_m;
		m_free_index_m = index_m;
	}
} // free_element


//////////////////////////////////////////////////////////////////////////////
// Function Description : Return the address of an indexed element
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void *C_g03_partition_ctrl::element(
						void	*p_base_addr_p,
						m		p_index_m) const
{
	void	*ret_val_p;
	b		*partition_bp;
//
// Calculate address of partition
//
	partition_bp = (b *) p_base_addr_p + m_partition_offset_m;
//
// Add index * size of element to address of start of element
//
	ret_val_p = (void *) (partition_bp +
								(true_size(m_size_of_element_m) * p_index_m));

	return ret_val_p;
} // element

//////////////////////////////////////////////////////////////////////////////
// Function Description : Calculate the true size of an element
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

m true_size(m	p_element_size_m)
{
	m	true_element_size_m;

	true_element_size_m = p_element_size_m + sizeof(S_alloc_sentinel) +
											 sizeof(S_tail_sentinel);

	return true_element_size_m;
} // true_size

//////////////////////////////////////////////////////////////////////////////
// Function Description : Round an element size to up required multiple
// Author               : Duncan Olesen
// Date Created         : 23/08/96
// Date Last Modified   : 23/08/96
///////////////////////////////////////////////////////////////////////////////

m rounded_size(m	p_element_size_m)
{
	m	rounded_size_m;
//
// Round the size up to a multiple of four
//
	rounded_size_m = ((p_element_size_m + 3) / 4) * 4;

	return rounded_size_m;
} // rounded_size
