/*****************************************************************************
** File Name:      guianim_display.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "ctrlanim.h"
#include "guicommon.h"
#include "guires.h"
#include "mmi_theme.h"


#define ARGB_SET_A(_argb, _alpha)   ((_argb) | (((_alpha) << 24) & 0xff000000))
#define ARGB_SET_R(_argb, _r)       ((_argb) | (((_r) << 16) & 0xff0000))
#define ARGB_SET_G(_argb, _g)       ((_argb) | (((_g) << 8) & 0xff00))
#define ARGB_SET_B(_argb, _b)       ((_argb) | (((_b)) & 0xff))
#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

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
//  Description : fill control bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FillAnimBg(
                      CTRLANIM_OBJ_T    *anim_ctrl_ptr
                      );

/*****************************************************************************/
//  Description : get anim control display rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAnimDisplayRect(
                                 CTRLANIM_OBJ_T *anim_ctrl_ptr,     //in:
                                 GUI_RECT_T     *display_rect_ptr   //in/out:
                                 );

/*****************************************************************************/
//  Description : get image display crossed rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetImgDisplayRect(
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr, //in:
                                GUI_RECT_T      *lcd_rect_ptr   //in/out:
                                );

/*****************************************************************************/
//  Description : display gif image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayGifImg(
                            BOOLEAN         is_only_update, //in:
                            GUI_RECT_T      *lcd_rect_ptr,  //in:
                            GUI_RECT_T      *image_rect_ptr,//in:
                            CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in:
                            );

/*****************************************************************************/
//  Description : display gif resource image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayGifRes(
                         GUI_RECT_T         *lcd_rect_ptr,          //in:
                         GUI_RECT_T         *image_rect_ptr,        //in:
                         CTRLANIM_OBJ_T     *anim_ctrl_ptr          //in:
                         );

/*****************************************************************************/
//  Description : copy or display bg buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CopyOrDisplayBg(
                           CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in
                           );

/*****************************************************************************/
//  Description : display bmp/wbmp/jpg/png image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayImgExceptGif(
                                  GUI_RECT_T        *lcd_rect_ptr,  //in:
                                  GUI_RECT_T        *image_rect_ptr,//in:
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                                  );

/*****************************************************************************/
//  Description : display image for wallpaper
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayWallpaperExceptGif(
										GUI_RECT_T		*lcd_rect_ptr,  //in:
										CTRLANIM_OBJ_T	*anim_ctrl_ptr  //in:
										);

/*****************************************************************************/
//  Description : display anim
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayAnim(
                          BOOLEAN           is_only_update, //in:
                          GUI_RECT_T        *lcd_rect_ptr,  //in:
                          GUI_RECT_T        *image_rect_ptr,//in:
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                          );

/*****************************************************************************/
//  Description : call back function
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void CallBackFunc(
                        CTRLANIM_OBJ_T  *anim_ctrl_ptr
                        );

/*****************************************************************************/
//  Description : display default wallpaper,only for gif file or data buffer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDefaultWallpaper(
                                   GUI_RECT_T       *lcd_rect_ptr,  //in:
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : display default picture icon
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDefaultPicIcon(
                                 CTRLANIM_OBJ_T     *anim_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : display img res bit map to target buffer
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GUIANIM_DisplayImgResWithBuffer(
                                                GUI_RECT_T      *lcd_rect_ptr,  //in:
                                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in:
                                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : display anim or image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_Display(
                               BOOLEAN          is_full_paint,  //是否是full paint消息
                               BOOLEAN          is_only_update, //in:
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_focus = FALSE;
    GUI_RECT_T      lcd_rect = {0};
    GUI_RECT_T      image_rect = {0};
    MMI_HANDLE_T    focus_win_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //if (IGUICTRL_GetState((IGUICTRL_T *)anim_ctrl_ptr,GUICTRL_STATE_INVISIBLE))
    if (!VTLBASE_GetVisible((CTRLBASE_OBJ_T*)anim_ctrl_ptr))
    {
        return (result);
    }

    //full paint消息，不用判断窗口是否处于focus
    if (is_full_paint)
    {
        is_focus = TRUE;
    }
    else
    {
        is_focus = MMK_IsWinNeedPaint(anim_ctrl_ptr->win_handle);
        if (!is_focus)
        {
            //get focus win
            focus_win_handle = MMK_GetFocusWinHandle();

            //focus win previous win is anim and it is transparent win
            if ((anim_ctrl_ptr->win_handle == MMK_GetPrevWinHandle(focus_win_handle)) &&
                (MMK_GetWinDisplayStyleState(focus_win_handle,WS_POPMENU_WIN)||MMK_GetWinDisplayStyleState(focus_win_handle,WS_HAS_TRANSPARENT)))
            {
                is_focus = TRUE;
            }
            else
            {
                focus_win_handle = 0;
            }
        }
    }

    //display border
    if (is_focus)
    {
        GUI_DisplayBorder(base_ctrl_ptr->rect,
            base_ctrl_ptr->display_rect,
            &anim_ctrl_ptr->theme.border,
            &base_ctrl_ptr->lcd_dev_info);

        //fill control bg color or bg image
        FillAnimBg(anim_ctrl_ptr);

        //is decode
        if (anim_ctrl_ptr->is_decoded)
        {
            //get anim display rect
            if (GetAnimDisplayRect(anim_ctrl_ptr,&lcd_rect))
            {
                //get image display rect
                if (GetImgDisplayRect(anim_ctrl_ptr,&lcd_rect))
                {
                    //image rect
                    image_rect.left   = (int16)(lcd_rect.left - anim_ctrl_ptr->display_x);
                    image_rect.top    = (int16)(lcd_rect.top - anim_ctrl_ptr->display_y);
                    image_rect.right  = (int16)(lcd_rect.right - anim_ctrl_ptr->display_x);
                    image_rect.bottom = (int16)(lcd_rect.bottom - anim_ctrl_ptr->display_y);

                    //display pic
                    switch (anim_ctrl_ptr->type)
                    {
                    case GUIANIM_TYPE_NONE:
                    case GUIANIM_TYPE_UNKNOWN:
                        //do nothing
                        break;

                    case GUIANIM_TYPE_IMAGE_GIF:
                        result = DisplayGifImg(is_only_update,&lcd_rect,&image_rect,anim_ctrl_ptr);
                        break;
                
                    case GUIANIM_TYPE_IMAGE_BMP:
                    case GUIANIM_TYPE_IMAGE_WBMP:
                    case GUIANIM_TYPE_IMAGE_JPG:
                    case GUIANIM_TYPE_IMAGE_PNG:
                    case GUIANIM_TYPE_IMAGE_ICO:
                    case GUIANIM_TYPE_IMAGE_WEBP:
                        result = DisplayImgExceptGif(&lcd_rect,&image_rect,anim_ctrl_ptr);
                        break;

                    case GUIANIM_TYPE_ANIM:
                    case GUIANIM_TYPE_ANIM_ARGB:
                        //display anim
                        result = DisplayAnim(is_only_update,&lcd_rect,&image_rect,anim_ctrl_ptr);
                        break;

                    default:
                        SCI_PASSERT(FALSE,("GUIANIM_Display:image type %d is error!",anim_ctrl_ptr->type)); /*assert verified*/
                        break;
                    }
                }
                
                if (!is_only_update)
                {
                    //call back
                    CallBackFunc(anim_ctrl_ptr);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"GUIANIM_Display:anim image rect and display rect are not crossed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_DISPLAY_266_112_2_18_3_14_58_21,(uint8*)"");
                result = FALSE;
            }
        }
        else
        {
            //display default picture icon
            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                //get anim display rect
                if (GetAnimDisplayRect(anim_ctrl_ptr,&lcd_rect))
                {
                    DisplayDefaultWallpaper(&lcd_rect,anim_ctrl_ptr);
                }
            }
            else
            {
                //display default picture icon
                DisplayDefaultPicIcon(anim_ctrl_ptr);
            }

            if (!is_only_update)
            {
                //call back
                CallBackFunc(anim_ctrl_ptr);
            }

            //SCI_TRACE_LOW:"GUIANIM_Display:image is_decoded = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_DISPLAY_293_112_2_18_3_14_58_22,(uint8*)"d",anim_ctrl_ptr->is_decoded);
            result = FALSE;
        }

        //update focus win
        MMK_SendMsg(focus_win_handle,MSG_FULL_PAINT,PNULL);
    }
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"GUIANIM_Display:is not focus!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_DISPLAY_303_112_2_18_3_14_58_23,(uint8*)"");

        //wallpaper not display, exit gif decode task
        if ((anim_ctrl_ptr->is_wallpaper_ctrl) && 
            (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type) &&
            (0 < anim_ctrl_ptr->img_handle))
        {
            //destroy image handle
            GUIANIM_DestroyHandle(FALSE,
                anim_ctrl_ptr->img_handle,
                base_ctrl_ptr->handle);
            anim_ctrl_ptr->img_handle = 0;

            //free backup target buffer
            if (PNULL != anim_ctrl_ptr->target_backup_buf)
            {
                SCI_FREE(anim_ctrl_ptr->target_backup_buf);
                anim_ctrl_ptr->target_backup_offset = 0;
            }
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : fill control bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FillAnimBg(
                      CTRLANIM_OBJ_T    *anim_ctrl_ptr
                      )
{
    BOOLEAN             is_fill = TRUE;
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          image_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //anim resource,not first frame,not argb don't display bg
    if ((0 != anim_ctrl_ptr->data_info.anim_id) &&
        (0 != anim_ctrl_ptr->cur_frame) &&
        (GUIANIM_TYPE_ANIM == anim_ctrl_ptr->type))
    {
        is_fill = FALSE;
    }

    //get anim display rect
    if ((is_fill) && 
        (GetAnimDisplayRect(anim_ctrl_ptr,&cross_rect)))
    {
        //set image rect
        image_rect.left   = (int16)(cross_rect.left - base_ctrl_ptr->rect.left);
        image_rect.top    = (int16)(cross_rect.top - base_ctrl_ptr->rect.top);
        image_rect.right  = (int16)(image_rect.left + cross_rect.right - cross_rect.left);
        image_rect.bottom = (int16)(image_rect.top + cross_rect.bottom - cross_rect.top);

        //set bg display info
        bg_display.is_form_bg     = anim_ctrl_ptr->is_form_bg;
        bg_display.rect           = base_ctrl_ptr->rect;
        bg_display.display_rect   = cross_rect;
        bg_display.img_rect       = image_rect;
        bg_display.ctrl_handle    = base_ctrl_ptr->handle;
        bg_display.win_handle     = anim_ctrl_ptr->win_handle;
        
        //display bg
        GUI_DisplayBgExt(&anim_ctrl_ptr->display_info.bg,
            &bg_display,
            anim_ctrl_ptr->is_notify_update,
            &base_ctrl_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : get anim control display rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAnimDisplayRect(
                                 CTRLANIM_OBJ_T *anim_ctrl_ptr,     //in:
                                 GUI_RECT_T     *display_rect_ptr   //in/out:
                                 )
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      anim_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //get anim rect,except border
    if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
    {
        //get crossed rect
        result = GUI_IntersectRect(display_rect_ptr,anim_rect,base_ctrl_ptr->display_rect);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image display crossed rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetImgDisplayRect(
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr, //in:
                                GUI_RECT_T      *lcd_rect_ptr   //in/out:
                                )
{
    BOOLEAN     result = FALSE;
    uint16      img_width = 0;
    uint16      img_height = 0;
    GUI_RECT_T  anim_rect = {0};
    GUI_RECT_T  image_display_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //get anim rect,except border
    GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

    //get image width and height
    GUIANIM_GetImgWidthAndHeight(FALSE,FALSE,&img_width,&img_height,anim_ctrl_ptr);

    //set image display rect
    image_display_rect.left   = (int16)MAX(anim_ctrl_ptr->display_x,anim_rect.left);
    image_display_rect.top    = (int16)MAX(anim_ctrl_ptr->display_y,anim_rect.top);
    image_display_rect.right  = (int16)MIN((image_display_rect.left + img_width - 1),anim_rect.right);
    image_display_rect.bottom = (int16)MIN((image_display_rect.top + img_height - 1),anim_rect.bottom);

    //get crossed rect is lcd rect
    result = GUI_IntersectRect(lcd_rect_ptr,base_ctrl_ptr->display_rect,image_display_rect);

    return (result);
}

/*****************************************************************************/
//  Description : display gif image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayGifImg(
                            BOOLEAN         is_only_update, //in:
                            GUI_RECT_T      *lcd_rect_ptr,  //in:
                            GUI_RECT_T      *image_rect_ptr,//in:
                            CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in:
                            )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_horizontal = FALSE;
    GUI_POINT_T         display_point = {0};
    GUIIMG_BITMAP_T     img_bitmap = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //kevin.lou modified:delete assert
    if ((0 == anim_ctrl_ptr->data_info.img_id)&&(PNULL != anim_ctrl_ptr->target_buffer))
    {
        //SCI_ASSERT(PNULL != anim_ctrl_ptr->target_buffer); /*assert verified*/

        //is horizontal display
        is_horizontal = GUIANIM_IsHorizontal(anim_ctrl_ptr);

        //set display point
        display_point.x = anim_ctrl_ptr->display_x;
        display_point.y = anim_ctrl_ptr->display_y;

        //set bitmap
        GUIANIM_GetDisplayBitmap(anim_ctrl_ptr,&img_bitmap);

        //display
        GUIIMG_DisplayBmp(is_horizontal,
            lcd_rect_ptr,
            &display_point,
            &img_bitmap,
            &base_ctrl_ptr->lcd_dev_info);
    }
    else
    {
        //display gif resource
        DisplayGifRes(lcd_rect_ptr,image_rect_ptr,anim_ctrl_ptr);
    }

    //start timer
    if ((!is_only_update) &&
        (!anim_ctrl_ptr->is_decoding) &&
        (anim_ctrl_ptr->is_focus) &&
        (!anim_ctrl_ptr->is_pause) &&
        (!anim_ctrl_ptr->display_info.is_disp_one_frame) &&
        (1 != anim_ctrl_ptr->total_frame))
    {
        if ((anim_ctrl_ptr->display_info.is_play_once) && 
            (0 < anim_ctrl_ptr->total_frame) &&
            (anim_ctrl_ptr->cur_frame == (anim_ctrl_ptr->total_frame-1)))
        {
            SCI_TRACE_LOW("GUIANIM:DisplayGifImg:only play gif once!");
        }
        else
        {
            GUIANIM_StartGifTimer(anim_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get display bitmap
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_GetDisplayBitmap(
                                     CTRLANIM_OBJ_T     *anim_ctrl_ptr, //in:
                                     GUIIMG_BITMAP_T    *bitmap_ptr     //in/out:
                                     )
{
    //set bitmap
    if ((anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->is_support_zoom))
    {
        bitmap_ptr->bit_ptr    = anim_ctrl_ptr->zoom_dest.buf_ptr + anim_ctrl_ptr->zoom_dest.buf_offset;
        bitmap_ptr->img_width  = anim_ctrl_ptr->zoom_dest.buf_width;
        bitmap_ptr->img_height = anim_ctrl_ptr->zoom_dest.buf_height;
        bitmap_ptr->date_type  = anim_ctrl_ptr->zoom_dest.data_type;
    }
    else if (anim_ctrl_ptr->is_rotate)
    {
        bitmap_ptr->bit_ptr    = anim_ctrl_ptr->rotate_dest.buf_ptr + anim_ctrl_ptr->rotate_dest.buf_offset;
        bitmap_ptr->img_width  = anim_ctrl_ptr->rotate_dest.img_width;
        bitmap_ptr->img_height = anim_ctrl_ptr->rotate_dest.img_height;
        bitmap_ptr->date_type  = anim_ctrl_ptr->rotate_dest.data_type;
    }
    else
    {
        bitmap_ptr->bit_ptr    = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
        bitmap_ptr->img_width  = anim_ctrl_ptr->img_width;
        bitmap_ptr->img_height = anim_ctrl_ptr->img_height;
        bitmap_ptr->date_type  = anim_ctrl_ptr->target_data_type;
    }
}

/*****************************************************************************/
//  Description : display gif resource image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayGifRes(
                         GUI_RECT_T         *lcd_rect_ptr,          //in:
                         GUI_RECT_T         *image_rect_ptr,        //in:
                         CTRLANIM_OBJ_T     *anim_ctrl_ptr          //in:
                         )
{
    BOOLEAN             is_free = FALSE;
    uint8               *src_data_ptr = PNULL;
    uint32              img_data_size = 0;
    GUIRES_ANIM_INFO_T  res_gif_info = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //free handle
    if (0 != anim_ctrl_ptr->res_gif_handle)
    {
        if ((anim_ctrl_ptr->is_next_frame) &&
            (anim_ctrl_ptr->is_dec_all_gif))
        {
            is_free = TRUE;
        }
        else
        {
            //set src data
            src_data_ptr = anim_ctrl_ptr->src_data_buffer;

            //reload
            anim_ctrl_ptr->src_data_buffer = (uint8 *)anim_ctrl_ptr->theme.GetLabelImg(anim_ctrl_ptr->data_info.img_id,anim_ctrl_ptr->win_handle,&img_data_size);
            anim_ctrl_ptr->src_data_size   = img_data_size;
            
            //data buffer change,need free gif handle
            if (src_data_ptr != anim_ctrl_ptr->src_data_buffer)
            {
                is_free = TRUE;
            }

        }

        if (is_free)
        {
            GUIRES_FreeHandle(anim_ctrl_ptr->res_gif_handle);
            anim_ctrl_ptr->res_gif_handle = 0;

            anim_ctrl_ptr->is_dec_all_gif = FALSE;
        }
    }

    //creat handle
    if (0 == anim_ctrl_ptr->res_gif_handle)
    {
        //copy or display bg
        CopyOrDisplayBg(anim_ctrl_ptr);

        //creat gif handle
        GUIRES_CreatHandle(&anim_ctrl_ptr->res_gif_handle,
            anim_ctrl_ptr->win_handle,
            anim_ctrl_ptr->data_info.img_id,
            &base_ctrl_ptr->lcd_dev_info);
        anim_ctrl_ptr->is_next_frame = TRUE;
    }
    
    //display gif anim
    IMG_EnableTransparentColor(TRUE);
    if (GUIRES_DisplayGif(anim_ctrl_ptr->is_next_frame,
        anim_ctrl_ptr->res_gif_handle,
        PNULL,
        lcd_rect_ptr,
        image_rect_ptr,
        anim_ctrl_ptr->win_handle,
        anim_ctrl_ptr->data_info.img_id,
        &res_gif_info,
        &base_ctrl_ptr->lcd_dev_info))
    {
        //set current frame and delay timer etc.
        anim_ctrl_ptr->is_next_frame = FALSE;
        anim_ctrl_ptr->cur_frame     = res_gif_info.cur_frame;
        anim_ctrl_ptr->delay_time    = res_gif_info.delay_time;
        if (res_gif_info.is_decode_all)
        {
            anim_ctrl_ptr->total_frame = (uint16)(res_gif_info.cur_frame + 1);

            anim_ctrl_ptr->is_dec_all_gif = TRUE;

            //post play once end indication msg
            GUIANIM_PostPlayOnceEndIndMsg(anim_ctrl_ptr);
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:DisplayGifRes:display gif fail!");
    }

    IMG_EnableTransparentColor(FALSE);
    
    //free gif handle
    if ((!MMK_IsFocusWin(anim_ctrl_ptr->win_handle)) &&
        (0 != anim_ctrl_ptr->res_gif_handle))
    {
        GUIRES_FreeHandle(anim_ctrl_ptr->res_gif_handle);
        anim_ctrl_ptr->res_gif_handle = 0;

        anim_ctrl_ptr->is_dec_all_gif = FALSE;
    }
}

/*****************************************************************************/
//  Description : copy or display bg buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CopyOrDisplayBg(
                           CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in
                           )
{
    uint32  bg_buf_size = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (anim_ctrl_ptr->display_info.is_bg_buf)
    {
        if (PNULL == anim_ctrl_ptr->bg_buf_ptr)
        {
            //alloc memory
            bg_buf_size = anim_ctrl_ptr->img_width*anim_ctrl_ptr->img_height*2;

            anim_ctrl_ptr->bg_buf_ptr = SCI_ALLOCA(bg_buf_size);
            if (PNULL != anim_ctrl_ptr->bg_buf_ptr)
            {
                SCI_MEMSET(anim_ctrl_ptr->bg_buf_ptr,0,bg_buf_size);
            
                LCD_CopyLayerBufToMem(
                    &base_ctrl_ptr->lcd_dev_info,
                    (GUI_COLOR_T *)anim_ctrl_ptr->bg_buf_ptr,
                    anim_ctrl_ptr->img_width,
                    anim_ctrl_ptr->img_height,
                    anim_ctrl_ptr->display_x,
                    anim_ctrl_ptr->display_y);
            }
            else
            {
                SCI_TRACE_LOW("GUIANIM:CopyOrDisplayBg:alloc bg buffer fail!");
            }
        }
        else
        {
            LCD_CopyMemToLayerBuf(
                &base_ctrl_ptr->lcd_dev_info,
                (GUI_COLOR_T *)anim_ctrl_ptr->bg_buf_ptr,
                anim_ctrl_ptr->img_width,
                anim_ctrl_ptr->img_height,
                anim_ctrl_ptr->display_x,
                anim_ctrl_ptr->display_y);
        }
    }
}

/*****************************************************************************/
//  Description : start gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_StartGifTimer(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  )
{
    uint32          time_out = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (MMITHEME_IsBacklightOn()) &&
        (0 == anim_ctrl_ptr->gif_timer_id))
    {
        //set time out
        time_out = anim_ctrl_ptr->delay_time;
        //SCI_TRACE_LOW:"GUIANIM_StartGifTimer: time_out is %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_DISPLAY_699_112_2_18_3_14_59_27,(uint8*)"d",time_out);

        //GIF动画 time out可能为0
        if (anim_ctrl_ptr->theme.period > time_out)
        {
            time_out = anim_ctrl_ptr->theme.period;
        }

        anim_ctrl_ptr->gif_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,time_out,FALSE);
    }
}

/*****************************************************************************/
//  Description : stop gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_StopGifTimer(
                                 uint8  *time_id_ptr
                                 )
{
    if (0 < *time_id_ptr)
    {
        MMK_StopTimer(*time_id_ptr);
        *time_id_ptr = 0;
    }
}
/*****************************************************************************/
//  Description : display img res bit map to target buffer
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GUIANIM_DisplayImgResWithBuffer(
                                                GUI_RECT_T      *lcd_rect_ptr,  //in:
                                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in:
                                               )
{
    BOOLEAN     result = TRUE;
    uint8       *img_ptr = PNULL;
    uint32      img_size = 0;
    GUI_POINT_T         dis_point = {0};
    GUIIMG_BITMAP_T     img_bitmap = {0};
    GUIRES_DISPLAY_T    display_info = {0};    
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (PNULL == anim_ctrl_ptr->target_buffer)
    {
        //alloc buffer
        anim_ctrl_ptr->target_buffer_size = anim_ctrl_ptr->img_width*anim_ctrl_ptr->img_height*2;
        anim_ctrl_ptr->target_buffer      = SCI_ALLOCA(anim_ctrl_ptr->target_buffer_size);
        anim_ctrl_ptr->target_buf_offset  = 0;
        if (PNULL == anim_ctrl_ptr->target_buffer)
        {
            //no buffer
            anim_ctrl_ptr->target_buffer = PNULL;
            anim_ctrl_ptr->target_buffer_size = 0;
            result = FALSE;
        }
        else
        {
            SCI_MEMSET(anim_ctrl_ptr->target_buffer,0,anim_ctrl_ptr->target_buffer_size);
            anim_ctrl_ptr->is_target_alloc = TRUE;

            //get resource pointer and size
            img_ptr = (uint8 *)anim_ctrl_ptr->theme.GetLabelImg(anim_ctrl_ptr->data_info.img_id,anim_ctrl_ptr->win_handle,&img_size);

            display_info.format = IMGREF_FORMAT_RGB565;
            //save decode buffer
            GUIRES_CopyImgBufEx(img_ptr,
                                anim_ctrl_ptr->img_width,
                                anim_ctrl_ptr->img_height,
                                anim_ctrl_ptr->target_buffer_size,
                                img_size,
                                &dis_point,
                                PNULL,
                                (GUI_COLOR_T *)anim_ctrl_ptr->target_buffer,
                                &display_info);
        }
    }

    if (result)
    {
        //set display point
        dis_point.x = anim_ctrl_ptr->display_x;
        dis_point.y = anim_ctrl_ptr->display_y;

        //set bitmap
        GUIANIM_GetDisplayBitmap(anim_ctrl_ptr,&img_bitmap);

        //display
        GUIIMG_DisplayBmp(FALSE,
                        lcd_rect_ptr,
                        &dis_point,
                        &img_bitmap,
                        &base_ctrl_ptr->lcd_dev_info);
    }
    return result;
}

/*****************************************************************************/
//  Description : set alpha for rgb565
//  Global resource dependence :
//  Author:zirui.li
//  Note:
/*****************************************************************************/
LOCAL void _SetAlpha(uint16* p_rgb565, uint8* p_alpha, uint32 width, uint32 height, uint32* p_argb)
{
	int32 pixels = width * height;

    if (PNULL == p_rgb565 || PNULL == p_alpha || PNULL == p_argb)
    {
        SCI_TRACE_LOW("[_SetAlpha]p_rgb565 or p_alpha or p_argb is PNULL!!!");
        return;
    }
    if (0 == width || 0 == height)
    {
        SCI_TRACE_LOW("[_SetAlpha]width or height is 0!!!");
        return;
    }
	while (pixels--)
	{
		uint8  alpha  = *p_alpha++;
		uint16 rgb565 = *p_rgb565++;
		uint32 argb  = 0;
		uint8  color = 0;

		argb = ARGB_SET_A(argb, alpha);

		color = RGB565_GET_R(rgb565);
		argb = ARGB_SET_R(argb, color);

		color = RGB565_GET_G(rgb565);
		argb = ARGB_SET_G(argb, color);

		color = RGB565_GET_B(rgb565);
		argb = ARGB_SET_B(argb, color);

		*p_argb++ = argb;
	}
}

/*****************************************************************************/
//  Description : get mask width && height
//  Global resource dependence :
//  Author:zirui.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMaskWidthHeight(CTRLANIM_OBJ_T* p_anim_ctrl,uint16* p_width,uint16* p_height)
{
    if (PNULL == p_anim_ctrl || PNULL == p_width || PNULL == p_height)
    {
        SCI_TRACE_LOW("[GetMaskWidthHeight]p_anim_ctrl or p_width or p_height is PNULL!!!");
        return FALSE;
    }
    //mask path
    if (GUIANIM_MASK_TYPE_PATH == p_anim_ctrl->display_info.mask_type)
    {
        if (PNULL == p_anim_ctrl->display_info.p_mask_path || 0 == p_anim_ctrl->display_info.mask_path_len)
        {
            SCI_TRACE_LOW("[GetMaskWidthHeight]anim_ctrl_ptr->display_info.p_mask_path or mask_path_len is PNULL!!!");
            return FALSE;
        }
        CTRLANIM_GetImgWidthHeight(p_width, p_height, p_anim_ctrl->display_info.p_mask_path, p_anim_ctrl->display_info.mask_path_len);
    }
    //mask id
    else if (GUIANIM_MASK_TYPE_ID == p_anim_ctrl->display_info.mask_type)
    {
        GUIRES_GetImgWidthHeight(p_width, p_height, p_anim_ctrl->display_info.mask_id, MMK_GetFocusWinId());
    }
    //no type
    else
    {
        SCI_TRACE_LOW("[GetMaskWidthHeight]mask type error!!!");
        return FALSE;
    }
    //check mask width && height
    if (0 == *p_width || 0 == *p_height)
    {
        SCI_TRACE_LOW("[GetMaskWidthHeight]mask width or height is 0!!!");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : decode mask image and get alpha array
//  Global resource dependence :
//  Author:zirui.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DecodeMaskImageAndGetAlpha(CTRLANIM_OBJ_T* p_anim_ctrl, uint16 width, uint16 height, uint8* p_alpha_arr)
{

    GUIIMG_SRC_T            mask_src     = {0};
    GUIIMG_DISPLAY_T        mask_display = {0};
    GUIIMG_DEC_OUT_T        mask_output  = {0};
    uint16                  i            = 0;
    uint32                  img_size     = 0;
    uint8*                  p_image_decompress = PNULL;
    IMG_RES_SRC_T           img_src      = {0};
    IMG_RES_DST_T           img_dst      = {0};
    IMG_RES_DST_CTL_T       dstctl       = {0};
    if (PNULL == p_anim_ctrl || PNULL == p_alpha_arr)
    {
        SCI_TRACE_LOW("[DecodeMaskImage]p_anim_ctrl or p_alpha_arr is PNULL!!!");
        return FALSE;
    }
    if (0 == width || 0 == height)
    {
        SCI_TRACE_LOW("[DecodeMaskImage]width or height is 0!!!");
        return FALSE;
    }
    //mask path
    if (GUIANIM_MASK_TYPE_PATH == p_anim_ctrl->display_info.mask_type)
    {
        if (PNULL == p_anim_ctrl->display_info.p_mask_path || 0 == p_anim_ctrl->display_info.mask_path_len)
        {
            SCI_TRACE_LOW("[DecodeMaskImage]anim_ctrl_ptr->display_info.p_mask_path or mask_path_len is PNULL!!!");
            return FALSE;
        }
        mask_src.is_file = TRUE;
        mask_src.full_path_ptr = p_anim_ctrl->display_info.p_mask_path;

        mask_display.dest_height = height;
        mask_display.dest_width = width;
        mask_display.is_handle_transparent = TRUE;

        mask_output.data_type = GUIIMG_DATE_TYPE_ARGB888;
        mask_output.decode_data_size = mask_display.dest_height * mask_display.dest_width * sizeof(uint32);
        mask_output.decode_data_ptr = (uint8*)SCI_ALLOC_APPZ(mask_output.decode_data_size);

        if (PNULL == mask_output.decode_data_ptr)
        {
            SCI_TRACE_LOW("[DecodeMaskImage]alloc mask_output.decode_data_ptr fail!!!");
            return FALSE;
        }
        // decode mask image and set alpha_arr
        if(GUIIMG_Decode(&mask_src, &mask_display, &mask_output))
        {
            SCI_TRACE_LOW("[DecodeMaskImage]handle mask:decode mask success");
            for (i = 0; i < (mask_output.decode_data_size)/ 4; i++)
            {
                p_alpha_arr[i] = mask_output.decode_data_ptr[i*4 + 3];
            }
            if (PNULL != mask_output.decode_data_ptr)
            {
                SCI_FREE(mask_output.decode_data_ptr);
                mask_output.decode_data_ptr = PNULL;
            }
        }
        else
        {
            SCI_FREE(mask_output.decode_data_ptr);
            SCI_TRACE_LOW("[DecodeMaskImage]decode mask image fail!!!");
            return FALSE;
        }
    }
    //mask id
    else if (GUIANIM_MASK_TYPE_ID == p_anim_ctrl->display_info.mask_type)
    {
        p_image_decompress = (uint8*)SCI_ALLOC_APPZ(width* height * sizeof(uint32));
        if (PNULL == p_image_decompress)
        {
            SCI_TRACE_LOW("[DecodeMaskImage]alloc alimage_decompress_ptrpha_arr fail!!!");
            return FALSE;
        }

        img_src.imgstream_ptr = (uint8*)MMI_GetLabelImage(p_anim_ctrl->display_info.mask_id, MMK_GetFocusWinId(), &img_size);
        img_src.imgstreamsize = img_size;
        img_src.imgrect.left = 0;
        img_src.imgrect.right = width - 1;
        img_src.imgrect.top = 0;
        img_src.imgrect.bottom = height - 1;

        img_dst.dstctl = &dstctl;
        img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
        img_dst.dstctl->dstmem = p_image_decompress;
        img_dst.dstctl->width = width;
        img_dst.dstctl->height = height;
        img_dst.dstrect.top = 0;
        img_dst.dstrect.left = 0;
        img_dst.dstrect.right = width- 1;
        img_dst.dstrect.bottom = height - 1;

        if(IMG_RES_SUCCESS == ALPHA_RES_Display_Img(&img_src, &img_dst))
        {
             SCI_TRACE_LOW("[DecodeMaskImage]handle mask:decode mask success");
             for (i = 0; i < (width* height * sizeof(uint32))/ 4; i++)
             {
                 p_alpha_arr[i] = p_image_decompress[i*4 + 3];
             }
             if (PNULL != p_image_decompress)
             {
                SCI_FREE(p_image_decompress);
                p_image_decompress = PNULL;
             }
        }
        else
        {
            SCI_TRACE_LOW("[DecodeMaskImage]decode mask image fail!!!");
            SCI_FREE(p_image_decompress);
            return FALSE;
        }
    }
    else
    {
        SCI_TRACE_LOW("[DecodeMaskImage]mask type error!!!");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : display bmp/wbmp/jpg/png image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayImgExceptGif(
                                  GUI_RECT_T        *lcd_rect_ptr,  //in:
                                  GUI_RECT_T        *image_rect_ptr,//in:
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                                  )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_horizontal = FALSE;
    GUI_POINT_T         dis_point = {0};
    GUIIMG_BITMAP_T     img_bitmap = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    uint16                  mask_width = 0;
    uint16                  mask_height = 0;
    uint8*                  p_alpha_arr = PNULL;
    uint32                  alpha_arr_size = 0;
    uint32*                 p_argb = PNULL;
    GUIIMG_BITMAP_T         data = {0};
    //kevin.lou modified:delete assert
    if ((0 == anim_ctrl_ptr->data_info.img_id)&&(PNULL != anim_ctrl_ptr->target_buffer))
    {
        //SCI_ASSERT(PNULL != anim_ctrl_ptr->target_buffer); /*assert verified*/

        //is horizontal display
        is_horizontal = GUIANIM_IsHorizontal(anim_ctrl_ptr);

        //set display point
        dis_point.x = anim_ctrl_ptr->display_x;
        dis_point.y = anim_ctrl_ptr->display_y;

        //set bitmap
        GUIANIM_GetDisplayBitmap(anim_ctrl_ptr,&img_bitmap);

        //handle mask
        if (anim_ctrl_ptr->display_info.is_handle_mask)
        {
            SCI_TRACE_LOW("[DisplayImgExceptGif]handle mask");

            // 1.get mask width && height
            result = GetMaskWidthHeight(anim_ctrl_ptr, &mask_width, &mask_height);
            if (TRUE == result)
            {
                if (mask_width != img_bitmap.img_width || mask_height != img_bitmap.img_height)
                {
                    SCI_TRACE_LOW("[DisplayImgExceptGif]mask size is not equal to original image size!!!");
                    return FALSE;
                }
            }
            else
            {
                SCI_TRACE_LOW("[DisplayImgExceptGif]get mask width && height fail!!!");
                return FALSE;
            }

            // 2.malloc alpha arr
            alpha_arr_size = mask_width * mask_height;
            p_alpha_arr = (uint8 *)SCI_ALLOC_APPZ(alpha_arr_size);
            if (PNULL == p_alpha_arr)
            {
                SCI_TRACE_LOW("[DisplayImgExceptGif]alloc alpha_arr fail!!!");
                return FALSE;
            }

            // 3.decode mask image and get alpha array
            result = DecodeMaskImageAndGetAlpha(anim_ctrl_ptr,mask_width,mask_height,p_alpha_arr);
            if (FALSE == result)
            {
                SCI_TRACE_LOW("[DisplayImgExceptGif]decode mask image fail!!!");
                SCI_FREE(p_alpha_arr);
                return FALSE;
            }

            // 4.set alpha data for original image decode buffer
            p_argb = (uint32*)SCI_ALLOC_APPZ(mask_width*mask_height*sizeof(uint32));
            if (PNULL == p_argb)
            {
                SCI_TRACE_LOW("[DisplayImgExceptGif]alloc argb fail!!!");
                SCI_FREE(p_alpha_arr);
                return FALSE;
            }
            _SetAlpha(img_bitmap.bit_ptr,p_alpha_arr,mask_width,mask_height,p_argb);

            SCI_TRACE_LOW("[DisplayImgExceptGif]handle mask:set alpha data for rgb565");
            if (PNULL != p_alpha_arr)
            {
                SCI_FREE(p_alpha_arr);
            }
            // 5.display
            data.bit_ptr = p_argb;
            data.date_type = GUIIMG_DATE_TYPE_ARGB888;
            data.img_height = img_bitmap.img_height;
            data.img_width = img_bitmap.img_width;

            GUIIMG_DisplayBmp(is_horizontal,
                lcd_rect_ptr,
                &dis_point,
                &data,
                &base_ctrl_ptr->lcd_dev_info);
            SCI_TRACE_LOW("[DisplayImgExceptGif]handle mask:display argb data");
            if (PNULL != p_argb)
            {
                SCI_FREE(p_argb);
            }
        }
        else
        {
            GUIIMG_DisplayBmp(is_horizontal,
                lcd_rect_ptr,
                &dis_point,
                &img_bitmap,
                &base_ctrl_ptr->lcd_dev_info);
        }

    }
    else
    {
        if (!DisplayWallpaperExceptGif(lcd_rect_ptr,anim_ctrl_ptr))
        {  
            if ((0 != anim_ctrl_ptr->data_info.img_id)&&(anim_ctrl_ptr->display_info.is_img_res_need_buffer))
            {
                if (GUIANIM_DisplayImgResWithBuffer(lcd_rect_ptr,anim_ctrl_ptr))
                {
                    return TRUE;
                }                 
            }
            //display
            IMG_EnableTransparentColor(TRUE);
            GUIRES_DisplayImg(PNULL,
                lcd_rect_ptr,
                image_rect_ptr,
                anim_ctrl_ptr->win_handle,
                anim_ctrl_ptr->data_info.img_id,
                &base_ctrl_ptr->lcd_dev_info);
            IMG_EnableTransparentColor(FALSE);                       
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display image for wallpaper
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayWallpaperExceptGif(
										GUI_RECT_T		*lcd_rect_ptr,  //in:
										CTRLANIM_OBJ_T	*anim_ctrl_ptr  //in:
										)
{
	BOOLEAN				result = FALSE;

	if ((anim_ctrl_ptr->is_wallpaper_ctrl) &&
        (anim_ctrl_ptr->display_info.is_save_wallpaper))
    {
        result = TRUE;
        // Low memory 版本墙纸直接刷屏
#ifdef LOW_MEMORY_SUPPORT
        if (0 != anim_ctrl_ptr->data_info.img_id)
        {
            return FALSE;
        }
#endif
        result = GUIANIM_DisplayImgResWithBuffer(lcd_rect_ptr,anim_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display anim
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayAnim(
                          BOOLEAN           is_only_update, //in:
                          GUI_RECT_T        *lcd_rect_ptr,  //in:
                          GUI_RECT_T        *image_rect_ptr,//in:
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                          )
{
    BOOLEAN     result = TRUE;
    uint32      img_data_size = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //copy or display bg
    CopyOrDisplayBg(anim_ctrl_ptr);
    
    //reload
    anim_ctrl_ptr->src_data_buffer = (uint8 *)anim_ctrl_ptr->theme.GetLabelAnim(anim_ctrl_ptr->data_info.anim_id,anim_ctrl_ptr->win_handle,&img_data_size);
    anim_ctrl_ptr->src_data_size   = img_data_size;

    IMG_EnableTransparentColor(TRUE);
    GUIRES_DisplayAnimByPtr(anim_ctrl_ptr->src_data_buffer,
        anim_ctrl_ptr->cur_frame,
        anim_ctrl_ptr->src_data_size,
        PNULL,
        lcd_rect_ptr,
        image_rect_ptr,
        &(base_ctrl_ptr->lcd_dev_info));
    IMG_EnableTransparentColor(FALSE);

    if ((!is_only_update) &&
        (!anim_ctrl_ptr->is_decoding) &&
        (anim_ctrl_ptr->is_focus) && 
        (!anim_ctrl_ptr->is_pause) && 
        (!anim_ctrl_ptr->display_info.is_disp_one_frame))
    {
        //start anim timer
        GUIANIM_StartAnimTimer(anim_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : start anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_StartAnimTimer(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr
                                   )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) && 
        (MMITHEME_IsBacklightOn()) &&
        (0 == anim_ctrl_ptr->timer_id))
    {
        anim_ctrl_ptr->timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,anim_ctrl_ptr->theme.res_period,FALSE);
    }
}

/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_StopAnimTimer(
                                  uint8     *time_id_ptr
                                  )
{
    if (0 < *time_id_ptr)
    {
        MMK_StopTimer(*time_id_ptr);
        *time_id_ptr = 0;
    }
}

/*****************************************************************************/
//  Description : call back function
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void CallBackFunc(
                        CTRLANIM_OBJ_T  *anim_ctrl_ptr
                        )
{
    GUI_RECT_T              anim_rect = {0};
    GUI_RECT_T              cross_rect = {0};
    GUIANIM_OWNER_DRAW_T    owner_draw = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr->CallBackFunc.OwnerDrawFunc) &&
         (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect)))
    {
        //get anim rect
        if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,anim_rect))
        {
            owner_draw.param        = anim_ctrl_ptr->CallBackFunc.param;
            owner_draw.display_x    = anim_ctrl_ptr->display_x;
            owner_draw.display_y    = anim_ctrl_ptr->display_y;
            owner_draw.display_rect = cross_rect;
            owner_draw.win_handle   = anim_ctrl_ptr->win_handle;
            owner_draw.ctrl_handle  = base_ctrl_ptr->handle;
            owner_draw.lcd_dev      = base_ctrl_ptr->lcd_dev_info;

            //set bitmap data
            owner_draw.bitmap_data.bit_ptr    = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
            owner_draw.bitmap_data.img_width  = anim_ctrl_ptr->img_width;
            owner_draw.bitmap_data.img_height = anim_ctrl_ptr->img_height;
            owner_draw.bitmap_data.date_type  = anim_ctrl_ptr->target_data_type;

            anim_ctrl_ptr->CallBackFunc.OwnerDrawFunc(&owner_draw);
        }
    }
}

/*****************************************************************************/
//  Description : display default wallpaper,only for gif file or data buffer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDefaultWallpaper(
                                   GUI_RECT_T       *lcd_rect_ptr,  //in:
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr
                                   )
{
    BOOLEAN         is_horizontal = FALSE;
    GUI_POINT_T     dis_point = {0};
    GUIIMG_BITMAP_T img_bitmap = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if ((anim_ctrl_ptr->is_wallpaper_ctrl) && 
        (PNULL != anim_ctrl_ptr->target_buffer) &&
        (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type) &&
        (0 == anim_ctrl_ptr->data_info.img_id) &&
        (anim_ctrl_ptr->is_adjust_disp_pos))
    {
        //is horizontal display
        is_horizontal = GUIANIM_IsHorizontal(anim_ctrl_ptr);

        //set display point
        dis_point.x = anim_ctrl_ptr->display_x;
        dis_point.y = anim_ctrl_ptr->display_y;

        //set bitmap
        GUIANIM_GetDisplayBitmap(anim_ctrl_ptr,&img_bitmap);

        //display
        GUIIMG_DisplayBmp(is_horizontal,
            lcd_rect_ptr,
            &dis_point,
            &img_bitmap,
            &base_ctrl_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : display default picture icon
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDefaultPicIcon(
                                 CTRLANIM_OBJ_T     *anim_ctrl_ptr
                                 )
{
    BOOLEAN         result = FALSE;
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          default_width = 0;
    uint16          default_height = 0;
    uint32          default_img_id = 0;
    GUI_RECT_T      anim_rect = {0};
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      lcd_rect = {0};
    GUI_RECT_T      image_display_rect = {0};
    GUI_BG_TYPE_E   bg_type = GUI_BG_NONE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //get anim rect,except border
    if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
    {
        //get bg type and image id
        if (anim_ctrl_ptr->is_support)
        {
            bg_type        = GUI_BG_IMG;
            default_img_id = anim_ctrl_ptr->theme.default_img_id;
        }
        else
        {
            bg_type        = anim_ctrl_ptr->theme.error_bg.bg_type;
            default_img_id = anim_ctrl_ptr->theme.error_bg.img_id;
        }

        //get default width and height
        switch (bg_type)
        {
        case GUI_BG_IMG:
            if (GUIRES_GetImgWidthHeight(&default_width,&default_height,default_img_id,anim_ctrl_ptr->win_handle))
            {
                result = TRUE;
            }
            else
            {
                SCI_TRACE_LOW("GUIANIM:DisplayDefaultPicIcon:default_img_id is invalid!");
            }
            break;

        case GUI_BG_COLOR:
            //kevin.lou modified:delete assert
            //SCI_ASSERT(anim_ctrl_ptr->display_info.is_zoom); /*assert verified*/
            if (anim_ctrl_ptr->display_info.is_zoom)
            {
                default_width  = anim_ctrl_ptr->zoom_dest.cur_img_width;
                default_height = anim_ctrl_ptr->zoom_dest.cur_img_height;
                result = TRUE;
            }            
            break;

        default:
            SCI_TRACE_LOW("GUIANIM:DisplayDefaultPicIcon:bg_type %d is error!", bg_type);
            break;
        }

        if (result)
        {
            //set init display x and y and adjust display position
            display_x = anim_rect.left;
            display_y = anim_rect.top;
            CTRLANIM_AdjustDisplayPosition(&display_x,
                &display_y,
                default_width,
                default_height,
                anim_rect,
                anim_ctrl_ptr->display_info.align_style);

            //set image display rect
            image_display_rect.left   = (int16)MAX(display_x,anim_rect.left);
            image_display_rect.top    = (int16)MAX(display_y,anim_rect.top);
            image_display_rect.right  = (int16)MIN((image_display_rect.left + (int16)default_width - 1),anim_rect.right);
            image_display_rect.bottom = (int16)MIN((image_display_rect.top + (int16)default_height - 1),anim_rect.bottom);

            //get crossed rect is lcd rect
            if (GUI_IntersectRect(&lcd_rect,base_ctrl_ptr->display_rect,image_display_rect))
            {
                switch (bg_type)
                {
                case GUI_BG_IMG:
                    //image rect
                    image_rect.left   = (int16)(lcd_rect.left - display_x);
                    image_rect.top    = (int16)(lcd_rect.top - display_y);
                    image_rect.right  = (int16)(lcd_rect.right - display_x);
                    image_rect.bottom = (int16)(lcd_rect.bottom - display_y);

                    IMG_EnableTransparentColor(TRUE);
                    //display default image icon
                    GUIRES_DisplayImg(PNULL,
                        &lcd_rect,
                        &image_rect,
                        anim_ctrl_ptr->win_handle,
                        default_img_id,
                        &base_ctrl_ptr->lcd_dev_info);
                    IMG_EnableTransparentColor(FALSE);
                    break;

                case GUI_BG_COLOR:
                    GUI_FillRect(&base_ctrl_ptr->lcd_dev_info,
                        lcd_rect,
                        anim_ctrl_ptr->theme.error_bg.color);
                    break;

                default:
                    break;
                }
            }
            else
            {
                SCI_TRACE_LOW("GUIANIM:DisplayDefaultPicIcon:anim image rect and display rect are not crossed!");
            }
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:DisplayDefaultPicIcon:anim rect is empty!");
    }
}

/*****************************************************************************/
//  Description : display anim frame
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_DisplayFrame(
                                 CTRLANIM_OBJ_T     *anim_ctrl_ptr
                                 )
{
    GUI_RECT_T      border_rect = {0};

    if ((anim_ctrl_ptr->display_info.is_disp_frame) &&
        (GUI_BORDER_NONE != anim_ctrl_ptr->theme.frame.type))
    {
        if ((GUIIMG_DATE_TYPE_RGB565 == anim_ctrl_ptr->target_data_type) &&
            (anim_ctrl_ptr->img_width > anim_ctrl_ptr->theme.frame.width) &&
            (anim_ctrl_ptr->img_height > anim_ctrl_ptr->theme.frame.width))
        {
            //set border rect
            border_rect.right  = (int16)(anim_ctrl_ptr->img_width - 1);
            border_rect.bottom = (int16)(anim_ctrl_ptr->img_height - 1);

            //display border
            GUI_DisplayBorderToBuf((GUI_COLOR_T *)(anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset),
                anim_ctrl_ptr->img_width,
                anim_ctrl_ptr->img_height,
                border_rect,
                &anim_ctrl_ptr->theme.frame);
        }
    }
}

