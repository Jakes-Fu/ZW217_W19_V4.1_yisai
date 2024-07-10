#if defined(HERO_ENGINE_TTS_SUPPORT)
#include "hero_id.h"
#include "heroEngine_mmi_text.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "Mn_api.h"
#include "Mmi_common.h"
#include "guitips.h"
#include "guilistbox.h"
#include "mmipub.h"
#include "guicommon.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "guires.h"
#include "guimenu.h"
#include "mmi_common.h"
#include "mmisms_nv.h"
#include "guitext.h"

#include "hero_porting.h"
#include "hero_tts_ui.h"

typedef enum
{
	MENU_SPEAK_OFF = 0,
	MENU_SPEAK_EIGHT = 1,
	MENU_SPEAK_NIGHT = 2,
	MENU_SPEAK_ALL = 3,
	MENU_SPEAK_CUSTOM = 4,

	MENU_SPEAK_NIGHT_START = 18,
	MENU_SPEAK_NIGHT_END = 23,
	MENU_SPEAK_EIGHT_START = 8,
	MENU_SPEAK_EIGHT_END = 18,

	MENU_SPEAK_MODE_TOTAL
} mmi_speak_hour_enum;

static MMI_TEXT_ID_T g_hero_dialog_content_id = 0;
static MMI_TEXT_ID_T g_hero_dialog_title_id = 0;
static struct hero_tts_apps_profile hero_tts_apps;
hero_tts_setting_struct g_herotts_setting;

void hero_menu_item_enter(uint32 menu_id);

void hero_tts_SettingsDefault(hero_tts_setting_struct* pTtsSetting)
{
	// 其他菜单默认打开
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
	pTtsSetting->cur_master_switch_status = 1;
#endif
	pTtsSetting->cur_menu_read_status = 1;
	pTtsSetting->cur_dial_read_status = 1;
	pTtsSetting->cur_akey_time_read_status = 1;
	pTtsSetting->cur_auto_time_read_status = 1;
	pTtsSetting->cur_auto_time_quantum_mode = MENU_SPEAK_EIGHT;
	pTtsSetting->cur_auto_time_start_time = MENU_SPEAK_EIGHT_START;
	pTtsSetting->cur_auto_time_end_time = MENU_SPEAK_EIGHT_END;
	pTtsSetting->cur_incoming_read_status = INCOMING_BROADCAST_5; // 1.播报一次；2.播报五次；3.重复播报；4.关闭播报
	pTtsSetting->cur_phb_read_status = 1;
	pTtsSetting->cur_sms_read_status =1;
	pTtsSetting->cur_new_sms_read_status = 1;
	pTtsSetting->cur_charge_read_status = 1;
	pTtsSetting->cur_low_power_read_status = 1;
	pTtsSetting->cur_lock_screen_read_status = 1;
	pTtsSetting->cur_akey_power_level_read_status = 1;

	pTtsSetting->cur_volume_status = MMISET_VOL_MAX; // 默认音量最大
}

HERO_INT32 hero_tts_RestoreSettings(hero_tts_setting_struct* pTtsSetting)
{
	MMINV_WRITE(MMINV_TTS_HERO_FLAG, pTtsSetting);
	return 1;
}

void hero_tts_apps_save_setting(void)
{
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
	g_herotts_setting.cur_master_switch_status= hero_tts_apps.read_master_switch;
#endif	
	g_herotts_setting.cur_volume_status = hero_tts_apps.volume;
	g_herotts_setting.cur_menu_read_status = hero_tts_apps.read_menu;
	g_herotts_setting.cur_dial_read_status = hero_tts_apps.read_dial;
	g_herotts_setting.cur_akey_time_read_status = hero_tts_apps.read_akey_time;
	g_herotts_setting.cur_auto_time_read_status = hero_tts_apps.read_auto_time;
	g_herotts_setting.cur_auto_time_quantum_mode = hero_tts_apps.read_auto_time_quantum_mode;
	g_herotts_setting.cur_auto_time_start_time = hero_tts_apps.read_auto_time_start;
	g_herotts_setting.cur_auto_time_end_time = hero_tts_apps.read_auto_time_end;
	g_herotts_setting.cur_incoming_read_status = hero_tts_apps.read_incoming;
	g_herotts_setting.cur_phb_read_status = hero_tts_apps.read_phb;
	g_herotts_setting.cur_sms_read_status = hero_tts_apps.read_sms;
	g_herotts_setting.cur_new_sms_read_status = hero_tts_apps.read_new_sms;
	g_herotts_setting.cur_charge_read_status = hero_tts_apps.read_charge;
	g_herotts_setting.cur_low_power_read_status = hero_tts_apps.read_low_power;
	g_herotts_setting.cur_lock_screen_read_status = hero_tts_apps.read_lock_screen;
	g_herotts_setting.cur_akey_power_level_read_status = hero_tts_apps.read_akey_power_level;
	hero_tts_RestoreSettings(&g_herotts_setting);
}

void hero_tts_apps_init(void)
{
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
    hero_tts_apps.read_master_switch = g_herotts_setting.cur_master_switch_status;
#endif
	hero_tts_apps.read_menu = g_herotts_setting.cur_menu_read_status;
	hero_tts_apps.read_dial = g_herotts_setting.cur_dial_read_status;
	hero_tts_apps.read_akey_time = g_herotts_setting.cur_akey_time_read_status;
	hero_tts_apps.read_auto_time = g_herotts_setting.cur_auto_time_read_status;
	hero_tts_apps.read_auto_time_quantum_mode = g_herotts_setting.cur_auto_time_quantum_mode;
	hero_tts_apps.read_auto_time_start = g_herotts_setting.cur_auto_time_start_time;
	hero_tts_apps.read_auto_time_end = g_herotts_setting.cur_auto_time_end_time;
	hero_tts_apps.read_incoming = g_herotts_setting.cur_incoming_read_status;
	hero_tts_apps.read_phb = g_herotts_setting.cur_phb_read_status;
	hero_tts_apps.read_sms = g_herotts_setting.cur_sms_read_status;
	hero_tts_apps.read_new_sms = g_herotts_setting.cur_new_sms_read_status;
	hero_tts_apps.read_charge = g_herotts_setting.cur_charge_read_status;
	hero_tts_apps.read_low_power = g_herotts_setting.cur_low_power_read_status;
	hero_tts_apps.read_lock_screen = g_herotts_setting.cur_lock_screen_read_status;
	hero_tts_apps.read_akey_power_level = g_herotts_setting.cur_akey_power_level_read_status;

	hero_tts_apps.volume = g_herotts_setting.cur_volume_status;
	if(hero_tts_apps.volume > MMISET_VOL_MAX)
	{
		hero_tts_apps.volume = MMISET_VOL_MAX;
		hero_tts_apps_save_setting();
	}
}


void hero_tts_Get_Settings( hero_tts_setting_struct* pTtsSetting)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_TTS_HERO_FLAG,pTtsSetting,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
		// 设置默认开关状态
		hero_tts_SettingsDefault(pTtsSetting);
		// 将默认开关状态写入nv
		hero_tts_RestoreSettings(pTtsSetting);
    }
}

static int is_init = 0;
int hero_tts_init()
{
	if(!is_init)
	{
		hero_tts_Get_Settings(&g_herotts_setting);
		hero_tts_apps_init();
	}
	is_init = 1;
	return 1;
}
#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
#include "heroEngineExport.h"
typedef struct tagHeroTtsMenuItemSt{
	unsigned char isShow; //为0时则不显示该选项
	unsigned char status;
	unsigned char params1;
	unsigned char params2;
	char* pMenuStr;
	char *pSecondMenuStr1;
	char *pSecondMenuStr2;
}heroTtsMenuItemSt;

typedef struct tagHeroTtsMenuStatusSt{
	unsigned int isOperate;
	heroTtsMenuItemSt masterSwitch; //总开关
	heroTtsMenuItemSt numberSwitch; //按键播报
	heroTtsMenuItemSt menuRingSwitch;//菜单播报
	heroTtsMenuItemSt speakHourSwitch;//整的报时模式
	heroTtsMenuItemSt speakTimeSwitch;//一键报时
	heroTtsMenuItemSt incomingRing;//来电播报
	heroTtsMenuItemSt readerPhbSwitch;//电话本
	heroTtsMenuItemSt newSmsSwitch;//来短信提醒
	heroTtsMenuItemSt readerSmsSwitch;//收件箱
	heroTtsMenuItemSt setVolumeSwitch;//音量
	heroTtsMenuItemSt littlePowerSwitch;//充电器插拔
	heroTtsMenuItemSt lowPowerSwitch;//低电量
	heroTtsMenuItemSt broadcastLockSwitch;//锁屏
	heroTtsMenuItemSt akeyPowerSwitch;//一键报电量
}heroTtsMenuStatusSt;

static heroTtsMenuStatusSt gHeroTtsMenuStatus = {0};
unsigned char hero_app_is_operate_tts();

int32 hero_app_get_tts_menu_switch_status(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	uint8 autotime_tag = 0;
	uint8 key_time = 0;

	if(output == NULL || output_len == NULL)
	{
		return HR_FAILED;
	}
	hero_tts_init();
	memset(&gHeroTtsMenuStatus, 0, sizeof(heroTtsMenuStatusSt));

#ifdef HERO_ENGINE_TTS_KEY_7_TIME
	key_time = HR_KEY_7;
#elif defined(HERO_ENGINE_TTS_KEY_8_TIME)
	key_time = HR_KEY_8;
#elif defined(HERO_ENGINE_TTS_KEY_0_TIME)
	key_time = HR_KEY_0;
#elif defined(HERO_ENGINE_TTS_KEY_STAR_TIME)
	key_time = HR_KEY_STAR;
#endif

	if(g_herotts_setting.cur_auto_time_read_status == 0)
	{
		autotime_tag = 0;
	}
	else
	{
		autotime_tag = g_herotts_setting.cur_auto_time_quantum_mode;
	}

	if (STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition())
	{
		gHeroTtsMenuStatus.isOperate = hero_app_is_operate_tts();
	}
	else
	{
		gHeroTtsMenuStatus.isOperate = 0;
	}
	
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
	gHeroTtsMenuStatus.masterSwitch.isShow = 1;
	gHeroTtsMenuStatus.masterSwitch.status = g_herotts_setting.cur_master_switch_status;
#else
	gHeroTtsMenuStatus.masterSwitch.isShow = 0;	
	gHeroTtsMenuStatus.masterSwitch.status = 0;
#endif

	gHeroTtsMenuStatus.numberSwitch.isShow = 1;
	gHeroTtsMenuStatus.numberSwitch.status = g_herotts_setting.cur_dial_read_status;

	gHeroTtsMenuStatus.menuRingSwitch.isShow = 1;
	gHeroTtsMenuStatus.menuRingSwitch.status = g_herotts_setting.cur_menu_read_status;

	gHeroTtsMenuStatus.speakHourSwitch.isShow = 1;
	gHeroTtsMenuStatus.speakHourSwitch.status = autotime_tag;
	gHeroTtsMenuStatus.speakHourSwitch.params1 = g_herotts_setting.cur_auto_time_start_time;
	gHeroTtsMenuStatus.speakHourSwitch.params2 = g_herotts_setting.cur_auto_time_end_time;

	gHeroTtsMenuStatus.speakTimeSwitch.isShow = 1;
	gHeroTtsMenuStatus.speakTimeSwitch.status = g_herotts_setting.cur_akey_time_read_status;
	gHeroTtsMenuStatus.speakTimeSwitch.params1 = key_time;

	gHeroTtsMenuStatus.incomingRing.isShow = 1;
	gHeroTtsMenuStatus.incomingRing.status = (g_herotts_setting.cur_incoming_read_status <= 0) ? 0 : (g_herotts_setting.cur_incoming_read_status - 1);
	gHeroTtsMenuStatus.incomingRing.params1 = 1;

	gHeroTtsMenuStatus.readerPhbSwitch.isShow = 1;
	gHeroTtsMenuStatus.readerPhbSwitch.status = g_herotts_setting.cur_phb_read_status;

	gHeroTtsMenuStatus.newSmsSwitch.isShow = 1;
	gHeroTtsMenuStatus.newSmsSwitch.status = g_herotts_setting.cur_new_sms_read_status;

	gHeroTtsMenuStatus.readerSmsSwitch.isShow = 1;
	gHeroTtsMenuStatus.readerSmsSwitch.status = g_herotts_setting.cur_sms_read_status;

	gHeroTtsMenuStatus.littlePowerSwitch.isShow = 1;
	gHeroTtsMenuStatus.littlePowerSwitch.status = g_herotts_setting.cur_charge_read_status;

	gHeroTtsMenuStatus.lowPowerSwitch.isShow = 1;
	gHeroTtsMenuStatus.lowPowerSwitch.status = g_herotts_setting.cur_low_power_read_status;

	gHeroTtsMenuStatus.broadcastLockSwitch.isShow = 1;
	gHeroTtsMenuStatus.broadcastLockSwitch.status = g_herotts_setting.cur_lock_screen_read_status;

	gHeroTtsMenuStatus.setVolumeSwitch.isShow = 1;
	gHeroTtsMenuStatus.setVolumeSwitch.status = (g_herotts_setting.cur_volume_status <= 0) ? 0 : (g_herotts_setting.cur_volume_status - 1);
	gHeroTtsMenuStatus.setVolumeSwitch.params1 = MMISET_VOL_MAX;

	gHeroTtsMenuStatus.akeyPowerSwitch.isShow = 1;
	gHeroTtsMenuStatus.akeyPowerSwitch.status = g_herotts_setting.cur_akey_power_level_read_status;
	gHeroTtsMenuStatus.akeyPowerSwitch.params1 = HR_KEY_STAR;

#if defined(HERO_ENGINE_TTS_VOICE_SPEED)
	gHeroTtsMenuStatus.voiceSpeed.isShow = 1;
	gHeroTtsMenuStatus.voiceSpeed.status = g_herotts_setting.cur_voice_speed_status;
#endif

	*output = (uint8*)&gHeroTtsMenuStatus;
	*output_len = sizeof(heroTtsMenuStatusSt);
	return HR_SUCCESS;
}

int32 hero_app_set_tts_menu_switch_status(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	heroTtsMenuStatusSt *pTtsStatus = (heroTtsMenuStatusSt*)input;
	uint8 autotime_tag = 0;
	uint8 autotime_mode = 0;
	if(input == NULL || input_len != sizeof(heroTtsMenuStatusSt))
	{
		return HR_FAILED;
	}

	if(pTtsStatus->masterSwitch.params1 == 1)
	{
		hero_tts_SettingsDefault(&g_herotts_setting);
	}
	else
	{
		if(pTtsStatus->speakHourSwitch.status <= 0)
		{
			autotime_tag = 0;
			autotime_mode = 0;
		}
		else
		{
			autotime_tag = 1;
			autotime_mode = pTtsStatus->speakHourSwitch.status;
		}
#if defined(HERO_ENGINE_TTS_ALL_SUPPORT)
		g_herotts_setting.cur_master_switch_status = pTtsStatus->masterSwitch.status;
#endif
		g_herotts_setting.cur_volume_status = pTtsStatus->setVolumeSwitch.status + 1;
		g_herotts_setting.cur_menu_read_status = pTtsStatus->menuRingSwitch.status;
		g_herotts_setting.cur_dial_read_status = pTtsStatus->numberSwitch.status;
		g_herotts_setting.cur_akey_time_read_status = pTtsStatus->speakTimeSwitch.status;
		g_herotts_setting.cur_auto_time_read_status = autotime_tag;
		g_herotts_setting.cur_auto_time_quantum_mode = autotime_mode;
		if(autotime_mode == MENU_SPEAK_EIGHT)
		{
			g_herotts_setting.cur_auto_time_start_time = MENU_SPEAK_EIGHT_START;
			g_herotts_setting.cur_auto_time_end_time = MENU_SPEAK_EIGHT_END;
		}
		else if(autotime_mode == MENU_SPEAK_NIGHT)
		{
			g_herotts_setting.cur_auto_time_start_time = MENU_SPEAK_NIGHT_START;
			g_herotts_setting.cur_auto_time_end_time = MENU_SPEAK_NIGHT_END;
		}
		else
		{
			g_herotts_setting.cur_auto_time_start_time = pTtsStatus->speakHourSwitch.params1;
			g_herotts_setting.cur_auto_time_end_time = pTtsStatus->speakHourSwitch.params2;
		}
		g_herotts_setting.cur_incoming_read_status = pTtsStatus->incomingRing.status + 1;
		g_herotts_setting.cur_phb_read_status = pTtsStatus->readerPhbSwitch.status;
		g_herotts_setting.cur_sms_read_status = pTtsStatus->readerSmsSwitch.status;
		g_herotts_setting.cur_new_sms_read_status = pTtsStatus->newSmsSwitch.status;
		g_herotts_setting.cur_charge_read_status = pTtsStatus->littlePowerSwitch.status;
		g_herotts_setting.cur_low_power_read_status = pTtsStatus->lowPowerSwitch.status;
		g_herotts_setting.cur_lock_screen_read_status = pTtsStatus->broadcastLockSwitch.status;
		g_herotts_setting.cur_akey_power_level_read_status = pTtsStatus->akeyPowerSwitch.status;

	#if defined(HERO_ENGINE_TTS_VOICE_SPEED)
		g_herotts_setting.cur_voice_speed_status = pTtsStatus->voiceSpeed.status; //语速
	#endif
	}
	hero_tts_RestoreSettings(&g_herotts_setting);
	return HR_SUCCESS;
}

//0:normal 1:slow 2:fast
#if 1//defined(HERO_ENGINE_TTS_VOICE_SPEED)
int hero_tts_get_speed(void)
{
	hero_tts_init();
	if(g_herotts_setting.cur_voice_speed_status == 0){
		return TIHO_TTS_SPEED_NORMAL;
	} else if(g_herotts_setting.cur_voice_speed_status == 1) {
		return TIHO_TTS_SPEED_MIN;
	} else if(g_herotts_setting.cur_voice_speed_status == 2) {
		return TIHO_TTS_SPEED_MAX;
	}
	return TIHO_TTS_SPEED_NORMAL;
}
#endif

void hero_app_operate_tts_setting_data(int isReset)
{ // 1 close // 0 open
	if(isReset == 1)
	{
		memset(&g_herotts_setting, 0, sizeof(hero_tts_setting_struct));
	}
	else
	{
		hero_tts_SettingsDefault(&g_herotts_setting);
	}
	hero_tts_RestoreSettings(&g_herotts_setting);
}

void hero_tts_set_enter(void)
{
    MMIHEROAPP_AppEntry(HERO_APP_TTS);
}

char get_hero_phb_status(void)
{
	return gHeroTtsMenuStatus.readerPhbSwitch.status;
}

#else
////////////////////////////////// DIALOG //////////////////////////////////
LOCAL MMI_RESULT_E HandleHeroDialogWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_STRING_T    title = {0};

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, HERO_CTRL_ID_DIALOG_TEXTBOX);
            MMIRES_GetText(g_hero_dialog_content_id, NULL, &kstring);
            GUITEXT_SetString(HERO_CTRL_ID_DIALOG_TEXTBOX, kstring.wstr_ptr, kstring.wstr_len, FALSE);
            MMIRES_GetText(g_hero_dialog_title_id, NULL, &title);
            GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
#ifdef MMI_PDA_SUPPORT
        case MSG_CTL_PENOK:
            if (PNULL != param)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                        MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                        break;
                    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                        MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                        break;
                    default:
                        break;
                }
            }
            break;
#endif
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(HERO_DIALOG_WIN_TAB) = {
    //WIN_STATUSBAR,
    WIN_ID(HERO_DIALOG_WIN_ID), 
    WIN_FUNC((uint32)HandleHeroDialogWinMsg),
    WIN_TITLE(TXT_HERO_TTS_SET_AKEY_TIME_MENU),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    CREATE_TEXT_CTRL(HERO_CTRL_ID_DIALOG_TEXTBOX),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN};
////////////////////////////////// DIALOG END //////////////////////////////////

////////////////////////////////// AKEYTIME //////////////////////////////////
static void hero_akey_time_show_spec(void)
{
#ifdef HERO_ENGINE_TTS_KEY_7_TIME
	g_hero_dialog_content_id = TXT_HERO_TTS_SET_7_AKEY_TIME_HELP;
#elif defined(HERO_ENGINE_TTS_KEY_8_TIME)
	g_hero_dialog_content_id = TXT_HERO_TTS_SET_8_AKEY_TIME_HELP;
#elif defined(HERO_ENGINE_TTS_KEY_0_TIME)
	g_hero_dialog_content_id = TXT_HERO_TTS_SET_0_AKEY_TIME_HELP;
#elif defined(HERO_ENGINE_TTS_KEY_STAR_TIME)
	g_hero_dialog_content_id = TXT_HERO_TTS_SET_STAR_AKEY_TIME_HELP;
#endif
	g_hero_dialog_title_id = TXT_HERO_TTS_SET_AKEY_TIME_MENU;
    MMK_CreateWin((uint32*)HERO_DIALOG_WIN_TAB, PNULL);
}

static void hero_CreateAKeyTimeListMenu( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint16           i = 0;
    MMI_STRING_T    kstring = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_TEXT_ID_T itemId = NULL;

    both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);
	MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_TIME, NULL, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    node_item.item_text_ptr = &kstring;
    if(hero_tts_apps.read_akey_time)
    {
        itemId = TXT_OPEN;
    }
	else
	{
        itemId = TXT_CLOSE;
    }
    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(itemId, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_TIME_MENU, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;
}

LOCAL MMI_RESULT_E HandleHeroAKeyTimeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            hero_CreateAKeyTimeListMenu(win_id, HERO_CTRL_ID_AKEY_TIME);
            break;
        case MSG_GET_FOCUS:
	        {
	            GUIMENU_DYNA_ITEM_T     node_item = {0};
	            MMI_TEXT_ID_T itemId = NULL;

	            node_item.item_text_ptr = &kstring;
	            if(hero_tts_apps.read_akey_time)
	            {
	                itemId = TXT_OPEN;
	            }
				else
				{
	                itemId = TXT_CLOSE;
	            }
	            MMIRES_GetText(itemId, NULL, &kstring);
	            GUIMENU_ReplaceNode(0, ID_HERO_MENU1, 0, &kstring, HERO_CTRL_ID_AKEY_TIME);
	        }
            break;
        case MSG_CLOSE_WINDOW:
            hero_tts_apps_save_setting();
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            menu_id = GUIMENU_GetCurNodeId(HERO_CTRL_ID_AKEY_TIME);
            switch ( menu_id )
            {
                case ID_HERO_MENU1:
                    hero_menu_item_enter(ID_HERO_MENU_SET_AKEY_TIME);
                    break;
                case ID_HERO_MENU2:
                    hero_akey_time_show_spec();
                    break;
                case ID_HERO_MENU3:
                    break;
                default:
                    break;
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_TIPS_NEED_DATA:
            break;
    }
    return recode;
}

WINDOW_TABLE(HERO_TTS_AKEY_TIME_WIN_TAB) = {
    //WIN_STATUSBAR,
    WIN_ID(HERO_AKEY_TIME_WIN_ID), 
    WIN_FUNC((uint32)HandleHeroAKeyTimeWinMsg),
    WIN_TITLE(TXT_HERO_TTS_SET_AKEY_TIME),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN};

void hero_tts_akey_time_setting_enter(void)
{
	MMK_CreateWin((uint32*)HERO_TTS_AKEY_TIME_WIN_TAB, PNULL);
}
////////////////////////////////// AKEYTIME END //////////////////////////////////

////////////////////////////////// AKEY POWER LEVEL //////////////////////////////////
static void hero_akey_power_level_show_spec(void)
{
	g_hero_dialog_content_id = TXT_HERO_TTS_SET_9_AKEY_POWER_LEVEL_HELP;
	g_hero_dialog_title_id = TXT_HERO_TTS_SET_AKEY_POWER_LEVEL_MENU;
    MMK_CreateWin((uint32*)HERO_DIALOG_WIN_TAB, PNULL);
}

static void hero_CreateAKeyPowerLevelListMenu( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint16           i = 0;
    MMI_STRING_T    kstring = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_TEXT_ID_T itemId = NULL;

    both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);
	MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_POWER_LEVEL, NULL, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    node_item.item_text_ptr = &kstring;
    if(hero_tts_apps.read_akey_power_level)
    {
        itemId = TXT_OPEN;
    }
	else
	{
        itemId = TXT_CLOSE;
    }
    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(itemId, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_POWER_LEVEL_MENU, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;
}

LOCAL MMI_RESULT_E HandleHeroAKeyPowerLevelWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            hero_CreateAKeyPowerLevelListMenu(win_id, HERO_CTRL_ID_AKEY_POWER_LEVEL);
            break;
        case MSG_GET_FOCUS:
	        {
	            GUIMENU_DYNA_ITEM_T     node_item = {0};
	            MMI_TEXT_ID_T itemId = NULL;

	            node_item.item_text_ptr = &kstring;
	            if(hero_tts_apps.read_akey_power_level)
	            {
	                itemId = TXT_OPEN;
	            }
				else
				{
	                itemId = TXT_CLOSE;
	            }
	            MMIRES_GetText(itemId, NULL, &kstring);
	            GUIMENU_ReplaceNode(0, ID_HERO_MENU1, 0, &kstring, HERO_CTRL_ID_AKEY_POWER_LEVEL);
	        }
            break;
        case MSG_CLOSE_WINDOW:
            hero_tts_apps_save_setting();
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            menu_id = GUIMENU_GetCurNodeId(HERO_CTRL_ID_AKEY_POWER_LEVEL);
            switch ( menu_id )
            {
                case ID_HERO_MENU1:
                    hero_menu_item_enter(ID_HERO_MENU_SET_AKEY_POWER_LEVEL);
                    break;
                case ID_HERO_MENU2:
                    hero_akey_power_level_show_spec();
                    break;
                case ID_HERO_MENU3:
                    break;
                default:
                    break;
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_TIPS_NEED_DATA:
            break;
    }
    return recode;
}


WINDOW_TABLE(HERO_TTS_AKEY_POWER_LEVEL_WIN_TAB) = {
    //WIN_STATUSBAR,
    WIN_ID(HERO_AKEY_POWER_LEVEL_WIN_ID), 
    WIN_FUNC((uint32)HandleHeroAKeyPowerLevelWinMsg),
    WIN_TITLE(TXT_HERO_TTS_SET_AKEY_POWER_LEVEL),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN};

void hero_tts_akey_power_level_setting_enter(void)
{
	MMK_CreateWin((uint32*)HERO_TTS_AKEY_POWER_LEVEL_WIN_TAB, PNULL);
}
////////////////////////////////// AKEY POWER LEVEL END //////////////////////////////////

////////////////////////////////// AUTOTIME //////////////////////////////////
//////////////////////////////// 自定义时间 ////////////////////////////////
static void hero_tts_auto_time_SetTimeParamEditWin(void)
{
    GUIEDIT_TIME_STYLE_E time_style = GUIEDIT_TIME_STYLE_24;
    MMI_STRING_T        title_text  = {0};

    GUIFORM_SetStyle(HERO_CTRL_ID_AUTO_EDIT_START_TIME_FORM,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_HERO_TTS_SET_AUTO_TIME_START_TIME, &title_text);
    GUILABEL_SetText(HERO_CTRL_ID_AUTO_START_LABEL, &title_text, FALSE);
    GUIEDIT_SetTimeStyle(HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX,PNULL,&time_style,PNULL,FALSE);
    {
		int start_hour = 0;
		start_hour = hero_tts_apps.read_auto_time_start;
        GUIEDIT_SetTime(HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX,
                            start_hour, 
                            0,
                            0);
    }
	GUIFORM_SetStyle(HERO_CTRL_ID_AUTO_EDIT_END_TIME_FORM,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_HERO_TTS_SET_AUTO_TIME_END_TIME, &title_text);
    GUILABEL_SetText(HERO_CTRL_ID_AUTO_END_LABEL, &title_text, FALSE);
    GUIEDIT_SetTimeStyle(HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX,PNULL,&time_style,PNULL,FALSE);
    {
		int end_hour = 0;
		end_hour = hero_tts_apps.read_auto_time_end;
        GUIEDIT_SetTime(HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX,
                            end_hour, 
                            0,
                            0);
    }
}

LOCAL MMI_RESULT_E  HandleHeroTTSAutoTimeCustomWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
	MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		{
		#ifndef MMI_PDA_SUPPORT
			GUIFORM_SetCircularHandleUpDown(HERO_CTRL_ID_AUTO_TIME_FORM,TRUE);
		#endif
	        GUIFORM_SetType(HERO_CTRL_ID_AUTO_TIME_FORM,GUIFORM_TYPE_TP);
	        hero_tts_auto_time_SetTimeParamEditWin();
		 	GUIEDIT_SetAlign(HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX,ALIGN_LVMIDDLE);
	        GUIEDIT_SetAlign(HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX,ALIGN_LVMIDDLE);
	        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
	        MMK_SetAtvCtrl(win_id, HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX);
	    }
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK: 
		{
			int start_hour = 0;
			int start_min = 0;
			int end_hour = 0;
			int end_min = 0;

			GUIEDIT_GetTime(HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX, &start_hour, &start_min, PNULL);
			GUIEDIT_GetTime(HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX, &end_hour, &end_min, PNULL);

			if(end_hour > start_hour)
			{
				hero_tts_apps.read_auto_time_quantum_mode = 4;
	            hero_tts_apps.read_auto_time_start = (HERO_UINT8)start_hour;
	            hero_tts_apps.read_auto_time_end = (HERO_UINT8)end_hour;
	            hero_tts_apps_save_setting();
				MMK_CloseWin(win_id);
			}
			else
			{
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
	    }
        break;
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
    break;
        case MSG_CLOSE_WINDOW:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

WINDOW_TABLE( HERO_TTS_AUTO_TIME_CUSTOM_MODE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleHeroTTSAutoTimeCustomWinMsg ),    
    WIN_ID( HERO_AUTO_TIME_CUSTOM_MODE_WIN_ID ),
    WIN_TITLE( TXT_HERO_TTS_SET_AUTO_TIME_MODE_CUSTOM ),  
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,HERO_CTRL_ID_AUTO_TIME_FORM),

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,HERO_CTRL_ID_AUTO_EDIT_START_TIME_FORM,HERO_CTRL_ID_AUTO_TIME_FORM),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,HERO_CTRL_ID_AUTO_START_LABEL,HERO_CTRL_ID_AUTO_EDIT_START_TIME_FORM),
	CHILD_EDIT_TIME_CTRL(TRUE,HERO_CTRL_ID_AUTO_EDIT_START_TIME_EDITBOX,HERO_CTRL_ID_AUTO_EDIT_START_TIME_FORM),

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,HERO_CTRL_ID_AUTO_EDIT_END_TIME_FORM,HERO_CTRL_ID_AUTO_TIME_FORM),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,HERO_CTRL_ID_AUTO_END_LABEL,HERO_CTRL_ID_AUTO_EDIT_END_TIME_FORM),
	CHILD_EDIT_TIME_CTRL(TRUE,HERO_CTRL_ID_AUTO_EDIT_END_TIME_EDITBOX,HERO_CTRL_ID_AUTO_EDIT_END_TIME_FORM),
    END_WIN
};

void hero_tts_auto_time_custom_enter(void)
{
	MMK_CreateWin((uint32*)HERO_TTS_AUTO_TIME_CUSTOM_MODE_WIN_TAB, PNULL);
}

//////////////////////////////// 时间段控制 ////////////////////////////////
LOCAL MMI_RESULT_E HandleHeroTTSAutoTimeQuantumWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                MMI_STRING_T  title = {0};
                int i = 0;
                int str_list[4] = {TXT_HERO_TTS_SET_AUTO_TIME_MODE_DAY, TXT_HERO_TTS_SET_AUTO_TIME_MODE_NIGHT,
				                   TXT_HERO_TTS_SET_AUTO_TIME_MODE_WHOLE_DAY, TXT_HERO_TTS_SET_AUTO_TIME_MODE_CUSTOM};
                int j, n;

                n = 4;
                GUILIST_SetMaxItem(HERO_CTRL_ID_SELECT_ON_OFF, n, FALSE );//max item 33
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
#ifdef MMI_PDA_SUPPORT
                item_data.softkey_id[0] = TXT_NULL;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#else
                item_data.softkey_id[0] = STXT_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#endif
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID; //GUIITEM_DATA_TEXT_BUFFER;
                for ( j = 0; j < n; ++ j )
                {
                    item_data.item_content[0].item_data.text_id = str_list[j];
                    GUILIST_AppendItem(HERO_CTRL_ID_SELECT_ON_OFF, &item_t);
                }
                i = hero_tts_apps.read_auto_time_quantum_mode - 1;
                GUILIST_SetSelectedItem(HERO_CTRL_ID_SELECT_ON_OFF, i, TRUE);
                GUILIST_SetCurItemIndex(HERO_CTRL_ID_SELECT_ON_OFF, i);
                MMK_SetAtvCtrl(win_id,HERO_CTRL_ID_SELECT_ON_OFF);
            }
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            {
                HERO_UINT8 flag;
                uint16                  cur_selection   =   0;
                GUILIST_GetSelectedItemIndex(HERO_CTRL_ID_SELECT_ON_OFF,&cur_selection,1);
                if(cur_selection == 3)
				{
					hero_tts_auto_time_custom_enter();
				}
				else
				{
					flag = cur_selection + 1;
	                if(hero_tts_apps.read_auto_time_quantum_mode != flag)
	                {
	                    hero_tts_apps.read_auto_time_quantum_mode = flag;
	                    hero_tts_apps_save_setting();
	                }
                	MMK_CloseWin(win_id);
				}
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(HERO_TTS_AUTO_TIME_QUANTUM_WIN_TAB) = 
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleHeroTTSAutoTimeQuantumWinMsg),   
    WIN_TITLE(TXT_HERO_TTS_SET_AUTO_TIME_QUANTUM_MODE),
    WIN_ID(HERO_AUTO_TIME_TIME_QUANTUM_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, HERO_CTRL_ID_SELECT_ON_OFF),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

void hero_tts_auto_time_quantum_enter(void)
{
    MMK_CreateWin((uint32*)HERO_TTS_AUTO_TIME_QUANTUM_WIN_TAB, PNULL);
}

//////////////////////////// 整点报时设置 ////////////////////////////
static void hero_CreateAutoTimeListMenu( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint16           i = 0;
    MMI_STRING_T    kstring = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_TEXT_ID_T itemId = NULL;

    both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);
	MMIRES_GetText(TXT_HERO_TTS_SET_AUTO_TIME, NULL, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    node_item.item_text_ptr = &kstring;
    if(hero_tts_apps.read_auto_time)
    {
        itemId = TXT_OPEN;
    }
	else
	{
        itemId = TXT_CLOSE;
    }
    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(itemId, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AUTO_TIME_QUANTUM_MODE, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;
}

LOCAL MMI_RESULT_E HandleHeroAutoTimeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            hero_CreateAutoTimeListMenu(win_id, HERO_CTRL_ID_AUTO_TIME);
            break;
        case MSG_GET_FOCUS:
	        {
	            GUIMENU_DYNA_ITEM_T     node_item = {0};
	            MMI_TEXT_ID_T itemId = NULL;

	            node_item.item_text_ptr = &kstring;
	            if(hero_tts_apps.read_auto_time)
	            {
	                itemId = TXT_OPEN;
	            }
				else
				{
	                itemId = TXT_CLOSE;
	            }
	            MMIRES_GetText(itemId, NULL, &kstring);
	            GUIMENU_ReplaceNode(0, ID_HERO_MENU1, 0, &kstring, HERO_CTRL_ID_AUTO_TIME);
	        }
            break;
        case MSG_CLOSE_WINDOW:
            hero_tts_apps_save_setting();
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            menu_id = GUIMENU_GetCurNodeId(HERO_CTRL_ID_AUTO_TIME);
            switch ( menu_id )
            {
                case ID_HERO_MENU1:
                    hero_menu_item_enter(ID_HERO_MENU_SET_AUTO_TIME);
                    break;
                case ID_HERO_MENU2:
                    //时间段控制
                    hero_tts_auto_time_quantum_enter();
                    break;
                case ID_HERO_MENU3:
                    break;
                default:
                    break;
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_TIPS_NEED_DATA:
            break;
    }
    return recode;
}


WINDOW_TABLE(HERO_TTS_AUTO_TIME_WIN_TAB) = {
    //WIN_STATUSBAR,
    WIN_ID(HERO_AUTO_TIME_WIN_ID), 
    WIN_FUNC((uint32)HandleHeroAutoTimeWinMsg),
    WIN_TITLE(TXT_HERO_TTS_SET_AUTO_TIME),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN};

void hero_tts_auto_time_setting_enter(void)
{
	MMK_CreateWin((uint32*)HERO_TTS_AUTO_TIME_WIN_TAB, PNULL);
}
////////////////////////////////// AUTOTIME END //////////////////////////////////

////////////////////////////////// INCOMING //////////////////////////////////
LOCAL MMI_RESULT_E HandleHeroTTSIncomingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                MMI_STRING_T  title = {0};
                int i = 0;
                int str_list[4] = {TXT_HERO_TTS_SET_INCOMING_1, TXT_HERO_TTS_SET_INCOMING_5, 
				                   TXT_HERO_TTS_SET_INCOMING_999, TXT_HERO_TTS_SET_INCOMING_0};
                int j, n;

                n = 4;
                GUILIST_SetMaxItem(HERO_CTRL_ID_SELECT_ON_OFF, n, FALSE );//max item 33
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
#ifdef MMI_PDA_SUPPORT
                item_data.softkey_id[0] = TXT_NULL;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#else
                item_data.softkey_id[0] = STXT_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#endif
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID; //GUIITEM_DATA_TEXT_BUFFER;

                for ( j = 0; j < n; ++ j )
                {
                    item_data.item_content[0].item_data.text_id = str_list[j];
                    GUILIST_AppendItem(HERO_CTRL_ID_SELECT_ON_OFF, &item_t);
                }
                i = hero_tts_apps.read_incoming - 1;
                GUILIST_SetSelectedItem(HERO_CTRL_ID_SELECT_ON_OFF, i, TRUE);
                GUILIST_SetCurItemIndex(HERO_CTRL_ID_SELECT_ON_OFF, i);
                MMK_SetAtvCtrl(win_id,HERO_CTRL_ID_SELECT_ON_OFF);
            }
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            {
                HERO_UINT8 flag;
                uint16                  cur_selection   =   0;
                GUILIST_GetSelectedItemIndex(HERO_CTRL_ID_SELECT_ON_OFF,&cur_selection,1);
                flag = cur_selection + 1;
                if(hero_tts_apps.read_incoming != flag)
				{
                    hero_tts_apps.read_incoming = flag;
                    hero_tts_apps_save_setting();
                }
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(HERO_TTS_INCOMING_SETTING_WIN_TAB ) = 
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleHeroTTSIncomingWinMsg),   
    WIN_TITLE(TXT_HERO_TTS_SET_INCOMING),
    WIN_ID(HERO_INCOMING_CALL_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, HERO_CTRL_ID_SELECT_ON_OFF),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

void hero_tts_incoming_setting_enter(void)
{
    MMK_CreateWin((uint32*)HERO_TTS_INCOMING_SETTING_WIN_TAB, PNULL);
}
////////////////////////////////// INCOMING END //////////////////////////////////

////////////////////////////////// VOLUME //////////////////////////////////
LOCAL MMI_RESULT_E HandleHeroTTSVolumeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                MMI_STRING_T  title = {0};
                int i = 0;
                int str_list[9] = {TXT_HERO_TTS_VOLUME_1, TXT_HERO_TTS_VOLUME_2, TXT_HERO_TTS_VOLUME_3, 
				                   TXT_HERO_TTS_VOLUME_4, TXT_HERO_TTS_VOLUME_5, TXT_HERO_TTS_VOLUME_6,
				                   TXT_HERO_TTS_VOLUME_7, TXT_HERO_TTS_VOLUME_8, TXT_HERO_TTS_VOLUME_9};
                int j, n;

                n = 9;
                GUILIST_SetMaxItem(HERO_CTRL_ID_SELECT_ON_OFF, n, FALSE );//max item 33
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
#ifdef MMI_PDA_SUPPORT
                item_data.softkey_id[0] = TXT_NULL;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#else
                item_data.softkey_id[0] = STXT_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#endif
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID; //GUIITEM_DATA_TEXT_BUFFER;

                for ( j = 0; j < n; ++ j )
                {
                    item_data.item_content[0].item_data.text_id = str_list[j];
                    GUILIST_AppendItem(HERO_CTRL_ID_SELECT_ON_OFF, &item_t);
                }
                i = hero_tts_apps.volume - 1;
                GUILIST_SetSelectedItem(HERO_CTRL_ID_SELECT_ON_OFF, i, TRUE);
                GUILIST_SetCurItemIndex(HERO_CTRL_ID_SELECT_ON_OFF, i);
                MMK_SetAtvCtrl(win_id,HERO_CTRL_ID_SELECT_ON_OFF);
            }
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            {
                HERO_UINT8 flag;
                uint16                  cur_selection   =   0;
                GUILIST_GetSelectedItemIndex(HERO_CTRL_ID_SELECT_ON_OFF,&cur_selection,1);
                flag = cur_selection + 1;
                if(hero_tts_apps.volume != flag)
				{
                    hero_tts_apps.volume = flag;
                    hero_tts_apps_save_setting();
                }
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(HERO_TTS_VOLUME_SETTING_WIN_TAB ) = 
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleHeroTTSVolumeWinMsg),   
    WIN_TITLE(TXT_HERO_TTS_SET_VOLUME),
    WIN_ID(HERO_MMITEST_WIN_ID1),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, HERO_CTRL_ID_SELECT_ON_OFF),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

void hero_tts_volume_setting_enter(void)
{
    MMK_CreateWin((uint32*)HERO_TTS_VOLUME_SETTING_WIN_TAB, PNULL);
}
////////////////////////////////// VOLUME END //////////////////////////////////

////////////////////////////////// ONOFF //////////////////////////////////
static MMI_TEXT_ID_T hero_tts_setting_win_title_id = 0;
LOCAL MMI_RESULT_E HandleHeroSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;
    char *flag = (char*)MMK_GetWinAddDataPtr(win_id);

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                MMI_STRING_T  title = {0};

                GUILIST_SetMaxItem(HERO_CTRL_ID_SELECT_ON_OFF, 2, FALSE );//max item 33
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
#ifdef MMI_PDA_SUPPORT
                item_data.softkey_id[0] = TXT_NULL;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#else
                item_data.softkey_id[0] = STXT_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
#endif
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID; //GUIITEM_DATA_TEXT_BUFFER;

                item_data.item_content[0].item_data.text_id = TXT_OPEN;
                GUILIST_AppendItem(HERO_CTRL_ID_SELECT_ON_OFF, &item_t);

                item_data.item_content[0].item_data.text_id = TXT_CLOSE;
                GUILIST_AppendItem(HERO_CTRL_ID_SELECT_ON_OFF, &item_t);
                
                GUILIST_SetSelectedItem(HERO_CTRL_ID_SELECT_ON_OFF, (*flag+1)&0x01, TRUE);
                GUILIST_SetCurItemIndex(HERO_CTRL_ID_SELECT_ON_OFF,(*flag+1)&0x01);

                MMIRES_GetText(hero_tts_setting_win_title_id, NULL, &title);
                GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);
                MMK_SetAtvCtrl(win_id,HERO_CTRL_ID_SELECT_ON_OFF);
            }
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            {
                uint16                  cur_selection   =   0;
                GUILIST_GetSelectedItemIndex(HERO_CTRL_ID_SELECT_ON_OFF,&cur_selection,1);

                switch( cur_selection )
                {
                    case 0:
                        *flag = 1;
                        break;
                    case 1:
                        *flag = 0;
                        break;
                    default:
                        break;
                }
                MMK_CloseWin(win_id);
                hero_tts_apps_save_setting();
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_FULL_PAINT:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE( HERO_TTS_SELECT_ONOFF_WIN_TAB ) = 
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleHeroSelectWinMsg), 
    WIN_TITLE(TXT_HERO_TTS),
    WIN_ID(HERO_MMITEST_WIN_ID2),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, HERO_CTRL_ID_SELECT_ON_OFF),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

void hero_pop_select_on_off(char *flag)
{
    MMK_CreateWin((uint32*)HERO_TTS_SELECT_ONOFF_WIN_TAB, (ADD_DATA)flag);
}

void hero_menu_item_enter(uint32 menu_id)
{
	switch(menu_id)
	{
		//case 1:
		case ID_HERO_MENU_SET_MENU:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_MENU;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_menu));
			break;
		case ID_HERO_MENU_SET_DIAL:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_DIAL;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_dial));
			break;
		case ID_HERO_MENU_SET_AKEY_TIME:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_AKEY_TIME;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_akey_time));
			break;
		case ID_HERO_MENU_SET_AUTO_TIME:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_AUTO_TIME;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_auto_time));
			break;
		case ID_HERO_MENU_SET_INCOMING:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_INCOMING;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_incoming));
			break;
		case ID_HERO_MENU_SET_PHB:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_PHB;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_phb));
			break;
		case ID_HERO_MENU_SET_SMS:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_SMS;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_sms));
			break;
		case ID_HERO_MENU_SET_NEW_SMS:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_NEW_SMS;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_new_sms));
			break;
		case ID_HERO_MENU_SET_CHARGE:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_CHARGE;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_charge));
			break;
		case ID_HERO_MENU_SET_LOW_POWER:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_LOW_POWER;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_low_power));
			break;
		case ID_HERO_MENU_SET_LOCK_SCREEN:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_LOCK_SCREEN;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_lock_screen));
			break;
		case ID_HERO_MENU_SET_AKEY_POWER_LEVEL:
			hero_tts_setting_win_title_id = TXT_HERO_TTS_SET_AKEY_POWER_LEVEL;
			hero_pop_select_on_off((char*)&(hero_tts_apps.read_akey_power_level));
			break;
		default:
			break;
	}
}
////////////////////////////////// ONOFF END //////////////////////////////////

static void hero_CreateListMenu( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, wchar* title, HERO_UINT16 title_len)
{
    GUI_RECT_T      rect;// = {0, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIMENU_INIT_DATA_T     init_data = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    void            *ctrl_ptr = PNULL;
    uint32          node_id = 0;
    uint16           i = 0;
    MMI_STRING_T    kstring = {0};
    HERO_UINT16 ** str_list;
    GUI_BOTH_RECT_T both_rect = {0};

    both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OK, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);

    kstring.wstr_ptr = title;
    if ( 0 == title_len )
	{
        kstring.wstr_len = MMIAPICOM_Wstrlen((const wchar *)title);
	}
    else
	{
        kstring.wstr_len = title_len;
	}
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    node_item.item_text_ptr = &kstring;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_VOLUME, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_MENU, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_DIAL, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_TIME, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AUTO_TIME, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_INCOMING, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_PHB, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_SMS, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_NEW_SMS, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_CHARGE, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_LOW_POWER, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_LOCK_SCREEN, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;

    memset(&kstring, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_HERO_TTS_SET_AKEY_POWER_LEVEL, NULL, &kstring);
    GUIMENU_InsertNode(i, ID_HERO_MENU1 + i, 0, &node_item, ctrl_id);
    i ++;
}

LOCAL MMI_RESULT_E HandleHeroVoiceMainWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T    kstring = {0};
    MMI_MENU_ID_T   menu_id;

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
	     	MDW_LOG_HIGH("HandleHeroVoiceMainWinMsg MSG_OPEN_WINDOW");
            MMI_GetLabelTextByLang(TXT_HERO_TTS, &kstring);
            hero_CreateListMenu(win_id, HERO_CTRL_ID_VOICE_MAIN, kstring.wstr_ptr, kstring.wstr_len);
            break;
        case MSG_GET_FOCUS:
            break;
        case MSG_CLOSE_WINDOW:
            break;
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_MIDDLE:
            {
            	menu_id = GUIMENU_GetCurNodeId(HERO_CTRL_ID_VOICE_MAIN);
            	if(menu_id == ID_HERO_MENU_SET_VOLUME) // volume
            	{
            		hero_tts_volume_setting_enter();
            	}
            	else if(menu_id == ID_HERO_MENU_SET_AUTO_TIME) // auto time
            	{
            		hero_tts_auto_time_setting_enter();
            	}
            	else if(menu_id == ID_HERO_MENU_SET_AKEY_TIME) // akey time
            	{
            		hero_tts_akey_time_setting_enter();
            	}
            	else if(menu_id == ID_HERO_MENU_SET_INCOMING) // incoming
            	{
            		hero_tts_incoming_setting_enter();
            	}
            	else if(menu_id == ID_HERO_MENU_SET_AKEY_POWER_LEVEL) // akey power level
            	{
            		hero_tts_akey_power_level_setting_enter();
            	}
            	else
            	{
            		hero_menu_item_enter(menu_id);
            	}
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_RED:
        	MMK_ReturnIdleWin();
        	break;
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_TIPS_NEED_DATA:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}


WINDOW_TABLE(HERO_TTS_SETTING_MAIN_WIN_TAB) = {
    //WIN_STATUSBAR,
    WIN_ID(HERO_MMITEST_WIN_ID), 
    WIN_FUNC((uint32)HandleHeroVoiceMainWinMsg),
    WIN_TITLE(TXT_HERO_TTS),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN};

void hero_tts_set_enter(void)
{
    MMK_CreateWin((uint32*)HERO_TTS_SETTING_MAIN_WIN_TAB, PNULL);
}
#endif // HERO_ENGINE_TTS_PLATUI
#endif // HERO_ENGINE_TTS_SUPPORT
