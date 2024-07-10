/*****************************************************************************
** File Name:      mmisrv_nv.h                                           *
** Author:                                                                   *
** Date:           10/2009                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMISRV_NV_H_
#define _MMISRV_NV_H_

#include "sci_types.h"
#include "nv_item_id.h"

#ifdef __cplusplus
extern   "C" {
#endif


/*****************************************************************************/
//  Description : get end id
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISRV_GetEndId(void);

/*****************************************************************************/
//  Description : get max id
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISRV_GetMaxId(void);

/*****************************************************************************/
//  Description : test sample nv
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_TestSampleNVItem(void);

/*****************************************************************************/
//     Description : get sum size of all modules nv size
//    Global resource dependence : 
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISRV_GetAllModuleSumNvSize(void);

/*****************************************************************************/
//  Description :  get nv item num of all mmi mudule 
//  Global resource dependence : none
//  Author: 		bin.ji
//  Note:		
/*****************************************************************************/
PUBLIC uint32 MMISRV_GetAllMMINVItem(void);

#ifdef __cplusplus
}
#endif

#endif // _MMISRV_NV_H_


