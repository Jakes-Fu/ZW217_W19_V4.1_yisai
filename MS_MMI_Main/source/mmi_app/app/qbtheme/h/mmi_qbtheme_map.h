/*****************************************************************************
** File Name:      xxx_mmi_qbtheme_map.h                                           *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef COMMON_MMI_QBTHEME_MAP_H_
#define COMMON_MMI_QBTHEME_MAP_H_

#include "sci_types.h"
#include "caf.h"

#ifdef __cplusplus
extern   "C" {
#endif
		
typedef struct
{
	uint32	img_id;
	uint32	range;
	wchar*  img_path;
	int32	offset;
}QBTHEME_MAP_ITEM;

extern QBTHEME_MAP_ITEM g_qbtheme_map[];

//////////////////////////////////////////////////////////////////////////

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetImageResData
//  Description : 	MMIQBTHEME_GetImageResData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_GetImageResData(
										  uint32 image_id, 
										  uint8** data_pptr, 
										  uint32* len_ptr
										  );

#ifdef __cplusplus
}
#endif

#endif //COMMON_MMI_QBTHEME_MAP_H_


