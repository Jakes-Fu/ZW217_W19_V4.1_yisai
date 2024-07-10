
#ifndef  _HERO_ENGINE_APP_H_    
#define  _HERO_ENGINE_APP_H_  


#include "mmidisplay_data.h"
#include "mmiset_export.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "block_mem_def.h"
#include "guistatusbar.h"
#include "mmipdp_export.h"


 
#ifdef __cplusplus
   extern   "C"
   {
#endif


//#define HERO_SLIM_OPEN
#define HERO_PORT_BASE_NILE

#define EXTPCM 
 

#if defined(MAINLCD_LOGIC_ANGLE_90) || defined(MAINLCD_LOGIC_ANGLE_270)
#define HR_SCREEN_WIDTH 						(MMI_MAINSCREEN_HEIGHT)
#define HR_SCREEN_HEIGHT 						(MMI_MAINSCREEN_WIDTH)
#else
#define HR_SCREEN_WIDTH 						(MMI_MAINSCREEN_WIDTH)

#if defined(MAINLCD_DEV_SIZE_240X284)
#define HR_SCREEN_HEIGHT 						(284)
#elif defined(MAINLCD_DEV_SIZE_240X240)
#define HR_SCREEN_HEIGHT 						(240)
#else
#define HR_SCREEN_HEIGHT 						(MMI_MAINSCREEN_HEIGHT)
#endif
#endif



#if 1//defined(PLATFORM_SC6800H)
	#define __HERO_ENGINE_CFG_VAR_VMMEM_SIZE__ (650*1024)//(4*1024*1024)
#else
#if defined(MAINLCD_SIZE_320X480)
	#define __HERO_ENGINE_CFG_VAR_VMMEM_SIZE__ (2*1024*1024)
#elif defined(MAINLCD_SIZE_240X400)  
	#define __HERO_ENGINE_CFG_VAR_VMMEM_SIZE__ (1024*1024)
#else
	#define __HERO_ENGINE_CFG_VAR_VMMEM_SIZE__ (1024*1024)
#endif
#endif

#define   __HERO_CFG_VMMEM_CUSTOM__     

#ifdef __HERO_CFG_VMMEM_CUSTOM__
#define __HERO_CFG_VAR_VMMEM_MAX_SIZE__ (2.5*1024*1024)
#endif

#define __HR_CFG_VAR_HERO_SPR_VERSION__		(0x12C1305)//支持12B,12C1234,12C1304
#define HERO_SPR_VERSION		                 __HR_CFG_VAR_HERO_SPR_VERSION__        

#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
#define __HERO_ENGINE_OVERSEA__ 
#endif
   

//#ifndef __HERO_ENGINE_OVERSEA__
#define __HERO_CFG_FEATURE_HANDSETID_EX__	// 20190430	
//#endif 


//#define HERO_USE_UDISK_FIRST //u盘优先启动


#ifndef TOUCHPANEL_TYPE_NONE 
#define HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
#endif


#define __HR_SUPPORT_BACKSTAGE__ //挂后台功能默认关闭

#define __HR_PHONE_FREE_SPACE__ (20*1024) //当手机盘空间大于这个值,应用当有T卡使用,填0该功能关闭 KB

#ifdef MMI_WIFI_SUPPORT
#define __HERO_APP_WIFI_SUPPORT__    
#endif
//#define __HERO_APP_MOTION_SUPPORT__   
#define __HR_CFG_LOCALAPP_MANAGER__  
#define __HR_RECORD_SUPPORT__        
//#define __HERO_CFG_CAMERA_SUPPORT__    
#ifdef CAP_TP_SUPPORT
#define __HR_CFG_MULTITP_SUPPORT__   
#endif
#ifdef DYNAMIC_MAINMENU_SUPPORT
//#define __HR_CFG_SHORTCUT_SUPPORT__  
#endif
//#define __HR_SMS_SET_STATE_SUPPORT__	
//#define __HR_SEND_MMS_SUPPORT__			

#define __USE_SCI_GETHOSTBYMANE__ //遇到平台无法联网时请换成sci_gethostbyname

#define MMI_AUD_VOLUME_MAX 					MMISET_VOL_MAX

#define MSG_RING_TYPE_VIBRATE 				1//信息提示－震动
#define VOLTAGE_WARN_TYPE_NOTRING 			1//低压警告－关
#define FLIP_RING_TYPE_NOTRING 				0//翻盖音－关
#define CALL_RING_TYPE_VIBRATE 				1//来电提示－震动
#define ALARM_RING_TYPE_VIBRATE 			    1//闹钟提示－震动


#define HERO_ENGINE_FONT_SIZE_SMALL 				MMI_DEFAULT_BIG_FONT//MMI_DEFAULT_SMALL_FONT
#define HERO_ENGINE_FONT_SIZE_MID 				MMI_DEFAULT_BIG_FONT//MMI_DEFAULT_NORMAL_FONT
#define HERO_ENGINE_FONT_SIZE_BIG 				MMI_DEFAULT_BIG_FONT

#define HERO_ENGINE_FONT_UI_SIZE 					MMI_DEFAULT_TEXT_FONT


#define HERO_ENGINE_ROOT_DIR		"\x68\x00\x65\x00\x72\x00\x6f\x00\x45\x00\x6e\x00\x67\x00\x69\x00\x6e\x00\x65\x00\x53\x00\x70\x00\x72\x00\x00\x00" //"\x6d\x00\x79\x00\x74\x00\x68\x00\x72\x00\x6f\x00\x61\x00\x64\x00\x5f\x00\x73\x00\x70\x00\x72\x00\x00\x00"//mythroad_spr

//#define HERO_ENGINE_NES_DIR_NAME			"\x00\x67\x00\x61\x00\x6d\x00\x65\x00\x73\x00\x00"//"games"		
#define HERO_ENGINE_NES_DIR_NAME			"games"		

#define HERO_SLIM_OPEN


#define HERO_ENGINE_MANUFACTORY 			"tzdt217"  
#define HERO_ENGINE_MANUFACTORY_EX 		"20231109"  
// 手机盘空间设置(单位:字节)
#define __HR_PHONE_DRV_FREE_SIZE__  500*1024

#if defined(HERO_ENGINE_SUPPORT_CLIENT_INFO) && !defined(WIN32)
#define GET_STR(x) #x
#define GET_FIRST_SEC(x) GET_STR(x)
#define _GET_FIRST_SEC(x,y) GET_FIRST_SEC(x##y)
#define __GET_FIRST_SEC(x,y)   _GET_FIRST_SEC(x,y)
#endif

/*集成商*/
#if defined(WIN32)
#define  HERO_ENGINE_HANDSET_FIRST  ""
#else
#if defined(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST)
#else
#error "please call fae!!!please define HERO_ENGINE_HANDSET_FIRST,it define customer id!"
//#define  HERO_ENGINE_HANDSET_FIRST                         "701"//最多3个字符		
#endif
#endif

/*机型标识，最多七个小写字符*/
#if defined(WIN32)
#define HERO_ENGINE_HANDSET_SEC      ""
#else
#if defined(HERO_ENGINE_CFG_VAR_HANDSETID_SEC)
#else
#error "please define HERO_ENGINE_HANDSET_SEC,it define phone project id!"
#if 0
#if(HR_SCREEN_WIDTH == 128 && HR_SCREEN_HEIGHT == 160)
#define HERO_ENGINE_HANDSET_SEC                    "1216"  	          
#elif(HR_SCREEN_WIDTH == 160 && HR_SCREEN_HEIGHT == 128)
#define HERO_ENGINE_HANDSET_SEC                    "1612"  	 
#elif(HR_SCREEN_WIDTH == 176 && HR_SCREEN_HEIGHT == 220)
#define HERO_ENGINE_HANDSET_SEC                    "1722"  	 
#elif(HR_SCREEN_WIDTH == 220 && HR_SCREEN_HEIGHT == 176)
#define HERO_ENGINE_HANDSET_SEC                    "2217"  	 
#elif(HR_SCREEN_WIDTH == 240 && HR_SCREEN_HEIGHT == 320)
#define HERO_ENGINE_HANDSET_SEC                    "2432"  	 
#elif(HR_SCREEN_WIDTH == 320 && HR_SCREEN_HEIGHT == 240)
#define HERO_ENGINE_HANDSET_SEC                    "3224"  	 
#elif(HR_SCREEN_WIDTH == 240 && HR_SCREEN_HEIGHT == 400)
#define HERO_ENGINE_HANDSET_SEC                    "2440"  	 
#elif(HR_SCREEN_WIDTH == 320 && HR_SCREEN_HEIGHT == 480)
#define HERO_ENGINE_HANDSET_SEC                    "3248"  	 
#else
#error "not define handset please add!!" 
#endif
#endif
#endif
#endif

#if defined(WIN32)
#define HERO_ENGINE_HANDSET ""
#else
#if defined(HERO_ENGINE_SUPPORT_CLIENT_INFO)
#if defined(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST)&&defined(HERO_ENGINE_CFG_VAR_HANDSETID_SEC)
#define HERO_ENGINE_HANDSET   __GET_FIRST_SEC(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST,HERO_ENGINE_CFG_VAR_HANDSETID_SEC)
#else
#error "please define HERO_ENGINE_CFG_VAR_HANDSETID_FIRST and HERO_ENGINE_CFG_VAR_HANDSETID_SEC in main .mak"
#endif
#else 
#define HERO_ENGINE_HANDSET   HERO_ENGINE_HANDSET_FIRST HERO_ENGINE_HANDSET_SEC
#endif
#endif

#define __HR_SMART_AUTH_ID__            HERO_ENGINE_MANUFACTORY"_"HERO_ENGINE_HANDSET"_smart_id_w217_watch"

#define __HERO_CFG_VAR_HANDSETID_EX__					"handset1" 
#define __HERO_CFG_VAR_HANDSETID_EX_LENGHT_MAX__	    34 


#define HERO_VIDEOPLAYER  "Hero videoplayer"



typedef enum
 {
	HERO_APP_CHBROWSER  = 920008,//browser
	HERO_APP_OSBROWSER  = 920009,//browser
	
	HERO_APP_NEWS  	  = 920037,//news
	HERO_APP_SUDOKU = 920016,//Sudoku
	HERO_APP_STONE = 920020,//Stone
	HERO_APP_TDODGE= 920034,//dodge

	HERO_APP_SMART = 920057, //语音助手 // modify 20181105 begin

	HERO_APP_TREADER= 920029,//READER	//HERO_APP_READER_OPEN
	HERO_APP_TIME= 920041,//TIME	//HERO_APP_TIME_OPEN
	HR_APPID_FAMILY = 920300,
	HR_APPID_FAMILY_BGRUN = 920301,
	HERO_APP_TWEATHER= 920044,//WEATHER	//HERO_APP_WEATHER_OPEN
	HERO_APP_TTS= 400002,
	HERO_APP_HMGS= 930008,//920040,
	HERO_APP_NES = 813105,
#ifdef HERO_ENGINE_FM_SUPPORT
    HERO_APP_FM = 920032,
#endif
#ifdef HERO_ENGINE_STVIDEO_SUPPORT
    HERO_APP_STVIDEO = 920309,
#endif

#if defined(HERO_APP_WSTORE_OPEN)
	HERO_APP_WSTORE = 920226,//WSTORE HERO_APP_WSTORE_OPEN
	HERO_APP_WSTORE_MANAGE = 999000,//WSTOREMANAGE HERO_APP_WSTORE_OPEN
#endif
	HR_SIMPLE_TTS_ENTRY = 999996,
	HR_TTS_MENU_ENTRY = 999998,

	HERO_ENGINE_TEST  = 0xFFFF,
	HERO_APP_MAX      = 127//MAX ID byte
 }HERO_APP_APPENTRY_E;



#ifdef __HR_CFG_LOCALAPP_MANAGER__

typedef int32 (*startHeroLapp)(uint8* param);
#endif
 

typedef BOOLEAN (*MMIHEROAPP_WAIT2DOPDPACT_FUNC)(uint32 data);

#define HERO_APP_MD_MAXNUM 1//NOTE: THIS VALUE PLS EQUAL TO E_HERO_MD_MAX
typedef enum
{
	E_HERO_MD_GPRS = 0, //GPRS
	E_HERO_MD_MAX
}e_hero_phone_mod;



/*****************************************************************************/
MMI_RESULT_E MMIHEROAPP_handlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr);


PUBLIC BOOLEAN MMIHEROAPP_waitToDeactiveMrappPdp(MMIHEROAPP_WAIT2DOPDPACT_FUNC WaitFunc, uint32 data);


PUBLIC BOOLEAN MMIHEROAPP_BLMemCb(BLOCK_MEM_ID_E block_id, BLOCK_MEM_RELATION_STATUS_E relation_info);

PUBLIC void MMIHEROAPP_getApnAccountRet(int32 report);

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
void hr_scut_start_app(void* param1, void *param2);
#endif
 
#ifdef   __cplusplus
   }
#endif

#endif
