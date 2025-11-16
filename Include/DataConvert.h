///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1997; Sony BPE
// All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
// DataConvert.h
///////////////////////////////////////////////////////////////////////////////

#ifndef __DATACONVERT_H__
#define __DATACONVERT_H__

#include <udp_message.h>


class CDataConversion 
{
public:
	static void SetBit(b& rbDataByte, i iBitNumber);
	static BOOL IsBitSet( b bDataByte, i iBitNum );
	//Constructor #1
	CDataConversion();
	//Constructor #2
	CDataConversion(const b* bpPlayListData, i iByteCnt);
	//
	// All Public Data methods
	//
	void		SetData(const b* bpPlayListData, i iByteCnt);
	b			ConvBCD(const b bBCD_Digit);
	void		ConvBCDDate(CString& sDispDate, i iStartPos, int nBytes);
	void		ConvBCDTime(CString& sDispTime, i iStartPos, int nBytes);
	void		ExtractAscii(CString& sText, i iStart, i iLen);
	void		DecodePulse(CString& sPulse);
	c			ExtractChan(b bChannel);
	b			GetByte(i iBytePos);
	z			GetBool(i iBytePos);
	z			CheckSum(  );
	z			StatusChanged(const b* bpVSTS);
	void		ConvToBCD(const CString sTime, char* sBCDTime);
	CString		DecodeBinNumber(b bBinNumber);
private:
	c			m_cCheckSum;
	b			m_bPlayListMsg[MAX_PLAYLIST_MSG_LEN];
	i			m_iEventType;
	i			m_iByteCnt;
};



#endif



