/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : mmipim_id.c
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-3 ,wangxiaolin,create originally.
*

 *******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmipim_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmipim_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmipim_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_PIM, mmipim_id_name_arr);/*lint !e64 */
}