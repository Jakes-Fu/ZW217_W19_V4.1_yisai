/*****************************************************************************
** File Name:      u_launcher_internal.h                                      *
** Author: kyle.jin         kyle                                              *
** Date:           14/02/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe go menu se                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      kyle            Creat
******************************************************************************/
#ifndef _U_LAUNCHER_INTERNAL_H_
#define _U_LAUNCHER_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "guistring.h"
#include "graphics_3d_rotate.h"
#include "guimenu.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GO_DEFAULT_COEFFCIENT 40

/*****************************************************************
**   U 桌面算法接口函数及相关
**
******************************************************************/
typedef struct GO_MAINMENU_ICON_INFO_TAG
{
    uint32* icon_buf;//图标BUF地址
    IMGREF_POINT_T icon_offset;//图标相对于屏幕左上角的偏移
    IMGREF_SIZE_T icon_size;//图标大小
    uint16 item_row_num;//第几行
    uint16 item_col_num;//第几列 
}GO_MAINMENU_ICON_INFO_T;

typedef struct GO_MAINMENU_PAGE_INFO_TAG
{
    uint8 total_row_num;//一屏上有几行
    uint8 total_col_num;//一屏上有几列
    uint16 icon_num;//一屏图标的数目
    GO_MAINMENU_ICON_INFO_T* icon_info;//图标属性
}GO_MAINMENU_PAGE_INFO_T;

typedef enum
{
    GO_MOVE_NONE = 0,//无
    GO_MOVE_LEFT,//向左滑动
    GO_MOVE_RIGHT,//向右滑动
    GO_MOVE_UP,//向上滑动
    GO_MOVE_DOWN,//向下滑动
    GO_MOVE_MAX
}GO_MOVE_DIRECTION_E;

typedef struct GO_rota_data_t {
	
   uint16	axis_x;

   uint16	axis_y;
															
   uint16	axis_z;
   
}GO_ROTA_DATA_T;

//算法的数据信息
typedef struct GO_CALC_DATA_INFO{
    S3D_DATA_TYPE_E input_type;
    S3D_DATA_TYPE_E output_type;
    S3D_EXPANDPTR_T* param;
}GO_CALC_DATA_INFO_T;

//算法函数类型:
typedef BOOLEAN (*go_calc_func)(GO_MAINMENU_PAGE_INFO_T *,GO_MAINMENU_PAGE_INFO_T *,GO_MAINMENU_PAGE_INFO_T *,uint16 ,uint16 ,IMGREF_SIZE_T ,void* ,GO_MOVE_DIRECTION_E ,FIX16_POINT_T*,GO_CALC_DATA_INFO_T*);

//每种特效的特殊信息
typedef struct GO_CALC_STYLE_INFO{
    go_calc_func calc_func;//算法函数
    GUIMENU_GO_SLIDE_TYPE_E slide_type;//支持的滑动方向
    uint16 calc_time;//每中效果计算的时间
}GO_CALC_STYLE_INFO_T;

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/


#endif/*_U_LAUNCHER_INTERNAL_H_*/