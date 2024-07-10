/*************************************************************************
 ** File Name:      mmifmm_nv.h                                          *
 ** Author:         jian.ma                                              *
 ** Date:           2008/08/22                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/08/22     jian.ma      Create.                                  *
*************************************************************************/

#ifndef MMIFMM_NV_H_
#define MMIFMM_NV_H_

#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

#define MMIFMM_WALLPAPER_STYLE (FMM_MAX_NV_ITEM_NUM - MMINV_MULTIM_AS_WALLPAPER_INFO)
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    FMM_SORT_TYPE = MMI_MODULE_FMM << 16,
    MMINV_MULTIM_AS_WALLPAPER_INFO,
#ifdef MMI_DIFF_WALLPAPER_STYLE    
    MMINV_MULTIM_AS_WALLPAPER_INFO_SMART,
    MMINV_MULTIM_AS_WALLPAPER_INFO_WIDGET,
    MMINV_MULTIM_AS_WALLPAPER_INFO_GRID,
    MMINV_MULTIM_AS_WALLPAPER_INFO_ISTYLE,
    MMINV_MULTIM_AS_WALLPAPER_INFO_QBTHEME,
#ifdef MMI_SAM_THEME    
    MMINV_MULTIM_AS_WALLPAPER_INFO_SAM,
#endif
#endif//MMI_DIFF_WALLPAPER_STYLE
    FMM_MAX_NV_ITEM_NUM
}FMM_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

