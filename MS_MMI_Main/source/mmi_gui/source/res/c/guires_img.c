/*****************************************************************************
** File Name:      guires_img.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2007        Jassmine              Creat
******************************************************************************/

#ifndef _GUIRES_IMG_C_
#define _GUIRES_IMG_C_

/*lint -e775*/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "guires.h"
#include "guilcd.h"
#include "mmi_theme.h"
#include "dal_img.h"
#include "ui_layer.h"
#include "mmi_autotest.h"
#include "img_ref_types.h"
#include "guigraph.h"
#include "guicommon.h"
#include "mmi_utility.h"
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
extern const uint8* MMI_GetLabelAnim(MMI_ANIM_ID_T  label, MMI_WIN_ID_T win_id, uint32 *size_ptr);
extern const uint8* MMI_GetLabelImage ( MMI_IMAGE_ID_T label, MMI_WIN_ID_T win_id, uint32 *size );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get image width and height etc. by image data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImgInfo(
                         uint16             *width_ptr,     //in/out:may PNULL
                         uint16             *height_ptr,    //in/out:may PNULL
                         uint16             *frame_num_ptr, //in/out:may PNULL
                         GUIRES_IMG_SRC_T   *img_src_ptr    //in:
                         );

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayImg(
                         GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                         GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                         GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                         IMG_RES_SRC_T          *res_src_ptr,       //in:
                         GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                         const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                         );

/*****************************************************************************/
//  Description : get image rect to display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImgRect(
                         uint16         img_width,          //in
                         uint16         img_height,         //in
                         GUI_RECT_T     *in_img_rect_ptr,   //in:may PNULL代表从图片(0,0)点显示
                         GUI_RECT_T     *out_img_rect_ptr   //in/out:
                         );

/*****************************************************************************/
//  Description : get image to display rect in lcd
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayRect(
                             uint16        img_width,          //in
                             uint16        img_height,         //in
                             uint16        lcd_width,          //in
                             uint16        lcd_height,         //in
                             GUI_POINT_T   *dis_point_ptr,     //in:may PNULL代表指定rect区域显示
                             GUI_RECT_T    *in_dis_rect_ptr,   //in:lcd display rect,may PNULL代表指定起始点显示
                             GUI_RECT_T    *out_dis_rect_ptr   //in/out:
                             );

/*****************************************************************************/
//  Description : get display image control info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayCtl(
                            GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
                            IMG_RES_DST_CTL_T       *display_ctl_ptr,   //in/out
                            const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                            LCD_INFO_T              *lcd_info_ptr       //in/out
                            );

/*****************************************************************************/
//  Description : adjust image rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: image rect, display rect宽高必须相等,而且是二者最小值
/*****************************************************************************/
LOCAL BOOLEAN AdjustImgAndDisRect(
                                  uint16           img_height,     //in
                                  GUI_RECT_T       *img_rect_ptr,  //in
                                  GUI_RECT_T       *dis_rect_ptr,  //in
                                  IMG_RES_TYPE_E   img_type        //in
                                  );

/*****************************************************************************/
//  Description : display resource anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAnim(
                          GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                          GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                          GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                          IMG_RES_SRC_T             *res_src_ptr,       //in:
                          GUIRES_ANIM_DIS_T         *anim_dis_ptr,      //in:
                          GUIRES_ANIM_INFO_T        *anim_info_ptr,     //in/out:may PNULL
                          GUIRES_DISPLAY_T          *display_ptr,       //in:may PNULL,default
                          const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                          );

/*****************************************************************************/
//  Description : copy image buffer to the pointed buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyImgBuf(
                         const uint8        *src_data_ptr,  //in:src data buffer
                         uint16             dest_width,     //in:
                         uint16             dest_height,    //in:
                         uint32             dest_buf_size,  //in:
                         uint32             src_data_size,  //in:
                         GUI_POINT_T        *src_point_ptr, //in:may PNULL代表指定rect区域
                         GUI_RECT_T         *src_rect_ptr,  //in:may PNULL代表指定起始点
                         GUI_COLOR_T        *dest_buf_ptr,  //out:dest buffer
                         GUIRES_DISPLAY_T   *display_ptr    //in:display info,may PNULL
                         );

/*****************************************************************************/
//  Description : get sabm image info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSabmImgInfo(
                             const uint8 *data_ptr,
                             uint32 data_size,
                             GUIRES_IMG_INFO_T *info_ptr   //in/out
                             );

/*****************************************************************************/
//  Description : get graph data info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetGraphDataInfo(
                               const uint8 *data_ptr,
                               uint32 data_size,
                               GUIRES_IMG_INFO_T *info_ptr   //in/out
                               );

/*****************************************************************************/
//  Description : display sabm image
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplaySabmImg(
                             GUI_RECT_T             *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                             GUI_RECT_T             *full_rect_ptr,     //in:
                             GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                             IMG_RES_SRC_T          *res_src_ptr,       //in:
                             GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                             const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                             );

/*****************************************************************************/
//  Description : display graph data
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayGraphData(
							   GUI_POINT_T            *display_point_ptr,
                               GUI_RECT_T             *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                               GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                               IMG_RES_SRC_T          *res_src_ptr,       //in:
                               GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                               const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                               );


LOCAL BOOLEAN DisplayImgDst(
    GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
    GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    IMG_RES_SRC_T           *res_src_ptr,       //in:
    GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
    IMG_RES_DST_CTL_T const *dst_ptr
);
LOCAL BOOLEAN DisplaySabmImgDst(
    GUI_RECT_T              *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    IMG_RES_SRC_T           *res_src_ptr,       //in:
    GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
    IMG_RES_DST_CTL_T const *dst_ptr
);

/*==============================================================================
Description: DisplayGraphDataDst
Global resource dependence: NONE
Author: James.Zhang
Note: 将数据写到res dst ctrl
==============================================================================*/
LOCAL BOOLEAN DisplayGraphDataDst(
                                  GUI_RECT_T              *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                                  GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                  IMG_RES_SRC_T           *res_src_ptr,       //in:
                                  GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
                                  IMG_RES_DST_CTL_T const *dst_ptr
                                  );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get sabm image info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSabmImgInfo(
                             const uint8 *data_ptr,
                             uint32 data_size,
                             GUIRES_IMG_INFO_T *info_ptr   //in/out
                             )
{
    BOOLEAN result = FALSE;
    SABM_INFO_T info = {0};

    if ( MMI_GetSabmImgInfo( data_ptr, data_size, &info ) )
    {
        if ( PNULL != info_ptr )
        {
            info_ptr->frame_num = 0;
            info_ptr->img_type  = GUIANIM_TYPE_IMAGE_PNG;
            info_ptr->width     = info.width;
            info_ptr->height    = info.height;
            info_ptr->is_argb   = TRUE;
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get graph data info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetGraphDataInfo(
                               const uint8 *data_ptr,
                               uint32 data_size,
                               GUIRES_IMG_INFO_T *info_ptr   //in/out
                               )
{
    BOOLEAN result = FALSE;
    MMI_GRAPH_DATA_T  data_info = {0};
    MMI_GRAPH_DATA_T* data_info_ptr = ( PNULL == info_ptr ) ? PNULL : &data_info;

    if ( MMI_GetGraphDataInfo( data_ptr, data_size, data_info_ptr ) )
    {
        if ( PNULL != info_ptr
            && PNULL != data_info_ptr )
        {
            info_ptr->frame_num = 0;
            info_ptr->img_type  = GUIANIM_TYPE_IMAGE_BMP;
            info_ptr->width     = data_info_ptr->width;
            info_ptr->height    = data_info_ptr->height;
            info_ptr->is_argb   = FALSE;
        }

        result = TRUE;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : display sabm image
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplaySabmImg(
                             GUI_RECT_T             *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                             GUI_RECT_T             *full_rect_ptr,     //in:
                             GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                             IMG_RES_SRC_T          *res_src_ptr,       //in:
                             GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                             const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                             )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          dis_rect = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T          blt_rect = {0};
#endif
    IMG_RES_DST_CTL_T   display_ctl = {0};
    LCD_INFO_T          lcd_info = {0};
    GUI_RECT_T          display_rect = {0};

    if ( PNULL != src_rect_ptr
        && PNULL != res_src_ptr
        && PNULL != dev_info_ptr )
    {
        src_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, src_rect_ptr, &display_rect);
        
        if ( GetDisplayCtl(display_ptr,&display_ctl,dev_info_ptr,&lcd_info) )
        {
            BOOLEAN is_got_rect = FALSE;
            
            //get display rect
            is_got_rect = GetDisplayRect( src_rect_ptr->right - src_rect_ptr->left + 1,
                src_rect_ptr->bottom - src_rect_ptr->top + 1,
                lcd_info.lcd_width,
                lcd_info.lcd_height,
                PNULL,
                src_rect_ptr,
                &dis_rect);

            if (!is_got_rect)
            {
                return FALSE;
            }

            //keep old dis_rect
            display_rect = dis_rect;
            
#ifdef UI_MULTILAYER_SUPPORT
            LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &dis_rect, &blt_rect);
            
            UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr,&blt_rect);
#endif
            
            result = MMI_DisplaySabmImg( (IMGREF_FORMAT_ARGB888 == display_ctl.format) ? MMI_BUF_ARGB888 : MMI_BUF_RGB565,
                res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, 
                &display_rect, full_rect_ptr, image_rect_ptr, display_ctl.width, display_ctl.height, display_ctl.dstmem);

            //store update rect
            MMITHEME_StoreUpdateRect(dev_info_ptr,dis_rect);
        }
    }
    
    return result;
}


/*****************************************************************************/
//  Description : display graph data
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayGraphData(
							   GUI_POINT_T            *display_point_ptr,
                               GUI_RECT_T             *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                               GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                               IMG_RES_SRC_T          *res_src_ptr,       //in:
                               GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                               const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                               )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          dis_rect = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T          blt_rect = {0};
#endif
    IMG_RES_DST_CTL_T   display_ctl = {0};
    LCD_INFO_T          lcd_info = {0};
    GUI_RECT_T          display_rect = {0};
	GUI_RECT_T          src_rect = {0};
	GUIRES_IMG_INFO_T   info = {0};

    if ( PNULL != res_src_ptr
        && PNULL != dev_info_ptr )
    {
		if ( PNULL == src_rect_ptr )
		{
			if ( PNULL == display_point_ptr )
			{
				return FALSE;
			}

			if ( !GetGraphDataInfo( res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, &info ) )
			{
				return FALSE;
			}

			src_rect.left = display_point_ptr->x;
			src_rect.top = display_point_ptr->y;
			src_rect.right = src_rect.left + info.width - 1;
			src_rect.bottom = src_rect.top + info.height - 1;

			src_rect_ptr = &src_rect;
		}
		else
		{
			src_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, src_rect_ptr, &display_rect);	
		}
        
        if ( GetDisplayCtl(display_ptr,&display_ctl,dev_info_ptr,&lcd_info) )
        {
            BOOLEAN is_got_rect = FALSE;
            
            //get display rect
            is_got_rect = GetDisplayRect( src_rect_ptr->right - src_rect_ptr->left + 1,
                src_rect_ptr->bottom - src_rect_ptr->top + 1,
                lcd_info.lcd_width,
                lcd_info.lcd_height,
                PNULL,
                src_rect_ptr,
                &dis_rect);

            if (!is_got_rect)
            {
                return FALSE;
            }

            //keep old dis_rect
            display_rect = dis_rect;
            
#ifdef UI_MULTILAYER_SUPPORT
            LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &dis_rect, &blt_rect);
            
            UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr,&blt_rect);
#endif
            
            result = MMI_DisplayGraphData( (IMGREF_FORMAT_ARGB888 == display_ctl.format) ? MMI_BUF_ARGB888 : MMI_BUF_RGB565,
                res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, 
                &display_rect, image_rect_ptr, display_ctl.width, display_ctl.height, display_ctl.dstmem);

            //store update rect
            MMITHEME_StoreUpdateRect(dev_info_ptr,dis_rect);
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get resource image info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgInfo(
                                 GUIRES_IMG_SRC_T  *src_ptr,   //in
                                 GUIRES_IMG_INFO_T *info_ptr   //in/out
                                 )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    IMG_RES_INFO_T      img_info = {0};
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;

    if (PNULL == src_ptr || PNULL == info_ptr)
    {
        return result;
    }

    //get sabm image info first
    if ( GetSabmImgInfo( src_ptr->data_ptr, src_ptr->data_size, info_ptr ) )
    {
        return TRUE;
    }

    //get graph data info second
    if ( GetGraphDataInfo( src_ptr->data_ptr, src_ptr->data_size, info_ptr ) )
    {
        return TRUE;
    }

    //set param
    res_src.imgstream_ptr = src_ptr->data_ptr;
    res_src.imgstreamsize = src_ptr->data_size;

    if (!res_src.imgstream_ptr)
    {
        return FALSE;
    }

    //get image info
    res_result = IMG_RES_GetImageInfo(&res_src,&img_info);
    if (IMG_RES_SUCCESS == res_result)
    {
        result = TRUE;

        //set output param
        info_ptr->width     = img_info.width;
        info_ptr->height    = img_info.height;
        info_ptr->frame_num = img_info.framenum;
        switch (img_info.img_type)
        {
        case IMG_RES_BMP:
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_BMP;
            break;

        case IMG_RES_ARGB:
            info_ptr->is_argb  = TRUE;
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_BMP;
            break;

        case IMG_RES_JPEG:
        case IMG_RES_SJPEG:
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_JPG;
            break;

        case IMG_RES_PNG:
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_PNG;
            break;
#ifdef WEBP_DEC_SUPPORT_WEBP_SW
        case IMG_RES_WEBP:
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_WEBP;
            break;
#endif
        case IMG_RES_GIF:
        case IMG_RES_GIF_NORMAL:
            info_ptr->img_type = GUIANIM_TYPE_IMAGE_GIF;
            break;

        case IMG_RES_MOV_ARGB:
            info_ptr->img_type = GUIANIM_TYPE_ANIM_ARGB;
            break;

        default:
            result = FALSE;
            info_ptr->img_type = GUIANIM_TYPE_UNKNOWN;
            //SCI_TRACE_LOW:"GUIRES_GetImgInfo:img_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_492_112_2_18_3_20_46_255,(uint8*)"d",img_info.img_type);
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIRES_GetImgInfo:res_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_498_112_2_18_3_20_46_256,(uint8*)"d",res_result);
    }

    return (result);
}

/*****************************************************************************/
//  Description : is ARGB image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_IsArgb(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_SRC_T    img_src = {0};
    GUIRES_IMG_INFO_T   img_info = {0};

    //check image id
    if (MMITHEME_CheckImageID(img_id))
    {
        //get resource pointer and size
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(img_src.data_size));
        if (PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            if (GUIRES_GetImgInfo(&img_src,&img_info))
            {
                if (img_info.is_argb)
                {
                    result = TRUE;
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_IsArgb == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_534_112_2_18_3_20_46_257,(uint8*)"");
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is sabm image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_IsSabm(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_SRC_T    img_src = {0};
    
    //check image id
    if (MMITHEME_CheckImageID(img_id))
    {
        //get resource pointer and size
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(img_src.data_size));

        if (PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            result = GetSabmImgInfo( img_src.data_ptr, img_src.data_size, PNULL );
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_IsSabm == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_565_112_2_18_3_20_46_258,(uint8*)"");
        }
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : get image width and height by image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeight(
                                        uint16          *width_ptr, //may PNULL
                                        uint16          *height_ptr,//may PNULL
                                        MMI_IMAGE_ID_T  img_id,     //in
                                        MMI_WIN_ID_T    win_id      //in
                                        )
{
    GUIRES_IMG_SRC_T    img_src = {0};
    BOOLEAN             result  = FALSE;

    if (PNULL == width_ptr && PNULL == height_ptr)
    {
        return result;
    }

    //check image id
    if (MMITHEME_CheckImageID(img_id))
    {
        //get resource pointer and size
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(img_src.data_size));
        if (PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            //get image info
            result = GetImgInfo(width_ptr,height_ptr,PNULL,&img_src);
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_GetImgWidthHeight == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_604_112_2_18_3_20_46_259,(uint8*)"");
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get image width and height etc. by image data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImgInfo(
                         uint16             *width_ptr,     //in/out:may PNULL
                         uint16             *height_ptr,    //in/out:may PNULL
                         uint16             *frame_num_ptr, //in/out:may PNULL
                         GUIRES_IMG_SRC_T   *img_src_ptr    //in:
                         )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_INFO_T   img_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT((PNULL != width_ptr) || (PNULL != height_ptr) || (PNULL != frame_num_ptr));  /*assert verified*/
    //SCI_ASSERT(PNULL != img_src_ptr);   /*assert verified*/
    if (((PNULL == width_ptr) && (PNULL == height_ptr) && (PNULL == frame_num_ptr)) || (PNULL == img_src_ptr))
    {
        return FALSE;
    }

    if (PNULL == img_src_ptr->data_ptr || 0 >= img_src_ptr->data_size)
    {
        return result;
    }

    if (GUIRES_GetImgInfo(img_src_ptr,&img_info))
    {
        //set width
        if (PNULL != width_ptr)
        {
            *width_ptr = img_info.width;
        }

        //set height
        if (PNULL != height_ptr)
        {
            *height_ptr = img_info.height;
        }

        //set frame number
        if (PNULL != frame_num_ptr)
        {
            *frame_num_ptr = img_info.frame_num;
        }
        
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image width and height by image data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeightByPtr(
                                             uint8         *data_ptr,  //in:
                                             uint16        *width_ptr, //may PNULL
                                             uint16        *height_ptr,//may PNULL
                                             uint32        data_size   //in:
                                             )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_SRC_T    img_src = {0};

    if (PNULL == data_ptr || 0 >= data_size
        || ((PNULL == width_ptr) && (PNULL == height_ptr)))
    {
        return result;
    }

    //set src
    img_src.data_ptr  = data_ptr;
    img_src.data_size = data_size;

    //get image info
    result = GetImgInfo(width_ptr,height_ptr,PNULL,&img_src);

    return (result);
}

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImg(
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if (PNULL == dev_info_ptr
        || ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)))
    {
        return result;
    }

    //check image id
    if ((MMITHEME_CheckImageID(img_id)) /*&&
        (MMITHEME_IsBacklightOn())*/)
    {
        MMI_TraceAutoTestOfImage(img_id);
        
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //display image
            result = DisplayImg(display_point_ptr,
                        display_rect_ptr,
                        image_rect_ptr,
                        &res_src,
                        PNULL,
                        dev_info_ptr);        
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplayImg == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_736_112_2_18_3_20_46_260,(uint8*)"");
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}


/*****************************************************************************/
//  Description : display sabm image
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplaySabmImg(
                                     GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                     GUI_RECT_T             *full_rect_ptr,     //in:may PNULL代表指定rect区域显示
                                     GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                     MMI_WIN_ID_T           win_id,             //in:win id
                                     MMI_IMAGE_ID_T         img_id,             //in:image id
                                     const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                     )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if ( PNULL == dev_info_ptr
        || PNULL == display_rect_ptr )
    {
        return FALSE;
    }

    //check image id
    if ((MMITHEME_CheckImageID(img_id)) &&
        (MMITHEME_IsBacklightOn()))
    {
        MMI_TraceAutoTestOfImage(img_id);
        
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //get sabm image info first
            if ( GetSabmImgInfo( res_src.imgstream_ptr, res_src.imgstreamsize, PNULL ) )
            {
                result = DisplaySabmImg( display_rect_ptr, full_rect_ptr, image_rect_ptr, &res_src, PNULL, dev_info_ptr );
            }
            //get graph data info second
            else if ( GetGraphDataInfo( res_src.imgstream_ptr, res_src.imgstreamsize, PNULL ) )
            {
                result = DisplayGraphData( PNULL, display_rect_ptr, image_rect_ptr, &res_src, PNULL, dev_info_ptr );
            }
            else
            {
                //display image
                result = DisplayImg(PNULL,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    PNULL,
                    dev_info_ptr);     
            }
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplaySabmImg == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_803_112_2_18_3_20_46_261,(uint8*)"");
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgEx(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr        //in:display info
                                   )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if (PNULL == dev_info_ptr
        || ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)))
    {
        return result;
    }

    //check image id
    if ((MMITHEME_CheckImageID(img_id)) &&
        (MMITHEME_IsBacklightOn()))
    {
        MMI_TraceAutoTestOfImage(img_id);
        
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));        
        
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //display image
            result = DisplayImg(display_point_ptr,
                        display_rect_ptr,
                        image_rect_ptr,
                        &res_src,
                        display_ptr,
                        dev_info_ptr);        
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplayImgEx == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_858_112_2_18_3_20_46_262,(uint8*)"");
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}
/*****************************************************************************/
//  Description : display resource image with color exchange
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgWitchColorExchange(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   BOOLEAN                  is_exchange_color,  //in:是否颜色替换
                                   uint32                   color               //in:替换目标颜色值
                                   )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if (PNULL == dev_info_ptr
        || ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)))
    {
        return result;
    }

    //check image id
    if ((MMITHEME_CheckImageID(img_id)) &&
        (MMITHEME_IsBacklightOn()))
    {
        MMI_TraceAutoTestOfImage(img_id);
        
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
        
        if (is_exchange_color)
        {
            res_src.color_type  = IMG_COLOR_EXCHANGE;
            res_src.color_value = color;
        }
        
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //display image
            result = DisplayImg(display_point_ptr,
                        display_rect_ptr,
                        image_rect_ptr,
                        &res_src,
                        PNULL,
                        dev_info_ptr);        
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplayImgWitchColorExchange == error when MMI_GetLabelImage"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_919_112_2_18_3_20_46_263,(uint8*)"");
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayImg(
                         GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                         GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                         GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                         IMG_RES_SRC_T          *res_src_ptr,       //in:
                         GUIRES_DISPLAY_T       *display_ptr,       //in:may PNULL,default
                         const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                         )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          img_rect = {0};
    GUI_RECT_T          dis_rect = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T          blt_rect = {0};
#endif
    IMG_RES_INFO_T      img_info = {0};
    IMG_RES_DST_CTL_T   display_ctl = {0};
    IMG_RES_DST_T       display_info = {0};
    LCD_INFO_T          lcd_info = {0};
    GUI_RECT_T          display_rect = {0};
    GUI_POINT_T         display_point = {0};
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;


    //kevin.lou modified:delete assert
    //SCI_ASSERT((PNULL != display_point_ptr) || (PNULL != display_rect_ptr)); /*assert verified*/
    //SCI_ASSERT(PNULL != res_src_ptr);   /*assert verified*/
    //SCI_ASSERT(PNULL != dev_info_ptr);  /*assert verified*/
    if (((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)) 
        || (PNULL == res_src_ptr)
        || (PNULL == dev_info_ptr))
    {
        return FALSE;
    }

    if ( PNULL != display_rect_ptr
        && GUI_IsInvalidRectEx( *display_rect_ptr ) )
    {
        return FALSE;
    }

    if ((PNULL != display_ptr) &&
        (display_ptr->is_exchange_color))
    {
        res_src_ptr->color_type  = IMG_COLOR_EXCHANGE;
        res_src_ptr->color_value = display_ptr->exchange_color;
    }

    //get sabm image info first
    if ( GetSabmImgInfo( res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, PNULL ) )
    {
        return DisplaySabmImg( display_rect_ptr, PNULL, image_rect_ptr, res_src_ptr, display_ptr, dev_info_ptr );
    }
    //get graph data info second
    else if ( GetGraphDataInfo( res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, PNULL ) )
    {
        return DisplayGraphData( display_point_ptr, display_rect_ptr, image_rect_ptr, res_src_ptr, display_ptr, dev_info_ptr );
    }

    //get image info
    res_result = IMG_RES_GetImageInfo(res_src_ptr,&img_info);
    if (IMG_RES_SUCCESS == res_result)
    {
        display_point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, display_point_ptr, &display_point);
        display_rect_ptr  = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, display_rect_ptr, &display_rect);

        //get display control info and image rect
        if ((GetDisplayCtl(display_ptr,&display_ctl,dev_info_ptr,&lcd_info)) &&
            (GetImgRect(img_info.width,img_info.height,image_rect_ptr,&img_rect)))
        {
            BOOLEAN is_got_rect = FALSE;

            //get display rect
            is_got_rect = GetDisplayRect(img_info.width,
                img_info.height,
                lcd_info.lcd_width,
                lcd_info.lcd_height,
                display_point_ptr,
                display_rect_ptr,
                &dis_rect);

            if (!is_got_rect)
            {
                return FALSE;
            }

            //adjust image rect and display rect
            if (AdjustImgAndDisRect(img_info.height,&img_rect,&dis_rect,img_info.img_type))
            {
                //set resource image rect
                res_src_ptr->imgrect.left   = img_rect.left;
                res_src_ptr->imgrect.top    = img_rect.top;
                res_src_ptr->imgrect.right  = img_rect.right;
                res_src_ptr->imgrect.bottom = img_rect.bottom;

                //set display rect
                display_info.dstrect.left   = dis_rect.left;
                display_info.dstrect.top    = dis_rect.top;
                display_info.dstrect.right  = dis_rect.right;
                display_info.dstrect.bottom = dis_rect.bottom;

                //set display control
                display_info.dstctl = &display_ctl;

#ifdef UI_MULTILAYER_SUPPORT
                if (IMG_RES_ARGB == img_info.img_type)
                {
#ifdef UI_MULTILAYER_SUPPORT
                    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &dis_rect, &blt_rect);
                    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr,&blt_rect);
#endif
                }
#endif

                //display
                res_result = IMG_RES_Display_Img(res_src_ptr,&display_info);
                if (IMG_RES_SUCCESS == res_result)
                {
                    result = TRUE;
                }
                else
                {
                    //SCI_TRACE_LOW:"GUIRES_DisplayImg:display image res_result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1043_112_2_18_3_20_47_264,(uint8*)"d",res_result);
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIRES_DisplayImg:get image info res_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1050_112_2_18_3_20_47_265,(uint8*)"d",res_result);
    }

    //store update rect
    MMITHEME_StoreUpdateRect(dev_info_ptr,dis_rect);

    return (result);
}

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtr(
                                      uint8                     *data_ptr,          //in:
                                      uint32                    data_size,          //in:
                                      GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                      GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                      GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                      const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                      )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if (PNULL == dev_info_ptr || PNULL == data_ptr || 0 >= data_size
        || ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)))
    {
        return result;
    }

    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = data_ptr;
        res_src.imgstreamsize = data_size;

        //display image
        result = DisplayImg(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    PNULL,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtrEx(
                                        uint8                   *data_ptr,          //in:
                                        uint32                  data_size,          //in:
                                        GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                        GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                        GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                        const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                        GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                        )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};

    if (PNULL == dev_info_ptr || PNULL == data_ptr || 0 >= data_size
        || ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr)))
    {
        return result;
    }

    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = data_ptr;
        res_src.imgstreamsize = data_size;

        //display image
        result = DisplayImg(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    display_ptr,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image rect to display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImgRect(
                         uint16         img_width,          //in
                         uint16         img_height,         //in
                         GUI_RECT_T     *in_img_rect_ptr,   //in:may PNULL代表从图片(0,0)点显示
                         GUI_RECT_T     *out_img_rect_ptr   //in/out:
                         )
{
    BOOLEAN     result = TRUE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != out_img_rect_ptr); /*assert verified*/
    if (PNULL == out_img_rect_ptr)
    {
        return FALSE;
    }
    
    if (PNULL == in_img_rect_ptr)
    {
        //display full image
        out_img_rect_ptr->left   = 0;
        out_img_rect_ptr->top    = 0;
        out_img_rect_ptr->right  = (int16)(img_width - 1);
        out_img_rect_ptr->bottom = (int16)(img_height - 1);
    }
    else
    {
        out_img_rect_ptr->left   = in_img_rect_ptr->left;
        out_img_rect_ptr->top    = in_img_rect_ptr->top;
        out_img_rect_ptr->right  = (int16)MIN(in_img_rect_ptr->right,img_width-1);
        out_img_rect_ptr->bottom = (int16)MIN(in_img_rect_ptr->bottom,img_height-1);
    }

    if ((0 > out_img_rect_ptr->left) ||
        (0 > out_img_rect_ptr->top) ||
        (out_img_rect_ptr->left > out_img_rect_ptr->right) ||
        (out_img_rect_ptr->top > out_img_rect_ptr->bottom))
    {
        result = FALSE;
        //SCI_TRACE_LOW:"GetImgRect:rect = {%d,%d,%d,%d} is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1182_112_2_18_3_20_47_266,(uint8*)"dddd",out_img_rect_ptr->left,out_img_rect_ptr->top,out_img_rect_ptr->right,out_img_rect_ptr->bottom);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image to display rect in lcd
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayRect(
                          uint16        img_width,          //in
                          uint16        img_height,         //in
                          uint16        lcd_width,          //in
                          uint16        lcd_height,         //in
                          GUI_POINT_T   *dis_point_ptr,     //in:may PNULL代表指定rect区域显示
                          GUI_RECT_T    *in_dis_rect_ptr,   //in:lcd display rect,may PNULL代表指定起始点显示
                          GUI_RECT_T    *out_dis_rect_ptr   //in/out:
                          )
{
    BOOLEAN is_rect_valid = FALSE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT((PNULL != dis_point_ptr) || (PNULL != in_dis_rect_ptr));  /*assert verified*/
    //SCI_ASSERT(PNULL  != out_dis_rect_ptr);  /*assert verified*/
    if (((PNULL == dis_point_ptr) && (PNULL == in_dis_rect_ptr)) || (PNULL == out_dis_rect_ptr))
    {
        return FALSE;
    }

    if (PNULL != dis_point_ptr)
    {
        out_dis_rect_ptr->left   = dis_point_ptr->x;
        out_dis_rect_ptr->top    = dis_point_ptr->y;
        out_dis_rect_ptr->right  = (int16)(dis_point_ptr->x + img_width - 1);
        out_dis_rect_ptr->bottom = (int16)(dis_point_ptr->y + img_height - 1);
    }
    else
    {
        out_dis_rect_ptr->left   = in_dis_rect_ptr->left;
        out_dis_rect_ptr->top    = in_dis_rect_ptr->top;
        out_dis_rect_ptr->right  = in_dis_rect_ptr->right;
        out_dis_rect_ptr->bottom = in_dis_rect_ptr->bottom;
    }

    //out_dis_rect_ptr->left   = (int16)MIN(out_dis_rect_ptr->left,(lcd_width-1));
    //out_dis_rect_ptr->top    = (int16)MIN(out_dis_rect_ptr->top,(lcd_height-1));
    out_dis_rect_ptr->right  = (int16)MIN(out_dis_rect_ptr->right,(lcd_width-1));
    out_dis_rect_ptr->bottom = (int16)MIN(out_dis_rect_ptr->bottom,(lcd_height-1));

    is_rect_valid = ((out_dis_rect_ptr->left <= out_dis_rect_ptr->right)
        && (out_dis_rect_ptr->top <= out_dis_rect_ptr->bottom));

    return is_rect_valid;
}

/*****************************************************************************/
//  Description : get display image control info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayCtl(
                            GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
                            IMG_RES_DST_CTL_T       *display_ctl_ptr,   //in/out
                            const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                            LCD_INFO_T              *lcd_info_ptr       //in/out
                            )
{
    BOOLEAN             result = FALSE;
    GUILCD_DRAW_DC_T    draw_dc = {0};

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != display_ctl_ptr);   /*assert verified*/
    //SCI_ASSERT(PNULL != dev_info_ptr);      /*assert verified*/
    //SCI_ASSERT(PNULL != lcd_info_ptr);      /*assert verified*/
    if ((PNULL == display_ctl_ptr)||(PNULL == dev_info_ptr)||(PNULL == lcd_info_ptr))
    {
        return FALSE;
    }

    //get lcd info
    if (GUILCD_GetInfo((LCD_ID_E)dev_info_ptr->lcd_id,lcd_info_ptr))
    {
        if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
        {
            return result;
        }
        
        lcd_info_ptr->lcd_width = draw_dc.width;
        display_ctl_ptr->height = lcd_info_ptr->lcd_height = draw_dc.height;
        display_ctl_ptr->width = draw_dc.mem_width;
        display_ctl_ptr->dstmem = draw_dc.lcd_buffer_ptr;
        //set display control info
        if (PNULL == display_ptr)
        {
            switch (draw_dc.data_type)
            {
            case DATA_TYPE_ARGB888:
                display_ctl_ptr->format  = IMGREF_FORMAT_ARGB888;

                if ( !GUI_IsAlphaOverwrite() )
                {
                    display_ctl_ptr->bg_type = IMG_BG_COMPOUND;
                }
                break;

            case DATA_TYPE_RGB565:
            default:
                display_ctl_ptr->format  = IMGREF_FORMAT_RGB565;
                break;
            }  
        }
        else
        {
            display_ctl_ptr->format  = display_ptr->format;
            display_ctl_ptr->bg_type = display_ptr->bg_type;
        }
        if(PNULL != display_ctl_ptr->dstmem)
        {
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"GetDisplayCtl: display block buffer is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1305_112_2_18_3_20_47_267,(uint8*)"");
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust image rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: image rect, display rect宽高必须相等,而且是二者最小值
/*****************************************************************************/
LOCAL BOOLEAN AdjustImgAndDisRect(
                                  uint16           img_height,     //in
                                  GUI_RECT_T       *img_rect_ptr,  //in
                                  GUI_RECT_T       *dis_rect_ptr,  //in
                                  IMG_RES_TYPE_E   img_type        //in
                                  )
{
    BOOLEAN     result = TRUE;
    uint16      img_dis_width = 0;
    uint16      img_dis_height = 0;
    uint16      bmp_top = 0;
    uint16      bmp_bottom = 0;

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != img_rect_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != dis_rect_ptr);  /*assert verified*/
    if ((PNULL == img_rect_ptr)||(PNULL == dis_rect_ptr))
    {
        return FALSE;
    }
    
    //image rect, display rect宽高必须相等,而且是二者最小值
    img_dis_width  = (uint16)MIN((img_rect_ptr->right-img_rect_ptr->left+1),(dis_rect_ptr->right-dis_rect_ptr->left+1));
    img_dis_height = (uint16)MIN((img_rect_ptr->bottom-img_rect_ptr->top+1),(dis_rect_ptr->bottom-dis_rect_ptr->top+1));

    //set image rect
    img_rect_ptr->right  = (int16)(img_rect_ptr->left + img_dis_width - 1);
    img_rect_ptr->bottom = (int16)(img_rect_ptr->top + img_dis_height - 1);

    //set display rect
    dis_rect_ptr->right  = (int16)(dis_rect_ptr->left + img_dis_width - 1);
    dis_rect_ptr->bottom = (int16)(dis_rect_ptr->top + img_dis_height - 1);

    //adjust display rect top and left
    if (0 > dis_rect_ptr->left)
    {
        img_rect_ptr->left = (int16)(img_rect_ptr->left - dis_rect_ptr->left);
        dis_rect_ptr->left = 0;
    }
    if (0 > dis_rect_ptr->top)
    {
        img_rect_ptr->top = (int16)(img_rect_ptr->top - dis_rect_ptr->top);
        dis_rect_ptr->top = 0;
    }

    //if is bmp,bmp的存放方式是倒序的,必须将top和bottom对换.
    if (IMG_RES_BMP == img_type)
    {
        bmp_top    = (uint16)(img_height - 1 - img_rect_ptr->bottom);
        bmp_bottom = (uint16)(img_height - 1 - img_rect_ptr->top);
        
        img_rect_ptr->top = bmp_top;
        img_rect_ptr->bottom = bmp_bottom;
    }

    //judge display rect
    if ((dis_rect_ptr->left > dis_rect_ptr->right) ||
        (dis_rect_ptr->top > dis_rect_ptr->bottom))
    {
        //SCI_TRACE_LOW:"AdjustImgAndDisRect:rect = {%d,%d,%d,%d} is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1376_112_2_18_3_20_48_268,(uint8*)"dddd",dis_rect_ptr->left,dis_rect_ptr->top,dis_rect_ptr->right,dis_rect_ptr->bottom);
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim width and height by anim id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeight(
                                         uint16         *width_ptr,     //in/out:may PNULL
                                         uint16         *height_ptr,    //in/out:may PNULL
                                         uint16         *frame_num_ptr, //in/out:may PNULL
                                         MMI_ANIM_ID_T  anim_id,        //in
                                         MMI_WIN_ID_T   win_id          //in
                                         )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_SRC_T    img_src = {0};

    if ((PNULL == width_ptr) && (PNULL == height_ptr) && (PNULL == frame_num_ptr))
    {
        return result;
    }

    if ( MMITHEME_CheckImageID(anim_id) )
    {  
        //get resource pointer and size
        img_src.data_ptr = (uint8 *)MMI_GetLabelAnim(anim_id,win_id,&(img_src.data_size));

        if (PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            //get image info
            result = GetImgInfo(width_ptr,height_ptr,frame_num_ptr,&img_src);
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIRES_GetAnimWidthHeight == error when MMI_GetLabelAnim"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1415_112_2_18_3_20_48_269,(uint8*)"");
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim width and height by anim data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeightByPtr(
                                              uint8          *data_ptr,      //in:may PNULL
                                              uint16         *width_ptr,     //in/out:may PNULL
                                              uint16         *height_ptr,    //in/out:may PNULL
                                              uint16         *frame_num_ptr, //in/out:may PNULL
                                              uint32         data_size       //in:
                                              )
{
    BOOLEAN             result = FALSE;
    GUIRES_IMG_SRC_T    img_src = {0};

    if (PNULL == data_ptr || 0 >= data_size
        || (PNULL == width_ptr && PNULL == height_ptr && PNULL == frame_num_ptr))
    {
        return result;
    }

    img_src.data_ptr = data_ptr;
    img_src.data_size = data_size;

    //get image info
    result = GetImgInfo(width_ptr,height_ptr,frame_num_ptr,&img_src);

    return (result);
}

/*****************************************************************************/
//  Description : display resource anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnim(
                                  uint16                    frame_index,        //in:from 0
                                  GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                  GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                  GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                  MMI_WIN_ID_T              win_id,             //in:win id
                                  MMI_ANIM_ID_T             anim_id,            //in:anim id
                                  const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                  )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (PNULL == dev_info_ptr
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        return result;
    }

    if (MMITHEME_IsBacklightOn() && MMITHEME_CheckImageID(anim_id))
    {
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelAnim(anim_id,win_id,&(res_src.imgstreamsize));
        if (PNULL == res_src.imgstream_ptr || 0 >= res_src.imgstreamsize)
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplayAnim == error when MMI_GetLabelAnim"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1482_112_2_18_3_20_48_270,(uint8*)"");
            return result;
        }

        //set anim display info
        anim_dis.frame_index = frame_index;

        //display anim
        result = DisplayAnim(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    &anim_dis,
                    PNULL,
                    PNULL,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimEx(
                                    uint16                  frame_index,        //in:from 0
                                    GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                    GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                    MMI_WIN_ID_T            win_id,             //in:win id
                                    MMI_ANIM_ID_T           anim_id,            //in:anim id
                                    const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                    GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                    )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (PNULL == dev_info_ptr 
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        return result;
    }

    if (MMITHEME_IsBacklightOn() && MMITHEME_CheckImageID(anim_id))
    {
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelAnim(anim_id,win_id,&(res_src.imgstreamsize));
        if (PNULL == res_src.imgstream_ptr || 0 >= res_src.imgstreamsize)
        {
            //SCI_TRACE_LOW:"== GUIRES_DisplayAnimEx == error when MMI_GetLabelAnim"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1534_112_2_18_3_20_48_271,(uint8*)"");
            return result;
        }

        //set anim display info
        anim_dis.frame_index = frame_index;

        //display anim
        result = DisplayAnim(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    &anim_dis,
                    PNULL,
                    display_ptr,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAnim(
                          GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                          GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                          GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                          IMG_RES_SRC_T             *res_src_ptr,       //in:
                          GUIRES_ANIM_DIS_T         *anim_dis_ptr,      //in:
                          GUIRES_ANIM_INFO_T        *anim_info_ptr,     //in/out:may PNULL
                          GUIRES_DISPLAY_T          *display_ptr,       //in:may PNULL,default
                          const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                          )
{
    BOOLEAN                     result = FALSE;
    uint32                      handle = 0;
    GUI_RECT_T                  img_rect = {0};
    GUI_RECT_T                  dis_rect = {0};
    IMG_RES_INFO_T              img_info = {0};
    IMG_RES_DST_CTL_T           display_ctl = {0};
    IMG_RES_DST_T               display_info = {0};
    IMG_RES_ERROR_E             res_result = IMG_RES_SUCCESS;
    LCD_INFO_T                  lcd_info = {0};
    IMG_RES_MOV_FRAME_T         anim_info = {0};
    IMG_RES_DST_MOV_INFO_T      anim_dis_info = {0};
    GUI_RECT_T                  display_rect = {0};
    GUI_POINT_T                 display_point = {0};    

    if (PNULL == dev_info_ptr 
        || PNULL == anim_dis_ptr 
        || PNULL == res_src_ptr 
        || PNULL == res_src_ptr->imgstream_ptr
        || 0 >= res_src_ptr->imgstreamsize
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        return result;
    }

    //get anim info
    res_result = IMG_RES_GetImageInfo(res_src_ptr,&img_info);
    if (IMG_RES_SUCCESS == res_result)
    {
        display_point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, display_point_ptr, &display_point);
        display_rect_ptr  = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, display_rect_ptr, &display_rect);
        
        //get display control info
        if ((GetDisplayCtl(display_ptr,&display_ctl,dev_info_ptr,&lcd_info)) && 
            (GetImgRect(img_info.width,img_info.height,image_rect_ptr,&img_rect)))
        {
            BOOLEAN is_got_rect = FALSE;
            
            //get display rect
            is_got_rect = GetDisplayRect(img_info.width,
                img_info.height,
                lcd_info.lcd_width,
                lcd_info.lcd_height,
                display_point_ptr,
                display_rect_ptr,
                &dis_rect);

            if (!is_got_rect)
            {
                return FALSE;
            }

            //adjust image rect and display rect
            if (AdjustImgAndDisRect(img_info.height,&img_rect,&dis_rect,IMG_RES_MOV_ARGB))
            {
                //set resource image rect
                res_src_ptr->imgrect.left   = img_rect.left;
                res_src_ptr->imgrect.top    = img_rect.top;
                res_src_ptr->imgrect.right  = img_rect.right;
                res_src_ptr->imgrect.bottom = img_rect.bottom;

                //set display rect
                display_info.dstrect.left   = dis_rect.left;
                display_info.dstrect.top    = dis_rect.top;
                display_info.dstrect.right  = dis_rect.right;
                display_info.dstrect.bottom = dis_rect.bottom;

                //set display control
                display_info.dstctl = &display_ctl;

                if (anim_dis_ptr->is_dis_ordinal)
                {
                    //handle has creat
                    handle = anim_dis_ptr->anim_handle;
                    res_result = IMG_RES_SUCCESS;
                }
                else
                {
                    //creat handle
                    res_result = IMG_RES_Create_Handler(res_src_ptr,&handle);
                }

                if (IMG_RES_SUCCESS == res_result)
                {
                    // anim_dis_ptr->is_dis_ordinal为
                    // TRUE时，需保证anim_dis_ptr->anim_handle值有效
                    // FALSE时，IMG_RES_Create_Handler必须成功
                    SCI_ASSERT(0 != handle);    /*assert verified*/

                    //set display info
                    anim_dis_info.handler        = handle;
                    anim_dis_info.img_dst        = display_info;
                    anim_dis_info.frame_index    = anim_dis_ptr->frame_index;
                    anim_dis_info.is_decode_cur  = (BOOLEAN)(!anim_dis_ptr->is_next_frame);

                    anim_dis_info.imgrect.left   = img_rect.left;
                    anim_dis_info.imgrect.right  = img_rect.right;
                    anim_dis_info.imgrect.top    = img_rect.top;
                    anim_dis_info.imgrect.bottom = img_rect.bottom;

                    if ((PNULL != display_ptr) &&
                        (display_ptr->is_exchange_color))
                    {
                        anim_dis_info.color_type  = IMG_COLOR_EXCHANGE;
                        anim_dis_info.color_value = display_ptr->exchange_color;
                    }

                    //display frame
                    res_result = IMG_RES_Display_MOV(&anim_dis_info,&anim_info);
                    if (IMG_RES_SUCCESS == res_result)
                    {
                        if (PNULL != anim_info_ptr)
                        {
                            anim_info_ptr->is_decode_all = anim_info.is_decode_finish;
                            SCI_ASSERT(0 < anim_info.frame_index);  /*assert verified*/
                            anim_info_ptr->cur_frame  = (uint16)(anim_info.frame_index - 1);
                            anim_info_ptr->delay_time = anim_info.interval;
                        }
                        result = TRUE;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"DisplayAnim:display anim res_result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1684_112_2_18_3_20_48_272,(uint8*)"d",res_result);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"DisplayAnim:creat handle res_result = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1689_112_2_18_3_20_48_273,(uint8*)"d",res_result);
                }
        
                //ordinal display movie,outside free handle
                if (!anim_dis_ptr->is_dis_ordinal && 0 != handle)
                {
                    //free handle
                    res_result = IMG_RES_Free_Handler(handle);
                    if (IMG_RES_SUCCESS != res_result)
                    {
                        //SCI_TRACE_LOW:"DisplayAnim:free handle res_result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1699_112_2_18_3_20_48_274,(uint8*)"d",res_result);
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"DisplayAnim:get anim info res_result = %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1707_112_2_18_3_20_48_275,(uint8*)"d",res_result);
    }

    //store update rect
    MMITHEME_StoreUpdateRect(dev_info_ptr,dis_rect);

    return (result);
}

/*****************************************************************************/
//  Description : display resource anim by data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtr(
                                       uint8                     *data_ptr,          //in:
                                       uint16                    frame_index,        //in:from 0
                                       uint32                    data_size,          //in:
                                       GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                       GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                       GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                       const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                       )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (PNULL == dev_info_ptr || PNULL == data_ptr || 0 >= data_size
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        return result;
    }
    
    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = data_ptr;
        res_src.imgstreamsize = data_size;

        //set anim display info
        anim_dis.frame_index = frame_index;

        //display anim
        result = DisplayAnim(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    &anim_dis,
                    PNULL,
                    PNULL,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display resource anim by data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtrEx(
                                         uint8                  *data_ptr,          //in:
                                         uint16                 frame_index,        //in:from 0
                                         uint32                 data_size,          //in:
                                         GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                         GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                         GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,      //in:
                                         GUIRES_DISPLAY_T       *display_ptr        //in:display info
                                         )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (PNULL == dev_info_ptr || PNULL == data_ptr || 0 >= data_size
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        return result;
    }
    
    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = data_ptr;
        res_src.imgstreamsize = data_size;

        //set anim display info
        anim_dis.frame_index = frame_index;

        //display anim
        result = DisplayAnim(display_point_ptr,
                    display_rect_ptr,
                    image_rect_ptr,
                    &res_src,
                    &anim_dis,
                    PNULL,
                    display_ptr,
                    dev_info_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : copy image buffer to the pointed buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBuf(
                                 const uint8   *src_data_ptr,  //in:src data buffer
                                 uint16        dest_width,     //in:
                                 uint16        dest_height,    //in:
                                 uint32        dest_buf_size,  //in:
                                 uint32        src_data_size,  //in:
                                 GUI_POINT_T   *src_point_ptr, //in:may PNULL代表指定rect区域
                                 GUI_RECT_T    *src_rect_ptr,  //in:may PNULL代表指定起始点
                                 GUI_COLOR_T   *dest_buf_ptr   //out:dest buffer
                                 )
{
    BOOLEAN     result = FALSE;

    result = CopyImgBuf(src_data_ptr,
                dest_width,
                dest_height,
                dest_buf_size,
                src_data_size,
                src_point_ptr,
                src_rect_ptr,
                dest_buf_ptr,
                PNULL);

    return (result);
}

/*****************************************************************************/
//  Description : copy image buffer to the pointed buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBufEx(
                                   const uint8          *src_data_ptr,  //in:src data buffer
                                   uint16               dest_width,     //in:
                                   uint16               dest_height,    //in:
                                   uint32               dest_buf_size,  //in:
                                   uint32               src_data_size,  //in:
                                   GUI_POINT_T          *src_point_ptr, //in:may PNULL代表指定rect区域
                                   GUI_RECT_T           *src_rect_ptr,  //in:may PNULL代表指定起始点
                                   void                 *dest_buf_ptr,  //out:dest buffer,888必须32bit
                                   GUIRES_DISPLAY_T     *display_ptr    //in:display info
                                   )
{
    BOOLEAN     result = FALSE;

    result = CopyImgBuf(src_data_ptr,
                dest_width,
                dest_height,
                dest_buf_size,
                src_data_size,
                src_point_ptr,
                src_rect_ptr,
                dest_buf_ptr,
                display_ptr);

    return (result);
}


/*****************************************************************************/
//  Description : copy image buffer to the pointed buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyImgBuf(
                         const uint8        *src_data_ptr,  //in:src data buffer
                         uint16             dest_width,     //in:
                         uint16             dest_height,    //in:
                         uint32             dest_buf_size,  //in:
                         uint32             src_data_size,  //in:
                         GUI_POINT_T        *src_point_ptr, //in:may PNULL代表指定rect区域
                         GUI_RECT_T         *src_rect_ptr,  //in:may PNULL代表指定起始点
                         GUI_COLOR_T        *dest_buf_ptr,  //out:dest buffer
                         GUIRES_DISPLAY_T   *display_ptr    //in:display info,may PNULL
                         )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          src_rect = {0};
    GUI_RECT_T          dest_rect = {0};
    IMG_RES_DST_CTL_T   display_ctl = {0};
    IMG_RES_SRC_T       res_src = {0};
    IMG_RES_INFO_T      img_info = {0};
    IMG_RES_DST_T       display_info = {0};
    LCD_INFO_T          lcd_info = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;

    if (PNULL == src_data_ptr
        || (PNULL == src_point_ptr && PNULL == src_rect_ptr)
        || dest_buf_size < (dest_width * dest_height))
    {
        return result;
    }

    //get sabm image info first
    if ( GetSabmImgInfo( src_data_ptr, src_data_size, PNULL ) )
    {
        return MMI_DisplaySabmImg( MMI_BUF_RGB565, src_data_ptr, src_data_size, src_rect_ptr, PNULL, PNULL,
            dest_width, dest_height, (void*)dest_buf_ptr );
    }
    //get graph data info second
    else if ( GetGraphDataInfo( src_data_ptr, src_data_size, PNULL ) )
    {
        return MMI_DisplaySabmImg( MMI_BUF_RGB565, src_data_ptr, src_data_size, src_rect_ptr, PNULL, PNULL,
            dest_width, dest_height, (void*)dest_buf_ptr );
    }

    //get image info
    res_src.imgstream_ptr = (uint8 *)src_data_ptr;
    res_src.imgstreamsize = src_data_size;
    res_result = IMG_RES_GetImageInfo(&res_src,&img_info);
    if (IMG_RES_SUCCESS == res_result)
    {
        //get display control info
        if (GetDisplayCtl(display_ptr,&display_ctl,&lcd_dev_info,&lcd_info))
        {
            //set width, height and dest buffer
            lcd_info.lcd_width = dest_width;
            lcd_info.lcd_height = dest_height;

            display_ctl.width  = dest_width;
            display_ctl.height = dest_height;
            display_ctl.dstmem = (uint8*)dest_buf_ptr;

            //get src rect
            if (GetImgRect(img_info.width,img_info.height,src_rect_ptr,&src_rect))
            {
                BOOLEAN is_got_rect = FALSE;
                
                //get dest rect
                is_got_rect = GetDisplayRect(img_info.width,
                    img_info.height,
                    lcd_info.lcd_width,
                    lcd_info.lcd_height,
                    src_point_ptr,
                    src_rect_ptr,
                    &dest_rect);

                if (!is_got_rect)
                {
                    return FALSE;
                }

                //adjust src rect and dest rect
                if (AdjustImgAndDisRect(img_info.height,&src_rect,&dest_rect,img_info.img_type))
                {
                    //set src rect
                    res_src.imgrect.left   = src_rect.left;
                    res_src.imgrect.top    = src_rect.top;
                    res_src.imgrect.right  = src_rect.right;
                    res_src.imgrect.bottom = src_rect.bottom;

                    //set dest rect
                    display_info.dstrect.left   = dest_rect.left;
                    display_info.dstrect.top    = dest_rect.top;
                    display_info.dstrect.right  = dest_rect.right;
                    display_info.dstrect.bottom = dest_rect.bottom;

                    //set display control
                    display_info.dstctl = &display_ctl;

                    //copy buffer
                    res_result = IMG_RES_Display_Img(&res_src,&display_info);
                    if (IMG_RES_SUCCESS == res_result)
                    {
                        result = TRUE;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"GUIRES_CopyImgBuf:display image res_result = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1982_112_2_18_3_20_49_276,(uint8*)"d",res_result);
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIRES_CopyImgBuf:get image info res_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_1990_112_2_18_3_20_49_277,(uint8*)"d",res_result);
    }

    return (result);
}

/*****************************************************************************/
//  Description : creat display anim or image handle before display
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_CreatHandle(
                                  uint32            *handle_ptr,//in/out:
                                  MMI_WIN_ID_T      win_id,     //in:win id
                                  MMI_IMAGE_ID_T    img_id,      //in:image id
                                  GUI_LCD_DEV_INFO  *dev_info_ptr
                                  )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;

    if (PNULL == handle_ptr)
    {
        return result;
    }

    //get resource pointer and size
    res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
    if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
    {
        //creat handle
        res_result = IMG_RES_Create_Handler(&res_src,handle_ptr);
        if (IMG_RES_SUCCESS == res_result)
        {
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"GUIRES_CreatHandle:creat handle res_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2028_112_2_18_3_20_49_278,(uint8*)"d",res_result);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : free display anim or image handle after display
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_FreeHandle(
                                 uint32     handle//in/out:
                                 )
{
    BOOLEAN             result = FALSE;
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;

    //free handle
    if (0 != handle)
    {
        res_result = IMG_RES_Free_Handler(handle);
        if (IMG_RES_SUCCESS == res_result)
        {
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"GUIRES_FreeHandle:free handle res_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2056_112_2_18_3_20_49_279,(uint8*)"d",res_result);
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display gif anim or image
//  Global resource dependence : 
//  Author: Jassmine
//  Note: Gif动画只能逐帧显示,不能指定当前帧
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGif(
                                 BOOLEAN                is_next_frame,      //in:
                                 uint32                 handle,             //in:
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 GUIRES_ANIM_INFO_T     *gif_info_ptr,      //in/out:may PNULL
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (0 == handle || PNULL == dev_info_ptr
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        //SCI_TRACE_LOW:"== GUIRES_DisplayGif == param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2090_112_2_18_3_20_49_280,(uint8*)"");
        return result;
    }

    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //set anim display info
            anim_dis.is_dis_ordinal = TRUE;
            anim_dis.is_next_frame  = is_next_frame;
            anim_dis.anim_handle    = handle;

            //display anim
            result = DisplayAnim(display_point_ptr,
                        display_rect_ptr,
                        image_rect_ptr,
                        &res_src,
                        &anim_dis,
                        gif_info_ptr,
                        PNULL,
                        dev_info_ptr);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : display gif anim or image
//  Global resource dependence : 
//  Author: Jassmine
//  Note: Gif动画只能逐帧显示,不能指定当前帧
/*****************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGifEx(
                                   BOOLEAN                  is_next_frame,      //in:
                                   uint32                   handle,             //in:
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   GUIRES_ANIM_INFO_T       *gif_info_ptr,      //in/out:may PNULL
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr    //in:display info
                                   )
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    GUIRES_ANIM_DIS_T   anim_dis = {0};

    if (0 == handle || PNULL == dev_info_ptr
        || (PNULL == display_point_ptr && PNULL == display_rect_ptr))
    {
        //SCI_TRACE_LOW:"== GUIRES_DisplayGifEx == param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2144_112_2_18_3_20_49_281,(uint8*)"");
        return result;
    }

    if (MMITHEME_IsBacklightOn())
    {
        //get resource pointer and size
        res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));
        if (PNULL != res_src.imgstream_ptr && 0 < res_src.imgstreamsize)
        {
            //set anim display info
            anim_dis.is_dis_ordinal = TRUE;
            anim_dis.is_next_frame  = is_next_frame;
            anim_dis.anim_handle    = handle;

            //display anim
            result = DisplayAnim(display_point_ptr,
                        display_rect_ptr,
                        image_rect_ptr,
                        &res_src,
                        &anim_dis,
                        gif_info_ptr,
                        display_ptr,
                        dev_info_ptr);
        }
    }
    
    return (result);
}

/*lint +e775*/

/*==============================================================================
Description: GUIRES_DisplayImgDst
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 将数据写到res dst ctrl
==============================================================================*/
PUBLIC BOOLEAN GUIRES_DisplayImgDst(
    GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
    GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    MMI_WIN_ID_T           win_id,
    MMI_IMAGE_ID_T         img_id,
    GUILCD_DRAW_DC_T const *dc_data_ptr
)
{
    BOOLEAN             result = FALSE;
    IMG_RES_SRC_T       res_src = {0};
    IMG_RES_DST_CTL_T   res_dst = {0};

    if ((PNULL == display_point_ptr) && (PNULL == display_rect_ptr))
    {
        return FALSE;
    }

    if (!MMITHEME_IsBacklightOn())
    {
        return FALSE;
    }

    if (!MMITHEME_CheckImageID(img_id))
    {
        return FALSE;
    }

    MMI_TraceAutoTestOfImage(img_id);
        
    //get resource pointer and size
    res_src.imgstream_ptr = (uint8 *)MMI_GetLabelImage(img_id,win_id,&(res_src.imgstreamsize));

    if (PNULL == res_src.imgstream_ptr || res_src.imgstreamsize == 0)
    {
        return FALSE;
    }

    if (DATA_TYPE_ARGB888 == dc_data_ptr->data_type)
    {
        res_dst.format = IMGREF_FORMAT_ARGB888;

        if ( !GUI_IsAlphaOverwrite() )
        {
            res_dst.bg_type = IMG_BG_COMPOUND;
        }
    }
    else
    {
        res_dst.format = IMGREF_FORMAT_RGB565;
    }
    
    res_dst.width = dc_data_ptr->width;
    res_dst.height = dc_data_ptr->height;
    res_dst.dstmem = dc_data_ptr->lcd_buffer_ptr;

    //display image
    result = DisplayImgDst(display_point_ptr,
                display_rect_ptr,
                image_rect_ptr,
                &res_src,
                PNULL,
                &res_dst);        

    return (result);
}


/*==============================================================================
Description: DisplayImgDst
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 将数据写到res dst ctrl
==============================================================================*/
LOCAL BOOLEAN DisplayImgDst(
    GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
    GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    IMG_RES_SRC_T           *res_src_ptr,       //in:
    GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
    IMG_RES_DST_CTL_T const *dst_ptr
)
{
    IMG_RES_ERROR_E     res_result = IMG_RES_SUCCESS;
    IMG_RES_INFO_T      img_info = {0};
    BOOLEAN             has_get_img_rect = FALSE;
    GUI_RECT_T          img_rect = {0};
    BOOLEAN             is_rect_invalid = FALSE;
    IMG_RES_ERROR_E     disp_result = IMG_RES_SUCCESS;
    GUI_RECT_T          dis_rect = {0};
    IMG_RES_DST_T       display_info = {0};
    BOOLEAN             is_got_rect = FALSE;

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT((PNULL != display_point_ptr) || (PNULL != display_rect_ptr)); /*assert verified*/
    //SCI_ASSERT(PNULL != res_src_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != dst_ptr); /*assert verified*/
    if (((PNULL == display_point_ptr)&&(PNULL == display_rect_ptr))||(PNULL == res_src_ptr)||(PNULL == dst_ptr))
    {
        return FALSE;
    }

    if ( GetSabmImgInfo(res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, PNULL) )
    {
        return DisplaySabmImgDst(display_rect_ptr, image_rect_ptr, res_src_ptr, display_ptr, dst_ptr);
    }
    //get graph data info second
    else if ( GetGraphDataInfo(res_src_ptr->imgstream_ptr, res_src_ptr->imgstreamsize, PNULL) )
    {
        return DisplayGraphDataDst(display_rect_ptr, image_rect_ptr, res_src_ptr, display_ptr, dst_ptr);
    }

    res_result = IMG_RES_GetImageInfo(res_src_ptr,&img_info);

    if (IMG_RES_SUCCESS != res_result)
    {
        //SCI_TRACE_LOW:"GUIRES_DisplayImg:get image info res_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2296_112_2_18_3_20_49_282,(uint8*)"d",res_result);
        return FALSE;
    }

    has_get_img_rect = GetImgRect(img_info.width,img_info.height,image_rect_ptr,&img_rect);

    if (!has_get_img_rect)
    {
        return FALSE;
    }

    //get display rect
    is_got_rect = GetDisplayRect(img_info.width,
                                 img_info.height,
                                 dst_ptr->width,
                                 dst_ptr->height,
                                 display_point_ptr,
                                 display_rect_ptr,
                                 &dis_rect);

    if (!is_got_rect)
    {
        return FALSE;
    }

    is_rect_invalid = AdjustImgAndDisRect(img_info.height,&img_rect,&dis_rect,img_info.img_type);

    if (!is_rect_invalid)
    {
        return FALSE;
    }

    //set resource image rect
    res_src_ptr->imgrect.left   = img_rect.left;
    res_src_ptr->imgrect.top    = img_rect.top;
    res_src_ptr->imgrect.right  = img_rect.right;
    res_src_ptr->imgrect.bottom = img_rect.bottom;

    //set display rect
    display_info.dstrect.left   = dis_rect.left;
    display_info.dstrect.top    = dis_rect.top;
    display_info.dstrect.right  = dis_rect.right;
    display_info.dstrect.bottom = dis_rect.bottom;

    //set display control
    display_info.dstctl = (IMG_RES_DST_CTL_T*)dst_ptr;

    //display
    disp_result = IMG_RES_Display_Img(res_src_ptr,&display_info);

    if (IMG_RES_SUCCESS != disp_result)
    {
        //SCI_TRACE_LOW:"GUIRES_DisplayImg:display image res_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRES_IMG_2348_112_2_18_3_20_49_283,(uint8*)"d",res_result);
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: DisplaySabmImg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 将数据写到res dst ctrl
==============================================================================*/
LOCAL BOOLEAN DisplaySabmImgDst(
    GUI_RECT_T              *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    IMG_RES_SRC_T           *res_src_ptr,       //in:
    GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
    IMG_RES_DST_CTL_T const *dst_ptr
)
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          dis_rect = {0};
    BOOLEAN             is_got_rect = FALSE;

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != src_rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != res_src_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != dst_ptr); /*assert verified*/
    if ((PNULL == src_rect_ptr)||(PNULL == res_src_ptr)||(PNULL == dst_ptr))
    {
        return FALSE;
    }
    
    //get display rect
    is_got_rect = GetDisplayRect(src_rect_ptr->right - src_rect_ptr->left + 1,
                                 src_rect_ptr->bottom - src_rect_ptr->top + 1,
                                 dst_ptr->width,
                                 dst_ptr->height,
                                 PNULL,
                                 src_rect_ptr,
                                 &dis_rect);

    if (!is_got_rect)
    {
        return FALSE;
    }
                
    result = MMI_DisplaySabmImg((IMGREF_FORMAT_ARGB888 == dst_ptr->format) ? MMI_BUF_ARGB888 : MMI_BUF_RGB565,
                                 res_src_ptr->imgstream_ptr,
                                 res_src_ptr->imgstreamsize, 
                                 &dis_rect,
                                 &dis_rect,
                                 image_rect_ptr,
                                 dst_ptr->width,
                                 dst_ptr->height,
                                 dst_ptr->dstmem);

    return result;
}


/*==============================================================================
Description: DisplayGraphDataDst
Global resource dependence: NONE
Author: James.Zhang
Note: 将数据写到res dst ctrl
==============================================================================*/
LOCAL BOOLEAN DisplayGraphDataDst(
                                  GUI_RECT_T              *src_rect_ptr,      //in:lcd display rect,may PNULL代表指定起始点显示
                                  GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                  IMG_RES_SRC_T           *res_src_ptr,       //in:
                                  GUIRES_DISPLAY_T        *display_ptr,       //in:may PNULL,default
                                  IMG_RES_DST_CTL_T const *dst_ptr
                                  )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          dis_rect = {0};
    BOOLEAN             is_got_rect = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != src_rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != res_src_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != dst_ptr); /*assert verified*/
    if ((PNULL == src_rect_ptr)||(PNULL == res_src_ptr)||(PNULL == dst_ptr))
    {
        return FALSE;
    }
    
    //get display rect
    is_got_rect = GetDisplayRect(src_rect_ptr->right - src_rect_ptr->left + 1,
                                 src_rect_ptr->bottom - src_rect_ptr->top + 1,
                                 dst_ptr->width,
                                 dst_ptr->height,
                                 PNULL,
                                 src_rect_ptr,
                                 &dis_rect);

    if (!is_got_rect)
    {
        return FALSE;
    }
                
    result = MMI_DisplayGraphData((IMGREF_FORMAT_ARGB888 == dst_ptr->format) ? MMI_BUF_ARGB888 : MMI_BUF_RGB565,
                                 res_src_ptr->imgstream_ptr,
                                 res_src_ptr->imgstreamsize, 
                                 &dis_rect,
                                 image_rect_ptr,
                                 dst_ptr->width,
                                 dst_ptr->height,
                                 dst_ptr->dstmem);

    return result;
}


#endif
