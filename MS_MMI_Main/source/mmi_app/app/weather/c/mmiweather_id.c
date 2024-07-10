/*************************************************************************
 ** File Name:      mmiweather_id.c                                      *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the IDs for weather module         *
 *************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiweather_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiweather_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register weather win id
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_WEATHER, (const uint8**)mmiweather_id_name_arr);
}
