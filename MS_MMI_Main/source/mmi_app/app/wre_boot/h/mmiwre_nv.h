/*************************************************************************
 ** File Name:      mmiwre_nv.h                                      *
 ** Author:          Qingjun.Yu                                      *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about nv              *
 *************************************************************************/

#ifndef MMIWRE_NV_H_
#define MMIWRE_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
   	 MMIWRE_NV_RUNTIME_PATH = MMI_MODULE_WRE << 16, 
	 MMIWRE_NV_MEDIA_VOLUME,//added by leichi 20120424
   	 MMIWRE_NV_MAX_ITEM_NUM
}MMIWRE_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register wre module nv len and max item
//	Global resource dependence : none
//  Author: Qingjun.YU
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_RegWreNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif	//MMIWEATHER_NV_H_
