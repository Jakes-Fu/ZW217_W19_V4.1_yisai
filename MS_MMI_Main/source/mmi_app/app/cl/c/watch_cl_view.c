/*****************************************************************************
** File Name:      watch_cl_view.c                                           *
** Author:         yuming.yang                                               *
** Date:           03/6/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw cl module interface             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 03/2020               yuming.yang            Create                       *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmicl_export.h"
#include "guilistbox.h"
#include "mmipb_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guimsgbox.h"
#include "mmi_appmsg.h"
#include "mmicc_id.h"
#include "mmicc_nv.h"
#include "mmicl_internal.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "watch_common_list.h"
#include "mmicc_export.h"
#include "watch_pb_view.h"
#include "watch_cc_view.h"
#include "watch_commonwin_export.h"
#ifdef SCREEN_SHAPE_CIRCULAR
#include "watch_common.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define  CALL_TIME_LEN_MAX    4


/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct _WATCHCL_CALLINFO_T
{
    MMI_IMAGE_ID_T          photo_image_id;
    wchar                   disp_wchar[PHONE_NUM_MAX_LEN + 2];
    MMI_IMAGE_ID_T          calltype_image_id;
    MMIPB_BCD_NUMBER_T      bcdNumber;
} WATCHCL_CALLINFO_T;


typedef struct _WATCHCL_ARRAY_CALLINFO_T
{
    uint8                   record_num;
    WATCHCL_CALLINFO_T      call_info[MMICL_RECORD_TOTAL_NUM];
} WATCHCL_ARRAY_CALLINFO_T;


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
WATCHCL_ARRAY_CALLINFO_T*   arraycallInfo = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取通话记录的联系人姓名或者号码显示
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL uint8 CL_GetNameOrNumberContent(wchar *in_str, uint16 max_len, MMICL_CALL_BCDINFO_T *callinfo);

/*****************************************************************************/
//  Discription: get call type image id
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL uint32 CL_GetCallTypeImageId(MMICL_CALL_TYPE_E  call_type);

/*****************************************************************************/
//  Discription: convert info
//  Global resource dependence: arraycallInfo
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CL_ConvertNvInfoToArrayInfo(MMICL_CALL_BCDINFO_T* nv_info, WATCHCL_CALLINFO_T* array_info);

/*****************************************************************************/
//  Description : ring back (回复电话)
//  Global resource dependence :
//  Author: yuming.yang
//  Note: is_need_query 为 TRUE 时表示需要弹 query win
/*****************************************************************************/
LOCAL void CL_HandleRingBack(WATCHCL_CALLINFO_T* call_info, BOOLEAN is_need_query);

/*****************************************************************************/
//  Description : handle cl main window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleClListWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );


/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DEFINITION                          */
/*---------------------------------------------------------------------------*/
WINDOW_TABLE(MMICL_CALLLOG_WIN_TAB) =
{
    WIN_ID(WATCHCL_CALLLOG_WIN_ID),
    WIN_FUNC((uint32)HandleClListWinMsg),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, WATCHCL_CALLLOG_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT, STXT_RETURN),
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                              LOCAL FUNCTION                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取通话记录的联系人姓名或者号码显示
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL uint8 CL_GetNameOrNumberContent(wchar *in_str, uint16 max_len, MMICL_CALL_BCDINFO_T *callinfo)
{
    MMI_STRING_T        nameStr = {0};
    wchar               nameWchar[PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_STRING_T        addtimesStr = {0};
    uint8               teleNum[PHONE_NUM_MAX_LEN + 2] = {0};
    uint16              teleNumLen = 0;
    uint8               addTimesNum[PHONE_NUM_MAX_LEN + 2 +CALL_TIME_LEN_MAX] = {0};
    uint16              addTimesStrLen = 0;
    uint8               photoFlag = 0;

    if ((in_str == PNULL)||(callinfo==PNULL))
    {
        return FALSE;
    }

    addtimesStr.wstr_ptr = in_str;
    nameStr.wstr_ptr = nameWchar;

    //get number
    teleNumLen = MMICL_GenDispNumber(callinfo->call_type,
                                        callinfo->number_type,
                                        callinfo->number_len,
                                        callinfo->number,
                                        teleNum,
                                        PHONE_NUM_MAX_LEN + 2);
    //get name and portrait in pb
    photoFlag = WatchPB_GetDispInfoContent((uint8 *)teleNum, teleNumLen, &nameWchar, TRUE);

    //no name in pb, imageId is stranger AND is not unknown number
    if ((photoFlag == PB_PROTRAIT_STRANGER) && (0 != teleNumLen))
    {
        if(callinfo->call_times > 1)
        {
            sprintf((char *)addTimesNum, "%s(%d)", teleNum, callinfo->call_times);
            addTimesStrLen = strlen((char *)addTimesNum);
            MMI_STRNTOWSTR(in_str,max_len,addTimesNum,addTimesStrLen,addTimesStrLen);
        }
        else
        {
            MMI_STRNTOWSTR(in_str,max_len,teleNum,teleNumLen,teleNumLen);
        }
    }
    else//exist name or is unknown number
    {
        if(callinfo->call_times > 1)
        {
            nameStr.wstr_len = MMIAPICOM_Wstrlen(nameStr.wstr_ptr);
            SCI_MEMCPY(nameStr.wstr_ptr, (wchar *)nameWchar, nameStr.wstr_len);
            MMICL_AddCallTimesStr(&nameStr,callinfo->call_times,&addtimesStr,max_len);
        }
        else
        {
            nameStr.wstr_len= MMIAPICOM_Wstrlen(nameStr.wstr_ptr);
            MMI_WSTRNCPY(in_str,max_len,(const wchar*)nameStr.wstr_ptr,
                        nameStr.wstr_len,nameStr.wstr_len);
        }
    }
    return photoFlag;

}

/*****************************************************************************/
//  Discription: get call type image id
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T CL_GetCallTypeImageId(MMICL_CALL_TYPE_E  call_type)
{
    MMI_IMAGE_ID_T      callTypeId = NULL;

    switch(call_type)
    {
        case MMICL_CALL_MISSED:
        {
            callTypeId = IAMGE_CALLLOG_MISSED_CALL;
            break;
        }
        case MMICL_CALL_RECEIVED:
        {
            callTypeId = IAMGE_CALLLOG_INCOMING_CALL;
            break;
        }
        case MMICL_CALL_DIALED:
        {
            callTypeId = IAMGE_CALLLOG_OUTGOING_CALL;
            break;
        }
        default:
            break;
    }
    return callTypeId;
}

/*****************************************************************************/
//  Discription: convert info
//  Global resource dependence: arraycallInfo
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CL_ConvertNvInfoToArrayInfo(MMICL_CALL_BCDINFO_T* nv_info, WATCHCL_CALLINFO_T* array_info)
{
    uint8    photoFlag = 0;

    if((PNULL == nv_info) || (PNULL == array_info))
    {
        return FALSE;
    }

    //disp_wchar
    photoFlag = CL_GetNameOrNumberContent(array_info->disp_wchar, PHONE_NUM_MAX_LEN + 2, nv_info);
    //photo_image_id
    array_info->photo_image_id = WatchPB_GetImageIdByFlag(photoFlag, FALSE);
    //calltype_image_id
    array_info->calltype_image_id = CL_GetCallTypeImageId(nv_info->call_type);
    //bcd number
    array_info->bcdNumber.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(nv_info->number_type, MN_NUM_PLAN_UNKNOW);
    array_info->bcdNumber.number_len = nv_info->number_len;
    array_info->bcdNumber.number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN, array_info->bcdNumber.number_len);
    MMI_MEMCPY(
        array_info->bcdNumber.number,
        MMIPB_BCD_NUMBER_MAX_LEN,
        nv_info->number,
        array_info->bcdNumber.number_len,
        array_info->bcdNumber.number_len);

    return TRUE;
}


/*****************************************************************************/
//  Description : ring back (回复电话)
//  Global resource dependence :
//  Author: yuming.yang
//  Note: is_need_query 为 TRUE 时表示需要弹 query win
/*****************************************************************************/
LOCAL void CL_HandleRingBack(WATCHCL_CALLINFO_T* call_info, BOOLEAN is_need_query)
{
    MN_DUAL_SYS_E       dualSys = MN_DUAL_SYS_1;
    wchar               nameWchar[PHONE_NUM_MAX_LEN + 2] = {0};

    //make call
    //if (call_info->bcdNumber.number_len > 0 ) //未知号码,长度为零
    {
        uint8     teleNum[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
        uint8     numLen = 0;
		MMIPB_BCDNumberToUTF8(&(call_info->bcdNumber), MMIPB_NUMBER_MAX_STRING_LEN, teleNum);
        numLen = MMIPB_GetBCDNumberStrlen(&(call_info->bcdNumber));

        if(!is_need_query)
        {
            WatchAPICC_MakeCall(dualSys, teleNum, numLen, PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
        }
        else
        {
            MMIPB_CONTACT_T         contactInfo = {0};
            MMIPB_CONTACT_T         *addData = NULL;
            addData = (MMIPB_CONTACT_T *)SCI_ALLOC(sizeof(MMIPB_CONTACT_T));
            SCI_ASSERT(addData != NULL);

            //get BCD number
            contactInfo.number->npi_ton = call_info->bcdNumber.npi_ton;
            contactInfo.number->number_len = call_info->bcdNumber.number_len;
            contactInfo.number->number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN,contactInfo.number->number_len);
            MMI_MEMCPY(
                contactInfo.number->number,
                MMIPB_BCD_NUMBER_MAX_LEN,
                call_info->bcdNumber.number,
                contactInfo.number->number_len,
                contactInfo.number->number_len);
            //get name
            WatchPB_GetDispInfoContent((uint8 *)teleNum, numLen, &nameWchar, TRUE);
            contactInfo.name.wstr_len = MMIAPICOM_Wstrlen(nameWchar);
            MMI_WSTRNCPY(contactInfo.name.wstr,
                        PHONE_NUM_MAX_LEN + 2,
                        (const wchar*)nameWchar,
                        contactInfo.name.wstr_len,
                        contactInfo.name.wstr_len);

            *addData = contactInfo;
            WatchCC_MoCallQueryWin_Enter(addData);
        }
    }
}

/*****************************************************************************/
//  Discription: array call info
//  Global resource dependence: arraycallInfo
//  Author: yuming.yang
//  Note: 支持未读 missed call 置顶，查看后转为正常按时间排序
/*****************************************************************************/
LOCAL void ArrayCallLogForMissedAlert(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T* callLogInfo = PNULL;
    MMICL_CALL_BCDINFO_T        *callInfo = PNULL;
    uint8                       i = 0;

    callLogInfo = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    //read call logs from NV
    MMICL_GetRecordInfo(MMICL_CALL_ALL, callLogInfo);
    SCI_MEMSET(arraycallInfo, 0x00, sizeof(WATCHCL_ARRAY_CALLINFO_T));  //bug 1347237
    arraycallInfo->record_num = callLogInfo->record_num;

    if(0 != MMIAPICC_GetMissedCallNumber())//s_call_context 中记录的未处理的missed call的数量
    {
        uint8   arrayIdx = 0;
        //top show missed call
        for ( i = 0; i < arraycallInfo->record_num; i++ )
        {
            callInfo = &(callLogInfo->call_bcdinfo[i]);
            if( MMICL_CALL_MISSED == callInfo->call_type )
            {
                CL_ConvertNvInfoToArrayInfo(callInfo, &(arraycallInfo->call_info[arrayIdx]));
                arrayIdx++;
            }
        }
        for ( i = 0; i < arraycallInfo->record_num; i++ )
        {
            callInfo = &(callLogInfo->call_bcdinfo[i]);
            if( MMICL_CALL_MISSED != callInfo->call_type )
            {
                CL_ConvertNvInfoToArrayInfo(callInfo, &(arraycallInfo->call_info[arrayIdx]));
                arrayIdx++;
            }
        }
        MMIAPICC_ClearMissedCallNum();
    }
    else
    {
        for ( i = 0; i < arraycallInfo->record_num; i++ )
        {
            callInfo = &(callLogInfo->call_bcdinfo[i]);
            CL_ConvertNvInfoToArrayInfo(callInfo, &(arraycallInfo->call_info[i]));
        }
    }
    SCI_FREE(callLogInfo);
}


/*****************************************************************************/
//  Discription: draw cl list items
//  Global resource dependence: arraycallInfo
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateCallLogWinList(void)
{
    MMI_CTRL_ID_T               ctrlId = WATCHCL_CALLLOG_LIST_CTRL_ID;
    MMI_STRING_T                numberStr = {0};
#ifndef SCREEN_SHAPE_CIRCULAR
    MMI_STRING_T                tempStr = {0};
#endif
    uint8                       i = 0;

    GUILIST_RemoveAllItems(ctrlId);

    for ( i = 0; i < arraycallInfo->record_num; i++ )
    {
        SCI_MEMSET(&numberStr,0,sizeof(MMI_STRING_T));
        numberStr.wstr_ptr = arraycallInfo->call_info[i].disp_wchar;
        numberStr.wstr_len= MMIAPICOM_Wstrlen(numberStr.wstr_ptr);
#ifdef SCREEN_SHAPE_CIRCULAR
        WatchCOM_ListItem_Draw_1Str_1Icon(ctrlId, numberStr, arraycallInfo->call_info[i].calltype_image_id);
#else
        WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str(ctrlId,
                arraycallInfo->call_info[i].photo_image_id,
                numberStr,
                arraycallInfo->call_info[i].calltype_image_id,
                tempStr);
#endif
    }
}

/*****************************************************************************/
//  Description : handle cl main window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleClListWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T                  list_rect = WATCHLIST_RECT;
#endif

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, WATCHCL_CALLLOG_LIST_CTRL_ID);
#ifdef SCREEN_SHAPE_CIRCULAR
            GUILIST_SetListState( WATCHCL_CALLLOG_LIST_CTRL_ID, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//bug 2088117 增加list自动滚动属性
            GUILIST_SetRect(WATCHCL_CALLLOG_LIST_CTRL_ID, &list_rect);
#endif
            GUILIST_SetMaxItem(WATCHCL_CALLLOG_LIST_CTRL_ID, MMICL_RECORD_MAX_NUM, FALSE);
            arraycallInfo = (WATCHCL_ARRAY_CALLINFO_T*)SCI_ALLOCAZ(sizeof(WATCHCL_ARRAY_CALLINFO_T));
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            if(NULL != arraycallInfo)
            {
                SCI_FREE(arraycallInfo);
            }
            break;
        }

        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
#endif
            ArrayCallLogForMissedAlert();
            UpdateCallLogWinList();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            WatchCL_ListWin_Exit();
            break;
        }

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_PENOK:
        {
            uint16 idx = GUILIST_GetCurItemIndex(WATCHCL_CALLLOG_LIST_CTRL_ID);
            if(arraycallInfo->record_num > 0)
            {
                CL_HandleRingBack(&(arraycallInfo->call_info[idx]), TRUE);
            }
            break;
        }

        case MSG_APP_GREEN:
        {
            uint16 idx = GUILIST_GetCurItemIndex(WATCHCL_CALLLOG_LIST_CTRL_ID);
            if(arraycallInfo->record_num > 0)
            {
                CL_HandleRingBack(&(arraycallInfo->call_info[idx]), FALSE);
            }
            break;
        }
#if 0
        case MSG_CTL_OK:
        {
            uint16 idx = GUILIST_GetCurItemIndex(WATCHCL_CALLLOG_LIST_CTRL_ID);
            if(arraycallInfo->record_num > 0)
            {
                MMICL_DeleteAppointedCallInfo(MMICL_CALL_ALL, (uint8)idx);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL );
            }
        }
#endif
        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*---------------------------------------------------------------------------*/
/*                             PUBLIC FUNCTION                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: Open cl list window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_ListWin_Enter(void)
{
    MMK_CreateWin((uint32 *)MMICL_CALLLOG_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Close cl list window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_ListWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCL_CALLLOG_WIN_ID))
    {
        MMK_CloseWin(WATCHCL_CALLLOG_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : open no call log tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_EmptyList_NoteWin(void)
{
    MMI_WIN_ID_T    winId   = WATCHCL_CALLLOG_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_NO_CALLLOG, &noteStr);
    WatchCOM_NoteWin_FullScreen_Enter(winId, NULL, &noteStr, IAMGE_CALLLOG_LIST_EMPTY, NULL, softKey, PNULL);
}


