/*****************************************************************************
** File Name:       watch_common.h                                           *
** Author:           fangfang.yao                                            *
** Date:             02/20/2020                                              *
** Copyright:                                                                *
** Description:    This file is used to define common part                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/07/2020           fangfang.yao          Create                         *
******************************************************************************/
#ifndef _WATCH_COMMON_H_
#define _WATCH_COMMON_H_

#include "mmk_type.h"

/*****************************************************************************/
//  Description : draw window background
//  Parameter: [In] win_id: the list control ID
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WATCHCOM_DisplayBackground( MMI_WIN_ID_T win_id );

PUBLIC void WATCHCOM_DisplayTitle( MMI_WIN_ID_T win_id, MMI_TEXT_ID_T title_id);

PUBLIC void WATCHCOM_DisplayTips( MMI_WIN_ID_T win_id, MMI_TEXT_ID_T title_id);
/*****************************************************************************/
//  Description : fill rounded rect 填充一个圆角矩形
//  Parameter: [In] win_id: 
//             [in] rect:矩形区域
//             [in] radius:圆角半径
//             [in] color:填充颜色
//             [Return] None
//  Author: deng
//  Note:
/*****************************************************************************/
PUBLIC void WATCHCOM_FillRoundedRect(MMI_WIN_ID_T win_id, GUI_RECT_T rect, uint16 radius, GUI_COLOR_T color);
/*****************************************************************************/
//  Description : 画个进度条
//  Parameter: [In] win_id: 
//             [in] rect:进度条区域
//             [in] percent:百分比
//             [in] bg_img_id:背景图片
//             [in] fg_img_id:前景图片
//             [in] hand_img_id:滑块图片 可以为空
//             [Return] None
//  Author: fangfang.yao
//  Note: 如果滑块图片高度高于进度条,窗口背景不是黑色;则会看到填充的黑色背景
/*****************************************************************************/
PUBLIC void WATCHCOM_ProgressBar(MMI_WIN_ID_T win_id, GUI_RECT_T rect, uint8 percent, MMI_IMAGE_ID_T bg_img_id, MMI_IMAGE_ID_T fg_img_id, MMI_IMAGE_ID_T hand_img_id);
/*****************************************************************************/
//  Description : 画个进度条
//  Parameter: [In] win_id: 
//             [in] rect:进度条区域
//             [in] cur_index:当前所处条序号
//             [in] total_item_num:总条数
//             [in] fg_img_id:前景图片
//             [in] hand_img_id:滑块图片 可以为空
//             [Return] None
//  Author: fangfang.yao
//  Note: 如果滑块图片高度高于进度条,窗口背景不是黑色;则会看到填充的黑色背景
/*****************************************************************************/
PUBLIC void WATCHCOM_ProgressBarByIndex(MMI_WIN_ID_T win_id, GUI_RECT_T rect, uint32 cur_index, uint32 total_item_num,MMI_IMAGE_ID_T bg_img_id, MMI_IMAGE_ID_T fg_img_id, MMI_IMAGE_ID_T hand_img_id);

PUBLIC MMI_IMAGE_ID_T WATCHCOM_GetAvaterImageId(wchar *name);
#ifdef ZTE_WATCH
PUBLIC MMI_IMAGE_ID_T WATCHCOM_GetAvaterBigImageId(wchar *name);
#endif
//zmt add start
PUBLIC uint8* WATCHCOM_GetDeviceModel();
PUBLIC uint8* WATCHCOM_GetSoftwareVersion();
//zmt add end
PUBLIC void WATCHCOM_CloseAudioOrVieo();
//zmt add start
PUBLIC uint32 GetCurrentTimeStamp(void);
PUBLIC uint8* GetCurrentTimeStampString(void);
//zmt add end
#define WINDOW_TITLE_RECT {0,0,240,30}

#define WINDOW_BACK_RECT {0,0,45,MMI_SPECIAL_TITLE_HEIGHT}

#endif

