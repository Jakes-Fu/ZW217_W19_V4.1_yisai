/*! @file ctrlinfo_export.h
@brief ���ļ������˳���ؼ��Ķ���ӿں���������
@author Jassmine
@version 1.0
@date 04/26/2004  
@remarks Copyright: 2003 Spreadtrum, Incoporated. All Rights Reserved.
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Ctrl Ctrl

(����)

*******************************************************************************/

/*! @addtogroup ctrlGrp Ctrl
@brief �ļ������˳���ؼ��Ķ���ӿ�
@details ctrlģ��
@{
*******************************************************************************/

#ifndef _CTRLINFO_EXPORT_H_
#define _CTRLINFO_EXPORT_H_ 

/*-------------------------------------------------------------------------
                         Include Files                                    
 --------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "mmk_type.h"

/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                          MACRO DEFINITION                                
 --------------------------------------------------------------------------*/

//��ʱ����, ������֤����ͨ��, ������Ҫ��ɾ��
//MMI_RESULT_E
/*! @def GUI_RESULT_DEFAULT
@brief Ĭ��
*/
#define GUI_RESULT_DEFAULT        MMI_RESULT_TRUE
/*! @def GUI_RESULT_PENOK
@brief ���OK
*/
#define GUI_RESULT_PENOK          (MMI_RESULT_TRUE+1)
/*! @def GUI_RESULT_CTLOK
@brief ѡ���
*/
#define GUI_RESULT_CTLOK          (MMI_RESULT_TRUE+2)
/*! @def GUI_RESULT_CTLMDL
@brief �м��
*/
#define GUI_RESULT_CTLMDL         (MMI_RESULT_TRUE+3)
/*! @def GUI_RESULT_CTLCANCEL
@brief �Ҽ�ȡ��
*/
#define GUI_RESULT_CTLCANCEL      (MMI_RESULT_TRUE+4)

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                 
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ��ȡ�ؼ��ӿ�
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param guid [in] �ؼ�id
@param object_pptr [in/out] ��ȡ�ؼ��ӿڵĽ��
@return CAF_RESULT_SUCCEEDED:��ȡ�ɹ��������Ӧ������š�
@note 
*******************************************************************************/
PUBLIC CAF_RESULT_E VTLCTRL_QueryInterface(
                                           IGUICTRL_T   *iguictrl_ptr,
                                           CAF_GUID_T   guid,
                                           void         **object_pptr
                                           );


/***************************************************************************//*!
@brief �ؼ������ü���ֵ��1
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@return �ؼ������ü���ֵ
@note 
*******************************************************************************/
PUBLIC uint32 VTLCTRL_AddRef(
                             IGUICTRL_T     *iguictrl_ptr
                             );



/***************************************************************************//*!
@brief �ؼ������ü���ֵ��1
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@return �ؼ������ü���ֵ
@note 
*******************************************************************************/
PUBLIC uint32 VTLCTRL_Release(
                              IGUICTRL_T    *iguictrl_ptr
                              );


/***************************************************************************//*!
@brief ��ȡ�ؼ�id
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC CAF_GUID_T VTLCTRL_GetCtrlGuid(
                                      IGUICTRL_T    *iguictrl_ptr
                                      );


/***************************************************************************//*!
@brief ���ÿؼ����
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param ctrl_handle [in] �ؼ����
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetCtrlHandle(
                                  IGUICTRL_T    *iguictrl_ptr,
                                  CAF_HANDLE_T  ctrl_handle
                                  );


/***************************************************************************//*!
@brief ��ÿؼ����
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@return �ؼ����
@note 
*******************************************************************************/
PUBLIC CAF_HANDLE_T VTLCTRL_GetCtrlHandle(
                                          IGUICTRL_T    *iguictrl_ptr
                                          );


/***************************************************************************//*!
@brief ���ÿؼ���״̬
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param state [in] �ؼ�״̬����
@param is_true [in] TRUE:����״̬;FALSE:���״̬
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetState(
                             IGUICTRL_T         *iguictrl_ptr,
                             GUICTRL_STATE_T    state,
                             BOOLEAN            is_true
                             );


/***************************************************************************//*!
@brief ��ȡ�ؼ���״̬
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param state [in] �ؼ�״̬����
@return TRUE:��ȡ״̬�ɹ�;FALSE:��ȡ״̬ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetState(
                                IGUICTRL_T          *iguictrl_ptr,
                                GUICTRL_STATE_T     state
                                );


/***************************************************************************//*!
@brief  ���ÿؼ��ľ�������
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param rect_ptr [in] ���������ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetRect(
                               IGUICTRL_T           *iguictrl_ptr,
                               const GUI_RECT_T     *rect_ptr
                               );


/***************************************************************************//*!
@brief ��ȡ�ؼ��ľ�������
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param rect_ptr [in/out] ���������ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetRect(
                               IGUICTRL_T   *iguictrl_ptr,
                               GUI_RECT_T   *rect_ptr
                               );


/***************************************************************************//*!
@brief ���ÿؼ���lcd��ʾ�豸
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param lcd_dev_ptr [in] ��ʾ�豸��ָ��
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetLcdDevInfo(
                                  IGUICTRL_T                *iguictrl_ptr,
                                  const GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                  );


/***************************************************************************//*!
@brief  ��ȡ�ؼ���lcd��ʾ�豸
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param lcd_dev_ptr [in/out] ��ʾ�豸��ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetLcdDevInfo(
                                     IGUICTRL_T         *iguictrl_ptr,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     );


/***************************************************************************//*!
@brief ���ÿؼ�������
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param properties [in] �ؼ�����������
@param is_true [in] TRUE:��������;FALSE:�������
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProperties(
                                     IGUICTRL_T                 *iguictrl_ptr,
                                     CAF_GUICTRL_PROPERTIES_T   properties,
                                     BOOLEAN                    is_true
                                     );


/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@return �ؼ�����������
@note 
*******************************************************************************/
PUBLIC CAF_GUICTRL_PROPERTIES_T VTLCTRL_GetProperties(
                                                      IGUICTRL_T    *iguictrl_ptr
                                                      );


/***************************************************************************//*!
@brief  ��ѯ�ؼ��Ƿ����ĳ����
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param properties [in] ��������
@return TRUE:�ؼ�����������;FALSE:�ؼ�������������
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_TestProperties(
                                      IGUICTRL_T                *iguictrl_ptr,
                                      CAF_GUICTRL_PROPERTIES_T  properties
                                      );


/***************************************************************************//*!
@brief �ػ�ؼ�
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_Redraw(
                           IGUICTRL_T   *iguictrl_ptr
                           );



/***************************************************************************//*!
@brief ���ÿؼ�active
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param is_true [in] TRUE:���ÿؼ�active;FALSE:���ÿؼ�inactive
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetActive(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 BOOLEAN        is_true
                                 );



/***************************************************************************//*!
@brief ��ѯ�ؼ��Ƿ�active
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@return [in] TRUE:�ؼ�active;FALSE:�ؼ�inactive
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_IsActive(
                                IGUICTRL_T  *iguictrl_ptr
                                );


/***************************************************************************//*!
@brief ���ÿؼ��ı߿���Ϣ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param border_ptr [in] �߿���Ϣ��ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBorder(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 GUI_BORDER_T   *border_ptr
                                 );



/***************************************************************************//*!
@brief ���ÿؼ��ı�����Ϣ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param bg_ptr [in] ������Ϣ��ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBg(
                             IGUICTRL_T     *iguictrl_ptr,
                             GUI_BG_T       *bg_ptr
                             );



/***************************************************************************//*!
@brief ���ÿؼ���������Ϣ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param font_ptr [in] ������Ϣ��ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               );




/***************************************************************************//*!
@brief ��ȡ�ؼ���������Ϣ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param font_ptr [in/out] ������Ϣ��ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               );


/***************************************************************************//*!
@brief �����Ƿ����ù�����
@author Jassmine
@param [in] iguictrl_ptr �ؼ��ӿڵ�ָ��
@param [in] is_display TRUE:��ʾ;FALSE:����ʾ
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProgress(
                                   IGUICTRL_T   *iguictrl_ptr,
                                   BOOLEAN      is_display
                                   );



/***************************************************************************//*!
@brief �����Ƿ����¼�ѭ��
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param is_circular [in] TRUE:���¼�ѭ��;FALSE:���¼���ѭ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleUpDown(
                                               IGUICTRL_T   *iguictrl_ptr,
                                               BOOLEAN      is_circular
                                               );



/***************************************************************************//*!
@brief �����Ƿ����Ҽ�ѭ��
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param is_circular [in] TRUE:���Ҽ�ѭ��;FALSE:���Ҽ�����ѭ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleLeftRight(
                                                  IGUICTRL_T    *iguictrl_ptr,
                                                  BOOLEAN       is_circular
                                                  );



/***************************************************************************//*!
@brief ���ݿؼ���Ȼ�ȡ�ؼ��߶�
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param width [in] �ؼ����
@param height_ptr [in/out] �ؼ��߶ȵ�ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetHeightByWidth(
                                        IGUICTRL_T  *iguictrl_ptr,  //in:
                                        uint16      width,          //in:
                                        uint16      *height_ptr     //in/out:
                                        );



/***************************************************************************//*!
@brief ��ȡ�ؼ��ľ�����ʾ����
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param rect_ptr [in/out] ���ε�ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetDisplayRect(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      );


/***************************************************************************//*!
@brief ���ÿؼ��ľ�����ʾ����
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param rect_ptr [in] ���ε�ָ��
@param is_update [in] �Ƿ���������
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetDisplayRect(
                                      IGUICTRL_T        *iguictrl_ptr,  //in:
                                      const GUI_RECT_T  *rect_ptr,      //in:
                                      BOOLEAN           is_update       //in:
                                      );


/***************************************************************************//*!
@brief  ���ÿؼ�ѡ������
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param rect_ptr [in] ���ε�ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectRect(
                                     IGUICTRL_T     *iguictrl_ptr,  //in:
                                     GUI_RECT_T     *rect_ptr       //in/out:
                                     );



/***************************************************************************//*!
@brief ��ȡ�ؼ�ѡ���������ֵ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param is_first_ptr [in] �Ƿ���ͷָ��
@param is_last_ptr [in] �Ƿ���βָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       *is_first_ptr,  //in/out:
                                      BOOLEAN       *is_last_ptr    //in/out:
                                      );

/***************************************************************************//*!
@brief ���ÿؼ�ѡ���������ֵ
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param is_first [in] �Ƿ���ͷ
@param is_last  [in] �Ƿ���β
@return TRUE:�ɹ�;FALSE:ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       is_first,       //in:
                                      BOOLEAN       is_last         //in:
                                      );


/***************************************************************************//*!
@brief ��ȡˮƽ�ʹ�ֱ�ľ���
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param both_rect_ptr [in] ���ε�ָ��
@return TRUE:��ȡ�ɹ�;FALSE:��ȡʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetBothRect(
                                   IGUICTRL_T       *iguictrl_ptr,  //in:
                                   GUI_BOTH_RECT_T  *both_rect_ptr  //in/out:
                                   );


/***************************************************************************//*!
@brief ����ˮƽ�ʹ�ֱ�ľ���
@author Jassmine
@param iguictrl_ptr [in] �ؼ��ӿڵ�ָ��
@param both_rect_ptr [in/out] ���ε�ָ��
@return TRUE:���óɹ�;FALSE:����ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBothRect(
                                   IGUICTRL_T               *iguictrl_ptr,  //in:
                                   const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
                                   );



/***************************************************************************//*!
@brief ������Ϣ�����ؼ����ߴ���
@author Jassmine
@param ctrl_handle [in] �ؼ��ӿڵľ��
@param code [in] ��Ϣid
@return MMI_RESULT_TRUE:�ɹ�MMI_RESULT_FALSE:ʧ��
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotify(
                                       MMI_HANDLE_T     ctrl_handle,
                                       MMI_MESSAGE_ID_E code
                                       );


/***************************************************************************//*!
@brief ������Ϣ�����ؼ����ߴ��ڣ�ͬʱ���ݲ���
@author Jassmine
@param ctrl_handle [in] �ؼ��ӿڵ�ָ��
@param code [in] ��Ϣid
@param notify_ptr [in] ������ָ��
@return MMI_RESULT_TRUE:�ɹ�MMI_RESULT_FALSE:ʧ��
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr
                                         );


/***************************************************************************//*!
@brief ������Ϣ�����ؼ����ߴ���
@author Jassmine
@param ctrl_handle [in] �ؼ��ӿڵ�ָ��
@param code [in] ��Ϣid
@return MMI_RESULT_TRUE:�ɹ�MMI_RESULT_FALSE:ʧ��
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotify(
                                       MMI_HANDLE_T         ctrl_handle,
                                       MMI_MESSAGE_ID_E     code
                                       );


/***************************************************************************//*!
@brief ������Ϣ�����ؼ����ߴ��ڣ�ͬʱ���ݲ���
@author Jassmine
@param ctrl_handle [in] �ؼ��ӿڵ�ָ��
@param code [in] ��Ϣid
@param notify_ptr [in] ������ָ��
@param size_of_notify [in] �����Ĵ�С
@return MMI_RESULT_TRUE:�ɹ�MMI_RESULT_FALSE:ʧ��
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr, 
                                         uint32             size_of_notify
                                         );

/*--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
