/********************************************************************
	FILE:	
		OEMOS.c
	DESCRIPTION:

	AUTHOR:		
		
	DATE:
		
*********************************************************************/
#ifndef WIN32
#ifdef SXH_APPLE_SUPPORT

#include "apple_os_api.h"
#include "apple_export.h"
 

// jxh add by 20111018
#define APPLE_MINIAPPMAN

#include "OEMOS.h"

#include "apple_appdata_appman.h"
#include "apple_appdata_vp.h"
// jxh add by 20111018
#if defined  APPLE_MINIAPPMAN_A   // 简单版本
#include "apple_appdata_miniapm_A.h"
#elif defined   APPLE_MINIAPPMAN_B
#include "apple_appdata_miniapm_B.h" // 彩色版本
#elif  defined APPLE_MINIAPPMAN
#include "apple_appdata_miniapm.h"  // 动态菜单
#endif
#include "apple_appdata_umvchat.h"
#include "apple_appdata_umvchatthin.h"
#include "apple_appdata_umvchatdemo.h"

#include "vceres.h"
#include "leogun.h"
#include "svgiv_ta.h"
#include "Mmifmm_export.h"

/*
// test jxh 2012-03-6
#include "disk_api.h"
#include "bsd.h"
// end test
*/

#define APPLE_ON_TARGET
// #define APPLE_CORE_DEBUG
#ifdef APPLE_CORE_DEBUG
#define APPLE_LOG
#define APPLE_DUBUG
//#define APPLE_MEM_LEAK
#define APPLE_NET_LOG
#define APPLE_FILE_LOG
//#define APPLE_DISPLAY_LOG
#define ADDSMS_LOG
#endif

#define __SXMOBI_PLATFORM_OK__    1
// #define __UM_DUAL_SENSOR__

//#define APPLE_USE_MMK_TIMER
#ifdef APPLE_USE_MMK_TIMER
static uint8 g_iAppleTimerId = 0; 
#endif

#define PLATFORM_6800H_VERSION 2//默认为2，展英通的需修改为1
// 增加启动mini app类型和id jxh 2011-2-11
uint32  g_apple_start_type = 0;  // 0表示无，1表示固定菜单，2表示动态菜单，3表示APP应用
uint32 g_apple_start_mini_id = 0;


int g_apple_send_sms = 0;

int32 g_apple_active_sim = -1;//0;	// no sim
int g_apple_bklt = 0;
static int OEMOS_Interrupt= 0;
int32 g_isAppleGB = 0; // 0没有打断,1为打断
uint32 g_apple_ps_mode = 0; 
unsigned short g_sKey = 0;
unsigned short g_up = 0;
unsigned short g_keydown_cont = 0;
uint32 g_testdata = 0;

uint32 g_apple_lanId = 0; // 1直接返回86


AUDIO_GETSRCDATA_CALLBACK_PFUNC g_SXMOBI_callback;

int32 g_apple_ucs2_sms = 0;
static unsigned short usKeyStart = KEY_NONE;//KEY_INVALID;//FALSE;
extern MMI_WIN_ID_T gAppleWinId;
#ifdef __UM_DUAL_SENSOR__
uint8  g_iCurCamState = 0;
#endif

char  g_back_apn[MMICONNECTION_MAX_APN_LEN + 1]={0};  //liuhongmou
//#ifdef __APPLE_ADDSMS__
//#endif
#define APPLE_OS_MODEL "spreadtrum"
#define  __APPLE_ZI_ID__

#ifdef __APPLE_ZI_ID__
#define APPLE_OS_ZI_ID "a0001"
#endif

// #define UMPLAYER_ROM  0

extern char apple_get_platform_driver(unsigned int ind, unsigned int *total_size, unsigned int *left_size);
extern void AppleScrRotate(unsigned int ulRotate);

#ifdef SXH_APPLE_YOUXIN	
extern 	int SXM_OpenFileDialog(int filter_type, void *SXM_OpenFileDialog_cb, void *user_data);

#ifdef APPLE_SYS_MOTION
  #define SXM_MOTION_API_ID (APPLE_OEM_EX_BEGIN + 203)
#define MOTION_FREQUENCY 50  // 定时器多少秒读一次
#include "mmk_gsensor.h"   //加入系统的头文件，解决一些没定义问题
 
 typedef int (*cb_system_motion_listen)(void* pAccData, void* pUser); //listen时对上层的回调函数
 int apple_system_motion_power_on(); 		//系统启动重力感应
 int apple_system_motion_power_off(); 		//系统关闭重力感应
 int apple_system_motion_listen(cb_system_motion_listen cb, void* pUser); //启动监听重力感应，并设置回调和userData
 int apple_system_motion_stop_listen(); 	//停止监听重力感应
 int apple_system_motion_get_max_acc(); 	//获取最大acc值
 void system_motion_on_timer(void* pUser); //系统启动重力感应
 int g_apple_motion_flag = 0;   // 0为停止，1为开始
 cb_system_motion_listen g_apple_motion_cb = NULL;  //重力传感器的回调函数，游戏传进去获取传感器数据
 void * g_apple_User = NULL;     
 uint8 motion_timer = 0;           		// 保存 timer ID
 int apple_motion_timer_on = FALSE;    		//timer是否正在监听

 typedef struct appleMotionAcc
 {
 	int x;
 	int y;
 	int z;
 }appleMotionAcc;
 appleMotionAcc motion_appleMotionAcc = {0};   //保存传感器数据的变量

#endif
#endif

#include "apple_export.h"


LOCAL MMISRV_HANDLE_T s_SXMOBI_audio_handle = 0;   //定义player audio service handle
LOCAL MP3_STREAM_BUFFER_T s_SXMOBI_mp3_head_info = {0};  
LOCAL WAV_STREAM_BUFFER_T s_SXMOBI_wav_header_info = {0};
HAUDIO hDspMp4Record = 0;


typedef enum
{
    SXMOBI_ERR_AUD_NONE,
    SXMOBI_ERR_AUD_PARAM,               // Input parameters is error.
    SXMOBI_ERR_AUD_CONFLICT,            // It's playing ring or midi now, don't handle tone.
    SXMOBI_ERR_AUD_MAX                  // Reserved, user can't use it.
} SXMOBI_ERR_AUD_E;


/*
// test jxh 2012-03-6
uint8* g_ramdisk_mem_ptr = NULL; 
#define MAX_RAM_DISK_SIZE  1.5*1024*1024
DISK_HANDLE_PTR g_test_disk_hand = NULL;
#define WRE_RAM_DISK_NAME	L"H" 

		


void OEMOS_testDiskCreate()
{
	
	DISK_CONFIG_T  config={WRE_RAM_DISK_NAME, BSD_NORFLASH, BSD_DEVICE_NORMAL};  //模拟创建H:盘，可读写； 
	
	g_ramdisk_mem_ptr = (uint8*)SCI_ALLOC_APP(MAX_RAM_DISK_SIZE); 
	
	g_test_disk_hand = DISK_Create(g_ramdisk_mem_ptr, MAX_RAM_DISK_SIZE, &config); 


	 if (SFS_NO_ERROR == MMIFILE_RegisterDevice(WRE_RAM_DISK_NAME, MMIAPICOM_Wstrlen(WRE_RAM_DISK_NAME))) 
		 { 
			  SCI_TRACE_LOW("[WRE]RegisterDevice : RAM disk success"); 
		 } 
		 else 
		 { 
			   SCI_TRACE_LOW("[WRE]RegisterDevice: RAM disk fail, the format device"); 
				MMIFILE_FormatDevice( 
					 WRE_RAM_DISK_NAME, 
					 MMIAPICOM_Wstrlen(WRE_RAM_DISK_NAME), 
					MMIFILE_FORMAT_AUTO 
				  ); 
		   } 
		 
}


void OEMOS_testDiskDel()
{
	DISK_Delete(g_test_disk_hand); 
	if(g_ramdisk_mem_ptr)
		{
			SCI_FREE(g_ramdisk_mem_ptr);
			g_ramdisk_mem_ptr = NULL;
		}
}
// end test jxh 2012-03-6
*/


int Apple_AppIsEnableRotate(int appid)
{
	return ((0x0300009a == appid)||(0x03000041 ==appid))?1:0;
}
/********************************************************start PCM*************************************************************/
// PCM
void SXMOBI_AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );

 
int32 apple_audio_pcm_recording = 0;
int (*g_apple_pcm_record_callback)(int event, void *data, unsigned int datasize, int datathis) = NULL;
int    g_apple_pcm_record_datathis = 0;

int s_apple_rec_buffer_offset = 0;
uint16 s_apple_rec_buffer[640] = {0};
AUDIO_DEVICE_MODE_TYPE_E s_apple_cur_mode = AUDIO_DEVICE_MODE_HANDHOLD;


void apple_mdi_rec_pcm_callback(uint16 data_length)
{
	SCI_TRACE_LOW("\nAPPLE_TRACE: apple_mdi_rec_callback data_length = %d  \n", data_length);

	if(s_apple_rec_buffer_offset+data_length > sizeof(s_apple_rec_buffer)/sizeof(s_apple_rec_buffer[0]))
	{
		SCI_TRACE_LOW("\nAPPLE_TRACE: apple_mdi_rec_callback No memory \n");
		return;
	}
#if 1//defined (PLATFORM_SC6800H)
	SXMOBI_AUD_ReadVoiceData(s_apple_rec_buffer+s_apple_rec_buffer_offset, data_length); 
#else 
	AUD_ReadVoiceData(s_apple_rec_buffer+s_apple_rec_buffer_offset,  data_length);
#endif
	s_apple_rec_buffer_offset += data_length;
	SCI_TRACE_LOW("\nAPPLE_TRACE: apple_mdi_rec_callback s_apple_rec_buffer_offset = %d  \n", s_apple_rec_buffer_offset);
	if(s_apple_rec_buffer_offset >= 480)
	{
		if(g_apple_pcm_record_callback)
			g_apple_pcm_record_callback(100, s_apple_rec_buffer, s_apple_rec_buffer_offset, g_apple_pcm_record_datathis);
		SCI_TRACE_LOW("\nAPPLE_TRACE: apple_mdi_rec_callback s_apple_rec_buffer_offset = %d  \n", s_apple_rec_buffer_offset);
		memset(s_apple_rec_buffer, 0x0, sizeof(s_apple_rec_buffer));
		s_apple_rec_buffer_offset = 0; 
	}
}

/*
return: 0--success other--fail
*/
SXMOBI_ERR_AUD_E SXMOBI_AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,                                    	
										// else return error value    
										BOOLEAN is_enable,               	// SCI_TRUE: Enable,   SCI_FALSE: Disable	
										AUD_REC_FORMAT_E recorder_format,	
										RECORD_CALLBACK callback_fun        );
int apple_mdi_audio_start_record_pcm(int format,  void *data_callback, void *data_this)
{
	int err = 0;
	g_apple_pcm_record_callback = NULL; 
	g_apple_pcm_record_datathis = 0;
	apple_audio_pcm_recording = 0; 
	memset(s_apple_rec_buffer, 0x0, sizeof(s_apple_rec_buffer));
	s_apple_rec_buffer_offset = 0;

	AUDIO_GetDevMode(&s_apple_cur_mode);
SCI_TRACE_LOW("\nAPPLE_TRACE:	apple_mdi_audio_start_record_pcm %d entry!\n",s_apple_cur_mode);	
#if 1//defined(PLATFORM_SC6600L) || defined (PLATFORM_SC6800H)
	AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
#else
	MMIAUDIO_SetCurDevMode(AUD_DEV_MODE_HANDFREE);
#endif
#if 1//defined (PLATFORM_SC6800H)
	err = SXMOBI_AUD_EnableVoiceRecord(TRUE, AUD_REC_PCM, apple_mdi_rec_pcm_callback);
#else
	err = AUD_EnableVoiceRecord(TRUE, AUD_REC_PCM, apple_mdi_rec_pcm_callback);
#endif 
	if(err == 0)
	{
		apple_audio_pcm_recording = 1;
		g_apple_pcm_record_callback = (int (*)(int , void *, unsigned int, int ))data_callback;
		g_apple_pcm_record_datathis = (int)data_this; 
	}

	SCI_TRACE_LOW("\nAPPLE_TRACE:   apple_mdi_audio_start_record  AUD_EnableVoiceRecord err= %d!\n", err);

	return err;
}

int apple_mdi_audio_stop_record_pcm(void)
{
SCI_TRACE_LOW("\nAPPLE_TRACE:   apple_mdi_audio_stop_record_pcm  entry!\n");

#if 1//defined (PLATFORM_SC6800H)
	SXMOBI_AUD_EnableVoiceRecord(FALSE, AUD_REC_PCM, PNULL);
#else
	AUD_EnableVoiceRecord(FALSE, AUD_REC_PCM, PNULL);
#endif
	AUDIO_SetDevMode(s_apple_cur_mode);

	g_apple_pcm_record_callback = NULL;
	g_apple_pcm_record_datathis = 0;
	apple_audio_pcm_recording = 0;
	return 0;
}

/********************************************************end PCM*************************************************************/

void OEMOS_Init_DY(uint32 appid)
{
//OEMOS_dbgprintf("OEMOS_Init_DY appid %d",appid);	

#if 0
#ifdef APPLE_ROTATE_DYNAMIC
		if(  Apple_AppIsEnableRotate(appid))
		{
			OEMOS_dbgprintf("OEMOS_Init_DY  set suport  rotate");
			//MMK_SetWinAngle(APPLE_MAIN_WIN_ID, LCD_ANGLE_0, FALSE);
			MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID,WIN_SUPPORT_ANGLE_ALL);
		}
		else
		{
			OEMOS_dbgprintf("OEMOS_Init_DY  set NO  suport  rotate");
			MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID,WIN_SUPPORT_ANGLE_0);
			MMK_SetWinAngle(APPLE_MAIN_WIN_ID, LCD_ANGLE_0, FALSE);
			AppleScrRotate(0);
		}
#else
		//MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID,WIN_SUPPORT_ANGLE_0);
		//MMK_SetWinAngle(APPLE_MAIN_WIN_ID, LCD_ANGLE_0, FALSE);
		OEMOS_dbgprintf("OEMOS_Init_DY  set  NO suport  rotate");
#endif
#endif

}
int Apple_Get_pbCont(void)
 {
 	uint16 count = 0;
	// count = MMIPB_GetEntryCount();
	count =MMIAPIPB_GetPhoneContactCount()+MMIAPIPB_GetAllSimContactCount();
 	return (int) count;
 }
extern uint16 *OEM_strtowstr(const char *pszIn, uint16 *pDest, int nSize);
 int ApplePhoneBookGetItem(unsigned int idx,
								   unsigned short *name, 
								   unsigned int name_len, 
								   unsigned short *pn, 
								   unsigned int pn_len, 
								   unsigned int *loc)
{
#if 1 //liuhongmou//SXH_APPLE_YOUXIN	
	MMIPB_CONTACT_BASE_INFO_T  um_sort_t={0};	
    	 MMIPB_CONTACT_T um_entry = {0};
	char testtemp[64] = {0};
	int result;
	 MMIPB_STRING_T      number_str = {0};
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
	dual_sys=OEMOS_GetActiveSim();
	MMIPB_ReadContactSimpleInfo( idx, &um_sort_t );
	um_entry.contact_id=um_sort_t.contact_id;
	result=MMIAPIPB_GetContactInfo(um_entry.contact_id,um_sort_t.storage_id,(MMIPB_CONTACT_T *)&um_entry);
	if(result!=MMIPB_ERROR_SUCCESS)
		return FALSE;
	   number_str.strlen = MMIAPICOM_GenDispNumber(
                                        MMIPB_GetNumberTypeFromUint8(um_entry.number[0].npi_ton),
                                        um_entry.number[0].number_len,
                                        um_entry.number[0].number,
                                        number_str.strbuf,
                                        MMIPB_MAX_STRING_LEN
                                        );
	//				    OEMOS_dbgprintf("PHONEBOOK_SIM_STORAGE \r\n");	
	//			          sxmvc_call_outgoing_lsk(number_str.strbuf);	
#ifdef APPLE_LOG	
						  
       OEMOS_dbgprintf("ApplePhoneBookGetItem idx %d  len %d num %s ",idx, number_str.strlen, number_str.strbuf);
#endif
	   
	// OEM_wstrtostr(um_entry.entry_name.wstr,(char *) testtemp,0);
	// OEMOS_dbgprintf("MMIPB_GetEntryInList testtemp%s",testtemp);
	  
	if(um_entry.name.wstr_len >= name_len)
	   	return -1;

#ifdef APPLE_LOG	
	OEMOS_dbgprintf("ApplePhoneBookGetItem len %d",um_entry.name.wstr_len);
#endif

     //  memcpy(name,um_entry.entry_name.wstr,um_entry.entry_name.wstr_len);
      MMIAPICOM_Wstrncpy(name,um_entry.name.wstr,um_entry.name.wstr_len);
	 
	if(number_str.strlen >= pn_len)
		return -1;

	OEM_strtowstr(number_str.strbuf, pn, 0);
	
	switch(um_sort_t.group)
	{
		case PB_GROUP_SIM_1:
			*loc = 1;
			break;
		case PB_GROUP_SIM_2:
			*loc = 2;
			break;
	     default:
		 	*loc = 0;
			break;
	}
#endif
	return 0;
	
}


int g_enable_idle_key = 1;  // 0为禁止，1为回到idle
void APPLE_enable_idle_key(int n)
{
	g_enable_idle_key = n;
}

extern void apple_os_release_filter(char * temp);

extern unsigned short apple_chset_ucs2_to_text_str(unsigned char *dest, int dest_size, unsigned char *src, int encoding);
extern unsigned short apple_chset_text_to_ucs2_str(unsigned char*dest, int dest_size, unsigned char *src, int encoding);


//#ifdef __APPLE_ADDSMS__
#define APPLE_ADD_SMS_DB  apple_game_016  ///
extern  int APPLE_ADD_SMS_DB(int action, void *param1, unsigned int param1_size);//for app
// #endif


#ifdef UMPLAYER_ROM
#include "avenginebe.c"
#endif


extern int sms_content_converter(unsigned int iappid, char *dst, unsigned int dst_len, char *src, unsigned int src_len);
//////////////////////////////////////////////////////////////////////////
//					APPLE SYSTEM Config									//
//////////////////////////////////////////////////////////////////////////


static int32  s_playerHandler =  0;

int32 OEMDPALY_GetPlayerHandler()
{
	return s_playerHandler;
}


// extern  void OEM_DISP_FreeMEM();
int OEMDPLAYER_Init()
{
	OEMOS_dbgprintf("OEMDPLAYER_Init  ");
	// OEM_DISP_FreeMEM();
#if PLATFORM_6800H_VERSION==1    
	DC_VideoDisplayClose(); //展英通的需要调用这个
#endif
	return DPLAYER_Init();
}

int OEMDPLAYER_Release()
{
	return DPLAYER_Release();
}

int32 OEMDPLAYER_Open(
    const char* szFileName)
{
	uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH] = {0};
	void  *argv[1]   = {0}; 
	
	//memset(szUCS2Filename, 0 ,sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)szFileName, strlen(szFileName), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);

	OEMOS_dbgprintf("OEMDPLAYER_Open  szFileName:%s",szFileName);
	argv[0] = szUCS2Filename;
	s_playerHandler = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE,1,argv);
	return s_playerHandler;
}

int OEMDPLAYER_Close(DPLAYER_HANDLE   player_handle)
{
	OEMOS_dbgprintf("OEMDPLAYER_Close  ");
	s_playerHandler = 0 ;

	return  DPLAYER_Close(player_handle);
}

int  OEMDPLAYER_Play(DPLAYER_HANDLE    player_handle)
{
    int ret = 0 ;

	ret = DPLAYER_Play(player_handle);

	OEMOS_dbgprintf("OEMDPLAYER_Play  ret:%d",ret);

    return ret;
}


int OEMDPLAYER_Stop(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_Stop(player_handle);
}


int OEMDPLAYER_Pause(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_Pause(player_handle);
}

int OEMDPLAYER_PauseEx(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_PauseEx(player_handle);
}


int OEMDPLAYER_Resume(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_Resume(player_handle);
}


int OEMDPLAYER_ResumeEx(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_ResumeEx(player_handle);
}


uint32 OEMDPLAYER_GetLength(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_GetLength(player_handle);
}


int OEMDPLAYER_SetTime(DPLAYER_HANDLE    player_handle,uint32 i_time)
{    
    return DPLAYER_SetTime(player_handle,i_time);
}

uint32 OEMDPLAYER_GetTime(DPLAYER_HANDLE    player_handle)
{    
    return DPLAYER_GetTime(player_handle);
}



int OEMDPLAYER_SetPlayRate(DPLAYER_HANDLE     player_handle,
        int rate)
{
	return DPLAYER_SetPlayRate(player_handle,rate);
}

int OEMDPLAYER_UpdateCurrentFrame(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_UpdateCurrentFrame(player_handle);
}

int OEMDPLAYER_UpdateNextFrame(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_UpdateNextFrame(player_handle);
}

int OEMDPLAYER_GetMovieInfo(DPLAYER_HANDLE    player_handle,
    DPLAYER_MEDIA_INFO_T *p_info)
{
	return DPLAYER_GetMovieInfo(player_handle,p_info);
}

int OEMDPLAYER_SetFrameEndNotify(DPLAYER_HANDLE   player_handle,
    void (*pf_notify)(int32 frame_num))
{
    return DPLAYER_SetFrameEndNotify(player_handle,pf_notify);
}

int OEMDPLAYER_SetEndNotify(DPLAYER_HANDLE   player_handle,
    void (*pf_notify)(int32 i_type))
{
    return DPLAYER_SetEndNotify(player_handle,pf_notify);
}

int OEMDPLAYER_SetDisplayParam(DPLAYER_HANDLE player_handle,int x,int y,int dx,int dy,int disp_mode,int rot)
{
	DPLAYER_DISPLAY_PARAM_T display_param ;
	DPLAYER_DISPLAY_PARAM_T  *param_ptr =&display_param;
	int ret = 0 ;

	display_param.RotateAngle = rot ;

	display_param.target_rect.x = x;//(240 - 160)/2;
	display_param.target_rect.y = y;//(320 - 120)/2;
	display_param.target_rect.dx = dx;//160;
	display_param.target_rect.dy = dy;//120;

	display_param.disp_rect.x = x;
	display_param.disp_rect.y = y ;
	display_param.disp_rect.dx = dx;
	display_param.disp_rect.dy = dy;
	
	display_param.LcdBlkId = 0;
	display_param.LcdId = 0;
	display_param.disp_mode = disp_mode;
	display_param.videoSupport = 1;
	display_param.audioSupport = 1;

	OEMOS_dbgprintf("[VP] APPLE VP_SwitchDisplayMode   disp(%d,%d,%d,%d) target_rect(%d,%d,%d,%d),rot:%d ,video_support:%d",
		display_param.disp_rect.x,display_param.disp_rect.y,display_param.disp_rect.dx,display_param.disp_rect.dy,
		display_param.target_rect.x,display_param.target_rect.y,display_param.target_rect.dx,display_param.target_rect.dy,
		display_param.RotateAngle,display_param.videoSupport);
	ret = DPLAYER_SetDisplayParam(player_handle,&display_param);

    return ret;
}

void OEMDPLAYER_SetOsd(int alpha,int color_key)
{
	int ret =0;
	ret = GUIBLOCK_SetType(alpha,color_key,GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());    

	OEMOS_dbgprintf("OEMDPLAYER_SetOsd    ret:%d",ret);
}

void OEMDPLAYER_ResetOsd()
{
	GUIBLOCK_ResetMainType(); 
	OEMOS_dbgprintf("OEMDPLAYER_ResetOsd    ");
}


int OEMDPLAYER_GetCurrentFrameData(DPLAYER_HANDLE     player_handle,  
    DPLAYER_IMAGE_DATA_T    * frame_data_ptr)
{
    return DPLAYER_GetCurrentFrameData(player_handle,frame_data_ptr,NULL,NULL);
}


int OEMDPLAYER_ForceAVSync(DPLAYER_HANDLE player_handle,
    BOOLEAN     support_skip )
{
    return DPLAYER_ForceAVSync(player_handle,support_skip);
}

void OEMDPLAYER_SetVolume(int  volume)
{
	MMIAPISET_InitAudioDevice(volume);
}

int OEMDPLAYER_GetCurVolume()
{
	// return MMIAUDIO_GetCurVolume();
	
   
	return MMISRVAUD_GetVolumeNoHandle();
}

int OEMDPLAYER_GetMaxVolume()
{
	return  0; //MMI_AUDIO_VOLUME_MAX;
}

int OEMDPLAYER_SetMute(int bMute)
{
	return  0; //MMIAUDIO_SetMute(bMute);
}

int OEMPLAYER_GetMute()
{
	return 0; //MMIAUDIO_GetMute();
}

extern void OEMOS_send_mediaplayer_msg(unsigned int data1, unsigned int data2,void* callback);


#if 1
extern int APPLE_FileMgr_EnumNext(void* pIFileMgr, void * pInfo);
#endif 


#ifdef APPLE_ROM_APP_VP//IMPORTANT
#include "sxmvp_aim.c"
#include "vpallinonearray.c"
#include "sxmvp_brw.c"
//#include "sxmvp_fone.c"
#endif

unsigned char *sccvv_mod =  NULL;
unsigned char *svgiv_lib = NULL;
unsigned char *leo1_mod = NULL;
unsigned char *leo2_mod = NULL;
// int g_apple_inputSuspend = 0;
int g_apple_inputState = 0;


void OEMOS_pauseMp3(void)
{
	MMISRVAUD_ReqVirtualHandle(APPLE_VIDEOPLAYER, MMISRVAUD_PRI_NORMAL);
   
}


void OEMOS_resumeMp3(void)
{
	
	MMISRVAUD_FreeVirtualHandle(APPLE_VIDEOPLAYER);
	
}


int OEM_GetDeviceInfoEx(int nItem, void *pBuff, int *pnSize)
{
	
    int *param = (int *)pBuff;
     switch(nItem)
	{
#ifdef SXH_APPLE_YOUXIN	
	// add jxh 2012-3-26
	 case (APPLE_OEM_EX_BEGIN + 223):
		 {
			 *(int *)pBuff = (int)SXM_OpenFileDialog;
		 }
		 break;
#endif
/*	case (APPLE_OEM_EX_BEGIN + 192)://展讯双T卡
		{
			*(int8*)pBuff = 'd';
		}
		break;*/
		case APPLE_UMPlayer_FSC_LeftOrRight:
				{
								*(int*)pBuff =0;// 全屏时向左还是向右旋转：1,向左；0，向右，默认向右
				}
				break;

	  case APPLE_UMVCHAT_SUPFULLSCREEN:
	  		{
					*(int*)pBuff =0;
			}
	  break;
	  case APPLE_OS_API_VP_MediaSupportResume:
	  		{
					*(int*)pBuff =0;
			}
	  break;
/*
		case APPLE_UMPlayer_GetHelpInfo://通过能力获得解霸帮助信息
                {     
                    
                    MMI_STRING_T   ump_help = {0};      
                    wchar ump_help_buf[] = { 0x6B22, 0x8FCE, 0x4F7F, 0x7528, 0x89E3, 0x9738, 0xFF0C, 0x89E3, 0x9738, 0x662F, 0x4E00, 0x6B3E, 0x5B89,
						0x5168, 0x514D, 0x8D39, 0x7684, 0x624B, 0x673A, 0x89C6, 0x9891, 0x5F71, 0x97F3, 0x64AD, 0x653E, 0x8F6F, 0x4EF6, 
						0xFF0C, 0x652F, 0x6301, 0x64AD, 0x653E, 0x0072, 0x006D, 0xFF0C, 0x0072, 0x006D, 0x0076, 0x0062, 0xFF0C, 0x006D,
						0x0070, 0x0034, 0xFF0C, 0x0061, 0x0076, 0x0069, 0xFF0C, 0x0033, 0x0067, 0x0070, 0xFF0C, 0x0033, 0x0067, 0x0070, 
						0x0070, 0xFF0C, 0x0066, 0x006C, 0x0076, 0x4E03, 0x79CD, 0x4E3B, 0x6D41, 0x624B, 0x673A, 0x89C6, 0x9891, 0x683C,
						0x5F0F, 0x3002, 0x0024, 0x89E6, 0x5C4F, 0x624B, 0x673A, 0x7528, 0x6237, 0x8BF7, 0x5728, 0x5C4F, 0x5E55, 0x76F8, 
						0x5E94, 0x533A, 0x57DF, 0x8FDB, 0x884C, 0x64CD, 0x4F5C, 0x002C, 0x5982, 0x6709, 0x4FA7, 0x6309, 0x952E, 0x0020, 
						0x7528, 0x4FA7, 0x6309, 0x952E, 0x8C03, 0x8282, 0x97F3, 0x91CF, 0x0024, 0x0031, 0x002E, 0x542F, 0x52A8, 0x89E3, 
						0x9738, 0xFF0C, 0x70B9, 0x51FB, 0x83DC, 0x5355, 0x4E2D, 0x7684, 0x89C6, 0x9891, 0x5217, 0x8868, 0x9009, 0x9879,
						0xFF0C, 0x8F7D, 0x5165, 0x89E3, 0x9738, 0x652F, 0x6301, 0x7684, 0x89C6, 0x9891, 0x683C, 0x5F0F, 0x7684, 0x672C, 
						0x5730, 0x6587, 0x4EF6, 0x5217, 0x8868, 0xFF0C, 0x9009, 0x62E9, 0x89C6, 0x9891, 0x6587, 0x4EF6, 0x5373, 0x53EF, 
						0x64AD, 0x653E, 0xFF1B, 0x0024, 0x0032, 0x002E, 0x9009, 0x62E9, 0x64AD, 0x653E, 0x89C6, 0x9891, 0x6587, 0x4EF6,
						0xFF0C, 0x70B9, 0x51FB, 0x201C, 0x64AD, 0x653E, 0x201D, 0x529F, 0x80FD, 0x9879, 0x3002, 0x89C6, 0x9891, 0x64AD, 
						0x653E, 0x8FC7, 0x7A0B, 0x4E2D, 0xFF0C, 0x201C, 0x64AD, 0x653E, 0x201D, 0x4F1A, 0x5207, 0x6362, 0x6210, 0x201C, 
						0x6682, 0x505C, 0x201D, 0x529F, 0x80FD, 0x9879, 0xFF0C, 0x53EF, 0x8FDB, 0x884C, 0x89C6, 0x9891, 0x7684, 0x201C,
						0x64AD, 0x653E, 0x002F, 0x6682, 0x505C, 0x201D, 0x64CD, 0x4F5C, 0xFF1B, 0x0024, 0x0033, 0x002E, 0x64AD, 0x653E, 
						0x89C6, 0x9891, 0x65F6, 0x5DE6, 0x53F3, 0x952E, 0x5206, 0x522B, 0x4E3A, 0x201C, 0x83DC, 0x5355, 0x201D, 0x548C,
						0x201C, 0x505C, 0x6B62, 0x201D, 0x529F, 0x80FD, 0x9879, 0xFF1B, 0x70B9, 0x51FB, 0x53F3, 0x8F6F, 0x952E, 0x53EF, 
						0x505C, 0x6B62, 0x5F53, 0x524D, 0x89C6, 0x9891, 0x64AD, 0x653E, 0xFF0C, 0x70B9, 0x51FB, 0x5DE6, 0x8F6F, 0x952E, 
						0x5F39, 0x51FA, 0x83DC, 0x5355, 0x9009, 0x9879, 0xFF1A, 0x70B9, 0x51FB, 0x201C, 0x5168, 0x5C4F, 0x64AD, 0x653E, 
						0x201D, 0x952E, 0x5168, 0x5C4F, 0x534A, 0x5C4F, 0x5207, 0x6362, 0x64CD, 0x4F5C, 0xFF1B, 0x901A, 0x8FC7, 0x201C, 
						0x64AD, 0x653E, 0x63A7, 0x5236, 0x201D, 0x529F, 0x80FD, 0x9879, 0x53EF, 0x5BF9, 0x5F53, 0x524D, 0x64AD, 0x653E, 
						0x89C6, 0x9891, 0x8FDB, 0x884C, 0x5FEB, 0x8FDB, 0x002F, 0x5FEB, 0x9000, 0xFF0C, 0x51CF, 0x5C0F, 0x002F, 0x589E, 
						0x5927, 0x97F3, 0x91CF, 0x64CD, 0x4F5C, 0xFF0C, 0x70B9, 0x51FB, 0x5E2E, 0x52A9, 0x4E0B, 0x201C, 0x5173, 0x4E8E, 
						0x201D, 0x53EF, 0x67E5, 0x770B, 0x5F53, 0x524D, 0x8F6F, 0x4EF6, 0x7248, 0x672C, 0x4FE1, 0x606F, 0x548C, 0x0069, 
						0x006D, 0x0065, 0x0069, 0x4FE1, 0x606F, 0xFF1B, 0x0024, 0x3010, 0x5BA2, 0x6237, 0x7535, 0x8BDD, 0xFF1A, 0x0034, 
						0x0030, 0x0030, 0x002D, 0x0031, 0x0030, 0x0031, 0x002D, 0x0030, 0x0030, 0x0030, 0x0032, 0x3011, 0 }; 
                    ump_help.wstr_ptr = ump_help_buf;
                    ump_help.wstr_len = sizeof(ump_help_buf);
                    memcpy(pBuff,ump_help.wstr_ptr ,ump_help.wstr_len );
                     *pnSize=ump_help.wstr_len;
                }
                break;
*/
#ifdef APPLE_ROM_APP_VP//IMPORTANT
       case APPLE_OS_API_VP_GetVPRomData://prom
       {
          if(param[0]== 0)
          {
              param[1]=(int)sxmvp_aim;
              param[2]=sizeof(sxmvp_aim);
          }
          else if(param[0]== 1)
          {
              param[1]=(int)C_VPAllInOnArray;
              param[2]=sizeof(C_VPAllInOnArray);
          }
          else if(param[0]== 2)
          {
              param[1]=(int)sxmvp_brw;//small rom,modify (int)0
              param[2]=sizeof(sxmvp_brw);//small rom,modify (int)0
          }
          /*
          else if(param[0]==3)
          {
              param[1]=(int)sxmvp_fone;
              param[2]=sizeof(sxmvp_fone);            
          }*/
          }
      break;
#endif

  case (APPLE_OEM_EX_BEGIN + 130):
    	 {
    	 	int rv = APPLE_getcalltime_sec();
    		if(g_testdata)
    		{
    			param[0]=g_testdata;
    		}
    		else
    		{
    	 	    param[0] = rv;
    		}
    		param[1] = 60 * 60;
    		param[2] = 60 * 60;
         }
	     break;

	case APPLE_UMVCHAT_USETH:
		{
			*(int*)pBuff = 0;
		}
	break;

	case APPLE_MINIAPM_NO_LOAD_WAP_PROFILE:
		{
			*(int*)pBuff = 1;
		}
		break;
	case APPLE_OS_API_FILEMGR_NEW:
		{
			param[0] = (int)APPLE_FileMgr_EnumNext; 	
#if 0
			param[0] = APPLE_FileMgr_OpenFile; 		
			param[1] = APPLE_FileMgr_GetFInfo; 
			param[2] = APPLE_FileMgr_Remove; 	
			param[3] = APPLE_FileMgr_Test;	
			param[4] = APPLE_FileMgr_EnumInit; 	
			param[5] = APPLE_FileMgr_EnumNext; 	
			param[6] = APPLE_FileMgr_Rename; 		
#endif
		}
		break;
#if 0	
	case APPLE_OS_API_MP_DPLAYER:
		{
			if(pBuff)
			{	
				param[0] = (int)OEMDPLAYER_Init; 		
				param[1] = (int)OEMDPLAYER_Release; 
				param[2] = (int)OEMDPLAYER_Open; 	
				param[3] = (int)OEMDPLAYER_Close;	
				param[4] = (int)OEMDPLAYER_Play; 	
				param[5] = (int)OEMDPLAYER_Stop; 	
				param[6] = (int)OEMDPLAYER_Pause; 		
				param[7] = (int)OEMDPLAYER_PauseEx; 	
				param[8] = (int)OEMDPLAYER_Resume; 		
				param[9] = (int)OEMDPLAYER_ResumeEx; 	
				param[10] =(int)OEMDPLAYER_GetLength ; 
				param[11] = (int)OEMDPLAYER_SetTime; 	
				param[12] = (int)OEMDPLAYER_GetTime; 	
				param[13] = (int)OEMDPLAYER_SetPlayRate;
				param[14] = (int)OEMDPLAYER_UpdateCurrentFrame; 	
				param[15] = (int)OEMDPLAYER_UpdateNextFrame; 		
				param[16] = (int)OEMDPLAYER_GetMovieInfo; 			
				param[17] = (int)OEMDPLAYER_SetFrameEndNotify; 		
				param[18] = (int)OEMDPLAYER_SetEndNotify; 			
				param[19] = (int)OEMDPLAYER_SetDisplayParam;	
				param[20] = (int)OEMDPALY_GetPlayerHandler;		
				param[21] = (int)OEMDPLAYER_SetOsd;				
				param[22] = (int)OEMDPLAYER_ResetOsd;			
				param[23] = (int)OEMDPLAYER_GetCurrentFrameData;// 
				param[24] = (int)OEMDPLAYER_ForceAVSync; 
				param[25] = (int)OEMOS_send_mediaplayer_msg;	/*(unsigned int data1, unsigned int data2,void* callback)*/;
				param[26] = (int)OEMDPLAYER_SetMute;	
				param[27] = (int)OEMDPLAYER_SetVolume;
				param[28] = (int)OEMDPLAYER_GetCurVolume;
				param[29] = (int)OEMDPLAYER_GetMaxVolume;
				param[30] = (int)OEMPLAYER_GetMute;
				
				
			}
		}
	break;
#endif	
#ifdef UMPLAYER_ROM	
	case APPLE_OS_API_MP_AVData:
		{

			if(pBuff)
			{
				param [0]=(int)C_AVEngineBe;
				param [1]=sizeof(C_AVEngineBe);
			}
		}
		break;
#endif
	case APPLE_OS_API_VP_GET_SWAPXY://
		{
			*(int*)pBuff = 0;
		}
		break;
	case APPLE_OS_SPT_JPEGBUFFSIZE:
		{
			*(int *)pBuff  = 120*1024;
		}
		break;
	case APPLE_OS_USELCDROTATE:
		{
			*(int*)pBuff = 0;
		}
	   break;
     	case APPLE_OS_SET_PS_MODE:
		{
			if(NULL == pBuff )
				return EFAILED;
			g_apple_ps_mode = *(uint32*)pBuff;
		} 
		break;
		case (APPLE_OEM_EX_BEGIN + 161)://IMPORTANT
     		{
     			*(int *)pBuff = 4;
     		}
     		break;

		case (APPLE_OEM_EX_BEGIN + 162): //IMPORTANT
	 	{
			if(param)
			{
				param[0] = 0;
				param[1] = 0;	
                	#ifdef SXH_APPLE_APP
    			param[1] = param[1] | 0x01;
    			#endif
    			#ifdef SXH_APPLE_UMVCHAT
    			param[1] = param[1] | 0x02;
    			#endif
    			#ifdef SXH_APPLE_VP
    			param[1] =param[1] | 0x04;
    			#endif
    			#ifdef SXH_APPLE_RP
    			param[1] = param[1] | 0x08;
    			#endif
    			#ifdef SXH_APPLE_BOOK
    			param[1] = param[1] | 0x10;
    			#endif
    			#ifdef SXH_APPLE_3GMUSIC
    			param[1] = param[1] | 0x20;
    			#endif
    			#ifdef SXH_APPLE_WANKE
    			param[1] =param[1] | 0x40;
    			#endif
    			#ifdef SXH_APPLE_VIDEODL
    			param[1] = param[1] | 0x80;
    			#endif
    			#ifdef SXH_APPLE_WEATHER
    			param[1] =param[1] | 0x100;
    			#endif
    			#ifdef SXH_APPLE_ONLINEGAME
    			param[1] = param[1] | 0x200;
    			#endif
			}
	     	}
		 break;
		
		case (APPLE_OEM_EX_BEGIN + 163): //IMPORTANT
	 	{
			param[0]=0;
			param[1]=5;
			param[2]=15;
     		}
		break;
		case (APPLE_OEM_EX_BEGIN + 148): //IMPORTANT
		{
			*(int *)pBuff = (int)APPLE_ADD_SMS_DB;
		}
		break;
		case (APPLE_OEM_EX_BEGIN + 159): //IMPORTANT
	     	{
	      	//	extern void apple_game_028(void);  // jxh 
	     	//	 *(int *)pBuff = (int)apple_game_028; // jxh
	      	}
	      	break;
		case (APPLE_OEM_EX_BEGIN + 134)://IMPORTANT
	     	{
	      		g_testdata = *(int *)pBuff;
	      	}
      		break;
		case (APPLE_OEM_EX_BEGIN + 168)://外文版语言测试
		{
			g_apple_lanId = *(uint32 *)pBuff;
		}
		break;
		case (APPLE_OEM_EX_BEGIN + 194):
		{
			//int *param = (int *)pBuff;
			param[0]=(int)2;
			param[1]=(int)apple_mdi_audio_start_record_pcm;
			param[2]=(int)apple_mdi_audio_stop_record_pcm;
		}
		break; 
	case APPLE_OS_API_MP_APPLECBCALL:
		AppleSysCall(pBuff);
		break;
	case APPLE_OS_APPVOICERECORD_API:
		{
			param[0]=(int)apple_mdi_audio_start_record;
			param[1]=(int)apple_mdi_audio_stop_record;
			param[6] = (int)OEMCAM_PowerOn;
			param[10] = (int)OEMCAM_PowerDown;
			param[8] = (int)OEMCAM_Init;
			param[7] = (int)OEMCAM_SetDataCb;
			param[9] = (int)OEMCAM_DataReq;
			param[17] = (int)OEMCAM_Switch;
			param[18] = (int)OEMCAM_SetRotate;
			param[19] = (int)OEMCAM_SendCmdWorker;
			
		}
	break;
	 case APPLE_OS_JPEGCODEC:
	 	{
//			int *param = (int *)pBuff;
			param[0]=(int)AppleJpegEnc;
			param[1]=(int)AppleJpegDec;	
			//param[2]=(int)OEMISP_ConvImg;
			param[3]=(int)OEMISP_Open;
			param[4]=(int)OEMISP_Close;
			param[5]=(int)OEMDisplay_DirectDrawImg;
		}
 	     return SUCCESS;
	case APPLE_OS_API_MP_PCMSETTING:
		{
			// test jxh
			/*
			extern  BOOLEAN SXMOBI_MMIAUDIO_PcmSetting(   int aChan, int aSamp,int aSampBit  )      ;
			int *param = (int *)pBuff;
			SXMOBI_MMIAUDIO_PcmSetting(param[0], param[1], param[2]);
			*/
			
		}
	     return SUCCESS;
	case APPLE_OS_DISP:
		{
			
			
			#if 0  // jxh waiting 2012-2-1
			extern int AppleDispSetParam_Impl(int iSrcW, int iSrcH, int iYPitch, int iUVPtich, apple_rect *pDispRect,int rot);
			extern int AppleDisp_Impl(char* pY, char* pU, char* pV);
			int *param = (int *)pBuff;
				param[0] = (int)AppleDispSetParam_Impl;
				param[1] = (int)AppleDisp_Impl;
			#endif

		}
		return SUCCESS;
	case APPLE_OS_PHONEBOOKAPI:
		{
			param[0]=(int)Apple_Get_pbCont;
			param[1]=(int)ApplePhoneBookGetItem;	
		}
	        return SUCCESS;
			
	 case APPLE_OS_API_IMEI:
		 if(*pnSize<16)
			 return EFAILED;
		 
		 APPLE_OS_GetPhoneID((unsigned char* )pBuff, (unsigned int )*pnSize);
		 *pnSize = 16;
		return SUCCESS;
	case APPLE_CURRENT_IMSI:
		 if(*pnSize<16)
			 return EFAILED;
		 
		 APPLE_OS_GetPhoneID((unsigned char* )pBuff, (unsigned int )*pnSize);
		 *pnSize = 16;
		return SUCCESS;
	case APPLE_OEM_EX_BEGIN + 152: //IMPORTANT

//		if(*pnSize<16)
//			 return EFAILED;
		 
		 OEMOS_GetSysIMEI((unsigned char* )pBuff, 16);
		 *pnSize = 16;
		 
		return SUCCESS;

       case APPLE_OEM_EX_BEGIN + 9:	//获得sim卡1的类型 0移动 1联通 2无效 //IMPORTANT
	   	if(pBuff)
			{
				PHONE_PLMN_NAME_E  NetWorkId = PLMN_TEST;

				NetWorkId = MMIPHONE_GetNetworkName(g_apple_active_sim);
				if(NetWorkId== PLMN_CMCC)
					{
						*(int *)pBuff  = 0;
					}
				else if(NetWorkId == PLMN_UNICOM)
					{
						*(int *)pBuff  = 1;
					}
				else
					{
						*(int *)pBuff  = 2;
					}
		
			}
	   	return SUCCESS;


	

	case (APPLE_OEM_EX_BEGIN + 141):  //IMPORTANT
      {
		if(param)
		{
			param[0]=(int)apple_get_platform_driver;
		}

	}
		 return SUCCESS;
	   	
	 case APPLE_OS_API_PLATFORM:
		{	

#ifdef PLATFORM_SC6600R
			char pltf[] = "SC6600R";
#elif defined(PLATFORM_SC6600L)
			char pltf[] = "SC6600L";
#elif defined(PLATFORM_SC6600H)
			char pltf[] = "SC6600H";
#elif defined(PLATFORM_SC6800H)
			char pltf[] = "SC6800H";
#elif defined(PLATFORM_SC6530)
			char pltf[] = "SC6530";
#endif
			if(*pnSize < strlen(pltf) || NULL==pBuff)
				return EFAILED;
			strcpy(pBuff,pltf);
			*pnSize = (int)strlen(pltf);
		}
		return SUCCESS;
	case APPLE_OS_API_MODEL:
		{	
			if(NULL == pBuff || strlen(APPLE_OS_MODEL) >= *pnSize)
				return EFAILED;
			strcpy(pBuff, APPLE_OS_MODEL);
			*pnSize = strlen(APPLE_OS_MODEL);
		}
		return SUCCESS;
#ifdef __APPLE_ZI_ID__
	case APPLE_OS_RELEASE_HW_VER:
		{
			if(NULL == pBuff || strlen(APPLE_OS_ZI_ID) >= *pnSize)
				return EFAILED;
			
			strcpy(pBuff, APPLE_OS_ZI_ID);
			*pnSize = strlen(APPLE_OS_ZI_ID);
			
		}
       	return SUCCESS;
#endif

	case APPLE_OS_API_SYS_DIR:
		{
			char drive;
			if(NULL == pBuff || APPLE_GCG_PATH_MAX_LEN >= *pnSize)
				return EFAILED;

			#ifdef APPLE_LOG
				OEMOS_dbgprintf("*pnSize %d fff%c", *pnSize,OEMFS_GetDrive(0));
			#endif
			sprintf((char* )pBuff, "%c:\\apple", OEMFS_GetDrive(0));
			*pnSize = strlen(pBuff);
		}
		return SUCCESS;
	case APPLE_OS_API_APN:
		{
			if(pBuff)
				*(int *)pBuff = (int)0; //默认接入点;
		}
		return SUCCESS;

		
	case APPLE_OS_API_DEFAULT_APN:
		{
			if(pBuff)
				*(int *)pBuff = (int)0; //默认接入点;
		}
		return SUCCESS;
	case APPLE_OS_API_CORE_MEMSOUND:
		{			
			if(pBuff)
				*(int *)pBuff = 1;
		}
		return SUCCESS;

	case APPLE_OS_SMSCENTER:
		{
			int active_sim = OEMOS_GetActiveSim();
			uint8 size = 0;
			if(NULL == pBuff || NULL == pnSize )
				return EFAILED;
			
			if( active_sim < 0)
			{
				 *pnSize = 0;
				 return SUCCESS;
			}
			
			if(!MMISMS_AppGetSCAddr(active_sim, pBuff,  &size, MMISMS_STRNUM_MAX_LEN + 2 ))
				 *pnSize = 0;
			else
				*pnSize = (int32)size;
 		}
	       return SUCCESS;

	case APPLE_OS_UCS2SMS:
		 g_apple_ucs2_sms = *(int *)pBuff;
		 
	 return SUCCESS;	

       case APPLE_OS_RELEASE_BUILD_DATETIME: //IMPORTANT
	   	{
			if(pBuff)
			{

				char temp[100]={0};
#ifndef WIN32
				strcpy(temp, PROJECT_BUILD_TIME);
#else
					strcpy(temp, "2011-01-01");
#endif
				apple_os_release_filter(temp);
				strcpy((char *)pBuff, temp);
			}	

       	}

	   return SUCCESS;
	  //sms hook，表示通过我们测试二次的平台测试OK后用此宏__SXMOBI_PLATFORM_OK__
     	case (APPLE_OEM_EX_BEGIN + 133)://IMPORTANT
     		{
     			*(int *)pBuff = __SXMOBI_PLATFORM_OK__;
     		
	     	}
	return SUCCESS;	

	case (APPLE_OEM_EX_BEGIN + 153):  // SPT mp3streamplayer
		{
			*(int *)pBuff = 1;
		}
	return SUCCESS;

	case (APPLE_OEM_EX_BEGIN + 154): // SPT create scan cnfile
		{
			*(int *)pBuff = 1;
		}
	return SUCCESS;
	
	
	 case (APPLE_OEM_EX_BEGIN + 115):   //IMPORTANT
	 	{

		param[0] = (int)apple_chset_text_to_ucs2_str;
		param[1] = (int)apple_chset_ucs2_to_text_str;
     		}	
	 	return SUCCESS;	

 	case (APPLE_OEM_EX_BEGIN + 97): //IMPORTANT
	 {
			*(int *)pBuff = (int)APPLE_enable_idle_key;
      	 }
	return SUCCESS;	
 
	case APPLE_MINIAPM_ALWAYS_BACKLIGHT : //IMPORTANT
		{
			if(pBuff)
			*(int *)pBuff = 1; 
		}
		return SUCCESS;
//mini entry
	case (APPLE_OEM_EX_BEGIN + 173):
		{
			param[0] = (int)g_apple_start_type;
			param[1] = (int)g_apple_start_mini_id;
#ifdef  APPLE_LOG
                           OEMOS_dbgprintf("APPLE_MINIAPM_GET_ID_INFO type: %d id: %d", g_apple_start_type,g_apple_start_mini_id);
#endif

		}
	    return SUCCESS;
//mini entry

  	case APPLE_APPMAN_ALWAYS_BACKLIGHT : //IMPORTANT
		{
			if(pBuff)
			*(int *)pBuff = 1; 
		}
		return SUCCESS;		
	case APPLE_MINIAPM_CHECK_SPACE:
		{
			if(pBuff)
			{
				uint32 size = 0;
				OEMFS_GetFreeSpace(&size);
#ifdef  APPLE_LOG	
                           OEMOS_dbgprintf("APPLE_MINIAPM_CHECK_SPACE size %d", size);
#endif
				if(size ==  0)
					*(int *)pBuff = 1; 
				else
					*(int *)pBuff = size;
			}
		}
		return SUCCESS;
	   
/*
	case APPLE_OS_API_YUV_SUPPORT:
		{
			int yuv=0;
			if(*pnSize < sizeof(yuv))
				return 1;
			*(int *)pBuff = 1;
		}
		return SUCCESS;
*/
	case APPLE_OS_API_LCDWIDTH:		
		{
			if(pBuff)	//	注意传入buf长度，字节序问题
			{

				if(2 == *pnSize)
					*(short *)pBuff = OEMOS_GetLCDWidth();//LCD_WIDTH;//
				else if(4 == *pnSize)
					*(int *)pBuff = OEMOS_GetLCDWidth();//LCD_WIDTH;//OEMOS_GetLCDWidth();
#ifdef APPLE_DISPLAY_LOG
				OEMOS_dbgprintf("APPLE_OS_API_LCDWIDTH   w:%d",OEMOS_GetLCDWidth());
#endif
			}
		}
		return SUCCESS;
	case APPLE_OS_API_LCDHEIGHT:	
		{
			if(pBuff)	//	注意传入buf长度，字节序问题
			{
				if(2 == *pnSize)
					*(short *)pBuff = OEMOS_GetLCDHeight();//LCD_HEIGHT;//OEMOS_GetLCDHeight();
				else if(4 == *pnSize)
					*(int *)pBuff = OEMOS_GetLCDHeight();//LCD_HEIGHT;//OEMOS_GetLCDHeight();
#ifdef APPLE_DISPLAY_LOG
				OEMOS_dbgprintf("APPLE_OS_API_LCDHEIGHT   w:%d",OEMOS_GetLCDHeight());
#endif
			}
		}
		return SUCCESS;
 	case APPLE_OS_API_LCDROTATE:
		{
			if(pBuff)
				*(int *)pBuff = 0;
 		}
 		return SUCCESS;
	case APPLE_OS_SUPPORTNEWAA_API: //是否支持电话模式 0 不支持
		{
			if(pBuff)
				*(int *)pBuff = 0;	
		}
	        return SUCCESS;
			
 	case APPLE_OS_LCD_FASTUPDATE:  // 快速刷屏
		{
			if(pBuff)
				*(int *)pBuff = 0;	
 		}
 		return SUCCESS;
	case APPLE_OS_API_SCR_ROTATE:
		{
			if(pBuff)
			{
			#ifdef APPLE_ROTATE_DYNAMIC
				  *(int *)pBuff =MMK_GetScreenAngle();
				  OEMOS_dbgprintf("APPLE_OS_API_SCR_ROTATE  rot:%d",MMK_GetScreenAngle());
			#else
				  *(int *)pBuff =0;
			#endif
			}
		}
		return SUCCESS;	
	case APPLE_OS_API_VP_FULLSCREEN_R:
		{
			if(pBuff)
				*(int *)pBuff = 1;	// turn right 
		}
		return SUCCESS;
	case APPLE_OS_API_VP_SETCONFIG:
		{
			if(NULL != pBuff )
			{
				((int*)pBuff)[0] = 600;    //    最大缓存大小
				((int*)pBuff)[1] = 400;    //    默认缓存大小
			}
		}
		return SUCCESS;
	case APPLE_OS_API_RP_SUPPORTAUDIO:
		{
			if(pBuff)
				//*(int *)pBuff = 0x1|0x02;	// arm|mp3
				*(int *)pBuff = 0x1;	// arm
		}
		break;
#ifdef __UM_DUAL_SENSOR__		
	case APPLE_OS_DCSWITCH:		//支持切换,双摄像头1支持,0不支持
		{
			if(pBuff != NULL)
			{
				if(OEMCAM_IsSecond())
					*param = 1;
			}
		}
		return SUCCESS;
#endif

#ifdef __UM_TURNOVER_SENSOR__
case APPLE_UMVCHAT_GetSpt_ModifyCamera:
	{

		   *param=(int)APPLE_ModifyCamera;
       }
       return SUCCESS;
#endif
case APPLE_SMS_CONVERTER:
	{
	*(int *)pBuff = (int)sms_content_converter;
	}
	return SUCCESS;

/*
	case APPLE_UMVCHAT_HOTKEY:
		{
			int i=0;
			int cnt = 0;
			CHotKeyInfo* pKey = (CHotKeyInfo *)pBuff;
			if(NULL != pKey || NULL ==pnSize)
				break;
			cnt = *pnSize;
			for(i=0; i<cnt; i++)
			{
				switch(pKey[i].iOpCode)
				{
				case EVChatOpCode_Mic1:	//抢麦1,默认按键是AVK_VOLUME_UP
					pKey[i].iKeyCode = AVK_VOLUME_UP;
					break;
				case EVChatOpCode_Mic2:	//抢麦2，默认按键是AVK_VOLUME_DOWN
					break;
				case EVChatOpCode_FireMsg:	//提取消息,默认按键是AVK_5,描述语是5
					break;
				case EVChatOpCode_SwitchCam://切换摄像头,默认按键是AVK_STAR
					break;
				case EVChatOpCode_ReduVol:	//降低音量，默认按键是AVK_9
					pKey[i].iKeyCode = AVK_9;
					break;
				case EVChatOpCode_AddVol:	//增加音量，默认按键是AVK_3
					pKey[i].iKeyCode = AVK_3;
					break;
				}
			}
		}
		break;
*/
	case APPLE_UMVCHAT_RecordKeyTip:
		{
			if(pBuff)
				*(int *)pBuff = -1;		//不提示
		}
		break;
		case APPLE_OS_SUPPORT_HighDef:   //是否支持高清模式，0不支持，1支持，默认支持
		{
			if(pBuff != NULL)
			{
				*param = 0;
			}
		}
		return SUCCESS;
	case APPLE_UMVCHAT_RotateClip:	//是否需要软件旋转camera数据（展讯），返回旋转角度0不旋转190,  2为180, 3为270
		{
			if(pBuff != NULL)
			{
			
				//OEMOS_dbgprintf("APPLE_UMVCHAT_RotateClip  MMK_GetScreenAngle:%d",MMK_GetScreenAngle());

			#ifdef APPLE_ROTATE_DYNAMIC
				if(APPLE_GetCurrAppID() == 0x0300009A)	//如果是视频聊天，可以转
					*param = (4 - MMK_GetScreenAngle())%4;
				else
					*param = 0;
			#else
				*param = 0;
			#endif
			}
		}
	return SUCCESS;

	case APPLE_OS_SUPPORT_MULTI_ONLYOPENRO:	 //多人聊天时只有房主能够avopen, 1只有房主支持，0全部支持，默认全部支持 
		{
			if(pBuff != NULL)
			{
//				int * param = (int *)pBuff; 
				*param = 1;
			}
		}
	return SUCCESS;
	case APPLE_OS_SUPPORT_MULTI: //0不支持多人聊天，1支持
		{
			if(pBuff != NULL)
			{
				*(int*)pBuff = 1;	
			}
		}
		return SUCCESS;
	case APPLE_UMVCHAT_StopVInputMode: //在输入法界面是否停止视频，1：停止，0：不停止，默认不停止
		{
			if(pBuff != NULL)
			{
				*(int*)pBuff = 1;   
			}
		}
		return SUCCESS;
	case APPLE_UMVCHAT_ROOM_SUP_OPENOWNER://进入固定房间是否打开房主视频，1：打开，0：不打开，默认是1：打开 
		{
			if(pBuff != NULL)
			{
				*(int*)pBuff = 1;   
			}
		}
		return SUCCESS;

	case APPLE_UMVCHAT_MULTIPERIOD: //  //绘制周期110ms 倍数，最小1倍，最大5倍，默认为1
		{
			if(pBuff != NULL)
			{
				*(int*)pBuff = 3;   
			}
		}
		return SUCCESS;
#ifdef SXH_APPLE_YOUXIN	
#ifdef APPLE_SYS_MOTION
	case (SXM_MOTION_API_ID):
	{
		int *param = (int *)pBuff;
		param[0] = (unsigned int)apple_system_motion_power_on;
		param[1] = (unsigned int)apple_system_motion_power_off;
		param[2] = (unsigned int)apple_system_motion_listen;
		param[3] = (unsigned int)apple_system_motion_stop_listen;
		param[4] = (unsigned int)apple_system_motion_get_max_acc;
	}
	break;
#endif
#endif

/*	
    case APPLE_OS_API_VP_NewEE:
        {
            if(pBuff)
            {
                *(int *)pBuff = (int)vceres;
            }
        }	
        break;
*/

    case APPLE_OS_API_VP_15x:
        {
		char mod_name[128]={0};
		unsigned int len = 0; 
			
		if(leo1_mod== NULL)
		{
			memset(mod_name,0,sizeof(mod_name));
			sprintf(mod_name, "%c:\\apple\\sys\\leo1.mod", OEMFS_GetDrive(0));
			leo1_mod = OEM_ReadFile((unsigned char *)mod_name, &len, 0,0);
		}
#ifdef APPLE_LOG
		OEMOS_dbgprintf("APPLE_OS_GetDeviceCapExLib leo1 %p  \r\n", leo1_mod);
#endif			


		if(leo2_mod== NULL)
		{
			memset(mod_name,0,sizeof(mod_name));

			sprintf(mod_name, "%c:\\apple\\sys\\leo2.mod", OEMFS_GetDrive(0));
			leo2_mod = OEM_ReadFile((unsigned char *)mod_name, &len, 0,0);
		}
#ifdef APPLE_LOG
		OEMOS_dbgprintf("APPLE_OS_GetDeviceCapExLib leo2 %p  \r\n", leo2_mod);
#endif			


            if(pBuff)
            {
                param[0]=(int)leo1_mod;
                param[1]=(int)leo2_mod;
            }
        }
        break;  	

	  case APPLE_OS_API_VP_SUPPORTSVGIV:
        {

#ifdef  APPLE_DUBUG
            if(pBuff)
                *(int *)pBuff = (int)SXMOBI_SVGIV;
#else
						char mod_name[128]={0};
						unsigned int len = 0; 
			
						if(svgiv_lib== NULL)
						{
							sprintf(mod_name, "%c:\\apple\\sys\\svg.mod", OEMFS_GetDrive(0));
							svgiv_lib = OEM_ReadFile((unsigned char *)mod_name, &len, 0,0);
						}
#ifdef APPLE_LOG
						OEMOS_dbgprintf("APPLE_OS_GetDeviceCapExLib svgiv_lib %p  \r\n", svgiv_lib);
#endif			
				if(pBuff)
					*(int *)pBuff = (int)svgiv_lib;

#endif
        }
        break;  	
        
/*		
     	case APPLE_UMVCHAT_DEFAULT_VIDEOCNT: //	多人聊天打开的视频数默认打开4路
		{
			*(int*)pBuff = 1;	
		}
		break;
*/		
// 	case APPLE_OS_API_MP_SIM:
// 		return SUCCESS;
// 	case APPLE_OS_API_GAME_CLOCK:
// 		return SUCCESS;
// 	case APPLE_OS_API_CORE_SIM1:
// 		return SUCCESS;
// 	case APPLE_OS_API_CORE_SIM2:
// 		return SUCCESS;		
// 	case APPLE_CURRENT_IMSI:
// 		return SUCCESS;
// 	case APPLE_CURRENT_OPERATOR:
// 		return SUCCESS;
	default:
		return EFAILED;
     }
     return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//			APPLE APP CONFIG (SYSTEM & APPLE)							//
//////////////////////////////////////////////////////////////////////////
const OEM_NA_Info gNAAppInfo[] =
{
	{0,0,NULL,NULL }
};

#include "apple_appdata_umplayer.h"

const apple_preinstal_appinfo apple_pre_app[] = 
{
	{0x01F00001,0,0,0,"sys","apple.sig","apple.bin",0,NULL},

	{0x03000000,0,0,0,"appman","appman.sig","appman.bin",0, (void*)NULL},

 	{0x03000098,0,0,0,"miniapm","miniapm.sig","miniapm.bin",0, (void*)apple_app_miniapm},


	{0x03000041,0,0,0,"videoplayer","videoplayer.sig","videoplayer.bin",0,(void*)NULL},

	{0x03000093,0,0,0,"radioplayer","radioplayer.sig","radioplayer.bin",0,NULL},
	{0x0300009A,0,0,0,"umvchat","umvchat.sig","umvchat.bin",0,NULL},


#ifdef SXH_APPLE_3GVEDIO_SUPPORT
	{0x0300009E,0,0,0,"umvchatthin","umvchatthin.sig","umvchatthin.bin",0,NULL},
#endif	
#ifdef SXH_APPLE_UMVCHAT_DEMO
	{0x0300009c,0,0,0,"umvchatdemo","umvchatthindemo.sig","umvchatthindemo.bin",0,(void*)apple_app_umvchatdemo},
#endif	

	{0x030000A8,0,0,0,"ummusic","ummusic.sig","ummusic.bin",0,NULL},
	{0x030000ED,0,0,0,"umbook","umbook.sig","umbook.bin",0,NULL},


		//tlbb
	{0x030000A9,0,0,0,"videodl","videodl.sig","videodl.bin",0,NULL},
	{0x03000026,0,0,0,"msweather","msweather.sig","msweather.bin",0,NULL},	

	{0x03000302,0,0,0,"ummsg","ummsg.sig","ummsg.bin",0,NULL},	

	//lwsg
	{0x030000B0,0,0,0,"lwsg_240320","lwsg_240320.sig","lwsg_240320.bin",0,NULL},	
#ifdef UMPLAYER_ROM	
	{0x03000042,0,0,0,"umplayer","umplayer.sig","umplayer.bin",0,(void*)apple_app_umplayer},
#else
	{0x03000042,0,0,0,"umplayer","umplayer.sig","umplayer.bin",0,NULL},
#endif
	{0,0,0,0,0,0,0,0,NULL}
};








//	to restore app from ROM
int apple_preapp_is_in_rom(unsigned int app_id)
{
	int i = 0;
	while(apple_pre_app[i].appid != 0)
	{
		if(apple_pre_app[i].appid == app_id && apple_pre_app[i].pAppData==NULL)
			return apple_pre_app[i].inrom;
		i++;
	}
	return 0;
}
//	to get static app entry
void* apple_preapp_get_entry(unsigned int app_id)
{
	int i = 0;
	while(apple_pre_app[i].appid != 0)
	{
		if(apple_pre_app[i].appid == app_id)
		{
			return apple_pre_app[i].pAppData;
		}
		i++;
	}
	return NULL;
}

void OEMNA_InitNa(void)
{
}
boolean OEMNA_IsSameNa( uint16 usID1, uint16 usID2 )
{
	// 	int32 iNaNum = 0;
	int32 i = 0;
	
	if(usID1 == usID2)
		return 1;
	//	iNaNum = sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1;
	while(NULL != gNAAppInfo[i].pName)
	{
		if(gNAAppInfo[i].usScrBaseID >usID1 &&  gNAAppInfo[i].usScrMaxID <usID1)
		{
			if(gNAAppInfo[i].usScrBaseID >usID2 &&  gNAAppInfo[i].usScrMaxID <usID2)
				return 1;
		}
		i++;
	}
	return 0;
}
int32 OEMNA_GetNaNum( void )
{
	int32 i = 0;
	while(NULL != gNAAppInfo[i].pName)
		i++;
	return i;
	//	return (sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1);
}
 uint16 OEMNA_GetNaID( int32 iIdx )
{
	int32 iNaSize = sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1;
	if(iNaSize<=iIdx ||  0>iIdx)
		return 0;
	return (gNAAppInfo[iIdx].usScrBaseID);
}
int32 OEMNA_GetNaIdx( uint16 usID )
{
	// 	int32 iNaNum = OEMNA_GetNaNum();//sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1;
	int32 i = 0;
	while(NULL != gNAAppInfo[i].pName)
	{
		// 		i = iNaNum - 1;
		if(gNAAppInfo[i].usScrBaseID<=usID &&  usID<=gNAAppInfo[i].usScrMaxID)
		{
			return i;
		}
		i++;
		/*		iNaNum--;*/
	}
	return -1;
}
char* OEMNA_GetNaName( int32 iIdx)
{
	int32 iNaSize = sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1;
	
	if(iNaSize<=iIdx ||  0>iIdx)
		return NULL;
	return gNAAppInfo[iIdx].pName;
}
PFNAENTRY OEMNA_GetNaEntry( int32 iIdx)
{
	int32 iNaSize = sizeof(gNAAppInfo)/sizeof(OEM_NA_Info) - 1;
	
	if(iNaSize<=iIdx ||  0>iIdx)
		return NULL;
	return gNAAppInfo[iIdx].pfNAEntry;	
}


//////////////////////////////////////////////////////////////////////////
////			SYSTEM EVENT HANDLE										//
//////////////////////////////////////////////////////////////////////////



#define  OEMOS_MEDIAPLAYER_MSG    222


typedef void   (*Media_player_cb)(unsigned int,unsigned int);
static Media_player_cb  s_Mediaplayer_Callback = NULL;

void OEMOS_send_mediaplayer_msg(unsigned int data1, unsigned int data2,void* callback)
{
	s_Mediaplayer_Callback = (Media_player_cb)callback;
	OEMOS_SendOsMsg(OEMOS_MEDIAPLAYER_MSG,data1,data2);
	
}

MMI_RESULT_E OEMOS_HandleAppleExtMsg(
									PWND				app_ptr, 
									uint16				msg_id, 
									DPARAM				param
									)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;

    //IMPORTANT
	if( (!AppleIsRun()) &&  (!SXHHttp_IsActive()))
	{
		return result;
	}
	
	switch (msg_id)
    	{
    		case APP_APPLE_END_IND:
			{
				apple_msg_struct *msg = (apple_msg_struct*)((uint32)param);
				if( OEMOS_MEDIAPLAYER_MSG ==   msg->op_code)
				{
					#ifdef APPLE_LOG	
					OEMOS_dbgprintf("OEMOS_MEDIAPLAYER_MSG ==   msg->op_code ");
					#endif
					s_Mediaplayer_Callback(msg->data1,msg->data2);
				}
				else	
					AppleMsgInd((void*)((uint32)param));
    			}
			break;
		default:
			break;
	}
	return (result);
}
MMI_APPLICATION_T   g_apple_app = {OEMOS_HandleAppleExtMsg,0,0};   
extern uint32 iAppId ;
uint8 cur_value = 2;


void Sxmobi_GetRotatePoint(apple_point *pos,DPARAM param)
{
	//OEMOS_dbgprintf("Sxmobi_GetRotatePoint   MMK_GetScreenAngle() %d,pt(%d,%d)",MMK_GetScreenAngle(),MMK_GET_TP_X(param),MMK_GET_TP_Y(param));

#if 0
	if(MMK_GetScreenAngle() == 1)//303,158
	{
		pos->x = LCD_HEIGHT - MMK_GET_TP_Y(param);
		pos->y = MMK_GET_TP_X(param);
	}
	else if(MMK_GetScreenAngle() ==2 )
	{
		pos->x = LCD_WIDTH - MMK_GET_TP_X(param);
		pos->y = LCD_HEIGHT - MMK_GET_TP_Y(param);

	}
	else if(MMK_GetScreenAngle() == 3 )
	{
		pos->x = MMK_GET_TP_Y(param);
		pos->y = LCD_WIDTH - MMK_GET_TP_X(param);
	}
	else
#endif
	{
		pos->x = MMK_GET_TP_X(param);
		pos->y = MMK_GET_TP_Y(param);
	}
}

unsigned int  SXMOBI_GetOemVer()
{
    return 11022116;
}





LOCAL unsigned int  s_apple_net_id= 0; 
LOCAL int g_apple_update_enable =0;
void OEMOS_ClearScreenToBlack()//sxh az
{
    apple_color cl = {0};
    apple_rect clipRect = {0,0,LCD_WIDTH,LCD_HEIGHT};
    OEMBitmap_FillRectangle(0,0,LCD_WIDTH,LCD_HEIGHT,cl,clipRect);
    OEMBitmap_LCDupdate(&clipRect);
}
//#ifdef PDA_UI_SUPPORT_ANDROID//sxh az
#if  1//def MMI_PDA_SUPPORT
void OEMOS_SetHideState(MMI_WIN_ID_T win_id)//sxh az
{
    IGUICTRL_T  * ctrl_handle=PNULL;
    ctrl_handle=MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(),win_id),MMITHEME_GetStatusBarCtrlId());
    if(0 !=ctrl_handle)
    {
        IGUICTRL_SetState(ctrl_handle,GUICTRL_STATE_DISABLE_ACTIVE|GUICTRL_STATE_INVISIBLE|GUICTRL_STATE_DISABLE_TP,TRUE);
    }
}
#endif
// jxh add by 20111018
int OEMOS_GetMiniAppmanVer()
{
#if defined  APPLE_MINIAPPMAN_A   // 简单版本
		return 1;
#elif  defined  APPLE_MINIAPPMAN_B // 彩色版本
		return 2;
#elif defined  APPLE_MINIAPPMAN    // 动态菜单
	      return 3;
#endif	
}
extern  int SXHHttp_IsActive();
extern unsigned int  APPLE_GetCoreVerLib();
void OEMDisplay_FreeRotateBuf();
void OEMOS_Delapn();
extern void OEM_Head_MarkFree();
MMI_RESULT_E OEMOS_HandleAppleWinMsg(
							   MMI_WIN_ID_T win_id, 
							   MMI_MESSAGE_ID_E msg_id, 
							   DPARAM param
							   )
{
	uint32           button_num    = 0;
	GUI_RECT_T       rect          = {0};
	MMI_RESULT_E     recode        = MMI_RESULT_TRUE;
	//GUI_BUTTON_BG_T  button_bg     = {GUI_BUTTON_BG_NULL, 0, 0, 0, 0};
	#ifdef APPLE_LOG	
	OEMOS_dbgprintf("OEMOS_HandleAppleWinMsg MSG: 0x%x\r\n ", msg_id);
	#endif
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		#ifdef APPLE_LOG	
		OEMOS_dbgprintf("MSG_OPEN_WINDOW, win_id:0x%x, APPLE_MAIN_WIN_ID:0x%x,iAppId:%d,Apple_GetCurStartAPPID:%d", win_id, APPLE_MAIN_WIN_ID,iAppId,Apple_GetCurStartAPPID());

		OEMOS_dbgprintf("MSG_OPEN_WINDOW  g_apple_active_sim:%d  s_apple_net_id:%d",g_apple_active_sim,s_apple_net_id);
		#endif
        #if 1//def MMI_PDA_SUPPORT//sxh az
		OEMOS_SetHideState(win_id);//sxh az
	#endif
		gAppleWinId = win_id ;
#ifdef APPLE_LOG  
        OEMOS_dbgprintf("MSG_OPEN_WINDOW Globak Size %d\r\n", OEMHEAP_GlobalSize());
		////防止启动的时候不把屏幕旋转成0度时，某些应用花屏		

		OEMOS_dbgprintf("OEMOS_GetLCDWidth:%d,OEMOS_GetLCDHeight:%d", OEMOS_GetLCDWidth() , OEMOS_GetLCDHeight());
#endif
/*
// test jxh 2012-03-06
		OEMOS_testDiskCreate();

// end test
*/

		if(MMK_GetScreenAngle()%2 != 0 )////防止启动的时候不把屏幕旋转成0度时，某些应用花屏
		{
			OEMOS_ClearScreenToBlack();
		}
#if 0
		if(0x030000B0 == Apple_GetCurStartAPPID()   && (OEMOS_GetLCDWidth() > OEMOS_GetLCDHeight()))
		{
			MMI_STRING_T            error_text = {0};
			if(OEMOS_GetLangId() == 86 )
			{
				const wchar U_ch[] = { 0x4E0D, 0x652F, 0x6301, 0x8BE5, 0x5206, 0x8FA8, 0x7387, 0 }; //不支持该分辨率
				error_text.wstr_ptr = U_ch;
				error_text.wstr_len = sizeof(U_ch)/sizeof(wchar);
			}
			else
			{
				const wchar U_en[] = { 0x0044, 0x006F, 0x0020, 0x006E, 0x006F, 0x0074, 0x0020, 0x0073, 0x0075, 0x0070, 0x0070, 0x006F, 0x0072, 0x0074, 0x0020, 0x0074, 0x0068, 0x0065, 0x0020, 0x0072, 0x0065, 0x0073, 0x006F, 0x006C, 0x0075, 0x0074, 0x0069, 0x006F, 0x006E, 0 }; //Do not support the resolution
				error_text.wstr_ptr = U_en;
				error_text.wstr_len = sizeof(U_en)/sizeof(wchar);
			}
			MMIPUB_OpenAlertWinByTextPtr(PNULL,&error_text,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			MMK_CloseWin(gAppleWinId);
			return MMI_RESULT_FALSE;
		}
#endif
// jxh add by 20111018
#ifdef APPLE_MINIAPPMAN
        // check update dms
#ifndef CHECK_UPDATE_DMS_BY_MINIAPM
        #error "please check update miniappman for dms"
#endif
#endif
		if(SXMOBI_GetOemVer() != APPLE_GetCoreVerLib())
		{
			MMI_STRING_T    no_dms_text = {0};
			wchar no_dms_text_buf[] = { 0x0070, 0x006C, 0x0065, 0x0061, 0x0073, 0x0065, 0x0020, 0x0063, 0x0068, 0x0065, 0x0063, 0x006B, 0x0020, 0x0075, 0x0070, 0x0064, 0x0061, 0x0074, 0x0065, 0x0020, 0x006D, 0x0069, 0x006E, 0x0069, 0x0061, 0x0070, 0x0070, 0x006D, 0x0061, 0x006E, 0x0020, 0x0066, 0x006F, 0x0072, 0x0020, 0x0064, 0x006D, 0x0073, 0 }; //please check update miniappman for dms
			no_dms_text.wstr_ptr = no_dms_text_buf;
			no_dms_text.wstr_len = sizeof(no_dms_text_buf)/sizeof(wchar);
			MMIPUB_OpenAlertWinByTextPtr(PNULL,&no_dms_text,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			MMK_CloseWin(gAppleWinId);
			return MMI_RESULT_FALSE;
		}

		if (Apple_GetCurStartAPPID() != 0x0300009c  && Apple_GetCurStartAPPID() !=  0x03000042)
		{
				//	check storage 
#ifdef SXH_SPT_H7810
				if (MMIUDISK_UdiskIsRun() ) 
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_SD_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#else
				if (MMIAPIUDISK_UdiskIsRun() ) 
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
					MMK_CloseWin(gAppleWinId);
					return MMI_RESULT_FALSE;
				}
		/*		
				else if(!MMIAPISD_GetStatus())
				{
					 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					MMK_CloseWin(gAppleWinId);
					return MMI_RESULT_FALSE;
				}
				
		*/	
				//	check sim
				if(OEMOS_GetActiveSim()<0)
				{
					if(MMIAPISET_GetFlyMode()) // 飞行模式
					    MMIAPISET_AlertFlyMode();
					else
					   MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

					MMK_CloseWin(gAppleWinId); 
					return MMI_RESULT_FALSE;
				}
		}
		OEMOS_Addapn();

#ifdef __UM_DUAL_SENSOR__
		if(OEMCAM_IsSecond())
		{
			g_iCurCamState = DCAMERA_IMG_SENSOR_SUB;
		}
#endif

//	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_ENVSET, FALSE);

#if !defined(SXH_SPT_H7810)
//		MMI_SetFreq(FREQ_INDEX_APP_HIGH);	//full speed
#endif

//		MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_MPEG4);

		OEMOS_pauseMp3();
		cur_value = MMIAPIENVSET_GetCurModeOptValue(g_apple_active_sim,TKEY_RING_TYPE);
		if(cur_value == 0)
			MMIENVSET_SetCurModeOptValue(TKEY_RING_TYPE,(uint8)1, g_apple_active_sim);
		
		g_apple_bklt = 0;

		g_isAppleGB = 0;
		
		g_apple_ps_mode = 0;
//============== __MMI_APPLE__	start ===================
		OEMOS_Interrupt = 0;
//============== __MMI_APPLE__	  end ===================
		g_up = 0;
		g_keydown_cont = 0;
		g_apple_update_enable =0;

		break;
#ifdef APPLE_ROTATE_DYNAMIC
	case MSG_LCD_SWITCH:
		{
			AppleScrRotate(MMK_GetScreenAngle());	
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("MSG_LCD_SWITCH  rot:%d",MMK_GetScreenAngle());
			#endif
			OEMDisplay_set_rotate(MMK_GetScreenAngle());
			return MMI_RESULT_TRUE;
		}
#endif
	case MSG_CLOSE_WINDOW:
#ifdef SXH_APPLE_YOUXIN	
	#ifdef APPLE_SYS_MOTION
		apple_system_motion_power_off();//sxmobi tong
	#endif
	#endif
		
		// OEM_DISP_FreeMEM(); //LIUHONGMOU
		//DC_VideoDisplayClose();

		OEMDisplay_FreeRotateBuf();
		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("MSG_CLOSE_WINDOW\r\n");
		#endif
		OEMOS_Delapn();
		AppleExit();
		gAppleWinId = 0;
#if !defined(SXH_SPT_H7810)
//		MMI_RestoreFreq();	
#endif


//	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_ENVSET, TRUE);
#if 0
		MMIDEFAULT_EnableKeyRing(TRUE);
#if !defined(SXH_SPT_H7810) && !defined(SXH_SPT_0839)
		MMIDEFAULT_EnableTpRing(TRUE);
#endif
#endif
//		MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MPEG4);

		if(cur_value == 0)
			MMIENVSET_SetCurModeOptValue(TKEY_RING_TYPE,(uint8)0, g_apple_active_sim);


		if(s_SXMOBI_audio_handle)
			MMISRVMGR_Free(s_SXMOBI_audio_handle);



		OEMOS_resumeMp3();

		
		s_SXMOBI_audio_handle = 0;
		

		cur_value = 2;
		
		g_apple_bklt = 0;
		g_isAppleGB = 0;

		g_apple_ps_mode = 0;

/*		
		// test jxh 2012-03-06
				OEMOS_testDiskDel();
		
		// end test
*/		
		OEM_Head_MarkFree();

#ifdef APPLE_LOG        
        OEMOS_dbgprintf("MSG_CLOSE_WINDOW Globak Size %d\r\n", OEMHEAP_GlobalSize());
#endif


		break;
	case MSG_FULL_PAINT:
// 		OEMOS_dbgprintf("MSG_FULL_PAINT\r\n");
		break;
	case MSG_BACKLIGHT_TURN_ON:
		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("MSG_BACKLIGHT_TURN_ON\r\n");
		#endif
		g_apple_bklt = 0;
		break;
	case MSG_BACKLIGHT_TURN_OFF:
		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("MSG_BACKLIGHT_TURN_OFF\r\n");
		#endif
		g_apple_bklt = 1;		// do not lose focus, sync-process
		break;
	case MSG_GET_FOCUS:
        g_apple_update_enable=0;
               #if 1//def MMI_PDA_SUPPORT//sxh az
		   OEMOS_SetHideState(win_id);//sxh az
		 #endif		
 		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("MSG_GET_FOCUS\r\n");
		#endif
		
		if(OEMOS_Interrupt)
		{
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("OEMOS_Interrupt = 0 \r\n");
			#endif
			OEMOS_Interrupt = 0;
			break;
		}
    
   // OEMOS_ClearScreenToBlack();

#if 0 //liuhongmou
#if !defined(SXH_SPT_H7810) && !defined(SXH_SPT_0839)
		MMIDEFAULT_EnableTpRing(FALSE);
#endif
		MMIDEFAULT_EnableKeyRing(FALSE);	//turn off key tune
#endif
		//MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_ENG_TEST, FALSE);
	

#if !defined(SXH_SPT_H7810)
//		MMI_SetFreq(FREQ_INDEX_APP_HIGH);	//full speed
#endif
	
		AppleToFG();
		OEMNET_ResumeTimer();
		g_isAppleGB = 0;
        	break;
	case MSG_LOSE_FOCUS:
        g_apple_update_enable=1;
 		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("MSG_LOSE_FOCUS\r\n");
		#endif
		g_up = 0;
		g_keydown_cont = 0;
		if(MMK_IsFocusWin(MMIAPPLE_EDITBOX_WIN_ID))
		{
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("OEMOS_Interrupt = 1 \r\n");
			#endif
			
			OEMOS_Interrupt = 1;
			break;
		}

#if 0
		if(g_apple_bklt)
		{
			g_apple_bklt = 0;
			break;
		}
#endif		
		OEMNET_PauseTimer();
#ifdef APPLE_ROTATE_DYNAMIC
		AppleScrRotate(0);	
#endif
		AppleToBG();
		g_isAppleGB = 1;
		
#if 0
		MMIDEFAULT_EnableKeyRing(TRUE);
#if !defined(SXH_SPT_H7810) && !defined(SXH_SPT_0839)
		MMIDEFAULT_EnableTpRing(TRUE);
#endif
#endif
		//MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_ENVSET, TRUE);

		break;
	case MSG_TIMER:
		{
			uint8 timerid = *( uint8*)param;
#ifdef APPLE_USE_MMK_TIMER
			extern uint8 g_iAppleSocTimerId;
			uint8 timerid = *( uint8*)param;
			if(g_iAppleTimerId == timerid)
			{		
//				OEMOS_dbgprintf("MSG_TIMER, g_iAppleTimerId:%d, IN", g_iAppleTimerId);
				AppleTimerExpiredInt(0);
			}
			else if(g_iAppleSocTimerId == timerid)
			{				
//				OEMOS_dbgprintf("MSG_TIMER, g_iAppleSocTimerId:%d, IN", g_iAppleSocTimerId);
				OEMNET_SocTimerExpiredInt();
			}
			
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("MSG_TIMER, timerid:%d, OUT", timerid);
			#endif
#endif
#ifdef SXH_APPLE_YOUXIN	
#ifdef APPLE_SYS_MOTION
		if( timerid  == motion_timer )  //sxmobi tong
		{
		 if(apple_motion_timer_on)
		 	{
			 	apple_motion_timer_on = FALSE;
				MMK_StopTimer(motion_timer);
				system_motion_on_timer(g_apple_User);
		 	}
		}
#endif
#endif
		}
		break;
	case APPLE_BASE_MSG_ID:
		AppleMsgInd(param);
		break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
		{
			apple_point pos;
			Sxmobi_GetRotatePoint(&pos,param);
			//OEMOS_dbgprintf("MSG_TP_PRESS_DOWN,x=%d,y=%d\r\n ",pos.x,pos.y);
			ApplePenDown(pos);
		}
		break;

	case MSG_TP_PRESS_UP:
		{
			apple_point pos;
			Sxmobi_GetRotatePoint(&pos,param);
			//OEMOS_dbgprintf("MSG_TP_PRESS_UP,x=%d,y=%d\r\n ",pos.x,pos.y);
			ApplePenUp(pos);
		}
		break;

	case MSG_TP_PRESS:
	case MSG_TP_PRESS_MOVE:
		{
			apple_point pos;
			Sxmobi_GetRotatePoint(&pos,param);
			//OEMOS_dbgprintf("MSG_TP_PRESS_MOVE,x=%d,y=%d\r\n ",pos.x,pos.y);
			ApplePen(pos);
		}
		break;

	case MSG_TP_PRESS_LONG:
		{
			apple_point pos;
			Sxmobi_GetRotatePoint(&pos,param);
			//OEMOS_dbgprintf("MSG_TP_PRESS_LONG,x=%d,y=%d\r\n ",pos.x,pos.y);
			ApplePenHeld(pos);
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_KEYDOWN_FLIP:
			recode = MMI_RESULT_FALSE;
		break;
 	case MSG_KEYDOWN_RED:
    case MSG_KEYUP_RED: //jxh add  20120410 
        {
    		if(g_enable_idle_key)
     			recode = MMI_RESULT_FALSE;
    		else
    			recode = MMI_RESULT_TRUE;
        }
 		break;
	case MSG_KEYLONG_POWER://sxh az 20120508
		{
			recode = MMI_RESULT_FALSE;
		}
		break;
	case MSG_KEYUP_POWER:
	case MSG_KEYDOWN_POWER:
		if(Apple_GetCurStartAPPID()==0x03000042)
 			recode = MMI_RESULT_TRUE;
		else
			{
				MMK_SendMsg(win_id,MSG_LOSE_FOCUS,PNULL);//sxh az 20120508
				recode = MMI_RESULT_FALSE;
			}
		break;
	default:
		{
			unsigned short sKey = 0;
			unsigned short sType = 0 ;
			
#if defined(SXH_SPT_H7810)
			if(msg_id<MSG_KEYLONG)
				sType = MSG_KEYUP;
			else if(msg_id<MSG_KEYDOWN)
				sType = MSG_KEYLONG;
			else if(msg_id<(MSG_KEYDOWN_MIDDLE+1))
				sType = MSG_KEYDOWN;
			sKey = msg_id - sType;
#else
			sKey = msg_id & 0xff;
			sType = msg_id & 0xff00 ;
#endif

	if(MSG_KEYDOWN == sType && g_up == 0 && g_keydown_cont==0)
			{
				g_sKey = sKey;
				if(g_sKey!=KEY_HEADSET_DETECT)//插入耳机
					g_keydown_cont=1;
			}
	else if(MSG_KEYDOWN == sType)
			{
				if(g_sKey != sKey)
					{
					
					return MMI_RESULT_TRUE;
					}
				else
					{
						// g_up = 0;
						 g_keydown_cont=0;
					}
			}
	
			if(MSG_KEYLONG==sType )
				{
					if(g_sKey != sKey)
					{
					  // g_up = 0;
					return MMI_RESULT_TRUE;
					}
					else
					{
						if(g_sKey!=KEY_HEADSET_DETECT)
							g_up = 1;
					}
				}
			
			if(MSG_KEYUP==sType ||  MSG_KEYPRESSUP==sType)
			{
				if(g_sKey != sKey)
					{
					
					return MMI_RESULT_TRUE;
					}
				else
					{
						 g_up = 0;
						 g_keydown_cont=0;
					}
				
			}

#if 0
			if(MSG_KEYDOWN == sType)
			{
				g_sKey = sKey;
			}

			if(MSG_KEYUP==sType || MSG_KEYLONG==sType || MSG_KEYPRESSUP==sType)
			{
				if(g_sKey != sKey)
					return MMI_RESULT_TRUE;
			}

#endif
			
			if(MSG_KEYDOWN==sType || MSG_KEYUP==sType || MSG_KEYLONG==sType || MSG_KEYPRESSUP==sType)
			{
// 				OEMOS_dbgprintf("key from 0xfa00 to 0xffff, msg_id:0x%x", msg_id, sKey, sType);
				AppleKeyHandler(sKey, sType);
			}
			else
			{
				#ifdef APPLE_LOG	
					OEMOS_dbgprintf("NoDealWith    msg_id  = %d ",msg_id);
				#endif
			}
			return MMI_RESULT_TRUE;
		}
		
//		OEMOS_dbgprintf("default MSG: 0x%x\r\n ", msg_id);
		
//		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
//			APPLE SYSTEM CALL											//
//////////////////////////////////////////////////////////////////////////


typedef void (*PFN_CVT_FUNC)(void *sv);
typedef void (*PFN_fast_load)(void **p1, void** p2, void** p3);
static PFN_CVT_FUNC   oemsvvc[3]={0};
// extern const  unsigned int sccvv_fast[];
#include "sccvv.c"
void OEMOS_CleanCache();
void OEMMedia_InitSccvv(void)
{

#ifdef  APPLE_DUBUG
	
	((PFN_fast_load)sccvv_fast)((void**)&oemsvvc[0], (void**)&oemsvvc[1], (void**)&oemsvvc[2]);
#else				
	char mod_name[128]={0};
	unsigned int len = 0; 
			
	if(sccvv_mod== NULL)
	{
		sprintf(mod_name, "%c:\\apple\\sys\\sccvv.mod", OEMFS_GetDrive(0));
		sccvv_mod = OEM_ReadFile((unsigned char *)mod_name, &len, 0,0);
#ifdef APPLE_LOG
		OEMOS_dbgprintf("APPLE_OS_GetDeviceCapExLib sccvv_mod %p  \r\n", sccvv_mod);
#endif			
//	*(int *)pBuff = (int)SXMOBI_SVGIV;
	if(sccvv_mod)
		{
		OEMOS_CleanCache();
		((PFN_fast_load)(sccvv_mod))((void**)&oemsvvc[0], (void**)&oemsvvc[1], (void**)&oemsvvc[2]);
		}

	}
#endif
}


void OEM_Fast_YUV2RGB_16(void *sv)
{

	OEMMedia_InitSccvv();
	if(oemsvvc[0])
	{
		oemsvvc[0](sv);
	}
}

void OEM_Fast_YUV2RGB_16_Any(void *sv)
{
	OEMMedia_InitSccvv();
	if(oemsvvc[1])
	{
		oemsvvc[1](sv);
	}
}

int16  OEMOS_InitLayer(  void) 
{
	
	//	register socket event handler
	OEMNET_RegNetEvt();

	OEMMedia_InitSccvv();
	return 0;
}

void  OEMOS_UnInitLayer(  void) 
{
	OEMNET_UnRegNetEvt();
	//	OEMTAPI_UnRegSmsEvent();
	//	OEMTAPI_UnRegIncomingCallEvent();

	OEMMedia_aud_Free_RingBuffer();
	
	return ;
}

void OEMOS_SignalDispatch( void)
{
	OEMOS_post_win_msg(0,0,0);
}
void OEMOS_ToExitCore( void)
{
  	OEMOS_post_win_msg(3,0,0);	//	to exit apple
//	MMK_CloseWin(APPLE_MAIN_WIN_ID);

}
void OEMOS_DLoadApp(uint32 iAppid, uint32 iCurrVer)
{	
	OEMOS_send_win_msg(11, iAppid,iCurrVer);
}
extern unsigned char initdata[];
unsigned char* OEMOS_GetBinInitCode(void)
{
#ifdef FEATURE_BIG_ENDIAN
	return NULL;
#else
	return initdata;
#endif
}
static int OEMOS_iTimerRef = 0;
void OEMOS_TimerAsyncCB(void *pUser)
{
	AppleTimerExpiredInt(pUser);
	if(OEMOS_iTimerRef>1)
	{
		OEMOS_SendOsMsg(12, (uint32)OEMOS_TimerAsyncCB, 0);	//async call, called in mmi task
		OEMOS_iTimerRef = 1;
	}
	else
		OEMOS_iTimerRef = 0;
}

static void OEMOS_TimerCB(uint32 param)
{
//	g_iAppleTimerId = 0;
//	MMK_PostMsg(APPLE_MAIN_WIN_ID,MSG_TIMER,(DPARAM)&g_iAppleTimerId ,sizeof(g_iAppleTimerId));

	OEMOS_iTimerRef++;
	if(OEMOS_iTimerRef>1)
		return;	
	OEMOS_SendOsMsg(12, (uint32)OEMOS_TimerAsyncCB, 0);	//async call, called in mmi task

}

uint32 OEMOS_InitTimer(void)
{
	//	create base timer
#ifdef APPLE_USE_MMK_TIMER
	g_iAppleTimerId = 0;//(uint32)MMK_CreateWinTimer(gAppleWinId, 1000, FALSE);
	return 1;	//	no use
#else
	OEMOS_iTimerRef = 0;
	return (uint32)SCI_CreateTimer(NULL, OEMOS_TimerCB, NULL, 1000, FALSE);
#endif
}
void OEMOS_UnInitTimer( uint32 iTimer)
{
	OEMOS_StopTimer(iTimer);
#ifdef APPLE_USE_MMK_TIMER
#else
	SCI_DeleteTimer((SCI_TIMER_PTR)iTimer);
#endif

}

void OEMOS_SetTimer( uint32 iTimer, uint32 nMSecs)
{	
	int rv = 0;

#ifdef APPLE_USE_MMK_TIMER
	if(0 == g_iAppleTimerId)
	{
		g_iAppleTimerId = MMK_CreateWinTimer(gAppleWinId, nMSecs, FALSE);
	}
	else
	{
		rv = MMK_StartWinTimer(gAppleWinId, g_iAppleTimerId,nMSecs, FALSE);
		if(!rv)
		{
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("OEMOS_SetTimer, iTimer:%d, FAILED!",iTimer);
			#endif
			MMK_PostMsg(gAppleWinId,MSG_TIMER,(DPARAM)&g_iAppleTimerId ,sizeof(g_iAppleTimerId));
		}
	}
#else
//	SCI_DeactiveTimer((SCI_TIMER_PTR)iTimer);
	rv = SCI_ChangeTimer((SCI_TIMER_PTR)iTimer, OEMOS_TimerCB, nMSecs);
	rv = SCI_ActiveTimer((SCI_TIMER_PTR)iTimer);
#endif 
}
void OEMOS_StopTimer( uint32 iTimer)
{	
#ifdef APPLE_USE_MMK_TIMER
	if(0!=g_iAppleTimerId)
		MMK_StopTimer(g_iAppleTimerId);
	g_iAppleTimerId = 0;
#else
	     if(SCI_IsTimerActive(iTimer))
			SCI_DeactiveTimer((SCI_TIMER_PTR)iTimer);
#endif
}

void OEMOS_SendOsMsg( unsigned int iMsg, unsigned int data1, unsigned int data2)
{
	OEMOS_post_win_msg(iMsg,data1,data2);
}
void OEMOS_post_win_msg(unsigned int iMsg, unsigned int data1, unsigned int data2)
{
	apple_msg_struct *sendSignal = PNULL;
	MmiCreateSignal(APP_APPLE_END_IND, sizeof(apple_msg_struct), (MmiSignalS**)&sendSignal);
	sendSignal->Sender = P_APP;
	sendSignal->op_code = iMsg;
	sendSignal->data1 = data1;
	sendSignal->data2 = data2;
	MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
	
#if 0
	apple_msg_struct msg;
	
	msg.op_code = iMsg;
	msg.data1 = data1;
	msg.data2 = data2;
	

	MMK_PostMsg(gAppleWinId/*APPLE_MAIN_WIN_ID*/,APPLE_BASE_MSG_ID,(DPARAM)&msg ,sizeof(msg));
	
#ifdef SXH_SPT_H7810
	MMI_TriggerMMITask();
#else
	MMIAPICOM_TriggerMMITask();
#endif
#endif
}
void OEMOS_send_win_msg(unsigned int iMsg, unsigned int data1, unsigned int data2)
{
	apple_msg_struct msg;
	
	msg.op_code = iMsg;
	msg.data1 = data1;
	msg.data2 = data2;
	
	MMK_SendMsg(gAppleWinId/*APPLE_MAIN_WIN_ID*/,APPLE_BASE_MSG_ID,(DPARAM)&msg);
#ifdef SXH_SPT_H7810
	MMI_TriggerMMITask();
#else
	MMIAPICOM_TriggerMMITask();
#endif
}




#ifdef APPLE_LOG

#undef SCI_TRACE_LOW
#ifndef WIN32
	#define SCI_TRACE_LOW    SCI_TraceLow
#else
	#define SCI_TRACE_LOW    SCI_Trace
#endif


#define APPLE_DBG_BUF_MAX_LEN 4096
static char buffer_log_os[APPLE_DBG_BUF_MAX_LEN] = {0};
#define APPLE_DBG_MAX_LEN (MAX_STR_SIZE-50)

#endif
//#define USE_FILE_LOG
void OEMOS_dbgprintf(const char *format, ...)
{
#ifdef APPLE_LOG
	
	int iLen = 0;
	int i = 0;
	va_list args = {0};
	char tmp ;
	
#ifdef USE_FILE_LOG
	SFS_HANDLE hFile= NULL;
	OEM_wsprintf((uint16*)buffer_log_os,APPLE_DBG_BUF_MAX_LEN/2,(const uint8*) L"%c:\\apple\\apple.log",OEMFS_GetDrive(0));
	hFile = SFS_CreateFile((unsigned short*)buffer_log_os,FFS_MODE_READ|FFS_MODE_WRITE|FFS_MODE_OPEN_EXISTING, 0, 0);
	if(NULL != hFile)
	{		
		SCI_MEMSET(buffer_log_os,0,APPLE_DBG_BUF_MAX_LEN);
		sprintf(buffer_log_os, "APPLE_OS-%d - ", OEM_GetTimeMS());		
		iLen = strlen(buffer_log_os);
		va_start(args, format);
		vsprintf(buffer_log_os+iLen,format, args);
		va_end(args);	 

		iLen = strlen(buffer_log_os);
		SFS_WriteFile(hFile,(void*)buffer_log_os,(uint32)iLen, (uint32*)&i, NULL);
		SFS_CloseFile(hFile);
	}
	
#else
	va_start(args, format);
	SCI_MEMSET(buffer_log_os,0,APPLE_DBG_BUF_MAX_LEN);
	vsprintf(buffer_log_os,format, args);
	va_end(args);    

	iLen = strlen(buffer_log_os);

	for(i=0; i<iLen; )
	{
		tmp = buffer_log_os[i+APPLE_DBG_MAX_LEN];
		buffer_log_os[i+APPLE_DBG_MAX_LEN] = 0;
		SCI_TRACE_LOW("APPLE_OS-%d - %s", OEM_GetTimeMS(), buffer_log_os+i);
		buffer_log_os[i+APPLE_DBG_MAX_LEN] = tmp;
		i += APPLE_DBG_MAX_LEN;
	}
#endif 

	
#endif
	return;
} 
void OEMOS_AppDbgprintf(const char *format, ...)
{

#ifdef APPLE_LOG
	int iLen = 0;
	int i = 0;
	va_list args = {0};
	char tmp ;
#ifdef USE_FILE_LOG
	SFS_HANDLE hFile= NULL;
	OEM_wsprintf((uint16*)buffer_log_os,APPLE_DBG_BUF_MAX_LEN/2,(const uint8*) L"%c:\\apple\\apple.log",OEMFS_GetDrive(0));
	hFile = SFS_CreateFile((unsigned short*)buffer_log_os,FFS_MODE_READ|FFS_MODE_WRITE|FFS_MODE_OPEN_EXISTING, 0, 0);
	if(NULL != hFile)
	{		
		SCI_MEMSET(buffer_log_os,0,APPLE_DBG_BUF_MAX_LEN);
		sprintf(buffer_log_os, "APPLE_APP(0x%x)-%d - ", APPLE_GetCurrAppID(), OEM_GetTimeMS());		
		iLen = strlen(buffer_log_os);
		va_start(args, format);
		vsprintf(buffer_log_os+iLen,format, args);
		va_end(args);	 

		iLen = strlen(buffer_log_os);
		SFS_WriteFile(hFile,(void*)buffer_log_os, (uint32)iLen, (uint32*)&i, NULL);
		SFS_CloseFile(hFile);

	}
	
#else	
	va_start(args, format);
	SCI_MEMSET(buffer_log_os,0,APPLE_DBG_BUF_MAX_LEN);
	vsprintf(buffer_log_os,format, args);
	va_end(args); 
	iLen = strlen(buffer_log_os);
	for(i=0; i<iLen; )
	{
		tmp = buffer_log_os[i+APPLE_DBG_MAX_LEN];
		buffer_log_os[i+APPLE_DBG_MAX_LEN] = 0;
		SCI_TRACE_LOW("APPLE_APP(0x%x)-%d - %s", APPLE_GetCurrAppID(), OEM_GetTimeMS(), buffer_log_os+i);
		buffer_log_os[i+APPLE_DBG_MAX_LEN] = tmp;
		i += APPLE_DBG_MAX_LEN;
	}
#endif	  
#endif	  
	return;
} 
void OEMOS_HexLog(const unsigned char *data, int datalen)
{

#ifdef APPLE_LOG

	int i = 0, j=0;
	if(NULL==data || 0==datalen)
		return ;
	for(i=0; i<((datalen-1)/16+1); i++)
	{
		int size = datalen - i*16;
		size = size<16 ? size : 16;
		for(j=0; j<size; j++)
		{
			sprintf(buffer_log_os+j*5,"0x%02x ", data[i*16+j]);
		}
		SCI_TRACE_LOW("APPLE_LOG - %s", buffer_log_os);
	}

#endif	

}

uint16 OEMOS_GetActiveNaID(void)	
{
	//	获取当前活跃的sys app id
	return MMK_GetFocusWinId();	//	force to uint16
}
uint16 OEMOS_GetIdleNaID(void)
{
	//	获取idle app id
	return MAIN_IDLE_WIN_ID;
}

typedef enum 
{
	AVK_FIRST = 0xE020,
		
		AVK_0,
		AVK_1,
		AVK_2,
		AVK_3,
		AVK_4,
		AVK_5,
		AVK_6,
		AVK_7,
		AVK_8,
		AVK_9,
		AVK_STAR,
		AVK_POUND,
		
		AVK_POWER,
		AVK_END,
		AVK_SEND,
		AVK_CLR,
		
		AVK_UP,
		AVK_DOWN,
		AVK_LEFT,
		AVK_RIGHT,
		AVK_SELECT,
		
		AVK_SOFT1,
		AVK_SOFT2,
		AVK_SOFT3,
		AVK_SOFT4,
		
		AVK_CAMERA = AVK_FIRST+29,
		
		AVK_VOLUME_UP = AVK_FIRST+39,
		AVK_VOLUME_DOWN = AVK_FIRST+40,
		
		AVK_LAST = AVK_FIRST+67
}AppleKey;

uint16 OEMOS_KeyCodeMap(uint16 sKey)
{
	switch (sKey)
	{
	case KEY_0:
		return AVK_0;
	case KEY_1:
		return AVK_1;
	case KEY_2:
		return AVK_2;
	case KEY_3:
		return AVK_3;
	case KEY_4:
		return AVK_4;
	case KEY_5:
		return AVK_5;
	case KEY_6:
		return AVK_6;
	case KEY_7:
		return AVK_7;
	case KEY_8:
		return AVK_8;
	case KEY_9:
		return AVK_9;
	case KEY_OK:
		return AVK_SOFT1;
//	case KEY_MIDDLE:
	case KEY_WEB:
		return AVK_SELECT;
	case KEY_CANCEL:
		return AVK_SOFT2;
	case KEY_UP:
		return AVK_UP;
	case KEY_DOWN:
		return AVK_DOWN;
	case KEY_LEFT:
		return AVK_LEFT;
	case KEY_RIGHT:
		return AVK_RIGHT;
	case KEY_GREEN:
		return AVK_SEND;
	case KEY_RED:
		return AVK_END;
	case KEY_STAR:
		return AVK_STAR;
	case KEY_HASH:
		return AVK_POUND;
	case KEY_UPSIDEKEY:
		return AVK_VOLUME_UP;
	case KEY_DOWNSIDEKEY:
		return AVK_VOLUME_DOWN;
	case KEY_CAMREA:
		return AVK_CAMERA;
	default:
		break;	
	}
	return AVK_LAST;	//	unknow key
}
uint16 OEMOS_KeyEvtMap(uint16 sEvt)
{
	switch (sEvt)
	{
	case MSG_KEYDOWN:
		return APPLE_EVT_KEY_PRESS;
	case MSG_KEYLONG:
		return APPLE_EVT_KEY_HELD;
	case MSG_KEYPRESSUP:
	case MSG_KEYUP:
		return APPLE_EVT_KEY_RELEASE;
// 	case KEY_REPEATED:
// 		return APPLE_EVT_KEY;
	default:
		break;
	}
	return 0;		//	unknow evt
}

int AppleKeyHandler(unsigned short sKey,unsigned short sType)
{
	boolean rv = FALSE;
	
	if(!g_IsAppleInited)
		return rv;
	
	rv = APPLE_Key(sKey,sType);
	
	if(MSG_KEYDOWN == sType)
		usKeyStart = KEY_NONE;
	
	if(rv)	//	apple handled it
	{
		if(MSG_KEYDOWN == sType)
			usKeyStart = sKey;
	}
	else
	{
		if(usKeyStart == KEY_NONE)
			return FALSE;
	}
	
	return TRUE;
}

uint32 OEMOS_GetKeyState(uint32 *r1,uint32 *r2,uint32 *r3)
{
	return 0;
}

void OEMOS_GetJulianDate(uint32 dwSecs, AppleJulianType *pDate)
{
	uint32 rv=0;
	SCI_TIME_T systime = {0};
	SCI_DATE_T sysdate = {0};
	MMI_TM_T systm = {0};
	if(pDate)
	{
		if(dwSecs==0)
		{
			TM_GetSysDate(&sysdate);
			TM_GetSysTime(&systime);
			pDate->wYear=sysdate.year;
			pDate->wMonth =sysdate.mon;
			pDate->wDay=sysdate.mday;
			pDate->wWeekDay=sysdate.wday;
			pDate->wHour=systime.hour;
			pDate->wMinute=systime.min;
			pDate->wSecond=systime.sec;

		}
		else
		{
#ifdef SXH_SPT_H7810
			systm = MMI_Second2Tm(dwSecs);
#else
			systm = MMIAPICOM_Second2Tm/*MMIAPICOM_FatData2Tm*/(dwSecs);
#endif			
			pDate->wYear=systm.tm_year;
			pDate->wMonth =systm.tm_mon;
			pDate->wDay=systm.tm_mday;
			pDate->wWeekDay=systm.tm_wday;
			pDate->wHour=systm.tm_hour;
			pDate->wMinute=systm.tm_min;
			pDate->wSecond=systm.tm_sec;
		}	
	}
}


int OEMOS_ClearScreen(void)
{
	GUI_RECT_T          disp_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	LCD_FillRect(&lcd_dev_info, disp_rect, MMI_WINDOW_BACKGROUND_COLOR);

	return 0;
}

void OEMOS_mmi_idle_key_event_backlight_check()
{
	MMIDEFAULT_TurnOnBackLight();
}

void OEMOS_Motion_Effect_Switch(uint32 enable)
{
}
//////////////////////////////////////////////////////////////////////////
//  APPLE FILE
////////////////////////////////////////////////////////////////////////

int OEMFS_GetDriveStatus()
{
	//if(MMIFILE_GetDeviceStatus( MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN ) 
	//  || MMIFILE_GetDeviceStatus( MMIFILE_DEVICE_UDISK, MMIFILE_DEVICE_UDISK_LEN ))
	if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD),MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD))
	|| MMIFILE_GetDeviceStatus( MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK) ))
	  return 1;
	else
	   return 0;
	
}

int8 OEMFS_GetDrive(uint32 type)
{
	uint16 *drv = NULL;
	int8 c = 0;
	
	if(MMIAPISD_GetStatus(MMI_DEVICE_SDCARD))
	{
		drv = (uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);//MMIFILE_DEVICE_SDCARD;
	}
	else
	{
		drv = (uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);//MMIFILE_DEVICE_UDISK;
	}

//	c = (int8)ntohs(drv[0]);//转为网络序后，再操作
	c = drv[0];

/*
	// test jxh 2012-03-06
	
    c = 'H';
// end test
*/
	return c;		

}

//////////////////////////////////////////////////////////////////////////
//		APPLE NETWORK													//
//////////////////////////////////////////////////////////////////////////
#define MMICMSBRW_MAX_SETTING_NUM              10
//extern MMICMSBRW_SETTING_T	g_cmsbrw_setting_info;
// extern MMICONNECTION_SETTING_T  s_connection_setting_info ;
// extern  MN_DUAL_SYS_E s_connection_handling_sim_id ;//sxh az
//MMICONNECTION_SETTING_T	g_cmsbrw_setting_info = s_connection_setting_info;//g_cmsbrw_setting_info;
// #define g_cmsbrw_setting_info s_connection_setting_info


// jxh new connection funtion by 2012-3-12
MMICONNECTION_LINKSETTING_DETAIL_T* p_apple_connection_detail = NULL;


int g_iapnindex = 0;
 int OEMOS_Addapn(void)
{
	uint32 index = 0;
	int i = 0;
	const uint16 * accout_name=  L"cmnet";
	const char *  apn_name = (char*) "cmnet";
		// jxh 2011-12-30
	MN_DUAL_SYS_E dual_sys = g_apple_active_sim;//sxh az  20111229

	// jxh new connection funtion by 2012-3-12
	MMICONNECTION_LINKSETTING_DETAIL_T* p_settingptr = NULL;
//	index = s_connection_setting_info .settinglink_num[dual_sys]; // OEMOS_GPRSDataAccountNode_GetSize();
    index = MMIAPICONNECTION_GetLinkSettingNum(dual_sys);

	if(index == MMICMSBRW_MAX_SETTING_NUM)
		return -1;
	
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("apple_Addapn index%d ", index);
	#endif
	
	for(i=0; i< index;i++)
	{
		p_apple_connection_detail = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys,i);
		if(strcmp(p_apple_connection_detail->apn,(char*)apn_name) == 0) // 已有自己建的cmnet
		{
			g_iapnindex = 0;
			return 0;
		}
	}

	p_settingptr = (MMICONNECTION_LINKSETTING_DETAIL_T*)OEMHEAP_GlobalMalloc(sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
	if (p_settingptr == NULL)
		return -1;

	strcpy(p_settingptr->apn,(char*)apn_name);
	MMIAPICOM_Wstrcpy(p_settingptr->name,(uint16*)accout_name);
    p_settingptr.visible = TRUE;
    
	MMIAPICONNECTION_AddNewItem(dual_sys,p_settingptr);

	OEMHEAP_GlobalFree(p_settingptr);
	
	g_iapnindex = index;
	return 0;
}

void OEMOS_Delapn()
{


}

int32 OEMOS_GetActiveSim(void)
{	

#if 0
	BOOLEAN is_sim1_ok = FALSE;
    BOOLEAN is_sim2_ok = FALSE;
	int32 sim = -1;
	char imsi[50]={0};
#ifdef MULTI_SIM_SYS_TRI
         BOOLEAN is_sim3_ok = FALSE;
#endif
#ifdef MMI_MULTI_SIM_SYS_QUAD
	BOOLEAN is_sim4_ok = FALSE;
#endif

	MMIPHONE_GetDualSimStatus(&is_sim1_ok, &is_sim2_ok);
#ifdef MULTI_SIM_SYS_TRI
		is_sim3_ok = MMIPHONE_IsSimOk(MN_DUAL_SYS_3);
#endif
#ifdef APPLE_LOG
	OEMOS_dbgprintf("OEMOS_GetActiveSim, set:%d, sim1:%d, sim2:%d", g_apple_active_sim, is_sim1_ok, is_sim2_ok);
#endif

	if(!is_sim1_ok && !is_sim2_ok
#ifdef MULTI_SIM_SYS_TRI
        && !is_sim3_ok
#endif
	)
	{
		g_apple_active_sim = -1;
	}
    else if ( is_sim1_ok && !is_sim2_ok
#ifdef MULTI_SIM_SYS_TRI
        && !is_sim3_ok
#endif
	)
    {
        g_apple_active_sim = MN_DUAL_SYS_1;
    }
    else if ( !is_sim1_ok && is_sim2_ok
#ifdef MULTI_SIM_SYS_TRI
        && !is_sim3_ok
#endif
	)
    {
        g_apple_active_sim = MN_DUAL_SYS_2;
    } 
#ifdef MULTI_SIM_SYS_TRI
   else if ( !is_sim1_ok && ! is_sim2_ok&& is_sim3_ok)
   	{
   		g_apple_active_sim = MN_DUAL_SYS_3;
   	}
#endif
	else
	{
		if(-1 != g_apple_active_sim)
			return g_apple_active_sim;
		g_apple_active_sim = MN_DUAL_SYS_1;
	}

#ifdef APPLE_LOG
	OEMOS_dbgprintf("OEMOS_GetActiveSim, sim:%d", g_apple_active_sim);
#endif
#else
    //BOOLEAN is_need_sel = FALSE;//记录是否需要进行网络选择
    
	uint16  sim_state_arr[MMI_DUAL_SYS_MAX] = {0};
	uint32  sim_num = 0;
	uint32  i = 0;

	int   active_sim = -1;

	for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
	{
		sim_state_arr[i] = MN_DUAL_SYS_MAX; 
		if (MMIPHONE_IsSimOk(i))
		{
			sim_state_arr[i] = i;
			sim_num++;
			if(-1 == active_sim)
			{
				active_sim = i;
			}
		}
	}		
	if(sim_num ==0 )
		g_apple_active_sim = -1;
	else if(sim_num == 1)
	{
		g_apple_active_sim = active_sim;
	}
	else if(sim_num > 1)
	{
		if(-1 != g_apple_active_sim)
			return g_apple_active_sim;
		g_apple_active_sim = active_sim;
	}
#endif
	return g_apple_active_sim;

}

int32 OEMOS_SetActiveSim(int32 iSim)
{
	if(!MMIPHONE_IsSimOk(iSim))
		return -1;

	g_apple_active_sim = iSim;
	return g_apple_active_sim;
}
void apple_select_network_sim(int sim)
{
	OEMOS_SetActiveSim(sim);
}
int apple_sys_get_sim_network(unsigned int *pSim1,unsigned int *pSim2)
{
	return 0;
}

void OEM_mmi_idle_key_event_backlight_check()
{
}
void sxm_bcd2dec(uint8 *bcd, uint8* dec, uint16 convertLen)
{
	uint8 i, j;

	if(bcd == NULL || dec == NULL)
		return;

	for(i = 0, j = 0; i < convertLen; i++)
	{
		dec[j++] = (bcd[i] & 0x0F) + '0';
		dec[j++] = ((bcd[i] >> 4) & 0x0F) + '0';
	}
}


void sxm_get_imsi(char* imsi, unsigned int imsi_len, int sim_id)
{
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("sxm_get_imsi ");					
	#endif
	
	if(imsi)
	{
		MN_IMSI_T  tmp_imsi={0};
		char temp[20] = {0x0};			
		memset(imsi, 0x00, imsi_len);
			// jxh 2011-12-30
		if(SIM_STATUS_OK == MMIPHONE_GetSimStatus(sim_id))
			tmp_imsi = MNSIM_GetImsiEx(sim_id);
		if(tmp_imsi.imsi_len == 0)
			{
				strcpy(imsi,"012345678912345");
				return;
			}
		sxm_bcd2dec(tmp_imsi.imsi_val, (uint8*)temp, tmp_imsi.imsi_len);
		memcpy(imsi,&temp[1],strlen(temp)-1);
#ifdef APPLE_LOG
		OEMOS_dbgprintf("sxm_get_imsi DUAL simid=0,imsi=%s",imsi);
#endif
	}
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("sxm_get_imsi E");	
	#endif
}

void OEMGetIMEI(uint8* pBuf, uint32 nBufSize)
{
	APPLE_OS_GetPhoneID((unsigned char* )pBuf, (unsigned int )nBufSize);
}

void testfunc(void *p)
{
	//	used for test os api
}

int32 OEMOS_CheckMsg(void)
{
	return 0;
}

void APPLE_DisplayPopup(uint8* szText)
{
}
int OEMOS_GetLangId(void)
{
	 MMISET_LANGUAGE_TYPE_E  language_type;  
	 if(g_apple_lanId)
	 {
	 	return 86;
	 }
	MMIAPISET_GetLanguageType(&language_type);
	switch(language_type)
	{
		case MMISET_LANGUAGE_SIMP_CHINESE:
			return 86;//CN
			
		case MMISET_LANGUAGE_ENGLISH:   //test
			if(APPLE_GetCurrAppID()==0x0300009A)
				return 86;
			break;
		default:
			break;
	} 
	return 44;	//EN
}

char* OEMOS_GetString(unsigned int appid, unsigned int id)
{
	char * rv = NULL;
	return rv;
}
static char g_brw_url[255] = {0};
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
/*static */void OEMOS_BRW_ConnectCB(void* p)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
#endif
	if(0 != gAppleWinId)
		MMK_CloseWin(gAppleWinId);

#ifdef BROWSER_SUPPORT
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = OEMOS_GetActiveSim();
    entry_param.url_ptr = g_brw_url;
    entry_param.user_agent_ptr = PNULL;
    MMIAPIBROWSER_Entry(&entry_param);
#endif	
}
int OEMOS_BRW_ConnectUrl(char* url)
{
	strcpy(g_brw_url, url);
	OEMOS_SendOsMsg(12, (uint32)OEMOS_BRW_ConnectCB, 0);	//async call, called in mmi task
	return 0;
}

int OEMOS_AccessNvRam(void *nvram_apple, int isWrite)
{
	char imsi[20]={0};
	MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
	if(isWrite)
	{
	//	MMINV_WRITE(APPLE_GLOBAL_SET, nvram_apple);
	}
	else
	{
		sxm_get_imsi(imsi, sizeof(imsi), g_apple_active_sim);
		imsi[15]='0';
/*	
		MMINV_READ(APPLE_GLOBAL_SET, nvram_apple, nv_value);
		if (MN_RETURN_SUCCESS != nv_value)
			return -1;
*/
	}
	return 0;
}

void OEMOS_GetSysIMEI(uint8* pBuf, uint32 nBufSize)
{
    uint8   imeiStr[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
	MN_IMEI_T           imei_arr = {0}; 
#ifndef WIN32
	MNNV_GetIMEIEx(MN_DUAL_SYS_1, imei_arr);
#ifdef SXH_SPT_H7810
	MMI_BcdToStr(PACKED_LSB_FIRST,imei_arr,MN_MAX_IMEI_LENGTH<<1,(char*)imeiStr);
#else
	MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imei_arr,MN_MAX_IMEI_LENGTH<<1,(char*)imeiStr);
#endif
#endif
	memcpy(pBuf, imeiStr+1, nBufSize);
//	OEMOS_dbgprintf("OEMOS_GetSysIMEI:%s", imeiStr);
}

int OEMOS_GetLCDWidth(void)
{
	#ifdef APPLE_ROTATE_STATIC
	
	if((MMK_GetWinSupportAngle(APPLE_MAIN_WIN_ID) == WIN_SUPPORT_ANGLE_90 )  ||  
		(MMK_GetWinSupportAngle(APPLE_MAIN_WIN_ID) == WIN_SUPPORT_ANGLE_270 )   )   //if(MMK_GetScreenAngle()%2 != 0 )
		return LCD_HEIGHT;
	#endif	
	

	return LCD_WIDTH;
}

int OEMOS_GetLCDHeight(void)
{	
	#ifdef APPLE_ROTATE_STATIC
		if((MMK_GetWinSupportAngle(APPLE_MAIN_WIN_ID) == WIN_SUPPORT_ANGLE_90 )  ||  
		(MMK_GetWinSupportAngle(APPLE_MAIN_WIN_ID) == WIN_SUPPORT_ANGLE_270 )   )   //if(MMK_GetScreenAngle()%2 != 0 )
			return LCD_WIDTH;
	#endif	

	return LCD_HEIGHT;
}

int OEMOS_GetLCDPitch(void)
{
	return LCD_WIDTH;
}

void *OEMOS_GetLCDFrameBuffer(unsigned int * buf_size)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_COLOR_T *block_buffer_ptr = PNULL;
	uint32 width = 0;
	uint32 height = 0;
	uint32 buf_width = 0;
	uint32 buf_height = 0;	

	//block_buffer_ptr = (GUI_COLOR_T *)GUIBLOCK_GetBlockBuffer((GUI_LCD_DEV_INFO *)&lcd_dev_info);
	block_buffer_ptr = (GUI_COLOR_T*)GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)&lcd_dev_info);

	GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, (uint16*)&width, (uint16*)&height);

	//GUI_GetLcdSize(lcd_dev_info.lcd_id, &width, &height);
#if !defined(SXH_SPT_0839) && !defined(SXH_SPT_H7810)
	///*GUI_GetLcdBufferSize*/GUI_GetLcdSize(lcd_dev_info.lcd_id, &buf_width, &buf_height);
#else
	buf_width = width;
	buf_height = height;
#endif
	*buf_size = buf_width * buf_height * sizeof(GUI_COLOR_T);

	return (void *)block_buffer_ptr;
}

struct APPLE_SMS_BLACKList
{
	unsigned char mode;
	unsigned char num;
	unsigned char pnlen[10];
	unsigned char pn[10][20];
}apple_sms_blacklist;

int AppleIsSmsDisplay(char *pn, int mode)//1:删除2:屏蔽3:忽略
{
	int i,j,rv,pnlen,len;
	
	rv = 0;
	pnlen=0;
	if(!AppleIsRun())
		return rv;
	
	if(NULL == pn)
		return rv;
	
	pn = strstr(pn,"86");
	if(pn){
			pn+=2;

			pnlen = strlen(pn);
			if(0==pnlen)
				return rv;
		}
//	apple_os_log("sms received %c%c%c%c%c%c%c%c mode:%d\r\n", pn[0],pn[1],pn[2],pn[3],pn[4],pn[5],pn[6],pn[7], apple_sms_blacklist.mode);	
	if((apple_sms_blacklist.mode==mode)
		||(apple_sms_blacklist.mode==3))
	{
		if(apple_sms_blacklist.mode==3)
			rv = 0;

		for(i=0;i<apple_sms_blacklist.num;i++)
		{	
			if(apple_sms_blacklist.pnlen[i]==0)
			{
				rv = !rv;
				break;
			}

			len = MIN(apple_sms_blacklist.pnlen[i], pnlen);
			for(j=0;j<len;j++)
			{
				if(NULL == pn[j])
					return rv;
				if(apple_sms_blacklist.pn[i][j]!=(pn[j]-'0'))
				{
					break;
				}
			}
			if(j == len)
			{
				rv = !rv;
				break;
			}
				
		}
	}
	
	return rv;
}

void AppleSetSmsDisplay(unsigned char *param)
{
	int i,len;
	
	if(param)
	{
		apple_sms_blacklist.mode = *param;
		param++;
		apple_sms_blacklist.num = MIN(10,*param);
		param++;
		for(i=0;i<apple_sms_blacklist.num;i++)
		{
			len = *param;
			apple_sms_blacklist.pnlen[i] = MIN(20, len);		
			param++;
			memcpy(apple_sms_blacklist.pn[i], param, apple_sms_blacklist.pnlen[i]);
			param+=len;
		}
	}

}

uint32 AppleIsRun(void)
{
	if(gAppleWinId)
		return 1;
	return 0;
}


uint32 AppleVCIsRun(void)
{
	if(gAppleWinId&&(APPLE_GetCurrAppID() ==0x0300009A ||APPLE_GetCurrAppID() ==0x0300009a))
		return 1;
	return 0;
}

int AppleMusicIsRun()
{
#ifdef SXMOBI_UMMUSIC
	return UMMusicIsRun();
#else
	return 0;
#endif //SXMOBI_UMMUSIC

}

int32 AppleIsBG()
{
	return g_isAppleGB;
}
//////////////////////////////////////////////////////////////////////////
//	pc emulator
#ifdef WIN32
char* OEMOS_ToPcPath(char* pPath)
{
	int32 iLen = 10;
	char *pPcPath = NULL;
	char *p = "D:\\dev\\apple\\app\\test_spt_apm\\appman.bin";

	iLen += strlen(p);
	pPcPath = OEM_Heap_Malloc_DBG(iLen);
	if(NULL == pPcPath)
		return NULL;
	sprintf(pPcPath, "%s",p);
	return pPcPath;
}

uint32 OEMOS_LoadLib(char* pPath)
{
	char* pPcPath = OEMOS_ToPcPath(pPath);
	uint32 hDll = 0;
	if(NULL == pPcPath)
		return 0;
	hDll = (uint32)LoadLibrary(pPcPath);
	OEMHeap_Free(pPcPath);
	return hDll; 
}
uint8* OEMOS_GetFunc(uint32 hLib, char* pFuncName)
{
	return (uint8*)GetProcAddress((HMODULE)hLib,pFuncName);
}
void OEMOS_FreeLib(uint32 hLib)
{
	FreeLibrary((HMODULE)hLib);
}
#endif //WIN32



/////	memory
void* OEMHEAP_GlobalMalloc(uint32 size)
{
 	char* p = NULL;
	char *pp = NULL;
	int i;


	if(size == 0)
		return NULL;

#ifdef APPLE_DUBUG
	
	size += 68;

   	 pp = p = SCI_ALLOCA(size);

	for(i=0;i<32;i++)
		{
	pp[i]=0xcc;
		}
	memcpy(pp+32, &size, sizeof(int));
	pp=pp+size-32;

	for(i=0;i<32;i++)
		{
	pp[i]=0xdd;
		}
	
	return (void *)&p[36];
#else	
	p = SCI_ALLOCA(size);

	return p;
 	
	
#endif

}
void OEMHEAP_GlobalFree(void* ptr)
{
#ifdef APPLE_DUBUG
	char* p = NULL;
	char *pp = NULL;	
	int size = 0, i = 0;

	if(!ptr)
		{
		return;
		}

	SCI_Sleep(1);
	
	pp = p = (char *)ptr - 36;
	
	memcpy(&size, pp+32, sizeof(int));

	for(i=0;i<32;i++)
		{
		if(pp[i]!=0xcc)
			break;
		}

	if(i!=32)
		{
		OEMOS_dbgprintf("memory corrupt ptr:%x size:%d ", ptr, size);	
		return;
		}

	pp = pp+size-32;

	for(i=0;i<32;i++)
		{
		if(pp[i]!=0xdd)
			break;
		}

	if(i!=32)
		{
		OEMOS_dbgprintf("memory corrupt ptr tail:%x size:%d ", ptr, size);	
		return;
		}


	SCI_FREE(p);
		   
#else			

	if(ptr)
		SCI_FREE(ptr);
#endif   
}
int OEMHEAP_GlobalSize(void)
{
	return  1*1024*1024; // SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_APP_MEMORY);
}
void* OEMHEAP_MainMemMalloc(void)
{
	return BL_Malloc(BLOCK_MEM_POOL_REF_DC);
}
void OEMHEAP_MainMemFree(void* p)
{
	BL_Free(BLOCK_MEM_POOL_REF_DC);
}
uint32 OEMHEAP_MainMemSize(void)
{
	return BL_GetSize(BLOCK_MEM_POOL_REF_DC);
}
#define USE_SECOND_MEM

#ifdef USE_SECOND_MEM
const int g_iSECONDMemMaxSize = 400*1024;

#endif
void* OEMHEAP_SecondMemMalloc(void)
{

#ifdef USE_SECOND_MEM
	
	return OEMHEAP_GlobalMalloc(g_iSECONDMemMaxSize);
#else
	return NULL;
#endif
      
}
void OEMHEAP_SecondMemFree(void* p)
{
		OEMHEAP_GlobalFree(p);

}

uint32 OEMHEAP_SecondMemSize(void)
{
#ifdef USE_SECOND_MEM
	return g_iSECONDMemMaxSize;
#else
	return 0;
#endif
}
#define USE_APP_MEM
#ifdef USE_APP_MEM
const int g_iAppMemMaxSize = 300*1024;
	#ifdef WIN32
	int g_iAppMemSize = 100*1024;
	#else
	int g_iAppMemSize = g_iAppMemMaxSize;
	#endif
#endif 
void * OEMHEAP_AppMemMalloc(uint32 size)
{
#ifdef USE_APP_MEM
	if(g_iAppMemSize <= 0 ||g_iAppMemSize < size)
		return NULL;
	g_iAppMemSize = 0;
	return OEMHEAP_GlobalMalloc(size);
#else
	return NULL;
#endif
}
void  OEMHEAP_AppMemFree(void* p)
{
#ifdef USE_APP_MEM
	if(g_iAppMemSize >0)
		return;
	g_iAppMemSize = g_iAppMemMaxSize;
	OEMHEAP_GlobalFree(p);
#endif
}
uint32 OEMHEAP_AppMemSize(void)
{
#ifdef USE_APP_MEM
	return g_iAppMemSize;
#else
	return 0;
#endif
}




#define DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))
//#define OEMOS_DIRECT_DRAW
#if 1//def OEMOS_DIRECT_DRAW
static uint32 s_DirectDrawRotateBuf = 0 ;
static uint32 s_DirectDrawDisplayBuf = 0 ;
extern uint32 SXMOBI_DCAMERA_ExtDisplay(DCAMERA_EXT_DISPLAY_T_PTR display_param );

typedef struct
{
	uint16 w;
	uint16 h;
}OEMDirectSize;


OEMDirectSize gOemDirectSize = {0};

void OEMDisplay_FreeRotateBuf()
{
	if(s_DirectDrawRotateBuf)
	{
		OEMHeap_Free((void*)s_DirectDrawRotateBuf );
		s_DirectDrawRotateBuf  = NULL;
	}

	if(s_DirectDrawDisplayBuf)
	{
		OEMHeap_Free((void*)s_DirectDrawDisplayBuf );
		s_DirectDrawDisplayBuf  = NULL;
	}
}

uint32 OEMDisplay_GetRotateBuf()
{
	return s_DirectDrawRotateBuf;
}
#else
uint32 OEMDisplay_GetRotateBuf()
{
	return 0;
}


void OEMDisplay_FreeRotateBuf()
{}

#endif

/*
extern void Sxmobi_ISP_Rotation(ISP_DATA_ADDR_T* src_buf_ptr, 
                                uint16 src_width, 
                                uint16 src_height, 
                                ISP_ROTATION_E rotation, 
                                ISP_DATA_ADDR_T* buf_ptr, 
                                ISP_DATA_FORMAT_E rot_format);
*/                                
/////	display
void OEMDisplay_DirectDrawImg( uint16 x,  uint16 y, uint16 width, uint16 height,  uint32 *buf_ptr, int rot  )
{
    LCD_ID_E lcd_id = MAIN_LCD_ID;
    BLOCKCFG_T cfg = {0};
//    BLOCKCFG_T rst_cfg = {0}; 
//    LCD_BRUSH_MODE_E lcd_brush_mode=LCD_BRUSH_MODE_ASYNC;
       uint16 temp = 0;
//    uint32 *rst_lcd_buf =  LCD_GetBlockBuffer( lcd_id, 0);
//	BLOCKCFG_T rst_cfg = {0}; 

#if 0
	DCAMERA_EXT_DISPLAY_T   display_info = {0};

	OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  1  x:%d,y:%d,width:%d,height:%d,buf_ptr:%d,rot:%d",x,y,width,height,buf_ptr,rot);
	x = x>>2<<2;
	y = y >>2<<2;
	width = width>>2<<2;
	height = height >>2<<2;


	display_info.src_format = DCAMERA_DATA_TYPE_RGB565;
	display_info.src_img_size.h = height;
	display_info.src_img_size.w = width;
	//display_info.src_yuv_addr.y_chn_addr = buf_ptr;
	//display_info.src_yuv_buf_len = width * height * sizeof(uint16);
	display_info.display_rect.x = x;
	display_info.display_rect.y = y;
	display_info.display_rect.w = width;
	display_info.display_rect.h = height;

	display_info.target_rect.x = 0;
	display_info.target_rect.y = 0;
	display_info.target_rect.w = width;
	display_info.target_rect.h = height;


	//if(NULL == s_DirectDrawRotateBuf)
		//s_DirectDrawRotateBuf = (uint32)OEMHeap_Malloc(width*height*2 + 1024);
	
	if(gOemDirectSize.w !=  width || gOemDirectSize.h != height)
	{
		OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  2  realloc memory");
		gOemDirectSize.w =  width ;
		gOemDirectSize.h = height;	
		OEMDisplay_FreeRotateBuf();
		s_DirectDrawRotateBuf = (uint32)OEMHeap_Malloc(width*height*2 + 1024);

		s_DirectDrawDisplayBuf = (uint32)OEMHeap_Malloc(width*height*2 + 1024);
	}

	display_info.display_addr = DC_RIGHT_TRUNK(s_DirectDrawDisplayBuf,256);
	display_info.display_buf_len = DC_RIGHT_TRUNK( width*height,256)*2;


	display_info.rotation_addr = DC_RIGHT_TRUNK(s_DirectDrawRotateBuf,256);
	display_info.rotation_buf_len =DC_RIGHT_TRUNK( width*height,256)*2;

	display_info.src_yuv_addr.y_chn_addr=buf_ptr;
	display_info.src_yuv_buf_len=DC_RIGHT_TRUNK(width*height,256)*2;

	display_info.lcd_id = MAIN_LCD_ID;
	display_info.display_rotation = rot;

	//OEMISP_Open();
	OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  2  x:%d,y:%d,width:%d,height:%d,buf_ptr:%d,rot:%d",x,y,width,height,buf_ptr,rot);
	SXMOBI_DCAMERA_ExtDisplay(&display_info);
#endif	

#ifdef OEMOS_DIRECT_DRAW

 	ISP_DATA_ADDR_T rot_scr_buf={0x00};
    	ISP_DATA_ADDR_T rot_dst_buf={0x00};
	uint32 *rst_lcd_buf =  LCD_GetBlockBuffer( lcd_id, 0);
	BLOCKCFG_T rst_cfg = {0}; 
	int disp_x ,disp_y ,disp_width,disp_height;
	
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  x:%d,y:%d,width:%d,height:%d,buf_ptr:%d,rot:%d",x,y,width,height,buf_ptr,rot);
	#endif
	
      LCD_GetConfigBlock(lcd_id , 0, &rst_cfg);
	cfg.colorkey   = 0;
   	cfg.resolution = LCD_RESOLUTION_RGB565;

	if(gOemDirectSize.w !=  width || gOemDirectSize.h != height)
	{
		gOemDirectSize.w =  width ;
		gOemDirectSize.h = height;	
		OEMDisplay_FreeRotateBuf();
		s_DirectDrawRotateBuf = (uint32)OEMHeap_Malloc(width*height*2 + 1024);
	}

	rot_scr_buf.y_addr = buf_ptr;
	 _ISP_Rotation(&rot_scr_buf, width, height, rot, &rot_dst_buf, ISP_DATA_RGB565);

	 #ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  Rotation end");
	 #endif

	switch(rot)
	{
	case 0:
	case 2:
		cfg.start_x=x;
		cfg.start_y=y;
		cfg.end_x=x+width-1;
		cfg.end_y=y+height-1;

		disp_x = x;
		disp_y = y;
		disp_width = width;
		disp_height = height;
		break;            

	case 1:
	case 3:
		cfg.start_x=x;
		cfg.start_y=y;
		cfg.end_x=x+height-1;
		cfg.end_y=y+width-1;

		disp_x = y;
		disp_y = x;
		disp_width = height;
		disp_height = width;

		break;
        default:
            break;
    }

	cfg.width = cfg.end_x - cfg.start_x + 1;

	
	LCDC_AppSetBGColor(0x0000);
	LCD_ConfigBlock(lcd_id , 0, cfg);
	 
	LCD_DisableBlock (lcd_id,0);
	LCD_DisableBlock (lcd_id,1);
	LCD_DisableBlock (lcd_id,2);
	LCD_DisableBlock (lcd_id,3);
#ifdef PLATFORM_SC8800G
	LCD_DisableBlock (lcd_id,4);
	LCD_DisableBlock (lcd_id,5); 
#endif
	LCD_SetBlockBuffer(lcd_id, 0, (uint32*)rot_dst_buf.y_addr);
	LCD_SetUVBuffer(lcd_id, 0, (uint32*)rot_dst_buf.uv_addr);
	
	LCD_EnableBlock (MAIN_LCD_ID, 0);
  
    
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMDisplay_DirectDrawImg  Rotation disp(%d,%d,%d,%d)end",disp_x,disp_y,disp_width,disp_height);
	#endif

	LCD_InvalidateRect( lcd_id, disp_x, disp_y, disp_x+disp_width-1,disp_y+disp_height-1);

    	LCD_ConfigBlock(lcd_id , 0, rst_cfg);
    LCD_SetBlockBuffer(0, 0, (uint32*)rst_lcd_buf);
#endif


    return ;
}  

BOOLEAN OEMOS_TextCtl_Active();
void OEMDisplay_set_rotate(int r)
{
	//OEMOS_dbgprintf("OEMDisplay_set_rotate  r:%d",r);

	//OEM_YUVDirectPainterDrawWait();
	//ROTATE这种实现效率非常差，要优化

	GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,r);

//	if(OEMOS_TextCtl_Active())
	// {
	 	// MMK_UpdateScreen();
	 //}
	//liuhongmou
	//GUILCD_SetLcdRotMode(GUI_MAIN_LCD_ID, r, GUI_LCD_ROTATE_IN_MTV/*GUILCD_GetLcdRotModule()*/);
}

int OEMDisplay_get_rotate(void)
{
//OEMOS_dbgprintf("OEMDisplay_get_rotate  rot:%d ,ang:%d,APPLE_GetCurrAppID:%d",MMK_GetScreenAngle(),GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID),APPLE_GetCurrAppID());
#ifdef APPLE_ROTATE_DYNAMIC
	
	//if(OEMDisplay_get_rotate)
	return GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
#else 
	return 0;
#endif
}

int OEMDisplay_LCD_SetDirection(int iLcd, uint8 direct)
{
	//OEMOS_dbgprintf("OEMDisplay_LCD_SetDirection :%d",direct);
	LCD_SetDirection(iLcd, direct);
	return 0;
}

/*
bool  g_apple_backlight = TRUE;
void OEMOS_setBacklight(bool bflag)
{

	 g_apple_backlight = bflag;
}

bool OEMOS_GetBacklight()
{
	return g_apple_backlight;
}

*/
void OEMDisplay_BackLight(int n)
{

//	OEMOS_setBacklight(FALSE);	
	switch(n)
	{
	case 0:	//	延时关
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
// 		MMIDEFAULT_TurnOnBackLight();
		break;
	case 1:	//	常亮
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
		break;
	case 2:	//	打开，自然关闭
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
		break;
	case 3:	//	立即关
		MMIDEFAULT_TurnOffBackLight();
		break;
	default :
		break;
	}	
//	OEMOS_setBacklight(TRUE);	
}

void AppleEnableLcdUpdate(void)
{
}

void AppleDisableLcdUpdate(void)
{
}

///////	edit box
MMI_RESULT_E HandleAppleEditBoxWinMsg(
   MMI_WIN_ID_T        win_id, 
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   );


// enum TEXTCTL_ACTION_CODES
// {
//	TEXTCTL_ACTION_DONE=0x00,
//	TEXTCTL_ACTION_CANCEL,
//	TEXTCTL_ACTION_LEAVE
// };

typedef void (*PFNTEXTCTLCB)(void * pUser,int nError);


typedef struct _TextCtl_Imp
{
	PFNTEXTCTLCB callback;
	int InputMode;
	uint16* text_ptr;
	uint32 text_length;
	uint32 iLeaveNotify;
	void *pUser;
}TextCtl_Imp;

static TextCtl_Imp g_ActiveTextCtl;

//////////////////////////////////////////////////////////////////////////

int OEMOS_TextCtl_Show(void *pTextCtl)
{
	return 0;
}

BOOLEAN OEMOS_TextCtl_Active()
{
	return MMK_IsOpenWin(MMIAPPLE_EDITBOX_WIN_ID);
}


WINDOW_TABLE(MMIAPPLE_EDITBOX_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAppleEditBoxWinMsg ),    
    WIN_ID(MMIAPPLE_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_NULL),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */   
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    CREATE_EDIT_TEXT_CTRL(0,MMIAPPLE_EDITBOX_CTRL_ID),
    END_WIN
};



void* OEMOS_TextCtl_Create(char* pTitle,char * pTextBuffer,int nTextMaxSize,int nFlags,int iLeaveNotify,void* callback,void *pUser)
{
	GUIEDIT_INIT_DATA_T	init_data = {0};
	void*             ctrl_ptr = PNULL;
	//GUI_RECT_T ctrl_rect = {MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP, MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM};
	uint8             allow_input_type = GUIIM_TYPE_SET_ALL;//
	uint32            cur_input_mode = GUIIM_TYPE_SMART;   // 
	
	memset(&g_ActiveTextCtl,0,sizeof(TextCtl_Imp));
	g_ActiveTextCtl.InputMode=nFlags;
	g_ActiveTextCtl.text_ptr=(uint16*)pTextBuffer;
	g_ActiveTextCtl.text_length=nTextMaxSize-1;
	g_ActiveTextCtl.callback=(PFNTEXTCTLCB)callback;
	g_ActiveTextCtl.pUser = pUser;
	g_ActiveTextCtl.iLeaveNotify = iLeaveNotify;

	if (!MMK_IsOpenWin(MMIAPPLE_EDITBOX_WIN_ID))
	{
#if 0
		MMI_WINDOW_CREATE_T win_create = {0};

		win_create.applet_handle = MMK_GetFirstAppletHandle();
		win_create.win_id = MMIAPPLE_EDITBOX_WIN_ID;
		win_create.func = HandleAppleEditBoxWinMsg;
		win_create.win_priority = WIN_ONE_LEVEL;
		win_create.add_data_ptr = (ADD_DATA)PNULL;

		if( 0 != MMK_CreateWindow( &win_create ) )
		{
			OEMOS_dbgprintf("OEMOS_TextCtl_Create ok");
		}
		else 
		{
			return NULL;
		}
#else
		MMK_CreateWin((uint32*)MMIAPPLE_EDITBOX_WIN_TAB,PNULL);
#endif
			
	}

	switch(g_ActiveTextCtl.InputMode&0x000000ff)
	{
		case 0x00://text


			break;
		case 0x01://num
			allow_input_type = GUIIM_TYPE_DIGITAL;// 
			cur_input_mode = GUIIM_TYPE_DIGITAL;   // 
			break;
		case 0x02:
			allow_input_type = GUIIM_TYPE_DIGITAL|GUIIM_TYPE_ABC;// 
			cur_input_mode = GUIIM_TYPE_DIGITAL;   // 
			break;
		default:
			break;

	}

	GUIEDIT_SetMaxLen(MMIAPPLE_EDITBOX_CTRL_ID,g_ActiveTextCtl.text_length);
	GUIEDIT_SetIm(MMIAPPLE_EDITBOX_CTRL_ID,allow_input_type,cur_input_mode);
	GUIEDIT_SetString(MMIAPPLE_EDITBOX_CTRL_ID,g_ActiveTextCtl.text_ptr,MMIAPICOM_Wstrlen(g_ActiveTextCtl.text_ptr));

#if 0
	
	{
		//	GUIEDIT_INIT_DATA_T
		GUIEDIT_INIT_DATA_T init_data = {0};
		//GUI_EDITBOX_INIT_DATA_T init_data = {0};
		MMI_CONTROL_CREATE_T create = {0};
		init_data.type = GUIEDIT_TYPE_TEXT;
		//init_data.rect = &ctrl_rect;
		init_data.both_rect.h_rect = init_data.both_rect.v_rect = MMITHEME_GetClientRect();

	    //set init data
	    init_data.type             = GUIEDIT_TYPE_TEXT;
	    init_data.str_max_len      = g_ActiveTextCtl.text_length;
	    init_data.is_rect_adaptive = TRUE;
		
		/*init_data.max_str_len = g_ActiveTextCtl.text_length;
		init_data.wstr_ptr = g_ActiveTextCtl.text_ptr;
		init_data.wstr_len = MMIAPICOM_Wstrlen(init_data.wstr_ptr);
		init_data.add_data.text_add.max_ucs2_len = g_ActiveTextCtl.text_length;
		init_data.add_data.text_add.max_default_len = g_ActiveTextCtl.text_length;
		init_data.add_data.text_add.allow_inputmethod = allow_input_type;
		init_data.add_data.text_add.cur_inputmethod = cur_input_mode;
    		*/
		create.ctrl_id = MMIAPPLE_EDITBOX_CTRL_ID;
		create.guid    = SPRD_GUI_EDITBOX_ID;
		create.parent_win_handle = MMIAPPLE_EDITBOX_WIN_ID; //use win handle here
		create.init_data_ptr = &init_data;
		ctrl_ptr = MMK_CreateControl( &create );

		GUIEDIT_SetIm(create.ctrl_id,allow_input_type,cur_input_mode);
		GUIEDIT_SetString(create.ctrl_id,g_ActiveTextCtl.text_ptr,g_ActiveTextCtl.text_length);
	}
	GUIWIN_CreateTitleDyna(MMIAPPLE_EDITBOX_WIN_ID,TXT_NULL);

	{
			GUISOFTKEY_INIT_DATA_T	softkey_data = {0};
			MMI_CONTROL_CREATE_T softkey_create = {0};
			MMI_CTRL_ID_T	softkey_ctrl_id = 0;

			softkey_data.leftsoft_id	= STXT_OPTION;
			softkey_data.midsoft_id		= TXT_NULL;
			softkey_data.rightsoft_id	= STXT_RETURN;

			softkey_ctrl_id = MMIAPPLE_SOFTKEY_CTRL_ID;
			softkey_create.ctrl_id = softkey_ctrl_id;
			softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
			softkey_create.parent_win_handle = MMIAPPLE_EDITBOX_WIN_ID;
			softkey_create.init_data_ptr = &softkey_data;

			MMK_CreateControl( &softkey_create );
			MMK_SetWinSoftkeyCtrlId(MMIAPPLE_EDITBOX_WIN_ID, softkey_ctrl_id);
	}

	if(ctrl_ptr != PNULL)
	{
		OEMOS_dbgprintf("OEMOS_TextCtl_Create MMK_CreateControl ok");
	}
#endif
	return (void *)&g_ActiveTextCtl;
}

void OEMOS_TextCtl_Destroy(void *pTextCtl)
{
	OEMOS_TextCtl_Close(pTextCtl);
}

int OEMOS_TextCtl_Close(void *pTextCtl)
{
	if(OEMOS_TextCtl_Active())
	MMK_CloseWin(MMIAPPLE_EDITBOX_WIN_ID);
	memset(&g_ActiveTextCtl,0,sizeof(TextCtl_Imp));
	return 0;
}

MMI_RESULT_E HandleAppleEditBoxWinMsg(
   MMI_WIN_ID_T        win_id, 
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   )
{
   MMI_RESULT_E   result   = MMI_RESULT_TRUE;
   MMI_STRING_T   str_info = {0};
   MMI_STRING_T   str_t    = {0};
   MMI_TEXT_ID_T  title_id = TXT_NULL;
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:

	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_OPEN_WINDOW ");
	#endif
	
       title_id = (MMI_TEXT_ID_T)MMK_GetWinAddDataPtr(win_id);
       GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
       MMK_SetAtvCtrl(win_id, MMIAPPLE_EDITBOX_CTRL_ID);
	g_apple_inputState = 0;   
       break;
#ifdef MMI_PDA_SUPPORT
	case MSG_CTL_PENOK:
		if (NULL != param)
		{
			unsigned int src_id = ((MMI_NOTIFY_T*)param)->src_id;
#ifdef APPLE_LOG
		OEMOS_dbgprintf(" src_id  %d ", src_id);
#endif		
			switch(src_id)
			{
			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
#ifdef APPLE_LOG				
				OEMOS_dbgprintf(" OK   ");
#endif
/*
				if(MMK_IsActiveCtrl(MMIAPPLE_EDITBOX_CTRL_ID))
					{
						MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);

					}
				else
					{
						MMK_SendMsg(win_id, MSG_APP_OK, NULL);

					}
*/					

				if(g_apple_inputState == 0)
					{
					MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
					g_apple_inputState = 1;
					}
				else
					{
					MMK_SendMsg(win_id, MSG_APP_OK, NULL);
					g_apple_inputState = 0;
					
					}
				
				break;
			
			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
#ifdef APPLE_LOG				
					OEMOS_dbgprintf(" close   ");
#endif	
				MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
				g_apple_inputState = 1;

				break;
			default:
				break;
			}
		}
		break;
#else
	case MSG_CTL_PENOK:
#endif	  
	   
   case MSG_APP_OK:
   case MSG_CTL_OK:
   case MSG_APP_WEB:



	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_APP_OK ");					
	#endif

	GUIEDIT_GetString(MMIAPPLE_EDITBOX_CTRL_ID, &str_info);	
		
	if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
	{
		int i;
		str_t.wstr_len = str_info.wstr_len;
		str_t.wstr_ptr = str_info.wstr_ptr;

		for(i=0;i<str_t.wstr_len;i++)
		{
			g_ActiveTextCtl.text_ptr[i]=str_t.wstr_ptr[i];
		}
		g_ActiveTextCtl.text_ptr[i] = 0;
	}
	else
	{
		str_t.wstr_len = 0;
		g_ActiveTextCtl.text_ptr[0]=0;
	}

	MMK_CloseWin(win_id);

	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("Input get data len:%d ", str_t.wstr_len);
	#endif

	if(g_ActiveTextCtl.callback)
	{
		g_ActiveTextCtl.callback(g_ActiveTextCtl.pUser,0);
	}

       break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
   	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_APP_CANCEL ");
	#endif
	
	   if(g_ActiveTextCtl.callback)
	   {
		g_ActiveTextCtl.callback(g_ActiveTextCtl.pUser,1);
	   }	
       MMK_CloseWin(win_id);
       break;
   case MSG_CLOSE_WINDOW:
   	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_CLOSE_WINDOW ");
	#endif
	
       break;

	case MSG_GET_FOCUS:
 		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_GET_FOCUS ");
		#endif
		
	//	SXMOBI_MMIAUDIO_Resume();
	//	g_apple_inputSuspend = 2;

		break;

	case MSG_LOSE_FOCUS:
 		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("HandleAppleEditBoxWinMsg MSG_LOSE_FOCUS ");
		#endif
		// add jxh 2012-1-6
		
	if(AppleVCIsRun())
	{
		MMK_CloseWin(win_id);//sxh az 20120105	edit error
		MMK_SendMsg(APPLE_MAIN_WIN_ID, MSG_GET_FOCUS, PNULL);
		MMK_SendMsg(APPLE_MAIN_WIN_ID, MSG_LOSE_FOCUS, PNULL);
	}
		break;
   default:
	if(!MMK_IsActiveCtrl(MMIAPPLE_EDITBOX_CTRL_ID))
 		   	g_apple_inputState = 0;
       result = MMI_RESULT_FALSE;
       break;
   }
   return (result);
}


void  OEMCAM_Switch(void)
{
#ifdef __UM_DUAL_SENSOR__
#ifdef APPLE_LOG	
	OEMOS_dbgprintf("OEMCAM_Switch g_iumCamState %d",g_iCurCamState);
#endif

	if(DCAMERA_IMG_SENSOR_MAIN == g_iCurCamState)
		g_iCurCamState = DCAMERA_IMG_SENSOR_SUB;
	else
		g_iCurCamState = DCAMERA_IMG_SENSOR_MAIN;

#endif
}
// 0 没有1表示有
int  OEMCAM_IsSecond()
{
#ifdef __UM_DUAL_SENSOR__
	return 1;
#else
	return 0;
#endif
}
void OEMCAM_Restore(void)
{

}

void OEMCAM_OPen()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// OEMBitmap 
////////////////////////////////////////////////////////////////////////////////////
#define SXH_APPLE_FONT MMI_DEFAULT_TEXT_FONT




void OEMBitmap_DrawLine(int x11, int y11, int x22, int y22, apple_color c,apple_rect clip_rect)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_COLOR_T line_color;
	GUI_RECT_T box = {0};
	GUI_RECT_T main_clip_rect = {0};
	GUI_RECT_T dest_rect = {0};

	OEM_YUVDirectPainterDrawWait();

	if(x11 > x22)
		{
			box.left = x22;
			box.right = x11;
 
		}
	else
		{
			box.left = x11;
			box.right = x22;
		}

	if(y11>y22)
		{
			box.top = y22;
		 	box.bottom = y11;
		}
	else
		{
		box.top = y11;
		box.bottom = y22;
		}

#ifdef APPLE_DISPLAY_LOG
		OEMOS_dbgprintf("OEMBitmap_DrawLine box.left %d, box.top %d	box.right %d box.bottom %d",	
			box.left,box.top,box.right, box.bottom);
#endif

	main_clip_rect.left = clip_rect.x;
	main_clip_rect.top = clip_rect.y;
	main_clip_rect.right = clip_rect.x + clip_rect.dx -1;
	main_clip_rect.bottom = clip_rect.y + clip_rect.dy - 1 ;

#ifdef APPLE_DISPLAY_LOG
	OEMOS_dbgprintf("OEMBitmap_DrawLine main_clip_rect.left %d, main_clip_rect.top %d  main_clip_rect.right %d main_clip_rect.bottom %d",  
		main_clip_rect.left,main_clip_rect.top,main_clip_rect.right, main_clip_rect.bottom);
#endif

	if(!GUI_IntersectRect(&dest_rect, box, main_clip_rect))
//	if(!OEMBitmap_ExtRect(&dest_rect, box, main_clip_rect))
		return;

	

	line_color = MAKE_BREW_RGB(c.r,c.g,c.b);
	LCD_DrawLine(&lcd_dev_info, dest_rect.left,dest_rect.top,dest_rect.right,dest_rect.bottom, line_color);
	
}

void OEMBitmap_FillRectangle(int x11, int y11, int x22, int y22, apple_color c,apple_rect clip_rect)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_COLOR_T fill_color;

	GUI_RECT_T box = {0};
	GUI_RECT_T main_clip_rect = {0};
	GUI_RECT_T dest_rect = {0};

	OEM_YUVDirectPainterDrawWait();	

	box.left = x11;
	box.top = y11;
	box.right = x22;
	box.bottom = y22;

	main_clip_rect.left = clip_rect.x;
	main_clip_rect.top = clip_rect.y;
	main_clip_rect.right = clip_rect.x + clip_rect.dx - 1;
	main_clip_rect.bottom = clip_rect.y + clip_rect.dy - 1;
	
	if(!GUI_IntersectRect(&dest_rect, box, main_clip_rect))
		return;
	
	fill_color = MAKE_BREW_RGB(c.r,c.g,c.b); 
	
	LCD_FillRect(&lcd_dev_info, 
				dest_rect , 
				fill_color);

}

void OEMBitmap_LCDupdate(apple_rect *prect)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T  rect = {0};
    if(g_apple_update_enable==1)
        return;
	OEM_YUVDirectPainterDrawWait();	
	
	rect.left = prect->x;
	rect.top = prect->y;
	rect.right = prect->x + prect->dx - 1;
	rect.bottom = prect->y + prect->dy - 1;

#ifdef APPLE_DISPLAY_LOG
	OEMOS_dbgprintf("OEMBitmap_LCDupdate rect.left %d, rect.top %d  rect.right %d rect.bottom %d",  
		rect.left,rect.top,rect.right, rect.bottom);
#endif
	if((rect.right<=rect.left)
		||(rect.bottom<=rect.top)
		)
	{
		return;
	}

    MMITHEME_StoreUpdateRect(
        &lcd_dev_info,
        rect);

	MMITHEME_UpdateRect();
}

uint16 OEMBitmap_GetStringWidth(uint16 nFont,     
                                 const uint16* text,
                                 uint16 text_len
                                 )
{
#if defined(SXH_SPT_0839) || defined(SXH_SPT_H7810)
	uint16 rv = 0;
	int i = 0;
	uint8 * str = PNULL;
	
	str = SCI_ALLOC((text_len + 1)  * 2);
	if(PNULL != str)
	{	
    	SCI_MEMSET(str, 0x00, (text_len + 1) * 2);
    	
    	for (i = 0; i < text_len; i++)
    	{
    		str[i*2] = (uint8)((text[i] & 0xff00)>>8);
    		str[i*2 + 1] = (uint8)(text[i] & 0x00ff);
    	}
    	
    	rv = GUI_GetStringWidth(SXH_APPLE_FONT, str, text_len*2, TRUE);
    	
    	if (PNULL != str)
    	{
    		SCI_FREE(str);
    	}	
	}
	return rv;
#else
	return GUI_GetStringWidth(SXH_APPLE_FONT, text, text_len);
#endif
}

uint16 OEMBitmap_GetStringHeight(uint16 nFont,     
                                 const uint16* text,
                                 uint16 text_len
                                 )
{
#if defined(SXH_SPT_0839) || defined(SXH_SPT_H7810)
	uint16 rv = 0;
	int i = 0;
	uint8 * str = PNULL;
	
	str = SCI_ALLOC((text_len + 1)  * 2);	
    if(PNULL != str)
    {	
    	SCI_MEMSET(str, 0x00, (text_len + 1) * 2);
    	
    	for (i = 0; i < text_len; i++)
    	{
    		str[i*2] = (uint8)((text[i] & 0xff00)>>8);
    		str[i*2 + 1] = (uint8)(text[i] & 0x00ff);
    	}
    	
    	rv = GUI_GetStringHeight(SXH_APPLE_FONT, str, text_len*2, TRUE);
    	
    	if (PNULL != str)
    	{
    		SCI_FREE(str);
    	}	
    }
	return rv;
#else
	return GUI_GetStringHeight(SXH_APPLE_FONT, text, text_len);
#endif
}



GUI_FONT_T OEMBITMAP_MapFont(uint16 nFont)
{
	GUI_FONT_T rv = SXH_APPLE_FONT;
	return rv;
}			
				   
void OEMBitmap_DrawText(uint16 nFont, const uint16* wstr_ptr,int nChars,apple_rect box,apple_rect clip, apple_color text_color)
{

	GUI_RECT_T          disp_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
	GUI_RECT_T          clip_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_FONT_T font = MMI_DEFAULT_TEXT_FONT;
	MMI_STRING_T stringT = {0};
	int h;
	GUISTR_STYLE_T          str_style = {0}; /*lint !e64*/


	if(!wstr_ptr )
		return ;

	
	OEM_YUVDirectPainterDrawWait();	

	disp_rect.left = box.x;
	disp_rect.top = box.y;
	disp_rect.right = box.x + box.dx - 1;
	disp_rect.bottom = box.y + box.dy - 1;

	
	clip_rect.left = clip.x;
	clip_rect.top = clip.y;
	clip_rect.right = clip.x + clip.dx - 1;
	clip_rect.bottom = clip.y + clip.dy - 1;
	
	h = OEMBitmap_GetStringHeight(font, wstr_ptr, nChars);

	if((disp_rect.bottom - disp_rect.top) > h)
	{
		disp_rect.bottom =   disp_rect.top + h + 3;
	}

	

	 stringT.wstr_ptr = wstr_ptr;
	 stringT.wstr_len = nChars;
	             //set string style
            str_style.font       = font;
            str_style.font_color = MAKE_BREW_RGB(text_color.r, text_color.g, text_color.b);
            str_style.align      = ALIGN_LEFT;
            str_style.line_color =0;
            str_style.char_space = 0;
		str_style.line_space = 4;

	 
	 GUISTR_DrawTextToLCDInRect( &lcd_dev_info, &disp_rect,&clip_rect,&stringT,&str_style,
	 	GUISTR_STATE_ALIGN /*| GUISTR_STATE_WORDBREAK*/,GUISTR_TEXT_DIR_LTR);

} 

void OEMBitmap_DrawRectangle(int x11, int y11, int x22, int y22, apple_color c,apple_rect clip_rect)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_COLOR_T color;

	GUI_RECT_T box = {0};
	GUI_RECT_T main_clip_rect = {0};
	GUI_RECT_T dest_rect = {0};

	OEM_YUVDirectPainterDrawWait();	

	box.left = x11;
	box.top = y11;
	box.right = x22;
	box.bottom = y22;

	main_clip_rect.left = clip_rect.x;
	main_clip_rect.top = clip_rect.y;
	main_clip_rect.right = clip_rect.x + clip_rect.dx - 1;
	main_clip_rect.bottom = clip_rect.y + clip_rect.dy - 1;

	GUI_IntersectRect(&dest_rect, box, main_clip_rect);
	

	color = MAKE_BREW_RGB(c.r,c.g,c.b); 

	box.left = x11;
	box.top = y11;
	box.right = x22;
	box.bottom = y22;
	
	LCD_DrawRect(&lcd_dev_info, 
				dest_rect , 
				color);
}

//////////////////////////////////////////////////////////////////////////////////////////
// OEMNET
/////////////////////////////////////////////////////////////////////////////////////////
#if 0
#define     ENOBUFS        1
#define     ETIMEDOUT      2
#define     EISCONN        3
#define     EOPNOTSUPP     4
#define     ECONNABORTED   5
#define     EWOULDBLOCK    6
#define     ECONNREFUSED   7
#define     ECONNRESET     8
#define     ENOTCONN       9
//#define     EALREADY       10
#define     EINVAL         11
#define     EMSGSIZE       12
#define     EPIPE          13
#define     EDESTADDRREQ   14
#define     ESHUTDOWN      15
#define     ENOPROTOOPT    16
#define     EHAVEOOB       17
#define     ENOMEM         18
#define     EADDRNOTAVAIL  19
#define     EADDRINUSE     20
#define     EAFNOSUPPORT   21
#define     EINPROGRESS    22
#define     ELOWER         23    // lower layer (IP) error 
#define     ENOTSOCK       24    // Includes sockets which closed while blocked 
#define     EIEIO 27             // bad input/output on Old Macdonald's farm :-)
#define     ETOOMANYREFS   28
#define     EFAULT         29
#define     ENETUNREACH    30
#endif
typedef struct
{
	int err;
	int serr;
} NetErrorMap;



int g_apple_is_use_wifi = 0 ;

int APPLE_GetIsUseWifi()
{
	return g_apple_is_use_wifi;
}

void Apple_SetIsUseWifi(int bUseWifi)
{
	g_apple_is_use_wifi = bUseWifi;
}


// extern MMI_WIN_ID_T gAppleWinId;
//extern MMICMSBRW_SETTING_T	s_connection_setting_info ;

static const NetErrorMap  gsockErrorMap[] = {
											{0/*AEE_NET_SUCCESS*/         ,0},
											//{-1/*AEE_NET_ERROR*/       ,},
											{-2/*AEE_NET_WOULDBLOCK*/     ,EWOULDBLOCK},
											{0x202/*AEE_NET_EBADF*/   ,EINVAL},
											{0x20D/*AEE_NET_EINPROGRESS*/    ,EINPROGRESS},
											{0x20A/*AEE_NET_EOPNOTSUPP*/    ,ENOPROTOOPT},
											{0x214/*AEE_NET_ECONNABORTED*/    ,ECONNABORTED},
											{0x203/*AEE_NET_EFAULT*/,EFAULT},
											{0x215/*AEE_NET_EPIPE*/    ,EPIPE},
											{0x210/*AEE_NET_ENOTCONN*/    ,ENOTCONN},
											{0x213/*AEE_NET_ECONNRESET*/    ,ECONNRESET},
										};

__align(4)  sci_fd_set g_sockfd_conn_set;
__align(4)  sci_fd_set g_sockfd_read_set;
__align(4)  sci_fd_set g_sockfd_write_set;
__align(4)  sci_fd_set g_sockfd_host_set;

//#define APPLE_USE_MMK_TIMER

#define APPLE_SOC_TIMER 300	//ms
uint8 g_iAppleSocTimerId =0 ;
 uint32 g_net_timer_pause = 0;
#ifdef APPLE_USE_MMK_TIMER
#else
	__align(4)  SCI_TIMER_PTR g_net_timer = NULL;
	 void OEMNET_TimerCB(uint32 param);
#endif

//MMI_APPLICATION_T g_apple_app = {HandleApplePsMsg, 0, 0};
//static OEMSocket g_init_socket = {0};
 int g_gprs_retry = 0;
enum 
{
	eGprsDeactived=0,
	eGprsDeactiving,
	eGprsActived,
	eGprsActiving
};
 int32 g_apple_sock_status = eGprsDeactived;
 int32 g_apple_gprs_reactive = 0;  // 0 不用得新激活，1需要



void OEMNET_Reset()
{
#ifdef APPLE_ON_TARGET
	SCI_FD_ZERO(&g_sockfd_conn_set);
	SCI_FD_ZERO(&g_sockfd_read_set);
	SCI_FD_ZERO(&g_sockfd_write_set);
	SCI_FD_ZERO(&g_sockfd_host_set);
	g_net_timer_pause = 0;
#endif

}

 int DSSToAEE_Err( int nMSMErr)
{
	int returnCode=0/*AEE_NET_SUCCESS*/;
	NetErrorMap * pe;
	pe = (NetErrorMap *)gsockErrorMap;
//	if(NULL == pe)
//		return -1;
		
	for(; /* exit below */ ; pe++){
		
		if ( (char*)pe >= ( (char*)gsockErrorMap + sizeof(gsockErrorMap) ) ){
			returnCode = -1/*AEE_NET_ERROR*/;
			break;
		}
		
		if(pe->serr == nMSMErr){
			returnCode = pe->err;
			break;
		}
	}	
	return returnCode;
}

 int OEMNET_GetTimerCB();
 void OEMNET_TimerCB(uint32 param)
{
//	if(!g_net_timer_run)
//		return ;
//	OEMOS_dbgprintf("OEMNET_TimerCB, send msg");
	//OEMOS_dbgprintf("OEMNET_TimerCB");

    //IMPORTANT
	OEMOS_SendOsMsg(12, (uint32)OEMNET_GetTimerCB()/*OEMHttp_SocTimerExpiredInt*/, 0);	//async call, called in mmi task
}

 void OEMNET_SetTimer( uint32 nMSecs)
{
#ifdef APPLE_USE_MMK_TIMER

	if(0 == g_iAppleSocTimerId)
	{
		g_iAppleSocTimerId = MMK_CreateWinTimer(gAppleWinId, nMSecs, FALSE);
	}
	else
	{
		int rv = MMK_StartWinTimer(gAppleWinId, g_iAppleSocTimerId,	nMSecs, FALSE);
		if(!rv)
		{
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("OEMNET_SetTimer, g_iAppleSocTimerId:%d, FAILED!",g_iAppleSocTimerId);
			#endif
			
			MMK_PostMsg(gAppleWinId,MSG_TIMER,(DPARAM)&g_iAppleSocTimerId ,sizeof(g_iAppleSocTimerId));
		}
	}
#else
    	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMNET_SetTimer, nMSecs:%d  g_net_timer:%d ",nMSecs);
	#endif
	
	SCI_ChangeTimer((SCI_TIMER_PTR)g_net_timer, OEMNET_TimerCB, nMSecs);
	SCI_ActiveTimer((SCI_TIMER_PTR)g_net_timer);
#endif
}
void OEMNET_ResumeTimer(void)
{
	if(g_net_timer_pause)
	{
		g_net_timer_pause = 0;
		OEMNET_SetTimer(APPLE_SOC_TIMER);
	}
}
void OEMNET_PauseTimer(void)
{
#ifdef APPLE_USE_MMK_TIMER
	if(0!=g_iAppleSocTimerId)
		MMK_StopTimer(g_iAppleSocTimerId);
	g_iAppleSocTimerId =0;
#else
	if(NULL != g_net_timer)
	{
		g_net_timer_pause = 1;
		SCI_DeactiveTimer(g_net_timer);
	}
#endif
}

void OEMNET_GprsActiveFailNotify()
{
	APPLE_app_soc_notify_ind_struct soc_notify = {0};
	OEM_DNSResult dns_notify = {0};
	int i = 0;

	dns_notify.nResult = 0; // failed

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_GprsActiveFailNotify: c count: %d  r count:%d  w count:%d h count:%d",
		g_sockfd_conn_set.fd_count, g_sockfd_read_set.fd_count,g_sockfd_write_set.fd_count,g_sockfd_host_set.fd_count);
#endif
	
	soc_notify.event_type = APPLE_SOC_EVENT_CONNECT;
	soc_notify.result = 0;		//failed
	for(i=0; i<g_sockfd_conn_set.fd_count; i++)
	{
		OEMSocket * psock = (OEMSocket*) g_sockfd_conn_set.fd_array[i];
		if(NULL == psock)
			break;
		
		soc_notify.socket_id = psock->sockfd;
		APPLE_Socket_Notify((void*)&soc_notify);
	}
	SCI_FD_ZERO(&g_sockfd_conn_set);

	soc_notify.event_type = APPLE_SOC_EVENT_READ;
	soc_notify.result = 0;		//failed
	for(i=0; i<g_sockfd_read_set.fd_count; i++)
	{
		soc_notify.socket_id = g_sockfd_read_set.fd_array[i];
		APPLE_Socket_Notify((void*)&soc_notify);
	}
	SCI_FD_ZERO(&g_sockfd_read_set);

	soc_notify.event_type = APPLE_SOC_EVENT_WRITE;
	soc_notify.result = 0;		//failed
	for(i=0; i<g_sockfd_write_set.fd_count; i++)
	{
		soc_notify.socket_id = g_sockfd_write_set.fd_array[i];
		APPLE_Socket_Notify((void*)&soc_notify);
	}
	SCI_FD_ZERO(&g_sockfd_write_set);

	for(i=0; i<g_sockfd_host_set.fd_count; i++)
	{
		dns_notify.request_id = (int)g_sockfd_host_set.fd_array[i];
		APPLE_GetHostByName_Notify((void*) &dns_notify);
	}
	SCI_FD_ZERO(&g_sockfd_host_set);
	
}

#ifdef MMI_WIFI_SUPPORT //IMPORTANT
extern void Apple_CreateMainWindow();
static void Handle_WifiMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	if(PNULL == msg_ptr)
    {
        return;
    }
#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("HandleOEMNET_GRPSMSG msg_id=%d",msg_ptr->msg_id);
    OEMOS_dbgprintf("HandleOEMNET_GRPSMSG result=%d",msg_ptr->result);
	OEMOS_dbgprintf("HandleOEMNET_GRPSMSG ps_interface=%d",msg_ptr->ps_interface);
#endif
	switch (msg_ptr->ps_interface)//protect 避免:gprs和wifi的消息错误交互
	{
		case MMIPDP_INTERFACE_WIFI:
			if (!APPLE_GetIsUseWifi())
			{
				return;
			}
			break;

		case MMIPDP_INTERFACE_GPRS:
			if (APPLE_GetIsUseWifi())
			{
				return;
			}
			break;

		default:
			return;
	}
    switch(msg_ptr->msg_id) 
    {
    	case MMIPDP_ACTIVE_CNF:
		if(MMIPDP_RESULT_SUCC == msg_ptr->result)	
		{
			MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
			g_apple_sock_status =eGprsActived;
			s_apple_net_id = msg_ptr->nsapi;
	#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GetGprsState SUCCESS  ,s_apple_net_id:%d",s_apple_net_id);    
            OEMOS_dbgprintf("OEMNET_GprsActive APPLE_GetIsUseWifi():%d,MMIAPIWIFI_GetStatus():0x%x",APPLE_GetIsUseWifi,MMIAPIWIFI_GetStatus());
	#endif        
            Apple_CreateMainWindow();
            strcpy(g_back_apn,"wifi");
#if 0
			memset(g_back_apn,0,MMICONNECTION_MAX_APN_LEN + 1);
			setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex(gnnwk_account_id);
			if(setting_item_ptr )
			{
				strcpy(g_back_apn,(char *)setting_item_ptr->apn);  // 备份apn
			}
#endif
		}
		else
		{
		}
		break;
	case MMIPDP_DEACTIVE_CNF:
		break;
	case MMIPDP_DEACTIVE_IND:
		break;
	case MMIPDP_APP_MSG_INVALID:
	case MMIPDP_ACTIVE_REQ:
	case MMIPDP_DEACTIVE_REQ:
	case MMIPDP_SERVICE_CHANGE_IND:
	case MMIPDP_SUSPEND_IND:
	case MMIPDP_RESUME_IND:
	case MMIPDP_ACTIVE_TIMOUT_IND:
	case MMIPDP_REACTIVE_TIMER_IND:
	case MMIPDP_PS_ACTIVE_CNF:
	case MMIPDP_PS_DEACTIVE_CNF:
	case MMIPDP_PS_DEACTIVE_IND:
	break;
    	}
}

extern int APPLE_GetIsUseWifi();
int32 OEMNET_WifiActive(void)
{

	ERR_MNGPRS_CODE_E gprserr = 0;
	MN_DUAL_SYS_E dual_sys = 0;
	
#ifdef APPLE_ON_TARGET
	uint8 setting_index = 0;
   	MMIPDP_ACTIVE_INFO_T app_info = {0};
    	MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
	BOOLEAN return_val = FALSE;

    if (APPLE_GetIsUseWifi())
    {
        app_info.app_handler =MMI_MODULE_SXH_APPLE;
        app_info.handle_msg_callback = Handle_WifiMsg;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    }
    return_val = MMIAPIPDP_Active(&app_info);
#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("OEMNET_WifiActive() active 2  g_apple_sock_status=%d",g_apple_sock_status);
#endif
	if(eGprsActived  == g_apple_sock_status )
	{
		return 0;
	}


	g_apple_sock_status = eGprsActiving;
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMNET_WifiActive, eGprsActiving");
#endif

#endif //#ifdef APPLE_ON_TARGET
    return EINPROGRESS;
}
#endif


void HandleOEMNET_GRPSMSG(MMIPDP_CNF_INFO_T *msg_ptr)
{
	// jxh 2011-12-30
	MN_DUAL_SYS_E dual_sys = g_apple_active_sim;//sxh az  20111229
	if(PNULL == msg_ptr)
    {
        return;
    }



#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("HandleOEMNET_GRPSMSG msg_id=%d",msg_ptr->msg_id);
    OEMOS_dbgprintf("HandleOEMNET_GRPSMSG result=%d",msg_ptr->result);
	OEMOS_dbgprintf("HandleOEMNET_GRPSMSG ps_interface=%d",msg_ptr->ps_interface);
#endif
	switch (msg_ptr->ps_interface)//protect 避免:gprs和wifi的消息错误交互
	{
		case MMIPDP_INTERFACE_WIFI:
			if (!APPLE_GetIsUseWifi())
			{
				return;
			}
			break;

		case MMIPDP_INTERFACE_GPRS:
			if (APPLE_GetIsUseWifi())
			{
				return;
			}
			break;

		default:
			return;
	}


     if(MMIPDP_ACTIVE_CNF == msg_ptr->msg_id )//    switch(msg_ptr->msg_id) 
    {
 //   	case MMIPDP_ACTIVE_CNF:
		if(MMIPDP_RESULT_SUCC == msg_ptr->result)	
		{
			MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
			g_apple_sock_status =eGprsActived;

			s_apple_net_id = msg_ptr->nsapi;
	#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GetGprsState SUCCESS  ,s_apple_net_id:%d",s_apple_net_id);
	#endif
			OEMNET_SetTimer(APPLE_SOC_TIMER);

			memset(g_back_apn,0,MMICONNECTION_MAX_APN_LEN + 1);
			//setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex(gnnwk_account_id);
			setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex(dual_sys,gnnwk_account_id);//sxh az
			if(setting_item_ptr )
			{
				strcpy(g_back_apn,(char *)setting_item_ptr->apn);  // 备份apn
			}
		}
//		else
//		{
			
//		}
//		break;
//	case MMIPDP_DEACTIVE_CNF:
//		break;
//	case MMIPDP_DEACTIVE_IND:
//		break;
    	}
}

//extern int APPLE_GetIsUseWifi();

int32 OEMNET_GprsActive(void)
{

	ERR_MNGPRS_CODE_E gprserr = 0;
	MN_DUAL_SYS_E dual_sys = 0;
	
	int iApnid = gnnwk_account_id;
#ifdef APPLE_ON_TARGET

	if(OEMOS_GetActiveSim()<0)
		return -1;
	dual_sys = OEMOS_GetActiveSim();

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_GprsActive dual_sys:%d",dual_sys);
#endif

//	if(dual_sys < 0)
//		return -1;

	g_apple_gprs_reactive = 0;


	
	switch(/*OEMNET_GetGprsState()*/g_apple_sock_status)
	{
		case eGprsActived:
			
			#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GprsActive  eGprsActived");
			#endif

			if(OEMNET_GprsCheckApn())
			{
				#ifdef APPLE_NET_LOG
				OEMOS_dbgprintf("OEMNET_GprsActive  OEMNET_GprsCheckApn");
				#endif
				//MNGPRS_ResetAndDeactivePdpContextEx(dual_sys);
				MMIAPIPDP_Deactive(MMI_MODULE_SXH_APPLE);
				g_apple_gprs_reactive = 1;
				g_apple_sock_status = eGprsDeactived;
				OEMNET_SetTimer(1500);
				return EINPROGRESS;
			}
			else
			{
				return 0;
			}
			
		case eGprsActiving:
			#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GprsActive eGprsActiving");
			#endif
			return EINPROGRESS;
		case eGprsDeactiving:
			#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GprsActive  eGprsDeactiving");
			#endif
			//MNGPRS_ResetAndDeactivePdpContextEx(dual_sys);
			MMIAPIPDP_Deactive(MMI_MODULE_SXH_APPLE);
			break;
		default:
			break;
	}
	



	{
		uint8 setting_index = 0;
	   	MMIPDP_ACTIVE_INFO_T app_info = {0};
	    	MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
		BOOLEAN return_val = FALSE;

#ifdef MMI_WIFI_SUPPORT
	    if (APPLE_GetIsUseWifi())
	    {
	        app_info.app_handler =MMI_MODULE_SXH_APPLE;
	        app_info.handle_msg_callback = HandleOEMNET_GRPSMSG;
	        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
	    }
	    else
#endif
		{
		
			//setting_index = GetNetSettingIndex(dual_sys);    
			setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex(dual_sys,iApnid);
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GprsActive() active 1  g_apple_sock_status=%d",g_apple_sock_status);
#endif
			if(setting_item_ptr != PNULL)
			{        
			
				app_info.app_handler = MMI_MODULE_SXH_APPLE;
				app_info.auth_type = setting_item_ptr->auth_type;
				app_info.apn_ptr = (char*)setting_item_ptr->apn;
				app_info.user_name_ptr = (char*)setting_item_ptr->username;
				app_info.psw_ptr = (char*)setting_item_ptr->password;
				app_info.dual_sys = dual_sys;
				app_info.priority = 3;
				app_info.ps_service_rat = MN_UNSPECIFIED;
				app_info.handle_msg_callback = HandleOEMNET_GRPSMSG;
				app_info.ps_service_type = IM_E;
				app_info.storage = MN_GPRS_STORAGE_ALL;
			}
			
		}
	     

	       return_val = MMIAPIPDP_Active(&app_info);
#ifdef APPLE_NET_LOG
		   OEMOS_dbgprintf("OEMNET_GprsActive() active pdp context apn = %s,return_val:%d,iApnid:%d,dual_sys:%d",  setting_item_ptr->apn,return_val,iApnid,dual_sys);
			OEMOS_dbgprintf("OEMNET_GprsActive() active 2  g_apple_sock_status=%d",g_apple_sock_status);
#endif
		if(eGprsActived  == g_apple_sock_status )
		{
			return 0;
		}

	}

	g_apple_sock_status = eGprsActiving;
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMNET_GprsActive, OK");
#endif

#endif //#ifdef APPLE_ON_TARGET
    return EINPROGRESS;
}
void OEMNET_GprsDeactive(void)
{
#ifdef APPLE_ON_TARGET
	MN_DUAL_SYS_E sim = (MN_DUAL_SYS_E)OEMOS_GetActiveSim();
	g_apple_sock_status = eGprsDeactiving;
//	if(sim < 0)
//		return ;
	//MNGPRS_ResetAndDeactivePdpContextEx(sim);

	s_apple_net_id = 0;
	MMIAPIPDP_Deactive(MMI_MODULE_SXH_APPLE);
#endif //#ifdef APPLE_ON_TARGET
}

int32 OEMNET_GetGprsState()
{
	int32 result = 0;
	uint32 pdp_con_num = 0;
	int32 current_card_id= OEMOS_GetActiveSim();
	MN_GPRS_CON_STATE_T    *pdp_con_state_arr = NULL;
	int32 i = 0;
	int32 gprs_state = 0;

	
#ifdef _USE_OLD_L4_API
		result = MNGPRS_ReadPdpContextState(&pdp_con_state_arr,&pdp_con_num);
#else
       	 result = MNGPRS_ReadPdpContextStateEx(current_card_id, &pdp_con_state_arr,&pdp_con_num);
#endif

#ifdef APPLE_NET_LOG
				OEMOS_dbgprintf("OEMNET_GetGprsState, result:%d,pdp_con_num:%d,pdp_con_state_arr:0x%x",result,pdp_con_num,pdp_con_state_arr);
#endif

  	if(result != ERR_MNGPRS_NO_ERR || 0 == pdp_con_num)
	{
	    if(pdp_con_state_arr != NULL)
	    {
	        SCI_FREE(pdp_con_state_arr);
	    }
		return eGprsDeactived;
	}


	for(i=0; i < pdp_con_num; i++)
	{
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMNET_GetGprsState, pdp_con_state_arr[%d]  pdp_id:%d, state(%d)",i,pdp_con_state_arr[i].pdp_id,pdp_con_state_arr[i].pdp_state );
#endif
	
		//if(pdp_con_state_arr[i].pdp_id == 1 )
		if(pdp_con_state_arr[i].pdp_id == 2 )
		{
			switch(pdp_con_state_arr[i].pdp_state)
			{
			case MN_CONTEXT_ACTIVATED:
				gprs_state = eGprsActived;
				break;

			case MN_CONTEXT_ACTIVATED_PENDING:
				gprs_state = eGprsActiving;
				break;
			case   MN_CONTEXT_DEACTIVATED:
				gprs_state = eGprsDeactived;
				break;
			case  MN_CONTEXT_DEACTIVATED_PENDING:
				gprs_state = eGprsDeactiving;
				break;
			default:
				gprs_state = eGprsDeactived;
				break;
			}
			
			break;
		}
	}

	 if(pdp_con_state_arr != NULL)
		    {
		        SCI_FREE(pdp_con_state_arr);
		    }

	return gprs_state;
	
}

// 1为不同，0为相同
int32 OEMNET_GprsCheckApn()
{
	uint32     i = 0;  
	int32 ret = 0;
		// jxh 2011-12-30
	MN_DUAL_SYS_E dual_sys = g_apple_active_sim;//sxh az  20111229

	// jxh new connection funtion by 2012-3-12
	MMICONNECTION_LINKSETTING_DETAIL_T* p_settingptr = NULL;	

	char  temp[MMICONNECTION_MAX_APN_LEN + 1]={0};

	p_settingptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys,gnnwk_account_id);
	strcpy(temp,(char *)p_settingptr->apn);
//	strcpy(temp,(char *)s_connection_setting_info .setting_detail[dual_sys][gnnwk_account_id].apn);
	for(i=0; i<strlen(temp); i++)
	{
		if(temp[i] >= 'A' && temp[i] <= 'Z')
			temp[i] = temp[i] + 32;
		
	}

	for(i=0; i<strlen(g_back_apn); i++)
	{
		if(g_back_apn[i] >= 'A' && g_back_apn[i] <= 'Z')
			g_back_apn[i] = g_back_apn[i] + 32;
		
	}

	if(strcmp((char *)temp, (char *)g_back_apn) != 0)
					ret = 1;

	return ret;

#if 0
	MN_GPRS_PDP_CON_T    *pdp_con_arr =  NULL;
	uint32          pdp_con_num = 0;
	uint32     i = 0;  
	int32 result = 0;
	int  current_card_id = OEMOS_GetActiveSim();
	int iApnid = gnnwk_account_id;
	int32 ret = 0;

	if(OEMNET_GetGprsState() == eGprsDeactived)
		return ret;

	#ifdef _USE_OLD_L4_API
		result = MNGPRS_ReadPdpContext(&pdp_con_arr,&pdp_con_num);
        #else
        result = MNGPRS_ReadPdpContextEx(current_card_id, &pdp_con_arr,&pdp_con_num);
        #endif
		
		if(result != ERR_MNGPRS_NO_ERR || 0 == pdp_con_num )
		{
			if(pdp_con_arr != PNULL)
			{
				SCI_FREE(pdp_con_arr);
			}
			return ret;
		}
						
		for(i = 0; i < pdp_con_num; i++)
		{
			if( pdp_con_arr[i].pdp_id == 1)
			{
				if(strcmp((char *)s_connection_setting_info .setting_detail[iApnid].apn, (char *)pdp_con_arr[i].apn) != 0)
					ret = 1;

#ifdef APPLE_NET_LOG
				OEMOS_dbgprintf("OEMNET_GprsCheckApn, pdp_con_arr[i].apn %s",pdp_con_arr[i].apn);
#endif
				break;
			}
		}

		if(pdp_con_arr != PNULL)
		{
			SCI_FREE(pdp_con_arr);
		}
		
		return ret;
#endif		
}


void APPLE_ServiceOpen()
{
 /*
    if(ISP_SERVICE_CLOSE_STATE == ISP_ServiceGetState())
     {
		ISP_ServiceOpen();
     }
    */ 
}


void OEMNET_Clear_hostname(long pme)
{
	// jxh 08-22
	if(SCI_FD_ISSET((long)pme,&g_sockfd_host_set))
	     SCI_FD_CLR((long)pme, &g_sockfd_host_set);

}


void OEMNET_SocTimerExpiredInt(void)
{
#ifdef APPLE_ON_TARGET
	APPLE_app_soc_notify_ind_struct soc_notify = {0};
	
	int i=0,rv=0;       
	int   count=0;     
	sci_fd_set fd_set_in={0},fd_set_out={0};   
	int fd_set_max_size = sizeof(fd_set_in.fd_array)/4;	//12

	if(AppleIsBG())
	{
/*		if(g_sockfd_conn_set.fd_count>0 ||g_sockfd_host_set.fd_count>0 
			|| g_sockfd_read_set.fd_count>0 || g_sockfd_write_set.fd_count>0)
				OEMNET_SetTimer(APPLE_SOC_TIMER);
*/
	   		return;
	}

	
	if(g_apple_gprs_reactive)
	{
			rv = OEMNET_GprsActive();
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt   jxh--OEMNET_GprsActive rv =%d", rv);
#endif	
			return;
	}

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt,    g_apple_sock_status:%d",g_apple_sock_status);
#endif
	
	if(g_apple_sock_status == eGprsActiving)
	{
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt,    g_gprs_retry:%d",g_gprs_retry);
#endif
		g_gprs_retry++;
		if(
			g_gprs_retry < 3 
#ifdef MMI_WIFI_SUPPORT
		&& !APPLE_GetIsUseWifi()
#endif
		 )
		{
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_GprsActive, ReActive Gprs");
#endif
			g_apple_sock_status = eGprsDeactived;
			OEMNET_GprsActive();
		}
		else
		{
			OEMNET_GprsActiveFailNotify();

		}
		return;
	}

	

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt,  g_sockfd_host_set.fd_count %d",  g_sockfd_host_set.fd_count);
#endif

	if( g_sockfd_host_set.fd_count>0 )
	{
		OEM_DNSResult dns_notify = {0};
 		fd_set_in = g_sockfd_host_set;
		
		for(i=0; i<fd_set_max_size; i++)
		{

			char * phostname = NULL;
			
			void *phost = (void *) fd_set_in.fd_array[i];
			if(!SCI_FD_ISSET((long)phost,&g_sockfd_host_set))
				continue;
			
			if(NULL == phost)
			{
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, HOST NULL,%d",i);
#endif
				continue;
			}
			
			phostname = (char *)APPLE_GetHostPtr((void*)phost);
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt phostname %d  phostname %s\r\n", 
			phostname , phostname);
#endif
			if(NULL == phostname)
			{
				dns_notify.nResult = 0; // fail
			}
			else
			{
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("sci_parse_hostr   s_apple_net_id:%d\n",s_apple_net_id);
#endif
				//rv = sci_parse_host((char*)phostname, (unsigned long *)&(dns_notify.addrs[0]), 1);
				rv = sci_parse_host_ext((char*)phostname, (unsigned long *)&(dns_notify.addrs[0]), 1,s_apple_net_id);
				if(rv == 0)
				{
					dns_notify.nResult = 1;
				}
				else
				{
					dns_notify.nResult = 0; // fail
				}

			
#ifdef APPLE_NET_LOG
					OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt,  host finish rv:%d, ip:0x%08x, host:%s", rv, dns_notify.addrs[0],phostname);
#endif
			}

			SCI_FD_CLR((long)phost, &g_sockfd_host_set);
			dns_notify.request_id = (int)phost;
			APPLE_GetHostByName_Notify((void*) &dns_notify);
		}
	}

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt,  g_sockfd_conn_set.fd_count %d",  g_sockfd_conn_set.fd_count);
#endif

	
	if( g_sockfd_conn_set.fd_count>0 )
	{
 		fd_set_in = g_sockfd_conn_set;
		
		for(i=0; i<fd_set_max_size; i++)
		{
			struct sci_sockaddr sockaddr; 
			OEMSocket * psock = (OEMSocket*) fd_set_in.fd_array[i];
			if(!SCI_FD_ISSET((long)psock,&g_sockfd_conn_set))
				continue;
			if(NULL == psock)
			{
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, CONN NULL,%d",i);
#endif
				continue;
			}
				memset(&sockaddr,0,sizeof(struct sci_sockaddr));	//clear sockaddr
			sockaddr.family = AF_INET;
			sockaddr.port = psock->port;					//set current server's service port
			sockaddr.ip_addr = psock->addr;
			memset((void*)sockaddr.sa_data, 0, (8*sizeof(char)));
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt to conn:%x %d sockfd:0x%x \r\n", 
				sockaddr.ip_addr,
				sockaddr.port,
				psock->sockfd);
#endif
 			rv = sci_sock_connect((long)psock->sockfd, &sockaddr, sizeof(struct sci_sockaddr));
			if (rv < 0)
			{
				int errcode = sci_sock_errno((long) psock->sockfd);
#ifdef APPLE_NET_LOG
				OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, reconn rv:%d err:%d\r\n", rv,errcode);
#endif
				if (errcode == EWOULDBLOCK || errcode==EINPROGRESS)
					continue;
				soc_notify.result = 0;//DSSToAEE_Err(rv);
			}
			else
			{
				soc_notify.result = 1;
			}
			SCI_FD_CLR((long)psock, &g_sockfd_conn_set);
			soc_notify.event_type = APPLE_SOC_EVENT_CONNECT;
			soc_notify.socket_id = psock->sockfd;
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, reconn finish, rv:%d, sock:0x%x,type:%d, result:%d%\r\n", 
				rv, soc_notify.socket_id,soc_notify.event_type, soc_notify.result);
#endif
			APPLE_Socket_Notify((void*)&soc_notify);
		}
	}
	
	
	if( g_sockfd_read_set.fd_count>0 || g_sockfd_write_set.fd_count>0)
	{
		fd_set_in = g_sockfd_read_set;
		fd_set_out = g_sockfd_write_set;
		count = sci_sock_select(&fd_set_in, &fd_set_out, NULL, 0);
#ifdef APPLE_NET_LOG		
		OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, r&w active fd max:%d, count:%d, r:%d, w%d, r set:%d,w set:%d", 
			fd_set_max_size, count, fd_set_in.fd_count, fd_set_out.fd_count,
			g_sockfd_read_set.fd_count,
			g_sockfd_write_set.fd_count);
#endif		
		for(i=0; i<fd_set_max_size; i++)
		{
			if(!SCI_FD_ISSET(fd_set_in.fd_array[i],&g_sockfd_read_set))
				continue;
			soc_notify.socket_id = fd_set_in.fd_array[i];
			soc_notify.event_type = APPLE_SOC_EVENT_READ;
			SCI_FD_CLR(fd_set_in.fd_array[i], &g_sockfd_read_set);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("APPLE_Socket_Notify   i:%d",i);
#endif
			APPLE_Socket_Notify((void*)&soc_notify);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("APPLE_Socket_Notify   endi");
#endif
		}
		for(i=0; i<fd_set_max_size; i++)
		{
			if(!SCI_FD_ISSET(fd_set_out.fd_array[i],&g_sockfd_write_set))
				continue;
			soc_notify.socket_id = fd_set_out.fd_array[i];
			soc_notify.event_type = APPLE_SOC_EVENT_WRITE;
			SCI_FD_CLR(fd_set_out.fd_array[i], &g_sockfd_write_set);
			APPLE_Socket_Notify((void*)&soc_notify);
		}
	}
	
	if(g_sockfd_conn_set.fd_count>0 || g_sockfd_read_set.fd_count>0 || g_sockfd_write_set.fd_count>0)
		OEMNET_SetTimer(APPLE_SOC_TIMER);

#ifdef APPLE_NET_LOG		
	OEMOS_dbgprintf("OEMNET_SocTimerExpiredInt, read set count:%d, write set cnt:%d", 
		g_sockfd_read_set.fd_count, g_sockfd_write_set.fd_count);
#endif		

#endif //#ifdef APPLE_ON_TARGET
}



/*===========================================================================

  FUNCTION OEMSocket_New
  
	DESCRIPTION
	Allocate a socket related data structure and remember it
	
===========================================================================*/
static OEMSocket* OEMSocket_New(void)
{
	OEMSocket* me;
	me = OEM_Heap_Malloc_DBG(sizeof(OEMSocket));
	memset(me,0,sizeof(OEMSocket));
	
	return me;
}

/*===========================================================================

  FUNCTION OEMSocket_Delete
  
	DESCRIPTION
	Forget and free a socket related data structure
	
===========================================================================*/
static void OEMSocket_Delete(OEMSocket* me)
{
	if(me)
	{
		OEMHeap_Free((void*)me);
		me = NULL;
	}
}

int OEMSocket_Inet_aton(char* ip_str, uint32* ipaddr_ptr)
{
	if(ip_str != NULL && ipaddr_ptr != NULL)
	{
#ifdef APPLE_NET_LOG
   OEMOS_dbgprintf("OEMSocket_Inet_aton -----ip_str != NULL && ipaddr_ptr != NULL----\r\n");
#endif	
		return inet_aton((char*)ip_str, (uint32*)ipaddr_ptr);
	}

	return 0;
}

uint16 OEMSocket_htons(uint16 val)
{
	return htons(val);
}


//AEE_SOCK_STREAM=0,   // TCP - streaming socket
//AEE_SOCK_DGRAM       // UDP - datagram socket
OEMCONTEXT OEMSocket_Open( int8 type, int *err, uint32 nwk_account_id)
{
   int32 sockfd;
   OEMSocket* me = NULL;
   byte dmss_type;

#ifdef APPLE_ON_TARGET
   switch (type) 
   {
   case 0 : //AEE_SOCK_STREAM
	   dmss_type = APPLE_SOCKET_STREAM;
	   break;
	   //       case 1: //AEE_SOCK_DGRAM  
	   // 		dmss_type = APPLE_SOCKET_DGRAM;
	   // 		break;
   default:
	   *err = DSSToAEE_Err(ENOPROTOOPT);
	   return (OEMCONTEXT)NULL;
   }

   me = OEMSocket_New();
   if ((OEMSocket*)0 == me) {
	   *err = -1;
	   return (OEMCONTEXT)NULL;
   }

// 	OEMNET_RegNetEvt();

#ifdef APPLE_NET_LOG
   OEMOS_dbgprintf("OEMSocket_Open - nwk_account_id:%d dmss_type:%d \r\n", nwk_account_id,dmss_type);
#endif

#ifdef APPLE_NET_LOG
   OEMOS_dbgprintf("OEMSocket_Open -s_apple_net_id:%d \r\n", s_apple_net_id);
#endif
	
	sockfd = sci_sock_socket(AF_INET, SOCK_STREAM, 0,s_apple_net_id);
	if(sockfd<0){
		OEMSocket_Delete(me);
		*err = DSSToAEE_Err(sockfd);
		return (OEMCONTEXT)NULL;
	} else {
		
   }


    /* Set ansync mode */
	sci_sock_setsockopt(sockfd, SO_NBIO, NULL);

	*err = DSSToAEE_Err(0);

	me->sockfd = sockfd;


#ifdef APPLE_NET_LOG 
	OEMOS_dbgprintf("OEMSocket_Open, me->sockfd:0x%x", me->sockfd);
#endif
	
	
//	if (0/*AEE_SOCK_STREAM*/ == type) {
		me->bStream = TRUE;
//	}
//	else
//	{
//		me->bStream = FALSE;
//	}
#endif //#ifdef APPLE_ON_TARGET
   return (OEMCONTEXT)me;

}

int OEMSocket_Connect( OEMCONTEXT  sockd, uint32  addr, uint16 port)
{
	int rv = -1;
#ifdef APPLE_ON_TARGET
	OEMSocket* me = (OEMSocket*)sockd;
	struct sci_sockaddr sockaddr;                    /* destination address */
	int result;
	
#ifdef APPLE_NET_LOG		
		OEMOS_dbgprintf("OEMSocket_Connect  %s:%d", __FILE__,__LINE__);
#endif
	
	
	if ((OEMSocket*)0 == me) {
		return -1;
	}

	me->addr = addr;
	me->port = port;

// 	if(g_apple_sock_status == eGprsActiving)
// 		return DSSToAEE_Err(EINPROGRESS);
#ifdef APPLE_NET_LOG			
		OEMOS_dbgprintf("OEMSocket_Connect  %s:%d", __FILE__,__LINE__);
#endif

	rv = OEMNET_GprsActive();
	if(EINPROGRESS == rv)
	{
	#ifdef APPLE_NET_LOG			
		OEMOS_dbgprintf("OEMSocket_Connect  %s:%d  rv:%d", __FILE__,__LINE__,rv);
#endif
// 		g_init_socket = *me;
		if(SCI_FD_ISSET((long)(me), &g_sockfd_conn_set))
		{
			SCI_FD_CLR((long)(me), &g_sockfd_conn_set);
		}
		SCI_FD_SET((long)(me), &g_sockfd_conn_set);	//	waiting for retry
		return DSSToAEE_Err(EWOULDBLOCK);
	}
	else if(0 != rv)
	{
#ifdef APPLE_NET_LOG			
		OEMOS_dbgprintf(" OEMSocket_Connect  %s:%d  rv:%d", __FILE__,__LINE__,rv);
#endif

		return DSSToAEE_Err(rv);
	}
	memset(&sockaddr,0,sizeof(struct sci_sockaddr));	//clear sockaddr
	sockaddr.family = AF_INET;
    sockaddr.port = port;					//set current server's service port
	sockaddr.ip_addr = addr;
    memset((void*)sockaddr.sa_data, 0, (8*sizeof(char)));

	result = 0;

	rv = sci_sock_connect((long)me->sockfd, &sockaddr, sizeof(struct sci_sockaddr));
#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("OEMSocket_Connect %x %d ret %d sockfd:0x%x \r\n", 
		sockaddr.ip_addr,
		sockaddr.port,
		rv,
		me->sockfd);
#endif
	if (rv < 0)
	{
		int errcode = sci_sock_errno((long) me->sockfd);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMSocket_Connect rv:%d err:%d, timer:%d\r\n", rv,errcode, g_iAppleSocTimerId);
#endif
        if (errcode == EWOULDBLOCK || errcode==EINPROGRESS)
        {
        	if(SCI_FD_ISSET((long)(me), &g_sockfd_conn_set))
		{
			SCI_FD_CLR((long)(me), &g_sockfd_conn_set);
		}
		SCI_FD_SET((long)(me), &g_sockfd_conn_set);

		OEMNET_SetTimer(APPLE_SOC_TIMER);
#ifdef APPLE_NET_LOG			
		OEMOS_dbgprintf("OEMSocket_Connect, create timer:%d", g_iAppleSocTimerId);
#endif
            return DSSToAEE_Err(EWOULDBLOCK);
        }
        else
        {
            return DSSToAEE_Err(rv);
        }
	}
#endif //#ifdef APPLE_ON_TARGET
	return 0;
}

int OEMSocket_Shutdown(OEMCONTEXT sockd, int32 how) 
{
#ifdef APPLE_ON_TARGET
	OEMSocket* me = (OEMSocket*)sockd;

	if (NULL == me) 
		return -1;

#ifdef APPLE_NET_LOG 
	OEMOS_dbgprintf("OEMSocket_Shutdown, me->sockfd:0x%x", me->sockfd);
#endif

	sci_sock_shutdown(me->sockfd, how);
#endif //#ifdef APPLE_ON_TARGET
    return 0;
}

int OEMSocket_Close(OEMCONTEXT sockd)
{
	OEMSocket* me = (OEMSocket*)sockd;
	int8 result = 0;

#ifdef APPLE_ON_TARGET
	if (NULL == me ) 
		return -1;
	
#ifdef APPLE_NET_LOG 
	OEMOS_dbgprintf("OEMSocket_Close, me->sockfd:0x%x", me->sockfd);
#endif
	
	if( SCI_FD_ISSET((long)me,&g_sockfd_conn_set) )
		SCI_FD_CLR((long)me, &g_sockfd_conn_set); 
	if( SCI_FD_ISSET((long)me->sockfd,&g_sockfd_read_set) )
		SCI_FD_CLR(me->sockfd, &g_sockfd_read_set); 
	if( SCI_FD_ISSET((long)me->sockfd,&g_sockfd_write_set) )
		SCI_FD_CLR(me->sockfd, &g_sockfd_write_set); 
	if(me->sockfd != 0)
		sci_sock_socketclose((long) me->sockfd);

	me->sockfd =0;

	OEMSocket_Delete(me);
#endif //#ifdef APPLE_ON_TARGET
	return DSSToAEE_Err(result);
}

int32 OEMSocket_Read(OEMCONTEXT sockd, byte *buffer, uint32 nbytes, int *err) 
{
	OEMSocket* me = (OEMSocket*)sockd;
	int      Ret;
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("sci_sock_recv sockfd:%0x buffer:%p nbytes:%d   entry\r\n",me->sockfd,buffer,nbytes);
#endif
#ifdef APPLE_ON_TARGET	
	if (NULL == me || NULL == buffer) 
	{
		if(NULL != err)
			*err = -1;
		
		return -1;
	}
// 	if(g_apple_sock_status == eGprsDeactived)
// 	{
// 		*err=DSSToAEE_Err(EPIPE)
// 		return -1;
// 	}
	
	Ret = sci_sock_recv(me->sockfd, (char *)buffer, (int)nbytes, 0);
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("sci_sock_recv sockfd:%0x buffer:%p nbytes:%d Ret:%d\r\n",me->sockfd,buffer,nbytes,Ret);
#endif
	if (Ret < 0)
	{
		int errcode = sci_sock_errno((long) me->sockfd);
#ifdef APPLE_NET_LOG	
		OEMOS_dbgprintf("sci_sock_recv err:%d \r\n", errcode);
#endif
        if (errcode == EWOULDBLOCK)
        {
		if(SCI_FD_ISSET(me->sockfd,&g_sockfd_read_set))
		{
			SCI_FD_CLR(me->sockfd, &g_sockfd_read_set);
		}
		SCI_FD_SET(me->sockfd, &g_sockfd_read_set);
		
		OEMNET_SetTimer(APPLE_SOC_TIMER);
            return (*err=DSSToAEE_Err(EWOULDBLOCK));
        }
        else
        {
            return (*err=-1);
        }
	}

	*err=0;
#endif //#ifdef APPLE_ON_TARGET
	return Ret; 
}

int32 OEMSocket_Write(OEMCONTEXT sockd, const byte *buffer, uint32 nbytes, int *err)
{
	OEMSocket* me = (OEMSocket*)sockd;
	int write_len = 0;

	if(NULL == me || buffer == NULL)
	{
		if(NULL != err)
			*err = -1;

		return -1;
	}
	
// 	if(g_apple_sock_status == eGprsDeactived)
// 	{
// 		*err=DSSToAEE_Err(EPIPE)
// 			return -1;
// 	}
#ifdef APPLE_ON_TARGET
    write_len = sci_sock_send( me->sockfd, (char*) buffer, (int)nbytes, 0);
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OS(soc_send) sockfd:%0x buffer:%p nbytes:%d write_len:%d\r\n",
					me->sockfd,buffer,nbytes,write_len);
#endif

    if (write_len < 0)
    {
	  int errcode = sci_sock_errno((long) me->sockfd);
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OS(soc_send) sockfd:%0x err:%d\r\n", errcode);
#endif

        if (errcode == EWOULDBLOCK || ENOBUFS==errcode)
        {
			if(SCI_FD_ISSET(me->sockfd,&g_sockfd_write_set))
			{
				SCI_FD_CLR(me->sockfd, &g_sockfd_write_set);
			}
			SCI_FD_SET(me->sockfd, &g_sockfd_write_set);
			OEMNET_SetTimer(APPLE_SOC_TIMER);
            return (*err=DSSToAEE_Err(EWOULDBLOCK));
        }
        else
        {
            return (*err=-1);
        }
    }
	*err=0;
#endif //#ifdef APPLE_ON_TARGET
    return write_len;
}

int OEMSocket_GetLastErr(OEMCONTEXT* sockd)
{
	int result=0,error;
#ifdef APPLE_ON_TARGET
	OEMSocket* me = (OEMSocket*)sockd;
	if(NULL==me)
		return 0;

	error = sci_sock_errno((long)me->sockfd);
	result = DSSToAEE_Err(error);
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMSocket_GetLastErr result:%d error:%d\r\n",result, error);
#endif
#endif //#ifdef APPLE_ON_TARGET	
   return(result);
}


static char ipnumber[16] = {0};
char *OEMSocket_getipnumber(OEMCONTEXT sockd, int local)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef APPLE_ON_TARGET
    long result;
	OEMSocket* me = (OEMSocket*)sockd;
	char* addr = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    result = sci_gethostaddress();
    if (result == 0)
    {
        return NULL;
    }

    /* Only support IP4 */
	addr = (char*)&result;
    sprintf(ipnumber, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
#endif //#ifdef APPLE_ON_TARGET
    return ipnumber;
}

int32 OEMSocket_GetSockName(OEMCONTEXT sockd, uint32 * pAddr, uint16 * pwPort)
{
#ifdef APPLE_ON_TARGET
	struct sci_sockaddr addr={0};
	sci_sock_getsockname(((OEMSocket*)sockd)->sockfd, &addr);
	*pAddr = addr.ip_addr;
	*pwPort = addr.port;
#endif //#ifdef APPLE_ON_TARGET
	return 0;
}

int32 OEMSocket_GetPeerName(OEMCONTEXT sockd, uint32 * pAddr, uint16 * pwPort)
{
#ifdef APPLE_ON_TARGET
	struct sci_sockaddr addr={0};
	sci_sock_getpeername(((OEMSocket*)sockd)->sockfd, &addr);
	*pAddr = addr.ip_addr;
	*pwPort = addr.port;
#endif //#ifdef APPLE_ON_TARGET
	return 0;
}



int32 OEMSocket_SendTo
(
 OEMCONTEXT            sockd,     /* socket descriptor */
 const byte           *buffer,    /* user buffer from which to copy the data */
 uint32                nbytes,    /* number of bytes to be written */
 uint16                flags,     /* data transport options */
 uint32                addr,      /* IP address: network byte order */
 uint16                port,      /* port number: network byte order */
 int                 *err
)
{

	OEMSocket* me = (OEMSocket*)sockd;
	struct sci_sockaddr sockaddr;                          /* destination address */
	int32 Ret = 0;
	uint16 ds_flags = 0;
#ifdef APPLE_ON_TARGET
	sockaddr.family = AF_INET;
    sockaddr.port = port;					//set current server's service port
	sockaddr.ip_addr = addr;
	
    memset((void*)sockaddr.sa_data, 0, (8*sizeof(char)));

	if (NULL == me) {
		return -1;
   }	
	*err = 0;
	Ret = sci_sock_sendto(me->sockfd, (char *)buffer,(int)nbytes,0,&sockaddr);
	if(Ret < 0)
	{
		*err = DSSToAEE_Err(Ret);
		return -1;
	}
#endif //#ifdef APPLE_ON_TARGET
	return Ret;
}

int32 OEMSocket_RecvFrom
(
 OEMCONTEXT        sockd,                         /* socket descriptor */
 byte             *buffer,  /* user buffer from which to copy the data */
 uint32            nbytes,            /* number of bytes to be written */
 uint16            flags,             /* unused */
 uint32           *addr,             /* IP address: network byte order */
 uint16           *port,            /* port number: network byte order */
 int            *err
 ) {

	OEMSocket* me = (OEMSocket*)sockd;
	struct sci_sockaddr sockaddr;  
	int32 Ret = 0;
#ifdef APPLE_ON_TARGET
    memset((void*)sockaddr.sa_data, 0, (8*sizeof(char)));

	*err = 0;
	Ret = sci_sock_recvfrom(me->sockfd, (char*)buffer, (int)nbytes, flags, &sockaddr);
	if(Ret < 0)
	{
		*err = DSSToAEE_Err(Ret);
		return -1;
	}
	*addr = sockaddr.ip_addr;
	*port = sockaddr.port;
#endif //#ifdef APPLE_ON_TARGET
	return Ret;
}

int32 OEMNET_GetMyIPAddr()
{
#ifdef APPLE_ON_TARGET
	return sci_gethostaddress();
#else
	return 0;
#endif //#ifdef APPLE_ON_TARGET
}

int OEMSocket_getipnumber_byename(const char *host,int request_id, void *pres,uint32 nwk_account_id)
{
#ifdef APPLE_ON_TARGET
	int rv =0; 
	OEM_DNSResult *dns = (OEM_DNSResult*)pres;
	struct sci_hostent *phost = NULL;

	if((OEM_DNSResult*)0 == dns)
		return -1;

	if(NULL == host)
		return -1;
	

	rv = OEMNET_GprsActive();
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("jxh--OEMSocket_getipnumber_byename OEMNET_GprsActive,  rv:%d", rv);
#endif

	if(EINPROGRESS == rv)
	{
		if(SCI_FD_ISSET((long)(request_id), &g_sockfd_host_set))
		{
			SCI_FD_CLR((long)(request_id), &g_sockfd_host_set);
		}
		SCI_FD_SET((long)(request_id), &g_sockfd_host_set);	//	waiting for retry
		return DSSToAEE_Err(EWOULDBLOCK);
	}
	else if(0 != rv)
		return DSSToAEE_Err(rv);
	
	//rv = sci_parse_host((char*)host, (unsigned long *)&(dns->addrs[0]), 1);
	rv = sci_parse_host_ext((char*)host, (unsigned long *)&(dns->addrs[0]), 1,s_apple_net_id);
	if(rv == 0 )
	{
		dns->nResult  = 1;
	}
	else
	{
		dns->nResult  = 0;
	}

//	dns->nResult  = 1;
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMSocket_getipnumber_byename,  rv:%d, ip:0x%08x, host:%s", rv, dns->addrs[0],host);
#endif
	
#endif //#ifdef APPLE_ON_TARGET	
	return 0;
}

//extern MMICMSBRW_SETTING_T	s_connection_setting_info ;
uint32 OEMOS_GPRSDataAccountNode_GetSize()
{
	// jxh 2011-12-30
	MN_DUAL_SYS_E dual_sys = g_apple_active_sim;//sxh az  20111229

#ifdef APPLE_ON_TARGET
//	return s_connection_setting_info .settinglink_num[dual_sys];
    return MMIAPICONNECTION_GetLinkSettingNum(dual_sys);
#else
	return 0;
#endif //#ifdef APPLE_ON_TARGET
}

uint32 OEMNET_GetDataAccount(uint32 index, void *pNode)
{
	// jxh 2011-12-30
	MN_DUAL_SYS_E dual_sys = g_apple_active_sim;//sxh az  20111229

#ifdef APPLE_ON_TARGET
	uint32 n;
	OEM_GPRSDataAccountNode *p = (OEM_GPRSDataAccountNode*)pNode;

	// jxh new connection funtion by 2012-3-12
	MMICONNECTION_LINKSETTING_DETAIL_T* p_settingptr = NULL;
	
	memset(p, 0, sizeof(OEM_GPRSDataAccountNode));

	p_settingptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

#ifdef MMI_WIFI_SUPPORT
	if (APPLE_GetIsUseWifi())
	{
		p->id = index;
		MMIAPICOM_StrToWstr("cmnet",(uint16*)p->APN);
			// jxh 2011-12-30
		MMIAPICOM_Wstrncpy((uint16*)p->DataAccountName, (uint16*)p_settingptr->name,p_settingptr->name_len);
		inet_aton((char*)p_settingptr->gateway, (uint32*)&(p->prx_ip));
		//	p->prx_ip = htonl(p->prx_ip);

		memset(p->prx_ip,0,sizeof(p->prx_ip));
		p->prx_port = 0;
		p->result = 0;

#ifdef APPLE_NET_LOG			
	OEMOS_dbgprintf("OEMNET_GetDataAccount   usewifi   idx:%d,apn:%s,ip:%s,port:%d",p->id,
		"cmnet",
		p_settingptr->gateway,
		p->prx_port);
#endif


		return 0;
	}


#endif		
	p->id = index;
		// jxh 2011-12-30
if((MMIPHONE_GetNetworkName(g_apple_active_sim)==PLMN_UNICOM)//sxh az 20111229
    && (stricmp((char*)p_settingptr->apn, "uniwap") == 0))
{
    MMIAPICOM_StrToWstr((uint8*)"cmwap",(uint16*)p->APN);
#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("convert uniwap to cmwap");
#endif
}
else if((MMIPHONE_GetNetworkName(g_apple_active_sim)==PLMN_UNICOM)
    && (stricmp((char*)p_settingptr->apn, "uninet") == 0))
{
    MMIAPICOM_StrToWstr((uint8*)"cmnet",(uint16*)p->APN);
#ifdef APPLE_NET_LOG
    OEMOS_dbgprintf("convert uninet to cmnet");
#endif   
}
else
{
	MMIAPICOM_StrToWstr((uint8*)p_settingptr->apn,(uint16*)p->APN);
}
	MMIAPICOM_Wstrcpy((uint16*)p->DataAccountName, (uint16*)p_settingptr->name);
	inet_aton((char*)p_settingptr->gateway, (uint32*)&(p->prx_ip));
//	p->prx_ip = htonl(p->prx_ip);
	p->prx_port = htons(p_settingptr->port);
	p->result = 0;


	
	
#ifdef APPLE_NET_LOG			
	OEMOS_dbgprintf("OEMNET_GetDataAccount idx:%d,apn:%s,ip:%s,port:%d",p->id,
		p_settingptr->apn,
		p_settingptr->gateway,
		p->prx_port);
#endif
#endif //#ifdef APPLE_ON_TARGET
	return 0;
}

uint32 OEMOS_GPRSDataAccountNode_GetNode(uint32 index)
{

	return 0;
}

void OEM_get_nitz_nw_name(int *nw)
{

}

//////////////////////////////////////////////////////////////////////////////////////
// OEMFILE
/////////////////////////////////////////////////////////////////////////////////////

// #define OEMFS_Aplfs

//#include "E:\project\spreadtrum6800h\mini_core\oem\fs\aplfs.h"
#include "aplfs.h"

static uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH];

typedef struct
{
   int err;
   int ferr;
} ErrorMap;

typedef struct
{
	OEMFSInfo				pub;
	char					szFile[OEMFS_FILENAME_MAX_LENGTH];
	int						nErr;
	SFS_FIND_DATA_T			fd;
	SFS_HANDLE				handle;

} EnumContext;

static   int gLastErr = 0;

// static const ModeMap          gsModeMap[] = {{_OFM_READ, FFS_MODE_READ},
//                                              {_OFM_READWRITE, FFS_MODE_READ|FFS_MODE_WRITE},
//                                              {_OFM_CREATE, FFS_MODE_CREATE_ALWAYS|FFS_MODE_WRITE},
//                                              {_OFM_APPEND, FFS_MODE_APPEND},
//                                              {(int8)0, (UINT)0}};

static const ErrorMap         gsErrorMap[] = {
												{0 /*SUCCESS*/         ,SFS_ERROR_NONE},
												{0x100 /*EFILEEXISTS*/       ,SFS_ERROR_HAS_EXIST},
												{0x101 /*EFILENOEXISTS*/     ,SFS_ERROR_NOT_EXIST},
												{0x102 /*EDIRNOTEMPTY*/   ,SFS_ERROR_ACCESS},
												{14 /*EBADPARM*/ , SFS_ERROR_INVALID_PARAM},
//                                              {EBADSEEKPOS    ,},
//                                              {EFILEEOF        ,},
												{0x106 /*EFSFULL*/    ,SFS_ERROR_NO_SPACE}};

static int xlate_err( int nDMSS );

int OEMFS_TranslateFileInfo(void * pInfo, const SFS_FIND_DATA_T* fd);

static void Log_WriteHex(unsigned char *log_buffer, unsigned int size)
{
	return;
// 
// 	char temp[100];
// 	int nOffset=0;
// 	while(size>=4)
// 		{
// 			APPLE_Std_Sprintf(temp, "%02x %02x %02x %02x", log_buffer[nOffset],log_buffer[nOffset+1],
// 				log_buffer[nOffset+2],
// 				log_buffer[nOffset+3]);
// 
// 			OEMOS_dbgprintf("%s",temp);
// 
// 			nOffset+=4;
// 			size-=4;
// 		}
// 
// 	
}

/*==================================================================
Function: OEMFS_GetLastError

Description: Gets the last error produced by the OEMFS subsystem.
             If the last operation was successful, this function
             will return success.
==================================================================*/
int OEMFS_GetLastError ( void )
{
   return(gLastErr);
}



/*==================================================================
Function: OEMFS_Open

Description: Opens a file in the EFS.
==================================================================*/

static int g_oemfs_cachesize = 0;

void* OEMFS_Open(const char *szFilename,int8 nMode)
{
	
	SFS_HANDLE handle = 0;
	int returnCode;
	uint32 flags;
	uint16 attr;
#ifdef OEMFS_Aplfs
	STAplfs *pstAplfs = NULL;
#endif

	handle=0;
	returnCode=0;
	flags=0;

	if (NULL==szFilename||(strlen(szFilename) >= OEMFS_FILENAME_MAX_LENGTH)) {
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return NULL;
	}	
	//MMIAPICOM_StrToWstr(szFilename, szUCS2Filename);
	memset(szUCS2Filename, 0 ,sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)szFilename, strlen(szFilename), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Open szFilename:%s, nMode:0x%x\r\n", szFilename, nMode);
#endif

#ifdef OEMFS_Aplfs
	    pstAplfs=(STAplfs *)OEM_Heap_Malloc_DBG(sizeof(STAplfs));
		if(NULL==pstAplfs)
			return NULL;
		memset(pstAplfs, 0, sizeof(STAplfs));
		
//	if(1)//
	if(g_oemfs_cachesize >0 )
	{
		Aplfs_Status st;
		//STAplfs *pstAplfs;
		Aplfs_FileMode eMode;
		int Seek2Begin=0;
		
		if(nMode&OEMFS_MODE_READ){
			eMode=0;/*Aplfs_READ*/
		}
		else if(nMode&OEMFS_MODE_CREATE){
			SFS_DeleteFile(szUCS2Filename, NULL);
			eMode=6;/*Aplfs_WRITE*/   //IMPORTANT
		}
		else if(nMode&OEMFS_MODE_APPEND){
			eMode=2;/*Aplfs_APPEND*/
		}
		else if(nMode&OEMFS_MODE_READWRITE){
			eMode=2;/*Aplfs_READWRITE*//*Aplfs_APPEND*//*Aplfs_WRITE*/
			Seek2Begin=1;
		}

#if 0
		pstAplfs=(STAplfs *)OEM_Heap_Malloc_DBG(sizeof(STAplfs));
		if(NULL==pstAplfs)
			return NULL;
		memset(pstAplfs, 0, sizeof(STAplfs));
#endif
		st=Aplfs_Open(pstAplfs, (void *)szUCS2Filename, eMode);
		if(Aplfs_OK!=st)
		{
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Open  failed\r\n");
#endif
			OEMHeap_Free(pstAplfs);
			pstAplfs=NULL;
		}
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Open Success\r\n");
#endif

		
		if(Seek2Begin)
		{
			Aplfs_Seek(pstAplfs, 0);
		}


		handle=(int)pstAplfs;
	}
else
#else
	
	{
		flags=SFS_MODE_READ;
		if(nMode&OEMFS_MODE_READ)
		{
			flags=SFS_MODE_READ|SFS_MODE_OPEN_EXISTING;
		}
		else if(nMode&OEMFS_MODE_CREATE)
		{
			flags=SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE|SFS_MODE_READ;//IMPORTANT
		}
		else if((nMode&OEMFS_MODE_READWRITE)||(nMode&OEMFS_MODE_APPEND))
		{
			if ( 0 != OEMFS_Test(szFilename))
				return NULL; //flags=FFS_MODE_CREATE_ALWAYS|FFS_MODE_READ|FFS_MODE_WRITE;
			else
				flags=SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING;//FFS_MODE_APPEND
		}	

		handle=SFS_CreateFile((unsigned short*)szUCS2Filename,flags, 0, 0);
#ifdef APPLE_FILE_LOG
		OEMOS_dbgprintf("OEMFS_Open,file handle:%d \r\n", handle);
#endif
		if(handle == 0){
#ifdef APPLE_FILE_LOG
			OEMOS_dbgprintf("OEMFS_Open failed return :%d szFilename:%s \r\n", handle);
#endif
			xlate_err(handle);
			return NULL;
			}

		if(nMode&OEMFS_MODE_APPEND)
		{
			returnCode=SFS_SetFilePointer( handle, 0, SFS_SEEK_END);	
			if(returnCode != SFS_NO_ERROR)
			{
#ifdef APPLE_FILE_LOG
				OEMOS_dbgprintf("FS_Seek end err");
#endif
				xlate_err(returnCode);
				SFS_CloseFile(handle);
				return NULL;
			}
		}
#ifdef APPLE_FILE_LOG
			OEMOS_dbgprintf("OEMFS_Open szFilename:%s, handle:%d\r\n", szFilename, handle);
#endif

//	pstAplfs->hFile = handle;
//	handle=(int)pstAplfs;
	}
#endif	//OEMFS_Aplfs	
	return (void*)handle;
}

int OEMFS_Close(void *pFileHandle)
{
	int returnCode;
	//SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#ifdef OEMFS_Aplfs
	STAplfs *pstAplfs=(STAplfs *)pFileHandle;
#else
    SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#endif

	if((uint32)pFileHandle == 0 )//sxh az 20120509
		return EFAILED;

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Close handle:%d \r\n", pFileHandle);
#endif

#ifdef OEMFS_Aplfs

	if(pstAplfs->uBufSize >0)
	{
		Aplfs_Status st;
		st=Aplfs_Close(pstAplfs);
		if(pstAplfs->bBuffering)
		{
			if(pstAplfs->pbBuf)
			{
				OEMHeap_Free(pstAplfs->pbBuf);
			}
		}
		OEMHeap_Free(pstAplfs);
		return SUCCESS;
	}
   	return -1;
#else
		
	{
		returnCode = (int)SFS_CloseFile(handle);
		if (returnCode != SFS_NO_ERROR) 
		{
#ifdef APPLE_FILE_LOG
			OEMOS_dbgprintf("FS_Close Error:%d", returnCode);
#endif
			return xlate_err(returnCode);
		}
	
		return SUCCESS;
	}
#endif	//OEMFS_Aplfs

}

void OEMFS_SetBuffer(void *pFileHandle, int aSize, uint8 *p/*do not use*/)
{
#ifdef OEMFS_Aplfs
	STAplfs *pstAplfs=(STAplfs *)pFileHandle;
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMFS_SetBuffer  handle:%d,aSize:%d ",pFileHandle,aSize);
	#endif
	
	if(aSize <= 0)
		return ;


	if(pstAplfs)
	{
		uint32 uBufferSize = (uint32)aSize;
		uint8 *pbBuf;
		
		if(pstAplfs->bBuffering)
		{
			if(pstAplfs->pbBuf)
			{
				if(pstAplfs->uBufSize == uBufferSize)
				{
					return;
				}
				else
				{
					OEMHeap_Free(pstAplfs->pbBuf);
				}
			}
		}
		
		pbBuf = (uint8 *)OEM_Heap_Malloc_DBG(aSize);
		if(!pbBuf)
		{
			return;
		}
		Aplfs_SetBuffer(pstAplfs, uBufferSize, pbBuf);
		Aplfs_Seek(pstAplfs, 0);
	}
#endif
}

/*==================================================================
Function: OEMFS_Read

Description: Reads bytes from a file in the EFS.
==================================================================*/
uint32 OEMFS_Read(void *pFileHandle,void *buffer,uint32 nBytes)
{
	int returnCode = 0;
	//SFS_HANDLE handle = (SFS_HANDLE)pFileHandle;
#ifdef OEMFS_Aplfs
		STAplfs *pstAplfs=(STAplfs *)pFileHandle;
#else
		SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#endif
	uint32 real_count = 0;	
	returnCode=0;
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Read handle:%d, pbuf:%x, size:%d \r\n", pFileHandle, buffer, nBytes);
#endif	
	if(NULL==buffer || NULL == pFileHandle){
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return 0;
		}

#ifdef OEMFS_Aplfs
	//if(NULL!=pFileHandle)
	if(pstAplfs->uBufSize >0)
	{
		
		Aplfs_Status st=Aplfs_Read(pstAplfs, (uint8*)buffer, (uint32)nBytes);
		int nRead=Aplfs_GetLastReadCount(pstAplfs);
		if(nRead < 0)
			nRead = 0;
		real_count = (uint32)nRead;
		pstAplfs->uLastReadCount = 0;
		return real_count;
	}
	else
#else
		
	{
		returnCode = (int)SFS_ReadFile(handle,(void*)buffer, nBytes, &real_count, NULL);
		xlate_err(returnCode);
		if(returnCode == SFS_NO_ERROR)
		{
			return real_count;
		}
	}
#endif	//OEMFS_Aplfs
	
	return 0;
}

/*==================================================================
Function: OEMFS_Write

Description: Writes bytes to a file in the EFS.
==================================================================*/
uint32 OEMFS_Write(void *pFileHandle,const void *buffer,uint32 nBytes)
{
	int returnCode = 0;
//	SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
	uint32 real_count = 0;
#ifdef OEMFS_Aplfs
		STAplfs *pstAplfs=(STAplfs *)pFileHandle;
#else
		SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#endif

	returnCode=0;
	
	if(NULL==buffer || NULL == pFileHandle){
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return 0;
		}
#ifdef OEMFS_Aplfs

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Write handle:%d, pbuf:%x, size:%d  pstAplfs->uBufSize %d \r\n", pFileHandle, buffer, nBytes, pstAplfs->uBufSize);
#endif	

	//if(NULL!=pFileHandle)
	if(pstAplfs->uBufSize >0)
	{
		
		Aplfs_Status st=Aplfs_Write(pstAplfs, (uint8*)buffer, (uint32)nBytes);

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Write st:%d,  nBytes:%d \r\n", st,  nBytes);
#endif		
		if(Aplfs_OK==st)
		{
			return nBytes;
		}
	}
	else
#else
		
	{
		returnCode=SFS_WriteFile(handle,(void*)buffer, nBytes, &real_count, NULL);

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Write returnCode:%d,  real_count:%d \r\n", returnCode,  real_count);
#endif		
		if(returnCode == SFS_NO_ERROR)
		{
			return real_count;
		}
		xlate_err(returnCode);
	}
#endif	//OEMFS_Aplfs
	
	return 0;
	
}

/*==================================================================
Function: OEMFS_GetFileAttributes

Description: Gets attributes of a file in the EFS.
==================================================================*/
int OEMFS_GetFileAttributes(const char *szFilename,void *pFileInfo)
{
	SFS_HANDLE handle = 0;
	int returnCode;
	
	SFS_FIND_DATA_T fd = {0};
	OEMFSInfo * pInfo = (OEMFSInfo*)pFileInfo;

	if(NULL==szFilename || NULL==pInfo){
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return EFAILED;
		}

	memset(szUCS2Filename, 0 ,sizeof(szUCS2Filename));//#ifdef SXH_APPLE_YOUXIN	
	OEM_UTF8ToWStr((const byte *)szFilename, strlen(szFilename), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);

// #ifdef OEMFS_Aplfs
// 	{
// 		STAplfs *pstAplfs=(STAplfs *)handle;//error
// 		returnCode=OEMFS_GetFileInfo(pstAplfs->hFile,  pInfo);
// 	}
// #else

	handle = SFS_FindFirstFile(szUCS2Filename, &fd);
	if(0 == handle)
	{
		SFS_FindClose(handle);
		return EFAILED;
	}

	OEMFS_TranslateFileInfo(pInfo, &fd);

	SFS_FindClose(handle);
// #endif	//OEMFS_Aplfs
	return SUCCESS;
}

int OEMFS_Seek(void * pFileHandle,int8 sType,int32 nOffset)
{
//	SFS_HANDLE hf=0;	
	int Whence=0;
   	int32 nFileSize = 0;
	int returnCode = 0;
	int32 Position = 0;
#ifdef OEMFS_Aplfs
	STAplfs *pstAplfs=(STAplfs *)pFileHandle;
#else
    SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#endif	
	
	
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Seek handle:%d, type:%d, offset:%d \r\n", pFileHandle, sType, nOffset);
#endif

	if(NULL == pFileHandle)
	{
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return EFAILED;
	}
//	hf = pstAplfs->hFile;
	
#ifdef OEMFS_Aplfs
	if(pstAplfs->uBufSize >0)
	{
		Aplfs_Status st;
		
		uint32 uOffset=0;	


		if(sType == 0 /*_SEEK_START*/)
		{
			uOffset=nOffset;
		}
		else if(sType == 1 /*_SEEK_END*/)
		{
			st = Aplfs_GetFileSize(pstAplfs, (uint32 *)&Position);
			if(st!=Aplfs_OK)
				return -1;
			uOffset = Position-nOffset;
		}
		else if(sType== 2 /*_SEEK_CURRENT*/)
		{
			st = Aplfs_GetCurPos(pstAplfs, (uint32 *)&Position);
			if(st!=Aplfs_OK)
				return -1;

			uOffset=Position + nOffset;
			if(!nOffset)
			{
				return(Position);
			}
		}
		else{
			return xlate_err(SFS_ERROR_INVALID_PARAM);
		}
		

		st = Aplfs_Seek(pstAplfs, uOffset);
		if(Aplfs_OK == st)
			return SUCCESS;
	}
	else
#else
		
	{

		if(sType == 0 /*_SEEK_START*/){
			Whence = SFS_SEEK_BEGIN;
		}
		else if(sType == 1 /*_SEEK_END*/){
			Whence=SFS_SEEK_END;
		}
		else if(sType== 2 /*_SEEK_CURRENT*/)
		{
			Whence=SFS_SEEK_CUR;
			if(!nOffset)
			{

				returnCode = SFS_GetFilePointer(handle, SFS_SEEK_BEGIN, &Position);
				if(returnCode != SFS_NO_ERROR)
					return(xlate_err(returnCode));
				return(Position);
			}
		}
		else{
			return xlate_err(SFS_ERROR_INVALID_PARAM);
		}
		returnCode=SFS_SetFilePointer(handle, nOffset, Whence);
		if(returnCode != SFS_NO_ERROR)
			return(xlate_err(returnCode));
		else
		{
			int returnCode2 = 0 ;
			int Position2 = 0;
			returnCode2 = SFS_GetFilePointer(handle, SFS_SEEK_BEGIN, (int *)&Position2);
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("OEMFS_Seek   returnCode2 :%d,Position2:%d",returnCode2,Position2);
			#endif
		
			return SUCCESS;
	
		}
	}
#endif	//OEMFS_Aplfs


//	return -1;
}


/*==================================================================
Function: OEMFS_Tell

Description: Returns the current position of the seek pointer in
             a file in the EFS.
==================================================================*/
int32 OEMFS_Tell(void * pFileHandle)
{
	int32 Position  = 0;
	int returnCode = 0;
#ifdef OEMFS_Aplfs
		STAplfs *pstAplfs=(STAplfs *)pFileHandle;
#else
		SFS_HANDLE handle=(SFS_HANDLE)pFileHandle;
#endif	

	
#ifdef APPLE_FILE_LOG
		OEMOS_dbgprintf("OEMFS_Tell handle:%d\r\n", pFileHandle);
#endif		
	if(NULL == pFileHandle)
	{
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return EFAILED;
	}
	
#ifdef OEMFS_Aplfs
	if(pstAplfs->uBufSize >0)
	{
		
		if(pstAplfs)
		{
			Aplfs_Status st;
			st = Aplfs_GetCurPos(pstAplfs, (uint32 *)&Position);
			if(st!=Aplfs_OK)
				return -1;
			return Position;
		}
		else 
		{
			return -1;
		}
	}
	else
#else
		
	{
		returnCode = (int)SFS_GetFilePointer(handle, SFS_SEEK_BEGIN, &Position);
		if(returnCode != SFS_NO_ERROR)
		{
			xlate_err(returnCode);
			return -1;
		}
	}
#endif	//OEMFS_Aplfs
	return Position;
}


/*==================================================================
Function: OEMFS_Truncate

Description: Truncates a file in the EFS to the specified size.
==================================================================*/
int OEMFS_Truncate(void *pFileHandle,uint32 nPos)
{
	int returnCode;
	int handle=(int)pFileHandle;
	uint32 wSize = 0;

	if(NULL == handle)
	{
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return EFAILED;
	}
	
	SFS_GetFileSize(handle, &wSize);
	if(nPos > (uint32)wSize)
      	return -1;
	returnCode = OEMFS_Seek(pFileHandle, 0, nPos);

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Truncate handle:%d, nPos:%d, size:%d \r\n", pFileHandle, nPos,wSize);
#endif		
	returnCode = SFS_SetFilePointer(handle, 0, SFS_SEEK_BEGIN);
	if(SFS_NO_ERROR == returnCode)
	{
		returnCode = SFS_SetFileSize(handle, nPos);
	}
	SFS_SetFilePointer(handle, 0, SFS_SEEK_END);
	return xlate_err(returnCode);	
}

/*==================================================================
Function: OEMFS_Remove

Description: Removes a file from the EFS.
==================================================================*/
int OEMFS_Remove(const char *filename)
{
	SFS_ERROR_E err=0;
	int16 attr = 0;
	int rv;
	// uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH];
	
   	if((!filename)||strlen(filename) >= OEMFS_FILENAME_MAX_LENGTH)
      	return(xlate_err(SFS_ERROR_INVALID_PARAM));
	
	//MMIAPICOM_StrToWstr(filename, szUCS2Filename);
	memset(szUCS2Filename, 0, sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)filename, strlen(filename), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);
//     rv = (int)SFS_GetAttr(szUCS2Filename, &attr);
//     if ( rv!=SFS_NO_ERROR && (attr&SFS_ATTR_RO) )
//         SFS_SetAttr(szUCS2Filename, (attr & ~(SFS_ATTR_RO)) );

	err = SFS_DeleteFile((unsigned short*)szUCS2Filename, NULL);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Remove, err:%d, %s %d",err, filename);
#endif

	return xlate_err( err );
}

/*==================================================================
Function: OEMFS_Rename

Description: Renames a file in the EFS to the specified new name.
==================================================================*/
int OEMFS_Rename(const char *old_filename,const char *new_filename)
{
	SFS_ERROR_E err=0;
	uint16 ucs2_old_filename[OEMFS_FILENAME_MAX_LENGTH];
	uint16 ucs2_new_filename[OEMFS_FILENAME_MAX_LENGTH];

   	if((!old_filename) || strlen(old_filename) >= OEMFS_FILENAME_MAX_LENGTH)
      		return(xlate_err(SFS_ERROR_INVALID_PARAM));

   	if((!new_filename) || strlen(new_filename) >= OEMFS_FILENAME_MAX_LENGTH)
      		return(xlate_err(SFS_ERROR_INVALID_PARAM));	

	memset(ucs2_old_filename, 0, sizeof(ucs2_old_filename));
	OEM_UTF8ToWStr((const byte *)old_filename, strlen(old_filename), ucs2_old_filename, OEMFS_FILENAME_MAX_LENGTH);

	memset(ucs2_new_filename, 0, sizeof(ucs2_new_filename));
	OEM_UTF8ToWStr((const byte *)new_filename, strlen(new_filename), ucs2_new_filename, OEMFS_FILENAME_MAX_LENGTH);

	err = SFS_RenameFile(ucs2_old_filename, ucs2_new_filename, NULL);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Rename, err:%d, %s to %s",err, old_filename,new_filename);
// 	OEMOS_HexLog((uint8*)ucs2_old_filename, 50);
// 	OEMOS_HexLog((uint8*)ucs2_new_filename, 50);
#endif
	return xlate_err( err );
}

/*==================================================================
Function: OEMFS_Mkdir

Description: Creates a direcory at the specified location in the EFS.
==================================================================*/
int OEMFS_Mkdir(const char *pszDir)
{
	SFS_ERROR_E err=0;
	// uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH] = {0};
	int len ;
   	if((!pszDir) || strlen(pszDir)>=OEMFS_FILENAME_MAX_LENGTH)
      	return( xlate_err(SFS_ERROR_INVALID_PARAM) );

	//MMIAPICOM_StrToWstr( pszDir, szUCS2Filename );
	memset(szUCS2Filename, 0, sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)pszDir, strlen(pszDir), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);
	len = MMIAPICOM_Wstrlen(szUCS2Filename);
	if(szUCS2Filename[len-1] == L'\\')
		szUCS2Filename[len-1] = 0;	
	err = SFS_CreateDirectory(szUCS2Filename);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Mkdir, err:%d, %s",err,pszDir);
#endif
	if(SFS_ERROR_HAS_EXIST==err)
		return 0;
	return xlate_err( err );
}


/*==================================================================
Function: OEMFS_Rmdir

Description: Removes the specified directory from the EFS.
==================================================================*/
int OEMFS_Rmdir(const char *pszDir)
{
	SFS_ERROR_E err=0;
	// uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH];
   	int nErr = 0, len;

   	if((!pszDir)||( strlen(pszDir) >= OEMFS_FILENAME_MAX_LENGTH))
      	return(xlate_err(SFS_ERROR_INVALID_PARAM));
	
	memset(szUCS2Filename, 0, sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)pszDir, strlen(pszDir), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);
	len = MMIAPICOM_Wstrlen(szUCS2Filename);
	if(szUCS2Filename[len-1] == L'\\')
		szUCS2Filename[len-1] = 0;

	err = SFS_DeleteDirectory(szUCS2Filename);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Rmdir, err:%d, %s",err,pszDir);
#endif

	return xlate_err( err );
}

/*==================================================================
Function: OEMFS_Test

Description: Tests if the specified name is a file or a directory.
==================================================================*/
int OEMFS_Test(const char * filename)
{	
	SFS_ERROR_E err=0;
	uint16 attr = 0, len;
	// uint16 szUCS2Filename[OEMFS_FILENAME_MAX_LENGTH];
	SFS_FIND_DATA_T fd;
	SFS_HANDLE handle;

   	if((!filename)|| strlen(filename) >= OEMFS_FILENAME_MAX_LENGTH)
      	return(xlate_err(SFS_ERROR_INVALID_PARAM));
	
	//MMIAPICOM_StrToWstr( filename, szUCS2Filename );
	memset(szUCS2Filename, 0, sizeof(szUCS2Filename));
	OEM_UTF8ToWStr((const byte *)filename, strlen(filename), szUCS2Filename, OEMFS_FILENAME_MAX_LENGTH);
	len = MMIAPICOM_Wstrlen(szUCS2Filename);
	if(szUCS2Filename[len-1] == L'\\')
		szUCS2Filename[len-1] = 0;
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Test, %s,len:%d",filename,len);
#endif
	handle = SFS_FindFirstFile(szUCS2Filename, &fd);
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_Test, handle: %d",handle);
#endif
	SFS_FindClose(handle);
	if(0 == handle)
		return 0x100;	//failed
//	SFS_FindClose(handle);
	return 0;
// 	err = SFS_GetAttr(szUCS2Filename, &attr) ;
// 	
// 	OEMOS_dbgprintf("OEMFS_Test, err:%d, %s, %d",err,filename, attr);
// 	if(SFS_ERROR_ACCESS==err || SFS_NO_ERROR==err || SFS_ERROR_HAS_EXIST==err)
// 		return 0;
// 	else
// 		return 0x100;
}
extern uint16 *OEM_wstrcat(uint16 *pDest, const uint16 *pSrc);
OEMFSInfo * OEMFS_EnumStart(const char *szDir,char isDir)
{
   	EnumContext *     pe;
	uint16 szUCS2Dir[OEMFS_FILENAME_MAX_LENGTH] = {0};
	uint16 * szDirPtr = NULL;

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_EnumStart szDir:%s ,isDir:%d\r\n", szDir, isDir);
#endif	
   	if((!szDir)||strlen(szDir) >= OEMFS_FILENAME_MAX_LENGTH)
	{
    	xlate_err(SFS_ERROR_INVALID_PARAM);
		return NULL;
   	}
	
	//OEM_strtowstr(szDir, szUCS2Dir, sizeof(szUCS2Dir));
	memset(szUCS2Dir, 0, sizeof(szUCS2Dir));
	OEM_UTF8ToWStr((const byte*)szDir,  strlen(szDir), szUCS2Dir, sizeof(szUCS2Dir));

	/*	if(!isDir)*/
	{
		szDirPtr=(uint16* )szUCS2Dir;
		szDirPtr=szUCS2Dir+OEM_wstrlen(szUCS2Dir)-1;
		if(*szDirPtr!=L'\\')
		{
			*(++szDirPtr)=L'\\';
			*(++szDirPtr)=L'\0';//2008.2.13 忘记在wchar后面补零
		}
		//OEM_wstrcat(szUCS2Dir, L"*.*");
		OEM_wstrcat(szUCS2Dir, L"*");
	}

// 	OEMOS_HexLog((unsigned char * )szUCS2Dir, OEM_wstrlen(szUCS2Dir) * sizeof(uint16));

   	pe = (EnumContext * )OEM_Heap_Malloc_DBG(sizeof(EnumContext));
   	if(!pe) 
	{
      	gLastErr = 2;/*ENOMEMORY*/
      	return(NULL);
   	}	
	memset(pe, 0, sizeof(EnumContext));
	
    pe->handle = SFS_FindFirstFile( szUCS2Dir, &(pe->fd) );
	if( pe->handle == 0 )
	{
		gLastErr = 1;/*EFAILED*/
		SFS_FindClose(pe->handle);
		OEMHeap_Free((void*)pe);
		return NULL;
	}
#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_EnumStart OK\r\n");
#endif
	return((OEMFSInfo *)pe);
}

/*==================================================================
Function: OEMFS_EnumNext

Description: Enumerates through the files in the location specified
             by OEMFS_EnumInit.
==================================================================*/
int OEMFS_EnumNext(OEMFSInfo * pcxt)
{
	SFS_ERROR_E rv;
	EnumContext *     pe = (EnumContext *)pcxt;

	if(NULL == pe)
		return xlate_err(SFS_ERROR_INVALID_PARAM);

	if(pe->nErr){
		memset(&pe->pub,0,sizeof(pe->pub));
		return pe->nErr;
	}

	OEMFS_TranslateFileInfo(&(pe->pub), &(pe->fd));
#ifdef APPLE_FILE_LOG	
	OEMOS_dbgprintf("OEMFS_EnumNext %s ,attr:%d\r\n", pe->pub.szFile, pe->pub.attrib);
#endif
	SCI_MEMSET(&pe->fd, 0, sizeof(pe->fd));
	rv = SFS_FindNextFile(pe->handle , &(pe->fd));
	if(rv != SFS_NO_ERROR)
		pe->nErr = 0x101;


	return(0);
}

/*==================================================================
Function: OEMFS_EnumStop

Description: Indicates that we can free resources related to enumeration
==================================================================*/
int OEMFS_EnumStop( OEMFSInfo * pcxt)
{
   EnumContext * pe = (EnumContext *)pcxt;

   if (!pe) 
      return(xlate_err(SFS_ERROR_INVALID_PARAM));

   SFS_FindClose(pe->handle);
   
   OEMHeap_Free((void*)pe);

   return(xlate_err(SFS_NO_ERROR));
}

uint32 OEMFS_SpaceAvail (void)
{
	return 0;
}

/*==================================================================
Function: OEMFS_SpaceUsed

Description: Returns the space already used on the EFS.
==================================================================*/
uint32 OEMFS_SpaceUsed (void)
{
   return 0;
}

//int OEMFS_GetFreeSpaceEx(const char *szPath, uint32 *pdwTotal, uint32 *pdwFree)
//{
//	return 20; /*EUNSUPPORTED*/;
//}


/*==================================================================
Function: xlate_err

Description: Translates DMSS errors into BREW errors
==================================================================*/
static int xlate_err( int nDMSS )
{
   ErrorMap * pe;

   for(pe = (ErrorMap *)gsErrorMap; /* exit below */ ; pe++){

      if ( (char*)pe > (char*)gsErrorMap + sizeof(gsErrorMap) ){
         gLastErr = 0x100; /*FILE_ERROR*/;
         break;
      }

      if(pe->ferr == nDMSS){
         gLastErr = pe->err;
         break;
      }
   }

   return(gLastErr);
}


int OEMFS_TranslateFileInfo(void * pFileInfo, const SFS_FIND_DATA_T* fd)
{
   	int attr;
	int returnValue = 0;
	OEMFSInfo * pInfo = (OEMFSInfo*)pFileInfo;

	if(NULL == pInfo)
	{
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return EFAILED;
	}
	
	attr = fd->attr;
	
    if(attr & SFS_ATTR_HIDDEN)
        returnValue |= 0x1; /*_FA_HIDDEN*/ 
    if(attr & SFS_ATTR_DIR)
        returnValue |= 0x2; /*_FA_DIR*/
    if(attr & SFS_ATTR_RO)
        returnValue |= 0x4; /*_FA_READONLY*/
    if(attr & SFS_ATTR_SYS)
        returnValue |= 0x8; /*_FA_SYSTEM*/	
    if(attr & SFS_ATTR_ARCH)
		returnValue |= 0x0; /*_FA_NORMAL*/ 

	pInfo->attrib = returnValue;
#ifdef SXH_SPT_H7810
	pInfo->dwCreationDate = TansferSFSTime(fd->create_Date, fd->create_time);
#else
	pInfo->dwCreationDate = MMIAPICOM_TansferSFSTime(fd->create_Date, fd->create_time);
#endif
	pInfo->dwSize = fd->length;
	//MMIAPICOM_WstrToStr(fd->name, pInfo->szFile );


#ifdef APPLE_FILE_LOG		
	OEMOS_dbgprintf("  OEM_wstrlen(fd->name) :%d   sizeof(pInfo->szFile):%d,APPLE_GetCurrAppID:0x%x",
		OEM_wstrlen(fd->name),sizeof(pInfo->szFile),APPLE_GetCurrAppID());	


//	OEMOS_dbgprintf("  OEMFS_TranslateFileInfo   short_name:%s ",fd->short_name);
#endif
	memset(pInfo->szFile, 0, 64);

	OEM_WStrToUTF8((const uint16*) fd->name, OEM_wstrlen(fd->name), (byte*) pInfo->szFile,  sizeof(pInfo->szFile));
#ifdef APPLE_FILE_LOG	
	OEMOS_dbgprintf("  OEMFS_TranslateFileInfo  end" );
#endif
	return 0;
}

uint32 OEMFS_GetFreeSpace(uint32 *pdwTotal)
{

	uint32 high_word = 0;
	uint32 low_word = 0;
	int rv = 0;
	
	if(NULL == pdwTotal)
	  return NULL;
	  	
	rv = SFS_GetDeviceFreeSpace((uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), &high_word, &low_word);
	if (SFS_NO_ERROR != rv)
		return 0;
	*pdwTotal = low_word;

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEMFS_GetFreeSpace free:%d total:%d \r\n", rv, *pdwTotal);
#endif
	return *pdwTotal;

//	return xlate_err(rv);

}


extern size_t OEM_strlen(const char *p);
unsigned char* OEM_ReadFile(unsigned char* filename, unsigned int *pLen,unsigned int iStart, unsigned int iEnd)
{
	unsigned char* data = NULL;
	SFS_HANDLE pf;
	SFS_ERROR_E err = 0;
	uint32 readsize =0;
	uint16 filepath[256] = {0};

	if(NULL == filename || NULL == pLen)
	{
		xlate_err(SFS_ERROR_INVALID_PARAM);
		return NULL;
	}
	
	if(!OEM_UTF8ToWStr((byte*)filename,OEM_strlen((char*)filename),filepath,256))
		return NULL;
	pf = SFS_CreateFile((const uint16 *)filepath, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ/*FFS_MODE_OPEN_EXISTING|FFS_MODE_READ*/,0,0);//sxh az
	if(0 != pf)
	{	
		SFS_GetFileSize(pf, (uint32*)pLen);
		if(*pLen > 0)
		{
			data = (unsigned char *) OEM_Heap_Malloc_DBG(*pLen + iStart + iEnd);

			if(data != 0)
			{
				uint32 pos;
				memset(data, 0, *pLen+iStart+iEnd);
				pos = 0;
				while(pos< *pLen)
				{
					err = SFS_ReadFile(pf, data+iStart+pos, *pLen - pos ,&readsize, NULL);
					if(SFS_ERROR_NONE != err)
						break;
					//					apple_os_log("apple_readfile FS_Read:%d \r\n", readsize);
					pos += readsize;
				}

			        readsize = pos;
			}
		}
		SFS_CloseFile(pf);	
	}

#ifdef APPLE_FILE_LOG
	OEMOS_dbgprintf("OEM_ReadFile *pLen:%d readsize:%d \r\n", *pLen, readsize);
#endif	
	if(*pLen != readsize)
		return NULL;
	
	return data;
}

uint32 apple_createfile(uint16 *szUCS2Filename2)
{
	SFS_HANDLE handle = 0;
	int32 len=0 ,cnt=0;	
	SFS_ERROR_E err = SFS_ERROR_NONE; 

	if(NULL == szUCS2Filename2)
	{
		return handle;
	}
	
	len = MMIAPICOM_Wstrlen(szUCS2Filename2);
	
	if(szUCS2Filename2[len-1] != L'\\')
	{
		SFS_DeleteFile((unsigned short*)szUCS2Filename2, NULL);
	}
	else
	{
		SFS_DeleteDirectory((unsigned short*)szUCS2Filename2);
	}
	//	if(handle < 0)
	{
		//Create dir
		for(cnt=0;  cnt< len; cnt++)
		{
			if(szUCS2Filename2[cnt] != L'\\')
				continue;
			if(szUCS2Filename2[cnt-1] == L':')		//	跳过盘符
				continue;
			
			szUCS2Filename2[cnt] = 0;
			//handle = SFS_CreateFile(szUCS2Filename,FFS_MODE_OPEN_EXISTING| FFS_MODE_READ, 0, 0);
			err = SFS_CreateDirectory(szUCS2Filename2);
// 			if (handle == 0)
// 			{
// 				handle = SFS_CreateDirectory(szUCS2Filename);
// 				if(SFS_NO_ERROR == handle)
// 					SFS_SetAttr(szUCS2Filename, SFS_ATTR_DIR);
// 			}
// 			else
// 			{
// 				SFS_CloseFile(handle);
// 			}
			szUCS2Filename2[cnt] = L'\\';
		}
		if(szUCS2Filename2[len-1] != L'\\')
			handle = SFS_CreateFile((unsigned short*)szUCS2Filename2, 
			SFS_MODE_CREATE_ALWAYS|SFS_MODE_READ|SFS_MODE_WRITE, 0, 0);
	}
	
	return handle;
}

#ifdef OEMFS_Aplfs 
uint32 OEMFS_DirectOpen(uint16* szFile, int eMode)
{
	uint32 uFlags = 0;
	switch (eMode) {
	case 0:/*Aplfs_READ*/
		uFlags = SFS_MODE_READ|SFS_MODE_OPEN_EXISTING; 
		break;
	case 1:/*Aplfs_WRITE*/
		uFlags = SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE;
		break;
	case 5:/*Aplfs_READWRITE*/
	case 2:/*Aplfs_APPEND*/
		uFlags = SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING; 
		break;
    case 6:  //IMPORTANT
        uFlags = SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE|SFS_MODE_READ;
		break;
	case 4:
		uFlags = SFS_MODE_OPEN_EXISTING|SFS_MODE_SHARE_READ;
		break;
	default:
		return 0;
	}

	return SFS_CreateFile((unsigned short*)szFile,uFlags, 0, 0);
}
uint32 OEMFS_DirectClose(uint32 hFile)
{
	if(SFS_CloseFile(hFile) != SFS_NO_ERROR)
		return 1;
	return 0;
}
uint32 OEMFS_DirectRead( uint32 handle, void *buffer, uint32 bytes_to_read, uint32 *bytes_read, void *overlapped )
{
	SFS_ERROR_E err;
	err = SFS_ReadFile(handle,buffer, bytes_to_read, (uint32*)bytes_read, overlapped);
	if(SFS_NO_ERROR != err)
		return 1;
	return 0;
}
uint32 OEMFS_DirectWrite( SFS_HANDLE handle, const void *buffer, uint32 bytes_to_write, uint32 *bytes_written, SFS_OVERLAPPED_T *overlapped )
{
	SFS_ERROR_E err;
	err = SFS_WriteFile(handle,buffer, bytes_to_write, (uint32*)bytes_written, overlapped);
	if(SFS_NO_ERROR != err)
		return 1;
	return 0;
}
uint32 OEMFS_DirectSetPos( SFS_HANDLE handle, int32 offset, uint32 origin )
{
	SFS_ERROR_E err;
	err = SFS_SetFilePointer(handle, offset, origin);
	if(SFS_NO_ERROR != err)
		return 1;
	return 0;
}
uint32 OEMFS_DirectGetPos( SFS_HANDLE handle, uint32 origin, int32* currentpos )
{
	SFS_ERROR_E err;
	err = SFS_GetFilePointer(handle, origin, currentpos);
	if(SFS_NO_ERROR != err)
		return 1;
	return 0;
}
uint32 OEMFS_GetFileSize(SFS_HANDLE sfs_handle, uint32* size)
{
	SFS_ERROR_E err;
	err = SFS_GetFileSize(sfs_handle, size);
	if(SFS_NO_ERROR != err)
		return 1;
	return 0;
}
#endif // OEMFS_Aplfs

void OEM_FillFileExtAPI(void *p)
{
// 	if(p)
// 	{
// 		APPLE_FileExt * FileExt = (APPLE_FileExt *)p;
// 		FileExt->FmgrCreate = (int (*)(void** ))OS(_COS_FmgrCreate);
// 		FileExt->FmgrRelease = (void (*)(void *))OS(_COS_FmgrRelease);
// 		FileExt->FmgrOpenFile = (void* (*)(void *,const char * ,int ))OS(_COS_FmgrOpenFile);
// 		FileExt->FmgrIsFileExist = (boolean (*)(void *,const char *))OS(_COS_FmgrIsFileExist);
// 		FileExt->FileGetFileSZ = (int (*)(void *,uint32 *))OS(_COS_FileGetFileSZ);
// 		FileExt->FileSetCache = (boolean (*)(void *,uint32 ))OS(_COS_FileSetCache);
// 		FileExt->FileRead = (uint32 (*)(void *,void *,uint32 ))OS(_COS_FileRead);
// 		FileExt->FileRelease = (void (*)(void *))OS(_COS_FileRelease);
// 		FileExt->FileSeek = (uint32 (*)(void *, int ,int ))OS(_COS_FileSeek);
// 	}
}

// OEM_meida

int OEMMedia_SetVolume(int iVol)
{
//      const uint8 vol_map[11] = {0,3,3,4,5,6,7,7,7,7,7};

	const uint8 vol_map[11] = {0,1,2,2,3,4,5,6,7,8,9};

#ifdef DBG_MEDIA
	OEMOS_dbgprintf("OEMMedia_SetVolume iVol:%d ", iVol);
#endif
	iVol = (iVol + 5) / 10;
#ifdef DBG_MEDIA
	OEMOS_dbgprintf("OEMMedia_SetVolume MMIAPISET_InitAudioDevice iVol:%d ", iVol);
#endif
	if(iVol > 10)
		iVol = 10;

	// MMIAPISET_InitAudioDevice(vol_map[iVol]);
	MMISRVAUD_SetVolume(s_SXMOBI_audio_handle,vol_map[iVol]);

	return 0;
}


unsigned int OEMOS_Get_SMSID(void)
{
	return APP_MN_SMS_IND;
}

unsigned int OEMOS_Get_SMSSendID(void)
{
	return APP_MN_SEND_SMS_CNF;
}


extern MMISMS_OPER_ERR_E APPLE_SendMsg( 
													 MN_DUAL_SYS_E            dual_sys,
													 MN_CALLED_NUMBER_T        *dest_number_ptr,    //IN:
													 APP_SMS_USER_DATA_T         *user_data_ptr,        //IN: the user data
													 MN_SMS_STORAGE_E            storage,            //IN: the storage (SIM or NV)
													 MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN: the dcs
													 BOOLEAN                    is_more_msg            //IN: the more message to send 
    );
extern PUBLIC void MMISMS_GetUserData(
                                uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                                MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                                MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data 
                                BOOLEAN                  is_need_convert,
                                APP_SMS_USER_DATA_T      *user_data_ptr,    //OUT: the user data
                                MMISMS_SEND_T *send_info_ptr
                                );
int OEMTAPI_SendSMS(void *p, const char * pszDst, const char * pszMsg)
{

	int rv = -1;
#ifdef APPLE_ON_TARGET
	MMISMS_OPER_ERR_E   send_result = MMISMS_NO_DEST_ADDRESS;
	
	MN_DUAL_SYS_E dual_sys = OEMOS_GetActiveSim();
	MN_CALLED_NUMBER_T        dest_num = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};
	APP_SMS_USER_DATA_T        user_data = {0};
	uint8                    total_num = 0;
	MN_SMS_USER_DATA_HEAD_T    user_head = {0};
	MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
//	MN_SMS_ALPHABET_TYPE_E alphabet_type = MN_SMS_UCS2_ALPHABET;
	MN_SMS_ALPHABET_TYPE_E alphabet_type = MN_SMS_DEFAULT_ALPHABET;
	
	MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
	uint8 *dest_addr_ptr = (uint8*)pszDst;
	uint8 dest_addr_len = strlen(pszDst);

	uint16 data_length = 0;
	uint16 glength=0;
	MMISMS_CONTENT_T *content_ptr = PNULL;
	uint8 *ascii_content_ptr = PNULL;	// add by vion
	uint8 *default_data_ptr = PNULL;
        ascii_content_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN);
        if (PNULL == ascii_content_ptr)
        {
            return rv;
        }
        default_data_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN);

        if (PNULL == default_data_ptr)
        {
            SCI_FREE(ascii_content_ptr);
            return rv;
        }

        content_ptr = SCI_ALLOCAZ(sizeof(MMISMS_CONTENT_T));
        if (PNULL == content_ptr)
        {
            SCI_FREE(ascii_content_ptr);

            SCI_FREE(default_data_ptr);

            return rv;
        }
#ifdef APPLE_LOG    
    OEMOS_dbgprintf("OEMTAPI_SendSMS pszDst:%s  \r\n", pszDst);
#endif		
	SCI_MEMSET(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));	
	if(g_apple_ucs2_sms)
	{
		glength=MMIAPICOM_Wstrlen((const uint16*)pszMsg)*sizeof(uint16);
		alphabet_type = MN_SMS_UCS2_ALPHABET;
		content_ptr->length     = MIN(glength, MMISMS_MAX_UCS2_MESSAGE_LEN);
		SCI_MEMCPY(
		content_ptr->data, 
		(uint8 *)pszMsg, 
		content_ptr->length
		);	
	}
	else
	{
		alphabet_type =  MN_SMS_DEFAULT_ALPHABET;
		glength=MMIAPICOM_Wstrlen((const uint16*)pszMsg);
		MMI_WSTRNTOSTR(ascii_content_ptr,
                                        MMISMS_MAX_MESSAGE_LEN,
                                        ((const uint16*)pszMsg),
                                        MMIAPICOM_Wstrlen((const uint16*)pszMsg),
                                        MIN(glength, MMISMS_MAX_MESSAGE_LEN));		
		// translate the ascii to default according GSM03.38
		glength=MMIAPICOM_Wstrlen((const uint16*)pszMsg);
            data_length = MMIAPICOM_Ascii2default(
                ascii_content_ptr,
                default_data_ptr,
                MIN(glength, MMISMS_MAX_MESSAGE_LEN)
                );

		content_ptr->length     = MIN(data_length,  MMISMS_MAX_MESSAGE_LEN);

		 SCI_MEMCPY(
                content_ptr->data,
                default_data_ptr,
                content_ptr->length 
                );
	}
	
	g_apple_ucs2_sms = 0;
	g_apple_send_sms++;
//	

/*
	
*/

/*
	if( MMISMS_SplitLongMsg(&content)<2)
		{
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("MMISMS_SplitLongMsg fail  \r\n");
#endif	
			return rv;
		}
*/
	MMISMS_SplitLongMsg(content_ptr,FALSE);
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("OEMTAPI_SendSMS send_length:%d   content.data %s  content 0x%x \r\n", content_ptr->length, (char*)content_ptr->data,content_ptr);
#endif	

	SCI_MEMSET(&user_head, 0,sizeof (MN_SMS_USER_DATA_HEAD_T));
	
//	MMISMS_GetUserData(1, &content, &user_head, &user_data);
	MMISMS_GetUserData(1, content_ptr, &user_head,TRUE,&user_data,PNULL);

#ifdef APPLE_LOG    
	OEMOS_dbgprintf("OEMTAPI_SendSMS user_length:%d  user_data %s \r\n", user_data.user_data_head_t.length, (char*)user_data.user_data_head_t.user_header_arr);
#endif		

        SCI_FREE(ascii_content_ptr);
        SCI_FREE(default_data_ptr);
        SCI_FREE(content_ptr);

	SCI_MEMSET(&party_number, 0, sizeof (MMI_PARTY_NUMBER_T));        

	MMIAPICOM_GenPartyNumber(dest_addr_ptr, dest_addr_len, &party_number);

	SCI_MEMSET(&dest_num, 0, sizeof (MN_CALLED_NUMBER_T));
	
	dest_num.number_type = party_number.number_type;
	dest_num.num_len     = MIN( party_number.num_len, MN_MAX_ADDR_BCD_LEN);
	SCI_MEMCPY(
		(void *)dest_num.party_num,
		(void *)party_number.bcd_num ,
		dest_num.num_len 
		);        

	MMISMS_SetOperStatus(MMISMS_SEND_SMS);
	 send_result =  APPLE_SendMsg(dual_sys, &dest_num, &user_data, storage, alphabet_type, FALSE);
	
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("OEMTAPI_SendSMS send_result:%d  \r\n", send_result);
#endif		
	
	if (MMISMS_NO_ERR == send_result)
	{
		rv = 0;
	}
	
	return rv;
	
#else
	return rv;	
#endif

}
// 新增的回调函数：
void OEMOS_MediaEnd_Callback()
{
      #ifdef APPLE_LOG	
		OEMOS_dbgprintf("OEMOS_MediaEnd_Callback");
	 #endif
#ifdef LED_DATA_DRV
      MMIAPISET_LedStopManage(MMISET_SCENE_MAX);
#endif
}
unsigned short apple_chset_ucs2_to_text_str(unsigned char *dest, int dest_size, unsigned char *src, int encoding)
{

 	unsigned short  len = strlen((char *)src);
	if(1 == encoding)
	{
		return ucs2str_to_gb2312str(src,len,dest);
	}
	
//eturn (kal_uint16)mmi_chset_ucs2_to_text_str((kal_uint8 *)dest, (kal_int16)dest_size, (kal_uint8 *)src, enc);
     return 0;
}

unsigned short apple_chset_text_to_ucs2_str(unsigned char*dest, int dest_size, unsigned char *src, int encoding)
{

	 int len = 0;

	 len = strlen((char*)src);
	 
	if(1 == encoding)
	{
		return (unsigned short)   gb2312_to_ucs2(              // return the UCS2 count
	   src,        // the pointer that contain the gb2312 string
	   len,    // the bytes count that the gb2312 string length
	    dest,           // the buffer that store the converted ucs2
	    dest_size) ;      // max ucs2 buffer size
	}
	
	
//eturn (kal_uint16)mmi_chset_text_to_ucs2_str((kal_uint8 *)dest, (kal_int16)dest_size, (kal_uint8 *)src, enc);
	return 0;
}
void apple_os_release_filter(char * param)
{
	// "10_1_2010_21_53_4"
	int i,n,j,k;
	char temp[100];
	
	n = strlen(param);
	for(i=0;i<n;i++)
	{
			if((param[i]>='0' && param[i]<='9')
				||(param[i]>='a' && param[i]<='z')
				||(param[i]>='A' && param[i]<='Z')
			)
				continue;

			param[i]='_';
	}

	for(i=0,j=0; i<n; i++)
	{
		if(param[i] == '_')
		{
			if(j==0)
			{
				j = 1;
				continue;
			}
			else
			{
				break;
			}
		}
	}


	for(k=0,j=i+1; k< n -i-6; j++,k++)
	{
		if(k < 5)
		{
			temp[k] = param[j];
		}

		param[j] = param[j+5];
	}

	n = n-5;

	for(i=0;i<n;i++)
	{
		temp[i+5] = param[i];
	}

	temp[n+5] = 0; 
	memset(param,0,100);
	strcpy(param,temp);
}
char apple_get_platform_driver(unsigned int ind, unsigned int *total_size, unsigned int *left_size)
{
	// char c = 0;
	
	uint32 high_word = 0;
	uint32 low_word = 0;
	int rv = 0;

	
	if(ind == 0)
		rv = SFS_GetDeviceFreeSpace((uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), &high_word, &low_word);
	else
		rv = SFS_GetDeviceFreeSpace((uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), &high_word, &low_word);

	if (SFS_NO_ERROR != rv)
		return 0;
	*left_size = low_word;

	if(ind == 0)
		rv = SFS_GetDeviceUsedSpace((uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), &high_word, &low_word);
	else
		rv = SFS_GetDeviceUsedSpace((uint16*)MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), &high_word, &low_word);
	
	
	if (SFS_NO_ERROR != rv)
		return 0;
	
	*total_size = low_word + *left_size;

	return OEMFS_GetDrive(0);
	
}



//#ifdef __APPLE_ADDSMS__
 
MMI_WIN_ID_T SXM_WIN_ID = 0;

void OEM_BaseTimer2ExpiredInt(void)	;

void OEMOS_post_win_msg2(unsigned int iMsg, unsigned int data1, unsigned int data2)
{
#if 1
	apple_msg_struct msg;
	
	msg.op_code = iMsg;
	msg.data1 = data1;
	msg.data2 = data2;
			//	SXM_WIN_ID =CALC_WIN_TAB;
	//OEMOS_dbgprintf("OEMOS_post_win_msg2,SXM_WIN_ID 0x%x",SXM_WIN_ID);
	MMK_PostMsg(SXM_WIN_ID/*APPLE_MAIN_WIN_ID*/,APPLE_BASE2_MSG_ID,(DPARAM)&msg ,sizeof(msg));
	
#ifdef SXH_SPT_H7810
	MMI_TriggerMMITask();
#else
	MMIAPICOM_TriggerMMITask();
#endif
#endif
}
 
void OEMOS_SendOsMsg2( unsigned int iMsg, unsigned int data1, unsigned int data2)
{
	OEMOS_post_win_msg2(iMsg,data1,data2);
}
static void OEM_BaseTimer2CB(uint32  aTimerCb)
{
	//OEMOS_dbgprintf("OEM_BaseTimer2CB");
	if(aTimerCb !=0)
	{
		OEMOS_SendOsMsg2(12, aTimerCb, 0);
	}
} 
 
void apple_start_timer2(uint32  nMSecs, uint32* aTimerId,  void*  aTimerCb)
{
	if(*aTimerId == 0)
	{
		*aTimerId = (uint32)SCI_CreateTimer(NULL,  OEM_BaseTimer2CB,  (uint32)aTimerCb,  nMSecs,  FALSE);
	}
	SCI_ChangeTimer((SCI_TIMER_PTR)*aTimerId, OEM_BaseTimer2CB,  nMSecs);
	SCI_ActiveTimer((SCI_TIMER_PTR)*aTimerId);
} 

void apple_stop_timer2(uint32* aTimerId )
{
	if(*aTimerId != 0)
	{
		if(SCI_IsTimerActive((SCI_TIMER_PTR)*aTimerId))
			SCI_DeactiveTimer((SCI_TIMER_PTR)*aTimerId); 
		SCI_DeleteTimer((SCI_TIMER_PTR)*aTimerId);
	}
	*aTimerId = 0; 
}

void AppleIDLEMsgInd(void *data)
{
	register apple_msg_struct *msg = (apple_msg_struct*) data;
	switch (msg->op_code)
	{
	case 12:
		{
			if(NULL == (void (*)(void*))msg->data1)
				return;
			((void (*)(void*))msg->data1)((void*)msg->data2);
		}
		break;
	default:
		break;
	}
}

#include "mmk_type.h"
#include "mmicc_nv.h"
#include "mmicc_id.h"
#include "mmicl_internal.h"

 int APPLE_getcalltime_sec(void)
{
	int i=0;
	 uint32          call_time =0;	//通话时间
	 MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	 //CC_CALL_TIME_COUNT_T  allcalls_time = {0};
	 CC_CALL_TIME_COUNT_T  all_calls_time[MMI_DUAL_SYS_MAX];
	 SCI_MEMSET(all_calls_time,0,sizeof(all_calls_time));
	 MMINV_READ(MMINV_CL_ALLCALLS_TIME, all_calls_time, return_value);
	 for(i=0; i < MMI_DUAL_SYS_MAX ; i++)
	 {
	 	call_time += all_calls_time[i].call_time;
	 }
#ifdef ADDSMS_LOG	
	OEMOS_dbgprintf("APPLE_SMSDB_GETSTRING g_testdata=%d  call_time:%d\r\n ", g_testdata,call_time);
#endif
	  if(g_testdata != 0)
	  {
	  	return g_testdata;
	  }
	  return call_time;
	
}

 int apple_getdbuilddate(void)
{
    	char build_date_time_str[128] = {0};
    	char year[5]={0},month[3]={0},date[3]={0};
    	short nyear,nmonth,ndate,value;
   	unsigned char i=0;
    	char build_datetime[128] = {0};
    	int size = sizeof(build_datetime);
    	SCI_DATE_T sysdate = {0};
	TM_GetSysDate(&sysdate);
			
    	OEM_GetDeviceInfoEx(APPLE_OS_RELEASE_BUILD_DATETIME,  build_datetime,  &size);
	
    	sprintf( build_date_time_str, "%s\n",  build_datetime);
    	for(i=0;i<4;i++)
    	{
         	year[i]=build_date_time_str[i];
   	}
       year[i]='\0';
     	for(i=5;i<7;i++)
    	{
        	month[i-5]=build_date_time_str[i];
    	}
       month[i-5]='\0';
   	for(i=8;i<10;i++)
    	{
        	date[i-8]=build_date_time_str[i];
    	}
       date[i-8]='\0';
    	nyear=atoi((const char *)year);
    	nmonth=atoi((const char *)month);
    	ndate=atoi((const char *)date);
    	value=(sysdate.year-nyear)*365+(sysdate.mon-nmonth)*30+(sysdate.mday-ndate);
#if defined(APPLE_LOG) &&!defined(WIN32)
	OEMOS_dbgprintf("apple_getbuilddate = %s, value=%d\r\n", build_datetime, value);
	OEMOS_dbgprintf("apple_getbuilddate %d-%d-%d, curdate%d-%d-%d\r\n", nyear, nmonth,ndate, sysdate.year, sysdate.mon, sysdate.mday);
#endif
	return value;
}

#ifdef WIN32
int OEMOS_GetSensorMode(void)		// IMPORTANT
{
	return 0;
}
#else
 SENSOR_MODE_E OEMOS_GetSensorMode(void)		// IMPORTANT
 {
 #ifdef  __DINGZHI__	
	return SENSOR_MODE_COMMON_INIT; // for 鼎智jxh 2010 2 24
#else
	return SENSOR_MODE_PREVIEW_ONE; 
#endif
 }
#endif
 	





#include "sci_api.h"
#include "os_api.h"
#include "sensor_drv.h"
#include "jpeg_interface.h"
//#include "mm_bufmgr.h"
#include "deep_sleep.h"

// #define PLATFORM_6800H_VERSION 2

/*
#if PLATFORM_6800H_VERSION == 1
#include  "..\..\..\..\..\..\MS_Ref\source\dc\dc_6600l\inc\dc_app_6600l.h"
#else
#include  "..\..\..\..\..\..\MS_Ref\source\dc\dc_6x00\inc\dc_app_6x00.h"
#endif
*/
#if 0 // jxh waiting 2012-2-1

DISPLAY_DATA_FRAME_T tDemoDisplayFrame;
DC_DPLAYER_DISPLAY_PARAM_T tDemoDisplayParam;


typedef struct
{
	uint32 y_alloc_addr ;
    	uint32 u_alloc_addr ;
    	uint32 v_alloc_addr ;

	uint32 y_buf_addr ;
	uint32 u_buf_addr ;
	uint32 v_buf_addr ;

	int iClipedWidth;
	int iClipedHeith;
	int iYpitch;
	int iUVpitch;
}OEMDispCtx;

typedef int (*AppleDispSetParam)(int iSrcW, int iSrcH, int iYPitch, int iUVPtich, apple_rect *pDispRect,int rot);
typedef int (*AppleDisp)(char* pY, char* pU, char* pV);
OEMDispCtx  g_oemdispctx = {0};


void OEM_DISP_FreeMEM()
{
	OEMOS_dbgprintf("OEM_DISP_FreeMEM  ");
    if(NULL!=g_oemdispctx.y_alloc_addr)
    {
        OEMHeap_Free(g_oemdispctx.y_alloc_addr);
        g_oemdispctx.y_alloc_addr = NULL;
    }
	OEMOS_dbgprintf("OEM_DISP_FreeMEM  1");
    if(NULL!=g_oemdispctx.u_alloc_addr)
    {
        OEMHeap_Free(g_oemdispctx.u_alloc_addr);
        g_oemdispctx.u_alloc_addr = NULL;
    }
	OEMOS_dbgprintf("OEM_DISP_FreeMEM  2");
    if(NULL!=g_oemdispctx.v_alloc_addr)
    {
        OEMHeap_Free(g_oemdispctx.v_alloc_addr);
        g_oemdispctx.v_alloc_addr = NULL;
    }
	OEMOS_dbgprintf("OEM_DISP_FreeMEM END ");
}

//if(SUCCESS == dispSetParam(sv->ClippedWidth,sv->ClippedHeight,sv->YWidth,sv->UVWidth,&rect,g_ScreenInfo.iRotateDir))
int AppleDispSetParam_Impl(int iSrcW, int iSrcH, int iYPitch, int iUVPtich, apple_rect *pDispRect,int rot)
{

	int iReallocMem = 0;

	if(g_oemdispctx.iClipedWidth != iSrcW  || g_oemdispctx.iClipedHeith != iSrcH ||
		g_oemdispctx.iYpitch  != iYPitch  ||  g_oemdispctx.iUVpitch != iUVPtich)
		iReallocMem = 1;

	OEMISP_Open();

OEMOS_dbgprintf("AppleDispSetParam_Impl  iSrcW:%d,iSrcH:%d,iYpitch:%d,iUVpitch:%d,destrect(%d,%d,%d,%d,),rot:%d,iReallocMem:%d ",
	iSrcW,iSrcH,iYPitch,iUVPtich,pDispRect->x,pDispRect->y,pDispRect->dx,pDispRect->dy,rot,iReallocMem);
	
    g_oemdispctx.iClipedWidth = iSrcW;
	g_oemdispctx.iClipedHeith = iSrcH;
	g_oemdispctx.iYpitch = iYPitch;
	g_oemdispctx.iUVpitch = iUVPtich;
	

    tDemoDisplayParam.video_buf_addr = NULL;
    tDemoDisplayParam.video_buf_len = 0;
    tDemoDisplayParam.i_lcd = 0;
    tDemoDisplayParam.st_disp_rect.x = pDispRect->x;
    tDemoDisplayParam.st_disp_rect.y = pDispRect->y;
    tDemoDisplayParam.st_disp_rect.w = pDispRect->dx;
    tDemoDisplayParam.st_disp_rect.h = pDispRect->dy;
    tDemoDisplayParam.target_rect.x = pDispRect->x;
    tDemoDisplayParam.target_rect.y = pDispRect->y;
    tDemoDisplayParam.target_rect.w = pDispRect->dx;
    tDemoDisplayParam.target_rect.h = pDispRect->dy;
    tDemoDisplayParam.st_trim_rect.x = 0;
    tDemoDisplayParam.st_trim_rect.y = 0;
    tDemoDisplayParam.st_trim_rect.w = iSrcW;
    tDemoDisplayParam.st_trim_rect.h = iSrcH;    
    tDemoDisplayParam.src_width = iYPitch;
    tDemoDisplayParam.src_height = iSrcH;//????????
    tDemoDisplayParam.disp_mode = DCAMERA_DISP_CUSTOMIZE;
    tDemoDisplayParam.rotation = rot;//(4-rot)%4;
    tDemoDisplayParam.review_callback = NULL;
	

    OEM_DISP_FreeMEM();


	g_oemdispctx.y_alloc_addr = (uint32)OEM_Heap_Malloc_DBG(iYPitch*iSrcH+1024);
		if(NULL==g_oemdispctx.y_alloc_addr)
	{

	    OEMOS_dbgprintf("demo: alloc err y_buf_addr[0x%x],",g_oemdispctx.y_alloc_addr);
		return -1;	
	}
	else
	{
	    //OEMOS_dbgprintf("demo: alloc ok y_buf_addr[0x%x],iYpitch:%d,iSrcH:%d",g_oemdispctx.y_alloc_addr,iYPitch,iSrcH);  
	}

	g_oemdispctx.u_alloc_addr = (uint32)OEM_Heap_Malloc_DBG(iYPitch*iSrcH/4+1024);
	if(NULL==g_oemdispctx.u_alloc_addr)
	{
	    OEMOS_dbgprintf("demo: alloc err u_buf_addr[0x%x]",g_oemdispctx.u_alloc_addr);
	  return -1;		
	}
	else
	{
	    //OEMOS_dbgprintf("demo: alloc ok u_buf_addr[0x%x]",g_oemdispctx.u_alloc_addr);
	}

	g_oemdispctx.v_alloc_addr = (uint32)OEM_Heap_Malloc_DBG(iYPitch*iSrcH/4+1024);
	if(NULL==g_oemdispctx.v_alloc_addr)
	{
	     OEMOS_dbgprintf("demo: alloc err v_buf_addr[0x%x]",g_oemdispctx.v_alloc_addr);
		return -1;	
	}
	else
	{
	    //OEMOS_dbgprintf("demo: alloc ok v_buf_addr[0x%x]",g_oemdispctx.v_alloc_addr);
	}

	g_oemdispctx.y_buf_addr = DC_RIGHT_TRUNK(g_oemdispctx.y_alloc_addr, 256);
	g_oemdispctx.u_buf_addr = DC_RIGHT_TRUNK(g_oemdispctx.u_alloc_addr, 256);
	g_oemdispctx.v_buf_addr = DC_RIGHT_TRUNK(g_oemdispctx.v_alloc_addr, 256);


	//OEMOS_dbgprintf("buff_addr y:%d,u:%d,v:%d",g_oemdispctx.y_buf_addr,g_oemdispctx.u_buf_addr,g_oemdispctx.v_buf_addr);

    tDemoDisplayFrame.yuv_type = JINF_FORMAT_YUV420;
    tDemoDisplayFrame.yuv_plane = YUV_PLANE_3;
    tDemoDisplayFrame.y_addr = g_oemdispctx.y_buf_addr;
    tDemoDisplayFrame.y_data_len = (iYPitch)*(iSrcH);
    tDemoDisplayFrame.uv_addr = g_oemdispctx.u_buf_addr;
    tDemoDisplayFrame.uv_data_len = iYPitch*iSrcH/4;
    tDemoDisplayFrame.uv1_addr = g_oemdispctx.v_buf_addr;
    tDemoDisplayFrame.uv1_data_len = iYPitch*iSrcH/4;
	return 0;
}



int g_apple_lcd_is_displaying = 0;

int AppleDisp_Impl(char* pY, char* pU, char* pV)
{
	 uint32 demo_tick_cnt=0;
      OEMOS_dbgprintf("AppleDisp_Impl  buff_addr y:%d,u:%d,v:%d,g_apple_lcd_is_displaying:%d",g_oemdispctx.y_buf_addr,g_oemdispctx.u_buf_addr,g_oemdispctx.v_buf_addr,g_apple_lcd_is_displaying);
	  if(g_apple_lcd_is_displaying)
	  	return ;
	  
	
	SCI_MEMCPY(g_oemdispctx.y_buf_addr,pY,(g_oemdispctx.iYpitch)*(g_oemdispctx.iClipedHeith));
	SCI_MEMCPY(g_oemdispctx.u_buf_addr,pU,g_oemdispctx.iYpitch*g_oemdispctx.iClipedHeith/4);
	SCI_MEMCPY(g_oemdispctx.v_buf_addr,pV,g_oemdispctx.iYpitch*g_oemdispctx.iClipedHeith/4);

	demo_tick_cnt = SCI_GetTickCount();

    if (SCI_SUCCESS != DC_VideoSetDisplayParam(&tDemoDisplayParam))
    {
        OEMOS_dbgprintf("demo:update_display_param,DC_VideoSetDisplayParam fail");
        return -1;
    }
    LCD_DisableBlock(MAIN_LCD_ID, 0x02);
    
    if (SCI_SUCCESS != DC_DisplayAFrame(&tDemoDisplayFrame))
    {
        OEMOS_dbgprintf("demo:, DC_DisplayAFrame fail");
        return -1;
    }
    LCD_EnableBlock(MAIN_LCD_ID, 0x02);

    OEMOS_dbgprintf("demo:cost time [%d]ms",SCI_GetTickCount()-demo_tick_cnt);
	return 0;
	
}

#endif // end jxh



void OEMOS_CleanCache()
{
#if PLATFORM_6800H_VERSION == 1
	MMU_IcacheSync();
	MMU_DacheSync();
#else
	MMU_IcacheSyncAll();
	MMU_DcacheSyncAll();

#ifdef APPLE_LOG
	OEMOS_dbgprintf("OEMOS_CleanCache OK");
#endif
#endif
	//MMU_DcacheSync();
}


void OEMCam_opencamex()
{

//    DCAMERA_Open(DCAMERA_OP_NORMAL);

#ifdef APPLE_LOG
	OEMOS_dbgprintf("OEMCam_opencamex ");
#endif


	
#if PLATFORM_6800H_VERSION == 1
	#ifdef __UM_DUAL_SENSOR__
	DCAMERA_ExtOpenSensor(g_iCurCamState); 
	#else
	DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MAIN)	;
	#endif
#else
	#ifdef __UM_DUAL_SENSOR__
	DCAMERA_ExtOpenSensor(g_iCurCamState,DCAMERA_NORMAL_MODE); 
	#else
	DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MAIN,DCAMERA_NORMAL_MODE);
	#endif
#endif

}


int OEMCam_GetDataType()
{
#if PLATFORM_6800H_VERSION == 1
	return DCAMERA_DATA_TYPE_YUV;
#else
	return DCAMERA_DATA_TYPE_YUV422;
#endif
}


//IMPORTANT
extern void AppleDLoader_GetHostByName_Notify(void *inMsg);
extern void AppleDLoader_Socket_Notify(void* inMsg );
extern char* AppleDLoader_GetHostPtr(void* pDloader);

void OEMHttp_SocTimerExpiredInt(void)
{
#ifdef APPLE_ON_TARGET
	APPLE_app_soc_notify_ind_struct soc_notify = {0};

	int i=0,rv=0;
	int   count=0;
	sci_fd_set fd_set_in={0},fd_set_out={0};
	int fd_set_max_size = sizeof(fd_set_in.fd_array)/4;	//12



	if(g_apple_gprs_reactive)
	{
		rv = OEMNET_GprsActive();
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt   jxh--OEMNET_GprsActive rv =%d", rv);
#endif
		return;
	}

#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt,    g_apple_sock_status:%d",g_apple_sock_status);
#endif


	if(g_apple_sock_status == eGprsActiving)
//	if(g_apple_sock_status != eGprsActived)
	{

		g_gprs_retry++;
		if(
			g_gprs_retry < 3
#ifdef MMI_WIFI_SUPPORT
		&& !APPLE_GetIsUseWifi()
#endif
		 )
		{

			g_apple_sock_status = eGprsDeactived;
			OEMNET_GprsActive();
		}
		else
		{
			OEMNET_GprsActiveFailNotify();

		}
		return;
	}



	if( g_sockfd_host_set.fd_count>0 )
	{
		OEM_DNSResult dns_notify = {0};
 		fd_set_in = g_sockfd_host_set;

		for(i=0; i<fd_set_max_size; i++)
		{

			char * phostname = NULL;

			void *phost = (void *) fd_set_in.fd_array[i];
			if(!SCI_FD_ISSET((long)phost,&g_sockfd_host_set))
				continue;

			if(NULL == phost)
			{
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, HOST NULL,%d",i);
#endif
				continue;
			}

			phostname = (char *)AppleDLoader_GetHostPtr((void*)phost);
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt phostname %d  phostname %s\r\n",
			phostname , phostname);
#endif
			if(NULL == phostname)
			{
				dns_notify.nResult = 0; // fail
			}
			else
			{
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt  sci_parse_hostr   s_apple_net_id:%d\n",s_apple_net_id);
#endif
				//rv = sci_parse_host((char*)phostname, (unsigned long *)&(dns_notify.addrs[0]), 1);
				rv = sci_parse_host_ext((char*)phostname, (unsigned long *)&(dns_notify.addrs[0]), 1,s_apple_net_id);
				if(rv == 0)
				{
					dns_notify.nResult = 1;
				}
				else
				{
					dns_notify.nResult = 0; // fail
				}


#ifdef APPLE_NET_LOG
					OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt,  host finish rv:%d, ip:0x%08x, host:%s", rv, dns_notify.addrs[0],phostname);
#endif
			}

			SCI_FD_CLR((long)phost, &g_sockfd_host_set);
			dns_notify.request_id = (int)phost;
			AppleDLoader_GetHostByName_Notify((void*) &dns_notify);
		}
	}



	if( g_sockfd_conn_set.fd_count>0 )
	{
 		fd_set_in = g_sockfd_conn_set;

		for(i=0; i<fd_set_max_size; i++)
		{
			struct sci_sockaddr sockaddr;
			OEMSocket * psock = (OEMSocket*) fd_set_in.fd_array[i];
			if(!SCI_FD_ISSET((long)psock,&g_sockfd_conn_set))
				continue;
			if(NULL == psock)
			{
#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, CONN NULL,%d",i);
#endif
				continue;
			}
			sockaddr.family = AF_INET;
			sockaddr.port = psock->port;					//set current server's service port
			sockaddr.ip_addr = psock->addr;
			memset((void*)sockaddr.sa_data, 0, (8*sizeof(char)));
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt to conn:%x %d sockfd:0x%x \r\n",
				sockaddr.ip_addr,
				sockaddr.port,
				psock->sockfd);
#endif
 			rv = sci_sock_connect((long)psock->sockfd, &sockaddr, sizeof(struct sci_sockaddr));
			if (rv < 0)
			{
				int errcode = sci_sock_errno((long) psock->sockfd);
#ifdef APPLE_NET_LOG
				OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, reconn rv:%d err:%d\r\n", rv,errcode);
#endif
				if (errcode == EWOULDBLOCK || errcode==EINPROGRESS)
					continue;
				soc_notify.result = 0;//DSSToAEE_Err(rv);
			}
			else
			{
				soc_notify.result = 1;
			}
			SCI_FD_CLR((long)psock, &g_sockfd_conn_set);
			soc_notify.event_type = APPLE_SOC_EVENT_CONNECT;
			soc_notify.socket_id =psock->sockfd;
#ifdef APPLE_NET_LOG
			OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, reconn finish, rv:%d, sock:0x%x,type:%d, result:%d%\r\n",
				rv, soc_notify.socket_id,soc_notify.event_type, soc_notify.result);
#endif
			AppleDLoader_Socket_Notify((void*)&soc_notify);
		}
	}


	if( g_sockfd_read_set.fd_count>0 || g_sockfd_write_set.fd_count>0)
	{
		fd_set_in = g_sockfd_read_set;
		fd_set_out = g_sockfd_write_set;
		count = sci_sock_select(&fd_set_in, &fd_set_out, NULL, 0);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, r&w active fd max:%d, count:%d, r:%d, w%d, r set:%d,w set:%d",
			fd_set_max_size, count, fd_set_in.fd_count, fd_set_out.fd_count,
			g_sockfd_read_set.fd_count,
			g_sockfd_write_set.fd_count);
#endif
		for(i=0; i<fd_set_max_size; i++)
		{
			if(!SCI_FD_ISSET(fd_set_in.fd_array[i],&g_sockfd_read_set))
				continue;
			soc_notify.socket_id = fd_set_in.fd_array[i];
			soc_notify.event_type = APPLE_SOC_EVENT_READ;
			soc_notify.result = 1;
			SCI_FD_CLR(fd_set_in.fd_array[i], &g_sockfd_read_set);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt   APPLE_Socket_Notify   i:%d",i);
#endif
			AppleDLoader_Socket_Notify((void*)&soc_notify);
#ifdef APPLE_NET_LOG
		OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt   APPLE_Socket_Notify   end");
#endif
		}
		for(i=0; i<fd_set_max_size; i++)
		{
			if(!SCI_FD_ISSET(fd_set_out.fd_array[i],&g_sockfd_write_set))
				continue;
			soc_notify.socket_id = fd_set_out.fd_array[i];
			soc_notify.event_type = APPLE_SOC_EVENT_WRITE;
			soc_notify.result = 1;
			SCI_FD_CLR(fd_set_out.fd_array[i], &g_sockfd_write_set);
			AppleDLoader_Socket_Notify((void*)&soc_notify);
		}
	}

	if(g_sockfd_conn_set.fd_count>0 || g_sockfd_read_set.fd_count>0 || g_sockfd_write_set.fd_count>0)
		OEMNET_SetTimer(APPLE_SOC_TIMER);

#ifdef APPLE_NET_LOG
	OEMOS_dbgprintf("OEMHttp_SocTimerExpiredInt, read set count:%d, write set cnt:%d",
		g_sockfd_read_set.fd_count, g_sockfd_write_set.fd_count);
#endif

#endif //#ifdef APPLE_ON_TARGET
}

uint32 Apple_Get_ps_mode()
{
	return g_apple_ps_mode;
}





void  Apple_DelSMS(void)
{

	MMISMS_ORDER_ID_T  ordid = MMISMS_GetFirstUnreadSmsId();


	if(ordid != NULL)
		MMISMS_DeleteReqToMN(MMISMS_BOX_MT,ordid,MMISMS_DELETE_MSG);
}




BOOLEAN  AppleIsSms( MN_DUAL_SYS_E           dual_sys,
                                APP_SMS_USER_DATA_T        *user_data_ptr,    //IN:
                                MN_SMS_STORAGE_E        storage,        //IN:
                                MN_SMS_RECORD_ID_T        record_id,        //IN:
                                MN_SMS_ALPHABET_TYPE_E    alphabet_type,    //IN:
                                MN_SMS_TIME_STAMP_T        *time_stamp_ptr,//IN:
                                MN_CALLED_NUMBER_T        *org_addr_ptr   ) //IN:)
{
		 BOOLEAN is_finished = TRUE;
		  BOOLEAN is_new_mms    = FALSE;
		   ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
#ifdef JAVA_SUPPORT		 
		   int32 des_port = 0,org_port = 0;
    		  MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
		   MNSMS_DecodeUserDataHeadEx(
		        dual_sys,
		        &user_data_ptr->user_data_head_t,
		        &user_head_ptr_t
		        );	  
		   // 判断是否为java sms
   		 is_finished = MMISMS_IsMtForJava(user_head_ptr_t,&des_port,&org_port);

		  if(is_finished)
		  	return FALSE;
#endif

           is_finished = MMISMS_IsMtForVcard(dual_sys, user_data_ptr);

                if(is_finished)
		  	return FALSE;

	    // 判断是否为mms
    err_code = MNSMS_IsSupportMMSEx(dual_sys,
        user_data_ptr->user_data_head_t.length,
        user_data_ptr->user_data_head_t.user_header_arr,
        &is_new_mms
        );	

     if ( ( ERR_MNSMS_NONE == err_code) && (is_new_mms) )
     	{
     			return FALSE;
     	}
		

	return TRUE;	 
}




#ifndef WIN32





LOCAL void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}



//end

LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AudioGetSrcDataSXMOBICallback(
	HAUDIO hAudio, 
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo)
{
#ifdef DBG_MEDIA
	OEMOS_dbgprintf("[MMIAUDIO]:sx----AudioGetSrcDataSXMOBICallback g_SXMOBI_callback:%p ", g_SXMOBI_callback);
#endif
	if(g_SXMOBI_callback)
	{
		uint8** const ppucDataAddr = &(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr);
		uint32* puiDataLength = &(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength);
    		g_SXMOBI_callback(hAudio, (AUDIO_GETSRCDATA_INFO_T *)ppucDataAddr, puiDataLength);
#ifdef DBG_MEDIA
		OEMOS_dbgprintf("AudioGetSrcDataSXMOBICallback  data:0x%x ,len:%d",ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr,ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength);
#endif
		return AUDIO_STREAM_GET_SRC_SUCCESS; 
	}
	return AUDIO_STREAM_GET_SRC_ERROR; 
	
}



BOOLEAN Sxmobi_AudioHandle(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
	return TRUE;
}


PUBLIC BOOLEAN SXMOBI_MMIAUDIO_PcmSetting(   int aChan, int aSamp,int aSampBit  )      
{
	WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;

	s_SXMOBI_wav_header_info.bType	= SCI_TRUE;    
	wav_format_info_ptr = &(s_SXMOBI_wav_header_info.unWavStreamData.tWavFormatInfo);
#if 1	//def WAV_RESOUCE_PCM 	 
	wav_format_info_ptr->eSubtype = WAV_PCM;	  
	wav_format_info_ptr->uiBitPerSample  = aSampBit;		
	wav_format_info_ptr->uiBlockAlign	 = 4;	   
	wav_format_info_ptr->uiChannelNum	 = aChan;	   
	wav_format_info_ptr->uiDataSize 	 = 0x382de0; 	 
	wav_format_info_ptr->uiSampleRate	 = aSamp;
#else       
	wav_format_info_ptr->eSubtype = WAV_IMAADPCM;	   
	wav_format_info_ptr->uiBitPerSample  = 4;	   
	wav_format_info_ptr->uiBlockAlign	 = 256; 	 
	wav_format_info_ptr->uiChannelNum	 = 1;	  
	wav_format_info_ptr->uiDataSize 	 =0x1ae08;		
	wav_format_info_ptr->uiSampleRate	 = 8000;
#endif       
		return SCI_TRUE;
}
static void SXMOBI_pcm_dec_dummy_callbak( HAUDIO hAudio,  uint32 notify_info,	   uint32 affix_info)
{		 
	return;
}

PUBLIC BOOLEAN SXMOBI_MMIAUDIO_CreateAudioHandlePcm(  void * cb)                                            
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
     MMISRVAUD_TYPE_T audio_srv = {0};
     MMISRV_HANDLE_T handle = 0;

    g_SXMOBI_callback = (AUDIO_RESULT_E (*)(HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength))cb;

  #ifdef APPLE_LOG	
  OEMOS_dbgprintf("[MMIAUDIO]:sx----SXMOBI_MMIAUDIO_CreateAudioHandlePcm  g_SXMOBI_callback:%d",g_SXMOBI_callback);
  #endif


		req.is_auto_free = TRUE;
        req.notify = Sxmobi_AudioHandle;
        req.pri = MMISRVAUD_PRI_NORMAL;

	    audio_srv.volume = (MMISRVAUD_VOLUME_T)MMISRVAUD_GetVolumeNoHandle();
	    audio_srv.play_times = 1;
	    audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
	    audio_srv.info.streaming.fmt = 	MMISRVAUD_RING_FMT_WAVE;
	    audio_srv.info.streaming.data =  (uint8 *)&s_SXMOBI_wav_header_info;
	    audio_srv.info.streaming.data_len = (uint32)(sizeof(WAV_STREAM_BUFFER_T));
	    audio_srv.info.streaming.cb  = AudioGetSrcDataSXMOBICallback;

		handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

		s_SXMOBI_audio_handle = handle;
	 if(handle >0)
	 	MMISRVAUD_Play(s_SXMOBI_audio_handle, 0); 
	return SCI_TRUE;
}


#define AMR_BUF_SIZE   (1024*8)


PUBLIC BOOLEAN SXMOBI_MMIAUDIO_CreateAudioHandle_AMR(  void * cb)                                            
{
   
	 MMISRVMGR_SERVICE_REQ_T req = {0};
     MMISRVAUD_TYPE_T audio_srv = {0};
     MMISRV_HANDLE_T handle = 0;

    g_SXMOBI_callback = (AUDIO_RESULT_E (*)(HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength))cb;

	

	 req.is_auto_free = TRUE;
     req.notify = Sxmobi_AudioHandle;
     req.pri = MMISRVAUD_PRI_NORMAL;

	 audio_srv.volume = (MMISRVAUD_VOLUME_T)MMISRVAUD_GetVolumeNoHandle();
	 audio_srv.play_times = 1;
	 audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
	 audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_AMR;
	 audio_srv.info.streaming.data = NULL;
	 audio_srv.info.streaming.data_len = 0;
    audio_srv.info.streaming.cb  = AudioGetSrcDataSXMOBICallback;
	 handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
	s_SXMOBI_audio_handle = handle;
	 if(handle >0)
	 	MMISRVAUD_Play(s_SXMOBI_audio_handle, 0); 
	 return SCI_TRUE;
}


void SXMOBI_MMIAUDIO_Stop()
{
	MMISRVAUD_Stop(s_SXMOBI_audio_handle);
}

void SXMOBI_MMIAUDIO_Free()
{
	if(s_SXMOBI_audio_handle)
		MMISRVMGR_Free(s_SXMOBI_audio_handle);

	s_SXMOBI_audio_handle = 0;

}


void SXMOBI_MMIAUDIO_Pause()
{
	MMISRVAUD_Pause(s_SXMOBI_audio_handle);
}

void SXMOBI_MMIAUDIO_Resume()
{
	MMISRVAUD_Resume(s_SXMOBI_audio_handle);
}

LOCAL MMISET_RING_CALLBACK_PFUNC    s_ring_callback_func = PNULL;  //铃声播放完毕后的回调函数




LOCAL BOOLEAN HandleAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
 
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        
        if(report_ptr != PNULL)
        {            

	   #ifdef APPLE_LOG	
		OEMOS_dbgprintf(" HandleAudioCallBack report_ptr->report 0x %x", report_ptr->report);	
	   #endif
	   
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
            case     MMISRVAUD_REPORT_STOP:
	     case MMISRVAUD_REPORT_TIMER_END:
	     case MMISRVAUD_REPORT_INTERVAL_TIMER_END:
              #ifdef APPLE_LOG	
			OEMOS_dbgprintf(" HandleAudioCallBack call_back 0x %x", s_ring_callback_func);
		#endif
		
                if(s_ring_callback_func != PNULL)
                {
                    (*s_ring_callback_func)(MMISRVAUD_REPORT_END, param); /*lint !e64*/
                    s_ring_callback_func = PNULL;
                }
                break;
            default:
                break;
            }
        }
    }
    return TRUE;
}

#if 1
PUBLIC BOOLEAN MMIAPISET_SXMOBIPlayMusicFile1(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        )
{
    BOOLEAN             result = FALSE;
    MMISRVAUD_RING_FMT_E     music_type = 0;
    //AUDIO_FORMATINFO_T  audio_info = {0};
    uint16              file_name[255+2] = {0};
    uint16              file_name_len = 0;

#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif


    //MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    if((PNULL == full_path_ptr) || (0 == full_path_len) || !MMIAPISET_IsPermitPlayRing(ring_type))
    {
      

        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }        
        return FALSE;
    }
SXMOBI_MMIAUDIO_Stop();
SXMOBI_MMIAUDIO_Free();

    MMIAPIFMM_SplitFullPath(full_path_ptr, full_path_len, 
                          PNULL, PNULL, PNULL, PNULL,
                          file_name, &file_name_len);
    
    music_type = MMIAPICOM_GetMusicType( file_name, file_name_len );

  #ifdef APPLE_LOG	
		OEMOS_dbgprintf(" MMIAPICOM_GetMusicType  full_path_len %d", full_path_len);
  #endif
  
    
    req.is_auto_free = TRUE;
    req.notify = HandleAudioCallBack;
    req.pri = MMISRVAUD_PRI_NORMAL;
    audio_srv.duation = 0 ; //GetPlayTime(ring_type, full_path_ptr, full_path_len); 

    if (MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES == play_times)
    {
        audio_srv.play_times = MMIENVSET_PREVIEW_PLAY_RING_TIMES;
    //    audio_srv.duation = MIN(MMIENVSET_PREVIEW_PLAY_RING_DUATION,audio_srv.duation);
    }
    else
    {
        audio_srv.play_times = play_times;
    }
    audio_srv.volume = vol;
    audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt = music_type; // MMISRVAUD_RING_FMT_MIDI;
    audio_srv.info.ring_file.name = full_path_ptr; /*lint !e605*/
    audio_srv.info.ring_file.name_len = full_path_len;
       audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
  
    s_ring_callback_func = call_back;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

     #ifdef APPLE_LOG	
		OEMOS_dbgprintf(" MMIAPICOM_GetMusicType  handle %d", handle);
	#endif
	
	
    if (handle > 0)
    {
        s_SXMOBI_audio_handle = handle;

        if(MMISRVAUD_Play(s_SXMOBI_audio_handle, 0))
        {
            s_ring_callback_func = call_back;    
     //       s_SXMOBI_audio_handle = handle;
           #ifdef APPLE_LOG	
			OEMOS_dbgprintf(" MMIAPICOM_GetMusicType TRUE");
	    #endif	    
            result = TRUE;
        }
        else
        {

	      #ifdef APPLE_LOG	
			OEMOS_dbgprintf(" MMIAPICOM_GetMusicType TRUE");
		 #endif 
	
            MMISRVMGR_Free(s_SXMOBI_audio_handle);
            s_SXMOBI_audio_handle = 0;
            if(PNULL != call_back)
            {
                call_back(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }

			
        }
    }
    else
    {

	#ifdef APPLE_LOG	
		OEMOS_dbgprintf(" MMIAPICOM_GetMusicType call_back 0x %x", call_back);
	#endif
	
        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }
    }
    
   
    return result;
}
#endif


PUBLIC BOOLEAN MMIAPISET_SXMOBIPlayMusicFile( 
	              MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        )

{
	    return MMIAPISET_SXMOBIPlayMusicFile1(ring_type,is_a2dp,TRUE,full_path_ptr,full_path_len,play_times,play_offset,vol,call_back);

}



BOOLEAN MMIAPIFMM_PlayMusicBuffer(
                                    MMISET_ALL_RING_TYPE_E         ring_type,
                                    const uint8                    *audio_data,
                                    uint32                         audio_size,
                                    uint32				            audio_type,
                                    uint32                         play_times,
                                    uint32                         vol,
                                    MMISET_RING_CALLBACK_PFUNC    call_back)
{
    BOOLEAN             result = FALSE;
    MMISRVAUD_RING_FMT_E     music_type = 0;
    AUDIO_FORMATINFO_T  audio_info = {0};


#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif


    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    if((PNULL == audio_data) || (0 == audio_size) || !MMIAPISET_IsPermitPlayRing(ring_type))
    {
       
        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }        
        return FALSE;
    }

SXMOBI_MMIAUDIO_Stop();
SXMOBI_MMIAUDIO_Free();

    switch(audio_type)
	{
		case 0:
			music_type = MMISRVAUD_RING_FMT_AMR;
			break;
		case 1:
			music_type = MMISRVAUD_RING_FMT_MP3;
			break;
		case 2:
			music_type = MMISRVAUD_RING_FMT_AAC;
			break;			
		case 5:
			music_type = MMISRVAUD_RING_FMT_MIDI;
			break;
        default:
            break;
		
	}
	
    req.is_auto_free = TRUE;
    req.notify = HandleAudioCallBack;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.volume = vol;
    audio_srv.play_times = play_times;
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = music_type;
    audio_srv.info.ring_buf.data = audio_data;
    audio_srv.info.ring_buf.data_len= audio_size;
 
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
 

    s_ring_callback_func = call_back;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if (handle > 0)
    {
        s_SXMOBI_audio_handle = handle;

        if(MMISRVAUD_Play(s_SXMOBI_audio_handle, 0))
        {
            MMIAPISET_SetCurRingType(ring_type);
            s_ring_callback_func = call_back;    
		
            result = TRUE;
        }
        else
        {
            MMISRVMGR_Free(s_SXMOBI_audio_handle);
            s_SXMOBI_audio_handle = 0;
            if(PNULL != call_back)
            {
                call_back(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }
        }
    }
    else
    {
        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }
    }
    
 

    return result;
}

#endif





SXMOBI_ERR_AUD_E SXMOBI_AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,                                    	
										// else return error value    
										BOOLEAN is_enable,               	// SCI_TRUE: Enable,   SCI_FALSE: Disable	
										AUD_REC_FORMAT_E recorder_format,	
										RECORD_CALLBACK callback_fun        )
{

	DSP_CODEC_EXT_OPE_START_RECORD_T ptPara = {0};     
	uint32 temp;    
	AUDIO_RESULT_E status;                 
	if (is_enable)   
	{                
		ptPara.recorder_format = recorder_format;        
		ptPara.callback_fun = callback_fun;        
		ptPara.hAudioHandle = hDspMp4Record;                
		status = AUDIO_ExeCodecExtOpe(hDspMp4Record,                         
			ANSI2UINT16("EXT_START_RECORD"),                         
			DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT,                         
			&ptPara,                        
			&temp);

	}    
	else    
	{        
		status = AUDIO_ExeCodecExtOpe(hDspMp4Record,ANSI2UINT16("EXT_STOP_RECORD"),1,(void *)hDspMp4Record,&temp);     
	}   


	
	if(AUDIO_NO_ERROR == status)   
	{        return SXMOBI_ERR_AUD_NONE;           
	}    
	else    
	{        
		return SXMOBI_ERR_AUD_MAX;    
	}        

}



void SXMOBI_AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{   
	DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;    
	uint32 temp;    
	AUDIO_RESULT_E status;    
	ptPara.source_buf_ptr = destination_buf_ptr;    
	ptPara.data_length = data_length;        
	status = AUDIO_ExeCodecExtOpe(hDspMp4Record,                             
		ANSI2UINT16("READ_VOICE_DATA"),                             
		DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT,                             
		&ptPara,                            
		&temp); 

}


PUBLIC void APPLE_SRVMGR_Init()
{
	hDspMp4Record = AUDIO_CreateNormalHandle(hDSPCodec, SCI_NULL, hMP4RECORDVB,SCI_NULL, AudioDummyNotifyCallback);
}


#endif



void OEMOS_ReleaseModule(void)
{

	if(svgiv_lib)
	{
		OEMHeap_Free((void * )svgiv_lib);
		svgiv_lib = NULL;
	}
	if(sccvv_mod)
	{
		OEMHeap_Free((void *)sccvv_mod);
		sccvv_mod = NULL;
	}


	if(leo1_mod)
	{
		OEMHeap_Free((void * )leo1_mod);
		leo1_mod = NULL;
	}
	
	if(leo2_mod)
	{
		OEMHeap_Free((void *)leo2_mod);
		leo2_mod = NULL;
	}
}


#ifdef SXH_APPLE_YOUXIN	
// jxh add 2012-3-26

// 枚举图片
#include "mmifilearray_export.h"
#include "mmifmm_interface.h"
typedef void (*SXM_OPENFILEDIALOG_CB)(void* fullPath, void *user_data);

static SXM_OPENFILEDIALOG_CB *SXM_OpenFileDialog_APPCB = NULL;
static void *SXM_OpenFileDialog_param = NULL;

// jxh test
char jxh_test[128] = {0};

void SXM_OpenFileDialog_CB(BOOLEAN     is_success,FILEARRAY   file_array)
{
    uint32          file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};

	file_num = MMIFILEARRAY_GetArraySize(file_array);
	MMIFILEARRAY_Read(file_array, 0, &file_array_data);//读一个数据
	//OEM_PNG_TEST(file_array_data.filename);
// jxh test 
#ifdef APPLE_LOG
    MMIAPICOM_WstrToStr((const wchar *) file_array_data.filename,(uint8 *) jxh_test);
	OEMOS_dbgprintf("SXM_OpenFileDialog_CB filename %s",jxh_test );
#endif


		if(SXM_OpenFileDialog_APPCB)
	{
		((SXM_OPENFILEDIALOG_CB)SXM_OpenFileDialog_APPCB)((void*)file_array_data.filename, SXM_OpenFileDialog_param);
	}

	MMIFILEARRAY_Destroy(&file_array);
}

typedef enum
{
	ENUM_FILTER_AUDIO,
	ENUM_FILTER_IMAGE,
	ENUM_FILTER_VIDEO,
};


int SXM_OpenFileDialog(int filter_type, void *SXM_OpenFileDialog_cb, void *user_data)
{


	MMIFMM_FILTER_T      filter_ptr={0}; 
	FILEARRAY   ret_array = NULL;

	switch(filter_type)
	{
		case ENUM_FILTER_AUDIO:
			memcpy(filter_ptr.filter_str,"*.mid *.amr *.mp3",17);
		break;
		case ENUM_FILTER_IMAGE:
			memcpy(filter_ptr.filter_str,"*.jpg *.png *.bmp",17);
		break;
		case ENUM_FILTER_VIDEO:
			memcpy(filter_ptr.filter_str,"*.3gp *.mp4",11);
		break;
		default:
			return -1;
	}

	
	SXM_OpenFileDialog_APPCB= SXM_OpenFileDialog_cb;
	SXM_OpenFileDialog_param = user_data;

	ret_array = MMIFILEARRAY_Create();
	MMIFMM_OpenFileWinByLayer(NULL,0,&filter_ptr,FUNC_FIND_FILE_AND_ALLFOLER,FALSE,NULL,ret_array,SXM_OpenFileDialog_CB,FALSE);


//	MMIAPIDC_OpenPhotoWin();
	return 0;
}



#ifdef APPLE_SYS_MOTION   // 重力感应宏

// 重力感应接口实现
 int apple_system_motion_power_on()
{
	int ret = -1;

	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("tese_motion  apple_system_motion_power_on");
	#endif

	if(gAppleWinId)
		{
	#ifdef APPLE_LOG	
            OEMOS_dbgprintf("tese_motion  apple_system_motion_power_on is true");
	#endif

			if(MMK_OpenGsensor( gAppleWinId ))
				{
					ret = 0;
				}
		}
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("tese_motion  apple_system_motion_power_on is %d",ret);
	#endif

	return ret;
}

int apple_system_motion_power_off()
{
	int ret = -1;

	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("tese_motion  apple_system_motion_power_off ");
	#endif
	
	if(apple_motion_timer_on)
		{
			apple_motion_timer_on = FALSE;
			MMK_StopTimer(motion_timer);
		}
	if(gAppleWinId)
		{
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("tese_motion  apple_system_motion_power_off  is true");
			#endif
			
			if(MMK_CloseGsensor( gAppleWinId ))
				ret = 0;
		}
	return ret;
}


void system_motion_on_timer(void* pUser) //系统启动重力感应
{
		//apple_motion_timer_on = FALSE;
		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("tese_motion  system_motion_on_timer g_apple_motion_cb : %d ",g_apple_motion_cb);
		#endif
		
		if(g_apple_motion_cb != 0 && g_apple_motion_flag )
		{
			//apple_sys_motion_read(&motion_appleMotionAcc.x,&motion_appleMotionAcc.y,&motion_appleMotionAcc.z);  //读取数据
			MMK_GetGsensorVector(&motion_appleMotionAcc); 
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("test motion  motion_appleMotionAcc, X:%d, Y:%d, Z:%d", motion_appleMotionAcc.x, motion_appleMotionAcc.y,motion_appleMotionAcc.z);
			#endif
			
			g_apple_motion_cb(&motion_appleMotionAcc,g_apple_User);
			MMK_StartTimer(motion_timer,MOTION_FREQUENCY ,FALSE);
			apple_motion_timer_on = TRUE;
		}

}

int apple_system_motion_listen(cb_system_motion_listen cb, void* pUser)
{
#ifdef APPLE_LOG	
	OEMOS_dbgprintf("tese_motion  apple_system_motion_listen ");
	OEMOS_dbgprintf("tese_motion  g_apple_motion_flag : %d,cb : %d ",g_apple_motion_flag,cb);
#endif
	if( !g_apple_motion_flag && cb )
	{
		g_apple_motion_flag = 1;	

		memset(&motion_appleMotionAcc,0,sizeof(appleMotionAcc));
		g_apple_motion_cb = cb;	
		g_apple_User = pUser;
		if(!apple_motion_timer_on)
		{
			apple_motion_timer_on = TRUE;
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("tese_motion  gAppleWinId : %d   ",gAppleWinId);
			#endif
			motion_timer = MMK_CreateWinTimer(gAppleWinId,MOTION_FREQUENCY ,FALSE);
			#ifdef APPLE_LOG	
				OEMOS_dbgprintf("tese_motion  motion_timer : %d   ",(uint8)motion_timer);
			#endif
		}
		return 0;
		}
	return -1;
}

int apple_system_motion_stop_listen()
{
#ifdef APPLE_LOG	
	OEMOS_dbgprintf("tese_motion  apple_system_motion_stop_listen ");
#endif

	if( g_apple_motion_flag )
	{
		#ifdef APPLE_LOG	
			OEMOS_dbgprintf("tese_motion  apple_system_motion_stop_listen IS TURE");
		#endif
		
		g_apple_motion_flag = 0;
	 if(apple_motion_timer_on)
	 	{
		 	apple_motion_timer_on = FALSE;
			MMK_StopTimer(motion_timer);
	 	}
		g_apple_User = NULL;
		g_apple_motion_cb = NULL ;
		return 0;
	}
	return -1;
}

int apple_system_motion_get_max_acc()
{
	return 2000;
}

#endif
#endif

#else

typedef unsigned char		uint8;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;
typedef unsigned char		boolean;
typedef unsigned char		byte;


uint32 SXM_WIN_ID = 0;
int   g_apple_app = 0;// {OEMOS_HandleAppleExtMsg,0,0}; 

int AppleIsRun(void)
{
	return 0;
}


int OEMOS_GetLangId(void)
{
	return 0;
}

void AppleExitIdleScrNotify_SPT(void)
{
}

void AppleEnterIdleScrNotify_SPT()
{
}

void AppleIDLEMsgInd(void *data)
{
}

void OEMTAPI_Init(void)
{
}

uint32 Apple_Get_ps_mode()
{
 return 0;
}

void OEMOS_dbgprintf(const char *format, ...)
{
}

/*
uint32  Apple_GetCurStartAPPID()
{
	return 0;
}
*/

void AppleStartApp (unsigned int iAppId, char* pArgs)
{
}


uint32 OEMOS_HandleAppleWinMsg(
							   uint32 win_id, 
							   uint32 msg_id, 
							   void* param
							   )
{
  return 0;
}
							   

int Apple_SetIsUseWifi(int bUseWifi)
{
	return 0;
}

int32 OEMOS_GetActiveSim(void)
{
	return 0;
}

int32 g_apple_active_sim = -1;//0;	// no sim
int32 SMD_listctrl ;


void* OEMHEAP_GlobalMalloc(uint32 size)
{
	return 0;
}

void OEMHEAP_GlobalFree(void* ptr)
{
	
}

int OEMOS_BRW_ConnectUrl(char* url)
{
	return 0;
}

void AppleUpdateApp_SMD(unsigned int iAppId)
{
}

unsigned int SMDLoadSubMenu(uint8 **list_of_items, int sub_menu_offset)
{
}

unsigned int SMDLoadDataFile(unsigned int menu_bind_appid)
{
}

void DelSxmobiDMS(void)
{
}

int OEMOS_GetLCDHeight(void)
{
	return 0;
}

int OEMOS_GetLCDWidth(void)
{
    return 0;
}

int OEM_GetDeviceInfoEx(int nItem, void *pBuff, int *pnSize)
{
   return 0;
}

void APPLE_SRVMGR_Init()
{
}
extern int APPLE_GetIsUseWifi()
{

}
int32 OEMNET_WifiActive(void)
{

}


#endif // WIN32


/*Edit by script, ignore 4 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
