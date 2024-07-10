/************************************************************************
 *										
 *  This software is copyrighted by and is the property of World Peace Group(China).
 *  All rights are reserved by World Peace Group(China)
 *  This software may only be used in accordance with the	
 *  corresponding license agreement. Any unauthorized use, duplication,		
 *  distribution, or disclosure of this software is expressly forbidden.
 *																
 *  This Copyright notice MUST not be removed or modified without prior		
 *  written consent of World Peace Group(China)							
 *																			
 *  World Peace Group reserves the right to modify this			
 *  software without notice.												
 *																		
 *  Author: 			    													
 *	Create Date:			17:42 pm, Dec 16th,2005
 *	Last Modified Date:		Oct 15th,2012
 *
 *
 *
 *	The BK1080 Driver totally contain 2 files:fm_bk1080.c and fm_bk1080.h
 *	fm_bk1080.c mainly inlcude the function definition 
 *	
 *	fm_bk1080.h is the macro definition the programmer may use and the API's declaration
 *	
 ***************************************************************************/
#include "os_api.h"
#include "fm_drv.h"
#include "fm_cfg.h"
#include "I2C_drvapi.h"
#include "gpio_prod_cfg.h"

#define BK1080_I2C_ADDR   		0xE0
#define BK1080_I2C_ACK			0x01

LOCAL uint16 g_fm_autoscan_freq_tab[20];
LOCAL BOOLEAN s_fm_init_state = 0;
LOCAL FM_REGION_E fm_region = FM_REGION_US;

LOCAL int32 BK1080_IIC_Handle=-1;

LOCAL uint16 g_freq_deviation_array[1081-875];  //max length=channel number

LOCAL I2C_DEV BK1080_IIC_cfg = 
{
	2,            	    	//id, will be reinit during init()/ open()
	100*1000,   			//freq
	0,						//bus
	BK1080_I2C_ADDR,        //slave_addr
	0,						//reg_addr_num
	1,						//check_ack
	1						// no_stop
};

LOCAL uint16 BK1080_Digital_Reg[] = 
{
    0x0008,	//REG0
    0x1080,	//REG1
    0x0201,	//REG2
    0x0000,	//REG3
    0x40C0,	//REG4
    0x0A1F,	//REG5
    0x002E,	//REG6
    0x02FF,	//REG7
    0x5B11,	//REG8
    0x0000,	//REG9
    0x411E,	//REG10
    0x0000,	//REG11
    0xCE00,	//REG12
    0x0000,	//REG13
    0x0000,	//REG14
    0x1000,	//REG15
    0x0010,	//REG16
    0x0000,	//REG17
    0x13FF,	//REG18
    0x9852,	//REG19
    0x0000,	//REG20
    0x0000,	//REG21
    0x0008,	//REG22
    0x0000,	//REG23
    0x51E1,	//REG24
    0x28DC,	//REG25
    0x2645,	//REG26
    0x00E4,	//REG27
    0x1CD8,	//REG28
    0x3A50,	//REG29
    0xEAF0,	//REG30
    0x3000,	//REG31
    0x0000,	//REG32
    0x0000,	//REG33
};

uint8 i2c_wr_BK1080(uint8 regaddr, uint8 datalen, uint8 *data);
uint8 i2c_rd_BK1080(uint8 regaddr, uint8 datalen, uint8 *data);
#define I2C_WriteCmdArr_BK1080(a,b,c,d,e)  i2c_wr_BK1080(b,d,c)
#define I2C_ReadCmdArr_BK1080(a,b,c,d,e)  i2c_rd_BK1080(b,d,c)

LOCAL uint32 BK1080_SetVol(FM_VOL_LEVEL_E  value);

LOCAL int32 BK1080_I2C_Init(void)
{
	BK1080_IIC_cfg.id=FM_GetCtrlPort();
	if(BK1080_IIC_Handle == -1) 	 	
	{
		BK1080_IIC_Handle = I2C_HAL_Open(&BK1080_IIC_cfg);
		if(BK1080_IIC_Handle==-1)
			SCI_TRACE_LOW("BK1080_I2C_Init==> open IIC failed!");
		return -1;
	}
	return 0;
}

LOCAL void BK1080_IicHandlerRelease(void) 
{
	if(BK1080_IIC_Handle != -1)
	{
		I2C_HAL_Close(BK1080_IIC_Handle);
		BK1080_IIC_Handle = -1;
	}
    SCI_TRACE_LOW("BK1080_IicHandlerRelease");
}

LOCAL uint32 BK1080_I2C_Write(uint8* addr, uint8* SetValue, uint32 len)
{
	uint8 writedata[70];
	uint8 i;
	if(BK1080_IIC_Handle == -1)
	{
		BK1080_I2C_Init();
	}

	writedata[0]=(*addr);
	for (i=0;i<len;i++)
	{
		writedata[i+1]=SetValue[i];
	}
	return I2C_HAL_Write(BK1080_IIC_Handle, addr, writedata, len+1);
}

LOCAL uint32  BK1080_I2C_Read(uint8* addr,uint8 *ret_value,uint32 len)
{
	uint8 writedata[3];
	if(BK1080_IIC_Handle == -1)
	{
		BK1080_I2C_Init();
	}
	writedata[0]=0x7E;
	writedata[1]=0x00;
	writedata[2]=(*addr);
	I2C_HAL_Write(BK1080_IIC_Handle, addr, writedata, 3);
	return I2C_HAL_Read(BK1080_IIC_Handle, addr,ret_value, len);  
}

uint8 i2c_wr_BK1080(uint8 regaddr, uint8 datalen, uint8 *data)
{
    BK1080_I2C_Write(&regaddr,data,datalen);
    return 1;
}
 
uint8 i2c_rd_BK1080(uint8 regaddr, uint8 datalen, uint8 *data)
{
    BK1080_I2C_Read(&regaddr,data,datalen);
    return 1;
}


/*****************************************************************************/
//  Description:  This function control fm mute;
//                  bValue = SCI_TRUE, set mute
//                  bValue = SCI_FALSE, Exit mute
//  Author:        youwj
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN FM_BK1080_SetMute(BOOLEAN bValue)
{
	uint8 TmpData8[2];
	uint8 eI2CReturn;

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 2, TmpData8, 2, BK1080_I2C_ACK);
	if(bValue)
		TmpData8[0]&=0xbf;	//dismute
	else
		TmpData8[0]|=0x40;	//mute

	eI2CReturn = I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 2, TmpData8, 2, BK1080_I2C_ACK);
	
    if(eI2CReturn)
    {
        SCI_TRACE_LOW("BK108_FmSetMute:Mute mode (%d) I2C write error (%d)",bValue,eI2CReturn);	
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description:  This function control fm enter sleep mode;
//                  bValue = SCI_TRUE, Enter sleep
//                  bValue = SCI_FALSE, Exit sleep
//  Author:        youwj
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN FmEnterSleep(BOOLEAN bValue) 
{
	uint8 TmpData8[2];
	uint8 eI2CReturn;

    // disable another slave address: 0x80
	TmpData8[0] = 0x00;
	TmpData8[1] = 0x01;
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 0x7D, TmpData8, 2, BK1080_I2C_ACK); //disable 0x80 I2C address

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 2, TmpData8, 2, BK1080_I2C_ACK);

	TmpData8[1]=TmpData8[1]|0x41;
	
	eI2CReturn = I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 2, TmpData8, 2, BK1080_I2C_ACK);
	
 	if(eI2CReturn)
	{
		SCI_TRACE_LOW("BK1080_FmEnterSleep:Sleep mode (%d) I2C write error (%d)",bValue,eI2CReturn);	
		return FALSE;
	}
    else
    {
        return TRUE;
    }
}

uint16 FmFreqToChan(uint16 frequency) 
{
	uint16 channelSpacing = 1;
	uint16 bottomOfBand = 0;
	uint16 channel = 0;
	uint8 readData8[2];

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 5, readData8, 2, BK1080_I2C_ACK);

	if (( readData8[1] & 0xc0) == 0x00)
		bottomOfBand = 875;
	else if (( readData8[1] & 0xc0) == 0x40)
		bottomOfBand = 760;
	else if (( readData8[1] & 0xc0) == 0x80)
		bottomOfBand = 760;

	if (( readData8[1] & 0x30) == 0x00)
		channelSpacing = 2;
	else if (( readData8[1] & 0x30) == 0x10)
		channelSpacing = 1;

    SCI_TRACE_LOW("BK1080_FmFreqToChan::frequency = %d, bottomOfBand = %d, channelSpacing = %d\r\n", frequency,
    bottomOfBand, channelSpacing);

	channel = (frequency - bottomOfBand) / channelSpacing;
	return (channel);
}

uint16 FmChanToFreq(uint16 channel)
{
	uint16 channelSpacing = 1;
	uint16 bottomOfBand = 0;
	uint16 frequency = 0;
	uint8 readData8[2];

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 5, readData8, 2, BK1080_I2C_ACK);

	if (( readData8[1] & 0xc0) == 0x00)
		bottomOfBand = 875;
	else if (( readData8[1] & 0xc0) == 0x40)
		bottomOfBand = 760;
	else if (( readData8[1] & 0xc0) == 0x80)
		bottomOfBand = 760;

	if (( readData8[1] & 0x30) == 0x00)
		channelSpacing = 2;
	else if (( readData8[1] & 0x30) == 0x10)
		channelSpacing = 1;

	frequency = (bottomOfBand + channelSpacing * channel);
	return (frequency);
}

PUBLIC uint16 FM_BK1080_GetRssi(void)
{
	uint8 readData8[2];
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 10, readData8, 2, BK1080_I2C_ACK);
	return readData8[1];
}


LOCAL uint32 BK1080_SetFreq(uint16 freq)
{
	uint8 writeData8[2];
	uint16 channel;
	SCI_TRACE_LOW("BK1080_SetFreq: %d",freq);
	channel=FmFreqToChan(freq);

	writeData8[0]=(channel>>8)&0xff;
	writeData8[1]=channel&0xff;
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 3, writeData8, 2, BK1080_I2C_ACK);
	writeData8[0] |= 0x80;//start tune
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 3, writeData8, 2, BK1080_I2C_ACK);
	return 1;

    
}


uint8 BK1080_ValidStop(int16 freq)  /* 自动搜索时，作为判定条件，再从中选择信号最强的9个台*/
{
	static int16 last_tuned_freq=0;
	uint16 cur_freq_deviation;
	uint8 TmpData8[10];
	
	BK1080_SetFreq(freq);

	SCI_Sleep(60);//kal_sleep_task(15);//delay 10ms

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 7, TmpData8, 2, BK1080_I2C_ACK); 

	cur_freq_deviation=TmpData8[0];
	cur_freq_deviation<<=8;
	cur_freq_deviation|=TmpData8[1];
	cur_freq_deviation=cur_freq_deviation>>4;

	g_freq_deviation_array[freq-875]=cur_freq_deviation;//save deviation

	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 7, TmpData8, 8, BK1080_I2C_ACK); 
	
	if(TmpData8[6]&0x10)    //check AFCRL bit12
	{
		last_tuned_freq=freq;//save last tuned freqency
		return 0;//false
	}

	if(TmpData8[7]<10) //RSSI<10
	{
		last_tuned_freq=freq;//save last tuned freqency
		return 0;//false
	}

	if( (TmpData8[1]&0xf) <2) //SNR<2
	{
		last_tuned_freq=freq;//save last tuned freqency
		return 0;//false
	}

	if( (cur_freq_deviation>=250)&&(cur_freq_deviation<=(0xfff-250)))
	{
		last_tuned_freq=freq;//save last tuned freqency
		return 0;		
	}
	
	if( (freq>=876)&&( (freq-last_tuned_freq)==1) )
	{
		if(g_freq_deviation_array[freq-875-1]&0x800)
		{
			last_tuned_freq=freq;//save last tuned freqency
			return 0;		
		}

		if(g_freq_deviation_array[freq-875-1]<100)
		{
			last_tuned_freq=freq;//save last tuned freqency
			return 0;
		}
	}
	
	if( (freq>=875)&&( (last_tuned_freq-freq)==1) )
	{
		if( (g_freq_deviation_array[freq-875+1]&0x800)==0)
		{
			last_tuned_freq=freq;//save last tuned freqency
			return 0;
		}

		if(g_freq_deviation_array[freq-875+1]>(0xfff-100) )
		{
			last_tuned_freq=freq;//save last tuned freqency
			return 0;
		}

	}
	if((freq==960)||(freq==1080))
	{
		last_tuned_freq=freq;//save last tuned freqency
		return 0;
	}
	
	last_tuned_freq=freq;//save last tuned freqency

	return 1; //OK      
}

LOCAL uint8 FmSeek_BK1080(uint16 start_freq,uint8 seekDirection)
{
	uint16 icount=0;
	uint8 result;
	BOOLEAN flag = SCI_FALSE; 
	
	do 
	{
		if (seekDirection)
		{
			start_freq++;
			if(start_freq>1080)
				start_freq=875;
		}
		else
		{
			start_freq--;
			if(start_freq<875)
				start_freq=1080;
		}
		
		result=BK1080_ValidStop(start_freq);

		if (result==1)
		{
			flag = SCI_TRUE;//break;
		}
		else
		{
			icount++;
			if (icount>205)
				flag = SCI_TRUE;//break;
		}
		
	}while(flag == SCI_FALSE);
}



LOCAL uint16 FmGetFreq_BK1080(void)
{
	uint8 readData8[2];	
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 11, readData8, 2, BK1080_I2C_ACK); 	
	return ( FmChanToFreq( readData8[1]|((readData8[0]&0x03) <<8 )) );
}


/*****************************************************************************/
//  FUNCTION:     BK1080_Initial
//  Description:  This function will init the BK1080 FM chip
//  return:     
//  Author:         
//  date:  
//	Note:         FM_SUCCESS: init success, FM_ERROR: init failed  
/*****************************************************************************/
LOCAL uint32 BK1080_Initial(void)
{
	uint16 i;
	uint8 writeData8[68],xbTemp;	
	uint16* preg;

	SCI_TRACE_LOW("***BK1080_Initial!!!");	
	
	s_fm_init_state = TRUE;
	
	BK1080_I2C_Init();

   	SCI_Sleep(10);
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 1, writeData8, 2, BK1080_I2C_ACK);	//check device ID

	SCI_TRACE_LOW("***bk1080_init_id= %2x%2x",writeData8[0],writeData8[1]);

       if (writeData8[0] != 0x10 || writeData8[1] != 0x80)
       {
           SCI_TRACE_LOW("GET DEVICE ID ERROR");
           return FM_ERROR;
       }

    // disable another slave address: 0x80
	writeData8[0] = 0x00;
	writeData8[1] = 0x01;
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 0x7D, writeData8, 2, BK1080_I2C_ACK); //disable 0x80 I2C address
	for(xbTemp = 0; xbTemp < 34; xbTemp++)//write reg2-reg31 ;reg0->device_id,reg1->chip_id only read ...
	{
		writeData8[xbTemp*2] = (BK1080_Digital_Reg[xbTemp] >> 8)&0xff;
		writeData8[xbTemp*2+1] = (BK1080_Digital_Reg[xbTemp])&0xff;
	}

	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 0, writeData8, 68, BK1080_I2C_ACK);

	SCI_Sleep(300);	//delay 250ms

	writeData8[25*2+1] = ((BK1080_Digital_Reg[25])&0x7f);
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 25, &(writeData8[25*2]), 2, BK1080_I2C_ACK);

	writeData8[25*2+1] = (BK1080_Digital_Reg[25])&0xff;
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 25, &(writeData8[25*2]), 2, BK1080_I2C_ACK);

	SCI_Sleep(80);	//delay 60ms
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 0, writeData8, 68, BK1080_I2C_ACK);
	
	BK1080_SetVol(FM_VOL_LEVEL_0);
	
	preg = (uint16*)writeData8;	
	for(i = 0; i < 34; i++)
	{
        SCI_TRACE_LOW("BK1080 REG[%d] = 0x%04x\r\n", i, *preg);
	    preg++;
	}
		
	return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     BK1080_Open
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
void BK1080_Open(void)
{
    SCI_TRACE_LOW("BK1080_Open!!!");
    if(!s_fm_init_state)
    {
        BK1080_Initial();
    }      
}

/*****************************************************************************/
//  FUNCTION:     BK1080_Close
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
void BK1080_Close(void)
{
    FmEnterSleep(SCI_TRUE);
    BK1080_IicHandlerRelease();    
	SCI_TRACE_LOW("BK1080_close!!!");
}

/*****************************************************************************/
//  Description:  This function control fm volume;
//                  value: 0~15
//  Author:        youwj
//  Note:           
/*****************************************************************************/
LOCAL uint32 BK1080_SetVol(FM_VOL_LEVEL_E  value)
{
	uint8 writeData8[8];
		
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 5, writeData8, 2, BK1080_I2C_ACK);
    value *= 2; /*mmi set value is 0~7 so multiply it with 2, mikeli20090327*/
    if(value > 14)
        value = 15;

	writeData8[1] &= 0xf0;
	writeData8[1] |= value&0x0f;
	
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 5, &writeData8[0], 2, BK1080_I2C_ACK);

    if(value == 0)
    FM_BK1080_SetMute(FALSE);
    else
    FM_BK1080_SetMute(TRUE);

	return TRUE;
}


/*****************************************************************************/
//  FUNCTION:     BK1080_ManualSeek
//  Description:  This function will tune to certain frequency
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 BK1080_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
	uint8 readData8[4];
	uint16 fm_strength=0;
	
    SCI_TRACE_LOW("BK1080_ManualSeek freq: %d",freq);
    
    BK1080_SetFreq(freq);
	SCI_Sleep(50);
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 10, readData8, 4, BK1080_I2C_ACK);
	
	if(single_channel_info != PNULL)
       {
		single_channel_info->freq =  FmChanToFreq( readData8[3]|((readData8[2]&0x03) <<8 ));

		fm_strength = readData8[1];
		
		if(fm_strength <= 0x60)
			single_channel_info->signal_strength = FM_SCANLEVEL_LOW;
		else if(fm_strength <= 0x80)
			single_channel_info->signal_strength = FM_SCANLEVEL_MID;
		else
			single_channel_info->signal_strength = FM_SCANLEVEL_HIGH;
		
		single_channel_info->is_stereo = (readData8[0] & 0x01);
	}
	
    BK1080_SetVol(FM_VOL_LEVEL_6);
	return(FM_SUCCESS);
}


/*****************************************************************************/
//  FUNCTION:     BK1080_AutoSeek
//  Description:  This function will seek from current channel automaticly,if want to seek from certain channel,  
//  use BK1080_AutoSeek to certain channel ahead
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
 LOCAL  uint32 BK1080_AutoSeek( uint16 freq, 
                                         FM_SCAN_DIR_E seek_dir,
                                         	FM_SCAN_MODE_E mode, 
                                         		FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{

    uint8 result = 0;
    uint16 rssi = 0;
	
	if(seek_dir == FM_SCAN_UP)
		result = FmSeek_BK1080(freq,1);
	else
		result = FmSeek_BK1080(freq,0);	
		
	g_fm_autoscan_freq_tab[0] = FmGetFreq_BK1080();	
	rssi = FM_BK1080_GetRssi();
	
	all_channel_info->freq_count = 1;
	all_channel_info->auto_freq_tab_ptr = &g_fm_autoscan_freq_tab[0];
	
    SCI_TRACE_LOW("BK1080_AutoSeek freq: %d, rssi: %d, result: %d, freq: %d",freq,rssi,result,g_fm_autoscan_freq_tab[0]);
	
	if(result)   	
	    return FM_SUCCESS;
    else
	    return FM_ERROR;

}

/*****************************************************************************/
//  FUNCTION:     BK1080_PlayStart
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 BK1080_PlayStart(uint16 freq)
{
    if(!s_fm_init_state)
    {
        BK1080_Initial();
    }      
	return FM_SUCCESS;  
}

/*****************************************************************************/
//  FUNCTION:     BK1080_PlayStop
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 BK1080_PlayStop()
{
    BK1080_SetVol(FM_VOL_LEVEL_0);   
	return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     BK1080_SetScanLevel
//  Description:  This function will config scane rssi level
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32 BK1080_SetScanLevel(FM_SCANLEVEL_E level)
{
	uint8 writeData8[8];
		
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 5, writeData8, 2, BK1080_I2C_ACK);

	//writeData8[0] = writeData8[0] & 0x0f;
	
	switch( level )
	{
		case FM_SCANLEVEL_LOW:
			writeData8[0] = 0x6;
			break;
		case FM_SCANLEVEL_MID:
			writeData8[0] = 0x8;
			break;
		case FM_SCANLEVEL_HIGH:
			writeData8[0] = 0xA;
			break;
		default:
			writeData8[0] = 0x8;
			break;
	}
	
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 5, &writeData8[0], 2, BK1080_I2C_ACK);
	return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:     BK1080_SetFMRegion
//  Description:  This function will select band region.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32  BK1080_SetFMRegion(FM_REGION_E  region)
{
	uint8 writeData8[4];
		
	I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 5, writeData8, 2, BK1080_I2C_ACK);
	writeData8[1] = writeData8[1] & 0x3f;

	switch(region)
	{
		case FM_REGION_US://87-108
			writeData8[1] = writeData8[1] | 0x00;
			fm_region = FM_REGION_US;
			break;

		case FM_REGION_JAPAN://76-91
			writeData8[1] = writeData8[1] | 0x40;
			fm_region = FM_REGION_JAPAN;
			break;

		default:
			break;
	}
	
	I2C_WriteCmdArr_BK1080(BK1080_I2C_ADDR, 5, &writeData8[0], 2, BK1080_I2C_ACK);
	return SCI_TRUE;
}  

//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     FM_BK1080_Ioctl
//  Description:  This function will control fm command.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32 BK1080_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}

PUBLIC FM_OPERATION_T g_BK1080_ops=
{
	BK1080_Initial,   
	BK1080_Open,     
	BK1080_Close,   

	BK1080_PlayStart,
	BK1080_PlayStop,

	BK1080_ManualSeek,

	BK1080_AutoSeek,

	BK1080_SetScanLevel,
	BK1080_SetVol,
	BK1080_SetFMRegion,
	BK1080_FM_Ioctl
};

PUBLIC BOOLEAN FM_IDENTIFY_BK1080(void)
{
    uint16 i;
    uint8 writeData8[68],xbTemp;	
    uint16* preg;
 
    BK1080_I2C_Init();

    SCI_Sleep(10);
    I2C_ReadCmdArr_BK1080(BK1080_I2C_ADDR, 1, writeData8, 2, BK1080_I2C_ACK);	//check device ID

    SCI_TRACE_LOW("FM_IDENTIFY_BK1080 writeData8 %2x%2x", writeData8[0], writeData8[1]);
    if (writeData8[0] == 0x10 && writeData8[1] == 0x80)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

