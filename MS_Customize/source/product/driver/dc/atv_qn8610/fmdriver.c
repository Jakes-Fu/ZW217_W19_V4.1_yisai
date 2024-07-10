/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       fmdriver.c
** Last Modified Date: 
** Last Version:     
** Descriptions: The fmdriver.c is used to driver fm function interface
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

#ifdef QN_SIM
#include "..\stdafx.h" 
#include "qmaconfig.h"
#include "qmacommon.h"
#endif

#include "qmadriver.h"
#include "qmafunc.h"
#include "fmdriver.h"

/**********************************************************************
    UINT16 QMA_FMGetCh(S_MA_DRIVER *qma) 
**********************************************************************
Description: 
    to get current channel frequency

Parameters:
     qma :	 the point of struct S_MA_DRIVER
    
Return value:
    channel frequency
**********************************************************************/
UINT16 QMA_FMGetCh(S_MA_DRIVER *qma) 
{
    UINT8 tCh;
    UINT8  tStep; 
    UINT16 ch = 0;
    
    tStep = QNF_GetData(QNR_FM_CH_STEP_CH98);    
    tCh = QNF_GetData(QNR_FM_CH_REG);    
    ch = (UINT16)((tStep<<8)|tCh);    
    return (UINT16)CHREG2FREQ(ch);
}

/**********************************************************************
    UINT8 QMA_FMSetCh(S_MA_DRIVER *qma,UINT16 freq)
**********************************************************************
Description: set channel frequency 

Parameters:
     qma : the point of struct S_MA_DRIVER

     freq:  channel frequency to be set
    
Return value:
    1: success
**********************************************************************/
UINT8 QMA_FMSetCh(S_MA_DRIVER *qma,UINT16 freq) 
{
    UINT16 f;	
    
    f = (UINT16)FREQ2CHREG(freq);    
    if(QNR_CH_WEN_EN == QNF_GetData(QNR_AUD_CTRL2_CH_WEN))
    {
        QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_DIS);
        QMA_Delay(20);
    }    
    QNF_SetData(QNR_FM_CH_STEP_CH98,(UINT8) (f >> 8));    
    QMA_WriteReg(QNR_FM_CH, (UINT8) f);    
    qma->fm->curFreq = freq;    
    QNF_SetData(0x4e80,0x80);
    QMA_Delay(20);
    QNF_SetData(0x4e80,0x00);    
    return 1;
}

/**********************************************************************
    void QMA_FMConfigScan(S_MA_DRIVER *qma,UINT32 start,UINT32 stop)
**********************************************************************
Description: config start, stop, step register for FM/AM CCA or CCS

Parameters:
     qma : the point of struct S_MA_DRIVER

     start
        Set the frequency (10kHz) where scan to be started,
        eg: 7600 for 76.00MHz.
     stop
        Set the frequency (10kHz) where scan to be stopped,
        eg: 10800 for 108.00MHz
     step        
        1: set leap step to (FM)100kHz / 10kHz(AM)
        2: set leap step to (FM)200kHz / 1kHz(AM)
        0:  set leap step to (FM)50kHz / 9kHz(AM)
        
Return value:
     None
**********************************************************************/
void QMA_FMConfigScan(S_MA_DRIVER *qma,UINT16 start,UINT16 stop) 
{
    UINT8 tS;
    UINT16 fStart;
    UINT16 fStop;	
    
    UINT8 tStep = qma->fm->chStep;    
    fStart = (UINT16)FREQ2CHREG(start);
    fStop = (UINT16)FREQ2CHREG(stop);    
    tS = (UINT8) (fStart & 0xff);
    QNF_SetData(QNR_FM_CH_STEP_START98,(UINT8) (fStart>>6));
    QNF_SetData(QNR_FM_CH_START_REG, tS);    
    tS = (UINT8)(fStop & 0xff);
    QNF_SetData(QNR_FM_CH_STEP_STOP98,(UINT8)((fStop >> 4) & 0x30));
    QNF_SetData(QNR_FM_CH_STOP_REG, tS);    
    if(tStep == QNM_FM_CH_STEP_50KHZ)
        QNF_SetData(QNR_FM_CH_STEP_REG, QNR_FM_CH_STEP_50KHZ);
    else if(tStep == QNM_FM_CH_STEP_100KHZ)
        QNF_SetData(QNR_FM_CH_STEP_REG, QNR_FM_CH_STEP_100KHZ);
    else if(tStep == QNM_FM_CH_STEP_200KHZ)
        QNF_SetData(QNR_FM_CH_STEP_REG, QNR_FM_CH_STEP_200KHZ);	
}

/**********************************************************************
    UINT8 QMA_FMGetRSSI(S_MA_DRIVER *qma,UINT16 ch)
**********************************************************************
Description : Get the RSSI value

Parameters: 
     qma : the point of struct S_MA_DRIVER

     ch: 
     
Return value: 
     RSSI value  of the channel set
**********************************************************************/
UINT8 QMA_FMGetRSSI(S_MA_DRIVER *qma,UINT16 ch) 
{
    UINT8 delayTime;
    
    delayTime = QNM_READ_RSSI_DELAY;  
    QMA_FMSetCh(qma,ch);
    QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_DIS);   
    QMA_Delay(delayTime); 
    
    return QMA_ReadReg(0xe3);  
}

/**********************************************************************
    void QMA_FMTuneToCH(S_MA_DRIVER *qma,UINT16 ch)
**********************************************************************
Description: Tune to the specific channel. call QMA_SetSysMode() before 
     call this function
Parameters:
     qma : the point of struct S_MA_DRIVER
     
     ch : 
         Set the frequency (10kHz) to be tuned,
         eg: 101.30MHz will be set to 10130.
     
Return value:
     None
**********************************************************************/
void QMA_FMTuneToCH(S_MA_DRIVER *qma, UINT16 ch) 
{	
	if(qma->fm->autoScanAll == 0)
	{
        QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);
	}
    if ((ch == 6880) || (ch == 7240) || (ch == 7510) || (ch == 8050) || (ch == 8590) || (ch == 10480))
    {
        QNF_SetData(QNR_SYSTEM2_IMR,0x80);
    } 
    else
    {
        QNF_SetData(QNR_SYSTEM2_IMR,0x00);
    }    
    QMA_FMSetCh(qma,ch);
	if(qma->fm->autoScanAll == 0)
	{	
	    QMA_Delay(300);
        QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);
	}
}

/**********************************************************************
UINT8 QMA_FMSeekChInit(S_MA_DRIVER *qma,UINT8 th)
***********************************************************************
Description: This function sets various parameters for FM/CCA, for example,
            NAGC,NRSSI,NASYM,PD, also including ASYM and RSSI threshold.
			it is needed to use before QMA_FMValidCH(),QMA_FMRXSeekCHALL(),
			and QMA_FMRXSeekCH().
Parameters: 
qma : the point of struct S_MA_DRIVER
th :  sensitivity threshold, the default is 2.

  Return value:
  None     
**********************************************************************/
UINT8 QMA_FMSeekChInit(S_MA_DRIVER *qma, UINT8 th)
{
	//QN8610 FM auto search threshold setting
    UINT8 rssiHTH,rssiTH,asymTH;    
    UINT32 rssi_asym_TH_tbl[6]={
    CCA_SENSITIVITY_LEVEL_0,
    CCA_SENSITIVITY_LEVEL_1,
    CCA_SENSITIVITY_LEVEL_2,
	CCA_SENSITIVITY_LEVEL_3,
	CCA_SENSITIVITY_LEVEL_4,
	CCA_SENSITIVITY_LEVEL_5};
    
    asymTH=(UINT8)(rssi_asym_TH_tbl[th]&0xff);
    rssiTH=(UINT8)((rssi_asym_TH_tbl[th]>>8)&0xff);
    rssiHTH=(UINT8)((rssi_asym_TH_tbl[th]>>16)&0xff);
    
    QNF_SetData(0xdc38,0x20);// NAGC 10ms
    QNF_SetData(0xdc07,0x00);// NRSSI  2.5ms
    QNF_SetData(0xe07f,0x00);// IFCNT high
    QNF_SetData(0xe1ff,0x83);//0x94);// IFCNT low
    QNF_SetData(0xddc0,0x00);// NASYM low   0ms
    QNF_SetData(0xdc80,0x00);// NASYM high
    QNF_SetData(0xdb38,0x28);// NSTOP  80ms
    QNF_SetData(0xdc40,0x00);// ASYM init value
    QNF_SetData(0xdd3f,asymTH);
    
    QNF_SetData(0xde7f,rssiTH);
    QNF_SetData(0xdf7f,rssiHTH);
    QNF_SetData(0xd6ff,0xb0);// ASYM PD constant time
    QMA_WriteReg(0xc4,0x00);// pilot detect
    QNF_SetData(0x5e0f,0x0f);//
    
    return 1;
}

/***********************************************************************
UINT16 QMA_FMRXSeekCH(S_MA_DRIVER *qma,UINT16 start, UINT16 stop, UINT8 up);
***********************************************************************
Description: Automatically scans the frequency range, and detects the 
     first channel(AM or FM, it will be determine by the system mode which set 
     by QMA_SetSysMode).
     A threshold value needs to be passed in for channel detection.
	 you need firstly call QMA_FMSeekChInit(),before using it.

Parameters:
     qma : the point of struct S_MA_DRIVER

     start
           Set the frequency (10kHz) where scan will be started,
           eg: 76.00MHz will be set to 7600.
     stop
           Set the frequency (10kHz) where scan will be stopped,
           eg: 108.00MHz will be set to 10800.

      up:
          Set the seaching direction :
              Up:0, to seach from stop to start
              Up:1 to seach from start to stop
        
Return value:
     The channel frequency (unit: 10kHz)
     -1: no channel found

***********************************************************************/
UINT16 QMA_FMRXSeekCH(S_MA_DRIVER *qma,UINT16 start, UINT16 stop, UINT8 up) 
{
    UINT8 regValue, ccaflag;
    UINT16 chfreq = 0; 
    UINT32 pstart = start;
    UINT32 pstop = stop;
    
    if (qma->fm->autoScanAll == 0)
    {
        QNF_SetAudMuteEnable(qma,QNR_MUTE_ENABLE);        
    }    		
    
    QNF_SetData(QNR_SYSTEM2_IMR,QNR_SYSTEM2_IMR_LOW);
    
    QMA_FMConfigScan(qma,(UINT16)pstart,(UINT16)pstop);    	
    
    QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_EN);
    QNF_SetData(QNR_AUD_CTRL2_CCA,QNR_CCA_EN);			
    
    do
    {
        QMA_Delay(1);
        regValue = QNF_GetData(QNR_AUD_CTRL2_CCA);	   
    }while(regValue & QNR_CCA_EN);		
    
    QMA_Delay(1);		
    ccaflag = QNF_GetData(QNR_STATUS1_FM_CCA_FAIL);
    chfreq = QMA_FMGetCh(qma);
    
    if(ccaflag == QNR_FM_CCA_OK)
    {					
        #ifdef QN_SIM
        TRACE("OK: chfreq = %d,\n",chfreq);
        #endif
    }
    else
    {			
        #ifdef QN_SIM
        TRACE("failed:chfreq = %d,\n",chfreq);
        #endif
        chfreq = 0;
    }
    
    if (qma->fm->autoScanAll == 0) 
    {					
        QMA_FMTuneToCH(qma,(UINT16)(chfreq? chfreq:pstop)); 
        #ifdef QN_SIM
        TRACE("chfreq = %d,\n", chfreq);
        #endif
        QNF_SetAudMuteEnable(qma,QNR_MUTE_DISABLE);
    }		
    
    return chfreq;  
}

/******************************************************************************
UINT8 QMA_FMRXSeekCHAll(S_MA_DRIVER *qma,UINT16 start, UINT16 stop, UINT8 db,UINT8 up)
*******************************************************************************
Description:    Automatically scans the complete FM or AM band and detects 
    all the available  channels(AM or FM, it will be determine by 
    the workmode which set by QMA_SetSysmode). A threshold value 
    needs to be passed in for the channel detection.
    
Parameters:
    qma : the point of struct S_MA_DRIVER
    
    start
        Set the frequency (10kHz) where scan will be started,
        eg: 76.00MHz will be set to 7600.
    stop
        Set the frequency (10kHz) where scan will be stopped,
        eg: 108.00MHz will be set to 10800.
    Step
    
    up:
        Set the seach direction :
            0 : seach from stop to start
            1 : seach from start to stop

Return value:
      The channel count found by this function
      -1: no channel found 
*******************************************************************************/
UINT8 QMA_FMRXSeekCHAll(S_MA_DRIVER *qma,UINT16 start, UINT16 stop, UINT8 db,UINT8 up)
{ 
	UINT8  stepvalue,qma_ChCount;
	UINT16 temp;
	UINT8  count,reg08_c0;
#ifdef QN_SIM
	UINT32  ifcnt;
	UINT16 ifcnt_t;
	UINT16 ifcnt_rate;	
#endif
	UINT16 pStart = qma->fm->chStart;
	UINT16 ch_stop = qma->fm->chStop;
	UINT16 pStop = ch_stop;
    QMA_FMSeekChInit(qma,db);
	QNF_SetAudMuteEnable(qma,QNR_MUTE_ENABLE);        
	up = (UINT8)((start<stop) ? 1 : 0);
	qma->fm->autoScanAll = 1;
	stepvalue = qma->fm->chStep;    
	qma_ChCount = 0;		
	do
	{  
        #ifdef QN_SIM
		TRACE(" pstart = %d\n",pStart);			
        #endif			
		temp = QMA_FMRXSeekCH(qma,pStart,pStop,up);
		if (temp) 
		{

			if(temp == 10020 ||temp == 10040 ||temp == 10190 ||temp == 9600 ||temp == 10000 || temp == 9290 || temp == 10530 || temp == 10800 || temp == 10730)
			{	
				count = 0;
				reg08_c0 = 0;	
				QNF_SetData(0x4e80, 0x80);
				QMA_Delay(20);
				QNF_SetData(0x4e80, 0x00);					
				QMA_Delay(130);						
				
				do 
				{
					reg08_c0++;
					if(QNR_ATV_AUD_STEREO == (QMA_ReadReg(0x08) & 0xc0))
						count++;
					QMA_Delay(5);
				} while ((count < 3) && (reg08_c0 < 10));					
				
				if(count >= 3)
				{
					qma->fm->chList[qma_ChCount++] = (UINT32)temp;
				}
			}
			else
				qma->fm->chList[qma_ChCount++] = (UINT32)temp;

            #ifdef QN_SIM				
			ifcnt = (QMA_ReadReg(0xe6) & 0x1f)*0x10000+QMA_ReadReg(0xe7)*0x100+QMA_ReadReg(0xe8);
			ifcnt_t = (UINT16)(843.75*2.5/2);
			ifcnt_rate = (UINT16)(100*(100.0*(ifcnt>ifcnt_t?(ifcnt-ifcnt_t):(ifcnt_t-ifcnt))/ifcnt_t));
			TRACE("CH account = %d,channel = %d, RSSI 0xe3 = %d,",qma_ChCount,temp,QMA_ReadReg(0xe3));
			TRACE("ASYM 0xe5 = %d,ifcnt = %d,flag 0xe2 = %x,",QMA_ReadReg(0xe5),ifcnt_rate,QMA_ReadReg(0xe2));
			QMA_Delay(30);
			TRACE("stereo 0x08[7:6] = %x\n",QMA_ReadReg(0x08)&0xc0);				
            #endif				
		}
		else
		{
			temp = pStop;
		}			
		pStart = (UINT16)(temp + (up ? stepvalue : -stepvalue));			
	}while((up ? (pStart <= pStop):(pStart >= pStop)) && (qma_ChCount < 50));
	
	QNF_SetData(0x5e0f,0x05);
	qma->fm->chCount = qma_ChCount;
	QMA_FMSetCh(qma,(qma_ChCount >= 1)? (UINT16)(qma->fm->chList[0]) : pStop); 	
	qma->fm->autoScanAll = 0;
	QNF_SetAudMuteEnable(qma,QNR_MUTE_DISABLE);        

	return qma_ChCount;	
}

/***********************************************************************
      UINT8 QMA_FMValidCH(S_MA_DRIVER *qma,UINT16 freq) 
***********************************************************************
Description: Automatically scans the frequency point, and detects whether 
it is valid channel.
A threshold value needs to be passed in for channel detection.
you need firstly call QMA_FMSeekChInit(),before using it.

Parameters:
    qma : the point of struct S_MA_DRIVER

    freq : 
          Set the frequency (10kHz) where scan will be used as the point,
          eg: 76.00MHz will be set to 7600.

Return value:
      1: it is valid freq point(unit: 10kHz)
      0:  no valid.
  
***********************************************************************/
UINT8 QMA_FMValidCH(S_MA_DRIVER *qma,UINT16 freq) 
{
    UINT8 regValue,ccaflag,count,reg08_c0;
    
	qma->fm->autoScanAll=1;
    QNF_SetAudMuteEnable(qma,QNR_MUTE_ENABLE);	
    QNF_SetData(QNR_SYSTEM2_IMR,QNR_SYSTEM2_IMR_LOW);	
    QMA_FMConfigScan(qma,(UINT16)freq,(UINT16)freq);   
    QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_EN);		
    QNF_SetData(QNR_AUD_CTRL2_CCA,QNR_CCA_EN);    
    do
    {
        QMA_Delay(1);
        regValue = QNF_GetData(QNR_AUD_CTRL2_CCA);
		
    }while(regValue & QNR_CCA_EN);		
    
    QMA_Delay(1);		
    ccaflag = QNF_GetData(QNR_STATUS1_FM_CCA_FAIL);	
    
    if(ccaflag == QNR_FM_CCA_OK)
    {   
        QMA_Delay(5);
        qma->fm->curFreq = (UINT16)freq;		
        QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_DIS);
        QNF_SetAudMuteEnable(qma,QNR_MUTE_DISABLE);
        QMA_FMTuneToCH(qma,(UINT16)freq); 
		qma->fm->autoScanAll=0;        
		// please use stereo check for special freq, according to your project requirement
        if(freq == 10020 ||freq == 10040 ||freq == 10190 ||freq==9450 || freq == 9600 ||freq == 10000 || freq == 9290 || freq == 10530 || freq == 10800 || freq == 10730)
        {	
            count = 0;
            reg08_c0 = 0;	
            QNF_SetData(0x4e80, 0x80);
            QMA_Delay(20);
            QNF_SetData(0x4e80, 0x00);					
            QMA_Delay(130);	
			
            do 
            {
                reg08_c0++;
                if(QNR_ATV_AUD_STEREO == (QMA_ReadReg(0x08) & 0xc0))
                count++;
                QMA_Delay(5);
            } while ((count < 3) && (reg08_c0 < 10));					
            
            if(count >= 3)			
                return 1;			
            else
                return 0;
        }		
        return 1;			
    }
    else
    {		
        QMA_Delay(5);
        QNF_SetData(QNR_AUD_CTRL2_CH_WEN,QNR_CH_WEN_DIS);
		qma->fm->autoScanAll=0;
        QNF_SetAudMuteEnable(qma,QNR_MUTE_DISABLE);			
        
        return 0;		 
    }
}

/************************************************************************
    void QMA_FMConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
*************************************************************************
Description: This function configures audio features for FM. 

Parameters:
    qma : the point of struct S_MA_DRIVER
    
    optiontype                        |            option  
         QNM_CONFIG_MUTE     |   0/1 : mute disable/enable 
         QNM_CONFIG_VOLUME |   00 ~ 41 (00 -> -23dB, ..., 23 -> 0dB, ..., 41 -> 18dB)
         
Return value:
     none
**********************************************************************/
void QMA_FMConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
{
    switch(optiontype)
    {			
        case QNM_CONFIG_MUTE:
            QNF_SetAudMuteEnable(qma,option);
            break;	
        case QNM_CONFIG_VOLUME:
            QNF_SetAudioVolume(qma,option);	
            break;
        default:
            break;
    }
}
