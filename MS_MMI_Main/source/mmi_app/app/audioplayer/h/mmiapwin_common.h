/*****************************************************************************
** File Name:      mmiapwin_common.h                                         *
** Author:                                                                   *
** Date:           28/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio player                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMIAPWIN_COMMON_H_
#define _MMIAPWIN_COMMON_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvaud_api.h"
#include "mmicom_panel.h"
#include "mmimp3_export.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//Mp3 button type
typedef enum
{
    MMIMP3_BUTTON_SHUFFLE               = 0x00,
    MMIMP3_BUTTON_REPEAT                = 0x01,
    MMIMP3_BUTTON_EQ_OR_LIST            = 0x02,
    MMIMP3_BUTTON_VOLUME_SET            = 0x03,
    MMIMP3_BUTTON_FOCUS_START           = 0x04,
    MMIMP3_BUTTON_PREV                  = 0x04,
    MMIMP3_BUTTON_PLAY_PAUSE            = 0x05,
    MMIMP3_BUTTON_NEXT                  = 0x06,
    MMIMP3_BUTTON_REVIEW_OR_DETAIL      = 0x07,
    MMIMP3_BUTTON_FOCUS_END             = 0x07,
    MMIMP3_BUTTON_LEFT_KEY              = 0x08,
    MMIMP3_BUTTON_RETURN                = 0x09,
    MMIMP3_BUTTON_MAX                   = 0x0A,
}
MMIMP3_BUTTON_TYPE_E;

//Mp3 button info
typedef struct
{
    MMIMP3_BUTTON_TYPE_E  focus_button;                     //当前激活的按钮
    BOOLEAN               button_valid[MMIMP3_BUTTON_MAX];  //按钮有效信息
} MMIAP_BUTTON_INFO_T;

typedef enum
{
    MMIAP_DIRECT_LEFT,
    MMIAP_DIRECT_MID,
    MMIAP_DIRECT_RIGHT,
    MMIAP_DIRECT_MAX
} MMIAP_DIRECT_E;


// 播放入口类型
typedef enum
{
    MMIMP3_ENTRY_TYPE_FAVORATE,
    MMIMP3_ENTRY_TYPE_PLAYER,
    MMIMP3_ENTRY_TYPE_MAX
} MMIMP3_ENTRY_TYPE_E;

//播放异常结果提示回调
typedef void (*MMIMP3_ALERT_CALLBACK)(void);

typedef enum
{
    MMIMP3_MUSIC_FILE_NOT_EXIST,
    MMIMP3_LIST_FILE_NOT_EXIST,
    MMIMP3_MUSIC_FILE_SIZE_ZERO,
    MMIMP3_A2DP_NOT_SUPPORT,
    MMIMP3_FORMAT_NOT_SUPPORT,
    MMIMP3_BT_NOT_SUPPORT_EQ,
#ifdef DRM_SUPPORT
    MMIMP3_DRM_RIGHT_INVALID,
    MMIMP3_DRM_FORMAT_NOT_AUDIO,
    MMIMP3_DRM_NOT_SYS_NITZ,
#endif
    MMIMP3_ALERT_MSG_MAX
} MMIMP3_ALERT_MSG_E;

//Mp3 alert info
typedef struct
{
    MMIMP3_ALERT_MSG_E      alert_type;     //alert 类型
    uint8                   alert_timer;    //timer id
    MMIMP3_ALERT_CALLBACK   alert_callback; //alert callbacks
    MMI_WIN_ID_T            win_id;         //依赖窗口
} MMIAP_ALERT_INFO_T;

typedef struct
{
    BOOLEAN is_review_open; //review 
    BOOLEAN is_reopen_osd_panel; //osd panel
    BOOLEAN is_osd_panel_open; //osd panel
    MMIAP_BUTTON_INFO_T button_info;//
    MMI_CTRL_ID_T glide_ctrl[MMIAP_DIRECT_MAX];// glide ctrl id
#ifdef CMCC_UI_STYLE
    MMIMP3_ENTRY_TYPE_E entry_type;
#endif    
    MMIAP_ALERT_INFO_T alert_info;
}MMIAP_COMMON_WIN_INFO_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get main anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetMainAnimCtrlId(void);

/*****************************************************************************/
//  Description : get left anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetLeftAnimCtrlId(void);

/*****************************************************************************/
//  Description : get right anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetRightAnimCtrlId(void);

/*****************************************************************************/
//  Description : get main anim ctrl vaild display rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMainAnimValidDisplayRect(
    GUI_RECT_T *rect_ptr
);


/*****************************************************************************/
//  Description : get main anim ctrl vaild display rect for bg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMainAnimValidDisplayRectBg(
    GUI_RECT_T *rect_ptr
);

/*****************************************************************************/
//  Description : ShuffleBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ShuffleBtnCallback(void);

/*****************************************************************************/
//  Description : EqBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_EqBtnCallback(void);

/*****************************************************************************/
//  Description : list button callback
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ListBtnCallback(void);

/*****************************************************************************/
//  Description : RepeatBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_RepeatBtnCallback(void);

/*****************************************************************************/
//  Description : PlayBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_PlayBtnCallback(void);

/*****************************************************************************/
//  Description : handle Play Pause setting
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleMp3PlayPauseButton(void);

/*****************************************************************************/
//  Description : NextBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_NextBtnCallback(void);

/*****************************************************************************/
//  Description : PrevBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_PrevBtnCallback(void);

/*****************************************************************************/
//  Description : return btn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ReturnBtnCallback(void);

/*****************************************************************************/
//  Description : VolumeBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_VolumeBtnCallback(void);

/*****************************************************************************/
//  Description : OptBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_OptBtnCallback(void);

#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
/*****************************************************************************/
//  Description : handle review control.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleMp3ReviewButton(void);

/*****************************************************************************/
//  Description : ReviewBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ReviewBtnCallback(void);

/*****************************************************************************/
//  Description : review open flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsReviewOpen(
    void
);

/*****************************************************************************/
//  Description : set review open flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReviewOpenFlag(
    BOOLEAN is_open //in
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateReviewMenuStatus(
    void
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleReviewMenu(
    void
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearReviewMenu(
    void
);

/*****************************************************************************/
//  Description : MMIMP3_GetReviewSlideWidthHeight
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetReviewSlideWidthHeight(
uint16 *width_ptr,
uint16 *height_ptr,
MMI_WIN_ID_T win_id
);

/*****************************************************************************/
//  Description : Display mp3 review button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayReviewButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
);

#endif //#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleSoftBarMsg(
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : Display mp3 volume images.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsVolumePanelOpen(
    void
);

/*****************************************************************************/
//  Description : Display mp3 volume images.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CloseVolumePanel(
    void
);

/*****************************************************************************/
//  Description : Judge whether a button is valid .
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsButtonValid(
    MMIMP3_BUTTON_TYPE_E button
);

/*****************************************************************************/
//  Description : Update mp3 button valid status.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_UpdateButtonValidStatus(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : update osd file tip
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateOsdFileTip(
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : get file tip info for osd up panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetOsdFileTipInfo(wchar   *wstr_ptr,
                                       uint16  wstr_len
                                      );

/*****************************************************************************/
//  Description : set osd panel reopen flag
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReopenOsdPanelFlag(
    BOOLEAN is_reopen
);

/*****************************************************************************/
//  Description : is need reopen osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsNeedReopenOsdPanel(void);

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DrawProgressOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
);

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DrawTimeOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
);

/*****************************************************************************/
//  Description : whether mp3 pda osd panel opened
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsOpenOsdPanel(void);

/*****************************************************************************/
//  Description : set osd panel open flag
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetOsdPanelFlag(BOOLEAN is_open);

/*****************************************************************************/
//  Description : set anim ctrl rect and display rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAnimCtrlRectDisplayRect(
    int16         move_x,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : set anim ctrl param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAnimCtrlParam(
    MMIAP_DIRECT_E which_ctrl, //in
    BOOLEAN is_update
);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : MMIAP_SetOpenMp3Entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetOpenMp3Entry(
    MMIMP3_ENTRY_TYPE_E entry_type
);

/*****************************************************************************/
//  Description : MMIAP_GetOpenMp3Entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_ENTRY_TYPE_E MMIAP_GetOpenMp3Entry(void);
#endif //CMCC_UI_STYLE

/*****************************************************************************/
//  Description : display mp3 play window button image
//  Global resource dependence :
//  Author:jassmine.meng
//  Note: MMIMP3_DisplayAllButtons
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayButton(
    BOOLEAN is_bg_player,
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayLeftKeyButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_update
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayReturnKeyButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_update
);


/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgress(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Display the end of the progress.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgressEnd(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Display current mp3 playing time.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayTimeInfo(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgressAndTimeInfo(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : display current mp3 name
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayFileName(
    BOOLEAN  is_update      //is update
);

/*****************************************************************************/
//  Description : Display mp3 play & pause button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayPlayOrPauseButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : Display mp3 shuffle button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayShuffleButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : Display mp3 repeat button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayRepeatButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author: xingdong.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayVolumeButton(
    BOOLEAN is_bg_player,
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);

/*****************************************************************************/
//  Description : Init mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_InitTitleAnim(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Pause mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StopTitleAnim(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Resume mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_PlayTitleAnim(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Clear mp3 anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearMainAnim(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : Clear mp3 anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopMainAnim(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : MMIMP3_GetProgressGrooveRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressGrooveRect(
    BOOLEAN is_bg_player,
    GUI_RECT_T *rect_ptr
);

/*****************************************************************************/
//  Description : MMIMP3_GetProgressRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressRect(
    BOOLEAN is_bg_player,
    GUI_RECT_T *rect_ptr
);


/*****************************************************************************/
//  Description : MMIMP3_GetProgressSlideWidthHeight
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressSlideWidthHeight(
    uint16 *width_ptr,
    uint16 *height_ptr,
    MMI_WIN_ID_T win_id
);

/*****************************************************************************/
//  Description : MMIMP3_GetTimeRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetTimeRect(
    BOOLEAN is_bg_player,
    GUI_RECT_T *cur_rect_ptr,
    GUI_RECT_T *total_rect_ptr
);

/*****************************************************************************/
//  Description : release anim ctrl info
//  Global resource dependence :
//  Author: paul.huang
//  Note: 释放专辑封面ANIM控件占用的内存
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_ReleaseAnimCtrlInfo(void);


/*****************************************************************************/
//  Description : init album glide ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_InitAlbumGlideCtrlId(
    BOOLEAN is_bg_player
);

/*****************************************************************************/
//  Description : get next album anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIAP_GetAnimCtrlID(
    MMIAP_DIRECT_E which_ctrl //in
);

/*****************************************************************************/
//  Description : update album anim ctrl info
//  Global resource dependence :
//  Author: xin.li
//  Note: 更新专辑图片信息
//        无专辑图片信息时默认专辑图片，非专辑显示模式时使用默认背景图片
/*****************************************************************************/
PUBLIC void MMIAP_UpdateAnimCtrlInfo(
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : close mp3 pda osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CloseMp3OsdPanel(
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : open mp3 pda osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_OpenMp3OsdPanel(
    BOOLEAN is_update
);

/*****************************************************************************/
//  Description : display alert message
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayAlertTips(
    void
);

/*****************************************************************************/
//  Description : handle mp3 alert timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_HandleMp3AlertTimer(
    uint8 timer_id
);

/*****************************************************************************/
//  Description : display bg image
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayBackgroudImg(
MMI_WIN_ID_T win_id
);

/*****************************************************************************/
//  Description : init album anim ctrl param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitAnimCtrlParam(void);

/*****************************************************************************/
//  Description : init common win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitCommonWinInfo(
    void
);

/*****************************************************************************/
//  Description : destroy common win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyCommonWinInfo(
    void
);

/*****************************************************************************/
//  Description : modify glide anim control id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ModifyAnimGlideCtrlId(
    BOOLEAN    is_prev,
    BOOLEAN    is_next
);

/*****************************************************************************/
//  Description : set pre album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetLeftAnimRect(BOOLEAN  is_update);

/*****************************************************************************/
//  Description : set main album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetMidAnimRect(BOOLEAN  is_update);

/*****************************************************************************/
//  Description : set next album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetRightAnimRect(BOOLEAN  is_update);

/*****************************************************************************/
//  Description : 显示歌手名、专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayAlbumTitleInfo(BOOLEAN is_update);

/*****************************************************************************/
//  Description : 获取指定LIST项的文本信息
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetListItemTextInfo(
    MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
    uint16        index,      //[in]  索引
    MMI_STRING_T  *main_text,  //[out] 主标题
    MMI_STRING_T  *sub_text    //[out] 副标题
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif


