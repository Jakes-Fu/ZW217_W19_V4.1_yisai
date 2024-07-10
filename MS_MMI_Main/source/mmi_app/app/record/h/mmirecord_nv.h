/*************************************************************************
 ** File Name:      mmirecord_nv.h                                       *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu        Create.                             *
*************************************************************************/

#ifndef _MMIRECORD_NV_H_
#define _MMIRECORD_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

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
        MMINV_RECORD_SETTING = MMI_MODULE_RECORD << 16,
        MMIRECORD_MAX_NV_ITEM_NUM
    }
    MMIRECORD_NV_ITEM_E;



    /**--------------------------------------------------------------------------*
     **                         MACRO DEFINITION                                 *
     **--------------------------------------------------------------------------*/
    /*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_RegNv(void);

    /**--------------------------------------------------------------------------*
     **                         Compiler Flag                                    *
     **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMIRECORD_NV_H_

