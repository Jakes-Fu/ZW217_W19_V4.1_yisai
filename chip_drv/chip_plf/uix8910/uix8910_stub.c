/******************************************************************************
 ** File Name:      sci_stub.c                                                 *
 ** Author:         guangqiao.she                                              *
 ** DATE:           18/10/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the stub funcation for UIS8910C porting    *
 ******************************************************************************

******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/18/2001     Richard.Yang     Create.                                   *
 ** 12/24/2001     Lin.liu          Add.                                      *
 **                                    Func:                                  *
 **                                         es_log_set_filter(...);           *
 **                                         es_log_get_filter_info(...);      *
 **                                    Struct and variables:                  *
 **                                         uint8 log_filter[];               *
 **                                         es_log_filter_struct;             *
 ** 06/03/2002     Xueliang.Wang    Modify according to new standard.         *
 ** 06/19/2002     Xueliang.Wang    Modify time stamp of log. It is the time  *
 **                                    since MS last power on.                *
 **                                    use SCI_GetTickCount()                 *
 **                                    instead of SCI_GetCurrentTime()        *
 ** 02/20/2003      Xueliang.Wang   Use SCI_LogMsg instead SCI_LogReceivedMsg *
 **                                     and SCI_LogSendedMsg                  *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include <string.h>
//#include <stdio.h>
#include <stdarg.h>

#include "os_api.h"
#include "os_apiext.h"
#include "sci_log.h"
#include "uix8910_stub.h"
#include "nv_item_id.h"
#include "nvitem.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*lint -save -e666 -esym(551, log_info) -e628 !e64 !e718 -esym(628, __va_start)*/
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/****************************************************************************
* referred from UIX8910_uix8910_version.o
****************************************************************************/
//const uint16  s_calibrationparam_version = 0xff0a;
//const uint16  s_downloadparam_version    = 0xff05; // for cr122466

/****************************************************************************
* referred from mnsms_signal.o
****************************************************************************/
BLOCK_ID        g_P_CC_ID[MULTI_SYS_NUM];
BLOCK_ID       g_P_SMR_ID[MULTI_SYS_NUM];

/****************************************************************************
* referred from simat_api.o
****************************************************************************/
BLOCK_ID       g_P_SIM_ID[MULTI_SYS_NUM];

/****************************************************************************
* referred from mnss_signal.o
****************************************************************************/
BLOCK_ID        g_P_SS_ID[MULTI_SYS_NUM]; /*lint -esym(552,g_P_SS_ID) */


/****************************************************************************
* referred from simat_envelope.o
****************************************************************************/
 uint32   g_sim_simat_task_id[MULTI_SYS_NUM];

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    referred from prod_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/

PUBLIC void L1API_RegisterRFResetFunc(L1_RFRESET_REG_CALLBACK func)
{
   return;
}


/*****************************************************************************/
//  Description:   referred from prod_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void REF_GetAdcCalibrationPara( 
							   uint32 battery_calibration[2],
							   uint32 program_calibration[2],
							   uint32 invalid_calibration,
							   uint32 chr_pm_ver_support
							   )
{
	battery_calibration[0] = 0xDB01068;
	battery_calibration[1] = 0xBC00E10;

	program_calibration[0] = 0;
	program_calibration[1] = 0;

#if 0
    calibration_param_T  * calibration_ptr = (calibration_param_T *)NVITEM_GetCalibratedParamAddr();
    
    // Set default value
    battery_calibration[0] = invalid_calibration;
    battery_calibration[1] = invalid_calibration;
    
    program_calibration[0] = invalid_calibration;
    program_calibration[1] = invalid_calibration;    
    
    if( (NULL != calibration_ptr) && (calibration_ptr->PM_version >= chr_pm_ver_support) )
    {
        // Yes, this version support ADC calibration.
        ADC_T  * adc_ptr = &(calibration_ptr->adc);
        
        if( (0x7FFFFFFF > adc_ptr->battery[0]) ||  
            (0x7FFFFFFF > adc_ptr->battery[1]) )
        {
            battery_calibration[0] = adc_ptr->battery[0];
            battery_calibration[1] = adc_ptr->battery[1];
        }
        if( (0x7FFFFFFF > adc_ptr->adc[0]) ||  
            (0x7FFFFFFF > adc_ptr->adc[1]) )
        {                
            program_calibration[0] = adc_ptr->adc[0];
            program_calibration[1] = adc_ptr->adc[1];
        }
    }    
#endif

}

/*****************************************************************************/
//  Description:   referred from prod_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 REF_GetMarkerValue(void)
{
//return 0;
#if 1
	uint32 value = 0;
	calibration_param_T *s_cali_nv_param_buf_ptr = PNULL;
	s_cali_nv_param_buf_ptr = (calibration_param_T *)SCI_ALLOC_BASE(sizeof(calibration_param_T));
	if(PNULL != s_cali_nv_param_buf_ptr)
	{
	  (void)EFS_NvitemRead(NV_L1_CALIBRATION_PARAMETER, sizeof(calibration_param_T), (uint8 *)s_cali_nv_param_buf_ptr);
	  value = s_cali_nv_param_buf_ptr->adc.reserved[7];
	  
	  SCI_FREE(s_cali_nv_param_buf_ptr);
	  s_cali_nv_param_buf_ptr = PNULL;
	}
	return	 value ; 
#endif
}

/*****************************************************************************/
//  Description:   referred from audio_config.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_CfgAmrParam( AMR_MODE amr_mode, uint8 channels, uint8 sample_rate )
{
return;
}


/*****************************************************************************/
//  Description:   referred from audio_config.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 LAYER1_EnableVoiceLoopback(
    BOOLEAN is_enable,             //0: disable  1:enable
    uint16  usLoopbackType,      // 
    uint16  usVoiceFormat,
    uint16  usReserved            // millisecones
    )
{
return SCI_FALSE;

}

/*****************************************************************************/
//  Description:   referred from audio_config.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 LAYER1_GetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
                                                   // else return SCI_ERROR
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,     
    AUDIO_TEST_DATA_T   *param_data				 
    )
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from audio_config.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
                                                   // else return SCI_ERROR
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,     
    AUDIO_TEST_DATA_T   param_data				 
    )
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from audio_config.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchTestMode(
	BOOLEAN is_test_mode
	)    
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from ref_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void REF_GetCalibrationPara_Adc(uint32  *adc_ptr,uint32 len)
{}

/*****************************************************************************/
//  Description:   referred from ref_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN  REF_SetCalibrationPara_Adc(uint32  *adc_ptr, uint32 len)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from ref_param.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN REF_WriteMarkerValue(uint32 value)
{
//return SCI_FALSE;
     BOOLEAN ret=SCI_TRUE;
  	 
  	 calibration_param_T *s_cali_nv_param_buf_ptr = PNULL;
	 s_cali_nv_param_buf_ptr = (calibration_param_T *)SCI_ALLOC_BASE(sizeof(calibration_param_T));

#ifndef WIN32
    if((PNULL == s_cali_nv_param_buf_ptr)||(NVERR_NONE != EFS_NvitemRead(NV_L1_CALIBRATION_PARAMETER,sizeof(calibration_param_T), (uint8 *)s_cali_nv_param_buf_ptr)))
    {
        if(PNULL != s_cali_nv_param_buf_ptr)
        {
            SCI_FREE(s_cali_nv_param_buf_ptr);
            s_cali_nv_param_buf_ptr = PNULL;
            return SCI_FALSE;	    		
        }
    }
#endif

    // fixed pclint 613. @arthur
    if(PNULL != s_cali_nv_param_buf_ptr)
    {
        s_cali_nv_param_buf_ptr->adc.reserved[7] = value;

#ifndef WIN32
        if(NVERR_NONE != NVITEM_UpdateCali(NV_L1_CALIBRATION_PARAMETER, sizeof(calibration_param_T),s_cali_nv_param_buf_ptr))
        //if(NVERR_NONE != EFS_NvitemWrite(NV_L1_CALIBRATION_PARAMETER, sizeof(calibration_param_T),(uint8*)s_cali_nv_param_buf_ptr,FALSE))
        {
            ret =  SCI_FALSE;
        }        
#endif

        SCI_FREE(s_cali_nv_param_buf_ptr);
    }
	
	
	return ret;

}

/*****************************************************************************/
//  Description:   referred from imei.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 * REF_SetMemCalibrationPara_Adc(uint32  *adc_ptr, uint32 len, uint32 * cali_len_ptr)
{
return NULL;
}

/*****************************************************************************/
//  Description:   referred from production_test_current.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_DSPCloseAllDevice() 
{}


/*****************************************************************************/
//  Description:   referred from production_test_current.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_DSPMaxPowerTX( BOOLEAN is_enable )
{}

/*****************************************************************************/
//  Description:   referred from production_test_current.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_DSPMinPowerRX(BOOLEAN is_enable, uint32 ms_band)
{}


/*****************************************************************************/
//  Description:   referred from mneng_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void GetCellInfoFromPs(
    DIAG_CELL_INFO_LOG_T *cell_info // Output: cell information sent to Monitoring tooL
)
{}

/*****************************************************************************/
//  Description:   referred from mneng_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void PS_CancelForceCampon(
    void
)
{
    return ;
}

/*****************************************************************************/
//  Description:   referred from mneng_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void PS_ForceCampon(
    uint16 sim_no,
    uint16 arfcn_count,  ///1 -- 6
    uint16 *arfcn_list
)
{}


/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN MM_SetdisablePowerSweep(BOOLEAN is_disable)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRC_GetCellBasicInfo(
                          uint32 dual_sys,
                          DIAG_CELL_INFO_T *scell_info,
                          DIAG_CELL_INFO_T *ncell_info
                          )
 {}
                     

/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRC_GetCellInfoPtr( 
                               uint32 dual_sys,
                               CELL_INFO_T ** cell_arr_ptr, //output cell information array address 
                               int16 ** scell_index_ptr     //output serving cell index of cell array
                               )
{
	return ;
}

/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
int32 RRC_GetTimesOfNoCellAvailable(uint32 dual_sys)
{
	return 0;
}


/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
int32 RRC_GetTimesOfPowerSweep(uint32 dual_sys)
{
	return 0;
}

/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRPROC_Set_disablePwr(BOOLEAN disable_Pwr)
{
return;
}


/*****************************************************************************/

/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRPROC_Set_identifyBSMode(BOOLEAN identify_mode)
{
return;
}


/*****************************************************************************/
//  Description:   referred from mn_main.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void SCI_SaveBaseMsg(
            xSignalHeaderRec *sig_ptr,
            uint32           comm_ref
            )
{
    return ;
}




/*****************************************************************************/
//  Description:   referred from mmieng_win.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L1API_GetBlockErrorRatio(uint32 task_id)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mmieng_win.o mneng_api
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_GetLayer1MonItemPtr(
    LAYER1_MONITOR_ITEM_T **item_ptr //[OUT] monitor data ptr
    )
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mmieng_win.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRA_GetRraMonItem(
        RRA_MONITOR_ITEM_T *item_ptr
        )
 {
return ;
}       

/*****************************************************************************/
//  Description:   referred from fm_drv.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 LAYER1_RxADCOnOff(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN     		 flag            // If uplink this tone.(SCI_TRUE: Uplink)
    )
{
return 0 ;
}

/*****************************************************************************/
//  Description:   referred from dspvb_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 uint32 LAYER1_PreSetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               )
  {
return 0 ;
}            

/*****************************************************************************/
//  Description:   referred from dspvb_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 uint32 LAYER1_SetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               )
{
return 0 ;
}

/*****************************************************************************/
//  Description:   referred from dspvb_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_SetDevVolume(uint16 volume_level)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from dspvb_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_SetDlnkMute(BOOLEAN is_enable,uint16 dlnk_mute_type)
{
return ;
}


/*****************************************************************************/
//  Description:   referred from bkmusic_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_SetMagicVoiceParam( uint16  type,
                                uint16  uplink_value, 
                                uint16  downlink_value) 
{
return ;
}


/*****************************************************************************/
//  Description:   referred from bkmusic_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StartMagicVoiceFunc(MAG_CALLBACK       callback_fun,
                                uint16              type,     //used to indicate which direction is valid.
                                uint16	            uplink_value,
                                uint16              downlink_value)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from bkmusic_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StopMagicVoiceFunc() 
{
return ;
}

/*****************************************************************************/
//  Description:   referred from bkmusic_as.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_WriteVoiceData (uint16 *source_buf_ptr,uint16 data_length)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from autotest_audio.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{
return ;
}

/*****************************************************************************/
//  Description:   referred from admm.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_SetDevModeParam(
	    uint16  *param_buffer_ptr,//buffer where stores the parameter of one mode
	    uint32 length           //byte uint
	    )
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void GMM_GetGU( 
                               uint32                dual_sys,
                               GMM_STATE_UPDATED     **gu_ptr
                               )
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void GMM_GetTLLICur( 
                               uint32     dual_sys,
                               TLLI_T     **tlli_cur_ptr
                               )
{
return ;
}



/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L1API_GetPagingWakeUpReport(void)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 BOOLEAN L1API_GetTBFSlotMask(
    uint8* ul_slot_ptr, //[OUT] uplink slot mask pointer
    uint8* dl_slot_ptr  //[OUT] downlink slot mask pointer
    )
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_SetPagingWakeUpReport(BOOLEAN flag)
{
return ;
}	



/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN LLCENG_IsDataCiphered(void)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN PS_GetPagingWakeUpReport(void)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void PS_SetPagingWakeUpReport(BOOLEAN flag)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRC_GetPowerSweepInfoPtr( 
                               uint32 dual_sys,
                               ARFCN_POWERLEVEL_T ** power_sweep_result,
                               int16 ** arfcn_num
                               )
{
return ;
}                              


/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRC_GetRrResPtr(uint32 dual_sys, RES_PARAM_T **rr_res_ptr)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
RR_STATUS_INFO_T* RR_GetStatusInfo(uint32 dual_sys)
{
return NULL;
}


/*****************************************************************************/
//  Description:   referred from mneng_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void SM_GetNsapiNumPtr( 
                               uint32       dual_sys,
                               int16        *nsapi_num_ptr
                               )
{
return ;
}



/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
ENG_GMM_STATUS_INFO_T* ENG_GMM_GetStatusInfo(uint32 dual_sys)
{
return NULL;
}


/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInAvailablePlmnlist(
    PLMN_T      plmn,     // the special plmn to be found
    AVAILABLE_PLMN_LA_LIST_T *list_ptr // pointer the plmn list
)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInFplmslist(
					PLMN_T            plmn,     // the special PLMN to be found
					FPLMN_LIST_T      *list_ptr // pointer to the fplmn list
					)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInLongFplmnlist(
    const PLMN_T            plmn,     // the special PLMN to be found
    const FPLMN_LONG_LIST_T *list_ptr // pointer to the long fplmn list
)
{
return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 BOOLEAN L3FUNC_UpdateFplmnList( // true means update ok,false means no operate
    BOOLEAN         del_flag, // false means add, true means delete
    PLMN_T          plmn,     // specified item be added or be deleted
    FPLMN_LIST_T    *list_ptr // forbidden plmn list
)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnphone_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_UpdateLongFplmnList( // true means update ok,false means no operate
    BOOLEAN           del_flag, // false means add, true means delete
    PLMN_T            plmn,     // specified item be added or be deleted
    FPLMN_LONG_LIST_T *list_ptr // long forbidden plmn list
)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnss_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 PS_TransStrToBCD(
                         char  *bcd_str_ptr,    // in: bcd string
                         uint8 *bcd_data_ptr,   // in/out: buffer for bcd data
                         int16 *bcd_data_len    // in: max bytes of the buffer for bcd data
                                                // out: bytes indicate the length of bcd data
                         )
{
    int16 max_bcd_data_len = *bcd_data_len;

    if((PNULL == bcd_data_ptr) || (PNULL == bcd_str_ptr))
    {
        return SCI_PTR_ERROR;
    }

    *bcd_data_len = 0;

    while(*bcd_str_ptr != '\0')
    {
        *bcd_data_len += 1;

        if(*bcd_data_len > max_bcd_data_len)
        {
            return SCI_SIZE_ERROR;
        }

        switch(*bcd_str_ptr)
        {
            case '*':
                *bcd_data_ptr = 0x0A;
                break;
            case '#':
                *bcd_data_ptr = 0x0B;
                break;
            case 'p':
            case 'P':
                *bcd_data_ptr = 0x0C;
                break;
            case 'w':
            case 'W':
                *bcd_data_ptr = 0x0D;
                break;
            case '+':
                *bcd_data_ptr = 0x0E;
                break;
            default:

                if(*bcd_str_ptr >= '0' && *bcd_str_ptr <= '9')
                {
                    *bcd_data_ptr = (uint8)(*bcd_str_ptr - '0');
                }
                else
                {
                    return SCI_ERROR;
                }

                break;
        }

        bcd_str_ptr++;

        if(*bcd_str_ptr == '\0')
        {
            *bcd_data_ptr += 0xF0;
        }
        else
        {
            switch(*bcd_str_ptr)
            {
                case '*':
                    *bcd_data_ptr += 0xA0;
                    break;
                case '#':
                    *bcd_data_ptr += 0xB0;
                    break;
                case 'p':
                case 'P':
                    *bcd_data_ptr += 0xC0;
                    break;
                case 'w':
                case 'W':
                    *bcd_data_ptr += 0xD0;
                    break;
                case '+':
                    *bcd_data_ptr += 0xE0;
                    break;
                default:

                    if((*bcd_str_ptr >= '0') && (*bcd_str_ptr <= '9'))
                    {
                        *bcd_data_ptr += (*bcd_str_ptr - '0') << 4;
                    }
                    else
                    {
                        return SCI_ERROR;
                    }

                    break;
            }

            bcd_str_ptr++;
        }

        bcd_data_ptr++;
    }

    return SCI_SUCCESS;
}

#if 0

/*****************************************************************************/
//  Description:   referred from simat_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN SIM_IsCallControlSupport(uint8 cur_sim_task)
{
    return SCI_FALSE;
}

#endif
/*****************************************************************************/
//  Description:   referred from mnnv_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN REF_GetIsCalibrationedFlag(void)
{
    return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mnnv_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN REF_WriteCaliCDACValue(uint32 value)
{
    return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from mngprs_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
int32 SM_CompareQosValid(                           //RETURN:
                         TOTAL_QOS_T    *qos1_ptr,  //IN:
                         TOTAL_QOS_T    *qos2_ptr   //IN:
                         )
{
    return 0;
}


/*****************************************************************************/
//  Description:   referred from mngprs_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void SM_SetSubscribeQosValue(
                             TOTAL_QOS_T* qos_ptr
                             )
{
    return ;
}                             


/*****************************************************************************/
//  Description:   referred from mngprs_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN RLC_IsGPRSActive(void)
{
    return SCI_FALSE;
}	


/*****************************************************************************/
//  Description:   referred from mngprs_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 void RRPROC_SetTbfPaging(
    uint16  dual_sys,      //in -- system index, 0 or 1
    BOOLEAN tbf_paging_on  //in -- 
)
{
    return ;
}

/*****************************************************************************/
//  Description:   referred from mngprs_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN SM_SetQos(
                  BOOLEAN         is_set_qmin,    //IN: true, is set min qos
                  TOTAL_QOS_T     *qos_ptr        //IN/OUT:
                  )
{
    return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from wav_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_AUDIO_CODEC_CopyData(
    DSP_CODEC_DATA_T *data_ptr
    )
 {
    return ;
}   


/*****************************************************************************/
//  Description:   referred from wav_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_AUDIO_CODEC_CopyLRData(
    DSP_CODEC_DATA_LR_T *data_ptr
    )
{
    return ;
}

/*****************************************************************************/
//  Description:   referred from wav_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_AUDIO_CODEC_GetAddressBase(
    AUDIO_CODEC_ARM_DSP_ADDRESS_MAP_T * const address_base_ptr
    )
{
    return ;
}


/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L1API_GetGsmRXPwrResult(
                        uint16 arfcn,
                        uint8  band,
                        int16* result)
{
    return SCI_FALSE;
}

/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_GsmRXCWPwrTest( BOOLEAN is_debug_on, 
                        uint16 arfcn,
                        uint8 band,
                        uint16 gain)
{
    return ;
}


/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_GsmTxCWPwrTest( BOOLEAN is_debug_on, 
                         uint16 arfcn,
                         uint8 band,
                         uint16 gainorpower
                        )
{
    return ;
}


/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1API_ReadWriteRfRegister(uint16 p0, uint16 p1, uint16 p2, uint16 p3)
{
    return ;
}

/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
L1_RW_RF_REG_RESULT_E L1API_ReadWriteRfRegisterResult(uint16 p0,uint16 *reg_val)
{
    return 0;
}


/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 LAYER1_SetVolume( // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    uint16  speaker_vol // Volume value of speaker(0 - DSP_VOL_SPEAKER_MAX)
    )
{
	return SCI_SUCCESS;

}

/*****************************************************************************/
//  Description:   referred from atc_basic_cmd.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void PS_GetRRCellsInfoBsicAndEx(
	uint16 dual_sys, 
	RR_CELLS_INFO_T *rr_cells_info_bsic_ptr, 
	RR_CELLS_INFO_TRAFFIC_T *rr_cells_info_bsic_traffic_ptr, 
	RR_CELLS_INFO_EX_T *rr_cells_info_ex_ptr,
	BOOLEAN *is_traffic_ptr)
{
	return ;

}


/*****************************************************************************/
//  Description:   referred from uplink_nxp_codec_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StartPlay( AUD_REC_FORMAT_E voice_format,
					           PLAY_CALLBACK callback_fun,
				               BOOLEAN is_to_speaker,
				               BOOLEAN is_to_net
	                          )
{
	return ;

}

/*****************************************************************************/
//  Description:   referred from uplink_nxp_codec_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StopPlay( void )
{
	return ;

}


/*****************************************************************************/
//  Description:   referred from uplink_nxp_codec_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void SIMSIGNAL_SIMDphoneInitReq(uint32 cur_sim_task, BOOLEAN is_entire_service)
{
	return ;

}


/*****************************************************************************/
//  Description:   referred from dsp_source_ex.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StartRecord( AUD_REC_FORMAT_E voice_format,
					    RECORD_CALLBACK callback_fun,
		                BOOLEAN is_from_mic,
		                BOOLEAN is_from_net
			   			)
{
	return ;

}

/*****************************************************************************/
//  Description:   referred from dsp_source_ex.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StopRecord( void )
{
	return ;

}


/*****************************************************************************/
//  Description:   referred from downlink_nxp_codec_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 LAYER1_DownlinkMuteEnable (
    	BOOLEAN mute_enable,
    	uint16  mute_type
)
{
	return 0;

}


/*****************************************************************************/
//  Description:   referred from downlink_nxp_codec_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 LAYER1_MuteSwitch(    // If succeed, return SCI_SUCCESS,
                                    // else return SCI_ERROR
    BOOLEAN b_mute,                 // SCI_TRUE: Mute ON
    uint16  mute_type               // add for bluetooth mode 
    )
{
	return 0;

}


/*****************************************************************************/
//  Description:   referred from simat_do_decode.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
SIM_FILE_NAME_E SIMFS_GetFileName(
    uint32 cur_sim_task,
    uint16 file_id,  //the file id
    uint8  path_id_len,
    uint8  *path_id)
{
	return 0;

}

/*****************************************************************************/
//  Description:   referred from simat_do_decode.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
SIM_FILE_TYPE_E SIMFS_GetFileType(  //return value:the file's file type
    SIM_FILE_NAME_E file_name   //the file name
)
{
	return 0;

}


/*****************************************************************************/
//  Description:   referred from simat_do_decode.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
int8 SIMFS_FileRouting( //reutrn value:the number of file in the routing to the destionation file
    SIM_FILE_NAME_E file_name,  //the desitination file
    SIM_FILE_NAME_E cur_file,   //the current selected direction file
    SIM_FILE_NAME_E *file_route //the file route to the destionation file
)
{
	return 0;

}

/*****************************************************************************/
//  Description:   referred from simat_do_decode.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint16  SIMFS_GetFileID(    //return value:the file ID
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name     //the file name
)
{
	return 0;

}


/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 void LAYER1_SendVoiceDataToDA(uint16 *data_ptr)
{
}

/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_SendVoiceDataToNet(uint16 *data_ptr)
{
}

/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StartRecord_Ad(RECORD_CALLBACK2 callback)
{
}


/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StartRecord_Downlink(RECORD_CALLBACK2 callback)
{
}


/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 void LAYER1_StopRecord_Ad(void)
{
}


/*****************************************************************************/
//  Description:   referred from voiceprocess_adp.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_StopRecord_Downlink(void)
{
}


/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1_AAC_DecodeFrame(//return none
    uint8* uc_frame_ptr, //pointer of input frame data
    uint16 us_frame_len, //length of input frame data
    AAC_LC_FRAME_IND_CALLBACK_PFUNC callback_func_ptr//pointer of callback which
                //is used to inform codec the message that decode frame is over
    )
{
}

/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1_AAC_DecodeHeader(  //return none
    uint8* uc_header_ptr,   //pointer of input header data
    uint16 us_header_len,   //length of input header data
    AAC_LC_HEADER_IND_CALLBACK_PFUNC callback_func_ptr//pointer of callback which 
                  //is used to inform codec the message that decode header is over
    )
{
}

/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void L1_AAC_PcmDataCopy(
    uint16 *us_dest_l_ptr, //pointer of dest left pcm data
    uint16 *us_dest_r_ptr, //pointer of dest right pcm data
    uint32 ui_len
    )
{
}    


/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L1_SBC_Config_Encoder  (uint8  channel_mode, 
                             uint8  alloc_method,
                             uint16 sample_freq, 
                             uint8  nrof_blocks, 
                             uint8  nrof_subbands, 
                             uint8  bitpool,
                             SBC_CONFIG_ENCODER_IND_CALLBACK_PFUNC callback_func_ptr)
{
return SCI_FALSE;
}                             

/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN L1_SBC_Encode_Frame(int16* ps_left_pcm, int16* ps_right_pcm, uint16 us_pcm_len, uint8* puc_frame, uint16 us_frame_len, uint8 bitpool, uint16 us_frame_size, SBC_ENCODE_FRAME_IND_CALLBACK_PFUNC callback_func_ptr)
{
return SCI_FALSE;
}


/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_TRANPCM(uint16 srcAddr, uint16 scrSampleRate, uint16 sampleNum, void * callback)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
 void LAYER1_TRANPCM_Config(uint16 samplerate)
{
return ;
}


/*****************************************************************************/
//  Description:   referred from aud_dsp_mng.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void Layer1_Audio_Register(
	LAYER1_AUDIO_CALLBACK callback_fun
)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from mp3_dsp_layer1_cmd_interface.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void Layer1_Cmd_Audio(Audio_CmdQ_T *cmd_msg)
{
return ;
}

/*****************************************************************************/
//  Description:   referred from amr_dsp_layer1_cmd_interface.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_AUDIO_CODEC_Data(//return none
    DSP_CODEC_SRC_DATA_T * data_info
    )
{
return ;
}


/*****************************************************************************/
//  Description:   referred from layer1_diag.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint16 CAL_GetCalibrationParamSize(CALIBRATION_PARAM_E  param_type)
{
return 0;
}

/*****************************************************************************/
//  Description:   referred from mnphone_api.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void RRPROC_Set_identifyBS(BOOLEAN start_identify)
{
return ;
}


/*****************************************************************************/
//  Description:   referred from mmieng_win.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
void LAYER1_GetLayer1MonItem(
    LAYER1_MONITOR_ITEM_T* item_ptr //[OUT] monitor data
    )
{
return ;
}


/*****************************************************************************/
//  Description:   referred from mnss_module.o
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
uint32 PS_TransBCDToStr(
                         uint8  *bcd_data_ptr,  // in: bcd data
                         int16  bcd_data_len,   // in: length of bcd data
                         char   *bcd_str_ptr,   // in/out: buffer for bcd string
                         int16  max_bcd_str_len // in: max bytes of the buffer for bcd string
                         )
{
    int16 i, j;
    uint8 semi_octet = 0;

    if((PNULL == bcd_data_ptr) || (PNULL == bcd_str_ptr))
    {
        return SCI_PTR_ERROR;
    }

    if(max_bcd_str_len - 1 < bcd_data_len * 2)
    {
        return SCI_SIZE_ERROR;
    }

    for(i = 0; i < bcd_data_len; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if(0 == j)
            {
                semi_octet = (uint8)(*bcd_data_ptr & 0x0f);
            }
            else
            {
                semi_octet = ((*bcd_data_ptr & 0xf0) >> 4);
            }

            switch(semi_octet)
            {
                case 10:
                    *bcd_str_ptr++ = '*';
                    break;
                case 11:
                    *bcd_str_ptr++ = '#';
                    break;
                case 12:
                    *bcd_str_ptr++ = 'p';
                    break;
                case 13:
                    *bcd_str_ptr++ = 'w';
                    break;
                case 14:
                    *bcd_str_ptr++ = '+';
                    break;
                case 15:
                    break;
                default:
                    *bcd_str_ptr++ = (uint8)(semi_octet + '0');
                    break;
            }
        }

        bcd_data_ptr++;
    }

    *bcd_str_ptr = '\0';

    return SCI_SUCCESS;
}
/*lint -restore*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*  End Of File */
