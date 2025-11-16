// GPI Trigger.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GPI Trigger.h"
#include <GPI TriggerIF.h>					// Defines the interface functions
#include <ArcPCI.h>						// For the Arcom GPI card
#include <itc.h>							// Thread controller
#include <itrq.h>							// Read queues

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const long MAIN_EXECUTION_DELAY = 10;		// The execution loop time (ms)
const long LED_DISPLAY_TIME = 400;			// The LED display time (ms)
const long OUTPUT_DURATION = 500;			// The duration of an output (ms)

const ULONG BOARD_IDENTIFIER = 1;			// The board identifier

// Global data
SERVICES_EXTERNAL	C_itc	Inter_Thread_Control;

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CGPITriggerApp

BEGIN_MESSAGE_MAP(CGPITriggerApp, CWinApp)
	//{{AFX_MSG_MAP(CGPITriggerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPITriggerApp construction

CGPITriggerApp::CGPITriggerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGPITriggerApp object

CGPITriggerApp theApp;


DWORD WINAPI ControlMain(LPVOID pParam);


GPIThreadParams gThreadParams;					// Thread params

HANDLE ghThread = NULL;							// Thread handle
BOOL gbThreadFlag = TRUE;						// Thread execution flag
BOOL gbThreadStarted = FALSE;					// Was the thread started?
BOOL gbDriverInitialised = FALSE;				// Is the driver initialised?

/******************************************************************************/
// Function name	: StartGPIThread
// Return type		: TRIGGER_EXTERNAL HANDLE 
// Argument         : BOOL* p_pThreadFlag
// Argument         : HWND p_hParent
// Argument         : UINT p_uMessage
// Author           : 
// Description	    : Starts the GPI thread
/******************************************************************************/

TRIGGER_EXTERNAL UINT StartGPIMonitor(GPIThreadParams p_Params)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ULONG ulResult;									// Result code
	UINT iExitCode = GPI_START_SUCCESS;				// The exit code
	DWORD dwThreadID;								// Thread ID

	if (! gbThreadStarted)
	{
		// Set up the parameters used by the thread
		gThreadParams = p_Params;

		// Attempt to initialise the GPI board driver
		ulResult = PciIB40_Init(BOARD_IDENTIFIER);

		// Did the GPI board driver initialise?
		if (ulResult == AN_SUCCESS)
		{
			// The driver is initialised
			gbDriverInitialised = TRUE;

			// Enable the inputs and outputs on the GPI board
			ulResult = PciIB40_OutputsEnable(BOARD_IDENTIFIER, AN_ENABLE);

			if (ulResult == AN_SUCCESS)
			{
				ghThread = ::CreateThread(
					NULL,						// Thread cannot be inherited
					0,							// Use same stack as parent
					ControlMain,				// Thread procedure function
					NULL,						// Thread parameters
					0,							// Thread starts immediately
					&dwThreadID);				// Thread identifier

				if (ghThread != NULL)
				{
					if (gThreadParams.m_bHigherPriority)
					{
						// Set the thread priority to be slightly higher than normal
						if (! ::SetThreadPriority(ghThread, THREAD_PRIORITY_ABOVE_NORMAL))
						{
							TRACE("Failed to set priority on GPI trigger thread\n");
						}
					}

					gbThreadStarted = TRUE;
					TRACE("GPI trigger thread started (ID = 0x%X)\n", dwThreadID);
				}
			}
			else
			{
				TRACE("GPI driver FAILED to enable outputs (error = %ld)\n", ulResult);
				iExitCode = GPI_OUTPUT_INIT_FAIL;
			}
		}
		else
		{
			TRACE("GPI driver FAILED (error = %ld)\n", ulResult);
			iExitCode = GPI_DRIVER_INIT_FAIL;
		}
	}
	else
	{
		TRACE("GPI trigger thread already running!\n");
		iExitCode = GPI_ALREADY_STARTED;
	}

	return iExitCode;
}


/******************************************************************************/
// Function name	: StopGPIThread
// Return type		: TRIGGER_EXTERNAL BOOL 
// Argument         : HANDLE p_hThread
// Author           : 
// Description	    : Stops the GPI thread
/******************************************************************************/

TRIGGER_EXTERNAL BOOL StopGPIMonitor(DWORD p_dwTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ULONG ulResult;									// Result code
	BOOL bThreadStopped = FALSE;					// Was the thread stopped?

	// Is the thread running
	if (ghThread != NULL)
	{
		gbThreadFlag = FALSE;
		Sleep(100);

		if (WaitForSingleObject(ghThread, p_dwTimeout) == WAIT_OBJECT_0)
		{
			bThreadStopped = TRUE;

			// Free the thread handle
			CloseHandle(ghThread);
		}
	}

	// Stop the driver if it is initialised
	if (gbDriverInitialised)
	{
		// Enable the inputs and outputs on the GPI board
		ulResult = PciIB40_Close(BOARD_IDENTIFIER);

		if (ulResult != AN_SUCCESS)
		{
			TRACE("Failed to close the GPI board driver (error = %ld)\n", ulResult);
		}
	}

	return bThreadStopped;
}

/******************************************************************************/
// Function name	: ControlMain
// Return type		: DWORD WINAPI 
// Argument         : LPVOID pParam
// Author           : 
// Description	    : The main GPI execution function
/******************************************************************************/

DWORD WINAPI ControlMain(LPVOID pParam)
{
	C_itrq ReadQueue;							// The read queue
	T_itrq_read_status Read_Status;				// Read status
	C_itq_q_msg	Msg;							// Received message
	WriteOutputMsg* pOutputMsg = NULL;			// Pointer to an output message
	UCHAR Input[MAX_GPI_GROUPS];				// The GPI inputs
	UCHAR byRegister;							// GPI input
	ULONG ulResult;								// Result code
	int iCount = 0;								// Counter variable
	int iLEDTimer = 0;							// The LED timer
	int iOutputTimer = 0;						// The output timer
	DWORD dwExitCode = 0;						// Exit code
	BOOL bLEDOn = FALSE;						// Is the LED on?
	BOOL bOutputSet = FALSE;					// Is an output set?
	BOOL bSuccess = TRUE;						// Success code

	// Register the thread
	g00_register_thread(gThreadParams.m_wThreadNum, 1);

	// Initialize the read queue
    if (ReadQueue.initialise(
					&Inter_Thread_Control,	// The inter-thread control object
					FALSE,					// Not a windows queue
					NULL,					// Not required (not a windows queue)
					20)						// Maximum queue size of 20 messages
					!= Q_OK)
    {
		// The read queue could not be initialised - a failure has occured
		bSuccess = FALSE;
		TRACE("Failed to initialise the GPI controller read queue!\n");
    }


	// Initialise the inputs
	for (iCount = 0; iCount < MAX_GPI_GROUPS; iCount++)
	{
		// Write to the output - this enables the inputs to be read
		ulResult = PciIB40_DigGroupWrite(
								BOARD_IDENTIFIER, 
								iCount, 
								0xFF);

		if (ulResult != AN_SUCCESS)
		{
			TRACE("GPI write failure for group %d (base address 0x%X)\n",
												iCount,
												gThreadParams.m_ulBaseAddress);

		}

		Sleep(50);

		// Take an initial reading of the board inputs
		ulResult = PciIB40_DigGroupRead(
								BOARD_IDENTIFIER,
								iCount,
								&Input[iCount]);

		if (ulResult != AN_SUCCESS)
		{
			TRACE("GPI read failure for group %d (base address 0x%X)\n",
												iCount,
												gThreadParams.m_ulBaseAddress);

		}
	}

/***********************/
/* Main execution loop */
/***********************/
	while (gbThreadFlag)
	{
		Read_Status = ReadQueue.read(
						Msg,
						MAIN_EXECUTION_DELAY);

		// Was a message received?
		if (Read_Status == RQ_OK)
		{
			if (Msg.m_id_m == GPI_WRITE_OUTPUT)
			{
				pOutputMsg = (WriteOutputMsg*) Msg.m_data_p;

				// Check that the message values are valid
				if ((pOutputMsg->m_iGroupNum >= 0) &&
					(pOutputMsg->m_iGroupNum < MAX_GPI_GROUPS) &&
					(pOutputMsg->m_iOutputNum >= 0) &&
					(pOutputMsg->m_iOutputNum < 8))
				{
					if (gThreadParams.m_eRegister[pOutputMsg->m_iGroupNum] == OUTPUT_REGISTER)
					{
						ulResult = PciIB40_DigBitWrite(
											BOARD_IDENTIFIER, 
											(pOutputMsg->m_iGroupNum * 8) + pOutputMsg->m_iOutputNum,
											0x00);

						if (ulResult == AN_SUCCESS)
						{
							// Store the new value
							Input[pOutputMsg->m_iGroupNum] &= ~(0x01 << pOutputMsg->m_iOutputNum);

							TRACE("GPI issued for group %d, output %d\n", 
												pOutputMsg->m_iGroupNum,
												pOutputMsg->m_iOutputNum);

							// Start the output timer
							bOutputSet = TRUE;
							iOutputTimer = 0;

							// Turn on the LED
							bLEDOn = TRUE;
							iLEDTimer = 0;
							PciIB40_UserLED(BOARD_IDENTIFIER, AN_LED_ON);
						}
						else
						{
							TRACE("GPI write failure for group %d (base address 0x%X)\n",
																iCount,
																gThreadParams.m_ulBaseAddress);
						}
					}
					else
					{
						TRACE("Ignoring GPI message: register %d is not an output\n",
										pOutputMsg->m_iGroupNum);
					}
				}
				else
				{
					TRACE("Invalid GPI group (%d) or output (%d) received!\n",
												pOutputMsg->m_iGroupNum,
												pOutputMsg->m_iOutputNum);
				}
			}
			else
			{
				TRACE("Invalid message sent to GPI controller!\n");
			}

			// Delete the memory allocated for the message
			ReadQueue.free_msg_data(Msg.m_data_p);
		}
		else if (Read_Status == RQ_TIMEOUT)
		{
			// Read all of the input registers
			for (iCount = 0; iCount < MAX_GPI_GROUPS; iCount++)
			{
				if (gThreadParams.m_eRegister[iCount] == INPUT_REGISTER)
				{
					ulResult = PciIB40_DigGroupRead(BOARD_IDENTIFIER, iCount, &byRegister);

					if (ulResult != AN_SUCCESS)
					{
						TRACE("GPI read failure for group %d (base address 0x%X)\n",
															iCount,
															gThreadParams.m_ulBaseAddress);
					}
					else
					{
						// Has a GPI changed
						if (byRegister != Input[iCount])
						{
							::PostMessage(
								gThreadParams.m_hParentWnd,
								gThreadParams.m_uMessageID,
								iCount,
								byRegister);

							// Store the new value
							Input[iCount] = byRegister;

							TRACE("GPI changed! Input %d - value 0x%02X\n", iCount, byRegister);

							// Turn on the LED
							bLEDOn = TRUE;
							iLEDTimer = 0;
							PciIB40_UserLED(BOARD_IDENTIFIER, AN_LED_ON);
						}
					}
				}
			}

			// Is an output set?
			if (bOutputSet)
			{
				// Increment the timer
				iOutputTimer++;

				// Has the timer expired?
				if ((iOutputTimer * MAIN_EXECUTION_DELAY) >= OUTPUT_DURATION)
				{
					// Reset the GPIs
					for (iCount = 0; iCount < MAX_GPI_GROUPS; iCount++)
					{
						// Is this register an output?
						if (gThreadParams.m_eRegister[iCount] == OUTPUT_REGISTER)
						{
							// Write to the output - this enables the inputs to be read
							ulResult = PciIB40_DigGroupWrite(
											BOARD_IDENTIFIER, 
											iCount, 
											0xFF);

							if (ulResult != AN_SUCCESS)
							{
								TRACE("GPI reset failure for group %d (base address 0x%X)\n",
																	iCount,
																	gThreadParams.m_ulBaseAddress);

							}
						}
					}

					iOutputTimer = 0;
					bOutputSet = FALSE;
				}
			}

			// Is the LED on?
			if (bLEDOn)
			{
				// Increment the timer
				iLEDTimer++;

				// Has the timer expired?
				if ((iLEDTimer * MAIN_EXECUTION_DELAY) >= LED_DISPLAY_TIME)
				{
					// Turn off the LED
					bLEDOn = FALSE;
					iLEDTimer = 0;
					PciIB40_UserLED(BOARD_IDENTIFIER, AN_LED_OFF);
				}
			}
		}
		else
		{
			TRACE("Error occurred on GPI controller read queue!\n");
		}
	}

	ReadQueue.disconnect();

	TRACE("Exiting GPI thread!\n");

	return dwExitCode;
}

