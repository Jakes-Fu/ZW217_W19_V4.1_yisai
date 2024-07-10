/*****************************************************************************
** File Name:      mmiacc_menutable.h                                        *
** Author:         haiyang.hu                                                *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to acc menu                             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     haiyang.hu       Create                                    *
******************************************************************************/

#ifndef _MMIACC_MENUTABLE_H_
#define _MMIACC_MENUTABLE_H_

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
    MMIACC_ID_MENU_START = (MMI_MODULE_ACC << 16),

	ID_ACC_CALC_MENU_MPLUS,  
	ID_ACC_CALC_MENU_MMINUS,   
	ID_ACC_CALC_MENU_MRECALL,
	ID_ACC_CALC_MENU_MCLEAR,
	ID_ACC_CALC_MENU_CLEAR,

    ID_ACC_WCLK_SET_ADD_CITY,  
    ID_ACC_WCLK_SET_DELETE_CITY,
    ID_ACC_WCLK_SET_LOCAL1,
    //ID_ACC_WCLK_SET_LOCAL2,
    //ID_ACC_WCLK_SET_CURRENT,
    ID_ACC_WCLK_SET_DST,
    ID_ACC_WCLK_CITY_LIST_ORDER,

    //ID_ACC_WCLK_UNSET_DST,
    // end of menu ID for accessroy part

    ID_ACC_SDCARD_MANAGE_FORMAT,
    ID_ACC_SDCARD_MANAGE_STATUS,

    //Udisk,Start
    ID_UDISK_FORMAT,
    ID_UDISK_MEMORY,
    ID_UDISK_PCC_START,
    ID_UDISK_PCC_SETTINGS,
    ID_UDISK_PCC_BRIGHTNESS,
    ID_UDISK_PCC_CONTRAST,
    ID_UDISK_PCC_EFFECT,
    ID_UDISK_PCC_RESET_SETTINGS,
    //Udisk,End

    MMIACC_ID_MENU_MAX
}MMIACC_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIACC_MENU_LABEL_START = (MMI_MODULE_ACC << 16),

    #include "mmiacc_menutable.def"

    MMIACC_MENUTABLE_MAX
} MMIACC_MENU_LABEL_E;


#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTION
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIACC_MENUTABLE_H_
