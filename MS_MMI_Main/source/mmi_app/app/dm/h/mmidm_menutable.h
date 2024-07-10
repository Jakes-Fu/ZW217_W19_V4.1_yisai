/*****************************************************************************
** File Name:      mmidm_menutable.h                                         *
** Author:         jian.ma                                                   *
** Date:           2009/07/02                                                *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to bt menu                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/07/02     jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIDM_MENUTABLE_H_
#define _MMIDM_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    DM_ID_MENU_START = (MMI_MODULE_DM << 16),              
    ID_DM_SOFTWARE_UPDATE_SERVICE,
    ID_DM_SOFTWARE_VERSION,
    ID_DM_SOFTWARE_UPDATE,
    ID_DM_UPDATE_PACKAGE,
    ID_DM_SELECT_SIM,
    ID_DM_APN_SET,
    ID_DM_MON_SET,
    ID_DM_DEV_VERSION_SET,
    ID_DM_MAN_SET,
    ID_DM_IMEI_SET,
    ID_DM_SRV_ADDR_SET,
    ID_DM_SELF_REG_NUMBER,
    ID_DM_SELF_REGPORT_NUMBER,
    ID_DM_CNNECT_SERVER,
    ID_DM_DEBUG_MODE,
    ID_DM_LOG,
    ID_DM_SMS_REG,
    //ID_DM_NULL_SESSION,
    //ID_DM_LOCK,
    ID_DM_TEST_SERVER,
    ID_DM_FOTA,
    //ID_DM_TREE,
    ID_DM_SWITCH,
    ID_DM_RESET,
    ID_DM_SCOMO_INSTALL,
#ifndef  MMI_MULTI_SIM_SYS_SINGLE
    ID_DM_MAIN_SIM,
#endif
    DM_ID_MENU_MAX
}DM_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    DM_MENU_LABEL_START = (MMI_MODULE_DM << 16),
    #include "mmidm_menutable.def"
    DM_MENUTABLE_MAX
} DM_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */

