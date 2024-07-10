/************************************************Copyright(c)***************************************************
**                                   Quintc(Shenzhen) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmafunc.c
** Last Modified Date: 
** Last Version:     
** Descriptions: qmafunc.c used for register setting related function
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-27
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
       
#if defined(QN_SIM) 
#if !defined(_USRDLL)
#include "..\stdafx.h"       
#endif 
#include "qmacommon.h"
#endif 

#include "qmadriver.h"
#include "qmafunc.h"
#include "fmdriver.h"

const S_MA_FREQ_SETTING qmaTVCHSetting[QNM_IMR_NUM][QNM_FREQ_BAND_MAX] =
{ 
    {	       
        {70525, 90375, 0xc0, 0x40, 0x20, 0x00, 0, 2, 36   },			
        {58125, 70525, 0xc0, 0x00, 0x20, 0x00, 0, 2, 36   },		
        {47125, 58125, 0xc0, 0x40, 0x00, 0x00, 0, 2, 36   },		
        {19455, 23975, 0x80, 0x00, 0x00, 0x10, 0,  4, 36  }, 		
        {17725, 19455, 0x80, 0x40,  0x20, 0x00, 0, 8, 36  }, 		
        {16725, 17725, 0x80, 0x00,  0x20, 0x00, 0, 8, 36  }, 		
        {8925, 10975,  0x40, 0x40,  0x20, 0x10, 0, 16, 36 }, 		
        {7375, 8925, 0x40, 0x00, 0x20, 0x10, 0, 16, 36    }, 
        {5995, 7375, 0x40, 0x40, 0x00, 0x10, 0, 16, 36    }, 		
        {5025, 5995, 0x40, 0x00, 0x00, 0x10, 0, 16, 36    }, 		
        {4525, 5025, 0x40, 0x40, 0x20, 0x00, 0, 32, 36    }, 		
    }, 
    {          
        {69925, 90375, 0xc0, 0x40, 0x20, 0x00, 6, 2, 36  }, 				
        {57525, 69925, 0xc0, 0x00, 0x20, 0x00, 6, 2, 36  }, 			
        {47125, 57525, 0x80, 0x40, 0x00, 0x00, 6, 2, 36  }, 			
        {18855, 23975, 0x80, 0x00, 0x00, 0x10, 6, 4, 36  }, 			
        {17125, 18855, 0x80, 0x40, 0x20, 0x00, 6, 8, 36  }, 			
        {16725, 17125, 0x40, 0x00, 0x20, 0x00, 6, 8, 36  }, 			
        {8325, 10975,  0x40, 0x40, 0x20, 0x10, 6, 16, 36 }, 			
        {6775, 8325, 0x40, 0x00, 0x20, 0x10, 6, 16, 36   }, 			
        {5395, 6775, 0x40, 0x40, 0x00, 0x10, 6, 16, 36   }, 			
        {4425, 5395, 0x40, 0x00, 0x00, 0x10, 6, 16, 36   }, 			
    },     
    {          
        {69825, 90375, 0xc0,  0x40,  0x20,  0x00,  7,  2,  36}, 					
        {57425, 69825, 0xc0,  0x00,  0x20,  0x00,  7,  2,  36}, 				
        {47125, 57425, 0xc0,  0x40,  0x00,  0x00,  7,  2,  36}, 				
        {18755, 23975, 0x80,  0x00,  0x00,  0x10,  7,  4,  36}, 				
        {17025, 18755, 0x80,  0x40,  0x20,  0x00,  7,  8,  36}, 				
        {16725, 17025, 0x80,  0x00,  0x20,  0x00,  7,  8,  36}, 				
        {8225, 10975, 0x40,  0x40,  0x20,  0x10,  7,  16,  36}, 				
        {6675, 8225, 0x40,  0x00,  0x20,  0x10,  7,  16,  36}, 				
        {5295, 6675, 0x40,  0x40,  0x00,  0x10,  7,  16,  36}, 				
        {4325, 5295, 0x40,  0x00,  0x00,  0x10,  7,  16,  36}, 				
    }, 		
    {          
        {69725, 90375, 0xc0, 0x40, 0x20,  0x00,  8,  2,  36}, 				
        {57325, 69725, 0xc0, 0x00, 0x20,  0x00,  8,  2,  36}, 			
        {47125, 57325, 0xc0, 0x40, 0x00,  0x00,  8,  2,  36}, 			
        {18655, 23975, 0x80, 0x00, 0x00,  0x10,  8,  4,  36}, 			
        {16925, 18655, 0x80, 0x40, 0x20,  0x00,  8,  8,  36}, 			
        {16725, 16925, 0x80, 0x00, 0x20,  0x00,  8,  8,  36}, 			
        {8125, 10975,  0x40, 0x40, 0x20,  0x10,  8,  16,  36}, 			
        {6575, 8125,   0x40, 0x00, 0x20,  0x10,  8,  16,  36}, 			
        {5195, 6575,   0x40, 0x40, 0x00,  0x10,  8,  16,  36}, 			
        {4225, 5195,   0x40, 0x00, 0x00,  0x10,  8,  16,  36}, 			
    },   
    {          
        {69725, 90325, 0xc0, 0x40, 0x20,  0x00,  8,  2,  36}, 				
        {57325, 69725, 0xc0, 0x00, 0x20,  0x00,  8,  2,  36}, 			
        {47125, 57325, 0xc0, 0x40, 0x00,  0x00,  8,  2,  36}, 			
        {18755, 23925, 0x80,  0x00,  0x00,  0x10,  7,  4,  36}, 				
        {17025, 18755, 0x80,  0x40,  0x20,  0x00,  7,  8,  36}, 				
        {16725, 17025, 0x80,  0x00,  0x20,  0x00,  7,  8,  36}, 				
        {8225, 10925, 0x40,  0x40,  0x20,  0x10,  7,  16,  36}, 				
        {6675, 8225, 0x40,  0x00,  0x20,  0x10,  7,  16,  36}, 				
        {5295, 6675, 0x40,  0x40,  0x00,  0x10,  7,  16,  36}, 				
        {4325, 5295, 0x40,  0x00,  0x00,  0x10,  7,  16,  36}, 	
    },     

};

const S_MA_RESOLUTION ATV_resolutionMap[QNM_RESOLUTIOM_COUNT] = 
{
    { 320, 240, 0x44, 0x20, 0x10, 0x40, }, 
	{ 360, 240, 0x3c, 0x20, 0x20, 0x40, },
    { 400, 240, 0x36, 0x20, 0x20, 0x40, }, 
    { 640, 240, 0x22, 0x20, 0x20, 0x40, }, 
    { 176, 220, 0x7b, 0x22, 0x10, 0x40, }, 
	{ 160, 128, 0x88, 0x3c, 0x00, 0x40, },
    { 176, 144, 0x7b, 0x35, 0x10, 0x40, },	
	{ 220, 176, 0x62, 0x2b, 0x10, 0x40, },
    { 352, 288, 0x3d, 0x1a, 0x20, 0x22, }, 
    { 480, 300, 0x2d, 0x19, 0x20, 0x22, }, 
    { 640, 300, 0x22, 0x19, 0x20, 0x11, }, 	
};

const UINT32  Nbw[33] = 
{ 	
    0x80000,        
    0x80377,        
    0x80DD9,        
    0x81F1D,        
    0x83734,        
    0x8560A,        
    0x87B82,        
    0x8A77E,        
    0x8D9D7,        
    0x91261,        
    0x950EC,        
    0x99540,        
    0x9DF24,        
    0xA2E57,        
    0xA8294,        
    0xADB92,        
    0xB3905,        
    0xB9A9B,        
    0xC0000,        
    0xC68DC,        
    0xCD4D4,        
    0xD438B,        
    0xDB4A1,        
    0xE27B3,        
    0xE9C5E,        
    0xF123E,        
    0xF88EB,        
    0x00000,        
    0x07714,        
    0x0EDC1,        
    0x163A1,        
    0x1D84C,        
    0x24B5E,        
};       

const S_MA_TF_COEFF ATV_TFParam[QNM_TF_LEVEL_MAX] = 
{
#if(QMA_ATV_REDUCE_SHAKE)
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, }, 
    {0x70, 0x60, 0x30, 0x60, 0x30, 0x60, 0x30, }, 
    {0x60, 0x50, 0x38, 0x50, 0x38, 0x50, 0x38, }, 
    {0x60, 0x40, 0x20, 0x40, 0x20, 0x40, 0x20, }, 
    {0x50, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, }, 
    {0x40, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, }, 
    {0x40, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, }, 
    {0x40, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, }, 
    {0x30, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, }, 
#else
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, }, 
    {0x70, 0x60, 0x30, 0x60, 0x30, 0x60, 0x30, }, 
    {0x60, 0x50, 0x28, 0x50, 0x28, 0x50, 0x28, }, 
    {0x60, 0x40, 0x20, 0x40, 0x20, 0x40, 0x20, }, 
    {0x50, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, }, 
    {0x40, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, }, 
    {0x40, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, }, 
    {0x40, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, }, 
    {0x30, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, }, 
#endif
};

/**********************************************************************
void QMA_SetSysClock(S_MA_DRIVER *qma,UINT8 on)
**********************************************************************
Description: 
    Set system clock source, waveform and frequency. call it in initialization function

Parameters:
        clkSource:       
           QMA_USING_CRYSTAL: using crystal as system clock source
           QMA_USING_EXTERNAL_CLOCK: using external clock as system clock source
        clkWaveForm: 
           QMA_RECTANGULAR_WAVEFORM: external input clock waveform is rectangular
           QMA_SINUSOID_WAVEFORM: external input clock waveform is sinusoid
        clkFreq: 
           unit: Hz, eg: 26000000 represents 26Mhz 
           crystal or external input clock frequency
           
Return Value:
        None
note:         32.768KHz <= clkFreq <=38.4MHz
**********************************************************************/
UINT8 QNF_SetSysClock(S_MA_DRIVER *qma, UINT8 clkSource, UINT8 clkWaveform, UINT32 clkFreq)
{
    UINT32 xtal_div;
    UINT32 freq_flg;
    UINT32 dlt;
    UINT8 div2,div2_lsb,div2_msb;
    UINT8 flg,div3=0;
    
    if(qma->dev == QNM_DEV_ATV)
    {
        qma->sysClk = clkFreq;
    }
    else
    {
        qma->fm->sysClk = clkFreq;
    }    
    
    if(clkSource == QMA_USING_EXTERNAL_CLOCK)
    {
        if(clkWaveform == QMA_SINUSOID_WAVEFORM)
        {
            QNF_SetData(0x5d10,0x10);
        }        
        QNF_SetData(0x42c0,clkWaveform);
    } 
    
    if((clkFreq<QNM_CRYSTAL_LOWTH) || (clkFreq>QNM_CRYSTAL_HIGHTH)) return 0;
    
    QNF_SetData(QNR_CHANNEL_CONFIG1_CLK2_EN,QNR_CLK2_EN);
	flg = (UINT8)((clkFreq <= QNM_CRYSTAL_MIDTH && clkFreq >= QNM_CRYSTAL_LOWTH)? 1:2);	
    QNF_SetData(QNR_XTAL_1_DIV,(UINT8)((flg == 1)? QNR_XTAL_1_DIV_1:QNR_XTAL_1_DIV_2));
    xtal_div=((clkFreq*2)/(QNM_CRYSTAL_LOWTH*flg)+1)/2;
    QMA_WriteReg(QNR_REF_DIV_1,(UINT8)(xtal_div&0xff));
    div2_lsb=(UINT8)((xtal_div>>8)&0x07);    
    
    if(clkFreq%QNM_10KHZ_UNIT == 0 )
    {
        freq_flg=clkFreq/QNM_10KHZ_UNIT;
        dlt = (QNM_CONST_COEFF4*2*xtal_div*flg/freq_flg+1)/2-QNM_CONST_COEFF3;
        div2_msb=(UINT8)((dlt>>5)&0xf8);
        div3=(UINT8)(dlt&0xFF);
    }
    else
    {
        div2_msb=CLK_DLT_MSB&0xF8;	  
        div3=CLK_DLT_LSB;
    }    
    div2=(UINT8)(div2_lsb|div2_msb);
    QMA_WriteReg(QNR_REF_DIV_2,div2);
    QMA_WriteReg(QNR_REF_DIV_3,div3);
    
	if(clkFreq == 32768)
	{
		QNF_SetData(0x4180,0x80);
 		QMA_Delay(1500);
		if(qma->dev == QNM_DEV_ATV)
		{
			QNF_SetData(0x4d80,0x80); 		
			QNF_SetData(0x4d80,0x00);
 		    QMA_Delay(20);
		}
		else
		{
 			QNF_SetData(0x0040,0x40);        
 			QNF_SetData(0x0040,0x00); 
 		    QNF_GetRxState(qma);
		} 		       
	}     
	else	
    {
        QMA_Delay(10);
        QNF_SetData(0x0040,0x40);        
        QNF_SetData(0x0040,0x00); 
        QNF_GetRxState(qma);
    }    
    return 1;
}

/**********************************************************************
    void QMA_SetIMR(S_MA_DRIVER *qma, UINT8 on)
**********************************************************************
Description: 
    This function configures the register specified bit
    for image rejection, before setting the TV channel.

Parameters:
    qma : the point of struct S_MA_DRIVER

    imr : 0/1 : IMR turn off/on
        
Return value :
    None
**********************************************************************/
void QNF_SetIMR(S_MA_DRIVER *qma, UINT8 imr)
{
    qma->IMR = imr;
    QNF_SetData(QNR_SYSTEM2_IMR, (UINT8)(imr<<7));
}

/**********************************************************************
    void QNF_SetTVBW(S_MA_DRIVER *qma, UINT8 on)
**********************************************************************
Description: 
    set register specified bit

Parameters:
    qma : the point of struct S_MA_DRIVER

    tvBW(0-3) : based on TV system setting
    
Return value :
    None
**********************************************************************/
void QNF_SetTVBW(S_MA_DRIVER *qma, UINT8 tvBW)
{
    qma->tvBW = tvBW;
}

/*********************************************************************
    void QNF_SetData(UINT16 reg, UINT8 val)
**********************************************************************
Description : 
    set register specified bit
    
Parameters:
    reg : 
        MSB of reg is for reg address
        LSB of reg is for mask specified bit
        
  val : data will be set for specified bit
  
Return value:
     none
**********************************************************************/
void QNF_SetData(UINT16 reg, UINT8 val)
{
    UINT8 addr, bitmask, temp;
    
    addr = (UINT8)(reg>>8);
    bitmask = (UINT8)(reg&0xff);
    temp = QMA_ReadReg(addr);
    temp = (UINT8)(temp&~bitmask);
    temp |= val&bitmask;
    QMA_WriteReg(addr, temp);
}

/*********************************************************************
    void QNF_SetData(UINT16 reg, UINT8 val)
**********************************************************************
Description : 
    set register specified bit
    
Parameters :
    reg : MSB of reg is for reg address
            LSB of reg is for mask specified bit
        
Return value :
    the data will be set for specified bit
**********************************************************************/
UINT8 QNF_GetData(UINT16 reg)
{
	UINT8 addr, bitmask, temp;

	addr = (UINT8)(reg>>8);
	bitmask = (UINT8)(reg&0xff);
	temp = QMA_ReadReg(addr);
	
	return (UINT8)(temp&bitmask);
}

/*********************************************************************
               void QNF_GetCHFreq(S_MA_DRIVER *qma, UINT8 chNo)
**********************************************************************
Description: 
    Get channel frequency by channel no
    
Parameters:
    qma : the point of struct S_MA_DRIVER
    chNo: Channel no 
Return value :
    channel frequency
**********************************************************************/
UINT32 QNF_GetCHFreq(S_MA_DRIVER *qma, UINT8 chNo)
{
    UINT8 i;
    UINT8 rgn=(UINT8)qma->regionCode;
    UINT32 chFreq = 0;

    for(i=0;i<qma->rgnChTotalCount;i++)
    {
        if (chNo == ATV_ChMap[rgn][i].chNo)
        {
            chFreq=ATV_ChMap[rgn][i].freq;
            break;
        }		
    }	
    return chFreq;
}

/*********************************************************************
               void QNF_GetCHNo(S_MA_DRIVER *qma, UINT8 index)
**********************************************************************
Description: 
    Get Channel No by index 
    
Parameters:
    qma : the point of struct S_MA_DRIVER
    index: usually from 0 ~ 69 
Return value :
    channel no
**********************************************************************/
UINT8 QNF_GetCHNo(S_MA_DRIVER *qma, UINT8 index)
{
    UINT8 rgn=(UINT8)qma->regionCode;
    return ATV_ChMap[rgn][index].chNo;
}

/*********************************************************************
    void QNF_InitVariable(S_MA_DRIVER *qma)
**********************************************************************
Description: 
    setting init status of struct S_MA_DRIVER, called only by QNM_Init()
    
Parameters:
    qma : the point of struct S_MA_DRIVER
  
Return value :
    none
**********************************************************************/
void QNF_InitVariable(S_MA_DRIVER *qma)
{
    UINT16 rgn_tmp = qma->regionCode;
    UINT8 tvstd_tmp=ATV_ChMap[rgn_tmp][0].tvStd;
    UINT8 chnbw_index = (UINT8)((tvstd_tmp &QNM_CHN_BW_MASK)>>2);
    
    qma->IMR = 0;
    qma->tvBW = chnbw_index;
    qma->prevFreq = 0;
    qma->chCount = 0;
    qma->volume = 0x1c;// 0x17~0x1c : 0db~5db
	qma->isPs = 0;// PAL
    
    if((QNM_REGION_WESTERNEUROPE == rgn_tmp)||(QNM_REGION_IRAQ == rgn_tmp))
    {
        qma->tvBW = 4;
    }
    
    if((qma->mode == QNM_MODE_WORKING)&&(QNM_DEV_FM == qma->dev))
    {		
		qma->fm->autoScanAll = 0;
        switch(rgn_tmp)
        {	
            case QNM_REGION_CHINA:			    		 
            case QNM_REGION_SHENZHEN:			    		
            case QNM_REGION_HONGKONG:
            case QNM_REGION_VIETNAM:
            case QNM_REGION_WESTERNEUROPE:
            case QNM_REGION_BURMA:
            case QNM_REGION_BRAZIL:
            case QNM_REGION_ARGENTINA:
			case QNM_REGION_IRAQ:
			case QNM_REGION_ITALY:
            case QNM_REGION_RUSSIA:			    		
                qma->fm->chStart = QNM_FM_CH_START01;
                qma->fm->chStop = QNM_FM_CH_STOP01;
                qma->fm->chStep = QNM_FM_CH_STEP_100KHZ;			    					    					 
                break;	
            case QNM_REGION_USA:
            case QNM_REGION_AUSTRALIA:
            case QNM_REGION_NEWZEALAND:			    		
                qma->fm->chStart = QNM_FM_CH_START01;
                qma->fm->chStop = QNM_FM_CH_STOP02;
                qma->fm->chStep = QNM_FM_CH_STEP_200KHZ;			    					    		
                break;
            case QNM_REGION_JAPAN:			    	
                qma->fm->chStart = QNM_FM_CH_START02;
                qma->fm->chStop = QNM_FM_CH_STOP03;
                qma->fm->chStep = QNM_FM_CH_STEP_100KHZ;			    					    		
                break;				    
            default:
                break;
        }
    }
}

/*********************************************************************
    void QNF_InitChip(S_MA_DRIVER *qma, UINT8 dev)
**********************************************************************
Description: 
    The function handles first step initialization chip as PAL system,  called only by QMA_Init()
    
Parameters:
    qma : the point of struct  S_MA_DRIVER   

    dev:
         FM/ATV
    
Return value :
    none
**********************************************************************/
void QNF_InitChip(S_MA_DRIVER *qma, UINT8 dev)
{    
	QMA_WriteReg(0x00,0x14);
    QMA_Delay(10);    
	QMA_WriteReg(0x00,0x04);
    QMA_Delay(10);    
	QMA_WriteReg(0x00,0x00);
    QMA_Delay(10);    
	QMA_WriteReg(0x00,0x02);
    QMA_Delay(10);	
	QMA_WriteReg(0x00,(UINT8)(0x02|dev));
    QMA_WriteReg(0x1d, 0xf0);   
	QMA_WriteReg(0x00,(UINT8)(0x22|dev));
    QMA_Delay(20);   
}

/**********************************************************************
    UINT8 QNF_GetRgnChCount(S_MA_DRIVER *qma)
***********************************************************************
Description: 
    This function gets the UHF and VHF channel frequency list, according to the special region.
    To ready to seek channel.
			  
Parameters:
    qma: the point of struct S_MA_DRIVER
      
Return value:
    Channel count

Note:    
    the freq unit : 10KHz    
**********************************************************************/
UINT8 QNF_GetRgnChCount(S_MA_DRIVER *qma)
{
	UINT8 i;
	UINT8 ch_count=0;
    UINT16 region=qma->regionCode;
	
	for (i=0;i<120;i++)
	{
		if (ATV_ChMap[region][i].chNo != 0)
		{
			ch_count++;
		}
		else
		{
			break;
		}
	}
	
    return ch_count;
}

/**********************************************************************
      UINT8 QNF_GetATVIndex(S_MA_DRIVER *qma, UINT32 chfreq)
**********************************************************************
Description: 
    This function gets the special channel index, the index is from some region Tv channel 
    table, the function is used in UINT32 QMA_RXSeekCH as setting the start parameter.
 
Parameters:
    qma : the point of struct S_MA_DRIVER

    chfreq:
        the channel freq .
         
Return value:
     None
**********************************************************************/
UINT8 QNF_GetATVIndex(S_MA_DRIVER *qma, UINT32 chfreq)
{
	UINT8 i,rgn;
	UINT8 rgnChTatol=qma->rgnChTotalCount;
	rgn=(UINT8)qma->regionCode;
	
	for(i=0; i< rgnChTatol; i++)
	{
		if(chfreq <= ATV_ChMap[rgn][i].freq)
		{            
			return (UINT8)(i);       
		}
	}
	
	return 0xff;
}

/**********************************************************************
    UINT32 QNF_GetATVChannel(S_MA_DRIVER *qma, UINT32 chFreq) 
**********************************************************************
Description: 
    This function gets the special channel frequency at the channel list table.

Parameters:
   qma : the point of struct S_MA_DRIVER

   chFreq : 4975-87125
       Set the frequency (10KHz) to be tuned, 
       eg: 871.25MHz will be set to 87125.
       
Return value:
      None
**********************************************************************/
UINT32 QNF_GetATVChannel(S_MA_DRIVER *qma, UINT32 chFreq) 
{
    UINT8 idx;
    UINT8 rgn=(UINT8)(qma->regionCode);
    
    idx  =  QNF_GetATVIndex(qma, chFreq);
    
    return ATV_ChMap[rgn][idx].freq;
}

/*******************************************************************
UINT8 QNF_SetSysClock2(S_MA_DRIVER *qma, UINT8 clkSource, UINT8 clkWaveform, UINT32 freq)
********************************************************************
Description: the function sets the main clock worked for chip.
note : 

     freq:  38.4MHz >= freq > 12MHz
*********************************************************************/
UINT8 QNF_SetSysClock2(S_MA_DRIVER *qma, UINT8 clkSource, UINT8 clkWaveform, UINT32 freq)
{
	UINT32 tmp, LSB;
	UINT8 MSB;
	UINT32 freq_tmp = freq/QNM_KHZ_UNIT;
    // doesn't support 32768 
    if(freq == 32768)
    {
        return 0;
    }
	tmp = 0;
    
	if(qma->dev == QNM_DEV_ATV)
    {
        qma->sysClk = freq;
    }
    else
    {
        qma->fm->sysClk = freq;
    }    
    
    if(clkSource == QMA_USING_EXTERNAL_CLOCK)
    {
        if(clkWaveform == QMA_SINUSOID_WAVEFORM)
        {
            QNF_SetData(0x5d10,0x10);
        }
        
        QNF_SetData(0x42c0,clkWaveform);
    } 
	QNF_SetData(QNR_CHANNEL_CONFIG1_CLK2_EN, QNR_CLK2_DIS);
	
	if((freq > QNM_CRYSTAL_TH) &&(freq <= QNM_CRYSTAL_HIGHTH)) 
	{
		QNF_SetData(QNR_XTAL_1_DIV, QNR_XTAL_1_DIV_2);       
		tmp  =  (UINT32)(QNM_CONST_CLKDIV_COEFF1/freq_tmp*QNM_KHZ_UNIT);
		tmp = (UINT32)(tmp+(QNM_CONST_CLKDIV_COEFF1%freq_tmp)*QNM_KHZ_UNIT/freq_tmp);
	}
	else if((freq < QNM_CRYSTAL_TH)&&(freq >=  QNM_CRYSTAL_MIDTH))
	{
		QNF_SetData(QNR_XTAL_1_DIV, QNR_XTAL_1_DIV_1); 
		tmp  =  (UINT32)(QNM_CONST_CLKDIV_COEFF2/freq_tmp*QNM_KHZ_UNIT);
		tmp = (UINT32)(tmp+(QNM_CONST_CLKDIV_COEFF2%freq_tmp)*QNM_KHZ_UNIT/freq_tmp);
	}

	MSB  =  (UINT8)((tmp/QNM_FREQ_DECI)&0xff);	
	LSB  =  (UINT32)(tmp%QNM_FREQ_DECI)&0x0000ffff;   
	QMA_WriteReg(QNR_REF_DIV_1, MSB);
	QMA_WriteReg(QNR_REF_DIV_2, (UINT8)((LSB>>8)&0xff));
	QMA_WriteReg(QNR_REF_DIV_3, (UINT8)((LSB)&0xff));
	
	
	QMA_Delay(10);
	QNF_SetData(0x0040,0x40);	
	QNF_SetData(0x0040,0x00); 
    QNF_GetRxState(qma);
	return 1;
}

/****************************************************************************
        UINT8 QNF_GetRxState(S_MA_DRIVER *qma)
*****************************************************************************
 Description:
     This function get the status of work mode.
     
 Parameters: 
     qma : the point of struct S_MA_DRIVER.  
     
     
Return value:
      0: failed;
	  1: success;

******************************************************************************/
UINT8 QNF_GetRxState(S_MA_DRIVER *qma)
{
	UINT8 status,count=0;
	do
	{
		QMA_Delay(5);
		status =QNF_GetData(0x070e);
		if(0x0c == status) break;
		count++;
	}while(count <= 40);
	return (UINT8)((count > 40)? 0:1);
}

/****************************************************************************
    UINT8 QNF_SetNF1(S_MA_DRIVER *qma, UINT32 freq, UINT16 p)
*****************************************************************************
 Description:
     This function sets the notch filter 1 for the special CH frequency.
     
 Parameters: 
     qma : the point of struct S_MA_DRIVER.
     
     Freq(4975-87125) :    
         the frequency value ,  unit (10KHz).
     
     p :
         the member of the struct S_MA_FREQ_SETTING.
     
Return value:
     The integer of ch_msb
     
Note :  
    the function is served for QNF_SetATVCH
******************************************************************************/
UINT8 QNF_SetNF1(S_MA_DRIVER *qma, UINT32 freq, UINT16 p)
{
    UINT32 freq_int_tmp, freq_int;
    UINT8  nbw_index;
    UINT32 freq_dem_tmp = freq+p*QNM_P_MULTI-QNM_FREQ_BIAS;
    freq_int_tmp =  freq_dem_tmp*2/2700;
    freq_int = freq_int_tmp*1350;
    
    if (qma->IMR == 0)
    {
        freq_dem_tmp = freq_int + 1350 - freq_dem_tmp;
    }
    else
    {
        freq_dem_tmp = freq_dem_tmp-freq_int;
    }
    
    nbw_index = (UINT8)(freq_dem_tmp/25);
    
    if(nbw_index <= 32 && (freq_dem_tmp%25 == 0))
    {
        QNF_SetData(0x370f, (UINT8)((Nbw[nbw_index]&0xfffff)>>16));
        QMA_WriteReg(0x38, (UINT8)((Nbw[nbw_index]&0xff00)>>8));
        QMA_WriteReg(0x39, (UINT8)(Nbw[nbw_index]&0xff));
        QNF_SetData(0x3780, 0x00);
    }
    else
    {
        QNF_SetData(0x3780, 0x80);
    }
    
    return 1;
}

/******************************************************************************
    INT8 QNF_SetNF2(S_MA_DRIVER *qma, UINT32 crystal,  UINT32 freq, UINT16 p)
 ******************************************************************************
 Description:
     This function sets the notch filter 2 for the special CH frequency
     
 Parameters: 
     p :                   
         the member of  struct  S_MA_FREQ_SETTING.
     Freq : 
         the range: 4975-87125(10KHz)         
     crystal :             
         26MHz, 24MHz external crystal
     
 Return value:
     The integer of ch_msb
     
 Note : 
     the function is served for QNF_SetATVCH
 ******************************************************************************/
UINT8 QNF_SetNF2(S_MA_DRIVER *qma, UINT32 crystal,  UINT32 freq, UINT16 p)
{
    UINT32 freq_int_tmp, freq_int;
    UINT8  nbw_index;
    UINT16 freq_div;
    UINT32 freq_dem_tmp = freq+p*QNM_P_MULTI-QNM_FREQ_BIAS;
    
    if (crystal > 20000000)
    {
        freq_div  = (UINT16)((crystal/2)/10000);
    }
    else
    {
        freq_div = (UINT16)(crystal/10000);
    }
    freq_int_tmp =  freq_dem_tmp/freq_div;
    freq_int = freq_int_tmp*freq_div;
    if (qma->IMR == 0)
    {
        freq_dem_tmp = freq_int+freq_div-freq_dem_tmp;
    }
    else
    {
        freq_dem_tmp = freq_dem_tmp-freq_int;
    }
    nbw_index = (UINT8)(freq_dem_tmp/25);
    if(nbw_index <= 32 && (freq_dem_tmp%25 == 0))
    {
        QNF_SetData(0x3a0f, (UINT8)((Nbw[nbw_index]&0xfffff)>>16));
        QMA_WriteReg(0x3b, (UINT8)((Nbw[nbw_index]&0xff00)>>8));
        QMA_WriteReg(0x3c, (UINT8)(Nbw[nbw_index]&0xff));
        QNF_SetData(0x3a80, 0x00);
    }
    else
    {
        QNF_SetData(0x3a80, 0x80);
    }
    
    return 1;
}

/******************************************************************************
    UINT32 QNF_GetInteger(UINT32 freq, UINT16 p, UINT8 n, UINT8 m)
******************************************************************************
Description:
     This function get the integer part of setting TV Channel frequency for 0x42
             
Parameters: 
     Freq(4975-87125) :    the frequency value ,  unit (10KHz)
     p :           the member variable  p  of  struct  S_MA_FREQ_SETTING
     n :           the member variable  n  of  struct  S_MA_FREQ_SETTING
     m :           the member variable  m  of  struct  S_MA_FREQ_SETTING

Return value :
     The integer of ch_msb

Note : 
    the function is served for QNF_SetATVCH
******************************************************************************/
UINT32 QNF_GetInteger(UINT32 freq, UINT16 p, UINT8 n, UINT8 m)
{
	UINT32 freq_temp = freq-QNM_FREQ_BIAS;
	UINT32 p_temp = p*QNM_P_MULTI;
	UINT32 m_temp = m*QNM_P_MULTI;
	UINT32 temp = (freq_temp+p_temp)*n/m_temp;
	
	return temp;
}

/******************************************************************************
    UINT32 QNF_GetRemainder(UINT32 freq, UINT16 p, UINT8 n, UINT8 m)
******************************************************************************
Description:
    This function get the remainder part of setting TV Channel frequency for 0x43, 0x44, 0x45
    
 Parameters: 
      Freq(4975-87125) :       the frequency value ,  unit (10KHz)
      p :          the member variable  p  of  struct  S_MA_FREQ_SETTING
      n :          the member variable  n  of  struct  S_MA_FREQ_SETTING
      m :          the member variable  m  of  struct  S_MA_FREQ_SETTING

Return value :
    The remainder of ch_lsb

Note :   
    the function is served for QNF_SetATVCH
 ******************************************************************************/
UINT32 QNF_GetRemainder(UINT32 freq, UINT16 p, UINT8 n, UINT8 m)
{
	UINT32 freq_temp = freq-QNM_P_BIAS; 
	UINT32 p_temp = p*QNM_P_MULTI;
	UINT32 m_temp = m*QNM_P_MULTI;
	UINT32 Int_temp = QNF_GetInteger(freq, p, n, m);
	UINT32 temp = (freq_temp+p_temp)*n;

	temp = temp-Int_temp*m_temp;
	temp = temp*QNM_CH_DECI/m_temp;
	
	return temp;
}

/**********************************************************************
    UINT8 QNF_SetATVCH(S_MA_DRIVER *qma,  UINT32 freq)
**********************************************************************
Description : 
    REF CLK Configuration,  getting the ATV channel,  according to the freq.
    
 parameters:
    qma : the point of the struct S_MA_DRIVER
    
    freq(4975-87125): unit(10KHz)

eg:
    Divider Ratio 
    (0x42-0x45) <-(fRF*2/36MHz)
**********************************************************************/
UINT8 QNF_SetATVCH(S_MA_DRIVER *qma,  UINT32 freq)
{
    UINT8 i;
    UINT32 ch_msb_tmp, ch_lsb_tmp;
    UINT8 ch_msb, conf134, tvBW_index;
    UINT16 tuneReg_MSB;
    UINT32 ch_lsb;
    UINT8 idx  = (UINT8)(qma->IMR ? qma->tvBW : 0); 
    
    tvBW_index = qma->tvBW;
    tuneReg_MSB = QNR_TUNER_CTRL_BAND_SEL|QNR_TUNER_CTRL_DIV23_SEL|QNR_TUNER_CTRL_DIV24_SEL;
    
    
    if (freq == 18425)
    {
        QNF_SetData(0x3a0f, 0x0e);
        QMA_WriteReg(0x3b, 0x9b);
        QMA_WriteReg(0x3c, 0xfe);
        QNF_SetData(0x3a80, 0x00);
    }	
    else
    {
        QNF_SetData(0x3a80, 0x80);
    }
    
    for(i = 0; i < QNM_FREQ_BAND_MAX; i++)
    {
        if(freq >= qmaTVCHSetting[idx][i].freqBandLow && freq < qmaTVCHSetting[idx][i].freqBandHigh)
        {
            if ((qma->IMR == 1)&&(4 == tvBW_index))
            {
                QNF_SetData(QNR_TV_STD1_CHN_BW, (UINT8)(freq >= QNM_UHF_START_FREQ ? QNR_CHN_BW_8MHz:QNR_CHN_BW_7MHz));
            }
            
            QNF_SetNF1(qma, freq, qmaTVCHSetting[idx][i].p);
            
            if(qma->sysClk == 26000000)
            {
                QNF_SetNF2(qma,26000000,freq,qmaTVCHSetting[idx][i].p);
            }  
#if QMA_ELIMINATION_VERTICAL_STRIPE
            if(freq == QMA_ELIMINATION_VERTICAL_STRIPE_CH_0)
            {
                QNF_SetData(0x3a0f, (UINT8)((Nbw[QMA_ELIMINATION_VERTICAL_STRIPE_IDX]&0xfffff)>>16));
                QMA_WriteReg(0x3b, (UINT8)((Nbw[QMA_ELIMINATION_VERTICAL_STRIPE_IDX]&0xff00)>>8));
                QMA_WriteReg(0x3c, (UINT8)(Nbw[QMA_ELIMINATION_VERTICAL_STRIPE_IDX]&0xff));
                QNF_SetData(0x3a80, 0x00);
            }
#endif
            conf134 = (UINT8)(qmaTVCHSetting[idx][i].conf1|qmaTVCHSetting[idx][i].conf3|qmaTVCHSetting[idx][i].conf4);
            QNF_SetData(QNR_CHANNEL_CONFIG1_LO_VCO_SEL,  qmaTVCHSetting[idx][i].conf2); 
            
            QNF_SetData(tuneReg_MSB, conf134);
            ch_msb_tmp = QNF_GetInteger(freq, qmaTVCHSetting[idx][i].p, qmaTVCHSetting[idx][i].n, qmaTVCHSetting[idx][i].m);
            ch_lsb_tmp = QNF_GetRemainder(freq, qmaTVCHSetting[idx][i].p, qmaTVCHSetting[idx][i].n, qmaTVCHSetting[idx][i].m);
            ch_msb = (UINT8)((ch_msb_tmp)&0x3f);
            ch_lsb = (UINT32)(ch_lsb_tmp)&0x000fffff;			
            QNF_SetData(QNR_CHANNEL_CONFIG1_REG, ch_msb);			
            QNF_SetData(QNR_CHANNEL_CONFIG2_REG, (UINT8)((ch_lsb>>16)&0x000000ff));			
            QMA_WriteReg(QNR_CHANNEL_CONFIG3, (UINT8)((ch_lsb>>8)&0x000000ff));			
            QMA_WriteReg(QNR_CHANNEL_CONFIG4, (UINT8)(ch_lsb&0x000000ff)); 
            qma->curFreq = freq;	
            QMA_Delay(35);
            QNF_PatchBG(qma);
            
            return 1;       
        }	      
    }
    
    return 0;   
}

/**********************************************************************
    UINT8 QNF_ATVSeekChCond(S_MA_DRIVER *qma)
**********************************************************************
Description: 
     The function confirm whether it is valid channel,  it is used after QNF_SetATVCH().
     
Parameters:
    qma : the point of the struct S_MA_DRIVER
    
Return value :
    1 : the condition is successful
    0 : failed

Note: the following Para is for weak signal channels:
    if (WeakSignFlag == 1)
    {
	Reg0x79	 = 0xb7;
	Reg0x88	 = 0x40;
	AudAsymThMin  = 0x0a;
	RssiTh		 = 0x5c;
	VidLockCnt	 = 3;
	SyncLockCnt  = 3;
	AudAsymCnt	 = 3;
    }	 
**********************************************************************/
UINT8 QNF_ATVSeekChCond(S_MA_DRIVER *qma)
{
    UINT8 validCount,hAsymCount,count,flag,i;
    // by default, use strict parameters to get less weak channels
    UINT8 para_reg79 ,para_reg88,AudAsymThMax,AudAsymThMin,
    rssi_th,vid_lock_cnt,sync_lock_cnt,aud_asym_cnt,aud_asym;
#if QMA_CCA_SENSITIVITY_HIGH
	UINT8 diff_rssi,aud_rssi,rssi;		
#endif
    UINT8 RegVal;    
    
    
    RegVal=validCount=hAsymCount=count=flag=i=0;

#if QMA_CCA_SENSITIVITY_HIGH	
    para_reg79  = 0xbf;
    para_reg88  = 0x80;
    AudAsymThMax = 0x10;
    AudAsymThMin = 0x09;
    rssi_th     = 0x60;
    vid_lock_cnt  = 5;
    sync_lock_cnt = 4;
    aud_asym_cnt   = 4;    
#else    
    para_reg79  = 0xbf;
    para_reg88  = 0x9c;
    AudAsymThMax = 0x10;
    AudAsymThMin = 0x03;
    rssi_th     = 0x60;
    vid_lock_cnt  = 5;
    sync_lock_cnt = 4;
    aud_asym_cnt   = 4;    
#endif    
    
    QMA_WriteReg(0x76,0x55);    
    QMA_WriteReg(0x77,0x55);
    QMA_WriteReg(0xd6,0xf3);
    QMA_WriteReg(0x79,para_reg79);
    QMA_WriteReg(0x88,para_reg88);
    
    for (i=0; i<5; i++)
    {
        QMA_Delay(10);
        RegVal = QNF_GetData(QNR_STATUS2_VID_LOCK);
        #ifdef QN_SIM
        TRACE("QNF_GetData(QNR_STATUS2_VID_LOCK) = %d \n",RegVal);
        #endif
        if (RegVal==0)
        {
            break;            
        }
        else
        {
            validCount++;
        }
    }
    
    if (validCount >= vid_lock_cnt)
    {
        validCount=0;
        // check audio ASYM first
        for(count=0;count<5;count++)
        {    
            QMA_Delay(25);
            aud_asym = QMA_ReadReg(0xE5); 
            #ifdef QN_SIM
            TRACE("QMA_ReadReg(0xE5) = %d \n",aud_asym);
            #endif
            hAsymCount =(UINT8)(hAsymCount+((aud_asym >= AudAsymThMax)?1:0));
            if (hAsymCount >=2)
            {
                // if audio ASYM high enough, then valid channel
                flag=1;
                break;
            }
            validCount =(UINT8)(validCount+((aud_asym >= AudAsymThMin)?1:0));
            if (validCount>=aud_asym_cnt)
            {
                break;
            }            
        }        
        if(flag==0 && validCount>=aud_asym_cnt)
        {        
            validCount=0;
            for(count=0;count < 5;count++)
            {   
                QMA_Delay(20);
                RegVal = QNF_GetData(0x0807);
                #ifdef QN_SIM
                TRACE("QNF_GetData(0x0807) = %d \n",RegVal);
                #endif   
#if QMA_CCA_SENSITIVITY_HIGH
                validCount = (UINT8)(validCount+((RegVal >= 3)?1:0));
#else
				validCount = (UINT8)(validCount+(((RegVal >= 3)&&(0x07==(RegVal&0x07)))?1:0));
#endif
                if(validCount>=sync_lock_cnt)
                {
                    flag=1;
                    break;
                } 
            }
#if QMA_CCA_SENSITIVITY_HIGH
            if(flag==1)
            { 

                count=0;                
                for (i=0; i<3; i++)
                {
                    QMA_Delay(20);                    
                    rssi=QMA_ReadReg(0xd9);
                    aud_rssi=QMA_ReadReg(0xe3);
                    diff_rssi=(UINT8)((rssi > aud_rssi)?rssi-aud_rssi:aud_rssi-rssi);
                    #ifdef QN_SIM
                    TRACE("diff_rssi = %d \n",diff_rssi);
                    #endif
                    if ( diff_rssi >= 19)
                    {                       
					    flag = 0;
					    break;
					}
                }

            }
#endif
        }
    }
    #ifdef QN_SIM
    RegVal=QMA_ReadReg(0xe5);
    TRACE("aud asym = %d, ",RegVal);
    RegVal=QMA_ReadReg(0xe3);
    TRACE("aud rssi = %d, ",RegVal);
    RegVal=QMA_ReadReg(0xd9);
    TRACE("rf rssi = %d \n",RegVal);
    #endif       
    QMA_WriteReg(0x76,0x58);    
    QMA_WriteReg(0x77,0x5b);
    QMA_WriteReg(0x79,0xa9);    
    QMA_WriteReg(0x88,0x40);
 
    return flag;
}

/******************************************************************************
    void QNF_PatchBG(S_MA_DRIVER *qma)
******************************************************************************
Description:
    This function configures the re-calibration all blocks and LNA after tuning to the channel.
             
Parameters: 
    qma: the point of struct S_MA_DRIVER
             
Return value :
    none
******************************************************************************/
void QNF_PatchBG(S_MA_DRIVER *qma)
{
    if ((QMA_ReadReg(0x07)&0x0e) == 0x08)
    {
        #ifdef QN_SIM
        TRACE("Powerdown BG-VREG-\n");
        #endif
        QNF_SetData(0x0010, 0x10);
        QMA_Delay(50);
        QNF_SetData(0x0010, 0x00);
        QMA_Delay(50);
    }    
} 

/******************************************************************************
    void QNF_PatchClrField(S_MA_DRIVER *qma)
******************************************************************************
Description : 
    This function configures the re-calibration all blocks and LNA after tuning to the channel.
             
Parameters: 
    qma : the point of struct S_MA_DRIVER
             
Return value :
    none
******************************************************************************/
void QNF_PatchClrField(S_MA_DRIVER *qma)
{
   QNF_SetData(0x717f, 0x2e);
   QNF_SetData(0x717f, 0x3e);
} 

/******************************************************************************
    void QNF_AudioAutoDetction(S_MA_DRIVER *qma)
******************************************************************************
Description : 
     This function automatically seeks the valid channel audio,  
     used in where is after QMA_TuneToCH().
             
Parameters: 
    qma: the point of struct S_MA_DRIVER
             
Return value:
    none
******************************************************************************/
void QNF_AudioAutoDetction(S_MA_DRIVER *qma)
{
	UINT8 sub_std_loop,rege6_low,rege7,rege8;	
	UINT8 min0,max0,min_index,sub_min_index;
	UINT32 ifc,ifc_percent;	
    UINT32 ifc_error[4]={0};	
	
	if(0x02 == (QMA_ReadReg(0x02)&0x03))
	{	
	    for (sub_std_loop = 1;sub_std_loop < 4; sub_std_loop++)
		{         
		    QNF_SetData(0x02c0,(UINT8)(sub_std_loop<<6));	
		    QNF_SetData(0xdb07,0x00);
		    QMA_WriteReg(0xe0,0x00);	
		    QMA_WriteReg(0xe0,0x80);			
		    QMA_Delay(20);
	        rege6_low = QNF_GetData(0xe60f);
		    rege7 = QMA_ReadReg(0xe7);            
		    rege8 = QMA_ReadReg(0xe8);
		    ifc = (UINT32)((rege6_low<<16)|(rege7<<8)|rege8);			
            ifc_percent = 2000*ifc/16875;//(16875=843.75*20);		
		    ifc_error[sub_std_loop] = 1000 > ifc_percent? 1000-ifc_percent : ifc_percent-1000;     
		}         
		min0 = (UINT8)(ifc_error[1] < ifc_error[2]? 1:2);
		max0 = (UINT8)(ifc_error[1] > ifc_error[2]? 1:2);
		min_index = (UINT8)(ifc_error[min0] < ifc_error[3]? min0:3);
		if(min_index == 3)
		{
			sub_min_index = min0;
		}
		else
		{
			sub_min_index = (UINT8)(ifc_error[max0] < ifc_error[3]? max0:3);
		}		
		if((8*ifc_error[min_index] > 6*ifc_error[sub_min_index])&&(ifc_error[sub_min_index]-ifc_error[min_index] < 30))
		{
            QNF_SetData(0x02c0, ATV_ChMap[qma->regionCode][0].tvStd);			
		}
		else
		{
	        QNF_SetData(0x02c0, (UINT8)(min_index << 6));		
		}
	}	
} 

/******************************************************************************
    void QNF_AudioAutoDetction1(S_MA_DRIVER *qma, UINT32 freq)
******************************************************************************
Description:
    This function automatically seeks the valid channel audio,  
    used in where is after QMA_TuneToCH().
             
Parameters: 
    qma : the point of struct S_MA_DRIVER.
             
Return value:
    none
******************************************************************************/
void QNF_AudioAutoDetction1(S_MA_DRIVER *qma, UINT32 freq)
{
    UINT8  i,reg_0x76,reg_0x77,reg_0x79,reg_0x02;
    UINT8  clk_flg,count,flag;
    
    clk_flg=0;
    count=0;
    flag=0;
    reg_0x76=QMA_ReadReg(0x76);
    reg_0x77=QMA_ReadReg(0x77);
    reg_0x79=QMA_ReadReg(0x79);
	reg_0x02=QMA_ReadReg(0x02);
    
    QMA_WriteReg(0x76, 0x55);
    QMA_WriteReg(0x77, 0x55);
    QMA_WriteReg(0x79, 0xa9);
	
    for (i = 1;i<4;i++)
    {
        QNF_SetATVCH(qma, freq+500+50*i);       
        do 
        {
            QMA_Delay(30);
            clk_flg = (UINT8)(QMA_ReadReg(0x08)&0x01);		 
            count++;
        } while (clk_flg<1 && count<10);
        if (clk_flg)
        {
            flag = 1;
            QNF_SetData(0x02c0, (UINT8)(i<<6));
            break;
        }
    }
    if (flag  ==  0)
    {
        QNF_SetData(0x02c0, reg_0x02);
    }
    
    QMA_WriteReg(0x76,reg_0x76);
    QMA_WriteReg(0x77,reg_0x77);
    QMA_WriteReg(0x79,reg_0x79);
} 

/**********************************************************************
    void QNF_SetATVStd(S_MA_DRIVER *qma)
***********************************************************************
Description : 
    This function configures the parameters of ATV standard.
    for ex : audio carrier,  chroma carrier,  video band width, 
		 and TV system,  before entering tuning to channel.
		 
Parameters:
    qma : the point of struct S_MA_DRIVER.
    
Return value:
    None     
**********************************************************************/
void QNF_SetATVStd(S_MA_DRIVER *qma, UINT8 tvstd)
{
    UINT8 tv_standard= (UINT8)(tvstd&0x03);
    
    QMA_WriteReg(QNR_TV_STD1,tvstd);
    
    if (QNR_TV_STD1_VID_STD_PALD == tv_standard)
    {
        QMA_WriteReg(0x81, 0x4d);
        QMA_WriteReg(0x82, 0x94);//0x91);
    }
    else if (QNR_TV_STD1_VID_STD_PALM == tv_standard || QNR_TV_STD1_VID_STD_NTSC == tv_standard)
    {
        QMA_WriteReg(0x81, 0x4d);
        QMA_WriteReg(0x82, 0x80);
    }
    else        
    {
        QMA_WriteReg(0x81, 0x6e);
        QMA_WriteReg(0x82, 0x95);
    }    
}

/**********************************************************************
    void QNF_ATVAudDecSel(S_MA_DRIVER *qma)
***********************************************************************
Description : 
    This function configures the parameters of ATV standard.
    for ex:  audio carrier,  chroma carrier,  video band width, 
		 and TV system,  before entering tuning to channel.
		 
Parameters:
    qma: the point of struct S_MA_DRIVER

Return value:
    None     
**********************************************************************/
void QNF_ATVAudDecSel(S_MA_DRIVER *qma, UINT8 tvstd)
{  
    if(tvstd&0x02)
    {
        QNF_SetData(0x0380, 0x00);       
        QNF_SetTVAudSTD(QNM_TV_AUD_STD_A2);
    }
    else
    {
        QNF_SetData(0x0380, 0x80);       
        QNF_SetTVAudSTD(QNM_TV_AUD_STD_BTSC);
    }
}

/******************************************************************************
    void QNF_SetGPIOEn(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 option)
 ******************************************************************************
 Description : 
     This function sets GPIO 0~1 input or output,  enable or disable
             
 Parameters: 
    qma : the point of struct S_MA_DRIVER
__________________________________________________________________
|                  gpio     |             option                  |
|___________________________|_____________________________________|
|    QNR_GPIO_DATA_GPIO1_IE | QNR_GPIO1_IE_DIS / QNR_GPIO1_IE_EN  |     
|    QNR_GPIO_DATA_GPIO0_IE | QNR_GPIO0_IE_DIS / QNR_GPIO0_IE_EN  |    
|    QNR_GPIO_DATA_GPIO1_OE | QNR_GPIO1_OE_DIS/ QNR_GPIO1_OE_EN   |
|    QNR_GPIO_DATA_GPIO0_OE | QNR_GPIO0_OE_DIS/ QNR_GPIO0_OE_EN   |
|_________________________________________________________________|

Return value :
    none
******************************************************************************/
void QNF_SetGPIOEn(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 option)
{
    QNF_SetData(gpio, option);
}

/******************************************************************************
    void QNF_SetGPIOData(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 data)
******************************************************************************
Description : 
    This function sets GPIO 0~1 data value, only after setting enabling PGIO input or output.
             
Parameters : 
    qma : the point of struct S_MA_DRIVER

_________________________________________________________________________
|			   gpio           | 				option	                 |
|____________________________ |__________________________________________|
|    QNR_GPIO_DATA_GPIO1_OUT  | QNR_GPIO1_OUT_LOW / QNR_GPIO1_OUT_HIGH   |	   
|    QNR_GPIO_DATA_GPIO0_OUT  | QNR_GPIO0_OUT_LOW / QNR_GPIO0_OUT_HIGH   |	  
|________________________________________________________________________|
				
Return value:
    none
 ******************************************************************************/
void QNF_SetGPIOData(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 data)
{
   QNF_SetData(gpio, data);
}

/******************************************************************************
    void QNF_SetGPIO01Select(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 data)
******************************************************************************
Description : 
    This function sets GPIO 0~1 different active. It is called before enabling GPIO2~3.
             
Parameters: 
    qma : the point of struct S_MA_DRIVER

____________________________________________________________________________________
|			   gpio   |			option					                          |
|__________________|_______________________________________________________________|
|	 QNR_GPIO1_SEL  | QNR_GPIO1_SEL_INT / QNR_GPIO1_SEL_FIELD /QNR_GPIO1_SEL_GPIO    |	   
|	 QNR_GPIO0_SEL  | QNR_GPIO0_SEL_SEB / QNR_GPIO0_SEL_CLKOUT/QNR_GPIO0_SEL_GPIO |	  
|___________________________________________________________________________________|

Return value :
    none
******************************************************************************/
void QNF_SetGPIO01Select(S_MA_DRIVER *qma,  UINT16 gpio, UINT8 data)
{
    QNF_SetData(gpio, data);
}

/******************************************************************************
    UINT8 QNF_GetGPIOStatus(S_MA_DRIVER *qma, UINT16 gpio)
******************************************************************************
Description : 
    This function return to the status of GPIO0~1. 
    the parameters setting may refer to QNF_SetGPIOEn(), 
    QNF_SetGPIOData(), and QNF_SetGPIO01Select().
             
Parameters: 
    qma: the point of struct S_MA_DRIVER

 ________________________________________________________________
 | 			 gpio	|		 Return value  						|
 |___________________|___________________________________________|
 |    QNR_GPIO_DATA_GPIO1_I  | QNR_GPIO1_I_LOW / QNR_GPIO1_I_HIGH |	 
 |    QNR_GPIO_DATA_GPIO0_I  | QNR_GPIO0_I_LOW / QNR_GPIO0_I_HIGH |	
 |_______________________________________________________________|

Return value:
       value
******************************************************************************/
UINT8 QNF_GetGPIOStatus(S_MA_DRIVER *qma, UINT16 gpio)
{
    return QNF_GetData(gpio);
}

/**********************************************************************
    UINT16 QNF_GetATVBW(S_MA_DRIVER *qma)
***********************************************************************
Description : 
    This function gets the channel band width, according to the special region.
    
Parameters:
    qma : the point of struct S_MA_DRIVER.
    
Return value:
    the value unit : 10KHz    
**********************************************************************/
UINT16 QNF_GetATVBW(S_MA_DRIVER *qma)
{
    UINT8 region = (UINT8)qma->regionCode;
    UINT8 chnbw;
    UINT8 tvstd_temp;
    UINT16 bw_freq;
    
    tvstd_temp=ATV_ChMap[region][0].tvStd;
    chnbw = (UINT8)(tvstd_temp &QNM_CHN_BW_MASK);
    if(chnbw == QNR_CHN_BW_6MHz)
    {
        bw_freq = QNM_CHN_BW_6MHZ;
        return bw_freq;
    }
    else if(chnbw == QNR_CHN_BW_7MHz)
    {
        bw_freq = QNM_CHN_BW_7MHZ;
        return bw_freq;
    }
    else 
    {
        bw_freq = QNM_CHN_BW_8MHZ;
        return bw_freq;
    }
}

/******************************************************************************
    void QNF_PowerDown(S_MA_DRIVER *qma)
 ******************************************************************************
 Description : 
     This function sets the power down mode,  the system does not work any more, 
     and enters saving power state.
     
 Parameters: 
     qma : the point of struct S_MA_DRIVER
     
 Return value :
     none
 ******************************************************************************/
void QNF_PowerDown(S_MA_DRIVER *qma)
{
	QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);
	QMA_Delay(500);
	QNM_SetVideoOutEnable(0);
	QMA_Delay(100); 

	QNF_SetData(QNR_PCLK_CTRL_PCLK_CTRL, QNR_PCLK_CTRL_TRI_STATE);
	QNF_SetData(QNR_PCLK_CTRL_DOUT_CTRL, QNR_DOUT_CTRL_TRI_STATE);	
	QNF_SetData(0x0014,0x14);
	QNF_SetData(QNR_SYSTEM1_RX_REQ, QNR_RX_REQ_EXIT);       
}

/******************************************************************************
     void QNF_RestartFromPowerDown(S_MA_DRIVER *qma)
 ******************************************************************************
 Description : 
     This function powers up and restart to work normally after it has been
     put into its saving power state
     
 Parameters: 
     qma: the point of struct S_MA_DRIVER.
     
 Return value:
       none
 ******************************************************************************/
void QNF_RestartFromPowerDown(S_MA_DRIVER *qma)
{	
	UINT8 dev = 0;    
	QNF_SetAudMuteEnable(qma, QNR_MUTE_ENABLE);	
	QNF_SetData(0x0014,0x00);
	QNF_SetData(QNR_PCLK_CTRL_PCLK_CTRL, QNR_PCLK_CTRL_NORMAL);
	QNF_SetData(QNR_PCLK_CTRL_DOUT_CTRL, QNR_DOUT_CTRL_NORMAL);
	QNF_SetData(QNR_SYSTEM1_RX_REQ, QNR_RX_REQ_ENTER);       
	dev =  QNF_GetData(QNR_SYSTEM1_RX_MODE);    
	if(QNM_DEV_ATV == dev)
	{
		qma->dev = QNM_DEV_ATV;
		QMA_TuneToCH(qma,qma->curFreq);
	}
	else
	{
		qma->dev = QNM_DEV_FM;
		QMA_FMTuneToCH(qma,qma->fm->curFreq);
	}	
}

/*********************** video processing *************************************/

/******************************************************************************
    void QNF_InitConfigVideo(S_MA_DRIVER *qma)
 ******************************************************************************
 Description :
     This function configures the initializing video parameters for PAL, called only by QMA_Init().
     
 Parameters:                
     qma : the point of struct S_MA_DRIVER.
     
 Return value :
       none
 ******************************************************************************/
void QNF_InitConfigVideo(S_MA_DRIVER *qma)
{     
   QMA_WriteReg(0x0b, 0x20);       
   QMA_WriteReg(0x0c, 0x43);       
   QMA_WriteReg(0x0d, 0x2d);       
   QMA_WriteReg(0x0e, 0x28);       
   QMA_WriteReg(0x9c, 0x01);       
   QMA_WriteReg(0xa1, 0x01);
   QMA_WriteReg(0x2b, 0x0a);
   QMA_WriteReg(0x2d, 0x40);
   QNF_SetData(0x293f, 0x2c);	
   QNF_SetData(0x2a3f, 0x28);	       
   QNF_SetData(0xf940, 0x00);
}

/******************************************************************************
    void QNF_ConfigResolution(S_MA_DRIVER *qma, UINT8 size)
 ******************************************************************************
 Description : 
     This function sets the resolution for LCD display.
     mainly used in LCD (like mobile phone),  always use TF and odd field.
     
 Parameters:
    qma : the point of struct S_MA_DRIVER.
    
    ituMode : set video output mode
		          BT.601     0x00
			   BT.656     0x02
			   
    size : set the video output resolution
    
	  _______________________________________________________ 
	  |		 Para			      |Macro Define   | Note	 |
	  |_________________________|____________|_______________|
      |QNM_RESOLUTION_320_240   |   0 		  |QVGA		     |
	  |QNM_RESOLUTION_360_240   |   1		  |	             |
	  |QNM_RESOLUTION_400_240	|	2		  |QWVGA:800*480 |
	  |QNM_RESOLUTIOM_640_240	|	3		  | 		     |
	  |QNM_RESOLUTIOM_176_220	|	4		  |sub-QVGA      |
	  |QNM_RESOLUTIOM_160_128	|	5		  |              |
	  |QNM_RESOLUTIOM_176_144	|	6		  |QCIF 	     |
      |QNM_RESOLUTIOM_220_176	|	7		  | 			 |
	  |QNM_RESOLUTIOM_352_288	|	8		  |CIF			 |
	  |QNM_RESOLUTIOM_480_300	|	9		  |QQ-WUXGA 	 |
	  |QNM_RESOLUTIOM_640_300   |   10 		  |			     |
	  --------------------------------------------------------
		        
 Return value :
    none
 ******************************************************************************/
void QNF_ConfigResolution(S_MA_DRIVER *qma, UINT8 size)
{
    UINT16  rgn = qma->regionCode;
    UINT8  deci_flg;
    UINT8 tv_standard=(UINT8)(ATV_ChMap[rgn][0].tvStd & 0x03);
    
    if((size == QNM_RESOLUTIOM_640_240)||QNM_RESOLUTIOM_640_300 == size||QNM_RESOLUTIOM_480_300 == size)
    {
        deci_flg = QNR_DECI_DIS;
    }
    else
    {
        deci_flg = QNR_DECI_EN;
    }
    
    QNF_SetData(QNR_VID_CTRL1_DECI_EN, deci_flg);   	
    QNF_SetData(QNR_SCALER_CTRL3_SCALER_EN, QNR_SCALER_EN);
    QNF_SetData(QNR_SCALER_CTRL3_TF_BP, QNR_TF_EN);   
    QNF_SetData(QNR_SCALER_CTRL4_VSCALE_WEN, QNR_VSCALE_WEN_NO_FIXED);
    QNF_SetData(QNR_VID_ASYM_WEN, QNR_ASYM_WEN_INTERNAL);
    
    QNF_SetActiveStartPxlNum(qma, QNM_CFG_START_PIXEL);  
    QNF_SetActivePxlNum(qma, ATV_resolutionMap[size].pxlNum);
    QNF_SetActiveLineNum(qma, ATV_resolutionMap[size].lineNum);
    QNF_SetData(QNR_SCALER_CTRL1_XDT, ATV_resolutionMap[size].xScale);
    QNF_SetData(QNR_SCALER_CTRL2_YDT, ATV_resolutionMap[size].yScale);
    QNF_SetData(QNR_SCALER_CTRL3_HSCALE_RATIO, ATV_resolutionMap[size].hScaleRatio); 
    
    QNF_SetData(QNR_OUT_LINE_DLY_REG, 
    (UINT8)(((QNM_RESOLUTIOM_640_300 == size) && (QNR_TV_STD1_VID_STD_PALD <= tv_standard))? QNM_CFG_OUT_LINE_DLY_640_PS : ATV_resolutionMap[size].outLineDly));
    
    QNF_SetVsyncStartLineNum1(qma, QNM_CFG_VS_START1);
    QNF_SetVsyncStopLineNum1(qma, QNM_CFG_VS_STOP1);
    
    if (QNR_TV_STD1_VID_STD_PALD<=  tv_standard)  // 625 lines
    {
		qma->isPs = 0;
        QNF_SetData(QNR_SCALER_CTRL4_VSCALE_EN, QNR_VSCALE_EN_PS);
        QNF_SetActiveStartLineNum1(qma, QNM_CFG_START_LINE1_PS);
        QNF_SetActiveStartLineNum2(qma, QNM_CFG_START_LINE2_PS);
        QNF_SetVsyncStartLineNum2(qma, QNM_CFG_VS_START2_PS);
        QNF_SetVsyncStopLineNum2(qma, QNM_CFG_VS_STOP2_PS);
    } 
    else
    {
		qma->isPs = 1;
       QNF_SetData(QNR_SCALER_CTRL4_VSCALE_EN, QNR_VSCALE_EN_NPM);
       QNF_SetActiveStartLineNum1(qma, QNM_CFG_START_LINE1_NPM);
       QNF_SetActiveStartLineNum2(qma, QNM_CFG_START_LINE2_NPM);
       QNF_SetVsyncStartLineNum2(qma, QNM_CFG_VS_START2_NPM);
       QNF_SetVsyncStopLineNum2(qma, QNM_CFG_VS_STOP2_NPM);
    }
}

/************************************************************************
    UINT8 QNF_ConfigTFParam(S_MA_DRIVER *qma, QMA_TF_PARAM_LEVEL val)
*************************************************************************
Description : 
	3D noise 
	This functions sets the de-noise function, 
	when video quality is poor,  during the image process.
             
Parameters:
	qma: 
	the point of struct S_MA_DRIVER;  

	val: 
	QMA_TF_PARAM_LEVEL

Return value:
	TRUE
**********************************************************************/
UINT8 QNF_ConfigTFParam(S_MA_DRIVER *qma, QMA_TF_PARAM_LEVEL val)
{
    QMA_WriteReg(0x24, ATV_TFParam[val].reg0x24);
    QMA_WriteReg(0xad, ATV_TFParam[val].reg0xad);
    QMA_WriteReg(0xae, ATV_TFParam[val].reg0xae);
    QMA_WriteReg(0x25, ATV_TFParam[val].reg0x25);
    QMA_WriteReg(0x26, ATV_TFParam[val].reg0x26);
    QMA_WriteReg(0x27, ATV_TFParam[val].reg0x27);
    QMA_WriteReg(0x28, ATV_TFParam[val].reg0x28);
	
    return 1;
}

/************************************************************************
     void QNF_ConfigVideoOutputFormat(S_MA_DRIVER *qma, UINT8 ituMode)
*************************************************************************
Description : 
    This functions sets video output format,  for example,  Hsync
    polarity, video out enable,  YUV and its order and so on.
    but it is needed to match to the back-end application processor.
             
Parameters:
    qma :  the point of struct S_MA_DRIVER.
    
    ituMode : 
        set video output mode
	     QNR_VID_ITU_MODE_BT601  
            QNR_VID_ITU_MODE_BT656	    
**********************************************************************/
void QNF_ConfigVideoOutputFormat(S_MA_DRIVER *qma, UINT8 ituMode)
{	
    UINT8 flagA, flagB;
    
    flagA = (UINT8)(QNR_DOUT_ZERO|QNR_CLR_TYPE_YCbCr|\
    QNR_VID_SRC_ATV_DECODER|ituMode|QNR_VID_VID_EN);
    flagB = (UINT8)(QNR_FULL_SCALE|QNR_FS_SEL_30FPS|QNR_BIT_SWAP_MLSB);
    
    QNF_SetData(0x103f, flagA);
    QNF_SetData(0x11c1, flagB);
}

/******************************************************************************
    void QNF_SetActiveStartPxlNum(S_MA_DRIVER *qma, UINT8 pixelNum)
 ******************************************************************************
 Description : 
    This function sets the active video start pixel number in one line.
     
 Parameters:
    qma :  the point of struct S_MA_DRIVER.
    pixelNum : 
        pixel numbers(0x00-0xff).
        
	 the actual start pixel  =  pixelNum*4.
 ******************************************************************************/
void QNF_SetActiveStartPxlNum(S_MA_DRIVER *qma, UINT8 pixelNum)
{	
    QNF_SetData(QNR_AVID_CFG1_START_PIXEL, pixelNum);
}

/******************************************************************************
     void QNF_SetActivePxlNum(S_MA_DRIVER *qma, UINT16 pixelNum)
 ******************************************************************************
 Description : 
     This function sets the 10-bit active video pixel number in one line,  the default is 320 pixels.
     
 Parameters:
     qma :  the point of struct S_MA_DRIVER.
     
     pixelNum : 
         pixel numbers(0x00-0x3ff)

 Note : 
     that one point must be noticed is that the value of AVID_START_PIXEL and
     AVID_STOP_PIXEL must be an integer multiple of 4 in horizontal decimation mode
     and be an integer multiple of 2 in non-horizontal decimation mode.
 ******************************************************************************/
void QNF_SetActivePxlNum(S_MA_DRIVER *qma, UINT16 pixelNum)
{
    QNF_SetData(QNR_AVID_CFG5_PIXEL_NUM, (UINT8)(pixelNum & 0xFF));	
    QNF_SetData(QNR_AVID_CFG4_PIXEL_NUM, (UINT8)((pixelNum>>4)&0x30));
}

/******************************************************************************
     void QNF_SetActiveStartLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
 ******************************************************************************
 Description : 
     This function sets the 10-bit active video start line number 1, 
     which is the start of active video in the odd field.
     
 Parameters:
     qma :  the point of struct S_MA_DRIVER.
     lineNum1 : line numbers(0x00-0x3ff)

 ******************************************************************************/
void QNF_SetActiveStartLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
{
    QNF_SetData(QNR_AVID_CFG2_START_LINE1, (UINT8)(lineNum1 & 0xFF));	
    QNF_SetData(QNR_AVID_CFG4_START_LINE1, (UINT8)((lineNum1>>8)&0x03));
}

/******************************************************************************
     void QNF_SetActiveStartLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
 ******************************************************************************
 Description : 
     This function sets the 10-bit active video start line number 2
 Parameters :
     qma : the point of struct S_MA_DRIVER.
     lineNum2: line numbers
     
 Return value :
     none
 ******************************************************************************/
void QNF_SetActiveStartLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
{
    QNF_SetData(QNR_AVID_CFG3_START_LINE2, (UINT8)(lineNum2 & 0xFF));	
    QNF_SetData(QNR_AVID_CFG4_START_LINE2, (UINT8)((lineNum2>>6)&0x0c));
}

/******************************************************************************
      void QNF_SetActiveLineNum(S_MA_DRIVER *qma, UINT16 lineNum)
 ******************************************************************************
 Description : 
     This function sets the 10-bit active video line number, the default is 240 lines.
     
 Parameters:
     qma : the point of struct S_MA_DRIVER.
     lineNum : line numbers(0x00-0x1ff)
     
 Return value :
     none
 ******************************************************************************/
void QNF_SetActiveLineNum(S_MA_DRIVER *qma, UINT16 lineNum)
{
    QNF_SetData(QNR_AVID_CFG6_LINE_NUM, (UINT8)(lineNum & 0xFF));	
    QNF_SetData(QNR_AVID_CFG4_LINE_NUM, (UINT8)((lineNum>>1)&0x80));
}

/******************************************************************************
        void QNF_SetVsyncStartLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
 ******************************************************************************
 Description : 
     This function sets the 10-bit VSYNC pulse start line number 1.
     
 Parameters:
     qma : the point of struct S_MA_DRIVER.
     lineNum1: line numbers(0x00-0x3ff)
     
 Return value :
     none
  Note: 
      The VS_START and VS_STOP registers are used to configure the valid vsync
      period which used in BT601 standard,  and its configuration depends
      the multimedia processor. In BT656 standard,  these two parameter will
      be not be cared and can be set to be any value,  while had better set them to be 0.
 ******************************************************************************/
void QNF_SetVsyncStartLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
{
    QNF_SetData(QNR_VS_CFG1_START1, (UINT8)(lineNum1 & 0xFF));	
    QNF_SetData(QNR_VS_CFG5_START1, (UINT8)((lineNum1>>8)&0x03));
}

/******************************************************************************
     void QNF_SetVsyncStopLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
 ******************************************************************************
 Description : 
     This function sets the 10-bit VSYNC pulse stop line number 1

 Parameters:
     qma : the point of struct S_MA_DRIVER.

     lineNum1: line numbers(0x00-0x3ff)
     
 Return value :
     none
     
 Note :
     The VS_START and VS_STOP registers are used to configure the valid vsync
     period which used in BT601 standard,  and its configuration depends
     the multimedia processor. In BT656 standard,  these two parameter will
     be not be cared and can be set to be any value,  while had better set them to be 0.
 ******************************************************************************/
void QNF_SetVsyncStopLineNum1(S_MA_DRIVER *qma, UINT16 lineNum1)
{
    QNF_SetData(QNR_VS_CFG2_STOP1, (UINT8)(lineNum1 & 0xFF));	
    QNF_SetData(QNR_VS_CFG5_STOP1, (UINT8)((lineNum1>>6)&0x0c));
}

/******************************************************************************
     void QNF_SetVsyncStartLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
 ******************************************************************************
 Description : 
     This function sets the 10-bit VSYNC pulse start line number 2
     
 Parameters:
     qma : the point of struct S_MA_DRIVER.

     lineNum2 : line numbers(0x00-0x3ff)
     
 Return value :
       none
       
 Note :
     The VS_START and VS_STOP registers are used to configure the valid vsync
     period which used in BT601 standard,  and its configuration depends
     the multimedia processor. In BT656 standard,  these two parameter will
     be not be cared and can be set to be any value,  while had better set them to be 0.
 ******************************************************************************/
void QNF_SetVsyncStartLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
{
    QNF_SetData(QNR_VS_CFG3_START2, (UINT8)(lineNum2 & 0xFF));	
    QNF_SetData(QNR_VS_CFG5_START2, (UINT8)((lineNum2>>4)&0x30));
}

/******************************************************************************
    void QNF_SetVsyncStopLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
 ******************************************************************************
 Description : 
     This function sets the 10-bit VSYNC pulse stop line number 2.
     
 Parameters :
     qma : the point of struct S_MA_DRIVER.
     
     lineNum2 : 
         line numbers(0x00-0x3ff).
     
 Return value :
       none
       
 Note : 
    The VS_START and VS_STOP registers are used to configure the valid vsync
    period which used in BT601 standard,  and its configuration depends
    the multimedia processor. In BT656 standard,  these two parameter will
    be not be cared and can be set to be any value,  while had better set them to be 0.
 ******************************************************************************/
void QNF_SetVsyncStopLineNum2(S_MA_DRIVER *qma, UINT16 lineNum2)
{
    QNF_SetData(QNR_VS_CFG4_STOP2, (UINT8)(lineNum2 & 0xFF));	
    QNF_SetData(QNR_VS_CFG5_STOP2, (UINT8)((lineNum2>>2)&0xc0));
}

/******************************************************************************
             UINT8 QNF_CheckSyncDetectState()
 ******************************************************************************
 Description:This function checks sync detection state,  including Vsync and Hsync

 Parameters: 
         none
 Return value :
         QNR_SYNC_FSM_HSYNC_SEARCHING    0x00
         QNR_SYNC_FSM_HSYNC_CONFIRMING   0x02
         QNR_SYNC_FSM_VSYNC_SEARCHING    0x04
         QNR_SYNC_FSM_TRACKING           0x06
      
         QNR_VID_LOCK_NOT        0x00
         QNR_VID_LOCK_WEN        0x01
      
 ******************************************************************************/
UINT8 QNF_CheckSyncDetectState(void)
{
    UINT8 tmp;
    
    tmp = QNF_GetData(QNR_STATUS2_SYNC_FSM|QNR_STATUS2_VID_LOCK);    
    QMA_WriteReg(0x79, 0xB7);	
    QMA_WriteReg(0x88, 0x40);//0x02);
    
    return tmp;
}

/******************************************************************************
    void QNF_SetBlackWhiteMode(S_MA_DRIVER *qma, UINT8 val)
 ******************************************************************************
 Description:This function sets the black and white of video output.
 
 Parameters: 
     qma : the point of struct S_MA_DRIVER.
     
     val: 
	       QNR_CLR_KILL_NOT_FORCE  0x00
              QNR_CLR_KILL_FORCE          0x04
	     
 Return value :
      none      
 ******************************************************************************/
void QNF_SetBlackWhiteMode(S_MA_DRIVER *qma, UINT8 val)
{
      QNF_SetData(0x9804, val);
}

/*********************** audio processing *************************************/

/******************************************************************************
    void QNF_InitConfigAudio(S_MA_DRIVER *qma)
 ******************************************************************************
 Description : 
     This function configures the initializing audio parameters. it is called only by QNM_Init()
     
 Parameters:                
     qma : the point of struct S_MA_DRIVER.
     
 Return value :
    none
 ******************************************************************************/
void QNF_InitConfigAudio(S_MA_DRIVER *qma)
{      
    QMA_WriteReg(0xdc, 0x19);
    QNF_SetData(0x3770, 0x00);       
    QNF_SetData(0x3a70, 0x00);       	
    QNF_SetTVAudSelect(QNM_TV_AUD_STEREO);
}

/******************************************************************************
    void QNF_SetAudMuteEnable(S_MA_DRIVER *qma, UINT8 on)
******************************************************************************
Description : 
    This function sets the video output type on the QNF8610.
    The video output type controls certain timing algorithms of the video output.

Parameters :
    qma : the point of struct S_MA_DRIVER.

    on : 
        1 : QNR_MUTE_ENABLE
        0 : QNR_MUTE_DISABLE

Return value :
    none
******************************************************************************/
void QNF_SetAudMuteEnable(S_MA_DRIVER *qma, UINT8 on)
{
    if(on == QNR_MUTE_ENABLE)
	{
		QNF_SetData(0x4008, 0x08);
	}
    else
	{
		QNF_SetData(0x4008, 0x00);
	}
}

/******************************************************************************
     void QNF_SetDeemp(UINT8 on,  UINT8 emp_val)
 ******************************************************************************
 Description : 
     This function sets the tv audio de-emphasis.
     
 Parameters:
     on : 
         QNR_DEEMP_EN           
         QNR_DEEMP_BP 
     emp_val : 
         QNR_DEEMP_50US  
         QNR_DEEMP_75US         
		 
 Return value :
     none
 ******************************************************************************/
void QNF_SetDeemp(UINT8 on,  UINT8 emp_val)
{
	QNF_SetData(QNR_TV_STD2_DEEMP_BP, on);
	QNF_SetData(QNR_TV_STD2_DEEMP_TC, emp_val);
}

/******************************************************************************
     void QNF_SetTVAudSTD(UINT8 tvAudSTD)
 ******************************************************************************
 Description : 
     This function sets the tv audio standard.
     
 Parameters:
     tvAudSTD : 
         QNM_TV_AUD_STD_BTSC  0
         QNM_TV_AUD_STD_A2      1
         
 Return value :
       none
 ******************************************************************************/
void QNF_SetTVAudSTD( UINT8 tvAudSTD)
{
    QNF_SetData(QNR_TV_STD2_AUD_STD, tvAudSTD);
}

/******************************************************************************
              void QNF_SetTVAudSelect(UINT8 audSel)
 ******************************************************************************
 Description : 
     This function sets the tv audio output selection.
     
 Parameters:
     audSel:  
         #define QNM_TV_AUD_STEREO          0
         #define QNM_TV_AUD_FAP             1
         #define QNM_TV_AUD_SAP             2
         #define QNM_TV_AUD_FSAP            3
                 
 Return value :
       none
 ******************************************************************************/
void QNF_SetTVAudSelect(UINT8 audSel)
{
    if(QNM_TV_AUD_FAP<audSel)
    {
        QNF_SetData(QNR_TV_STD2_SAP_EN, QNR_SAP_EN);
        QNF_SetData(QNR_AUD_CTRL2_A2_CH2_OPT, QNR_A2_CH2_OPT_SUB242);
    }
    else
    {
        QNF_SetData(QNR_TV_STD2_SAP_EN, QNR_SAP_DIS);	
    }
    QNF_SetData(QNR_TV_STD2_AUD_SEL, (UINT8)(audSel<<4));
}

/******************************************************************************
            void QNF_SetAudioGain(S_MA_DRIVER *qma, UINT8 val)
 ******************************************************************************
 Description : 
    This function sets the audio gain on DAC
    
 Parameters:
    qma:   the point of struct S_MA_DRIVER
      
    val:   
             QNR_DAC_GAIN0             0x07     0db
             QNR_DAC_GAIN1             0x06    -6db
             QNR_DAC_GAIN2             0x05    -12db
             QNR_DAC_GAIN3             0x04    -18db			  
             QNR_DAC_GAIN4             0x03    -24db
             QNR_DAC_GAIN5             0x02    -30db
             QNR_DAC_GAIN6             0x01    -36db
             QNR_DAC_GAIN7             0x00    -42db
             
Return value :
    none
******************************************************************************/
void QNF_SetAudioGain(S_MA_DRIVER *qma, UINT8 val)
{
	QNF_SetData(QNR_TUNER_CTRL_DAC_GAIN, val);
}

/******************************************************************************
    void QNF_SetAudioVolume(S_MA_DRIVER *qma, UINT8 val)
 ******************************************************************************
 Description : 
     This function sets the audio digital gain.

 Parameters:
    qma:   the point of struct S_MA_DRIVER
    
    val : the range in Reg Value :    00  ~ 23 ~ 41                                                                                                    
	    the range in Audio Value:    -23dB~ 0dB~ 18dB
	         
 Return value :
    none
 ******************************************************************************/
void QNF_SetAudioVolume(S_MA_DRIVER *qma, UINT8 val)
{
    QNF_SetData(QNR_AUD_CTRL1_GAIN_DIG, val);    
}

/******************************************************************************
    void QNF_TVAutoDetection(S_MA_DRIVER *qma)
 ******************************************************************************
 Description : 
     This function sets the auto detecting PAL/SECAM tv system.
     
 Parameters:
     qma : the point of struct S_MA_DRIVER
      	         
 Return value:
       none
 ******************************************************************************/
void QNF_TVAutoDetection(S_MA_DRIVER *qma)
{
	UINT8 i;
    UINT8 failed = 1, asym_high = 0;
    UINT8 region =(UINT8) qma->regionCode;
    UINT8 tv_standard;
    UINT8 reg_value,reg02_bit01;
    UINT8 Rega3_443=0, Rega3_425=0;
	
    if(region == QNM_REGION_NORTH_KOREA || region == QNM_REGION_GREECE)
    {
	//	TRACE("PAL/SECAM auto detection ...\n");
        QMA_WriteReg(0x76, 0x55);
        QMA_WriteReg(0x77, 0x55);
        QMA_WriteReg(0x9b, 0x05);        
        /// check PAL first
		reg02_bit01=(UINT8)(QMA_ReadReg(QNR_TV_STD1)&0xfc);
        tv_standard = QNR_TV_STD1_VID_STD_PALD;
        reg_value = (UINT8)(reg02_bit01 | tv_standard);
        QNF_SetATVStd(qma,reg_value);

		QMA_WriteReg(0x71, 0x2e);
        QMA_WriteReg(0x71, 0x3e);

		QMA_Delay(200);
        for(i=0; i<40; i++)
        {
            QMA_Delay(40);
            if(QNF_GetData(QNR_STATUS2_FSC_LOCK)) 
            {
                failed = 0;
                break;
            }
		}

	//	reg_value = QMA_ReadReg(0x08);
	//	TRACE("Check PAL: 0x%0x, reg08=0x%0x, failed=%0d\n", tv_standard, reg_value, failed);

        // if PAL color locked failed, then check audio ASYM
        if(failed)
        {
            for(i=0; i<3; i++)
            {
                QMA_Delay(20);
                reg_value = QMA_ReadReg(0xe5);
                if (reg_value >= 20) 
				{
		            asym_high = 1;
		            break;
				}
			}
		   // TRACE("Check audio: ASYM=%0d, failed=%0d\n", reg_value, failed);
		}

		// if PAL color lock failed, but ASYM high enough, it's highly possible a SECAM channel,
        // else check the clk lock value to improve accuracy
        if(failed==1 && asym_high==0)
        {
            for(i=0; i<20; i++)
            {
                QMA_Delay(40);
                reg_value = QMA_ReadReg(0xa3);
                Rega3_443 = (UINT8)(Rega3_443 + reg_value);
		      //  TRACE("Check 4.43 lock value: rega3=%0d, rega3_acc=%0d\n", reg_value, Rega3_443);
			}

		    QMA_WriteReg(0x9e, 0x7f);
            QMA_WriteReg(0x71, 0x2e);
            QMA_WriteReg(0x71, 0x3e);
            QMA_Delay(1000);
            for(i=0; i<20; i++)
            {
                QMA_Delay(40);
                reg_value = QMA_ReadReg(0xa3);
                Rega3_425 = (UINT8)(Rega3_425 + reg_value);
		      //  TRACE("Check 4.25 lock value: rega3=%0d, rega3_acc=%0d\n", reg_value, Rega3_425);
		    } 

			if (Rega3_443>=Rega3_425) 
            {
                failed = 0;
		    } 
		    QMA_WriteReg(0x9e, 0x00);
		   // TRACE("Check lock : F443=0x%0x, F425=0x%0x, failed=%0d\n", Rega3_443, Rega3_425, failed);
 		}

		//reg_value = QMA_ReadReg(0x08);
		//TRACE("VID STD: 0x%0x, reg08=0x%0x, failed=%0d\n", tv_standard, reg_value, failed);
        
        // if PAL failed, then it's SECAM
        if(failed)
        {
            tv_standard = QNR_TV_STD1_VID_STD_SECAM;
            QNF_SetATVStd(qma,(UINT8)(reg02_bit01 | tv_standard));
        }
        //reg_value = QMA_ReadReg(0x08);
		//TRACE("VID STD: 0x%0x, reg08=0x%0x, failed=%0d\n", tv_standard, reg_value, failed);
		// back to default		
		QMA_WriteReg(0x76, 0x58);
		QMA_WriteReg(0x77, 0x5b);
		QMA_WriteReg(0x9b, 0x10);        
    }	
}

/******************************************************************************
    void QNF_TVCkillSelection(S_MA_DRIVER *qma,UINT8 option)
 ******************************************************************************
 Description : 
     This function sets the color kill condition customer likes.
	 since different projects will have some different for sensitivity,
	 end custom may set different level according to the actual condition.
	 the default is QNM_CKILL_SETTING_2.
	 when sensitivity is more higher, the level value you need to set is more lower.
     
 Parameters:
     qma : the point of struct S_MA_DRIVER
	 option:
	       QNM_CKILL_NONE                   0
	       QNM_CKILL_SETTING_1              1
	       QNM_CKILL_SETTING_2              2
	       QNM_CKILL_SETTING_3              3
	       QNM_CKILL_SETTING_4              4
           QNM_CKILL_SETTING_5              5
      	         
 Return value:
       none
 ******************************************************************************/
void QNF_TVCkillSelection(S_MA_DRIVER *qma,UINT8 option)
{
    if(!option)
    {
        QNF_SetData(0x9808,0x00);
    }
    else
    {
        QNF_SetData(0x9808,0x08);
        switch (option)
        {		    
            case QNM_CKILL_SETTING_1:
                QMA_WriteReg(0xa0,0x20);
                QMA_WriteReg(0xa1,0x18);
                break;
            case QNM_CKILL_SETTING_2:
                QMA_WriteReg(0xa0,0x28);
                QMA_WriteReg(0xa1,0x20);
                break;
            case QNM_CKILL_SETTING_3:
                QMA_WriteReg(0xa0,0x30);
                QMA_WriteReg(0xa1,0x28);
                break;
            case QNM_CKILL_SETTING_4:
                QMA_WriteReg(0xa0,0x38);
                QMA_WriteReg(0xa1,0x30);
                break;
            case QNM_CKILL_SETTING_5:
                QMA_WriteReg(0xa0,0x40);
                QMA_WriteReg(0xa1,0x38);
                break;
	     case QNM_CKILL_SETTING_6:
		 QMA_WriteReg(0xa0,0x05);
		 QMA_WriteReg(0xa1,0x02);
		break;
            default:
                break;
        }
    }
}
