/*************************************************************************
 ** File Name:      mmiebook_nv.c                                        *
 ** Author:         maggie.ren                                           *
 ** Date:           2007/01/15                                           *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/01/15     maggie.ren       Create.                              *
*************************************************************************/


#define _MMIEBOOK_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef EBOOK_SUPPORT
#include "mmifmm_export.h"
#include "mmiebook_nv.h"
#include "mmi_modu_main.h"
//#include "mmiebook.h"
#include "mmiebook_internal.h"
#include "mmiebook_file.h"

/*the length of EBOOK nv*/
const uint16 ebook_nv_len[] =
{
    sizeof(uint16),
    sizeof(MMIEBOOK_SET_CODING_TYPE_E),   
    sizeof(MMIEBOOK_FONT_SIZE_E),
#ifdef TTS_SUPPORT
    sizeof( BOOLEAN ),  //TTS open
    sizeof( uint8 ),    //TTS vol
#endif
    sizeof(MMIEBOOK_SET_COLOR_E),
    sizeof(MMIEBOOK_SET_COLOR_E),
    sizeof(MMIEBOOK_FILE_BRIEF_T),	//ÎÄ¼þÃû
    sizeof( BOOLEAN ),
    sizeof(MMIEBOOK_SET_AUTO_READ_E),
    sizeof(MMIEBOOK_DAY_NIGHT_MODE_E),
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: maggie.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_EBOOK, ebook_nv_len, sizeof(ebook_nv_len) / sizeof(uint16));
}
#endif
