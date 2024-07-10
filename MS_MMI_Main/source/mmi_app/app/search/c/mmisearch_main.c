/*****************************************************************************
** File Name:      mmimtv_export.c                                           *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv app                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef SEARCH_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmi_modu_main.h"
#include "mmisearch_id.h"
//#include "mmisearch_text.h"
//#include "mmi_text.h"
//#include "mmi_image.h"
#include "mmipub.h"
#include "mmk_type.h"
#include "mmk_msg.h"

#include "mmisearch_menutable.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_search_app = {0};

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/




/*****************************************************************************/
//  Description : init pic viewer module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_InitModule(void)
{
    //register menu
    MMISEARCH_RegMenuGroup();

    //register nv
    
    //register win id name
    MMISEARCH_RegWinIdNameArr();
}
#endif
