// SNMPMIBParameters.h: interface for the SNMPMIBParameters class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNMPMIBPARAMETERS_H__948F7F48_3A97_11D3_BBC3_00C04F79B6F0__INCLUDED_)
#define AFX_SNMPMIBPARAMETERS_H__948F7F48_3A97_11D3_BBC3_00C04F79B6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SNMPMIBParameters  
{
public:
	int GetPetasiteStatus();
	void SetPetasiteStatus(int iStatus);

	SNMPMIBParameters();
	virtual ~SNMPMIBParameters();

	void SetMAVStatus(int iStatus);
	int GetMAVStatus();

	void SetTDBStatus(int iStatus);
	int GetTDBStatus();

	void SetAppStatus(int iStatus);
	int GetAppStatus();

	void SetMissingMaterialStatus(int iStatus);
	int GetMissingMaterialStatus();

	void SetPubliplanStatus(int iStatus);
	int GetPubliplanStatus();

	int GetCommsStatus();
	void SetCommsStatus(int iStatus);

	int GetBZA820CommsStatus();
	void SetBZA820CommsStatus(int iStatus);

	int GetGatewayStatus();
	void SetGatewayStatus(int iStatus);

	int GetFcmStatus();
	void SetFcmStatus(int iStatus);




	int GetRouterStatus();
	void SetRouterStatus(int iStatus);

	int GetDataInserterStatus();
	void SetDataInserterStatus(int iStatus);

	int GetTxSyncStatus();
	void SetTxSyncStatus(int iStatus);

	int GetStillsStoreStatus();
	void SetStillsStoreStatus(int iStatus);



	int GetSSPlatoStatus();
	void SetSSPlatoStatus(int iStatus);

	int GetSSStcStatus();
	void SetSSStcStatus(int iStatus);

	int GetSSSubtitlingStatus();
	void SetSSSubtitlingStatus(int iStatus);



	int GetGenericVar18Status();
	void SetGenericVar18Status(int iStatus);

	int GetGenericVar19Status();
	void SetGenericVar19Status(int iStatus);

	int m_iAppStatus;
	int m_iMAVStatus;
	int m_iTDBStatus;
	int m_iPubliplanStatus;
	int m_iMissingMaterial;
	int m_iBZA820CommsStatus;
	int m_iCommsStatus;
	int m_iPetasiteStatus;
	int m_iGatewayStatus;
	int m_iFcmStatus;

	int m_iRouterStatus;
	int m_iDataInserterStatus;
	int m_iTxSyncStatus;
	int m_iStillsStoreStatus;

	int m_iSSPlatoStatus;
	int m_iSSStcStatus;
	int m_iSSSubtitlingStatus;

	int m_iGenericVar18Status;
	int m_iGenericVar19Status;


};

#endif // !defined(AFX_SNMPMIBPARAMETERS_H__948F7F48_3A97_11D3_BBC3_00C04F79B6F0__INCLUDED_)
