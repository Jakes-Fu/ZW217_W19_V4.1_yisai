/******************************************************************************
**  File Name:      mmimtv_nv.h                                               *
**  Author:         xinrui.wang                                               *
**  Date:           2008/01/3                                                 *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  2008/01/3     xinrui.wang      Create.                                    *
******************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_NV_H_ /* compile limiting */
#define _MMIMTV_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmimtv_data.h"
#include "mn_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMIMTV_NV_CTRL_TABLE_MAX_LENGTH		812
#define MMIMTV_NV_CTRL_TABLE_MAX_NUM		(MMIMTV_NET_NUMBER)

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
             MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_MTV_CTL_TAB = MMI_MODULE_MTV << 16,
    MMINV_MTV_PROG_ID,
    MMINV_MTV_SETTING_INFO,
#ifdef MBBMS_SUPPORT
    MMINV_MBBMS_SETTING_INFO,
    MMINV_MBBMS_SGDD_INFO,    
#endif
    MMINV_MTV_MAX_NV_ITEM_NUM
}FM_NV_ITEM_E;


typedef struct MMIMTV_NV_CTL_T_tag
{
#ifdef MBBMS_SUPPORT
    anyURI sgdd_id;
#endif
    //uint32	is_invalid;
	uint16	net_id;
    uint32  center_freq;
	uint16  data_len;
	uint8	ctl_data[MMIMTV_NV_CTRL_TABLE_MAX_LENGTH];
}MMIMTV_NV_CTL_T;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description: register java module nv len and max item
// Global resource dependence: none
// Author: xinrui.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_RegMtvNv(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif

