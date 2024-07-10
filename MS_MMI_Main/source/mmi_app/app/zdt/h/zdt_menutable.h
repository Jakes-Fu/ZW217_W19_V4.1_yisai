/*****************************************************************************
** File Name:      mmicc_menutable.h                                         *
** Author:         bruce.chi                                                 *
** Date:           2006/09/25                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe cc menu id                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/25     bruce.chi        Create                                    *
******************************************************************************/

#ifndef _MMIZDT_MENUTABLE_H_
#define _MMIZDT_MENUTABLE_H_

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
    ID_ZDT_MENU_START = (MMI_MODULE_ZDT << 16),
    ID_ZDT_MENU_TEST1,
    ID_ZDT_MENU_TEST2,
    ID_ZDT_MENU_BH_ONOFF,
    ID_ZDT_MENU_BH_CHAT,
    ID_ZDT_MENU_BH_2VM,
    ID_ZDT_MENU_BH_HELP,
    ID_ZDT_MENU_VOICE_DEL,
    ID_ZDT_MENU_VOICE_DELALL,
	#ifdef ZDT_PLAT_SHB_SUPPORT
	ID_ZDT_MENU_SHB_SERVER,
	ID_ZDT_MENU_SHB_SPEEDDIAL,
	ID_ZDT_MENU_SHB_SOS,
	ID_ZDT_MENU_SHB_HELP,

	ID_ZDT_MENU_SHB_INTRODUCTION,
	ID_ZDT_MENU_SHB_USERGUIDE,
	ID_ZDT_MENU_SHB_CLIENTDOWNLOAD,
	ID_ZDT_MENU_SHB_HOTLINE,
	ID_ZDT_MENU_SHB_VERSION,
	ID_ZDT_MENU_SHB_ABOUTHELP,


	#endif
    ID_ZDT_MENU_MAX
}MMIZDT_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIZDT_MENU_LABEL_START = (MMI_MODULE_ZDT << 16),

    #include "zdt_menutable.def"

    MMIZDT_MENUTABLE_MAX
} MMIZDT_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_CC_MENUTABLE_H_

