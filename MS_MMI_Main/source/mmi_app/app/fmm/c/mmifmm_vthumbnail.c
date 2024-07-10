/*****************************************************************************
** File Name:      mmifmm_vthumbnail.c                                       *
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

#define _MMIFMM_VTHUMBNAIL_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_vthumbnail.h"
#include "sci_types.h"
#include "guilistbox.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmi_image.h"
#include "mmifmm_id.h"
#include "mmifmm_comfunc.h"
#include "mmi_image.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#define THUMBNAIL_FOLDER_NAME           L"SysTumbNailRes"
#define THUMBNAIL_FOLDER_NAME_LEN  14
#define THUMBNAIL_SEEKTIME                    0//1000
#define THUMBNAIL_VIDEOWIDTH            120
#define THUMBNAIL_VIDEOHEIGHT           90

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

LOCAL GUILIST_SLIDE_STATE_E s_current_sildestate = GUILIST_STATE_MAX;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/




/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中UI 的回调函数 ，主要是存储BMP并实时刷新 ICONLIST列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/

LOCAL  BOOLEAN  FmmVideoThumbnailCallback (DPLAYER_IMAGE_DATA_T *image_data);
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中开启缩略图获取
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmVideoThumbnailOpen (wchar  *video_fullpath_name);

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中添加视频至视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmVideoThumbnailAdd (wchar  *video_fullpath_name);


/*****************************************************************************/
//  Description :获取VIDEO缩略图文件夹
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  FmmVideoThumbnailGetFloder (wchar* FloderPath, uint16 *length, wchar* videopath)
{
    BOOLEAN result = TRUE;
    int32 filename_len = 0;
    uint16  index = 0;

    if (FloderPath == PNULL || videopath == PNULL)
    {
        //SCI_TRACE_LOW:"[VP]  FmmVideoThumbnailGetFloder error parameter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13870_112_2_18_2_21_4_565, (uint8*) "");
        return FALSE;
    }

    filename_len = MMIAPICOM_Wstrlen (videopath);
    index = MMIFMM_GetFinallySlashIndex (videopath, (filename_len - 1));
    MMIAPICOM_Wstrncpy (FloderPath,  videopath, index + 1);
    MMIAPICOM_Wstrncpy (FloderPath + index + 1,  THUMBNAIL_FOLDER_NAME, THUMBNAIL_FOLDER_NAME_LEN);
    *length = MMIAPICOM_Wstrlen (FloderPath);
    return result;
}

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 通过视频全路径获取对应BMP 全路径
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIFMM_FmmVideoThumbnailGetPathname (wchar* thumbnail, uint16 *length, wchar* videopath)
{
    BOOLEAN result = TRUE;
    int32 filename_len = 0;
    int32 list_name_len = 0;
    uint16  index = 0;
    wchar  list_file_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};

    if (thumbnail == PNULL || videopath == PNULL || 0 == MMIAPICOM_Wstrlen (videopath))
    {
        //SCI_TRACE_LOW:"[VP]  FmmVideoThumbnailGetPathname error parameter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13896_112_2_18_2_21_4_566, (uint8*) "");
        return FALSE;
    }

    if (MMIAPICOM_Wstrlen (videopath) + THUMBNAIL_FOLDER_NAME_LEN + 4 + 1 <= MMIFMM_FILE_FILE_NAME_MAX_LEN) //  ""\"+.bmp" + "SysTumbNailRes"
    {
        filename_len = MMIAPICOM_Wstrlen (videopath);
        index = MMIFMM_GetFinallySlashIndex (videopath, (filename_len - 1));
        list_name_len = (uint16) (filename_len - 1 - index);
        MMIAPICOM_Wstrncpy (list_file_name,  videopath + index + 1, list_name_len);
        MMIAPICOM_Wstrncpy (list_file_name + list_name_len,  L".bmp", 4);

        MMIAPICOM_Wstrncpy (thumbnail,  videopath, index + 1);
        MMIAPICOM_Wstrncpy (thumbnail + index + 1,  THUMBNAIL_FOLDER_NAME, THUMBNAIL_FOLDER_NAME_LEN);
        SCI_MEM16SET (thumbnail + THUMBNAIL_FOLDER_NAME_LEN + index + 1, MMIFILE_SLASH, 2);
        //MMIAPICOM_Wstrncpy(thumbnail+THUMBNAIL_FOLDER_NAME_LEN+index + 1+1,  list_file_name,  MMIAPICOM_Wstrlen(list_file_name));
        MMIAPICOM_Wstrcpy (thumbnail + THUMBNAIL_FOLDER_NAME_LEN + index + 1 + 1, list_file_name);
        *length = MMIAPICOM_Wstrlen (thumbnail);
    }
    else
    {
        MMIAPICOM_Wstrncpy (thumbnail, videopath, MMIAPICOM_Wstrlen (videopath));
        *length = MMIAPICOM_Wstrlen (thumbnail);
        MMIAPICOM_Wstrncpy (thumbnail + *length - 4, L".bmp", 4);
    }

    return result;
}
/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 刷新未显示的ICONLIST
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnailUpdateIcon (MMIFMM_DATA_INFO_T* list_data_ptr,
        MMIFMM_PATH_INFO_T* current_path_ptr
                                         )
{
    // 更新数据
    wchar  thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
    uint16  thumbnail_name_len = 0;
    uint16  index = 0;
    uint16 top_index = GUILIST_GetTopItemIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID);
    uint16 item_num = GUILIST_GetPageItemNum (MMIFMM_UDISK_LISTBOX_CTRL_ID);
    item_num += 1;
    //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailUpdateIcon  !"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13935_112_2_18_2_21_4_567, (uint8*) "");

    for (index = 0; index < item_num; index++)
    {
        wchar  s_fullfile_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
        uint16 full_name_len = 0;

        if (list_data_ptr->data[top_index+index]  == PNULL)
        {
            //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailUpdateIcon  list_data_ptr->data[ top_index+index :%d ]->filename_ptr PNULL!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13942_112_2_18_2_21_4_568, (uint8*) "d", top_index + index);
            break;
        }

        full_name_len = (uint16) MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                        list_data_ptr->data[top_index+index]->filename_ptr, list_data_ptr->data[top_index+index]->filename_len,
                        s_fullfile_name, (uint16) MMIFMM_FULL_FILENAME_LEN);
        SCI_MEMSET (thumbnail_name, 0x00, (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar));
        MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_name, &thumbnail_name_len, s_fullfile_name);

        if (MMIAPIFMM_IsFileExist (thumbnail_name, thumbnail_name_len))
        {
            const GUILIST_ITEM_T*  item_t_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, top_index + index);/*lint !e605 */
            //SCI_TRACE_LOW:"MMIFMM: FmmVideoThumbnailUpdateIcon,index:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13953_112_2_18_2_21_4_569, (uint8*) "d", top_index + index);

            if (item_t_ptr == PNULL)
            {
                continue;
            }

            if (item_t_ptr->item_data_ptr == PNULL)
            {
                continue;
            }

            if (item_t_ptr->item_data_ptr->item_content[0].item_data_type ==  GUIITEM_DATA_IMAGE_ID)
            {
                GUILIST_ITEM_T    item_t = {0};
                //SCI_TRACE_LOW:"MMIFMM: FmmVideoThumbnailUpdateIcon,GUILIST_ReplaceItem index:%d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13965_112_2_18_2_21_4_570, (uint8*) "d", top_index + index);
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
                GUILIST_ReplaceItem (MMIFMM_UDISK_LISTBOX_CTRL_ID, &item_t, index + top_index);
            }
        }
    }

    MMK_SendMsg (MMIFMM_UDISK_LISTBOX_CTRL_ID, MSG_CTL_PAINT, PNULL);
}

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 加载当前页面未解析的视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIFMM_FmmVideoThumbnailLoadingMovieList (MMIFMM_DATA_INFO_T* list_data_ptr,
        MMIFMM_PATH_INFO_T* current_path_ptr
                                                       )
{
    wchar  thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
    uint16  thumbnail_name_len = 0;
    uint16  index = 0;
    uint16 top_index = 0;
    uint16 item_num = 0;
    top_index = GUILIST_GetTopItemIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID);
    item_num = GUILIST_GetPageItemNum (MMIFMM_UDISK_LISTBOX_CTRL_ID) + 1;
    //SCI_TRACE_LOW:"MMIFMM: FmmVideoThumbnailLoadingMovieList,top_index:%d,item_num:%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13987_112_2_18_2_21_4_571, (uint8*) "dd", top_index, item_num);

    if (0 == GUILIST_GetPageItemNum (MMIFMM_UDISK_LISTBOX_CTRL_ID))
    {
        return;
    }

    for (index = 0; index < item_num; index++)
    {
        wchar  s_fullfile_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
        uint16 full_name_len = 0;
        BOOLEAN getpathname_result = FALSE;

        if (PNULL == list_data_ptr->data[top_index+index]   || PNULL == list_data_ptr->data[top_index+index]->filename_ptr   || 0 == MMIAPICOM_Wstrlen (list_data_ptr->data[top_index+index]->filename_ptr))
        {
            //SCI_TRACE_LOW:"MMIFMM: FmmVideoThumbnailLoadingMovieList,list_data_ptr->data[ top_index+index:%d ]->filename_ptr PNULL"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13994_112_2_18_2_21_4_572, (uint8*) "d", top_index + index);
            continue;
        }

        full_name_len = (uint16) MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                        list_data_ptr->data[top_index+index]->filename_ptr, list_data_ptr->data[top_index+index]->filename_len,
                        s_fullfile_name, (uint16) MMIFMM_FULL_FILENAME_LEN);
        SCI_MEMSET (thumbnail_name, 0x00, (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar));
        getpathname_result = MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_name, &thumbnail_name_len, s_fullfile_name);

        if (getpathname_result && !MMIAPIFMM_IsFileExist (thumbnail_name, thumbnail_name_len))
        {
            //SCI_TRACE_LOW:"MMIFMM: FmmVideoThumbnailLoadingMovieList,FmmVideoThumbnailAdd,index:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14004_112_2_18_2_21_4_573, (uint8*) "d", top_index + index);
            FmmVideoThumbnailAdd (s_fullfile_name);
        }
    }
}


/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中UI 的回调函数 ，主要是存储BMP并实时刷新 ICONLIST列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/

LOCAL  BOOLEAN  FmmVideoThumbnailCallback (DPLAYER_IMAGE_DATA_T *image_data)
{
    wchar  thumbnail_file_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
    wchar  FloderPath [MMIFMM_FULL_FILENAME_LEN+1] = {0};
    uint16 length = 0;
    uint32  image_dataheight = 0;
    uint32  image_datawidth = 0;
    wchar* video_fullpathname = PNULL;
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailCallback !"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14025_112_2_18_2_21_4_574, (uint8*) "");
    image_dataheight = image_data->coordinate.trim_h;
    image_datawidth = image_data->coordinate.trim_w;
    video_fullpathname = MMIAPIVP_ThumbnailGetVideo();
    FmmVideoThumbnailGetFloder (FloderPath, & length, video_fullpathname);

    if (!MMIAPIFMM_IsFolderExist (FloderPath, length))
    {
        //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailCallback MMIAPIFMM_CreateDir !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14032_112_2_18_2_21_4_575, (uint8*) "");
        MMIAPIFMM_CreateDir (FloderPath, length);
        MMIAPIFMM_SetAttr (FloderPath, length, TRUE, FALSE, FALSE, FALSE);
    }

    MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_file_name, & length, video_fullpathname);
    result = MMIAPICOM_WriteBMPFile (thumbnail_file_name, image_datawidth, image_dataheight, (uint8*) image_data->buf.oneplane.p_data, 16);

    if (result && MMIAPICOM_Wstrlen (video_fullpathname) + THUMBNAIL_FOLDER_NAME_LEN + 4 + 1 > MMIFMM_FILE_FILE_NAME_MAX_LEN) //  ""\"+.bmp" + "SysTumbNailRes"
    {
        MMIAPIFMM_SetAttr (thumbnail_file_name, length, TRUE, FALSE, FALSE, FALSE);
    }


    if (MMK_IsFocusWin (MMIFMM_UDISK_CHILD_WIN_ID)) //(s_thumbnail_videoqueue->videonum==0 && MMK_IsFocusWin(MMIFMM_UDISK_CHILD_WIN_ID))
    {
		MMIFMM_VideoThumbnailUpdateIcon();
	}

    return result;
}
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中开启缩略图获取
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmVideoThumbnailOpen (wchar  *video_fullpath_name)
{
    VP_DISPLAY_PARAM_T  display_param = {0};
    GUI_RECT_T  display_rect = {0, 0, THUMBNAIL_VIDEOWIDTH, THUMBNAIL_VIDEOHEIGHT};
    BOOLEAN result = FALSE;
    display_param.audioSupport = 0;
    display_param.videoSupport = 1;
    display_param.b_support_seek = 1;
    display_param.disp_mode = DPLAYER_DISP_FULLVIEW;
    display_param.RotateAngle = 0;//GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);//s_video_player_control_info.cur_angle;
    display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
    display_param.b_no_osd = 1;
    //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailOpen  !"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14064_112_2_18_2_21_4_576, (uint8*) "");

    if (s_current_sildestate == GUILIST_START_SLIDE)
    {
        //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailOpen  slide now!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14068_112_2_18_2_21_4_577, (uint8*) "");
        return result;
    }

    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;
    display_param.disp_rect.x = display_rect.left;
    display_param.disp_rect.y = display_rect.top;

    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;

    MMIAPIVP_TranslateDisplayParamLogicToPhysical (&display_param, FALSE);

    if (MMIAPIVP_ThumbnailOpen (video_fullpath_name, MMIAPICOM_Wstrlen (video_fullpath_name) , THUMBNAIL_SEEKTIME, FmmVideoThumbnailCallback, &display_param, TRUE))
    {
        result = TRUE;
    }

    return result;

}
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中关闭缩略图获取，并重置状态
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailClose (void)
{
    //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailClose  !"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14096_112_2_18_2_21_5_578, (uint8*) "");

    if (MMIAPIVP_IsThumbnailOpen())
    {
        MMIAPIVP_ThumbnailClose();
    }

    GUILIST_SetDefaultIcon (MMIFMM_UDISK_LISTBOX_CTRL_ID, IMAGE_COMMON_PIC_ICON, NULL);
    s_current_sildestate = GUILIST_STATE_MAX;
}

/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中添加视频至视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailAdd (wchar  *video_fullpath_name)
{
    if (s_current_sildestate != GUILIST_START_SLIDE) //如果滑动状态的话，进行open操作会很慢
    {
        FmmVideoThumbnailAdd (video_fullpath_name);
    }
}
/*****************************************************************************/
//  Description : change state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumnailChangeState (
    GUILIST_SLIDE_STATE_E state,
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
)
{
    if (GUILIST_START_SLIDE == state)
    {
        //SCI_TRACE_LOW:"MMIFMM: HandleUdiskChildWindow,GUILIST_START_SLIDE"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9644_112_2_18_2_20_55_492, (uint8*) "");
        s_current_sildestate = GUILIST_START_SLIDE;
        MMIAPIVP_ThumbnailClose();
    }
    else if (GUILIST_END_SLIDE == state)
    {
        if (s_current_sildestate == GUILIST_START_SLIDE)
        {
            s_current_sildestate = GUILIST_END_SLIDE;
            MMIFMM_FmmVideoThumbnailLoadingMovieList (list_data_ptr, current_path_ptr);
        }

    }
}
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能中添加视频至视频列表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmVideoThumbnailAdd (wchar  *video_fullpath_name)
{
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnailAdd  !"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14111_112_2_18_2_21_5_579, (uint8*) "");

    if (PNULL == video_fullpath_name || 0 == MMIAPICOM_Wstrlen (video_fullpath_name))
    {
        return result;
    }

    if (MMICOM_VIDEO_TYPE_MAX == MMIAPICOM_GetMovieType (video_fullpath_name, MMIAPICOM_Wstrlen (video_fullpath_name)))
    {
        return result;
    }

    if (!MMIAPIVP_IsThumbnailOpen())
    {
        FmmVideoThumbnailOpen (video_fullpath_name);
    }
    else
    {
        MMIAPIVP_ThumbnailVideoAdd (video_fullpath_name);
        MMIAPIVP_ThumbnailResume();
    }

    result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description :VIDEO文件夹支持缩略图功能中 删除相关的文件
//  Global resource dependence : 
//  Author: 
//  Note: //cr130686
/*****************************************************************************/
PUBLIC void  MMIFMM_VideoThumbnailDeleteRelatedFile(wchar *file_name_ptr)
{
	wchar   thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1]= {0};   
    uint16  thumbnail_name_len = 0;    
    BOOLEAN result = FALSE;
       
    if (file_name_ptr == PNULL)
    {
        SCI_TRACE_LOW("FmmVideoThumbnailDeleteRelatedFile NULL POINTER");
    }
    
    result = MMIFMM_FmmVideoThumbnailGetPathname(thumbnail_name, &thumbnail_name_len, file_name_ptr);

    if (result && MMIFILE_IsFileExist(thumbnail_name, thumbnail_name_len))
    {
        //DELETE
        MMIFILE_DeleteFile(thumbnail_name, PNULL);//pclint
        SCI_TRACE_LOW("FmmVideoThumbnailDeleteRelatedFile NULL POINTER");
    }

    SCI_TRACE_LOW("FmmVideoThumbnailDeleteRelatedFile Leave result = %d", result);
}

/*****************************************************************************/
//  Description : whether file is in default video folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDefaultVideoFolder (
    const wchar      *file_name,
    uint16      name_len
)
{
    BOOLEAN     result = FALSE;
    uint16     offset = 0;
    wchar       video_name [MMIMULTIM_DIR_MOVIE_LEN+MMIFMM_PATH_NAME_OFFSET+1] = {0};
    uint16      video_len = 0;

    if (file_name == PNULL || name_len == 0)
    {
        return FALSE;
    }

    video_len = MMIAPICOM_Wstrlen(file_name);
	offset = MMIFMM_PATH_NAME_OFFSET;
	if((video_len - offset) == MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE))
	{
        MMIAPICOM_Wstrncpy(video_name,file_name,video_len);
	}
	else
	{
		return FALSE;
	}

    if (offset > name_len)
    {
        return FALSE;
    }

    if ( (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE) == (video_len - offset)))
    {
        // 判断是否是video 文件夹
        if (0 == MMIAPICOM_CompareTwoWstrExt (&video_name[offset], MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE),
                                              MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_MOVIE), FALSE))
        {
            result = TRUE;
        }
    }
    else
    {
        result = FALSE;
    }

    return result;
}
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能的状态初始化，列表的显示风格
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnail_Init (MMIFMM_PATH_INFO_T* current_path_ptr)
{
    if (MMIFMM_IsDefaultVideoFolder (current_path_ptr->pathname, current_path_ptr->pathname_len))
    {
        //SCI_TRACE_LOW:"[VP]:FmmVideoThumbnail_Init !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14176_112_2_18_2_21_5_580, (uint8*) "");
        GUILIST_SetDefaultIcon (MMIFMM_UDISK_LISTBOX_CTRL_ID, IMAGE_COMMON_MOVIE_UDISK_ICON, NULL);
        s_current_sildestate = GUILIST_STATE_MAX;
    }
    else
    {
        if (MMIAPIVP_IsThumbnailOpen())
        {
            MMIAPIVP_ThumbnailClose();
        }

        GUILIST_SetDefaultIcon (MMIFMM_UDISK_LISTBOX_CTRL_ID, IMAGE_COMMON_PIC_ICON, NULL);
    }
}

#endif
