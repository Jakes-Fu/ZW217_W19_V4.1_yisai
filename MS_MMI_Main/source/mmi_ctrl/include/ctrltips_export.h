/*! @file ctrltips_export.h
@brief ���ļ�������tips�ؼ��Ķ���ӿں���������
@author Xiaoqing.Lu
@version 1.0
@date 02/20/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 2/2009         Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_Tips Tips(Tips)

-# @ref sec_tips_function
-# @ref sec_tips_create
-# @ref sec_tips_app

@section sec_tips_function �ؼ�����

Tips�Ǵ���������Ϣ����ʾ�ؼ������ܹ�����������ʾһЩ������Ϣ��ʹ�ô��ڵ�ĳЩ���Ը�Ϊ��������ʾ���û�����һ�������У�ֻ����һ��Ψһ��tips�ؼ���Tips�ؼ�����Ӧ�κ���Ϣ��

@section sec_tips_create Tips�ؼ�����

��״̬�����Ƶģ�Tips�ؼ�Ҳ����Ϊ���ڵ�һ�����ԣ����Ĵ����ǵ���WIN_TIPS�꣬��״̬��һ���������ڴ��ڵ�Table�С���Tips��Ҫ��ʾ���ݵ�ʱ�����ᷢ��MSG_CTL_TIPS_NEED_DATA��Ϣ���û�����Я��һ���Ĳ��������統ǰlistbox���������ŵȣ��û����յ������Ϣ֮������������ݡ���Tips�ؼ���ʾ��

Tips�ؼ��Ƚϼ򵥣����������κ���Ϣ��Ҳ����Ӧ�û������룬�������þ�����ʾ��ʾ��Ϣ������û�ʹ������Ҳ�Ƚϼ򵥡�

��ȻTips�Ǵ��ڵ�һ�����ԣ���Ŀǰ���������б�˵����б�ؼ������Ž��ܵĹ�ϵ�����Ҳֻ�����б�˵����б�ؼ��Ĵ�����ʹ��Tips��

���û����յ�Tips��Ϣʱ������յ�һ����Ϣ���������������ʵ���������£�
- @ref GUITIPS_DATA_T

��Ϣ��ʱ���������Я������Ϣ�пؼ���ID����ǰitem�������ţ��û����Ծ����Ƿ���ʾTips���Լ���Tips����ʾ��icon id���ַ������û�ֻ��Ҫ�޸���������Ϳ����ˣ�������Ҫ�Լ����������ڴ档

@section sec_tips_app �ؼ�Ӧ��

���磬�ڵ绰���У�ʹ��tips�ؼ���ʾ����Ϊ���߼��𡱵ĵ绰����Ϊ��123456789����
@image html tips_display.jpg
ͼ 2 38 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup tipsGrp Tips
@brief tips�ؼ�
@details tips�ؼ�
@{
*******************************************************************************/
#ifndef _CTRLTIPS_EXPORT_H_
#define _CTRLTIPS_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mmitheme_tips.h"
#include "guictrl.h"

#include "ctrlbase_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//tips handle
typedef struct _tips_obj_tag* CTRL_TIPS_HANDLE_T;

/*! @struct GUITIPS_DATA_T
@brief tips ���ݽṹ
*/
typedef struct
{
    BOOLEAN         is_need_tips;                   /*!< �Ƿ���Ҫ���� */
    uint16          index;                          /*!< ��� */
    MMI_CTRL_ID_T   ctrl_id;                        /*!< ctrl_id */
    MMI_IMAGE_ID_T  icon_id;                        /*!< icon_id */
    wchar           wstr_ptr[MMITIPS_TEXT_MAX_LEN]; /*!< ͼ���ַ� */
    uint32          wstr_len;                       /*!< �ַ����� */
} GUITIPS_DATA_T;

/*! @struct GUITIPS_TEXT_T
@brief tips �ı���Ϣ
*/
typedef struct
{
    wchar           wstr_ptr[MMITIPS_TEXT_MAX_LEN]; /*!< ͼ���ַ� */
    uint32          wstr_len;                       /*!< �ַ����� */
} GUITIPS_TEXT_T;

/*! @struct GUITIPS_INIT_DATA_T
@brief tips��ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T           both_rect;    /*!< ������Ϣ */
} GUITIPS_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief  ��tips�ؼ�
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param orig_rect_ptr        [in]    ԭʼ������Ϣ
@param tips_data_ptr        [in]    tips����
@return  ��
@note
*******************************************************************************/
PUBLIC void	CTRLTIPS_Update (
    MMI_HANDLE_T    win_handle,
    GUI_RECT_T	   *orig_rect_ptr,
    GUITIPS_DATA_T  *tips_data_ptr
);

/***************************************************************************//*!
@brief  ����tips��ʾ�ı߽����򣬷�������֮ǰ��ֵ
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param edge_rect            [in]    Ҫ���õı߽�������Ϣ
@return   ���óɹ�����������֮ǰ��ֵ��ʧ�ܣ�����"0";
@note
*******************************************************************************/
PUBLIC GUI_RECT_T CTRLTIPS_SetEdgeRect (
    MMI_HANDLE_T win_handle,
    GUI_RECT_T   edge_rect
);

/***************************************************************************//*!
@brief  ����tips�ı���Ϣ
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param tips_text            [in]    �ı���Ϣ
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsText (
    MMI_HANDLE_T    win_handle,
    GUITIPS_TEXT_T  tips_text
);

/***************************************************************************//*!
@brief    ��ȡtips�ı���Ϣ
@author xiyuan.ma
@param win_handle           [in]    ����handle
@param tips_text            [in]    �ı���Ϣ
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_GetTipsText (
    MMI_HANDLE_T    win_handle,
    GUITIPS_TEXT_T  tips_text
);

/***************************************************************************//*!
@brief  ����tips�Ƿ�ɼ�
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param is_valid             [in]    �Ƿ�ɼ�
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsValid (
    MMI_HANDLE_T    win_handle,
    BOOLEAN         is_valid
);

/***************************************************************************//*!
@brief  ��ȡtips�ؼ��Ƿ�ɼ�
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@return  tips�ؼ��ɼ�����TRUE�����ɼ�����FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_GetTipsValid (
    MMI_HANDLE_T    win_handle
);

/***************************************************************************//*!
@brief  ����tips�ı���ɫ
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param text_color           [in]    �ı���ɫ
@return  ���óɹ��������õ���ɫ������ʧ�ܷ���"0";
@note
*******************************************************************************/
PUBLIC GUI_COLOR_T	CTRLTIPS_SetTextColor (
    MMI_HANDLE_T    win_handle,
    GUI_COLOR_T     text_color
);

/***************************************************************************//*!
@brief  ����lcd��Ϣ
@author xiaoqing.lu
@param win_handle           [in]    ����handle
@param lcd_info_ptr         [in]    lcd��Ϣ
@return  ��
@note
*******************************************************************************/
PUBLIC void CTRLTIPS_SetLcdInfo (
    MMI_HANDLE_T    win_handle,
    GUI_LCD_DEV_INFO *lcd_info_ptr
);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif//_CTRLTIPS_H_
/*@}*/
