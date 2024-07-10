/*****************************************************************************
** File Name:      ctrlsoftkey.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmk_window_internal.h"
#include "ctrlsoftkey.h"
#include "ctrlsoftkey_export.h"
#include "guilcd.h"
#include "mmi_theme.h"
#include "mmi_textfun.h"
#include "guistring.h"
#include "mmitheme_softkey.h"
#include "mmk_tp.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUISOFTKEY_EDGE                 DP2PX_VALUE(4)
#define GUISOFTKEY_GRAY_TEXT_ALPHA      89 //�һ�ʱ����͸��ֵ
#define GUISOFTKEY_GRAY_IMG_ALPHA       189//ͼƬ�û�
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init softkey class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlInitVtbl(
    CTRLSOFTKEY_VTBL_T        *softkey_vtbl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SoftkeyCtrlPackInitParam (
    GUISOFTKEY_INIT_DATA_T     *softkey_init_ptr,   //in
    CTRLSOFTKEY_INIT_PARAM_T   *softkey_param_ptr   //out
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
//  Description : set softkey rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlSetRect(
                              CTRLBASE_OBJ_T        *ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              );

/*****************************************************************************/
//  Description : is button style
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsButtonStyle( 
                            GUISOFTKEY_STYLE_E   style
                            );

/*****************************************************************************/
// Description : draw all buttons
// Global resource dependence : 
// Author: hua.fang
// Note:
/*****************************************************************************/
LOCAL void DrawAllButtons(
                          CTRLSOFTKEY_OBJ_T*    softkey_ptr
                          );

/*****************************************************************************/
//     Description : �õ�button�Ŀ��
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL uint16 GetButtonWidth(
                            CTRLSOFTKEY_OBJ_T* softkey_ptr,
                            GUISOFTKEY_INDEX button_num
                            );

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISOFTKEY_Construct(
                                   CTRLBASE_OBJ_T	        *iguictrl_ptr,
                                   CTRLSOFTKEY_INIT_PARAM_T *softkey_param_ptr
                                   );

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISOFTKEY_Destruct(
                                 CTRLBASE_OBJ_T		*iguictrl_ptr
                                 );

/*****************************************************************************/
//  Description : init button
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
LOCAL void GUISOFTKEY_Init( 
                          CTRLBASE_OBJ_T		*ctrl_ptr,
						  MMI_TEXT_ID_T		leftsoft_id,
						  MMI_TEXT_ID_T		midsoft_id,
						  MMI_TEXT_ID_T		rightsoft_id
                          );

/*****************************************************************************/
//  Description : handle msg of button
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E GUISOFTKEY_HandleMsg( 
                                       CTRLBASE_OBJ_T        *ctrl_ptr,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       );

/*****************************************************************************/
//  Description : reset softkey pressed state
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ResetSoftkey(
                        CTRLSOFTKEY_OBJ_T   *softkey_ptr
                        );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL void SetWinSKBPressed(
                            CTRLSOFTKEY_OBJ_T	*softkey_ptr,
                            BOOLEAN         is_softkey_pressed
                            );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinSKBPressed(
                               CTRLSOFTKEY_OBJ_T	*softkey_ptr
                               );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetWinSKBButtonPressed(
                                     CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                     GUISOFTKEY_INDEX   button_index,
                                     BOOLEAN			is_pressed
                                     );

/*****************************************************************************/
//     Description : handle press in softkeybar  
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSoftkeyBarTPMsg(
                                         CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                         MMI_MESSAGE_ID_E		msg_id,
                                         GUI_POINT_T			point
                                         );

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawSoftkey(
                       CTRLSOFTKEY_OBJ_T* softkey_ptr
                       );

/*****************************************************************************/
//  Description : Get softkey ptr
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLSOFTKEY_OBJ_T* GetSoftkeyPtr(
                                   MMI_HANDLE_T    win_handle,
                                   MMI_HANDLE_T    ctrl_handle //control id
                                   );

/*****************************************************************************/
// Description : ��softkey ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawSoftkeyBg(
                         CTRLSOFTKEY_OBJ_T* softkey_ptr
                         );

/*****************************************************************************/
// Description : ��button text
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonText(
                          CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                          GUISOFTKEY_INDEX  button_num
                          );

/*****************************************************************************/
// Description : ��button ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonContent(
                             CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                             GUISOFTKEY_INDEX  button_num
                             );

/*****************************************************************************/
// Description : ����button��ͼƬ��ʾ����ÿ������Ϊ1/3��������Ŀ��
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSoftkeyButtonDispRect(
                                          CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                                          GUISOFTKEY_INDEX  button_num
                                          );

/*****************************************************************************/
// Description : ��button
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonBg(
                        CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                        GUISOFTKEY_INDEX  button_num
                        );

/*****************************************************************************/
// Description : get text font
// Global resource dependence : 
// Author: hua.fang
// Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetTextFont(
                             CTRLSOFTKEY_OBJ_T  *softkey_ptr
                             );

/*****************************************************************************/
//  Description : get softkey height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SoftkeyCtrlOnStateChanged(
                               CTRLBASE_OBJ_T       *ctrl_ptr,
                               GUICTRL_STATE_T  state,
                               BOOLEAN          is_true
                               );

/*****************************************************************************/
//  Description : get softkey height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlGetHeightByWidth(
                                          CTRLBASE_OBJ_T    *ctrl_ptr,  //in:
                                          uint16        width,      //in:
                                          uint16        *height_ptr //in/out:
                                          );

/*****************************************************************************/
//  Description : get softkey display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlGetDisplayRect(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,  //in:
                                        GUI_RECT_T  *rect_ptr   //in/out:
                                        );

/*****************************************************************************/
//  Description : set softkey display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlSetDisplayRect(
                                        CTRLBASE_OBJ_T          *ctrl_ptr,  //in:
                                        const GUI_RECT_T    *rect_ptr,  //in:
                                        BOOLEAN             is_update   //in:
                                        );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get softkey type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SOFTKEY_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "softkey",                                         // control name
                       sizeof (CTRLSOFTKEY_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) GUISOFTKEY_Construct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) GUISOFTKEY_Destruct,      // control destructor function
                       sizeof (CTRLSOFTKEY_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) SoftkeyCtrlPackInitParam,     // convert init param
                       sizeof (CTRLSOFTKEY_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) SoftkeyCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init softkey class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlInitVtbl (
    CTRLSOFTKEY_VTBL_T        *softkey_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) softkey_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = GUISOFTKEY_HandleMsg;

    // ���½ӿ�����ʵ����Ƕform
    base_ctrl_vtbl_ptr->OnStateChanged = SoftkeyCtrlOnStateChanged;
    base_ctrl_vtbl_ptr->SetRect  = SoftkeyCtrlSetRect;
    base_ctrl_vtbl_ptr->GetHeightByWidth = SoftkeyCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->GetDisplayRect = SoftkeyCtrlGetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = SoftkeyCtrlSetDisplayRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// �÷������ڽ��ؼ��Ķ����ʼ�������ṹת��Ϊ�ڲ��ĳ�ʼ�������ṹ
// ���ڿؼ��м̳й�ϵ���ؼ���ʼ��ʱ�ִ��в����������ڲ��ĳ�ʼ������Ҳ�����м̳й�ϵ
/*****************************************************************************/
LOCAL void SoftkeyCtrlPackInitParam (
    GUISOFTKEY_INIT_DATA_T     *softkey_init_ptr,   //in
    CTRLSOFTKEY_INIT_PARAM_T   *softkey_param_ptr   //out
)
{
    if (PNULL == softkey_init_ptr || PNULL == softkey_param_ptr)
    {
        return;
    }

    softkey_param_ptr->leftsoft_id  = softkey_init_ptr->leftsoft_id;
    softkey_param_ptr->midsoft_id   = softkey_init_ptr->midsoft_id;
    softkey_param_ptr->rightsoft_id = softkey_init_ptr->rightsoft_id;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_SOFTKEY_TYPE);
}

/*****************************************************************************/
//  Description : ����softkey������button��rect
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
LOCAL void CalSoftkeyRect(
                          CTRLSOFTKEY_OBJ_T    *softkey_ptr
                          )
{
    uint16  left_width   = 0;   //���button�Ŀ��
    uint16  middle_width = 0;   //�м�button�Ŀ��
    uint16  right_width  = 0;   //�ұ�button�Ŀ��
    uint16  total_width  = 0;   
    CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

	if (PNULL == ctrl_ptr)
	{
        return;
	}

    total_width = ctrl_ptr->rect.right - ctrl_ptr->rect.left + 1;
    //get softkey text width
    middle_width = GetButtonWidth(softkey_ptr, MIDDLE_BUTTON);
    
    // �м���Ŀ���������Ƶ�,������������ȵ�1/3��
    if (middle_width > total_width/GUISOFTKEY_BUTTON_NUM)
    {
        middle_width = total_width/GUISOFTKEY_BUTTON_NUM;
    }
    
    right_width = left_width = (total_width - middle_width) / 2;

    // ��
    softkey_ptr->button_arr[LEFT_BUTTON].rect = GUI_CreateRect(
				(uint16)(ctrl_ptr->rect.left),
				ctrl_ptr->rect.top,
				(uint16)(ctrl_ptr->rect.left + left_width - 1),
				ctrl_ptr->rect.bottom
				);

    // ��
    softkey_ptr->button_arr[MIDDLE_BUTTON].rect = GUI_CreateRect(
            (uint16)((ctrl_ptr->rect.left + ctrl_ptr->rect.right - middle_width + 1) / 2),
            ctrl_ptr->rect.top,
            (uint16)((ctrl_ptr->rect.left + ctrl_ptr->rect.right + middle_width + 1) / 2),
            ctrl_ptr->rect.bottom
            );

    // ��
    softkey_ptr->button_arr[RIGHT_BUTTON].rect = GUI_CreateRect(
				(uint16)(ctrl_ptr->rect.right - right_width + 1),
				ctrl_ptr->rect.top,
				(uint16)(ctrl_ptr->rect.right),
				ctrl_ptr->rect.bottom
				);
}

/*****************************************************************************/
// Description : draw all buttons
// Global resource dependence : 
// Author: hua.fang
// Note:
/*****************************************************************************/
LOCAL void DrawAllButtons(
                          CTRLSOFTKEY_OBJ_T*    softkey_ptr
                          )
{
    // ��ʾ����
    DrawButtonBg(softkey_ptr, MIDDLE_BUTTON);
    DrawButtonBg(softkey_ptr, LEFT_BUTTON);
    DrawButtonBg(softkey_ptr, RIGHT_BUTTON);

    // ��ʾ����
    DrawButtonContent(softkey_ptr, MIDDLE_BUTTON);
    DrawButtonContent(softkey_ptr, LEFT_BUTTON);
    DrawButtonContent(softkey_ptr, RIGHT_BUTTON);
}


/*****************************************************************************/
// Description : ��button
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonBg(
                        CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                        GUISOFTKEY_INDEX  button_num
                        )
{
    GUI_BUTTON_T        *button_ptr = &softkey_ptr->button_arr[button_num];
    GUI_RECT_T          disp_rect = GetSoftkeyButtonDispRect(softkey_ptr, button_num);
    GUI_RECT_T          image_rect = disp_rect;
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *    ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

    if (disp_rect.left == disp_rect.right
		|| PNULL == ctrl_ptr)
    {
        return;
    }

    image_rect.left -= ctrl_ptr->rect.left;
    image_rect.right -= ctrl_ptr->rect.left;
    image_rect.top -= ctrl_ptr->rect.top;
    image_rect.bottom -= ctrl_ptr->rect.top;

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = ctrl_ptr->rect;
    bg_display.display_rect   = disp_rect;
    bg_display.img_rect       = disp_rect;
    bg_display.ctrl_handle    = ctrl_ptr->handle;
    bg_display.win_handle     = softkey_ptr->win_handle;

    ctrl_ptr->bg = MMITHEME_GetCommonBg();

    switch(softkey_ptr->bg_type) // �ػ�ÿ�����ı�����Ŀ���Ƿ�ֹˢ��ʱ��Ӱ
    {
    case GUI_SOFTKEY_BG_NULL:
        GUI_DisplayBg(&ctrl_ptr->bg,
                &bg_display,
                &ctrl_ptr->lcd_dev_info);
        break;

    case GUI_SOFTKEY_BG_IMAGE_ONLY:
        //display bg
        GUI_DisplayBg(&ctrl_ptr->bg,
                &bg_display,
                &ctrl_ptr->lcd_dev_info);

        if (button_ptr->is_pressed)
        {
            GUIRES_DisplayImg(PNULL, &disp_rect, &image_rect, softkey_ptr->win_handle, button_ptr->icon_down_id, &ctrl_ptr->lcd_dev_info);
        }
        else
        {
            //display softkey up
            GUIRES_DisplayImg(PNULL, &disp_rect, &image_rect, softkey_ptr->win_handle, button_ptr->icon_up_id, &ctrl_ptr->lcd_dev_info);
        }
        break;
        
    case GUI_SOFTKEY_BG_COLOR_ONLY:
        LCD_FillArgbRect(&ctrl_ptr->lcd_dev_info, disp_rect, softkey_ptr->bg_color, softkey_ptr->bg_alpha);
//        LCD_FillRect(&ctrl_ptr->lcd_dev_info, disp_rect, softkey_ptr->bg_color);
        break;
        
    default:
        //SCI_TRACE_LOW:"GUISOFTKEY: softkey_ptr->bg_type error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_528_112_2_18_3_21_26_310,(uint8*)"", softkey_ptr->bg_type);
        break;
    }
}
/*****************************************************************************/
//     Description : �õ�button�Ŀ��
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL uint16 GetButtonWidth(
                            CTRLSOFTKEY_OBJ_T* softkey_ptr,
                            GUISOFTKEY_INDEX button_num
                            )
{
    uint16          piexl_num = 0;
	MMI_STRING_T	text = {0};
    GUI_BUTTON_T*   button_ptr = &softkey_ptr->button_arr[button_num];

    switch (softkey_ptr->frameType)
    {
    case BUTTON_FRAME_RECT:    //����
        piexl_num = DP2PX_VALUE(2);
        break;

    case BUTTON_FRAME_3D:      //3D����
        piexl_num = DP2PX_VALUE(2);
        break;

    case BUTTON_FRAME_IMAGE:   //ͼ�α߿�
        break;

    default:
        break;
    }

    switch(button_ptr->content.data_type)
    {
    case GUISK_DATA_TEXT_ID:
    case GUISK_DATA_TEXT_BUFFER:
        if (GUISK_DATA_TEXT_ID == button_ptr->content.data_type)
        {
            MMITHEME_GetResText(button_ptr->content.data_u.text_id, softkey_ptr->win_handle, &text);
        }
        else if (GUISK_DATA_TEXT_BUFFER == button_ptr->content.data_type)
        {
            text.wstr_len = button_ptr->content.data_u.text_buffer.wstr_len;
            text.wstr_ptr = button_ptr->content.data_u.text_buffer.wstr_ptr;
        }

        if (0 == text.wstr_len || PNULL == text.wstr_ptr)
        {
            piexl_num = 0;
        }
        else
        {
            piexl_num = GUI_CalculateStringPiexlNum(
                text.wstr_ptr,    //[IN] Stringָ��
                text.wstr_len,    //[IN] String����(Byte����)
                GetTextFont(softkey_ptr),       //[IN] String������
                0  //[IN] �ּ��
                );
        }
        break;
    case GUISK_DATA_IMAGE_ID:
        {
            GUIRES_GetImgWidthHeight(&piexl_num, PNULL, button_ptr->content.data_u.image_id, softkey_ptr->win_handle);            
        }
        break;
    default:
        break;
    }

    return piexl_num;
}

/*****************************************************************************/
// Description : get text font
// Global resource dependence : 
// Author: hua.fang
// Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetTextFont(
                             CTRLSOFTKEY_OBJ_T  *softkey_ptr
                             )
{
    GUI_FONT_T font = 0;

    if (0 != softkey_ptr->font)
    {
        font = softkey_ptr->font;
    }
    else
    {
        font = MMITHEME_GetSoftKeyTextFont();
    }

    return font;
}

/*****************************************************************************/
// Description : ����button��ͼƬ��ʾ����ÿ������Ϊ1/3��������Ŀ��
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSoftkeyButtonDispRect(
                                          CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                                          GUISOFTKEY_INDEX  button_num
                                          )
{
    uint16            total_width = 0;
    GUI_RECT_T        rect = {0};
    GUI_RECT_T        mid_rect = {0};
    CTRLBASE_OBJ_T *  ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;
	
	if (PNULL == ctrl_ptr)
	{
		return rect;
	}
	total_width = (ctrl_ptr->rect.right - ctrl_ptr->rect.left + 1);

    switch (button_num)
    {
    case LEFT_BUTTON:
        mid_rect = GetSoftkeyButtonDispRect(softkey_ptr, MIDDLE_BUTTON);
        rect.left = ctrl_ptr->rect.left;
        rect.right = mid_rect.left - 1;
        rect.top = ctrl_ptr->rect.top;
        rect.bottom = ctrl_ptr->rect.bottom;
        break;

    case MIDDLE_BUTTON:
        rect.left = rect.left + total_width/GUISOFTKEY_BUTTON_NUM;
        rect.right = rect.left + total_width/GUISOFTKEY_BUTTON_NUM - 1;
        rect.top = ctrl_ptr->rect.top;
        rect.bottom = ctrl_ptr->rect.bottom;
        
        break;

    case RIGHT_BUTTON:
        mid_rect = GetSoftkeyButtonDispRect(softkey_ptr, MIDDLE_BUTTON);
        rect.left = mid_rect.right + 1;
        rect.right = ctrl_ptr->rect.right;
        rect.top = ctrl_ptr->rect.top;
        rect.bottom = ctrl_ptr->rect.bottom;
        break;

    default:
        break;
    }

    return rect;
}

/*****************************************************************************/
// Description : ��button text
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonText(
                          CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                          GUISOFTKEY_INDEX  button_num
                          )
{
    GUI_RECT_T      rect = softkey_ptr->button_arr[button_num].rect;
    GUI_RECT_T      part_rect = {0};
    MMI_STRING_T	text = {0};
    GUI_BUTTON_T    *button_ptr = &softkey_ptr->button_arr[button_num];
    GUISTR_STYLE_T  str_style = {0};/*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    GUI_ALIGN_E     align_num = ALIGN_LEFT;
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;
    MMI_SOFTKEY_STYLE_T softkey_style = {0};

	if (PNULL == ctrl_ptr)
	{
		return;
	}
    //��ȡsoftkey���
    MMITHEME_GetSoftkeyStyle(&softkey_style);

    // �������ַ��
    if (button_num == 0)
    {
        align_num = softkey_style.align[LEFT_BUTTON];//ALIGN_LVMIDDLE
    }
    else if (button_num == 1)
    {
        align_num = softkey_style.align[MIDDLE_BUTTON];//ALIGN_HVMIDDLE
    }
    else
    {
        align_num = softkey_style.align[RIGHT_BUTTON];//ALIGN_RVMIDDLE
    }
    // �����ı����
    str_style.align = align_num;
    str_style.font = GetTextFont(softkey_ptr);
    str_style.font_color = softkey_ptr->font_color;
#ifdef ADULT_WATCH_SUPPORT
    str_style.align = ALIGN_HVMIDDLE;
#endif
    // ��������
    part_rect = MMITHEME_GetSoftKeyTextRect(align_num, rect);
    if (button_ptr->content.data_type == GUISK_DATA_TEXT_ID)
    {
        // ��ȡ�ı���Ϣ
	    MMITHEME_GetResText(button_ptr->content.data_u.text_id, softkey_ptr->win_handle, &text);
    }
    else if (button_ptr->content.data_type == GUISK_DATA_TEXT_BUFFER)
    {
        text.wstr_len = button_ptr->content.data_u.text_buffer.wstr_len;
        text.wstr_ptr= button_ptr->content.data_u.text_buffer.wstr_ptr;
    }

    if(PNULL != text.wstr_ptr && 0 != text.wstr_len) // ��ʾ����
    {
        // ��ʾ���µ�Ч��ʱ�����µ�������
        if (button_ptr->is_pressed)
        {
            part_rect.left += 1;
            part_rect.top += 1;
            part_rect.right += 1;
            part_rect.bottom += 1;
        }
        GUISTR_DrawTextToLCDInRect(&ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*)&part_rect, 
            (const GUI_RECT_T*)&part_rect, 
            (const MMI_STRING_T*)&text, 
            &str_style, 
            str_state, 
            GUISTR_TEXT_DIR_AUTO);
    }
}

/*****************************************************************************/
//  Description : ���µ�������
//  Global resource dependence : 
//  Author:  xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ReadjustDispRect(
                                  GUI_RECT_T rect, 
                                  int16 width, 
                                  int16 height,
                                  GUISOFTKEY_INDEX button_num
                                  )
{
	GUI_RECT_T disp_rect = {0};

	if (0 == width || 0 == height)
	{
		disp_rect = rect;
		return disp_rect;
	}
    
    // ������������
    if (rect.right - rect.left + 1 <= width)
    {
        disp_rect.left = rect.left;
        disp_rect.right = rect.right;
    }
    else
    {
        if (LEFT_BUTTON == button_num)
        {
            disp_rect.left = rect.left + GUISOFTKEY_EDGE;
            disp_rect.right = disp_rect.left + width - 1;
        }
        else if (RIGHT_BUTTON == button_num)
        {
            disp_rect.right = rect.right - GUISOFTKEY_EDGE;
            disp_rect.left = rect.right - width + 1;
        }
        else
        {
            disp_rect.left = rect.left + ((rect.right - rect.left + 1 - width)/2);
            disp_rect.right = disp_rect.left + width -1;
        }
    }
    // ������������
    if (rect.bottom - rect.top + 1 <= height)
    {
        disp_rect.top = rect.top;
        disp_rect.bottom = rect.bottom;
    }
    else
    {
        disp_rect.top = rect.top+ ((rect.bottom - rect.top + 1 - height)/2);
        disp_rect.bottom = disp_rect.top + height - 1;
    }
    
    return disp_rect;

}

/*****************************************************************************/
// Description : ��button text
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonContent(
                             CTRLSOFTKEY_OBJ_T*    softkey_ptr,
                             GUISOFTKEY_INDEX  button_num
                             )
{
    GUI_RECT_T      rect = softkey_ptr->button_arr[button_num].rect;
    GUI_BUTTON_T *  button_ptr = &softkey_ptr->button_arr[button_num]; 
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

    switch(button_ptr->content.data_type)
    {
    case GUISK_DATA_TEXT_ID:
    case GUISK_DATA_TEXT_BUFFER:
        DrawButtonText(softkey_ptr, button_num);
        break;

    case GUISK_DATA_IMAGE_ID:
        {
            uint16     width      = 0;
            uint16     height     = 0;
            GUI_RECT_T disp_rect  = {0};
            GUI_RECT_T image_rect = {0};
            
            if (MMITHEME_CheckImageID(button_ptr->content.data_u.image_id))
            {
                GUIRES_GetImgWidthHeight(&width, &height, button_ptr->content.data_u.image_id, softkey_ptr->win_handle);
                
                disp_rect = ReadjustDispRect(rect, width, height, button_num);
                IMG_EnableTransparentColor(TRUE); 
                image_rect.right = width - 1;
                image_rect.bottom = height - 1;
                GUIRES_DisplayImg(PNULL,
                    &disp_rect,
                    &image_rect,
                    softkey_ptr->win_handle,
                    button_ptr->content.data_u.image_id,
                    &ctrl_ptr->lcd_dev_info);
                IMG_EnableTransparentColor(FALSE);
            }
            if(softkey_ptr->button_arr[button_num].is_gray)
            {
                LCD_FillArgbRect(&ctrl_ptr->lcd_dev_info, softkey_ptr->button_arr[button_num].rect, softkey_ptr->bg_color, GUISOFTKEY_GRAY_IMG_ALPHA);
            }
        }

        break;

    default:
        //SCI_TRACE_LOW:"DrawButtonContent invalid type = %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_706_112_2_18_3_21_27_311,(uint8*)"d", button_ptr->content.data_type);
        break;
    }

    return;
}

/*****************************************************************************/
// Description : ��softkey ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawSoftkeyBg(
                         CTRLSOFTKEY_OBJ_T* softkey_ptr
                         )
{
    GUI_POINT_T         dis_point = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T*     ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return;
    }

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = ctrl_ptr->rect;
    bg_display.display_rect   = ctrl_ptr->rect;
    bg_display.img_rect       = ctrl_ptr->rect;
    bg_display.ctrl_handle    = ctrl_ptr->handle;
    bg_display.win_handle     = softkey_ptr->win_handle;

    ctrl_ptr->bg = MMITHEME_GetCommonBg();

    //������ͼƬ����䱳��ɫ
    switch (softkey_ptr->bg_type)
    {
    case GUI_SOFTKEY_BG_NULL:
        //display bg
        GUI_DisplayBg(&ctrl_ptr->bg,
                &bg_display,
                &ctrl_ptr->lcd_dev_info);
        break;

    case GUI_SOFTKEY_BG_IMAGE_ONLY:
        //display bg
        GUI_DisplayBg(&ctrl_ptr->bg,
                &bg_display,
                &ctrl_ptr->lcd_dev_info);

        //display softkey bg
        dis_point.x = ctrl_ptr->rect.left;
        dis_point.y = ctrl_ptr->rect.top;

        GUIRES_DisplayImg(PNULL,//&dis_point,
            &ctrl_ptr->rect,
            PNULL,//&image_rect,
            softkey_ptr->win_handle,
            softkey_ptr->bg_image,
            &ctrl_ptr->lcd_dev_info);
        break;

    case GUI_SOFTKEY_BG_COLOR_ONLY:
        LCD_FillArgbRect(&ctrl_ptr->lcd_dev_info, ctrl_ptr->rect, softkey_ptr->bg_color, softkey_ptr->bg_alpha);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawSoftkey(
                       CTRLSOFTKEY_OBJ_T* softkey_ptr
                       )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return;
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONOFTKEY_Update(softkey_ptr);
    }
    else
    {
        //������ͼƬ����䱳��ɫ
        DrawSoftkeyBg(softkey_ptr);

        //����softkey������button����
        CalSoftkeyRect(softkey_ptr);

        //��softkey
        DrawAllButtons(softkey_ptr);
    }
}

/*****************************************************************************/
// Description : handle the point in softkeybar rect
// Global resource dependence : 
// Author:Robert.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSoftkeyBar(
                                    CTRLSOFTKEY_OBJ_T	*softkey_ptr,
                                    GUI_POINT_T     point    //[IN]
                                    )
{
    MMI_TEXT_ID_T    invalid_text = 0;
    MMI_TEXT_ID_T    leftsoft_id = 0;
    MMI_TEXT_ID_T    middlesoft_id = 0;
    MMI_TEXT_ID_T    rightsoft_id = 0;
    CTRLBASE_OBJ_T*  ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }

	leftsoft_id = softkey_ptr->button_arr[LEFT_BUTTON].content.data_u.text_id;
	middlesoft_id = softkey_ptr->button_arr[MIDDLE_BUTTON].content.data_u.text_id;
	rightsoft_id = softkey_ptr->button_arr[RIGHT_BUTTON].content.data_u.text_id;
    
    //���softkey bar����������ַ���Ϊ�գ���ȡĬ�Ͽ��
    invalid_text = MMITHEME_GetInvalidTextID();

    if (GUI_PointIsInRect(point, ctrl_ptr->rect))
    {
		//���softkey bar����������ַ�Ϊ�գ��򷵻�MMI_RESULT_FALSE
        if (GUI_PointIsInRect(point, softkey_ptr->button_arr[LEFT_BUTTON].rect)
			&& softkey_ptr->button_arr[LEFT_BUTTON].is_show
            && !softkey_ptr->button_arr[LEFT_BUTTON].is_gray)
        {
            return GUI_RESULT_CTLOK;
        }

        if (GUI_PointIsInRect(point, softkey_ptr->button_arr[MIDDLE_BUTTON].rect)
			&& softkey_ptr->button_arr[MIDDLE_BUTTON].is_show
            && !softkey_ptr->button_arr[MIDDLE_BUTTON].is_gray)
        {
            return GUI_RESULT_CTLMDL;
        }

        if (GUI_PointIsInRect(point, softkey_ptr->button_arr[RIGHT_BUTTON].rect)
			&& softkey_ptr->button_arr[RIGHT_BUTTON].is_show
            && !softkey_ptr->button_arr[RIGHT_BUTTON].is_gray)
        {
            return GUI_RESULT_CTLCANCEL;
        }

        return MMI_RESULT_FALSE;
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawPressedSoftkey(
                                 CTRLSOFTKEY_OBJ_T *softkey_ptr,
                                 GUI_POINT_T   point,
                                 BOOLEAN       is_pressed
                                 )
{
    BOOLEAN        recode           = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return FALSE;
    }

   
    if (GUI_PointIsInRect(point, softkey_ptr->button_arr[LEFT_BUTTON].rect) // in left rect
        && softkey_ptr->button_arr[LEFT_BUTTON].is_show)
    {
        softkey_ptr->button_arr[LEFT_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
        recode = TRUE;
    }
    else if (GUI_PointIsInRect(point, softkey_ptr->button_arr[MIDDLE_BUTTON].rect) // in middle rect
        && softkey_ptr->button_arr[MIDDLE_BUTTON].is_show)
    {
        softkey_ptr->button_arr[MIDDLE_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
        recode = TRUE;
    }
    else if (GUI_PointIsInRect(point, softkey_ptr->button_arr[RIGHT_BUTTON].rect) // in right rect
        && softkey_ptr->button_arr[RIGHT_BUTTON].is_show)
    {
        softkey_ptr->button_arr[RIGHT_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
        recode = TRUE;
    }
    
    return recode;
}

/*****************************************************************************/
// Description : ��softkey, �����Ƿ��µĲ���
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawSoftkeyByIndex(
							   CTRLSOFTKEY_OBJ_T			*softkey_ptr,
                               uint32                   sk_index,
                               BOOLEAN                  is_pressed
                               )
{
	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return;
    }

    // draw left/right/middle softkey
    switch(sk_index)
    {
    case LEFT_BUTTON:
        softkey_ptr->button_arr[LEFT_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
    	break;

    case MIDDLE_BUTTON:
        softkey_ptr->button_arr[MIDDLE_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
        break;

    case RIGHT_BUTTON:
        softkey_ptr->button_arr[RIGHT_BUTTON].is_pressed = is_pressed;
        DrawAllButtons(softkey_ptr);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// Description : handle press in softkeybar  
// Global resource dependence : 
// Author:Robert.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSoftkeyBarTPMsg(
                                         CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                         MMI_MESSAGE_ID_E		msg_id,
                                         GUI_POINT_T			point
                                         )
{
    MMI_RESULT_E        result              =  MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT    
    BOOLEAN             is_send_msg         = FALSE;
    MMI_MESSAGE_ID_E    send_msg_id         = 0;
	int16				i				    = 0;
	GUI_RECT_T          softkey_rect        = {0};
	
    MMI_RESULT_E		pre_result		    =  MMI_RESULT_FALSE;
	MMI_HANDLE_T		win_handle          = 0;
	MMI_HANDLE_T        ctrl_handle         = 0;
	BOOLEAN             is_press_long       = FALSE;
	static BOOLEAN		s_tplong_handle     = FALSE; //���tp long��Ϣ�������ˣ���tp up��Ϣ���ٴ���
    CTRLBASE_OBJ_T *    ctrl_ptr            = (CTRLBASE_OBJ_T *)softkey_ptr;

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }
	win_handle = softkey_ptr->win_handle;
	softkey_rect = ctrl_ptr->rect;

	//��Ϊ���ڱ����涨����Txt_null,��������Ҫsoftkey�ģ�����ϵͳ�Ѿ���������ˣ������������⴦���¡���ʹϵͳ������tp��
	if (MSG_TP_PRESS_DOWN == msg_id
		&& GUI_PointIsInRect(point,MMITHEME_GetSoftkeyRect(MIDDLE_BUTTON))
		&& GUISK_DATA_TEXT_ID == softkey_ptr->button_arr[MIDDLE_BUTTON].content.data_type
		&& MMITHEME_IsMidkeyNull(softkey_ptr->button_arr[MIDDLE_BUTTON].content.data_u.text_id))
	{
		//DrawSoftkey(softkey_ptr);
        return MMI_RESULT_FALSE;
	}

    if ( !softkey_ptr->tp_disable )
	{
		//���tp long��Ϣ�����ڴ����ˣ���tp up��Ϣ���ٴ���
		if (MSG_TP_PRESS_DOWN == msg_id)
		{
			s_tplong_handle = FALSE;
		}
		else if(MSG_TP_PRESS_UP == msg_id && s_tplong_handle)
		{
			s_tplong_handle = FALSE;
			return MMI_RESULT_TRUE;
		}

		if (MSG_TP_PRESS_DOWN == msg_id)
		{
			if (GUI_PointIsInRect( point, softkey_rect ))
			{
				SetWinSKBPressed( softkey_ptr, TRUE );
			}
			else
			{
				//���ȵ��������softkey��
				return MMI_RESULT_FALSE;
			}
		}
		else if ( !GetWinSKBPressed( softkey_ptr ) )
		{
			//softkey δ����
			return MMI_RESULT_FALSE;
		}
	}
	else
	{
		//������softkey
		return MMI_RESULT_FALSE;
	}

    switch ( msg_id )
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        SetWinSKBPressed( softkey_ptr, TRUE );

        //�Ƿ�����������ĳ��button ͼ��
        for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
        {
            SetWinSKBButtonPressed( softkey_ptr, i, FALSE );

            if ( softkey_ptr->button_arr[i].is_show 
                && GUI_PointIsInRect( point, softkey_ptr->button_arr[i].rect )
                && !softkey_ptr->button_arr[i].is_gray)
            {   
                DrawPressedSoftkey(softkey_ptr, point, TRUE);

                SetWinSKBButtonPressed( softkey_ptr, i, TRUE );
                break;
            }      
        }

        result = MMI_RESULT_TRUE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (GetWinSKBPressed( softkey_ptr ))
        {
            result = HandleSoftkeyBar( softkey_ptr, point );//handle the softkey 

            for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
            {
                if ( softkey_ptr->button_arr[i].is_show 
                    && softkey_ptr->button_arr[i].is_pressed )
                {   
                    SetWinSKBButtonPressed( softkey_ptr, i, FALSE );

                    DrawSoftkeyByIndex( softkey_ptr, i, FALSE);
                }
            } 

			SetWinSKBPressed( softkey_ptr, FALSE );

			if (MMI_RESULT_FALSE == result)
			{
				result = MMI_RESULT_TRUE;
			}
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
		if (GetWinSKBPressed( softkey_ptr ))
		{
			//�Ƿ���������ڣ������д���
			result  = HandleSoftkeyBar( softkey_ptr, point );//handle the softkey 			

			for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
			{
				if ( softkey_ptr->button_arr[i].is_pressed && GUI_PointIsInRect( point, softkey_ptr->button_arr[i].rect) 
					&& softkey_ptr->button_arr[i].is_long_press)
				{   
					SetWinSKBButtonPressed( softkey_ptr, i, FALSE );

					DrawSoftkeyByIndex( softkey_ptr, i, FALSE);

					s_tplong_handle = TRUE;

					SetWinSKBPressed( softkey_ptr, FALSE );
				}
			} 

			if (!s_tplong_handle)
			{
				result = MMI_RESULT_TRUE;
			}
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		if ( GetWinSKBPressed( softkey_ptr ) )
		{
			for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
			{
				if ( softkey_ptr->button_arr[i].is_pressed && !GUI_PointIsInRect( point, softkey_ptr->button_arr[i].rect) )
				{   
					SetWinSKBButtonPressed( softkey_ptr, i, FALSE );

					DrawSoftkeyByIndex( softkey_ptr, i, FALSE);
				}
				else if ( softkey_ptr->button_arr[i].is_show && GUI_PointIsInRect( point, softkey_ptr->button_arr[i].rect ) 
					&& !softkey_ptr->button_arr[i].is_pressed && !softkey_ptr->button_arr[i].is_gray)
				{
					DrawPressedSoftkey(softkey_ptr, point, TRUE);

					SetWinSKBButtonPressed( softkey_ptr, i, TRUE );
				}
			} 

			result =  MMI_RESULT_TRUE;
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
		result  =  MMI_RESULT_FALSE;
        break;
    }

	if (MMI_RESULT_FALSE != result && MSG_TP_PRESS_LONG == msg_id)
	{
		is_press_long = TRUE;
	}

    //handle the softkey bar
    pre_result = result;
    switch (result)
    {
    case GUI_RESULT_CTLOK:
        if (!is_press_long)
        {
            is_send_msg = TRUE;
            send_msg_id = MSG_APP_OK;
        }
        break;

    case GUI_RESULT_CTLCANCEL:
        is_send_msg = TRUE;
        if (!is_press_long)
        {
            send_msg_id = MSG_APP_CANCEL;
        }
        else
        {
            send_msg_id = MSG_KEYLONG_CANCEL;
        }            
        break;

    case GUI_RESULT_CTLMDL:
        if (!is_press_long)
        {
            is_send_msg = TRUE;
            send_msg_id = MSG_APP_WEB;
        }
        break;

    default:
        break;         
    }

    //get active control handle
    ctrl_handle = MMK_GetActiveCtrl(win_handle);
    if ((0 != ctrl_handle) &&
        (is_send_msg))
    {
        result = MMK_RunCtrlProc(ctrl_handle,send_msg_id,PNULL);
        if (!result)
        {
            //get parent handle
            ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
            while (0 != ctrl_handle)
            {
                result = MMK_RunCtrlProc(ctrl_handle,send_msg_id,PNULL);
                if (result)
                {
                    break;
                }
                ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
            }
        }
    }
    
    if ((MMI_RESULT_FALSE == result) || (pre_result == result))
    {
        result = MMK_RunWinProc(win_handle,send_msg_id,PNULL);                    
    }
#endif
	return result;
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISOFTKEY_Construct(
                                   CTRLBASE_OBJ_T	        *iguictrl_ptr,
                                   CTRLSOFTKEY_INIT_PARAM_T *softkey_param_ptr
                                   )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != iguictrl_ptr && PNULL != init_data_ptr ); /*assert verified*/    
    if ((PNULL == iguictrl_ptr)||(PNULL == softkey_param_ptr))
    {
        return FALSE;
    }

    GUISOFTKEY_Init(
            iguictrl_ptr,
            softkey_param_ptr->leftsoft_id,
            softkey_param_ptr->midsoft_id,
            softkey_param_ptr->rightsoft_id);
    
    IGUICTRL_SetState((IGUICTRL_T *)iguictrl_ptr, GUICTRL_STATE_DISABLE_ACTIVE, TRUE );
    
    return TRUE;
}

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISOFTKEY_Destruct(
                                  CTRLBASE_OBJ_T		*iguictrl_ptr
                                  )
{
    uint32 i = 0;
    CTRLSOFTKEY_OBJ_T *softkey_ptr = (CTRLSOFTKEY_OBJ_T *)iguictrl_ptr;

    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        if (softkey_ptr->button_arr[i].content.data_type == GUISK_DATA_TEXT_BUFFER
            && softkey_ptr->button_arr[i].content.data_u.text_buffer.wstr_ptr != PNULL)
        {
            SCI_FREE(softkey_ptr->button_arr[i].content.data_u.text_buffer.wstr_ptr);
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : is button style
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsButtonStyle( 
                            GUISOFTKEY_STYLE_E   style
                            )
{
    if (GUISOFTKEY_STYLE_BUTTON == style)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : init button
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
LOCAL void GUISOFTKEY_Init( 
                           CTRLBASE_OBJ_T		*ctrl_ptr,
                           MMI_TEXT_ID_T		leftsoft_id,
                           MMI_TEXT_ID_T		midsoft_id,
                           MMI_TEXT_ID_T		rightsoft_id
                           )
{
    CTRLSOFTKEY_OBJ_T*  softkey_ptr = (CTRLSOFTKEY_OBJ_T*)ctrl_ptr;
	GUI_BOTH_RECT_T		both_rect = {0};
	MMI_SOFTKEY_STYLE_T softkey_style = {0};
	
	SCI_MEMSET(softkey_ptr->button_arr, 0, sizeof(softkey_ptr->button_arr));

	softkey_ptr->win_handle = MMK_GetWinHandleByCtrl( ctrl_ptr->handle );
	SCI_ASSERT(NULL != softkey_ptr->win_handle);/*assert verified*/

	MMK_GetBothRect(softkey_ptr->win_handle, &both_rect);

    ctrl_ptr->both_rect = both_rect;     
    ctrl_ptr->both_rect.v_rect.top = (int16)(ctrl_ptr->both_rect.v_rect.bottom - (int16)MMITHEME_GetWinSoftkeyHeight(softkey_ptr->win_handle) + 1);
    ctrl_ptr->both_rect.h_rect.top = (int16)(ctrl_ptr->both_rect.h_rect.bottom - (int16)MMITHEME_GetWinSoftkeyHeight(softkey_ptr->win_handle) + 1);        

#ifdef GUIF_SOFTKEY_PDASTYLE
    softkey_ptr->style = GUISOFTKEY_STYLE_BUTTON;
#else
    softkey_ptr->style = GUISOFTKEY_STYLE_BAR;
#endif
    
#ifdef MMI_PDA_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        softkey_ptr->is_set_text_to_title = TRUE;
    }
    else
#endif
    {
        softkey_ptr->is_set_text_to_title = FALSE;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_INIT_DATA_T init_data = {0};

        init_data.win_handle = softkey_ptr->win_handle;
        init_data.handle = ctrl_ptr->handle;
        init_data.text_id_arr[0] = leftsoft_id;
        init_data.text_id_arr[1] = midsoft_id;
        init_data.text_id_arr[2] = rightsoft_id;
    
        GUIBUTTONSOFTKEY_Init(softkey_ptr, &init_data);
    }
    else
    {
        softkey_ptr->frameType  = BUTTON_FRAME_NULL;

	    if (MMITHEME_GetSoftkeyStyle(&softkey_style))
	    {
		    softkey_ptr->bg_type    = softkey_style.bg_type;
		    softkey_ptr->bg_image   = softkey_style.bg_image;
		    softkey_ptr->bg_color   = softkey_style.bg_color;
            softkey_ptr->bg_alpha   = softkey_style.bg_alpha;//����͸��ɫ
		    softkey_ptr->font_color = softkey_style.font_color;

		    softkey_ptr->button_arr[LEFT_BUTTON].is_pressed     = FALSE;
		    softkey_ptr->button_arr[LEFT_BUTTON].rect			= softkey_style.skb_button[LEFT_BUTTON].rect;
		    softkey_ptr->button_arr[LEFT_BUTTON].is_show        = softkey_style.skb_button[LEFT_BUTTON].is_show;
		    softkey_ptr->button_arr[LEFT_BUTTON].icon_up_id     = softkey_style.skb_button[LEFT_BUTTON].icon_up_id;
		    softkey_ptr->button_arr[LEFT_BUTTON].icon_down_id   = softkey_style.skb_button[LEFT_BUTTON].icon_down_id;
		    softkey_ptr->button_arr[LEFT_BUTTON].gray_icon_id   = softkey_style.skb_button[LEFT_BUTTON].icon_up_id;

		    softkey_ptr->button_arr[MIDDLE_BUTTON].is_pressed     = FALSE;
		    softkey_ptr->button_arr[MIDDLE_BUTTON].rect			  = softkey_style.skb_button[MIDDLE_BUTTON].rect;
		    softkey_ptr->button_arr[MIDDLE_BUTTON].is_show        = softkey_style.skb_button[MIDDLE_BUTTON].is_show;
		    softkey_ptr->button_arr[MIDDLE_BUTTON].icon_up_id     = softkey_style.skb_button[MIDDLE_BUTTON].icon_up_id;
		    softkey_ptr->button_arr[MIDDLE_BUTTON].icon_down_id   = softkey_style.skb_button[MIDDLE_BUTTON].icon_down_id;
		    softkey_ptr->button_arr[MIDDLE_BUTTON].gray_icon_id   = softkey_style.skb_button[MIDDLE_BUTTON].icon_up_id;

		    softkey_ptr->button_arr[RIGHT_BUTTON].is_pressed     = FALSE;
		    softkey_ptr->button_arr[RIGHT_BUTTON].rect			 = softkey_style.skb_button[RIGHT_BUTTON].rect;
		    softkey_ptr->button_arr[RIGHT_BUTTON].is_show        = softkey_style.skb_button[RIGHT_BUTTON].is_show;
		    softkey_ptr->button_arr[RIGHT_BUTTON].icon_up_id     = softkey_style.skb_button[RIGHT_BUTTON].icon_up_id;
		    softkey_ptr->button_arr[RIGHT_BUTTON].icon_down_id   = softkey_style.skb_button[RIGHT_BUTTON].icon_down_id;
		    softkey_ptr->button_arr[RIGHT_BUTTON].gray_icon_id   = softkey_style.skb_button[RIGHT_BUTTON].icon_up_id;
	    }

        // Ĭ�������֧�ֵ��ı���
        softkey_ptr->button_arr[0].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[0].content.data_u.text_id = leftsoft_id;
        softkey_ptr->button_arr[1].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[1].content.data_u.text_id = midsoft_id;
        softkey_ptr->button_arr[2].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[2].content.data_u.text_id	= rightsoft_id;

        // ����ı�����Ч��
        if (MMI_IsTextNull(leftsoft_id))
        {
            softkey_ptr->button_arr[0].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[0].is_show = TRUE;
	    }

        if (MMI_IsTextNull(midsoft_id))
        {
            softkey_ptr->button_arr[1].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[1].is_show = TRUE;
	    }

        if (MMI_IsTextNull(rightsoft_id))
        {
            softkey_ptr->button_arr[2].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[2].is_show = TRUE;
	    }

        softkey_ptr->is_visible = TRUE;
	    softkey_ptr->style = GUISOFTKEY_STYLE_NULL;

        ctrl_ptr->bg = MMITHEME_GetCommonBg();
    }
}

/*****************************************************************************/
//  Description : handle msg of button
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E GUISOFTKEY_HandleMsg( 
                                        CTRLBASE_OBJ_T        *ctrl_ptr,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint	
    GUI_POINT_T           point = {0};
#endif
	CTRLSOFTKEY_OBJ_T *   softkey_ptr = PNULL;
    int16                 i = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    softkey_ptr = (CTRLSOFTKEY_OBJ_T *)ctrl_ptr;
    if (!softkey_ptr->is_visible)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
	{
    case MSG_CTL_OPEN:
        break;

	case MSG_CTL_PAINT:
		DrawSoftkey(softkey_ptr);
		break;
        
    case MSG_NOTIFY_UPDATE:
        if (IsButtonStyle(softkey_ptr->style))
        {
            //draw softkey bg
            GUIBUTTONSOFTKEY_DrawBg(softkey_ptr,(GUI_RECT_T *)param);
        }
        break;

    case MSG_CTL_LOSE_FOCUS:
        if (!IsButtonStyle(softkey_ptr->style))
        {        
            for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
            {
                SetWinSKBButtonPressed( softkey_ptr, i, FALSE );
            } 
		    SetWinSKBPressed( softkey_ptr, FALSE );
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
	case MSG_TP_PRESS_UP:
	case MSG_TP_PRESS_LONG:	
	case MSG_TP_PRESS_MOVE:
        if (!IsButtonStyle(softkey_ptr->style))
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
//             if ( msg_id == MSG_TP_PRESS_DOWN )
//             {
//                 MMK_DelayTpMove();
//             }

		    recode = HandleSoftkeyBarTPMsg(softkey_ptr, msg_id, point);
        }
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;
}

/*****************************************************************************/
// 	Description : ����softkey���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetTextId(
                                    MMI_HANDLE_T    win_handle,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update       // whether update
                                    )
{
    BOOLEAN				result  = TRUE;
	CTRLSOFTKEY_OBJ_T		*softkey_ptr = PNULL;

    //SCI_TRACE_LOW:"GUISOFTKEY_SetTextId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_1558_112_2_18_3_21_29_312,(uint8*)"d", win_handle);

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);

	if (NULL == softkey_ptr)
	{
		return FALSE;
	}

    // ��softkey���ַ�ת��title��
    if (softkey_ptr->is_set_text_to_title
        && MMK_GetWinDisplayStyleState(win_handle, WS_HAS_TITLE)
        && !MMK_GetWinDisplayStyleState(win_handle, WS_DISABLE_SOFTKEY_TO_TITLE))
    {
        GUISOFTKEY_BUTTONSTYLE_E softkey_style = GUISOFTKEY_BUTTONSTYLE_NONE;

        softkey_style = GUIBUTTONSOFTKEY_GetStyleFromText(leftsoft_id, middlesoft_id, rightsoft_id);

        switch (softkey_style)
        {
        case GUISOFTKEY_BUTTONSTYLE_1BUTTON:
            if (MMITHEME_GetCancelTextID() == middlesoft_id
                || MMITHEME_GetReturnTextID() == middlesoft_id)
            {
                GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, middlesoft_id, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, middlesoft_id, FALSE);
            }
            break;

        case GUISOFTKEY_BUTTONSTYLE_2BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, leftsoft_id, FALSE);
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, rightsoft_id, FALSE);
            break;

        case GUISOFTKEY_BUTTONSTYLE_3BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, leftsoft_id, FALSE);

            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, rightsoft_id, FALSE);

            leftsoft_id = MMITHEME_GetInvalidTextID();
            rightsoft_id = MMITHEME_GetInvalidTextID();
            break;

        default:
            break;
        }
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetTextId(softkey_ptr, leftsoft_id, middlesoft_id, rightsoft_id, is_need_update);
    }
    else
    {
        //reset softkey
        ResetSoftkey(softkey_ptr);

        softkey_ptr->button_arr[0].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[0].content.data_u.text_id = leftsoft_id;
        softkey_ptr->button_arr[1].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[1].content.data_u.text_id = middlesoft_id;
        softkey_ptr->button_arr[2].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[2].content.data_u.text_id	= rightsoft_id;

        if (MMI_IsTextNull(leftsoft_id))
        {
            softkey_ptr->button_arr[0].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[0].is_show = TRUE;
	    }

        if (MMI_IsTextNull(middlesoft_id))
        {
            softkey_ptr->button_arr[1].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[1].is_show = TRUE;
	    }

        if (MMI_IsTextNull(rightsoft_id))
        {
            softkey_ptr->button_arr[2].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[2].is_show = TRUE;
	    }
    }

    if (softkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawSoftkey(softkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset softkey pressed state
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ResetSoftkey(
                        CTRLSOFTKEY_OBJ_T   *softkey_ptr
                        )
{
    uint16      i = 0;

    if (GetWinSKBPressed(softkey_ptr))
    {
        for (i=0; i<GUISOFTKEY_BUTTON_NUM; i++)
        {
            if ((softkey_ptr->button_arr[i].is_show) &&
                (softkey_ptr->button_arr[i].is_pressed))
            {
                SetWinSKBButtonPressed(softkey_ptr,i,FALSE);
            }
        }

        SetWinSKBPressed(softkey_ptr,FALSE);
    }
}

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetButtonTextId(
                                          MMI_HANDLE_T    win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          MMI_TEXT_ID_T   text_id,   // the left softkey id
                                          GUISOFTKEY_INDEX button_num,
                                          BOOLEAN         is_need_update       // whether update
                                          )
{
    BOOLEAN				result  = TRUE;
	CTRLSOFTKEY_OBJ_T		*softkey_ptr = PNULL;

    //SCI_TRACE_LOW:"GUISOFTKEY_SetButtonTextId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_1701_112_2_18_3_21_29_313,(uint8*)"d", win_handle);

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);

	if (NULL == softkey_ptr)
	{
		return FALSE;
	}

    // ��softkey������ת��title�Ͻ�����ʾ
    if (softkey_ptr->is_set_text_to_title
        && MMK_GetWinDisplayStyleState(win_handle, WS_HAS_TITLE)
        && !MMK_GetWinDisplayStyleState(win_handle, WS_DISABLE_SOFTKEY_TO_TITLE))
    {
        switch (button_num)
        {
        case LEFT_BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_RIGHT, text_id, FALSE);

            if (MMITHEME_GetInvalidTextID() == text_id)
            {
                GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            break;

        case MIDDLE_BUTTON:
            break;

        case RIGHT_BUTTON:
            GUIWIN_SetTitleButtonTextId(win_handle, GUITITLE_BUTTON_LEFT, text_id, FALSE);

            if (MMITHEME_GetInvalidTextID() == text_id)
            {
                GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            break;

        default:
            break;
        }
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetButtonTextId(softkey_ptr, text_id, button_num, is_need_update);
    }
    else
    {
        //reset softkey
        ResetSoftkey(softkey_ptr);

        softkey_ptr->button_arr[button_num].content.data_type = GUISK_DATA_TEXT_ID;
	    softkey_ptr->button_arr[button_num].content.data_u.text_id = text_id;

        if (MMI_IsTextNull(text_id))
        {
            softkey_ptr->button_arr[button_num].is_show = FALSE;
        }
	    else
	    {
		    softkey_ptr->button_arr[button_num].is_show = TRUE;
	    }    
    }


    if (softkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawSoftkey(softkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetButtonTextPtr(
                                           MMI_HANDLE_T    win_handle,
                                           MMI_CTRL_ID_T   ctrl_id,
                                           MMI_STRING_T    str_info,   // the left softkey id
                                           GUISOFTKEY_INDEX button_num,
                                           BOOLEAN         is_need_update       // whether update
                                           )
{
    BOOLEAN				result  = TRUE;
	CTRLSOFTKEY_OBJ_T		*softkey_ptr = PNULL;
    GUISK_DATA_TYPE_E   text_type = GUISK_DATA_MAX;

    //SCI_TRACE_LOW:"GUISOFTKEY_SetButtonTextId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_1790_112_2_18_3_21_29_314,(uint8*)"d", win_handle);

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);

	if (PNULL == softkey_ptr)
	{
		return FALSE;
	}

    // ���ｫsoftkey���ַ���ת�����õ�title��
    if (softkey_ptr->is_set_text_to_title
        && MMK_GetWinDisplayStyleState(win_handle, WS_HAS_TITLE)
        && !MMK_GetWinDisplayStyleState(win_handle, WS_DISABLE_SOFTKEY_TO_TITLE))
    {
        switch (button_num)
        {
        case LEFT_BUTTON:
            GUIWIN_SetTitleButtonText(win_handle, GUITITLE_BUTTON_RIGHT, &str_info, FALSE);

            if (PNULL == str_info.wstr_ptr
                || 0 == str_info.wstr_len)
            {
                GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            break;

        case MIDDLE_BUTTON:
            break;

        case RIGHT_BUTTON:
            GUIWIN_SetTitleButtonText(win_handle, GUITITLE_BUTTON_LEFT, &str_info, FALSE);

            if (PNULL == str_info.wstr_ptr
                || 0 == str_info.wstr_len)
            {
                GUIWIN_SetTitleButtonState(win_handle, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            break;

        default:
            break;
        }
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetButtonTextPtr(softkey_ptr, str_info, button_num, is_need_update);
    }
    else
    {
        //reset softkey
        ResetSoftkey(softkey_ptr);

        // ����֮ǰ������,Ϊ������ͷ��ڴ�ʹ��
        text_type = softkey_ptr->button_arr[button_num].content.data_type;

        softkey_ptr->button_arr[button_num].content.data_type = GUISK_DATA_TEXT_BUFFER;
	    softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_len = str_info.wstr_len;

        if (0 == str_info.wstr_len)
        {
            softkey_ptr->button_arr[button_num].is_show = FALSE;
            if (PNULL != softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr
                && text_type == GUISK_DATA_TEXT_BUFFER) 
            {
                // �ͷŵ�ʱ����Ҫ�ж�ԭ���������Ƿ�Ϊbuffer,�����ͷŻ��쳣��
                SCI_FREE(softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr);
                softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr = PNULL;
            }
        }
	    else
	    {
            softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*str_info.wstr_len);
            SCI_MEMSET(softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr, 0, sizeof(wchar)*str_info.wstr_len);
            MMIAPICOM_Wstrncpy(softkey_ptr->button_arr[button_num].content.data_u.text_buffer.wstr_ptr, str_info.wstr_ptr, str_info.wstr_len);
		    softkey_ptr->button_arr[button_num].is_show = TRUE;
	    }
    }

    if (softkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawSoftkey(softkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : ����softkey����IconͼƬID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetButtonIconId(
                                          MMI_HANDLE_T    win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          MMI_IMAGE_ID_T  icon_id,
                                          GUISOFTKEY_INDEX button_num,
                                          BOOLEAN         is_need_update       // whether update
                                          )
{
    BOOLEAN				result  = TRUE;
	CTRLSOFTKEY_OBJ_T		*softkey_ptr = PNULL;

    //SCI_TRACE_LOW:"GUISOFTKEY_SetButtonIconId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_1892_112_2_18_3_21_29_315,(uint8*)"d", win_handle);

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);

	if (NULL == softkey_ptr)
	{
		return FALSE;
	}

    //reset softkey
    ResetSoftkey(softkey_ptr);
	softkey_ptr->button_arr[button_num].content.data_type = GUISK_DATA_IMAGE_ID;
	softkey_ptr->button_arr[button_num].content.data_u.image_id = icon_id;
    if (MMITHEME_CheckImageID(icon_id))
    {
		softkey_ptr->button_arr[button_num].is_show = TRUE;
    }
    else
    {
        softkey_ptr->button_arr[button_num].is_show = FALSE;
    }

    if (softkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawSoftkey(softkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_SetStyle(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		font_color,
                                uint32			bg_type,
                                GUI_COLOR_T		bg_color,
                                MMI_IMAGE_ID_T	bg_img
                                )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_SetStyle() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_1953_112_2_18_3_21_29_316,(uint8*)"");
        return;
    }
    softkey_ptr->bg_type    = bg_type;
    softkey_ptr->bg_image   = bg_img;
    softkey_ptr->bg_color   = bg_color;
    softkey_ptr->font_color = font_color;

    return;
}


//++++CR242519
/*****************************************************************************/
// Description :set softkey pressed Style
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_SetPressedStyle(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		font_color,
                                uint32			bg_type,
                                GUI_COLOR_T		bg_color,
                                MMI_IMAGE_ID_T	bg_img
                                )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }
    softkey_ptr->bg_type    = bg_type;
    softkey_ptr->bg_color   = bg_color;
    softkey_ptr->font_color = font_color;
    softkey_ptr->button_arr[LEFT_BUTTON].icon_down_id = bg_img;
    softkey_ptr->button_arr[MIDDLE_BUTTON].icon_down_id = bg_img;
    softkey_ptr->button_arr[RIGHT_BUTTON].icon_down_id = bg_img;
    
    return;
}
//++++CR242519

/*****************************************************************************/
// Description : ����softkey�ı�������
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_SetBgType(
                                 MMI_HANDLE_T    win_handle,
                                 MMI_CTRL_ID_T   ctrl_id,
                                 GUI_SOFTKEY_BG_TYPE_T bg_type
                                 )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }
    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_SetBgType() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2030_112_2_18_3_21_30_317,(uint8*)"");
        return;
    }
    softkey_ptr->bg_type    = bg_type;

    return;
}
/*****************************************************************************/
// Description : ����softkey��͸��ֵ
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_SetBgAlpha(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                uint8         bg_alpha
                                )
{
    CTRLSOFTKEY_OBJ_T       *softkey_ptr = PNULL;
    
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

    softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }
    softkey_ptr->bg_alpha= bg_alpha;
    return;
}

/*****************************************************************************/
// Description : �����������ɫ
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_Setfont(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		*color_ptr,
                                GUI_FONT_T      *font_ptr
                                )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_Setfont() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2066_112_2_18_3_21_30_318,(uint8*)"");
        return;
    }
    if (PNULL != color_ptr)
    {
        softkey_ptr->font_color = *color_ptr;
    }
    if (PNULL != font_ptr)
    {
        softkey_ptr->font = *font_ptr;
    }

    return;
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL void SetWinSKBPressed(
                            CTRLSOFTKEY_OBJ_T	*softkey_ptr,
                            BOOLEAN         is_softkey_pressed
                            )
{
	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return;
    }

	softkey_ptr->is_softkey_pressed = is_softkey_pressed;

    return;
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinSKBPressed(
                               CTRLSOFTKEY_OBJ_T	*softkey_ptr
                               )
{
	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return FALSE;
    }

    return (BOOLEAN)(softkey_ptr->is_softkey_pressed);
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetWinSKBButtonPressed(
                                     CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                     GUISOFTKEY_INDEX   button_index,
                                     BOOLEAN			is_pressed
                                     )
{
	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != softkey_ptr); /*assert verified*/
    if (PNULL == softkey_ptr)
    {
        return FALSE;
    }
	SCI_ASSERT(button_index < GUISOFTKEY_BUTTON_NUM );/*assert verified*/

    softkey_ptr->button_arr[button_index].is_pressed = is_pressed;
    
    return TRUE;
}

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  CTRLSOFTKEY_SetButtonState(
                                          MMI_HANDLE_T	 win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          GUISOFTKEY_INDEX button_index,
                                          BOOLEAN         is_gray,
                                          BOOLEAN		 is_long_press
                                          )
{
	CTRLSOFTKEY_OBJ_T		 *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }
    SCI_ASSERT(button_index < GUISOFTKEY_BUTTON_NUM);/*assert verified*/
    
    //SCI_TRACE_LOW:"GUISOFTKEY_SetButtonState(),win_handle = %d, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2165_112_2_18_3_21_30_319,(uint8*)"dd", win_handle, is_gray);

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetButtonState(softkey_ptr, button_index, is_gray, is_long_press);
    }
    else
    {
        softkey_ptr->button_arr[button_index].is_gray = is_gray;
	    softkey_ptr->button_arr[button_index].is_long_press = is_long_press;
    }

    return TRUE;
}

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  CTRLSOFTKEY_SetButtonGray(
                                          MMI_HANDLE_T	 win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          GUISOFTKEY_INDEX button_index,
                                          BOOLEAN         is_gray
                                          )
{
	CTRLSOFTKEY_OBJ_T		 *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle || button_index >= GUISOFTKEY_BUTTON_NUM)
    {
        return FALSE;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetButtonGray(softkey_ptr, button_index, is_gray);
    }
    else
    {
        softkey_ptr->button_arr[button_index].is_gray = is_gray;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_GetTextId(
                                    MMI_HANDLE_T    win_handle,     //in:
                                    MMI_CTRL_ID_T   ctrl_id,        //in:
                                    MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                    MMI_TEXT_ID_T   *middle_id_ptr, //in:may PNULL
                                    MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                    )
{
	CTRLSOFTKEY_OBJ_T	*softkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle);/*assert verified*/
    //SCI_ASSERT((PNULL != left_id_ptr) || (PNULL != middle_id_ptr) || (PNULL != right_id_ptr));/*assert verified*/
    if ((0 == win_handle)||((PNULL == left_id_ptr) && (PNULL == middle_id_ptr) && (PNULL == right_id_ptr)))
    {
        return FALSE;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_GetTextId() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2254_112_2_18_3_21_30_320,(uint8*)"");
        return FALSE;
    }
    if (PNULL != left_id_ptr)
    {
        *left_id_ptr = softkey_ptr->button_arr[0].content.data_u.text_id;
    }
    
    if (PNULL != middle_id_ptr)
    {
        *middle_id_ptr = softkey_ptr->button_arr[1].content.data_u.text_id;
    }

    if (PNULL != right_id_ptr)
    {
        *right_id_ptr = softkey_ptr->button_arr[2].content.data_u.text_id;
    }

    return TRUE;
}

/*****************************************************************************/
// Description : ��softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSOFTKEY_Update(
                              MMI_HANDLE_T    win_handle,
                              MMI_CTRL_ID_T   ctrl_id
                              )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return;
    }    

	DrawSoftkey(softkey_ptr);

    return;
}

/*****************************************************************************/
// Description : whether softkey response to tp msg
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_IsTPEnable(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id
                                     )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_IsTPEnable() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2330_112_2_18_3_21_30_321,(uint8*)"");
        return FALSE;
    }

    return (BOOLEAN)(!softkey_ptr->tp_disable);
}

/*****************************************************************************/
//  Description : set tp enable or not
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetTPEnable(
                                      MMI_HANDLE_T    win_handle,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      BOOLEAN         tp_enable
                                      )
{
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    if (IsButtonStyle(softkey_ptr->style))
    {
        //SCI_TRACE_LOW:"GUISOFTKEY_SetTPEnable() not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_2364_112_2_18_3_21_30_322,(uint8*)"");
        return FALSE;
    }
    softkey_ptr->tp_disable = (BOOLEAN)(!tp_enable);

	return TRUE;
}

/*****************************************************************************/
//  Description : �����Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_SetVisible(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id,
                                     BOOLEAN         is_visible
                                     )
{
    BOOLEAN result = FALSE;
	CTRLSOFTKEY_OBJ_T		  *softkey_ptr = PNULL;
    
	//kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);
    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    result = softkey_ptr->is_visible;
    softkey_ptr->is_visible = is_visible;

    if (IsButtonStyle(softkey_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetVisible(softkey_ptr, is_visible);
    }

	return result;
}

/*****************************************************************************/
//  Description : ��ȡSoftkey�Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_GetVisible(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id
                                     )
{
    BOOLEAN result = FALSE;
    CTRLSOFTKEY_OBJ_T	 *softkey_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

    softkey_ptr = GetSoftkeyPtr(win_handle,ctrl_id);

    if (NULL == softkey_ptr)
    {
        return FALSE;
    }
    
    result = softkey_ptr->is_visible;

    return result;
}

/*****************************************************************************/
//  Description : Get softkey ptr
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLSOFTKEY_OBJ_T* GetSoftkeyPtr(
                                   MMI_HANDLE_T    win_handle,
                                   MMI_HANDLE_T    ctrl_handle //control id
                                   )
{
    CTRLSOFTKEY_OBJ_T* ctrl_ptr = PNULL;
    MMI_HANDLE_T ctrl_id = 0;
    
    if(0 != win_handle)
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        ctrl_id = MMK_ConvertIdToHandle( ctrl_handle );

        // ����������handle
        if (0 != ctrl_id && ctrl_handle == ctrl_id)
        {
            ctrl_ptr = (CTRLSOFTKEY_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);
        }
        else // ����������ID
        {
            ctrl_ptr = (CTRLSOFTKEY_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle, ctrl_handle);
        }

        if(PNULL != ctrl_ptr)
        {
            SCI_ASSERT(SoftkeyTypeOf((CTRLBASE_OBJ_T*)ctrl_ptr ));/*assert verified*/
        }
    }
    
    return ((CTRLSOFTKEY_OBJ_T*)ctrl_ptr);
}

/*****************************************************************************/
//  Description : ��̬����״̬���ؼ�
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSOFTKEY_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id, 
                                        MMI_TEXT_ID_T left_text_id,
                                        MMI_TEXT_ID_T mid_text_id,
                                        MMI_TEXT_ID_T right_text_id
                                        )
{
    BOOLEAN result = FALSE;
    MMI_CONTROL_CREATE_T   softkey_ctrl = {0};
    GUISOFTKEY_INIT_DATA_T init_data = {0};
    
    //init softkey
    init_data.leftsoft_id  = left_text_id;
    init_data.midsoft_id   = mid_text_id;
    init_data.rightsoft_id = right_text_id;
    
    //create softkey control
    softkey_ctrl.ctrl_id           = ctrl_id;// MMITHEME_GetSoftkeyCtrlId();
    softkey_ctrl.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_ctrl.init_data_ptr     = &init_data;
    softkey_ctrl.parent_win_handle = win_handle;
    if (PNULL != MMK_CreateControl(&softkey_ctrl))
    {
        result = TRUE;
        MMK_SetWinSoftkeyCtrlId(win_handle, ctrl_id);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get softkey height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SoftkeyCtrlOnStateChanged(
                               CTRLBASE_OBJ_T       *ctrl_ptr,
                               GUICTRL_STATE_T      state,
                               BOOLEAN              is_true
                               )
{
    CTRLSOFTKEY_OBJ_T   *softkey_ctrl_ptr = (CTRLSOFTKEY_OBJ_T *)ctrl_ptr;

    if (IsButtonStyle(softkey_ctrl_ptr->style))
    {
        GUIBUTTONSOFTKEY_SetState(softkey_ctrl_ptr, state, is_true);
    }
}

/*****************************************************************************/
//  Description : set softkey rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlSetRect(
                              CTRLBASE_OBJ_T        *ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              )
{
    BOOLEAN             result = TRUE;
    CTRLSOFTKEY_OBJ_T       *softkey_ctrl_ptr = PNULL;

    softkey_ctrl_ptr = (CTRLSOFTKEY_OBJ_T *)ctrl_ptr;

    if ((PNULL == softkey_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }
    if (!GUI_EqualRect(ctrl_ptr->rect,*rect_ptr))
    {
        //set display rect
        ctrl_ptr->display_rect  = *rect_ptr;  
        //set rect
        ctrl_ptr->rect          = *rect_ptr;        
        GUI_SetVOrHRect( ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(ctrl_ptr->both_rect));       
        if (IsButtonStyle(softkey_ctrl_ptr->style))
        {
            GUIBUTTONSOFTKEY_SetRect(softkey_ctrl_ptr, (GUI_RECT_T *)rect_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get softkey height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlGetHeightByWidth(
                                          CTRLBASE_OBJ_T    *ctrl_ptr,  //in:
                                          uint16        width,      //in:
                                          uint16        *height_ptr //in/out:
                                          )
{
    BOOLEAN     result = TRUE;

    //get softkey height
    *height_ptr = (uint16)MMITHEME_GetSoftkeyHeight();

    GUI_INVALID_PARAM(ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM(width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
//  Description : get softkey display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlGetDisplayRect(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,  //in:
                                        GUI_RECT_T  *rect_ptr   //in/out:
                                        )
{
    BOOLEAN         result = FALSE;
    CTRLSOFTKEY_OBJ_T   *softkey_ctrl_ptr = PNULL;

    softkey_ctrl_ptr = (CTRLSOFTKEY_OBJ_T *)ctrl_ptr;
    if (PNULL != rect_ptr)
    {
        if (!GUI_IsRectEmpty(ctrl_ptr->display_rect))
        {
            *rect_ptr = ctrl_ptr->display_rect;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set softkey display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SoftkeyCtrlSetDisplayRect(
                                        CTRLBASE_OBJ_T          *ctrl_ptr,  //in:
                                        const GUI_RECT_T    *rect_ptr,  //in:
                                        BOOLEAN             is_update   //in:
                                        )
{
    BOOLEAN         result = TRUE;
    CTRLSOFTKEY_OBJ_T   *softkey_ctrl_ptr = PNULL;

    softkey_ctrl_ptr = (CTRLSOFTKEY_OBJ_T *)ctrl_ptr;

    //set display rect
    ctrl_ptr->display_rect = *rect_ptr;

    //////////////////////////////////////////////////////////////////////////
    // fixed by feng.xiao 4 NEWMS00167829
    // -fix: incorrectly update softkey when unfocused
    if (is_update && MMK_IsFocusWin(softkey_ctrl_ptr->win_handle))
    {
        //update softkey
        DrawSoftkey(softkey_ctrl_ptr);
    }

    return (result);
}

