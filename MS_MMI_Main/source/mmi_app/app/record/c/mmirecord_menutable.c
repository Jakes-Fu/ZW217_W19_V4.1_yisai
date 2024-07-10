/*****************************************************************************
** File Name:	   mmirecord_menutable.c									 *
** Author:																	 *
** Date:		   2006/09/18												 *
** Copyright:	   2006 Spreadtrum, Incorporated. All Rights Reserved.		 *
** Description:    This file is used to handle RECORD menutable 			 *
******************************************************************************
**						   Important Edit History							 *
** --------------------------------------------------------------------------*
** DATE 		  NAME			   DESCRIPTION								 *
** 2006/09/18	  haiyagn.hu	   Creat									 *
******************************************************************************/

#define _MMIRECORD_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 ** 						Include Files									 *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmirecord_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmirecord_text.h"
#include "mmirecord_id.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_text.h"
#endif
#include "mmimms_text.h"
#include "mmirecord_image.h"

/**--------------------------------------------------------------------------*
 ** 						MACRO DEFINITION								 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 ** 						STATIC DEFINITION								 *
 **--------------------------------------------------------------------------*/
#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
const GUIMENU_ITEM_T menu_record_start_option[] =
{
    {ID_RECORD_START,			 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CC_STARTRECORD,  0, 0, 0, MENU_NULL},	 //≤•∑≈¬º“Ù
    {ID_RECORD_STOPPED_PLAY,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_MULTIM_AUDITION,  0, 0, 0, MENU_NULL},	//≤•∑≈¬º“Ù
    {ID_RECORD_AGAIN,			 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_AGAIN,  0, 0, 0,	MENU_NULL},    //œÍ«È
    {ID_RECORD_DELETE,			 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_DELETE,  0, 0, 0,  MENU_NULL},	//delete
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
    {ID_RECORD_SEND,			 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_SEND,  0, 0, 0,  MENU_MULTIM_SEND_FILE},	  //send
#endif
    {ID_RECORD_INSERT,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_INSERT,  0, 0, 0,  MENU_NULL},//±‡º≠≤Â»Îrecordwork
    {ID_RECORD_LIST,		 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_RECORD_FILE_LIST,  0, 0, 0,	MENU_NULL},    //œÍ«È
    {ID_RECORD_STORIGE_POS, 	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   STXT_STORE_MEDIUM,  0, 0, 0,  MENU_RECORD_STORE_STATUS},//andy.he_record  format style
    {ID_RECORD_STORIGE_FORMAT, 	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_FILE_FORMAT,  0, 0, 0,  MENU_RECORD_STORE_FORMAT}//andy.he_record  format style

};
#else
const GUIMENU_ITEM_T menu_record_start_option[] =
{
    {ID_RECORD_START,			 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_CC_STARTRECORD,  0, 0, 0, MENU_NULL},	 //≤•∑≈¬º“Ù
    {ID_RECORD_STOPPED_PLAY,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_MULTIM_AUDITION,  0, 0, 0, MENU_NULL},	//≤•∑≈¬º“Ù
    {ID_RECORD_AGAIN,			 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_AGAIN,  IMAGE_RECORD_RECORD_AGAIN, 0, 0,	MENU_NULL},    //œÍ«È
    {ID_RECORD_DELETE,			 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_DELETE,  IMAGE_COMMON_OPTMENU_DELETE, 0, 0,  MENU_NULL},	//delete
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
    {ID_RECORD_SEND,			 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_SEND,  IMAGE_COMMON_OPTMENU_SEND, 0, 0,  MENU_MULTIM_SEND_FILE},	  //send
#endif
    {ID_RECORD_INSERT,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_INSERT,  IMAGE_COMMON_OPTMENU_SET, 0, 0,  MENU_NULL},//±‡º≠≤Â»Îrecordwork
    {ID_RECORD_LIST,		 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_RECORD_FILE_LIST,  0, 0, 0,	MENU_NULL},    //œÍ«È
    {ID_RECORD_STORIGE_POS, 	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   STXT_STORE_MEDIUM,  IMAGE_RECORD_STORAGE, 0, 0,  MENU_RECORD_STORE_STATUS},//andy.he_record  format style
    {ID_RECORD_STORIGE_FORMAT, 	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_FILE_FORMAT,  IMAGE_COMMON_OPTMENU_SET, 0, 0,  MENU_RECORD_STORE_FORMAT}//andy.he_record  format style

};
#endif //MMI_PDA_SUPPORT


const GUIMENU_ITEM_T menu_record_store_status[] =
{
    {ID_RECORD_UDISK,			 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_UDISK,  0, 0, 0, MENU_NULL},
#ifndef	 DUAL_TCARD_SUPPORT
	{ID_RECORD_UDISK +1,	 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD,  0, 0, 0,  MENU_NULL},
#else
	{ID_RECORD_UDISK +1,	 TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD0,  0, 0, 0,  MENU_NULL},
    {ID_RECORD_UDISK + 2,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD1,  0, 0, 0,  MENU_NULL},
#endif
};

/*------------andy.he_record  format style-----------------*/
const GUIMENU_ITEM_T menu_record_store_format[] =
{

    {ID_RECORD_STORIGE_FORMAT_AMR, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_FORMATAMR,  0, 0, 0,  MENU_NULL},
    {ID_RECORD_STORIGE_FORMAT_MP3,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_FORMATMP3,  0, 0, 0,  MENU_NULL},
    {ID_RECORD_STORIGE_FORMAT_WAV, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_RECORD_FORMATWAV,  0, 0, 0, MENU_NULL}

};

/*------------end andy.he_record  format style-------------*/


#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
const GUIMENU_ITEM_T	menu_record_send_file [] =
{
#ifdef MMS_SUPPORT
    {ID_RECORD_SEND_BY_MMS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_MMS, 0, 0, 0, MENU_NULL},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_RECORD_SEND_BY_BT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BLUETOOTH, 0, 0, 0, MENU_NULL}
#endif
};
#endif

/*---------------------------------------------------------------------------*/
/*							TYPE AND CONSTANT								 */
/*---------------------------------------------------------------------------*/


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
                 _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT,

const GUIMENU_GROUP_T record_menu_table[] =
{
    {NULL},
#include "mmirecord_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//	Description : Register record menu group
//	Global resource dependence : none
//	Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_RECORD, record_menu_table);
}

