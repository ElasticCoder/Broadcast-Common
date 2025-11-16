//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : GPI IDC
// File Name   : ioctrl.cpp
// File Type   : C++ Source
// Description : I/O Control Class for the generic Digital I/O card
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions

#include "ioctrl.h"

#include <g01_event_logging.h>


///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ioctrl constructor
// Author               : Duncan Olesen
// Date Created         : 12/11/96
///////////////////////////////////////////////////////////////////////////////

C_ioctrl::C_ioctrl(
			b		p_num_ports_b,
			const	tc	*p_device_name_tcp)
{
	m_num_ports_b = p_num_ports_b;
	m_card_configured_z = FALSE;

	if (p_device_name_tcp != NULL &&
		p_device_name_tcp[0] != _TEXT('\0') &&
		_tcslen(p_device_name_tcp) < MAX_DEVICE_NAME_LEN_M)
	{
		_tcscpy(m_device_name_tca, p_device_name_tcp);
	}
	else
	{
		m_device_name_tca[0] = _TEXT('\0');
	}

	m_port_config_p = NULL;

	if (p_num_ports_b != 0)
	{
//
// Allocate array of port configuration statii
//
		m_port_config_p = new S_GPI_port_config[m_num_ports_b];

		if (m_port_config_p == NULL)
		{
			g01_log_system_event(
					G01_SE_SOFTWARE_EXCEPTION,
					G01_SE_ERROR,
					G01_SE_G00_CODE_BASE_W + 1,
					CString("C_ioctrl::C_ioctrl: could not allocate array"),
					CString(""),
					G01_SE_RAISED );

			m_num_ports_b = 0;
		}
		else
		{
			b	index_b;
//
// Initialise port configuration statii (unconfigured)
//
			for (index_b = 0; index_b < m_num_ports_b; index_b++)
			{
				m_port_config_p[index_b].m_status	= IO_GPI_PORT_UNCONFIGURED;
				m_port_config_p[index_b].m_value_b	= 0;
			}
		} // allocated config array
	} // number of ports > 0
	else
	{
		g01_log_system_event(
				G01_SE_SOFTWARE_EXCEPTION,
				G01_SE_ERROR,
				G01_SE_G00_CODE_BASE_W + 2,
				CString("C_ioctrl::C_ioctrl: no ports"),
				CString(""),
				G01_SE_RAISED );
	} // number of ports = 0
} // C_ioctrl

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ioctrl destructor
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

C_ioctrl::~C_ioctrl()
{
	if (m_port_config_p != NULL)
	{
		delete [] m_port_config_p;
	}
} // ~C_ioctrl

///////////////////////////////////////////////////////////////////////////////
// Function Description : Records whether a port is input or output
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::config_port(
								const b		p_port_num_b,
								const z		p_input_z)
{
	T_GPI_Status	ret_val;

	if (p_port_num_b < m_num_ports_b)
	{
		if (p_input_z)
		{
			m_port_config_p[p_port_num_b].m_status = IO_GPI_INPUT_PORT;
		}
		else
		{
			m_port_config_p[p_port_num_b].m_status = IO_GPI_OUTPUT_PORT;
		}

		ret_val = IO_GPI_SUCCESS;
	}
	else
	{
		ret_val = IO_GPI_NOT_CONFIG;
	}

	return ret_val;
} // config_port

///////////////////////////////////////////////////////////////////////////////
// Function Description : Performs card specific configuration.
//						: The base class does nothing
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::config_card()
{
	T_GPI_Status	ret_val;

	if (m_num_ports_b > 0 && m_device_name_tca[0] != _TEXT('\0'))
	{
		m_card_configured_z = TRUE;
		ret_val = IO_GPI_SUCCESS;
	}
	else
	{
		ret_val = IO_GPI_NOT_CONFIG;
	}

	return ret_val;
} // config_card
	
///////////////////////////////////////////////////////////////////////////////
// Function Description : Reads the state of a specified line
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::read_line(
								const b		p_port_num_b,
								const b		p_line_num_b,
								z			&p_line_high_z)
{
	T_GPI_Status	ret_val;
	b				port_value_b;

	if (p_line_num_b < LINES_PER_PORT_B)
	{
		ret_val = read_port(p_port_num_b, port_value_b);

		if (ret_val == IO_GPI_SUCCESS)
		{
			p_line_high_z = ((port_value_b & (1 << p_line_num_b)) != 0);
		}
	} // valid parameters
	else
	{
		ret_val = IO_GPI_INVALID;
	} // invalid parameters

	return ret_val;
} // read_line

///////////////////////////////////////////////////////////////////////////////
// Function Description : Read the input value of a port
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::read_port(
								const b		p_port_num_b,
								b			&p_port_value_b)
{
	T_GPI_Status	ret_val = IO_GPI_FAILURE;
	HANDLE			device_h;

	if (m_card_configured_z)
	{
		if (p_port_num_b < m_num_ports_b &&
			m_port_config_p[p_port_num_b].m_status == IO_GPI_INPUT_PORT)
		{
//
// Open I/O device
//
	        device_h = CreateFile(
								m_device_name_tca,  // File name
								GENERIC_READ,     // Desired access
								FILE_SHARE_READ,  // Share mode
								NULL,             // Security attributes
								OPEN_EXISTING,    // Creation distribution
								0,                // Flags and attributes
								NULL);            // Template file
                
			if (device_h == INVALID_HANDLE_VALUE)
			{
				g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
									 G01_SE_ERROR,
									 G01_SE_G00_CODE_BASE_W + 3,
									 CString("C_ioctrl::read_port: device open failure"),
									 CString(""),
									 G01_SE_RAISED);

				ret_val = IO_GPI_FAILURE;
			}
			else
			{
//
// Read data value from port
//
				ret_val = device_read(
								device_h,
								p_port_num_b,
								p_port_value_b);
//
// If successful record the new port value
//
				if (ret_val == IO_GPI_SUCCESS)
				{
					m_port_config_p[p_port_num_b].m_value_b = p_port_value_b;
				}
//
// Close I/O device
//
		        if (!CloseHandle(device_h))
				{
					g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
										 G01_SE_WARNING,
										 G01_SE_G00_CODE_BASE_W + 4,
										 CString("C_ioctrl::read_port"),
										 CString("GPI device close failure"),
										 G01_SE_RAISED);
				}
			} // handle OK
		} // valid parameters
	} // card configured
	else
	{
		ret_val = IO_GPI_NOT_CONFIG;
	} // card not configured

	return ret_val;
} // read_port

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write to a specified line
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::write_line(
								const b		p_port_num_b,
								const b		p_line_num_b,
								const z		p_line_high_z)
{
	T_GPI_Status	ret_val;
	b				port_value_b;

	if (m_card_configured_z)
	{
		if (p_port_num_b < m_num_ports_b &&
			m_port_config_p[p_port_num_b].m_status == IO_GPI_OUTPUT_PORT &&
			p_line_num_b < LINES_PER_PORT_B)
		{
			port_value_b = m_port_config_p[p_port_num_b].m_value_b;
			port_value_b &= ~(1 << p_line_num_b);
			port_value_b |= (!!p_line_high_z) << p_line_num_b;

			ret_val = write_port(p_port_num_b, port_value_b);
		} // valid parameters
		else
		{
			ret_val = IO_GPI_INVALID;
		} // invalid parameters
	} // card configured
	else
	{
		ret_val = IO_GPI_NOT_CONFIG;
	} // card not configured

	return ret_val;
} // write_line

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write a value to a port
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl::write_port(
								const b		p_port_num_b,
								const b		p_port_value_b)
{
	T_GPI_Status	ret_val = IO_GPI_FAILURE;
	HANDLE			device_h;

	if (m_card_configured_z)
	{
		if (p_port_num_b < m_num_ports_b &&
			m_port_config_p[p_port_num_b].m_status == IO_GPI_OUTPUT_PORT)
		{
//
// Open I/O device
//
	        device_h = CreateFile(
								  m_device_name_tca, // File name
                                  GENERIC_WRITE,     // Desired access
                                  FILE_SHARE_WRITE,  // Share mode
                                  NULL,              // Security attributes
                                  OPEN_EXISTING,     // Creation distribution
                                  0,                 // Flags and attributes
                                  NULL);             // Template file
                
			if (device_h == INVALID_HANDLE_VALUE)
			{
				g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
									 G01_SE_ERROR,
									 G01_SE_G00_CODE_BASE_W + 5,
									 CString("C_ioctrl::write_port: device open failure"),
									 CString(""),
									 G01_SE_RAISED);

				ret_val = IO_GPI_FAILURE;
			}
			else
			{
//
// Write data value to port
//
				ret_val = device_write(
								device_h,
								p_port_num_b,
								p_port_value_b);
//
// If successful record the new port value
//
				if (ret_val == IO_GPI_SUCCESS)
				{
					m_port_config_p[p_port_num_b].m_value_b = p_port_value_b;
				}
//
// Close I/O device
//
		        if (!CloseHandle(device_h))
				{
					g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
										 G01_SE_WARNING,
										 G01_SE_G00_CODE_BASE_W + 6,
										 CString("C_ioctrl::write_port"),
										 CString("GPI device close failure"),
										 G01_SE_RAISED);
				}
			} // handle OK
		} // valid parameters
	} // card configured
	else
	{
		ret_val = IO_GPI_NOT_CONFIG;
	} // card not configured

	return ret_val;
} // write_port

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the offset from the base address of a port
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

w C_ioctrl::port_offset(w	p_port_num_w)
{
	return p_port_num_w;
} // port_offset

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return card configuration details
// Author               : Duncan Olesen
// Date Created         : 15/11/96
// Date Last Modified   : 15/11/96
///////////////////////////////////////////////////////////////////////////////

S_GPI_port_config const *C_ioctrl::card_config(b	&p_num_ports_b) const
{
	p_num_ports_b	= m_num_ports_b;
	return m_port_config_p;
	//p_port_config_p	= &m_port_config_p;
} // card_config

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
