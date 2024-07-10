// Lv24Ekit.h: project include file

//-----------------------------------------------------------------------------
// General include (for type definitions)
//-----------------------------------------------------------------------------
#include "LvErr.h"			// For error codes
#include "fm_common.h"
//-----------------------------------------------------------------------------
// Code options
//-----------------------------------------------------------------------------
// ---- System clock
#define	CF12MHZ

#define USE_LV2400x			// Select LV2400x code (FM)
//#define USE_LCD				// Enable LCD code
//Disable ExtClk by Eric.wei 2005/03/23
//#define USE_EXTCLK			// Enable measuring with external clock code

/*
#ifdef USE_EXTCLK
#define EXT_CLK_12MHZ	((DWORD)12000000)
#define EXT_CLK_32KHZ	((DWORD)32768)
#endif //USE_EXTCLK
*/
//-----------------------------------------------------------------------------
// Firmware version
//-----------------------------------------------------------------------------
#define I3W_MAJ_VER	'0'		// Also modify the version string!!!
#define I3W_MIN_VER	'1'
#define I3W_SUB_VER	'4'
#define STR_VERSION	"V0.14"

#define STR_SIGNON1	"LV2400x Eval.Kit"
#define STR_SIGNON2	" Sanyo/ItoM"
#define STR_USBMODE	"USB mode "

//-----------------------------------------------------------------------------
// Delay time base on 8MHz main clock (tcyc=375ns)/12 MHz (tcyc=250ns) 
// with PRR is 1 (so 1 count is 2*tcyc=750ns for 8MHz/500ns for 12 MHz)
//-----------------------------------------------------------------------------



#define DLT_1ms		1
#define DLT_2ms		2
#define DLT_5ms		5
#define DLT_15ms	15
#define DLT_20ms	20

//-----------------------------------------------------------------------------
// LC87F1564 GPIO Mapping for LV2400x Evaluation kit
//-----------------------------------------------------------------------------
// ----- 3-wire bus 1 -------
//modified by Eric.wei 2005/03/23
#define I3W_PORT			0//_P2		// Use GPIO port 2 to control the 3-wire bus
#define I3W_DIRPORT			1//_P2DDR	// Direction port (Use port A: 0 is input, 1 is output)



//-----------------------------------------------------------------------------
// Macro's
//-----------------------------------------------------------------------------
#define LSB(x)				((BYTE)(x & 0xFF))
#define MSB(x)				((BYTE)(x >> 8))
#define MAKEWORD(byLo, byHi)( ((WORD)byHi<<8) | (byLo) )

#define Hz		1
#define KHz 	1000
#define MHz 	1000000
#define GHz 	1000000000

//-----------------------------------------------------------------------------
// Some type def
//-----------------------------------------------------------------------------
typedef BYTE (*FPTR_WRITE_OSC)(WORD wOscValue);	// Function pointer to be used by LinTuneDac

//-----------------------------------------------------------------------------
// Equations for Callback-function
//-----------------------------------------------------------------------------
// Callback reasons
#define CRSN_STN_CHNG		1	// Station (frequency, fieldstrength...) changed
#define CRSN_SCAN_UPD		2	// Update frequency during scanning
#define CRSN_ASCAN_FOUND 	3	// A station is found during auto scan
#define CRSN_FS_CHNG		4	// Field strength changed
#define CRSN_MS_CHNG		5	// Mono stereo state changed
#define CRSN_AFC_FAILED		6	// AFC failed

//-----------------------------------------------------------------------------
// Time constances used by CountPulse
//-----------------------------------------------------------------------------
#define LV_MSR_TIME_8ms			8000	// us
#define LV_MSR_TIME_32ms		32000	// us
#define LV_MSR_TIME_64ms		64000	// us
#define LV_MSR_TIME_100ms		100000	// us

//-----------------------------------------------------------------------------
// Precision level used by CalculateRfCapOsc
//-----------------------------------------------------------------------------
#define QSSF_PRECISION_NONE	0
#define QSSF_PRECISION_LOW	1
#define QSSF_PRECISION_MED	2
#define QSSF_PRECISION_HIGH	3

//-----------------------------------------------------------------------------
// Audio types used by AudioFeedback
//-----------------------------------------------------------------------------
#define AUDFB_TYPE1		((BYTE)1)	// Short key feedback
#define AUDFB_TYPE2		((BYTE)2)	// Long key feedback

//-----------------------------------------------------------------------------
// Radio default frequencies
//-----------------------------------------------------------------------------
#define DEFAULT_IF_FREQ			((WORD)11000)	// Default IF value (in 10Hz unit)
#define DEFAULT_SD_FREQ			((WORD)38000)	// Default SD value for V6
//-----------------------------------------------------------------------------
// Constances used by SetUpChipMode/GetOutputSelect/SetOutputSelect
//-----------------------------------------------------------------------------
// Equations for virtualizing chip output select (used by SetUpChipMode/GetOutputSelect)
// The whole Output config (WORD) is currently mapped as following:
// Bit [3:0]:	Clock source to be measured (CO_xxx)
// Bit [7:4]:	TBD
// Bit 8:		Want measuring mode 
// Bit 9:		TBD
// Bit 10:		Adjust AFC flag
// Bit 11:		AFC value to be set
// Bit 12:		Adjust chip measure mode flag
// Bit 13:		Chip measure state to be set
// Bit [14:15]:	TBD						NA
#define CO_SELECT_MASK			((WORD)0x0F<<0)			// Bit [3:0]: output select mask. Below outputs are defined:
														// Bit [7:4]: free
#define CHIP_MEASURE_MODE		(WORD)((WORD)1<<8)		// Bit 8: Measuring mode required if 1
														// Bit 9: free
#define CHIP_SET_AFC			(WORD)((WORD)1<<10)		// Bit 10: Adjust AFC mark
#define CHIP_AFC_STATE			(WORD)((WORD)1<<11)		// Bit 11: AFC state
	#define CHIP_AFC_ON			(WORD)(CHIP_SET_AFC|CHIP_AFC_STATE)	// Set AFC on
	#define CHIP_AFC_OFF		(WORD)(CHIP_SET_AFC|0)	// Set AFC off
#define CHR01_MSR_MODE_FLG		(WORD)((WORD)1<<12)		// Bit 12: chip measure mode change mark
#define CHR01_MSR_MODE_ON		(WORD)((WORD)1<<13)		// Bit 13: Chip measure mode bit is on if 1

// definitions of CO_SELECT_MASK
#define CO_NONE			0	// 0: nothing at output
#define	CO_RF_OSC		1	// FREQ1: RF frequency
#define	CO_IF_OSC		2	// FREQ2: IF Osccilator output
#define	CO_SD_OSC		3	// FREQ3: Stereo decoder clock output
#define CO_RD_OSC		4	// FREQ4: RDS Osccilator output

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
#include "LvProto.h"

//-----------------------------------------------------------------------------
// Radio definitions
//-----------------------------------------------------------------------------
// De-emphasis value
#define DEEMP_75 	1
#define DEEMP_50 	0

// Regions
// FM
#define REGION_NONE				0	// None: use hardware limit
#define REGION_MOBILE			1	// FM Mobile (76MHz - 108 MHz, de-emp=50 Step100KHz)
#define REGION_JAPAN			2	// FM Japan  (76MHz - 90MHz, de-emp=50)
#define REGION_US				3	// FM US (same as Europe, De-emp=75)
#define REGION_JAPAN_WIDE		4	// FM Japan wide: Same as Japan, with extra CH1(95.75) CH2(101.75) CH3(107.75)


/*-------------------------------------------------------------------
        Device capability ID (Used by:
        					 GetHwFeatureLimit
        					 SetHwFeatureValue
        					 GetHwFeatureValue)
-------------------------------------------------------------------*/
#define IHCAP_TUNERPWR		0	// ID 0: Tuner's power can be switched on/off

#define IHCAP_VOLUME		1 	// ID 1: Volume level supported
#define IHCAP_AMUTE			2	// ID 2: Audio mute
#define IHCAP_ATREBLE		3	// ID 3: Audio treble supported
#define IHCAP_ABASS			4	// ID 4: Audio bass supported
#define IHCAP_DYNBASSBOOST	5 	// ID 5: Dynamic bass boost supported
#define IHCAP_SMUTE			6 	// ID 6: Audio soft mute supported

//#define IHCAP_DEEMP			7 	// ID 7: Hardware De-emphasis supported
#define IHCAP_STEREO		8 	// ID 8: Stereo supported
#define IHCAP_SOFT_ST		9 	// ID 9: Soft stereo supported

#define IHCAP_RADIOSOURCE	10 	// ID 10: Hardware supports radio source select 
    #define RBAND_OFF   0       // Value of radio source feature
    #define RBAND_FM    1
    #define RBAND_AM    2
#define IHCAP_EXTSOURCE		11 	// ID 11: Hardware supports external source select 
#define IHCAP_BEEPSOURCE	12 	// ID 12: Hardware has pre-programmed beep tone
//#define IHCAP_BEEPBIT		13 	// ID 13: Hardware has beep-bit
#if defined(USE_FM_IRQ)	

#define IHCAP_AFC			14	// ID 14: AFC (automatic frequency control) supported

#endif

#define IHCAP_HPA			15	// ID 15: Hardware headphone amplifier presents
//#define IHCAP_HPBASS		16 	// ID 16: Hardware supports headphone bass level

//#define IHCAP_RDS			17	// ID 17: RDS supported if set
//#define IHCAP_CNT2			18	// ID 18: Counter2 supported if set
//#define IHCAP_AFC2			19	// ID 19: Hardware AFC2 presents (required external clock - limit is this clock)

// Software features
#define IHCAP_SCANLVL		20	// ID 20: Scan level
#define IHCAP_SCANWRP		21	// ID 21: Frequency wrapping by scanning
#define IHCAP_REGION		22	// ID 22: Region
#define IHCAP_BEEPVOL		23	// ID 23: Beep volume for audible feedback
/*
//#ifdef USE_EXTCLK
#define	IHCAP_EXTCLK		24	// ID 24: external clock (0= no external clock, 1= 12MHz, 2=32kHz)
//#endif //USE_EXTCLK
*/
//-----------------------------------------------------------------------------
// Definitions for function's parameters
//-----------------------------------------------------------------------------
// Scan station parameters
#define SCANSTN_DIR			((BYTE)1<<0)	// Bit 0: 1 => scan next, 0 => scan previous
	#define SCANSTN_NEXT	SCANSTN_DIR
	#define SCANSTN_PREV	0
#define SCANSTN_AUTO 		((BYTE)1<<1)	// Bit 1: Auto scan if 1, scan next/previous if 0

// Adjust volume parameters
#define VOLUME_UP			0xFF
#define VOLUME_DOWN			0xFE

// Return values of GetStereoState
#define GSS_MONO		0	// Mono
#define GSS_STEREO		1	// Stereo enabled
#define GSS_STEREO_DET	2	// Stereo enabled and detected

//-----------------------------------------------------------------------------
// Bit definitions for global variables
//-----------------------------------------------------------------------------
// Bit definitions of g_bySwFlag1
/*
#ifdef USE_EXTCLK								// Bit 0-1: external clock setting if USE_EXTCLK is enabled
												// Don't change bit position of external clock (to use 0..2 after AND mask)
#define SF1_EXTCLK				((BYTE)3<<0)	// Bit [1:0]:  external clock setting
	#define EXTCLK_NONE			((BYTE)0<<0)	// 	0: No external clock
	#define EXTCLK_12MHz		((BYTE)1<<0)	// 	1: 12MHz external clock
	#define EXTCLK_32KHz		((BYTE)2<<0)	// 	2: 32kHz external clock (reserved)
#endif //USE_EXTCLK
*/												// Bit 0-1: available if USE_EXTCLK is not defined
#define SF1_PULSE3W_BUSY		((BYTE)1<<2)	// Bit 2: measuring is busy
#define SF1_USB_HOST_CON		((BYTE)1<<3)	// Bit 3: USB host controls the EVK
												// Bit 4: available
#define MUTESRC_APPLICATION		((BYTE)1<<5)	// bit 5: Mute source: application
#define MUTESRC_OPERATION		((BYTE)1<<6)	// bit 6: mute source: operation (scan, set frequency)
#define MUTESRC_VOLUME			((BYTE)1<<7)	// bit 7: mute source: volume
#define MUTESRC_MASK			((BYTE)(MUTESRC_VOLUME|MUTESRC_APPLICATION|MUTESRC_OPERATION))

// Bit definitions of g_byStnFlag
#define STN_WRAP_MODE			((BYTE)3<<0)	// Bit [1:0]:  frequency wrapping mode by Set/Scan station
												// Don't change bit position of wrap mode (to use 0..2 after AND mask)
	#define STNWRAP_NONE		((BYTE)0<<0)	// 	0: No wrap
	#define STNWRAP_ONCE		((BYTE)1<<0)	// 	1: wrap once
	#define STNWRAP_CONT		((BYTE)2<<0)	// 	2: wrap continue
												// Bit 2-3: available
#define STN_VALID				((BYTE)1<<4)	// Bit 4: Valid station mark
												// Bit 5-6: available
// Bit definitions of g_byHwFlag1
#define HF1_NEG_IF_PHASE		((BYTE)1<<0)	// Bit 0: Negative IF phase is 1
#define HF1_HP					((BYTE)1<<1)	// Bit 1: Hardware has headphone amplifier
#define HF1_AM					((BYTE)1<<2)	// Bit 2: Hardware supports AM
//#define HF1_RDS				((BYTE)1<<3)	// Bit 3: Hardware supports RDS
												// Bit 4-7: available
// Bit definitions for g_byLvIrqFlg
#define IFL_CHIP_IRQ_EN			((BYTE)1<<0)	// Bit 0: Chip interrupt is enabled in stand alone mode
#define IFL_CHIP_IRQ_OCC		((BYTE)1<<1)	// Bit 1: Chip interrupt occurred (stand alone mode)
												// Bit 2-3: available
#define IFL_USBIRQ_INSTALLED	((BYTE)1<<4)	// Bit 4: USB host IRQ script is installed
												// Bit 5-7: available

//-----------------------------------------------------------------------------
// 3W specific global variables
//-----------------------------------------------------------------------------
// _define macro declares global variables or refers as extern
#ifdef _MAIN_MODULE
#define _define
#else
#define _define extern
#endif // _MAIN_MODULE

// Radio resource
_define BYTE g_byStnFlag;				// Station flags
_define BYTE g_bySwFlag1;				// Software control flag 1. See bit definitions above
_define BYTE g_byRegion;				// Current radio region
_define BYTE g_byScanLevel;				// Scan level
_define BYTE g_byDynBassBoost;			// Dynamic bass boost setting
_define BYTE g_byBeepVol;				// Beep volume

// LV2400x register resources
_define BYTE  g_byHwFlag1;				// Chip attribute flag 1(hardware dependant)
_define BYTE  g_byLvIrqFlg;				// Several control flags for chip interrupt
_define BYTE  g_byBlock;				// Current block selected
//BYTE g_byaShwRegValue[LVSHADOW_LSIZE];// array to hold the shadowed value - defined in DgtParser.c

// LV2400x frequency resources
_define WORD g_wLastSetRF;		// Last RF frequency which is set (in 10 kHz-unit)
_define WORD g_wLastMsrRF;		// Last measured RF frequency (in 10 kHz-unit)
_define WORD g_wFreqGrid;		// Frequency grid

// LV2400x QSSF resources
typedef struct
{
	WORD CoefLo;
	WORD CoefHi;
} QSSF_ITEM;

_define WORD g_wHwRfLow;			// The minimal hardware RF
_define WORD g_wHwRfHigh;			// The maximal hardware RF
_define WORD g_wDisplayBandLow;		// Band limit low
_define WORD g_wDisplayBandHigh;	// Band limit high
_define QSSF_ITEM Qssf1;			// Coefficient low
_define QSSF_ITEM Qssf2;			// Coefficient high

// Keypad resource
_define BYTE g_byPressedKey;
_define BYTE g_byMenu;
_define BYTE g_byNrPressed;
_define BYTE g_byInpPos;		// current frequency input (cursor) position
_define BYTE g_byaInp[5];		// Array to hold user's input (for setting frequency manually)
_define BYTE g_byCurPreset;		// Current preset location for storing/recalling preset

// 2005-03-03
//#ifdef USE_EXTCLK
_define DWORD g_dwCnt2ExtClk;		// External Clock frequency 
//#endif
//ADD by Eric.wei for auto scan station
#define MAX_STATION 20
typedef struct AutoScanResult_Tag
{
	WORD validCnt;
	WORD station_rfFreq[MAX_STATION];
}AutoScanResult;

_define AutoScanResult g_autoScanResult;
// Free space
//_define char  g_xx[512+181];

/* -------------------------------EOF------------------------------------------ */
