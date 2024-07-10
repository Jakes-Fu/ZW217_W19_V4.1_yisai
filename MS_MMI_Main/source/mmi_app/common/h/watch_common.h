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
//  Description : fill rounded rect ���һ��Բ�Ǿ���
//  Parameter: [In] win_id: 
//             [in] rect:��������
//             [in] radius:Բ�ǰ뾶
//             [in] color:�����ɫ
//             [Return] None
//  Author: deng
//  Note:
/*****************************************************************************/
PUBLIC void WATCHCOM_FillRoundedRect(MMI_WIN_ID_T win_id, GUI_RECT_T rect, uint16 radius, GUI_COLOR_T color);
/*****************************************************************************/
//  Description : ����������
//  Parameter: [In] win_id: 
//             [in] rect:����������
//             [in] percent:�ٷֱ�
//             [in] bg_img_id:����ͼƬ
//             [in] fg_img_id:ǰ��ͼƬ
//             [in] hand_img_id:����ͼƬ ����Ϊ��
//             [Return] None
//  Author: fangfang.yao
//  Note: �������ͼƬ�߶ȸ��ڽ�����,���ڱ������Ǻ�ɫ;��ῴ�����ĺ�ɫ����
/*****************************************************************************/
PUBLIC void WATCHCOM_ProgressBar(MMI_WIN_ID_T win_id, GUI_RECT_T rect, uint8 percent, MMI_IMAGE_ID_T bg_img_id, MMI_IMAGE_ID_T fg_img_id, MMI_IMAGE_ID_T hand_img_id);
/*****************************************************************************/
//  Description : ����������
//  Parameter: [In] win_id: 
//             [in] rect:����������
//             [in] cur_index:��ǰ���������
//             [in] total_item_num:������
//             [in] fg_img_id:ǰ��ͼƬ
//             [in] hand_img_id:����ͼƬ ����Ϊ��
//             [Return] None
//  Author: fangfang.yao
//  Note: �������ͼƬ�߶ȸ��ڽ�����,���ڱ������Ǻ�ɫ;��ῴ�����ĺ�ɫ����
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

