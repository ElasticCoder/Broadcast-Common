///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
// System Name : PLATO
// Subsystem   : Common
// File Name   : itc_heap_dump.cpp
// File Type   : C++ Source
// Description : Dump unreleased buffers in ITC message data heap
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////
//
// Issue Date Released  Change
// 0.1   24/01/97		Draft
//
///////////////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include <standard.h>
#include <tchar.h>
#include <portable_types.h>		   // Type definitions
#include <g00_process_registry.h>
#include <g02_datetime.h>
#include "itc_heap_info.h"

#include <stdio.h>

#include "OleAuto.h"
#include "AtlConv.h"

#ifdef _DEBUG
//
// Define data to enable exclusive access to the dump file
//
static HANDLE	v_file_access_mutex_h = NULL;	// 
static const tc v_file_access_mutex_name_tca[] = _TEXT("itc_dump_file_access");
static const tc	v_dump_file_name_tca[] = _TEXT("C:\\temp\\heap_dump.txt");

///////////////////////////////////////////////////////////////////////////////
// Function Description : Obtain exclusive access to the dump file
// Author               : Duncan Olesen
// Date Created         : 24/01/97
// Date Last Modified   : 24/01/97
///////////////////////////////////////////////////////////////////////////////

BOOL itc_lock_file()
{
	BOOL	locked;

	locked = FALSE;
//
// Obtain handle to mutex if not already done so
//
	if (v_file_access_mutex_h == NULL)
	{
		v_file_access_mutex_h = CreateMutex(
									NULL,		// Security attributes
									FALSE,		// Initial Owner
									v_file_access_mutex_name_tca); // mutex name
	}

	if (v_file_access_mutex_h != NULL)
	{
		DWORD	dwWaitStatus;

		dwWaitStatus = WaitForSingleObject(
							v_file_access_mutex_h,	// hObject
							(DWORD) 1000);			// dwTimeout

		if (dwWaitStatus == WAIT_OBJECT_0)
		{
			locked = TRUE;
		}
	}

	return locked;
} // itc_lock_file

///////////////////////////////////////////////////////////////////////////////
// Function Description : Build a string containing a line to write to dump file
// Author               : Duncan Olesen
// Date Created         : 24/01/97
// Date Last Modified   : 24/01/97
///////////////////////////////////////////////////////////////////////////////

void build_line(
			PVOID	lpData,
			DWORD	dwOffset,
			DWORD	dwSize,
			c		*line_buffer_ca,
			int		bytes_per_line)
{
	const int	BYTE_NUM_LEN = 3;
	const int	BYTE_TEXT_LEN = 1;
	const int	NUM_TEXT_SEPARATOR_LEN = 2;
	const int	TEXT_OFFSET =
					BYTE_NUM_LEN * bytes_per_line +
					NUM_TEXT_SEPARATOR_LEN;
	const int	DATA_LEN =
					TEXT_OFFSET + BYTE_TEXT_LEN * bytes_per_line;

	c		data_ca[256];
	c		temp_ca[3];
	int		bytes_to_format;
	b		*buffer_bp;

	ASSERT(DATA_LEN < 256);

//
// Initialise data string
//
	memset(data_ca, ' ', DATA_LEN);
	data_ca[DATA_LEN] = '\0';
//
// Determine number of bytes to format
//
	if (dwSize - dwOffset < (DWORD) bytes_per_line)
	{
		bytes_to_format = (int) (dwSize - dwOffset);
	}
	else
	{
		bytes_to_format = bytes_per_line;
	}
//
// Loop to output each byte
//
	buffer_bp = ((b *) lpData) + dwOffset;

	for (int index = 0; index < bytes_to_format; index++)
	{
//
// Format number data
//
		sprintf(
			temp_ca,
			"%02X",
			(unsigned int) buffer_bp[index]);

		data_ca[BYTE_NUM_LEN * index]		= temp_ca[0];
		data_ca[BYTE_NUM_LEN * index + 1]	= temp_ca[1];
//
// Format text data
//
		if (isprint((char) buffer_bp[index]))
		{
			data_ca[TEXT_OFFSET + index * BYTE_TEXT_LEN] = buffer_bp[index];
		}
		else
		{
			data_ca[TEXT_OFFSET + index * BYTE_TEXT_LEN] = '.';
		}
	}
//
// Format the line
//
	sprintf(
		line_buffer_ca,
		"%08X: %s\r\n",
		dwOffset,
		data_ca);
} // build_line

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write an unreleased block to file
// Author               : Duncan Olesen
// Date Created         : 24/01/97
// Date Last Modified   : 24/01/97
///////////////////////////////////////////////////////////////////////////////

void itc_dump_block(
			PVOID		lpData,
			DWORD		dwSize,
			HANDLE		hFile)
{
	DWORD	dwOffset;

	c		line_buffer_ca[512];
	tc		thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	tc		alloc_thread_name_tca[G00_MAX_THREAD_NAME_LEN_W + 1];
	DWORD	dwBytesWritten;
	C_g02_datetime	current_time;
	CString	timestamp;
//
// Set timestamp to current time
//
	current_time.set_datetime();
	timestamp = current_time.Format(CString(_TEXT("%d/%m/%y %H:%M:%S.%F")));

	g00_thread_name(thread_name_tca);

	if (dwSize <= sizeof(S_heap_head_sentinel) + sizeof(S_heap_tail_sentinel))
	{
		sprintf(
			line_buffer_ca,
			"\r\n%s %s\r\nInvalid Buffer Size = %d (0x%08X)\r\n",
			timestamp,
			thread_name_tca,
			dwSize,
			dwSize);
	}
	else
	{
//
// Validate buffer
//
		S_heap_head_sentinel	*head_sentinel_p;
		S_heap_tail_sentinel	*tail_sentinel_p;
		BOOL					valid;
		CString					error;

		valid = TRUE;
//
// Check that allocated buffer has a valid head sentinel
//
		head_sentinel_p = (S_heap_head_sentinel *) lpData;

		if (head_sentinel_p->m_magic_num1_m != HEAP_SENTINEL_M ||
			head_sentinel_p->m_magic_num2_m != ~HEAP_SENTINEL_M)
		{
//
// Head sentinel invalid
//
			valid = FALSE;
			error = _TEXT("*** Head corrupt. ");
		}
//
// Check the tail sentinel
//
		tail_sentinel_p = (S_heap_tail_sentinel *)
				(((b *) (head_sentinel_p + 1)) + head_sentinel_p->m_data_size_m);

		if (tail_sentinel_p->m_magic_num_m != HEAP_SENTINEL_M)
		{
//
// Tail sentinel invalid
//
			valid = FALSE;
			error += _TEXT("*** Tail corrupt.");
		}

		if (valid)
		{

//
// If buffer has not been corrupted then adjust pointer and buffer size
// in order to exclude the sentinel information
//
			dwSize  = head_sentinel_p->m_data_size_m;
			head_sentinel_p->m_alloc_thread.thread_name(alloc_thread_name_tca);
			lpData  = (PVOID) (((b *) lpData) + sizeof(S_heap_head_sentinel));

			sprintf(
				line_buffer_ca,
				"\r\n%s %s\r\nData Size = %d (0x%08X), alloc'd by %s\r\n",
				timestamp,
				thread_name_tca,
				dwSize,
				dwSize,
				alloc_thread_name_tca);
//
// Append buffer status information
//
			c	*ptr_cp;

			ptr_cp = line_buffer_ca + strlen(line_buffer_ca);

			switch (head_sentinel_p->m_status)
			{
			case ITC_DATA_ALLOCATED:
				strcat(
					ptr_cp,
					"Status = ALLOCATED\r\n");
				break;

			case ITC_DATA_SENT:
				sprintf(
					ptr_cp,
					"Status = SENT, id = WM_USER + %d, queue = %s\r\n",
					head_sentinel_p->m_msg_id_m - WM_USER,
					head_sentinel_p->m_read_q_tca);
				break;

			case ITC_DATA_READ:
				sprintf(
					ptr_cp,
					"Status = READ, id = WM_USER + %d, queue = %s\r\n",
					head_sentinel_p->m_msg_id_m - WM_USER,
					head_sentinel_p->m_read_q_tca);
				break;

			case ITC_DATA_POSTED:
				sprintf(
					ptr_cp,
					"Status = POSTED, id = WM_USER + %d, queue = %s\r\n",
					head_sentinel_p->m_msg_id_m - WM_USER,
					head_sentinel_p->m_read_q_tca);
				break;

			default:
				strcat(
					ptr_cp,
					"Status unrecognised\r\n");
				break;
			} // switch on buffer status
		}
		else
		{
			USES_CONVERSION;

			sprintf(
				line_buffer_ca,
				"\r\n%s %s\r\nBuffer Size = %d (0x%08X)\r\n%s\r\n",
				timestamp,
				thread_name_tca,
				dwSize,
				dwSize,
				T2A(LPTSTR(LPCTSTR(error))));
		}
	}
//
// Write buffer header to file
//
	WriteFile(
		hFile,
		line_buffer_ca,
		strlen(line_buffer_ca),
		&dwBytesWritten,
		NULL);
//
// Format and Write buffer data line by line
//
	const int	BYTES_PER_LINE = 8;

	for (dwOffset = 0; dwOffset < dwSize; dwOffset += BYTES_PER_LINE)
	{
		build_line(lpData, dwOffset, dwSize, line_buffer_ca, BYTES_PER_LINE);

		WriteFile(
			hFile,
			line_buffer_ca,
			strlen(line_buffer_ca),
			&dwBytesWritten,
			NULL);
	} // loop to output buffer
} // itc_dump_block

///////////////////////////////////////////////////////////////////////////////
// Function Description : Write all unreleased buffers in a heap to file
// Author               : Duncan Olesen
// Date Created         : 24/01/97
// Date Last Modified   : 24/01/97
///////////////////////////////////////////////////////////////////////////////

void itc_dump_heap(HANDLE		hHeap)
{
	HANDLE				hFile;
	PROCESS_HEAP_ENTRY	Entry;
	BOOL				ok;
//
// Obtain exclusive access to the log file
//
	if (itc_lock_file())
	{
		hFile = CreateFile(
						v_dump_file_name_tca,
						GENERIC_WRITE,	// fdwAccess
						0,				// dfwShareMode
						NULL,			// lpsa
						OPEN_ALWAYS,	// fdwCreate
						FILE_ATTRIBUTE_NORMAL, // fdwAttrsAndFlags
						NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
//
// Move to end of file
//
			SetFilePointer(
				hFile,
				0,
				NULL,
				FILE_END);
//
// Walk through entries in the heap, dumping any that are allocated
//
			if (HeapLock(hHeap))
			{
				Entry.lpData = NULL;	// start at the beginning

				ok = HeapWalk(hHeap, &Entry);

				while (ok)
				{
					if (Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY)
					{
//
// Entry is an allocated region so dump it
//
						itc_dump_block(Entry.lpData, Entry.cbData, hFile);
					}

					ok = HeapWalk(hHeap, &Entry);
				}

				HeapUnlock(hHeap);
			}
			else
			{
				tc		line_buffer_tca[256];
				DWORD	dwBytesWritten;

				_tcscpy(line_buffer_tca, _TEXT("Error: could not lock heap\r\n"));

				WriteFile(
					hFile,
					line_buffer_tca,
					_tcslen(line_buffer_tca) * sizeof(tc),
					&dwBytesWritten,
					NULL);
			}

			CloseHandle(hFile);
		}
//
// Unlock file
//
		ReleaseMutex(v_file_access_mutex_h);
	} // file locked
} // itc_dump_heap

#endif _DEBUG