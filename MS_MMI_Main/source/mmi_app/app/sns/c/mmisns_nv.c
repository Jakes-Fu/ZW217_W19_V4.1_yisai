/*************************************************************************
 ** File Name:      mmisns_nv.c                                           *
 ** Author:         renwei                                          *
 ** Date:           11/2011                                       *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 11/2011        renwei      Create.                                   *
*************************************************************************/
#ifdef SNS_SUPPORT
#include "std_header.h"
#include "sci_types.h"
#include "mmisns_nv.h"

#include "mmi_modu_main.h"
#include "mmisns_srv.h"

/*the length of sns nv*/
const uint16 sns_reg_nv_len[] =
{
    sizeof(BOOLEAN), // MMINV_SNS_FLAG

    //sns client data info
    sizeof(T_SNSClientRegInfo), // size of T_SNSClientRegInfo
};

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: renwei
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegSNSClientInfoNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_MMISNS, sns_reg_nv_len, sizeof(sns_reg_nv_len)/sizeof(uint16));
}
#endif

