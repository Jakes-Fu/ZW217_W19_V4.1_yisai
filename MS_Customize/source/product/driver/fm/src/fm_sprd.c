/******************************************************************************
 ** File Name:      fm_phy_v0.c                                           *
 ** Author:         Jackey Ling                                           *
 ** DATE:           15/10/2012                                            *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:    This file defines the basic information on chip.      *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "fm_drvapi.h"
#include "fm_drv.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

LOCAL uint16 s_frequency = 0;
LOCAL uint32 s_fm_seek_state = 0;
LOCAL BOOLEAN s_fm_init_state = SCI_FALSE;

/*****************************************************************************/
//  FUNCTION:     fm_sprd_Initial
//  Description:  This function will init the sprd FM chip
//  return:     
//  Author:         
//  date:  
//  Note:         FM_SUCCESS: init success, FM_ERROR: init failed  
/*****************************************************************************/
LOCAL uint32 FM_SPRD_Initial(void)
{     
    FM_SPRD_Enable();
    s_fm_init_state = SCI_TRUE;

	//Add by luther.li 2012-12-6  init s_fm_seek_state value
	s_fm_seek_state = 0;

    return FM_SUCCESS;
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_Open
//  Description:  This function will open the sprd FM chip
//  return:     
//  Author:         
//  date:  
//  Note:        
/*****************************************************************************/
LOCAL void FM_SPRD_Open(void)
{   
    if(s_fm_init_state == SCI_FALSE)
    {
        FM_SPRD_Initial();
    }
    SCI_TRACE_LOW("FM_SPRD_Open");     	
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_Close
//  Description:  This function will close  the sprd FM chip
//  return:     
//  Author:         
//  date:  
//  Note:        
/*****************************************************************************/
LOCAL void FM_SPRD_Close(void)
{   
    FM_SPRD_Disable();
    s_fm_init_state = SCI_FALSE;

	//Add by luther.li 2012-12-06 init s_fm_seek_state value
	s_fm_seek_state = 0;

    SCI_TRACE_LOW("FM_SPRD_Close");

}

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_PlayStart
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 FM_SPRD_PlayStart(uint16 freq)
{
    if(s_fm_init_state == SCI_FALSE)
    {
        FM_SPRD_Initial();
    }   
    SCI_TRACE_LOW("FM_SPRD_PlayStart");    
    return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_PlayStop
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 FM_SPRD_PlayStop()
{
    SCI_TRACE_LOW("FM_SPRD_PlayStop");   
    FM_SPRD_Close();           
    return FM_SUCCESS;
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_ManualSeek
//  Description:  This function will tune to certain frequency
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 FM_SPRD_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{    
	SCI_TRACE_LOW("FM_SPRD_ManualSeek freq =%d ",freq); 
	
    if(s_fm_init_state == SCI_FALSE)
    {
        FM_SPRD_Initial();
    }
	
    FM_SPRD_Tune(freq,seek_dir); 
    
    s_frequency = FM_SPRD_GetCurFreq(); 
    
    if(single_channel_info != PNULL)
    {			
            single_channel_info->freq = s_frequency;
            single_channel_info->signal_strength = 0;  
            single_channel_info->is_stereo = 0;
    }

    return SCI_TRUE;
}



/*****************************************************************************/
//  FUNCTION:     FM_SPRD_AutoSeek
//  Description:  This function will seek from current channel automaticly,if want to seek from certain channel,  
//  use FM_SPRD_AutoSeek to certain channel ahead
//  return:     
//  Author:         
//  date:  
//	Note: autoseek() do mute FM chip, please mute off later if necessary       
/*****************************************************************************/	
 LOCAL  uint32 FM_SPRD_AutoSeek( uint16 freq, 
                                        FM_SCAN_DIR_E seek_dir,
                                        FM_SCAN_MODE_E mode, 
                                        FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
  	   // uint32   loop = 0; /*lint !e529*/
        uint32   ret = 0;
        
        if(s_fm_init_state == SCI_FALSE)
        {
            FM_SPRD_Initial();
        }
        
        if(FM_NOT_COMPLETE == s_fm_seek_state)
        {
            freq = s_frequency;
        }
        
        SCI_TRACE_LOW("2012-12-27 FM_SPRD_AutoSeek freq =%d ",freq);

        if(FM_SCAN_UP == seek_dir)
            ret = FM_SPRD_Seek(freq+1,seek_dir);
        else
            ret = FM_SPRD_Seek(freq-1,seek_dir);
            
        s_frequency = FM_SPRD_GetCurFreq();

        SCI_TRACE_LOW("2012-12-27 s_frequency =%d ",s_frequency);//add by luther.li 2012-12-27

        if(ret)
        {
	        s_fm_seek_state = FM_SUCCESS;
        }
        else if(s_frequency < SPRD_FREQ_UP_LIMIT)
        {
            s_fm_seek_state = FM_NOT_COMPLETE ;
        }   
        else   
        {
           s_frequency = SPRD_FREQ_DOWN_LIMIT;
           s_fm_seek_state = FM_SUCCESS; 
        }
	        
        all_channel_info->freq_count = 1;
        all_channel_info->auto_freq_tab_ptr = &s_frequency;
        
        SCI_TRACE_LOW("[FM_DRV]SPRD_AutoSeek: ret = %d,  state = %d, freq = %d", 
                     ret, s_fm_seek_state, all_channel_info->auto_freq_tab_ptr[0]);
        
        return s_fm_seek_state; 
}

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_SetScanLevel
//  Description:  This function will  set FM chip side volume
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 FM_SPRD_SetScanLevel(FM_SCANLEVEL_E level)
{
	return SCI_TRUE;
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_SetVol
//  Description:  This function will  set FM chip side volume
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 FM_SPRD_SetVol(FM_VOL_LEVEL_E vol_level)
{
	
        return SCI_TRUE;
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_SetFMRegion
//  Description:  This function will select band region.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32  FM_SPRD_SetFMRegion(FM_REGION_E  region)
{
	return SCI_TRUE;
}


LOCAL uint32 FM_SPRD_Ioctl(uint32 cmd, void *data)
{
    uint16 data_val = 0;
    switch(cmd)
    {
    case FM_SET_THRESHOLD_RSSI:
		break;
	//Add by luther.li 2012-12-01 set fm volume
	case FM_GET_STEREO:
		data_val = FM_SPRD_RecStereo();
		if(data != PNULL)
		{
		    *((uint16 *)data) = data_val;
		}	
		break;
	case FM_GET_RSSI:
		data_val = FM_SPRD_RecRSSI();
		if(data != PNULL)
		{
			*((uint16 *)data) = data_val;
		}
		break;
	case FM_CHANNEL_SPACE:
            break;
	default:
	    break;
    }
    return 0;
}

/*****************************************************************************/
//  FUNCTION:     g_fm_sprd_ops
//  Description:  This function will return FM_OPERATION_T.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
//FM operations set
PUBLIC FM_OPERATION_T g_fm_sprd_ops=
{
    FM_SPRD_Initial,   
    FM_SPRD_Open,     
    FM_SPRD_Close ,   

    FM_SPRD_PlayStart,
    
    FM_SPRD_PlayStop,

    FM_SPRD_ManualSeek,
    FM_SPRD_AutoSeek,

    FM_SPRD_SetScanLevel,
    FM_SPRD_SetVol,
    FM_SPRD_SetFMRegion,
    FM_SPRD_Ioctl
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  




