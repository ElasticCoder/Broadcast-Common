//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : GPI IDC
// File Name   : ioctrl_pc272e.cpp
// File Type   : C++ Source
// Description : I/O Control Class for the Amplicon PC272E Digital I/O card
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions

#include "ioctrl_pc272e.h"
#include "ioctrl_pc272e_types.h"

#include <g01_event_logging.h>
#include <devioctl.h>  // Device driver I/O control definitions

///////////////////////////////////////////////////////////////////////////////
// Define constants
///////////////////////////////////////////////////////////////////////////////
// I/O control values
const int	GPI_TYPE=40000;

#define IOCTL_GPI_READ_PORT CTL_CODE(GPI_TYPE,          \
                                     0x900,             \
                                     METHOD_BUFFERED,   \
                                     FILE_READ_ACCESS)

#define IOCTL_GPI_WRITE_PORT CTL_CODE(GPI_TYPE,           \
                                      0x910,              \
                                      METHOD_BUFFERED,    \
                                      FILE_WRITE_ACCESS)

///////////////////////////////////////////////////////////////////////////////
// Define structures
///////////////////////////////////////////////////////////////////////////////
struct S_GPI_write_buffer
{
    b	m_register_num_b;	// Card register number
    b	m_register_value_b;	// Data value
};

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ioctrl constructor
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

C_ioctrl_pc272e::C_ioctrl_pc272e()
: C_ioctrl(NUM_PC272E_PORTS_B, _T("\\\\.\\GpiDev"))
{
} // C_ioctrl_pc272e

///////////////////////////////////////////////////////////////////////////////
// Function Description : C_ioctrl destructor
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

C_ioctrl_pc272e::~C_ioctrl_pc272e()
{
} // ~C_ioctrl_pc272e

///////////////////////////////////////////////////////////////////////////////
// Function Description : Configure the card
// Author               : Duncan Olesen
// Date Created         : 13/11/96
// Date Last Modified   : 13/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl_pc272e::config_card()
{
	T_GPI_Status	ret_val;
	HANDLE			device_h;
//
// Open a device handle for writing to the card
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
		CString		temp;

		temp.Format(_TEXT("device open failure, error code = 0x%08X"),
							GetLastError());

		g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
							 G01_SE_ERROR,
							 G01_SE_G00_CODE_BASE_W + 1,
							 CString(_TEXT("C_ioctrl_pc272e::config_card")),
							 temp,
							 G01_SE_RAISED);

		ret_val = IO_GPI_FAILURE;
	}
	else
	{
//
// Configure PPI-X data registers
//
		ret_val = configure_cluster(
					device_h,
					REG_PPI_X_COMMAND_B,
					m_port_config_p[0].m_status,
					m_port_config_p[1].m_status,
					m_port_config_p[2].m_status);

		if (ret_val == IO_GPI_SUCCESS)
		{
//
// Configure PPI-Y data registers
//
			ret_val = configure_cluster(
						device_h,
						REG_PPI_Y_COMMAND_B,
						m_port_config_p[3].m_status,
						m_port_config_p[4].m_status,
						m_port_config_p[5].m_status);
		}

		if (ret_val == IO_GPI_SUCCESS)
		{
//
// Configure PPI-Z data registers
//
			ret_val = configure_cluster(
						device_h,
						REG_PPI_Z_COMMAND_B,
						m_port_config_p[6].m_status,
						m_port_config_p[7].m_status,
						m_port_config_p[8].m_status);
		}

		if (ret_val == IO_GPI_SUCCESS)
		{
			m_card_configured_z = TRUE;
		}

		if (!CloseHandle(device_h))
		{
			g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
								 G01_SE_WARNING,
								 G01_SE_G00_CODE_BASE_W + 2,
								 CString(_TEXT("C_ioctrl_pc272e::config_card")),
								 CString(_TEXT("device close failure")),
								 G01_SE_RAISED);
		}
	} // device opened OK

	return ret_val;
} // config_card

///////////////////////////////////////////////////////////////////////////////
// Function Description : Read the input value of a port on a device
// Author               : Duncan Olesen
// Date Created         : 13/11/96
// Date Last Modified   : 13/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl_pc272e::device_read(
								HANDLE		p_device_h,
								const b		p_port_num_b,
								b			&p_port_value_b)
{
	T_GPI_Status	ret_val;
	z				ok_z;
	b				register_num_b;
	m				byte_count_m;

	register_num_b = (b) port_offset((w) p_port_num_b);

	ok_z = DeviceIoControl(
					p_device_h,					// Device handle
					(DWORD)IOCTL_GPI_READ_PORT,	// IO control code
					(LPVOID)&register_num_b,	// Input buffer
					sizeof(register_num_b),		// Input buffer size
					(LPVOID)&p_port_value_b,	// Output buffer
					sizeof(p_port_value_b),		// Output buffer size
					&byte_count_m,				// Bytes returned
					NULL);						// Overlapped I/O data

	if (!ok_z)
	{
		CString		temp;

		temp.Format(_TEXT("Error code = 0x%08X"),
							GetLastError());

		g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
							 G01_SE_ERROR,
							 G01_SE_G00_CODE_BASE_W + 3,
							 CString(_T("C_ioctrl_pc272e::device_read: ")
									 _T("GPI port read failure")),
							 temp,
							 G01_SE_RAISED);

		ret_val = IO_GPI_FAILURE;
    }
    else if (byte_count_m != sizeof(p_port_value_b))
    {
		CString		temp;

        temp.Format(_TEXT("Device Name %s, Port = %d, Byte count = %d"),
							m_device_name_tca,
                            p_port_num_b,
                            byte_count_m);

        g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
                             G01_SE_ERROR,
                             G01_SE_G00_CODE_BASE_W + 4,
                             CString(_TEXT("C_ioctrl_pc272e::device_read: bytes read error")),
                             temp,
                             G01_SE_RAISED);

        ret_val = IO_GPI_FAILURE;
    }
    else
    {
        ret_val = IO_GPI_SUCCESS;
    }

	return ret_val;
} // device_read

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write a value to a port on a device
// Author               : Duncan Olesen
// Date Created         : 13/11/96
// Date Last Modified   : 13/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl_pc272e::device_write(
								HANDLE		p_device_h,
								const b		p_port_num_b,
								const b		p_port_value_b)
{
	T_GPI_Status	ret_val;
	b				register_num_b;

	register_num_b = (b) port_offset((w) p_port_num_b);
	
	ret_val = write_register(
					p_device_h,
					register_num_b,
					p_port_value_b);

	return ret_val;
} // device_write

///////////////////////////////////////////////////////////////////////////////
// Function Description : Return the offset from the base address of a port
// Author               : Duncan Olesen
// Date Created         : 12/11/96
// Date Last Modified   : 12/11/96
///////////////////////////////////////////////////////////////////////////////

w	C_ioctrl_pc272e::port_offset(w	p_port_num_w)
{
	w	ret_val_w;
//
// The nine ports are arranged as below
//		Port	Name			Offset
//		0		PPI-X Data A	0
//		1		PPI-X Data B	1
//		2		PPI-X Data C	2	
//		3		PPI-Y Data A	8
//		4		PPI-Y Data B	9
//		5		PPI-Y Data C	10	
//		6		PPI-Z Data A	16
//		7		PPI-Z Data B	17
//		8		PPI-Z Data C	18
//
	if (p_port_num_w < 3)
	{
		ret_val_w = p_port_num_w;
	}
	else if (p_port_num_w < 6)
	{
		ret_val_w = static_cast<w>(p_port_num_w + 5);
	}
	else
	{
		ret_val_w = static_cast<w>(p_port_num_w + 10);
	}

	return ret_val_w;
} // port_offset

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write a value to a register on the device
// Author               : Duncan Olesen
// Date Created         : 13/11/96
// Date Last Modified   : 13/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl_pc272e::write_register(
								HANDLE		p_device_h,
								const b		p_register_num_b,
								const b		p_register_value_b)
{
	T_GPI_Status		ret_val;
    m					byte_count_m;
    S_GPI_write_buffer	write_buffer;
	z				ok_z;

// Write data value to port
    write_buffer.m_register_num_b	= p_register_num_b;
    write_buffer.m_register_value_b	= p_register_value_b;

	ok_z = DeviceIoControl(
					p_device_h,						// Device handle
					(DWORD)IOCTL_GPI_WRITE_PORT,	// IO control code
					(LPVOID)&write_buffer,			// Input buffer
					sizeof(write_buffer),			// Input buffer size
					NULL,							// Output buffer
					0,								// Output buffer size
					&byte_count_m,					// Bytes returned
					NULL);							// Overlapped I/O data

	if (!ok_z)
	{
		CString		temp;

		temp.Format(_TEXT("Error code = 0x%08X"),
							GetLastError());

		g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
							 G01_SE_ERROR,
							 G01_SE_G00_CODE_BASE_W + 5,
							 CString("C_ioctrl_pc272e::write_register: write failure"),
							 temp,
							 G01_SE_RAISED);

		ret_val = IO_GPI_FAILURE;
    }
    else if (byte_count_m != 0)
    {
		CString		temp;

        temp.Format(_TEXT("Device Name %s, Register = %0x%02X, Byte count = %d"),
							m_device_name_tca,
                            p_register_num_b,
                            byte_count_m);

        g01_log_system_event(G01_SE_SOFTWARE_EXCEPTION,
                             G01_SE_ERROR,
                             G01_SE_G00_CODE_BASE_W + 6,
                             CString(_TEXT("C_ioctrl_pc272e::write_register: bytes returned error")),
                             temp,
                             G01_SE_RAISED);

        ret_val = IO_GPI_FAILURE;
    }
    else
    {
        ret_val = IO_GPI_SUCCESS;
    }

	return ret_val;
} // write_register

///////////////////////////////////////////////////////////////////////////////
// Function Description : Configure a register cluster
// Author               : Duncan Olesen
// Date Created         : 13/11/96
// Date Last Modified   : 13/11/96
///////////////////////////////////////////////////////////////////////////////

T_GPI_Status C_ioctrl_pc272e::configure_cluster(
								HANDLE						p_device_h,
								const b						p_control_reg_num_b,
								T_GPI_port_config_status	p_port_a_config,
								T_GPI_port_config_status	p_port_b_config,
								T_GPI_port_config_status	p_port_c_config)
{
	T_GPI_Status	ret_val;
	b				command_word_b;
//
// Set command to define Group A mode 0 and Group B mode 0
//
	command_word_b = MODE_DEF_SELECT_B | MODE_DEF_MODE_A0_B | MODE_DEF_MODE_B0_B;
//
// Set command to configure ports for input or output.
// Unconfigured ports are set to input
//
// Port A
//
	if (p_port_a_config == IO_GPI_OUTPUT_PORT)
	{
		command_word_b |= MODE_DEF_PORT_A_OUT_B;
	}
	else
	{
		command_word_b |= MODE_DEF_PORT_A_IN_B;
	}
//
// Port B
//
	if (p_port_b_config == IO_GPI_OUTPUT_PORT)
	{
		command_word_b |= MODE_DEF_PORT_B_OUT_B;
	}
	else
	{
		command_word_b |= MODE_DEF_PORT_B_IN_B;
	}
//
// Port C
//
	if (p_port_c_config == IO_GPI_OUTPUT_PORT)
	{
		command_word_b |= MODE_DEF_PORT_C_OUT_B;
	}
	else
	{
		command_word_b |= MODE_DEF_PORT_C_IN_B;
	}
//
// Write command to the control register to configure the register cluster
//
	ret_val = write_register(
					p_device_h,
					p_control_reg_num_b,
					command_word_b);

	return ret_val;
} // configure_cluster

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
