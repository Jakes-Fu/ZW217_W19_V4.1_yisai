/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef _MMIZDT_SHB_WINTAB_C_
#define _MMIZDT_SHB_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "img_dec_interface.h"
#include "guiownerdraw.h"
#include "graphics_draw.h"
#include "mmiparse_export.h"
#include "mmicl_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif
#include "mmiset_export.h"
#include "mmiset_id.h"

#include "shb_txt1.h"
#include "shb_txt2.h"

BOOLEAN s_shbserver_type = 1; 
uint16 g_cur_media_select_index = 0;
extern uint8  g_shb_sms_bdrq_num[MAX_SHB_IP_DOMAIN_SIZE+1];

PUBLIC void ZdtSHB_IntroductionWin(void);
PUBLIC void ZdtSHB_UserguideWin(void);
PUBLIC void ZdtSHB_ClientDoladWin(void);
PUBLIC void ZdtSHB_HotlineWin(void);
PUBLIC void ZdtSHB_AbouthelpWin(void);
PUBLIC void ZdtSHB_VersionWin(void);

PUBLIC void Zdt_ShbServerWin(void);

PUBLIC void ZdtSHB_AboutWin(void);

LOCAL MMI_RESULT_E ShbWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMIZDT_SHB_MENU_CTRL_ID,&group_id,&menu_id);


           
            MMK_SetAtvCtrl(win_id, MMIZDT_SHB_MENU_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMIZDT_SHB_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

				case ID_ZDT_MENU_SHB_SERVER:
						Zdt_ShbServerWin();
					break;
			
             case ID_ZDT_MENU_SHB_SPEEDDIAL:
				 //MMIAPICC_OpenSpeedDialSettingsWin();
                            MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0);
					break;

                
#ifdef SOS_KEYLONGWEB
           case ID_ZDT_MENU_SHB_SOS:
					break;
#endif
			case ID_ZDT_MENU_SHB_HELP:
				 ZdtSHB_AboutWin();
				 break;

				
                default:
                    break;
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



WINDOW_TABLE(MMIZDT_SHB_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)ShbWinHandleMsg),
        //win_id
        WIN_ID(MMIZDT_SHB_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_SHB),
        //创建  控件
		CREATE_MENU_CTRL(ZDT_MENU_SHB, MMIZDT_SHB_MENU_CTRL_ID),
		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
		//   WIN_TIPS,
	 END_WIN

};
PUBLIC void Zdt_ShbWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_WIN_TAB, PNULL);
}



LOCAL void MMIAPISET_CommonTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

LOCAL void ShbServerTypeSet(void)
{

    s_shbserver_type =(uint16)GUILIST_GetCurItemIndex(SHB_SERVER_TYPE_CTRL_ID);   ///GUISETLIST_GetCurIndex(MMISET_TORCH_SETLIST_CTRL_ID);
    if(s_shbserver_type == 0)
	{
		SHB_API_SeNetOpen();
	}
	else
	{
		SHB_API_SeNetClose();
	}

 
}

PUBLIC uint8 Get_ShbServer_type( void )
{
	if(SHB_Net_IsAllow())
	{
		s_shbserver_type = 0;
	}
	else
	{
		s_shbserver_type = 1;
	}

    return   s_shbserver_type ;
}

PUBLIC void Set_ShbServer_type( uint8  cur_set )
{
    
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

	s_shbserver_type = cur_set;
	return;
}


LOCAL void  SetShbServerListCode( MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_CommonTextId(TXT_COMM_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_CommonTextId(TXT_COMMON_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	 if(Get_ShbServer_type() == 0)
    {
		cur_selection = 0;

	}
	else if(Get_ShbServer_type() == 1)
    {
		cur_selection = 1;
    }

	///}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}


PUBLIC MMI_RESULT_E ShbServerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	  MMI_CTRL_ID_T   ctrl_id  = SHB_SERVER_TYPE_CTRL_ID;
	uint16					 max_item = 2; 

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
	//	GUIFORM_SetType(MMISET_TORCH_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif      
	  SetShbServerListCode(ctrl_id);

	  MMK_SetAtvCtrl(win_id, SHB_SERVER_TYPE_CTRL_ID);
		break;
			
	 case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
		ShbServerTypeSet();
		MMK_CloseWin(win_id);
		break;		  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}

WINDOW_TABLE(ZDT_SHB_SERVER_TYPE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)ShbServerWinHandleMsg),
        //win_id
        WIN_ID(ZDT_SHB_SERVER_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_SHB_SERVER),

       CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,SHB_SERVER_TYPE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
PUBLIC void Zdt_ShbServerWin(void)
{
    MMK_CreateWin((uint32*)ZDT_SHB_SERVER_TYPE_WIN_TAB, PNULL);
}












LOCAL MMI_RESULT_E AboutShbWinHandleMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
	GUI_POINT_T 				point				= {0};
		
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
			
		
            GUIMENU_GetId(MMIZDT_ABOUT_SHB_MENU_CTRL_ID,&group_id,&menu_id);


           
            MMK_SetAtvCtrl(win_id, MMIZDT_ABOUT_SHB_MENU_CTRL_ID);
			
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
			
            GUIMENU_GetId(MMIZDT_ABOUT_SHB_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

				case ID_ZDT_MENU_SHB_INTRODUCTION:
						ZdtSHB_IntroductionWin();
					break;
			
             	case ID_ZDT_MENU_SHB_USERGUIDE:
				 ZdtSHB_UserguideWin();

					break;

                
           	case ID_ZDT_MENU_SHB_CLIENTDOWNLOAD:
			  	 ZdtSHB_ClientDoladWin();

					break;
			case ID_ZDT_MENU_SHB_HOTLINE:

				 ZdtSHB_HotlineWin();
			
				 break;
                 
			 case ID_ZDT_MENU_SHB_VERSION:
                            ZdtSHB_VersionWin();
				 break;

			 case ID_ZDT_MENU_SHB_ABOUTHELP:
			 	ZdtSHB_AbouthelpWin();
			 	break;
                default:
                    break;
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


WINDOW_TABLE(MMIZDT_ABOUT_SHB_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)AboutShbWinHandleMsg),
        //win_id
        WIN_ID(MMIZDT_ABOUT_SHB_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_ABOUT_SHB),
        //创建  控件
		CREATE_MENU_CTRL(ZDT_MENU_ABOUT_SHB, MMIZDT_ABOUT_SHB_MENU_CTRL_ID),
		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
		//   WIN_TIPS,
	 END_WIN

};

PUBLIC void ZdtSHB_AboutWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_ABOUT_SHB_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E IntroductionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)
	GUI_FONT_T font = SONG_FONT_22 ;
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif
	GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
        MMK_SetAtvCtrl(win_id, INTRODUCTION_INFO_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_INTRODUCTION,&text_s);
	GUITEXT_SetFont(INTRODUCTION_INFO_CTRL_ID, &font, &color );
	GUITEXT_SetString(INTRODUCTION_INFO_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);

        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_INTRODUCTION_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)IntroductionWinHandleMsg),
	WIN_ID(MMIZDT_SHB_INTRODUCTION_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_INTRODUCTION),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(INTRODUCTION_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_IntroductionWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_INTRODUCTION_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E UserguideWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)
       GUI_FONT_T font = SONG_FONT_22 ;
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif
	GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
        MMK_SetAtvCtrl(win_id, USERGUIDE_INFO_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_USERGUIDE,&text_s);
	GUITEXT_SetFont(USERGUIDE_INFO_CTRL_ID, &font, &color );
	GUITEXT_SetString(USERGUIDE_INFO_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);

        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_USERGUIDE_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)UserguideWinHandleMsg),
	WIN_ID(MMIZDT_SHB_USERGUIDE_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_USERGUIDE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(USERGUIDE_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_UserguideWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_USERGUIDE_WIN_TAB, PNULL);
}




LOCAL MMI_RESULT_E DownloadWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
	GUI_POINT_T 				point				= {0};
	
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};


    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
         GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_COMMON_BG  ,&lcd_dev_info);
		 point.y = 40;///
    	 GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_SHB  ,&lcd_dev_info);
	    break;

    case MSG_FULL_PAINT:
		 point.y = 40;///
		 GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_SHB  ,&lcd_dev_info);
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif

    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMIZDT_SHB_CLIENTDOWNLOAD_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DownloadWinHandleMsg),
	WIN_ID(MMIZDT_SHB_CLIENTDOWNLOAD_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_CLIENTDOWNLOAD),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
   // CREATE_TEXT_CTRL(USERGUIDE_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_ClientDoladWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_CLIENTDOWNLOAD_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E HotlineWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
	GUI_FONT_T font = SONG_FONT_22 ;
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif

	GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
        MMK_SetAtvCtrl(win_id, HOTLINE_INFO_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_HOTLINE,&text_s);
	GUITEXT_SetFont(HOTLINE_INFO_CTRL_ID, &font, &color );
	GUITEXT_SetString(HOTLINE_INFO_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);

        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_HOTLINE_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HotlineWinHandleMsg),
	WIN_ID(MMIZDT_SHB_HOTLINE_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_HOTLINE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(HOTLINE_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_HotlineWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_HOTLINE_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E AbouthelpWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
       GUI_FONT_T font = SONG_FONT_22 ;   
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif
	GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
        MMK_SetAtvCtrl(win_id, ABOUTHELP_INFO_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_ABOUTHELP,&text_s);
	GUITEXT_SetFont(ABOUTHELP_INFO_CTRL_ID, &font, &color );
	GUITEXT_SetString(ABOUTHELP_INFO_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);

        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_ABOUTHELP_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)AbouthelpWinHandleMsg),
	WIN_ID(MMIZDT_SHB_ABOUTHELP_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_ABOUTHELP),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(ABOUTHELP_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_AbouthelpWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_ABOUTHELP_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E SHBVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //MMI_STRING_T            text = {0};
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
		   GUI_FONT_T font = SONG_FONT_22 ;   
#else
		GUI_FONT_T font = SONG_FONT_28 ;
#endif
    GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       {
            char*           temp_ptr = PNULL;
            uint16          temp_len = 0;
            uint16          uint16_str_len = 0;
            uint16          uint16_str[100] = {0};
            
            MMK_SetAtvCtrl(win_id, SHB_VERSION_INFO_CTRL_ID);
            temp_ptr = (char*)SHB_SOFT_VERSION;
            temp_len = SCI_STRLEN((char*)temp_ptr);
            MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
            text_s.wstr_ptr = uint16_str;
            text_s.wstr_len = uint16_str_len;
            //MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_ABOUTHELP,&text_s);
            GUITEXT_SetFont(SHB_VERSION_INFO_CTRL_ID, &font, &color );
            GUITEXT_SetString(SHB_VERSION_INFO_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
        }
        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_VERSION_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)SHBVersionWinHandleMsg),
	WIN_ID(MMIZDT_SHB_VERSION_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB_VERSION),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(SHB_VERSION_INFO_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_VersionWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_VERSION_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E SHBTxt1WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //MMI_STRING_T            text = {0};
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
    GUI_FONT_T font = SONG_FONT_22 ;   
#else
    GUI_FONT_T font = SONG_FONT_28 ;
#endif

    GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       {
            MMK_SetAtvCtrl(win_id, SHB_TEST_1_CTRL_ID);
            text_s.wstr_ptr = shb_txt1;
            text_s.wstr_len = shb_txt1_len;
            GUITEXT_SetFont(SHB_TEST_1_CTRL_ID, &font, &color );
            GUITEXT_SetString(SHB_TEST_1_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
        }
        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_TXT1_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)SHBTxt1WinHandleMsg),
	WIN_ID(MMIZDT_SHB_TXT1_WIN_ID),
	WIN_TITLE(STXT_SHB_FIRST_ALERT_2),
	WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),	
    CREATE_TEXT_CTRL(SHB_TEST_1_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_Txt1Win(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_TXT1_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E SHBTxt2WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    //MMI_STRING_T            text = {0};
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
		GUI_FONT_T font = SONG_FONT_22 ;   
#else
		GUI_FONT_T font = SONG_FONT_28 ;
#endif
    GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       {
            
            MMK_SetAtvCtrl(win_id, SHB_TEST_2_CTRL_ID);
            text_s.wstr_ptr = shb_txt2;
            text_s.wstr_len = shb_txt2_len;
            GUITEXT_SetFont(SHB_TEST_2_CTRL_ID, &font, &color );
            GUITEXT_SetString(SHB_TEST_2_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
        }
        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
     
    case MSG_KEYDOWN_CANCEL:

    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_SHB_TXT2_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)SHBTxt2WinHandleMsg),
	WIN_ID(MMIZDT_SHB_TXT2_WIN_ID),
	WIN_TITLE(STXT_SHB_FIRST_ALERT_4),
	WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),	
    CREATE_TEXT_CTRL(SHB_TEST_2_CTRL_ID),
    END_WIN
};
PUBLIC void ZdtSHB_Txt2Win(void)
{
    MMK_CreateWin((uint32*)MMIZDT_SHB_TXT2_WIN_TAB, PNULL);
}

#if 1
void ZdtSHB_FirstAlert_ShowTxt(uint32 tex_id,GUI_RECT_T   * p_rect, uint16 color, BOOLEAN is_focus)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
		GUI_FONT_T font = SONG_FONT_22 ;   
#else
		GUI_FONT_T font = SONG_FONT_28 ;
#endif
    MMI_STRING_T    cur_str_t   = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    GUI_RECT_T   cur_rect          = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    
    cur_rect = *p_rect;
    if(is_focus)
    {
        state |= GUISTR_STATE_UNDERLINE;
        text_style.font_color = MMI_RED_COLOR;
    }
    else
    {
        text_style.font_color = color;
    }
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = font;
    

    MMI_GetLabelTextByLang(tex_id, &cur_str_t);

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    return;
}

void ZdtSHB_FirstAlert_Show(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    BOOLEAN is_focus = FALSE; 
    GUI_BORDER_T  border = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state_single =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUISTR_STATE_T      state =           state_single;
    GUISTR_STATE_T      state_focus =  GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK |GUISTR_STATE_SINGLE_LINE |GUISTR_STATE_UNDERLINE;
    GUI_COLOR_T    color = MMI_BLACK_COLOR;
    GUI_FONT_T t_font = SONG_FONT_18;
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);

    border.color = MMI_BLUE_COLOR;
    border.type = GUI_BORDER_ROUNDED;
    border.width = 4;
    
    cur_rect.left   = 0;
    cur_rect.top    = 10; 
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-10;
    cur_rect.bottom = cur_rect.top+36;
    text_style.font_color = color;
    text_style.align = ALIGN_RVMIDDLE;
#if defined(ZDT_MMI_USE_SMALL_FONT)	
    text_style.font = SONG_FONT_22;
#else
    text_style.font = SONG_FONT_28;
#endif	
    if(g_cur_media_select_index == 0)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        state = state_focus;
    }
    else
    {
        state = state_single;
    }
    MMI_GetLabelTextByLang(STXT_SHB_CLOSE, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    cur_rect.left   = 0;
    cur_rect.top    = cur_rect.bottom+20;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = cur_rect.top+40;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
 #if defined(ZDT_MMI_USE_SMALL_FONT)	
    text_style.font = SONG_FONT_22;
#else
    text_style.font = SONG_FONT_28;
#endif	
    state = state_single;
    MMI_GetLabelTextByLang(STXT_SHB_WELCOME, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 


    cur_rect.left   = 0;
    cur_rect.top    = cur_rect.bottom+20;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = cur_rect.top+22;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = t_font;
    state = state_single;
    MMI_GetLabelTextByLang(STXT_SHB_FIRST_ALERT_1, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_rect.left   = 0;
    cur_rect.top    = cur_rect.bottom+5;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = cur_rect.top+22;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = t_font;
    if(g_cur_media_select_index == 1)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        state = state_focus;
    }
    else
    {
        state = state_single;
    }
    MMI_GetLabelTextByLang(STXT_SHB_FIRST_ALERT_2, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_rect.left   = 0;
    cur_rect.top    = cur_rect.bottom+5;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = cur_rect.top+22;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = t_font;
    state = state_single;
    MMI_GetLabelTextByLang(STXT_SHB_FIRST_ALERT_3, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    cur_rect.left   = 0;
    cur_rect.top    = cur_rect.bottom+5;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = cur_rect.top+22;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = t_font;
    if(g_cur_media_select_index == 2)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        state = state_focus;
    }
    else
    {
        state = state_single;
    }
    MMI_GetLabelTextByLang(STXT_SHB_FIRST_ALERT_4, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_rect.left   = 0;
    cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
    cur_rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect.top    = cur_rect.bottom - 60;
    text_style.font_color = color;
    text_style.align = ALIGN_HVMIDDLE;
#if defined(ZDT_MMI_USE_SMALL_FONT)	
		text_style.font = SONG_FONT_22;
#else
		text_style.font = SONG_FONT_28;
#endif	
    if(g_cur_media_select_index == 3)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        state = state_focus;
    }
    else
    {
        state = state_single;
    }
    MMI_GetLabelTextByLang(TXT_COMMON_OK, &cur_str_t);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    MMITHEME_UpdateRect();
    return;
}

LOCAL MMI_RESULT_E FirstAlertWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	
	GUI_FONT_T font = SONG_FONT_22 ;
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif		
	GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
        	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_SHB_FIRST_ALERT,&text_s);
        
        break;

    case MSG_FULL_PAINT:
            ZdtSHB_FirstAlert_Show(win_id);
     break;
     
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_LEFT:
            if(g_cur_media_select_index > 0)
            {
                g_cur_media_select_index--;
            }
            else
            {
                g_cur_media_select_index = 3;
            }
            ZdtSHB_FirstAlert_Show(win_id);
        break;
        
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_RIGHT:
            if(g_cur_media_select_index < 3)
            {
                g_cur_media_select_index++;
            }
            else
            {
                g_cur_media_select_index = 0;
            }
            ZdtSHB_FirstAlert_Show(win_id);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_0:       
    case MSG_APP_1:       
    case MSG_APP_2:       
    case MSG_APP_3:       
    case MSG_APP_4:       
    case MSG_APP_5:       
    case MSG_APP_6:       
    case MSG_APP_7:       
    case MSG_APP_8:       
    case MSG_APP_9:       
    case MSG_APP_STAR:       
    case MSG_APP_HASH:       
    case MSG_APP_OK:
    case MSG_APP_CANCEL:       
    case MSG_APP_RED:       
    case MSG_APP_GREEN:
        if(g_cur_media_select_index == 3)
        {
            MMIZDT_SHB_NV_SetAlert(0);
            SHB_API_SeNetOpen();
            if(SHB_Net_IsInit())
            {
                SHB_HTTP_RegCheck();
            }
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
        if(g_cur_media_select_index == 0)
        {
            SHB_API_SeNetClose();
            MMK_CloseWin(win_id);
        }
        else if(g_cur_media_select_index == 1)
        {
            ZdtSHB_Txt1Win();
        }
        else if(g_cur_media_select_index == 2)
        {
            ZdtSHB_Txt2Win();
        }
        else if(g_cur_media_select_index == 3)
        {
            MMIZDT_SHB_NV_SetAlert(0);
            SHB_API_SeNetOpen();
            if(SHB_Net_IsInit())
            {
                SHB_HTTP_RegCheck();
            }
            MMK_CloseWin(win_id);
        }
        
     break;
              
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
WINDOW_TABLE(MMIZDT_SHB_FIRST_ALERT_WIN_TAB) = 
{
    WIN_FUNC((uint32)FirstAlertWinHandleMsg),
    WIN_ID(MMIZDT_SHB_FIRST_ALERT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
PUBLIC void ZdtSHB_FirstAlertWin(void)
{
    #ifndef WIN32
    if(MMIZDT_SHB_GetAlert())
    #endif
    {
        g_cur_media_select_index = 3;
        MMK_CreateWin((uint32*)MMIZDT_SHB_FIRST_ALERT_WIN_TAB, PNULL);
    }
}

LOCAL BOOLEAN ZdtSHB_GetPBNameByBCDNum(MMIPB_BCD_NUMBER_T* pb_number,MMI_STRING_T* initial_name)
{
    BOOLEAN result = FALSE;
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    MMI_STRING_T name = {0};
    MMI_STRING_T abbr_name = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};
    wchar anim_full_path_wstr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    BOOLEAN is_find = FALSE;
    anim_path.full_path_wstr_ptr = anim_full_path_wstr;

    if(pb_number->number_len > 0)
    {
        name.wstr_ptr = SCI_ALLOC_APP( (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );
        if (PNULL == name.wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(name.wstr_ptr, 0x00, (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );		 
        abbr_name.wstr_ptr = SCI_ALLOC_APP( (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );
        if (PNULL == abbr_name.wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(abbr_name.wstr_ptr, 0x00, (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );		  
        is_find = MMIAPIPB_GetNameAvatarByNumber(pb_number, &name, &abbr_name, &anim_path,  &entry_id, &storage_id,MMIPB_MAX_STRING_LEN, FALSE);
        if(is_find && anim_path.full_path_wstr_len<=0 && abbr_name.wstr_len>0)
        {
            if(initial_name!=PNULL && initial_name->wstr_ptr != PNULL)
            {
                initial_name->wstr_len  = MIN(MMIAPICOM_Wstrlen(abbr_name.wstr_ptr), MMIPB_MAX_STRING_LEN);
                MMIAPICOM_Wstrncpy(initial_name->wstr_ptr, abbr_name.wstr_ptr, abbr_name.wstr_len);
            }
            result =  TRUE;
        }
        SCI_FREE(name.wstr_ptr);
        SCI_FREE(abbr_name.wstr_ptr);
    }
    return result;
}

PUBLIC void ZdtSHB_Bind_SetTxt(void)
{
    MMI_STRING_T prompt_str1   = {0};
    MMI_STRING_T prompt_str2   = {0};
    MMI_STRING_T pb_wstr_num = {0};
    MMI_STRING_T pb_name = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint16          num_wstr[MAX_SHB_APP_NUMBER_SIZE + 1] = {0};
    uint16          num_wstr_len = 0;
    uint16          uint16_str[500] = {0};
    uint16          uint16_str_len = 0;
    
    MMIAPICOM_StrcatFromStrToUCS2(num_wstr, &num_wstr_len, (uint8*)g_shb_sms_bdrq_num,SCI_STRLEN((char *)g_shb_sms_bdrq_num));
    MMIAPICOM_ConvertWStr2Num(num_wstr,num_wstr_len,&pb_num);
    pb_name.wstr_ptr = SCI_ALLOC_APP( (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );
    if (PNULL == pb_name.wstr_ptr)
    {
        return;
    }
    SCI_MEMSET(pb_name.wstr_ptr, 0x00, (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );		  
    if(ZdtSHB_GetPBNameByBCDNum(&pb_num,&pb_name))
    {
        prompt_str1 = pb_name;		  
    }
    else
    {
        prompt_str1.wstr_ptr = num_wstr;
        prompt_str1.wstr_len = num_wstr_len;
    }
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &prompt_str1);
    MMI_GetLabelTextByLang(STXT_SHB_BIND, &prompt_str2);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &prompt_str2);
    GUITEXT_SetString(SHB_BIND_INFO_CTRL_ID, uint16_str,uint16_str_len, TRUE);
    SCI_FREE(pb_name.wstr_ptr);
}

LOCAL MMI_RESULT_E BindQueryWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
//  MMI_STRING_T            text = {0};
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
#if defined(ZDT_MMI_USE_SMALL_FONT)	
	GUI_FONT_T font = SONG_FONT_22 ;
#else
	GUI_FONT_T font = SONG_FONT_28 ;
#endif		
    GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, SHB_BIND_INFO_CTRL_ID);
            GUITEXT_SetFont(SHB_BIND_INFO_CTRL_ID, &font, &color );
            GUITEXT_SetAlign(SHB_BIND_INFO_CTRL_ID,ALIGN_HVMIDDLE);
            ZdtSHB_Bind_SetTxt();
            MMIDEFAULT_TurnOnBackLight();
        break;

    case MSG_FULL_PAINT:
     break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
        SHB_HTTP_ZTE1211_Send(g_shb_sms_bdrq_num,1);
        MMK_CloseWin(win_id);
        
     break;
     
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:     	
        SHB_HTTP_ZTE1211_Send(g_shb_sms_bdrq_num,0);
        MMK_CloseWin(win_id);
        break;
         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMIZDT_SHB_BIND_QUERY_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)BindQueryWinHandleMsg),
	WIN_ID(MMIZDT_SHB_CONFIRM_WIN_ID),
	WIN_TITLE(TXT_ZDT_SHB),
	WIN_SOFTKEY(STXT_SHB_AGREE, TXT_NULL, STXT_SHB_DISAGREE),	
    CREATE_TEXT_CTRL(SHB_BIND_INFO_CTRL_ID),
    END_WIN
};

PUBLIC BOOLEAN ZdtSHB_IsOpenBindWin(void)
{
    return MMK_IsOpenWin(MMIZDT_SHB_CONFIRM_WIN_ID);
}

PUBLIC void ZdtSHB_OpenBindWin(void)
{
    ZDT_LOG("ZdtSHB_OpenBindWin %s",g_shb_sms_bdrq_num);
    MMK_CreateWin((uint32*)MMIZDT_SHB_BIND_QUERY_WIN_TAB, PNULL);
}

#endif

#if 1  //联系人窗口
#define ZDT_PB_ICON_LEFT_START		0
#define ZDT_PB_ICON_TOP_START		10
#define ZDT_PB_ICON_WIDTH		120
#define ZDT_PB_ICON_HEIGHT		75
#define ZDT_PB_TEXT_HEIGHT		30

uint16 m_yx_cur_whitelist_idx = 0;
static uint16 m_yx_whitelist_totalnum=0;
static uint16 m_yx_whitelist_pagenum=0;
static uint16 m_yx_whitelist_curpage=0;
static uint16 m_yx_whitelist_curindex=0;
static int s_pb_tp_x;
static int s_pb_tp_y;
static BOOLEAN s_is_pb_tp_down = FALSE;
LOCAL THEMELIST_ITEM_STYLE_T new_style = {0};

PUBLIC void ZDT_PB_GetContactNameByNumber(uint8 *number, uint16 *dst_name)
{
    uint8 i = 0;
    if(number == NULL || dst_name == NULL)
    {
        return;
    }
#ifdef ZDT_PLAT_YX_SUPPORT
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        
        if(strcmp(yx_DB_White_Reclist[i].white_num,  number) == 0)
        {
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,dst_name, 100);
        }
    }  
#endif

}

extern uint16  MMIZDT_PB_GetNext(uint16 white_idx)
{
    uint16 i = 0;
    uint16 next_idx = 0;
    
#ifdef ZDT_PLAT_YX_SUPPORT
    for(i = white_idx; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_White_Reclist[i].white_num) > 0)
        {
            next_idx = i+1;
            break;
        }
    }

    if(next_idx == 0)
    {
        for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
        {
            if(yx_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_White_Reclist[i].white_num) > 0)
            {
                next_idx = i+1;
                break;
            }
        }
    }
#endif
    return next_idx;
}

BOOLEAN MMIZDT_PB_Call_Start(uint16 white_idx)
{
    uint8 i = 0;
    uint16 tempStr[101] ={0};
#ifdef ZDT_PLAT_YX_SUPPORT
    if(white_idx == 0 || white_idx > YX_DB_WHITE_MAX_SUM || SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num) == 0)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        return FALSE;
    }
    SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
    ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[white_idx-1].white_name, SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_name) ,tempStr, 100);            
        
    MMIZDT_MakeAll(yx_DB_White_Reclist[white_idx-1].white_num,SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num),tempStr);
#endif
    return TRUE;
}

void  MMIZDT_PB_DrawBg(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    return;
}

void  MMIZDT_PB_DrawCur(MMI_WIN_ID_T win_id,uint16 index)
{
    uint16 pb_sum = 0;
    uint16 i = 0;
    uint32 image_id = IMAGE_ZDT_PB_FATHER;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};
    GUI_POINT_T                 dis_point = {0,0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
#if defined(ZDT_MMI_USE_SMALL_FONT)	  
    GUI_FONT_T f_big =SONG_FONT_22;
#else
    GUI_FONT_T f_big =SONG_FONT_28;
#endif
    uint16 TempStr[101] ={0};
    uint16 ba_wStr[2] = {0x7238,0};
    uint16 ma_wStr[2] = {0x5988,0};
    uint16 ye_wStr[2] = {0x7237,0};
    uint16 nai_wStr[2] = {0x5976,0};
    #if 1 // xiongkai    通讯录界面图标不一致
    uint16 fu_wStr[2] = {0x7236,0};//fu
    uint16 mu_wStr[2] = {0x6BCD,0};//mu qin
    uint16 shu_wStr[2] = {0x53D4,0};//shushu
    uint16 yi_wStr[2] = {0x59E8,0};//yi
    uint16 waigong_wStr[3] = {0x5916, 0x516C,0};//wai gong
    uint16 waipo_wStr[3] = {0x5916, 0x5A46,0};//wai po    
    #endif
	if(index > 0)
	{
		index--;
	}
     //if(index != m_yx_whitelist_pagenum)
     {
        MMIZDT_PB_DrawBg();
    	
    	 for(i=(index)*4;i<(index)*4+4;i++)
    	 {
#ifdef ZDT_PLAT_YX_SUPPORT
            if(yx_DB_White_Reclist[i].db_id == 0)
            {
                break;
            }
            pb_sum++;
            SCI_MEMSET(TempStr,0,(101)*sizeof(uint16));	 
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,TempStr, 100);
#else
                break;
#endif

#if 0	// xiongkai   通讯录界面图标不一致
    	 	if(MMIAPICOM_Wstrstr(TempStr,ba_wStr))
    	 	{
                    image_id = IMAGE_ZDT_PB_FATHER;
    	 	}
    		else if(MMIAPICOM_Wstrstr(TempStr,ma_wStr))
    	 	{
                    image_id = IMAGE_ZDT_PB_MOTHER;
    	 	} 
    		else if(MMIAPICOM_Wstrstr(TempStr,ye_wStr))
    	 	{
                    image_id = IMAGE_ZDT_PB_GRANDPA;
    	 	}
    		else if(MMIAPICOM_Wstrstr(TempStr,nai_wStr))
    	 	{
                    image_id = IMAGE_ZDT_PB_GRANDMA;
    	 	}
#else			
	 	if(MMIAPICOM_Wstrstr(TempStr,ba_wStr)||MMIAPICOM_Wstrstr(TempStr,fu_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_FATHER;
	 	}
		else if(MMIAPICOM_Wstrstr(TempStr,ma_wStr)||MMIAPICOM_Wstrstr(TempStr,mu_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_MOTHER;
	 	} 
		else if(MMIAPICOM_Wstrstr(TempStr,ye_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_GRANDPA;
	 	}
		else if(MMIAPICOM_Wstrstr(TempStr,nai_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_GRANDMA;
	 	}
		else if(MMIAPICOM_Wstrstr(TempStr,waigong_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_WAIGONG;
	 	}
		else if(MMIAPICOM_Wstrstr(TempStr,waipo_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_WAIPO;
	 	}	 	
	 	else if(MMIAPICOM_Wstrstr(TempStr,shu_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_UNCLE;
	 	}
	 	else if(MMIAPICOM_Wstrstr(TempStr,yi_wStr))
	 	{
	            image_id = IMAGE_ZDT_PB_AUNT;
	 	}	
#endif	
    		else
    		{
    		#if 1 // xiongkai   亲人号码前面图标固定用一个。
		    image_id = IMAGE_ZDT_PB_OTHERMAN;	 
		#else	
    			if(i%2)
    			{
                          image_id = IMAGE_ZDT_PB_OTHERWOMAN;
    			}
    			else
    			{
                          image_id = IMAGE_ZDT_PB_OTHERMAN;
    			}
		#endif		
    	 	}	
            cur_rect.left = ZDT_PB_ICON_LEFT_START+ ((i%2)*ZDT_PB_ICON_WIDTH);
            cur_rect.right = cur_rect.left+ZDT_PB_ICON_WIDTH-1;
 		if(i/2%2)
 		{
                cur_rect.top = ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START;
 		}
		else
		{
                cur_rect.top = ZDT_PB_ICON_TOP_START;
 		}
            cur_rect.bottom = cur_rect.top+ZDT_PB_ICON_HEIGHT-1;
			dis_point.x = cur_rect.left;
			dis_point.y = cur_rect.top;
            GUIRES_DisplayImg(&dis_point,
                    &rect,
                    PNULL,
                    win_id,
                    image_id,
                    &lcd_dev_info);
            
            cur_rect.top = cur_rect.bottom+1;
            cur_rect.bottom = cur_rect.top + ZDT_PB_TEXT_HEIGHT-1;
            
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_WHITE_COLOR;
            #ifdef ZDT_PCBA_Z605_SUPPORT
            if(m_yx_whitelist_curindex == (i%4))
            {
                text_style.font_color = MMI_DARK_BLUE_COLOR;
            }
            #endif
            cur_str_t.wstr_ptr = TempStr;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(TempStr);

            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
	 }
         
        if(pb_sum == 0)
        {
            cur_rect.left   = 0;
            cur_rect.top    = 0; 
            cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
            cur_rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
            //空7A7A
            TempStr[0] = 0x7A7A;
            TempStr[1] = 0;
            
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_WHITE_COLOR;

            cur_str_t.wstr_ptr = TempStr;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(TempStr);

            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
        }
     }
     MMITHEME_UpdateRect();
    return;
}

void  MMIZDT_PB_ShowInit(void)
{
        uint16 i = 0;
        m_yx_whitelist_curindex = 0;
	 m_yx_whitelist_totalnum=0;
#ifdef ZDT_PLAT_YX_SUPPORT
    	 for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    	 {
    	 	if(yx_DB_White_Reclist[i].db_id)
    	 	{
    	 		m_yx_whitelist_totalnum ++;
    	 	}
    	 }
#endif
	 if(m_yx_whitelist_totalnum%4)
	 {
	 	m_yx_whitelist_pagenum = m_yx_whitelist_totalnum/4 + 1;
	 }
	 else
	 {
	 	m_yx_whitelist_pagenum = m_yx_whitelist_totalnum/4;
	 }
	 m_yx_whitelist_curpage= 1;
       return;
}

void  MMIZDT_PB_Show(MMI_WIN_ID_T win_id)
{
    MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
}

#ifdef BAIRUI_VIDEOCHAT_SUPPORT
extern VIDEO_USER_INFO video_users[MAX_VIDEO_USERS_COUNT];
PUBLIC void GetNameAndIndexByTalkUserId(int talk_user_id, uint16 *name_wstr, uint8 *index)
{
    int i = 0;
    for(i = 0 ; i < MAX_VIDEO_USERS_COUNT; i++)
    {
        if(talk_user_id == atoi(video_users[i].phone_user_id))
        {
            ZDT_PB_GetContactNameByNumber(video_users[i].phone_num, name_wstr);
            *index = i;
            break;
        }

    }
}
//yangyu end

PUBLIC MMI_IMAGE_ID_T MMIZDT_GetHeadImageId(int appUserId)
{
    uint16 ba_wStr[2] = {0x7238,0};//ba
    uint16 fu_wStr[2] = {0x7236,0};//fu
    uint16 ma_wStr[2] = {0x5988,0};//ma
    uint16 mu_wStr[2] = {0x6BCD,0};//mu qin
    uint16 ye_wStr[2] = {0x7237,0};//ye
    uint16 nai_wStr[2] = {0x5976,0};//nai
    uint16 shu_wStr[2] = {0x53D4,0};//shushu
    uint16 yi_wStr[2] = {0x59E8,0};//yi
    uint16 waigong_wStr[3] = {0x5916, 0x516C,0};//wai gong
    uint16 waipo_wStr[3] = {0x5916, 0x5A46,0};//wai po
    uint16 tempStr[101] ={0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint8 i = 0;

    if(appUserId > 0)
    {
        //GetNameAndIndexByTalkUserId(appUserId, tempStr, &i);
        //cur_str_t.wstr_ptr = tempStr;
        //cur_str_t.wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
         //int i = 0;
        for(i = 0 ; i < MAX_VIDEO_USERS_COUNT; i++)
        {
            if(appUserId == atoi(video_users[i].phone_user_id))
            {
                //ZDT_PB_GetContactNameByNumber(video_users[i].phone_num, name_wstr);
                SCI_MEMCPY(tempStr, video_users[i].family_relative, 16*2);
                break;
            }

        }
        SCI_TRACE_LOW("MMIZDT_GetHeadImageId have name");
    }
    else
    {
        return IMAGE_NULL;
    }
    if(MMIAPICOM_Wstrstr(tempStr,ba_wStr)||MMIAPICOM_Wstrstr(tempStr,fu_wStr))
 	{
            image_id = IMAGE_ZDT_PB_FATHER;
 	}
	else if(MMIAPICOM_Wstrstr(tempStr,ma_wStr)||MMIAPICOM_Wstrstr(tempStr,mu_wStr))
 	{
            image_id = IMAGE_ZDT_PB_MOTHER;
 	} 
	else if(MMIAPICOM_Wstrstr(tempStr,ye_wStr))
 	{
            image_id = IMAGE_ZDT_PB_GRANDPA;
 	}
	else if(MMIAPICOM_Wstrstr(tempStr,nai_wStr))
 	{
            image_id = IMAGE_ZDT_PB_GRANDMA;
 	}
	else if(MMIAPICOM_Wstrstr(tempStr,waigong_wStr))
 	{
            image_id = IMAGE_ZDT_PB_WAIGONG;
 	}
	else if(MMIAPICOM_Wstrstr(tempStr,waipo_wStr))
 	{
            image_id = IMAGE_ZDT_PB_WAIPO;
 	}	 	
 	else if(MMIAPICOM_Wstrstr(tempStr,shu_wStr))
 	{
            image_id = IMAGE_ZDT_PB_UNCLE;
 	}
 	else if(MMIAPICOM_Wstrstr(tempStr,yi_wStr))
 	{
            image_id = IMAGE_ZDT_PB_AUNT;
 	}
	else
	{
	    #if 1 // xiongkai      亲人号码前面图标固定用一个。
                  image_id = IMAGE_ZDT_PB_OTHERMAN;	 
	    #else
		if(i%2)
		{
                  image_id = IMAGE_ZDT_PB_OTHERWOMAN;
		}
		else
		{
                  image_id = IMAGE_ZDT_PB_OTHERMAN;
		}
	    #endif
 	}
    return image_id;

}
#endif

PUBLIC BOOLEAN MMIZDT_IsPhoneBookEmpty()
{
    BOOLEAN ret = TRUE;
    uint16 count = 0;
    int i = 0;
#ifdef ZDT_PLAT_YX_SUPPORT
    for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    {
       if(yx_DB_White_Reclist[i].db_id)
       {
           count++;
       }
    }
#endif
    if(count > 0)
    {
        ret = FALSE; 
    }
    return ret;

}

void  MMIZDT_PB_ShowList(MMI_WIN_ID_T win_id)
{
    BOOLEAN ret = FALSE;
    MMI_STRING_T    cur_str_t   = {0};
    uint16 tempStr[101] ={0};
    uint16 ba_wStr[2] = {0x7238,0};//ba
    uint16 fu_wStr[2] = {0x7236,0};//fu
    uint16 ma_wStr[2] = {0x5988,0};//ma
    uint16 mu_wStr[2] = {0x6BCD,0};//mu qin
    uint16 ye_wStr[2] = {0x7237,0};//ye
    uint16 nai_wStr[2] = {0x5976,0};//nai
    uint16 shu_wStr[2] = {0x53D4,0};//shushu
    uint16 yi_wStr[2] = {0x59E8,0};//yi
    uint16 waigong_wStr[3] = {0x5916, 0x516C,0};//wai gong
    uint16 waipo_wStr[3] = {0x5916, 0x5A46,0};//wai po
    uint16 i = 0, j = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint16 position = 0;


    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    //add by James li begin
    uint16 total_item_num = 0;
    MMI_STRING_T    empty_str = {0};
    GUILIST_EMPTY_INFO_T empty_info = {0};
    //add by James li end

    #if 1//def NEW_LEFT_RIGHT_STYLE   
    const uint16 h_gap = 10;
    const uint16 w_gap = 2;
    const uint16 img_height = 60;
    const uint16 img_width = 60;
    const uint16 height = img_height + 2*h_gap;
    GUI_RECT_T l_rect0 = { w_gap, h_gap,  img_width + w_gap,  img_height+h_gap};//bg
    GUI_RECT_T l_rect1 = { img_width+w_gap+8,  h_gap,  MMI_MAINSCREEN_WIDTH-2*h_gap , img_height+h_gap};//text
    //GUI_RECT_T r_rect0 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap, h_gap,   MMI_MAINSCREEN_WIDTH - w_gap,  img_height+h_gap};//bg
    //GUI_RECT_T r_rect1 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap+2,  h_gap,  MMI_MAINSCREEN_WIDTH - 2*w_gap , img_height+h_gap};//text
    
    
    //uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS);
    SCI_MEMCPY(&new_style, pItemStyle, sizeof(*pItemStyle));
   
    new_style.height = new_style.height_focus = img_height + h_gap*2;
   
    
    new_style.content[0].rect = new_style.content[0].rect_focus = l_rect0;
    new_style.content[1].rect = new_style.content[1].rect_focus = l_rect1;
    //new_style.content[1].font= new_style.content[1].font_focus = SONG_FONT_26;
   
  
    #endif

    
    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 
    #if 1//def NEW_LEFT_RIGHT_STYLE 
    item_t.item_style =  GUIITEM_STYLE_CUSTOMIZE;
    #endif



    
    if(0)//if(isLeft)
    {
        //item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS; 
        //item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT; // no effect
        //item_data.item_content[0].font_color_id=MMITHEME_COLOR_9; // no effect
        
        item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS;
    }
    else
    {
        //item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS; 
        //item_t.right_item_style = GUIITEM_STYLE_ONE_LINE_TEXT;// no effect
        //item_data.item_content[0].font_color_id=MMITHEME_COLOR_BLUE;// no effect
        item_t.right_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS;
    }
     //item_t.left_item_style = GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;

     

#ifdef ZDT_PLAT_YX_SUPPORT
    for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    {
       if(yx_DB_White_Reclist[i].db_id)
       {
           m_yx_whitelist_totalnum++;
       }
    }
#endif
    GUILIST_RemoveAllItems(MMIZDT_PB_LIST_CTRL_ID);

    for(i = 0 ;i < m_yx_whitelist_totalnum ; i++)
    {
#ifdef ZDT_PLAT_YX_SUPPORT
        if(yx_DB_White_Reclist[i].db_id == 0)
        {
            break;
        }
        SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));	 
        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,tempStr, 100);
#else
        break;
 #endif
	 	if(MMIAPICOM_Wstrstr(tempStr,ba_wStr)||MMIAPICOM_Wstrstr(tempStr,fu_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_FATHER;
	 	}
		else if(MMIAPICOM_Wstrstr(tempStr,ma_wStr)||MMIAPICOM_Wstrstr(tempStr,mu_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_MOTHER;
	 	} 
		else if(MMIAPICOM_Wstrstr(tempStr,ye_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_GRANDPA;
	 	}
		else if(MMIAPICOM_Wstrstr(tempStr,nai_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_GRANDMA;
	 	}
		else if(MMIAPICOM_Wstrstr(tempStr,waigong_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_WAIGONG;
	 	}
		else if(MMIAPICOM_Wstrstr(tempStr,waipo_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_WAIPO;
	 	}	 	
	 	else if(MMIAPICOM_Wstrstr(tempStr,shu_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_UNCLE;
	 	}
	 	else if(MMIAPICOM_Wstrstr(tempStr,yi_wStr))
	 	{
                image_id = IMAGE_ZDT_PB_AUNT;
	 	}
		else
		{
	    #if 1  //xiongkai    亲人号码前面图标固定用一个。
                  image_id = IMAGE_ZDT_PB_OTHERMAN;	 
	    #else		
			if(i%2)
			{
                      image_id = IMAGE_ZDT_PB_OTHERWOMAN;
			}
			else
			{
                      image_id = IMAGE_ZDT_PB_OTHERMAN;
			}
	    #endif		
	 	}	
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[0].item_data.image_id = image_id;
        

        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = tempStr;
        //MMIAPICOM_StrToWstr(content , item_data.item_content[0].item_data.text_buffer.wstr_ptr);// luan ma,Garbled
        //GUI_GBToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese

        //GUI_GB2UCS(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        //GUI_GBKToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        //GUI_UTF8ToWstr(item_data.item_content[1].item_data.text_buffer.wstr_ptr, GUILIST_STRING_MAX_NUM + 1, content, SCI_STRLEN(content));

        item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
        ret = GUILIST_AppendItem (MMIZDT_PB_LIST_CTRL_ID, &item_t);
        if(ret)
        {
            CTRLLIST_SetItemUserData(MMIZDT_PB_LIST_CTRL_ID, position, &i); 
            #if 1//def NEW_LEFT_RIGHT_STYLE 
            GUILIST_SetItemStyleEx(MMIZDT_PB_LIST_CTRL_ID, position, &new_style );
            #endif
            position++;   
        }
        
    }

    total_item_num = CTRLLIST_GetTotalItemNum(MMIZDT_PB_LIST_CTRL_ID);

    if (0 == total_item_num)
    {
       MMI_GetLabelTextByLang(STR_CONTA_NOTE_NO_CONTA_TEXT, &empty_str);
	empty_info.text_buffer = empty_str;
	CTRLLIST_SetEmptyInfo(MMIZDT_PB_LIST_CTRL_ID, &empty_info);
    }
    
}


void  MMIZDT_PB_TP_Move(MMI_WIN_ID_T win_id,uint8 dir)
{
    if(dir) 
    {
        	if(m_yx_whitelist_curpage>1)
        		m_yx_whitelist_curpage--;
    }
    else
    {
        	if(m_yx_whitelist_curpage<m_yx_whitelist_pagenum)
        		m_yx_whitelist_curpage++;
    }
    MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
}

LOCAL MMI_RESULT_E  HandleZDT_PBWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMIZDT_PB_ShowInit();
        }
        break;
        
        case MSG_FULL_PAINT:
                MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_GET_FOCUS:
                MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_pb_tp_x = point.x;
            s_pb_tp_y = point.y;
            s_is_pb_tp_down = TRUE;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_pb_tp_down)
            {
                offset_y =  point.y - s_pb_tp_y;
                offset_x =  point.x - s_pb_tp_x;
                if(offset_x <= -(20))	
                {
                    MMIZDT_PB_TP_Move(win_id,0);
                }	
                else if(offset_x >= (20))
                {
                    MMIZDT_PB_TP_Move(win_id,1);
                }
                else
                {
        		  if(point.y > ZDT_PB_ICON_TOP_START && point.y<(ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
        		  {
        		  	if(point.x<120)
        		  	{
        		  		MMIZDT_PB_Call_Start(1+(m_yx_whitelist_curpage-1)*4);
        		  	}
        			else
        			{
        				MMIZDT_PB_Call_Start(2+(m_yx_whitelist_curpage-1)*4);
        			}
        		  }
        		  else if(point.y> (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START) 
                      && point.y< (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
        		  {
        		  	if(point.x<120)
        		  	{
        		  		MMIZDT_PB_Call_Start(3+(m_yx_whitelist_curpage-1)*4);
        		  	}
        			else
        			{
        				MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        			}		  
        		  }
                }
            }
            s_is_pb_tp_down = FALSE;
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(point.y > ZDT_PB_ICON_TOP_START && point.y<(ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
            {
                if(point.x<120)
                {
                    MMIZDT_PB_Call_Start(1+(m_yx_whitelist_curpage-1)*4);
                }
                else
                {
                    MMIZDT_PB_Call_Start(2+(m_yx_whitelist_curpage-1)*4);
                }
            }
            else if(point.y> (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START) 
              && point.y< (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
            {
                if(point.x<120)
                {
                    MMIZDT_PB_Call_Start(3+(m_yx_whitelist_curpage-1)*4);
                }
                else
                {
                    MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
                }		  
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        
    case MSG_KEYDOWN_CANCEL:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_DOWN:
            m_yx_whitelist_curindex = 0;
            MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            m_yx_whitelist_curindex = 0;
            MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:
            if(m_yx_whitelist_curindex > 0)
            {
                m_yx_whitelist_curindex--;
                MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
            }
            else
            {
                MMIZDT_PB_TP_Move(win_id,1);
            }
        break;

    case MSG_KEYDOWN_RIGHT:
            m_yx_whitelist_curindex++;
            if(m_yx_whitelist_curindex > 3)
            {
                m_yx_whitelist_curindex = 0;
                MMIZDT_PB_TP_Move(win_id,0);
            }
            else
            {
                MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
            }
        break;
        
    case MSG_CLOSE_WINDOW:
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

LOCAL MMI_RESULT_E  HandleZDT_NewPbWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_PB_LIST_CTRL_ID;
    BOOLEAN ret = FALSE;
    uint32 pos_user_data = 0; // position user data
    uint8 * p_index = PNULL;
    GUILIST_INIT_DATA_T list_init = {0}; 
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T rect = {0,0,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_WIDTH -1)};
            //MMIZDT_PB_ShowInit();
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_HEIGHT-1);//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_WIDTH -1);//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_HEIGHT -1);
            
            list_init.type = GUILIST_TEXTLIST_E;
                        
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);  
#ifdef ZDT_PLAT_YX_SUPPORT
            ret = GUILIST_SetMaxItem(ctrl_id, YX_DB_WHITE_MAX_SUM, FALSE);
#else
            ret = GUILIST_SetMaxItem(ctrl_id, 20, FALSE);
#endif
            //GUILIST_SetRect(MMK_ConvertIdToHandle(ctrl_id), &rect);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            //xiongkai del  这里导致进电话本会卡。MMIAPICC_DecMissedCallNumber(MMIAPICC_GetMissedCallNumber());//remove miss call status bar icon
            //MMIAPICOM_StatusAreaSetMissedCallState(FALSE); //remove miss call status bar icon
        }
        break;
        
        case MSG_FULL_PAINT:
                //MMIZDT_PB_Show(win_id);
                MMIZDT_PB_ShowList(win_id);
            break;
            
        case MSG_GET_FOCUS:
                //MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
	 case MSG_KEYUP_OK:
	 case MSG_KEYUP_GREEN:
		 {
			uint16 num_index = GUILIST_GetCurItemIndex(ctrl_id);
			GUILIST_GetItemData(ctrl_id, num_index, &pos_user_data);// user_data stand position
			MMIZDT_PB_Call_Start(pos_user_data+1);
		 }
	 	break;
#if 0
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        {
        //case MSG_KEYDOWN_WEB:
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
            MMIZDT_PB_Call_Start(pos_user_data+1);
            //ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_CTL_MIDSK  index=%d, pos_user_data=%d" , index, pos_user_data);
        }

        break;
#endif
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_pb_tp_x = point.x;
            s_pb_tp_y = point.y;
            s_is_pb_tp_down = TRUE;
            
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(point.x - s_pb_tp_x > 120 && z_abs(point.y - s_pb_tp_y) < 100)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
        
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
                
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            //MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        

        
    case MSG_KEYDOWN_DOWN:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:

        break;

    case MSG_KEYDOWN_RIGHT:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


#if 0
WINDOW_TABLE( MMIZDT_WATCH_PB_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_PBWinMsg),    
    WIN_ID( MMIZDT_WATCH_PB_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};
#else
WINDOW_TABLE( MMIZDT_WATCH_PB_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_NewPbWinMsg),    
    WIN_ID( MMIZDT_WATCH_PB_WIN_ID ),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIZDT_PB_LIST_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
#endif

BOOLEAN MMIZDT_OpenPBWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_PB_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_ClosePBWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_PB_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_PB_WIN_ID);
    }
    return TRUE;
}


#endif

#if 1  //天气窗口
#ifdef ZDT_PLAT_YX_SUPPORT
extern YX_WEATHER_INFO_T g_yx_wt_info;
#endif

LOCAL MMI_RESULT_E  HandleZDT_WeatherWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );



WINDOW_TABLE( MMIZDT_WEATHER_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_WeatherWinMsg),    
    WIN_ID( MMIZDT_WEATHER_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};


#define WEATHER_EXPIRE_MINUTES  30//120


LOCAL uint8 weather_timer_id = 0;
LOCAL uint16 weather_click_count = 0;
LOCAL uint16 wt_tp_down_x = 0;
LOCAL uint16 wt_tp_down_y = 0;
LOCAL SCI_TIME_T last_wt_time = {0};

LOCAL void MMIZDT_StartWeatherTimer()
{
    if(weather_timer_id != 0)
    {
        MMK_StopTimer(weather_timer_id);
    }
    weather_timer_id =  MMK_CreateWinTimer(MMIZDT_WEATHER_WIN_ID, 60*1000,TRUE);

}

LOCAL void MMIZDT_StopWeatherTimer()
{
    if(weather_timer_id != 0)
    {
        MMK_StopTimer(weather_timer_id);
    }
}



PUBLIC BOOLEAN MMIZDT_IsInWeatherWin()
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId() ;

    return win_id == MMIZDT_WEATHER_WIN_ID;
}

PUBLIC BOOLEAN MMIZDT_IsInWeatherWinHandle(MMI_HANDLE_T win_handle)
{
    return MMK_GetWinId( win_handle) == MMIZDT_WEATHER_WIN_ID;
}


PUBLIC void MMIZDT_OpenWeatherWin()
{
   if(MMK_GetFocusWinId() != MMIZDT_WEATHER_WIN_ID)
   {
        MMK_CreateWin((uint32*)MMIZDT_WEATHER_WIN_TAB,PNULL);
   }
}

PUBLIC void MMK_CloseWeatherWin()
{
    if(MMK_IsOpenWin(MMIZDT_WEATHER_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WEATHER_WIN_ID);
    }
}

#ifdef WIN32
LOCAL void ZDT_InitWeather()
{
#ifdef ZDT_PLAT_YX_SUPPORT
	uint16 shenzhen_city[3] = {0x6DF1, 0x5733 , 0};//shenzhen
	//uint16 shenzhen_city[3] = {0x4178, 0x5958 ,0};//shenzhen
	SCI_MEMCPY(g_yx_wt_info.city_name, shenzhen_city, 3*sizeof(uint16));
	g_yx_wt_info.min_degree = 20;
	g_yx_wt_info.max_degree = 30;
	g_yx_wt_info.cur_degree = 26;
	g_yx_wt_info.num = 1;
#endif
	
	
}
#endif

LOCAL BOOLEAN Is_WeatherExpired()
{
	BOOLEAN ret = TRUE;	
	uint8 year_str[5] = {0};
	uint8 mon_str[3] = {0};
	uint8 day_str[3] = {0};
	
	SCI_TIME_T time = {0};
	SCI_DATE_T date = {0};
	TM_GetSysTime(&time);
	TM_GetSysDate(&date);
	#if 0
	SCI_MEMCPY(year_str, g_yx_wt_info.date_str+0, 4);
	SCI_MEMCPY(mon_str, g_yx_wt_info.date_str+5, 2);
	SCI_MEMCPY(day_str, g_yx_wt_info.date_str+8, 2);
	
	if(atoi(year_str) == date.year 
		&& atoi(mon_str) == date.mon
		&& atoi(day_str) == date.mday
		)
	{
		ret = FALSE;
	}
	#endif
	if(time.hour < last_wt_time.hour //next day
		|| (time.hour*60 + time.min) - (last_wt_time.hour*60 + last_wt_time.min) >= WEATHER_EXPIRE_MINUTES //2hour

	)
	{
		ret = TRUE;
		last_wt_time = time;

	}
	else
	{
		ret = FALSE;

	}
	ZDT_LOG("zdt__ Is_WeatherExpired, ret=%d", ret);
	return ret;
}


/*LOCAL*/ BOOLEAN Is_WeatherGetFromServer()
{
	BOOLEAN ret = TRUE;
#ifdef ZDT_PLAT_YX_SUPPORT
	if((g_yx_wt_info.min_degree == 0 &&  g_yx_wt_info.max_degree == 0)

		|| Is_WeatherExpired()
	)
	{
		ret = FALSE;
	}
#else
		ret = FALSE;
#endif
	ZDT_LOG("zdt__ Is_WeatherGetFromServer, ret=%d", ret);
	return ret;
}


LOCAL void DisplayWeather(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    SCI_TIME_T time = {0};
    GUI_POINT_T point = {0};
    const uint16 img_width = 82;
    const uint16 img_height = 82;
    //add by James li begin
    MMI_STRING_T	display_string	= {0};
    wchar	wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]	= {0};
    //add by James li end
   
    MMI_IMAGE_ID_T weather_img_id = IMAGE_WEATHER_UNKNOWN;

    uint16 qing_wstr[2] = {0x6674, 0};//qing
    uint16 yin_wstr[2] = {0x9634, 0};//yin
    uint16 yu_wstr[2] = {0x96E8, 0};//yu
    uint16 xue_wstr[2] = {0x96EA, 0};//xue


    GUI_RECT_T	wt_rect={150,60,240,100};
    GUI_RECT_T	temperature_rect={145,110,(MMI_MAINSCREEN_WIDTH -1),160};
    GUI_RECT_T	update_time_rect={150,169,(MMI_MAINSCREEN_WIDTH -1),199};	
    GUI_RECT_T	city_rect={42,10,(MMI_MAINSCREEN_WIDTH -1),40};
    GUI_RECT_T	sim_rect={0,200,(MMI_MAINSCREEN_WIDTH -1),240};//add by James li
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();


    GUI_RECT_T	text_rect={0};
    MMI_STRING_T tmp_string = {0};
    MMI_STRING_T        content_str = {0};
    MMI_STRING_T        update_time_str = {0};	
    wchar               content_text[100 + 1] = {0};
    wchar               content_text_1[100 + 1] = {0};	
    uint16 uint16_str_len = 0;
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_TEXT_ID_T weather_string_id;
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;

    uint8 temp_str[32] = {0};
    uint8 temp_wstr[32] = {0};
    
    TM_GetSysTime(&time);
    ZDT_LOG("zdt__ DisplayWeather, ");

    //GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);
    
    point.x = 0;
    point.y = 0 ;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_BIG_WEATHER_BG,&lcd_dev_info); 
#ifdef ZDT_PLAT_YX_SUPPORT
	//city
	tmp_string.wstr_ptr = g_yx_wt_info.city_name;
	tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
	text_style.align = ALIGN_LEFT;
	text_style.font = SONG_FONT_24;
	text_style.font_color = MMI_WHITE_COLOR;
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&city_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&city_rect,       //用户要剪切的实际区域
		        (const MMI_STRING_T    *)&tmp_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	

	//weather icon  
	switch(g_yx_wt_info.num)
	{
		case 1:
			weather_img_id = IMAGE_BIG_WEATHER_SUNNY;
			weather_string_id=TXT_WEATHER_SUNNY;
			break;
		case 2:
			weather_img_id = IMAGE_BIG_WEATHER_CLOUD;
			weather_string_id=TXT_WEATHER_RAIN;			
			break;
		case 3:
			weather_img_id = IMAGE_BIG_WEATHER_RAIN;
			weather_string_id=TXT_WEATHER_CLOUD;					
			break;
		case 4:
			weather_img_id = IMAGE_BIG_WEATHER_SNOW;
			weather_string_id=TXT_WEATHER_SNOW;					
			break;
		default:
			weather_img_id = IMAGE_BIG_WEATHER_UNKNOW;
			weather_string_id=TXT_WEATHER_UNKNOW;					
			break;				
	}
 
	point.x = 40;
	point.y = 60 ;
	GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, weather_img_id,&lcd_dev_info); 
	

	//weather text
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)weather_string_id,&tmp_string);
	text_style.align = ALIGN_LEFT;
	text_style.font_color = MMI_WHITE_COLOR;
	text_style.font = SONG_FONT_32;
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&wt_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&wt_rect,       //用户要剪切的实际区域
		        (const MMI_STRING_T    *)&tmp_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	
	
	//temperature
	memset(temp_str,0,sizeof(temp_str));
	sprintf((char*) temp_str, "%d", g_yx_wt_info.cur_degree);
	MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, temp_str, strlen(temp_str));
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WEATHER_TEMPERATURE_DU,&tmp_string);
	MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &tmp_string);
	text_style.font = SONG_FONT_40;	
	text_style.align = ALIGN_LEFT;
	text_style.font_color = MMI_YELLOW_COLOR;
	content_str.wstr_ptr = content_text;
	content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&temperature_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&temperature_rect,       //用户要剪切的实际区域
		        (const MMI_STRING_T    *)&content_str,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	

	//update time
        text_style.font = SONG_FONT_28;
        text_style.font_color = MMI_WHITE_COLOR;	
	 text_style.align=ALIGN_LEFT; 		
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;		
		
        memset(temp_str,0,sizeof(temp_str));
        sprintf((char*) temp_str, "%02d:%02d", time.hour,time.min);

        temp_ptr = (char*)temp_str;
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(content_text_1, &uint16_str_len, (uint8*)temp_ptr, temp_len);
        update_time_str.wstr_ptr = content_text_1;
        update_time_str.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&update_time_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&update_time_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&update_time_str,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );	
		
	//add by James li begin
	text_style.align = ALIGN_HVMIDDLE;
	text_style.font_color = MMI_GREEN_COLOR;
	text_style.font = SONG_FONT_24;

	display_string.wstr_ptr = wstr_buf;
	MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_1, &display_string);

	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&sim_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&sim_rect,       //ó??§òa???Dμ?êμ?ê??óò
		        (const MMI_STRING_T    *)&display_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );
#endif
	//add by James li end
}

PUBLIC void MMIZDT_UpdateWeatherWin()
{
	MMK_SendMsg(MMIZDT_WEATHER_WIN_ID, MSG_FULL_PAINT, NULL);
}

LOCAL MMI_RESULT_E  HandleZDT_WeatherWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T        str_data = {0};
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;

	
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            weather_click_count = 0;

	        //MMIZDT_StartWeatherTimer();
#ifdef ZDT_PLAT_YX_SUPPORT
	        #ifdef WIN32
			ZDT_InitWeather();
			#else
			if(!Is_WeatherGetFromServer())
			{
				YX_API_WT_Send();
			}
			#endif
#endif
            break;
		case MSG_GET_FOCUS:

	        //MMIZDT_StartWeatherTimer();

	        break;
		case MSG_FULL_PAINT:
		{
		        
	       

	        DisplayWeather(win_id);
				
		        

	    }
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
	    case MSG_TP_PRESS_DOWN:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
				wt_tp_down_x = point.x;
				wt_tp_down_y = point.y;
	            
	        }
	        break;
	        
	    case MSG_TP_PRESS_UP:
	        {
	            GUI_POINT_T   point = {0};
	            
				point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
		        if(wt_tp_down_y - point.y  > 90)
		        {
		            MMK_SendMsg(MMK_ConvertIdToHandle(win_id), MSG_APP_CANCEL, PNULL);//exit to clock screen
		            break;
		        }
		        else if(ABS(wt_tp_down_x - point.x) > 40)
		        {
		            break;
		        }
		    }
		        
	        break;
	        
	    case MSG_TP_PRESS_MOVE:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
	        }
	        break;
	    case MSG_TP_PRESS_LONG:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
	        }
	        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            break;
        case MSG_TIMER:
        {
            if (*(uint8*)param == weather_timer_id)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        
        break;
        case MSG_BACKLIGHT_TURN_ON:
        case MSG_BACKLIGHT_TURN_OFF:

        break;
        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
        { 
            //MMIZDT_StopClassModeTimer();

        }
        
        break;

		case MSG_KEYUP_RED:		//BUG 天气界面按红键去菜单了
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

