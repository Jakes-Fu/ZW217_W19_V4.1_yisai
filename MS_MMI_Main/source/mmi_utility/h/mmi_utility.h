/****************************************************************************
** File Name:      mmi_utility.h                                           *
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

#ifndef MMK_UTILITY_H
#define MMK_UTILITY_H

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "os_api.h"
#include "caf.h"
#include "mmk_type.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#if (0 == LIVE_WALLPAPER_SUPPORT_LIGHTGRID) || (0 == LIVE_WALLPAPER_SUPPORT_COLLISION) \
        || (0 == LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON) || (0 == LIVE_WALLPAPER_SUPPOT_COLLISION_MENU) \
        || (0 == LIVE_WALLPAPER_SUPPORT_LINEAR) || (0 == LIVE_WALLPAPER_SUPPORT_POP) \
        || (0 == LIVE_WALLPAPER_SUPPORT_BEE) || (0 == LIVE_WALLPAPER_SUPPORT_GALAXY) \
        || (0 == LIVE_WALLPAPER_SUPPORT_SAKURA) || (0 == LIVE_WALLPAPER_SUPPORT_SEAWORLD) \
        || (0 == LIVE_WALLPAPER_SUPPORT_RIPPLE) || (0 == LIVE_WALLPAPER_SUPPORT_LITTLECAT)\
        || (0 == LIVE_WALLPAPER_SUPPORT_SWEETTREE) || (0 == LIVE_WALLPAPER_SUPPORT_WINDMILL) \
        || (0 == LIVE_WALLPAPER_SUPPORT_RAINBOWCITY) || (0 == LIVE_WALLPAPER_SUPPORT_PAPERTOWN)\
        || (0 == LIVE_WALLPAPER_SUPPORT_AQUARIUM) || (0 == LIVE_WALLPAPER_SUPPORT_GIRLSWING)\
        || (0 == LIVE_WALLPAPER_SUPPORT_SPRING)
    #define LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
#endif
#endif

#define USER_HEAD_SIZE              12

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/  
/*sabm, 输入PNG，输出32位ARGB,展讯私有格式, 用于图像的平铺和拉伸
typedef struct
{
	uint8 tag[4]; //'s','a','b','m'; 
	uint8 flag;
	uint8 reserved1;
	uint8 reserved2;
	uint8 reserved3;
	uint16 width;
	uint16 height;	
}IMG_SABM_HDR_T;
*/
typedef uint16 MMI_BUF_TYPE_T;
#define MMI_BUF_RGB565   0   //565
#define MMI_BUF_ARGB888  1   //argb8888

typedef struct
{
    uint16  width;			/*image widht*/
    uint16  height;			/*image height*/
}SABM_INFO_T; 

typedef enum _IMG_USER_DEFINED_TYPE_E
{
	 IMG_USER_GRAPH_DATA = 0,
	 IMG_USER_LIVE_WALLPAPER,
	 
	 IMG_USER_MAX
}IMG_USER_DEFINED_TYPE_E;

//graph, 用图形代替图片
typedef uint16 MMI_GRAPH_TYPE_T;
#define MMI_GRAPH_RECT     0  
#define MMI_GRAPH_POLYGON  1

typedef uint16  MMI_GRAPH_FORMAT_T;
#define MMI_GRAPH_FORMAT_USE_RES_COLOR       0x00000001         //使用资源颜色id

typedef struct
{
    uint32               point_num;    //多边形点的个数
    const GUI_POINT_T*   point_array;  //多边形点的数组
}MMI_GRAPH_POLYGON_T;

typedef struct
{
    MMI_GRAPH_TYPE_T        type;       //图形类型
    MMI_GRAPH_FORMAT_T      format;     //图形格式
    uint32                  color;      //颜色值或资源颜色id
    const void*             data_ptr;   //根据MMI_GRAPH_TYPE_T确定图形类型
                                        //MMI_GRAPH_RECT为GUI_RECT_T
                                        //MMI_GRAPH_POLYGON为MMI_GRAPH_POLYGON_T
}MMI_GRAPH_ITEM_T;

typedef struct
{
	const char*             name;         //图形替换的资源id的名字
    uint32                  graph_num;    //图形的数量
    const MMI_GRAPH_ITEM_T* graph_array;  //图形的数组
    uint16                  width;        //整个图形的宽
    uint16                  height;       //整个图形的高
}MMI_GRAPH_DATA_T;

typedef struct  
{
	const char*				name;		 // 动态墙纸的资源id名称
	int32					offset;		 // 偏移
}MMI_CUSTOM_LW_DATA_T;

//////////////////////////////////////////////////////////////////////////
// common
LOCAL __inline uint16 B2LU16(const uint8* p)
{
    uint16 result = (uint16)( (p[0] << 8) | p[1] );
    
    return result;
}

LOCAL __inline uint32 B2LU32(const uint8* p)
{
    uint32 result = (uint32)( (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3] );
    
    return result;
}

#define B2L_U16_F(p)    B2LU16((const uint8*)(p))
#define B2L_U32_F(p)    B2LU32((const uint8*)(p))

LOCAL __inline BOOLEAN MMI_IsBigEndian( void )
{
#ifndef WIN32
    uint32 u32 = 0x1;
    uint8* pu8 = (uint8*)(&u32);
    
    return (BOOLEAN)((0x1 == *pu8)? FALSE : TRUE);
#else
#if defined (PLATFORM_SC6800H)
    return TRUE;
#else
    return FALSE;
#endif
#endif
}

#ifndef WIN32
    #if defined(__BIG_ENDIAN) || defined(__BigEndian)
        #define B2L_U16(p)    B2L_U16_F(p)
        #define B2L_U32(p)    B2L_U32_F(p)
    #else
        #define B2L_U16(p)    (*(uint16*)(p))
        #define B2L_U32(p)    (*(uint32*)(p))
    #endif
#else
    #if defined (PLATFORM_SC6800H)
        #define B2L_U16(p)    B2L_U16_F(p)
        #define B2L_U32(p)    B2L_U32_F(p)
    #else
        #define B2L_U16(p)    (*(uint16*)(p))
        #define B2L_U32(p)    (*(uint32*)(p))	
    #endif
#endif

PUBLIC void MMI_B2LWstr(wchar *str_ptr);
PUBLIC uint32 MMI_WDJBHash(const wchar *str);
PUBLIC uint32 MMI_DJBHash(const char *c_str);
PUBLIC void MMI_GetMainLCDRealSpecs(
                                    uint16* lcd_w_ptr, 
                                    uint16* lcd_h_ptr
                                    );

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
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
                                  );

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
                                  GUI_RECT_T    *image_rect_ptr, //in:image rect,may PNULL代表从图片(0,0)点显示
                                  uint16        dst_width,     //in:
                                  uint16        dst_height,    //in:
                                  void          *dst_buf_ptr   //out:dest buffer
                                  );

/*****************************************************************************/
//     Description : 将ARGB888格式转为预乘
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMI_ARGB888TOPMARGB888(
                                   uint32 *src_ptr, //in,out
                                   uint32 size
                                   );

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
                         );


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
                                    );

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
                                    );

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
/*******************************************************************/
//  Interface:		MMI_GetLivewallpaerImageResData
//  Description : 	MMI_GetLivewallpaerImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_GetLivewallpaerImageResData(
                                               uint32        image_id,
                                               const char*   image_id_str,
                                               uint8**       data_pptr, 
                                               uint32*       len_ptr
                                               );

/*******************************************************************/
//  Interface:		MMI_LivewallpaperResListInit
//  Description : 	MMI_LivewallpaperResListInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_LivewallpaperResListInit(void);
/*******************************************************************/
//  Interface:		MMI_DestroyLivewallpaperResList
//  Description : 	MMI_DestroyLivewallpaperResList
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_DestroyLivewallpaperResList(void);
/*******************************************************************/
//  Interface:		MMI_IsLivewallpaperResExist
//  Description : 	MMI_IsLivewallpaperResExist
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMI_IsLivewallpaperResExist(
                                           const char* name_ptr
                                           );

/*******************************************************************/
//  Interface:		MMI_LivewallpaperCleanUp
//  Description : 	MMI_LivewallpaperCleanUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_LivewallpaperCleanUp(void);
/*******************************************************************/
//  Interface:		MMI_LivewallpaperInitHandle
//  Description : 	MMI_LivewallpaperInitHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_LivewallpaperInitHandle(
                                        const char* lw_name_ptr
                                        );
#endif

/*******************************************************************/
//  Interface:		MMI_LivewallpaperResFoldInit
//  Description : 	MMI_LivewallpaperResFoldInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMI_LivewallpaperResFoldInit(void);

#ifdef _cplusplus
	}
#endif

#endif /* MMK_UTILITY_H */
