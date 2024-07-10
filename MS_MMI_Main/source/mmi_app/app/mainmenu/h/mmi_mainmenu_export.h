/*****************************************************************************
** File Name:      mmimenu_export.h                                                 *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMI_MAINMENU_EXPORT_H_
#define _MMI_MAINMENU_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "mmi_mainmenu_data.h"
#include "mmitheme_mainmenu.h"

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
#ifdef DYNAMIC_MAINMENU_SUPPORT
#define DYNAMIC_MAINMENU_DEFAULT_GROUP_ID 0xffffffff//��̬�˵�Ĭ��group_id"-1"
#define DYNAMIC_MAINMENU_DEFAULT_MENU_ID 0xffffffff//��̬�˵�Ĭ��menu_id"-1"
#define DYNAMIC_MAINMENU_DEFAULT_CHECK_INFO 0xffffffff//��̬�˵�Ĭ��check_info"-1"
#define DYNAMIC_MAINMENU_DATA_PATH_MAX 255//��̬�˵�·�����ֵ
#define DYNAMIC_MAINMENU_DATA_FILE_LEN 16//��̬�˵��ļ������ֵ
#endif

#define MAINMENU_FILEFOLD_DEFAULT_GROUP_ID 0xfffffffe
 /**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT  //��ΪĬ�ϣ������
    GO_MENU_STYLE_CLASSIC = 0,
//#endif 
    GO_MENU_STYLE_RANDOM,
#ifdef GO_FLASH_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_FLASH,
#endif
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_BINARY_STAR,    /*!< ˫���Ƿ��  */
#endif
#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_SHUTTER,/*!< ��Ҷ�����  */ 
#endif
#ifdef GO_SPHERE_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_SPHERE, /*!< ���� */
#endif
#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_CYLINDER, /*!< ����� *///10
#endif
#ifdef GO_SLOPE_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_SLOPE,  /*б��*/
#endif
#ifdef GO_WHEEL_MAIN_MENU_SUPPORT
    GO_MENU_STYLE_WHEEL,   /*�����*/ 
#endif
#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_OCEANWAVE,
#endif    
#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_PEGTOP,
#endif    
#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_TURNTABLE,
#endif    
#ifdef GO_BOXROT_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_BOXROT,
#endif 
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_ICONBOXROT,
#endif 
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_BOOKPAGE,
#endif 
#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_MIDROTATE,
#endif 
#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_SMALLITEMS,
#endif
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_CONSTICK,
#endif
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_SYMMETRY,
#endif
#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT  
    GO_MENU_STYLE_BENZITEM,
#endif
	GO_MENU_STYLE_MAX
}GO_MENU_STYLE_E;

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
//�����Ч���
typedef enum
{
    SELECT_EFFECT_STATIC = 0,//��ֹ
    SELECT_EFFECT_ROUNDLIGHT, //���ƹ���Ч��
    SELECT_EFFECT_FIRE, //����Ч��
    SELECT_EFFECT_LIGHTING,//����Ч��
    SELECT_EFFECT_WATER,//ˮ
    SELECT_EFFECT_SNOW,//ѩ
    SELECT_EFFECT_RANDOM,//��̬���
    SELECT_EFFECT_MAX
}MMIMENU_SELECT_EFFECT_STYLE_E;//ͼ��ѡ��Ч��ö��

typedef enum
{
    SELECT_EFFECT_RENDER_FG = 0,//ѡ��Ч��ֻ��ǰ��
    SELECT_EFFECT_RENDER_BG,    //ѡ��Ч��ֻ����
    SELECT_EFFECT_RENDER_BOTH,  //ѡ��Ч��ǰ�󾰾���
    SELECT_EFFECT_RENDER_MAX
} MMIMENU_SELECT_EFFECT_RENDER_MODE_E;//ͼ��ѡ����ͼģʽö��

typedef struct MMIMENU_SELECT_EFFECT_INFO
{
    MMI_IMAGE_ID_T  select_image_id_begin;
    MMI_IMAGE_ID_T  select_image_id_end;
    MMI_IMAGE_ID_T  select_image_id_index;
    MMIMENU_SELECT_EFFECT_STYLE_E select_effect_style;
    MMIMENU_SELECT_EFFECT_RENDER_MODE_E select_effect_render_mode;
} MMIMENU_SELECT_EFFECT_INFO_T;

#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : creat main menu
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIMENU_CreatMainMenu(void);


#if defined MMI_ISTYLE_SUPPORT || defined MMI_GRID_IDLE_SUPPORT || defined MMI_SMART_IDLE_SUPPORT
/*****************************************************************************/
// 	Description : handle main menu win msg
//	Global resource dependence : 
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIMENU_HandleMainMenuWinMsg(
                                                    MMI_WIN_ID_T         win_id, 
                                                    MMI_MESSAGE_ID_E     msg_id, 
                                                    DPARAM               param
                                                    );
#endif

#ifdef FLASH_SUPPORT
PUBLIC void MMIAPIMENU_EnterEntertainment(void);
#endif

PUBLIC void MMIAPIMENU_EnterSetting(void);
PUBLIC void MMIAPIMENU_EnterTools(void);
PUBLIC void MMIAPIMENU_EnterPlayer(void);

#ifdef CMCC_UI_STYLE    
PUBLIC void MMIAPIMENU_EnterMyFavorite(void);
PUBLIC void MMIAPIMENU_EnterCmccService(void);
PUBLIC void MMIAPIMENU_EnterApps(void);
#endif

/*****************************************************************************/
// Description : close main menu
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMENU_UpdateMainMenu(void);


#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : ��ʾ��̬menuͼƬ
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_DisplayDynamicMenuIcon(
                            CTRLMAINMENU_ITEM_T *item_ptr, 
                            GUI_RECT_T *icon_rect_ptr,
                            GUI_RECT_T *disp_rect_ptr,
                            GUI_LCD_DEV_INFO *lcd_dev_ptr, 
                            MMI_WIN_ID_T win_handle
                            );                            
#endif


#if (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT) || ( defined MMI_AUTOTEST_SUPPORT )

/*****************************************************************************/
// 	Description : handle main menu id, called by idle.
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMENU_HandleIdleMenuItem(MMI_MENU_GROUP_ID_T  group_id, MMI_MENU_ID_T menu_id); 
#endif

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : handle i-style menu search msg 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIISTYLESEARCH_HandleSearchWinMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E     msg_id, 
                                                       DPARAM               param
                                                       );

/*****************************************************************************/
//  Description : is in istyle search or not 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMENU_IsInIsyleSearch( void );

/*****************************************************************************/
//  Description : release search page to save mem 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ReleaseIstyleSearch( void );

/*****************************************************************************/
//  Description : change page when click home key 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note:�����м����ҳ
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIMENU_HandleKeySlide(
                                              MMI_WIN_ID_T         win_id,
                                              MMI_MESSAGE_ID_E     msg_id,
                                              DPARAM               param
                                              );
#endif

#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_SetQbThemeIdleStyle(void);

/*****************************************************************************/
//  Description : read all menu item info.
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ReadQbthemeMenuItemInfo(
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 *num_ptr
);

/*****************************************************************************/
//  Description : ��ȡǧ���dockbar����
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMENU_ReadQbThemeDockbar(
    MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
);

/*****************************************************************************/
//  Description : ��дqb��dock����
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIMENU_ResetQbThemeDockbar(
    MMIMAINMENU_PAGE_ITEM_INFO_T *menu_info_ptr
);
#endif

#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ChangeGOMenuStyle(MMI_WIN_ID_T win_id);

#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ChangeQbThemeGOStyle(MMI_WIN_ID_T win_id);
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
