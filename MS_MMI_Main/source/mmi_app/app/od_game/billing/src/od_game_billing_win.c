#include "window_parse.h"
#include "mn_api.h"
#include "mn_type.h"
#include "mmk_app.h"
#include "guibutton.h"

#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "tb_dal.h"
#include "cafcontrol.h"

#include "mmi_textfun.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmk_timer.h"

#include "os_api.h"
#include "mmiset_export.h"
#include "gui_ucs2b_converter.h"
#include "mmiutil.h"
#include "mmisms_app.h"

#include "od_game_res_id_def.h"
#include "od_game_nv.h"
#include "od_adapter_log.h"
#ifdef OD_GAME_BILLING_SUPPORT
#include "od_game_billing.h"
#include "od_game_billing_config.h"
void od_game_set_unlock_type(OD_GAME_TYPE game_type,OD_U16 unlock_type)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;

    if(nvram_id == 0)
    {
        return;
    }
    od_game_read_nvram_by_id(nvram_id,&data);
    data.unlock_type = unlock_type;
    OD_MMINV_WRITE(nvram_id,&data);    
}


OD_U16 od_game_get_unlock_type(OD_GAME_TYPE game_type)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;
    if(nvram_id > 0)
    {
        od_game_read_nvram_by_id(nvram_id,&data);
        return data.unlock_type;
    }
    return 0;
}



void od_game_set_random_code(OD_GAME_TYPE game_type,OD_U16 random_code)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;
    if(nvram_id == 0)
    {
        return;
    }
    od_game_read_nvram_by_id(nvram_id,&data);
    data.random_type= random_code;
    OD_MMINV_WRITE(nvram_id,&data);    
}

OD_U16 od_game_get_random_code(OD_GAME_TYPE game_type)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;
    if(nvram_id > 0)
    {
        od_game_read_nvram_by_id(nvram_id,&data);
        return data.random_type;
    }
    return 0;
}


void od_game_set_free_trials(OD_GAME_TYPE game_type,OD_U8 trials)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;
    if(nvram_id == 0)
    {
        return;
    }
    od_game_read_nvram_by_id(nvram_id,&data);
    data.free_trials = trials;
    OD_MMINV_WRITE(nvram_id,&data);    
}

OD_U8 od_game_get_free_trials(OD_GAME_TYPE game_type)
{
    OD_U32 nvram_id = od_game_get_nvram_id(game_type);
    od_game_nv_t data;
    if(nvram_id > 0)
    {
        od_game_read_nvram_by_id(nvram_id,&data);
        return data.free_trials;
    }
    return 0;
}


OD_U8* od_game_get_content_name(OD_GAME_TYPE game_type)
{
    switch(game_type)
    {
    #ifdef OD_DOODLE_JUMP_SUPPORT
        case OD_GAME_DOODLE_JUMP:
            return OD_GAME_DOODLE_JUMP_CONTENT_NAME;
    #endif      
    #ifdef OD_CROSSY_ROAD_SUPPORT
        case OD_GAME_CROSSY_ROAD:
            return OD_GAME_CROSSY_ROAD_CONTENT_NAME;
    #endif
    #ifdef OD_STICK_HERO_SUPPORT
        case OD_GAME_STICK_HERO:
            return OD_GAME_STICK_HERO_CONTENT_NAME;
    #endif
    #ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
        case OD_GAME_BUBBLE_PARTY_MIX:
            return OD_GAME_BUBBLE_PARTY_MIX_CONTENT_NAME;
    #endif    	
    #ifdef OD_SUPER_PET_SUPPORT
        case OD_GAME_SUPER_PET:
            return OD_GAME_SUPER_PET_CONTENT_NAME;
    #endif    
        default:
            return "";
     }
}


OD_U8* od_game_get_model_string()
{
    return OD_GAME_MODEL_STRING;
}


OD_U8* od_game_get_os_string()
{
    return OD_GAME_OS_STRING;
}


OD_U16 od_game_get_project_id(OD_GAME_TYPE game_type)
{
    switch(game_type)
    {
    #ifdef OD_DOODLE_JUMP_SUPPORT
        case OD_GAME_DOODLE_JUMP:
            return OD_GAME_DOODLE_JUMP_PROJECT_ID;
    #endif      
    #ifdef OD_CROSSY_ROAD_SUPPORT
        case OD_GAME_CROSSY_ROAD:
            return OD_GAME_CROSSY_ROAD_PROJECT_ID;
    #endif
    #ifdef OD_STICK_HERO_SUPPORT
        case OD_GAME_STICK_HERO:
            return OD_GAME_STICK_HERO_PROJECT_ID;
    #endif
    #ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
        case OD_GAME_BUBBLE_PARTY_MIX:
            return OD_GAME_BUBBLE_PARTY_MIX_PROJECT_ID;
    #endif    
	#ifdef OD_SUPER_PET_SUPPORT
        case OD_GAME_SUPER_PET:
            return OD_GAME_SUPER_PET_PROJECT_ID;
    #endif    
        default:
            return 0;
     }
}

OD_U32 od_game_get_game_title_id(OD_GAME_TYPE game_type)
{
    switch(game_type)
    {
#ifdef OD_DOODLE_JUMP_SUPPORT
        case OD_GAME_DOODLE_JUMP:
            return OD_GAME_DOODLE_JUMP_TEXT;
#endif      
#ifdef OD_CROSSY_ROAD_SUPPORT
        case OD_GAME_CROSSY_ROAD:
            return OD_GAME_CROSSY_ROAD_TEXT;
#endif
#ifdef OD_STICK_HERO_SUPPORT
        case OD_GAME_STICK_HERO:
            return OD_GAME_STICK_HERO_TEXT;
#endif
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
        case OD_GAME_BUBBLE_PARTY_MIX:
            return OD_GAME_BUBBLE_PARTY_MIX_TEXT;
#endif    
#ifdef OD_SUPER_PET_SUPPORT
        case OD_GAME_SUPER_PET:
            return OD_GAME_SUPER_PET_TEXT;
#endif    
        default:
            return 0;
    }
}
#ifdef OD_GAME_DEBUG_SUPPORT
OD_U32 od_game_debug_get_test_plmn();
#endif	
extern OD_U8* od_game_get_billing_price_string(OD_BOOL flag);
extern OD_U16 od_game_get_billing_unlock_type();
extern OD_U8 od_game_get_billing_free_trials();

const MMI_IMAGE_ID_T od_game_num_icon_id[] = 
{
    IMAGE_NUMBER_1,
    IMAGE_NUMBER_2,
    IMAGE_NUMBER_3,
    IMAGE_NUMBER_4,
    IMAGE_NUMBER_5,
    IMAGE_NUMBER_6,
    IMAGE_NUMBER_7,
    IMAGE_NUMBER_8,
    IMAGE_NUMBER_9,
    IMAGE_NUMBER_10,
    IMAGE_NUMBER_11,
    IMAGE_NUMBER_12,
    IMAGE_NUMBER_13,
    IMAGE_NUMBER_14,
    IMAGE_NUMBER_15,
    IMAGE_NUMBER_16,
    IMAGE_NUMBER_17,
    IMAGE_NUMBER_18,
    IMAGE_NUMBER_19,
    IMAGE_NUMBER_20
};


void od_game_menu_append_items(MMI_CTRL_ID_T ctrl_id,OD_U32 index,OD_U16 *itemPtr,OD_U16 len)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = index;//indicate which item  is appended !!

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = od_game_num_icon_id[index];
    //display item string!!
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = itemPtr;
    GUILIST_AppendItem(ctrl_id,&item_t);
}


LOCAL MMI_RESULT_E Handle_od_game_buy_window(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                OD_U8* price = "";
                OD_U8* text = NULL;
                OD_U16 wstr[128] ={0};
                MMI_STRING_T 	string = {0};
                OD_U32 num_items = 0;

                MMI_GetLabelTextByLang(od_game_get_billing_game_title_id(), &string);
                GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	


                MMK_SetAtvCtrl(win_id,OD_GAME_BUY_MENU_CTRL_ID);
            }
            break;
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                od_game_buy_lsk_function(GUILIST_GetCurItemIndex(OD_GAME_BUY_MENU_CTRL_ID));
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_FULL_PAINT:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                OD_U8* price = "";
                OD_U8* text = NULL;
                OD_U16 wstr[128] ={0};
                MMI_STRING_T 	string = {0};
                OD_U32 num_items = 0;


                GUILIST_SetMaxItem(OD_GAME_BUY_MENU_CTRL_ID, 4, FALSE );//max item 33

                if(od_game_get_billing_sim_type() >= OD_GAME_SIM_1 &&
                od_game_get_billing_sim_type() <= OD_GAME_SIM_4)
                {
                    //发过短信
                    if(od_game_get_billing_unlock_type() == 1)
                    {
                        MMI_GetLabelTextByLang(OD_TXT_BTN_ENTER_CODE,&string);
                        od_game_menu_append_items(OD_GAME_BUY_MENU_CTRL_ID,num_items,string.wstr_ptr,string.wstr_len);
                        price = NULL;
                    }
                    else
                    {
                        price = od_game_get_billing_price_string(OD_TRUE);
                    }
                }

                if(price != NULL)
                {
                    MMI_GetLabelTextByLang(OD_TXT_BTN_BUY_PRICE,&string);    
                    text = od_game_get_format_string(string.wstr_ptr,string.wstr_len,0,price); 
                    od_game_menu_append_items(OD_GAME_BUY_MENU_CTRL_ID,num_items,text,od_game_wstrlen(text));
                    od_game_free(text);
                }
                num_items++;
                MMI_GetLabelTextByLang(OD_TXT_BTN_PLAY_NOW_TRIALS_REMAINING,&string);    
                text = od_game_get_format_string(string.wstr_ptr,string.wstr_len,od_game_get_billing_free_trials(),NULL);     
                od_game_menu_append_items(OD_GAME_BUY_MENU_CTRL_ID,num_items,text,od_game_wstrlen(text));
                od_game_free(text);
                num_items++;

                 //还有一个help菜单
                if(od_game_get_billing_sim_type() >= OD_GAME_SIM_1 &&
                od_game_get_billing_sim_type() <= OD_GAME_SIM_4)
                {
                    if(od_game_get_billing_unlock_type() == 1)
                    {
                        MMI_GetLabelTextByLang(OD_TXT_BTN_HELP,&string);
                        od_game_menu_append_items(OD_GAME_BUY_MENU_CTRL_ID,num_items,string.wstr_ptr,string.wstr_len);
                        num_items++;
                    }
                }
            }
            break;
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
        }
    return recode;
}



WINDOW_TABLE(MMI_OD_GAME_BUY_SCREEN_WIN_TAB) = 
{
    WIN_FUNC((uint32)Handle_od_game_buy_window),
    WIN_ID(OD_GAME_BUY_SCREEN_WIN_ID),
    WIN_TITLE(OD_TXT_BACK),
    WIN_SOFTKEY(OD_TXT_SELECT, TXT_NULL, OD_TXT_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, OD_GAME_BUY_MENU_CTRL_ID),
    END_WIN
};

void od_game_buy_enter()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_BUY_SCREEN_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E  Handle_od_game_input_unlock_code_help_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
                    {
                    	   MMI_STRING_T 	string = {0};
                        OD_U16* wstr = od_game_malloc(1024);
                        MMK_SetAtvCtrl(win_id, OD_GAME_HELP_CTRL_ID);
                        MMI_GetLabelTextByLang(OD_TXT_CREDITS_WARNING, &string);
                        MMIAPICOM_Wstrncpy(wstr,string.wstr_ptr,string.wstr_len);
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_HELP, &string);
                        MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_SUPPORT_CONTACT, &string);
                        MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
    		          GUITEXT_SetAlign(OD_GAME_HELP_CTRL_ID, ALIGN_HMIDDLE);
                        GUITEXT_SetString(OD_GAME_HELP_CTRL_ID,wstr,od_game_wstrlen(wstr), FALSE);
                        od_game_free(wstr);
		      }
                    break;
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			od_game_buy_again();
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


WINDOW_TABLE( MMI_OD_GAME_INPUT_UNLOCK_CODE_HELP_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_input_unlock_code_help_window ),    
	WIN_ID(OD_GAME_HELP_SCREEN_WIN_ID ),
	WIN_TITLE(OD_TXT_BTN_HELP),
	WIN_SOFTKEY(OD_TXT_BTN_BUY, TXT_NULL,OD_TXT_BACK),
	CREATE_TEXT_CTRL(OD_GAME_HELP_CTRL_ID),
	END_WIN
};

void od_game_input_unlock_code_help_enter()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_INPUT_UNLOCK_CODE_HELP_WIN_TAB, PNULL);
} 

void od_game_tips_enter(OD_U32 str_id);

void od_game_unlock_done_func_callback(OD_BOOL unlocked)
{
    if(unlocked)
    {
	MMIPUB_OpenAlertSuccessWin(OD_TXT_UNLOCK_SUCCESSFUL);
    }
    else
    {
	 od_game_tips_enter(OD_TXT_ERROR_WRONG_CODE);
    }
}  


LOCAL void od_game_create_set_input_code_view(void)
{
    MMI_STRING_T  temp_str = {0};
    GUI_BORDER_T  edit_border = {0}; 
    GUI_RECT_T edit_rect =  {10,OD_LCD_HEIGHT/5,OD_LCD_WIDTH -10,OD_LCD_HEIGHT/5+35};


    edit_border.type  = GUI_BORDER_ROUNDED;
    edit_border.width = 2;
    edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    
    GUIEDIT_SetRect(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_rect);
    GUIEDIT_SetBorder(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_border);
	
    GUIEDIT_SetFont(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT);
    GUIEDIT_SetAlign(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
}


PUBLIC MMI_RESULT_E Handle_od_game_input_code_window(
														  MMI_WIN_ID_T win_id,
														  MMI_MESSAGE_ID_E msg_id,
														  DPARAM param
														  )
{
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    uint32 i =0;
    GUI_LCD_DEV_INFO lcd_dev_info= {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)	
    MMI_CTRL_ID_T ctrl_id_keybar;
#endif
   
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                od_game_create_set_input_code_view();
                GUIWIN_SetSoftkeyTextId(win_id, OD_TXT_SELECT,COMMON_TXT_NULL,OD_TXT_BACK,FALSE);
                MMK_SetAtvCtrl(win_id, OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID);
            }
            break;

         case MSG_FULL_PAINT:
            {
                // 画背景
                GUI_POINT_T startpoint = {0};
                startpoint.y = MMI_TITLE_HEIGHT;
                GUIRES_DisplayImg(&startpoint,PNULL,PNULL,win_id,IMAGE_COMMON_BG,&lcd_dev_info);                           
            }
            break; 
         
        case MSG_CTL_PENOK:
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)
			ctrl_id_keybar = ((MMI_NOTIFY_T*) param)->src_id;
			if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id_keybar)
			{
	                    MMI_STRING_T edit_box_str = {0};
	                    uint32 edit_num_value = 0;

	                    GUIEDIT_GetString(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_box_str);

	                    MMIAPICOM_WstrToNum(edit_box_str.wstr_ptr,edit_box_str.wstr_len,&edit_num_value);
	                    od_game_unlock_done_func(edit_num_value);              
			}
			else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id_keybar)
			{
				MMK_CloseWin(win_id);
			}
				break;
#endif    

			
        case MSG_CTL_OK: 
        case MSG_APP_OK:
        case MSG_APP_WEB: 
        case MSG_KEYUP_WEB:
            {		
                    MMI_STRING_T edit_box_str = {0};
                    uint32 edit_num_value = 0;

                    GUIEDIT_GetString(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_box_str);

                    MMIAPICOM_WstrToNum(edit_box_str.wstr_ptr,edit_box_str.wstr_len,&edit_num_value);
                    od_game_unlock_done_func(edit_num_value);              
            }    
            break;  
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}


WINDOW_TABLE(MMI_OD_GAME_INPUT_CODE_WIN_TAB) = 
{
    WIN_FUNC((uint32) Handle_od_game_input_code_window ),    
    WIN_ID( OD_GAME_INPUT_SCREEN_WIN_ID),
    WIN_TITLE(OD_TXT_BTN_ENTER_CODE),
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)
	WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
	WIN_SOFTKEY(OD_TXT_SELECT, TXT_NULL, OD_TXT_BACK),
#else
	WIN_SOFTKEY(OD_TXT_SELECT, TXT_NULL, OD_TXT_BACK),
#endif    
    
    CREATE_EDIT_DIGITAL_CTRL(5,OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID),
    END_WIN
};


void od_game_input_unlock_code_enter()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_INPUT_CODE_WIN_TAB, PNULL);
}


void od_game_exit_billing()
{
    MMK_CloseWin(OD_GAME_BUY_SCREEN_WIN_ID);
    MMK_CloseWin(OD_GAME_INPUT_SCREEN_WIN_ID);
    MMK_CloseWin(OD_GAME_TIPS_SCREEN_WIN_ID);
    MMK_CloseWin(OD_GAME_HELP_SCREEN_WIN_ID);
    MMK_CloseWin(OD_GAME_BUY_CONFIRM_SCREEN_WIN_ID);
    MMK_CloseWin(OD_GAME_SEND_WAIT_SCREEN_WIN_ID);
}

OD_U32 g_od_game_tips_str_id = 0;

LOCAL MMI_RESULT_E  Handle_od_game_tips_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_STRING_T 	string = {0};

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
                    MMK_SetAtvCtrl(win_id, OD_GAME_TIPS_TEXT_CTRL_ID);
                    MMI_GetLabelTextByLang(od_game_get_billing_game_title_id(), &string);
                    GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	
                    MMI_GetLabelTextByLang(g_od_game_tips_str_id, &string);
		      GUITEXT_SetAlign(OD_GAME_TIPS_TEXT_CTRL_ID, ALIGN_HMIDDLE);                    
                    GUITEXT_SetString(OD_GAME_TIPS_TEXT_CTRL_ID, string.wstr_ptr, string.wstr_len, FALSE);
                    break;
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			MMK_CloseWin(win_id);
                     break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
                     od_game_exit_billing();   
			break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}


WINDOW_TABLE( MMI_OD_GAME_TIPS_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_tips_window ),    
	WIN_ID(OD_GAME_TIPS_SCREEN_WIN_ID ),
	WIN_TITLE(OD_GAME_HELP_TEXT),
	WIN_SOFTKEY(OD_TXT_BACK, TXT_NULL,OD_TXT_EXIT),
	CREATE_TEXT_CTRL(OD_GAME_TIPS_TEXT_CTRL_ID),
	END_WIN
};


void od_game_tips_enter(OD_U32 str_id)
{
    g_od_game_tips_str_id = str_id;
    MMK_CreateWin((uint32 *) MMI_OD_GAME_TIPS_WIN_TAB, PNULL);
}






void od_game_tips_no_free_trials()
{
    od_game_tips_enter(OD_TXT_ON_SESSIONS_LIMIT_REACHED);
}

#if 1
void od_game_tips_no_buy_info()
{
#if 0
     od_game_tips_enter(OD_TXT_ERROR_MCC);
#else
    if(od_game_get_billing_sim_type() == OD_GAME_SIM_NONE)
    {
        od_game_tips_enter(OD_TXT_ERROR_MCC);
    }
    else if(od_game_get_billing_sim_type() == OD_GAME_SIM_FLASE)
    {
        od_game_tips_enter(OD_TXT_ERROR_NO_PAYMENT_METHOD);
    }
#endif    
}
#endif


extern OD_U8 od_game_get_billing_no_mo();
extern OD_U8* od_game_get_billing_short_code();
OD_BOOL g_od_game_is_sending_sms = OD_FALSE;

LOCAL MMI_RESULT_E  Handle_od_game_buy_confirm_screen_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
                    {
                        OD_U16* wstr = od_game_malloc(1024);
                        OD_U16 price[20] = {0};

                        MMI_STRING_T 	string = {0};
                        MMI_GetLabelTextByLang(od_game_get_billing_game_title_id(), &string);
                        GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	

                        MMK_SetAtvCtrl(win_id, OD_GAME_BUY_CONFIRM_SCREEN_CTRL_ID);
                        MMI_GetLabelTextByLang(OD_TXT_PURCHASE_CONFIRMATION, &string);
                        MMIAPICOM_Wstrncpy(wstr,string.wstr_ptr,string.wstr_len);
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_PRICE, &string);
                        MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        od_game_utf8_to_wstr((OD_U16 *)price,20,od_game_get_billing_price_string(OD_TRUE));
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)price);
                        if(od_game_get_billing_vat_text_is_needed())
                        {
                            MMI_GetLabelTextByLang(OD_TXT_BTN_BUY_THE_GAME_VAT, &string);
                            MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
                            MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        }
                        if(od_game_get_billing_sms_count_text_is_needed())
                        {
                            OD_U8* text;
                            MMI_GetLabelTextByLang(OD_TXT_BTN_BUY_THE_GAME_SMS_COUNT, &string);

                            text = od_game_get_format_string(string.wstr_ptr,string.wstr_len,od_game_get_billing_no_mo(),NULL);     
                            MMIAPICOM_Wstrcat(wstr, (const wchar *)text);
                            od_game_free(text);
                            MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        }
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_TERMS, &string);
                        {
                            OD_U8* text2 = od_game_get_format_string(string.wstr_ptr,string.wstr_len,0,od_game_get_billing_short_code());     
                            MMIAPICOM_Wstrcat(wstr, (const wchar *)text2);
                            od_game_free(text2);
                        }
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_SUPPORT, &string);
                        MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
                        MMIAPICOM_Wstrcat(wstr, (const wchar *)L"\n");
                        MMI_GetLabelTextByLang(OD_TXT_SUPPORT_CONTACT, &string);
                        MMIAPICOM_Wstrncat(wstr,string.wstr_ptr,string.wstr_len);                        
    		          GUITEXT_SetAlign(OD_GAME_BUY_CONFIRM_SCREEN_CTRL_ID, ALIGN_HMIDDLE);
                        GUITEXT_SetString(OD_GAME_BUY_CONFIRM_SCREEN_CTRL_ID,wstr,od_game_wstrlen(wstr), FALSE);
                        od_game_free(wstr);
		      }
                    break;
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			od_game_send_billing_sms();
                     break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
                     MMK_CloseWin(win_id);   
			break;
             case MSG_CLOSE_WINDOW:
                     g_od_game_is_sending_sms = OD_FALSE;
                     break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}


WINDOW_TABLE( MMI_OD_GAME_BUY_CONFIRM_SCREEN_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_buy_confirm_screen_window ),    
	WIN_ID(OD_GAME_BUY_CONFIRM_SCREEN_WIN_ID ),
	WIN_TITLE(OD_TXT_BTN_HELP),
	WIN_SOFTKEY(OD_TXT_BTN_BUY, TXT_NULL,OD_TXT_BACK),
	CREATE_TEXT_CTRL(OD_GAME_BUY_CONFIRM_SCREEN_CTRL_ID),
	END_WIN
};


void od_game_buy_confirm_enter()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_BUY_CONFIRM_SCREEN_WIN_TAB, PNULL);
} 


void od_game_send_sms_time_out()
{
    g_od_game_is_sending_sms = OD_FALSE;
    if(MMK_IsOpenWin(OD_GAME_BUY_CONFIRM_SCREEN_WIN_ID))
    {
        MMK_CloseWin(OD_GAME_SEND_WAIT_SCREEN_WIN_ID);
        MMK_CloseWin(OD_GAME_BUY_CONFIRM_SCREEN_WIN_ID);
        od_game_tips_enter(OD_TXT_HELP_COMMON);
    }
}


OD_U8 g_od_game_sending_sms_timer_id = 0;

LOCAL MMI_RESULT_E  Handle_od_game_wait_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_STRING_T 	string = {0};

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
					MMK_SetAtvCtrl(win_id, OD_GAME_HELP_CTRL_ID);
					MMI_GetLabelTextByLang(od_game_get_billing_game_title_id(), &string);
					GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	
					MMI_GetLabelTextByLang(OD_TXT_PLEASE_WAIT, &string);
					GUITEXT_SetAlign(OD_GAME_HELP_CTRL_ID, ALIGN_HMIDDLE);                    
					GUITEXT_SetString(OD_GAME_HELP_CTRL_ID, string.wstr_ptr, string.wstr_len, FALSE);
                    break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
             od_game_exit_billing(); 
			 MMK_CloseWin(win_id);
			break;
		case MSG_CLOSE_WINDOW:
			if(g_od_game_sending_sms_timer_id > 0)
			{
				od_game_cancel_timer(g_od_game_sending_sms_timer_id);
				g_od_game_sending_sms_timer_id = 0;
			}
			break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}


WINDOW_TABLE( MMI_OD_GAME_WAIT_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_wait_window ),    
	WIN_ID(OD_GAME_SEND_WAIT_SCREEN_WIN_ID ),
	WIN_TITLE(OD_GAME_HELP_TEXT),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL,OD_TXT_EXIT),
	CREATE_TEXT_CTRL(OD_GAME_HELP_CTRL_ID),
	END_WIN
};



void od_game_send_sms_wait_enter()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_WAIT_WIN_TAB, PNULL);
}


void od_game_send_sms_fail()
{
    MMK_CloseWin(OD_GAME_SEND_WAIT_SCREEN_WIN_ID);
    od_game_tips_enter(OD_TXT_ERROR_SENDING_SMS);
}


void od_game_send_sms_callback(OD_BOOL is_succss, void* parm)
{
#ifdef WIN32
    is_succss = TRUE;
#endif

    if(is_succss)
    {  
        od_game_billing_send_sms_callback(OD_TRUE);
        g_od_game_is_sending_sms = OD_TRUE;
		if(g_od_game_sending_sms_timer_id > 0)
		{
			od_game_cancel_timer(g_od_game_sending_sms_timer_id);
			g_od_game_sending_sms_timer_id = 0;
		}
        g_od_game_sending_sms_timer_id = od_game_start_timer(30*1000,od_game_send_sms_time_out);
    }
    else
    {
        od_game_send_sms_fail();
       od_game_billing_send_sms_callback(OD_FALSE);
    }
}  


OD_U8* od_game_get_lang_str()
{
    MMISET_LANGUAGE_TYPE_E current_langid;
    MMIAPISET_GetLanguageType(&current_langid);

    switch(current_langid){
        case MMISET_LANGUAGE_ENGLISH:
              return "EN";
        break;
        case MMISET_LANGUAGE_FRENCH:
              return "FR";
        break;
        case MMISET_LANGUAGE_ITALIAN:
              return "IT";
        break;
        case MMISET_LANGUAGE_GERMAN:
              return "DE";
        break;
        case MMISET_LANGUAGE_SPANISH:
              return "ES";
        break;
        case MMISET_LANGUAGE_PORTUGUESE:
              return "PT";
        break;
        case MMISET_LANGUAGE_RUSSIAN:
              return "RU";
        case MMISET_LANGUAGE_HINDI:
              return "IN";
        break;
        default :
              return "EN";
        break;	    
    }
}

void od_game_auto_unlock_successful()
{
    MMIPUB_OpenAlertSuccessWin(OD_TXT_UNLOCK_SUCCESSFUL);
	if(g_od_game_sending_sms_timer_id > 0)
	{
		od_game_cancel_timer(g_od_game_sending_sms_timer_id);
		g_od_game_sending_sms_timer_id = 0;
	}
    g_od_game_is_sending_sms = OD_FALSE;
}

extern OD_BOOL od_game_auto_unlock_code_by_sms(OD_GAME_SIM_TYPE sim_type,OD_U8* number,OD_U8* content);
#ifdef OD_GAME_DEBUG_SUPPORT
void od_game_billing_reset_all()
{
    OD_U16 i=0;

    for(i=0;i<OD_GAME_TYPE_MAX;i++)
    {
        od_game_set_random_code(i,0);
        od_game_set_unlock_type(i,0);
    #ifdef WIN32
        od_game_set_free_trials(i,255);
    #else
        od_game_set_free_trials(i,5);
    #endif
    }
   
    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
}

void  od_game_debug_get_game_info_string(OD_U8* str,OD_GAME_TYPE game_type)
{    
    if(str)
    {
        sprintf(str,"%s:\nProject_id=%d\nRANDOM_CODE:%d\n",
                    od_game_get_content_name(game_type),
                    od_game_get_project_id(game_type),
                    od_game_get_random_code(game_type)
                );
    }
}


LOCAL MMI_RESULT_E  Handle_od_game_debug_information_screen_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
                    {
                        OD_GAME_BILLING_INFO billing_info = {0};
                        OD_U8* utf8_string, *utf8_game_string;
                        OD_U16* wstr;

                        MMI_STRING_T 	string = {0};
                        string.wstr_ptr = L"Od Game Debug";
                        string.wstr_len = od_game_wstrlen(string.wstr_ptr);
                        GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	

                        MMK_SetAtvCtrl(win_id, OD_GAME_TIPS_TEXT_CTRL_ID);

                        od_game_check_sim_billing_info(&billing_info);

                        utf8_string = (OD_U8*)od_game_malloc(2048);
                        if(billing_info.sim_type == OD_GAME_SIM_NONE || billing_info.sim_type == OD_GAME_SIM_FLASE)
                        {
                            sprintf(utf8_string,"Plmn=%d",billing_info.plmn);
                        }
                        else
                        {
                            sprintf(utf8_string,"Plmn:%d\nSim type:%d\nProfile ID:%d\nShortcode:%s\nKeyword:%s\nPrice:%s\ncurrency_code:%s\nVAT Text:%d\nSMS Cost:%d\nprovider_id:%d\n",
                                billing_info.plmn,
                                billing_info.sim_type,
                                billing_info.profile.profile_id,
                                billing_info.profile.short_code,
                                billing_info.profile.keyword,
                                billing_info.profile.price,
                                billing_info.profile.currency_code,
                                billing_info.profile.vat_text_is_needed,
                                billing_info.profile.sms_count_text_is_needed,
                                billing_info.profile.provider_id
                            );
                        utf8_game_string = (OD_U8*)od_game_malloc(100);
                        #ifdef OD_DOODLE_JUMP_SUPPORT
                            memset(utf8_game_string,0,100);        
                            od_game_debug_get_game_info_string(utf8_game_string,OD_GAME_DOODLE_JUMP);
                            strcat(utf8_string,utf8_game_string);
                        #endif      
                        #ifdef OD_CROSSY_ROAD_SUPPORT
                            memset(utf8_game_string,0,100);        
                            od_game_debug_get_game_info_string(utf8_game_string,OD_GAME_CROSSY_ROAD);
                            strcat(utf8_string,utf8_game_string);
                        #endif
                        #ifdef OD_STICK_HERO_SUPPORT
                            memset(utf8_game_string,0,100);        
                            od_game_debug_get_game_info_string(utf8_game_string,OD_GAME_STICK_HERO);
                            strcat(utf8_string,utf8_game_string);
                        #endif    
                        #ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
                            memset(utf8_game_string,0,100);        
                            od_game_debug_get_game_info_string(utf8_game_string,OD_GAME_BUBBLE_PARTY_MIX);
                            strcat(utf8_string,utf8_game_string);
                        #endif      
                        #ifdef OD_SUPER_PET_SUPPORT
                            memset(utf8_game_string,0,100);        
                            od_game_debug_get_game_info_string(utf8_game_string,OD_GAME_SUPER_PET);
                            strcat(utf8_string,utf8_game_string);
                        #endif      
                        od_game_free(utf8_game_string);
                        }
                        wstr = od_game_malloc(4096);
                        od_game_utf8_to_wstr(wstr,2048, utf8_string);
                        od_game_free(utf8_string);

                        GUITEXT_SetString(OD_GAME_TIPS_TEXT_CTRL_ID,wstr,od_game_wstrlen(wstr), FALSE);
                        od_game_free(wstr);
		      }
                    break;
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			od_game_billing_reset_all();
                     MMK_CloseWin(win_id);   
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


WINDOW_TABLE( MMI_OD_GAME_DEBUG_INFORMATION_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_debug_information_screen_window ),    
	WIN_ID(OD_GAME_TIPS_SCREEN_WIN_ID ),
	WIN_TITLE(OD_GAME_RESET_TEXT),
	WIN_SOFTKEY(OD_GAME_RESET_TEXT, TXT_NULL,OD_TXT_BACK),
	CREATE_TEXT_CTRL(OD_GAME_TIPS_TEXT_CTRL_ID),
	END_WIN
};


void od_game_debug_display_infomation()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_DEBUG_INFORMATION_WIN_TAB, PNULL);
} 


OD_U32 g_od_game_test_plmn_num = 0;
PUBLIC MMI_RESULT_E Handle_od_game_input_test_plmn_window(
														  MMI_WIN_ID_T win_id,
														  MMI_MESSAGE_ID_E msg_id,
														  DPARAM param
														  )
{
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    uint32 i =0;
    GUI_LCD_DEV_INFO lcd_dev_info= {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)	
    MMI_CTRL_ID_T ctrl_id_keybar;
#endif
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                MMI_STRING_T 	string = {0};
                string.wstr_ptr = L"Input Test Plmn";
                string.wstr_len = od_game_wstrlen(string.wstr_ptr);
                GUIWIN_SetTitleText(win_id,string.wstr_ptr,string.wstr_len, FALSE);	

                od_game_create_set_input_code_view();
                GUIWIN_SetSoftkeyTextId(win_id, OD_TXT_SELECT,COMMON_TXT_NULL,OD_TXT_BACK,FALSE);
                MMK_SetAtvCtrl(win_id, OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID);
            }
            break;

         case MSG_FULL_PAINT:
            {
                // 画背景
                GUI_POINT_T startpoint = {0};
                startpoint.y = MMI_TITLE_HEIGHT;
                GUIRES_DisplayImg(&startpoint,PNULL,PNULL,win_id,IMAGE_COMMON_BG,&lcd_dev_info);                           
            }
            break; 
	case MSG_CTL_PENOK:
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)
			ctrl_id_keybar = ((MMI_NOTIFY_T*) param)->src_id;
			if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id_keybar)
			{
	                    MMI_STRING_T edit_box_str = {0};
	                    uint32 edit_num_value = 0;

	                    GUIEDIT_GetString(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_box_str);

	                    if(edit_box_str.wstr_len >=5)
	                    {
	                        MMIAPICOM_WstrToNum(edit_box_str.wstr_ptr,edit_box_str.wstr_len,&edit_num_value);
	                        g_od_game_test_plmn_num = edit_num_value;
	                        od_game_debug_display_infomation();
	                    }
	                    MMK_CloseWin(win_id);
			}
			else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id_keybar)
			{
				MMK_CloseWin(win_id);
			}
				break;
#endif    
               case MSG_CTL_OK: 
        case MSG_APP_OK:
        case MSG_APP_WEB: 
        case MSG_KEYUP_WEB:
            {		
                    MMI_STRING_T edit_box_str = {0};
                    uint32 edit_num_value = 0;

                    GUIEDIT_GetString(OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,&edit_box_str);

                    if(edit_box_str.wstr_len >=5)
                    {
                        MMIAPICOM_WstrToNum(edit_box_str.wstr_ptr,edit_box_str.wstr_len,&edit_num_value);
                        g_od_game_test_plmn_num = edit_num_value;
                        od_game_debug_display_infomation();
                    }
                    MMK_CloseWin(win_id);
            }    
            break;  
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}


WINDOW_TABLE(MMI_OD_GAME_SET_PLMN_WIN_TAB) = 
{
    WIN_FUNC((uint32) Handle_od_game_input_test_plmn_window ),    
    WIN_ID( OD_GAME_INPUT_SCREEN_WIN_ID),
    WIN_TITLE(OD_TXT_BTN_ENTER_CODE),
#if defined( MMI_PDA_SUPPORT)||defined(PDA_UI_SUPPORT)
	WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
	WIN_SOFTKEY(OD_TXT_SELECT, TXT_NULL, OD_TXT_BACK),
#else
	WIN_SOFTKEY(OD_TXT_SELECT, TXT_NULL, OD_TXT_BACK),
#endif    
    CREATE_EDIT_DIGITAL_CTRL(6,OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID),
    END_WIN
};


void od_game_debug_set_plmn()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_SET_PLMN_WIN_TAB, PNULL);
}



OD_U32 od_game_debug_get_test_plmn()
{
    return g_od_game_test_plmn_num;
}



OD_BOOL od_game_debug_ProcessEntrStr(OD_U8* str_ptr, OD_U16 str_len)
{
    if(strncmp((OD_U8*)str_ptr, "*#13790#", str_len) == 0
       && str_len == strlen("*#13790#"))
    {
         od_game_debug_set_plmn();
         return OD_TRUE;
    }

    if(strncmp((OD_U8*)str_ptr, "*#13795#", str_len) == 0
       && str_len == strlen("*#13795#"))
    {
         od_game_debug_display_infomation();   
         return OD_TRUE;
    }
	return OD_FALSE;
}
#else
OD_U32 od_game_debug_get_test_plmn()
{
    return 0;
}

OD_BOOL od_game_debug_ProcessEntrStr(OD_U8* str_ptr, OD_U16 str_len)
{
	return OD_FALSE;	
}
#endif
OD_U8* od_game_debug_get_test_phone_number()
{
	return OD_GAME_TEST_PHONE_NUMBER;
}
#endif

OD_U32 od_game_check_billing_game(OD_U16 code,OD_U8 game_type)
{
  	#ifdef OD_DOODLE_JUMP_SUPPORT
		return od_game_check_doodle_jump(code,game_type);
    #elif defined(OD_CROSSY_ROAD_SUPPORT)
		return od_game_check_crossy_road(code,game_type);
    #elif defined(OD_STICK_HERO_SUPPORT)
		return od_game_check_stick_hero(code,game_type);
    #endif
	return code;
}
