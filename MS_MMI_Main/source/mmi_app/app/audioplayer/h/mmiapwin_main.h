/*****************************************************************************
** File Name:               mmiapwin_main.h                                  *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAPWIN_MAIN_H_
#define _MMIAPWIN_MAIN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvaud_api.h"
#include "mmicom_panel.h"
#include "mmimp3_export.h"
#include "mmiapwin_common.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//��� BUTTON ���
#define MMIMP3_BUTTON_LINE_NUM              1  //Mp3�������BUTTON������2Ϊ˫��BUTTON
#define MMIMP3_BUTTON_ONE_LINE_MAX_NUM      5  //Mp3������嵥����BUTTON�������Ŀ
#define MMIMP3_BUTTON_ONE_LINE_MAX_NUM_EXT  5  //����λ������BUTTON�������Ŀ

#define MMIMP3_GLIDE_ALBUM_NUM              3  //ר��ͼƬ���������Ŀ
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN is_web_key_down;      //web key�Ƿ񱻰���
    BOOLEAN is_tp_key_down;       //tp key�Ƿ񱻰���
    BOOLEAN is_up_side_key_down;      //up_side�Ƿ񱻰���
    BOOLEAN is_down_side_key_down;    //down_side�Ƿ񱻰���
    BOOLEAN is_up_key_down;           //up_�Ƿ񱻰���
    BOOLEAN is_down_key_down;         //down_�Ƿ񱻰���
    BOOLEAN is_left_key_down;         //left_key�Ƿ񱻰���
    BOOLEAN is_right_key_down;        //right_key�Ƿ񱻰���
} MMIAP_MAIN_WIN_KEY_INFO_T;


typedef void (*MMIAPWIN_CREATEMAINWIN)(MMI_WIN_ID_T win_id);

typedef MMI_RESULT_E (*MMIAPWIN_HANDLEWINMSG)(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

typedef void (*MMIAPWIN_APPLETRESUME)(MMI_WIN_ID_T win_id);

typedef struct
{
    MMIAPWIN_CREATEMAINWIN Func_CreateMainWin;

    MMIAPWIN_HANDLEWINMSG  Func_SubWinMsg;
    MMIAPWIN_HANDLEWINMSG Func_DefaultOpenWinMsg;

    MMIAPWIN_APPLETRESUME Func_AppletResume;
} MMIAP_WIN_FUNC_INFO_T;


typedef struct
{
    MMI_HANDLE_T win_handle;
    MMI_WIN_ID_T win_id;
    MMIAP_MAIN_WIN_KEY_INFO_T key_info;
    uint16    tp_x;
    MMIAP_WIN_FUNC_INFO_T func_info;
    BOOLEAN is_notify_update;

    BOOLEAN   is_forward;           //forward or backward by key timer
    uint8     forward_timer_id;//forward or backward timer

    BOOLEAN   is_tp_draging;
    uint8     tp_drag_timer_id;
} MMIAP_MAIN_WIN_INFO_T;

typedef struct
{
    BOOLEAN is_play_file;
    BOOLEAN is_lose_focus;
    MMIFILE_DEVICE_E file_dev; //for search music file
} MMIAP_WAIT_WIN_INFO_T;
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_Enter(BOOLEAN is_playfile);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_ResumeMainWin(void);

/*****************************************************************************/
//  Description : display mp3 play window
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
void MMIMP3_DisplayMp3PlayWin(
    BOOLEAN           is_update,//is update name and lyric
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : handle mp3 alert info display.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_HandleMp3AlertMsg(
    MMIMP3_ALERT_MSG_E      alert_type,
    MMIMP3_ALERT_CALLBACK   alert_callback
);
/*****************************************************************************/
//  Description : display mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayAlertInfo(
    void
);

/*****************************************************************************/
//  Description : clear mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearMp3AlertInfo(
    BOOLEAN exe_callback
);

/*****************************************************************************/
//  Description : stop mp3 forward or backward timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopForwardOrBackwardTimer(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StartTpDrag(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopTpDrag(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsTpDraging(void);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsSeeking(void);

#ifdef MMIAP_F_RELEASE_BG_AUDIO
/*****************************************************************************/
//  Description :judge mp3 applet is suspend by kl
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsMp3AppletSuspendByKey(void);
#endif

/*****************************************************************************/
//  Description : init win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_InitSubWin(
    MMIAP_WIN_FUNC_INFO_T *func_ptr //in
);

/*****************************************************************************/
//  Description : handle app web key win msg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_HandleAppWebKeyWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description : OpenMp3MainWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_OpenMainWin(BOOLEAN is_playfile);

/*****************************************************************************/
//  Description : stop timers
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_StopTimers(
    void
);

/*****************************************************************************/
//  Description : update play surface
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_UpdatePlaySurface(
    void
);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


