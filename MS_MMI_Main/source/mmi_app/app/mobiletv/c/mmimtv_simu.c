/*****************************************************************************
** File Name:      mmimtv_simu.c                                             *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv ref simultor.           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

 
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef CMMB_SUPPORT
#ifdef WIN32
#include "mtv_api.h"
#include "mbbms_service.h"

#ifdef MBBMS_SUPPORT
#include "mbbms_service.h"
#include "mbbms_sg_api.h"
#endif

#include "os_api.h"
#include "mmimtv_simu.h"
#include "mmimtv_main.h"
#include "mmimtv_data.h"

#include "lcd_tv.h"
#include "guilcd.h"
#include "block_mem.h"
#include "guiref_scale.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL uint32 s_language_code = MMIMTV_CHINESE_CODE; 
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL uint32 volume_value = 0;

typedef void (* MTV_CALLBACK) (
    KDuint32 ack_id, 
    KDuint32 para0,
    KDuint32 para1,
    KDuint32 para2
);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : send signal to MMI
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MTV_SendSignaltoMMI(MTV_MIDDLEWARE_SIGNALCODE_E msg);


/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MTV_STATE_UNINITIALIZED,
    MTV_STATE_IDLE,
    MTV_STATE_WAITTUNERDISABLEACK,
    MTV_STATE_PLAYING,
    MTV_STATE_PAUSED,
    MTV_STATE_WAITSTOPACK,
    MTV_STATE_WAITSWITCHOK,
    MTV_STATE_WAITSTOPPLAYBACKACK,
    MTV_STATE_WAITEXITDISABLEFILTERACK,
    MTV_STATE_WAITPLAYPARAM,
    MTV_STATE_LAST
} MTV_STATE_E;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 MTV_Initialize(MTV_MODE_E mode, MTV_CALLBACK callback, MTV_CALLBACK dac_callback)
{
    SCI_Sleep(150);
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MTV_Cleanup()
{
    return;
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void LCDTV_SetLCDSpec(LCDTV_SPEC_T *spec_ptr)
{
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void LCDTV_SetOSDSpec(LCDTV_OSD_SPEC_T *o_spec_ptr)
{
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void LCDTV_SetVideoSpec(LCDTV_VIDEO_SPEC_T *v_spec_ptr)
{
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDTV_Invalidate(LCD_ID_E lcd_id)
{
    GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
    return ERR_LCD_NONE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void LCDTV_EnableLCDSpec(BOOLEAN is_enable)
{
    return;
}

/***********************************************************************/
// 	Description : stop searching ESG
//	Global resource dependence : s_esg_state, s_notify_esg_state
//  Author: robert lu
//	Note:
/***********************************************************************/
void CMMB_StopSearchESGData(void)
{
}

/***********************************************************************/
// 	Description : This function is used to stop searching program. 
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
void  CMMB_StopSearchProgram(void)
{
    return;
}

/***********************************************************************/
// 	Description : whether the hint is displayed.
//	Global resource dependence : 
//  Author: robert lu
//	Note:
/***********************************************************************/
PUBLIC BOOLEAN  MTVSTIMI_SetProgHintInfo (BOOLEAN is_display,
                                          ESG_PROG_HINT_LANG_E lang
                                          )
{
    return SCI_TRUE;
}

/***********************************************************************/
// 	Description :free memory,which emegency broadcast allocated
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
void CMMB_FreeBroadcastMsg(
                           MTV_STIMI_BROADCAST_U *bc_ptr
                           )
{
    return;
}

/***********************************************************************/
// 	Description : This fuction is used to get all service's under appointed network.
//	Global resource dependence : 
//   Author:
//	Note:
/***********************************************************************/
BOOLEAN  MTVSTIMI_GetNetworkServiceTab(
                                       uint16    network_id,     //  network id
                                       MTV_STIMI_NETWORK_SERVICE_TAB_T * network_service_tab_ptr//out
                                       )
{
    return SCI_TRUE;
}

/***********************************************************************/
// 	Description : This function is used to free memory ,which network
//                service table allocated
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MTVSTIMI_FreeNetworkServiceTab (
                                     MTV_STIMI_NETWORK_SERVICE_TAB_T * network_service_tab_ptr
                                     )
{
    return;
}

/***********************************************************************/
// 	Description : begin searching ESG
//	Global resource dependence : s_esg_state, s_notify_esg_state
//  Author: robert lu
//	Note: MTV_STIMI_LACKOF_CONTROL_INFO, MTV_STIMI_FILE_SYSTEM_ERR, MTV_STIMI_NO_SPACE
//            MTV_STIMI_SEARCHING_ESG,  MTV_STIMI_ERR_NONE
/***********************************************************************/
PUBLIC uint32  CMMB_SearchESGData(uint16 net_id)
{
    MTV_SendSignaltoMMI(MTV_STIMI_SEARCH_ESG_END_CNF);
    
    return MTV_STIMI_ERR_NONE;
}

/***********************************************************************/
// 	Description : This fuction is used to get all network information
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
BOOLEAN  MTVSTIMI_GetNetworkInfoTab(
                                    MTV_STIMI_NET_INFO_TAB_T * network_info_tab_ptr
                                    )
{
    return SCI_TRUE;
}

/***********************************************************************/
// 	Description : This function is used to free memory ,which network
//                information table allocated.
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MTVSTIMI_FreeNetworkInfoTab(
                                 MTV_STIMI_NET_INFO_TAB_T * network_info_tab_ptr
                                 )
{
    return;
}





//private

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_Mute(void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_UnMute(void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_SetVolume(KDuint32 dsp_volume, KDuint32 dac_volume)
{
    volume_value = dac_volume;
    return SCI_SUCCESS;
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void CMMB_Init(uint32 ca_type)
{
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
void CMMB_Cleanup (void)
{
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
void MTV_Close (void)
{
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_PlayNetwork(KDuint32 net_eid, KDuint32 service_id, KDuint32 audio_language)
{
    MTV_SendSignaltoMMI(MTV_PLAY_ACK);
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_PlayNetworkByPlayParam(void *playparam_ptr,
                                          uint32 audio_language,
                                          uint32 sub_language)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_PlayStream(const KDuint16 *file_name_ptr, KDuint32 audio_language)
{
    MTV_SendSignaltoMMI(MTV_PLAY_ACK);
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_Abort(void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_Pause(void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_Resume(void)
{
    return 0;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_GetStreamLength(const KDuint16 *file_name_ptr)
{
    // 01:08:15
    return 0xFFF;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_SetStreamPos(KDuint32 pos)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_EnableRecord(MTV_ENABLE_RECORD_E param,
                                const uint16 *file_name_ptr)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_AbortRecord(void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_IdentifyStream (const KDuint16 *file_name_ptr)
{
    return SCI_TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
KDuint32 CMMB_EnumAudioLanguages(KDuint32 *language_code)
{
    if(PNULL != language_code)
    {
        language_code[0] = MMIMTV_CHINESE_CODE;
        language_code[1] = MMIMTV_ENGLISH_CODE;
        language_code[2] = 10;
        language_code[3] = 0;
    }
    return 4;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_GetAudioLanguage(void)
{
    return s_language_code;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void CMMB_SetAudioLanguage(uint32 language_code)
{
    s_language_code = language_code;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_EnumSubLanguages(uint32 *language_code)
{
    return 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_GetSubLanguage(void)
{
    return 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void CMMB_SetSubLanguage(uint32 language_code)
{
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_FillBuffer(uint32* ts_buf_ptr,
                              uint32 *desp_buf_ptr)
{
    return 0;
}


/*****************************************************************************/
// 	Description : send signal to MMI
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MTV_SendSignaltoMMI(MTV_MIDDLEWARE_SIGNALCODE_E msg)
{
#if 0
    MTV_STIMI_SIG_T *sig_ptr = SCI_NULL;
    
    sig_ptr = (MTV_STIMI_SIG_T *)SCI_ALLOC(sizeof(MTV_STIMI_SIG_T));
    if(PNULL == sig_ptr)
    {
        return;
    }
    
    /* Init the signal.*/
    sig_ptr->SignalCode = msg;
    sig_ptr->SignalSize = sizeof(sig_ptr);
    sig_ptr->Sender = SCI_IdentifyThread();
    
    SCI_SendSignal((xSignalHeader)sig_ptr,  P_APP);
#endif
}

/*****************************************************************************/
// 	Description : Description: This function is used to free memory ,which network
//                information table allocated.
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
void CMMB_FreeNetworkInfoList(MTV_STIMI_NET_INFO_LIST_T * network_info_list_ptr)
{
    if (network_info_list_ptr->network_number > 0)
    {
        SCI_FREE(network_info_list_ptr->network_info_ptr);
    }
}
/*****************************************************************************/
// 	Description : init for create dtlsvc task
//	Global resource dependence : none
//  Author: 
//	Note:  for static task
/*****************************************************************************/
PUBLIC void CMMB_Task_Init(void)
{
}

/*****************************************************************************/
// Description : 释放MTV_STRING_T数据
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
void MMIMTV_FreeString(MTV_STRING_T  *p_str)
{
    if (p_str)
    {
        p_str->encode = MTV_ENCODE_UNKNOWN;
        p_str->length = 0;
        if (p_str->str_ptr)
        {
            SCI_FREE((void *)p_str->str_ptr);
            p_str->str_ptr = PNULL;
        }
    }
    return;
}

/*****************************************************************************/
// Description : 释放节目数据
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_FreeProg(MMIMTV_PROGRAMME_T  *prog_ptr)
{
    //SCI_TRACE_LOW:"mtvesg_common.c.c:MMIMTV_FreeProg()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_SIMU_542_112_2_18_2_40_51_885,(uint8*)"");
    if(PNULL == prog_ptr)
    {
        return;
    }
    
    // 附属信息清零
    prog_ptr->lang = LANG_UNKNOWN;
    prog_ptr->Duration = 0;
    SCI_MEMSET(&prog_ptr->StartTime, 0, sizeof(TIMEPOINT_T));
    // 释放节目信息
    MMIMTV_FreeString(&(prog_ptr->Title));
    MMIMTV_FreeString(&(prog_ptr->detail.Title));
    MMIMTV_FreeString(&(prog_ptr->detail.Digestinfo));
    MMIMTV_FreeString(&(prog_ptr->detail.AudioLang));
    MMIMTV_FreeString(&(prog_ptr->detail.SubtitleLang));
    return;
}

/*****************************************************************************/
// Description : 获取指定服务及日期的所有节目列表
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
KDuint32 CMMB_GetAllProg(KDuint16           network_id,
                         KDuint16           service_id,
                         MTV_DATE_T         date,
                         MMIMTV_SERVICE_T  *proglist_ptr)
{
    ESG_ERROR_E        ret = ESG_SUCCESS;
    uint8 str[10] = "hello";
    uint8 str2[10] = "spread";
    uint8 str_service[10] = "hbtv";
    
    if(PNULL == proglist_ptr)
    {
        return ret;
    }
    
    proglist_ptr->NumOfProg = 3;
    proglist_ptr->ProgTotal = 4;
    proglist_ptr->prog_ptr = SCI_ALLOC_APP(proglist_ptr->ProgTotal * sizeof(MMIMTV_PROGRAMME_T));
    SCI_MEMSET(proglist_ptr->prog_ptr,0,proglist_ptr->ProgTotal * sizeof(MMIMTV_PROGRAMME_T));
    proglist_ptr->ServiceName.str_ptr = SCI_ALLOC_APP(10);
    SCI_MEMCPY(proglist_ptr->ServiceName.str_ptr, str,10);
    proglist_ptr->ServiceName.encode = MTV_ASCII;
    proglist_ptr->ServiceName.length = 4;

    proglist_ptr->prog_ptr[0].Title.str_ptr = SCI_ALLOC_APP(10);
    SCI_MEMCPY(proglist_ptr->prog_ptr[0].Title.str_ptr, str,10);
    proglist_ptr->prog_ptr[0].Title.encode = MTV_ASCII;
    proglist_ptr->prog_ptr[0].Title.length = 5;

    proglist_ptr->prog_ptr[1].Title.str_ptr = SCI_ALLOC_APP(10);
    SCI_MEMCPY(proglist_ptr->prog_ptr[1].Title.str_ptr, str2,10);
    proglist_ptr->prog_ptr[1].Title.encode = MTV_ASCII;
    proglist_ptr->prog_ptr[1].Title.length = 6;
    return ret;
}

/*****************************************************************************/
// Description : 释放所有节目数据
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void CMMB_FreeAllProg(MMIMTV_SERVICE_T  *proglist_ptr)
{
    uint32                 i = 0, count = 0;

    //SCI_TRACE_LOW:"mtvesg_common.c.c:CMMB_FreeAllProg()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_SIMU_609_112_2_18_2_40_51_886,(uint8*)"");
    if(PNULL == proglist_ptr)
    {
        return;
    }
    
    // 释放业务名称
    MMIMTV_FreeString(&proglist_ptr->ServiceName);
    // 释放节目列表
    if (PNULL != proglist_ptr->prog_ptr)
    {
        count  = proglist_ptr->ProgTotal;
        for(i = 0;i < count;i ++)
        {
            MMIMTV_FreeProg(&proglist_ptr->prog_ptr[i]);
        }
        SCI_FREE((void *)proglist_ptr->prog_ptr);
        proglist_ptr->prog_ptr = PNULL;
    }
    // 附属信息清零
    proglist_ptr->ServiceID =  0xFFFF;
    proglist_ptr->NumOfProg = 0;
    proglist_ptr->ProgTotal = 0;
    return;
}


/****************************************************************************/
//  Description: 解析ESG数据(only service information)
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC ESG_ERROR_E CMMB_ParseService(uint16  network_id)
{
    return ESG_SUCCESS;
}

/***********************************************************************/
// 	Description : This function is used to set background picture.
//	Global resource dependence : none
//  Author:
//	parameters;
//             luma_data_ptr: Y Data buffer;luma_data_ptr: UV data buffer
//             wide : width of picture; high : hight of picture;
/***********************************************************************/
void CMMB_SetBackGround(KDuint8* luma_data_ptr, KDuint8 *chroma_data_ptr, KDuint32 wide, KDuint32 high)
{
}

/***********************************************************************/
// 	Description : This function is used to take picture.
//	Global resource dependence : none
//  Author:
//	Note: none;
/***********************************************************************/
uint32 CMMB_TakePicture(uint8 * buffer_addr,LCDC_VIDEO_CAP_DATA_TYPE_E mode,uint32 *wide,uint32 *high)
{
      return 0;
}

/***********************************************************************/
// 	Description : This function is used to complete take picture.
//	Global resource dependence : none
//  Author:
//	Note: none;
/***********************************************************************/
void  CMMB_ResumePicture(void)
{
}

KDuint32 CMMB_GetEsgDateRange(KDuint16     network_id,
                              KDuint16     service_id,
                              MTV_DATE_T  *sdate_ptr,
                              MTV_DATE_T  *edate_ptr)
{
    SCI_DATE_T current_date={0};

    TM_GetSysDate(&current_date);
    MMIMTV_MovePrevDay(current_date, (SCI_DATE_T*)sdate_ptr);
    MMIMTV_MoveNextDay(current_date, (SCI_DATE_T*)edate_ptr);
    return 0;
}

/*****************************************************************************/
//! \fn KDuint32 CMMB_SetEsgDataPath(KDuint16    esg_path_len,  KDuint8     *esg_path_ptr ); 
//! \param esg_path_len   esg path byte len
//! \param esg_path_ptr    esg path pointer
//! \return 0 : on success, non-zero on failed.
//! \brief  Description: This Function is used to set esg data path
//! \author Author: Robert Lu
//! \note   Note: path is existing; esg_path_ptr uses ascii , format according to "D:\MTV",
//!               default "D:\\MTV", max len 20 bytes.
/*****************************************************************************/
KDuint32 CMMB_SetEsgDataPath(KDuint16    esg_path_len,  KDuint8     *esg_path_ptr )
{
    return 0;
}

/*****************************************************************************/
//! \fn KDuint32 CMMB_ParseESG
//! \param network_id.
//! \return KDuint32.
//! \brief  Description: This Function is used to parse all ESG data
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_ParseESG(KDuint16  network_id)
{
    MTV_SendSignaltoMMI(MTV_ESG_PARSE_RESULT_IND);

    return 0;
}

/*****************************************************************************/
//! \fn CMMB_AbortESG
//! \param none.
//! \return none.
//! \brief  Description: This Function is used to abort ESG parse process.
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_AbortESG(void)
{
    return;
}

/***********************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:
//	Note: none;
/***********************************************************************/
KDboolean CMMB_GetEmergBroadcast(KDuint32                trigger_flag,
                                 KDuint32                msg_id,
                                 MTV_STIMI_BROADCAST_U  *bc_ptr)
{
    return TRUE;
}

/*****************************************************************************/
//! \fn void MTV_GetSWVersionNum (void)
//! \param void No parameter.
//! \return version string pointer
//! \brief  Description: This Function is used to get version string
//! \author Author: Geng Ren
//! \note   Note:Nothing.
/*****************************************************************************/
// KDchar * MTV_GetSWVersionNum (void)
// {
//     return PNULL;
// }


/*****************************************************************************/
//! \fn void CMMB_GetChannelStatus
//! \param cs: channel status pointer
//! \return 0 on success, non-zero on failed.
//! \brief  Description: This Function is used to get channel status.
//! \author Author: geng.ren
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_GetChannelStatus(MTV_CHANNEL_STATUS_T *cs)
{
    cs->block_count = 100;
    cs->error_block_count = 1;
    cs->frequency_offset  = 0;
    cs->heartbeat = 5;
    cs->padding = 2;
    cs->rssi = 20;

    return 0;
}


/*****************************************************************************/
//! \fn KDboolean CMMB_GetFrequency(KDuint16 service_id, KDuint32 *freq_no_ptr,  KDuint32 *center_freq_ptr);
//! \param service_id[IN], service id 
//! \param freq_no_ptr[IN], frequency number.
//! \param center_freq_ptr[OUT], center frequency.
//! \return KD_TRUE , get a center frequency, or not .
//! \brief  Description: This Function is used to get center frequency.
//! \author Author: Robert Lu
//! \note   Note: unit kHz.
/*****************************************************************************/
KDboolean CMMB_GetFrequencyByServid(KDuint16 service_id, KDuint32 *freq_no_ptr,  KDuint32 *center_freq_ptr)
{
    *freq_no_ptr = 21;
    *center_freq_ptr = 754000; 
    return TRUE;
}

/*****************************************************************************/
//! \fn CMMB_SetSearchMode(KDboolean is_mbbms);
//! \param is_mbbms[IN], is mbbms.
//! \return none .
//! \brief  Description: This Function is used to set searching mode.
//! \author Author: Robert Lu
//! \note   Note: .
/*****************************************************************************/
void CMMB_SetSearchMode(KDboolean is_mbbms)
{
    return;
}

#endif
#endif
