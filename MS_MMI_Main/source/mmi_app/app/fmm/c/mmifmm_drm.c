/*****************************************************************************
** File Name:      mmifmm_drm.c                                              *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/
#define _MMIFMM_DRM_C_

#include "mmi_app_fmm_trc.h"
#include "window_parse.h"
#ifdef DRM_SUPPORT
#include "mmifmm_drm.h"
#include "mmi_drmfilemgr.h"
#include "guirichtext.h"
#include "mmidrm_export.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmifmm_menutable.h"
#include "mmifmm_comfunc.h"
#include "mmifmm_interface.h"
#include "window_parse.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void SetCommonCharacter (GUIRICHTEXT_ITEM_T* p_item);

/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem (
    MMI_CTRL_ID_T   ctrl_id,
    MMI_TEXT_ID_T   label_title,
    MMI_TEXT_ID_T   label_name,
    uint16          *index_ptr,
    wchar           *detail_ptr,
    uint32          detail_len
);
/*****************************************************************************/
//  Description : handle download drm rights file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
//LOCAL MMI_RESULT_E HandleDrmDLRoQueryMsg(
//    MMI_WIN_ID_T win_id,
//    MMI_MESSAGE_ID_E msg_id,
//    DPARAM param
// );

/*****************************************************************************/
//  Description : handle drm copy files query window msg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
//LOCAL MMI_RESULT_E HandleDrmCopyFilesQueryMsg(
//    MMI_WIN_ID_T win_id,
//    MMI_MESSAGE_ID_E msg_id,
//    DPARAM param
//);

/****************************************************************************/
//  Description : handle download popmenu window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/****************************************************************************/
LOCAL MMI_RESULT_E  DrmDownloadPopMenuWinMsg (
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

WINDOW_TABLE (MMIFMM_DRM_DOWNLOAD_MENU_WIN_TAB) =
{
    WIN_FUNC ( (uint32) DrmDownloadPopMenuWinMsg),
    WIN_ID (MMIFMM_DRM_DOWNLOAD_OPT_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL (MENU_FMM_DRM_DOWNLOAD_OPT, MMIFMM_DRM_DOWNLOAD_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


/*****************************************************************************/
// Description : add drm file detail
// Global resource dependence :
// Author:haiwu.chen
// Note: 由于协议目前只能给出单个版权，所以目前实现单个版权的详情显示
/*****************************************************************************/
PUBLIC void MMIAPIFMM_AddDrmFileDetail (
    const uint16 *file_name,
    uint32 access_mode,
    uint32 share_mode,
    uint32 file_attri,
    MMI_CTRL_ID_T ctrl_id
)
{
    MMIDRMFILE_HANDLE drm_handle = 0;
    MMI_TEXT_ID_T title_name = 0;
    MMI_TEXT_ID_T title_value = 0;
    uint8 detail_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    wchar wchar_detail_arr[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    uint16  nIndex = 0;
    MMI_TM_T tm = {0};
    DRM_RIGHTS_T drm_rights = {0};
    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;
    BOOLEAN is_forward = FALSE;

    if (PNULL == file_name)
    {
        return;
    }

    drm_handle = SFS_CreateFile (file_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE == drm_handle)
    {
        return;
    }

    SCI_MEMSET (&drm_rights, 0, sizeof (DRM_RIGHTS_T));
    MMIAPIDRM_GetRightsinfo (drm_handle, PNULL, &drm_rights);
    drm_permission = MMIAPIDRM_GetFilePermission (drm_handle, PNULL);
    is_forward = MMIAPIDRM_IsCanForward (&drm_rights, SFS_INVALID_HANDLE, PNULL);

    if (MMIAPIDRM_IsRightsValid (drm_handle, PNULL, TRUE, drm_permission))
    {
        const DRM_CONSTRAINT_T *rights_cons_ptr = PNULL;

        //数字版权:有效
        title_name = TXT_DRM_DIGITAL_RIGHTS;
        title_value = TXT_DRM_DIGITAL_RIGHTS_VALID;
        AddOneDetailItem (ctrl_id, title_name, title_value, &nIndex, PNULL, 0);

        //转发禁止:是
        title_name = TXT_DRM_FORWARD_FORBIDDEN;
        title_value = is_forward ? TXT_DRM_FORWARD_FBN_NO : TXT_DRM_FORWARD_FBN_YES;
        AddOneDetailItem (ctrl_id, title_name, title_value, &nIndex, PNULL, 0);

        //只有下面这两种类型的才可能会有各种限制等
        if (DRM_LEVEL_CD == drm_rights.method_level
                || DRM_LEVEL_SD == drm_rights.method_level)
        {
            //先得到各个限制的数据
            rights_cons_ptr = & (drm_rights.constraint);
//            if (PNULL == rights_cons_ptr)
//            {
//                SFS_CloseFile(drm_handle);
//                return;
//            }

            //限制可能是组合的，也可以单独存在，但是datetime 与interval只能取一个
            if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_COUNT))
            {
                //剩余次数:6/100
                SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
                SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
                title_name = TXT_DRM_REMAIND_NUMBER;
                sprintf ( (char*) detail_arr, "%u/%u", rights_cons_ptr->total_count - rights_cons_ptr->used_count, rights_cons_ptr->total_count);
                MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, title_name, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));
            }

            if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_INTERVAL))
            {
                uint8 detail_arr1[MMIFMM_FULL_FILENAME_LEN] = {0};
                DRM_RIGHTS_INTERVAL_T interval_time = {0};

                MMIAPIDRM_GetDateIntervalTime (drm_handle, &interval_time);

                //版权开始日期:2011.3.9 12:10
                SCI_MEMSET (detail_arr1, 0, sizeof (detail_arr1));
                SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
                title_name = TXT_DRM_REMAIND_TIME;

                switch (MMIAPISET_GetDateDisplayType())
                {
                case MMISET_DATE_YMD:
                    sprintf ( (char*) detail_arr1, "%4dY-%2dM-%2dD %2dH:%2dM:%2dS",
                              interval_time.date.year, interval_time.date.mon, interval_time.date.mday,
                              interval_time.time.hour, interval_time.time.min, interval_time.time.sec);
                    break;

                case MMISET_DATE_MDY:
                    sprintf ( (char*) detail_arr1, "%2dM-%2dD-%4dY %2dH:%2dM:%2dS",
                              interval_time.date.mon, interval_time.date.mday, interval_time.date.year,
                              interval_time.time.hour, interval_time.time.min, interval_time.time.sec);
                    break;

                case MMISET_DATE_DMY:
                    sprintf ( (char*) detail_arr1, "%2dD-%2dM-%4dY %2dH:%2dM:%2dS",
                              interval_time.date.mday, interval_time.date.mon, interval_time.date.year,
                              interval_time.time.hour, interval_time.time.min, interval_time.time.sec);
                    break;

                default:
                    sprintf ( (char*) detail_arr1, "%4dY-%2dM-%2dD %2dH:%2dM:%2dS",
                              interval_time.date.year, interval_time.date.mon, interval_time.date.mday,
                              interval_time.time.hour, interval_time.time.min, interval_time.time.sec);
                    break;
                }


                MMIAPICOM_StrToWstr (detail_arr1, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, title_name, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr1));
            }

            if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_DATETIME))
            {
                //等明天需求定义清楚了再来添加剩余时间
                uint8 detail_arr1[MMIFMM_FULL_FILENAME_LEN] = {0};
                uint8 detail_arr2[MMIFMM_FULL_FILENAME_LEN] = {0};
                MMIDRM_NITZ_TYPE_T nitz_time = {0};
                uint32 time_value = 0;

                MMIAPIDRM_GetDrmNitzTime (&nitz_time);
                time_value = nitz_time.is_syn_time ? nitz_time.sys_time_value : 0;

                //版权开始日期:2011.3.9 12:10
                SCI_MEMSET (detail_arr1, 0, sizeof (detail_arr1));
                SCI_MEMSET (detail_arr2, 0, sizeof (detail_arr2));
                SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
                title_name = TXT_DRM_BEGIN_TIME;

                if (DRM_NET_TIME_ADD == nitz_time.net_time_type)
                {
                    time_value = rights_cons_ptr->start_time + time_value;
                }
                else if (DRM_NET_TIME_REDUCE == nitz_time.net_time_type)
                {
                    time_value = rights_cons_ptr->start_time - time_value;
                }
                else
                {
                    time_value = rights_cons_ptr->start_time;
                }

                tm = MMIAPICOM_Second2Tm (time_value);
                MMIAPISET_FormatDateStrByDateStyle (tm.tm_year, tm.tm_mon, tm.tm_mday, '.', detail_arr1, MMIFMM_FULL_FILENAME_LEN);
                MMIAPISET_FormatTimeStrByTime (tm.tm_hour, tm.tm_min, detail_arr2, MMIFMM_FULL_FILENAME_LEN);
                strcat ( (char*) detail_arr1, " ");
                strcat ( (char*) detail_arr1, (char*) detail_arr2);
                MMIAPICOM_StrToWstr (detail_arr1, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, title_name, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr1));


                //版权结束日期:2011.3.9 12:10
                SCI_MEMSET (detail_arr1, 0, sizeof (detail_arr1));
                SCI_MEMSET (detail_arr2, 0, sizeof (detail_arr2));
                SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
                title_name = TXT_DRM_END_TIME;
                time_value = nitz_time.is_syn_time ? nitz_time.sys_time_value : 0;

                if (DRM_NET_TIME_ADD == nitz_time.net_time_type)
                {
                    time_value = rights_cons_ptr->end_time + time_value;
                }
                else if (DRM_NET_TIME_REDUCE == nitz_time.net_time_type)
                {
                    time_value = rights_cons_ptr->end_time - time_value;
                }
                else
                {
                    time_value = rights_cons_ptr->end_time;
                }

                tm = MMIAPICOM_Second2Tm (time_value);
                MMIAPISET_FormatDateStrByDateStyle (tm.tm_year, tm.tm_mon, tm.tm_mday, '.', detail_arr1, MMIFMM_FULL_FILENAME_LEN);
                MMIAPISET_FormatTimeStrByTime (tm.tm_hour, tm.tm_min, detail_arr2, MMIFMM_FULL_FILENAME_LEN);
                strcat ( (char*) detail_arr1, " ");
                strcat ( (char*) detail_arr1, (char*) detail_arr2);
                MMIAPICOM_StrToWstr (detail_arr1, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, title_name, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr1));
            }
        }

        //modified by liyan.zhu for c++ test
        //else
        //{
        //fl类型的,无内容再显示了
        //}
    }
    else
    {
        //数字版权:无效
        title_name = TXT_DRM_DIGITAL_RIGHTS;
        title_value = TXT_DRM_DIGITAL_RIGHTS_INVALID;
        AddOneDetailItem (ctrl_id, title_name, title_value, &nIndex, PNULL, 0);
        //转发禁止:是
        title_name = TXT_DRM_FORWARD_FORBIDDEN;
        title_value = is_forward ? TXT_DRM_FORWARD_FBN_NO : TXT_DRM_FORWARD_FBN_YES;
        AddOneDetailItem (ctrl_id, title_name, title_value, &nIndex, PNULL, 0);
    }

    SFS_CloseFile (drm_handle);
    return;
}


/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void SetCommonCharacter (GUIRICHTEXT_ITEM_T* p_item)
{
    //SCI_PASSERT(PNULL != p_item,("[MMIMPEG4]:SetCommonCharacter,param error"));
    if (PNULL == p_item)
    {
        //SCI_TRACE_LOW:"[MMIFMM]:SetCommonCharacter,param error:PNULL == p_item"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2243_112_2_18_2_20_10_229, (uint8*) "");
        return;
    }

    p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
    p_item->text_type = GUIRICHTEXT_TEXT_BUF;
}


/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem (
    MMI_CTRL_ID_T   ctrl_id,
    MMI_TEXT_ID_T   label_title,
    MMI_TEXT_ID_T   label_name,
    uint16          *index_ptr,
    wchar           *detail_ptr,
    uint32          detail_len
)
{
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T detail_str = {0};/*lint !e64*/

    //set XXX title
    SetCommonCharacter (&item_data);
    MMI_GetLabelTextByLang (label_title, &detail_str);
    item_data.text_data.buf.len = detail_str.wstr_len;
    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
    GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);

    //set XXX name
    if (PNULL == label_name && PNULL != detail_ptr)
    {
        SCI_MEMSET (&item_data, 0, sizeof (item_data));
        SetCommonCharacter (&item_data);
        item_data.text_data.buf.len = (uint16) detail_len;
        item_data.text_data.buf.str_ptr = detail_ptr;
        GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);
    }
    else if (PNULL != label_name)
    {
        SCI_MEMSET (&item_data, 0, sizeof (item_data));
        SCI_MEMSET (&detail_str, 0, sizeof (detail_str));
        SetCommonCharacter (&item_data);
        MMI_GetLabelTextByLang (label_name, &detail_str);
        item_data.text_data.buf.len = detail_str.wstr_len;
        item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
        GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"AddOneDetailItem fail "
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2512_112_2_18_2_20_11_233, (uint8*) "");
    }

}

/*****************************************************************************/
//  Description : set the select drm file constraint
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsSelDrmConstraintFile (
    uint16 *filename,
    uint16 name_len,
    MMIFMM_DRM_LIMIT_TYPE_T limit_value
)
{
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    DRM_RIGHTS_T drm_rights = {0};
    MMIDRMFILE_ERROR_E file_error = SFS_NO_ERROR;
    BOOLEAN is_ret = FALSE;
    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;
    BOOLEAN is_valid_check = FALSE;

    if (PNULL == filename
            || 0 == name_len
            || (0 == limit_value.cons_mode_limit
                && 0 == limit_value.method_level_limit))
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIFMM_IsSelDrmConstraintFile: invalid param!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_6074_112_2_18_2_20_18_267, (uint8*) "");
        return FALSE;
    }

    if (0 != limit_value.cons_mode_limit
            || 0 != limit_value.method_level_limit)
    {
        file_handle = MMIAPIFMM_CreateFile (filename, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);

        if (SFS_INVALID_HANDLE != file_handle)
        {
            if (MMIAPIDRM_IsDRMFile (file_handle, PNULL))
            {
                //优先检查是否需要版权有效
                if (limit_value.is_rights_valid)
                {
                    drm_permission = MMIAPIDRM_GetFilePermission (file_handle, PNULL);
                    is_valid_check = MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, drm_permission);
                }

                //需要版权检查，而且版权无效，则立马返回
                if (limit_value.is_rights_valid && !is_valid_check)
                {
                    is_ret = TRUE;
                }
                else
                {
                    file_error = MMIAPIDRM_GetRightsinfo (file_handle, PNULL, &drm_rights);

                    if (SFS_NO_ERROR == file_error)
                    {
                        //先检查是否消费类型的限制
                        if (0 != limit_value.cons_mode_limit)
                        {
                            if (0 != (drm_rights.constraint.type & DRM_CONSTRAINT_COUNT)	/*lint !e655*/
                                    && (0 != (limit_value.cons_mode_limit & MMIFMM_DRM_CONS_LIMIT_COUNT)))
                            {
                                is_ret = TRUE;
                            }
                            else if (0 != (drm_rights.constraint.type & DRM_CONSTRAINT_INTERVAL)	/*lint !e655*/
                                     && (0 != (limit_value.cons_mode_limit & MMIFMM_DRM_CONS_LIMIT_INTERVAL)))
                            {
                                is_ret = TRUE;
                            }
                            else if (0 != (drm_rights.constraint.type & DRM_CONSTRAINT_DATETIME)	/*lint !e655*/
                                     && (0 != (limit_value.cons_mode_limit & MMIFMM_DRM_CONS_LIMIT_DATETIME)))
                            {
                                is_ret = TRUE;
                            }
                            else
                            {
                                is_ret = FALSE;
                            }
                        }

                        //如果消费类型不限，则接着检查是否转发类型限制
                        if (!is_ret && 0 != limit_value.method_level_limit)
                        {
                            if (0 != (drm_rights.method_level & DRM_LEVEL_FL)	/*lint !e655*/
                                    && (0 != (limit_value.method_level_limit & MMIFMM_DRM_METHOD_LIMIT_FL)))
                            {
                                is_ret = TRUE;
                            }
                            else if (0 != (drm_rights.method_level & DRM_LEVEL_CD)	/*lint !e655*/
                                     && (0 != (limit_value.method_level_limit & MMIFMM_DRM_METHOD_LIMIT_CD)))
                            {
                                is_ret = TRUE;
                            }
                            else if (0 != (drm_rights.method_level & DRM_LEVEL_SD)	/*lint !e655*/
                                     && (0 != (limit_value.method_level_limit & MMIFMM_DRM_METHOD_LIMIT_SD)))
                            {
                                is_ret = TRUE;
                            }
                            else
                            {
                                is_ret = FALSE;
                            }
                        }
                    }
                    else
                    {
                        //是DRM文件，但是又取不到版权的非法文件，则一样不让往下走
                        is_ret = TRUE;
                    }
                }
            }
            else
            {
                //非drm文件
                is_ret = FALSE;
            }
        }

        MMIAPIFMM_CloseFile (file_handle);
    }
    else
    {
        //无任何受限
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIFMM_IsSelDrmConstraintFile: no any limit value!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_6171_112_2_18_2_20_18_268, (uint8*) "");
        is_ret = FALSE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : combine drm suffix
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 通过非DRM的后缀，返回组合上drm的后缀
/*****************************************************************************/
PUBLIC uint32 MMIFMM_CombineDrmSuffixInfo (uint32 suffix_type)
{
    //如果此种后缀drm支持，则需要查找时需要加上drm的相关后缀

    BOOLEAN is_support = FALSE;
    uint32 new_suffix = suffix_type;

    is_support = (
                     (MMIFMM_PIC_ALL & suffix_type)
                     || (MMIFMM_MUSIC_ALL & suffix_type)
                     || (MMIFMM_MOVIE_ALL & suffix_type)
                     || (MMIFMM_TXT_ALL & suffix_type)
                     || (MMIFMM_JAVA_JAR & suffix_type)
                     || (MMIFMM_JAVA_JAD & suffix_type)
                 );

    if (is_support)
    {
        new_suffix = suffix_type | MMIFMM_DM_FILE;
        new_suffix = new_suffix | MMIFMM_DCF_FILE;
    }

    return new_suffix;
}


/*****************************************************************************/
//  Description : drm support type
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDrmSupportType (uint32 suffix_type)
{
    BOOLEAN is_ret = FALSE;
    uint32 i = 0;
    uint32 support_type[] = {MMIFMM_PIC_ALL, MMIFMM_MUSIC_ALL, MMIFMM_MOVIE_ALL, MMIFMM_TXT_ALL};

    for (i = 0; i < ARR_SIZE (support_type); i++)
    {
        is_ret = ( (MMIFMM_FILE_TYPE (suffix_type) == MMIFMM_FILE_TYPE (support_type[i])) //大类别相同
                   && (MMIFMM_MEDIA_TYPE (suffix_type) & MMIFMM_MEDIA_TYPE (support_type[i])) //小类别包含
                 );

        //is_ret = (suffix_type & support_type[i]);
        if (is_ret)
        {
            return is_ret;
        }
    }

    return is_ret;
}


/*****************************************************************************/
//  Description : handle download drm rights file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
//LOCAL MMI_RESULT_E HandleDrmDLRoQueryMsg(
//    MMI_WIN_ID_T win_id,
//    MMI_MESSAGE_ID_E msg_id,
//    DPARAM param
//)
//{
//    MMI_RESULT_E        is_ret = MMI_RESULT_TRUE;
//    MMIPUB_INFO_T       *pub_win_ptr = PNULL;
//    wchar               *drmfile_path_ptr = PNULL;
//
//    //get pubwin data
//    pub_win_ptr = MMIPUB_GetWinAddDataPtr(win_id);
//    if (PNULL == pub_win_ptr)
//    {
//        return is_ret;
//    }
//
//    //get file info
//    drmfile_path_ptr = (wchar *)pub_win_ptr->user_data;
//    if (PNULL == drmfile_path_ptr)
//    {
//        return is_ret;
//    }
//
//    switch (msg_id)
//    {
//    case MSG_APP_WEB:
//    case MSG_APP_OK:
//    case MSG_CTL_OK:
//        //down copyrights
//        MMIAPIDRM_DownloadRightsFile(SFS_INVALID_HANDLE, drmfile_path_ptr);
//
//        MMK_CloseWin(win_id);
//        break;
//
//    case MSG_APP_CANCEL:
//    case MSG_CTL_CANCEL:
//        MMK_CloseWin(win_id);
//        break;
//
//    case MSG_CLOSE_WINDOW:
//        //free memory
//        SCI_FREE(drmfile_path_ptr);
//
//        MMK_FreeWinAddData(win_id);
//        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
//        break;
//
//    default:
//        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
//        break;
//    }
//
//    return (is_ret);
// }

/*****************************************************************************/
//  Description : handle drm copy files query window msg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
//LOCAL MMI_RESULT_E HandleDrmCopyFilesQueryMsg(
//    MMI_WIN_ID_T win_id,
//    MMI_MESSAGE_ID_E msg_id,
//    DPARAM param
//)
//{
//    MMI_RESULT_E            result = MMI_RESULT_TRUE;
//
//    switch (msg_id)
//    {
//    case MSG_APP_WEB:
//    case MSG_APP_OK:
//        MMK_CreateWin((uint32*)MMIFMM_COPY_SELECTFILE_TOFOLDER_WIN_TAB, PNULL);
//        MMK_CloseWin(win_id);
//        break;
//
//    case MSG_APP_CANCEL:
//        MMK_CloseWin(win_id);
//        break;
//
//    case MSG_CLOSE_WINDOW:
////        MMK_FreeWinAddData(win_id);
//        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
//        break;
//
//    default:
//        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
//        break;
//    }
//
//    return (result);
// }


/****************************************************************************/
//  Description : handle download popmenu window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/****************************************************************************/
LOCAL MMI_RESULT_E  DrmDownloadPopMenuWinMsg (
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    wchar *full_path_ptr = PNULL;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;

    full_path_ptr = (wchar*) MMK_GetWinAddDataPtr (win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        MMK_SetAtvCtrl (win_id, MMIFMM_DRM_DOWNLOAD_OPT_MENU_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        break;
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_OK:
        GUIMENU_GetId (MMIFMM_DRM_DOWNLOAD_OPT_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *) (&group_id), (MMI_MENU_ID_T *) (&menu_id));

        switch (menu_id)
        {
        case ID_FMM_DRM_DOWNLOAD:
            //open browser
            //MMIPUB_OpenAlertSuccessWin(TXT_DRM_PLEASE_WAITING);
            MMIAPIDRM_DownloadRightsFile (SFS_INVALID_HANDLE, full_path_ptr);
            MMK_CloseWin (win_id);
            break;

        default:
            break;
        }

        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin (win_id);
        break;

    case MSG_CLOSE_WINDOW:

        if (PNULL != full_path_ptr)
        {
            SCI_FREE (full_path_ptr);
        }

        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : create drm pop menu
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIFMM_DrmOpenDownloadMenuWin (wchar *full_path)
{
    //create download option menu
    wchar *path = PNULL;

    path = SCI_ALLOCA ( (MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof (wchar));

    if (PNULL == path)
    {
        //SCI_TRACE_LOW:"[MMIFMM:]MMIAPIFMM_DrmOpenDownloadMenuWin = PNULL!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13092_112_2_18_2_21_2_558, (uint8*) "");
        return MMI_RESULT_FALSE;
    }

    SCI_MEMSET (path, 0, (MMIFILE_FULL_PATH_MAX_LEN + 1) *sizeof (wchar));
    SCI_MEMCPY (path, full_path, (MMIFILE_FULL_PATH_MAX_LEN + 1) *sizeof (wchar));
    MMK_CreateWin ( (uint32*) MMIFMM_DRM_DOWNLOAD_MENU_WIN_TAB, (ADD_DATA) path);
    return TRUE;

}

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 是否可以预览，如果不能预览，则需要提示下载
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateListIconData (
    MMI_CTRL_ID_T ctrl_id,
    uint16 index,
    const wchar *file_dir_ptr,              //文件所在文件夹名称
    const wchar *file_path_ptr              //文件全路径
)
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_T *item_ptr = PNULL;
    MMI_IMAGE_ID_T icon_id = IMAGE_NULL;
		//添加PICTHUMBNAIL_SUPPORT控制
#if (defined MMI_FMM_PICTHUMBNAIL_SUPPORT) || (defined VIDEOTHUMBNAIL_SUPPORT)//coverity 18314
    BOOLEAN is_two_line = FALSE;
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT     
    BOOLEAN                  is_videotwo_line = FALSE;
#endif
    BOOLEAN is_replace = FALSE;
    uint16 file_len = 0;

    if (PNULL == file_path_ptr
            || PNULL == file_dir_ptr)
    {
        return is_replace;
    }

    file_len = (uint16) MMIAPICOM_Wstrlen (file_dir_ptr);
#ifdef MMI_FMM_PICTHUMBNAIL_SUPPORT 
	is_two_line = MMIFMM_IsDefaultPicFolder(file_dir_ptr, file_len);
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT
    is_videotwo_line  = MMIFMM_IsDefaultVideoFolder (file_dir_ptr, file_len);
    is_two_line = is_two_line || is_videotwo_line;
#endif
    item_ptr = GUILIST_GetItemPtrByIndex (ctrl_id, index);	/*lint !e605*/

    if (PNULL != item_ptr)
    {
        //把原来的内容都copy过来，其实哥只想改左边显示图标
        SCI_MEMCPY (&item_t, item_ptr, sizeof (GUILIST_ITEM_T));
        SCI_MEMCPY (&item_data, item_ptr->item_data_ptr, sizeof (GUILIST_ITEM_DATA_T));

        item_t.item_data_ptr = &item_data;

        /* icon */
#if (defined MMI_FMM_PICTHUMBNAIL_SUPPORT) || (defined VIDEOTHUMBNAIL_SUPPORT)//coverity 19314
        if (is_two_line)
        {
            icon_id = IMAGE_DRM_PIC_LOCK_ICON;
        }
        else
#endif
        {
            icon_id = MMIAPIFMM_GetDrmIconId ( (uint16 *) file_path_ptr);	/*lint !e605*/
        }

        if (icon_id != item_data.item_content[0].item_data.image_id)
        {
            item_data.item_content[0].item_data.image_id = icon_id;
            is_replace = TRUE;
        }

        if (is_replace)
        {
            GUILIST_ReplaceItem (ctrl_id, &item_t, index); 	/*lint !e605*/
        }
    }

    return is_replace;
}
/*****************************************************************************/
//  Description : get drm icon id by full path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetDrmIconId (uint16 *file_path_ptr)
{
    MMI_IMAGE_ID_T icon_id = IMAGE_DRM_UNKNOWN_LOCK_ICON;
    BOOLEAN is_valid = FALSE;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMIDRMFILE_HANDLE drm_handle = 0;
    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

    if (PNULL == file_path_ptr)
    {
        return icon_id;
    }

    drm_handle = SFS_CreateFile (file_path_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE == drm_handle)
    {
        return icon_id;
    }

    file_type = MMIAPIDRM_GetMediaFileType (drm_handle, PNULL);
    drm_permission = MMIAPIDRM_GetFilePermission (drm_handle, PNULL);
    is_valid = MMIAPIDRM_IsRightsValid (drm_handle, PNULL, TRUE, drm_permission);

    switch (file_type)
    {
    case MMIFMM_FILE_TYPE_PICTURE:
        icon_id = is_valid ? IMAGE_DRM_IMAGE_UNLOCK_ICON : IMAGE_DRM_IMAGE_LOCK_ICON;
        break;

    case MMIFMM_FILE_TYPE_MUSIC:
        icon_id = is_valid ? IMAGE_DRM_AUDIO_UNLOCK_ICON : IMAGE_DRM_AUDIO_LOCK_ICON;
        break;

    case MMIFMM_FILE_TYPE_MOVIE:
        icon_id = is_valid ? IMAGE_DRM_VIDEO_UNLOCK_ICON : IMAGE_DRM_VIDEO_LOCK_ICON;
        break;
#if defined MMI_VCARD_SUPPORT
    case MMIFMM_FILE_TYPE_VCARD:
        icon_id = is_valid ? IMAGE_DRM_UNKNOWN_UNLOCK_ICON : IMAGE_DRM_UNKNOWN_LOCK_ICON;
        break;
#endif
#ifdef EBOOK_SUPPORT
    case MMIFMM_FILE_TYPE_EBOOK:
        icon_id = is_valid ? IMAGE_DRM_TEXT_UNLOCK_ICON : IMAGE_DRM_TEXT_LOCK_ICON;
        break;
#endif

#ifdef JAVA_SUPPORT
    case MMIFMM_FILE_TYPE_JAVA:
        icon_id = is_valid ? IMAGE_DRM_JAVA_UNLOCK_ICON : IMAGE_DRM_JAVA_LOCK_ICON;
        break;
#endif

    default:
        icon_id = is_valid ? IMAGE_DRM_UNKNOWN_UNLOCK_ICON : IMAGE_DRM_UNKNOWN_LOCK_ICON;
        break;
    }

    SFS_CloseFile (drm_handle);

    return icon_id;
}

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 本函数用于更新选择文件夹中的图标
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateSelectListIconData (
    wchar *file_path_ptr,
    uint16 file_path_len
)
{
    BOOLEAN is_ret = FALSE;
    uint16 path_len = MMIFILE_FILE_NAME_MAX_LEN;
    wchar *new_path_ptr = PNULL;
    uint16 cur_index = 0;

    if (PNULL != file_path_ptr && 0 != file_path_len)
    {
        cur_index = GUILIST_GetCurItemIndex (MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID);
        new_path_ptr = SCI_ALLOC_APPZ ( (path_len + 1) * sizeof (wchar));

        MMIAPIFMM_SplitFullPathExt (file_path_ptr, file_path_len,
                                    new_path_ptr, &path_len, PNULL, PNULL, PNULL, PNULL);

        //如果是无效的，则需要提示guilist去刷新本行
        MMIAPIFMM_UpdateListIconData (MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID, cur_index, new_path_ptr, file_path_ptr);

        SCI_FREE (new_path_ptr);

        is_ret = TRUE;
    }
    else
    {
        is_ret = FALSE;
    }

    return is_ret;
}


/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIFMM_ConvertMimeType (
    MIME_TYPE_E mime_type
)
{
    MMIFMM_FILE_TYPE_E media_type = MMIFMM_FILE_TYPE_NORMAL;

    switch (mime_type)
    {
    case MIME_TYPE_IMAGE_ANY:                // image start
    case MIME_TYPE_IMAGE_GIF:
    case MIME_TYPE_IMAGE_JPEG:
    case MIME_TYPE_IMAGE_TIFF:
    case MIME_TYPE_IMAGE_PNG:
    case MIME_TYPE_IMAGE_VND_WAP_WBMP:
    case MIME_TYPE_IMAGE_SVG:
    case MIME_TYPE_IMAGE_BMP:
    case MIME_TYPE_IMAGE_X_UP_WPNG:
        media_type = MMIFMM_FILE_TYPE_PICTURE;   // 图片文件
        break;

    case MIME_TYPE_TEXT_ANY:         // "text/*"
    case MIME_TYPE_TEXT_HTML:        // "text/html"
    case MIME_TYPE_TEXT_PLAIN:       // "text/plain"
        //case MIME_TYPE_TEXT_X_VCALENDAR: // "text/x-vcalendar
        //case MIME_TYPE_TEXT_X_VCARD:     // "text/x-vcard"
    case MIME_TYPE_TEXT_XML:         // "text/xml"
        media_type = MMIFMM_FILE_TYPE_EBOOK;     // 电子书
        break;
#if defined MMI_VCARD_SUPPORT
    case MIME_TYPE_TEXT_X_VCARD:                // "text/x-vcard"
        media_type = MMIFMM_FILE_TYPE_VCARD;    // 电子书
        break;
#endif
#ifdef JAVA_SUPPORT
    case MIME_TYPE_APPLICATION_JAVA_VM:
    case MIME_TYPE_APPLICATION_JAVA_JAR:
    case MIME_TYPE_APPLICATION_JAVA_JAD:
        media_type = MMIFMM_FILE_TYPE_JAVA; 	//JAVA 文件
        break;
#endif

    case MIME_TYPE_AUDIO_ANY:                // audio start
    case MIME_TYPE_AUDIO_MID:
    case MIME_TYPE_AUDIO_MPEG:
    case MIME_TYPE_AUDIO_X_WAV:
    case MIME_TYPE_AUDIO_AMR:
    case MIME_TYPE_AUDIO_X_AAC:
    case MIME_TYPE_AUDIO_MIDI:
    case MIME_TYPE_AUDIO_X_MIDI:
    case MIME_TYPE_AUDIO_SP_MIDI:
    case MIME_TYPE_AUDIO_IMELODY:
        media_type = MMIFMM_FILE_TYPE_MUSIC;     // 音乐文件
        break;

    case MIME_TYPE_VIDEO_MPEG:
    case MIME_TYPE_VIDEO_3GPP:
    case MIME_TYPE_VIDEO_AVI:
    case MIME_TYPE_VIDEO_MP4:
        media_type = MMIFMM_FILE_TYPE_MOVIE;     // 影像文件
        break;

    default:
        break;

    }

    return media_type;
}
#endif