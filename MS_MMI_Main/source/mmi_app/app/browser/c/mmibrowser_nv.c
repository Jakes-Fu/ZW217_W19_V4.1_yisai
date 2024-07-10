/*****************************************************************************
** File Name:      mmibrowser_nv.c                                           *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser nv              *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmibrowser_nv.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_bookmark.h"
#include "mmibrowser_func.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*the length of browser nv*/
const uint16 mmibrowser_nv_len[] =
{
    sizeof(MMIBROWSER_NV_SETTING_T),    // MMIBROWSER_SETTING
    sizeof(BOOLEAN),//MMINV_BROWSER_IS_BOOKMARK_LOAD
#ifdef BROWSER_SEARCH_SUPPORT
    sizeof( MMIBROWSER_KEY_T ),//MMINV_BROWSER_KEY
    sizeof(MMIBROWSER_SEARCH_ENGINE_MODE_E),//MMINV_BROWSER_SEARCH_ENGINE
#endif    

#ifdef MMIDORADO_MY_NAV_SUPPORT
    sizeof(MMIBRW_MY_NAV_CONTEXT_T),//MMINV_BROWSER_MY_NAV_INFO
#endif  

#ifdef MMIDORADO_BOOKMARK_BY_NV
    sizeof(MMIBRW_BOOKMARK_T),//MMINV_BROWSER_BOOKMARK_INFO
#endif
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_MMIBROWSER,mmibrowser_nv_len,sizeof(mmibrowser_nv_len)/sizeof(uint16));
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
