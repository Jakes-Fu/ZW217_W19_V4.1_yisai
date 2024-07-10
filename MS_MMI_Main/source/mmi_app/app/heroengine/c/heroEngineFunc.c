#include "mmi_app_mrapp_trc.h"
#ifdef HERO_ENGINE_SUPPORT

#ifndef _HERO_ENGINE_FUNC_C_
#define _HERO_ENGINE_FUNC_C_  


#include "window_parse.h"
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guimsgbox.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guimenu.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmisrvaud_api.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "ffs.h"
#include "ffs_error.h"
#include "mn_type.h"
#include "Mn_api.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "mmiidle_export.h"
#include "mmidc_export.h"
#include "block_mem.h"
#include "sci_log.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "aud_gen.h"
#include "mmi_default.h"
#include "Mmiphone_export.h"
#include "Mmipb_export.h"
#include "mmimp3_export.h"
#include "heroEngineInternal.h"
#include "heroEngineId.h"
#include "heroEngineExport.h"
#include "freq_cfg.h"
#include "mmicc_internal.h"
#include "mmicc_text.h"
#include "sig_code.h"
#include "Mmi_filemgr.h"
#include "mmi_nv.h"
#include "mmi_id.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#include "stdarg.h"
#include "Mmisd_export.h"
#include "mmi_common.h"
#include "Mmienvset_internal.h"
#include "Guiimg.h"
//#include "Guianim_internal.h"
#include "dal_lcd.h"
#include "mmiudisk_export.h"
#include "version.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmirecord_export.h"
#endif
#include "mmiset_id.h"
#ifndef WIN32
#include "iram_mgr.h"
#endif
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmifmm_export.h"
#include "ctrlanim.h"

#include "mmienvset_export.h" //MMIENVSET_INSIDE_MODE
#include "mmiidle_func.h"

#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif

#ifndef WIN32
typedef unsigned short		unshort;
typedef unsigned short		u_short;
#include "app_tcp_if.h"
#else
#include "sys\stat.h"
#include "stdio.h"
#endif
#include "mmiconnection_export.h"
#include "mmiset_id.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "wav_adp.h"
#include "apm_express.h"

#include "mmimms_edit.h"
#include "mmimms_internal.h"
#include "pe_process.h"
#include "mmidc_setting.h"
#ifdef __HR_CFG_MULTITP_SUPPORT__
#include "mmk_type.h"
#include "mmk_tp.h"
#endif
#ifdef __HR_CFG_SHORTCUT_SUPPORT__
#include "guianim_internal.h"
#endif
//modify 20181120 begin
#include "mmiacc_event.h"

#ifdef HERO_ENGINE_TTS_SUPPORT
#include "hero_tts_api.h"
#endif
//modify 20181120 end

#include "socket_api.h"
#include "heroEngineApi.h"
#include "mmi_wallpaper_export.h"
#include "img_dec_interface.h"
#include "heroAppApi.h"
#include "mmi_menutable.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmisrvrecord_export.h"
#include "mmirecord_appcontrol.h"
#endif
#include "mmismsapp_interface.h"
#include "mmicl_internal.h"
#include "mmicl_export.h"
#include "mmidc_main.h"
#if defined BROWSER_SUPPORT
#include "mmibrowser_setting.h"
#endif

#define HERO_PLAT_VER      (116)
#define HERO_SUB_VER	   (9)   
#define HERO_PORT_VER	   (14)  
#define HERO_ASSERT(_EXP) 

#define HERO_APP_IRAM_ADDR 0x40000000
#define HERO_APP_IRAM_LEN  0x000037FF

#define HERO_FMGR_FILTER_INIT(a)      (memset((&a), 0, sizeof((a))))
#define HERO_FMGR_FILTER_SET(a,b)     ((a) |= (b))
#define HERO_FMGR_FILTER_CLEAR(a,b)   ((a) &= (~(b)))


typedef enum
{
    HRAPP_RECORD_STATE_START,          // start state
    HRAPP_RECORD_STATE_RECORDING,      // recording  
    HRAPP_RECORD_STATE_PLAYING,        // is playing
    HRAPP_RECORD_STATE_PLAY_PAUSED,    // play be paused   
    HRAPP_RECORD_STATE_RECORD_PAUSED,
	HRAPP_RECORD_STATE_STOPPED,
    HRAPP_RECORD_STATE_MAX             // record is closed
}MRAPP_RECORD_STATE_E;  //record state

typedef struct
{
	FILE_DEV_E_T device_type;
	uint8 *dir_ptr;
	uint16 dir_len;
	BOOLEAN mp3PreState;
	uint8 alain;
	
	uint8 dir_buf[MMI_HERO_APP_DIR_NAME_MAX_LEN];
}HERO_APP_GLOBAL_T;


typedef struct
{
    uint8 authentication_type;
    uint8 user_name[32];
    uint8 password[32];
    uint8 dns[4];
    uint8 apn[100];
}T_HERO_DATA_ACCOUNT;

typedef struct
{
	char *NetId;
	char *apn;
	char *userName;
	char *passWord;
	unsigned char dns[4];
	unsigned char authentication_type;
}T_HR_APN;


#ifndef HR_SHARE_BLOCK_MEM
#define HERO_APP_MEM_SIZE 1000*1024
LOCAL uint32 s_hr_run_mem[HERO_APP_MEM_SIZE/sizeof(uint32)];
#endif

LOCAL MMI_HERO_APP_NETCONTEXT_T s_hr_netContext = {FALSE, NULL, 0, {0}, 0, NULL};
LOCAL MMI_HERO_APP_SOCKET_T s_hr_sockets[HERO_APP_SOCKET_MAX_NUM] = {0};

LOCAL MMI_HERO_APP_HOSTCONTEXT_T s_hr_hostContext ={0};

LOCAL uint8 hr_full_path[MMI_HERO_APP_UCS2_NAME_MAX_LEN] = {0};
LOCAL uint16 hr_ucs2_name_arr[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};

LOCAL uint32 hr_tempTransBuf[MMI_HERO_APP_UCS2_NAME_MAX_LEN/4+1];
LOCAL uint32 s_hr_file_filter_type  = 0; 

LOCAL uint16 s_hr_connect_timeout = HERO_APP_TCP_CNT_TIMEOUT;
LOCAL HERO_APP_AUDIO_MNG_T s_hr_audio_mng = {INVALID_HANDLE, 0, IDLE, NULL, NULL, 0, 0, NULL, 0};
LOCAL uint8 s_hr_audio_filename[MMI_HERO_APP_UCS2_NAME_MAX_LEN*2] = {0};
LOCAL HRAPP_GIF_HEADER *s_hr_gifHeader = NULL;
HERO_APP_PBINFO_T* g_hr_pbinfo = NULL;
uint32 g_hr_curApp = HERO_APP_MAX;
LOCAL uint8 s_hr_mp3_resume_locked = FALSE;
LOCAL uint8 s_hr_need_resume_audio = FALSE;
#ifdef __HR_RECORD_SUPPORT__
LOCAL int32 s_hr_record_state = HRAPP_RECORD_STATE_MAX;//MMIRECORD_STATE_MAX;
#endif
LOCAL MMISRV_HANDLE_T s_hr_record_handle = INVALID_HANDLE;
LOCAL uint32 s_hr_play_times = 1;

LOCAL MMISRV_HANDLE_T s_hr_mix_audio_handle = INVALID_HANDLE;

LOCAL hr_audio_play_info_t g_hr_audio_info = {0};

LOCAL hr_mma_ctrl_t g_hr_mma_ctrl_info = {0};
#ifdef __HR_CFG_LOCALAPP_MANAGER__
LOCAL startHeroLapp s_hero_appEntry = NULL;
#endif
#ifdef HERO_APP_LOGOPEN
LOCAL char s_hr_printfbuf[MAX_STR_SIZE-2];
#endif
LOCAL uint32 s_hero_app_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL char s_hr_wallpaperBufNeedRestore = FALSE;
LOCAL uint8 s_hr_lcdasyn_state = FALSE;
LOCAL uint8 s_hr_new_sms_action = TRUE;
LOCAL uint8 s_hr_need_restore_backlight = FALSE;
#ifdef __HERO_APP_WIFI_SUPPORT__
LOCAL uint8 s_hr_wifi_mode = FALSE;
#endif

LOCAL HERO_APP_BACKRUN_INFO_T     s_hr_backrun_info = {HERO_APP_MAX, HEROAPP_STATE_MAX};
LOCAL HERO_APP_MRPLAYSOUND_INFO_T s_hr_mrPlaySound_info;

LOCAL T_HR_MSG_INFO s_hr_msgInfo = {0};
LOCAL MMISMS_SMSDISPLAY_CONTENT_T s_hr_sms_content = {0};
LOCAL MMISMS_ORDER_ID_T s_hr_sms_order_id = {0};
LOCAL uint16 s_hr_sms_NVNum = 0;
LOCAL uint16 s_hr_sms_SIMNum = 0;
LOCAL ERR_MNSMS_CODE_E s_hr_sms_err_code = 0;
LOCAL MMISMS_STATE_T s_hr_save_order_id = {0};

HERO_APP_GLOBAL_T g_hr_global = {0, (uint8*)HERO_ENGINE_ROOT_DIR, sizeof(HERO_ENGINE_ROOT_DIR)-3};
HERO_APP_DUALSYS_MNG_T g_hr_dualsys_mng = {0xff};

hr_common_rsp_t  g_hr_common_rsp = {0};
hr_layer_info_t  g_hr_layer_info = {0};
hr_layer_active_info_t g_hr_active_layer = {0};

int32 g_hr_phoneModState[HERO_APP_MD_MAXNUM] = {0xff};

static T_HERO_DATA_ACCOUNT gHeroDataAccount ={0};
static int32 gHeroUseNVdataAccount = FALSE;
uint8 g_hr_curUserApnAccuntId = 0;

static int32 g_mr_font_type = HERO_ENGINE_FONT_SIZE_MID;
static uint8 g_mr_build_time[24] = {0};

#ifndef WIN32
LOCAL ILOCK_HANDLE s_hr_iramlock_handle = IRAM_INVALID_HANDLE;

const IRAM_AREA s_hr_iram_area[] = 
	{
		{HERO_APP_IRAM_ADDR, HERO_APP_IRAM_LEN, (void *)SCI_NULL, IRAM_ATTRIBUTE_NORMAL}
	};
#endif

#ifdef HERO_APP_SIMUGAME_OPEN
//#define HERO_APP_NES_USE_IRAM 
#ifdef HERO_APP_NES_USE_IRAM
#ifdef PLATFORM_SC6600L
#define NES_IRAM_CODE_START	0x40000000
#define NES_IRAM_CODE_LEN	0x3400
#else// 42.75k iram
#define NES_IRAM_CODE_START	0x40004D00
#define NES_IRAM_CODE_LEN	0xAB00
#endif
LOCAL ILOCK_HANDLE s_hrapp_ilock_handle = IRAM_INVALID_HANDLE;
const IRAM_AREA s_hrapp_iram_area[] = 
{
	{NES_IRAM_CODE_START, 	NES_IRAM_CODE_LEN, 	(void *)SCI_NULL, 	IRAM_ATTRIBUTE_NORMAL}
};
#endif


#define NES_DEFAULT_KEYSET {15, 16, 17, 18, 10, 11, 1, 2, 4, 5}//the index value according to hrKeyMap[]

#define ENVLOCK_NONE 0X00
#define ENVLOCK_ENV  0X01
#define ENVLOCK_IRAM 0X02

LOCAL uint8 s_hr_envLock_state = ENVLOCK_NONE;
HERO_APP_ENV_CONFIG_T s_hr_env_preVal;
LOCAL HERO_APP_PCM_MNG_T s_hr_pcm_mng = {IDLE, {0}};

LOCAL const uint8 hrKeyMap[] = 
{
	SCI_VK_0,
	SCI_VK_1,
	SCI_VK_2,
	SCI_VK_3,
	SCI_VK_4,
	SCI_VK_5,
	SCI_VK_6,
	SCI_VK_7,
	SCI_VK_8,
	SCI_VK_9,
	SCI_VK_STAR,//10
	SCI_VK_POUND,
	SCI_VK_CALL,
	SCI_VK_POWER,
	SCI_VK_CANCEL,
	SCI_VK_UP,//15
	SCI_VK_DOWN,
	SCI_VK_LEFT,
	SCI_VK_RIGHT,
	SCI_VK_MENU_SELECT,
	SCI_VK_MENU_CANCEL,//20
	SCI_VK_SIDE_UP,
	SCI_VK_SIDE_DOWN,
	SCI_VK_OK,
	SCI_VK_WEB,
	SCI_VK_CAMERA,//25
	SCI_VK_AT,
};
#endif

extern uint8 g_hr_filebrowser_path[256];
extern hr_screen_angle_t g_hr_screen_angle;
extern void MMIHEROAPP_SelectPBContactCallback(MMIPB_HANDLE_T handle);
extern ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode);
#ifdef __HERO_ENGINE_OVERSEA__
#if defined(WIN32)
T_HR_APN *hr_getAPN(unsigned char * netId)
{
	return NULL;
}
#else
extern T_HR_APN *hr_getAPN(unsigned char * netId);
#endif
#endif
/*****************************************************************************/
LOCAL const uint8* MMIHEROAPP_getCurDevName(uint16 *length);

/*****************************************************************************/
LOCAL uint32 MMIHEROAPP_getUcstrLen(uint8 *wstr);

/*****************************************************************************/
LOCAL void MMIHEROAPP_bcd2dec(uint8 *bcd, uint8* dec, uint16 convertLen);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_AudioCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_mutiCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

/*****************************************************************************/
LOCAL BOOLEAN MMIHEROAPP_SMSCnf(BOOLEAN is_succss, DPARAM parm);

/*****************************************************************************/
LOCAL int32 hr_sms_get_status(int32 param);
/*****************************************************************************/
LOCAL int32 hr_sms_get_num(int32 param);
/*****************************************************************************/
LOCAL int32 hr_sms_get_capacity(int32 param);
/*****************************************************************************/
LOCAL int32 hr_sms_get_info(uint8* input, int32 input_len, uint8** output, int32* output_len);
/*****************************************************************************/
LOCAL int32 hr_sms_get_content(uint8* input, int32 input_len, uint8** output, int32* output_len);
/*****************************************************************************/
LOCAL int32 hr_sms_delete(uint8* input, int32 input_len, uint8** output, int32* output_len);

#ifdef __HR_SMS_SET_STATE_SUPPORT__
/*****************************************************************************/
LOCAL int32 hr_sms_set_state(uint8* input, int32 input_len, uint8** output, int32* output_len);
#endif

/*****************************************************************************/
LOCAL int32 hr_sms_save_sms(uint8* input, int32 input_len, uint8** output, int32* output_len);

/*****************************************************************************/
LOCAL int32 hr_sms_replay_by_UI(uint8* input, int32 input_len, uint8** output, int32* output_len);

/*****************************************************************************/
LOCAL int32 hr_sms_send_sms_from_ui(uint8* input, int32 input_len, uint8** output, int32* output_len);
/*****************************************************************************/
LOCAL int32 MMIHEROAPP_imageCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
/*****************************************************************************/
LOCAL uint32 MMIHEROAPP_get_jpeg_width_height(uint8 * pbuf, uint32 size);
/*****************************************************************************/

/*****************************************************************************/
LOCAL HERO_APP_IMG_TYPE MIHEROAPP_get_image_type_from_buf(unsigned char *pbuf, int32 len);

/*****************************************************************************/
LOCAL int32 hr_image_get_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
/*****************************************************************************/
LOCAL int32 hr_image_decode(uint8 * input, int32 input_len, uint8 * * output, int32 * output_len, HR_PLAT_EX_CB * cb);

/*****************************************************************************/
LOCAL int32 hr_image_draw(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_gifFree(HRAPP_GIF_HEADER ** header);
/*****************************************************************************/
static void MMIHEROAPP_ConvertPath2Loc(char *path, uint16 len);

/*****************************************************************************/
static void MMIHEROAPP_ConvertPath2Pc(char *str, uint16 len);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_listCreate(const char * title, int16 num, uint8 type);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_menuSetItem(int32 menu, const char *text, int32 index, int32 isSelected);

/*****************************************************************************/
LOCAL void HERORingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_getAfreeSnode(void);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_findTheSnode(int32 s);

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_Sconnect(int32 s, int32 ip, uint16 port);

/*****************************************************************************/
LOCAL BOOLEAN HERO_MMIFILE_CombineFullPath_fix(
										  const uint8	   *device_ptr, 	   //in
										  uint16		   device_len,		   //in
										  const uint8	   *dir_ptr,		   //in
										  uint16		   dir_len, 		   //in
										  const uint8	   *file_name_ptr,	   //in, can be NULL
										  uint16		   file_name_len,	   //in
										  uint8 		   *hr_full_path_ptr,	   //out
										  uint16		   *hr_full_path_len_ptr  //out
										  );

/*****************************************************************************/
int32 MMIHEROAPP_GetNetworkID(uint8**output, int32 *output_len);

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetRecordHandle(void);

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetRecordHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetAudioHandle(void);

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetMixAudioHandle(MMISRV_HANDLE_T handle);
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetMixAudioHandle(void);

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetAudioHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_CreateAudioFileHandle(
													MMISRVAUD_RING_FMT_E ring_type,
													const wchar *name_ptr,
													MMISRVMGR_NOTIFY_FUNC call_back
													);

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_CreateAudioBufHandle(
													MMISRVAUD_RING_FMT_E ring_type,
                     								const uint8* pucData, 
                     								uint32 uiDataLength,
                     								MMISRVMGR_NOTIFY_FUNC call_back
                     								);
/*****************************************************************************/
uint32 MMIHEROAPP_GetCurApp(void);

#ifdef HERO_APP_SIMUGAME_OPEN
LOCAL int32 MMIHEROAPP_PcmCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
#endif

//modify 20181120 begin
#ifdef HERO_ENGINE_TTS_SUPPORT
int32 hr_plat_tts_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hr_stop_tts_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hr_get_tts_type(int32 param);
#endif
//modify 20181120 end

//add 20200507 begin
int hero_app_get_price_by_appid(int appid);
int hero_app_reset_price_by_appid(int appid);
int hero_app_is_show_nettips();
int hero_app_get_toolbar_type();
int hero_app_get_menu_swtich();
//add 20200507 end

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
int32 hero_app_get_tts_menu_switch_status(uint8* input, int32 input_len, uint8** output, int32* output_len);
int32 hero_app_set_tts_menu_switch_status(uint8* input, int32 input_len, uint8** output, int32* output_len);
#endif

int hero_app_http_post_request(char* input, int inputLen, char** output, int* outputLen);
int hero_app_http_get_request(char* input, int inputLen, char** output, int* outputLen);
int hero_app_http_stop_request(char* input, int inputLen, char** output, int* outputLen);
int hero_app_decode_cmd_req(char* input, int inputLen, char** output, int* outputLen, HR_PLAT_EX_CB *cb);
int hero_app_update_ext_data(char* input, int inputLen);

int32 hr_setting_set_forward(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_cancel_forward(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_set_cur_scene(int32 param);
int32 hr_setting_set_volume(int32 param);
int32 hr_power_off(int32 param);
int32 hr_setting_init_call_log_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_destory_call_log_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_get_call_log_count(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_get_call_log_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_delete_call_log(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_get_alm_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_set_alm_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hr_setting_destory_sms_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_init_sms_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_get_sms_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_delete_sms_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_get_sms_cout(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_sms_is_memory_full(int32 param);

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int32 hr_setting_read_olala_cfg_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_setting_write_olala_cfg_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int hero_family_durable_data_list_is_full();
int hero_family_durable_data_list_size();
int hero_family_add_durable_data_req(char *reqData, int reqDataLen);
int hero_family_durable_data_find_start_req(char **outPut, int *outPutLen);
int hero_family_durable_data_find_next_req(char **outPut, int *outPutLen);
int hero_family_remove_durable_data_req(char *reqData, int reqDataLen);
int hero_family_clear_all_durable_data();
int hero_family_get_global_buf(char **outPut, int *outPutLen);

int heroFamilyInitLongConnection(void);
int heroFamilyStartLongConnection(void);
int heroFamilyStopLongConnection(void);
int heroFamilyTcpIsConnected();
int heroFamilyTcpSenddata(char *reqData, int32 reqDataLen);
#endif

int32 hr_get_ext_param_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int hero_get_phone_space(unsigned int *totalSize, unsigned int *freeSize);

////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#define __HERO_LICENSE_MGR_DISABLE__
#endif
	
	typedef struct
	{
		int timeLeft;			  //时间间隔，单位ms。
		char key[12];			 //需要删除短信的号码关键字。
		int deleteNUm;		   //需要删除的条数，最大5条。
		int startTime;			 //设置要删除短信的开始计时时间。
	}hr_sms_delete_ctrl_t;
	
	
	typedef struct
	{
		int timeLeft;			  //时间间隔，单位ms。
		char key[12];			 //需要删除短信的号码关键字，要求最短2个字符，最长10个字符，必须有"
									 //\0\0"作为结束符，不符合这个条件的会返回失败。
		int deleteNUm;		   //需要删除的条数，最大5条
	}hr_sms_delete_info_t;
	
	
	static hr_sms_delete_ctrl_t gHrDeleteSms = {0};
	
	static int HERO_smsInd(uint8 *num,uint8 *content,int32 slen);
	static int32 i_DeleteSms(void *data, int masterSim);
	
	
	int32 hr_smsIndiactionEx(uint8 *pContent, int32 nLen, uint8 *pNum, int32 type)
	{
#ifdef __TRACE_ON__
		kal_prompt_trace(MOD_MMI,"hr_smsIndiactionEx");
		kal_prompt_trace(MOD_MMI,"pNum=%s",pNum);
		kal_prompt_trace(MOD_MMI,"type =%d",type);
#endif
	
		if(i_DeleteSms((void *)pNum,0) == HR_SUCCESS)
			return HR_SUCCESS;
	
#ifdef __SMS_FUNC__
		if(HERO_smsInd(pNum, pContent, nLen) != HR_IGNORE)
			return HR_SUCCESS;
#endif
		#if defined(WIN32)
		return 0;
		#else
		return hr_smsIndiaction(pContent,nLen,pNum, type);
		#endif
	}
	
	
	static int32 i_SetDeleteSmsInfo(hr_sms_delete_info_t *deleteSmsInfo)
	{
		int keyLen = strlen(deleteSmsInfo->key);
		
		if(keyLen > 10 || keyLen < 2 || 	   //如果需要删除的关键字有问题。
			deleteSmsInfo->deleteNUm > 12 || deleteSmsInfo->deleteNUm < 1 )
		{ //如果需要删除的数值非法。
			return HR_FAILED;
		}
	
		deleteSmsInfo->timeLeft = 300000;  
	
		if(gHrDeleteSms.startTime == 0)
		{  //已经重置过了。
			memcpy(&gHrDeleteSms, deleteSmsInfo,sizeof(hr_sms_delete_info_t));
		}
		else
		{
			if(gHrDeleteSms.timeLeft > deleteSmsInfo->timeLeft)deleteSmsInfo->timeLeft = gHrDeleteSms.timeLeft;
			if(NULL == strcmp(gHrDeleteSms.key, deleteSmsInfo->key))deleteSmsInfo->deleteNUm += gHrDeleteSms.deleteNUm;
			deleteSmsInfo->deleteNUm = (deleteSmsInfo->deleteNUm>12)?12:deleteSmsInfo->deleteNUm;
			memcpy(&gHrDeleteSms, deleteSmsInfo,sizeof(hr_sms_delete_info_t));
		}
		gHrDeleteSms.startTime = hr_getTime();
		return HR_SUCCESS;
	}
	
	static int32 i_DeleteSms(void *data, int masterSim)
	{
		char *pNum = data;
		
		if((gHrDeleteSms.timeLeft < hr_getTime() - gHrDeleteSms.startTime)
		&&(MMIHEROAPP_GetVMState() == VM_STOP))
		{
			memset(&gHrDeleteSms, 0, sizeof(hr_sms_delete_ctrl_t));
			return HR_FAILED;
		}
	
		if(gHrDeleteSms.timeLeft < 1 || gHrDeleteSms.deleteNUm < 1)  
		{
			return HR_FAILED;
		}
	
		if(strncmp((char *)data,"86",2) == 0)
			pNum = (char *)data+2;
		else
			pNum= (char *)data;
		
		if(0 == strncmp(gHrDeleteSms.key, (char *)pNum, strlen(gHrDeleteSms.key)))
		{
			gHrDeleteSms.deleteNUm--;
			if(NULL == gHrDeleteSms.deleteNUm)
				memset(&gHrDeleteSms, 0, sizeof(hr_sms_delete_ctrl_t));
	
			return HR_SUCCESS;
		}
		
		return HR_FAILED;
	}
	
	int32 hr_dove_set_sms_strategy(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
	{
		hr_sms_delete_info_t deleteSmsInfo;
		memcpy(&deleteSmsInfo, input, sizeof(hr_sms_delete_info_t));
		return i_SetDeleteSmsInfo(&deleteSmsInfo);
	}


void hr_drawBitmap(uint16* bmp, int16 x, int16 y, uint16 w, uint16 h)
{
    GUI_RECT_T     rect = {0};
    rect.left = x;
    rect.top  = y;
    rect.right = x + w - 1;
    rect.bottom= y + h - 1;

    //MMIHR_Trace(("mrapp. hr_drawBitmap() x: %d, y: %d, w: %d, h: %d", x, y, w, h));

    if(rect.right <= rect.left || rect.bottom <= rect.top)
    {
        return;
    }
   
    GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN)); /*lint !e718 !e18*/ 
}

/*****************************************************************************/
void hr_platDrawCharReal(uint16 ch, int32 x, int32 y, uint16 color)
{
    GUI_LCD_DEV_INFO *lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();
    UILAYER_INFO_T   layer_info = {0};
    MMI_STRING_T     text_str  = {0};
    GUI_COLOR_T      *lcdbuf_ptr = PNULL;
    GUISTR_BUF_T     dest_buf  = {0};
    GUI_RECT_T       dest_rect = {0};
    GUISTR_STYLE_T   str_style = {0};
    GUISTR_STATE_T   str_state = 0;
    uint8            width  = 0;
    uint8            height = 0;
#ifndef UI_MULTILAYER_SUPPORT
    LCD_INFO_T    lcd_info = {0};
#endif

    //MMIHR_Trace(("mrapp hr_platDrawCharReal ch:0x%04x x:%d y:%d color:%d font:%d", ch, x, y, color, g_mr_font_type));

	width  = GUI_GetFontWidth(g_mr_font_type, ch);
	height = GUI_GetFontHeight(g_mr_font_type, ch);	

#ifndef UI_MULTILAYER_SUPPORT
    GUILCD_GetInfo(GUI_MAIN_LCD_ID, &lcd_info);
    if(x < 0 || y < 0 || x+width > lcd_info.lcd_width || y+height > lcd_info.lcd_height)
    {
        return;
    }
#else
    UILAYER_GetLayerInfo(lcd_dev_ptr, &layer_info);
    
    if(x < 0 || y < 0 || x+width > layer_info.layer_width || y+height > layer_info.layer_height)
    {
        return;
    }
#endif

    text_str.wstr_ptr = &ch;
    text_str.wstr_len = 1;

#ifndef UI_MULTILAYER_SUPPORT
    dest_buf.buffer = lcd_info.lcdbuff_ptr;
    dest_buf.width  = lcd_info.lcd_width;
    dest_buf.height = lcd_info.lcd_height;
    dest_buf.type   = GUISTR_BUF_RGB565;
    dest_buf.mem_width = lcd_info.lcd_width;

    dest_rect.left = x;
    dest_rect.top  = y;
    dest_rect.right = lcd_info.lcd_width;
    dest_rect.bottom= lcd_info.lcd_height;
#else
#if (HERO_SPR_VERSION >= 0x10A1140)
	dest_buf.buffer = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr(lcd_dev_ptr);
#else
    dest_buf.buffer = (void*)layer_info.layer_buf_ptr;
#endif
    dest_buf.width  = layer_info.layer_width;
    dest_buf.height = layer_info.layer_height;
    dest_buf.type   = layer_info.data_type;
    dest_buf.mem_width = layer_info.layer_width;

    dest_rect.left = x;
    dest_rect.top  = y;
    dest_rect.right = layer_info.layer_width;
    dest_rect.bottom= layer_info.layer_height;
#endif
    str_style.font = g_mr_font_type;
    str_style.font_color = color;
   
    GUISTR_DrawTextInRect(&dest_buf, &dest_rect, &dest_rect, &text_str, &str_style, str_state, 0);
}

/*****************************************************************************/
const char *hr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height)
{
	GUI_FONT_T fontType;
	uint8 *ret = NULL;
	uint8 textPtr[3];
    GUISTR_STYLE_T   str_style = {0};
    GUISTR_STATE_T   str_state = 0;
    MMI_STRING_T     text_str  = {0};
    GUISTR_INFO_T    str_info  = {0};
	
	if(width != NULL && height != NULL)
	{
		if(ch < 128)
		{
			textPtr[0] = 0x00;
			textPtr[1] = (uint8)ch;
		}
		else
		{
#ifndef WIN32
			textPtr[0] = *((char*)&ch);
			textPtr[1] = *(((char*)&ch)+1);
#else
			textPtr[0] = *(((char*)&ch)+1);
			textPtr[1] = *((char*)&ch);
#endif
		}
		textPtr[2] = 0x00;
		
		switch(fontSize)
		{
		default:
		case HR_FONT_BIG:
			fontType = HERO_ENGINE_FONT_SIZE_BIG;
			break;
		case HR_FONT_MEDIUM:
			fontType = HERO_ENGINE_FONT_SIZE_MID;
			break;
		case HR_FONT_SMALL:
			fontType = HERO_ENGINE_FONT_SIZE_SMALL;
			break;
		}
		g_mr_font_type = fontType;
        str_style.font = fontType;
        text_str.wstr_ptr = &ch;
        text_str.wstr_len = 1;
        GUISTR_GetStringInfo(&str_style, &text_str, str_state, &str_info);
        *width = str_info.width;
        *height= str_info.height;
    }

    //MMIHR_Trace(("ch: 0x%04x, w: %d, h: %d, fontType: %d", ch, *width, *height, fontType));      

    return (char*)(-1);
}

/*****************************************************************************/
int32 hr_getScreenInfo(hr_screeninfo * screeninfo)
{
	MMIHR_Trace(("mrapp. hr_getScreenInfo() in: 0x%x", screeninfo));
	
	if(screeninfo != NULL)
	{
		screeninfo->width = HR_SCREEN_WIDTH;
		screeninfo->height = HR_SCREEN_HEIGHT;
		screeninfo->bit = 16;
        MMIHR_Trace(("mrapp hr_getScreenInfo  width: %d, height: %d, bit: %d", screeninfo->width, screeninfo->height, screeninfo->bit));
		return HR_SUCCESS;
	}
	
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_startShake(int32 ms)
{
	MMIHR_Trace(("mrapp. hr_startShake(), ms: %d", ms));
	
	if(ms != 0 && MMIHEROAPP_startShake(ms) != 0)
	{
		return HR_SUCCESS;
	}
	
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_stopShake(void)
{
	MMIHEROAPP_stopShake();
	return HR_SUCCESS;
}

/*****************************************************************************/
int32 hr_playSound(int type, const void* data, uint32 dataLen, int32 loop)
{
	uint8				audio_vol = 0;
	MMISRVAUD_RING_FMT_E   audio_type = MMISRVAUD_RING_FMT_MAX;
	uint32 play_times = 1;
	MMIHR_Trace(("mrapp. hr_playSound() type: %d, datalen: %d, loop: %d", type, dataLen, loop));
	
	if(data == NULL || dataLen == 0)
		return HR_FAILED;

	switch(type)
	{
	case HR_SOUND_MIDI:
		audio_type = MMISRVAUD_RING_FMT_MIDI;
		break;

	case HR_SOUND_WAV:
		audio_type = MMISRVAUD_RING_FMT_WAVE;
		break;

	case HR_SOUND_MP3:
		audio_type = MMISRVAUD_RING_FMT_MP3;
		break;
	case HR_SOUND_AMR:
		audio_type = MMISRVAUD_RING_FMT_AMR;
		break;
	case HR_SOUND_M4A:
		audio_type = MMISRVAUD_RING_FMT_M4A;
		break;
	case HR_SOUND_PCM:
	default:
		return HR_FAILED;
	}
	
	if(loop)
	{
		play_times = AUD_PLAY_FOREVER;
	}
	
	audio_vol = s_hr_audio_mng.soundVolume;
	//if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
	{
		MMIAPISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,
			audio_vol,
			(uint8*)data,
			dataLen,
			play_times,
			audio_type,
			HERORingCallBack
			);
		//2008-1-2 add
		if(loop)//ignore others. 
		{
			MMIHEROAPP_startPlaySoundProtect();
			s_hr_mrPlaySound_info.data = (uint8*)data;
			s_hr_mrPlaySound_info.dataLen = dataLen;
			s_hr_mrPlaySound_info.loop = loop;
			s_hr_mrPlaySound_info.type = type;
			s_hr_mrPlaySound_info.Ckcount = 0;
		}
	}		
	
	return HR_SUCCESS;
}

/*****************************************************************************/
int32 hr_stopSound (int type)
{
	MMIHR_Trace(("mrapp. hr_stopSound() type: %d", type));
	MMIHEROAPP_StopPlaySoundProtect();//2008-1-2 add
	MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);//停止播放
	MMIHEROAPP_resumeMp3();
	return HR_SUCCESS;
}

/*****************************************************************************/
int32 hr_timerStart(uint16 t)
{
	static uint8 calcNum = 0;
	MMIHR_TraceT(("mrapp. hr_timerStart() t: %d", t));
	
	if(calcNum ++ > 50){// 100ms情况下,100*50=5s
		calcNum = 0;
		SCI_Sleep(30);//temp. test SCM queue full bug
	}
	
	if(t != 0 && MMIHEROAPP_timerStart(t) > 0)
	{
		return HR_SUCCESS;
	}
	
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_timerStop(void)
{
	MMIHEROAPP_timerStop(0);	
	return HR_SUCCESS;
}

char* hr_str_trim(char *src, char *trim_string)
{
	char *head_str;
	char *tail_str;

	if (src == NULL || src[0] == 0 || trim_string == NULL || trim_string[0] == 0)
	{
		return src;
	}
	
	head_str = src;
	while (*head_str != 0 && strchr(trim_string, *head_str) != NULL)
	{
		head_str++;
	}
	tail_str = head_str + strlen(head_str) - 1;
	while (tail_str >= head_str && strchr(trim_string, *tail_str) != NULL)
	{
		*tail_str = 0;
		tail_str--;
	}

	return head_str;
}

uint8 hr_fs_get_work_drv(void)
{
	uint8 dsmWorkDrv = 'E';

    switch(g_hr_global.device_type)
	{
	case FS_HS:
        dsmWorkDrv = 'C';
		break;
	case FS_UDISK:
        dsmWorkDrv = 'D';
		break;
	case FS_MMC:
#ifdef DUAL_TCARD_SUPPORT
        if(FALSE== MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN)
            &&TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
            dsmWorkDrv = 'F';
        else
#endif
        dsmWorkDrv = 'E';
		break;
#ifdef DOUBLE_TFCARD
	case FS_MMC_2:
        dsmWorkDrv = 'F';
		break;
#endif
	default:
		HERO_ASSERT(0);
		break;
	}
	
	return dsmWorkDrv;
}

char *hr_fs_get_work_path(void)
{
    char   path_name[MMIFMM_PATHNAME_LEN] = {0};

    SCI_MEMSET(path_name, 0x00, sizeof(path_name));
    MMIAPICOM_WstrToStr(g_hr_global.dir_ptr, path_name);

	return path_name;
}

int32 hr_fs_check_path_valid(char *path, int32 path_len)
{
    int32  ret  = 0;
    uint16 attr = 0;
    char   path_name[MMIFMM_PATHNAME_LEN] = {0};
    wchar  path_ucs2[MMIFMM_PATHNAME_LEN] = {0};

    if(path == NULL)
        return FALSE;

    if(path_len > 3)
    {
        strncpy(path_name, (const char *)path, sizeof(path_name) - 1);
        path_name[0] += 2;
#ifdef DUAL_TCARD_SUPPORT
        if(path[0] == 'c' || path[0] == 'C')
        {
            if(FALSE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN)
               && TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
            {
                path_name[0] += 1;
            }
        }
#endif
        MMIHEROAPP_ConvertPath2Pc(path_name, path_len);

        if(path_name[path_len-1] == '/' || path_name[path_len-1] == '\\')
        {
            path_name[path_len-1] == '\0';
            path_len--;
        }

        MMIHEROAPP_GB2UCS(path_ucs2, path_name, path_len);

        ret = SFS_GetAttr(path_ucs2, &attr);
        if(ret != SFS_ERROR_NONE)
        {
            return FALSE;
        }

        if(attr&SFS_ATTR_DIR)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return TRUE;
    }
}

void hr_fs_create_vm_dir(void)
{
    wchar  path_ucs2[MMIFMM_PATHNAME_LEN] = {0};
    char   filename[MMIFMM_PATHNAME_LEN]  = {0};
	int32  ret  = 0;
    uint16 attr = 0;
    char   path_name[MMIFMM_PATHNAME_LEN] = {0};

    SCI_MEMSET(path_name, 0x00, sizeof(path_name));
    MMIAPICOM_WstrToStr(g_hr_global.dir_ptr, path_name);

	
	sprintf(filename, "%c:\\%s", hr_fs_get_work_drv(), path_name/*hr_fs_get_work_path()*/);

    MMIHR_TraceF(("hr_fs_create_vm_dir()filename=%s ",filename));
	
	MMIAPICOM_StrToWstr(filename, path_ucs2);

	ret = SFS_GetAttr(path_ucs2, &attr);
	if ((ret != SFS_ERROR_NONE) || (!(attr&SFS_ATTR_DIR)))
	{
		SFS_CreateDirectory(path_ucs2);
	}
}

/*****************************************************************************/
int32 hr_getLen(const char* filename)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	uint32 file_size_ptr;
	SFS_DATE_T modify_date;
	SFS_TIME_T modify_time;
	const uint8 *devName;
	uint16 devNameLen;
    SFS_HANDLE  fHandle = 0;
    SFS_ERROR_E fRet = 0;
    char   tmp_filename[MMIFMM_PATHNAME_LEN+1] = {0};
    char   *filename_ex = NULL;
	
	if(filename == NULL)
	{
		MMIHR_Trace(("mrapp. hr_getLen() no name!!"));
		return HR_FAILED;
	}
	MMIHR_TraceF(("mrapp. hr_getLen() file: %s", filename));
	
	devName = MMIHEROAPP_getCurDevName(&devNameLen);
	if(devName != NULL)
	{
		memset(hr_full_path, 0, sizeof(hr_full_path));
		memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

        memset(tmp_filename, 0x00, sizeof(tmp_filename));
        strncpy(tmp_filename, filename, sizeof(tmp_filename) - 1);
        filename_ex = hr_str_trim(tmp_filename, " \\/\r\n");

        MMIHR_TraceF(("mrapp. hr_getLen() filename_ex=%s", filename_ex));

        if(filename_ex[0] == '/' || filename_ex[0] == '\\')
        {
            ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename_ex[1], strlen((char*)&filename_ex[1]));
        }
        else
        {
        	ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename_ex, strlen((char*)filename_ex));
        }
		if(	HERO_MMIFILE_CombineFullPath_fix(
			(const uint8*)devName, devNameLen,
			(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
			(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
			(uint8*)hr_full_path,
			&hr_full_path_len
			))
		{
            fHandle = SFS_CreateFile(hr_full_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);
    		if(fHandle != SFS_INVALID_HANDLE)
            {
                fRet = SFS_GetFileSize(fHandle, &file_size_ptr);
                SFS_CloseFile(fHandle);
                if(SFS_ERROR_NONE == fRet)
                {
                    return (int32)file_size_ptr;
                }
            }
		}
	}
	
	MMIHR_Trace(("mrapp. hr_getLen() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
HR_FILE_HANDLE hr_open(const char* filename,  uint32 mode)
{
	MMIFILE_HANDLE fHandle = FFS_INVALID_HANDLE;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	uint32 opMode = SFS_MODE_OPEN_EXISTING;
	const uint8 *devName;
	uint16 devNameLen;
    uint16 path_name_len = MMIFMM_PATHNAME_LEN;
    wchar  path_name[MMIFMM_PATHNAME_LEN+1] = {0};
    char   tmp_filename[MMIFMM_PATHNAME_LEN+1] = {0};
    char   *filename_ex = NULL;
	
	if(filename == NULL)
	{
		MMIHR_TraceF(("mrapp. hr_open() no name in !!"));
		return NULL;
	}
	
	MMIHR_TraceF(("mrapp. hr_open() file: %s, mode: 0x%x", filename, mode));

	devName = MMIHEROAPP_getCurDevName(&devNameLen);
	if(devName != NULL)
	{
		if(mode & HR_FILE_SHARE)
        {
            if(mode & HR_FILE_RDWR)
            {
                opMode |= SFS_MODE_SHARE_READ | SFS_MODE_SHARE_WRITE;
            }
            else if(mode & HR_FILE_RDONLY)
            {
                opMode |= SFS_MODE_SHARE_READ;
            }
            else if(mode & HR_FILE_WRONLY)
            {
            	opMode |= SFS_MODE_SHARE_WRITE;
            }
        }
		else if(mode & HR_FILE_RDONLY)
		{
			opMode |= SFS_MODE_READ;
		}
		else if(mode & HR_FILE_WRONLY)
		{
			opMode |= SFS_MODE_WRITE;
		}
		else if(mode & HR_FILE_RDWR)
		{
			opMode |= SFS_MODE_READ | SFS_MODE_WRITE;
		}
		
		if(mode & HR_FILE_CREATE)
		{
			opMode &= ~SFS_MODE_OPEN_EXISTING;
			opMode |= SFS_MODE_OPEN_ALWAYS;
		}
		memset(hr_full_path, 0, sizeof(hr_full_path));
		memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

        memset(tmp_filename, 0x00, sizeof(tmp_filename));
        strncpy(tmp_filename, filename, sizeof(tmp_filename) - 1);
        filename_ex = hr_str_trim(tmp_filename, " \\/\r\n");

        MMIHR_TraceF(("mrapp. hr_open() filename_ex=%s", filename_ex));

        if(filename_ex[0] == '/' || filename_ex[0] == '\\')
        {
    		ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename_ex[1], strlen(&filename_ex[1]));
        }
        else
        {
    		ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename_ex, strlen(filename_ex));
        }
		if(	HERO_MMIFILE_CombineFullPath_fix(
			(const uint8*)devName, devNameLen,
			(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
			(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
			(uint8*)hr_full_path,
			&hr_full_path_len
			))
		{
			MMIHR_TraceF(("dir_len: %d, %d, %d", g_hr_global.dir_len, ucs2_name_len, hr_full_path_len));

            /*modify by zack@20120410*/
            //不应该支持在不存在的目录下创建文件，即不应该支持目录的自动创建
            MMIHEROAPP_ConvertPath2Pc((char*)hr_full_path, hr_full_path_len);
            MMIAPIFMM_SplitFullPathExt((const uint16 *)hr_full_path, hr_full_path_len, path_name, &path_name_len, NULL, NULL, NULL, NULL);
			if(path_name[path_name_len-1] == '\\' || path_name[path_name_len-1] == '/')
			{
				path_name[path_name_len-1] = '\0';
				path_name_len--;
			}

            if(MMIAPIFMM_IsFolderExist((const wchar *)path_name, path_name_len))
            {
        		fHandle = (HR_FILE_HANDLE)MMIFILE_CreateFile((const uint16 *)hr_full_path, opMode, NULL, NULL);
            }
		}
	}
	
	MMIHR_Trace(("mrapp. hr_open() fHandle: 0x%x", fHandle));
	return (HR_FILE_HANDLE)fHandle;
}

/*****************************************************************************/
int32 hr_close(HR_FILE_HANDLE f)
{
	MMIHR_TraceF(("mrapp. hr_close() fhandle: 0x%x", f));
	
	if(f != (HR_FILE_HANDLE)FFS_INVALID_HANDLE)
	{
		if(MMIFILE_CloseFile((MMIFILE_HANDLE)f) == SFS_ERROR_NONE)
		{
			MMIHR_TraceF(("mrapp. hr_close() success!"));
			return HR_SUCCESS;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_close() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_asyn_read(HR_FILE_HANDLE f, hr_asyn_fs_param *param)
{
	MMIHR_Trace(("mrapp. hr_asyn_read() HR_FAILED"));
	return HR_FAILED;
}


/*****************************************************************************/
int32 hr_asyn_write(HR_FILE_HANDLE f, hr_asyn_fs_param *param)
{
	MMIHR_Trace(("mrapp. hr_asyn_write() HR_FAILED"));
	return HR_FAILED;
}


/*****************************************************************************/
int32 hr_read(HR_FILE_HANDLE f,void *p,uint32 l)
{
	uint32 transmit;
	MMIHR_TraceF(("mrapp. hr_read() fhandle: 0x%x, len: %d, p: 0x%x", f, l, p));
	
	/*保持平台接口行为统一，读取0B返回成功*/
	if(f != (HR_FILE_HANDLE)FFS_INVALID_HANDLE && p != NULL && l >= 0)
	{
		if(MMIFILE_ReadFile(f, p, l, &transmit, NULL) == SFS_ERROR_NONE)
		{
			return (int32)transmit;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_read() HR_FAILED "));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_write(HR_FILE_HANDLE f,void *p,uint32 l)
{
	uint32 transmit;
	MMIHR_TraceF(("mrapp. hr_write() fhandle: 0x%x, len: %d, p: 0x%x", f, l, p));
	
	/*保持平台接口行为统一，写入0B返回成功*/
	if(f != (HR_FILE_HANDLE)FFS_INVALID_HANDLE && p != NULL && l >= 0)
	{
		if(MMIFILE_WriteFile(f, p, l, &transmit, NULL) == SFS_ERROR_NONE)
			return (int32)transmit;
	}
	
	MMIHR_Trace(("mrapp. hr_write() HR_FAILED "));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_info(const char* filename)
{
	SFS_HANDLE		sfs_handle = 0;
	SFS_FIND_DATA_T find_data = {0};
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
	
	if(filename != NULL)
	{
		MMIHR_Trace(("mrapp. hr_info() file: %s", filename));

		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

        if(filename[0] == '/' || filename[0] == '\\')
        {			
			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename[1], strlen(&filename[1]));
        }
        else
        {
            ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename, strlen(filename));
        }

			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				sfs_handle = SFS_FindFirstFile((const uint16 *)hr_full_path, &find_data);				
				MMIHR_Trace(("find_data.attr: 0x%x", find_data.attr));
				if(SFS_INVALID_HANDLE != sfs_handle)
				{
					SFS_FindClose(sfs_handle);
					if(SFS_ATTR_DIR & find_data.attr)
					{
						return HR_IS_DIR;
					}
					else
					{
						return HR_IS_FILE;
					}
				}
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_info() HR_IS_INVALID"));	
	return HR_IS_INVALID;	
}
/*****************************************************************************/
int32 hr_seek(HR_FILE_HANDLE f, int32 pos, int method)
{
	uint8 origin;
	uint32 ret;
	MMIHR_TraceF(("mrapp. hr_seek() fhandle: 0x%x, pos: %d, method: 0x%x", f, pos, method));
	
	if(f != (HR_FILE_HANDLE)FFS_INVALID_HANDLE)
	{
		switch(method)
		{
		case HR_SEEK_SET:
			origin = SFS_SEEK_BEGIN;
			break;
		case HR_SEEK_CUR:
			origin = SFS_SEEK_CUR;
			break;
		case HR_SEEK_END:
			origin = SFS_SEEK_END;
			break;
		default:
			MMIHR_TraceM(("hr_seek para error !!"));
			return HR_FAILED;
		}
		ret = MMIFILE_SetFilePointer((MMIFILE_HANDLE)f, pos, origin);
		MMIHR_TraceF(("mrapp. hr_seek() ret: %d", ret));
		if(ret == SFS_ERROR_NONE)//comment. care the returne value define !
		{
			return HR_SUCCESS;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_seek() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_remove(const char* filename)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
	
	if(filename != NULL)
	{
		MMIHR_TraceF(("mrapp. hr_remove() file: %s", filename));

		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

            if(filename[0] == '/' || filename[0] == '\\')
            {			
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename[1], strlen(&filename[1]));
            }
            else
            {
                ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename, strlen(filename));
            }

			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
				
			{
				if(MMIFILE_DeleteFile((const uint16*)hr_full_path, NULL) == SFS_ERROR_NONE)
					return HR_SUCCESS;
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_remove() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
BOOLEAN hr_fs_diff_path(const char* oldname, const char* newname)
{
    int32 oldname_len = 0;
    int32 newname_len = 0;
    int32 comp_result = 0;
    uint16 oldpath_len = MMI_HERO_APP_FILE_NAME_MAX_LEN;
    uint16 newpath_len = MMI_HERO_APP_FILE_NAME_MAX_LEN;
    uint8 oldname_ex[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    uint8 newname_ex[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    uint8 fullpathname_old[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    uint8 fullpathname_new[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    uint8 pathname_old[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    uint8 pathname_new[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};

    if(oldname == NULL || newname == NULL)
    {
        return FALSE;
    }

    if(newname[0] == 0 || (newname[0] == ' ' && strlen(newname) == 1))
    {
        return FALSE;
    }

    SCI_MEMSET(oldname_ex, 0, sizeof(oldname_ex));
    SCI_MEMSET(newname_ex, 0, sizeof(newname_ex));
    SCI_MEMCPY(oldname_ex, oldname, strlen(oldname)); /*lint !e666*/
    SCI_MEMCPY(newname_ex, newname, strlen(newname)); /*lint !e666*/

    MMIHEROAPP_ConvertPath2Pc(oldname_ex, strlen(oldname_ex));
    MMIHEROAPP_ConvertPath2Pc(newname_ex, strlen(newname_ex));

    oldname_len = hr_fs_get_filename(fullpathname_old, oldname_ex);
    newname_len = hr_fs_get_filename(fullpathname_new, newname_ex);

    MMIAPIFMM_SplitFullPathExt(fullpathname_old, oldname_len, pathname_old, &oldpath_len, NULL, NULL, NULL, NULL);
    MMIAPIFMM_SplitFullPathExt(fullpathname_new, newname_len, pathname_new, &newpath_len, NULL, NULL, NULL, NULL);

    comp_result = MMIAPICOM_CompareTwoWstrExt(pathname_old, oldpath_len*2, pathname_new, newpath_len*2, FALSE);

    MMIHR_TraceF(("%d %d, %d, %d, %d", comp_result, oldname_len, newname_len, oldpath_len, newpath_len));
    
    if(0 != comp_result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
int32 hr_rename_ex(const char* oldname, const char* newname)
{
    int32 oldHandle = 0;
    int32 newHandle = 0;
    int32 oldName_len = 0;
    int32 newName_len = 0;
    int32 fResult   = 0;
    uint32 fileSize = 0;
    uint32 fileLeft = 0;
    uint32 fileRead = 0;
    uint32 fileWrite= 0;
    uint32 fileBufLen = 5*1024;
    uint8  *fileBuf = NULL;
    uint16 pathdir_len = MMI_HERO_APP_FILE_NAME_MAX_LEN;
	wchar fullpathname_old[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
	wchar fullpathname_new[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};
    wchar pathdir_new[MMI_HERO_APP_FILE_NAME_MAX_LEN] = {0};

    if(oldname == NULL || newname == NULL)
    {
        return HR_FAILED;
    }
        
    oldName_len = hr_fs_get_filename(fullpathname_old, oldname);
    newName_len = hr_fs_get_filename(fullpathname_new, newname);

    MMIHEROAPP_ConvertPath2Pc(fullpathname_old, oldName_len);
    MMIHEROAPP_ConvertPath2Pc(fullpathname_new, newName_len);

    MMIAPIFMM_SplitFullPathExt(fullpathname_new, newName_len, pathdir_new, &pathdir_len, NULL, NULL, NULL, NULL);
    
	if(pathdir_new[pathdir_len-1] == '\\' || pathdir_new[pathdir_len-1] == '/')
	{
		pathdir_new[pathdir_len-1] = '\0';
		pathdir_len--;
	}
	
    if(!MMIAPIFMM_IsFolderExist(pathdir_new, pathdir_len))
    {
        return HR_FAILED;
    }

    oldHandle = SFS_CreateFile(fullpathname_old, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);
    if(oldHandle == 0)
    {
        return HR_FAILED;
    }

    fResult = SFS_GetFileSize(oldHandle, &fileSize);
    if(fResult == SFS_ERROR_NONE)
    {
        newHandle = SFS_CreateFile(fullpathname_new, SFS_MODE_CREATE_NEW|SFS_MODE_WRITE, NULL, NULL);
        if(newHandle == 0)
        {
            SFS_CloseFile(oldHandle);
            return HR_FAILED;
        }

        fileBuf = (uint8*)SCI_ALLOCA(fileBufLen);

        if(fileBuf == NULL)
        {
            SFS_CloseFile(oldHandle);
            SFS_CloseFile(newHandle);
            return HR_FAILED;
        }

        while(fileLeft < fileSize)
        {
            SCI_MEMSET(fileBuf, 0x00, fileBufLen);
            SFS_ReadFile(oldHandle, (void *)fileBuf, fileBufLen, &fileRead, NULL);
            SFS_WriteFile(newHandle, (void *)fileBuf, fileRead, &fileWrite, NULL);
            if(fileRead != fileWrite)
            {
                SFS_CloseFile(oldHandle);
                SFS_CloseFile(newHandle);
                SFS_DeleteFile(fullpathname_new, NULL);
            }
            fileLeft += fileWrite;
        }

        if(fileBuf != NULL)
        {
            SCI_FREE(fileBuf);
            fileBuf = NULL;
        }

        SFS_CloseFile(oldHandle);
        SFS_CloseFile(newHandle);
        SFS_DeleteFile(fullpathname_old, NULL);
        MMIHR_TraceF(("mrapp. hr_rename_ex() suceess"));
        return HR_SUCCESS;
    }
    else
    {
        SFS_CloseFile(oldHandle);
        return HR_FAILED;
    }
}

/*****************************************************************************/
int32 hr_rename(const char* oldname, const char* newname)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
    char oldname_ex[MMI_HERO_APP_FILE_NAME_MAX_LEN*2] = {0};
    char newname_ex[MMI_HERO_APP_FILE_NAME_MAX_LEN*2] = {0};
	
	if(oldname != NULL && newname != NULL)
	{
		MMIHR_TraceF(("mrapp. hr_rename() oldname: %s, newname: %s", oldname, newname));

        /*modify by zack@20120411*/
        //统一各平台接口行为, 该接口不支持文件夹重命名
        if(hr_info(oldname) != HR_IS_FILE)
        {
            return HR_FAILED;
        }

        //处理不同路径下的文件重命名
        if(hr_fs_diff_path(oldname, newname))
        {
            return hr_rename_ex(oldname, newname);
        }

        SCI_MEMSET(oldname_ex,0,sizeof(oldname_ex));
        SCI_MEMSET(newname_ex,0,sizeof(newname_ex));
        SCI_MEMCPY(oldname_ex, oldname, strlen(oldname)); /*lint !e666*/
        SCI_MEMCPY(newname_ex, newname, strlen(newname)); /*lint !e666*/

        MMIHEROAPP_ConvertPath2Loc(oldname_ex, strlen(oldname_ex));
        MMIHEROAPP_ConvertPath2Loc(newname_ex, strlen(newname_ex));


		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

            if(oldname_ex[0] == '/' || oldname_ex[0] == '\\')
            {	
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&oldname_ex[1], strlen(&oldname_ex[1]));
            }
            else
            {
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)oldname_ex, strlen(oldname_ex));
            }
			
			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

                if(newname_ex[0] == '/' || newname_ex[0] == '\\')
                {	
    				ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&newname_ex[1], strlen((char*)&newname_ex[1]));
                }
                else
                {
                    ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)newname_ex, strlen((char*)newname_ex));
                }
				if(MMIFILE_RenameFile((wchar*)hr_full_path, hr_full_path_len/2, (wchar*)hr_ucs2_name_arr, ucs2_name_len/2))
					return HR_SUCCESS;
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_rename() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_mkDir(const char* name)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
    uint16 path_name_len = MMIFMM_PATHNAME_LEN;
    wchar  path_name[MMIFMM_PATHNAME_LEN+1] = {0};
    char   tmp_filename[MMIFMM_PATHNAME_LEN+1] = {0};
    char   *filename_ex = NULL;
	
	if(name != NULL)
	{
		MMIHR_TraceF(("hrapp. hr_mkDir() name: %s", name));


        //系统保留目录不应该被创建成功
        if((strncmp(name, ".", 1) == 0) || (strncmp(name, "..", 2) == 0) || (strncmp(name, " ", 2) == 0))
        {
            return HR_FAILED;
        }
            
		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

            memset(tmp_filename, 0x00, sizeof(tmp_filename));
            strncpy(tmp_filename, name, sizeof(tmp_filename) - 1);
            filename_ex = hr_str_trim(tmp_filename, " \\/\r\n");

            MMIHR_TraceF(("mrapp. hr_mkDir() filename_ex=%s", filename_ex));

            if(filename_ex[0] == '/' || filename_ex[0] == '\\')
            {				
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename_ex[1], strlen((char*)&filename_ex[1]));
            }
            else
            {
                ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename_ex, strlen((char*)filename_ex));
            }
			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{

                //不应该支持在不存在的目录下创建文件夹，即不应该支持目录的自动创建
                MMIHEROAPP_ConvertPath2Pc((char*)hr_full_path, hr_full_path_len);
                MMIAPIFMM_SplitFullPathExt((const uint16 *)hr_full_path, hr_full_path_len, path_name, &path_name_len, NULL, NULL, NULL, NULL);
    			if(path_name[path_name_len-1] == '\\' || path_name[path_name_len-1] == '/')
    			{
    				path_name[path_name_len-1] = '\0';
    				path_name_len--;
    			}
                MMIHR_TraceF(("dir_len: %d, %d, %d, %d", g_hr_global.dir_len, ucs2_name_len, hr_full_path_len, path_name_len));

                if(MMIAPIFMM_IsFolderExist((const wchar *)path_name, path_name_len))
                {
    				if(MMIFILE_CreateDirectory((const uint16*)hr_full_path) == SFS_ERROR_NONE)
    					return HR_SUCCESS;
                }
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_mkDir() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_rmDir(const char* name)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
	
	if(name != NULL)
	{
		MMIHR_TraceF(("mrapp. hr_rmDir() name: %s", name));
		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

            if(name[0] == '/' || name[0] == '\\')
            {				
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&name[1], strlen((char*)&name[1]));
            }
            else
            {
    			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)name, strlen((char*)name));
            }
			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				if(MMIFILE_DeleteDirectory((const uint16*)hr_full_path) == SFS_ERROR_NONE)
					return HR_SUCCESS;
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_rmDir() HR_FAILED"));
	return HR_FAILED;
}


/*****************************************************************************/
HR_SEARCH_HANDLE hr_findStart(const char* name, char* buffer, uint32 len)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;	
	MMIFILE_FIND_DATA_T  find_data = {0};
	MMIFILE_HANDLE		file_handle = PNULL;
	uint32 ucsNameLen, gbNameLen;
	const uint8 *devName;
	uint16 devNameLen;
	
	if(name != NULL && buffer != NULL && len != 0)
	{
		MMIHR_Trace(("mrapp. hr_findStart() name: %s, len: %d", name, len));
	
		devName = MMIHEROAPP_getCurDevName(&devNameLen);
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));
			if(name[0] == 0 || (name[0] == ' ' && strlen(name) == 1))
			{
				ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)"*", strlen("*"));
			}
			else
			{
                if(name[0] == '/' || name[0] == '\\')
                {
    				strcpy((char*)hr_full_path, &name[1]);
                }
                else
                {
                    strcpy((char*)hr_full_path, name);
                }
				
				/*add by zack@20120508 start*/
					gbNameLen = strlen(hr_full_path);
					if(hr_full_path[gbNameLen-1] == '/' || hr_full_path[gbNameLen-1] == '\\')
					{
						 hr_full_path[gbNameLen-1] = '\0';
					}
                /*add by zack@20120508 end*/
				strcat((char*)hr_full_path, "\\*");
				ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)hr_full_path, strlen((char*)hr_full_path));			
			}
			if( HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				file_handle = MMIFILE_FindFirstFile((const uint16 *)hr_full_path, hr_full_path_len/2, &find_data);
				
				if(SFS_INVALID_HANDLE != file_handle)
				{
					ucsNameLen = MMIHEROAPP_getUcstrLen((uint8 *)find_data.name);
					if(ucsNameLen >= HERO_MMIFILE_FILE_NAME_MAX_LEN_AD)//for rocky file name length limit of filemgr
					{
						if(HERO_MMIFILE_FILE_NAME_MAX_LEN_AD%2 == 0) /*lint !e506 !e774 !e831*/
							ucsNameLen = HERO_MMIFILE_FILE_NAME_MAX_LEN_AD - 2;
						else//奇数-1
							ucsNameLen = HERO_MMIFILE_FILE_NAME_MAX_LEN_AD - 1;
					}
					gbNameLen = GUI_WstrToGB((uint8*)hr_ucs2_name_arr, (uint8 *)find_data.name, ucsNameLen/2); /*lint !e64*/
					MMIHR_Trace(("ucsNameLen: %d, gbNameLen: %d, len: %d", ucsNameLen, gbNameLen, len));
					if(gbNameLen >= len)
					{
						gbNameLen = len - 1;
					}
					SCI_MEMCPY(buffer, hr_ucs2_name_arr, gbNameLen);
					buffer[gbNameLen] = '\0';
					MMIHR_Trace(("buffer: %s", buffer));
					return file_handle;
				}
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_findStart() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_findGetNext(HR_SEARCH_HANDLE search_handle, char* buffer, uint32 len)
{
	MMIFILE_FIND_DATA_T  find_data = {0};
	uint32 ucsNameLen, gbNameLen;
	MMIFILE_ERROR_E ret;
	MMIHR_Trace(("mrapp. hr_findGetNext() fhandle: 0x%x, len: %d", search_handle, len));
	if(search_handle != (HR_SEARCH_HANDLE)FFS_INVALID_HANDLE && buffer != NULL && len != 0)
	{
		ret = MMIFILE_FindNextFile((MMIFILE_HANDLE)search_handle, &find_data);
		MMIHR_Trace(("mrapp. hr_findGetNext() ret: %d", ret));
		if(ret == FFS_ERROR_NONE) /*lint !e58*/
		{
			ucsNameLen = MMIHEROAPP_getUcstrLen((uint8 *)find_data.name);
			if(ucsNameLen >= HERO_MMIFILE_FILE_NAME_MAX_LEN_AD)//for rocky file name length limit of filemgr
			{
				if(HERO_MMIFILE_FILE_NAME_MAX_LEN_AD%2 == 0) /*lint !e506 !e774 !e831*/
					ucsNameLen = HERO_MMIFILE_FILE_NAME_MAX_LEN_AD - 2;
				else//奇数-1
					ucsNameLen = HERO_MMIFILE_FILE_NAME_MAX_LEN_AD - 1;
			}
			gbNameLen = GUI_WstrToGB((uint8*)hr_full_path, (uint8 *)find_data.name, ucsNameLen/2); /*lint !e64*/
			MMIHR_Trace(("ucsNameLen: %d, gbNameLen: %d, len: %d", ucsNameLen, gbNameLen, len));
			if(gbNameLen >= len)
			{
				gbNameLen = len - 1;
			}
			SCI_MEMCPY(buffer, hr_full_path, gbNameLen);
			buffer[gbNameLen] = '\0';
			MMIHR_Trace(("buffer: %s", buffer));
			return HR_SUCCESS;
		}	
	}
	
	MMIHR_Trace(("mrapp. hr_findGetNext() HR_FAILED"));
	return HR_FAILED;
}

/*****************************************************************************/
int32 hr_findStop(HR_SEARCH_HANDLE search_handle)
{
	MMIHR_TraceF(("mrapp. hr_findStop() fhandle: 0x%x", search_handle));
	
	if(search_handle != (HR_SEARCH_HANDLE)FFS_INVALID_HANDLE)
	{
		if(MMIFILE_FindClose((MMIFILE_HANDLE)search_handle) == SFS_ERROR_NONE)
			return HR_SUCCESS;
	}
	
	return HR_FAILED;
}

int32 hr_ferrno(void)
{
	return 0;
}

int32 hr_fs_get_filename(char *fullpath, const char *filename) 
{
	const uint8 *devName;
	uint16 devNameLen;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;

	devName = MMIHEROAPP_getCurDevName(&devNameLen);

    if(filename[0] == '/' || filename[0] == '\\')
    {
    	ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename[1], strlen(&filename[1]));
    }
    else
    {
        ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename, strlen(filename));
    }

	HERO_MMIFILE_CombineFullPath_fix(
		(const uint8*)devName, devNameLen,
		(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
		(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
		(uint8*)fullpath,
		&hr_full_path_len
		);

	return hr_full_path_len;
}

void hr_printf(const char *format,...)
{
    char *ptr;
    va_list params;
    int32 fHandle = 0;
    int32 ret = 0;
    int32 writen = 0;

#ifdef HERO_APP_LOGOPEN
#ifdef HR_APP_LOG_FILE    
    {
    	memset(s_hr_printfbuf, 0, sizeof(s_hr_printfbuf));
    	strcpy(s_hr_printfbuf, "\n* MR log: ");
    	ptr = s_hr_printfbuf + sizeof("\n* MR log: ") - 1;
    	va_start(params, format);
#ifndef WIN32
    	vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("\n* MR log: ") -1, format, params); 
#else
    	//vsprintf(ptr, format, params); 
    	_vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("\n* MR log: ") -1, format, params); 
#endif
    	fHandle = SFS_CreateFile(L"E:\\heroEngineSpr\\spr.txt", SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, PNULL, PNULL);
    	if(fHandle != 0)
        {
            SFS_SetFilePointer(fHandle, 0, SFS_SEEK_END);
            SFS_WriteFile(fHandle, (void*)s_hr_printfbuf, sizeof(s_hr_printfbuf), &writen, PNULL);
            SFS_CloseFile(fHandle);
        }   
    	va_end(params);	
    }
#else
    {
    	memset(s_hr_printfbuf, 0, sizeof(s_hr_printfbuf));
    	strcpy(s_hr_printfbuf, "* MR log: ");
    	ptr = s_hr_printfbuf + sizeof("* MR log: ") - 1;
    	va_start(params, format);
#ifndef WIN32
    	vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("* MR log: ") -1, format, params); 
#else
    	//vsprintf(ptr, format, params); 
    	_vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("* MR log: ") -1, format, params); 
#endif
    	MMIHR_TraceApp(((char*)s_hr_printfbuf));
    	
    	va_end(params);	
    }
#endif
#endif
}

void hr_video_printf(const char *format,...)
{
    char *ptr;
    va_list params;
    int32 fHandle = 0;
    int32 ret = 0;
    int32 writen = 0;

#if 1 //def HERO_APP_LOGOPEN
#if 1 //def HR_APP_LOG_FILE    
    {
    	memset(s_hr_printfbuf, 0, sizeof(s_hr_printfbuf));
    	strcpy(s_hr_printfbuf, "\n* MR log: ");
    	ptr = s_hr_printfbuf + sizeof("\n* MR log: ") - 1;
    	va_start(params, format);
#ifndef WIN32
    	vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("\n* MR log: ") -1, format, params); 
#else
    	//vsprintf(ptr, format, params); 
    	_vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("\n* MR log: ") -1, format, params); 
#endif
    	fHandle = SFS_CreateFile(L"E:\\heroEngineSpr\\videoSpr.txt", SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, PNULL, PNULL);
    	if(fHandle != 0)
        {
            SFS_SetFilePointer(fHandle, 0, SFS_SEEK_END);
            SFS_WriteFile(fHandle, (void*)s_hr_printfbuf, sizeof(s_hr_printfbuf), &writen, PNULL);
            SFS_CloseFile(fHandle);
        }   
    	va_end(params);	
    }
#else
    {
    	memset(s_hr_printfbuf, 0, sizeof(s_hr_printfbuf));
    	strcpy(s_hr_printfbuf, "* MR log: ");
    	ptr = s_hr_printfbuf + sizeof("* MR log: ") - 1;
    	va_start(params, format);
#ifndef WIN32
    	vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("* MR log: ") -1, format, params); 
#else
    	//vsprintf(ptr, format, params); 
    	_vsnprintf(ptr, sizeof(s_hr_printfbuf) - sizeof("* MR log: ") -1, format, params); 
#endif
    	MMIHR_TraceApp(((char*)s_hr_printfbuf));
    	
    	va_end(params);	
    }
#endif
#endif
}

void hr_video_writebuf(char *buf, int bufsize, int index)
{
    char *ptr;
    va_list params;
    int32 fHandle = 0;
    int32 ret = 0;
    int32 writen = 0;
	char tempBuf[64] = {0};
	char ucstempBuf[128] = {0};

	if(buf == NULL || bufsize == 0)
	{
		return;
	}
	sprintf(tempBuf, "E:\\heroEngineSpr\\testdat_%d.dat", index);

	GUI_UTF8ToWstr((wchar*)ucstempBuf, sizeof(ucstempBuf), tempBuf, strlen(tempBuf));
	fHandle = SFS_CreateFile(ucstempBuf, SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, PNULL, PNULL);
	if(fHandle != 0)
    {
        SFS_SetFilePointer(fHandle, 0, SFS_SEEK_BEGIN);
        SFS_WriteFile(fHandle, (void*)buf, bufsize, &writen, PNULL);
        SFS_CloseFile(fHandle);
    }   
}

int32 hr_mem_get (char** mem_base, uint32* mem_len)
{
#ifdef __HERO_CFG_VMMEM_CUSTOM__
    uint8 *buf_ptr_1 = PNULL;
	int32 mem_size = __HERO_ENGINE_CFG_VAR_VMMEM_SIZE__;

	MMIHR_Trace(("hr_mem_get: mem_len=%d mem_size=%d", *mem_len, mem_size));

	if(*mem_len > 0 && *mem_len <= __HERO_CFG_VAR_VMMEM_MAX_SIZE__) 
	{
	mem_size = *mem_len;
	}
    else if(*mem_len > __HERO_CFG_VAR_VMMEM_MAX_SIZE__)
	{
	mem_size = __HERO_CFG_VAR_VMMEM_MAX_SIZE__;
	}
#ifdef UI_MULTILAYER_SUPPORT
	UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
#endif	
	buf_ptr_1 = SCI_ALLOCA(mem_size);
#ifdef UI_MULTILAYER_SUPPORT
	UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
#endif	
	if(buf_ptr_1 != NULL)
    {
        *mem_base = (char*)buf_ptr_1;
		*mem_len = mem_size;
        memset(*mem_base, 0, *mem_len);
		MMIHR_Trace(("mrapp. hr_mem_get(). Start addr: 0x%x, size: 0x%x, %d", *mem_base, *mem_len, g_hr_curApp));
		
		/*保证内存不要少于600k底线(240*320屏幕)*/
		SCI_ASSERT(*mem_len >= 600*1024);
		return HR_SUCCESS;
	}	
	
	MMIHR_Trace(("mrapp. hr_mem_get(). failed! "));
	*mem_base = NULL;
	*mem_len = 0;
	return HR_FAILED;
#else
#ifdef HR_SHARE_BLOCK_MEM
	uint8 *buf_ptr_1 = PNULL;
	BLOCK_MEM_ID_E mem_id; 
    
	mem_id = BLOCK_MEM_POOL_HERO_ENGINE; /*lint !e64*/

    MMIHR_Trace(("mrapp. hr_mem_get(). mem_id: %d, size: 0x%x, left: 0x%08x, mem_len: %d", mem_id, BL_GetSize(mem_id), SCI_GetSystemSpaceMaxAvalidSpace(), *mem_len));
#ifdef UI_MULTILAYER_SUPPORT
	UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
#endif
	buf_ptr_1 = BL_Malloc(mem_id);
#ifdef UI_MULTILAYER_SUPPORT
	UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
#endif    
	if(buf_ptr_1 != NULL)
    {
		*mem_base = (char*)buf_ptr_1;
		*mem_len = BL_GetSize(mem_id);
		MMIHR_Trace(("mrapp. hr_mem_get(). Start addr: 0x%x, size: 0x%x, left: 0x%08x, %d", *mem_base, *mem_len, SCI_GetSystemSpaceMaxAvalidSpace(), g_hr_curApp));
		
		/*保证内存不要少于600k底线(240*320屏幕)*/
		SCI_ASSERT(*mem_len >= 600*1024);
			return HR_SUCCESS;
    }	

	MMIHR_Trace(("mrapp. hr_mem_get(). failed! "));
	*mem_base = NULL;
	*mem_len = 0;
	return HR_FAILED;
#else//使用直接划分的mem buf
	*mem_base = (char*)s_hr_run_mem;
	*mem_len = (uint32)HERO_APP_MEM_SIZE;// 4byte对齐
	MMIHR_Trace(("mrapp. hr_mem_get(). Start addr: 0x%x, size: 0x%x", *mem_base, *mem_len));
	return HR_SUCCESS;
#endif	
#endif	
}

int32 hr_mem_free (char* mem, uint32 mem_len)
{
	MMIHR_Trace(("mrapp. hr_mem_free(), len: %d", mem_len));
#ifdef __HERO_CFG_VMMEM_CUSTOM__
	if(mem != NULL)
	{
	    SCI_FREE(mem);
	}
#else
#ifdef HR_SHARE_BLOCK_MEM
	BL_Free(BLOCK_MEM_POOL_HERO_ENGINE); /*lint !e64*/
#else
	//do nothing
#endif
#endif
	return HR_SUCCESS;
}

int32 hr_exit(void)
{
	MMIHR_Trace(("mrapp. hr_exit()"));
	
	MMIHEROAPP_setAllReady(FALSE);
#ifdef HR_BINDTO_VIRTUAL_WIN
	if(g_hr_vmBackRun != HERO_APP_MAX)
	{
		MMIHR_Trace(("mrapp. exit bk, %d", g_hr_vmBackRun));
		MMK_PostMsg(VIRTUAL_WIN_ID, HERO_APP_MSG_SYS_EXIT, NULL, 0);
		MMI_TriggerMMITask();
	}
	else
#endif
		if(MMK_IsOpenWin(MMI_HERO_APP_BASE_WIN_ID))
		{
			MMK_PostMsg(MMI_HERO_APP_BASE_WIN_ID, HERO_APP_MSG_SYS_EXIT, NULL, 0);
			MMI_TriggerMMITask();
		}
		
		MMIHR_Trace(("mrapp. hr_exit() END"));
		return HR_SUCCESS;
}

void hr_vm_sleep(int time)
{
}

int32 hr_cacheSync(void* addr, int32 len)
{
    MMIHR_Trace(("mrapp. hr_cacheSync() start"));
#ifndef WIN32
	MMU_IcacheSyncAll();
	//#if defined(PLATFORM_UIX8910)
	MMU_DcacheSyncAll();
	//#endif
#endif
    MMIHR_Trace(("mrapp. hr_cacheSync() end"));

    return HR_SUCCESS;
}

uint32 hr_getTime(void)
{
	MMIHR_TraceT(("mrapp. hr_getTime()"));
	return SCI_GetTickCount();	
}

int32 hr_getDatetime(hr_datetime* datetime)
{
	SCI_DATE_T  date;
	SCI_TIME_T  time;
	MMIHR_Trace(("mrapp. hr_getDatetime()"));
	
	if(datetime != NULL)
	{
		if(TM_GetSysDate(&date) == ERR_TM_NONE
			&& TM_GetSysTime(&time) == ERR_TM_NONE
			)
		{
			datetime->year = date.year;
			datetime->month = date.mon;
			datetime->day = date.mday;
			datetime->hour = time.hour;
			datetime->minute = time.min;
			datetime->second = time.sec;
			
			return HR_SUCCESS;
		}
	}
	
	return HR_FAILED;
}

int32 hr_sleep(uint32 ms)
{
	MMIHR_Trace(("mrapp. hr_sleep() ms: %d", ms));
	
	SCI_Sleep(ms);
	
	return HR_SUCCESS;
}

int32 hr_sendSms(char* pNumber, char*pContent, int32 flags)
{
	int32 resValue = HR_FAILED;
	BOOLEAN result = FALSE;
	BOOLEAN repFlag = FALSE;
	MMISMS_SEND_DATA_T send_data_ptr = {0};
	uint8 numLen = 0;
	MMI_STRING_T msg = {0};
	char *temp = NULL;
	int32 lenth = 0;
	int32 i =0;
	MMIHR_Trace(("mrapp. hr_sendSms() pNumber: %s, len: %d, ucs2?: 0x%x", pNumber, strlen(pNumber), flags)); /*lint !e831*/
	
	if(pNumber == NULL  /*lint !e831*/
		|| pContent == NULL /*lint !e831*/
		|| (numLen = strlen(pNumber)) == 0) /*lint !e774*/
	{
		return HR_IGNORE;
	}
	
	if((int)(flags&0x00000007) == HR_ENCODE_ASCII)
	{
		msg.wstr_len = (uint16)strlen(pContent);
		msg.wstr_ptr = (wchar*)SCI_ALLOCA((msg.wstr_len + 1) * sizeof(wchar)); 
		memset(msg.wstr_ptr, 0, (msg.wstr_len + 1) * sizeof(wchar));
		MMI_STRNTOWSTR((wchar*)msg.wstr_ptr, msg.wstr_len, (uint8*)pContent, msg.wstr_len, msg.wstr_len); /*lint !e64*/
	}
	else
	{
#ifndef WIN32
		lenth = hr_str_wstrlen(pContent) + 2;
		temp = (char*)SCI_ALLOCA(lenth);
		memset(temp, 0, lenth);
		SCI_MEMCPY(temp, pContent, lenth);
		hr_str_convert_endian((char*)temp);
#endif	
		msg.wstr_len = (uint16)MMIHEROAPP_getUcstrLen((uint8*)temp)/2;
		msg.wstr_ptr = (wchar*)SCI_ALLOCA((msg.wstr_len + 1) * sizeof(wchar));
		memset(msg.wstr_ptr, 0, (msg.wstr_len + 1) * sizeof(wchar));
		MMIAPICOM_Wstrncpy(msg.wstr_ptr, (const wchar*)temp, msg.wstr_len);
	}
	
	if((int)(flags&0x00000008))
	{
		repFlag = MMISMS_GetStatusReportFlag();
		if(repFlag)
		{
			MMISMS_SetStatusReportFlag(FALSE);
		}
	}

	if((int)(flags&0x00000010))
	{
		MMISMS_ClearOperInfo();
	}

	MMIHR_Trace(("mrapp. hr_sendSms() msg.wstr_len: %d", msg.wstr_len));
	if(0 != msg.wstr_len)
	{
		numLen = MIN(numLen, MMISMS_NUMBER_MAX_LEN);
		send_data_ptr.is_need_packet = TRUE;
		send_data_ptr.dual_sys = MMIHEROAPP_GetActiveSim();
		send_data_ptr.src_port = 0;
		send_data_ptr.dst_port = 0;
		send_data_ptr.sms_content_ptr = &msg;
		send_data_ptr.dest_addr_ptr = (const uint8*)pNumber;
		send_data_ptr.dest_addr_len = numLen;
		send_data_ptr.send_callback = MMIHEROAPP_SMSCnf;

		result = MMIAPISMS_SendSmsForOther(&send_data_ptr);
		
		if(result)
		{
			resValue= HR_SUCCESS;
		}

		MMIHR_Trace(("mrapp. hr_sendSms() resValue: %d", result));
	}

	if(NULL != temp)
	{
		SCI_FREE(temp);
	}

	if(NULL != msg.wstr_ptr)
	{
		SCI_FREE(msg.wstr_ptr);
	}
	
	return resValue;
}

//modify 20181114 begin
static char gCallNumber[64] = {0};
void hr_call_timer_callback()
{
	uint8 len;
	MN_DUAL_SYS_E dual_sys = MMIHEROAPP_GetActiveSim();

	MMIHERO_StopCallTimer();

	len = strlen(gCallNumber);
	if(len > CC_MAX_TELE_NUM_LEN)
	{//length limited
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	}
	else
	{
		MMIHR_Trace(("mrapp. hr_call() pNumber: %s", gCallNumber));
#if (HERO_SPR_VERSION >= 0x10A1140)
		MMIAPICC_MakeCall(dual_sys, (uint8*)gCallNumber, len, PNULL, PNULL, MMIAPICC_GetCallSimType(dual_sys), CC_CALL_NORMAL_CALL, PNULL);
#else
		MMIAPICC_MakeCall(dual_sys, (uint8*)gCallNumber, len, PNULL, MMIAPICC_GetCallSimType(dual_sys), CC_CALL_NORMAL_CALL, PNULL);
#endif
	}
}

void hr_call(char * pNumber)
{
	if(pNumber != NULL)
	{
		memset(gCallNumber, 0, sizeof(gCallNumber));
		strncpy(gCallNumber, pNumber, sizeof(gCallNumber));
		MMIHERO_StartCallTimer();
	}
	return;
}
//modify 20181114 end

void hr_connectWAP(char* url)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    if(NULL != url)
    {
        entry_param.type = MMIBROWSER_ACCESS_URL;
        entry_param.dual_sys = MMIHEROAPP_GetActiveSim();
        entry_param.url_ptr = url;
        entry_param.user_agent_ptr = PNULL;
        
        MMIBROWSER_Entry(&entry_param);
    }
#endif
}

int32 hr_getNetworkID(void)
{
	int32 resValue = HR_NET_ID_NONE;
	MN_IMSI_T imsi;
	char temp[10];	
	MMIHR_Trace(("mrapp. hr_getNetworkID()"));
	
	imsi = MNSIM_GetImsiEx(MMIHEROAPP_GetActiveSim());
	MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)temp, 3);
	temp[6] = '\0';
	MMIHR_Trace(("temp: %s", temp));	

#ifdef __HERO_ENGINE_OVERSEA__
	if (MMIAPISET_GetFlyMode())
	{
		resValue= HR_NET_ID_NONE;
	}
    else if(MMIPHONE_IsSimOk(MMIHEROAPP_GetActiveSim()))
    {
        resValue= HR_NET_ID_OTHER;
    }
#else
	if (MMIAPISET_GetFlyMode())
	{
		resValue= HR_NET_ID_NONE; //modify by zack@20100628
	}
	else if(!strcmp(&temp[1], "46000") || !strcmp(&temp[1], "46002") || !strcmp(&temp[1], "46007"))//china-mobile
	{
		resValue = HR_NET_ID_MOBILE;
	}
	else if(!strcmp(&temp[1], "46001") || !strcmp(&temp[1], "46006"))//china-union
	{
		resValue = HR_NET_ID_CN;
	}
	else if(!strcmp(&temp[1], "46003") || !strcmp(&temp[1], "46005"))//cdma
	{
		resValue= HR_NET_ID_CDMA;
	}
	else if(MMIPHONE_IsSimOk(MMIHEROAPP_GetActiveSim()))//other sim
	{
		resValue= HR_NET_ID_OTHER;
	}
#endif

	MMIHR_Trace(("mrapp. hr_getNetworkID() resValue: %d", resValue));
	return resValue;
}

#if 1//defined(HERO_ENGINE_SUPPORT_CLIENT_INFO) && !defined(WIN32)
#include "Version_software_project.h"	//_MOCOR_SW_PROJECT_
void hr_toLower(kal_uint8 *str)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 *ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ptr = str;
    while (*ptr != 0)
    {
        if ((*ptr >= 'A') && (*ptr <= 'Z'))
        {
            *ptr += 'a' - 'A';
        }
        ptr++;
    }
}
#endif /* __HERO_ENGINE_SUPPORT_CLIENT_INFO__ */


int32 hr_getUserInfo(hr_userinfo* info)
{
	int32 resValue = HR_FAILED;
	MN_IMSI_T imsi;
	MN_IMEI_T imei;
	BOOLEAN ret;
	MMIHR_Trace(("mrapp. hr_getUserInfo()"));
	
	if(info != NULL)
	{
		memset(info, 0, sizeof(hr_userinfo));
		imsi = MNSIM_GetImsiEx(MMIHEROAPP_GetActiveSim());
		MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)info->IMSI, 8);
		SCI_MEMCPY(info->IMSI, &info->IMSI[1], 15);
		info->IMSI[15] = 0x00;
		ret = MNNV_GetIMEIEx(MMIHEROAPP_GetActiveSim(), imei);
		if(!ret)
        {
			ret = MNNV_GetIMEIEx(MN_DUAL_SYS_1, imei);
#ifdef MULTI_SIM_SYS_DUAL
			if(!ret)
            {
				ret = MNNV_GetIMEIEx(MN_DUAL_SYS_2, imei);
			}
#endif
		}
		
		if(ret)
		{
			MMIHEROAPP_bcd2dec(imei, (uint8*)info->IMEI, 8);
			SCI_MEMCPY(info->IMEI, &info->IMEI[1], 15);
			info->IMEI[15] = 0x00;
		}
		
		MMIHR_Trace(("IMSI: %s", info->IMSI));
		MMIHR_Trace(("IMEI: %s", info->IMEI));
		SCI_MEMCPY(info->manufactory, HERO_ENGINE_MANUFACTORY, sizeof(info->manufactory)-1);

	#if defined(HERO_ENGINE_SUPPORT_CLIENT_INFO) && !defined(WIN32)
		{
			extern void hr_toLower(kal_uint8 *str);
			U8 length = 0;
			kal_uint8  tmp[8]; 
			memset(tmp, '\0', sizeof(tmp) );
			length = strlen(HERO_ENGINE_HANDSET);
			length = MIN(length,/*7*/MIN((sizeof(info->type)-1),7));
			strncpy(tmp, HERO_ENGINE_HANDSET, length);
			hr_toLower(tmp);
			strncpy(info->type, tmp, MIN(strlen(tmp),(sizeof(info->type)-1)));
			//strncpy(info->type, HERO_ENGINE_HANDSET, 7);
		}
	#else /* HERO_ENGINE_SUPPORT_CLIENT_INFO */		
		SCI_MEMCPY(info->type, HERO_ENGINE_HANDSET, sizeof(info->type)-1);
	#endif /* HERO_ENGINE_SUPPORT_CLIENT_INFO */
		MMIHR_Trace(("manufactory: %s, HANDSET: %s", info->manufactory, info->type));
		info->ver = (HERO_PLAT_VER%1000)*1000000 + (HERO_SUB_VER%100)*10000 + (HERO_PORT_VER%10000);
		MMIHR_Trace(("Ver: %d", info->ver));
		resValue = HR_SUCCESS;
	}
	
	MMIHR_Trace(("mrapp. hr_getUserInfo() resValue: %d", resValue));
	return resValue;
}

int32 hr_initNetwork(HR_INIT_NETWORK_CB cb, const char *mode)
{
	int32 resValue = HR_FAILED;
	int plmnStatus = 0;
	char *apn = (char*)(mode != NULL ? mode : "cmnet"); /*lint !e831*/
	uint8 *user_ptr = ""; /*lint !e64*/
	uint8 *passwd_ptr = ""; /*lint !e64*/
    uint32 dns = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
	MMIHR_Trace(("mrapp. hr_initNetwork() state: %d, apn: %s useNvData: %d", s_hr_netContext.gprsState, apn, gHeroUseNVdataAccount));

#if defined(PLATFORM_UIX8910)
    if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
	return resValue;
#endif
	
#ifndef WIN32
#ifdef __HERO_APP_WIFI_SUPPORT__
	if(s_hr_wifi_mode)
	{
		return HR_SUCCESS;
	}
#endif

	if(cb == NULL)
		return resValue;

	MMIHEROAPP_freeAllSnode();

//	if(!MMIAPIPDP_PsIsPdpLinkExist())
//		return resValue;

	plmnStatus = MMIAPIPHONE_GetPlmnStatus(MMIHEROAPP_GetActiveSim());
	MMIHR_Trace(("mrapp. plmnStatus= %d", plmnStatus));
	if(MMIAPISET_GetFlyMode() || !MMIPHONE_IsSimOk(MMIHEROAPP_GetActiveSim()) || (PLMN_NO_SERVICE == plmnStatus) || (PLMN_EMERGENCY_ONLY == plmnStatus) || (PLMN_EMERGENCY_SIM_INVALID == plmnStatus))
	{
		return resValue;
	}

#if defined(MRAPP_BIRD_OPEN)//shenjiang.he 20140701
	if(MMIAPICONNECTION_isGPRSSwitchedOff())
		return resValue;
#endif

	s_hr_netContext.mrNetActived = TRUE;
	s_hr_netContext.deactWaitFunc = NULL;
	s_hr_netContext.gprsCb = (void*)cb; /*lint !e611*/
	if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED)
	{
		resValue = HR_SUCCESS;
	}
	else if(s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVEING)
	{
#ifdef __HERO_ENGINE_OVERSEA__
		if(gHeroUseNVdataAccount)
        {
			apn = (char*)gHeroDataAccount.apn;
			user_ptr = gHeroDataAccount.user_name;
			passwd_ptr = gHeroDataAccount.password;
#ifndef WIN32
            dns = (gHeroDataAccount.dns[0]<<24)|(gHeroDataAccount.dns[1]<<16)|(gHeroDataAccount.dns[2]<<8)|(gHeroDataAccount.dns[3]);
            sci_setsparedns(dns, 0);
#endif
    	}
        else if(hr_getNetworkID() == HR_NET_ID_OTHER)
        {//2009-11-10 111,17787 fix 
    		MN_IMSI_T imsi = {0};
            uint8  temp[10];	
    		T_HR_APN *apnAccount = NULL;
    		imsi = MNSIM_GetImsiEx(MMIHEROAPP_GetActiveSim());
	        MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)temp, 4);
	        temp[7] = '\0';
    		apnAccount = hr_getAPN(&temp[1]);
    		if(apnAccount == NULL)
            {
                temp[6] = '\0';
        		apnAccount = hr_getAPN(&temp[1]);
                if(!apnAccount)
    			{
        			s_hr_netContext.gprsCb = NULL;
        			return HR_FAILED;
                }
                MMIHR_Trace(("apn2 plmn:%s apn:%s", &temp[1], apnAccount->apn));
    		}
            MMIHR_Trace(("apn1 plmn:%s apn:%s", &temp[1], apnAccount->apn));
    		apn = apnAccount->apn;
    		user_ptr = (uint8*)apnAccount->userName;
    		passwd_ptr = (uint8*)apnAccount->passWord;			
#ifndef WIN32
            dns = (apnAccount->dns[0]<<24)|(apnAccount->dns[1]<<16)|(apnAccount->dns[2]<<8)|(apnAccount->dns[3]);
            sci_setsparedns(dns, 0);
#endif			
		}
#endif
		s_hr_netContext.subEvn = HERO_APP_NETSUBEVN_WAITDAIL;
		s_hr_netContext.pdp_retry_times = 0;
	}
	else if(s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVED)
	{
		int32 ret;
		MMIHR_Trace(("cur sim: %d", MMIHEROAPP_GetActiveSim()));

#ifdef __HERO_ENGINE_OVERSEA__
		if(gHeroUseNVdataAccount)
        {
			apn = (char*)gHeroDataAccount.apn;
			user_ptr = gHeroDataAccount.user_name;
			passwd_ptr = gHeroDataAccount.password;
#ifndef WIN32
            dns = (gHeroDataAccount.dns[0]<<24)|(gHeroDataAccount.dns[1]<<16)|(gHeroDataAccount.dns[2]<<8)|(gHeroDataAccount.dns[3]);
            sci_setsparedns(dns, 0);
#endif
    	}
        else if(hr_getNetworkID() == HR_NET_ID_OTHER)
        {//2009-11-10 111,17787 fix 
    		MN_IMSI_T imsi = {0};
            uint8  temp[10];	
    		T_HR_APN *apnAccount = NULL;
    		imsi = MNSIM_GetImsiEx(MMIHEROAPP_GetActiveSim());
	         MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)temp, 4);
	        temp[7] = '\0';
    		apnAccount = hr_getAPN(&temp[1]);
    		if(apnAccount == NULL)
            {
                temp[6] = '\0';
        		apnAccount = hr_getAPN(&temp[1]);
                if(!apnAccount)
    			{
        			s_hr_netContext.gprsCb = NULL;
        			return HR_FAILED;
                }
                MMIHR_Trace(("apn2 plmn:%s apn:%s", &temp[1], apnAccount->apn));
    		}
            MMIHR_Trace(("apn1 plmn:%s apn:%s", &temp[1], apnAccount->apn));
    		apn = apnAccount->apn;
    		user_ptr = (uint8*)apnAccount->userName;
    		passwd_ptr = (uint8*)apnAccount->passWord;			
#ifndef WIN32
            dns = (apnAccount->dns[0]<<24)|(apnAccount->dns[1]<<16)|(apnAccount->dns[2]<<8)|(apnAccount->dns[3]);
            sci_setsparedns(dns, 0);
#endif			
		}
#endif

        app_info.app_handler = MMI_MODULE_HEROENGINE;
        app_info.apn_ptr = (char*)apn;
        app_info.user_name_ptr = (char*)user_ptr;
        app_info.psw_ptr = (char*)passwd_ptr;
        app_info.dual_sys = MMIHEROAPP_GetActiveSim();
        app_info.priority = 3;
        app_info.ps_service_rat = MN_UNSPECIFIED;
		app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
#ifdef IPVERSION_SUPPORT_V4_V6
		app_info.ip_type = MMICONNECTION_IP_V4;
#endif
        app_info.handle_msg_callback = MMIHEROAPP_handlePsMsg; /*lint !e64*/
        MNGPRS_ReadStorageExtReqQosEx(MMIHEROAPP_GetActiveSim(),MN_GPRS_STORAGE_ALL,IM_E,&app_info.qos_info);
	#ifdef IPVERSION_SUPPORT_V4_V6
		{
			MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
			MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
			apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(app_info.dual_sys);
			MMIHR_Trace(("MMIAPICONNECTION_GetApnTypeIndex: %d", apn_idx_arr));
			if (apn_idx_arr != NULL)
			{
				linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(app_info.dual_sys, apn_idx_arr->internet_index[app_info.dual_sys].index);
				MMIHR_Trace(("MMIAPICONNECTION_GetLinkSettingItemByIndex: %d", linksetting));
				if(PNULL != linksetting)
				{
					app_info.apn_ptr = (char*)linksetting->apn;
					app_info.user_name_ptr = (char*)linksetting->username;
					app_info.psw_ptr = (char*)linksetting->password;
					app_info.ip_type = linksetting->ip_type;
					MMIHR_Trace(("app_info.apn_ptr: %s, app_info.user_name_ptr = %s, app_info.psw_ptr = %s, app_info.ip_type = %d", linksetting->apn, linksetting->username, linksetting->password, linksetting->ip_type));
				}
			}
		}
	#endif
		s_hr_netContext.pdp_retry_times = 0;
		s_hr_netContext.gprsState = HERO_APP_GPRS_ACTIVING;
        ret = MMIAPIPDP_Active(&app_info);
		MMIHR_Trace(("mrapp apn:%s ret: %d", apn, ret));
		if(!ret){
			s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
		}
	}
	
	if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVING
		|| (s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVEING && s_hr_netContext.subEvn == HERO_APP_NETSUBEVN_WAITDAIL))
	{
		memcpy(s_hr_netContext.apn, apn, sizeof(s_hr_netContext.apn)-1); /*lint !e670 !e831*/
		s_hr_netContext.apn[sizeof(s_hr_netContext.apn)-1] = '\0';
		MMIHEROAPP_gprsProtectTimerStart(MMI_HERO_APP_GPRSPROTECT_TIME_OUT);
		resValue = HR_WAITING;
	} else if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED) {
		resValue = HR_SUCCESS;
	}
#else
	resValue = HR_SUCCESS;
#endif
	
	MMIHR_Trace(("mrapp. hr_initNetwork() resValue: %d", resValue));
	return resValue;
}


int32 hr_closeNetwork(void)
{
	int i = 0;
	for(i = 0; i < HERO_APP_SOCKET_MAX_NUM; i++)
	{
		if(s_hr_sockets[i].s != INVALID_SOCKET)
		{
			int ret = sci_sock_socketclose((long)s_hr_sockets[i].s);
			MMIHR_Trace(("mrapp. hr_closeNetwork() ret:%d ", ret));
		}
		s_hr_sockets[i].s = INVALID_SOCKET;
		s_hr_sockets[i].state = HERO_APP_SOCKET_CLOSED;
	}
	MMIHEROAPP_CloseHrNet();
	return HR_SUCCESS;
}

uint8 hr_getHostTimerId(void)
{
	return s_hr_hostContext.getHostTimer;
}

void hr_gethostTimerDeal(void)
{
	int ret = -1;
	int32 ip = 0;
	uint32 ipExt = 0 ;
	const char *name = NULL;
	struct sci_hostent * hostent = NULL;
	if(s_hr_hostContext.getHostTimer >0)
	MMK_StopTimer(s_hr_hostContext.getHostTimer);
	s_hr_hostContext.getHostTimer = 0;

	if(SCI_STRLEN(s_hr_hostContext.hostBuffer)&&s_hr_hostContext.cb)
	{
		s_hr_hostContext.timerCount ++;

#ifdef __HERO_APP_WIFI_SUPPORT__
	    	if(s_hr_wifi_mode)
	    	{
	            //hostent = sci_gethostbyname_ext((char*)name, MMIAPIWIFI_GetNetId());
	            ret = sci_parse_host_ext((char *)s_hr_hostContext.hostBuffer,&ipExt,0,MMIAPIWIFI_GetNetId());
	        }
	        else
	        {
	    		//hostent = sci_gethostbyname((char*)name);
	    		ret = sci_parse_host_ext((char *)s_hr_hostContext.hostBuffer,&ipExt,0,s_hr_netContext.netid);
	        }
#else
	    	hostent = sci_gethostbyname((char*)name);
			if(hostent != NULL)
			{
				SCI_MEMCPY(&ipExt, hostent->h_addr_list[0], 4);
				ret = 0;
			}
    		//ret = sci_parse_host_ext((char *)s_hr_hostContext.hostBuffer,&ipExt,0,s_hr_netContext.netid);
#endif
		MMIHR_Trace(("mrapp. hr_gethostTimerDeal ret: %d", ret));

		if(ret == 0)
		{
			ip = ipExt;
			ip = ntohl(ip); /*lint !e737*/
			s_hr_hostContext.cb(ip);
			s_hr_hostContext.cb = NULL;
		}
		else if((ret == 1)&&s_hr_hostContext.timerCount <30)
		{
			s_hr_hostContext.getHostTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID,1500 ,FALSE);
		}
		else
		{
			s_hr_hostContext.cb(HR_FAILED);
			s_hr_hostContext.cb = NULL;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_gethostTimerDeal ip: %d", ip));	
}

int32 hr_getHostByName(const char *name, HR_GET_HOST_CB cb)
{
	int32 resValue = HR_FAILED;
	struct sci_hostent * hostent = NULL;
	int32 hostAddr = 0;
	int32 ip = 0;
	uint32 ipExt = 0;
	
#ifndef WIN32
#ifdef __HERO_APP_WIFI_SUPPORT__
	 if(s_hr_wifi_mode)
	 {
	   if(name== NULL)
		 return resValue;
	 }
	 else
#endif
	   if(name == NULL || s_hr_netContext.gprsState != HERO_APP_GPRS_ACTIVED)
		return resValue;
	{
		MMIHR_Trace(("mrapp. hr_getHostByName() in name: %s", name));
		
#ifdef __HERO_APP_WIFI_SUPPORT__
    	if(s_hr_wifi_mode)
    	{
            //hostent = sci_gethostbyname_ext((char*)name, MMIAPIWIFI_GetNetId());
            resValue = sci_parse_host_ext((char *)name,&ipExt,0,MMIAPIWIFI_GetNetId());
        }
        else
        {
    		//hostent = sci_gethostbyname((char*)name);
	#ifdef __USE_SCI_GETHOSTBYMANE__//clark add 6531e 20170329
			hostent = sci_gethostbyname((char*)name);
	#else
			resValue = sci_parse_host_ext((char *)name,&ipExt,0,s_hr_netContext.netid);
	#endif
        }
#else
	#ifdef __USE_SCI_GETHOSTBYMANE__//clark add 6531e 20170329
    	hostent = sci_gethostbyname((char*)name);
    	#else
    	resValue = sci_parse_host_ext((char *)name,&ipExt,0,s_hr_netContext.netid);
    	#endif
#endif

	MMIHR_Trace(("mrapp. hr_getHostByName() resValue: %d", resValue));
	MMIHR_Trace(("mrapp. hr_getHostByName() hostent: %s",hostent));
#ifdef __USE_SCI_GETHOSTBYMANE__///clark add 6531e 20170329
			if(hostent != NULL)
			{
				SCI_MEMCPY(&hostAddr, hostent->h_addr_list[0], 4);
				resValue = ntohl(hostAddr);
			}	
#else

	if(resValue ==0)
	{
		ip = ipExt;
		ip = ntohl(ip); /*lint !e737*/
		MMIHR_Trace(("mrapp. hr_getHostByName() ip: %d", ip));
		return ip;
	}
	else if(resValue ==1&&cb)
	{
		if(s_hr_hostContext.getHostTimer >0)
			MMK_StopTimer(s_hr_hostContext.getHostTimer);
		
		SCI_MEMSET(s_hr_hostContext.hostBuffer,0,sizeof(s_hr_hostContext.hostBuffer));
		strncpy(s_hr_hostContext.hostBuffer,name,sizeof(s_hr_hostContext.hostBuffer)-1);
		s_hr_hostContext.cb = cb;
		s_hr_hostContext.timerCount = 0;
		s_hr_hostContext.getHostTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 1500,FALSE);
		
		MMIHR_Trace(("mrapp. hr_getHostByName() if(resValue ==1&&cb)"));
		return HR_WAITING;
	}
	else
	{
		return HR_FAILED;
	}
#endif

	}
#endif
	MMIHR_Trace(("mrapp. hr_getHostByName() out :0x%08x", resValue)); /*lint !e527*/	
	return resValue;
}
int32 hr_socket(int32 type, int32 protocol)
{
	int socket = INVALID_SOCKET;
	int32 e = 0;
	int32 sNode;
	MMIHR_Trace(("mrapp. hr_socket() type: %d", type));
	
	sNode = MMIHEROAPP_getAfreeSnode();
	if(sNode >= 0)
	{
#ifndef WIN32
#ifdef __HERO_APP_WIFI_SUPPORT__
	if(s_hr_wifi_mode)
	{
    	socket = sci_sock_socket(AF_INET, SOCK_STREAM, 0, MMIAPIWIFI_GetNetId());
	}
    else
    {
		socket = sci_sock_socket(AF_INET, SOCK_STREAM, 0, s_hr_netContext.netid);
    }
#else
    socket = sci_sock_socket(AF_INET, SOCK_STREAM, 0, s_hr_netContext.netid);
#endif

		MMIHR_Trace(("mrapp. socket:%d", socket));
		
		if(socket != INVALID_SOCKET)
		{
			sci_sock_setsockopt(socket, SO_NBIO, NULL);
			s_hr_sockets[sNode].s = socket;
			s_hr_sockets[sNode].state = HERO_APP_SOCKET_OPENED;
		}
		else
		{ 
			e = sci_sock_errno(socket);
			MMIHR_Trace(("mrapp. errorno: %d", e)); 
			return HR_FAILED;
		}
#endif
	}
	MMIHR_Trace(("mrapp. hr_socket() snode: %d", sNode));
	return sNode;
}


int32 hr_connect(int32 sNode, int32 ip, uint16 port, int32 type)
{
	int32 ret = 0;
	int32 error = 0;
	int32 socket = -1;
#ifndef WIN32
	MMIHR_Trace(("mrapp. hr_connect() sNode: %d, ip: 0x%x, port: %d, type: %d", sNode, ip, port, type));

#ifdef __HERO_APP_WIFI_SUPPORT__
	 if(s_hr_wifi_mode)
	 {
	   if(sNode < 0)
		 return HR_FAILED;
	 }
	 else
#endif
	 {
	   if(sNode < 0 || s_hr_netContext.gprsState != HERO_APP_GPRS_ACTIVED)
		return HR_FAILED;
	 }

	{
		if(sNode >= 0)
		{
			socket = s_hr_sockets[sNode].s;
			MMIHR_TraceN(("mrapp. socket:%d", socket));
			
			ret = MMIHEROAPP_Sconnect(socket, ip, port);
			MMIHR_TraceN(("mrapp. ret:%d", ret));
			s_hr_sockets[sNode].ip = ip;
			s_hr_sockets[sNode].port = port;
			if(ret < 0)
			{
				error = sci_sock_errno(socket);
				MMIHR_TraceN(("error0:%d", error));
				if(EINPROGRESS == error)
				{
					if(HR_SOCKET_NONBLOCK == type)
					{
						s_hr_sockets[sNode].state = HERO_APP_SOCKET_CONNECTING;
						return HR_WAITING;
					}
					else
					{
						s_hr_sockets[sNode].state = HERO_APP_SOCKET_CONNECTING;
						return HR_SUCCESS;//allways show apps a SUCCESS result.
					}
				}
			}
			else
			{
				s_hr_sockets[sNode].state = HERO_APP_SOCKET_CONNECTED;
				return HR_SUCCESS;
			}
		}
	}
	error = sci_sock_errno((long)socket);
#endif
	
	MMIHR_Trace(("mrapp. hr_connect() failed: %d", error));
	return HR_FAILED;
}


int32 hr_closeSocket(int32 sNode)
{
	int32 resValue = HR_FAILED;
	int ret = 0;
	int32 socket;
	MMIHR_Trace(("mrapp. hr_closeSocket() sNode: %d", sNode));
	
#ifndef WIN32
	if(sNode >= 0)
	{
		socket = s_hr_sockets[sNode].s;
		MMIHR_Trace(("mrapp. hr_closeSocket() socket: %d", socket));
		
		if(socket != -1)
		{
			ret = sci_sock_socketclose((long)socket);
			MMIHR_Trace(("mrapp. hr_closeSocket() ret:%d ", ret));
			if(ret < 0)
			{
				MMIHR_Trace(("mrapp. error:%d", sci_sock_errno((long)socket)));
				s_hr_sockets[sNode].state = HERO_APP_SOCKET_ERROR;
			}
			else
			{
				s_hr_sockets[sNode].state = HERO_APP_SOCKET_CLOSED;
				s_hr_sockets[sNode].s = INVALID_SOCKET;
				resValue = HR_SUCCESS;
			}
		}
	}
#endif
	
	MMIHR_Trace(("mrapp. hr_closeSocket() resValue: %d", resValue));
	return resValue;
}


int32 hr_recv (int32 sNode, char *buf, int len)
{
	int32 resValue = HR_FAILED;
	int ret, e;
	int32 socket;
	MMIHR_TraceN(("mrapp. hr_recv()  sNode: %d, len:%d", sNode, len));
	
#ifndef WIN32
	if(sNode >= 0 && buf != PNULL && len != 0)
	{
		socket = s_hr_sockets[sNode].s;
		if(socket != -1)
		{
			if(s_hr_sockets[sNode].state == HERO_APP_SOCKET_CONNECTED)
			{
				ret = sci_sock_recv((long)socket, buf, len, 0);
				MMIHR_TraceM(("mrapp. recv ret:%d", ret));
				if(ret > 0)
				{
					MMIHR_TraceN(("mrapp. recved some things !!:%d", ret));
					resValue = ret;
				}
				else if(ret == 0)//对端已关闭连接
				{
					resValue = HR_FAILED;
				}
				else
				{
					e = sci_sock_errno(socket);
					MMIHR_TraceM(("mrapp. errorno:%d", e));
					
					if(e == ECONNRESET || e == EPIPE)//对端已关闭连接或服务器死机后重启
					{
						resValue = HR_FAILED;
					}
					else
					{
						resValue = 0;
					}
				}
			}
			else if(s_hr_sockets[sNode].state == HERO_APP_SOCKET_CONNECTING)
			{
				ret = MMIHEROAPP_Sconnect(socket, s_hr_sockets[sNode].ip, s_hr_sockets[sNode].port);
				MMIHR_TraceN(("ret:%d", ret));
				if(ret < 0)
				{
					e = sci_sock_errno(socket);
					MMIHR_TraceM(("mrapp. errorno:%d", e));
					if(EINPROGRESS == e)
					{
						resValue = 0;
					}
				}
				else
				{
					s_hr_sockets[sNode].state = HERO_APP_SOCKET_CONNECTED;
					resValue = 0;
				}
			}
		}
	}
#endif
	
	MMIHR_TraceN(("mrapp. hr_recv() resValue: %d", resValue));
	return resValue;
}


int32 hr_send(int32 sNode, const char *buf, int len)
{
	int32 resValue = HR_FAILED;
	int32 e = 0;
	int ret = 0;
	int32 socket;
	MMIHR_TraceN(("mrapp. hr_send() sNode: %d, len: %d", sNode, len));
	
#ifndef WIN32
#ifdef __HERO_APP_WIFI_SUPPORT__
	if(sNode >= 0 && buf != PNULL && len != 0)
#else
	if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED && sNode >= 0 && buf != PNULL && len != 0)
#endif
	{
		socket = s_hr_sockets[sNode].s;
		if(socket != -1)
		{
			if(s_hr_sockets[sNode].state == HERO_APP_SOCKET_CONNECTED)
			{
				ret = sci_sock_send((long)socket, (char*)buf, len, 0);
				
				MMIHR_TraceM(("mrapp. send ret: %d", ret));
				if(ret < 0)
				{
					e = sci_sock_errno((int32)socket);
					MMIHR_TraceM(("mrapp. error0: %d", e)); 
					if(e == EWOULDBLOCK)
					{
						return 0;
					}
					else
					{
						resValue = HR_FAILED;
					}
				}
				else
				{
					resValue = ret;
				}
			}
			else if(s_hr_sockets[sNode].state == HERO_APP_SOCKET_CONNECTING)
			{
				ret = MMIHEROAPP_Sconnect(socket, s_hr_sockets[sNode].ip, s_hr_sockets[sNode].port);
				MMIHR_TraceN(("ret:%d", ret));
				if(ret < 0)
				{
					int error;
					error = sci_sock_errno(socket);
					MMIHR_TraceN(("error0:%d", error));
					if(EINPROGRESS == error)
					{
						resValue = 0;
					}
				}
				else
				{
					s_hr_sockets[sNode].state = HERO_APP_SOCKET_CONNECTED;
					resValue = 0;
				}
			}
		}
	}
#endif
	
	MMIHR_TraceN(("mrapp. hr_send() resValue: %d", resValue));
	return resValue;
}


int32 hr_recvfrom(int32 sNode, char *buf, int len, int32 *ip, uint16 *port)
{
	int32 resValue = HR_FAILED;
	int32 socket;
#ifndef WIN32
	int ret, e;
	struct sci_sockaddr to;
	MMIHR_Trace(("hr_recvfrom()"));
	
	if(sNode >= 0 && buf != PNULL && ip != PNULL && port != NULL)
	{
		socket = s_hr_sockets[sNode].s;
		to.family = AF_INET;
		
		ret = sci_sock_recvfrom(socket, (char*)buf, len, 0, &to);
		MMIHR_Trace(("mrapp. recv len:%d", ret));
		if(ret > 0)
		{
			MMIHR_Trace(("mrapp. recved some things !!:%d", ret));
			resValue = ret;
			*port = ntohs(to.port);
			*ip = ntohl(to.ip_addr);
		}
		else if(ret == 0)//对端已关闭连接
		{
			resValue = HR_FAILED;//comment. 
		}
		else
		{
			e = sci_sock_errno(socket);
			MMIHR_Trace(("mrapp. errorno:%d", e));
			
			if(e == ECONNRESET || e == EPIPE)//对端已关闭连接或服务器死机后重启
			{
				resValue = HR_FAILED;
			}
			else
			{
				resValue = 0;
			}
		}
	}
#endif	
	return resValue;
}

int32 hr_sendto(int32 sNode, const char *buf, int len, int32 ip, uint16 port)
{
	int32 resValue = HR_FAILED;
	int32 socket;
#ifndef WIN32
	int ret, e;
	struct sci_sockaddr to;
	MMIHR_Trace(("mrapp.hr_sendto()"));
	
	if(sNode >= 0 && buf != PNULL && len != 0)
	{
		socket = s_hr_sockets[sNode].s;
		to.family = AF_INET;
		to.port = htons(port);
		to.ip_addr = (unsigned long)htonl(ip); /*lint !e737*/
		SCI_MEMSET((void*)to.sa_data, 0, 8*sizeof(char));
		ret = sci_sock_sendto(socket, (char*)buf, len, 0, &to);
		if(ret < 0)
		{
			e = sci_sock_errno((int32)socket);
			MMIHR_Trace(("mrapp. error0: %d", e));	
			if(e == EWOULDBLOCK)//comment. EWOULDBLOCK will ok ??
			{
				return 0;
			}
			else
			{
				resValue = HR_FAILED;
			}
		}
		else
		{
			resValue = ret;
		}
	}
#endif	
	return resValue;
}

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
int32 g_first_menu_id = HR_FAILED;
#endif
int32 hr_menuCreate(const char * title, int16 num)
{
#if 0 //defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	if(g_first_menu_id == HR_FAILED)
	{
		g_first_menu_id = MMIHEROAPP_listCreate(title, num, HR_LIST_NORMAL_ICON);
		return g_first_menu_id;
	}
	else
#endif
		return MMIHEROAPP_listCreate(title, num, HR_LIST_NORMAL_ICON);
}

int32 hr_menuSetItem(int32 menu, const char *text, int32 index)
{
	return MMIHEROAPP_menuSetItem(menu, text, index, 0);
}

int32 hr_menuShow(int32 menu)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T* addData;
	MMIHR_Trace(("mrapp. hr_menuShow() menu: %d", menu));
	
	addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(menu);
	if(addData != NULL)
	{
		MMK_SetAtvCtrl(addData->winId, addData->ctrlId);
		resValue = HR_SUCCESS;
	}
	MMIHR_Trace(("mrapp. hr_menuShow() resValue: %d", resValue));
	return resValue;
}

int32 hr_menuRefresh(int32 menu)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T* addData;
	MMIHR_Trace(("mrapp. hr_menuRefresh() menu: %d", menu));

	addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(menu);
	if(addData != NULL)
	{
        if(MMK_IsFocusWin(addData->winId))
        {   
    		MMK_SetAtvCtrl(addData->winId, addData->ctrlId);
    		if(MMK_SendMsg(addData->ctrlId, MSG_CTL_PAINT, PNULL))
    		{
    			resValue = HR_SUCCESS;
    		}
    	}
	}

	MMIHR_Trace(("mrapp. hr_menuRefresh() resValue: %d", resValue));
	return resValue;
}

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
void hr_initFirstMenuId(void)
{
	g_first_menu_id = HR_FAILED;
}

void hr_freeFirstMenuId(void)
{
#if 0
	if(g_first_menu_id != HR_FAILED)
	{
		MMK_CloseWin((MMI_WIN_ID_T)g_first_menu_id);
		g_first_menu_id = HR_FAILED;
	}
#endif
}
#endif

int32 hr_menuRelease(int32 menu)
{
	int32 i = 0;
	int32 widCount = 0;
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("mrapp. hr_menuRelease() menu: %d", menu));

#if 0 //defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	if(MMIHEROAPP_GetCurApp() == HERO_APP_TTS)
	{
		if(g_first_menu_id == menu)
		{
			return HR_SUCCESS;
		}
	}
	else
	{
		g_first_menu_id = HR_FAILED;
	}
#endif

	for(i = MMI_HERO_APP_BASE_WIN_ID; i < HERO_APP_MAX_WIN_ID; i++)
	{
		if(MMK_IsOpenWin(i))
		{
			widCount++;
		}
	}
	if(widCount == 2)
	{
		if(MMK_PostMsg(MMI_HERO_APP_BASE_WIN_ID, HERO_APP_MSG_ASY_CLOSE_MENU, &menu, sizeof(int32)))
		{
			MMI_TriggerMMITask();
			resValue = HR_SUCCESS;
		}
	}
	else
	{
		if(MMK_IsOpenWin((MMI_WIN_ID_T)menu))
		{
			if(MMK_CloseWin((MMI_WIN_ID_T)menu))
			{
				resValue = HR_SUCCESS;
			}
		}
	}

	MMIHR_Trace(("mrapp. hr_menuRelease() resValue: %d", resValue));
	return resValue;
}

int32 hr_dialogCreate(const char * title, const char * text, int32 type)
{
	int32 resValue = HR_FAILED;
	MMI_STRING_T textInfo;
	MMI_HERO_APP_DIALOG_PARAMETERS_T *dialogNode = NULL;
	char *temp_title = NULL;
	char *temp_text = NULL;
	int32 len_title = 0;
	int32 len_text = 0;
	MMIHR_Trace(("mrapp. hr_dialogCreate() type: %d", type));

#ifndef WIN32
	if(title != NULL)
	{
		len_title = hr_str_wstrlen(title) + 2;
		temp_title = (char*)SCI_ALLOCA(len_title);
    	if(PNULL == temp_title)
    	{
        	return resValue;
    	}
    	SCI_MEMCPY(temp_title, title, len_title);
    	hr_str_convert_endian((char*)temp_title);
	}

	if(text != NULL)
	{
		len_text = hr_str_wstrlen(text) + 2;
		temp_text = (char*)SCI_ALLOCA(len_text);
    	if(PNULL == temp_text)
    	{
       		SCI_FREE(temp_title);
       		return resValue;
    	}
    	SCI_MEMCPY(temp_text, text, len_text);
    	hr_str_convert_endian((char*)temp_text);
	}
#endif
	
	dialogNode = (MMI_HERO_APP_DIALOG_PARAMETERS_T *)SCI_ALLOC(sizeof(MMI_HERO_APP_DIALOG_PARAMETERS_T));
	if(dialogNode != NULL)
	{		
		dialogNode->next = NULL;
		dialogNode->dialog.type = type;
		textInfo.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_text)/2;
		textInfo.wstr_ptr = (uint8*)temp_text; /*lint !e64*/
		
		resValue = MMIHEROAPP_createPromptWin(dialogNode, &textInfo);
		if(resValue < 0)
		{
			SCI_FREE(dialogNode);
			resValue = HR_FAILED;
		}
	}	

	if((temp_title != NULL) && (temp_text != NULL))
	{
		SCI_FREE(temp_title);
		SCI_FREE(temp_text);
	}
	MMIHR_Trace(("mrapp. hr_dialogCreate() resValue: %d", resValue));
	return resValue;
}

int32 hr_dialogRefresh(int32 dialog, const char * title, const char * text, int32 type)
{
	int32 resValue = HR_FAILED;
	MMI_STRING_T   	textInfo;
	MMI_HERO_APP_DIALOG_PARAMETERS_T *addData = (MMI_HERO_APP_DIALOG_PARAMETERS_T*)MMIHEROAPP_GetPromptWinAddDataPtr((MMI_WIN_ID_T)dialog);	
	char *temp_title = NULL;
	char *temp_text = NULL;
	int32 len_title = 0;
	int32 len_text = 0;
	MMIHR_Trace(("mrapp. hr_dialogRefresh() dialog: %d", dialog));

#ifndef WIN32
	if(title != NULL)
	{
		len_title = hr_str_wstrlen(title) + 2;
		temp_title = (char*)SCI_ALLOCA(len_title);
		if(PNULL == temp_title)
		{
		    return resValue;
		}
		SCI_MEMCPY(temp_title, title, len_title);
		hr_str_convert_endian((char*)temp_title);
	}

	if(text != NULL)
	{
		len_text = hr_str_wstrlen(text) + 2;
		temp_text = (char*)SCI_ALLOCA(len_text);
    	if(PNULL == temp_text)
    	{
        	SCI_FREE(temp_title);
        	return resValue;
    	}
    	SCI_MEMCPY(temp_text, text, len_text);
    	hr_str_convert_endian((char*)temp_text);
	}
#endif
	
	if(addData != NULL)
	{
		if(MMK_IsOpenWin(addData->winId))
		{
			textInfo.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_text)/2;
			textInfo.wstr_ptr = (uint8*)temp_text; /*lint !e64*/
			
			if(type != -1)
			{
				addData->dialog.type = type;
			}
			//MMIPUB_SetAlertWinTextByPtrEx(addData->winId, &textInfo, TRUE);
			MMIPUB_SetAlertWinTextByPtr(addData->winId, &textInfo, NULL, TRUE);
			MMK_SendMsg(addData->winId, MSG_FULL_PAINT, PNULL);//刷新全部内容
			resValue = HR_SUCCESS;
		}
	}

	if((temp_title != NULL) && (temp_text != NULL))
	{
		SCI_FREE(temp_title);
		SCI_FREE(temp_text);
	}
	MMIHR_Trace(("mrapp. hr_dialogRefresh() resValue: %d", resValue));
	return resValue;
}


int32 hr_dialogRelease(int32 dialog)
{
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("mrapp. hr_dialogRelease() dialog: %d", dialog));
	
	if(MMK_IsOpenWin((MMI_WIN_ID_T)dialog))
	{
		if(MMK_CloseWin((MMI_WIN_ID_T)dialog))
		{
			resValue = HR_SUCCESS;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_dialogRelease() resValue: %d", resValue));
	return resValue;
}

int32 hr_textCreate(const char * title, const char * text, int32 type)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T *addData = NULL;
	HERO_APP_TEXT_DATA *textPtr;
	MMI_STRING_T textInfo = {0};
	char *temp_title = NULL;
	char *temp_text = NULL;
	int32 len_title = 0;
	int32 len_text = 0;
	MMIHR_Trace(("mrapp. hr_textCreate() type: %d", type));

#ifndef WIN32
	if(title != NULL)
	{
		len_title = hr_str_wstrlen(title) + 2;
		temp_title = (char*)SCI_ALLOCA(len_title);
		if(PNULL == temp_title)
		{
		    return resValue;
		}
		SCI_MEMCPY(temp_title, title, len_title);
		hr_str_convert_endian((char*)temp_title);
	}

	if(text != NULL)
	{
		len_text = hr_str_wstrlen(text) + 2;
		temp_text = (char*)SCI_ALLOCA(len_text);
		if(PNULL == temp_text)
	    {
	        SCI_FREE(temp_title);
	        return resValue;
	    }
		SCI_MEMCPY(temp_text, text, len_text);
		hr_str_convert_endian((char*)temp_text);
	}
#endif

	addData = (MMI_HERO_APP_ADD_PARAMETERS_T *)SCI_ALLOC(sizeof(MMI_HERO_APP_ADD_PARAMETERS_T));
	
	if(addData != NULL)
	{
		addData->dynWinType = HERO_APP_WINTYPE_TEXT;
		textPtr = &addData->u.text;
		textPtr->type = type;
		textPtr->title.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_title)/2;
		if(textPtr->title.wstr_len != 0)
		{
			textPtr->title.wstr_ptr = (wchar*)SCI_ALLOC(textPtr->title.wstr_len*2);
			if(textPtr->title.wstr_ptr != NULL)
			{
				SCI_MEMCPY(textPtr->title.wstr_ptr, temp_title, textPtr->title.wstr_len*2);
			}
			else
			{
				textPtr->title.wstr_len = 0;
			}
		}
		else
		{
			textPtr->title.wstr_ptr = NULL;
		}

        textPtr->textInfo.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_text)/2;
		if(textPtr->textInfo.wstr_len != 0)
		{
			textPtr->textInfo.wstr_ptr = (wchar*)SCI_ALLOC(textPtr->textInfo.wstr_len*2);
			if(textPtr->textInfo.wstr_ptr != NULL)
			{
				SCI_MEMCPY(textPtr->textInfo.wstr_ptr, temp_text, textPtr->textInfo.wstr_len*2);
			}
			else
			{
				textPtr->textInfo.wstr_len = 0;
			}
		}
		else
		{
			textPtr->textInfo.wstr_ptr = NULL;
		}
		
		resValue = MMIHEROAPP_createADynWin(addData, &textInfo);
		if(resValue < 0)
		{
			if(resValue == -2)
			{
				if(textPtr->title.wstr_ptr != NULL)
				{
					SCI_FREE(textPtr->title.wstr_ptr);
                    textPtr->title.wstr_len = 0;
				}

				if(textPtr->textInfo.wstr_ptr != NULL)
				{
					SCI_FREE(textPtr->textInfo.wstr_ptr);
                    textPtr->textInfo.wstr_len = 0;
				}
				
				SCI_FREE(addData);
			}
			resValue = HR_FAILED;
		}
	}

	if((temp_title != NULL) && (temp_text != NULL))
	{
		SCI_FREE(temp_title);
		SCI_FREE(temp_text);
	}
	MMIHR_Trace(("mrapp. hr_textCreate() resValue: %d", resValue));
	return resValue;
}

int32 hr_textRefresh(int32 handle, const char * title, const char * text)
{
	int32 resValue = HR_FAILED;
	MMI_STRING_T   	textInfo, *titlePtr;
	MMI_HERO_APP_ADD_PARAMETERS_T* addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(handle);
	HERO_APP_TEXT_DATA *textPtr;
	uint16 len;
	char *temp_title = NULL;
	char *temp_text = NULL;
	int32 len_title = 0;
	int32 len_text = 0;
	MMIHR_Trace(("mrapp. hr_textRefresh() win: %d", handle));

#ifndef WIN32
	if(title != NULL)
	{
		len_title = hr_str_wstrlen(title) + 2;
		temp_title = (char*)SCI_ALLOCA(len_title);
		if(PNULL == temp_title)
	    {
	        return resValue;
	    }
		SCI_MEMCPY(temp_title, title, len_title);
		hr_str_convert_endian((char*)temp_title);
	}

	if(text != NULL)
	{
		len_text = hr_str_wstrlen(text) + 2;
		temp_text = (char*)SCI_ALLOCA(len_text);
		if(PNULL == temp_text)
	    {
	        SCI_FREE(temp_title);
	        return resValue;
	    }
		SCI_MEMCPY(temp_text, text, len_text);
		hr_str_convert_endian((char*)temp_text);
	}
#endif

	if(addData != NULL)
	{
		textPtr = &addData->u.text;
		titlePtr = &addData->u.text.title;
		len = MMIHEROAPP_getUcstrLen((uint8 *)temp_title)/2;
		if(len > titlePtr->wstr_len)
		{
			SCI_FREE(titlePtr->wstr_ptr);
			titlePtr->wstr_ptr = (wchar*)SCI_ALLOC(len*2);
		}
		titlePtr->wstr_len = len;
		SCI_MEMCPY(titlePtr->wstr_ptr, temp_title, len*2);
		
		textInfo.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_text)/2;
		textInfo.wstr_ptr = (wchar*)temp_text;
		GUITEXT_SetString(addData->ctrlId, textInfo.wstr_ptr, textInfo.wstr_len, TRUE);
		
		if(MMK_SendMsg(addData->winId, MSG_FULL_PAINT, PNULL) == TRUE)
		{
			resValue = HR_SUCCESS;
		}
		
	}

	if((temp_title != NULL) && (temp_text != NULL))
	{
		SCI_FREE(temp_title);
		SCI_FREE(temp_text);
	}
	MMIHR_Trace(("mrapp. hr_textRefresh() resValue: %d", resValue));
	return resValue;
}

int32 hr_textRelease(int32 text)
{	
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("mrapp. hr_textRelease() text: %d", text));
	
	if(MMK_IsOpenWin((MMI_WIN_ID_T)text))
	{
		if(MMK_CloseWin((MMI_WIN_ID_T)text))
		{
			resValue = HR_SUCCESS;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_textRelease() resValue: %d", resValue));
	return resValue;
}

int32 hr_editCreate(const char * title, const char * text, int32 type, int32 max_size)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T *addData = NULL;
	HERO_APP_EDIT_DATA *editPtr;
	MMI_STRING_T textInfo;
	char *temp_title = NULL;
	char *temp_text = NULL;
	int32 len_title = 0;
	int32 len_text = 0;
	MMIHR_Trace(("mrapp. hr_editCreate() type: %d, max_size: %d", type, max_size));

#ifndef WIN32
	if(title != NULL)
	{
		len_title = hr_str_wstrlen(title) + 2;
		temp_title = (char*)SCI_ALLOCA(len_title);
		if(PNULL == temp_title)
    	{
        	return resValue;
    	}
		SCI_MEMCPY(temp_title, title, len_title);
		hr_str_convert_endian((char*)temp_title);
	}

	if(text != NULL)
	{
		len_text = hr_str_wstrlen(text) + 2;
		temp_text = (char*)SCI_ALLOCA(len_text);
		if(PNULL == temp_text)
    	{
        	SCI_FREE(temp_title);
        	return resValue;
    	}
		SCI_MEMCPY(temp_text, text, len_text);
		hr_str_convert_endian((char*)temp_text);
	}
#endif
	
	addData = (MMI_HERO_APP_ADD_PARAMETERS_T *)SCI_ALLOC(sizeof(MMI_HERO_APP_ADD_PARAMETERS_T));
	if(addData != NULL)
	{
		addData->dynWinType = HERO_APP_WINTYPE_EDITOR;
		editPtr = &addData->u.edit;
		textInfo.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_text)/2;
		textInfo.wstr_ptr = (wchar*)temp_text;
		editPtr->textOut.wstr_ptr = NULL;
		editPtr->textOut.wstr_len = 0;
		editPtr->title.wstr_len = MMIHEROAPP_getUcstrLen((uint8 *)temp_title)/2;
		if(editPtr->title.wstr_len != 0)
		{
			editPtr->title.wstr_ptr = (wchar*)SCI_ALLOC(editPtr->title.wstr_len*2);
			if(editPtr->title.wstr_ptr != NULL)
			{
				SCI_MEMCPY(editPtr->title.wstr_ptr, temp_title, editPtr->title.wstr_len*2);
			}
			else
			{
				editPtr->title.wstr_len = 0;
			}
		}
		else
		{
			editPtr->title.wstr_ptr = NULL;
		}
		editPtr->type = type;
		editPtr->maxLen = (uint16)max_size;		
		resValue = MMIHEROAPP_createADynWin(addData, &textInfo);
		if(resValue < 0)
		{
			if(resValue == -2)
			{
				if(editPtr->title.wstr_ptr != NULL)
				{
					SCI_FREE(editPtr->title.wstr_ptr);
				}
				
				SCI_FREE(addData);
			}
			resValue = HR_FAILED;
		}
	}

	if((temp_title != NULL) && (temp_text != NULL))
	{
		SCI_FREE(temp_title);
		SCI_FREE(temp_text);
	}
	MMIHR_Trace(("mrapp. hr_editCreate() resValue: %d", resValue));
	return resValue;
}

const char* hr_editGetText(int32 edit)
{
	MMI_HERO_APP_ADD_PARAMETERS_T* addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(edit);
	MMI_STRING_T string_info;
	HERO_APP_EDIT_DATA *editPtr; /*lint !e831*/
	uint8* ret = NULL;
	int32 lenth = 0;

	MMIHR_Trace(("mrapp. hr_editGetText() edit: %d", edit));
	
	if(addData != NULL)
	{
		editPtr = &addData->u.edit;
		if(editPtr->type == HR_EDIT_NUMERIC)
		{
			GUIEDIT_GetPhoneNumString(addData->ctrlId, &string_info);
		}
		else
		{
			GUIEDIT_GetString(addData->ctrlId, &string_info);
		}
		
		if(editPtr->textOut.wstr_ptr != NULL)
		{
			SCI_FREE(editPtr->textOut.wstr_ptr);
		}

		editPtr->textOut.wstr_len = string_info.wstr_len;
		editPtr->textOut.wstr_ptr = SCI_ALLOC((string_info.wstr_len+1)*2);
		if(editPtr->textOut.wstr_ptr != NULL)
		{
			SCI_MEMCPY(editPtr->textOut.wstr_ptr, string_info.wstr_ptr, string_info.wstr_len*2);
			editPtr->textOut.wstr_ptr[string_info.wstr_len] = 0x0000;
		}	
#ifndef WIN32
		if(string_info.wstr_ptr != NULL)
		{
			hr_str_convert_endian((char*) editPtr->textOut.wstr_ptr);
		}
#endif
		ret = (uint8*)editPtr->textOut.wstr_ptr;
	}
	MMIHR_Trace(("mrapp. hr_editGetText() ucPtr: 0x%x", editPtr->textOut.wstr_ptr)); /*lint !e613 !e644*/
	return (const char*)ret;
}


int32 hr_editRelease(int32 edit)
{
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("mrapp. hr_editRelease() edit: %d", edit));
	
	if(MMK_IsOpenWin((MMI_WIN_ID_T)edit))
	{
		if(MMK_CloseWin((MMI_WIN_ID_T)edit))
		{
			resValue = HR_SUCCESS;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_editRelease() resValue: %d", resValue));
	return resValue;
}

int32 hr_winCreate(void)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T *addData = NULL;
	MMIHR_Trace(("mrapp. hr_winCreate()"));
	
	addData = (MMI_HERO_APP_ADD_PARAMETERS_T *)SCI_ALLOC(sizeof(MMI_HERO_APP_ADD_PARAMETERS_T));
	if(addData != NULL)
	{
		addData->dynWinType = HERO_APP_WINTYPE_APP;		
		resValue = MMIHEROAPP_createADynWin(addData, NULL); /*lint !e831*/
		if(resValue < 0)
		{
			if(resValue == -2)
			{
				SCI_FREE(addData);
			}
			resValue = HR_FAILED;
		}
	}
	
	MMIHR_Trace(("mrapp. hr_winCreate() resValue: %d", resValue));
	return resValue;
}

int32 hr_winRelease(int32 win)
{
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("mrapp. hr_winRelease() win: %d", win));
	
	if(MMK_IsOpenWin((MMI_WIN_ID_T)win))
	{	
		if(MMK_CloseWin((MMI_WIN_ID_T)win))
		{
			resValue = HR_SUCCESS;
		}
	}
	
	return resValue;
} 

int32 hr_str_wstrlen(const char * str)
{
	int32 lenth = 0;
	unsigned char *tmp = (unsigned char*)str;

	if(str == NULL)
	{
		return 0;
	}
	
	while(tmp[0] | tmp[1])
	{
		lenth += 2;
		tmp += 2;
	}
	return lenth;
}

char* hr_str_convert_endian(char * str)
{
	char temp;
	int32 i=0;
	int32 len = 0;

	if(str == NULL)
	{
		return NULL;
	}
	
	len = hr_str_wstrlen(str);
	for(i = 0; i < len; i += 2)
	{
		temp = str[i];
		str[i] = str[i+1];
		str[i+1] = temp;
	}
	return str;
}
char* hr_str_convert_endian_with_size(char * str, int32 size)
{
	char temp;
	int32 i = 0;
	int len = 0;

	if(str == NULL)
	{
		return NULL;
	}

	len = hr_str_wstrlen(str);
	len = (len > size) ? size : len;

	for(i = 0; i < len; i += 2)
	{
		temp = str[i];
		str[i] = str[i+1];
		str[i+1] = temp;
	}
	return str;
}
LOCAL int32 MMIHEROAPP_adaptCpuClk(int32 param)
{
#ifdef HERO_ENGINE_SUPPORT_CLKADAPT
	MMIHR_Trace(("cur: %d", HAL_GetMCUClk()));
	switch(param)
	{  
	case 0:
	default:
		MMIHEROAPP_RestoreARMClk();
		MMIHEROAPP_CLKSetted(FALSE, FREQ_INDEX_APP_BOOT, HAL_GetMCUClk());
		
		break;
	case 1:
	case 2:
	case 3:
		MMIHEROAPP_SetArmClkHigh();
		MMIHEROAPP_CLKSetted(TRUE, FREQ_INDEX_APP_HIGH, HAL_GetMCUClk());
		break;
	}
#endif
	return HR_SUCCESS;
}

#ifdef HERO_APP_SIMUGAME_OPEN
LOCAL int32 MMIHEROAPP_nesActionInd(HR_NES_ACTION action)
{
	int32 resValue = HR_SUCCESS;
	
#ifndef WIN32
	if(action == NES_LOCK_IRAM)//启动	rom
	{
		MMIHEROAPP_pauseMp3();
		MMIAPISET_StopAllRing(TRUE);
		MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);  //停止按键音
#ifdef HERO_APP_NES_USE_IRAM
		*(volatile uint32 *)0x8b00002c |= 0x01;//2009-11-10 111,17787 L add
		s_hrapp_ilock_handle = IRAM_LockArea((const IRAM_AREA *)&(s_hrapp_iram_area), 
												sizeof(s_hrapp_iram_area)/sizeof(IRAM_AREA), 
												IRAM_WAIT_FOR_EVER);
	
		if(IRAM_INVALID_HANDLE != s_hrapp_ilock_handle)//申请IRAM成功
#endif
		{
			MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
			MMIDEFAULT_SetBackLight(TRUE);
			s_hr_envLock_state |= ENVLOCK_IRAM;
		}
#ifdef HERO_APP_NES_USE_IRAM
		else
		{
			MMIHEROAPP_resumeMp3();
			resValue = HR_FAILED;//申请资源失败，通知NES
		}
#endif
	}
	else if(action == NES_UNLOCK_IRAM)
	{
#ifdef HERO_APP_NES_USE_IRAM
		//释放IRAM
		if(IRAM_INVALID_HANDLE != s_hrapp_ilock_handle)
		{
			IRAM_UnlockArea (s_hrapp_ilock_handle);
			s_hrapp_ilock_handle = IRAM_INVALID_HANDLE;
			*(volatile uint32 *)0x8b00002c &= ~(0x01);//2009-11-10 111,17787 L add
		}
#endif
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_StartLcdBackLightTimer();
		MMIHEROAPP_resumeMp3();
		s_hr_envLock_state &= (~ENVLOCK_IRAM);
	}
	else if(action == NES_LOCK_OTHERS)
	{
		/*将短消息等提示暂设为震动*/
		s_hr_env_preVal.msg[0] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, MSG_RING_TYPE);
		s_hr_env_preVal.msg[1] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_2, MSG_RING_TYPE);
		s_hr_env_preVal.voltage[0] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, VOLTAGE_WARN);
		s_hr_env_preVal.voltage[1] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_2, VOLTAGE_WARN);
		s_hr_env_preVal.flip[0] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, FLIP_RING_TYPE);
		s_hr_env_preVal.flip[1] = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_2, FLIP_RING_TYPE);
		MMIENVSET_SetActiveModeOptValue(MSG_RING_TYPE, (uint8)MSG_RING_TYPE_VIBRATE, MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(MSG_RING_TYPE, (uint8)MSG_RING_TYPE_VIBRATE, MN_DUAL_SYS_2);
		MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, (uint8)VOLTAGE_WARN_TYPE_NOTRING, MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, (uint8)VOLTAGE_WARN_TYPE_NOTRING, MN_DUAL_SYS_2);
		MMIENVSET_SetActiveModeOptValue(FLIP_RING_TYPE, (uint8)FLIP_RING_TYPE_NOTRING, MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(FLIP_RING_TYPE, (uint8)FLIP_RING_TYPE_NOTRING, MN_DUAL_SYS_2);
		s_hr_envLock_state |= ENVLOCK_ENV;
	}
	else if(action == NES_UNLOCK_OTHERS)
	{
		/*恢复短消息等原始提示模式*/
		MMIENVSET_SetActiveModeOptValue(MSG_RING_TYPE, (uint8)s_hr_env_preVal.msg[0], MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(MSG_RING_TYPE, (uint8)s_hr_env_preVal.msg[1], MN_DUAL_SYS_2);
		MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, (uint8)s_hr_env_preVal.voltage[0], MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, (uint8)s_hr_env_preVal.voltage[1], MN_DUAL_SYS_2);
		MMIENVSET_SetActiveModeOptValue(FLIP_RING_TYPE, (uint8)s_hr_env_preVal.flip[0], MN_DUAL_SYS_1);
		MMIENVSET_SetActiveModeOptValue(FLIP_RING_TYPE, (uint8)s_hr_env_preVal.flip[1], MN_DUAL_SYS_2);
		s_hr_wallpaperBufNeedRestore = TRUE;
		s_hr_envLock_state &= (~ENVLOCK_ENV);
	}
#endif
	MMIHR_Trace(("mrapp MMIHEROAPP_nesActionInd action = %d, resValue = %d", action, resValue));
	return resValue;
}
#endif

LOCAL int32 MMIHEROAPP_connectStateQuery(int32 param)
{
#ifndef WIN32
	int32 ret = 0, error = 0;
	sci_fd_set writefds;
	int32 socket;
	MMIHR_Trace(("param: %d", param));
	
	if(param >= 0)
	{
		socket = s_hr_sockets[param].s;
		MMIHR_Trace(("MMIHEROAPP_connectStateQuery socket: %d", socket));
		if(socket != INVALID_SOCKET)
		{
			SCI_FD_ZERO (&writefds);
			SCI_FD_SET((long)socket, &writefds);
			
			ret = sci_sock_select(NULL, &writefds, NULL, 0); 
			
			MMIHR_Trace(("sci_sock_select() ret: %d ", ret));
			if(ret > 0)
			{
				if(SCI_FD_ISSET(socket, &writefds))
				{
					MMIHR_Trace(("writefds true "));
	                s_hr_sockets[param].state = HERO_APP_SOCKET_CONNECTED;
					return HR_SUCCESS;
				}
			}
			else if(ret == 0)
			{
				return HR_WAITING;
			}
			else
			{
				error = sci_sock_errno((long)socket);
				MMIHR_Trace(("error: %d", error));
				return HR_FAILED;
			}
		}
		return HR_FAILED;
	}
#endif
	
	return HR_FAILED;
}

LOCAL int32 MMIHEROAPP_setSoundVolume(int32 param)
{
	uint32 vol = param;
	
	//设置音量
	if(vol > HERO_APP_AUDIO_VOL_MAX)
		vol = HERO_APP_AUDIO_VOL_MAX;
	else if(vol < 1)
		vol = 0;
	
	vol = vol*MMI_AUD_VOLUME_MAX/HERO_APP_AUDIO_VOL_MAX;
	
	MMIHR_Trace(("audio.vol: %d", vol));
#ifndef WIN32
	if (0 == vol)
	{
		MMISRVAUD_SetMuteNohandle(TRUE);
	}
	else
	{
		MMISRVAUD_SetMuteNohandle(FALSE);
		MMISRVAUD_SetVolumeNoHandle(vol);
	}
#endif
	s_hr_audio_mng.soundVolume = vol;
	return HR_SUCCESS;
}
#ifdef __HERO_ENGINE_OVERSEA__
LOCAL int32 MMIHEROAPP_setApnAccount(T_HERO_DATA_ACCOUNT *param)
{
	if(param)
    {
		memset(&gHeroDataAccount, param, sizeof(gHeroDataAccount)); /*lint !e64*/
        MMIHEROAPP_startSetApnAccountTimer();
		return HR_WAITING;
	}
	return HR_FAILED;
}

PUBLIC void MMIHEROAPP_setApnAccountRet(void)
{
	if(s_hr_allReady )
	{
        MMIHR_Trace(("mrapp MMIHEROAPP_getApnAccountRet"));
		hr_event(HR_DATA_ACCOUNT_EVENT, HR_DATA_ACCOUNT_OP_SET, HR_SUCCESS);
	}
}

LOCAL int32 MMIHEROAPP_setApnByUser(void)
{
    return MMIHEROAPP_OpenSelectDataAccountWin(); /*lint !e718 !e746*/
}

LOCAL int32 MMIHEROAPP_getApnAccount(void)
{	
	MMIHEROAPP_startGetApnAccountTimer();
	return HR_WAITING;//must asyn return.
}

PUBLIC void MMIHEROAPP_getApnAccountRet(int32 report)
{
	int32 i   = 0;
	int32 ret = HR_SUCCESS;
	MMICONNECTION_SETTING_T *connection_setting_info = NULL;
	
#if(HERO_SPR_VERSION >= 0x10A1140)
	MN_DUAL_SYS_E simsel;
#endif
	connection_setting_info = MMIMRAPP_GetConnetInfo(); /*lint !e718 !e18*/

#if (HERO_SPR_VERSION >= 0x10A1140)
    simsel = MMIHEROAPP_GetActiveSim();

	if(g_hr_curUserApnAccuntId < MMICONNECTION_MAX_SETTING_NUM)
	{
		memset(&gHeroDataAccount,0,sizeof(gHeroDataAccount));
		memcpy(gHeroDataAccount.apn, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].apn, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].apn_len);
		memcpy(gHeroDataAccount.user_name, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].username, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].username_len);
		memcpy(gHeroDataAccount.password, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].password, connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].password_len);
		gHeroDataAccount.authentication_type = connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].auth_type;
		
		for(i =0; i<4; i++)
		{
			gHeroDataAccount.dns[i] = connection_setting_info->setting_detail[simsel][g_hr_curUserApnAccuntId].dns[i];
		}

		ret = HR_SUCCESS;
	}
	else
	{
		ret = HR_FAILED;
	}
#else
	if(g_hr_curUserApnAccuntId < MMICONNECTION_MAX_SETTING_NUM)
	{
		memset(&gHeroDataAccount,0,sizeof(gHeroDataAccount));
		memcpy(gHeroDataAccount.apn, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].apn, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].apn_len);
		memcpy(gHeroDataAccount.user_name, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].username, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].username_len);
		memcpy(gHeroDataAccount.password, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].password, connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].password_len);
		gHeroDataAccount.authentication_type = connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].auth_type;
		
		for(i =0; i<4; i++)
		{
			gHeroDataAccount.dns[i] = connection_setting_info->setting_detail[g_hr_curUserApnAccuntId].dns[i];
		}

		ret = HR_SUCCESS;
	}
	else
	{
		ret = HR_FAILED;
	}
#endif
	MMIHR_Trace(("mrapp MMIHEROAPP_getApnAccountRet g_hr_curUserApnAccuntId:%d apn:%s ret:%d report:%d",
		          g_hr_curUserApnAccuntId, gHeroDataAccount.apn, ret, report));

	if(s_hr_allReady && report)
	{
		hr_event(HR_DATA_ACCOUNT_EVENT, HR_DATA_ACCOUNT_OP_GET, ret);
	}
}
#endif

LOCAL int32 MMIHEROAPP_SetNewSmsAction(uint8 param)
{
    MMIHR_Trace(("mrapp MMIHEROAPP_SetNewSmsAction param = %d", param));

	if(0 == param || 1 == param)
    {
        s_hr_new_sms_action = param;
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

PUBLIC int32 MMIHEROAPP_GetNewSmsAction(void)
{ /*lint !e18*/
    MMIHR_Trace(("mrapp MMIHEROAPP_GetNewSmsAction s_hr_new_sms_action = %d", s_hr_new_sms_action));
	return s_hr_new_sms_action;
}

LOCAL int32 MMIHEROAPP_GetSCNumber(void)
{	
	MMIHEROAPP_StartGetSCNumberTimer();
	return HR_WAITING;//must asyn return.
}

PUBLIC void MMIHEROAPP_GetSCNumberRet(void)
{
    BOOLEAN ret = FALSE;
	uint8 sc_num_len = 0;
    uint8 sc_num[40 + 2] = {0};

    if(!s_hr_allReady)
        return;

    ret = MMISMS_AppGetSCAddr(MMIHEROAPP_GetActiveSim(), sc_num, &sc_num_len, 40 + 2); /*lint !e718 !e18*/

    MMIHR_Trace(("mrapp MMIHEROAPP_GetSCNumberRet ret = %d, sc_num = %s sc_num_len = %d", ret, sc_num, sc_num_len));
    
    if(ret && sc_num_len > 0)
    {
		hr_event(HR_SMS_GET_SC, (int32)sc_num, strlen(sc_num)+1); /*lint !e64*/
    }
    else
    {
		hr_event(HR_SMS_GET_SC, 0, 0);
    }
}

LOCAL int32 MMIHEROAPP_GetTCardStatus(void)
{	
#if (HERO_SPR_VERSION >= 0x10A1140)
#ifdef DUAL_TCARD_SUPPORT
    if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN)
      || TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
#else
    if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#endif
#else
    if(MMIAPISD_GetStatus())
#endif
	{
		if(MMIAPIUDISK_UdiskIsRun())
		{
			return HR_MSDC_NOT_USEFULL;
		}
		else
		{
			return HR_MSDC_OK;
		}
	}
	else
	{
	#if __HR_PHONE_FREE_SPACE__ > 0
		unsigned int totalSize = 0, freeSize = 0;
		if(hero_get_phone_space(&totalSize, &freeSize))
		{
			if(freeSize >= __HR_PHONE_FREE_SPACE__)
			{
				return HR_LARGE_PHONE_FREE_SPACE;
			}
		}
	#endif
		return HR_MSDC_NOT_EXIST;
	}
}

LOCAL int32 MMIHEROAPP_CheckWifiSupport(void)
{	
#ifdef __HERO_APP_WIFI_SUPPORT__
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        return HR_SUCCESS;
    }
    else
    {
       return HR_FAILED;
    }
#else
    return HR_FAILED;
#endif
}

LOCAL int32 MMIHEROAPP_SetWifiSwitch(int32 param)
{	
#ifdef __HERO_APP_WIFI_SUPPORT__
	if(param)
		s_hr_wifi_mode = TRUE;
	else		
		s_hr_wifi_mode = FALSE;
	return HR_SUCCESS;
#else
	return HR_IGNORE;
#endif
}

PUBLIC uint8 MMIHEROAPP_GetWifiMode(void)
{
#ifdef __HERO_APP_WIFI_SUPPORT__
    return s_hr_wifi_mode;
#else
    return 0;
#endif
}

LOCAL int32 MMIHEROAPP_GetFilePos(int32 fHandle)
{	
	int32 pos = 0;
	int32 ret = 0;

    ret = SFS_GetFilePointer(fHandle, MMIFILE_SEEK_BEGIN, &pos);
    MMIHR_Trace(("mrapp MMIHEROAPP_GetFilePos handle = 0x%08x pos = %d ret = %d", fHandle, pos, ret));

    if(ret == SFS_NO_ERROR)
		return (pos+HR_PLAT_VALUE_BASE);
	else
		return HR_FAILED;
}

LOCAL int32 MMIHEROAPP_GetCurScene(void)
{	
    int32 mode = 0;
    int32 cur_mode = MMIAPIENVSET_GetActiveModeId();

    MMIHR_Trace(("mrapp MMIHEROAPP_GetCurScene cur_mode = %d", cur_mode));

    switch(cur_mode)
    {
    case MMIENVSET_STANDARD_MODE:
        mode = HR_SCENE_NORMAL;
        break;
        
    case MMIENVSET_SILENT_MODE:
        mode = HR_SCENE_MUTE;
        break;
#if !defined(ELT_NO_VIBRATOR)
    case MMIENVSET_MEETING_MODE:
        mode = HR_SCENE_MEETING;
        break;
#endif

#if 1//ndef ENVSET_INSIDE_MODE_DEL
    case MMIENVSET_INSIDE_MODE:
        mode = HR_SCENE_INDOOR;
        break;
#endif		
#if !defined(ELT_NO_VIBRATOR)
    case MMIENVSET_NOISY_MODE:
        mode = HR_SCENE_OUTDOOR;
        break;
#endif
    default:
        mode = HR_SCENE_NORMAL;
        break;
    }

    return mode;
}

int32 MMIHEROAPP_GetBuildTime(uint8** output, int32* output_len)
{
    int32 i = 0;
    int32 len  = 0;
    int32 year = 0;
    int32 mon  = 0;
    int32 day  = 0;
    int32 hour = 0;
    int32 min  = 0;
    int32 ret  = HR_FAILED;
    uint8 *build_time = NULL;
    uint8 date[12] = {0};
    uint8 time[12] = {0};

#ifndef WIN32    
    build_time = (uint8*)VERSION_GetInfo((uint32)BUILD_TIME);
#endif
    MMIHR_Trace(("mrapp build_time: %s", build_time));

    if(build_time != NULL)
    {
        while(*build_time != 0x20)
        {
            date[i++] = *build_time++;
        }
        date[i] = '\0';
        build_time++;
        i = 0;
        while(*build_time != '\0')
        {
            time[i++] = *build_time++;
        }
        time[i] = '\0';
        
        if(date[0] >= '0' && date[0] <= '9')
        {
            mon = date[0]-'0';
        }
        
        if(date[1] >= '0' && date[1] <= '9')
        {
            mon = mon*10+date[1]-'0';
            day = date[3]-'0';
            
            if(date[4] >= '0' && date[4] <= '9')
            {
                day = day*10+date[4]-'0';

                year = (date[6]-'0')*1000+(date[7]-'0')*100+(date[8]-'0')*10+(date[9]-'0');
            }
            else
            {
                year = (date[5]-'0')*1000+(date[6]-'0')*100+(date[7]-'0')*10+(date[8]-'0');
            }
        }
        else
        {
            day = date[2]-'0';
            if(date[3] >= '0' && date[3] <= '9')
            {
                day = day*10+date[3]-'0';
                year = (date[5]-'0')*1000+(date[6]-'0')*100+(date[7]-'0')*10+(date[8]-'0');
            }
            else
            {
                year = (date[4]-'0')*1000+(date[5]-'0')*100+(date[6]-'0')*10+(date[7]-'0');
            }
        }

        if(time[0] >= '0' && time[0] <= '9')
        {
            hour = time[0]-'0';
        }

        if(time[1] >= '0' && time[1] <= '9')
        {
            hour = hour*10+time[1]-'0';

            if(time[3] >= '0' && time[3] <= '9')
            {
                min = time[3]-'0';
                if(time[4] >= '0' && time[4] <= '9')
                {
                    min = min*10+time[4]-'0';
                }
            }
        }
        else
        {
            if(time[2] >= '0' && time[2] <= '9')
            {
                min = time[2]-'0';
                if(time[3] >= '0' && time[3] <= '9')
                {
                    min = min*10+time[3]-'0';
                }
            }
        }

        sprintf((char*)g_mr_build_time, "%04d/%02d/%02d %02d:%02d", year, mon, day, hour, min);
		*output = g_mr_build_time;
        *output_len = strlen((char*)g_mr_build_time);
        ret = HR_SUCCESS;
    }

    MMIHR_Trace(("mrapp output: %s output_len:%d", *output, *output_len));

    return ret;
}

int32 MMIHEROAPP_GetFileCreateTime(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
    uint32 create_time = 0;
    uint32 file_size   = 0;
    BOOLEAN  result = FALSE;
    SFS_HANDLE          sfs_handle = 0;
    SFS_FIND_DATA_T     find_data = {0};
    MMI_TM_T tm = {0};
    
    if(input == NULL || input_len == NULL || output == NULL || output_len == NULL)
    {
        return HR_FAILED;
    }

    devName = MMIHEROAPP_getCurDevName(&devNameLen);

	memset(hr_full_path, 0, sizeof(hr_full_path));
	memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));
		
	ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)input, (uint16)strlen(input)); /*lint !e64*/
	if(	HERO_MMIFILE_CombineFullPath_fix(
			(const uint8*)devName, devNameLen,
			(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
			(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
			(uint8*)hr_full_path,
			&hr_full_path_len
			))
    {
        sfs_handle = MMIFILE_FindFirstFile(hr_full_path, hr_full_path_len, &find_data); /*lint !e64*/

        if(SFS_INVALID_HANDLE != sfs_handle)
        {
            hr_datetime *temp = (hr_datetime*)output;
            temp->year = find_data.create_Date.year;
            temp->month = find_data.create_Date.mon;
            temp->day   = find_data.create_Date.mday;
            temp->hour  = find_data.create_time.hour;
            temp->minute= find_data.create_time.min;
            temp->second= find_data.create_time.sec;

            MMIHR_Trace(("mrapp MMIHEROAPP_GetFileCreateTime %d-%d-%d %d:%d:%d", temp->year, temp->month, temp->day, temp->hour, temp->minute, temp->second));
            SFS_FindClose(sfs_handle);
			*output_len = sizeof(hr_datetime);
            return HR_SUCCESS;
        }        
    }   

    return HR_FAILED;
}

int32 MMIHEROAPP_SetAllowTurnOffBacklight(BOOLEAN allow)
{
    MMIHR_Trace(("mrapp MMIHEROAPP_SetAllowTurnOffBacklight allow: %d need_restore: %d", allow, s_hr_need_restore_backlight));
    
    if(allow)
    {
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        if(s_hr_need_restore_backlight == TRUE)
        {
            s_hr_need_restore_backlight = FALSE;
        }
    }
    else
    {
        //是否需要自动解锁，不需要则将下面if语句注掉
		//if(MMK_IsOpenWin(MMIKL_CLOCK_DISP_WIN_ID))
		//{
		//	MMK_CloseWin(MMIKL_CLOCK_DISP_WIN_ID);
		//}
		MMIDEFAULT_SetBackLight(TRUE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE); 

        if(s_hr_need_restore_backlight == FALSE)
        {
            s_hr_need_restore_backlight = TRUE;
        }
    }
    return HR_SUCCESS;
}

#ifdef __HR_RECORD_SUPPORT__
void MMIHEROAPP_HandleRecordResult(MMISRVAUD_REPORT_RESULT_E  result, DPARAM param)
{
	MMIHR_Trace(("mrapp MMIHEROAPP_HandleRecordResult state: %d result: %d", s_hr_record_state, result));

    if(s_hr_record_state == HRAPP_RECORD_STATE_RECORDING)
    {
        if(MMISRVAUD_REPORT_RESULT_SUCESS == result)
        {
			if(!MMIFILE_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, g_hr_global.device_type, MMIRECORD_SRV_MIN_NEED_SPACE, PNULL)) /*lint !e64*/
            {
                hr_event(HR_ERROR_EVENT, HR_ERROR_NO_SPACE, 0);
            }
        }
        else
        {
            hr_event(HR_ERROR_EVENT, HR_ERROR_UNKNOW, 0);
        }
        s_hr_record_state == HRAPP_RECORD_STATE_STOPPED;//MMIRECORD_STATE_RECORD_STOPED;
    }
}

BOOLEAN MMIHEROAPP_HandleRecordCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
	MMISRVAUD_REPORT_T *report_ptr = NULL;
	MMIHR_Trace(("mrapp MMIHEROAPP_HandleRecordResult state: %d handle: %d", s_hr_record_state, handle));

	if(param != NULL && handle > 0)
	{
		report_ptr = (MMISRVAUD_REPORT_T *)param->data;
		MMIHR_Trace(("mrapp MMIHEROAPP_HandleRecordResult report_ptr: 0x%x", report_ptr));

		if(report_ptr != NULL)
		{
			MMIHR_Trace(("mrapp MMIHEROAPP_HandleRecordResult report_ptr->report=%d", report_ptr->report));

			switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                MMISRVAUD_Stop(handle);
                MMISRVMGR_Free(handle);
                MMIHEROAPP_SetRecordHandle(0);
                
                MMIHEROAPP_HandleRecordResult(report_ptr->data1, NULL);
                break;

            default:
                break;
            }
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
int32 MMIHEROAPP_RecordStart(HR_T_VOICE_REC_REQ *req)
{
    int32 resValue = FALSE;
    uint8  *devName = NULL;
	uint16 devNameLen = 0;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
    int32  file_handle = 0;
    uint16 file_path[SFS_MAX_PATH] = {0};
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
	MMISRVMGR_SERVICE_REQ_T req_data = {0};
	MMISRVAUD_RET_E play_res = MMISRVAUD_RET_MAX;
    MMISRVAUD_TYPE_T audio_srv = {0};

    MMIHR_Trace(("mrapp MMIHEROAPP_RecordStart src: %s srcLen: %d srcType: %d format: %d",
        req->src, req->src_len, req->src_type, req->format));

    switch(req->format)
    {
    case HR_SOUND_WAV:
        audio_srv.info.record_file.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
        break;

    case HR_SOUND_AMR:
        audio_srv.info.record_file.fmt = MMISRVAUD_RECORD_FMT_AMR;
        break;

    case HR_SOUND_PCM:
        audio_srv.info.record_file.fmt = MMISRVAUD_RECORD_FMT_PCM;
        break;

    default:
        return HR_IGNORE;
    }

    if(req->src_type != SRC_NAME || req->src == NULL)
    {
        return HR_FAILED;
    }

    hr_fs_get_filename(file_path, req->src);

    file_handle = SFS_CreateFile(file_path, SFS_MODE_SHARE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL);
    MMIHR_Trace(("mrapp MMIHEROAPP_RecordStart file_handle: %d", file_handle));
    if(file_handle == SFS_INVALID_HANDLE)
    {
        return HR_FAILED;
    }

	req_data.notify = MMIHEROAPP_HandleRecordCallBack;
    req_data.pri = MMISRVAUD_PRI_NORMAL;
	
    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
	
	audio_srv.info.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
	audio_srv.info.record_file.file_handle = file_handle;
	audio_srv.info.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;    
    audio_srv.info.record_file.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
	audio_srv.info.record_file.name = file_path;

	audio_handle = MMIHEROAPP_GetRecordHandle();
	if(audio_handle > INVALID_HANDLE)
	{
		MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
		audio_handle = INVALID_HANDLE;
        MMIHEROAPP_SetRecordHandle(INVALID_HANDLE);
	}
	audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req_data, &audio_srv);
    MMIHR_Trace(("mrapp MMIHEROAPP_RecordStart audio_handle: %d", audio_handle));
    if(audio_handle > INVALID_HANDLE)
    {
		MMIHEROAPP_SetRecordHandle(audio_handle);
        play_res = MMISRVAUD_Play(audio_handle, 0);
		MMIHR_Trace(("mrapp MMIHEROAPP_RecordStart play_res: %d", play_res));
		
        s_hr_record_state = HRAPP_RECORD_STATE_RECORDING;//MMIRECORD_STATE_RECORDING;
        resValue = HR_SUCCESS;
    }
    else
    {
		s_hr_record_state = HRAPP_RECORD_STATE_START;//MMIRECORD_STATE_START;
        resValue = HR_FAILED;
    }
	MMIHR_Trace(("mrapp MMIHEROAPP_RecordStart resValue: %d", resValue));

	return resValue;
}

int32 MMIHEROAPP_RecordPause(void)
{
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
    BOOLEAN result = FALSE;

	audio_handle = MMIHEROAPP_GetRecordHandle();
	MMIHR_Trace(("mrapp MMIHEROAPP_RecordPause audio_handle: %d", audio_handle));

	if(audio_handle > INVALID_HANDLE)
	{
    	result = MMISRVAUD_Pause(audio_handle);

    	MMIHR_Trace(("mrapp MMIHEROAPP_RecordPause result: %d", result));

    	if(result)
    	{
       		s_hr_record_state = HRAPP_RECORD_STATE_RECORD_PAUSED;//MMIRECORD_STATE_RECORD_PAUSED;
       		return HR_SUCCESS;
    	}
    	else
    	{
        	return HR_FAILED;
    	}
	}
	return HR_FAILED;
}

int32 MMIHEROAPP_RecordResume(void)
{
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
    BOOLEAN result = FALSE;

	audio_handle = MMIHEROAPP_GetRecordHandle();
	MMIHR_Trace(("mrapp MMIHEROAPP_RecordPause audio_handle: %d", audio_handle));

	if(audio_handle > INVALID_HANDLE)
	{
    	result = MMISRVAUD_Resume(audio_handle);

    	MMIHR_Trace(("mrapp MMIHEROAPP_RecordResume result: %d", result));

    	if(result)
    	{
        	s_hr_record_state = HRAPP_RECORD_STATE_RECORDING;//MMIRECORD_STATE_RECORDING;
        	return HR_SUCCESS;
    	}
    	else
    	{
        	return HR_FAILED;
    	}
	}
	return HR_FAILED;
}
int32 MMIHEROAPP_RecordStop(void)
{
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
    BOOLEAN stop_result  = FALSE;
    BOOLEAN close_result = FALSE;

	audio_handle = MMIHEROAPP_GetRecordHandle();
	MMIHR_Trace(("mrapp MMIHEROAPP_RecordStop audio_handle: %d", audio_handle));

	if(audio_handle > INVALID_HANDLE)
	{
    	MMIRECORD_Destructor();
		//s_hr_record_state = MMIRECORD_STATE_STOPING;
    	stop_result = MMISRVAUD_Stop(audio_handle);
    	close_result = MMISRVMGR_Free(audio_handle);
		audio_handle = INVALID_HANDLE;
		MMIHEROAPP_SetRecordHandle(INVALID_HANDLE);
    	MMIHR_Trace(("mrapp MMIHEROAPP_RecordStop stop_result: %d close_result: %d", stop_result, close_result));

    	if(stop_result && close_result)
    	{
			s_hr_record_state = HRAPP_RECORD_STATE_STOPPED;//MMIRECORD_STATE_RECORD_STOPED;
        	return HR_SUCCESS;
    	}
    	else
    	{
        	return HR_FAILED;
    	}
	}
	return HR_FAILED;
}
#endif

int32 hr_lcd_set_rotation(int32 param)
{
    switch(param)
    {
    case HR_LCD_ROTATE_NORMAL:
        g_hr_screen_angle.lcd_angle = LCD_ANGLE_0;
        g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_0;
        break;
    case HR_LCD_ROTATE_90:
        g_hr_screen_angle.lcd_angle = LCD_ANGLE_270;
        g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_270;
        break;
    case HR_LCD_ROTATE_180:
        g_hr_screen_angle.lcd_angle = LCD_ANGLE_180;
        g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_180;
        break;
    case HR_LCD_ROTATE_270:
        g_hr_screen_angle.lcd_angle = LCD_ANGLE_90;
        g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_90;
        break;
    case HR_LCD_MIRROR:
    case HR_LCD_MIRROR_ROTATE_90:
    case HR_LCD_MIRROR_ROTATE_180:
    case HR_LCD_MIRROR_ROTATE_270:
    default:
        return HR_IGNORE;
    }

    MMK_SetWinSupportAngle(MMI_HERO_APP_BASE_WIN_ID, g_hr_screen_angle.win_angle);
    //MMK_SetScreenAngle(g_hr_screen_angle.cur_angle);
    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID, g_hr_screen_angle.lcd_angle);

    return HR_SUCCESS;
}

enum{
	HR_PLAT_MEM_TYPE_NONE = 0,
	HR_PLAT_MEM_TYPE_MEDIA,
	HR_PLAT_MEM_TYPE_SCRREN,
	HR_PLAT_MEM_TYPE_MAX
};

typedef struct tagPlatMemMallocSt{
	int memType;
	int memSize;
}platMemMallocSt;
int32 hr_plat_mem_malloc(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	platMemMallocSt *inputData = (platMemMallocSt*)input;
	if(input == NULL || sizeof(platMemMallocSt) != input_len || output == NULL)
	{
		return HR_FAILED;
	}
	if(inputData->memType <= HR_PLAT_MEM_TYPE_NONE || inputData->memType >= HR_PLAT_MEM_TYPE_MAX)
	{
		return HR_FAILED;
	}
	*output = (uint8*)SCI_ALLOCA(inputData->memSize);
	return HR_SUCCESS;
}

typedef struct tagPlatMemFreeSt{
	int memType;
	void *memPtr;
}platMemFreeSt;
int32 hr_plat_mem_free(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	platMemFreeSt *inputData = (platMemFreeSt*)input;
	if(input == NULL || sizeof(platMemFreeSt) != input_len )
	{
		return HR_FAILED;
	}
	if(inputData->memType <= HR_PLAT_MEM_TYPE_NONE || inputData->memType >= HR_PLAT_MEM_TYPE_MAX)
	{
		return HR_FAILED;
	}
	SCI_FREE(inputData->memPtr);
	return HR_SUCCESS;
}

// modify 20181105 begin
static char gSmartAuthIdBuf[128] = {0};
int32 hr_get_smart_authId(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
//20200709
#if defined(HERO_APP_SMART_OPEN)
	if(output != NULL && output_len != NULL)
	{
		strncpy(gSmartAuthIdBuf, __HR_SMART_AUTH_ID__, sizeof(gSmartAuthIdBuf));
		*output = (uint8*)gSmartAuthIdBuf;
		*output_len = strlen(gSmartAuthIdBuf);
		return HR_SUCCESS;
	}
#endif 
	return HR_FAILED;
}
// modify 20181105 end

//modify 20181114 begin
typedef struct tagSmartCustomerKeySt{
	int customerKey;
	int isShortPress;
	int reserved;
}smartCustomerKeySt;
static smartCustomerKeySt gSmartCustomerKey = {0};
int32 hr_get_customer_key(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(output != NULL)
	{
		gSmartCustomerKey.customerKey = HR_KEY_0;
		gSmartCustomerKey.isShortPress = 0;
		*output = (uint8*)&gSmartCustomerKey;
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

static char gCustomerHelpBuf[512] = {0};
int32 hr_get_customer_help_str(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(output != NULL)
	{
		MMI_STRING_T textStr = {0};

		MMI_GetLabelTextByLang(TXT_HERO_SMART_HELP, &textStr);
		memset(gCustomerHelpBuf, 0, sizeof(gCustomerHelpBuf));
		MMIAPICOM_Wstrncpy((wchar*)gCustomerHelpBuf, (const wchar*)textStr.wstr_ptr, textStr.wstr_len);
		hr_str_convert_endian(gCustomerHelpBuf);

		*output = gCustomerHelpBuf;
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

static char gAudioPlayListDir[128] = {0};
int32 hr_get_audio_play_list_dir(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(output != NULL && output_len != NULL)
	{
		memset(gAudioPlayListDir, 0, sizeof(gAudioPlayListDir));
		*output_len = GUI_WstrToUTF8((uint8*)gAudioPlayListDir, sizeof(gAudioPlayListDir), (uint16*)MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC));
		strcat(gAudioPlayListDir, "\\");
		*output = (uint8*)gAudioPlayListDir;
		*output_len = strlen(gAudioPlayListDir);
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

#define PLAT_APP_ID_CAMERA  (1)
#define PLAT_APP_ID_PHONEBOOK (2)
#define PLAT_APP_ID_INBOX  (3)
#define PLAT_APP_ID_FILE_MANAGER (4)
#define PLAT_APP_ID_CALCULATOR (5)
#define PLAT_APP_ID_FM (6)
#define PLAT_APP_ID_ALM (7)
#define PLAT_APP_ID_CLOG (8)
#define PLAT_APP_ID_BROWSER  (9)
#define PLAT_APP_ID_CALENDAR   (21) //日历

//modify 20181120 begin
static int32 hr_set_alm(int Hour, int Min)
{
	SCI_DATE_T  date = {0};
	SCI_TIME_T  time = {0};
	uint16 index = 0;

	if(Hour < 0 || Hour > 23 || Min < 0 || Hour > 59)
	{
		return HR_FAILED;
	}

	if(TM_GetSysDate(&date) == ERR_TM_NONE
			&& TM_GetSysTime(&time) == ERR_TM_NONE)
	{
		MMIACC_SMART_EVENT_T event = {0};

		MMIALM_DelOneEvent(EVENT_SCH_0);

		event.event_fast_info.is_valid = TRUE;

		event.event_fast_info.year = date.year;
		event.event_fast_info.mon = date.mon;
		event.event_fast_info.day = date.mday;
		event.event_fast_info.hour = (uint8)Hour;
		event.event_fast_info.minute = (uint8)Min;		

		event.event_fast_info.type = SMART_EVENT_ALARM;
		event.event_fast_info.fre_mode = ALM_MODE_ONCE;

		//
		event.event_content_info.ring_type = ALM_FIX_RING;

		MMIALM_AddOneEvent(&event, &index);
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

static int32 hr_cancle_alm()
{
	MMIALM_DelOneEvent(EVENT_SCH_0);
	return HR_SUCCESS;
}
//modify 20181120 end

int hr_check_app_is_support(int appid)
{
	if(appid != PLAT_APP_ID_PHONEBOOK
	#ifdef  CAMERA_SUPPORT
		&& appid != PLAT_APP_ID_CAMERA
	#endif
		&& appid != PLAT_APP_ID_INBOX
		&& appid != PLAT_APP_ID_FILE_MANAGER
	#ifdef CALCULATOR_SUPPORT
		&& appid != PLAT_APP_ID_CALCULATOR
	#endif
	#ifdef FM_SUPPORT
		&& appid != PLAT_APP_ID_FM
	#endif
	#ifdef CALENDAR_SUPPORT
		&& appid != PLAT_APP_ID_CALENDAR
	#endif
		&& appid != PLAT_APP_ID_ALM
		&& appid != PLAT_APP_ID_CLOG
	)
	{
		return 0;
	}
	return 1;
}

//modify 20181120 begin
#define HR_SMART_REMINDER_MAGIC   (0x65)

static hero_smart_reminder_nv gHeroSmartReminder = {0};

static int hr_read_reminder(hero_smart_reminder_nv *pReminderNv)
{
    MN_RETURN_RESULT_E return_value = 0;

	MMIHR_Trace(("=================hr_read_reminder pReminderNv = %d", pReminderNv));

	if(pReminderNv == NULL)
	{
		return HR_FAILED;
	}
    MMINV_READ(NVRAM_EF_SMART_REMINDER_LID, pReminderNv, return_value);   
	MMIHR_Trace(("=================hr_read_reminder 22222222 pReminderNv = %d, return_value = %d", pReminderNv, return_value));
	if(return_value != MN_RETURN_SUCCESS)
	{
		return HR_FAILED;
	}
	if(pReminderNv != &gHeroSmartReminder)
	{
		memcpy(&gHeroSmartReminder, pReminderNv, sizeof(hero_smart_reminder_nv));
	}
	return HR_SUCCESS;
}

static int hr_write_reminder(hero_smart_reminder_nv *pReminderNv)
{
	MMIHR_Trace(("=================hr_write_reminder 11111 pReminderNv = %d", pReminderNv));
	if(pReminderNv == NULL)
	{
		return HR_FAILED;
	}
	pReminderNv->magic = HR_SMART_REMINDER_MAGIC;
	MMINV_WRITE(NVRAM_EF_SMART_REMINDER_LID, pReminderNv);
	MMIHR_Trace(("=================hr_write_reminder 22222 pReminderNv = %d", pReminderNv));
	if(pReminderNv != &gHeroSmartReminder)
	{
		memcpy(&gHeroSmartReminder, pReminderNv, sizeof(hero_smart_reminder_nv));
	}
	return HR_SUCCESS;
}

int32 hr_get_reminder_nv_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(output != NULL && output_len != NULL)
	{
		int ret = hr_read_reminder(&gHeroSmartReminder);
		if(ret != HR_SUCCESS)
		{
			hero_smart_reminder_nv data = {0};
			hr_write_reminder(&data);
			ret = hr_read_reminder(&gHeroSmartReminder);
		}
		if(ret == HR_SUCCESS)
		{
			*output = (uint8*)&gHeroSmartReminder;
			*output_len = sizeof(hero_smart_reminder_nv);
			return ret;
		}
	}
	return HR_FAILED;
}

int32 hr_set_reminder_nv_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int index = 0;
	hero_smart_reminder_nv tempReqData = {0};
	hero_smart_reminder_nv *reqData = (hero_smart_reminder_nv*)input;

	if(reqData == NULL || input_len != sizeof(hero_smart_reminder_nv))
	{
		return HR_FAILED;
	}

	memcpy(&tempReqData, reqData, sizeof(hero_smart_reminder_nv));
	for(index=0; index<HERO_SMART_REMINDER_COUNT; index++)
	{
		hero_smart_reminder_nv_item *pItem = &(tempReqData.item[index]);
	
		if(pItem->audioPath[1] != ':' && pItem->audioPath[2] != '/')
		{
			wchar fullpathname[128] = {0};
			hr_fs_get_filename((char *)fullpathname, pItem->audioPath);
			GUI_WstrToUTF8((uint8*)pItem->audioPath, sizeof(pItem->audioPath), (const wchar*)fullpathname, MMIAPICOM_Wstrlen((const wchar*)fullpathname));
		}
	}
	return hr_write_reminder(&tempReqData);
}

//hour: 小时
//minute: 分钟
//reminder: 提醒内容（unicode大端编码）
//audioPath: 音频文件目录(相对目录)
//audioType: 音频类型
int hr_set_reminder(unsigned char hour, unsigned char minute, char *reminder, char *audioPath, unsigned char audioType)
{
	int ret = HR_FAILED;
	unsigned char tempAudioType = 0;
	hero_smart_reminder_nv *pReminderNv = NULL;
	do
	{
		int index = 0;
		hero_smart_reminder_nv_item *pItem = NULL;

		if(hour < 0 || hour > 23 || minute < 0 || minute > 59)
		{
			break;
		}

		pReminderNv = (hero_smart_reminder_nv*)SCI_ALLOCA(sizeof(hero_smart_reminder_nv));
		if(pReminderNv == NULL)
		{
			break;
		}
		memset(pReminderNv, 0, sizeof(hero_smart_reminder_nv));
		if(hr_read_reminder(pReminderNv) != HR_SUCCESS)
		{
			memset(pReminderNv, 0, sizeof(hero_smart_reminder_nv));
		}
		if(pReminderNv->magic != HR_SMART_REMINDER_MAGIC)
		{
			memset(pReminderNv, 0, sizeof(hero_smart_reminder_nv));
			pReminderNv->magic = HR_SMART_REMINDER_MAGIC;
			hr_write_reminder(pReminderNv);
		}
		for(index = 0; index < HERO_SMART_REMINDER_COUNT; index++)
		{
			pItem = &(pReminderNv->item[index]);
			if(!pItem->isOpen || (pItem->hour == hour && pItem->minute == minute))
			{
				break;
			}
		}
		if(index == HERO_SMART_REMINDER_COUNT)
		{
			pItem = &(pReminderNv->item[0]);
		}
		MMIHR_Trace(("cccc pItem = %d, sizeof(hero_smart_reminder_nv_item)= %d", pItem, sizeof(hero_smart_reminder_nv_item)));

		memset(pItem, 0, sizeof(hero_smart_reminder_nv_item));

		pItem->isOpen = 1;
		pItem->hour = hour;
		pItem->minute = minute;
		if(reminder != NULL)
		{
			MMIAPICOM_Wstrncpy((uint16*)pItem->reminder, (uint16*)reminder, 31);
			hr_str_convert_endian((char*)pItem->reminder);
		}
		if(audioPath != NULL)
		{
			unsigned short fullpathname[128] = {0};
			hr_fs_get_filename((char *)fullpathname, audioPath);
			GUI_WstrToUTF8((uint8*)pItem->audioPath, sizeof(pItem->audioPath), (const wchar*)fullpathname, MMIAPICOM_Wstrlen((const wchar*)fullpathname));
			pItem->audioType = MMISRVAUD_RING_FMT_AMR;
		}
		ret = hr_write_reminder(pReminderNv);
	}while(0);
	if(pReminderNv != NULL)
	{
		SCI_FREE(pReminderNv);
	}
	return ret;
}

static void hr_show_reminder_popup(unsigned short *reminder)
{
	unsigned short tempBuf[128] = {0};
	MMI_STRING_T reminderText = {0};

	MMIAPICOM_Wstrncpy((uint16*)tempBuf, (uint16*)reminder, 127);

	reminderText.wstr_ptr = tempBuf;
	reminderText.wstr_len = MMIAPICOM_Wstrlen((const wchar *)tempBuf);
	MMIPUB_OpenAlertWinByTextPtr(PNULL,&reminderText,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

static int hr_check_audio_is_file(char *path)
{
	SFS_HANDLE sfs_handle = 0;
	SFS_FIND_DATA_T find_data = {0};
	unsigned short dl_buffer_or_path[128] = {0};

	GUI_UTF8ToWstr((wchar*)dl_buffer_or_path, 128, (const uint8*)path, strlen(path));
	sfs_handle = SFS_FindFirstFile((const uint16 *)dl_buffer_or_path, &find_data);				
	if(SFS_INVALID_HANDLE != sfs_handle)
	{
		SFS_FindClose(sfs_handle);
		if(SFS_ATTR_DIR & find_data.attr)
		{
			return 0;
		}
		return 1;
	}
	return 0;
}

#define REMINDER_AUDIO_PLAY_MAX_COUNT (3)

typedef struct tagReminderAudioDataSt{
	unsigned char playIndex;
	unsigned char audioType;
	MMISRV_HANDLE_T audio_handle;
	unsigned short audioPath[128];
}reminderAudioDataSt;

static reminderAudioDataSt gReminderAudioData = {0};

LOCAL BOOLEAN HRReminder_AudioPlayCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(handle == gReminderAudioData.audio_handle && PNULL != param)
    {
        MMIHR_Trace(("MMIHEROAPP_AudioPlayCallback event = %d", param->event));
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
        	MMISRVAUD_SetVolume(handle, s_hr_audio_mng.soundVolume);
            break;
        case MMISRVMGR_NOTIFY_EXT:
			MMISRVAUD_Stop(handle);
			MMISRVMGR_Free(handle);
			gReminderAudioData.audio_handle = INVALID_HANDLE;
            break;
        default:
            break;
        }
    }
    return TRUE;
}

static int hr_paly_reminder_audio_req()
{
	if(gReminderAudioData.audio_handle > INVALID_HANDLE)
	{
		MMISRVAUD_RET_E play_res = MMISRVAUD_Play(gReminderAudioData.audio_handle, 0);
		MMIHR_Trace(("hr_paly_reminder_audio_req: %d", play_res));
		if(MMISRVAUD_RET_OK != play_res)
		{
			if(gReminderAudioData.audio_handle != INVALID_HANDLE)
			{
				MMISRVMGR_Free(gReminderAudioData.audio_handle);
				gReminderAudioData.audio_handle = INVALID_HANDLE;
			}
		}
	}
}

static void hr_play_reminder_audio(unsigned short *reminder, char *audioPath, kal_uint8 audioType)
{
	if(hr_check_audio_is_file(audioPath))
	{
		int8 soundVolume = s_hr_audio_mng.soundVolume;
		if(gReminderAudioData.audio_handle != INVALID_HANDLE)
		{
			MMISRVAUD_Stop(gReminderAudioData.audio_handle);
			MMISRVMGR_Free(gReminderAudioData.audio_handle);
		}
		memset(&gReminderAudioData, 0, sizeof(reminderAudioDataSt));
		gReminderAudioData.audio_handle = INVALID_HANDLE;
		gReminderAudioData.audioType = audioType;
		GUI_UTF8ToWstr((wchar*)gReminderAudioData.audioPath, 128, (const uint8*)audioPath, strlen(audioPath));

		s_hr_audio_mng.soundVolume = MMI_AUD_VOLUME_MAX;
		s_hr_play_times = REMINDER_AUDIO_PLAY_MAX_COUNT;
		gReminderAudioData.audio_handle = MMIHEROAPP_CreateAudioFileHandle(gReminderAudioData.audioType, (const wchar *)gReminderAudioData.audioPath, HRReminder_AudioPlayCallback);
		if(gReminderAudioData.audio_handle > INVALID_HANDLE)
		{
			hr_paly_reminder_audio_req();
		}
		s_hr_audio_mng.soundVolume = soundVolume;
	}
#ifdef HERO_ENGINE_TTS_SUPPORT
	else if(hr_get_tts_type(0) == HR_PLAT_VALUE_BASE)
	{
		char *reminderBuf = NULL;
		do
		{
			int index = 0;
			int bufLen = 0;
			tihoTtsReqDataSt ttsReqData = {NULL};

			hr_stop_tts_req(NULL, 0, NULL, NULL, NULL);
			bufLen = (MMIAPICOM_Wstrlen((const wchar*)reminder)<<1) * REMINDER_AUDIO_PLAY_MAX_COUNT + 4;

			reminderBuf = SCI_ALLOCA(bufLen);
			if(reminderBuf == NULL)
			{
				break;
			}
			memset(reminderBuf, 0, bufLen);
			for(index = 0; index < REMINDER_AUDIO_PLAY_MAX_COUNT; index++)
			{
				MMIAPICOM_Wstrcat((wchar*)reminderBuf, (const wchar*)reminder);
			}

			hr_str_convert_endian(reminderBuf);
			ttsReqData.textPtr = reminderBuf;
			ttsReqData.volume = 5;
			hr_plat_tts_req((uint8*)&ttsReqData, sizeof(tihoTtsReqDataSt), NULL, NULL, NULL);
		}while(0);
		if(reminderBuf != NULL)
		{
			SCI_FREE(reminderBuf);
		}
	}
#endif
}

static void hr_show_reminder(hero_smart_reminder_nv_item *pItem)
{
	hr_show_reminder_popup(pItem->reminder);
	hr_play_reminder_audio(pItem->reminder, pItem->audioPath, pItem->audioType);
}

void hr_notify_reminder()
{
	SCI_TIME_T  time = {0};
	if(TM_GetSysTime(&time) == ERR_TM_NONE)
	{
		int isUpdate = 0;
		int index = 0;
		hero_smart_reminder_nv_item *pItem = NULL;

		if(gHeroSmartReminder.magic != HR_SMART_REMINDER_MAGIC)
		{
			memset(&gHeroSmartReminder, 0, sizeof(hero_smart_reminder_nv));
			if(hr_read_reminder(&gHeroSmartReminder) != HR_SUCCESS)
			{
				return;
			}
			if(gHeroSmartReminder.magic != HR_SMART_REMINDER_MAGIC)
			{
				return;
			}
		}

		///
		for(index = 0; index < HERO_SMART_REMINDER_COUNT; index++)
		{
			pItem = &gHeroSmartReminder.item[index];
			if(pItem->isOpen)
			{
				if(time.hour == pItem->hour && time.min >= pItem->minute)
				{
					hr_show_reminder(pItem);
					pItem->isOpen = 0;
					isUpdate = 1;
					break;
				}
			}
		}

		///
		if(isUpdate)
		{
			hr_write_reminder(&gHeroSmartReminder);
		}
	}
}
//modify 20181120 end

int hr_run_plat_app(int appId, char *action, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) //modify 20181120 begin
{
#ifdef	CAMERA_SUPPORT
	if(appId == PLAT_APP_ID_CAMERA)
	{
		MMIAPIDC_OpenPhotoWin();
		return 1;
	}
#endif

	if(appId == PLAT_APP_ID_PHONEBOOK)
	{
		MMIAPIPB_OpenListAllGroupWin();
		return 1;
	}

	if(appId == PLAT_APP_ID_INBOX)
	{
		MMISMS_EnterSmsBox( MMISMS_BOX_MT );
		return 1;
	}

	if(appId == PLAT_APP_ID_FILE_MANAGER)
	{
		MMIAPIFMM_OpenExplorer();
		return 1;
	}

#ifdef CALCULATOR_SUPPORT
	if(appId == PLAT_APP_ID_CALCULATOR)
	{
		MMIAPICALC_OpenMainWin();
		return 1;
	}
#endif

#ifdef CALENDAR_SUPPORT
	if(appId == PLAT_APP_ID_CALENDAR)
	{
	#ifdef MMI_SCHEDULE_SUPPORT
		MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
	#else
		MMIAPICALEND_OpenMainWin();
	#endif
		return 1;
	}
#endif

#ifdef FM_SUPPORT
	if(appId == PLAT_APP_ID_FM)
	{	
		MMIAPIFM_OpenMainWin();
	}
#endif

	if(appId == PLAT_APP_ID_ALM && action != NULL)
	{
		if(strcmp(action, "open") == 0)
		{
			MMIAPIALM_OpenMainWin();
			return 1;
		}
		else if(strcmp(action, "setAlm") == 0)
		{
			hr_set_alm(arg1, arg2); //modify 20181120 begin
			return 1;
		}
		else if(strcmp(action, "cancelAlm") == 0)
		{
			hr_cancle_alm(); //modify 20181120 begin
			return 1;
		}
//modify 20181120 begin
		else if(strcmp(action, "setReminder") == 0)
		{
			hr_set_reminder((unsigned char)arg1, (unsigned char)arg2, (char*)arg3, (char*)arg5, arg4);
			return 1;
		}
//modify 20181120 end
	}

	if(appId == PLAT_APP_ID_CLOG)
	{
		MMIAPICL_OpenMainMenuWindow();
		return 1;
	}

	return 0;
}

typedef void (*smartTimerFuncCallback)(void*);
static smartTimerFuncCallback gSmartTimerFuncCallback = NULL;
static void *gSmartTimerUserData = NULL;
void hr_smart_timer_callback()
{
	if(gSmartTimerFuncCallback != NULL)
	{
		gSmartTimerFuncCallback(gSmartTimerUserData);
	}
	gSmartTimerFuncCallback = NULL;
	gSmartTimerUserData = NULL;
}

void hr_smart_start_timer(int delay, smartTimerFuncCallback callback, void *userData)
{
	gSmartTimerFuncCallback = callback;
	gSmartTimerUserData = userData;
	MMIHERO_StartSmartTimer(delay);
}

int32 HEROAPI_GetActiveSim(void)
{
	uint32  i = 0;

	for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
	{
		int plmnStatus = MMIAPIPHONE_GetPlmnStatus(i);
		if (MMIPHONE_IsSimOk(i) && (PLMN_NO_SERVICE != plmnStatus && PLMN_EMERGENCY_ONLY != plmnStatus && PLMN_EMERGENCY_SIM_INVALID != plmnStatus))
		{
			MMIHR_Trace(("HEROAPI_GetActiveSim i = %d", i));
			return i;
		}
	}
	return MMIAPISET_GetActiveSim();
}

#define HERO_APP_CELL_INFO_MAX_SIZE  (8)
typedef struct tagHeroAppCellInfoNewSt{
	unsigned short mcc;
	unsigned short mnc;
	unsigned short lac;
	unsigned short cid;
	unsigned char sig;
}heroAppCellInfoNewSt;
typedef struct tagHeroAppCellDataSt{
	int count;
	heroAppCellInfoNewSt sheroAppCellInfo[HERO_APP_CELL_INFO_MAX_SIZE];
	char lat[12]; // local20170921
	char lng[12];
}heroAppCellDataSt;

typedef struct tagHeroAppCellInfoStV2{
	unsigned short mcc;
	unsigned short mnc;
	unsigned int lac;
	unsigned int cid;
	unsigned int sig;
}heroAppCellInfoStV2;
typedef struct tagHeroAppCellDataStV2{
	int count;
	heroAppCellInfoStV2 sheroAppCellInfo[HERO_APP_CELL_INFO_MAX_SIZE];
	char lat[12];
	char lng[12];
}heroAppCellDataStV2;

static heroAppCellDataStV2 sheroAppCellData = {0};

LOCAL int32 hero_get_cell_info()
{
    MN_DUAL_SYS_E    dual_sys_mn;
	MN_PHONE_CURRENT_PLMN_INFO_T info;

    dual_sys_mn = HEROAPI_GetActiveSim();
	info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys_mn);

    memset((void*)&sheroAppCellData, 0, sizeof(sheroAppCellData));

    sheroAppCellData.count++;
    sheroAppCellData.sheroAppCellInfo[0].mcc = info.mcc;
    sheroAppCellData.sheroAppCellInfo[0].mnc = info.mnc;
    sheroAppCellData.sheroAppCellInfo[0].cid = info.cell_id;
    sheroAppCellData.sheroAppCellInfo[0].lac = info.lac;
    hr_event(9004, (int32)&sheroAppCellData, sizeof(heroAppCellDataStV2));
    return HR_SUCCESS;
}

typedef struct tagHeroAppStartGetCellInfoReqSt{
	int active_sim;
}heroAppStartGetCellInfoReqSt;
int32 hero_app_start_get_cell_info_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	heroAppStartGetCellInfoReqSt *reqData = (heroAppStartGetCellInfoReqSt*)input;
	if(reqData == NULL || input_len != sizeof(heroAppStartGetCellInfoReqSt))
	{
		return HR_FAILED;
	}
	return hero_get_cell_info() ? HR_SUCCESS : HR_FAILED;
}

typedef struct{
	char *gbStr;
	char *outBuf;
	int outBufLen;
}hrGb2Ucs2ReqData;
int32 hr_gb_2_ucs2(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(input != NULL && input_len == sizeof(hrGb2Ucs2ReqData))
	{
		hrGb2Ucs2ReqData *pReqData = (hrGb2Ucs2ReqData*)input;
		if(pReqData->gbStr != NULL && pReqData->outBuf != NULL && pReqData->outBufLen > 0)
		{
			GUI_GB2UCS((uint16*)pReqData->outBuf, (uint8*)pReqData->gbStr, strlen(pReqData->gbStr));
			hr_str_convert_endian(pReqData->outBuf);
			return HR_SUCCESS;
		}
	}
	return HR_FAILED;
}
//modify 20181114 end

#define SUPPORT_IMG_DECODE 0x2
#define SUPPORT_RECORD  0x4
#define SUPPORT_OTHER   0x8 
#define SUPPORT_VIBRA  0x10
#define SUPPORT_AUDIO  0x20
#define SUPPORT_ROUNDDISP  0x40
#define SMART_REC_START_AUDIO  0x800
#ifdef __TIHO_TTS_REMOVE_TIME_DESCRIPTION__
#define SUPPORT_REMOVE_TIME_DESCRIPTION 0x400
#endif
int32 hr_get_device_support(int32 param)
{
	int ret = SUPPORT_IMG_DECODE | SUPPORT_RECORD |SUPPORT_OTHER |SUPPORT_AUDIO|SMART_REC_START_AUDIO
        #ifdef __TIHO_TTS_REMOVE_TIME_DESCRIPTION__
	    | SUPPORT_REMOVE_TIME_DESCRIPTION
	    #endif
		#ifndef __NO_VIBRATOR__
		|SUPPORT_VIBRA
		#endif
		#ifdef __HR_ROUND_DISP__
		|SUPPORT_ROUNDDISP
		#endif
		;
	return HR_PLAT_VALUE_BASE + ret;
} 

// 判断手机盘空间
int32 hero_app_phone_free_size_state()
{
	int32 ret = 0;	
	int32 resValue = HR_SUCCESS;
    uint32 free_low  = 0;
    uint32 free_high = 0;
	uint64 free_space = 0;

	MMIHR_Trace(("hero_app_phone_free_size_state start"));
	if(MMIFILE_GetDeviceStatus((uint8*)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
	{
		MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN, &free_high, &free_low);
	}
	else
	{
		resValue = HR_FAILED;
	}

	MMIHR_Trace(("hero_app_phone_free_size_state resValue = %d", resValue));

	if(resValue == HR_SUCCESS)
	{
		if(free_high || (uint32)free_low > (uint32)0x7fffffff)
        {
            free_space = (uint64)((free_high<<22) + (free_low>>10))*1024;
		}
        else
        {
            free_space = free_low;
		}
		
		MMIHR_Trace(("hero_app_phone_free_size_state free_space = %lld", free_space));
		if((free_space) > __HR_PHONE_DRV_FREE_SIZE__)
		{
			ret = 1;
		}
	}
	return ret;
}

int32 hr_check_extern_app_support()
{
	int32 ret = 0x0;
	ret |= HR_EXTERN_APP_AUDIO_STREAM;
	return ret;
}


//modify 20211211 begin
enum{
	HERO_DEVICE_ADULT_PHONE = HR_PLAT_VALUE_BASE,
	HERO_DEVICE_CHILD_PHONE,
	HERO_DEVICE_ADULT_WATCH,  //成人手表
	HERO_DEVICE_CHILD_WATCH,  //儿童手表
	HERO_DEVICE_STUDENT_CARD, //儿童卡片
	HERO_DEVICE_ADULT_CARD,   //成人卡片
	HERO_DEVICE_ADULT_B6T,    //成人手环
	HERO_DEVICE_CHILD_B6T,    //儿童手环
};
int32 hr_plat_get_device_type()
{
	return HERO_DEVICE_CHILD_WATCH;
}
//modify 20211211 end

/*********************************************************
*
*添加了whatsapp的几个功能支持，FAE可以按照客户项目的多媒体支持情况自行配置
*SUPPORT_AUDIO:支持语音播放;
*SUPPORT_IMG_DECODE:支持图片解码；
*SUPPORT_RECORD:支持录音；
*SUPPORT_VIBRA:支持震动；
*
**********************************************************/
#define SUPPORT_AUDIO  0x1
#define SUPPORT_IMG_DECODE 0x2
#define SUPPORT_RECORD  0x4
#define SUPPORT_OTHER   0x8 
#define SUPPORT_VIBRA  0x10

int32 hr_plat(int32 code, int32 param)
{
    MMIHR_Trace(("mrapp hr_plat code:%d param:%d", code, param));
	switch(code)
	{
	case HR_REQ_POWER_OFF:
		return hr_power_off(param);
	case HR_REQ_SET_VOLUME:
		return hr_setting_set_volume(param);
	case HR_REQ_SET_CUR_SENCE:
		return hr_setting_set_cur_scene(param);
// 20181107 start
#ifdef HERO_ENGINE_TTS_SUPPORT
	case HR_GET_TTS_TYPE:
		{
			return hr_get_tts_type(param);
		}
#endif
// 20181107 end
	case HR_PLAT_CALL_INTERFACE_SUPPORT:
		return HR_SUCCESS;
#ifdef __HERO_ENGINE_OVERSEA__
	case HR_SET_DATA_ACCOUNT_USE_FLAG:
		if(param)
        {
			gHeroUseNVdataAccount = 0;
		}
        else
        {
			gHeroUseNVdataAccount = 1; 
		}
		return HR_SUCCESS;
        
	case HR_SET_DATA_ACCOUNT_MTK_UI:
		MMIHEROAPP_setApnByUser();
		return HR_SUCCESS;
        
	case HR_GET_DATA_ACCOUNT_REQ:
		return MMIHEROAPP_getApnAccount();
#endif

	case HR_PLAT_ADAPT_CPUCLK:
		return MMIHEROAPP_adaptCpuClk(param);
        
#ifdef HERO_APP_SIMUGAME_OPEN
	case HR_PLAT_GET_KEY_NUM:
		return HR_PLAT_VALUE_BASE + sizeof(hrKeyMap);
	case HR_PLAT_GET_KEY_STATE:
		if(param < sizeof(hrKeyMap))
		{
			uint32 state = KPDSVR_GetKeyState(hrKeyMap[param]);
			//MMIHR_Trace(("key[%d] state:%d", param, state));
			return HR_PLAT_VALUE_BASE + state;
		}
		break;
	case HR_PLAT_NES_ACTION_IND:
		return MMIHEROAPP_nesActionInd(param);
#endif

	case HR_PLAT_ENVINFO_WRITE:
		{
			HERO_APP_ENVINFO_T envinfo;
			envinfo.count = param;
			MMINV_WRITE(HERO_APP_ENV_INFO, &envinfo);
			return HR_SUCCESS;
		}
    
	case HR_PLAT_ENVINFO_READ:
		{
			HERO_APP_ENVINFO_T envinfo;
			int8 inited;
			MN_RETURN_RESULT_E result;
			
			MMINV_READ(HERO_APP_ENV_INITED, &inited, result);
			MMIHR_Trace(("result: %d, inited: %d", result, inited));
			if(result != MN_RETURN_SUCCESS)
			{
				inited = 0;
			}
			
			if(inited == HERO_APP_ENV_VALUE_INITED)
			{
				MMINV_READ(HERO_APP_ENV_INFO, &envinfo, result);
				MMIHR_Trace(("result: %d, count: %d", result, envinfo.count));
				if(result == MN_RETURN_SUCCESS)
				{
					return HR_PLAT_VALUE_BASE + envinfo.count;
				}
				else
				{
					return HR_FAILED;
				}
			}
			else
			{
				inited = HERO_APP_ENV_VALUE_INITED;
				MMINV_WRITE(HERO_APP_ENV_INITED, &inited);
				return HR_WAITING;
			}
		}
			
	case HR_PLAT_SET_KEYSOUND://按键音开启关闭控制: 0 关闭， 1 开启
		if(param == 0)
		{
			MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);  //停止按键音//for Z998 by zlm 20071114
			MMIDEFAULT_EnableKeyRing(HERO_VIDEOPLAYER, 0);
//			MMIDEFAULT_EnableKeyRing(FALSE);
		}
		else
		{
			MMIDEFAULT_EnableKeyRing(HERO_VIDEOPLAYER, 1);
//			MMIDEFAULT_EnableKeyRing(TRUE);
		}
		return HR_SUCCESS;
        
#ifdef HERO_APP_SIMUGAME_OPEN
	case HR_PLAT_SET_NESWAPSAVEDEV:
		MMIHR_Trace(("dev: %c", param));
		g_hr_dualsys_mng.devSel = (uint8)param;
		return HR_SUCCESS;	
#endif		

	case HR_PLAT_SET_LCD_REFRESH_MODE:// 2009-4-28 add
		MMIHEROAPP_setLcdAsynMode(param);
		return HR_SUCCESS;

    case HR_PLAT_SET_TP_SOUND://触屏音开启关闭控制: 0 关闭， 1 开启
		if(param == 0)
		{
			MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);
            MMIDEFAULT_EnableTpRing(HERO_VIDEOPLAYER, 0);
//	        MMIDEFAULT_EnableTpRing(FALSE);
		}
		else
		{
            MMIDEFAULT_EnableTpRing(HERO_VIDEOPLAYER, 1);
//			MMIDEFAULT_EnableTpRing(TRUE);
		}
		return HR_SUCCESS;
        
	case HR_PLAT_CONNECT_RESULT:
		return MMIHEROAPP_connectStateQuery(param);
        
	case HR_PLAT_SET_CONNECT_TIMEOUT:
		s_hr_connect_timeout = param/1000;
		return HR_SUCCESS;

	case HR_PLAT_APP_PICSTOP_IND:			
		MMIHR_Trace(("mrapp HR_PLAT_APP_PICSTOP_IND g_hr_vmBackRun = %d", g_hr_vmBackRun));

        MMIHEROAPP_StopStatusIconTimer();
		#if !defined(PLATFORM_UIX8910)
        MAIN_SetIdleHeroappState(FALSE); /*lint !e718 !e18*/
        #endif
		return HR_SUCCESS;
        
	case HR_PLAT_SET_ACTIVE_SIM:
		{
            int32 simId = (-param)-1;
            BOOLEAN ret = FALSE;
            if(simId >= MN_DUAL_SYS_1 && simId < MN_DUAL_SYS_MAX)
            {
    			ret = MMIPHONE_IsSimOk(simId);
    			MMIHR_Trace(("ret: %d, simId: %d", ret, simId));
    			if(ret)
    			{
    				g_hr_dualsys_mng.simSel = (int32)simId;
    				return HR_SUCCESS;
    			}
            }
			break;
		}

    case HR_PLAT_SMS_RECIVE_ACTION:
        return MMIHEROAPP_SetNewSmsAction(param);

	case HR_PLAT_GET_SMS_STATUS:
		return hr_sms_get_status(param);
		
	case HR_PLAT_GET_SMS_NUM:
		return hr_sms_get_num(param);
		
	case HR_PLAT_GET_SMS_CAPACITY:
		return hr_sms_get_capacity(param);
	
    case HR_PLAT_GET_BACKLIGHT_STATE:
        if(MMIDEFAULT_IsAllowBackLightTurnOff())
        {
            return HR_BACKLIGHT_OFF;
        }
        else
        {
            return HR_BACKLIGHT_ON;
        }

    case HR_PLAT_BACKSTAGE_SUPPORT:
	{
	#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
		return HR_SUCCESS;
	#endif

#ifdef __HR_SUPPORT_BACKSTAGE__
		return HR_SUCCESS;
#else
		return HR_IGNORE;
#endif
	}
	break;

	//modify 20211211 begin
	case HR_PLAT_GET_DEVIDE_TYPE:
		return hr_plat_get_device_type();
	//modify 20211211 end

    case HR_PLAT_GET_SC_REQ:
        return MMIHEROAPP_GetSCNumber();
        
	case HR_PLAT_APP_ACTIVE2FRONT_REQ:
		MMIHR_Trace(("DSMQQ HR_PLAT_APP_ACTIVE2FRONT_REQ g_hr_vmBackRun = %d", g_hr_vmBackRun));
		switch(g_hr_vmBackRun)
		{
		case HERO_APP_MAX:
			break;				
		default:
			if(MMIHEROAPP_HrAppRun(NULL, g_hr_vmBackRun))
			{
				hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_APPRESUME, 0);
				return HR_SUCCESS;
			}
			break;
		}
		return HR_FAILED;
        
	case HR_PLAT_GET_TC_INFO:
#ifdef HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
#ifdef MMI_PDA_SUPPORT
        return HR_SETTING_SCREEN_ONLY_TOUCH;
#else
		return HR_SETTING_SCREEN_TOUCH;
#endif
#else
		return HR_SETTING_SCREEN_NORMAL;
#endif

	case HR_PLAT_GET_LANG_INFO:
		{
			MMISET_LANGUAGE_TYPE_E language_type;
			MMIAPISET_GetLanguageType(&language_type);
			if(param== 1000)   
			{
				return HR_SUCCESS;
			}else if(param == 1001){//
			//例如如果客户不支持图片解码，就将SUPPORT_IMG_DECODE这个宏去掉，默认是全支持
				return SUPPORT_AUDIO|SUPPORT_IMG_DECODE|SUPPORT_RECORD|SUPPORT_VIBRA;
			}
			switch(language_type)
            {
			default:
		    case MMISET_LANGUAGE_ENGLISH:     
				return MR_ENGLISH;
			#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)	
		    case MMISET_LANGUAGE_SIMP_CHINESE:   
				return MR_CHINESE;
			#endif	
			#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
		    case MMISET_LANGUAGE_TRAD_CHINESE:   
				return MR_TCHINESE;
			#endif
			#ifdef IM_ARABIC_SUPPORT	
		    case MMISET_LANGUAGE_ARABIC:
				return MR_ARABIC;
			#endif
            #ifdef IM_FRENCH_SUPPORT
		    case MMISET_LANGUAGE_FRENCH:
				return MR_FRENCH;
			#endif
			#ifdef IM_HINDI_SUPPORT
		    case MMISET_LANGUAGE_HINDI:
				return MR_HINDI;
			#endif
			#ifdef IM_HUNGARIAN_SUPPORT	
		    case MMISET_LANGUAGE_HUNGARIAN:
				return MR_HUNGARIAN;
			#endif
			#ifdef IM_INDONESIAN_SUPPORT
		    case MMISET_LANGUAGE_INDONESIAN:
				return MR_INDONESIAN;
			#endif
			#ifdef IM_MALAY_SUPPORT	
		    case MMISET_LANGUAGE_MALAY:
				return MR_MALAY;
			#endif
			#ifdef IM_PORTUGUESE_SUPPORT	
		    case MMISET_LANGUAGE_PORTUGUESE:
				return MR_PORTUGUESE;
			#endif
			#ifdef IM_RUSSIAN_SUPPORT	
		    case MMISET_LANGUAGE_RUSSIAN:
				return MR_RUSSIAN;
			#endif
			#ifdef IM_SPANISH_SUPPORT	
		    case MMISET_LANGUAGE_SPANISH:
				return MR_SPANISH;
			#endif
			#ifdef IM_TAGALOG_SUPPORT	
		    case MMISET_LANGUAGE_TAGALOG:
				return MR_ENGLISH;//temp.
			#endif
			#ifdef IM_THAI_SUPPORT	
		    case MMISET_LANGUAGE_THAI:
				return MR_THAI;
			#endif
			#ifdef IM_VIETNAMESE_SUPPORT	
		    case MMISET_LANGUAGE_VIETNAMESE://only include text of above 15 langugages
		    	return MR_VIETNAMESE;
			#endif
			#ifdef IM_URDU_SUPPORT	
		    case MMISET_LANGUAGE_URDU:
				return MR_URDU;
			#endif
			#ifdef IM_ITALIAN_SUPPORT	
		    case MMISET_LANGUAGE_ITALIAN:
				return MR_ITALIAN;
			#endif
			#ifdef IM_PERSIAN_SUPPORT	
		    case MMISET_LANGUAGE_PERSIAN:
				return MR_PERSIAN;
			#endif
			#ifdef IM_TURKISH_SUPPORT	
		    case MMISET_LANGUAGE_TURKISH:
				return MR_TURKISH;
			#endif
			#ifdef IM_GERMAN_SUPPORT	
		    case MMISET_LANGUAGE_GERMAN:
				return MR_GERMAN;
			#endif
			#ifdef IM_GREEK_SUPPORT	
		    case MMISET_LANGUAGE_GREEK:
				return MR_GREEK;
			#endif
				//break;
    		}
		}
	case HR_PLAT_GET_RAND_NUM:
		srand(SCI_GetTickCount());
		return HR_PLAT_VALUE_BASE + rand()%param;

    case HR_PLAT_GET_CUR_SCENE:
        return MMIHEROAPP_GetCurScene() + HR_PLAT_VALUE_BASE;
    
	case HR_PLAT_FORBID_HUP2IDLE:
		s_hr_fbhup2idle = param;
		return HR_SUCCESS;

    case HR_PLAT_GET_CELLINFO_START:
        return HR_SUCCESS;

    case HR_PLAT_GET_CELLINFO_END:
        return HR_SUCCESS;

    case HR_PLAT_TCARD_STATUS:
        return MMIHEROAPP_GetTCardStatus();
        
	case HR_PLAT_GET_FILE_POS:
        return MMIHEROAPP_GetFilePos(param);
    
	case HR_PLAT_GET_SOUND_VOLUME:
		return HERO_APP_AUDIO_VOL_MAX + HR_PLAT_VALUE_BASE;
    
	case HR_PLAT_SET_SOUND_VOLUME:
		return MMIHEROAPP_setSoundVolume(param);

    case HR_PLAT_CHECK_WIFI:
        return MMIHEROAPP_CheckWifiSupport();

    case HR_PLAT_SET_WIFI_SWITCH:
        return MMIHEROAPP_SetWifiSwitch(param);
		
#ifdef __HR_RECORD_SUPPORT__
    case HR_VOICE_RECORD_PAUSE:
        return MMIHEROAPP_RecordPause();

    case HR_VOICE_RECORD_RESUME:
        return MMIHEROAPP_RecordResume();
        
    case HR_VOICE_RECORD_STOP:
        return MMIHEROAPP_RecordStop();
#endif

#ifdef __HERO_APP_MOTION_SUPPORT__
    case HR_MOTION_STOP_LISTEN:				
        return hr_motion_stop_listen(param);
    case HR_MOTION_PWRON:
        return hr_motion_power_on(param);
    case HR_MOTION_PWROFF:
        return hr_motion_power_off(param);
    case HR_MOTION_LISTEN_SHAKE:
        return hr_motion_listen_shake(param);
    case HR_MOTION_LISTEN_TILT:
        return hr_motion_listen_tilt(param);
    case HR_MOTION_GET_MAX_ACC:	
        return hr_motion_get_max_acc(param);
#endif

#ifdef UI_MULTILAYER_SUPPORT
    case HR_PLAT_RELEASE_LAYER:
        return hr_layer_release(param);

    case HR_PLAT_SET_ACTIVE_LAYER:
        return hr_layer_set_active(param);

    case HR_PLAT_SET_LAYER_ALPHA:
        return hr_layer_set_alpha(TRUE, param);

    case HR_PLAT_SET_LAYER_NO_ALPHA:
        return hr_layer_set_alpha(FALSE, param);
        
    case HR_PLAT_SET_LAYER_TRANSPARENT:
        return hr_layer_set_transparent(TRUE, param);
        
    case HR_PLAT_SET_LAYER_NO_TRANSPARENT:
        return hr_layer_set_transparent(FALSE, param);
#endif

#ifdef __HERO_CFG_CAMERA_SUPPORT__
    case HR_PLAT_CAMERA_INIT:
        return hr_camera_init(param);
        
    case HR_PLAT_CAMERA_PAUSE:
        return hr_camera_pause(param);
        
    case HR_PLAT_CAMERA_RESUME:
        return hr_camera_resume(param);
            
    case HR_PLAT_CAMERA_CLOSE:
        return hr_camera_destroy(param);

    case HR_PLAT_CAMERA_ZOOM_MAX:
        return hr_camera_get_zoom_max(param);
        
    case HR_PLAT_CAMERA_ZOOM_IN:
        return hr_camera_zoom_in(param);
        
    case HR_PLAT_CAMERA_ZOOM_OUT:
        return hr_camera_zoom_out(param);
#endif 

    case HR_PLAT_SET_LCD_ROTATE:
        return hr_lcd_set_rotation(param);

	case HR_GET_DEVICE_SUPPORT:
		return hr_get_device_support(param);

#ifdef _GSM_ONLY_
	case HR_CHECK_GPRS_SUPPORT:
		return HR_FAILED;
#endif

	case HR_DELETE_SMS_IS_MEMORY_FULL:
		return hr_setting_sms_is_memory_full(param);

	//add 20200507 begin
	case HERO_APP_GET_PRICE_REQ:
		return hero_app_get_price_by_appid(param) + HR_PLAT_VALUE_BASE;
	case HERO_APP_REST_PRICE_REQ:
		return hero_app_reset_price_by_appid(param) ? HR_SUCCESS : HR_FAILED;
	case HERO_APP_IS_SHOW_NETTIP_REQ:
		return hero_app_is_show_nettips() ? HR_SUCCESS : HR_FAILED;
	case HERO_APP_GET_TOOLBAR_TYPE_REQ:
		return hero_app_get_toolbar_type() + HR_PLAT_VALUE_BASE;
	case HERO_APP_GET_MENU_SWTICE_REQ:
		return  hero_app_get_menu_swtich() + HR_PLAT_VALUE_BASE; //hero_app_get_menu_swtich() ? HR_SUCCESS : HR_FAILED;
	//add 20200507 end

	case HERO_EXTERN_CHECK_APP_SUPPORT:
		return hr_check_extern_app_support() + HR_PLAT_VALUE_BASE;

	case HR_PHONE_FREE_SIZE_STATE:
		return hero_app_phone_free_size_state() + HR_PLAT_VALUE_BASE;
	case HR_PLAT_VM_PAUSE_MODE:
	default:
		return HR_IGNORE;
	}
	
	return HR_FAILED;
}

int32 hr_platEx(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 resValue = HR_IGNORE;

#ifdef __MPCHAT_SUPPORT__
	if(code>>16 == 3)
	{
		extern int32 mr_platEx_mpchat(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
		mr_platEx_mpchat(code, input, input_len, output, output_len, cb);
		return HR_SUCCESS;
	}
#endif

//modify 20181117 begin
    MMIHR_Trace(("mrapp hr_platEx code:%d input:0x%08x input_len:%d output:0x%08x output_len:%d", code, input, input_len, output, output_len));
	if(code == 903)
	{
		resValue = hr_plat_mem_free(input, input_len, output, output_len, cb);
	}

#ifdef HERO_CAMERA_SUPPORT
	else if(code == HR_CAMERA_SUPPORT)
	{
		resValue = HR_SUCCESS;
	}
	else if(code == HR_CAMERA_OPEN)
	{
		extern int hero_camera_open(char *input, int inputsize, char **output, int *outputsize);
		resValue = hero_camera_open((char*)input, (int)input_len, (char**)output, (int*)output_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_CAMERA_CLOSE)
	{
		extern int hero_camera_close(char *input, int inputsize);
		resValue = hero_camera_close((char*)input, (int)input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_CAMERA_CAPTURE)
	{
		extern int hero_camera_capture(char **output, int *outputsize);
		resValue = hero_camera_capture((char**)output, (int*)output_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_CAMERA_CAPTURE_RELEASE)
	{
		extern int hero_camera_capture_release(char *input, int inputsize);
		resValue = hero_camera_capture_release((char*)input, (int)input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_CAMERA_GET_ROTATION_MODE)
	{
		extern int hero_camera_get_rotation_mode(char *input, int inputsize);
		resValue = hero_camera_get_rotation_mode((char*)input, (int)input_len) + HR_PLAT_VALUE_BASE;
	}
#endif

#ifdef HERO_QR_DECODE_SUPPORT
	else if(code == HR_QR_DECODE_SUPPORT)
	{
		resValue = HR_SUCCESS;
	}
	else if(code == HR_QR_DECODE_OPEN)
	{
		extern int hero_qr_decipher_open_context(char *input, int inputsize);
		resValue = hero_qr_decipher_open_context((char*)input, (int)input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_QR_DECODE_CLOSE)
	{
		extern int hero_qr_decipher_close_context(char *input, int inputsize);
		resValue = hero_qr_decipher_close_context((char*)input, (int)input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_QR_DECODE_PUSH_IMG)
	{
		extern int hero_qr_decipher_push_luma_img(char *input, int inputsize);
		resValue = hero_qr_decipher_push_luma_img((char*)input, (int)input_len) ? HR_SUCCESS : HR_FAILED;
	}
#endif

	// fm start
	else if(HERO_FM_AMR_STREAM_OPEN == code)
	{
		resValue = heroFmAudioOpen((char*)input, (int)input_len, (char**)output, (int*)output_len);
	}
	else if(HERO_FM_AMR_STREAM_CLOSE == code)
	{
		resValue = heroFmAudioClose();
	}
	else if(HERO_FM_AMR_STREAM_PUSH == code)
	{
		resValue = heroFmAudioAppendData((char*)input, (int)input_len, (char**)output, (int*)output_len);
	}
	else if(HERO_FM_AMR_STREAM_RESET == code)
	{
		resValue = heroFmAudioClear();
	}
	else if(HERO_FM_AMR_STREAM_GET_CACHE_SIZE == code)
	{
		resValue = heroFmAudioGetCacheSize(output, output_len);
	}
	else if(HERO_FM_AMR_STREAM_SET_VOLUME == code)
	{
		resValue = heroFmAudioSetVolume((char*)input, (int)input_len);
	}
	else if(HERO_FM_AMR_STREAM_GET_VOLUME == code)
	{
		resValue = heroFmAudioGetVolume((char*)input, (int)input_len);
	}
	else if(HERO_FM_AMR_STREAM_GET_MAX_CACHE_SIZE == code)
	{
		resValue = heroFmAudioGetMaxCacheSize();
	}
	else if(HERO_FM_AMR_STREAM_PAUSE == code)
	{
		resValue = heroFmAudioPause();
	}
	else if(HERO_FM_AMR_STREAM_RESUME == code)
	{
		resValue = heroFmAudioResume();
	}
	else if(HERO_FM_AMR_STREAM_GET_CURRENT_TIME == code)
	{
		resValue = heroFmAudioGetInfo(output, output_len);
	}
	// fm end
	else if(code == 904)
	{
		resValue = hr_plat_mem_malloc(input, input_len, output, output_len, cb);
	}
	else if(code == HR_INIT_CALL_LOG_OPERATE)
	{
		resValue = hr_setting_init_call_log_operate(input, input_len, output, output_len, cb);
	}
	else if(code == HR_DESTORY_CALL_LOG_OPERATE)
	{
		resValue = hr_setting_destory_call_log_operate(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_CALL_LOG_COUNT)
	{
		resValue = hr_setting_get_call_log_count(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_CALL_LOG_BY_INDEX)
	{
		resValue = hr_setting_get_call_log_by_index(input, input_len, output, output_len, cb);
	}
	else if(code == HR_DELETE_CALL_LOG_BY_INDEX)
	{
		resValue = hr_setting_delete_call_log(input, input_len, output, output_len, cb);
	}
	else if(code == HR_INIT_SMS_OPERATE)
	{
		resValue = hr_setting_init_sms_operate(input, input_len, output, output_len, cb);
	}
	else if(code == HR_DESTORY_SMS_OPERATE)
	{
		resValue = hr_setting_destory_sms_operate(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_SMS_COUNT)
	{
		resValue = hr_setting_get_sms_cout(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_SMS_BY_INDEX)
	{
		resValue = hr_setting_get_sms_by_index(input, input_len, output, output_len, cb);
	}
	else if(code == HR_DELETE_SMS_BY_INDEX)
	{
		resValue = hr_setting_delete_sms_by_index(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_EXT_PARAM)
	{
		resValue = hr_get_ext_param_req(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_AMAM_INFO)
	{
		resValue = hr_get_alm_info(input, input_len, output, output_len, cb);
	}
	else if(code == HR_SET_AMAM_INFO)
	{
		resValue = hr_set_alm_info(input, input_len, output, output_len, cb);
	}
	else if(code == HR_SET_FORWARD_REQ)
	{
		resValue = hr_setting_set_forward(input, input_len, output, output_len, cb);
	}
	else if(code == HR_CANCEL_FORWARD_REQ)
	{
		resValue = hr_setting_cancel_forward(input, input_len, output, output_len, cb);
	}
#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
	else if(code == HR_READ_OLALA_CFG_DATA)
	{
		resValue = hr_setting_read_olala_cfg_data(input, input_len, output, output_len, cb);
	}
	else if(code == HR_WRITE_OLALA_CFG_DATA)
	{
		resValue = hr_setting_write_olala_cfg_data(input, input_len, output, output_len, cb);
	}
	else if(code == HR_FAMILY_DURABLE_DATA_LIST_IS_FULL)
	{
		resValue = hero_family_durable_data_list_is_full() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_DURABLE_DATA_LIST_SIZE)
	{
		resValue = hero_family_durable_data_list_size();
	}
	else if(code == HR_FAMILY_DURABLE_DATA_ADD)
	{
		resValue = hero_family_add_durable_data_req(input, input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_DURABLE_DATA_FIND_START)
	{
		resValue = hero_family_durable_data_find_start_req((char**)output, (int*)output_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_DURABLE_DATA_FIND_NEXT)
	{
		resValue = hero_family_durable_data_find_next_req((char**)output, (int*)output_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_DURABLE_DATA_REMOVE)
	{
		resValue = hero_family_remove_durable_data_req(input, input_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_CLEAR_ALL_DURABLE_DATA)
	{
		resValue = hero_family_clear_all_durable_data() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HR_FAMILY_GET_GLOBAL_BUF)
	{
		resValue = hero_family_get_global_buf((char**)output, (int*)output_len) ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HERO_FAMILY_TCP_SUPPORT)
	{
		resValue = HR_SUCCESS;
	}
	else if(code == HERO_FAMILY_TCP_INIT_CONNECTION)
	{
		resValue = heroFamilyInitLongConnection() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HERO_FAMILY_TCP_START_CONNECTION)
	{
		resValue = heroFamilyStartLongConnection() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HERO_FAMILY_TCP_STOP_CONNECTION)
	{
		resValue = heroFamilyStopLongConnection() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HERO_FAMILY_TCP_IS_CONNECTED)
	{
		resValue = heroFamilyTcpIsConnected() ? HR_SUCCESS : HR_FAILED;
	}
	else if(code == HERO_FAMILY_TCP_SEND_DATA)
	{
		resValue = heroFamilyTcpSenddata(input, input_len) ? HR_SUCCESS : HR_FAILED;
	}
#endif
	else if(code == HR_START_DSM_REQ__x)
	{
		resValue = hr_start_dsm_app_req_x(input, input_len, output, output_len, cb);
	}
	else if(code == HR_DSM_APP_VERSION__x)
	{
		resValue = hr_get_dsm_app_version_x(input, input_len, output, output_len, cb);
	}
	else if(code == HR_START_HERO_APP_REQ__x)
	{
		resValue = hero_start_hero_app_req_ex(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_SEND_SMS)
	{
		resValue = hero_app_send_sms_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_GET_AVAILABLE_SIM)
	{
		resValue = hero_app_get_available_sim_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_ADD_KEY)
	{
		resValue = hero_app_add_key_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_GET_VERSION)
	{
		resValue = hero_app_get_version_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_SET_CYCLE)
	{
		resValue = hero_app_set_cycle_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_SET_START_PARAMETER)
	{
		resValue = hero_app_set_start_parameter_req(input, input_len, output, output_len, cb);
	}
#ifdef HERO_APP_SHORTCUT_SUPPORT
	else if(code == HERO_APP_API_GET_SHORTCUT_DATA)
	{
		resValue = hr_get_shortcut_data_req(input, input_len, output, output_len, cb);
	}
	else if(code == HERO_APP_API_SET_SHORTCUT_DATA)
	{
		resValue = hr_set_shortcut_data_req(input, input_len, output, output_len, cb);
	}
#endif
//modify 20181117 end
	#ifdef HERO_ENGINE_HMGS_SUPPORT
	else if(code == HR_GET_PHONE_NUMBER_LOCATION__x)
	{
		extern int32 hero_get_phone_number_location_ex(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
		resValue = hero_get_phone_number_location_ex(input, input_len, output, output_len, cb);
	}
    #endif
    #ifdef HERO_ENGINE_TTS_SUPPORT
	else if(code == HR_PLAY_TTS_REQ__x)
	{
		resValue = hr_plat_tts_req(input, input_len, output, output_len, cb);
	}
	else if(code == HR_STOP_TTS_REQ__x)
	{
		resValue = hr_stop_tts_req(input, input_len, output, output_len, cb);
	}
    #endif
	#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	else if(code == HERO_GET_TTS_SWITCH__x)
	{
		resValue = hero_app_get_tts_menu_switch_status(input, input_len, output, output_len);
	}
	else if(code == HERO_SET_TTS_SWITCH__x)
	{
		resValue = hero_app_set_tts_menu_switch_status(input, input_len, output, output_len);
	}
    #endif
    else if(code == HERO_HTTP_SUPPORT__x)
	{
		resValue = HR_SUCCESS;
	}
	else if(code == HERO_HTTP_GET_REQ__x)
	{
		resValue = hero_app_http_get_request(input, input_len, output, output_len);
	}
	else if(code == HERO_HTTP_POST_REQ__x)
	{
		resValue = hero_app_http_post_request(input, input_len, output, output_len);
	}
	else if(code == HERO_HTTP_STOP_REQ__x)
	{
		resValue = hero_app_http_stop_request(input, input_len, output, output_len);
	}
	else if(code == HERO_HTTP_DECODE_PKG__x)
	{
	#if defined(WIN32)
		resValue = HR_IGNORE;
	#else
		resValue = hero_app_decode_cmd_req(input, input_len, output, output_len, cb);
	#endif
	}
	else if(code == HERO_HTTP_UPDATE_EXTDATA__x)
	{
		resValue = hero_app_update_ext_data(input, input_len);
	}
    else if(code == 8000)  // plat type (via - 1),(zte - 2),(spr - 3)
	{
		resValue = HR_PLAT_VALUE_BASE + 3;
	}
//modify 20181114 begin
	else if(code == HERO_APP_API_GET_CELL_INFO)
	{
		resValue = hero_app_start_get_cell_info_req(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_SMART_AUTH_ID) // modify 20181105 begin
	{
		resValue = hr_get_smart_authId(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_CUSTOMER_KEY)
	{
		resValue = hr_get_customer_key(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_CUSTOMER_HELP_STR)
	{
		resValue = hr_get_customer_help_str(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_AUDIO_PLAY_LIST_DIR)
	{
		resValue = hr_get_audio_play_list_dir(input, input_len, output, output_len, cb);
	}
//modify 20181120 begin
#ifdef HERO_APP_SMART_OPEN
	else if(code == HR_START_UP_PLAT_APP)
	{
		extern int hr_start_up_plat_app(unsigned char* input, int input_len, unsigned char** output, unsigned char* output_len, void *cb);
		resValue = hr_start_up_plat_app(input, input_len, output, output_len, cb);
	}
#endif
	else if(code == HR_GB_2_UNICODE)
	{
		resValue = hr_gb_2_ucs2(input, input_len, output, output_len, cb);
	}
	else if(code == HR_GET_REMINDER_NV_DATA)
	{
		resValue = hr_get_reminder_nv_data_req(input, input_len, output, output_len, cb);
	}
	else if(code == HR_SET_REMINDER_NV_DATA)
	{
		resValue = hr_set_reminder_nv_data_req(input, input_len, output, output_len, cb);
	}
//modify 20181120 end
	else if(code < 2000 || code >= 4000)
	{
		resValue = MMIHEROAPP_mutiCmd(code, input, input_len, output, output_len, cb);
	}
	else if(code >= 2000 && code < 3000)//audio
	{
#ifdef HERO_APP_SIMUGAME_OPEN
		if(code%10 == HR_ACI_AUDIO_PCM && code/10 >= 201 && code/10 <= 220)//pcm
		{
			resValue = MMIHEROAPP_PcmCmd(code, input, input_len, output, output_len, cb);
		}
		else
#endif
		{
			resValue = MMIHEROAPP_AudioCmd(code, input, input_len, output, output_len, cb);
		}
	}
	else if(code >= 3000 && code < 4000)//Image
	{
		resValue = MMIHEROAPP_imageCmd(code, input, input_len, output, output_len, cb);
	}
	
	return resValue;
}


static void MMIHEROAPP_ConvertPath2Loc(char *str, uint16 len)
{
	int i;
	
	for(i = 0; i < len; i++)
	{
		if(str[i] == 0x5c)// "\" to "/"
			str[i] = 0x2f;
	}
}

static void MMIHEROAPP_ConvertPath2Pc(char *str, uint16 len)
{
	int i;
	
	for(i = 0; i < len; i++)
	{
		if(str[i] == 0x2f)// '/' to '\'
			str[i] = 0x5c;
	}
}


LOCAL const uint8* MMIHEROAPP_getCurDevName(uint16 *length)
{
	const uint8* ptr = NULL;
    MMIHR_Trace(("mrapp MMIHEROAPP_getCurDevName device_type:%d", g_hr_global.device_type));
	switch(g_hr_global.device_type)
	{
	case FS_HS:
		*length = HERO_HERO_MMIFILE_DEVICE_SYS_NAME_LEN_AD;
		ptr = (const uint8*)HERO_MMIFILE_DEVICE_SYS_NAME;
		break;
	case FS_UDISK:
		*length = HERO_HERO_MMIFILE_DEVICE_UDISK_LEN_AD;
		ptr = (const uint8*)HERO_MMIFILE_DEVICE_UDISK;
		break;
	case FS_MMC:
        if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
        {
    		*length = HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_AD;
    		ptr = (const uint8*)HERO_MMIFILE_DEVICE_SDCARD;
        }
#ifndef DUAL_TCARD_SUPPORT
        else if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            *length = HERO_MMIFILE_DEVICE_SDCARD2_LEN;
    		ptr = (const uint8*)HERO_MMIFILE_DEVICE_SDCARD2;
        }
#endif
		break;
#ifdef DOUBLE_TFCARD
	case FS_MMC_2:
		*length = HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_2_AD;
		ptr = (const uint8*)HERO_MMIFILE_DEVICE_SDCARD_2;
		break;
#endif
	default:
		HERO_ASSERT(0);
		break;
	}
	
	return ptr;
}

LOCAL uint32 MMIHEROAPP_getUcstrLen(uint8 *wstr)
{
	uint32 wlen = 0;
	uint8 *ptr = (uint8 *)wstr;
	
	if(ptr != NULL)
	{
		while((*ptr<<8) + *(ptr+1) != 0)
		{
			wlen += 2;
			ptr += 2;
		}
	}
	
	return wlen;
}

LOCAL void MMIHEROAPP_bcd2dec(uint8 *bcd, uint8* dec, uint16 convertLen)
{
	uint8 i, j;
	MMIHR_Trace(("mrapp. MMIHEROAPP_bcd2dec() convertLen: %d", convertLen));
	
	if(bcd == NULL || dec == NULL)
		return;
	
	for(i = 0, j = 0; i < convertLen; i++)
	{
		dec[j++] = (bcd[i] & 0x0F) + '0';
		dec[j++] = ((bcd[i] >> 4) & 0x0F) + '0';
	}
}

LOCAL int32 MMIHEROAPP_ProcessGprsActiveCnf(MMIPDP_RESULT_E result)
{
	int32 ret = HR_FAILED;
	
	MMIHR_Trace(("mrapp MMIHEROAPP_ProcessGprsActiveCnf result: %d", result));
    
	if(result == MMIPDP_RESULT_SUCC)
    {
		s_hr_netContext.gprsState = HERO_APP_GPRS_ACTIVED;
		ret = HR_SUCCESS;
	}
//底层已经做了重试操作，这里没必要再重拨
#if 0
    else if(s_hr_netContext.pdp_retry_times < HERO_APP_PDP_RETRY_TIMES)
    {
		s_hr_netContext.pdp_retry_times++;
		MMIHEROAPP_gprsRetryTimerStart(HERO_APP_PDP_RETRY_TIMER_OUT);
		ret = HR_WAITING;
    }
    else
    {
		MMIHEROAPP_gprsRetryTimerStop();
		s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
		s_hr_netContext.mrNetActived = FALSE;
	}
#endif
	return ret;
}

MMI_RESULT_E MMIHEROAPP_handlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	uint8 recode = MMI_RESULT_TRUE;
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_NETINIT_RESULT_T opResult;
    MMIPDP_CNF_INFO_T *signal_ptr = (MMIPDP_CNF_INFO_T*)msg_ptr;

	MMIHR_Trace(("mrapp. MMIHEROAPP_handlePsMsg() msg_id = %d state = %d", signal_ptr->msg_id, s_hr_netContext.mrNetActived));
	
	if(!signal_ptr)
    {
		MMIHR_Trace(("mrapp. not mr event"));
		return MMI_RESULT_FALSE;
	}
	
	MMIHEROAPP_gprsProtectTimerStop();

	switch( signal_ptr->msg_id )
	{
	case MMIPDP_ACTIVE_CNF:
        s_hr_netContext.netid = signal_ptr->nsapi;
		resValue = MMIHEROAPP_ProcessGprsActiveCnf(signal_ptr->result);
    	if(resValue != HR_WAITING)
    	{
    		opResult.cb = s_hr_netContext.gprsCb;
    		opResult.result = resValue;
    		MMK_PostMsg(
#ifdef HR_BINDTO_VIRTUAL_WIN
    			VIRTUAL_WIN_ID,
#else
    			MMI_HERO_APP_BASE_WIN_ID,
#endif
    			HERO_APP_MSG_NETINIT_RESULT, 
    			&opResult, sizeof(MMI_HERO_APP_NETINIT_RESULT_T));
    		
    		MMI_TriggerMMITask();
    	}
		break;
		
	case MMIPDP_DEACTIVE_CNF:
		if(s_hr_netContext.subEvn == HERO_APP_NETSUBEVN_WAITDAIL)
		{
			s_hr_netContext.subEvn = HERO_APP_NETSUBEVN_NONE;
			resValue = HR_WAITING;
			MMK_PostMsg(
#ifdef HR_BINDTO_VIRTUAL_WIN
				VIRTUAL_WIN_ID,
#else
				MMI_HERO_APP_BASE_WIN_ID,
#endif				
				HERO_APP_MSG_NETREDAIL_IND, 
				PNULL, 0);
		}
		else
		{			
            s_hr_netContext.netid = 0;
			
			if(s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVEING)
            {
				s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
				s_hr_netContext.mrNetActived = FALSE;
			}
		}
		break;
    
	case MMIPDP_DEACTIVE_IND:
        s_hr_netContext.need_count = FALSE;
		s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
		s_hr_netContext.mrNetActived = FALSE;
		break;
		
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	MMIHR_Trace(("mrapp. MMIHEROAPP_processPsMsg() RETURN"));
	return recode;
}

PUBLIC void MMIHEROAPP_redail(void)
{
	int32 ret = 0;
	uint8 *user_ptr = ""; /*lint !e64*/
	uint8 *passwd_ptr = ""; /*lint !e64*/
    MMIPDP_ACTIVE_INFO_T app_info = {0};
	
	MMIHR_Trace(("mrapp MMIHEROAPP_redail ret: %d, APN: %s", ret, s_hr_netContext.apn));

    app_info.app_handler = MMI_MODULE_HEROENGINE;
    app_info.apn_ptr = (char*)s_hr_netContext.apn;
    app_info.user_name_ptr = (char*)user_ptr;
    app_info.psw_ptr = (char*)passwd_ptr;
    app_info.dual_sys = MMIHEROAPP_GetActiveSim();
    app_info.priority = 3;
    app_info.ps_service_rat = MN_UNSPECIFIED;
	app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
#ifdef IPVERSION_SUPPORT_V4_V6
	app_info.ip_type = MMICONNECTION_IP_V4;
#endif
    app_info.handle_msg_callback = MMIHEROAPP_handlePsMsg; /*lint !e64*/
    MNGPRS_ReadStorageExtReqQosEx(MMIHEROAPP_GetActiveSim(),MN_GPRS_STORAGE_ALL,IM_E,&app_info.qos_info);
#ifdef IPVERSION_SUPPORT_V4_V6
	{
		MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
		MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
		apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(app_info.dual_sys);
		MMIHR_Trace(("MMIAPICONNECTION_GetApnTypeIndex: %d", apn_idx_arr));
		if (apn_idx_arr != NULL)
		{
			linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(app_info.dual_sys, apn_idx_arr->internet_index[app_info.dual_sys].index);
			MMIHR_Trace(("MMIAPICONNECTION_GetLinkSettingItemByIndex: %d", linksetting));
			if(PNULL != linksetting)
			{
				app_info.apn_ptr = (char*)linksetting->apn;
				app_info.user_name_ptr = (char*)linksetting->username;
				app_info.psw_ptr = (char*)linksetting->password;
				app_info.ip_type = linksetting->ip_type;
				MMIHR_Trace(("app_info.apn_ptr: %s, app_info.user_name_ptr = %s, app_info.psw_ptr = %s, app_info.ip_type = %d", linksetting->apn, linksetting->username, linksetting->password, linksetting->ip_type));
			}
		}
	}
#endif

	s_hr_netContext.gprsState = HERO_APP_GPRS_ACTIVING;
    ret = MMIAPIPDP_Active(&app_info);
    if(ret == TRUE)
	{
		MMIHEROAPP_gprsProtectTimerStart(MMI_HERO_APP_GPRSPROTECT_TIME_OUT);
	}
	else
	{
		s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
		s_hr_netContext.mrNetActived = FALSE;
		if(s_hr_netContext.gprsCb != NULL)
		{
			HR_INIT_NETWORK_CB callBack;
			callBack = (HR_INIT_NETWORK_CB)s_hr_netContext.gprsCb; /*lint !e611*/
			callBack(HR_FAILED);
			s_hr_netContext.gprsCb = NULL;
		}
	}
}

void MMIHEROAPP_CloseHrNet(void)
{
	ERR_MNGPRS_CODE_E err;
	MMIHR_Trace(("mrapp. MMIHEROAPP_CloseHrNet() need? :%d, state; %d", s_hr_netContext.mrNetActived,s_hr_netContext.gprsState));
	
#ifndef WIN32
	if(s_hr_netContext.mrNetActived == TRUE)
	{
		MMIHEROAPP_gprsProtectTimerStop();//2007-10-8 add
		MMIHEROAPP_gprsRetryTimerStop();
		//2007-11-9dis for sys err. close it ignore all !! if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED || s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVING)//acting can ir directly ?
		{
			err = MMIAPIPDP_Deactive(MMI_MODULE_HEROENGINE); /*lint !e64*/
			MMIHR_Trace(("mrapp MMIAPIPDP_Deactive err: %d", err));
            s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
            s_hr_netContext.mrNetActived = FALSE;
//没有拨号成功的情况下底层会返回FALSE，此时也应该将状态复位
#if 0
			if(err == TRUE)
            {
				s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVED;
				s_hr_netContext.mrNetActived = FALSE;
			}
            else if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED
				|| s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVING)
			{
				s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVEING;
			}
#endif
			s_hr_netContext.gprsCb = NULL;

			if(s_hr_hostContext.cb != NULL)
			{
				s_hr_hostContext.cb = NULL;
				s_hr_hostContext.timerCount = 0;
				SCI_MEMSET(s_hr_hostContext.hostBuffer, 0x00, sizeof(s_hr_hostContext.hostBuffer));
				MMK_StopTimer(s_hr_hostContext.getHostTimer);
				s_hr_hostContext.getHostTimer = 0;
			}
		}
	}
#endif
	
}

void MMIHEROAPP_netInitResult(int32 result)
{
	HR_INIT_NETWORK_CB callBack;
	MMIHR_Trace(("mrapp. MMIHEROAPP_netInitResult() result :%d", result));
	
	if(s_hr_netContext.deactWaitFunc != NULL && result == HR_FAILED)
	{
		MMIHEROAPP_WAIT2DOPDPACT_FUNC waitCb;
		waitCb = (MMIHEROAPP_WAIT2DOPDPACT_FUNC)s_hr_netContext.deactWaitFunc; /*lint !e611*/
		s_hr_netContext.deactWaitFunc = NULL;
		waitCb(s_hr_netContext.data);
	}
	if(gHeroUseNVdataAccount && result == HR_SUCCESS)
    {
		MMIHEROAPP_saveApn();
	}

	MMIHEROAPP_gprsRetryTimerStop();
    
    MMIHR_Trace(("mrapp. MMIHEROAPP_netInitResult() mrNetActived:%d gprsState:%d", s_hr_netContext.mrNetActived, s_hr_netContext.gprsState));
    if(s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVED || s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVEING)
    {
        return;
    }
    
	if(s_hr_netContext.gprsCb != NULL)
	{
		callBack = (HR_INIT_NETWORK_CB)s_hr_netContext.gprsCb; /*lint !e611*/
		MMIHR_Trace(("mrapp. MMIHEROAPP_netInitResult() callBack :0x%08x", callBack));
		callBack(result);
		MMIHR_Trace(("mrapp. MMIHEROAPP_netInitResult() callBack end"));
	}
}
#ifdef HERO_ENGINE_SUPPORT
///mike.song add begin by 20140610
BOOL hero_mmi_msg_new_sms_ind(unsigned char *pContent, int nLen, char *pNum, int type);
///mike.song add end by 20140610
#endif
int32 MMIHEROAPP_SMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number)
{
	uint8 num[MMISMS_PBNUM_MAX_LEN + 2];
	uint8 bcd_limit_len = MMISMS_PBNUM_MAX_LEN >> 1; 
	uint8 copy_len = 0;
	uint8 user_data_arr[MN_SMS_MAX_USER_VALID_DATA_LENGTH + 1];
	uint8 *content = NULL;
	int32 result = HR_IGNORE;
	int32 type;
	
	if(data == NULL || number == NULL || number->num_len == 0)
		return HR_IGNORE;
	
	copy_len = MIN( bcd_limit_len , number->num_len);
	SCI_MEMSET(num, '\0', sizeof(num));
	MMIAPICOM_GenDispNumber( number->number_type,
		copy_len,
		number->party_num,
		num,
		MMISMS_PBNUM_MAX_LEN + 2);
	
	MMIHR_Trace(("alphabet_type: %d", alphabet_type));
	if(alphabet_type == MN_SMS_DEFAULT_ALPHABET)
	{
		SCI_MEMSET(user_data_arr, '\0', sizeof(user_data_arr));
		MMIAPICOM_Default2Ascii(data->user_valid_data_arr,
			user_data_arr,
			data->length);
		content = user_data_arr;
		type = HR_ENCODE_ASCII;
	}
	else
	{
		content = data->user_valid_data_arr;
		if(alphabet_type == MN_SMS_8_BIT_ALPHBET)
		{
			type = HR_ENCODE_ASCII;
		}
		else
		{
			type = HR_ENCODE_UNICODE;
		}
	}
	
	MMIHR_Trace(("number: %s, clen: %d, type: %d", num, data->length, type));
#ifndef WIN32
	result = hr_smsIndiactionEx(content, data->length, num, type);
#endif
#ifdef HERO_ENGINE_SUPPORT
	if(hero_mmi_msg_new_sms_ind((unsigned char *)content,data->length,(char*)num, type) )
	{
		result = HR_SUCCESS;
	}
#endif
	MMIHR_Trace(("res: %d", result));
	return result;
}

LOCAL int32 MMIHEROAPP_AudioGetSoundInfo(int32 cmd)
{
	int32 pos = -1;
	
	if(cmd == HR_ACI_TOTALTIME)
	{
		pos = s_hr_audio_mng.totalTime;
	}
	else
	{
		if(s_hr_audio_mng.curStatus == PLAYING)
		{
			MMISRVAUD_PLAY_INFO_T ptPlayInfo = {0};

			MMIHR_Trace(("mrapp MMIHEROAPP_AudioGetSoundInfo curAudioHdle: %d", s_hr_audio_mng.audioHdle));
			if(s_hr_audio_mng.audioHdle > INVALID_HANDLE)
			{
				MMISRVAUD_GetPlayingInfo(s_hr_audio_mng.audioHdle, &ptPlayInfo);
				MMIHR_Trace((".uiTotalTime: %d, CurrentTime: %d", ptPlayInfo.total_time, ptPlayInfo.cur_time));
				MMIHR_TraceM((".uiTotalDataLength: %d, Offset: %d", ptPlayInfo.total_data_length, ptPlayInfo.cur_data_offset));
				pos = ptPlayInfo.cur_data_offset;											
			}
		}
		else if(s_hr_audio_mng.curStatus == PAUSE
			|| s_hr_audio_mng.curStatus == LOADED)
		{
			pos = s_hr_audio_mng.curSetPlayPos;
		}
		
		if(pos != -1 && cmd == HR_ACI_PLAYTIME)
		{
			pos = (pos*((s_hr_audio_mng.totalTime<<18)/s_hr_audio_mng.totalLen))>>18;
		}
	}
	
	MMIHR_Trace(("MMIMRAPP_Audio, pos: %d", pos));
	return pos;
}

void MMIHEROAPP_AudioPlayEnd(void)
{
	s_hr_audio_mng.totalLen = -1;
	s_hr_audio_mng.totalTime = -1;
	s_hr_audio_mng.uiSampleRate = -1;
	s_hr_audio_mng.curSetPlayTime = 0;
}

PUBLIC void MMIHEROAPP_HandleAudioPlayCBMsg(
                                                MmiAudioCallbackMsgS *callback_info_ptr
                                                )
{
    BOOLEAN         result = FALSE;
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
    HAUDIO          haudiohandle = callback_info_ptr->hAudio;
    AUDIO_RESULT_E  audio_result = (AUDIO_RESULT_E)callback_info_ptr->affix_info;
    
    MMIHR_Trace(("MMIHEROAPP_HandleAudioPlayCBMsg haudiohandle = %u audio_result = %d msg_id = %d cb = 0x%08x loop = %d", haudiohandle, 
													audio_result, callback_info_ptr->notify_info, g_hr_audio_info.cb, g_hr_audio_info.loop));

    switch (callback_info_ptr->notify_info)
    {
    case AUDIO_PLAYEND_IND:  
		//播放结束或者播放失败后释放handle
		audio_handle = MMIHEROAPP_GetAudioHandle();
			
		if(audio_handle > INVALID_HANDLE)
		{
		    if(g_hr_audio_info.loop == 1) //解决不能循环播放的问题
            {
                break;
            }      
			MMISRVAUD_Stop(audio_handle);
			MMISRVMGR_Free(audio_handle);
			audio_handle = INVALID_HANDLE;
			MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);

    		//播放结束后应修改audio的状态
    		if(s_hr_audio_mng.curStatus == PLAYING)
            {
                s_hr_audio_mng.curStatus = LOADED;
            }
            
            if(AUDIO_NO_ERROR == audio_result)
            {            
                if(g_hr_audio_info.cb && (g_hr_audio_info.loop == 0))
                {
                    g_hr_audio_info.cb(HR_ACI_PLAY_COMPLETE);
                    MMIHR_Trace(("MMIHEROAPP_HandleAudioPlayCBMsg CallBack End"));
                }
                else
                {
                    hr_event(HR_AUDIO_EVENT, HR_ACI_PLAY_COMPLETE, 0);
                }
            }
            else
            {
                if(g_hr_audio_info.cb)
                {
                    g_hr_audio_info.cb(HR_ACI_PLAY_ERROR);
                }
                else
                {
                    hr_event(HR_AUDIO_EVENT, HR_ACI_PLAY_ERROR, 0);
                }
            }
		}
        break;

    default:
        break;
    }
}

LOCAL BOOLEAN MMIHEROAPP_AudioPlayCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;

    if(handle == MMIHEROAPP_GetAudioHandle() && PNULL != param)
    {
        MMIHR_Trace(("MMIHEROAPP_AudioPlayCallback event = %d", param->event));
        
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
            
        case MMISRVMGR_NOTIFY_RESUME:
            MMISRVAUD_SetVolume(handle, s_hr_audio_mng.soundVolume);
            break;
            
        case MMISRVMGR_NOTIFY_EXT:
//modify 20181114 begin
			MMISRVAUD_Stop(handle);
			MMISRVMGR_Free(handle);
			MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
    		//播放结束后应修改audio的状态
    		if(s_hr_audio_mng.curStatus == PLAYING)
            {
                s_hr_audio_mng.curStatus = LOADED;
            }
			if(g_hr_audio_info.cb && (g_hr_audio_info.loop == 0))
			{
				g_hr_audio_info.cb(HR_ACI_PLAY_COMPLETE);
			}
			else
			{
				hr_event(HR_AUDIO_EVENT, HR_ACI_PLAY_COMPLETE, 0);
			}
//modify 20181114 end
            break;
            
        default:
            break;
        }
    }
    return TRUE;

}

void hr_media_mma_init(void)
{
    int32 i = 0;

    g_hr_mma_ctrl_info.total_channel = 0;
    g_hr_mma_ctrl_info.play_channel  = 0;
    g_hr_mma_ctrl_info.hCodecHandle = 0;
    g_hr_mma_ctrl_info.hExpHandle   = 0;

    for(i=0; i<HR_MMA_CHANNEL_MAX; i++)
    {
        g_hr_mma_ctrl_info.mmaInfo[i].cur   = 0;
        g_hr_mma_ctrl_info.mmaInfo[i].len   = 0;
        g_hr_mma_ctrl_info.mmaInfo[i].loop  = 0;
        g_hr_mma_ctrl_info.mmaInfo[i].handle= -1;
        g_hr_mma_ctrl_info.mmaInfo[i].state = HR_MMA_INIT;
        if(g_hr_mma_ctrl_info.mmaInfo[i].dataBuf != NULL)
        {
            SCI_FREE(g_hr_mma_ctrl_info.mmaInfo[i].dataBuf);
            g_hr_mma_ctrl_info.mmaInfo[i].dataBuf = NULL;
        }
    }
}

int32 hr_media_mma_get_handle(void)
{
    int32 i = 0;
    int32 audioIndex = -1;

    for(i=0; i<HR_MMA_CHANNEL_MAX; i++)
    {
        if(g_hr_mma_ctrl_info.mmaInfo[i].handle == -1)
        {
            audioIndex = i;
            break;
        }
    }

    MMIHR_Trace(("[SKYAUDIO] hr_media_mma_get_handle audioHandle=%d", audioIndex));

    return audioIndex;
}

LOCAL void  HEROAUDIOMIX_Process(
                                    AUDIO_OBJECT_T *audio_obj_ptr,
                                    int16* psSrcLeftData,
                                    int16* psSrcRightData,
                                    uint32 uiSrcCount,
                                    int16* psDestLeftData,
                                    int16* psDestRightData,
                                    uint32* puiDestCount
                                    )
{
    int32 i = 1;
    int32 j = 0;
    int32 cur = 0;

    //MMIHR_Trace(("[SKYAUDIO] HEROAUDIOMIX_Process cur=%d len=%d uiSrcCount=%d play_channel=%d", g_hr_mma_ctrl_info.mmaInfo[i].cur, g_hr_mma_ctrl_info.mmaInfo[i].len, uiSrcCount, g_hr_mma_ctrl_info.play_channel));

    if(g_hr_mma_ctrl_info.play_channel == 1)
    {
    	SCI_MEMCPY((int16*)psDestLeftData, (int16*)psSrcLeftData, uiSrcCount*2);
    	SCI_MEMCPY((int16*)psDestRightData, (int16*)psSrcRightData, uiSrcCount*2);
    }
    else
    {
    	SCI_MEMCPY((int16*)psDestLeftData, (int16*)psSrcLeftData, uiSrcCount*2);
    	SCI_MEMCPY((int16*)psDestRightData, (int16*)psSrcRightData, uiSrcCount*2);
    	for (i = 1; i < HR_MMA_CHANNEL_MAX; i++)
        {
            //MMIHR_Trace(("[SKYAUDIO] HEROAUDIOMIX_Process i=%d cur=%d state=%d", i, g_hr_mma_ctrl_info.mmaInfo[i].cur, g_hr_mma_ctrl_info.mmaInfo[i].state));
            if(g_hr_mma_ctrl_info.mmaInfo[i].state != HR_MMA_PLAY)
                continue;
            
            cur = g_hr_mma_ctrl_info.mmaInfo[i].cur;
          
            for (j = 0; j < uiSrcCount; j++)
            {
                if(cur < g_hr_mma_ctrl_info.mmaInfo[i].len/2)
                {
            		psDestLeftData[j] = psDestLeftData[j] +g_hr_mma_ctrl_info.mmaInfo[i].dataBuf[cur];
                    psDestRightData[j]= psDestRightData[j]+g_hr_mma_ctrl_info.mmaInfo[i].dataBuf[cur];
                    if(psDestLeftData[j] > 32767)
                    {
                        psDestLeftData[j] = 32767;
                    }
                    if(psDestLeftData[j] < -32768)
                    {
                        psDestLeftData[j] = -32768;
                    }
                    cur++;
                    g_hr_mma_ctrl_info.mmaInfo[i].cur++;
                }
                else
                {
                    cur = 0;
                    g_hr_mma_ctrl_info.mmaInfo[i].cur = 0;
                    if(g_hr_mma_ctrl_info.mmaInfo[i].loop == 0)
                    {
                        g_hr_mma_ctrl_info.play_channel--;
                        g_hr_mma_ctrl_info.mmaInfo[i].state = HR_MMA_STOP;
                        break;
                    }
                }
            }
        }
    }
    *puiDestCount = uiSrcCount;
}


PUBLIC HAUDIOEXP HEROAUDIOMIX_RegExpPlugger( void )
{
#ifndef WIN32
	AUDIO_EXPRESS_T ptAudioExp = {0};
	HAUDIOEXP hAudioMix;
    
	ptAudioExp.pusExpName = ANSI2UINT16("AUDIOMIX");
	ptAudioExp.pusExpDescribe = SCI_NULL;
	ptAudioExp.process =  HEROAUDIOMIX_Process;
	ptAudioExp.set_para =  SCI_NULL;
	ptAudioExp.init_para = SCI_NULL;
	ptAudioExp.deinit_para = SCI_NULL;
	hAudioMix = AUDIO_PM_RegExpressPlugger(&ptAudioExp);
    MMIHR_Trace(("[SKYAUDIO] HEROAUDIOMIX_RegExpPlugger hAudioMix=%d", hAudioMix));

	return hAudioMix;
#endif
}

int32 hr_media_mma_open(int32 code ,uint8 *input,int32 input_len)
{ 
 	int32 ret = 0; 
    uint32 result = 0;
    uint32 audioIndex = -1;
    uint32 fHandle = 0;
    int32 len = 0;
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
    MMISRVAUD_RING_FMT_E audioType = MMISRVAUD_RING_FMT_MIDI;
    hr_bg_play_info_t *req = (hr_bg_play_info_t*)input;

	if((req == NULL)
	||(input_len < sizeof(hr_bg_play_info_t))
	||(req->data == NULL)
	||(req->len  == 0)
	)
    {   
		return HR_FAILED;
    }

    MMIHR_Trace(("[SKYAUDIO] hr_media_mma_open type=%d data=0x%08x len=%d loop=%d total_channel=%d", code%10, req->data, req->len, req->loop, g_hr_mma_ctrl_info.total_channel));

    if(g_hr_mma_ctrl_info.total_channel == 0)
    {
        hr_media_mma_init();
    }

    g_hr_mma_ctrl_info.total_channel++;

    audioIndex = hr_media_mma_get_handle();

    if(audioIndex == -1)
    {
        g_hr_mma_ctrl_info.total_channel--;
        return HR_FAILED;
    }

    if(g_hr_mma_ctrl_info.total_channel == 1)
    {
        switch(code%10)
        {
        case HR_ACI_AUDIO_MIDI:
            audioType = MMISRVAUD_RING_FMT_MIDI;
            g_hr_mma_ctrl_info.hCodecHandle = hMIDICodec;
            break;
        case HR_ACI_AUDIO_WAV:
            audioType = MMISRVAUD_RING_FMT_WAVE;
            g_hr_mma_ctrl_info.hCodecHandle = hWAVCodec;
            break;
        case HR_ACI_AUDIO_MP3:
            audioType = MMISRVAUD_RING_FMT_MP3;
            g_hr_mma_ctrl_info.hCodecHandle = hMP3Codec;
            break;
        case HR_ACI_AUDIO_AAC:
            audioType = MMISRVAUD_RING_FMT_AAC;
            g_hr_mma_ctrl_info.hCodecHandle = hAACCodec;
            break;
        case HR_ACI_AUDIO_AMR:
            audioType = MMISRVAUD_RING_FMT_AMR;
            g_hr_mma_ctrl_info.hCodecHandle = hAACCodec;
            break;
        default:
            audioType = MMISRVAUD_RING_FMT_WAVE;
            g_hr_mma_ctrl_info.hCodecHandle = hWAVCodec;
            break;
        }
        g_hr_mma_ctrl_info.hExpHandle = HEROAUDIOMIX_RegExpPlugger();    
        result = AUDIO_PM_AddCodecExpress(g_hr_mma_ctrl_info.hCodecHandle, g_hr_mma_ctrl_info.hExpHandle);
        MMIHR_Trace(("[SKYAUDIO] AUDIO_PM_AddCodecExpress result=%d", result));

		//创建handle之前需要将播放次数初始化一次
		s_hr_play_times = 1;
			
		if(req->loop == 1)
        {
            s_hr_play_times = 0xFFFFFFFF;
        }
		
        audio_handle = MMIHEROAPP_CreateAudioBufHandle(audioType, req->data, req->len, MMIHEROAPP_AudioPlayCallback);
        MMIHR_Trace(("[SKYAUDIO] MMIAUDIO_CreateAudioBufHandle handle = %d", audio_handle));

		if(audio_handle > INVALID_HANDLE)
		{
			MMIHEROAPP_SetMixAudioHandle(audio_handle);
		}
    }
    else
    {
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf = (int16*)SCI_ALLOCA(req->len);
        if(g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf == NULL)
        {
            SCI_MEMSET(g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf, 0x00, req->len);
            g_hr_mma_ctrl_info.total_channel--;
            return HR_FAILED;
        }
        SCI_MEMCPY((uint8*)g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf, req->data, req->len);
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].len = req->len;
    }

	g_hr_mma_ctrl_info.mmaInfo[audioIndex].loop = req->loop;
    g_hr_mma_ctrl_info.mmaInfo[audioIndex].handle = audioIndex;
    g_hr_mma_ctrl_info.mmaInfo[audioIndex].state  = LOADED;
        
	return (audioIndex+HR_PLAT_VALUE_BASE);
}


int32 hr_media_mma_play(int32 code ,uint8 *input,int32 input_len)
{
	uint32 audioIndex = 0;
    MMISRVAUD_RET_E  ret = MMISRVAUD_RET_MAX;
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;

    if(input != NULL)
    {
        audioIndex = *(int32*)input - HR_PLAT_VALUE_BASE;

        MMIHR_Trace(("[SKYAUDIO]: hr_media_mma_play audioHandle=%d total_channel=%d play_channel=%d", audioIndex, g_hr_mma_ctrl_info.total_channel, g_hr_mma_ctrl_info.play_channel));

        if(audioIndex < 0)
        {
            return HR_FAILED;
        }

        if(g_hr_mma_ctrl_info.total_channel <= 0)
        {
            return HR_FAILED;
        }

        if(g_hr_mma_ctrl_info.mmaInfo[audioIndex].state == HR_MMA_PLAY)
        {
            return HR_SUCCESS;
        }

        g_hr_mma_ctrl_info.play_channel++;
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].state = HR_MMA_PLAY;
        if(g_hr_mma_ctrl_info.total_channel == 1)
        {
			MMIAPISET_SetCurRingType(MMISET_RING_TYPE_OTHER);
			audio_handle = MMIHEROAPP_GetMixAudioHandle();
			MMIHR_Trace(("[SKYAUDIO]: MMIAUDIO_AudioPlay audio_handle = %d", audio_handle));

			if(audio_handle > INVALID_HANDLE)
			{
				ret = MMISRVAUD_Play(audio_handle, 0);
				MMISRVAUD_SetVolume(audio_handle, s_hr_audio_mng.soundVolume);
            	MMIHR_Trace(("[SKYAUDIO]: MMIAUDIO_AudioPlay ret=%d volume=%d", ret, s_hr_audio_mng.soundVolume));
			}
        }

    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_media_mma_stop(int32 code ,uint8 *input,int32 input_len)
{
    uint32 audioIndex = 0;
	MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;

    if(input != NULL)
    {
        audioIndex = *(int32*)input - HR_PLAT_VALUE_BASE;

        MMIHR_Trace(("[SKYAUDIO]: hr_media_mma_stop audioHandle=%d total_channel=%d play_channel=%d", audioIndex, g_hr_mma_ctrl_info.total_channel, g_hr_mma_ctrl_info.play_channel));

        if(audioIndex < 0)
        {
            return HR_FAILED;
        }

        if(g_hr_mma_ctrl_info.total_channel <= 0)
        {
            return HR_FAILED;
        }

        if(g_hr_mma_ctrl_info.mmaInfo[audioIndex].state == HR_MMA_PLAY)
        {
        	g_hr_mma_ctrl_info.play_channel--;
        	g_hr_mma_ctrl_info.mmaInfo[audioIndex].cur   = 0;
        	g_hr_mma_ctrl_info.mmaInfo[audioIndex].state = HR_MMA_STOP;
    	}
        
        if(g_hr_mma_ctrl_info.total_channel == 1)
        {
			audio_handle = MMIHEROAPP_GetMixAudioHandle();

			if(audio_handle > INVALID_HANDLE)
			{
			 	MMISRVAUD_Stop(audio_handle);
			 	MMISRVMGR_Free(audio_handle);
			}
        }

    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_media_mma_close(int32 code ,uint8 *input,int32 input_len)
{
    int32  ret = 0;
    uint32 audioIndex = 0;
    AUDIO_RESULT_E result = 0;

    if(input != NULL)
    {
        audioIndex = *(int32*)input - HR_PLAT_VALUE_BASE;

        MMIHR_Trace(("[SKYAUDIO]: hr_media_mma_close audioIndex=%d total_channel=%d", audioIndex, g_hr_mma_ctrl_info.total_channel));

        if(audioIndex < 0)
        {
            return HR_FAILED;
        }

        if(g_hr_mma_ctrl_info.total_channel <= 0)
        {
            return HR_FAILED;
        }

        g_hr_mma_ctrl_info.total_channel--;
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].state  = HR_MMA_CLOSE;
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].handle = -1;
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].len = 0;
        g_hr_mma_ctrl_info.mmaInfo[audioIndex].loop= 0;
        if(g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf != NULL)
        {
            SCI_FREE(g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf);
            g_hr_mma_ctrl_info.mmaInfo[audioIndex].dataBuf = NULL;
        }

        if(g_hr_mma_ctrl_info.total_channel == 0)
        {
            result = AUDIO_PM_UnRegExpressPlugger(g_hr_mma_ctrl_info.hExpHandle);
            MMIHR_Trace(("[SKYAUDIO] AUDIO_PM_UnRegExpressPlugger result=%d", result));
            
            result = AUDIO_PM_DeleteCodecExpress(g_hr_mma_ctrl_info.hCodecHandle, g_hr_mma_ctrl_info.hExpHandle);
            MMIHR_Trace(("[SKYAUDIO] AUDIO_PM_DeleteCodecExpress result=%d", result));
        }

    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }

}

LOCAL int32 MMIHEROAPP_AudioCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 resValue = HR_FAILED;
	int32 cmd;
	int type;
	MMIHR_TraceM(("MMIHEROAPP_AudioCmd() :%d", code));

    switch(code)
    {
#ifdef __HERO_CFG_CAMERA_SUPPORT__
    case HR_PLATEX_CAMERA_PREVIEW:
        return hr_camera_start_preview(input, input_len, output, output_len, cb);
    case HR_PLATEX_CAMERA_CAPTURE:
        return hr_camera_capture(input, input_len, output, output_len, cb);
    case HR_PLATEX_CAMERA_GETDATA:
        return hr_camera_get_preview_data(input, input_len, output, output_len, cb);
#endif

#ifdef __HR_RECORD_SUPPORT__
    case HR_VOICE_RECORD_START:
        {
        if(input_len < sizeof(HR_T_VOICE_REC_REQ)) /*lint !e574 !e737*/
            return HR_FAILED;
        return MMIHEROAPP_RecordStart((HR_T_VOICE_REC_REQ*)input);
        }
#endif

	default:
		break;
    }

	cmd = (code/10)%200;
	type = code%10;
	switch(type)
	{
	case HR_ACI_AUDIO_MIDI:
		type = MMISRVAUD_RING_FMT_MIDI;
		break;
	case HR_ACI_AUDIO_WAV:
		type = MMISRVAUD_RING_FMT_WAVE;
		break;
	case HR_ACI_AUDIO_MP3:
		type = MMISRVAUD_RING_FMT_MP3;
		break;       
	case HR_ACI_AUDIO_AMR:
		type = MMISRVAUD_RING_FMT_AMR;
		break;
	case HR_ACI_AUDIO_AAC:
		type = MMISRVAUD_RING_FMT_AAC;
		break;
	case HR_ACI_AUDIO_M4A:
		type = MMISRVAUD_RING_FMT_M4A;
		break;
	default:
		MMIHR_Trace(("type err !!! : %d", type));
		//return HR_IGNORE;
		break;
	}
	
	MMIHR_Trace(("s_hr_audio_mng.curStatus: %d, type :%d", s_hr_audio_mng.curStatus, type));	
	switch(cmd)
	{
	case HR_ACI_OPEN://设备初始化
		if(s_hr_audio_mng.curStatus > LOADED && s_hr_audio_mng.curStatus <= PAUSE)
		{
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
			audio_handle = MMIHEROAPP_GetAudioHandle();
			
			if(audio_handle > INVALID_HANDLE)
			{
				MMISRVAUD_Stop(audio_handle);
				MMISRVMGR_Free(audio_handle);
				audio_handle = INVALID_HANDLE;
				MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
			}
		}

		MMIHR_Trace(("s_hr_audio_mng.soundVolume: %d", s_hr_audio_mng.soundVolume));
		s_hr_audio_mng.curStatus = INITED;
		MMIHEROAPP_AudioPlayEnd();
		resValue = HR_SUCCESS;
		break;
        
	case HR_ACI_LOAD_FILE://加载一个文件
		if(s_hr_audio_mng.curStatus >= INITED && s_hr_audio_mng.curStatus <= PAUSE)
		{
			int len = strlen((char*)input);
			uint16 hr_full_path_len = 0;
			uint16 ucs2_name_len = 0;
			const uint8 *devName;
			uint16 devNameLen;
			
			MMIHR_Trace(("MMIHEROAPP_AudioCmd input_file: %s", (char*)input));
			if(len < MMI_HERO_APP_FILE_NAME_MAX_LEN)
			{
				devName = MMIHEROAPP_getCurDevName(&devNameLen);
				if(devName != NULL)
				{
					MMIHEROAPP_ConvertPath2Pc((char *)input, strlen((char*)input));
					memset(hr_full_path, 0, sizeof(hr_full_path));
					memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));
					ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)input, strlen((char*)input));
					if(HERO_MMIFILE_CombineFullPath_fix(
						(const uint8*)devName, devNameLen,
						(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
						(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
						(uint8*)hr_full_path,
						&hr_full_path_len
						)
						)
					{
						memset(s_hr_audio_filename, 0, sizeof(s_hr_audio_filename));
						s_hr_audio_mng.name = (uint8*)s_hr_audio_filename;
						SCI_MEMCPY(s_hr_audio_mng.name, hr_full_path, hr_full_path_len);
						s_hr_audio_mng.ring_type = type;
						s_hr_audio_mng.src_type = SRC_NAME;
						if(s_hr_audio_mng.curStatus != PAUSE)
						{
							s_hr_audio_mng.curStatus = LOADED;
						}
						resValue = HR_SUCCESS;
					}
				}
			}
		}
		break;
        
	case HR_ACI_LOAD_BUF://加载缓冲数据
		if(s_hr_audio_mng.curStatus >= INITED && s_hr_audio_mng.curStatus <= PAUSE && s_hr_audio_mng.curStatus != PAUSE)
		{	
			HRAPP_AUDIO_BUF_T *buf = (HRAPP_AUDIO_BUF_T*)input;
			MMIHR_Trace(("buf addr: 0x%x, input_len: %d ", buf->buf, buf->buf_len));
			
			s_hr_audio_mng.ring_type = type;
			s_hr_audio_mng.src_type = SRC_STREAM;
			s_hr_audio_mng.name = (uint8*)buf->buf;
			s_hr_audio_mng.dataLen = buf->buf_len;
			if(s_hr_audio_mng.curStatus != PAUSE)
			{
				s_hr_audio_mng.curStatus = LOADED;
			}
			resValue = HR_SUCCESS;
		}
		break;
        
	case HR_ACI_PLAY://从当前的位置开始播放
		{
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
			MMISRVAUD_PLAY_INFO_T ptPlayInfo;
			MMISRVAUD_RET_E play_res = MMISRVAUD_RET_MAX;
			MMIHR_Trace(("src_type: %d, ring_type: %d", s_hr_audio_mng.src_type, s_hr_audio_mng.ring_type));

			//每次播放的时候将播放次数初始化一次
			s_hr_play_times = 1;
			
			if(s_hr_audio_mng.curStatus == LOADED || s_hr_audio_mng.curStatus == PAUSE)
			{
                hr_audio_play_info_t *playReq = NULL;
                if(input_len == sizeof(hr_audio_play_info_t) && input != NULL)
                {
                    playReq = (hr_audio_play_info_t *)input;
                    g_hr_audio_info.cb = playReq->cb;
                    g_hr_audio_info.loop = playReq->loop;
                    g_hr_audio_info.block = playReq->block;
                    if(playReq->loop)
                    {
                        s_hr_play_times = 0xFFFFFFFF;
                    }                        
                }
			
				if(s_hr_audio_mng.src_type == SRC_STREAM)
				{						
					audio_handle = MMIHEROAPP_CreateAudioBufHandle(s_hr_audio_mng.ring_type, s_hr_audio_mng.name, s_hr_audio_mng.dataLen, MMIHEROAPP_AudioPlayCallback);
				}
				else
				{
					audio_handle = MMIHEROAPP_CreateAudioFileHandle(s_hr_audio_mng.ring_type, (const wchar *)s_hr_audio_mng.name, MMIHEROAPP_AudioPlayCallback);
				}
				
				MMIHR_Trace(("MMIHEROAPP_AudioCmd create audio_handle: %d", audio_handle));
				if(audio_handle > INVALID_HANDLE)
				{
					MMIHEROAPP_SetAudioHandle(audio_handle);
					MMIHR_TraceM(("MRAPP_AudioCmd play started pos: %d", s_hr_audio_mng.curSetPlayPos));
					play_res = MMISRVAUD_Play(audio_handle, s_hr_audio_mng.curSetPlayPos);
					MMIHR_Trace(("MRAPP_AudioCmd play_res: %d", play_res));

					if(MMISRVAUD_RET_OK == play_res)
					{
						MMISRVAUD_GetPlayingInfo(audio_handle, &ptPlayInfo);
						
						s_hr_audio_mng.curStatus = PLAYING;
						s_hr_audio_mng.totalLen = ptPlayInfo.total_data_length;
						s_hr_audio_mng.totalTime = ptPlayInfo.total_time;
						
						MMIHR_Trace(("MRAPP_AudioCmd audioHdle: %d", s_hr_audio_mng.audioHdle));
						MMIHR_Trace(("MRAPP_AudioCmd uiTotalTime: %d, CurrentTime: %d", ptPlayInfo.total_time, ptPlayInfo.cur_time));
						MMIHR_TraceM(("MRAPP_AudioCmd uiTotalDataLength: %d, Offset: %d", ptPlayInfo.total_data_length, ptPlayInfo.cur_data_offset));
						
						resValue = HR_SUCCESS;
					}
				}
				else
				{
					MMISRVMGR_Free(audio_handle);
					audio_handle = INVALID_HANDLE;
					MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
				}
			}
		}	
		break;
        
	case HR_ACI_PAUSE://暂停播放
#if 1
		if(s_hr_audio_mng.curStatus == PLAYING
			&& s_hr_audio_mng.audioHdle > INVALID_HANDLE)
		{
			MMISRVAUD_Pause(s_hr_audio_mng.audioHdle);
			s_hr_audio_mng.curStatus = PAUSE;
            resValue = HR_SUCCESS;
		}
		else
		{
			resValue = HR_FAILED;
		}
#else
		if(s_hr_audio_mng.curStatus == PLAYING)
		{
			BOOLEAN isPlaying;
			HAUDIO curAudioHdle = INVALID_HANDLE;
			
			isPlaying= MMIAUDIO_GetIsPlaying();
			MMIHR_Trace(("isPlaying: %d", isPlaying));
			if(isPlaying)
			{
				if(MRAPP_AUDIO_PLAYERTYPE == MMIAUDIO_GetAudioHtype())
				{
					curAudioHdle = MMIAUDIO_GetAudioHandle(MRAPP_AUDIO_PLAYERTYPE);
					MMIHR_Trace(("audioHdle: %d", curAudioHdle));
					if(s_hr_audio_mng.audioHdle == curAudioHdle)
					{
						AUDIO_PLAYINFO_T ptPlayInfo;
						MMIAUDIO_AudioGetPlayingFileInfo(MRAPP_AUDIO_PLAYERTYPE, &ptPlayInfo);				
						MMIAUDIO_AudioStop(MRAPP_AUDIO_PLAYERTYPE);
						MMIAUDIO_CloseAudioHandle(MRAPP_AUDIO_PLAYERTYPE);
						s_hr_audio_mng.curStatus = PAUSE;
						s_hr_audio_mng.curSetPlayPos = ptPlayInfo.uiCurrentDataOffset;
						MMIHR_TraceM(("pause Pos: %d", s_hr_audio_mng.curSetPlayPos));
						s_hr_audio_mng.audioHdle = INVALID_HANDLE;	
						resValue = HR_SUCCESS;
					}						
				}		
			}
			MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_NES, TRUE);
		}
#endif
		break;

	case HR_ACI_RESUME://继续播放
#if 1
		if(s_hr_audio_mng.curStatus == PAUSE
			&& s_hr_audio_mng.audioHdle > INVALID_HANDLE)
		{
			MMISRVAUD_SetVolume(s_hr_audio_mng.audioHdle, s_hr_audio_mng.soundVolume);
			MMISRVAUD_Resume(s_hr_audio_mng.audioHdle);
			s_hr_audio_mng.curStatus = PLAYING;
            resValue = HR_SUCCESS;
		}
		else
		{
			resValue = HR_FAILED;
		}
#else
		if(s_hr_audio_mng.curStatus == PAUSE)
		{
			resValue = MMIHEROAPP_AudioCmd(2043, NULL, 0, NULL, NULL, NULL);
			resValue = HR_SUCCESS;
		}
#endif
		break;

	case HR_ACI_STOP://停止播放
		{
			BOOLEAN stop_res = FALSE;
			BOOLEAN close_res = FALSE;
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
			audio_handle = MMIHEROAPP_GetAudioHandle();
			MMIHR_Trace(("audioHdle: %d", audio_handle));
			MMIHR_Trace(("audioStatus: %d", s_hr_audio_mng.curStatus));
		
			if(s_hr_audio_mng.curStatus >= PLAYING && s_hr_audio_mng.curStatus <= PAUSE)
			{
				if(audio_handle > INVALID_HANDLE)
				{
					stop_res = MMISRVAUD_Stop(audio_handle);
					close_res= MMISRVMGR_Free(audio_handle);
					audio_handle = INVALID_HANDLE;
					MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
					MMIHR_Trace(("audio_stop stop_res: %d, close_res : %d", audio_handle));
					
					if(stop_res && close_res)
					{
						s_hr_audio_mng.curSetPlayPos = 0;
						s_hr_audio_mng.curStatus = LOADED;
						MMIHEROAPP_AudioPlayEnd();
						resValue = HR_SUCCESS;
					}
				}
			}
		}
		break;
        
	case HR_ACI_CLOSE://关闭设备
		if(s_hr_audio_mng.curStatus >= INITED && s_hr_audio_mng.curStatus <= PAUSE)
		{
			if(s_hr_audio_mng.curStatus >= PLAYING && s_hr_audio_mng.curStatus <= PAUSE)
			{
				MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
				audio_handle = MMIHEROAPP_GetAudioHandle();
				
				if(audio_handle > INVALID_HANDLE)
				{
					MMISRVAUD_Stop(audio_handle);
					MMISRVMGR_Free(audio_handle);
					audio_handle = INVALID_HANDLE;
					MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
				}
			}
			s_hr_audio_mng.curStatus = IDLE;
			MMIHEROAPP_AudioPlayEnd();
			resValue = HR_SUCCESS;
		}
		break;
        
	case HR_ACI_STATUS://获取当前设备的状态
		MMIHR_Trace(("audio get_status : %d", s_hr_audio_mng.curStatus));
		
		resValue = HR_PLAT_VALUE_BASE + s_hr_audio_mng.curStatus;
		break;
        
	case HR_ACI_POSITION_TIME://设置播放位置，已经播放了多少时间
	case HR_ACI_POSITION_LEN://设置播放位置	
		MMIHR_Trace(("total len:%d total time: %d", s_hr_audio_mng.totalLen, s_hr_audio_mng.totalTime));
#if 1 //modify 20181228
		{
			BOOLEAN res = FALSE;
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
			uint32 len = ((HRAPP_AUDIO_OFFSET_T*)input)->pos;

			audio_handle = MMIHEROAPP_GetAudioHandle();
			MMIHR_Trace(("audio audio_handle %d", audio_handle));

			if(audio_handle > INVALID_HANDLE)
			{
				res = MMISRVAUD_SeekByTime(s_hr_audio_mng.audioHdle, len);
				MMIHR_Trace(("result: %d", res));
				if(res)
				{
					resValue = HR_SUCCESS;
				}
				else
				{
					resValue = HR_FAILED;
				}
			}
		}
#else
		if(input != NULL && s_hr_audio_mng.curStatus >= LOADED && s_hr_audio_mng.curStatus <= SUSPENDED)
		{
			BOOLEAN stop_res = FALSE;
			BOOLEAN close_res = FALSE;
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;	
			uint32 len = ((HRAPP_AUDIO_OFFSET_T*)input)->pos;
			audio_handle = MMIHEROAPP_GetAudioHandle();
			if(s_hr_audio_mng.curStatus != LOADED && s_hr_audio_mng.curStatus != SUSPENDED)
			{
				MMIHR_Trace(("current status: %d", s_hr_audio_mng.curStatus));	
				if(audio_handle > INVALID_HANDLE)
				{
					stop_res = MMISRVAUD_Stop(audio_handle);
					close_res = MMISRVMGR_Free(audio_handle);
					audio_handle = INVALID_HANDLE;
					MMIHEROAPP_SetAudioHandle(INVALID_HANDLE);
				}
			}

			if(stop_res && close_res)
			{
				if(cmd == HR_ACI_POSITION_TIME)
				{
    				if(s_hr_audio_mng.totalTime <= 0)
                	{
                    	return HR_FAILED;
                	}         
					len = (len*((s_hr_audio_mng.totalLen<<8)/s_hr_audio_mng.totalTime))>>8;
				}
			
				s_hr_audio_mng.curSetPlayPos = len;
				s_hr_audio_mng.curStatus = LOADED;
				MMIHR_Trace(("uiOffset: %d, input: %d", s_hr_audio_mng.curSetPlayPos, ((HRAPP_AUDIO_OFFSET_T*)input)->pos));
				resValue = HR_SUCCESS;
			}
		}
#endif
		break;
        
	case HR_ACI_TOTALTIME://获取整首歌的播放时间			
		{
			BOOLEAN res = FALSE;
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
            MMISRVAUD_PLAY_INFO_T   audio_info = {0};
            HRAPP_AUDIO_OFFSET_T *total_info = (HRAPP_AUDIO_OFFSET_T*)hr_tempTransBuf;
            SCI_MEMSET(total_info, 0x00, sizeof(HRAPP_AUDIO_OFFSET_T));
			if(output != NULL && output_len != NULL)
            {
                if(s_hr_audio_mng.curStatus == LOADED)
                {
    				if(s_hr_audio_mng.src_type == SRC_STREAM)
    				{						
    					audio_handle = MMIHEROAPP_CreateAudioBufHandle(s_hr_audio_mng.ring_type, s_hr_audio_mng.name, s_hr_audio_mng.dataLen, MMIHEROAPP_AudioPlayCallback);
    				}
    				else
    				{
    					audio_handle = MMIHEROAPP_CreateAudioFileHandle(s_hr_audio_mng.ring_type, (const wchar *)s_hr_audio_mng.name, MMIHEROAPP_AudioPlayCallback);
    				}
                    
                    if(audio_handle > INVALID_HANDLE)
                    {
						MMIHEROAPP_setSoundVolume(s_hr_audio_mng.soundVolume);
						MMISRVAUD_Play(audio_handle, 0);
            			res= MMISRVAUD_GetPlayingInfo(audio_handle, &audio_info);

						if(res)
						{
							MMISRVAUD_Stop(audio_handle);
                        	MMISRVMGR_Free(audio_handle);
							audio_handle= INVALID_HANDLE;
            				MMIHR_Trace(("uiTotalDataLength: %d, uiTotalTime: %d", audio_info.total_data_length, audio_info.total_time));
        					total_info->pos = audio_info.total_time;
        					*output = (uint8*)total_info;
        					*output_len = sizeof(HRAPP_AUDIO_OFFSET_T);
        					resValue = HR_SUCCESS;
						}
                    }
                }
                else if(s_hr_audio_mng.curStatus >= PLAYING)
                {
                    total_info->pos = s_hr_audio_mng.totalTime;
      				*output = (uint8*)total_info;
    				*output_len = sizeof(HRAPP_AUDIO_OFFSET_T);
                    MMIHR_Trace(("uiTotalDataLength: %d, uiTotalTime: %d", s_hr_audio_mng.totalLen, s_hr_audio_mng.totalTime));
    				resValue = HR_SUCCESS;
                }
			}
		}
        break;
        
	case HR_ACI_PLAYTIME://获取当前的播放进度	
    	if(output != NULL && output_len != NULL)
        {   
    		if(s_hr_audio_mng.curStatus >= PLAYING)
    		{
				MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;	
                HRAPP_AUDIO_OFFSET_T *cur_info = (HRAPP_AUDIO_OFFSET_T *)hr_tempTransBuf;
                MMISRVAUD_PLAY_INFO_T ptPlayInfo = {0};
                SCI_MEMSET(cur_info, 0x00, sizeof(HRAPP_AUDIO_OFFSET_T));
				
				audio_handle = MMIHEROAPP_GetAudioHandle();
				MMIHR_Trace(("mmimrapp audio audio_handle %d", audio_handle));
				 
				if(audio_handle > INVALID_HANDLE)
				{
                	MMISRVAUD_GetPlayingInfo(s_hr_audio_mng.audioHdle, &ptPlayInfo);
                	MMIHR_Trace(("cur time: %d offset: %d", ptPlayInfo.cur_time, ptPlayInfo.cur_data_offset));
                	cur_info->pos = ptPlayInfo.cur_time;
                	*output = (uint8*)cur_info;
                	resValue = HR_SUCCESS;
				}
    		}	
            else
            {
                HRAPP_AUDIO_OFFSET_T *cur_info = (HRAPP_AUDIO_OFFSET_T *)hr_tempTransBuf;
                SCI_MEMSET(cur_info, 0x00, sizeof(HRAPP_AUDIO_OFFSET_T));
                cur_info->pos = 0;
                *output = (uint8*)cur_info;
                resValue = HR_SUCCESS;
            }
        }
        break;

       case HR_ACI_PLAYTIME_MS://获取当前的播放进度(ms)
        	if(output != NULL && output_len != NULL)
            {   
        		if(s_hr_audio_mng.curStatus >= PLAYING)
        		{
					MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
                    HRAPP_AUDIO_OFFSET_T *cur_info = (HRAPP_AUDIO_OFFSET_T *)hr_tempTransBuf;
                    MMISRVAUD_PLAY_INFO_T ptPlayInfo = {0};
                    BOOLEAN result = AUDIO_NO_ERROR;
                    SCI_MEMSET(cur_info, 0x00, sizeof(HRAPP_AUDIO_OFFSET_T));

					audio_handle = MMIHEROAPP_GetAudioHandle();
					MMIHR_Trace(("mmimrapp audio audio_handle %d", audio_handle));

					if(audio_handle > INVALID_HANDLE)
					{
                    	MMISRVAUD_GetPlayingInfo(s_hr_audio_mng.audioHdle, &ptPlayInfo);
                    	MMIHR_Trace(("cur time: %d offset: %d", ptPlayInfo.cur_time, ptPlayInfo.cur_data_offset));
                    	cur_info->pos = ptPlayInfo.cur_time*1000;
                    	*output = (uint8*)cur_info;
                    	resValue = HR_SUCCESS;
					}
        		}
                else
                {
                    HRAPP_AUDIO_OFFSET_T *cur_info = (HRAPP_AUDIO_OFFSET_T *)hr_tempTransBuf;
                    SCI_MEMSET(cur_info, 0x00, sizeof(HRAPP_AUDIO_OFFSET_T));
                    cur_info->pos = 0;
                    *output = (uint8*)cur_info;
                    resValue = HR_SUCCESS;
                }
            }
            break;

    case HR_ACI_SETTIME_MS:
        {
			BOOLEAN res = FALSE;
			MMISRV_HANDLE_T audio_handle = INVALID_HANDLE;
            uint32 len = ((HRAPP_AUDIO_OFFSET_T*)input)->pos;

			audio_handle = MMIHEROAPP_GetAudioHandle();
			MMIHR_Trace(("audio audio_handle %d", audio_handle));

			if(audio_handle > INVALID_HANDLE)
			{
				res = MMISRVAUD_SeekByTime(s_hr_audio_mng.audioHdle, len);
            	MMIHR_Trace(("result: %d", res));
			
            	if(res)
            	{
                	resValue = HR_SUCCESS;
            	}
            	else
            	{
                	resValue = HR_FAILED;
            	}
			}
        }
        break;
		
	case HR_ACI_PLAYLEN://获取当前的播放进度
		{
			int32 len = -1;			
			len = MMIHEROAPP_AudioGetSoundInfo(cmd);
			
			MMIHR_Trace(("pos: %d", len));
			if(len != -1 && output != NULL && output_len != NULL)
			{
				s_hr_audio_mng.curDataPos.pos = len;
				*output = (uint8*)&s_hr_audio_mng.curDataPos;
				*output_len = sizeof(HRAPP_AUDIO_OFFSET_T);
				resValue = HR_SUCCESS;
			}
		}
		break;

    case HR_ACI_MULTIPATH_OPEN:
        return hr_media_mma_open(code, input, input_len);
        
    case HR_ACI_MULTIPATH_PLAY:
        return hr_media_mma_play(code, input, input_len);
        
    case HR_ACI_MULTIPATH_STOP:
        return hr_media_mma_stop(code, input, input_len);
        
    case HR_ACI_MULTIPATH_CLOSE:
        return hr_media_mma_close(code, input, input_len);

	default:
		resValue = HR_IGNORE;//命令不支持
		break;
	}
	
	MMIHR_TraceM(("MMIHEROAPP_AudioCmd().resValue: %d", resValue));
	return resValue;
}

#ifdef HERO_APP_SIMUGAME_OPEN
void MMIHEROAPP_SetAudioDevice(void)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = 0;
    AUDIO_DEVICE_MODE_TYPE_E set_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;

    if(MMISRVAUD_IsHeadSetPlugIn())
    {
        set_dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
    }
    else
    {
        set_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    
    if(AUDIO_GetDevMode(&cur_dev_mode) == AUDIO_NO_ERROR) 
    {
        if(set_dev_mode != cur_dev_mode)
        {
            result = AUDIO_SetDevMode(set_dev_mode);
        }
    }
}
LOCAL int32 MMIHEROAPP_PcmCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 cmd, resValue = HR_SUCCESS;
	MMIHR_TraceM(("MMIHEROAPP_PcmCmd() :%d, curStatus: %d", code,s_hr_pcm_mng.curStatus));
	
#ifndef WIN32
#ifdef EXTPCM	
	cmd = (code/10)%200;
	switch(cmd)
	{
	case HR_ACI_OPEN://设备初始化
		if(s_hr_pcm_mng.curStatus == IDLE)
		{
			ExtPCM_SoundInit(15*1024, 8000);
			ExtPCM_SoundOpen(15*1024, 8000);
			s_hr_pcm_mng.curStatus = PLAYING;
		}	
		break;
	case HR_ACI_LOAD_BUF://加载缓冲数据
		if(s_hr_pcm_mng.curStatus == PLAYING && input != NULL)
		{	
			HRAPP_AUDIO_BUF_T *buf = (HRAPP_AUDIO_BUF_T*)input;
			MMIHR_Trace(("buf addr: 0x%x, input_len: %d ", buf->buf, buf->buf_len));
			MMIHEROAPP_SetAudioDevice(); //add by zack@20120711
			ExtPCM_SoundOutput(buf->buf_len/2, (int16 *)buf->buf);
			s_hr_pcm_mng.curStatus = PLAYING;
		}
		else
		{
			resValue = HR_FAILED;
		}				
		break;
	case HR_ACI_PLAY://从当前的位置开始播放
		//do nothing
		break;
	case HR_ACI_STOP://停止播放
	case HR_ACI_CLOSE://关闭设备
		if(s_hr_pcm_mng.curStatus == PLAYING)
		{
			ExtPCM_SoundClose();
			s_hr_pcm_mng.curStatus = IDLE;
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
	case HR_ACI_STATUS://获取当前设备的状态				
		resValue = HR_PLAT_VALUE_BASE + s_hr_pcm_mng.curStatus;			
		break;
	case HR_ACI_PLAYLEN://获取还剩多少数据没有播放
		if(s_hr_pcm_mng.curStatus == PLAYING && output != NULL && output_len != NULL)
		{
			s_hr_pcm_mng.dataPos.pos = 2*ExtPCM_SoundGetPCMNum();//get left bytes
			MMIHR_Trace(("left: %d", s_hr_pcm_mng.dataPos.pos));
			*output = (uint8*)&s_hr_pcm_mng.dataPos;
			*output_len = sizeof(HRAPP_AUDIO_OFFSET_T);
			*cb = (HR_PLAT_EX_CB)NULL;		
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
	default:
		resValue = HR_IGNORE;//命令不支持
		break;
	}	
#endif
#endif
	
	MMIHR_TraceM(("MMIHEROAPP_PcmCmd(). code: %d, resValue: %d", code, resValue));
	return resValue;
}
#endif
LOCAL int32 MMIHEROAPP_SetMp3AsRing(HR_RING_SET_T *info)
{
	int32 resValue = HR_FAILED;
	SFS_DATE_T modify_date;
	SFS_TIME_T modify_time;
	
	return resValue;
}

int32 MMHEROAPP_FileMgrInit(void)
{
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        return HR_FAILED;
    }
    HERO_FMGR_FILTER_INIT(s_hr_file_filter_type);
    return HR_SUCCESS;
}

int32 MMHEROAPP_FileMgrSetFilter(int32 type)
{
    int32  ret = HR_SUCCESS;
    uint32 plat_file_type = 0;
    MMIHR_Trace(("mrapp MMHEROAPP_FileMgrSetFilter type:%d", type));

    switch(type)
    {
	case HR_FMGR_TYPE_ALL:
        plat_file_type = MMIFMM_FILE_ALL;
		break;
	case HR_FMGR_TYPE_FOLDER:
        plat_file_type = MMIFMM_FILE_ALL;
		break;
	case HR_FMGR_TYPE_BMP:
        plat_file_type = MMIFMM_PICTURE_BMP;
		break;
	case HR_FMGR_TYPE_JPG:
        plat_file_type = MMIFMM_PICTURE_JPG;
		break;
	case HR_FMGR_TYPE_JPEG:
        plat_file_type = MMIFMM_PICTURE_JPG;
		break;
	case HR_FMGR_TYPE_GIF:
        plat_file_type = MMIFMM_PICTURE_GIF;
		break;
	case HR_FMGR_TYPE_PNG:
        plat_file_type = MMIFMM_PICTURE_PNG;
		break;
	case HR_FMGR_TYPE_WBMP:
         plat_file_type = MMIFMM_PICTURE_WBMP;
		break;
	case HR_FMGR_TYPE_MID:
        plat_file_type = MMIFMM_MUSIC_MID;
		break;
	case HR_FMGR_TYPE_MIDI:
        plat_file_type = MMIFMM_MUSIC_MIDI;
		break;
	case HR_FMGR_TYPE_WAV:
        plat_file_type = MMIFMM_MUSIC_WAV;
		break;
	case HR_FMGR_TYPE_AMR:
        plat_file_type = MMIFMM_MUSIC_AMR;
		break;
	case HR_FMGR_TYPE_AAC:
        plat_file_type = MMIFMM_MUSIC_ACC;
		break;
	case HR_FMGR_TYPE_WMA:
        plat_file_type = MMIFMM_MUSIC_WMA;
		break;
	case HR_FMGR_TYPE_3GP:
        plat_file_type = MMIFMM_MOVIE_3GP;
		break;
	case HR_FMGR_TYPE_MP4:
        plat_file_type = MMIFMM_MOVIE_MP4;
		break;
	case HR_FMGR_TYPE_AVI:	
        plat_file_type = MMIFMM_MOVIE_AVI;
		break;
	case HR_FMGR_TYPE_JAD:
        plat_file_type = MMIFMM_JAVA_JAD;
		break;
	case HR_FMGR_TYPE_JAR:
        plat_file_type = MMIFMM_JAVA_JAR;
		break;
	case HR_FMGR_TYPE_TOO:
        plat_file_type = MMIFMM_HERO_TOO;
		break;
	case HR_FMGR_TYPE_DAF:
	case HR_FMGR_TYPE_VM:
	case HR_FMGR_TYPE_AWB:
	case HR_FMGR_TYPE_AIF:
	case HR_FMGR_TYPE_AIFF:
	case HR_FMGR_TYPE_AIFC:
	case HR_FMGR_TYPE_AU:
	case HR_FMGR_TYPE_SND:
	case HR_FMGR_TYPE_M4A:
	case HR_FMGR_TYPE_MMF:
	case HR_FMGR_TYPE_WBM:
	case HR_FMGR_TYPE_IMY:
	case HR_FMGR_TYPE_VCF:
	case HR_FMGR_TYPE_VCS:
	case HR_FMGR_TYPE_THEME:
#ifndef HERO_APP_SIMUGAME_OPEN
	case HR_FMGR_TYPE_NES:
#endif
	case HR_FMGR_TYPE_ZIP:	
	case HR_FMGR_TYPE_ZPK:
	case HR_FMGR_TYPE_EMS:
	case HR_FMGR_TYPE_ANM:
	case HR_FMGR_TYPE_FOLDER_DOT:
	case HR_FMGR_TYPE_UNKNOW:
        return HR_IGNORE;
	default:
		break;
	}

    HERO_FMGR_FILTER_SET(s_hr_file_filter_type, plat_file_type);

    return ret;
}

int32 MMHEROAPP_FileMgrCleanFilter(int32 type)
{
    int32  ret = HR_SUCCESS;
    uint32 plat_file_type = 0;
    MMIHR_Trace(("mrapp MMHEROAPP_FileMgrCleanFilter type:%d", type));

    switch(type)
    {
	case HR_FMGR_TYPE_ALL:
        plat_file_type = MMIFMM_FILE_ALL;
		break;
	case HR_FMGR_TYPE_BMP:
        plat_file_type = MMIFMM_PICTURE_BMP;
		break;
	case HR_FMGR_TYPE_JPG:
        plat_file_type = MMIFMM_PICTURE_JPG;
		break;
	case HR_FMGR_TYPE_JPEG:
        plat_file_type = MMIFMM_PICTURE_JPG;
		break;
	case HR_FMGR_TYPE_GIF:
        plat_file_type = MMIFMM_PICTURE_GIF;
		break;
	case HR_FMGR_TYPE_PNG:
        plat_file_type = MMIFMM_PICTURE_PNG;
		break;
	case HR_FMGR_TYPE_WBMP:
         plat_file_type = MMIFMM_PICTURE_WBMP;
		break;
	case HR_FMGR_TYPE_MID:
        plat_file_type = MMIFMM_MUSIC_MID;
		break;
	case HR_FMGR_TYPE_MIDI:
        plat_file_type = MMIFMM_MUSIC_MIDI;
		break;
	case HR_FMGR_TYPE_WAV:
        plat_file_type = MMIFMM_MUSIC_WAV;
		break;
	case HR_FMGR_TYPE_AMR:
        plat_file_type = MMIFMM_MUSIC_AMR;
		break;
	case HR_FMGR_TYPE_AAC:
        plat_file_type = MMIFMM_MUSIC_ACC;
		break;
	case HR_FMGR_TYPE_WMA:
        plat_file_type = MMIFMM_MUSIC_WMA;
		break;
	case HR_FMGR_TYPE_3GP:
        plat_file_type = MMIFMM_MOVIE_3GP;
		break;
	case HR_FMGR_TYPE_MP4:
        plat_file_type = MMIFMM_MOVIE_MP4;
		break;
	case HR_FMGR_TYPE_AVI:	
        plat_file_type = MMIFMM_MOVIE_AVI;
		break;
	case HR_FMGR_TYPE_JAD:
        plat_file_type = MMIFMM_JAVA_JAD;
		break;
	case HR_FMGR_TYPE_JAR:
        plat_file_type = MMIFMM_JAVA_JAR;
		break;
	case HR_FMGR_TYPE_TOO:
        plat_file_type = MMIFMM_HERO_TOO;
		break;
	case HR_FMGR_TYPE_FOLDER:
      	return HR_SUCCESS;
	case HR_FMGR_TYPE_DAF:
	case HR_FMGR_TYPE_VM:
	case HR_FMGR_TYPE_AWB:
	case HR_FMGR_TYPE_AIF:
	case HR_FMGR_TYPE_AIFF:
	case HR_FMGR_TYPE_AIFC:
	case HR_FMGR_TYPE_AU:
	case HR_FMGR_TYPE_SND:
	case HR_FMGR_TYPE_M4A:
	case HR_FMGR_TYPE_MMF:
	case HR_FMGR_TYPE_WBM:
	case HR_FMGR_TYPE_IMY:
	case HR_FMGR_TYPE_VCF:
	case HR_FMGR_TYPE_VCS:
	case HR_FMGR_TYPE_THEME:
#ifndef HERO_APP_SIMUGAME_OPEN
	case HR_FMGR_TYPE_NES:
#endif
	case HR_FMGR_TYPE_ZIP:	
	case HR_FMGR_TYPE_ZPK:
	case HR_FMGR_TYPE_EMS:
	case HR_FMGR_TYPE_ANM:
	case HR_FMGR_TYPE_FOLDER_DOT:
	case HR_FMGR_TYPE_UNKNOW:
        return HR_IGNORE;
	default:
		break;
	}

    HERO_FMGR_FILTER_CLEAR(s_hr_file_filter_type, plat_file_type);

    return ret;
}

int32 MMHEROAPP_FileMgrBrowser(HR_T_FMGR_REQ *input)
{
    int32 i = 0;
    int32 ret = 0;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName = NULL;
	uint16 devNameLen = 0;
    MMIFMM_FILTER_T filter = {0};
    uint16  path_ucs2[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  find_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  find_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if(NULL == input)
    {
        return HR_FAILED;
    }

    switch(input->list_path[0])
    {
    case 'a':
    case 'A':
    	input->list_path[0] = 'C';
        break;
    case 'b':
    case 'B':
    	input->list_path[0] = 'D';
        break;
    case 'c':
    case 'C':
#ifdef DUAL_TCARD_SUPPORT
        if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
        {
        	input->list_path[0] = 'E';
        }
        else if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
        	input->list_path[0] = 'F';
        }
#else
    	input->list_path[0] = 'E';
#endif
        break;
	default:
		break;
    }
    MMIHR_Trace(("mrapp MMHEROAPP_FileMgrBrowser type:%d path:%s", input->dft_list, input->list_path));
    
    switch(input->dft_list)
    {
    case HR_DSPL_IMAGE:
#if (HERO_SPR_VERSION >= 0x10A1140)
		if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#else
		if(MMIAPISD_GetStatus())
#endif
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN,
               MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
               PNULL, 0,
               find_path, &find_path_len);
        }
#ifdef DUAL_TCARD_SUPPORT
        else if (TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN,
               MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
               PNULL, 0,
               find_path, &find_path_len);
        }
#endif
        else if (MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN,
               MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
               PNULL, 0,
               find_path, &find_path_len);
        }
        break;
        
    case HR_DSPL_AUDIO:
#if (HERO_SPR_VERSION >= 0x10A1140)
		if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#else
		if(MMIAPISD_GetStatus())
#endif
		{
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN,
               MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
               PNULL, 0,
               find_path, &find_path_len);
        }
#ifdef DUAL_TCARD_SUPPORT
        else if (TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN,
               MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
               PNULL, 0,
               find_path, &find_path_len);
        }
#endif
        else if (MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN,
               MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
               PNULL, 0,
               find_path, &find_path_len);
        }
        break;
        
    case HR_DSPL_VIDEO:
#if (HERO_SPR_VERSION >= 0x10A1140)
		if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#else
		if(MMIAPISD_GetStatus())
#endif
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN,
               MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
               PNULL, 0,
               find_path, &find_path_len);
        }
#ifdef DUAL_TCARD_SUPPORT
        else if (TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN,
               MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
               PNULL, 0,
               find_path, &find_path_len);
        }
#endif
        else if (MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN,
               MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
               PNULL, 0,
               find_path, &find_path_len);
        }
        break;
        
    case HR_DSPL_NES:
        ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)path_ucs2, (const uint8 *)HERO_ENGINE_NES_DIR_NAME, strlen(HERO_ENGINE_NES_DIR_NAME));
#if (HERO_SPR_VERSION >= 0x10A1140)
		if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#else
		if(MMIAPISD_GetStatus())
#endif
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN,
               path_ucs2, ucs2_name_len/2,
               PNULL, 0,
               find_path, &find_path_len);
        }
#ifdef DUAL_TCARD_SUPPORT
        else if (TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN,
               path_ucs2, ucs2_name_len/2,
               PNULL, 0,
               find_path, &find_path_len);
        }
#endif
        else if (MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
        {
            MMIFILE_CombineFullPath(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN,
               path_ucs2, ucs2_name_len/2,
               PNULL, 0,
               find_path, &find_path_len);
        }
        break;
        
    case HR_DSPL_NONE:
    default:
        MMIHEROAPP_ConvertPath2Pc(input->list_path, strlen(input->list_path));
    	find_path_len = MMIHEROAPP_GB2UCS((uint16*)find_path, (const uint8 *)input->list_path, strlen(input->list_path));
        find_path_len = find_path_len/2;
        break;
    }

	if(s_hr_file_filter_type == MMIFMM_FILE_ALL)
	{
		SCI_MEMCPY(filter.filter_str, "*.*", strlen("*.*")); /*lint !e666*/
	}
	else
	{
		MMIFMM_GetFilerInfo(&filter, s_hr_file_filter_type);
	}
    MMIHR_Trace(("filter:%s find_path_len: %d", filter.filter_str, find_path_len));
	
	if(find_path_len>3)
	{
		ret = MMIFMM_OpenFileWinByLayer(find_path, find_path_len, &filter, FUNC_FIND_FILE_AND_ALLFOLER, FALSE, MMIHEROAPP_HandleFileBrowserWinMsg, NULL, NULL, FALSE);
	}
	else
	{
		ret = MMIFMM_OpenFileWinByLayer(NULL, 0, &filter, FUNC_FIND_FILE_AND_ALLFOLER, FALSE, MMIHEROAPP_HandleFileBrowserWinMsg, NULL, NULL, FALSE);
	}
	MMIHR_Trace(("fileMgrBroswer openWin ret : %d", ret));
	
    if(FMM_OPENWIN_SUCCESS != ret)
    {
        return HR_FAILED;
    }
    else
    {
        return HR_SUCCESS;
    }
}

int32 MMHEROAPP_FileMgrGetPath(uint8 **output, int32 *output_len)
{
    if(output == NULL || output_len == NULL)
    {
        return HR_FAILED;
    }
    else
    {
        switch(g_hr_filebrowser_path[0])
        {
        case 'c':
        case 'C':
            g_hr_filebrowser_path[0] = 'A';
            break;

        case 'd':
        case 'D':
            g_hr_filebrowser_path[0] = 'B';
            break;

        case 'e':
        case 'E':
#ifdef DUAL_TCARD_SUPPORT
        case 'f':
        case 'F':
#endif
            g_hr_filebrowser_path[0] = 'C';
            break;
		default:
		break;
        }
        MMIHEROAPP_ConvertPath2Loc((char*)g_hr_filebrowser_path, strlen((char*)g_hr_filebrowser_path));
        *output = g_hr_filebrowser_path;
        *output_len = strlen((char*)g_hr_filebrowser_path);
        return HR_SUCCESS;
    }
}

int32 MMHEROAPP_FileMgrExit(void)
{
    s_hr_file_filter_type = 0;
    SCI_MEMSET(g_hr_filebrowser_path, 0x00, sizeof(g_hr_filebrowser_path));

    return HR_SUCCESS;
}

void heroGB2UCS2(char *src, char *dest)
{
    uint16 len = 0;
    
    if(NULL != src && NULL != dest)
    {
       len = GUI_GB2UCS((uint16*)dest, (uint8*)src, strlen(src));
       //MMIHR_Trace(("mrapp heroGB2UCS2 src:0x%02x 0x%02x 0x%02x 0x%02x dest: 0x%02x 0x%02x len:%d", src[0], src[1], src[2], src[3], dest[0], dest[1], len));
    }
}

int32 hr_setting_set_wallpaper(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    BOOLEAN result = TRUE;
    uint16 fullpathname[SFS_MAX_PATH] = {0};  
    int32  fullPathLen = 0;
	HR_SET_IDLE_PAPER_REQ *req = (HR_SET_IDLE_PAPER_REQ*)input;
	
	if(req == NULL || req->filename == NULL)
		return HR_FAILED;

    fullPathLen = hr_fs_get_filename(fullpathname, req->filename);

    if(req->setType == HR_IDLE_BG_PAPER)
    {
    #ifdef MMI_WALLPAPER_SUPPORT
        result = MMIAPIMULTIM_SetPictureToWallpaper(FALSE, fullpathname, fullPathLen);
	#else
        return HR_IGNORE;
	#endif
    }
    else
    {
        return HR_IGNORE;
    }
    MMIHR_Trace(("mrapp hr_setting_set_wallpaper type=%d filename=%s result=%d", req->setType, req->filename, result));

    if(result == TRUE)
    {
    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_setting_get_system_image_path(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	uint16 ucs2_name_len = SFS_MAX_PATH;
	uint16 devName = 'C';
	uint16 devNameLen = 1;
#if (HERO_SPR_VERSION >= 0x10A1140)
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
#else
    DC_DEV_E dev = DC_DEV_UDISK;
#endif
	wchar  *save_path = NULL;

    MMIHR_Trace(("[SKYSETTING] hr_setting_get_system_image_path output=0x%08x", output));

	if(output == NULL)
	{
		return HR_FAILED;
	}

#ifdef CAMERA_SUPPORT
    MMIDC_AllocSettingMemory();
#if (HERO_SPR_VERSION >= 0x10A1140)
    MMIDC_Setting_GetNVDefaultValue();
#else
    MMIDC_Setting_InitDefaultValue();
#endif
	dev = MMIDC_GetPhotoStorageDevice();
    save_path = MMIDC_GetPhotoSavePath();
    MMIDC_FreeSettingMemory();
#endif

#if (HERO_SPR_VERSION >= 0x10A1140)
    switch(dev)
    {
    case MMI_DEVICE_SYSTEM:
        devName = 'A';
        break;
    case MMI_DEVICE_UDISK:
        devName = 'B';
        break;
    case MMI_DEVICE_SDCARD:
    default:
        devName = 'C';
        break;
    }
#else
    switch(dev)
    {
    case DC_DEV_UDISK:
        devName = 'B';
        break;
    case DC_DEV_SD:
    default:
        devName = 'C';
        break;
    }
#endif

	memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));

	if(save_path != NULL)
	{
		ucs2_name_len = MMIAPICOM_Wstrlen(save_path);
		MMIAPICOM_Wstrncpy(hr_ucs2_name_arr, save_path, ucs2_name_len);
		hr_ucs2_name_arr[0] = devName;
	}
	else
	{ 
		if(!MMIFILE_CombineFullPath(
			(const uint16*)&devName, devNameLen,
			(const uint16*)MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
			NULL, 0,
			(uint16*)hr_ucs2_name_arr,
			&ucs2_name_len
			))
		{
			return HR_FAILED;
		}
	}
	
	MMIAPICOM_WstrTraceOut(hr_ucs2_name_arr, ucs2_name_len);
	MMIHEROAPP_ConvertPath2Loc((uint8*)hr_ucs2_name_arr, ucs2_name_len*2);
	*output = (uint8*)hr_ucs2_name_arr;
	
	return HR_SUCCESS;
}

int32 hr_setting_image_encode(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    int32 result = FALSE;
	uint16 path_buf[SFS_MAX_PATH] = {0};
    uint32 path_len = 0;
	uint8 *outbuf_ptr = NULL;
	uint8 *probuf_ptr = NULL;
    int32 outbuf_size = 0;
    int32 probuf_size = 0;
	PEPROCESS_INPUT_T       input_param  = {0};
    PEPROCESS_OUTPUT_T      output_param = {0};
    PEPROCESS_PARAMETER_T   process_param= {0};
    PEPROCESS_IMAGE_FORMAT_E out_type = PEPROCESS_IMAGE_FORMAT_JPEG;
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
	hr_image_encode_info* info = (hr_image_encode_info* )input;

    if(info == NULL || info->buf == NULL || info->path == NULL || info->w <= 0 || info->h <= 0)
    {
        return HR_FAILED;
    }

    MMIHR_Trace(("[SKYSETTING] hr_setting_image_encode type=%d w=%d h=%d", info->type, info->w, info->h));
    MMIAPICOM_WstrTraceOut(info->path, MMIAPICOM_Wstrlen(info->path));
	
	path_len = MMIAPICOM_Wstrlen(info->path);
	SCI_MEMSET(path_buf, 0x00, sizeof(path_buf));
	MMIAPICOM_Wstrncpy(path_buf, info->path, path_len);
	path_buf[0] += 2;
	
    outbuf_size = info->w*info->h*2;
    probuf_size = info->w*info->h*3+100*1024;

    outbuf_ptr = (uint8*)SCI_ALLOCA(outbuf_size);
	MMIHR_Trace(("[SKYSETTING] hr_setting_image_encode outbuf_ptr=0x%08x outbuf_size=%d", outbuf_ptr, outbuf_size));
    if(outbuf_ptr == NULL)
    {
        return HR_FAILED;
    }
    
    probuf_ptr = (uint8*)SCI_ALLOCA(probuf_size);
	MMIHR_Trace(("[SKYSETTING] hr_setting_image_encode probuf_ptr=0x%08x probuf_size=%d", probuf_ptr, probuf_size));
    if(probuf_ptr == NULL)
    {
		if(outbuf_ptr != NULL)
		{
			SCI_FREE(outbuf_ptr);
			outbuf_ptr = NULL;
		}
        return HR_FAILED;
    }

	SCI_MEMSET(outbuf_ptr, 0x00, outbuf_size);
	SCI_MEMSET(probuf_ptr, 0x00, probuf_size);

    if(info->type == IMG_BMP)
    {
        out_type = PEPROCESS_IMAGE_FORMAT_BMP;
    }
    else if(info->type == IMG_JPG)
    {
        out_type = PEPROCESS_IMAGE_FORMAT_JPEG;
    }

    //set input param   
    input_param.src_ptr     = info->buf;
    input_param.out_buf_ptr = outbuf_ptr;
    input_param.out_buf_size= outbuf_size;
    input_param.input_width = info->w;
    input_param.input_height= info->h;
    input_param.target_width  = info->w;
    input_param.target_height = info->h;
    input_param.in_type  = PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type = PEPROCESS_IMAGE_FORMAT_JPEG;
    
    //set process_param_ptr param
    process_param.process_buf_ptr  = probuf_ptr;
    process_param.process_buf_size = probuf_size;
    process_param.type = PEPROCESS_ENCODE_JPEG;
    
    //param
    process_param.process_param.encode_to_jpeg.quality_level = PEPROCESS_COMPRESS_LEVEL_LOW;
    
    //image encode
#if 0//def IMG_EDITER_SUPPORT
    ref_return = PEPROCESS_ImageProcess(&input_param, &output_param, &process_param);
#else
	ref_return = PEPROCESS_NO_SUPPORT;
#endif
    if(PEPROCESS_OK == ref_return)
    {
		if(MMIFILE_WriteFilesDataSyn(path_buf, path_len, input_param.out_buf_ptr, output_param.compress_out_size))
		{
			result = TRUE;
		}
    }

	if(outbuf_ptr)
	{
		SCI_FREE(outbuf_ptr);
		outbuf_ptr = NULL;
	}

	if(probuf_ptr)
	{
		SCI_FREE(probuf_ptr);
		probuf_ptr = NULL;
	}

    if(result == TRUE)
    {
        return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_setting_get_touchscreen_type(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
#ifdef TOUCHPANEL_TYPE
#ifdef CAP_TP_SUPPORT
    return HR_SCREEN_TYPE_CAPACITOR;
#else
    return HR_SCREEN_TYPE_RESISTOR;
#endif
#else
    return HR_IGNORE;
#endif
}

int32 hr_setting_get_camera_support(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
#ifdef __HERO_CFG_CAMERA_SUPPORT__
    return HR_SUCCESS;
#else
    return HR_IGNORE;
#endif
}

#ifdef __HR_SEND_MMS_SUPPORT__
int32 hr_sms_send_mms(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB* cb)
{
    T_HERO_MMS *pMMS = (T_HERO_MMS*)input;
    MMIMMS_ERROR_E ret = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = NULL;
    MMIFILE_FILE_INFO_T    file_info = {0};
    MMI_STRING_T addr = {0};
    MMI_STRING_T subject = {0};
    MMI_STRING_T content = {0};
    BOOLEAN success = TRUE;
    uint16 path[SFS_MAX_PATH] = {0};
    uint16 num[MMIPB_NUMBER_MAX_STRING_LEN] = {0};

    ret = MMIMMS_EnterEditCheck();

    MMIHR_Trace(("[SKYSMS] hr_sms_send_mms ret=%d pMMS=0x%08x input_len=%d", ret, pMMS, input_len));

    if(ret != MMIMMS_NO_ERROR || pMMS == NULL || input_len != sizeof(T_HERO_MMS))
    {
        return HR_FAILED;
    }

    editdoc_ptr = MMIMMS_CreateEditDocument();
    if(editdoc_ptr)
    {
        MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);

        if(pMMS->addr != NULL)
        {
            addr.wstr_len = GUI_GB2UCS(num, pMMS->addr, strlen(pMMS->addr));
            addr.wstr_ptr = num;
            MMIMMS_SetMMSTo(&addr, editdoc_ptr);
#if (HERO_SPR_VERSION >= 0x10A1140)
            MMIMMS_SetMMSNameListByTo(editdoc_ptr);
#endif
        }

        if(pMMS->subject != NULL)
        {
            MMIAPICOM_WstrTraceOut(pMMS->subject, MMIAPICOM_Wstrlen(pMMS->subject));
            subject.wstr_ptr = pMMS->subject;
            subject.wstr_len = MMIAPICOM_Wstrlen(pMMS->subject);
            MMIMMS_SetSubject(&subject, editdoc_ptr);
        }

        if(pMMS->content != NULL)
        {
            MMIAPICOM_WstrTraceOut(pMMS->content, MMIAPICOM_Wstrlen(pMMS->content));
            content.wstr_ptr = pMMS->content;
            content.wstr_len = MMIAPICOM_Wstrlen(pMMS->content);
            MMIMMS_AddText(editdoc_ptr, &content);
        }

        if(pMMS->file_path != NULL)
        {
            MMIAPICOM_WstrTraceOut(pMMS->file_path, MMIAPICOM_Wstrlen(pMMS->file_path));
            MMIAPICOM_Wstrncpy(path, pMMS->file_path, MMIAPICOM_Wstrlen(pMMS->file_path));
            path[0] += 2;
            MMIHEROAPP_ConvertPath2Pc((uint8*)path, MMIAPICOM_Wstrlen(path)*2);
            MMIAPIFMM_GetFileInfoFormFullPath(path, MMIAPICOM_Wstrlen(path), &file_info);

            if(!MMIMMS_AddFile(editdoc_ptr, &file_info, MMIFMM_FILE_TYPE_PICTURE, FALSE, 0))
            {
                success = FALSE;
            }
        }

        if(success == TRUE)
        {
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
            MMIMMS_CreateEditWin(); 
            return HR_SUCCESS;
        }
        else
        {
            MMIMMS_DestroyEditDocument(editdoc_ptr);//正常发送结束之后也需要Destroy?
            return HR_FAILED;
        }
    }

    return HR_FAILED;
}
#endif

int32 hr_fs_get_free_space(uint8 *input, int32 input_len, uint8** output, int32 *output_len, HR_PLAT_EX_CB *cb)
{
    uint32 kb_size = 1 << 10;
    uint32 mb_size = 1 << 20;
    uint32 gb_size = 1 << 30;
    uint32 used_low  = 0; 
    uint32 used_high = 0;
    uint32 free_low  = 0;
    uint32 free_high = 0;
    uint64 total_space = 0;
    uint64 free_space  = 0;
    uint32 total_space_low  = 0;
    uint32 total_space_high = 0;
    int32 resValue = HR_SUCCESS;
    T_HR_FREE_SAPCE *spaceInfo = (T_HR_FREE_SAPCE*)hr_tempTransBuf;
	
	if(input == NULL || spaceInfo == NULL)
		return HR_FAILED;
	
	MMIHR_Trace(("input: %s", input));
    
	switch((char)input[0])
	{
	case 'A':case 'a':
#ifdef HERO_PORT_BASE_NILE
		if(MMIFILE_GetDeviceStatus((uint8 *)HERO_MMIFILE_DEVICE_SYS_NAME, HERO_MMIFILE_DEVICE_SYS_NAME_LEN))
		{
			MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_MMIFILE_DEVICE_SYS_NAME, HERO_MMIFILE_DEVICE_SYS_NAME_LEN, &used_high, &used_low);
			MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_SYS_NAME, HERO_MMIFILE_DEVICE_SYS_NAME_LEN, &free_high, &free_low);
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
#endif

	case 'B':case 'b':
		if(MMIFILE_GetDeviceStatus((uint8*)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
		{
			MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN, &used_high, &used_low);
			MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN, &free_high, &free_low);
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
        
	case 'C':case 'c':
#if (HERO_SPR_VERSION >= 0x10A1140)
		if(TRUE == MMIFILE_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#else
		if(MMIAPISD_GetStatus())
#endif
		{
			MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN, &used_high, &used_low);
			MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN, &free_high, &free_low);
		}
#ifdef DUAL_TCARD_SUPPORT
        else if(TRUE == MMIFILE_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
        {
            MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN, &used_high, &used_low);
            MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN, &free_high, &free_low);
        }
#endif
		else
		{
			resValue = HR_FAILED;
		}
		break;
        
#ifdef DOUBLE_TFCARD
	case 'D':case 'd':
		if(MMIAPISD2_GetStatus())
		{
			MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD_2, HERO_MMIFILE_DEVICE_SDCARD_LEN_2, &used_high, &used_low);
			MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_SDCARD_2, HERO_MMIFILE_DEVICE_SDCARD_LEN_2, &free_high, &free_low);
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
#endif

	default:
		resValue = HR_FAILED;
		break;
	}	

	memset(spaceInfo, 0, sizeof(T_HR_FREE_SAPCE));
	//MMIHR_Trace(("free_high: %u, free_low: %u, used_high: %u, used_low: %u", free_high, free_low, used_high, used_low));		
	if(resValue == HR_SUCCESS)
    {
		/*free space*/
		if(free_high || (uint32)free_low > (uint32)0x7fffffff)
        {
            free_space = (uint64)((free_high<<22) + (free_low>>10))*1024;
		}
        else
        {
            free_space = free_low;
		}
		
		/*total space*/
		total_space_low  = used_low  + free_low;  
		total_space_high = used_high + free_high;
        
		if ((total_space_low < used_low) || (total_space_low < free_low))
        {
			total_space_high += 1;
		}
		//MMIHR_Trace(("total_space_low: %u, total_space_high: %u", total_space_low, total_space_high));		
		if(total_space_high || (uint32)total_space_low > (uint32)0x7fffffff)
        {
			total_space = (uint64)((total_space_high<<22) + (total_space_low>>10))*1024;
		}
        else
        {
			total_space = total_space_low;
		}
        MMIHR_Trace(("total_space: %llu, free_space: %llu", total_space, free_space));		
#if 0   //单位太大会导致精度不精确
        if((total_space/10) > (uint64)gb_size)
    	{
    		spaceInfo->tunit = gb_size;
    	}
    	else if((total_space/10) > mb_size)
    	{
    		spaceInfo->tunit = mb_size;
    	}
        else
#endif
    	if((total_space/10) > kb_size)
    	{
    		spaceInfo->tunit = kb_size;
    	}
    	else 
    	{
    		spaceInfo->tunit = 1;
    	}
    	
    	spaceInfo->total = total_space/spaceInfo->tunit;
#if 0   //单位太大会导致精度不精确
    	if((free_space/10) >(uint64)gb_size)
    	{
    		spaceInfo->unit = gb_size;
    	}
    	else if((free_space/10) > mb_size)
    	{
    		spaceInfo->unit = mb_size;
    	}
    	else
#endif
        if((free_space/10) > kb_size)
    	{
    		spaceInfo->unit = kb_size;
    	}
    	else 
    	{
    		spaceInfo->unit = 1;
    	}
    	
    	spaceInfo->account = free_space/spaceInfo->unit;

		MMIHR_Trace(("free: %d, total: %d, unit: %d, tunit: %d", spaceInfo->account, spaceInfo->total, spaceInfo->unit, spaceInfo->tunit));
            
		*output = (uint8*)spaceInfo;
		*output_len = sizeof(T_HR_FREE_SAPCE);    	
    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_setting_wifi_exist(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
//是否存在WIFI功能
#ifdef __HERO_APP_WIFI_SUPPORT__
	return HR_SUCCESS;
#else
	return HR_FAILED;
#endif
}

// 20190430
#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__
//#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__

int hr_get_local_version(char* temp, int buffSize)
{
#ifdef WIN32
	return 0;
#else
	extern char* const s_version_info[];
	int iIndex = 0;
	while( *(s_version_info[1]+iIndex) !='\0'
		&& *(s_version_info[1]+iIndex) == ' ' )
	{
		iIndex++;
	}
	SCI_MEMCPY(temp, s_version_info[1]+iIndex, MIN(strlen(s_version_info[1])-iIndex, buffSize));
	return strlen(temp);
	
#endif
}

int32 hr_setting_get_handsetID_ex(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	hr_userinfo usrinfo;
	int templen = 0;
	int tempIndex = 0;
	char temp[256] = {0};

	if(!input || input_len < __HERO_CFG_VAR_HANDSETID_EX_LENGHT_MAX__)
		return HR_FAILED;

	hr_getUserInfo(&usrinfo);
	templen = hr_get_local_version(temp, sizeof(temp) - 1);
	if(templen == 0)
	{
		_snprintf( (char*)input, input_len, "%s-_%s", usrinfo.type, __HERO_CFG_VAR_HANDSETID_EX__); 
		return HR_SUCCESS;
	}

	for(tempIndex = 0; tempIndex < templen; tempIndex++)
	{
		if((temp[tempIndex] >= '0' && temp[tempIndex] <= '9') || (temp[tempIndex] >= 'a' && temp[tempIndex] <= 'z') || (temp[tempIndex] >= 'A' && temp[tempIndex] <= 'Z'))
		{
			continue;
		}
		temp[tempIndex] = '_';
	}

	_snprintf( (char*)input, input_len - 1, "%s-_%s", usrinfo.type, temp);	
	MMIHR_Trace(("mrapp hr_setting_get_handsetID_ex type=%s", input));

	return HR_SUCCESS;
}
#endif


#ifdef __HR_CFG_LOCALAPP_MANAGER__
void hr_registerLappEntry(void* entry)
{
    s_hero_appEntry = (startHeroLapp)entry;
}

int32 hr_start_plat_application(uint8* input)
{
    if(s_hero_appEntry)
    {
        return s_hero_appEntry(input);
    }

    return HR_FAILED;
}
#endif
PUBLIC MMISMS_STATE_T * MMIHEROAPP_GetSaveSMSOrderId(void)
{
	return &s_hr_save_order_id;
}

LOCAL BOOLEAN MMIHEROAPP_SMSCnf(BOOLEAN is_succss, DPARAM parm)
{
	BOOLEAN resValue = TRUE;
	
	MMIHR_Trace(("mrapp MMIHEROAPP_SMSCnf is_succss: %d", is_succss));
	if(TRUE == is_succss)
	{
	 	hr_event(HR_SMS_RESULT, HR_SUCCESS, 0);//短信发送成功
	}
	else
	{
		hr_event(HR_SMS_RESULT, HR_FAILED, 0);//短信发送失败
	}

	MMIHR_Trace(("MMIHEROAPP_SMSCnf resValue: %d", resValue));
	return resValue;
}

PUBLIC void MMIHEROAPP_GetSMSCapacityCallBack(void)
{
	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_GetSMSCapacityCallBack NVMaxNum: %d", s_hr_sms_NVNum));
	hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_GET_ME_SIZE, s_hr_sms_NVNum);

	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_GetSMSCapacityCallBack SIMMaxNum: %d", s_hr_sms_SIMNum));
	hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_GET_SIM_SIZE, s_hr_sms_SIMNum);
}

PUBLIC void MMIHEROAPP_GetSMSContentCallBack()
{ /*lint !e18*/	

	MMI_STRING_T string_ptr = {0};
	wchar content_ptr[500] = {0};
	string_ptr.wstr_ptr = content_ptr;

	MMIHEROAPP_SetIsReadBySMS(FALSE);
	
	MMISMS_GetSMSContent(&string_ptr);
	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_GetSMSContentCallBack string_ptr.wstr_ptr: %d",  MMIAPICOM_Wstrlen(string_ptr.wstr_ptr)));
	MMIAPICOM_WstrTraceOut(string_ptr.wstr_ptr, MMIAPICOM_Wstrlen(string_ptr.wstr_ptr));
	MMIAPICOM_Wstrcpy(s_hr_sms_content.content, string_ptr.wstr_ptr);

	if(NULL != s_hr_sms_content.content) /*lint !e506 !e774 !e831*/
	{
#ifndef WIN32
		hr_str_convert_endian((char*)s_hr_sms_content.content);
#endif
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_GET_MSG_CONTENT, s_hr_sms_content.content); /*lint !e64*/
	}
}

PUBLIC void MMIHEROAPP_DeleteSMSCallBack(void *param)
{
	BOOLEAN result = FALSE;
	MN_SMS_CAUSE_E    cause = *(int32*)param;
    MMISMS_ORDER_ID_T order_id = NULL;
    MMISMS_BOX_TYPE_E type = MMISMS_BOX_NONE;
        
	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_DeleteSMSCallBack order_id: 0x%08x cause=%d", s_hr_sms_order_id, cause));

	if(cause != MN_SMS_OPERATE_SUCCESS)
	{
		MMIHEROAPP_SetIsReadBySMS(FALSE);
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_DELETE, HR_FAILED);
		return;
	}

    if(s_hr_sms_order_id != NULL)
    {
        order_id = s_hr_sms_order_id;
        s_hr_sms_order_id = order_id->next_long_sms_ptr;
        if(order_id->flag.mo_mt_type == MMISMS_MT_HAVE_READ || order_id->flag.mo_mt_type == MMISMS_MT_TO_BE_READ)
        {
            type = MMISMS_BOX_MT;
        }
        else if(order_id->flag.mo_mt_type == MMISMS_MO_SEND_SUCC)
        {
            type = MMISMS_BOX_SENDSUCC;
        }
        
        result = MMISMS_DeleteReqToMN(type, order_id, MMISMS_DELETE_MSG);
        if(FALSE == result)
        {
            MMIHEROAPP_SetIsReadBySMS(FALSE);
            hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_DELETE, HR_FAILED);
        }
    }
    else
    {
        MMIHEROAPP_SetIsReadBySMS(FALSE);
    	if(cause == MN_SMS_OPERATE_SUCCESS)
    	{
    		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_DELETE, HR_SUCCESS);
    	}
    	else
    	{
    		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_DELETE, HR_FAILED);
    	}
    }
}

PUBLIC void MMIHEROAPP_SetSMSStateCallBack(void)
{
	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_SetSMSStateCallBack err_code: %d", s_hr_sms_err_code));
	if(s_hr_sms_err_code == ERR_MNSMS_NONE)
	{
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_SET_STATUS, HR_SUCCESS);
	}
	else
	{
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_SET_STATUS, HR_FAILED);
	}
}

PUBLIC void MMIHEROAPP_SaveSMSToMtCallBack(MN_SMS_CAUSE_E result)
{
	MMIHR_Trace(("MRAPP[SMS] MMIHEROAPP_SaveSMSToMtCallBack result: %d", result));
	if(result == MN_SMS_OPERATE_SUCCESS)
	{
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_SAVE_SMS, TRUE);
	}
	else
	{
		hr_event(HR_SMS_OP_EVENT, HR_SMS_OP_SAVE_SMS, FALSE);
	}	
}

#ifdef __HR_CFG_MULTITP_SUPPORT__
#define MOUSE_MOVE_SPACE (6)
static HR_TOUCH_PANEL_COORD_LIST_T g_last_touch_info = {0};

static uint16 hr_touch_panel_down_func(uint16 x_diff, uint16 y_diff, uint16 count)
{
	if (x_diff > MOUSE_MOVE_SPACE || y_diff > MOUSE_MOVE_SPACE)
	{
		return HR_MOUSE_MOVE;
	}
	else
	{
		return HR_MOUSE_DOWN | 0x0100;
	}
}

static uint16 hr_touch_panel_up_func(uint16 x_diff, uint16 y_diff, uint16 count)
{
	return HR_MOUSE_UP;
}

static uint16 hr_touch_panel_move_func(uint16 x_diff, uint16 y_diff, uint16 count)
{
	if (x_diff > MOUSE_MOVE_SPACE || y_diff > MOUSE_MOVE_SPACE)
	{
		return HR_MOUSE_MOVE;
	}
	else
	{
		return HR_MOUSE_DOWN | 0x0100;
	}
}

typedef uint16 (* HR_TOUCH_PANEL_FUNC)(uint16 x_diff, uint16 y_diff, uint16 count);

static const HR_TOUCH_PANEL_FUNC g_hr_touch_panel_func[] =  
{
	hr_touch_panel_down_func,    /*Down */
	hr_touch_panel_up_func,      /*Up*/
	hr_touch_panel_move_func,    /*Move*/
};

static float hr_touch_panel_inv_sqrt(float x)
{
	/* 0x5f375a86 这是一个非常神奇的数字*/
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86 - (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}

static int16 mr_touch_panel_get_diff(int16 x, int16 y)
{
	int xx,yy,tmp;
	
	if (x == 0)
	{
		return y;
	}
	else if (y == 0)
	{
		return x;
	}
	else
	{
		xx = x*x; yy = y*y; tmp = xx + yy;
		return (int16)(1/hr_touch_panel_inv_sqrt(tmp));
	}
}

static int16 mr_touch_panel_find_nearest(int16 diff[MAX_TOUCH_POINTS][MAX_TOUCH_POINTS], int16 *row, int16 *col)
{
#define VERY_LARGE 0x7FFF
	uint16 i, j;
	int16 min_diff = VERY_LARGE;
	int16 k = -1, l = -1;

	for (i = 0; i < *row; i++)
	{
		for (j = 0; j < *col; j++)
		{
			if (diff[i][j] >= 0 && min_diff > diff[i][j])
			{
				min_diff = diff[i][j];
				k = i;
				l = j;
			}
		}
	}

	if (k != -1 && l != -1)
	{
		for (i = 0; i < *row; i++)
		{
			if (diff[i][l] >= 0)
			{
				diff[i][l] = -1;
			}
		}
		for (i = 0; i < *col; i++)
		{
			diff[k][i] = -2;
		}
		*row = k;
		*col = l;
		return 1;
	}
	else
	{
		for (i = 0; i < *row; i++)
		{
			if (diff[i][0] > -2)
			{
				for (j = 0; j < *col; j++)
				{
					diff[i][j] = -2;
				}
				*row = i;
				*col = -1;
				return 1;
			}
			
		}
		return 0;
	}
}


static uint16 mr_touch_panel_state_machine(uint16 x_diff, uint16 y_diff, uint16 pre_event, uint16 count)
{
	int index;
	
	if (pre_event == HR_MOUSE_DOWN)
	{
		index = 0;
	}
	else if (pre_event == HR_MOUSE_UP)
	{
		index = 1;
	}
	else
	{
		index = 2;
	}
	
	return g_hr_touch_panel_func[index](x_diff, y_diff, count);
}

void hr_touch_panel_convert_event(HR_TOUCH_PANEL_COORD_LIST_T *current_event, HR_TOUCH_PANEL_COORD_LIST_T *last_event)
{
	int16 i, j;
	uint32 event;
	int16 x_diff;
	int16 y_diff;
	uint32 touch_count = 0;
	uint32 ori_touch_count = 0;
	HR_TOUCH_PANEL_COORD_LIST_T tmp_event;
	int16 diff[MAX_TOUCH_POINTS][MAX_TOUCH_POINTS];
	uint8 dir;

	if (current_event->touch_count == 0) //all touch up
	{
		for (i = 0; i < last_event->touch_count; i++)
		{
			if (last_event->points[i].event != HR_MOUSE_UP)
			{
				current_event->points[i].x = last_event->points[i].x;
				current_event->points[i].y = last_event->points[i].y;
				current_event->points[i].z = last_event->points[i].z;
				current_event->points[i].event = HR_MOUSE_UP;
				touch_count++;
			}
		}

		current_event->touch_count = touch_count;

		return ;
	}

	for (i = 0; i < last_event->touch_count; i++)
	{
		if (last_event->points[i].event != HR_MOUSE_UP)
		{
			ori_touch_count++;
		}
	}

	if (ori_touch_count == 0)
	{
		/* no need convert */
		return ;
	}

	if (ori_touch_count != last_event->touch_count)
	{
		ori_touch_count = 0;
		for (i = 0; i < last_event->touch_count; i++)
		{
			if (last_event->points[i].event != HR_MOUSE_UP)
			{
				last_event->points[ori_touch_count].x = last_event->points[i].x;
				last_event->points[ori_touch_count].y = last_event->points[i].y;
				last_event->points[ori_touch_count].z = last_event->points[i].z;
				last_event->points[ori_touch_count].event = last_event->points[i].event;
				ori_touch_count++;
			}
		}
		last_event->touch_count = ori_touch_count;
	}

	if (current_event->touch_count < last_event->touch_count)
	{
		dir = 1;
	}
	else
	{
		dir = 0;
	}

	/* calc the diff of all the down point */
	for (i = 0; i < current_event->touch_count; i++)
	{
		for (j = 0; j < last_event->touch_count; j++)
		{
			if (current_event->points[i].x > last_event->points[j].x)   
				x_diff = current_event->points[i].x - last_event->points[j].x;
			else
				x_diff = last_event->points[j].x - current_event->points[i].x;
			if (current_event->points[i].y > last_event->points[j].y)
				y_diff = current_event->points[i].y - last_event->points[j].y;
			else
				y_diff = last_event->points[j].y - current_event->points[i].y;
			if (dir)
			{
				diff[j][i] = mr_touch_panel_get_diff(x_diff, y_diff);
			}
			else
			{
				diff[i][j] = mr_touch_panel_get_diff(x_diff, y_diff);
			}
		}
	}

	if (dir)
	{
		j = current_event->touch_count;
		i = last_event->touch_count;
	}
	else
	{
		i = current_event->touch_count;
		j = last_event->touch_count;
	}

	touch_count = 0;
	while (mr_touch_panel_find_nearest(diff, &i, &j))
	{
		if (i != -1 && j != -1)
		{
			if (dir)
			{
				if (current_event->points[j].x > last_event->points[i].x)   
					x_diff = current_event->points[j].x - last_event->points[i].x;
				else
					x_diff = last_event->points[i].x - current_event->points[j].x;
				if (current_event->points[j].y > last_event->points[i].y)
					y_diff = current_event->points[j].y - last_event->points[i].y;
				else
					y_diff = last_event->points[i].y - current_event->points[j].y;

				event = mr_touch_panel_state_machine(x_diff, y_diff, last_event->points[i].event, i);

				tmp_event.points[touch_count].x = current_event->points[j].x;
				tmp_event.points[touch_count].y = current_event->points[j].y;
				tmp_event.points[touch_count].z = current_event->points[j].z;
				tmp_event.points[touch_count].event = event;
			}
			else
			{
				if (current_event->points[i].x > last_event->points[j].x)   
					x_diff = current_event->points[i].x - last_event->points[j].x;
				else
					x_diff = last_event->points[j].x - current_event->points[i].x;
				if (current_event->points[i].y > last_event->points[j].y)
					y_diff = current_event->points[i].y - last_event->points[j].y;
				else
					y_diff = last_event->points[j].y - current_event->points[i].y;

				event = mr_touch_panel_state_machine(x_diff, y_diff, last_event->points[j].event, j);

				tmp_event.points[touch_count].x = current_event->points[i].x;
				tmp_event.points[touch_count].y = current_event->points[i].y;
				tmp_event.points[touch_count].z = current_event->points[i].z;
				tmp_event.points[touch_count].event = event;
			}
			
			touch_count++;
		}
		else if (i != -1)
		{
			if (dir)
			{
				tmp_event.points[touch_count].x = last_event->points[i].x;
				tmp_event.points[touch_count].y = last_event->points[i].y;
				tmp_event.points[touch_count].z = last_event->points[i].z;
				tmp_event.points[touch_count].event = HR_MOUSE_UP;
			}
			else
			{
				tmp_event.points[touch_count].x = current_event->points[i].x;
				tmp_event.points[touch_count].y = current_event->points[i].y;
				tmp_event.points[touch_count].z = current_event->points[i].z;
				tmp_event.points[touch_count].event = HR_MOUSE_DOWN;
			}
			touch_count++;
		}
		
		if (dir)
		{
			j = current_event->touch_count;
			i = last_event->touch_count;
		}
		else
		{
			i = current_event->touch_count;
			j = last_event->touch_count;
		}
	}
	
	tmp_event.touch_count = touch_count;

	memcpy(current_event, &tmp_event, sizeof(tmp_event));
}

int32 hr_touch_panel_get_position(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    int32 i = 0;
	int16 touch_count = 0;
	uint8 have_touch_count = 0;
    GUI_POINT_T     tp_point  = {0};
    MMI_TP_POINTS_T tp_points = {0};
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    
	HR_TOUCH_PANEL_COORD_T *pCoord = (HR_TOUCH_PANEL_COORD_T *)input;
    HR_TOUCH_PANEL_COORD_LIST_T *list = (HR_TOUCH_PANEL_COORD_LIST_T *)output;
	
	if (!pCoord || input_len != sizeof(HR_TOUCH_PANEL_COORD_T))
		return HR_FAILED;

    MMK_GetLogicTPMsg(&tp_status, &tp_point);

    pCoord->x = tp_point.x;
    pCoord->y = tp_point.y;
    
//#ifdef TOUCHPANEL_TYPE_MULTITP
    MMK_GetTpPoints(&tp_points);

	if (output != NULL)
	{
        list->touch_count = tp_points.num;
        for(i=0; i<tp_points.num; i++)
        {
            list->points[i].x = tp_points.points[i].x;
            list->points[i].y = tp_points.points[i].y;
            list->points[i].z = 0;
            list->points[i].event = HR_MOUSE_DOWN;
        }
        //MMIHR_Trace(("SKY total tp_status:%d points:%d", tp_status, list->touch_count));
		hr_touch_panel_convert_event(list, &g_last_touch_info);
		memcpy(&g_last_touch_info, list, sizeof(HR_TOUCH_PANEL_COORD_LIST_T));
		for (i = 0; i < g_last_touch_info.touch_count; i++)
		{
			if ((g_last_touch_info.points[i].event & 0xff00) != 0)
			{
				g_last_touch_info.points[i].event &= 0x00ff;
			}
			else
			{
				have_touch_count = 1;
			}
		}
		for (i = 0; i < list->touch_count; i++)
		{
			if (((list->points[i].event & 0xff00) == 0) || (have_touch_count > 0 && output_len != NULL && *output_len != 0))
			{
				//MMIHR_Trace(("point[%d] = [%d, %d, %d]", touch_count, list->points[i].x, list->points[i].y, list->points[i].event));
				memcpy(&list->points[touch_count], &g_last_touch_info.points[i], sizeof(HR_TOUCH_PANEL_COORD_EX_T));
				touch_count++;
			}
		}
		list->touch_count = touch_count;

    }
    else
    {
        memset(&g_last_touch_info, 0, sizeof(g_last_touch_info));
    }

    if(output_len != NULL)
    {
        switch(tp_status)
        {
        default:
        case MMI_TP_DOWN:
            *output_len = HR_MOUSE_DOWN;
            break;

        case MMI_TP_UP:
            *output_len = HR_MOUSE_UP;
            break;
        }        
    }
//#endif

    return HR_SUCCESS;
}
#endif

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
uint32 hr_skybmp_open(const uint16* path, uint8 *isSkyBmp)
{
	uint32 f;
	char type[5] = {0};
	uint32 read;
	
	f = SFS_CreateFile(path, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);

	if (f == 0)
		return 0;

	SFS_ReadFile(f, type, sizeof(type) - 1, &read, NULL);

    MMIHR_Trace(("[SKYSHORTCUT] hr_skybmp_open type = %s", type));

	if(strcmp(type, "SKBM") == 0)
	{
        *isSkyBmp = 1;
	}
    else
    {
        *isSkyBmp = 0;
    }

	return f;
}

void hr_skybmp_get_size(uint32 f, int16* w, int16* h)
{
	uint32 read = 0;
	
	SFS_SetFilePointer(f, 4, SFS_SEEK_BEGIN);	// 跳过4个字节的头

	SFS_ReadFile(f, w, 2, &read, NULL);
	SFS_ReadFile(f, h, 2, &read, NULL);
#ifdef WIN32
    hr_str_convert_endian_with_size(w, 2);
    hr_str_convert_endian_with_size(h, 2);
#endif
    MMIHR_Trace(("[SKYSHORTCUT] hr_skybmp_get_size w = %d h = %d", *w, *h));
}

uint32 hr_skybmp_get_data(uint32 f, void* data, int32 len)
{
    int16 w = 0;
    int16 h = 0;
    int32 ret = 0;
	uint32 read = 0;
    int32 outbuf_size = 0;
    int32 probuf_size = 0;
	uint8 *outbuf_ptr = NULL;
	uint8 *probuf_ptr = NULL;
	PEPROCESS_INPUT_T       input_param  = {0};
    PEPROCESS_OUTPUT_T      output_param = {0};
    PEPROCESS_PARAMETER_T   process_param= {0};
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;

    hr_skybmp_get_size(f, &w, &h);

	ret = SFS_SetFilePointer(f, 8, SFS_SEEK_BEGIN);
    if(ret != SFS_ERROR_NONE)
    {
        return 0;
    }
    
	ret = SFS_ReadFile(f, data, len, &read, NULL);
    if(ret != SFS_ERROR_NONE)
    {
        return 0;
    }

    outbuf_size = w*h*2;
    probuf_size = w*h*3+100*1024;

    outbuf_ptr = (uint8*)SCI_ALLOCA(outbuf_size);
    if(outbuf_ptr == NULL)
    {
        return 0;
    }
    
    probuf_ptr = (uint8*)SCI_ALLOCA(probuf_size);
    if(probuf_ptr == NULL)
    {
		if(outbuf_ptr != NULL)
		{
			SCI_FREE(outbuf_ptr);
			outbuf_ptr = NULL;
		}
        return 0;
    }

	SCI_MEMSET(outbuf_ptr, 0x00, outbuf_size);
	SCI_MEMSET(probuf_ptr, 0x00, probuf_size);

    //set input param   
    input_param.src_ptr     = data;
    input_param.out_buf_ptr = outbuf_ptr;
    input_param.out_buf_size= outbuf_size;
    input_param.input_width = w;
    input_param.input_height= h;
    input_param.target_width  = w;
    input_param.target_height = h;
    input_param.in_type  = PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type = PEPROCESS_IMAGE_FORMAT_JPEG;
    
    //set process_param_ptr param
    process_param.process_buf_ptr  = probuf_ptr;
    process_param.process_buf_size = probuf_size;
    process_param.type = PEPROCESS_ENCODE_JPEG;
    
    //image encode
#if 0//def IMG_EDITER_SUPPORT
    ref_return = PEPROCESS_ImageProcess(&input_param, &output_param, &process_param);
#else
	ref_return = PEPROCESS_NO_SUPPORT;
#endif
    if(PEPROCESS_OK == ref_return)
    {
        read = MIN(read, output_param.compress_out_size);
        SCI_MEMSET((uint8*)data, 0x00, len);
        SCI_MEMCPY((uint8*)data, input_param.out_buf_ptr, read);
    }
    else
    {
        read = 0;
		SCI_MEMSET((uint8*)data, 0x00, len);
    }

	if(outbuf_ptr)
	{
		SCI_FREE(outbuf_ptr);
		outbuf_ptr = NULL;
	}

	if(probuf_ptr)
	{
		SCI_FREE(probuf_ptr);
		probuf_ptr = NULL;
	}

    MMIHR_Trace(("[SKYSHORTCUT] hr_skybmp_get_data len = %d read = %d ref_return = %d", len, read, ref_return));

	return read;
}

void hr_skybmp_close(uint32 f)
{
	SFS_CloseFile(f);
}

int32 hr_scut_save_data(int32 appId, HR_SHORTCUT_APPINFO_T *appInfo)
{
    int32 fInfo = 0;
    int32 fHandle = 0;
    uint8 fName[SFS_MAX_PATH] = {0};

    if(appInfo != NULL && appId != 0)
    {
        if(hr_info("dam") != HR_IS_DIR)
        {
            hr_mkDir("dam");
        }
        sprintf(fName, "dam\\%d.dat", appId);
        fInfo = hr_info(fName);
        if(fInfo == HR_IS_FILE)
        {
            hr_remove(fName);
        }
        
        fHandle = hr_open(fName, HR_FILE_WRONLY|HR_FILE_CREATE);
        if(fHandle > 0)
        {
            hr_write(fHandle, (void *)appInfo, sizeof(HR_SHORTCUT_APPINFO_T));
        }
        hr_close(fHandle);

        return HR_SUCCESS;
    }

    return HR_FAILED;
}

int32 hr_scut_read_data(int32 appId, HR_SHORTCUT_APPINFO_T *appInfo)
{
    int32 fInfo = 0;
    int32 fHandle = 0;
    uint8 fName[SFS_MAX_PATH] = {0};

    if(appInfo != NULL && appId != 0)
    {
        sprintf(fName, "dam\\%d.dat", appId);
        fInfo = hr_info(fName);

        if(fInfo == HR_IS_FILE)
        {
            fHandle = hr_open(fName, HR_FILE_RDONLY);
            if(fHandle > 0)
            {
                hr_read(fHandle, (void *)appInfo, sizeof(HR_SHORTCUT_APPINFO_T));
                hr_close(fHandle);
                return HR_SUCCESS;
            }
        }
    }

    return HR_FAILED;
}

void hr_scut_remove_data_file(int32 appId)
{
    int32 fInfo = 0;
    uint8 fName[SFS_MAX_PATH] = {0};

    if(appId != 0)
    {
        sprintf(fName, "dam\\%d.dat", appId);
        fInfo = hr_info(fName);

        if(fInfo == HR_IS_FILE)
        {
            hr_remove(fName);
        }
    }
}

int32 hr_scut_refresh_icon(uint32 menuId, HR_SHORTCUT_APPINFO_T appInfo)
{
    int32 has_icon = 0;
    int32 fState = 0;
    BOOLEAN ret = FALSE;
    BOOLEAN is_found = FALSE;
    int16 imgW = 0;
    int16 imgH = 0;
    uint8 isSkyBmp = 0;
    uint32 fHandle = 0;
    uint32 dataLen = 0;
    uint32 imageSize = 0;
    uint8 *iconbuf = NULL;
    uint16 fullpath[SFS_MAX_PATH] = {0};  
	GUIANIM_IMG_INFO_T img_info = {0};
    GUIMENU_MAINMENU_DY_ITEM_T menu_item = {0};


    fState = hr_info(appInfo.image_path);

    if(fState != HR_IS_FILE)
    {
        return HR_FAILED;
    }

    is_found = MMIMENU_DySlideHasIcon(menuId, &has_icon);

    if(is_found && !has_icon)
    {
        ret = MMIMENU_DySlideGetItemInfo(menuId, &menu_item);
		if(ret == TRUE)
        {
            hr_fs_get_filename(fullpath, appInfo.image_path);
            fHandle = hr_skybmp_open(fullpath, &isSkyBmp);
            if(fHandle > 0)
            {
                if(isSkyBmp == 1)
                {
                    hr_skybmp_get_size(fHandle, &imgW, &imgH);
                    dataLen = imgW*imgH*2;
                    iconbuf = (uint8*)SCI_ALLOCA(dataLen);
                    if(iconbuf != NULL)
                    {
                        imageSize = hr_skybmp_get_data(fHandle, iconbuf, dataLen);
                    }
                }
                else
                {
                    SFS_GetFileSize(fHandle, &dataLen);
                    iconbuf = (uint8*)SCI_ALLOCA(dataLen);
                    if(iconbuf != NULL)
                    {
                        SFS_ReadFile(fHandle, (void*)iconbuf, dataLen, &imageSize, NULL);
        				GUIANIM_GetInfo(NULL, fullpath, PNULL, &img_info);
        				imgW = img_info.img_width;
        				imgH = img_info.img_height;
                    }
                }
                hr_skybmp_close(fHandle);
            	menu_item.ori_data_ptr->dynamic_menu_info.has_icon = TRUE;
            	menu_item.ori_data_ptr->dynamic_menu_info.icon_datasize = imageSize;
                menu_item.ori_data_ptr->dynamic_menu_info.icon_ptr = (uint8 *)iconbuf;
                menu_item.ori_data_ptr->dynamic_menu_info.icon_height = imgH;
                menu_item.ori_data_ptr->dynamic_menu_info.icon_width  = imgW;
                menu_item.ori_data_ptr->dynamic_menu_info.select_icon_id= IMAGE_MAINMENU_ICON_SELECT;
                ret = MMIMENU_DySlideSaveItemInfo(menuId, &menu_item);

                if(iconbuf != NULL)
                {
                    SCI_FREE(iconbuf);
                }
            }
        }      
    }    
    
    MMIHR_Trace(("[SKYSHORTCUT] hr_scut_refresh_icon menuId = %u ret = %d", menuId, ret));
    return ret;
}

void hr_scut_delete_app(void* param1,void *param2)
{
    int32 appId = (*(int32 *)param1);
	uint32 menuId = (*(uint32 *)param2);
    HR_SHORTCUT_APPINFO_T appInfo = {0};
    
    MMIHR_Trace(("[SKYSHORTCUT] hr_scut_delete_app appId = %u menuId = %d", appId, menuId));

    if(HR_SUCCESS == hr_scut_read_data(appId, &appInfo))  
    {
        if(HR_IS_FILE == hr_info(appInfo.filepath))
        {
            hr_remove(appInfo.filepath);
            hr_scut_remove_data_file(appId);
        }
    }
}

int32 hr_scut_support(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
#ifdef DYNAMIC_MAINMENU_SUPPORT
    return HR_SUCCESS;
#else
    return HR_IGNORE;
#endif
}

int32 hr_scut_install_app(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    int16 imgW = 0;
    int16 imgH = 0;
    uint8 isSkyBmp = 0;
    uint32 fHandle = 0;
    uint32 dataLen = 0;
    uint32 imageSize = 0;
    uint8 *iconbuf = NULL;
    BOOLEAN ret = FALSE;
    uint16 fullpath[SFS_MAX_PATH] = {0};  
	GUIANIM_IMG_INFO_T img_info = {0};
    GUIMENU_FILE_DEVICE_E store_idx = GUIMENU_DEVICE_UDISK;
    GUIMENU_MAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr = PNULL;
	HR_SHORTCUT_APPINFO_T* appinfo = (HR_SHORTCUT_APPINFO_T* )input;

	if(!input || input_len != sizeof(HR_SHORTCUT_APPINFO_T)
		|| appinfo->filepath[0] == 0)
	{
		return HR_FAILED;
	}

    MMIHR_Trace(("[SKYSHORTCUT] hr_scut_install_app appId = %u", appinfo->appid));
    MMIHR_Trace(("[SKYSHORTCUT] total_size = %d cur_size = %d", appinfo->total_size, appinfo->cur_size));
    MMIHR_Trace(("[SKYSHORTCUT] provider = %d startParam = %s", appinfo->provider, appinfo->start_data));
    MMIHR_Trace(("[SKYSHORTCUT] filePath = %s", appinfo->filepath));
    MMIHR_Trace(("[SKYSHORTCUT] imagePath = %s", appinfo->image_path));
    MMIHR_Trace(("[SKYSHORTCUT] title_en = %s", appinfo->title_en));
    MMIHR_Trace(("[SKYSHORTCUT] title_mt_len = %d", MMIAPICOM_Wstrlen(appinfo->title_mt)));

    dynamic_item_ptr = SCI_ALLOCA(sizeof(GUIMENU_MAINMENU_DYNAMIC_INFO_T));
    if (dynamic_item_ptr == PNULL)
    {
        MMIHR_Trace(("[SKYSHORTCUT]hr_scut_install_app: alloc dynamic_item_ptr return PNULL"));    
        return HR_FAILED;
    }
    SCI_MEMSET(dynamic_item_ptr, 0, sizeof(GUIMENU_MAINMENU_DYNAMIC_INFO_T));
#ifndef WIN32   
    hr_fs_get_filename(fullpath, appinfo->image_path);
    fHandle = hr_skybmp_open(fullpath, &isSkyBmp);
    MMIHR_Trace(("[SKYSHORTCUT] hr_skybmp_open: fHandle = 0x%08x isSkyBmp = %d", fHandle, isSkyBmp));    
    if(fHandle > 0)
    {
        if(isSkyBmp == 1)
        {
            hr_skybmp_get_size(fHandle, &imgW, &imgH);
            dataLen = imgW*imgH*2;
            iconbuf = (uint8*)SCI_ALLOCA(dataLen);
            if(iconbuf != NULL)
            {
                imageSize = hr_skybmp_get_data(fHandle, iconbuf, dataLen);
            }
        }
        else
        {
            SFS_GetFileSize(fHandle, &dataLen);
            iconbuf = (uint8*)SCI_ALLOCA(dataLen);
            if(iconbuf != NULL)
            {
                SFS_ReadFile(fHandle, (void*)iconbuf, dataLen, &imageSize, NULL);
				GUIANIM_GetInfo(NULL, fullpath, PNULL, &img_info);
				imgW = img_info.img_width;
				imgH = img_info.img_height;
            }
        }
        hr_skybmp_close(fHandle);
    	dynamic_item_ptr->dynamic_menu_info.has_icon = TRUE;
    	dynamic_item_ptr->dynamic_menu_info.icon_datasize = imageSize;
        dynamic_item_ptr->dynamic_menu_info.icon_ptr = (uint8 *)iconbuf;
        dynamic_item_ptr->dynamic_menu_info.icon_height = imgH;
        dynamic_item_ptr->dynamic_menu_info.icon_width  = imgW;
        dynamic_item_ptr->dynamic_menu_info.select_icon_id= IMAGE_MAINMENU_ICON_SELECT;
    }
    else
#endif
    {
        dynamic_item_ptr->dynamic_menu_info.has_icon = FALSE;
    }

	if(appinfo->title_mt != NULL)
	{
		hr_str_convert_endian(appinfo->title_mt);
	}
    MMIAPICOM_Wstrcpy(dynamic_item_ptr->dynamic_menu_info.text, appinfo->title_mt);
    dynamic_item_ptr->dynamic_menu_info.text_len = MIN(MMIAPICOM_Wstrlen(appinfo->title_mt), GUIMENU_DYMAINMENU_STR_MAX_NUM);

    dynamic_item_ptr->dynamic_menu_info.link_function_ptr  = hr_scut_start_app;
    dynamic_item_ptr->dynamic_menu_info.delete_function_ptr= hr_scut_delete_app;
    dynamic_item_ptr->dynamic_menu_info.param1ptr = &appinfo->appid;
    dynamic_item_ptr->dynamic_menu_info.param2ptr = &dynamic_item_ptr->menu_id;
    dynamic_item_ptr->dynamic_menu_info.parm1_size = 4;
    dynamic_item_ptr->dynamic_menu_info.parm2_size = 4;
    
    ret = MMIMENU_DyAppendItem(dynamic_item_ptr, store_idx);

    MMIHR_Trace(("[SKYSHORTCUT] hr_scut_install_app: menu_id = %d ret = %d", dynamic_item_ptr->menu_id, ret));

    if(dynamic_item_ptr != NULL)
    {
        SCI_FREE(dynamic_item_ptr);
    }

    if(iconbuf != NULL)
    {
        SCI_FREE(iconbuf);
    }

    if(ret == TRUE)
    {
        hr_scut_save_data(appinfo->appid, appinfo);
        return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_scut_update_progress(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	HR_SHORTCUT_PROGRESS_T* progress = (HR_SHORTCUT_PROGRESS_T*)input;

	return HR_IGNORE;
}

int32 hr_scut_remove_app(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    int32 nAppNameLen = 0;
    BOOLEAN   ret = TRUE;
    GUIMENU_MAINMENU_DYNAMIC_INFO_T dynamic_item_info = {0};
	HR_SHORTCUT_REMOVE_T* pRemoveInfo = (HR_SHORTCUT_REMOVE_T*)input;
    HR_SHORTCUT_APPINFO_T appInfo = {0};
    
	if(!input || input_len != sizeof(HR_SHORTCUT_REMOVE_T))
		return HR_FAILED;

    if(HR_SUCCESS != hr_scut_read_data(pRemoveInfo->appid, &appInfo))
    {
        return HR_FAILED;
    }

    nAppNameLen = MMIAPICOM_Wstrlen(appInfo.title_mt);
    nAppNameLen = nAppNameLen>GUIMENU_DYMAINMENU_STR_MAX_NUM?GUIMENU_DYMAINMENU_STR_MAX_NUM:nAppNameLen;
    MMIAPICOM_Wstrncpy(dynamic_item_info.dynamic_menu_info.text, appInfo.title_mt, nAppNameLen);
    dynamic_item_info.dynamic_menu_info.link_function_ptr = hr_scut_start_app;

    ret = MMIMENU_DyDeleteItem(&dynamic_item_info);
    MMIHR_Trace(("[SKYSHORTCUT] hr_scut_remove_app: appid = %d provider = %d ret = %d", pRemoveInfo->appid, pRemoveInfo->provider, ret));

    if(ret == TRUE)
    {
        hr_scut_remove_data_file(pRemoveInfo->appid);
    	return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

void test_hero_install(void)
{
	HR_SHORTCUT_APPINFO_T appinfo = {0};

	memset(&appinfo, 0, sizeof(appinfo));

	appinfo.appid = 10000;
	strcpy(appinfo.title_mt, L"测试");
	strcpy(appinfo.title_en, "test");
	strcpy(appinfo.image_path, "test.jpg");
	strcpy(appinfo.filepath, "test.ho");
    strcpy(appinfo.start_data, "test,10000");

	MMIHEROAPP_setDefFilePath();
	hr_scut_install_app((uint8*)&appinfo, sizeof(appinfo), NULL, NULL, NULL);
}

void test_hero_remove(void)
{
	HR_SHORTCUT_REMOVE_T appinfo;

	memset(&appinfo, 0, sizeof(appinfo));

	appinfo.appid = 10000;

	hr_scut_remove_app((uint8*)&appinfo, sizeof(appinfo), NULL, NULL, NULL);
}
#endif

int32 hr_setting_get_hw_version(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
   const char *hw_ver = NULL;

	if(output == NULL||output_len == NULL)
		return HR_FAILED;

#if defined(PLATFORM_SC6530)
    hw_ver = "SC6530";
#elif defined(PLATFORM_SC6600L)
    hw_ver = "SC6600L";
#elif defined(PLATFORM_SC6600R)
    hw_ver = "SC6600R";
#elif defined(PLATFORM_SC6800H)
    hw_ver = "SC800H";
#else
    return HR_IGNORE;
#endif

	*output =(uint8*) hw_ver;
	*output_len = strlen((char*)*output)+1;
    
	MMIHR_Trace(("hr_setting_get_hw_version hw_ver=%s len=%d", *output, *output_len));
	return HR_SUCCESS;
}
LOCAL int32 MMIHEROAPP_mutiCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 resValue = HR_SUCCESS;
	MMIHR_Trace(("MMIHEROAPP_mutiCmd() code:%d", code));
	
	switch(code)
	{	
#ifdef __HERO_ENGINE_OVERSEA__
		case HR_GET_DATA_ACCOUNT:
		{
			if(output == NULL||output_len == NULL)
				return HR_FAILED;
			*output = (uint8 *)&gHeroDataAccount;
			*output_len = sizeof(gHeroDataAccount);
			return HR_SUCCESS;
		}
    
		case HR_SET_DATA_ACCOUNT:
		{
			if(input == NULL||input_len != (int32)sizeof(T_HERO_DATA_ACCOUNT))
				return HR_FAILED;
			return MMIHEROAPP_setApnAccount((T_HERO_DATA_ACCOUNT*)input);
		}
#endif
	case HR_PLATEX_GET_VM_TYPE:
		return HR_PLAT_VALUE_BASE + 1;
		
	case HR_PLATEX_CREATE_LIST:
		{
			MMIHR_LISTBOXSELECT_T *list = (MMIHR_LISTBOXSELECT_T*)input;
			return MMIHEROAPP_listCreate((const char *)list->title, list->itemNum, list->listType);
		}
    
	case HR_PLATEX_SET_LISTITEM:
		{
			MMIHR_LISTBOXITEM_T *item = (MMIHR_LISTBOXITEM_T*)input;
			return MMIHEROAPP_menuSetItem(item->menu, (const char*)item->text, item->index, item->selected);
		}
    
	case HR_PLATEX_GET_MP3_DIR:
        {
            SCI_MEMSET(hr_ucs2_name_arr, 0x00, sizeof(hr_ucs2_name_arr));
            *output_len = GUI_WstrToGB((uint8*) hr_ucs2_name_arr, (uint16*)MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC));
            //SCI_TRACE_LOW:"MP3: %d 0x%02x 0x%02x 0x%02x 0x%02x"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMRAPP_FUNC_6212_112_2_18_3_12_45_0,(uint8*)"ddddd", *output_len, hr_ucs2_name_arr[0], hr_ucs2_name_arr[1], hr_ucs2_name_arr[2], hr_ucs2_name_arr[3]);
			*output = (uint8*)hr_ucs2_name_arr;
			break;
        }
        
    case HR_PLATEX_GET_PIC_DIR:
        {
            SCI_MEMSET(hr_ucs2_name_arr, 0x00, sizeof(hr_ucs2_name_arr));
            *output_len = GUI_WstrToGB((uint8*) hr_ucs2_name_arr, (uint16*)MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE));
            //SCI_TRACE_LOW:"PIC: %d 0x%02x 0x%02x 0x%02x 0x%02x"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMRAPP_FUNC_6221_112_2_18_3_12_45_1,(uint8*)"ddddd", *output_len , hr_ucs2_name_arr[0], hr_ucs2_name_arr[1], hr_ucs2_name_arr[2], hr_ucs2_name_arr[3]);
    		*output = (uint8*)hr_ucs2_name_arr;
    		break;
        }
    
	case HR_PLATEX_APP_BACKRUN_IND:
        {
            hr_backstage_st *ptr = (hr_backstage_st*)input;
    		MMIHR_Trace((" HR_PLATEX_APP_BACKRUN_IND g_hr_vmBackRun = %d, g_hr_curApp = %d, param = %d", g_hr_vmBackRun, g_hr_curApp, ptr->appid));
    		if(g_hr_vmBackRun == HERO_APP_MAX && ptr != NULL && ptr->appid > 0)
    		{
    			g_hr_vmBackRun = ptr->appid;
    		    MMIHEROAPP_closeAllWins();
    			/*如果mp3是在启动hr 应用时pause的，恢复*/
    			s_hr_mp3_resume_locked = FALSE;
				MMIHEROAPP_resumeMp3();

                /* 为后台应用添加默认图标 */
                s_hr_backrun_info.app_id = ptr->appid;
                s_hr_backrun_info.icon_state = HEROAPP_STATE_ONLINE;
		#if !defined(PLATFORM_UIX8910)		
                MAIN_SetIdleHeroappState(TRUE); /*lint !e718 !e18*/
              #endif
    		}
    		else
    		{
    			resValue = HR_FAILED;
    		}
    		break;
       }
        
	case HR_PLATEX_APP_PICIND_REQ:

		MMIHR_Trace(("QQ HR_PLATEX_APP_PICIND_REQ g_hr_vmBackRun = %d, g_hr_curApp = %d", g_hr_vmBackRun, g_hr_curApp));

		if(g_hr_vmBackRun != HERO_APP_MAX || input != NULL) /*lint !e831*/
		{
			hr_pic_req *pic = (hr_pic_req*)input;
            s_hr_backrun_info.app_id = pic->appid;
            if(pic->time == -1) //离线
            {
                s_hr_backrun_info.icon_state = HEROAPP_STATE_OFFLINE;
            }
            else if(pic->time == 1 || pic->time == 1001)  //离开
            {
                s_hr_backrun_info.icon_state = HEROAPP_STATE_AWAY;
            }
            else if(pic->time == 2 || pic->time == 1002)  //隐身
            {
                s_hr_backrun_info.icon_state = HEROAPP_STATE_INVISIBLE;
            }
            else//在线
            {
                s_hr_backrun_info.icon_state = HEROAPP_STATE_ONLINE;
            }
	     #if !defined(PLATFORM_UIX8910)		
            MAIN_SetIdleHeroappState(TRUE); /*lint !e718 !e18*/
            #endif
            if(pic->time >= 1000) //有消息
            {
                MMIHEROAPP_StartStatusIconTimer();
            }
		}
		else
		{
			resValue = HR_FAILED;
		}

		break;
        
#ifdef HR_USE_PLAT_LCD_BUFFER
	case HR_PLATEX_GET_LCD_INFO://获取LCD buffer指针与大小
		{
#ifndef UI_MULTILAYER_SUPPORT
			BOOLEAN    ret = FALSE;
			LCD_INFO_T lcd_info = {0};
            ret = GUILCD_GetInfo(GUI_MAIN_LCD_ID, &lcd_info);  
			if(ret == TRUE)
			{
				*output = (uint8 *)lcd_info.lcdbuff_ptr;
				*output_len = (int32)(lcd_info.lcd_width*lcd_info.lcd_height*lcd_info.bits_per_pixel/8);
			}
			else
			{
				resValue = HR_FAILED;
			}
#else
            GUI_LCD_DEV_INFO *lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();
            UILAYER_INFO_T   layer_info = {0};
            UILAYER_GetLayerInfo(lcd_dev_ptr, &layer_info);
#if (HERO_SPR_VERSION >= 0x10A1140)
			{
				GUI_COLOR_T *lcd_buffer_ptr = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr(lcd_dev_ptr);
				MMIHR_Trace(("mrapp layer_buf_ptr = 0x%08x width = %d height = %d mem_width = %d", lcd_buffer_ptr, layer_info.layer_width, layer_info.layer_height, layer_info.mem_width));
				if(output != NULL && output_len != NULL && lcd_buffer_ptr != NULL)
				{
					*output = (uint8 *)lcd_buffer_ptr;
					*output_len = (int32)(layer_info.layer_width*layer_info.layer_height*2);
				}
				else
				{
					resValue = HR_FAILED;
				}
			}
#else
            MMIHR_Trace(("mrapp layer_buf_ptr = 0x%08x width = %d height = %d mem_width = %d", layer_info.layer_buf_ptr, layer_info.layer_width, layer_info.layer_height, layer_info.mem_width));
			if(output != NULL && output_len != NULL && layer_info.layer_buf_ptr != NULL)
			{
				*output = (uint8 *)layer_info.layer_buf_ptr;
				*output_len = (int32)(layer_info.layer_width*layer_info.layer_height*2);
			}
			else
			{
				resValue = HR_FAILED;
			}
#endif
#endif
		}
		break;
	case HR_PLATEX_FREE_LCD_BUF:
		return HR_SUCCESS;
#endif

	case HR_PLATEX_ALLOC_MEM:
		{
            uint8 *buf = NULL;
            if(output == NULL||output_len == NULL||input_len == 0)
            {
                resValue = HR_FAILED;
            }
            else
            {
                buf = (uint8*)SCI_ALLOCA(input_len);
                if(buf != NULL)
                {
                    SCI_MEMSET(buf, 0x00, input_len);
                    *output = (uint8 *)buf;
                    *output_len =sizeof(int32);
                    resValue = HR_SUCCESS;
                }
                else
                {
                    resValue = HR_FAILED;
                }
            }
            MMIHR_Trace(("mrapp HR_PLATEX_ALLOC_MEM len = %d, buf = 0x%08x, resValue = %d", input_len, buf, resValue));
		}
		break;
        
	case HR_PLATEX_FREE_MEM:
        {
            if(input != NULL)
            {
                SCI_FREE(input);
                input = NULL;
                resValue = HR_SUCCESS;
            }
            else
            {
                resValue = HR_FAILED;
            }
            MMIHR_Trace(("mrapp HR_PLATEX_FREE_MEM resValue = %d", resValue));
        }
		break;

    case HR_PLATEX_GET_BUILD_DT:
        {
			if(output == NULL || output_len == NULL)
            {         
				return HR_FAILED;
            }
            else
            {
                return MMIHEROAPP_GetBuildTime(output, output_len);
            }
        }
       

#ifndef WIN32
	case HR_PLATEX_SOCKET_BIND://socket－bind
		{
			HRAPP_BIND_T *info = (HRAPP_BIND_T*)input;
			struct sci_sockaddr  addr;
			int ret;
			
			addr.family = AF_INET;
			addr.port = htons(info->port);
			addr.ip_addr = (uint32)sci_gethostaddress();
			SCI_MEMSET((void*)addr.sa_data, 0, 8*sizeof(char));
			
			ret = sci_sock_bind((long)info->socket, &addr, sizeof(struct sci_sockaddr));
			if(ret != 0)
			{
				resValue = HR_FAILED;
			}
		}
		break;
#endif

#ifdef __HR_SEND_MMS_SUPPORT__
    case HR_PLATEX_SEND_MMS:
        return hr_sms_send_mms(input, input_len, output, output_len, cb);
#endif

	case HR_PLATEX_SET_FILE_DEV://存储路径选择
		if(input != NULL && input_len > 0)
		{
			uint8 *ptr = input;
			uint16 len;
			const uint8 *device_ptr;
			uint16 device_len;
			FILE_DEV_E_T device_type;
			char* tempPtr = (char*)hr_tempTransBuf; /*lint !e831*/
			
			MMIHR_Trace(("input: %s input_len:%d output:0x%x output_len:0x%x cb:0x%x", input, input_len, output, output_len, cb));
			switch((char)ptr[0])
			{
			case 'A':case 'a':
				device_type = FS_HS;
				device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_SYS_NAME;
				device_len = HERO_MMIFILE_DEVICE_SYS_NAME_LEN;
				break;
			case 'B':case 'b':
				device_type = FS_UDISK;
				device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_UDISK;
				device_len = HERO_MMIFILE_DEVICE_UDISK_LEN;
				break;
			case 'C':case 'c':
#ifdef DUAL_TCARD_SUPPORT
                if(TRUE == MMIFILE_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
                {
    				device_type = FS_MMC;
    				device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD;
    				device_len = HERO_MMIFILE_DEVICE_SDCARD_LEN;
                }
                else if(TRUE == MMIFILE_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
                {
                    device_type = FS_MMC;
                    device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD2;
                    device_len = HERO_MMIFILE_DEVICE_SDCARD2_LEN;
                }
                else
#endif
                {
    				device_type = FS_MMC;
    				device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD;
    				device_len = HERO_MMIFILE_DEVICE_SDCARD_LEN;
                }
				break;
#ifdef DOUBLE_TFCARD
			case 'D':case 'd':
				device_type = FS_MMC_2;
				device_ptr = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD_2;
				device_len = HERO_MMIFILE_DEVICE_SDCARD_LEN_2;
				break;
#endif
			case 'Y':case 'y':
				if(output != NULL && output_len != NULL)
				{
    				MMIHR_Trace(("mrapp device type %d", g_hr_global.device_type));
					SCI_MEMSET(tempPtr, 0, sizeof(hr_tempTransBuf));
					if(g_hr_global.device_type == FS_MMC)
						strcpy((char*)tempPtr, "C:\\");
					else if(g_hr_global.device_type == FS_UDISK)
						strcpy((char*)tempPtr, "B:\\");
					else
						strcpy((char*)tempPtr, "A:\\");
					
					if(g_hr_global.dir_len != 0)
					{
						GUI_WstrToGB((uint8*)tempPtr+3, g_hr_global.dir_ptr, g_hr_global.dir_len/2); /*lint !e64*/
						strcat((char*)tempPtr, "\\");
					}
					len = strlen((char*)tempPtr);
					MMIHEROAPP_ConvertPath2Loc((char*)tempPtr, len);
					
					MMIHR_TraceF(("after tempPtr: %s", (char*)tempPtr));
					*output = (uint8*)tempPtr;
					*output_len = len;
                    if(cb != NULL)
                    {
    					*cb = NULL;
                    }
					return HR_SUCCESS;
				}
				else
				{
					return HR_FAILED;
				}
			case 'X':case 'x':
			case 'Z':case 'z':
				MMIHEROAPP_setDefFilePath();
				return HR_SUCCESS;
			default:
				return HR_FAILED;
			}	
			
			if(!MMIAPIFMM_GetDeviceStatus((uint8*)device_ptr, device_len)) /*lint !e64*/
				return HR_FAILED;
			MMIHR_TraceF(("mrapp get device status OK"));
            if(!hr_fs_check_path_valid(input, input_len))
            {
                MMIHR_TraceF(("mrapp mr_fs_check_path_invalid "));
                return HR_FAILED;
            }
			g_hr_global.device_type = device_type;
			memset(g_hr_global.dir_buf, 0, sizeof(g_hr_global.dir_buf));
			g_hr_global.dir_ptr = (uint8*)g_hr_global.dir_buf;
			g_hr_global.dir_len = 0;
			if(input_len > 3 && ptr[1] == ':' && (ptr[2] == '\\' || ptr[2] == '/'))//"C:\App\"-->"/App"
			{
				len = input_len - 3;
				if(len >= HERO_MMIFILE_DIR_NAME_MAX_LEN_AD/2)//simple deal
					return HR_FAILED;
				
				SCI_MEMCPY((char*)tempPtr, &ptr[3], len+1);//add '\0'
				MMIHEROAPP_ConvertPath2Loc((char*)tempPtr, len);
				if(tempPtr[len-1] == '/')  /*lint !e676*/
					tempPtr[len-1] = '\0'; /*lint !e676*/
				g_hr_global.dir_len = MMIHEROAPP_GB2UCS((uint16*)g_hr_global.dir_ptr, (const uint8 *)tempPtr, strlen((char*)tempPtr));
			}
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
		
	case HR_PLATEX_UCS_TO_GB:
		{
			char *temp = NULL;
			int32 lenth = 0;
			uint16 len;
			uint8 *out = (uint8*)*output;
#ifndef WIN32
			if(input != NULL)
			{
				len = hr_str_wstrlen((char*)input) + 2;
				temp = (char*)SCI_ALLOCA(len);
				SCI_MEMCPY(temp, input, len);
				hr_str_convert_endian((char*)temp);
			}
#endif
			len = GUI_WstrToGB(out, (const uint8*)temp, MMIHEROAPP_getUcstrLen(input)/2); /*lint !e64*/
			out[len] = 0x00;
			MMIHR_Trace(("len: %d", len));
			SCI_FREE(temp);
		}
		break;

	case HR_PLATEX_SET_RINGS:
		resValue = MMIHEROAPP_SetMp3AsRing((HR_RING_SET_T*)input);
		break;

#ifdef HERO_APP_SIMUGAME_OPEN
	case HR_PLATEX_GET_NES_KEYSCONFIG://获取平台默认按键设置for游戏模拟器
		{
			hr_nes_keys_st keys = NES_DEFAULT_KEYSET;
			hr_nes_keys_st *defaultKeys = (hr_nes_keys_st*)hr_tempTransBuf;
			memcpy(defaultKeys, &keys, sizeof(hr_nes_keys_st));
			*output = (uint8*)defaultKeys;
			*output_len = sizeof(hr_nes_keys_st);
		}
		break;
		
	case HR_PLATEX_GET_NES_DIR_NAME:
		*output = (uint8*)HERO_ENGINE_NES_DIR_NAME;
		*output_len = sizeof(HERO_ENGINE_NES_DIR_NAME)-1;
		break;
#endif

    case HR_PLATEX_GET_NES_TITLE_NAME:
        {
            if(output == NULL || output_len == NULL)
                return HR_FAILED;
        	*output = (uint8 *)HERO_APP_APPLIST_TITLE;
        	*output_len = strlen(HERO_APP_APPLIST_TITLE);
        }
        break;

	case HR_PLATEX_LCD_SLEEP_OFF:
        return MMIHEROAPP_SetAllowTurnOffBacklight(FALSE);

	case HR_PLATEX_LCD_SLEEP_ON:
        return MMIHEROAPP_SetAllowTurnOffBacklight(TRUE);
        
	case HR_PLATEX_GET_CELL_INFO:
		{
			char temp[4] = {0};
			MN_PHONE_CURRENT_PLMN_INFO_T info = MNPHONE_GetCurrentPLMNInfoEx(MMIHEROAPP_GetActiveSim());
			HRAPP_CELL_INFO_T_V2 *cells = (HRAPP_CELL_INFO_T_V2*)hr_tempTransBuf;
            MMIHR_Trace(("mrapp HR_PLATEX_GET_CELL_INFO plmn_status=%d, plmn_is_roaming=%d, mcc=%d, mnc=%d, lac=%d, cell_id=%d, mnc_digit_num=%d", \
                info.plmn_status, info.plmn_is_roaming, info.mcc, info.mnc, info.lac, info.cell_id, info.mnc_digit_num));
			if(output == NULL||output_len == NULL)
            {         
				return HR_FAILED;
            }
			memset(cells, 0, sizeof(HRAPP_CELL_INFO_T_V2));
			cells->lac = info.lac;
			cells->cell_id = info.cell_id;		
			sprintf((char*)temp, "%03d", info.mnc);
			memcpy(cells->mnc, temp, 2);
			memcpy(cells->mnc3, &temp[2], 1);
			sprintf((char*)temp, "%03d", info.mcc);
			memcpy(cells->mcc, temp, 3);
			*output = (uint8*)cells;
			*output_len = sizeof(HRAPP_CELL_INFO_T_V2);
			return HR_SUCCESS;
		}

	case HR_PLATEX_GET_NETWORK_ID:
		{
            if(output == NULL||output_len == NULL)
            {         
                return HR_FAILED;
            }
            else
            {
                return MMIHEROAPP_GetNetworkID(output, output_len);
            }
		}

    case HR_PLATEX_GET_FILE_CRTTIME:
        return 	MMIHEROAPP_GetFileCreateTime(input, input_len, output, output_len);
                
	case HR_PLATEX_GET_DISK_SPACE:		
        return hr_fs_get_free_space(input, input_len, output, output_len, cb);
        
	case HR_PLATEX_GET_DISK_NAME://unicode
		{
			MMISET_LANGUAGE_TYPE_E language_type;
			MMIAPISET_GetLanguageType(&language_type);
			
			resValue = HR_FAILED;
			switch((char)input[0])
			{
			case 'A':case 'a':	
				if(MMIAPIFMM_GetDeviceStatus((uint8 *)HERO_MMIFILE_DEVICE_SYS_NAME, HERO_MMIFILE_DEVICE_SYS_NAME_LEN)) /*lint !e64*/
				{
					if(language_type == MMISET_LANGUAGE_SIMP_CHINESE)
					{
						*output = (uint8*)"\x7c\xfb\x7e\xdf\x76\xd8\x00\x00";//系统盘
						*output_len = sizeof("\x7c\xfb\x7e\xdf\x76\xd8\x00\x00")-1; 
					}
					else
					{
						*output = (uint8*)"\x00\x53\x00\x79\x00\x73\x00\x64\x00\x69\x00\x73\x00\x6b\x00\x00";//Sysdisk
						*output_len = sizeof("\x00\x53\x00\x79\x00\x73\x00\x64\x00\x69\x00\x73\x00\x6b\x00\x00")-1; 
					}					
					resValue = HR_SUCCESS;
				}
				break;
			case 'B':case 'b':
				if(MMIAPIFMM_GetDeviceStatus((uint8*)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN)) /*lint !e64*/
				{
					if(language_type == MMISET_LANGUAGE_SIMP_CHINESE)
					{
						*output = (uint8*)"\x62\x4b\x67\x3a\x51\x85\x5b\x58\x00\x00";//手机内存
						*output_len = sizeof("\x62\x4b\x67\x3a\x51\x85\x5b\x58\x00\x00")-1; 
					}
					else
					{
						*output = (uint8*)"\x00\x55\x00\x64\x00\x69\x00\x73\x00\x6b\x00\x00";//Udisk
						*output_len = sizeof("\x00\x55\x00\x64\x00\x69\x00\x73\x00\x6b\x00\x00")-1; 
					}					
					resValue = HR_SUCCESS;
				}
				break;
			case 'C':case 'c':	
#if (HERO_SPR_VERSION >= 0x10A1140)
#ifdef DUAL_TCARD_SUPPORT
                if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN)
                   || TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
#else
				if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
#endif
#else
				if(MMIAPISD_GetStatus())
#endif
				{
#ifdef DOUBLE_TFCARD
					if(language_type == MMISET_LANGUAGE_SIMP_CHINESE)
					{
						*output = (uint8*)"\x5b\x58\x50\xa8\x53\x61\x00\x2d\x00\x31\x00\x00";//存储卡-1
						*output_len = sizeof("\x5b\x58\x50\xa8\x53\x61\x00\x2d\x00\x31\x00\x00")-1; 
					}
					else
					{
						*output = (uint8*)"\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x31\x00\x00";//T-card1
						*output_len = sizeof("\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x31\x00\x00")-1; 
					}					
#else
					if(language_type == MMISET_LANGUAGE_SIMP_CHINESE)
					{
						*output = (uint8*)"\x5b\x58\x50\xa8\x53\x61\x00\x00";//存储卡
						*output_len = sizeof("\x5b\x58\x50\xa8\x53\x61\x00\x00")-1; 
					}
					else
					{
						*output = (uint8*)"\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x00";//T-card
						*output_len = sizeof("\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x00")-1; 
					}					
#endif
					resValue = HR_SUCCESS;
				}
				break;
#ifdef DOUBLE_TFCARD
			case 'D':case 'd':	
				if(MMIAPISD2_GetStatus())
				{
					if(language_type == MMISET_LANGUAGE_SIMP_CHINESE)
					{
						*output = (uint8*)"\x5b\x58\x50\xa8\x53\x61\x00\x2d\x00\x32\x00\x00";//存储卡-2
						*output_len = sizeof("\x5b\x58\x50\xa8\x53\x61\x00\x2d\x00\x32\x00\x00")-1; 
					}
					else
					{
						*output = (uint8*)"\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x32\x00\x00";//T-card2
						*output_len = sizeof("\x00\x54\x00\x2d\x00\x63\x00\x61\x00\x72\x00\x64\x00\x32\x00\x00")-1; 
					}
					resValue = HR_SUCCESS;
				}
				break;
#endif
			default:
				break;
			}
		}
		break;
        
	case HR_PLATEX_GET_DUALSIM_INFO:
		{
            int32 i = 0;
			HR_DUALSIM_SET_T *simInfo = (HR_DUALSIM_SET_T*)hr_tempTransBuf;
			memset(simInfo, 0, sizeof(HR_DUALSIM_SET_T));

            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIPHONE_IsSimOk(i))
    			{
                    simInfo->simId[simInfo->totalNum] = -(i+1);
                    if(i == 0)
    				{
                        simInfo->simNamePtr[simInfo->totalNum] = (char*)"\x00\x53\x00\x49\x00\x4d\x00\x31\x00\x00";
    					simInfo->simNameLen[simInfo->totalNum] = sizeof("\x00\x53\x00\x49\x00\x4d\x00\x31\x00\x00")-1;
                    }
                    else if(i == 1)
                    {
                        simInfo->simNamePtr[simInfo->totalNum] = (char*)"\x00\x53\x00\x49\x00\x4d\x00\x32\x00\x00";
                        simInfo->simNameLen[simInfo->totalNum] = sizeof("\x00\x53\x00\x49\x00\x4d\x00\x32\x00\x00")-1;
                    }
                    else if(i == 2)
                    {
                        simInfo->simNamePtr[simInfo->totalNum] = (char*)"\x00\x53\x00\x49\x00\x4d\x00\x33\x00\x00";
                        simInfo->simNameLen[simInfo->totalNum] = sizeof("\x00\x53\x00\x49\x00\x4d\x00\x33\x00\x00")-1;
                    }   
                    else if(i == 3)
                    {
                        simInfo->simNamePtr[simInfo->totalNum] = (char*)"\x00\x53\x00\x49\x00\x4d\x00\x34\x00\x00";
                        simInfo->simNameLen[simInfo->totalNum] = sizeof("\x00\x53\x00\x49\x00\x4d\x00\x34\x00\x00")-1;
                    }  
					simInfo->totalNum++;
				}
                else
                {
                    if(g_hr_dualsys_mng.simSel == i)
    				{
                        g_hr_dualsys_mng.simSel = (i+1)%MMI_DUAL_SYS_MAX;
                    }
                }
			}
				
			resValue = HR_SUCCESS;
            *output = (uint8*)simInfo;
            *output_len = sizeof(HR_DUALSIM_SET_T); 
			
            for(i = 0; i < sizeof(simInfo->simId)/sizeof(int32); i++) /*lint !e574 !e737*/
			{
				if(simInfo->simId[i] == -(g_hr_dualsys_mng.simSel + 1))
				{
					simInfo->curActive =i;
					break;
				}
			}

            MMIHR_Trace(("simInfo total:%d, simSel:%d, curActive:%d simId:%d %d %d %d",
                          simInfo->totalNum, g_hr_dualsys_mng.simSel, simInfo->curActive, 
                          simInfo->simId[0], simInfo->simId[1], simInfo->simId[2], simInfo->simId[3]));
		}
		break;

#ifdef UI_MULTILAYER_SUPPORT
    case HR_PLATEX_SET_BLT_LAYER:
        return hr_layer_set_blt_layer(input, input_len, output, output_len, cb);
    case HR_PLATEX_CREATE_LAYER:
        return hr_layer_create(input, input_len, output, output_len, cb);
    case HR_PLATEX_GET_BASE_LAYER:
        return hr_layer_get_base_handle(input, input_len, output, output_len, cb);
    case HR_PLATEX_SET_LAYER_POS:
        return hr_layer_set_postion(input, input_len, output, output_len, cb);
    case HR_PLATEX_SET_PNGBG_LAYER:
        return HR_IGNORE;
    case HR_PLATEX_GET_ACTICE_LAYER:
        return hr_layer_get_active_layer(input, input_len, output, output_len, cb);
    case HR_PLATEX_LAYER_APPEND:
        return HR_IGNORE;
    case HR_PLATEX_LOCK_INVALIDATE:
        return HR_IGNORE;
    case HR_PLATEX_UNLOCK_INVALIDATE:
        return HR_IGNORE;
#endif

    case HR_PLATEX_BROWSE_HERO_FMGR_FILTER_INIT:
        return MMHEROAPP_FileMgrInit();

    case HR_PLATEX_BROWSE_HERO_FMGR_FILTER_SET:
        return MMHEROAPP_FileMgrSetFilter(*(int32*)input);

    case HR_PLATEX_BROWSE_HERO_FMGR_FILTER_CLEAR:
        return MMHEROAPP_FileMgrCleanFilter(*(int32*)input);

    case HR_PLATEX_BROWSE_FMGR:
        return MMHEROAPP_FileMgrBrowser((HR_T_FMGR_REQ*)input);

    case HR_PLATEX_BROWSE_FMGR_GET_PATH:
        return MMHEROAPP_FileMgrGetPath(output, output_len);

    case HR_PLATEX_BROWSE_FMGR_EXIT:
        return MMHEROAPP_FileMgrExit();

	case HR_PLATEX_CREATE_PB_OBJ:
		resValue = HR_FAILED;
		if(g_hr_pbinfo == NULL)
		{
			g_hr_pbinfo = (HERO_APP_PBINFO_T*)SCI_ALLOCA(sizeof(HERO_APP_PBINFO_T));
			if(g_hr_pbinfo != NULL)
			{
				SCI_MEMSET(g_hr_pbinfo,0,sizeof(HERO_APP_PBINFO_T));
				//s_hr_inMrSelfWinCreating = TRUE;
				s_hr_PBOfCancelMsg = TRUE;
				s_hr_isWindowOfPB = TRUE;
				if(MMIAPIPB_OpenMainWin(MMIPB_ALL_CONTACT, 
										MMIPB_LIST_RADIO, 
										MMIPB_LIST_FIELD_WHOLE, 
										1, NULL, MMIHEROAPP_SelectPBContactCallback))
				{
					resValue = HR_SUCCESS;
				}
				else
				{
					SCI_FREE(g_hr_pbinfo);
					g_hr_pbinfo = NULL;
				}		

				s_hr_isWindowOfPB = FALSE;
				//s_hr_inMrSelfWinCreating = FALSE;
			}
		}			
		break;

	case HR_PLATEX_GET_A_PB_INFO:
		if(g_hr_pbinfo != NULL && g_hr_pbinfo->result != HR_FAILED)
		{
#ifndef WIN32
			/* 大小端转换 */
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.name, HR_PHB_MAX_NAME);
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.number, HR_PHB_MAX_NUMBER);
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.homeNumber, HR_PHB_MAX_NUMBER);
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.officeNumber, HR_PHB_MAX_NUMBER);
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.faxNumber, HR_PHB_MAX_NUMBER);
			hr_str_convert_endian_with_size(g_hr_pbinfo->info.emailAddress, HR_PHB_MAX_NUMBER);
#endif
			
			*output = (uint8*)&g_hr_pbinfo->info;
			*output_len = sizeof(T_HR_PHB_ENTRY);
		}
		else
		{
			resValue = HR_FAILED;
		}
		break;
        
	case HR_PLATEX_CLOSE_PB_OBJ:
		if(g_hr_pbinfo != NULL)
		{
			SCI_FREE(g_hr_pbinfo);
			g_hr_pbinfo = NULL;
		}
		break;
        
	case HR_PLATEX_SET_TIMES:
		{
			HRAPP_MYTIME *time = (HRAPP_MYTIME*)input;
			SCI_TIME_T  sys_time;
			SCI_DATE_T  sys_date;
			sys_date.year = time->nYear;
			sys_date.mon = time->nMonth;
			sys_date.mday = time->nDay;
			sys_time.sec = time->nSec;
			sys_time.min = time->nMin;
			sys_time.hour = time->nHour;
            if(MMK_IsOpenWin(MMISET_SET_DATE_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_DATE_WIN_ID);
            }
            if(MMK_IsOpenWin(MMISET_SET_TIME_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_TIME_WIN_ID);
            }
			TM_SetSysDate(sys_date);
			TM_SetSysTime(sys_time);
			MMIAPISET_UpdateAlarmSet();
			MMIAPISET_SetNeedResetDateTime(FALSE);
	    #if !defined(PLATFORM_UIX8910)		
            MAIN_SetStbDispTime(); /*lint !e718 !e18*/
           #endif
		}
		break;

    case HR_PLATEX_LOCK_IRAM:
        {
#ifndef WIN32
            if(output == NULL || output_len == NULL)
            {
                return HR_FAILED;
            }

    		MMIAPISET_StopAllRing(TRUE);
    		MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);  //停止按键音
    		MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);  //停止触屏音
    		
    		s_hr_iramlock_handle = IRAM_LockArea((const IRAM_AREA *)&(s_hr_iram_area), 
    												sizeof(s_hr_iram_area)/sizeof(IRAM_AREA), 
    												IRAM_NO_WAIT);
    		
    		if(IRAM_INVALID_HANDLE != s_hr_iramlock_handle)//申请IRAM成功
    		{
 //               MMIDEFAULT_EnableKeyRing(TRUE);
                *(volatile uint32 *)0x8b00002c |= 0x01;
                *output = (uint8*)HERO_APP_IRAM_ADDR;
                *output_len = HERO_APP_IRAM_LEN;
    		}
    		else
    		{
    			resValue = HR_FAILED;//申请资源失败
    		}
            
            MMIHR_Trace(("mrapp HR_PLATEX_LOCK_IRAM handle: %d iram_addr: 0x%08x size: %d", s_hr_iramlock_handle, *output, *output_len));
#endif
        }
		break;
        
    case HR_PLATEX_UNLOCK_IRAM:
        {
#ifndef WIN32
            MMIHR_Trace(("mrapp HR_PLATEX_UNLOCK_IRAM handle: %d input: 0x%08x input_len: %d", s_hr_iramlock_handle, input, input_len));
            if(input == NULL || input_len == 0)
            {
                return HR_FAILED;
            }

    		//释放IRAM
    		if(IRAM_INVALID_HANDLE != s_hr_iramlock_handle)
    		{
    			IRAM_UnlockArea (s_hr_iramlock_handle);
    			s_hr_iramlock_handle = IRAM_INVALID_HANDLE;
    			*(volatile uint32 *)0x8b00002c &= ~(0x01);
    		}
#endif
        }
		break;

	/* begin:added by Tommy.yan 20120201 */
	case HR_PLATEX_GET_PB_TOTAL_NUM:
	    return MMIHEROAPP_PHBGetTotalNum(input, input_len, output, output_len); /*lint !e718 !e746*/

	//case HR_PLATEX_REPLY_SMS_REQ:
	    //return MMIMRAPP_ReplySmsByUI(input, input_len, output, output_len);

	//case HR_PLATEX_SAVE_SMS_INBOX:
	    //return MMIMRAPP_SaveSmsInbox(input, input_len, output, output_len);

	case HR_PLATEX_PHB_SEARCH_ENTRY:
		return MMIHEROAPP_PHBSearchEntry(input, input_len, output, output_len); /*lint !e718 !e746*/
		
	case HR_PLATEX_PHB_SET_ENTRY:
	    return MMIHEROAPP_PHBSetEntry(input, input_len, output, output_len); /*lint !e718 !e746*/

	case HR_PLATEX_PHB_GET_ENTRY:
		return MMIHEROAPP_PHBGetEntry(input, input_len, output, output_len); /*lint !e718 !e746*/

	case HR_PLATEX_PHB_COPY_ENTRY:
		 break;

	case HR_PLATEX_PHB_SET_OWNER_ENTRY:
		 break;

	case HR_PLATEX_PHB_GET_OWNER_ENTRY:
		 break;

	case HR_PLATEX_PHB_DELETE_ENTRY_ALL:
		 break;

	case HR_PLATEX_PHB_GET_COUNT:
		return MMIHEROAPP_PHBGetCount(input, input_len, output, output_len); /*lint !e718 !e746*/

	case HR_PLATEX_PHB_GET_STATUS:
		return MMIHEROAPP_PHBGetStatus(); /*lint !e718 !e746*/
		
	case HR_PLATEX_GET_SMS_INFO:
		return hr_sms_get_info(input, input_len, output, output_len);

	case HR_PLATEX_GET_SMS_CONTENT:
		return hr_sms_get_content(input, input_len, output, output_len);

	case HR_PLATEX_DELETE_SMS:
		return hr_sms_delete(input, input_len, output, output_len);

#ifdef __HR_SMS_SET_STATE_SUPPORT__
	case HR_PLATEX_SET_SMS_STATE:
		return hr_sms_set_state(input, input_len, output, output_len);
#endif

	case HR_PLATEX_SAVE_SMS:
		return hr_sms_save_sms(input, input_len, output, output_len);

	case HR_PLATEX_REPLY_SMS_REQ:
		return hr_sms_replay_by_UI(input, input_len, output, output_len);

    case HR_PLATEX_SEND_SMS_LOCALUI:
        return hr_sms_send_sms_from_ui(input, input_len, output, output_len);

   case HR_PLATEX_CHECK_WIFI_SUPPORT:
        return hr_setting_wifi_exist(input, input_len, output, output_len, cb);
	
#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__
    case HR_PLATEX_GET_HEADSET_EX:
        return hr_setting_get_handsetID_ex(input, input_len, output, output_len, cb);
#endif
		
    case HR_PLATEX_SET_WALLPAPER:
        return hr_setting_set_wallpaper(input, input_len, output, output_len, cb);
    case HR_PLATEX_GET_ABLUM_PATH:
        return hr_setting_get_system_image_path(input, input_len, output, output_len, cb);

    case HR_PLATEX_IMAGE_ENCODE:
        return hr_setting_image_encode(input, input_len, output, output_len, cb);

    case HR_PLATEX_GET_TOUCHSCREEN_TYPE:
        return hr_setting_get_touchscreen_type(input, input_len, output, output_len, cb);

    case HR_PLATEX_GET_CAMERA_SUPPORT:
        return hr_setting_get_camera_support(input, input_len, output, output_len, cb);
		
#ifdef __HR_CFG_LOCALAPP_MANAGER__
    case HR_PLATEX_START_LOCALAPP:
        return hr_start_plat_application(input);
#endif

#ifdef __HR_CFG_MULTITP_SUPPORT__
    case HR_PLATEX_GET_TP_POINTS:
        return hr_touch_panel_get_position(input, input_len, output, output_len, cb);
#endif
#ifdef __HR_CFG_SHORTCUT_SUPPORT__
    case HR_PLATEX_SHORTCUT_SUPPORT:
        return hr_scut_support(input, input_len, output, output_len, cb);
    case HR_PLATEX_SHORTCUT_INSTALL:
        return hr_scut_install_app(input, input_len, output, output_len, cb);
    case HR_PLATEX_SHORTCUT_REMOVE:
        return hr_scut_remove_app(input, input_len, output, output_len, cb);
    case HR_PLATEX_SHORTCUT_PROGESS:
        return hr_scut_update_progress(input, input_len, output, output_len, cb);
#endif

    case HR_PLATEX_GET_HW_VER:
        return hr_setting_get_hw_version(input, input_len, output, output_len, cb);

#ifndef WIN32
    case HR_PLATEX_DOVE_SET_SMS_STRATEGY:
        return hr_dove_set_sms_strategy(input, input_len, output, output_len, cb); 
#endif
	default:
		resValue = HR_IGNORE;
		break;
	}
	
	return resValue;
}

LOCAL int32 hr_sms_get_status(int32 param)
{
	if(!MMISMS_IsOrderOk())
	{
		return HR_SMS_NOT_READY;
	}
	else
	{
		return HR_SMS_READY;
	}
}

LOCAL int32 hr_sms_get_num(int32 param)
{
	int32 num = 0;
	if(param == HR_SMS_INBOX)
	{
		num = MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_MT);
		return num + HR_PLAT_VALUE_BASE;
	}
	else if(param == HR_SMS_OUTBOX)
	{
		num = MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_SENDSUCC);
		return num + HR_PLAT_VALUE_BASE;
	}
	else if(param == HR_SMS_DRAFTS)
	{
		num = MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_NOSEND);
		return num + HR_PLAT_VALUE_BASE;
	}
	return num;
}

LOCAL int32 hr_sms_get_capacity(int32 param)
{
	uint16 maxNum = 0;
	uint16 usedNum = 0;
	uint16 smsUsedNum = 0;
	MN_DUAL_SYS_E activeSim = MMIHEROAPP_GetActiveSim();

	MMISMS_GetSMSUsedAndMax(activeSim, TRUE, &maxNum, &usedNum, &smsUsedNum);
	s_hr_sms_NVNum = maxNum;

	if(activeSim >= MN_DUAL_SYS_1 && activeSim < MN_DUAL_SYS_MAX)
	{
		MMISMS_GetSMSUsedAndMax(activeSim, FALSE, &maxNum, &usedNum, &smsUsedNum);
		s_hr_sms_SIMNum = maxNum;
	}

	MMIHEROAPP_StartGetSMSCapacityTimer();
	return HR_WAITING;
}

LOCAL int32 hr_sms_get_info(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	int32 resValue = HR_FAILED; 
	T_HR_GET_SMS_INFO_REQ *req = NULL;
	MMISMS_ORDER_ID_T order_id = NULL;
	MMI_TM_T tm ={0};

	if(input == NULL || input_len == 0)
	{
		return HR_FAILED;
	}

	req = (T_HR_GET_SMS_INFO_REQ *)input;
	
	if(req->type == HR_SMS_INBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_MT ,req->index);
	}
	else if(req->type == HR_SMS_OUTBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC,req->index);
	}
	
	if(order_id != NULL)
	{
		MMIHR_Trace(("hr_platEx GET_SMS_INFO order_id = %d",order_id));
    	MMIHR_Trace(("hr_platEx GET_SMS_INFO mo_mt_type = %d",order_id->flag.mo_mt_type));
    	MMIHR_Trace(("hr_platEx GET_SMS_INFO storage = %d",order_id->flag.storage));
    
		if(req->type == HR_SMS_INBOX)
		{
			if(MMISMS_MT_TO_BE_READ == order_id->flag.mo_mt_type)
			{
				s_hr_msgInfo.sms_status = HR_SMS_UNREAD;
			}
			else
			{
				s_hr_msgInfo.sms_status = HR_SMS_READED;
			}
		}
		else
		{
			if(MMISMS_MO_SEND_SUCC == order_id->flag.mo_mt_type)
			{
				s_hr_msgInfo.sms_status = HR_SMS_SENT;
			}
			else if(MMISMS_MO_DRAFT == order_id->flag.mo_mt_type)
			{
				s_hr_msgInfo.sms_status = HR_SMS_UNSENT ;
			}
		}

		if(MN_SMS_STORAGE_ME == order_id->flag.storage)
		{
			s_hr_msgInfo.sms_storage = HR_SMS_STORAGE_ME;
		}
		else
		{
			s_hr_msgInfo.sms_storage = HR_SMS_STORAGE_SIM;
		}
		
		MMIAPICOM_GenDispNumber( 
				MMIPB_GetNumberTypeFromUint8(order_id->number.bcd_number.npi_ton), 
				(uint8)MIN(order_id->number.bcd_number.number_len, HR_SMS_MAX_NUMBER), 
				order_id->number.bcd_number.number,  
				(uint8*)s_hr_msgInfo.num,
				(HR_SMS_MAX_NUMBER * 2 + 2)
    			);

		tm = MMIAPICOM_Second2Tm(order_id->time);
		s_hr_msgInfo.t_stamp[0] = tm.tm_year - 2000;
		s_hr_msgInfo.t_stamp[1] = tm.tm_mon;
		s_hr_msgInfo.t_stamp[2] = tm.tm_mday;
		s_hr_msgInfo.t_stamp[3] = tm.tm_hour;
		s_hr_msgInfo.t_stamp[4] = tm.tm_min;
		s_hr_msgInfo.t_stamp[5] = tm.tm_sec;
		*output = (uint8*)&s_hr_msgInfo;
		*output_len = sizeof(T_HR_MSG_INFO);
		
		resValue = HR_SUCCESS;
	}
	else
	{
		resValue = HR_FAILED;
	}

	return resValue;
}

LOCAL int32 hr_sms_get_content(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	T_HR_GET_SMS_INFO_REQ *req = NULL;
	MMISMS_ORDER_ID_T order_id = NULL;

	if(input == NULL || input_len == 0)
	{
		return HR_FAILED;
	}

	req = (T_HR_GET_SMS_INFO_REQ *)input;
	MMIHEROAPP_SetIsReadBySMS(TRUE);
	
	if(req->type == HR_SMS_INBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_MT ,req->index);
		MMISMS_SetCurOperationOrderId(MMISMS_BOX_MT, req->index);
		MMISMS_SetCurType(MMISMS_BOX_MT);
	}
	else if(req->type == HR_SMS_OUTBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC,req->index);
		MMISMS_SetCurOperationOrderId(MMISMS_BOX_SENDSUCC, req->index);
		MMISMS_SetCurType(MMISMS_BOX_SENDSUCC);
	}
	MMIHR_Trace(("hr_platEx:MMIMRAPP_GetSMSContent order_id = %d",order_id));
	
	if(order_id == NULL)
	{
		return HR_FAILED;
	}

	//设置当前操作状态
	MMISMS_SetOperStatus(MMISMS_READ_SMS);
    MMISMS_ReadyReadSms();
    //开始读取
    MMISMS_AppReadSms(order_id); /*lint !e718 !e18*/
	MMIHEROAPP_StartGetSMSContentTimer();
		
	return HR_WAITING;
}

LOCAL int32 hr_sms_delete(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
    BOOLEAN result = FALSE;
    MMISMS_BOX_TYPE_E box_type = 0;
	T_HR_GET_SMS_INFO_REQ *req = NULL;
    MMISMS_ORDER_ID_T order_id  = NULL;
    
	if(input == NULL || input_len == 0)
	{
		return HR_FAILED;
	}
	
	req = (T_HR_GET_SMS_INFO_REQ *)input;
    s_hr_sms_order_id = NULL;

	if(req->type == HR_SMS_INBOX)
	{
    	box_type = MMISMS_BOX_MT;
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_MT, req->index);
	}
	else if(req->type == HR_SMS_OUTBOX)
	{
    	box_type = MMISMS_BOX_SENDSUCC;
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC, req->index);
	}
	MMIHR_Trace(("hr_platEx:MMIMRAPP_DeleteSMS order_id=0x%08x", order_id));

	if(order_id == NULL)
	{
		return HR_FAILED;
	}

    if(order_id->next_long_sms_ptr != NULL)
    {
        s_hr_sms_order_id = order_id->next_long_sms_ptr;
    }

    MMIHEROAPP_SetIsReadBySMS(TRUE);
    result = MMISMS_DeleteReqToMN(box_type, order_id, MMISMS_DELETE_MSG);
	
	//MMIHEROAPP_StartDeleteSMSTimer();
	if(TRUE == result)
    {   
    	return HR_WAITING;
    }
    else
    {
        MMIHEROAPP_SetIsReadBySMS(FALSE);
        return HR_FAILED;
    }
}

#ifdef __HR_SMS_SET_STATE_SUPPORT__
LOCAL int32 hr_sms_set_state(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	MN_SMS_STATUS_E update_status = MN_SMS_FREE_SPACE;
	MN_SMS_STORAGE_E  storage = MN_SMS_NO_STORED;
	T_HR_SET_SMS_STATE_REQ *req = NULL;
	MN_DUAL_SYS_E dual_sys = 0;
	MN_SMS_RECORD_ID_T record_id = 0;
	MMISMS_ORDER_ID_T order_id = NULL;
	MMISMS_STATE_T new_order_id = {0};
	MMISMS_MO_MT_TYPE_E sms_new_status = 0;

	if(input == NULL || input_len == 0)
	{
		return HR_FAILED;
	}

	req = (T_HR_SET_SMS_STATE_REQ*)input;
	
	if(req->type == HR_SMS_INBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_MT ,req->index);
	}
	else if(req->type == HR_SMS_OUTBOX)
	{
		order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC,req->index);
	}
	MMIHR_Trace(("hr_platEx:hr_sms_set_state order_id = %d",order_id));

	if(order_id == NULL)
	{
		return HR_FAILED;
	}

	storage = (MN_SMS_STORAGE_E)order_id->flag.storage;
	dual_sys = (MN_DUAL_SYS_E)order_id->flag.dual_sys;
	record_id = (MN_SMS_RECORD_ID_T)order_id->record_id;

	switch(req->state)
	{
		case HR_SMS_UNREAD:
			update_status = MN_SMS_MT_TO_BE_READ;
			sms_new_status = MMISMS_MT_TO_BE_READ;
			break;

		case HR_SMS_READED:
			update_status = MN_SMS_MT_READED;
			sms_new_status = MMISMS_MT_HAVE_READ;
			break;

		case HR_SMS_UNSENT:
			update_status = MN_SMS_MO_TO_BE_SEND;
			sms_new_status = MMISMS_MO_DRAFT;
			break;

		case HR_SMS_SENT:
			update_status = MN_SMS_MO_SR_RECEIVED_AND_STORED;
			sms_new_status = MMISMS_MO_SEND_SUCC;
			break;

		default:
			break;
	}

	s_hr_sms_err_code = MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, update_status);

	//重设修改后短信的状态

    MMISMS_GetSmsInfoFromOrder(order_id, &new_order_id);
	new_order_id.flag.mo_mt_type = sms_new_status;
	MMISMS_UpdateSmsInfoInOrder(order_id, &new_order_id);
	MAIN_SetMsgState(TRUE); /*lint !e718 !e18*/
	
	MMIHEROAPP_StartSetSMSStateTimer();
	return HR_WAITING;
}
#endif
LOCAL int32 hr_sms_save_sms(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	HR_SMS_SAVE_ENTRY_REQ_T *pSmsInput = NULL;
	MN_SMS_STATUS_E sms_status = MN_SMS_FREE_SPACE;
	MN_SMS_MT_SMS_T sms_ptr = {0};
	MMI_PARTY_NUMBER_T party_num = {0};
	APP_SMS_USER_DATA_T user_data_ptr = {0};
	SCI_DATE_T date = {0};
	SCI_TIME_T time = {0};
	BOOLEAN result = FALSE;
	
	if(input == NULL || input_len == 0)
	{
		return HR_FAILED;
	}

	pSmsInput = (HR_SMS_SAVE_ENTRY_REQ_T *)input;

	SCI_MEMSET(&s_hr_save_order_id, 0x00, sizeof(MMISMS_STATE_T));
	SCI_MEMSET(&sms_ptr, 0x00, sizeof(MN_SMS_MT_SMS_T));

#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	if(pSmsInput->flag & HR_SMSSAVE_DISP_SIM_OPT)
	{
		s_hr_save_order_id.flag.dual_sys = MN_DUAL_SYS_1;
	}
	else if(pSmsInput->flag & HR_SMSSAVE_TO_SIM1)
	{
		s_hr_save_order_id.flag.dual_sys = MN_DUAL_SYS_1;
	}
	else if(pSmsInput->flag & HR_SMSSAVE_TO_SIM2)
	{
		s_hr_save_order_id.flag.dual_sys = MN_DUAL_SYS_2;
	}
#else
	if(pSmsInput->flag & HR_SMSSAVE_DISP_SIM_OPT
		|| pSmsInput->flag & HR_SMSSAVE_TO_SIM1)
	{
		s_hr_save_order_id.flag.dual_sys = MN_DUAL_SYS_1;
	}
#endif

	if(strlen(pSmsInput->number) > 0)
	{
		MMIAPICOM_GenPartyNumber((uint8*)pSmsInput->number, (int16)strlen(pSmsInput->number), &party_num);
		s_hr_save_order_id.number.bcd_number.number_len = (uint8)party_num.num_len;
		s_hr_save_order_id.number.bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, 0);
		MMI_MEMCPY(s_hr_save_order_id.number.bcd_number.number, MMIPB_BCD_NUMBER_MAX_LEN, party_num.bcd_num, party_num.num_len, party_num.num_len);
	}

	if(pSmsInput->status == 0)
	{
		s_hr_save_order_id.flag.mo_mt_type = MMISMS_MT_TO_BE_READ;
		sms_status = MN_SMS_MT_TO_BE_READ;
	}
	else
	{
		s_hr_save_order_id.flag.mo_mt_type = MMISMS_MT_HAVE_READ;
		sms_status = MN_SMS_MT_READED;
	}

	s_hr_save_order_id.flag.msg_type   = MMISMS_TYPE_SMS;
	s_hr_save_order_id.flag.storage    = MN_SMS_STORAGE_ME;

	//存储的短信时间
	TM_GetSysDate(&date);
	TM_GetSysTime(&time);
	sms_ptr.time_stamp_t.year = date.year-MMISMS_MT_OFFSERT_YEAR; //modify by zack@20120618
	sms_ptr.time_stamp_t.month= date.mon;
	sms_ptr.time_stamp_t.day  = date.mday;
	sms_ptr.time_stamp_t.hour = time.hour;
	sms_ptr.time_stamp_t.minute = time.min;
	sms_ptr.time_stamp_t.second = time.sec;
  
    s_hr_save_order_id.time= MMIAPICOM_Tm2Second(
        					time.sec,
        					time.min,
        					time.hour,
        					date.mday,
        					date.mon,
        					date.year
        					);

	//存储的短信内容
	sms_ptr.dcs.alphabet_type = MN_SMS_UCS2_ALPHABET;
	sms_ptr.user_data_t.length = MIN(MMIAPICOM_Wstrlen(pSmsInput->content)*2, MN_SMS_USER_DATA_LENGTH);
	sms_ptr.user_data_t.user_data_arr[0] = sms_ptr.user_data_t.length;
	SCI_MEMCPY(sms_ptr.user_data_t.user_data_arr+1, pSmsInput->content, sms_ptr.user_data_t.length);
	sms_ptr.user_data_t.length++;

	MMIHR_Trace(("hr_sms_save_sms length = %d", sms_ptr.user_data_t.length));
	MMIAPICOM_WstrTraceOut(sms_ptr.user_data_t.user_data_arr, sms_ptr.user_data_t.length/2);

	//存储的短信号码
	sms_ptr.origin_addr_t.num_len 	  = party_num.num_len;
	sms_ptr.origin_addr_t.number_type = party_num.number_type;
	sms_ptr.origin_addr_t.number_plan = MN_NUM_PLAN_ISDN_TELE;
	SCI_MEMCPY(sms_ptr.origin_addr_t.party_num, party_num.bcd_num, party_num.num_len);

	//unicode 大小转换
	hr_str_convert_endian_with_size((char *)pSmsInput->content, HR_SMS_MAX_CONTENT);

	//短信主题显示内容
	s_hr_save_order_id.flag.is_ucs2 = TRUE;
	s_hr_save_order_id.display_content.content_len = MIN(MMIAPICOM_Wstrlen(pSmsInput->content)*2, MMIMESSAGE_DIAPLAY_LEN);
	SCI_MEMCPY(s_hr_save_order_id.display_content.content, pSmsInput->content, s_hr_save_order_id.display_content.content_len);
	
	//设置当前操作状态
//	MMISMS_SetOperStatus(MMISMS_MRAPP_SAVE_SMS);
	
	//将短信内容写入
	MNSMS_WriteMtSmsEx(s_hr_save_order_id.flag.dual_sys, MN_SMS_STORAGE_ME, sms_status, &sms_ptr);

	return HR_SUCCESS;
}

LOCAL int32 hr_sms_replay_by_UI(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
	uint8 num_len = 0;
	uint8 number[MMISMS_NUMBER_MAX_LEN] = {0};
	T_HR_SMS_REPLY_ENTRY_REQ *req = NULL;
	
	if(input == NULL || input_len == 0)
	{
	    return HR_FAILED;
	}

	req = (T_HR_SMS_REPLY_ENTRY_REQ *)input;
	num_len = MIN(strlen(req->number), MMISMS_NUMBER_MAX_LEN);
	SCI_MEMSET(number, 0x00, sizeof(number));
	SCI_MEMCPY(number, req->number, num_len);
	MMISMS_WriteNewMessage(req->simid, NULL, FALSE, number, num_len);
	
	return HR_SUCCESS;
}

LOCAL int32 hr_sms_send_sms_from_ui(uint8* input, int32 input_len, uint8** output, int32* output_len)
{
    wchar *text = NULL;
	uint8 num_len = 0;
	uint8 number[MMISMS_NUMBER_MAX_LEN] = {0};
    MMI_STRING_T content = {0};
	HR_SMS_SEND_REQ_T* req = (HR_SMS_SEND_REQ_T *)input;
    
	if(input == NULL || input_len != sizeof(HR_SMS_SEND_REQ_T))
	{
	    return HR_FAILED;
	}

	if (req->content != NULL)
	{
		content.wstr_len = hr_str_wstrlen(req->content);
	}

	if (content.wstr_len > 0)
	{
		text = SCI_ALLOCA(content.wstr_len*2 + 2);
		if (text == NULL)
		{
			return HR_FAILED;
		}

		memset(text, 0x00, content.wstr_len*2+2);
		memcpy(text, req->content, content.wstr_len*2);
		hr_str_convert_endian_with_size((char *)text, content.wstr_len*2);
        content.wstr_ptr = text;

        if(req->addr != NULL)
        {
        	num_len = MIN(strlen(req->addr), MMISMS_NUMBER_MAX_LEN);
        	SCI_MEMSET(number, 0x00, sizeof(number));
        	SCI_MEMCPY(number, req->addr, num_len);
            MMISMS_WriteNewMessage(MMIHEROAPP_GetActiveSim(), &content, FALSE, number, num_len);
        }
        else
        {
        	MMISMS_WriteNewMessage(MMIHEROAPP_GetActiveSim(), &content, FALSE, NULL, 0);
        }

        if(text != NULL)
        {
            SCI_FREE(text);
            text = NULL;
        }
	}
    else
    {
        if(req->addr != NULL)
        {
        	num_len = MIN(strlen(req->addr), MMISMS_NUMBER_MAX_LEN);
        	SCI_MEMSET(number, 0x00, sizeof(number));
        	SCI_MEMCPY(number, req->addr, num_len);
            MMISMS_WriteNewMessage(MMIHEROAPP_GetActiveSim(), NULL, FALSE, number, num_len);
        }
        else
        {
            MMISMS_WriteNewMessage(MMIHEROAPP_GetActiveSim(), NULL, FALSE, NULL, 0);
        }
    }

	return HR_SUCCESS;
}


LOCAL int32 hr_image_type_translate(GUIANIM_TYPE_E type)
{
	int32 image_type;
	if(type == GUIANIM_TYPE_IMAGE_BMP)
	{
		image_type = HR_IMAGE_IMG_TYPE_BMP;
	}
	else if(type == GUIANIM_TYPE_IMAGE_JPG)
	{
		image_type = HR_IMAGE_IMG_TYPE_JPG;
	}
	else if(type == GUIANIM_TYPE_IMAGE_PNG)
	{
		image_type = HR_IMAGE_IMG_TYPE_PNG;
	}
	else if(type == GUIANIM_TYPE_IMAGE_GIF)
	{
		image_type = HR_IMAGE_IMG_TYPE_GIF;
	}
	else
	{
		image_type = HR_IMAGE_IMG_TYPE_NONE;
	}

	return image_type;
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_imageCmd(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 resValue = HR_FAILED;
	MMIHR_Trace(("MMIMRAPP_ImageCmd() :%d", code));
	
	switch(code)
	{
	case HR_IMG_GET_INFO://取得一张图片的信息
		return hr_image_get_info(input, input_len, output, output_len, cb);
               
	case HR_IMG_DEC_JPG://解码一张图片，将解完的数据放到dest所指的buf中，并通知vm解码结果
		return hr_image_decode(input, input_len, output, output_len, cb);
        
	case HR_IMG_GET_STAT://取得图像解码状态(异步解码)
		break;

	case HR_IMG_DEC_GIF://GIF DECODE
		break;
        
	case HR_IMG_FREE_GIF://FREE GIF		
		break;

	case HR_IMG_DRAW_JPG://DRAW PIC
		return hr_image_draw(input, input_len, output, output_len, cb);

	default:
		resValue = HR_IGNORE;//命令不支持
		break;
	}

	return resValue;
}

LOCAL int32 hr_image_get_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	HRAPP_IMAGE_ORIGIN_T* origin = (HRAPP_IMAGE_ORIGIN_T*)input;
	int32 resValue = HR_FAILED;
	HERO_APP_IMG_TYPE imgType;
	uint8* data = NULL;
	int32 dataLen;
			
	MMIHR_Trace(("hr_image_get_info origin:%x", origin));
	if(origin != NULL)
	{
		MMIHR_Trace(("hr_image_get_info src_type: %d, src: %d, len: %d", origin->src_type, origin->src, origin->len));
		if(origin->src_type == SRC_STREAM)
		{
			data = (uint8*)origin->src;
			dataLen = origin->len;
				
			if(data != NULL && dataLen > 0)
			{
				HRAPP_IMAGE_INFO_T *rspInfo = (HRAPP_IMAGE_INFO_T*)hr_tempTransBuf;
				IMG_DEC_SRC_T    dec_src = {0};
    			IMG_DEC_INFO_T  dec_info = {0};
				
				dec_src.src_ptr 	  = (unsigned char*)data;
				dec_src.src_file_size = (uint32)dataLen;
				
				if(IMG_DEC_RET_SUCCESS == IMG_DEC_GetInfo(&dec_src, &dec_info))
				{
					if(IMG_DEC_TYPE_JPEG == dec_info.img_type)
					{
						rspInfo->width    = dec_info.img_detail_info.jpeg_info.img_width;
						rspInfo->height   = dec_info.img_detail_info.jpeg_info.img_heigth;
						rspInfo->img_type = HR_IMAGE_IMG_TYPE_JPG;
					}
					else if(IMG_DEC_TYPE_GIF == dec_info.img_type)
					{
						rspInfo->width    = dec_info.img_detail_info.gif_info.img_width;
						rspInfo->height   = dec_info.img_detail_info.gif_info.img_height;
						rspInfo->img_type = HR_IMAGE_IMG_TYPE_GIF;
					}
					else if(IMG_DEC_TYPE_PNG == dec_info.img_type)
					{
						rspInfo->width    = dec_info.img_detail_info.png_info.img_width;
						rspInfo->height   = dec_info.img_detail_info.png_info.img_height;
						rspInfo->img_type = HR_IMAGE_IMG_TYPE_PNG;
					}
					else
					{
						resValue = HR_FAILED;
					}

					MMIHR_Trace(("hr_image_get_info h : %d, w: %d, type: %d", rspInfo->height, rspInfo->width, rspInfo->img_type));
							
					*output = (uint8*)rspInfo;
					*output_len = sizeof(HRAPP_IMAGE_INFO_T);
					resValue = HR_SUCCESS;
				}
				else
				{
					resValue = HR_FAILED;//不支持
				}
			}
		}

		else if(origin->src_type == SRC_NAME)
		{
			HRAPP_IMAGE_INFO_T *rspInfo = (HRAPP_IMAGE_INFO_T*)hr_tempTransBuf;
			char* fileName = NULL;
			uint32 fileSize = 0;
			int32 fullNameLen = 0;
			GUIANIM_IMG_INFO_T  img_info = {0};
			fileName = (char*)origin->src;

			if(fileName != NULL)
			{
				memset(hr_full_path, 0, sizeof(hr_full_path));
				fileSize = hr_getLen(fileName);
				fullNameLen = hr_fs_get_filename((char*)hr_full_path, fileName);
						
				if(fullNameLen > 0)
				{
					GUIANIM_GetInfo(NULL, hr_full_path, NULL, &img_info);
					rspInfo->width    = img_info.img_width;
					rspInfo->height   = img_info.img_height;
					rspInfo->img_type = hr_image_type_translate(img_info.type);
					
					MMIHR_Trace(("hr_image_get_info h : %d, w: %d, type: %d", rspInfo->height, rspInfo->width, rspInfo->img_type));
							
					*output = (uint8*)rspInfo;
					*output_len = sizeof(HRAPP_IMAGE_INFO_T);
					resValue = HR_SUCCESS;
				}
			}
			else
			{
				resValue = HR_FAILED;
			}
		}
	}

	return resValue;
}

/*****************************************************************************/
LOCAL int32 hr_image_decode(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	HRAPP_IMAGE_DECODE_T *image = (HRAPP_IMAGE_DECODE_T*)input;
	int32 result = HR_FAILED;
	GUIIMG_SRC_T		img_src = {0};     
	GUIIMG_DISPLAY_T	img_display = {0};  
	GUIIMG_DEC_OUT_T	img_output = {0};  
	
	MMIHR_Trace(("mrapp. jpg decode before!type: %d",image->src_type));
	MMIHR_Trace(("mrapp.decode in!w: %d, h: %d",image->width, image->height));
	
	if(image == NULL || image->src == NULL || image->dest == NULL) /*lint !e831*/
	{
		return HR_FAILED;
	}
	
	if(image != NULL) /*lint !e774 !e831*/
	{
		if(image->src_type == SRC_STREAM)
		{
			uint8* data = NULL;
			int32 dataLen = 0;
			data = (uint8*)image->src;
			dataLen = image->len;
			
			if(data != NULL && dataLen > 0)
			{
				img_src.is_file = FALSE;
				img_src.src_buf_ptr = data;
				img_src.src_data_size = dataLen;
			}
		}
		
		else if(image->src_type == SRC_NAME)
		{
			char* fileName = NULL;
			int32 fullNameLen = 0;
			fileName = (char*)image->src;

			if(fileName != NULL)
			{
				memset(hr_full_path, 0, sizeof(hr_full_path));
				fullNameLen = hr_fs_get_filename((char*)hr_full_path, fileName);
				
				img_src.is_file = TRUE;
				img_src.full_path_ptr = (wchar*)hr_full_path;
			}
		}
	}
		
	img_display.dest_width = image->width;
	img_display.dest_height = image->height;
	img_output.decode_data_ptr = (uint8*)image->dest;
	img_output.decode_data_size = image->width*image->height*2;

	if(GUIIMG_Decode(&img_src, &img_display, &img_output))
	{
		MMIHR_Trace(("mrapp.decode by name ok!w: %d, h: %d,w: %d, h: %d", img_output.actual_width, img_output.actual_height, 
																		image->width, image->height));
		image->width = img_output.actual_width;
		image->height = img_output.actual_height;
		result = HR_SUCCESS;
	}
	else
	{
		result = HR_FAILED;
	}

	MMIHR_Trace(("mrapp.decode by name ok!result: %d", result));
	return result;
}

/*****************************************************************************/
LOCAL int32 hr_image_draw(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	T_HRAW_DIRECT_REQ *image = (T_HRAW_DIRECT_REQ*)input;
	int32 result = HR_FAILED;
	int32 decodeBufSize = 0;
	GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T  rect = {0};
	GUI_POINT_T point = {0};
	GUIIMG_BITMAP_T bitmap_ptr = {0};
	GUIIMG_SRC_T		img_src = {0};     
	GUIIMG_DISPLAY_T	img_display = {0};  
	GUIIMG_DEC_OUT_T	img_output = {0};

	if(image == NULL || image->src == NULL)
	{
		return HR_FAILED;
	}

	decodeBufSize = image->h*image->w*4;
	bitmap_ptr.bit_ptr = (void*)SCI_ALLOCA(decodeBufSize);	

	if(bitmap_ptr.bit_ptr == NULL)
	{
		return HR_FAILED;
	}

	if(image->src_type == SRC_STREAM)
	{
		uint8* data = NULL;
		int32 dataLen = 0;
		data = (uint8*)image->src;
		dataLen = image->src_len;
			
		if(data != NULL && dataLen > 0)
		{
			img_src.is_file = FALSE;
			img_src.src_buf_ptr = data;
			img_src.src_data_size = dataLen;
		}
	}
	else if(image->src_type == SRC_NAME)
	{
		char* fileName = NULL;
		int32 fullNameLen = 0;
		fileName = (char*)image->src;
			
		if(fileName != NULL)
		{
			memset((void*)hr_full_path, 0, sizeof(hr_full_path));
			fullNameLen = hr_fs_get_filename((char*)hr_full_path, fileName);
				
			img_src.is_file = TRUE;
			img_src.full_path_ptr = (wchar*)hr_full_path;
		}
	}

	memset(bitmap_ptr.bit_ptr, 0, decodeBufSize);
	
	img_display.dest_width = image->w;
	img_display.dest_height = image->h;
	img_output.decode_data_ptr = bitmap_ptr.bit_ptr;
	img_output.decode_data_size = decodeBufSize;

	point.x = image->ox;
	point.y = image->oy;
    rect.left = image->ox;
    rect.top  = image->oy;
    rect.right = image->ox + image->w - 1;
    rect.bottom= image->oy + image->h - 1;

	MMIHR_Trace(("mrapp.decode in!w: %d, h: %d", image->w, image->h));
	if(GUIIMG_Decode(&img_src, &img_display, &img_output))
	{
		MMIHR_Trace(("mrapp.decode by name ok!w: %d, h: %d,w: %d, h: %d", img_output.actual_width, img_output.actual_height, 
																		image->w, image->h));
		bitmap_ptr.img_height = img_output.actual_height;
		bitmap_ptr.img_width = img_output.actual_width;
		if(GUIIMG_DisplayBmp(FALSE, &rect, &point, (void*)&bitmap_ptr, &dev_info))
		{
			MMIHR_Trace(("mrapp.Display ok!w: %d, h: %d", bitmap_ptr.img_width, bitmap_ptr.img_height));
			result = HR_SUCCESS;
		}
		else
		{
			result = HR_FAILED;
		}
	}
	else
	{
		MMIHR_Trace(("mrapp.decode failed!w: %d, h: %d, ptr: 0x%x, size: %d", img_output.actual_width, img_output.actual_height, 
			          img_output.decode_data_ptr, img_output.decode_data_size));
		result = HR_FAILED;
	}
	SCI_FREE(bitmap_ptr.bit_ptr);

	return result;
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_gifFree(HRAPP_GIF_HEADER ** header)
{
	int32 resValue = HR_FAILED;
	HRAPP_GIF_HEADER *hdr;
	if(header != NULL)
	{
		hdr = *header;
		if(hdr != NULL)
		{
			HRAPP_GIF_FRAME_INFO_T *framePtr, *temp;
			framePtr = hdr->first;
			while(framePtr != NULL)
			{
				temp = framePtr;
				framePtr = framePtr->next;
				MMIHR_Trace(("temp->pdata: 0x%x", temp->pdata));
				if(temp->pdata != NULL)
				{
					SCI_FREE(temp->pdata);
				}
				SCI_FREE(temp);
			}
			MMIHR_Trace(("hdr: 0x%x", hdr));
			SCI_FREE(hdr);
			*header = NULL;
			resValue = HR_SUCCESS;
		}
	}
	
	return resValue;
}

/*****************************************************************************/
LOCAL uint32 MMIHEROAPP_get_jpeg_width_height(uint8 * pbuf, uint32 size)
{	
	uint32 i = 0, width = 0, height = 0,len = 0;;
	MMIHR_Trace(("MMIHEROAPP_get_jpeg_width_height() :%d", size));
	
	for(i = 2; i < size;)
	{
		if ((pbuf[i]==0xff) &&(pbuf[i+1]==0xC0))
		{
			width = (((uint16)pbuf[i+7]) << 8)|(((uint16)pbuf[i+8]));
			height = (((uint16)pbuf[i+5]) << 8)|(((uint16)pbuf[i+6]));
			MMIHR_Trace(("jpeg_width: %d", width));
			MMIHR_Trace(("jpeg_height: %d", height));
			return (height << 16) |width;
		}
		else
		{
			i+=2;
			len = ((pbuf[i])<<8)|(pbuf[i+1]);
			i += len;
		}
	}
	return 0;
}

/*****************************************************************************/
LOCAL HERO_APP_IMG_TYPE MIHEROAPP_get_image_type_from_buf(unsigned char *pbuf, int32 len)
{
	if(pbuf == NULL)
		return HERO_IMAGE_TYPE_INVALID;
	   
	if((len>=3)&&( pbuf[0] == 0xff && pbuf[1] == 0xd8 && pbuf[2] == 0xff))
		return HERO_IMAGE_TYPE_JPG_FILE;
	else if((len>=3)&&( pbuf[0] == 'G' && pbuf[1] == 'I' && pbuf[2] == 'F'))
		return HERO_IMAGE_TYPE_GIF_FILE;
	else if((len>=8)&&(memcmp(pbuf,"\x89PNG\x0d\x0a\x1a\x0a",8) == 0))
		return HERO_IMAGE_TYPE_PNG_FILE;
	else if((len>=2)&&( pbuf[0] == 'B' && pbuf[1] == 'M'))
		return HERO_IMAGE_TYPE_BMP_FILE;
	else if((len>=2)&&( pbuf[0] == 0 && pbuf[1] == 0))
		return HERO_IMAGE_TYPE_WBMP_FILE;
	else
		return HERO_IMAGE_TYPE_INVALID;
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_listCreate(const char * title, int16 num, uint8 type)
{
	int32 resValue = HR_FAILED;
	MMI_HERO_APP_ADD_PARAMETERS_T *addData = NULL;
	HERO_APP_MENU_DATA *menuPtr;	
	char *temp = NULL;
	int32 lenth  = 0;
	MMIHR_Trace(("mrapp. hr_menuCreate() num: %d", num));

	if(title != NULL)
	{
		lenth = hr_str_wstrlen(title) + 2;
		temp = (char*)SCI_ALLOCA(lenth);
		SCI_MEMCPY(temp, title, lenth);
		hr_str_convert_endian((char*)temp);
	}
	
	addData = (MMI_HERO_APP_ADD_PARAMETERS_T *)SCI_ALLOC(sizeof(MMI_HERO_APP_ADD_PARAMETERS_T));
	if(addData != NULL)
	{		
		addData->dynWinType = HERO_APP_WINTYPE_MENU;
		menuPtr = &addData->u.menu;
		menuPtr->type = type;
		menuPtr->title.wstr_len = (uint16)MMIHEROAPP_getUcstrLen((uint8 *)temp)/2;
		menuPtr->maxItemNum = MIN(num, 40);
		menuPtr->curAddItemIdx = -1;
		menuPtr->curFocusItemIdx = 0;
		menuPtr->preFocusItemIdx = 0;
		menuPtr->listElem = NULL;
		MMIHR_Trace(("menu.title len: %d", menuPtr->title.wstr_len));
		if(menuPtr->title.wstr_len != 0)
		{
			menuPtr->title.wstr_ptr = (wchar*)SCI_ALLOC(menuPtr->title.wstr_len*2+2);
			if(menuPtr->title.wstr_ptr != NULL)
			{
				SCI_MEMCPY(menuPtr->title.wstr_ptr, temp, menuPtr->title.wstr_len*2+2);
			}
			else
			{
				menuPtr->title.wstr_len = 0;
			}
		}
		else
		{
			menuPtr->title.wstr_ptr = NULL;
		}
		
		resValue = MMIHEROAPP_createADynWin(addData, NULL);
		if(resValue < 0)
		{
			if(resValue == -2)
			{
				if(menuPtr->title.wstr_ptr != NULL)
				{
					SCI_FREE(menuPtr->title.wstr_ptr);
				}
				SCI_FREE(addData);
			}
			resValue = HR_FAILED;
		}	
	}

	if(temp != NULL)
	{
		SCI_FREE(temp);
	}
	MMIHR_Trace(("mrapp. hr_menuCreate() resValue: %d", resValue));
	return resValue;
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_menuSetItem(int32 menu, const char *text, int32 index, int32 isSelected)
{
	int32 resValue = HR_FAILED;
	GUIMENU_DYNA_ITEM_T node_item = {0};
	MMI_STRING_T item_text;
	HERO_APP_MENU_DATA *menuPtr;
	HERO_APP_LIST_ITEM_T *addElem, **continueSite = NULL;
	MMI_HERO_APP_ADD_PARAMETERS_T* addData;
	uint16 len;
	int32 replaceIndex = -1;	
	char *temp = NULL;
	int32 lenth = 0;
	MMIHR_Trace(("mrapp. hr_menuSetItem() menu: %d, index: %d", menu, index));
	
	addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(menu);

	if(text != NULL)
	{
		lenth = hr_str_wstrlen(text) + 2;
		temp = (char*)SCI_ALLOCA(lenth);
		SCI_MEMCPY(temp, text, lenth);
		hr_str_convert_endian((char*)temp);
	}

	if(addData != NULL)
	{
		len = MMIHEROAPP_getUcstrLen((uint8 *)temp);
		len = (len <= (HERO_GUILIST_STRING_MAX_NUM_AD - 2)) ? len : (HERO_GUILIST_STRING_MAX_NUM_AD - 2);
		if(len != 0)
		{
			menuPtr = &addData->u.menu;			
			//txt info
			item_text.wstr_len = len/2;
			item_text.wstr_ptr = (wchar*)temp;
			
			//local info
			continueSite = &menuPtr->listElem;
			while(*continueSite != NULL)
			{
				if((*continueSite)->key == index)
				{
					replaceIndex = (*continueSite)->index;
					continueSite = &(*continueSite)->nextItem;
					break;
				}
				continueSite = &(*continueSite)->nextItem;
			}
			
			node_item.item_text_ptr = &item_text;
			if(menuPtr->type == HR_LIST_NORMAL_ICON)
            {
				if(replaceIndex == -1)
                {
					node_item.select_icon_id = MMIHEROAPP_getItemIcon(HERO_APP_ICON_OFF, menuPtr->curAddItemIdx + 1);
				}
                else
                {
					node_item.select_icon_id = MMIHEROAPP_getItemIcon(HERO_APP_ICON_OFF, replaceIndex);
				}
			}
            else if(menuPtr->type == HR_LIST_SINGLE_SEL)
            {
				if(isSelected == 1)
                {
                #if defined(SPDE_UI_STYLE_WHITE_BG_BLACK_FONT)		
					node_item.select_icon_id = IMAGE_RADIOBUTTON_ON;
		  #else		
					node_item.select_icon_id = IMAGE_RADIO_SELECTED_ICON;
		  #endif
				}
                else
                {
                #if defined(SPDE_UI_STYLE_WHITE_BG_BLACK_FONT)
					node_item.select_icon_id = IMAGE_RADIOBUTTON_OFF;
		  #else	
					node_item.select_icon_id = IMAGE_RADIO_UNSELECTED_ICON;
		  #endif
				}
			}
            else if(menuPtr->type == HR_LIST_MUTI_SEL)
            {
				if(isSelected == 1)
                {
					node_item.select_icon_id = IMAGE_CHECK_SELECTED_ICON;
				}
                else
                {
					node_item.select_icon_id = IMAGE_CHECK_UNSELECTED_ICON;
				}
			}
			
			if(replaceIndex == -1)
            {//add a new item
				MMIHR_Trace(("mrapp. curAddItemIdx: %d, index: %d", menuPtr->curAddItemIdx, index));
				if(menuPtr->curAddItemIdx+1 < menuPtr->maxItemNum)
                {
					GUIMENU_InsertNode(menuPtr->curAddItemIdx+1, menuPtr->curAddItemIdx+2, 0, &node_item, addData->ctrlId);
					menuPtr->curAddItemIdx ++;
					addElem = (HERO_APP_LIST_ITEM_T*)SCI_ALLOC(sizeof(HERO_APP_LIST_ITEM_T));
					SCI_MEMSET(addElem, 0, sizeof(HERO_APP_LIST_ITEM_T));
					addElem->index = menuPtr->curAddItemIdx;
					addElem->key = index;
					addElem->nextItem = NULL;
					*continueSite = addElem;
					if (isSelected == 1)
					{
						GUIMENU_SetFirstAndSelectedItem(0, menuPtr->curAddItemIdx, addData->ctrlId);
					}
				}
			}
            else
            {	//replace a item
				MMIHR_Trace(("mrapp. replaceIndex: %d, index: %d", replaceIndex, index));
			#ifndef WIN32
				GUIMENU_ReplaceNodeEx(replaceIndex, replaceIndex+1, 0, &node_item, addData->ctrlId);
			#endif
			}
			
			resValue = HR_SUCCESS;
		}
	}

	if(temp != NULL)
	{
		SCI_FREE(temp);
	}
	MMIHR_Trace(("mrapp. hr_menuSetItem() resValue: %d", resValue));
	return resValue;
}

/*****************************************************************************/
LOCAL void HERORingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
	MMIHR_TraceM(("mrapp. HERORingCallBack() result: %d", result));
	MMIHEROAPP_StopPlaySoundProtect();//2008-1-2 add
	MMIHEROAPP_resumeMp3();
    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MP3_CONTINUE,PNULL,0);
}

/*****************************************************************************/

int32 hr_save_ho(void *p,uint32 l)
{
    return 0;
}

int32 MMIHEROAPP_savePack(uint8 *p,uint32 l)
{
	int32 ret = HR_IGNORE;
	MMIFILE_HANDLE f;
	const uint8 *devName;
	uint16 devNameLen;		
	uint32 space_low = 0, space_high = 0;
	uint16 hr_full_path_len = 0, ucs2_name_len = 0, dirLen = 0;
	uint16 dir[MMIFILE_DIR_NAME_MAX_LEN] = {0};
	MMIHR_Trace(("MMIHEROAPP_savePack() len: %d", l));
	
	if(p != NULL && l != 0)
	{
		if((*((char*)p + 69) !=0x7e) || (*((char*)p + 70) != 0x7e) || (*((char*)p + 71) != 0x7e))
		{
			ret = hr_save_ho((void*)p, l);
		}
#ifdef HERO_APP_SIMUGAME_OPEN
		else if((*(char*)p == 'M') && (*((char*)p + 1) == 'R') && (*((char*)p + 2) == 'P') && (*((char*)p + 3) == 'G'))//.nes pack
		{
			MMIHR_Trace(("##NES ZPK !!!!"));
			ret = HR_FAILED;
			switch(g_hr_dualsys_mng.devSel)
			{
			default:
			case 'b':
			case 'B':
				devName = (const uint8 *)HERO_MMIFILE_DEVICE_UDISK;
				devNameLen = HERO_MMIFILE_DEVICE_UDISK_LEN;
				break;
			case 'c':
			case 'C':
#ifdef DUAL_TCARD_SUPPORT
                if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
                {
#endif
        			devName = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD;
        			devNameLen = HERO_MMIFILE_DEVICE_SDCARD_LEN;
#ifdef DUAL_TCARD_SUPPORT
                }
                else if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
                {
             	    devName = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD2;
    				devNameLen = HERO_MMIFILE_DEVICE_SDCARD2_LEN;
                }
#endif
				break;
#ifdef DOUBLE_TFCARD				
			case 'd':
			case 'D':
				devName = (const uint8 *)HERO_MMIFILE_DEVICE_SDCARD_2;
				devNameLen = HERO_MMIFILE_DEVICE_SDCARD_LEN_2;
				break;
#endif
			}
			
			if(MMIAPIFMM_GetDeviceStatus((uint8*)devName, devNameLen))
			{
				MMIFILE_GetDeviceFreeSpace((uint8 *)devName, devNameLen, &space_high, &space_low);
			}
			if(space_low == 0 || space_low == 0xFFFFFFFF)
			{
				return 2;
			}
			
			SCI_MEMSET(hr_tempTransBuf, 0, sizeof(hr_tempTransBuf));
			SCI_MEMCPY(hr_tempTransBuf , (char*)p + 28, 24);
			SCI_MEMSET(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));
			SCI_MEMSET(hr_full_path, 0, sizeof(hr_full_path));
			ucs2_name_len = MMIHEROAPP_GB2UCS(hr_ucs2_name_arr, (const uint8 *)hr_tempTransBuf, strlen((char *)(hr_tempTransBuf)));
			dirLen = MMIHEROAPP_GB2UCS(dir, (const uint8 *)HERO_ENGINE_NES_DIR_NAME, strlen(HERO_ENGINE_NES_DIR_NAME));
#ifdef HERO_PORT_BASE_NILE
			devNameLen = devNameLen*2;
#endif
			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)dir, dirLen,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				MMIFILE_DeleteFile((const uint16*)hr_full_path, NULL);
				f = MMIFILE_CreateFile((const uint16 *)hr_full_path, SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, NULL, NULL);
				if(f != FFS_INVALID_HANDLE)
				{
					if(hr_write(f, (void *)p, l) == (int32)l)
					{
						ret = HR_SUCCESS;
					}
					hr_close(f);
				}
			}
		}
#endif	
	}
	
	return ret;
}

#ifdef HERO_USE_UDISK_FIRST
extern char  g_hero_appparam[64];
int32 hr_info_udisk(const char* filename)
{
	SFS_HANDLE		sfs_handle = 0;
	SFS_FIND_DATA_T find_data = {0};
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	const uint8 *devName;
	uint16 devNameLen;
	
	if(filename != NULL)
	{
		MMIHR_Trace(("mrapp. hr_info() file: %s", filename));

		//devName = MMIHEROAPP_getCurDevName(&devNameLen);
		devName=HERO_MMIFILE_DEVICE_UDISK;
		devNameLen=HERO_HERO_MMIFILE_DEVICE_UDISK_LEN_AD;
		if(devName != NULL)
		{
			memset(hr_full_path, 0, sizeof(hr_full_path));
			memset(hr_ucs2_name_arr, 0, sizeof(hr_ucs2_name_arr));
			
#ifdef SKY_MTSHOW_SUPPORT
        if(filename[0] == '/')
        {			
			ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)&filename[1], strlen(&filename[1]));
        }
        else
        {
            ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename, strlen(filename));
        }
#else
        ucs2_name_len = MMIHEROAPP_GB2UCS((uint16*)hr_ucs2_name_arr, (const uint8 *)filename, strlen(filename));
#endif
			if(	HERO_MMIFILE_CombineFullPath_fix(
				(const uint8*)devName, devNameLen,
				(const uint8*)g_hr_global.dir_ptr, g_hr_global.dir_len,
				(const uint8*)hr_ucs2_name_arr, ucs2_name_len,
				(uint8*)hr_full_path,
				&hr_full_path_len
				))
			{
				sfs_handle = SFS_FindFirstFile((const uint16 *)hr_full_path, &find_data);				
				MMIHR_Trace(("find_data.attr: 0x%x", find_data.attr));
				if(SFS_INVALID_HANDLE != sfs_handle)
				{
					SFS_FindClose(sfs_handle);
					if(SFS_ATTR_DIR & find_data.attr)
					{
						return HR_IS_DIR;
					}
					else
					{
						return HR_IS_FILE;
					}
				}
			}
		}
	}
	
	MMIHR_Trace(("mrapp. hr_info() HR_IS_INVALID"));	
	return HR_IS_INVALID;	
}
int hr_is_find_comma_site()
{
 int i=0;
	  
#ifdef  MRAPP_OVERSEA_SUPPORT	
          for(i=strlen(g_hero_appparam)-5;i>0;i--)
          {
                if(g_hero_appparam[i]==',')
			return i;
          }
          return -1;
#else
          for(i=3;i<strlen(g_hero_appparam);i++)
          {
             if(g_hero_appparam[i]==',')
			return i;
          }
          return -1;
#endif
}

BOOL hr_is_runapp_in_udisk(void)
{
    char tempfile[64]={0};
    char filepath[256]={0};
    hr_screeninfo screen={0};
	int commasite=-1;
	commasite=hr_is_find_comma_site();
    #ifdef  MRAPP_OVERSEA_SUPPORT
    MMIHR_Trace(("hr_is_runapp_in_udisk commasite=%d",commasite));
    if(commasite==-1)
		return FALSE;
    SCI_MEMCPY(tempfile,g_hero_appparam+commasite+1,strlen(g_hero_appparam)-commasite-5);
    MMIHR_Trace(("hr_is_runapp_in_udisk tempfile=%s",tempfile));

    hr_getScreenInfo(&screen);
    sprintf(filepath, "mrp%dX%d\\%s.mrp", screen.width, screen.height,tempfile);
    MMIHR_Trace(("hr_is_runapp_in_udisk filepath=%s",filepath));
    if(hr_info_udisk(filepath)==HR_IS_FILE)
		return TRUE;
    return FALSE;
    #else
    if(!strcmp(g_hero_appparam,"*J,mtshow"))
	return FALSE;
	if(commasite==-1)
	{
      SCI_MEMCPY(tempfile,g_hero_appparam+3,strlen(g_hero_appparam)-3);
	}
	else
	{
	  SCI_MEMCPY(tempfile,g_hero_appparam+3,commasite-3);
	}
    hr_getScreenInfo(&screen);
    sprintf(filepath, "app%d%d\\%s.mrp", screen.width, screen.height,tempfile);
    if(hr_info_udisk(filepath)==HR_IS_FILE)
		return TRUE;
    return FALSE;
    #endif
    
}

BOOL hr_is_enough_space_in_udisk(void)
{
	uint32 space_low2 = 0, space_high2 = 0;
	int32 account=0;  //剩余空间的大小
	int32 unit=0;      //剩余大小的单位
   if(MMIFILE_GetDeviceStatus((uint8*)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
	 {
		MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN,&space_high2,&space_low2);
      
		if(space_high2 || (uint32)space_low2 > (uint32)0x7fffffff)
         {
					account = (space_high2<<22) + (space_low2>>10);//kb
					unit = 1024;
		 }
         else
         {
					account = space_low2;
					unit = 1;
		 }
		 if(account*unit>=512000)
		 	return TRUE;
		 return FALSE;
      }
   
	return FALSE;			
}
#endif

void MMIHEROAPP_setDefFilePath(void)
{
    if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN)
#ifdef DUAL_TCARD_SUPPORT
        || TRUE == MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN)
#endif
        )
    {
        
#ifdef HERO_USE_UDISK_FIRST
		if(hr_is_runapp_in_udisk()&&hr_is_enough_space_in_udisk())
		{
				  g_hr_global.device_type = FS_UDISK;
		}
		else
#endif
        {
        		 g_hr_global.device_type = FS_MMC;
		}
    }
    else
    {
        g_hr_global.device_type = FS_UDISK;
    }
	
	if(g_hr_global.device_type == FS_MMC){//t-card
		g_hr_dualsys_mng.devSel = 'c';		
	}else{//u-disk
		g_hr_dualsys_mng.devSel = 'b';		
	}
	
	g_hr_global.dir_ptr = (uint8*)HERO_ENGINE_ROOT_DIR;
	g_hr_global.dir_len = MMIHEROAPP_getUcstrLen((uint8*)HERO_ENGINE_ROOT_DIR);
	MMIHR_Trace(("MMIHEROAPP_setDefFilePath(), %d, %d", g_hr_global.device_type, g_hr_global.dir_len));

    hr_fs_create_vm_dir();
}

/*****************************************************************************/
void MMIHEROAPP_initAllExtraSrc(void)
{
	/*audio*/
	SCI_MEMSET(&s_hr_audio_mng, 0, sizeof(HERO_APP_AUDIO_MNG_T));
	s_hr_audio_mng.audioHdle = INVALID_HANDLE;
	s_hr_audio_mng.soundVolume = MMI_AUD_VOLUME_MAX;
	s_hr_audio_mng.curStatus = LOADED;
	s_hr_audio_mng.curSetPlayPos = 0;
	
	/*dual sim*/
	g_hr_dualsys_mng.simSel = HEROAPI_GetActiveSim();

#ifdef __HERO_APP_WIFI_SUPPORT__
    s_hr_wifi_mode = FALSE;
#endif
}

/*****************************************************************************/
void MMIHEROAPP_freeAllExtraSrc(void)
{		
	if(s_hr_mp3_resume_locked == TRUE)
	{
		s_hr_mp3_resume_locked = FALSE;
	}
	
	/*audio*/
	if(s_hr_audio_mng.curStatus >= PLAYING && s_hr_audio_mng.curStatus <= PAUSE)
	{
		MMISRVAUD_Stop(s_hr_audio_mng.audioHdle);
		MMISRVMGR_Free(s_hr_audio_mng.audioHdle);
		s_hr_audio_mng.audioHdle = INVALID_HANDLE;
		s_hr_audio_mng.curSetPlayPos = 0;
		s_hr_audio_mng.curStatus = LOADED;
	}
	
#ifndef WIN32
#ifdef HERO_APP_SIMUGAME_OPEN	
#ifdef EXTPCM	
	if(s_hr_pcm_mng.curStatus == PLAYING)
	{
		ExtPCM_SoundClose();
		s_hr_pcm_mng.curStatus = IDLE;
	}
#endif
	
	if(s_hr_envLock_state & ENVLOCK_IRAM){
		MMIHEROAPP_nesActionInd(NES_UNLOCK_IRAM);
	}
	if(s_hr_envLock_state & ENVLOCK_ENV){
		MMIHEROAPP_nesActionInd(NES_UNLOCK_OTHERS);
	}
#endif
#endif
	
	MMIHEROAPP_setLcdAsynMode(0);
	hr_stopSound(0);
	if(s_hr_wallpaperBufNeedRestore){
		s_hr_wallpaperBufNeedRestore = FALSE;
		#ifdef MMI_WALLPAPER_SUPPORT
		MMIIDLE_SetWallpaper(TRUE); /*lint !e718 !e18*/
		#endif
	}
	
#ifdef __HERO_APP_WIFI_SUPPORT__
    if(s_hr_wifi_mode == TRUE)
    {
        s_hr_wifi_mode = FALSE;
    }
#endif
	
	/*image*/
	MMIHEROAPP_gifFree(&s_hr_gifHeader);
	
    if(s_hr_need_restore_backlight)
    {
        MMIHEROAPP_SetAllowTurnOffBacklight(TRUE);
    }

    /*退出VM时恢复状态*/
    if(s_hr_new_sms_action == FALSE)
    {
        s_hr_new_sms_action = TRUE;
    }
	
	MMIHR_Trace(("g_hr_curApp :%d", g_hr_curApp));	
	switch(g_hr_curApp)
	{
		default:
		break;
	}	
}

/*****************************************************************************/
void MMIHEROAPP_setCurApp(HERO_APP_APPENTRY_E app)
{
	g_hr_curApp = (uint32)app;
}
/*****************************************************************************/
uint32 MMIHEROAPP_GetCurApp(void)
{
	return g_hr_curApp;
}

/*****************************************************************************/
void MMIHEROAPP_pauseMp3(void)
{
	MMISRVAUD_ReqVirtualHandle(HERO_VIDEOPLAYER, MMISRVAUD_PRI_NORMAL);
    //MMIAPISET_StopAllRing(FALSE);
	s_hr_mp3_resume_locked = TRUE;
}

/*****************************************************************************/
void MMIHEROAPP_resumeMp3(void)
{
	if(!s_hr_mp3_resume_locked)
	{
		MMISRVAUD_FreeVirtualHandle(HERO_VIDEOPLAYER);
	}
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_getAfreeSnode(void)
{
	int i;
	for(i = 0; i < HERO_APP_SOCKET_MAX_NUM; i++)
	{
		MMIHR_Trace(("s_hr_sockets[%d].s :%d", i, s_hr_sockets[i].s));	
		if(s_hr_sockets[i].s == INVALID_SOCKET)
			return i;
	}
	
	return -1;
}

/*****************************************************************************/
static unsigned char socketsMgrIsInit = 0;
PUBLIC void MMIHEROAPP_freeAllSnode(void)
{
	int i;
	if(!socketsMgrIsInit)
	{
		for(i = 0; i < HERO_APP_SOCKET_MAX_NUM; i++)
		{
			s_hr_sockets[i].s = INVALID_SOCKET;
			s_hr_sockets[i].state = HERO_APP_SOCKET_CLOSED;
		}
		socketsMgrIsInit = 1;
	}

	// free all socket
	for(i = 0; i < HERO_APP_SOCKET_MAX_NUM; i++)
	{
		if(s_hr_sockets[i].s != INVALID_SOCKET)
		{
			int ret = sci_sock_socketclose((long)s_hr_sockets[i].s);
			MMIHR_Trace(("mrapp. hr_closeNetwork() ret:%d ", ret));
		}
		s_hr_sockets[i].s = INVALID_SOCKET;
		s_hr_sockets[i].state = HERO_APP_SOCKET_CLOSED;
	}
}

/*****************************************************************************/
LOCAL int32 MMIHEROAPP_findTheSnode(int32 s)
{
	int i;
	for(i = 0; i < HERO_APP_SOCKET_MAX_NUM; i++)
	{
		MMIHR_Trace(("s_hr_sockets[%d].s :%d, s: %d", i, s_hr_sockets[i].s, s));	
		if(s_hr_sockets[i].s == s)
			return i;
	}
	
	return -1;
}

/*****************************************************************************/
int32 MMIHEROAPP_Sconnect(int32 s, int32 ip, uint16 port)
{
#ifndef WIN32
	struct sci_sockaddr sockAddr;
	int32 ret = 0;
	
	MMIHR_TraceN(("MMIHEROAPP_Sconnect(), socket: %d, ip: 0x%x, port: %d", s, ip, port));
	sockAddr.family = AF_INET;
	sockAddr.port = htons(port);
	sockAddr.ip_addr = (unsigned long)htonl(ip); /*lint !e737*/
	SCI_MEMSET((void*)sockAddr.sa_data, 0, 8*sizeof(char));
	ret = sci_sock_connect((long)s, &sockAddr, sizeof(struct sci_sockaddr));	
	
	return ret;
#else
	return 0;
#endif
}

/*****************************************************************************/
uint8 MMIHEROAPP_GetMp3Lock(void)
{
	return s_hr_mp3_resume_locked;
}

/*****************************************************************************/
LOCAL BOOLEAN HERO_MMIFILE_CombineFullPath_fix(
										  const uint8	   *device_ptr, 	   //in
										  uint16		   device_len,		   //in
										  const uint8	   *dir_ptr,		   //in
										  uint16		   dir_len, 		   //in
										  const uint8	   *file_name_ptr,	   //in, can be NULL
										  uint16		   file_name_len,	   //in
										  uint8 		   *hr_full_path_ptr,	   //out
										  uint16		   *hr_full_path_len_ptr  //out
										  )
{
	uint16		*wchar_ptr = NULL;
	uint32		j = 0;
	
	MMIHR_Trace(("mrapp, CombineFullPath: %d, %d, %d, %d, %d, %d", device_ptr, device_len, dir_ptr, dir_len, file_name_ptr, file_name_len));
	
    if((NULL == device_ptr)||(0 >= device_len || HERO_MMIFILE_DEVICE_NAME_MAX_LEN_AD < device_len))
    {
        return FALSE;
    }
	if(dir_len > HERO_MMIFILE_DIR_NAME_MAX_LEN_AD)
		return FALSE;

    /*modify by zack@20120411*/
    //应该支持传""的hr_info操作
	if(NULL == file_name_ptr || file_name_len > HERO_MMIFILE_FILE_NAME_MAX_LEN_AD)
	{
		return FALSE;
	}
	
    if(NULL == hr_full_path_ptr || NULL == hr_full_path_len_ptr)
    {
        return FALSE;
    }	
	
	//copy device to full path
	MMI_MEMCPY(hr_full_path_ptr, device_len,
		device_ptr, device_len, device_len);
	
	j = device_len;
	
	wchar_ptr = (uint16 *)(hr_full_path_ptr+j);
	*wchar_ptr = MMIFILE_COLON;
	wchar_ptr++;
	*wchar_ptr = MMIFILE_SLASH;
	
	j = j+4;
	
	//copy dir to full path
	if(dir_len > 0)
	{
		MMI_MEMCPY(hr_full_path_ptr+j, dir_len,
			dir_ptr, dir_len, dir_len);
		
		j = j+dir_len;
	}
	
	if(NULL != file_name_ptr && file_name_len > 0)
	{
		wchar_ptr = (uint16 *)(hr_full_path_ptr+j);
		if(dir_len > 0)
		{
			*wchar_ptr = MMIFILE_SLASH;
			j = j+2;
		}
		
		//copy file name to full path
		MMI_MEMCPY(hr_full_path_ptr+j, file_name_len,
			file_name_ptr, file_name_len, file_name_len);
		
		j = j+file_name_len;
	}
	
	*hr_full_path_len_ptr = j;
	
	return TRUE;
}

/*****************************************************************************/
MN_DUAL_SYS_E MMIHEROAPP_GetActiveSim(void)
{
	MMIHR_Trace(("mrapp, curSim: %d, total Sim Num: %d", g_hr_dualsys_mng.simSel, MN_DUAL_SYS_MAX));
	return (MN_DUAL_SYS_E)g_hr_dualsys_mng.simSel;
}

/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_waitToDeactiveMrappPdp(MMIHEROAPP_WAIT2DOPDPACT_FUNC WaitFunc, uint32 data)
{
	if(s_hr_netContext.mrNetActived)
	{
		if(s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVED || s_hr_netContext.gprsState == HERO_APP_GPRS_ACTIVING)//acting can ir directly ?
		{
			MNGPRS_ResetAndDeactivePdpContextEx(MMIHEROAPP_GetActiveSim());
			s_hr_netContext.gprsState = HERO_APP_GPRS_DEACTIVEING;
		}
		
		if(s_hr_netContext.gprsState == HERO_APP_GPRS_DEACTIVEING)
		{
			s_hr_netContext.deactWaitFunc = (void*)WaitFunc; /*lint !e611*/
			s_hr_netContext.data = (uint32)data;
			return TRUE;
		}
		s_hr_netContext.mrNetActived = FALSE;//2008-4-24 add
	}
	
	return FALSE;
}


/*****************************************************************************/
void MMIHEROAPP_playingCheck(void)
{	
	MMIHR_Trace(("MMIHEROAPP_playingCheck(), count: %d", s_hr_mrPlaySound_info.Ckcount));
	
	/*if(s_hr_mrPlaySound_info.Ckcount++ > 0)//delay 1 count
	{
		s_hr_mrPlaySound_info.Ckcount = 0;
		hr_playSound((int)s_hr_mrPlaySound_info.type, s_hr_mrPlaySound_info.data, s_hr_mrPlaySound_info.dataLen, (int32)s_hr_mrPlaySound_info.loop);
	}*/
}

/*****************************************************************************/
PUBLIC void MMIHEROAPP_SetArmClkHigh(void)
{
#ifndef WIN32 
	if (CHNG_FREQ_REQ_HANDLER_NULL == s_hero_app_chng_freq_req_handler){
		s_hero_app_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("mrapp");
		SCI_ASSERT(CHNG_FREQ_REQ_HANDLER_NULL != s_hero_app_chng_freq_req_handler);
	}								  	
	CHNG_FREQ_SetArmClk(s_hero_app_chng_freq_req_handler, FREQ_INDEX_DC_HIGH);
#endif
}

/*****************************************************************************/
PUBLIC void MMIHEROAPP_RestoreARMClk(void)
{
#ifndef WIN32 
	CHNG_FREQ_RestoreARMClk(s_hero_app_chng_freq_req_handler);
#endif
}

/*****************************************************************************/
PUBLIC uint16 MMIHEROAPP_GB2UCS(uint16 *ucs2_ptr, const uint8 *gb_ptr, uint16 len)
{
    return GUI_GB2UCS( ucs2_ptr, gb_ptr, len )*2;
}

/*****************************************************************************/
PUBLIC void MMIHEROAPP_setLcdAsynMode(int32 isSet)
{
	if(!s_hr_lcdasyn_state && isSet)
    {
		LCD_SetBrushMode(MAIN_LCD_ID, LCD_BRUSH_MODE_ASYNC); /*lint !e628 !e718 !e746*/
		s_hr_lcdasyn_state = TRUE;
	}
    else if(s_hr_lcdasyn_state && !isSet)
    {
		LCD_SetBrushMode(MAIN_LCD_ID, LCD_BRUSH_MODE_SYNC); /*lint !e628 !e718 !e746*/	
		s_hr_lcdasyn_state = FALSE;
	}
}

/*****************************************************************************/
void MMIHEROAPP_initApn(void)
{
#ifndef WIN32
	MMIFILE_HANDLE f;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	uint32 bytes_read;
	const uint8 *devName;
	uint16 devNameLen;
    
    devName = MMIHEROAPP_getCurDevName(&devNameLen);
	memset(hr_full_path, 0, sizeof(hr_full_path));
	if(	HERO_MMIFILE_CombineFullPath_fix(
							(const uint8*)devName, devNameLen,
							(const uint8*)L"heroEngineSpr", 2*strlen("heroEngineSpr"),
							(const uint8*)L"useApnAct.dat", 2*strlen("useApnAct.dat"),
							(uint8*)hr_full_path,
							&hr_full_path_len
							))
	{
		f = (HR_FILE_HANDLE)MMIFILE_CreateFile((const uint16 *)hr_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);
		if(f != FFS_INVALID_HANDLE){
			MMIFILE_ReadFile(f, (void *)&gHeroDataAccount, (uint32)sizeof(T_HERO_DATA_ACCOUNT), &bytes_read, NULL);
			hr_close(f);
		}
	}
#endif
}

/*****************************************************************************/
void MMIHEROAPP_saveApn(void)
{
#ifndef WIN32
	MMIFILE_HANDLE f;
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	uint32 bytes_write;
	const uint8 *devName;
	uint16 devNameLen;

    devName = MMIHEROAPP_getCurDevName(&devNameLen);
	memset(hr_full_path, 0, sizeof(hr_full_path));
	if(	HERO_MMIFILE_CombineFullPath_fix(
							(const uint8*)devName, devNameLen,
							(const uint8*)L"heroEngineSpr", 2*strlen("heroEngineSpr"),
							(const uint8*)L"useApnAct.dat", 2*strlen("useApnAct.dat"),
							(uint8*)hr_full_path,
							&hr_full_path_len
							))
	{
		f = (HR_FILE_HANDLE)MMIFILE_CreateFile((const uint16 *)hr_full_path, SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE, NULL, NULL);
		if(f != FFS_INVALID_HANDLE){
			MMIFILE_WriteFile(f, (void *)&gHeroDataAccount, (uint32)sizeof(T_HERO_DATA_ACCOUNT), &bytes_write, NULL);
			hr_close(f);
		}
	}
#endif
}

void MMIHEROAPP_SetBackRun(HERO_APP_APPENTRY_E app)
{
    g_hr_vmBackRun = app;
}

void MMIHEROAPP_PauseAudioPlayer(void)
{
    /*AudioService会根据优先级自动激活处于Paused状态的音频*/
#if 0
    if(s_hr_audio_mng.curStatus == PLAYING)
    {
        s_hr_need_resume_audio = TRUE;
		hr_platEx(2053, NULL, 0, NULL, NULL, NULL);
    }
#endif
}

void MMIHEROAPP_ResumeAudioPlayer(void)
{
    /*AudioService会根据优先级自动激活处于Paused状态的音频*/
#if 0
    if(s_hr_need_resume_audio == TRUE)
    {
        MMIHEROAPP_setSoundVolume(s_hr_audio_mng.soundVolume);
        hr_platEx(2063, NULL, 0, NULL, NULL, NULL);
        s_hr_need_resume_audio = FALSE;
    }
#endif
}
/*****************************************************************************/
int32 MMIHEROAPP_GetNetworkID(uint8** output, int32* output_len)
{
	MN_IMSI_T imsi;
	char *temp = (char*)hr_tempTransBuf;
	MMIHR_Trace(("mrapp. MMIHEROAPP_GetNetworkID()"));
	
    SCI_MEMSET(hr_tempTransBuf, 0x00, sizeof(hr_tempTransBuf));
	imsi = MNSIM_GetImsiEx(MMIHEROAPP_GetActiveSim());
	MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)temp, 3);
	temp[6] = '\0';
    
	*output = (uint8*)&temp[1];
    *output_len = strlen(&temp[1]);

	MMIHR_Trace(("mrapp. MMIHEROAPP_GetNetworkID() output: %s output_len: %d", *output, *output_len));

    return HR_SUCCESS;
}

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetRecordHandle(void)
{
	return s_hr_record_handle;
}

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetRecordHandle(MMISRV_HANDLE_T handle)
{
	s_hr_record_handle = handle;
}

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetAudioHandle(void)
{
	return s_hr_audio_mng.audioHdle;
}

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetAudioHandle(MMISRV_HANDLE_T handle)
{
	s_hr_audio_mng.audioHdle = handle;
}

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_GetMixAudioHandle(void)
{
	return s_hr_mix_audio_handle;
}

/*****************************************************************************/
LOCAL void MMIHEROAPP_SetMixAudioHandle(MMISRV_HANDLE_T handle)
{
	s_hr_mix_audio_handle = handle;
}

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_CreateAudioFileHandle(
													MMISRVAUD_RING_FMT_E ring_type,
													const wchar *name_ptr,
													MMISRVMGR_NOTIFY_FUNC call_back
													)
{
	MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = INVALID_HANDLE;

	req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = call_back;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = s_hr_play_times;
	audio_srv.volume = s_hr_audio_mng.soundVolume;
	audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
	audio_srv.info.ring_file.fmt = ring_type;
    audio_srv.info.ring_file.name = name_ptr;
    audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(name_ptr);
	
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
	MMIHR_Trace(("mrapp MMIHEROAPP_CreateAudioFileHandle handle: %d", handle));

	return handle;
}

/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIHEROAPP_CreateAudioBufHandle(
													MMISRVAUD_RING_FMT_E ring_type,
                     								const uint8* pucData, 
                     								uint32 uiDataLength,
                     								MMISRVMGR_NOTIFY_FUNC call_back
                     								)
{
	MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = INVALID_HANDLE;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = call_back;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = s_hr_play_times;
	audio_srv.volume = s_hr_audio_mng.soundVolume;
	audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = ring_type;
    audio_srv.info.ring_buf.data = pucData;
    audio_srv.info.ring_buf.data_len = uiDataLength;
	
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
	MMIHR_Trace(("mrapp MMIHEROAPP_CreateAudioBufHandle handle: %d", handle));

	return handle;
}

/*****************************************************************************/
PUBLIC void MMIHEROAPP_GetBackRunAPPInfo(HERO_APP_BACKRUN_INFO_T *info)
{/*lint !e18*/
    if(info != NULL)
    {
        info->app_id     = s_hr_backrun_info.app_id;
        info->icon_state = s_hr_backrun_info.icon_state;
    }
}

#ifdef UI_MULTILAYER_SUPPORT
int32 hr_layer_create(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    UILAYER_RESULT_E result = 0;
    UILAYER_CREATE_T layer = {0};
	hr_layer_create_info_t *pReq = (hr_layer_create_info_t *)input;
            
	if(pReq == NULL||output == NULL||output_len == NULL||input_len < sizeof(hr_layer_create_info_t)||pReq->buffer== NULL||((pReq->size) < (pReq->w*pReq->h*2)))
    {   
		return HR_FAILED;
    }

    layer.lcd_id = GUI_MAIN_LCD_ID;
    layer.owner_handle = MMI_HERO_APP_BASE_WIN_ID;
    layer.offset_x = pReq->x;
    layer.offset_y = pReq->y;
    layer.width = pReq->w;
    layer.height = pReq->h;
    layer.is_bg_layer = TRUE;
    layer.is_static_layer = TRUE; 

#ifndef WIN32
    result = hr_layer_create_ex(&layer, &g_hr_common_rsp.p1, pReq->buffer, pReq->size);
#endif
    //MMIHR_Trace(("SKYENGINE [layer] hr_layer_create result:%d handle:%d", result, g_hr_common_rsp.p1));

    if(result == UILAYER_RESULT_SUCCESS)
    {
        *output = (uint8*)&g_hr_common_rsp;
        *output_len = sizeof(UILAYER_HANDLE_T);
        return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_layer_release(int32 param)
{
    GUI_LCD_DEV_INFO dev_info = {0};

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = param;

    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return HR_FAILED;
    }
    //MMIHR_Trace(("SKYENGINE [layer] hr_layer_release actHandle:%d handle:%d", g_hr_active_layer.block_id, param));

    if(g_hr_active_layer.block_id == param)
    {
        SCI_MEMSET(&g_hr_active_layer, 0x00, sizeof(hr_layer_active_info_t))
        g_hr_active_layer.block_id = UILAYER_NULL_HANDLE;
    }

#ifndef WIN32
    hr_layer_release_ex(&dev_info);
#endif

    return HR_SUCCESS;
}

int32 hr_layer_set_postion(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    hr_layer_position_info_t *pReq = (hr_layer_position_info_t *)input;

    if(input == NULL||input_len < sizeof(hr_layer_position_info_t))
    {
		return HR_FAILED;
    }

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;

    //MMIHR_Trace(("SKYENGINE [layer] hr_layer_set_postion actHandle:%d x:%d y:%d", g_hr_active_layer.block_id, pReq->x, pReq->y));
    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return HR_FAILED;
    }
    
    UILAYER_SetLayerPosition(&dev_info, pReq->x, pReq->y);
    return HR_SUCCESS;
}

int32 hr_layer_get_base_handle(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    GUI_LCD_DEV_INFO *dev_info_ptr = MMITHEME_GetDefaultLcdDev();

	if(output == NULL || output_len == NULL || dev_info_ptr == NULL)
    {   
		return HR_FAILED;
    }

    g_hr_common_rsp.p1 = dev_info_ptr->block_id;
	*output = (uint8*)&g_hr_common_rsp;
	*output_len = sizeof(UILAYER_HANDLE_T);
    
    //MMIHR_Trace(("SKYENGINE [layer] hr_layer_get_base_handle baseHandle:%d", g_hr_common_rsp.p1));

	return HR_SUCCESS;
}

int32 hr_layer_set_active(int32 param)
{
    UILAYER_HANDLE_T oldHandle = NULL;
    GUI_LCD_DEV_INFO dev_info = {0};
    GUI_LCD_DEV_INFO *default_dev_info_ptr = MMITHEME_GetDefaultLcdDev();

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = param;

    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return HR_FAILED;
    }

    if(g_hr_active_layer.block_id == UILAYER_NULL_HANDLE)
    {
        oldHandle = default_dev_info_ptr->block_id;
    }
    else
    {
        oldHandle = g_hr_active_layer.block_id;
    }
    g_hr_active_layer.block_id = param;
    //MMIHR_Trace(("MR_JSKY hr_layer_set_active handle=%d old_act_handle = %d", param, oldHandle));    
    return HR_PLAT_VALUE_BASE+oldHandle;
}

int32 hr_layer_get_active_handle(void)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    GUI_LCD_DEV_INFO *default_dev_info_ptr = MMITHEME_GetDefaultLcdDev();

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;

    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return default_dev_info_ptr->block_id;
    }
    else
    {
        return g_hr_active_layer.block_id;
    }
}

int32 hr_layer_set_blt_layer(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    UILAYER_APPEND_BLT_T blt_layer = {0};
    hr_layer_blt_info_t *pReq = (hr_layer_blt_info_t *)input;

	if(input == NULL || input_len < sizeof(hr_layer_blt_info_t))
    {   
		return HR_FAILED;
    }

    //MMIHR_Trace(("hr_layer_set_blt_layer layer1 = %d layer2 = %d layer3 = %d layer4 = %d", pReq->layer1, pReq->layer2, pReq->layer3, pReq->layer4));

#ifndef WIN32
    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = pReq->layer1;
    if(pReq->layer1 != NULL && !UILAYER_IsEmptyLayerHandle(&dev_info) && !hero_UILAYER_IsMainLayerHandle(&dev_info))
    {
        blt_layer.layer_level = UILAYER_LEVEL_NORMAL;
        blt_layer.lcd_dev_info.lcd_id   = dev_info.lcd_id;
        blt_layer.lcd_dev_info.block_id = dev_info.block_id;
        UILAYER_AppendBltLayer(&blt_layer);
    }

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = pReq->layer2;
    if(pReq->layer2 != NULL && !UILAYER_IsEmptyLayerHandle(&dev_info) && !hero_UILAYER_IsMainLayerHandle(&dev_info))
    {
        blt_layer.layer_level = UILAYER_LEVEL_NORMAL;
        blt_layer.lcd_dev_info.lcd_id   = dev_info.lcd_id;
        blt_layer.lcd_dev_info.block_id = dev_info.block_id;
        UILAYER_AppendBltLayer(&blt_layer);
    }

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = pReq->layer3;
    if(pReq->layer3 != NULL && !UILAYER_IsEmptyLayerHandle(&dev_info) && !hero_UILAYER_IsMainLayerHandle(&dev_info))
    {
        blt_layer.layer_level = UILAYER_LEVEL_NORMAL;
        blt_layer.lcd_dev_info.lcd_id   = dev_info.lcd_id;
        blt_layer.lcd_dev_info.block_id = dev_info.block_id;
        UILAYER_AppendBltLayer(&blt_layer);
    }

    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = pReq->layer4;
    if(pReq->layer4 != NULL && !UILAYER_IsEmptyLayerHandle(&dev_info) && !hero_UILAYER_IsMainLayerHandle(&dev_info))
    {
        blt_layer.layer_level = UILAYER_LEVEL_NORMAL;
        blt_layer.lcd_dev_info.lcd_id   = dev_info.lcd_id;
        blt_layer.lcd_dev_info.block_id = dev_info.block_id;
        UILAYER_AppendBltLayer(&blt_layer);
    }
#endif

    return HR_SUCCESS;
}

int32 hr_layer_get_active_layer(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    UILAYER_INFO_T   layer_info = {0};
    GUI_LCD_DEV_INFO dev_info   = {0};
    GUI_LCD_DEV_INFO *default_dev_info_ptr = MMITHEME_GetDefaultLcdDev();
            
	if(output == NULL || output_len == NULL)
		return HR_FAILED;
        
    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;

    if(g_hr_active_layer.block_id != UILAYER_NULL_HANDLE && !UILAYER_IsEmptyLayerHandle(&dev_info))
    {
		g_hr_layer_info.handle = g_hr_active_layer.block_id;
        UILAYER_GetLayerInfo(&dev_info, &layer_info);
    }
    else
    {
		g_hr_layer_info.handle = default_dev_info_ptr->block_id;
        UILAYER_GetLayerInfo(default_dev_info_ptr, &layer_info);
    }

	g_hr_layer_info.w = layer_info.layer_width;
	g_hr_layer_info.h = layer_info.layer_height;
#if !defined(WIN32)
#if (HERO_SPR_VERSION >= 0x10A1140)
	g_hr_layer_info.buffer = (GUI_COLOR_T*)hero_UILAYER_GetLayerBufferPtrEx(&dev_info);
#else
    g_hr_layer_info.buffer = (void*)layer_info.layer_buf_ptr;
#endif
#endif
    //SCI_TRACE_MID("MR_JSKY hr_layer_get_active_layer handle=%d buf_ptr=0x%08x w=%d h=%d", g_hr_layer_info.handle, g_hr_layer_info.buffer, g_hr_layer_info.w, g_hr_layer_info.h);    
	*output = (uint8*)&g_hr_layer_info;
	*output_len = sizeof(g_hr_layer_info);
	return HR_SUCCESS;
}

int32 hr_layer_set_alpha(BOOLEAN enable, int32 param)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    
    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;
    
    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return HR_FAILED;
    }

    if(enable)
    {
        if(param > 255)
        {
            param = 255;
        }
        param = 255-param;
    }
    else
    {
        param = 255;
    }
    //SCI_TRACE_MID("MR_JSKY hr_layer_set_alpha enable=%d handle=%d alpha=%d", enable, g_hr_active_layer.block_id, param);    
    UILAYER_SetLayerAlpha(&dev_info, param);
    return HR_SUCCESS;
}

int32 hr_layer_set_transparent(BOOLEAN enable, int32 param)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    
    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;
    
    if(UILAYER_IsEmptyLayerHandle(&dev_info))
    {
        return HR_FAILED;
    }
    //SCI_TRACE_MID("MR_JSKY hr_layer_set_transparent enable=%d handle=%d transprent=%d", enable, g_hr_active_layer.block_id, param);    
    UILAYER_SetLayerColorKey(&dev_info, enable, MMI_BLUE_COLOR);

    return HR_SUCCESS;
}

int32 hr_jsky_push_clip(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    GUI_RECT_T *rect = NULL;

    if(g_hr_active_layer.clips_top >= HR_LAYER_CLIP_STACK_COUNT)
    {
        return HR_FAILED;
    }

    rect = &g_hr_active_layer.clips[g_hr_active_layer.clips_top];

    rect->left = g_hr_active_layer.clip.left; 
    rect->top  = g_hr_active_layer.clip.top;
    rect->right  = g_hr_active_layer.clip.right;
    rect->bottom = g_hr_active_layer.clip.bottom;

    g_hr_active_layer.clips_top++;
    //SCI_TRACE_MID("MR_JSKY hr_jsky_push_clip total=%d rect[%d %d %d %d]", g_hr_active_layer.clips_top, rect->left, rect->top, rect->right, rect->bottom);    
	return HR_SUCCESS;
}

int32 hr_jhero_pop_clip(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    GUI_RECT_T *rect = NULL;

    if(g_hr_active_layer.clips_top <= 0)
    {
        return HR_FAILED;
    }

    rect = &g_hr_active_layer.clips[--(g_hr_active_layer.clips_top)];

    g_hr_active_layer.clip.left = rect->left; 
    g_hr_active_layer.clip.top  = rect->top;
    g_hr_active_layer.clip.right  = rect->right;
    g_hr_active_layer.clip.bottom = rect->bottom;
    //SCI_TRACE_MID("MR_JSKY hr_jhero_pop_clip total=%d rect[%d %d %d %d]", g_hr_active_layer.clips_top, rect->left, rect->top, rect->right, rect->bottom);    
	return HR_SUCCESS;
}

int32 hr_jhero_set_clip(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	skyclip *clip = (skyclip*)input;

	if(clip != NULL)
	{
    	//SCI_TRACE_MID("MR_JSKY hr_jhero_set_clip l:%d t:%d r:%d b:%d", clip->x1, clip->y1, clip->x2, clip->y2);
		g_hr_active_layer.clip.left = clip->x1;
		g_hr_active_layer.clip.top  = clip->y1;
		g_hr_active_layer.clip.right  = clip->x2;
		g_hr_active_layer.clip.bottom = clip->y2;

		return HR_SUCCESS;
	}
	else 
	{
		return HR_FAILED;
	}
}

int32 hr_jhero_get_clip(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	skyclip *clip = (skyclip*)output;

	if((output != NULL) && (output_len != NULL))
	{
		clip->x1 = g_hr_active_layer.clip.left;
		clip->y1 = g_hr_active_layer.clip.top;
		clip->x2 = g_hr_active_layer.clip.right;
		clip->y2 = g_hr_active_layer.clip.bottom;
        
    	//MMIHR_Trace(("MR_JSKY hr_jhero_get_clip l:%d t:%d r:%d b:%d", clip->x1, clip->y1, clip->x2, clip->y2));
    	
		*output_len = sizeof(skyclip);
		return HR_SUCCESS;
	}
	else
	{
		return HR_FAILED;
	}
}
#endif

PUBLIC int32 MMIHEROAPP_GetVMState(void)
{/*lint !e18*/
    int32 vm_state = 0;
    
    if(s_hr_allReady)
    {
        if(MMIHEROAPP_IsVMPaused()) /*lint !e718 !e746*/
        {
            vm_state = VM_PAUSE;
        }
        else
        {
            if(MMIHEROAPP_GetCurHungApp() != HERO_APP_MAX) /*lint !e718 !e746*/
            {
                vm_state = VM_BACK_RUN;
            }
            else
            {
                vm_state = VM_RUN;
            }
        }
    }
    else
    {
        vm_state = VM_STOP;
    }

    MMIHR_Trace(("mrapp MMIHEROAPP_GetVMState vm_state = %d", vm_state));
    return vm_state;
}

int32 getHeroEngineState(void)
{
	return MMIHEROAPP_GetVMState();
}

#if 1//Kalyy 2012-06-29
PUBLIC BOOLEAN MMIHEROAPP_isRunning(void)
{
    return MMK_IsOpenWin(MMI_HERO_APP_BASE_WIN_ID);
}
#endif


/* end:add by Tommy.yan 20111102 */

///mike.song add by 20140610
#ifdef HERO_ENGINE_SUPPORT
#include "mmialarm_service.h"
#include "mmischedule_export.h"
#include "mmicom_time.h"
#include "dal_time.h"

#define HERO_LOG(x)
#define HERO_LOGQ

#define HERO_MAX_IMSI_LENG     20
#define HERO_MAX_IMEI_LENG     20

#define HERO_TIMER_TICK   (50)

typedef struct HERO_CardInfo
{
	unsigned int m_sim_count;   // HERO_SIM_CARD1 | HERO_SIM_CARD2 |...
	unsigned int m_active_sim;  // HERO_SIM_CARD1
	char simImsi[MMI_DUAL_SYS_MAX][HERO_MAX_IMSI_LENG];
	char simImei[MMI_DUAL_SYS_MAX][HERO_MAX_IMEI_LENG];
}HERO_CardInfo;
static HERO_CardInfo g_hero_sim_card_info = {0};

static void *hero_engine_malloc(unsigned int size)
{
	void *ptr = NULL;
	if (0 == size)
	{
		return NULL;
	}
	return SCI_ALLOC(size);
}

static void hero_engine_free(char **pptr)
{
	if(pptr == NULL) return;
	SCI_FREE(*pptr);
}

static unsigned int init_valid_sim(void)
{
	uint32 i = 0;
	unsigned int simCount = HERO_SIM_NONE;
	unsigned int sim = 1;

	for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
	{
		int plmnStatus = MMIAPIPHONE_GetPlmnStatus(i);
		if (MMIPHONE_IsSimOk(i) && (PLMN_NO_SERVICE != plmnStatus && PLMN_EMERGENCY_ONLY != plmnStatus && PLMN_EMERGENCY_SIM_INVALID != plmnStatus))
		{
			simCount |= sim;
			MMIHR_Trace(("mr_sim_initialize SIM %d OK", i + 1));
		}
		sim <<= 1;
	}
	return simCount;
}

static int get_valid_sim_count(unsigned int ValidSim)
{
	int i;
	unsigned int count = 0;
	unsigned int sim = 1;

	for (i = 0; i < MMI_DUAL_SYS_MAX; ++i)
	{
		if(ValidSim & sim)
		{
			count++;
		}
		sim <<= 1;
	}
	HERO_LOG(("get_valid_sim_count %d",count));
	return count;
}

static int get_action_sim_index(unsigned int actionSim);
static unsigned int init_action_sim(unsigned int simCount)
{
	int i = 0;
	int actionSimIndex = 0;
	unsigned int sim = 1;
	unsigned int actionSim = HERO_SIM_NONE;
	int tmp = 0;

	if(simCount == HERO_SIM_NONE)
	{
		return HERO_SIM_NONE;
	}

	// 随机取一张SIM卡
	srand(SCI_GetTickCount());
	actionSimIndex = 0; //rand()%get_valid_sim_count(simCount); 此处注释掉
	for (i = 0; i < MMI_DUAL_SYS_MAX; ++i)
	{
		if(simCount & sim)
		{
			if(get_action_sim_index(sim) == HEROAPI_GetActiveSim())
			{
				actionSim = sim;
				break;
			}
			if(tmp == actionSimIndex)
			{
				actionSim = sim;
				//break; 此处注释掉
			}
			tmp++;
		}
		sim <<= 1;
	}
	HERO_LOG(("sh_mmi_getSim active_sim=%d",actionSim));
	return actionSim;
}

static int get_action_sim_index(unsigned int actionSim)
{
	int i=0;
	unsigned int sim = 1;
	for(i = 0; i < MMI_DUAL_SYS_MAX; ++i)
	{
		if(actionSim & sim)
		{
			return i;
		}
		sim <<= 1;
	}
	return 0;
}

static void init_sim_imsi(HERO_CardInfo *pCardInfo)
{
	int i=0;
	uint32 sim = 1;
	MN_IMSI_T imsi = {0};

	for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
	{
		if(pCardInfo->m_sim_count & sim)
		{
			imsi = MNSIM_GetImsiEx(get_action_sim_index(sim));

			MMIHEROAPP_bcd2dec(imsi.imsi_val, (uint8*)pCardInfo->simImsi[i], 8);
			SCI_MEMCPY((uint8*)pCardInfo->simImsi[i],(uint8*) (&pCardInfo->simImsi[i][1]), 15);
			pCardInfo->simImsi[i][15] = 0x00;

			HERO_LOG(("init_sim_imsi index=%d,imsi=%s",i,pCardInfo->simImsi[i]));
		}
		sim <<= 1;
	}
}

static void init_sim_imei(HERO_CardInfo *pCardInfo)
{
	int i=0;
	uint32 sim = 1;
	MN_IMEI_T imei = {0};
	BOOLEAN ret;

	for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
	{
		if(pCardInfo->m_sim_count & sim)
		{
			ret = MNNV_GetIMEIEx(get_action_sim_index(sim), imei);
			if(!ret)
			{
				ret = MNNV_GetIMEIEx(MN_DUAL_SYS_1, imei);
			#ifdef MULTI_SIM_SYS_DUAL
				if(!ret)
				{
					ret = MNNV_GetIMEIEx(MN_DUAL_SYS_2, imei);
				}
			#endif
			}
			if(ret)
			{
				MMIHEROAPP_bcd2dec(imei, (uint8*)pCardInfo->simImei[i], 8);
				SCI_MEMCPY(pCardInfo->simImei[i], &pCardInfo->simImei[i][1], 15);
				pCardInfo->simImei[i][15] = 0x00;
			}
			HERO_LOG(("init_sim_imei index=%d,imei=%s",i,pCardInfo->simImei[i]));
		}
		sim <<= 1;
	}
}

unsigned int hero_get_action_sim()
{
	return g_hero_sim_card_info.m_active_sim;
}

char *hero_get_action_sim_imsi()
{
	int index = get_action_sim_index(g_hero_sim_card_info.m_active_sim);
	return g_hero_sim_card_info.simImsi[index];
}

char *hero_get_action_sim_imei()
{
	int index = get_action_sim_index(g_hero_sim_card_info.m_active_sim);
	return g_hero_sim_card_info.simImei[index];
}

static void hero_init_cell_info();
void hero_init_sim_card_info(init_sim_info_cb cb)
{
	int ret = 0;//-1;
	if(g_hero_sim_card_info.m_sim_count != init_valid_sim())
	{
		g_hero_sim_card_info.m_active_sim = HERO_SIM_NONE;
	}
	if(g_hero_sim_card_info.m_active_sim == HERO_SIM_NONE)
	{
		g_hero_sim_card_info.m_sim_count = init_valid_sim();
		g_hero_sim_card_info.m_active_sim = init_action_sim(g_hero_sim_card_info.m_sim_count);
		if(g_hero_sim_card_info.m_sim_count != HERO_SIM_NONE)
		{
			init_sim_imsi(&g_hero_sim_card_info);
			init_sim_imei(&g_hero_sim_card_info);
			hero_init_cell_info();
			ret = 0;
		}
	}
	if(cb)
	{
		cb(ret);
	}
}

#if 0
/*
* 获取活动的SIM卡plmn号
*/
char *hero_get_action_sim_plmn(unsigned int *output_len)
{
}
#endif

typedef struct timerManagerSt
{
	sh_timer_cb cb;
	uint32 delayTick; // 循环计数
	uint32 delayCount;
	void *userData;
	BOOL isStop;
	BOOL isUser;
}timerManager;
static timerManager gTimerManager[HERO_MAX_TIMER_COUNT] = {0};
static int gTimerCount = 0;
void heroTimerCallback()
{
	int i = 0;
	for(i=0; i<HERO_MAX_TIMER_COUNT; i++)
	{
		timerManager *pTimerManager = &gTimerManager[i];
		if(pTimerManager->isUser && !pTimerManager->isStop) 
		{
			pTimerManager->delayTick ++;
			if(pTimerManager->delayTick == pTimerManager->delayCount)
			{
				pTimerManager->delayTick = 0;
				pTimerManager->isStop = TRUE;
				if(pTimerManager->cb)
				{
					pTimerManager->cb(pTimerManager->userData);
				}
			}
		}
	}
}

int hero_create_timerId()
{
	int i = 0;
	for(i=0; i<HERO_MAX_TIMER_COUNT; i++)
	{
		if(!gTimerManager[i].isUser) break;
	}
	if(i != HERO_MAX_TIMER_COUNT)
	{
		memset(&gTimerManager[i],0,sizeof(timerManager));
		gTimerManager[i].isUser = TRUE;
		gTimerManager[i].isStop = TRUE;
		if(gTimerCount == 0)
		{
			MMIHERO_startDsmTimer(HERO_TIMER_TICK);
		}
		gTimerCount ++;
		HERO_LOG(("hero_create_timerId gTimerCount=%d",gTimerCount));
		return i+1;
	}
	return 0;
}

void hero_delete_timer(int timerId)
{
	if(timerId <= 0 || timerId > HERO_MAX_TIMER_COUNT) return;
	hero_stop_timer(timerId);
	memset(&gTimerManager[timerId - 1],0,sizeof(timerManager));
	if(--gTimerCount == 0)
	{
		MMIHERO_StopDsmTimer();
	}
	HERO_LOG(("hero_delete_timer gTimerCount=%d",gTimerCount));
}

void hero_stop_timer(int timerId)
{
	int count = 0;
	if(timerId <= 0 || timerId > HERO_MAX_TIMER_COUNT) return;
	HERO_LOG(("hero_stop_timer AAA %d",timerId));

	timerId = timerId - 1;
	gTimerManager[timerId].isStop = TRUE;
}

void hero_start_timer(int timerId,unsigned int delay,sh_timer_cb cb,void *userData)
{
	int i = 0;
	int count = 0;
	if(timerId <= 0 || timerId > HERO_MAX_TIMER_COUNT) return;
	HERO_LOG(("hero_start_timer AAA timerId=%d",timerId));

	timerId = timerId - 1;
	gTimerManager[timerId].isStop = FALSE;
	gTimerManager[timerId].cb = cb;
	gTimerManager[timerId].delayTick = 0; // 循环计数
	gTimerManager[timerId].delayCount = (delay/HERO_TIMER_TICK == 0 ? 1 : delay/HERO_TIMER_TICK);
	gTimerManager[timerId].userData = userData;
}

void hero_get_manufactory(char *manufactory,int bufLen)
{
	char tempBuf[32] = {0};
	sprintf(tempBuf,"%s_%s",HERO_ENGINE_MANUFACTORY,HERO_ENGINE_MANUFACTORY_EX);
	strncpy(manufactory,tempBuf,bufLen);
}

void hero_get_type(char *type,int bufLen)
{
	#if defined(HERO_ENGINE_SUPPORT_CLIENT_INFO) && !defined(WIN32)
	{
		extern void hr_toLower(kal_uint8 *str);
		U8 length = 0;
		kal_uint8  tmp[8]; 
		memset(tmp, '\0', sizeof(tmp) );
		length = MIN(strlen(HERO_ENGINE_HANDSET),bufLen);
		//strncpy(tmp, HERO_ENGINE_HANDSET, length);
		strncpy(tmp, HERO_ENGINE_HANDSET, MIN((sizeof(tmp)-1),length));
		hr_toLower(tmp);
		//strncpy(type, tmp, MIN(strlen(tmp),bufLen));
		strncpy(type, tmp, MIN((sizeof(tmp)-1),bufLen));
		//strncpy(type,HERO_ENGINE_HANDSET,MIN(strlen(HERO_ENGINE_HANDSET),bufLen));
	}
	#else /* HERO_ENGINE_SUPPORT_CLIENT_INFO */	
	strncpy(type,HERO_ENGINE_HANDSET,MIN(strlen(HERO_ENGINE_HANDSET),bufLen));
	#endif /* HERO_ENGINE_SUPPORT_CLIENT_INFO */
}

int hero_get_screen_width()
{
	return HR_SCREEN_WIDTH; //MMI_MAINSCREEN_WIDTH;
}

int hero_get_screen_height()
{
	return HR_SCREEN_HEIGHT; //MMI_MAINSCREEN_HEIGHT;
}

/****************************************************
*					网络							*
*****************************************************/
#define DSM_MAX_CONNECT_TIMEOUT (60*1000)	//连接超时时间
#define HERO_CONNECT_TIMER_TICK (25000)

#define HERO_SOCKET_MAX_COUNT (5)
#define HERO_SOCKET_BASE_OFFSET (1)

typedef struct tagHeroSocketItemSt{
	int realSocketId;
	int timeroutId;
	int connectState;
	socket_connect_cb cb;
}heroSocketItemSt;

typedef struct tagHeroSocketMgrSt{
	heroSocketItemSt socketMgr[HERO_SOCKET_MAX_COUNT];
}heroSocketMgrSt;

static heroSocketMgrSt* hero_app_net_get_mgr(void)
{
	static unsigned char socketMgrIsInit = 0;
	static heroSocketMgrSt sSocketMgr = {0};
	if(!socketMgrIsInit)
	{
		int i = 0;
		memset(&sSocketMgr, 0, sizeof(heroSocketMgrSt));
		for (i = 0; i < HERO_SOCKET_MAX_COUNT; ++i)
		{
			sSocketMgr.socketMgr[i].realSocketId = -1;
		}
		socketMgrIsInit = 1;
	}
	return &sSocketMgr;
}

static heroSocketItemSt* hero_app_net_alloc_socket(void)
{
	int i = 0;
	heroSocketMgrSt* mgr = hero_app_net_get_mgr();
	for (i  = 0; i < HERO_SOCKET_MAX_COUNT; ++i)
	{
		if(mgr->socketMgr[i].realSocketId == -1)
		{
			return (mgr->socketMgr + i);
		}
	}
	return NULL;
}

static int hero_app_net_socket2index(heroSocketItemSt* socket)
{
	heroSocketMgrSt* mgr = hero_app_net_get_mgr();
	return (socket - mgr->socketMgr) + HERO_SOCKET_BASE_OFFSET;
}

static void hero_app_net_free_socket(heroSocketItemSt* socket)
{
	if(socket->timeroutId != 0)
	{
		hero_delete_timer(socket->timeroutId);
		socket->timeroutId = 0;
	}
	memset(socket, 0, sizeof(heroSocketItemSt));
	socket->realSocketId = -1;
}

static heroSocketItemSt* hero_app_net_index2socket(int index)
{
	heroSocketMgrSt* mgr = hero_app_net_get_mgr();

	index = index - HERO_SOCKET_BASE_OFFSET;
	if (index < 0 || index >= HERO_SOCKET_MAX_COUNT)
		return NULL;

	return mgr->socketMgr + index;
}

static void hero_app_net_socket_close(int s)
{
	heroSocketItemSt* socket = hero_app_net_index2socket(s);
	if (socket != NULL && socket->realSocketId != -1)
	{
		sci_sock_socketclose((long)socket->realSocketId);
		hero_app_net_free_socket(socket);
	}
}

static void hero_app_net_socket_close_all()
{
	int i = 0;
	for (i = 0; i < HERO_SOCKET_MAX_COUNT; ++i)
	{
		hero_app_net_socket_close(i + HERO_SOCKET_BASE_OFFSET);
	}
}

static void hero_get_action_sim_plmn(char *buff)
{
	char *pImsi = hero_get_action_sim_imsi();
	strncpy(buff,pImsi,6);
}

static init_nwk_account_cb gInitNwkAccountCallback = NULL;
static BOOL account_init_flag = FALSE;
static unsigned int dsm_nwk_account_id = 0;
static BOOL gprs_is_connected = FALSE;
static MMI_RESULT_E hero_nwk_handlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	HERO_LOG(("HandleKSCPsMsg=%d,result=%d,nsapi=%d",msg_ptr->msg_id,msg_ptr->result,msg_ptr->nsapi));
	switch(msg_ptr->msg_id) 
	{
	case MMIPDP_ACTIVE_CNF:
		dsm_nwk_account_id = (MMIPDP_RESULT_SUCC == msg_ptr->result ? msg_ptr->nsapi : 0);
		HERO_LOG(("nwk_handlePsMsg dsm_nwk_account_id %d",dsm_nwk_account_id));
		account_init_flag = (MMIPDP_RESULT_SUCC == msg_ptr->result ? TRUE : FALSE);
		if(gInitNwkAccountCallback)
		{
			gInitNwkAccountCallback(MMIPDP_RESULT_SUCC == msg_ptr->result ? 0 : -1);
			gInitNwkAccountCallback = NULL;
		}
	break;
	case MMIPDP_DEACTIVE_CNF:
	case MMIPDP_DEACTIVE_IND:
	break;
	default:
	break;
    }
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
BOOL hero_init_nwk_account_ex(HERO_HTTP_TYPE conntype,init_nwk_account_cb cb)
#else
BOOL hero_init_nwk_account(HERO_HTTP_TYPE conntype,init_nwk_account_cb cb)
#endif
{
	int32 resValue = HR_FAILED;
	char *apn = (char*)(conntype == HERO_HTTP_TYPE_CMWAP ? "cmwap" : "cmnet"); 
	uint8 *user_ptr = ""; 
	uint8 *passwd_ptr = "";
	uint32 dns = 0;
	char plmnBuff[10] = {0};
	MMIPDP_ACTIVE_INFO_T app_info = {0};
	BOOLEAN ret = FALSE;

	HERO_LOG(("hero_init_nwk_account %d",cb));
	if(cb == NULL)
	{
		return FALSE;
	}
	HERO_LOG(("sh_init_http_data %d",account_init_flag));
	if(account_init_flag)
	{
		cb(0);
		return TRUE;
	}
	gprs_is_connected = TRUE;
	if(MMIAPISET_GetFlyMode() || !MMIPHONE_IsSimOk(get_action_sim_index(hero_get_action_sim())) )
	{
		cb(-1);
		HERO_LOG(("exception hero_init_nwk_account failed !!!!"));
		return FALSE;
	}

	hero_app_net_socket_close_all();
	hero_get_action_sim_plmn(plmnBuff);
	HERO_LOG(("hero_get_action_sim_plmn %s",plmnBuff));
#ifdef __HERO_ENGINE_OVERSEA__	
	if(strncmp(plmnBuff, "460", 3) != 0)
	{
		T_HR_APN *apnAccount  = NULL;
		BOOLEAN tmpVmAllReady = s_hr_allReady;

		s_hr_allReady = TRUE;
		apnAccount  = hr_getAPN(plmnBuff);
		if(apnAccount == NULL)
		{
			plmnBuff[6] = '\0';
	        apnAccount = hr_getAPN(plmnBuff);
			if(apnAccount == NULL)
			{
				plmnBuff[5] = '\0';
				apnAccount = hr_getAPN(plmnBuff);
				s_hr_allReady = tmpVmAllReady;
				if(apnAccount == NULL)
				{
					cb(-1);
					HERO_LOG(("exception init_nwk_account failed !!! plim = %s",plmnBuff));
					return FALSE;
				}
			}
		}
		s_hr_allReady = tmpVmAllReady;

		HERO_LOG(("hr_getAPN %s",apnAccount->apn));
		apn = apnAccount->apn;
		user_ptr = (uint8*)apnAccount->userName;
		passwd_ptr = (uint8*)apnAccount->passWord;
	#ifndef WIN32
		dns = (apnAccount->dns[0]<<24)|(apnAccount->dns[1]<<16)|(apnAccount->dns[2]<<8)|(apnAccount->dns[3]);
		sci_setsparedns(dns, 0);
	#endif
	}
	else
#endif		
	{
		if(strncmp(plmnBuff, "46001", 5) ==0)  // 联通gsm
		{
			if (strncmp(apn, "cmwap",5)  == 0)
			{
				apn = "uniwap";
			}
			else
			{
				apn = "uninet";
			}
		}
	}
	HERO_LOG(("hero_init_nwk_account apn = %s",apn));

	app_info.app_handler = MMI_MODULE_HEROAPP;
	app_info.apn_ptr = (char*)apn;
	app_info.user_name_ptr = (char*)user_ptr;
	app_info.psw_ptr = (char*)passwd_ptr;
	app_info.dual_sys = get_action_sim_index(g_hero_sim_card_info.m_active_sim);
	app_info.priority = 3;
	app_info.ps_service_rat = MN_UNSPECIFIED;
	app_info.handle_msg_callback = hero_nwk_handlePsMsg;
	app_info.ps_service_type = BROWSER_E;
	app_info.storage = MN_GPRS_STORAGE_ALL;
	app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
#ifdef IPVERSION_SUPPORT_V4_V6
	app_info.ip_type = MMICONNECTION_IP_V4;
#endif
#ifdef IPVERSION_SUPPORT_V4_V6
	{
		MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
		MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
		apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(app_info.dual_sys);
		MMIHR_Trace(("MMIAPICONNECTION_GetApnTypeIndex: %d", apn_idx_arr));
		if (apn_idx_arr != NULL)
		{
			linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(app_info.dual_sys, apn_idx_arr->internet_index[app_info.dual_sys].index);
			MMIHR_Trace(("MMIAPICONNECTION_GetLinkSettingItemByIndex: %d", linksetting));
			if(PNULL != linksetting)
			{
				app_info.apn_ptr = (char*)linksetting->apn;
				app_info.user_name_ptr = (char*)linksetting->username;
				app_info.psw_ptr = (char*)linksetting->password;
				app_info.ip_type = linksetting->ip_type;
				MMIHR_Trace(("app_info.apn_ptr: %s, app_info.user_name_ptr = %s, app_info.psw_ptr = %s, app_info.ip_type = %d", linksetting->apn, linksetting->username, linksetting->password, linksetting->ip_type));
			}
		}
	}
#endif
	gInitNwkAccountCallback = cb;
	ret = MMIAPIPDP_Active(&app_info);
	HERO_LOG(("MMIAPIPDP_Active apn:%s ret: %d", apn, ret));
	if(!ret) {
		gInitNwkAccountCallback = NULL;		
	}
	return ret;
}

static char gDsmHostBuffer[128] = {0};
static int gDsmGetHostNameTimerId = 0;
static int gDsmGetHostNameTimerCount = 0;
static hero_dsn_cb gDsmGetHostNameCb = NULL;
static void get_host_ind_timerout_callback(void* userData)
{
	int32 ip = 0;
	uint32 ipExt = 0;
	int resValue = 0;

	resValue = sci_parse_host((char *)gDsmHostBuffer,&ipExt,1);
	HERO_LOG(("hero_get_host_by_name() %s,resValue: %d,count=%d", gDsmHostBuffer,resValue,gDsmGetHostNameTimerCount));
	if(resValue ==0)
	{
		ip = ipExt;
		ip = ntohl(ip); 
		if(gDsmGetHostNameCb != NULL)
		{
			gDsmGetHostNameCb(ip);
			gDsmGetHostNameCb = NULL;
		}
		memset(gDsmHostBuffer,0,sizeof(gDsmHostBuffer));
		gDsmGetHostNameTimerCount = 0;
		hero_delete_timer(gDsmGetHostNameTimerId);
		gDsmGetHostNameTimerId = 0;
		return;
	}
	if(gDsmGetHostNameCb != NULL)
	{
		gDsmGetHostNameCb(HERO_SOC_ERROR);
		gDsmGetHostNameCb = NULL;
	}
	memset(gDsmHostBuffer,0,sizeof(gDsmHostBuffer));
	gDsmGetHostNameTimerCount = 0;
	hero_delete_timer(gDsmGetHostNameTimerId);
	gDsmGetHostNameTimerId = 0;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int hero_get_host_by_name_ex(const char *name, hero_dsn_cb cb)
#else
int hero_get_host_by_name(const char *name, hero_dsn_cb cb)
#endif
{
#ifdef __USE_SCI_GETHOSTBYMANE__ //clark add 6531e 20170329
	struct sci_hostent * hostent = NULL;
	int32 hostAddr = 0;
	int resValue = HERO_SOC_ERROR;
	if(!account_init_flag || name == NULL)
	{
		return resValue;
	}
	hostent = sci_gethostbyname((char*)name);//6531E
	if(hostent != NULL)
	{
		SCI_MEMCPY(&hostAddr, hostent->h_addr_list[0], 4);
		resValue = ntohl(hostAddr);
		return resValue;
	}	
	return HERO_SOC_ERROR;
#else
	int32 ip = 0;
	uint32 ipExt = 0;
	int resValue = HERO_SOC_ERROR;
	if(!account_init_flag || name == NULL)
	{
		return resValue;
	}

	resValue = sci_parse_host((char *)name,&ipExt,1);

	if(resValue ==0)
	{
		ip = ipExt;
		ip = ntohl(ip); 
		return ip;
	}
	else if(resValue ==1 && cb)
	{
		memset(gDsmHostBuffer,0,sizeof(gDsmHostBuffer));
		strncpy(gDsmHostBuffer,name,sizeof(gDsmHostBuffer)-1);
		gDsmGetHostNameCb = cb;
		gDsmGetHostNameTimerCount = 0;

		if(gDsmGetHostNameTimerId == 0)
			gDsmGetHostNameTimerId = hero_create_timerId();
		hero_stop_timer(gDsmGetHostNameTimerId);
		hero_start_timer(gDsmGetHostNameTimerId,(35*1000),get_host_ind_timerout_callback,(void*)0);

		return HERO_SOC_WOULDBLOCK;
	}

	return HERO_SOC_ERROR;
#endif
//clark add 6531e 20170329
	
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
signed int hero_socket_create_ex()
#else
signed int hero_socket_create()
#endif
{
	int socket = INVALID_SOCKET;

	heroSocketItemSt *pSocektItem = hero_app_net_alloc_socket();
	if(pSocektItem == NULL)
	{
		return -1;
	}

	socket = sci_sock_socket(AF_INET, SOCK_STREAM, 0, dsm_nwk_account_id);
	HERO_LOG(("hero_socket_create ==== %d,accountid=%d",socket,dsm_nwk_account_id));
	if(socket != INVALID_SOCKET)
	{
		sci_sock_setsockopt(socket, SO_NBIO, NULL);
	}
	else
	{ 
		int32 e = sci_sock_errno(socket);
		HERO_LOG(("mrapp. errorno: %d", e)); 
		return -1;
	}

	pSocektItem->realSocketId = socket;
	return hero_app_net_socket2index(pSocektItem);
}

static int32 hero_sconnect(int32 s, int32 ip, uint16 port)
{
	struct sci_sockaddr sockAddr;
	int32 ret = 0;
	heroSocketItemSt *pSocketItem = hero_app_net_index2socket(s);

	MMIHR_Trace(("hero_sconnect(), socket: %d, ip: 0x%x, port: %d, pSocketItem:%d", s, ip, port, pSocketItem));
	if(pSocketItem == NULL || pSocketItem->realSocketId == -1)
	{
		return ret;
	}

	sockAddr.family = AF_INET;
	sockAddr.port = htons(port);
	sockAddr.ip_addr = (unsigned long)htonl(ip); /*lint !e737*/
	SCI_MEMSET((void*)sockAddr.sa_data, 0, 8*sizeof(char));
	ret = sci_sock_connect((long)pSocketItem->realSocketId, &sockAddr, sizeof(struct sci_sockaddr));

	return ret;
}

static void soc_connect_timerout_callback(void* userData)
{
	heroSocketItemSt *pSocketItem = hero_app_net_index2socket((int)userData);
	if(pSocketItem != NULL && pSocketItem->realSocketId != -1)
	{
		int32 ret = 0, error = 0;
		sci_fd_set writefds;

		SCI_FD_ZERO (&writefds);
		SCI_FD_SET((long)pSocketItem->realSocketId, &writefds);

		ret = sci_sock_select(NULL, &writefds, NULL, 0); 
		MMIHR_Trace(("hero_video_connect_timer_out_cb() ret: %d ", ret));
		if(ret > 0)
		{
			if(SCI_FD_ISSET(pSocketItem->realSocketId, &writefds))
			{
				MMIHR_Trace(("hero_video_connect_timer_out_cb true "));

				pSocketItem->connectState = HERO_SOC_STATE_CONNECTED;
				if(pSocketItem->timeroutId != 0)
				{
					hero_delete_timer(pSocketItem->timeroutId);
					pSocketItem->timeroutId = 0;
				}

				if(pSocketItem->cb != NULL)
				{
					pSocketItem->cb(pSocketItem->connectState, (int)userData);
					pSocketItem->cb = NULL;
				}
			}
		}
		else if(ret == 0)
		{
			pSocketItem->connectState = HERO_SOC_STATE_CONNECTING;
			if(pSocketItem->timeroutId != 0)
			{
				hero_start_timer(pSocketItem->timeroutId, 100, soc_connect_timerout_callback, (void*)userData);
			}
		}
		else
		{
			hero_app_net_socket_close((int)userData);
		}
	}
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int hero_socket_connect_ex(int socket, int ip, unsigned short port,socket_connect_cb cb)
#else
int hero_socket_connect(int socket, int ip, unsigned short port,socket_connect_cb cb)
#endif
{
	int32 conRet = HERO_SOC_STATE_CONNECT_FALIED;
	heroSocketItemSt *pSocketItem = hero_app_net_index2socket(socket);

	MMIHR_Trace(("hero_socket_connect: %d, ip: 0x%x, port: %d, pSocketItem: %d", socket, ip, port, pSocketItem));
	if(pSocketItem != NULL && pSocketItem->realSocketId != -1)
	{
		int32 ret = hero_sconnect(socket, ip, port);
		HERO_LOG(("dsm. ret:%d", ret));
		if(ret < 0)
		{
			int error = sci_sock_errno(pSocketItem->realSocketId);
			MMIHR_Trace(("hero_socket_connect :%d", error));
			if(EINPROGRESS == error)
			{
				pSocketItem->cb = cb;
				pSocketItem->connectState = HERO_SOC_STATE_CONNECTING;				
				if(pSocketItem->timeroutId == 0)
				{
					pSocketItem->timeroutId = hero_create_timerId();
				}
				hero_start_timer(pSocketItem->timeroutId, 100, soc_connect_timerout_callback, (void*)socket);
				conRet = HERO_SOC_STATE_CONNECTING;
			}
		}
		else
		{
			conRet = HERO_SOC_STATE_CONNECTED;
		}
	}
	MMIHR_Trace(("dsm_connect() : %d,%d", conRet, pSocketItem));

	return conRet;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int hero_socket_recv_ex(int s, const char *buf, int len)
#else
int hero_socket_recv(int s, const char *buf, int len)
#endif
{
	int recv_len = 0;
	int ret = 0;
	heroSocketItemSt *pSocketItem = hero_app_net_index2socket(s);

	HERO_LOG(("hero_socket_recv pSocketItem:%d", pSocketItem));
	if(pSocketItem == NULL || pSocketItem->realSocketId == -1)
	{
		return HERO_SOC_ERROR;
	}

	recv_len = sci_sock_recv((long)pSocketItem->realSocketId, buf, len, 0);
	HERO_LOG(("hero_socket_recv  recv ret:%d", recv_len));
	if(recv_len > 0)
	{
		ret = recv_len;
	}
	else
	{
		int e = sci_sock_errno(pSocketItem->realSocketId);
		HERO_LOG(("hero_socket_recv. errorno:%d", e));
		if(e == ECONNRESET || e == EPIPE)//对端已关闭连接或服务器死机后重启
		{
			ret = HERO_SOC_ERROR;
		}
		else
		{
			ret = 0;
		}
	}
	return ret;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int hero_socket_send_ex(int s, const char *buf, int len)
#else
int hero_socket_send(int s, const char *buf, int len)
#endif
{
	int ret = 0;
	int send_len = 0;
	heroSocketItemSt *pSocketItem = hero_app_net_index2socket(s);

	HERO_LOG(("hero_socket_send pSocketItem:%d", pSocketItem));
	if(pSocketItem == NULL || pSocketItem->realSocketId == -1)
	{
		return HERO_SOC_ERROR;
	}

	send_len = sci_sock_send((long)pSocketItem->realSocketId, (char*)buf, len, 0);
	HERO_LOG(("hero_socket_send. send ret: %d", send_len));
	if(send_len > 0)
	{
		ret = send_len;
	}
	else
	{
		int e = sci_sock_errno((int32)pSocketItem->realSocketId);
		HERO_LOG(("hero_socket_send. error0: %d", e)); 
		if(e == EWOULDBLOCK)
		{
			ret = 0;
		}
		else
		{
			ret = HERO_SOC_ERROR;
		}
	}
	return ret;
}

static void rest_connect_gprs_status_callback(void* timerId)
{
	HERO_LOG(("rest_connect_gprs_status_callback %d,%d",gprs_is_connected,(int)timerId));
	hero_delete_timer((int)timerId);
	gprs_is_connected = FALSE;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
void hero_free_nwk_account_ex()
#else
void hero_free_nwk_account()
#endif
{
	ERR_MNGPRS_CODE_E err;

	HERO_LOG(("hero_free_nwk_account %d",account_init_flag));
	if(!account_init_flag)
	{
		return;
	}
	account_init_flag = FALSE;
	gInitNwkAccountCallback = NULL;
	dsm_nwk_account_id = 0;

	if(gDsmGetHostNameTimerId)
	{
		hero_delete_timer(gDsmGetHostNameTimerId);
		gDsmGetHostNameTimerId = 0;
	}
	hero_app_net_socket_close_all();

	err = MMIAPIPDP_Deactive(MMI_MODULE_HEROAPP);
	HERO_LOG(("hero_free_nwk_account err: %d", err));

	{
		int restGprsTimerId = hero_create_timerId();
		if(restGprsTimerId)
		{
			hero_start_timer(restGprsTimerId,15*1000,rest_connect_gprs_status_callback,(void*)restGprsTimerId);
		}
		else
		{
			gprs_is_connected = FALSE;
		}
	}
}

BOOL hero_gprs_is_connected()
{
	HERO_LOG(("hero_gprs_is_connected %d",gprs_is_connected));
	return gprs_is_connected;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
int hero_socket_close_ex(int s)
#else
int hero_socket_close(int s)
#endif
{
	int ret = 0;
	hero_app_net_socket_close(s);
	HERO_LOG(("hero_socket_close ret:%d ", ret));
	return ret;
}

/****************************************************
*					内存相关						*
*****************************************************/
#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
#define HERO_MEM_POOL_SIZE (40*1024)    // 内存池的大小必须大于15K
#else
#define HERO_MEM_POOL_SIZE (25*1024)    // 内存池的大小必须大于15K
#endif
#if !defined(WIN32)
LOCAL __align(32) char gHeroPlatMemPool[HERO_MEM_POOL_SIZE] = {0};
int hero_get_mem_pool(char** mem_base, unsigned int* mem_len)
{
	if(mem_base != NULL && mem_len != NULL)
	{
		*mem_base = gHeroPlatMemPool;
		*mem_len = HERO_MEM_POOL_SIZE;
		return 0;
	}
	return -1;
}
#endif

/****************************************************
*					短信相关						*
*****************************************************/
typedef struct tagSendSmsSt
{
	void *userData;
	send_sms_cb cb;
}sendSmsSt;
static sendSmsSt gSendSmsStData = {0};
LOCAL BOOLEAN hero_SMSCnf(BOOLEAN is_succss, DPARAM parm)
{
	BOOLEAN resValue = TRUE;

	HERO_LOG(("hero_SMSCnf is_succss: %d, cb=%d", is_succss,gSendSmsStData.cb));
	if(gSendSmsStData.cb)
	{
		gSendSmsStData.cb(TRUE == is_succss ? 1 : 0,gSendSmsStData.userData);
	}
	return resValue;
}

#if defined(WIN32)
int hero_wstrlen(const char *str)
{
	return 0;
}

int hero_str2wstr(char *utf8, short *unicode, int size)
{
	return 0;
}
#else
extern int hero_wstrlen(const char *str);
extern int hero_str2wstr(char *utf8, short *unicode, int size);
#endif
int hero_send_sms(char *pNumber,char *pContent,int contentLen,int smsType,int active_sim,send_sms_cb cb,void *userData,int port)
{
	int32 resValue = -1;
#if 0
	BOOLEAN result = FALSE;
	BOOLEAN repFlag = FALSE;
	uint8 numLen = 0;
	MMI_STRING_T msg = {0};
	MMISMS_SEND_DATA_T send_data_ptr = {0};
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
	MMISMS_ALPHABET_MANAGEMENT_E alpha_management = MMISMS_ALPHABET_MANAGEMENT_DEFAULT;
#endif

	MMIHR_Trace(("hero_send_sms pNumber: %s, len: %d, ucs2?: 0x%x", pNumber, strlen(pNumber), active_sim)); 	
	if(pNumber == NULL || pContent == NULL || contentLen <= 0 || cb == NULL || (numLen = strlen(pNumber)) == 0)
	{
		return resValue;
	}

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    alpha_management = MMISMS_GetAlphabetManagement();
#endif

	memset(&send_data_ptr, 0, sizeof(MMISMS_SEND_DATA_T));
	if(smsType == HERO_SMS_TYPE_7BIT)
	{
		msg.wstr_len = contentLen;
		msg.wstr_ptr = (wchar*)SCI_ALLOCA((msg.wstr_len + 1) * sizeof(wchar));
		if(msg.wstr_ptr == NULL)
		{
			return -1;
		}
		memset(msg.wstr_ptr, 0, (msg.wstr_len + 1) * sizeof(wchar));
		hero_str2wstr(pContent, msg.wstr_ptr, (msg.wstr_len + 1) * sizeof(wchar));
		send_data_ptr.sms_content_ptr = &msg;
	#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
		MMISMS_SetAlphabetManagement(MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET);
	#endif
	}
	else if(smsType == HERO_SMS_TYPE_UCS2)
	{
		msg.wstr_len = hero_wstrlen(pContent)/2;
		msg.wstr_ptr = (wchar*)SCI_ALLOCA((msg.wstr_len + 1) * sizeof(wchar));
		if(msg.wstr_ptr == NULL)
		{
			return -1;
		}
		memset(msg.wstr_ptr, 0, (msg.wstr_len + 1) * sizeof(wchar));
		MMIAPICOM_Wstrncpy(msg.wstr_ptr, (const wchar*)pContent, msg.wstr_len);
		send_data_ptr.sms_content_ptr = &msg;
	#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
		MMISMS_SetAlphabetManagement(MMISMS_ALPHABET_MANAGEMENT_UCS2);
	#endif
	}
	else if(smsType == HERO_SMS_TYPE_8BIT)
	{
		msg.wstr_len = contentLen;
		msg.wstr_ptr = (wchar*)SCI_ALLOCA(msg.wstr_len + 1);
		if(msg.wstr_ptr == NULL)
		{
			return -1;
		}
		memcpy((char*)msg.wstr_ptr, pContent, contentLen);
		send_data_ptr.binary_sms_data.is_binary = TRUE;
		send_data_ptr.binary_sms_data.binary_data = (char*)msg.wstr_ptr;
		send_data_ptr.binary_sms_data.binary_length = contentLen;
		send_data_ptr.sms_content_ptr = &msg;
	}
	else
	{
		return -1;
	}

	repFlag = MMISMS_GetStatusReportFlag();
	if(repFlag)
	{
		MMISMS_SetStatusReportFlag(FALSE);
	}

	HERO_LOG(("hero_send_sms msg.wstr_len: %d,activeSim:%d", msg.wstr_len,get_action_sim_index(active_sim)));
	if(0 != msg.wstr_len)
	{
		numLen = MIN(numLen, MMISMS_NUMBER_MAX_LEN);
		if(smsType == HERO_SMS_TYPE_8BIT)
		{
			send_data_ptr.is_need_packet = FALSE;
		}
		else
		{
			send_data_ptr.is_need_packet = TRUE;
		}
		send_data_ptr.dual_sys = get_action_sim_index(active_sim);
		send_data_ptr.src_port = 0;
		send_data_ptr.dst_port = port;
		send_data_ptr.dest_addr_ptr = (const uint8*)pNumber;
		send_data_ptr.dest_addr_len = numLen;
		send_data_ptr.send_callback = hero_SMSCnf;

		result = MMIAPISMS_SendSmsForOther(&send_data_ptr);
		if(result)
		{
			resValue = 0;
		}
		gSendSmsStData.cb = cb;
		gSendSmsStData.userData = userData;
		HERO_LOG(("hero_send_sms resValue: %d", resValue));
	}
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
	MMISMS_SetAlphabetManagement(alpha_management);
#endif
	if(NULL != msg.wstr_ptr)
	{
		SCI_FREE(msg.wstr_ptr);
	}
#endif
	return resValue;
}

//modify 20181117 begin
typedef struct tagHeroUssdReqData{
	int timerId;
	send_sms_cb cb;
	void *userData;
}heroUssdReqData;

static void hero_send_ussd_cb(void *p)
{
	heroUssdReqData *pReqData = (heroUssdReqData*)p;
	if(pReqData != NULL)
	{
		if(pReqData->cb != NULL)
		{
			pReqData->cb(1, pReqData->userData);
		}
		hero_delete_timer(pReqData->timerId);
		SCI_FREE(pReqData);
	}
}

int hero_send_ussd(char *pContent,int contentLen,int smsType,int active_sim,send_sms_cb cb,void *userData)
{
#if 0
	int ret = 0;
	char *str_ptr = NULL;
	do
	{
	    short str_len = 0;
		heroUssdReqData *pReqData = NULL;
		ERR_MNSS_CODE_E response_code = ERR_MNSS_NO_ERR;

		if(pContent == NULL || contentLen <= 0)
		{
			break;
		}
		str_ptr = SCI_ALLOCA((MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE + 1)*2);
		if(str_ptr == NULL)
		{
			break;
		}

		memset(str_ptr, 0, (MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE + 1)*2);
		if(smsType == HERO_SMS_TYPE_7BIT)
		{
			hero_str2wstr(pContent, (short*)str_ptr, (MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE + 1)*2);
		}
		else if(smsType == HERO_SMS_TYPE_UCS2)
		{
			MMIAPICOM_Wstrncpy(str_ptr, (const wchar*)pContent, MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE);
		}
		else
		{
			SCI_FREE(str_ptr);
			str_ptr = NULL;
			return -1;
		}

		str_len = hero_wstrlen(str_ptr)/2;
		if(str_len <= 0)
		{
			break;
		}

		if (MMIAPICOM_IsDefaultString(str_ptr, str_len))
		{
			uint8 temp_str[(CC_MAX_TELE_NUM_LEN << 1) + 1] = {0};

			str_len = MMIAPICOM_Wchar2default(str_ptr, temp_str, str_len);

			response_code = MNSS_ResponseUSSDEx(
					                MMIAPIUSSD_GetDualSys(),//dual sys later
					                MN_SS_DEFAULT_DCS_VALUE, 
					                (uint8)str_len, 
					                (uint8*)temp_str);
		}
		else
		{
		#if defined(__BIG_ENDIAN) || defined(__BigEndian)
			response_code = MNSS_ResponseUSSDEx(
									MMIAPIUSSD_GetDualSys(),//dual sys later
									MN_SS_USC2_NO_CLASS_DCS, 
									(uint8)str_len, 
									(uint8*)str_ptr);
		#else
			uint8 *temp_str = PNULL;

			temp_str = SCI_ALLOCA(str_len * sizeof(wchar) + 1);
			if (PNULL != temp_str)
			{
				SCI_MEMSET(temp_str, 0x00, (str_len * sizeof(wchar) + 1));

				GUI_UCS2B2UCS2L((uint8 *)temp_str,
						str_len * sizeof(wchar),
						(uint8 *)str_ptr,
						str_len * sizeof(wchar));

				response_code = MNSS_ResponseUSSDEx(
						MMIAPIUSSD_GetDualSys(),//dual sys later
						MN_SS_USC2_NO_CLASS_DCS, 
						(uint8)str_len, 
						(uint8*)temp_str);

				SCI_FREE(temp_str);
				temp_str = PNULL;
			}
			else
			{
				response_code = ERR_MNSS_INVALID_OPERATION;
			}
		#endif
		}

		if(response_code != ERR_MNSS_NO_ERR)
		{
			break;
		}

		pReqData = SCI_ALLOCA(sizeof(heroUssdReqData));
		if(pReqData == NULL)
		{
			break;
		}
		pReqData->timerId = hero_create_timerId();
		pReqData->cb = cb;
		pReqData->userData = userData;
		hero_start_timer(pReqData->timerId, 15000, hero_send_ussd_cb, (void*)pReqData);

		ret = 1;
	}while(0);
	if(str_ptr != NULL)
	{
		SCI_FREE(str_ptr);
	}
	return ret;
#else
	return -1;
#endif
}
//modify 20181117 end

static char hero_app_mmi_msg_new_sms_ind(unsigned char *pContent, int nLen, char *pNum, int smsCheckRet);

#if defined(WIN32)
int hero_check_sms(unsigned char *pContent, int nLen, char *pNum,unsigned short msgId)
{
	return 0;
}
#else
extern int hero_check_sms(unsigned char *pContent, int nLen, char *pNum,unsigned short msgId);
#endif
BOOL hero_mmi_msg_new_sms_ind(unsigned char *pContent, int nLen, char *pNum, int type)
{
#if 0
	int ret = 0;
	char *tempBuf = NULL;

	HERO_LOG(("hero_mmi_msg_new_sms_ind num=%s,ContentLen=%d,Content=%s",pNum,nLen,pContent));

	tempBuf = hero_engine_malloc((unsigned int)(nLen + 1)<<1);
	if(tempBuf == NULL)
	{
		HERO_LOG(("exception hero_mmi_msg_new_sms_ind hero_engine_malloc failed %d",nLen));
		return ret;
	}
	memset(tempBuf,0,(nLen + 1)<<1);
	if(type == HR_ENCODE_ASCII)
	{
		hero_str2wstr(pContent, (short*)tempBuf, (nLen + 1)<<1);
		nLen = hero_wstrlen((const char*)tempBuf);
	}
	else
	{
		memcpy(tempBuf,pContent,nLen);
		hr_str_convert_endian_with_size(tempBuf,nLen);
	}
	HERO_LOG(("dsm_mmi hr_str_convert_endian_with_size %s",tempBuf));

	ret = hero_check_sms(tempBuf,nLen,pNum,0);

	hero_app_mmi_msg_new_sms_ind(tempBuf, nLen, pNum, ret);

	hero_engine_free((char **)&tempBuf);
	return ret;
#endif
	return 0;
}
void hero_sms_remove(unsigned short msg_id)
{
}

//modify 20181117 begin
typedef void (*HeroAppFuncPtr) (void);
typedef struct tagHeroCheckUssdCBDataSt{
	int timerId;
	HeroAppFuncPtr cb;
}heroCheckUssdCBDataSt;

static void hero_check_ussd_release_callback(void *p)
{
	heroCheckUssdCBDataSt *userData = (heroCheckUssdCBDataSt*)p;
	if(userData != NULL)
	{
		if(userData->cb != NULL)
		{
			userData->cb();
		}
		hero_delete_timer(userData->timerId);
		hero_engine_free((char**)&userData);
	}
}

char hero_check_ussd(unsigned char *pContent, int nLen);
BOOL hero_app_platform_check_ussd(MMI_STRING_T *prompt_str_ptr, unsigned int msg_id, HeroAppFuncPtr releaseFunc)
{
#if 0
	BOOL ret = 0;

	HERO_LOG(("hero_app_platform_check_ussd prompt_str_ptr = %d, msg_id = %d, op_code = %d", prompt_str_ptr, msg_id, releaseFunc));
    if(prompt_str_ptr == NULL || prompt_str_ptr->wstr_len == 0)
    {
        return ret;
    }

	ret = (BOOL)hero_check_ussd((unsigned char*)prompt_str_ptr->wstr_ptr, prompt_str_ptr->wstr_len<<1);
	if(ret)
	{
		heroCheckUssdCBDataSt *userData = hero_engine_malloc(sizeof(heroCheckUssdCBDataSt));
		if(userData != NULL)
		{
			userData->cb = releaseFunc;
			userData->timerId = hero_create_timerId();
			hero_start_timer(userData->timerId, 15000, hero_check_ussd_release_callback, (void*)userData);
		}
	}

	HERO_LOG(("ret = %d", ret));
	return (BOOL)ret;
#else
	return 0;
#endif
}
//modify 20181117 end

/****************************************************
*					文件相关						*
*****************************************************/
#define HERO_FLASH_PHONE                (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK)
#define HERO_FLASH_PHONE_LEN            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)

#define HERO_FLASH_TCRAD               (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD)
#define HERO_FLASH_TCRAD_LEN           MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)

#define HERO_FLASH_SYS             (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM)
#define HERO_FLASH_SYS_LEN         MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM)

static uint16 shapp_file[4] = {0};
static uint8 gHeroFullPath[128] = {0};

#define DSM_ROOT_DIR_LEN  (6) 
void hero_init_file_drv()
{
	uint16 *pDeviceRoot = NULL;
	if(TRUE == MMIAPIFMM_GetDeviceStatus(HERO_FLASH_TCRAD, HERO_FLASH_TCRAD_LEN))
	{
		pDeviceRoot =  (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
	}
	else
	{
		pDeviceRoot =  (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
	}
	shapp_file[0] = pDeviceRoot[0];
	shapp_file[1] = ':';
	shapp_file[2] = '\\';
	shapp_file[3] = 0;
}

int hero_file_create_dir(const char *dir,char isHidden)
{
	int dirLen = 0;
	MMIFILE_ERROR_E createRet = SFS_ERROR_NONE;
	if(dir == NULL) 
	{
		return -1;
	}
	memset(gHeroFullPath, 0, sizeof(gHeroFullPath));
	memcpy(gHeroFullPath,(char*)shapp_file,DSM_ROOT_DIR_LEN);
	dirLen = GUI_GB2UCS(gHeroFullPath+DSM_ROOT_DIR_LEN,dir,strlen(dir));

	createRet = MMIFILE_CreateDirectory((const uint16*)gHeroFullPath) ;
	if(createRet== SFS_ERROR_NONE)
	{
		MMIAPIFMM_SetAttr(gHeroFullPath, dirLen+(DSM_ROOT_DIR_LEN>>1), isHidden, FALSE, FALSE, FALSE);
		return 0;
	}
	else if(createRet == SFS_ERROR_HAS_EXIST)
	{
		return 1;
	}
	return -1;
}

int hero_file_open(const char* filename, unsigned int mode)
{
	int fHandle;
	unsigned int new_mode = 0;

	if(filename == NULL) 
	{
		return 0;
	}

	if (mode & HERO_FILE_RDONLY) 
		new_mode |= (SFS_MODE_READ | SFS_MODE_OPEN_EXISTING);
	if (mode & HERO_FILE_WRONLY) 
		new_mode |= (SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING);
	if (mode & HERO_FILE_RDWR) 
		new_mode |= (SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING);
	if (mode & HERO_FILE_CREATE) 
	{
		new_mode &= ~SFS_MODE_OPEN_EXISTING;
		new_mode |= SFS_MODE_OPEN_ALWAYS;
	}
	if(mode & HERO_FILE_SHARD)
		new_mode  |= SFS_MODE_SHARE_READ | SFS_MODE_SHARE_WRITE;

	memset(gHeroFullPath, 0, sizeof(gHeroFullPath));
	memcpy(gHeroFullPath,(char*)shapp_file,DSM_ROOT_DIR_LEN);
	GUI_GB2UCS(gHeroFullPath+DSM_ROOT_DIR_LEN,filename,strlen(filename));

	return  (int)MMIFILE_CreateFile((const uint16 *)gHeroFullPath, new_mode, NULL, NULL);
}

int hero_file_write(int f,void *p,unsigned int l)
{
	uint32 transmit = 0;
	if(f != (int)FFS_INVALID_HANDLE && p != NULL && l != 0)
	{
		if(MMIFILE_WriteFile(f, p, l, &transmit, NULL) == SFS_ERROR_NONE)
			return (int32)transmit;
	}
	return -1;
}

int hero_file_read(int f,void *p,unsigned int l)
{
	uint32 transmit = 0;
	if(f != (int)FFS_INVALID_HANDLE && p != NULL && l != 0)
	{
		if(MMIFILE_ReadFile(f, p, l, &transmit, NULL) == SFS_ERROR_NONE)
		{
			return (int32)transmit;
		}
	}
	return -1;
}

int hero_file_seek(int f, int pos, int method)
{
	uint8 origin = 0;
	uint32 ret;
	if(f != (int)FFS_INVALID_HANDLE)
	{
		switch(method)
		{
		case HERO_SEEK_SET:
			origin = SFS_SEEK_BEGIN;
			break;
		case HERO_SEEK_CUR:
			origin = SFS_SEEK_CUR;
			break;
		case HERO_SEEK_END:
			origin = SFS_SEEK_END;
			break;
		default:
			return -1;
		}
		ret = MMIFILE_SetFilePointer((MMIFILE_HANDLE)f, pos, origin);
		if(ret == SFS_ERROR_NONE)
		{
			return 0;
		}
	}
	return -1;
}

int hero_file_close(int f)
{
	if(f != (int)FFS_INVALID_HANDLE)
	{
		if(MMIFILE_CloseFile((MMIFILE_HANDLE)f) == SFS_ERROR_NONE)
		{
			return 0;
		}
	}
	return -1;
}

int hero_get_file_size(const char* filename)
{
	uint16 hr_full_path_len = 0;
	uint16 ucs2_name_len = 0;
	uint32 file_size_ptr;
	SFS_DATE_T modify_date = {0};
	SFS_TIME_T modify_time = {0};

	memset(gHeroFullPath, 0, sizeof(gHeroFullPath));
	memcpy(gHeroFullPath,(char*)shapp_file,DSM_ROOT_DIR_LEN);
	GUI_GB2UCS(gHeroFullPath+DSM_ROOT_DIR_LEN,filename,strlen(filename));

	if(MMIFILE_GetFileInfo(gHeroFullPath, hr_full_path_len/2, &file_size_ptr, &modify_date, &modify_time)) 
	{
		return (int32)file_size_ptr;
	}

	return 0;
}

int hero_get_file_info(const char* filename)
{
	SFS_HANDLE sfs_handle = 0;
	SFS_FIND_DATA_T find_data = {0};

	memset(gHeroFullPath, 0, sizeof(gHeroFullPath));
	memcpy(gHeroFullPath,(char*)shapp_file,DSM_ROOT_DIR_LEN);
	GUI_GB2UCS(gHeroFullPath+DSM_ROOT_DIR_LEN,filename,strlen(filename));

	sfs_handle = SFS_FindFirstFile((const uint16 *)gHeroFullPath, &find_data);				
	if(SFS_INVALID_HANDLE != sfs_handle)
	{
		SFS_FindClose(sfs_handle);
		if(SFS_ATTR_DIR & find_data.attr)
		{
			return HR_IS_DIR;
		}
		else
		{
			return HR_IS_FILE;
		}
	}
	return HR_IS_INVALID;	
}

int hero_file_remove(const char* filename)
{
	memset(gHeroFullPath, 0, sizeof(gHeroFullPath));
	memcpy(gHeroFullPath,(char*)shapp_file,DSM_ROOT_DIR_LEN);
	GUI_GB2UCS(gHeroFullPath+DSM_ROOT_DIR_LEN,filename,strlen(filename));

	if(MMIFILE_DeleteFile((const uint16*)gHeroFullPath, NULL) == SFS_ERROR_NONE)
		return 0;

	return -1;
}

char hero_is_back_light_on()
{
	return MMIDEFAULT_GetBackLightState() ? 1 : 0;
}

void hero_get_date_time(hero_mytime *ptime)
{
	SCI_DATE_T  date = {0};
	SCI_TIME_T  time = {0};
	if(ptime != NULL)
	{
		if(TM_GetSysDate(&date) == ERR_TM_NONE
			&& TM_GetSysTime(&time) == ERR_TM_NONE)
		{
			ptime->nYear = date.year;
			ptime->nMonth = date.mon;
			ptime->nDay = date.mday;
			ptime->nHour = time.hour;
			ptime->nMin = time.min;
			ptime->nSec = time.sec;
			ptime->DayIndex = 0;
		}
	}
}

static const uint8 dsm_RTC_month_info[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint8 dsm_RTC_month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static void HERO_IncreaseDateByOne(SCI_DATE_T  *pdate_ptr)
{
    if (pdate_ptr->mon > MMICOM_DATE_MAX_MONTH)
    {
        pdate_ptr->mon = MMICOM_DATE_MAX_MONTH;
    }
    if(MMIAPICOM_IsLeapYear((pdate_ptr->year)))
	{
		if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
		{
			pdate_ptr->mday = 1;
			pdate_ptr->mon = 1;
			pdate_ptr->year++; 
		}
		else if(pdate_ptr->mday == dsm_RTC_month_info_leap[pdate_ptr->mon-1])
		{
			pdate_ptr->mday = 1;
			pdate_ptr->mon++;
		}
		else
		{
			pdate_ptr->mday++;
		}
	}
	else        // not leap year
	{
		if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
		{
			pdate_ptr->mday = 1;
			pdate_ptr->mon = 1;
			pdate_ptr->year++;
		}
		else if(pdate_ptr->mday == dsm_RTC_month_info[pdate_ptr->mon-1])
		{
			pdate_ptr->mday = 1;
			pdate_ptr->mon++;
		}
		else
		{
			pdate_ptr->mday++;
		}
	}

	if (pdate_ptr->year >MMICOM_DATE_MAX_YEAR)
	{
		pdate_ptr->year = MMICOM_DATE_MIN_YEAR;       // when 2099, return 1980
	}
}

#if 0
void MMIAPIIM_HeroWordRemind()
{
	HERO_LOG(("========MMIAPIIM_HeroWordRemind=======%d",MMIAPIPHONE_GetStartUpCondition()));
	if (STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition() && STARTUP_CHARGE != MMIAPIPHONE_GetStartUpCondition())
	{
		hero_evt_cb(HERO_REMINDER_NOTIFY_EXPIRY,0);
	}
}
#endif

int hero_event_set(const hero_mytime *expiry_time,unsigned int usr_data)
{
	int ret = 0;
#if 0
	SCI_TIME_T second_time = {0};
	SCI_DATE_T cur_date = {0};
	//SCI_TIME_T cur_time = {0};
	MMIACC_SMART_EVENT_T event = {0};

	HERO_LOG(("hero_event_set AAAA %d",expiry_time));
	if(expiry_time == NULL)
	{
		return 0;
	}

	//TM_GetSysTime(&cur_time);
	//HERO_LOG(("hero_event_set curdata %d-%d-%d %d:%d:%d",cur_date.year,cur_date.mon,cur_date.mday,cur_time.hour,cur_time.min,cur_time.sec));

	TM_GetSysDate(&cur_date);
	cur_date.year = expiry_time->nYear;
	cur_date.mon = expiry_time->nMonth;
	cur_date.mday = expiry_time->nDay;
	second_time.hour = expiry_time->nHour;
	second_time.min = expiry_time->nMin;
	second_time.sec = 0;

#if 0
	if (!(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time) == CMP_FIRST_SMALL))
	{
		HERO_LOG(("MMIAPIALM_CompareDateAndTime ======"));
		HERO_IncreaseDateByOne(&cur_date);
	}
	HERO_LOG(("newdata %d-%d-%d %d:%d:%d",cur_date.year,cur_date.mon,cur_date.mday,second_time.hour,second_time.min,second_time.sec));
#endif

	SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
	event.event_fast_info.is_valid = TRUE;
	event.event_fast_info.type = SMART_EVENT_HERO_APP;
	event.event_fast_info.is_on = TRUE;
	event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
	#if 0
		event.event_fast_info.start_year = event.event_fast_info.end_year = 0;
		event.event_fast_info.start_mon = event.event_fast_info.end_mon = 0;
		event.event_fast_info.start_day = event.event_fast_info.end_day = 0;
		event.event_fast_info.hour = second_time.hour;
		event.event_fast_info.minute = second_time.min;
		event.event_fast_info.year = cur_date.year;
		event.event_fast_info.mon = cur_date.mon;
		event.event_fast_info.day = cur_date.mday;
		event.event_content_info.ring_type = ALM_RING_TYPE_MAX;
	#else
		event.event_fast_info.start_year = cur_date.year;
		event.event_fast_info.start_mon = cur_date.mon;
		event.event_fast_info.start_day = cur_date.mday;
		event.event_fast_info.hour = second_time.hour;
		event.event_fast_info.minute = second_time.min;
		event.event_fast_info.year = cur_date.year;
		event.event_fast_info.mon = cur_date.mon;
		event.event_fast_info.day = cur_date.mday;
		event.event_fast_info.end_year = cur_date.year+1;
		event.event_fast_info.end_mon = cur_date.mon;
		event.event_fast_info.end_day = cur_date.mday;
		event.event_content_info.ring_type = ALM_RING_TYPE_MAX;
	#endif

	ret = MMIAPIALM_SaveInteEventInfo(EVENT_HERO_APP, &event) ? 1 : 0;

	HERO_LOG(("hero_event_set ret = %d",ret));
#endif
	return ret;
}

int hero_event_cance(unsigned int usr_data)
{
#if 0
	MMIAPIALM_RestoreFactorySetting();
#endif
	HERO_LOG(("====hero_event_cance == "));
	return 1;
}

void hero_reg_event() 
{
	HERO_LOG(("hero_reg_event =========="));
}

void hero_sand(unsigned int seed)
{
	srand(seed);
}

unsigned int hero_rand(void)
{
	return rand();
}

void hero_send_sms_event(int ret, int retData)
{
	if(s_hr_allReady)
	{
		hr_event(HR_SEND_SMS_EVENT, ret, (int32)retData);
	}
}

void hero_send_heart_beat(int code)
{
	if (s_hr_allReady)
	{
		hr_event(HR_SEND_HEART_BEAT_EVENT, code, 0);
	}
}

typedef struct tagHeroAppCellInfoSt{
    unsigned int cellid;
    unsigned int areacode;
}heroAppCellInfoSt;

static heroAppCellInfoSt sheroAppCellInfoData = {0};

static void hero_init_cell_info()
{
	MN_PHONE_CURRENT_PLMN_INFO_T info = MNPHONE_GetCurrentPLMNInfoEx(get_action_sim_index(hero_get_action_sim()));
	sheroAppCellInfoData.cellid = info.cell_id;
	sheroAppCellInfoData.areacode = info.lac;
}

static int hero_app_isalnum(int ch)
{
	return (unsigned int)((ch | 0x20) - 'a') < 26u  || (unsigned int)( ch - '0') < 10u;
}

#define HERO_APP_UTF8_COMPUTE(Char, Mask, Len)					      \
	if (Char < 128)							      \
	{									      \
		Len = 1;								      \
		Mask = 0x7f;							      \
	}									      \
	else if ((Char & 0xe0) == 0xc0)					      \
	{									      \
		Len = 2;								      \
		Mask = 0x1f;							      \
	}									      \
	else if ((Char & 0xf0) == 0xe0)					      \
	{									      \
		Len = 3;								      \
		Mask = 0x0f;							      \
	}									      \
	else if ((Char & 0xf8) == 0xf0)					      \
	{									      \
		Len = 4;								      \
		Mask = 0x07;							      \
	}									      \
	else if ((Char & 0xfc) == 0xf8)					      \
	{									      \
		Len = 5;								      \
		Mask = 0x03;							      \
	}									      \
	else if ((Char & 0xfe) == 0xfc)					      \
	{									      \
		Len = 6;								      \
		Mask = 0x01;							      \
	}									      \
	else									      \
		Len = -1;


#define HERO_APP_UTF8_GET(Result, Chars, Count, Mask, Len)			      \
	(Result) = (Chars)[0] & (Mask);					      \
	for ((Count) = 1; (Count) < (Len); ++(Count))				      \
	{									      \
		if (((Chars)[(Count)] & 0xc0) != 0x80)				      \
		{								      \
			(Result) = -1;						      \
			break;							      \
		}								      \
		(Result) <<= 6;							      \
		(Result) |= ((Chars)[(Count)] & 0x3f);				      \
	}

static const char hero_app_utf8_skip_data[256] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

#define hero_app_utf8_next_char(p) (char *)((p) + hero_app_utf8_skip_data[*(const uint8 *)(p)])

static unsigned int hero_app_utf8_get_char(const char *p)
{
	int i, mask = 0, len;
	unsigned int result;
	unsigned char c = (unsigned char) *p;

	HERO_APP_UTF8_COMPUTE (c, mask, len);
	if (len == -1)
		return (unsigned int)-1;
	HERO_APP_UTF8_GET (result, p, i, mask, len);

	return result;
}

static unsigned int hero_app_unichar_to_utf8(unsigned int c, char *outbuf)
{
	/* If this gets modified, also update the copy in g_string_insert_unichar() */
	int i;
	int first;
	unsigned int len = 0;

	if (c < 0x80)
	{
		first = 0;
		len = 1;
	}
	else if (c < 0x800)
	{
		first = 0xc0;
		len = 2;
	}
	else if (c < 0x10000)
	{
		first = 0xe0;
		len = 3;
	}
	else if (c < 0x200000)
	{
		first = 0xf0;
		len = 4;
	}
	else if (c < 0x4000000)
	{
		first = 0xf8;
		len = 5;
	}
	else
	{
		first = 0xfc;
		len = 6;
	}

	if (outbuf)
	{
		for (i = len - 1; i > 0; --i)
		{
			outbuf[i] = (c & 0x3f) | 0x80;
			c >>= 6;
		}
		outbuf[0] = c | first;
	}
	return len;
}

const char *hero_app_url_encode(const char *str, char *outbuf, unsigned int outbufsize)
{
	char utf_char[6];
	const char *iter;
	unsigned int i, j = 0;

	iter = str;
	for (; *iter && j < (outbufsize - 1) ; iter = hero_app_utf8_next_char(iter)) {
		unsigned int c = hero_app_utf8_get_char(iter);
		/* If the character is an ASCII character and is alphanumeric * no need to escape */
		if (c < 128 && (hero_app_isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~' || c == '*')) {
			outbuf[j++] = c;
		} else {
			unsigned int bytes = hero_app_unichar_to_utf8(c, utf_char);
			for (i = 0; i < bytes; i++) {
				if (j > (outbufsize - 4)) {
					break;
				}
				sprintf(outbuf + j, "%%%02X", utf_char[i] & 0xff);
				j += 3;
			}
		}
	}
	outbuf[j] = '\0';

	return outbuf;
}

enum{
	HERO_DSM_APP_SMS_SUPPORT_7BIT = 0x1,
	HERO_DSM_APP_SMS_SUPPORT_8BIT = 0x2,
	HERO_DSM_APP_SMS_SUPPORT_UCS2 = 0x4,
};

int hero_get_req_ext_param_count()
{
	return 9;
}

int hero_get_req_ext_param(int index, char *key, int keyLen, char *value, int valueLen)
{
	switch(index)
	{
		case 0:
		{
			strncpy(key, "cellid", keyLen);
			sprintf(value, "%d", sheroAppCellInfoData.cellid);
			return 1;
		}
		break;
		case 1:
		{
			strncpy(key, "areacode", keyLen);
			sprintf(value, "%d", sheroAppCellInfoData.areacode);
			return 1;
		}
		break;
		case 2:
		{
			strncpy(key, "plat", keyLen);
			strncpy(value, "spr", valueLen);
			return 1;
		}
		break;
		case 3:
		{
			strncpy(key, "sms", keyLen);
			sprintf(value, "%d", HERO_DSM_APP_SMS_SUPPORT_7BIT | HERO_DSM_APP_SMS_SUPPORT_8BIT | HERO_DSM_APP_SMS_SUPPORT_UCS2);
			return 1;
		}
		break;
		case 4:
		{
			char extType[64] = {0};
			hr_platEx(9510, extType, sizeof(extType), NULL,NULL,NULL);
			strncpy(key, "exttype", keyLen);
			hero_app_url_encode(extType, value, valueLen);
			return 1;
		}
		break;
		case 5:
		{
			char applist[128] = {0};
			hero_get_install_app_list(applist, sizeof(applist));
			strncpy(key, "apps", keyLen);
			hero_app_url_encode(applist, value, valueLen);
			return 1;
		}
		break;
		case 6:
		{
			int i = 0;
			int pos = 0;
			uint16 maxNum = 0;
			uint16 usedNum = 0;
			uint16 smsUsedNum = 0;
			char smsMemStatus[128] = {0};

			strncpy(key, "smscapacity", keyLen);
			for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
			{
				if (MMIAPIPHONE_IsSimAvailable(i))
				{
					MMISMS_GetSMSUsedAndMax(i, FALSE, &maxNum, &usedNum, &smsUsedNum);
					pos += sprintf(smsMemStatus + pos, "SIM%d(%d-%d)", i + 1, usedNum, maxNum);
				}
			}
			MMISMS_GetSMSUsedAndMax(MN_DUAL_SYS_1, TRUE, &maxNum, &usedNum, &smsUsedNum);
			sprintf(smsMemStatus + pos, "PHONE(%d-%d)", usedNum, maxNum);

			hero_app_url_encode(smsMemStatus, value, valueLen);
			return 1;
		}
		break;
		case 7:
		{
			strncpy(key, "sysbrowser", keyLen);
		#ifdef BROWSER_SUPPORT
			strncpy(value, "1", valueLen);
		#else
			strncpy(value, "0", valueLen);
		#endif
			return 1;
		}
		break;
		case 8:
		{
			strncpy(key, "spcall", keyLen);
			strncpy(value, hr_plat(HR_PLAT_CALL_INTERFACE_SUPPORT, 0) == HR_SUCCESS ? "1" : "0", valueLen);
			return 1;
		}
		break;
	}
	return 0;
}

static int32 hr_test_ext_param_size()
{
	int memsize = 0;
	int index = 0;
	char keyBuf[64] = {0};
	char valueBuf[128] = {0};
	int count = hero_get_req_ext_param_count();

	memsize += 32;
	for(index = 0; index < count; index++)
	{
		if(hero_get_req_ext_param(index, keyBuf, sizeof(keyBuf), valueBuf, sizeof(valueBuf)))
		{
			memsize += strlen(keyBuf) + 1 + strlen(valueBuf) + 1;
		}
	}
	memsize += 8;
	return memsize;
}

static void hr_free_ext_param_buf(uint8* output, int32 output_len)
{
	if(output != NULL)
	{
		SCI_FREE(output);
	}
}

int32 hr_get_ext_param_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	do
	{
		int index = 0;
		int pos = 0;
		int extParamSize = 0;
		char *extParamBuf = NULL;
		char keyBuf[64] = {0};
		char valueBuf[128] = {0};
		int count = hero_get_req_ext_param_count();

		if(output == NULL || output_len == NULL || cb == NULL)
		{
			break;
		}

		///
		extParamSize = hr_test_ext_param_size();
		extParamBuf = SCI_ALLOC(extParamSize);
		if(extParamBuf == NULL)
		{
			break;
		}
		memset(extParamBuf, 0, extParamSize);

		for(index = 0; index < count; index++)
		{
			if(hero_get_req_ext_param(index, keyBuf, sizeof(keyBuf), valueBuf, sizeof(valueBuf)))
			{
				pos += sprintf(extParamBuf + pos, "%s=%s&", keyBuf, valueBuf);
			}
		}

		*output = extParamBuf;
		*output_len = pos;
		*cb = hr_free_ext_param_buf;

		ret = HR_SUCCESS;
	}while(0);
	return ret;
}

//modify 20181117 begin
int hero_sprintf(char * s,const char * format, ...)
{
	int ret;
	va_list ap;
	va_start(ap, format);
	ret = vsprintf(s, format, ap);
	va_end(ap);
	return ret;
}

int hero_app_get_vm_plat_code()
{
#ifdef HERO_ENGINE_SUPPORT
	return hr_platEx(8000, NULL, 0, NULL, 0, NULL);
#else
	return HR_PLAT_VALUE_BASE + 3;
#endif
}

int hero_app_get_hsver()
{
#ifdef HERO_ENGINE_SUPPORT
	hr_userinfo userinfo = {0};
	hr_getUserInfo(&userinfo);
	return userinfo.ver;
#else
	return (116%1000)*1000000 + (9%100)*10000 + (14%10000);
#endif
}

int hero_app_read_nv_data(char *buff, int buffLen)
{
    MN_RETURN_RESULT_E return_value = 0;
	char tempBuf[NVRAM_HERO_APP_CACHE_SIZE] = {0};

	HERO_LOG(("hero_app_read_nv_data buf = %d, bufflen = %d", (int)buff, (int)buffLen));
	if(buff == NULL || buffLen <= 0)
	{
		return 0;
	}
    MMINV_READ(NVRAM_EF_HERO_APP_CONFIG_LID, tempBuf, return_value);
	HERO_LOG(("hero_app_read_nv_data return_value = %d", (int)return_value));
	if(return_value != MN_RETURN_SUCCESS)
	{
		return 0;
	}
	memcpy(buff, tempBuf, MIN(NVRAM_HERO_APP_CACHE_SIZE, buffLen));

	return 1;
}

int hero_app_write_nv_data(char *buff, int buffLen)
{
	char tempBuf[NVRAM_HERO_APP_CACHE_SIZE] = {0};

	if(buff == NULL || buffLen <= 0)
	{
		return 0;
	}
	memcpy(tempBuf, buff, MIN(NVRAM_HERO_APP_CACHE_SIZE, buffLen));
	MMINV_WRITE(NVRAM_EF_HERO_APP_CONFIG_LID, tempBuf);
	return 1;
}

int hero_app_get_plat_status()
{
	return 1;
}

static void hero_app_disk_space_conver(uint32 used_low, uint32 used_high, uint32 free_low, uint32 free_high, unsigned int *totalSize, unsigned int *freeSize)
{
	uint64 total_space = 0;
	uint64 free_space  = 0;
	uint32 total_space_low	= 0;
	uint32 total_space_high = 0;

	/*free space*/
	if(free_high || (uint32)free_low > (uint32)0x7fffffff)
	{
		free_space = (uint64)((free_high<<22) + (free_low>>10))*1024;
	}
	else
	{
		free_space = free_low;
	}

	/*total space*/
	total_space_low  = used_low  + free_low;  
	total_space_high = used_high + free_high;
	if ((total_space_low < used_low) || (total_space_low < free_low))
	{
		total_space_high += 1;
	}
	if(total_space_high || (uint32)total_space_low > (uint32)0x7fffffff)
	{
		total_space = (uint64)((total_space_high<<22) + (total_space_low>>10))*1024;
	}
	else
	{
		total_space = total_space_low;
	}
	
	if(totalSize != NULL)
	{
		*totalSize = (unsigned int)(total_space>>10);
		HERO_LOG(("totalSize = %d", *totalSize));
	}

	if(freeSize != NULL)
	{
		*freeSize = (unsigned int)(free_space>>10);
		HERO_LOG(("freeSize = %d", *freeSize));
	}
}

//返回 KB
int hero_get_tcard_space(unsigned int *totalSize, unsigned int *freeSize)
{
	int ret = 0;
	do
	{
		uint32 used_low  = 0; 
		uint32 used_high = 0;
		uint32 free_low  = 0;
		uint32 free_high = 0;

		if(TRUE != MMIAPIFMM_GetDeviceStatus(HERO_FLASH_TCRAD, HERO_FLASH_TCRAD_LEN))
		{
			break;
		}
		MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_FLASH_TCRAD, HERO_FLASH_TCRAD_LEN, &used_high, &used_low);
		MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_FLASH_TCRAD, HERO_FLASH_TCRAD_LEN, &free_high, &free_low);

		hero_app_disk_space_conver(used_low, used_high, free_low, free_high, totalSize, freeSize);
		ret = 1;
	}while(0);
	return ret;
}

//返回 KB
int hero_get_phone_space(unsigned int *totalSize, unsigned int *freeSize)
{
	uint32 used_low  = 0; 
	uint32 used_high = 0;
	uint32 free_low  = 0;
	uint32 free_high = 0;

	if(TRUE != MMIAPIFMM_GetDeviceStatus(HERO_FLASH_PHONE, HERO_FLASH_PHONE_LEN))
	{
		return 0;
	}
	MMIFILE_GetDeviceUsedSpace((uint8 *)HERO_FLASH_PHONE, HERO_FLASH_PHONE_LEN, &used_high, &used_low);
	MMIFILE_GetDeviceFreeSpace((uint8 *)HERO_FLASH_PHONE, HERO_FLASH_PHONE_LEN, &free_high, &free_low);

	hero_app_disk_space_conver(used_low, used_high, free_low, free_high, totalSize, freeSize);

	return 1;
}

int hero_app_get_default_menu_swtich()
{
	return 0;
}

int32 hr_start_dsm_app_req_x(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	if(input == NULL || input_len != sizeof(smsInputData))
	{
		return HR_FAILED;
	}
	hero_start_dsm_app_req((smsInputData*)input);
	return HR_SUCCESS;
}

int32 hr_get_dsm_app_version_x(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	return hero_get_dsm_app_version();
}

int32 hero_start_hero_app_req_ex(uint8 * input, int32 input_len, uint8 ** output, int32 * output_len, HR_PLAT_EX_CB * cb)
{
	appInputData *inputData = (appInputData*)input;
	if(input == NULL || input_len != sizeof(appInputData))
	{
		return HR_FAILED;
	}
	hero_start_app_req(inputData);
	return HR_SUCCESS;
}

void hero_app_init_file_drv()
{
	hero_init_file_drv();
}

int hero_app_file_create_dir(const char *dir, char isHidden)
{
	return hero_file_create_dir(dir, isHidden);
}

int hero_app_file_write(int f,void *p,unsigned int l)
{
	return hero_file_write(f, p, l);
}

int hero_app_file_read(int f,void *p,unsigned int l)
{
	return hero_file_read(f, p, l);
}

int hero_app_file_seek(int f, int pos, int method)
{
	return hero_file_seek(f, pos, method);
}

int hero_app_file_open(const char* filename, unsigned int mode)
{
	return hero_file_open(filename, mode);
}

int hero_app_file_close(int f)
{
	return hero_file_close(f);
}

int hero_app_get_file_info(const char* filename)
{
	return hero_get_file_info(filename);
}

void hero_app_get_date_time(hero_mytime *ptime)
{
	hero_get_date_time(ptime);
}

typedef struct tagHeroSmsInputDataSt{
	char *number;
	char *content;
	int contentLen;
	int smsType;
	int active_sim;
}heroSmsInputDataSt;

static void hero_app_send_sms_req_callback(int ret,void *userData)
{
	HERO_LOG(("hero_app_send_sms_req_callback"));
	if (MMIHEROAPP_GetVMState() != VM_STOP)
	{
		hr_event(HERO_APP_EVENT_SMS_RET, ret, 0);
	}
}

int32 hero_app_send_sms_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	do
	{
		char *pNumber = NULL;
		char *pContent = NULL;
		int contentLen = 0;
		int smsType = 0;
		int active_sim = 0;
		heroSmsInputDataSt *smsInputData = (heroSmsInputDataSt*)input;

		HERO_LOG(("rrrrrrrr hero_app_send_sms_req %d", (int)input));
		if(input == NULL ||input_len != sizeof(heroSmsInputDataSt))
		{
			break;
		}
		if(smsInputData->number == NULL || smsInputData->content == NULL ||
			smsInputData->smsType < HERO_SMS_TYPE_7BIT || smsInputData->smsType >= HERO_SMS_TYPE_MAX)
		{
			break;
		}
		pNumber = smsInputData->number;
		pContent = smsInputData->content;
		contentLen = smsInputData->contentLen;
		smsType = smsInputData->smsType;
		active_sim = smsInputData->active_sim;
		HERO_LOG(("pNumber = %s,contentLen=%d,smsType=%d,active_sim=%d,pContent=%s", pNumber, contentLen, smsType, active_sim, pContent));
		ret = hero_send_sms(pNumber, pContent, contentLen, smsType, active_sim, hero_app_send_sms_req_callback, 0, 0);
	}while(0);
	return ret;
}

int32 hero_app_get_available_sim_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	hero_init_sim_card_info(NULL);
	return HR_PLAT_VALUE_BASE + hero_get_action_sim();
}

int32 hero_app_add_key_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	return hero_app_add_key((void*)input, input_len) ? HR_SUCCESS : HR_FAILED;
}

int32 hero_app_get_version_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	return hero_app_get_version();
}

int32 hero_app_set_cycle_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	return hero_app_set_cycle(input, input_len) ? HR_SUCCESS : HR_FAILED;
}

int32 hero_app_set_start_parameter_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	return hero_app_set_start_parameter(input, input_len) ? HR_SUCCESS : HR_FAILED;
}

enum{
	HERO_APP_MSG_TYPE_UTF8,
	HERO_APP_MSG_TYPE_UCS2
};
typedef struct tagHeroAppNewMsgInfoSt{
	char *pNumber;
	char *pContent;
	int contentLen;
	int contentType;
	int smsCheckRet;
}heroAppNewMsgInfoSt;

static char hero_app_mmi_msg_new_sms_ind(unsigned char *pContent, int nLen, char *pNum, int smsCheckRet)
{
	if (MMIHEROAPP_GetVMState() != VM_STOP)
	{
		heroAppNewMsgInfoSt msgInfoData = {0};

		msgInfoData.contentLen = nLen;
		msgInfoData.pContent = (char*)pContent;
		msgInfoData.contentType = HERO_APP_MSG_TYPE_UCS2;
		msgInfoData.pNumber = (char*)pNum;
		msgInfoData.smsCheckRet = smsCheckRet;
		hr_event(HERO_APP_EVENT_NEW_MSG, (int32)&msgInfoData, sizeof(heroAppNewMsgInfoSt));
	}
	return (char)hero_app_check_sms(pContent,(int)nLen, (char*)pNum);
}

void hero_app_send_heart_beat_event()
{
	if (MMIHEROAPP_GetVMState() != VM_STOP)
	{
		hr_event(HERO_APP_EVENT_HEART_BEAT, 0, 0);
	}
}

//modify 20181129

enum {
	HERO_TOOLBAR_OK_CANCEL,
	HERO_TOOLBAR_CANCEL_OK,
	HERO_TOOLBAR_OK,
	HERO_TOOLBAR_CANCEL
};

typedef struct tagHeroAppTextViewSt{
	void *userData;
	int toolbarType;
	hero_app_text_view_callback callback;
	unsigned short text[512];
}heroAppTextViewSt;

static heroAppTextViewSt gHeroAppTextView = {0};

LOCAL void hero_app_refresh_view();

LOCAL MMI_RESULT_E HandleHeroAppTipsWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E  msg_id,DPARAM param);

WINDOW_TABLE(HERO_APP_TIPS_WIN_TAB) = 
{
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    WIN_FUNC( (uint32)HandleHeroAppTipsWinMsg),
    WIN_ID( HERO_APP_TIPS_WIN_ID),
	WIN_TITLE(TXT_HERO_APP_TIPS),

	CREATE_TEXT_CTRL(HERO_APP_TIPS_TEXT_CTRL_ID),

    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL MMI_RESULT_E HandleHeroAppTipsWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E  msg_id,DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

	HERO_LOG(("HandleHeroAppTipsWinMsg msg_id = %d", msg_id));
	switch(msg_id)
	{		
	case MSG_OPEN_WINDOW:
		{
			MMI_STRING_T strInfo = {0};

			strInfo.wstr_ptr = (wchar*)gHeroAppTextView.text;
			strInfo.wstr_len = MMIAPICOM_Wstrlen((const wchar *)gHeroAppTextView.text);
			GUITEXT_SetString(HERO_APP_TIPS_TEXT_CTRL_ID, strInfo.wstr_ptr,strInfo.wstr_len, FALSE);
		}
		break;
	case MSG_CLOSE_WINDOW:
		if(gHeroAppTextView.callback != NULL)
		{
			gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_EXIT, gHeroAppTextView.userData);
		}
		memset(&gHeroAppTextView, 0, sizeof(heroAppTextViewSt));
		break;
	case MSG_CTL_PENOK:
		break;
	case MSG_TIMER:
		break;
	case MSG_LOSE_FOCUS:
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_KEYPRESSUP_RED:
	case MSG_KEYDOWN_RED:
	case MSG_KEYUP_RED:
	case MSG_KEYUP_POWER:
		HERO_LOG(("HandleHeroAppTipsWinMsg MSG_KEYUP_RED..."));
		recode = MMI_RESULT_TRUE;
		break;
	case MSG_CTL_OK:
	case MSG_APP_OK:
		if(gHeroAppTextView.callback != NULL)
		{
			if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_CANCEL_OK)
			{
				gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_CANCEL, gHeroAppTextView.userData);
			}
			else if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_OK_CANCEL || gHeroAppTextView.toolbarType == HERO_TOOLBAR_OK)
			{
				gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_OK, gHeroAppTextView.userData);
			}
		}
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		if(gHeroAppTextView.callback != NULL)
		{
			if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_CANCEL_OK)
			{
				gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_OK, gHeroAppTextView.userData);
			}
			else if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_OK_CANCEL || gHeroAppTextView.toolbarType == HERO_TOOLBAR_CANCEL)
			{
				gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_CANCEL, gHeroAppTextView.userData);
			}
		}

		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}

void hero_app_update_text_view(int handle, unsigned short *text, void *userData)
{
#if 0
	MMI_STRING_T strInfo = {0};

	if(!MMK_IsOpenWin(HERO_APP_TIPS_WIN_ID) || (handle != (int)&gHeroAppTextView))
    {
		return;
	}

	if(gHeroAppTextView.callback != NULL)
	{
		gHeroAppTextView.callback(HERO_APP_TEXT_VIEW_UPDATE, gHeroAppTextView.userData);
	}

	gHeroAppTextView.userData = userData;
	MMIAPICOM_Wstrncpy((wchar*)gHeroAppTextView.text, (const wchar *)text, sizeof(gHeroAppTextView.text)/sizeof(gHeroAppTextView.text[0]));

	strInfo.wstr_ptr = (wchar*)gHeroAppTextView.text;
	strInfo.wstr_len = MMIAPICOM_Wstrlen((const wchar *)gHeroAppTextView.text);
	GUITEXT_SetString(HERO_APP_TIPS_TEXT_CTRL_ID, strInfo.wstr_ptr,strInfo.wstr_len, TRUE);

	hero_app_refresh_view();
#endif
}

void hero_app_close_text_view(int handle)
{
#if 0
	if(!MMK_IsOpenWin(HERO_APP_TIPS_WIN_ID))
    {
		return;
	}
	MMK_CloseWin(HERO_APP_TIPS_WIN_ID);
	memset(&gHeroAppTextView, 0, sizeof(heroAppTextViewSt));
#endif
}

static void hero_app_refresh_view()
{
	int32 win_id = HERO_APP_TIPS_WIN_ID;
	MMI_TEXT_ID_T	left_sk_id = STXT_OK;
	MMI_TEXT_ID_T	right_sk_id = STXT_RETURN;

	HERO_LOG(("mrapp. hero_app_refresh_view gHeroAppTextView.toolbarType = %d", gHeroAppTextView.toolbarType));
	if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_CANCEL_OK)
	{
		left_sk_id = STXT_RETURN;
		right_sk_id = STXT_OK;
	}
	else if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_OK_CANCEL)
	{
		left_sk_id = STXT_OK;
		right_sk_id = STXT_RETURN;
	}
	else if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_OK)
	{
		left_sk_id = STXT_OK;
		right_sk_id = TXT_NULL;
	}
	else if(gHeroAppTextView.toolbarType == HERO_TOOLBAR_CANCEL)
	{
		left_sk_id = TXT_NULL;
		right_sk_id = STXT_RETURN;
	}
	
	if(MMK_IsOpenWin(win_id))
	{
		HERO_LOG(("hero_app_refresh_view update softkey text"));
		GUIWIN_SetSoftkeyTextId(win_id, left_sk_id, TXT_NULL, right_sk_id, FALSE);
	}
}

int hero_app_can_show_tip()
{
	//int ret1 = (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId());
	//int ret2 = (MMIKL_CLOCK_DISP_WIN_ID == MMK_GetFocusWinId());
	//int ret3 = !hero_is_back_light_on();
	//HERO_LOG(("hero_app_can_show_tip ret1 = %d, ret2 = %d, ret3 = %d", ret1, ret2, ret3));
	return 0; //ret3; //((ret1 || ret2) && ret3);
}

int hero_app_open_text_view(int toolbarType, unsigned short *text, hero_app_text_view_callback callback, void *userData)
{
#if 0
//    if (!MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
//    {
//		return 0;
//	}

	HERO_LOG(("mrapp. hero_app_open_text_view toolbarType = %d", toolbarType));
	gHeroAppTextView.toolbarType = toolbarType;
	gHeroAppTextView.callback = callback;
	gHeroAppTextView.userData = userData;
	MMIAPICOM_Wstrncpy((wchar*)gHeroAppTextView.text, (const wchar *)text, sizeof(gHeroAppTextView.text)/sizeof(gHeroAppTextView.text[0]));

    if(!MMK_IsOpenWin(HERO_APP_TIPS_WIN_ID))
    {
		if(MMK_CreateWin((uint32*)HERO_APP_TIPS_WIN_TAB, NULL) != 0)
		{
			hero_app_refresh_view();
			return (int)&gHeroAppTextView;
		}
    }
	return 0;
#endif
	return 0;
}
//modify 20181129

int hero_app_notify_start()
{
	int ret = 0;
	do
	{
		HERO_LOG(("====hero_app_notify_start=============="));
		if(MMIHEROAPP_GetVMState() != VM_STOP)
		{
			HERO_LOG((" hero_app_notify_start 1111111"));
			break;
		}
		if(MMIAPIUDISK_UdiskIsRun())
		{
			HERO_LOG((" hero_app_notify_start 333333"));
			break;
		}
		if(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
		{
			HERO_LOG((" hero_app_notify_start 22222"));
			break;
		}
//		if(MMIDC_GetDCModuleRunningFlag()) // camera
//		{
//			HERO_LOG((" MMIDC_GetDCModuleRunningFlag"));
//			break;
//		}
	#ifdef BROWSER_SUPPORT_DORADO
		if(MMIDORADO_IsRunning())
		{
			HERO_LOG((" MMIDORADO_IsRunning"));
			break;
		}
	#endif
		if(MMIAPIMP3_IsPlayerPlaying())
		{
			HERO_LOG((" hero_app_notify_start 444444444"));
			break;
		}
	#ifdef FM_SUPPORT
		if(MMIAPIFM_GetStatus() == FM_PLAYING)//后台播放FM
		{
			HERO_LOG((" hero_app_notify_start 55555555555"));
			break;
		}
	#endif

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
		ret = MMIHEROAPP_AppEntry(HR_APPID_FAMILY_BGRUN) ? 1 : 0;
#endif
	}while(0);
	HERO_LOG((" hero_app_notify_start ret = %d", ret));
	return ret;
}

#if defined(BROWSER_SUPPORT)
int hero_set_homepage(char *homepage)
{
	char *urlBuf = NULL;
	int ret = 0;
	do
	{
		int homepageLen = 0;
		wchar *wstrBuf = NULL;
	    MMI_STRING_T url_str = {0};

		if(homepage == NULL)
		{
			break;
		}
		homepageLen = strlen(homepage);
		if(homepageLen <= 0 || homepageLen >= 256)
		{
			return 0;
		}
		urlBuf = hero_engine_malloc(512);
		if(urlBuf == NULL)
		{
			break;
		}
		wstrBuf = (wchar*)urlBuf;

		MMIAPICOM_StrToWstr(homepage,wstrBuf);
		url_str.wstr_ptr = wstrBuf;
		url_str.wstr_len = MMIAPICOM_Wstrlen(url_str.wstr_ptr);

		ret = MMIBROWSER_SetHomepageUrl(&url_str) ? 1 : 0;
	}while(0);
	if(urlBuf != NULL)
	{
		hero_engine_free((char**)&urlBuf);
	}

	return ret;
}
#else
int hero_set_homepage(char * homepage)
{
	return 0;
}
#endif

void hero_send_http_cb_event(int data, int dataSize)
{
	if (MMIHEROAPP_GetVMState() == VM_RUN)
	{
		hr_event(HERO_APP_EVENT_HTTP_RSP, data, dataSize);
	}
}

#endif

#define HERO_LOACL_INTERVAL_TIME 300  //秒为单位
int32 hero_app_get_local_interval_time()
{
#ifdef __HR_CFG_ENTRY_LOCAL__
	return HERO_LOACL_INTERVAL_TIME;
#else
	return 0;
#endif
}

typedef struct tagSettingForwardDataSt{
	char *number;
}settingForwardDataSt;
int32 hr_setting_set_forward(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	MN_SS_REGISTER_PARAM_U register_param = {0};
	SIMAT_DO_SS_STRING_T ss_string = {0};
	MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
	MN_DUAL_SYS_E dual_sys = HEROAPI_GetActiveSim();
	MN_SS_BASIC_SERVICE_T basic_service = {0};
	MN_SS_CODE_E ss_code = MN_SS_CFU;
	MMI_PARTY_NUMBER_T			party_num = {0};
	MN_CALLED_NUMBER_T			number = {0};
	uint8  phonenum[MMIDIAL_PHONENUM_LEN + 1];
	int32 ret = HR_FAILED;
	settingForwardDataSt *pForwardReq = NULL;

	if(input == NULL || input_len != sizeof(settingForwardDataSt))
	{
		return HR_FAILED;
	}

	pForwardReq = (settingForwardDataSt*)input;
	if(pForwardReq->number == NULL || strlen(pForwardReq->number) == 0)
	{
		return HR_FAILED;
	}

	basic_service.service_type = MN_SS_TELE_SERVICE;
	basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;

	//MMI_WSTRNTOSTR((uint8 *)phonenum, MMIDIAL_PHONENUM_LEN, HERO_FORWARDING_NUMBER_WSTR_PTR, HERO_FORWARDING_NUMBER_WSTR_LEN, HERO_FORWARDING_NUMBER_WSTR_LEN);
	MMI_MEMCPY((uint8 *)phonenum, MN_MAX_ADDR_BCD_LEN, pForwardReq->number, MN_MAX_ADDR_BCD_LEN, strlen(pForwardReq->number));
	MMIAPICOM_GenPartyNumber( phonenum, strlen(pForwardReq->number), &party_num);
	number.number_type = party_num.number_type;
	number.number_plan = MN_NUM_PLAN_ISDN_TELE;
	number.num_len = party_num.num_len;
	MMI_MEMCPY(number.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);
	register_param.forward_num = number;

	ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_REGISTERSS, ss_code, basic_service, register_param, ss_string);

	if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
	{
		ret = HR_SUCCESS;
		MMIHR_Trace(("MMIHEROAPP_StartCallForwarding() success."));
	}
	else
	{
		MMIHR_Trace(("MMIHEROAPP_StartCallForwarding() failed."));
	}
	return ret;
}

int32 hr_setting_cancel_forward(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	MN_SS_REGISTER_PARAM_U register_param = {0};
	SIMAT_DO_SS_STRING_T ss_string = {0};
	MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
	MN_DUAL_SYS_E dual_sys = HEROAPI_GetActiveSim();
	MN_SS_BASIC_SERVICE_T basic_service = {0};
	MN_SS_CODE_E ss_code = MN_SS_ALL_FORWARD;
	int32 ret = HR_FAILED;

	basic_service.service_type = MN_SS_ALL_SERVICE;
	basic_service.service_code = 0;

	ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_ERASESS, ss_code, basic_service, register_param, ss_string);

	if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
	{
		ret = HR_SUCCESS;
		MMIHR_Trace(("MMIHEROAPP_CloseAllCallForwarding() success."));
	}
	else
	{
		MMIHR_Trace(("MMIHEROAPP_CloseAllCallForwarding() failed."));
	}
	return ret;
}

int32 hr_setting_set_cur_scene(int32 param)
{
	uint8 scene;

	switch (param)
	{
	case HR_SCENE_INDOOR:
	case HR_SCENE_NORMAL:
		scene = MMIENVSET_STANDARD_MODE;
		break;
#if !defined(ELT_NO_VIBRATOR)		
	case HR_SCENE_MEETING:
		scene = MMIENVSET_MEETING_MODE;
		break;
	case HR_SCENE_OUTDOOR:
		scene = MMIENVSET_NOISY_MODE;
		break;
#endif		
	case HR_SCENE_MUTE:
		scene = MMIENVSET_SILENT_MODE;
		break;
	default:
		return HR_FAILED;
	}

	MMIAPIENVSET_SetActiveModeForIdle(scene);
	MMIENVSET_SetPreActiveMode(scene);
	return HR_SUCCESS;
}

int32 hr_setting_set_volume(int32 param)
{
	uint8 volume_level = MMISET_VOL_ZERO;
	if(param == 0)
	{
		volume_level = MMISET_VOL_ZERO;
	}
	else if(param > 0 && param <= 10)
	{
		volume_level = param * MMISET_VOL_MAX;
		if(volume_level < 10)
		{
			volume_level = MMISET_VOL_ONE;
		}
		else
		{
			volume_level = volume_level / 10;
		}
	}
	else
	{
		volume_level = MMISET_VOL_MAX / 2;
	}
	MMIENVSET_SetCurModeOptValue(MMISET_RING_TYPE_CALL, volume_level, HEROAPI_GetActiveSim());
	return HR_SUCCESS;
}

int32 hr_power_off(int32 param)
{
	MMIAPIPHONE_PowerOff();
	return HR_SUCCESS;
}

typedef struct tagCallLogOperateSt{
	int count;
	uint16 *callLogIds;
}callLogOperateSt;

typedef struct tagCallLogOperateInitRspSt{
	int callLogHandle;
}callLogOperateInitSt;

static void hr_setting_destory_call_log_operate_imp(callLogOperateSt *pOperate)
{
	if(pOperate != NULL)
	{
		if(pOperate->callLogIds != NULL)
		{
			SCI_FREE(pOperate->callLogIds);
		}
		SCI_FREE(pOperate);
	}
}

static callLogOperateInitSt gCallLogOperateInitData = {0};
int32 hr_setting_init_call_log_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	callLogOperateSt *pOperate = NULL;
	do
	{
		if(output == NULL || output_len == NULL)
		{
			break;
		}

		pOperate = (callLogOperateSt*)SCI_ALLOC(sizeof(callLogOperateSt));
		if(pOperate == NULL)
		{
			break;
		}
		memset(pOperate, 0, sizeof(callLogOperateSt));

		pOperate->count = MMICL_GetAllRecordsNum();
		if(pOperate->count > 0)
		{
			int index = 0;
			pOperate->callLogIds = (uint16*)SCI_ALLOC(pOperate->count * sizeof(uint16));
			if(pOperate->callLogIds == NULL)
			{
				break;
			}
			for(index = 0; index < pOperate->count; index++)
			{
				pOperate->callLogIds[index] = index;
			}
		}

		gCallLogOperateInitData.callLogHandle = (int)pOperate;

		*output = (uint8*)&gCallLogOperateInitData;
		*output_len = sizeof(callLogOperateInitSt);

		ret = HR_SUCCESS;
	}while(0);
	if(ret == HR_FAILED && pOperate != NULL)
	{
		hr_setting_destory_call_log_operate_imp(pOperate);
	}
	return ret;
}

int32 hr_setting_destory_call_log_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	callLogOperateInitSt *pOperateData = (callLogOperateInitSt*)input;
	if(input == NULL || input_len != sizeof(callLogOperateInitSt))
	{
		return HR_FAILED;
	}
	hr_setting_destory_call_log_operate_imp((callLogOperateSt*)pOperateData->callLogHandle);
	return HR_SUCCESS;
}

typedef struct tagGetCallLogReqDataSt{
	int index;
	int handle;
}getCallLogReqDataSt;

#define HR_CLOG_CLT_IDX_UNKONW (0)
#define HR_CLOG_CLT_IDX_DIALED (1)
#define HR_CLOG_CLT_IDX_MISSED (2)
#define HR_CLOG_CLT_IDX_RECVED (3)
#define HR_CLOG_CLT_IDX_REJED (4)
typedef struct tagCallLogItemDataSt{
	short log_type;
	short total_num;
	hr_datetime time;
	char name[HR_PHB_MAX_NAME];
	char number[HR_PHB_MAX_NUMBER];
}callLogItemDataSt;

static callLogItemDataSt gTempCallLogItemData = {0};
int32 hr_setting_get_call_log_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	getCallLogReqDataSt *pReqData = NULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
	do
	{
		MMI_TM_T tm = {0};
		uint8 tele_len = 0;
		uint16 calllogIndex = 0;
		callLogOperateSt *pOperate = NULL;
		MMICL_CALL_BCDINFO_T *pBcdInfo = NULL;
		MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
		uint8 temp_num[MMIPB_BCD_NUMBER_MAX_LEN*2 + 2]= {0};

		pReqData = (getCallLogReqDataSt*)input;
		if(pReqData == NULL || input_len != sizeof(getCallLogReqDataSt) || output == NULL || output_len == NULL)
		{
			break;
		}
		pOperate = (callLogOperateSt*)pReqData->handle;
		if(pOperate == NULL)
		{
			break;
		}
		if(pReqData->index >= pOperate->count || pReqData->index < 0)
		{
			break;
		}

		arraycall_last_info = SCI_ALLOC(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
		if(arraycall_last_info == NULL)
		{
			break;
		}
		SCI_MEMSET(arraycall_last_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        return_value = MMICL_ReadAllNV(arraycall_last_info);
    	if (MN_RETURN_SUCCESS != return_value)
    	{
			break;
		}

		calllogIndex = pOperate->callLogIds[pReqData->index];
		if(calllogIndex == 0xFFFF || calllogIndex >= arraycall_last_info->record_num)
		{
			break;
		}
		pBcdInfo = &arraycall_last_info->call_bcdinfo[calllogIndex];

		memset(&gTempCallLogItemData, 0, sizeof(callLogItemDataSt));

		tele_len = MMICL_GenDispNumber(pBcdInfo->call_type,
					pBcdInfo->number_type, 
					(uint8)pBcdInfo->number_len, 
					pBcdInfo->number, 
					temp_num, 
					(MMIPB_BCD_NUMBER_MAX_LEN*2 + 2));
		MMI_STRNTOWSTR(gTempCallLogItemData.number, (HR_PHB_MAX_NUMBER-1)/2, temp_num, (MMIPB_BCD_NUMBER_MAX_LEN*2 + 2), tele_len); /*lint !e64*/
		hr_str_convert_endian((char*)gTempCallLogItemData.number);

	#ifdef CALL_TIMES_SUPPORT
		gTempCallLogItemData.total_num = pBcdInfo->call_times;
	#else
		gTempCallLogItemData.total_num = 1;
	#endif
		gTempCallLogItemData.log_type = HR_CLOG_CLT_IDX_UNKONW;
		if(pBcdInfo->call_type == MMICL_CALL_DIALED)
		{
			gTempCallLogItemData.log_type = HR_CLOG_CLT_IDX_DIALED;
		}
		else if(pBcdInfo->call_type == MMICL_CALL_MISSED)
		{
			gTempCallLogItemData.log_type = HR_CLOG_CLT_IDX_MISSED;
		}
		else if(pBcdInfo->call_type == MMICL_CALL_RECEIVED)
		{
			gTempCallLogItemData.log_type = HR_CLOG_CLT_IDX_RECVED;
		}

	#ifdef CALL_TIMES_SUPPORT
		if(pBcdInfo->call_times > 0)
		{
			tm = MMIAPICOM_Second2Tm(pBcdInfo->call_start_time[pBcdInfo->call_times - 1]);
		}
	#else
		tm = MMIAPICOM_Second2Tm(pBcdInfo->call_start_time);
	#endif

		gTempCallLogItemData.time.year = tm.tm_year;
		gTempCallLogItemData.time.month = tm.tm_mon;
		gTempCallLogItemData.time.day = tm.tm_mday;
		gTempCallLogItemData.time.hour = tm.tm_hour;
		gTempCallLogItemData.time.minute = tm.tm_min;
		gTempCallLogItemData.time.second = tm.tm_sec;

		*output = (uint8*)&gTempCallLogItemData;
		*output_len = sizeof(callLogItemDataSt);

		ret = HR_SUCCESS;
	}while(0);
	if(arraycall_last_info != NULL)
	{
		SCI_FREE(arraycall_last_info);
	}
	return ret;
}

int32 hr_setting_get_call_log_count(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	callLogOperateSt *pOperate = NULL;
	getCallLogReqDataSt *pReqData = (getCallLogReqDataSt*)input;

	if(input == NULL || input_len != sizeof(getCallLogReqDataSt))
	{
		return HR_FAILED;
	}
	pOperate = (callLogOperateSt*)pReqData->handle;
	if(pOperate == NULL)
	{
		return HR_FAILED;
	}
	return HR_PLAT_VALUE_BASE + pOperate->count;
}

int32 hr_setting_delete_call_log(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	getCallLogReqDataSt *pReqData = NULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
	do
	{
		uint16 i = 0;
		uint16 index = 0;
		uint16 calllogIndex = 0;
		int tempIndex = 0;
		callLogOperateSt *pOperate = NULL;
		MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;

		pReqData = (getCallLogReqDataSt*)input;
		if(pReqData == NULL || input_len != sizeof(getCallLogReqDataSt))
		{
			break;
		}

		pOperate = (callLogOperateSt*)pReqData->handle;
		if(pOperate == NULL)
		{
			break;
		}

		if(pReqData->index >= pOperate->count || pReqData->index < 0)
		{
			break;
		}

		arraycall_last_info = SCI_ALLOC(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
		if(arraycall_last_info == NULL)
		{
			break;
		}
		SCI_MEMSET(arraycall_last_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        return_value = MMICL_ReadAllNV(arraycall_last_info);
    	if (MN_RETURN_SUCCESS != return_value)
    	{
			break;
		}

		calllogIndex = pOperate->callLogIds[pReqData->index];
		if(calllogIndex == 0xFFFF || calllogIndex >= arraycall_last_info->record_num)
		{
			break;
		}
		index = calllogIndex;

		//删除指定位置的电话记录
		SCI_MEMSET(&(arraycall_last_info->call_bcdinfo[index]), 0, sizeof(MMICL_CALL_BCDINFO_T));

		if (arraycall_last_info->record_num > MMICL_RECORD_TOTAL_NUM)
		{
			break;
		}

		//后面的电话记录前移
		for (i = index; i < (arraycall_last_info->record_num - 1) && (i + 1 < MMICL_RECORD_TOTAL_NUM); i++)
		{
			arraycall_last_info->call_bcdinfo[i] = arraycall_last_info->call_bcdinfo[i + 1];
		}

		//最后一条电话记录赋零
		SCI_MEMSET(&arraycall_last_info->call_bcdinfo[arraycall_last_info->record_num - 1], 0, sizeof(MMICL_CALL_BCDINFO_T));

		if (0 < arraycall_last_info->record_num)
		{
			arraycall_last_info->record_num--;
		}

		tempIndex = 0;
		for(i = 0; i < pOperate->count; ++i)
		{
			if(i == pReqData->index)
			{
				pOperate->callLogIds[pReqData->index] = 0xFFFF;
			}
			else
			{
				if(pOperate->callLogIds[i] != 0xFFFF)
				{
					pOperate->callLogIds[i] = tempIndex;
					tempIndex++;
				}
			}
		}

		MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);

		MMICL_UpdateEventInd();

		ret = HR_SUCCESS;
	}while(0);
	if(arraycall_last_info != NULL)
	{
		SCI_FREE(arraycall_last_info);
	}
	return ret;
}

typedef struct tagSmsInfoSt{
	char dual_sys;
	char storage;
	uint16 record_id;
}smsInfoSt;

typedef struct tagSmsOperateSt{
	int count;
	int type;
	smsInfoSt *smsInfoList;
}smsOperateSt;

typedef struct tagSmsInitCommonDataSt{
	int param1;
	int param2;
}smsInitCommonDataSt;

static int32 hr_setting_destory_sms_operate_imp(smsOperateSt *pOperate)
{
	if(pOperate != NULL)
	{
		if(pOperate->smsInfoList != NULL)
		{
			SCI_FREE(pOperate->smsInfoList);
		}
		SCI_FREE(pOperate);
	}
	return HR_SUCCESS;
}

int hero_app_destory_sms_operate(int handle)
{
	return hr_setting_destory_sms_operate_imp((smsOperateSt*)handle) == HR_SUCCESS ? 1 : 0;
}

int32 hr_setting_destory_sms_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	smsInitCommonDataSt *reqData = (smsInitCommonDataSt*)input;
	if(reqData == NULL || input_len != sizeof(smsInitCommonDataSt))
	{
		return HR_FAILED;
	}
	return hero_app_destory_sms_operate(reqData->param1) ? HR_SUCCESS : HR_FAILED;
}

int hero_app_init_sms_operate()
{
	int ret = HR_FAILED;
	smsOperateSt *pOperate = NULL;
	do
	{
		int32 count = 0;
		uint16 msg_list_size = 0;

		if(!MMISMS_IsOrderOk())
		{
			break;
		}

		msg_list_size = MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_MT);

		pOperate = (smsOperateSt*)SCI_ALLOC(sizeof(smsOperateSt));
		if(pOperate == NULL)
		{
			break;
		}
		memset(pOperate, 0, sizeof(smsOperateSt));
		pOperate->type = MMISMS_BOX_MT;

		if(msg_list_size > 0)
		{
			int i = 0;

			pOperate->smsInfoList = (smsInfoSt*)SCI_ALLOC(msg_list_size * sizeof(smsInfoSt));
			if(pOperate->smsInfoList == NULL)
			{
				break;
			}
			memset(pOperate->smsInfoList, 0, msg_list_size * sizeof(smsInfoSt));

			for(i = 0; i< msg_list_size; i++)
			{
				MMISMS_ORDER_ID_T order_id = MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_MT, i);
				if(order_id != NULL)
				{
					pOperate->count++;

					pOperate->smsInfoList[i].dual_sys = (char)order_id->flag.dual_sys;
					pOperate->smsInfoList[i].storage = (char)order_id->flag.storage;
					pOperate->smsInfoList[i].record_id = (uint16)order_id->record_id;
				}
			}
		}

		ret = HR_SUCCESS;
	}while(0);
	if(ret == HR_FAILED)
	{
		hr_setting_destory_sms_operate_imp(pOperate);
		pOperate = NULL;
	}
	return (int)pOperate;
}

static smsInitCommonDataSt gSmsInitRetData = {0};
int32 hr_setting_init_sms_operate(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int handle = 0;
	int ret = HR_FAILED;
	do
	{
		smsInitCommonDataSt *reqData = (smsInitCommonDataSt*)input;

		if(reqData == NULL || input_len != sizeof(smsInitCommonDataSt) || output == NULL || output_len == NULL)
		{
			break;
		}
		if(reqData->param1 != HR_SMS_INBOX)
		{
			break;
		}
		handle = hero_app_init_sms_operate();
		if(handle == 0)
		{
			break;
		}
		gSmsInitRetData.param1 = handle;
		*output = (uint8*)&gSmsInitRetData;
		*output_len = sizeof(smsInitCommonDataSt);
		ret = HR_SUCCESS;
	}while(0);
	if(ret == HR_FAILED)
	{
		hero_app_destory_sms_operate(handle);
	}
	return ret;
}

typedef struct tagOpreateSmsByIndexSt{
	int index;
	int handle;
}OpreateSmsByIndexSt;

typedef struct tagGetSmsResultEventDataSt{
	int index;
	int type;
	int8 t_stamp [6];
	int8 num[21];
	char *content;
}getSmsResultEventDataSt;

typedef void(*hero_app_get_sms_callback)(getSmsResultEventDataSt *smsData, int userData);

typedef struct tagGetSmsEventDataSt{
	int timerId;
	int index;
	smsInfoSt smsInfo;
	int userData;
	int msg_box_type;
	hero_app_get_sms_callback callback;
}getSmsEventDataSt;

static void hr_setting_get_sms_by_index_rsp(void *p)
{
	MMI_STRING_T string_ptr = {0};
	getSmsEventDataSt *eventReqData = (getSmsEventDataSt*)p;
	do
	{
		MMI_TM_T tm = {0};
		MMISMS_ORDER_ID_T order_id = NULL;
		getSmsResultEventDataSt retEventData = {0};

		if(eventReqData == NULL)
		{
			break;
		}

		MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, eventReqData->smsInfo.dual_sys, eventReqData->smsInfo.storage, eventReqData->smsInfo.record_id);
		if(order_id == NULL)
		{
			break;
		}

		retEventData.index = eventReqData->index;

		// time
		tm = MMIAPICOM_Second2Tm(order_id->time);
		retEventData.t_stamp[0] = tm.tm_year - 2000;
		retEventData.t_stamp[1] = tm.tm_mon;
		retEventData.t_stamp[2] = tm.tm_mday;
		retEventData.t_stamp[3] = tm.tm_hour;
		retEventData.t_stamp[4] = tm.tm_min;
		retEventData.t_stamp[5] = tm.tm_sec;

		// number
		MMIAPICOM_GenDispNumber( 
				MMIPB_GetNumberTypeFromUint8(order_id->number.bcd_number.npi_ton), 
				(uint8)MIN(order_id->number.bcd_number.number_len, HR_SMS_MAX_NUMBER), 
				order_id->number.bcd_number.number,  
				(uint8*)retEventData.num,
				sizeof(retEventData.num)
    			);

		// content
		MMIHEROAPP_SetIsReadBySMS(FALSE);
		MMISMS_GetSMSContent(&string_ptr);

		retEventData.content = string_ptr.wstr_ptr;
		if(retEventData.content != NULL)
		{
			hr_str_convert_endian((char*)retEventData.content);
		}

		if(eventReqData->callback != NULL)
		{
			eventReqData->callback(&retEventData, eventReqData->userData);
		}
		else
		{
			if (MMIHEROAPP_GetVMState() != VM_STOP)
			{
				hr_event(HR_OPREATE_SMS_EVENT, HR_SMS_OP_GET_MSG_CONTENT, (int)&retEventData);
			}
		}
	}while(0);
	if (PNULL != string_ptr.wstr_ptr)
	{
		SCI_FREE(string_ptr.wstr_ptr);
		string_ptr.wstr_ptr = PNULL;
	}
	if(eventReqData != NULL)
	{
		if(eventReqData->timerId != 0)
		{
			hero_delete_timer(eventReqData->timerId);
		}
		SCI_FREE(eventReqData);
	}
}

extern BOOLEAN g_mmisms_is_need_mem_full;
int hero_app_get_sms_by_index(int handle, int index, hero_app_get_sms_callback callback, int userData)
{
	int ret = HR_FAILED;
	smsInfoSt *pSmsInfo = NULL;
	getSmsEventDataSt *eventReqData = NULL;
	do
	{
		smsOperateSt *pOperate = NULL;
		MMISMS_ORDER_ID_T order_id = NULL;

		if(!MMISMS_IsOrderOk())
		{
			break;
		}

		if(handle == 0)
		{
			break;
		}

		pOperate = (smsOperateSt*)handle;
		if(index >= pOperate->count)
		{
			break;
		}

		pSmsInfo = pOperate->smsInfoList + index;
		if(pSmsInfo->record_id == 0xFFFF)
		{
			break;
		}
		MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, pSmsInfo->dual_sys, pSmsInfo->storage, pSmsInfo->record_id);
		if(order_id == NULL)
		{
			break;
		}

		eventReqData = SCI_ALLOC(sizeof(getSmsEventDataSt));
		if(eventReqData == NULL)
		{
			break;
		}
		eventReqData->index = index;
		eventReqData->msg_box_type = pOperate->type;
		memcpy(&eventReqData->smsInfo, pSmsInfo, sizeof(smsInfoSt));
		eventReqData->callback = callback;
		eventReqData->userData = userData;

		eventReqData->timerId = hero_create_timerId();
		if(eventReqData->timerId == 0)
		{
			break;
		}

		//MMISMSAPP_ReadOneSms
		//if (MMISMS_IsMemoryFull(order_id->flag.dual_sys))
		{
			g_mmisms_is_need_mem_full = TRUE;
		}

		MMIHEROAPP_SetIsReadBySMS(TRUE);
	#if 1
		MMISMS_SetSMSUpdateInfo(order_id);
		MMISMS_SetGlobalOperationOrderId(order_id);
	#else
		MMISMS_SetCurOperationOrderId(MMISMS_BOX_MT, req->index);
	#endif
		MMISMS_SetCurType(MMISMS_BOX_MT);
		MMIHR_Trace(("hr_platEx:MMIMRAPP_GetSMSContent order_id = %d", order_id));

		//设置当前操作状态
		MMISMS_SetOperStatus(MMISMS_READ_SMS);
		MMISMS_ReadyReadSms();
		//开始读取
		MMISMS_AppReadSms(order_id); /*lint !e718 !e18*/

		hero_start_timer(eventReqData->timerId, 500, hr_setting_get_sms_by_index_rsp, (void*)eventReqData);
		ret = HR_WAITING;
	}while(0);
	if(ret == HR_FAILED)
	{
		if(eventReqData != NULL)
		{
			SCI_FREE(eventReqData);
		}
	}
	return ret == HR_FAILED ? 0 : 1;
}

int32 hr_setting_get_sms_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int ret = HR_FAILED;
	do
	{
		OpreateSmsByIndexSt *reqData = (OpreateSmsByIndexSt*)input;

		if(reqData == NULL || sizeof(OpreateSmsByIndexSt) != input_len)
		{
			break;
		}
		if(reqData->handle == 0)
		{
			break;
		}
		ret = hero_app_get_sms_by_index(reqData->handle, reqData->index, NULL, 0) ? HR_WAITING : HR_FAILED;
	}while(0);

	return ret;
}

#define DELETE_USE_MARK
#if 0 //def DELETE_USE_MARK

typedef struct tagDeleteSmsRspSt{
	int ret;
	int index;
}deleteSmsRspSt;

typedef void(*hero_app_delete_sms_callback)(deleteSmsRspSt *smsRsp, int userData);

typedef struct tagDeleteSmsAsynSt{
	int index;
	int userData;
	hero_app_delete_sms_callback callback;
}deleteSmsAsynSt;

static void hr_setting_delete_sms_by_index_rsp(void *data)
{
	deleteSmsAsynSt *reqAsynData = NULL;
	do
	{
		deleteSmsRspSt resp = {0};

		reqAsynData = (deleteSmsAsynSt*)data;
		if(reqAsynData == NULL)
		{
			break;
		}
		resp.index = reqAsynData->index;
		resp.ret = HR_SUCCESS;

		if(reqAsynData->callback != NULL)
		{
			reqAsynData->callback(&resp, reqAsynData->userData);
		}
		else
		{
			if(MMIHEROAPP_GetVMState() != VM_STOP)
			{
				hr_event(HR_OPREATE_SMS_EVENT, HR_SMS_OP_DELETE, (int)&resp);
			}
		}		
	}while(0);
	if(reqAsynData != NULL)
	{
		SCI_FREE(reqAsynData);
	}
}

static BOOL hero_engine_del_sms_req(void *userData);
PUBLIC BOOLEAN hero_engine_del_sms_req_callback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
	if(callback_data == NULL)
	{
		return FALSE;
	}
	return hero_engine_del_sms_req(callback_data->user_data);
}

static BOOL hero_engine_del_sms_req(void *userData)
{
	MMISMS_OPERATE_DATA_INFO_T data_info = {0};
	MMISMS_ORDER_ID_T cur_del_order = PNULL;

	data_info.box_type = MMISMS_BOX_MT;
	cur_del_order = MMISMS_GetCurDeleteData(&data_info);
	if(cur_del_order == PNULL)
	{
	    MMISMS_SetDelAllFlag(FALSE);
	    MMISMS_InsertUnhandledSmsToOrder();
	    MMISMS_ReadMMSPushFromUnhandledList();
		hr_setting_delete_sms_by_index_rsp(userData);
	    return TRUE;
	}
	return MMISMSAPP_DeleteSms(cur_del_order, hero_engine_del_sms_req_callback, PNULL, userData) == NULL ? FALSE : TRUE;
}

int hero_app_delete_sms_by_index(int handle, int index, hero_app_delete_sms_callback callback, int userData)
{
	int32 ret = HR_FAILED;
	deleteSmsAsynSt *reqAsynData = NULL;
	do
	{
		BOOLEAN delRet = FALSE;
		smsOperateSt *pOperate = NULL;
		MMISMS_ORDER_ID_T order_id = NULL;
		smsInfoSt *pSmsInfo = NULL;

		// 如果当前在短信应用界面，停止短信删除操作
		{
			extern PUBLIC BOOLEAN MMISMS_IsInSMSWin(void);
			if(MMISMS_IsInSMSWin())
			{
				break;
			}
		}

		if(!MMISMS_IsOrderOk() || handle == 0)
		{
			break;
		}

		pOperate = (smsOperateSt*)handle;
		if(index >= pOperate->count)
		{
			break;
		}

		pSmsInfo = pOperate->smsInfoList + index;
		if(pSmsInfo->record_id == 0xFFFF)
		{
			break;
		}
		MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, pSmsInfo->dual_sys, pSmsInfo->storage, pSmsInfo->record_id);
		if(order_id == NULL)
		{
			break;
		}

		reqAsynData = SCI_ALLOC(sizeof(deleteSmsAsynSt));
		if(reqAsynData == NULL)
		{
			break;
		}
		memset(reqAsynData, 0, sizeof(deleteSmsAsynSt));
		reqAsynData->index = index;
		reqAsynData->callback = callback;
		reqAsynData->userData = userData;

		MMISMS_SetCurOrderMarked(order_id, TRUE);
		if(!hero_engine_del_sms_req((void*)reqAsynData))
		{
			break;
		}

		ret = HR_WAITING;
	}while(0);
	if(ret == HR_FAILED && reqAsynData != NULL)
	{
		SCI_FREE(reqAsynData);
	}
	return ret == HR_FAILED ? 0 : 1;
}

int32 hr_setting_delete_sms_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	do
	{
		OpreateSmsByIndexSt *reqData = (OpreateSmsByIndexSt*)input;
		if(reqData == NULL || sizeof(OpreateSmsByIndexSt) != input_len)
		{
			break;
		}
		ret = (hero_app_delete_sms_by_index(reqData->handle, reqData->index, NULL, 0) ? HR_WAITING : HR_FAILED);
	}while(0);
	return ret;
}
#else

typedef struct tagDeleteSmsRspSt{
	int ret;
	int index;
}deleteSmsRspSt;

typedef void(*hero_app_delete_sms_callback)(deleteSmsRspSt *smsRsp, int userData);

typedef struct tagDeleteSmsAsynSt{
	int timerId;
	int index;
	MMISMS_ORDER_ID_T nextLongSmsOrderId;
	int userData;
	hero_app_delete_sms_callback callback;
}deleteSmsAsynSt;

static void hr_setting_delete_sms_by_index_rsp(void *data)
{
	deleteSmsAsynSt *reqAsynData = NULL;
	do
	{
		deleteSmsRspSt resp = {0};
		MMISMS_ORDER_ID_T order_id = NULL;

		reqAsynData = (deleteSmsAsynSt*)data;
		if(reqAsynData == NULL)
		{
			break;
		}
		resp.index = reqAsynData->index;
		resp.ret = HR_SUCCESS;

		order_id = reqAsynData->nextLongSmsOrderId;
		if(order_id != NULL && order_id->flag.is_concatenate_sms)
		{
			if(MMISMS_DeleteReqToMN(MMISMS_BOX_MT, order_id, MMISMS_DELETE_MSG))
			{
				reqAsynData->nextLongSmsOrderId = order_id->next_long_sms_ptr;
				MMISMS_DelSMSByStore(TRUE, order_id->flag.dual_sys, order_id->flag.storage, order_id->record_id);
				hero_start_timer(reqAsynData->timerId, 100, hr_setting_delete_sms_by_index_rsp, (void*)reqAsynData);
				return;
			}
			else
			{
				resp.ret = HR_FAILED;
			}
		}

		if(reqAsynData->callback != NULL)
		{
			reqAsynData->callback(&resp, reqAsynData->userData);
		}
		else
		{
			if(MMIHEROAPP_GetVMState() != VM_STOP)
			{
				hr_event(HR_OPREATE_SMS_EVENT, HR_SMS_OP_DELETE, (int)&resp);
			}
		}
	}while(0);
	if(reqAsynData != NULL)
	{
		if(reqAsynData->timerId != 0)
		{
			hero_delete_timer(reqAsynData->timerId);
		}
		SCI_FREE(reqAsynData);
	}
}

int hero_app_delete_sms_by_index(int handle, int index, hero_app_delete_sms_callback callback, int userData)
{
	int32 ret = HR_FAILED;
	deleteSmsAsynSt *reqAsynData = NULL;
	do
	{
		BOOLEAN delRet = FALSE;
		smsOperateSt *pOperate = NULL;
		MMISMS_ORDER_ID_T order_id = NULL;
		smsInfoSt *pSmsInfo = NULL;

		// 如果当前在短信应用界面，停止短信删除操作
		{
			extern PUBLIC BOOLEAN MMISMS_IsInSMSWin(void);
			if(MMISMS_IsInSMSWin())
			{
				break;
			}
		}

		if(!MMISMS_IsOrderOk() || handle == 0)
		{
			break;
		}

		pOperate = (smsOperateSt*)handle;
		if(index >= pOperate->count)
		{
			break;
		}

		pSmsInfo = pOperate->smsInfoList + index;
		if(pSmsInfo->record_id == 0xFFFF)
		{
			break;
		}
		MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, pSmsInfo->dual_sys, pSmsInfo->storage, pSmsInfo->record_id);
		if(order_id == NULL)
		{
			break;
		}

		reqAsynData = SCI_ALLOC(sizeof(deleteSmsAsynSt));
		if(reqAsynData == NULL)
		{
			break;
		}
		memset(reqAsynData, 0, sizeof(deleteSmsAsynSt));
		reqAsynData->index = index;

		reqAsynData->timerId = hero_create_timerId();
		if(reqAsynData->timerId == 0)
		{
			break;
		}
		reqAsynData->callback = callback;
		reqAsynData->userData = userData;

		if(order_id->flag.is_concatenate_sms && order_id->next_long_sms_ptr != NULL)
		{
			reqAsynData->nextLongSmsOrderId = order_id->next_long_sms_ptr;
		}

		if(!MMISMS_DeleteReqToMN(MMISMS_BOX_MT, order_id, MMISMS_DELETE_MSG))
		{
			break;
		}
		delRet = MMISMS_DelSMSByStore(TRUE, pSmsInfo->dual_sys, pSmsInfo->storage, pSmsInfo->record_id);
		pSmsInfo->record_id = 0xFFFF;

		hero_start_timer(reqAsynData->timerId, 100, hr_setting_delete_sms_by_index_rsp, (void*)reqAsynData);
		ret = HR_WAITING;
	}while(0);
	if(ret == HR_FAILED && reqAsynData != NULL)
	{
		if(reqAsynData->timerId != 0)
		{
			hero_delete_timer(reqAsynData->timerId);
		}
		SCI_FREE(reqAsynData);
	}
	return ret == HR_FAILED ? 0 : 1;
}

int32 hr_setting_delete_sms_by_index(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int32 ret = HR_FAILED;
	do
	{
		OpreateSmsByIndexSt *reqData = (OpreateSmsByIndexSt*)input;
		if(reqData == NULL || sizeof(OpreateSmsByIndexSt) != input_len)
		{
			break;
		}
		ret = (hero_app_delete_sms_by_index(reqData->handle, reqData->index, NULL, 0) ? HR_WAITING : HR_FAILED);
	}while(0);
	return ret;
}
#endif

int hero_app_get_sms_count(int handle)
{
	smsOperateSt *pOperate = (smsOperateSt*)handle;
	if(pOperate == NULL)
	{
		return 0;
	}
	return pOperate->count;
}

int32 hr_setting_get_sms_cout(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	smsInitCommonDataSt *reqData = (smsInitCommonDataSt*)input;
	if(reqData == NULL || input_len != sizeof(smsInitCommonDataSt))
	{
		return HR_FAILED;
	}
	return HR_PLAT_VALUE_BASE + hero_app_get_sms_count(reqData->param1);
}

int32 hr_setting_sms_is_memory_full(int32 param)
{
	uint16 maxNum = 0;
	uint16 usedNum = 0;
	uint16 smsUsedNum = 0;
	BOOL nvIsFull = FALSE;
	BOOL simIsFull = FALSE;
	MN_DUAL_SYS_E activeSim = MMIHEROAPP_GetActiveSim();

	MMISMS_GetSMSUsedAndMax(activeSim, TRUE, &maxNum, &usedNum, &smsUsedNum);
	nvIsFull = usedNum >= maxNum;

	if(activeSim >= MN_DUAL_SYS_1 && activeSim < MN_DUAL_SYS_MAX)
	{
		MMISMS_GetSMSUsedAndMax(activeSim, FALSE, &maxNum, &usedNum, &smsUsedNum);
		simIsFull = usedNum >= maxNum;
	}
	return (nvIsFull && simIsFull) ? HR_SUCCESS : HR_FAILED;
}

void hero_clear_up_inbox(void)
{
//	hero_clean_spam_msg(0);
}
void hero_app_open_local_browser(char *url)
{
	if(url != NULL)
	{
#if defined(BROWSER_SUPPORT)
	{
		MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

		entry_param.type = MMIBROWSER_ACCESS_URL;
		entry_param.dual_sys = MN_DUAL_SYS_MAX;
		entry_param.url_ptr = (char *)url;
		entry_param.user_agent_ptr = PNULL;
#ifdef CMCC_UI_STYLE
		entry_param.is_alert = TRUE;
#endif
		MMIAPIBROWSER_Entry(&entry_param);
	}
#else
	#if defined(__MMI_HERO_SUPPORT_SYSBRW__)
		wap_start_browser2(2, url);
	#endif
#endif
	}
}
// modify 20191017 end

#define HR_ALM_NONE (0)
#define HR_ALM_SET (1)
#define HR_ALM_UNSET (2)
typedef struct tagAlmItemInfoSt{
	unsigned char state;
	unsigned char hour;
	unsigned char min;
    unsigned char volume;
}almItemInfoSt;

typedef struct tagAlmInfoSt{
	int almMaxCount;
	almItemInfoSt items[10];
}almInfoSt;

static almInfoSt gAlmInfo = {0};
int32 hr_get_alm_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int index = 0;

	if(output == NULL || output_len == NULL)
	{
		return HR_FAILED;
	}

	gAlmInfo.almMaxCount = MIN(ALM_ALARM_NUM, 10);
	for(index = 0; index < MIN(ALM_ALARM_NUM, 10); index++)
	{
		MMIACC_SMART_EVENT_FAST_TABLE_T *alm_node = MMIALM_GeEvent(index);

		gAlmInfo.items[index].hour = alm_node->hour;
		gAlmInfo.items[index].min = alm_node->minute;
		gAlmInfo.items[index].state = (alm_node->is_on == TRUE) ? HR_ALM_SET : HR_ALM_NONE;
		gAlmInfo.items[index].volume = 0;
	}

	*output = (uint8*)&gAlmInfo;
	*output_len = sizeof(almInfoSt);
	return HR_SUCCESS;
}

int32 hr_set_alm_info(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	int index = 0;
	SCI_DATE_T  date = {0};
	SCI_TIME_T  time = {0};
	almInfoSt *pReqData = NULL;
	int32 ret = HR_FAILED;

	if(input == NULL || input_len != sizeof(almInfoSt))
	{
		return ret;
	}
	pReqData = (almInfoSt*)input;

	if(TM_GetSysDate(&date) == ERR_TM_NONE
			&& TM_GetSysTime(&time) == ERR_TM_NONE)
	{
		for(index = 0; index < MIN(ALM_ALARM_NUM, pReqData->almMaxCount); index++)
		{
			if(pReqData->items[index].state == HR_ALM_SET)
			{
				unsigned char hour = pReqData->items[index].hour;
				unsigned char min = pReqData->items[index].min;
				unsigned char volume = pReqData->items[index].volume;

				MMIHR_Trace(("hr_set_alm_info hour = %d, min = %d", hour, min));
				if(hour >= 0 && hour <= 23 && min >= 0 && min <= 59)
				{
					MMIACC_SMART_EVENT_FAST_TABLE_T *alm_node = MMIALM_GeEvent(index);

					alm_node->year = date.year;
					alm_node->mon = date.mon;
					alm_node->day = date.mday;
					alm_node->hour = hour;
					alm_node->minute = min;

					alm_node->is_on = TRUE;
					alm_node->is_valid = TRUE;
					alm_node->type = SMART_EVENT_ALARM;
					alm_node->fre_mode = ALM_MODE_ONCE;

					if(MMIALM_ModifyEvent(index, alm_node))
					{
						ret = HR_SUCCESS;
					}
				}
			}
			else if(pReqData->items[index].state == HR_ALM_UNSET)
			{
				MMIACC_SMART_EVENT_FAST_TABLE_T *alm_node = MMIALM_GeEvent(index);
				alm_node->is_on = FALSE;
				if(MMIALM_ModifyEvent(index, alm_node))
				{
					ret = HR_SUCCESS;
				}
			}
		}
	}
	return ret;
}

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
static void hero_app_start_app_request_callback()
{
	if(MMIHEROAPP_GetVMState() != VM_STOP)
	{
		hr_event(HR_FAMILY_SMS_NOTIFY_START, 0, 0);
	}
	hero_app_notify_start();
}

void hero_app_start_app_request(int appid)
{
	hero_app_start_app_request_callback();
}

LOCAL MMI_RESULT_E MMIHEROAPP_HandleQueryStartFamilyWinMsg(MMI_WIN_ID_T     win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	BOOLEAN recode = MMI_RESULT_TRUE;

	switch(msg_id)
	{
		case MSG_APP_OK:
		case MSG_APP_WEB:
			MMIHEROAPP_AppEntry(HR_APPID_FAMILY);
		break;

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
		break;

		case MSG_TIMER://pass timer
		break;

		default:
			recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return recode;
}

void hero_family_msg_notify()
{
	if(MMIHEROAPP_GetVMState() == VM_STOP)
	{
		MMIPUB_OpenAlertWinByTextId(NULL, TXT_HERO_FAMILY_NEWMSG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, MMIHEROAPP_HandleQueryStartFamilyWinMsg);
	}
}

typedef struct tagOlalaCfgDataSt{
	int bufLen;
	char *buf;
}olalaCfgDataSt;

int32 hr_setting_read_olala_cfg_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    MN_RETURN_RESULT_E return_value = 0;
	olalaCfgDataSt *reqData = NULL;
	char temOutBuf[NVRAM_OLALA_SLIM_CACHE_SIZE] = {0};

	if(input == NULL || input_len != sizeof(olalaCfgDataSt))
	{
		return HR_FAILED;
	}

	reqData = (olalaCfgDataSt*)input;
	if(reqData->buf == NULL || reqData->bufLen == 0)
	{
		return HR_FAILED;
	}

    MMINV_READ(NVRAM_EF_OLALA_CFG_LID, temOutBuf, return_value);   
	if(return_value == MN_RETURN_SUCCESS)
	{
		memcpy(reqData->buf, temOutBuf, MIN(NVRAM_OLALA_SLIM_CACHE_SIZE, reqData->bufLen));
		return HR_SUCCESS;
	}
	return HR_FAILED;
}

int32 hr_setting_write_olala_cfg_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	short err_code;
	int ret;
	olalaCfgDataSt *reqData = NULL;
	char temOutBuf[NVRAM_OLALA_SLIM_CACHE_SIZE] = {0};

	if(input == NULL || input_len != sizeof(olalaCfgDataSt))
	{
		return HR_FAILED;
	}

	reqData = (olalaCfgDataSt*)input;
	if(reqData->buf == NULL || reqData->bufLen == 0)
	{
		return HR_FAILED;
	}

	memcpy(temOutBuf, reqData->buf, MIN(NVRAM_OLALA_SLIM_CACHE_SIZE, reqData->bufLen));
	MMINV_WRITE(NVRAM_EF_OLALA_CFG_LID, temOutBuf);

	return HR_SUCCESS;
}

void hero_family_tcp_data_notify(char *recvBuff, int recvDataLen)
{
	if(MMIHEROAPP_GetVMState() != VM_STOP)
	{
		hr_event(HR_FAMILY_TCP_RECV_NOTIFY, (int)recvBuff, (int)recvDataLen);
	}
}
#endif

void hero_gbhw_free(void *ptr)
{
	if(ptr != NULL)
	{
		SCI_FREE(ptr);
	}
}

void *hero_gbhw_malloc(int size)
{
	return (void*)SCI_ALLOCA(size);
}
#if !defined(HERO_ENGINE_TTS_SUPPORT)
void hero_app_operate_tts_setting_data(int isReset)
{
}
#endif
#endif
#endif

