/*****************************************************************************
** File Name:      mmiset_lock_effect.c                                      *
** Author:                                                                   *
** Date:           09/20/2011                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004       mary.xiao      Create
******************************************************************************/

#define _MMISET_LOCK_EFFECT_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "window_parse.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_text.h"
#include "guiform.h"
#include "guisetlist.h"
#include "guilabel.h"
#include "mmikl_export.h"
#include "mmiset_func.h"
#include "mmiset_lock_effect_internal.h"
#include "mmiset_menutable.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
#include "mmikl_drawgraphic.h"
#endif
#include "mmiset_image.h"
#include "mmiset_menutable.h"
#include "mmiset_display.h"
#include "mmi_nv.h"

#define TIP_NULL 0

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle screen lock type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleScreenLockTypeSetWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                  );
/*****************************************************************************/
//  Description : Append screen lock type list item.
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/ 
LOCAL void AppendScreenLockTypeListItems(MMI_CTRL_ID_T ctrl_id);
#endif
/*****************************************************************************/
//  Description : handle screen lock preview win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
//LOCAL MMI_RESULT_E  HandleScreenLockTypePreviewWinMsg(
//                                                      MMI_WIN_ID_T        win_id, 
//                                                      MMI_MESSAGE_ID_E    msg_id, 
//                                                      DPARAM              param
//                                                       ); 

#if 0
/*****************************************************************************/
// 	Description : Handle key lock effect setting.
//	Global resource dependence : 
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetKeyLockEffectWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
#endif
/*****************************************************************************/
// 	Description : MMIAPISET_SetKeyLockEffectLableStr
//	Global resource dependence : none
//  Author: juanwu
//	Note:
/*****************************************************************************/
LOCAL void SetKeyLockEffectLableStr(void);

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
/*****************************************************************************/
// 	Description : Handle key lock effect5 submenu setting.
//	Global resource dependence : 
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetKeyLockEffect5SubWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
#if defined MMISET_KEYLOCK_SUPPORT
#if 0
WINDOW_TABLE( MMISET_SET_KEY_LOCK_EFFECT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetKeyLockEffectWindow ),    
        WIN_ID( MMISET_SET_KEY_LOCK_EFFECT_WIN_ID ),
        WIN_TITLE( TXT_SET_KEY_LOCK_EFFECT_TITLE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN    
};
#endif
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
WINDOW_TABLE( MMISET_SET_KEY_LOCK_EFFECT5_SUB_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSetKeyLockEffect5SubWindow ),    
        WIN_ID( MMISET_SET_KEY_LOCK_EFFECT5_SUB_WIN_ID ),
        WIN_TITLE( TXT_SET_KEY_LOCK_EFFECT_TITLE ),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_KEY_LOCK_EFFECT5_FORM_CTRL_ID),
#if 1
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_SET_ENABLE_PATTERN_UNLOCK_FORM_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT5_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_LABEL_CTRL_ID,MMISET_DISPLAY_SET_ENABLE_PATTERN_UNLOCK_FORM_CTRL_ID),//Enable pattern unlock
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID,MMISET_DISPLAY_SET_ENABLE_PATTERN_UNLOCK_FORM_CTRL_ID),//Enable pattern unlock
#endif
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_SET_CHANGE_PATTERN_UNLOCK_FORM_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT5_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE, MMISET_DISPLAY_CHANGE_PATTERN_UNLOCK_LABEL_CTRL_ID,MMISET_DISPLAY_SET_CHANGE_PATTERN_UNLOCK_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_SET_VISIBLE_PATTERN_UNLOCK_FORM_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT5_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_LABEL_CTRL_ID,MMISET_DISPLAY_SET_VISIBLE_PATTERN_UNLOCK_FORM_CTRL_ID),//VISIBLE KEYPAD LOCK
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID,MMISET_DISPLAY_SET_VISIBLE_PATTERN_UNLOCK_FORM_CTRL_ID),//VISIBLE KEYPAD LOCK
                                                    
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#endif   

//#if 0
//// screen lock type set win table
//WINDOW_TABLE( MMISET_SCREENLOCK_TYPE_WIN_TAB ) = 
//{
//    //CLEAR_LCD,
//    //WIN_PRIO( WIN_ONE_LEVEL ),
//    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
//    WIN_FUNC( (uint32)HandleScreenLockTypeSetWinMsg ),    
//    WIN_ID( MMISET_SET_SCREENLOCK_TYPE_WIN_ID ),
//    WIN_TITLE(TXT_NULL),
//    // WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//    CREATE_ICONLIST_CTRL(MMISET_MAINMENU_TYPE_CTRL_ID),
//    END_WIN
//};
// #else
// screen lock type set win table
WINDOW_TABLE( MMISET_SCREENLOCK_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    //WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_FUNC( (uint32)HandleScreenLockTypeSetWinMsg ),    
    WIN_ID( MMISET_SET_SCREENLOCK_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_KEYLOCK_TYPE_SELECT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //CREATE_MENU_CTRL(MENU_LOCK_EFFECT_SETTYPE,MMISET_MAINMENU_TYPE_CTRL_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_DISPLAY_SCREEN_LOCK_TYPE_LIST_CTRL_ID),
    END_WIN
};
#endif
//#endif
// screen lock type set win table
//WINDOW_TABLE( MMISET_SCREENLOCK_TYPE_PREVIEW_WIN_TAB ) = 
//{
//    //CLEAR_LCD,
//    //WIN_PRIO( WIN_ONE_LEVEL ),
//    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
//    WIN_FUNC( (uint32)HandleScreenLockTypePreviewWinMsg ),    
//    WIN_ID( MMISET_SET_SCREENLOCK_PREVIEW_TYPE_WIN_ID ),
//    WIN_TITLE(TXT_NULL),
//    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),
//    END_WIN
// };

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
const GUIMENU_ITEM_T menu_lock_effect_settype[] = 
{ 
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
#ifdef MMI_KEYLOCK_MINI_SUPPORT
    {ID_SET_KEYLOCK_LONG_TP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KEYLOCK_NORMAL_UNLOCK_TIP, IMAGE_NULL, IMAGE_NULL, 0, NULL},
#else
#ifdef TOUCHPANEL_TYPE
    {ID_SET_KEYLOCK_LONG_TP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KEYLOCK_UNLOCK_TIP, IMAGE_SET_ICON_SCREEN_LOCK_0, IMAGE_SET_ICON_SCREEN_LOCK_0, 0, NULL},
#else
    {ID_SET_KEYLOCK_LONG_TP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KEYLOCK_NORMAL_UNLOCK_TIP, IMAGE_SET_ICON_SCREEN_LOCK_0, IMAGE_SET_ICON_SCREEN_LOCK_0, 0, NULL},
#endif
#endif
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT  
    {ID_SET_KEYLOCK_SLIDEDOWN, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_SLIDEDOWN, IMAGE_SET_ICON_SCREEN_LOCK_1, IMAGE_SET_ICON_SCREEN_LOCK_1, 0, NULL},
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT  
    {ID_SET_KEYLOCK_SLIDE_SCROLLKEY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_SLIDE_SCROLLKEY, IMAGE_SET_ICON_SCREEN_LOCK_2, IMAGE_SET_ICON_SCREEN_LOCK_2, 0, NULL},
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT  
    {ID_SET_KEYLOCK_SMART_SLIDEDOWN, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_SMART_SLIDEDOWN, IMAGE_SET_ICON_SCREEN_LOCK_3, IMAGE_SET_ICON_SCREEN_LOCK_3, 0, NULL},
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT  
    {ID_SET_KEYLOCK_SLIDE_IPHONE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_SLIDE_IPHONE, IMAGE_SET_ICON_SCREEN_LOCK_4, IMAGE_SET_ICON_SCREEN_LOCK_4, 0, NULL},
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT  
    {ID_SET_KEYLOCK_GRAPHIC,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_GRAPHIC, IMAGE_SET_ICON_SCREEN_LOCK_5, IMAGE_SET_ICON_SCREEN_LOCK_5, 0, NULL},
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT  
    {ID_SET_KEYLOCK_ICON_RING,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_ICON_RING,   IMAGE_SET_ICON_SCREEN_LOCK_6, IMAGE_SET_ICON_SCREEN_LOCK_6, 0, NULL},
#endif 
#ifdef MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT  
    {ID_SET_KEYLOCK_FRUIT_SPLIT,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_FRUIT_SPLIT,   IMAGE_SET_ICON_SCREEN_LOCK_FRUIT, IMAGE_SET_ICON_SCREEN_LOCK_FRUIT, 0, NULL},
#endif 
    //michael wang kl
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT  
    {ID_SET_KEYLOCK_DOUBLE_CIRCLE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_DOUBLE_CIRCLE,   IMAGE_SET_ICON_SCREEN_LOCK_CIRCLE, IMAGE_SET_ICON_SCREEN_LOCK_CIRCLE, 0, NULL},
#endif 
    //michael wang
    //spec
#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT  
    {ID_SET_KEYLOCK_DOUBLE_CIRCLE_SPEC,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_KEY_LOCK_DOUBLE_CIRCLE_SPEC,   IMAGE_SET_ICON_SCREEN_LOCK_SPECCIRCLE, IMAGE_SET_ICON_SCREEN_LOCK_SPECCIRCLE, 0, NULL},
#endif 
#ifdef MMI_KEY_LOCK_EFFECT_QQVGA  
    {ID_SET_KEYLOCK_LONG_TP_QQVGA, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KEYLOCK_UNLOCK_TIP, IMAGE_NULL, IMAGE_NULL, 0, NULL},
#endif 
};
#endif

/*****************************************************************************/
//  Description : lock type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateLockStyleWin(void) /*lint !e18*/
{
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
    MMK_CreateWin((uint32 *)MMISET_SCREENLOCK_TYPE_WIN_TAB,PNULL);
#endif
}

#if defined MMISET_KEYLOCK_SUPPORT
#if 0
/*****************************************************************************/
//  Description : lock  set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_CreateKeyLockEffectWin(void)
{
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
    MMK_CreatePubListWin((uint32*)MMISET_SET_KEY_LOCK_EFFECT_WIN_TAB,PNULL);
#endif
}
#endif
#endif
/*****************************************************************************/
//  Description : handle screen lock type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
//#if 0
//LOCAL MMI_RESULT_E  HandleScreenLockTypeSetWinMsg(
//                                                  MMI_WIN_ID_T        win_id, 
//                                                  MMI_MESSAGE_ID_E    msg_id, 
//                                                  DPARAM            param
//                                                  )
//{
//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
//    uint16       item_index = 0;
//    GUIANIM_DATA_INFO_T             data_info = {0};
//    MMI_CTRL_ID_T ctrl_id = MMISET_MAINMENU_TYPE_CTRL_ID;
//
//    SCI_TRACE_LOW("HandleScreenLockTypeSetWinMsg, msg_id = 0x%x", msg_id);
//    
//    switch (msg_id)
//    {
//    case MSG_OPEN_WINDOW:
//        GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_EFFECT_TITLE, FALSE);
//        GUIICONLIST_SetTotalIcon(ctrl_id,MMISET_KEY_LOCK_EFFECT_MAX);
//        GUIICONLIST_SetMaxSelectedItem(ctrl_id, 1);
//        GUIICONLIST_SetIconWidthHeight(ctrl_id, MMISET_SCREEN_LOCK_ICON_WIDTH, MMISET_SCREEN_LOCK_ICON_HEIGHT);
//        GUIICONLIST_SetLoadType(ctrl_id,GUIICONLIST_LOAD_ALL);
//
//        item_index = MMIAPISET_GetKeyLockEffect();
//        GUIICONLIST_SetSelectedItem(ctrl_id, item_index, TRUE);
//        GUIICONLIST_SetCurIconIndex(item_index,ctrl_id);
//        MMK_SetAtvCtrl(win_id,ctrl_id);
//        break;
//
//    case MSG_CTL_ICONLIST_APPEND_ICON:
//        if (PNULL != param)
//        {
//            //get pic index and item index
//            item_index = *((uint16 *)param);
//            
//            if (item_index < MMISET_KEY_LOCK_EFFECT_MAX)
//            {
//                data_info.img_id = s_mmiset_screenlock_style[item_index][0];
//                
//                //append icon
//                GUIICONLIST_AppendIcon(item_index, ctrl_id, &data_info, NULL);
//            }
//        }
//        break;
//        
//    case MSG_FULL_PAINT:
//        {
//            uint16 sel_index = 0;
//            GUIICONLIST_GetSelectedItemIndex(ctrl_id, &sel_index, 1);
//            GUIICONLIST_SetSelectedItem(ctrl_id, sel_index, FALSE);
//            item_index = MMIAPISET_GetKeyLockEffect();
//            GUIICONLIST_SetSelectedItem(ctrl_id, item_index, TRUE);
//        }
//        break;
//
//    case MSG_CTL_MIDSK:
//        break;
//
//    case MSG_APP_WEB:
//    case MSG_CTL_OK:
//    case MSG_CTL_PENOK:
//    case MSG_APP_OK:
//        {
//            uint16 cur_item_index = 0;
//            cur_item_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
//            MMK_CreateWin((uint32 *)MMISET_SCREENLOCK_TYPE_PREVIEW_WIN_TAB,(ADD_DATA)cur_item_index);
//        }
//        break;
//
//    case MSG_CTL_CANCEL:    
//    case MSG_APP_CANCEL:
//        MMK_CloseWin(win_id);
//        break;
//        
//    default:
//        recode = MMI_RESULT_FALSE;
//        break;
//    }
//   
//    return recode;
//}
// #else
/*****************************************************************************/
//  Description : 把MENU ID转化为SET LOCK EFFECT
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
// LOCAL MMISET_KEY_LOCK_EFFECT_E  GetLockEffectIndexByMenuID(MMI_MENU_ID_T menu_id)
// {
//     
//     MMISET_KEY_LOCK_EFFECT_E lock_type = MMISET_KEY_LOCK_EFFECT_MAX;
//     switch(menu_id)
//     {
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT  
//         case ID_SET_KEYLOCK_LONG_TP:
//             lock_type = MMISET_KEY_LOCK_EFFECT0;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT  
//         case ID_SET_KEYLOCK_SLIDEDOWN:
//             lock_type = MMISET_KEY_LOCK_EFFECT1;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT  
//         case ID_SET_KEYLOCK_SLIDE_SCROLLKEY:
//             lock_type = MMISET_KEY_LOCK_EFFECT2;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT  
//         case ID_SET_KEYLOCK_SMART_SLIDEDOWN:
//             lock_type = MMISET_KEY_LOCK_EFFECT3;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT  
//         case ID_SET_KEYLOCK_SLIDE_IPHONE:
//             lock_type = MMISET_KEY_LOCK_EFFECT4;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT  
//         case ID_SET_KEYLOCK_GRAPHIC:
//             lock_type = MMISET_KEY_LOCK_EFFECT5;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT  
//         case ID_SET_KEYLOCK_ICON_RING:
//             lock_type = MMISET_KEY_LOCK_EFFECT6;
//             break;     
// #endif
// #ifdef MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT  
//         case ID_SET_KEYLOCK_FRUIT_SPLIT:
//             lock_type = MMISET_KEY_LOCK_FRUIT_SPLIT;
//             break;
// #endif
// #ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
//         case ID_SET_KEYLOCK_DOUBLE_CIRCLE:
//             lock_type = MMISET_KEY_LOCK_DOUBLE_CIRCLE;
//             break;
// #endif    
//         default:
//             break;
//     }
//     return lock_type;
//     
// }
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle screen lock type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: Modified by leon.wang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleScreenLockTypeSetWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_DISPLAY_SCREEN_LOCK_TYPE_LIST_CTRL_ID;
    MMISET_KEY_LOCK_EFFECT_E    lock_type = MMISET_KEY_LOCK_EFFECT_MAX;
    
    //SCI_TRACE_LOW:"HandleScreenLockTypeSetWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_LOCK_EFFECT_335_112_2_18_2_51_17_232,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        AppendScreenLockTypeListItems(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        lock_type = MMIAPISET_GetKeyLockEffect();
        if( lock_type >= MMISET_KEY_LOCK_EFFECT_MAX)    
        {
            lock_type = 0;
        }    
        GUILIST_SetCurItemIndex(ctrl_id, (uint16)lock_type);
        GUILIST_SetSelectedItem(ctrl_id, (uint16)lock_type, TRUE);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
  //  case MSG_APP_MENU:
        {
            uint16  item_index = 0;
            item_index = GUILIST_GetCurItemIndex(ctrl_id);
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_OpenPreviewLockPhone(item_index);
#endif
        }     
        break;

    case MSG_GET_FOCUS:
        break;
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}


/*****************************************************************************/
//  Description : Append screen lock type list item.
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/ 
LOCAL void AppendScreenLockTypeListItems(MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_T      item = {0}; 
    uint32              totalNum = MMISET_KEY_LOCK_EFFECT_MAX;
    uint16              index = 0;   

    item.item_style = GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT;
    item_data.softkey_id[0] = TXT_COMMON_OK; 
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item.item_data_ptr = &item_data;
    
    GUILIST_SetMaxItem(ctrl_id, MMISET_KEY_LOCK_EFFECT_MAX, FALSE);
    for(index = 0; index < totalNum; index++)
    {         
        item_data.item_content[0].item_data.image_id = menu_lock_effect_settype[index].unselect_icon_id;        
        item_data.item_content[1].item_data.text_id = menu_lock_effect_settype[index].text_str_id;        
        GUILIST_AppendItem(ctrl_id, &item);
    }    
}
#endif

//#endif
/*****************************************************************************/
//  Description : handle screen lock preview win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
//LOCAL MMI_RESULT_E  HandleScreenLockTypePreviewWinMsg(
//                                                      MMI_WIN_ID_T        win_id, 
//                                                      MMI_MESSAGE_ID_E    msg_id, 
//                                                      DPARAM              param
//                                                      )
//{
//    uint32                  image_id        = 0;
//    GUI_RECT_T              client_rect     = {0};
//    MMI_RESULT_E            recode          = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T           ctrl_id         = MMISET_MAINMENU_TYPE_CTRL_ID;
//    GUI_LCD_DEV_INFO        lcd_dev_info    = {0};
//    GUIMENU_STYLE_E         mainmenu_style  = GUIMENU_STYLE_ICON;
//    uint16                  tmp_item_index = 0;
//    static uint16           item_index = 0;
//    GUI_POINT_T             tp_point = {0};
//
//    SCI_TRACE_LOW("HandleScreenLockTypePreviewWinMsg, msg_id = 0x%x", msg_id);
//    
//    switch (msg_id)
//    {
//    case MSG_OPEN_WINDOW:
//        item_index = 0;
//        if(NULL != (uint32)MMK_GetWinAddDataPtr(win_id))
//        {
//            item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
//        }
//        switch(item_index)
//        {
//           #ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT  
//            case MMISET_KEY_LOCK_EFFECT0:
//            GUIWIN_SetTitleTextId(win_id, TXT_KEYLOCK_UNLOCK_TIP, FALSE);
//            break;
//           #endif
//
//           #ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT1:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_SLIDEDOWN, FALSE);
//            break;
//           #endif
//
//           #ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT2:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_SLIDE_SCROLLKEY, FALSE);
//            break;
//           #endif
//
//           #ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT3:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_SMART_SLIDEDOWN, FALSE);
//            break;
//           #endif
//
//           #ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT4:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_SLIDE_IPHONE, FALSE);
//            break;
//            #endif
//            
//            #ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT5:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_GRAPHIC, FALSE);
//            break;
//            #endif
//            
//            #ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT  
//           case MMISET_KEY_LOCK_EFFECT6:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_ICON_RING, FALSE);
//            break;
//            #endif
//            
//			#ifdef  MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT
//            case MMISET_KEY_LOCK_FRUIT_SPLIT:
//            GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_FRUIT_SPLIT, FALSE);
//            break;
//            #endif
//
//            default:
//             GUIWIN_SetTitleTextId(win_id, TXT_SET_KEY_LOCK_EFFECT_TITLE, FALSE);
//               break;
//        }
//        break;
//
//    case MSG_FULL_PAINT:
//        {
//            uint16 image_width = 0;
//            uint16 image_height = 0;
//            GUI_RECT_T disp_rect = {0};
//            GUI_RECT_T image_rect = {0};
//
//            client_rect = MMITHEME_GetClientRectEx(win_id);
//
//            MMITHEME_DisplayCommonBg(&client_rect, win_id, &lcd_dev_info);
//            
//            MainmenuStylePreviewDispImage(item_index, 0, client_rect, win_id);
//        }
//        break;
//
//    case MSG_TIMER:
//        if (IsMainMenuRedrawTimer(*(uint8*)param))
//        {
//            int16   lcd_pos = 0;
//            BOOLEAN result = TRUE;
//            MMI_TP_STATUS_E state = MMI_TP_NONE;
//
//            StopSetingRedrawTimer(win_id);
//            StartSetingRedrawTimer(win_id);
//
//            MMK_GetLogicTPMsg(&state, &tp_point);
//
//            if (MoveSetingPreview(tp_point, item_index, MMISET_KEY_LOCK_EFFECT_MAX, win_id))
//            {
//                SetMainmenuPrePoint(tp_point.x, tp_point.y);
//            }
//        }
//        else
//        {
//            recode = MMI_RESULT_FALSE;
//        }
//        break;
//
//    case MSG_LOSE_FOCUS:
//        StopSetingRedrawTimer(win_id);
//        break;
//
//    case MSG_TP_PRESS_DOWN:
//        SetMainmenuStartPoint(MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
//        SetMainmenuPrePoint(MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
//        StartSetingRedrawTimer(win_id);
//        break;
//        
//    case MSG_TP_PRESS_MOVE:
//        break;
//        
//    case MSG_TP_PRESS_UP:
//        {
//            StopSetingRedrawTimer(win_id);
//
//            tp_point.x = MMK_GET_TP_X(param);
//            tp_point.y = MMK_GET_TP_Y(param);
//
//            item_index = ResumeSetingPreview(tp_point, item_index, MMISET_KEY_LOCK_EFFECT_MAX, win_id);
//        }
//        break;
//        
//    case MSG_APP_MENU:
//        break;
//
//    case MSG_APP_WEB:
//    case MSG_CTL_PENOK:
//    case MSG_APP_OK:
//#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
//        if(MMISET_KEY_LOCK_EFFECT5 == item_index)
//        {
//            MMK_CreateWin((uint32*)MMISET_SET_KEY_LOCK_EFFECT5_SUB_WIN_TAB,PNULL);
//            MMK_CloseWin( win_id ); 
//        }
//        else
//#endif
//        {
//            if (item_index != MMIAPISET_GetKeyLockEffect())
//            {
//                //set main menu style
//                MMIAPISET_SetKeyLockEffect((MMISET_KEY_LOCK_EFFECT_E)item_index);
//            }
//        }
//        MMK_CloseWin(win_id); // 关闭窗口
//        StopSetingRedrawTimer(win_id);
//        break;
//        
//    case MSG_CLOSE_WINDOW:
//        item_index = 0;
//        SetMainmenuPrePoint(0,0);
//        break;
//
//    case MSG_APP_CANCEL:
//        StopSetingRedrawTimer(win_id);
//        MMK_CloseWin(win_id);
//        break;
//        
//    default:
//        recode = MMI_RESULT_FALSE;
//        break;
//    }
//   
//    return recode;
//}              

/*****************************************************************************/
//  Description : Set key lock control's param.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetKeyLockParam(void)
{
    MMI_STRING_T label_str = {0};
    MMISET_KEY_LOCK_EFFECT_E effect = 0;
    //uint16 index = 0;
    //uint32 loop = 0;
    //uint8 str_index[5] = {0};
    //wchar wstr_text[35] = {0};
    MMI_TEXT_ID_T  text_id = 0;
    
    GUIFORM_SetStyle(MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_SET_KEY_LOCK_EFFECT_TITLE, &label_str);
    GUILABEL_SetText(MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_CTRL_ID, &label_str, FALSE);
    
    //MMI_GetLabelTextByLang(TXT_SET_KEY_LOCK_EFFECT_PREFIX, &label_str);
    //MMIAPICOM_Wstrncpy(wstr_text, label_str.wstr_ptr, label_str.wstr_len);

    effect = MMIAPISET_GetKeyLockEffect();
    text_id = menu_lock_effect_settype[effect].text_str_id;
    //sprintf((char *)str_index, "%d", effect + 1);
    //MMIAPICOM_StrToWstr(str_index, wstr_text + label_str.wstr_len);
    //label_str.wstr_ptr = wstr_text;
    //label_str.wstr_len += strlen((char*)str_index);
    GUILABEL_SetTextById(MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID, text_id, FALSE);
    //GUILABEL_SetText(MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID, &label_str, FALSE);

#if 0   
    for(loop = 0; loop < MMISET_KEY_LOCK_EFFECT_MAX; loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, text_id[loop]);
    }

    GUISETLIST_SetCtrlState(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, effect );
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, TXT_SET_KEY_LOCK_EFFECT_TITLE);    
#endif    
}
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : Append Set key lock list item.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/ 
LOCAL void AppendSetKeyLockListItems(void)
{
    MMISET_KEY_LOCK_EFFECT_E effect = 0;
    GUILIST_SetMaxItem(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, MMISET_KEY_LOCK_EFFECT_MAX, FALSE);
    MMIAPISET_AppendFixedItems(
                                        TXT_SET_KEY_LOCK_EFFECT_PREFIX,
                                        MMISET_KEY_LOCK_EFFECT_MAX,
                                        TXT_COMMON_OK,
                                        TXT_NULL,
                                        STXT_RETURN,
                                        MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID,
                                        GUIITEM_STYLE_ONE_LINE_RADIO
                                        );

    effect = MMIAPISET_GetKeyLockEffect();

    GUILIST_SetCurItemIndex(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, (uint16)effect);
    GUILIST_SetSelectedItem(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID, (uint16)effect, TRUE);

}
#endif

#if defined MMISET_KEYLOCK_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Handle key lock effect setting.
//	Global resource dependence : 
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetKeyLockEffectWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E    res      =   MMI_RESULT_TRUE;    
    static uint16  cur_index    =   0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        AppendSetKeyLockListItems();
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID);
        cur_index = 0;
        break;        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:    
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_index = GUILIST_GetCurItemIndex(MMISET_DISPLAY_KEY_LOCK_EFFECT_SETLIST_CTRL_ID);
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
        if(MMISET_KEY_LOCK_EFFECT5 == cur_index)
        {
            MMK_CreateWin((uint32*)MMISET_SET_KEY_LOCK_EFFECT5_SUB_WIN_TAB,PNULL);
            MMK_CloseWin( win_id ); 
        }
        else
#endif
        {
            MMIAPISET_SetKeyLockEffect((MMISET_KEY_LOCK_EFFECT_E)cur_index);
            MMK_CloseWin( win_id ); 
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
    case MSG_CLOSE_WINDOW:
        MMISET_SetKeyLockParam();
        break;
    default:
        res = MMI_RESULT_FALSE;
        break;
    }   
    return res;
}
#endif
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
/*****************************************************************************/
//  Description : open key lock effect5 sun win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_OpenKeyLockEffect5SubWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SET_KEY_LOCK_EFFECT5_SUB_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void SetEnablePatternUnlock(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T itemStr = {0};
    GUISETLIST_ITEM_INFO_T item_info  ={0};
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_COMMON_ENABLE_ON,
        TXT_CLOSE,
    };
        
    GUIFORM_SetStyle(MMISET_DISPLAY_SET_ENABLE_PATTERN_UNLOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_USE_PATTERN_UNLOCK, &label_name);
    GUILABEL_SetText(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_LABEL_CTRL_ID, &label_name, FALSE);
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, ALIGN_LVMIDDLE);
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        MMI_GetLabelTextByLang(text_id[loop], &itemStr);
        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
        item_info.str_info.wstr_len = itemStr.wstr_len;
        item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
        GUISETLIST_AddItem(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, &item_info);
    }
    
    if(MMISET_GetEnableKeyLock())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void SetVisiblePatternUnlock(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T itemStr = {0};
    GUISETLIST_ITEM_INFO_T item_info  ={0};
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_COMMON_ENABLE_ON,
        TXT_CLOSE,
    };
        
    GUIFORM_SetStyle(MMISET_DISPLAY_SET_VISIBLE_PATTERN_UNLOCK_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_VISIABLE_PATTERN_UNLOCK, &label_name);
    GUILABEL_SetText(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_LABEL_CTRL_ID, &label_name, FALSE);
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
    
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        MMI_GetLabelTextByLang(text_id[loop], &itemStr);
        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
        item_info.str_info.wstr_len = itemStr.wstr_len;
        item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
        GUISETLIST_AddItem(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, &item_info);
    }
    
    if(MMIAPISET_GetVisibleKeyLock())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKeyLockPassward(int8* kl_passward)
{
    int8 passward[MMI_KL_PASSWARD_POINT_MAX] ={0};
    SCI_MEMCPY(passward, kl_passward, MMI_KL_PASSWARD_POINT_MAX);
    MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_PASSWARD, passward);
    //SaveAutoLockKeyPadList();
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetKeyLockPassward(int8* kl_passward)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    int8 passward[MMI_KL_PASSWARD_POINT_MAX] ={0};

    MMINV_READ(MMINV_SET_AUTO_KEYLOCK_PASSWARD, passward, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMISET_SetAutoBoardKey(MMISET_KEYLOCK_CLOSE);
        SCI_MEMSET(kl_passward, -1,  MMI_KL_PASSWARD_POINT_MAX);
        MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_PASSWARD, kl_passward);
        return ;
    }
    SCI_MEMCPY(kl_passward,  passward, MMI_KL_PASSWARD_POINT_MAX);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetEnableKeyLock(BOOLEAN is_enable_keylock) 
{
    MMINV_WRITE(MMINV_SET_IS_ENABLE_KEYLOCK,&is_enable_keylock);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_GetEnableKeyLock(void)
{
    BOOLEAN                 is_enable_keylock = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_IS_ENABLE_KEYLOCK,&is_enable_keylock,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_enable_keylock = FALSE;
        MMINV_WRITE(MMINV_SET_IS_ENABLE_KEYLOCK,&is_enable_keylock);
    }
    
    return (is_enable_keylock);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void SaveEnableLockKeyList(void)
{
    uint16                       cur_selection          =   0;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        MMIAPISET_SetEnableKeyLock(TRUE);
    }
    else
    {
        MMIAPISET_SetEnableKeyLock(FALSE);
        MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_ASTYLE);
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetVisibleKeyLock(BOOLEAN is_visible_keylock) 
{
    MMINV_WRITE(MMINV_SET_IS_VISIBLE_KEYLOCK,&is_visible_keylock);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPISET_GetVisibleKeyLock(void)
{
    BOOLEAN                 is_visible_keylock = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_IS_VISIBLE_KEYLOCK,&is_visible_keylock,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_visible_keylock = FALSE;
        MMINV_WRITE(MMINV_SET_IS_VISIBLE_KEYLOCK,&is_visible_keylock);
    }
    
    return (is_visible_keylock);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void SaveVisibleLockKeyList(void)
{
    uint16                       cur_selection          =   0;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        MMISET_SetVisibleKeyLock(TRUE);
    }
    else
    {
        MMISET_SetVisibleKeyLock(FALSE);
    }
}

/*****************************************************************************/
// 	Description : Handle key lock effect5 submenu setting.
//	Global resource dependence : 
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetKeyLockEffect5SubWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E    res      =   MMI_RESULT_TRUE;    
    //static uint16  cur_index    =   0;
    MMI_CTRL_ID_T ctrl_id = 0;
	MMI_STRING_T label_name  = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
#if defined MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_DISPLAY_KEY_LOCK_EFFECT5_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        
        GUIFORM_SetStyle(MMISET_DISPLAY_SET_CHANGE_PATTERN_UNLOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SET_CHANGE_PATTERN_UNLOCK, &label_name);
        GUILABEL_SetText(MMISET_DISPLAY_CHANGE_PATTERN_UNLOCK_LABEL_CTRL_ID, &label_name, FALSE);
#ifdef MMI_PDA_SUPPORT
        GUISETLIST_SetFontAlign(MMISET_DISPLAY_CHANGE_PATTERN_UNLOCK_LABEL_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        
        SetEnablePatternUnlock();
        SetVisiblePatternUnlock();
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_SET_CHANGE_PATTERN_UNLOCK_FORM_CTRL_ID);

        GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, TXT_SET_USE_PATTERN_UNLOCK);    
        GUISETLIST_SetTitleTextId(MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, TXT_SET_VISIABLE_PATTERN_UNLOCK);    

        break;      

    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(ctrl_id == MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID)
        {
            if(!MMISET_GetEnableKeyLock())
            {
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
                MMIKL_OpenSetKLPassward(); 
#endif
            }
            else
            {
                SaveEnableLockKeyList();
            }
        }
        else if(ctrl_id == MMISET_DISPLAY_VISIBLE_PATTERN_UNLOCK_SETLIST_CTRL_ID)
        {
            SaveVisibleLockKeyList();
        }
        break;

    case MSG_PRE_FULL_PAINT:
        if(MMISET_GetEnableKeyLock())
        {
            GUISETLIST_SetCurIndex(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, 0);
        }
        else
        {
            GUISETLIST_SetCurIndex(MMISET_DISPLAY_ENABLE_PATTERN_UNLOCK_SETLIST_CTRL_ID, 1);
        }
        break;        
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:    
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(ctrl_id == MMISET_DISPLAY_CHANGE_PATTERN_UNLOCK_LABEL_CTRL_ID)
        {
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
            MMIKL_OpenSetKLPassward();
#endif
        }

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
    case MSG_CLOSE_WINDOW:
        if(MMISET_GetEnableKeyLock())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT5);
        }
        MMISET_SetKeyLockParam();
        break;
    case MSG_GET_FOCUS:
        if(MMISET_GetEnableKeyLock())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT5);
        }
        break;
        
    default:
        res = MMI_RESULT_FALSE;
        break;
    }   
    return res;    
}

#endif



/*****************************************************************************/
// 	Description : Get key lock display style
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC MMISET_KEY_LOCK_EFFECT_E MMIAPISET_GetKeyLockEffect(void)
{
    MMISET_KEY_LOCK_EFFECT_E effect = 0;
#ifndef MMI_KEY_LOCK_EFFECT_QQVGA
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_KEY_LOCK_EFFECT, &effect, return_value);

    //if (MN_RETURN_SUCCESS != return_value || (effect < MMISET_KEY_LOCK_EFFECT0 || effect >=MMISET_KEY_LOCK_EFFECT_MAX))
    if (MN_RETURN_SUCCESS != return_value || (effect >=MMISET_KEY_LOCK_EFFECT_MAX))
    {
        effect = MMISET_KEY_LOCK_DEFAULT;
        MMINV_WRITE(MMINV_SET_KEY_LOCK_EFFECT, &effect);
    }
#endif    
    return effect;
}

/*****************************************************************************/
// 	Description : Set key lock display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT_E effect)
{
    //if((effect < MMISET_KEY_LOCK_EFFECT0) || (effect >= MMISET_KEY_LOCK_EFFECT_MAX))
    if(effect >= MMISET_KEY_LOCK_EFFECT_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetKeyLockStyle: style =%d, is invalid!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_LOCK_EFFECT_1049_112_2_18_2_51_19_233,(uint8*)"d", effect);
        return;
    }
    MMINV_WRITE(MMINV_SET_KEY_LOCK_EFFECT, &effect);
    //update display setting window label
    SetKeyLockEffectLableStr();
}

/*****************************************************************************/
// 	Description : MMIAPISET_SetKeyLockEffectLableStr
//	Global resource dependence : none
//  Author: juanwu
//	Note:
/*****************************************************************************/
LOCAL void SetKeyLockEffectLableStr(void)
{
    MMISET_KEY_LOCK_EFFECT_E keylock_type = MMIAPISET_GetKeyLockEffect();
    MMI_TEXT_ID_T            text_id  = 0;
    MMI_CTRL_ID_T            ctrl_id  = MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID;

    
    text_id = menu_lock_effect_settype[keylock_type].text_str_id;
    GUILABEL_SetTextById(ctrl_id, text_id, FALSE);
}



#endif//#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT                                     


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
