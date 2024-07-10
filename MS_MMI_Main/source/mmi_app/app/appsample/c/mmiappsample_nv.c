/*****************************************************************************
 ** File Name:   mmiappsample_nv.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe nv 
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/



#define _MMIAPPSAMPLE_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiappsample_nv.h"
#include "mmi_modu_main.h"
#include "mmiappsample_internal.h"


/*the length of set nv*/
const uint16 appsample_nv_len[] =
{
    sizeof(SRVSAMPLE_PERSON_DATA_T),              
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_APPSAMPLE, appsample_nv_len, ARR_SIZE(appsample_nv_len));
}



