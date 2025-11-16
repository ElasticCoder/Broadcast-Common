// SCReportError.cpp: implementation of the SCReportError class.

/////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "SCReportError.h"
#include "SD_SNMPVariableDefinitions.h"
#include "SD_SNMPSpecificInfo.h"


SCReportError::SCReportError()
{
	m_hPipe = INVALID_HANDLE_VALUE;
}

SCReportError::~SCReportError()
{
	if( m_hPipe != NULL )
	{
		CloseTrapPipe();
	}
}


//=============================================================================
// Function name	: ReportSNMPError
// Return type		: SNMP_INTERFACE_EXTERNAL HRESULT 
// Argument         : int Severity
// Argument         : TCHAR* ApplicationName
// Argument         : int ErrorCode
// Author           : Nneka Akwule
// Description	    : Function which passes information to the trap
SNMP_INTERFACE_EXTERNAL HRESULT ReportSNMPError(int Severity, LPCTSTR ApplicationName, int ErrorCode, LPCTSTR DescriptionString)
{
	switch(ErrorCode)
	{

	case SNMP_APP_VERSION:
		m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		break;

	case SNMP_APP_STATUS:
		if(m_MIBParameters.GetAppStatus() != Severity)
		{
			m_MIBParameters.SetAppStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;


	case SNMP_MAV_STATUS:
		if(m_MIBParameters.GetMAVStatus() != Severity)
		{
			m_MIBParameters.SetMAVStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_TDB_STATUS:
		if(m_MIBParameters.GetTDBStatus() != Severity)
		{
			m_MIBParameters.SetTDBStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_PUBLIPLAN_STATUS:
		if(m_MIBParameters.GetPubliplanStatus() != Severity)
		{
			m_MIBParameters.SetPubliplanStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_MATERIAL_STATUS:
		if(m_MIBParameters.GetMissingMaterialStatus() != Severity)
		{
			m_MIBParameters.SetMissingMaterialStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_820_STATUS:
		if(m_MIBParameters.GetBZA820CommsStatus() != Severity)
		{
			m_MIBParameters.SetBZA820CommsStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_COMMS_STATUS:
		if(m_MIBParameters.GetCommsStatus() != Severity)
		{
			m_MIBParameters.SetCommsStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;


	case SNMP_PETASITE_STATUS:
		if(m_MIBParameters.GetPetasiteStatus() != Severity)
		{
			m_MIBParameters.SetPetasiteStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_GATEWAY_STATUS:
		m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		break;
	case SNMP_FCM_STATUS:
		if(m_MIBParameters.GetFcmStatus() != Severity)
		{
			m_MIBParameters.SetFcmStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;



/// For KAVO
	case SNMP_KAVO_ROUTER_STATUS:
		if(m_MIBParameters.GetRouterStatus() != Severity)
		{
			m_MIBParameters.SetRouterStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;

	case SNMP_KAVO_DATA_INSERTER_STATUS:
		if(m_MIBParameters.GetDataInserterStatus() != Severity)
		{
			m_MIBParameters.SetDataInserterStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_KAVO_TX_SYNC_STATUS:
		if(m_MIBParameters.GetTxSyncStatus() != Severity)
		{
			m_MIBParameters.SetTxSyncStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;
	case SNMP_KAVO_STILLS_STORE_STATUS:
		if(m_MIBParameters.GetStillsStoreStatus() != Severity)
		{
			m_MIBParameters.SetStillsStoreStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;


// For SS
	case SNMP_STREAM_SYNC_PLATO_STATUS:
		if(m_MIBParameters.GetSSPlatoStatus() != Severity)
		{
			m_MIBParameters.SetSSPlatoStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;

	case SNMP_STREAM_SYNC_STC_STATUS:
		if(m_MIBParameters.GetSSStcStatus() != Severity)
		{
			m_MIBParameters.SetSSStcStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}break;

	case SNMP_STREAM_SYNC_SUBTITLING_STATUS:
		if (m_MIBParameters.GetSSSubtitlingStatus() != Severity)
		{
			m_MIBParameters.SetSSSubtitlingStatus(Severity);
			m_Report.SendTrapInfo(ErrorCode,Severity,ApplicationName,DescriptionString);
		}
		break;


	default:
		break;
	}

	return S_OK;
}



//=============================================================================
// Function name	: SCReportError::SendTrapInfo
// Return type		: void 
// Argument         :  int Trap
// Argument         : int value
// Argument         : TCHAR* ApplicationName
// Author           : Nneka Akwule
// Description	    : Sends information to the Proxy agent
void SCReportError::SendTrapInfo( int Trap, int value, LPCTSTR ApplicationName, LPCTSTR DescriptionString)
{
	BOOL fReturn;
	TCHAR szWriteBuf[128];
	TCHAR szReadBuf[3];
	long lBytesWritten;
	long lBytesRead;
	char temp[1] ;
	if( WaitNamedPipe( SZNAMEDPIPESERVER, 5000) == 0)
		printf("No Pipe.\n");
	else
	{	
		if ( m_hPipe == INVALID_HANDLE_VALUE )
			OpenTrapPipe(  );
	}

	if ( m_hPipe != INVALID_HANDLE_VALUE )
	{
		itoa(Trap,temp,10);
		strcpy( szWriteBuf, temp);
		strcat(szWriteBuf,"|");
		itoa(value,temp,10);
		strcat( szWriteBuf, _ltoa(value,temp,10));
		strcat(szWriteBuf,"|");
		strcat(szWriteBuf,ApplicationName);
		strcat(szWriteBuf,"|");
		strcat(szWriteBuf,DescriptionString);
		

	    fReturn = WriteFile( m_hPipe, szWriteBuf, lstrlen( szWriteBuf ), (unsigned long*)&lBytesWritten, NULL );
	    if ( fReturn == FALSE )
			TRACE( "WriteFile: Failed to write data to named pipe (GetLastError = %lu)\n", GetLastError() );
		else
		{
	        TRACE( "Data written to named pipe.\nWrote: %s\n", szWriteBuf );
			fReturn = ReadFile( m_hPipe, szReadBuf, lstrlen( szReadBuf ), (unsigned long*)&lBytesRead, NULL );
	    }
	}				
	CloseTrapPipe( );
}
//=============================================================================
// Function name	: SCReportError::SendTrapInfo
// Return type		: void 
// Argument         :  int Trap
// Argument         : int value
// Argument         : TCHAR* ApplicationName
// Author           : Nneka Akwule
// Description	    : Sends information to the Proxy agent
void SCReportError::SendTrapInfo( int Trap, int value, LPCTSTR ApplicationName)
{
	BOOL fReturn;
	TCHAR szWriteBuf[128];
	long lBytesWritten;
	char temp[1] ;
	if( WaitNamedPipe( SZNAMEDPIPESERVER, 5000) == 0)
		printf("No Pipe.\n");
	else
	{	
		if ( m_hPipe == INVALID_HANDLE_VALUE )
			OpenTrapPipe(  );
	}

	if ( m_hPipe != INVALID_HANDLE_VALUE )
	{
		itoa(Trap,temp,10);
		strcpy( szWriteBuf, temp);
		strcat(szWriteBuf,"|");
		itoa(value,temp,10);
		strcat( szWriteBuf, _ltoa(value,temp,10));
		strcat(szWriteBuf,"|");
		strcat(szWriteBuf,ApplicationName);
		

	    fReturn = WriteFile( m_hPipe, szWriteBuf, lstrlen( szWriteBuf ), (unsigned long*)&lBytesWritten, NULL );
	    if ( fReturn == FALSE )
			TRACE( "WriteFile: Failed to write data to named pipe (GetLastError = %lu)\n", GetLastError() );
		else
		{
	        TRACE( "Data written to named pipe.\nWrote: %s\n", szWriteBuf );
	    }
	}				
	CloseTrapPipe( );
}

//=============================================================================
// Function name	: SCReportError::OpenTrapPipe
// Return type		: BOOL 
// Argument         : void
// Author           : Nneka Akwule
// Description	    : Opens a named pipe
BOOL SCReportError::OpenTrapPipe( void )
{
	BOOL fReturn = TRUE;

    m_hPipe = CreateFile( SZNAMEDPIPESERVER, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 
	  0, OPEN_EXISTING, 0, 0);
    if ( m_hPipe == INVALID_HANDLE_VALUE )
	{
        TRACE( "CreateFile: Failed to connect to named pipe %s (GetLastError = %lu)\n", SZNAMEDPIPESERVER, GetLastError() );
        fReturn = FALSE;
	}
    else
	{
		DWORD dwMode = PIPE_READMODE_MESSAGE|PIPE_WAIT;
        if ( SetNamedPipeHandleState( m_hPipe, &dwMode, NULL, NULL ) == 0 )
		{
			TRACE( "SetNamedPipeHandleState: Failed to set named pipe mode" );
            fReturn = FALSE;
		}
	}
	return( fReturn ); 
}
                                                            

void SCReportError::CloseTrapPipe( void )
{
    FlushFileBuffers( m_hPipe );
    DisconnectNamedPipe( m_hPipe );
    CloseHandle( m_hPipe );
    m_hPipe = INVALID_HANDLE_VALUE;
}
