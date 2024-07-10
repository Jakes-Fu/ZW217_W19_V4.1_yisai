
/*****************************************************************************
** File Name:      mmidcd_menutable.h                                        *
** Author:         violent.wei                                               *
** Date:           2007/09/24                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    					                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/09/24     violent.wei      Create                                    *
******************************************************************************/
#ifndef _MMICS_MENUTABLE_H_
#define _MMICS_MENUTABLE_H_

#ifdef CMCC_UI_STYLE
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
	ID_CS_MENU_START = (MMI_MODULE_CMCS << 16),
	//CS
    //for cs menu
    ID_CS_PHONE_SERVICE_GUIDE,
    ID_CS_E100,
    ID_CS_CUSTMER_MANAGER,
    ID_CS_MY_MONERNET,
    ID_CS_CUSTMER_SETTING,

	ID_CS_HOTLINE,
	ID_CS_SMS_OFFICE,
	ID_CS_POCKET_HALL,
	ID_CS_ABOUT_E100,

	ID_CS_SERVICE_HOTLINE,
	ID_CS_MANAGER_NUMBER,
    
	ID_DCD_MENU_MAX
    
}MMICS_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMICS_MENU_LABEL_START = (MMI_MODULE_CMCS << 16),

    #include "mmics_menutable.def"

    MMICS_MENUTABLE_MAX
} MMICS_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif //_DCD_MENUTABLE_H_

