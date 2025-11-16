///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Services
// File Name   : g03_shared_memory.cpp
// File Type   : C++ Source
// Description : Memory Shared By Several Processes Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g01_event_logging.h>

#include <g03_shared_memory.h>

#include "g03_event_codes.h"

static const tc	v_no_name_tca[] = _TEXT("");

struct T_shared_mem_ctrl
{
	tc		m_name_tca[G03_SHARED_MEM_NAME_LEN_W + 1];
	m		m_size_m;
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_shared_memory constructor which does not attach to
//						: shared memory
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_shared_memory::C_g03_shared_memory()
{
	m_file_mapping_h	= NULL;
	m_memory_address_p	= NULL;
	m_size_m			= 0;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_shared_memory constructor which attaches to
//						: shared memory
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_shared_memory::C_g03_shared_memory(const tc	*p_memory_name_tcp)
{
	m_file_mapping_h   = NULL;
	m_memory_address_p = NULL;

	attach(p_memory_name_tcp);
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_shared_memory destructor
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

C_g03_shared_memory::~C_g03_shared_memory()
{
//
// Unmap the view of the shared memory
//
	if (m_memory_address_p != NULL)
	{
		UnmapViewOfFile((LPVOID) (((T_shared_mem_ctrl *) m_memory_address_p) - 1));
	}
//
// Close the file mapping handle
//
	if (m_file_mapping_h != NULL)
	{
		CloseHandle(m_file_mapping_h);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Configure the shared memory
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

void	*C_g03_shared_memory::configure(
					const tc	*p_memory_name_tcp,
					m			p_size_m)
{
	T_shared_mem_ctrl	*memory_ctrl_p;
	m					map_size_m;
  	void				*ret_val_p;

	ret_val_p = NULL;
	map_size_m = p_size_m + sizeof(T_shared_mem_ctrl);
//
// Validate parameters
//
	if (p_memory_name_tcp == NULL ||
		_tcslen(p_memory_name_tcp) == 0 ||
		_tcslen(p_memory_name_tcp) > G03_SHARED_MEM_NAME_LEN_W)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 1,
			CString(_TEXT("C_g03_shared_memory::configure: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else if (m_memory_address_p != NULL)
	{
//
// Configuration only permitted on object created using default constructor
// and has not yet been configured
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 2,
			CString(_TEXT("C_g03_shared_memory::configure: object already configured")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
//
// Create a file mapping
//
		m_file_mapping_h = CreateFileMapping(
					INVALID_HANDLE_VALUE,	// use paging file
					NULL,					// no security attributes
					PAGE_READWRITE | SEC_COMMIT,	// protection
					0,						// bits 32-63 of max size
					map_size_m,				// bits  0-31 of max size
					p_memory_name_tcp);
//
// Check that file mapping was successful
//
		if (m_file_mapping_h == NULL)
		{
			CString temp;

			temp.Format(_TEXT("Error = %d"), GetLastError());

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_APPLICATION_FATAL,
				G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 3,
				CString(_TEXT("C_g03_shared_memory::configure: could not create mapping")),
				CString(temp),
				G01_SE_RAISED);
		}
		else if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
//
// The mapped file already exists
//
			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_APPLICATION_FATAL,
				G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 4,
				CString(_TEXT("C_g03_shared_memory::configure: memory already configured")),
				CString(_TEXT("")),
				G01_SE_RAISED);
		}
		else
		{
//
// Map a view of the file mapping
//
			memory_ctrl_p = (T_shared_mem_ctrl *) MapViewOfFile(
					m_file_mapping_h,
					FILE_MAP_ALL_ACCESS,
					0,						// bits 32-63 of offset
					0,						// bits  0-31 of offset
					map_size_m);			// size of view
//
// Validate the view
//
			if (memory_ctrl_p == NULL)
			{
//
// Could not map the view of the shared memory
//
				CString temp;

				temp.Format(_TEXT("Error = %d"), GetLastError());

				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_APPLICATION_FATAL,
					G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 5,
					CString(_TEXT("C_g03_shared_memory::configure: map view error")),
					CString(temp),
					G01_SE_RAISED);
			}
			else
			{
//
// Record memory name and size of shared memory at start of view
//
				_tcscpy(memory_ctrl_p->m_name_tca, p_memory_name_tcp);
				memory_ctrl_p->m_size_m = p_size_m;
//
// Usable shared memory is after the header initialised above
//
				m_memory_address_p	= memory_ctrl_p + 1;
				m_size_m			= p_size_m;

				ret_val_p = m_memory_address_p;
			} // valid view
		} // valid mapping of file
	} // valid parameters & usage of the method

	return ret_val_p;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Attach to the shared memory
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

void C_g03_shared_memory::attach(const tc	*p_memory_name_tcp)
{
	T_shared_mem_ctrl	*memory_ctrl_p;
	m					map_size_m;
//
// Validate parameter
//
	if (p_memory_name_tcp == NULL ||
		_tcslen(p_memory_name_tcp) == 0)
	{
//
// Invalid parameter
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_APPLICATION_FATAL,
			G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 6,
			CString(_TEXT("C_g03_shared_memory::attach: invalid parameter")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
//
// Validate usage
//
	if (m_memory_address_p != NULL)
	{
//
// Invalid usage of the method
//
		g01_log_system_event(
			G01_SE_SOFTWARE_EXCEPTION,
			G01_SE_ERROR,
			G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 7,
			CString(_TEXT("C_g03_shared_memory::attach: already attached")),
			CString(_TEXT("")),
			G01_SE_RAISED);
	}
	else
	{
//
// Open mapping
//
		m_file_mapping_h = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,	// use paging file
				FALSE,					// no inheritance
				p_memory_name_tcp);

		if (m_file_mapping_h == NULL)
		{
//
// Could not open mapping
//
			CString temp;

			temp.Format(_TEXT("Error = %d"), GetLastError());

			g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_APPLICATION_FATAL,
				G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 8,
				CString(_TEXT("C_g03_shared_memory::attach: could not open file mapping")),
				CString(temp),
				G01_SE_RAISED);
		}
		else
		{
//
// Map a view of the file mapping to determine its size
//
			memory_ctrl_p = (T_shared_mem_ctrl *) MapViewOfFile(
					m_file_mapping_h,
					FILE_MAP_ALL_ACCESS,
					0,						// bits 32-63 of offset
					0,						// bits  0-31 of offset
					sizeof(T_shared_mem_ctrl));	// size of view
//
// Validate the view
//
			if (memory_ctrl_p == NULL)
			{
//
// Could not map a view
//
				CString temp;

				temp.Format(_TEXT("Error = %d"), GetLastError());

				g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_APPLICATION_FATAL,
					G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 9,
					CString(_TEXT("C_g03_shared_memory::attach: invalid view")),
					CString(temp),
					G01_SE_RAISED);
			}
			else
			{
				map_size_m = memory_ctrl_p->m_size_m +
								sizeof(T_shared_mem_ctrl);
//
// Unmap the view of the shared memory size prior to mapping a
// view of the entire shared memory
//
				UnmapViewOfFile((LPVOID) memory_ctrl_p);
//
// Map view of entire shared memory
//
				memory_ctrl_p = (T_shared_mem_ctrl *) MapViewOfFile(
						m_file_mapping_h,
						FILE_MAP_ALL_ACCESS,
						0,						// bits 32-63 of offset
						0,						// bits  0-31 of offset
						map_size_m);			// size of view
//
// Validate the view
//
				if (memory_ctrl_p == NULL)
				{
//
// Could not remap the view of the shared memory
//
					CString temp;

					temp.Format(_TEXT("Error = %d"), GetLastError());

					g01_log_system_event(
						G01_SE_SOFTWARE_EXCEPTION,
						G01_SE_APPLICATION_FATAL,
						G01_SE_G03_CODE_BASE_W + SHARED_MEMORY_CODE_BASE_W + 10,
						CString(_TEXT("C_g03_shared_memory::attach: remap error")),
						CString(temp),
						G01_SE_RAISED);
				}
				else
				{
					m_memory_address_p	= memory_ctrl_p + 1;
					m_size_m			= map_size_m - sizeof(T_shared_mem_ctrl);
				} // valid remap of view
			} // valid view
		} // valid file mapping
	} // valid parameter
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Convert address in shared memory into a process
//						: independent identifier
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

T_g03_memory_id	C_g03_shared_memory::memory_id(const void *p_mem_p) const
{
//
// Return the offset from the start of the shared memory
//
	T_g03_memory_id	ret_val;

	ret_val = G03_INVALID_ID;

	if (p_mem_p != NULL)
	{
		if (m_memory_address_p != NULL)
		{
//
// Check that address is in the shared memory
//
			if ((b *) p_mem_p >= (b *) m_memory_address_p &&
				(b *) p_mem_p < ((b *) m_memory_address_p) + m_size_m)
			{
				ret_val = (b *) p_mem_p - (b *) m_memory_address_p;
			}
		}
	}

	return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Convert a process independent identifier into an
//						: address
// Author               : Duncan Olesen
// Date Created         : 23/04/96
// Date Last Modified   : 23/04/96
///////////////////////////////////////////////////////////////////////////////

void *C_g03_shared_memory::address(T_g03_memory_id p_memory_id) const
{
	void	*ret_val_p;

	ret_val_p = NULL;

	if (p_memory_id != G03_INVALID_ID)
	{
		if (m_memory_address_p != NULL)
		{
//
// Check that offset is not larger than the shared memory
//
			if (p_memory_id < m_size_m)
			{
				ret_val_p = (void *) (((b *) m_memory_address_p) + p_memory_id);
			}
		}
	}

	return ret_val_p;
}

///////////////////////////////////////////////////////////////////////////////
// Function Description : Shared Memory name
// Author               : Duncan Olesen
// Date Created         : 25/04/96
// Date Last Modified   : 25/04/96
///////////////////////////////////////////////////////////////////////////////

const tc *C_g03_shared_memory::name() const
{
	const tc	*ret_val_tcp;

	if (m_memory_address_p != NULL)
	{
		ret_val_tcp = (((T_shared_mem_ctrl *) m_memory_address_p) - 1)->
								m_name_tca;
	}
	else
	{
		ret_val_tcp = v_no_name_tca;
	}

	return ret_val_tcp;
}
