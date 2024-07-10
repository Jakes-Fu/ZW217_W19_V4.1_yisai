/*****************************************************************************
** File Name:      watch_gallery_internal.h                                  *
** Author:         longwei.qiao                                              *
** Date:           16/02/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gallery internal interface  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020       longwei.qiao         Create                                  *
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _WATCH_GALLERY_INTERNAL_H_
#define _WATCH_GALLERY_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipicview_export.h"
#include "mmifilearray_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define   GALLERY_FILE_SUFFIX_MAX_LEN       10   //include dot
//color
#define   GALLERY_FOCUS_BORDER_COLOR        GUI_RGB2RGB565(0x17,0x51,0x55)

#define GRAY2_COLOR     GUI_RGB8882RGB565(0X999999)

#define   GALLERY_MODULE_NAME               "Gallery"

#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#define   GALLERY_BG_RECT              DP2PX_RECT(0,0,239,239)
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//gallery  file type
typedef enum
{
    MMIGALLERY_FILE_TYPE_ERROR,
    MMIGALLERY_FILE_TYPE_PIC,
    MMIGALLERY_FILE_TYPE_VIDEO,
    MMIGALLERY_FILE_TYPE_MAX
} MMIGALLERY_FILE_TYPE_E;

//gallery file video type
typedef enum
{
#ifdef AVI_DEC_SUPPORT
    MMIGALLERY_VIDEO_TYPE_AVI,
#endif
    MMIGALLERY_VIDEO_TYPE_MP4,
    MMIGALLERY_VIDEO_TYPE_3GP,
#ifdef FLV_DEC_SUPPORT
    MMIGALLERY_VIDEO_TYPE_FLV,
#endif
#ifdef MMI_RMVB_SUPPORT
    MMIGALLERY_VIDEO_TYPE_RM,
    MMIGALLERY_VIDEO_TYPE_RV,
    MMIGALLERY_VIDEO_TYPE_RMVB,
#endif
    MMIGALLERY_VIDEO_TYPE_MAX
} MMIGALLERY_VIDEO_TYPE_E;
//gallery file picture type
typedef enum
{
#ifdef BMP_DEC_SUPPORT
    MMIGALLERY_PIC_TYPE_BMP,
    MMIGALLERY_PIC_TYPE_WBMP,
#endif
#ifdef GIF_DEC_SUPPORT
    MMIGALLERY_PIC_TYPE_GIF,
#endif
#ifdef PNG_DEC_SUPPORT
    MMIGALLERY_PIC_TYPE_PNG,
#endif
	MMIGALLERY_PIC_TYPE_JPG,
    MMIGALLERY_PIC_TYPE_JPEG,
#ifdef DRM_SUPPORT
    MMIGALLERY_PIC_TYPE_DM,
    MMIGALLERY_PIC_TYPE_DCF,
#endif
    MMIGALLERY_PIC_TYPE_MAX
} MMIGALLERY_PIC_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 PUBLIC void Gallery_main_ClearScreen(void);

#if defined( ADULT_WATCH_SUPPORT) 
 /*****************************************************************************/
//  Description : Delete Cur Preview File
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Main_DeleteCurPreviewFile(void);
/*****************************************************************************/
//  Description : DrawIndicator
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Main_DrawIndicator(BOOLEAN is_draw);
#endif
 /*****************************************************************************/
//  Description : Get is dispaly preview osd flag
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:s_gallery_cur_panel_info.is_dis_osd
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Gallery_Main_IsDisplayOsd(void);
/*****************************************************************************/
//  Description : set is dispaly gallery osd
//  Paramter:
//          [In]:win_id,is_display
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void  Gallery_Main_SetDisplayOsd(MMI_WIN_ID_T win_id,BOOLEAN is_display);
/*****************************************************************************/
//  Description : display pic file preview
//  Paramter:
//          [In]:file_info
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Pic_PictureFilePreview(MMI_WIN_ID_T win_id,FILEARRAY_DATA_T* file_info);
/*****************************************************************************/
//  Description : set pic button is visible
//  Paramter:
//          [In]:win_id,is_visible,is_update
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:当需要隐藏button时，full paint可以刷新anim控件达到清除button图标的效果
/*****************************************************************************/
PUBLIC void Gallery_Pic_SetBtnVisible(MMI_WIN_ID_T win_id,BOOLEAN is_visible,BOOLEAN is_update);
/*****************************************************************************/
//  Description : handle gallery pic msg
//  Paramter:
//          [In]:win_id,msg_id,param
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Gallery_Pic_HandleMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                               );
/*****************************************************************************/
//  Description : display video file preview
//  Paramter:
//          [In]:file_info
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_VideoFilePreview(MMI_WIN_ID_T win_id,FILEARRAY_DATA_T* file_info);

/*****************************************************************************/
//  Description  :create vp fsm and init
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_CreateFsmAndInit(void);
/*****************************************************************************/
//  Description  :release vp fsm
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_FSMRelease(void);
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description  :release vp layer
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_DestoryLayer(void);
#endif

/*****************************************************************************/
//  Description : set video button is visible
//  Paramter:
//          [In]:is_visible,is_update
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_SetBtnVisible(BOOLEAN is_visible);
/*****************************************************************************/
//  Description  :stop vp play and clear display
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:清空图层显示界面，如果当前视频播放并未结束，需要停止
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Vp_StopPlayAndClearDis(void);
/*****************************************************************************/
//  Description : set video button is visible
//  Paramter:
//          [In]:is_visible
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_SetPanelVisible(BOOLEAN is_visible);
/*****************************************************************************/
//  Description : handle gallery Vp msg
//  Paramter:
//          [In]:win_id,msg_id,param
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Gallery_Vp_HandleMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                               );
/*****************************************************************************/
//  Description : get cur preview file type
//  Paramter:
//          [In]:win_id,msg_id,param
//          [Out]:None
//          [Return]:MMIGALLERY_FILE_TYPE_E
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC MMIGALLERY_FILE_TYPE_E Gallery_GetCurPreviewFileType(void);
/*****************************************************************************/
//  Description : open gallery del file query win
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Main_OpenDelQueryWin(void);

PUBLIC void Gallery_Main_ReLoadFile(void);

PUBLIC void Gallery_NextFilePreview(MMI_WIN_ID_T win_id);

#endif
#endif

