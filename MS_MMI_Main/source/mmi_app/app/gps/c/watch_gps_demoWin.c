/*****************************************************************************
** File Name:        watch_gps_demoWin.c                                     *
** Author:           xiuyun.wang                                             *
** Date:             03/03/2020                                              *
** Copyright:                                                                *
** Description:    This file is used to create a demo window for GPS         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 03/03/2020           xiuyun.wang           Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "window_parse.h"
#include "mmigps_api.h"
#include "mmicom_trace.h"
#include "os_api.h"
#include "sig_code.h"
#include "mmi_signal_ext.h"
#include "guilistbox.h"
#include "mmi_id.h"
#include "mmi_text.h"

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

LOCAL uint8  s_request_sat_info_timer_id = 0;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
LOCAL void _HandleRequestSatInfoTimer(uint8 timer_id,uint32 param)
{
    if (s_request_sat_info_timer_id == timer_id)
    {
        MMK_StopTimer(s_request_sat_info_timer_id);
        s_request_sat_info_timer_id = 0;
    }
    TRACE_APP_GPS("request gps info again\n");
    MNGPS_ReadInfoReq();
}

LOCAL void DemoGps_StartRequestSatInfoTimer(void)
{
    if (NULL != s_request_sat_info_timer_id)
    {
        MMK_StopTimer(s_request_sat_info_timer_id);
        s_request_sat_info_timer_id = 0;
    }
    TRACE_APP_GPS("[GPS]:Gps_StartRequestSatInfoTimer\n");
    s_request_sat_info_timer_id = MMK_CreateTimerCallback(10000, _HandleRequestSatInfoTimer, NULL, FALSE);
}

LOCAL void DemoGps_AppendSatelliteListItem(MMI_CTRL_ID_T  ctrl_id, APP_MN_GPS_READ_INFO_CNF_T *gpsSatelliteInfo)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};

    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    uint8 i = 0;

    TRACE_APP_GPS("enter function\n");
    GUILIST_RemoveAllItems(MMIDEMO_LIST_CTRL_ID);    

    TRACE_APP_GPS("[GPS]: longitude %f \n",gpsSatelliteInfo->gps_info.longitude);
    TRACE_APP_GPS("[GPS]: satellite num %d\n",gpsSatelliteInfo->satellite_list_info.satellite_num);    

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_info.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    //append location info
    // Satellite Number
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    sprintf(tmp_item, "%s:%d", "Satellite Number", gpsSatelliteInfo->satellite_list_info.satellite_num);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id, &item_info);
    TRACE_APP_GPS("[GPS]: %s\n",tmp_item);

    //TTFF
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    sprintf(tmp_item, "%s:%lds", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
    MMI_STRNTOWSTR( tmp_wstr_1,
        GUILIST_STRING_MAX_NUM,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id, &item_info);
    TRACE_APP_GPS("[GPS]: %s\n",tmp_item);
    
    //Latitude
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    sprintf(tmp_item, "%s:%f", "Latitude", gpsSatelliteInfo->gps_info.latitude);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id, &item_info);
    TRACE_APP_GPS("[GPS]: %s\n",tmp_item);

    //Longitude
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    sprintf(tmp_item, "%s:%f", "Longitude", gpsSatelliteInfo->gps_info.longitude);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id, &item_info);
    TRACE_APP_GPS("[GPS]: %s\n",tmp_item);

    //Altitude
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    sprintf(tmp_item, "%s:%f", "Altitude", gpsSatelliteInfo->gps_info.altitude);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id, &item_info);
    TRACE_APP_GPS("[GPS]: %s\n",tmp_item);
    
    MMK_PostMsg(MMIDEMO_GPS_WIN_ID,MSG_FULL_PAINT,PNULL,0);
}
LOCAL BOOLEAN _HandleGpsMsgCallback(APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    static APP_MN_GPS_READ_INFO_CNF_T report_gpsinfo = {0};//get information from report
    APP_MN_GPS_READ_INFO_CNF_T read_gpsinfo = {0};    
    int i =0;
    char test_str[256] = {0};
    TRACE_APP_GPS("[GPS]: gps_sig_id:%d\n",gps_sig_id);
    switch ( gps_sig_id )
    {
        case APP_MN_GPS_START_CNF:
        {
            //CALL report req
            APP_MN_GPS_START_CNF_T *pStartInfo = (APP_MN_GPS_START_CNF_T *)gps_info;
            if(TRUE == pStartInfo->is_ok)
            {
                MNGPS_ReportInfoReq(pStartInfo->dual_sys,TRUE,TRUE);   //request gps and location info
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_START_CNF is OK\n");

            }else
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_START_CNF is NOT OK\n");
            }
            break;
        }
        case APP_MN_GPS_RESET_CNF:
        {
            break;
        }
        case APP_MN_GPS_READ_STATUS_CNF:
        {
            break;
        }
        case APP_MN_GPS_READ_INFO_CNF:
        {
            APP_MN_GPS_READ_INFO_CNF_T *param_ptr = (APP_MN_GPS_READ_INFO_CNF_T *)gps_info;
            TRACE_APP_GPS("[GPS]: APP_MN_GPS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_READ_INFO_CNF  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
            read_gpsinfo.gps_info.latitude  = param_ptr->gps_info.latitude;     
            read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;            
            read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
            read_gpsinfo.gps_info.satellite_num = param_ptr->gps_info.satellite_num;
            TRACE_APP_GPS("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.longitude = %d ",read_gpsinfo.gps_info.longitude);
            TRACE_APP_GPS("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.latitude = %d ",read_gpsinfo.gps_info.latitude);
            TRACE_APP_GPS("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.altitude = %d ",read_gpsinfo.gps_info.altitude);
            TRACE_APP_GPS("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.ttff_time = %d ",read_gpsinfo.gps_info.ttff_time);
            TRACE_APP_GPS("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.satellite_num = %d ",read_gpsinfo.gps_info.satellite_num);

            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info, 
                param_ptr->satellite_list_info.satellite_info, 
                sizeof(param_ptr->satellite_list_info.satellite_info));
            TRACE_APP_GPS("[GPS]read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                sprintf(test_str, "%s: APP_MN_GPS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_cn0,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nElevation,  param_ptr->satellite_list_info.satellite_info[i].m_nAzimuth,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_IsUsed);
                TRACE_APP_GPS("[GPS]:%s",test_str); 
            }
            
            if(0 != read_gpsinfo.satellite_list_info.satellite_num)
            {
                DemoGps_AppendSatelliteListItem(MMIDEMO_LIST_CTRL_ID, &read_gpsinfo);
            }
            else
            {
                SCI_MEMCPY(&read_gpsinfo, &report_gpsinfo, sizeof(report_gpsinfo));
                DemoGps_AppendSatelliteListItem(MMIDEMO_LIST_CTRL_ID, &read_gpsinfo);
                DemoGps_StartRequestSatInfoTimer();    
            }
            break;
        }
         case APP_MN_GPS_REPORT_CNF:
        {
            APP_MN_GPS_REPORT_CNF_T* pReportCnf= (APP_MN_GPS_REPORT_CNF_T *)gps_info;
            TRACE_APP_GPS("[GPS]: enter APP_MN_GPS_REPORT_CNF\n");
            TRACE_APP_GPS("[GPS]: enter APP_MN_GPS_REPORT_CNF %d\n",pReportCnf->is_ok);
            if(FALSE == pReportCnf->is_ok)
            {
                MNGPS_ReportInfoReq(pReportCnf->dual_sys,TRUE,TRUE);
            }
            break;
        }
        case APP_MN_GPS_SATELLITE_INFO_IND:
        {
            APP_MN_GPS_SATELLITE_INFO_IND_T * pSateInfo = (APP_MN_GPS_SATELLITE_INFO_IND_T *) gps_info;
            TRACE_APP_GPS("[GPS]: enter APP_MN_GPS_SATELLITE_INFO_IND\n");

            if(PNULL == pSateInfo)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_SATELLITE_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            if(0 == pSateInfo->satellite_list_info.satellite_num)
            {
                TRACE_APP_GPS("[GPS]: enter APP_MN_GPS_SATELLITE_INFO_IND\n");
            }
            
            //save satellite information to report_gpsinfo
            report_gpsinfo.satellite_list_info.satellite_num = pSateInfo->satellite_list_info.satellite_num;
            SCI_MEMCPY(report_gpsinfo.satellite_list_info.satellite_info, 
                pSateInfo->satellite_list_info.satellite_info, 
                sizeof(pSateInfo->satellite_list_info.satellite_info));
            TRACE_APP_GPS("[GPS]:APP_MN_GPS_SATELLITE_INFO_IND, copyed satellite num=%d\n",  report_gpsinfo.satellite_list_info.satellite_num);
            break;
        }
        
        case APP_MN_GPS_LOCATION_INFO_IND:
        {
            APP_MN_GPS_LOC_INFO_IND_T * pSateInfo = (APP_MN_GPS_LOC_INFO_IND_T *) gps_info;
            TRACE_APP_GPS("[GPS]: APP_MN_GPS_LOCATION_INFO_IND\n");
            if(PNULL == pSateInfo)
            {

                TRACE_APP_GPS("[GPS]: APP_MN_GPS_LOCATION_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }

            TRACE_APP_GPS("[GPS]:APP_MN_GPS_LOCATION_INFO_IND, latitude=%f\n", pSateInfo->gps_info.latitude);
            //save gps information to g_gpsinfo
            report_gpsinfo.gps_info.altitude = pSateInfo->gps_info.altitude;
            report_gpsinfo.gps_info.latitude= pSateInfo->gps_info.latitude;
            report_gpsinfo.gps_info.longitude= pSateInfo->gps_info.longitude;
            report_gpsinfo.gps_info.satellite_num= pSateInfo->gps_info.satellite_num;
            report_gpsinfo.gps_info.ttff_time= pSateInfo->gps_info.ttff_time;
            //SCI_MEMCPY(&(report_gpsinfo.gps_info), &(pSateInfo->gps_info), sizeof(pSateInfo->gps_info));
            TRACE_APP_GPS("[GPS]:APP_MN_GPS_LOCATION_INFO_IND, altitude =%f, latitude=%f, longitude=%f, satellite_num=%d, ttff_time = %f",
                report_gpsinfo.gps_info.altitude,
                report_gpsinfo.gps_info.latitude,
                report_gpsinfo.gps_info.longitude,
                report_gpsinfo.gps_info.satellite_num,
                report_gpsinfo.gps_info.ttff_time);
            break;
         }
        case APP_MN_GPS_FIX_IND:
        {
            APP_MN_GPS_FIX_IND_T * pFixInfo = (APP_MN_GPS_FIX_IND_T *)gps_info;
            if(PNULL == pFixInfo)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_SATELLITE_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            break;
        }
        case APP_MN_GPS_STOP_CNF:
        {
            APP_MN_GPS_STOP_CNF_T *pStopcnf = (APP_MN_GPS_STOP_CNF_T *)gps_info;
            if(PNULL == pStopcnf)
            {
                TRACE_APP_GPS("[GPS]: APP_MN_GPS_SATELLITE_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            TRACE_APP_GPS("[GPS]:APP_MN_GPS_STOP_CNF is OK\n");
            break;
        }

        default:
        {
            result = MMI_RESULT_FALSE;
            TRACE_APP_GPS("[GPS]: Unknow msg id!\n");
            break;
        }
    }
    return result;
}

LOCAL void DemoGps_InitSatelliteList(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T item_data= {0};
    wchar tmp_wstr_1[] = {'w', 'a', 'i', 't', 'i', 'n', 'g', 0};

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;     
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_wstr_1);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;

    item_info.item_data_ptr = &item_data;
    GUILIST_AppendItem(ctrl_id, &item_info);    
}
LOCAL MMI_RESULT_E HandleDemoGPSWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;  

    TRACE_APP_GPS("[GPS]: MMI_MESSAGE_ID_E:%d\n",msg_id);
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
        {
            /*start GPS first*/
            MMIGPS_START_GPS_PARAM_T start_param = {0};         
            start_param.request_type = MMIGPS_REQUEST_TYPE_ENG;
            start_param.start_type = MMIGPS_START_TYPE_NONE;
            start_param.start_mode = 0;
            TRACE_APP_GPS("start gps");
            MMIGPS_Start(&start_param,_HandleGpsMsgCallback);

            /*init the list ctrl*/
            GUILIST_SetMaxItem(MMIDEMO_LIST_CTRL_ID, 140, FALSE );
            GUILIST_SetNeedHiLightBar(MMIDEMO_LIST_CTRL_ID,FALSE);
            MMK_SetAtvCtrl(win_id,MMIDEMO_LIST_CTRL_ID);
            DemoGps_InitSatelliteList(MMIDEMO_LIST_CTRL_ID);

            //request read gps info
            TRACE_APP_GPS("start MNGPS_ReadInfoReq");
            MNGPS_ReadInfoReq(0);
             //the first request has no response so start timer to avoid null display 
            DemoGps_StartRequestSatInfoTimer();    
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(MMIDEMO_GPS_WIN_ID);
            break;  
        }
        case MSG_CTL_MIDSK: 
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        {
            TRACE_APP_GPS(" refresh satellite information, start MNGPS_ReadInfoReq");
            MNGPS_ReadInfoReq(0);
            break;   
        }
        case MSG_KEYUP_UP:
        case MSG_KEYDOWN_UP:
        case MSG_KEYLONG_UP:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(MMIDEMO_LIST_CTRL_ID);
            GUILIST_SetTopItemIndex(MMIDEMO_LIST_CTRL_ID, (top_item - 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
            break;
        }
        case MSG_KEYUP_DOWN:
        case MSG_KEYDOWN_DOWN:
        case MSG_KEYLONG_DOWN:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(MMIDEMO_LIST_CTRL_ID);
            GUILIST_SetTopItemIndex(MMIDEMO_LIST_CTRL_ID, (top_item + 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}
WINDOW_TABLE( MMIDEMO_GPS_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleDemoGPSWinMsg ),    
    WIN_ID( MMIDEMO_GPS_WIN_ID ),
    WIN_SOFTKEY(TXT_REFRESH, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIDEMO_LIST_CTRL_ID),
    END_WIN                                                                                   

};

PUBLIC void MMIDEMO_GPS_EnterWin(void)
{
    MMK_CreateWin((uint32 *)MMIDEMO_GPS_WIN_TAB, PNULL);
}

