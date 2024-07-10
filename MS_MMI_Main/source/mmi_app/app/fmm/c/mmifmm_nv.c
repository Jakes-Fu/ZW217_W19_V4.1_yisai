/*************************************************************************
 ** File Name:      mmifmm_nv.C                                          *
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

#define MMIFMM_NV_C
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmifmm_nv.h"
#include "mmifmm_export.h"
#include "mmi_modu_main.h"


/*the length of fm nv*/
const uint16 fmm_nv_len[] =
{
    sizeof(MMIFMM_SORT_TYPE_E),  
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
#ifdef MMI_DIFF_WALLPAPER_STYLE
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
#ifdef MMI_SAM_THEME    
    sizeof(MMIMULTIM_AS_WALLPAPER_INFO),
#endif
#endif//MMI_DIFF_WALLPAPER_STYLE    
};

/*****************************************************************************/
// 	Description : register fmm module nv len and max item
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RegFMMNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_FMM, fmm_nv_len, sizeof(fmm_nv_len) / sizeof(uint16));
}
