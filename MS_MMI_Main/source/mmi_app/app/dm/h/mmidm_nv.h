/*************************************************************************
 ** File Name:      mmidm_nv.h                                           *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/

#ifndef MMIDM_NV_H_
#define MMIDM_NV_H_

#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_DM_IMSI_INFO = MMI_MODULE_DM << 16,
    MMINV_DM_APN,
    MMINV_DM_GATEWAY_IP,
    MMINV_DM_PORT,
    MMINV_DM_SRNUM,
    MMINV_DM_SRPORT,
    MMINV_DM_MON_SET,
    MMINV_DM_MAN_SET,
    MMINV_DM_IMEI_SET,  
    MMINV_DM_HANDSET_LOCK,
    MMINV_DM_DOWNLOAD_FINISH,
    MMINV_DM_SERVER_TYPE,
    MMINV_DM_REG_SWITCH,
    MMINV_DM_FOTA_STATE,
    MMINV_DM_APPID_TAB,
    MMINV_DM_RESUME_TYPE,
    MMINV_DM_UI_MODE,
    MMINV_DM_BROKEN_TYPE,
    MMINV_DM_ALERT_TYPE,
    MMINV_DM_MAIN_SIM,
    MMINV_DM_MAX_ITEM_NUM
}DM_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

