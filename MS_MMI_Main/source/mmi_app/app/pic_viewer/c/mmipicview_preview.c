/*****************************************************************************
** File Name:      mmipicview_preview.c                                      *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#define _MMIPICVIEW_PREVIEW_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#if defined (TOUCH_GESTURE_ENABLE) && defined(PICVIEW_F_VIEW_ZOOM)
#include <math.h>
#endif
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "guilistbox.h"
#include "guisetlist.h"
#include "guibutton.h"
#include "guires.h"
#include "guictrl_api.h"
#include "mmipicview_internal.h"
#include "mmipicview_id.h"
#include "mmipicview_image.h"
#include "mmidisplay_data.h"
#include "guiiconlist.h"
#ifdef UI_P3D_SUPPORT
#include "mmitheme_special.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmipicview_text.h"
#include "mmipub.h"
#ifdef MOTION_SENSOR_TYPE   
#include "mmiset_func.h"
#endif
#include "mmimultim_image.h"
#include "mmk_tp.h"
#include "mmipicview_position.h"

#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#include "mmi_appmsg.h"
#include "mmiset_image.h"
#endif
#if defined (ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#include "watch_commonwin_export.h"
#endif
#include "mmivp_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_TITLE_STR_MAX_LEN    15      //title string max len
#define MMIPICVIEW_GLIDE_PAGE_MAXNUM       3
#define MMIPICVIEW_DISPLAY_TIME         2000    //3s
#define MMIPICVIEW_GLIDE_RATIO          4       //移动距离超过lcd的1/4,则切换
#define MMIPICVIEW_GLIDE_AUTO_FRAME     10      //松开后自动的桢数
#define MMIPICVIEW_GLIDE_KEY_FRAME      10      //方向键切换移动的桢数

#define MMIPICVIEW_DOUBLECLICK_TIME     400     //双击的down/up 每次最大时间间隔

#define MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM1  20
#define MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM2  19
#ifdef MAINLCD_SIZE_128X160
#define MMIPICVIEW_TITLE_MARGIN 5
#define MMIPICVIEW_PREVIEW_TITLE_LEFT 0
#define MMIPICVIEW_PREVIEW_TITLE_V_RIGHT 127
#define MMIPICVIEW_PREVIEW_TITLE_H_RIGHT 159
#define MMIPICVIEW_PREVIEW_TITLE_HEIGHT 20
#elif defined MAINLCD_SIZE_176X220
#define MMIPICVIEW_TITLE_MARGIN 5
#define MMIPICVIEW_PREVIEW_TITLE_LEFT 0
#define MMIPICVIEW_PREVIEW_TITLE_V_RIGHT 175
#define MMIPICVIEW_PREVIEW_TITLE_H_RIGHT 219
#define MMIPICVIEW_PREVIEW_TITLE_HEIGHT 24
#else
#define MMIPICVIEW_TITLE_MARGIN 5
#endif
/*
enum 
{
    DoubleClick_NONE,
    DoubleClick_DOWN,
    DoubleClick_CLICK,
    DoubleClick_CLICK_DOWN,
    DoubleClick_CLICK_CLICK,
    DoubleClick_Max
};

#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
enum
{
    MMIPICVIEW_GLIDE_LEFT_ANIM,                //左anim控件
    MMIPICVIEW_GLIDE_MIDDLE_ANIM,           //中anim控件
    MMIPICVIEW_GLIDE_RIGHT_ANIM              //右anim控件
};
#endif
*/
#define MMIPICVIEW_BUTTON_MARGIN_WIDTH      10

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN       	s_mmipicview_is_display_osd = FALSE;
LOCAL BOOLEAN       	s_mmipicview_is_tp_softkey = FALSE;
LOCAL BOOLEAN       	s_mmipicview_is_glide_move = FALSE;
LOCAL uint8         	s_mmipicview_display_timer_id = 0;
//LOCAL MMI_CTRL_ID_T 	s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM] = {0};
//多一个MMIPICVIEW_GLIDE_ASSISTANT_ANIM_CTRL_ID控件，用来后台异步解码原图片
#ifdef PIC_GLIDE_SUPPORT
LOCAL MMI_CTRL_ID_T 	s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM + 1] = {0};
LOCAL BOOLEAN			s_picview_is_exchanged = FALSE;
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL MMI_TP_POINTS_T 	tp_points_old = {0};
#endif
#if 0
LOCAL GUI_BOTH_RECT_T 	s_left_anim_both_rect = {0};
LOCAL GUI_BOTH_RECT_T 	s_mid_anim_both_rect = {0};
LOCAL GUI_BOTH_RECT_T 	s_right_anim_both_rect = {0};
LOCAL uint8         	s_mmipicview_glide_page_num = MMIPICVIEW_GLIDE_PAGE_MAXNUM;
#endif
#if defined(PIC_GLIDE_SUPPORT) || defined(TOUCH_PANEL_SUPPORT)
LOCAL GUI_POINT_T   	s_mmipicview_glide_tp_start_point = {0};
LOCAL GUI_POINT_T   	s_mmipicview_glide_tp_prev_point = {0};
LOCAL int16           	s_mmipicview_glide_tp_x_offset     = 0; //控件滑动的距离(不包括空间图片的移动)
#endif
#ifdef DRM_SUPPORT
LOCAL BOOLEAN       	s_mmipicview_is_consume_rights = FALSE;
LOCAL uint32        	s_mmipicview_consume_rights_index = 0;
#endif
#if defined(PIC_GLIDE_SUPPORT) || defined(TOUCH_PANEL_SUPPORT)
#define INVALID_TP_POINT_X       	0x1FFF
#define INVALID_TP_POINT_Y      	0x1FFF
#endif
#if 0
#define PICVIEW_PREVIEW_ZOOM_IN_BUTTON_LEFT              	160
#define PICVIEW_PREVIEW_ZOOM_IN_BUTTON_RIGHT            	(PICVIEW_PREVIEW_ZOOM_IN_BUTTON_LEFT +36 )
#define PICVIEW_PREVIEW_ZOOM_IN_BUTTON_TOP               	420
#define PICVIEW_PREVIEW_ZOOM_IN_BUTTON_BOTTOM         		(PICVIEW_PREVIEW_ZOOM_IN_BUTTON_TOP + 30)

#define PICVIEW_PREVIEW_ZOOM_OUT_BUTTON_LEFT                240
#define PICVIEW_PREVIEW_ZOOM_OUT_BUTTON_RIGHT              	(PICVIEW_PREVIEW_ZOOM_OUT_BUTTON_LEFT +36 )
#define PICVIEW_PREVIEW_ZOOM_OUT_BUTTON_TOP                 PICVIEW_PREVIEW_ZOOM_IN_BUTTON_TOP//420
#define PICVIEW_PREVIEW_ZOOM_OUT_BUTTON_BOTTOM           	PICVIEW_PREVIEW_ZOOM_IN_BUTTON_BOTTOM//(PICVIEW_PREVIEW_ZOOM_IN_BUTTON_TOP + 30)
#endif
extern uint16       g_download_file_name[ ];
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef PIC_GLIDE_SUPPORT  //norflashcut_pic
/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPreviewWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
#endif
/*****************************************************************************/
//  Description : set display preview osd
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayPreviewOsd(
    BOOLEAN     is_display
);

/*****************************************************************************/
//  Description : set anim preview title and softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewOsd(
    uint32         cur_index,  //in:from 1 start
    uint32         total_num,  //in:
    MMI_CTRL_ID_T  ctrl_id,
    MMI_WIN_ID_T   win_id
);

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewTitleOsd(
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id,
    MMI_CTRL_ID_T     return_ctrl_id,
    MMI_WIN_ID_T      win_id
);
#if defined(PICVIEW_F_VIEW_ZOOM)  //norflashcut_pic
#if 0
/*****************************************************************************/
//  Description : set anim preview zoom button
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewZoomButtonOsd(MMI_WIN_ID_T      win_id);
/*****************************************************************************/
//  Description : ZoomIn button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ZoomInButtonCallBack(void);
/*****************************************************************************/
//  Description : ZoomOut  button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ZoomOutButtonCallBack(void);
#endif
#endif

#if 0
/*****************************************************************************/
//  Description : set anim preview slide play button
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSlidePlayButtonOsd(MMI_WIN_ID_T      win_id);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : Slide Play button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePlayButtonCallBack(void);
#endif
/*****************************************************************************/
//  Description : modify preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPreviewTitle(
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id
);


/*****************************************************************************/
//  Description : display preview osd softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSoftkey(
    MMI_WIN_ID_T   win_id  //in:
);

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetAnimParam(
    BOOLEAN          is_zoom,   //是否需要缩放
    BOOLEAN          is_update,  //是否立即刷新
    uint32           item_index, //from 0
    MMI_CTRL_ID_T    ctrl_id
);

#ifdef BAIDU_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetBaiduAnimParam(
     FILEARRAY_DATA_T       * file_info ,
    uint32           item_index, //from 0
    MMI_CTRL_ID_T    ctrl_id
);

/*****************************************************************************/
//  Description : handle BaiduDrv msg
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PIC_PREVIEW_HandleBaiduDrv_msg(MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param);

#endif
/*****************************************************************************/
//  Description : anim owner draw call back function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AnimOwnerDrawCallback(
    GUIANIM_OWNER_DRAW_T   *owner_draw_ptr
);

/*****************************************************************************/
//  Description : handle preview timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewTimer(
    uint8             timer_id,       //in:
    MMI_WIN_ID_T      win_id,         //in:
    MMI_CTRL_ID_T     ctrl_id,        //in:
    MMI_CTRL_ID_T     setlist_ctrl_id //in:
);

/*****************************************************************************/
//  Description : handle display preview osd
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleDisplayPreviewOsd(//is display osd
    MMI_WIN_ID_T     win_id,          //in:
    MMI_CTRL_ID_T    setlist_ctrl_id  //in:
);

/*****************************************************************************/
//  Description : start display title and softkey timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayTimer(
    MMI_WIN_ID_T   win_id
);

/*****************************************************************************/
//  Description : stop display title and softkey timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayTimer(void);

/*****************************************************************************/
//  Description : switch the previous picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PreviousPic(//is change
    BOOLEAN           *anim_result_ptr,//in/out:
    uint32            cur_index,      //in:from 0
    uint32            *prev_index_ptr,//in/out:may PNULL
    uint32            total_num ,     //in
    MMI_CTRL_ID_T     ctrl_id,        //in
    MMI_CTRL_ID_T     list_ctrl_id    //in
);

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NextPic(//is change
    BOOLEAN           *anim_result_ptr,   //in/out:
    uint32            cur_index,          //in:from 0
    uint32            *next_index_ptr,    //in/out:may PNULL
    uint32            total_num,          //in
    MMI_CTRL_ID_T     ctrl_id,            //in
    MMI_CTRL_ID_T     list_ctrl_id        //in
);

/*****************************************************************************/
//  Description : handle preview tp switch
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePreviewTpSwitch(
    BOOLEAN        is_next,        //in:
    uint32         cur_index,      //in:from 0 start
    uint32         total_num,      //in:
    MMI_CTRL_ID_T  ctrl_id,        //in:
    MMI_CTRL_ID_T  list_ctrl_id,   //in:
    MMI_CTRL_ID_T  setlist_ctrl_id,//in:
    MMI_WIN_ID_T   win_id          //in:
);
#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
//*****************************************************************************/
//  Description : handle preview glide win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicGlideWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : init anim param for glide
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void InitGlideAnimParam(
    uint32        item_index, //from 0
    uint32        total_num,  //total num
    MMI_WIN_ID_T  win_id      //win id
);

/*****************************************************************************/
//  Description : init glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideCtrlId(void);

/*****************************************************************************/
//  Description : Set glide anim control info
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetGlideAnimCtrlInfo(uint16  cur_index,BOOLEAN is_prev,BOOLEAN is_next);

/*****************************************************************************/
//  Description : get left glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetLeftAnimCtrlId(void);

/*****************************************************************************/
//  Description : get Middle glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetMiddleAnimCtrlId(void);

/*****************************************************************************/
//  Description : get Assistant glide anim control id
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetAssistantAnimCtrlId(void);

/*****************************************************************************/
//  Description : Exchange Assistant glide anim control with mid anim ctrl
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ExchangeAssisoMidAnimCtrl(void);

/*****************************************************************************/
//  Description : get right glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetRightAnimCtrlId(void);

/*****************************************************************************/
//  Description : set left glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimRect(
    BOOLEAN  is_update
);

/*****************************************************************************/
//  Description : set middle glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimRect(
    BOOLEAN   is_update
);

/*****************************************************************************/
//  Description : set right glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimRect(
    BOOLEAN     is_update
);
/*****************************************************************************/
//  Description : set left anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimParam(
    uint32      item_index, //in:from 0,middle item index
    uint32      total_num   //in:total num
);

/*****************************************************************************/
//  Description : set middle anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimParam(
    uint32           item_index, //in:from 0,middle item index
    MMI_WIN_ID_T     win_id      //in:
);

/*****************************************************************************/
//  Description : set right anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimParam(
    uint32     item_index, //in:from 0,middle item index
    uint32     total_num   //in:total num
);

/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpDown(
    DPARAM     param   //in:
);

/*****************************************************************************/
//  Description : handle picture glide tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpMove(
    DPARAM     param   //in:
);

/*****************************************************************************/
//  Description : sest control rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRectDisplayRect(
    int16             move_x, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
);

/*****************************************************************************/
//  Description : handle picture glide tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpUp(
    uint32           item_index,     //in:from 0
    uint32           total_num,      //in:total num
    MMI_WIN_ID_T     win_id,         //in:
    MMI_CTRL_ID_T    setlist_ctrl_id,//in:
    MMI_CTRL_ID_T    list_ctrl_id,   //in:
    DPARAM           param           //in:
);

/*****************************************************************************/
//  Description : switch picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideSwitchKey(
    BOOLEAN         is_prev,
    uint32          item_index,     //in:from 0
    uint32          total_num,      //in:total num
    MMI_WIN_ID_T    win_id,         //in:
    MMI_CTRL_ID_T   setlist_ctrl_id,//in:
    MMI_CTRL_ID_T   list_ctrl_id    //in:
);
#else
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture picview tp move msg
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void HandlePicviewTpMove(
    DPARAM     param   //in:
);

/*****************************************************************************/
//  Description : handle picture picview tp down msg
//  Global resource dependence :
//  Author: yaye
//  Note:
/*****************************************************************************/
LOCAL void HandlePicviewTpDown(
    DPARAM     param   //in:
);
#endif
#endif
#if defined(PICVIEW_F_VIEW_ZOOM)  //norflashcut_pic
/*****************************************************************************/
//  Description : handle zoom in picture
//  Global resource dependence : none
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void HandleZoomInPic(
    uint32 item_index     //in:from 0
);
/*****************************************************************************/
//  Description : handle zoom out picture
//  Global resource dependence : none
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void HandleZoomOutPic(
    uint32 item_index     //in:from 0
);
/*****************************************************************************/
//  Description : set zoom ratio
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void SetZoomRatio(
    MMI_HANDLE_T        ctrl_handle,  //in:
    uint16 num1,  //in
    uint16 num2   //in
);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
#ifdef PIC_GLIDE_SUPPORT 
/*****************************************************************************/
//  Description : init the info of anim for decoding progress
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressAnimCtrlInfo(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : set the rect of anim for decoding progress
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressAnimDispRect(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_update);
#endif
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewButtonsOSD(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : set pre button param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewButtonParam(
    MMI_WIN_ID_T   win_id  //in:
);
#endif

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewNoteRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Set Label Display By Text Id
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetLabelDisplayByTextId(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id);
#if defined (TOUCH_GESTURE_ENABLE) && defined(PICVIEW_F_VIEW_ZOOM)
/*****************************************************************************/
//  Description : gesture zoom
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL void PicviewGestureZoom(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint32 item_index, MMI_TP_POINTS_T* tp_points, MMI_MULTI_KEY_TP_MSG_PARAM_T* param);
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifndef PIC_GLIDE_SUPPORT  //norflashcut_pic
//picture full screen preview
#ifdef BAIDU_DRIVE_SUPPORT
WINDOW_TABLE(MMIPICVIEW_PREVIEW_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicPreviewWinMsg),
    WIN_ID(MMIPICVIEW_PREVIEW_WIN_ID),
    //WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG | WS_NEED_DBLCLK),
	WIN_STYLE(WS_DISABLE_COMMON_BG | WS_NEED_DBLCLK),
	WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIPICVIEW_PREVIEW_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
//    CREATE_BUTTON_CTRL(res_gallery_ic_delete, MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID),    
//    CREATE_BUTTON_CTRL(res_gallery_ic_back , MMIPICVIEW_RETURN_BUTTON_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE(MMIPICVIEW_PREVIEW_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicPreviewWinMsg),
    WIN_ID(MMIPICVIEW_PREVIEW_WIN_ID),
    WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG | WS_NEED_DBLCLK),
	WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIPICVIEW_PREVIEW_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
    CREATE_SETLIST_CTRL(MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPICVIEW_PREVIEW_LABEL_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};
#endif
#else
WINDOW_TABLE(MMIPICVIEW_GLIDE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicGlideWinMsg),
    WIN_ID(MMIPICVIEW_PREVIEW_WIN_ID),
    WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG | WS_NEED_DBLCLK),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIPICVIEW_GLIDE1_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
    CREATE_ANIM_CTRL(MMIPICVIEW_GLIDE2_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
    CREATE_ANIM_CTRL(MMIPICVIEW_GLIDE3_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
    CREATE_ANIM_CTRL(MMIPICVIEW_GLIDE4_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPICVIEW_PREVIEW_LABEL_CTRL_ID),
#ifdef PIC_PREVIEW_U200_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_RETURN, MMIPICVIEW_RETURN_BUTTON_CTRL_ID),
#endif
    CREATE_SETLIST_CTRL(MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID),
    CREATE_ANIM_CTRL(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, MMIPICVIEW_PREVIEW_WIN_ID),
#ifdef PIC_PREVIEW_U200_SUPPORT
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_OPTION_BUTTON_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef PIC_GLIDE_SUPPORT  //norflashcut_pic


#ifdef BAIDU_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : handle BaiduDrv msg
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PIC_PREVIEW_HandleBaiduDrv_msg(MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param)
{
	MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PREVIEW_ANIM_CTRL_ID;
	GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;   
	GUIANIM_CTRL_INFO_T     control_info = {0};
	GUIANIM_FILE_INFO_T     anim_info = {0};
	GUIANIM_CALL_BACK_T     call_back = {0};
	GUIANIM_DISPLAY_INFO_T  display_info = {0};
	FILEARRAY_DATA_T fileinfo = {0};	
#ifdef TOUCHPANEL_TYPE_NONE
	BOOLEAN b_zoom = FALSE;
	BOOLEAN b_update = FALSE;
#else
	BOOLEAN b_zoom = TRUE;
	BOOLEAN b_update = FALSE;
#endif	

	SCI_TRACE_LOW("PIC_PREVIEW_HandleBaiduDrv_msg: Enter , msg_id = 0x%x" , msg_id);	

	switch (msg_id)
	{
	case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF:
		{
			BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;	
			SCI_TRACE_LOW("MMIPICVIEW HandlePicPreviewWinMsg receive MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF done=%d",data->baidu_drv_msg_data.bDone);
			if(data->baidu_drv_msg_data.bDone)
			{				
				if(MMIAPIFMM_IsFileExist(data->baidu_drv_msg_data.filename , data->baidu_drv_msg_data.file_len))
				{    
					SCI_TRACE_LOW("MMIPICVIEW HandlePicPreviewWinMsg receive MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF MMIAPIFMM_IsFileExist !!!");
				
					//set anim param
					control_info.is_ctrl_id = TRUE;
					control_info.ctrl_id    = ctrl_id;
					
#ifdef PICVIEW_F_VIEW_ZOOM
					display_info.is_zoom        = b_zoom;
#else
					display_info.is_zoom        = FALSE;
#endif
					if (b_zoom)
					{
						display_info.is_syn_decode  = FALSE;
						display_info.is_auto_zoom_in = TRUE;
					}
					
#ifdef PIC_GLIDE_SUPPORT
					if(ctrl_id == GetAssistantAnimCtrlId())
#else
						if(MMIPICVIEW_PREVIEW_ANIM_CTRL_ID == ctrl_id)
#endif
						{
							display_info.is_disp_one_frame = FALSE;
						}
						else
						{
							display_info.is_disp_one_frame = TRUE;
						}
						display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
						display_info.is_update      = b_update;
						display_info.bg.bg_type     = GUI_BG_COLOR;
						
						//背景色设为黑色
						display_info.bg.color       = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;												
#ifdef  VIDEOTHUMBNAIL_SUPPORT
						fileinfo.name_len = data->baidu_drv_msg_data.file_len ; 
						MMIAPICOM_Wstrncpy(fileinfo.filename , data->baidu_drv_msg_data.filename , fileinfo.name_len);
						if(MMIPICVIEW_IsVideoSuffix(&fileinfo)){
							MMIAPIVP_PlayVideoFromVideoPlayer (VP_ENTRY_FROM_FILE, fileinfo.filename, fileinfo.name_len, PNULL);
						}
						else
#endif
						{
							anim_info.full_path_wstr_ptr = data->baidu_drv_msg_data.filename ; 
							anim_info.full_path_wstr_len = data->baidu_drv_msg_data.file_len ;
							anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
						}
						
				}
			}
			break;
		}
	}		
}
#endif

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPreviewWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    BOOLEAN             anim_result = FALSE;
    uint32              item_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PREVIEW_ANIM_CTRL_ID;
    MMI_CTRL_ID_T       setlist_ctrl_id = MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetFullScreenBothRect();
    MMI_TP_POINTS_T 	tp_points = {0};
    FILEARRAY_DATA_T        file_info = {0};
	GUI_RECT_T             animRect    = DP2PX_RECT(36,36,203,203);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef DRM_SUPPORT
        s_mmipicview_is_consume_rights = FALSE;
        s_mmipicview_consume_rights_index = 0;
#endif

        //GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
		GUIAPICTRL_SetRect(ctrl_id, &animRect);

        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);

        //set display preview osd
        SetDisplayPreviewOsd(TRUE);

        //set title and softkey
        SetPreviewOsd((item_index + 1), pic_num, setlist_ctrl_id, win_id);

#ifdef BAIDU_DRIVE_SUPPORT
	MMIPICVIEW_GetPicFileInfo(item_index,&file_info);
       MMISRV_BAIDUDRV_Enable_Download();
	if(MMISRV_BAIDUDRV_is_in_BaiduFolder (PICVIEW_SOURCE_CLOUD_THUMBNAIL, file_info.filename , file_info.name_len))
	{
		SetBaiduAnimParam(&file_info , item_index, ctrl_id);
	}
	else
	{
#ifdef TOUCHPANEL_TYPE_NONE
		//set anim param
		SetAnimParam(FALSE, FALSE,item_index, ctrl_id);
#else
		SetAnimParam(TRUE, FALSE, item_index, ctrl_id);
#endif
	}

#else	
#ifdef TOUCHPANEL_TYPE_NONE
        //set anim param
	SetAnimParam(FALSE, FALSE,item_index, ctrl_id);
#else
	SetAnimParam(TRUE, FALSE, item_index, ctrl_id);
#endif
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifndef PIC_GLIDE_SUPPORT
	GUIAPICTRL_SetState(setlist_ctrl_id,GUISETLIST_STATE_OPEN_POPWIN,FALSE);
#endif
        break;

    case MSG_GET_FOCUS:
        //set display preview osd
        SetDisplayPreviewOsd(TRUE);
        break;

    case MSG_LOSE_FOCUS:
        StopDisplayTimer();
        break;

    case MSG_TIMER:
        result = HandlePreviewTimer(*(uint8*)param, win_id, ctrl_id, setlist_ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_APP_MENU:
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_OPT_WIN_TAB, PNULL);
        }
        break;

    case MSG_APP_WEB:
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            MMIPICVIEW_SliePlay(list_ctrl_id);
//             MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_UP:
    case MSG_APP_LEFT:
        //previous picture
        if (PreviousPic(&anim_result, item_index, PNULL, pic_num, ctrl_id, list_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            //set not display preview osd
            SetDisplayPreviewOsd(FALSE);

            if ((!anim_result) &&
                    (MMK_IsFocusWin(win_id)))
            {
                //not display osd
                MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
            }
        }
        break;

    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        //next picture
        if (NextPic(&anim_result, item_index, PNULL, pic_num, ctrl_id, list_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            //set not display preview osd
            SetDisplayPreviewOsd(FALSE);

            if ((!anim_result) &&
                    (MMK_IsFocusWin(win_id)))
            {
                //not display osd
                MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
            }
        }
        break;
#ifdef BAIDU_DRIVE_SUPPORT
	case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF:
		SCI_TRACE_LOW("[MMIPICVIEW] MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF");
		PIC_PREVIEW_HandleBaiduDrv_msg(win_id, msg_id,   param);
		break;
#endif
    case MSG_APP_CANCEL:
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_SETLIST_SWITCH:
        HandlePreviewTpSwitch(*(BOOLEAN*)param,
                              item_index,
                              pic_num,
                              ctrl_id,
                              list_ctrl_id,
                              setlist_ctrl_id,
                              win_id);
        break;

    case MSG_FULL_PAINT:	
#ifdef BAIDU_DRIVE_SUPPORT		
        if(MMK_IsFocusWin(win_id))
        {
		GUI_RECT_T          rect   = MMITHEME_GetFullScreenRect();
		GUI_LCD_DEV_INFO    lcd_dev_info    = {0};
//		GUI_RECT_T  leftBtnRect = MMI_PIC_BTN_TWO_LEFT_RECT;
//		GUI_RECT_T  rightBtnRect = MMI_PIC_BTN_TWO_RIGHT_RECT;

		GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
//		GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID, &leftBtnRect);
//		GUIBUTTON_SetRect(MMIPICVIEW_RETURN_BUTTON_CTRL_ID, &rightBtnRect);
//		GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID,TRUE,TRUE);
//		GUIBUTTON_SetVisible(MMIPICVIEW_RETURN_BUTTON_CTRL_ID,TRUE,TRUE);
        }
#endif		
    break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined (ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#ifdef BAIDU_DRIVE_SUPPORT
    case MSG_CTL_PENOK:
        {
            if(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
				MMI_STRING_T            queryTipStr = { 0 };
				WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };


				MMI_GetLabelTextByLang( TXT_DELETE, &queryTipStr );
				WatchCOM_QueryWin_2Btn_Enter(
					MMIPICVIEW_QUERY_WIN_ID,
					&queryTipStr,
					PNULL,
					PNULL,
					IMAGE_SET_COM_BTN_FIXED,
					IMAGE_SET_COM_BTN_CANCEL,
					softkey,
					HandlePicPreviewDelQueryWin
					);

            }
            else if(MMIPICVIEW_RETURN_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
			MMK_CloseWin(win_id);
            }
        }
        break;
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_LONG:
        //stop timer
        HandlePicviewTpDown(param);
        break;
#endif		
#endif		
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        //set display preview osd
	    if (!s_mmipicview_is_glide_move)
	    {
	        SetDisplayPreviewOsd(FALSE);
	    }
        HandleDisplayPreviewOsd(win_id, setlist_ctrl_id);
		if(tp_points_old.num != 0)
        {
            MMITHEME_UpdateRect();//手势消息后，防止最后一帧刷屏问题
        }
        SCI_MEMSET(&tp_points_old, 0, sizeof(MMI_TP_POINTS_T));
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_UPDATE:
        GUIANIM_UpdateRect(*(GUI_RECT_T *)param, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_GESTURE:		
#if defined(TOUCH_GESTURE_ENABLE)
#if defined(PICVIEW_F_VIEW_ZOOM)
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicPreviewWinMsg MSG_TP_GESTURE param=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_746_112_2_18_2_46_30_30,(uint8*)"d", MMK_GET_TP_GESTURE(param));
		MMK_GetTpPoints(&tp_points);
		PicviewGestureZoom(win_id, ctrl_id, item_index, &tp_points, (MMI_MULTI_KEY_TP_MSG_PARAM_T *)param);
#endif
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOUBLE:
#ifdef PICVIEW_F_VIEW_ZOOM
        if (GUIANIM_TYPE_IMAGE_GIF != GUIANIM_GetAnimType(ctrl_id))
        {
			GUIANIM_ZoomUtmost(ctrl_id);
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		HandlePicviewTpMove(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        //stop timer
        StopDisplayTimer();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : set display preview osd
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayPreviewOsd(
    BOOLEAN     is_display
)
{
    s_mmipicview_is_display_osd = is_display;
}
#if 0//def PIC_GLIDE_SUPPORT  //norflashcut_pic
LOCAL MMI_RESULT_E DoubleClickCheck(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param);
#endif
#if defined(PICVIEW_F_VIEW_ZOOM)  //norflashcut_pic
#if 0
/*****************************************************************************/
//  Description : ZoomIn button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ZoomInButtonCallBack(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    HandleZoomInPic(MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID));
    return result;
}
/*****************************************************************************/
//  Description : ZoomOut  button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ZoomOutButtonCallBack(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    HandleZoomOutPic(MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID));
    return result;
}
#endif
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : Slide Play button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePlayButtonCallBack(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMIPICVIEW_ReleaseAnimMemory();
    MMIPICVIEW_SliePlay(MMIPICVIEW_LIST_CTRL_ID);
    //close preview win    
//     MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);    
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewOsd(
    uint32         cur_index,  //in:from 1 start
    uint32         total_num,  //in:
    MMI_CTRL_ID_T  ctrl_id,
    MMI_WIN_ID_T   win_id
)
{
#ifndef PIC_PREVIEW_U200_SUPPORT
    SetPreviewTitleOsd(cur_index, total_num, ctrl_id, PNULL, win_id);
    SetPreviewSoftkey(win_id);
#else
    //set title
    SetPreviewTitleOsd(cur_index, total_num, ctrl_id, MMIPICVIEW_RETURN_BUTTON_CTRL_ID,win_id);
    SetPreviewButtonsOSD(win_id);
#endif
    SetPreviewNoteRect(win_id);
}

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewTitleOsd(
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id,
    MMI_CTRL_ID_T     return_ctrl_id,
    MMI_WIN_ID_T      win_id
)
{
    uint8                   str[MMIPICVIEW_TITLE_STR_MAX_LEN] = {0};
    wchar                   wstr[MMIPICVIEW_TITLE_STR_MAX_LEN] = {0};
    uint16                  str_len = 0;
    uint16                  title_height = 0;
    uint16                  char_width = 0;
    int16                   min_width = 0;
#ifdef  PIC_PREVIEW_U200_SUPPORT
    uint16                  img_width = 0;
    BOOLEAN                 result = FALSE;
#endif
    uint16                  img_height = 0;
    GUI_BG_T                bg = {0};
    GUI_BORDER_T            border = {0};
    GUI_FONT_ALL_T          font = {0};
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T         title_rect = both_rect;
    GUISETLIST_ITEM_INFO_T  item_info = {0};
    wchar                   temp_wstr[2] = {'8', 0};
    MMI_STRING_T            temp_string = {0};
    GUISTR_STYLE_T          string_style = {0};
    GUISTR_STATE_T          string_state = 0;

    title_height = GUIFONT_GetHeight(MMIPICPREVIEW_TITLE_FONT);
#ifdef  PIC_PREVIEW_U200_SUPPORT	
    result = GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICVIEW_RETURN, win_id);
#endif
    title_height = MAX(title_height, img_height);
#ifdef PIC_PREVIEW_U200_SUPPORT 
#ifdef PIC_GLIDE_SUPPORT
    GUIRES_GetImgWidthHeight(PNULL, &img_height, IMAGE_DECODE_ON, win_id);
#endif
#endif
    title_rect.v_rect.top   	+= img_height;
    title_rect.h_rect.top       += img_height;
#ifndef MMI_GUI_STYLE_MINISCREEN
    title_rect.v_rect.bottom = title_rect.v_rect.top + title_height + 15 - 1;
    title_rect.h_rect.bottom = title_rect.h_rect.top + title_height + 10 - 1;
#else
    title_rect.v_rect.bottom = MMIPICVIEW_PREVIEW_TITLE_HEIGHT;
    title_rect.h_rect.bottom = MMIPICVIEW_PREVIEW_TITLE_HEIGHT;
#endif
    if(PNULL != ctrl_id)
    {
        // FIXBUG: NEWMS00108255
        //  设置给控件的宽度，不足以显示整个字串，这里再计算下文本的长度
        temp_string.wstr_ptr = temp_wstr;
        temp_string.wstr_len = 1;

        string_style.font = MMIPICPREVIEW_TITLE_FONT;
        string_style.effect = FONT_EFFECT_REVERSE_EDGE;

        string_state = GUISTR_STATE_EFFECT | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;

        char_width = GUISTR_GetStringWidth(&string_style, &temp_string, string_state);

        // 这里假设setlist的显示内容为:最多11个文本字符 uint16,5+"/"+5
        min_width = (int16)(char_width * 11);
#ifndef MMI_GUI_STYLE_MINISCREEN
        title_rect.v_rect.left   = MMIPICVIEW_TITLE_MARGIN;
        title_rect.h_rect.left   = MMIPICVIEW_TITLE_MARGIN;
        title_rect.v_rect.right  = title_rect.v_rect.left + min_width - 1;
        title_rect.h_rect.right  = title_rect.h_rect.left + min_width - 1;
#else
        title_rect.v_rect.left   = MMIPICVIEW_PREVIEW_TITLE_LEFT;
        title_rect.h_rect.left   = MMIPICVIEW_PREVIEW_TITLE_LEFT;
        title_rect.v_rect.right  = MMIPICVIEW_PREVIEW_TITLE_V_RIGHT;
        title_rect.h_rect.right  = MMIPICVIEW_PREVIEW_TITLE_H_RIGHT;
#endif
        //set setting list rect
        GUIAPICTRL_SetBothRect(ctrl_id, &title_rect);

        //set align
        GUISETLIST_SetFontAlign(ctrl_id, ALIGN_LVMIDDLE);

        GUISETLIST_SetStrEffect(ctrl_id, FONT_EFFECT_REVERSE_EDGE);

        //set string
        sprintf((char *)str, "%ld/%ld", cur_index, total_num); /*lint !e64*/
        str_len = (uint16)strlen((char *)str);
        MMI_STRNTOWSTR(wstr, MMIPICVIEW_TITLE_STR_MAX_LEN,
                       str, MMIPICVIEW_TITLE_STR_MAX_LEN,
                       str_len);

        //set title
        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
        item_info.str_info.wstr_ptr = wstr;
        item_info.str_info.wstr_len = str_len;
        GUISETLIST_AddItem(ctrl_id, &item_info);

        //set font
        font.color = MMI_DEFAULT_TEXT_COLOR;
        font.font  = MMIPICPREVIEW_TITLE_FONT;
        GUISETLIST_SetFont(ctrl_id, &font);
#ifndef MMI_GUI_STYLE_MINISCREEN
        //no bg
        bg.bg_type = GUI_BG_NONE;
        GUISETLIST_SetBackGround(ctrl_id, &bg);
#else 
        // 128X160 & 176X220 Title BG
        bg.bg_type = GUI_BG_IMG;
        bg.img_id = IMAGE_PICVIEW_TITLE_OSD_BG;
        GUISETLIST_SetBackGround(ctrl_id, &bg);
#endif

        //no border
        border.type = GUI_BORDER_NONE;
        GUISETLIST_SetBorder(ctrl_id, &border);

        //circular handle left/right
        GUISETLIST_SetCircular(ctrl_id, TRUE);
        GUISETLIST_SetNeedArrow(ctrl_id, FALSE);
         //not active,not tp
        GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE); 
    }
#ifndef MMI_GUI_STYLE_MINISCREEN
#ifdef  PIC_PREVIEW_U200_SUPPORT
    if(PNULL != return_ctrl_id)
    {
        title_rect.v_rect.right     = both_rect.v_rect.right - MMIPICVIEW_TITLE_MARGIN + 1;
        title_rect.h_rect.right     = both_rect.h_rect.right - MMIPICVIEW_TITLE_MARGIN + 1;
        
        //return
        title_rect.v_rect.left      = title_rect.v_rect.right - img_width + 1;
        title_rect.h_rect.left      = title_rect.h_rect.right - img_width + 1;
        GUIAPICTRL_SetBothRect(return_ctrl_id, &title_rect);
    }
#endif
#endif
}

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewButtonsOSD(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T           form_id     = MMIPICVIEW_PREVIEW_FORM_CTRL_ID;
    GUI_BOTH_RECT_T         form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BG_T                bg_info     = {0};
    GUI_BG_T                fg_info     = {0};
    GUI_BOTH_RECT_T         button_rect = {0};
    uint16                  button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16                  button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
    uint16                  button_height = MMITHEME_GetSoftkeyHeight();
    uint16                  button_num  = 3;
    MMI_CTRL_ID_T           ctrl_id[]   = {MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID, MMIPICVIEW_OPTION_BUTTON_CTRL_ID};
    MMI_IMAGE_ID_T          img_id[]    = {IMAGE_PICVIEW_PLAY_DEFAULT, IMAGE_PICVIEW_DELETE, IMAGE_PICVIEW_OPTION};
    uint16                  i;
    
    form_rect.v_rect.top = form_rect.v_rect.bottom - button_height + 1;
    form_rect.h_rect.top = form_rect.h_rect.bottom - button_height + 1;
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = IMAGE_PICVIEW_OSD_BG;
    
    button_width_v    = button_width_v / button_num;
    button_width_h    = button_width_h / button_num;
    button_rect       = form_rect;

    fg_info.bg_type = GUI_BG_IMG;
    for(i = 0; i < button_num; i++)
    {
        button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
        button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
        button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
        button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;
        if (i == (button_num - 1))
        {
            button_rect.v_rect.right = MAX(button_rect.v_rect.right, form_rect.v_rect.right);
            button_rect.h_rect.right = MAX(button_rect.h_rect.right, form_rect.h_rect.right);
        }
        GUIAPICTRL_SetBothRect(ctrl_id[i], &button_rect);
    
        fg_info.img_id = img_id[i];
        GUIBUTTON_SetBg(ctrl_id[i], &bg_info);        
        GUIBUTTON_SetFg(ctrl_id[i], &fg_info);
        GUIBUTTON_PermitBorder(ctrl_id[i], FALSE);
    }
    
    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, SlidePlayButtonCallBack);
}

/*****************************************************************************/
//  Description : set pre button param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewButtonParam(
    MMI_WIN_ID_T   win_id  //in:
)
{
    GUI_RECT_T option_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T return_rect = MMITHEME_GetFullScreenRect();
    uint16 image_height = 0;
    uint16 image_width = 0;

    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_COMMON_BTN_RETURN, win_id);

    option_rect.left += MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    option_rect.right = option_rect.left + image_width;
    //option_rect.top = option_rect.top + MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    option_rect.bottom = option_rect.top + image_height;

    GUIBUTTON_SetRect(MMIPICVIEW_RETURN_BUTTON_CTRL_ID, &option_rect);

    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_COMMON_BTN_OPTION, win_id);

    return_rect.right = return_rect.right - MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    return_rect.left = return_rect.right - image_width;
    //return_rect.top = return_rect.top + MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    return_rect.bottom = return_rect.top + image_height;

    GUIBUTTON_SetRect(MMIPICVIEW_OPTION_BUTTON_CTRL_ID, &return_rect);
}
#endif

#if defined(PICVIEW_F_VIEW_ZOOM)  //norflashcut_pic
#if 0
/*****************************************************************************/
//  Description : set anim preview zoom button
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewZoomButtonOsd(MMI_WIN_ID_T      win_id)
{
    uint16                             	lcd_width       	= 0;
    uint16                             	lcd_height          = 0;
    uint16                             	img_width           = 0;
    uint16                             	img_height          = 0;
    GUI_BOTH_RECT_T         			both_rect           = {0};
    GUI_BG_T                			bg_ptr              = {0};
    GUI_BG_T                      button_bg_none = {0};
    GUI_BOTH_RECT_T                     note_rect           = {0};

    //get button height
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICVIEW_ZOOM_IN, win_id);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if (!MMITHEME_IsMainScreenLandscape())
    {
        //for zoom in button
        both_rect.v_rect.left   = (int16)(lcd_width >> 1);
        both_rect.v_rect.right  = (int16)(lcd_width - (lcd_width >> 2) - 1);
        both_rect.v_rect.top  = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.v_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.v_rect.top + img_height - 1);


        both_rect.h_rect.left   = (int16)(lcd_height >> 1);
        both_rect.h_rect.right  = (int16)(lcd_height - (lcd_height >> 2) - 1);
        both_rect.h_rect.top  = lcd_width - (uint16)MAX(img_height, (lcd_width >> 3));
        both_rect.h_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.h_rect.top + img_height - 1);
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &both_rect.v_rect);
    }
    else
    {
        //for zoom in button
        both_rect.h_rect.left   = (int16)(lcd_width >> 1);
        both_rect.h_rect.right  = (int16)(lcd_width - (lcd_width >> 2) - 1);
        both_rect.h_rect.top  = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.h_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.h_rect.top + img_height - 1);


        both_rect.v_rect.left   = (int16)(lcd_height >> 1);
        both_rect.v_rect.right  = (int16)(lcd_height - (lcd_height >> 2) - 1);
        both_rect.v_rect.top  = lcd_width - (uint16)MAX(img_height, (lcd_width >> 3));
        both_rect.v_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.v_rect.top + img_height - 1);
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &both_rect.h_rect);
    }

    GUICTRL_SetBothRect((IGUICTRL_T *)  MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID), &both_rect);

    button_bg_none.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &button_bg_none);
    GUIBUTTON_SetPressedBg(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &button_bg_none);

    bg_ptr.bg_type  =  GUI_BG_IMG;
    bg_ptr.img_id    = IMAGE_PICVIEW_ZOOM_IN_GRAY;
    bg_ptr.shape     = GUI_SHAPE_RECT;
    GUIBUTTON_SetGrayed(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, FALSE, &bg_ptr, FALSE);
    GUIBUTTON_SetPressedFg(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &bg_ptr);
    bg_ptr.img_id    = IMAGE_PICVIEW_ZOOM_IN;
    GUIBUTTON_SetFg(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, &bg_ptr);

    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, ZoomInButtonCallBack);
    GUIBUTTON_SetRunSheen(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID, FALSE);


    if (!MMITHEME_IsMainScreenLandscape())
    {
        //for zoom out button
        both_rect.v_rect.left   = (int16)(lcd_width - (lcd_width >> 2));
        both_rect.v_rect.right  = (int16)(lcd_width - 1);
        both_rect.v_rect.top  = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.v_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.v_rect.top + img_height - 1);


        both_rect.h_rect.left   = (int16)(lcd_height - (lcd_height >> 2));
        both_rect.h_rect.right  = (int16)(lcd_height - 1);
        both_rect.h_rect.top  = lcd_width - (uint16)MAX(img_height, lcd_width >> 3);
        both_rect.h_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.h_rect.top + img_height - 1);
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &both_rect.v_rect);
    }
    else
    {
        //for zoom out button
        both_rect.h_rect.left   = (int16)(lcd_width - (lcd_width >> 2));
        both_rect.h_rect.right  = (int16)(lcd_width - 1);
        both_rect.h_rect.top  = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.h_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.h_rect.top + img_height - 1);


        both_rect.v_rect.left   = (int16)(lcd_height - (lcd_height >> 2));
        both_rect.v_rect.right  = (int16)(lcd_height - 1);
        both_rect.v_rect.top  = lcd_width - (uint16)MAX(img_height, lcd_width >> 3);
        both_rect.v_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.v_rect.top + img_height - 1);
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &both_rect.h_rect);
    }

    GUICTRL_SetBothRect((IGUICTRL_T *)  MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID), &both_rect);

    button_bg_none.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &button_bg_none);
    GUIBUTTON_SetPressedBg(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &button_bg_none);

    bg_ptr.bg_type  =  GUI_BG_IMG;
    bg_ptr.img_id    = IMAGE_PICVIEW_ZOOM_OUT_GRAY;
    bg_ptr.shape     = GUI_SHAPE_RECT;
    GUIBUTTON_SetGrayed(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, TRUE, &bg_ptr, FALSE);
    GUIBUTTON_SetPressedFg(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &bg_ptr);
    bg_ptr.img_id    = IMAGE_PICVIEW_ZOOM_OUT;
    GUIBUTTON_SetFg(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, &bg_ptr);

    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, ZoomOutButtonCallBack);
    GUIBUTTON_SetRunSheen(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID, FALSE);

    note_rect               = both_rect;
    note_rect.h_rect.left	= note_rect.v_rect.left  = 0;
    note_rect.h_rect.bottom	= both_rect.h_rect.top - 2;
    note_rect.v_rect.bottom	= both_rect.v_rect.top - 2;
    note_rect.h_rect.top	= note_rect.h_rect.bottom - MMI_DEFAULT_NORMAL_FONT_SIZE - 2;
    note_rect.v_rect.top	= note_rect.v_rect.bottom - MMI_DEFAULT_NORMAL_FONT_SIZE - 2;
    if (!MMITHEME_IsMainScreenLandscape())
    {
        GUILABEL_SetRect(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, &note_rect.v_rect, FALSE);
    }
    else
    {
        GUILABEL_SetRect(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, &note_rect.h_rect, FALSE);
    }
    GUICTRL_SetBothRect((IGUICTRL_T *)  MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID), &note_rect);
    GUILABEL_SetVisible(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, FALSE, FALSE);
}
#endif
#endif
#if 0
/*****************************************************************************/
//  Description : set anim preview slide play button
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSlidePlayButtonOsd(MMI_WIN_ID_T      win_id)
{
    uint16                              lcd_width           = 0;
    uint16                              lcd_height          = 0;
    uint16                              img_width           = 0;
    uint16                              img_height          = 0;
    GUI_BOTH_RECT_T                     both_rect           = {0};

    //get button height
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICVIEW_PLAY, win_id);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if (!MMITHEME_IsMainScreenLandscape())
    {
        both_rect.v_rect.left   = (int16)(((lcd_width>>1) - img_width)>> 1);
        both_rect.v_rect.right  = (int16)(((lcd_width>>1) + img_width)>> 1);
        both_rect.v_rect.top    = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.v_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.v_rect.top + img_height - 1);

        both_rect.h_rect.left   = (int16)(((lcd_height>>1) - img_width)>> 1);
        both_rect.h_rect.right  = (int16)(((lcd_height>>1) + img_width)>> 1);
        both_rect.h_rect.top    = lcd_width - (uint16)MAX(img_height, lcd_width >> 3);
        both_rect.h_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.h_rect.top + img_height - 1);
        
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, &both_rect.v_rect);
    }
    else
    {
        //for zoom in button
        both_rect.h_rect.left   = (int16)(((lcd_width>>1) - img_width)>> 1);
        both_rect.h_rect.right  = (int16)(((lcd_width>>1) + img_width)>> 1);
        both_rect.h_rect.top    = lcd_height - (uint16)MAX(img_height, lcd_height >> 3);
        both_rect.h_rect.bottom = (int16)(lcd_height - 1);//(int16)(both_rect.h_rect.top + img_height - 1);

        both_rect.v_rect.left   = (int16)(((lcd_height>>1) - img_width)>> 1);
        both_rect.v_rect.right  = (int16)(((lcd_height>>1) + img_width)>> 1);
        both_rect.v_rect.top    = lcd_width - (uint16)MAX(img_height, lcd_width >> 3);
        both_rect.v_rect.bottom = (int16)(lcd_width - 1);//(int16)(both_rect.v_rect.top + img_height - 1);
        GUIBUTTON_SetRect(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, &both_rect.h_rect);
    }    
    GUIAPICTRL_SetBothRect(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID,&both_rect);    
    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, SlidePlayButtonCallBack);
    GUIBUTTON_SetRunSheen(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, FALSE);
}
#endif
/*****************************************************************************/
//  Description : modify preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPreviewTitle(
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id
)
{
    BOOLEAN                 is_need_arrow = FALSE;
    uint8                   str[MMIPICVIEW_TITLE_STR_MAX_LEN] = {0};
    wchar                   wstr[MMIPICVIEW_TITLE_STR_MAX_LEN] = {0};
    uint16                  str_len = 0;
    GUISETLIST_ITEM_INFO_T  item_info = {0};

    //set string
    sprintf((char *)str, "%ld/%ld", cur_index, total_num); /*lint !e64*/
    str_len = (uint16)strlen((char *)str);
    MMI_STRNTOWSTR(wstr, MMIPICVIEW_TITLE_STR_MAX_LEN,
                   str, MMIPICVIEW_TITLE_STR_MAX_LEN,
                   str_len);

    //set title
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = wstr;
    item_info.str_info.wstr_len = str_len;

    GUISETLIST_ModifyItem(ctrl_id, &item_info, 0);
    if (1 < total_num)
    {
        is_need_arrow = TRUE;
        GUISETLIST_ModifyItem(ctrl_id, &item_info, 1);
    }
}

/*****************************************************************************/
//  Description : display preview osd softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSoftkey(
    MMI_WIN_ID_T   win_id  //in:
)
{
    GUI_BG_T    common_bg = {0};

    //set common bg type
    common_bg.bg_type = GUI_BG_NONE;
    GUIWIN_SetSoftkeyCommonBg(win_id, &common_bg);
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
  /*    
    //set left image
    GUIWIN_SeSoftkeytButtonIconId(win_id,
                                  IMAGE_PICVIEW_OPTION,
                                  0,
                                  FALSE);

    //set middle softkey
    GUIWIN_SeSoftkeytButtonIconId(win_id,
                                  IMAGE_PICVIEW_PLAY,
                                  1,
                                  FALSE);

    //set right softkey
    GUIWIN_SeSoftkeytButtonIconId(win_id,
                                  IMAGE_PICVIEW_RETURN,
                                  2,
                                  FALSE);
*/
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetAnimParam(
    BOOLEAN          is_zoom,   //是否需要缩放
    BOOLEAN          is_update,  //是否立即刷新
    uint32           item_index, //from 0
    MMI_CTRL_ID_T    ctrl_id
)
{
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    SCI_TRACE_LOW("SetAnimParam:item_index=%d, ctrl_id=%d, is_zoom=%d", item_index, ctrl_id, is_zoom);

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index, &file_info))
    {
#ifdef DRM_SUPPORT
        if ((s_mmipicview_is_consume_rights) &&
                (s_mmipicview_consume_rights_index != item_index))
        {
#ifdef PIC_GLIDE_SUPPORT
            if (ctrl_id == GetAssistantAnimCtrlId())
#endif
            {
                s_mmipicview_is_consume_rights = FALSE;
            }
        }
#endif

        if (!MMITHEME_IsSupportGsensor())
        {
            //set anim auto adapt display
//            GUIANIM_SetAutoAdaptDisplay(TRUE, ctrl_id);
        }

        //set anim min period
        GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD, ctrl_id);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id    = ctrl_id;

        
#ifdef PICVIEW_F_VIEW_ZOOM
        display_info.is_zoom        = is_zoom;
        //if (!is_zoom)
        //{
            //display_info.is_syn_decode = TRUE;
        //}
#else
        display_info.is_zoom        = FALSE;
#endif
        if (is_zoom)
        {
            display_info.is_syn_decode  = FALSE;
            display_info.is_auto_zoom_in = TRUE;
        }

#ifdef PIC_GLIDE_SUPPORT
		if(ctrl_id == GetAssistantAnimCtrlId())
#else
		if(MMIPICVIEW_PREVIEW_ANIM_CTRL_ID == ctrl_id)
#endif
		{
			display_info.is_disp_one_frame = FALSE;
		}
		else
		{
			display_info.is_disp_one_frame = TRUE;
		}
        display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_update      = is_update;
        display_info.bg.bg_type     = GUI_BG_COLOR;

        //背景色设为黑色
        display_info.bg.color       = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;
#ifdef DRM_SUPPORT
		if(!MMIPICVIEW_IsPreview(FALSE, item_index))
		{
			GUIANIM_DATA_INFO_T data_info={0};
			data_info.img_id  = IMAGE_DRM_PIC_LOCK_ICON;
			anim_result = GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
		}
		else
#endif
		{
			
			anim_info.full_path_wstr_ptr = file_info.filename;
			anim_info.full_path_wstr_len = file_info.name_len;
			anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
		}

        //SCI_TRACE_LOW:"SetAnimParam:GUIANIM_SetParam,filename:%s,name_len =%d,result = %d,item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1378_112_2_18_2_46_31_32,(uint8*)"sddd", file_info.filename, file_info.name_len, anim_result, item_index);

        //set call back
        call_back.param         = item_index;
        call_back.OwnerDrawFunc = AnimOwnerDrawCallback;
        GUIANIM_SetOwnerDrawEx(ctrl_id, &call_back);

    }

    return (anim_result);
}

#ifdef BAIDU_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetBaiduAnimParam(
    FILEARRAY_DATA_T       * file_info ,
    uint32           item_index, //from 0
    MMI_CTRL_ID_T    ctrl_id
)
{
	GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;   
	GUIANIM_CTRL_INFO_T     control_info = {0};
	GUIANIM_FILE_INFO_T     anim_info = {0};
	GUIANIM_CALL_BACK_T     call_back = {0};
	GUIANIM_DISPLAY_INFO_T  display_info = {0};
	const wchar* dev_path = PNULL;
	int dev_len = MMIAPIFMM_GetDevicePathLen(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));	
	MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};	
	uint16       w_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0}; 
       uint8       temp_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
	uint16      w_file_name_len=0; 
	FILEARRAY_DATA_T tmp_file_info = {0};
#ifdef TOUCHPANEL_TYPE_NONE
	BOOLEAN b_zoom = FALSE;
	BOOLEAN b_update = FALSE;
#else
	BOOLEAN b_zoom = TRUE;
	BOOLEAN b_update = FALSE;
#endif
	MMIPICVIEW_DEFAULT_PATH_T   download_file_path = {0};
	MMIPICVIEW_DEFAULT_PATH_T   display_file_path = {0};
	wchar           progress_text_wstr[] = L" ";
	MMI_STRING_T process_content_str = {0};
	MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
	BOOLEAN b_file_exist = FALSE;

	SCI_TRACE_LOW("SetBaiduAnimParam: Enter");	

	//check whether the file has been downloaded 
	dev_path = MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));

	if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
	{
		MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, PICVIEW_SOURCE_CLOUD_DOWNLOADED,&default_path);     
	}	

	SCI_MEMCPY((void*)&tmp_file_info , file_info , sizeof(FILEARRAY_DATA_T));

	MMIAPIFMM_SplitLastName(tmp_file_info.filename , &tmp_file_info.name_len ,  w_file_name, &w_file_name_len);  
	
	download_file_path.path_len = MMIFMM_PATHNAME_LEN ; 
	MMIAPIFMM_CombineFullPath(dev_path, dev_len,
	                    MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD, MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD_LEN,
	                    w_file_name, w_file_name_len,
	                    download_file_path.path, &download_file_path.path_len);
    
       MMIAPICOM_WstrToStr(w_file_name, temp_str);
       SCI_TRACE_LOW("w_file_name:%s", temp_str);
	if(!MMIAPIFMM_IsFileExist(download_file_path.path, download_file_path.path_len))
	{
		SCI_TRACE_LOW("SetBaiduAnimParam: file does not exist , start download");	

		SCI_MEMCPY((void*)display_file_path.path , (void*)file_info->filename , sizeof(wchar) * file_info->name_len);
		display_file_path.path_len = file_info->name_len;
        
              SCI_STRCPY(g_download_file_name, temp_str);
		MMISRV_BAIDUDRV_Download_Photo(g_download_file_name , SCI_STRLEN(temp_str));

		GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID,FALSE,FALSE);
		GUIBUTTON_SetVisible(MMIPICVIEW_RETURN_BUTTON_CTRL_ID,FALSE,FALSE);

		// if waiting window is open , then close it first 
		MMK_CloseWin(MMIPICVIEW_WAIT_WIN_ID);
		
		MMIPIC_CreateDownloadProgressDialogWin();
		//return anim_result;
	}
	else
	{
		SCI_TRACE_LOW("SetBaiduAnimParam: file exists !! display directly");
		display_file_path.path_len = download_file_path.path_len;
		MMIAPICOM_Wstrncpy(display_file_path.path, download_file_path.path, download_file_path.path_len);
		b_file_exist = TRUE;
	}
	SCI_TRACE_LOW("[baidu] download ok.");
      
	{
		{
			//set anim param
			control_info.is_ctrl_id = TRUE;
			control_info.ctrl_id    = ctrl_id;
			display_info.is_zoom        = FALSE;
			display_info.is_syn_decode  = FALSE;
			display_info.is_auto_zoom_in = FALSE;
			display_info.is_disp_one_frame = TRUE;
			display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
			display_info.is_update      = TRUE ; //b_update;
			display_info.bg.bg_type     = GUI_BG_COLOR;

			//背景色设为黑色
			display_info.bg.color       = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;
#ifdef DRM_SUPPORT
			if(!MMIPICVIEW_IsPreview(FALSE, item_index)){	
			GUIANIM_DATA_INFO_T data_info={0};
			SCI_TRACE_LOW("[baidu] 1.");
			data_info.img_id  = IMAGE_DRM_PIC_LOCK_ICON;
			anim_result = GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
			}else
#endif
			{
#ifdef  VIDEOTHUMBNAIL_SUPPORT
				if(MMIPICVIEW_IsVideoSuffix(file_info) && b_file_exist){
					SCI_TRACE_LOW("[baidu] 2");
					MMIAPIVP_PlayVideoFromVideoPlayer (VP_ENTRY_FROM_FILE, display_file_path.path, display_file_path.path_len, PNULL);

					anim_info.full_path_wstr_ptr = file_info->filename; 
					anim_info.full_path_wstr_len = file_info->name_len;
					anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);

				}
				else
#endif
				{
					anim_info.full_path_wstr_ptr = display_file_path.path ; 
					anim_info.full_path_wstr_len = display_file_path.path_len;
					anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
				}
			}

			//SCI_TRACE_LOW("SetBaiduAnimParam:GUIANIM_SetParam,filename:%s,name_len =%d,result = %d,item_index = %d" , file_info->filename, file_info->name_len, anim_result, item_index);	

			//set call back
//			call_back.param         = item_index;
//			call_back.OwnerDrawFunc = AnimOwnerDrawCallback;
//			GUIANIM_SetOwnerDrawEx(ctrl_id, &call_back);
		}
	}
	
    return (anim_result);
}

#endif

/*****************************************************************************/
//  Description : anim owner draw call back function
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL void AnimOwnerDrawCallback(
    GUIANIM_OWNER_DRAW_T   *owner_draw_ptr
)
{
    MMI_CTRL_ID_T       setlist_ctrl_id = MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID;

#ifdef DRM_SUPPORT
    uint32              item_index = 0;
    FILEARRAY_DATA_T    file_info = {0};

    if ((!GUI_IsRectEmpty(owner_draw_ptr->display_rect)) 
            &&(!s_mmipicview_is_consume_rights) 
#ifdef PIC_GLIDE_SUPPORT
            &&(owner_draw_ptr->ctrl_handle == MMK_ConvertIdToHandle(GetAssistantAnimCtrlId())
			||owner_draw_ptr->ctrl_handle == MMK_ConvertIdToHandle(GetMiddleAnimCtrlId())) 
#endif
            &&(GUIANIM_IsSupport(owner_draw_ptr->ctrl_handle)))
    {
        //get item index
        item_index = (uint32)owner_draw_ptr->param;

        if (MMIPICVIEW_GetPicFileInfo(item_index, &file_info))
        {
            MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, file_info.filename);

            s_mmipicview_is_consume_rights = TRUE;
            s_mmipicview_consume_rights_index = item_index;
        }
    }
#endif

    if ((s_mmipicview_is_display_osd) &&
            (!GUI_IsRectEmpty(owner_draw_ptr->display_rect)))
    {
        //update title
        GUISETLIST_SetVisible(setlist_ctrl_id, TRUE, TRUE);
#ifndef PIC_PREVIEW_U200_SUPPORT
        //set softkey visible
        GUIWIN_SetSoftkeyVisible(owner_draw_ptr->win_handle, TRUE);
        
        //update softkey
        GUIWIN_UpdateSoftkey(owner_draw_ptr->win_handle);
#else
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_OPTION_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_RETURN_BUTTON_CTRL_ID, TRUE, TRUE);
#endif

        if (MMK_IsFocusWin(owner_draw_ptr->win_handle))
        {
            //start display timer
            StartDisplayTimer(owner_draw_ptr->win_handle);
        }
    }
}

/*****************************************************************************/
//  Description : handle preview timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewTimer(
    uint8             timer_id,       //in:
    MMI_WIN_ID_T      win_id,         //in:
    MMI_CTRL_ID_T     ctrl_id,        //in:
    MMI_CTRL_ID_T     setlist_ctrl_id //in:
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if (s_mmipicview_display_timer_id == timer_id)
    {
        //stop timer
        StopDisplayTimer();

        //set title not visible
        GUISETLIST_SetVisible(setlist_ctrl_id, FALSE, FALSE);
#ifndef PIC_PREVIEW_U200_SUPPORT
        //set softkey not visible
        GUIWIN_SetSoftkeyVisible(win_id, FALSE);

        //set not display preview osd
        SetDisplayPreviewOsd(FALSE);
#else
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(MMIPICVIEW_OPTION_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(MMIPICVIEW_RETURN_BUTTON_CTRL_ID, FALSE, TRUE);
#endif
        //set not display preview osd
        SetDisplayPreviewOsd(FALSE);

        if (MMK_IsFocusWin(win_id))
        {
            //display zoom anim
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle display preview osd
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL BOOLEAN HandleDisplayPreviewOsd(//is display osd
    MMI_WIN_ID_T     win_id,          //in:
    MMI_CTRL_ID_T    setlist_ctrl_id  //in:
)
{
    BOOLEAN             result = FALSE;
    
#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
    MMI_HANDLE_T        ctrl_id = GetMiddleAnimCtrlId();
#else
    MMI_HANDLE_T        ctrl_id = MMIPICVIEW_PREVIEW_ANIM_CTRL_ID;
#endif
    GUIANIM_TYPE_E      anim_type = GUIANIM_TYPE_NONE;
    
    //is display preview osd
    result = s_mmipicview_is_display_osd;
    if (!result)
    {
        //set display preview osd
        SetDisplayPreviewOsd(TRUE);
        
        //update title
        GUISETLIST_SetVisible(setlist_ctrl_id, TRUE, TRUE);
#ifndef PIC_PREVIEW_U200_SUPPORT
        //set softkey visible
        GUIWIN_SetSoftkeyVisible(win_id, TRUE);
        
        //update softkey
        GUIWIN_UpdateSoftkey(win_id);
#else
        GUISETLIST_SetNeedArrow(setlist_ctrl_id, FALSE);
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_OPTION_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(MMIPICVIEW_RETURN_BUTTON_CTRL_ID, TRUE, TRUE);
#endif
//         anim_type = GUIANIM_GetAnimType(ctrl_id);
//         if (GUIANIM_TYPE_IMAGE_GIF == anim_type)
//         {
//             GUIANIM_PauseAnim(ctrl_id);
//         }
        //start display timer
        StartDisplayTimer(win_id);
    }
       
    return (result);
}

/*****************************************************************************/
//  Description : start display title and softkey timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayTimer(
    MMI_WIN_ID_T   win_id
)
{
    if (0 == s_mmipicview_display_timer_id)
    {
        s_mmipicview_display_timer_id = MMK_CreateWinTimer(win_id, MMIPICVIEW_DISPLAY_TIME, FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"StartDisplayTimer:don't creat display timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1571_112_2_18_2_46_31_33,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop display title and softkey timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayTimer(void)
{
    if (0 < s_mmipicview_display_timer_id)
    {
        MMK_StopTimer(s_mmipicview_display_timer_id);
        s_mmipicview_display_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : switch the previous picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PreviousPic(//is change
    BOOLEAN           *anim_result_ptr,//in/out:
    uint32            cur_index,      //in:from 0
    uint32            *prev_index_ptr,//in/out:may PNULL
    uint32            total_num,      //in
    MMI_CTRL_ID_T     ctrl_id,        //in
    MMI_CTRL_ID_T     list_ctrl_id    //in
)
{
    BOOLEAN     result = FALSE;
    uint32      start_index = 0;
    GUIANIM_RESULT_E anim_result = GUIANIM_RESULT_SUCC;

    if (0 == total_num)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW PreviousPic total_num is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1607_112_2_18_2_46_31_34,(uint8*)"");
        return result;
    }

    if (1 < total_num)
    {
        //set start index
        start_index = cur_index;

        //set current index
        if (0 < cur_index)
        {
            cur_index = cur_index - 1;
        }
        else
        {
            cur_index = total_num - 1;
        }

//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE, cur_index))
//        {
//            //set current index
//            if (0 < cur_index)
//            {
//                cur_index = cur_index - 1;
//            }
//            else
//            {
//                cur_index = total_num - 1;
//            }
//
//            if (start_index == cur_index)
//            {
//                return (result);
//            }
//        }
//#endif
        //set list index
        MMIPICVIEW_SetCurIndex(list_ctrl_id, (uint16)cur_index);

        //modify title
        ModifyPreviewTitle((cur_index + 1), total_num, MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID);

        //add pic
        anim_result = SetAnimParam(TRUE, TRUE, cur_index, ctrl_id);
        if ((GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
        {
            *anim_result_ptr = TRUE;
        }
        else
        {
            *anim_result_ptr = FALSE;
        }

        result = TRUE;
    }

    if (PNULL != prev_index_ptr)
    {
        *prev_index_ptr = cur_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NextPic(//is change
    BOOLEAN           *anim_result_ptr,   //in/out:
    uint32            cur_index,          //in:from 0
    uint32            *next_index_ptr,    //in/out:may PNULL
    uint32            total_num,          //in
    MMI_CTRL_ID_T     ctrl_id,            //in
    MMI_CTRL_ID_T     list_ctrl_id        //in
)
{
    BOOLEAN     result = FALSE;
    uint32      start_index = 0;
    GUIANIM_RESULT_E anim_result = GUIANIM_RESULT_SUCC;
        
    if (0 == total_num)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW NextPic total_num is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1684_112_2_18_2_46_32_35,(uint8*)"");
        return result;
    }

    if (1 < total_num)
    {
        //set start index
        start_index = cur_index;

        //set current index
        if (cur_index < (total_num - 1))
        {
            cur_index = cur_index + 1;
        }
        else
        {
            cur_index = 0;
        }

//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE, cur_index))
//        {
//            //set current index
//            if (cur_index < (total_num - 1))
//            {
//                cur_index = cur_index + 1;
//            }
//            else
//            {
//                cur_index = 0;
//            }
//
//            if (start_index == cur_index)
//            {
//                return (result);
//            }
//        }
//#endif
        //set list index
        MMIPICVIEW_SetCurIndex(list_ctrl_id, (uint16)cur_index);

        //modify title
        ModifyPreviewTitle((cur_index + 1), total_num, MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID);

        //add pic
        anim_result = SetAnimParam(TRUE, TRUE, cur_index, ctrl_id);
        if ((GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
        {
            *anim_result_ptr = TRUE;
        }
        else
        {
            *anim_result_ptr = FALSE;
        }
        result = TRUE;
    }

    if (PNULL != next_index_ptr)
    {
        *next_index_ptr = cur_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle preview tp switch
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePreviewTpSwitch(
    BOOLEAN        is_next,        //in:
    uint32         cur_index,      //in:from 0 start
    uint32         total_num,      //in:
    MMI_CTRL_ID_T  ctrl_id,        //in:
    MMI_CTRL_ID_T  list_ctrl_id,   //in:
    MMI_CTRL_ID_T  setlist_ctrl_id,//in:
    MMI_WIN_ID_T   win_id          //in:
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     anim_result = FALSE;
    uint32      pre_next_index = 0;

    if (is_next)
    {
        //next picture
        result = NextPic(&anim_result, cur_index, &pre_next_index, total_num, ctrl_id, list_ctrl_id);
    }
    else
    {
        //previous picture
        result  = PreviousPic(&anim_result, cur_index, &pre_next_index, total_num, ctrl_id, list_ctrl_id);
    }

    if (result)
    {
        //stop timer
        StopDisplayTimer();

        //set not display preview osd
        SetDisplayPreviewOsd(FALSE);

        HandleDisplayPreviewOsd(win_id, setlist_ctrl_id);
    }
}
#if defined(PICVIEW_F_VIEW_ZOOM)  //norflashcut_pic
/*****************************************************************************/
//  Description : handle zoom in picture
//  Global resource dependence : none
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void HandleZoomInPic(
    uint32     item_index  //in:from 0
)

{
    //FILEARRAY_DATA_T    file_info = {0};

    //SCI_TRACE_LOW:"MMIPICVIEW HandleZoomInPic item_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1795_112_2_18_2_46_32_36,(uint8*)"d",item_index);

    //get picture info
    //if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    //{
    //     MMIPICVIEW_Zoom(file_info.filename,file_info.name_len);

    //}
    #ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
    GUIANIM_Zoom(GetMiddleAnimCtrlId(), TRUE, TRUE);
	#else
    GUIANIM_Zoom(MMIPICVIEW_PREVIEW_ANIM_CTRL_ID, TRUE, TRUE);
    #endif
}
/*****************************************************************************/
//  Description : handle zoom out picture
//  Global resource dependence : none
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void HandleZoomOutPic(
    uint32     item_index  //in:from 0
)

{
    //FILEARRAY_DATA_T    file_info = {0};

    //SCI_TRACE_LOW:"MMIPICVIEW HandleZoomOutPic item_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1819_112_2_18_2_46_32_37,(uint8*)"d",item_index);

    //get picture info
    //if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    //{
    //    MMIPICVIEW_Zoom(file_info.filename,file_info.name_len);
    //}
    #ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
    GUIANIM_Zoom(GetMiddleAnimCtrlId(), FALSE, TRUE);
    #endif
}
#endif
#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
/*****************************************************************************/
//  Description : 用于是否可以显示全屏的相关信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDisplaySwitchScreenReferInfoEnable(void)
{   
    BOOLEAN result = FALSE;
    
#if defined(MOTION_SENSOR_TYPE)
    #ifdef MMI_RES_ORIENT_BOTH
        if(!MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_LANDSCAPE_90) 
                && !MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_LANDSCAPE_270)
                && !MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_LANDSCAPE_180))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
    #else
            result = TRUE;
    #endif	
#else
        result = TRUE; 
#endif

    return result;
}

LOCAL void SetPicGlideWinAngle(MMI_GESTURE_TYPE_E  gesturetype)
{
      MMI_WIN_ID_T win_id = MMIPICVIEW_PREVIEW_WIN_ID;
    WINDOW_SUPPORT_ANGLE support_angel = WIN_SUPPORT_ANGLE_ALL;
    LCD_ANGLE_E angle=LCD_ANGLE_0;
    MMK_GetWinAngle(MMIPICVIEW_PREVIEW_WIN_ID,&angle);
    
    switch( angle )
    {
    case LCD_ANGLE_0:
        if(gesturetype == MMI_GESTURE_ROTATE_LEFT)
        {
            angle = LCD_ANGLE_90;
            support_angel = WIN_SUPPORT_ANGLE_90;
        }
        else if(gesturetype == MMI_GESTURE_ROTATE_RIGHT)
        {
             angle = LCD_ANGLE_270;
            support_angel = WIN_SUPPORT_ANGLE_270;
        }
        break;
    case LCD_ANGLE_90:
        if(gesturetype == MMI_GESTURE_ROTATE_LEFT)
        {
              angle = LCD_ANGLE_180;
            support_angel = WIN_SUPPORT_ANGLE_180;
        }
        else if(gesturetype == MMI_GESTURE_ROTATE_RIGHT)
        {
             angle = LCD_ANGLE_0;
            support_angel = WIN_SUPPORT_ANGLE_0;
        }
        break;
    case LCD_ANGLE_180:
        if(gesturetype == MMI_GESTURE_ROTATE_LEFT)
        {
             angle = LCD_ANGLE_270;
            support_angel = WIN_SUPPORT_ANGLE_270;
        }
        else if(gesturetype == MMI_GESTURE_ROTATE_RIGHT)
        {
             angle = LCD_ANGLE_90;
            support_angel = WIN_SUPPORT_ANGLE_90;
        }
        break;
    case LCD_ANGLE_270:
        if(gesturetype == MMI_GESTURE_ROTATE_LEFT)
        {
            angle = LCD_ANGLE_0;
            support_angel = WIN_SUPPORT_ANGLE_0;
        }
        else if(gesturetype == MMI_GESTURE_ROTATE_RIGHT)
        {
            angle = LCD_ANGLE_180;
            support_angel = WIN_SUPPORT_ANGLE_180;
        }
        break;
    default:
        break;
    }
    
    MMK_SetWinSupportAngle(win_id,support_angel);
    MMK_SetWinAngle(win_id,angle,TRUE);
}

/*****************************************************************************/
//  Description : handle preview glide win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicGlideWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    uint32              item_index      = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32              pic_num         = MMIPICVIEW_GetAllPicNum();
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id         = GetMiddleAnimCtrlId();
    MMI_CTRL_ID_T       setlist_ctrl_id = MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id    = MMIPICVIEW_LIST_CTRL_ID;
    MMI_TP_POINTS_T tp_points = {0};

    //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg msg_id = 0x%x "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1947_112_2_18_2_46_32_38,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_OPEN_WINDOW "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1951_112_2_18_2_46_32_39,(uint8*)"");
#ifdef DRM_SUPPORT
        s_mmipicview_is_consume_rights = FALSE;
        s_mmipicview_consume_rights_index = 0;
#endif
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);

        //set display preview osd
        SetDisplayPreviewOsd(TRUE);

        SetPreviewOsd((item_index + 1), pic_num, setlist_ctrl_id, win_id);

        //init glide anim param
        InitGlideAnimParam(item_index, pic_num, win_id);
#if defined(PICVIEW_F_VIEW_ZOOM)        
        SetZoomRatio(GetMiddleAnimCtrlId(), MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM1, MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM2);
        SetZoomRatio(GetAssistantAnimCtrlId(), MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM1, MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM2);
#endif
        break;

    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_GET_FOCUS "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1972_112_2_18_2_46_32_40,(uint8*)"");
        //set display preview osd
        SetDisplayPreviewOsd(TRUE);
        if(0 != s_mmipicview_glide_tp_x_offset)
        {
            s_mmipicview_glide_tp_x_offset = 0;
        }
        break;

    case MSG_LOSE_FOCUS:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_LOSE_FOCUS "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_1978_112_2_18_2_46_32_41,(uint8*)"");
        StopDisplayTimer();

        //reset control rect and display rect
        SetLeftAnimRect(FALSE);
        SetRightAnimRect(FALSE);
        SetMidAnimRect(FALSE);
//         //GUIANIM_SetVisible(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, FALSE, FALSE);
//         SetProgressAnimDispRect(win_id, MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, FALSE);
        break;

    case MSG_TIMER:
        result = HandlePreviewTimer(*(uint8*)param, win_id, ctrl_id, setlist_ctrl_id);

        break;

#ifdef PIC_PREVIEW_U200_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(PNULL != param)
        {
            switch(((MMI_NOTIFY_T*) param)->src_id)
            {
            case MMIPICVIEW_RETURN_BUTTON_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, param, PNULL);
                break;

            case MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID:
                MMIAPIPICVIEW_DeletePreviewFile(list_ctrl_id);
                break;

            case MMIPICVIEW_OPTION_BUTTON_CTRL_ID:
                 MMK_PostMsg(win_id, MSG_APP_MENU, param, PNULL);
                break;
                
            default:
                break;
            }
        }
        break;
#endif  //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_LCD_SWITCH:
        //SetPreviewOsd((item_index + 1), pic_num, setlist_ctrl_id, win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_OK "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2025_112_2_18_2_46_32_42,(uint8*)"");
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_OPT_WIN_TAB, PNULL);
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_WEB "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2037_112_2_18_2_46_32_43,(uint8*)"");
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            //stop timer
            StopDisplayTimer();

            MMIPICVIEW_SliePlay(list_ctrl_id);
//             MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_UP:
    case MSG_APP_LEFT:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg SCI_TRACE_LOW "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2050_112_2_18_2_46_32_44,(uint8*)"");
        //previous picture
        HandleGlideSwitchKey(TRUE,
                             item_index,
                             pic_num,
                             win_id,
                             setlist_ctrl_id,
                             list_ctrl_id);
        break;

    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_RIGHT "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2062_112_2_18_2_46_33_45,(uint8*)"");
        //next picture
        HandleGlideSwitchKey(FALSE,
                             item_index,
                             pic_num,
                             win_id,
                             setlist_ctrl_id,
                             list_ctrl_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_CTL_CANCEL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2074_112_2_18_2_46_33_46,(uint8*)"");
        if (HandleDisplayPreviewOsd(win_id, setlist_ctrl_id))
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg GUIICONLIST_PrepareSpecial "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2077_112_2_18_2_46_33_47,(uint8*)"");
            GUIICONLIST_PrepareSpecial(0, list_ctrl_id);
            MMK_CloseWin(win_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOWN "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2084_112_2_18_2_46_33_48,(uint8*)"");
        HandleGlideTpDown(param);        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_MOVE "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2089_112_2_18_2_46_33_49,(uint8*)"");
        HandleGlideTpMove(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_UP "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2094_112_2_18_2_46_33_50,(uint8*)"");
        HandleGlideTpUp(item_index,
                        pic_num,
                        win_id,
                        setlist_ctrl_id,
                        list_ctrl_id,
                        param);
        if(tp_points_old.num != 0)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MMITHEME_UpdateRect "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2103_112_2_18_2_46_33_51,(uint8*)"");
            MMITHEME_UpdateRect();//手势消息后，防止最后一帧刷屏问题
        }
        SCI_MEMSET(&tp_points_old, 0, sizeof(MMI_TP_POINTS_T));
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOUBLE:
        {
		//SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOUBLE "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2110_112_2_18_2_46_33_52,(uint8*)"");
		//if (!GetTpPointButton(win_id, param))
        if (GUIANIM_TYPE_IMAGE_GIF != GUIANIM_GetAnimType(ctrl_id))
        {
			GUIANIM_ZoomUtmost(GetMiddleAnimCtrlId());
		}
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_NOTIFY_UPDATE:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_UPDATE "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2119_112_2_18_2_46_33_53,(uint8*)"");
        GUIANIM_UpdateRect(*(GUI_RECT_T *)param, ctrl_id);
        MMK_PostMsg(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, MSG_CTL_PAINT, (DPARAM)PNULL, (uint32)0);
        break;

#if defined(TOUCH_GESTURE_ENABLE)
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_GESTURE:
#if defined(PICVIEW_F_VIEW_ZOOM)
	
	//SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE param=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2128_112_2_18_2_46_33_54,(uint8*)"d", MMK_GET_TP_GESTURE(param));

        MMK_GetTpPoints(&tp_points);
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points.num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2131_112_2_18_2_46_33_55,(uint8*)"d", tp_points.num);
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points_old.num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2132_112_2_18_2_46_33_56,(uint8*)"d", tp_points_old.num);
		PicviewGestureZoom(win_id, ctrl_id, item_index,&tp_points, (MMI_MULTI_KEY_TP_MSG_PARAM_T *)param);
#endif
        break;
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 

    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_CLOSE_WINDOW "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2230_112_2_18_2_46_33_61,(uint8*)"");
        //stop timer
        StopDisplayTimer();
		s_picview_is_exchanged = FALSE;
        break;

    case MSG_NOTIFY_ANIM_DISPLAY_IND:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_ANIM_DISPLAY_IND "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2236_112_2_18_2_46_33_62,(uint8*)"");
        {            
            GUIANIM_DISPLAY_IND_T   anim_display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
            
            //解码替换之前先停止提示的gif动画
            if (s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM] == MMK_GetCtrlId(anim_display_ind.ctrl_handle)) 
            {
                //SetProgressAnimDispRect(win_id, MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, FALSE);
                GUIANIM_PauseAnim(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID);
                GUIANIM_SetVisible(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, FALSE, TRUE);

                SCI_TRACE_LOW("MMIPICVIEW HandlePicGlideWinMsg anim_display_ind.result=%d", anim_display_ind.result);
				if (GUIANIM_RESULT_SUCC == anim_display_ind.result)
				{
					ExchangeAssisoMidAnimCtrl();
				}
				else if(!MMIPICVIEW_isFileError(GetMiddleAnimCtrlId(), item_index, MMIPICVIEW_PICOPE_GLID))
				{
                    GUILABEL_SetVisible(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, FALSE, FALSE);
	                SetLabelDisplayByTextId(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, TXT_MULTIM_READ_FAIL);
				}
            }

        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#if 0
/*****************************************************************************/
//  Description : Double Click Check
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DoubleClickCheck(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param)
{
    static uint16 doubleclick_timer_count   = 0;
    static uint8  doubleclick_timer_id      = 0;

    if ((MSG_TIMER == msg_id)&&(*(uint8 *)param == doubleclick_timer_id))
    {
        doubleclick_timer_count = DoubleClick_NONE;
        doubleclick_timer_id = 0;     
        return MMI_RESULT_TRUE;
    }

    if ((MSG_TP_PRESS_DOWN != msg_id)&&(MSG_TP_PRESS_UP != msg_id))
    {
        return MMI_RESULT_FALSE;
    }

    switch(doubleclick_timer_count)
    {
        case DoubleClick_NONE:
        {
            if (MSG_TP_PRESS_DOWN == msg_id)
            {
                doubleclick_timer_count = DoubleClick_DOWN;
            }
        }
        break;
        case DoubleClick_DOWN:
        {
            if (MSG_TP_PRESS_UP == msg_id)
            {
                doubleclick_timer_count = DoubleClick_CLICK;
            }
            else if (MSG_TP_PRESS_DOWN == msg_id)
            {
                doubleclick_timer_count = DoubleClick_DOWN;
            }            
        }
        break;
        case DoubleClick_CLICK:
        {
            if (MSG_TP_PRESS_DOWN == msg_id)
            {
                doubleclick_timer_count = DoubleClick_CLICK_DOWN;
            }
            else if (MSG_TP_PRESS_UP == msg_id)
            {
                doubleclick_timer_count = DoubleClick_NONE;
            }
        }
        break;
        case DoubleClick_CLICK_DOWN:
        {
            if (MSG_TP_PRESS_DOWN == msg_id)
            {
                doubleclick_timer_count = DoubleClick_DOWN;
            }
            else if (MSG_TP_PRESS_UP == msg_id)
            {
                doubleclick_timer_count = DoubleClick_CLICK_CLICK;
            }
        }
        break;
        default:
            return MMI_RESULT_FALSE;        
    }
    
    if ((DoubleClick_NONE < doubleclick_timer_count)&&(doubleclick_timer_count < DoubleClick_CLICK_CLICK))
    {
        //reset timer
        if (0 != doubleclick_timer_id)
        {
            MMK_StopTimer(doubleclick_timer_id);
            doubleclick_timer_id = 0;
        }
        
        doubleclick_timer_id = MMK_CreateWinTimer(win_id, MMIPICVIEW_DOUBLECLICK_TIME, FALSE);
    }
    else if (DoubleClick_NONE == doubleclick_timer_count)
    {
        //stop timer
        if (0 != doubleclick_timer_id)
        {
            MMK_StopTimer(doubleclick_timer_id);
            doubleclick_timer_id = 0;
        }
    }
    else if (DoubleClick_CLICK_CLICK == doubleclick_timer_count )
    {
        //stop timer
        if (0 != doubleclick_timer_id)
        {
            MMK_StopTimer(doubleclick_timer_id);
            doubleclick_timer_id = 0;
        }
        doubleclick_timer_count = DoubleClick_NONE;
        //send MSG_TP_PRESS_DOUBLE
        MMK_SendMsg(win_id, MSG_TP_PRESS_DOUBLE, PNULL);
    }
    
    return MMI_RESULT_TRUE;
}
#endif
/*****************************************************************************/
//  Description : init anim param for glide
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void InitGlideAnimParam(
    uint32        item_index, //from 0
    uint32        total_num,  //total num
    MMI_WIN_ID_T  win_id      //win id
)
{
#if 0
#endif
	s_picview_is_exchanged = FALSE;
    //set glide control id
    InitGlideCtrlId();
#if 0
    if (total_num > 1)
    {
        for (i = 1; i < MMIPICVIEW_GLIDE_PAGE_MAXNUM; i++)
        {
            //当total_num <MMIPICVIEW_GLIDE_PAGE_MAXNUM时，有些控件是重复的
            s_mmipicview_glide_ctrl[(i+middle_anim_ctrl_index)%MMIPICVIEW_GLIDE_PAGE_MAXNUM]
            = s_mmipicview_glide_ctrl[(i%total_num+middle_anim_ctrl_index)%MMIPICVIEW_GLIDE_PAGE_MAXNUM];
        }
    }


    for (i = 0; i < MIN(total_num, MMIPICVIEW_GLIDE_PAGE_MAXNUM); i++)
    {
        //初始化不重复的控件
        if (0 == i)
        {
            SetAnimParam(TRUE, TRUE, (item_index + i) % total_num, s_mmipicview_glide_ctrl[(middle_anim_ctrl_index+i)%MMIPICVIEW_GLIDE_PAGE_MAXNUM]);
        }
        else
        {
            SetAnimParam(FALSE, FALSE, (item_index + i) % total_num, s_mmipicview_glide_ctrl[(middle_anim_ctrl_index+i)%MMIPICVIEW_GLIDE_PAGE_MAXNUM]);
        }
    }

    ModifyGlideCtrlId(FALSE, FALSE, item_index);
#else
    SetGlideAnimCtrlInfo((uint16)item_index,FALSE,FALSE);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
    SetProgressAnimCtrlInfo(win_id);
#endif
    MMK_SetAtvCtrl(win_id, win_id);
    MMITHEME_UpdateRect();

}

/*****************************************************************************/
//  Description : init glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideCtrlId(void)
{
    s_mmipicview_glide_ctrl[0] = MMIPICVIEW_GLIDE1_ANIM_CTRL_ID;
    s_mmipicview_glide_ctrl[1] = MMIPICVIEW_GLIDE2_ANIM_CTRL_ID;
    s_mmipicview_glide_ctrl[2] = MMIPICVIEW_GLIDE3_ANIM_CTRL_ID;
    s_mmipicview_glide_ctrl[3] = MMIPICVIEW_GLIDE4_ANIM_CTRL_ID;
}
/*****************************************************************************/
//  Description : Set glide anim control info
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void SetGlideAnimCtrlInfo(uint16  cur_index,BOOLEAN is_prev,BOOLEAN is_next)
{
    uint32          pic_num = MMIPICVIEW_GetAllPicNum(); 
#ifdef DRM_SUPPORT
	BOOLEAN			is_drm_valid = MMIPICVIEW_IsPreview(FALSE, cur_index);	
#endif
    GUIANIM_RESULT_E anim_result = GUIANIM_RESULT_SUCC;
    //mid
//     MMI_CTRL_ID_T   assistant_anim_id   = GetAssistantAnimCtrlId();
    MMI_CTRL_ID_T   temp_id = 0;
// #if 1
//     if (is_prev)
//     {
//         temp_id = s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2];
//         s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2]     = GetLeftAnimCtrlId();
//         s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2 -1]  = temp_id;
//         //SetAnimParam(TRUE, FALSE, cur_index,assistant_anim_id);
//         //GUIANIM_SetVisible(assistant_anim_id, FALSE, FALSE);
//     }
//     else if (is_next)
//     {
//         temp_id = s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2];
//         s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2]     = GetRightAnimCtrlId();
//         s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2 +1]  = temp_id;
//         //SetAnimParam(TRUE, FALSE, cur_index,assistant_anim_id);
//         //GUIANIM_SetVisible(assistant_anim_id, FALSE, FALSE);
//     }
//     else
//     {
//         SetMidAnimParam(cur_index, 0);
// 		GUIANIM_SetVisible(assistant_anim_id, FALSE, FALSE);
//     }   
// #else
//     assistant_anim_id = SetAnimParam(TRUE, FALSE, cur_index,assistant_anim_id);
//     SetMidAnimParam(cur_index, 0);
// #endif
//     //assistant
//     SetAnimParam(TRUE, FALSE, cur_index,assistant_anim_id);
//     //left
//     //SetAnimParam(FALSE, FALSE,(cur_index-1+ pic_num)%pic_num,GetLeftAnimCtrlId());
//     SetLeftAnimParam(cur_index, pic_num);
//     
//     //right
//     //SetAnimParam(FALSE, FALSE,(cur_index + 1 + pic_num)%pic_num,GetRightAnimCtrlId());
//     SetRightAnimParam(cur_index, pic_num);
    if (is_prev)
    {
        temp_id = GetRightAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2+1]   = GetMiddleAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2]     = GetLeftAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2-1]   = temp_id;
        if(s_picview_is_exchanged)
        {
            temp_id = GetRightAnimCtrlId();
			GUIANIM_SetVisible(temp_id, FALSE, FALSE);
            s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2+1]	= GetAssistantAnimCtrlId();
            s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM] = temp_id;
            s_picview_is_exchanged = FALSE;
        }
#ifdef DRM_SUPPORT
		if(!is_drm_valid)
		{
			SetMidAnimParam(cur_index, 0);
		}
#endif
        anim_result = SetAnimParam(TRUE, FALSE, cur_index,GetAssistantAnimCtrlId());
        SetLeftAnimParam(cur_index, pic_num);
		
    }
    else if (is_next)
    {
        temp_id = GetLeftAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2-1]   = GetMiddleAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2]     = GetRightAnimCtrlId();
        s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2+1]   = temp_id;
        if(s_picview_is_exchanged)
        {
            temp_id = GetLeftAnimCtrlId();
			GUIANIM_SetVisible(temp_id, FALSE, FALSE);
            s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2-1]	= GetAssistantAnimCtrlId();
            s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM] = temp_id;
			s_picview_is_exchanged = FALSE;
        }
#ifdef DRM_SUPPORT
		if(!is_drm_valid)
		{
			SetMidAnimParam(cur_index, 0);
		}
#endif
        anim_result = SetAnimParam(TRUE, FALSE, cur_index,GetAssistantAnimCtrlId());
        SetRightAnimParam(cur_index, pic_num);
    }
    else
    {
        SetMidAnimParam(cur_index, 0);
        anim_result = SetAnimParam(TRUE, FALSE, cur_index,GetAssistantAnimCtrlId());
        SetLeftAnimParam(cur_index, pic_num);
        SetRightAnimParam(cur_index, pic_num);
    }
	GUIANIM_SetVisible(GetMiddleAnimCtrlId(), TRUE, TRUE);
	GUIANIM_SetVisible(GetLeftAnimCtrlId(), TRUE, TRUE);
	GUIANIM_SetVisible(GetRightAnimCtrlId(), TRUE, TRUE);

    SetLeftAnimRect(TRUE);
    SetRightAnimRect(TRUE);
    SetMidAnimRect(TRUE);

    //update
    MMITHEME_UpdateRect();
    if(is_prev || is_next)
    {
		//         GUIANIM_ResumeAnim(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID);
		//         GUIANIM_SetVisible(MMIPICVIEW_PROGRESS_ANIM_CTRL_ID, TRUE, TRUE);
#ifdef DRM_SUPPORT
		if(!is_drm_valid)
		{
			MMIPICVIEW_IsPreview(TRUE, cur_index);
		}
		else
#endif
		{
    		if(GUIANIM_RESULT_BIG_RESOLUTION == anim_result)          
    	    {
                SetLabelDisplayByTextId(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, TXT_MULTIM_PICTOO_LARGE);
    	    }
			else //if(!MMIPICVIEW_isFileError(GetMiddleAnimCtrlId(), cur_index, MMIPICVIEW_PICOPE_GLID))
			{
#ifdef PIC_PREVIEW_U200_SUPPORT
				SetProgressAnimCtrlInfo(MMIPICVIEW_PREVIEW_WIN_ID);
#endif
                GUILABEL_SetVisible(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, FALSE, FALSE);
			}
		}
        
    }
}


/*****************************************************************************/
//  Description : get left glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetLeftAnimCtrlId(void)
{
    uint16  left_anim = MMIPICVIEW_GLIDE_PAGE_MAXNUM / 2;
    return (s_mmipicview_glide_ctrl[ left_anim-1]);
}

/*****************************************************************************/
//  Description : get Middle glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetMiddleAnimCtrlId(void)
{
    uint16  middle_anim = MMIPICVIEW_GLIDE_PAGE_MAXNUM / 2 + 1 ;
    return (s_mmipicview_glide_ctrl[middle_anim -1]);
}

/*****************************************************************************/
//  Description : get Assistant glide anim control id
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetAssistantAnimCtrlId(void)
{
    return s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM];
}

/*****************************************************************************/
//  Description : Exchange Assistant glide anim control with mid anim ctrl
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ExchangeAssisoMidAnimCtrl(void)
{
    GUI_BOTH_RECT_T both_rect       = {0};
    GUI_RECT_T      display_rect    = {0};
    MMI_CTRL_ID_T   assistant_anim_id   = GetAssistantAnimCtrlId();
    MMI_CTRL_ID_T   mid_anim_id         = GetMiddleAnimCtrlId();

    //SCI_TRACE_LOW:"MMIPICVIEW ExchangeAssisoMidAnimCtrl"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2534_112_2_18_2_46_33_64,(uint8*)"");
    GUIAPICTRL_GetBothRect(mid_anim_id,&both_rect);
    GUIANIM_GetDisplayRect(mid_anim_id,&display_rect);
    GUIANIM_SetVisible(mid_anim_id,FALSE,FALSE);

    s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM]   = mid_anim_id;
    s_mmipicview_glide_ctrl[MMIPICVIEW_GLIDE_PAGE_MAXNUM/2] = assistant_anim_id;                
	s_picview_is_exchanged = TRUE;

    mid_anim_id = GetMiddleAnimCtrlId();
    GUIANIM_SetVisible(mid_anim_id, TRUE, TRUE);
    GUIAPICTRL_SetBothRect(mid_anim_id, &both_rect);
    GUIANIM_SetDisplayRect(mid_anim_id, &display_rect, TRUE);
    MMITHEME_UpdateRect();
}
/*****************************************************************************/
//  Description : get right glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetRightAnimCtrlId(void)
{
    uint16  right_anim = MMIPICVIEW_GLIDE_PAGE_MAXNUM / 2 + 1 + 1 ;
    return (s_mmipicview_glide_ctrl[ right_anim -1]);
}

/*****************************************************************************/
//  Description : set left glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimRect(
    BOOLEAN  is_update
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      display_rect = {0};
    MMI_CTRL_ID_T   ctrl_id = GetLeftAnimCtrlId();
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(MMIPICVIEW_PREVIEW_WIN_ID);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //screen wide mode
    if (lcd_width > lcd_height)
    {
        both_rect.h_rect.right  = -1;
        both_rect.h_rect.left   = (int16)(both_rect.h_rect.right - lcd_width + 1);
//         both_rect.h_rect.bottom = (int16)(lcd_height - 1);

        both_rect.v_rect.right  = -1;
        both_rect.v_rect.left   = (int16)(both_rect.v_rect.right - lcd_height + 1);
//         both_rect.v_rect.bottom = (int16)(lcd_width - 1);
    }
    else
    {
        both_rect.v_rect.right  = -1;
        both_rect.v_rect.left   = (int16)(both_rect.v_rect.right - lcd_width + 1);
//         both_rect.v_rect.bottom = (int16)(lcd_height - 1);

        both_rect.h_rect.right  = -1;
        both_rect.h_rect.left   = (int16)(both_rect.h_rect.right - lcd_height + 1);
//         both_rect.h_rect.bottom = (int16)(lcd_width - 1);
    }
//     both_rect.v_rect.top = MMITHEME_GetStatusBarHeight();
//     both_rect.h_rect.top = both_rect.v_rect.top ;
    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set middle glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimRect(
    BOOLEAN   is_update
)
{
    MMI_CTRL_ID_T       ctrl_id = GetMiddleAnimCtrlId();
    GUI_RECT_T          display_rect = MMITHEME_GetClientRectEx(MMIPICVIEW_PREVIEW_WIN_ID);
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetWinClientBothRect(MMIPICVIEW_PREVIEW_WIN_ID);
    
//     both_rect.v_rect.top = MMITHEME_GetStatusBarHeight();
//     both_rect.h_rect.top = both_rect.v_rect.top ;
    
    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set right glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimRect(
    BOOLEAN     is_update
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      display_rect = {0};
    MMI_CTRL_ID_T   ctrl_id = GetRightAnimCtrlId();
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(MMIPICVIEW_PREVIEW_WIN_ID);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //screen wide mode
    if (lcd_width > lcd_height)
    {
        both_rect.h_rect.left   = lcd_width;
        both_rect.h_rect.right  = (int16)(both_rect.h_rect.left + lcd_width - 1);
//         both_rect.h_rect.bottom = (int16)(lcd_height - 1);

        both_rect.v_rect.left   = lcd_height;
        both_rect.v_rect.right  = (int16)(both_rect.v_rect.left + lcd_height - 1);
//         both_rect.v_rect.bottom = (int16)(lcd_width - 1);
    }
    else
    {
        both_rect.v_rect.left   = lcd_width;
        both_rect.v_rect.right  = (int16)(both_rect.v_rect.left + lcd_width - 1);
//         both_rect.v_rect.bottom = (int16)(lcd_height - 1);

        both_rect.h_rect.left   =  lcd_height;
        both_rect.h_rect.right  = (int16)(both_rect.h_rect.left + lcd_height - 1);
//         both_rect.h_rect.bottom = (int16)(lcd_width - 1);
    }
    
//     both_rect.v_rect.top = MMITHEME_GetStatusBarHeight();
//     both_rect.h_rect.top = both_rect.v_rect.top ;
    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}
/*****************************************************************************/
//  Description : set left anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimParam(
    uint32      item_index, //in:from 0,middle item index
    uint32      total_num   //in:total num
)
{
    uint32          start_index = 0;
    GUI_BG_T        error_bg = {0};
    MMI_CTRL_ID_T   ctrl_id = GetLeftAnimCtrlId();

    //SCI_TRACE_LOW:"SetLeftAnimParam:total_num:%d,item_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2678_112_2_18_2_46_34_65,(uint8*)"dd", total_num, item_index);

    if (1 < total_num)
    {
        //set start index
        start_index = item_index;

        //set left index
        if (0 < item_index)
        {
            item_index = item_index - 1;
        }
        else
        {
            item_index = total_num - 1;
        }

//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//        {
//            //set current index
//            if (0 < item_index)
//            {
//                item_index = item_index - 1;
//            }
//            else
//            {
//                item_index = total_num - 1;
//            }
//
//            if (start_index == item_index)
//            {
//                //no error bg
//                error_bg.bg_type = GUI_BG_NONE;
//                GUIANIM_SetErrorBg(ctrl_id, &error_bg);
//                return;
//            }
//        }
//#endif

        //set left anim param
        SetAnimParam(FALSE, FALSE, item_index, ctrl_id);
    }
    else
    {
        //no error bg
        error_bg.bg_type = GUI_BG_COLOR;
        error_bg.color = MMI_BLACK_COLOR; 
        GUIANIM_SetErrorBg(ctrl_id, &error_bg);
    }
}

/*****************************************************************************/
//  Description : set middle anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimParam(
    uint32           item_index, //in:from 0,middle item index
    MMI_WIN_ID_T     win_id      //in:
)
{
    MMI_CTRL_ID_T   ctrl_id = GetMiddleAnimCtrlId();

    //SCI_TRACE_LOW:"SetMidAnimParam:item_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2740_112_2_18_2_46_34_66,(uint8*)"d", item_index);

    //set middle anim param
    SetAnimParam(FALSE, FALSE, item_index, ctrl_id);

    //set active
    //MMK_SetAtvCtrl(win_id,ctrl_id);
}

/*****************************************************************************/
//  Description : set right anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimParam(
    uint32     item_index, //in:from 0,middle item index
    uint32     total_num   //in:total num
)
{
    uint32          start_index = 0;
    GUI_BG_T        error_bg = {0};
    MMI_CTRL_ID_T   ctrl_id = GetRightAnimCtrlId();

    //SCI_TRACE_LOW:"SetRightAnimParam:total_num:%d,item_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2762_112_2_18_2_46_34_67,(uint8*)"dd", total_num, item_index);

    if (1 < total_num)
    {
        //set start index
        start_index = item_index;

        //set current index
        if (item_index < (total_num - 1))
        {
            item_index = item_index + 1;
        }
        else
        {
            item_index = 0;
        }

//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//        {
//            //set current index
//            if (item_index < (total_num - 1))
//            {
//                item_index = item_index + 1;
//            }
//            else
//            {
//                item_index = 0;
//            }
//
//            if (start_index == item_index)
//            {
//                //no error bg
//                error_bg.bg_type = GUI_BG_NONE;
//                GUIANIM_SetErrorBg(ctrl_id, &error_bg);
//                return;
//            }
//        }
//#endif
        //set right anim param
        SetAnimParam(FALSE, FALSE, item_index, ctrl_id);
    }
    else
    {
        //no error bg
        error_bg.bg_type = GUI_BG_COLOR;
        error_bg.color = MMI_BLACK_COLOR; 
        GUIANIM_SetErrorBg(ctrl_id, &error_bg);
    }
}

/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpDown(
    DPARAM     param   //in:
)
{
    GUI_POINT_T     tp_point = {0};

    //get point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //stop timer
    StopDisplayTimer();

    //is tp softkey
    s_mmipicview_is_tp_softkey = MMIPICVIEW_IsTpOsdSoftkey(&tp_point);
    if (!s_mmipicview_is_tp_softkey)
    {
        //get point
        s_mmipicview_glide_tp_start_point = tp_point;
        s_mmipicview_glide_tp_prev_point  = s_mmipicview_glide_tp_start_point;
        s_mmipicview_glide_tp_x_offset      =  0;
    }

    //glide is move
    s_mmipicview_is_glide_move = FALSE;
}
#endif
/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsTpOsdSoftkey(
    GUI_POINT_T    *point_ptr
)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  softkey_rect = {0};

    //get softkey rect
    softkey_rect = MMITHEME_GetSoftkeyRect(0);
    if ((point_ptr->y >= softkey_rect.top) &&
            (point_ptr->y <= softkey_rect.bottom))
    {
        result = TRUE;
    }

    return (result);
}
#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic
/*****************************************************************************/
//  Description : handle picture glide tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpMove(
    DPARAM     param   //in:
)
{
    int16           move_x = 0;
    int16           move_y = 0;

    GUI_POINT_T     tp_point    = {0};
    GUI_RECT_T  clip_rect       = {0};
    GUI_RECT_T  anim_rect       = {0};
    uint16      image_width     = 0, image_height = 0;
    uint16      lcd_width       = 0;
    int16       pic_move_x      = 0;
    MMI_CTRL_ID_T   mid_ctrl_id = GetMiddleAnimCtrlId();

    if (!s_mmipicview_is_tp_softkey && INVALID_TP_POINT_X != s_mmipicview_glide_tp_start_point.x)
    {
        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //set move x
        move_x = (int16)(tp_point.x - s_mmipicview_glide_tp_prev_point.x);
        move_y = (int16)(tp_point.y - s_mmipicview_glide_tp_prev_point.y);

        //set previous tp point
        s_mmipicview_glide_tp_prev_point = tp_point;

        //set not display osd
        SetDisplayPreviewOsd(FALSE);

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

        GUIANIM_GetImgWidthHeightEx(mid_ctrl_id, &image_width, &image_height);
        GUIANIM_GetZoomClipRect(mid_ctrl_id, & clip_rect);
        if (image_width > lcd_width)
        {
            //移动左中右三个控件
            GUIAPICTRL_GetRect(GetMiddleAnimCtrlId(), &anim_rect);
            if ((move_x > 0) && (anim_rect.left < 0))
            {
                if ((move_x + anim_rect.left) <= 0)
                {
                    s_mmipicview_glide_tp_x_offset += move_x;
                    SetCtrlRectDisplayRect(move_x, GetLeftAnimCtrlId());
                    SetCtrlRectDisplayRect(move_x, GetRightAnimCtrlId());
                    SetCtrlRectDisplayRect(move_x, GetMiddleAnimCtrlId());
                    move_x = 0;
                }
                else
                {
                    s_mmipicview_glide_tp_x_offset -= anim_rect.left;
                    SetCtrlRectDisplayRect(-anim_rect.left, GetLeftAnimCtrlId());
                    SetCtrlRectDisplayRect(-anim_rect.left, GetRightAnimCtrlId());
                    SetCtrlRectDisplayRect(-anim_rect.left, GetMiddleAnimCtrlId());
                    move_x += anim_rect.left;
                }
            }
            else if ((move_x < 0) && (anim_rect.left > 0))
            {
                if ((move_x + anim_rect.left) >= 0)
                {
                    s_mmipicview_glide_tp_x_offset += move_x;
                    SetCtrlRectDisplayRect(move_x, GetLeftAnimCtrlId());
                    SetCtrlRectDisplayRect(move_x, GetRightAnimCtrlId());
                    SetCtrlRectDisplayRect(move_x, GetMiddleAnimCtrlId());
                    move_x = 0;
                }
                else
                {
                    s_mmipicview_glide_tp_x_offset -= anim_rect.left;
                    SetCtrlRectDisplayRect(-anim_rect.left, GetLeftAnimCtrlId());
                    SetCtrlRectDisplayRect(-anim_rect.left, GetRightAnimCtrlId());
                    SetCtrlRectDisplayRect(-anim_rect.left, GetMiddleAnimCtrlId());
                    move_x += anim_rect.left;
                }
            }

            if((clip_rect.right - clip_rect.left) > 0 || (clip_rect.bottom - clip_rect.top) > 0)
            {
            //计算中间图片的x方向滑动距离
            if ((move_x > 0) && (clip_rect.left > 0))
            {
                if (move_x > clip_rect.left)
                {
                    //能滑到边
                    pic_move_x = clip_rect.left;
                    move_x     = move_x - pic_move_x;
                }
                else
                {
                    pic_move_x = move_x;
                    move_x     = 0;
                }
            }
            else if ((move_x < 0) && (image_width - 1 > clip_rect.right))
            {
                if ((image_width - 1) >= (clip_rect.right - move_x))
                {
                    pic_move_x = move_x;
                    move_x     = 0;
                }
                else
                {
                    //能滑到边
                    pic_move_x = clip_rect.right - (image_width - 1);
                    move_x     = move_x - pic_move_x;
                }
            }
            //移动中间的图片
            if ((0 != pic_move_x) || (0 != move_y))
            {
                GUIANIM_ZoomMove(mid_ctrl_id, -pic_move_x, -move_y, TRUE);
            }
            }
        }
        //移动左中右三个控件
        if (0 != move_x)
        {
            s_mmipicview_glide_tp_x_offset += move_x;
            SetCtrlRectDisplayRect(move_x, GetLeftAnimCtrlId());
            SetCtrlRectDisplayRect(move_x, GetRightAnimCtrlId());
            SetCtrlRectDisplayRect(move_x, GetMiddleAnimCtrlId());
        }
    }
    MMITHEME_UpdateRect();
}

/*****************************************************************************/
//  Description : sest control rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRectDisplayRect(
    int16             move_x, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
)
{
    GUI_RECT_T      ctrl_rect = {0};
    GUI_RECT_T      display_rect = {0};

    //get control rect
    GUIAPICTRL_GetRect(ctrl_id, &ctrl_rect);

    ctrl_rect.left  = (int16)(ctrl_rect.left + move_x);
    ctrl_rect.right = (int16)(ctrl_rect.right + move_x);

    //set control rect
    GUIANIM_SetCtrlRect(ctrl_id, &ctrl_rect);

    //get display rect
    display_rect = MMITHEME_GetFullScreenRect();
    if (!GUI_IntersectRect(&display_rect, ctrl_rect, display_rect))
    {
        display_rect.left   = 0;
        display_rect.top    = 0;
        display_rect.right  = 0;
        display_rect.bottom = 0;
    }

    //set display rect
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, TRUE);
}

/*****************************************************************************/
//  Description : handle picture glide tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpUp(
    uint32           item_index,     //in:from 0
    uint32           total_num,      //in:total num
    MMI_WIN_ID_T     win_id,         //in:
    MMI_CTRL_ID_T    setlist_ctrl_id,//in:
    MMI_CTRL_ID_T    list_ctrl_id,   //in:
    DPARAM           param           //in:
)
{
    BOOLEAN         is_prev = FALSE;
    BOOLEAN         is_next = FALSE;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          lcd_width = 0;
    uint16          i = 0;
    uint16          start_index = 0;
    GUI_POINT_T     tp_point = {0};

    if (!s_mmipicview_is_tp_softkey && INVALID_TP_POINT_X != s_mmipicview_glide_tp_start_point.x && 0 != s_mmipicview_glide_tp_x_offset)
    {
        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //get lcd width
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

        //set start index
        start_index = item_index;

        //is current picture
        if ((1 == total_num) ||
                (abs(s_mmipicview_glide_tp_x_offset) < (lcd_width / MMIPICVIEW_GLIDE_RATIO)))
        {
            //reset
            move_width = -s_mmipicview_glide_tp_x_offset;//(int16)(s_mmipicview_glide_tp_start_point.x - s_mmipicview_glide_tp_prev_point.x);
        }
        else//previous or next picture
        {
            //set move space
            if (0 < s_mmipicview_glide_tp_x_offset)
            {
                //previous
                is_prev = TRUE;
                move_width = (int16)(lcd_width - s_mmipicview_glide_tp_x_offset);//(int16)(lcd_width + s_mmipicview_glide_tp_start_point.x - s_mmipicview_glide_tp_prev_point.x);

                //set current index
                if (0 < item_index)
                {
                    item_index = item_index - 1;
                }
                else
                {
                    item_index = total_num - 1;
                }

//#ifdef DRM_SUPPORT
//                while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//                {
//                    //set current index
//                    if (0 < item_index)
//                    {
//                        item_index = item_index - 1;
//                    }
//                    else
//                    {
//                        item_index = total_num - 1;
//                    }
//
//                    if (start_index == item_index)
//                    {
//                        break;
//                    }
//                }
//#endif
                if (start_index == item_index)
                {
                    //reset
                    is_prev = FALSE;
                    is_next = FALSE;
                    move_width = (int16)(s_mmipicview_glide_tp_start_point.x - s_mmipicview_glide_tp_prev_point.x);
                }
            }
            else if (0 > s_mmipicview_glide_tp_x_offset)
            {
                //next
                is_next = TRUE;
                move_width = (int16)(s_mmipicview_glide_tp_start_point.x - s_mmipicview_glide_tp_prev_point.x - lcd_width);

                //set current index
                if (item_index < (total_num - 1))
                {
                    item_index = item_index + 1;
                }
                else
                {
                    item_index = 0;
                }

//#ifdef DRM_SUPPORT
//                while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//                {
//                    //set current index
//                    if (item_index < (total_num - 1))
//                    {
//                        item_index = item_index + 1;
//                    }
//                    else
//                    {
//                        item_index = 0;
//                    }
//
//                    if (start_index == item_index)
//                    {
//                        break;
//                    }
//                }
//#endif
                if (start_index == item_index)
                {
                    //reset
                    is_prev = FALSE;
                    is_next = FALSE;
                    move_width = (int16)(s_mmipicview_glide_tp_start_point.x - s_mmipicview_glide_tp_prev_point.x);
                }
            }
            else
            {
                move_width = 0;
            }


        }

        //auto move
        for (i = 1; i <= MMIPICVIEW_GLIDE_AUTO_FRAME; i++)
        {
            //set move space
            if (i <= (MMIPICVIEW_GLIDE_AUTO_FRAME / 2))
            {
                move_offset = (int16)((2 * move_width * i * i) / 100);
            }
            else
            {
                move_offset = (int16)(move_width - (2 * move_width * (MMIPICVIEW_GLIDE_AUTO_FRAME - i) * (MMIPICVIEW_GLIDE_AUTO_FRAME - i)) / 100);
            }

            //set move space
            move_space = (int16)(move_offset - move_all);
            move_all   = (int16)(move_all + move_space);

            if (0 != move_space)
            {
                //set control rect and display rect
                SetCtrlRectDisplayRect(move_space, GetLeftAnimCtrlId());
                SetCtrlRectDisplayRect(move_space, GetRightAnimCtrlId());
                SetCtrlRectDisplayRect(move_space, GetMiddleAnimCtrlId());

                //update
                MMITHEME_UpdateRect();
            }
        }

        //modify control id and load
        //ModifyGlideCtrlId(is_prev,is_next,(uint16)item_index);
        if (start_index != item_index)
        {
            SetGlideAnimCtrlInfo((uint16)item_index,is_prev,is_next);
            //set list index
            MMIPICVIEW_SetCurIndex(list_ctrl_id, (uint16)item_index);
            //modify title
            ModifyPreviewTitle((item_index + 1), total_num, setlist_ctrl_id);
        }
    }

    //set not display preview osd
    SetDisplayPreviewOsd(FALSE);

    if (!s_mmipicview_is_glide_move)
    {
        //display preview osd
        HandleDisplayPreviewOsd(win_id, setlist_ctrl_id);
    }

    s_mmipicview_glide_tp_start_point.x = INVALID_TP_POINT_X;
    s_mmipicview_glide_tp_start_point.y = INVALID_TP_POINT_Y;
#if defined(PICVIEW_F_VIEW_ZOOM)
    SetZoomRatio(GetMiddleAnimCtrlId(), MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM1, MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM2);
#endif
    //set active
    //MMK_SetAtvCtrl(win_id,GetMiddleAnimCtrlId());
}

/*****************************************************************************/
//  Description : switch picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideSwitchKey(
    BOOLEAN         is_prev,
    uint32          item_index,     //in:from 0
    uint32          total_num,      //in:total num
    MMI_WIN_ID_T    win_id,         //in:
    MMI_CTRL_ID_T   setlist_ctrl_id,//in:
    MMI_CTRL_ID_T   list_ctrl_id    //in:
)
{
    int16           move_space = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    uint16          lcd_width = 0;
    uint16          i = 0;
    uint32			start_index = 0;
    BOOLEAN         is_next = FALSE;
    
    //is current picture
    if (1 < total_num)
    {
        //set start index
        start_index = item_index;

        //set move space
        if (is_prev)
        {
            //previous
            move_width = (int16)(lcd_width);

            //set current index
            if (0 < item_index)
            {
                item_index = item_index - 1;
            }
            else
            {
                item_index = total_num - 1;
            }

//#ifdef DRM_SUPPORT
//            while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//            {
//                //set current index
//                if (0 < item_index)
//                {
//                    item_index = item_index - 1;
//                }
//                else
//                {
//                    item_index = total_num - 1;
//                }
//
//                if (start_index == item_index)
//                {
//                    break;
//                }
//            }
//#endif
        }
        else
        {
            //next
            move_width = (int16)(-lcd_width);

            //set current index
            if (item_index < (total_num - 1))
            {
                item_index = item_index + 1;
            }
            else
            {
                item_index = 0;
            }

//#ifdef DRM_SUPPORT
//            while (!MMIPICVIEW_IsPreview(FALSE, item_index))
//            {
//                //set current index
//                if (item_index < (total_num - 1))
//                {
//                    item_index = item_index + 1;
//                }
//                else
//                {
//                    item_index = 0;
//                }
//
//                if (start_index == item_index)
//                {
//                    break;
//                }
//            }
//#endif
        }

        if (item_index != start_index)
        {
            //stop timer
            StopDisplayTimer();

            //set not display preview osd
            SetDisplayPreviewOsd(FALSE);

            //get lcd width
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

            //auto move
            for (i = 1; i < MMIPICVIEW_GLIDE_KEY_FRAME; i++)
            {
                //set move space
                if (i <= (MMIPICVIEW_GLIDE_KEY_FRAME / 2))
                {
                    move_offset = (int16)((2 * move_width * i * i) / 100);
                }
                else
                {
                    move_offset = (int16)(move_width - (2 * move_width * (MMIPICVIEW_GLIDE_KEY_FRAME - i) * (MMIPICVIEW_GLIDE_KEY_FRAME - i)) / 100);
                }

                //set move space
                move_space = (int16)(move_offset - move_all);
                move_all   = (int16)(move_all + move_space);

                if (0 == move_space)
                {
                    break;
                }
                else
                {
                    //set control rect and display rect
                    SetCtrlRectDisplayRect(move_space, GetLeftAnimCtrlId());
                    SetCtrlRectDisplayRect(move_space, GetRightAnimCtrlId());
                    SetCtrlRectDisplayRect(move_space, GetMiddleAnimCtrlId());

                    //update
                    MMITHEME_UpdateRect();
                }
            }

            //modify control id and load
            //ModifyGlideCtrlId(is_prev,(BOOLEAN)(!is_prev),(uint16)item_index);
            if(!is_prev)
            {
                is_next = TRUE;
            }
            SetGlideAnimCtrlInfo((uint16)item_index,is_prev, is_next);

            //set list index
            MMIPICVIEW_SetCurIndex(list_ctrl_id, (uint16)item_index);

            //modify title
            ModifyPreviewTitle((item_index + 1), total_num, setlist_ctrl_id);

            //set active
            MMK_SetAtvCtrl(win_id, GetMiddleAnimCtrlId());
        }
    }
}
#else
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture picview tp move msg
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void HandlePicviewTpMove(
    DPARAM     param   //in:
)
{
    int16           move_x = 0;
    int16           move_y = 0;

    GUI_POINT_T     tp_point    = {0};
    GUI_RECT_T  clip_rect       = {0};
    GUI_RECT_T  anim_rect       = {0};
    uint16      image_width     = 0, image_height = 0;
    uint16      lcd_width       = 0;
    int16       pic_move_x      = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_PREVIEW_ANIM_CTRL_ID;

    if (!s_mmipicview_is_tp_softkey && INVALID_TP_POINT_X != s_mmipicview_glide_tp_start_point.x)
    {
        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //set move x
        move_x = (int16)(tp_point.x - s_mmipicview_glide_tp_prev_point.x);
        move_y = (int16)(tp_point.y - s_mmipicview_glide_tp_prev_point.y);

        //set previous tp point
        s_mmipicview_glide_tp_prev_point = tp_point;

        //set not display osd
        SetDisplayPreviewOsd(FALSE);

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

        GUIANIM_GetImgWidthHeightEx(ctrl_id, &image_width, &image_height);
        GUIANIM_GetZoomClipRect(ctrl_id, & clip_rect);
        if (image_width > lcd_width)
        {
            if((clip_rect.right - clip_rect.left) > 0 || (clip_rect.bottom - clip_rect.top) > 0)
            {
            //计算中间图片的x方向滑动距离
            if ((move_x > 0) && (clip_rect.left > 0))
            {
                if (move_x > clip_rect.left)
                {
                    //能滑到边
                    pic_move_x = clip_rect.left;
                    move_x     = move_x - pic_move_x;
                }
                else
                {
                    pic_move_x = move_x;
                    move_x     = 0;
                }
            }
            else if ((move_x < 0) && (image_width - 1 > clip_rect.right))
            {
                if ((image_width - 1) >= (clip_rect.right - move_x))
                {
                    pic_move_x = move_x;
                    move_x     = 0;
                }
                else
                {
                    //能滑到边
                    pic_move_x = clip_rect.right - (image_width - 1);
                    move_x     = move_x - pic_move_x;
                }
            }
            //移动中间的图片
            if ((0 != pic_move_x) || (0 != move_y))
            {
                GUIANIM_ZoomMove(ctrl_id, -pic_move_x, -move_y, TRUE);
            }
            }
        }
    }
    MMITHEME_UpdateRect();
}

/*****************************************************************************/
//  Description : handle picture picview tp down msg
//  Global resource dependence :
//  Author: yaye
//  Note:
/*****************************************************************************/
LOCAL void HandlePicviewTpDown(
    DPARAM     param   //in:
)
{
    GUI_POINT_T     tp_point = {0};

    //get point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //stop timer
    StopDisplayTimer();

    //is tp softkey
    s_mmipicview_is_tp_softkey = MMIPICVIEW_IsTpOsdSoftkey(&tp_point);
    if (!s_mmipicview_is_tp_softkey)
    {
        //get point
        s_mmipicview_glide_tp_start_point = tp_point;
        s_mmipicview_glide_tp_prev_point  = s_mmipicview_glide_tp_start_point;
    }

    //glide is move
    s_mmipicview_is_glide_move = FALSE;
}
#endif
#endif
/*****************************************************************************/
//  Description : modify name,modify current preview anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_ModifyName(
    uint16    item_index  //from 0
)
{
    MMI_WIN_ID_T    win_id = MMIPICVIEW_PREVIEW_WIN_ID;

    if (MMK_IsOpenWin(win_id))
    {
#ifdef PIC_GLIDE_SUPPORT
        //SetMidAnimParam(item_index, win_id);
        SetAnimParam(TRUE, FALSE, item_index,GetMiddleAnimCtrlId());
#else
        SetAnimParam(TRUE, FALSE, item_index, MMIPICVIEW_PREVIEW_ANIM_CTRL_ID);
#endif
    }
}
#if defined(PICVIEW_F_VIEW_ZOOM)
/*****************************************************************************/
//  Description : set zoom ratio
//  Global resource dependence :
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
LOCAL void SetZoomRatio(
    MMI_HANDLE_T        ctrl_handle,  //in:
    uint16 num1,  //in
    uint16 num2   //in
)
{
    MMITHEME_RATIO_T    zoom_in_ratio_ptr = {0};
    MMITHEME_RATIO_T    zoom_out_ratio_ptr = {0};

    zoom_in_ratio_ptr.num1 = num1;
    zoom_in_ratio_ptr.num2 = num2;
    zoom_out_ratio_ptr.num1 = num2;
    zoom_out_ratio_ptr.num2 = num1;
    GUIANIM_SetZoomRatio(ctrl_handle, &zoom_in_ratio_ptr, &zoom_out_ratio_ptr);
}
#endif
#if 0
/*****************************************************************************/
//  Description : get if tp point at the zoom_in or zoom_out button
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpPointButton(MMI_WIN_ID_T win_id,
                               DPARAM     param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    //GUI_RECT_T          zoomin_point_rect  = {0};
    GUI_RECT_T          zoomout_point_rect  = {0};

    //GUICTRL_GetRect(MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID), &zoomin_point_rect);
    //GUICTRL_GetRect(MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID), &zoomout_point_rect);

    GUI_RECT_T      dc_valid_rect = {0};//double click valid rect
    GUI_RECT_T      return_rect   = {0};
    uint16          font_height   = 0;

    GUICTRL_GetRect(MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID), &zoomout_point_rect);
    font_height = GUIFONT_GetHeight(MMIPICPREVIEW_TITLE_FONT);;

    dc_valid_rect.left    = 0;
    dc_valid_rect.top     = font_height; 
    dc_valid_rect.right   = zoomout_point_rect.right;
    dc_valid_rect.bottom  = zoomout_point_rect.top;
   
    //if((!GUI_PointIsInRect(((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point, zoomin_point_rect))
	//&& (!GUI_PointIsInRect(((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point, zoomout_point_rect)))
    if (GUI_PointIsInRect(((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point, dc_valid_rect))
    {
        result = MMI_RESULT_FALSE;
    }
    
    return result;
}
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
#ifdef PIC_GLIDE_SUPPORT
/*****************************************************************************/
//  Description : init the info of anim for decoding progress
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressAnimCtrlInfo(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T           ctrl_id     = MMIPICVIEW_PROGRESS_ANIM_CTRL_ID;
    GUIANIM_CTRL_INFO_T     control_info= {0};
    GUIANIM_DATA_INFO_T     data_info   = {0};
    GUIANIM_DISPLAY_INFO_T  display_info= {0};
    GUI_BOTH_RECT_T         anim_rect   = MMITHEME_GetWinClientBothRect(win_id);
    uint16                  img_width   = 0;
    uint16                  img_height  = 0;
    
    //设置rect
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DECODE_ON, win_id);
    anim_rect.v_rect.bottom = anim_rect.v_rect.top + img_height - 1;
    anim_rect.h_rect.bottom = anim_rect.h_rect.top + img_height - 1;
	anim_rect.v_rect.right	= anim_rect.v_rect.left+ img_width - 1;
    anim_rect.h_rect.right	= anim_rect.h_rect.left+ img_width - 1;

    GUIAPICTRL_SetBothRect(ctrl_id, &anim_rect);
    
    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id    = ctrl_id;
    
    //data_info.anim_id       = IMAGE_DECODE_ON;
    //data_info.is_bitmap     = FALSE;
    data_info.img_id        = IMAGE_DECODE_ON;//IMAGE_DECODE_ON;
    
    display_info.is_play_once   = FALSE;    
    display_info.bg.bg_type 	= GUI_BG_NONE;//GUI_BG_COLOR;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
    SetProgressAnimDispRect(win_id, ctrl_id, TRUE);
    GUIANIM_SetVisible(ctrl_id, TRUE, TRUE);
}

/*****************************************************************************/
//  Description : set the rect of anim for decoding progress
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressAnimDispRect(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_update)
{
    GUI_RECT_T              disp_rect   = MMITHEME_GetClientRectEx(win_id);       
    uint16                  img_width   = 0;
    uint16                  img_height  = 0;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DECODE_ON, win_id);
    disp_rect.bottom = disp_rect.top + img_height;
    disp_rect.right  = disp_rect.left + img_width;
    GUIANIM_SetDisplayRect(ctrl_id, &disp_rect, is_update);
}
#endif
#endif
/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewNoteRect(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T	note_rect	 = {0};
    
    note_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef PIC_PREVIEW_U200_SUPPORT
    note_rect.v_rect.bottom -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    note_rect.h_rect.bottom -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
#endif
    note_rect.v_rect.bottom = note_rect.v_rect.bottom - 2;
    note_rect.h_rect.bottom = note_rect.h_rect.bottom - 2;
    note_rect.v_rect.top	= note_rect.v_rect.bottom - MMI_DEFAULT_NORMAL_FONT_SIZE - 2;
    note_rect.h_rect.top	= note_rect.h_rect.bottom - MMI_DEFAULT_NORMAL_FONT_SIZE - 2;

    if (!MMITHEME_IsMainScreenLandscape())
    {
        GUILABEL_SetRect(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, &note_rect.v_rect, FALSE);
    }
    else
    {
        GUILABEL_SetRect(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, &note_rect.h_rect, FALSE);
    }
    GUICTRL_SetBothRect((IGUICTRL_T *)  MMK_GetCtrlPtr(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID), &note_rect);
    GUILABEL_SetVisible(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, FALSE, FALSE);
}

/*****************************************************************************/
//  Description : is picture file error and note the 0 size of file
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_isFileError(
                                      MMI_CTRL_ID_T   ctrl_id,
                                      uint32      	  item_index,  //in:
                                      MMIPICVIEW_PICOPE_TYPE_E  ope_type
                                      )
{
    BOOLEAN             result = FALSE;
    FILEARRAY_DATA_T    file_info = {0};
    FILEARRAY_DATA_T    *send_data_ptr = PNULL;
    MMI_STRING_T        note_info   = {0};
	uint16			    pic_width =0;
	uint16			    pic_height =0;
    
    GUILABEL_SetVisible(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, FALSE, FALSE);
    if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
        if(MMIPICVIEW_PICOPE_POINT == ope_type)
        {
        	if(0 == file_info.size)
            {
                result = TRUE;
                MMI_GetLabelTextByLang(TXT_EMPTY_FILE,&note_info); //NEWMS00181048 
                MMIPUB_OpenAlertWinByTextPtr(PNULL,&note_info,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                if (!MMIAPIFMM_GetPictureWidthHeight(file_info.filename,file_info.name_len,file_info.size,&pic_width,&pic_height))
                {
                    result = TRUE;
                    MMI_GetLabelTextByLang(TXT_COMMON_NO_SUPPORT,&note_info);
                    MMIPUB_OpenAlertWinByTextPtr(PNULL,&note_info,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
        }
#ifdef PIC_GLIDE_SUPPORT  //norflashcut_pic 
        else if (MMIPICVIEW_PICOPE_GLID == ope_type && ctrl_id == GetMiddleAnimCtrlId())
        {
            if (0 == file_info.size)
            {
                result = TRUE;
                SetLabelDisplayByTextId(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, TXT_EMPTY_FILE); //NEWMS00181048 
            }
            else
            {
                if (!MMIAPIFMM_GetPictureWidthHeight(file_info.filename,file_info.name_len,file_info.size,&pic_width,&pic_height))
                {
	                result = TRUE;
	                SetLabelDisplayByTextId(MMIPICVIEW_PREVIEW_LABEL_CTRL_ID, TXT_COMMON_NO_SUPPORT);
				}
            }
        }
#endif        
    }
    return (result);
}

/*****************************************************************************/
//  Description : Set Label Display By Text Id
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetLabelDisplayByTextId(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id)
{
    GUILABEL_SetTextById(ctrl_id, text_id, TRUE);
    GUILABEL_SetBackgroundColor(ctrl_id, 1);//MMI_BLACK_COLOR);
    GUILABEL_SetFont(ctrl_id, MMI_DEFAULT_NORMAL_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetVisible(ctrl_id, TRUE, TRUE);
}

/*****************************************************************************/
//  Description : release the memory of anim controls
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_ReleaseAnimMemory(void)
{
	GUIANIM_CTRL_INFO_T		ctrl_info = {0};
	GUIANIM_DATA_INFO_T		data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

	data_info.img_id = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type  = GUI_BG_COLOR;
    display_info.bg.color = MMI_BLACK_COLOR;
    ctrl_info.is_ctrl_id = TRUE;
	
#ifdef PIC_GLIDE_SUPPORT
    ctrl_info.ctrl_id    = MMIPICVIEW_GLIDE1_ANIM_CTRL_ID;
	GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    ctrl_info.ctrl_id    = MMIPICVIEW_GLIDE2_ANIM_CTRL_ID;
	GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    ctrl_info.ctrl_id    = MMIPICVIEW_GLIDE3_ANIM_CTRL_ID;
	GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    ctrl_info.ctrl_id    = MMIPICVIEW_GLIDE4_ANIM_CTRL_ID;
	GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
#else
    ctrl_info.ctrl_id    = MMIPICVIEW_PREVIEW_ANIM_CTRL_ID;
	GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
#endif
}

/*****************************************************************************/
//  Description : slide callback
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RefreshAnimDisplay(uint32	item_index)
{
    uint32              		pic_num         = MMIPICVIEW_GetAllPicNum();
    MMI_CTRL_ID_T       setlist_ctrl_id = MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID;

#ifdef PIC_GLIDE_SUPPORT
        SetMidAnimParam(item_index, 0);
        SetAnimParam(TRUE, FALSE, item_index, GetAssistantAnimCtrlId());
        SetLeftAnimParam(item_index, pic_num);
        SetRightAnimParam(item_index, pic_num);
#else
        SetAnimParam(TRUE, FALSE, item_index, MMIPICVIEW_PREVIEW_ANIM_CTRL_ID);
#endif
        ModifyPreviewTitle((item_index + 1), pic_num, setlist_ctrl_id);
}
#if defined (TOUCH_GESTURE_ENABLE) && defined(PICVIEW_F_VIEW_ZOOM)
/*****************************************************************************/
//  Description : gesture zoom
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL void PicviewGestureZoom(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint32 item_index, MMI_TP_POINTS_T* tp_points, MMI_MULTI_KEY_TP_MSG_PARAM_T* param)
{
    uint16          width_min 	= 0;
    uint16          height_min 	= 0;
	uint16			width_max 	= 0;
	uint16			height_max	= 0;
    uint16          img_width 	= 0;
	uint16          img_height 	= 0;
#ifdef PIC_GLIDE_SUPPORT 
	ctrl_id = GetMiddleAnimCtrlId();
#endif
    if (GUIANIM_TYPE_IMAGE_GIF == GUIANIM_GetAnimType(ctrl_id))
    {
		return;
    }
    else if(tp_points_old.num ==0)
    {   
        SetZoomRatio(ctrl_id, MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM1, MMIPICVIEW_DEFAULT_ZOOM_RATIO_NUM2);
    }
    else
    {
        int16 dist_x = 0;
        int16 dist_y = 0;
        uint32 dist1 = 0;
        uint32 dist2 = 0;
        double dist_temp1 = 0;
        double dist_temp2 = 0;            
        
        dist_x = tp_points_old.points[0].x - tp_points_old.points[1].x;
        dist_y = tp_points_old.points[0].y - tp_points_old.points[1].y;
        dist1 = (dist_x * dist_x + dist_y * dist_y);
        dist_x = tp_points->points[0].x - tp_points->points[1].x;
        dist_y = tp_points->points[0].y - tp_points->points[1].y;
        dist2 = (dist_x * dist_x + dist_y * dist_y);
        
        dist_temp1 = sqrt((double)dist1);
        dist_temp2 = sqrt((double)dist2);
        dist1 = (uint16)dist_temp1;
        dist2 = (uint16)dist_temp2;
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp1 = %.2f"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2157_112_2_18_2_46_33_57,(uint8*)"f", dist_temp1);
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp2 = %.2f"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2158_112_2_18_2_46_33_58,(uint8*)"f", dist_temp2);
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist1 = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2159_112_2_18_2_46_33_59,(uint8*)"d", dist1);
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist2 = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_PREVIEW_2160_112_2_18_2_46_33_60,(uint8*)"d", dist2);
        
        if(dist1 >= dist2)
        {
            SetZoomRatio(ctrl_id, (uint16)dist1, (uint16)dist2);
        }
        else
        {
            SetZoomRatio(ctrl_id, (uint16)dist2, (uint16)dist1);                
        }            
    }
    MMI_MEMCPY(&tp_points_old, sizeof(MMI_TP_POINTS_T),
        tp_points, sizeof(MMI_TP_POINTS_T),
        sizeof(MMI_TP_POINTS_T));
    
    
    if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))
    {
        HandleZoomInPic(item_index);            
        UILAYER_SetDirectDraw(TRUE);
        //UILAYER_SetMainLayerImmutable(TRUE);

        if (GUIANIM_GetZoomWidthRange(ctrl_id, PNULL, &width_max, PNULL, &height_max))
        {
            //get current image width
            GUIANIM_GetImgWidthHeightEx(ctrl_id,
                &img_width,
                &img_height);
            
            if ((img_width >= width_max)||(img_height >= height_max))
            { 
                MMIPUB_DisplayTipsIDEx(PNULL, TXT_PICVIEW_ZOOM_INMAX, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
            }
        }                
    }
    else  if (MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))
    {
        HandleZoomOutPic(item_index);
        UILAYER_SetDirectDraw(TRUE);
        //UILAYER_SetMainLayerImmutable(TRUE);
        
//#ifdef PIC_GLIDE_SUPPORT 
        {
            //图片最小时，缩小手势回到上一个界面
            
            if (GUIANIM_GetZoomWidthRange(ctrl_id, &width_min, PNULL, &height_min, PNULL))
            {
                //get current image width
                GUIANIM_GetImgWidthHeightEx(ctrl_id,
                    &img_width,
                    &img_height);
                
                if ((img_width <= width_min)||(img_height <= height_min))
                { 
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_PICVIEW_ZOOM_OUTMIN, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
					/*
                    //return to picture list window when receive zoom out gesture 
                    if (HandleDisplayPreviewOsd(win_id, MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID))
                    {
                        GUIICONLIST_PrepareSpecial(0, MMIPICVIEW_LIST_CTRL_ID);
                        MMK_CloseWin(win_id);
                    }
                    */
                }
            }                
        }
//#endif
    }
#ifdef PIC_GLIDE_SUPPORT 
    else if(MMI_GESTURE_ROTATE_LEFT == MMK_GET_TP_GESTURE(param) || MMI_GESTURE_ROTATE_RIGHT == MMK_GET_TP_GESTURE(param))
    {
        if(IsDisplaySwitchScreenReferInfoEnable())
        {
//            SetPicGlideWinAngle(MMK_GET_TP_GESTURE(param) );
        }
        
    }
#endif
//     if (GUIANIM_TYPE_IMAGE_GIF == GUIANIM_GetAnimType(ctrl_id))
//     {
//         GUIANIM_PauseAnim(ctrl_id);
//     }
}
#endif
#endif //#ifdef PIC_VIEWER_SUPPORT

/*Edit by script, ignore 19 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
