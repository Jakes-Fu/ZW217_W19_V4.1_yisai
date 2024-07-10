/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_MV_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#ifdef MV_SUPPORT
#include "mmicc_export.h"
#endif
#include "mmicc_text.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MV_SUPPORT
#if !defined(MMI_GUI_STYLE_TYPICAL) 
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:   
/*****************************************************************************/
LOCAL void  InitPdaMVSettingCtrl(void);
#else

/*****************************************************************************/
//  Description : open magic voice menu
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void  OpenMagicVoiceMenu(void);
#endif

/*****************************************************************************/
//  Description : to handle magic voice main window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
                                             
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:   
/*****************************************************************************/
LOCAL void AppendSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_IMAGE_ID_T image_id,   //icon
                                 uint32         menu_id   //用户数据
                                 );
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

WINDOW_TABLE( MMISET_MAGIC_VOICE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleMagicVoiceWindow),    
    WIN_ID( MMISET_MAGIC_VOICE_WIN_ID ),
    WIN_TITLE(TXT_CC_MAGICVOICE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined(MMI_GUI_STYLE_TYPICAL) 
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_MAGIC_VOICE_MENU_CTRL_ID),
#endif
    //CREATE_MENU_CTRL(MENU_SET_MAGIC_VOICE, MMISET_MAGIC_VOICE_MENU_CTRL_ID),//@dave.ruan change for dynamic menu
    END_WIN
};

#if !defined(MMI_GUI_STYLE_TYPICAL) 

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:   
/*****************************************************************************/
LOCAL void AppendSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_IMAGE_ID_T image_id,   //icon
                                 uint32         menu_id   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

     //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

     //icon
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = menu_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:   
/*****************************************************************************/
LOCAL void  InitPdaMVSettingCtrl(void)
{
    MMI_CTRL_ID_T ctrl_id = MMISET_MAGIC_VOICE_MENU_CTRL_ID;
    uint16 cur_select = GUILIST_GetCurItemIndex(ctrl_id);
    
    GUILIST_SetMaxItem(ctrl_id, 2, FALSE );

    AppendSettingsItem1Line(ctrl_id,TXT_CC_MAGICVOICE,MMIAPISET_GetIsMagicVoice()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,ID_SET_MAGIC_VOICE_OPEN_CLOSE);
    AppendSettingsItem1Line(ctrl_id, TXT_CC_MAGICVOICE_PARAM_SET, IMAGE_COMMON_RIGHT_ARROW, ID_SET_MAGIC_VOICE_PARAM_SET);
    GUILIST_SetItemGray(ctrl_id, 1, !MMIAPISET_GetIsMagicVoice());   /*lint !e730 */

    GUILIST_SetCurItemIndex(ctrl_id, cur_select);
}

/*****************************************************************************/
//  Description : open magic voice main window
//  Global resource dependence : 
//  Author: fen.xie
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMagicVoiceWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_MAGIC_VOICE_WIN_TAB,PNULL);         
}

/*****************************************************************************/
//  Description : to handle magic voice main window message
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        ctrl_id = MMISET_MAGIC_VOICE_MENU_CTRL_ID;
    MMI_MENU_ID_T             menu_id = 0;
    uint16 index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaMVSettingCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
        InitPdaMVSettingCtrl();
        break;
        
    case MSG_FULL_PAINT:
        InitPdaMVSettingCtrl();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {          
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &menu_id);
            switch (menu_id)
            {
            case ID_SET_MAGIC_VOICE_OPEN_CLOSE:
                MMIAPISET_SetIsMagicVoice(!MMIAPISET_GetIsMagicVoice());    /*lint !e730 */
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case ID_SET_MAGIC_VOICE_PARAM_SET:
                MMIAPICC_OpenMagicVoiceSetMenu();
                break;
                
            default:
                break;
            }
        }
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
#else
/*****************************************************************************/
//  Description : open magic voice main window
//  Global resource dependence : 
//  Author: fen.xie
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMagicVoiceWin(void)
{
#ifdef MV_SUPPORT
    OpenMagicVoiceMenu();//open magic voice dynamic setting menu
#endif
}

/*****************************************************************************/
//  Description : creat Dynamic magic voice menu
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void OpenMagicVoiceMenu(void)
{
    MMI_WIN_ID_T    win_id = MMISET_MAGIC_VOICE_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMISET_MAGIC_VOICE_MENU_CTRL_ID;
    GUI_BOTH_RECT_T both_rect = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    MMI_TEXT_ID_T            text_id = 0;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16           i = 0;


    MMK_CreateWin((uint32 *)MMISET_MAGIC_VOICE_WIN_TAB,PNULL);
    
    //MMITHEME_GetWinClientBothRect must behind  MMK_CreatWin, then its handle(win_id) can get
    both_rect = MMITHEME_GetWinClientBothRect(win_id);//set both rect
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id, ctrl_id, GUIMENU_STYLE_THIRD);

    //set menu title
    MMI_GetLabelTextByLang(TXT_CC_MAGICVOICE, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
 
    /*add close/open magic voice menu node*/
    if(MMIAPISET_GetIsMagicVoice())
    {
        text_id = TXT_CC_MAGICVOICE_CLOSE;
    }
    else
    {
        text_id = TXT_CC_MAGICVOICE_OPEN;
    }
    node_id = ID_SET_MAGIC_VOICE_OPEN_CLOSE;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    /*add param set menu node*/
    if(MMIAPISET_GetIsMagicVoice())
    {
        node_item.is_grayed =FALSE;
    }
    else
    {
        node_item.is_grayed =TRUE;
    }

    text_id = TXT_CC_MAGICVOICE_PARAM_SET;
    node_id = ID_SET_MAGIC_VOICE_PARAM_SET;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);
        
}

/*****************************************************************************/
//  Description : to handle magic voice main window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        menu_ctrl_id = MMISET_MAGIC_VOICE_MENU_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_MENU_ID_T   menu_id     =   0;
            MMI_TEXT_ID_T   text_id = TXT_NULL;
            MMI_STRING_T    node_text = {0};
            BOOLEAN     is_magic_voice_actived = FALSE;
            BOOLEAN     is_grayed= FALSE;
            
            menu_id = GUIMENU_GetCurNodeId(menu_ctrl_id);
            switch (menu_id)
            {
            case ID_SET_MAGIC_VOICE_OPEN_CLOSE:
                {
                    is_magic_voice_actived = MMIAPISET_GetIsMagicVoice();
                    MMIAPISET_SetIsMagicVoice(!is_magic_voice_actived);/*lint !e730*/
                    
                    if(MMIAPISET_GetIsMagicVoice())
                    {
                        text_id = TXT_CC_MAGICVOICE_CLOSE;
                        is_grayed = FALSE;
                    }
                    else
                    {
                        text_id = TXT_CC_MAGICVOICE_OPEN;
                        is_grayed = TRUE;
                    }
                    
                    //reset dynamic menus
                    MMI_GetLabelTextByLang(text_id, &node_text);
                    //set text
                    GUIMENU_ReplaceNode(0, ID_SET_MAGIC_VOICE_OPEN_CLOSE, NULL, &node_text, menu_ctrl_id);
                    GUIMENU_SetNodeGrayed(is_grayed, 1, NULL, menu_ctrl_id);

                    //show alert messages
                    if(MMIAPISET_GetIsMagicVoice())
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_CC_MAGICVOICE_OPEN_PROMPT);
                    }
                    else
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_CC_MAGICVOICE_CLOSE_PROMPT);
                    }
              
                }
                break;
                
            case ID_SET_MAGIC_VOICE_PARAM_SET:
                MMIAPICC_OpenMagicVoiceSetMenu();
                break;
                
            default:
                break;
            }
        }
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

#endif   


#endif




/*Edit by script, ignore 2 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
