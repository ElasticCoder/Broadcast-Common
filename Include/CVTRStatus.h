#ifndef __VTRSTATUS_H__
#define __VTRSTATUS_H__

#include <udp_message.h>
#include "resource.h"
#include <dataconvert.h>

#define NO_EVENT_TYPE (b) -1;


//====================================================
// These constants provide a means of determining the 
// way that the status column should draw itself.
//====================================================
enum VTRStatusCond
{
	VTR_NORMAL,
	VTR_ONLINE,
	VTR_WARNING,
	VTR_ERROR_1
};




class CPetraVTStat
{

public:
	enum PlItemState
	{
		MAT_ERROR,
		TITLE_MISM,
		ON_LINE,
		OTHER
	};

	//============================================
	// Construction
	//============================================
	CPetraVTStat( );
	CPetraVTStat(b bEventType, b* bpVSTS, i iSeq , z zTrace = FALSE);

	~CPetraVTStat();

	//============================================
	// Functions
	//============================================
	CString			GetVTStatus(PlItemState& ItemState);
	CString			GetVTStatus();

	VTRStatusCond	GetVTStatusCond();
	z				IsBitSet(b bDataByte, i iBitNum);
private:
	
	void			CheckVSTS3();
	void			CheckVSTS2();
	void			CheckVSTS1();
	void			CheckVSTS0();
	//============================================
	// Functions
	//============================================
	z				IsMaterialErr(){return CDataConversion::IsBitSet(m_bListStatus, STAT_MATERIAL_ERROR);}
	z				IsTitleMismatch(){return CDataConversion::IsBitSet(m_bListStatus, STAT_TITLE_MISM);}
	//---------------------------------------------------------------------------------
	z				IsVTRLocal(){return CDataConversion::IsBitSet(m_baStatus[0], VSTS0_VTR_LOCAL);}
	z				IsLineEvent(){return CDataConversion::IsBitSet(m_baStatus[0], VSTS0_LINE_EVENT);}
	z				IsVtrAssigned(){return(m_baStatus[0] & 0x0f);};
	//---------------------------------------------------------------------------------
	z				IsOnAir(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_ON_AIR);}
	z				IsLoadErr(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_LOAD_ERR);}
	z				IsHardErr(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_HARD_ERR);}
	z				IsColourFrameLock(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_COLOUR_FRAME_LOCK);}
	z				IsServoLock(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_SERVO_LOCK);}
	z				IsNotCued(){return CDataConversion::IsBitSet(m_baStatus[1], VSTS1_NOT_CUED);}
	//---------------------------------------------------------------------------------
	z				IsStill(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_STILL);}
	z				IsPlay(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_PLAY);}
	z				IsCued(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_CUED);}
	z				IsCueing(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_CUEING);}
	z				IsThread(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_THREAD);}
	z				IsTapeOut(){return CDataConversion::IsBitSet(m_baStatus[2], VSTS2_TAPE_OUT);}
	//---------------------------------------------------------------------------------
	z				IsStandbyOn(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_STDBY_ON);}
	z				IsTensionRel(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_TENSION_REL);}
	z				IsFrSOM2EOM(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_FROM_SOM_2_EOM);}
	z				IsEOMOverrun(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_EOM_OVERRUN);}
	z				IsEject(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_EJECT);}
	z				IsRewind(){return CDataConversion::IsBitSet(m_baStatus[3],VSTS3_REWIND);}
	//---------------------------------------------------------------------------------
	z				NoStatusFound(){return (m_wStringTableId == IDS_STATUS_UNKNOWN);}
	//============================================
	// Private Data 
	//============================================
	b				m_bEventType;
	CString			m_sStatus;
	b				m_bListStatus;
	b				m_baStatus[ NUM_STATUS_BYTES ];
	WORD			m_wStringTableId;
	z				m_zTrace;
	i				m_iSequence;
	VTRStatusCond	m_VTRStatCond;
	PlItemState		m_ItemState;
};



#endif
