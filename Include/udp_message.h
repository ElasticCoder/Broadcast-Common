///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : t05_udp_transmitter / Remote Playlist Monitor
// File Name   : udp_message.h
// File Type   : C++ header
// Description : Contains sizes and message position for broadcast messages
// Author      : Andy Hill
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _UDP_MESSAGES_H
#define _UDP_MESSAGES_H

#include <portable_types.h>		// Type definitions


const w MAX_SYSMODE_MSG_LEN = 2;
const w MAX_CARTSTAT_MSG_LEN = 9;
const w MAX_EVENT_MISM_LEN = 10;

const w NUM_STATUS_BYTES = 4;
const w NUM_CART_STATUS_BYTES = 8;

const i IDENT_BUF_LEN = 32;
const i IDENT_MSG_LEN = (IDENT_BUF_LEN * 2) + 1;

const i IDENT_DEL_MSG_LEN = 1;
const i PLAYLIST_SAVED_MSG_LEN = 1;
//
// Common Element Positions
//
const b CMD_STX				= 0;
const b CMD_BYTE_COUNT		= 1;
const b CMD_TYPE			= 2;
//
// Element positions in PLAY command message
//
const b CMD_UA_1			= 3;
const b CMD_UA_2			= 4;
const b CMD_START_OF_DATA	= 5;
const b CMD_FIRST_DATA_BYTE	= 6;
//
// Element positions in FLEX command message
//
const b CMD_START_OF_FLEX_DATA		= 4;
const b CMD_FIRST_FLEX_DATA_BYTE	= 5;

const b EVENT_IN_LOCKOUT	= 0x1;
const b TOP_RECORD_CHANGED  = 0x2;

enum CHECKSUM_OPTIONS
{
	NO_CHECKSUM = 0,
	CHECKSUM 
};

const CHECKSUM_OPTIONS CMD_CHKSUM_BYTES	= CHECKSUM;//Number of checksum bytes
//
// Event types
//
const b PLAY_EVENT_SS = 0;
const b BREAK_EVENT	  = 1;
const b LINE_EVENT	  = 2;
const b PLAY_EVENT_MS = 3;
const b SERVER_EVENT  = 4;
const b EMPTY_EVENT   = 5;
//===================
// List STATUS BIT POSITIONS
//===================
const b STAT_MATERIAL_ERROR = 4;
const b STAT_TITLE_MISM = 1;
//===================
// VSTS BIT POSITIONS
//===================
//=== 
const b VSTS0_VTR_LOCAL = 7;
const b VSTS0_LINE_EVENT = 6;
//=== 
const b VSTS1_ON_AIR = 7;
const b VSTS1_LOAD_ERR = 6;
const b VSTS1_HARD_ERR = 3;
const b VSTS1_COLOUR_FRAME_LOCK = 2;
const b VSTS1_SERVO_LOCK = 1;
const b VSTS1_NOT_CUED = 0;
//===
const b VSTS2_STILL		= 6;
const b VSTS2_PLAY		= 5;
const b VSTS2_CUED		= 3;
const b VSTS2_CUEING	= 2;
const b VSTS2_THREAD	= 1;
const b VSTS2_TAPE_OUT	= 0;
//===
const b VSTS3_STDBY_ON			= 7;
const b VSTS3_TENSION_REL		= 6;
const b VSTS3_FROM_SOM_2_EOM	= 5;
const b VSTS3_EOM_OVERRUN		= 4;
const b VSTS3_EJECT				= 1;
const b VSTS3_REWIND			= 0;
//===================================
// CART Status Bit Positions
//===================================
const b CART0_DOOR_OPEN			= 5;
const b CART0_VTR_LOCAL			= 1;
const b CART0_VTR_TEST			= 0;
//===
const b CART1_CART_DOWN			= 7;
const b CART1_POWER_FAIL		= 6;
const b CART1_REF_MISSING		= 5;
const b CART1_VTR_ERROR			= 0;
//===
const b CART4_ELEVATOR_RECOVER_REQ = 1;
const b CART4_ELEVATOR_INITIAL_REQ = 0;
//===
const b CART6_EVENT_CHG_BY_LOCAL = 0;
//===
const b CART7_TX_STOP	= 0x30;
const b CART7_TX_RUN	= 0x31;
const b CART7_TX_PAUSE	= 0x32;
const b CART7_TX_CLOSE	= 0x33;
const b CART7_TX_DISCONN= 0x80;
//===================================

//
// Key items in a message
//
const b IDENT_HDR = 10;
const b PLAYLIST_HDR = 79;
const b EVENT_MISMATCH_HDR = 13;
const b PLAYLIST_DELETED = 12;
const b SYSMODE_HDR = 11;
const b ONLINE_HDR = 14;
const b PLAYLIST_SAVED_HDR = 15;
const b CARTSTAT_HDR = 74;
const b SOH = 1;

const b PLAYLIST_CMD = 0x41;
const b FLEXSYS_CMD  = 0x42;

const b STX = 2;
const b ETX = 3;
//
// Event mismatch message
//
const b EV_MISM_HDR			= 0;
const b EV_MISM_FLAG		= 1;
const b EV_MISM_EVENT_NUM	= 2;

//
// Start positions in the message for data
//
const b PL_HDR			= 0;
const b PL_IN_LOCKOUT	= 1;
const b PL_SEQ			= 2;
const b PL_SIZE			= 3;
const b PL_EVENT_TYPE	= 4;
const b PL_NUMBER		= 5;
const b PL_ON_AIR_DAY	= 13;
const b PL_ON_AIR_TIME	= 16;
const b PL_IDENT		= 20;
const b PL_TITLE		= 28;
const b PL_SOM			= 44;
const b PL_DURATION		= 48;
const b PL_CHANNEL		= 52;
const b PL_BIN			= 53;
const b PL_SEG_NUM		= 54;
const b PL_RESERVED		= 55;
const b PL_PATTERN		= 59;
const b PL_LIST_STAT	= 67;
const b PL_VTS0			= 68;
const b PL_VTS1			= 69;
const b PL_VTS2			= 70;
const b PL_VTS3			= 71;

const b PL_CALC_START   = 68;

const w MAX_PLAYLIST_MSG_LEN = (PL_VTS3 + 1 + CMD_CHKSUM_BYTES);
const b BCD_TIME_LEN = 4;
const b BCD_DATE_LEN = 3;

//
// Countdown message positions
//
const b CD_HDR = 0;
const b CD_EVENT_NUM = 1;
const b CD_START_TIME = 9;
const b CD_REM_DUR = 13;

const b CD_MESSAGE_LEN = (CD_REM_DUR + BCD_TIME_LEN);




#endif