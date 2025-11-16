//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Key And Voice-Over
// Subsystem   : GPI IDC
// File Name   : ioctrl_pc272e.h
// File Type   : C++ Header
// Description : I/O Control Class for the Amplicon PC272E Digital I/O card
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IOCTRL_PC272E_H_
#define _IOCTRL_PC272E_H_

#include "ioctrl.h"

class C_ioctrl_pc272e : public C_ioctrl
{
public:
	C_ioctrl_pc272e();
	~C_ioctrl_pc272e();

// Implementation
public:
	//
	// config_card
	// Function:
	//	Depending on the card this will configure the card with the port
	//	settings recorded via config_port and any other card specific settings.
	//
	virtual T_GPI_Status	config_card();
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
								b			&p_port_value_b);
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
								const b		p_port_value_b);
	//
	// Return the offset from the base address of a port
	//
	virtual	w	port_offset(w	p_port_num_w);

private:
	//
	// write_register
	// Function:
	//	Write a value to a register on the device
	// Parameters:
	//	p_device_h			Handle of the device
	//	p_register_num_b	The number of the register to write
	//	p_register_value_b	Value to write to the register
	//
	T_GPI_Status			write_register(
								HANDLE		p_device_h,
								const b		p_register_num_b,
								const b		p_register_value_b);
	//
	// configure_cluster
	// Function:
	//	Configure a cluster of ports for input/output.
	//	Ports not configured via config_port will default to input
	// Parameters:
	//	p_device_h			Handle of the device
	//	p_register_num_b	The number of the control register for the cluster
	//	p_port_a_config		Required configuration of port A in the cluster
	//	p_port_b_config		Required configuration of port B in the cluster
	//	p_port_c_config		Required configuration of port C in the cluster
	//
	T_GPI_Status			configure_cluster(
								HANDLE						p_device_h,
								const b						p_control_reg_num_b,
								T_GPI_port_config_status	p_port_a_config,
								T_GPI_port_config_status	p_port_b_config,
								T_GPI_port_config_status	p_port_c_config);
};

#endif  _IOCTRL_PC272E_H_

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
