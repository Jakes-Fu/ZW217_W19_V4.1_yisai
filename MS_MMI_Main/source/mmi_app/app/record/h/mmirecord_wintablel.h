/*************************************************************************
** File Name:      mmirecord_wintable.h                                  *
** Author:         bin.ji                                                *
** Date:           3/2/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about record          *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 3/2/2005     bin.ji             Create.                              *
*************************************************************************/

#ifndef _MMIRECORD_WINTABLE_H_
#define _MMIRECORD_WINTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmirecord_export.h"
#include "mmisrvrecord_export.h"
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
#define  RECORD_ANIMAL_TOTAL_FRAME_NUM  4
#define  MMI_RECORD_TIME_OUT  300
    /**--------------------------------------------------------------------------*
     **                         TYPE AND CONSTANT                                *
     **--------------------------------------------------------------------------*/
    typedef enum
    {
        MMIRECORD_BUTTON_VOLUME,     //音量
        MMIRECORD_BUTTON_PLAY,
        MMIRECORD_BUTTON_PAUSE,
        MMIRECORD_BUTTON_STOP,
        MMIRECORD_BUTTON_RECORD,
        MMIRECORD_BUTTON_RECORD_PAUSE,
        MMIRECORD_BUTTON_RECORD_STOP,
        MMIRECORD_BUTTON_OPTION,
        MMIRECORD_BUTTON_LIST,
        MMIRECORD_BUTTON_BACK,
        MMIRECORD_BUTTON_MAX
    }
    MMIRECORD_BUTTON_E;

    typedef enum
    {
        MMIRECORD_BUTTON_STATE_NORMAL,
        MMIRECORD_BUTTON_STATE_GRAY,
        MMIRECORD_BUTTON_STATE_HIDE,
        MMIRECORD_BUTTON_STATE_FOCUS,
        MMIRECORD_BUTTON_STATE_MAX
    } MMIRECORD_BUTTON_STATE_E;

    typedef struct
    {
        const void				*data_ptr;
        uint32					data_size;
        IMGREF_DATA_FORMAT_E    data_type;
        uint16					width;
        uint16					height;
        int16					center_x;       //horizontal coordinate of rotation center
        int16					center_y;       //vertical coordinate of rotation center
    } MMIRECORD_IMAGE_T;

    typedef struct
    {

        int16                   record_time_x;
        int16                   record_time_y;
        int16                   record_time_w;
        int16                   record_time_h;

        int16                   record_num_bg_x;//录音时间数字 背景图片
        int16                   record_num_bg_y;
        int16                   record_num_bg_w;
        int16                   record_num_bg_h;

        int16                   record_record_bg_x;
        int16                   record_record_bg_y;
        int16                   record_record_bg_w;
        int16                   record_record_bg_h;

        int16                   volume_button_x;
        int16                   volume_button_y;
        int16                   volume_button_w;
        int16                   volume_button_h;

        int16                   needle_bg_x;
        int16                   needle_bg_y;
        int16                   needle_bg_w;
        int16                   needle_bg_h;

        int16                   needle_centre_x;
        int16                   needle_centre_y;
        int16                   needle_centre_w;
        int16                   needle_centre_h;

        int16                   left_button_x;
        int16                   left_button_y;
        int16                   left_button_w;
        int16                   left_button_h;

        int16                   right_button_x;
        int16                   right_button_y;
        int16                   right_button_w;
        int16                   right_button_h;

        int16                   play_stop_button_x;
        int16                   play_stop_button_y;
        int16                   play_stop_button_w;
        int16                   play_stop_button_h;
#ifndef MMI_RECORD_MINI_SUPPORT
        int16                   name_label_x;
        int16                   name_label_y;
        int16                   name_label_w;
        int16                   name_label_h;
#endif
        int16                   progress_bg_x;
        int16                   progress_bg_y;
        int16                   progress_bg_w;
        int16                   progress_bg_h;

        int16                   progress_point_x;
        int16                   progress_point_y;
        int16                   progress_point_w;
        int16                   progress_point_h;

        int16                   play_time_label_x;
        int16                   play_time_label_y;
        int16                   play_time_label_w;
        int16                   play_time_label_h;

        int16                   play_all_time_label_x;
        int16                   play_all_time_label_y;
        int16                   play_all_time_label_w;
        int16                   play_all_time_label_h;

        int16                   play_bg_x;
        int16                   play_bg_y;
        int16                   play_bg_w;
        int16                   play_bg_h;

        int16                   need_refresh_bg_x;
        int16                   need_refresh_bg_y;
        int16                   need_refresh_bg_w;
        int16                   need_refresh_bg_h;

        int16                   needle_center_point_x;
        int16                   needle_center_point_y;

        int16                   needle_rotate_point_offset_x;
        int16                   needle_rotate_point_offset_y;

        int16                   istyle_back_button_x;
        int16                   istyle_back_button_y;
        int16                   istyle_back_button_w;
        int16                   istyle_back_button_h;

        int16                   istyle_option_button_x;
        int16                   istyle_option_button_y;
        int16                   istyle_option_button_w;
        int16                   istyle_option_button_h;
    } MMIRECORD_POSITION_T;

    typedef struct
    {
        uint16                   cur_frame;          /*当前桢索引*/
        uint16                   total_frame;        /*总桢数*/
        GUI_RECT_T               dis_rect;           /*显示区域*/
        GUI_BG_T                 bg;                 /*背景*/
        MMI_WIN_ID_T             win_id;             /*父窗口id*/
        GUI_LCD_DEV_INFO         lcd_dev;            /*lcd dev*/
    } MMIRECORD_OWNERDRAW_ANIM_T;

    typedef struct
    {
        MMI_HANDLE_T    win_handle;
        uint8  timer_id;
        uint32      max_file_time;                      //最长录音时间，实际时间可能小于等于此值
        uint32      max_file_size;                       //最大文件尺寸，实际尺寸可能小于等于此值
        MMIRECORD_WORK_FORMAT_E      record_support_type;           // 调用模块所支持的录音格式
        uint32      record_all_support_type;           // 调用模块所支持的全部录音格式
        BOOLEAN     is_insert_record;
        BOOLEAN  is_record_work_paused;
        MMIRECORD_BUTTON_STATE_E   button_validate[MMIRECORD_BUTTON_MAX];
        MMIRECORD_POSITION_T position;
        MMIRECORD_OWNERDRAW_ANIM_T record_play_anim;
        GUI_LCD_DEV_INFO lcd_dev_info;
        wchar   record_file_full_path[MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1];
        uint16  record_file_full_path_len;
        wchar   record_file_name[MMIRECORD_SRV_MAX_FILE_NAME_LEN + 1];
        uint16  record_file_name_len;
        BOOLEAN  is_begin;
    } RECORD_WIN_DATA_T;

    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/

    /*****************************************************************************/
//  Description : MMIRECORD_UpdateButtonStatusPlaying
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusPlaying(MMI_HANDLE_T win_handle);

    /*****************************************************************************/
//  Description : MMIRECORD_UpdateButtonStatusPlayPause
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusPlayPause(MMI_HANDLE_T win_handle);

    /*****************************************************************************/
//  Description : MMIRECORD_UpdateButtonStatusRecording
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusRecording(MMI_HANDLE_T win_handle);

    /*****************************************************************************/
//  Description : UpdateButtonStatusRecordPaused
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusRecordPaused(MMI_HANDLE_T win_handle);


    /*****************************************************************************/
//  Description : UpdateButtonStatusStopped
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusStopped(MMI_HANDLE_T win_handle);

    /*****************************************************************************/
//  Description : UpdateButtonStatusReady
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateButtonStatusReady(MMI_HANDLE_T win_handle);

    /*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_UpdateScreen(MMI_WIN_ID_T win_id,
                                       BOOLEAN is_full_paint);

    /*****************************************************************************/
//  Description : MMIRECORD_LoadRecordMenu
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_LoadRecordMenu(MMI_HANDLE_T parent_handle);

    /*****************************************************************************/
//  Description :MMIRECORD_OpenRecordMainWin
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC  MMI_HANDLE_T  MMIRECORD_OpenRecordMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr);

    /*****************************************************************************/
//  Description : Close Menu
//  Global resource dependence : none
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_CloseMenu(void);

    /*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_RESULT_E MMIRECORD_HandleRecordMainPlayWinMsg(
        MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E    msg_id,
        DPARAM                param
    );
#endif





















