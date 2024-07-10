/*! @file guictrl_api.h
@brief ���ļ������˳���ؼ��Ķ���ӿں���������
@author Jassmine
@version 1.0
@date 04/26/2004    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Ctrl_api Ctrl_api

(����)

*******************************************************************************/

/*! @addtogroup ctrl_apiGrp Ctrl_api
@brief ctrl_apiģ��
@details �ļ������˳���ؼ��Ķ���ӿ�
@{
*******************************************************************************/

#ifndef _GUICTRL_API_H_
#define _GUICTRL_API_H_ 
/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ���ÿؼ���״̬
@author James.Zhang
@param ctrl_handle [in] �ؼ����
@param state [in] �ؼ�״̬����
@param is_true [in] TRUE:����״̬;FALSE:���״̬
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetState( 
                                MMI_HANDLE_T    ctrl_handle,
                                GUICTRL_STATE_T state,
                                BOOLEAN is_true
                                );


/***************************************************************************//*!
@brief ��ȡ�ؼ���״̬
@author James.Zhang
@param ctrl_handle [in] �ؼ����
@param state [in] �ؼ�״̬����
@return TRUE:��ȡ״̬�ɹ�;FALSE:��ȡ״̬ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetState( 
                                   MMI_HANDLE_T    ctrl_handle,
                                   GUICTRL_STATE_T state
                                   );



/***************************************************************************//*!
@brief ���ÿؼ��ľ���
@author James.Zhang
@param ctrl_handle [in] �ؼ����
@param rect_ptr [in] �ؼ����ε�ָ��
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetRect( 
                               MMI_HANDLE_T      ctrl_handle,
                               const GUI_RECT_T* rect_ptr
                               );



/***************************************************************************//*!
@brief ��ȡ�ؼ��ľ���
@author James.Zhang
@param ctrl_handle [in] �ؼ����
@param rect_ptr [in/out] �ؼ����ε�ָ��
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_GetRect( 
                               MMI_HANDLE_T ctrl_handle,
                               GUI_RECT_T*  rect_ptr
                               );



/***************************************************************************//*!
@brief ���ÿؼ���LCD ��ʾ�豸
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param lcd_dev_ptr [in] ��ʾ�豸��ָ��
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetLcdDevInfo(
                                     MMI_HANDLE_T           ctrl_handle,    //in:
                                     const GUI_LCD_DEV_INFO *lcd_dev_ptr    //in:
                                     );


/***************************************************************************//*!
@brief ��ȡ�ؼ���LCD ��ʾ�豸
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param lcd_dev_ptr [in/out] ��ʾ�豸��ָ��
@return TRUE:��ȡ��ʾ�豸�ɹ�;FALSE:��ȡ��ʾ�豸ʧ��
@note
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetLcdDevInfo(
                                        MMI_HANDLE_T        ctrl_handle,    //in:
                                        GUI_LCD_DEV_INFO    *lcd_dev_ptr    //in/out:
                                        );



/***************************************************************************//*!
@brief ��ȡ�ؼ���ʾ����ľ���
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param rect_ptr [in/out] �ؼ���ʾ������ε�ָ��
@return TRUE:��ȡ��ʾ����ɹ�;FALSE:��ȡ��ʾ����ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetDisplayRect(
                                         MMI_HANDLE_T   ctrl_handle,    //in:
                                         GUI_RECT_T     *rect_ptr       //in/out:
                                         );



/***************************************************************************//*!
@brief ���ÿؼ���ʾ����ľ���
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param rect_ptr [in] �ؼ���ʾ������ε�ָ��
@param is_update [in] �Ƿ���������
@return TRUE:������ʾ����ɹ�;FALSE:������ʾ����ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetDisplayRect(
                                         MMI_HANDLE_T       ctrl_handle,//in:
                                         const GUI_RECT_T   *rect_ptr,  //in/out:
                                         BOOLEAN            is_update   //in:
                                         );



/***************************************************************************//*!
@brief ��ȡ�ؼ���ֱ�ľ��κ�ˮƽ�ľ���
@author James.Zhang
@param ctrl_handle [in] �ؼ����
@param both_rect_ptr [in/out] �ؼ���ֱ���κ�ˮƽ���ε�ָ��
@return TRUE:��ȡ���γɹ�;FALSE:��ȡ����ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetBothRect(
                                      MMI_HANDLE_T     ctrl_handle,
                                      GUI_BOTH_RECT_T* both_rect_ptr
                                      );



/***************************************************************************//*!
@brief ���ÿؼ���ֱ�ľ��κ�ˮƽ�ľ���
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param both_rect_ptr [in] �ؼ���ֱ���κ�ˮƽ���ε�ָ��
@return TRUE:���þ��γɹ�;FALSE:���þ���ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetBothRect(
                                      MMI_HANDLE_T      ctrl_handle,
                                      GUI_BOTH_RECT_T   *both_rect_ptr
                                      );



/***************************************************************************//*!
@brief ���ÿؼ�������Ϣ
@author Jassmine
@param ctrl_handle [in] �ؼ����
@param bg_ptr [in] �ؼ�������Ϣ��ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetBg(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_BG_T       *bg_ptr
                                );

/*****************************************************************************/
// 	Description : Get control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_HandleEvent(
                                      MMI_HANDLE_T     ctrl_handle,
                                      CAF_MESSAGE_ID_E msg_id, 
                                      void* param_ptr
                                      );

/*****************************************************************************/
//  Description : set control state
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetBorder( 
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_BORDER_T *border_ptr
                                ) ;

/*****************************************************************************/
// 	Description : Redraw ctrl [ send MSG_CTL_PAINT to Ctrl]
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_Redraw(
                                 MMI_HANDLE_T     ctrl_handle
                                 );

/***************************************************************************//*!
@brief ���ö���ؼ�������Ч
@author James.Zhang
@param win_handle [in] ���ھ��
@param ctrl_handle_ptr [in] �ؼ������ָ��
@param ctrl_num [in] �ؼ���Ŀ
@param is_hide [in] �Ƿ����ؿؼ�
@return TRUE:���óɹ�;FALSE:����ʧ��
@note ����ؼ�������Ч
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_CtrlsHidle(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_HANDLE_T*   ctrl_handle_ptr,
                                     uint16          ctrl_num,
                                     BOOLEAN         is_hide
                                     );
/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
