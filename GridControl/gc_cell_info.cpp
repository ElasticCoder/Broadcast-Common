/******************************************************************************
 * Project     : Grid Control
 * File        : gc_cell_info.cpp
 * Author      : Duncan Olesen
 * Date        : 20/12/96
 *
 * Description : Grid Control Cell Info Structure
 *****************************************************************************/

#include "stdafx.h"

#include <gc_cell_info.h>

///////////////////////////////////////////////////////////////////////////////
// Function Description : SGrdCellInfo constructor
// Author               : Duncan Olesen
// Date Created         : 20/12/96
// Date Last Modified   : 20/12/96
///////////////////////////////////////////////////////////////////////////////

SGrdCellInfo::SGrdCellInfo()
{
	text_tcp = NULL;
} // SGrdCellInfo

///////////////////////////////////////////////////////////////////////////////
// Function Description : SGrdCellInfo destructor
// Author               : Duncan Olesen
// Date Created         : 20/12/96
// Date Last Modified   : 20/12/96
///////////////////////////////////////////////////////////////////////////////

SGrdCellInfo::~SGrdCellInfo()
{
} // ~SGrdCellInfo


///////////////////////////////////////////////////////////////////////////////
// Function Description : Set the cell text to a number
// Author               : Duncan Olesen
// Date Created         : 20/12/96
// Date Last Modified   : 20/12/96
///////////////////////////////////////////////////////////////////////////////

BOOL SGrdCellInfo::set_num(
				DWORD		p_num_l,
				const TCHAR	*p_format_tcp)
{
	BOOL	ret_val_z;
	int		printf_ret_i;
	TCHAR	buffer_tca[256];

	if (p_format_tcp == NULL)
	{
		p_format_tcp = _TEXT("%ld");
	}

	printf_ret_i = _stprintf(buffer_tca, p_format_tcp, p_num_l);

	if (printf_ret_i > 0 &&
		_tcslen(buffer_tca) <= DWORD_DIGITS_B)
	{
		_tcscpy(number_tca, buffer_tca);
		text_tcp = number_tca;
		ret_val_z = TRUE;
	}
	else
	{
		ret_val_z = FALSE;
	}

	return ret_val_z;
} // set_num
