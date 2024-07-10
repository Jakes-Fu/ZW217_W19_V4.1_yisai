/*************************************************************************
 ** File Name:      mmiwww_nv.h                                        *
 ** Author:          kun.yu                                         *
 ** Date:             2009/06/26                                          *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/06/26     kun.yu      Create.                              *
*************************************************************************/

#ifndef _MMIWWW_NV_H_
#define _MMIWWW_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmiwww_internal.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_WWW_FONT_STYLE = MMI_MODULE_WWW << 16,
    MMINV_WWW_VIEW_MODE,
    MMINV_WWW_SCRIPT_MODE,
    MMINV_WWW_ANIMATION_MODE,
    MMINV_WWW_PHOTO_MODE,
    MMINV_WWW_TABLE_MODE,
    MMINV_WWW_BGMUSIC_MODE,
    MMINV_WWW_POPUPWIN_MODE,
    MMINV_WWW_COOKIE_MODE,
    MMINV_WWW_CACHE_MODE,
    MMINV_WWW_WEBDATA_MODE,
    MMINV_WWW_DISKCACHE_MODE,
    MMINV_WWW_KEY,
    MMINV_WWW_SEARCH_ENGINE,
    MMINV_WWW_LINK_SETTING,
    MMINV_WWW_PAGEMEMO_SORT_TYPE,
    MMINV_WWW_FIRST_ENTRY,
    MMINV_WWW_CHARSET,
    MMINV_WWW_POINTER_MODE,
    MMINV_WWW_HOMEPAGE,

    MMINV_WWW_MAX_ITEM_NUM

}WWW_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register mmiwww module nv len and max item
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegNv(void);

/*****************************************************************************/
// 	Description : write key name to nv list
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NV_WriteKeyItem( MMI_STRING_T *in_url  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIWWW_NV_H_

