/*****************************************************************************
** File Name:      mmiacc_menutable.c                                        *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle ACC menutable                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     haiyagn.hu       Creat                                     *
******************************************************************************/

#define _MMIACC_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
//#include "sci_types.h"
#include "mmiacc_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmiacc_text.h"
#include "mmiacc_id.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiacc_image.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL 0
#define MENU_NULL NULL


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// menu for option of calculator app
const GUIMENU_ITEM_T menu_calc_option[] =
{   
    {ID_ACC_CALC_MENU_MPLUS,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CALC_MPLUS,  0, 0, 0, MENU_NULL},
    {ID_ACC_CALC_MENU_MMINUS,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CALC_MSUB,  0, 0, 0, MENU_NULL},
    {ID_ACC_CALC_MENU_MRECALL,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CALC_MR,  0, 0, 0, MENU_NULL},
	{ID_ACC_CALC_MENU_MCLEAR,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CALC_MCLEAR,  0, 0, 0, MENU_NULL},
	{ID_ACC_CALC_MENU_CLEAR,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_CLEAR,  0, 0, 0, MENU_NULL}
};

#ifdef WORLD_CLOCK_SUPPORT
// menu for option of world clock app
const GUIMENU_ITEM_T menu_wclk_dst_option[] =
{   
#ifdef MMI_PDA_SUPPORT
    {ID_ACC_WCLK_SET_ADD_CITY,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_ADD_CITY,  IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, MENU_NULL},
    {ID_ACC_WCLK_SET_DELETE_CITY,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_DELETE_CITY,  IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, MENU_NULL},
    {ID_ACC_WCLK_SET_LOCAL1,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_SET_LOCAL,  IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, IMAGE_WT_OPTION_SET_LOCAL, MENU_NULL},
    {ID_ACC_WCLK_SET_DST,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_SET_DST, IMAGE_WT_OPTION_SET_SUMMER, IMAGE_WT_OPTION_SET_SUMMER, IMAGE_WT_OPTION_SET_SUMMER, MENU_NULL}
#else
    {ID_ACC_WCLK_SET_ADD_CITY,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_ADD_CITY,  0, 0, 0, MENU_NULL},
    {ID_ACC_WCLK_SET_DELETE_CITY,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_DELETE_CITY, 0, 0, 0, MENU_NULL},
    {ID_ACC_WCLK_SET_LOCAL1,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_SET_LOCAL,  0, 0, 0, MENU_NULL},
    {ID_ACC_WCLK_SET_DST,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_SET_DST, 0, 0, 0, MENU_NULL}

#endif
};

const GUIMENU_ITEM_T menu_wclk_city_list_option[] =
{   
    {ID_ACC_WCLK_CITY_LIST_ORDER,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_ACC_WORLD_CITY_LIST_ORDER,  0, 0, 0, MENU_NULL},
};
#endif

//SDcard management
const GUIMENU_ITEM_T	menu_multim_sdcard_manage [] =
{
	{ID_ACC_SDCARD_MANAGE_FORMAT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MMIMULTIM_SDCARD_FORMAT, 0, 0, 0, MENU_NULL},
	{ID_ACC_SDCARD_MANAGE_STATUS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MMIMULTIM_SDCARD_STATUS, 0, 0, 0, MENU_NULL}
	
 };

const GUIMENU_ITEM_T menu_udisk[] =
{
    {ID_UDISK_FORMAT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIMULTIM_SDCARD_FORMAT, 0, 0, 0, MENU_NULL},
    {ID_UDISK_MEMORY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_UDISK_MEMORY, 0, 0, 0, MENU_NULL},
};


const GUIMENU_ITEM_T menu_upcc[] =
{
    {ID_UDISK_PCC_START, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_UDISK_PCC_START, 0, 0, 0, MENU_NULL},
//    {ID_UDISK_PCC_SETTINGS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_UDISK_PCC_SETTING, 0, 0, 0, MENU_UPCC_SETTINGS},
};

const GUIMENU_ITEM_T menu_upcc_settings[] =
{
    {ID_UDISK_PCC_BRIGHTNESS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_COMMON_BRIGHTNESS, 0, 0, 0, MENU_NULL},
    {ID_UDISK_PCC_CONTRAST, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMM_CONTRAST, 0, 0, 0, MENU_NULL},
    {ID_UDISK_PCC_EFFECT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_EFFECT, 0, 0, 0, MENU_NULL},
    {ID_UDISK_PCC_RESET_SETTINGS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_RESET_DEFAULT, 0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T menu_phonesearch_option[] =
{
    {MMI_PHONESEARCH_POP_QUERY_MENU_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_INQUIRE, 0, 0, 0, MENU_NULL},

#if defined(BROWSER_SUPPORT) && defined(HOME_LOCATION_INQUIRE_SUPPORT_TCARD)
    {MMI_PHONESEARCH_POP_DOWNLOAD_MENU_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SEARCHPHONE_DOWNLOAD_PHONENUM_FILE, 0, 0, 0, MENU_NULL},
#endif
};
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T acc_menu_table[] = 
{
    {NULL},
    #include "mmiacc_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_ACC, acc_menu_table);
}

