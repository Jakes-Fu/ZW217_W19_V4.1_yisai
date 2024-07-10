/****************************************************************************
** File Name:      ctrltoolbar.c                                            *
** Author:         James.Zhang                                              *
** Date:           9/02/2010                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 9/2010         James.Zhang      Create                                   *
**                                                                          *
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "guitoolbar.h"
#ifdef GUIF_TOOLBAR
#include "mmitheme_toolbar.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "guires.h"

#include "ctrltoolbar.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
//  Description : init toolbar class
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarInitVtbl(
                             CTRLTOOLBAR_VTBL_T        *toolbar_vtbl_ptr
                             );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ToolbarCtrlPackInitParam(
                                  GUITOOLBAR_INIT_DATA_T     *toolbar_init_ptr,   //in
                                  CTRLTOOLBAR_INIT_PARAM_T   *toolbar_param_ptr   //out
                                  );

/*****************************************************************************/
//  Description : toolbar control construct 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarCtrlConstruct(
                               CTRLBASE_OBJ_T           *base_ctrl_ptr,
                               CTRLTOOLBAR_INIT_PARAM_T *init_data_ptr
                               );

/*****************************************************************************/
//  Description : toolbar control destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarCtrlDestruct(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr
                              );

/*****************************************************************************/
// Description : handle toolbar tp up
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleToolbarTpUp(
                                     CTRLTOOLBAR_OBJ_T *toolbar_ptr,
                                     GUI_POINT_T        point
                                     );

/*****************************************************************************/
//  Description : handle toolbar msg function
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ToolbarHandleMsg(
                                    CTRLBASE_OBJ_T          *base_ctrl_ptr,  //control pointer
                                    MMI_MESSAGE_ID_E    msg_id,     //message
                                    DPARAM              param       //add data
                                    );

/*****************************************************************************/
//  Description : toolbar set client rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarSetRect( 
                             CTRLBASE_OBJ_T *base_ctrl_ptr, 
                             const GUI_RECT_T *rect_ptr
                             );

/*****************************************************************************/
//  Description : get toolbar pointer by control id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLTOOLBAR_OBJ_T* GetToolbarPtr(
                                       MMI_HANDLE_T     ctrl_handle
                                       );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ToolbarTypeOf(
                            CTRLBASE_OBJ_T  *ctrl_ptr
                            );

/*****************************************************************************/
// Description : draw tool bar
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL void DrawToolbar(
                       CTRLTOOLBAR_OBJ_T *toolbar_ptr
                       );

/*****************************************************************************/
//  Description : draw toolbar bg
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarBg(
                         CTRLTOOLBAR_OBJ_T*      toolbar_ptr,
                         const GUI_LCD_DEV_INFO* dev_info_ptr,
                         GUI_RECT_T              bg_rect
                         );

/*****************************************************************************/
//  Description : draw sting
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarString( 
                             CTRLTOOLBAR_OBJ_T*       toolbar_ptr,
                             const GUI_LCD_DEV_INFO*  dev_info_ptr,
                             const wchar*             wstr_ptr, 
                             uint16                   wstr_len,
                             GUI_RECT_T               rect,
                             GUI_ALIGN_E              align
                             );

/*****************************************************************************/
//  Description : draw image
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarImage(
                            CTRLTOOLBAR_OBJ_T*      toolbar_ptr,
                            const GUI_LCD_DEV_INFO* dev_info_ptr,
                            MMI_IMAGE_ID_T          image_id,
                            GUI_RECT_T              rect
                            );

/*****************************************************************************/
// Description : draw tool bar all item
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL void DrawToolbarAllItem(
                              CTRLTOOLBAR_OBJ_T *toolbar_ptr
                              );

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       );

/*****************************************************************************/
//  Description : set the toolbar state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetToolbarState( 
                           CTRLTOOLBAR_OBJ_T* toolbar_ptr,
                           GUITOOLBAR_STATE_T src_state,
                           BOOLEAN is_true
                           );

/*****************************************************************************/
//  Description : get the toolbar state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetToolbarState(
                              CTRLTOOLBAR_OBJ_T* toolbar_ptr,
                              GUITOOLBAR_STATE_T src_state
                              );

/*****************************************************************************/
//  Description : validate toolbar
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 计算toolbar的区域和item的区域
/*****************************************************************************/
LOCAL void ValidateToolbar( 
                           CTRLTOOLBAR_OBJ_T* toolbar_ptr,
                           BOOLEAN is_recreate
                           );

/*****************************************************************************/
//  Description : create toolbar layer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void CreateToolbarLayer(
                              CTRLTOOLBAR_OBJ_T* toolbar_ptr
                              );

/*****************************************************************************/
//  Description : destroy toolbar layer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DestroyToolbarLayer(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr
                               );

/*****************************************************************************/
//  Description : Update toolbar ctrl
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void UpdateToolbarWnd(
                            CTRLTOOLBAR_OBJ_T* toolbar_ptr
                            );

/*****************************************************************************/
//  Description : show toolbar next page
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ShowNextPage(
                        CTRLTOOLBAR_OBJ_T* toolbar_ptr
                        );

/*****************************************************************************/
//  Description : create auto shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateShrinkTimer(
                                CTRLTOOLBAR_OBJ_T* toolbar_ptr
                                );

/*****************************************************************************/
//  Description : stop shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void StopShrinkTimer(
                           CTRLTOOLBAR_OBJ_T* toolbar_ptr
                           );

/*****************************************************************************/
//  Description : reset shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetShrinkTimer(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr
                               );

/*****************************************************************************/
//  Description : get toolbar show state
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL GUITOOLBAR_SHOW_TYPE_E GetToolbarShowType(
                                                CTRLTOOLBAR_OBJ_T* toolbar_ptr
                                                );

/*****************************************************************************/
//  Description : set toolbar show state
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void SetToolbarShowType(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                               GUITOOLBAR_SHOW_TYPE_E show_type
                               );

/*****************************************************************************/
//  Description : toolbar init runtime
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ToolbarInitRunTimeInfo(CTRLTOOLBAR_OBJ_T *toolbar_ptr);

/*****************************************************************************/
//  Description : play toolbar pop anim
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarDisplayAnim(
                                 CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                                 GUITOOLBAR_ANIM_DIRECTION_E direction,
                                 GUITOOLBAR_ANIM_ACTION_E act
                                 );

/*****************************************************************************/
//  Description : display pop anim frame
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void DisplayToolbarAnimFarme(
                                   CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                                   int16 offset_x, 
                                   int16 offset_y, 
                                   BOOLEAN is_last_frame
                                   );

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEqualPoint(
                           GUI_POINT_T pt1,
                           GUI_POINT_T pt2
                           );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get toolbar type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TOOLBAR_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "toolbar",
            sizeof(CTRLTOOLBAR_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)ToolbarCtrlConstruct,
            (OBJECT_DESTRUCTOR_FUNC)ToolbarCtrlDestruct,
            sizeof(CTRLTOOLBAR_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)ToolbarCtrlPackInitParam,
            sizeof(CTRLTOOLBAR_VTBL_T),
            (VTBL_INIT_FUNC)ToolbarInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init toolbar class
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarInitVtbl(
                             CTRLTOOLBAR_VTBL_T        *toolbar_vtbl_ptr
                             )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)toolbar_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = ToolbarHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = ToolbarSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ToolbarCtrlPackInitParam(
                                  GUITOOLBAR_INIT_DATA_T     *toolbar_init_ptr,   //in
                                  CTRLTOOLBAR_INIT_PARAM_T   *toolbar_param_ptr   //out
                                  )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)toolbar_param_ptr;

    base_ctrl_param_ptr->both_rect = toolbar_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : toolbar control construct 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarCtrlConstruct(
                               CTRLBASE_OBJ_T           *base_ctrl_ptr,
                               CTRLTOOLBAR_INIT_PARAM_T *init_data_ptr
                               )
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T tmp_rect = {0};
    
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = (CTRLTOOLBAR_OBJ_T*)base_ctrl_ptr;
    const CTRLTOOLBAR_INIT_PARAM_T* init_ptr = (const CTRLTOOLBAR_INIT_PARAM_T*)init_data_ptr;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != toolbar_ptr && PNULL != init_ptr ); /*assert verified*/
    if ((PNULL == toolbar_ptr)||(PNULL == init_ptr))
    {
        return FALSE;
    }
    
    MMITHEME_GetToolbarStyle( &toolbar_ptr->style );

    // TODO:
    // this is a bug
    base_ctrl_ptr->rect = tmp_rect;
    base_ctrl_ptr->display_rect = tmp_rect;
    
    toolbar_ptr->align = ALIGN_BOTTOM;
    
    toolbar_ptr->show_type = GUITOOLBAR_INVALID;
    toolbar_ptr->auto_shrink.interval = toolbar_ptr->style.auto_shrink_interval;
    toolbar_ptr->auto_shrink.is_open = TRUE;
    
    toolbar_ptr->total_num = 0;
    toolbar_ptr->click_index = GUITOOLBAR_INVALID_INDEX;
    
    toolbar_ptr->pop_anim.is_poping = FALSE;
    toolbar_ptr->pop_anim.dirction = GUITOOLBAR_DIR_DOWN;
    
    //set win handle
    toolbar_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
    
    // get resource attr
    GUIRES_GetImgWidthHeight(&img_width, &img_height, toolbar_ptr->style.bg_image, toolbar_ptr->win_handle);
    toolbar_ptr->res_attr.space = img_height;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, toolbar_ptr->style.bg_icon, toolbar_ptr->win_handle);
    toolbar_ptr->res_attr.icon_height = img_height;
    toolbar_ptr->res_attr.icon_width = img_width;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, toolbar_ptr->style.separator_img, toolbar_ptr->win_handle);
    toolbar_ptr->res_attr.separator_width = img_width;

    VTLBASE_SetCanActive(base_ctrl_ptr, FALSE);
    VTLBASE_SetTpPriority(base_ctrl_ptr, CTRL_TP_PRIORITY_HIGH);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : toolbar control destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarCtrlDestruct(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr
                              )
{
    CTRLTOOLBAR_OBJ_T   *toolbar_ptr = (CTRLTOOLBAR_OBJ_T*)base_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != toolbar_ptr); /*assert verified*/
    if (PNULL == toolbar_ptr)
    {
        return FALSE;
    }

    if ( PNULL != toolbar_ptr->item_info_ptr )
    {
        SCI_FREE( toolbar_ptr->item_info_ptr );
    }

    DestroyToolbarLayer( toolbar_ptr );

    return TRUE;
}

/*****************************************************************************/
//  Description : toolbar init runtime
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ToolbarInitRunTimeInfo(CTRLTOOLBAR_OBJ_T *toolbar_ptr)
{
    if (toolbar_ptr)
    {
        toolbar_ptr->run_time.hit_index = GUITOOLBAR_INVALID_INDEX;
        toolbar_ptr->run_time.is_hit_more = FALSE;
        toolbar_ptr->run_time.is_hit_icon = FALSE;
    }
}

/*****************************************************************************/
// Description : update hit item
// Global resource dependence : 
// Author: cheng.luo
// Note:
/*****************************************************************************/
LOCAL void UpdateHitItemRect(
                             CTRLTOOLBAR_OBJ_T *toolbar_ptr,
                             GUI_POINT_T        point
                             )
{
    uint16 i = 0;
    const GUITOOLBAR_ITEM_INFO_T* item_info_ptr = PNULL;
    uint16 index_begin = GUITOOLBAR_INVALID_INDEX;
    
    if (PNULL != toolbar_ptr)
    {
        if(GUITOOLBAR_EXPAND == toolbar_ptr->show_type)
        {
            toolbar_ptr->run_time.is_hit_icon = FALSE;
            
            // more
            if (!toolbar_ptr->run_time.is_show_end
                && GUI_PointIsInRect(point, toolbar_ptr->run_time.rect_more)) 
            {
                toolbar_ptr->run_time.hit_index = GUITOOLBAR_INVALID_INDEX;
                toolbar_ptr->run_time.is_hit_more = TRUE;
                DrawToolbar(toolbar_ptr);
            }
            else
            {
                if (0 < toolbar_ptr->run_time.page_max_items)
                {
                    index_begin = toolbar_ptr->run_time.cur_page * (toolbar_ptr->run_time.page_max_items - 1);
                    toolbar_ptr->run_time.is_hit_more = FALSE;
                    
                    for ( i = 0; i < toolbar_ptr->run_time.cur_page_items; i++ )
                    {
                        if (index_begin + i >= toolbar_ptr->total_num)
                        {
                            break;
                        }
                        
                        item_info_ptr = toolbar_ptr->item_info_ptr + index_begin + i;
                        
                        if (GUI_PointIsInRect(point, item_info_ptr->rect))
                        {
                            toolbar_ptr->run_time.hit_index = index_begin + i;
                            break;
                        }
                    }
                }             
                
                if (GUITOOLBAR_INVALID_INDEX != toolbar_ptr->run_time.hit_index)
                {
                    DrawToolbar(toolbar_ptr);
                }
            }
        }   
        else if (GUITOOLBAR_SHRINK == toolbar_ptr->show_type)
        {
            // icon highlight
            if (GUI_PointIsInRect(point, toolbar_ptr->icon_rect))
            {
                toolbar_ptr->run_time.is_hit_icon = TRUE;
                DrawToolbar(toolbar_ptr);
            }
        }
    }
}

/*****************************************************************************/
// Description : handle toolbar tp up
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleToolbarTpUp(
                                     CTRLTOOLBAR_OBJ_T *toolbar_ptr,
                                     GUI_POINT_T        point
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16 i = 0;
    uint16 index_begin = 0;
    const GUITOOLBAR_ITEM_INFO_T* item_info_ptr = PNULL;
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;

    // shrink state
    if (GUITOOLBAR_SHRINK == toolbar_ptr->show_type)
    {
        if (GUI_PointIsInRect(point, toolbar_ptr->icon_rect))
        {
            // pre expand
            if (PNULL != toolbar_ptr->func_ptr)
            {
                toolbar_ptr->func_ptr(base_ctrl_ptr->handle, toolbar_ptr->param_ptr);
            }
            
            SetToolbarShowType(toolbar_ptr, GUITOOLBAR_EXPAND);
        }
    }
    // expand state
    else if ( PNULL != toolbar_ptr->item_info_ptr )
    {
        // more
        if (!toolbar_ptr->run_time.is_show_end
            && GUI_PointIsInRect(point, toolbar_ptr->run_time.rect_more))   
        {
            ShowNextPage(toolbar_ptr);
        }
        else if (0 < toolbar_ptr->run_time.page_max_items)
        {
            index_begin = toolbar_ptr->run_time.cur_page * (toolbar_ptr->run_time.page_max_items - 1);
            
            for ( i = 0; i < toolbar_ptr->run_time.cur_page_items; i++ )
            {
                if (index_begin + i >= toolbar_ptr->total_num)
                {
                    break;
                }
                
                item_info_ptr = toolbar_ptr->item_info_ptr + index_begin + i;
                
                if ( GUI_PointIsInRect( point, item_info_ptr->rect ) )
                {
                    //whether gray
                    if (!item_info_ptr->is_gray)
                    {
                        toolbar_ptr->click_index = index_begin + i;
                        
                        CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_PENOK);
                        // set panel shrink
                        SetToolbarShowType(toolbar_ptr, GUITOOLBAR_SHRINK);
                    }
                    else
                    {
                        toolbar_ptr->click_index = GUITOOLBAR_INVALID_INDEX;
                    }
                    
                    break;
                }
                
            }
        }    
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle toolbar msg function
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ToolbarHandleMsg(
                                    CTRLBASE_OBJ_T          *base_ctrl_ptr,  //control pointer
                                    MMI_MESSAGE_ID_E    msg_id,     //message
                                    DPARAM              param       //add data
                                    )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T         point  = {0};
#endif
    CTRLTOOLBAR_OBJ_T   *toolbar_ptr = (CTRLTOOLBAR_OBJ_T*)base_ctrl_ptr;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != toolbar_ptr); /*assert verified*/
    if (PNULL == toolbar_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    ValidateToolbar( toolbar_ptr, TRUE);
    
    switch (msg_id)
    {
    case MSG_CTL_LOSE_FOCUS:
        // restore shrink
        StopShrinkTimer(toolbar_ptr);
        ToolbarInitRunTimeInfo(toolbar_ptr);
        toolbar_ptr->show_type = GUITOOLBAR_SHRINK;
        break;
    case MSG_CTL_GET_FOCUS:
        SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
        break;
        
    case MSG_CTL_CLOSE:
        StopShrinkTimer(toolbar_ptr);
        break;
        
    case MSG_CTL_PAINT:
        DrawToolbar(toolbar_ptr);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN: 
    case MSG_TP_PRESS_MOVE:
        {
            if (toolbar_ptr->pop_anim.is_poping)
            {
                return TRUE;
            }
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
            UpdateHitItemRect(toolbar_ptr, point);
            
            StopShrinkTimer(toolbar_ptr);
        }     
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            if (toolbar_ptr->pop_anim.is_poping)
            {
                return TRUE;
            }
            
            point.x=MMK_GET_TP_X(param);
            point.y=MMK_GET_TP_Y(param);
            
            result = HandleToolbarTpUp(toolbar_ptr, point);
            
            ToolbarInitRunTimeInfo(toolbar_ptr);
            DrawToolbar(toolbar_ptr);
            
            ResetShrinkTimer(toolbar_ptr);
        }       
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : toolbar set client rect
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarSetRect( 
                             CTRLBASE_OBJ_T *base_ctrl_ptr, 
                             const GUI_RECT_T *rect_ptr
                             )
{
    BOOLEAN           result = FALSE;
    CTRLTOOLBAR_OBJ_T *toolbar_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL!= iguictrl_ptr && PNULL != rect_ptr ); /*assert verified*/
    if ( PNULL == base_ctrl_ptr || PNULL == rect_ptr )
    {
        return FALSE;
    }
    
    toolbar_ptr = (CTRLTOOLBAR_OBJ_T *)base_ctrl_ptr;
    
    if ( !GUI_EqualRect( toolbar_ptr->client_rect, *rect_ptr ))
    {
        DestroyToolbarLayer(toolbar_ptr);
        
        toolbar_ptr->client_rect = *rect_ptr;
        
        SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get toolbar pointer by control id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLTOOLBAR_OBJ_T* GetToolbarPtr(
                                       MMI_HANDLE_T     ctrl_handle
                                       )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        if (!ToolbarTypeOf(ctrl_ptr))
        {
            return PNULL;
        }
    }
    
    return (CTRLTOOLBAR_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ToolbarTypeOf(
                            CTRLBASE_OBJ_T  *ctrl_ptr
                            )
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_TOOLBAR_TYPE);
}

/*****************************************************************************/
// Description : draw tool bar
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL void DrawToolbar(
                       CTRLTOOLBAR_OBJ_T *toolbar_ptr
                       )
{
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;
    
    GUITOOLBAR_SHOW_TYPE_E show_type = GetToolbarShowType(toolbar_ptr);
    if (GUITOOLBAR_INVALID == show_type)
    {
        return;
    }
    
    if ((0 != base_ctrl_ptr->lcd_dev_info.block_id) 
        && UILAYER_IsLayerActive(&base_ctrl_ptr->lcd_dev_info))
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        
        append_layer.lcd_dev_info = base_ctrl_ptr->lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_SetLayerColorKey(&base_ctrl_ptr->lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
        
        if (GetToolbarState(toolbar_ptr, GUITOOLBAR_STATE_VISIBLE))
        {
            UILAYER_AppendBltLayer(&append_layer);
        }
        else
        {
            UILAYER_RemoveBltLayer(&append_layer);
        }
        
        UILAYER_Clear(&base_ctrl_ptr->lcd_dev_info);
    }
    
    if (!(toolbar_ptr->pop_anim.is_poping) && GUITOOLBAR_SHRINK == show_type)
    {
        MMI_IMAGE_ID_T img_id = toolbar_ptr->style.bg_icon;
        
        if (toolbar_ptr->run_time.is_hit_icon)
        {
            img_id = toolbar_ptr->style.bg_icon_highlight;
        }
        if (GUI_RectIsCovered(toolbar_ptr->client_rect, toolbar_ptr->icon_rect))
        {
            DrawToolbarImage( toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, img_id, toolbar_ptr->icon_rect);
        }        
    }
    else
    {
        if (GUI_RectIsCovered(toolbar_ptr->client_rect, base_ctrl_ptr->rect))
        {
            DrawToolbarBg( toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, base_ctrl_ptr->rect);
            DrawToolbarAllItem( toolbar_ptr );
        }
    }
}

/*****************************************************************************/
//  Description : draw toolbar bg
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarBg(
                         CTRLTOOLBAR_OBJ_T*      toolbar_ptr,
                         const GUI_LCD_DEV_INFO* dev_info_ptr,
                         GUI_RECT_T              bg_rect
                         )
{
    GUI_RECT_T img_rect = {0};
    GUI_BG_T   bg       = {0};       
    GUI_BG_DISPLAY_T bg_disp = {0};
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;

    //set image rect
    img_rect.left   = 0;
    img_rect.top    = 0;
    img_rect.right  = (int16)(bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16)(bg_rect.bottom - bg_rect.top);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id  = toolbar_ptr->style.bg_image;
    
    bg_disp.is_transparent = FALSE;
    bg_disp.ctrl_handle = base_ctrl_ptr->handle;
    bg_disp.win_handle = toolbar_ptr->win_handle;
    bg_disp.is_form_bg = TRUE;
    bg_disp.display_rect = bg_rect;
    bg_disp.img_rect = img_rect;
    bg_disp.rect = base_ctrl_ptr->rect;
    
    //display bg
    GUI_DisplayBg(&bg, &bg_disp, (GUI_LCD_DEV_INFO *)dev_info_ptr);
}

/*****************************************************************************/
//  Description : draw sting
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarString( 
                             CTRLTOOLBAR_OBJ_T*       toolbar_ptr,
                             const GUI_LCD_DEV_INFO*  dev_info_ptr,
                             const wchar*             wstr_ptr, 
                             uint16                   wstr_len,
                             GUI_RECT_T               rect,
                             GUI_ALIGN_E              align
                             )
{
    GUISTR_STYLE_T str_style = {0}; 
    MMI_STRING_T   str_info = {0};
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != toolbar_ptr); /*assert verified*/
    if (PNULL == toolbar_ptr)
    {
        return;
    }
    
    str_style.align      = align;
    str_style.font       = toolbar_ptr->style.font;
    str_style.font_color = toolbar_ptr->style.font_color;
    
    str_info.wstr_ptr = (wchar *)wstr_ptr;
    str_info.wstr_len = wstr_len;
    
    GUISTR_DrawTextToLCDInRect(dev_info_ptr,
        (const GUI_RECT_T*)&rect, 
        (const GUI_RECT_T*)&rect, 
        (const MMI_STRING_T*)&str_info, 
        &str_style, 
        str_state, 
        GUISTR_TEXT_DIR_AUTO);
}

/*****************************************************************************/
//  Description : draw image
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawToolbarImage(
                            CTRLTOOLBAR_OBJ_T*      toolbar_ptr,
                            const GUI_LCD_DEV_INFO* dev_info_ptr,
                            MMI_IMAGE_ID_T          image_id,
                            GUI_RECT_T              rect
                            )
{
    uint16          icon_width  = 0; 
    uint16          icon_height = 0;
    GUI_RECT_T      icon_rect   = {0};
    
    if ( MMITHEME_CheckImageID( image_id )
        && !GUI_IsInvalidRect( rect ) )
    {
        //Get icon pointer from image_id
        GUIRES_GetImgWidthHeight( &icon_width, &icon_height, image_id, toolbar_ptr->win_handle );
        
        //Draw icon
        icon_rect = GUI_GetCenterRect( rect, icon_width, icon_height );

        IMG_EnableTransparentColor(TRUE); 

        GUIRES_DisplayImg( PNULL,
            &icon_rect,
            PNULL,
            toolbar_ptr->win_handle,
            image_id,
            dev_info_ptr );

        IMG_EnableTransparentColor(FALSE); 
    }
}

/*****************************************************************************/
// Description : draw tool bar all item
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL void DrawToolbarAllItem(
                              CTRLTOOLBAR_OBJ_T *toolbar_ptr
                              )
{
    uint16 i = 0;
    uint16 index_begin = 0;
    const GUITOOLBAR_ITEM_INFO_T* item_info_ptr = PNULL;
    GUI_RECT_T  rect = {0};
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;
    
    if ( PNULL != toolbar_ptr->item_info_ptr 
        && 0 < toolbar_ptr->run_time.page_max_items)
    {
        index_begin = toolbar_ptr->run_time.cur_page * (toolbar_ptr->run_time.page_max_items - 1);
        
        for ( i = 0; i < toolbar_ptr->run_time.cur_page_items; i++ )
        {
            if (index_begin + i >= toolbar_ptr->total_num)
            {
                break;
            }
            
            item_info_ptr = toolbar_ptr->item_info_ptr + index_begin + i;

//          move upper to show 
//             if (!item_info_ptr->is_gray
//                 && toolbar_ptr->run_time.hit_index == index_begin + i)
//             {             
//                 DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, toolbar_ptr->style.bg_hit, item_info_ptr->rect);
//             }
            
            switch( item_info_ptr->item.type )
            {
            case GUITOOLBAR_ITEM_TEXT_ID:
                {
                    MMI_STRING_T temp_str = {0};
                    
                    MMITHEME_GetResText( item_info_ptr->item.text_id, toolbar_ptr->win_handle, &temp_str );
                    
                    DrawToolbarString(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, temp_str.wstr_ptr, temp_str.wstr_len,
                        item_info_ptr->rect, ALIGN_HVMIDDLE);
                }
                break;
                
            case GUITOOLBAR_ITEM_IMAGE_ID:
            case GUITOOLBAR_ITEM_BOTH_ID:
                {
                    MMI_IMAGE_ID_T img_id = item_info_ptr->item.image_id;
                    
                    if (item_info_ptr->is_gray)
                    {
                        img_id = item_info_ptr->item.image_gray;
                    }
                    
                    DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, img_id, item_info_ptr->rect);   
                }
                break;
                
            default:
                break;
            }
            
            if (!item_info_ptr->is_gray
                && toolbar_ptr->run_time.hit_index == index_begin + i)
            {             
                DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, toolbar_ptr->style.bg_hit, item_info_ptr->rect);
            }

            // separator
            if (i < toolbar_ptr->run_time.cur_page_items - 1
                || !toolbar_ptr->run_time.is_show_end)
            {
                rect.left = item_info_ptr->rect.right;
                rect.right = rect.left + toolbar_ptr->res_attr.separator_width;
                rect.top = item_info_ptr->rect.top;
                rect.bottom = item_info_ptr->rect.bottom;

                DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, toolbar_ptr->style.separator_img, rect);
            }
        }
        
        // more
        if (!toolbar_ptr->run_time.is_show_end)
        {
            if (toolbar_ptr->run_time.is_hit_more)
            {
                DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, 
                    toolbar_ptr->style.bg_hit, toolbar_ptr->run_time.rect_more);
            }
            
            DrawToolbarImage(toolbar_ptr, &base_ctrl_ptr->lcd_dev_info, 
                toolbar_ptr->style.icon_more,
                toolbar_ptr->run_time.rect_more);
        }
    }
}

/*****************************************************************************/
//  Description : Update toolbar ctrl
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void UpdateToolbarWnd(
                            CTRLTOOLBAR_OBJ_T* toolbar_ptr
                            )
{
    if (NULL != toolbar_ptr)
    {
        CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;

        //MMK_SendMsg(toolbar_ptr->win_handle, MSG_FULL_PAINT, PNULL );
        MMK_SendMsg(base_ctrl_ptr->handle, MSG_CTL_PAINT, PNULL );
    }
}

/*****************************************************************************/
//  Description : calc offset point 
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void CalcOffsetPoint(
                           GUI_POINT_T *point_ptr, 
                           int16 offset_x, 
                           int16 offset_y
                           )
{
    if (PNULL != point_ptr)
    {
        point_ptr->x += offset_x;
        point_ptr->y += offset_y;
    }
}

/*****************************************************************************/
//  Description : calc offset rect
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void CalcOffsetRect(
                          GUI_RECT_T *rect_ptr, 
                          int16 offset_x, 
                          int16 offset_y
                          )
{
    if (PNULL != rect_ptr)
    {
        rect_ptr->left += offset_x;
        rect_ptr->right += offset_x;
        rect_ptr->top += offset_y;
        rect_ptr->bottom += offset_y;
    }
}

/*****************************************************************************/
//  Description : show toolbar next page
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ShowNextPage(
                        CTRLTOOLBAR_OBJ_T* toolbar_ptr
                       )
                    
{
    toolbar_ptr->run_time.cur_page++;
    SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE ); 
    UpdateToolbarWnd(toolbar_ptr);
}

/*****************************************************************************/
//  Description : display pop anim frame
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void DisplayToolbarAnimFarme(
                                   CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                                   int16 offset_x, 
                                   int16 offset_y, 
                                   BOOLEAN is_last_frame
                                   )
{
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;

    if (PNULL == toolbar_ptr)
    {
        return;
    }
    
    if (is_last_frame)
    {
        base_ctrl_ptr->rect = toolbar_ptr->pop_anim.rect_stop;
    }
    else
    {
        base_ctrl_ptr->rect = toolbar_ptr->pop_anim.rect_real;    
        CalcOffsetRect(&toolbar_ptr->pop_anim.rect_real, offset_x, offset_y);
    }
    
    SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
    ValidateToolbar(toolbar_ptr, FALSE);
    DrawToolbar(toolbar_ptr);
    
    if (is_last_frame)
    {
        //MMK_StopTimer(toolbar_ptr->pop_anim.timer_id);
        //toolbar_ptr->pop_anim.timer_id = 0;
        
        toolbar_ptr->pop_anim.is_poping = FALSE;
        
        if (GUITOOLBAR_SHRINK == toolbar_ptr->show_type)
        {
            ToolbarInitRunTimeInfo(toolbar_ptr);
            toolbar_ptr->run_time.is_show_end = TRUE;
            
            SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );    
            UpdateToolbarWnd (toolbar_ptr);
        }
        else if (GUITOOLBAR_EXPAND == toolbar_ptr->show_type)
        {
            ResetShrinkTimer(toolbar_ptr);
        }
    }
}

#if 0  // timer 方式动画
/*****************************************************************************/
//  Description : pop anim timer callback (timer 方式)
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ToolbarAnimCallback(
                               uint8 timer_id, 
                               uint32 param
                               )
{
    CTRLTOOLBAR_OBJ_T *toolbar_ptr = (CTRLTOOLBAR_OBJ_T *)param;
    
    if (PNULL == toolbar_ptr)
    {
        return;
    }

    if (timer_id == toolbar_ptr->pop_anim.timer_id)
    {
        GUITOOLBAR_ANIM_DIRECTION_E direction = toolbar_ptr->pop_anim.dirction;
            
        if (GUITOOLBAR_DIR_INVALID != (direction & GUITOOLBAR_DIR_VERTICAL))  // vertical
        {
            if (toolbar_ptr->pop_anim.rect_stop.top > toolbar_ptr->pop_anim.rect_start.top) // bottom
            {
                DisplayToolbarAnimFarme(toolbar_ptr, 0, toolbar_ptr->style.pop_anim_step_v,
                    toolbar_ptr->pop_anim.rect_stop.top <= toolbar_ptr->pop_anim.rect_real.top);

            }
            else    // top
            {
                DisplayToolbarAnimFarme(toolbar_ptr, 0, -toolbar_ptr->style.pop_anim_step_v,
                    toolbar_ptr->pop_anim.rect_stop.top >= toolbar_ptr->pop_anim.rect_real.top);
                
            }
        }
        else if (GUITOOLBAR_DIR_INVALID != (direction & GUITOOLBAR_DIR_HORIZONTAL))   // horizontal
        {
            if (toolbar_ptr->pop_anim.rect_stop.left > toolbar_ptr->pop_anim.rect_start.left)   // right
            {
                 DisplayToolbarAnimFarme(toolbar_ptr, toolbar_ptr->style.pop_anim_step_h, 0,
                    toolbar_ptr->pop_anim.rect_stop.left <= toolbar_ptr->pop_anim.rect_real.left);

            }
            else    // left
            {
                DisplayToolbarAnimFarme(toolbar_ptr, -toolbar_ptr->style.pop_anim_step_h, 0,
                    toolbar_ptr->pop_anim.rect_stop.left >= toolbar_ptr->pop_anim.rect_real.left);
            }
        }
   }
}
#endif

/*****************************************************************************/
//  Description : play toolbar pop anim (sleep 方式)
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void ShowToolbarAnim(CTRLTOOLBAR_OBJ_T* toolbar_ptr, GUITOOLBAR_ANIM_DIRECTION_E direction)
{
    if (GUITOOLBAR_DIR_INVALID != (direction & GUITOOLBAR_DIR_VERTICAL)) /*lint !e655*/ // vertical
    {
        if (toolbar_ptr->pop_anim.rect_stop.top > toolbar_ptr->pop_anim.rect_start.top) // bottom
        {
            while (toolbar_ptr->pop_anim.rect_stop.top > toolbar_ptr->pop_anim.rect_real.top)
            {
                DisplayToolbarAnimFarme(toolbar_ptr, 0, toolbar_ptr->style.pop_anim_step_v,
                    (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.top <= toolbar_ptr->pop_anim.rect_real.top));
                
                MMITHEME_UpdateRect();
                SCI_Sleep(toolbar_ptr->style.pop_anim_delay_ticks);
            }

            // last frame
            DisplayToolbarAnimFarme(toolbar_ptr, 0, toolbar_ptr->style.pop_anim_step_v,
                (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.top <= toolbar_ptr->pop_anim.rect_real.top));
        }
        else    // top
        {
            while(toolbar_ptr->pop_anim.rect_stop.top < toolbar_ptr->pop_anim.rect_real.top)
            {
                DisplayToolbarAnimFarme(toolbar_ptr, 0, -toolbar_ptr->style.pop_anim_step_v,
                    (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.top >= toolbar_ptr->pop_anim.rect_real.top));
                
                MMITHEME_UpdateRect();
                SCI_Sleep(toolbar_ptr->style.pop_anim_delay_ticks);
            }
            
            // last frame
            DisplayToolbarAnimFarme(toolbar_ptr, 0, -toolbar_ptr->style.pop_anim_step_v,
                (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.top >= toolbar_ptr->pop_anim.rect_real.top));
        }
    }
    else if (GUITOOLBAR_DIR_INVALID != (direction & GUITOOLBAR_DIR_HORIZONTAL)) /*lint !e655*/ // horizontal
    {
        if (toolbar_ptr->pop_anim.rect_stop.left > toolbar_ptr->pop_anim.rect_start.left)   // right
        {
            while(toolbar_ptr->pop_anim.rect_stop.left > toolbar_ptr->pop_anim.rect_real.left)
            {
                DisplayToolbarAnimFarme(toolbar_ptr, toolbar_ptr->style.pop_anim_step_h, 0,
                    (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.left <= toolbar_ptr->pop_anim.rect_real.left));
                
                MMITHEME_UpdateRect();
                SCI_Sleep(toolbar_ptr->style.pop_anim_delay_ticks);
            }
            
            // last frame
            DisplayToolbarAnimFarme(toolbar_ptr, toolbar_ptr->style.pop_anim_step_h, 0,
                (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.left <= toolbar_ptr->pop_anim.rect_real.left));
            
        }
        else    // left
        {
            while (toolbar_ptr->pop_anim.rect_stop.left < toolbar_ptr->pop_anim.rect_real.left)
            {
                DisplayToolbarAnimFarme(toolbar_ptr, -toolbar_ptr->style.pop_anim_step_h, 0,
                    (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.left >= toolbar_ptr->pop_anim.rect_real.left));
                
                MMITHEME_UpdateRect();
                SCI_Sleep(toolbar_ptr->style.pop_anim_delay_ticks);
            }
            
            // last frame
            DisplayToolbarAnimFarme(toolbar_ptr, -toolbar_ptr->style.pop_anim_step_h, 0,
                (BOOLEAN)(toolbar_ptr->pop_anim.rect_stop.left >= toolbar_ptr->pop_anim.rect_real.left));
        }
    }
}

/*****************************************************************************/
//  Description : play toolbar pop anim
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ToolbarDisplayAnim(
                                 CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                                 GUITOOLBAR_ANIM_DIRECTION_E direction, 
                                 GUITOOLBAR_ANIM_ACTION_E act
                                 )
{
    GUI_RECT_T rect = {0};
    int16 width = 0;
    int16 height = 0;
    
    if (PNULL == toolbar_ptr
        /*|| 0 != toolbar_ptr->pop_anim.timer_id*/)
    {
        return FALSE;
    }
    
    //GUICTRL_GetRect((IGUICTRL_T *)toolbar_ptr, &rect);
    rect = toolbar_ptr->panel_rect;
    
    toolbar_ptr->pop_anim.rect_start = rect;
    toolbar_ptr->pop_anim.rect_stop = rect;

    width = rect.right - rect.left + 1;
    height = rect.bottom - rect.top + 1;
    
    toolbar_ptr->pop_anim.dirction = direction;
    switch(direction)
    {
     case GUITOOLBAR_DIR_RIGHT:
        CalcOffsetRect(&rect, width, 0);
        break;
    case GUITOOLBAR_DIR_UP:
        CalcOffsetRect(&rect, 0, -height);
        break;     
    case GUITOOLBAR_DIR_DOWN:
        CalcOffsetRect(&rect, 0, height);
        break;
    case GUITOOLBAR_DIR_LEFT:
        CalcOffsetRect(&rect, -width, 0);
        break;
    default :
        break;
    }
    
    toolbar_ptr->pop_anim.act = act;
    if (GUITOOLBAR_ACT_SHRINK == act)
    {
        toolbar_ptr->pop_anim.rect_stop = rect;
    }
    else if (GUITOOLBAR_ACT_EXPAND == act)
    {
        toolbar_ptr->pop_anim.rect_start = rect;
    }
    toolbar_ptr->pop_anim.rect_real = toolbar_ptr->pop_anim.rect_start; 

    toolbar_ptr->pop_anim.is_poping = TRUE;

#if 0 // timer方式
    toolbar_ptr->pop_anim.timer_id = MMK_CreateTimerCallback(
                                            toolbar_ptr->style.pop_anim_interval,
                                            (MMI_TIMER_FUNC)ToolbarAnimCallback,
                                            (uint32)toolbar_ptr,
                                            TRUE);
#else    // sleep方式
    ShowToolbarAnim(toolbar_ptr, direction);
#endif
    
    return TRUE;
}

/*****************************************************************************/
//  Description : auto shrink timer callback
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void AutoShrinkCallback(
                              uint8 timer_id, 
                              uint32 param
                              )
{
    CTRLTOOLBAR_OBJ_T *toolbar_ptr = (CTRLTOOLBAR_OBJ_T *)param;
    if (PNULL == toolbar_ptr)
    {
        return;
    }
        
    if (timer_id == toolbar_ptr->auto_shrink.timer_id)
    {
        // 需要判断状态
        if (GUITOOLBAR_EXPAND == toolbar_ptr->show_type)
        {
            SetToolbarShowType(toolbar_ptr, GUITOOLBAR_SHRINK);
        }
    }
}

/*****************************************************************************/
//  Description : create auto shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateShrinkTimer(
                                CTRLTOOLBAR_OBJ_T* toolbar_ptr
                                )
{
    if (PNULL == toolbar_ptr 
        || !toolbar_ptr->auto_shrink.is_open)
    {
        return FALSE;
    }
    
    toolbar_ptr->auto_shrink.timer_id = MMK_CreateTimerCallback(
        toolbar_ptr->auto_shrink.interval,
        (MMI_TIMER_FUNC)AutoShrinkCallback,
        (uint32)toolbar_ptr,
        FALSE);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : stop shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void StopShrinkTimer(
                           CTRLTOOLBAR_OBJ_T* toolbar_ptr
                           )
{
    if (PNULL != toolbar_ptr && 0 != toolbar_ptr->auto_shrink.timer_id)
    {
        MMK_StopTimer(toolbar_ptr->auto_shrink.timer_id);
        toolbar_ptr->auto_shrink.timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : reset shrink timer
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetShrinkTimer(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr
                               )
{
    StopShrinkTimer(toolbar_ptr);
    
    return CreateShrinkTimer(toolbar_ptr);
}

/*****************************************************************************/
//  Description : ger toolbar show type
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL GUITOOLBAR_SHOW_TYPE_E GetToolbarShowType(CTRLTOOLBAR_OBJ_T* toolbar_ptr)
{
      if (PNULL == toolbar_ptr)
      {
          return GUITOOLBAR_INVALID;
      }
      
      return toolbar_ptr->show_type;
}

/*****************************************************************************/
//  Description : get toolbar show state
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void SetToolbarShowType(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr, 
                               GUITOOLBAR_SHOW_TYPE_E show_type
                               )
{
    if (PNULL == toolbar_ptr)
    {
        return;
    }
    
    if( show_type != toolbar_ptr->show_type
        && GUITOOLBAR_INVALID <=show_type 
        && GUITOOLBAR_TYPE_MAX > show_type)
    {
        toolbar_ptr->show_type = show_type;
        switch(show_type)
        {
        case GUITOOLBAR_SHRINK:
            StopShrinkTimer(toolbar_ptr);
            
            // play anim
            ToolbarDisplayAnim(toolbar_ptr, toolbar_ptr->pop_anim.dirction, GUITOOLBAR_ACT_SHRINK);
            
            break;
        case GUITOOLBAR_EXPAND:         
            // first frame, recreate layer
            SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
            ValidateToolbar( toolbar_ptr, TRUE);
            
            // play anim
            ToolbarDisplayAnim(toolbar_ptr, toolbar_ptr->pop_anim.dirction, GUITOOLBAR_ACT_EXPAND);
            
            break;
        default:
            return;
        }
    }
}

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEqualPoint(
                           GUI_POINT_T pt1,
                           GUI_POINT_T pt2
                           )
{
    BOOLEAN result = FALSE;
    
    if ((pt1.x == pt2.x)
        && (pt1.y == pt2.y))
    {
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    )
{
    uint32 state;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != dst_state_ptr); /*assert verified*/
    if (PNULL == dst_state_ptr)
    {
        return;
    }
    
    state = *dst_state_ptr;
    
    if( is_true )
    {
        state |= src_state;
    }
    else
    {
        state &= ~src_state;
    }
    
    *dst_state_ptr = state;
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       )
{
    return (BOOLEAN)( ( dst_state & src_state ) == src_state );
}

/*****************************************************************************/
//  Description : set the toolbar state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetToolbarState( 
                           CTRLTOOLBAR_OBJ_T*   toolbar_ptr,
                           GUITOOLBAR_STATE_T    src_state,
                           BOOLEAN is_true
                           )
{
    SetState( &toolbar_ptr->toolbar_state, src_state, is_true );
}

/*****************************************************************************/
//  Description : get the toolbar state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetToolbarState(
                              CTRLTOOLBAR_OBJ_T* toolbar_ptr,
                              GUITOOLBAR_STATE_T src_state
                              )
{
    return GetState( toolbar_ptr->toolbar_state, src_state );
}

/*****************************************************************************/
//  Description : restric rect in area
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void RestrictRectInArea(
                              GUI_RECT_T *rect_ptr,
                              GUI_RECT_T *area_ptr
                              )
{
    if (PNULL != rect_ptr && PNULL != area_ptr)
    {
        if (rect_ptr->top < area_ptr->top)
        {
            rect_ptr->bottom += area_ptr->top - rect_ptr->top;
            rect_ptr->top = area_ptr->top;
        }
        else if (rect_ptr->bottom > area_ptr->bottom)
        {
            rect_ptr->top += area_ptr->bottom - rect_ptr->bottom;
            rect_ptr->bottom = area_ptr->bottom;
        }
        
        if (rect_ptr->left < area_ptr->left)
        {
            rect_ptr->right += area_ptr->left - rect_ptr->left;
            rect_ptr->left = area_ptr->left;
        }
        else if (rect_ptr->right > area_ptr->right)
        {
            rect_ptr->left += area_ptr->right - rect_ptr->right;
            rect_ptr->right = area_ptr->right;
        }
    }
}

/*****************************************************************************/
//  Description : validate toolbar
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 计算toolbar的区域和item的区域
/*****************************************************************************/
LOCAL void ValidateToolbar( 
                           CTRLTOOLBAR_OBJ_T* toolbar_ptr,
                           BOOLEAN is_recreate
                           )
{
    uint32  i = 0;
    int32   show_max_count = 0;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;    
    int32   panel_width  = 0;
    uint16  index_begin = 0;
    uint16  left = 0;
    uint16  right = 0;
    uint16  top = 0;
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != toolbar_ptr); /*assert verified*/
    if (PNULL == toolbar_ptr)
    {
        return;
    }
    
    if (GetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID ))
    {
        if (toolbar_ptr->total_num > 0
            && PNULL != toolbar_ptr->item_info_ptr)
        {
            SCI_MEMSET((GUI_RECT_T*)&toolbar_ptr->panel_rect, 0, sizeof(GUI_RECT_T) );
            
            if (toolbar_ptr->res_attr.space > 0)
            {
                GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id, &lcd_width, &lcd_height);
                
                if (0 == toolbar_ptr->style.item_width + toolbar_ptr->res_attr.separator_width)
                {
                    return;
                }
                
                if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
                {
                    show_max_count = (toolbar_ptr->panel_pt.x - - toolbar_ptr->style.left_margin -  \
                        toolbar_ptr->style.right_margin) / (toolbar_ptr->style.item_width \
                        + toolbar_ptr->res_attr.separator_width);
                }
                else
                {
                    show_max_count = (lcd_width - toolbar_ptr->style.left_margin - toolbar_ptr->style.right_margin \
                        - toolbar_ptr->panel_pt.x) / (toolbar_ptr->style.item_width \
                        + toolbar_ptr->res_attr.separator_width);
                }
                
                if (0 >= show_max_count)
                {
                    return;
                }
                
                toolbar_ptr->run_time.page_max_items = show_max_count;                                
                index_begin = toolbar_ptr->run_time.cur_page * (show_max_count - 1);
                toolbar_ptr->run_time.cur_page_items = toolbar_ptr->total_num - index_begin;
                
                if (toolbar_ptr->run_time.cur_page_items > show_max_count)
                {
                    panel_width = toolbar_ptr->style.left_margin + toolbar_ptr->style.right_margin \
                        + toolbar_ptr->total_num * toolbar_ptr->style.item_width \
                        + (toolbar_ptr->total_num - 1) * toolbar_ptr->res_attr.separator_width;
                    
                    toolbar_ptr->run_time.cur_page_items = show_max_count - 1;  // 预留一个more
                    toolbar_ptr->run_time.is_show_end = FALSE;
                }
                else
                {
                    panel_width = toolbar_ptr->style.left_margin + toolbar_ptr->style.right_margin \
                        + toolbar_ptr->run_time.cur_page_items * toolbar_ptr->style.item_width \
                        + (toolbar_ptr->run_time.cur_page_items - 1) * toolbar_ptr->res_attr.separator_width;
                    
                    toolbar_ptr->run_time.is_show_end = TRUE;
                }
                
                if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
                {
                    toolbar_ptr->panel_rect.right = toolbar_ptr->panel_pt.x;
                    toolbar_ptr->panel_rect.left = toolbar_ptr->panel_rect.right - panel_width + 1;
                }
                else
                {
                    toolbar_ptr->panel_rect.left = toolbar_ptr->panel_pt.x;
                    toolbar_ptr->panel_rect.right = toolbar_ptr->panel_rect.left + panel_width - 1;
                }
                
                toolbar_ptr->panel_rect.bottom = toolbar_ptr->panel_pt.y;
                toolbar_ptr->panel_rect.top = toolbar_ptr->panel_rect.bottom - toolbar_ptr->res_attr.space + 1;              
                
                RestrictRectInArea(&toolbar_ptr->panel_rect, &toolbar_ptr->client_rect);
                
                if (is_recreate)
                {
                    left = toolbar_ptr->panel_rect.left + toolbar_ptr->style.left_margin - 1;
                    right = toolbar_ptr->panel_rect.right - toolbar_ptr->style.right_margin + 1;
                    top = toolbar_ptr->panel_rect.top + toolbar_ptr->style.top_margin - 1;
                }
                else
                {
                    left = base_ctrl_ptr->rect.left + toolbar_ptr->style.left_margin - 1;
                    right = base_ctrl_ptr->rect.right - toolbar_ptr->style.right_margin + 1;
                    top = base_ctrl_ptr->rect.top + toolbar_ptr->style.top_margin - 1;
                }
                
                if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
                {
                    for (i = 0; i < toolbar_ptr->run_time.cur_page_items; i++)
                    {
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.right = right;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.left = toolbar_ptr->item_info_ptr[i + index_begin].rect.right \
                            - toolbar_ptr->style.item_width + 1;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.top = top;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.bottom = toolbar_ptr->item_info_ptr[i + index_begin].rect.top \
                            + toolbar_ptr->style.item_height - 1;
                        
                        right -= toolbar_ptr->res_attr.separator_width + toolbar_ptr->style.item_width;
                    }
                    
                    if (!toolbar_ptr->run_time.is_show_end )
                    {
                        toolbar_ptr->run_time.rect_more.right = right;
                        toolbar_ptr->run_time.rect_more.left = toolbar_ptr->run_time.rect_more.right \
                            - toolbar_ptr->style.item_width + 1;
                        toolbar_ptr->run_time.rect_more.top = top;
                        toolbar_ptr->run_time.rect_more.bottom = toolbar_ptr->run_time.rect_more.top \
                            + toolbar_ptr->style.item_height - 1;
                    }
                }
                else
                {
                    for (i = 0; i < toolbar_ptr->run_time.cur_page_items; i++)
                    {
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.left = left;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.right = toolbar_ptr->item_info_ptr[i + index_begin].rect.left \
                            + toolbar_ptr->style.item_width - 1;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.top = top;
                        toolbar_ptr->item_info_ptr[i + index_begin].rect.bottom = toolbar_ptr->item_info_ptr[i + index_begin].rect.top \
                            + toolbar_ptr->style.item_height - 1;
                        
                        left += toolbar_ptr->res_attr.separator_width + toolbar_ptr->style.item_width;
                    }
                    
                    if (!toolbar_ptr->run_time.is_show_end )
                    {
                        toolbar_ptr->run_time.rect_more.left = left;
                        toolbar_ptr->run_time.rect_more.right = toolbar_ptr->run_time.rect_more.left \
                            + toolbar_ptr->style.item_width - 1;
                        toolbar_ptr->run_time.rect_more.top = top;
                        toolbar_ptr->run_time.rect_more.bottom = toolbar_ptr->run_time.rect_more.top \
                            + toolbar_ptr->style.item_height - 1;
                    }
                }
            } 
         
            if (is_recreate)
            {
                if (GUITOOLBAR_SHRINK == toolbar_ptr->show_type)
                {
                    base_ctrl_ptr->rect = toolbar_ptr->icon_rect;
                }
                else
                {
                    base_ctrl_ptr->rect = toolbar_ptr->panel_rect;
                }
                
                DestroyToolbarLayer( toolbar_ptr );
                CreateToolbarLayer( toolbar_ptr );
            }
        } 

        SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, FALSE );     
    }
}

/*****************************************************************************/
//  Description : create toolbar layer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void CreateToolbarLayer(
                              CTRLTOOLBAR_OBJ_T* toolbar_ptr
                              )
{   
    GUI_RECT_T       page_rect       = {0};
    UILAYER_CREATE_T create_info = {0};
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;
    
    if (PNULL != toolbar_ptr)
    {
        page_rect = base_ctrl_ptr->rect;

        if ( 0 == base_ctrl_ptr->lcd_dev_info.block_id
            && !GUI_IsInvalidRect( page_rect ) )
        {
            // 创建层
            create_info.lcd_id = base_ctrl_ptr->lcd_dev_info.lcd_id;
            create_info.owner_handle = base_ctrl_ptr->handle;
            create_info.offset_x = page_rect.left;
            create_info.offset_y = page_rect.top;
            create_info.width = (page_rect.right + 1 - page_rect.left);
            create_info.height = (page_rect.bottom + 1 - page_rect.top);
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE; 
            UILAYER_CreateLayer(
                &create_info,  
                &base_ctrl_ptr->lcd_dev_info
                );
        }
    }
}


/*****************************************************************************/
//  Description : destroy toolbar layer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DestroyToolbarLayer(
                               CTRLTOOLBAR_OBJ_T* toolbar_ptr
                               )
{
    CTRLBASE_OBJ_T   *base_ctrl_ptr = (CTRLBASE_OBJ_T*)toolbar_ptr;

    // 释放层
    UILAYER_RELEASELAYER(&base_ctrl_ptr->lcd_dev_info);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&base_ctrl_ptr->lcd_dev_info);
    
    base_ctrl_ptr->lcd_dev_info.block_id = 0;
}

/*****************************************************************************/
//  Description : create dynamic toolbar
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLTOOLBAR_CreateDynamic(
                                             MMI_WIN_ID_T            win_id,     //in:
                                             MMI_CTRL_ID_T           ctrl_id,    //in:
                                             GUITOOLBAR_INIT_DATA_T   *init_ptr   //in:
                                             )
{
    MMI_HANDLE_T handle = 0;
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLBASE_OBJ_T          *ctrl_ptr = PNULL;
    
    //set control param
    ctrl_create.ctrl_id           = ctrl_id;
    ctrl_create.guid              = SPRD_GUI_TOOLBAR_ID;
    ctrl_create.init_data_ptr     = init_ptr;
    ctrl_create.parent_win_handle = win_id;
    
    //creat control
    ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_CreateControl(&ctrl_create);
    if (PNULL != ctrl_ptr)
    {
        handle = ctrl_ptr->handle;
    }

    return (handle);
}

/*****************************************************************************/
//  Description : open or close auto shrink timer
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTOOLBAR_SetAutoShrink(
                                     MMI_HANDLE_T ctrl_handle,
                                     BOOLEAN is_open
                                     )
{
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    if (PNULL == toolbar_ptr)
    {
        return;
    }
    
    if (is_open != toolbar_ptr->auto_shrink.is_open)
    {
        toolbar_ptr->auto_shrink.is_open = is_open;
        
        if (is_open)
        {
            ResetShrinkTimer(toolbar_ptr);
        }
        else
        {
            StopShrinkTimer(toolbar_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set shrink timer out
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAutoShrinkInterval(
                                                MMI_HANDLE_T ctrl_handle, 
                                                uint32 interval
                                                )
{
     BOOLEAN ret = FALSE;
     CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
     if (PNULL == toolbar_ptr || 0 == interval)
     {
         return ret;
     }

     toolbar_ptr->auto_shrink.interval = interval;

     ret = ResetShrinkTimer(toolbar_ptr);

     return ret;
}

/*****************************************************************************/
//  Description : get show state
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC GUITOOLBAR_SHOW_TYPE_E CTRLTOOLBAR_GetShowType(
                                                       MMI_HANDLE_T ctrl_handle
                                                       )
{
    GUITOOLBAR_SHOW_TYPE_E toolbar_type = GUITOOLBAR_INVALID;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    toolbar_type = GetToolbarShowType(toolbar_ptr);

    return toolbar_type;
}

/*****************************************************************************/
//  Description : set show state
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTOOLBAR_SetShowType(
                                    MMI_HANDLE_T ctrl_handle,
                                    GUITOOLBAR_SHOW_TYPE_E show_type
                                    )
{
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    SetToolbarShowType(toolbar_ptr, show_type);
}

/*****************************************************************************/
//  Description : set visible
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTOOLBAR_SetVisible(
                                  MMI_HANDLE_T             ctrl_handle,
                                  BOOLEAN                  is_visible
                                  )
{
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if (PNULL != toolbar_ptr)
    {
        if (is_visible != GetToolbarState(toolbar_ptr, GUITOOLBAR_STATE_VISIBLE))
        {
            SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_VISIBLE, is_visible );
            if (is_visible)
            {
                if (GUITOOLBAR_EXPAND == toolbar_ptr->show_type)
                {
                    ResetShrinkTimer(toolbar_ptr);
                }
            }
            else
            {
                StopShrinkTimer(toolbar_ptr);
            }
            
            UpdateToolbarWnd(toolbar_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set toolbar all item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAllItem(
                                     MMI_HANDLE_T             ctrl_handle,
                                     const GUITOOLBAR_ITEM_T* item_ptr,
                                     uint16                   total_num
                                     )
{
    BOOLEAN            result      = FALSE;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    uint32             i           = 0; 

    if( PNULL != toolbar_ptr )
    {
        if ( PNULL != toolbar_ptr->item_info_ptr )
            
        {
            SCI_FREE( toolbar_ptr->item_info_ptr );
            toolbar_ptr->item_info_ptr = PNULL;
            
            toolbar_ptr->total_num = 0;
        }
        
        if( PNULL != item_ptr && 0 != total_num )
        {
            toolbar_ptr->item_info_ptr = (GUITOOLBAR_ITEM_INFO_T*)SCI_ALLOC_APP( total_num * sizeof(GUITOOLBAR_ITEM_INFO_T) );
            SCI_MEMSET((GUITOOLBAR_ITEM_INFO_T *)toolbar_ptr->item_info_ptr, 0, total_num * sizeof(GUITOOLBAR_ITEM_INFO_T) );
            
            for ( i = 0; i < total_num; i++ )
            {
                toolbar_ptr->item_info_ptr[i].item = item_ptr[i];
            }
            
            toolbar_ptr->total_num = total_num;
            
            SCI_MEMSET((GUITOOLBAR_RUNTIME_INFO *)&toolbar_ptr->run_time, 0, sizeof(GUITOOLBAR_RUNTIME_INFO));
        }
        
        ToolbarInitRunTimeInfo(toolbar_ptr);
        toolbar_ptr->show_type = GUITOOLBAR_SHRINK;
        SetToolbarState(toolbar_ptr, GUITOOLBAR_STATE_VISIBLE, TRUE);
        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get cur index
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLTOOLBAR_GetClickItemIndex(
                                         MMI_HANDLE_T ctrl_handle
                                         )
{
    uint16 click_index = GUITOOLBAR_INVALID_INDEX;
    CTRLTOOLBAR_OBJ_T *toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if( PNULL != toolbar_ptr )
    {
        click_index =  toolbar_ptr->click_index;
    }

    return click_index;
}

/*****************************************************************************/
//  Description : get item ptr
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const GUITOOLBAR_ITEM_T* CTRLTOOLBAR_GetItemPtr(
                                                      MMI_HANDLE_T ctrl_handle,
                                                      uint16       index
                                                      )
{
    GUITOOLBAR_ITEM_T* item_ptr = PNULL;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr )
    {
        if (GUITOOLBAR_INVALID_INDEX != index
            && index < toolbar_ptr->total_num
            && PNULL != toolbar_ptr->item_info_ptr )
        {
            GUITOOLBAR_ITEM_INFO_T* item_info_ptr = toolbar_ptr->item_info_ptr + index;
            
            item_ptr = &item_info_ptr->item;
        }
    }

    return item_ptr;
}

/*****************************************************************************/
//  Description : get item user data
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLTOOLBAR_GetItemUserDate(
                                         MMI_HANDLE_T ctrl_handle,
                                         uint16       index
                                         )
{
    GUITOOLBAR_ITEM_T* item_ptr = PNULL;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr(ctrl_handle);
    
    if ( PNULL != toolbar_ptr )
    {
        if (GUITOOLBAR_INVALID_INDEX != index
            && index < toolbar_ptr->total_num
            && PNULL != toolbar_ptr->item_info_ptr )
        {
            GUITOOLBAR_ITEM_INFO_T* item_info_ptr = toolbar_ptr->item_info_ptr + index;
            
            item_ptr = &item_info_ptr->item;
            return item_ptr->user_data;
        }
    }

    return 0;
}

/*****************************************************************************/
//  Description : set item whether gray or not
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTOOLBAR_SetItemGray(
                                  MMI_HANDLE_T ctrl_handle,
                                  uint16       index,
                                  BOOLEAN      is_gray
                                  )
{
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr )
    {
        if (GUITOOLBAR_INVALID_INDEX != index
            && index < toolbar_ptr->total_num
            && PNULL != toolbar_ptr->item_info_ptr)
        {
            GUITOOLBAR_ITEM_INFO_T* item_info_ptr = toolbar_ptr->item_info_ptr + index;
            
            if(is_gray != item_info_ptr->is_gray)
            {
                item_info_ptr->is_gray = is_gray;
            }
        }
    }
}

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_GetIconWidthHeight(
                                             MMI_HANDLE_T    ctrl_handle,
                                             uint16          *width_ptr,
                                             uint16          *height_ptr
                                             )
{
    BOOLEAN           result = FALSE;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr)
    {
        *width_ptr = toolbar_ptr->res_attr.icon_width;
        *height_ptr = toolbar_ptr->res_attr.icon_height;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetIconRect(
                                   MMI_HANDLE_T                ctrl_handle,
                                   const GUI_RECT_T            *rect_ptr,
                                   BOOLEAN                      is_update
                                   )
{
    BOOLEAN           result = FALSE;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr && PNULL != rect_ptr)
    {
        if (!GUI_EqualRect( toolbar_ptr->icon_rect, *rect_ptr ))
        {
            toolbar_ptr->icon_rect = *rect_ptr;
            
            SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
            if (is_update)
            {
                UpdateToolbarWnd(toolbar_ptr);
            }
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelBasePt(
                                      MMI_HANDLE_T              ctrl_handle,
                                      const GUI_POINT_T         *pt_ptr,
                                      BOOLEAN                   is_update
                                      )
{
    BOOLEAN           result = FALSE;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr && PNULL != pt_ptr)
    {
        if (!IsEqualPoint( toolbar_ptr->panel_pt, *pt_ptr ))
        {
            toolbar_ptr->panel_pt = *pt_ptr;
            
            SetToolbarState( toolbar_ptr, GUITOOLBAR_STATE_INVALID, TRUE );
            if (is_update)
            {
                UpdateToolbarWnd(toolbar_ptr);
            }
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set pre expand func
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC TOOLBAR_PRE_EXPAND_FUNC CTRLTOOLBAR_SetPreExpandFunc(
                                                           MMI_HANDLE_T                ctrl_handle,
                                                           TOOLBAR_PRE_EXPAND_FUNC     func_ptr,
                                                           void*                       param_ptr
                                                           )
{
    TOOLBAR_PRE_EXPAND_FUNC     old_func_ptr = PNULL;

    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle );
    
    if ( PNULL != toolbar_ptr )
    {
        old_func_ptr = toolbar_ptr->func_ptr;
        toolbar_ptr->func_ptr = func_ptr; 
        toolbar_ptr->param_ptr = param_ptr;
    }

    return old_func_ptr;
}

/*****************************************************************************/
//  Description : set panel pop form
//  Global resource dependence : 
//  Author:cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelPopForm(
                                          MMI_HANDLE_T                   ctrl_handle,
                                          GUITOOLBAR_ANIM_DIRECTION_E    direction
                                          )
{
    BOOLEAN           result = FALSE;
    CTRLTOOLBAR_OBJ_T* toolbar_ptr = GetToolbarPtr( ctrl_handle ); 
    
    if ( PNULL != toolbar_ptr )
    {
        if (direction != toolbar_ptr->pop_anim.dirction)
        {
            toolbar_ptr->pop_anim.dirction = direction;
            result = TRUE;
        }
    }

    return result;
}

#endif

/*Edit by script, ignore 4 case. Thu Apr 26 19:01:15 2012*/ //IGNORE9527
