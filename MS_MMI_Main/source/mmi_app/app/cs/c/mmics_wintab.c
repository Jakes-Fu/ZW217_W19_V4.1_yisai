/*****************************************************************************
** File Name:      mmidcd_wintab.c                                           *
** Author:                                                                   *
** Date:           09/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2007       violent.wei       Create
** 08/2009       jun.hu            modify
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "gui_ucs2b_converter.h"
#include "mmi_appmsg.h"

#ifdef CMCC_UI_STYLE   //!宏定义开头
#include "mmics_main.h"
#include "mmics_id.h"
#include "mmics_text.h"
#include "mmics_menutable.h"
#include "mmics_wintab.h"
#include "mmics_nv.h"
#include "mmics_image.h"
#include "mmics_position.h"

#include "mmisms_image.h"
#include "mmk_app.h"
#include "guilabel.h"
#include "guitab.h"
#include "guirichtext.h"
#include "guitext.h"
#include "mmi_appmsg.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmifmm_export.h"

#include "mmisms_export.h"
//#include "mmisms_app.h"
#include "guires.h" 
#include "guitab.h" 
#include "guimenu.h"
#include "guilistbox.h"
#include "mmk_timer.h"
#include "mmiidle_export.h"
#include "mmics_export.h" 
#include "mmimms_export.h" 
#include "mmiudisk_export.h" 
#include "mmiconnection_export.h" 
#include "mn_type.h"  
#include "guicommon.h"   
#include "mmi_mainmenu_export.h"
//#include "mmiset_wintab.h"
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// 窗口控件id
LOCAL const uint32 s_cs_win_active_attr[] = 
{
        0,
        MMICS_CM_NAME_LABEL_CTRL_ID,       //name content
        MMICS_CM_NUM_LABEL_CTRL_ID,       //date content
        MMICS_SERVICE_HOTLINE_NUM_CTRL_ID,
        
};

LOCAL uint16 s_cs_win_focus_ctrl = MMICS_CM_SELECT_NAME;
    
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainCsMenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );

/*****************************************************************************/
//  Description :  custmer service guid win
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 2009/8/12
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceGuideWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );
/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsE100MenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );
/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCustmerManagerWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );

/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  zhuo.wang
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsTextEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsPhoneNumEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceHotlineWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );

/*****************************************************************************/
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_WinDrawFocusRect( GUI_RECT_T rect,  GUI_COLOR_T color);

/*****************************************************************************/
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_EditWinDrawFocus(MMI_WIN_ID_T win_id,uint16 index);

/*****************************************************************************/
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_EditWinCleanFocus(MMI_WIN_ID_T win_id,uint16 index);

/*****************************************************************************/
// 	Description : HandleSchEditWinWebKeyMsg
//	Global resource dependence : 
//  Author:  zhuo.wang
//	Note:   
/*****************************************************************************/
LOCAL void HandleCsEditWinWebKeyMsg(
                                     MMI_WIN_ID_T	    win_id, 
                                     uint16 index
                                     );


/*****************************************************************************/
// 	Description : handle TP down message to customer manager window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CustmerManagerWinHandleTPPressDown(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
// 	Description : handle TP up message to customer manager window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CustmerManagerWinHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
// 	Description : handle TP up message to Service hotline window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E ServiceHotlineWinHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : CMCC send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CMCCSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//CS main menu window
WINDOW_TABLE(MMICS_MAINMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainCsMenuWinMsg),    
    WIN_ID(MMICS_MAINMENU_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_CS, MMICS_MAIN_MENU_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMICS_SERVICE_GUIDE_WIN_TAB) = 
{ 
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMICS_SERVICE_GUIDE_WIN_ID),
    WIN_TITLE(TXT_CS_SERVICE_GUIDE),
    WIN_FUNC((uint32)HandleServiceGuideWinMsg),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_RICHTEXT_CTRL(MMICS_SERVICE_GUIDE_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMICS_E100_WIN_TAB) = 
{
    //WIN_FUNC((uint32)HandleCsE100MenuWinMsg),    
    WIN_FUNC((uint32)HandleMainCsMenuWinMsg),        
    WIN_ID(MMICS_E100_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_E100, MMICS_MAIN_MENU_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMICS_ABOUT_E100_WIN_TAB) = 
{ 
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMICS_ABOUT_E100_WIN_ID),
    WIN_TITLE(TXT_CS_SERVICE_GUIDE),
    WIN_FUNC((uint32)HandleServiceGuideWinMsg),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
//    CREATE_RICHTEXT_CTRL(MMICS_ABOUT_E100_ID),
    CREATE_RICHTEXT_CTRL(MMICS_SERVICE_GUIDE_ID),


    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMICS_SERVICE_SETTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainCsMenuWinMsg),        
    WIN_ID(MMICS_SERVICE_SETTING_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_SERVICE_SETTING, MMICS_MAIN_MENU_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/*客户服务热线*/
WINDOW_TABLE(MMICS_SERVICE_HOTLINE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleServiceHotlineWinMsg),        
    WIN_ID(MMICS_SERVICE_HOTLINE_WIN_ID),
    WIN_TITLE(TXT_CS_SERVICE_HOTLINE),    //TXT_CS_MANAGER_NUMBER cr238580
   
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_SERVICE_HOTLINE_NAME_CTRL_ID),  //name
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_SERVICE_HOTLINE_NUM_CTRL_ID),     //name content
        
     WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        
     END_WIN
};

// 输入客户热线  的内容
WINDOW_TABLE( MMICS_SERVICE_HOTLINE_EDITNUM_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleCsPhoneNumEditorWinMsg ),    
    WIN_ID( MMICS_SERVICE_HOTLINE_EDITNUM_WIN_ID ), 
    WIN_TITLE(TXT_COMMON_NUMBER),
    CREATE_EDIT_PHONENUM_CTRL(MMICS_TEXT_MAX_LEN,MMICS_SERVICE_HOTLINE_NUMEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

/*客户经理*/
WINDOW_TABLE(MMICS_CUSTMER_MANAGER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleCustmerManagerWinMsg),        
    WIN_ID(MMICS_CUSTMER_MANAGER_WIN_ID),
    WIN_TITLE(TXT_CS_MANAGER_NUMBER),    
   
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_CM_NAME_TITLE_LABEL_CTRL_ID),  //name
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_CM_NAME_LABEL_CTRL_ID),     //name content
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_CM_NUM_TITLE_LABEL_CTRL_ID), //num
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMICS_CM_NUM_LABEL_CTRL_ID),    //num content	
        
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        
     END_WIN
};

// 输入客户经理 的内容
WINDOW_TABLE( MMICS_CM_NAME_EDITTEXT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleCsTextEditorWinMsg ),    
    WIN_ID( MMICS_CM_NAME_EDITTEXT_WIN_ID ), 
    WIN_TITLE(TXT_NAME),
    CREATE_EDIT_TEXT_CTRL(MMICS_TEXT_MAX_LEN,MMICS_CM_NAME_TEXTEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 输入客户经理 的内容
WINDOW_TABLE( MMICS_CM_CALL_EDITNUM_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleCsPhoneNumEditorWinMsg ),    
    WIN_ID( MMICS_CM_CALL_EDITNUM_WIN_ID ), 
    WIN_TITLE(TXT_COMMON_NUMBER),
    CREATE_EDIT_PHONENUM_CTRL(MMICS_TEXT_MAX_LEN,MMICS_CM_PHONE_NUMEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMICS_OpenMainWin(void)
{
  MMK_CreateWin((uint32 *)MMICS_MAINMENU_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : CMCC send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CMCCSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;
    
    return result;
}

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainCsMenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_MENU_GROUP_ID_T   group_id    =   0;
    MMI_MENU_ID_T         menu_id     =   0;
    MMI_RESULT_E          recode      =   MMI_RESULT_TRUE; 
    MMI_CTRL_ID_T                       ctrl_id = MMICS_MAIN_MENU_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            switch (menu_id)
            {  
            case ID_CS_PHONE_SERVICE_GUIDE:
                MMK_CreateWin((uint32 *)MMICS_SERVICE_GUIDE_WIN_TAB,PNULL);
                break;
                
            case ID_CS_E100:
                MMK_CreateWin((uint32 *)MMICS_E100_WIN_TAB,PNULL);				
                break;
                
            case ID_CS_CUSTMER_MANAGER:
                MMK_CreateWin((uint32 *)MMICS_CUSTMER_MANAGER_WIN_TAB,PNULL);				
                break;
                
            case ID_CS_MY_MONERNET:
                MMIAPISET_EnterMonternetWin();
                break;
                
            case ID_CS_CUSTMER_SETTING:
                MMK_CreateWin((uint32 *)MMICS_SERVICE_SETTING_WIN_TAB,PNULL);				
                break;

            case ID_CS_HOTLINE:
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                                   "10086",
                                   5,
                                   PNULL,
                                   PNULL,
                                   CC_MAJOR_SIM_CALL,
                                   CC_CALL_NORMAL_CALL,
                                   PNULL);
                break;
				
            case ID_CS_SMS_OFFICE:
                {
                    MMISMS_SEND_DATA_T send_data = {0};
                    MMI_STRING_T    message_content = {0};

                    message_content.wstr_ptr = L"10086";
                    message_content.wstr_len = 5;
                    send_data.sms_content_ptr = &message_content;
                    send_data.dst_port = 0;
                    send_data.src_port = 0;
                    send_data.dest_addr_ptr = (uint8 *)"10086";
                    send_data.dest_addr_len = 5;
                    send_data.dual_sys = MN_DUAL_SYS_1;
                    send_data.is_need_packet = TRUE;
                    send_data.send_callback = CMCCSMSSendCnfCallback;

                    MMIAPISMS_SendSmsForOther(&send_data);
                }                
                break;

            case ID_CS_POCKET_HALL:
                MMIAPISET_EnterPocketHallWin();
                break;
				
            case ID_CS_ABOUT_E100:
                MMK_CreateWin((uint32 *)MMICS_ABOUT_E100_WIN_TAB,PNULL);					
                break;

            case ID_CS_SERVICE_HOTLINE:
                MMK_CreateWin((uint32 *)MMICS_SERVICE_HOTLINE_WIN_TAB,PNULL);				
	     break;
		 
            case ID_CS_MANAGER_NUMBER:
                MMK_CreateWin((uint32 *)MMICS_CUSTMER_MANAGER_WIN_TAB,PNULL);				
	     break;
                
            default:
                break;
                
            }
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        break;
        
    case MSG_PROMPTWIN_OK:
        
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
//  Description :  custmer service guid win
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 2009/8/12
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceGuideWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  richtext_ctrl_id = MMICS_SERVICE_GUIDE_ID;
    GUIRICHTEXT_ITEM_T  item_data = {0};             /*lint !e64*/
    MMI_STRING_T  dststring= {0};
    uint16 index = 0;
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            
                GUIRICHTEXT_SetFont(richtext_ctrl_id, MMI_DEFAULT_BIG_FONT);
                // set all the context in richtext
                if(win_id == MMICS_SERVICE_GUIDE_WIN_ID)
                {
                MMI_GetLabelTextByLang(TXT_CS_SERVICE_GUIDE_DETAIL, &dststring);
                }
		  else
		  {
                MMI_GetLabelTextByLang(TXT_CS_ABOUT_E100_DETAIL, &dststring);
		  }
                item_data.text_data.buf.len = (uint16)MIN(dststring.wstr_len, MMICS_MAX_DETAIL_STR_LEN);
                item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
                GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index); 
            
                MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
            }       
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            // 关闭当前窗口
            MMK_CloseWin(win_id);
            break;
        
        default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    

    return (recode);
}

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsE100MenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_MENU_GROUP_ID_T   group_id    =   0;
    MMI_MENU_ID_T         menu_id     =   0;
    MMI_RESULT_E          recode      =   MMI_RESULT_TRUE; 
    MMI_CTRL_ID_T                       ctrl_id = MMICS_E100_MENU_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            switch (menu_id)
            {  
            case ID_CS_HOTLINE:
                break;
				
            case ID_CS_SMS_OFFICE:
                break;	
				
            case ID_CS_ABOUT_E100:
                break;
                
            default:
                break;
                
            }
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        break;
        
    case MSG_PROMPTWIN_OK:
        
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
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_WinDrawFocusRect( GUI_RECT_T rect,  GUI_COLOR_T color)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}

/*****************************************************************************/
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_EditWinDrawFocus(MMI_WIN_ID_T win_id,uint16 index)
{
    MMI_TEXT_ID_T   middlesoft_id	= TXT_NULL;	
    
    GUI_RECT_T rect = {0};//{MMICS_NAME_LEFT - 1, MMICS_NAME_TOP - 1, MMICS_NAME_RIGHT + 5, MMICS_NAME_BOTTOM + 1};
    rect.left = MMICS_NAME_LEFT - 1;
    rect.top = MMICS_NAME_TOP - 1;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
    rect.right = MMICS_NAME_RIGHT + 5;
    rect.bottom = MMICS_NAME_BOTTOM + 1;
    //所有输入框都有白色线框
    if(index == MMICS_CM_HOTLINE_NUM)
    {        
        CS_WinDrawFocusRect(rect, MMI_DARK_GRAY_COLOR);        
    }
    else
    {           
        CS_WinDrawFocusRect(rect, MMI_DARK_GRAY_COLOR);        
        rect.top = MMICS_NUM_TOP - 1;
        rect.bottom = MMICS_NUM_BOTTOM + 1;
        CS_WinDrawFocusRect(rect, MMI_DARK_GRAY_COLOR);
    }
    switch(index)
    {
    case MMICS_CM_SELECT_NAME:
        {
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NAME;
            GUILABEL_SetBackgroundColor(MMICS_CM_NAME_LABEL_CTRL_ID, MMI_DARK_GRAY_COLOR);
            middlesoft_id = TXT_EDIT;
        }
        break;
        
    case MMICS_CM_SELECT_NUM:
        {
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NUM;
            GUILABEL_SetBackgroundColor(MMICS_CM_NUM_LABEL_CTRL_ID, MMI_DARK_GRAY_COLOR);
            middlesoft_id = TXT_EDIT;
        }
        break;
        
    case MMICS_CM_HOTLINE_NUM:
        {
            s_cs_win_focus_ctrl = MMICS_CM_HOTLINE_NUM;
            GUILABEL_SetBackgroundColor(MMICS_SERVICE_HOTLINE_NUM_CTRL_ID, MMI_DARK_GRAY_COLOR);
            middlesoft_id = TXT_EDIT;
        }		
        break;
    default:
        break;
        
    }
    
    //更新软键栏
    GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, middlesoft_id, STXT_RETURN, TRUE);					  
}

/*****************************************************************************/
//  Description :draw dropdownlist focus
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void CS_EditWinCleanFocus(MMI_WIN_ID_T win_id,uint16 index)
{
    uint8 i = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch(index) 
    {
    case MMICS_CM_SELECT_NAME:
        {
            IGUICTRL_T *ctrl_ptr = NULL;
            GUI_RECT_T rect = {0};//{MMICS_NAME_LEFT - 1, MMICS_NAME_TOP - 1, MMICS_NAME_RIGHT + 5, MMICS_NAME_BOTTOM + 1};
            rect.left = MMICS_NAME_LEFT - 1;
            rect.top = MMICS_NAME_TOP - 1;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
            rect.right = MMICS_NAME_RIGHT + 5;
            rect.bottom = MMICS_NAME_BOTTOM + 1;
            
            ctrl_ptr = MMK_GetCtrlPtr(MMICS_CM_NAME_LABEL_CTRL_ID);
            GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            GUILABEL_SetBackgroundColor(MMICS_CM_NAME_LABEL_CTRL_ID, MMI_BACKGROUND_COLOR);
            IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
            //MMK_PostMsg(MMICS_CUSTMER_MANAGER_WIN_ID, MSG_CTL_PAINT, PNULL, 0);
        }
        break;
        
    case MMICS_CM_SELECT_NUM:
        {
            IGUICTRL_T *ctrl_ptr = NULL;
            GUI_RECT_T rect = {0};//{MMICS_NUM_LEFT - 1, MMICS_NUM_TOP - 1, MMICS_NUM_RIGHT + 5, MMICS_NUM_BOTTOM + 1};
            rect.left = MMICS_NUM_LEFT - 1;
            rect.top = MMICS_NUM_TOP - 1;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
            rect.right = MMICS_NUM_RIGHT + 5;
            rect.bottom = MMICS_NUM_BOTTOM + 1;
            ctrl_ptr = MMK_GetCtrlPtr(MMICS_CM_NUM_LABEL_CTRL_ID);            
            GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            GUILABEL_SetBackgroundColor(MMICS_CM_NUM_LABEL_CTRL_ID, MMI_BACKGROUND_COLOR);
            IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
            
        }
        break;
        
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsTextEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_STRING_T   str = {0}; 
    MMI_CTRL_ID_T current_ctrl_id = MMICS_CM_NAME_TEXTEDIT_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILABEL_GetText(s_cs_win_active_attr[MMICS_CM_SELECT_NAME], &str);
            GUIEDIT_SetMaxLen(current_ctrl_id,MMICS_TEXT_MAX_LEN );
            GUIEDIT_SetString(
                current_ctrl_id,
                str.wstr_ptr,
                str.wstr_len           
                );
            
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
            break;
        }
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // get string info
        {  uint16 wlen=0;
            GUIEDIT_GetString(current_ctrl_id, &str);            
            wlen=(uint16)MIN(MMICS_TEXT_MAX_LEN,str.wstr_len );
            SCI_MEMSET(MMICS_GeSettingInfo()->name_string, 0, sizeof(MMICS_GeSettingInfo()->name_string));			
            MMI_WSTRNCPY(MMICS_GeSettingInfo()->name_string,MMICS_TEXT_MAX_LEN,str.wstr_ptr,MMICS_TEXT_MAX_LEN,wlen);
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCsPhoneNumEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_STRING_T   str = {0}; 
    MMI_CTRL_ID_T current_ctrl_id = MMICS_CM_PHONE_NUMEDIT_CTRL_ID;
    MMI_CTRL_ID_T label_ctrl_id = MMICS_CM_NUM_LABEL_CTRL_ID; 	
    wchar * content_ptr=PNULL;
    uint16 index=MMICS_CM_SELECT_NUM;
    
    
    label_ctrl_id = (MMI_CTRL_ID_T )MMK_GetWinAddDataPtr(win_id);	
    
    if(label_ctrl_id == MMICS_CM_NUM_LABEL_CTRL_ID)
    {
        index= MMICS_CM_SELECT_NUM;
        content_ptr= MMICS_GeSettingInfo()->callnum_string;	
    }
    else
    {
        index= MMICS_CM_HOTLINE_NUM;
        current_ctrl_id = MMICS_SERVICE_HOTLINE_NUMEDIT_CTRL_ID;		
        content_ptr = MMICS_GeSettingInfo()->hotline_string;   
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILABEL_GetText(s_cs_win_active_attr[index], &str);
            GUIEDIT_SetMaxLen(current_ctrl_id,MMICS_TEXT_MAX_LEN );
            GUIEDIT_SetString(
                current_ctrl_id,
                str.wstr_ptr,
                str.wstr_len           
                );
            
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
            break;
        }
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // get string info
        {
            uint16 wlen=0;
            GUIEDIT_GetString(current_ctrl_id, &str);
            wlen=(uint16)MIN(MMICS_TEXT_MAX_LEN,str.wstr_len );
            SCI_MEMSET(content_ptr, 0, MMICS_TEXT_MAX_LEN);						
            MMI_WSTRNCPY(content_ptr,MMICS_TEXT_MAX_LEN,str.wstr_ptr,MMICS_TEXT_MAX_LEN,wlen); 
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : HandleSchEditWinWebKeyMsg
//	Global resource dependence : 
//  Author:  zhuo.wang
//	Note:   
/*****************************************************************************/
LOCAL void HandleCsEditWinWebKeyMsg(
                                     MMI_WIN_ID_T	    win_id, 
                                     uint16 index
                                     )
{
    switch(index)
    {
    case MMICS_CM_SELECT_NAME:
        MMK_SetAtvCtrl(win_id, s_cs_win_active_attr[index]);
        MMK_CreateWin((uint32 *)MMICS_CM_NAME_EDITTEXT_WIN_TAB, PNULL);
        break;
        
    case MMICS_CM_SELECT_NUM:
        MMK_SetAtvCtrl(win_id, s_cs_win_active_attr[index]);
        MMK_CreateWin((uint32 *)MMICS_CM_CALL_EDITNUM_WIN_TAB, (ADD_DATA)s_cs_win_active_attr[index]);
        
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCustmerManagerWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_MENU_GROUP_ID_T   group_id    =   0;
    MMI_MENU_ID_T         menu_id     =   0;
    MMI_RESULT_E          recode      =   MMI_RESULT_TRUE; 
    GUI_RECT_T                  bk_rect         = MMITHEME_GetClientRect();	
    MMI_CTRL_ID_T                       ctrl_id = MMICS_CM_NAME_LABEL_CTRL_ID;
    GUI_LCD_DEV_INFO			lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};	
    MMI_STRING_T        name_str                                = {0};
    MMI_STRING_T        num_str                                = {0};
    
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
	        {
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NAME_TITLE_LEFT,MMICS_NAME_TITLE_TOP,MMICS_NAME_TITLE_RIGHT,MMICS_NAME_TITLE_BOTTOM};
                lab_rect.left = MMICS_NAME_TITLE_LEFT;
                lab_rect.top = MMICS_NAME_TITLE_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NAME_TITLE_RIGHT;
                lab_rect.bottom = MMICS_NAME_TITLE_BOTTOM;
				GUILABEL_SetRect(MMICS_CM_NAME_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
	        }    
	        {
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NAME_LEFT,MMICS_NAME_TOP,MMICS_NAME_RIGHT + 4,MMICS_NAME_BOTTOM};
                lab_rect.left = MMICS_NAME_LEFT;
                lab_rect.top = MMICS_NAME_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NAME_RIGHT + 4;
                lab_rect.bottom = MMICS_NAME_BOTTOM;
				GUILABEL_SetRect(MMICS_CM_NAME_LABEL_CTRL_ID, &lab_rect, FALSE);
	        }    
	        {
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NUM_TITLE_LEFT,MMICS_NUM_TITLE_TOP,MMICS_NUM_TITLE_RIGHT,MMICS_NUM_TITLE_BOTTOM};
                lab_rect.left = MMICS_NUM_TITLE_LEFT;
                lab_rect.top = MMICS_NUM_TITLE_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NUM_TITLE_RIGHT;
                lab_rect.bottom = MMICS_NUM_TITLE_BOTTOM;
				GUILABEL_SetRect(MMICS_CM_NUM_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
	        }
	        {
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NUM_LEFT,MMICS_NUM_TOP,MMICS_NUM_RIGHT + 4,MMICS_NUM_BOTTOM};
                lab_rect.left = MMICS_NUM_LEFT;
                lab_rect.top = MMICS_NUM_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NUM_RIGHT + 4;
                lab_rect.bottom = MMICS_NUM_BOTTOM;
				GUILABEL_SetRect(MMICS_CM_NUM_LABEL_CTRL_ID, &lab_rect, FALSE);
	        }    
        
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NAME;				 
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);		
            GUIRES_DisplayImg(PNULL,
                &bk_rect,
                &bk_rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            
            GUILABEL_SetTextById(MMICS_CM_NAME_TITLE_LABEL_CTRL_ID, TXT_NAME,FALSE);
            GUILABEL_SetTextById(MMICS_CM_NUM_TITLE_LABEL_CTRL_ID, TXT_COMMON_NUMBER,FALSE);
            GUILABEL_SetBackgroundColor(MMICS_CM_NAME_LABEL_CTRL_ID, MMI_BACKGROUND_COLOR);
            GUILABEL_SetBackgroundColor(MMICS_CM_NUM_LABEL_CTRL_ID, MMI_BACKGROUND_COLOR);
            
            name_str.wstr_len = MMIAPICOM_Wstrlen(MMICS_GeSettingInfo()->name_string);
            name_str.wstr_ptr = MMICS_GeSettingInfo()->name_string;
            GUILABEL_SetText(MMICS_CM_NAME_LABEL_CTRL_ID, &name_str, FALSE);
            
            num_str.wstr_len = MMIAPICOM_Wstrlen(MMICS_GeSettingInfo()->callnum_string);
            num_str.wstr_ptr = MMICS_GeSettingInfo()->callnum_string;			
            GUILABEL_SetText(MMICS_CM_NUM_LABEL_CTRL_ID, &num_str, FALSE);
            
            CS_EditWinDrawFocus(win_id,s_cs_win_focus_ctrl);
            
            
        }			
        break;
        
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
        HandleCsEditWinWebKeyMsg(win_id, s_cs_win_focus_ctrl);
        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMICS_WriteSettingToNv();
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:    
        {
            MMICS_SETTING_INFO_T*    setting_info_ptr = MMICS_GeSettingInfo();
            MMI_STRING_T             disp_str = {0};
            char                     call_num[MMICS_TEXT_MAX_LEN] = {0};
            #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
            #else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            MN_DUAL_SYS_E  dual_sys = MMIAPICC_CheckDualSysType(msg_id, MN_DUAL_SYS_MAX);
            #endif
            
            MMIAPICOM_WstrToStr(setting_info_ptr->callnum_string, call_num);
            disp_str.wstr_ptr = setting_info_ptr->name_string;
            disp_str.wstr_len = MMIAPICOM_Wstrlen(disp_str.wstr_ptr);

            #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                                   call_num,
                                   strlen(call_num),
                                   &disp_str,
                                   PNULL,
                                   sim_type,
                                   CC_CALL_NORMAL_CALL,
                                   PNULL);
            #else
            MMIAPICC_MakeCall(dual_sys,
                       call_num,
                       strlen(call_num),
                       &disp_str,
                       PNULL,
                       sim_type,
                       CC_CALL_NORMAL_CALL,
                       PNULL);
            #endif

        }
        break;
        
    case MSG_APP_UP:
        CS_EditWinCleanFocus(win_id,s_cs_win_focus_ctrl);		
        if(s_cs_win_focus_ctrl==MMICS_CM_SELECT_NAME)
        {
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NUM;
        }
        else	
        {
            s_cs_win_focus_ctrl=MMICS_CM_SELECT_NAME;
        }
        
        CS_EditWinDrawFocus(win_id,s_cs_win_focus_ctrl);
        if(s_cs_win_active_attr[s_cs_win_focus_ctrl]!=0)
        {
            MMK_SetAtvCtrlEx(win_id, s_cs_win_active_attr[s_cs_win_focus_ctrl], TRUE);
        }       
        
        break;
        
    case MSG_APP_DOWN:
        CS_EditWinCleanFocus(win_id,s_cs_win_focus_ctrl);		
        if(s_cs_win_focus_ctrl == MMICS_CM_SELECT_NAME)
        {
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NUM;            
        }
        else		
        {
            s_cs_win_focus_ctrl = MMICS_CM_SELECT_NAME;
        }	
        CS_EditWinDrawFocus(win_id,s_cs_win_focus_ctrl);
        if(s_cs_win_active_attr[s_cs_win_focus_ctrl]!=0)
        {
            MMK_SetAtvCtrlEx(win_id, s_cs_win_active_attr[s_cs_win_focus_ctrl], TRUE);
        }
        
        
        break;
        
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        recode = CustmerManagerWinHandleTPPressDown(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        recode = CustmerManagerWinHandleTPPressUp(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
    
} 

/*****************************************************************************/
// 	Description : icon main second menu handle window message
//	Global resource dependence : 
//  Author: zhuo.wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceHotlineWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_MENU_GROUP_ID_T   group_id    =   0;
    MMI_MENU_ID_T         menu_id     =   0;
    MMI_RESULT_E          recode      =   MMI_RESULT_TRUE; 
    GUI_RECT_T                  bk_rect         = MMITHEME_GetClientRect();	
    MMI_CTRL_ID_T                       ctrl_id = MMICS_SERVICE_HOTLINE_NUM_CTRL_ID;
    GUI_LCD_DEV_INFO			lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};	
    MMI_STRING_T        num_str                                = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
	        {
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NAME_TITLE_LEFT,MMICS_NAME_TITLE_TOP,MMICS_NAME_TITLE_RIGHT,MMICS_NAME_TITLE_BOTTOM};
                lab_rect.left = MMICS_NAME_TITLE_LEFT;
                lab_rect.top = MMICS_NAME_TITLE_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NAME_TITLE_RIGHT;
                lab_rect.bottom = MMICS_NAME_TITLE_BOTTOM;
				GUILABEL_SetRect(MMICS_SERVICE_HOTLINE_NAME_CTRL_ID, &lab_rect, FALSE);
	        }      
	        
			{
	        	GUI_RECT_T lab_rect = {0};//{MMICS_NAME_LEFT,MMICS_NAME_TOP,MMICS_NAME_RIGHT + 4,MMICS_NAME_BOTTOM};
                lab_rect.left = MMICS_NAME_LEFT;
                lab_rect.top = MMICS_NAME_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
                lab_rect.right = MMICS_NAME_RIGHT + 4;
                lab_rect.bottom = MMICS_NAME_BOTTOM;
				GUILABEL_SetRect(MMICS_SERVICE_HOTLINE_NUM_CTRL_ID, &lab_rect, FALSE);
	        }         
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);		
            GUIRES_DisplayImg(PNULL,
                &bk_rect,
                &bk_rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            
            GUILABEL_SetTextById(MMICS_SERVICE_HOTLINE_NAME_CTRL_ID, TXT_CS_SERVICE_HOTLINE,FALSE);
            GUILABEL_SetBackgroundColor(MMICS_SERVICE_HOTLINE_NUM_CTRL_ID, MMI_BACKGROUND_COLOR);
            
            num_str.wstr_len = MMIAPICOM_Wstrlen(MMICS_GeSettingInfo()->hotline_string);
            num_str.wstr_ptr = MMICS_GeSettingInfo()->hotline_string;			
            GUILABEL_SetText(ctrl_id, &num_str, FALSE);
            
            CS_EditWinDrawFocus(win_id,MMICS_CM_HOTLINE_NUM);
            
            
        }			
        break;
        
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
        MMK_SetAtvCtrl(win_id, MMICS_SERVICE_HOTLINE_NUM_CTRL_ID);
        MMK_CreateWin((uint32 *)MMICS_SERVICE_HOTLINE_EDITNUM_WIN_TAB, (ADD_DATA)ctrl_id);        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMICS_WriteSettingToNv();
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
        break;      
        
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

      case MSG_APP_GREEN:
      case MSG_APP_CALL2:
      case MSG_APP_CALL3:
      case MSG_APP_CALL4:  
        {
            MMICS_SETTING_INFO_T*    setting_info_ptr = MMICS_GeSettingInfo();
            char                     call_num[MMICS_TEXT_MAX_LEN] = {0};
            
            #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
            #else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
            MN_DUAL_SYS_E dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX);
            #endif
            
            MMIAPICOM_WstrToStr(setting_info_ptr->hotline_string, call_num);

            #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                                   call_num,
                                   strlen(call_num),
                                   PNULL,
                                   PNULL,
                                   sim_type,
                                   CC_CALL_NORMAL_CALL,
                                   PNULL);
            #else
            MMIAPICC_MakeCall(dual_sys,
                       call_num,
                       strlen(call_num),
                       PNULL,
                       PNULL,
                       sim_type,
                       CC_CALL_NORMAL_CALL,
                       PNULL);
            #endif

        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        recode =ServiceHotlineWinHandleTPPressUp(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
    
    
    
} 

/*****************************************************************************/
// 	Description : handle TP down message to customer manager window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CustmerManagerWinHandleTPPressDown(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T name_rect = {0};//{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT + 4, MMICS_NAME_BOTTOM};
    GUI_RECT_T num_rect = {0};//{MMICS_NUM_LEFT, MMICS_NUM_TOP, MMICS_NUM_RIGHT + 4, MMICS_NUM_BOTTOM};
    name_rect.left = MMICS_NAME_LEFT;
    name_rect.top = MMICS_NAME_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
    name_rect.right = MMICS_NAME_RIGHT + 4;
    name_rect.bottom = MMICS_NAME_BOTTOM;
    
    num_rect.left = MMICS_NUM_LEFT;
    num_rect.top = MMICS_NUM_TOP;
    num_rect.right = MMICS_NUM_RIGHT + 4;
    num_rect.bottom = MMICS_NUM_BOTTOM;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if(GUI_PointIsInRect(point, name_rect))
    {
        CS_EditWinCleanFocus(win_id,MMICS_CM_SELECT_NUM);		
        CS_EditWinDrawFocus(win_id,MMICS_CM_SELECT_NAME);
        if(s_cs_win_active_attr[MMICS_CM_SELECT_NAME]!=0)
        {
            MMK_SetAtvCtrlEx(win_id, s_cs_win_active_attr[MMICS_CM_SELECT_NAME], TRUE);
        }       
        
    }
    else if(GUI_PointIsInRect(point, num_rect))
    {
        CS_EditWinCleanFocus(win_id,MMICS_CM_SELECT_NAME);		
        CS_EditWinDrawFocus(win_id,MMICS_CM_SELECT_NUM);
        if(s_cs_win_active_attr[MMICS_CM_SELECT_NUM]!=0)
        {
            MMK_SetAtvCtrlEx(win_id, s_cs_win_active_attr[MMICS_CM_SELECT_NUM], TRUE);
        }       
        
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : handle TP up message to customer manager window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CustmerManagerWinHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T name_rect = {0};//{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT + 4, MMICS_NAME_BOTTOM};
    GUI_RECT_T num_rect = {0};//{MMICS_NUM_LEFT, MMICS_NUM_TOP, MMICS_NUM_RIGHT + 4, MMICS_NUM_BOTTOM};
    name_rect.left = MMICS_NAME_LEFT;
    name_rect.top = MMICS_NAME_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
    name_rect.right = MMICS_NAME_RIGHT + 4;
    name_rect.bottom = MMICS_NAME_BOTTOM;
    
    num_rect.left = MMICS_NUM_LEFT;
    num_rect.top = MMICS_NUM_TOP;
    num_rect.right = MMICS_NUM_RIGHT + 4;
    num_rect.bottom = MMICS_NUM_BOTTOM;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if(GUI_PointIsInRect(point, name_rect))
    {
        HandleCsEditWinWebKeyMsg(win_id, MMICS_CM_SELECT_NAME);
    }
    else if(GUI_PointIsInRect(point, num_rect))
    {
        HandleCsEditWinWebKeyMsg(win_id, MMICS_CM_SELECT_NUM);
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : handle TP up message to Service hotline window
//	Global resource dependence : 
//  Author: hongyhing wang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E ServiceHotlineWinHandleTPPressUp(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T name_rect = {0};//{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT + 4, MMICS_NAME_BOTTOM};
    name_rect.left = MMICS_NAME_LEFT;
    name_rect.top = MMICS_NAME_TOP;//xuefangjiang modify ,MMICS_NAME_TOP宏对应了函数MMITHEME_GetWinTitleHeight(),不能直接用函数初始化
    name_rect.right = MMICS_NAME_RIGHT + 4;
    name_rect.bottom = MMICS_NAME_BOTTOM;
   
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if(GUI_PointIsInRect(point, name_rect))
    {
         MMK_SetAtvCtrl(win_id, MMICS_SERVICE_HOTLINE_NUM_CTRL_ID);
         MMK_CreateWin((uint32 *)MMICS_SERVICE_HOTLINE_EDITNUM_WIN_TAB, (ADD_DATA)MMICS_SERVICE_HOTLINE_NUM_CTRL_ID);        
   
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:52 2012*/ //IGNORE9527


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
