/*****************************************************************************
** File Name:      watchface_config_adapter.c                                *
** Author:         yifan.yang                                                *
** Date:           10/08/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define json adaptation.              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2021       chunjuan.liang        Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sfs.h"
#include "json_parser.h"
#include "watchface_config.h"
#include "cJSON.h"
#include "json_parser_watchface.h"
#include "watchface_config_adapter.h"
#include "mmicom_trace.h"
#include "mmi_default.h"
#include "ctrlwf_export.h"
#include "watchface_ctrl_adapter.h"
#include "guilcd.h"
#include "ctrlanim_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define XIAOXUAN_PREVIEW_NAME        "preview.jpg"
#define XIAOXUAN_JSONFILE_NAME        "watchface_config.json"
#define UNISOC_JSONFILE_NAME          "watchface_layout.json"
#define UNISOC_JSONFILE_SIZE          6000
#define MONTH_SIZE                    20
#define TIME_SIZE                     15
#define DAY_SIZE                      15
#define DATE_SIZE                     15
#define MULTINUM_SIZE                 15
#define ANALOG_UPDATE                 500
#define DIGITAL_UPDATE                30000
#define MONTH_COUNT                   12
#define DAY_COUNT                     10
#define DATE_COUNT                    10
#define MULTINUM_COUNT                10
//xiaoxuan标签
#define XIAOXUAN_BACKGOUND            "background"
#define XIAOXUAN_BACKGOUNDS           "backgrounds"
#define XIAOXUAN_SHOWMONTH            "showMonth"
#define XIAOXUAN_MONTHPOSITIONX       "monthPositionX"
#define XIAOXUAN_MONTHPOSITIONY       "monthPositionY"
#define XIAOXUAN_SHOWWEEK             "showWeek"
#define XIAOXUAN_WEEKPOSITIONX        "weekPositionX"
#define XIAOXUAN_WEEKPOSITIONY        "weekPositionY"
#define XIAOXUAN_SHOWDAY              "showDay"
#define XIAOXUAN_DAYPOSITIONX         "dayPositionX"
#define XIAOXUAN_DAYPOSITIONY         "dayPositionY"
#define XIAOXUAN_DAYSUFFIX            "daySuffix"
#define XIAOXUAN_SHOWTIME             "showTime"
#define XIAOXUAN_TIMEPOSITIONX        "timePositionX"
#define XIAOXUAN_TIMEPOSITIONY        "timePositionY"
#define XIAOXUAN_TIMESUFFIX           "timeSuffix"
#define XIAOXUAN_HOURPOSITIONX        "timeHourPositionX"
#define XIAOXUAN_HOURPOSITIONY        "timeHourPositionY"
#define XIAOXUAN_HOURSUFFIX           "timeHourSuffix"
#define XIAOXUAN_MINUTEPOSITIONX      "timeMinutePositionX"
#define XIAOXUAN_MINUTEPOSITIONY      "timeMinutePositionY"
#define XIAOXUAN_MINUTESUFFIX         "timeMinuteSuffix"
#define XIAOXUAN_HOUR                 "hourHand"
#define XIAOXUAN_MINUTE               "minuteHand"
#define XIAOXUAN_SECOND               "secondHand"
#define XIAOXUAN_UPDATEINTERVAL       "updateInterval"
#define XIAOXUAN_COLON                "colon.png"
#define XIAOXUAN_SHOWSTEP             "showStepImage"
#define XIAOXUAN_STEPPOSITIONX        "stepPositionX"
#define XIAOXUAN_STEPPOSITIONY        "stepPositionY"
#define XIAOXUAN_STEPIMGNONE          "stepPlaceHolder"
#define XIAOXUAN_STEPSUFFIX           "stepSuffix"
#define XIAOXUAN_SHOWHEARTRATE        "showheartRateImage"
#define XIAOXUAN_HEARTRATEPOSITIONX   "heartRatePositionX"
#define XIAOXUAN_HEARTRATEPOSITIONY   "heartRatePositionY"
#define XIAOXUAN_HEARTRATEIMGNONE     "heartRatePlaceHolder"
#define XIAOXUAN_HEARTRATESUFFIX      "heartRateSuffix"
#define XIAOXUAN_SHOWBATTERYTEXT      "showBatteryText"
#define XIAOXUAN_BATTERYTEXTPOSITIONX "batteryTextPositionX"
#define XIAOXUAN_BATTERYTEXTPOSITIONY "batteryTextPositionY"
#define XIAOXUAN_BATTERYTEXTIMGNONE   "batteryTextPlaceHolder"
#define XIAOXUAN_BATTERYTEXTSUFFIX    "batteryTextSuffix"
#define XIAOXUAN_SHOWDATE             "showDateImage"
#define XIAOXUAN_DATEPOSITIONX        "dateImagePositionX"
#define XIAOXUAN_DATEPOSITIONY        "dateImagePositionY"
#define XIAOXUAN_DATESUFFIX           "dateImageSuffix"
#define XIAOXUAN_DATE_MONTHIMG        "dateImageMonth"
#define XIAOXUAN_DATE_DAYIMG          "dateImageDay"
#define XIAOXUAN_LOOPCOUNT            "loopCount"
#define XIAOXUAN_HOURSHADOW           "hourHandShadow"
#define XIAOXUAN_MINUTESHADOW         "minuteHandShadow"
#define XIAOXUAN_SECONDSHADOW         "secondHandShadow"
#define XIAOXUAN_SHADOWTRANSLATEX     "hourShadowTranslateX"
#define XIAOXUAN_SHADOWTRANSLATEY     "hourShadowTranslateY"

//unisoc 标签
#define UNISOC_SCALE                  "Scale"
#define UNISOC_BACKGOUND              "Background"
#define UNISOC_DIGITALTIME            "DigitalTime"
#define UNISOC_MONTH                  "Month"
#define UNISOC_DAY                    "Day"
#define UNISOC_DIGITALDATE            "DigitalDate"
#define UNISOC_WEEK                   "Week"
#define UNISOC_MULINUM                "MultiNum"
#define UNISOC_ANALOGTIME             "AnalogTime"
#define UNISOC_CENTER_POSITION        "center_position"
#define UNISOC_X                      "X"
#define UNISOC_Y                      "Y"
#define UNISOC_IMAGE_LIST             "image_list"
#define UNISOC_IMAGE_NONE             "image_none"
#define UNISOC_DATA_TYPE              "data_type"
#define UNISOC_IMAGE_CONNECTOR        "image_connector"
#define UNISOC_IMAGE_HOUR             "image_hour"
#define UNISOC_IMAGE_MINUTE           "image_minute"
#define UNISOC_IMAGE_SECOND           "image_second"
#define UNISOC_UPDATE_INTERVAL        "update_interval"
#define UNISOC_LOOPCOUNT              "loopcount"
#define UNISOC_DOUBLE_DIGITAL         "double_digital"
#define UNISOC_TYPE                   "type"
#define UNISOC_NONE                   "NONE"
#define UNISOC_HOUR                   "hour"
#define UNISOC_MIN                    "min"
#define UNISOC_HOURMIN                "hourmin"
#define UNISOC_DATE_MONTHDAY          "md"
#define UNISOC_DATE_DAY               "day"

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL float  s_scale_value = 1.0;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :AddAnalogTimeItem:添加analogtime的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaldate type
//             [Out]p_uni_analogtime:analogtime item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddAnalogTimeItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_analogtime,
                                        BOOLEAN   is_shadow
                                    );

/*****************************************************************************/
//  Description :AddDigitalTimeItem:添加digitaltime的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaltime type
//             [Out]p_uni_digitaltime:digitaltime item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddDigitalTimeItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_digitaltime,
                          WATCHFACE_ALL_TYPE_E    type
                                    );

/*****************************************************************************/
//  Description :AddDigitalDateItem:添加digitaldate的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaldate type
//             [Out]p_uni_digitaldate:digitaldate item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddDigitalDateItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_digitaldate,
                         WATCHFACE_ALL_TYPE_E     type
                                    );

/*****************************************************************************/
//  Description :AddMultinumItem:添加multinum的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：multinum type
//             [Out]p_uni_multinum:multinum item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddMultinumItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_multinum,
                          WATCHFACE_ALL_TYPE_E    type
                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddBackground:添加background信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddBackground (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddDigitalTime:添加digitaltime信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDigitalTime (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddAnalogTime:添加analogtime信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddAnalogTime (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddMonth:添加month信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddMonth (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );
#if 0
/*****************************************************************************/
//  Description :JSONPARSER_AddDay:添加day信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDay (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );
#endif
/*****************************************************************************/
//  Description :JSONPARSER_AddDigitalDate:添加date信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDigitalDate (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddWeek:添加week信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddWeek (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );

/*****************************************************************************/
//  Description :JSONPARSER_AddMultinum:添加multinum信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddMultinum (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    );
/*****************************************************************************/
//  Description :CalculatorScaleValue:计算scale值
//  Parameter: [In] p_wf_id,watchface id
//             [Out]
//             [Return] 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculatorScaleValue(char* p_wf_id);
/*****************************************************************************/
//  Description :JSONPARSER_AddScale:添加scale值
//  Parameter: [In]: p_uni_root:watchface layout 根标签
//             [Out]:
//             [Return] JSON_PARSER_RESULT_E
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddScale (cJSON* p_uni_root );

/*****************************************************************************/
//  Description :Get_CustomerJsonString:获取小萱json文件信息
//  Parameter: [In] p_wf_id:表盘id
//             [Out]p_json_cus_string:对应表盘的uni JSON文件信息
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL WF_CONFIG_ADAPTER_RESULT_E Get_CustomerJsonString(char *p_wf_id,char *p_json_cus_string)
{
    SFS_HANDLE src_file_handle = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    BOOLEAN            del_ret = FALSE;
    char     str_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    TRACE_JSON_PARSER("[adapter]enter");

    if(PNULL == p_wf_id)
    {
        TRACE_JSON_PARSER("[adapter]p_wf_id is null");
        return ADAPTER_PARAM_ERROR;
    }

    if(PNULL == p_json_cus_string)
    {
        TRACE_JSON_PARSER("[adapter]p_json_cus_string is null");
        return ADAPTER_PARAM_ERROR;
    }

    sprintf(str_file_name, "%s\\%s\\%s",WATCHFACE_Get_UnintalledFileFolder(),p_wf_id,UNISOC_JSONFILE_NAME);
    TRACE_JSON_PARSER("[adapter]unisoc file name is %s", str_file_name);

    MMIAPICOM_StrToWstr(str_file_name, src_file_name);

    if(TRUE == MMIFILE_IsFileExist(src_file_name,MMIAPICOM_Wstrlen(src_file_name)))
    {
        TRACE_JSON_PARSER("[adapter]layout exist");

        SCI_MEMSET(str_file_name,0x00,sizeof(char)*SCI_STRLEN(str_file_name));
        SCI_MEMSET(src_file_name,0x00,sizeof(wchar)*MMIAPICOM_Wstrlen(src_file_name));

        sprintf(str_file_name, "%s\\%s\\%s",WATCHFACE_Get_UnintalledFileFolder(),p_wf_id,XIAOXUAN_JSONFILE_NAME);
        TRACE_JSON_PARSER("[adapter]xiaoxuan file name is %s", str_file_name);

        MMIAPICOM_StrToWstr(str_file_name, src_file_name);
        if(TRUE == MMIFILE_IsFileExist(src_file_name,MMIAPICOM_Wstrlen(src_file_name)))
        {
            TRACE_JSON_PARSER("[adapter]config exist");
            del_ret = MMIAPIFMM_DeleteFileSyn(src_file_name,MMIAPICOM_Wstrlen(src_file_name));
            if (!del_ret)
            {
                TRACE_JSON_PARSER("[adapter]delete config failed");
                return ADAPTER_OPERATION_FAILED;
            }
        }
        return ADAPTER_PROCESS_FINISHED;
    }

    SCI_MEMSET(str_file_name,0x00,sizeof(char)*SCI_STRLEN(str_file_name));
    SCI_MEMSET(src_file_name,0x00,sizeof(wchar)*MMIAPICOM_Wstrlen(src_file_name));

    sprintf(str_file_name, "%s\\%s\\%s",WATCHFACE_Get_UnintalledFileFolder(),p_wf_id,XIAOXUAN_JSONFILE_NAME);
    TRACE_JSON_PARSER("[adapter]file name is %s", str_file_name);

    MMIAPICOM_StrToWstr(str_file_name, src_file_name);

    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_JSON_PARSER("[adapter]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_JSON_PARSER("[adapter]handle = 0");
        return ADAPTER_PARAM_ERROR;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_JSON_PARSER("[adapter]file_size = %d",file_size);

    SFS_ReadFile(src_file_handle, p_json_cus_string, file_size,&bytes_readed, PNULL);
    if(0 != src_file_handle)
    {
        SFS_CloseFile(src_file_handle);
        src_file_handle = 0;
    }

    TRACE_JSON_PARSER("[adapter]exit");

    return ADAPTER_SUCCESS;
}
/*****************************************************************************/
//  Description :Write_PlatformJson:将表盘信息写入JSON文件
//  Parameter: [In] p_wf_id:表盘id
//             [Out]p_json_unisoc_string:对应表盘的uni JSON文件信息
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL WF_CONFIG_ADAPTER_RESULT_E Write_PlatformJson(char *p_wf_id,char *p_json_unisoc_string)
{
    SFS_HANDLE src_file_handle_write = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    uint32        bytes_writen = 0;
    uint16            path_len = 0;
    BOOLEAN            del_ret = FALSE;
    char     str_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    TRACE_JSON_PARSER("[adapter]enter");

    if(PNULL == p_json_unisoc_string)
    {
        TRACE_JSON_PARSER("[adapter]p_json_unisoc_string is NULL");
        return ADAPTER_PARAM_ERROR;
    }
    if(PNULL == p_wf_id)
    {
        TRACE_JSON_PARSER("[adapter]p_wf_id is NULL");
        return ADAPTER_PARAM_ERROR;
    }

    sprintf(str_file_name, "%s\\%s\\%s",WATCHFACE_Get_UnintalledFileFolder(),p_wf_id,UNISOC_JSONFILE_NAME);
    TRACE_JSON_PARSER("[adapter]unisoc file name is %s", str_file_name);

    MMIAPICOM_StrToWstr(str_file_name, src_file_name);

    if(TRUE == MMIFILE_IsFileExist(src_file_name,MMIAPICOM_Wstrlen(src_file_name)))
    {
        //delete
        TRACE_JSON_PARSER("[adapter]layout exist");
        del_ret = MMIAPIFMM_DeleteFileSyn(src_file_name,MMIAPICOM_Wstrlen(src_file_name));
        if (!del_ret)
        {
            TRACE_JSON_PARSER("[adapter]delete layout failed");
            return ADAPTER_PARAM_ERROR;
        }
    }

    src_file_handle_write = SFS_CreateFile (src_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_JSON_PARSER("[adapter]handle = %d",src_file_handle_write);
    if (0 == src_file_handle_write)
    {
        TRACE_JSON_PARSER("[adapter]handle = 0");
        return ADAPTER_PARAM_ERROR;
    }

    SFS_WriteFile(src_file_handle_write,p_json_unisoc_string,SCI_STRLEN(p_json_unisoc_string),&bytes_writen,PNULL);
    SFS_CloseFile(src_file_handle_write);
    src_file_handle_write = 0;

    SCI_MEMSET(str_file_name,0x00,sizeof(char)*SCI_STRLEN(str_file_name));
    SCI_MEMSET(src_file_name,0x00,sizeof(wchar)*MMIAPICOM_Wstrlen(src_file_name));

    sprintf(str_file_name, "%s\\%s\\%s",WATCHFACE_Get_InstalledFileFolder(),p_wf_id,XIAOXUAN_JSONFILE_NAME);
    TRACE_JSON_PARSER("[adapter]xiaoxuan file name is %s", str_file_name);

    MMIAPICOM_StrToWstr(str_file_name, src_file_name);

    if(TRUE == MMIFILE_IsFileExist(src_file_name,MMIAPICOM_Wstrlen(src_file_name)))
    {
        //delete
        TRACE_JSON_PARSER("[adapter]config exist");
        del_ret = MMIAPIFMM_DeleteFileSyn(src_file_name,MMIAPICOM_Wstrlen(src_file_name));
        if (!del_ret)
        {
            TRACE_JSON_PARSER("[adapter]delete config failed");
            return ADAPTER_OPERATION_FAILED;
        }
    }

    TRACE_JSON_PARSER("[adapter]exit Write_PlatformJson");
    return ADAPTER_SUCCESS;

}

/*****************************************************************************/
//  Description :JSONPARSER_XiaoxuanToUni:解析表盘某个控件的信息
//  Parameter: [In] p_json_customer_string:某个表盘的小萱JSON文件信息
//             [Out]p_json_unisoc_string:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_XiaoxuanToUni (
                                                            char*   p_json_customer_string,
                                                            char*   p_json_unisoc_string
                                                            )
{
    cJSON*          p_root = PNULL;
    cJSON*    p_background = PNULL;
    cJSON*   p_backgrounds = PNULL;
    cJSON*       p_distype = PNULL;
    cJSON*         p_month = PNULL;
    cJSON*           p_day = PNULL;
    cJSON*   p_digitaldate = PNULL;
    cJSON*          p_week = PNULL;
    cJSON*      p_multinum = PNULL;
    cJSON*      p_uni_root = PNULL;
    char*         p_string = PNULL;

    JSONPARSER_RESULT_E scale_ret       = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E background_ret  = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E backgrounds_ret = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E digital_ret     = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E analog_ret      = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E month_ret       = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E day_ret         = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E digitaldate_ret = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E week_ret        = JSON_PARSER_MAX;
    JSONPARSER_RESULT_E multinum_ret    = JSON_PARSER_MAX;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_json_customer_string)
    {
       TRACE_JSON_PARSER("[adapter]p_json_customer_string is NULL");
       return JSON_STRING_NULL;
    }

    if (PNULL == p_json_unisoc_string)
    {
       TRACE_JSON_PARSER("[adapter]p_json_unisoc_string is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_customer_string);
    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("[adapter]p_root is NULL");
       return JSON_PARSER_ERROR;
    }

    p_uni_root = cJSON_CreateObject();
    if (PNULL == p_uni_root)
    {
       TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
       cJSON_Delete(p_root);
       return JSON_PARSER_ERROR;
    }

    scale_ret = JSONPARSER_AddScale(p_uni_root);
    if (JSON_PARSER_SUCCESS != scale_ret)
    {
        TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddScale failed");
        cJSON_Delete(p_uni_root);
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    background_ret = JSONPARSER_AddBackground(p_root,p_uni_root);
    if (JSON_PARSER_SUCCESS != background_ret)
    {
        TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddBackground failed");
        cJSON_Delete(p_uni_root);
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    p_distype= cJSON_GetObjectItem(p_root, XIAOXUAN_HOUR);
    if (PNULL != p_distype)
    {
        analog_ret = JSONPARSER_AddAnalogTime(p_root,p_uni_root);
        if (JSON_PARSER_SUCCESS != analog_ret)
        {
            TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddAnalogTime failed");
            cJSON_Delete(p_uni_root);
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
    }
    else
    {
        digital_ret = JSONPARSER_AddDigitalTime(p_root,p_uni_root);
        if (JSON_PARSER_SUCCESS != digital_ret)
        {
            TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddDigitalTime failed");
            cJSON_Delete(p_uni_root);
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
    }

    p_month = cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWMONTH);
    if (PNULL != p_month)
    {
        month_ret = JSONPARSER_AddMonth(p_root, p_uni_root);
        if (JSON_PARSER_SUCCESS != month_ret)
        {
            TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddMonth failed");
            cJSON_Delete(p_uni_root);
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
    }

    digitaldate_ret = JSONPARSER_AddDigitalDate(p_root, p_uni_root);
    if (JSON_PARSER_SUCCESS != digitaldate_ret)
    {
        TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddDate failed");
        cJSON_Delete(p_uni_root);
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    p_week = cJSON_GetObjectItem(p_root,XIAOXUAN_SHOWWEEK);
    if (PNULL != p_week)
    {
        week_ret = JSONPARSER_AddWeek(p_root, p_uni_root);
        if (JSON_PARSER_SUCCESS != week_ret)
        {
            TRACE_JSON_PARSER("[adapter]JSONPARSER_ParseAddWeek failed");
            cJSON_Delete(p_uni_root);
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
    }

    multinum_ret = JSONPARSER_AddMultinum(p_root,p_uni_root);
    if (JSON_PARSER_SUCCESS != multinum_ret)
    {
        TRACE_JSON_PARSER("[adapter]JSONPARSER_AddMultinum failed");
        cJSON_Delete(p_uni_root);
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    p_string = cJSON_Print(p_uni_root);

    SCI_MEMSET(p_json_unisoc_string,0x00,SCI_STRLEN(p_json_unisoc_string));
    SCI_MEMCPY(p_json_unisoc_string,p_string,SCI_STRLEN(p_string));

    SCI_FREE(p_string);
    cJSON_Delete(p_uni_root);
    cJSON_Delete(p_root);

    TRACE_JSON_PARSER("[adapter]exit JSONPARSER_ParseXiaoxuanToUni");
    return JSON_PARSER_SUCCESS;

}

/*****************************************************************************/
//  Description :JSONPARSER_AddBackground:添加background信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddBackground (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON*  p_uni_background = PNULL;
    cJSON*        p_uni_item = PNULL;
    cJSON*      p_background = PNULL;
    cJSON*     p_backgrounds = PNULL;
    cJSON*           p_array = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_background = cJSON_CreateObject();
    if (PNULL == p_uni_background)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_background is NULL");
        return JSON_PARSER_ERROR;
    }

    p_uni_item = cJSON_CreateObject();
    if (PNULL == p_uni_item)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item is NULL");
        cJSON_Delete(p_uni_background);
        return JSON_PARSER_ERROR;
    }

    cJSON_AddNumberToObject(p_uni_item, UNISOC_X, 0.5);
    cJSON_AddNumberToObject(p_uni_item, UNISOC_Y, 0.5);

    cJSON_AddItemToObject(p_uni_background, UNISOC_CENTER_POSITION, p_uni_item);

    p_background = cJSON_GetObjectItem(p_root, XIAOXUAN_BACKGOUND);
    p_backgrounds = cJSON_GetObjectItem(p_root, XIAOXUAN_BACKGOUNDS);
    if (PNULL != p_background)
    {
        p_array = cJSON_CreateArray();
        if (PNULL == p_array)
        {
            TRACE_JSON_PARSER("[adapter]p_array is NULL");
            cJSON_Delete(p_uni_background);
            return JSON_PARSER_ERROR;
        }
        cJSON_AddItemReferenceToArray(p_array, p_background);
        cJSON_AddItemToObject(p_uni_background, UNISOC_IMAGE_LIST, p_array);

        cJSON_AddNumberToObject(p_uni_background, UNISOC_LOOPCOUNT, 0);
    }
    else if (PNULL != p_backgrounds)
    {
        cJSON_AddItemReferenceToObject(p_uni_background, UNISOC_IMAGE_LIST, p_backgrounds);
        cJSON_AddItemReferenceToObject(p_uni_background, UNISOC_LOOPCOUNT, cJSON_GetObjectItem(p_root, XIAOXUAN_LOOPCOUNT));
    }
    else
    {
        TRACE_JSON_PARSER("[adapter]background is null");
    }
    if (PNULL == cJSON_GetObjectItem(p_root, XIAOXUAN_UPDATEINTERVAL))
    {
        cJSON_AddNumberToObject(p_uni_background, UNISOC_UPDATE_INTERVAL, 100);
    }
    else
    {
        cJSON_AddItemReferenceToObject(p_uni_background, UNISOC_UPDATE_INTERVAL, cJSON_GetObjectItem(p_root, XIAOXUAN_UPDATEINTERVAL));
    }

    cJSON_AddItemToObject(p_uni_root, UNISOC_BACKGOUND, p_uni_background);

    TRACE_JSON_PARSER("[adapter]exit add background");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddDigitalTime:添加digitaltime信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDigitalTime (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON* p_uni_digitaltime = PNULL;
    cJSON* p_type            = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_digitaltime = cJSON_CreateArray();
    if (PNULL == p_uni_digitaltime)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_backgrounds is NULL");
        return JSON_PARSER_ERROR;
    }

    p_type = cJSON_GetObjectItem(p_root, XIAOXUAN_HOURPOSITIONX);
    if (PNULL != p_type)
    {
        AddDigitalTimeItem(p_root, p_uni_digitaltime, WATCHFACE_TYPE_DIGITAL_TIME_HOUR);
        AddDigitalTimeItem(p_root, p_uni_digitaltime, WATCHFACE_TYPE_DIGITAL_TIME_MIN);
    }
    else
    {

        AddDigitalTimeItem(p_root, p_uni_digitaltime, WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN);
    }

    cJSON_AddItemToObject(p_uni_root, UNISOC_DIGITALTIME, p_uni_digitaltime);

    TRACE_JSON_PARSER("[adapter]exit add digitaltime");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddAnalogTime:添加analogtime信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddAnalogTime (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON* p_uni_analogtime = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_analogtime = cJSON_CreateArray();
    if (PNULL == p_uni_analogtime)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_analogtime is NULL");
        return JSON_PARSER_ERROR;
    }

    if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_HOURSHADOW))
    {
        AddAnalogTimeItem(p_root, p_uni_analogtime, TRUE);
    }

    AddAnalogTimeItem(p_root, p_uni_analogtime, FALSE);

    cJSON_AddItemToObject(p_uni_root, UNISOC_ANALOGTIME, p_uni_analogtime);

    TRACE_JSON_PARSER("[adapter]exit add analogtime");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddMonth:添加month信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddMonth (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON* p_uni_month = PNULL;
    cJSON* p_uni_item1 = PNULL;
    cJSON* p_uni_item2 = PNULL;
    char   month[MONTH_SIZE] = {0};
    uint16 i = 0;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_month = cJSON_CreateObject();
    if (PNULL == p_uni_month)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_month is NULL");
        return JSON_PARSER_ERROR;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_uni_month);
        return JSON_PARSER_ERROR;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_uni_month);
        return JSON_PARSER_ERROR;
    }

    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_MONTHPOSITIONX));
    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_MONTHPOSITIONY));

    cJSON_AddItemToObject(p_uni_month, UNISOC_CENTER_POSITION, p_uni_item1);

    for (i = 0; i < MONTH_COUNT; i++)
    {
        sprintf(month, "month-%d.png", i + 1);
        cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString(month));
    }
    cJSON_AddItemToObject(p_uni_month, UNISOC_IMAGE_LIST, p_uni_item2);

    cJSON_AddItemToObject(p_uni_root, UNISOC_MONTH, p_uni_month);

    TRACE_JSON_PARSER("[adapter]exit add analogtime");
    return JSON_PARSER_SUCCESS;
}

#if 0
/*****************************************************************************/
//  Description :JSONPARSER_AddDay:添加day信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDay (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON*   p_uni_day = PNULL;
    cJSON* p_uni_item1 = PNULL;
    cJSON* p_uni_item2 = PNULL;
    uint16           i = 0;
    char*   suffix     = PNULL;
    char day[DAY_SIZE] = {0};

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_day = cJSON_CreateObject();
    if (PNULL == p_uni_day)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_day is NULL");
        return JSON_PARSER_ERROR;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_uni_day);
        return JSON_PARSER_ERROR;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_uni_day);
        return JSON_PARSER_ERROR;
    }

    cJSON_AddItemToObject(p_uni_day, UNISOC_DOUBLE_DIGITAL, cJSON_CreateTrue());

    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_DAYPOSITIONX));
    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_DAYPOSITIONY));
    cJSON_AddItemToObject(p_uni_day, UNISOC_CENTER_POSITION, p_uni_item1);

    suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_DAYSUFFIX));

    for (i = 0; i < DAY_COUNT; i++)
    {
        if (PNULL == suffix)
        {
            sprintf(day, "%d.png", i);
        }
        else
        {
            sprintf(day, "%d%s.png", i, suffix);
        }
        cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString(day));
    }

    cJSON_AddItemToObject(p_uni_day, UNISOC_IMAGE_LIST, p_uni_item2);

    cJSON_AddItemToObject(p_uni_root, UNISOC_DAY, p_uni_day);

    TRACE_JSON_PARSER("[adapter]exit add day");
    return JSON_PARSER_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description :JSONPARSER_AddDigitalDate:添加date信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddDigitalDate (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON*   p_uni_digitaldate = PNULL;
    BOOLEAN                ret = FALSE;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_digitaldate = cJSON_CreateArray();
    if (PNULL == p_uni_digitaldate)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_digitaldate is NULL");
        return JSON_PARSER_ERROR;
    }

    if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWDAY))
    {
        TRACE_JSON_PARSER("[adapter]Add day to digital date");
        AddDigitalDateItem(p_root, p_uni_digitaldate, WATCHFACE_TYPE_DIGITAL_DATE_DAY);
        ret = TRUE;
    }
    else if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWDATE))
    {
        TRACE_JSON_PARSER("[adapter]Add date to digital date");
        AddDigitalDateItem(p_root, p_uni_digitaldate, WATCHFACE_TYPE_DIGITAL_DATE_MD);
        ret = TRUE;
    }

    if (FALSE == ret)
    {
        TRACE_JSON_PARSER("[adapter]Digital date doesn't exist");
        cJSON_Delete(p_uni_digitaldate);
    }
    else
    {
        cJSON_AddItemToObject(p_uni_root, UNISOC_DIGITALDATE, p_uni_digitaldate);
    }

    TRACE_JSON_PARSER("[adapter]exit add digitaldate");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddWeek:添加week信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddWeek (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON*  p_uni_week = PNULL;
    cJSON* p_uni_item1 = PNULL;
    cJSON* p_uni_item2 = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_week = cJSON_CreateObject();
    if (PNULL == p_uni_week)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_week is NULL");
        return JSON_PARSER_ERROR;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_uni_week);
        return JSON_PARSER_ERROR;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_uni_week);
        return JSON_PARSER_ERROR;
    }

    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_WEEKPOSITIONX));
    cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_WEEKPOSITIONY));

    cJSON_AddItemToObject(p_uni_week, UNISOC_CENTER_POSITION, p_uni_item1);

    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("sunday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("monday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("tuesday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("wednesday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("thursday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("friday.png"));
    cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString("saturday.png"));

    cJSON_AddItemToObject(p_uni_week, UNISOC_IMAGE_LIST, p_uni_item2);

    cJSON_AddItemToObject(p_uni_root, UNISOC_WEEK, p_uni_week);

    TRACE_JSON_PARSER("[adapter]exit add week");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddMultinum:添加multinum信息
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//             [Out]p_uni_root:对应表盘的uni JSON文件信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddMultinum (
                                                            cJSON*    p_root,
                                                            cJSON*    p_uni_root
                                                    )
{
    cJSON* p_uni_multinum = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_root is NULL");
        return JSON_STRING_NULL;
    }

    p_uni_multinum = cJSON_CreateArray();
    if (PNULL == p_uni_multinum)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_multinum is NULL");
        return JSON_PARSER_ERROR;
    }

    if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWSTEP))
    {
        AddMultinumItem(p_root, p_uni_multinum, WATCHFACE_TYPE_MULTI_NUM_STEPS);
    }
    if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWHEARTRATE))
    {
        AddMultinumItem(p_root, p_uni_multinum, WATCHFACE_TYPE_MULTI_NUM_HEARTRATE);
    }
    if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_SHOWBATTERYTEXT))
    {
        AddMultinumItem(p_root, p_uni_multinum, WATCHFACE_TYPE_MULTI_NUM_POWER);
    }

    if (0 == cJSON_GetArraySize(p_uni_multinum))
    {
        TRACE_JSON_PARSER("[adapter]multinum is null");
        cJSON_Delete(p_uni_multinum);
        return JSON_PARSER_SUCCESS;
    }

    cJSON_AddItemToObject(p_uni_root, UNISOC_MULINUM, p_uni_multinum);

    TRACE_JSON_PARSER("[adapter]exit add multinum");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description :AddAnalogTimeItem:添加analogtime的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaldate type
//             [Out]p_uni_analogtime:analogtime item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddAnalogTimeItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_analogtime,
                                        BOOLEAN   is_shadow
                                    )
{
    cJSON* p_uni_item         = PNULL;
    cJSON* p_analogtime_item  = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is null");
        return FALSE;
    }

    if (PNULL == p_uni_analogtime)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_analogtime is null");
        return FALSE;
    }

    p_analogtime_item = cJSON_CreateObject();
    if (PNULL == p_analogtime_item)
    {
        TRACE_JSON_PARSER("[adapter]p_analogtime_item is null");
        return FALSE;
    }

    p_uni_item = cJSON_CreateObject();
    if (PNULL == p_uni_item)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item is NULL");
        cJSON_Delete(p_analogtime_item);
        return FALSE;
    }

    if (PNULL != is_shadow)
    {
        TRACE_JSON_PARSER("[adapter]display shadow");
        cJSON_AddNumberToObject(p_uni_item, UNISOC_X, 0.5 + cJSON_GetObjectDouble(cJSON_GetObjectItem(p_root, XIAOXUAN_SHADOWTRANSLATEX)));
        cJSON_AddNumberToObject(p_uni_item, UNISOC_Y, 0.5 + cJSON_GetObjectDouble(cJSON_GetObjectItem(p_root, XIAOXUAN_SHADOWTRANSLATEX)));
        cJSON_AddItemToObject(p_analogtime_item, UNISOC_CENTER_POSITION, p_uni_item);

        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_HOUR, cJSON_GetObjectItem(p_root, XIAOXUAN_HOURSHADOW));
        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_MINUTE, cJSON_GetObjectItem(p_root, XIAOXUAN_MINUTESHADOW));
        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_SECOND, cJSON_GetObjectItem(p_root, XIAOXUAN_SECONDSHADOW));
    }
    else
    {
        TRACE_JSON_PARSER("[adapter]display time");
        cJSON_AddNumberToObject(p_uni_item, UNISOC_X, 0.5);
        cJSON_AddNumberToObject(p_uni_item, UNISOC_Y, 0.5);
        cJSON_AddItemToObject(p_analogtime_item, UNISOC_CENTER_POSITION, p_uni_item);

        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_HOUR, cJSON_GetObjectItem(p_root, XIAOXUAN_HOUR));
        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_MINUTE, cJSON_GetObjectItem(p_root, XIAOXUAN_MINUTE));
        cJSON_AddItemReferenceToObject(p_analogtime_item, UNISOC_IMAGE_SECOND, cJSON_GetObjectItem(p_root, XIAOXUAN_SECOND));
    }

    cJSON_AddNumberToObject(p_analogtime_item, UNISOC_UPDATE_INTERVAL, ANALOG_UPDATE);

    cJSON_AddItemToArray(p_uni_analogtime, p_analogtime_item);

    TRACE_JSON_PARSER("[adapter]exit AddAnalogTimeItem");
    return TRUE;
}
/*****************************************************************************/
//  Description :AddDigitalTimeItem:添加digitaltime的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaltime type
//             [Out]p_uni_digitaltime:digitaltime item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddDigitalTimeItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_digitaltime,
                          WATCHFACE_ALL_TYPE_E    type
                                    )
{
    uint16 i                 = 0;
    char   time[TIME_SIZE]   = {0};
    char*  suffix            = PNULL;
    cJSON* p_uni_item1       = PNULL;
    cJSON* p_uni_item2       = PNULL;
    cJSON* p_digital_item    = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is null");
        return FALSE;
    }

    if (PNULL == p_uni_digitaltime)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_digitaltime is null");
        return FALSE;
    }

    p_digital_item = cJSON_CreateObject();
    if (PNULL == p_digital_item)
    {
        TRACE_JSON_PARSER("[adapter]p_digital_item is null");
        return FALSE;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_digital_item);
        return FALSE;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_digital_item);
        return FALSE;
    }

    TRACE_JSON_PARSER("[adapter]digitaltime type is %d", type);
    switch(type)
    {
        case WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN:
        {
            cJSON_AddStringToObject(p_digital_item, UNISOC_TYPE, UNISOC_HOURMIN);
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_TIMESUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_TIMEPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_TIMEPOSITIONY));
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_HOUR:
        {
            cJSON_AddStringToObject(p_digital_item, UNISOC_TYPE, UNISOC_HOUR);
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_HOURSUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_HOURPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_HOURPOSITIONY));
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_MIN:
        {
            cJSON_AddStringToObject(p_digital_item, UNISOC_TYPE, UNISOC_MIN);
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_MINUTESUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_MINUTEPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_MINUTEPOSITIONY));
            break;
        }
        default:
        {
            TRACE_JSON_PARSER("[adapter]digitaltime type is error");
            break;
        }
    }

    cJSON_AddItemToObject(p_digital_item, UNISOC_CENTER_POSITION, p_uni_item1);

    for (i = 0; i < MULTINUM_COUNT; i++)
    {
        if (PNULL == suffix)
        {
            sprintf(time, "%d.png", i);
        }
        else
        {
            sprintf(time, "%d%s.png", i, suffix);
        }
        cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString(time));
    }
    cJSON_AddItemToObject(p_digital_item, UNISOC_IMAGE_LIST, p_uni_item2);

    cJSON_AddStringToObject(p_digital_item, UNISOC_IMAGE_CONNECTOR, XIAOXUAN_COLON);

    cJSON_AddNumberToObject(p_digital_item, UNISOC_UPDATE_INTERVAL, DIGITAL_UPDATE);

    cJSON_AddItemToArray(p_uni_digitaltime, p_digital_item);

    TRACE_JSON_PARSER("[adapter]exit AddDigitalTimeItem");
    return TRUE;
}

/*****************************************************************************/
//  Description :AddDigitalDateItem:添加digitaldate的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：digitaldate type
//             [Out]p_uni_digitaldate:digitaldate item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddDigitalDateItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_digitaldate,
                          WATCHFACE_ALL_TYPE_E    type
                                    )
{
    uint16 i = 0;
    char   date[DATE_SIZE]   = {0};
    char*  suffix            = PNULL;
    cJSON* p_uni_item1       = PNULL;
    cJSON* p_uni_item2       = PNULL;
    cJSON* p_digital_item    = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is null");
        return FALSE;
    }

    if (PNULL == p_uni_digitaldate)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_digitaldate is null");
        return FALSE;
    }

    p_digital_item = cJSON_CreateObject();
    if (PNULL == p_digital_item)
    {
        TRACE_JSON_PARSER("[adapter]p_digital_item is null");
        return FALSE;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_digital_item);
        return FALSE;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_digital_item);
        return FALSE;
    }

    TRACE_JSON_PARSER("[adapter]digitaldate type is %d", type);
    switch(type)
    {
        case WATCHFACE_TYPE_DIGITAL_DATE_DAY:
        {
            cJSON_AddStringToObject(p_digital_item, UNISOC_TYPE, UNISOC_DATE_DAY);
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_DAYSUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_DAYPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_DAYPOSITIONY));
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_MD:
        {
            cJSON_AddStringToObject(p_digital_item, UNISOC_TYPE, UNISOC_DATE_MONTHDAY);
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_DATESUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_DATEPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_DATEPOSITIONY));
            break;
        }
        default:
        {
            TRACE_JSON_PARSER("[adapter]digitaldate type is error");
            break;
        }
    }

    cJSON_AddItemToObject(p_digital_item, UNISOC_CENTER_POSITION, p_uni_item1);

    for (i = 0; i < DATE_COUNT; i++)
    {
        if (PNULL == suffix)
        {
            sprintf(date, "%d.png", i);
        }
        else
        {
            sprintf(date, "%d%s.png", i, suffix);
        }
        cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString(date));
    }
    cJSON_AddItemToObject(p_digital_item, UNISOC_IMAGE_LIST, p_uni_item2);

    if (WATCHFACE_TYPE_DIGITAL_DATE_MD == type)
    {
        cJSON_AddItemReferenceToObject(p_digital_item, UNISOC_IMAGE_CONNECTOR, cJSON_GetObjectItem(p_root, XIAOXUAN_DATE_MONTHIMG));
    }
    else if (WATCHFACE_TYPE_DIGITAL_DATE_DAY == type)
    {
        cJSON_AddStringToObject(p_digital_item, UNISOC_IMAGE_CONNECTOR, UNISOC_NONE);
    }
    cJSON_AddItemToArray(p_uni_digitaldate, p_digital_item);

    TRACE_JSON_PARSER("[adapter]exit AddDigitalDateItem");
    return TRUE;
}

/*****************************************************************************/
//  Description :AddMultinumItem:添加multinum的item
//  Parameter: [In] p_root:某个表盘的小萱JSON文件信息
//                  type  ：multinum type
//             [Out]p_uni_multinum:multinum item
//             [Return] BOOLEAN
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddMultinumItem(
                                        cJSON*    p_root,
                                        cJSON*    p_uni_multinum,
                          WATCHFACE_ALL_TYPE_E    type
                                    )
{
    uint16 i = 0;
    char   multi[MULTINUM_SIZE]   = {0};
    char*  suffix            = PNULL;
    cJSON* p_uni_item1       = PNULL;
    cJSON* p_uni_item2       = PNULL;
    cJSON* p_multinum_item   = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is null");
        return FALSE;
    }

    if (PNULL == p_uni_multinum)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_multinum is null");
        return FALSE;
    }

    p_multinum_item = cJSON_CreateObject();
    if (PNULL == p_multinum_item)
    {
        TRACE_JSON_PARSER("[adapter]p_multinum_item is null");
        return FALSE;
    }

    p_uni_item1 = cJSON_CreateObject();
    if (PNULL == p_uni_item1)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item1 is NULL");
        cJSON_Delete(p_multinum_item);
        return FALSE;
    }

    p_uni_item2 = cJSON_CreateArray();
    if (PNULL == p_uni_item2)
    {
        TRACE_JSON_PARSER("[adapter]p_uni_item2 is NULL");
        cJSON_Delete(p_uni_item1);
        cJSON_Delete(p_multinum_item);
        return FALSE;
    }

    TRACE_JSON_PARSER("[adapter]multinum type is %d", type);
    switch(type)
    {
        case WATCHFACE_TYPE_MULTI_NUM_STEPS:
        {
            cJSON_AddStringToObject(p_multinum_item, UNISOC_DATA_TYPE, "steps");
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_STEPSUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_STEPPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_STEPPOSITIONY));
            if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_STEPIMGNONE))
            {
                cJSON_AddItemReferenceToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_GetObjectItem(p_root, XIAOXUAN_STEPIMGNONE));
            }
            else
            {
                cJSON_AddItemToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_CreateString(UNISOC_NONE));
            }
            break;
        }
        case WATCHFACE_TYPE_MULTI_NUM_HEARTRATE:
        {
            cJSON_AddStringToObject(p_multinum_item, UNISOC_DATA_TYPE, "pulse");
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_HEARTRATESUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_HEARTRATEPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_HEARTRATEPOSITIONY));
            if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_HEARTRATEIMGNONE))
            {
                cJSON_AddItemReferenceToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_GetObjectItem(p_root, XIAOXUAN_HEARTRATEIMGNONE));
            }
            else
            {
                cJSON_AddItemToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_CreateString(UNISOC_NONE));
            }
            break;
        }
        case WATCHFACE_TYPE_MULTI_NUM_POWER:
        {
            cJSON_AddStringToObject(p_multinum_item, UNISOC_DATA_TYPE, "power");
            suffix = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, XIAOXUAN_BATTERYTEXTSUFFIX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_X, cJSON_GetObjectItem(p_root, XIAOXUAN_BATTERYTEXTPOSITIONX));
            cJSON_AddItemReferenceToObject(p_uni_item1, UNISOC_Y, cJSON_GetObjectItem(p_root, XIAOXUAN_BATTERYTEXTPOSITIONY));
            if (PNULL != cJSON_GetObjectItem(p_root, XIAOXUAN_BATTERYTEXTIMGNONE))
            {
                cJSON_AddItemReferenceToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_GetObjectItem(p_root, XIAOXUAN_BATTERYTEXTIMGNONE));
            }
            else
            {
                cJSON_AddItemToObject(p_multinum_item, UNISOC_IMAGE_NONE, cJSON_CreateString(UNISOC_NONE));
            }
            break;
        }
        default:
        {
            TRACE_JSON_PARSER("[adapter]multinum type is error");
            break;
        }
    }

    cJSON_AddItemToObject(p_multinum_item, UNISOC_CENTER_POSITION, p_uni_item1);

    for (i = 0; i < 10; i++)
    {
        if (PNULL == suffix)
        {
            sprintf(multi, "%d.png", i);
        }
        else
        {
            sprintf(multi, "%d%s.png", i, suffix);
        }
        cJSON_AddItemToArray(p_uni_item2,cJSON_CreateString(multi));
    }
    cJSON_AddItemToObject(p_multinum_item, UNISOC_IMAGE_LIST, p_uni_item2);

    cJSON_AddItemToArray(p_uni_multinum, p_multinum_item);

    TRACE_JSON_PARSER("[adapter]exit AddMultinumItem");
    return TRUE;

}
/*****************************************************************************/
//  Description :CalculatorScaleValue:计算scale值
//  Parameter: [In] p_wf_id,watchface id
//             [Out]
//             [Return] 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculatorScaleValue(char* p_wf_id)
{
    char str_file_name_path[WATCHFACE_FILES_FULLPATH_LEN + 1]  = {0};
    wchar wstr_file_name_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    uint16  img_width  = 0;
    uint16  img_height = 0;
    uint16  path_len   = 0;

    uint16  lcd_width  = 0;
    uint16  lcd_height = 0;

    if(PNULL == p_wf_id)
    {
        TRACE_JSON_PARSER("[adapter]p_wf_id is null");
        return FALSE;
    }

    sprintf(str_file_name_path, "%s\\%s\\%s",WATCHFACE_Get_UnintalledFileFolder(),p_wf_id,XIAOXUAN_PREVIEW_NAME);
    MMIAPICOM_StrToWstr(str_file_name_path, wstr_file_name_path);

    path_len = MMIAPICOM_Wstrlen(wstr_file_name_path);
    TRACE_JSON_PARSER("[adapter]path_len is %d",path_len);
    if(TRUE == MMIFILE_IsFileExist(wstr_file_name_path,path_len))
    {
        TRACE_JSON_PARSER("[adapter]file exist ");
    }
    else
    {
        TRACE_JSON_PARSER("[adapter]file is not exist ");
        return FALSE;
    }

    CTRLANIM_GetImgWidthHeight(&img_width,&img_height,wstr_file_name_path,path_len);
    TRACE_JSON_PARSER("[adapter]img_width=%d,img_height=%d",img_width,img_height);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    TRACE_JSON_PARSER("[adapter]img_width=%d,img_height=%d,lcd_width=%d,",img_width,img_height,lcd_width);

    if(0 < img_width)
    {
        s_scale_value = (float)lcd_width/(float)img_width;
        TRACE_JSON_PARSER("[adapter]s_scale_value=%f",s_scale_value);
    }
    else
    {
        TRACE_JSON_PARSER("[adapter]*s_scale_value error");
        s_scale_value = 1.0;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :JSONPARSER_AddScale:添加scale值
//  Parameter: [In]: p_uni_root:watchface layout 根标签
//             [Out]:
//             [Return] JSON_PARSER_RESULT_E
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E JSONPARSER_AddScale (cJSON* p_uni_root )
{
    cJSON* json_scale = PNULL;

    TRACE_JSON_PARSER("[adapter]enter");

    if (PNULL == p_uni_root)
    {
        TRACE_JSON_PARSER("[adapter]p_root is NULL");
        return JSON_STRING_NULL;
    }

    cJSON_AddNumberToObject(p_uni_root,UNISOC_SCALE, s_scale_value);

    TRACE_JSON_PARSER("[adapter]exit add scale");
    return JSON_PARSER_SUCCESS;
}

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description :JSONPARSER_ParseAdaptation:小萱JSON文件适配
//  Parameter: [In] p_wf_id:表盘id
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN JSONPARSER_Adaptation(char* p_wf_id)
{
    char*  p_customer_string = PNULL;
    char*    p_unisoc_string = PNULL;
    BOOLEAN              ret = FALSE;
    WF_CONFIG_ADAPTER_RESULT_E   get_ret = ADAPTER_MAX;
    WF_CONFIG_ADAPTER_RESULT_E write_ret = ADAPTER_MAX;
    JSONPARSER_RESULT_E       parser_ret = JSON_PARSER_MAX;

    if (PNULL == p_wf_id)
    {
        TRACE_JSON_PARSER("[adapter]p_wf_id is NULL");
        return ret;
    }

    TRACE_JSON_PARSER("[adapter]watchface id is %s", p_wf_id);

    p_customer_string = SCI_ALLOC_APPZ(UNISOC_JSONFILE_SIZE);
    if (PNULL == p_customer_string)
    {
        TRACE_JSON_PARSER("[adapter]p_customer_string is NULL");
        return ret;
    }

    p_unisoc_string = SCI_ALLOC_APPZ(UNISOC_JSONFILE_SIZE);
    if (PNULL == p_unisoc_string)
    {
        TRACE_JSON_PARSER("[adapter]p_unisoc_string is NULL");
        SCI_FREE(p_customer_string);
    }

    get_ret = Get_CustomerJsonString(p_wf_id, p_customer_string);
    TRACE_JSON_PARSER("[adapter]Get_CustomerJsonString result is %d", get_ret);

    if (ADAPTER_PROCESS_FINISHED == get_ret || ADAPTER_OPERATION_FAILED == get_ret)
    {
        TRACE_JSON_PARSER("[adapter]layout exist");
        ret = TRUE;
    }
    else if (ADAPTER_PARAM_ERROR == get_ret)
    {
        TRACE_JSON_PARSER("[adapter]Get_CustomerJsonString error");
    }
    else
    {
        CalculatorScaleValue(p_wf_id);

        parser_ret = JSONPARSER_XiaoxuanToUni(p_customer_string, p_unisoc_string);
        TRACE_JSON_PARSER("[adapter]JSONPARSER_XiaoxuanToUni result is %d", parser_ret);

        if (JSON_PARSER_SUCCESS!= parser_ret)
        {
            TRACE_JSON_PARSER("[adapter]JSONPARSER_XiaoxuanToUni failed");
        }
        else
        {
            write_ret = Write_PlatformJson(p_wf_id, p_unisoc_string);
            TRACE_JSON_PARSER("[adapter]Write_PlatformJson result is %d", write_ret);
            if (ADAPTER_PARAM_ERROR == write_ret)
            {
                TRACE_JSON_PARSER("[adapter]Write_PlatformJson failed");
            }
            else
            {
                ret = TRUE;
            }
        }
    }

    if (PNULL != p_unisoc_string)
    {
        SCI_FREE(p_unisoc_string);
    }

    if (PNULL != p_customer_string)
    {
        SCI_FREE(p_customer_string);
    }

    TRACE_JSON_PARSER("[adapter]exit");
    return ret;
}
