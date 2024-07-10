/*****************************************************************************
** File Name:      mmiset_mainmenu_export.h                                                  *
** Author:                                                                   *
** Date:           10/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
** 02/2012       xiyuan.ma           Edite
******************************************************************************/
#ifndef _MMISET__MAINMENU_EXPORT_H_
#define _MMISET__MAINMENU_EXPORT_H_ 

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT
#ifdef MMI_MULTI_MAINMENU_SUPPORT
#define MMISET_CreateMainmenuStyleWin        MMIAPISET_CreateMainmenuStyleWin
#endif
#endif //MMI_FUNC_COMPATIBLE_SUPPORT
#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
typedef enum
{
//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_CLASSIC = 0,
//#endif
    GO_MENU_STYLE_INDEX_RANDOM,
#ifdef GO_FLASH_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_FLASH,
#endif 
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_BINARY_STAR,
#endif
#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_SHUTTER,
#endif    
#ifdef GO_SPHERE_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_SPHERE,
#endif
#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_CYLINDER,
#endif 
#ifdef GO_SLOPE_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_SLOPE,
#endif 
#ifdef GO_WHEEL_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_INDEX_WHEEL,
#endif 
#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_OCEANWAVE,
#endif    
#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_PEGTOP,
#endif    
#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_TURNTABLE,
#endif    
#ifdef GO_BOXROT_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_BOXROT,
#endif 
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_ICONBOXROT,
#endif 
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_BOOKPAGE,
#endif 
#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_MIDROTATE,
#endif 
#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_SMALLITEMS,
#endif 
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_CONSTICK,
#endif 
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_SYMMETRY,
#endif 
#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_INDEX_BENZITEM,
#endif 
    GO_MENU_STYLE_INDEX_MAX
}GO_MENU_STYLE_INDEX_E;
#endif

// 屏锁和主菜单风格设置功能模块——xiaoqing
typedef enum
{
#ifdef MATRIX_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_ICON,
#endif
#ifdef MENU_CRYSTALCUBE_SUPPORT
    MMISET_PDA_MAINMENU_CRYSTAL_CUBE,
#endif

#ifdef MENU_CYCLONE_SUPPORT
    MMISET_PDA_MAINMENU_CYCLONE,
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
	MMISET_PDA_MAINMENU_GO,
#else
#ifdef SLIDE_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_SLIDE_PAGE,
#endif
#ifdef TRAPEZOID_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_TRAPEZOID,
#endif
#ifdef SPHERE_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_SPHERE,
#endif

#ifdef CYLINDER_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_CYLINDER,
#endif

#ifdef BINARY_STAR_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_BINARY_STAR,
#endif    
#ifdef SHUTTER_MAIN_MENU_SUPPORT
    MMISET_PDA_MAINMENU_SHUTTER,
#endif    
#ifdef CHARIOT_MAIN_MENU_SUPPORT 
    MMISET_PDA_MAINMENU_CHARIOT,
#endif
#endif
#ifdef MENU_SWEETS_SUPPORT
    MMISET_PDA_MAINMENU_SWEETS,
#endif
    MMISET_PDA_MAINMENU_MAX
}MMISET_PDA_MAINMENU_STYLE_INDEX_E;

#if defined(PDA_UI_SUPPORT_U_IDLE)
typedef enum
{
    UIDLE_STYLE_INDEX_CLASSIC = 0, // TXT_COMMON_CLASSIC
    UIDLE_STYLE_INDEX_RANDOM,
    UIDLE_STYLE_INDEX_SLOPE,  	 //	TXT_SET_MAINMENU_SLOPE
    UIDLE_STYLE_INDEX_BOXROT,		 // TXT_SET_MAINMENU_BOXROT
    UIDLE_STYLE_INDEX_BOOKPAGE,    //	TXT_SET_MAINMENU_BOOKPAGE
    UIDLE_STYLE_INDEX_MIDROTATE,   //	TXT_SET_MAINMENU_MIDROTATE
    UIDLE_STYLE_INDEX_SMALLITEMS,	 //	TXT_SET_MAINMENU_SMALLITEMS
    UIDLE_STYLE_INDEX_MAX
}UIDLE_STYLE_INDEX_E;
#endif
 #ifdef MMI_MULTI_MAINMENU_SUPPORT                               
/*****************************************************************************/
//  Description : main menu type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateMainmenuStyleWin(void);                                                                 

 #if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : main menu slect type set win  (highlight)
//  Global resource dependence : 
//  Author:kyle
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateMainmenuSelectStyleWin(void);
/*****************************************************************************/
//  Description : 获取选中BY INDEX
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIMENU_SELECT_EFFECT_STYLE_E MMIAPISET_GetMainmenuSelectStyleByIndex(
                                              uint16 item_index
                                              );

PUBLIC MMI_TEXT_ID_T  MMISET_GetMenuSelectStyleTxt(void);
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif
