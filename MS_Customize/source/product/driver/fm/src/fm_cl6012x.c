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
 *  	Author: 				Snow Hsu(TME of WPG)														
 *	Create Date:			17:42 pm, Dec 16th,2005
 *	Last Modified Date:		Dec 16th,2005
 *
 *
 *
 *	The CL6012X Driver totally contain 2 files:CL6012X.c and CL6012X.h
 *	CL6012X.c mainly inlcude the function definition 
 *	
 *	CL6012X.h is the macro definition the programmer may use and the API's declaration
 *	
 ***************************************************************************/

#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "fm_drv.h"
#include "fm_cfg.h"
#include "fm_cl6012x.h"
 
#ifdef WIN32
#define __func__
#endif
// 20101104: Will.jiang_cr211861 : get i2c port id from fm_cfg.c 
#define FM_I2C_PORT                     FM_GetCtrlPort()
#define CL6012X_I2C_WRITE_ADDR  		0x20
#define CL6012X_I2C_READ_ADDR   		0x21

//IIC CFG INFO
LOCAL I2C_DEV CL6012X_IIC_cfg = 
{
	1,            	    	//id, will be reinit during init()/ open()
	100*1000,   			//freq
	0,						//bus
	CL6012X_I2C_WRITE_ADDR, //slave_addr
	0,						//reg_addr_num
	1,						//check_ack
	0						// no_stop
};

LOCAL int32 CL6012X_IIC_Handler = -1;

#define CL6012X_SEEK_INTERVAL           50
#define CL6012X_SEEK_MAX_CNT            10 /*max 10s*/

#define CL6012X_POWER_ON_DELAY          800 // 800 ms
// 20101217: Will.Jiang: 
// form datasheet: CL6018_customer__V1.5_EN_20080624_
// DEV_ID_CL6012X == 0xa7, CL6018=oxa5

#define DEV_ID_CL6012X		0xa7
#define DEV_ID_CL6012		0xa5
#define CL6012X_BITSET(_data, _bit_mask, _bit_set) \
            do{\
            	 _data &= ~(_bit_mask); \
           		 _data |=  (_bit_set);}while(0)

LOCAL const uint8 CL6012_reg_init[12]  = {0xe4,0x81,0x3d,0x5e,0x1f,0x48,0xb0,0xe9/*0xf1*/,0x0b,0xaa,0x90,0x06};
// 20101220: will.jiang_cr215111 : use the default(power-reset) value from datasheet
LOCAL const uint8 CL6012X_reg_init[24]  = {0xe4,0x81,0x01,0x5e,0x1f,0x40,0x38,0x5a,0xe8,0xac,0xb0,0xf1,\
									   0x8b,0xaa,0xc6,0x04,0x6d,0x25,0xff,0xfc,0x12,0x0f,0x45,0x1d};

LOCAL uint8  CL6012X_Write_buffer[24] =  { 0 };
LOCAL uint8  CL6012X_Read_buffer[18]  =  { 0 };
LOCAL uint16 s_frequency = 0;  // unit: 100 KHz
LOCAL uint8   s_cur_vol = FM_VOL_LEVEL_9;
LOCAL BOOLEAN s_is_busy_seek = SCI_FALSE;

LOCAL uint32	write_register_count=0;

#define  CL6012X_STATUS_RSSI_HIGH	CL6012X_Read_buffer[0]
#define  CL6012X_STATUS_RSSI_LOW	CL6012X_Read_buffer[1]
#define  CL6012X_READ_CHANNEL_HIGH	CL6012X_Read_buffer[2]
#define  CL6012X_READ_CHANNEL_LOW	CL6012X_Read_buffer[3]
#define  CL6012X_QOS_DEVICEID_HIGH	CL6012X_Read_buffer[4]
#define  CL6012X_QOS_DEVICEID_LOW	CL6012X_Read_buffer[5]
#define  CL6012X_FUNCTION_HIGH   	CL6012X_Write_buffer[0]
#define  CL6012X_FUNCTION_LOW   		CL6012X_Write_buffer[1]
#define  CL6012X_CHAN_VOL_HIGH  		CL6012X_Write_buffer[2]
#define  CL6012X_CHAN_VOL_LOW   		CL6012X_Write_buffer[3]
#define  CL6012X_SYSCONFIG_HIGH 	CL6012X_Write_buffer[4]
#define  CL6012X_SYSCONFIG_LOW  	CL6012X_Write_buffer[5]
// 20101220: will.jiang_cr215111 : update according to datasheet
#define  CL6012X_SEEK_HIGH  			CL6012X_Write_buffer[10]
// #define  CL6012X_SEEK_LOW   			CL6012X_Write_buffer[7]
#if 0
// folowing regs are reserved in cl6012x
#define  CL6012X_MUTE_HIGH 			CL6012X_Write_buffer[8]
#define  CL6012X_MUTE_LOW  			CL6012X_Write_buffer[9]
#define  CL6012X_POWERDOWN_HIGH 	CL6012X_Write_buffer[10]
#define  CL6012X_POWERDOWN_LOW  	CL6012X_Write_buffer[11]
#endif
LOCAL void CL6012X_FMTune(unsigned int TunerFrequency);
LOCAL  uint32 CL6012X_SetVol(FM_VOL_LEVEL_E vol_level);

/*****************************************************************************/
//  FUNCTION:     CL6012X_I2C_Write
//  Description:  This function will write cl1608 regs through i2c
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void CL6012X_I2C_Write(unsigned char* addr, uint8 cnt)
{
 	I2C_HAL_Write(CL6012X_IIC_Handler, 0, addr, cnt);
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_I2C_Read
//  Description:  This function will reg cl1608 regs through i2c
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void CL6012X_I2C_Read(unsigned char* addr, uint8 cnt)
{
	I2C_HAL_Read(CL6012X_IIC_Handler, 0, addr, cnt);
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_I2C_Read
//  Description:  This function will reg cl1608 regs through i2c
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void CL6012X_ReadAllReg(void)
{
    int32 i       = 0;
    int32 reg_num = sizeof(CL6012X_Read_buffer)/sizeof(unsigned char);
    
    CL6012X_I2C_Read(CL6012X_Read_buffer,reg_num);

	for(i = 0; i< reg_num; i++)
    {
    	//FMDRV_PRINT:"[FM_DRV]CL6012X Reg[%d]= 0x%02x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_145_112_2_18_0_32_45_1413,(uint8*)"dd", i, CL6012X_Read_buffer[i]);
    }
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_Config
//  Description:  this function will write all cl6018 chip regs
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void CL6012X_Config(void)
{
	//FMDRV_PRINT:"CL6012X_Config"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_157_112_2_18_0_32_45_1414,(uint8*)"");
	CL6012X_I2C_Write(CL6012X_Write_buffer,write_register_count);
}


LOCAL BOOLEAN CL6012X_isInitOk(void)
{
    uint8 reg1,reg2;
    CL6012X_ReadAllReg();

    reg1 = CL6012X_Read_buffer[6];
    reg2 = CL6012X_Read_buffer[17];
    if((reg1 != 0xff)&&(reg1 != 0x00)&&(reg2 != 0x00)&&(reg2 != 0xff))
    {
        /* if write reg's begin pos and end pos is ok, 
            then init success
        */
        return SCI_TRUE;
    }

    return SCI_FALSE;	
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_Initial
//  Description:  This function will init the cl6018 FM chip
//  return:     
//  Author:         
//  date:  
//	Note:         FM_SUCCESS: init success, FM_ERROR: init failed  
/*****************************************************************************/
LOCAL uint32 CL6012X_Initial(void)
{    
    //FMDRV_PRINT:"----CL6012X_Initial----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_188_112_2_18_0_32_45_1415,(uint8*)"");

	CL6012X_IIC_cfg.id = FM_I2C_PORT;
	if((CL6012X_IIC_Handler == -1) && 
		((CL6012X_IIC_Handler = I2C_HAL_Open(&CL6012X_IIC_cfg)) == -1))
	{
		//FMDRV_PRINT:"FM::==> open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_194_112_2_18_0_32_45_1416,(uint8*)"");
		return FM_ERROR;
	}
	
    if(CL6012X_isInitOk())
    {
        /*init success*/
        if(CL6012X_QOS_DEVICEID_HIGH == DEV_ID_CL6012X)
        {
        	write_register_count = sizeof(CL6012X_reg_init);
        	SCI_MEMCPY(CL6012X_Write_buffer,CL6012X_reg_init,write_register_count);
        	
        }
	    else
	    {
	 	    write_register_count = sizeof(CL6012_reg_init);
	 	    SCI_MEMCPY(CL6012X_Write_buffer,CL6012_reg_init,write_register_count);
	 	
	    }
        CL6012X_FUNCTION_HIGH  &= 0x7c;   // power on, seek :0 , tune :0 
        CL6012X_FUNCTION_HIGH  |= 0x60;   // DSP Work : 1, mute on : 1; 
        CL6012X_I2C_Write(CL6012X_Write_buffer,2);
        SCI_Sleep(CL6012X_POWER_ON_DELAY);
        CL6012X_SetVol(FM_VOL_LEVEL_9);
        //CL6012X_ReadAllReg();		
        return FM_SUCCESS;
    }

    return FM_ERROR;
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_PlayStart
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 CL6012X_PlayStart(uint16 freq)
{
    CL6012X_FMTune(freq * 10);

    if(FM_VOL_LEVEL_0 != s_cur_vol)
    {
        CL6012X_FUNCTION_HIGH &=0xdf;  /*mute off when vol is not zero*/
        CL6012X_I2C_Write(CL6012X_Write_buffer,2);
    }	

    return CL6012X_RET_OK;
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_PlayStop
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 CL6012X_PlayStop()
{
    //FMDRV_PRINT:"CL6012X_PlayStop()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_252_112_2_18_0_32_45_1417,(uint8*)"");
    // 20101216: Will.Jiang_cr218295 : only mute, do not do any more
    CL6012X_FUNCTION_HIGH |= 0x20;  // mute on
    CL6012X_I2C_Write(CL6012X_Write_buffer, 2);

    return CL6012X_RET_OK;
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_Open
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note: 20101223: will.jiang: open() not used right now, use init() instead       
/*****************************************************************************/
LOCAL void CL6012X_Open(void)
{

}

/*****************************************************************************/
//  FUNCTION:     CL6012X_Close
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void CL6012X_Close(void)
{
    //FMDRV_PRINT:"CL6012X_Close()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_279_112_2_18_0_32_45_1418,(uint8*)"");

	s_is_busy_seek = SCI_FALSE;
	
    CL6012X_FUNCTION_HIGH |= 0x20;  // mute on
    CL6012X_I2C_Write(CL6012X_Write_buffer, 2);
    CL6012X_FUNCTION_HIGH |= 0x80; // set DISABLE=1 to power down
    CL6012X_I2C_Write(CL6012X_Write_buffer, 2);
    // 20101220: will.jiang_cr215111 : CL6012X has no PDDAC bit
    // CL6012X_POWERDOWN_LOW |= 0x80; // set PDDAC = 1 , hgh impedance
    // CL6012X_Config();
    CL6012X_ReadAllReg();
	if(CL6012X_IIC_Handler != -1)
	{
 		I2C_HAL_Close(CL6012X_IIC_Handler);
		CL6012X_IIC_Handler = -1;
	}	
}
/*****************************************************************************/
//  FUNCTION:     CL6012X_FMTune
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note: 20101223: will.jiang: tune() do mute FM chip, please mute off later if necessary
/*****************************************************************************/
LOCAL void CL6012X_FMTune(unsigned int TunerFrequency)
{
	unsigned int ch = 0;

	// frequency transfer to channel number,  channel=(frequencyMhz-70)/0.05, e.g. 87.5Mhz->350, 108Mhz->760
	ch = (TunerFrequency - 7000)/5; 

	// set channel number 
	CL6012X_CHAN_VOL_HIGH &= 0xfc;
	CL6012X_CHAN_VOL_HIGH |= (ch & 0x0300)>>8;
	CL6012X_CHAN_VOL_LOW  =	ch & 0xff;

    // tune disable      Bit9:TUNE->0
    // seek disable      Bit8:SEEK->0
	CL6012X_FUNCTION_HIGH &= 0xfc;
	CL6012X_FUNCTION_HIGH |= 0x20;   // mute on
	CL6012X_I2C_Write(CL6012X_Write_buffer,4);

	CL6012X_FUNCTION_HIGH|= 0x02;//tune=1, begin tune operaton
	CL6012X_I2C_Write(CL6012X_Write_buffer,2);
	SCI_Sleep(50); // wait for 50ms

}
/*****************************************************************************/
//  FUNCTION:     CL6012X_ManualSeek
//  Description:  This function will tune to certain frequency
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 CL6012X_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
	uint32 ch          = 0;
	uint32 read_freq   = 0;  // unit: 100 KHz
	uint8  rssi        = 0;
	uint8  stereo      = 0;
	
	//FMDRV_PRINT:"CL6012X_ManualSeek enter:freq = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_342_112_2_18_0_32_46_1419,(uint8*)"d",freq);

	if (seek_dir==FM_SCAN_UP)
	{
		CL6012X_FUNCTION_LOW |= 0x80;//Seek up
	}
	else
	{
		CL6012X_FUNCTION_LOW &= 0x7f;//Seek down
	}
    CL6012X_FMTune(freq * 10);
    // return current freq
    CL6012X_ReadAllReg(); 
    ch   = ((CL6012X_READ_CHANNEL_HIGH & 0x03) << 8) + CL6012X_READ_CHANNEL_LOW;
    read_freq = (ch*5 + 7000)/10;
    rssi      = CL6012X_STATUS_RSSI_LOW;
    stereo    = CL6012X_STATUS_RSSI_HIGH & BIT_0;
    //FMDRV_PRINT:"[FM_DRV]CL6012X_ManualSeek: freq = %d, signal_strength = %d, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_360_112_2_18_0_32_46_1420,(uint8*)"ddd", read_freq, rssi, stereo);

    if(single_channel_info != PNULL)
	{
		single_channel_info->freq = freq;
	    single_channel_info->signal_strength = rssi;
	    single_channel_info->is_stereo = stereo;
	}

    if(FM_VOL_LEVEL_0 != s_cur_vol)
    {
        CL6012X_FUNCTION_HIGH &=0xdf;  /*mute off when vol is not zero*/
        CL6012X_I2C_Write(CL6012X_Write_buffer,2);
    }	

    return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_AutoSeek
//  Description:  This function will seek from current channel automaticly,if want to seek from certain channel,  
//  use CL6012X_ManualSeek to certain channel ahead
//  return:     
//  Author:         
//  date:  
//	Note: 20101223: will.jiang: autoseek() do mute FM chip, please mute off later if necessary       
/*****************************************************************************/	
 LOCAL  uint32 CL6012X_AutoSeek( uint16 freq, 
                                        FM_SCAN_DIR_E seek_dir,
                                        FM_SCAN_MODE_E mode, 
                                        FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
	uint32   ch       = 0;
  	uint32   loop     = 0;
  	uint8    Temp[6]  = {0};
  	uint32   ret      = FM_SUCCESS;
	BOOLEAN  stcflag  = SCI_FALSE, sfflag = SCI_FALSE;
    
	// check busy or not            
	if(SCI_FALSE == s_is_busy_seek)
	{
    	//_FM_Disable_AudioOutput();
    	if (seek_dir==FM_SCAN_UP)
    	{
    		CL6012X_FUNCTION_LOW  |= 0x80;//Seek up
    	}
    	else
    	{
    		CL6012X_FUNCTION_LOW  &= 0x7f;//Seek down
    	}

    	CL6012X_FMTune(freq * 10);
    	// seek operation
    	CL6012X_FUNCTION_HIGH &= 0xfc; //tune=0,seek=0
    	CL6012X_I2C_Write(CL6012X_Write_buffer, 2);
    	CL6012X_FUNCTION_HIGH |= 0x01; //seek=1, begin seek operation
    	CL6012X_I2C_Write(CL6012X_Write_buffer, 2);
    }

	// waiting seek completed or timeout
    do
    {
        SCI_Sleep(CL6012X_SEEK_INTERVAL); 
        CL6012X_I2C_Read(Temp, 6);  
        stcflag = Temp[0] & 0x04; // STC indicator
        sfflag  = Temp[0] & 0x02; //  SF indicator
        loop++;
    }while((!stcflag) && (loop < CL6012X_SEEK_MAX_CNT));

    // sfflag  : 0: Seek successful; 1: Seek fail 
    // stcflag : 0: Notcomplete;     1: Complete
    if( SCI_FALSE == stcflag)
    {
        s_is_busy_seek = SCI_TRUE;
        ret = FM_NOT_COMPLETE;
    }
    else
    {
        s_is_busy_seek = SCI_FALSE;

        if( SCI_FALSE == sfflag)
            ret = FM_SUCCESS;
        else 
            ret = FM_ERROR;
    }
    // return current freq
    CL6012X_ReadAllReg(); 
    ch = ((CL6012X_READ_CHANNEL_HIGH & 0x03) << 8) + CL6012X_READ_CHANNEL_LOW;
    s_frequency =  (ch*5 + 7000)/10;	
    //FMDRV_PRINT:"[FM_DRV]CL6012X_AutoSeek: stc= %d, sff= %d, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_449_112_2_18_0_32_46_1421,(uint8*)"ddddd", stcflag, sfflag, s_is_busy_seek, ret, s_frequency);
    if(all_channel_info)
    {
        all_channel_info->freq_count = 1;
        all_channel_info->auto_freq_tab_ptr  = &s_frequency;
    }
	
	return ret;
}

/*****************************************************************************/
//  FUNCTION:     CL6012X_SetVol
//  Description:  This function will  set FM chip side volume
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 CL6012X_SetVol(FM_VOL_LEVEL_E vol_level)
{
	static const uint8 s_u8_vol_tab[] = 
	{
		CL6012X_VOLUME_LEVEL0,	//FM_VOL_LEVEL_0
		CL6012X_VOLUME_LEVEL1,  //FM_VOL_LEVEL_1
		CL6012X_VOLUME_LEVEL2,  //FM_VOL_LEVEL_2
		CL6012X_VOLUME_LEVEL3,  //FM_VOL_LEVEL_3
		CL6012X_VOLUME_LEVEL4,  //FM_VOL_LEVEL_4
		CL6012X_VOLUME_LEVEL5,  //FM_VOL_LEVEL_5
		CL6012X_VOLUME_LEVEL6,  //FM_VOL_LEVEL_6
		CL6012X_VOLUME_LEVEL7,  //FM_VOL_LEVEL_7
		CL6012X_VOLUME_LEVEL8,  //FM_VOL_LEVEL_8
		CL6012X_VOLUME_LEVEL9   //FM_VOL_LEVEL_9
	};
    int32 vol_value;
	
	//FMDRV_PRINT:"--CL6012X_SetVol:vol_level=%d---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CL6012X_482_112_2_18_0_32_46_1422,(uint8*)"d",vol_level);
	if(vol_level > FM_VOL_LEVEL_9)
	{
		vol_level = FM_VOL_LEVEL_6;
	}
	vol_value = s_u8_vol_tab[vol_level];

    CL6012X_BITSET(CL6012X_CHAN_VOL_HIGH, BIT_2|BIT_3|BIT_4|BIT_5, vol_value);
    s_cur_vol = vol_level;
	
    if(FM_VOL_LEVEL_0 == vol_level)
    {
        CL6012X_FUNCTION_HIGH |= 0x20; /*mute on when vol is zero*/	    
	}
    else
    {
        CL6012X_FUNCTION_HIGH &= 0xdf; /*mute off when vol is not zero*/	    
    }

    CL6012X_I2C_Write(CL6012X_Write_buffer, 4);

    CL6012X_ReadAllReg();
    return SCI_TRUE;
}
/*****************************************************************************/
//  FUNCTION:     CL6012X_SetFMRegion
//  Description:  This function will select band region.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32  CL6012X_SetFMRegion(FM_REGION_E  region)
{
	switch(region)
	{
		case FM_REGION_US:
			CL6012X_BITSET(CL6012X_SYSCONFIG_HIGH, BIT_6|BIT_7, CL6012X_CHINA_BAND);
			break;
		case FM_REGION_JAPAN:
			CL6012X_BITSET(CL6012X_SYSCONFIG_HIGH, BIT_6|BIT_7, CL6012X_JAPAN_BAND);
			break;	
		default:
			CL6012X_BITSET(CL6012X_SYSCONFIG_HIGH, BIT_6|BIT_7, CL6012X_CHINA_BAND);
			break;
	}
	CL6012X_Config();
	return SCI_TRUE;
}
/*****************************************************************************/
//  FUNCTION:     CL6012X_SetScanLevel
//  Description:  This function will config scane rssi level
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32 CL6012X_SetScanLevel(FM_SCANLEVEL_E level)
{
	switch( level )
	{
		case FM_SCANLEVEL_LOW:
			CL6012X_BITSET(CL6012X_SEEK_HIGH, 0xff, CL6012X_SSL_LEVEL_LOW);
			break;
		case FM_SCANLEVEL_MID:
			CL6012X_BITSET(CL6012X_SEEK_HIGH, 0xff, CL6012X_SSL_LEVEL_MID);
			break;
		case FM_SCANLEVEL_HIGH:
			CL6012X_BITSET(CL6012X_SEEK_HIGH, 0xff, CL6012X_SSL_LEVEL_HIGH);
			break;
		default:
			CL6012X_BITSET(CL6012X_SEEK_HIGH, 0xff, CL6012X_SSL_LEVEL_HIGH);
			break;
	}
	CL6012X_Config();
	return SCI_TRUE;
}


//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     CL6012X_FM_Ioctl
//  Description:  This function will contrl fm command
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	 
LOCAL uint32 CL6012X_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}

//FM operations set
PUBLIC FM_OPERATION_T g_CL6012X_ops=
{
    CL6012X_Initial,   
    CL6012X_Open,     
    CL6012X_Close ,   

    CL6012X_PlayStart,
    CL6012X_PlayStop,

    CL6012X_ManualSeek,
    CL6012X_AutoSeek,

    CL6012X_SetScanLevel,
    CL6012X_SetVol,

    CL6012X_SetFMRegion,
    CL6012X_FM_Ioctl
};


