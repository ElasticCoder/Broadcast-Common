//
// Amplicon NT Series 200 DIO Driver
//
// $Id$
//
// Written for Amplicon LiveLine Ltd, Brighton, UK
//
// by MEV Ltd
// Copyright (c) 1996-1997 MEV Limited
// 33 Willis Road, Cale Green, Stockport, SK3 8HQ
//  +44 161 477 1898
//  @mev-ltd.demon.co.uk
//
// Based on (with copyright retained by)
//       DIO_CT.H - 200 Series DIO & TC Library Header        
//                Jonathan East  (Amplicon LiveLine Ltd)  18-03-96
//
// MODULE CONTENTS
//
//      Header File
//
// DESCRIPTION
//
//      Header File for Amplicon Series 200 DIO driver 
//      Defines relevant to IOCTL drivers
//
// ENVIRONMENT 
//      Compiler:      Microsoft Visual C++ V4.0
//      Target System: PC, running MS-Windows NT v4.0
//
// AMENDMENT RECORD
//
// $Log$
//
// Started 31 October 1996
//

//
// Defines for user API (compatible with 16 bit DLL version)
//
// Boards supported...

#define PC212E          212
#define PC214E          214
#define PC215E          215
#define PC218E          218
#define PC272E          272

// Group base address offsets...
#define PPIX            0               // PPI X
#define PPIY            0x08            // PPI Y
#define PPIZ            0x10            // PPI Z

#define X1              0               // Timer X1
#define X2              0x04            // Timer X2
#define Y1              0x08            // Timer Y1
#define Y2              0x0c            // Timer Y2
#define Z1              0x10            // Timer Z1
#define Z2              0x14            // Timer Z2

#define MODE0           0
#define MODE1           1
#define MODE2           2
#define MODE3           3
#define MODE4           4

// PPI Ports...
#define PORTA           0
#define PORTB           1
#define PORTC_L         2
#define PORTC_U         3

// PPI Control settings
#define PPI_TRISA       0x10   // PPI port tristate enable masks
#define PPI_TRISC_H     0x8
#define PPI_TRISB       0x2
#define PPI_TRISC_L     0x1
#define PPI_CMD         0x80

// TC Control settings
#define READBACK        0x3
#define LATCHSTATUS     0x2
#define LATCHCOUNT      0x1
#define LATCHBOTH       0x0

// mask to take channels out of 
// readback ctrl byte
#define CTRDCHANMASK    0xF1;   

#define LATCHED         0x0
#define LOWBYTEONLY     0x1
#define HIGHBYTEONLY    0x2
#define LOWHIGHBYTE     0x3

#define BCD             1
#define BINARY          0


// Timer counter clock sources...
#define CLK_Z1          0
#define CLK_Z2          0x20
#define CLK_CLK         0
#define CLK_10MHZ       1
#define CLK_1MHZ        2
#define CLK_100KHZ      3
#define CLK_10KHZ       4
#define CLK_1KHZ        5
#define CLK_OUTN_1      6
#define CLK_EXT         7

// Timer counter gate sources...
#define GAT_Z1          0
#define GAT_Z2          0x20
#define GAT_VCC         0
#define GAT_GND         1
#define GAT_EXT         2
#define GAT_OUTN_2      3

//
// Error codes
//
#define NOTAVAILABLE    0
#define ISAVAILABLE     1

#define OK              0
#define ERRSUPPORT     -1
#define ERRBASE        -2
#define ERRIRQ         -3
#define ERRHANDLE      -4
#define ERRCHAN        -5
#define ERRDATA        -6
#define ERRRANGE       -7
#define ERRMEMORY      -8
#define ERRBUFFER      -9
#define ERRPC226       -10

//
// type define for functions that will be called back by
// the interrupt service when the interrupt occurs in
// the general form and in the form specific to TCsetUserCO
//
typedef VOID (CALLBACK *TTCCALLBACK)( const short h
                                    , const short wParam
                                    , const ULONG lparam
                                    );

typedef VOID (CALLBACK *TTCUUSERCOCALLBACK)( const short h
                                    , const short hCO
                                    , const ULONG value
                                    );

//
// Function Prototypes
//

//
// Registration
//
short  WINAPI registerBoard (short model, short baseAddr, short irq);
short  WINAPI GetBoardModel (short h);
short  WINAPI FreeBoard(short h);
short  WINAPI setIntMask( short h, short mask );
short  WINAPI getIntStat( short h );
short  WINAPI enableInterrupts( short h );
short  WINAPI disableInterrupts( short h );


//
// Memory
//
short WINAPI allocateIntegerBuf( long nItems );
short WINAPI allocateLongBuf( long nItems );
short WINAPI freeIntegerBuf( short hiB );
short WINAPI freeLongBuf( short hiB );
short WINAPI readIntegerBuf(short hiB, long item, short *pResult );
short WINAPI readLongBuf( short hiB, long item, long *pResult );
short WINAPI writeIntegerBuf( short hiB, long item, short data );
short WINAPI writeLongBuf( short hiB, long item, long data );
short WINAPI copyToIntegerBuf( short hiB, long start, long nItems, short *pSrc );
short WINAPI copyToLongBuf( short hiB, long start, long nItems, long *pSrc );
short WINAPI copyFromIntegerBuf( short hiB, long start, long nItems, short *pDest );
short WINAPI copyFromLongBuf( short hiB, long start, long nItems, long *pDest );

short WINAPI getIntegerIntItem( short hiB, long *item );
short WINAPI getLongIntItem( short hlB, long *item );

//
// TC functions
//
short WINAPI TCisAvailable (short h, short chip, short chan);
short WINAPI TCfreeResource (short h, short chip, short chan);
short WINAPI TCsetClock(short h, short chip, short chan, short clk);
short WINAPI TCsetGate(short h, short chip, short chan, short gate);
short WINAPI TCsetMode(short h, short chip, short chan, short mode);
short WINAPI TCgetStatus(short h, short chip, short chan);
short WINAPI TCsetCount(short h, short chip, short chan, long count);
short WINAPI TCgetCount(short h, short chip, short chan, long *count);
short WINAPI TCgetUpCount(short h, short chip, short chan, long *count);
short WINAPI TCsetDiffCounters(short h, short chip1, short chan1, short clk1, short gat1, short chip2, short chan2, short clk2, short gat2);
short WINAPI TCgetDiffCount (short h, short hDiff, long *diff);
short WINAPI TCgetRatio (short h, short hDiff, float *ratio);
short WINAPI TCfreeDiffCounters (short h, short hDiff);
short WINAPI TCsetMonoShot (short h, short chip, short chan, float duration);
short WINAPI TCsetAstable (short h, short chip1, short chan1, short chip2, short chan2, float freq, float msratio);
short WINAPI TCfreeAstable (short h, short hAstable);
short WINAPI TCgetExtPeriod (short h, short chip1, short chan1, float *pPeriod);
short WINAPI TCgetExtFreq(short h, short chip, short chan, float *fPtr);
short WINAPI TCgenerateFreq (short h, short chip1, short chan1, float freq);
short WINAPI TCgenerateAccFreq (short h, short chip1, short chan1, float freq);
short WINAPI TCmultiplyFreq ( short h, short ipChip, short ipChan,
                                short opChip, short opChan, float factor);
short WINAPI TCdivideFreq (short h, short ipChip, short ipChan, short opChip, short opChan, float divisor);
short WINAPI TCsetStopwatch (short h, short chip, short chan);
short WINAPI TCstartStopwatch (short h, short hS);
short WINAPI TCgetElapsedTime (short h, short hS, long *lPtr);
short WINAPI TCfreeStopwatch (short h, short hS);
short WINAPI TCgetTimeStr (long ms, char *strPtr);

short WINAPI TCsetUserInterrupt(short h, TTCCALLBACK pfn, short wParam, short chip, short ISRDATA, short chip1,  short chan1, short chip2, short chan2);
short WINAPI TCfreeUserInterrupt (short h, short hUserInt);

short WINAPI TCsetEventRecorder (short h, short hS, short chip, short hlB);
short WINAPI TCfreeEventRecorder (short h, short hE);
short WINAPI TCsetDCO (short h, short diChip, short diChan, short opChip, short opChan, float udFreq, short udChip, float MinF, float MaxF);
short WINAPI TCsetUserCO(short h, TTCUUSERCOCALLBACK pfn, short opChip, short opChan, float udFreq, short udChip, float MinF, float MaxF);
short WINAPI TCsetUserCOLevel(short h, short hCO, ULONG value);
short WINAPI TCfreeDCO (short h, short hO);

//
// DIO
//
short WINAPI DIOisAvailable (short h, short chip);
short WINAPI DIOsetMode(short h, short chip, short port, short isInput);
short WINAPI DIOsetChanWidth (short h, short chip, short numBits);
short WINAPI DIOsetData(short h, short chip, short chan, long data);
short WINAPI DIOgetData(short h, short chip, short chan, long *data);

short WINAPI DIOsetSwitchMatrix( short h, short order );
short WINAPI DIOgetSwitchStatus( short h, short xcoord, short ycoord );
short WINAPI DIOfreeSwitchMatrix (short h);

































