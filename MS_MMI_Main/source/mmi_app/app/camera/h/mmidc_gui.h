
#ifndef _DC_MENU_ITEMS_H_
#define _DC_MENU_ITEMS_H_

#include "mmi_module.h"
#include "mmidc_setting.h"
#include "mmidc_display.h"

#include "mmidc_internal.h"

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

#if defined(MAINLCD_SIZE_320X480)
    #define MENU_ICON_SPACE 6
#elif defined(MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_240X400)
    #define MENU_ICON_SPACE 2
#else
    #define MENU_ICON_SPACE 0
#endif

typedef enum
{
    ID_CAMERA_MENU_START = (MMI_MODULE_CAMERA << 16),       
    
    ID_CAMERA_OPT_SEND,
       
    ID_CAMERA_OPT_DELETE,
    ID_CAMERA_OPT_WALLPAPER,
    ID_CAMERA_OPT_PHOTO_DIRECTORY,
#ifdef PIC_EDITOR_SUPPORT
    ID_CAMERA_OPT_PIC_EDIT,
#endif    
#if defined(MMS_SUPPORT)      
    ID_CAMERA_SEND_BY_MMS,
#endif    
#if defined(BLUETOOTH_SUPPORT)           
    ID_CAMERA_SEND_BY_BT,
#endif
#ifdef MMIEMAIL_SUPPORT
     ID_CAMERA_SEND_BY_EMAIL,
#endif
#ifdef SNS_SUPPORT
    ID_CAMERA_SHARE_TO_SINA,
    ID_CAMERA_SHARE_TO_FACEBOOK,
    ID_CAMERA_SHARE_TO_TIWTTER,
#endif
    ID_CAMERA_MENU_MAX
}MMICAMERA_ID_MENU_E;
    
    
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _TABLEID,
    
    typedef enum
    {
        MMICAMERA_MENU_LABEL_START = (MMI_MODULE_CAMERA << 16),
            
#include "mmidc_menu_wintable.def"
            
            MMICAMERA_MENUTABLE_MAX
    } MMICAMERA_MENU_LABEL_E;
    
#undef MENU_DEF
    
    /**---------------------------------------------------------------------------*
    **                         Compiler Flag                                     *
    **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*-------------------------------------------------------------------------*/

typedef void (*MENU_ITEM_CALL_BACK)(void);

typedef struct 
{
    void					(*MenuDisplay)(void);
    BOOLEAN					(*MenuHandleTpDown)(uint32 x, uint32 y);
    BOOLEAN					(*MenuHandleKeyDown)(DIRECTION_KEY key);
    void					(*MenuClose)(void);
    BOOLEAN					(*MenuIsOpen)(void);
    void					(*MenuSetDisplayNumber)(uint32 number);
    
    void					(*MenuSetStartPositionX)(uint32 x);
    void					(*MenuSetStartPositionY)(uint32 y);
    void					(*MenuSetMenuBackGroudColor)(uint32 color);
    void					(*MenuSetFocusItemBackGroundImage)(int32 image_data);
    void					(*MenuSetFocusItemBackGroundColor)(uint32 color);
    void					(*MenuAppendItemText)(int32 text_data);
    void					(*MenuSetItemText)(int32 index, int32 text_data);
    void					(*MenuSetMenuTitle)(int32 text_data);
    
    void					(*MenuFocusNextItem)(void);
    void					(*MenuFocusPreviousItem)(void);
    void					(*MenuSelectItem)(uint32 index);
    void					(*MenuFocusItem)(uint32 index);
    void					(*MenuSetItemCallback)(int32 index, MENU_ITEM_CALL_BACK call_back);
    void					(*MenuSetDisplayMode)(DISPLAY_DIRECT_E display_mode);
    void					(*MenuHideSelectedSign)(void);
    void					(*MenuSetNotCallbackFocus)(void);
    void                    (*MenuSetItemGray)(int32 index);
    void                    (*MenuClearRect)(void); 
}DC_MENU_ITEMS_OP;


typedef void (*SETTING_ITEM_CALL_BACK)(uint32 index);

/*****************************************************************************/
// 	Description : get menu handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_MENU_ITEMS_OP* MMIDC_InitMenu(void);

/*****************************************************************************/
// 	Description : get menu handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_MENU_ITEMS_OP* MMIDC_GetMenuHandle(void);

/*****************************************************************************/
// 	Description : init and get icon handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIAPIDC_InitIcon(void);

/*****************************************************************************/
// 	Description : get icon handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIDC_GetIconHandle(void);

/*****************************************************************************/
// 	Description : Register DC menu group
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RegMenuWin(void);

/*****************************************************************************/
// 	Description :  focused menu item is gray or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsFocusMenuGray(void);

#endif

