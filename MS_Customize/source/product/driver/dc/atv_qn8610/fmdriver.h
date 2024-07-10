/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       fmdriver.h
** Last Modified Date: 
** Last Version:    
** Descriptions: The fmdriver.h is used to driver fm function interface
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-26
** Version:      0.1
** Descriptions: 
**
**-------------------------------------------------------------------------------------
** Modified by:     
** Modified Date : 
** Version:         
** Descriptions:     For QN8610 A1 
** Descriptions:   QN8610 A1 version
**
**************************************************************************************************/

#ifndef _FMDRIVER_H_
#define _FMDRIVER_H_

#define FREQ2CHREG(freq)   ((freq-6000)/5)
#define CHREG2FREQ(ch)     (ch*5+6000)

#ifdef QN_SIM
#define FALSE       0
#define TRUE		1
#define S_XDATA 
#endif

enum {
	// Bit[23-16] of the word : RSSI High threshold
    // Bit[15-8] of the word: RSSI Threshold
    // Bit[7-0] of the word: ASYM Threshold    
	// asym: 0~0x3f
	// rssith:0~0x7f;
	// rssihth: 0~0x7f
	CCA_SENSITIVITY_LEVEL_0 = 0x463208,
	CCA_SENSITIVITY_LEVEL_1 = 0x4b320b,//0x4b370b,   
	CCA_SENSITIVITY_LEVEL_2 = 0x4e320e,// normally work
	CCA_SENSITIVITY_LEVEL_3 = 0x504112,
	CCA_SENSITIVITY_LEVEL_4 = 0x524116,
	CCA_SENSITIVITY_LEVEL_5 = 0x544120,
};

extern UINT16 QMA_FMGetCh(S_MA_DRIVER *qma);
extern UINT8  QMA_FMSetCh(S_MA_DRIVER *qma,UINT16 freq);
extern void QMA_FMConfigScan(S_MA_DRIVER *qma,UINT16 start,UINT16 stop);
extern UINT8 QMA_FMSeekChInit(S_MA_DRIVER *qma,UINT8 th);
extern UINT8 QMA_FMGetRSSI(S_MA_DRIVER *qma,UINT16 ch);
extern void  QMA_FMTuneToCH(S_MA_DRIVER *qma,UINT16 ch);
extern UINT16 QMA_FMRXSeekCH(S_MA_DRIVER *qma,UINT16 start, UINT16 stop,UINT8 up);
extern UINT8 QMA_FMRXSeekCHAll(S_MA_DRIVER *qma,UINT16 start, UINT16 stop, UINT8 db,UINT8 up);
extern UINT8 QMA_FMValidCH(S_MA_DRIVER *qma,UINT16 freq);
extern void QMA_FMConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option);
#endif
