/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmariver.h
** Last Modified Date: 
** Last Version:    
** Descriptions: The qmadriver.h is used to customer driver interface
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-26
** Version:      0.1
** Descriptions: 
**
**-------------------------------------------------------------------------------------
** Modified by:      SW 
** Modified Date :   
** Version:          1.60 
** Descriptions:     For QN8610 B1 (R1028)
**
**************************************************************************************************/
#ifndef _QMADRIVER_H_
#define _QMADRIVER_H_

// specify ATV chip
#define QN8610
       
// external driver interface 
// logical layer

typedef unsigned char  UINT8; 
#ifndef QN_SIM
typedef unsigned long   UINT32;            
#else
typedef unsigned int    UINT32;            
#endif 
typedef unsigned short UINT16;            

#ifdef QN_SIM
#define SPRINTF(a)	    TRACE a
#define MPRINTF(b,a)    TRACE a
#else
#define SPRINTF(a)  
#define MPRINTF(b,a)
#endif 

#ifdef QN_SIM
#define DLL_214x                 0
#define DLL_2440                 1
#endif 

// clock setting
#define QMA_USING_CRYSTAL        0
#define QMA_USING_EXTERNAL_CLOCK 1
#define QMA_RECTANGULAR_WAVEFORM 0x40
#define QMA_SINUSOID_WAVEFORM    0x80
#define QMA_SYS_CLK_0            32768
#define QMA_SYS_CLK_1            26000000
#define QMA_SYS_CLK_2            27000000
#define QMA_SYS_CLK_3            24000000

// testing selection
#define QMA_TEST_INIT_CH         0
#define QMA_TEST_TONE            1 
#define QMA_TEST_COLOR_BAR       2 
#define QMA_TEST_DETECT_COLOR    3
       
// YUV format selection: BT601, BT656
// for mobile phone, normally it's BT601
#define QMA_TESTING_YUV_BT601    1       
#define QMA_TESTING_YUV_BT656    0        
       
// Display output: TV composite video output (connect to TV composite video input), YUV output (connect to LCD driver)
// for testing purpose
#define QMA_TESTING_OUTPUT_TV    0        
#define QMA_TESTING_OUTPUT_LCD   0        
       
// TV type: PAL, NTSC
// for testing purpose
#define QMA_TESTING_TYPE_PAL     1       
#define QMA_TESTING_TYPE_NTSC    0        

// ATV CCA sensitivity
// 0: normal sensitivity
// 1: high sensitivity, will get more TV channels but increase possibility to get fake channel 
#define QMA_CCA_SENSITIVITY_HIGH  0

// Color Kill Feature
// When do "Sensitivity Testing", please set it to "0" 
#define QMA_COLOR_KILL_ENABLE     1

//for SPRD skipping the LCD refresh interference, in general, it is not used
#define QMA_VIDEO_HOLD_RELEASE_IS_USED 0
#define QMA_LCD_REFRESH_DELAY_IS_USED 0
#define QMA_BB_REFRESH_DELAY_TIME_PAL 19
#define QMA_BB_REFRESH_DELAY_TIME_NTSC 1

// audio auto detection feature
// 0: disable this feature, TV mode setting depend on country setting
// 1: enable this feature, TV mode setting depend on country setting and auto detection
//    pros: it's useful for "country boundary crossing" area which has different TV mode
//    cons: take more time to tune to a channel and some else
#define QMA_AUDIO_AUTO_DETECTION   0

// tv auto detection for PAL&SECAM
// 0: disable
// 1: enable
#define QMA_TV_AUTO_DETECTION     0

#define QMA_ATV_REDUCE_SHAKE	1

// Elimination of vertical stripe
#define QMA_ELIMINATION_VERTICAL_STRIPE   0   // 0: disable, 1: enable
#define QMA_ELIMINATION_VERTICAL_STRIPE_CH_0    6725  
// ELIMINATION_VERTICAL_STRIPE_CH_N  ... could be more
#define QMA_ELIMINATION_VERTICAL_STRIPE_IDX   3 // to be specified for specific platform, it's HW platform dependent

#define QMA_FMCH_COUNT_MAX         80
#define QMA_CH_COUNT_MAX           69 
#define QMA_REG_NUM_MAX            229
#define QMA_VHF_CH_MAX             63       
       
//setting mode status to all whole system
#define QNM_DEV_ATV                0
#define QNM_DEV_FM                 1

#define QNM_MODE_WORKING           1
#define QNM_MODE_SLEEP             2
       
//setting the region code for different frequencies of ATV and FM

//ASIA 
/*********** East Asia **********/
#define QNM_REGION_CHINA           0
#define QNM_REGION_SHENZHEN        1
#define QNM_REGION_HONGKONG        2
#define QNM_REGION_JAPAN           4
#define QNM_REGION_KOREA           QNM_REGION_USA
#define QNM_REGION_TAIWAN          QNM_REGION_USA
#define QNM_REGION_NORTH_KOREA     QNM_REGION_RUSSIA
#define QNM_REGION_MONGOLIA        QNM_REGION_RUSSIA
#define QNM_REGION_MACAU           QNM_REGION_HONGKONG

/*********** Southeast Asia ********/
#define QNM_REGION_PHILIPPINES     QNM_REGION_USA
#define QNM_REGION_VIETNAM         5
#define QNM_REGION_SINGAPORE       QNM_REGION_WESTERNEUROPE
#define QNM_REGION_BRUNEI          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_THAILAND        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_CAMBODIA        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_INDONESIA       QNM_REGION_WESTERNEUROPE
#define QNM_REGION_MALAYSIA        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_BURMA           7//myanmar
#define QNM_REGION_LAOS            QNM_REGION_WESTERNEUROPE
#define QNM_REGION_EAST_TIMOR      QNM_REGION_WESTERNEUROPE

/*********** Western Asia   ********/
#define QNM_REGION_UAE             QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ABU_DHABI	   QNM_REGION_WESTERNEUROPE
#define QNM_REGION_BAHRAIN		   QNM_REGION_WESTERNEUROPE
#define QNM_REGION_AFGHANISTAN     QNM_REGION_WESTERNEUROPE
#define QNM_REGION_PAKISTAN        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_YEMEN           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_IRAQ            13
#define QNM_REGION_IRAN            QNM_REGION_IRAQ
#define QNM_REGION_SAUDIARABIA     QNM_REGION_IRAQ
#define QNM_REGION_JORDAN          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_KUWAIT          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_LEBANON         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_MALDIVES        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_QATAR           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SYRIA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ISRAEL          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_OMAN            QNM_REGION_WESTERNEUROPE//

/*********** South Asia  ***********/
#define QNM_REGION_SRILANKA        QNM_REGION_WESTERNEUROPE//
#define QNM_REGION_INDIA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_BANGLADESH      QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SRI_LANKA       QNM_REGION_WESTERNEUROPE

//African
#define QNM_REGION_NAMIBIA         QNM_REGION_SHENZHEN
#define QNM_REGION_SOUTHAFRICA     QNM_REGION_HONGKONG
#define QNM_REGION_SUDAN           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_CONGO           QNM_REGION_RUSSIA
#define QNM_REGION_EGYPT           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_KENYA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ALGERIA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_LIBERIA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_LIBYA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_NIGERIA         QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_SOMALIA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_TANZANIA        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_TUNISIA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_GHANA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ZIMBABWE        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_CAMEROON        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_UGANDA          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ZAMBIA          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_GAMBIA          QNM_REGION_HONGKONG
#define QNM_REGION_SIERRALEONE     QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SWAZILAND       QNM_REGION_WESTERNEUROPE
#define QNM_REGION_MADEIRA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SENEGAL         QNM_REGION_RUSSIA
#define QNM_REGION_MADAGASCAR      QNM_REGION_RUSSIA
#define QNM_REGION_MOROCCO         QNM_REGION_IRAQ
#define QNM_REGION_MALI            QNM_REGION_RUSSIA
#define QNM_REGION_ANGOLA          QNM_REGION_HONGKONG
#define QNM_REGION_BENIN           QNM_REGION_RUSSIA
#define QNM_REGION_BURKINA_FASO    QNM_REGION_RUSSIA
#define QNM_REGION_CAPE_VERDE      QNM_REGION_WESTERNEUROPE
#define QNM_REGION_COTE_DIVOIRE    QNM_REGION_RUSSIA
#define QNM_REGION_GUINEA          QNM_REGION_RUSSIA
#define QNM_REGION_GUINEA_BISSAU   QNM_REGION_HONGKONG
#define QNM_REGION_MAURITANIA      QNM_REGION_IRAQ
#define QNM_REGION_NIGER           QNM_REGION_RUSSIA
#define QNM_REGION_TOGO            QNM_REGION_RUSSIA

// North American
#define QNM_REGION_USA             3
#define QNM_REGION_CANADA          QNM_REGION_USA
#define QNM_REGION_GUAM            QNM_REGION_USA
//Latin American
#define QNM_REGION_MEXICO          QNM_REGION_USA
#define QNM_REGION_JAMAICA         QNM_REGION_USA
#define QNM_REGION_COSTARICA       QNM_REGION_USA
#define QNM_REGION_HONDURAS        QNM_REGION_USA
#define QNM_REGION_PUERTO_RICO     QNM_REGION_USA
#define QNM_REGION_NICARAGUA       QNM_REGION_USA
#define QNM_REGION_GUATEMALA       QNM_REGION_USA
#define QNM_REGION_DOMINICA        QNM_REGION_USA
#define QNM_REGION_ANTIGUA         QNM_REGION_USA
#define QNM_REGION_CUBA            QNM_REGION_USA
#define QNM_REGION_PANAMA          QNM_REGION_USA
#define QNM_REGION_ELSALVADOR      QNM_REGION_USA
#define QNM_REGION_HAITI           QNM_REGION_USA
#define QNM_REGION_MONTSERRAT      QNM_REGION_USA
#define QNM_REGION_GUYANA          QNM_REGION_USA
// South American
#define QNM_REGION_SURINAM         QNM_REGION_USA
#define QNM_REGION_CHILE           QNM_REGION_USA
#define QNM_REGION_VENEZUELA       QNM_REGION_USA
#define QNM_REGION_BRAZIL          8
#define QNM_REGION_ARGENTINA       9
#define QNM_REGION_PARAGUAY        QNM_REGION_ARGENTINA
#define QNM_REGION_URUGUAY         QNM_REGION_ARGENTINA
#define QNM_REGION_PERU            QNM_REGION_USA
#define QNM_REGION_COLUMBIA        QNM_REGION_USA
#define QNM_REGION_BOLIVIA         QNM_REGION_USA
#define QNM_REGION_ECUADOR         QNM_REGION_USA
// Europe
#define QNM_REGION_WESTERNEUROPE   6
#define QNM_REGION_TURKEY          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_PORTUGAL        QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SWEDEN          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SPAIN           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_UK              QNM_REGION_HONGKONG
#define QNM_REGION_RUSSIA          12
#define QNM_REGION_BULGARIA        QNM_REGION_RUSSIA
#define QNM_REGION_HUNGARY         QNM_REGION_RUSSIA
#define QNM_REGION_POLAND          QNM_REGION_RUSSIA
#define QNM_REGION_ROMANIA         QNM_REGION_VIETNAM 
#define QNM_REGION_GERMAN          QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_AUSTRIA         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_BELGIUM         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_DENMARK         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_FINLAND         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_ICELAND         QNM_REGION_WESTERNEUROPE
#define QNM_REGION_MACEDONIA       QNM_REGION_WESTERNEUROPE
#define QNM_REGION_NALTA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_NORWAY          QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SWITZERLAND     QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_GREENLAND       QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_CYPRUS          QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_MALTA           QNM_REGION_WESTERNEUROPE
#define QNM_REGION_HOLAND          QNM_REGION_WESTERNEUROPE 
#define QNM_REGION_AZERBAIJAN      QNM_REGION_RUSSIA
#define QNM_REGION_CZECH_REPUBLIC  QNM_REGION_RUSSIA
#define QNM_REGION_GEORGIA         QNM_REGION_RUSSIA
#define QNM_REGION_UKRAIN          QNM_REGION_RUSSIA 
#define QNM_REGION_GREECE          QNM_REGION_IRAQ
#define QNM_REGION_IRELAND         QNM_REGION_HONGKONG
#define QNM_REGION_ITALY           14
//Oceania
#define QNM_REGION_AUSTRALIA       10
#define QNM_REGION_NEWZEALAND      11
#define QNM_REGION_FIJI_ISLANDS    QNM_REGION_WESTERNEUROPE
#define QNM_REGION_SOLOMON_ISLANDS QNM_REGION_WESTERNEUROPE

#define QNM_REGION_COUNT           15
#define QNM_TOTAL_REGION           142

//setting the current antenna type used
#define QNM_ATV_ANTN_UHF           1
#define QNM_ATV_ANTN_VHFIII        2
#define QNM_ATV_ANTN_VHFI          3
#define QNM_FM_ANTN                4

typedef struct {
    UINT32 sysClk;     // system clock frequency, eg: for 26Mhz, it's 26000000
	UINT16 curFreq;
    UINT16 chStart;
	UINT16 chStop;
	UINT8  chStep;
	UINT8  autoScanAll;
	UINT8  chCount;
	UINT32 chList[QMA_FMCH_COUNT_MAX];
}S_FM_DRIVER;

typedef struct {	
    UINT16 chipID;
    UINT32 sysClk;     // system clock frequency, eg: for 26Mhz, it's 26000000
	UINT8 dev;
	UINT8 mode;
    UINT32 curFreq;
    UINT32 prevFreq;
    UINT32 chList[QMA_CH_COUNT_MAX];       
	UINT16 regionCode;
	UINT8 rgnChTotalCount;
    UINT8 chCount;
    UINT8 IMR; 
    UINT8 tvBW;
	UINT8 volume;
	UINT8 isPs;   // 0: PAL SECAM; 1: NTSC,PAL M
	S_FM_DRIVER *fm;
}S_MA_DRIVER;

typedef struct {
    UINT32 freqBandLow;
    UINT32 freqBandHigh;
    UINT8  conf1;
    UINT8  conf2; 
    UINT8  conf3;
    UINT8  conf4;
    UINT16  p;
    UINT8  n;
    UINT8  m;
}S_MA_FREQ_SETTING;


typedef struct
{
	UINT8 chNo;
	UINT32 freq;
	UINT8  tvStd;
}S_MA_CHMAP;

typedef struct {
	UINT16  pxlNum;
	UINT16  lineNum;
	UINT8   xScale;
	UINT8   yScale;
	UINT8   hScaleRatio;
	UINT8   outLineDly;
}S_MA_RESOLUTION;

typedef struct {
	UINT8 reg0x24;
	UINT8 reg0xad;
	UINT8 reg0xae;
	UINT8 reg0x25;
	UINT8 reg0x26;
	UINT8 reg0x27;
	UINT8 reg0x28;
}S_MA_TF_COEFF;

extern const S_MA_CHMAP ATV_ChMap[QNM_REGION_COUNT][120];

#if !defined(DLL_DEMO)
extern void QMA_Delay(UINT16 ms);
extern UINT16 QMA_GetChipVersion(void);
extern UINT8 QMA_Testing(S_MA_DRIVER *qma,UINT8 optiontype);
extern UINT8 QMA_Init(S_MA_DRIVER *qma,UINT8 dev);
extern void QMA_SetSysMode(S_MA_DRIVER *qma,UINT8 mode);
extern UINT8 QMA_SetRegion(S_MA_DRIVER *qma, UINT16 regionCode);
extern UINT8 QMA_GetAntennaType(S_MA_DRIVER *qma);
extern UINT32 QMA_RXSeekCH(S_MA_DRIVER *qma, UINT32 start, UINT32 stop,UINT8 up);						  
extern UINT8 QMA_RXSeekCHAll(S_MA_DRIVER *qma, UINT32 start, UINT32 stop);
extern UINT8 QMA_TuneToCH(S_MA_DRIVER *qma, UINT32 ch);
extern UINT32 QMA_RXValidCH(S_MA_DRIVER *qma, UINT32 chfreq);
extern void QMA_ConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option);
extern void QMA_ConfigVideo(S_MA_DRIVER *qma, UINT8 optiontype, UINT16 option);
extern void QMA_ConfigChrmAGC(S_MA_DRIVER *qma);
extern UINT8 QMA_GetRSSI(S_MA_DRIVER *qma);
extern void QMA_SetTestMode(S_MA_DRIVER *qma,UINT8 optiontype, UINT8 option);
extern void QMA_SetTestToneFreq( UINT8 value);
extern void QMA_SetRefreshDelay(S_MA_DRIVER *qma);
#endif 

extern UINT8 QMA_ReadReg(UINT8 adr);
extern UINT8 QMA_WriteReg(UINT8 adr, UINT8 value); 
#endif //_QMARIVER_H_