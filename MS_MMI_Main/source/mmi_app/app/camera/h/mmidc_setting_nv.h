/*************************************************************************
** File Name:      mmidc_nv.h                                           *
** Author:         ryan.xu                                              *
** Date:           2007/02/28                                           *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.   *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2007/02/28     ryan.xu          Create.                              *
*************************************************************************/

#ifndef _MMIDC_SETTING_NV_H_
#define _MMIDC_SETTING_NV_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#if 0    
#define MMINV_READ(_item_id,_addr,_ret_value)\
    _ret_value = MMI_ReadNVItem(_item_id, _addr)
    
#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)
#endif			 
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_DC_SET_INFO = MMI_MODULE_CAMERA<< 16,
    MMINV_SENSOR_INFO,
    MMIDC_MAX_NV_ITEM_NUM
}MMIDC_NV_ITEM_E;
    

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMIDC_NV_H_

