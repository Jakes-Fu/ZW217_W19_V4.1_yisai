/*****************************************************************************
** File Name:      ctrlmsgbox.c                                              *
** Author:                                                                   *
** Date:           16/04/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe message box control         *
** the use of message box:                                                   *
**  1 call GUIMSGBOX_SetAttribute function initialize message box control    *
**     when mmi initialization.                                              *
**  2 call GUIMSGBOX_Init when we need to use message box.                   *
**  3 call GUIMSGBOX_InitExt if the type of the message box control isn't    *
**     enough.                                                               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2004       panda.luo          Create                                   *
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guimsgbox.h"
#include "mmk_timer.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmi_imagefun.h"
#include "guianim.h"
#include "mmi_textfun.h"
#include "guires.h"
#include "guistring.h"

#include "ctrlmsgbox.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init msgbox class
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxInitVtbl(
                             CTRLMSGBOX_VTBL_T        *msgbox_vtbl_ptr
                             );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void MsgboxCtrlPackInitParam(
                                  GUIMSGBOX_INIT_DATA_T     *msgbox_init_ptr,   //in
                                  CTRLMSGBOX_INIT_PARAM_T   *msgbox_param_ptr   //out
                                  );

/*******************************************************************************************/
// 	Description : 这个函数用来显示消息框
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void ShowMsgbox(
                      CTRLMSGBOX_OBJ_T*	msgbox  //	需要显示的消息框
                      );

/*******************************************************************************************/
// 	Description : 这个函数用来显示sofykey栏
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void DrawSoftkey(
                       CTRLMSGBOX_OBJ_T*	msgbox_ptr  //	需要显示的消息框
                       );

/*******************************************************************************************/
// 	Description : 处理animation消息
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL MMI_RESULT_E HandleAnimationMsg( 
            void                *animate_ptr,
            MMI_MESSAGE_ID_E    msg_id,
            DPARAM              param 
            );

/*******************************************************************************************/
// 	Description : 这个函数用来显示消息框
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void DrawTitle(
                     CTRLMSGBOX_OBJ_T   *msgbox_ptr  //	需要显示的消息框
                     );

/*******************************************************************************************/
// 	Description : 此函数用于create animation control pointer of msgbox
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL BOOLEAN CreateAnimation(
                              CTRLMSGBOX_OBJ_T      *msgbox_ptr,
                              MMI_ANIM_ID_T         anima_id    //动画或图片id  
                              );

/*******************************************************************************************/
// 	Description :设置消息框的文本部分
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL GUI_RETURN_E SetMsgboxText(
                               GUIMSGBOX_TEXT_INFO_T*   text_info_ptr,   //显示的提示信息
                               MMI_STRING_T*            string_ptr       //需要显示的字符串
                               );

/*******************************************************************************************/
// 	Description : 显示文本内容；
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void ShowMsgBoxText(
						CTRLMSGBOX_OBJ_T        *msgbox_ptr,
                        GUIMSGBOX_TEXT_INFO_T   *text_info_ptr,    //文本信息
                        GUIMSGBOX_STYLE_T       style
                        );

/*******************************************************************************************/
// 	Description :计算消息框的文本区和图片区
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL GUI_RETURN_E AssignMsgBoxArea(
                                    CTRLMSGBOX_OBJ_T*   msgbox_ptr,   //消息框控件
                                    GUIMSGBOX_STYLE_T	msgbox_style, //给出动画和文本的位置关系, 
            	                    GUIMSGBOX_IMAGE_E   image_type,   //标明当前为动画或图片                                    
                                    MMI_IMAGE_ID_T      image_id,     //图片id
                                    MMI_ANIM_ID_T       anim_id,      //动画id
                                    GUI_RECT_T          client_area   //消息框的客户区
                                    );

/*******************************************************************************************/
// 	Description : 处理消息框失去焦点事件
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void HandleMsgBoxLoseFocus(
                                 CTRLMSGBOX_OBJ_T   *msgbox_ptr //	消息框
                                 );

/*******************************************************************************************/
// 	Description :Set text paramter
//	Global resource dependence :
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
LOCAL void ArrangeTextRect(
                      GUIMSGBOX_TEXT_INFO_T* text_info_ptr
                      );


/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的image id 或者anima id
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL void SetImageAnim(
                        CTRLMSGBOX_OBJ_T    *msgbox_ptr,
                        GUIMSGBOX_IMAGE_E   image_type,  //标明当前为动画或图片
                        MMI_IMAGE_ID_T      image_id,     //动画或图片id  
                        MMI_ANIM_ID_T       anim_id,     //动画或图片id  
                        GUI_RECT_T          *rect_ptr
                        );

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxCtrlConstruct(
                                  CTRLBASE_OBJ_T            *base_ctrl_ptr,
                                  CTRLMSGBOX_INIT_PARAM_T   *msgbox_param_ptr
                                  );

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxCtrlDestruct(
                                 CTRLBASE_OBJ_T    *base_ctrl_ptr
                                 );

/**********************************************************************************/
/*!\brief  This function is used to handle message of msgbox
 * \author Jibin
 * \param[in]  *ctrl_ptr 	control ptr
 * \param[in]  msg_id	 	message id
 * \param[in]  param 		paramter
 */
/**********************************************************************************/
LOCAL MMI_RESULT_E MsgboxCtrlHandleMsg(
                                       CTRLBASE_OBJ_T       *ctrl_ptr,
                                       MMI_MESSAGE_ID_E 	msg_id,
                                       DPARAM           	param
                                       );

/****************************************************************************/
/*!\brief  This function is used to init message box,default value will be set if no setting attribute
* \author panda.luo
* \param[in]  style 	style of message windown	
* \param[in]  lcd_id	which lcd will display on
* \param[in]  *rect_ptr rect of message frame control.if PNULL,control default define will instead
* \return faile or succeed  
*	- <i><c> =PNULL</c></i>:	faile
*	- <i><c>!=PNULL</c></i>:	succeed
*/
/*****************************************************************************/
LOCAL void GUIMSGBOX_Init(  
                          CTRLBASE_OBJ_T *ctrl_ptr,
                          GUIMSGBOX_STYLE_T   style,
                          uint8               lcd_id,
                          GUI_RECT_T			*rect_ptr
                          );

/*****************************************************************************/
//  Description : Get msgbox list pointer from control id
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLMSGBOX_OBJ_T* GetMsgBoxPtr(
                                     MMI_HANDLE_T ctrl_handle
                                     );

/*****************************************************************************/
//  Description : msgbox type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN MsgBoxTypeOf(
                           CTRLBASE_OBJ_T* msgbox_ptr
                           );

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------*
 **                                     FUNCTION DEFINITION                                 *
 **----------------------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get msgbox type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MSGBOX_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "msgbox",
            sizeof(CTRLMSGBOX_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)MsgboxCtrlConstruct,
            (OBJECT_DESTRUCTOR_FUNC)MsgboxCtrlDestruct,
            sizeof(CTRLMSGBOX_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)MsgboxCtrlPackInitParam,
            sizeof(CTRLMSGBOX_VTBL_T),
            (VTBL_INIT_FUNC)MsgboxInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init msgbox class
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxInitVtbl(
                             CTRLMSGBOX_VTBL_T        *msgbox_vtbl_ptr
                             )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)msgbox_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = MsgboxCtrlHandleMsg;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void MsgboxCtrlPackInitParam(
                                  GUIMSGBOX_INIT_DATA_T     *msgbox_init_ptr,   //in
                                  CTRLMSGBOX_INIT_PARAM_T   *msgbox_param_ptr   //out
                                  )
{
    msgbox_param_ptr->style = msgbox_init_ptr->style;
    msgbox_param_ptr->lcd_id = msgbox_init_ptr->lcd_id;
    msgbox_param_ptr->rect_ptr = msgbox_init_ptr->rect_ptr;
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxCtrlConstruct(
                                  CTRLBASE_OBJ_T            *base_ctrl_ptr,
                                  CTRLMSGBOX_INIT_PARAM_T   *msgbox_param_ptr
                                  )
{
    if ( PNULL == base_ctrl_ptr || PNULL == msgbox_param_ptr )
    {
        return FALSE;
    }

    GUIMSGBOX_Init(
        base_ctrl_ptr,
        msgbox_param_ptr->style,
        msgbox_param_ptr->lcd_id,
        msgbox_param_ptr->rect_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgboxCtrlDestruct(
                                 CTRLBASE_OBJ_T    *base_ctrl_ptr
                                 )
{
    CTRLMSGBOX_OBJ_T *msgbox_ptr = (CTRLMSGBOX_OBJ_T *)base_ctrl_ptr;

    //停止定时器；
    HandleMsgBoxLoseFocus(msgbox_ptr);

	//释放文本占用的空间
    SCI_FREE(msgbox_ptr->text_info.string.wstr_ptr);
    msgbox_ptr->text_info.string.wstr_ptr = PNULL;
    
    return TRUE;
}

/*******************************************************************************************/
// 	Description : 此函数用于对消息框作简单的初始化操作，没有设置的属性用缺省值代替。
//	Global resource dependence : g_guimsgbox_type
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void GUIMSGBOX_Init(  
                          CTRLBASE_OBJ_T      *ctrl_ptr,
                          GUIMSGBOX_STYLE_T   style,
                          uint8               lcd_id,
                          GUI_RECT_T          *rect_ptr
                          )
{
    CTRLMSGBOX_OBJ_T    *msgbox_ptr = (CTRLMSGBOX_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;
    MMI_MSGBOX_STYLE_T  msgbox_style = {0};

	//分配控件空间；
    SCI_ASSERT(PNULL != rect_ptr ); /*assert verified*/
	
    GUI_CHECK_RECT(*rect_ptr);
	base_ctrl_ptr->lcd_dev_info = *(MMITHEME_GetDefaultLcdDev());
    base_ctrl_ptr->rect = *rect_ptr;
    //msgbox_ptr->type = (uint8)(TYPE_MSGBOX);
    msgbox_ptr->style = style;
    base_ctrl_ptr->lcd_dev_info.lcd_id = (GUI_LCD_ID_E)lcd_id;

    msgbox_ptr->text_info.text_rect = *rect_ptr;

    msgbox_ptr->ani_timeout = GUIMSGBOX_ANIMA_TIMEOUT; 
    msgbox_ptr->win_id = MMK_GetWinHandleByCtrl( base_ctrl_ptr->handle );
    if (MMITHEME_GetMsgBoxStyle(&msgbox_style))
    {
        msgbox_ptr->back_color              = msgbox_style.back_color;
        msgbox_ptr->back_image_id           = msgbox_style.back_image_id;
        msgbox_ptr->img_hspace              = msgbox_style.img_hspace;
        msgbox_ptr->img_vspace              = msgbox_style.img_vspace;
        msgbox_ptr->title_color             = msgbox_style.title_color;
        msgbox_ptr->title_height            = msgbox_style.title_height;
        msgbox_ptr->text_info.back_color    = msgbox_style.text_back_color;
        msgbox_ptr->text_info.font          = msgbox_style.font;
        msgbox_ptr->text_info.font_color    = msgbox_style.font_color;
        msgbox_ptr->text_info.move_line     = msgbox_style.move_line;
        msgbox_ptr->text_info.space_char    = msgbox_style.char_space;
        msgbox_ptr->text_info.space_line    = msgbox_style.line_space;
        msgbox_ptr->image_rect              = msgbox_style.image_rect;
        msgbox_ptr->text_info.text_rect     = msgbox_style.text_rect;
        msgbox_ptr->sk_rect                 = msgbox_style.sk_rect;
        msgbox_ptr->sk_bg_color             = msgbox_style.sk_bg_color;        
        msgbox_ptr->softkey_id[0]           = msgbox_style.sk_text_id[0];
        msgbox_ptr->softkey_id[1]           = msgbox_style.sk_text_id[1];
        msgbox_ptr->softkey_id[2]           = msgbox_style.sk_text_id[2];
        msgbox_ptr->title_text_id           = msgbox_style.title_text_id;
    }
    else
    {
		SCI_PASSERT(FALSE,("GUIMSGBOX_Init: get msg box style error!")); /*assert verified*/
    }
}

/*******************************************************************************************/
// 	Description : 此函数用于对消息框作简单的初始化操作，没有设置的属性用缺省值代替。
//	Global resource dependence : g_guimsgbox_type
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetParam(   
                        MMI_CTRL_ID_T	            ctrl_id,
                        MMI_TEXT_ID_T       title_text_id,     //title text id
	                    GUIMSGBOX_IMAGE_E   image_type,        //标明当前为动画或图片
                        MMI_IMAGE_ID_T      image_id,          //动画或图片id 
                        MMI_ANIM_ID_T       anim_id,           //动画id                          
                        GUI_RECT_T*         image_rect_ptr,
	                    MMI_STRING_T*		 string_ptr,        //提示内容，不允许等于PNULL
                        GUI_RECT_T*         text_rect_ptr,
                        MMI_TEXT_ID_T       left_sk_id,
	                    MMI_TEXT_ID_T       right_sk_id
                        )
{
    CTRLMSGBOX_OBJ_T    *msgbox_ptr = GetMsgBoxPtr(ctrl_id);
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;

    if (PNULL == msgbox_ptr || PNULL == string_ptr)
    {
        return;
    }

    if (!MMI_IsTextNull(left_sk_id) || !MMI_IsTextNull(right_sk_id))
    {
        msgbox_ptr->softkey_id[0] = left_sk_id;
        msgbox_ptr->softkey_id[GUISOFTKEY_BUTTON_NUM-1] = right_sk_id;
    }

    SetImageAnim(msgbox_ptr, image_type, image_id, anim_id, image_rect_ptr);

    SetMsgboxText(&msgbox_ptr->text_info,string_ptr);
    if ((msgbox_ptr->style & GUIMSGBOX_BITMAP_MASK) == GUIMSGBOX_STYLE_CUSTOMED) 
    {
        if (PNULL != text_rect_ptr)
        {
            msgbox_ptr->text_info.text_rect = *text_rect_ptr;
        }
        ArrangeTextRect(&msgbox_ptr->text_info);
    }

    if (!MMI_IsTextNull(title_text_id))
    {
        msgbox_ptr->title_text_id = title_text_id;
        base_ctrl_ptr->rect.top = (int16)(base_ctrl_ptr->rect.top + msgbox_ptr->title_height);
    }

    if ((msgbox_ptr->style & GUIMSGBOX_BITMAP_MASK) != GUIMSGBOX_STYLE_CUSTOMED) 
    {
        GUIMSGBOX_Layout(ctrl_id);
    }
}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的image id 或者anima id
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL void SetImageAnim(
                        CTRLMSGBOX_OBJ_T    *msgbox_ptr,
                        GUIMSGBOX_IMAGE_E   image_type,  //标明当前为动画或图片
                        MMI_IMAGE_ID_T      image_id,     //动画或图片id  
                        MMI_ANIM_ID_T       anim_id,     //动画或图片id  
                        GUI_RECT_T          *rect_ptr
                        )
{
    uint16      img_width = 0;
    uint16      img_height = 0;
    GUI_RECT_T  img_rect, area_rect;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != msgbox_ptr); /*assert verified*/
    if (PNULL == msgbox_ptr)
    {
        return;
    }
    SCI_ASSERT(image_type < GUIMSGBOX_IMAGE_INVALID); /*assert verified*/

    if ((msgbox_ptr->style & GUIMSGBOX_BITMAP_MASK) == GUIMSGBOX_STYLE_CUSTOMED) 
    {
        if (PNULL != rect_ptr)
        {
            area_rect = *rect_ptr;            
        }
        else
        {
            area_rect = msgbox_ptr->image_rect;
        }
        
        switch (image_type)
        {
        case GUIMSGBOX_IMAGE_NONE:
            break;

        case GUIMSGBOX_IMAGE_ANIMA:
            GUIRES_GetAnimWidthHeight(&img_width,&img_height,PNULL,anim_id,msgbox_ptr->win_id);
            break;

        case GUIMSGBOX_IMAGE_BMP:
            GUIRES_GetImgWidthHeight(&img_width,&img_height,image_id,msgbox_ptr->win_id);
            break;

        default:
            break;
        }
    
        img_rect.left   = (int16)((area_rect.right + area_rect.left - img_width) / 2); 
        img_rect.right  = (int16)(img_rect.left + img_width);
        img_rect.top    = (int16)((area_rect.bottom + area_rect.top - img_height) / 2);
        img_rect.bottom = (int16)(img_rect.top + img_height);

        msgbox_ptr->image_rect = img_rect;        
    }

    msgbox_ptr->image_type = image_type;
    switch (image_type)
    {
    case GUIMSGBOX_IMAGE_NONE:
        msgbox_ptr->animate_ptr = PNULL;
        msgbox_ptr->image_id = 0;
        break;

    case GUIMSGBOX_IMAGE_ANIMA:
        CreateAnimation(msgbox_ptr, anim_id);
        HandleAnimationMsg(msgbox_ptr->animate_ptr, MSG_CTL_OPEN, PNULL);
        msgbox_ptr->anim_id = anim_id;
        break;

    case GUIMSGBOX_IMAGE_BMP:
        msgbox_ptr->animate_ptr = PNULL;
        msgbox_ptr->image_id = image_id;
        break;

    default:
		SCI_PASSERT(FALSE,("SetImageAnim: image_type %d is error!",image_type)); /*assert verified*/
        break;
    }
}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的image id
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetImageAnim(   //返回PNULL，表示初始化失败，不等于PNULL，表示初始化成功。
                            MMI_CTRL_ID_T	            ctrl_id,
    	                    GUIMSGBOX_IMAGE_E   image_type,  //标明当前为动画或图片
                            MMI_IMAGE_ID_T      image_id,     //图片id  
                            MMI_ANIM_ID_T       anim_id,     //动画id  
                            GUI_RECT_T          *rect_ptr
    	                    )
{
    CTRLMSGBOX_OBJ_T* msgbox_ptr = GetMsgBoxPtr(ctrl_id);

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    SetImageAnim(msgbox_ptr, image_type, image_id, anim_id, rect_ptr);

}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的提示文本
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetText(   
                        MMI_CTRL_ID_T	ctrl_id,
	                    MMI_STRING_T*		string_ptr,         //提示内容，不允许等于PNULL
                        GUI_RECT_T*         rect_ptr
                      )
{
    CTRLMSGBOX_OBJ_T* msgbox_ptr = GetMsgBoxPtr(ctrl_id);

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    	//初始化文本结构；
    SetMsgboxText(&msgbox_ptr->text_info,string_ptr);

    if ((msgbox_ptr->style & GUIMSGBOX_BITMAP_MASK) == GUIMSGBOX_STYLE_CUSTOMED) 
    {
        if (PNULL != rect_ptr)
        {
            msgbox_ptr->text_info.text_rect = *rect_ptr;
        }
        ArrangeTextRect(&msgbox_ptr->text_info);
    }
}


/*******************************************************************************************/
// 	Description : 此函数用于create animation control pointer of msgbox
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL BOOLEAN CreateAnimation(
                              CTRLMSGBOX_OBJ_T  *msgbox_ptr,
                              MMI_ANIM_ID_T     anima_id    //动画或图片id  
                              )
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_CONTROL_CREATE_T create = {0};
    GUIANIM_INIT_DATA_T  init_data = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(0 != anima_id); /*assert verified*/
    if (0 == anima_id)
    {
        return FALSE;
    }

    init_data.both_rect.h_rect = init_data.both_rect.v_rect = msgbox_ptr->image_rect;
    create.ctrl_id = 0;
    create.guid    = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = msgbox_ptr->win_id;
    create.init_data_ptr = &init_data;
    msgbox_ptr->animate_ptr = MMK_CreateControl( &create );

    SCI_ASSERT(PNULL != msgbox_ptr->animate_ptr); /*assert verified*/

    control_info.is_ctrl_id = FALSE;
    control_info.ctrl_id = 0;
    control_info.ctrl_ptr = msgbox_ptr->animate_ptr;

    data_info.anim_id = anima_id;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.is_update = FALSE;

    //set anim parameter
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    
    return TRUE;
}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的背景颜色或图片
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetBackground(   
                        MMI_CTRL_ID_T	ctrl_id,
                        MMI_IMAGE_ID_T         image_id,     //图片id  
                        GUI_COLOR_T         back_color    //背景颜色
	                   )
{
    CTRLMSGBOX_OBJ_T* msgbox_ptr = GetMsgBoxPtr(ctrl_id);
    
    if (PNULL == msgbox_ptr)
    {
        return;
    }
    if (0 != image_id)
    {
        msgbox_ptr->back_image_id = image_id;        
    }
    else
    {
        msgbox_ptr->back_color = back_color;
        msgbox_ptr->back_image_id = 0;
    }
 
   GUIMSGBOX_Update(ctrl_id);    //重新刷新
}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的标题
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetTitleText(   
                        MMI_CTRL_ID_T	ctrl_id,
                        MMI_TEXT_ID_T       title_text_id           //title text id
	                   )
{
    CTRLMSGBOX_OBJ_T    *msgbox_ptr = GetMsgBoxPtr(ctrl_id);
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    msgbox_ptr->title_text_id = title_text_id;
    
    base_ctrl_ptr->rect.top = (int16)(base_ctrl_ptr->rect.top + msgbox_ptr->title_height);
}


/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的标题
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetTitleParam(   
                        MMI_CTRL_ID_T	ctrl_id,
                        uint8               height,   
                        GUI_COLOR_T         color   
	                   )
{
    CTRLMSGBOX_OBJ_T* msgbox_ptr = GetMsgBoxPtr(ctrl_id);
  
    if (PNULL == msgbox_ptr)
    {
        return;
    }
    msgbox_ptr->title_height = height;
    msgbox_ptr->title_color = color;
}

/*******************************************************************************************/
// 	Description : 此函数用于设置消息框的softkey
//	Global resource dependence : 
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetSoftkey(   
                          MMI_CTRL_ID_T	        ctrl_id,
                          MMI_TEXT_ID_T     left_sk_id,
	                      MMI_TEXT_ID_T     right_sk_id,
                          GUI_RECT_T*       sk_rect_ptr,
                          GUI_COLOR_T       sk_bg_color
                          )
{
    CTRLMSGBOX_OBJ_T* msgbox_ptr = GetMsgBoxPtr(ctrl_id);

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    msgbox_ptr->softkey_id[0] = left_sk_id;
    msgbox_ptr->softkey_id[GUISOFTKEY_BUTTON_NUM -1] = right_sk_id;

    if(PNULL != sk_rect_ptr)
    {
        msgbox_ptr->sk_rect = *sk_rect_ptr;
    }

    if(0 != sk_bg_color)
    {
        msgbox_ptr->sk_bg_color = sk_bg_color;
    }
}


/*******************************************************************************************/
// 	Description :设置消息框的文本部分
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL GUI_RETURN_E SetMsgboxText(
                               GUIMSGBOX_TEXT_INFO_T*   text_info_ptr,   //显示的提示信息
                               MMI_STRING_T*            string_ptr       //需要显示的字符串
                               )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != text_info_ptr); /*assert verified*/
    if (PNULL == text_info_ptr)
    {
        return GUI_RET_ERR_PARAM;
    }
   
    if(PNULL == string_ptr)
    {
        text_info_ptr->string.wstr_len = 0;
        text_info_ptr->string.wstr_ptr = PNULL;
        return GUI_RET_SUCCESS;
    }
    
    text_info_ptr->string.wstr_len = string_ptr->wstr_len;
    text_info_ptr->string.wstr_ptr  = SCI_ALLOCA( ( string_ptr->wstr_len + 1 ) * sizeof(wchar) );
    //kevin.lou modified:delete assert
    SCI_ASSERT(PNULL != text_info_ptr->string.wstr_ptr); /*assert verified*/
    if (PNULL == text_info_ptr->string.wstr_ptr)
    {
        text_info_ptr->string.wstr_len = 0;        
        return GUI_RET_SUCCESS;
    }
    

    MMI_WSTRNCPY( text_info_ptr->string.wstr_ptr, string_ptr->wstr_len, 
        string_ptr->wstr_ptr, string_ptr->wstr_len, string_ptr->wstr_len );

    return GUI_RET_SUCCESS;
}


/*******************************************************************************************/
// 	Description : Set the position of title, image, and text
//	Global resource dependence : 
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_Layout( 
                     MMI_CTRL_ID_T	ctrl_id
	                  )
{
    CTRLMSGBOX_OBJ_T    *msgbox_ptr = GetMsgBoxPtr(ctrl_id);     //消息框控件
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;
    GUI_RECT_T          client_area = {0};

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    client_area = base_ctrl_ptr->rect;
   
    if (!MMI_IsTextNull(msgbox_ptr->title_text_id))
    {
        client_area.top = (int16)(client_area.top + msgbox_ptr->title_height);
    }

    if ((!MMI_IsTextNull(msgbox_ptr->softkey_id[0])) || (!MMI_IsTextNull(msgbox_ptr->softkey_id[GUISOFTKEY_BUTTON_NUM -1])))
    {
          client_area.bottom = (int16)(client_area.bottom - msgbox_ptr->sk_rect.bottom + msgbox_ptr->sk_rect.top);
    }

    if (0 != msgbox_ptr->image_id)
    {
        AssignMsgBoxArea(msgbox_ptr, msgbox_ptr->style, msgbox_ptr->image_type, msgbox_ptr->image_id, msgbox_ptr->anim_id, client_area);
        if (PNULL != msgbox_ptr->animate_ptr)
        {
            GUIANIM_SetCtrlRectByPtr(msgbox_ptr->animate_ptr,&msgbox_ptr->image_rect);
        }
    }
    else
    {
        msgbox_ptr->text_info.text_rect = client_area;
    }

    ArrangeTextRect(&msgbox_ptr->text_info);
}

/*******************************************************************************************/
// 	Description :Set text paramter
//	Global resource dependence :
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
LOCAL void ArrangeTextRect(
                      GUIMSGBOX_TEXT_INFO_T* text_info_ptr
                      )
{
    uint16 height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != text_info_ptr); /*assert verified*/
    if (PNULL == text_info_ptr)
    {
        return;
    }

    if(PNULL == text_info_ptr->string.wstr_ptr)
    {
        return;
    }
    
	height = GUI_GetFontHeight(text_info_ptr->font, UNICODE_HANZI);

    text_info_ptr->first_line   = 0;
    text_info_ptr->max_lines    = (uint8)((text_info_ptr->text_rect.bottom - text_info_ptr->text_rect.top)/
        (height + text_info_ptr->space_line));

    
    //计算总的显示行数,暂用移软的函数
    text_info_ptr->total_lines = (uint8)GUI_CalculateStringLinesByPixelNum(
                                                    (uint16)(text_info_ptr->text_rect.right - text_info_ptr->text_rect.left),
                                                     text_info_ptr->string.wstr_ptr,
                                                     text_info_ptr->string.wstr_len,
                                                    //text_info_ptr->string.is_ucs2,
                                                    text_info_ptr->font,
                                                    (uint8)(text_info_ptr->space_char),
                                                    TRUE
                                                    );
        
    //判断是否需要滚动显示
    if(text_info_ptr->total_lines > text_info_ptr->max_lines)
    {
        //需要滚动
        text_info_ptr->text_timeout = GUIMSGBOX_TEXT_MOVE_TIMEOUT;
        text_info_ptr->is_timer_need = TRUE;
    }
    else
    {
        text_info_ptr->is_timer_need = FALSE;
    }    
}


/*******************************************************************************************/
// 	Description :计算消息框的文本区和图片区
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL GUI_RETURN_E AssignMsgBoxArea(
                                    CTRLMSGBOX_OBJ_T    *msgbox_ptr,   //消息框控件
                                    GUIMSGBOX_STYLE_T	msgbox_style, //给出动画和文本的位置关系, 
            	                    GUIMSGBOX_IMAGE_E   image_type,   //标明当前为动画或图片                                    
                                    MMI_IMAGE_ID_T      image_id,     //图片id
                                    MMI_ANIM_ID_T       anim_id,      //动画id
                                    GUI_RECT_T          client_area   //消息框的客户区
                                    )
{
    uint16      icon_width = 0,icon_height = 0; //动画或图片的长和宽
    GUI_RECT_T  bitmap_rect = {0,0,0,0};     //放图片的区域
    uint16      large = (uint16)(msgbox_style & GUIMSGBOX_BITMAP_LARGE); 

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != msgbox_ptr); /*assert verified*/
    if (PNULL == msgbox_ptr)
    {
        return GUI_RET_ERR_PARAM;
    }
    
    GUI_CHECK_RECT(client_area);
    GUI_INVALID_PARAM(image_type);/*lint !e522*/

    //获得动画或图片的长和宽
    switch (msgbox_ptr->image_type)
    {
    case GUIMSGBOX_IMAGE_BMP:
        GUIRES_GetImgWidthHeight(&icon_width,&icon_height,image_id,msgbox_ptr->win_id);
        break;
    case GUIMSGBOX_IMAGE_ANIMA:
        GUIRES_GetAnimWidthHeight(&icon_width,&icon_height,PNULL,anim_id,msgbox_ptr->win_id);
        break;
        
    default:
        break;
    }    

    switch(msgbox_style & GUIMSGBOX_BITMAP_MASK)
    {
    case GUIMSGBOX_BITMAP_BOTTOM:
        bitmap_rect.bottom = client_area.bottom;
        bitmap_rect.top = (int16)(client_area.bottom - icon_height - 2 * msgbox_ptr->img_vspace);
        if(large)
        {
            bitmap_rect.left    = client_area.left;
            bitmap_rect.right   = client_area.right;
        }
        else
        {
            bitmap_rect.left    = (int16)((client_area.right + client_area.left - icon_width)/2);
            bitmap_rect.right   = (int16)(bitmap_rect.left  + icon_width);
        }

        //计算消息框的文本显示区域
        msgbox_ptr->text_info.text_rect.left    = client_area.left;
        msgbox_ptr->text_info.text_rect.right   = client_area.right;
        msgbox_ptr->text_info.text_rect.top     = client_area.top;
        msgbox_ptr->text_info.text_rect.bottom  = bitmap_rect.top;
        break;

    case GUIMSGBOX_BITMAP_TOP:
        bitmap_rect.top = (int16)(client_area.top + msgbox_ptr->img_vspace);
        bitmap_rect.bottom = (int16)(bitmap_rect.top + icon_height) ;
        if(large)
        {
            bitmap_rect.left    = client_area.left;
            bitmap_rect.right   = client_area.right;
        }
        else
        {
            //居中显示
            bitmap_rect.left    = (int16)((client_area.right + client_area.left - icon_width)/2);
            bitmap_rect.right   = (int16)(bitmap_rect.left  + icon_width);
        }

        //计算消息框的文本显示区域
        msgbox_ptr->text_info.text_rect.left    = client_area.left;
        msgbox_ptr->text_info.text_rect.right   = client_area.right;
        msgbox_ptr->text_info.text_rect.top     = bitmap_rect.bottom;
        msgbox_ptr->text_info.text_rect.bottom  = client_area.bottom;
        break;

    case GUIMSGBOX_BITMAP_RIGHT:
        bitmap_rect.right = client_area.right ;
        bitmap_rect.left = (int16)(client_area.right - icon_width - 2 * msgbox_ptr->img_hspace);
        if(large)
        {
            bitmap_rect.top     = client_area.top;
            bitmap_rect.bottom  = client_area.bottom;
        }
        else
        {
            bitmap_rect.top     = (int16)((client_area.top + client_area.bottom - icon_height)/2);
            bitmap_rect.bottom  = (int16)(bitmap_rect.top + icon_height);
        }

        //计算消息框的文本显示区域
        msgbox_ptr->text_info.text_rect.left    = client_area.left;
        msgbox_ptr->text_info.text_rect.right   = bitmap_rect.left;
        msgbox_ptr->text_info.text_rect.top     = client_area.top;
        msgbox_ptr->text_info.text_rect.bottom  = client_area.bottom;
        break;

    case GUIMSGBOX_BITMAP_LEFT:
        bitmap_rect.left = client_area.left;
        bitmap_rect.left = (int16)(client_area.right + icon_width + 2 * msgbox_ptr->img_hspace);

        if(large)
        {
            bitmap_rect.top     = client_area.top;
            bitmap_rect.bottom  = client_area.bottom;
        }
        else
        {
            bitmap_rect.top     = (int16)((client_area.top + client_area.bottom - icon_height)/2);
            bitmap_rect.bottom  = (int16)(bitmap_rect.top + icon_height);
        }

        //计算消息框的文本显示区域
        msgbox_ptr->text_info.text_rect.left    = bitmap_rect.right;
        msgbox_ptr->text_info.text_rect.right   = client_area.right;
        msgbox_ptr->text_info.text_rect.top     = client_area.top;
        msgbox_ptr->text_info.text_rect.bottom  = client_area.bottom;
        break;

    default:
        break;
    }
    
    GUI_CHECK_RECT(bitmap_rect);
    GUI_CHECK_RECT(msgbox_ptr->text_info.text_rect);

    msgbox_ptr->image_rect = bitmap_rect;

    return GUI_RET_SUCCESS;
}

/*****************************************************************************/
// 	Description : msgbox的消息处理
//	Global resource dependence : 
//  Author: Jibin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MsgboxCtrlHandleMsg(
                                       CTRLBASE_OBJ_T       *ctrl_ptr,
                                       MMI_MESSAGE_ID_E 	msg_id,
                                       DPARAM           	param
                                       )
{
    CTRLMSGBOX_OBJ_T    *msgbox_ptr = (CTRLMSGBOX_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;
    MMI_RESULT_E        return_value = MMI_RESULT_TRUE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != msgbox_ptr); /*assert verified*/
    if (PNULL == msgbox_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch( msg_id )
    {
    case  MSG_CTL_OPEN:
        //控件获得焦点
        msgbox_ptr->is_focus = TRUE;
        break;

    case MSG_CTL_GET_ACTIVE:
        //控件获得焦点;
        msgbox_ptr->is_focus	= TRUE;
        msgbox_ptr->is_active	= TRUE;
        
        if(msgbox_ptr->text_info.is_timer_need && msgbox_ptr->text_info.text_timer == 0)
        {
            msgbox_ptr->text_info.text_timer  = MMK_CreateWinTimer( base_ctrl_ptr->handle, msgbox_ptr->text_info.text_timeout, 0 );
            SCI_ASSERT(0 != msgbox_ptr->text_info.text_timer); /*assert verified*/
        }
        break;

    case MSG_CTL_GET_FOCUS:
        //控件获得焦点;
        msgbox_ptr->is_focus	= TRUE;
        msgbox_ptr->is_active	= TRUE;
        
        if(msgbox_ptr->text_info.is_timer_need && msgbox_ptr->text_info.text_timer == 0)
        {
            msgbox_ptr->text_info.text_timer  = MMK_CreateWinTimer( base_ctrl_ptr->handle, msgbox_ptr->text_info.text_timeout, 0);
            SCI_ASSERT(0 != msgbox_ptr->text_info.text_timer); /*assert verified*/
        }
        break;

    case  MSG_TIMER:
        //if(文本滚动定时器到时)		//只有在设置了滚动模式下才会出现
        SCI_ASSERT(PNULL != param); /*assert verified*/
        if(msgbox_ptr->text_info.text_timer == *( GUI_TIMER_ID_T*)param )
        {
            //    文本上移；		//可能移动多行
            GUIMSGBOX_TEXT_INFO_T* text_info_ptr = &msgbox_ptr->text_info;

            text_info_ptr->first_line = (uint16)(text_info_ptr->first_line + text_info_ptr->move_line); 
            if(text_info_ptr->first_line> text_info_ptr->total_lines)
            {
                text_info_ptr->first_line = 0;
            }
            ShowMsgbox(msgbox_ptr);
            /* please use control handle here */
            MMK_StartWinTimer( base_ctrl_ptr->handle, text_info_ptr->text_timer, text_info_ptr->text_timeout, 0 );            
        }
        else
        {
            return_value = MMI_RESULT_FALSE;
        }
         break;

    case  MSG_CTL_PAINT:
        ShowMsgbox(msgbox_ptr);
        break;

    case  MSG_APP_OK:		//确认键
        //return_value = GUI_RESULT_CTLOK;
        return_value = CTRLMSG_PostNotify( base_ctrl_ptr->handle, MSG_NOTIFY_OK );
        break;

    case  MSG_APP_CANCEL:		//取消键
        return_value = CTRLMSG_PostNotify( base_ctrl_ptr->handle, MSG_NOTIFY_CANCEL );
        //return_value = GUI_RESULT_CTLCANCEL;
        break;

    default:
        return MMI_RESULT_FALSE;
    }

    if (PNULL != msgbox_ptr->animate_ptr)
    {
        HandleAnimationMsg(msgbox_ptr->animate_ptr, msg_id, param );
    }

    return return_value;
}

/*******************************************************************************************/
// 	Description : 处理animation消息
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*********************************************************************************************/
LOCAL MMI_RESULT_E HandleAnimationMsg( 
            void                *animate_ptr,
            MMI_MESSAGE_ID_E    msg_id,
            DPARAM              param 
            )
{
    return (IGUICTRL_HandleEvent( (IGUICTRL_T*)animate_ptr, msg_id, param));
}

/*******************************************************************************************/
// 	Description : 应用程序调用这个函数用来显示消息框
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_Update(   //成功返回TRUE，失败返回FALSE。
                     MMI_CTRL_ID_T ctrl_id //需要显示的消息框
                     )
{
	//获得消息框的指针
    CTRLMSGBOX_OBJ_T *msgbox_ptr = GetMsgBoxPtr(ctrl_id);
    if (PNULL == msgbox_ptr)
    {
        return;
    }
    
	ShowMsgbox(msgbox_ptr);
}

/*******************************************************************************************/
// 	Description : 这个函数用来显示消息框
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void DrawTitle(
                     CTRLMSGBOX_OBJ_T   *msgbox_ptr  //	需要显示的消息框
                     )
{
    GUI_RECT_T      title_rect = {0,0,0,0};
    MMI_STRING_T    title_text = {0};
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != msgbox_ptr); /*assert verified*/
    if (PNULL == msgbox_ptr)
    {
        return;
    }
    
    SCI_ASSERT(0 != msgbox_ptr->title_height); /*assert verified*/    

    title_rect = base_ctrl_ptr->rect;
    title_rect.bottom = msgbox_ptr->title_height;

    LCD_FillRect(&base_ctrl_ptr->lcd_dev_info, title_rect, msgbox_ptr->title_color);

    MMITHEME_GetResText( msgbox_ptr->title_text_id, msgbox_ptr->win_id, &title_text );

    str_style.align = ALIGN_HVMIDDLE;
    str_style.line_space = 0;
    str_style.char_space = 0;
    str_style.font = msgbox_ptr->text_info.font;
    str_style.font_color = msgbox_ptr->text_info.font_color;

    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO*)&(base_ctrl_ptr->lcd_dev_info),
            (const GUI_RECT_T*)&title_rect,       //the fixed display area
            (const GUI_RECT_T*)&title_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T*)&title_text,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO
            );
}

/*******************************************************************************************/
// 	Description : 这个函数用来显示sofykey栏
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void DrawSoftkey(
                CTRLMSGBOX_OBJ_T*	msgbox_ptr  //	需要显示的消息框
                )
{
#ifndef MMI_PDA_SUPPORT	
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != msgbox_ptr); /*assert verified*/
    if (PNULL == msgbox_ptr)
    {
        return;
    }

	GUIWIN_SetSoftkeyTextId(msgbox_ptr->win_id,
						msgbox_ptr->softkey_id[0],
						msgbox_ptr->softkey_id[1],
						msgbox_ptr->softkey_id[2],
						TRUE);
#endif
}

/*******************************************************************************************/
// 	Description : 这个函数用来显示消息框
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void ShowMsgbox(
                CTRLMSGBOX_OBJ_T*	msgbox_ptr  //	需要显示的消息框
                )
{
    GUI_POINT_T         dis_point = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;

    if(PNULL == msgbox_ptr)
    {
        //SCI_TRACE_MID:"gui warning:ShowMsgbox: msgbox_ptr = PNULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMSGBOX_1200_112_2_18_3_20_34_249,(uint8*)"");
        return;
    }

    if (0 == msgbox_ptr->back_image_id)
    {
        LCD_FillRect(&base_ctrl_ptr->lcd_dev_info, base_ctrl_ptr->rect, msgbox_ptr->back_color);
    }
    else
    {
        dis_point.x = base_ctrl_ptr->rect.left;
        dis_point.y = base_ctrl_ptr->rect.top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            msgbox_ptr->win_id,
            msgbox_ptr->back_image_id,
            &base_ctrl_ptr->lcd_dev_info);
    }

    if (!MMI_IsTextNull(msgbox_ptr->title_text_id))
    {
        DrawTitle(msgbox_ptr);
    }
	//显示文本内容；
    ShowMsgBoxText(msgbox_ptr, &msgbox_ptr->text_info, msgbox_ptr->style);

	//显示图片或动画；
    switch (msgbox_ptr->image_type)
    {
    case GUIMSGBOX_IMAGE_BMP:
        IMG_EnableTransparentColor(TRUE); 
        dis_point.x = base_ctrl_ptr->rect.left;
        dis_point.y = base_ctrl_ptr->rect.top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            msgbox_ptr->win_id,
            msgbox_ptr->image_id,
            &base_ctrl_ptr->lcd_dev_info);
        IMG_EnableTransparentColor(FALSE); 
        break;
    case GUIMSGBOX_IMAGE_ANIMA:
        SCI_ASSERT(PNULL != msgbox_ptr->animate_ptr); /*assert verified*/
        GUIANIM_PlayAnimByPtr(msgbox_ptr->animate_ptr);
        break;

    default:
        break;
    }

	//显示softkey;
    DrawSoftkey(msgbox_ptr);
}

/*******************************************************************************************/
// 	Description : 显示文本内容；
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void ShowMsgBoxText(
						CTRLMSGBOX_OBJ_T*	msgbox_ptr,
                        GUIMSGBOX_TEXT_INFO_T* text_info_ptr,     //文本信息
                        GUIMSGBOX_STYLE_T style
                        )
{
    uint16 pos = 0;
    GUI_ALIGN_E align = ALIGN_LEFT;
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
    MMI_STRING_T    str_info = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)msgbox_ptr;

    if(PNULL == text_info_ptr)
    {
        //SCI_TRACE_MID:"gui warning:ShowMsgBoxText: text_info_ptr = PNULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMSGBOX_1273_112_2_18_3_20_34_250,(uint8*)"");
        return;
    }
    
    if (0 != text_info_ptr->back_color)
    {
        LCD_FillRect(&base_ctrl_ptr->lcd_dev_info, text_info_ptr->text_rect, text_info_ptr->back_color);
    }

    pos = GUI_CalculateStringFirstPosByLines(
        text_info_ptr->first_line,
        (uint16)(text_info_ptr->text_rect.right - text_info_ptr->text_rect.left + 1),   //[IN] 像素个数
        text_info_ptr->string.wstr_ptr,    //[IN] String指针
        text_info_ptr->string.wstr_len,    //[IN] String长度(Byte长度)
//        text_info_ptr->string.is_ucs2,    //[IN] 是否为ucs2编码
        text_info_ptr->font,       //[IN] String的字体
        text_info_ptr->space_char  //[IN] 字间距
        );


    switch(style & GUIMSGBOX_TEXT_MASK)
    {
    case GUIMSGBOX_TEXT_RIGHT:
        align = ALIGN_RIGHT;
        break;

    case GUIMSGBOX_TEXT_TOP:
        align = ALIGN_TOP;
        break;
    
    case GUIMSGBOX_TEXT_HCENTER:
        align = ALIGN_HMIDDLE;
        break;

    case GUIMSGBOX_TEXT_VCENTER:
        align = ALIGN_VMIDDLE;
        break;

    case (GUIMSGBOX_TEXT_HCENTER | GUIMSGBOX_TEXT_VCENTER):
        align = ALIGN_HVMIDDLE;
        break;
    
    default:
        align = ALIGN_LEFT;
    }


    str_style.align = align;
    str_style.line_space = 0;
    str_style.char_space = text_info_ptr->space_char;
    str_style.font = text_info_ptr->font;
    str_style.font_color = text_info_ptr->font_color;

    str_info.wstr_len = (uint16)(text_info_ptr->string.wstr_len - pos);
    str_info.wstr_ptr = &text_info_ptr->string.wstr_ptr[pos];

    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO*)&(base_ctrl_ptr->lcd_dev_info),
            (const GUI_RECT_T*)&text_info_ptr->text_rect,       //the fixed display area
            (const GUI_RECT_T*)&text_info_ptr->text_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T*)&str_info,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO
            );
}

/*******************************************************************************************/
// 	Description : 处理消息框失去焦点事件
//	Global resource dependence : 
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
LOCAL void HandleMsgBoxLoseFocus(
                                 CTRLMSGBOX_OBJ_T   *msgbox_ptr //	消息框
                                 )
{
    if(PNULL == msgbox_ptr)
    {
        //SCI_TRACE_MID:"gui warning:HandleMsgBoxLoseFocus: msgbox_ptr = PNULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMSGBOX_1350_112_2_18_3_20_34_251,(uint8*)"");
        return;
    }

    //控件失去焦点;
    msgbox_ptr->is_focus = FALSE;
    
    if(msgbox_ptr->text_info.text_timer)
    {
        MMK_StopTimer( msgbox_ptr->text_info.text_timer);    
        msgbox_ptr->text_info.text_timer = 0;
    }
}

/*******************************************************************************************/
// 	Description : Set font and font color of message box
//	Global resource dependence : 
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
void GUIMSGBOX_SetFontParam(   
                        MMI_CTRL_ID_T    ctrl_id,
	                    GUI_FONT_T          font,
                        GUI_COLOR_T         font_color,
                        uint8               space_char,     //字间距
                        uint8               space_line,     //行间距
                        uint8               move_line,      //每次滚动行数
                        GUI_COLOR_T         back_color      //显示文本区域的背景色
                        )
{
	//获得消息框的指针
    CTRLMSGBOX_OBJ_T *msgbox_ptr = GetMsgBoxPtr(ctrl_id);

    if (PNULL == msgbox_ptr)
    {
        return;
    }
    msgbox_ptr->text_info.font = font;
    msgbox_ptr->text_info.font_color = font_color;
    msgbox_ptr->text_info.space_char = space_char;
    msgbox_ptr->text_info.space_line = space_line;
    msgbox_ptr->text_info.move_line  = move_line;
    msgbox_ptr->text_info.back_color = back_color;

}

/*****************************************************************************/
//  Description : Get msgbox list pointer from control id
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLMSGBOX_OBJ_T* GetMsgBoxPtr(
                                 MMI_HANDLE_T ctrl_handle
                                 )
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr( ctrl_handle );

    if ( PNULL != ctrl_ptr )
    {
        if (!MsgBoxTypeOf( ctrl_ptr ))
        {
            ctrl_ptr = PNULL;
        }
    }
    
    return (CTRLMSGBOX_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : msgbox type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN MsgBoxTypeOf(
                           CTRLBASE_OBJ_T* msgbox_ptr
                           )
{
    return TYPEMNG_IsTypeOf(msgbox_ptr, CTRL_MSGBOX_TYPE);
}



