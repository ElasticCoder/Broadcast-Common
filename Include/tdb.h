///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : 
// File Name   : tdb.h
// File Type   : C++ header
// Description : Plato definitions associated with tdb database
// Author      : Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _TDB_H
#define _TDB_H

#define TDB_DATE_LEN			6	// (ddmmyy)
#define TDB_PC_IDENT_LEN		6
#define TDB_DV_IDENT_LEN		6
#define TDB_SI_TITLE_LEN		16
#define TDB_SI_MATERIAL_ID_LEN	8
#define TDB_SI_PATTERN_LEN		8
#define TDB_SI_BROADCAST_ID_LEN 32
#define TDB_AI_SOURCE_NO_LEN	8
#define TDB_AI_SOURCE_NAME_LEN	32
#define TDB_MT_TITLE_LEN		200


#define TDB_AI_TYPE_KEY			1
#define TDB_AI_TYPE_VOICEOVER	2
///////////////////////////////////////////////////////////////////////////////
// Define constants
///////////////////////////////////////////////////////////////////////////////
static const c SI_ALERT_NAME[]  = "SI_ALERT";
static const c AI_ALERT_NAME[]   = "AI_ALERT";

static const u	TDB_GPI_AUTO_U			= 0xFFFF;	// Internal representation of NULL gpi field (auto)
static const m	TDB_SOM_UNSPECIFIED_M	= 0xFFFF;	// Internal representation of NULL som field (unspecified)

#endif  // _TDB_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
