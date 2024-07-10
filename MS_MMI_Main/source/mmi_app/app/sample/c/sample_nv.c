/*************************************************************************
 ** File Name:      hello_nv.c                                           *
 ** Author:         xiaoqing.lu                                          *
 ** Date:           04/09/2009                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 04/09/2009     xiaoqing.lu      Create.                              *
*************************************************************************/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE

#include "sci_types.h"
#include "sample_nv.h"
#include "sample_main.h"
#include "mmi_modu_main.h"


/*the length of alarm nv*/
const uint16 hello_nv_len[] =
{
    sizeof(BOOLEAN), // MMINV_HELLO_FLAG

    //hello data info
    sizeof(MMI_HELLO_T), // size of MMI_HELLO_T
};

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegHelloNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SAMPLE, hello_nv_len, sizeof(hello_nv_len)/sizeof(uint16));
}
#endif

