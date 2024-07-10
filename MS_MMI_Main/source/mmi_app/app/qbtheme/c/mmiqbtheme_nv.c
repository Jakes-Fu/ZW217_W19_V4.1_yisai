/*************************************************************************
 ** File Name:      mmiqbtheme_nv.h                                       *
 ** Author:         cheng.luo                                           *
 ** Date:           2011/09/17                                          *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/09/17     cheng.luo       Create.                              *
*************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef QBTHEME_SUPPORT

#include "mmiqbtheme_nv.h"
#include "mmi_modu_main.h"
#include "mmiqbtheme_internal.h"

/*the length of Qbtheme nv*/
const uint16 qbtheme_nv_len[] =
{
	sizeof(uint8),
	sizeof(uint8),
	sizeof(uint8),
	sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1),
	sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1)
};

/*****************************************************************************/
// 	Description : register Qbtheme module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIQBTHEME_RegQbThemeNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_QBTHEME, qbtheme_nv_len, sizeof(qbtheme_nv_len)/sizeof(qbtheme_nv_len[0]));
}

#endif


