#ifndef __HERO_TTS_UI_H_
#define __HERO_TTS_UI_H_
#ifdef HERO_ENGINE_TTS_SUPPORT

#include "hero_types.h"

typedef enum
{
    ID_HERO_MENU0, 
    ID_HERO_MENU1, 
    ID_HERO_MENU2, 
    ID_HERO_MENU3, 
    ID_HERO_MENU_END = ID_HERO_MENU1 + 50, 
    ID_HERO_TOTAL
}MMI_HERO_MENU_ID_E;

typedef enum
{
	ID_HERO_MENU_SET_START,
	ID_HERO_MENU_SET_VOLUME,		    // auto
    ID_HERO_MENU_SET_MENU,		// 按照号码方式读
    ID_HERO_MENU_SET_DIAL,		//  按照值读
	ID_HERO_MENU_SET_AKEY_TIME,
	ID_HERO_MENU_SET_AUTO_TIME,
	ID_HERO_MENU_SET_INCOMING,
	ID_HERO_MENU_SET_PHB,
	ID_HERO_MENU_SET_SMS,
	ID_HERO_MENU_SET_NEW_SMS,
	ID_HERO_MENU_SET_CHARGE,
	ID_HERO_MENU_SET_LOW_POWER,
	ID_HERO_MENU_SET_LOCK_SCREEN,
	ID_HERO_MENU_SET_AKEY_POWER_LEVEL,
	ID_HERO_MENU_SET_END
}MMI_HERO_SETTING_MENU_ID_E;


struct hero_tts_apps_profile
{
	HERO_UINT8   read_master_switch;
	HERO_UINT8   read_menu;
	HERO_UINT8   read_dial;
	HERO_UINT8   read_akey_time;
	HERO_UINT8   read_auto_time;
	HERO_UINT8   read_auto_time_quantum_mode;
	HERO_UINT8   read_auto_time_start;
	HERO_UINT8   read_auto_time_end;
	HERO_UINT8   read_incoming;
	HERO_UINT8   read_phb;
	HERO_UINT8   read_sms;
	HERO_UINT8   read_new_sms;
	HERO_UINT8   read_charge;
	HERO_UINT8   read_low_power;
	HERO_UINT8   read_lock_screen;
	HERO_UINT8   read_akey_power_level;
	HERO_UINT8   volume;
};

enum{
	INCOMING_BROADCAST_1 = 1,
	INCOMING_BROADCAST_3,
	INCOMING_BROADCAST_5,
	INCOMING_BROADCAST_REPEAT,
	INCOMING_BROADCAST_OFF,
};

typedef struct
{
	HERO_UINT8   cur_master_switch_status;
	HERO_UINT8   cur_menu_read_status;
	HERO_UINT8   cur_dial_read_status;
	HERO_UINT8   cur_akey_time_read_status;
	HERO_UINT8   cur_auto_time_read_status;
	HERO_UINT8   cur_auto_time_quantum_mode;
	HERO_UINT8   cur_auto_time_start_time;
	HERO_UINT8   cur_auto_time_end_time;
	HERO_UINT8   cur_incoming_read_status;
	HERO_UINT8   cur_phb_read_status;
	HERO_UINT8   cur_sms_read_status;
	HERO_UINT8   cur_new_sms_read_status;
	HERO_UINT8   cur_charge_read_status;
	HERO_UINT8   cur_low_power_read_status;
	HERO_UINT8   cur_lock_screen_read_status;
	HERO_UINT8   cur_akey_power_level_read_status;
	HERO_UINT8   cur_volume_status;
	HERO_UINT8   cur_voice_speed_status; //HERO_ENGINE_TTS_VOICE_SPEED
}hero_tts_setting_struct;

#define TIHO_TTS_SPEED_MIN					-32768		/* slowest voice speed */
#define TIHO_TTS_SPEED_NORMAL				0			/* normal voice speed (default) */
#define TIHO_TTS_SPEED_MAX					+32767		/* fastest voice speed */
int hero_tts_get_speed(void);


#endif
#endif
