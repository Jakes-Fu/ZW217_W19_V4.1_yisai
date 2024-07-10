/****************************************************************************
** File Name:      mmi_utility.c                                           *
** Author:         James.Zhang                                             *
** Date:           01/15/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        James.Zhang       Create
** 
****************************************************************************/
#ifndef MMK_UTILITY_C
#define MMK_UTILITY_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_utility.h"
#include "guicommon.h"
#include "graphics_draw.h"
#include "mmitheme_pos.h"
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 /*Dither data for select*/
 LOCAL const uint16 s_dither_matrix_3bit_16[4] = {
		0x5140, 0x3726, 0x4051, 0x2637
	};
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//sabm flag
#define SABM_TILE           (0x1)    //平铺
#define SABM_HOR_STRETCH  	(0x2)    //水平拉伸
#define SABM_VER_STRETCH  	(0x4)    //垂直拉伸
#define SABM_CEN_STRETCH  	(0x8)    //中间拉伸

#define SABM_HEAD_SIZE      12

#define RGB_888_TO_565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | (((b) >> 3) & 0x1f))
#define SABM_U16(p)    SABMU16((const uint8*)(p))
#define SABM_U32(p)    SABMU32((const uint8*)(p))


#define DITHER_565_SCAN(y)  ( s_dither_matrix_3bit_16[(y) & 3])
#define DITHER_VALUE(x,y) ((( s_dither_matrix_3bit_16[(y) & 3]) >> (((x) & 3) << 2)) & 0xF)

#define SPRD_R16_BITS     5
#define SPRD_G16_BITS     6
#define SPRD_B16_BITS     5
#define SPRD_R32_BITS     8
#define SPRD_G32_BITS     8
#define SPRD_B32_BITS     8

#define SPRD_R16_SHIFT    (SPRD_B16_BITS + SPRD_G16_BITS)
#define SPRD_G16_SHIFT    (SPRD_B16_BITS)
#define SPRD_B16_SHIFT    0
#define HARRY_MODIFY

#define DITHER_R32_FOR_565(r, d)  (r + d - (r >> 5))
#define DITHER_G32_FOR_565(g, d)  (g + (d >> 1) - (g >> 6))
#define DITHER_B32_FOR_565(b, d)  (b + d - (b >> 5))

#define SPRD_R32ToR16(r)   ((uint32)(r) >> (SPRD_R32_BITS - SPRD_R16_BITS))
#define SPRD_G32ToG16(g)   ((uint32)(g) >> (SPRD_G32_BITS - SPRD_G16_BITS))
#define SPRD_B32ToB16(b)   ((uint32)(b) >> (SPRD_B32_BITS - SPRD_B16_BITS))

#if 0
#define ALPHA_BLEND( dst_ptr, argb_ptr )\
{\
    uint8 alpha = argb_ptr[3];\
    uint32 result = 0;\
    if (0 == alpha)\
    {\
        (dst_ptr)++;\
    }\
    else\
    {\
        uint8 src_r = argb_ptr[2];\
        uint8 src_g = argb_ptr[1];\
        uint8 src_b = argb_ptr[0];\
        result = RGB_888_TO_565(src_r, src_g, src_b);\
        if (0xFF == alpha)\
        {\
            *(dst_ptr)++ = result;\
        }\
        else\
        {\
            uint8  n = (alpha + 1) >> 3;\
            uint32 x = result;\
            uint32 y = *(dst_ptr);\
            x = ( x | (x << 16)) & 0x7E0F81F;\
            y = ( y | (y << 16)) & 0x7E0F81F;\
            result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;\
            *(dst_ptr)++ = ((result & 0xFFFF) | (result >> 16));\
        }\
    }\
}
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct  
{
    void (*DrawHorline)( void *dst_ptr, const uint8* argb_ptr,uint16 left,uint16 right,int32 row);
    void (*DrawVerline)( void *dst_ptr, uint16 dst_width, const uint8* argb_ptr, uint16 top, uint16 bottom ,uint16 col);
    void (*DrawPoint)  ( void *dst_ptr, const uint8* argb_ptr ,uint16 col,uint16 row);
    uint8 bits_per_pixel;

}SABM_FUNC_T;
/*****************************************************************************/
//  Description : get color value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
extern BOOLEAN MMIRES_GetColor(
                               MMI_COLOR_ID_T      color_id,     
                               CAF_HANDLE_T       handle_id,      
                               GUI_COLOR_T          *color_ptr
                               );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLine(
                       void           *dst_ptr,  //目标指针
                       const uint8*   argb_ptr,
                       uint16         left,
                       uint16         right,
                       int32          row
                       );

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLine(
                       void           *dst_ptr,  //目标指针
                       uint16         dst_width,
                       const uint8*   argb_ptr,
                       uint16         top,
                       uint16         bottom,
                       uint16         col
                       );

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPoint(
                     void *          dst_ptr,  //目标指针
                     const uint8*    argb_ptr,
                     uint16          col,
                     uint16          row
                     );

/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLineToArgb8888(
                                 void           *dst_ptr,  //目标指针
                                 const uint8*   argb_ptr,
                                 uint16         left,
                                 uint16         right,
                                 int32          row
                                 );

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLineToArgb8888(
                                 void           *dst_ptr,  //目标指针
                                 uint16         dst_width,
                                 const uint8*   argb_ptr,
                                 uint16         top,
                                 uint16         bottom,
                                 uint16         col
                                 );

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPointToArgb8888(
                               void *          dst_ptr,  //目标指针
                               const uint8*    argb_ptr,
                               uint16          col,
                               uint16          row
                               );

/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLineToArgb8888Overwrite(
                                          void           *dst_ptr,  //目标指针
                                          const uint8*   argb_ptr,
                                          uint16         left,
                                          uint16         right,
                                          int32          row
                                          );

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLineToArgb8888Overwrite(
                                          void           *dst_ptr,  //目标指针
                                          uint16         dst_width,
                                          const uint8*   argb_ptr,
                                          uint16         top,
                                          uint16         bottom,
                                          uint16         col
                                          );

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPointToArgb8888Overwrite(
                                        void *          dst_ptr,  //目标指针
                                        const uint8*    argb_ptr,
                                        uint16          col,
                                        uint16          row
                                        );


/*****************************************************************************/
//     Description : do sabm tile
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:平铺
/*****************************************************************************/
LOCAL void DoSabmTile(
                      const SABM_FUNC_T* func,
                      const uint8   *src_data_ptr, //图像数据
                      uint16        image_x,       //图像的x起始位置
                      uint16        image_y,       //图像的y起始位置
                      uint16        src_width,     //图像宽度
                      uint16        src_height,    //图像高度
                      void          *dst_buf_ptr,  //目标指针
                      GUI_RECT_T    dst_rect,      //目标区域
                      uint16        dest_width     //目标buffer的宽度
                      );

/*****************************************************************************/
//     Description : do sabm hor stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:水平拉伸
/*****************************************************************************/
LOCAL void DoSabmHorStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void          *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            );

/*****************************************************************************/
//     Description : do sabm hor stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:垂直拉伸
/*****************************************************************************/
LOCAL void DoSabmVerStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void         *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            );

/*****************************************************************************/
//     Description : do sabm cen stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:中间拉伸
/*****************************************************************************/
LOCAL void DoSabmCenStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void          *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            );

/*****************************************************************************/
//     Description : Dither func . algorithm form Guangyao.ou
//    Global resource dependence : 
//  Author:Arvin.wu
//    Note:
/*****************************************************************************/
LOCAL uint16   RGB888to565WithDither(
                            uint16 col, //列行
                            uint16 row, //行
                            uint8 _r,   //red分量
                            uint8 _g, //green分量
                            uint8 _b //blue分量
                            );

LOCAL const SABM_FUNC_T s_sabm_defualt_func = 
{
    DrawHorLine,
    DrawVerLine,
    DrawPoint,
    2
};

LOCAL const SABM_FUNC_T s_sabm_argb8888_func = 
{
    DrawHorLineToArgb8888,
    DrawVerLineToArgb8888,
    DrawPointToArgb8888,
    4
};

LOCAL const SABM_FUNC_T s_sabm_argb8888_overwrite_func = 
{
    DrawHorLineToArgb8888Overwrite,
    DrawVerLineToArgb8888Overwrite,
    DrawPointToArgb8888Overwrite,
    4
};

/*****************************************************************************/
//     Description : SABMU16
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL __inline uint16 SABMU16(const uint8* p)
{
    uint16 result = (uint16)( (p[0] << 8) | p[1] );

    return result;
}

/*****************************************************************************/
//     Description : SABMU32
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL __inline uint32 SABMU32(const uint8* p)
{
    uint32 result = (uint32)( (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3] );

    return result;
}

/*****************************************************************************/
//     Description : get sabm func
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL __inline const SABM_FUNC_T* GetSambFunc(
                                              MMI_BUF_TYPE_T type
                                              )
{
    if ( MMI_BUF_ARGB888 == type )
    {
        if ( GUI_IsAlphaOverwrite() )
        {
            return &s_sabm_argb8888_overwrite_func;
        }
        else
        {
            return &s_sabm_argb8888_func;
        }
    }
    else
    {
        return &s_sabm_defualt_func;
    }
}

/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLine(
                       void           *dst_ptr,  //目标指针
                       const uint8*   argb_ptr,
                       uint16         left,
                       uint16         right,
                       int32          row
                       )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32 j = 0;
        uint8        alpha = argb_ptr[3];
        uint8        src_r = argb_ptr[2];
        uint8        src_g = argb_ptr[1];
        uint8        src_b = argb_ptr[0];
        GUI_COLOR_T* dst_buf_ptr = (GUI_COLOR_T*)dst_ptr;

        //need not dither
        if ( row < 0 )
        {
            GUI_COLOR_T color = RGB_888_TO_565(src_r, src_g, src_b);

            if( 0xFF == alpha )
            {
                for ( j = left; j <= right; j++ )
                {
                    *dst_buf_ptr++ = color;
                }
            }
            else
            {
                for ( j = left; j <= right; j++ )
                {
                    uint8  n = (alpha + 1) >> 3;
                    uint32 result = 0;
                    uint32 x = color;
                    uint32 y = *dst_buf_ptr;
                    
                    x = ( x | (x << 16)) & 0x7E0F81F;
                    y = ( y | (y << 16)) & 0x7E0F81F;
                    
                    result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
                    *dst_buf_ptr++ = ((result & 0xFFFF) | (result >> 16));
                }
            }
        }
        else
        {
            if( 0xFF == alpha )
            {
                for ( j = left; j <= right; j++ )
                {
                    *dst_buf_ptr++ = RGB888to565WithDither(j,row,src_r, src_g, src_b);
                }
            }
            else
            {
                for ( j = left; j <= right; j++ )
                {
                    uint8  n = (alpha + 1) >> 3;
                    uint32 result = 0;
                    uint32 x = RGB888to565WithDither(j,row,src_r, src_g, src_b);
                    uint32 y = *dst_buf_ptr;
                    
                    x = ( x | (x << 16)) & 0x7E0F81F;
                    y = ( y | (y << 16)) & 0x7E0F81F;
                    
                    result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
                    *dst_buf_ptr++ = ((result & 0xFFFF) | (result >> 16));
                }
            }
        }
    }
}

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLine(
                       void           *dst_ptr,  //目标指针
                       uint16         dst_width,
                       const uint8*   argb_ptr,
                       uint16         top,
                       uint16         bottom,
                       uint16         col
                       )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32       j = 0;
        uint8        alpha = argb_ptr[3];
        uint8        src_r = argb_ptr[2];
        uint8        src_g = argb_ptr[1];
        uint8        src_b = argb_ptr[0];
        GUI_COLOR_T* dst_buf_ptr = (GUI_COLOR_T*)dst_ptr;

        if( 0xFF == alpha )
        {
            for ( j = top; j <= bottom; j++ )
            {
                *dst_buf_ptr = RGB888to565WithDither(col,j,src_r, src_g, src_b);
                dst_buf_ptr += dst_width;
            }
        }
        else
        {
            for ( j = top; j <= bottom; j++ )
            {
                uint8  n = (alpha + 1) >> 3;
                uint32 result = 0;
                uint32 x = RGB888to565WithDither(col,j,src_r, src_g, src_b);
                uint32 y = *dst_buf_ptr;
                
                x = ( x | (x << 16)) & 0x7E0F81F;
                y = ( y | (y << 16)) & 0x7E0F81F;
                
                result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
                
                *dst_buf_ptr = ((result & 0xFFFF) | (result >> 16));
                dst_buf_ptr += dst_width;
            }
        }
    }
}

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPoint(
                     void *          dst_ptr,  //目标指针
                     const uint8*    argb_ptr,
                     uint16          col,
                     uint16          row
                     )
{
    if ( 0 != argb_ptr[3] )
    {
        uint8        alpha = argb_ptr[3];
        uint8        src_r = argb_ptr[2];
        uint8        src_g = argb_ptr[1];
        uint8        src_b = argb_ptr[0];
        GUI_COLOR_T* temp_pptr = (GUI_COLOR_T*)dst_ptr;

        if (0xFF == alpha)
        {
            *temp_pptr = RGB888to565WithDither(col,row,src_r, src_g, src_b);
        }
        else
        {
            uint8  n = (alpha + 1) >> 3;
            uint32 result  = RGB888to565WithDither(col,row,src_r, src_g, src_b);
            uint32 x = result;
            uint32 y = *temp_pptr;
            x = ( x | (x << 16)) & 0x7E0F81F;
            y = ( y | (y << 16)) & 0x7E0F81F;
            result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
            *temp_pptr = ((result & 0xFFFF) | (result >> 16));
        }
    }
}

/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLineToArgb8888(
                                 void           *dst_ptr,  //目标指针
                                 const uint8*   argb_ptr,
                                 uint16         left,
                                 uint16         right,
                                 int32          row
                                 )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32   j = 0;
        uint32   u32_color = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
        uint32*  dst_buf_ptr = (uint32*)dst_ptr;
        
        for ( j = left; j <= right; j++ )
        {
            *dst_buf_ptr = GUI_CompoundARGB888( *dst_buf_ptr, u32_color );
            dst_buf_ptr++;
        }
    }
}

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLineToArgb8888(
                                 void           *dst_ptr,  //目标指针
                                 uint16         dst_width,
                                 const uint8*   argb_ptr,
                                 uint16         top,
                                 uint16         bottom,
                                 uint16         col
                                 )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32   j = 0;
        uint32   u32_color = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
        uint32*  dst_buf_ptr = (uint32*)dst_ptr;
        
        for ( j = top; j <= bottom; j++ )
        {
            *dst_buf_ptr = GUI_CompoundARGB888( *dst_buf_ptr, u32_color );
            dst_buf_ptr += dst_width;
        }
    }
}

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPointToArgb8888(
                               void *          dst_ptr,  //目标指针
                               const uint8*    argb_ptr,
                               uint16          col,
                               uint16          row
                               )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32* dst_buf_ptr  = (uint32*)dst_ptr;
        uint32  u32_color = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
        
        *dst_buf_ptr = GUI_CompoundARGB888( *dst_buf_ptr, u32_color );
    }
}

/*****************************************************************************/
//     Description : draw argb hor line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawHorLineToArgb8888Overwrite(
                                          void           *dst_ptr,  //目标指针
                                          const uint8*   argb_ptr,
                                          uint16         left,
                                          uint16         right,
                                          int32          row
                                          )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32   j = 0;
        uint32   u32_color = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
        uint32*  dst_buf_ptr = (uint32*)dst_ptr;
        
        for ( j = left; j <= right; j++ )
        {
            *dst_buf_ptr = u32_color;
            dst_buf_ptr++;
        }
    }
}

/*****************************************************************************/
//     Description : draw argb ver line
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawVerLineToArgb8888Overwrite(
                                          void           *dst_ptr,  //目标指针
                                          uint16         dst_width,
                                          const uint8*   argb_ptr,
                                          uint16         top,
                                          uint16         bottom,
                                          uint16           col
                                          )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32   j = 0;
        uint32   u32_color = u32_color = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
        uint32*  dst_buf_ptr = (uint32*)dst_ptr;
        
        for ( j = top; j <= bottom; j++ )
        {
            *dst_buf_ptr = u32_color;
            dst_buf_ptr += dst_width;
        }
    }
}

/*****************************************************************************/
//     Description : draw argb point
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawPointToArgb8888Overwrite(
                                        void *          dst_ptr,  //目标指针
                                        const uint8*    argb_ptr,
                                        uint16          col,
                                        uint16          row
                                        )
{
    if ( 0 != argb_ptr[3] )
    {
        uint32*  dst_buf_ptr = (uint32*)dst_ptr;

        *dst_buf_ptr = (argb_ptr[3] << 24 ) | ( argb_ptr[2] << 16 ) | ( argb_ptr[1] << 8 ) | argb_ptr[0];
    }
}

/*****************************************************************************/
//     Description : do sabm tile
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:平铺
/*****************************************************************************/
LOCAL void DoSabmTile(
                      const SABM_FUNC_T* func,
                      const uint8   *src_data_ptr, //图像数据
                      uint16        image_x,       //图像的x起始位置
                      uint16        image_y,       //图像的y起始位置
                      uint16        src_width,     //图像宽度
                      uint16        src_height,    //图像高度
                      void          *dst_buf_ptr,  //目标指针
                      GUI_RECT_T    dst_rect,      //目标区域
                      uint16        dest_width     //目标buffer的宽度
                      )
{
    uint32       i = 0;
    uint32       j = 0;
    const uint8* temp_src_ptr = PNULL;
    const uint8* data_ptr     = PNULL;
    uint16       left  = dst_rect.left;
    uint16       right = dst_rect.right;
    uint16       top   = dst_rect.top;
    uint16       bottom = dst_rect.bottom;
    uint8*       dst_ptr = (uint8*)dst_buf_ptr;
    uint8*       temp_dst_ptr = PNULL;
    uint8        bits_per_pixel = func->bits_per_pixel;

    if ( 1 == src_width )
    {
        temp_dst_ptr = dst_ptr + (top * dest_width + left) * bits_per_pixel;
        
        for ( i = top; i <= bottom; i++ )
        {   
            //拉伸的像素为图片中间, 宽度为1 width / 2 * 4
            temp_src_ptr = src_data_ptr + ( ( ( image_y + i - top ) % src_height ) << 2 );
            
            func->DrawHorline( temp_dst_ptr, temp_src_ptr, left, right,i);
            
            temp_dst_ptr += dest_width * bits_per_pixel;
        }
    }
    else if ( 1 == src_height )
    {
        temp_dst_ptr = dst_ptr + (top * dest_width + left) * bits_per_pixel;
        
        for ( i = left; i <= right; i++ )
        {
            //拉伸的像素为图片中间, 高度为1 height / 2 * 4
            temp_src_ptr = src_data_ptr + ( ( ( image_x + i - left ) % src_width ) << 2 );
            
            func->DrawVerline( temp_dst_ptr, dest_width, temp_src_ptr, top, bottom,i);
            
            temp_dst_ptr += bits_per_pixel;
        }
    }
    else
    {
        for ( i = top; i <= bottom; i++ )
        {
            temp_dst_ptr = dst_ptr + (i * dest_width + (uint16)dst_rect.left) * bits_per_pixel;
            temp_src_ptr = src_data_ptr + ( ( ( image_y + i - top ) % src_height * src_width ) << 2 );
            
            for ( j = left; j <= right; j++ )
            {
                data_ptr = temp_src_ptr + ( ( ( image_x + j - left ) % src_width ) << 2 );
                
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
        }
    }
}

/*****************************************************************************/
//     Description : do sabm hor stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:水平拉伸
/*****************************************************************************/
LOCAL void DoSabmHorStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void          *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            )
{
    int32        start_pos = 0;
    int32        end_pos = 0;
    int32        i = 0;
    int32        j = 0;
    uint16       half_width = src_width >> 1;
    //宽度为奇数时为1
    uint16       width_offset = 0;
    uint8*       dst_ptr = (uint8*)dst_buf_ptr;
    uint8        bits_per_pixel = func->bits_per_pixel;
    uint16       left;
    uint16       right;
    uint16       top;
    uint16       bottom;
    uint8*       temp_dst_ptr;
    const uint8* data_ptr;
    const uint8* temp_src_ptr;

    if ( half_width > 0 )
    {
        width_offset = ( src_width & 1 ) ? 1 : 0;
    }

    if ( PNULL != full_rect_ptr )
    {
        uint16 full_width   = full_rect_ptr->right - full_rect_ptr->left + 1;
        //uint16 full_height  = full_rect_ptr->bottom - full_rect_ptr->top + 1;
        uint16 image_width  = image_rect_ptr->right - image_rect_ptr->left + 1;
        uint16 image_height = image_rect_ptr->bottom - image_rect_ptr->top + 1;
        uint16 dst_height   = dst_rect.bottom - dst_rect.top + 1;
        uint16 dst_width    = dst_rect.right - dst_rect.left + 1;
        const uint8* middle_ptr;
                
        left         = image_rect_ptr->left;
        right        = image_rect_ptr->right;
        top          = image_rect_ptr->top;
        bottom       = image_rect_ptr->bottom;
        
        if ( src_height <= top )
        {
            return;
        }
        else
        {
            image_height = src_height - top;
        }
        
        image_width = MIN( image_width, dst_width );
        image_height = MIN( image_height, dst_height );

        for ( i = 0; i < image_height; i++ )
        {
            temp_src_ptr = src_data_ptr + (((i + top) * src_width ) << 2);

            middle_ptr = temp_src_ptr + (half_width << 2);

            temp_dst_ptr = dst_ptr + ( ( i + (uint16)dst_rect.top ) * dest_width + (uint16)dst_rect.left) * bits_per_pixel;
            
#if 0
            for ( j = 0; j < image_width; j++ )
            {
                //左
                if ( j + left < half_width )
                {
                    data_ptr = temp_src_ptr + (( j + left ) << 2);
                }
                //中
                else if ( (uint16)(j + left) < full_width - half_width )
                {
                    data_ptr = middle_ptr;
                }
                //右
                else
                {
                    data_ptr = temp_src_ptr + ((j + left - (full_width - half_width) + half_width + width_offset) << 2);
                }
                
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
#else
            //左
            start_pos = 0;
            end_pos = half_width - left;
            if ( end_pos > image_width )
            {
                end_pos = image_width;
            }
            for( j = start_pos; j < end_pos; j++ )
            {
                data_ptr = temp_src_ptr + (( j + left ) << 2);
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
            
            //中
            start_pos = end_pos;
            if ( start_pos < 0 )
            {
                start_pos = 0;
            }
            end_pos = full_width - half_width - left;
            if ( end_pos > image_width )
            {
                end_pos = image_width;
            }
            if ( end_pos - start_pos > 0 )
            {
                func->DrawHorline( temp_dst_ptr, middle_ptr, start_pos, end_pos - 1, i );
                
                temp_dst_ptr += (end_pos - start_pos) * bits_per_pixel;
            }
            else
            {
                end_pos = start_pos;
            }
            
            //右
            start_pos = end_pos;
            if ( start_pos < 0 )
            {
                start_pos = 0;
            }
            end_pos = image_width;
            for( j = start_pos; j < end_pos; j++ )
            {
                data_ptr = temp_src_ptr + ((j + left - (full_width - half_width) + half_width + width_offset) << 2);
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
#endif
        }
    }
    else
    {
        uint16       image_x = 0;
        uint16       image_y = 0;

        left  = dst_rect.left;
        right = dst_rect.right;
        top   = dst_rect.top;
        bottom = dst_rect.bottom;

        if ( PNULL != image_rect_ptr )
        {
            if ( image_rect_ptr->left > 0)
            {
                image_x = image_rect_ptr->left;
            }
            
            if ( image_rect_ptr->top > 0 )
            {
                image_y = image_rect_ptr->top;
            }
        }
        
        //水平拉伸, 垂直方向做截断
        if ( bottom - top + 1 > src_height )
        {
            bottom = top + src_height - 1;
        }
        
        //填充中间区域
        if ( src_width > 0
            && src_height > 0 )
        {
            if ( image_x > 0 || image_y > 0 )
            {
                left   = dst_rect.left;
                right  = dst_rect.right;
            }
            else
            {
                left  = MIN( dst_rect.left + half_width, dst_rect.right );
                right = MAX( dst_rect.right - half_width, dst_rect.left );
            }
            
            temp_dst_ptr = dst_ptr + (top * dest_width + left) * bits_per_pixel;
            
            for ( i = top; i <= bottom; i++ )
            {
                //拉伸的像素为图片中间, 宽度为1 width / 2 * 4
                temp_src_ptr = src_data_ptr + ( ( ( image_y + i - top ) % src_height * src_width + half_width ) << 2 );
                
                func->DrawHorline( temp_dst_ptr, temp_src_ptr, left, right,i);
                
                temp_dst_ptr += dest_width * bits_per_pixel;
            }
        }
    }
}

/*****************************************************************************/
//     Description : do sabm hor stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:垂直拉伸
/*****************************************************************************/
LOCAL void DoSabmVerStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void         *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            )
{
    uint32       i = 0;
    uint32       j = 0;
    uint16       half_height = src_height >> 1;
    //高度为奇数时为1
    uint16       height_offset = 0;
    uint8*       dst_ptr = (uint8*)dst_buf_ptr;
    uint8        bits_per_pixel = func->bits_per_pixel;
    uint16       left;
    uint16       right;
    uint16       top;
    uint16       bottom;
    uint8*       temp_dst_ptr;
    const uint8* data_ptr;
    const uint8* temp_src_ptr;

    if ( half_height > 0 )
    {
        height_offset = ( src_height & 1 ) ? 1 : 0;
    }

    if ( PNULL != full_rect_ptr )
    {
        //uint16 full_width   = full_rect_ptr->right - full_rect_ptr->left + 1;
        uint16 full_height  = full_rect_ptr->bottom - full_rect_ptr->top + 1;
        uint16 image_width  = image_rect_ptr->right - image_rect_ptr->left + 1;
        uint16 image_height = image_rect_ptr->bottom - image_rect_ptr->top + 1;
        uint16 dst_height   = dst_rect.bottom - dst_rect.top + 1;
        uint16 dst_width    = dst_rect.right - dst_rect.left + 1;
        
        left         = image_rect_ptr->left;
        right        = image_rect_ptr->right;
        top          = image_rect_ptr->top;
        bottom       = image_rect_ptr->bottom;

        if ( src_width <= left )
        {
            return;
        }
        else
        {
            image_width = src_width - left;
        }
        
        image_width = MIN( image_width, dst_width );
        image_height = MIN( image_height, dst_height );

        for ( i = 0; i < image_height; i++ )
        {
            //上
            if ( i + top < half_height )
            {
                temp_src_ptr = src_data_ptr + (((i + top) * src_width ) << 2);
            }
            //中
            else if ( (uint16)(i + top) < full_height - half_height )
            {
                temp_src_ptr = src_data_ptr + ((half_height * src_width ) << 2);
            }
            //下
            else
            {
                temp_src_ptr = src_data_ptr + (((i + top  - (full_height - half_height) + half_height + height_offset ) * src_width ) << 2);
            }
            
            temp_dst_ptr = dst_ptr + ( ( i + (uint16)dst_rect.top ) * dest_width + (uint16)dst_rect.left) * bits_per_pixel;

            for ( j = 0; j < image_width; j++ )
            {
                data_ptr = temp_src_ptr + ((j + left ) << 2);
              
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
        }
    }
    else
    {
        uint16       image_x = 0;
        uint16       image_y = 0;

        left  = dst_rect.left;
        right = dst_rect.right;
        top   = dst_rect.top;
        bottom = dst_rect.bottom;
        
        if ( PNULL != image_rect_ptr )
        {
            if ( image_rect_ptr->left > 0)
            {
                image_x = image_rect_ptr->left;
            }
            
            if ( image_rect_ptr->top > 0 )
            {
                image_y = image_rect_ptr->top;
            }
        }

        //垂直拉伸, 水平方向做截断
        if ( right - left + 1 > src_width )
        {
            right = left + src_width - 1;
        }

        //填充中间区域
        if ( src_width > 0
            && src_height > 0 )
        {
            if ( image_x > 0 || image_y > 0 )
            {
                top    = dst_rect.top;
                bottom = dst_rect.bottom;
            }
            else
            {
                top  = MIN( dst_rect.top + half_height, dst_rect.bottom );
                bottom = MAX( dst_rect.bottom - half_height, dst_rect.top );
            }
            
            temp_dst_ptr = dst_ptr + (top * dest_width + left) * bits_per_pixel;
            
            for ( i = left; i <= right; i++ )
            {
                //拉伸的像素为图片中间, 高度为1 height / 2 * 4
                temp_src_ptr = src_data_ptr + ( ( ( half_height ) * src_width + ( image_x + i - left ) % src_width ) << 2 );
                
                func->DrawVerline( temp_dst_ptr, dest_width, temp_src_ptr, top, bottom,i);
                
                temp_dst_ptr += bits_per_pixel;
            }
        }
    }
}

/*****************************************************************************/
//     Description : do sabm cen stretch
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:中间拉伸
/*****************************************************************************/
LOCAL void DoSabmCenStretch(
                            const SABM_FUNC_T* func,
                            const uint8   *src_data_ptr, //图像数据
                            GUI_RECT_T    *full_rect_ptr, //in:
                            GUI_RECT_T    *image_rect_ptr, //in:
                            uint16        src_width,     //图像宽度
                            uint16        src_height,    //图像高度
                            void          *dst_buf_ptr,  //目标指针
                            GUI_RECT_T    dst_rect,      //目标区域
                            uint16        dest_width     //目标buffer的宽度
                            )
{
    int32        start_pos = 0;
    int32        end_pos = 0;
    int32        i = 0;
    int32        j = 0;
    uint16       half_width = src_width >> 1;
    uint16       half_height = src_height >> 1;
    //宽度为奇数时为1
    uint16       width_offset = 0;
    //高度为奇数时为1
    uint16       height_offset = 0;
    uint8*       dst_ptr = (uint8*)dst_buf_ptr;
    uint8        bits_per_pixel = func->bits_per_pixel;
    uint16       left;
    uint16       right;
    uint16       top;
    uint16       bottom;
    uint8*       temp_dst_ptr;
    const uint8* data_ptr;
    const uint8* temp_src_ptr;
    
    if ( half_width > 0 )
    {
        width_offset = ( src_width & 1 ) ? 1 : 0;
    }
    
    if ( half_height > 0 )
    {
        height_offset = ( src_height & 1 ) ? 1 : 0;
    }
    
    if ( PNULL != full_rect_ptr )
    {
        uint16 full_width   = full_rect_ptr->right - full_rect_ptr->left + 1;
        uint16 full_height  = full_rect_ptr->bottom - full_rect_ptr->top + 1;
        uint16 image_width  = image_rect_ptr->right - image_rect_ptr->left + 1;
        uint16 image_height = image_rect_ptr->bottom - image_rect_ptr->top + 1;
        uint16 dst_height   = dst_rect.bottom - dst_rect.top + 1;
        uint16 dst_width    = dst_rect.right - dst_rect.left + 1;
        const uint8* middle_ptr = PNULL;
        
        left   = image_rect_ptr->left;
        right  = image_rect_ptr->right;
        top    = image_rect_ptr->top;
        bottom = image_rect_ptr->bottom;

        image_width = MIN( image_width, dst_width );
        image_height = MIN( image_height, dst_height );

        for ( i = 0; i < image_height; i++ )
        {
            BOOLEAN is_center = FALSE;

            //上
            if ( i + top < half_height )
            {
                temp_src_ptr = src_data_ptr + (((i + top) * src_width ) << 2);
            }
            //中
            else if ( (uint16)(i + top) < full_height - half_height )
            {
                temp_src_ptr = src_data_ptr + ((half_height * src_width ) << 2);

                is_center = TRUE;
            }
            //下
            else
            {
                temp_src_ptr = src_data_ptr + (((i + top  - (full_height - half_height) + half_height + height_offset ) * src_width ) << 2);
            }
            
            middle_ptr = temp_src_ptr + (half_width << 2);
            
            temp_dst_ptr = dst_ptr + ( ( i + (uint16)dst_rect.top ) * dest_width + (uint16)dst_rect.left) * bits_per_pixel;
            
#if 0
            for ( j = 0; j < image_width; j++ )
            {
                //左
                if ( j + left < half_width )
                {
                    data_ptr = temp_src_ptr + (( j + left ) << 2);
                }
                //中
                else if ( (uint16)(j + left) < full_width - half_width )
                {
                    data_ptr = middle_ptr;
                }
                //右
                else
                {
                    data_ptr = temp_src_ptr + ((j + left - (full_width - half_width) + half_width + width_offset) << 2);
                }
                
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
#else
            //左
            start_pos = 0;
            end_pos = half_width - left;
            if ( end_pos > image_width )
            {
                end_pos = image_width;
            }
            for( j = start_pos; j < end_pos; j++ )
            {
                data_ptr = temp_src_ptr + (( j + left ) << 2);
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
            
            //中
            start_pos = end_pos;
            if ( start_pos < 0 )
            {
                start_pos = 0;
            }
            end_pos = full_width - half_width - left;
            if ( end_pos > image_width )
            {
                end_pos = image_width;
            }
            if ( end_pos - start_pos > 0 )
            {
                //need not dither
                func->DrawHorline( temp_dst_ptr, middle_ptr, start_pos, end_pos - 1, is_center ? -1 :i );
                
                temp_dst_ptr += (end_pos - start_pos) * bits_per_pixel;
            }
            else
            {
                end_pos = start_pos;
            }
            
            //右
            start_pos = end_pos;
            if ( start_pos < 0 )
            {
                start_pos = 0;
            }
            end_pos = image_width;
            for( j = start_pos; j < end_pos; j++ )
            {
                data_ptr = temp_src_ptr + ((j + left - (full_width - half_width) + half_width + width_offset) << 2);
                func->DrawPoint( temp_dst_ptr, data_ptr,j,i);
                temp_dst_ptr += bits_per_pixel;
            }
#endif
        }
    }
    else
    {
        uint16       image_x = 0;
        uint16       image_y = 0;

        if ( PNULL != image_rect_ptr )
        {
            if ( image_rect_ptr->left > 0)
            {
                image_x = image_rect_ptr->left;
            }
            
            if ( image_rect_ptr->top > 0 )
            {
                image_y = image_rect_ptr->top;
            }
        }

        //填充中间区域
        if ( src_width > 0
            && src_height > 0 )
        {
            if ( image_x > 0 || image_y > 0 )
            {
                left   = dst_rect.left;
                right  = dst_rect.right;
                top    = dst_rect.top;
                bottom = dst_rect.bottom;
            }
            else
            {
                left  = MIN( dst_rect.left + half_width, dst_rect.right );
                right = MAX( dst_rect.right - half_width, dst_rect.left );
                top  = MIN( dst_rect.top + half_height, dst_rect.bottom );
                bottom = MAX( dst_rect.bottom - half_height, dst_rect.top );
            }
            
            temp_dst_ptr = dst_ptr + (top * dest_width + left) * bits_per_pixel;
            
            //拉伸的像素为图片正中间, 宽度为1 width / 2 * 4, 高度为1 height / 2 * 4
            temp_src_ptr = src_data_ptr + ( ( ( half_height )  * src_width + half_width ) << 2 );
            
            for ( i = top; i <= bottom; i++ )
            {
                //need not dither
                func->DrawHorline( temp_dst_ptr, temp_src_ptr, left, right, - 1/*i*/);
                
                temp_dst_ptr += dest_width * bits_per_pixel;
            }
        }
    }
}

/*****************************************************************************/
//  Description : get sabm image info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetSabmImgInfo(
                                  const uint8 *data_ptr,
                                  uint32 data_size,
                                  SABM_INFO_T *info_ptr   //in/out
                                  )
{
    BOOLEAN result = FALSE;

    if ( PNULL != data_ptr
        && 0 != data_size )
    {
        if ( 's' == data_ptr[0]
            && 'a' == data_ptr[1]
            && 'b' == data_ptr[2]
            && 'm' == data_ptr[3] )
        {
            if ( PNULL != info_ptr )
            {
                info_ptr->width     = SABM_U16(data_ptr + 8);
                info_ptr->height    = SABM_U16(data_ptr + 10);
            }

            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : display sabm image
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_DisplaySabmImg(
                                  MMI_BUF_TYPE_T type,
                                  const uint8   *src_data_ptr,  //in:src data buffer
                                  uint32        src_data_size,  //in:
                                  GUI_RECT_T    *dst_rect_ptr,  //in:
                                  GUI_RECT_T    *full_rect_ptr, //in:
                                  GUI_RECT_T    *image_rect_ptr, //in:
                                  uint16        dst_width,     //in:
                                  uint16        dst_height,    //in:
                                  void          *dst_buf_ptr   //out:dest buffer
                                  )
{
    BOOLEAN result = FALSE;
    //uint32 ticks = SCI_GetTickCount();
    
    if ( PNULL == src_data_ptr
        || src_data_size <= SABM_HEAD_SIZE
        || PNULL == dst_rect_ptr )
    {
        return FALSE;
    }
    
    if ( 's' == src_data_ptr[0]
        && 'a' == src_data_ptr[1]
        && 'b' == src_data_ptr[2]
        && 'm' == src_data_ptr[3] )
    {
        GUI_RECT_T dst_rect = {0};
        GUI_RECT_T buf_rect = {0};
        GUI_RECT_T full_rect = {0};
        GUI_RECT_T img_rect = {0};
        GUI_RECT_T* temp_full_rect = full_rect_ptr;
        GUI_RECT_T* temp_img_rect = image_rect_ptr;
        
        buf_rect.right = dst_width - 1;
        buf_rect.bottom = dst_height - 1;
        
        if ( GUI_IntersectRect( &dst_rect, buf_rect, *dst_rect_ptr ) )
        {
            const uint8* data_ptr = src_data_ptr + SABM_HEAD_SIZE;
            uint16 width = SABM_U16(src_data_ptr + 8);
            uint16 height = SABM_U16(src_data_ptr + 10);
            uint8 flag = *(src_data_ptr + 4);
            uint8 cmpflag = *(src_data_ptr+6);
            const SABM_FUNC_T* func = PNULL;
            uint8 *dst_tmp_ptr = PNULL;
            
            if ( 0 == width 
                || 0 == height )
            {
                return FALSE;
            }
            
            if ( 1 == cmpflag ) //sabm压缩
            {         			
                IMG_RES_SRC_T   res_src = {0};
                IMG_RES_DST_T   display_info = {0};
                IMG_RES_DST_CTL_T   display_ctl = {0};
                
                //set resource image rect
                res_src.imgrect.left   = 0;
                res_src.imgrect.top    = 0;
                res_src.imgrect.right  = width - 1;
                res_src.imgrect.bottom = height - 1;
                
                res_src.imgstream_ptr = (uint8*)data_ptr;
                //res_src.imgstreamsize   
                //set display rect
                display_info.dstrect = res_src.imgrect;
                display_info.dstctl  = &display_ctl;
                
                data_ptr = dst_tmp_ptr = (uint8*)SCI_ALLOCA(width*height*sizeof(uint32));			
                if( PNULL == dst_tmp_ptr )
                {
                    return FALSE;
                }
                
                SCI_MEMSET(dst_tmp_ptr, 0, width*height*sizeof(uint32));
                
                //set display control
                display_ctl.bg_type = IMG_BG_NORMAL;
                display_ctl.format  = IMGREF_FORMAT_ARGB888;
                display_ctl.width   = width;
                display_ctl.height  = height;
                display_ctl.dstmem  = dst_tmp_ptr;
                if (IMG_RES_SUCCESS != IMG_RES_Display_Img(&res_src,&display_info) )
                {
                    SCI_FREE(dst_tmp_ptr);
                    return FALSE;
                }		
            }

            //只绘制中间区域, 兼容之前只有display rect和image rect两个参数的接口
            if ( PNULL == full_rect_ptr 
                && PNULL != image_rect_ptr
                && ( 0 != image_rect_ptr->left || 0 != image_rect_ptr->top ) )
            {
                //do nothing
            }
            else
            {
                if ( PNULL == full_rect_ptr )
                {
                    full_rect.right = dst_rect_ptr->right - dst_rect_ptr->left;
                    full_rect.bottom = dst_rect_ptr->bottom - dst_rect_ptr->top;
                }
                else
                {
                    full_rect.right = full_rect_ptr->right - full_rect_ptr->left;
                    full_rect.bottom = full_rect_ptr->bottom - full_rect_ptr->top;
                }
                
                if ( PNULL == image_rect_ptr )
                {
                    img_rect = full_rect;

//                     if ( dst_rect_ptr->left < 0 )
//                     {
//                         img_rect.left -= dst_rect_ptr->left;
//                     }
//                     
//                     if ( dst_rect_ptr->top < 0 )
//                     {
//                         img_rect.top -= dst_rect_ptr->top;
//                     }
                }
                else
                {
                    img_rect = *image_rect_ptr;
                }
                
                //NEWMS00127373 [6800h_PDA][6804h_240X400]会话模式短信界面异常
                //GUI_IntersectRect( &img_rect, buf_rect, img_rect );
                if ( img_rect.right - img_rect.left > buf_rect.right - buf_rect.left )
                {
                    img_rect.right = img_rect.left + buf_rect.right - buf_rect.left;
                }
                
                if ( img_rect.bottom - img_rect.top > buf_rect.bottom - buf_rect.top )
                {
                    img_rect.bottom = img_rect.top + buf_rect.bottom - buf_rect.top;
                }
                
                
                //图片区域不大于buf_rect
                if ( GUI_IntersectRect( &img_rect, full_rect, img_rect ) )
                {
                    temp_full_rect = &full_rect;
                    temp_img_rect = &img_rect;
                }
                else
                {
                    //若区域无效, 只绘制中间区域
                    temp_full_rect = PNULL;
                }
            }
            
            func = GetSambFunc( type );
            
            if ( PNULL != func )
            {
                if ( flag & SABM_TILE )
                {
                    DoSabmTile( func, data_ptr, temp_img_rect->left, temp_img_rect->top, width, height, dst_buf_ptr, dst_rect, dst_width );
                }
                else if ( flag & SABM_HOR_STRETCH )
                {
                    DoSabmHorStretch( func, data_ptr, temp_full_rect, temp_img_rect, width, height, dst_buf_ptr, dst_rect, dst_width );
                }
                else if ( flag & SABM_VER_STRETCH )
                {
                    DoSabmVerStretch( func, data_ptr, temp_full_rect, temp_img_rect, width, height, dst_buf_ptr, dst_rect, dst_width );
                }
                else if ( flag & SABM_CEN_STRETCH )
                {
                    DoSabmCenStretch( func, data_ptr, temp_full_rect, temp_img_rect, width, height, dst_buf_ptr, dst_rect, dst_width );
                }
            }
            
            if( PNULL != dst_tmp_ptr )
            {
                SCI_FREE(dst_tmp_ptr); 
                dst_tmp_ptr = PNULL;
            } 
            
            result = TRUE;
        }
    }

    //SCI_TRACE_LOW("MMI_DisplaySabmImg ticks =%d", SCI_GetTickCount() - ticks );
        
    return result;
}


/*****************************************************************************/
//     Description : 将ARGB888格式转为预乘
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMI_ARGB888TOPMARGB888(
                                   uint32 *src_ptr, //in,out
                                   uint32 size
                                   )
{
    uint32 count;
    uint32 rb;
    uint32 g;
    uint32 alpha;
    uint32 dst32;

    if ( PNULL == src_ptr
        || 0 == size )
    {
        return;
    }

    count = size / sizeof(uint32);

    while ( count )
    {
#if 1
        count--;
        
        dst32 = *src_ptr;
        
        alpha = (dst32>>24);
        if(0 == alpha)
        {
            *src_ptr++ = 0xff000000;
        }
        else if(255 == alpha)
        {
            *src_ptr++ = (dst32&0x00ffffff);
        }
        else
        {
            g = dst32&0x0000ff00;
            rb = dst32&0x00ff00ff;
            //alpha = 255 - alpha;
            g = ((g*alpha)>>8)&0x0000ff00;
            rb =((rb*alpha)>>8)&0x00ff00ff;
            alpha= ((255-alpha)<<24);
            g = (g+rb);
            *src_ptr++ = (g|alpha) ;
        }
#else
        uint32 temp1;
        uint32 temp2;

        temp1 = *src_ptr;
        alpha = (temp1>>24)&0xff;
        temp2 = temp1&0x00ff00ff;
        temp1 = temp1&0x0000ff00;
        temp2 = ((temp2*alpha)>>8)&0x00ff00ff;
        temp1 = ((temp1*alpha)>>8)&0x0000ff00;
        *src_ptr = temp1|temp2|((0xFF-alpha)<<24);

        src_ptr++;
        count--;
#endif
    }
}

/*****************************************************************************/
//     Description : mem32set
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:地址必须4字节对齐
/*****************************************************************************/
PUBLIC void MMI_MEM32SET(
                         uint32 *u32_ptr, 
                         uint32 value,
                         uint32 size
                         )
{
    if ( PNULL == u32_ptr 
        || 0 == size
        //alignment 4
        || ( ( (uint32)u32_ptr ) & 3 ) )
    {
        return;
    }
    
#ifdef WIN32
    {
        uint32 count = size / sizeof(uint32);

        while (count)
        {
            *u32_ptr++ = value;
            count--;
        }
    }
#else
    {
        extern void *_memset(void *buffer, uint32 value, uint32 size);

        _memset( u32_ptr, size, value );
    }
#endif
}


/*****************************************************************************/
// 	Description : get graph data
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL const MMI_GRAPH_DATA_T* GetGraphData(
                                           const char* name
                                           )
{
    uint32 i = 0;
    uint32 data_num = 0;
    const MMI_GRAPH_DATA_T* data_ptr = PNULL;
#if defined MMI_RES_ORIENT_BOTH
    uint32 data2_num = 0;
    const MMI_GRAPH_DATA_T* data2_ptr = PNULL;
    extern const MMI_GRAPH_DATA_T g_graph_data_array_v[];
    extern const uint32 g_graph_data_num_v;
    extern const MMI_GRAPH_DATA_T g_graph_data_array_h[];
    extern const uint32 g_graph_data_num_h;

     if ( MMITHEME_IsMainScreenLandscape() )
     {
          data_ptr    = g_graph_data_array_h;
          data_num  = g_graph_data_num_h;
          data2_ptr   = g_graph_data_array_v;
          data2_num = g_graph_data_num_v;
     }
     else
     {
          data_ptr    = g_graph_data_array_v;
          data_num  = g_graph_data_num_v;
          data2_ptr   = g_graph_data_array_h;
          data2_num = g_graph_data_num_h;
     }
    
#elif defined MMI_RES_ORIENT_LANDSCAPE
    extern const MMI_GRAPH_DATA_T g_graph_data_array_h[];
    extern const uint32 g_graph_data_num_h;

    data_ptr = g_graph_data_array_h;
    data_num = g_graph_data_num_h;
          
#elif defined MMI_RES_ORIENT_PORTRAIT
    extern const MMI_GRAPH_DATA_T g_graph_data_array_v[];
    extern const uint32 g_graph_data_num_v;

    data_ptr = g_graph_data_array_v;
    data_num = g_graph_data_num_v;
#else
#error res error!!!
#endif

    for ( i = 0; i < data_num; i++ )
    {
        if ( 0 == strcmp( name, data_ptr[i].name ) )
        {
            return data_ptr + i;
        }
    }

 #if defined MMI_RES_ORIENT_BOTH
    for ( i = 0; i < data2_num; i++ )
    {
        if ( 0 == strcmp( name, data2_ptr[i].name ) )
        {
            return data2_ptr + i;
        }
    }
 #endif
 
    return PNULL;
}

/*****************************************************************************/
//  Description : get graph data info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetGraphDataInfo(
                                    const uint8 *data_ptr,
                                    uint32 data_size,
                                    MMI_GRAPH_DATA_T *info_ptr   //in/out
                                    )
{
    BOOLEAN result = FALSE;

    if ( PNULL == data_ptr
        || data_size <= USER_HEAD_SIZE )
    {
        return FALSE;
    }
    
    if ( 'u' == data_ptr[0]
        && 's' == data_ptr[1]
        && 'e' == data_ptr[2]
        && 'r' == data_ptr[3] 
		&& IMG_USER_GRAPH_DATA == *(data_ptr + 4))
    {
        if ( PNULL != info_ptr )
        {
            const MMI_GRAPH_DATA_T* graph_data_ptr = GetGraphData( (char*)(data_ptr + USER_HEAD_SIZE ) );
            
            if ( PNULL != graph_data_ptr )
            {
                *info_ptr = *graph_data_ptr;
            }
        }
        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : display graph data
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_DisplayGraphData(
                                    MMI_BUF_TYPE_T type,
                                    const uint8   *src_data_ptr,   //in:src data buffer
                                    uint32        src_data_size,   //in:
                                    GUI_RECT_T    *dst_rect_ptr,   //in:
                                    GUI_RECT_T    *image_rect_ptr, //in:
                                    uint16        dst_width,       //in:
                                    uint16        dst_height,      //in:
                                    void          *dst_buf_ptr     //out:dest buffer
                                    )
{
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T buf_rect = {0};

    if ( PNULL == src_data_ptr
        || PNULL == dst_rect_ptr
        || src_data_size <= USER_HEAD_SIZE )
    {
        return FALSE;
    }
    
    buf_rect.right = dst_width - 1;
    buf_rect.bottom = dst_height - 1;
    
    if ( !GUI_IntersectRect( &dst_rect, buf_rect, *dst_rect_ptr ) )
    {
        return FALSE;
    }
    
    if ( 'u' == src_data_ptr[0]
        && 's' == src_data_ptr[1]
        && 'e' == src_data_ptr[2]
        && 'r' == src_data_ptr[3] 
		&& IMG_USER_GRAPH_DATA == *(src_data_ptr + 4))
    {
        const MMI_GRAPH_DATA_T* data_ptr = GetGraphData( (char*)(src_data_ptr + USER_HEAD_SIZE ) );
        
        if ( PNULL != data_ptr
            && PNULL != data_ptr->graph_array )
        {
            uint32 i = 0;
            uint32 j = 0;
            uint32 color = 0;
            GUI_RECT_T clip_rect = {0};
            int16 polygon_x = dst_rect_ptr->left;
            int16 polygon_y = dst_rect_ptr->top;

            if ( PNULL != image_rect_ptr )
            {
                clip_rect.left   = dst_rect_ptr->left;
                clip_rect.right  = dst_rect_ptr->left + image_rect_ptr->right - image_rect_ptr->left;
                clip_rect.top    = dst_rect_ptr->top;
                clip_rect.bottom = dst_rect_ptr->top + image_rect_ptr->bottom - image_rect_ptr->top;
                
                GUI_IntersectRect( &clip_rect, dst_rect, clip_rect );

                polygon_x -= image_rect_ptr->left;
                polygon_y -= image_rect_ptr->top;
            }
            else
            {
                clip_rect = *dst_rect_ptr;
            }
            
            for ( ; i < data_ptr->graph_num; i++ )
            {
                //使用资源颜色
                if ( data_ptr->graph_array[i].format & MMI_GRAPH_FORMAT_USE_RES_COLOR )
                {
                    GUI_COLOR_T u16_color = 0;
                    MMIRES_GetColor( data_ptr->graph_array[i].color, MMK_GetFirstAppletHandle(), &u16_color);
                    color = ConvertRgb565ToArgb888( u16_color, 0xFF );
                }
                else
                {
                    color = data_ptr->graph_array[i].color | 0xFF000000;
                }


                switch( data_ptr->graph_array[i].type )
                {
                case MMI_GRAPH_RECT:
                    {
                        if ( PNULL != data_ptr->graph_array[i].data_ptr )
                        {
                            GUI_IntersectRect( &dst_rect, dst_rect, *((GUI_RECT_T*)data_ptr->graph_array[i].data_ptr ) );
                        }
                        
                        if ( !GUI_IsInvalidRectEx( dst_rect )
                            && !GUI_IsInvalidRectEx( clip_rect ) )
                        {
                            if ( MMI_BUF_ARGB888 == type )
                            {
                                LCD_FillRectToBufU32( dst_buf_ptr, dst_width, dst_width, dst_height, dst_rect, color );
                            }
                            else
                            {
                                LCD_FillRectToBuf( dst_buf_ptr, dst_width, dst_width, dst_height, dst_rect, RGB8882RGB565(color) );
                            }
                        }
                    }
                    break;
                case MMI_GRAPH_POLYGON:
                    if ( !GUI_IsInvalidRectEx( dst_rect )
                        && !GUI_IsInvalidRectEx( clip_rect ) )
                    {
                        const MMI_GRAPH_POLYGON_T* polygon_ptr = (const MMI_GRAPH_POLYGON_T*)data_ptr->graph_array[i].data_ptr;

                        if ( PNULL != polygon_ptr
                            && PNULL != polygon_ptr->point_array
                            && polygon_ptr->point_num > 0 )
                        {
                            FILL_POLYGON_IN_T  in_param  = {0};
                            FILL_POLYGON_OUT_T out_param = {0};

                            in_param.pt_num = polygon_ptr->point_num;
                            in_param.points_ptr = SCI_ALLOC_APP( in_param.pt_num * sizeof(IMGREF_POINT_T));

                            for (j = 0; j < in_param.pt_num; j++)
                            {
                                in_param.points_ptr[j].x = polygon_ptr->point_array[j].x + polygon_x;
                                in_param.points_ptr[j].y = polygon_ptr->point_array[j].y + polygon_y;
                            }

                            if ( MMI_BUF_ARGB888 == type )
                            {
                                in_param.data_format = IMGREF_FORMAT_ARGB888;
                                in_param.img_chn.chn0.size = dst_width * dst_height * sizeof(uint32);
                            }
                            else
                            {
                                in_param.data_format = IMGREF_FORMAT_RGB565;
                                in_param.img_chn.chn0.size = dst_width * dst_height * sizeof(GUI_COLOR_T);
                            }

							in_param.u32_color = color;
                            in_param.img_size.w = dst_width;
                            in_param.img_size.h = dst_height;
                            in_param.img_chn.chn0.ptr = (void*)dst_buf_ptr;
                            in_param.clip_rect.x = clip_rect.left;
                            in_param.clip_rect.w = clip_rect.right - clip_rect.left + 1;
                            in_param.clip_rect.y = clip_rect.top;
                            in_param.clip_rect.h = clip_rect.bottom - clip_rect.top + 1;
    
                            GRAPH_FillPolygon( &in_param, &out_param );

                            SCI_FREE(in_param.points_ptr);
                        }
                    }
                    break;
                    
                default:
                    break;
                }
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
//     Description : Dither func . algorithm form Guangyao.ou
//    Global resource dependence : 
//  Author:Arvin.wu
//    Note:
/*****************************************************************************/
LOCAL uint16   RGB888to565WithDither(
                            uint16 row, //行
                            uint16 col, //列
                            uint8 _r,   //red分量
                            uint8 _g, //green分量
                            uint8 _b //blue分量
                            )
{
	uint16 color;
    
	uint32 dither;

	dither = DITHER_VALUE(col,row);
	//R
	color  = (SPRD_R32ToR16(DITHER_R32_FOR_565(_r, dither)) << SPRD_R16_SHIFT) ;
	//G
	color |= (SPRD_G32ToG16(DITHER_G32_FOR_565(_g, dither)) << SPRD_G16_SHIFT) ;
	//B
	color |= (SPRD_B32ToB16(DITHER_B32_FOR_565(_b, dither)) << SPRD_B16_SHIFT) ;

	return (uint16)color;
}
#endif