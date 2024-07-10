/*****************************************************************************
** File Name:      mmi3d.c                                            *
** Author:                                                                   *
** Date:           02/07/2009                                                *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi3d                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/07/2009      bin.ji              Creat
******************************************************************************/

#define _MMI3D_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_mmi3d_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef UI_P3D_SUPPORT

#include "guilcd.h"
#include "ui_special_effect.h"
#include "mmi3d.h"
#include "mmi3d_export.h"

#include "mmiset_display.h"
#include "mmidisplay_data.h"
#include "block_mem.h"
#include "mmi_default.h"
#include "mmi_imagefun.h"
//#include "guires.h"
#include "ui_layer.h"
#include "mmitheme_special.h"
//#include "chng_freq.h"
#include "mmi_id.h"
#include "mmitheme_menu.h"
#include "mmitheme_pos.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define ICONMENUANIM_DELAY_TIME         300//100//500     //一次特效显示的时间

//icon_size的倍数
#define PREV_ICON_BUF_SIZE              1   
#define NEXT_ICON_BUF_SIZE              1  
#define OUT_ICON_BUF_SIZE               3
#define TEMP_ICON_BUF_SIZE              4
#define PREV_BG_ICON_BUF_SIZE           1
#define NEXT_BG_ICON_BUF_SIZE           1
#define RESTORE_BG_BUF_SIZE             2
    
#define PREV_ICON_BUF_OFFSET            (0)
#define NEXT_ICON_BUF_OFFSET            (PREV_ICON_BUF_OFFSET+PREV_ICON_BUF_SIZE)
#define OUT_ICON_BUF_OFFSET             (NEXT_ICON_BUF_OFFSET+NEXT_ICON_BUF_SIZE)
#define TEMP_ICON_BUF_OFFSET            (OUT_ICON_BUF_OFFSET+OUT_ICON_BUF_SIZE)
#define PREV_BG_ICON_BUF_OFFSET         (TEMP_ICON_BUF_OFFSET+TEMP_ICON_BUF_SIZE)
#define NEXT_BG_ICON_BUF_OFFSET         (PREV_BG_ICON_BUF_OFFSET+PREV_BG_ICON_BUF_SIZE)
#define RESTORE_BG_BUF_OFFSET           (NEXT_BG_ICON_BUF_OFFSET+NEXT_BG_ICON_BUF_SIZE)
#define ICON_BUF_TOTAL_SIZE             (RESTORE_BG_BUF_OFFSET+RESTORE_BG_BUF_SIZE)

//list menu 
#define PREV_LIST_BUF_SIZE              1   
#define NEXT_LIST_BUF_SIZE              1 
#define OUT_LIST_BUF_SIZE               3

#define PREV_LIST_BUF_OFFSET            (0)
#define NEXT_LIST_BUF_OFFSET            (PREV_LIST_BUF_OFFSET+PREV_LIST_BUF_SIZE)
#define OUT_LIST_BUF_OFFSET             (NEXT_LIST_BUF_OFFSET+NEXT_LIST_BUF_SIZE)
#define RESTORE_BG_LIST_BUF_OFFSET      (OUT_LIST_BUF_OFFSET+OUT_LIST_BUF_SIZE)
#define LIST_BUF_TOTAL_SIZE             (RESTORE_BG_LIST_BUF_OFFSET+RESTORE_BG_BUF_SIZE)

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//特效算法函数
typedef int32 (*SE_FUNC_T)(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

typedef struct
{
    SE_FUNC_T se_func;              //特效算法函数
    SE_EFFECT_ID_E se_effect_type;  //特效算法类型
}SE_PARAM_T;    //特效算法结构

LOCAL SE_PARAM_T s_se_param_arr[] =
{
    {PNULL, SE_EFFECT_BEGIN},   //none
    {SE_MagicMove,      SE_MAGIC_MOVE}, //移动魔术
    {SE_RotateMove3D,   SE_ROTATE_MOVE_3D}, //3D魔方
    {SE_SlideMove,      SE_SLIDE_MOVE}, //滑滑布丁
    {SE_RotateMove,     SE_ROTATE_MOVE}, //旋转乐趣
    {SE_LuckyBoard,     SE_LUCKY_BOARD}, //幸运翻板
    {SE_WrappedMove,    SE_WRAPPED_MOVE}, //卷动彩带
    {SE_DistortMove,    SE_DISTORT_MOVE}, //流转星云
};  //特效算法结构树组

LOCAL SE_INPUT_PARAM_T s_input_param = {0};         //保存3D动画输入参数
LOCAL MMIMENU_3DMENUBUF_T s_menu_buf_info = {0};    //menu_buf指针
    
LOCAL uint32 s_cur_ticks = 0;   //记录当前时刻

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
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

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 启动动画
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void StartIconMenuAnim(
                             SE_PARAM_T *se_param_ptr,
                             SE_INPUT_PARAM_T *input_param_ptr
                             );

/*****************************************************************************/
//  Description : 保存背景
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SavePartBG(
                      GUI_RECT_T *old_rcOutput_ptr,
                      GUI_COLOR_T *restore_bg_ptr
                      );
#endif

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 判断icon菜单动画是否开启
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_IsIconMenuAnimOpen(void)
{
    MMISET_3D_STYLE_MAINMENU_E style_3d = 0;
    BOOLEAN result = FALSE;

    style_3d = (MMISET_3D_STYLE_MAINMENU_E)MMISET_Get3DStyleMainmenu();

    if (style_3d >= MMISET_3D_STYLE_MAINMENU_MAX)
    {
        //SCI_TRACE_LOW:"style_3d < MMISET_3D_STYLE_MAINMENU_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI3D_150_112_2_18_2_37_19_0,(uint8*)"");
        return result;
    }
    if (MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_ICON_MENU) && (MMISET_3D_STYLE_MAINMENU_NONE != style_3d))
    {
        result = TRUE;
    }    
    return result;
}

/*****************************************************************************/
//  Description : 设置icon菜单动画的方向
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SetIconMenuDirection(
                               SE_DIRECTION_E dir
                               )
{
    s_input_param.direction = dir;
}

/*****************************************************************************/
//  Description : 将lcd_buf中指定区域内容保存到menuicon_buf中
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SaveMenuIcon(
                               MMI3D_ICONMENU_BUF_E buf_id,
                               SE_RECT_T *rect_ptr
                               )
{
    uint16 *buf_ptr = PNULL;
    GUI_RECT_T rect = {0};

    if (PNULL == rect_ptr)/*lint !e774*/
    {
        //SCI_TRACE_LOW:"MMIAPI3D_SaveMenuIcon PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI3D_185_112_2_18_2_37_19_1,(uint8*)"");
        return;
    }

    if (!s_input_param.is_mem_ok)
    {
        return;
    }
    
    //执行3d特效之前，将lcd buf内容准备好
    rect.left = MAX(s_input_param.menu_rect.left, rect_ptr->left);
    rect.top = MAX(s_input_param.menu_rect.top, rect_ptr->top);
    rect.right = MIN(s_input_param.menu_rect.right, rect_ptr->right);
    rect.bottom = MIN(s_input_param.menu_rect.bottom, rect_ptr->bottom);    
    if (PREV_BUF == buf_id)
    {
        s_input_param.selected_rect.left = rect.left;
        s_input_param.selected_rect.top = rect.top;
        s_input_param.selected_rect.right = rect.right;
        s_input_param.selected_rect.bottom = rect.bottom;
        buf_ptr = s_input_param.selected_ptr;
    }
    else if (NEXT_BUF == buf_id)
    {
        s_input_param.next_rect.left = rect.left;
        s_input_param.next_rect.top = rect.top;
        s_input_param.next_rect.right = rect.right;
        s_input_param.next_rect.bottom = rect.bottom;
        buf_ptr = s_input_param.next_ptr;
    }
    else if (PREV_BG_BUF == buf_id)
    {
        buf_ptr = s_input_param.selected_menu_bk_ptr;
    }
    else if (NEXT_BG_BUF == buf_id)
    {
        buf_ptr = s_input_param.next_menu_bk_ptr;
    }

    //copy lcd tu buf
    if (PNULL != buf_ptr
        && !GUI_IsInvalidRect(rect))
    {
        UILAYER_InvalidateRect(0, &rect);
        LCD_CopyMainLcdBufToMem(
            buf_ptr,
            rect_ptr->right + 1 - rect_ptr->left, 
            rect_ptr->bottom + 1 - rect_ptr->top,
            rect_ptr->left,
            rect_ptr->top,
            TRUE
            );
    }
}

/*****************************************************************************/
//  Description : 启动icon菜单动画
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_StartIconMenuAnim(void)
{
    uint16 style_3d = MMISET_Get3DStyleMainmenu();

    if (!s_input_param.is_mem_ok)
    {
        return FALSE;
    }
    
    //anim   
    StartIconMenuAnim(&s_se_param_arr[style_3d], &s_input_param);
    
    MMIAPI3D_Free3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_ICON_MENU);

    return TRUE;
}

/*****************************************************************************/
//  Description : 恢复背景
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void RestorePartBG(
                         GUI_RECT_T *old_rcOutput_ptr,
                         GUI_COLOR_T *restore_bg_ptr
                         )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0}; 
        
    LCD_CopyMemToMainLcdBuf(
        restore_bg_ptr,
        old_rcOutput_ptr->right + 1 - old_rcOutput_ptr->left, 
        old_rcOutput_ptr->bottom + 1 - old_rcOutput_ptr->top,
        old_rcOutput_ptr->left,
        old_rcOutput_ptr->top,
        MMI3D_KEY_COLOR,
        TRUE
        );
    MMITHEME_StoreUpdateRect(&lcd_dev_info, *old_rcOutput_ptr);
}

/*****************************************************************************/
//  Description : 启动动画
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void StartIconMenuAnim(
                             SE_PARAM_T *se_param_ptr,
                             SE_INPUT_PARAM_T *input_param_ptr
                             )
{
    uint32 iIndex = 0;
    GUI_RECT_T rcOutput = {0};
    GUI_RECT_T old_rcOutput = MMITHEME_GetFullScreenRect();
    GUI_RECT_T total_rcOutput = {0};
    SE_OUTPUT_PARAM_T output_param = {0};
    uint32 frame_num = 0;

    if ( MMK_IsQuickKey())
    {
        return;
    }
    
    if(PNULL == se_param_ptr ||PNULL == input_param_ptr||PNULL == se_param_ptr->se_func)
    {
        return;
    }

    
    output_param.output_buf_ptr = s_menu_buf_info.out_icon_ptr;
    if (PNULL == output_param.output_buf_ptr)
    {
        return;
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    frame_num = SE_GetEffectFrameAmount(se_param_ptr->se_effect_type);
    //执行3d特效之前，将lcd buf内容准备好
    MMITHEME_UpdateMultiLayer();
    for (iIndex = 0; iIndex < frame_num; iIndex++)
    {        
        MMI3D_ForceDelayStart();
        input_param_ptr->frame_index = iIndex;
        input_param_ptr->key_color = MMI3D_KEY_COLOR;

        se_param_ptr->se_func(input_param_ptr, &output_param);
               
        rcOutput.left = MAX(0, output_param.output_rect.left);	//position and size of output icon
        rcOutput.top = MAX(0, output_param.output_rect.top);
        rcOutput.right = MMITHEME_GetMainScreenRightPixel();
        rcOutput.right = MIN(rcOutput.right, output_param.output_rect.right);
        rcOutput.bottom = MMITHEME_GetMainScreenBottomPixel();
        rcOutput.bottom = MIN(rcOutput.bottom, output_param.output_rect.bottom);
        
        total_rcOutput.left = MIN(old_rcOutput.left, rcOutput.left);
        total_rcOutput.top = MIN(old_rcOutput.top, rcOutput.top);
        total_rcOutput.right = MAX(old_rcOutput.right, rcOutput.right);
        total_rcOutput.bottom = MAX(old_rcOutput.bottom, rcOutput.bottom);

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        if (iIndex)
        {
            RestorePartBG(&old_rcOutput, s_menu_buf_info.restore_bg_ptr);
        }
        SavePartBG(&rcOutput, s_menu_buf_info.restore_bg_ptr);
#endif
        LCD_CopyMemToMainLcdBuf(
            s_menu_buf_info.out_icon_ptr,
            (rcOutput.right+1-rcOutput.left),
            (rcOutput.bottom+1-rcOutput.top),
            rcOutput.left,
            rcOutput.top,
            MMI3D_KEY_COLOR,
            TRUE
            );
        old_rcOutput = rcOutput;      
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,total_rcOutput,0);
        
        MMI3D_ForceDelayEnd(ICONMENUANIM_DELAY_TIME/frame_num);
    }
    //restore icon
    if(frame_num > 0)
    {
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        RestorePartBG(&old_rcOutput, s_menu_buf_info.restore_bg_ptr);
#endif
    }

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif      
}

/*****************************************************************************/
//  Description : 保存背景
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SavePartBG(
                      GUI_RECT_T *old_rcOutput_ptr,
                      GUI_COLOR_T *restore_bg_ptr
                      )
{
    LCD_CopyMainLcdBufToMem(
        restore_bg_ptr,
        old_rcOutput_ptr->right + 1 - old_rcOutput_ptr->left, 
        old_rcOutput_ptr->bottom + 1 - old_rcOutput_ptr->top,
        old_rcOutput_ptr->left,
        old_rcOutput_ptr->top,
        TRUE
        );
}
#endif

/*****************************************************************************/
//  Description : 分配3d menu的内存，进入主菜单或二级菜单时调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Alloc3DMenuBuf(
                                 BLOCK_MEM_ID_E block_id,
                                 uint32 icon_size,   //icon大小，字节数
                                 MMI_HANDLE_T    win_handle
                                 )
{
    uint8       *buf_start_ptr = PNULL;
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_handle);

    if ((BLOCK_MEM_POOL_MMI_3D_ICON_MENU != block_id) 
     && (BLOCK_MEM_POOL_MMI_3D_SECOND_LIST != block_id))
    {
         return;
    }
    buf_start_ptr = (uint8 *)MMI_BL_MALLOC((BLOCK_MEM_ID_E)block_id);

    if (PNULL != buf_start_ptr)/*lint !e774*/
    {
        SCI_MEMSET(buf_start_ptr, 0x00, BLOCK_MEM_POOL_MMI_MENU3D_SIZE);
        SCI_MEMSET(&s_menu_buf_info, 0x00, sizeof(s_menu_buf_info));
        SCI_MEMSET(&s_input_param, 0x00, sizeof(s_input_param));
        
        if (BLOCK_MEM_POOL_MMI_3D_ICON_MENU == block_id)
        {
            if (icon_size * ICON_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_MENU3D_SIZE)
            {
                //SCI_TRACE_LOW:"icon_size * ICON_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_MENU3D_SIZE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI3D_420_112_2_18_2_37_19_2,(uint8*)"");
                s_input_param.is_mem_ok = FALSE;
                MMIAPI3D_Free3DMenuBuf(block_id);
                return;
            }
            
            s_menu_buf_info.prev_icon_ptr    = (GUI_COLOR_T*)(buf_start_ptr+icon_size*PREV_ICON_BUF_OFFSET);
            s_menu_buf_info.next_icon_ptr    = (GUI_COLOR_T*)(buf_start_ptr+icon_size*NEXT_ICON_BUF_OFFSET);
            s_menu_buf_info.out_icon_ptr     = (GUI_COLOR_T*)(buf_start_ptr+icon_size*OUT_ICON_BUF_OFFSET);
            s_menu_buf_info.temp_icon_ptr    = (GUI_COLOR_T*)(buf_start_ptr+icon_size*TEMP_ICON_BUF_OFFSET);
            s_menu_buf_info.prev_bg_icon_ptr = (GUI_COLOR_T*)(buf_start_ptr+icon_size*PREV_BG_ICON_BUF_OFFSET);
            s_menu_buf_info.next_bg_icon_ptr = (GUI_COLOR_T*)(buf_start_ptr+icon_size*NEXT_BG_ICON_BUF_OFFSET);
            s_menu_buf_info.restore_bg_ptr   = (GUI_COLOR_T*)(buf_start_ptr+icon_size*RESTORE_BG_BUF_OFFSET);
    
            s_input_param.selected_ptr         = s_menu_buf_info.prev_icon_ptr;
            s_input_param.next_ptr             = s_menu_buf_info.next_icon_ptr;
            s_input_param.selected_menu_bk_ptr = s_menu_buf_info.prev_bg_icon_ptr;
            s_input_param.next_menu_bk_ptr     = s_menu_buf_info.next_bg_icon_ptr;
            s_input_param.temp_buf_ptr         = s_menu_buf_info.temp_icon_ptr;
        }

        if (BLOCK_MEM_POOL_MMI_3D_SECOND_LIST == block_id)
        {
            if (icon_size * LIST_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_MENU3D_SIZE)
            {
                //SCI_TRACE_LOW:"icon_size * LIST_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_MENU3D_SIZE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI3D_445_112_2_18_2_37_19_3,(uint8*)"");
                s_input_param.is_mem_ok = FALSE;
                MMIAPI3D_Free3DMenuBuf(block_id);                
                return;
            }
            
            s_menu_buf_info.prev_icon_ptr    = (GUI_COLOR_T*)(buf_start_ptr+icon_size*PREV_LIST_BUF_OFFSET);
            s_menu_buf_info.next_icon_ptr    = (GUI_COLOR_T*)(buf_start_ptr+icon_size*NEXT_LIST_BUF_OFFSET);
            s_menu_buf_info.out_icon_ptr     = (GUI_COLOR_T*)(buf_start_ptr+icon_size*OUT_LIST_BUF_OFFSET);
            s_menu_buf_info.restore_bg_ptr   = (GUI_COLOR_T*)(buf_start_ptr+icon_size*RESTORE_BG_LIST_BUF_OFFSET);
            s_menu_buf_info.temp_icon_ptr    = PNULL;
            s_menu_buf_info.prev_bg_icon_ptr = PNULL;
            s_menu_buf_info.next_bg_icon_ptr = PNULL;

    
            s_input_param.selected_ptr         = s_menu_buf_info.prev_icon_ptr;
            s_input_param.next_ptr             = s_menu_buf_info.next_icon_ptr;
            s_input_param.selected_menu_bk_ptr = PNULL;
            s_input_param.next_menu_bk_ptr     = PNULL;
            s_input_param.temp_buf_ptr         = PNULL;  
        }

        s_input_param.is_mem_ok        = TRUE;        
        s_input_param.menu_rect.top    = 0;
        s_input_param.menu_rect.left   = 0;
        s_input_param.menu_rect.right  = client_rect.right;
        s_input_param.menu_rect.bottom = client_rect.bottom;
    
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
    	if(MMK_ConvertIdToHandle(MAIN_MAINMENU_WIN_ID) == win_handle)
    		{
    			if (MMITHEME_IsMainScreenLandscape())
    			{
    				GUI_RECT_T title_text_display_rect = MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT;
    				client_rect.top = title_text_display_rect.bottom;
    				client_rect.bottom -= MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H;
    			}
    			else
    			{
    				GUI_RECT_T title_text_display_rect = MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT;
    				client_rect.top = title_text_display_rect.bottom;
    				client_rect.bottom -= MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V;
    			}
    			s_input_param.menu_rect.top    = client_rect.top;
        		s_input_param.menu_rect.left   = 0;
        		s_input_param.menu_rect.right  = client_rect.right;
        		s_input_param.menu_rect.bottom = client_rect.bottom;
    		}
#endif
    }
    else
    {
        s_input_param.is_mem_ok = FALSE;
    }
}

/*****************************************************************************/
//  Description : 释放3d menu的内存，退出主菜单或二级菜单时调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Free3DMenuBuf(BLOCK_MEM_ID_E block_id)
{
    MMI_BL_FREE((BLOCK_MEM_ID_E)block_id);
    SCI_MEMSET(&s_menu_buf_info, 0x00, sizeof(s_menu_buf_info));
    SCI_MEMSET(&s_input_param, 0x00, sizeof(s_input_param));
}

/*****************************************************************************/
//  Description : 得到3d menu的内存指针，给二三级菜单调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_Get3DMenuBufInfo(
                                   MMIMENU_3DMENUBUF_T *menubuf_info_ptr    //out
                                   )
{
    if (PNULL != menubuf_info_ptr)/*lint !e774*/
    {
        *menubuf_info_ptr = s_menu_buf_info;
    }
}

#if 0
/*****************************************************************************/
//  Description : 将icon_id图片解码到icon_buf_ptr指向的buf中
//  Global resource dependence : 
//  Author: bin.ji
//  Note: part_rect_ptr=PNULL时，将整张icon解码
/*****************************************************************************/
PUBLIC void MMIAPI3D_DecodeIconBuf(
                                MMI_IMAGE_ID_T  icon_id,
                                MMI_WIN_ID_T    win_id,
                                GUI_COLOR_T     *icon_buff_ptr,   //out
                                uint32          icon_buf_width,
                                uint32          icon_buf_height,
                                int16           start_x,
                                int16           start_y,
                                GUI_RECT_T      *part_rect_ptr
                                )
{
    const uint8*    icon_ptr = PNULL;
    uint32          icon_size = 0;
    GUI_POINT_T     startpoint = {0, 0};

    icon_ptr = MMI_GetLabelImage(icon_id, win_id, &icon_size);
    
    startpoint.x = start_x;
    startpoint.y = start_y;
    
    if (PNULL != part_rect_ptr)
    {
        //copy image buffer
        GUIRES_CopyImgBuf(icon_ptr,
            icon_buf_width,
            icon_buf_height,
            icon_buf_width*icon_buf_height,
            &icon_size,
            PNULL,
            part_rect_ptr,
            (GUI_COLOR_T *)icon_buff_ptr);
    }
    else
    {
        //copy image buffer
        GUIRES_CopyImgBuf(icon_ptr,
            icon_buf_width,
            icon_buf_height,
            icon_buf_width*icon_buf_height,
            &icon_size,
            &startpoint,
            PNULL,
            (GUI_COLOR_T *)icon_buff_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : start item anim,for menu and list,two item move
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_StartItemAnim(BLOCK_MEM_ID_E block_id)
{
    uint32              i = 0;
    uint32              frame_num = 0;
    GUI_RECT_T          rcOutput = {0};
    GUI_RECT_T          old_rcOutput = MMITHEME_GetFullScreenRect();
    GUI_RECT_T          total_rcOutput = {0};
    SE_INPUT_PARAM_T    *input_param_ptr = &s_input_param;
    SE_OUTPUT_PARAM_T   output_param = {0};

    if (!s_input_param.is_mem_ok)
    {
        return;
    }

    if ( !MMK_IsQuickKey())
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif
        
        //get frame number
        frame_num = SE_GetEffectFrameAmount(SE_MENU_ITEM_MOVE);
        
        output_param.output_buf_ptr = s_menu_buf_info.out_icon_ptr;
        
        //执行3d特效之前，将lcd buf内容准备好
        MMITHEME_UpdateMultiLayer();
        for (i=0; i<frame_num; i++)
        {
            input_param_ptr->frame_index = i;
            input_param_ptr->key_color   = MMI3D_KEY_COLOR;
            
            SE_MenuItemMove(input_param_ptr,&output_param);
            
            rcOutput.left   = output_param.output_rect.left;
            rcOutput.top    = output_param.output_rect.top;
            rcOutput.right  = output_param.output_rect.right;
            rcOutput.bottom = output_param.output_rect.bottom;
            
            total_rcOutput.left   = MIN(old_rcOutput.left,rcOutput.left);
            total_rcOutput.top    = MIN(old_rcOutput.top,rcOutput.top);
            total_rcOutput.right  = MAX(old_rcOutput.right,rcOutput.right);
            total_rcOutput.bottom = MAX(old_rcOutput.bottom,rcOutput.bottom);

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
            if (i)
            {
                RestorePartBG(&old_rcOutput,s_menu_buf_info.restore_bg_ptr);
            }

            SavePartBG(&rcOutput,s_menu_buf_info.restore_bg_ptr);
#endif

            LCD_CopyMemToMainLcdBuf(s_menu_buf_info.out_icon_ptr,
                (rcOutput.right+1-rcOutput.left),
                (rcOutput.bottom+1-rcOutput.top),
                rcOutput.left,
                rcOutput.top,
                MMI3D_KEY_COLOR,
                TRUE
                );
            
            old_rcOutput = rcOutput;
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,total_rcOutput,0);
            SCI_Sleep(MMI_WIN_ANIM_3DLIST_SLEEP_TIME);
        }
        
        //restore icon
        if (0 < frame_num)
        {
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
            RestorePartBG(&old_rcOutput,s_menu_buf_info.restore_bg_ptr);
#endif   
        }
        
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif    
    }
        
    //free menu buffer
    MMIAPI3D_Free3DMenuBuf((BLOCK_MEM_ID_E)block_id);
}

/*****************************************************************************/
//  Description : 强制延迟一段时间（记录开始时间）
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_ForceDelayStart(void)
{
    s_cur_ticks = SCI_GetTickCount();
}

/*****************************************************************************/
//  Description : 强制延迟一段时间（记录结束时间，如果不够delay_ticks补足）
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_ForceDelayEnd(
                                uint32 delay_ticks
                                )
{
    uint32 cur_ticks = SCI_GetTickCount();
    uint32 ticks = (cur_ticks - s_cur_ticks);

    if (ticks < delay_ticks)
    {
        //增加延迟至100ms
        SCI_Sleep(delay_ticks - ticks);
    }
}

#endif

