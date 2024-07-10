/*! @file guiprgbox.h
@brief ���ļ�������prgbox�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 01/10/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 01/2007      Jassmine              Creat
*******************************************************************************/

/*! @page page_PrgBox �������ͽ�����(PrgBox)

-# @ref sec_prgbox_function
-# @ref sec_prgbox_create
-# @ref sec_prgbox_app

@section sec_prgbox_function �ؼ�����

- Prgbox���������ͽ������ؼ�������prg��progress����д������ʾ�ı���ʱ�������ʾ����һҳ������Ļ������ʾ���ݣ���ͨ��ʹ�ù������ؼ������û�������ҳû����ʾ����������ʹ�����¼��л�����ҳ����һ�С�
- ���⣬�������ļ�ʱ������ʹ�ý�������ʾ�ļ�����Ľ��ȣ��������������ļ���web���صȡ�
- �������ͽ������ؼ���Ϊ������ʹ��ʱ��һ����Ƕ��List��Menu��Dropdownlist�ؼ��������ؼ��У��û�������Ĺ�������ʹ�á���Ȼ���û�Ҳ���Ե���ʹ�á�
- ��Ϊ�������ؼ���һ�㱻����Ӧ���ڴ����У������ʾ�����ļ��Ľ��ȡ�
- �������ȿ����Ǻ���ģ�Ҳ����������ģ����ܹ���Ӧ������Ϣ�����ʱ��Ϊ���������������¼�ͷ�����������Ϊ���Ҽ�ͷ�������磬�����ʵ�����¼�ͷ��ʱ�򣬿����л��б����һ������һ����¼�������ʵ���ڹ����������л������������ʱ�������л�����ҳ�������Դ����϶����顣

@section sec_prgbox_create �ؼ��Ĵ���

�������ͽ������ؼ������¼��Prg��һ�����������Ƕ�������ؼ��У�����Textbox��Editbox��menu��list�ȣ�����Ҫ�û��Լ�����������û���Ҫ����ʹ�øÿؼ�������ʱ��Ϊ��̬�����;�̬�������䴴����ʽ�������ؼ���ͬ��
����֮�󣬸�������������صĲ����Ϳ����ˣ�����ؼ��Ľӿ������˵���٣�Ҳ�Ƚϼ򵥡�

@section sec_prgbox_app �ؼ�Ӧ��
��ͼΪ�������ؼ���ʹ�ã�
@image html prgbox_display.jpg
ͼ 2 42 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup groupGrp PrgBox
@brief groupģ��
@details groupģ��
@{
*******************************************************************************/

#ifndef _GUIPRGBOX_H_
#define _GUIPRGBOX_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmitheme_prgbox.h"
#include "guictrl.h"

#include "ctrlprogressbar_export.h"
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
 typedef CTRLPRGBOX_NOTIFY_T GUIPRGBOX_NOTIFY_T;
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUIPRGBOX_SetPosByCtrl              CTRLPROGRESSBAR_SetPos
#define GUIPRGBOX_UpdateByCtrl              CTRLPROGRESSBAR_Update
#define GUIPRGBOX_SetParamByCtrl            CTRLPROGRESSBAR_SetParam
#define GUIPRGBOX_SetBgColor                CTRLPROGRESSBAR_SetBgColor
#define GUIPRGBOX_SetFobidThumb             CTRLPROGRESSBAR_SetFobidThumb
#define GUIPRGBOX_SetNeedSlideBlockByCtrl   CTRLPROGRESSBAR_SetNeedSlideBlock
#define GUIPRGBOX_SetVerGrooveBgImg         CTRLPROGRESSBAR_SetVerGrooveBgImg
#define GUIPRGBOX_SetScrollBarImg           CTRLPROGRESSBAR_SetScrollBarImg
#define GUIPRGBOX_SetRect                   CTRLPROGRESSBAR_SetRect

/*****************************************************************************/
//  Description : set position by pointer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetPosByPtr (
    BOOLEAN      is_update,      //is update
    uint32       cur_item_index,
    uint32       first_item_index,
    IGUICTRL_T   *prgbox_ctrl_ptr
);

/*****************************************************************************/
// Description : this function only for progress bar
// Global resource dependence : none
// Author: allen
// Note: 2005.01.11
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_GetRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      *rect
);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author: allen
// Note: 2005.02.02
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_ResizeRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      new_rect  // the new total rectangle
);

/***************************************************************************//*!
@brief ���ݿؼ���Ϣˢ��
@author Jassmine
@param  prgbox_ctrl_ptr        [in] �������ؼ�id
@return ��
@note 
*******************************************************************************/
void GUIPRGBOX_UpdateByPtr(
                           IGUICTRL_T *prgbox_ctrl_ptr
                           );

 /***************************************************************************//*!
@brief ���ݿؼ�id����item������ÿҳitem����
@author guanyong.zhang
@param      prgbox_ctrl_ptr      [in]   �ؼ�������Ϣ
@param      total_item_count     [in]   item������                 
@param      items_per_page       [in]   ÿҳitem����
@return ��
@note 
*******************************************************************************/
PUBLIC void GUIPRGBOX_SetParamByPtr(
                                    IGUICTRL_T          *prgbox_ctrl_ptr,
                                    uint32              total_item_count,
                                    uint32              items_per_page
                                    );

 /***************************************************************************//*!
@brief ���ݿؼ�������Ϣ�����Ƿ���ѭ��������
@author guanyong.zhang
@param      prgbox_ctrl_ptr     [in]    �ؼ�������Ϣ
@param      is_loop_scroll      [in]    �Ƿ�����ѭ��������               
@return ��
@note 
*******************************************************************************/

PUBLIC void GUIPRGBOX_SetLoopScrollByPtr(
                                         IGUICTRL_T       *prgbox_ctrl_ptr,
                                         BOOLEAN          is_loop_scroll
                                         );

/***************************************************************************//*!
@brief ����������Ϣ�����Ƿ���Ҫ����
@author xiaoqing.lu
@param      prgbox_ctrl_ptr     [in]    �ؼ�������Ϣ
@param      is_true             [in]    �Ƿ���Ҫ���û���
@return ��������ǰ������
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedSlideBlock(
                                           IGUICTRL_T           *prgbox_ctrl_ptr,
                                           BOOLEAN              is_true
                                           );

/***************************************************************************//*!
@brief ���ݿؼ�������Ϣ�ж��Ƿ���Ҫ���ñ���
@author xiaoqing.lu
@param      prgbox_ctrl_ptr [in]    �ؼ�������Ϣ
@param      is_true         [in]    �Ƿ���Ҫ���ñ���
@return ��������ǰ������
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedBg(
                                   IGUICTRL_T           *prgbox_ctrl_ptr,
                                   BOOLEAN              is_true
                                   );

/***************************************************************************//*!
@brief ���ݿؼ�������Ϣ���ñ�����ɫ
@author guanyong.zhang
@param      prgbox_ctrl_ptr [in]    �ؼ�������Ϣ
@param      bg_color        [in]    ������ɫ
@return ��
@note 
*******************************************************************************/
PUBLIC void GUIPRGBOX_SetBgColorByPtr(
                                      IGUICTRL_T        *prgbox_ctrl_ptr,
                                      GUI_COLOR_T       bg_color
                                      );

/***************************************************************************//*!
@brief ��ȡ��ǰ��������λ������Ӧ��ĸ��unicode
@author 
@param      prgbox_ctrl_ptr����IN]�������Ŀؼ�ָ��
@param      item_pos���������ĵ�ǰλ��
@return ����item_pos����Ӧ����ĸ��unicode
@note 
*******************************************************************************/
PUBLIC wchar  GUIPRGBOX_GetWcharByPos (
                                       IGUICTRL_T * prgbox_ctrl_ptr,          
                                       uint16  item_pos
                                       );

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetBothRectByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    GUI_BOTH_RECT_T     *both_rect_ptr
);

/*****************************************************************************/
//  Description : �����Ƿ���Ҫ����
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:��������ǰ������
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetFobidThumbByPtr(
										 IGUICTRL_T *prgbox_ctrl_ptr
										 );

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
/*@}*/
