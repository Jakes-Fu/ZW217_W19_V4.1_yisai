/*****************************************************************************
** File Name:      mmiatv_main.c                                             *
** Author:                                                                   *
** Date:           2010-05                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv flow control            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create                                    *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_atv_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
#include "mmiidle_export.h"
//#include "guiform.h"
#include "guilcd.h"
#include "mmitheme_pos.h"
#include "guiblock.h"
#include "audio_api.h"
#include "mmiatv_export.h"
#include "mmiatv_main.h"
#include "mmiatv_wintab.h"
#include "mmiatv_data.h"
#include "mmiatv_id.h"
#include "mmiatv_nv.h"
#include "mmiatv_text.h"
#include "mmiatv_image.h"
#include "mmiatv_menutable.h"
#include "gpio_prod_api.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "Mmisrvaud_api.h"
#include "mmi_ring.h"
#include "mmi_resource.h"
#include "mmiidle_statusbar.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

//signal icon array
LOCAL const MMI_IMAGE_ID_T s_signal_image_arr[] = 
{
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL1,
        IMAGE_IDLE_TITLE_ICON_TVSIGNAL2,
        IMAGE_IDLE_TITLE_ICON_TVSIGNAL3,
        IMAGE_IDLE_TITLE_ICON_TVSIGNAL4,
        IMAGE_IDLE_TITLE_ICON_TVSIGNAL5
};

LOCAL wchar                 s_record_file_name[MMIATV_FULL_PATH_LEN +1] = {0}; //current saved record file name

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : earphone play pre file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void EventPre(void);

/*****************************************************************************/
//  Description : earphone play next file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void EventNext(void);

/*****************************************************************************/
//  Description : play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AtvPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : stop current action
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_ExitATV(FSMSRV_HANDLE atv_handle)
{ 
    if(MMIATV_IsPlaying()
        ||MMIATV_IsSettingCHN())
    {
        MMIATV_FSMStop(atv_handle);
    }
    else if(MMIATV_IsRecording())
    {
        MMIATV_FSMStopRecord(atv_handle,TRUE);
    }
    else if(MMIATV_IsSearching())
    {
        MMIATV_StopSearchService(atv_handle);
    } 

	MMIATV_FSMClose(atv_handle);
}
/*****************************************************************************/
//  Description : start search service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_StartSearchService(FSMSRV_HANDLE atv_handle)
{   
    BOOLEAN        search_ret = TRUE;
    MMIATV_SERVICE_NODE_T   *favorite_ptr = MMIATV_GetFavoriteServiceNodeViaIndex(0);
    
	if (MMIATV_IsRecording())
    {
		MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);
        return;
    }

    //clear service list
    MMIATV_ClearServiceList();
    MMIATV_WriteServiceListFile();    
    
    //收藏列表的region与当前搜索的region不一致时，清空收藏列表
    if(PNULL != favorite_ptr)
    {
        MMIATV_ClearFavoriteServiceList();
        MMIATV_WriteFavoriteServiceListFile();
    } 
    
    //if playing, stop first
    MMIATV_FSMStop(atv_handle);
    search_ret = MMIATV_FSMSearch(atv_handle);
    
    SCI_TRACE_LOW("[ATV] MMIATV_StartSearchService search_ret = %d",search_ret);
    if (search_ret)
    {
        MMIATV_EnableLcdSwitch(FALSE);
    }
    else
    {   
        MMIATV_DisplayCommonTip(TXT_ATV_SEARCH_ERROR);
        MMIATV_FSMPlay(atv_handle,MMIATV_GetCurrentServiceID());
    }
    
} 

/*****************************************************************************/
//  Description : stop search service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_StopSearchService(FSMSRV_HANDLE atv_handle)
{
    MMIATV_FSMStopSearch(atv_handle);
    MMIATV_WriteServiceListFile();   
    MMIATV_EnableLcdSwitch(TRUE);
    MMIATV_ClearCommonTip();
    MMIATV_ClearSearchProgress();    
}


/*****************************************************************************/
//  Description : get signal iamge_id for status bar
//  Global resource dependence :
//  Author: xin.li
//  Note: level 0-5
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIATV_GetTVStatusImageID(uint8 signal)
{
    if(signal > MMIATV_SIGNAL_LEVEL_MAX)
    {
        signal = MMIATV_SIGNAL_LEVEL_MAX;
    }
    
    return s_signal_image_arr[signal];
}

/*****************************************************************************/
//  Description : earphone play next file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void EventNext(void)
{
    //SCI_TRACE_LOW:"[MMIATV]: EventNext enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_1125_112_2_18_2_3_12_77,(uint8*)"");
    MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_APP_RIGHT, PNULL);
}

/*****************************************************************************/
//  Description : earphone play pre file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void EventPre(void)
{
    //SCI_TRACE_LOW:"[MMIATV]: EventPre enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_1135_112_2_18_2_3_12_78,(uint8*)"");
    MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_APP_LEFT, PNULL);
}

/*****************************************************************************/
//  Description : play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AtvPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    
    if(PNULL != param)
    {
        //SCI_TRACE_LOW:"[MMIATV]:AtvPlayCallBack event=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_1149_112_2_18_2_3_12_79,(uint8*)"d",param->event);
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                //SCI_TRACE_LOW:"[MMIATV]:AtvPlayCallBack report=%d,data=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_1160_112_2_18_2_3_12_80,(uint8*)"dd",report_ptr->report,report_ptr->data1);
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_BT_EVENT:
                    switch(report_ptr->data1) 
                    {
                    case MMISRVAUD_BT_EVENT_KEY_PAUSE:
                    case MMISRVAUD_BT_EVENT_KEY_RESUME:
                        break;
                        
                    case MMISRVAUD_BT_EVENT_KEY_PREV:
                        EventPre();
                        break;
                    case MMISRVAUD_BT_EVENT_KEY_NEXT:
                        EventNext();
                        break;
                        
                    case MMISRVAUD_BT_EVENT_KEY_VOL_UP:
                        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_INC);
                        break;
                        
                    case MMISRVAUD_BT_EVENT_KEY_VOL_DOWN:
                        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_DEC);
                        break;
                        
                    default:
                        break;
                    }
                    break;
                    
                case MMISRVAUD_REPORT_EARPHONE_EVENT:
                    switch(report_ptr->data1) 
                    {
                    case (MMISRVAUD_EARPHONE_EVENT_CLK + KEY_HEADSET_BUTTIN):
                    	break;

                    case (MMISRVAUD_EARPHONE_EVENT_LONG + KEY_HEADSET_BUTTIN):
                        EventNext();
                    	break;

                    default:
                        break;
                    }
                    break;
                    
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    return TRUE;
    
}


/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIATV_RequestHandle(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.pri = MMISRVAUD_PRI_NORMAL;
    req.notify = AtvPlayCallBack;
    audio_srv.volume =  MMIAPISET_GetMultimVolume();
    audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
    audio_srv.info.type = MMISRVAUD_TYPE_ATV;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    
    return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

/*****************************************************************************/
//  Description : save current record file name
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIATV_SaveRecordFileName(void)
{
    wchar   *video_name_ptr = MMIATV_GetVideoName();
    uint16  name_len = MMIAPICOM_Wstrlen(video_name_ptr);
    
    if (PNULL != video_name_ptr)
    {   
        SCI_MEMSET(s_record_file_name, 0, MMIATV_FULL_PATH_LEN);
        name_len = MIN(name_len, MMIATV_FULL_PATH_LEN);
        MMIAPICOM_Wstrncpy(s_record_file_name, video_name_ptr, name_len);
    }
}



/*****************************************************************************/
//  Description : get current saved record file name
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIATV_GetRecordFileName(void)
{
    return s_record_file_name;
}

/*****************************************************************************/
//  Description : save last played service id to nv
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveLastPlayServiceID(uint32 serviceId)
{
    MMIATV_SAVE_PROG_T last_play_prog = {0};
    
    last_play_prog.service_id = serviceId;
    
    //save the last played service_id
    MMINV_WRITE(MMINV_ATV_PROG_ID, &last_play_prog);
}

/*****************************************************************************/
//  Description : get last played service id from nv
//  Global resource dependence : none
//  Author: xin.li
//  Note: if get success return TRUE, else return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetLastPlayServiceID(uint32 *id_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMIATV_SAVE_PROG_T last_play_prog = {0};
    
    if(PNULL == id_ptr)
    {
        return FALSE;
    }
    
    //get last play service info from nv
    MMINV_READ(MMINV_ATV_PROG_ID, &last_play_prog, return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        *id_ptr = last_play_prog.service_id;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : update current play index by the service id
//  Global resource dependence : none
//  Author: xin.li
//  Note: 通过service_id更新当前播放索引值，如初始进入播放、从收藏列表播放
/*****************************************************************************/
PUBLIC uint32 MMIATV_UpdateCurrentPlayIndexByServiceId(uint32 service_id)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    uint32  index = 0;
    
    
    if(0 == service_id)
    {
        //invalid service id        
        return 0;
    }
    
    //get service list head
    cursor = MMIATV_GetServiceNodeViaIndex(0);
    
    while(PNULL != cursor)
    {
        if(cursor->service_id == service_id)
        {
            //update current play index           
            return index;
        }
        
        index ++;
        cursor = cursor->service_next_ptr;
    }
    
    //not found, reset current play index
    return 0;
}

/*****************************************************************************/
//  Description : play previous service
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_PlayPreService(FSMSRV_HANDLE atv_handle ,
                                     uint32 *cur_index_ptr
                                     )
{
    MMIATV_SERVICE_NODE_T *node_ptr = PNULL;
    uint32 total_num = MMIATV_GetServiceListNum();
    
    if (PNULL == cur_index_ptr)
    {
        return FALSE;
    }
    
    SCI_TRACE_LOW("[ATV] MMIATV_PlayPreService cur_index = %d",*cur_index_ptr);
	if (0 == total_num
        || 1 == total_num)
    {
        
    }
    else if(0 == *cur_index_ptr)
    {
        *cur_index_ptr = total_num - 1;
    }
    else
    {
        (*cur_index_ptr) --;
    }
    SCI_TRACE_LOW("[ATV] MMIATV_PlayPreService cur_index = %d",*cur_index_ptr);
    node_ptr = MMIATV_GetServiceNodeViaIndex(*cur_index_ptr);
    if(PNULL != node_ptr)
    {
        return MMIATV_FSMPlay(atv_handle,node_ptr->service_id); 
        
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIATV]: PlayNextService PNULL == node_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4235_112_2_18_2_3_22_107,(uint8*)"");
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : play next service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_PlayNextService(FSMSRV_HANDLE atv_handle,
                                      uint32 *cur_index_ptr)
{
    MMIATV_SERVICE_NODE_T *node_ptr = PNULL;
    uint32 total_num = MMIATV_GetServiceListNum();
    
    if (PNULL == cur_index_ptr)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("[ATV] MMIATV_PlayNextService cur_index = %d",*cur_index_ptr);
    
    if (0 == total_num
        || 1 == total_num)
    {
        
    }
    else
    {
        *cur_index_ptr = (*cur_index_ptr + 1) % total_num;
    }
    
    node_ptr = MMIATV_GetServiceNodeViaIndex(*cur_index_ptr);
    if(PNULL != node_ptr)
    {
        return MMIATV_FSMPlay(atv_handle,node_ptr->service_id); 
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIATV]: PlayNextService PNULL == node_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4285_112_2_18_2_3_22_110,(uint8*)"");
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : exit atv
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_ReleaseATV(FSMSRV_HANDLE atv_handle )
{
    uint32 service_id = 0;
    
    //SCI_TRACE_LOW:"[MMIATV]: ReleaseATV"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4052_112_2_18_2_3_21_104,(uint8*)"");
    MMIATV_WriteFavoriteServiceListFile();
    
    //stop play or search    
    service_id = MMIATV_GetCurrentServiceID();
    MMIATV_SaveLastPlayServiceID(service_id);
    MMIATV_WriteServiceListFile();
    
    //close dc
    MMIATV_ExitATV(atv_handle);
    
    MMIATV_ClearServiceList();
    MMIATV_ClearFavoriteServiceList();
    MMIATV_SaveSetting();
    MMIATV_DeleteData();   
    
#ifndef WIN32
    //恢复频率
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif   
    
}

/*****************************************************************************/
//  Description : enter atv, init data, setting, ref etc.
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_InitATV(FSMSRV_HANDLE atv_handle,
                              uint32 *service_id_ptr)
{    
    uint32 list_num = 0;
    MMIATV_SERVICE_NODE_T* service_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIATV]: InitATV"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_3964_112_2_18_2_3_21_101,(uint8*)"");
    
    if (PNULL == service_id_ptr)
    {
        return FALSE;
    }
    
#ifndef WIN32
    //升频
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),(CHNG_FREQ_INDEX_E)FREQ_INDEX_APP_HIGH);
#endif   
    
    //init data
    MMIATV_InitData();
    MMIATV_InitSetting();
    MMIATV_ReadServiceListFile();
    MMIATV_ReadFavoriteServiceListFile();
    
    //init ref
    if(!MMIATV_FSMOpen(atv_handle))
    {        
        return FALSE;
    }    
    
    list_num = MMIATV_GetServiceListNum();
    MMIATV_GetLastPlayServiceID(service_id_ptr);
    if(list_num > 0 && 0 == *service_id_ptr)
    {
        service_ptr = MMIATV_GetServiceNodeViaIndex(0);
        if(PNULL != service_ptr)
        {
            *service_id_ptr = service_ptr->service_id;
        }
    }
    else if(0 == list_num && 0 != *service_id_ptr) //搜索时异常关机的情况
    {
        MMIATV_SaveLastPlayServiceID(0);
        *service_id_ptr = 0;
    }
    else
    {
        // nothing to do
    }
    //SCI_TRACE_LOW:"[MMIATV]: InitAtv lastService=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4020_112_2_18_2_3_21_103,(uint8*)"d", *service_id_ptr);
    //if get last played service then start play, else start search
    if(*service_id_ptr == 0)    
    {
        //start search
        MMIATV_StartSearchService(atv_handle);
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : add a new service node to service list
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_AddServiceNodeByID(uint32 cur_num, uint32 service_id)
{
    char str_name[MMIATV_NAME_MAX_LEN + 1] = {0};
    MMIATV_SERVICE_NODE_T service_node = {0};
    
    //SCI_TRACE_LOW:"[MMIATV]: AddServiceNode cur_num=%d service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_804_112_2_18_2_3_11_66,(uint8*)"dd", cur_num, service_id);
    
    //service name
    sprintf(str_name, "CH%d", (cur_num+1));
    MMIAPICOM_StrToWstr((const uint8*)str_name, service_node.service_name);
    
    //region
    service_node.region = MMIATV_GetRegion();
    
    //service id
    service_node.service_id = service_id;
    
    //add to service list
    MMIATV_AddServiceNode(&service_node);
}
