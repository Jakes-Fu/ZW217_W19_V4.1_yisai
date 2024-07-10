/*****************************************************************************
** File Name:      mmischedule_atc.h                                               *
** Author:         xiaohua.liu                                               *
** Date:           06/15/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi phonebook files that    *
**                 sync witch pc.                                            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/15/2004        xiaohua liu      Create
******************************************************************************/

#ifndef _MMISCHEDULE_ATC_H_
#define _MMISCHEDULE_ATC_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_atc.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef MMI_SYNCTOOL_V3
/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncAdd( 
						SCHEDULE_ENTRY_T *  schedule_info_ptr, // IN
						uint16 *index_ptr
						);
/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:  xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncModify( 
							uint16           entry_id,    //IN:
							SCHEDULE_ENTRY_T *  schedule_info_ptr  // IN 
							);

/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:  xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncHeadInfo( 
							SCHEDULE_HEAD_INFO_T *schedule_head_info_ptr  // OUT
							);
/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:   xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncRead( 
						uint16           entry_id,    //IN:
						SCHEDULE_ENTRY_T *   schedule_info_ptr  //OUT
						);

/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:     xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncDelete( 
							uint16           entry_id  // IN
							);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#endif

#ifdef   __cplusplus
    }
#endif

#endif

