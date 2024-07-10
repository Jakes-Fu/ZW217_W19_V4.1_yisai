/*****************************************************************************
** File Name:      ctrltitle_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLTITLE_EXPORT_H_
#define _CTRLTITLE_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
/*! @def GUITITLE_TEXT_MAXLEN
@brief title�ı���󳤶�
*/ 
#define GUITITLE_TEXT_MAXLEN 	    60

/*! @def GUITITLE_SUBTEXT_MAXLEN
@brief ������Ϣ��󳤶�
*/
#define GUITITLE_SUBTEXT_MAXLEN     15

/*! @typedef GUITITLE_BUTTON_INDEX_T
@brief title�ļ�ͷ��Ϣ
*/
typedef uint8 GUITITLE_BUTTON_INDEX_T;

/*! @def GUITITLE_BUTTON_LEFT
@brief title���Button
*/
#define GUITITLE_BUTTON_LEFT         0

/*! @def GUITITLE_BUTTON_RIGHT
@brief title�ұ�Button
*/
#define GUITITLE_BUTTON_RIGHT        1

/*! @def GUITITLE_BUTTON_NUM
@brief Button����
*/
#define GUITITLE_BUTTON_NUM          2

        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*!  @typedef GUITITLE_SCROLL_TYPE_E
@brief title scroll
*/
typedef enum
{
    TITLE_NOSCROLL,                 /*!< �� */
	TITLE_SCROLL_LEFT_OUT,          /*!< �����������*/  
	TITLE_NOSCROLL_BY_ELLIPSIS      /*!< ��ѭ�������򲻹�����ʾʡ�Ե� */   
}GUITITLE_SCROLL_TYPE_E;

/*! @typedef GUITITLE_BUTTON_STATE_E
@brief title��ͷ��ʾģʽ
*/
typedef enum
{
    TITLE_BUTTON_NORMAL,     /*!< ����ģʽ */
    TITLE_BUTTON_INVISIBLE,  /*!< ���ɼ�ģʽ */
	TITLE_BUTTON_GRAYED,     /*!< �һ�ģʽ */
	TITLE_BUTTON_STATE_MAX   /*!< ϵͳ���� */
}GUITITLE_BUTTON_STATE_E;

typedef enum
{
    GUITITLE_DATA_TEXT_ID,     /*!< ��ʾ�ı� */ 
	GUITITLE_DATA_TEXT_BUFFER  /*!< ��ʾ�ı� */  
}GUITITLE_TEXT_DATA_E;

typedef struct  
{
    MMI_IMAGE_ID_T       image_id;       /*!< image_id */ 
    MMI_IMAGE_ID_T       press_image_id; /*!< press_image_id */ 
    MMI_IMAGE_ID_T       gray_image_id;  /*!< gray_image_id */ 

    GUITITLE_TEXT_DATA_E text_type;      /*!< �ı����� */ 
    MMI_TEXT_ID_T        text_id;        /*!< �ı�ID */ 
    wchar                *icon_str_ptr;  /*!< �ı�ָ��*/
    uint16               str_len;        /*!< �ı����� */ 
}GUITITLE_BUTTON_INFO_T;


/*! @struct GUITITLE_INIT_DATA_T
@brief title�ؼ���ʼ����Ϣ
*/
typedef struct
{
  	MMI_TEXT_ID_T		text_id;    /*!< text_id */ 
} GUITITLE_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : ���sub text����ʾ��Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_ClearSubTextInfo(    
                                         MMI_HANDLE_T   win_handle,
                                         MMI_CTRL_ID_T    ctrl_id
                                         );

/*****************************************************************************/
// Description : ��̬����״̬���ؼ�
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_CreateCtrlDyn(
                                      MMI_HANDLE_T win_handle,
                                      MMI_CTRL_ID_T ctrl_id, 
                                      MMI_TEXT_ID_T text_id
                                      );

/*****************************************************************************/
// Description : ���title��button��״̬
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetButtonStatus(    
                                                        MMI_HANDLE_T   win_handle,
                                                        MMI_CTRL_ID_T    ctrl_id,
                                                        GUITITLE_BUTTON_INDEX_T   index
                                                        );

/*****************************************************************************/
// Description : ���title��arrow״̬
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetRollArrowStatus(    
                                                          MMI_HANDLE_T   win_handle,
                                                          MMI_CTRL_ID_T    ctrl_id,
                                                          GUITITLE_BUTTON_INDEX_T   index
                                                          );

/*****************************************************************************/
// Description : set the title background
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetBackground(
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T     ctrl_id,
                                      MMI_IMAGE_ID_T bg_image,
                                      GUI_COLOR_T    bg_color
                                      );

/*****************************************************************************/
// Description : set title font color
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_SetFontColor(
                                  MMI_HANDLE_T      win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  GUI_COLOR_T       font_color
                                  );

/*****************************************************************************/
// Description : ����title���Ƿ���arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrow(    
                                     MMI_HANDLE_T   win_handle,
                                     MMI_CTRL_ID_T    ctrl_id,
                                     BOOLEAN        is_need_button  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                     );

/*****************************************************************************/
// Description : ����title���Ƿ���arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrowStatus(    
                                           MMI_HANDLE_T   win_handle,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUITITLE_BUTTON_INDEX_T   index,
                                           GUITITLE_BUTTON_STATE_E  status
                                           );

/*****************************************************************************/
// Description : ����title���Ƿ���button
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetNeedButton(    
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN        is_need_button  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                      );

/*****************************************************************************/
// Description : ����title��button��״̬
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonStatus(    
                                        MMI_HANDLE_T   win_handle,
                                        MMI_CTRL_ID_T    ctrl_id,
                                        GUITITLE_BUTTON_INDEX_T   index,
                                        GUITITLE_BUTTON_STATE_E  status
                                        );

/*****************************************************************************/
// Description : ����title���½��б�������Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubText(    
                                   MMI_HANDLE_T     win_handle,
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_need_sub_text,   //�Ƿ���Ҫ�б�������Ϣ
                                   const wchar      *wstr_ptr,        //[IN] string pointer
                                   uint16            wstr_len,        //[IN] string length
                                   BOOLEAN          is_need_update        //�Ƿ�������ʾ
                                   );

/*****************************************************************************/
// Description : update the window title
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_Update(
                            MMI_HANDLE_T    win_handle,
                            MMI_CTRL_ID_T    ctrl_id
                            );

/*****************************************************************************/
// Description : ����title��ߵ�Сͼ��
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetIcon(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                MMI_IMAGE_ID_T  icon_id
                                );

/*****************************************************************************/
// Description : ����title����
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRect(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_RECT_T        rect
                                );

/*****************************************************************************/
// Description : ����title���½��б�������Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParam(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUI_RECT_T      *text_rect_ptr,   // ���ַ���������
                                        GUI_FONT_T      *font_ptr,        // ���ַ�������
                                        GUI_ALIGN_E     *align_ptr        // align type
                                        );
                                        
/*****************************************************************************/
// Description : set the window title string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  );

/*****************************************************************************/
// Description : set the window title align type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextAlignType(
                                         MMI_HANDLE_T                win_handle,
                                         MMI_CTRL_ID_T            ctrl_id,
                                         GUI_ALIGN_E                align
                                         );

/*****************************************************************************/
// Description : set the window title scroll type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextScrollType(
                                          MMI_HANDLE_T                win_handle,
                                          MMI_CTRL_ID_T            ctrl_id,
                                          GUITITLE_SCROLL_TYPE_E    scroll_type
                                          );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
