/*****************************************************************************
 ** File Name:   mmiappsample_id.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe sample app id 
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiappsample_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiappsample_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register app samle win and win name
//	Global resource dependence : none
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_APPSAMPLE, (const uint8 **)mmiappsample_id_name_arr);
}
