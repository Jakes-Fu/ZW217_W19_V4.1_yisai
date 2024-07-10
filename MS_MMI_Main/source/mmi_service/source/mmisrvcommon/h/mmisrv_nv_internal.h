/*****************************************************************************
** File Name:      mmisrv_nv_internal.h                                           *
** Author:                                                                   *
** Date:           10/2009                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMISRV_NV_INTERNAL_H_
#define _MMISRV_NV_INTERNAL_H_

#include "sci_types.h"
#include "nv_item_id.h"
#include "mmisrv_nv.h"

#ifdef __cplusplus
extern   "C" {
#endif

	/*
#ifdef CMCC_UI_STYLE
#define MAX_MMI_NV_USER_ITEM_NUM     3800
#else
#define MAX_MMI_NV_USER_ITEM_NUM     2800
#endif

#ifdef MMI_DUALMODE_ENABLE
#define MMI_NV_USER_BASE     MN_NV_TD_USER_BASE//800  @$discuss D¨¨¨°a?¨´?Y¨º¦Ì?¨º¡ä¨®D?DT??
#else
#define MMI_NV_USER_BASE     MN_NV_USER_BASE
#endif

#define MMI_NV_MAX_NUM  (MMI_NV_USER_BASE + MAX_MMI_NV_USER_ITEM_NUM)
  */
#ifdef CMCC_UI_STYLE
#define MAX_MMI_NV_USER_ITEM_NUM     3800
#else
#define MAX_MMI_NV_USER_ITEM_NUM     2800
#endif

#ifdef PRODUCT_DM
#define MMI_NV_USER_BASE     MN_NV_TD_USER_BASE//800  @$discuss D¡§¡§¡§¡ãa?¡§¡ä?Y¡§o|¨¬?¡§o??¡§?D?DT??
#else
#define MMI_NV_USER_BASE     MN_NV_USER_BASE
#endif

#define MMI_NV_MAX_NUM  (MMI_NV_USER_BASE + MAX_MMI_NV_USER_ITEM_NUM)

#define MMISRV_NV_DEF(nv_id, nv_item_size)          nv_id,

//The special NV item
typedef enum
{
	MMISRVSAMPLE_NV_START = MMISRV_MODULE_SAMPLE << 16, 
#include "mmisrvsample_nv_def.h"
	MMISRVSAMPLE_NV_MAX
};

#undef MMISRV_NV_DEF

/*****************************************************************************/
//  Description : Read NV info by id  
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMISRV_ReadNVItem(uint32 item_id, void * buffer_ptr);

/*****************************************************************************/
//  Description : write NV info by id  
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_WriteNVItem(uint32 item_id, void * buffer_ptr);

/*****************************************************************************/
//  Description : register sample module nv len and  max item
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_RegSampleNv(void);

/*****************************************************************************/
//  Description : set reg modle nv start flag
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_RegModuleNvStart (void);

/*****************************************************************************/
//  Description : set reg modle nv end flag
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_RegModuleNvEnd (void);

/*****************************************************************************/
//  Description : register module nv len and max nv item 
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_RegModuleNv(uint16 module_id, const uint16 nv_len[], uint16 nv_max_item);

#define MMISRVNV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMISRV_ReadNVItem(_item_id, _addr)

#define MMISRVNV_WRITE(_item_id,_addr) \
			 MMISRV_WriteNVItem(_item_id, _addr)

#ifdef __cplusplus
}
#endif

#endif // _MMISRV_NV_H_


