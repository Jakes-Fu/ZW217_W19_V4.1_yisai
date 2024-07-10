/*************************************************************************
 ** File Name:      mmiim_nv.c                                           *
 ** Author:         jassmine                                             *
 ** Date:           2008/12/19                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/12/19    Jassmine           Create.                             *
*************************************************************************/

#define MMIIM_NV_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_modu_main.h"
#include "mmiim_nv.h"
#include "guiim_base.h"
#ifdef IM_ENGINE_SOGOU
#include "wdp_update_wintab.h"
#endif
#ifdef MMI_IME_USER_DATABASE
#include "mmiim_t9_base.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//the length of mmiim nv
LOCAL const uint16 mmiim_nv_len[] =
{
    sizeof(uint16), //MMINV_IM_PINYIN_FUZZY
    4,  //MMINV_IM_HW_SPEED
    4,  //MMINV_IM_HW_THICKNESS
    sizeof(GUI_COLOR_T), //MMINV_IM_HW_COLOR
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    4,      //MMINV_IM_HW_AREA_TYPE,      
#endif    
#ifdef MMI_IM_PDA_SUPPORT
    4, //MMINV_IM_KEYBOARD_SIZE
#endif
    sizeof(GUIIM_METHOD_T), //MMINV_IM_DEFAULT_METHOD
    sizeof(GUIIM_LANGUAGE_T), //MMINV_IM_LANGUAGE

#if defined MMI_IM_PDA_SUPPORT
    sizeof(GUIIM_LANGUAGE_T), //MMINV_IM_PREV_LANGUAGE,     //记录前一次非英语的语言
#endif

	sizeof(uint16),	//MMINV_IM_KEYBOARD_TYPE

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
	sizeof(VOCABULARY_UPDATE_FREQUENCY_E), //IMNV_SOGOU_UPDATE_FREQUENCY
	sizeof(SYSTEM_UPDATE_SETTINGS_DATA_T),  //IMNV_SOGOU_UPDATE_SETTING
	sizeof(SCI_TIME_T),         //IMNV_SOGOU_UPDATE_TIME
    sizeof(uint16),   /* Save virtual keyboard type */  //IMNV_SOGOU_UPDATE_EVENT_INDEX   
    sizeof(MMISG_SORT_TYPE_E)   //IMNV_SOGOU_SKIN_SORT_TYPE
#endif

    sizeof(uint16), //MMINV_IM_WORD_PREDIC

#ifdef MMI_IME_USER_DATABASE
    T9_UDB_SIZE,
#ifdef IM_SIMP_CHINESE_SUPPORT
    T9_CAUDB_SIZE,
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT  
    T9_CAUDB_SIZE,
#endif
#endif

};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register im module nv len and max item
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIIM_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_IM,mmiim_nv_len,sizeof(mmiim_nv_len)/sizeof(uint16));
}
