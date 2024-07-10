/******************************************************************************
 ** File Name:      fm_kt0812g.c                                              *
 ** Author:         Sunny.Fei                                                 *
 ** DATE:           2010.12.06                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.12.06   Sunny.Fei             Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "fm_drv.h"
#include "fm_cfg.h"
#include "fm_kt0812g.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

#ifdef WIN32
#define __func__
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/

#define FM_I2C_PORT                 FM_GetCtrlPort()
#define KT0812G_I2C_WRITE_ADDR      0x6e
#define KT0812G_I2C_READ_ADDR       0x6f 

#define KT0812G_WAKEUP_TIME         100   // ms 
#define KT0812G_WAKEUP_CHK_CNT      8   

#define KT0812G_TUNE_TIME           50    // ms 
#define KT0812G_SEEK_MAX_CNT        10    // for APP to return to do other work

#define DEV_ID_KT0812G              0x0440 // KT0812G CHIP ID(Reg0x01):FM Receiver
#define KT0812G_ONE_REG_BYTE_CNT    2  // 16-bit ( 2 bytes) register

#define KT0812G_U8_TO_U16(_U8_H, _U8_L, _U16)   ((_U16) = (_U8_H) << 8 | (_U8_L))
#define KT0812G_U16_TO_U8(_U16, _U8_H, _U8_L)   \
        do{\
        	 _U8_H = (uint8) ((_U16) >> 8); \
       		 _U8_L = (uint8) ((_U16) & 0xff);}while(0)

#define KT0812G_REG_OR(reg, value)      ((reg) |= (value))
#define KT0812G_REG_AND(reg, value)     ((reg) &= (value))
#define KT0812G_REG_SET(reg, bit_mask, value)     \
        do{\
        	 (reg) &= ~(bit_mask); \
       		 (reg) |=  (value);}while(0)

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
 
LOCAL I2C_DEV KT0812G_IIC_cfg = 
{
	1,            	    	//id, will be reinit during init()/ open()
	100*1000,   			//freq
	0,						//bus
	KT0812G_I2C_WRITE_ADDR, //slave_addr
	1,						//reg_addr_num
	1,						//check_ack
	1						// no_stop
};

LOCAL int32 KT0812G_IIC_Handler = -1;


/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
LOCAL  uint32 KT0812G_SetVol(FM_VOL_LEVEL_E vol_level);

/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/

LOCAL const uint8 KT0812G_reg_addr[]={
    KT0812G_REG_DEVICE, 
    KT0812G_REG_CHIPID, 
    KT0812G_REG_SEEK, 
    KT0812G_REG_TUNE,
    KT0812G_REG_VOLUME, 
    KT0812G_REG_DSPCFGA, 
    KT0812G_REG_RFCFG, 
    KT0812G_REG_LOCFGA,
    KT0812G_REG_SYSCFG, 
    KT0812G_REG_STATUSA, 
    KT0812G_REG_STATUSB, 
    KT0812G_REG_STATUSC,
    KT0812G_REG_STATUSD, 
    KT0812G_REG_ANTENNA, 
    KT0812G_REG_SNR, 
    KT0812G_REG_SEEKTH,
    KT0812G_REG_SOFTMUTE, 
    KT0812G_REG_CLOCK
};



LOCAL uint16    s_frequency = 0;
LOCAL uint8     s_cur_vol = 0;
LOCAL BOOLEAN   s_is_busy_seek = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  FUNCTION:     KT0812G_init_i2c_handler
//  Description:  This function will get i2c handler
//	Note:  
/*****************************************************************************/
LOCAL int32 KT0812G_init_i2c_handler(void)
{

	KT0812G_IIC_cfg.id = FM_I2C_PORT;
	if((KT0812G_IIC_Handler == -1) && 
		((KT0812G_IIC_Handler = I2C_HAL_Open(&KT0812G_IIC_cfg)) == -1))
	{
		//FMDRV_PRINT:"[DRV_FM]==> open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_128_112_2_18_0_32_47_1423,(uint8*)"");
		return -1;
	}
	return KT0812G_IIC_Handler;
}


/*****************************************************************************/
//  FUNCTION:     KT0812G_release_i2c_handler
//  Description:  This function will release i2c handler
//	Note:  
/*****************************************************************************/
LOCAL int32 KT0812G_release_i2c_handler(void)
{
	if(KT0812G_IIC_Handler != -1 && SCI_InThreadContext() )
	{
 		I2C_HAL_Close(KT0812G_IIC_Handler);
		KT0812G_IIC_Handler = -1;
	}
	return KT0812G_IIC_Handler;
}


/*****************************************************************************/
//  FUNCTION:     _Write_One_Reg
//  Description:  This function will write kt0812g regs through i2c
//	Note:        
/*****************************************************************************/
LOCAL void _Write_One_Reg(uint8 addr, uint16 val)
{
	uint8  Reg_buf[KT0812G_ONE_REG_BYTE_CNT] = {0};

    if(KT0812G_IIC_Handler == -1)
    {
        KT0812G_init_i2c_handler();
    }
    //FMDRV_PRINT:"[DRV_FM] _Write_One_Reg: [0x%02X] = 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_160_112_2_18_0_32_47_1424,(uint8*)"dd", addr, val);
    KT0812G_U16_TO_U8(val, Reg_buf[0], Reg_buf[1]);
	I2C_HAL_Write(KT0812G_IIC_Handler, &addr, Reg_buf, KT0812G_ONE_REG_BYTE_CNT);    
	return;
}
 
/*****************************************************************************/
//  FUNCTION:     _Read_One_Reg
//  Description:  This function will reg kt0812g regs through i2c
//	Note:  
/*****************************************************************************/
LOCAL uint16 _Read_One_Reg(uint8 addr)
{  
	uint8  Reg_buf[KT0812G_ONE_REG_BYTE_CNT] = {0};
    uint16 reg_val = 0;
    
    if(KT0812G_IIC_Handler == -1)
    {
        KT0812G_init_i2c_handler();
    }   
	I2C_HAL_Read(KT0812G_IIC_Handler, &addr, Reg_buf, KT0812G_ONE_REG_BYTE_CNT);
    KT0812G_U8_TO_U16(Reg_buf[0], Reg_buf[1], reg_val);
    //FMDRV_PRINT:"[DRV_FM] _Read_One_Reg: [0x%02X] = 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_180_112_2_18_0_32_47_1425,(uint8*)"dd", addr, reg_val);
    return reg_val;	                 
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_I2C_Read
//  Description:  This function will reg kt0812g regs through i2c
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void KT0812G_ReadAllReg(void)
{
	uint32 i =0;
	uint16 reg_val = 0;
	uint32 reg_num = sizeof(KT0812G_reg_addr)/sizeof(KT0812G_reg_addr[0]);
	
	for(i=0; i<reg_num; i++)
	{        
         reg_val = _Read_One_Reg(KT0812G_reg_addr[i]);
    	 //FMDRV_PRINT:"[DRV_FM] KT0812G_ReadAllReg: [0x%02X]=0x%04X "
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_199_112_2_18_0_32_47_1426,(uint8*)"dd", KT0812G_reg_addr[i], reg_val);
	}
	return;
}


 /*****************************************************************************/
//  FUNCTION:     KT0812G_Initial
//  Description:  This function will init the kt0812g FM chip
//                set register and open i2c bus.
//  return:     
//  Author:         
//  date:  
//	Note:         FM_SUCCESS: init success, FM_ERROR: init failed  
/*****************************************************************************/
LOCAL uint32 KT0812G_Initial(void)
{    
    uint16 reg_val = 0;
    uint32 i = 0;
    //FMDRV_PRINT:"[DRV_FM] KT0812G_Initial: i2c port = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_217_112_2_18_0_32_47_1427,(uint8*)"d", FM_I2C_PORT);


    KT0812G_init_i2c_handler();

    reg_val = 0x8a00; //wakeup
    _Write_One_Reg(KT0812G_REG_SYSCFG, reg_val);

    // [0x23]= 0x1001: set KT0812G_ANT_TUNE_EN & KT0812G_RCLK_EN,32KHz
     reg_val = 0x0001 | KT0812G_RCLK_EN ;  
    _Write_One_Reg(KT0812G_REG_CLOCK, reg_val);
      
    // wait FM chip ready according to Datasheet and Q&A
    do
    {
        SCI_Sleep(KT0812G_WAKEUP_TIME);
        // check chip states
    	reg_val = _Read_One_Reg(KT0812G_REG_STATUSA);
        if((KT0812G_XTAL_OK  & reg_val) &&
           (KT0812G_PLL_LOCK & reg_val))
        {
            break;
        }
        i++;
    }while(i < KT0812G_WAKEUP_CHK_CNT);
    
    //FMDRV_PRINT:"[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X, i = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_243_112_2_18_0_32_47_1428,(uint8*)"dd", reg_val, i);
    if(i >= KT0812G_WAKEUP_CHK_CNT)
    {
       return FM_ERROR;
    }

    // [0x2]= 0x2207: default SEEK_TH, 87--108MHz, 100Khz space
    reg_val = 0x2203 | KT0812G_CHAN_JAPAN_SPACE;
    _Write_One_Reg(KT0812G_REG_SEEK, reg_val);
    
    // [0x4]= 0xC027: MUTE_B Disable 
    reg_val = 0x8027 | KT0812G_MUTE_B;
    _Write_One_Reg(KT0812G_REG_VOLUME, reg_val);
    
    // [0xa]= 0x0000: Turn On AFC
    reg_val = KT0812G_LO_AFCD_ENABLE;
    _Write_One_Reg(KT0812G_REG_LOCFGA, reg_val);
    
    // [0xf]= 0x8a40: wakeup, adv_seek(HW_SEEK)
    reg_val = 0x8a00 | KT0812G_SEEK_SEL;
    _Write_One_Reg(KT0812G_REG_SYSCFG, reg_val);
    
    KT0812G_ReadAllReg(); // useful for debug
    
    return FM_SUCCESS;

}


/*****************************************************************************/
//  FUNCTION:     KT0812G_Open
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void KT0812G_Open(void)
{
    //FMDRV_PRINT:"[DRV_FM] KT0812G_Open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_280_112_2_18_0_32_47_1429,(uint8*)"");
    return;
}

/*****************************************************************************/
//  FUNCTION:     KT0812G_Close
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void KT0812G_Close(void)
{
    uint16 reg_val = 0;
    //FMDRV_PRINT:"[DRV_FM] KT0812G_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_293_112_2_18_0_32_47_1430,(uint8*)"");
	s_is_busy_seek = SCI_FALSE;
    // [0xf]= 0x8a40: standby
	reg_val = _Read_One_Reg(KT0812G_REG_SYSCFG);
    KT0812G_REG_OR(reg_val, KT0812G_STANDBY_EN);
    _Write_One_Reg(KT0812G_REG_SYSCFG, reg_val);
    KT0812G_release_i2c_handler(); 
    return;
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_FMTune
//  Description:  
//  Parameter:
//      Freq : unit 100KHz
//  return:  
//	Note: 
/*****************************************************************************/
LOCAL void KT0812G_FMTune(uint16 freq)
{
	uint16 ch = 0;
    uint16 reg_val = 0;
	// frequency transfer to channel number, 
	// freq (MHz) = 50KHz X CHAN + 64MHz ==> freq (100KHz) = 0.5 * ch + 640
	// ch = (freq - 640)/0.5 ==>
	ch =  (freq -640) * 2;
    // read TUNE first
	reg_val = _Read_One_Reg(KT0812G_REG_TUNE);
    if(reg_val & KT0812G_TUNE_ENABLE )
    {
        // disable TUNE first
        KT0812G_REG_AND(reg_val, ~KT0812G_TUNE_ENABLE);
        _Write_One_Reg(KT0812G_REG_TUNE, reg_val);
        reg_val = _Read_One_Reg(KT0812G_REG_TUNE);
    }
    // set chan and TUNE 
    KT0812G_REG_SET(reg_val, KT0812G_TUNE_CHAN, ch);
    KT0812G_REG_OR(reg_val, KT0812G_TUNE_ENABLE);
    _Write_One_Reg(KT0812G_REG_TUNE, reg_val);
    //FMDRV_PRINT:"[DRV_FM] KT0812G_FMTune: at TUNE = 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_329_112_2_18_0_32_47_1431,(uint8*)"d", reg_val);
    
    // it's TUNE time
    SCI_Sleep(KT0812G_TUNE_TIME);
    // read STATUS
    //FMDRV_PRINT:"[DRV_FM] KT0812G_FMTune: After TUNE = 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_334_112_2_18_0_32_47_1432,(uint8*)"d", _Read_One_Reg(KT0812G_REG_TUNE));
    //FMDRV_PRINT:"[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_335_112_2_18_0_32_47_1433,(uint8*)"d", _Read_One_Reg(KT0812G_REG_STATUSA));
    //FMDRV_PRINT:"[DRV_FM] KT0812G_FMTune: READ CHAN= 0x%04X "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_336_112_2_18_0_32_47_1434,(uint8*)"d", _Read_One_Reg(KT0812G_REG_STATUSB));
    return;
    
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_GetTuneFreq
//  Description:  
//  Parameter:
//  return:  
//      Freq : unit 100KHz
//	Note: 
/*****************************************************************************/

LOCAL uint16 KT0812G_GetTuneFreq(void)
{
    uint16 reg_val = 0;
    uint16 freq = 0;
    reg_val = _Read_One_Reg(KT0812G_REG_STATUSB);
	// freq (MHz) = 50KHz X CHAN + 64MHz ==> freq (100KHz) = 0.5 * ch + 640
    freq  =(reg_val & KT0812G_READ_CHAN)/2 + 640;
    //FMDRV_PRINT:"[DRV_FM] KT0812G_GetTuneFreq: CHAN= 0x%04X, Freq = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_354_112_2_18_0_32_47_1435,(uint8*)"dd", reg_val, freq);
    return freq;
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_FMSeek
//  Description:  
//  Parameter:
//      Freq : unit 100KHz
//  return:     
//  Author:         
//  date:  
//	Note: 20101223: will.jiang: tune() do mute FM chip, please mute off later if necessary
/*****************************************************************************/
LOCAL void KT0812G_FMSeek(uint16 freq, 
                                 FM_SCAN_DIR_E seek_dir,
                                 FM_SCAN_MODE_E mode )
{
    uint16 reg_val = 0;
    // set start freq
    KT0812G_FMTune(freq);
    // read seek first
	reg_val = _Read_One_Reg(KT0812G_REG_SEEK);
	if(reg_val & KT0812G_SEEK_ENABLE )
    {
        // disable SEEK first
        KT0812G_REG_AND(reg_val, ~KT0812G_REG_SEEK);
        _Write_One_Reg(KT0812G_REG_SEEK, reg_val);
        reg_val = _Read_One_Reg(KT0812G_REG_SEEK);
    }
    // set seek dir
	if (seek_dir==FM_SCAN_UP)
	{
		KT0812G_REG_OR(reg_val, KT0812G_SEEK_DIR);//Seek up
	}
	else
	{
		KT0812G_REG_AND(reg_val, ~KT0812G_SEEK_DIR);//Seek down
	}
    // seeking...
    KT0812G_REG_OR(reg_val, KT0812G_SEEK_ENABLE);
	_Write_One_Reg(KT0812G_REG_SEEK, reg_val);
	return;
    	
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_FMMute
//  Description:  
//	Note: 
/*****************************************************************************/
LOCAL void KT0812G_FMMute(BOOLEAN enable)
{
    return;
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_PlayStart
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 KT0812G_PlayStart(uint16 Freq)
{  
    //FMDRV_PRINT:"[DRV_FM] KT0812G_PlayStart: Freq = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_411_112_2_18_0_32_47_1436,(uint8*)"d", Freq);
    KT0812G_FMTune(Freq); 
    return FM_SUCCESS;
}



/*****************************************************************************/
//  FUNCTION:     KT0812G_PlayStop
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 KT0812G_PlayStop()
{
    //FMDRV_PRINT:"[DRV_FM] KT0812G_PlayStop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_426_112_2_18_0_32_47_1437,(uint8*)"");
    return FM_SUCCESS;
}



/*****************************************************************************/
//  FUNCTION:     KT0812G_ManualSeek
//  Description:  This function will tune to certain frequency
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 KT0812G_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
	uint32 read_freq   = 0;  // unit: 100 KHz
	uint8  rssi        = 0;
	uint8  stereo      = 0;
	
    KT0812G_FMTune(freq);
   
    read_freq = KT0812G_GetTuneFreq();
    rssi      = 0;
    stereo    = 0;
    //FMDRV_PRINT:"[DRV_FM]KT0812G_ManualSeek: freq = %d, rssi = %d, stereo = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_453_112_2_18_0_32_47_1438,(uint8*)"ddd",read_freq, rssi, stereo);

    if(single_channel_info != PNULL)
	{
		single_channel_info->freq = read_freq;
	    single_channel_info->signal_strength = rssi;
	    single_channel_info->is_stereo = stereo;
	}

	return FM_SUCCESS;
}



/*****************************************************************************/
//  FUNCTION:     KT0812G_AutoSeek
//  Description:  This function will seek from current channel automaticly,if want to seek from certain channel,  
//  use KT0812G_ManualSeek to certain channel ahead
//  return:     
//  Author:         
//  date:  
//	Note: 20101223: will.jiang: autoseek() do mute FM chip, please mute off later if necessary       
/*****************************************************************************/	
 LOCAL  uint32 KT0812G_AutoSeek( uint16 freq, 
                                        FM_SCAN_DIR_E seek_dir,
                                        FM_SCAN_MODE_E mode, 
                                        FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
    //FMDRV_PRINT("[DRV_FM] KT0812G_AutoSeek"); 
	uint32   ch       = 0;
  	uint32   loop     = 0;
  	uint32   ret      = FM_SUCCESS;
	BOOLEAN  seek_end  = SCI_FALSE, seek_fail = SCI_FALSE;
    uint16   reg_val  = 0;

    
	// check busy or not            
	if(SCI_FALSE == s_is_busy_seek)
	{ 
        KT0812G_FMSeek(freq,seek_dir,mode);
    }

	// waiting seek completed or timeout
    do
    {   
        SCI_Sleep(KT0812G_TUNE_TIME); // the same as tune
        reg_val   = _Read_One_Reg(KT0812G_REG_STATUSA);  
        seek_end  = (KT0812G_SEEKTUNE_COMP == (KT0812G_SEEKTUNE_COMP & reg_val));
        loop++;
    }while((!seek_end) && (loop < KT0812G_SEEK_MAX_CNT));

    if(seek_end)
    {
        s_is_busy_seek = SCI_FALSE;
        seek_fail = (KT0812G_SF_BL_FAIL == (KT0812G_SF_BL_FAIL & reg_val));
        if(seek_fail)
            ret = FM_ERROR;
        else 
            ret = FM_SUCCESS;
    }
    else
    {
        s_is_busy_seek = SCI_TRUE;
        ret = FM_NOT_COMPLETE;
    }
    s_frequency =   KT0812G_GetTuneFreq();	
    //FMDRV_PRINT:"[DRV_FM]KT0812G_AutoSeek: seek_dir = %d, seek_end= %d, seek_fail= %d, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_519_112_2_18_0_32_47_1439,(uint8*)"dddddd", seek_dir, seek_end, seek_fail, s_is_busy_seek, ret, s_frequency);
    if(all_channel_info)
    {
        all_channel_info->freq_count = 1;
        all_channel_info->auto_freq_tab_ptr  = &s_frequency;
    }
	
	return ret;
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_SetScanLevel
//  Description:  This function will config scane rssi level
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32 KT0812G_SetScanLevel(FM_SCANLEVEL_E level)
{
    //FMDRV_PRINT:"[DRV_FM] KT0812G_SetScanLevel"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_536_112_2_18_0_32_47_1440,(uint8*)"");
	return FM_SUCCESS;
}
/*****************************************************************************/
//  FUNCTION:     KT0812G_SetVol
//  Description:  This function will  set FM chip side volume
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 KT0812G_SetVol(FM_VOL_LEVEL_E vol_level)
{
    uint16       reg_val   = 0;
	const uint16 vol_tab[] = 
	{
		KT0812G_VOLUME_LEVEL0,	//FM_VOL_LEVEL_0
		KT0812G_VOLUME_LEVEL1,  //FM_VOL_LEVEL_1
		KT0812G_VOLUME_LEVEL2,  //FM_VOL_LEVEL_2
		KT0812G_VOLUME_LEVEL3,  //FM_VOL_LEVEL_3
		KT0812G_VOLUME_LEVEL4,  //FM_VOL_LEVEL_4
		KT0812G_VOLUME_LEVEL5,  //FM_VOL_LEVEL_5
		KT0812G_VOLUME_LEVEL6,  //FM_VOL_LEVEL_6
		KT0812G_VOLUME_LEVEL7,  //FM_VOL_LEVEL_7
		KT0812G_VOLUME_LEVEL8,  //FM_VOL_LEVEL_8
		KT0812G_VOLUME_LEVEL9   //FM_VOL_LEVEL_9
	};

	if(vol_level > FM_VOL_LEVEL_9)
	{
		s_cur_vol = FM_VOL_LEVEL_9;
	}
	else
	{
		s_cur_vol = vol_level;
	}
	//FMDRV_PRINT:"[DRV_FM] KT0812G_SetVol = %d, s_cur_vol = %d, RegVol = %02X"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_571_112_2_18_0_32_47_1441,(uint8*)"ddd",vol_level, s_cur_vol, vol_tab[s_cur_vol]);
	
	reg_val = _Read_One_Reg(KT0812G_REG_VOLUME);
    KT0812G_REG_SET(reg_val, KT0812G_VOLUME, vol_tab[s_cur_vol]);
    _Write_One_Reg(KT0812G_REG_VOLUME, reg_val);
    
    return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     KT0812G_SetFMRegion
//  Description:  This function will select band region.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32  KT0812G_SetFMRegion(FM_REGION_E  region)
{
    //FMDRV_PRINT:"[DRV_FM] KT0812G_SetFMRegion"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_588_112_2_18_0_32_47_1442,(uint8*)"");

	return FM_SUCCESS;
}
/*****************************************************************************/
//  Description:  force fm to output stereo or mono 
//	Note:        
/*****************************************************************************/	
LOCAL uint32  KT0812G_SetStereo(BOOLEAN is_on)
{
    uint16 reg_val = 0;

    //FMDRV_PRINT:"[DRV_FM] KT0812G_SetStereo = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_KT0812G_598_112_2_18_0_32_47_1443,(uint8*)"d",is_on);

   	reg_val = _Read_One_Reg(KT0812G_REG_DSPCFGA);
	if (is_on)
	{
		KT0812G_REG_AND(reg_val, ~KT0812G_DSP_MONO);//stereo
	}
	else
	{
		KT0812G_REG_OR(reg_val, KT0812G_DSP_MONO);//mono
	}
	
    _Write_One_Reg(KT0812G_REG_DSPCFGA, reg_val);
    
	return FM_SUCCESS;
}

//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     KT0812G_FM_Ioctl
//  Description:  This function will contrl fm command
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	 
LOCAL uint32 KT0812G_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}

PUBLIC FM_OPERATION_T g_KT0812G_ops=
{
    KT0812G_Initial,   
    KT0812G_Open,     
    KT0812G_Close ,   

    KT0812G_PlayStart,
    KT0812G_PlayStop,

    KT0812G_ManualSeek,
    KT0812G_AutoSeek,

    KT0812G_SetScanLevel,
    KT0812G_SetVol,

    KT0812G_SetFMRegion,
    KT0812G_FM_Ioctl
};
#ifdef __cplusplus
}
#endif

/******************************************************************************/
