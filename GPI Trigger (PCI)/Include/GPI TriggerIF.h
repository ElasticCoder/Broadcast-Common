#ifndef _TRIGGERIF_H
	#define _TRIGGERIF_H
#endif

// Ensures that the single header can be used for imports and exports
#ifdef _DLL_IS_GPITRIGGER_
	#define TRIGGER_EXTERNAL __declspec(dllexport)
#else
	#define TRIGGER_EXTERNAL __declspec(dllimport)
#endif

#include <portable_types.h>						// Type definitions

#define MAX_GPI_GROUPS		4					// The number of GPI groups

enum eRegisterType
{
	OUTPUT_REGISTER = 0,
	INPUT_REGISTER
};

/***************************************/
/* The messages sent to the controller */
/***************************************/
const m GPI_MSG_BASE = WM_USER + 1024;
const m GPI_WRITE_OUTPUT = GPI_MSG_BASE + 1;

struct WriteOutputMsg
{
	int m_iGroupNum;
	int m_iOutputNum;
};

/*******************************************/
/* The parameters passed to the controller */
/*******************************************/
struct GPIThreadParams
{
	HWND m_hParentWnd;							// The receiving parent window
	ULONG m_ulBaseAddress;						// Base address of the GPI board
	UINT m_uMessageID;							// The message sent to the parent[MAX_GPI_GROUPS];			// Group information
	BOOL m_bHigherPriority;						// Does the thread have a higher than normal priority?
	w m_wThreadNum;								// The thread number
	eRegisterType m_eRegister[MAX_GPI_GROUPS];	// The register types
};


/************************/
/* The functions called */
/************************/

TRIGGER_EXTERNAL UINT StartGPIMonitor(GPIThreadParams p_Params);
TRIGGER_EXTERNAL BOOL StopGPIMonitor(DWORD p_dwTimeout = 5000);

/****************************************************/
/* The codes returned when starting the GPI monitor */
/****************************************************/
const UINT GPI_START_SUCCESS = 0;					// The GPI monitor started
const UINT GPI_ALREADY_STARTED = 1;					// The monitor has already started
const UINT GPI_DRIVER_INIT_FAIL = 2;				// Driver failed
const UINT GPI_OUTPUT_INIT_FAIL = 3;				// Output enable failed

