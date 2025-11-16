// SNMPMIBParameters.cpp: implementation of the SNMPMIBParameters class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "snmperrorreporting.h"
#include "SNMPMIBParameters.h"
#include "SD_SNMPVariableDefinitions.h"
#include "SD_SNMPSpecificInfo.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SNMPMIBParameters::SNMPMIBParameters()
{
	m_iMAVStatus			= SD_SNMP_UNKNOWN;
	m_iAppStatus			= SD_SNMP_UNKNOWN;
	m_iTDBStatus			= SD_SNMP_UNKNOWN;
	m_iMissingMaterial		= SD_SNMP_UNKNOWN;
	m_iPubliplanStatus		= SD_SNMP_UNKNOWN;
	m_iBZA820CommsStatus	= SD_SNMP_UNKNOWN;
	m_iCommsStatus			= SD_SNMP_UNKNOWN;

	m_iPetasiteStatus		= SD_SNMP_UNKNOWN;
	m_iGatewayStatus		= SD_SNMP_UNKNOWN;
	m_iFcmStatus			= SD_SNMP_UNKNOWN;

	m_iRouterStatus			= SD_SNMP_UNKNOWN;
	m_iDataInserterStatus	= SD_SNMP_UNKNOWN;
	m_iTxSyncStatus			= SD_SNMP_UNKNOWN;
	m_iStillsStoreStatus	= SD_SNMP_UNKNOWN;

	m_iSSPlatoStatus		= SD_SNMP_UNKNOWN;
	m_iSSStcStatus			= SD_SNMP_UNKNOWN;
	m_iSSSubtitlingStatus	= SD_SNMP_UNKNOWN;

	m_iGenericVar18Status	= SD_SNMP_UNKNOWN;
	m_iGenericVar19Status	= SD_SNMP_UNKNOWN;


}



SNMPMIBParameters::~SNMPMIBParameters()
{

}


int SNMPMIBParameters::GetMAVStatus()
{
	return m_iMAVStatus;
}

void SNMPMIBParameters::SetMAVStatus(int iStatus)
{
	m_iMAVStatus = iStatus;
}

////

void SNMPMIBParameters::SetAppStatus(int iStatus)
{
	m_iAppStatus = iStatus;
}
int SNMPMIBParameters::GetAppStatus()
{
	return m_iAppStatus;
}

///

void SNMPMIBParameters::SetTDBStatus(int iStatus)
{
	m_iTDBStatus = iStatus;

}
int SNMPMIBParameters::GetTDBStatus()
{
	return m_iTDBStatus;
}

////

void SNMPMIBParameters::SetMissingMaterialStatus(int iStatus)
{
	m_iMissingMaterial = iStatus;
}
int SNMPMIBParameters::GetMissingMaterialStatus()
{
	return m_iMissingMaterial;
}

///

void SNMPMIBParameters::SetPubliplanStatus(int iStatus)
{
	m_iPubliplanStatus = iStatus;
}
int SNMPMIBParameters::GetPubliplanStatus()
{
	return m_iPubliplanStatus;
}

///



void SNMPMIBParameters::SetBZA820CommsStatus(int iStatus)
{
	m_iBZA820CommsStatus = iStatus;
}

int SNMPMIBParameters::GetBZA820CommsStatus()
{
	return m_iBZA820CommsStatus;
}

////

void SNMPMIBParameters::SetCommsStatus(int iStatus)
{
	m_iCommsStatus = iStatus;
}

int SNMPMIBParameters::GetCommsStatus()
{
	return m_iCommsStatus;
}

////

void SNMPMIBParameters::SetPetasiteStatus(int iStatus)
{
	m_iPetasiteStatus = iStatus;
}

int SNMPMIBParameters::GetPetasiteStatus()
{
	return m_iPetasiteStatus;
}



////

void SNMPMIBParameters::SetGatewayStatus(int iStatus)
{
	m_iGatewayStatus = iStatus;
}

int SNMPMIBParameters::GetGatewayStatus()
{
	return m_iGatewayStatus;
}


////

void SNMPMIBParameters::SetFcmStatus(int iStatus)
{
	m_iFcmStatus = iStatus;
}

int SNMPMIBParameters::GetFcmStatus()
{
	return m_iFcmStatus;
}



////

void SNMPMIBParameters::SetRouterStatus(int iStatus)
{
	m_iRouterStatus = iStatus;
}

int SNMPMIBParameters::GetRouterStatus()
{
	return m_iRouterStatus;
}



////

void SNMPMIBParameters::SetDataInserterStatus(int iStatus)
{
	m_iDataInserterStatus = iStatus;
}

int SNMPMIBParameters::GetDataInserterStatus()
{
	return m_iDataInserterStatus;
}

////

void SNMPMIBParameters::SetTxSyncStatus(int iStatus)
{
	m_iTxSyncStatus = iStatus;
}

int SNMPMIBParameters::GetTxSyncStatus()
{
	return m_iTxSyncStatus;
}

////

void SNMPMIBParameters::SetStillsStoreStatus(int iStatus)
{
	m_iStillsStoreStatus = iStatus;
}

int SNMPMIBParameters::GetStillsStoreStatus()
{
	return m_iStillsStoreStatus;
}


///

void SNMPMIBParameters::SetSSPlatoStatus(int iStatus)
{
	m_iSSPlatoStatus = iStatus;
}

int SNMPMIBParameters::GetSSPlatoStatus()
{
	return m_iSSPlatoStatus;
}


////

void SNMPMIBParameters::SetSSStcStatus(int iStatus)
{
	m_iSSStcStatus = iStatus;
}

int SNMPMIBParameters::GetSSStcStatus()
{
	return m_iSSStcStatus;
}


////

void SNMPMIBParameters::SetSSSubtitlingStatus(int iStatus)
{
	m_iSSSubtitlingStatus = iStatus;
}

int SNMPMIBParameters::GetSSSubtitlingStatus()
{
	return m_iSSSubtitlingStatus;
}



////

void SNMPMIBParameters::SetGenericVar18Status(int iStatus)
{
	m_iGenericVar18Status = iStatus;
}

int SNMPMIBParameters::GetGenericVar18Status()
{
	return m_iGenericVar18Status;
}


////

void SNMPMIBParameters::SetGenericVar19Status(int iStatus)
{
	m_iGenericVar19Status = iStatus;
}

int SNMPMIBParameters::GetGenericVar19Status()
{
	return m_iGenericVar19Status;
}
