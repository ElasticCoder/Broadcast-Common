///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g03_heap.cpp
// File Type   : C++ Header
// Description : Heap Shared By Several Processes Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>

#include <g03_heap.h>

#include "g03_event_codes.h"


const	m	HEAP_MAGIC_NUM_M = 0xABCD1234;

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_heap constructor
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_heap::C_g03_heap()
: m_shared_memory()
{
	m_heap_config_p			= NULL;
	m_partition_ctrl_p		= NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_heap constructor
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_heap::C_g03_heap(const tc	*p_heap_name_tcp)
: m_shared_memory()
{
//
// Initialise members
//
	m_heap_config_p			= NULL;
	m_partition_ctrl_p		= NULL;

	attach(p_heap_name_tcp);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_g03_heap destructor
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_heap::~C_g03_heap()
{
//
// Unmap the view of the heap
//
	if (m_heap_config_p != NULL)
	{
		m_heap_config_p->m_usage_count_w--;

		if (m_heap_config_p->m_usage_count_w == 0 &&
			m_heap_config_p->m_allocated_m != 0)
		{
			CString temp;

			temp.Format(_TEXT("heap = %s, bytes leaked = %u, buffers leaked = %u"),
						m_shared_memory.name(),
						m_heap_config_p->m_allocated_m,
						m_heap_config_p->m_buffers_m);

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 1,
				CString(_TEXT("C_g03_heap::~C_g03_heap: memory leakage")),
				temp,
				G01_SE_RAISED);
		}

		UnmapViewOfFile((LPVOID) m_heap_config_p);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : return the number of bytes required for a heap with
//						: a given configuration
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

m C_g03_heap::mem_required(
			w				p_num_partitions_w,
			const S_g03_partition_config
							*p_partition_conf_p)
{
	m	ret_val_m;
	w	count_w;

	ret_val_m = 0;
//
// Determine total size of partitions
//
	for (count_w = 0; count_w < p_num_partitions_w; count_w++)
	{
		ret_val_m += C_g03_partition_ctrl::mem_required(
							p_partition_conf_p[count_w]);
	}
//
// Add size of heap config and size of the partition control array
//
	ret_val_m += sizeof(S_heap_config) +
					p_num_partitions_w * sizeof(C_g03_partition_ctrl);

	return ret_val_m;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Configure the heap
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

z C_g03_heap::configure(
					const tc	*p_heap_name_tcp,
					w			p_num_partitions_w,
					const S_g03_partition_config
								*p_partition_conf_p)
{
	m		heap_size_m;
	w		count_w;
	m		last_size_m;
	m		partition_offset_m;
  	z		ret_val_z;

	ret_val_z = FALSE;
//
// Validate parameters
//
//
// Loop to ensure partitions are ordered by increasing element size
//
	count_w		= 0;
	last_size_m	= 0;
	while (count_w < p_num_partitions_w &&
		   p_partition_conf_p[count_w].m_size_of_element_m >
		   last_size_m)
	{
		last_size_m = p_partition_conf_p[count_w].m_size_of_element_m;
		count_w++;
	}

	if (p_heap_name_tcp == NULL ||
		_tcslen(p_heap_name_tcp) == 0 ||
		count_w != p_num_partitions_w)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 2,
			CString(_TEXT("C_g03_heap::configure: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (m_heap_config_p != NULL)
	{
//
// Configuration only permitted on object created using default constructor
// and has not yet been configured
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 3,
			CString(_TEXT("C_g03_heap::configure: object already configured")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
//
// Configure shared memory
//
		heap_size_m = mem_required(
							p_num_partitions_w,
							p_partition_conf_p);

		m_heap_config_p = (S_heap_config *) m_shared_memory.
							configure(
								p_heap_name_tcp,
								heap_size_m);

		if (m_heap_config_p == NULL)
		{
//
// Could not map the shared memory
//
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_APPLICATION_FATAL,
				G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 4,
				CString(_TEXT("C_g03_heap::configure: map shared memory error")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Record heap configuration at head of heap
//
			m_heap_config_p->m_magic_num_m = HEAP_MAGIC_NUM_M;
			m_heap_config_p->m_num_partitions_w = p_num_partitions_w;
			m_heap_config_p->m_heap_size_m = heap_size_m;
			m_heap_config_p->m_usage_count_w = 1;
			m_heap_config_p->m_allocated_m = 0;
			m_heap_config_p->m_buffers_m   = 0;

			m_partition_ctrl_p = (C_g03_partition_ctrl *)
						(m_heap_config_p + 1);
//
// First partition is stored after heap configuration and all
// the partition controllers
//
			partition_offset_m = sizeof(S_heap_config) +
						sizeof(C_g03_partition_ctrl) * p_num_partitions_w;
//
// Configure the heap partitions
//
			for (count_w = 0; count_w < p_num_partitions_w; count_w++)
			{
				m_partition_ctrl_p[count_w].init(
						p_partition_conf_p[count_w],
						count_w,
						partition_offset_m);
//
// Adjust partition offset to be past the end of this partition
//
				partition_offset_m += 
					C_g03_partition_ctrl::mem_required(
							p_partition_conf_p[count_w]);
			}

			ret_val_z = TRUE;
		} // valid mapping of shared memory
	} // valid parameters & usage of the method

	return ret_val_z;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Attach to a configured heap
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_heap::attach(const tc	*p_heap_name_tcp)
{
//
// Validate parameter
//
	if (p_heap_name_tcp == NULL ||
		_tcslen(p_heap_name_tcp) == 0)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 5,
			CString(_TEXT("C_g03_heap::attach: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Validate usage
//
	if (m_heap_config_p != NULL ||
		m_partition_ctrl_p != NULL)
	{
//
// Invalid usage of the method
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 6,
			CString(_TEXT("C_g03_heap::C_g03_heap: already attached")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
		m_shared_memory.attach(p_heap_name_tcp);

		m_heap_config_p	= (S_heap_config *) m_shared_memory.address();
//
// Validate the view
//
		if (m_heap_config_p == NULL ||
			m_heap_config_p->m_magic_num_m != HEAP_MAGIC_NUM_M)
		{
//
// Could not map a view, or view is not of a heap
//
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_APPLICATION_FATAL,
				G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 7,
				CString(_TEXT("C_g03_heap::C_g03_heap: invalid view")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
			m_partition_ctrl_p = (C_g03_partition_ctrl *) (m_heap_config_p + 1);
//
// Maintain usage count
//
			m_heap_config_p->m_usage_count_w++;
		} // valid mapping of shared memory
	} // valid parameter
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Allocate memory suitable for sharing between processes
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void *C_g03_heap::alloc(l p_size_l)
{
	void	*ret_val_p;
	w		partition_w;
	m		size_m;

	ret_val_p = NULL;

	if (p_size_l >= 0 && 
		m_heap_config_p != NULL &&
		m_partition_ctrl_p != NULL)
	{
		size_m = (m) p_size_l;
		partition_w = 0;
//
// A binary search is not necessary to find the appropriate partition from
// which to allocate an element because the number of partitions should be
// small
//
		while (ret_val_p == NULL &&
			   partition_w < m_heap_config_p->m_num_partitions_w)
		{
			if (m_partition_ctrl_p[partition_w].element_size() >= size_m)
			{
				ret_val_p = m_partition_ctrl_p[partition_w].
						alloc_element((void *) m_heap_config_p);
			}

			partition_w++;
		}
//
// Maintain count of memory allocated
//
		if (ret_val_p != NULL)
		{
			m_heap_config_p->m_allocated_m +=
					m_partition_ctrl_p[partition_w-1].element_size();
			m_heap_config_p->m_buffers_m++;
		}

	} // valid size

	return ret_val_p;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Free memory shared between processes
// Author               : Duncan Olesen
// Date Created         : 22/04/96
// Date Last Modified   : 22/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_heap::free(void *p_mem_p)
{
	if (p_mem_p != NULL)
	{
		if (m_heap_config_p != NULL &&
			m_partition_ctrl_p != NULL)
		{
//
// Check that address is in the heap
//
			if ((b *) p_mem_p > (b *) m_heap_config_p &&
				(b *) p_mem_p < ((b *) m_heap_config_p) +
									m_heap_config_p->m_heap_size_m)
			{
				w	partition_w;

				partition_w = C_g03_partition_ctrl::partition_num(p_mem_p);

				if (partition_w != G03_INVALID_PARTITION_W &&
					partition_w < m_heap_config_p->m_num_partitions_w)
				{
					m_partition_ctrl_p[partition_w].free_element(
								(void *) m_heap_config_p,
								p_mem_p);
//
// Maintain count of memory allocated
//
					m_heap_config_p->m_allocated_m -=
						m_partition_ctrl_p[partition_w].element_size();
					m_heap_config_p->m_buffers_m--;
				}
				else
				{
//
// Invalid memory, already released or sentinel corrupt
//
					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_APPLICATION_FATAL,
						G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 8,
						CString(_T("C_g03_heap::free: memory already ")
								_T("released or corrupted")),
						CString(_TEXT("")),
						G01_SE_RAISED);
				}
			} // address in heap
			else
			{
//
// Address not in this heap
//
				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_APPLICATION_FATAL,
					G01_SE_G03_CODE_BASE_W + HEAP_CODE_BASE_W + 9,
					CString(_TEXT("C_g03_heap::free: address not in heap")),
					CString(_TEXT("")),
					G01_SE_RAISED);
			}
		} // heap configured
	} // non null address
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return High Water Mark for all the partitions in the
//						: heap
// Author               : Duncan Olesen
// Date Created         : 03/06/96
// Date Last Modified   : 03/06/96
///////////////////////////////////////////////////////////////////////////////

S_g03_partition_config	*C_g03_heap::high_water() const
{
//
// The function returns a pointer to an array of records allocated from the
// default process heap. Each record specifies the size of blocks in the partition
// and the maximum number allocated simultaneously
//
	S_g03_partition_config	*ret_val;

	ret_val = NULL;

	if (m_heap_config_p != NULL &&
		m_partition_ctrl_p != NULL)
	{
		ret_val = new S_g03_partition_config
					[m_heap_config_p->m_num_partitions_w];

		if (ret_val != NULL)
		{
			for (int i = 0; i < m_heap_config_p->m_num_partitions_w; i++)
			{
				ret_val[i].m_size_of_element_m =
						m_partition_ctrl_p[i].element_size();

				ret_val[i].m_num_elements_m = 
						m_partition_ctrl_p[i].high_water_mark();
			} // for
		} // allocated return data
	} // heap configured

	return ret_val;
}
