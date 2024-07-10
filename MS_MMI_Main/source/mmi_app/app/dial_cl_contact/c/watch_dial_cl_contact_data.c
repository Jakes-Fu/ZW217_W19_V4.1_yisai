/*****************************************************************************
** File Name:      watch_dial_cl_contact_view.c                              *
** Author:         feiyue.ji                                                 *
** Date:           18/1/2021                                                 *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw dial&calllog&contact interface  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 01/2021               feiyue.ji            Create                         *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "watch_dial_cl_contact_internal.h"
#include "mmicc_export.h"
#include "mmidisplay_color.h"
#include "mmipb_datalist.h"
//#include "mmipb_common.h"
#include "mmipb_search.h"
#include "mmipb_export.h"
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#include "watch_notifycenter.h"
#endif
#include "watch_dial_cl_contact_export.h"
// TODO:cc text image,just for test
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef DROPDOWN_NOTIFY_SHORTCUT
typedef struct
{
    uint32 notify_handle;//cl显示的信息在notify center中的handle
    uint8  number[MMIPB_BCD_NUMBER_MAX_LEN];   //the dialling number
//cl信息的index
}CL_NOTIFY_DATA_STRUCT;

typedef struct
{
    CL_NOTIFY_DATA_STRUCT clnotify_data[WATCH_CL_RECORD_MAX_NUM];//cl在notifyceenter中 的 数据信息
    uint16 count;//记录cl在notifyceenter中 的 数据个数
}CL_NOTIFY_INFO_STRUCT;
CL_NOTIFY_INFO_STRUCT s_cl_notify_info = {0};
#endif

LOCAL void CALLLog_NotifyInfo_ResetClNotifyData(void);

/*****************************************************************************/
//  Description : Calllog_CreateViewData
//  Global resource dependence : none
//  Author: feiyue.ji
//  Note:创建calllog的view data
/*****************************************************************************/
LOCAL CL_VIEW_DATA_INFO_T* Calllog_CreateViewData(void)
{
    CL_VIEW_DATA_INFO_T* calllog_viewdata_info_ptr = {0};

    TRACE_APP_DIAL_CL_CONTACT("calllog data create");

    calllog_viewdata_info_ptr = (CL_VIEW_DATA_INFO_T*)SCI_ALLOC_APP(sizeof(CL_VIEW_DATA_INFO_T));
    if(PNULL == calllog_viewdata_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("create data fail!");
        return PNULL;
    }
    SCI_MEMSET(calllog_viewdata_info_ptr,
               0,
               sizeof(CL_VIEW_DATA_INFO_T)
               );

    return calllog_viewdata_info_ptr;
}

/*****************************************************************************/
//  Description : Calllog_DestroyViewData
//  Global resource dependence : none
//  Author: feiyue.ji
//  Note:销毁calllog的view data
/*****************************************************************************/
LOCAL void Calllog_DestroyViewData(CL_VIEW_DATA_INFO_T* cl_view_data_info_ptr)
{
    TRACE_APP_DIAL_CL_CONTACT("calllog data destroy");

    if(PNULL == cl_view_data_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("data is  PNULL!");
        return;
    }
    SCI_FREE(cl_view_data_info_ptr);
    cl_view_data_info_ptr = PNULL;
}

/*****************************************************************************/
//  Description : 获得显示可用的电话号码
//  Global resource dependence : none
//  [IN]:cl_nv_info,nv存储的单条calllog信息
//  [OUT]:name_str,名字字串
//  [IN]:isaddcalltimes，TRUE，名字中显示来电次数；FALSE，不显示来电次数
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void Calllog_GetName( MMICL_CALL_BCDINFO_T* cl_nv_info,
                                               MMI_STRING_T* name_str,
                                               BOOLEAN isaddcalltimes)
{
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 tele_len = 0;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    MMI_STRING_T name = {0};
    wchar wchar_namestr[PHONE_NUM_MAX_LEN +MMICL_MAX_CALL_TIMES+ 1] = {0};
    BOOLEAN b_search_result = FALSE;
    uint8 calltimes_char[MMICL_MAX_CALL_TIMES+2] = {0};
    wchar calltimes_str[MMICL_MAX_CALL_TIMES+2] = {0};
    uint32 calltimes_len = 0;

    TRACE_APP_DIAL_CL_CONTACT("enter Calllog_GetName");
    if(PNULL == name_str || PNULL == name_str->wstr_ptr)//fix bug1623526
    {
        TRACE_APP_DIAL_CL_CONTACT("param error!");
        return;
    }

    name.wstr_ptr = wchar_namestr;
    name.wstr_len = PHONE_NUM_MAX_LEN +MMICL_MAX_CALL_TIMES;
    SCI_MEMSET(name.wstr_ptr, 0, sizeof(wchar)*name.wstr_len);

    tele_len = MMIAPICL_GenDispNumber(cl_nv_info,
                                      tele_num,
                                      PHONE_NUM_MAX_LEN + 2);
    //get name and portrait in pb
    WatchPB_GetDispInfoContent((uint8 *)tele_num, tele_len, wchar_namestr, 
    TRUE);//fix bug1623526 



#ifdef CALL_TIMES_SUPPORT
    //display times in name
    if((cl_nv_info->call_times > 1)&&(isaddcalltimes))
    {
        sprintf(calltimes_char, "(%d)", cl_nv_info->call_times);
        calltimes_len = strlen(calltimes_char) ;
        MMI_STRNTOWSTR(calltimes_str,MMICL_MAX_CALL_TIMES,calltimes_char,calltimes_len,calltimes_len);
        if((PHONE_NUM_MAX_LEN + MMICL_MAX_CALL_TIMES)>=(name.wstr_len + calltimes_len))
        {
            MMIAPICOM_Wstrncat(name.wstr_ptr, calltimes_str, calltimes_len);
            name.wstr_len = name.wstr_len + (uint16)calltimes_len;
        }
        else
        {
            uint16 len = 0;
            TRACE_APP_DIAL_CL_CONTACT("name length too long!");
            len = name.wstr_len + (uint16)calltimes_len - (PHONE_NUM_MAX_LEN + MMICL_MAX_CALL_TIMES);
            MMIAPICOM_Wstrncat(name.wstr_ptr, calltimes_str, len);

        }
    }
#endif

    name_str->wstr_len = MIN(name_str->wstr_len, name.wstr_len);

    SCI_MEMCPY(name_str->wstr_ptr,
               name.wstr_ptr,
               name_str->wstr_len*sizeof(wchar));

}

/*****************************************************************************/
//  Description : Calllog_ClNVData2ViewData
//  Global resource dependence : none
//  [IN]:clNVdata,nv存储的单条calllog信息
//  [OUT]:clviewdata， 单条calllog的显示信息
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void Calllog_ClNVData2ViewData(MMICL_CALL_BCDINFO_T* clNVdata, CL_VIEW_DATA_T* clviewdata)
{
    uint32 image_id = IMAGE_NULL;
    MMI_STRING_T name_str = {0};
    wchar name_wchar[PHONE_NUM_MAX_LEN +WATCH_MAX_CALL_TIME_LEN+ 1] = {0};
    wchar datetime_str[WATCH_DATA_TIME_STR_MAX_LEN + 1] = {0};
    uint16 datetime_wstr_len = 0;
    uint16 call_times_index = 0;
    uint8 tele_num[WATCH_PHONE_NUMMAX_LEN + 2] = {0};
    uint16 tele_len = 0;

    TRACE_APP_DIAL_CL_CONTACT("calllog data to view data");

    if((PNULL == clNVdata) || (PNULL == clviewdata))
    {
        TRACE_APP_DIAL_CL_CONTACT("param  error!");
        return;
    }

    TRACE_APP_DIAL_CL_CONTACT("call type: %d", clNVdata->call_type);
    //call log type image
    switch(clNVdata->call_type)
    {
        case MMICL_CALL_MISSED:
        {
            image_id = res_aw_info_missedcall;
            break;
        }
        case MMICL_CALL_RECEIVED:
        {
            image_id = res_aw_calllog_incoming_call;
            break;
        }
        case MMICL_CALL_DIALED:
        {
            image_id = res_aw_calllog_outgoing_call;
            break;
        }
        default:
        {
            TRACE_APP_DIAL_CL_CONTACT("unkown call type: %d", clNVdata->call_type);
            break;
        }
    }
    clviewdata->calltype_image_id = image_id;

    //calllog name string
    name_str.wstr_ptr = name_wchar;
    name_str.wstr_len = PHONE_NUM_MAX_LEN +WATCH_MAX_CALL_TIME_LEN;
    Calllog_GetName(clNVdata, &name_str, TRUE);

    //calllog phone number
    tele_len = MMIAPICL_GenDispNumber(clNVdata,
                                     tele_num,
                                     WATCH_PHONE_NUMMAX_LEN + 2);

    TRACE_APP_DIAL_CL_CONTACT("GenDispNumber tele_len: %d", tele_len);

    //calllog time
#ifdef CALL_TIMES_SUPPORT
    call_times_index = clNVdata->call_times -1;
#endif
    datetime_wstr_len = MMIAPICL_GenDispTimeAndDate(clNVdata,
                                   datetime_str,
                                   WATCH_DATA_TIME_STR_MAX_LEN,
                                   call_times_index,
                                   '/',
                                   TRUE);
    TRACE_APP_DIAL_CL_CONTACT("datetimestr: %s", datetime_str);

    //set call log view data
    clviewdata->calltype_image_id = image_id;
    SCI_MEMCPY(clviewdata->disp_str,
               name_str.wstr_ptr,
               MIN(name_str.wstr_len, DISPLAY_STR_MAX_LEN)*sizeof(wchar));

    SCI_MEMCPY(clviewdata->datetime_wstr,
               datetime_str,
               MIN(datetime_wstr_len, WATCH_DATA_TIME_STR_MAX_LEN)*sizeof(wchar));

    SCI_MEMCPY(clviewdata->number,
               tele_num,
               MIN(tele_len, WATCH_PHONE_NUMMAX_LEN)*sizeof(uint8));

}

/*****************************************************************************/
//  Description : to load calllog  data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void Calllog_LoadData(CL_VIEW_DATA_INFO_T* cl_view_info)
{
    MMICL_CALL_ARRAY_BCDINFO_T* callLogInfo = PNULL;
    MMICL_CALL_BCDINFO_T        *callInfo = PNULL;
    uint8                       i = 0;
    uint8   arrayIdx = 0;

    TRACE_APP_DIAL_CL_CONTACT("calllog data load");

    if( PNULL == cl_view_info)
    {
        TRACE_APP_DIAL_CL_CONTACT("param error!");
        return;
    }
    callLogInfo = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == callLogInfo)
    {
        TRACE_APP_DIAL_CL_CONTACT("ALLOC FAIL!");
        return;
    }

    //read call logs from NV
    MMICL_GetRecordInfo(MMICL_CALL_ALL, callLogInfo);

    cl_view_info->calllog_count = callLogInfo->record_num;

    //load missed call data
    if(0 != MMIAPICC_GetMissedCallNumber())//s_call_context 中记录的未处理的missed call的数量
    {
        for ( i = 0; i < WATCH_CL_RECORD_MAX_NUM; i++ )
        {
            callInfo = &(callLogInfo->call_bcdinfo[i]);
            if(0 == callInfo->call_times)//fix bug 1903639
            {
                TRACE_APP_DIAL_CL_CONTACT("not find unread missed call!");
                continue;
            }

            if((MMICL_CALL_MISSED == callInfo->call_type)
                &&(FALSE == callInfo->is_missed_call_read))
            {
                SCI_MEMSET(&(cl_view_info->calllog_view_data_array[arrayIdx]),0, sizeof(CL_VIEW_DATA_T));
                Calllog_ClNVData2ViewData(callInfo,&(cl_view_info->calllog_view_data_array[arrayIdx]));
                cl_view_info->calllog_view_data_array[arrayIdx].is_unread_missed_call= TRUE;

                arrayIdx++;
            }
        }
        cl_view_info->missedcall_count = arrayIdx;
    }

    for ( i = 0; i < WATCH_CL_RECORD_MAX_NUM; i++ )
    {
        callInfo = &(callLogInfo->call_bcdinfo[i]);
        if((MMICL_CALL_MISSED == callInfo->call_type)
            &&(FALSE == callInfo->is_missed_call_read))
        {
            TRACE_APP_DIAL_CL_CONTACT("no need to transfer!");
        }
        else
        {
            SCI_MEMSET(&(cl_view_info->calllog_view_data_array[arrayIdx]),0, sizeof(CL_VIEW_DATA_T));
            Calllog_ClNVData2ViewData(callInfo, &(cl_view_info->calllog_view_data_array[arrayIdx]));
            arrayIdx++;
        }
        cl_view_info->calllog_view_data_array[arrayIdx].is_unread_missed_call = FALSE;
    }

    SCI_FREE(callLogInfo);
    //load calllog data
}

/*****************************************************************************/
//  Description : Contact_CreateViewData
//  Global resource dependence : none
//  Author: feiyue.ji
//  Note:创建contact的view data
/*****************************************************************************/
LOCAL CONTACT_VIEW_DATA_INFO_T* Contact_CreateViewData(void)
{
    CONTACT_VIEW_DATA_INFO_T* contact_viewdata_info_ptr = {0};

    TRACE_APP_DIAL_CL_CONTACT("contact data create");
    contact_viewdata_info_ptr = (CONTACT_VIEW_DATA_INFO_T*)SCI_ALLOC_APP(sizeof(CONTACT_VIEW_DATA_INFO_T));
    if(PNULL == contact_viewdata_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("create data fail!");
        return PNULL;
    }
    SCI_MEMSET(contact_viewdata_info_ptr,
               0,
               sizeof(CONTACT_VIEW_DATA_INFO_T)
               );

    return contact_viewdata_info_ptr;
}

/*****************************************************************************/
//  Description : Contact_DestroyViewData
//  Global resource dependence : none
//  Author: feiyue.ji
//  Note:销毁contact的view data
/*****************************************************************************/
LOCAL void Contact_DestroyViewData(CONTACT_VIEW_DATA_INFO_T* contact_view_data_info_ptr)
{
    TRACE_APP_DIAL_CL_CONTACT("contact data destroy");

    if(PNULL == contact_view_data_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("data is  PNULL!");
        return;
    }
    SCI_FREE(contact_view_data_info_ptr);
    contact_view_data_info_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Contact_ContactData2ViewData
//  Global resource dependence : none
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void Contact_ContactData2ViewData(MMIPB_CONTACT_BASE_INFO_T* contactbaseinfo, CONTACT_VIEW_DATA_T* contactviewdata)
{
    uint16 contact_id;
    uint16 storage_id;
    uint16 disp_str_len = 0;
    MMIPB_NAME_NUMBER_MAIL_T string_ptr = {0};
    uint16  tele_len                              = 0;
    uint8  tele_num[WATCH_PHONE_NUMMAX_LEN + 2]  = {0};
    MN_NUMBER_TYPE_E  number_type = MN_NUM_TYPE_UNKNOW;
    uint8 max_number_len = 0;

    TRACE_APP_DIAL_CL_CONTACT("contact data to view data");

    if((PNULL == contactbaseinfo) || (PNULL == contactviewdata))
    {
        TRACE_APP_DIAL_CL_CONTACT("param is error!");
        return;
    }
    contact_id = contactbaseinfo->contact_id;
    storage_id = contactbaseinfo->storage_id;
    MMIPB_ReadContactNameNumberMail(contact_id, storage_id, &string_ptr);
    disp_str_len = MIN(DISPLAY_STR_MAX_LEN, string_ptr.name.wstr_len);
    SCI_MEMCPY(contactviewdata->name_str,
               string_ptr.name.wstr,
               disp_str_len*sizeof(wchar));

    TRACE_APP_DIAL_CL_CONTACT("contact name:%s", string_ptr.name.wstr);

    max_number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN, string_ptr.number_t[0].number_len);
    number_type = MMIPB_GetNumberTypeFromUint8(string_ptr.number_t[0].npi_ton);
    tele_len = MMIAPICOM_GenDispNumber(
        number_type,
        max_number_len,
        string_ptr.number_t[0].number,
        contactviewdata->number,
        WATCH_PHONE_NUMMAX_LEN + 2);
    TRACE_APP_DIAL_CL_CONTACT("GenDispNumber tele_len: %d", tele_len);

}

/*****************************************************************************/
//  Description : to load contact view  data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL void Contact_LoadData(CONTACT_VIEW_DATA_INFO_T* contact_view_info)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_MAX;
    MMIPB_CONTACT_LIST_INFO_T contactlist = {0};
    MMI_STRING_T     search_str = {0};
    uint32          total_count = 0;
    uint16 i = 0;

    TRACE_APP_DIAL_CL_CONTACT("contact data load");

    if(PNULL == contact_view_info)
    {
        TRACE_APP_DIAL_CL_CONTACT("param is  PNULL!");
        return;
    }
    total_count = MMIPB_MultiSearch(&search_str, 1<<PB_GROUP_ALL, 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
    MMIPB_ReadContactList(&contactlist);
    contact_view_info->contact_count = MIN(contactlist.count, WATCHPB_CONTACT_NUM_MAX);
    for(i=0; i< contact_view_info->contact_count; i++)
    {
        MMIPB_CONTACT_BASE_INFO_T contact_info = {0};
        CONTACT_VIEW_DATA_T* contactviewdata_ptr = PNULL;
        MMIPB_ReadContactSimpleInfo(i, &contact_info);
        contactviewdata_ptr = &(contact_view_info->contact_view_data_array[i]);
        Contact_ContactData2ViewData(&contact_info, contactviewdata_ptr);
    }
}

/*****************************************************************************/
//  Description : to create dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC DIAL_CL_CONTACT_VIEW_DATA_T* Watch_Dial_Cl_Contact_CreateViewData(void)
{
    DIAL_CL_CONTACT_VIEW_DATA_T* view_data_ptr = PNULL;

    view_data_ptr = (DIAL_CL_CONTACT_VIEW_DATA_T*)SCI_ALLOC_APP(sizeof(DIAL_CL_CONTACT_VIEW_DATA_T));

    if(PNULL == view_data_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("create data fail!");
        return PNULL;
    }

    SCI_MEMSET(view_data_ptr,
               0,
               sizeof(DIAL_CL_CONTACT_VIEW_DATA_T)
               );

    //create cl data
    view_data_ptr->calllog_view_data_ptr = Calllog_CreateViewData();

    //create contact data
    view_data_ptr->contact_view_data_ptr = Contact_CreateViewData();
    return view_data_ptr;
}

/*****************************************************************************/
//  Description : to destroy dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_DestroyViewData(DIAL_CL_CONTACT_VIEW_DATA_T* view_data_ptr)
{
    if(PNULL == view_data_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("param error!");
        return;
    }

    //destroy cl data
    if(PNULL != view_data_ptr->calllog_view_data_ptr)
    {
        Calllog_DestroyViewData(view_data_ptr->calllog_view_data_ptr);
    }

    //destroy contact data
    if(PNULL != view_data_ptr->contact_view_data_ptr)
    {
        Contact_DestroyViewData(view_data_ptr->contact_view_data_ptr);
    }

    //view data
    SCI_FREE(view_data_ptr);
    view_data_ptr = PNULL;
}

/*****************************************************************************/
//  Description : to load dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_LoadViewData(DIAL_CL_CONTACT_VIEW_DATA_T* view_data_ptr)
{
    //call log load data
    Calllog_LoadData(view_data_ptr->calllog_view_data_ptr);

    //contact load data
    Contact_LoadData(view_data_ptr->contact_view_data_ptr);
}

/*****************************************************************************/
//  Description : to lsave calllog  data to nv
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_SaveMissedCallData(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T* callLogInfo = PNULL;
    MMICL_CALL_BCDINFO_T        *callInfo = PNULL;
    uint8                       i = 0;

    callLogInfo = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == callLogInfo)
    {
        TRACE_APP_DIAL_CL_CONTACT("ALLOC FAIL!");
        return;
    }

    //read call logs from NV
    MMICL_ReadNV(MMICL_CALL_ALL, callLogInfo);
    if(0 == callLogInfo->record_num)
    {
        TRACE_APP_DIAL_CL_CONTACT("there is no missd call!");
        SCI_FREE(callLogInfo); //fixbug 1592718
        callLogInfo= PNULL;
        return;
    }
    for ( i = 0; i < callLogInfo->record_num; i++ )
    {
        callInfo = &(callLogInfo->call_bcdinfo[i]);
        if(MMICL_CALL_MISSED == callInfo->call_type)
        {
            callInfo->is_missed_call_read = TRUE;
        }
    }
    MMICL_WriteNV(MMICL_CALL_MISSED,callLogInfo);
    MMIAPICC_ClearMissedCallNum();
#ifdef DROPDOWN_NOTIFY_SHORTCUT
    for ( i = 0; i < WATCH_CL_RECORD_MAX_NUM; i++ )
    {
        NotifyCenter_Delete(s_cl_notify_info.clnotify_data[i].notify_handle);
    }

    CALLLog_NotifyInfo_ResetClNotifyData();
#endif
    SCI_FREE(callLogInfo);
    callLogInfo= PNULL;
}

#ifdef DROPDOWN_NOTIFY_SHORTCUT

LOCAL void CALLLog_NotifyInfo_ResetClNotifyData(void)
{
    SCI_MEMSET(&s_cl_notify_info, 0, sizeof(CL_NOTIFY_INFO_STRUCT));
}

LOCAL BOOLEAN CALLLog_NotifyInfo_GetCallNumberByHandle(CL_NOTIFY_INFO_STRUCT* cl_notify_info, NOTIFY_HANDLE_T notify_handle, uint8* num, uint16 num_len)
{
    uint16 i = 0;
    BOOLEAN find_flag = FALSE;

    for(i=0; i<cl_notify_info->count; i++)
    {
        if(notify_handle == cl_notify_info->clnotify_data[i].notify_handle)
        {
            SCI_MEMCPY(num,
                       cl_notify_info->clnotify_data[i].number, 
                       sizeof(uint8)*MIN(MMIPB_BCD_NUMBER_MAX_LEN, num_len));
            TRACE_APP_DIAL_CL_CONTACT("notify handle:%d  i:%d",notify_handle, i);
            find_flag = TRUE;
            break;
        }
    }
    return find_flag;
}

LOCAL void CALLLog_NotifyInfo_ClearItemByHandle(CL_NOTIFY_INFO_STRUCT* cl_notify_info,NOTIFY_HANDLE_T notify_handle)
{
    uint16 i = 0;

    for(i=0; i<cl_notify_info->count; i++)
    {
        if(notify_handle == cl_notify_info->clnotify_data[i].notify_handle)
        {
            SCI_MEMSET(&cl_notify_info->clnotify_data[i], 0, sizeof(CL_NOTIFY_DATA_STRUCT));
            cl_notify_info->count--;
            TRACE_APP_DIAL_CL_CONTACT("notify handle:%d  i:%d",notify_handle, i);
            break;
        }
    }
}

LOCAL void CALLLog_UpdateMissedCallNVDataByNotifyHandle(NOTIFY_HANDLE_T notify_handle)
{
    MMICL_CALL_ARRAY_BCDINFO_T* callLogInfo = PNULL;
    MMICL_CALL_BCDINFO_T        *callInfo = PNULL;
    uint8  i = 0;
    uint8 num[MMIPB_BCD_NUMBER_MAX_LEN + 1];
    uint16 pos = 0;
    BOOLEAN find_flag= FALSE;

    find_flag = CALLLog_NotifyInfo_GetCallNumberByHandle(&s_cl_notify_info, notify_handle, num, MMIPB_BCD_NUMBER_MAX_LEN);
    if(FALSE == find_flag)
    {
        TRACE_APP_DIAL_CL_CONTACT("can not find missd call!");
        return;
    }

    //update nv missed call data
    callLogInfo = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == callLogInfo)
    {
        TRACE_APP_DIAL_CL_CONTACT("ALLOC FAIL!");
        return;
    }

    //read call logs from NV
    MMICL_ReadNV(MMICL_CALL_ALL, callLogInfo);
    if(0 == callLogInfo->record_num)
    {
        TRACE_APP_DIAL_CL_CONTACT("there is no missd call!");
        SCI_FREE(callLogInfo);
        return;
    }

    for ( i = 0; i < callLogInfo->record_num; i++ )
    {
        callInfo = &(callLogInfo->call_bcdinfo[i]);
        if(MMICL_CALL_MISSED == callInfo->call_type)
        {
            if(!memcmp(num, callInfo->number, MMIPB_BCD_NUMBER_MAX_LEN))
            {
                callInfo->is_missed_call_read = TRUE;
                break;
            }
        }
    }
    MMICL_WriteNV(MMICL_CALL_MISSED,callLogInfo);
    SCI_FREE(callLogInfo);
    callLogInfo= PNULL;
}

LOCAL void CALLLog_NotifyEnterAPPCallback(NOTIFY_HANDLE_T notify_handle)
{
    Watch_Dial_Cl_Contact_Win_Enter();
}

LOCAL void CALLLog_NotifyReadMissedCallCallback(NOTIFY_HANDLE_T notify_handle)
{
    //update nv data
    CALLLog_UpdateMissedCallNVDataByNotifyHandle(notify_handle);
    //clear cl notify data
    CALLLog_NotifyInfo_ClearItemByHandle(&s_cl_notify_info,notify_handle);

}


/*****************************************************************************/
//  Description : CC missedcall callback
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void CALLLog_AddMissedCallToNotifyCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    NOTIFYCENTER_NOTIFY_INFO_T notify_info = {0};
    MMICL_CALL_BCDINFO_T* call_bcdinfo_ptr = PNULL;
    MMI_STRING_T    appname_str = {0};
    uint16 app_name_len = 0;
    uint16 notify_title_len = 0;
    MMI_STRING_T title = {0};
    wchar wchar_titlestr[PHONE_NUM_MAX_LEN +MMICL_MAX_CALL_TIMES+ 1] = {0};
    uint8 tele_num[WATCH_PHONE_NUMMAX_LEN + 2] = {0};
    uint16 tele_len = 0;
    wchar wchar_tele_num[WATCH_PHONE_NUMMAX_LEN + 2] = {0};
    wchar datetime_str[WATCH_DATA_TIME_STR_MAX_LEN + 1] = {0};
    uint16 datetime_wstr_len = 0;
    uint16 wstr_len = 0;
    uint16 short_wstr_len = 0;
    uint16 cl_notify_index = 0;
    uint16 call_times_index = 0;
    BOOLEAN is_need_update = FALSE;
    uint16 i = 0;
    uint8 calltimes_char[MMICL_MAX_CALL_TIMES] = {0};
    uint32 calltimes_len = 0;
    wchar calltimes_str[MMICL_MAX_CALL_TIMES] = {0};
    uint16 len =0;
    if(PNULL == event_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("param error!");
        return;
    }

    call_bcdinfo_ptr = SCI_ALLOCA(sizeof(MMICL_CALL_BCDINFO_T));

    if (PNULL == call_bcdinfo_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("malloc fail!");
        return;
    }

    SCI_MEMSET(call_bcdinfo_ptr, 0, sizeof(MMICL_CALL_BCDINFO_T));
    MMICL_ReadLastMissedCall(call_bcdinfo_ptr);

    MMI_GetLabelTextByLang(TXT_COMMON_MISS_CALLS, &appname_str);
    app_name_len = MIN(appname_str.wstr_len, NOTIFYCENTER_APPNAME_LEN);

    SCI_MEMCPY(notify_info.app_name_wstr, appname_str.wstr_ptr, app_name_len*sizeof(wchar));
    notify_info.cb_NotifyEnterApp = CALLLog_NotifyEnterAPPCallback;
    notify_info.cb_NotifyRead = CALLLog_NotifyReadMissedCallCallback;


    //notify detail
    title.wstr_ptr = wchar_titlestr;
    title.wstr_len = PHONE_NUM_MAX_LEN +MMICL_MAX_CALL_TIMES;

    Calllog_GetName(call_bcdinfo_ptr, &title, TRUE);

    tele_len = MIN(title.wstr_len, NOTIFYCENTER_DETAILDESC_LEN);
    SCI_MEMCPY(notify_info.detail_desc_wstr, title.wstr_ptr, tele_len*sizeof(wchar));

    //notify image
    notify_info.notify_image = res_aw_info_missedcall;

#ifdef CALL_TIMES_SUPPORT
    if(call_bcdinfo_ptr->call_times > 1)
    {
        call_times_index =  call_bcdinfo_ptr->call_times - 1;
    }
#endif

    //notify time
    MMIAPICL_GenDispTimeAndDate(call_bcdinfo_ptr,
                                datetime_str,
                                WATCH_DATA_TIME_STR_MAX_LEN,
                                call_times_index,
                                '/', //in
                                TRUE);
    wstr_len = (uint16)MMIAPICOM_Wstrlen(datetime_str);

    datetime_wstr_len = MIN(wstr_len, NOTIFYCENTER_TIME_LEN);
    SCI_MEMCPY(notify_info.time_wstr, datetime_str, datetime_wstr_len*sizeof(wchar));

    //notify short content
    len = MMIAPICOM_Wstrlen(datetime_str);
    short_wstr_len = MIN(len, NOTIFYCENTER_SHORTDESC_LEN);
    SCI_MEMCPY(notify_info.short_desc_wstr, datetime_str, short_wstr_len*sizeof(wchar));
    if(call_bcdinfo_ptr->call_times > 1)
    {
        sprintf(calltimes_char, ",(%d)", call_bcdinfo_ptr->call_times);
        calltimes_len = strlen(calltimes_char) ;
        MMI_STRNTOWSTR(calltimes_str,MMICL_MAX_CALL_TIMES,calltimes_char,calltimes_len,calltimes_len);
        if(NOTIFYCENTER_SHORTDESC_LEN > (calltimes_len + datetime_wstr_len))
        {
            MMIAPICOM_Wstrncat(notify_info.short_desc_wstr, calltimes_str, calltimes_len);
        }
    }

    //notify title
    SCI_MEMSET(title.wstr_ptr, 0, sizeof(title.wstr_len));
    Calllog_GetName(call_bcdinfo_ptr, &title, FALSE);
    notify_title_len = MIN(title.wstr_len, NOTIFYCENTER_TITLE_LEN);
    SCI_MEMCPY(notify_info.title_wstr, title.wstr_ptr, notify_title_len*sizeof(wchar));

    //search if the data has exist, if exist, update notify;else add notify
    for(i = 0; i<s_cl_notify_info.count; i++)
    {
        if(!memcmp(call_bcdinfo_ptr->number, s_cl_notify_info.clnotify_data[i].number, MMIPB_BCD_NUMBER_MAX_LEN))
        {
            TRACE_APP_DIAL_CL_CONTACT("update notify center info");
            TRACE_APP_DIAL_CL_CONTACT("notify handle:%d",
                                      s_cl_notify_info.clnotify_data[i].notify_handle);
            NotifyCenter_Delete(s_cl_notify_info.clnotify_data[i].notify_handle);
            s_cl_notify_info.clnotify_data[i].notify_handle = NotifyCenter_Add(&notify_info);
            SCI_FREE(call_bcdinfo_ptr);
            return;
        }
    }

    if(WATCH_CL_RECORD_MAX_NUM == s_cl_notify_info.count)
    {
        NotifyCenter_Delete(s_cl_notify_info.clnotify_data[s_cl_notify_info.count-1].notify_handle);
        SCI_MEMSET(&s_cl_notify_info.clnotify_data[s_cl_notify_info.count-1], 
                   0,
                   sizeof(CL_NOTIFY_DATA_STRUCT));
        s_cl_notify_info.count--;
    }
    for (i=s_cl_notify_info.count; i>0; i--)
    {
        s_cl_notify_info.clnotify_data[i] = s_cl_notify_info.clnotify_data[i-1];
    }
    SCI_MEMSET(&s_cl_notify_info.clnotify_data[0], 
               0,
               sizeof(CL_NOTIFY_DATA_STRUCT));

    s_cl_notify_info.clnotify_data[0].notify_handle = NotifyCenter_Add(&notify_info);
    SCI_MEMCPY(s_cl_notify_info.clnotify_data[0].number,
               call_bcdinfo_ptr->number,
               MMIPB_BCD_NUMBER_MAX_LEN);
    s_cl_notify_info.count++;
    SCI_FREE(call_bcdinfo_ptr);
}

#endif
/*****************************************************************************/
//  Description : CL update callback
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void CALLLog_UpdateCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    if(PNULL == event_info_ptr)
    {
        TRACE_APP_DIAL_CL_CONTACT("param error!");
        return;
    }

    MMK_SendMsg(WATCH_DIAL_CL_CONTACT_WIN_ID, MMI_CL_RELOAD_DATA, (ADD_DATA)TRUE); 
#ifdef DROPDOWN_NOTIFY_SHORTCUT
    if(PNULL != event_info_ptr->param_ptr)
    {
        uint32 cl_event = MMIEVENT_CL_EVENT_NUM;
        SCI_MEMCPY(&cl_event,event_info_ptr->param_ptr, sizeof(uint32));

        if(MMIEVENT_CL_MISSED_CALL == cl_event)
        {
            CALLLog_AddMissedCallToNotifyCallback(event_info_ptr);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : to init calllog
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void WatchCallLog_Init(void)
{
    MMIFW_SetEventListener(CL_UPDATE_EVENT,CALLLog_UpdateCallback, TRUE );

}


