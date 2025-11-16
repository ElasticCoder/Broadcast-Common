///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : PLATO
// Subsystem   : DBAccess/DBInterface
// File Name   : oracle.h       
// File Type   : C++ header
// Description : ORACLE status definitions
// Author      : Jez Walters modified for PLATO by Rick Miles
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ORACLE_H
#define _ORACLE_H

#include <portable_types.h>  // Type definitions

static const l ORA_SUCCESS				= 0;      // Successful completion status
static const l ORA_NO_DATA_WARN			= 100;    // No data found warning status
static const l ORA_IPC_FAILURE			= -1;     // IPC failure status
static const l ORA_NO_DATA_ERR			= -1403;  // No data found error status
static const l ORA_FETCHED_COLUMN_NULL	= -1405;  // Fecthed column is NULL

#define IS_DB_FAILURE(SQLCODE) ((SQLCODE == -1012)   || \
                                (SQLCODE == -1014)   || \
                                (SQLCODE == -1033)   || \
                                (SQLCODE == -1034)   || \
                                (SQLCODE == -1089)   || \
                                (SQLCODE == -1090)   || \
                                (SQLCODE == -1092)   || \
                                (SQLCODE == -3113)   || \
                                (SQLCODE == -3114)   || \
                                (SQLCODE == -12571))

#define IS_DB_NO_DATA(SQLCODE) ((SQLCODE == 100)    || \
                                (SQLCODE == -1403)	|| \
								(SQLCODE == -1405))

#endif // _ORACLE_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
