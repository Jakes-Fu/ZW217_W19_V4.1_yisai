/*****************************************************************************
** File Name:      mmifmm_menutable.h                                        *
** Author:                                                                   *
** Date:             06/04/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to file manager module                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       robert           Create
******************************************************************************/

#ifndef _MMIFMM_MENUTABLE_H_
#define _MMIFMM_MENUTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmi_filemgr.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
	MMIFMM_ID_MENU_START = (MMI_MODULE_FMM << 16),

	ID_FMM_OPEN,
	ID_FMM_NEW_FOLDER,
	ID_FMM_DELETE, 
	ID_FMM_RENAME,
	ID_FMM_COPY, 
	ID_FMM_MOVE,
	ID_FMM_DETAIL,
	ID_FMM_SORT,
	ID_FMM_SEARCH,
	ID_FMM_MARK, 
	ID_FMM_SUB_MARK,
	ID_FMM_CANCEL_MARK,
	ID_FMM_MARK_ALL,
	ID_FMM_CANCEL_ALL_MARK,

	ID_FMM_WALLPAPER, 
	ID_FMM_PB,
	ID_FMM_SEND, 

	ID_FMM_RING,
    ID_FMM_SET_ALARM_RING,

	ID_FMM_NAME,
	ID_FMM_DATE,
	ID_FMM_SIZE,
	ID_FMM_TYPE, 

	ID_FMM_SMS,
	ID_FMM_MMS,
	ID_FMM_BLUETOOTH,
	ID_FMM_MARK_OPT,
	ID_FMM_EDIT,
	ID_FMM_PREVIEW,
	ID_FMM_MANAGER,
	ID_FMM_PIC_MANAGER,
	ID_FMM_MUSIC_MANAGER,
#ifdef SCREENSAVER_SUPPORT
	ID_FMM_SCEENSAVE,
#endif
    ID_FMM_VIDEO_WALLPAPER,
	ID_FMM_POWER_ON_SET,
	ID_FMM_POWER_OFF_SET,
	ID_FMM_SAVE,
	ID_FMM_SIM1_RING,
	ID_FMM_SIM2_RING,
	ID_FMM_SIM3_RING,
	ID_FMM_SIM4_RING,
	ID_FMM_ALARM1_RING,
	ID_FMM_MOVIE_OPT_PLAY,
	ID_FMM_MOVIE_OPT_SELECT,
    ID_FMM_MEMORY_OPT,
    ID_FMM_UDISK_MEMORY,
    ID_FMM_MEMCARD_MEMORY_NUM = ID_FMM_UDISK_MEMORY + MMI_SDCARD_MAX_NUM,
    //ID_FMM_SDCARD1_MEMORY,
    ID_FMM_UDISK_FORMAT,   
    ID_FMM_MEMCARD_FORMAT_NUM = ID_FMM_UDISK_FORMAT + MMI_SDCARD_MAX_NUM,
    //ID_FMM_SDCARD1_FORMAT,

#ifdef DRM_SUPPORT
    ID_FMM_DRM_DOWNLOAD,
//    ID_FMM_DRM_DOWNLOAD_MORE,
#endif
    ID_FMM_EMAIL,
    ID_FMM_VCALENDAR_SMS,
    ID_FMM_SAVE_VCALENDAR,
    ID_FMM_LOAD_VCARD,
    ID_FMM_MEMCARD_RENAME,
    ID_FMM_MEMCARD1_RENAME,
//    ID_FMM_PHOTO_SET_PB,
	MMIFMM_ID_MENU_MAX
}MMIFMM_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIFMM_MENU_LABEL_START = (MMI_MODULE_FMM << 16),
    #include "mmifmm_menutable.def"
    MMIFMM_MENUTABLE_MAX
} MMIFMM_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register fmm menu group
//	Global resource dependence : none
//  Author: robert
//	Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
