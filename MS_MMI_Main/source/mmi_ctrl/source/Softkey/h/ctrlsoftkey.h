/*****************************************************************************
** File Name:      ctrlsoftkey.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLSOFTKEY_H_
#define _CTRLSOFTKEY_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"

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
// softkey type info
#define CTRL_SOFTKEY_TYPE     (SOFTKEY_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct 
{
//    GUISOFTKEY_STYLE_E      style;
    MMI_TEXT_ID_T           text_id_arr[GUISOFTKEY_BUTTON_NUM];
    MMI_HANDLE_T            win_handle;
    MMI_HANDLE_T            handle;
}GUIBUTTONSOFTKEY_INIT_DATA_T;

// softkey init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
    MMI_TEXT_ID_T		        leftsoft_id;    /*!< ����� */
	MMI_TEXT_ID_T		        midsoft_id;     /*!< �м���� */
	MMI_TEXT_ID_T		        rightsoft_id;   /*!< ����� */  
} CTRLSOFTKEY_INIT_PARAM_T;

//softkey control
typedef struct _softkey_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    // ״̬
	BOOLEAN						tp_disable;			/*!< �Ƿ���Ӧtp��Ϣ */
	BOOLEAN						is_visible;			/*!< �Ƿ�ɼ���������ɼ�������Ӧ��Ϣ */
	BOOLEAN                     is_softkey_pressed; /*!< �Ƿ�softkey������ */

    // ���
    GUISOFTKEY_STYLE_E          style;              /*!< softkey�ķ�� */
    GUI_BUTTON_FRAME_TYPE_T     frameType;	        /*!< ��Եģʽ rectangle,3Drectangle,image,no border*/

    // ����
	GUI_SOFTKEY_BG_TYPE_T       bg_type;    	    /*!< ����ģʽ */
    MMI_IMAGE_ID_T             	bg_image;           /*!< ����ͼƬ */
    GUI_COLOR_T                	bg_color;           /*!< ������ɫ */
    uint8                       bg_alpha;           /*!< ͸����*/
  
    // �ı�
    GUI_COLOR_T                	font_color;         /*!< ������ɫ */ 
    GUI_FONT_T      		   	font;               /*!< ���� */ 
    uint16            		   	char_space;         /*!< �ַ���� */

    // button
    GUI_BUTTON_T       		   	button_arr[GUISOFTKEY_BUTTON_NUM];    /*!< �������� */

    // �Ƿ�text���õ�title������istyle����ϵͳ��softkey���ַ���ת��titleλ�ô�
    // �Ƿ�����ת������Ȼ��Ҫ�����ڴ��ڵ�һ��״̬WS_DISABLE_SOFTKEY_TO_TITLE
    BOOLEAN                     is_set_text_to_title;  

    MMI_HANDLE_T				win_handle;         /*!< ����handle */

} CTRLSOFTKEY_OBJ_T;

// softkey class for public or protected method
typedef struct _softkey_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLSOFTKEY_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : ����softkey��͸��ֵ
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_SetBgAlpha(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                uint8         bg_alpha
                                );
/*****************************************************************************/
//  Description : get softkey type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SOFTKEY_GetType(void);
/*****************************************************************************/
//  Description :����button softkey
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_Init(
                                    CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                    GUIBUTTONSOFTKEY_INIT_DATA_T *init_data_ptr
                                    );

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void GUIBUTTONOFTKEY_Update(
                       CTRLSOFTKEY_OBJ_T* softkey_ptr
                       );

/*****************************************************************************/
//  Description : �����Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetVisible(
                                     CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                     BOOLEAN         is_visible
                                     );

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GUIBUTTONSOFTKEY_SetButtonState(
                                          CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                          GUISOFTKEY_INDEX button_index,
                                          BOOLEAN         is_gray,
                                          BOOLEAN		 is_long_press
                                          );

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GUIBUTTONSOFTKEY_SetButtonGray(
                                                CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                                GUISOFTKEY_INDEX button_index,
                                                BOOLEAN         is_gray
                                                );

/*****************************************************************************/
// 	Description : ����softkey���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetTextId(
                                    CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update       // whether update
                                    );

/*****************************************************************************/
// Description : ��softkey ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void GUIBUTTONSOFTKEY_DrawBg(
                                    CTRLSOFTKEY_OBJ_T   *softkey_ptr,
                                    GUI_RECT_T      *rect_ptr
                                    );

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetButtonTextId(
                                          CTRLSOFTKEY_OBJ_T*  softkey_ptr,
                                          MMI_TEXT_ID_T   text_id,   // the left softkey id
                                          GUISOFTKEY_INDEX button_num,
                                          BOOLEAN         is_need_update       // whether update
                                          );

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetButtonTextPtr(
                                           CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                           MMI_STRING_T    str_info,   // the left softkey id
                                           GUISOFTKEY_INDEX button_num,
                                           BOOLEAN         is_need_update       // whether update
                                           );

/*****************************************************************************/
//  Description :set state
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetState(
                                    CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                    GUICTRL_STATE_T  state,
                                    BOOLEAN          is_true
                                    );

/*****************************************************************************/
//  Description :set rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetRect(
                                    CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                    const CAF_RECT_T  *rect_ptr
                                    );

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetButtonTextId(
                                                CTRLSOFTKEY_OBJ_T*  softkey_ptr,
                                                MMI_TEXT_ID_T   text_id,   // the left softkey id
                                                GUISOFTKEY_INDEX button_num,
                                                BOOLEAN         is_need_update       // whether update
                                                );
/*****************************************************************************/
//  Description : get style from text
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUISOFTKEY_BUTTONSTYLE_E CTRLBUTTONSOFTKEY_GetStyleFromText( 
                                                                  MMI_TEXT_ID_T		leftsoft_id,
                                                                  MMI_TEXT_ID_T		midsoft_id,
                                                                  MMI_TEXT_ID_T		rightsoft_id
                                                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
