/*****************************************************************************
** File Name:      ctrltitle.c                                               *
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

#include "ctrltitle.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ASCII_CODE_DOT                0x2E  // ascii code /
#define LONG_TITLE_MAX_DISPLAY_LEN    7

#define SUBTEXT_LEFT_OFFSET           DP2PX_VALUE(62) 
#define SUBTEXT_RIGHT_OFFSET          DP2PX_VALUE(2)

#define TEXT_TOP_OFFSET               DP2PX_VALUE(2)
#define TEXT_BOTTOM_OFFSET            DP2PX_VALUE(0)

#define SUBTEXT_MAX_L_MARGIN          DP2PX_VALUE(75)

#define SUBTEXT_BOTTOM_OFFSET         DP2PX_VALUE(2)

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
// Description : init 
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void TitleInit( 
                     CTRLBASE_OBJ_T        *ctrl_ptr,
                     MMI_TEXT_ID_T    text_id
                     );

/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLTITLE_OBJ_T *GetTitlePtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   );

/*****************************************************************************/
// Description : handle msg of title
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E TitleHandleMsg( 
//                                   CTRLBASE_OBJ_T        *ctrl_ptr,
//                                   MMI_MESSAGE_ID_E  msg_id,
//                                   DPARAM            param
//                                   );

/*****************************************************************************/
// Description : to calc the pix of window title string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcTitleStrLenByPix(
                                  MMI_HANDLE_T win_handle,
                                  CTRLTITLE_OBJ_T    *title_ptr
                                  );

/*****************************************************************************/
// Description : to calc the pix of window title string
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 CalcStrLenByPix( 
                             const MMI_STRING_T    *title_str_ptr, 
                             CTRLTITLE_OBJ_T        *title_ptr
                             );

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length. scroll as character
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinTitleTimerAsCharMsg(
                                        CTRLTITLE_OBJ_T    *title_ptr
                                        );

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length. scroll as pixel
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinTitleTimerAsPixelMsg(
                                         CTRLTITLE_OBJ_T    *title_ptr
                                         );

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length.
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HandleWinTitleTimerMsg(
                                  CTRLTITLE_OBJ_T    *title_ptr
                                  );

/*****************************************************************************/
// Description : display the window title, is called by the MMI Kernel
// Global resource dependence : 
// Author:Liqing.peng
// Modify:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitle(
                        CTRLTITLE_OBJ_T    *title_ptr
                        );

/*****************************************************************************/ 
// Description : display the window title, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollTitleByOffset(
                                         const wchar*  str_ptr, //[IN] string pointer
                                         uint16  str_len, //[IN] string length (byte number)
                                         uint16 offset,//[in]scroll offset
                                         CTRLTITLE_OBJ_T    *title_ptr
                                         );

/*****************************************************************************/ 
// Description : display the window title, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetTextRectWidth(
                              CTRLTITLE_OBJ_T    *title_ptr
                              );

/*****************************************************************************/
// Description : display the left and right arrow in title 
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleArrow(
                             CTRLTITLE_OBJ_T    *title_ptr
                             );

/*****************************************************************************/
// Description : display title list index info
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleSubText(
                               CTRLTITLE_OBJ_T    *title_ptr,
                               BOOLEAN is_disp_bg
                               );

/*****************************************************************************/
// Description : get the title text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             CTRLTITLE_OBJ_T    *title_ptr
                             );

/*****************************************************************************/
// Description : get the title text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSubTextRect(
                                CTRLTITLE_OBJ_T    *title_ptr
                                );

/*****************************************************************************/
// Description : display title background
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleBg(
                          CTRLTITLE_OBJ_T    *title_ptr
                          );

/*****************************************************************************/
// Description : display title icon
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleIcon(
                            CTRLTITLE_OBJ_T    *title_ptr
                            );

/*****************************************************************************/
// Description : handle window tp message
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWinTitleTPMsg(
                                       MMI_MESSAGE_ID_E  msg_id,
                                       GUI_POINT_T      point,
                                       CTRLTITLE_OBJ_T    *title_ptr
                                       );

/*****************************************************************************/
// Description : start title scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StartTitleScrollTimer(
                                 CTRLTITLE_OBJ_T    *title_ptr
                                 );

/*****************************************************************************/
// Description : stop title scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StopTitleScrollTimer(
                                CTRLTITLE_OBJ_T    *title_ptr
                                );

/*****************************************************************************/
// Description : display title text
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleText(
                            CTRLTITLE_OBJ_T    *title_ptr
                            );

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetTitleText(
                                CTRLTITLE_OBJ_T    *title_ptr
                                );

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetTitleButtonText(
                                      CTRLTITLE_OBJ_T    *title_ptr,
                                      uint8 button_idx
                                      );

/*****************************************************************************/
// Description : 显示title子区域背景
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleSubBg(
                             CTRLTITLE_OBJ_T    *title_ptr,
                             GUI_RECT_T sub_rect
                             );

/*****************************************************************************/
// Description : 获得文本的排版信息
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetFontAlign(
                               CTRLTITLE_OBJ_T *title_ptr
                               );

/*****************************************************************************/
// Description : 显示图片,封装接口的目的是不要重复调用透明色的接口
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleImg(
                           GUI_POINT_T            *display_point_ptr,
                           GUI_RECT_T             *display_rect_ptr,
                           GUI_RECT_T             *image_rect_ptr,
                           MMI_HANDLE_T           win_handle,
                           MMI_IMAGE_ID_T         img_id,       
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           );

/*****************************************************************************/ 
// Description : 根据文本区域确定背景图片的区域 
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTitleBgImgRect(
                                   CTRLTITLE_OBJ_T *title_ptr, 
                                   GUI_RECT_T text_rect
                                   );

/*****************************************************************************/ 
// Description : 根据文本区域显示背景
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleTextBg(
                              CTRLTITLE_OBJ_T    *title_ptr
                              );

/*****************************************************************************/
// Description : title set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleSetRect( 
                           CTRLBASE_OBJ_T *iguictrl_ptr, 
                           const GUI_RECT_T *rect_ptr
                           );

/*****************************************************************************/ 
// Description : 获得title控件的文本显示风格
// Global resource dependence :  
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void GetTitleTextStyle(
                             CTRLTITLE_OBJ_T *title_ptr,
                             GUISTR_STYLE_T *style_ptr
                             );

/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseTitleStr(
                           CTRLTITLE_OBJ_T *title_ptr
                           );

/*****************************************************************************/
// 	Description : display the Istyle icon in title 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void DisplayButtonText(
                             CTRLTITLE_OBJ_T *title_ptr,
                             GUITITLE_BUTTON_INDEX_T icon_index
                             );

/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetButtonTextByPtr(    
                                 CTRLTITLE_OBJ_T *title_ptr,
                                 GUITITLE_BUTTON_INDEX_T icon_index,
                                 MMI_STRING_T *str_ptr
                                 );

/*****************************************************************************/
// Description : title set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetTitleContentRect( 
                                    CTRLTITLE_OBJ_T *title_ptr
                                    );

/*****************************************************************************/
//  Description : init title class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlInitVtbl(
    CTRLTITLE_VTBL_T        *title_vtbl_ptr
);

/*****************************************************************************/
//  Description : title construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLTITLE_INIT_PARAM_T   *title_param_ptr
);

/*****************************************************************************/
//  Description : title destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TitleCtrlPackInitParam (
    GUITITLE_INIT_DATA_T     *title_init_ptr,   //in
    CTRLTITLE_INIT_PARAM_T   *title_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle title msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TitleCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get title pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTITLE_OBJ_T* GetTitleobjPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TitleTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
);

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get title type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TITLE_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "title",                                         // control name
                       sizeof (CTRLTITLE_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) TitleCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) TitleCtrlDestruct,      // control destructor function
                       sizeof (CTRLTITLE_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) TitleCtrlPackInitParam,     // convert init param
                       sizeof (CTRLTITLE_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) TitleCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init title class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlInitVtbl (
    CTRLTITLE_VTBL_T        *title_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) title_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = TitleCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = TitleCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetRect = TitleSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;


    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void TitleCtrlPackInitParam (
    GUITITLE_INIT_DATA_T     *title_init_ptr,   //in
    CTRLTITLE_INIT_PARAM_T   *title_param_ptr   //out
)
{
    if (PNULL == title_init_ptr || PNULL == title_param_ptr)
    {
        return;
    }

    title_param_ptr->text_id = title_init_ptr->text_id;
}

/*****************************************************************************/
//  Description : title construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLTITLE_INIT_PARAM_T   *title_param_ptr
)
{
    BOOLEAN              result = TRUE;
        
    if ((PNULL == base_ctrl_ptr)||(PNULL == title_param_ptr))
    {
        return FALSE;
    }
    
    TitleInit( base_ctrl_ptr, title_param_ptr->text_id);
    
    IGUICTRL_SetState( (IGUICTRL_T *)base_ctrl_ptr, GUICTRL_STATE_DISABLE_ACTIVE, TRUE );
    
    return (result);
}

/*****************************************************************************/
//  Description : title destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    uint32 i = 0;
    CTRLTITLE_OBJ_T        *title_ptr = (CTRLTITLE_OBJ_T*)base_ctrl_ptr;
    
    if (PNULL == title_ptr)
    {
        return TRUE;
    }
    
    ReleaseTitleStr(title_ptr);
    
    for (i = 0; i < GUITITLE_BUTTON_NUM; i++)
    {
        if (PNULL != title_ptr->button_info[i].icon_str_ptr)
        {
            SCI_FREE(title_ptr->button_info[i].icon_str_ptr);
            title_ptr->button_info[i].icon_str_ptr = PNULL;
            
            title_ptr->button_info[i].str_len = 0;
        }
    }
    
    return TRUE;

}

/*****************************************************************************/
//  Description : handle title msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TitleCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLTITLE_OBJ_T     *title_ctrl_ptr = (CTRLTITLE_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_TITLE_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL == base_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    title_ctrl_ptr = (CTRLTITLE_OBJ_T *)base_ctrl_ptr;
        
    switch(msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_GET_FOCUS:
        break;

    case MSG_CTL_LOSE_FOCUS:
        StopTitleScrollTimer(title_ctrl_ptr);
        break;

    case MSG_CTL_PAINT:
        DisplayTitle(title_ctrl_ptr);
        break;

    case MSG_TIMER:      
        if((*(uint8*)param) == title_ctrl_ptr->scroll_timer_id)
        {
            if( 0 != title_ctrl_ptr->scroll_timer_id )
            {
                MMK_StopTimer(title_ctrl_ptr->scroll_timer_id);
                title_ctrl_ptr->scroll_timer_id = 0;
            }
            HandleWinTitleTimerMsg(title_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_LONG:    
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			result = HandleWinTitleTPMsg(msg_id, point, title_ctrl_ptr);
		}
        break;
#endif 

    case MSG_CTL_CLOSE:
        StopTitleScrollTimer(title_ctrl_ptr);
        break;

    default:
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get title pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTITLE_OBJ_T* GetTitleobjPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (TitleTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLTITLE_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TitleTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_TITLE_TYPE);
}

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 有滚动条功能的控件需要实现这个接口
    // ...

    return result;
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
// Description : init button
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void TitleInit( 
                     CTRLBASE_OBJ_T    *ctrl_ptr,
                     MMI_TEXT_ID_T     text_id
                     )
{
    CTRLTITLE_OBJ_T   *title_ptr = (CTRLTITLE_OBJ_T*)ctrl_ptr;
    MMI_TITLE_STYLE_T title_style = {0};

    ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl( ctrl_ptr->handle );
    SCI_ASSERT(NULL != title_ptr->base_ctrl.win_handle);/*assert verified*/

    MMITHEME_GetTitleStyle(&title_style);

    ctrl_ptr->both_rect = MMITHEME_GetWinTitleBothRect(ctrl_ptr->win_handle);
    //title_ptr->rect      = title_ptr->both_rect.v_rect;

    title_ptr->is_need_button = title_style.is_need_button;
    title_ptr->scroll_type = TITLE_SCROLL_LEFT_OUT;

    // 初始化字符串
    title_ptr->text_type = GUITITLE_DATA_TEXT_ID;
    title_ptr->text_id = text_id;
    title_ptr->title_str.wstr_ptr = PNULL;
    title_ptr->title_str.wstr_len = 0;

    
    title_ptr->str_style.align = title_style.align_type;
    title_ptr->str_style.font = title_style.font;
    title_ptr->str_style.font_color = title_style.font_color;
    title_ptr->str_style.char_space = title_style.char_space;

    title_ptr->bg_image = title_style.bg_image;
    title_ptr->bg_color = title_style.bg_color;
    title_ptr->sub_font   = title_style.sub_font;
    title_ptr->scroll_time = title_style.scroll_time;
    title_ptr->scroll_pixel_step = title_style.scroll_pixel_step;
    title_ptr->is_need_bg = TRUE;

#ifdef GUIF_TITLE_PDASTYLE
    title_ptr->sub_align = ALIGN_RVMIDDLE;
#else
    title_ptr->sub_align = ALIGN_RIGHT;
#endif
    title_ptr->title_text_margin = title_style.title_text_margin;
    title_ptr->icon_space = title_style.icon_space;
    
    ctrl_ptr->bg = MMITHEME_GetCommonBg();
    
    title_ptr->button_info[GUITITLE_BUTTON_LEFT].str_len = 0;
    title_ptr->button_info[GUITITLE_BUTTON_LEFT].icon_str_ptr = PNULL;

    title_ptr->button_info[GUITITLE_BUTTON_RIGHT].str_len = 0;
    title_ptr->button_info[GUITITLE_BUTTON_RIGHT].icon_str_ptr = PNULL;

#ifdef GUIF_TITLE_PDASTYLE
    if (MMITHEME_IsIstyle())
    {   
        title_ptr->is_need_sub_text = FALSE;
        //back Icon
        title_ptr->button_state[GUITITLE_BUTTON_LEFT] = TITLE_BUTTON_NORMAL;//return icon default state to visible
        CTRLTITLE_SetButtonTextId(title_ptr, GUITITLE_BUTTON_LEFT, title_style.left_button_text_id, FALSE);
        
        //menu icon
        title_ptr->button_state[GUITITLE_BUTTON_RIGHT] = TITLE_BUTTON_INVISIBLE;//menu icon default state to invisible
        CTRLTITLE_SetButtonTextId(title_ptr, GUITITLE_BUTTON_RIGHT, title_style.right_button_text_id, FALSE);
        
        title_ptr->button_msg[GUITITLE_BUTTON_LEFT] = MSG_APP_CANCEL; //default to MSG_APP_CANCEL
        title_ptr->button_msg[GUITITLE_BUTTON_RIGHT] = MSG_APP_MENU;  //default to MSG_APP_MENU
    }
    else
#endif
    {
        //left Icon
        title_ptr->button_state[GUITITLE_BUTTON_LEFT] = TITLE_BUTTON_NORMAL;//return icon default state to visible
        title_ptr->button_msg[GUITITLE_BUTTON_LEFT] = MSG_CTL_TITLE_MOVE_PREV;//default to MSG_APP_CANCEL

        //right icon
        title_ptr->button_state[GUITITLE_BUTTON_RIGHT] = TITLE_BUTTON_NORMAL;//menu icon default state to invisible
        title_ptr->button_msg[GUITITLE_BUTTON_RIGHT] = MSG_CTL_TITLE_MOVE_NEXT;//default to MSG_APP_MENU
    }

    title_ptr->button_info[GUITITLE_BUTTON_LEFT].image_id = title_style.left_button_icon_up;
    title_ptr->button_info[GUITITLE_BUTTON_LEFT].press_image_id = title_style.left_button_icon_down;
    title_ptr->button_info[GUITITLE_BUTTON_LEFT].gray_image_id = title_style.left_button_icon_disable;
    
    title_ptr->button_info[GUITITLE_BUTTON_RIGHT].image_id = title_style.right_button_icon_up;
    title_ptr->button_info[GUITITLE_BUTTON_RIGHT].press_image_id = title_style.right_button_icon_down;
    title_ptr->button_info[GUITITLE_BUTTON_RIGHT].gray_image_id = title_style.right_button_icon_disable;

    return;
}

/*****************************************************************************/
// Description : handle msg of button
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E TitleHandleMsg( 
//                                   CTRLBASE_OBJ_T        *ctrl_ptr,
//                                   MMI_MESSAGE_ID_E  msg_id,
//                                   DPARAM            param
//                                   )
// {
//     MMI_RESULT_E      recode = MMI_RESULT_TRUE;
// #ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint	
//     GUI_POINT_T       point = {0};
// #endif
//     CTRLTITLE_OBJ_T      *title_ptr = PNULL;
// 
//     //kevin.lou modified:delete assert
//     //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
//     if (PNULL == ctrl_ptr)
//     {
//         return MMI_RESULT_FALSE;
//     }
// 
//     title_ptr = (CTRLTITLE_OBJ_T *)ctrl_ptr;
//     
//     switch(msg_id)
//     {
//     case MSG_CTL_OPEN:
//         break;
// 
//     case MSG_CTL_GET_FOCUS:
//         break;
// 
//     case MSG_CTL_LOSE_FOCUS:
//         StopTitleScrollTimer(title_ptr);
//         break;
// 
//     case MSG_CTL_PAINT:
//         DisplayTitle(title_ptr);
//         break;
// 
//     case MSG_TIMER:      // @Rui
//         if((*(uint8*)param) == title_ptr->scroll_timer_id)
//         {
//             if( 0 != title_ptr->scroll_timer_id )
//             {
//                 MMK_StopTimer(title_ptr->scroll_timer_id);
//                 title_ptr->scroll_timer_id = 0;
//             }
//             HandleWinTitleTimerMsg(title_ptr);
//         }
//         else
//         {
//             recode = MMI_RESULT_FALSE;
//         }
//         break;
// 
// #ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
//     case MSG_TP_PRESS_DOWN:
//     case MSG_TP_PRESS_UP:
//     case MSG_TP_PRESS_LONG:    
//     case MSG_TP_PRESS_MOVE:
//         point.x = MMK_GET_TP_X(param);
//         point.y = MMK_GET_TP_Y(param);
//         recode = HandleWinTitleTPMsg(msg_id, point, title_ptr);
//         break;
// #endif //TOUCH_PANEL_SUPPORT //IGNORE9527
// 
//     case MSG_CTL_CLOSE:
//         StopTitleScrollTimer(title_ptr);
//         break;
// 
//     default:
//         recode = MMI_RESULT_FALSE;
//         break;
//     }
// 
//     return recode;
// }

/*****************************************************************************/
// Description : title set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN TitleSetRect( 
                           CTRLBASE_OBJ_T *iguictrl_ptr, 
                           const GUI_RECT_T *rect_ptr
                           )
{
    BOOLEAN           result = FALSE;
    CTRLTITLE_OBJ_T   *title_ptr = PNULL;
    //MMI_TITLE_STYLE_T title_style = {0};
    //uint16 image_width = 0;
    //uint16 image_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != iguictrl_ptr && PNULL != rect_ptr ); /*assert verified*/
    if ((PNULL == iguictrl_ptr)||(PNULL == rect_ptr))
    {
        return FALSE;
    }

    title_ptr = (CTRLTITLE_OBJ_T *)iguictrl_ptr;

    if (!GUI_EqualRect(iguictrl_ptr->rect, *rect_ptr)
#ifdef GUIF_TITLE_PDASTYLE
        || MMITHEME_IsIstyle()
#endif
        )
    {
        iguictrl_ptr->rect = *rect_ptr;
        
        GUI_SetVOrHRect( iguictrl_ptr->handle, (GUI_RECT_T *)rect_ptr, &(iguictrl_ptr->both_rect));

        // 重设各项内容的区域
        ResetTitleContentRect(title_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseTitleStr(
                           CTRLTITLE_OBJ_T *title_ptr
                           )
{
    if (PNULL != title_ptr)
    {
        if (PNULL != title_ptr->title_str.wstr_ptr)
        {
            SCI_FREE(title_ptr->title_str.wstr_ptr);
        }
        
        title_ptr->title_str.wstr_ptr = PNULL;
        title_ptr->title_str.wstr_len = 0;
    }
}

/*****************************************************************************/
// Description : to calc the pix of window title string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcTitleStrLenByPix(
                                  MMI_HANDLE_T   win_handle,
                                  CTRLTITLE_OBJ_T    *title_ptr
                                  )
{

    MMI_STRING_T  title_str = {0};
    uint16        length = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return 0;
    }

    title_str = GetTitleText(title_ptr);
    
    length = CalcStrLenByPix(&title_str, title_ptr);

    return (length);
}

/*****************************************************************************/
// Description : to calc the pix of window title string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcStrLenByPix(
                             const MMI_STRING_T    *title_str_ptr,
                             CTRLTITLE_OBJ_T        *title_ptr
                             )
{

    uint16          length = 0;
 
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_str_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if ((PNULL == title_str_ptr)||(PNULL == title_ptr))
    {
        return 0;
    }
    
    length = GUI_CalculateStringPiexlNum(
                        title_str_ptr->wstr_ptr,
                        title_str_ptr->wstr_len,
                        title_ptr->str_style.font,
                        title_ptr->str_style.char_space);

    return (length);
}


/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLTITLE_OBJ_T *GetTitlePtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    CTRLTITLE_OBJ_T* title_ptr = PNULL;
    MMI_HANDLE_T ctrl_handle = 0;
    
    if(0 != win_handle)
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        ctrl_handle = MMK_ConvertIdToHandle( ctrl_id );

        if (0 != ctrl_id && ctrl_handle == ctrl_id)
        {
            title_ptr = (CTRLTITLE_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);
        }
        else
        {
            title_ptr = (CTRLTITLE_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle, ctrl_id);
        }

        if(PNULL != title_ptr)
        {
            SCI_ASSERT(TitleTypeOf((CTRLBASE_OBJ_T*)title_ptr ));/*assert verified*/
        }
    }
    
    return title_ptr;
}

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length.
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HandleWinTitleTimerMsg(
                                  CTRLTITLE_OBJ_T *title_ptr
                                  )
{
    if( MMITHEME_GetIsScrollAsPixel() )
    {
        HandleWinTitleTimerAsPixelMsg(title_ptr);
    }
    else
    {
        HandleWinTitleTimerAsCharMsg(title_ptr);
    }    
}

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length. scroll as character
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinTitleTimerAsCharMsg(
                                        CTRLTITLE_OBJ_T    *title_ptr
                                        )
{
    uint16          length = 0;
    MMI_STRING_T    title_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    title_str = GetTitleText(title_ptr);
    
    //得到当前要显示的窗口标题的长度，以PX记.
    length = CalcStrLenByPix(&title_str, title_ptr);

    // if title string length larger then 10 byte , need to circular display.
    if (length > GetTextRectWidth(title_ptr)) 
    {
        title_ptr->is_text_overstep = TRUE;
        // disappeared from left byte by byte
        if (TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)   
        {
            if (title_ptr->scroll_char_offset < title_str.wstr_len)
            {
                title_ptr->scroll_char_offset++;
                title_str.wstr_ptr += title_ptr->scroll_char_offset;
                title_str.wstr_len  = (uint16)(title_str.wstr_len - title_ptr->scroll_char_offset);
            }
            else
            {
                title_ptr->scroll_char_offset = 0;
            }
        }

        DisplayScrollTitleByOffset(title_str.wstr_ptr, title_str.wstr_len, 0, title_ptr);

        if (MMK_IsFocusWin(title_ptr->base_ctrl.win_handle) && TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)
        {
            title_ptr->scroll_timer_id = MMK_CreateWinTimer(title_ptr->base_ctrl.handle, title_ptr->scroll_time, FALSE);
        }
    }
    else
    {
        title_ptr->is_text_overstep = FALSE;
    }
}

/*****************************************************************************/
// Description : handle window timer message to update window title if title  
// string length larger then the title's length. scroll as pixel
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinTitleTimerAsPixelMsg(
                                         CTRLTITLE_OBJ_T *title_ptr
                                         )
{
    uint16          pixel_length = 0;
    MMI_STRING_T    title_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    title_str = GetTitleText(title_ptr);

    //得到当前要显示的窗口标题的长度，以pixel记.
    pixel_length = CalcStrLenByPix(&title_str, title_ptr);

    // if title string length larger then title rect width , need to circular display.
    if (pixel_length > GetTextRectWidth(title_ptr)) 
    {
        title_ptr->is_text_overstep = TRUE;
        //disappeared from left byte by byte
        if (TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)    
        { 
            if ( title_ptr->scroll_pixel_offset < pixel_length - title_ptr->scroll_pixel_step)
            {
                title_ptr->scroll_pixel_offset += title_ptr->scroll_pixel_step;
            }
            else
            {
                title_ptr->scroll_pixel_offset = 0;
            }
        }

        DisplayScrollTitleByOffset(title_str.wstr_ptr, title_str.wstr_len, title_ptr->scroll_pixel_offset, title_ptr);

        // 启动timer
        if (MMK_IsFocusWin(title_ptr->base_ctrl.win_handle) && TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)
        {
            title_ptr->scroll_timer_id = MMK_CreateWinTimer(title_ptr->base_ctrl.handle, title_ptr->scroll_time, FALSE);
        }
    }
    else
    {
        title_ptr->is_text_overstep = FALSE;
    }
}

/*****************************************************************************/
// Description : display the window title, is called by the MMI Kernel
// Global resource dependence : 
// Author:Liqing.peng
// Modify:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitle(
                        CTRLTITLE_OBJ_T    *title_ptr
                        )
{

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }


    if (!IGUICTRL_GetState((IGUICTRL_T *)title_ptr, GUICTRL_STATE_INVISIBLE))
    {
        ResetTitleContentRect(title_ptr);

        // 显示背景
        DisplayTitleBg(title_ptr);

        //显示文本
        DisplayTitleText(title_ptr);

        //display the left and right arrow in title
        DisplayTitleArrow(title_ptr);
        DisplayButtonText(title_ptr, GUITITLE_BUTTON_LEFT);
        DisplayButtonText(title_ptr, GUITITLE_BUTTON_RIGHT);

        //显示列表索引信息等子文本
        DisplayTitleSubText(title_ptr, FALSE);

        //显示标题小图标
        DisplayTitleIcon(title_ptr);

        //启动文本滚动定时器
        StartTitleScrollTimer(title_ptr);
    }
}

/*****************************************************************************/ 
// Description : 根据文本区域确定背景图片的区域 
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTitleBgImgRect(
                                   CTRLTITLE_OBJ_T *title_ptr, 
                                   GUI_RECT_T text_rect
                                   )
{
    GUI_RECT_T image_rect = {0};

    image_rect.left = text_rect.left - title_ptr->base_ctrl.rect.left;
    image_rect.top = text_rect.top - title_ptr->base_ctrl.rect.top;
    image_rect.right = image_rect.left + text_rect.right - text_rect.left;
    image_rect.bottom = image_rect.top + text_rect.bottom - text_rect.top;

    return image_rect;
}

/*****************************************************************************/ 
// Description : 根据文本区域显示背景
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleTextBg(
                              CTRLTITLE_OBJ_T    *title_ptr
                              )
{
    GUI_RECT_T       image_rect = {0};
    GUI_RECT_T       text_rect =  GetTextRect(title_ptr);
    GUI_BG_DISPLAY_T bg_display = {0};
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }
    
    if (title_ptr->is_need_bg)
    {
        // 显示背景
        if (0 != title_ptr->bg_image && title_ptr->bg_image != MMITHEME_GetInvalidImageID())
        {
            image_rect = GetTitleBgImgRect(title_ptr, text_rect);
            
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = title_ptr->base_ctrl.rect;
            bg_display.display_rect   = text_rect;
            bg_display.img_rect       = text_rect;
            bg_display.ctrl_handle    = title_ptr->base_ctrl.handle;
            bg_display.win_handle     = title_ptr->base_ctrl.win_handle;
           
            // common背景
//             GUI_DisplayBg(&title_ptr->common_bg,
//                 &bg_display,
//                 &title_ptr->lcd_dev_info);
             
            // 显示title背景
            GUIRES_DisplaySabmImg(
                &(text_rect),
                &title_ptr->base_ctrl.rect,//full rect
                &(image_rect),
                title_ptr->base_ctrl.win_handle,
                title_ptr->bg_image,
                &title_ptr->base_ctrl.lcd_dev_info
                );

        }
#ifdef ADULT_WATCH_SUPPORT
        else
#else
        else if (0 != title_ptr->bg_color)
#endif
        {
            LCD_FillRect(&title_ptr->base_ctrl.lcd_dev_info, text_rect, title_ptr->bg_color);
        }
    }
}

/*****************************************************************************/ 
// Description : 获得title控件的文本显示风格
// Global resource dependence :  
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void GetTitleTextStyle(
                             CTRLTITLE_OBJ_T *title_ptr,
                             GUISTR_STYLE_T *style_ptr
                             )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr && PNULL != style_ptr); /*assert verified*/
    if (PNULL == title_ptr || PNULL == style_ptr)
    {
        return;
    }

    style_ptr->align = GetFontAlign(title_ptr);
    style_ptr->font = title_ptr->str_style.font;
    style_ptr->font_color = title_ptr->str_style.font_color;
    style_ptr->char_space = title_ptr->str_style.char_space;
}

/*****************************************************************************/ 
// Description : display the window title, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollTitleByOffset(
                                         const wchar*  str_ptr, //[IN] string pointer
                                         uint16  str_len, //[IN] string length (byte number)
                                         uint16 offset,//[in]scroll offset
                                         CTRLTITLE_OBJ_T    *title_ptr
                                         )
{
    BOOLEAN recode    = TRUE;
    GUI_RECT_T disp_rect =  GetTextRect(title_ptr);
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;
    MMI_STRING_T    str_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return FALSE;
    }
    
    GetTitleTextStyle(title_ptr, &str_style);
    str_info.wstr_len = str_len;
    str_info.wstr_ptr = (wchar *)str_ptr;

    //显示背景
    DisplayTitleTextBg(title_ptr);

    if (TITLE_SCROLL_LEFT_OUT != title_ptr->scroll_type)    
    {
        str_state |= GUISTR_STATE_ELLIPSIS;
    }

    if ( (PNULL != str_ptr) && (0 < str_len) )
    {
        if( MMITHEME_GetIsScrollAsPixel() && TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)
        {
            GUISTR_DrawTextToLCDByOffset( 
                                         (const GUI_LCD_DEV_INFO*)&title_ptr->base_ctrl.lcd_dev_info,
                                         (const GUI_RECT_T*)&disp_rect,
                                         PNULL,
                                         offset,
                                         0,
                                         (const MMI_STRING_T*)&str_info,
                                         &str_style,
                                         str_state
                                         );
        }
        else
        {
            GUISTR_DrawTextToLCDInRect( 
                                       (const GUI_LCD_DEV_INFO*)&title_ptr->base_ctrl.lcd_dev_info,
                                       (const GUI_RECT_T*)&disp_rect,       //the fixed display area
                                       (const GUI_RECT_T*)&disp_rect,       //用户要剪切的实际区域
                                       (const MMI_STRING_T*)&str_info,
                                       &str_style,
                                       str_state,
                                       GUISTR_TEXT_DIR_AUTO
                                       );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"DisplayScrollTitleByOffset: the param is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITITLE_1618_112_2_18_3_22_23_370,(uint8*)"");
        recode = FALSE;
    }

    return (recode);
}

/*****************************************************************************/ 
// Description : display the window title, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetTextRectWidth(
                              CTRLTITLE_OBJ_T    *title_ptr
                              )
{
    GUI_RECT_T    text_rect = GetTextRect(title_ptr);

    return (uint16)(text_rect.right - text_rect.left + 1);
}

/*****************************************************************************/
// Description : display the left and right arrow in title 
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleArrow(
                             CTRLTITLE_OBJ_T *title_ptr
                             )
{
    MMI_TITLE_STYLE_T   title_style = {0};
    GUI_POINT_T         dis_point = {0};

    MMITHEME_GetTitleStyle(&title_style);

    if (NULL != title_ptr && title_ptr->is_need_button)
    {
        BOOLEAN is_draw = TRUE;
        MMI_IMAGE_ID_T image_id = 0;
        GUITITLE_BUTTON_INDEX_T button_index = 0;
        
        for (button_index = 0; button_index < GUITITLE_BUTTON_NUM; button_index++)
        {
            //draw left arrow
            dis_point.x = title_ptr->button_rect[button_index].left;
            dis_point.y = title_ptr->button_rect[button_index].top;
            
            switch(title_ptr->button_state[button_index])
            {
            case TITLE_BUTTON_NORMAL:
                image_id = title_ptr->button_info[button_index].image_id;
                is_draw = TRUE;
                break;
                
            case TITLE_BUTTON_GRAYED:
                is_draw = TRUE;
                image_id = title_ptr->button_info[button_index].gray_image_id;
                break;
                
            case TITLE_BUTTON_INVISIBLE:
                is_draw = FALSE;
                break;
                
            default:
                is_draw = FALSE;
                break;
            }
            
            if (is_draw)
            {
                // 画背景
                DisplayTitleSubBg(title_ptr, title_ptr->button_rect[button_index]);
                // 画图
                DisplayTitleImg(&dis_point, &title_ptr->button_rect[button_index], PNULL, title_ptr->base_ctrl.win_handle, image_id, &title_ptr->base_ctrl.lcd_dev_info);
            }
        }
    }
}

/*****************************************************************************/
// Description : 显示title 的字區域背景
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleSubBg(
                             CTRLTITLE_OBJ_T *title_ptr,
                             GUI_RECT_T      sub_rect       // [in] this pos is relative to title control
                             )
{
    GUI_RECT_T          image_pos_rect = sub_rect;       // position where we should display
    GUI_BG_DISPLAY_T    bg_display = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    if (title_ptr->is_need_bg)
    {
        image_pos_rect.top -= title_ptr->base_ctrl.rect.top;
        image_pos_rect.bottom -= title_ptr->base_ctrl.rect.top;
        image_pos_rect.left -= title_ptr->base_ctrl.rect.left;
        image_pos_rect.right -= title_ptr->base_ctrl.rect.left;

        // 显示背景
        if (0 != title_ptr->bg_image && title_ptr->bg_image != MMITHEME_GetInvalidImageID())
        {
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = title_ptr->base_ctrl.rect;
            bg_display.display_rect   = sub_rect;
            bg_display.img_rect       = sub_rect;
            bg_display.ctrl_handle    = title_ptr->base_ctrl.handle;
            bg_display.win_handle     = title_ptr->base_ctrl.win_handle;
            
            //title_ptr->common_bg = MMITHEME_GetCommonBg();

//             GUI_DisplayBg(&title_ptr->common_bg,
//                 &bg_display,
//                 &title_ptr->lcd_dev_info);
            
            GUIRES_DisplayImg(
                NULL,
                &(sub_rect),
                &(image_pos_rect),
                title_ptr->base_ctrl.win_handle,
                title_ptr->bg_image,
                &title_ptr->base_ctrl.lcd_dev_info
                );
        }
        else if (0 != title_ptr->bg_color)
        {
            LCD_FillRect(&title_ptr->base_ctrl.lcd_dev_info, image_pos_rect, title_ptr->bg_color);
        }
    }
}

/*****************************************************************************/
// Description : display title list index info
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleSubText(
                               CTRLTITLE_OBJ_T    *title_ptr,
                               BOOLEAN          is_disp_bg
                               )
{
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_WORDBREAK;
    MMI_STRING_T    str_info = {0};
    GUI_RECT_T      str_rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }
	str_rect = title_ptr->sub_text_rect;

    if (title_ptr->is_need_sub_text)
    {
        str_style.align = title_ptr->sub_align;
        str_style.font = title_ptr->sub_font;
        str_style.font_color = title_ptr->str_style.font_color;
        
        str_info.wstr_len = title_ptr->sub_str_len;
        str_info.wstr_ptr = title_ptr->sub_str;

        if (is_disp_bg)
        {
            DisplayTitleSubBg(title_ptr, title_ptr->sub_text_rect);
        }

        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO*)&title_ptr->base_ctrl.lcd_dev_info,
            (const GUI_RECT_T*)&str_rect,       //the fixed display area
            (const GUI_RECT_T*)&str_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T*)&str_info,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
}

/*****************************************************************************/
// Description : get the title text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             CTRLTITLE_OBJ_T *title_ptr
                             )
{
    GUI_RECT_T          rect = {0};
    GUI_RECT_T          sub_rect = {0};
    uint16              left_icon_width = 0;
    uint16              right_icon_width = 0;
    MMI_TITLE_STYLE_T   title_style = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return rect;
    }

    rect = title_ptr->base_ctrl.rect;

    rect.top += TEXT_TOP_OFFSET;
    rect.bottom -= TEXT_BOTTOM_OFFSET;

    rect.left += title_ptr->title_text_margin;
    rect.right -= title_ptr->title_text_margin;

    if (title_ptr->is_need_button)
    {

        MMITHEME_GetTitleStyle(&title_style);

        if (TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_LEFT])
        {
            left_icon_width = (title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right
                                                    - title_ptr->button_rect[GUITITLE_BUTTON_LEFT].left + 1) + title_style.button_margin;
        }

        if (TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_RIGHT])
        {
            right_icon_width = (title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].right
                                                    - title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left + 1) + title_style.button_margin;
        }

		if ((TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_LEFT])&&
			(TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_RIGHT]))
		{
			rect.left += left_icon_width;
			rect.right -= right_icon_width;
		} 
		else
		{
			rect.left += MAX(left_icon_width, right_icon_width);
			rect.right -= MAX(left_icon_width, right_icon_width);
		}
    }
    sub_rect = GetSubTextRect(title_ptr);

	if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
	{
		rect.right = rect.right - (sub_rect.right - sub_rect.left + 1);
	} 
	else
	{
		rect.left = rect.left + (sub_rect.right - sub_rect.left + 1);
	}

    return rect;
}

/*****************************************************************************/
// Description : get the title text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSubTextRect(
                                CTRLTITLE_OBJ_T *title_ptr
                                )
{
    GUI_RECT_T  rect = {0};
    
#ifdef GUIF_TITLE_PDASTYLE
    GUISTR_STYLE_T style_info = {0};
    MMI_STRING_T   str_ptr = {0};
    GUISTR_STATE_T       state = 0;
#endif
    uint16         str_width = 0;
	uint16         str_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return rect;
    }

    if (!title_ptr->is_need_sub_text || (0 == title_ptr->sub_str_len ))/*lint !e774*/
    {
        return rect;
    }

    // because title control's region begins from (0, 0), so we regard this region
    // as offset to title control.
#ifdef GUIF_TITLE_PDASTYLE
    rect.top = title_ptr->base_ctrl.rect.top;
    rect.bottom = title_ptr->base_ctrl.rect.bottom;
    rect.left = title_ptr->base_ctrl.rect.right - SUBTEXT_LEFT_OFFSET;
    rect.right = title_ptr->base_ctrl.rect.right - title_ptr->title_text_margin;
//////////////////////////////////////////////////////////////////////////
//fixed by hongjun.jia.通过获取subtext的宽度，动态设置subtext的显示区域。
#else
	str_width = GUI_GetStringWidth(title_ptr->sub_font,title_ptr->sub_str,title_ptr->sub_str_len);
	str_height = GUI_GetStringHeightEx(title_ptr->sub_font, title_ptr->sub_str, title_ptr->sub_str_len);
	if (str_width > SUBTEXT_MAX_L_MARGIN)
	{
		str_width = SUBTEXT_MAX_L_MARGIN;
	} 
    rect.bottom = title_ptr->base_ctrl.rect.bottom - SUBTEXT_BOTTOM_OFFSET;
    rect.top = rect.bottom - str_height;
    rect.left = title_ptr->base_ctrl.rect.right - str_width - SUBTEXT_RIGHT_OFFSET;
    rect.right = title_ptr->base_ctrl.rect.right - SUBTEXT_RIGHT_OFFSET;

    if (title_ptr->is_need_button
        && TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_RIGHT])
    {
        if(MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
        {
            rect.left  -= (title_ptr->base_ctrl.rect.right - title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left + 1);
            rect.right -= (title_ptr->base_ctrl.rect.right - title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left + 1);
        }
        //阿拉伯语等从右向左显示语言
        else
        {
            rect.left  += (title_ptr->base_ctrl.rect.left - title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right + 1);
            rect.right += (title_ptr->base_ctrl.rect.left - title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right + 1);
        }
    }
#endif

    //begin 20101202    
#ifdef MAINLCD_SIZE_128X64
    if (rect.bottom <= 0 )
    {
        rect.top = 0;
        rect.bottom = rect.top + (int16)title_ptr->sub_font - SUBTEXT_BOTTOM_OFFSET;
    }
#endif
    //end 20101202

#ifdef GUIF_TITLE_PDASTYLE
    if (title_ptr->is_need_button
        && TITLE_BUTTON_INVISIBLE != title_ptr->button_state[GUITITLE_BUTTON_RIGHT])
    {
        GUISTR_INFO_T info_ptr = {0};

        state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_WORDBREAK;
        
        style_info.align = title_ptr->sub_align;
        style_info.font = title_ptr->sub_font;
        style_info.font_color = title_ptr->str_style.font_color;
        
        str_ptr.wstr_len = title_ptr->sub_str_len;
        str_ptr.wstr_ptr = title_ptr->sub_str;
        
        GUISTR_GetStringInfo(&style_info, &str_ptr, state, &info_ptr);

        str_width = info_ptr.width; // GUISTR_GetStringWidth(&style_info, &str_ptr, state);
        
        // rect.top = title_ptr->rect.bottom - SUBTEXT_TOP_OFFSET;
        rect.bottom = title_ptr->base_ctrl.rect.bottom - SUBTEXT_BOTTOM_OFFSET;
        rect.top = rect.bottom - info_ptr.height;

		if(MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
		{
			rect.left  = title_ptr->base_ctrl.rect.right - title_ptr->title_text_margin - str_width + 1;
			rect.right = title_ptr->base_ctrl.rect.right - title_ptr->title_text_margin + 1;
			
			rect.left  -= (title_ptr->base_ctrl.rect.right - title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left + 1);
			rect.right -= (title_ptr->base_ctrl.rect.right - title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left + 1);
		}
		//阿拉伯语等从右向左显示语言
		else
		{
			rect.left  = title_ptr->base_ctrl.rect.left + title_ptr->title_text_margin + 1;
			rect.right = title_ptr->base_ctrl.rect.left + title_ptr->title_text_margin + str_width + 1;
			
			rect.left  += (title_ptr->base_ctrl.rect.left + title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right + 1);
			rect.right += (title_ptr->base_ctrl.rect.left + title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right + 1);
		}
	
    }
#else
    GUI_CheckRectDirection( &title_ptr->base_ctrl.rect, &rect);
#endif

    return (rect);
}

/*****************************************************************************/
// Description : display title background
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleBg(
                          CTRLTITLE_OBJ_T *title_ptr
                          )
{
    GUI_RECT_T       image_rect = {0};
    GUI_BG_DISPLAY_T bg_display = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    if (title_ptr->is_need_bg)
    {
        // 显示背景
        if (0 != title_ptr->bg_image && title_ptr->bg_image != MMITHEME_GetInvalidImageID())
        {
            image_rect.left = 0;
            image_rect.top = 0;
            image_rect.right = title_ptr->base_ctrl.rect.right - title_ptr->base_ctrl.rect.left;
            image_rect.bottom = title_ptr->base_ctrl.rect.bottom - title_ptr->base_ctrl.rect.top;
            
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = title_ptr->base_ctrl.rect;
            bg_display.display_rect   = title_ptr->base_ctrl.rect;
            bg_display.img_rect       = title_ptr->base_ctrl.rect;
            bg_display.ctrl_handle    = title_ptr->base_ctrl.handle;
            bg_display.win_handle     = title_ptr->base_ctrl.win_handle;

            //title_ptr->common_bg = MMITHEME_GetCommonBg();

//             GUI_DisplayBg(&title_ptr->common_bg,
//                 &bg_display,
//                 &title_ptr->lcd_dev_info);

            GUIRES_DisplayImg(
                NULL,
                &(title_ptr->base_ctrl.rect),
                &(image_rect),
                title_ptr->base_ctrl.win_handle,
                title_ptr->bg_image,
                &title_ptr->base_ctrl.lcd_dev_info
                );

        }
//#ifdef ADULT_WATCH_SUPPORT
        else
//#else
//        else if (0 != title_ptr->bg_color)
//#endif
        {
            LCD_FillRect(&title_ptr->base_ctrl.lcd_dev_info, title_ptr->base_ctrl.rect, title_ptr->bg_color);
        }
    }
}


/*****************************************************************************/
// Description : display title icon
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleIcon(
                            CTRLTITLE_OBJ_T    *title_ptr
                            )
{
#ifndef GUIF_TITLE_PDASTYLE
    uint16              icon_height = 0;
    GUI_POINT_T         dis_point = {0};
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    if (0 != title_ptr->icon_id)
    {
        //get image height
        GUIRES_GetImgWidthHeight(PNULL,
            &icon_height,
            title_ptr->icon_id,
            title_ptr->base_ctrl.win_handle);

        //set display point
        dis_point.x = title_ptr->base_ctrl.rect.left + 1;

        if (title_ptr->base_ctrl.rect.bottom > (icon_height + title_ptr->icon_space))
        {
            dis_point.y = title_ptr->base_ctrl.rect.bottom - icon_height - title_ptr->icon_space + 1;
        }
        else
        {
            dis_point.y = title_ptr->base_ctrl.rect.top;
        }
        
        DisplayTitleImg(&dis_point, PNULL, PNULL, title_ptr->base_ctrl.win_handle, title_ptr->icon_id, &title_ptr->base_ctrl.lcd_dev_info);
    }
#endif
}

/*****************************************************************************/
// Description : 显示图片,封装接口的目的是不要重复调用透明色的接口
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleImg(
                           GUI_POINT_T            *display_point_ptr,
                           GUI_RECT_T             *display_rect_ptr,
                           GUI_RECT_T             *image_rect_ptr,
                           MMI_HANDLE_T           win_handle,
                           MMI_IMAGE_ID_T         img_id,       
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           )
{
    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(display_point_ptr, 
        display_rect_ptr, 
        image_rect_ptr, 
        win_handle,
        img_id, 
        dev_info_ptr
        );

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetTitleButtonText(
                                      CTRLTITLE_OBJ_T    *title_ptr,
                                      uint8 button_idx
                                      )
{
    MMI_STRING_T    title_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return title_str;
    }

    if (GUITITLE_DATA_TEXT_ID == title_ptr->button_info[button_idx].text_type)
    {
        MMITHEME_GetResText( title_ptr->button_info[button_idx].text_id, title_ptr->base_ctrl.win_handle, &title_str );
    }
    else if (GUITITLE_DATA_TEXT_BUFFER == title_ptr->button_info[button_idx].text_type)
    {
        //get string
        title_str.wstr_ptr = title_ptr->button_info[button_idx].icon_str_ptr;
        title_str.wstr_len = title_ptr->button_info[button_idx].str_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return title_str;
}

/*****************************************************************************/
// 	Description : display the Istyle icon in title 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void DisplayButtonText(
                             CTRLTITLE_OBJ_T *title_ptr,
                             GUITITLE_BUTTON_INDEX_T icon_index
                             )
{
    MMI_TITLE_STYLE_T title_style = {0};
    
    MMITHEME_GetTitleStyle(&title_style);
    
    if (NULL != title_ptr && title_ptr->is_need_button)
    {
        //draw icon
        if (TITLE_BUTTON_INVISIBLE != title_ptr->button_state[icon_index])
        {
            MMI_STRING_T title_str = {0};
            GUI_ALIGN_E align_style = ALIGN_HVMIDDLE;
            GUISTR_STYLE_T str_style = {0}; /*lint !e64*/
            GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR|GUISTR_STATE_ELLIPSIS;
            MMI_STRING_T str_info = {0};
            
            GetTitleTextStyle(title_ptr, &str_style);
            
            //get string
            title_str = GetTitleButtonText(title_ptr, icon_index);
            
            //display text
            if ( (PNULL != title_str.wstr_ptr) && (0 < title_str.wstr_len) )
            {
                GUI_RECT_T text_rect = title_ptr->button_rect[icon_index];
                
                str_style.align = align_style;
                str_style.font = title_style.button_font_type;
                str_info.wstr_len = title_str.wstr_len;
                str_info.wstr_ptr = title_str.wstr_ptr;
                
                GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO*)&title_ptr->base_ctrl.lcd_dev_info,
                    (const GUI_RECT_T*)&text_rect,       //the fixed display area
                    (const GUI_RECT_T*)&text_rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T*)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO
                    );
            }
            else
            {
                //SCI_TRACE_LOW:"DisplayButtonText: text str is NULL!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITITLE_2230_112_2_18_3_22_24_371,(uint8*)"");
            }
        }
    }
}

/*****************************************************************************/
// Description : handle window tp message
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWinTitleTPMsg(
                                       MMI_MESSAGE_ID_E msg_id,
                                       GUI_POINT_T      point,
                                       CTRLTITLE_OBJ_T    *title_ptr
                                       )
{
    MMI_RESULT_E        recode       = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT    
    GUI_RECT_T          lbutton_rect  = {0};
    GUI_RECT_T          rbutton_rect  = {0};
    GUI_RECT_T          disp_rect  = {0};
    GUI_POINT_T         dis_point    = {0};
    
    MMI_TITLE_STYLE_T   title_style  = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (IGUICTRL_GetState((IGUICTRL_T *)title_ptr, GUICTRL_STATE_INVISIBLE))
    {
        return MMI_RESULT_FALSE;
    }
    
    MMITHEME_GetTitleStyle(&title_style);

    if (title_ptr->is_need_button)
    {
        MMI_IMAGE_ID_T image_id = 0;
        GUITITLE_BUTTON_INDEX_T icon_index = 0;

        lbutton_rect = title_ptr->button_rect[GUITITLE_BUTTON_LEFT];
        rbutton_rect = title_ptr->button_rect[GUITITLE_BUTTON_RIGHT];

        if (MSG_TP_PRESS_DOWN == msg_id)
        {
            BOOLEAN is_redraw = TRUE;

            if (GUI_PointIsInRect(point, lbutton_rect) && TITLE_BUTTON_NORMAL == title_ptr->button_state[0])
            {
                icon_index = GUITITLE_BUTTON_LEFT;
                //触笔点击左箭头
                dis_point.x = lbutton_rect.left;
                dis_point.y = lbutton_rect.top;

                disp_rect = lbutton_rect;

                image_id = title_ptr->button_info[GUITITLE_BUTTON_LEFT].press_image_id;
            }
            else if (GUI_PointIsInRect(point, rbutton_rect) && TITLE_BUTTON_NORMAL == title_ptr->button_state[1])
            {
                icon_index = GUITITLE_BUTTON_RIGHT;
                //触笔点击右箭头
                dis_point.x = rbutton_rect.left;
                dis_point.y = rbutton_rect.top;

                disp_rect = rbutton_rect;

                image_id = title_ptr->button_info[GUITITLE_BUTTON_RIGHT].press_image_id;
            }
            else
            {
                is_redraw = FALSE; // 不需要重画
            }

            if (is_redraw)
            {
                // 画背景
                DisplayTitleSubBg(title_ptr, disp_rect);
                // 画icon
                DisplayTitleImg(&dis_point, &disp_rect, PNULL, title_ptr->base_ctrl.win_handle, image_id, &title_ptr->base_ctrl.lcd_dev_info);
                // 画文字
                DisplayButtonText(title_ptr, icon_index);
            }
        }
        else if (MSG_TP_PRESS_UP == msg_id)
        {
            //cr00218015,tp up时不应该通过判断point区域来画arrow，这样会导致触笔拖到屏幕外后arrow高亮不消失 (by gokuGu)
            if (TITLE_BUTTON_NORMAL == title_ptr->button_state[0] || TITLE_BUTTON_NORMAL == title_ptr->button_state[1])
            {
                DisplayTitleArrow(title_ptr);
                
                DisplayButtonText(title_ptr, GUITITLE_BUTTON_LEFT);
                DisplayButtonText(title_ptr, GUITITLE_BUTTON_RIGHT);

                if (GUI_PointIsInRect(point, lbutton_rect) && TITLE_BUTTON_NORMAL == title_ptr->button_state[0] )
                {
                    if (MSG_NOTIFY_PENOK == title_ptr->button_msg[GUITITLE_BUTTON_LEFT])
                    {
                        MMI_NOTIFY_T notify = {0};

                        notify.src_handle = MMK_ConvertIdToHandle(title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_LEFT]);
                        notify.src_id     = title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_LEFT];
                        notify.code       = FALSE;

                        MMK_PostMsg(MMK_GetFocusWinHandle(), title_ptr->button_msg[GUITITLE_BUTTON_LEFT], &notify, sizeof(notify));
                    }
                    else
                    {
                        MMK_PostMsg(MMK_GetFocusWinHandle(), title_ptr->button_msg[GUITITLE_BUTTON_LEFT], (DPARAM)&title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_LEFT], sizeof(title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_LEFT]));
                    }

                    // MMK_PostMsg(title_ptr->base_ctrl.win_handle, MSG_CTL_TITLE_MOVE_PREV, NULL, 0);
                }
            
                if (GUI_PointIsInRect(point, rbutton_rect) && TITLE_BUTTON_NORMAL == title_ptr->button_state[1])
                {
                    if (MSG_NOTIFY_PENOK == title_ptr->button_msg[GUITITLE_BUTTON_RIGHT])
                    {
                        MMI_NOTIFY_T notify = {0};

                        notify.src_handle = MMK_ConvertIdToHandle(title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_RIGHT]);
                        notify.src_id     = title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_RIGHT];
                        notify.code       = FALSE;

                        MMK_PostMsg(MMK_GetFocusWinHandle(), title_ptr->button_msg[GUITITLE_BUTTON_RIGHT], &notify, sizeof(notify));
                    }
                    else
                    {
                        MMK_PostMsg(MMK_GetFocusWinHandle(), title_ptr->button_msg[GUITITLE_BUTTON_RIGHT], (DPARAM)&title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_RIGHT], sizeof(title_ptr->handle_msg_ctrl_id[GUITITLE_BUTTON_RIGHT]));
                    }

                    // MMK_PostMsg(title_ptr->base_ctrl.win_handle, MSG_CTL_TITLE_MOVE_NEXT, NULL, 0);
                }
            }
        }
        else if (MSG_TP_PRESS_MOVE == msg_id)
        {
            if (!GUI_PointIsInRect(point, lbutton_rect) && !GUI_PointIsInRect(point, rbutton_rect))
            {
                //触笔移出箭头区域
                DisplayTitleArrow(title_ptr);
                DisplayButtonText(title_ptr, GUITITLE_BUTTON_LEFT);
                DisplayButtonText(title_ptr, GUITITLE_BUTTON_RIGHT);
            }
        }
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }
#endif
    return recode;
}

/*****************************************************************************/
// Description : start title scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StartTitleScrollTimer(
                                 CTRLTITLE_OBJ_T *title_ptr
                                 )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    if( CalcTitleStrLenByPix(title_ptr->base_ctrl.win_handle, title_ptr) > GetTextRectWidth(title_ptr) )
    {
        if (MMK_IsFocusWin(title_ptr->base_ctrl.win_handle) && TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type
            && 0 == title_ptr->scroll_timer_id)
        {
            title_ptr->scroll_timer_id = MMK_CreateWinTimer(title_ptr->base_ctrl.handle, title_ptr->scroll_time, FALSE);
        }
    }
}

/*****************************************************************************/
// Description : stop title scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StopTitleScrollTimer(
                                CTRLTITLE_OBJ_T    *title_ptr
                                )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }
    
    if( 0 != title_ptr->scroll_timer_id )
    {
        MMK_StopTimer(title_ptr->scroll_timer_id);
        title_ptr->scroll_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : 获得文本的排版信息
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetFontAlign(
                               CTRLTITLE_OBJ_T *title_ptr
                               )
{
    GUI_ALIGN_E align_type = ALIGN_HMIDDLE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return align_type;
    }
    
    switch ( title_ptr->str_style.align )
    {
    case ALIGN_HMIDDLE:
        if (title_ptr->is_text_overstep)
        {
            align_type = ALIGN_LVMIDDLE;
        }
        else
        {
            align_type = ALIGN_HVMIDDLE;
        }
        break;
        
    case ALIGN_LEFT:
        align_type = ALIGN_LEFT;
        break;
        
    case ALIGN_RIGHT:
        align_type = ALIGN_RIGHT;
        break;

    case ALIGN_LVMIDDLE:
        align_type = ALIGN_LVMIDDLE;
        break;
        
    default:
        align_type = ALIGN_HMIDDLE;
        break;
        
    }

    return align_type;
}

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetTitleText(
                                CTRLTITLE_OBJ_T    *title_ptr
                                )
{
    MMI_STRING_T    title_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return title_str;
    }

    if (GUITITLE_DATA_TEXT_ID == title_ptr->text_type)
    {
        MMITHEME_GetResText( title_ptr->text_id, title_ptr->base_ctrl.win_handle, &title_str );
    }
    else if (GUITITLE_DATA_TEXT_BUFFER == title_ptr->text_type)
    {
        //get string
        title_str.wstr_ptr = title_ptr->title_str.wstr_ptr;
        title_str.wstr_len = title_ptr->title_str.wstr_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return title_str;
}

/*****************************************************************************/
// Description : display title text
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayTitleText(
                            CTRLTITLE_OBJ_T    *title_ptr
                            )
{
    MMI_STRING_T    title_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != title_ptr); /*assert verified*/
    if (PNULL == title_ptr)
    {
        return;
    }

    title_str = GetTitleText(title_ptr);

    //display text
    if ( (PNULL != title_str.wstr_ptr) && (0 < title_str.wstr_len) )
    {

        // disappeared from left byte by byte
        if( MMITHEME_GetIsScrollAsPixel() )
        {
            DisplayScrollTitleByOffset(title_str.wstr_ptr, title_str.wstr_len, title_ptr->scroll_pixel_offset, title_ptr);            
        }
        else
        {
            MMI_STRING_T    str_info = {0};
            GUI_RECT_T      text_rect =  GetTextRect(title_ptr);
            GUI_ALIGN_E     align_style = ALIGN_HVMIDDLE;
            GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
            GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;

            align_style = GetFontAlign(title_ptr);

            GetTitleTextStyle(title_ptr, &str_style);
            if (TITLE_SCROLL_LEFT_OUT == title_ptr->scroll_type)    
            { 
                if (title_ptr->scroll_char_offset < title_str.wstr_len)
                {
                    title_str.wstr_ptr += title_ptr->scroll_char_offset;
                    title_str.wstr_len  = (uint16)(title_str.wstr_len - title_ptr->scroll_char_offset);
                }
                if (title_ptr->scroll_char_offset != 0)
                {
                    align_style = ALIGN_LVMIDDLE;
                }
            }
            else if( TITLE_NOSCROLL_BY_ELLIPSIS == title_ptr->scroll_type )
            {
                str_state |= GUISTR_STATE_ELLIPSIS;
            }

            str_style.align = align_style;
            str_info.wstr_len = title_str.wstr_len;
            str_info.wstr_ptr = title_str.wstr_ptr;

            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO*)&title_ptr->base_ctrl.lcd_dev_info,
                (const GUI_RECT_T*)&text_rect,       //the fixed display area
                (const GUI_RECT_T*)&text_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T*)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO
                );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"DisplayTitle: text str is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITITLE_2592_112_2_18_3_22_25_372,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetButtonTextByPtr(    
                                 CTRLTITLE_OBJ_T *title_ptr,
                                 GUITITLE_BUTTON_INDEX_T icon_index,
                                 MMI_STRING_T *str_ptr
                                 )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr && icon_index < GUITITLE_BUTTON_NUM && PNULL != str_ptr)
    {
        title_ptr->button_info[icon_index].text_type = GUITITLE_DATA_TEXT_BUFFER;

        if (PNULL != title_ptr->button_info[icon_index].icon_str_ptr)
        {
            SCI_FREE(title_ptr->button_info[icon_index].icon_str_ptr);
            title_ptr->button_info[icon_index].icon_str_ptr = PNULL;
            title_ptr->button_info[icon_index].str_len = 0;
        }

        if (PNULL != str_ptr->wstr_ptr && 0 != str_ptr->wstr_len)
        {
            title_ptr->button_info[icon_index].icon_str_ptr = SCI_ALLOCA((str_ptr->wstr_len + 1)*sizeof(wchar));

            if (PNULL != title_ptr->button_info[icon_index].icon_str_ptr)
            {
                SCI_MEMSET(title_ptr->button_info[icon_index].icon_str_ptr, 0x00, ((str_ptr->wstr_len + 1)*sizeof(wchar)));

                MMI_MEMCPY(title_ptr->button_info[icon_index].icon_str_ptr,
                                    (str_ptr->wstr_len*sizeof(wchar)),
                                    str_ptr->wstr_ptr,
                                    (str_ptr->wstr_len*sizeof(wchar)),
                                    (str_ptr->wstr_len*sizeof(wchar)));

                title_ptr->button_info[icon_index].str_len = str_ptr->wstr_len;
            }
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : title set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetTitleContentRect( 
                                    CTRLTITLE_OBJ_T *title_ptr
                                    )
{
    BOOLEAN             result          = FALSE;
    uint16              image_width     = 0;
    uint16              image_height    = 0;
    MMI_TITLE_STYLE_T   title_style     = {0};

    // 参数检测
    if (PNULL != title_ptr && !GUI_IsInvalidRect(title_ptr->base_ctrl.rect))
    {
        MMITHEME_GetTitleStyle(&title_style);
        
        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_style.left_button_icon_up, title_ptr->base_ctrl.win_handle);

        title_ptr->button_rect[GUITITLE_BUTTON_LEFT] = title_ptr->button_rect[GUITITLE_BUTTON_RIGHT] = title_ptr->base_ctrl.rect;
        
        title_ptr->button_rect[GUITITLE_BUTTON_LEFT].left += title_style.button_margin;
        title_ptr->button_rect[GUITITLE_BUTTON_LEFT].right = title_ptr->button_rect[GUITITLE_BUTTON_LEFT].left + image_width - 1;
        
        title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].right -= title_style.button_margin;
        title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].left = title_ptr->button_rect[GUITITLE_BUTTON_RIGHT].right - image_width + 1;
        
        title_ptr->button_rect[GUITITLE_BUTTON_LEFT] = GUI_GetCenterRect(title_ptr->button_rect[GUITITLE_BUTTON_LEFT], image_width, image_height);
        title_ptr->button_rect[GUITITLE_BUTTON_RIGHT] = GUI_GetCenterRect(title_ptr->button_rect[GUITITLE_BUTTON_RIGHT], image_width, image_height);

        //istyle icon rect
        {
            uint32 i = 0;
            MMI_IMAGE_ID_T istyle_icon_id = 0;
            MMI_STRING_T title_str = {0};
            uint16 title_str_width[GUITITLE_BUTTON_NUM] = {0};
            uint16 display_width = 0;

            for (i = 0; i < GUITITLE_BUTTON_NUM; i++)
            {
                if (MMITHEME_CheckImageID(title_ptr->button_info[i].image_id))
                {
                    istyle_icon_id = title_ptr->button_info[i].image_id;
                }
                else
                {
                    if (GUITITLE_BUTTON_LEFT == i)
                    {
                        istyle_icon_id = title_style.left_button_icon_up;
                    }
                    else
                    {
                        istyle_icon_id = title_style.right_button_icon_up;
                    }
                }

                //get icon width/height
                GUIRES_GetImgWidthHeight(&image_width, &image_height, istyle_icon_id, title_ptr->base_ctrl.win_handle);

                title_ptr->button_rect[i] = title_ptr->base_ctrl.rect;

                //get string
                title_str = GetTitleButtonText(title_ptr, i);

                //get string width
                if ( (PNULL != title_str.wstr_ptr) && (0 < title_str.wstr_len) )
                {
                    GUISTR_STYLE_T str_style = {0}; /*lint !e64*/
                    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
                    GUI_ALIGN_E align_style = ALIGN_HVMIDDLE;
                    
                    GetTitleTextStyle(title_ptr, &str_style);

                    str_style.align = align_style;
                    str_style.font = title_style.button_font_type;

                    title_str_width[i] = GUISTR_GetStringWidth(&str_style, &title_str, str_state);
                }

                display_width = image_width + title_str_width[i];
                
                display_width = MIN(display_width,(title_ptr->base_ctrl.rect.right - title_ptr->base_ctrl.rect.left + 1)/3);

                if (GUITITLE_BUTTON_LEFT == i)
                {
                    title_ptr->button_rect[i].left += title_style.button_margin;
                    title_ptr->button_rect[i].right = title_ptr->button_rect[i].left + display_width - 1;
                }
                else
                {
                    title_ptr->button_rect[i].right -= title_style.button_margin;
                    title_ptr->button_rect[i].left = title_ptr->button_rect[i].right - display_width + 1;
                }

                title_ptr->button_rect[i] = GUI_GetCenterRect(title_ptr->button_rect[i], display_width, image_height);
            }
        }
        
        title_ptr->sub_text_rect = GetSubTextRect(title_ptr);
        
        title_ptr->scroll_char_offset = 0;
        title_ptr->scroll_pixel_offset = 0;
        title_ptr->is_text_overstep = FALSE;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : 清除sub text的显示信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_ClearSubTextInfo(    
                                         MMI_HANDLE_T   win_handle,
                                         MMI_CTRL_ID_T    ctrl_id
                                         )
{
    BOOLEAN            result = FALSE;
    CTRLTITLE_OBJ_T      *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if(PNULL != title_ptr)
    {
        SCI_MEMSET(title_ptr->sub_str, 0, sizeof(title_ptr->sub_str));
        title_ptr->sub_str_len = 0;
        
        if (MMK_IsFocusWin( win_handle ) && title_ptr->is_need_sub_text)
        {
            DisplayTitleSubBg(title_ptr, title_ptr->sub_text_rect);
            
            title_ptr->is_need_sub_text = FALSE;
        }

        result = TRUE;
    }
    
    return result;
}


/*****************************************************************************/
// Description : 动态创建状态栏控件
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_CreateCtrlDyn(
                                      MMI_HANDLE_T win_handle,
                                      MMI_CTRL_ID_T ctrl_id, 
                                      MMI_TEXT_ID_T text_id
                                      )
{
    BOOLEAN result = FALSE;
    GUITITLE_INIT_DATA_T   title_data   = {0};
    MMI_CONTROL_CREATE_T   create       = {0};
    
    title_data.text_id = text_id;
    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_TITLE_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &title_data;
    if (PNULL != MMK_CreateControl( &create ))
    {
        result = TRUE;
        MMK_SetWinTitleCtrlId( win_handle, create.ctrl_id );
    }

    return result;
}


/*****************************************************************************/
// Description : 获得title上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetButtonStatus(    
                                                        MMI_HANDLE_T   win_handle,
                                                        MMI_CTRL_ID_T    ctrl_id,
                                                        GUITITLE_BUTTON_INDEX_T   index
                                                        )
{
    return CTRLTITLE_GetRollArrowStatus(win_handle, ctrl_id, index);
}


/*****************************************************************************/
// Description : 获得title上arrow状态
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetRollArrowStatus(    
                                                          MMI_HANDLE_T   win_handle,
                                                          MMI_CTRL_ID_T    ctrl_id,
                                                          GUITITLE_BUTTON_INDEX_T   index
                                                          )
{
    CTRLTITLE_OBJ_T              *title_ptr = PNULL;
    GUITITLE_BUTTON_STATE_E status = TITLE_BUTTON_NORMAL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if (GUITITLE_BUTTON_LEFT != index && GUITITLE_BUTTON_RIGHT != index)
    {
        return status;
    }

    if(PNULL != title_ptr)
    {
        status = title_ptr->button_state[index];
    }
    
    return status;
}

/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC CTRLTITLE_OBJ_T *CTRLTITLE_GetTitlePtr(
                                             MMI_HANDLE_T  win_handle,
                                             MMI_CTRL_ID_T ctrl_id
                                             )
{
    return GetTitlePtr(win_handle, ctrl_id);
}

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
                                      )
{
    BOOLEAN           result  = FALSE;
    CTRLTITLE_OBJ_T      *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if(NULL != title_ptr)
    {
        title_ptr->bg_image = bg_image;
        title_ptr->bg_color = bg_color;
        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
// 	Description : Set Istyle Icon Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonHandleMsgInfo(    
                                               CTRLTITLE_OBJ_T *title_ptr,
                                               GUITITLE_BUTTON_INDEX_T icon_index,
                                               MMI_MESSAGE_ID_E button_msg_id,
                                               MMI_CTRL_ID_T handle_msg_ctrl_id
                                               )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr
        && icon_index < GUITITLE_BUTTON_NUM)
    {
        title_ptr->button_msg[icon_index] = button_msg_id;
        title_ptr->handle_msg_ctrl_id[icon_index] = handle_msg_ctrl_id;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Image
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonImage(    
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUITITLE_BUTTON_INDEX_T icon_index,
                                       MMI_IMAGE_ID_T image_id,
                                       MMI_IMAGE_ID_T press_image_id,
                                       BOOLEAN is_need_update
                                       )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr
        && icon_index < GUITITLE_BUTTON_NUM)
    {
        title_ptr->button_info[icon_index].image_id = image_id;
        title_ptr->button_info[icon_index].press_image_id = press_image_id;

        ResetTitleContentRect(title_ptr);

        if (is_need_update)
        {
            DisplayTitle(title_ptr);
        }
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon state
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonState(    
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUITITLE_BUTTON_INDEX_T icon_index,
                                       GUITITLE_BUTTON_STATE_E icon_state,
                                       BOOLEAN is_need_update
                                       )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr
        && icon_index < GUITITLE_BUTTON_NUM
        && icon_state < TITLE_BUTTON_STATE_MAX)
    {
        title_ptr->button_state[icon_index] = icon_state;

        ResetTitleContentRect(title_ptr);
        
        if (is_need_update)
        {
            DisplayTitle(title_ptr);
        }

        result = TRUE;
    }
    
    return result;
}
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
                                  )
{
    CTRLTITLE_SetFontColorByPtr(GetTitlePtr(win_handle,ctrl_id), font_color);
}

/*****************************************************************************/
// Description : set title font color
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_SetFontColorByPtr(
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUI_COLOR_T       font_color
                                       )
{
    if(NULL != title_ptr && TitleTypeOf((CTRLBASE_OBJ_T*)title_ptr ))
    {
        //set font color
        title_ptr->str_style.font_color = font_color;
    }
}

/*****************************************************************************/
// Description : 设置title区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRectByPtr(    
                                     CTRLTITLE_OBJ_T *title_ptr,
                                     GUI_RECT_T        rect
                                     )
{
    BOOLEAN         result = FALSE;
    
    if (PNULL != title_ptr 
        && TitleTypeOf((CTRLBASE_OBJ_T*)title_ptr ))
    {
        TitleSetRect((CTRLBASE_OBJ_T*)title_ptr, &rect);
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Title Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetIsVisible(    
                                     CTRLTITLE_OBJ_T *title_ptr,
                                     BOOLEAN is_visible,
                                     BOOLEAN is_update
                                     )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr)
    {
        IGUICTRL_SetState((IGUICTRL_T *)title_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN)(!is_visible));

        if ((is_update) &&
            (MMK_IsFocusWin(title_ptr->base_ctrl.win_handle)))
        {
            if (is_visible)
            {
                DisplayTitle(title_ptr);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Text
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonTextId(    
                                        CTRLTITLE_OBJ_T *title_ptr,
                                        GUITITLE_BUTTON_INDEX_T icon_index,
                                        MMI_TEXT_ID_T text_id,
                                        BOOLEAN is_need_update
                                        )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr
        && icon_index < GUITITLE_BUTTON_NUM)
    {
        title_ptr->button_info[icon_index].text_type = GUITITLE_DATA_TEXT_ID;

        title_ptr->button_info[icon_index].text_id = text_id;

        ResetTitleContentRect(title_ptr);

        if (is_need_update)
        {
            DisplayTitle(title_ptr);
        }

        result = TRUE;
    }
    
    return result;
}
/*****************************************************************************/
// Description : 设置title上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrow(    
                                     MMI_HANDLE_T   win_handle,
                                     MMI_CTRL_ID_T    ctrl_id,
                                     BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                     )
{
    BOOLEAN            result = FALSE;
    CTRLTITLE_OBJ_T      *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);

    if(NULL != title_ptr)
    {
        title_ptr->is_need_button = is_need_button;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 设置title上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrowStatus(    
                                           MMI_HANDLE_T   win_handle,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUITITLE_BUTTON_INDEX_T   index,
                                           GUITITLE_BUTTON_STATE_E  status
                                           )
{
    BOOLEAN         result      = FALSE;
    CTRLTITLE_OBJ_T    *title_ptr  = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if (GUITITLE_BUTTON_LEFT != index && GUITITLE_BUTTON_RIGHT != index)
    {
        return result;
    }
    
    if(PNULL != title_ptr)
    {
        title_ptr->button_state[index] = status;
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 设置title上是否有button
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetNeedButton(    
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                      )
{
    return CTRLTITLE_SetRollArrow(win_handle, ctrl_id, is_need_button);
}

/*****************************************************************************/
// Description : 设置title上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonStatus(    
                                        MMI_HANDLE_T   win_handle,
                                        MMI_CTRL_ID_T    ctrl_id,
                                        GUITITLE_BUTTON_INDEX_T   index,
                                        GUITITLE_BUTTON_STATE_E  status
                                        )
{
    return CTRLTITLE_SetRollArrowStatus(win_handle, ctrl_id, index, status);
}

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubText(    
                                   MMI_HANDLE_T     win_handle,
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_need_sub_text,   //是否需要列表索引信息
                                   const wchar      *wstr_ptr,        //[IN] string pointer
                                   uint16            wstr_len,        //[IN] string length
                                   BOOLEAN          is_need_update        //是否立即显示
                                   )
{
    return CTRLTITLE_SetSubTextByPtr(GetTitlePtr(win_handle, ctrl_id), is_need_sub_text, wstr_ptr, wstr_len, is_need_update);
}

/*****************************************************************************/
// Description : update the window title
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_Update(
                            MMI_HANDLE_T    win_handle,
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    CTRLTITLE_OBJ_T          *title_ptr = PNULL;

    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if(PNULL != title_ptr)
    {
        DisplayTitle(title_ptr);
    }
}
/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonTextByPtr(    
                                           CTRLTITLE_OBJ_T *title_ptr,
                                           GUITITLE_BUTTON_INDEX_T icon_index,
                                           MMI_STRING_T *str_ptr,
                                           BOOLEAN is_need_update
                                           )
{
    BOOLEAN result = FALSE;

    if (PNULL != title_ptr && icon_index < GUITITLE_BUTTON_NUM && PNULL != str_ptr)
    {
        SetButtonTextByPtr(title_ptr, icon_index, str_ptr);

        ResetTitleContentRect(title_ptr);

        if (is_need_update)
        {
            DisplayTitle(title_ptr);
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 设置title左边的小图标
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetIcon(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                MMI_IMAGE_ID_T  icon_id
                                )
{
    BOOLEAN          result = FALSE;
    CTRLTITLE_OBJ_T     *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);

    if(NULL != title_ptr)
    {
        title_ptr->icon_id = icon_id;
        result = TRUE;
    }
    
    return result;
}


/*****************************************************************************/
// Description : 设置title区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRect(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_RECT_T        rect
                                )
{
    BOOLEAN            result = FALSE;
    CTRLTITLE_OBJ_T       *title_ptr = PNULL;

    title_ptr = GetTitlePtr(win_handle, ctrl_id);

    if(PNULL != title_ptr)
    {
        result = CTRLTITLE_SetRectByPtr( title_ptr, rect );
    }
    
    return result;
}
/*****************************************************************************/
// Description : set the window title string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextByPtr(
                                     CTRLTITLE_OBJ_T  *title_ptr,
                                     const wchar*     wstr_ptr, //[IN] string pointer
                                     uint16           wstr_len, //[IN] string length, <=16
                                     BOOLEAN          is_need_update //[IN] is flash
                                     )
{
    BOOLEAN                result  = TRUE;
    
    if(PNULL == title_ptr)
    {
        return FALSE;
    }

    StopTitleScrollTimer(title_ptr);

    // 申请之前必须释放原来的内存，否则会产生内存泄漏
    ReleaseTitleStr(title_ptr);

    if (0 != wstr_len && NULL != wstr_ptr)
    {
        title_ptr->title_str.wstr_ptr = SCI_ALLOC_APP(((wstr_len + 1)*sizeof(wchar)));
        SCI_MEMSET(title_ptr->title_str.wstr_ptr, 0, ((wstr_len + 1)*sizeof(wchar)));
        title_ptr->title_str.wstr_len = wstr_len;
        
        MMI_WSTRNCPY( title_ptr->title_str.wstr_ptr, title_ptr->title_str.wstr_len, wstr_ptr, wstr_len, wstr_len );

        GUI_ReplaceChar(&title_ptr->title_str, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&title_ptr->title_str, CR_CHAR, BL_CHAR);
    }

    title_ptr->text_type = GUITITLE_DATA_TEXT_BUFFER;

    // 重新设置title的长度和滚动起点
    title_ptr->scroll_char_offset = 0;
    title_ptr->scroll_pixel_offset = 0;
    title_ptr->is_text_overstep = FALSE;
    
    if (is_need_update)
    {
        DisplayTitle(title_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextByPtr(    
                                        CTRLTITLE_OBJ_T  *title_ptr,       // 控件指针
                                        BOOLEAN          is_need_sub_text, // 是否需要sub文本
                                        const wchar      *wstr_ptr,           // [IN] string pointer
                                        uint16           wstr_len,           // [IN] string length
                                        BOOLEAN          is_need_update    // 是否立即显示
                                        )
{
    BOOLEAN    result = FALSE;
    GUI_RECT_T sub_text_rect = {0};

    if(PNULL != title_ptr)
    {    
        if(NULL != wstr_ptr)
        {
            title_ptr->is_need_sub_text = is_need_sub_text;
            
            if (GUITITLE_SUBTEXT_MAXLEN < wstr_len)
            {
                wstr_len = GUITITLE_SUBTEXT_MAXLEN;
            }
            
            SCI_MEMSET(title_ptr->sub_str, 0, sizeof(title_ptr->sub_str));

            MMI_WSTRNCPY(title_ptr->sub_str, (GUITITLE_SUBTEXT_MAXLEN), wstr_ptr, (wstr_len), (wstr_len));

            title_ptr->sub_str_len = wstr_len;
        }
        else
        {
            SCI_MEMSET(title_ptr->sub_str, 0, sizeof(title_ptr->sub_str));
            title_ptr->sub_str_len = wstr_len;
			title_ptr->is_need_sub_text = FALSE;
        }

        sub_text_rect = title_ptr->sub_text_rect;
        // 重新计算区域
        title_ptr->sub_text_rect = GetSubTextRect(title_ptr);

        if (is_need_update)
        {
            DisplayTitleSubBg(title_ptr, sub_text_rect);
            //显示列表索引信息等子文本
            DisplayTitleSubText(title_ptr, TRUE);
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParam(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUI_RECT_T      *text_rect_ptr,   // 子字符串的区域
                                        GUI_FONT_T      *font_ptr,        // 子字符串字体
                                        GUI_ALIGN_E     *align_ptr        // align type
                                        )
{
    BOOLEAN             result = FALSE;
    CTRLTITLE_OBJ_T     *title_ptr = PNULL;

    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    if (PNULL != title_ptr)
    {
        result = CTRLTITLE_SetSubTextParamByPtr(title_ptr,
                    text_rect_ptr,
                    font_ptr,
                    align_ptr);
    }

    return (result);
}

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParamByPtr(    
                                             CTRLTITLE_OBJ_T  *title_ptr,       // 控件指针
                                             GUI_RECT_T       *text_rect_ptr,   // 子字符串的区域
                                             GUI_FONT_T       *font_ptr,        // 子字符串字体
                                             GUI_ALIGN_E      *align_ptr        // align type
                                             )
{
    BOOLEAN result = FALSE;
    
    if(NULL != title_ptr)
    {
        if(NULL != text_rect_ptr)
        {
            title_ptr->sub_text_rect = *text_rect_ptr;
        }

        if (PNULL != font_ptr)
        {
            title_ptr->sub_font = *font_ptr;
        }

        if (PNULL != align_ptr)
        {
            title_ptr->sub_align = *align_ptr;
        }

        result = TRUE;
    }

    return result;
}

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
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLTITLE_OBJ_T        *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    if(NULL != title_ptr)
    {
        StopTitleScrollTimer(title_ptr);

        // text type
        title_ptr->text_type = GUITITLE_DATA_TEXT_ID;

        //  text id
        title_ptr->text_id = text_id;

        // 重新设置title的长度和滚动起点
        title_ptr->scroll_char_offset = 0;
        title_ptr->scroll_pixel_offset = 0;
        title_ptr->is_text_overstep = FALSE;
        
        result = TRUE;

        if (is_need_update)
        {
            DisplayTitle(title_ptr);
        }
    }

    return (result);
}

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
                                         )
{
    BOOLEAN           result  = FALSE;
    CTRLTITLE_OBJ_T          *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    if(PNULL != title_ptr)
    {
        title_ptr->str_style.align = align;
        result = TRUE;
    }

    return (result);
}

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
                                          )
{
    BOOLEAN                result  = FALSE;
    CTRLTITLE_OBJ_T        *title_ptr = PNULL;
    
    title_ptr = GetTitlePtr(win_handle, ctrl_id);
    
    // check the param
    if ( (TITLE_SCROLL_LEFT_OUT != scroll_type)
        && (TITLE_NOSCROLL_BY_ELLIPSIS != scroll_type))
    {
        //SCI_TRACE_LOW:"GUITITLE_SetTextScrollType: the type is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITITLE_1041_112_2_18_3_22_22_369,(uint8*)"");
        return (result);
    }

    if(PNULL != title_ptr)
    {
        
        title_ptr->scroll_type = scroll_type;
        result = TRUE;
    }
    
    return (result);
}
/*****************************************************************************/
// Description : set the window title string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetText(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                const wchar*    wstr_ptr, //[IN] string pointer
                                uint16          wstr_len, //[IN] string length, <=16
                                BOOLEAN         is_need_update //[IN] is flash
                                )
{
    return CTRLTITLE_SetTextByPtr(GetTitlePtr(win_handle, ctrl_id), wstr_ptr, 
wstr_len, is_need_update);
}


