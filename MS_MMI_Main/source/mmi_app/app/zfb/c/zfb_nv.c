/*************************************************************************
 ** File Name:      mmicc_nv.c                                          *
 ** Author:         bruce.chi                                           *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                              *
*************************************************************************/


#define _MMIZFB_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "zfb_nv.h"
#include "mmi_modu_main.h"

/*the length of set nv*/
const uint16 zfb_nv_len[] =
{
    sizeof(uint8),
    sizeof(uint8), //max
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIZFB_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ZFB, zfb_nv_len, ARR_SIZE(zfb_nv_len));
}

