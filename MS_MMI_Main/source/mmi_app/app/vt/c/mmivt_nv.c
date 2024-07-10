/*************************************************************************
 ** File Name:      mmivt_nv.c                                              *
 ** Author:         bin.ji                                          *
 ** Date:           2006/05/17                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/3/13     bin.ji      Create.                              *
*************************************************************************/
#define MMIVT_NV



//#include "std_header.h"

#ifdef VT_SUPPORT

#include "sci_types.h"
#include "mmivt_nv.h"
#include "mmi_modu_main.h"

/*the length of fm nv*/
const uint16 vt_nv_len[] =
{
    sizeof(uint32),    //MMINV_VT_RECORD_ID
    sizeof(BOOLEAN),   //MMINV_VT_IS_EP_DEFAULT
};

/*****************************************************************************/
// 	Description : register vt module nv len and max item
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_VT, vt_nv_len, sizeof(vt_nv_len) / sizeof(uint16));
}



#endif


