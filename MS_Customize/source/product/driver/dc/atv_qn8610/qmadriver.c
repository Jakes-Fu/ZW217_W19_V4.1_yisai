/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmariver.c
** Last Modified Date: 
** Last Version:     
** Descriptions: The qmadriver.c is used to customer driver interface
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
** Descriptions:     For QN8610 B1 
**
**************************************************************************************************/
#ifdef QN_SIM
#include "..\stdafx.h"        
#endif 

#include "qmadriver.h"
#include "qmafunc.h"
#include "fmdriver.h"
#ifdef QN_SIM
#include "winbase.h"
#endif

#include "os_api.h"

//客户根据TV菜单中国家名，和顺序，在使用时，可自己更改相对应的国家列表
UINT8 Region_code_table[QNM_TOTAL_REGION]=
{
    QNM_REGION_CHINA,	
    QNM_REGION_HONGKONG,
    QNM_REGION_SHENZHEN,
    QNM_REGION_CAMBODIA,
    QNM_REGION_CANADA,
    QNM_REGION_CHILE,
    QNM_REGION_AFGHANISTAN,
    QNM_REGION_ARGENTINA,
    QNM_REGION_BRAZIL,
    QNM_REGION_INDONESIA,
    QNM_REGION_JAPAN,
    QNM_REGION_KOREA,
    QNM_REGION_LAOS,
    QNM_REGION_MALAYSIA,
    QNM_REGION_MEXICO,
    QNM_REGION_BURMA,//myanmar
    QNM_REGION_PHILIPPINES,
    QNM_REGION_SINGAPORE,
    QNM_REGION_SOUTHAFRICA,
    QNM_REGION_TAIWAN,
    QNM_REGION_THAILAND,
    QNM_REGION_TURKEY,
    QNM_REGION_UAE,
    QNM_REGION_UK,
    QNM_REGION_USA,		
    QNM_REGION_VENEZUELA,
    QNM_REGION_VIETNAM,
    QNM_REGION_WESTERNEUROPE,
    QNM_REGION_SPAIN,
    QNM_REGION_PORTUGAL,
    QNM_REGION_PAKISTAN,
    QNM_REGION_INDIA,
    QNM_REGION_AUSTRALIA,
    QNM_REGION_PARAGUAY,
    QNM_REGION_URUGUAY,
    QNM_REGION_BULGARIA,
    QNM_REGION_CONGO,
    QNM_REGION_EGYPT,
    QNM_REGION_GERMAN,
    QNM_REGION_IRAQ,
    QNM_REGION_IRAN,
    QNM_REGION_HUNGARY,
    QNM_REGION_LIBYA,
    QNM_REGION_LEBANON,
    QNM_REGION_POLAND,
    QNM_REGION_ROMANIA,		
    QNM_REGION_SAUDIARABIA,
    QNM_REGION_TUNISIA,
    QNM_REGION_RUSSIA,
    QNM_REGION_BOLIVIA,
    QNM_REGION_JAMAICA,		
    QNM_REGION_SWEDEN,		
    QNM_REGION_SUDAN,
    QNM_REGION_YEMEN,
    QNM_REGION_PERU,
    QNM_REGION_COLUMBIA,	
    QNM_REGION_ECUADOR,
    QNM_REGION_SURINAM,		
    QNM_REGION_NEWZEALAND,		
    QNM_REGION_COSTARICA,
    QNM_REGION_HONDURAS,		
    QNM_REGION_PUERTO_RICO,
    QNM_REGION_NICARAGUA,
    QNM_REGION_GUATEMALA,
    QNM_REGION_DOMINICA,
    QNM_REGION_ANTIGUA,
    QNM_REGION_CUBA,
    QNM_REGION_PANAMA,
    QNM_REGION_GUAM,
    QNM_REGION_ELSALVADOR,
    QNM_REGION_HAITI,
    QNM_REGION_MONTSERRAT,
    QNM_REGION_GUYANA,
    QNM_REGION_KENYA,
    QNM_REGION_ALGERIA,
    QNM_REGION_LIBERIA,
    QNM_REGION_NIGERIA,
    QNM_REGION_SOMALIA,
    QNM_REGION_TANZANIA,
    QNM_REGION_BANGLADESH,
    QNM_REGION_EAST_TIMOR,
    QNM_REGION_JORDAN,
    QNM_REGION_KUWAIT,
    QNM_REGION_MALDIVES,
    QNM_REGION_QATAR,
    QNM_REGION_SRI_LANKA,
    QNM_REGION_SYRIA,
    QNM_REGION_AUSTRIA,
    QNM_REGION_BELGIUM,
    QNM_REGION_DENMARK,
    QNM_REGION_FINLAND,
    QNM_REGION_ICELAND,
    QNM_REGION_ISRAEL,
    QNM_REGION_MACEDONIA,
    QNM_REGION_NALTA,
    QNM_REGION_NORWAY,
    QNM_REGION_SWITZERLAND,
    QNM_REGION_FIJI_ISLANDS,
    QNM_REGION_SOLOMON_ISLANDS,
    QNM_REGION_GREENLAND,
    QNM_REGION_GHANA,
    QNM_REGION_ZIMBABWE,
    QNM_REGION_CAMEROON,
    QNM_REGION_UGANDA,
    QNM_REGION_ZAMBIA,
    QNM_REGION_NORTH_KOREA,
    QNM_REGION_MONGOLIA,
    QNM_REGION_AZERBAIJAN,
    QNM_REGION_CZECH_REPUBLIC,
    QNM_REGION_GEORGIA,
    QNM_REGION_UKRAIN,
    QNM_REGION_SENEGAL,
    QNM_REGION_MADAGASCAR,
    QNM_REGION_GREECE,
    QNM_REGION_MOROCCO,
    QNM_REGION_MALI,
    QNM_REGION_ITALY,
    QNM_REGION_ANGOLA,
    QNM_REGION_IRELAND,
    QNM_REGION_MACAU,
	QNM_REGION_ABU_DHABI,
	QNM_REGION_BAHRAIN,
	QNM_REGION_BRUNEI,
	QNM_REGION_NAMIBIA,
	QNM_REGION_OMAN,
	QNM_REGION_SRILANKA,
	QNM_REGION_GAMBIA,
	QNM_REGION_SIERRALEONE,
	QNM_REGION_SWAZILAND,
	QNM_REGION_MADEIRA,
	QNM_REGION_CYPRUS,
	QNM_REGION_MALTA,
	QNM_REGION_HOLAND,
    QNM_REGION_BENIN,
    QNM_REGION_BURKINA_FASO,
    QNM_REGION_CAPE_VERDE,
    QNM_REGION_COTE_DIVOIRE,   
    QNM_REGION_GUINEA,
    QNM_REGION_GUINEA_BISSAU,
    QNM_REGION_MAURITANIA,
    QNM_REGION_NIGER,    
    QNM_REGION_TOGO,           
};

/**********************************************************************
    void QMA_Delay(UINT16 ms)
**********************************************************************
Description: 
    Delay in ms. Customer needs to calibrate this function
    according to the specific MCU platform used.

Parameters:
    ms : mini seconds counts

Return value:
    None

Note:   
   Both TV and RX/FM need this function.
   This function should be re-implemented depending 
   on the hardware and system used.
**********************************************************************/
void QMA_Delay(UINT16 ms) 
{
#ifdef QN_SIM
    Sleep(ms);
#else
   SCI_Sleep(ms);
#endif
}

/*********************************************************************
    UINT16 QMA_GetChipVersion(void)
*********************************************************************
description: 
    This function confirms that the master can identify 
    chip QN8610 to decide whether the communication command
    is successful with the master and the slave block (QN8610),
    before entering QMA_Init().
		 
Parameters:
    qma: the point of struct S_MA_DRIVER
     
Return value:
    chip ID value
    qn8610a1: 0x05

**********************************************************************/
UINT16 QMA_GetChipVersion(void)
{
    UINT8 chip1, chip2;
    UINT16 chipID;

    chip1 = QNF_GetData(QNR_CID1_REG);
    chip2 = QNF_GetData(QNR_CID2_REG);
    chipID = (UINT16)(chip1 | (chip2 << 8));
	
    return chipID;
}

/************************************************************************
        UINT8 QMA_Testing(S_MA_DRIVER *qma,UINT8 optiontype)
*************************************************************************
Description: This function configures testing function, suitable for 
              integrating new cases for the first time.
Parameters:
  optiontype: (0-3)

   #define QMA_TEST_INIT_CH      0
   #define QMA_TEST_TONE         1 
   #define QMA_TEST_COLOR_BAR    2 
   #define QMA_TEST_DETECT_COLOR 3        

Return Value:
     0:   
	 1:
**********************************************************************/
UINT8 QMA_Testing(S_MA_DRIVER *qma,UINT8 optiontype)
{
    UINT8 status=1;
    switch (optiontype)
    {
        case QMA_TEST_INIT_CH:
            QMA_WriteReg(0x00,0x80);
            QMA_WriteReg(0x00,0x22);
            QNF_SetSysClock(qma,QMA_USING_EXTERNAL_CLOCK, QMA_SINUSOID_WAVEFORM, QMA_SYS_CLK_1);
            QMA_WriteReg(0x02,0xbe);
            QMA_WriteReg(0x10,0x11);	   
            QMA_WriteReg(0x1d,0xf0);
            QMA_WriteReg(0x0b,0x20);	
            QMA_WriteReg(0x0c,0x43);	
            QMA_WriteReg(0x0d,0x2d);	
            QMA_WriteReg(0x0e,0x2d);	
            QMA_WriteReg(0x79,0xa9);	
            QMA_WriteReg(0x7a,0x08);	
            QMA_WriteReg(0x7b,0x94);	
            QMA_WriteReg(0x29,0x5e);	
            QMA_WriteReg(0x2a,0x15);	
            QMA_WriteReg(0x2b,0x2a);	
            QMA_WriteReg(0x2d,0x80);	
            QMA_WriteReg(0x8e,0x48);
            QMA_WriteReg(0x76,0x58);	
            QMA_WriteReg(0x77,0x5b);
            QMA_WriteReg(0x82,0x8f);	   
            QMA_WriteReg(0x85,0xe0);	   
            QMA_WriteReg(0x86,0xca);	   
            QMA_WriteReg(0x8a,0xff);	   
            QMA_WriteReg(0x8d,0x82);	   
            QMA_WriteReg(0x8c,0x30);	   
            QMA_WriteReg(0x8e,0x1a);	   
            QMA_WriteReg(0x8f,0xff);
            QMA_WriteReg(0xcb,0xe0);
            QMA_WriteReg(0xc9,0x33);
            QMA_WriteReg(0xca,0xa1);
            QMA_WriteReg(0xc8,0x93);	      
            QMA_WriteReg(0x37,0x0c);	   
            QMA_WriteReg(0x3a,0x80);
            QNF_SetATVCH(qma, 67125);
            QNM_CFG_HSYNC_LOW;
            QNM_CFG_VSYNC_LOW;
            QNM_CFG_PCLK_LOW;
            QNM_CFG_COLOR_ORDER_Cr1Y1Cb1Y2;
            QNM_CFG_PCLK_DRV_STRENGTH_LOW;
            QNM_CFG_DATA_DRV_STRENGTH_LOW;			
            break;
        case QMA_TEST_TONE:
            QMA_WriteReg(0x00,0x80);
            QMA_WriteReg(0x00,0x22);
            QNF_SetSysClock(qma,QMA_USING_EXTERNAL_CLOCK, QMA_SINUSOID_WAVEFORM, QMA_SYS_CLK_1);
            QMA_WriteReg(0x01,0x02);			
            break;
        case QMA_TEST_COLOR_BAR:
            QMA_WriteReg(0x00,0x80);
            QMA_WriteReg(0x00,0x22);	
            QNF_SetSysClock(qma,QMA_USING_EXTERNAL_CLOCK, QMA_SINUSOID_WAVEFORM, QMA_SYS_CLK_1);
            QMA_WriteReg(0x1d,0xf0);
            QMA_WriteReg(0x0c,0x00);
            QMA_WriteReg(0x10,0x15);//11 snow dots 
            QNM_CFG_HSYNC_LOW;
            QNM_CFG_VSYNC_LOW;
            QNM_CFG_PCLK_LOW;
            QNM_CFG_COLOR_ORDER_Cr1Y1Cb1Y2;
            QNM_CFG_PCLK_DRV_STRENGTH_LOW;
            QNM_CFG_DATA_DRV_STRENGTH_LOW;		
            break;
        case QMA_TEST_DETECT_COLOR:
            status=(UINT8)(QMA_ReadReg(0x08)&0x08);
            break;
        default:
            break;
    }
    return status;
}


/**********************************************************************
    UINT8 QMA_Init(S_MA_DRIVER *qma, UINT8 dev)
**********************************************************************
Description : 
    Initialize device to have all functionality ready for use, 
    used in where is after calling QMA_SetRegion(). 
              
Parameters:
    qma : the point of struct S_MA_DRIVER
    dev : 
          QNM_DEV_ATV
          QNM_DEV_FM
	    
Return value :
    1: Device is ready to use.
    0: Device is not ready to serve function.
**********************************************************************/
//特别说明：
//省电设置：保持电源正常供电，只需要CEN脚拉低，
//功耗为9uA(demo测试值)。
UINT8 QMA_Init(S_MA_DRIVER *qma, UINT8 dev) 
{
	UINT16 chpID = 0;
    qma->dev=dev;
    if(qma->mode == QNM_MODE_WORKING)
    {
		//qn8610使用标准I2C协议，如果芯片供电正常，CEN脚拉高，
		//I2C工作就没有问题，不管下面的时钟配置是否正确。
        chpID = QMA_GetChipVersion();//如果读回chpID =5，I2C读正确
        SCI_TRACE_LOW("Chip ID = %x\n", chpID);
#ifdef QN_SIM							
        TRACE("Chip ID = %x\n", chpID);
#endif
        QNF_InitVariable(qma);//全局变量初始化，包括FM的,start,stop值		
        if(QNM_DEV_ATV==qma->dev)//电视初始化
        {		
		QNF_SetData(0x0080,0x80);//reset qn8610 by software
		QMA_Delay(10);            
		QMA_ConfigAudio(qma,QNM_CONFIG_MUTE, QNR_MUTE_ENABLE);//qn8610 audio mute enable            	
		QNF_InitChip(qma,dev);

		//如果时钟配置不正确，就不会有视频数据输出，ATV芯片没工作起来，但I2C工作没有问题
		QNF_SetSysClock(qma,QMA_USING_EXTERNAL_CLOCK, QMA_RECTANGULAR_WAVEFORM, QMA_SYS_CLK_0);//Set QN8610 to use 32.768KHz external clock
		QNF_InitConfigVideo(qma);
#if(QMA_ATV_REDUCE_SHAKE)
		QNF_ConfigTFParam(qma,QNM_TF_LEVEL_2);
#else
		QNF_ConfigTFParam(qma,QNM_TF_LEVEL_3);
#endif
		QNF_ConfigVideoOutputFormat(qma,QNR_VID_ITU_MODE_BT601);//如果需要BT656,要使用 QNR_VID_ITU_MODE_BT656

		//特别说明：请确保DATA线，PCLK线，使用最小驱动电流能正常工作，
		//如果出现绿线，或接上摄像头不能正常工作，请联系我们的FAE。
		QNM_CFG_HSYNC_LOW;//horizontal sync polarity low
		QNM_CFG_VSYNC_HIGH;//vertical sync polarity high
		QNM_CFG_PCLK_LOW;//pixel clock polarity low
		QNM_CFG_COLOR_ORDER_Y1Cb1Y2Cr1;// YUV order : Y1Cb1Y2Cr1,Y1Cr1Y2Cb1,Cb1Y1Cr1Y2,Cr1Y1Cb1Y2
		QNM_CFG_PCLK_DRV_STRENGTH_LOW;
		QNM_CFG_DATA_DRV_STRENGTH_LOW;
		QNF_ConfigResolution(qma,QNM_RESOLUTION_320_240);// resolution setting
		QNF_SetATVStd(qma,ATV_ChMap[qma->regionCode][0].tvStd);
		QNF_ATVAudDecSel(qma,ATV_ChMap[qma->regionCode][0].tvStd);
		QNF_InitConfigAudio(qma);
		QMA_ConfigAudio(qma,QNM_CONFIG_VOLUME, qma->volume);//音量设置，最大不失真 5dB

		QMA_WriteReg(0x86,0xf3);
		QMA_WriteReg(0x88,0x40);
		QMA_WriteReg(0x8c,0x9f); 

		QNF_SetData(0x3d30,0x00);
		QNF_SetData(0x3f80,0x80);
		QMA_WriteReg(0xb0,0x27);
		QMA_WriteReg(0xb1,0x4a);                
		QMA_WriteReg(0x2e,0x1d);	 // 0x35 ? better?		
		QNF_SetData(0x3780,0x80);	   
		QNF_SetData(0xcb10,0x00); 
		QNF_SetData(0x6a40,0x40);//ADC
		QNF_SetData(0x1140,0x40);
		QNF_SetData(0x6701, 0x01);	// increase mixer gain! did it ok?
		
#if(QMA_ATV_REDUCE_SHAKE)
		QMA_WriteReg(0x5a, 0xc4);
		QMA_WriteReg(0x6d, 0x45);
		QMA_WriteReg(0x6f, 0x75);
		QMA_WriteReg(0x7c, 0xdd);
		QMA_WriteReg(0xc8, 0xa3);
		
		QMA_WriteReg(0x8e, 0x05);	// for solving video shake left and right  0x0A
		//QNF_SetData(0x2a3f, 0x1a);	// for solving video shake left and right
#endif			
		//此宏默认是关闭，如果信号非常弱，彩色噪点不可接受，
		//可打开此宏，变成黑白色，或者希望是黑白雪花点，而不是彩点，也可以打开此宏。
#if QMA_COLOR_KILL_ENABLE
		QNF_TVCkillSelection(qma, QNM_CKILL_SETTING_6);
#endif			
		return 1;
        }
        else
        {		
			//收音机初始化
            QNF_SetData(0x0080,0x80);//reset qn8610 by software
            QMA_Delay(10);
            QMA_ConfigAudio(qma,QNM_CONFIG_MUTE, QNR_MUTE_ENABLE);
            QMA_Delay(10);			
            QNF_InitChip(qma,dev);
			QNF_SetSysClock(qma,QMA_USING_EXTERNAL_CLOCK, QMA_RECTANGULAR_WAVEFORM, QMA_SYS_CLK_0);
            QNF_InitConfigAudio(qma);
			QMA_ConfigAudio(qma,QNM_CONFIG_VOLUME, qma->volume);//音量设置，最大不失真 5dB
            QNF_SetData(0xcb1f,0x19);
            QMA_WriteReg(0xcc,0x39);
            QNF_SetData(0x1001,0x00);
            QMA_WriteReg(0x1d, 0x00);
			
		    QMA_WriteReg(0xc8, 0xa3);  // low down FM noise while signal is not well 
			//QMA_WriteReg(0xca,0xc9);//default is 0xa1;			
            return 1;
        }       
    }
    return 0;
}

/**********************************************************************
    void QMA_SetSysMode(S_MA_DRIVER *qma, UINT8 mode) 
***********************************************************************
Description: 
     Set device working mode. 
     Call this function after QMA_Init().
     
Parameters:
    qma : the point of struct S_MA_DRIVER

    mode: 
        QNM_MODE_WORKING: chip is on normal working mode
        QNM_MODE_SLEEP      : chip is on power saving mode

Return value:
    None     
**********************************************************************/
void QMA_SetSysMode(S_MA_DRIVER *qma, UINT8 mode) 
{
    switch(mode)
    {
        case QNM_MODE_WORKING:
            if(qma->mode == QNM_MODE_SLEEP)
            {
                QNF_RestartFromPowerDown(qma);
                qma->mode=QNM_MODE_WORKING;
            }
            break;    
        case QNM_MODE_SLEEP:
            if(qma->mode != QNM_MODE_SLEEP)
            {
                QNF_PowerDown(qma);
                qma->mode=QNM_MODE_SLEEP;
            }
            break;    
        default:
            break;
    }
}

/**********************************************************************
    UINT8 QMA_SetRegion(S_MA_DRIVER *qma, UINT16 regionCode)
***********************************************************************
Description: 
    Set start and stop for RX FM on different
    cities and countries, and sets ATV system 
    for different cities and countries, initializes the region 
    channel table,  gets the total channel number.
    it is called before QMA_Init().

Parameters:
    qma : the point of struct S_MA_DRIVER

    regionCode(0-7):
        Set the chip for specified city and country:
        please refer as the upper array rgnIndex[QNM_TOTAL_REGION]

Return value:
    None     
**********************************************************************/
//在调用初始化（QMA_Init( ) ）之前，请调用此函数
UINT8 QMA_SetRegion(S_MA_DRIVER *qma, UINT16 regionCode) 
{	
    qma->regionCode = regionCode;
    qma->rgnChTotalCount = QNF_GetRgnChCount(qma);
    qma->mode = QNM_MODE_WORKING;
    return 1;
}

/******************************************************************************
    UINT8 QMA_GetAntennaType(S_MA_DRIVER *qma)
******************************************************************************
Description:
    This function gets the current antenna state, it is used after 
    QMA_SetRegion(), QMA_SetSysMode().
    
Parameters: 
    qma : the point of struct S_MA_DRIVER

 Return:
       None                  0
       QNM_ATV_ANTN_UHF      1
       QNM_ATV_ANTN_VHFIII   2
       QNM_ATV_ANTN_VHFI     3
	   QNM_FM_ANTN           4
 ******************************************************************************/
UINT8 QMA_GetAntennaType(S_MA_DRIVER *qma)
{
    UINT8 dev = qma->dev;
    UINT8 temp = QNF_GetData(QNR_TUNER_CTRL_BAND_SEL);	
    
    if(temp == QNR_BAND_SEL_UHF)
    {
        return QNM_ATV_ANTN_UHF;
    }
    else if(temp == QNR_BAND_SEL_VHFIII)
    {
        return QNM_ATV_ANTN_VHFIII;	
    }
    else
    {
        if(dev == QNM_DEV_FM)
        {
            return QNM_FM_ANTN;
        }
        else
        {
            return QNM_ATV_ANTN_VHFI;
        }
    }
}

/***********************************************************************
    UINT32 QMA_RXSeekCH(S_MA_DRIVER *qma, UINT32 start, UINT32 stop, UINT8 up)
***********************************************************************
Description: 
    Automatically scans the frequency range, and detects the 
    first channel(ATV or FM, it will be determine by the system mode 
    which is set by QMA_SetSysMode).
    A threshold value needs to be passed in for channel detection.
    
Parameters:
    qma : the point of struct S_MA_DRIVER

    start : 
        0~0xff: start at specified channel index, eg: 2 means start at channel list index 2
        >0xff: start at specified channel frequency, eg: 4975 means start at frequency 49.75Mhz

    stop : 
        0: stop will be last channel index, stop = rgnChTotalCount;
        1~0xff: stop will be specified by its value (channel index), eg: 20 means stop at channel index = 20
        >0xff: stop will be specified by frequency, eg: 87125 means stop at 871.25Mhz
    up :  the step direction, reserved.

    note : 
        the different regions have different channel frequencies and ranges.
	 for example, China is from 49.75MHz to 871.25MHz, its bandwidth is 8MHz, 
	 while USA is from 55.25MHz to 867.25MHz, its bandwidth is 6MHz. 
	 
Return value :
    The channel frequency (unit: 10KHz)
    0: no channel found.    
***********************************************************************/
UINT32 QMA_RXSeekCH(S_MA_DRIVER *qma, UINT32 start, UINT32 stop, UINT8 up)
{
    UINT8 i,j,k,rgn;
    UINT8 rgnChTatol=qma->rgnChTotalCount;
    UINT32 pstart=start;
    UINT32 pstop=stop;
    	
    rgn=(UINT8)qma->regionCode;
    if(pstart > pstop)
    {
        return 0;
    }    
    
	j=QNF_GetATVIndex(qma, start);
    if((start>0xff)&&(j == 0xff))
    {
        return 0;
    }
    else if((start<0xff)&& start>=rgnChTatol)
    {
        j = 0;
    }
	else
	{
		if( start < rgnChTatol)
			j=(UINT8)start;
	}
	
	k = QNF_GetATVIndex(qma, stop);
    if(stop>0xFF &&(k == 0xff))
    {
        k = rgnChTatol; 
    }
    else if( stop<0xFF && stop >= rgnChTatol)
    {
        k = rgnChTatol;
    }
	else
	{
		if (stop < rgnChTatol)
			k=(UINT8)stop;
	}
	k=(UINT8)((k == rgnChTatol)? rgnChTatol-1:k);
    QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);    
    for (i = j; i <= k; i++)    
    {		
        QNF_SetATVCH(qma,ATV_ChMap[rgn][i].freq);						
        if(QNF_ATVSeekChCond(qma))
        {
            QNF_SetAudMuteEnable(qma,QNR_MUTE_DISABLE);					
            return ATV_ChMap[rgn][i].freq;
        }			
    }    
    QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);    
    return 0;
}

/***********************************************************************
    UINT32 QMA_RXValidCH(S_MA_DRIVER *qma, UINT32 chfreq)
***********************************************************************
Description: Automatically scans the frequency, and detects the 
channel whether it is valid channel.
Parameters:
    qma : the point of struct S_MA_DRIVER

    chfreq: 
        Two way to use it:
        1. Use it as channel no (indicate TV station no) when chfreq < 0xff
        2. Use it as exact frequency value (unit: 10Khz)
           eg: 49.75MHz will be set to 4975.
Return value :
      1: The channel is valid
      0: not a valid channel

***********************************************************************/
//搜台函数，注意变量chfreq，它既可以是频道号，也可以是频率，
//但记住，如果是频率，他的单位是10KHz。
UINT32 QMA_RXValidCH(S_MA_DRIVER *qma, UINT32 chfreq)
{   
    
    UINT32 freq;
    UINT8 rgn;
	UINT32 ret = 0;

    rgn=(UINT8)qma->regionCode;
    freq=0;
    if(chfreq<0xff)
    {
        freq = QNF_GetCHFreq(qma,(UINT8)chfreq);
    }
    else
    {
        freq=chfreq;
    }	
    QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);    
    ret = QNF_SetATVCH(qma, freq);
	if(ret)
	{
        ret = QNF_ATVSeekChCond(qma);    	
	}
    QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);
    return ret;
}

/**********************************************************************
    UINT8 QMA_RXSeekCHAll(S_MA_DRIVER *qma, UINT32 start, UINT32 stop)
**********************************************************************
Description:    Automatically scans the complete TV band and detects 
all the available  channels(ATV, it will be determine by 
the work mode which is set by QMA_SetSysmode). A threshold value 
needs to be passed in for the channel detection.
Parameters:
    qma : the point of struct S_MA_DRIVER

    start : 
    0~0xff: start at specified channel index, eg: 2 means start at channel list index 2
    >0xff: start at specified channel frequency, eg: 4975 means start at frequency 49.75Mhz
  
    stop : 
    0: stop will be last channel index, stop = rgnChTotalCount;
    1~0xff: stop will be specified by its value (channel index), eg: 20 means stop at channel index = 20
    >0xff: stop will be specified by frequency, eg: 87125 means stop at 871.25Mhz

  note: different regions have the different channel frequencies and ranges.
	    for example, China is from 49.75MHz to 871.25MHz, bandwidth is 8MHz, 
	      while USA is from 55.25MHz to 867.25MHz, bandwidth is 6MHz.

Return value :
  The channel count found by this function
  0: no channel found 
**********************************************************************/
UINT8 QMA_RXSeekCHAll(S_MA_DRIVER *qma, UINT32 start, UINT32 stop)
{
    UINT8 i,j,k,rgn,chlistcount=0;	
    UINT8 rgnChTatol=qma->rgnChTotalCount;    
    #ifdef QN_SIM
    UINT32 freq_tmp;
    #endif
    
    rgn=(UINT8)qma->regionCode;
	if (start > stop)
	{
		return 0;
	}
	
	j=QNF_GetATVIndex(qma, start);
    if((start>0xff)&&(j == 0xff))
    {
        return 0;
    }
    else if((start<0xff)&&start>=rgnChTatol)
    {
        j = 0;
    }
	else
	{
		if( start < rgnChTatol)
			j=(UINT8)start;
	}
	
	k = QNF_GetATVIndex(qma, stop);
    if(stop>0xff &&(k == 0xff))
    {
        k = rgnChTatol; 
    }
    else if(stop<0xff && stop >= rgnChTatol)
    {
        k = rgnChTatol;
    }
	else
	{
		if (stop < rgnChTatol)
			k=(UINT8)stop;
	}
	k=(UINT8)((k == rgnChTatol)? rgnChTatol-1:k);
    QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);    
    for (i = j; i <= k; i++)    
    {
        QNF_SetATVCH(qma, ATV_ChMap[rgn][i].freq);        
#ifdef QN_SIM
        if(i%15 == 0)
        TRACE(("\n"));
        freq_tmp=ATV_ChMap[rgn][i].freq;
        TRACE("%d ",freq_tmp);
#endif
        if(QNF_ATVSeekChCond(qma))
        {
            qma->chList[chlistcount]=ATV_ChMap[rgn][i].freq;
            chlistcount++;
            if(chlistcount >= QMA_CH_COUNT_MAX-1)
            {
                break;
            }
        }
    }		
    
    if(!chlistcount)
    {	
        qma->chCount=chlistcount;		
        QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);    
        return 0;
    }    
    qma->chCount=chlistcount;
    QMA_TuneToCH(qma, qma->chList[0]);		
    QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);    
    return chlistcount;	
}

/**********************************************************************
    UINT8 QMA_TuneToCH(S_MA_DRIVER *qma, UINT32 ch)
**********************************************************************
Description : 
    Tune to the specific channel. call QMA_SetSysMode() before calling this function
    
Parameters:
   qma : the point of struct S_MA_DRIVER

   ch:
       Set the frequency (10KHz) to be tuned, 
       eg: 49.75MHz will be set to 4975.

       or as the ATV channel index, if ch<0xff

note : 
    different regions have the different channel frequency ranges.
    for example, China is from 49.75MHz to 871.25MHz, 
    while USA is from 55.25MHz to 867.25MHz.

Return value :
     1 : successful
     0 : fail
**********************************************************************/
//设台函数，注意变量ch，它既可以是频道号，也可以是频率，
//但记住，如果是频率，他的单位是10KHz。
UINT8 QMA_TuneToCH(S_MA_DRIVER *qma, UINT32 ch)
{
    UINT32 freq;
    UINT8 rgn,sync,count;    
	
    count=0;
    rgn=(UINT8)qma->regionCode;
    freq=0;    
    if(ch<0xff)
    {
        freq = QNF_GetCHFreq(qma,(UINT8)ch);
    }
    else
    {
        freq = ch;
    }    
    QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);    
    QNM_SetVideoOutEnable(0);
	if(rgn == QNM_REGION_NORTH_KOREA || rgn == QNM_REGION_GREECE)
	{	
	    QNF_SetData(0x9804,0x04);
	    QMA_WriteReg(0xb1, 0x00);
	}
    QNF_PatchClrField(qma);		
    if (freq > 68000)
    {
        QNF_SetData(0x62c0, 0x00);
    } 
    else
    {
        QNF_SetData(0x62c0, 0x80);
    }
    if (freq == 21625)
    {
        QNF_SetIMR(qma, 1);
        QNF_SetTVBW(qma, 3);
    }
    else
    {
        QNF_SetIMR(qma, 0);
        QNF_SetTVBW(qma, 0);
    }    
    QMA_Delay(10);    
    QNF_SetATVCH(qma, freq);	
    if (rgn==QNM_REGION_UAE||rgn==QNM_REGION_IRAQ||rgn==QNM_REGION_ITALY)
    {
        if(freq<47125)
        {
            QMA_WriteReg(QNR_TV_STD1,(UINT8)((QMA_ReadReg(QNR_TV_STD1)&0xf3)|QNR_CHN_BW_7MHz));
        }
        else
        {
            QMA_WriteReg(QNR_TV_STD1,(UINT8)((QMA_ReadReg(QNR_TV_STD1)&0xf3)|QNR_CHN_BW_8MHz));
        }
    }
    do 
    {
        QMA_Delay(20);
        sync= (UINT8)(QMA_ReadReg(0x08)&0x06);        
        count++;    
    } while (0x06 != sync && count<15);
    QMA_Delay(70);	
    QNM_SetVideoOutEnable(1);
#if QMA_AUDIO_AUTO_DETECTION
	QNF_AudioAutoDetction(qma);
#endif
    QNF_SetAudMuteEnable(qma, QNR_MUTE_DISABLE);

    // auto detect
#if QMA_TV_AUTO_DETECTION
    QNF_TVAutoDetection(qma);
#endif
	if(rgn == QNM_REGION_NORTH_KOREA || rgn == QNM_REGION_GREECE)
	{	
	    QMA_WriteReg(0xb1, 0x4a);
	    QNF_SetData(0x9804,0x00);
	}
    return 1;
}


UINT8 QMA_AutoDetectionRegion(S_MA_DRIVER *qma)
{
#if QMA_TV_AUTO_DETECTION
    QNF_AudioAutoDetction(qma);
#endif
}

/************************************************************************
    void QMA_ConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
*************************************************************************
Description : 
    This function configures audio features. 

Parameters:
    qma : the point of struct S_MA_DRIVER

optiontype :                      option                 |      Note
    QNM_CONFIG_MUTE   : 0/1 : mute disable/enable 
    QNM_CONFIG_TV_AUD_STD       : 0/1 : BTSC/A2          |control audio standard
    QNM_CONFIG_TV_AUD_OUTPUT  : 0:stereo, 
                                1:first audio program, 
                                2:second audio program, 
                                3:first(L)+second(R)          |control audio output, 
    QNM_CONFIG_VOLUME:     00 ~ 41                            |00 -> -23dB, ..., 23 -> 0dB, ..., 41 -> 18dB
                            
Return value :
    none
**********************************************************************/
void QMA_ConfigAudio(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
{
    switch(optiontype)
    {			
    case QNM_CONFIG_MUTE:
        QNF_SetAudMuteEnable(qma, option);
        break;	
    case QNM_CONFIG_TV_AUD_STD:
        QNF_SetTVAudSTD( option);
        break;
    case QNM_CONFIG_TV_AUD_OUTPUT:
        QNF_SetTVAudSelect(option);
        break;	
    case QNM_CONFIG_VOLUME:
        QNF_SetAudioVolume(qma, option);		      
    default:
        break;
    }
}

/************************************************************************
    void QMA_ConfigVideo(S_MA_DRIVER *qma, UINT8 optiontype, UINT16 option)
*************************************************************************
Description: 
    This functions configures video parameters. when running on ATV mode, 
    you may use it to adjust image quality. 
    
Parameters:
    qma : the point of struct S_MA_DRIVER

    optiontype: (0-4)               
         QNM_CONFIG_BRIGHTNESS        :  (0-127)
         QNM_CONFIG_CONTRAST          :  (0-255)
         QNM_CONFIG_HUE               :  (0-1023)
         QNM_CONFIG_COLOR_SATURATION  :  (0-255)
	     QNM_CONFIG_LUMA              :  (0-255)
         QNM_CONFIG_BLACK_WHITE       :  (0x00/0x04 -> black & white)
	     QNM_CONFIG_TFLT              :  (0 ~ 8)
	     QNM_CONFIG_HOLD              :(1~0) 0:PAL SECAM; 1: NTSC PAL/Ms

Return value :
        none
**********************************************************************/
void QMA_ConfigVideo(S_MA_DRIVER *qma, UINT8 optiontype, UINT16 option)
{
    UINT8 lsb=(UINT8)(option&0xff);
    UINT8 msb=(UINT8)(option>>8&0x03);
    
    switch(optiontype)
    {			
        case QNM_CONFIG_BRIGHTNESS:
            QMA_WriteReg(0x0c, lsb);
            break;				
        case QNM_CONFIG_CONTRAST:
            QMA_WriteReg(0x0b, lsb);
            break;						
        case QNM_CONFIG_COLOR_SATURATION:
            QMA_WriteReg(0x0e, lsb);
            break;				
        case QNM_CONFIG_LUMA:
            QMA_WriteReg(0x0d, lsb);
            break;						
        case QNM_CONFIG_BLACK_WHITE:
            QNF_SetBlackWhiteMode(qma, lsb);
            break;			
        case QNM_CONFIG_HUE:
            QNF_SetData(QNR_HUE_REG, lsb);
            QNF_SetData(QNR_VID_CTRL1_HUE98, (UINT8)(msb<<6));
            break;
        case QNM_CONFIG_TFLT:
            QNF_SetData(0x2240, 0x00);
            QNF_ConfigTFParam(qma, (lsb));
			break;
		case QNM_CONFIG_HOLD:
#if QMA_VIDEO_HOLD_RELEASE_IS_USED
			QNF_SetData(0x2310,(UINT8)(lsb << 4));
			break;
#endif
        default: 			
            break;
    }
}

/************************************************************************
    void QMA_ConfigChrmAGC(S_MA_DRIVER *qma)
*************************************************************************
Description: 
    This functions adjust the gain for each VSYNC detected by AP.
    That means one gain adjustment every two field, 
     regarding we just output one field per frame. 
     The threshold and gain. The threshold is compared with 
     the RegA3 read back. The new gain is written into Reg0E. 
     
Parameters:
    qma : the point of struct S_MA_DRIVER

Return value :
    none
**********************************************************************/
void QMA_ConfigChrmAGC(S_MA_DRIVER *qma)
{
    UINT8 reg_0xa3;
    
    if(0x03 != (QMA_ReadReg(0x02)&0x03))
    {
        reg_0xa3= QMA_ReadReg(0xa3);        
        if (reg_0xa3>= 0xa8)
        {
            QMA_WriteReg(0x0e, 0x10);  // 0x20
        }
        else if (reg_0xa3>= 0x90)
        {
            QMA_WriteReg(0x0e, 0x18);  // 0x24
        }
        else if (reg_0xa3>= 0x75)
        {
            QMA_WriteReg(0x0e, 0x20);  // 0x28
        }
        else if (reg_0xa3>= 0x58)
        {
            QMA_WriteReg(0x0e, 0x24);  // 0x2b
        }
        else if (reg_0xa3>= 0x48)
        {
            QMA_WriteReg(0x0e, 0x28);  // 0x30
        }
        else if (reg_0xa3>= 0x38)
        {
            QMA_WriteReg(0x0e, 0x2c);	  // 0x32
        }
        else if (reg_0xa3>= 0x28)
        {
            QMA_WriteReg(0x0e, 0x30);  // 0x34
        }
        else if (reg_0xa3>= 0x20)
        {
            QMA_WriteReg(0x0e, 0x34);  // 0x30
        }
        else if (reg_0xa3>= 0x10)
        {
            QMA_WriteReg(0x0e, 0x28);  // 0x2d
        }
        else if (reg_0xa3>= 0x05)
        {
            QMA_WriteReg(0x0e, 0x20);  // 0x26
        }
        else
        {
            QMA_WriteReg(0x0e, 0x10);
        }
    }

}

/************************************************************************
    UINT8 QMA_GetRSSI(S_MA_DRIVER *qma)
*************************************************************************
Description: 
    This functions provides a signal strength indicator including TV and FM.
             
Parameters:
    qma : the point of struct S_MA_DRIVER
    
Return value:
    RSSI value : For ATV, 8 segments threshold:
	               ~ 138 ~ 125 ~ 118 ~ 111 ~ 105 ~ 101 ~ 95 ~
**********************************************************************/
UINT8 QMA_GetRSSI(S_MA_DRIVER *qma)
{
  UINT8 dev = qma->dev;

  if(QNM_DEV_ATV == dev )
  {
      return QNF_GetData(0xd9ff);
  }
  if(QNM_DEV_FM == dev )
  {
      return QNF_GetData(0xe3ff);
  }
  
  return 0;
}

/************************************************************************
      void QMA_SetTestMode(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
*************************************************************************
Description: 
    This function configures the test mode for audio and video.
    When you first are ready to match to a custom system, 
    it shows that the match is ok for audio output and video output.
    For audio test, you also need to set the audio tone frequency
    with the function QMA_SetTestToneFreq. 
    
Parameters:
    qma : the point of struct S_MA_DRIVER

optiontype                   
    QNM_TEST_PATTERN;
	   option: 0: video decode;1 : color bar; 2 : box pattern
    QNM_TEST_TONE
       option: 0: normal output; 1: test tone enable

Return value :
  none
**********************************************************************/
void QMA_SetTestMode(S_MA_DRIVER *qma, UINT8 optiontype, UINT8 option)
{
    static UINT8 regbrg, reg13, reg14, reg22, reg23, reg2c, reg2d, flg;	
    UINT8 dev =qma->dev;
    
    if((dev == QNM_DEV_ATV)||(dev == QNM_DEV_FM))
    {	
        switch(optiontype)
        {		
            case QNM_TEST_PATTERN:
                QNM_SetVideoOutEnable(0);
                QNF_PatchClrField(qma);
                QMA_Delay(20);            
                if(option)
                {
                    QNF_SetData(0x7180, (UINT8)(option == 1?0x00:0x80));            
                    if(flg == 0)
                    {
                        regbrg= QMA_ReadReg(0x0c);					 
                        reg13 = QMA_ReadReg(0x13);					 
                        reg14 = QMA_ReadReg(0x14);					 
                        reg22 = QMA_ReadReg(0x22);
                        reg23 = QMA_ReadReg(0x23);
                        reg2c = QMA_ReadReg(0x2c);					 
                        reg2d = QMA_ReadReg(0x2d);
                    }                    
                    QNF_SetData(QNR_VID_CTRL1_VID_SRC, QNR_VID_SRC_TEST_PATTERN);
                    QNF_SetData(0x7110, 0x00);             
                    QNF_SetData(0x7110, 0x10);
                    QMA_WriteReg(0x0c, 0x00);					  
                    QMA_WriteReg(0x2d, 0xe0);					  
                    QMA_WriteReg(0x22, (UINT8)((0xb1&0xcf)|(reg22&0x30)));
                    QMA_WriteReg(0x23, (UINT8)(reg23|0x08));
                    QMA_WriteReg(0x2c, 0xac);                    
                    if ((QMA_ReadReg(0x02)&0x03) >= 2)						  
                    {       
                        QMA_WriteReg(0x13, 0x12);						  
                        QMA_WriteReg(0x14, 0x49);						  
                    }					  
                    else						  
                    {
                        QMA_WriteReg(0x13, 0x0f);						  
                        QMA_WriteReg(0x14, 0x14);						  
                    }                    
                    flg=1;
                }
                else
                {
                    if(flg == 1)
                    {					 
                        QMA_WriteReg(0x0c, regbrg);					 
                        QMA_WriteReg(0x13, reg13);					 
                        QMA_WriteReg(0x14, reg14);					 
                        QMA_WriteReg(0x2d, reg2d);					 
                        QMA_WriteReg(0x22, reg22);
                        QMA_WriteReg(0x23, reg23);
                        QMA_WriteReg(0x2c, reg2c);                    
                        flg = 0;
                    }
                    QNF_SetData(QNR_VID_CTRL1_VID_SRC, QNR_VID_SRC_ATV_DECODER);
                }                
                QMA_Delay(600);
                QNM_SetVideoOutEnable(1);
                break;            
            case QNM_TEST_TONE:
                if(option)
                {
                QNF_SetData(QNR_SYSTEM2_AUD_TEST, QNR_AUD_TEST);
                }
                else
                {
                QNF_SetData(QNR_SYSTEM2_AUD_TEST, QNR_AUD_NORMAL);
                }
                break;            
            default:
                break;
        }
    }
}

/*******************************************************************
    void QMA_SetTestToneFreq(UINT8 value)
********************************************************************
description: 
    configure test tone frequency, it is valid after setting tone test enable
              
Parameters:
    Value (0-255): 
        frequency  to be output                   
        formular : Reg   = (Freq*843.75/64/1024).
            for ex : 0x06 = 467Hz *843.75/64/1024).

Note:     
        Different values specify different frequencies.
        For example, when the value is 0x06, the frequency is about 467Hz.

Return value :
    none
**********************************************************************/
void QMA_SetTestToneFreq(UINT8 value)
{
    UINT8 testone_flag = QNF_GetData(QNR_SYSTEM2_AUD_TEST);
    
    if(testone_flag == QNR_AUD_TEST)
    {
        QNF_SetData(0xf4ff, value);
    }
}

/*******************************************************************
    void QMA_SetRefreshDelay(S_MA_DRIVER *qma)
********************************************************************
description: the function sets the refresh delay time for 
           BB platform, in order to skip the LCD refresh interference.
    
Parameters:
    
Note:      

Return value :
    none
**********************************************************************/
void QMA_SetRefreshDelay(S_MA_DRIVER *qma)
{
#if QMA_LCD_REFRESH_DELAY_IS_USED
	if (!(qma->isPs))
	{
		QMA_Delay(QMA_BB_REFRESH_DELAY_TIME_PAL);
	}
	else
	{
		QMA_Delay(QMA_BB_REFRESH_DELAY_TIME_NTSC);
	}
#endif
}

#if 1//UPDATE_DRIVER
UINT32 atv_qma_scan_chn(S_MA_DRIVER *qma,UINT32* ch_id,UINT32* is_va)
{    
	
	 UINT32 freq;	 
	 UINT8 chlist,i,rgn;
	 UINT8 rgnChTatol=qma->rgnChTotalCount;
	 
	 rgn=(UINT8)qma->regionCode;
	 freq=ATV_ChMap[rgn][qma->chCount].freq;
	 chlist=ATV_ChMap[rgn][qma->chCount].chNo;     
    
     if( QMA_RXValidCH(qma,freq))
     {
         //TV_SaveCh(chlist);
        //SCI_TRACE_LOW("ATV:atv_qma_scan_chn got id[%d]",chlist);
        *is_va = 1;//SCI_TRUE;
        *ch_id = chlist;
     }
     else
     {
        *is_va = 0;//SCI_FALSE;
        *ch_id = 0;
     }
	SCI_TRACE_LOW("ATV: FREQ=%d,qma->chCount=%d,rgn=%d,freq=%d,rgnChTatol=%d,chlist=%d",ATV_ChMap[rgn][qma->chCount].freq,qma->chCount,rgn,freq,rgnChTatol,chlist);
    qma->chCount++;
    
    if(qma->chCount > rgnChTatol-1)
    return 0;
    else
    return 1;
    
}	    

UINT32 QMAAPP_GetCurrSearchNum(S_MA_DRIVER *qma)
{
	return qma->chCount;
}

UINT32 QMAAPP_GetTotalSearchNum(S_MA_DRIVER *qma)
{
	return qma->rgnChTotalCount;
}
#endif