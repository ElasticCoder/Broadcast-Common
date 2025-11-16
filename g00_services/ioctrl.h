//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : GPI IDC
// File Name   : ioctrl.h
// File Type   : C++ Header
// Description : I/O Control Class for the generic Digital I/O card
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IOCTRL_H_
#define _IOCTRL_H_

///////////////////////////////////////////////////////////////////////////////
// Define constants
///////////////////////////////////////////////////////////////////////////////
const b		LINES_PER_PORT_B = 8;

///////////////////////////////////////////////////////////////////////////////
// Define enumerations
///////////////////////////////////////////////////////////////////////////////
enum T_GPI_Status
{
    IO_GPI_SUCCESS,
    IO_GPI_NOT_CONFIG,
	IO_GPI_INVALID,
    IO_GPI_FAILURE
};

enum T_GPI_port_config_status
{
    IO_GPI_PORT_UNCONFIGURED,
	IO_GPI_INPUT_PORT,
	IO_GPI_OUTPUT_PORT
};

///////////////////////////////////////////////////////////////////////////////
// Define structures
///////////////////////////////////////////////////////////////////////////////
struct S_GPI_port_config
{
	T_GPI_port_config_status	m_status;	// configuration status
	b							m_value_b;	// port (read/write) value
};

const m		MAX_DEVICE_NAME_LEN_M = 128; // temporary definition

class C_ioctrl
{
public:
	C_ioctrl(b			p_num_ports_b,
			 const	tc	*p_device_name_tcp);
	virtual ~C_ioctrl();

protected:
	// array of statii, one per port
	S_GPI_port_config
			*m_port_config_p;
	z		m_card_configured_z;
	tc		m_device_name_tca[MAX_DEVICE_NAME_LEN_M + 1];

private:
	b		m_num_ports_b;

// Implementation
public:
	//
	// config_port
	// Function:
	//	Records whether a port is input or output. Depending on the
	//	card this may be reflected on the card by this method or by
	//	the config_card method.
	// Parameters:
	//	p_port_num_b		The number of the port to configure
	//	p_input_z			TRUE indicates that the port is input, FALSE output
	//
	virtual T_GPI_Status	config_port(
								const b		p_port_num_b,
								const z		p_input_z);
	//
	// config_card
	// Function:
	//	Depending on the card this will configure the card with the port
	//	settings recorded via config_port and any other card specific settings.
	//
	virtual T_GPI_Status	config_card();
	//
	// read_line
	// Function:
	//	Reads the state of a specified line
	// Parameters:
	//	p_port_num_b		The number of the port to read
	//	p_line_num_b		The number of the line on the port to read
	//	p_line_high_z		Set to TRUE if input line is high, FALSE if line is low
	//
	T_GPI_Status			read_line(
								const b		p_port_num_b,
								const b		p_line_num_b,
								z			&p_line_high_z);
	//
	// read_port
	// Function:
	//	Read the input value of a port
	// Parameters:
	//	p_port_num_b		The number of the port to read
	//	p_port_value_b		The value read from the port
	//
	T_GPI_Status			read_port(
								const b		p_port_num_b,
								b			&p_port_value_b);
	//
	// device_read
	// Function:
	//	Read the input value of a port on a device
	// Parameters:
	//	p_device_h			Handle of the device
	//	p_port_num_b		The number of the port to read
	//	p_port_value_b		The value read from the port
	//
	virtual T_GPI_Status	device_read(
								HANDLE		p_device_h,
								const b		p_port_num_b,
								b			&p_port_value_b) = 0;
	//
	// write_line
	// Function:
	//	Writes the state of a specified line
	// Parameters:
	//	p_port_num_b		The number of the port to read
	//	p_line_num_b		The number of the line on the port to read
	//	p_line_high_z		TRUE - output input line is set high,
	//						FALSE - output line line is set low
	//
	T_GPI_Status			write_line(
								const b		p_port_num_b,
								const b		p_line_num_b,
								const z		p_line_high_z);
	//
	// write_port
	// Function:
	//	Write a value to a port
	// Parameters:
	//	p_port_num_b		The number of the port to write
	//	p_port_value_b		Value to write to the port
	//
	T_GPI_Status			write_port(
								const b		p_port_num_b,
								const b		p_port_value_b);
	//
	// device_write
	// Function:
	//	Write a value to a port on a device
	// Parameters:
	//	p_device_h			Handle of the device
	//	p_port_num_b		The number of the port to write
	//	p_port_value_b		Value to write to the port
	//
	virtual T_GPI_Status	device_write(
								HANDLE		p_device_h,
								const b		p_port_num_b,
								const b		p_port_value_b) = 0;
	//
	// Return the offset from the base address of a port
	//
	virtual	w	port_offset(w	p_port_num_w);
	//
	// card_config
	// Function:
	//	Returns a pointer to the port configuration array and the number
	//	of ports
	//
	S_GPI_port_config const	*card_config(
						b					&p_num_ports_b) const;
};

#endif  _IOCTRL_H_

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
