/*
 ****************************************************************************
 * (C) Copyright 1999, Arcom Control Systems Inc.                           $
 *                                                                          $
 ****************************************************************************
 */

// Include this file only once
#ifndef  __ARCBOARD_H_
#define  __ARCBOARD_H_

//***************************************************************************
//************************* Global Constants ********************************
//***************************************************************************
// Version
#define  AN_VERSION           0                //Version Number
#define  AN_RELEASE           1                //Release Number   

// LED control
#define  AN_LED_OFF          (UCHAR) 0x00      //set LED OFF
#define  AN_LED_ON           (UCHAR) 0x01      //set LED ON

// Digital output control
#define  AN_DISABLE          (UCHAR) 0x00      //disbale Output groups
#define  AN_ENABLE           (UCHAR) 0x01      //enable Output groups
 
// Digital I/O groups
#define  AN_GROUP_0          (UCHAR) 0x00      //digital group 0
#define  AN_GROUP_1          (UCHAR) 0x01      //digital group 1
#define  AN_GROUP_2          (UCHAR) 0x02      //digital group 2
#define  AN_GROUP_3          (UCHAR) 0x03      //digital group 3
#define  AN_GROUP_4          (UCHAR) 0x04      //digital group 4

// Counters
#define  AN_COUNTER0         (UCHAR) 0x00      //counter no 0
#define  AN_COUNTER1         (UCHAR) 0x01      //counter no 1 
#define  AN_COUNTER2         (UCHAR) 0x02      //counter no 2
#define  AN_COUNTER3         (UCHAR) 0x03      //counter no 3
#define  AN_COUNTER4         (UCHAR) 0x04      //counter no 4 
#define  AN_COUNTER5         (UCHAR) 0x05      //counter no 5
#define  AN_COUNTER6         (UCHAR) 0x06      //counter no 6
#define  AN_COUNTER7         (UCHAR) 0x07      //counter no 7 
#define  AN_COUNTER8         (UCHAR) 0x08      //counter no 8

// Counter modes
#define  AN_CTC_MODE0        (UCHAR) 0x00      //Interrupt on Terminal Count
#define  AN_CTC_MODE1        (UCHAR) 0X02      //Hardware Retriggerable One-Shot
#define  AN_CTC_MODE2        (UCHAR) 0X04      //Rate Generator
#define  AN_CTC_MODE3        (UCHAR) 0X06      //Square Wave Mode
#define  AN_CTC_MODE4        (UCHAR) 0X08      //Software Triggered Strobe
#define  AN_CTC_MODE5        (UCHAR) 0X0A      //Hardware Triggered Strobe

// PCADADIO Dig-IO Nibble configuration
#define  AN_NIB_OUTPUT       (UCHAR) 0X00      //Nibble if for Digital Output
#define  AN_NIB_INPUT        (UCHAR) 0X01      //Nibble is for Digital Input
#define  AN_READ_EASY        (UCHAR) 0x01      //Read ADC using Easy Reads
#define  AN_READ_BLOCK       (UCHAR) 0x00      //Read ADC not using Easy Read

// APCI-COM4 serial parameters
#define COM4_BAUD_50    0x0900
#define COM4_BAUD_300   0x0180
#define COM4_BAUD_600   0x00C0
#define COM4_BAUD_1200  0x0060
#define COM4_BAUD_2400  0x0030
#define COM4_BAUD_4800  0x0018
#define COM4_BAUD_9600  0x000C
#define COM4_BAUD_19200 0x0006
#define COM4_BAUD_38400 0x0003
#define COM4_BAUD_57600 0x0002
#define COM4_BAUD_115200 0x0001

#define COM4_WORD_5     0x00
#define COM4_WORD_6     0x01
#define COM4_WORD_7     0x02
#define COM4_WORD_8     0x03

#define COM4_PARITY_EVEN    0x18
#define COM4_PARITY_ODD     0x08
#define COM4_PARITY_NONE    0x00

#define COM4_STOP_1     0x00
#define COM4_STOP_15    0x04
#define COM4_STOP_2     0x04

#define COM4_RS232_MANUAL   1
#define COM4_RS232_AUTO     2
#define COM4_RS485          3



//***************************************************************************
//************************* Error Codes *************************************
//***************************************************************************
// General error codes
#define  AN_SUCCESS                  (ULONG) 0xFFFFFFFF //Success
#define  AN_ERR_EXCEPTION            (ULONG) 101      //An internal exception occured  
#define  AN_ERR_KERNELDRIVER_FAILED  (ULONG) 102
#define  AN_ERR_REGISTRYREAD_FAILED  (ULONG) 103      //Could not read info from registry
#define  AN_ERR_INVALID_DEVICE       (ULONG) 104      //an invalid device number was specified
#define  AN_ERR_NO_BOARD_AT_ADDRESS  (ULONG) 105      //Specified board is not at the base address

// Error codes - DIO
#define  AN_ERR_BAD_DIO_GROUPNO      (ULONG) 201      //bad digital i/o group number
#define  AN_ERR_BAD_DIO_BITNO        (ULONG) 202      //bad digital i/o bit number
#define  AN_ERR_BAD_COUNTERNO        (ULONG) 203      //bad counter number
#define  AN_ERR_BAD_COUNTERMODE      (ULONG) 203      //bad counter mode

// Error codes - PCADADAIO
#define  AN_ERR_BAD_DIGIOCONFIG      (ULONG) 401      //bad nibble configuration
#define  AN_ERR_BAD_ANA_TIMEOUT      (ULONG) 402      //timeout must not be zero
#define  AN_ERR_BAD_ANA_SAMPLERATE   (ULONG) 403      // sample rate must be > 20 microseconds
#define  AN_ERR_BAD_ANA_CHANNEL      (ULONG) 404      //bad channel number

//Error code for PCSER4
#define  AN_ERR_BAD_COMPORT_NUMBER		(ULONG) 601      //Bad Com Port specified (must be 0,1, 2 or 3)
#define  AN_ERR_BAD_PARITY			    (ULONG) 602      //Bad parity specified (must be 'n','o' or 'e')
#define  AN_ERR_BAD_DATABITS		    (ULONG) 603      //Bad data-bits specified (must be 7 or 8)
#define  AN_ERR_BAD_STOPBITS		    (ULONG) 604      //Bad Stop bits specified (must be 1 or 2)
#define  AN_ERR_NO_BOARDS_FREE			(ULONG) 605      //Max limit of serial ports in system reached.
#define  AN_ERR_BAD_COM_TIMEOUT			(ULONG) 606      //Xmit timeout occured
#define  AN_ERR_BUFFER_EMPTY			(ULONG) 607      //ADC Buffer contains no data
#define  AN_ERR_BUFFER_NOT_EMPTY        (ULONG) 608      //ADC buffer has valid data


//****************************************************************************
//*********************** Function Prototypes ********************************
//****************************************************************************
#define CCONV __stdcall 
# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */
//****************************************************************************

//PciCOM4 functions
ULONG CCONV   PciCOM4_Init (ULONG nDevice);
ULONG CCONV   PciCOM4_Close(ULONG nDevice);
ULONG CCONV   PciCOM4_PortWrite (ULONG nDevice, ULONG ulChannel, UCHAR cRegister, UCHAR cValue);
ULONG CCONV   PciCOM4_PortRead  (ULONG nDevice, ULONG ulChannel, UCHAR cRegister, PUCHAR pcValue);
ULONG CCONV   PciCOM4_Open(ULONG nDevice, 
                            ULONG ulChannel, 
                            ULONG unBaud, 
                            UCHAR cParity, 
                            UCHAR cDatabits, 
                            UCHAR cStopbits, 
                            UCHAR cHandshakeMode);
ULONG CCONV   PciCOM4_GetTXBuffCount(
                            ULONG   nDevice, 
                            ULONG   ulChannel,
                            ULONG   *RingBufferSize,
                            ULONG   *TransmitCount);
ULONG CCONV   PciCOM4_GetRXBuffCount(
                            ULONG   nDevice, 
                            ULONG   ulChannel,
                            ULONG   *RingBufferSize,
                            ULONG   *ReceiveCount);
ULONG CCONV   PciCOM4_RXFlush(ULONG   nDevice,  ULONG   ulChannel);
ULONG CCONV   PciCOM4_TXFlush(ULONG   nDevice,  ULONG   ulChannel);
ULONG CCONV   PciCOM4_Put(ULONG nDevice, ULONG ulChannel, PUCHAR lpBuffer, PULONG pulLen);
ULONG CCONV   PciCOM4_Get(ULONG nDevice, ULONG ulChannel, PUCHAR lpBuffer, PULONG pulLen);
ULONG CCONV   PciCOM4_SetStatusDTR(ULONG nDevice, ULONG ulChannel, ULONG bNewDtrState);
ULONG CCONV   PciCOM4_SetStatusRTS(ULONG nDevice, ULONG ulChannel, ULONG bNewRtsState);
ULONG CCONV   PciCOM4_GetStatusCTS(ULONG nDevice, ULONG ulChannel, ULONG *CtsState);
ULONG CCONV   PciCOM4_GetStatusDCD(ULONG nDevice, ULONG ulChannel, ULONG *DcdState);
ULONG CCONV   PciCOM4_GetStatusDSR(ULONG nDevice, ULONG ulChannel, ULONG *DsrState);
ULONG CCONV   PciCOM4_GetStatusRI(ULONG nDevice, ULONG ulChannel, ULONG *RiState);

// PciSER4 functions
ULONG CCONV   PciSER4_Init (ULONG nDevice);
ULONG CCONV   PciSER4_UserLED (ULONG nDevice, UCHAR cAction);
ULONG CCONV   PciSER4_Open(ULONG nDevice, ULONG ulComPort, ULONG unBaud, UCHAR cParity, UCHAR cDatabits, UCHAR cStopbits);
ULONG CCONV   PciSER4_Close(ULONG nDevice);
ULONG CCONV   PciSER4_Put(ULONG nDevice, ULONG ulComPort, PUCHAR lpBuffer,PULONG pulLen);
ULONG CCONV   PciSER4_Get(ULONG nDevice, ULONG ulComPort, PUCHAR lpBuffer,PULONG pulLen);
ULONG CCONV   PciSER4_SetStatusDTR(ULONG nDevice, ULONG ulComPort, ULONG bNewDtrState,ULONG nWarmUpTime);
ULONG CCONV   PciSER4_SetStatusRTS(ULONG nDevice, ULONG ulComPort, ULONG bNewRtsState, ULONG nWarmUpTime);
ULONG CCONV   PciSER4_GetStatusCTS(ULONG nDevice, ULONG ulComPort, ULONG *CtsState);
ULONG CCONV   PciSER4_GetStatusDCD(ULONG nDevice, ULONG ulComPort, ULONG *CdState);
ULONG CCONV   PciSER4_GetTXBuffCount(
                    ULONG   nDevice, 
                    ULONG   ulComPort,
                    ULONG   *RingBufferSize,
                    ULONG   *TransmitCount);
ULONG CCONV   PciSER4_GetRXBuffCount(
                    ULONG   nDevice, 
                    ULONG   ulComPort,
                    ULONG   *RingBufferSize,
                    ULONG   *ReceiveCount);
ULONG CCONV   PciSER4_RXFlush(ULONG   nDevice,  ULONG   ulComPort);
ULONG CCONV   PciSER4_TXFlush(ULONG   nDevice,  ULONG   ulComPort);
ULONG CCONV   PciSER4_PortWrite (ULONG nDevice, UCHAR cPort, UCHAR cValue);
ULONG CCONV   PciSER4_PortRead  (ULONG nDevice, UCHAR cPort, PUCHAR pcValue);

// PciRELAY8/IN8 functions
ULONG CCONV  PciRELAY8_Init (ULONG nDevice);
ULONG CCONV  PciRELAY8_Close(ULONG nDevice);
ULONG CCONV  PciRELAY8_RelaysEnable (ULONG nDevice, UCHAR cAction);
ULONG CCONV  PciRELAY8_RelayGroupRead (ULONG nDevice, PUCHAR pcValue);
ULONG CCONV  PciRELAY8_RelayGroupWrite (ULONG nDevice, UCHAR cValue);
ULONG CCONV  PciRELAY8_OptoInputGroupRead (ULONG nDevice, PUCHAR pcValue);
ULONG CCONV  PciRELAY8_RelayRead (ULONG nDevice, UCHAR cRelayNo, PUCHAR pcValue);
ULONG CCONV  PciRELAY8_RelayWrite (ULONG nDevice, UCHAR cRelayNo, UCHAR cValue);
ULONG CCONV  PciRELAY8_OptoInputRead (ULONG nDevice, UCHAR cInputNo, PUCHAR pcValue);

// PciIB40 functions 
ULONG CCONV  PciIB40_Init (ULONG nDevice);
ULONG CCONV  PciIB40_Close(ULONG nDevice);
ULONG CCONV  PciIB40_UserLED (ULONG nDevice, UCHAR cAction);
ULONG CCONV  PciIB40_OutputsEnable (ULONG nDevice, UCHAR cAction);
ULONG CCONV  PciIB40_DigGroupRead (ULONG nDevice,  UCHAR cGroup, PUCHAR pcValue);
ULONG CCONV  PciIB40_DigGroupWrite (ULONG nDevice,  UCHAR cGroup, UCHAR cValue);
ULONG CCONV  PciIB40_DigBitRead (ULONG nDevice, UCHAR cBitNo, PUCHAR pcValue);
ULONG CCONV  PciIB40_DigBitWrite (ULONG nDevice, UCHAR cBitNo, UCHAR cValue);
ULONG CCONV  PciIB40_CounterInit (ULONG nDevice, UCHAR cCounterNo, UCHAR cMode, USHORT unNewCount);
ULONG CCONV  PciIB40_CounterRead (ULONG nDevice, UCHAR cCounterNo, PUSHORT punCount);



// PciADADIO functions
ULONG CCONV  PciADADIO_Init (ULONG nDevice,UCHAR bEasy);
ULONG CCONV  PciADADIO_Close(ULONG nDevice);
ULONG CCONV  PciADADIO_UserLED (ULONG nDevice, UCHAR cAction);
ULONG CCONV  PciADADIO_AnalogWrite (ULONG nDevice, UCHAR cChannel, USHORT unValue);
ULONG CCONV  PciADADIO_DigIoConfig (ULONG nDevice, UCHAR cNib0, UCHAR cNib1, UCHAR cNib2, UCHAR cNib3);
ULONG CCONV  PciADADIO_DigitalIoRead (ULONG nDevice, PUSHORT punValue);
ULONG CCONV  PciADADIO_DigitalIoWrite (ULONG nDevice, USHORT unValue);
ULONG CCONV  PciADADIO_DigBitRead (ULONG nDevice, UCHAR cBitNo, PUCHAR pcValue);
ULONG CCONV  PciADADIO_CounterInit (ULONG nDevice, UCHAR cCounterNo, UCHAR cMode, USHORT unNewCount);
ULONG CCONV  PciADADIO_CounterRead (ULONG nDevice, UCHAR cCounterNo, PUSHORT punCount);
ULONG CCONV  PciADADIO_AnalogAdcSetup(ULONG nDevice, UCHAR cChannel, ULONG ulSampleRate, ULONG ulTimeout);
ULONG CCONV  PciADADIO_AnalogStartConversion (ULONG nDevice, ULONG ulNumSamples);
ULONG CCONV  PciADADIO_AnalogGetConversionStatus (ULONG nDevice, PUCHAR pcStatus);
ULONG CCONV  PciADADIO_AnalogReadData (ULONG nDevice, PUSHORT punData);
ULONG CCONV  PciADADIO_AnalogReadEasy(ULONG nDevice, 
                                      UCHAR cChannel, 
                                      ULONG ulCount, 
                                      ULONG ulSampleRate, 
                                      ULONG ulTimeout);

// For Arcom debugging only ---
//ULONG CCONV  PciADADIO_ReadIsrCount (ULONG nDevice, PULONG punValue);



//****************************************************************************
# ifdef __cplusplus
}
# endif /* __cplusplus */
//****************************************************************************
//****************************************************************************
//****************************************************************************

#endif /* __ARCBOARD_H_ */

//eof//
   
