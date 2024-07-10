/****************************************************************************
** File Name:      watchface_flow.c                                         *
** Author:                                                                  *
** Date:           2021.8.23                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to do provide network access api       *
*****************************************************************************
**                         Important Edit History                           *
** --------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                  *
** 2021.8.23      xiaoju.cheng                 Create
**
****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "os_param.h"
#include "json_parser_watchface.h"
#include "watchface_flow.h"
#include "sfs.h"
#include "window_parse.h"
#include "mmicom_trace.h"
#include "watchface_ctrl_adapter.h"
#include "watchface_config.h"
#include "watchface_multifunctional_dial.h"
#include "mmiudisk_export.h"

/*---------------------------------------------------------------------------*/
/*                        Macro Define                                       */
/*---------------------------------------------------------------------------*/
#define MAX_ITEM_COUNT            (20)
#define MAX_OBJECT_COUNT          (15)

/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/
LOCAL WATCHFACE_CTRL_DESTROY_INFO_T s_object_destroy[MAX_ITEM_COUNT] = {0};
LOCAL WATCHFACE_CTRL_CREATE_LIST_T*  s_p_ctrl_create_info_list = PNULL;//layout中解析出的信息
LOCAL char s_current_watchface_path[MAX_PATH_LEN] = {0};


LOCAL WATCHFACE_OBJECT_ITEM_ST s_watchface_object_item[] =
{
    {"IMG",             WATCHFACE_Img_Create,           NULL,                           NULL,                       WATCHFACE_Img_Destory},
    {"Background",      WATCHFACE_Background_Create,    CTRLWF_Bg_StopTimer,            CTRLWF_Bg_RestartTimer,           WATCHFACE_Background_Destory},
    {"AnalogTime",      WATCHFACE_AnalogTime_Create,    CTRLWF_AnalogTime_StopTimer,    CTRLWF_AnalogTime_RestartTimer,   WATCHFACE_AnalogTime_Destory},
    {"DigitalTime",     WATCHFACE_DigitalTime_Create,   CTRLWF_DigitalTime_StopTimer,   CTRLWF_DigitalTime_RestartTimer,  WATCHFACE_DigitalTime_Destory},
    {"Month",           WATCHFACE_Month_Create,         CTRLWF_Month_StopTimer,         CTRLWF_Month_RestartTimer,        WATCHFACE_Month_Destory},
    {"Day",             WATCHFACE_Day_Create,           CTRLWF_Day_StopTimer,           CTRLWF_Day_RestartTimer,          WATCHFACE_Day_Destory},
    {"Week",            WATCHFACE_Week_Create,          CTRLWF_Week_StopTimer,          CTRLWF_Week_RestartTimer,         WATCHFACE_Week_Destory},
    {"MultiNum",        WATCHFACE_MultiNum_Create,      CTRLWF_MultiNum_StopTimer,      CTRLWF_MultiNum_RestartTimer,     WATCHFACE_MultiNum_Destory},
    {"DigitalDate",     WATCHFACE_DigitalDate_Create,   CTRLWF_DigitalDate_StopTimer,   CTRLWF_DigitalDate_RestartTimer,  WATCHFACE_DigitalDate_Destory},
};
LOCAL WATCHFACE_ALL_TYPE_T s_watchface_typetable[]=
{
    WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN,       "hourmin",
    WATCHFACE_TYPE_DIGITAL_TIME_HOUR,          "hour",
    WATCHFACE_TYPE_DIGITAL_TIME_MIN,           "min",
    WATCHFACE_TYPE_DIGITAL_DATE_YEAR,          "year",
    WATCHFACE_TYPE_DIGITAL_DATE_MONTH,         "month",
    WATCHFACE_TYPE_DIGITAL_DATE_DAY,           "day",
    WATCHFACE_TYPE_DIGITAL_DATE_MD,            "md",
    WATCHFACE_TYPE_DIGITAL_DATE_YM,            "ym",
    WATCHFACE_TYPE_DIGITAL_DATE_YMD,           "ymd",
    WATCHFACE_TYPE_MULTI_NUM_STEPS,            "steps",
    WATCHFACE_TYPE_MULTI_NUM_HEARTRATE,        "pulse",
    WATCHFACE_TYPE_MULTI_NUM_POWER,            "power",
};
/*****************************************************************************/
//  Description :获取s_p_ctrl_create_info_list
//  Parameter: [In] None
//             [Out] None
//             [Return] s_p_ctrl_create_info_list
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_CTRL_CREATE_LIST_T* WATCHFACE_Get_Ctrl_Create_List(void)
{
    TRACE_WATCHFACE("[watchface_flow]: enter");
    if(PNULL ==s_p_ctrl_create_info_list)
    {
        TRACE_WATCHFACE("[watchface_flow]:s_p_ctrl_create_info_list is PNULL");
        return PNULL;
    }
    return (const WATCHFACE_CTRL_CREATE_LIST_T*)s_p_ctrl_create_info_list;
}
/*****************************************************************************/
//  Description :get s_watchface_object_item
//  Parameter: [In] None
//             [Out] None
//             [Return] s_watchface_object_item
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void WATCHFACE_Get_Object_Item_Array(WATCHFACE_OBJECT_ITEM_ST* pTemp)
{
    if(PNULL == pTemp)
    {
        TRACE_WATCHFACE("[watchface_flow]: pTemp is null.");
        return;
    }
    SCI_MEMCPY(pTemp, s_watchface_object_item,sizeof(s_watchface_object_item));
}

/*****************************************************************************/
//  Description : watchface fullpaint
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
LOCAL void WatchFace_Generate_Path (char* p_image_name, char* p_full_path)
{
    char    src_file_name[MAX_PATH_LEN] = {0};
    uint16  i = 0;

    if(PNULL == p_image_name)
    {
        TRACE_WATCHFACE("[watchface_flow]:p_image_name is PNULL");
        return;
    }

    if(PNULL == p_full_path)
    {
        TRACE_WATCHFACE("[watchface_flow]:p_full_path is PNULL");
        return;
    }

    WATCHFACE_Get_CurrentWatchfaceFile_Path(src_file_name);

    for(i=strlen(src_file_name)-1;i>0;i--)
    {
        if('\\' == src_file_name[i])
        {
            SCI_MEMCPY(p_full_path,src_file_name,i+1);
            break;
        }
    }
    strcat(p_full_path,p_image_name);
    TRACE_WATCHFACE("[watchface_flow]:p_full_path is %s",p_full_path);
}
LOCAL void WatchFace_GetLayoutInfo(
                                    char*                       p_watchface_path,
                                    uint8*                      object_num,
                                    double*                     scale,
                                    WATCHFACE_OBJECT_LIST_T*    p_object_list[]
                                    )
{
    SFS_HANDLE    src_fmm_handle = 0;
    uint32  file_size = 0;
    uint32  bytes_readed = 0;
    uint8   obj_num = 0;
    char*   buf = PNULL;
    char    src_file_name[MAX_PATH_LEN] = {0};
    wchar   wsrc_file_name[MAX_PATH_LEN] = {0};
    JSONPARSER_RESULT_E json_result = JSON_PARSER_MAX;

    if(PNULL == p_watchface_path)
    {
        TRACE_WATCHFACE("[watchface_flow]:path is PNULL");
        return;
    }
    if(PNULL == p_object_list)
    {
        TRACE_WATCHFACE("[watchface_flow]:p_object_list is not PNULL");
        return;
    }

    MMIAPICOM_StrToWstr((uint8*)p_watchface_path, wsrc_file_name);

    src_fmm_handle = SFS_CreateFile(wsrc_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read
    if(0 == src_fmm_handle)
    {
        TRACE_WATCHFACE("[watchface_flow]:src_fmm_handle is NULL");
        return;
    }

    SFS_GetFileSize(src_fmm_handle, &file_size);

    TRACE_WATCHFACE("[watchface_flow]:file_size = %d",file_size);
    buf = SCI_ALLOC_APPZ(file_size);
    if(PNULL == buf)
    {
        TRACE_WATCHFACE("[watchface_flow]:buf is NULL");
        if(0 != src_fmm_handle )
        {
            SFS_CloseFile( src_fmm_handle );
        }
        return;
    }

    SFS_ReadFile(src_fmm_handle, buf, file_size,&bytes_readed, PNULL);

    JSONPARSER_GetWatchfaceAllObjectInfo(buf, &obj_num, scale, p_object_list);
    *object_num = obj_num;
    if(0 != src_fmm_handle )
    {
        SFS_CloseFile( src_fmm_handle );
    }
    if(PNULL != buf)
    {
        SCI_FREE(buf);
    }
}
/*****************************************************************************/
//  Description :申请全局变量s_p_ctrl_create_info_list的空间
//  Parameter: [In] item_num
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WatchFace_Data_Alloc (uint8  item_num)
{
    uint8 i = 0;
    //malloc list
    s_p_ctrl_create_info_list = (WATCHFACE_CTRL_CREATE_LIST_T *)SCI_ALLOC_APPZ(sizeof(WATCHFACE_CTRL_CREATE_LIST_T));
    if(PNULL == s_p_ctrl_create_info_list)
    {
        TRACE_WATCHFACE("malloc list space fail");
        return FALSE;
    }
    SCI_MEMSET(s_p_ctrl_create_info_list, 0x00, sizeof(WATCHFACE_CTRL_CREATE_LIST_T));
    //molloc p_ctrl_create_info
    s_p_ctrl_create_info_list->p_ctrl_create_info = (WATCHFACE_CTRL_CREATE_INFO_T *)SCI_ALLOC_APPZ(item_num * sizeof(WATCHFACE_CTRL_CREATE_INFO_T));
    if(PNULL == s_p_ctrl_create_info_list->p_ctrl_create_info)
    {
        SCI_FREE(s_p_ctrl_create_info_list);
        s_p_ctrl_create_info_list = PNULL;
        TRACE_WATCHFACE("malloc p_ctrl_create_info space fail");
        return FALSE;
    }
    SCI_MEMSET(s_p_ctrl_create_info_list->p_ctrl_create_info, 0x00, item_num * sizeof(WATCHFACE_CTRL_CREATE_INFO_T));
    for(i = 0;i<item_num; i++)
    {
        s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param = SCI_ALLOC_APPZ(sizeof(WATCHFACE_CTRL_PARAM_T));
        if(PNULL == s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param)
        {
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info);
            s_p_ctrl_create_info_list->p_ctrl_create_info = PNULL;
            SCI_FREE(s_p_ctrl_create_info_list);
            s_p_ctrl_create_info_list = PNULL;
            TRACE_WATCHFACE("malloc p_ctrl_param space fail");
            return FALSE;
        }
        SCI_MEMSET(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param, 0x00, sizeof(WATCHFACE_CTRL_PARAM_T));
        s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_connector = SCI_ALLOC_APPZ(MAX_PATH_LEN);
        s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_none = SCI_ALLOC_APPZ(MAX_PATH_LEN);
        if(PNULL == s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_connector ||
            PNULL == s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_none)
        {
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param);
            s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param = PNULL;
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info);
            s_p_ctrl_create_info_list->p_ctrl_create_info = PNULL;
            SCI_FREE(s_p_ctrl_create_info_list);
            s_p_ctrl_create_info_list = PNULL;
            return FALSE;
        }
        SCI_MEMSET(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_connector,0x00, MAX_PATH_LEN);
        SCI_MEMSET(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_none,0x00, MAX_PATH_LEN);
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :释放全局变量s_p_ctrl_create_info_list的空间
//  Parameter: [In] item_num
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void WatchFace_Ctrl_Create_Info_List_Free()
{
    uint8 i = 0;
    uint8 j = 0;
    if(PNULL != s_p_ctrl_create_info_list)
    {
        for(i = 0; i < s_p_ctrl_create_info_list->item_number; i++)
        {
            for(j=0; PNULL != s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_list[j]; j++)
            {
                SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_list[j]);
                s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_list[j] = PNULL;
            }
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_connector);
            s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_connector = PNULL;
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_none);
            s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param->p_image_none = PNULL;
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param);
            s_p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param = PNULL;
        }
        if(PNULL != s_p_ctrl_create_info_list->p_ctrl_create_info)
        {
            SCI_FREE(s_p_ctrl_create_info_list->p_ctrl_create_info);
            s_p_ctrl_create_info_list->p_ctrl_create_info = PNULL;
        }
        if(PNULL != s_p_ctrl_create_info_list)
        {
            SCI_FREE(s_p_ctrl_create_info_list);
            s_p_ctrl_create_info_list = PNULL;
        }
    }
}
LOCAL void WatchFace_Object_List_Free(WATCHFACE_OBJECT_LIST_T*  p_object_list[])
{
    uint8   i = 0;
    for(i = 0;i<MAX_OBJECT_COUNT; i++)
    {
        if (PNULL != p_object_list[i])
        {
            JSONPARSER_FreeObjectList(&p_object_list[i]);
            p_object_list[i] = PNULL; //bug 1958580 CID 589440
        }
    }
}
/************************************************************************************************/
//  Description : WATCHFACE_Construct_Layout_Data:解析表盘layout数据，并存到全局变量s_p_display_info里
//  Parameter: [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/************************************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Construct_Layout_Data(void)
{
    char    watchface_path[MAX_PATH_LEN] = {0};
    BOOLEAN alloc_result = TRUE;
    WATCHFACE_OBJECT_COMMON_T* p_temp_object = PNULL;
    WATCHFACE_OBJECT_LIST_T*   p_temp_object_list[MAX_OBJECT_COUNT] = {PNULL};
    WATCHFACE_OBJECT_ITEM_ST   watchface_object_item[MAX_OBJECT_COUNT] = {0};
    uint8   object_num = 0;
    double  scale = 0;
    uint8   item_number = 0;
    uint8   display_num = 0;
    uint8   arr_num = 0;
    uint8   i = 0;
    uint32   j = 0;
    uint32   k = 0;//bug 1958580 CID 589723

    for (i = 0; i < MAX_OBJECT_COUNT; i++)
    {
        p_temp_object_list[i] = PNULL;
    }
    WATCHFACE_Get_CurrentWatchfaceFile_Path(watchface_path);
    TRACE_WATCHFACE("[watchface_flow]:watchface_path=%s",watchface_path);

    WatchFace_GetLayoutInfo(watchface_path, &object_num, &scale, p_temp_object_list);
    TRACE_WATCHFACE("[watchface_flow]:object_num=%d,scale=%lf", object_num, scale);

    for(i = 0;i<object_num; i++)
    {
        item_number = item_number + p_temp_object_list[i]->item_num;
    }
    TRACE_WATCHFACE("[watchface_flow]:item_num=%d", item_number);
    WatchFace_Ctrl_Create_Info_List_Free();
    alloc_result = WatchFace_Data_Alloc(item_number);
    if(FALSE == alloc_result)
    {
        WatchFace_Object_List_Free(p_temp_object_list);
        TRACE_WATCHFACE("[watchface_flow]:s_p_display_info[%d] alloc fail.",item_number);//bug 1958580 CID 588949
        return FALSE;
    }
    s_p_ctrl_create_info_list->item_number = item_number;
    for(i = 0;i<object_num; i++)
    {
        if(PNULL == p_temp_object_list[i]->p_object_info)
        {
            WatchFace_Object_List_Free(p_temp_object_list);
            WatchFace_Ctrl_Create_Info_List_Free();
            TRACE_WATCHFACE("[watchface_flow]:p_temp_object_list[%d]->p_object_info is pnull.",i);//bug 1958580 CID 588949
            return FALSE;
        }
        p_temp_object = p_temp_object_list[i]->p_object_info;
        for(j = 0; j<p_temp_object_list[i]->item_num; j++)
        {
            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->center_position.ratio_x = p_temp_object[j].center_position.ratio_x;
            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->center_position.ratio_y = p_temp_object[j].center_position.ratio_y;
            TRACE_WATCHFACE("[watchface_flow]:ratio_x=%lf, ratio_y=%lf",s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->center_position.ratio_x,
            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->center_position.ratio_y);
            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->double_digital = p_temp_object[j].digit_capacity;
            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->update_interval =  p_temp_object[j].update_interval;
            TRACE_WATCHFACE("[watchface_flow]:double_digital=%d, update_interval=%d",s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->double_digital, s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->update_interval);
            //获取image_connector,image_none的全路径
            WatchFace_Generate_Path(p_temp_object[j].image_connector, s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_connector);
            TRACE_WATCHFACE("[watchface_flow]:image connector path = %s",s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_connector);
            WatchFace_Generate_Path(p_temp_object[j].image_none, s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_none);
            TRACE_WATCHFACE("[watchface_flow]:image none path = %s",s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_none);

            s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->scale = (float)scale;
            TRACE_WATCHFACE("[watchface_flow]:scale = %lf", s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->scale);
            for (k = 0; k<WATCHFACE_TYPE_MAX; k++)
            {
                if(0 == strcmp(p_temp_object[j].type, s_watchface_typetable[k].type_str))
                {
                    s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->type = s_watchface_typetable[k].type;
                    TRACE_WATCHFACE("watchface object type=%d",s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->type);
                    break;
                }
            }
            WATCHFACE_Get_Object_Item_Array(watchface_object_item);
            for (k=0; k<MAX_OBJECT_COUNT; k++)
            {
                if(0 == strcmp(p_temp_object_list[i]->object_name, watchface_object_item[k].object_name))
                {
                    s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].object_callback = watchface_object_item[k].object_callback;
                    break;
                }
            }
            k = 0;
            while(PNULL != p_temp_object[j].p_image_list[k])
            {
                s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_list[k] = SCI_ALLOC_APPZ(MAX_PATH_LEN);
                if(PNULL ==  s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_list[k])
                {
                    WatchFace_Object_List_Free(p_temp_object_list);
                    WatchFace_Ctrl_Create_Info_List_Free();
                    TRACE_WATCHFACE("[watchface_flow]:p_image_list[%d] alloc fail.",k);//bug 1958580 CID 588949
                    return FALSE;
                }
                SCI_MEMSET(s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_list[k],0x00, MAX_PATH_LEN);
                WatchFace_Generate_Path(p_temp_object[j].p_image_list[k] ,s_p_ctrl_create_info_list->p_ctrl_create_info[display_num].p_ctrl_param->p_image_list[k]);
                k ++;
            }
            display_num++;
        }

    }

    WatchFace_Object_List_Free(p_temp_object_list);
    return TRUE;
}
/*****************************************************************************/
//  Description : watchface fullpaint(destory,create 控件)
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_FullPaint(void)
{
    uint8 i = 0;
    uint8 j = 0;
    uint8 arr_num = 0;
    BOOLEAN is_change_multi = FALSE;
    WATCHFACE_CTRL_CREATE_LIST_T* p_ctrl_create_info_list = PNULL;
    char watchface_path[MAX_PATH_LEN] = {0};

    WATCHFACE_Get_CurrentWatchfaceFile_Path(watchface_path);
    TRACE_WATCHFACE("[watchface_flow]:watchface_path=%s",watchface_path);
 #ifdef WATCHFACE_CTRL_EDIT_SUPPORT
    is_change_multi = WATCHFACE_Get_Is_Change_Multi();
 #else
     is_change_multi = FALSE;
 #endif
    arr_num = ARR_SIZE(s_watchface_object_item);
    if(0 == strcmp(s_current_watchface_path,watchface_path))
    {
        if(is_change_multi == TRUE)
        {
            is_change_multi = FALSE;
  #ifdef WATCHFACE_CTRL_EDIT_SUPPORT
            WATCHFACE_Set_Is_Change_Multi(is_change_multi);
  #endif
        }
        else
        {
            TRACE_WATCHFACE("[watchface_flow]:s_p_ctrl_create_info_list is not change.");
            for(i=0;i<MAX_ITEM_COUNT;i++)
            {
                if(0 != s_object_destroy[i].handle) 
                {
                    for(j = 0;j < arr_num;j++)
                    {
                        if(s_object_destroy[i].destroy_callback == s_watchface_object_item[j].destroy_callback)
                        {
                            if(PNULL != s_watchface_object_item[j].restart_timer_callback)
                            {
                                s_watchface_object_item[j].restart_timer_callback(s_object_destroy[i].handle);
                            }
                            break;
                        }
                    }
                }
            }
            return;
        }
    }
    else
    {
        SCI_MEMSET(s_current_watchface_path,0x00,MAX_PATH_LEN);
        WATCHFACE_Get_CurrentWatchfaceFile_Path(s_current_watchface_path);
        TRACE_WATCHFACE("[watchface_flow]:watchface_path=%s",s_current_watchface_path);
    }

    for(i=0;i<MAX_ITEM_COUNT;i++)
    {
        if(0 != s_object_destroy[i].handle)
        {
            s_object_destroy[i].destroy_callback(s_object_destroy[i].handle);
        }

    }

    if (FALSE == MMIAPIUDISK_UdiskIsRun())
    {
        //SCI_MEMSET(s_object_destroy,0, MAX_ITEM_COUNT * sizeof(WATCHFACE_CTRL_DESTROY_INFO_T));//bug1998393
        for(i=0;i<MAX_ITEM_COUNT; i++)
        {
            SCI_MEMSET(&s_object_destroy[i],0,sizeof(WATCHFACE_CTRL_DESTROY_INFO_T));//bug1998393
        }
        p_ctrl_create_info_list = WATCHFACE_Get_Ctrl_Create_List();
        if(PNULL == p_ctrl_create_info_list)
        {
            TRACE_WATCHFACE("[watchface_flow]: s_p_ctrl_create_info_list is pnull.");
            return;
        }
        TRACE_WATCHFACE("[watchface_flow]: s_p_ctrl_create_info_list item_number is:%d.",p_ctrl_create_info_list->item_number);
        for(i=0;i<p_ctrl_create_info_list->item_number;i++)
        {
            s_object_destroy[i].handle= p_ctrl_create_info_list->p_ctrl_create_info[i].object_callback(WATCHFACE_WIN_ID,
                                        p_ctrl_create_info_list->p_ctrl_create_info[i].p_ctrl_param
                                                      );
            for(j = 0;j < arr_num;j++)
            {
                if(p_ctrl_create_info_list->p_ctrl_create_info[i].object_callback == s_watchface_object_item[j].object_callback)
                {
                    s_object_destroy[i].destroy_callback = s_watchface_object_item[j].destroy_callback;
                    break;
                }
            }

        }
        TRACE_WATCHFACE("[watchface_flow]: s_object_destroy number is:%d.",i);//Bug 2114188
    }

}

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)

/*****************************************************************************/
//  Description : append watchface layer
//  Parameter: None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_AppendLayer(void)
{
    uint8   i = 0;

    TRACE_WATCHFACE("[watchface_flow]: WATCHFACE_AppendLayer.");
    for(i=0;i<MAX_ITEM_COUNT;i++)
    {
        if(0 != s_object_destroy[i].handle)
        {
            WATCHFACE_Adapter_AppendLayer(s_object_destroy[i].handle);
        }

    }
}

/*****************************************************************************/
//  Description : remove watchface layer
//  Parameter: None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_RemoveLayer(void)
{
    uint8   i = 0;

    TRACE_WATCHFACE("[watchface_flow]: WATCHFACE_RemoveLayer.");
    for(i=0;i<MAX_ITEM_COUNT;i++)
    {
        if(0 != s_object_destroy[i].handle)
        {
            WATCHFACE_Adapter_RemoveLayer(s_object_destroy[i].handle);
        }

    }
}

#endif

/*****************************************************************************/
//  Description : stop watchface timer
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_StopTimer(void)
{
    uint8   i = 0;
    uint8   j = 0;
    uint8   arr_num = 0;

    TRACE_WATCHFACE("[watchface_flow]: WATCHFACE_StopTimer.");
    for(i=0;i<MAX_ITEM_COUNT;i++)
    {
        if(0 != s_object_destroy[i].handle)
        {
            arr_num = ARR_SIZE(s_watchface_object_item);
            for(j = 0;j < arr_num;j++)
            {
                if(s_object_destroy[i].destroy_callback == s_watchface_object_item[j].destroy_callback)
                {
                    if(PNULL != s_watchface_object_item[j].stop_timer_callback)
                    {
                        s_watchface_object_item[j].stop_timer_callback(s_object_destroy[i].handle);
                    }
                    break;
                }
            }
        }
    }
}

