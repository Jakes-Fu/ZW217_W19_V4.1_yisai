/*****************************************************************************
** File Name:      mmipicview_drm.c                                          *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_DRM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "guilistbox.h"
#include "mmi_modu_main.h"
#include "mmipicview_id.h"
#include "mmipicview_internal.h"
#include "mmifmm_export.h"
#include "mmidrm_export.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmimms_export.h"
#include "mmibt_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_picview_is_down_right = FALSE;
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//send info
typedef struct
{
    BOOLEAN             is_mms;     //is mms
    FILEARRAY_DATA_T    file_data;  //file data
} MMIPICVIEW_DRM_SEND_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : is drm file has right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrmRight(
                         MMIFILE_HANDLE     file_handle
                         );

/*****************************************************************************/
//  Description : get drm file type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL DRM_METHOD_LEVEL_E GetDrmFileType(
                                        MMIFILE_HANDLE  file_handle
                                        );

/*****************************************************************************/
//  Description : handle drm preview query window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmPreviewQueryMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );

/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrmDelete(
                          wchar     *file_name_ptr, //in:
                          uint16    file_name_len   //in:
                          );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : is drm file by suffix name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDrmFile(
                                    wchar   *file_name_ptr, //in:
                                    uint16  file_name_len   //in:
                                    )
{
    BOOLEAN     result = FALSE;
    wchar       suffix[MMIPICVIEW_SUFFIX_MAX_LEN+1] = {0};
    const wchar suffix_dm[] = {'.','d','m',0};
    const wchar suffix_dcf[] = {'.','d','c','f',0};
    uint16      suffix_len = MMIPICVIEW_SUFFIX_MAX_LEN;
    uint16      dm_len = 0;
    uint16      dcf_len = 0;

    //split file suffix
    MMIAPIFMM_SplitFullPathExt(file_name_ptr,
        file_name_len,
        PNULL,PNULL,
        PNULL,PNULL,
        suffix,&suffix_len);

    //get dm and dcf suffix len
    dm_len  = (uint16)MMIAPICOM_Wstrlen(suffix_dm);
    dcf_len = (uint16)MMIAPICOM_Wstrlen(suffix_dcf);

    if ((0 == MMIAPICOM_CompareTwoWstrExt(suffix,suffix_len,suffix_dm,dm_len,FALSE)) ||
        (0 == MMIAPICOM_CompareTwoWstrExt(suffix,suffix_len,suffix_dcf,dcf_len,FALSE)))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is drm file has right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDrmRight(
                                     wchar      *file_name_ptr, //in:
                                     uint16     file_name_len   //in:
                                     )
{
    BOOLEAN             result = FALSE;
    MMIFILE_HANDLE      file_handle = 0;

    //open file
    file_handle = MMIAPIFMM_CreateFile(file_name_ptr,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);

    //is has right
    result = IsDrmRight(file_handle);

    //close file
    MMIAPIFMM_CloseFile(file_handle);

    //GUI_INVALID_PARAM(file_name_len);

    return (result);
}

/*****************************************************************************/
//  Description : is drm file has right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrmRight(
                         MMIFILE_HANDLE     file_handle
                         )
{
    BOOLEAN     result = FALSE;

    if (SFS_INVALID_HANDLE != file_handle)
    {
        result = MMIAPIDRM_IsRightsValid(file_handle, PNULL, TRUE, DRM_PERMISSION_DISPLAY);
    }

    return (result);
}

/*****************************************************************************/
//  Description : is preview drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsPreview(
                                    BOOLEAN     is_prompt,  //in:
                                    uint32      item_index  //in:
                                    )
{
    BOOLEAN             result = FALSE;
    MMIFILE_HANDLE      file_handle = 0;
    FILEARRAY_DATA_T    file_info = {0};
    FILEARRAY_DATA_T    *send_data_ptr = PNULL;
    DRM_METHOD_LEVEL_E  file_type = DRM_LEVEL_FL;
    DRMFILE_PRE_CHECK_STATUS_E  ret_value = DRMFILE_PRE_CHECK_INVALID_PARAM;
    
    //get file info, is drm file
    if ((MMIPICVIEW_GetPicFileInfo(item_index,&file_info)) && 
        (MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)))
    {
        //open file
        file_handle = MMIAPIFMM_CreateFile(file_info.filename,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
        
        //is it has right
        if (IsDrmRight(file_handle))
        {
            result = TRUE;
        }
        else
        {
            //modify
            MMIPICVIEW_ModifyItemIcon(MMIPICVIEW_LIST_CTRL_ID,(uint16)item_index);

            //is prompt
            if (is_prompt)
            {
            	ret_value = MMIAPIDRM_PreCheckFileStatus(file_info.filename, DRM_PERMISSION_DISPLAY);
                if (DRMFILE_PRE_CHECK_NORMAL == ret_value)
                {
                    //get drm file type
                    file_type = GetDrmFileType(file_handle);
                    switch (file_type)
                    {
                    case DRM_LEVEL_SD:
                        //alloc memory
                        send_data_ptr = SCI_ALLOC_APPZ(sizeof(FILEARRAY_DATA_T));

                        //set send param
                        MMI_MEMCPY(send_data_ptr,sizeof(FILEARRAY_DATA_T),
                            &file_info,sizeof(FILEARRAY_DATA_T),
                            sizeof(FILEARRAY_DATA_T));

                        MMIPUB_OpenQueryWinByTextIdEx(
                            MMK_GetFirstAppletHandle(),
                            TXT_DRM_COPYRIGHTS_INVLIAD_DOWNLOAD_RO,
                            IMAGE_PUBWIN_QUERY,
                            PNULL,
                            HandleDrmPreviewQueryMsg,
                            (uint32)send_data_ptr);
                        break;

                    default:
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);
                        break;
                    }
                }
                else
                {
                    if(DRMFILE_PRE_CHECK_NO_RIGHTS == ret_value)
                    {
                        MMIPICVIEW_SetIsDownloadRightsFile(TRUE);
                    }
                    result = FALSE;
                }
            }
        }

        //close file
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get drm file type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL DRM_METHOD_LEVEL_E GetDrmFileType(
                                        MMIFILE_HANDLE  file_handle
                                        )
{
    DRM_METHOD_LEVEL_E  file_type = DRM_LEVEL_INVALID;

    //open file
    if (SFS_INVALID_HANDLE != file_handle)
    {
        file_type = MMIAPIDRM_GetDRMFileType(file_handle, PNULL);
    }

    return (file_type);
}

/*****************************************************************************/
//  Description : handle drm preview query window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmPreviewQueryMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pub_win_ptr = PNULL;
    FILEARRAY_DATA_T    *file_info_ptr = PNULL;

    //get pubwin data
    pub_win_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    //get file info
    if(PNULL != pub_win_ptr)
    {
        file_info_ptr = (FILEARRAY_DATA_T *)pub_win_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //down copyrights
        if(PNULL != file_info_ptr)
        {
            if(MMIAPIDRM_DownloadRightsFile(SFS_INVALID_HANDLE, file_info_ptr->filename))
        	{
        		MMIPICVIEW_SetIsDownloadRightsFile(TRUE);
        	}
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        SCI_FREE(file_info_ptr);

        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;

    default:
        MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is send drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSend(
                                 BOOLEAN            is_sms,
                                 FILEARRAY_DATA_T   *file_data_ptr
                                 )
{
    BOOLEAN                 result = FALSE;
    MMIFILE_HANDLE          file_handle = 0;
    MMIFILE_FILE_INFO_T     file_info = {0};
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
    //open file
    file_handle = MMIAPIFMM_CreateFile(file_data_ptr->filename,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
    
    //is drm file
    if (MMIAPIDRM_IsDRMFile(file_handle, PNULL))
    {
        //can this file forward 
        if (MMIAPIDRM_IsCanForward(PNULL, file_handle, PNULL))
        {
            if (is_sms)
            {
                //send by mms
                MMIAPIFMM_GetFileInfoFormFullPath(file_data_ptr->filename,
                    file_data_ptr->name_len,
                    &file_info);
                MMIAPIMMS_EditMMSFromFile(&file_info,MMIFMM_FILE_TYPE_NORMAL);
            }
            else
            {
                //send by bt
                MMIAPICOM_Wstrncpy(send_file_info.filepath_name,file_data_ptr->filename,file_data_ptr->name_len);
                send_file_info.filepath_len = file_data_ptr->name_len;
                send_file_info.file_size = file_data_ptr->size;
                send_file_info.is_temp_file = FALSE;
                MMIAPIBT_SendMultiFile(&send_file_info,1);
		    }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
        }
    }
    else
    {
        result = TRUE;
    }

    //close file
    MMIAPIFMM_CloseFile(file_handle);

    return (result);
}

/*****************************************************************************/
//  Description : is delete drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDeleteDrm(
                                      MMI_CTRL_ID_T		ctrl_id    //in:
                                      )
{
    BOOLEAN             result = FALSE;
    uint16              i = 0;
    uint16              mark_num = 0;
    uint16              item_index = 0;
    uint16              *item_index_ptr = PNULL;
    FILEARRAY_DATA_T    file_info = {0};

    //get selected item number
    mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0);
    if (0 < mark_num)
    {
        //alloc memory
        item_index_ptr = SCI_ALLOC_APPZ(mark_num * sizeof(uint16));

        MMIPICVIEW_GetSelectItemIndex(ctrl_id,item_index_ptr,mark_num);
        
        for (i=0; i<mark_num; i++)
        {
            //get file info
            SCI_MEMSET(&file_info,0,sizeof(FILEARRAY_DATA_T));
            if (MMIPICVIEW_GetPicFileInfo(item_index_ptr[i],&file_info))
            {
                if (IsDrmDelete(file_info.filename,file_info.name_len))
                {
                    result = TRUE;
                    break;
                }
            }
        }
    }
    else
    {
        //ger current index
        item_index = MMIPICVIEW_GetCurIndex(ctrl_id);

        //get file info
        if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
        {
            result = IsDrmDelete(file_info.filename,file_info.name_len);
        }
    }

    //free memory
    if (PNULL != item_index_ptr)
    {
        SCI_FREE(item_index_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrmDelete(
                          wchar     *file_name_ptr, //in:
                          uint16    file_name_len   //in:
                          )
{
    BOOLEAN     result = FALSE;

    if ((MMIPICVIEW_IsDrmFile(file_name_ptr,file_name_len)) &&
        (MMIPICVIEW_IsDrmRight(file_name_ptr,file_name_len)))
    {
        //need prompt
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is drm picture file set wallpaper
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSetWallpaper(
                                         uint32     item_index  //in:
                                         )
{
    BOOLEAN             result = FALSE;
    DRM_RIGHTS_T        drm_rights = {0};
    MMIFILE_HANDLE      file_handle = 0;
    FILEARRAY_DATA_T    file_info = {0};
    
    //get file info, is drm file
    if ((MMIPICVIEW_GetPicFileInfo(item_index,&file_info)) && 
        (MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)))
    {
        //open file
        file_handle = MMIAPIFMM_CreateFile(file_info.filename,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
        
        //get right
        MMIAPIDRM_GetRightsinfo(file_handle, PNULL, &drm_rights);

        //次数限制的drm文件不能设为墙纸
        if (0 == (drm_rights.constraint.type & DRM_CONSTRAINT_COUNT))
        {
            //is it has right
            if (MMIAPIDRM_IsRightsValid(file_handle,PNULL,TRUE,DRM_PERMISSION_DISPLAY))
            {
                result = TRUE;
            }
        }

        //close file
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : add drm file detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_AddDrmDetail(
                                    wchar           *file_name_ptr, //in:
                                    uint16          file_name_len,  //in:
									MMI_WIN_ID_T	win_id,			//in:
                                    MMI_CTRL_ID_T   ctrl_id         //in:
                                    )
{
	MMIFILE_HANDLE      file_handle = 0;

    //is drm file
    if (MMIPICVIEW_IsDrmFile(file_name_ptr,file_name_len))
    {
        MMIAPIFMM_AddDrmFileDetail(file_name_ptr,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0,ctrl_id);
		
		//open file
        file_handle = MMIAPIFMM_CreateFile(file_name_ptr,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);

		//get drm file type
		if (DRM_LEVEL_SD == GetDrmFileType(file_handle))
		{
#ifdef MMI_PDA_SUPPORT
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                    GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DRM_DOWNLOAD_RIGHTS, FALSE);
                    GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, ctrl_id);
#endif       
			//set softkey
			GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_NULL,STXT_RETURN,FALSE);
		}
#ifdef MMI_PDA_SUPPORT
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
#endif       

		//close file
		MMIAPIFMM_CloseFile(file_handle);
    }
}

/*****************************************************************************/
//  Description : set if download drm rights file 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetIsDownloadRightsFile(BOOLEAN is_down)
{
	s_picview_is_down_right = is_down;
}

/*****************************************************************************/
//  Description : get if download drm rights file 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetIsDownloadRightsFile(void)
{
	return s_picview_is_down_right;
}
#endif //#ifdef PIC_VIEWER_SUPPORT
