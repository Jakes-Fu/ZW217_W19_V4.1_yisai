/*****************************************************************************
** File Name:      ctrlscrollkey.c                                               *
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
#ifdef GUIF_SCROLLKEY

#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmitheme_scrollkey.h"
#include "ctrlscrollkey.h"
#include "mmk_tp.h"
#include "mmk_window_internal.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define SCROLLKEY_ANIM_TIMER_ONE            20
#define SCROLLKEY_ANIM_TIMER_TWO            20
#define SCROLLKEY_ANMI_OFFSET               10
#define DISTANCE_FROM_ARC_TO_POINT          50

#ifdef MAINLCD_SIZE_240X320
#define CONFIRM_POINT_OFFSET                40
#define TEXT_AREA_OFFSET                    52
#define L_SCROLLKEY_OFFSET                  80
#define R_SCROLLKEY_OFFSET                  80
#elif defined(MAINLCD_SIZE_240X400)
#define CONFIRM_POINT_OFFSET                40
#define TEXT_AREA_OFFSET                    52
#define L_SCROLLKEY_OFFSET                  80
#define R_SCROLLKEY_OFFSET                  80
#elif defined(MAINLCD_SIZE_320X480)
#define CONFIRM_POINT_OFFSET                40
#define TEXT_AREA_OFFSET                    52
#define L_SCROLLKEY_OFFSET                  95
#define R_SCROLLKEY_OFFSET                  95
#elif defined(MAINLCD_SIZE_176X220)
#define CONFIRM_POINT_OFFSET                20
#define TEXT_AREA_OFFSET                    26
#define L_SCROLLKEY_OFFSET                  48
#define R_SCROLLKEY_OFFSET                  48
#else
#define CONFIRM_POINT_OFFSET                20
#define TEXT_AREA_OFFSET                    26
#define L_SCROLLKEY_OFFSET                  48
#define R_SCROLLKEY_OFFSET                  48
#endif

#define GUISCROLLKEY_EDGE    				4

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
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
//  Description : init scrollkey class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlInitVtbl(
    CTRLSCROLLKEY_VTBL_T        *scrollkey_vtbl_ptr
);

/*****************************************************************************/
//  Description : scrollkey construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLSCROLLKEY_INIT_PARAM_T   *scrollkey_param_ptr
);

/*****************************************************************************/
//  Description : scrollkey destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ScrollkeyCtrlPackInitParam (
    GUISCROLLKEY_INIT_DATA_T     *scrollkey_init_ptr,   //in
    CTRLSCROLLKEY_INIT_PARAM_T   *scrollkey_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle scrollkey msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollkeyCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
//     Description : ��button
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyButton(
                        CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                        GUISCROLLKEY_INDEX  button_num,
                        GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                        );

/*****************************************************************************/
//     Description : �õ�button�Ŀ��
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL uint16 GetButtonWidth(
                            CTRLSCROLLKEY_OBJ_T* scrollkey_ptr,
                            GUISCROLLKEY_INDEX button_num
                            );

/*****************************************************************************/
//  Description : reset SCROLLKEY pressed state
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ResetScrollkey(
                        CTRLSCROLLKEY_OBJ_T   *scrollkey_ptr
                        );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL void SetWinSKBPressed(
                            CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr,
                            BOOLEAN         is_scrollkey_pressed
                            );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinSKBPressed(
                               CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr
                               );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetWinSKBButtonPressed(
                                     CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr,
                                     GUISCROLLKEY_INDEX   button_index,
                                     BOOLEAN			is_pressed
                                     );

/*****************************************************************************/
//     Description : handle press in scrollkeybar  
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScrollkeyBarTPMsg(
                                         CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr,
                                         MMI_MESSAGE_ID_E		msg_id,
                                         GUI_POINT_T			point
                                         );

/*****************************************************************************/
// Description : ��scrollkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkey(
                       CTRLSCROLLKEY_OBJ_T* scrollkey_ptr
                       );

/*****************************************************************************/
//  Description : Get scrollkey ptr
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLLKEY_OBJ_T* GetScrollkeyPtr(
                                   MMI_HANDLE_T    win_handle,
                                   MMI_HANDLE_T    ctrl_handle //control id
                                   );

/*****************************************************************************/
// Description : ��scrollkey ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyBg(
                         CTRLSCROLLKEY_OBJ_T* scrollkey_ptr,
                         GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                         );

/*****************************************************************************/
// Description : ��button text
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonText(
                          CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                          GUISCROLLKEY_INDEX  button_num,
                          GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                          );

/*****************************************************************************/
// Description : ��button ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonContent(
                             CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                             GUISCROLLKEY_INDEX  button_num,
                             GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                             );

/*****************************************************************************/
// Description : ����button��ͼƬ��ʾ����ÿ������Ϊ1/3��������Ŀ��
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollkeyButtonDispRect(
                                          CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                                          GUISCROLLKEY_INDEX  button_num
                                          );

/*****************************************************************************/
// Description : ��button
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonBg(
                        CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                        GUISCROLLKEY_INDEX  button_num,
                        GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                        );

/*****************************************************************************/
// Description : get text font
// Global resource dependence : 
// Author: hua.fang
// Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetTextFont(
                             CTRLSCROLLKEY_OBJ_T  *scrollkey_ptr
                             );

/*****************************************************************************/
//  Description : Get scrollkey layer info
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetScrollkeyLayerInfo(
                                            CTRLSCROLLKEY_OBJ_T *scrollkey_ptr
                                            );

/*****************************************************************************/
//  Description : draw scrollkey anim one
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyAnimOne(
                       uint8  timer_id, 
                       uint32 param
                       );

/*****************************************************************************/
//  Description : draw scrollkey anim two
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyAnimTwo(
                       uint8  timer_id, 
                       uint32 param
                       );

/*****************************************************************************/
//  Description : cal moved scrollkey 
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void CalMovedScrollkey(
					   CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr,
                       GUI_POINT_T         point,
                       GUISCROLLKEY_INDEX  button_index
                       );

/*****************************************************************************/
//  Description : start scrollkey proc
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void StartScrollkeyProc(
					   CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr,
                       GUISCROLLKEY_INDEX  button_index
                       );

/*****************************************************************************/
//  Description : draw moved scrollkey
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawMovedScrollkey(
                       CTRLSCROLLKEY_OBJ_T* scrollkey_ptr
                       );

/*****************************************************************************/
//  Description : draw confirm point
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawConfirmPoint(
                       CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                       GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                       );

/*****************************************************************************/
//  Description : create scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateScrollkeyLayer(
                       CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr
                       );

/*****************************************************************************/
//  Description : destroy scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DestroyScrollkeyLayer(
					   CTRLSCROLLKEY_OBJ_T 	   *scrollkey_ptr
					   );

/*****************************************************************************/
//  Description : update scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateScrollkeyLayer(
                               CTRLSCROLLKEY_OBJ_T 	   *scrollkey_ptr
                               );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollKeyTextRect(
    CTRLSCROLLKEY_OBJ_T*	scrollkey_ptr,
    GUISCROLLKEY_INDEX	    button_idx
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8 g_scrollkey_timer_id_one = 0;
LOCAL uint8 g_scrollkey_timer_id_two = 0;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get scrollkey type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SCROLLKEY_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "scrollkey",                                         // control name
                       sizeof (CTRLSCROLLKEY_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) ScrollkeyCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) ScrollkeyCtrlDestruct,      // control destructor function
                       sizeof (CTRLSCROLLKEY_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) ScrollkeyCtrlPackInitParam,     // convert init param
                       sizeof (CTRLSCROLLKEY_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) ScrollkeyCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init scrollkey class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlInitVtbl (
    CTRLSCROLLKEY_VTBL_T        *scrollkey_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) scrollkey_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ScrollkeyCtrlHandleMsg;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

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
LOCAL void ScrollkeyCtrlPackInitParam (
    GUISCROLLKEY_INIT_DATA_T     *scrollkey_init_ptr,   //in
    CTRLSCROLLKEY_INIT_PARAM_T   *scrollkey_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) scrollkey_param_ptr;

    if (PNULL == scrollkey_init_ptr || PNULL == scrollkey_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = scrollkey_init_ptr->both_rect;

    scrollkey_param_ptr->left_scroll_key_id = scrollkey_init_ptr->left_scroll_key_id;
    scrollkey_param_ptr->right_scroll_key_id = scrollkey_init_ptr->right_scroll_key_id;
    scrollkey_param_ptr->left_scroll_text_id = scrollkey_init_ptr->left_scroll_text_id;
    scrollkey_param_ptr->right_scroll_text_id = scrollkey_init_ptr->right_scroll_text_id;
    scrollkey_param_ptr->left_scroll_skin_idx = scrollkey_init_ptr->left_scroll_skin_idx;
    scrollkey_param_ptr->right_scroll_skin_idx = scrollkey_init_ptr->right_scroll_skin_idx;
}

/*****************************************************************************/
//  Description : scrollkey construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLSCROLLKEY_INIT_PARAM_T  *scrollkey_param_ptr
)
{
	GUI_BOTH_RECT_T		    both_rect = {0};
	MMI_SCROLLKEY_STYLE_T   scrollkey_style = {0};
	uint16                  width = 0;
    uint16                  height = 0;
    CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr = (CTRLSCROLLKEY_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == scrollkey_param_ptr)
    {
        return FALSE;
    }

	SCI_MEMSET(scrollkey_ptr->button_arr, 0, sizeof(scrollkey_ptr->button_arr));

	scrollkey_ptr->win_handle = MMK_GetWinHandleByCtrl( base_ctrl_ptr->handle );   
	SCI_ASSERT(NULL != scrollkey_ptr->win_handle);  /*assert verified*/

	MMK_GetBothRect(scrollkey_ptr->win_handle, &both_rect);

    base_ctrl_ptr->both_rect = both_rect;
    base_ctrl_ptr->both_rect.v_rect.top = (int16)(base_ctrl_ptr->both_rect.v_rect.bottom - (int16)MMITHEME_GetScrollkeyHeight() + 1);
    base_ctrl_ptr->both_rect.h_rect.top = (int16)(base_ctrl_ptr->both_rect.h_rect.bottom - (int16)MMITHEME_GetScrollkeyHeight() + 1);

	base_ctrl_ptr->both_rect.v_rect.top = (int16)(base_ctrl_ptr->both_rect.v_rect.top - (int16)MMITHEME_GetScrollkeyOffsetFromBottom());
	base_ctrl_ptr->both_rect.h_rect.top = (int16)(base_ctrl_ptr->both_rect.h_rect.top - (int16)MMITHEME_GetScrollkeyOffsetFromBottom());
	base_ctrl_ptr->both_rect.v_rect.bottom = (int16)(base_ctrl_ptr->both_rect.v_rect.bottom - (int16)MMITHEME_GetScrollkeyOffsetFromBottom());
	base_ctrl_ptr->both_rect.h_rect.bottom = (int16)(base_ctrl_ptr->both_rect.h_rect.bottom - (int16)MMITHEME_GetScrollkeyOffsetFromBottom());

	if (MMITHEME_GetScrollkeyStyle(
            &scrollkey_style,
            scrollkey_param_ptr->left_scroll_skin_idx,
            scrollkey_param_ptr->right_scroll_skin_idx))
	{
		scrollkey_ptr->bg_type    = scrollkey_style.bg_type;
		scrollkey_ptr->bg_image   = scrollkey_style.bg_image;
		scrollkey_ptr->bg_color   = scrollkey_style.bg_color;
		scrollkey_ptr->font_color = scrollkey_style.font_color;
		//scrollkey_ptr->font       = scrollkey_style.font;     // ������������ˣ���Ϊ����theme������ȡ���壬��Ϊ�����С���������
		scrollkey_ptr->frameType  = scrollkey_style.frameType;
		scrollkey_ptr->char_space = scrollkey_style.char_space;
		//scrollkey_ptr->is_has_icon = TRUE;

		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed   = FALSE;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_confirmed = FALSE;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_animated  = FALSE;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect		      = scrollkey_style.skb_button[LEFTSCROLL_BUTTON].rect;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].icon_up_id       = scrollkey_style.skb_button[LEFTSCROLL_BUTTON].icon_up_id;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].icon_down_id     = scrollkey_style.skb_button[LEFTSCROLL_BUTTON].icon_down_id;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].icon_confirm_id  = scrollkey_style.skb_button[LEFTSCROLL_BUTTON].icon_confirm_id;
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].confirm_point_id = scrollkey_style.skb_button[LEFTSCROLL_BUTTON].confirm_point_id;			

		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed   = FALSE;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_confirmed = FALSE;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_animated  = FALSE;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect		       = scrollkey_style.skb_button[RIGHTSCROLL_BUTTON].rect;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].icon_up_id       = scrollkey_style.skb_button[RIGHTSCROLL_BUTTON].icon_up_id;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].icon_down_id     = scrollkey_style.skb_button[RIGHTSCROLL_BUTTON].icon_down_id;
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].icon_confirm_id  = scrollkey_style.skb_button[RIGHTSCROLL_BUTTON].icon_confirm_id;	
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].confirm_point_id = scrollkey_style.skb_button[RIGHTSCROLL_BUTTON].confirm_point_id;			
	}

    scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].content.data_type = GUISCROLLKEY_DATA_IMAGE_ID;
	scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].content.data_u_main.image_id = scrollkey_param_ptr->left_scroll_key_id;
	scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].content.data_u_secondary.text_id = scrollkey_param_ptr->left_scroll_text_id;
    scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].content.data_type = GUISCROLLKEY_DATA_IMAGE_ID;
	scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].content.data_u_main.image_id = scrollkey_param_ptr->right_scroll_key_id;
	scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].content.data_u_secondary.text_id = scrollkey_param_ptr->right_scroll_text_id;	

	GUIRES_GetImgWidthHeight(
            &width, &height,
            scrollkey_param_ptr->left_scroll_key_id,
            scrollkey_ptr->win_handle);

	GUIRES_GetImgWidthHeight(
            &width, &height,
            scrollkey_param_ptr->right_scroll_key_id,
            scrollkey_ptr->win_handle);	

    scrollkey_ptr->is_visible = TRUE;

    base_ctrl_ptr->lcd_dev_info = *(MMITHEME_GetDefaultLcdDev());

    scrollkey_ptr->scrollkey_layer.block_id = UILAYER_NULL_HANDLE;
    scrollkey_ptr->scrollkey_layer.lcd_id   = GUI_MAIN_LCD_ID;

    VTLBASE_SetCanActive(base_ctrl_ptr, FALSE);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : scrollkey destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    uint32                  i = 0;
    CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr = (CTRLSCROLLKEY_OBJ_T*) base_ctrl_ptr;

    if (PNULL == scrollkey_ptr)
    {
        return FALSE;
    }

    for (i = 0; i < GUISCROLLKEY_BUTTON_NUM; i++)
    {
        if (scrollkey_ptr->button_arr[i].content.data_type == GUISCROLLKEY_DATA_TEXT_BUFFER
            && scrollkey_ptr->button_arr[i].content.data_u_main.text_buffer.wstr_ptr != PNULL)
        {
            SCI_FREE(scrollkey_ptr->button_arr[i].content.data_u_main.text_buffer.wstr_ptr);
        }
        if (PNULL != scrollkey_ptr->button_arr[i].param)
        {
            MMK_DestroyControl(GUICTRL_GetCtrlHandle( (IGUICTRL_T*)scrollkey_ptr->button_arr[i].param));
            scrollkey_ptr->button_arr[i].param = PNULL;
        }
    }
    MMK_StopTimer(g_scrollkey_timer_id_two);
    MMK_StopTimer(g_scrollkey_timer_id_one);

	DestroyScrollkeyLayer(scrollkey_ptr);
	
    return TRUE;
}

/*****************************************************************************/
//  Description : handle scrollkey msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollkeyCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    int16                   i = 0;
    GUI_POINT_T             point = {0};
    CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr = (CTRLSCROLLKEY_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;

    if (PNULL == scrollkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (!scrollkey_ptr->is_visible)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
	{
    case MSG_CTL_OPEN:
    case MSG_CTL_GET_FOCUS:
        DestroyScrollkeyLayer(scrollkey_ptr);
		CreateScrollkeyLayer(scrollkey_ptr);
        UpdateScrollkeyLayer(scrollkey_ptr);
        break;

	case MSG_CTL_PAINT:
		DrawScrollkey(scrollkey_ptr);
		break;

    case MSG_CTL_LOSE_FOCUS:
        for ( i = 0; i < GUISCROLLKEY_BUTTON_NUM; i++ )
        {
            SetWinSKBButtonPressed( scrollkey_ptr, i, FALSE );
        } 
		SetWinSKBPressed( scrollkey_ptr, FALSE );
        
        MMK_StopTimer(g_scrollkey_timer_id_two);
        MMK_StopTimer(g_scrollkey_timer_id_one);

        DrawScrollkey(scrollkey_ptr);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
	case MSG_TP_PRESS_UP:
	case MSG_TP_PRESS_LONG:	
	case MSG_TP_PRESS_MOVE:
        {    
            MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;
            MMK_GetLogicTPMsg(&tp_status,&point);
		    result = HandleScrollkeyBarTPMsg(scrollkey_ptr, msg_id, point);
        }
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_SCROLLKEY_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get scrollkey pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLLKEY_OBJ_T* GetScrollkeyPtr (
    MMI_HANDLE_T    win_handle,
    MMI_HANDLE_T    ctrl_handle //control id
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle,ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (ScrollkeyTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLSCROLLKEY_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ScrollkeyTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_SCROLLKEY_TYPE);
}

/*****************************************************************************/
//     Description : �õ�button�Ŀ��
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL uint16 GetButtonWidth(
                            CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                            GUISCROLLKEY_INDEX      button_num
                            )
{
    uint16                piexl_num = 0;
	MMI_STRING_T          text = {0};
    GUI_SCROLLKEY_ITEM_T  *button_ptr = &scrollkey_ptr->button_arr[button_num];

    switch (scrollkey_ptr->frameType)
    {
	    case BUTTON_FRAME_RECT:    //����
	        piexl_num = 2;
	        break;

	    case BUTTON_FRAME_3D:      //3D����
	        piexl_num = 2;
	        break;

	    case BUTTON_FRAME_IMAGE:   //ͼ�α߿�
	        break;

	    default:
	        break;
    }

    switch(button_ptr->content.data_type)
    {
	    case GUISCROLLKEY_DATA_TEXT_ID:
	    case GUISCROLLKEY_DATA_TEXT_BUFFER:
	        if (GUISCROLLKEY_DATA_TEXT_ID == button_ptr->content.data_type)
	        {
	            MMITHEME_GetResText(button_ptr->content.data_u_main.text_id, scrollkey_ptr->win_handle, &text);
	        }
	        else if (GUISCROLLKEY_DATA_TEXT_BUFFER == button_ptr->content.data_type)
	        {
	            text.wstr_len = button_ptr->content.data_u_main.text_buffer.wstr_len;
	            text.wstr_ptr = button_ptr->content.data_u_main.text_buffer.wstr_ptr;
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
	                GetTextFont(scrollkey_ptr),       //[IN] String������
	                (uint8)(scrollkey_ptr->char_space)  //[IN] �ּ��
	                );
	        }
	        break;

	    case GUISCROLLKEY_DATA_IMAGE_ID:
	        GUIRES_GetImgWidthHeight(
                    &piexl_num, PNULL,
                    button_ptr->content.data_u_main.image_id,
                    scrollkey_ptr->win_handle);            
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
                             CTRLSCROLLKEY_OBJ_T  *scrollkey_ptr
                             )
{
    GUI_FONT_T font = 0;

    if (0 != scrollkey_ptr->font)
    {
        font = scrollkey_ptr->font;
    }
    else
    {
        font = MMITHEME_GetScrollKeyTextFont();
    }

    return font;
}

/*****************************************************************************/
// Description : ����button��ͼƬ��ʾ����ÿ������Ϊ1/3��������Ŀ��
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollkeyButtonDispRect(
                                          CTRLSCROLLKEY_OBJ_T*      scrollkey_ptr,
                                          GUISCROLLKEY_INDEX        button_num
                                          )
{
    GUI_RECT_T  rect = {0};
    uint16      left_width   = 0;   //���button�Ŀ��
    uint16      right_width  = 0;   //�ұ�button�Ŀ��
    uint16      side_width   = 0;   //����button�Ŀ��
    uint16      average_width = 0;  //ƽ�����
    uint16      total_width  = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    rect = base_ctrl_ptr->rect;
    total_width  = (base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1);

    average_width = total_width / GUISCROLLKEY_BUTTON_NUM;

    left_width = GetButtonWidth(scrollkey_ptr, LEFTSCROLL_BUTTON);
    right_width = GetButtonWidth(scrollkey_ptr, RIGHTSCROLL_BUTTON);

    side_width = MAX(left_width, right_width);
    if (side_width < average_width)                             // �������buttonС��ƽ��ֵ����ȡƽ��ֵ
    {
        side_width = average_width;
    }

    switch (button_num)
    {
	    case LEFTSCROLL_BUTTON:
	        rect.left = base_ctrl_ptr->rect.left;
	        rect.right = base_ctrl_ptr->rect.left + side_width - 1;
	        break;

	    case RIGHTSCROLL_BUTTON:
	        rect.left = base_ctrl_ptr->rect.right - side_width + 1;
	        rect.right = base_ctrl_ptr->rect.right;
	        break;

	    default:
	        SCI_ASSERT(FALSE && "wrong param");     /*assert verified*/
	        break;
    }

    if (rect.left > rect.right)
    {
        rect.right = rect.left;
    }

    return rect;
}


/*****************************************************************************/
// Description : ��button
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonBg(
                        CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                        GUISCROLLKEY_INDEX      button_num,
                        GUI_LCD_DEV_INFO *      lcd_dev_info_ptr
                        )
{
    GUI_SCROLLKEY_ITEM_T  *button_ptr = &scrollkey_ptr->button_arr[button_num];
    GUI_RECT_T            disp_rect = scrollkey_ptr->button_arr[button_num].rect;
    GUI_RECT_T            image_rect = disp_rect;
    GUI_BG_DISPLAY_T      bg_display = {0};
    GUI_BOTH_RECT_T	      both_rect = {0};
    CTRLBASE_OBJ_T        *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    if (disp_rect.left == disp_rect.right)
    {
        return;
    }

	MMK_GetBothRect(scrollkey_ptr->win_handle, &both_rect);

    image_rect.left -= base_ctrl_ptr->rect.left;
    image_rect.right -= base_ctrl_ptr->rect.left;
    image_rect.top -= base_ctrl_ptr->rect.top;
    image_rect.bottom -= base_ctrl_ptr->rect.top;

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = disp_rect;
    bg_display.img_rect       = disp_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = scrollkey_ptr->win_handle;

    //base_ctrl_ptr->bg = MMITHEME_GetThemeBg();//MMITHEME_GetCommonBg();

    switch(scrollkey_ptr->bg_type) // �ػ�ÿ�����ı�����Ŀ���Ƿ�ֹˢ��ʱ��Ӱ
    {
	    case GUI_SCROLLKEY_BG_NULL:
         #if 0
	        GUI_DisplayBg(&base_ctrl_ptr->bg,
	                &bg_display,
	                lcd_dev_info_ptr);
         #endif
	        break;

	    case GUI_SCROLLKEY_BG_IMAGE_ONLY:
	        //display bg
	        GUI_DisplayBg(&base_ctrl_ptr->bg,
	                &bg_display,
	                lcd_dev_info_ptr);

	        if (button_ptr->is_pressed)
	        {
	            GUIRES_DisplayImg(
                    PNULL, &disp_rect, &image_rect,
                    scrollkey_ptr->win_handle,
                    button_ptr->icon_down_id,
                    lcd_dev_info_ptr);
	        }
	        else
	        {
	            //display scrollkey up
	            GUIRES_DisplayImg(
	                PNULL, &disp_rect, &image_rect,
	                scrollkey_ptr->win_handle,
	                button_ptr->icon_up_id,
	                lcd_dev_info_ptr);
	        }
	        break;
	        
	    case GUI_SCROLLKEY_BG_COLOR_ONLY:
	        LCD_FillRect(lcd_dev_info_ptr, disp_rect, scrollkey_ptr->bg_color);
	        break;
	        
	    default:
	        //SCI_TRACE_LOW:"GUISCROLLKEY: scrollkey_ptr->bg_type error!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_546_112_2_18_3_21_12_285,(uint8*)"", scrollkey_ptr->bg_type);
	        break;
    }
}

/*****************************************************************************/
// Description : ��button text
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawButtonText(
                          CTRLSCROLLKEY_OBJ_T*      scrollkey_ptr,
                          GUISCROLLKEY_INDEX        button_num,
                          GUI_LCD_DEV_INFO *        lcd_dev_info_ptr
                          )
{
    GUI_RECT_T            part_rect = {0};
    MMI_STRING_T	      text = {0};
    GUI_SCROLLKEY_ITEM_T  *button_ptr = &scrollkey_ptr->button_arr[button_num];
    GUISTR_STYLE_T        str_style = {0};/*lint !e64*/
    GUISTR_STATE_T        str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    
    // �����ı����
    str_style.align = ALIGN_HVMIDDLE;
    str_style.font = GetTextFont(scrollkey_ptr);
    str_style.font_color = scrollkey_ptr->font_color;
    str_style.char_space = scrollkey_ptr->char_space;
	
    // ��������
    part_rect = GetScrollKeyTextRect(scrollkey_ptr, button_num);
    if (button_ptr->content.data_type == GUISCROLLKEY_DATA_TEXT_ID)
    {
        // ��ȡ�ı���Ϣ
	    MMITHEME_GetResText(
	        button_ptr->content.data_u_main.text_id,
	        scrollkey_ptr->win_handle, &text);
    }
    else if (button_ptr->content.data_type == GUISCROLLKEY_DATA_TEXT_BUFFER)
    {
        text.wstr_len = button_ptr->content.data_u_main.text_buffer.wstr_len;
        text.wstr_ptr= button_ptr->content.data_u_main.text_buffer.wstr_ptr;
    }
	else if(button_ptr->content.data_type == GUISCROLLKEY_DATA_IMAGE_ID)
	{
        // ��ȡ�ı���Ϣ
	    MMITHEME_GetResText(
	            button_ptr->content.data_u_secondary.text_id,
	            scrollkey_ptr->win_handle, &text);	
	}

    if(PNULL != text.wstr_ptr && 0 != text.wstr_len) // ��ʾ����
    {
        GUISTR_DrawTextToLCDInRect(lcd_dev_info_ptr,
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
                                  GUI_RECT_T            rect, 
                                  int16                 width, 
                                  int16                 height,
                                  GUISCROLLKEY_INDEX    button_num
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
        if (LEFTSCROLL_BUTTON == button_num)
        {
            disp_rect.left = rect.left + GUISCROLLKEY_EDGE;
            disp_rect.right = disp_rect.left + width - 1;
        }
        else if (RIGHTSCROLL_BUTTON == button_num)
        {
            disp_rect.right = rect.right - GUISCROLLKEY_EDGE;
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
                             CTRLSCROLLKEY_OBJ_T*       scrollkey_ptr,
                             GUISCROLLKEY_INDEX         button_num,
                             GUI_LCD_DEV_INFO *         lcd_dev_info_ptr
                             )
{
    GUI_RECT_T            rect = scrollkey_ptr->button_arr[button_num].rect;
    GUI_SCROLLKEY_ITEM_T  *button_ptr = &scrollkey_ptr->button_arr[button_num];
    
    switch(button_ptr->content.data_type)
    {
	    case GUISCROLLKEY_DATA_TEXT_ID:
	    case GUISCROLLKEY_DATA_TEXT_BUFFER:
	        DrawButtonText(scrollkey_ptr, button_num, lcd_dev_info_ptr);
	        break;

	    case GUISCROLLKEY_DATA_IMAGE_ID:
	        {
	            uint16     width = 0;
	            uint16     height = 0;
	            GUI_RECT_T disp_rect = {0};
	            GUI_RECT_T image_rect = {0};
	            
	            if (MMITHEME_CheckImageID(button_ptr->content.data_u_main.image_id))
	            {
	                disp_rect = ReadjustDispRect(rect, width, height, button_num);
	                IMG_EnableTransparentColor(TRUE);
					
					GUIRES_GetImgWidthHeight(
                            &width, &height,
                            button_ptr->content.data_u_main.image_id,
                            scrollkey_ptr->win_handle);
	                image_rect.right = width - 1;
	                image_rect.bottom = height - 1;                

					if(scrollkey_ptr->button_arr[button_num].is_pressed)
					{
						if(scrollkey_ptr->button_arr[button_num].is_confirmed)
						{
			                GUIRES_DisplayImg(PNULL,
			                    &disp_rect,
			                    &image_rect,
			                    scrollkey_ptr->win_handle,
			                    scrollkey_ptr->button_arr[button_num].icon_confirm_id,
			                    lcd_dev_info_ptr);
						}
						else
						{
			                GUIRES_DisplayImg(PNULL,
			                    &disp_rect,
			                    &image_rect,
			                    scrollkey_ptr->win_handle,
			                    scrollkey_ptr->button_arr[button_num].icon_down_id,
			                    lcd_dev_info_ptr);
						}
					}
					else
					{
		                GUIRES_DisplayImg(PNULL,
		                    &disp_rect,
		                    &image_rect,
		                    scrollkey_ptr->win_handle,
		                    scrollkey_ptr->button_arr[button_num].icon_up_id,
		                    lcd_dev_info_ptr);
	            	}
									
	                GUIRES_DisplayImg(PNULL,
	                    &disp_rect,
	                    &image_rect,
	                    scrollkey_ptr->win_handle,
	                    button_ptr->content.data_u_main.image_id,
	                    lcd_dev_info_ptr);
					
					DrawConfirmPoint(scrollkey_ptr, lcd_dev_info_ptr);					
					
					DrawButtonText(scrollkey_ptr, button_num, lcd_dev_info_ptr);
					
	                IMG_EnableTransparentColor(FALSE);
	            }
	        }

	        break;

	    default:
	        //SCI_TRACE_LOW:"DrawButtonContent invalid type = %d!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_747_112_2_18_3_21_13_286,(uint8*)"d", button_ptr->content.data_type);
	        break;
    }

    return;
}


/*****************************************************************************/
// Description : ��button
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyButton(
                        CTRLSCROLLKEY_OBJ_T*    scrollkey_ptr,
                        GUISCROLLKEY_INDEX      button_num,
                        GUI_LCD_DEV_INFO *      lcd_dev_info_ptr
                        )
{
    // ��ʾ����
    DrawButtonBg(scrollkey_ptr, button_num, lcd_dev_info_ptr);
    // ��ʾ����
    DrawButtonContent(scrollkey_ptr, button_num, lcd_dev_info_ptr);

    return;
}

/*****************************************************************************/
//  Description : ����scrollkey������button��rect
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
LOCAL void CalScrollkeyRect(
                          CTRLSCROLLKEY_OBJ_T    *scrollkey_ptr
                          )
{
	uint16  width = 0;
    uint16  height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    // ��
	GUIRES_GetImgWidthHeight(
	        &width, &height,
	        scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].content.data_u_main.image_id,
	        scrollkey_ptr->win_handle); 

	scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect = GUI_CreateRect(
			(uint16)(base_ctrl_ptr->rect.left - (width - L_SCROLLKEY_OFFSET)),
			base_ctrl_ptr->rect.top,
			(uint16)(base_ctrl_ptr->rect.left + L_SCROLLKEY_OFFSET),
			base_ctrl_ptr->rect.bottom
			);

    // ��
	GUIRES_GetImgWidthHeight(
	        &width, &height,
	        scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].content.data_u_main.image_id,
	        scrollkey_ptr->win_handle); 

    scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect = GUI_CreateRect(
			(uint16)(base_ctrl_ptr->rect.right - R_SCROLLKEY_OFFSET),
			base_ctrl_ptr->rect.top,
			(uint16)(base_ctrl_ptr->rect.right + (width - R_SCROLLKEY_OFFSET)),
			base_ctrl_ptr->rect.bottom
			);
}


/*****************************************************************************/
// Description : ��scrollkey ����
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyBg(
                         CTRLSCROLLKEY_OBJ_T*   scrollkey_ptr,
                         GUI_LCD_DEV_INFO *     lcd_dev_info_ptr
                         )
{
    GUI_POINT_T         dis_point = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = base_ctrl_ptr->rect;
    bg_display.img_rect       = base_ctrl_ptr->rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = scrollkey_ptr->win_handle;

    //base_ctrl_ptr->bg = MMITHEME_GetThemeBg();//MMITHEME_GetCommonBg();

    //������ͼƬ����䱳��ɫ
    switch (scrollkey_ptr->bg_type)
    {
	    case GUI_SCROLLKEY_BG_NULL:
         #if 0
	        //display bg
	        GUI_DisplayBg(&base_ctrl_ptr->bg,
	                &bg_display,
	                lcd_dev_info_ptr);
         #endif
	        break;

	    case GUI_SCROLLKEY_BG_IMAGE_ONLY:
	        //display bg
	        GUI_DisplayBg(&base_ctrl_ptr->bg,
	                &bg_display,
	                lcd_dev_info_ptr);

	        //display scrollkey bg
	        dis_point.x = base_ctrl_ptr->rect.left;
	        dis_point.y = base_ctrl_ptr->rect.top;

	        GUIRES_DisplayImg(PNULL,//&dis_point,
	            &base_ctrl_ptr->rect,
	            PNULL,//&image_rect,
	            scrollkey_ptr->win_handle,
	            scrollkey_ptr->bg_image,
	            lcd_dev_info_ptr);
	        break;

	    case GUI_SCROLLKEY_BG_COLOR_ONLY:
	        LCD_FillRect(lcd_dev_info_ptr, base_ctrl_ptr->rect, scrollkey_ptr->bg_color);
	        break;

	    default:
	        break;
    }
}

/*****************************************************************************/
// Description : ��scrollkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkey(
                       CTRLSCROLLKEY_OBJ_T* scrollkey_ptr
                       )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    lcd_dev_info = GetScrollkeyLayerInfo(scrollkey_ptr);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
	{
	    UILAYER_Clear(&lcd_dev_info);
	}
	
    //������ͼƬ����䱳��ɫ
    DrawScrollkeyBg(scrollkey_ptr, &lcd_dev_info);

    //����scrollkey������button����
    // xiaoqing����δ���Ŀǰû�ã���ʱ����
    //if (!scrollkey_ptr->is_has_icon)
    {
        CalScrollkeyRect(scrollkey_ptr);
    }

    //��scrollkey
    DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info); // left
    DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info); // right
}

/*****************************************************************************/
// Description : handle the point in scrollkeybar rect
// Global resource dependence : 
// Author:Robert.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScrollkeyBar(
                                    CTRLSCROLLKEY_OBJ_T	*   scrollkey_ptr,
                                    GUI_POINT_T             point    //[IN]
                                    )
{
    MMI_TEXT_ID_T         invalid_text = 0;
    MMI_TEXT_ID_T         leftscroll_id = 0;
    MMI_TEXT_ID_T         rightscroll_id = 0;
    CTRLBASE_OBJ_T        *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }

	leftscroll_id = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].content.data_u_main.text_id;
	rightscroll_id = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].content.data_u_main.text_id;
    
    //���scrollkey bar����������ַ���Ϊ�գ���ȡĬ�Ͽ��
    invalid_text = MMITHEME_GetInvalidTextID();

    if (GUI_PointIsInRect(point, base_ctrl_ptr->rect))
    {
		//���scrollkey bar����������ַ�Ϊ�գ��򷵻�MMI_RESULT_FALSE
        if (GUI_PointIsInRect(point, scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect)
            && !scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_gray)
        {
            return GUI_RESULT_CTLOK;
        }

        if (GUI_PointIsInRect(point, scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect)
            && !scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_gray)
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
// Description : ��scrollkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawPressedScrollkey(
                                 CTRLSCROLLKEY_OBJ_T *  scrollkey_ptr,
                                 GUI_POINT_T            point,
                                 BOOLEAN                is_pressed
                                 )
{
    BOOLEAN        recode           = FALSE;
    GUI_LCD_DEV_INFO lcd_dev_info = {0}; 

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    lcd_dev_info = GetScrollkeyLayerInfo(scrollkey_ptr);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
		{
	    UILAYER_Clear(&lcd_dev_info);
	}
   
    if (scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed)
    {
        scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed = is_pressed;
        scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed = FALSE;
        DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info);
        DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info);
        recode = TRUE;
    }
    else if (scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed)
    {
        scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed = FALSE;
        scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed = is_pressed;
        DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info);
        DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info);
        recode = TRUE;
    }
    
    return recode;
}

/*****************************************************************************/
// Description : ��scrollkey, �����Ƿ��µĲ���
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyByIndex(
							   CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr,
                               uint32                   sk_index,
                               BOOLEAN                  is_pressed
                               )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0}; 

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    lcd_dev_info = GetScrollkeyLayerInfo(scrollkey_ptr);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
		{
	    UILAYER_Clear(&lcd_dev_info);
	}

    // draw left/right scrollkey
    switch(sk_index)
    {
	    case LEFTSCROLL_BUTTON:
	        scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed = is_pressed;
	        DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info);
	        DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info);
	    	break;

	    case RIGHTSCROLL_BUTTON:
	        scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed = is_pressed;
	        DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info);
	        DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info);
	        break;

	    default:
	        break;
    }
}

/*****************************************************************************/
// Description : handle press in scrollkeybar  
// Global resource dependence : 
// Author:Robert.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScrollkeyBarTPMsg(
                                         CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr,
                                         MMI_MESSAGE_ID_E		msg_id,
                                         GUI_POINT_T			point
                                         )
{
	int16				i				    = 0;
	GUI_RECT_T          scrollkey_rect      = {0};
	MMI_RESULT_E        result              = MMI_RESULT_FALSE;
	static BOOLEAN		s_tplong_handle     = FALSE; //���tp long��Ϣ�������ˣ���tp up��Ϣ���ٴ���
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

	//kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return MMI_RESULT_FALSE;
    }
	scrollkey_rect = base_ctrl_ptr->rect;

    if ( !scrollkey_ptr->tp_disable )
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
			if (GUI_PointIsInRect( point, scrollkey_rect ))
			{
				SetWinSKBPressed( scrollkey_ptr, TRUE );
			}
			else
			{
				//���ȵ��������scrollkey��
				return MMI_RESULT_FALSE;
			}
		}
		else if ( !GetWinSKBPressed( scrollkey_ptr ) )
		{
			//scrollkey δ����
			return MMI_RESULT_FALSE;
		}
	}
	else
	{
		//������scrollkey
		return MMI_RESULT_FALSE;
	}

    switch ( msg_id )
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_TP_PRESS_DOWN:
	        SetWinSKBPressed( scrollkey_ptr, TRUE );

	        //�Ƿ�����������ĳ��button ͼ��
	        for ( i = 0; i < GUISCROLLKEY_BUTTON_NUM; i++ )
	        {
	            SetWinSKBButtonPressed( scrollkey_ptr, i, FALSE );

	            if ( GUI_PointIsInRect( point, scrollkey_ptr->button_arr[i].rect )
	                && !scrollkey_ptr->button_arr[i].is_gray )
	            {		
	                SetWinSKBButtonPressed( scrollkey_ptr, i, TRUE );

                	DrawPressedScrollkey(scrollkey_ptr, point, TRUE);
                                MMK_StopTimer(g_scrollkey_timer_id_one);
					g_scrollkey_timer_id_one = MMK_CreateTimerCallback(SCROLLKEY_ANIM_TIMER_ONE, DrawScrollkeyAnimOne, (uint32)scrollkey_ptr, FALSE);
					
	                break;
	            }      
	        }
	        result = MMI_RESULT_TRUE;
	        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_TP_PRESS_UP:
	        if (GetWinSKBPressed( scrollkey_ptr ))
	        {
	            for ( i = 0; i < GUISCROLLKEY_BUTTON_NUM; i++ )
	            {
	                if ( scrollkey_ptr->button_arr[i].is_pressed )
	                {
	        			MMK_StopTimer(g_scrollkey_timer_id_one);
						
	                    SetWinSKBButtonPressed( scrollkey_ptr, i, FALSE );

						if( !scrollkey_ptr->button_arr[i].is_confirmed )
						{
	                    	DrawScrollkeyByIndex( scrollkey_ptr, i, FALSE);

							DrawScrollkey( scrollkey_ptr );
						}

						break;
	                }
	            } 
				SetWinSKBPressed( scrollkey_ptr, FALSE );
	        }
			result = MMI_RESULT_TRUE;			
	        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_TP_PRESS_MOVE:
	        if (GetWinSKBPressed( scrollkey_ptr ))
	        {
	            for ( i = 0; i < GUISCROLLKEY_BUTTON_NUM; i++ )
	            {
				    if ( scrollkey_ptr->button_arr[i].is_pressed && 
						!scrollkey_ptr->button_arr[i].is_confirmed )
	                {			
						CalMovedScrollkey(scrollkey_ptr, point, i);
						
						DrawPressedScrollkey(scrollkey_ptr, point, TRUE);						
                	}
            	}
        	}			
			result = MMI_RESULT_TRUE;
	        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	    default:
			result = MMI_RESULT_FALSE;
	        break;
    }

	return result;
}

/*****************************************************************************/
//  Description : Get scrollkey layer info
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetScrollkeyLayerInfo(
                                            CTRLSCROLLKEY_OBJ_T *scrollkey_ptr
                                            )
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return lcd_dev_info;
    }
    
    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&scrollkey_ptr->scrollkey_layer))
    {
        lcd_dev_info.block_id = scrollkey_ptr->scrollkey_layer.block_id;
    }
    else
    {
        lcd_dev_info = base_ctrl_ptr->lcd_dev_info;
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : reset scrollkey pressed state
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ResetScrollkey(
                        CTRLSCROLLKEY_OBJ_T   *scrollkey_ptr
                        )
{
    uint16      i = 0;

    if (GetWinSKBPressed(scrollkey_ptr))
    {
        for (i=0; i<GUISCROLLKEY_BUTTON_NUM; i++)
        {
            if ( scrollkey_ptr->button_arr[i].is_pressed )
            {
                SetWinSKBButtonPressed(scrollkey_ptr,i,FALSE);
            }
        }

        SetWinSKBPressed(scrollkey_ptr,FALSE);
    }
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL void SetWinSKBPressed(
                            CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr,
                            BOOLEAN             is_scrollkey_pressed
                            )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

	scrollkey_ptr->is_scrollkey_pressed = is_scrollkey_pressed;

    return;
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinSKBPressed(
                               CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr
                               )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return FALSE;
    }

    return (BOOLEAN)(scrollkey_ptr->is_scrollkey_pressed);
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetWinSKBButtonPressed(
                                     CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr,
                                     GUISCROLLKEY_INDEX     button_index,
                                     BOOLEAN			    is_pressed
                                     )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return FALSE;
    }
	SCI_ASSERT(button_index < GUISCROLLKEY_BUTTON_NUM );    /*assert verified*/

    scrollkey_ptr->button_arr[button_index].is_pressed = is_pressed;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : draw scrollkey anim one
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyAnimOne(
                       uint8  timer_id,
                       uint32 param
                       )
{
    BOOLEAN is_need_redraw = FALSE;
	CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr = (CTRLSCROLLKEY_OBJ_T *)param;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }
    MMK_StopTimer(g_scrollkey_timer_id_one);

	if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_pressed)
	{
		if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left < base_ctrl_ptr->rect.right)
		{
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left + SCROLLKEY_ANMI_OFFSET;
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right + SCROLLKEY_ANMI_OFFSET;			
                        is_need_redraw = TRUE;
		}
	}
	else if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_pressed)
	{
		if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right > base_ctrl_ptr->rect.left)
		{
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left - SCROLLKEY_ANMI_OFFSET;
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right - SCROLLKEY_ANMI_OFFSET;			
                        is_need_redraw = TRUE;
		}	
	}
	
	DrawMovedScrollkey(scrollkey_ptr);	

    if (is_need_redraw)
    {
        g_scrollkey_timer_id_one = MMK_CreateTimerCallback(SCROLLKEY_ANIM_TIMER_ONE, DrawScrollkeyAnimOne, (uint32)scrollkey_ptr, FALSE);
    }
}

/*****************************************************************************/
//  Description : draw scrollkey anim two
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawScrollkeyAnimTwo(
                       uint8  timer_id,
                       uint32 param
                       )
{
	BOOLEAN                 is_send_msg = FALSE;
    BOOLEAN                 is_need_redraw = FALSE;
	MMI_MESSAGE_ID_E        send_msg_id = 0;
	MMI_HANDLE_T	        win_handle  = 0;
	CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr = (CTRLSCROLLKEY_OBJ_T *)param;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }
	win_handle = scrollkey_ptr->win_handle;	
        MMK_StopTimer(g_scrollkey_timer_id_two);
	if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_confirmed)
	{
		if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left < base_ctrl_ptr->rect.right)
		{
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left + SCROLLKEY_ANMI_OFFSET;
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right + SCROLLKEY_ANMI_OFFSET;			
            is_need_redraw = TRUE;
		}
		else
		{
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_confirmed = FALSE;
            is_send_msg = TRUE;
            send_msg_id = MSG_APP_LSKOK;			
		}
	}
	else if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_confirmed)
	{
		if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right > base_ctrl_ptr->rect.left)
		{
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left - SCROLLKEY_ANMI_OFFSET;
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right - SCROLLKEY_ANMI_OFFSET;			
            is_need_redraw = TRUE;
		}
		else
		{
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_confirmed = FALSE;
	        is_send_msg = TRUE;
            send_msg_id = MSG_APP_RSKOK; 			
		}	
	}

    if (is_send_msg)
    {   
        MMK_RunWinProc(win_handle,send_msg_id,PNULL);
		DrawScrollkey(scrollkey_ptr);
    }
	else
	{
		DrawMovedScrollkey(scrollkey_ptr);
	}

    if (is_need_redraw)
    {
        g_scrollkey_timer_id_two = MMK_CreateTimerCallback(SCROLLKEY_ANIM_TIMER_TWO, DrawScrollkeyAnimTwo, (uint32)scrollkey_ptr, FALSE);
    }
}

/*****************************************************************************/
//  Description : cal moved scrollkey 
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void CalMovedScrollkey(
					   CTRLSCROLLKEY_OBJ_T  *scrollkey_ptr,
                       GUI_POINT_T          point,
                       GUISCROLLKEY_INDEX   button_index
                       )
{
	uint16 left_point_offset = 0;
    uint16 right_point_offset = 0;
    uint16 offset = 0;
    uint16 width = 0;
    uint16 height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

	left_point_offset = base_ctrl_ptr->rect.left + CONFIRM_POINT_OFFSET;
	right_point_offset = base_ctrl_ptr->rect.right - CONFIRM_POINT_OFFSET;
	
	GUIRES_GetImgWidthHeight(
            &width, &height,
            scrollkey_ptr->button_arr[button_index].content.data_u_main.image_id,
            scrollkey_ptr->win_handle);
	
	switch(button_index)
	{
		case LEFTSCROLL_BUTTON:

			scrollkey_ptr->button_arr[button_index].rect.left = point.x + DISTANCE_FROM_ARC_TO_POINT - width;
			scrollkey_ptr->button_arr[button_index].rect.right = point.x + DISTANCE_FROM_ARC_TO_POINT;

			if(scrollkey_ptr->button_arr[button_index].rect.right > right_point_offset)
			{
				scrollkey_ptr->button_arr[button_index].is_confirmed = TRUE;
				if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left < base_ctrl_ptr->rect.right)
				{
					MMK_StopTimer(g_scrollkey_timer_id_one);
					offset = base_ctrl_ptr->rect.right - scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left;
					scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left + offset;
					scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right + offset;
				}
				if(scrollkey_ptr->button_arr[button_index].is_animated)
				{
				    MMK_StopTimer(g_scrollkey_timer_id_two);

			    	g_scrollkey_timer_id_two = MMK_CreateTimerCallback(
                            SCROLLKEY_ANIM_TIMER_TWO,
                            DrawScrollkeyAnimTwo, (uint32)scrollkey_ptr, FALSE);
				}
				else
				{
					StartScrollkeyProc(scrollkey_ptr, button_index);
				}
			}
			break;

		case RIGHTSCROLL_BUTTON:

			scrollkey_ptr->button_arr[button_index].rect.left = point.x - DISTANCE_FROM_ARC_TO_POINT;
			scrollkey_ptr->button_arr[button_index].rect.right = point.x - DISTANCE_FROM_ARC_TO_POINT + width;

			if(scrollkey_ptr->button_arr[button_index].rect.left < left_point_offset)
			{
				scrollkey_ptr->button_arr[button_index].is_confirmed = TRUE;
				if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right > base_ctrl_ptr->rect.left)
				{
					MMK_StopTimer(g_scrollkey_timer_id_one);
					offset = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right - base_ctrl_ptr->rect.left;
					scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left - offset;
					scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right = scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right - offset;
				}				
				if(scrollkey_ptr->button_arr[button_index].is_animated)
				{				
				    MMK_StopTimer(g_scrollkey_timer_id_two);

			    	g_scrollkey_timer_id_two = MMK_CreateTimerCallback(
                            SCROLLKEY_ANIM_TIMER_TWO,
                            DrawScrollkeyAnimTwo, (uint32)scrollkey_ptr, FALSE);
				}
				else
				{
					StartScrollkeyProc(scrollkey_ptr, button_index);				
				}
			}
			break;

		default:
			break;
	}
}

/*****************************************************************************/
//  Description : start scrollkey proc
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void StartScrollkeyProc(
					   CTRLSCROLLKEY_OBJ_T  *scrollkey_ptr,
                       GUISCROLLKEY_INDEX   button_index
                       )
{
	MMI_MESSAGE_ID_E   send_msg_id = 0;
	MMI_HANDLE_T	   win_handle  = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

	DrawMovedScrollkey(scrollkey_ptr);

	win_handle = scrollkey_ptr->win_handle;	

	if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_confirmed)
	{
		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].is_confirmed = FALSE;
        send_msg_id = MSG_APP_LSKOK;			
	}
	else if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_confirmed)
	{
		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].is_confirmed = FALSE;
        send_msg_id = MSG_APP_RSKOK; 			
	}

    MMK_RunWinProc(win_handle, send_msg_id, PNULL);
}

/*****************************************************************************/
//  Description : draw moved scrollkey
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawMovedScrollkey(
                       CTRLSCROLLKEY_OBJ_T* scrollkey_ptr
                       )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    lcd_dev_info = GetScrollkeyLayerInfo(scrollkey_ptr);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
	    UILAYER_Clear(&lcd_dev_info);
    }

    //������ͼƬ����䱳��ɫ
    DrawScrollkeyBg(scrollkey_ptr, &lcd_dev_info);

   	DrawScrollkeyButton(scrollkey_ptr, RIGHTSCROLL_BUTTON, &lcd_dev_info); // right
	DrawScrollkeyButton(scrollkey_ptr, LEFTSCROLL_BUTTON, &lcd_dev_info);  // left
}

/*****************************************************************************/
//  Description : update scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateScrollkeyLayer(
                               CTRLSCROLLKEY_OBJ_T 	   *scrollkey_ptr
                               )
{
	UILAYER_APPEND_BLT_T append_layer = {0};
		
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    if (!UILAYER_IsLayerActive(&scrollkey_ptr->scrollkey_layer))
    {
        return;
    }
		
    append_layer.lcd_dev_info = scrollkey_ptr->scrollkey_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;

    UILAYER_SetLayerColorKey(&scrollkey_ptr->scrollkey_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
		
	if (scrollkey_ptr->is_visible)
	{
		UILAYER_AppendBltLayer(&append_layer);
	}
	else
	{
		UILAYER_RemoveBltLayer(&append_layer);
	}
}

/*****************************************************************************/
//  Description : draw confirm point
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DrawConfirmPoint(
                      CTRLSCROLLKEY_OBJ_T*  scrollkey_ptr,
                      GUI_LCD_DEV_INFO *    lcd_dev_info_ptr
                      )
{
	GUI_RECT_T  left_point_rect, right_point_rect;
	uint16      width, height, left_point_offset, right_point_offset;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;

	left_point_offset = base_ctrl_ptr->rect.left + CONFIRM_POINT_OFFSET;
	right_point_offset = base_ctrl_ptr->rect.right - CONFIRM_POINT_OFFSET;	

	left_point_rect = right_point_rect = base_ctrl_ptr->rect;
	GUIRES_GetImgWidthHeight(
            &width, &height,
            scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].confirm_point_id,
            scrollkey_ptr->win_handle);
	left_point_rect.left = left_point_rect.left + left_point_offset - width / 2;
	left_point_rect.right = left_point_rect.left + width;

	GUIRES_GetImgWidthHeight(
            &width, &height,
            scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].confirm_point_id,
            scrollkey_ptr->win_handle);
	right_point_rect.left = right_point_rect.left + right_point_offset - width / 2;
	right_point_rect.right = right_point_rect.left + width;

	if(scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.left > left_point_offset ||
	   scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].rect.right < left_point_offset )
	{
		GUIRES_DisplayImg(PNULL,//&dis_point,
			&left_point_rect,
			PNULL,//&image_rect,
			scrollkey_ptr->win_handle,
			scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].confirm_point_id,
			lcd_dev_info_ptr);
	}	
	
	if(scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.left > right_point_offset ||
	   scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].rect.right < right_point_offset)
	{
		GUIRES_DisplayImg(PNULL,//&dis_point,
			&right_point_rect,
			PNULL,//&image_rect,
			scrollkey_ptr->win_handle,
			scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].confirm_point_id,
			lcd_dev_info_ptr);
	}
}

/*****************************************************************************/
//  Description : create scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateScrollkeyLayer(
                       CTRLSCROLLKEY_OBJ_T     *scrollkey_ptr
                       )
{   
    GUI_RECT_T       page_rect       = {0};
    UILAYER_CREATE_T create_info = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)scrollkey_ptr;
    
    if (PNULL != scrollkey_ptr)
    {
        page_rect = base_ctrl_ptr->rect;

        if (!GUI_IsInvalidRect(page_rect))
        {
            // ������
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = base_ctrl_ptr->handle;
            create_info.offset_x = page_rect.left;
            create_info.offset_y = page_rect.top;
            create_info.width = (page_rect.right + 1 - page_rect.left);
            create_info.height = (page_rect.bottom + 1 - page_rect.top);
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE; 
            UILAYER_CreateLayer(
                &create_info,  
                &scrollkey_ptr->scrollkey_layer
                );

            UILAYER_SetLayerPosition(&scrollkey_ptr->scrollkey_layer, page_rect.left, page_rect.top);
        }
    }
}

/*****************************************************************************/
//  Description : destroy scrollkye layer
//  Global resource dependence : 
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void DestroyScrollkeyLayer(
						CTRLSCROLLKEY_OBJ_T 	*scrollkey_ptr
						)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

    if (UILAYER_NULL_HANDLE != scrollkey_ptr->scrollkey_layer.block_id)
    {
    // �ͷŲ�
        UILAYER_RELEASELAYER(&scrollkey_ptr->scrollkey_layer);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&scrollkey_ptr->scrollkey_layer);
//        scrollkey_ptr->scrollkey_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollKeyTextRect(
    CTRLSCROLLKEY_OBJ_T*	scrollkey_ptr,
    GUISCROLLKEY_INDEX	    button_idx
)
{
    GUI_RECT_T button_rect = {0};
	
	if(button_idx == LEFTSCROLL_BUTTON)
	{
		button_rect = GUI_CreateRect(
			scrollkey_ptr->button_arr[button_idx].rect.left,
			scrollkey_ptr->button_arr[button_idx].rect.top,
			scrollkey_ptr->button_arr[button_idx].rect.right - TEXT_AREA_OFFSET,
			scrollkey_ptr->button_arr[button_idx].rect.bottom
			);
	}
	else if(button_idx == RIGHTSCROLL_BUTTON)
	{
		button_rect = GUI_CreateRect(
			scrollkey_ptr->button_arr[button_idx].rect.left + TEXT_AREA_OFFSET,
			scrollkey_ptr->button_arr[button_idx].rect.top,
			scrollkey_ptr->button_arr[button_idx].rect.right,
			scrollkey_ptr->button_arr[button_idx].rect.bottom
			);
	}

    return button_rect;
}

/*****************************************************************************/
// 	Description : ����scrollkey���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetTextId(
                                    MMI_HANDLE_T    win_handle,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    MMI_TEXT_ID_T   leftscroll_id,   // the left scrollkey id
                                    MMI_TEXT_ID_T   rightscroll_id,  // the right scrollkey id
                                    BOOLEAN         is_need_update       // whether update
                                    )
{
    BOOLEAN				result  = TRUE;
	CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr = PNULL;

    //SCI_TRACE_LOW:"CTRLSCROLLKEY_SetTextId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_1452_112_2_18_3_21_14_287,(uint8*)"d", win_handle);

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);

	if (NULL == scrollkey_ptr)
	{
		return FALSE;
	}

    //reset scrollkey
    ResetScrollkey(scrollkey_ptr);

    scrollkey_ptr->button_arr[0].content.data_type = GUISCROLLKEY_DATA_TEXT_ID;
	scrollkey_ptr->button_arr[0].content.data_u_main.text_id = leftscroll_id;
    scrollkey_ptr->button_arr[1].content.data_type = GUISCROLLKEY_DATA_TEXT_ID;
	scrollkey_ptr->button_arr[1].content.data_u_main.text_id	= rightscroll_id;

    if (scrollkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawScrollkey(scrollkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : ����scrollkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonTextId(
                                          MMI_HANDLE_T    win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          MMI_TEXT_ID_T   text_id,   // the left scrollkey id
                                          GUISCROLLKEY_INDEX button_num,
                                          BOOLEAN         is_need_update       // whether update
                                          )
{
    BOOLEAN				result  = TRUE;
	CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr = PNULL;

    //SCI_TRACE_LOW:"CTRLSCROLLKEY_SetButtonTextId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_1516_112_2_18_3_21_15_288,(uint8*)"d", win_handle);

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);

	if (NULL == scrollkey_ptr)
	{
		return FALSE;
	}

    //reset scrollkey
    ResetScrollkey(scrollkey_ptr);

    scrollkey_ptr->button_arr[button_num].content.data_type = GUISCROLLKEY_DATA_TEXT_ID;
	scrollkey_ptr->button_arr[button_num].content.data_u_main.text_id = text_id;

    if (scrollkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawScrollkey(scrollkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : ����scrollkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonTextPtr(
                                           MMI_HANDLE_T    win_handle,
                                           MMI_CTRL_ID_T   ctrl_id,
                                           MMI_STRING_T    str_info,   // the left scrollkey id
                                           GUISCROLLKEY_INDEX button_num,
                                           BOOLEAN         is_need_update       // whether update
                                           )
{
    BOOLEAN				result  = TRUE;
	CTRLSCROLLKEY_OBJ_T		*scrollkey_ptr = PNULL;
    GUISCROLLKEY_DATA_TYPE_E   text_type = GUISCROLLKEY_DATA_MAX;

    //SCI_TRACE_LOW:"CTRLSCROLLKEY_SetButtonTextPtr: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_1555_112_2_18_3_21_15_289,(uint8*)"d", win_handle);

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);

	if (NULL == scrollkey_ptr)
	{
		return FALSE;
	}

    //reset scrollkey
    ResetScrollkey(scrollkey_ptr);

    // ����֮ǰ������,Ϊ������ͷ��ڴ�ʹ��
    text_type = scrollkey_ptr->button_arr[button_num].content.data_type;

    scrollkey_ptr->button_arr[button_num].content.data_type = GUISCROLLKEY_DATA_TEXT_BUFFER;
	scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_len = str_info.wstr_len;

    if (0 == str_info.wstr_len)
    {
        if (PNULL != scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr
            && text_type == GUISCROLLKEY_DATA_TEXT_BUFFER) 
        {
            // �ͷŵ�ʱ����Ҫ�ж�ԭ���������Ƿ�Ϊbuffer,�����ͷŻ��쳣��
            SCI_FREE(scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr);
            scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr = PNULL;
        }
    }
	else
	{
        scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*str_info.wstr_len);
        SCI_MEMSET(scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr, 0, sizeof(wchar)*str_info.wstr_len);
        MMIAPICOM_Wstrncpy(scrollkey_ptr->button_arr[button_num].content.data_u_main.text_buffer.wstr_ptr, str_info.wstr_ptr, str_info.wstr_len);
	}

    if (scrollkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawScrollkey(scrollkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : ����scrollkey����IconͼƬID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonIconId(
                                          MMI_HANDLE_T        win_handle,
                                          MMI_CTRL_ID_T       ctrl_id,
                                          MMI_IMAGE_ID_T      icon_id,
                                          MMI_TEXT_ID_T       text_id,
                                          GUISCROLLSKIN_INDEX skin_idx,
                                          GUISCROLLKEY_INDEX  button_num,
                                          BOOLEAN             is_need_update       // whether update
                                          )
{
    BOOLEAN				result  = TRUE;
    GUISCROLLKEY_SKIN_T skin_table = {0};
	CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr = PNULL;

    //SCI_TRACE_LOW:"CTRLSCROLLKEY_SetButtonIconId: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_1615_112_2_18_3_21_15_290,(uint8*)"d", win_handle);

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);

	if (NULL == scrollkey_ptr)
	{
		return FALSE;
	}

    //reset scrollkey
    ResetScrollkey(scrollkey_ptr);

    if (MMITHEME_CheckImageID(icon_id))
    {
        scrollkey_ptr->button_arr[button_num].content.data_type = GUISCROLLKEY_DATA_IMAGE_ID;
        scrollkey_ptr->button_arr[button_num].content.data_u_main.image_id = icon_id;
		scrollkey_ptr->button_arr[button_num].content.data_u_secondary.text_id = text_id;

        MMITHEME_GetScrollkeySkin(skin_table);

    	scrollkey_ptr->button_arr[button_num].icon_up_id        = skin_table[button_num][skin_idx].scroll_bar_up_id;
    	scrollkey_ptr->button_arr[button_num].icon_down_id 	    = skin_table[button_num][skin_idx].scroll_bar_down_id;
    	scrollkey_ptr->button_arr[button_num].icon_confirm_id   = skin_table[button_num][skin_idx].scroll_bar_confirm_id;
    	if(button_num == LEFTSCROLL_BUTTON)
    	{
    		scrollkey_ptr->button_arr[RIGHTSCROLL_BUTTON].confirm_point_id = skin_table[RIGHTSCROLL_BUTTON][skin_idx].scroll_point_id;
    	}
    	else if(button_num == RIGHTSCROLL_BUTTON)
    	{
    		scrollkey_ptr->button_arr[LEFTSCROLL_BUTTON].confirm_point_id = skin_table[LEFTSCROLL_BUTTON][skin_idx].scroll_point_id;	
    	}
    }

    if (scrollkey_ptr->is_visible && is_need_update && MMK_IsFocusWin(win_handle))
    {
        DrawScrollkey(scrollkey_ptr);
    }

    return (result);
}

/*****************************************************************************/
// Description : ��scrollkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_SetStyle(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		font_color,
                                uint32			bg_type,
                                GUI_COLOR_T		bg_color,
                                MMI_IMAGE_ID_T	bg_img
                                )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return;
    }
    

    scrollkey_ptr->bg_type    = bg_type;
    scrollkey_ptr->bg_image   = bg_img;
    scrollkey_ptr->bg_color   = bg_color;
    scrollkey_ptr->font_color = font_color;

    return;
}

/*****************************************************************************/
// Description : ����scrollkey�ı�������
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_SetBgType(
                                 MMI_HANDLE_T    win_handle,
                                 MMI_CTRL_ID_T   ctrl_id,
                                 GUI_SCROLLKEY_BG_TYPE_T bg_type
                                 )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return;
    }

    scrollkey_ptr->bg_type    = bg_type;

    return;
}

PUBLIC void CTRLSCROLLKEY_SetConfirmAnim(
                                 MMI_HANDLE_T       win_handle,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 GUISCROLLKEY_INDEX index,
                                 BOOLEAN            is_enable                                 
								 )
{
	CTRLSCROLLKEY_OBJ_T *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }
	scrollkey_ptr = GetScrollkeyPtr(win_handle, ctrl_id);

    //SCI_ASSERT(PNULL != scrollkey_ptr); /*assert verified*/
    if (PNULL == scrollkey_ptr)
    {
        return;
    }

	scrollkey_ptr->button_arr[index].is_animated = is_enable;
}

/*****************************************************************************/
// Description : �����������ɫ
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_Setfont(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		*color_ptr,
                                GUI_FONT_T      *font_ptr
                                )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return;
    }

    if (PNULL != color_ptr)
    {
        scrollkey_ptr->font_color = *color_ptr;
    }
    if (PNULL != font_ptr)
    {
        scrollkey_ptr->font = *font_ptr;
    }

    return;
}

/*****************************************************************************/
// Description : set  the valid state of the scrollkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  CTRLSCROLLKEY_SetButtonState(
                                          MMI_HANDLE_T	 win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          GUISCROLLKEY_INDEX button_index,
                                          BOOLEAN         is_gray,
                                          BOOLEAN		 is_long_press
                                          )
{
	CTRLSCROLLKEY_OBJ_T		 *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }
    SCI_ASSERT(button_index < GUISCROLLKEY_BUTTON_NUM); /*assert verified*/
    
    //SCI_TRACE_LOW:"CTRLSCROLLKEY_SetButtonState(),win_handle = %d, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISCROLLKEY_1858_112_2_18_3_21_15_291,(uint8*)"dd", win_handle, is_gray);

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return FALSE;
    }
    
    scrollkey_ptr->button_arr[button_index].is_gray = is_gray;
	scrollkey_ptr->button_arr[button_index].is_long_press = is_long_press;

    return TRUE;
}

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_GetTextId(
                                    MMI_HANDLE_T    win_handle,     //in:
                                    MMI_CTRL_ID_T   ctrl_id,        //in:
                                    MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                    MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                    )
{
    CTRLSCROLLKEY_OBJ_T	*scrollkey_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    //SCI_ASSERT((PNULL != left_id_ptr) || (PNULL != right_id_ptr)); /*assert verified*/
    if ((0 == win_handle)||((PNULL == left_id_ptr) && (PNULL == right_id_ptr)))
    {
        return FALSE;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return FALSE;
    }
    
    if (PNULL != left_id_ptr)
    {
        *left_id_ptr = scrollkey_ptr->button_arr[0].content.data_u_main.text_id;
    }
    
    if (PNULL != right_id_ptr)
    {
        *right_id_ptr = scrollkey_ptr->button_arr[1].content.data_u_main.text_id;
    }

    return TRUE;
}

/*****************************************************************************/
// Description : ��scrollkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_Update(
                              MMI_HANDLE_T    win_handle,
                              MMI_CTRL_ID_T   ctrl_id
                              )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return;
    }    

	DrawScrollkey(scrollkey_ptr);

    return;
}

/*****************************************************************************/
// Description : whether scrollkey response to tp msg
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_IsTPEnable(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id
                                     )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
     //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return FALSE;
    }
    

    return (BOOLEAN)(!scrollkey_ptr->tp_disable);
}

/*****************************************************************************/
//  Description : set tp enable or not
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetTPEnable(
                                      MMI_HANDLE_T    win_handle,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      BOOLEAN         tp_enable
                                      )
{
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return FALSE;
    }
    
    scrollkey_ptr->tp_disable = (BOOLEAN)(!tp_enable);

	return TRUE;
}

/*****************************************************************************/
//  Description : �����Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetVisible(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id,
                                     BOOLEAN         is_visible
                                     )
{
    BOOLEAN result = FALSE;
	CTRLSCROLLKEY_OBJ_T		  *scrollkey_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(NULL != win_handle); /*assert verified*/
    if (0 == win_handle)
    {
        return FALSE;
    }

	scrollkey_ptr = GetScrollkeyPtr(win_handle,ctrl_id);
    if (NULL == scrollkey_ptr)
    {
        return FALSE;
    }
    
    result = scrollkey_ptr->is_visible;
    scrollkey_ptr->is_visible = is_visible;

    UpdateScrollkeyLayer(scrollkey_ptr);

	return result;
}

#endif

