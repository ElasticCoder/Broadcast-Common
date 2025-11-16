//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : Radio Automation Workstation
// Subsystem   : GPI IDC
// File Name   : ioctrl_pc272e_types.h
// File Type   : C++ Header
// Description : Types needed to communicate with the
//			   : Amplicon PC272E Digital I/O card
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   13/11/96		Draft
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IOCTRL_PC272E_TYPES_H_
#define _IOCTRL_PC272E_TYPES_H_

///////////////////////////////////////////////////////////////////////////////
// Define constants
///////////////////////////////////////////////////////////////////////////////
//
// Number of I/O ports on the card
//
const b		NUM_PC272E_PORTS_B = 9;
//
// Device register numbers (offsets from base address)
//
const b		REG_PPI_X_COMMAND_B = 3;
const b		REG_PPI_Y_COMMAND_B = 11;
const b		REG_PPI_Z_COMMAND_B = 19;
//
// Command Register masks
//
const b		MODE_DEF_SELECT_B		= 0x80;	// Command Select for Mode Definition
const b		MODE_DEF_MODE_A0_B		= 0x00;	// Group A mode 0
const b		MODE_DEF_MODE_A1_B		= 0x20;	// Group A mode 1
const b		MODE_DEF_MODE_A2_B		= 0x60;	// Group A mode 2
const b		MODE_DEF_MODE_B0_B		= 0x00;	// Group B mode 0
const b		MODE_DEF_MODE_B1_B		= 0x04;	// Group B mode 1

const b		MODE_DEF_PORT_A_IN_B	= 0x10;	// Port A input
const b		MODE_DEF_PORT_A_OUT_B	= 0x00; // Port A output
const b		MODE_DEF_PORT_B_IN_B	= 0x02;	// Port B input
const b		MODE_DEF_PORT_B_OUT_B	= 0x00; // Port B output
const b		MODE_DEF_PORT_C_IN_B	= 0x09;	// Port C (lower & upper) input
const b		MODE_DEF_PORT_C_OUT_B	= 0x00; // Port C (lower & upper) output

#endif  _IOCTRL_PC272E_TYPES_H_

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
