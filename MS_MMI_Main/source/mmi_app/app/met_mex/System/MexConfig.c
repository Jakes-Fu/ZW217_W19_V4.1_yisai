/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexConfig.c
* 文件标识：
* 内容摘要： // 定义各种配置变量，供不同厂商不同系统定制
*
************************************************************************/

#include "mmi_app_met_mex_trc.h"
#if defined(MEX_TARGET_SERVER)

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#include "MMI_features.h"
#endif

#ifdef WIN32
#include "Windows.h"
#endif

#include "MexLib.h"


#if(MEX_MTK_CODE_VERSION >= 0x08B0)
#include "mmi_include.h"
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "EventsGprot.h"
#endif
#include "mtpnp_pfal_master_def.h"
#include "SSCStringHandle.h"
#include "SimDetectionDef.h"
#if (MEX_MTK_CODE_VERSION >= 0x08B0)
#include "l4c_common_enum.h"
#endif
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "mmi_msg_struct.h"
#endif
#include "mtpnp_ad_master_header.h"

#if (MEX_MTK_CODE_VERSION >= 0x0828)
#include "MTPNP_AD_common_def.h"
#include "cbm_api.h"
#endif

#ifdef MEX_PLATFORM__FOR_D450
#include "slt_gemini.h"
#endif
#include "gdi_datatype.h"


#include "PhoneSetup.h"
#include "worldclock.h"
#include "FileManagerGProt.h"
#include "IdleAppDef.h"
#include "MMI_features_camera.h"
#include "mdi_datatype.h"
#include "mdi_camera.h"
#include "CameraApp.h"
#include "ProtocolEvents.h"
#include "soc_consts.h"
#include "kal_release.h"
#include "soc_api.h"
#if (MEX_MTK_CODE_VERSION < 0x08B0)
#include "wgui_categories_inputsenum.h"
#endif
#include "Conversions.h"
#include "Mdi_audio.h"
#include "l1audio.h"
#include "med_main.h"
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "SMSApi.h"
#endif
#include "MessagesL4Def.H"
#include "MessagesMiscell.h"
#include "SmsGuiInterfaceType.h"
#include "PhoneBookTypes.h"
#if (MEX_MTK_CODE_VERSION < 0x1032)
#include "CallsDefs.h"       // Defs for call log(call history)
#include "CallsStruct.h"     // Structs for call log(call history)
#endif
#include "ems.h"
#include "mmi_msg_context.h"
#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "NVRAMProt.h"       // NV item
#endif
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "wgui_status_icons.h"
#endif
#if (MEX_MTK_CODE_VERSION < 0x1032)
#include "CallhistoryEnum.h"
#endif
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "SMSStruct.h"
#endif
#include "AlarmFrameworkProt.h"

#include "MyTone.h"
#include "AudioPlayerDef.h"
#include "AudioPlayerPlayList.h"
#include "FileManagerGProt.h"
#if (MEX_MTK_CODE_VERSION < 0x08B0)
#include "ConnectManageGProt.h"
#endif
#include "med_utility.h"
#include "msf_cfg.h"
#if (MEX_MTK_CODE_VERSION >= 0x0952)
#include "phbsrvgprot.h"
#include "PhoneBookProt.h"
#include "smssrvgprot.h"
#include "MediaPlayerPlayList.h"
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "ProfilesAppGprot.h"
#include "ProfilesSrv.h"
#include "CallLogSrvGprot.h" // for clog service
#include "Nwinfosrv.h"
#endif

#if defined(__SYNCML_SUPPORT__) && (MEX_MTK_CODE_VERSION < 0x1112)
#include "SyncMLDef.h"
#include "SyncMLGprot.h"
#include "SyncMLProt.h"
#include "SyncMLStruct.h"
#endif

#include "app_mem.h"
#include "med_smalloc.h"
#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "mmi_rp_app_metmex_def.h"
#include "mex_app_id.h"
#else
#include "Mex_Res.h"
#endif
#include "TimerEvents.h"

#if (MEX_MTK_CODE_VERSION >= 0x1018)
#include "l4c_nw_cmd.h"
#define SMS_INVALID_INDEX SMSAL_INVALID_INDEX
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1112)
#include "gui_typedef.h"
#include "FileMgrType.h"
#include "AudioPlayerPlayList.h"
#include "smsal_l4c_enum.h"
#include "MediaPlayerPlayList.h"
#include "smsal_l4c_defs.h"
#include "AudioPlayerPlayList.h"
#include "SmsGuiInterfaceProt.h"
#include "BTMMIScrGprots.h"
#include "BTMMIScr.h"
#include "FMRadioProt.h"
#include "MessagesExDcl.h"
#include "NwInfoSrvGprot.h"
#include "UcmSrvGprot.h"
#include "vapp_uc_gprot.h"
#include "CharBatSrvGprot.h"

#ifndef MUSIC_FOLDER
#define MUSIC_FOLDER L"My Music\\"
#endif

#endif
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#include "mn_api.h"
#include "mmi_filemgr.h"      
#include "Dal_time.h"
#include "mmimp3_export.h"
#include "mmipb_app.h"
#include "mmifm_internal.h"
#include "met_mex_export.h"
#include "mmidc_flow.h"
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
#include "mmialarm_export.h"
#include "mmiphone_export.h"
#include "mmipb_common.h"
#include "mmicl_export.h"
#include "mmicc_export.h "
#include "mmifmm_export.h" //for wallpaper
#include "mmisms_edit.h"
#include "mmisms_send.h"

#ifdef BROWSER_SUPPORT
#include "mmibrowser_wintable.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmipicview_export.h"
#include "mmicalc_export.h"
#include "mmicalendar_export.h"
#include "mmiidle_export.h"
#else
#include "mmiphone.h"
#include "mmipb_internal.h" 
#include "mmicl.h"
#include "mmicc.h"
#include "mmifmm.h" //for wallpaper
#endif


#include "mmisms_app.h"
#include "lcd_cfg.h"
#include "block_mem_def.h"
#include "block_mem.h"
#include "mmicl_internal.h"

#include "mmiacc_nv.h"
#include "mmiacc_text.h" //for clock text
#include "mmifm_export.h"//for FM

#include "mmiudisk_simu.h"
#include "mmiset_nv.h"
#include "mmiebook_file.h"
#include "mmibt_app.h"//for BT
#include "mmi_id.h"
#include "sig_code.h "
#include "window_parse.h"

#include "met_mex_id.h"
#include "mex_app_id.h"
#include "met_mex_text.h"
#include "met_mex_image.h"
#include "met_mex_osbridge.h"
#include "wav_adp.h"
#ifdef BROWSER_SUPPORT_NETFRONT
#include "Mmiwww_internal.h"
#endif
#ifdef WIN32
#ifndef  _SOCKET_TYPES_H_
#include "socket_types.h"
#endif
#endif
#define module_type  int32

#define  SS_NOFDREF           0x0001   // no file table ref any more 
#define  SS_ISCONNECTED       0x0002   // socket connected to a peer 
#define  SS_ISCONNECTING      0x0004   // in process of connecting to peer 
#define  SS_ISDISCONNECTING   0x0008   //  in process  of disconnecting 
#define  SS_CANTSENDMORE      0x0010   // can't send more data to peer 
#define  SS_CANTRCVMORE       0x0020   //can't receive more data from peer 
#define  SS_RCVATMARK         0x0040   // at mark on input 
#define  SS_PRIV              0x0080   // privileged for broadcast, raw... 
#define  SS_NBIO              0x0100   // non-blocking ops 
#define  SS_ASYNC             0x0200   // async i/o notify 
#define  SS_UPCALLED          0x0400   // zerocopy data has been upcalled (for select) 
#define  SS_INUPCALL          0x0800   // inside zerocopy upcall (reentry guard) 
#define  SS_UPCFIN            0x1000   // inside zerocopy upcall (reentry guard) 
#define  SS_WASCONNECTING     0x2000   // SS_ISCONNECTING w/possible pending error 

#endif

#if defined(MEX_PLATFORM_SPREAD)
#include "app_tcp_if.h"
#endif

#include "MexLib_Os.h"
#include "MexLib_Std.h"
#include "MexLib_Phs.h"
#include "MexLib_Soc.h"
#include "MexLib_Gui.h"
#include "MexLib_Mdi.h"
#include "MexInterface.h"

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#include "MexConfig.h"
#include "mmi_filemgr.h"  
#include "mmiudisk_export.h" 
extern int32 MexConfig_ConvertSprdDef(E_SYS2MEX_Def_Convert param);
extern bool Mex_IsMexQQMessageRunning(void);

extern bool g_mex_bGotoMainMenu;//是否要到main menu画面

//记录当前网络的状态。
extern PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];

uint16   g_uLocalCity = 0;//城市索引
wchar *g_pCurCityName = SCI_NULL;//保存当前城市名称
//世界城市
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)

LOCAL const uint32  s_mex_world_city_arr[] =
{ TXT_ACC_WORLD_CLOCK1, TXT_ACC_WORLD_CLOCK2,
TXT_ACC_WORLD_CLOCK3, TXT_ACC_WORLD_CLOCK4,
TXT_ACC_WORLD_CLOCK5, TXT_ACC_WORLD_CLOCK6,
TXT_ACC_WORLD_CLOCK7, TXT_ACC_WORLD_CLOCK8,
TXT_ACC_WORLD_CLOCK9, TXT_ACC_WORLD_CLOCK10,
TXT_ACC_WORLD_CLOCK11,TXT_ACC_WORLD_CLOCK12,
TXT_ACC_WORLD_CLOCK13,TXT_ACC_WORLD_CLOCK14,
TXT_ACC_WORLD_CLOCK15,TXT_ACC_WORLD_CLOCK16,
TXT_ACC_WORLD_CLOCK17,TXT_ACC_WORLD_CLOCK18,
TXT_ACC_WORLD_CLOCK19,TXT_ACC_WORLD_CLOCK20,
TXT_ACC_WORLD_CLOCK21,TXT_ACC_WORLD_CLOCK22,
TXT_ACC_WORLD_CLOCK23,TXT_ACC_WORLD_CLOCK24
};

#elif (MEX_SPRD_CODE_VERSION == 0x09A1025)
#ifndef WORLD_CLOCK_SUPPORT
LOCAL const uint32  s_mex_world_city_arr[] ={0};
#else
LOCAL const uint32  s_mex_world_city_arr[] =
{ 
	TXT_ACC_WORLD_GMT_0_LISBON, TXT_ACC_WORLD_GMT_1_P_MADRID,
	TXT_ACC_WORLD_GMT_2_P_CAPE_TOWN, TXT_ACC_WORLD_GMT_3_P_MOSCOW,
	TXT_ACC_WORLD_GMT_3P5_P_TEHRAN, TXT_ACC_WORLD_GMT_4_P_ABUDHABI,
	TXT_ACC_WORLD_GMT_4P5_P_KABUL, TXT_ACC_WORLD_GMT_5_P_ISLAMABAD,
	TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI, TXT_ACC_WORLD_GMT_5P75_P_KATHMANDU,
	TXT_ACC_WORLD_GMT_6_P_DHAKA,TXT_ACC_WORLD_GMT_6P5_P_YANGON,
	TXT_ACC_WORLD_GMT_7_P_BANGKOK,TXT_ACC_WORLD_GMT_8_P_BEIJING,
	TXT_ACC_WORLD_GMT_9_P_TOKYO,TXT_ACC_WORLD_GMT_9P5_P_MELBOURNE,
	TXT_ACC_WORLD_GMT_10_P_VLADIVOSTOK,TXT_ACC_WORLD_GMT_10P5_P_BRISBANE,
	TXT_ACC_WORLD_GMT_11_P_SOLOMON,TXT_ACC_WORLD_GMT_11P5_P_NORFOLD_ISLAND,
	TXT_ACC_WORLD_GMT_12_P_AUCKLAND,TXT_ACC_WORLD_GMT_12P75_P_TONGA,
	TXT_ACC_WORLD_GMT_12_N_KWAJALEIN,TXT_ACC_WORLD_GMT_11_N_APIA,
	TXT_ACC_WORLD_GMT_10_N_HONOLULU,TXT_ACC_WORLD_GMT_9P5_N_POLYNESIA,
	TXT_ACC_WORLD_GMT_9_N_ANCHORAGE,TXT_ACC_WORLD_GMT_8P5_N_EASTER_ISLAND,
	TXT_ACC_WORLD_GMT_8_N_VANCOUVER,TXT_ACC_WORLD_GMT_7_N_PHOENIX,	
	TXT_COMMON_WORLD_MEXICO,TXT_ACC_WORLD_GMT_5_N_BOGOTA,
	TXT_ACC_WORLD_GMT_4_N_SANTIAGO,TXT_ACC_WORLD_GMT_3P5_N_LAPAZ,
	TXT_ACC_WORLD_GMT_3_N_BUENOS_AIRES,TXT_ACC_WORLD_GMT_2_N_CENTRAL_ATLANTIC,
	TXT_ACC_WORLD_GMT_1_N_PRAIA
};
#endif

#elif (MEX_SPRD_CODE_VERSION > 0x09A1025)
#if !defined(WORLD_CLOCK_SUPPORT) || (MEX_SPRD_CODE_VERSION == 0x10A6530)
LOCAL const uint32  s_mex_world_city_arr[] ={0};
#else
LOCAL const uint32  s_mex_world_city_arr[] =
{ 
TXT_ACC_WORLD_GMT_0_LISBON, TXT_ACC_WORLD_GMT_1_P_MADRID,
TXT_ACC_WORLD_GMT_2_P_CAPE_TOWN, TXT_ACC_WORLD_GMT_3_P_MOSCOW,
TXT_ACC_WORLD_GMT_3P5_P_TEHRAN, TXT_ACC_WORLD_GMT_4_P_ABUDHABI,
TXT_ACC_WORLD_GMT_4P5_P_KABUL, TXT_ACC_WORLD_GMT_5_P_ISLAMABAD,
TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI, TXT_ACC_WORLD_GMT_5P75_P_KATHMANDU,
TXT_ACC_WORLD_GMT_6_P_DHAKA,TXT_ACC_WORLD_GMT_6P5_P_YANGON,
TXT_ACC_WORLD_GMT_7_P_BANGKOK,TXT_ACC_WORLD_GMT_8_P_BEIJING,
TXT_ACC_WORLD_GMT_9_P_TOKYO,TXT_ACC_WORLD_GMT_9P5_P_MELBOURNE,
TXT_ACC_WORLD_GMT_10_P_VLADIVOSTOK,TXT_ACC_WORLD_GMT_10P5_P_BRISBANE,
TXT_ACC_WORLD_GMT_11_P_SOLOMON,TXT_ACC_WORLD_GMT_11P5_P_NORFOLD_ISLAND,
TXT_ACC_WORLD_GMT_12_P_AUCKLAND,TXT_ACC_WORLD_GMT_12P75_P_TONGA,
TXT_ACC_WORLD_GMT_12_N_KWAJALEIN,TXT_ACC_WORLD_GMT_11_N_APIA,
TXT_ACC_WORLD_GMT_10_N_HONOLULU,TXT_ACC_WORLD_GMT_9P5_N_POLYNESIA,
TXT_ACC_WORLD_GMT_9_N_ANCHORAGE,TXT_ACC_WORLD_GMT_8P5_N_EASTER_ISLAND,
TXT_ACC_WORLD_GMT_8_N_VANCOUVER,TXT_ACC_WORLD_GMT_7_N_PHOENIX,	
TXT_COMMON_WORLD_MEXICO,TXT_ACC_WORLD_GMT_5_N_BOGOTA,
TXT_ACC_WORLD_GMT_4_N_SANTIAGO,TXT_ACC_WORLD_GMT_3P5_N_LAPAZ,
TXT_ACC_WORLD_GMT_3_N_BUENOS_AIRES,TXT_ACC_WORLD_GMT_2_N_CENTRAL_ATLANTIC,
TXT_ACC_WORLD_GMT_1_N_PRAIA
};
#endif
#endif

#if defined MEX_PLATFORM_SIMULATOR
#include "tx_api_win32.h"
#elif defined MEX_PLATFORM_SPREAD
#include "tx_api_thumb.h"
#endif
#endif

#if 0
#if defined(MEX_PLATFORM_SIMULATOR)
/* Berkeley style "Socket address" */
struct sci_sockaddr
{
	unsigned short    family;     /* address family */
	unsigned short    port;		 /* port number */
	unsigned long     ip_addr;    /* ip address */  
	char     sa_data[8];			 /* up to 14 bytes of direct address */
};
#endif
#endif
//------------fro SOC--------------------------


//----------end for SOC------------------------





#if (MEX_MTK_CODE_VERSION >= 0x0828 && MEX_MTK_CODE_VERSION < 0x1032)
#include "mtpnp_pfal_master_callhistory.h"
#endif /* MEX_PLATFORM__FOR_MTK23C */

//修改时间:2011-03-24, 冯旭超
#if (MEX_BUILD_FINAL_USER)
#define	MetVenderID		7				//客户版本的渠道号
#elif (MEX_BUILD_SDK_VERSION)
#define	MetVenderID		9				//SDK版本的渠道号
#elif (defined(MEX_PLATFORM_SIMULATOR))
#define	MetVenderID		9				//展讯模拟器版本的渠道号
#elif (defined(MEX_PLATFORM_SPREAD))
#define	MetVenderID		9				//展讯真机测试版本的渠道号 
#elif (defined(MEX_PLATFORM_MODIS))
#define	MetVenderID		9				//MTK模拟器版本的渠道号
#elif (defined(MEX_PLATFORM_MTK))
#define	MetVenderID		9				//MTK真机测试版本的渠道号 
#endif

#define	MetPhoneUA		"100"			//机型				
#define	MetMachineID    0			   //机器编号
/*请把这里定义为 手机菜单中 "网络服务"－>"GPRS"的菜单数目 通常默认为10 */
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#if (MEX_MTK_CODE_VERSION < 0x1032)
    #define MEX_GPRS_ACCOUNT_NO			(custom_get_gprs_profile_num())
    #define MEX_GSM_ACCOUNT_NO			(custom_get_csd_profile_num())	//一般前10个账号都会被GSM占用
	#else
		#define MEX_GPRS_ACCOUNT_NO			MAX_GPRS_PROFILE_NUM
		#define MEX_GSM_ACCOUNT_NO			CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM
	#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_GPRS_ACCOUNT_NO			10
#define MEX_GSM_ACCOUNT_NO			10	//一般前10个账号都会被GSM占用
#endif

/*移植过程中 需要检查一下联网账号 是否与其他第三方冲突，最好不要冲突*/
#define MEX_MASTER_NWK_ID			(MEX_GSM_ACCOUNT_NO + MEX_GPRS_ACCOUNT_NO - 2)	//联网账号设置为 网络服务"－>"GPRS" 的菜单数据账号的倒数第二位
#define MEX_SLAVE_NWK_ID			(MEX_GSM_ACCOUNT_NO + MEX_GPRS_ACCOUNT_NO - 3)	//联网账号设置为 网络服务"－>"GPRS" 的菜单数据账号的倒数第三位

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1112)
#define APPMEM_MEX_POOL_SIZE 0
#endif
#define MEX_MEM_INTERNAL_SIZE_APP	APPMEM_MEX_POOL_SIZE		//APP内存 至少要120K 	最好是180K+ 根据不同项目而定
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_MEM_INTERNAL_SIZE_APP	BLOCK_MEM_POOL_MET_MEX_MEM_INTERNAL_SIZE		//APP内存 至少要120K 	最好是180K+ 根据不同项目而定
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(MEX_PLATFORM_FOR_MTK6235) || defined(MT6235) || defined(MT6235B)
#define MEX_MEM_INTERNAL_SIZE_MED	(200 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(300 * 1024)
#define MEX_MEM_RESERVED_SIZE		(800 * 1024)
#elif defined(MT6236) || defined(MT6236B)
#define MEX_MEM_INTERNAL_SIZE_MED	(200 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(500 * 1024)
#define MEX_MEM_RESERVED_SIZE		(1024 * 1024)
#elif defined(MT6252)
#define MEX_MEM_INTERNAL_SIZE_MED	(180 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(170 * 1024)
#define MEX_MEM_RESERVED_SIZE		(480 * 1024)
#elif (MEX_MTK_CODE_VERSION >= 0x1032)
#define MEX_MEM_INTERNAL_SIZE_MED	(200 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(300 * 1024)
#define MEX_MEM_RESERVED_SIZE		(800 * 1024)
#elif defined(MEX_PLATFORM__FOR_D600)
#if (MEX_MEM_INTERNAL_SIZE_APP < (100 * 1024))
#error "MEX_MEM_INTERNAL_SIZE_APP is too small!!!"
#endif
#define MEX_MEM_INTERNAL_SIZE_MED	(80 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(170 * 1024)
#define MEX_MEM_RESERVED_SIZE		(480 * 1024)
#elif defined(MEX_PLATFORM_FOR_MTK6253)
#if (MEX_MEM_INTERNAL_SIZE_APP < (100 * 1024))
#error "MEX_MEM_INTERNAL_SIZE_APP is too small!!!"
#endif
#define MEX_MEM_INTERNAL_SIZE_MED	(80 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(170 * 1024)
#define MEX_MEM_RESERVED_SIZE		(480 * 1024)
#else
#define MEX_MEM_INTERNAL_SIZE_MED	(180 * 1024)
#define MEX_MEM_EXTERNAL_SIZE		(170 * 1024)
#define MEX_MEM_RESERVED_SIZE		(480 * 1024)
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_MEM_INTERNAL_SIZE_MED	(0)
#define MEX_MEM_EXTERNAL_SIZE		BLOCK_MEM_POOL_MET_MEX_MEM_EXTERNAL_SIZE
#define MEX_MEM_RESERVED_SIZE		BLOCK_MEM_POOL_MET_MEX_MEM_RESERVED_SIZE

#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
/* 移植过程中，请根据scatter file 的DYNAMIC_CODE 修改以下宏定义 */
/*
举例: 根据mex_sys\MTK6253\Main\build\m620\scatM620.txt 进行配置

1. 打开scatM620.txt，搜索关键字DYNAMIC_CODE ，发现如下片段
MED_MAIN_REGION 0x4000CC70 OVERLAY 0x9600
{
med_main.obj (INTERNRW1, INTERNZI1)
btmt_mem.obj (INTSRAM_RW, INTSRAM_ZI)
}    
DYNAMIC_CODE12 0x4000cc70 OVERLAY 0x9600
{
* (MP4DEC2_DYNAMIC_ZI, MP4DEC2_DYNAMIC_RO)
}
DYNAMIC_CODE16 0x4000cc70 OVERLAY 0x9600
{
* (MP4ENC1_DYNAMIC_CODE, MP4ENC1_DYNAMIC_RO, MP4ENC1_DYNAMIC_ZI)
}
DYNAMIC_CODE18 0x40016270 OVERLAY 0x0d90
{
* (MP4DEC_DYNAMIC_CODE, MP4DEC_DYNAMIC_RO)
}
DYNAMIC_CODE19 0x40016270 OVERLAY 0x0d90
{
* (MP4ENC_DYNAMIC_CODE, MP4ENC_DYNAMIC_RO)
}
DYNAMIC_CODE1 0x40017000 OVERLAY 0x5000
{
* (AMR515_DYNAMIC_CODE, AMR515_DYNAMIC_ZI)
}
DYNAMIC_CODE11 0x40017000 OVERLAY 0x5000
{
* (VRENGINE_DYNAMIC_CODE, VRENGINE_DYNAMIC_ZI)
}
DYNAMIC_CODE13 0x40017000 OVERLAY 0x5000
{
* (MP4ENC2_DYNAMIC_ZI, MP4ENC2_DYNAMIC_RO)
}
DYNAMIC_CODE14 0x40017000 OVERLAY 0x5000
{
* (264DEC2_DYNAMIC_ZI, 264DEC2_DYNAMIC_RO)
}
DYNAMIC_CODE3 0x40017000 OVERLAY 0x5000
{
* (CTM_DYNAMIC_CODE, CTM_DYNAMIC_ZI)
}
DYNAMIC_CODE4 0x40017000 OVERLAY 0x5000
{
* (G729_DYNAMIC_CODE, G729_DYNAMIC_ZI)
}
DYNAMIC_CODE5 0x40017000 OVERLAY 0x5000
{
* (SBC_DYNAMIC_CODE, SBC_DYNAMIC_ZI)
}

2. 从上面的片段发现，有三个地址和长度:
MED_MAIN_REGION 0x4000CC70 OVERLAY 0x9600
DYNAMIC_CODE18 0x40016270 OVERLAY 0x0d90
DYNAMIC_CODE1 0x40017000 OVERLAY 0x5000
这三个地址有以下关系:
0x4000cc70 + 0x9600 = 0x40016270
0x40016270 + 0x0d90 = 0x40017000
3. 根据地址和长度确定配置如下:
#define MEX_DCM_ADDRESS      0x4000CC70
#define MEX_DCM_LENGTH       (0x9600 + 0x0d90 + 0x5000)
*/
#if MEX_BUILD_FINAL_USER
// 移植过程中，请根据scatter file 定义宏MEX_DCM_ADDRESS 和 MEX_DCM_LENGTH
#if (MEX_MTK_CODE_VERSION == 0x1112)
static char dummyDcmArea[10];
#define MEX_DCM_ADDRESS      (dummyDcmArea)
#define MEX_DCM_LENGTH       (sizeof(dummyDcmArea))
#endif

#else
#if defined(MEX_PLATFORM__FOR_D600)
#define MEX_DCM_ADDRESS      0xa000d000
#define MEX_DCM_LENGTH       0x5000
#elif defined(MEX_PLATFORM__FOR_MTK23C)
#define MEX_DCM_ADDRESS      0x40008C00
#define MEX_DCM_LENGTH       0x1400
#elif defined(MEX_PLATFORM_FOR_MTK6253)
	#if  defined(MEX_PLATFORM_FOR_MTK6236) || defined(MT6236) || defined(MT6236B)
		char MEX_MT6236_DCM[200*1024];
		#define MEX_DCM_ADDRESS 	(MEX_MT6236_DCM)	//(MEX_MT6236_DCM)		0x40000000   
		#define MEX_DCM_LENGTH		(sizeof(MEX_MT6236_DCM))		//(sizeof(MEX_MT6236_DCM))		0x18000
	#elif defined(MEX_PLATFORM_FOR_MTK6235) || defined(MT6235) || defined(MT6235B)
		#define MEX_DCM_ADDRESS      0x4000CC70
		#define MEX_DCM_LENGTH       (0x9600 + 0x0d90 + 0x5000)
	#else
		#define MEX_DCM_ADDRESS      0x4000CC70
		#define MEX_DCM_LENGTH       (0x9600 + 0x0d90 + 0x5000)
	#endif/*MEX_PLATFORM_FOR_MTK6236*/
#elif defined(MEX_PLATFORM_FOR_MTK6235)
#define MEX_DCM_ADDRESS 	 0x50108c00
#define MEX_DCM_LENGTH		 (0x3c00 + 0x2800 + 0x5000)
#endif
#endif
#endif


// 定义是否为双卡项目 请根据实际项目修改对应的宏
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(__MMI_DUAL_SIM_MASTER__)
#define MEX_DUAL_SIM 		
#elif defined(__SLT_DUAL_SIM_GEMINI__)
#define MEX_DUAL_SIM 	
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MMI_DUALSIM_SUPPORT) || defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MULTI_SIM_SYS_DUAL)
#define MEX_DUAL_SIM
#elif defined(MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_TRI)
#define MEX_THIRD_SIM
#elif defined(MULTI_SIM_SYS_QUAD)||defined(MMI_MULTI_SIM_SYS_QUAD)
#define MEX_QUAD_SIM
#elif defined(MULTI_SIM_SYS_SINGLE)||defined(MMI_MULTI_SIM_SYS_SINGLE)
#define MEX_SINGLE_SIM
#else
#error Please Check MMI_MULTI_SIM_SYS in project file. // 此处为防止系统宏不一致，如果出错，请对应替换默认宏
#endif

#endif



//定义mex系统文件夹 请根据具体项目以及其存储卡盘符进行修改
#if defined(MEX_PLATFORM__FOR_D450)||defined(MEX_PLATFORM__FOR_MTK23C)
#define MEX_SYSROOTPATH L"D:\\MexSys"					
#define MEX_APPROOTPATH L"D:\\MexApp"
#elif defined(MEX_PLATFORM__FOR_D600)
#define MEX_SYSROOTPATH L"E:\\MexSys"					
#define MEX_APPROOTPATH L"E:\\MexApp"
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
#define MEX_SYSROOTPATH L"E:\\MexSys"					
#define MEX_APPROOTPATH L"E:\\MexApp"
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
#define MEX_SYSROOTPATH L"E:\\MexSys"					
#define MEX_APPROOTPATH L"E:\\MexApp"
#else
#define MEX_SYSROOTPATH	L"E:\\MexSys"
#define MEX_APPROOTPATH	L"E:\\MexApp"
#endif

//运行平台需满足的最低存储卡空间	单位字节
#define MINFREESPACE			(1*1024*1024)		

//平台补丁号
#define MEX_PLATFORM_PATCH_VERSION	""

//默认网络接入方式
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_SIMULATOR) || (defined(MEX_PLATFORM_MTK) && !defined(__MMI_LANG_SM_CHINESE__))
#define MEX_DEFAULT_NW_MODE E_MNM_CMNET
#else
#define MEX_DEFAULT_NW_MODE E_MNM_CMWAP
#endif
//默认网络接入sim卡
#define MEX_DEFAULT_NW_SIM E_MEX_SIM_NONE

#if defined(MEX_PLATFORM_SIMULATOR)
#define  SO_RXDATA      0x1011      /* get count of bytes in sb_rcv */
#define  SO_TXDATA      0x1012
#define  SO_MYADDR      0x1013      /* return my IP address */
#define  SO_NBIO        0x1014      /* set socket into NON-blocking mode */
#define  SO_BIO         0x1015      /* set socket into blocking mode */
#endif
//背光暗了 是否能够文件读写
//以及屏幕由暗变亮时的预热过程延时
#if defined (MEX_PLATFORM__FOR_MTK23C)
#define MEX_CAN_OP_FILE_IN_BACKLIGHT_OFF	1
#define MEX_SCREEN_WARMUP_DELAY				10
#else
#define MEX_CAN_OP_FILE_IN_BACKLIGHT_OFF	1
#define MEX_SCREEN_WARMUP_DELAY				2
#endif
//用sound play 播放声音时为减少播放声音的延时
//而设置的系统sleeptime
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_TIME_DELAY_FOR_SOUNDPLAY	0
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_TIME_DELAY_FOR_SOUNDPLAY	130
#endif

//是否支持双缓冲优化
#if defined(MEX_PLATFORM__FOR_MTK23C)
#define	MEX_SUPPORT_BLTDBBUF 0
#else
#define	MEX_SUPPORT_BLTDBBUF 0
#endif

//是否支持摄像头
#if defined(MEX_PLATFORM__FOR_MTK23C)// || defined(MEX_PLATFORM__FOR_SPRD6600L)
#define MEX_SUPPORT_CAMERA  0
#elif defined(DUAL_CAMERA_SUPPORT)			//DUAL CAMERA
#define MEX_SUPPORT_CAMERA  2
#else 
#define MEX_SUPPORT_CAMERA  1
#endif

//摄像头是否使用双缓冲
#if defined(MEX_PLATFORM_FOR_MTK6253) || defined(MEX_PLATFORM_FOR_MTK6235) || defined(MEX_PLATFORM__FOR_SPRD6600L) || defined(MEX_PLATFORM__FOR_SPRD6800H)
#define MEX_CAMERA_DBUFFER		0
#else
#define MEX_CAMERA_DBUFFER		1

#endif

#if defined(MEX_PLATFORM__FOR_SPRD6600L) || defined(MEX_PLATFORM__FOR_SPRD6800H)

//定义摄像头的旋转角度0:不旋转1:90°2:180° 3:270°
#if defined(SENSOR_ROTATION_0) || defined(CAMERA_SENSOR_ANGLE_0) // 第二个宏需要针对情况手动调整
#define CAMERA_LCM_MAINLCD_ROTATE     0    
#elif defined(SENSOR_ROTATION_90)
#define CAMERA_LCM_MAINLCD_ROTATE     1    
#elif defined(SENSOR_ROTATION_180)
#define CAMERA_LCM_MAINLCD_ROTATE     2    
#elif defined(SENSOR_ROTATION_270) || defined(CAMERA_SENSOR_ANGLE_90)
#define CAMERA_LCM_MAINLCD_ROTATE     3    
#else
#define CAMERA_LCM_MAINLCD_ROTATE     0    
#endif

#endif

//mex 是否控制了系统播放器，如果是1，需要移植对应播放器接口函数
#ifdef MEX_PLATFORM_MODIS
//模拟器上面如果是第三方的，可能没有模拟器版本
#if defined(__MMI_AUDIO_PLAYER__) && (MEX_MTK_CODE_VERSION < 0x1112)
#define MEX_AUDIO_PLAYER_UNDER_CONTROL 		1
#else
#define MEX_AUDIO_PLAYER_UNDER_CONTROL 		0
#endif
#else
//真机请修改此处
#if (MEX_MTK_CODE_VERSION < 0x1112)
#define MEX_AUDIO_PLAYER_UNDER_CONTROL 		1
#else
#define MEX_AUDIO_PLAYER_UNDER_CONTROL 		0
#endif
#endif

//启动平台时是否关闭背景音乐
//如果发现平台运行时，背景音乐切换歌曲有问题，请设置成1,
//同时需要把上面MEX_AUDIO_PLAYER_UNDER_CONTROL 打开
#define MEX_PAUSE_BGMUSIC_ON_LAUNCH		1

//系统允许同时建立的socket数
#define MEX_MAX_SOCKET_NUM	(6)

//是否支持smalloc： 
//	0: 不支持，1: med_int_smalloc(size)，
//	2: med_int_smalloc(size, file, line)，
//	3: 平台自定义的快速内存管理
//	4: 展讯定义
//	5: Kodak 36_10A_M580 中创建
//以下为移植前代码，移植时请尝试设置成 1 或者 2，测试均不通过才设置成 0
#if (MEX_MTK_CODE_VERSION >= 0x1112)
#define MEX_ENABLE_SMALLOC		0
#elif (MEX_MTK_CODE_VERSION >= 0x0936)
#if defined(MT6235) || defined(MT6235B)
#define MEX_ENABLE_SMALLOC		2
#else
#define MEX_ENABLE_SMALLOC		3
#endif
#elif defined(MEX_PLATFORM__FOR_D600)
#define MEX_ENABLE_SMALLOC		1
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
#define MEX_ENABLE_SMALLOC		0
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
#define MEX_ENABLE_SMALLOC		0
#else
#define MEX_ENABLE_SMALLOC		2
#endif

//是否支持tmalloc： 0：不支持，非0：支持；
//先尝试1，如果 RTPCodec 没有开启会编译失败，此时只能设置成 0
#define MEX_ENABLE_TMALLOC		0

//是否开机自动申请内部内存
#define MEX_ALLOC_KERNAL_MEM_FOLLOW_MMISTARTUP	0		

//退出平台是否释放内部内存
#define MEX_ENABLE_FREE_KERNAL_MEM		1

//如果是开机就申请的内存 退出平台是没有必要释放的

#if (MEX_ALLOC_KERNAL_MEM_FOLLOW_MMISTARTUP&&MEX_ENABLE_FREE_KERNAL_MEM)
#error "MEX_ENABLE_FREE_KERNAL_MEM define error!!!"
#endif

#ifdef MEX_ST_SUPPORT
#define MEX_ENABLE_STATISTIC	(1)
#else
#define MEX_ENABLE_STATISTIC	(0)
#endif


//是否支持直接压缩jpg到内存里
#if defined(MEX_PLATFORM_FOR_MTK6253)||defined(MEX_PLATFORM_FOR_MTK6235)
#define MEX_ENABLE_ENCODEJPGTOMEM	(1)
#else
#define MEX_ENABLE_ENCODEJPGTOMEM	(0)
#endif

//是否开启预处理功能

#define MEX_ENABLE_PRE_START_UP		(1)



//是否将mex相关文件隐藏
#define MEX_HIDE_MEXFILES	(0)	

//是否开启文件读取缓存
#define MEX_HAS_FILE_READ_CACHE	(1)	

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//是否在启动mex时停止正在播放的fm
#define MEX_PAUSE_BGFM_ON_LAUNCH	(1)

#if (defined(MEX_PLATFORM_FOR_MTK6253))&&(MEX_BUILD_FINAL_USER == 0)
#define MEX_SUPPORT_MASSDATE_IN_INTRAM	 (0)
#else
#define MEX_SUPPORT_MASSDATE_IN_INTRAM	 (0)
#endif

//是否可选联网账号
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
#define MEX_ENABLE_SELECT_DATAACCOUNT	(1) 
#else
#define MEX_ENABLE_SELECT_DATAACCOUNT	(0) 
#endif

#endif

// Mex Mdi 是否支持循环播放mid文件
#define MEX_MDI_SUPPORT_MID_PLAY_FILE		(1)
// Mex Mdi 是否支持循环播放Wav文件
#define MEX_MDI_SUPPORT_WAV_PLAY_FILE		(1)
// Mex Mdi 是否支持循环播放Mp3文件
#define MEX_MDI_SUPPORT_MP3_PLAY_FILE		(1)



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 注意：如果不是独立QQ版本，请修改本文件后面 E_MESA_MEX_SPECIAL_ENTRY 处的代码！！
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//////////////////////////////////////////////////////////////////////////////////////
#define MEX_MAX_SIGNAL_STRENGTH 100
int32 g_iMasterSimSignal = 0;

#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
int32 g_iSlaveSimSignal = 0;	
#endif

#ifdef MEX_QA_MODE
int32 iMex_QA_VenderID = MetVenderID;
char pMex_QA_PhoneUA[10] = {0};
extern int32 Mex_Test_CheckCurrAppId(char *appInitParam );
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//充电是否完成
bool g_bCharingComplete = false;
#endif

//update的最小时间间隔 单位MS
int32 iUpdateDelay = 1;     /*lint !e552*/

//保存平台注册网络账号的timeout
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                    
int g_mex_init_net_timeout = 10000;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
int g_mex_init_net_timeout = 20000;
#endif


//-------for SRAM-------------
#if (MEX_ENABLE_SMALLOC == 3 || MEX_ENABLE_SMALLOC == 5)
//是否支持备份DCM： 0: 不支持，1: 备份到C盘，2: 备份到多媒体内存
#define MEX_ENABLE_BACKUP_DCM 1
#define MEX_BACKUP_DCM_FILE L"C:\\MexBackupDcm"

typedef struct
{
	void   *m_iPoolId;
	uint32  m_iAllocCount;
	uint32  m_iLeftSize;
	void   *m_pSramAddr;
	uint32  m_iSramLen;
} Mex_Sram_Context;

static Mex_Sram_Context g_mex_sram = { 0 };
#endif // (MEX_ENABLE_SMALLOC == 3)


#if (MEX_ENABLE_SMALLOC == 4)
#include "iram_mgr.h"
//IRAM可以使用的基地址 和长度
#if defined PLATFORM_SC6600R
#define  MEX_IRAM_BASE_ADDR ((uint32)0x40004000) 
#define  MEX_IRAM_END_ADDR  ((uint32)0x40013800)
#elif defined PLATFORM_SC6600L
#define  MEX_IRAM_BASE_ADDR ((uint32)0x40001028) //该值可能会改变，基地址本来是0x4000000,但是可以使用的不一定为0x4000000
#define  MEX_IRAM_END_ADDR  ((uint32)0x40001B00)
#endif

#define  MEX_IRAM_LOCK_CHECK_BLOCK  16    //锁的时候,探索的最小块大小

//快速内存池结构
typedef struct 
{
	int32 m_iMemId;  //pool handle
	uint8 *m_pMemAddr; //地址
	int32 m_iAllocCount; //被分配的次数
	int32 m_iMemSize;  //剩余的内存
#if defined(MEX_PLATFORM_SPREAD) 
	ILOCK_HANDLE lock; //真机需要一鰈ock
#endif
} Mex_Sram_Mem_Pool;

Mex_Sram_Mem_Pool *g_Mex_Sram_Mem_Pool = NULL;
#endif


#if(MEX_ENABLE_SMALLOC == 5)
typedef enum
{
    MEX_MM_INTMEM_SEG__13B_0 = 0 
  , MEX_MM_INTMEM_SEG__13B_1
  , MEX_MM_INTMEM_SEG__13b_2
  , MEX_MM_INTMEM_SEG__13B_3
  , MEX_MM_INTMEM_SEG__13B_4
  , MEX_MM_INTMEM_SEG__13B_5
  , MEX_MM_INTMEM_SEG__13B_6         /*1300*7=9100*/
  , MEX_MM_INTMEM_SEG__13K_0         /*13*1024=13312*/
  , MEX_MM_INTMEM_SEG__33K_0          /*33*1024=33792*/
  , MEX_MM_INTMEM_SEG____COUNT     /*9100+13312+33792=56204*/
} mex_mm_intmem_segment_id_t;

typedef struct
{
  mex_mm_intmem_segment_id_t  segment_id;
  kal_uint32              start_address;
  kal_uint32              size_in_bytes;
  kal_uint8                allocated;
} mex_mm_intmem_segment_entry_t;

#define MEX_MM_SIZE_0 0x514
#define MEX_MM_SIZE_1 0x3400
#define MEX_MM_SIZE_2 0x8400

static mex_mm_intmem_segment_entry_t mex_mm_seg_tbl[MEX_MM_INTMEM_SEG____COUNT] = 
{
    {MEX_MM_INTMEM_SEG__13B_0,   MEX_DCM_ADDRESS,   							MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13B_1,   MEX_DCM_ADDRESS+MEX_MM_SIZE_0,  				MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13b_2,   MEX_DCM_ADDRESS+2*MEX_MM_SIZE_0,   			MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13B_3,   MEX_DCM_ADDRESS+3*MEX_MM_SIZE_0,   			MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13B_4,   MEX_DCM_ADDRESS+4*MEX_MM_SIZE_0, 				MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13B_5,   MEX_DCM_ADDRESS+5*MEX_MM_SIZE_0, 				MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13B_6,   MEX_DCM_ADDRESS+6*MEX_MM_SIZE_0, 				MEX_MM_SIZE_0,   KAL_FALSE} // 1300byte
  , {MEX_MM_INTMEM_SEG__13K_0,   MEX_DCM_ADDRESS+7*MEX_MM_SIZE_0, 				MEX_MM_SIZE_1,   KAL_FALSE} // 13kbyte
  , {MEX_MM_INTMEM_SEG__33K_0,   MEX_DCM_ADDRESS+7*MEX_MM_SIZE_0+MEX_MM_SIZE_1, MEX_MM_SIZE_2,   KAL_FALSE} // 33kbyte
};
#endif

//------------end for SRAM----------------------


#define	MEX_SHOW_LOADING_PAGE	(0)			//是否需要显示loading界面

bool g_bNeedShowLoadingPage = false;


//加载引导界面
#if MEX_SHOW_LOADING_PAGE
Mex_Config_Loading_Page g_mex_tiny_loading_page = 
{
	true,
	MEX_COLOR_BLACK,
	MEX_COLOR_WHITE,
	MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER, 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)||defined(MEX_PLATFORM_SIMULATOR)
	(const wchar_t *)"\x1D\x52\xCB\x59\x16\x53\x2E\x0\x2E\x0\x2E\x0\x20\x0\x2E\x0\x2E\x0\x2E\x0\0" 		//初始化... ...
#elif defined(MEX_PLATFORM_SPREAD)
	(const wchar_t *)"\x52\x1D\x59\xCB\x53\x16\x0\x2E\x0\x2E\x0\x2E\x0\x20\x0\x2E\x0\x2E\x0\x2E\0"
#endif


};


Mex_Config_Loading_Page g_mex_man_loading_page = 
{ 
	true,
	MEX_COLOR_BLACK, 
	MEX_COLOR_WHITE,
	MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER, 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)||defined(MEX_PLATFORM_SIMULATOR)
	(const wchar_t *)"\xF7\x8B\xD\x7A\x19\x50\x2E\x0\x2E\x0\x2E\x0\x20\x0\x2E\x0\x2E\x0\x2E\x0\0" 		//请稍候... ...
#elif defined(MEX_PLATFORM_SPREAD)
	(const wchar_t *)"\x8B\xF7\x7A\xD\x50\x19\x0\x2E\x0\x2E\x0\x2E\x0\x20\x0\x2E\x0\x2E\x0\x2E\0"
#endif
};
#endif


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)

#if (MEX_SDK_VER >= 1018)
int8 g_mex_simcard_map[E_MEX_SIM_MAX];
#endif

#endif

#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
extern MMISMS_GLOBAL_T g_mmisms_global;      // SMS的全局变量
#endif

#endif

/************************************************************************/
/* 引用到外部的函数                                                     */
/************************************************************************/


extern int8* MexPhs_Get_Imei(void);
extern int8* MexPhs_Get_Master_Imsi(void);
extern int8* MexPhs_Get_Slave_Imsi(void);
extern int8* MexPhs_Get_Master_SmsCent(void);
extern int8* MexPhs_Get_Slave_SmsCent(void);
extern Mex_Cell_Info* MexPhs_Get_Master_CellInfo(void);
extern Mex_Cell_Info* MexPhs_Get_Slave_CellInfo(void);
extern E_MEX_Net_Service_ID MexPhs_GetNetworkID(E_MEX_Sim_Card_ID sim_id);
extern E_MEX_Phs_Language MexPhs_Get_Language(void);
extern char* MexSystem_GetSystemVersion(void);
extern int8* MexPhs_Get_Third_Imsi(void);  /*lint !e752 */
extern int8* MexPhs_Get_Third_SmsCent(void); /*lint !e752 */
extern Mex_Cell_Info* MexPhs_Get_Third_CellInfo(void); /*lint !e752 */
extern void mex_Phs_SmsSendCallBack(E_MEX_PHS_SMS_RESULT res);
extern void MexSystem_Shutdown( E_Mex_PlatformShutdown eShutdownMode);	
extern void Mex_GetCurrentEntryAppInfo(char *pAppName, uint32 iAppNameLen, int32 *pAppID);
extern void Mex_Exit_System( bool bExitAll );

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
extern uint8 GetRingVolumeLevel(void);

extern MMI_BOOL IsBacklightOn(void);

extern void MexPhs_UpdatePhoneConfigs(void);

#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
/*获取当前被选中或者播放歌曲的名字*/
extern UI_character_type * Mex_Func_get_audply_title(void);
/*获取当前歌曲已播放时间 单位ms*/
extern U32 Mex_Func_get_audply_played_ms(void);
/*获取当前歌曲播放的总时间*/
extern U32 Mex_Func_get_audply_total_ms(void);
/*获取当前歌曲在播放列表中的索引号即顺序号*/
extern U32 Mex_Func_get_audply_played_index(void);
/*获取当前列表中歌曲总数*/
extern U32 Mex_Func_get_audply_total_num(void);
/*获取音量*/
extern uint8 Mex_Func_get_audply_volume(void);
/*获取最大音量*/
extern uint8 Mex_Func_get_audply_max_volume(void);
/*初始化播放列表*/
extern void Mex_Init_AudPly(void);
extern uint16 GetActiveScreenId(void);
#endif
#endif


int32 MexConfig_InitPhoneConfigs()
{
	return 0;
}

void MexConfig_UpdatePhoneConfigs(void)
{

}
#if defined(MEX_PLATFORM_MODIS)
uint32 MexConfig_SwitchStackToRun_Dummy(void *stack_start, uint32 stack_size, funcptr func, uint32 argc, ...)
{
	mex_log(MEX_LOG_SYSTEM, "W: This is Dummy SwitchStackToRun for WIN32");
	return 0;
}
#endif
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

MMI_BOOL MexConfig_GetSIM_Using(E_MEX_Sim_Card_ID sim_id )
{

	int cur_setting = MTPNP_AD_NVRAM_NONE;
	MMI_BOOL ret = MMI_FALSE;	
	

	#if (MEX_MTK_CODE_VERSION >= 0x1112)
	if ( (srv_sim_ctrl_is_available(MMI_SIM1)) && (srv_sim_ctrl_is_available(MMI_SIM2)))
	{
		cur_setting = MTPNP_AD_NVRAM_DUALCARD;
	}
	else if(srv_sim_ctrl_is_available(MMI_SIM1))
	{
		cur_setting = MTPNP_AD_NVRAM_CARD1;
	}
	else if(srv_sim_ctrl_is_available(MMI_SIM2))
	{
		cur_setting = MTPNP_AD_NVRAM_CARD2;
	}
	
	#elif (MEX_MTK_CODE_VERSION >= 0x1108)
	
	unsigned char sim_status;
	unsigned char mode_status;
	mmi_idle_get_dual_sim_status(&sim_status, &mode_status);
	if(MMI_SIM_ID_DUAL_SIM == sim_status)
	{
		cur_setting = MTPNP_AD_NVRAM_DUALCARD;
	}
	else if(MMI_SIM_ID_SIM1 == sim_status)
	{
		cur_setting = MTPNP_AD_NVRAM_CARD1;
	}
	else if(MMI_SIM_ID_SIM2 == sim_status)
	{
		cur_setting = MTPNP_AD_NVRAM_CARD2;
	}
	
	#else	//MEX_MTK_CODE_VERSION
	
	E_MTPNP_AD_SIMCARD_USABLE_SIDE state = MTPNP_AD_Get_UsableSide();
	
	if(MTPNP_AD_DUALSIM_USABLE == state)	
	{
		cur_setting = MTPNP_AD_NVRAM_DUALCARD;
	}
	else if(MTPNP_AD_SIMCARD1_USABLE == state)
	{
		cur_setting = MTPNP_AD_NVRAM_CARD1;
	}
	else if(MTPNP_AD_SIMCARD2_USABLE == state)
	{
		cur_setting = MTPNP_AD_NVRAM_CARD2;
	}
	
	#endif	//MEX_MTK_CODE_VERSION

	//#if defined(MEX_DUAL_SIM)
	if(sim_id != E_MEX_SIM_1 && sim_id != E_MEX_SIM_2)
	{
		return MMI_FALSE;
	}
	switch(cur_setting)
	{
		case MTPNP_AD_NVRAM_DUALCARD:
			ret = MMI_TRUE;
			break;
		case MTPNP_AD_NVRAM_CARD1:
			if(sim_id == E_MEX_SIM_1)
				 ret = MMI_TRUE;
			break;
		case MTPNP_AD_NVRAM_CARD2:
			if(sim_id == E_MEX_SIM_2)
				 ret = MMI_TRUE;
			break;
		case MTPNP_AD_NVRAM_FLIGHT:
			ret = MMI_FALSE;
			break;
		default:
			ret = MMI_FALSE;
	}
	//#endif
	
	return ret;

}
#endif	//defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

//获取配置信息，可参考MexLib_Def.h中枚举E_Mex_Phone_Config的注释
int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg)
{    
	switch (cfg)
	{
	case E_MPC_MexVenderID:			//渠道号
#ifdef MEX_QA_MODE
		return iMex_QA_VenderID;
#else
		return MetVenderID;
#endif
	case E_MPC_MexProjectID:		//项目编号
		return 11045;
	case E_MPC_MexPhoneUA:			//机型
#ifdef MEX_QA_MODE
		if(pMex_QA_PhoneUA[0] == 0)
		{
			return (int32)MetPhoneUA; 
		}
		else
		{
			return (int32)pMex_QA_PhoneUA;
		}
#else
		return (int32)MetPhoneUA;
#endif
	case E_MPC_MexMachineID:
		return MetMachineID;
	case E_MPC_DualSim:
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return 1;		
#else
		return 0;
#endif
	case E_MPC_MaxIntMem:
		return MEX_MEM_INTERNAL_SIZE_APP + MEX_MEM_INTERNAL_SIZE_MED;
	case E_MPC_MaxExtMem:
		return MEX_MEM_EXTERNAL_SIZE;
	case E_MPC_MaxReservedMem:
		return MEX_MEM_RESERVED_SIZE;			
	case E_MPC_MinFreeSpace: //最少应满足的T卡空间
		return MINFREESPACE;
	case E_MPC_SystemRootDir:
		return (int32)MEX_SYSROOTPATH;
	case E_MPC_AppRootDir:
		return (int32)MEX_APPROOTPATH;
	case E_MPC_MexDir_In_Phone:
		#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		return (int32)L"C:\\Mex";
		#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		return (int32)L"D:\\Mex";
		#endif
	case E_MPC_Imei:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if(MEX_MTK_CODE_VERSION >= 0x1108)
		{
		static U8 gIMEIBuf[20];
		memset(gIMEIBuf,0,sizeof(gIMEIBuf));
		if(srv_imei_is_valid(MMI_SIM1))
		{
			srv_imei_get_imei(MMI_SIM1,gIMEIBuf,(15 + 1));
		}
#if defined(__MMI_DUAL_SIM_MASTER__)
		else if(srv_imei_is_valid(MMI_SIM2))
		{
			srv_imei_get_imei(MMI_SIM1,gIMEIBuf,(15 + 1));
		}
#endif
		return (int32)gIMEIBuf;
		}
#else
		return (int32)MexPhs_Get_Imei();
#endif
#endif
	case E_MPC_Master_Service_Type:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1112)
			{
				srv_nw_info_service_availability_enum service_availability = srv_nw_info_get_service_availability(MMI_SIM1);
				switch (service_availability)
				{
				case SRV_NW_INFO_SA_LIMITED_SERVICE:
				case SRV_NW_INFO_SA_FULL_SERVICE:
						return 1;
				default:
					return 0;
				}
			}
#else
	#if (MEX_MTK_CODE_VERSION >= 0x1108)
		{	
			extern pwr_context_struct g_pwr_context;
		#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return (MexConfig_GetSIM_Using(E_MEX_SIM_1)&&srv_nw_usab_is_usable(MMI_SIM1));
		#else
			return srv_nw_usab_is_usable(MMI_SIM1);
		#endif
		}
	#else
			{
				extern pwr_context_struct g_pwr_context;
				switch (g_pwr_context.CurrentServiceType)
				{
				case FULL_SERVICE:
				case LIMITED_SERVICE:
				#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
					if(MexConfig_GetSIM_Using(E_MEX_SIM_1))
						return 1;
					else
						return 0;
				#else
					return 1;
				#endif
				default:
					return 0; // no service 	   
				}

			}
	#endif
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            {
                extern E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId);
                E_MEX_Sim_Card_ID eSimId = MexConfig_ConvertSimCard(E_MEX_SIM_1);

                if(eSimId == E_MEX_SIM_1)
                {
        			return MMIPHONE_IsSimOk(MN_DUAL_SYS_1);
               
                }
               else if(eSimId == E_MEX_SIM_3)
                {
                    return MMIPHONE_IsSimOk(MN_DUAL_SYS_3);
                }
#if defined(MEX_QUAD_SIM)      
               else if(eSimId == E_MEX_SIM_4)
                {
                    return MMIPHONE_IsSimOk(MN_DUAL_SYS_4);
                }         
#endif
                else
                {
                    return 0;
	            }
            }
#else 

       return MMIPHONE_IsSimOk(MN_DUAL_SYS_1);

#endif

#endif
		}
	case E_MPC_Slave_Service_Type:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			int service_type = NO_SERVICE;
#if defined(__MMI_DUAL_SIM_MASTER__)
#if (MEX_MTK_CODE_VERSION >= 0x1112)
			{
				srv_nw_info_service_availability_enum service_availability = srv_nw_info_get_service_availability(MMI_SIM2);
				switch (service_availability)
				{
				case SRV_NW_INFO_SA_LIMITED_SERVICE:
				case SRV_NW_INFO_SA_FULL_SERVICE:
					return 1;
				default:
					return 0;
				}
			}
#elif (MEX_MTK_CODE_VERSION >= 0x1108)
			{
				extern pwr_context_struct g_pwr_context;
				return (MexConfig_GetSIM_Using(E_MEX_SIM_2)&&srv_nw_usab_is_usable(MMI_SIM2));
			}		
#else
			{
				extern pwr_context_struct g_pwr_context_2;
	            service_type = g_pwr_context_2.CurrentServiceType;
			}
#endif
#elif defined(__SLT_DUAL_SIM_GEMINI__)
			{
				extern pwr_context_struct gemini_pwr_context_Sim2;
	            service_type = gemini_pwr_context_Sim2.CurrentServiceType; 
			}
#else
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
#endif
            switch (service_type)
            {
            case FULL_SERVICE:
            case LIMITED_SERVICE:
				if(MexConfig_GetSIM_Using(E_MEX_SIM_2))
	                return 1;
				else
					return 0;
            default:
                return 0; // no service        
            }

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_DUAL_SIM)
			return MMIPHONE_IsSimOk(MN_DUAL_SYS_2);
#endif

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            {
                extern E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId);
                E_MEX_Sim_Card_ID eSimId = MexConfig_ConvertSimCard(E_MEX_SIM_2);

                if(eSimId == E_MEX_SIM_2)
                {
        			return MMIPHONE_IsSimOk(MN_DUAL_SYS_2);
               
                }
               else if(eSimId == E_MEX_SIM_3)
                {
                    return MMIPHONE_IsSimOk(MN_DUAL_SYS_3);
                }
#if defined(MEX_QUAD_SIM)      
               else if(eSimId == E_MEX_SIM_4)
                {
                    return MMIPHONE_IsSimOk(MN_DUAL_SYS_4);
                }     
#endif
            }
#endif

#endif
			return 0;   /*lint !e527*/
		}
	case E_MPC_Master_Imsi:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
		return (int32)MexPhs_Get_Master_Imsi();
#endif
	case E_MPC_Slave_Imsi:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return (int32)MexPhs_Get_Slave_Imsi();
#else
		return 0;		
#endif
#endif
	case E_MPC_Master_SmsCent:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
		return (int32)MexPhs_Get_Master_SmsCent();
#endif
	case E_MPC_Slave_SmsCent:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return (int32)MexPhs_Get_Slave_SmsCent();
#else
		return 0;		
#endif
#endif
	case E_MPC_Master_CellInfo:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
		return (int32)MexPhs_Get_Master_CellInfo();
#endif
	case E_MPC_Slave_CellInfo:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return (int32)MexPhs_Get_Slave_CellInfo();
#else
		return 0;
#endif
#endif
	case E_MPC_Master_PLMN:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                
#if (MEX_MTK_CODE_VERSION < 0x1108)
        	extern uint8 gPLMN[];
        	return (int32)gPLMN;
#else
            static U8 gPLMN[6+1];
            memset(gPLMN,0x00,6+1);
            srv_sim_ctrl_get_home_plmn(MMI_SIM1,(S8 *)gPLMN,6+1);
            return (int32)gPLMN;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			PHONE_PLMN_NAME_E  plmn = PLMN_TEST;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_1);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_1);
#endif
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            {
                extern E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId);
                E_MEX_Sim_Card_ID eSimId = MexConfig_ConvertSimCard(E_MEX_SIM_1);

                if(eSimId == E_MEX_SIM_1)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_1);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_1);
#endif
                }
               else if(eSimId == E_MEX_SIM_3)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#endif
                }
#if defined(MEX_QUAD_SIM)
               else if(eSimId == E_MEX_SIM_4)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_4);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_4);
#endif
                }
#endif
            }
#endif
			if(plmn == PLMN_CMCC)
			{
				return E_MEX_NET_ID_CMCC;
			}
			else if(plmn == PLMN_UNICOM)
			{
				return E_MEX_NET_ID_CNGSM;            
			}     
			else
			{
				return E_MEX_NET_ID_UNKNOW;
			}
#endif            
		}		
	case E_MPC_Slave_PLMN:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                

#if defined(__MMI_DUAL_SIM_MASTER__)
#if (MEX_MTK_CODE_VERSION < 0x1108)
    		extern uint8 gPLMN_2[];
    		return (int32)gPLMN_2;	
#else
        	static U8 gPLMN_2[6+1];
        	memset(gPLMN_2,0x00,6+1);
        	srv_sim_ctrl_get_home_plmn(MMI_SIM2,(S8 *)gPLMN_2,6+1);
        	return (int32)gPLMN_2;	
#endif
#elif defined(__SLT_DUAL_SIM_GEMINI__)
			extern uint8 gemini_gPLMNSim2[];
			return (int32)gemini_gPLMNSim2;
#endif
			return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			PHONE_PLMN_NAME_E  plmn = PLMN_TEST;

#if defined(MEX_DUAL_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_2);
#else
			plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_2);
#endif
#endif

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            {
                extern E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId);
                E_MEX_Sim_Card_ID eSimId = MexConfig_ConvertSimCard(E_MEX_SIM_2);

                if(eSimId == E_MEX_SIM_2)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_2);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_2);
#endif              
                }
               else if(eSimId == E_MEX_SIM_3)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#endif
                }
#if defined(MEX_QUAD_SIM)
               else if(eSimId == E_MEX_SIM_4)
                {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_4);
#else
		plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_4);
#endif
                }
#endif
            }
#endif
			if(plmn == PLMN_CMCC)
			{
				return E_MEX_NET_ID_CMCC;
			}
			else if(plmn == PLMN_UNICOM)
			{
				return E_MEX_NET_ID_CNGSM;            
			}     
			else
			{
				return E_MEX_NET_ID_UNKNOW;
			}
#endif           
		}
	case E_MPC_Master_Sim_Remove:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                    
			return (int32)((!MexConfig_GetSIM_Using(E_MEX_SIM_1))||mmi_bootup_is_sim_removed());
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			SIM_STATUS_E  status = SIM_STATUS_OK;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1);
#else
		status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_1);
#endif
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            if(status == SIM_STATUS_NO_SIM)
            {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3);
#else
		status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_3);
#endif
            }
#if defined(MEX_QUAD_SIM)
            if(status == SIM_STATUS_NO_SIM)
            {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_4);
#else
		status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_4);
#endif
            }
#endif
#endif
			return (int32)(status == SIM_STATUS_NO_SIM);
#endif
		}
	case E_MPC_Slave_Sim_Remove:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                
#if defined(__MMI_DUAL_SIM_MASTER__)
			return (int32)((!MexConfig_GetSIM_Using(E_MEX_SIM_2))||mmi_bootup_is_sim2_removed());	
#elif defined(__SLT_DUAL_SIM_GEMINI__)
			return (int32)slt_mmi_bootup_is_sim2_removed();		
#endif
			return 1;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			SIM_STATUS_E  status = SIM_STATUS_OK;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2);
#else
		status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_2);
#endif
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if((status == SIM_STATUS_NO_SIM) &&
	(SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
	{
	status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3);
	}	

#else
	if((status == SIM_STATUS_NO_SIM) &&
	(SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
	{
	status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_3);
	}	
#endif
#if defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if((status == SIM_STATUS_NO_SIM) &&
	(SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
	{
		status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_4);
	}	
#else
	if((status == SIM_STATUS_NO_SIM) &&
	(SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
	{
		status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_4);
	}	
#endif
	
#endif	
#endif
#endif
            return (int32)(status == SIM_STATUS_NO_SIM);
#endif
		}
	case E_MPC_Master_Sim_Valid:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)        
		return (int32)(MexConfig_GetSIM_Using(E_MEX_SIM_1)&&mmi_bootup_is_sim_valid());
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
{
			BOOLEAN  status = FALSE;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1);
#else
		status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_1);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
            status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM1_ON);       /*lint !e655 */
#endif
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            if(status == FALSE)
            {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#else
		status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
                status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM3_ON);       /*lint !e655 */
#endif
            }
#if defined(MEX_QUAD_SIM)
            if(status == FALSE)
            {
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_4);
#else
		status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_4);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
                status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM4_ON);
#endif
            }
#endif
#endif
			return (int32)status;
}
#endif        
	case E_MPC_Slave_Sim_Valid:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if defined(__MMI_DUAL_SIM_MASTER__)
			return (int32)(MexConfig_GetSIM_Using(E_MEX_SIM_2)&&mmi_bootup_is_sim2_valid());	
#elif defined(__SLT_DUAL_SIM_GEMINI__)
			return (int32)slt_mmi_bootup_is_sim2_valid();		
#endif
			return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
{
            BOOLEAN  status = FALSE;

#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_2);
#else
			status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_2);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
            status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM2_ON);       /*lint !e655 */
#endif

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
 			if((status == FALSE) &&
                (SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
            {
		status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#else
		if((status == FALSE) &&
					(SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
				{
		status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
                status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM3_ON);       /*lint !e655 */
#endif
            }			
#if defined(MEX_QUAD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
						if((status == FALSE) &&
							(SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
						{
					status = MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_4);
#else

 			if((status == FALSE) &&
                (SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
            {
                status = MMIPHONE_IsSimCardOk(MN_DUAL_SYS_4);
#endif
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)
                status = status && (MMIAPISET_GetMultiSysSetting() & MMISET_DAULSYS_ONLY_SIM4_ON);
#endif
            }			
#endif
#endif
#endif
            return (int32)status;
}
#endif        
		}
	case E_MPC_Master_Sim_Service:
	#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		if(MexConfig_GetSIM_Using(E_MEX_SIM_1))
			return MexPhs_GetNetworkID(E_MEX_SIM_1);
		else
			return E_MEX_NET_ID_NONE;
	#else
		return MexPhs_GetNetworkID(E_MEX_SIM_1);
	#endif
	case E_MPC_Slave_Sim_Service:
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)  
	#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		if(MexConfig_GetSIM_Using(E_MEX_SIM_2))
			return MexPhs_GetNetworkID(E_MEX_SIM_2);
		else
			return E_MEX_NET_ID_NONE;
	#else
		return MexPhs_GetNetworkID(E_MEX_SIM_2);
	#endif
#else
		return E_MEX_NET_ID_NONE;		
#endif	
		//MTK 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
	case E_MPC_PHONE_HW_VERSION:
		{
#if defined(MT6205B)
			return E_MMH_6205B;
#elif defined(MT6208)
			return E_MMH_6208;
#elif defined(MT6217)
			return E_MMH_6217;
#elif defined(MT6218)
			return E_MMH_6218;
#elif defined(MT6218B)
			return E_MMH_6218B;
#elif defined(MT6219)
			return E_MMH_6219;
#elif defined(MT6223)
			return E_MMH_6223;
#elif defined (MT6223P)
			return E_MMH_6223P;
#elif defined(MT6225)
			return E_MMH_6225;
#elif defined(MT6226)
			return E_MMH_6226;
#elif defined(MT6226D)
			return E_MMH_6226D;
#elif defined(MT6226M)
			return E_MMH_6226M;
#elif defined(MT6227)
			return E_MMH_6227;
#elif defined(MT6227D)
			return E_MMH_6227D;
#elif defined(MT6228)
			return E_MMH_6228;
#elif defined(MT6229)
			return E_MMH_6229;
#elif defined(MT6230)
			return E_MMH_6230;
#elif defined(MT6235) 
			return E_MMH_6235;
#elif defined(MT6235B)
			return E_MMH_6235B;
#elif defined(MT6238) 
			return E_MMH_6238;
#elif defined(MT6239)
			return E_MMH_6239;
#elif defined(MT6253)
			return E_MMH_6253;
#elif defined(MT6253T)
			return E_MMH_6253T;
#elif defined(MT6268)
			return E_MMH_6268;
#elif defined(MT6268A)
			return E_MMH_6268A;
#elif defined(MT6268T)
			return E_MMH_6268T;
#elif defined(MT6516)
			return E_MMH_6516; 
#else
			return E_MMH_UNKNOW_VER;
#endif
		}

	case E_MPC_PHONE_CODE_VERSION:           //mtk代码版本号  目前开发用的23为08a版的代码  其他的D450 D480 D600 是07B的代码  35为09A的代码
		return MEX_MTK_CODE_VERSION;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		//TODO:	展讯区分版本
	case E_MPC_PHONE_HW_VERSION:
		{
			return E_MSH_UNKNOW_VER;
		}

	case E_MPC_PHONE_CODE_VERSION:
		{
			return E_MSS_UNKNOW_VER;
		}
#endif

	case E_MPC_Profile:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
			extern srv_prof_internal_profile_enum srv_prof_get_activated_profile(void);
			return (int32)srv_prof_get_activated_profile();
#else
			extern uint8 gactivatedprofile;
			return gactivatedprofile;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			uint8 uCurMode = MMIAPIENVSET_GetActiveModeId();//从NV获取情景模式
			switch(uCurMode)
			{
			case MMIENVSET_STANDARD_MODE://普通
				return MEX_PROFILE_GENERAL;
			case MMIENVSET_MEETING_MODE://会议
				return MEX_PROFILE_MEETING;
			case MMIENVSET_NOISY_MODE:  //户外
				return MEX_PROFILE_OUTDOOR;
			case MMIENVSET_INSIDE_MODE: //户内
				return MEX_PROFILE_INDOOR;
			case MMIENVSET_SILENT_MODE: //静音
				return MEX_PROFILE_SILENT;
			default:
				if((MMIENVSET_NOISY_MODE < uCurMode) && (MexConfig_GetPhoneConfig(E_MPC_EarphonePlugged)))
				{
					return MEX_PROFILE_HEADSET; //耳机
				}
				break;
			}
			return (int32)uCurMode;//其他
#endif
		}
	case E_MPC_RingVolumeLevel:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
			extern srv_prof_volume_level srv_prof_get_ring_vol(void);
			return srv_prof_get_ring_vol();

#else
			return GetRingVolumeLevel();
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return MMISET_GetCallRingVolume(); //MMISET_VOL_ZERO ~ MMISET_VOL_SEVEN 7个等级
#endif
		}
	case E_MPC_BacklightOn:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern MMI_BOOL IsBacklightOn(void);
			return IsBacklightOn();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			extern BOOLEAN MMIDEFAULT_IsBacklightOn(void);
			return MMIDEFAULT_IsBacklightOn();
#endif
		}
	case E_MPC_Language:
		return MexPhs_Get_Language();
	case E_MPC_TimeFmt:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern PHNSET_CNTX  g_phnset_cntx;
			return g_phnset_cntx.CurTimeFmt;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (MMISET_TIME_24HOURS == MMIAPISET_GetTimeDisplayType());		
#endif
		}		
	case E_MPC_DateFmt:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern PHNSET_CNTX  g_phnset_cntx;
			return g_phnset_cntx.CurDateFmt;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			MMISET_DATE_DISPLAY_TYPE_E  date_display_type = MMISET_GetDateDisplayType();
			switch(date_display_type)
			{
			case MMISET_DATE_YMD:
				return MEX_DF_YYYY_MM_DD_H;
			case MMISET_DATE_MDY:
				return MEX_DF_MM_DD_YYYY_S;
			case MMISET_DATE_DMY:
				return MEX_DF_DD_MMM_YYYY_H;
            default:
                break;
			}
			return date_display_type; //日期格式定义 参考(mmiset.h)    MMISET_DATE_DISPLAY_TYPE_E							
#endif
		}	
	case E_MPC_FlightMode:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return (mmi_bootup_get_active_flight_mode() != FLIGHTMODE_OFF);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return ((MMIAPISET_GetFlyMode()==false)? 0:1);//参见mmiset_func.h
#endif
		}
	case E_MPC_CurCityName:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern PHNSET_CNTX  g_phnset_cntx;
			return (int32)GetString(TimeZoneData[g_phnset_cntx.currentCity + 1].CityName);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)

			MMI_STRING_T     city_string = {0};
			uint16   uLocalCity = 0;
			if(MN_RETURN_FAILURE == MMI_ReadNVItem(MMINV_ACC_WORLDCLOCK_LOCALCITY,( int16*)&uLocalCity))
				return NULL;		
#if defined(MEX_PLATFORM_SPREAD)
			uLocalCity = mex_ltobs(uLocalCity);
#endif
			if(uLocalCity == g_uLocalCity)
				return g_pCurCityName;//当前保存的城市名返回
			g_uLocalCity = uLocalCity;//更新城市ID	
			if(uLocalCity >= sizeof(s_mex_world_city_arr)/sizeof(uint32))//防止越界
				return NULL;
			MMI_GetLabelTextByLang(s_mex_world_city_arr[uLocalCity], &city_string);
			//得到的城市名不为空
			if((city_string.wstr_len > 0) && (city_string.wstr_ptr != NULL))
			{
				uint8 uLen = city_string.wstr_len+1;
				//获取新的城市名
				if(g_pCurCityName != SCI_NULL)
					SCI_FREE(g_pCurCityName);
				g_pCurCityName = (wchar*)SCI_ALLOC(uLen*sizeof(wchar));
				if(SCI_NULL == g_pCurCityName)
					return NULL;

				SCI_MEM16SET(g_pCurCityName,0,(uLen)*sizeof(wchar));//置0
				MMIAPICOM_Wstrncpy(g_pCurCityName,city_string.wstr_ptr,city_string.wstr_len);
				return (int32)g_pCurCityName;
			}
			return NULL;
#else
            return NULL;
#endif
#endif
		}
	case E_MPC_TimeZone:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern PHNSET_CNTX  g_phnset_cntx;
			float  CityTimeZone = TimeZoneData[g_phnset_cntx.currentCity + 1].CityTimeZone +  0.0001;//精度矫正
			return (int32)(100*CityTimeZone);//返回值放大100倍
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//时区差值表
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)

#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
			int16 time_zone_arr[]  = { 0,1,2,3,4,5,6,7,8,9,10,11,12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1 };
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
		    int16 time_zone_arr[]  = { 0,60,120,180,210,240,270,300,330,345,360,390,420,480,540,570,600,630,660,690,720,765,
				-720,-660,-600,-570,-540,-510,-480,-420,-360,-300,-240,-210,-180,-120,-60};
#endif	
			int16   nLocalCity = 0;
			int32   nRet = 0;
			if(MN_RETURN_FAILURE == MMI_ReadNVItem(MMINV_ACC_WORLDCLOCK_LOCALCITY,( int16*)&nLocalCity))
				return 0;			 
#if defined(MEX_PLATFORM_SPREAD)//大小端转换
			nLocalCity =  mex_ltobs((uint16)nLocalCity);
#endif	
			
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
			nRet = time_zone_arr[nLocalCity]*100;	//返回值放大100倍	
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
			nRet = (int32)(time_zone_arr[nLocalCity]*5/3);		
#endif	
			return nRet;	
#endif
            return 0;   // FIXME: 
#endif
		}		
	case E_MPC_AudPlyRun:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(__MMI_AUDIO_PLAYER__)
			extern MMI_BOOL mmi_audply_is_playing();
			return mmi_audply_is_playing();
#elif defined(__MMI_MEDIA_PLAYER__)
			extern MMI_BOOL mmi_medply_is_play_activated(void);
			return mmi_medply_is_play_activated();
#endif
			return 0;

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return  MMIAPIMP3_IsPlayerPlaying();
      
#endif
		}
	case E_MPC_FMRun:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_FM_RADIO__
			return mmi_fmrdo_is_power_on();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#ifdef FM_SUPPORT
		return (FM_PLAYING == MMIFM_GetStatus());
#endif
        return 0;   /*lint !e527*/
#endif
		}
	case E_MPC_CurWPID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern PHNSET_CNTX  g_phnset_cntx;
			return g_phnset_cntx.curtWpID;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
			MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
			if(MMISET_WALLPAPER_DEFAULT_ID == wallpaper_info.wallpaper_id)
				return 0;//默认壁纸
			return wallpaper_info.wallpaper_id;
#endif
		}	
	case E_MPC_CurWPPath:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern S8* idle_screen_wallpaper_name;
			return (int32)idle_screen_wallpaper_name;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr = {0};
			//get file name info
			MMIMULTIM_GetWallPaperInfo(&wallpaper_file_ptr);
			if(//(PNULL != wallpaper_file_ptr)&&
				(wallpaper_file_ptr.full_path_len > 0))
				return (int32)wallpaper_file_ptr.full_path_name;//如果是系统的壁纸(例如默认、墙纸1、墙纸2或墙纸3等)，则返回NULL
			return 0;
#endif
		}	
	case E_MPC_Usb_MassStorage:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __USB_IN_NORMAL_MODE__
			extern pBOOL mmi_usb_is_in_mass_storage_mode();
			return (int32)mmi_usb_is_in_mass_storage_mode();
#endif
			return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
            extern BOOLEAN MMIAPIUDISK_UdiskIsRun(void);
			return MMIAPIUDISK_UdiskIsRun();
#endif
		}
	case E_MPC_Tcard_State:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		{
#if defined(__SLT_NAND_UNABLE__)
			return fmgr_is_msdc_present();
#elif defined(__MMI_FILE_MANAGER__)
#if  (MEX_MTK_CODE_VERSION >= 0x1112)
		{
			int fs_ret;
			extern int FS_GetDevStatus(kal_uint32 QueryTarget, kal_uint32 QueryType)			;
			fs_ret = FS_GetDevStatus(SRV_FMGR_CARD_DRV, FS_MOUNT_STATE_ENUM);
			if (fs_ret == FS_DRIVE_NOT_FOUND ||
			(fs_ret <= FS_MSDC_MOUNT_ERROR && fs_ret >= FS_MSDC_NOT_PRESENT) ||
			fs_ret == FS_MEDIA_CHANGED )
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			//extern MMI_BOOL srv_fmgr_drv_is_valid(U8 drv_letter);
			//return srv_fmgr_drv_is_valid(SRV_FMGR_CARD_DRV);  //only card 1
			if (FS_GetDevStatus(SRV_FMGR_CARD_DRV, FS_MOUNT_STATE_ENUM) < FS_NO_ERROR)
				return FALSE;
			else
				return TRUE;
#elif (MEX_MTK_CODE_VERSION >= 0x0824)
			return mmi_fmgr_is_msdc_present();		
#else
			return fmgr_is_msdc_present();	
#endif
#else	
			return 1;
#endif
		}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		{
			extern BOOLEAN MMIAPIUDISK_UdiskIsRun(void);
			extern BOOLEAN  MMIAPISD_IsCardLogOn(void);
			extern BOOLEAN MMIAPIENG_GetIQModeStatus(void);
			
			if (MMIAPIUDISK_UdiskIsRun() || MMIAPISD_IsCardLogOn() || MMIAPIENG_GetIQModeStatus())
			{
				return FALSE;
			}
			
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
			if (DEVICE_STATUS_OK != MMIAPIUDISK_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
#else
			if (DEVICE_STATUS_OK != MMIAPIUDISK_GetDeviceStatus(MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN))
#endif
			    return FALSE;
			else
				return TRUE;
		}
#else
        mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
		return 0;
#endif

	case E_MPC_Tcard_Drive:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#if (MEX_MTK_CODE_VERSION >= 0x1032)
		{
			return SRV_FMGR_CARD_DRV; // only card 1
		}
	#else
		return MMI_CARD_DRV;
	#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		return (int32)'E';
#else
        mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
		return 'C';
#endif

	case E_MPC_Tcard_FreeSpace:
		{
			extern int32 MexPhs_Get_FreeSpace(uint8);
			return MexPhs_Get_FreeSpace(1);
		}
	case E_MPC_System_FreeSpace:
		{
			extern int32 MexPhs_Get_FreeSpace(uint8);
			return (int32)MexPhs_Get_FreeSpace(0);
		}
	case E_MPC_ClamClose:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return IsClamClose();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return MMIDEFAULT_GetFlipStatus()?1:0;
#endif
		}
	case E_MPC_BT_Receiving:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if  (MEX_MTK_CODE_VERSION >= 0x1112)
#if defined(__MEX_G7_UI_BOOTUP__) || defined(MEX_G7_UI_SUPPORT)
			extern BOOL srv_bt_cm_is_activated(void);   //*  This function is to check if bt is activated
			return srv_bt_cm_is_activated()
#else
			return 0
#endif
#ifdef __MMI_FTS_SUPPORT__
				|| srv_ftps_is_receiving()
#endif
#ifdef __MMI_OPP_SUPPORT__
				|| srv_opps_is_receiving()
#endif
				;
#else
			return 0
#if defined(__MMI_FTS_SUPPORT__)
				|| mmi_ftp_server_is_receiving()
#endif
#if defined(__MMI_OPP_SUPPORT__)
				|| mmi_opp_is_receiving()
#endif
				;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//return MMIAPIBT_A2dpBusy();
#ifdef BLUETOOTH_SUPPORT
			return (MMIBT_HEADSET_STATUS_OPEN == MMIBT_GetDeviceStatus()); //见mmibt_app.h
#endif
#endif
		}
	case E_MPC_MSG_Interrupt:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1112)
			if (srv_ucm_query_call_count(SRV_UCM_INCOMING_STATE, SRV_UCM_CALL_TYPE_ALL, NULL) > 0 || srv_reminder_is_expiring())
			{
    			MMI_TRACE(MMI_COMMON_TRC_G6_MSG, TRC_MMI_SMS_MMI_MSG_CHECK_INTERRUPT);
				return MMI_TRUE;
			}
			else
			{
				return MMI_FALSE;
			}
#else
			return mmi_msg_check_interrupt();
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (MMIAPICC_IsInState(CC_IN_CALL_STATE) || MMIAPIALM_IsAliveWinOpened());//来电或闹铃
#endif
		}

	case E_MPC_Screen_Width:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		{
#if MEX_SCREEN_ESPECIAL_VERSION
			extern uint16 g_now_screen_width;
			return (int32)g_now_screen_width;
#else
			return UI_DEVICE_WIDTH;
#endif
		}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		{
#if MEX_SCREEN_ESPECIAL_VERSION
			extern uint16 g_now_screen_width;
			return (int32)g_now_screen_width;
#else
			return LCD_WIDTH;
#endif
		}
#endif
	case E_MPC_Screen_Height:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		{
#if MEX_SCREEN_ESPECIAL_VERSION
			extern uint16 g_now_screen_height;
			return (int32)g_now_screen_height;
#else
			return UI_DEVICE_HEIGHT;
#endif
		}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		{
#if MEX_SCREEN_ESPECIAL_VERSION
			extern uint16 g_now_screen_height;
			return (int32)g_now_screen_height;
#else
			return LCD_HEIGHT;
#endif
		}
#endif
	case E_MPC_Touch_Supprot:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_TOUCH_SCREEN__
		return 1;
#else
		return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef TOUCH_PANEL_SUPPORT        
		return 1;
#else
		return 0;
#endif
#endif

	case E_MPC_KeyPadLock:	
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return g_keylock_context.gKeyPadLockFlag;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			{
				MMISET_KEYLOCK_TYPE_E  auto_keylock_time = MMISET_KEYLOCK_CLOSE;
				MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
				//init auto boardkey
				MMINV_READ(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time,return_value);
				if (MN_RETURN_SUCCESS != return_value)
				{
					return 1;//初始化为开启
				}
				return  (MMISET_KEYLOCK_CLOSE != auto_keylock_time);
			}
#endif
		}
	case E_MPC_EarphonePlugged:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
			extern MMI_BOOL srv_earphone_is_plug_in(void);
			return srv_earphone_is_plug_in();
#else
			extern uint8 isEarphonePlugged;
			return isEarphonePlugged;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR)
			return 0;
#elif defined(MEX_PLATFORM_SPREAD)
			AUDIO_DEVICE_MODE_TYPE_E    dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
			MMIAPISET_GetDeviceMode(&dev_mode);
			//not headset, can not play
			if( !((AUDIO_DEVICE_MODE_EARPHONE == dev_mode) 
				|| (AUDIO_DEVICE_MODE_EARFREE  == dev_mode))) //let audio output to tv
			{
				return 0;
			}
			else
				return 1;
#endif
		}		
	case E_MPC_LcdSleep:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return mmi_gpio_is_lcd_sleep();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (MMIDEFAULT_IsBacklightOn()?0:1);
#endif
		}
	case E_MPC_Master_NWK_ACCOUNT:
		return MEX_MASTER_NWK_ID;
	case E_MPC_Slave_NWK_ACCOUNT:
		return MEX_SLAVE_NWK_ID;
	case E_MPC_Screen_AllowBltDbBuf:				//是否使用双缓冲
		return MEX_SUPPORT_BLTDBBUF;

	case E_MPC_SUPPORT_CAMERA_NUM:
		return MEX_SUPPORT_CAMERA;

	case E_MPC_CMAERA_ZOOM_LIMIT:
		{
#ifdef __CAMERA_FEATURE_ZOOM__	 
			return CAMERA_FEATURE_ZOOM_COUNT;
#else
			return 0;
#endif
		}
	case E_MPC_BACK_CAMERA_HORIZONTAL:
		{
#ifdef HORIZONTAL_CAMERA
			return 1;
#else
			return 0;
#endif
		}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	case E_MPC_CAMERA_PREVIEW_MAINLCD_ROTATE:
		{
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)			
			return CAMERA_PREVIEW_MAINLCD_ROTATE;
#else
            // TODO: 
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
			return 0;
#endif
		}
#endif
	case E_MPC_LCM_MAINLCD_ROTATE:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)			
			return CAMERA_LCM_MAINLCD_ROTATE;
#else
            // TODO: 
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
            return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return CAMERA_LCM_MAINLCD_ROTATE;
#endif
		}
	case E_MPC_PHONEBOOK_READY:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if (MEX_MTK_CODE_VERSION >= 0x0952)
			return mmi_phb_check_ready(FALSE);
#else
			extern mmi_phb_context_struct g_phb_cntx;            
			if (g_phb_cntx.phb_ready)
			{
				return 1;
			}
			return 0; // Default return 0. Phone book is not ready
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
            return MMIAPIPB_IsPbReady();
#else
			return MMIAPIPB_IsPbReady(true);
#endif
#endif

		}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	case E_MPC_PHONEBOOK_ENTRY_INDEX_PTR:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0952)
			extern U32 MEX_pb_get_syslist();
			return (int32) MEX_pb_get_syslist();
#else
			extern uint16 g_phb_name_index[MAX_PB_ENTRIES];
			return (int32) g_phb_name_index;
#endif
		}
#endif
	case E_MPC_PHONEBOOK_ENTRY_COUNT:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                        
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_used_contacts(PHB_STORAGE_MAX);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_used_contacts_count(PHB_STORAGE_MAX);
#else
			extern uint16 PhoneBookEntryCount;
			return (int32) PhoneBookEntryCount;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 
			return MMIPB_GetEntryCount();
#endif
            return 0; // FIXME:
#endif
		}
	case E_MPC_PHONEBOOK_MEM_PHONE_TOTAL:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_total_contacts(PHB_STORAGE_NVRAM);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_total_contacts_count(PHB_STORAGE_NVRAM);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			return (int32) g_phb_cntx.phone_total;
			return 0; // default return 0
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//judge phonebook whether is ready through read the first phonebook entry on sim
			MMIPB_STORAGE_INFO_T     phb_info;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			phb_info = MMIPB_GetContactStorageInfo(PHONEBOOK_NV_STORAGE);	  // IN:
#else

			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_MAX,                // false if no entry about the id
				PHONEBOOK_NV_STORAGE);    // IN:
#endif		
			//if(phb_info.is_valid)
			{
				return phb_info.max_record_num;
			}            

#endif
		}
	case E_MPC_PHONEBOOK_MEM_PHONE_USED:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_used_contacts(PHB_STORAGE_NVRAM);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_used_contacts_count(PHB_STORAGE_NVRAM);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			return (int32) g_phb_cntx.phone_used;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//judge phonebook whether is ready through read the first phonebook entry on sim
			MMIPB_STORAGE_INFO_T     phb_info;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			phb_info = MMIPB_GetContactStorageInfo(PHONEBOOK_NV_STORAGE);	  // IN:
#else

			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_MAX,                // false if no entry about the id
				PHONEBOOK_NV_STORAGE);    // IN:
#endif
			//if(phb_info.is_valid)
			{
				return phb_info.used_record_num;
			}

#endif
		}
	case E_MPC_PHONEBOOK_MEM_SIM1_TOTAL:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_total_contacts(PHB_STORAGE_SIM);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_total_contacts_count(PHB_STORAGE_SIM);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			if (1 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Valid))
				return 0;
			return (int32) g_phb_cntx.sim_total;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 			
			MMIPB_STORAGE_INFO_T     phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			return MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_1);	  // IN:
#else
			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_1,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:

			if(phb_info.is_valid)
			{
				return phb_info.max_record_num;
			}
			return 0;            
#endif
#endif
		}        
	case E_MPC_PHONEBOOK_MEM_SIM1_USED:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)            
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_used_contacts(PHB_STORAGE_SIM);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_used_contacts_count(PHB_STORAGE_SIM);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			if (1 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Valid))
				return 0;
			return (int32) g_phb_cntx.sim_used;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 	
			MMIPB_STORAGE_INFO_T     phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Master_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 			
			//return MMIPB_GetContactNumOfSim(MN_DUAL_SYS_1);
            return MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, MMIPB_NORMAL_PHONE_CONTACT));
#else
			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_1,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.used_record_num;
			}
			return 0;            
#endif
#endif
		} 
	case E_MPC_PHONEBOOK_MEM_SIM2_TOTAL:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#ifdef MEX_DUAL_SIM
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_total_contacts(PHB_STORAGE_SIM2);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_total_contacts_count(PHB_STORAGE_SIM2);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			if (1 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Valid))
				return 0;
#if defined(__MTPNP_AD_MASTER__)
			return MTPNP_AD_ADN_SIM2GetCapacity();
#else
			return (int32) g_phb_cntx.sim2_total;
#endif
#endif
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 
			MMIPB_STORAGE_INFO_T     phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			return MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_2);   // IN:
#else

			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_2,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.max_record_num;
			}
#endif  
#endif
#else
			return 0;            
#endif
		}        
	case E_MPC_PHONEBOOK_MEM_SIM2_USED:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#ifdef MEX_DUAL_SIM
#if (MEX_MTK_CODE_VERSION >= 0x1108)
			return srv_phb_datamgr_used_contacts(PHB_STORAGE_SIM2);
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
			return srv_phb_datamgr_used_contacts_count(PHB_STORAGE_SIM2);
#else
			extern mmi_phb_context_struct g_phb_cntx;
			if (1 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Valid))
				return 0;
#if defined(__MTPNP_AD_MASTER__)	
			return MTPNP_AD_ADN_SIM2GetUsedNumber();
#else
			return (int32) g_phb_cntx.sim2_used;
#endif
#endif
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 			
			MMIPB_STORAGE_INFO_T phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Slave_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 			
			//return MMIPB_GetContactNumInSim(MN_DUAL_SYS_2);
			
			return MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_SIM2, MMIPB_NORMAL_PHONE_CONTACT));
#else
			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_2,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.used_record_num;
			}
#endif
#endif       
#else
			return 0;
#endif
		}     	
	case E_MPC_MEX_PLATFOEM_STATUS_ICON_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
			return STATUS_ICON_MEX_PLATFORM;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
            // FIXME: not sure what will happen
            return 0;
#endif
		}
	case E_MPC_MEX_MASTER_QQ_STATUS_ICON_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return STATUS_ICON_MEX_MASTER_QQ;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             extern PUBLIC MMI_WIN_ICON_E GetMasterQQIconId(void);
			//             return GetMasterQQIconId();
			//#else
			//             return -1;
			//#endif
#ifdef MET_MEX_QQ_SUPPORT  
		extern PUBLIC int32 GetMasterQQIconId(void);
		return GetMasterQQIconId();
#elif defined(MET_MEX_DOUBLE_QQ_SUPPORT)
		extern PUBLIC int32 GetMasterQQIconId(void);
		return GetMasterQQIconId();
#else
		break;
#endif
#endif
		}
	case E_MPC_MEX_SLAVE_QQ_STATUS_ICON_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return STATUS_ICON_MEX_SLAVE_QQ;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//
			//            extern PUBLIC MMI_WIN_ICON_E GetSlaveQQIconId(void);
			//            return GetSlaveQQIconId();
			//#else
			//             return -1;
			//#endif
#ifdef MET_MEX_QQ_SUPPORT 		
			extern PUBLIC int32 GetSlaveQQIconId(void);
			return GetSlaveQQIconId();          
#elif defined(MET_MEX_DOUBLE_QQ_SUPPORT)			
			extern PUBLIC int32 GetSlaveQQIconId(void);
			return GetSlaveQQIconId();
#else 
            break;
#endif
#endif
		}

	case E_MPC_MEX_PLATFOEM_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
			return E_MII_MEX_PLATFORM_STATUS_ICON;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
            return 0;
#endif
		}
	case E_MPC_MEX_QQ_ONLINE_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return E_MII_MEX_QQ_ONLINE_ICON;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             return  IMAGE_IDLE_TITLE_ICON_QQ_ONLINE;
			//#else
			//             return -1;
			//#endif
#if defined(MET_MEX_QQ_SUPPORT)||defined(MET_MEX_DOUBLE_QQ_SUPPORT)
			return IMG_MET_MEX_ICON_QQ_ONLINE;
#else
			return -1;
#endif
#endif

		}
	case E_MPC_MEX_QQ_OFFLINE_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return E_MII_MEX_QQ_OFFLINE_ICON;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             return  IMAGE_IDLE_TITLE_ICON_QQ_OFFLINE;
			//#else
			//             return -1;
			//#endif
#if defined(MET_MEX_QQ_SUPPORT)||defined(MET_MEX_DOUBLE_QQ_SUPPORT)
			return  IMG_MET_MEX_ICON_QQ_OFFLINE;
#else
			return -1;
#endif
#endif
		}
	case E_MPC_MEX_QQ_MSG_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return E_MII_MEX_QQ_MSG_ICON;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             return  IMAGE_IDLE_TITLE_ICON_QQ_MSG;
			//#else
			//             return -1;
			//
			//#endif
#if defined(MET_MEX_QQ_SUPPORT)||defined(MET_MEX_DOUBLE_QQ_SUPPORT)
			return  IMG_MET_MEX_ICON_QQ_MSG;
#else
			return -1;
#endif
#endif
		}
	case E_MPC_MEX_QQ_HIDE_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return E_MII_MEX_QQ_HIDE_ICON;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             return  IMAGE_IDLE_TITLE_ICON_QQ_HIDE;
			//#else
			//             return -1;
			//#endif
#if defined(MET_MEX_QQ_SUPPORT)||defined(MET_MEX_DOUBLE_QQ_SUPPORT)
			return  IMG_MET_MEX_ICON_QQ_HIDE;
#else
			return -1;
#endif
#endif
		}
	case E_MPC_MEX_QQ_AWAY_ICON_IMAGE_ID:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_QQ_SUPPORT__
			return E_MII_MEX_QQ_AWAY_ICON;
#else
			return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			//#ifdef __MEX_QQ_SUPPORT__
			//             return  IMAGE_IDLE_TITLE_ICON_QQ_AWAY;
			//#else
			//             return -1;
			//#endif
#if defined(MET_MEX_QQ_SUPPORT)||defined(MET_MEX_DOUBLE_QQ_SUPPORT)
			return  IMG_MET_MEX_ICON_QQ_AWAY;
#else
			return -1;
#endif
#endif
		}

	case E_MPC_MEX_DEFUALT_FOLDER_PHOTO:				// 默认照片/图片存放路径
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)FMGR_DEFAULT_FOLDER_PHOTOS;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MMIMULTIM_DIR_PICTURE;//不是以  '\\'  结尾				
#endif
		}
	case E_MPC_MEX_DEFUALT_FOLDER_EBOOKS:			// 默认电子书存放路径 
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)FMGR_DEFAULT_FOLDER_EBOOKS;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	#if defined(MEX_PLATFORM__FOR_SPRD6600L)
return (int32)MMIEBOOK_DIR_UCS2;
	#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
return (int32)MMIMULTIM_DIR_EBOOK;
#endif

			
			

#endif
		}
	case E_MPC_MEX_DEFUALT_FOLDER_VIDEO:				// 默认视频数据存放路径	
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)FMGR_DEFAULT_FOLDER_VIDEO;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MMIMULTIM_DIR_MOVIE;// Vedio
#endif
		}
	case E_MPC_MEX_DEFUALT_FOLDER_TEMP:				// 默认系统临时文件夹
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)FMGR_DEFAULT_FOLDER_TEMP;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)L"Download"; //自定义的
#endif
		}
	case E_MPC_MEX_DEFUALT_FOLDER_MUSIC:				// 默认MP3存放歌曲路径
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)MUSIC_FOLDER;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MMIMULTIM_DIR_MUSIC;
#endif
		}
	case E_MPC_MEX_DEFUALT_FOLDER_AUDIO:				// 默认铃声存放路径
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			return (int32)L"Audio\\";
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MMIMULTIM_DIR_MUSIC;

#endif
		}
	case E_MPC_DualBattery:								//用于双电池的项目
		{
			return 0;
		}
	case E_MPC_Master_Battery_Status:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
#if (MEX_MTK_CODE_VERSION >= 0x1112)
		extern MMI_BOOL srv_charbat_get_charger_status(srv_charbat_status_enum status_id);
		extern MMI_BOOL srv_charbat_is_charging(void);
		if(srv_charbat_is_charging())
		{
			if(srv_charbat_get_charger_status(SRV_CHARBAT_STATUS_CHARGE_COMPLETE))
			{
				return E_MBS_CHARGE_COMPLETE;		//充电完成
			}
			else
			{
				return E_MBS_CHARGE_IN; 				//正在充电
			}
		}
		return E_MBS_NORMAL;						//正常状态
#else
	extern charbat_context_struct g_charbat_context;
			/*
			switch(g_charbat_context.batteryStatus)
			{
			case PMIC_CHARGER_IN:
			case PMIC_USB_CHARGER_IN:
			return E_MBS_CHARGE_IN;
			case PMIC_CHARGE_COMPLETE:
			return E_MBS_CHARGE_COMPLETE;
			default:
			return E_MBS_NORMAL;
			}
			*/
			if(g_charbat_context.isChargerConnected)
			{
				/*
				if( (g_charbat_context.batteryStatus == PMIC_CHARGE_COMPLETE)	
				#ifdef __MMI_USB_SUPPORT__
				|| (g_charbat_context.batteryStatus == PMIC_USB_NO_CHARGER_IN)  //插入usb而没有充电动作
				#endif
				)
				{
				return E_MBS_CHARGE_COMPLETE;		//充电完成
				}
				*/

				if(g_bCharingComplete)
				{
					return E_MBS_CHARGE_COMPLETE;		//充电完成
				}	
				return E_MBS_CHARGE_IN;					//正在充电

			}
			return E_MBS_NORMAL;						//正常状态
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifndef WIN32
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		if(MMIAPIPHONE_GetBatCapacity() >= 5)
#else
		if(MMIPHONE_GetBatCapacity() >= 5)
#endif
			{
				return E_MBS_CHARGE_COMPLETE;//充电完成
			}
#if (MEX_SPRD_CODE_VERSION == 0x10A1145)||(MEX_SPRD_CODE_VERSION == 0x10A6530)
			else if(CHR_BATTERY_CHARGING_S == MEX_CHR_CheckBatteryStatus())
#else
			else if(CHR_BATTERY_CHARGING_S == CHR_CheckBatteryStatus())
#endif
			{
				return  E_MBS_CHARGE_IN;//正在充电
			}
#endif
				return E_MBS_NORMAL;//普通状态		
#endif
		}
	case E_MPC_Master_Battery_Level:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
#if (MEX_MTK_CODE_VERSION >= 0x1112)
			extern srv_charbat_battery_level_enum srv_charbat_get_battery_level(void);
			
			switch (srv_charbat_get_battery_level())
			{
				case BATTERY_LOW_POWEROFF:
				case BATTERY_LOW_TX_PROHIBIT:
				case BATTERY_LOW_WARNING:
					return 0;
					break;
				case BATTERY_LEVEL_0:
					return 2;
					break;
				case BATTERY_LEVEL_1:
					return 3;
					break;
					
				case BATTERY_LEVEL_2:
					return 4;
					break;
					
				case BATTERY_LEVEL_3:
					return 6;
					break;
				default:
					return 4;
			}

#else
			extern charbat_context_struct g_charbat_context;
			return g_charbat_context.chargerLevel;
#endif			
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			extern uint32 CHR_GetBatCapacity(void);
			return CHR_GetBatCapacity();
#endif
		}
	case E_MPC_Slave_Battery_Status:				//用于双电池的项目
		{
			return -1;
		}
	case E_MPC_Slave_Battery_Level:					//用于双电池的项目
		{
			return -1;
		}
	case E_MPC_Max_Battery_Level:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
#ifdef __SLT_BATTERY_4_LEVEL__	//added by yangxinzhao 20071213
			return BATTERY_LEVEL_4;
#elif defined(__SLT_BATTERY_7_LEVEL__)  //slt rdh, 2008-10-27: for 7 battery level 
			return BATTERY_LEVEL_7;
#else	//original codes 
			return BATTERY_LEVEL_3;
#endif    
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return 100;
#endif
		}
	case E_MPC_Master_Sim_Signal:					//networkfunctions.c
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
			#if (MEX_MTK_CODE_VERSION >= 0x1112)
				return srv_nw_info_get_signal_strength_in_percentage(MMI_SIM1);
			#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			srv_nw_info_cntx_struct *cntx = srv_nw_info_get_cntx(MMI_SIM1);
			g_iMasterSimSignal = cntx->signal_strength_in_percentage;
			g_iMasterSimSignal = g_iMasterSimSignal*MEX_MAX_SIGNAL_STRENGTH/100;
			#elif ((MEX_MTK_CODE_VERSION >= 0x0932)&&(MEX_MTK_CODE_VERSION <= 0x0952))
			{
			  #include "sim_common_enums.h"
			  extern U8 mmi_nw_get_signal_strength_in_percentage(const sim_interface_enum required_sim);
			  g_iMasterSimSignal = mmi_nw_get_signal_strength_in_percentage(SIM1);
			}
			#endif
			return g_iMasterSimSignal; //0~100
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			//参考HandleScellRssiInd       Mmiphone.c
			return g_service_status[MN_DUAL_SYS_1].rssiLevel;		
#endif
		}
	case E_MPC_Slave_Sim_Signal:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
#ifdef MEX_DUAL_SIM
			#if (MEX_MTK_CODE_VERSION >= 0x1112)
			return srv_nw_info_get_signal_strength_in_percentage(MMI_SIM2);
			#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			srv_nw_info_cntx_struct *cntx = srv_nw_info_get_cntx(MMI_SIM2);
			g_iSlaveSimSignal = cntx->signal_strength_in_percentage;
			g_iSlaveSimSignal = g_iSlaveSimSignal*MEX_MAX_SIGNAL_STRENGTH/100;
			#elif ((MEX_MTK_CODE_VERSION >= 0x0932)&&(MEX_MTK_CODE_VERSION <= 0x0952))
			{
			  #include "sim_common_enums.h"
			  extern U8 mmi_nw_get_signal_strength_in_percentage(const sim_interface_enum required_sim);
			  g_iSlaveSimSignal = mmi_nw_get_signal_strength_in_percentage(SIM2);
			}
			#endif
			return	g_iSlaveSimSignal; //0~100
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
			//参考HandleScellRssiInd       Mmiphone.c
			return g_service_status[MN_DUAL_SYS_2].rssiLevel;	
#endif
			return 0;   /*lint !e527*/
#endif
		}
	case E_MPC_Max_Sim_Signal:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
			return 32;  //参看 文件SimDetectionDef.h
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 63; //参考HandleScellRssiInd       Mmiphone.c
#endif
		}
	case E_MPC_Invaild_Sim_Signal:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
			return 99;  
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return -1; //未知
#endif
		}

	case E_MPC_MEX_Widget_AudioPlayer:	//系统是否支持mex桌面MP3接口   参考文件 AudioPlayerSrc.c
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return 1;
#else
			return 0;
#endif
		}

	case E_MPC_AudPly_Play_Handle:				// 播放/暂停按钮事件
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_play_handle;
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Stop_Handle:					//	停止按钮事件
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_stop_handle;
#else
			return 0;
#endif

		}
	case E_MPC_AudPly_Next_Handle:					// 下一曲
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_next_handle;
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Prev_Handle:					// 上一曲
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_prev_handle;
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Inc_Vol_Handle:				// 增大音量
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_inc_volume_handle;
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Dec_Vol_Handle:				// 减少音量
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_dec_volume_handle;
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Fast_Forward_Handle:			// 快进
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_fast_forward;
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}
	case E_MPC_AudPly_Rewind_Handle:					// 快退
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_rewind;
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
            return 0;
#endif
		}
	case E_MPC_AudPly_Cencal_Seek_Handle:			// 取消快进快退
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_AudPly_cencal_seek_handle;
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}

	case E_MPC_AudPly_GetInitState:					//获取更新状态
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return Mex_Func_get_audply_init_state();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
            return 0;
#endif

		}
	case E_MPC_AudPly_Is_Stop:						//是否已停止
		{
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return Mex_Func_get_audply_is_stoped();
#else
			return 1;
#endif
		}
	case E_MPC_AudPly_GetMusicName:				//获取当前歌曲的名字
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_title();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
            return 0;
#endif

		}
	case E_MPC_AudPly_GetPlayedMs:					//获取当前歌曲已播放毫秒数
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_played_ms();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
            return 0;
#endif

		}
	case E_MPC_AudPly_GetTotalMs:					//获取当前歌曲总时间
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_total_ms();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}
	case E_MPC_AudPly_MusicIndex:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_played_index();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}
	case E_MPC_AudPly_TotalMusicNum:					//获取当前播放列表中总歌曲数
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_total_num();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}
	case E_MPC_AudPly_Volume:						//获取当前音量
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_volume();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif

		}
	case E_MPC_AudPly_Max_Volume:					//获取最大音量
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Func_get_audply_max_volume();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)  
			return 0;
#endif

		}
	case E_MPC_AudPly_Init:						//初始化播放器
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if MEX_AUDIO_PLAYER_UNDER_CONTROL
			return (int32)Mex_Init_AudPly;
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif
		}
	case E_MPC_AudPly_Spectrum_Display:			//是否支持频谱显示
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#ifdef __RICH_AUDIO_PROFILE__
			return 1;
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return 0;
#endif
		}
	case E_MPC_AudPly_Start_Calc_Spectrum:		//开始计算频谱信息
		{
#ifdef __RICH_AUDIO_PROFILE__
			{
				extern int32 MexInterface_audio_start_calc_spectrum(void);
				return (int32)MexInterface_audio_start_calc_spectrum;
			}
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Stop_Calc_Spectrum:			//停止计算频谱信息
		{
#ifdef __RICH_AUDIO_PROFILE__
			{
				extern int32 MexInterface_audio_stop_calc_spectrum(void);
				return (int32)MexInterface_audio_stop_calc_spectrum;
			}
#else
			return 0;
#endif
		}
	case E_MPC_AudPly_Get_Spectrum:					//获取频谱信息  参数 uint8 top_p[16] uint8 val_p[16]
		{

#ifdef __RICH_AUDIO_PROFILE__
			{
				extern int32 MexInterface_audio_get_spectrum(uint8 *,uint8 *);
				return (int32)MexInterface_audio_get_spectrum;
			}
#else
			return 0;
#endif
		}
	case E_MPC_DEFAULT_NW_MODE:
		{
			return MEX_DEFAULT_NW_MODE;
		}
	case E_MPC_DEFAULT_NW_SIM:
		{
			return MEX_DEFAULT_NW_SIM;
		}

	case E_MPC_MexPlatFromVersion:
		{
			return (int32) MexSystem_GetSystemVersion();
		}
	case E_MPC_VideoIsPlaying:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#if defined(__SF_MTK_APPLICATION_MEDIAPLAYER__ENABLE__)&&!defined(WIN32)		//各种第三方播放器是否在播放的判断
			{
				extern int SFMediaPlayer_IsPlaying(void);
				return SFMediaPlayer_IsPlaying();
			}
#endif
#if defined(__MMI_VIDEO_PLAYER__)
			{
				extern BOOL mdi_video_is_playing();
				return mdi_video_is_playing();
			}
#endif
			return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
			return (MPEG4_STATE_PLAY == MMIMPEG4_GetMpeg4Staus());
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
			return 0;
#endif

#endif
		}
	case E_MPC_VideoIsRecing:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
#ifdef __MMI_VIDEO_RECORDER__
			extern BOOL mdi_video_is_recording();
			return mdi_video_is_recording();
#else
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return (MMIDC_IsDVOpen() ? 1:0);
#endif
		}
	case E_MPC_EXTERNALLIB_GUI:				//是否不将gui函数库写入rom
#if defined(MEX_EXTERNALLIB_GUI)
		return 1;
#else
		return 0;
#endif
	case E_MPC_Master_DummyInCall:
#if defined(MEX_PLATFORM__FOR_D600) || defined(MEX_PLATFORM__FOR_MTK23C) || defined(MEX_PLATFORM_FOR_MTK6253) ||defined(MEX_PLATFORM_FOR_MTK6235)
		return 1;
#endif /* MEX_PLATFORM__FOR_D600 */
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
		return 1;
#endif
		return 0;   /*lint !e527*/
	case E_MPC_Slave_DummyInCall:
#if defined(MEX_PLATFORM__FOR_D600)
		return 1;
#endif /* MEX_PLATFORM__FOR_D600 */
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
		return 1;
#endif
		return 0;   /*lint !e527*/
	case E_MPC_MaxIntMem_App:
		return MEX_MEM_INTERNAL_SIZE_APP;

	case E_MPC_MaxIntMem_Med:
		return MEX_MEM_INTERNAL_SIZE_MED;
	case E_MPC_GetActiveScreenId:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK) 
			if (GetActiveScreenId() == IDLE_SCREEN_ID)
				return E_MSS_Idle;
			return GetActiveScreenId();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
			return MMK_GetFocusWinId();
#endif
		}
	case E_MPC_SysMinUpdateDelay:
#if defined(MEX_PLATFORM__FOR_D600)
		return iUpdateDelay;
#elif defined(MEX_PLATFORM__FOR_MTK23C)
		return iUpdateDelay;
#elif defined(MEX_PLATFORM__FOR_SPRD6600L) || defined(MEX_PLATFORM__FOR_SPRD6800H)
              if((!Mex_IsMexQQMessageRunning()) &&		     
		     (!MexSystem_IsSysRunning() ) &&
		    (!MMIDEFAULT_IsBacklightOn()))
          	{
          	       //更改平台更新时间间隔为3秒
          	       mex_log(MEX_LOG_CONFIG,"E_MPC_SysMinUpdateDelay Enter DeepSleep");
          		return 3 * 1000;
          	}
	       
		return 1;
#else
		 if(!MexSystem_IsSysRunning())
			return 2000;
		else 
		return 1;
#endif
	case E_MPC_ScrWarmUpDelay:
		return MEX_SCREEN_WARMUP_DELAY;

	case E_MPC_CanFileOptionInBackLightOff:
		return MEX_CAN_OP_FILE_IN_BACKLIGHT_OFF;

	case E_MPC_SPECIAL_VERSION:
		{
			extern char * MexSystem_GetSystemInitParam(void);
			char *pSysInitParam = MexSystem_GetSystemInitParam();
			if( pSysInitParam == NULL )
			{
				return E_MSVT_Normal;
			}
			else
			{
				extern int32 Mex_GetCurrEntryAppId(char *pInitParam);
				int32 result = Mex_GetCurrEntryAppId(pSysInitParam);
				if(result > 0)
				{
					return result;
				}
#if defined(MEX_QA_MODE)
                result = Mex_Test_CheckCurrAppId( pSysInitParam );
				if(result > 0)
				{
					return result;
				}			
#endif
				return E_MSVT_Normal;
			}
		}

	case E_MPC_MEX_VERSION:
		return (int32)MexSystem_GetSystemVersion();

	case E_MPC_MEX_PATCH_VERSION:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			extern char *MexConfig_GetMexPatchVer(void);
			return (int32)MexConfig_GetMexPatchVer();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MEX_PLATFORM_PATCH_VERSION;
#endif
		}
	case E_MPC_PauseBgMusicOnLaunch:
		return MEX_PAUSE_BGMUSIC_ON_LAUNCH;

	case E_MPC_MEX_SDK_VER:
		return MEX_SDK_VER;
#if MEX_SDK_VER >= 1002						//添加sdk版本控制宏 方便代码回溯
	case E_MPC_PhnSetWallPaperTypeMask:
		{
			uint16 mask = 0;
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			mask |= E_MWPT_WallPaper;
#ifdef __MMI_SUB_WALLPAPER__
			mask |= E_MWPT_SubWallPaper;
#endif /* __MMI_SUB_WALLPAPER__ */
			mask |= E_MWPT_ScreenSaver;
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
			mask |= E_MWPT_PowerOn;
			mask |= E_MWPT_PowerOff;
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			mask = MMISET_WALLPAPER_FIXED_PIC|MMISET_WALLPAPER_FIXED_ANIM|MMISET_WALLPAPER_ALBUM|MMISET_WALLPAPER_DOWNLOAD;       /*lint !e655 */
#endif
			return mask;
		}
#endif
#if MEX_SDK_VER >= 1003	
	case E_MPC_MaxSocNum:
		{
			return MEX_MAX_SOCKET_NUM;
		}
#endif
#if MEX_SDK_VER >= 1007
	case E_MPC_DeleteScreenHotShutdown:
		{
			#if defined(MEX_G7_UI_BOOT)
			{
				extern U8 Mex_Get_UI_Status(void);
				if(Mex_Get_UI_Status() == E_MEX_UI_STYLE_FAUI) //E_MEX_UI_STYLE_FAUI
				{
					return 0;
				}
			}
			#endif
			
			//mex屏幕被删除时，是否hotshutdown
			return 1;
		}
#endif

#if MEX_SDK_VER >= 1008
	case E_MPC_SUPPORT_ENCODEJPGTOMEM:
		{
			return MEX_ENABLE_ENCODEJPGTOMEM;
		}
#endif /* MEX_SDK_VER */

	case E_MPC_Reserved_UpdateDelay:
		{
			return (int32)&iUpdateDelay;       /*lint !e552 */
		}
	case E_MPC_Reserved_Pre_StartUp:
		{
			return MEX_ENABLE_PRE_START_UP;
		}

		//Tiny加载页面显示文字背景信息，返回Mex_Config_Loading_Page *
	case E_MPC_Reserved_Tiny_Loaing_Page:
		{
		#if MEX_SHOW_LOADING_PAGE
			if(g_bNeedShowLoadingPage)
			{
				return (int32)&g_mex_tiny_loading_page;
			}	
		#endif
			return 0;
		}

		//Man加载页面显示文字背景信息，返回Mex_Config_Loading_Page *
	case E_MPC_Reserved_Man_Loaing_Page:
		{
		#if MEX_SHOW_LOADING_PAGE
			if(g_bNeedShowLoadingPage)
			{
				return (int32)&g_mex_man_loading_page;
			}
		#endif
			return 0;
		}
#if MEX_SDK_VER >= 1009
	case E_MPC_CanConfigScreen:
		{
			return MEX_SCREEN_ESPECIAL_VERSION;
		}
#endif
#if MEX_SDK_VER >= 1010
	case E_MPC_Master_Have_Missed_Call:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_MTK_CODE_VERSION < 0x0952
			extern U8 g_chist_have_missed_call;
			return (g_chist_have_missed_call == 1);

#else
			// TODO:
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

			int32 iRet =0;
			MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
			MMICL_CALL_ARRAY_BCDINFO_T*   arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
			if(PNULL == arraycall_info)
			{
                mex_log(MEX_LOG_SYSTEM,"malloc call info buf fail");
                return iRet;
			}
			return_value = MMICL_ReadNV(MMICL_CALL_MISSED, arraycall_info);
			if((arraycall_info->record_num > 0)&&(MN_DUAL_SYS_1 == arraycall_info->dual_sys))
				iRet = 1;
			SCI_FREE(arraycall_info);
			return iRet;
#endif
		}
		break;  /*lint !e527*/
	case E_MPC_Slave_Have_Missed_Call:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_MTK_CODE_VERSION < 0x0952
#if defined(__SLT_MMI_GEMINI_SEPARATE_DISPLAY_CALLLOG__)
			{
				extern U8 g_chist_have_missed_call_slave;
				return (g_chist_have_missed_call == 1);
			}
#elif defined(__MMI_DUAL_SIM_MASTER__)
			{
				extern U8 MTPNP_PFAL_chist_get_have_missed_call_flag(void);
				return (MTPNP_PFAL_chist_get_have_missed_call_flag() == 1);
			}
#else
			return 0;
#endif

#else
			// TODO:
			return 0;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)

			int32 iRet =0;
			MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
			MMICL_CALL_ARRAY_BCDINFO_T*   arraycall_info = SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
			if(PNULL == arraycall_info)
			{
                mex_log(MEX_LOG_SYSTEM,"malloc call info buf fail");
                return iRet;
			}
			return_value = MMICL_ReadNV(MMICL_CALL_MISSED, arraycall_info);
			if((arraycall_info->record_num > 0)&&(MN_DUAL_SYS_2 == arraycall_info->dual_sys))
				iRet = 1;
			SCI_FREE(arraycall_info);
			return iRet;
#else
       return 0;
#endif
#endif
		}
		break;  /*lint !e527*/
#endif

#if MEX_SDK_VER >= 1009
	case E_MPC_AnyAlarmActivated:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION < 0x1112)
			extern U8 AlmIsAnyAlarmActivated(void);
			return AlmIsAnyAlarmActivated();
#else
			extern U8 srv_alm_active_alms(void);
			if(srv_alm_active_alms() != 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return (int32)MMIALM_HasActiveEvent();
#endif
		}
#endif
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	case E_MPC_Reserved_Enable_Select_DataAccount:
		{
			return MEX_ENABLE_SELECT_DATAACCOUNT;
		}
	case E_MPC_Reserved_Entry_Select_DataAccount:
		{
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
			extern void Mex_Entry_ApnSelectScreen(void);	
			return (int32)Mex_Entry_ApnSelectScreen;
#else
			return 0;
#endif
		}

#endif
#if MEX_SDK_VER >= 1011
	case E_MPC_Has_File_Read_Cache:
		{
			return MEX_HAS_FILE_READ_CACHE;
		}
#endif

	case E_MPC_Phone_Platform:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
			return E_MPP_MTK;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MEX_BIGEND
			return E_MPP_SPREAD;
#else
			return E_MPP_SPREAD_LITTLE;
#endif
#else
			return E_MPP_UNKNOW_PLFM;
#endif

		}

#if MEX_SDK_VER >= 1012  
	case E_MPC_Sram_Address:
		{
#if (MEX_ENABLE_SMALLOC == 0)
			return 0;
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
			extern void MetMex_GetMedIntMemInfo(uint8 **ppSramAddr,uint32 *pSramLen);
			uint8 *pSramAddr = NULL;
			MetMex_GetMedIntMemInfo(&pSramAddr, NULL);
			return (int32) pSramAddr;
#elif (MEX_ENABLE_SMALLOC == 3)
			return MexConfig_GetPhoneConfig(E_MPC_DCM_Address);
#elif (MEX_ENABLE_SMALLOC == 4)
			return MEX_IRAM_BASE_ADDR;
#else
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
			return NULL;
#endif
		}
	case E_MPC_Sram_Length:
		{
#if (MEX_ENABLE_SMALLOC == 0)
			return 0;
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
			extern void MetMex_GetMedIntMemInfo(uint8 **ppSramAddr,uint32 *pSramLen);
			uint32 iSramLen = 0;
			MetMex_GetMedIntMemInfo(NULL, &iSramLen);
			return (int32) iSramLen;
#elif (MEX_ENABLE_SMALLOC == 3)
			return MexConfig_GetPhoneConfig(E_MPC_DCM_Length);
#elif (MEX_ENABLE_SMALLOC == 4)
			return (MEX_IRAM_END_ADDR -MEX_IRAM_BASE_ADDR);
#else
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
			return 0;
#endif

		}
	case E_MPC_Sram_Left_Size:
		{
#if (MEX_ENABLE_SMALLOC == 0)
			return 0;
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
			return (int32) med_int_left_size();
#elif (MEX_ENABLE_SMALLOC == 3)
			if (g_mex_sram.m_iPoolId != NULL)
				return g_mex_sram.m_iLeftSize;
			return 0;
#elif (MEX_ENABLE_SMALLOC == 4)
			if(g_Mex_Sram_Mem_Pool != NULL)
				return g_Mex_Sram_Mem_Pool->m_iMemSize;
			else
				return 0;
#else
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
			return 0;
#endif

		}
	case E_MPC_Sram_Max_Alloc_Size:
		{
#if (MEX_ENABLE_SMALLOC == 0)
			return 0;
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
			extern uint32 MetMex_MedInt_MaxAllocSize(void);
			return (int32) MetMex_MedInt_MaxAllocSize();
#elif (MEX_ENABLE_SMALLOC == 3)
			if (g_mex_sram.m_iPoolId != NULL)
				return kal_adm_get_max_alloc_size( (KAL_ADM_ID) g_mex_sram.m_iPoolId );
			return 0;
#elif (MEX_ENABLE_SMALLOC == 4)
			if(g_Mex_Sram_Mem_Pool != NULL)
				return g_Mex_Sram_Mem_Pool->m_iMemSize;
			else
				return 0;
#else
            mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
			return 0;
#endif

		}
	case E_MPC_DCM_Address:
#if defined(MEX_PLATFORM_MODIS)
		{
			static uint8 dummy_dcm[MEX_DCM_LENGTH];
			return (int32) dummy_dcm;
		}
#elif defined(MEX_PLATFORM_MTK)
		return (int32)MEX_DCM_ADDRESS;
#else
		return NULL;
#endif
	case E_MPC_DCM_Length:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		return MEX_DCM_LENGTH;
#else
        return 0;
#endif
	case E_MPC_Mdi_Support_Mid_Play_File:
		{
			return MEX_MDI_SUPPORT_MID_PLAY_FILE;
		}
	case E_MPC_Mdi_Support_Wav_Play_File:
		{
			return MEX_MDI_SUPPORT_WAV_PLAY_FILE;
		}
	case E_MPC_Mdi_Support_Mp3_Play_File:
		{
			return MEX_MDI_SUPPORT_MP3_PLAY_FILE;
		}

	case E_MPC_SwitchStackToRun_Funcptr:
		{
#if defined(MEX_PLATFORM_MODIS)
			return (int32) MexConfig_SwitchStackToRun_Dummy;
#elif defined(MEX_PLATFORM_MTK)
			extern uint32 INT_SwitchStackToRun(void *stack_start, uint32 stack_size, funcptr func, uint32 argc, ...);
			return (int32) INT_SwitchStackToRun;
#else
			return NULL;
#endif
		}


	case E_MPC_Mix_Control_Interface:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		{
			return NULL;
		}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		{

			extern Mex_Mix_Control_Interface_Struct *MexMix_GetControlInterface(void);
			return (int32)MexMix_GetControlInterface();
		}
#endif
#endif
#if MEX_SDK_VER >= 1013     
	case E_MPC_SleepTime_For_SoundPlay:
		{
			return MEX_TIME_DELAY_FOR_SOUNDPLAY;
		}
#endif 
#if MEX_SDK_VER >= 1017
	case E_MPC_MSN_ICON_ID:
		
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return STATUS_ICON_MEX_MSN;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#ifdef MET_MEX_MSN_SUPPORT			
{
		extern int32 GetMSNIconId(void);
		return GetMSNIconId();  
}
#else
        return -1;
#endif 

#endif
	case E_MPC_MSN_IMAGE_ONLINE_ID:
		
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_ONLINE_ICON;
#else
		return -1;
#endif  	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT	
			return IMG_MET_MEX_ICON_MSN_ONLINE;
#else
        return -1;
#endif 
#endif

	case E_MPC_MSN_IMAGE_OFFLINE_ID: 
		
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_OFFLINE_ICON;
#else
		return -1;
#endif 
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT	
		return IMG_MET_MEX_ICON_MSN_OFFLINE;
#else
		return -1;
#endif 
#endif
	case E_MPC_MSN_IMAGE_AWAY_ID:
		
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_AWAY_ICON;
#else
		return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT	
		return IMG_MET_MEX_ICON_MSN_AWAY;
#else
		return -1;
#endif 
#endif

	case E_MPC_MSN_IMAGE_BUSY_ID:

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_BUSY_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT	
		return IMG_MET_MEX_ICON_MSN_BUSY;
#else
		return -1;
#endif 
#endif

	case E_MPC_MSN_IMAGE_MSG_ID:

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_MSG_ICON;
#else
		return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT
		return IMG_MET_MEX_ICON_MSN_MSG;
#else
		return -1;
#endif 
#endif

	case E_MPC_MSN_IMAGE_IDLE_ID:

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#ifdef __MEX_MSN_SUPPORT__
		return E_MII_MEX_MSN_IDLE_ICON;
#else
		return -1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_MSN_SUPPORT
		return IMG_MET_MEX_ICON_MSN_IDLE;
#else
		return -1;
#endif 
#endif
#endif

#if (MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__)   //???SIM?
    case E_MPC_Third_NWK_ACCOUNT:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		return MEX_MASTER_NWK_ID;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)			
        return MEX_SLAVE_NWK_ID;
#endif
    case E_MPC_Third_Service_Type:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#if (GEMINI_PLUS >= 3)
		return srv_nw_usab_is_usable(MMI_SIM3);
	#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
			return MMIPHONE_IsSimOk(MN_DUAL_SYS_3);
#else
            return 0;
#endif
#endif
		}   
    case E_MPC_Third_Imsi:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	#if (GEMINI_PLUS >= 3)
		return (int32)MexPhs_Get_Third_Imsi();
	#endif
#elif defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return (int32)MexPhs_Get_Third_Imsi();
#else
        return 0;       
#endif
#endif
#endif
    case E_MPC_Third_SmsCent:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_THIRD_SIM)||defined (__MET_MEX_SIM_PLUS__)   //支持三SIM卡
		return (int32)MexPhs_Get_Third_SmsCent();
#else
		return 0;		
#endif
#endif
    case E_MPC_Third_CellInfo:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
#if defined(MEX_THIRD_SIM)||defined (__MET_MEX_SIM_PLUS__)   //支持三SIM卡
		return (int32)MexPhs_Get_Third_CellInfo();
#else
		return 0;		
#endif
#endif
	case E_MPC_Third_PLMN:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                
			
						 U8 gPLMN[6+1];
							memset(gPLMN,0x00,6+1);
					
		#if (GEMINI_PLUS >= 3)
							srv_sim_ctrl_get_home_plmn(MMI_SIM3,(S8 *)gPLMN,6+1);
		#endif
						return (int32)gPLMN;	

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM)
			PHONE_PLMN_NAME_E  plmn;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	plmn = MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#else
	plmn = MMIPHONE_GetNetworkName(MN_DUAL_SYS_3);
#endif
			if(plmn == PLMN_CMCC)
			{
				return E_MEX_NET_ID_CMCC;
			}
			else if(plmn == PLMN_UNICOM)
			{
				return E_MEX_NET_ID_CNGSM;            
			}     
			else
			{
				return E_MEX_NET_ID_UNKNOW;
			}
#else
			return 0;            
#endif
#endif           
		}    
	case E_MPC_Third_Sim_Remove:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                
#if (GEMINI_PLUS >= 3)
						return (int32)(srv_sim_ctrl_get_unavailable_cause(MMI_SIM3) ==
							SRV_SIM_CTRL_UA_CAUSE_NOT_INSERTED);
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM)
			SIM_STATUS_E  status;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	status = MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3);
#else
	status = MMIPHONE_GetSimStatus(MN_DUAL_SYS_3);
#endif
			return (int32)(status == SIM_STATUS_NO_SIM);
#else
			return 1;
#endif
#endif

		}
	case E_MPC_Third_Sim_Valid:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
	#if (GEMINI_PLUS >= 3)
					return (int32) srv_sim_ctrl_is_available(MMI_SIM3);
	#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	return (int32)MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#else
	return (int32)MMIPHONE_IsSimCardOk(MN_DUAL_SYS_3);
#endif
#else
            return 0;
#endif
#endif
		}
    case E_MPC_Third_Sim_Service:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)  
	#if (GEMINI_PLUS >= 3)

		return MexPhs_GetNetworkID(E_MEX_SIM_3);
	#endif
#elif defined(MEX_THIRD_SIM)
		return MexPhs_GetNetworkID(E_MEX_SIM_3);
#else
		return E_MEX_NET_ID_NONE;		
#endif           
    case E_MPC_PHONEBOOK_MEM_SIM3_TOTAL:
		{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 
			MMIPB_STORAGE_INFO_T     phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Third_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Third_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			return MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_3);	  // IN:
#else

			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_3,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.max_record_num;
			}
#endif
#else
			return 0;  
#endif       

#endif
		}        
	case E_MPC_PHONEBOOK_MEM_SIM3_USED:
		{

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_THIRD_SIM)
			//judge phonebook whether is ready through read the first phonebook entry on sim
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 
			MMIPB_STORAGE_INFO_T phb_info;
#endif

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Third_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Third_Sim_Valid))
				return 0;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 			
			//return MMIPB_GetContactNumInSim(MN_DUAL_SYS_3);

			return MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_SIM3, MMIPB_NORMAL_PHONE_CONTACT));
#else
			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_3,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.used_record_num;
			}
#endif
#else
			return 0;
#endif       

#endif
		}
    case E_MPC_Third_Sim_Signal:
		{

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#if defined(MEX_THIRD_SIM)
			//参考HandleScellRssiInd       Mmiphone.c
			return g_service_status[MN_DUAL_SYS_3].rssiLevel;	
#else
			return 0;
#endif
#endif
		}
#endif //MEX_SDK_VER >= 1018 
#if MEX_SDK_VER >= 1022
	case E_MPC_FETION_ICON_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   

#ifdef __MEX_FETION_SUPPORT__
		return STATUS_ICON_MEX_FETION;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT			
{
		extern int32 GetFETIONIconId(void);
		return GetFETIONIconId();  
}
#else
        return -1;
#endif 
#endif
		break;  /*lint !e527*/
		
	case E_MPC_FETION_IMAGE_ONLINE_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_ONLINE_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_ONLINE;
#else
		return -1;
#endif
#endif
		break;  /*lint !e527*/
		
	case E_MPC_FETION_IMAGE_OFFLINE_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_OFFLINE_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_OFFLINE;
#else
		return -1;
#endif
#endif
		break;  /*lint !e527*/
		
	case E_MPC_FETION_IMAGE_AWAY_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_AWAY_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_AWAY;
#else
		return -1;
#endif
#endif
		break;  /*lint !e527*/
	case E_MPC_FETION_IMAGE_BUSY_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_BUSY_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_BUSY;
#else
		return -1;
#endif
#endif
		break;  /*lint !e527*/
	case E_MPC_FETION_IMAGE_MSG_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_IDLE_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_MSG;
#else
		return -1;
#endif
#endif
		break;  /*lint !e527*/
	case E_MPC_FETION_IMAGE_HIDE_ID:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if defined(__MEX_FETION_SUPPORT__) && !(!(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032))  // the latter is for the MTK6236_10A_M580 Modis compiled successfully
		return E_MII_MEX_FETION_HIDE_ICON;
#else
		return -1;
#endif

#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MET_MEX_FETION_SUPPORT
		return IMG_MET_MEX_ICON_FETION_HIDE;
#else
		return -1;
#endif	
#endif
		break;  /*lint !e527*/
#endif

	case E_MPC_Reserved_BreathLed_PowerOn:
		{
            return 0;
        }
	case E_MPC_Reserved_BreathLed_PowerOff:
		{
            return 0;
        }
#if MEX_SDK_VER >= 1023		
	case E_MPC_Numbers_Sim:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
        //TODO:
        return -1;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)//获取当前软件版本支持的sim卡数目
    
#if defined(MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_TRI)
	    return 3;
#elif defined(MULTI_SIM_SYS_DUAL)
	    return 2;
#else
		return 1;
#endif
#endif
		break;  /*lint !e527*/
#endif
	case E_MPC_Download_font_size: 
		#if defined(MEX_DOWNLOADER_FONT_SIZE) && (MEX_DOWNLOADER_FONT_SIZE == 2)
			mex_log(MEX_LOG_CONFIG,"MEX_DOWNLOADER_FONT_SIZE 2");
			return 2;
		#endif

		#if defined(MEX_DOWNLOADER_FONT_SIZE) && (MEX_DOWNLOADER_FONT_SIZE == 1)
			mex_log(MEX_LOG_CONFIG,"MEX_DOWNLOADER_FONT_SIZE 1");
			return 0;
		#endif

		#if defined(MEX_DOWNLOADER_FONT_SIZE) && (MEX_DOWNLOADER_FONT_SIZE == 0)
			mex_log(MEX_LOG_CONFIG,"MEX_DOWNLOADER_FONT_SIZE 0");
			return 1;
		#endif

	break;  /*lint !e527*/
		case E_MPC_GB2312_SUPPORT:
		{
			#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
				#ifdef __MMI_CHSET_GB2312__
					return 0;
				#else
					return 1;
				#endif
			#else
				return 0;
			#endif
		}
	case E_MPC_Gprs_sim_card:
#if MEX_SOC_GPRS_SIM_SELECT
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		#if (MEX_MTK_CODE_VERSION >= 0x1032)
		  if (Mex_Get_gprs_sim()== E_MEX_SIM_2)
		  	return E_MEX_SIM_2;
		  else
		  	return E_MEX_SIM_1;
		#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
        if (Mex_Get_Select_sim()== E_MEX_SIM_2)
		  return E_MEX_SIM_2;
		else
		  return E_MEX_SIM_1;
#endif
#else
			return E_MEX_SIM_1;
#endif
		break;  /*lint !e527*/
#ifdef __MET_MEX_SIM_PLUS__
	
	
	case E_MPC_Fouth_Service_Type:
	#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
		#if (GEMINI_PLUS >= 4)
			return srv_nw_usab_is_usable(MMI_SIM4);
		#endif
	#endif
			break;

	case E_MPC_Fouth_Imsi:
#ifdef MET_MEX_OVERSEAS_VERSION        
        return 0;
#else
		return (int32)MexPhs_Get_Fouth_Imsi();	
#endif
	case E_MPC_Fouth_PLMN:
		{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                

#if defined(__MMI_DUAL_SIM_MASTER__)
			 U8 gPLMN[6+1];
				memset(gPLMN,0x00,6+1);
		
	#if (GEMINI_PLUS >= 4)
				srv_sim_ctrl_get_home_plmn(MMI_SIM4,(S8 *)gPLMN,6+1);
	#endif
			return (int32)gPLMN;	
#elif defined(__SLT_DUAL_SIM_GEMINI__)
			extern uint8 gemini_gPLMNSim2[];
			return (int32)gemini_gPLMNSim2;
#endif
			return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
			return 0;
#endif           
		}

	case E_MPC_Fouth_Sim_Remove:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)                                
#if (GEMINI_PLUS >= 4)
			return (int32)(srv_sim_ctrl_get_unavailable_cause(MMI_SIM4) ==
                SRV_SIM_CTRL_UA_CAUSE_NOT_INSERTED);
#endif
#endif
			return 0;

	break;
	
		case E_MPC_Fouth_Sim_Valid:
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)   
#if (GEMINI_PLUS >= 4)
		return (int32) srv_sim_ctrl_is_available(MMI_SIM4);
#endif
#endif
		return 0;
		break;
		
	case E_MPC_Fouth_Sim_Service:
		return MexPhs_GetNetworkID(E_MEX_SIM_4);
		
	case E_MPC_Fouth_NWK_ACCOUNT:
		return MEX_SLAVE_NWK_ID;
        
    case E_MPC_PHONEBOOK_MEM_SIM4_TOTAL:
		{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_QUAD_SIM)
			//judge phonebook whether is ready through read the first phonebook entry on sim
			MMIPB_STORAGE_INFO_T     phb_info;

			if (1 == MexConfig_GetPhoneConfig(E_MPC_Fouth_Sim_Remove))
				return 0;
			if (0 == MexConfig_GetPhoneConfig(E_MPC_Fouth_Sim_Valid))
				return 0;

			phb_info = MMIPB_GetPhonebookInfo(                
				MN_DUAL_SYS_4,                // false if no entry about the id
				PHONEBOOK_SIM_STORAGE);    // IN:
			if(phb_info.is_valid)
			{
				return phb_info.max_record_num;
			}
#endif       
			return 0;            

#endif
		}        

#endif/*__MET_MEX_SIM_PLUS__*/

	case E_MPC_FuncPtrInvoke_Post_Event:
		if (memcmp("2.2.3.0" , (const char *)mex_GetPhoneConfig(E_MPC_MexPlatFromVersion), sizeof("2.2.3.0")) > 0)
		{
			return 0;
		}
        else
        {
            extern int32 MexInterface_invoke_post_event(void);
		return MexInterface_invoke_post_event();
        }
#if defined(MEX_SUPPORT_FOREIGN_VERSION)
	case E_MPC_PHONE_CHINESE_IS_DEFINED:
#if defined(__MMI_LANG_SM_CHINESE__)
		return 1;
#else
		return	0;
#endif
#endif
	
	default:
        mex_log(MEX_LOG_SYSTEM,"Mex GetPhoneConfig %d invalid",cfg);
        break;
	}
	return 0;
}

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//从mex keycode转换到mtk系统常量
mmi_keypads_enum MexConfig_MexKeyCodeToMtk(E_Mex_KeyCode eKeyCode)
{
	switch (eKeyCode)
	{
	case E_MKC_KEY_0:
		return KEY_0;
	case E_MKC_KEY_1:
		return KEY_1;
	case E_MKC_KEY_2:
		return KEY_2;
	case E_MKC_KEY_3:
		return KEY_3;
	case E_MKC_KEY_4:
		return KEY_4;
	case E_MKC_KEY_5:
		return KEY_5;
	case E_MKC_KEY_6:
		return KEY_6;
	case E_MKC_KEY_7:
		return KEY_7;
	case E_MKC_KEY_8:
		return KEY_8;
	case E_MKC_KEY_9:
		return KEY_9;
	case E_MKC_KEY_SOFTLEFT: 				//按键 左软键
		return KEY_LSK;
	case E_MKC_KEY_SOFTRIGHT:				//按键 右软键
		return KEY_RSK;
	case E_MKC_KEY_ENTER:					//按键 确认/选择
		return KEY_ENTER;
	case E_MKC_KEY_UP:						//按键 上
		return KEY_UP_ARROW;
	case E_MKC_KEY_DOWN: 					//按键 下
		return KEY_DOWN_ARROW;
	case E_MKC_KEY_LEFT: 					//按键 左
		return KEY_LEFT_ARROW;
	case E_MKC_KEY_RIGHT:					//按键 右
		return KEY_RIGHT_ARROW;
	case E_MKC_KEY_SEND: 					//按键 接听键
		return KEY_SEND;
	case E_MKC_KEY_END:						//按键 挂机键
		return KEY_END;
	case E_MKC_KEY_CLEAR:					//按键 清除键
		return KEY_CLEAR;
	case E_MKC_KEY_STAR: 					//按键 *
		return KEY_STAR;
	case E_MKC_KEY_POUND:					//按键 #
		return KEY_POUND;
	case E_MKC_KEY_VOLUME_UP: 				//按键 侧键上
		return KEY_VOL_UP;
	case E_MKC_KEY_VOLUME_DOWN:				//按键 侧键下
		return KEY_VOL_DOWN;
	case E_MKC_KEY_INVALID:					//非法值
		return KEY_INVALID;
	}
	return KEY_INVALID;
}


mmi_key_types_enum MexConfig_MexKeyEventToMtk(E_Mex_KeyEvent eEvent)
{
	switch (eEvent)
	{
	case E_MKE_Up:
		return KEY_EVENT_UP;
	case E_MKE_Down:
		return KEY_EVENT_DOWN;
	case E_MKE_Repeat:
		return KEY_REPEAT;
	}
	return MAX_KEY_TYPE;
}


//从mtk系统常量到mex常量的转换 目前仅提供给system使用
int32 MexConfig_ConvertMtkDef(E_SYS2MEX_Def_Convert param)
{
	switch(param)
	{
	case E_M2MDC_MOD_NVRAM:
		return MOD_NVRAM;

	case E_M2MDC_MOD_SMS:
		return MOD_SMS;

	case E_M2MDC_MOD_SMS_2:
#ifdef __GEMINI__
		return MOD_SMS_2;
#else
		return -1;
#endif

	case E_M2MDC_MOD_SIM:
		return MOD_SIM;

	case E_M2MDC_MOD_SIM_2:
#ifdef __GEMINI__
		return MOD_SIM_2;
#else
		return -1;
#endif

	case E_M2MDC_MOD_L4C:
		return MOD_L4C;

	case E_M2MDC_MOD_L4C_2:
#ifdef __GEMINI__
		return MOD_L4C_2;
#else
		return -1;
#endif

	case E_M2MDC_MOD_PHB:
		return MOD_PHB;

	case E_M2MDC_MOD_PHB_2:
#ifdef __GEMINI__
		return MOD_PHB_2;
#else
		return -1;
#endif

	case E_M2MDC_MOD_AS:
		return MOD_AS;

	case E_M2MDC_MOD_AS_2:
#ifdef __GEMINI__
		return MOD_AS_2;
#else
		return -1;
#endif

	case E_M2MDC_MOD_MED:
		return MOD_MED;

	case E_M2MDC_MOD_MMI:
		return MOD_MMI;
		//SAP
	case E_M2MDC_MMI_L4C_SAP:
		return MMI_L4C_SAP;
		//END
		//按键   可参考文件 GlobalConstants.h
	case E_M2MDC_KEY_0:
		return KEY_0;

	case E_M2MDC_KEY_1:
		return KEY_1;

	case E_M2MDC_KEY_2:
		return KEY_2;

	case E_M2MDC_KEY_3:
		return KEY_3;

	case E_M2MDC_KEY_4:
		return KEY_4;

	case E_M2MDC_KEY_5:
		return KEY_5;

	case E_M2MDC_KEY_6:
		return KEY_6;

	case E_M2MDC_KEY_7:
		return KEY_7;

	case E_M2MDC_KEY_8:
		return KEY_8;

	case E_M2MDC_KEY_9:
		return KEY_9;

	case E_M2MDC_KEY_UP_ARROW:
		return KEY_UP_ARROW;

	case E_M2MDC_KEY_DOWN_ARROW:
		return KEY_DOWN_ARROW;

	case E_M2MDC_KEY_LEFT_ARROW:
		return KEY_LEFT_ARROW;

	case E_M2MDC_KEY_RIGHT_ARROW:
		return KEY_RIGHT_ARROW;

	case E_M2MDC_KEY_ENTER:
		return KEY_ENTER;
#if defined(__MEX_G7_UI_BOOTUP__) || defined(MEX_G7_UI_SUPPORT)
    case E_M2MDC_KEY_End:
		return KEY_END;
#endif

	case E_M2MDC_KEY_STAR:
		return KEY_STAR;

	case E_M2MDC_KEY_POUND:
		return KEY_POUND;

	case E_M2MDC_KEY_LSK:
		return KEY_LSK;

	case E_M2MDC_KEY_RSK:
		return KEY_RSK;
		//END
		//KEY EVENT
	case E_M2MDC_KEY_EVENT_UP:
		return KEY_EVENT_UP;

	case E_M2MDC_KEY_EVENT_DOWN:
		return KEY_EVENT_DOWN;
		//END

		//ProtocolEvents MSG ID
	case E_M2MDC_MSG_MMI_CALL_RING_IND:				
		return MSG_ID_MMI_CC_CALL_RING_IND;

	case E_M2MDC_MSG_MMI_SMS_DELIVER_IND:			
		return MSG_ID_MMI_SMS_DELIVER_MSG_IND;

	case E_M2MDC_MSG_NW_GET_IMEI_REQ:				
		return MSG_ID_MMI_NW_GET_IMEI_REQ;

	case E_M2MDC_MSG_NW_GET_IMEI_RSP:
		return MSG_ID_MMI_NW_GET_IMEI_RSP;

	case E_M2MDC_PRT_GET_IMSI_REQ:					
		return PRT_GET_IMSI_REQ;

	case E_M2MDC_PRT_GET_IMSI_RSP:					
		return PRT_GET_IMSI_RSP;

	case E_M2MDC_MSG_EM_START_REQ:
#if (MEX_MTK_CODE_VERSION < 0x08B0)
		return MSG_ID_MMI_EM_START_REQ;
#else
		return 0;
#endif

	case E_M2MDC_MSG_EM_START_RSP:
#if (MEX_MTK_CODE_VERSION < 0x08B0)
		return MSG_ID_MMI_EM_START_RSP;
#else
		return 0;
#endif

	case E_M2MDC_MSG_EM_STATUS_IND:					
		return MSG_ID_MMI_EM_STATUS_IND;

	case E_M2MDC_MSG_EM_STOP_REQ:
#if (MEX_MTK_CODE_VERSION < 0x08B0)
		return MSG_ID_MMI_EM_STOP_REQ;
#else
		return 0;
#endif
	case E_M2MDC_GET_GPRS_DATA_ACCOUNT_REQ:			
		return PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;

	case E_M2MDC_GET_GPRS_DATA_ACCOUNT_RSP:		
		return PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP;

	case E_M2MDC_SET_GPRS_DATA_ACCOUNT_REQ:			
		return PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ;

	case E_M2MDC_SET_GPRS_DATA_ACCOUNT_RSP:
		return PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP;

	case E_M2MDC_MSG_GET_HOST_BY_NAME_IND:
		return MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND;

	case E_M2MDC_MSG_SOC_NOTIFY_IND:
		return MSG_ID_APP_SOC_NOTIFY_IND;
		//end

		//sms
	case E_M2MDC_SMSAL_MAX_MSG_LEN:
		return SMSAL_MAX_MSG_LEN;

	case E_M2MDC_SMSAL_MTI_DELIVER:
		return SMSAL_MTI_DELIVER;

	case E_M2MDC_SMSAL_MTI_SUBMIT:
		return SMSAL_MTI_SUBMIT;


	case E_M2MDC_SMSAL_DEFAULT_DCS:
		return SMSAL_DEFAULT_DCS;

	case E_M2MDC_SMSAL_UCS2_DCS:
		return SMSAL_UCS2_DCS;

	case E_M2MDC_SMSAL_DEFAULT_PID:
		return SMSAL_DEFAULT_PID;

	case E_M2MDC_SMS_APP_NOBOX:
#if (MEX_MTK_CODE_VERSION < 0x0952)
		return MMI_FRM_SMS_APP_NOBOX;
#else
		return SRV_SMS_BOX_NONE;
#endif
	case E_M2MDC_SMS_INVALID_INDEX:
		return SMS_INVALID_INDEX;

	case E_M2MDC_MAX_DIGITS_SMS:
		return MAX_DIGITS_SMS;

	case E_M2MDC_ENCODING_LENGTH:
		return ENCODING_LENGTH;

	case E_M2MDC_SMS_SEND_SAVE_NIL:
#if (MEX_MTK_CODE_VERSION < 0x0932)
		return SEND_SAVE_NIL;
#else
		return 0;
#endif
	case E_M2MDC_SEND_CASE_SEND_ONLY:
#if (MEX_MTK_CODE_VERSION < 0x0932)
		return SEND_CASE_SEND_ONLY;
#else
		return 0;
#endif

	case E_M2MDC_FRM_SMS_OK:							
#if (MEX_MTK_CODE_VERSION < 0x0952)
		return MMI_FRM_SMS_OK;
#else
		return SMS_OK;
#endif
		//end
		//ENCODE TYPE
	case E_M2MDC_ENCODE_ASCII:
		return 0;

	case E_M2MDC_ENCODE_UNICODE:
		return 1;

		/*	case E_M2MDC_ENCODE_H_ASCII:
		return 0x08;   
		case E_M2MDC_ENCODE_H_UNICODE:
		return 
		*/
		//END
		//INPUT TYPE   参考 wgui_categories_inputsenum.h /imeGprot.h
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	case E_M2MDC_INPUT_NUMERIC:
		return IMM_INPUT_TYPE_NUMERIC;

	case E_M2MDC_INPUT_ALPHANUMERIC_UPPERCASE:
		return IMM_INPUT_TYPE_ENGLISH_SENTENCE_UPPERCASE;

	case E_M2MDC_INPUT_ALPHANUMERIC_LOWERCASE:
		return IMM_INPUT_TYPE_ENGLISH_SENTENCE_LOWERCASE;

	case E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE:
		return IMM_INPUT_TYPE_SENTENCE;

	case E_M2MDC_INPUT_NUMERIC_PASSWORD:
		return IMM_INPUT_TYPE_NUMERIC_PASSWORD;

	case E_M2MDC_INPUT_ALPHANUMERIC_PASSWORD:
		return IMM_INPUT_TYPE_ALPHANUMERIC_PASSWORD;

	case E_M2MDC_INPUT_SM_PINYIN:
		return IMM_INPUT_TYPE_SENTENCE;

	case E_M2MDC_INPUT_SM_STROKE:
		return IMM_INPUT_TYPE_SENTENCE;

	case E_M2MDC_INPUT_SM_MULTITAP_PINYIN:
		return IMM_INPUT_TYPE_SENTENCE;
#else /*  MEX_PLATFORM__FOR_MTK23C  */
	case E_M2MDC_INPUT_NUMERIC:
		return INPUT_TYPE_NUMERIC;

	case E_M2MDC_INPUT_ALPHANUMERIC_UPPERCASE:
		return INPUT_TYPE_ALPHANUMERIC_UPPERCASE;

	case E_M2MDC_INPUT_ALPHANUMERIC_LOWERCASE:
		return INPUT_TYPE_ALPHANUMERIC_LOWERCASE;

	case E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE:
		return INPUT_TYPE_ALPHANUMERIC_SENTENCECASE;

	case E_M2MDC_INPUT_NUMERIC_PASSWORD:
		return INPUT_TYPE_NUMERIC_PASSWORD;

	case E_M2MDC_INPUT_ALPHANUMERIC_PASSWORD:
		return INPUT_TYPE_ALPHANUMERIC_PASSWORD;

	case E_M2MDC_INPUT_SM_PINYIN:
		return INPUT_TYPE_SM_PINYIN;

	case E_M2MDC_INPUT_SM_STROKE:
		return INPUT_TYPE_SM_STROKE;

	case E_M2MDC_INPUT_SM_MULTITAP_PINYIN:
		return INPUT_TYPE_SM_MULTITAP_PINYIN;
#endif /*  MEX_PLATFORM__FOR_MTK23C  */ 
		//END

		//EM_STRUCT   参考em_struct.h
	case E_M2MDC_RR_EM_CELL_SELECT_PARA_INFO:			
		return RR_EM_CELL_SELECT_PARA_INFO;

	case E_M2MDC_RR_EM_CHANNEL_DESCR_INFO:				
		return RR_EM_CHANNEL_DESCR_INFO;

	case E_M2MDC_RR_EM_CTRL_CHANNEL_DESCR_INFO:			
		return RR_EM_CTRL_CHANNEL_DESCR_INFO;

	case E_M2MDC_RR_EM_RACH_CTRL_PARA_INFO:				
		return RR_EM_RACH_CTRL_PARA_INFO;

	case E_M2MDC_RR_EM_LAI_INFO:							
		return RR_EM_LAI_INFO;

	case E_M2MDC_RR_EM_RADIO_LINK_COUNTER_INFO:
		return RR_EM_RADIO_LINK_COUNTER_INFO;

	case E_M2MDC_RR_EM_MEASUREMENT_REPORT_INFO:
		return RR_EM_MEASUREMENT_REPORT_INFO;

	case E_M2MDC_RR_EM_CONTROL_MSG_INFO:					
		return RR_EM_CONTROL_MSG_INFO;

	case E_M2MDC_RR_EM_TBF_INFO:
		return RR_EM_TBF_INFO;

	case E_M2MDC_RR_EM_GPRS_GENERAL_INFO:
		return RR_EM_GPRS_GENERAL_INFO;
		//END
		//sscstring 参考sscstringhandle.h
	case E_M2MDC_SSC_AUTO:
		return (int32)SSC_AUTO;

	case E_M2MDC_SSC_SCHINESE:
		return (int32)SSC_SCHINESE;

	case E_M2MDC_SSC_TCHINESE:
		return (int32)SSC_TCHINESE;

	case E_M2MDC_SSC_ENGLISH:
		return (int32)SSC_ENGLISH;

	case E_M2MDC_SSC_DEFAULT:
		return (int32)SSC_DEFAULT; 
		//END

		//FS
	case E_M2MDC_FS_READ_WRITE:
		return FS_READ_WRITE;

	case E_M2MDC_FS_READ_ONLY:
		return FS_READ_ONLY;

	case E_M2MDC_FS_OPEN_SHARED:
		return FS_OPEN_SHARED;

	case E_M2MDC_FS_OPEN_NO_DIR:
		return FS_OPEN_NO_DIR;

	case E_M2MDC_FS_OPEN_DIR:
		return FS_OPEN_DIR;

	case E_M2MDC_FS_CREATE:
		return FS_CREATE;

	case E_M2MDC_FS_CREATE_ALWAYS:
		return FS_CREATE_ALWAYS;

	case E_M2MDC_FS_COMMITTED:
		return FS_COMMITTED;

	case E_M2MDC_FS_CACHE_DATA:
		return FS_CACHE_DATA;

	case E_M2MDC_FS_LAZY_DATA:
		return FS_LAZY_DATA;

	case E_M2MDC_FS_NONBLOCK_MODE:
		return FS_NONBLOCK_MODE;

	case E_M2MDC_FS_PROTECTION_MODE:
		return FS_PROTECTION_MODE;

	case E_M2MDC_FS_NOBUSY_CHECK_MODE:
		return FS_NOBUSY_CHECK_MODE;

	case E_M2MDC_FS_DI_BASIC_INFO:
		return FS_DI_BASIC_INFO;

	case E_M2MDC_FS_DI_FREE_SPACE:
		return FS_DI_FREE_SPACE;

	case E_M2MDC_FS_DI_FAT_STATISTICS:
		return FS_DI_FAT_STATISTICS; 
		//END
		//GDI&LCD
	case E_M2MDC_GDI_COLOR_TRANSPARENT:
		return GDI_COLOR_TRANSPARENT;

	case E_M2MDC_CAMERA_PREVIEW_LCM_MAINLCD:
		return MDI_CAMERA_PREVIEW_LCM_MAINLCD;

	case E_M2MDC_GDI_LAYER_ENABLE_LAYER_0:
		return GDI_LAYER_ENABLE_LAYER_0;

	case E_M2MDC_GDI_LAYER_ENABLE_LAYER_1:
		return GDI_LAYER_ENABLE_LAYER_1;

	case E_M2MDC_GDI_LAYER_ROTATE_0:
		return GDI_LAYER_ROTATE_0;
		//END
		//CAMERA RESULT
	case E_M2MDC_MDI_RES_CAMERA_SUCCEED:
		return MDI_RES_CAMERA_SUCCEED;
		//END
		//mdi audio  参考 Mdi_audio.h
	case E_M2MDC_MDI_DEVICE_SPEAKER:
		return MDI_DEVICE_SPEAKER;

	case E_M2MDC_MDI_DEVICE_MICROPHONE:
		return MDI_DEVICE_MICROPHONE;

	case E_M2MDC_MDI_DEVICE_SPEAKER2:
		return MDI_DEVICE_SPEAKER2;

	case E_M2MDC_MDI_DEVICE_LOUDSPEAKER:
		return MDI_DEVICE_LOUDSPEAKER;

	case E_M2MDC_MDI_DEVICE_SPEAKER_BOTH:
		return MDI_DEVICE_SPEAKER_BOTH;

	case E_M2MDC_MDI_DEVICE_BT_HEADSET:
		return MDI_DEVICE_BT_HEADSET;

	case E_M2MDC_MDI_DEVICE_AUTO_SELECT:			
		//自动选择声音输出设备 可以参考audioPlayerMsgHandler.c中函数mmi_audply_do_portion_play_action_without_handle_result
		//调用mdi_audio_play_file_portion_with_vol_path的最后一个参数
		//
		return  MDI_AUD_PTH_EX(MDI_DEVICE_SPEAKER2);			

	case E_M2MDC_MDI_AUDIO_SUCCESS:
		return MDI_AUDIO_SUCCESS;

	case E_M2MDC_MDI_AUDIO_FAIL:						
		return MDI_AUDIO_FAIL;
	case E_M2MDC_MDI_AUDIO_END_OF_FILE:
		return MDI_AUDIO_END_OF_FILE;

	case E_M2MDC_MDI_AUDIO_TERMINATED:
		return MDI_AUDIO_TERMINATED;

	case E_M2MDC_MDI_AUDIO_BAD_FORMAT:
		return MDI_AUDIO_BAD_FORMAT;

	case E_M2MDC_MDI_FORMAT_WAV:						
		return MDI_FORMAT_WAV;

	case E_M2MDC_MDI_FORMAT_SMF:
		return MDI_FORMAT_SMF;

	case E_M2MDC_MEDIA_FORMAT_PCM_8K:
		return MEDIA_FORMAT_PCM_8K;
	case E_M2MDC_MEDIA_FORMAT_PCM_16K:
		return MEDIA_FORMAT_PCM_16K;

	case E_M2MDC_MEDIA_SUCCESS:
		return MEDIA_SUCCESS;
	case E_M2MDC_MEDIA_FAIL:
		return MEDIA_FAIL;

	case E_M2MDC_MED_RES_OK:
		return MED_RES_OK;
	case E_M2MDC_MED_RES_AUDIO_END:
		return MED_RES_AUDIO_END;

	case E_M2MDC_MDI_AUD_VOL_EX_NUM:
		return MDI_AUD_VOL_NUM;

		//参考文件devices.h
	case E_M2MDC_DEVICE_AUDIO_PLAY_INFINITE:
		return DEVICE_AUDIO_PLAY_INFINITE;

	case E_M2MDC_DEVICE_AUDIO_PLAY_ONCE:
		return DEVICE_AUDIO_PLAY_ONCE;

	case E_M2MDC_VOL_TYPE_MEDIA:
		return VOL_TYPE_MEDIA;

		//end		
		//SOC
	case E_M2MDC_SOCK_STREAM:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return SOC_SOCK_STREAM;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return SOCK_STREAM;
#else	//win32环境下的值  参考 winsock2.h
		return 1;
#endif

	case E_M2MDC_SOCK_DGRAM:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return SOC_SOCK_DGRAM;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return SOCK_DGRAM;
#else
		return 2;
#endif
	case E_M2MDC_SOCK_SMS:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return SOC_SOCK_SMS;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return SOCK_SMS;
#else
		return 3;
#endif

	case E_M2MDC_SOCK_RAW:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return SOC_SOCK_RAW;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return SOCK_RAW;
#else
		return 4;
#endif

	case E_M2MDC_SOC_OOBINLINE:
		return SOC_OOBINLINE; 

	case E_M2MDC_SOC_LINGER:
		return SOC_LINGER;

	case E_M2MDC_SOC_NBIO:
		return SOC_NBIO;

	case E_M2MDC_SOC_ASYNC:
		return SOC_ASYNC;

	case E_M2MDC_SOC_NODELAY:
		return SOC_NODELAY;

	case E_M2MDC_SOC_KEEPALIVE:
		return SOC_KEEPALIVE;

	case E_M2MDC_SOC_RCVBUF:
		return SOC_RCVBUF;

	case E_M2MDC_SOC_SENDBUF:
		return SOC_SENDBUF;

	case E_M2MDC_SOC_NREAD:
		return SOC_NREAD;

	case E_M2MDC_SOC_PKT_SIZE:
		return SOC_PKT_SIZE;

	case E_M2MDC_SOC_SILENT_LISTEN:
		return SOC_SILENT_LISTEN;

	case E_M2MDC_SOC_QOS:
		return SOC_QOS;

	case E_M2MDC_SOC_TCP_MAXSEG:
		return SOC_TCP_MAXSEG;

	case E_M2MDC_SOC_IP_TTL:
		return SOC_IP_TTL;

	case E_M2MDC_SOC_LISTEN_BEARER:
		return SOC_LISTEN_BEARER;

	case E_M2MDC_SOC_UDP_ANY_FPORT:
		return SOC_UDP_ANY_FPORT;

	case E_M2MDC_SOC_WIFI_NOWAKEUP:
		return SOC_WIFI_NOWAKEUP;

	case E_M2MDC_SOC_UDP_NEED_ICMP:
		return SOC_UDP_NEED_ICMP;

	case E_M2MDC_SOC_IP_HDRINCL:
		return SOC_IP_HDRINCL;

	case E_M2MDC_SOC_NON_AUTO_ACT_BEARER:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return 0;
#else
		return SOC_NON_AUTO_ACT_BEARER;
#endif
	case E_M2MDC_SOC_AUTO_DEACT_BEARER:
#if (MEX_MTK_CODE_VERSION >= 0x0824)	
		return 0;	
#else
		return SOC_AUTO_DEACT_BEARER;
#endif
	case E_M2MDC_SOC_READ:
		return SOC_READ;

	case E_M2MDC_SOC_WRITE:
		return SOC_WRITE;

	case E_M2MDC_SOC_ACCEPT:
		return SOC_ACCEPT;

	case E_M2MDC_SOC_CONNECT:
		return SOC_CONNECT;

	case E_M2MDC_SOC_CLOSE:
		return SOC_CLOSE;

	case E_M2MDC_SOC_SUCCESS:
		return SOC_SUCCESS;

	case E_M2MDC_SOC_ERROR:
		return SOC_ERROR;

	case E_M2MDC_SOC_WOULDBLOCK:
		return SOC_WOULDBLOCK;

	case E_M2MDC_SOC_LIMIT_RESOURCE:
		return SOC_LIMIT_RESOURCE;

	case E_M2MDC_SOC_INVALID_SOCKET:
		return SOC_INVALID_SOCKET;

	case E_M2MDC_SOC_INVALID_ACCOUNT:
		return SOC_INVALID_ACCOUNT;

	case E_M2MDC_SOC_NAMETOOLONG:
		return SOC_NAMETOOLONG;

	case E_M2MDC_SOC_ALREADY:
		return SOC_ALREADY;

	case E_M2MDC_SOC_OPNOTSUPP:
		return SOC_OPNOTSUPP;

	case E_M2MDC_SOC_CONNABORTED:
		return SOC_CONNABORTED;

	case E_M2MDC_SOC_INVAL:
		return SOC_INVAL;

	case E_M2MDC_SOC_PIPE:
		return SOC_PIPE;

	case E_M2MDC_SOC_NOTCONN:
		return SOC_NOTCONN;

	case E_M2MDC_SOC_MSGSIZE:
		return SOC_MSGSIZE;

	case E_M2MDC_SOC_BEARER_FAIL:
		return SOC_BEARER_FAIL;

	case E_M2MDC_SOC_CONNRESET:
		return SOC_CONNRESET;

	case E_M2MDC_SOC_DHCP_ERROR:
		return SOC_DHCP_ERROR;

	case E_M2MDC_SOC_IP_CHANGED:
		return SOC_IP_CHANGED;

	case E_M2MDC_SOC_ADDRINUSE:
		return SOC_ADDRINUSE;

	case E_M2MDC_SOC_CANCEL_ACT_BEARER:
		return SOC_CANCEL_ACT_BEARER;

	case E_M2MDC_MAX_SOCKET_NUM:
		return MAX_TCPIP_SOC_NUM;

	case E_M2MDC_PF_INET:
#if (MEX_MTK_CODE_VERSION >= 0x0824)
		return SOC_PF_INET;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return PF_INET;
#else
		return 0;
#endif

	case E_M2MDC_IPPROTO_IP:
#if (MEX_MTK_CODE_VERSION >= 0x0824)		
		return SOC_IPPROTO_IP;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
		return IPPROTO_IP;
#else
		return 0;
#endif
		//profile 参考 SettingProf.c
	case E_M2MDC_PROFILE_GENERAL:
		return MMI_PROFILE_GENERAL;

	case E_M2MDC_PROFILE_MEETING:
		return MMI_PROFILE_MEETING;

	case E_M2MDC_PROFILE_OUTDOOR:
		return MMI_PROFILE_OUTDOOR;

	case E_M2MDC_PROFILE_INDOOR:
		return MMI_PROFILE_INDOOR;

	case E_M2MDC_PROFILE_HEADSET:
		return MMI_PROFILE_HEADSET;

	case E_M2MDC_PROFILE_SILENT:
#ifdef __MMI_ALLSILENT_PROFILE__
		return MMI_PROFILE_ALLSILENT;
#else
		return MMI_PROFILE_SILENT;
#endif

	case E_M2MDC_PROFILE_BT:
		return MMI_PROFILE_BT;
		//end
		//date format  参考 DateAndTime.c
	case E_M2MDC_DF_DD_MMM_YYYY_H:
		return 0;

	case E_M2MDC_DF_DD_MM_YYYY_S:
		return 1;

	case E_M2MDC_DF_MM_DD_YYYY_S:
		return 2;

	case E_M2MDC_DF_YYYY_MM_DD_S:
		return 3;

	case E_M2MDC_DF_YYYY_MM_DD_H:
		return 4;

	case E_M2MDC_DF_MMM_DD_YYYY:
		return 5;
		//end
		//TextEncodeing   参考 Conversions.h
	case E_M2MDC_BIG_5_ENCODING:
		return BIG_5_ENCODING_TYPE;

	case E_M2MDC_GB2312_ENCODING:
		return GB2312_ENCODING_TYPE;
		//end

	case E_M2MDC_ALWAYS_ASK_SIM_INFO_1:    //参考文件 ConnectManageGProt.h   枚举 always_ask_sim_info_enum
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return CBM_SIM_ID_SIM1;
#else
			return ALWAYS_ASK_SIM_INFO_SIM1;
#endif
		}
	case E_M2MDC_ALWAYS_ASK_SIM_INFO_2:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return CBM_SIM_ID_SIM2;
#else
			return ALWAYS_ASK_SIM_INFO_SIM2;
#endif

		}

	case E_M2MDC_IPPROTO_ICMP:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_ICMP;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_ICMP;
#else
			return 1;
#endif
		}
	case E_M2MDC_IPPROTO_HOPOPTS:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_HOPOPTS;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_HOPOPTS;
#else
			return 0;
#endif
		}
	case E_M2MDC_IPPROTO_IGMP:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_IGMP;	
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_IGMP;
#else
			return 2;
#endif
		}
	case E_M2MDC_IPPROTO_IPV4:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_IPV4;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_IPV4;
#else
			return 4;
#endif
		}
	case E_M2MDC_IPPROTO_IPIP:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_IPIP;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_IPIP;
#else
			return 4;
#endif
		}
	case E_M2MDC_IPPROTO_TCP:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_TCP;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_TCP;
#else
			return 6;
#endif
		}
	case E_M2MDC_IPPROTO_UDP:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_UDP;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_UDP;
#else
			return 17;
#endif
		}
	case E_M2MDC_IPPROTO_RAW:
		{
#if (MEX_MTK_CODE_VERSION >= 0x0824)
			return SOC_IPPROTO_RAW;
#elif defined(__MTK_TARGET__) && !defined(WIN_SOC)
			return IPPROTO_RAW;
#else
			return 255;
#endif
		}  


		//mmi_frm_sms_app_msgbox_enum
	case E_M2MDC_SMS_APP_UNREAD:    
#if (MEX_MTK_CODE_VERSION < 0x0952)
		return MMI_FRM_SMS_APP_UNREAD;
#else
		return SRV_SMS_BOX_RESERVED;
#endif
	case E_M2MDC_SMS_APP_INBOX:     
#if (MEX_MTK_CODE_VERSION < 0x0952)  
		return MMI_FRM_SMS_APP_INBOX;
#else
		return SRV_SMS_BOX_INBOX;
#endif
	case E_M2MDC_SMS_APP_OUTBOX:     
#if (MEX_MTK_CODE_VERSION < 0x0952)   
		return MMI_FRM_SMS_APP_OUTBOX;
#else
		return SRV_SMS_BOX_OUTBOX;
#endif
	case E_M2MDC_SMS_APP_DRAFTS:    
#if (MEX_MTK_CODE_VERSION < 0x0952)   
		return MMI_FRM_SMS_APP_DRAFTS;
#else
		return SRV_SMS_BOX_DRAFTS;
#endif
	case E_M2MDC_SMS_APP_AWAITS:   
#if (MEX_MTK_CODE_VERSION < 0x0952)  
		return MMI_FRM_SMS_APP_AWAITS;
#else
		return SRV_SMS_BOX_RESERVED;
#endif
	case E_M2MDC_SMS_APP_DATA:     
#if (MEX_MTK_CODE_VERSION < 0x0952)   
		return MMI_FRM_SMS_APP_DATA;
#else
		return SRV_SMS_BOX_RESERVED;
#endif
		//end
	case E_M2MDC_KEY_VOL_UP:
		return KEY_VOL_UP;
	case E_M2MDC_KEY_VOL_DOWN:
		return KEY_VOL_DOWN;
	case E_M2MDC_KEY_SEND:
		return KEY_SEND;

#if MEX_SDK_VER >= 1005
	case E_M2MDC_MDI_FORMAT_MP3:
		return MDI_FORMAT_DAF;
	case E_M2MDC_MDI_FORMAT_AMR:
		return MDI_FORMAT_AMR;

	case E_M2MDC_MEDIA_FORMAT_AMR:
		return MEDIA_FORMAT_AMR;
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1012
	case E_M2MDC_KEY_CLEAR:
		return KEY_CLEAR;
#endif

#if defined(MEX_SUPPORT_KEY_EXTRA_1)
    case E_M2MDC_KEY_EXTRA_1:
		return KEY_EXTRA_1;
#endif

#ifdef __MET_MEX_SIM_PLUS__
	case E_M2MDC_MOD_SMS_3:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 3)
			return MOD_SMS_3;
		#else
			return -1;
		#endif
	case E_M2MDC_MOD_SMS_4:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 4)
			return MOD_SMS_4;
		#else
			return -1;
		#endif

	 case E_M2MDC_MOD_SIM_3:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 3)
					return MOD_SIM_3;
		#else
					return -1;
		#endif
			
	 case E_M2MDC_MOD_SIM_4:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 4)
				return MOD_SIM_4;
		#else
				return -1;
		#endif
	case E_M2MDC_MOD_L4C_3:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 3)
					return MOD_L4C_3;
		#else
					return -1;
		#endif
	case E_M2MDC_MOD_L4C_4:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 4)
						return MOD_L4C_4;
		#else
						return -1;
		#endif
	case E_M2MDC_MOD_PHB_3:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 3)
				return MOD_PHB_3;
		#else
				return -1;
		#endif
	case E_M2MDC_MOD_PHB_4:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 4)
					return MOD_PHB_4;
		#else
					return -1;
		#endif
	case E_M2MDC_MOD_AS_3:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 3)
				return MOD_AS_3;
		#else
				return -1;
		#endif
	case E_M2MDC_MOD_AS_4:
		#if defined(GEMINI_PLUS) && (GEMINI_PLUS >= 4)
				return MOD_AS_4;
		#else
				return -1;
		#endif
	case E_M2MDC_ALWAYS_ASK_SIM_INFO_3:
			{
		#if (MEX_MTK_CODE_VERSION >= 0x0824)
		
			#if (GEMINI_PLUS >= 3)
				return CBM_SIM_ID_SIM3;
			#endif
		#else
			return ALWAYS_ASK_SIM_INFO_SIM2;
		#endif
			
			}
		case E_M2MDC_ALWAYS_ASK_SIM_INFO_4:
			{
			#if (MEX_MTK_CODE_VERSION >= 0x0824)
					#if (GEMINI_PLUS >= 4)
						return CBM_SIM_ID_SIM4;
					#endif
			#else
					return ALWAYS_ASK_SIM_INFO_SIM2;
			#endif
					
			}

#endif/*__MET_MEX_SIM_PLUS__*/

	default:
        mex_log(MEX_LOG_SYSTEM,"Mex ConvertDef %d invalid",param);
		break;
	}
	return 0;
}
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_SDK_VER >= 1009

// FilemgrType.h mmi_fmgr_filetype_enum
int32 MexConfig_ConvertMtkFileType(E_Mex_Fmgr_Filtertype param)
{
	switch(param)
	{
	case E_MFF_TYPE_ALL:
		return FMGR_TYPE_ALL;
	case E_MFF_TYPE_UNKNOWN:
	#if (MEX_MTK_CODE_VERSION < 0x1032)
		return FMGR_TYPE_UNKNOW;
	#else
		return FMGR_TYPE_UNKNOWN;
	#endif
	case E_MFF_TYPE_FOLDER:
		return FMGR_TYPE_FOLDER;
	case E_MFF_TYPE_FOLDER_DOT:
		return FMGR_TYPE_FOLDER_DOT;

		// Image type

	case E_MFF_TYPE_BMP:
		return FMGR_TYPE_BMP;
	case E_MFF_TYPE_JPG:
		return FMGR_TYPE_JPG;
	case E_MFF_TYPE_JPEG:
		return FMGR_TYPE_JPEG;
	case E_MFF_TYPE_GIF:
		return FMGR_TYPE_GIF;
	case E_MFF_TYPE_PNG:
		return FMGR_TYPE_PNG;
	case E_MFF_TYPE_WBMP:
		return FMGR_TYPE_WBMP;
	case E_MFF_TYPE_WBM:
		return FMGR_TYPE_WBM;
	case E_MFF_TYPE_M3D:
		return FMGR_TYPE_M3D;
	case E_MFF_TYPE_EMS:
		return FMGR_TYPE_EMS;
	case E_MFF_TYPE_ANM:
		return FMGR_TYPE_ANM;
	case E_MFF_TYPE_SVG:
		return FMGR_TYPE_SVG;

		// Audio type
	case E_MFF_TYPE_IMY:
		return FMGR_TYPE_IMY;
	case E_MFF_TYPE_MID:
		return FMGR_TYPE_MID;
	case E_MFF_TYPE_MIDI:
		return FMGR_TYPE_MIDI;
	case E_MFF_TYPE_WAV:
		return FMGR_TYPE_WAV;
	case E_MFF_TYPE_AMR:
		return FMGR_TYPE_AMR;
	case E_MFF_TYPE_AAC:
		return FMGR_TYPE_AAC;
	case E_MFF_TYPE_MP3:
		return FMGR_TYPE_DAF;
	case E_MFF_TYPE_MP2:
		return FMGR_TYPE_MP2;
	case E_MFF_TYPE_VM:
		return FMGR_TYPE_VM;
	case E_MFF_TYPE_AWB:
		return FMGR_TYPE_AWB;
	case E_MFF_TYPE_AIF:
		return FMGR_TYPE_AIF;
	case E_MFF_TYPE_AIFF:
		return FMGR_TYPE_AIFF;
	case E_MFF_TYPE_AIFC:
		return FMGR_TYPE_AIFC;
	case E_MFF_TYPE_AU:
		return FMGR_TYPE_AU;
	case E_MFF_TYPE_SND:
		return FMGR_TYPE_SND;
	case E_MFF_TYPE_M4A:
		return FMGR_TYPE_M4A;
	case E_MFF_TYPE_MMF:
		return FMGR_TYPE_MMF;
	case E_MFF_TYPE_WMA:
		return FMGR_TYPE_WMA;

		// Video type
	case E_MFF_TYPE_3PG:
		return FMGR_TYPE_3GP;
	case E_MFF_TYPE_MP4:
		return FMGR_TYPE_MP4;
	case E_MFF_TYPE_MPG:
		return FMGR_TYPE_MPG;
	case E_MFF_TYPE_AVI:
		return FMGR_TYPE_AVI;
	case E_MFF_TYPE_3G2:
		return FMGR_TYPE_3G2;

	case E_MFF_TYPE_SDP:
		return FMGR_TYPE_SDP;
	case E_MFF_TYPE_RAM:
		return FMGR_TYPE_RAM;

	case E_MFF_TYPE_WML:
		return FMGR_TYPE_WML;
	case E_MFF_TYPE_HTML:
		return FMGR_TYPE_HTML;
	case E_MFF_TYPE_HTM:
		return FMGR_TYPE_HTM;
	case E_MFF_TYPE_XHTML:
		return FMGR_TYPE_XHTML;


	case E_MFF_TYPE_JAD:
		return FMGR_TYPE_JAD;
	case E_MFF_TYPE_JAR:
		return FMGR_TYPE_JAR;

	case E_MFF_TYPE_LRC:
		return FMGR_TYPE_LRC;
	case E_MFF_TYPE_THEME:
		return FMGR_TYPE_THEME;
	case E_MFF_TYPE_EBOOK_TXT:
#ifdef __MMI_EBOOK_READER__
		return FMGR_TYPE_EBOOK_TXT;
#else
        mex_log(MEX_LOG_SYSTEM,"Mex ConvertDef %d invalid",param);
		return 0;
#endif
	case E_MFF_TYPE_EBOOK_PDB:
#ifdef __MMI_EBOOK_READER__
		return FMGR_TYPE_EBOOK_PDB;
#else
        mex_log(MEX_LOG_SYSTEM,"Mex ConvertDef %d invalid",param);
		return 0;
#endif
	case E_MFF_TYPE_FOTA_FULL:
		return FMGR_TYPE_FOTA_FULL;

	case E_MFF_TYPE_URL:
		return FMGR_TYPE_URL;

	default:
	#if (MEX_MTK_CODE_VERSION < 0x1032)
		return FMGR_TYPE_UNKNOW;
	#else
		return FMGR_TYPE_UNKNOWN;
	#endif		
	}
}
#endif

uint8 g_mex_calllog_need_update = 1;  // 标记通话记录是否需要更新
uint8 g_mex_calllog_updating    = 0;  // 标记是否正在更新
uint8 g_mex_calllog_simid       = E_MEX_SIM_NONE;
#if defined(__MMI_CH_MIXED_CALL_LOG__)
// 支持Mixed Call Log
uint8 g_mex_calllog_calltype    = E_MCT_CALLLOG_MIXED;
#else
// 不支持Mixed Call Log
uint8 g_mex_calllog_calltype    = E_MCT_CALLLOG_MIXED + 1;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */

#ifdef MEX_PLATFORM__FOR_D450
// APIs for call log (Call History)
uint32 MexPhs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;

#ifdef __SLT_MMI_GEMINI_CALLLOG__
	extern call_history_context_struct *chis_p_slave;
#endif /*__SLT_MMI_GEMINI_CALLLOG__*/


	call_history_context_struct *mex_chis = NULL;

	if (simId == E_MEX_SIM_1)
	{
		mex_chis = chis_p;
	}

#ifdef __SLT_MMI_GEMINI_CALLLOG__
	if (simId == E_MEX_SIM_2)
	{
		mex_chis = chis_p_slave;
	}
#endif /*__SLT_MMI_GEMINI_CALLLOG__*/


	if (mex_chis == NULL)
	{
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return 0;
	}

	switch (callType)
	{
	case E_MCT_CALLLOG_DIALED:
		return mex_chis->nDialedCalls;
	case E_MCT_CALLLOG_MISSED:
		return mex_chis->nMissedCalls;
	case E_MCT_CALLLOG_RECVD:
		return mex_chis->nRecvdCalls;
	case E_MCT_CALLLOG_MIXED:
		return mex_chis->nMixedCalls;
	}

	return 0; // Default return 0
}

bool MexPhs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog)
{
	extern call_history_context_struct *chis_p;

#ifdef __SLT_MMI_GEMINI_CALLLOG__
	extern call_history_context_struct *chis_p_slave;
#endif /*__SLT_MMI_GEMINI_CALLLOG__*/

	call_history_context_struct *mex_chis = NULL;
	LOG_CALL *log = NULL;
	uint32 nLog = MexPhs_GetNumOfCallLog(simId, callType);

	if (simId == E_MEX_SIM_1)
	{
		mex_chis = chis_p;
	}

#ifdef __SLT_MMI_GEMINI_CALLLOG__
	if (simId == E_MEX_SIM_2)
	{
		mex_chis = chis_p_slave;
	}
#endif /*__SLT_MMI_GEMINI_CALLLOG__*/

	if (mex_chis == NULL)
	{
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return false;
	}

	if (index < nLog)
	{
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			log = &mex_chis->dialedCalls[index];
			break;
		case E_MCT_CALLLOG_MISSED:
			log = &mex_chis->missedCalls[index];
			break;
		case E_MCT_CALLLOG_RECVD:
			log = &mex_chis->recvdCalls[index];
			break;
		case E_MCT_CALLLOG_MIXED:
			log = mex_chis->mixedCalls[index];     // mixedCalls is a array of pointer
			break;
		}

		if (log)
		{
			uint32 len = 0;
			memset(callLog->number, 0, (MAX_CALLLOG_NUMBER_LENGTH+1) * sizeof(wchar_t));
			memset(callLog->pbName, 0, (MAX_CALLLOG_PBNAME_LENGTH+1) * sizeof(wchar_t));

			len = log->numLength;
			if (len > MAX_CALLLOG_NUMBER_LENGTH)
				len = MAX_CALLLOG_NUMBER_LENGTH;
			memcpy(callLog->number, log->number, len * 2);
			callLog->numberLen = len;

			len = log->nameLength;
			if (len > MAX_CALLLOG_PBNAME_LENGTH)
				len = MAX_CALLLOG_PBNAME_LENGTH;
			memcpy(callLog->pbName, log->pbName, len * 2);
			callLog->pbNameLen = len;

			callLog->startTime.year = log->startTime.nYear;
			callLog->startTime.month = log->startTime.nMonth;
			callLog->startTime.day = log->startTime.nDay;
			callLog->startTime.hour = log->startTime.nHour;
			callLog->startTime.minute = log->startTime.nMin;
			callLog->startTime.second = log->startTime.nSec;
			callLog->startTime.dayIndex = log->startTime.DayIndex;

			callLog->duration.year = log->duration.nYear;
			callLog->duration.month = log->duration.nMonth;
			callLog->duration.day = log->duration.nDay;
			callLog->duration.hour = log->duration.nHour;
			callLog->duration.minute = log->duration.nMin;
			callLog->duration.second = log->duration.nSec;
			callLog->duration.dayIndex = log->duration.DayIndex;

			callLog->nTimes = log->no_of_times;
			return true;
		}
	}

	return false; // Default return false
}

//获取通话记录模块的状态
//参数1：无
//返回：E_MEX_CALLOG_STATUS 通话记录模块的状态
E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	return (0); // not supported 
}

//更新通话记录
//参数1：无
//返回：是否成功
bool MexPhs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	return (0); // not supported
}


#endif /* MEX_PLATFORM__FOR_D450 */

#if defined(MEX_PLATFORM__FOR_D600)
// APIs for call log (Call History)
uint32 MexPhs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;
	uint32 i;
	uint32 num = 0;
	LOG_CALL *pLogCall = NULL;
	uint32    nLogCall = 0;
	bool isSlaveLog = false;

	// isSlaveLog
	switch (simId)
	{
	case E_MEX_SIM_1:
		isSlaveLog = false;
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_SIM_2:
		isSlaveLog = true;
		break;
#endif /* __SLT_DUAL_SIM_GEMINI__ */
	default:
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return 0;
	}

	// pLogCall & nLogCall
	switch (callType)
	{
	case E_MCT_CALLLOG_DIALED:
		pLogCall = chis_p->dialedCalls;
		nLogCall = chis_p->nDialedCalls;
		break;
	case E_MCT_CALLLOG_MISSED:
		pLogCall = chis_p->missedCalls;
		nLogCall = chis_p->nMissedCalls;
		break;
	case E_MCT_CALLLOG_RECVD:
		pLogCall = chis_p->recvdCalls;
		nLogCall = chis_p->nRecvdCalls;
		break;
	case E_MCT_CALLLOG_MIXED:

#ifdef __SLT_DUAL_SIM_GEMINI__
		for (i = 0; i < chis_p->nMixedCalls; i++)
		{
			if (chis_p->mixedCalls[i]->gemini_isslavelog == isSlaveLog)
				num++;
		}
#else  /* __SLT_DUAL_SIM_GEMINI__ */
#ifdef __MMI_CH_MIXED_CALL_LOG__
		num = chis_p->nMixedCalls;
#endif
#endif /* __SLT_DUAL_SIM_GEMINI__ */

		return num;
	default:
		mex_log(MEX_LOG_PHS, "E:wrong callType");
		return 0;
	}

#ifdef __SLT_DUAL_SIM_GEMINI__
	for (i = 0; i < nLogCall; i++)
	{
		if (pLogCall[i].gemini_isslavelog == isSlaveLog)
		{
			num++;
		}
	}
#else  /* __SLT_DUAL_SIM_GEMINI__ */
	num = nLogCall;
#endif /* __SLT_DUAL_SIM_GEMINI__ */

	return num;
}

bool MexPhs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog)
{
	extern call_history_context_struct *chis_p;
	uint32 i;
	uint32 num = 0;
	LOG_CALL *pLogCall = NULL;
	uint32    nLogCall = 0;
	LOG_CALL *log = NULL;
	bool isSlaveLog = false;

	// isSlaveLog
	switch (simId)
	{
	case E_MEX_SIM_1:
		isSlaveLog = false;
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_SIM_2:
		isSlaveLog = true;
		break;
#endif /* __SLT_DUAL_SIM_GEMINI__ */
	default:
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return false;
	}

	// pLogCall & nLogCall
	switch (callType)
	{
	case E_MCT_CALLLOG_DIALED:
		pLogCall = chis_p->dialedCalls;
		nLogCall = chis_p->nDialedCalls;
		break;
	case E_MCT_CALLLOG_MISSED:
		pLogCall = chis_p->missedCalls;
		nLogCall = chis_p->nMissedCalls;
		break;
	case E_MCT_CALLLOG_RECVD:
		pLogCall = chis_p->recvdCalls;
		nLogCall = chis_p->nRecvdCalls;
		break;
	case E_MCT_CALLLOG_MIXED:

#ifdef __SLT_DUAL_SIM_GEMINI__
		for (i = 0; i < chis_p->nMixedCalls; i++)
		{
			if (chis_p->mixedCalls[i]->gemini_isslavelog == isSlaveLog)
			{
				if (num == index)
				{
					log = chis_p->mixedCalls[i];
					break;
				}
				num++;
			}
		}
#else  /* __SLT_DUAL_SIM_GEMINI__ */
#ifdef __MMI_CH_MIXED_CALL_LOG__
		log = chis_p->mixedCalls[index];
#endif
#endif /* __SLT_DUAL_SIM_GEMINI__ */

		break;
	default:
		mex_log(MEX_LOG_PHS, "E:wrong callType");
		return false;
	}

#ifdef __SLT_DUAL_SIM_GEMINI__
	for (i = 0; i < nLogCall; i++)
	{
		if (pLogCall[i].gemini_isslavelog == isSlaveLog)
		{
			if (num == index)
			{
				log = &pLogCall[i];
				break;
			}
			num++;
		}
	}
#else  /* __SLT_DUAL_SIM_GEMINI__ */
	if (log == NULL)
		log = &pLogCall[index];
#endif /* __SLT_DUAL_SIM_GEMINI__ */

	if (log)
	{
		uint32 len = 0;
		memset(callLog->number, 0, (MAX_CALLLOG_NUMBER_LENGTH+1) * sizeof(wchar_t));
		memset(callLog->pbName, 0, (MAX_CALLLOG_PBNAME_LENGTH+1) * sizeof(wchar_t));

		len = log->numLength;
		if (len > MAX_CALLLOG_NUMBER_LENGTH)
			len = MAX_CALLLOG_NUMBER_LENGTH;
		memcpy(callLog->number, log->number, len * 2);
		callLog->numberLen = len;

		len = log->nameLength;
		if (len > MAX_CALLLOG_PBNAME_LENGTH)
			len = MAX_CALLLOG_PBNAME_LENGTH;
		memcpy(callLog->pbName, log->pbName, len * 2);
		callLog->pbNameLen = len;

		callLog->startTime.year = log->startTime.nYear;
		callLog->startTime.month = log->startTime.nMonth;
		callLog->startTime.day = log->startTime.nDay;
		callLog->startTime.hour = log->startTime.nHour;
		callLog->startTime.minute = log->startTime.nMin;
		callLog->startTime.second = log->startTime.nSec;
		callLog->startTime.dayIndex = log->startTime.DayIndex;

		callLog->duration.year = log->duration.nYear;
		callLog->duration.month = log->duration.nMonth;
		callLog->duration.day = log->duration.nDay;
		callLog->duration.hour = log->duration.nHour;
		callLog->duration.minute = log->duration.nMin;
		callLog->duration.second = log->duration.nSec;
		callLog->duration.dayIndex = log->duration.DayIndex;

		callLog->nTimes = log->no_of_times;
		return true;
	}


	return false; // Default return false
}

//获取通话记录模块的状态
//参数1：无
//返回：E_MEX_CALLOG_STATUS 通话记录模块的状态
E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;

	// flag for read call logs after boot up
	if (chis_p->isFirstTime == TRUE)
	{
		mex_log(MEX_LOG_PHS, "W:CALLLOG_BUSY");
		return E_MCS_CALLLOG_BUSY;
	}

	if (g_mex_calllog_updating)
	{
		return E_MCS_CALLLOG_UPDATING;
	}

	if (g_mex_calllog_need_update)
	{
		if (chis_p->isReqSent == TRUE ||
			chis_p->process != CALL_LOG_PROC_NONE)
		{
			return E_MCS_CALLLOG_BUSY;
		}
		return E_MCS_CALLLOG_NEED_UPDATE;
	}

	return E_MCS_CALLLOG_READY;
}

//更新通话记录
//参数1：无
//返回：是否成功
bool MexPhs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;

	if (g_mex_calllog_updating ||
		chis_p->isFirstTime == TRUE ||
		chis_p->isReqSent == TRUE ||
		chis_p->process != CALL_LOG_PROC_NONE)
	{
		return false;
	}

	if (g_mex_calllog_need_update)
	{
		g_mex_calllog_updating = 1;
#ifdef __MMI_CH_MIXED_CALL_LOG__
		chis_p->isMixedStyle = TRUE;
#endif
		ReqReadDialedNumByKey();
		return true;
	}

	return false;
}

#endif /* MEX_PLATFORM__FOR_D600 */

#if (MEX_MTK_CODE_VERSION >= 0x1032)

typedef struct
{
	SRV_CLOG_HANDLE        m_hClog;      // CLOG service handle
	E_MEX_Sim_Card_ID      m_eSimId;     // current sim
	E_MEX_CALLLOG_TYPE     m_eClogType;  // current call log type
	uint8                  m_bUpdating;  // updating call log ?
	uint32                 m_iNumOfClog; // number of call log
	SRV_CLOG_ID            m_iClogIds[SRV_CLOG_LOG_ID_LIST_MAX_NUM];  // id list....
} MexPhs_CallLog_Context;

static MexPhs_CallLog_Context g_mex_clog_context = { 0 };

E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);
static bool MexPhs_UpdateCallLog_Internal(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	int32 iRet;
	srv_clog_log_identity_struct identity = { 0 };
	srv_clog_num_info_struct num_info = { 0 };
	if(g_mex_clog_context.m_hClog == 0)
	{
        mex_log(MEX_LOG_SYSTEM,"Clog handle is NULL");
        return false;
	}
	identity.cate = SRV_CLOG_CATE_CALL;
	if (simId == E_MEX_SIM_1)
		identity.sim_id = MMI_SIM1;
	else
		identity.sim_id = MMI_SIM2;
	if (callType == E_MCT_CALLLOG_DIALED)
		identity.log_type = SRV_CLOG_CLT_DIALED;
	else if (callType == E_MCT_CALLLOG_MISSED)
		identity.log_type = SRV_CLOG_CLT_MISSED;
	else if (callType == E_MCT_CALLLOG_RECVD)
		identity.log_type = SRV_CLOG_CLT_RECVED;
	else if (callType == E_MCT_CALLLOG_MIXED)
		identity.log_type = SRV_CLOG_CLT_ALL;
	else
	{
		mex_log(MEX_LOG_PHS, "E: CLOG WT");
		return false;
	}
	identity.sub_type = SRV_CLOG_CALL_TYPE_ALL;
	iRet = srv_clog_get_list_num_info(
		g_mex_clog_context.m_hClog, 
		&identity,
		&num_info);
	if (iRet != SRV_CLOG_RET_OK) // err handle
	{
		mex_log(MEX_LOG_PHS, "E: CLOG GLN");
		return false;
	}
	g_mex_clog_context.m_iNumOfClog = num_info.num[SRV_CLOG_LOG_NUM_TYPE_TOTAL_LOGS];
	mex_log(MEX_LOG_PHS, "I: CLOG sim:%d ct:%d num:%d_%d", simId, callType, num_info.num[0], num_info.num[1]);
	
	// get id list...
	{
		srv_clog_get_list_log_ids_req_struct req_data;
		srv_clog_get_list_log_ids_cnf_struct cnf_data;
		
		srv_clog_init_para(SRV_CLOG_PARA_GET_LIST_LOG_IDS_REQ, &req_data);
		srv_clog_init_para(SRV_CLOG_PARA_GET_LIST_LOG_IDS_CNF, &cnf_data);
		
		SRV_CLOG_INIT_IDT(
			(srv_clog_log_identity_struct*) & req_data,
			identity.cate,
			identity.sim_id,
			identity.log_type,
			identity.sub_type);
		req_data.start_by = SRV_CLOG_BY_ID;
		req_data.log_num = SRV_CLOG_LOG_ID_LIST_MAX_NUM;
		if (g_mex_clog_context.m_iNumOfClog > SRV_CLOG_LOG_ID_LIST_MAX_NUM)
			g_mex_clog_context.m_iNumOfClog = SRV_CLOG_LOG_ID_LIST_MAX_NUM;
		req_data.start_elm = cnf_data.next_start_elm;
		iRet = srv_clog_get_list_log_ids(g_mex_clog_context.m_hClog, &req_data, &cnf_data);
		if (iRet != SRV_CLOG_RET_OK)
			g_mex_clog_context.m_iNumOfClog = 0;
		else
			memcpy(g_mex_clog_context.m_iClogIds, cnf_data.log_ids, cnf_data.log_num * sizeof(SRV_CLOG_ID));
	}
	g_mex_clog_context.m_bUpdating = false;
	return true;

}


uint32 MexPhs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	if (MexPhs_GetCallLogStatus(simId, callType) != E_MCS_CALLLOG_READY)
	{
		mex_log(MEX_LOG_PHS, "E: CLOG NR1");
		return 0;
	}
	if (MexPhs_UpdateCallLog_Internal(simId, callType))
		return g_mex_clog_context.m_iNumOfClog;
	else
		return 0;
}

bool MexPhs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog)
{
	if (MexPhs_GetCallLogStatus(simId, callType) != E_MCS_CALLLOG_READY)
	{
		mex_log(MEX_LOG_PHS, "E: CLOG NR2");
		return false;
	}
	if (index >= g_mex_clog_context.m_iNumOfClog)
	{
		mex_log(MEX_LOG_PHS, "E: CLOG WI");
		return false;
	}

	// get single clog by id...
	{
		int32 iRet;
		srv_clog_log_struct srv_clog = { 0 };
		iRet = srv_clog_get_log_by_id(
			g_mex_clog_context.m_hClog,
			g_mex_clog_context.m_iClogIds[index],
			SRV_CLOG_LOG_FIELD_ALL,
			&srv_clog);
		if (iRet != SRV_CLOG_RET_OK)
		{
			mex_log(MEX_LOG_PHS, "E: CLOG GET");
			return false;
		}
		mex_memset(callLog, 0, sizeof(Mex_CallLog_Info));
		mex_wcsncpy(callLog->number, srv_clog.data.call_log.cid, MAX_CALLLOG_NUMBER_LENGTH);
		callLog->numberLen = mex_wcslen(callLog->number);
		mex_wcsncpy(callLog->pbName, srv_clog.data.call_log.name, MAX_CALLLOG_PBNAME_LENGTH);
		callLog->pbNameLen = mex_wcslen(callLog->pbName);
		callLog->nTimes = srv_clog.data.call_log.total_num;

		// timestamp		
		{
			uint32 local_sec = 0;
			applib_time_struct time;
			local_sec = applib_dt_sec_utc_to_local(srv_clog.data.call_log.timestamp);
			applib_dt_utc_sec_2_mytime(local_sec, &time, MMI_FALSE);

			callLog->startTime.year     = time.nYear;
			callLog->startTime.month    = time.nMonth;
			callLog->startTime.day      = time.nDay;
			callLog->startTime.hour     = time.nHour;
			callLog->startTime.minute   = time.nMin;
			callLog->startTime.second   = time.nSec;
			callLog->startTime.dayIndex = time.DayIndex;
		}

		// duration
		{
			U32 val;
			applib_time_struct time = {0};
				
			val = srv_clog.data.call_log.duration % (24 * 60 * 60);
			time.nHour = (val / (60 * 60)) % 255;
			val = val % (60 * 60);
			time.nMin = val / 60;
			time.nSec = val % 60;
			
			callLog->duration.hour     = time.nHour;
			callLog->duration.minute   = time.nMin;
			callLog->duration.second   = time.nSec;
		}
	}
	return true;
}

E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	if (!srv_clog_is_ready())  // CLOG service is ready ?
	{
		return E_MCS_CALLLOG_BUSY;
	}

	if (g_mex_clog_context.m_hClog == 0 ||
		g_mex_clog_context.m_eSimId != simId ||
		g_mex_clog_context.m_eClogType != callType)
	{
		return E_MCS_CALLLOG_NEED_UPDATE;
	}

	if (g_mex_clog_context.m_bUpdating)
	{
		return E_MCS_CALLLOG_UPDATING;
	}

	return E_MCS_CALLLOG_READY;
}

bool MexPhs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	if (MexPhs_GetCallLogStatus(simId, callType) != E_MCS_CALLLOG_NEED_UPDATE)
	{
		return false;
	}
	// create CLOG service handle
	if (g_mex_clog_context.m_hClog == 0)
	{
		SRV_CLOG_HANDLE hClog;
		srv_clog_create_struct create;
		srv_clog_init_para(SRV_CLOG_PARA_CREATE, &create);
		create.user_data = &g_mex_clog_context;
		hClog = srv_clog_create(&create);
		if (SRV_CLOG_IS_ELM_INVALID(hClog))
		{
			mex_log(MEX_LOG_PHS, "E: CLOG CH");
			return false;
		}
		g_mex_clog_context.m_hClog = hClog;
	}
	
	g_mex_clog_context.m_eSimId = simId;
	g_mex_clog_context.m_eClogType = callType;
	g_mex_clog_context.m_bUpdating = true;

	// updating call log...
	if (MexPhs_UpdateCallLog_Internal(simId, callType))
	{
		return true;
	}

update_clog_err:
	if (g_mex_clog_context.m_hClog != 0)
		srv_clog_destroy(g_mex_clog_context.m_hClog);
	memset(&g_mex_clog_context, 0, sizeof(g_mex_clog_context));
	return false;
}

#elif (MEX_MTK_CODE_VERSION >= 0x0824)//defined(MEX_PLATFORM__FOR_MTK23C)


#if (MEX_MTK_CODE_VERSION >= 0x0932)

// 获取通话记录Response
void MexPhs_GetCallLogRsp(void* data, uint8 result)
{
	mex_log(MEX_LOG_PHS, "PGCLRsp ILT:%d res:%d", *(uint8*)data, result);

	OslMfree(data);

#ifdef MEX_DUAL_SIM
	if (g_mex_calllog_simid == E_MEX_SIM_2)
	{
		if (result == MTPNP_TRUE)
		{
			SlaveCHISTSortCallLogsByTime();
			g_mex_calllog_need_update = 0;
		}
		else
		{
			g_mex_calllog_need_update = 1;
		}
		g_mex_calllog_updating = 0;
	}
	else
#endif /* MEX_DUAL_SIM */
	{
		if (result == CALL_LOG_RESULT_OK)
		{
			CHISTSortCallLogsByTime();
			g_mex_calllog_need_update = 0;
		}
		else
		{
			g_mex_calllog_need_update = 1;
		}
		g_mex_calllog_updating = 0;
	}
	AlmEnableExpiryHandler();
	return;
}

// 获取通话记录Request
void MexPhs_GetCallLogReq(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	mex_log(MEX_LOG_PHS, "PGCLReq simId:%d callType:%d", simId, callType);
	g_mex_calllog_simid = simId;
	g_mex_calllog_calltype = callType;
	g_mex_calllog_updating = 1;

	// switch sim card
	Mex_SetPhoneADChannel(simId, E_MAC_CALL_CHANNEL);

	// set ad channel
#ifdef MEX_DUAL_SIM
	if (simId == E_MEX_SIM_2)
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_SLAVE);
	}
	else
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
	}
#endif	/* MEX_DUAL_SIM */

	// get call log req
	{
		extern void mmi_chist_read_call_log(uint8* type);
#ifdef MEX_DUAL_SIM
		extern void mmi_slave_chist_read_all_calls(uint8* type);
#endif /* MEX_DUAL_SIM */
		uint8* type = OslMalloc(sizeof(uint8));
		AlmDisableExpiryHandler();

		// call type
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			*type = mmi_chist_action_log_type_dialed;
			break;
		case E_MCT_CALLLOG_MISSED:
			*type = mmi_chist_action_log_type_missed;
			break;
		case E_MCT_CALLLOG_RECVD:
			*type = mmi_chist_action_log_type_recvd;
			break;
		case E_MCT_CALLLOG_MIXED:
			*type = mmi_chist_action_log_type_all;
			break;
		default:
			*type = mmi_chist_action_log_type_none;
			break;
		}

#ifdef MEX_DUAL_SIM
		if (simId == E_MEX_SIM_2)
		{
			mmi_slave_chist_add_action(
				1,
				(void*) type,
				(mmi_chist_action_func_ptr) mmi_slave_chist_read_all_calls,
				(mmi_chist_action_cbk_ptr) MexPhs_GetCallLogRsp);

		}
		else
#endif /* MEX_DUAL_SIM */
		{
			mmi_chist_add_action(
				1,
				(void*) type,
				(mmi_chist_action_func_ptr) mmi_chist_read_call_log,
				(mmi_chist_action_cbk_ptr) MexPhs_GetCallLogRsp);

		}
	}
}
#endif /* MEX_PLATFORM_FOR_MTK6253 */

// 获得某一类型通话记录的条数
// 参数1: [IN] simId 指定SIM卡
// 参数2: [IN] callType 通话记录的类型
// 返回: uint32 通话记录的条数
uint32 MexPhs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;
	extern E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);
	E_MEX_CALLOG_STATUS status = MexPhs_GetCallLogStatus(simId, callType);

	// check status
	if (status != E_MCS_CALLLOG_READY)
	{
		mex_log(MEX_LOG_PHS, "E:calllog not ready");
		return 0;
	}

	if (simId == E_MEX_SIM_1)
	{
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			return chis_p->nDialedCalls;
		case E_MCT_CALLLOG_MISSED:
			return chis_p->nMissedCalls;
		case E_MCT_CALLLOG_RECVD:
			return chis_p->nRecvdCalls;

#ifdef __MMI_CH_MIXED_CALL_LOG__
		case E_MCT_CALLLOG_MIXED:
			return chis_p->nMixedCalls;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */        
		}        
	}

#ifdef MEX_DUAL_SIM
	if (simId == E_MEX_SIM_2)
	{
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			return slave_chis_p->nDialedCalls;
		case E_MCT_CALLLOG_MISSED:
			return slave_chis_p->nMissedCalls;
		case E_MCT_CALLLOG_RECVD:
			return slave_chis_p->nRecvdCalls;

#ifdef __MMI_CH_MIXED_CALL_LOG__
		case E_MCT_CALLLOG_MIXED:
			return slave_chis_p->nMixedCalls;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */        
		}             
	}
#endif /*MEX_DUAL_SIM*/

	mex_log(MEX_LOG_PHS, "E:wrong simId");
	return 0; // Default return 0
}


// 读取一条通话记录
// 参数1: [IN] simId 指定SIM卡
// 参数2: [IN] callType 通话记录的类型
// 参数3: [OUT] callLog 指向通话记录的指针
// 返回: true读取成功, false读取失败
bool MexPhs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog)
{
	extern call_history_context_struct *chis_p;
	extern E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);
	E_MEX_CALLOG_STATUS status = MexPhs_GetCallLogStatus(simId, callType);

	LOG_CALL *log = NULL;
	uint32 nLog = MexPhs_GetNumOfCallLog(simId, callType);

	if(callLog == NULL)
	{
        mex_log(MEX_LOG_SYSTEM,"callLog Buffer is NULL");
        return false;
    }

	// check status
	if (status != E_MCS_CALLLOG_READY)
	{
		mex_log(MEX_LOG_PHS, "E:calllog not ready");
		return false;
	}

	// check index
	if (index >= nLog)
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return false;
	}

	if (simId == E_MEX_SIM_1)
	{
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			log = &chis_p->dialedCalls[index];
			break;
		case E_MCT_CALLLOG_MISSED:
			log = &chis_p->missedCalls[index];
			break;
		case E_MCT_CALLLOG_RECVD:
			log = &chis_p->recvdCalls[index];
			break;
#ifdef __MMI_CH_MIXED_CALL_LOG__        
		case E_MCT_CALLLOG_MIXED:
			log = chis_p->mixedCalls[index];     // mixedCalls is a array of pointer
			break;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */        
		}        
	}

#ifdef MEX_DUAL_SIM
	if (simId == E_MEX_SIM_2)
	{
		switch (callType)
		{
		case E_MCT_CALLLOG_DIALED:
			log = &slave_chis_p->dialedCalls[index];
			break;
		case E_MCT_CALLLOG_MISSED:
			log = &slave_chis_p->missedCalls[index];
			break;
		case E_MCT_CALLLOG_RECVD:
			log = &slave_chis_p->recvdCalls[index];
			break;
#ifdef __MMI_CH_MIXED_CALL_LOG__        
		case E_MCT_CALLLOG_MIXED:
			log = slave_chis_p->mixedCalls[index];     // mixedCalls is a array of pointer
			break;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */        
		}        
	}
#endif /*MEX_DUAL_SIM*/

	if (log != NULL)
	{
		uint32 len = 0;
		memset(callLog->number, 0, (MAX_CALLLOG_NUMBER_LENGTH+1) * sizeof(wchar_t));
		memset(callLog->pbName, 0, (MAX_CALLLOG_PBNAME_LENGTH+1) * sizeof(wchar_t));

		len = log->numLength;
		if (len > MAX_CALLLOG_NUMBER_LENGTH)
			len = MAX_CALLLOG_NUMBER_LENGTH;
		memcpy(callLog->number, log->number, len * 2);
		callLog->numberLen = len;

		len = log->nameLength;
		if (len > MAX_CALLLOG_PBNAME_LENGTH)
			len = MAX_CALLLOG_PBNAME_LENGTH;
		memcpy(callLog->pbName, log->pbName, len * 2);
		callLog->pbNameLen = len;

		callLog->startTime.year = log->startTime.nYear;
		callLog->startTime.month = log->startTime.nMonth;
		callLog->startTime.day = log->startTime.nDay;
		callLog->startTime.hour = log->startTime.nHour;
		callLog->startTime.minute = log->startTime.nMin;
		callLog->startTime.second = log->startTime.nSec;
		callLog->startTime.dayIndex = log->startTime.DayIndex;

		callLog->duration.year = log->duration.nYear;
		callLog->duration.month = log->duration.nMonth;
		callLog->duration.day = log->duration.nDay;
		callLog->duration.hour = log->duration.nHour;
		callLog->duration.minute = log->duration.nMin;
		callLog->duration.second = log->duration.nSec;
		callLog->duration.dayIndex = log->duration.DayIndex;

		callLog->nTimes = log->no_of_times;
		return true;
	}


	mex_log(MEX_LOG_PHS, "E:wrong simId");
	return false; // default return false
}

// 获取通话记录模块的状态
// 参数1: [IN] simId 指定SIM卡
// 参数2: [IN] callType 通话记录的类型
// 返回：E_MEX_CALLOG_STATUS 通话记录模块的状态
E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	// 1. 手机刚启动时-> E_MCS_CALLLOG_BUSY
	// 2. 正在更新或发送CALLLOG请求时-> E_MCS_CALLLOG_BUSY

	extern call_history_context_struct *chis_p;

	// flag for read call logs after boot up
	if (chis_p->isFirstTime == TRUE 
	#ifdef MEX_DUAL_SIM
		|| slave_chis_p->isFirstTime == TRUE
	#endif
		)
	{
		mex_log(MEX_LOG_PHS, "W:CALLLOG_BUSY first");
		return E_MCS_CALLLOG_BUSY;
	}

	if (g_mex_calllog_updating)
	{
		return E_MCS_CALLLOG_UPDATING;
	}

	// CALLLOG模块正在执行其它操作
	if (chis_p->isReqSent == TRUE ||
		chis_p->process != 0 
		#ifdef MEX_DUAL_SIM
		|| //CALL_LOG_PROCESS_ENUM CALL_LOG_PROC_NONE
		slave_chis_p->isReqSent == TRUE ||
		slave_chis_p->process != 0
		#endif
		)
	{
		mex_log(MEX_LOG_PHS, "W:CALLLOG_BUSY");
		return E_MCS_CALLLOG_BUSY;
	}

	// 支持Mixed Call Log
#if defined(__MMI_CH_MIXED_CALL_LOG__)
	if (g_mex_calllog_simid != simId ||
		g_mex_calllog_calltype != callType ||
		g_mex_calllog_need_update == 1)
	{
		return E_MCS_CALLLOG_NEED_UPDATE;
	}
	// 不支持Mixed Call Log
#else
	if (callType == E_MCT_CALLLOG_MIXED)
	{
		return E_MCS_CALLLOG_NOT_SUPPORTED;
	}

	if (g_mex_calllog_simid != simId ||
		g_mex_calllog_need_update == 1 ||
		g_mex_calllog_calltype != callType)
	{
		return E_MCS_CALLLOG_NEED_UPDATE;
	}
#endif /* __MMI_CH_MIXED_CALL_LOG__ */

	return E_MCS_CALLLOG_READY;
}

// 更新通话记录
// 参数1: [IN] simId 指定SIM卡
// 参数2: [IN] callType 通话记录的类型
// 返回：是否成功
bool MexPhs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	extern call_history_context_struct *chis_p;
	/*
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog need_update:%d", g_mex_calllog_need_update);
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog updating:%d", g_mex_calllog_updating);

	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim1 first_time:%d", chis_p->isFirstTime);
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim1 req_send:%d", chis_p->isReqSent);
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim1 process:%d", chis_p->process);

	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim2 first_time:%d", slave_chis_p->isFirstTime);
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim2 req_send:%d", slave_chis_p->isReqSent);
	mex_log(MEX_LOG_PHS, "Phs_UpdateCallLog sim2 process:%d", slave_chis_p->process);
	*/    
	if (g_mex_calllog_updating ||
		chis_p->isFirstTime == TRUE ||
		chis_p->isReqSent == TRUE ||
		chis_p->process != 0 
		#ifdef MEX_DUAL_SIM
		||
		slave_chis_p->isFirstTime == TRUE ||
		slave_chis_p->isReqSent == TRUE ||
		slave_chis_p->process != 0
		#endif
		)
	{
		mex_log(MEX_LOG_PHS, "E: calllog busy");
		return false;
	}


#if (MEX_MTK_CODE_VERSION >= 0x0932)
	if (g_mex_calllog_simid != simId ||
		g_mex_calllog_calltype != callType ||
		g_mex_calllog_need_update)
	{
		MexPhs_GetCallLogReq(simId, callType);
		return true;
	}
	return false;
#endif /* MEX_PLATFORM_FOR_MTK6253 */

	// 支持Mixed Call Log
#if defined(__MMI_CH_MIXED_CALL_LOG__)
	if (g_mex_calllog_simid != simId ||
		g_mex_calllog_calltype != callType ||
		g_mex_calllog_need_update)
	{
		g_mex_calllog_updating = 1;
		g_mex_calllog_simid = simId;
		g_mex_calllog_calltype = E_MCT_CALLLOG_MIXED;

#ifdef __MMI_CH_MIXED_CALL_LOG__
		chis_p->isMixedStyle = TRUE;
#endif /* __MMI_CH_MIXED_CALL_LOG__ */

		ReqReadDialedNumByKey();
		return true;
	}
	// 不支持Mixed Call Log
#else
	if (g_mex_calllog_simid != simId ||
		g_mex_calllog_calltype != callType ||
		g_mex_calllog_need_update) // Fix Bug #5402
	{
		g_mex_calllog_updating = 1;

		// 根据不同的CallType 发送不同的请求
		if (simId == E_MEX_SIM_1)
		{
			switch (callType)
			{
			case E_MCT_CALLLOG_DIALED:
				ReqReadDialedNumByKey();
				break;
			case E_MCT_CALLLOG_MISSED:
				ReqReadMissedNumByKey();
				break;
			case E_MCT_CALLLOG_RECVD:
				ReqReadRecvdNumByKey();
				break;
			default:
				g_mex_calllog_updating = 0;
				mex_log(MEX_LOG_PHS, "E:wrong callType");
				return false;
			}
		}
#ifdef MEX_DUAL_SIM
		else if (simId == E_MEX_SIM_2)
		{
			switch (callType)
			{
			case E_MCT_CALLLOG_DIALED:
				MTPNP_PFAL_chist_slave_req_read_dialed_num_by_key();
				break;
			case E_MCT_CALLLOG_MISSED:
				MTPNP_PFAL_chist_slave_req_read_missed_num_by_key();
				break;
			case E_MCT_CALLLOG_RECVD:
				MTPNP_PFAL_chist_slave_req_read_recvd_num_by_key();
				break;
			default:
				g_mex_calllog_updating = 0;
				mex_log(MEX_LOG_PHS, "E:wrong callType");
				return false;
			}
		}
#endif
		else
		{
			g_mex_calllog_updating = 0;
			mex_log(MEX_LOG_PHS, "E:wrong simId");
			return false;
		}

		g_mex_calllog_simid = simId;
		g_mex_calllog_calltype = callType;
		return true;
	}
#endif /* __MMI_CH_MIXED_CALL_LOG__ */

	mex_log(MEX_LOG_PHS, "E:no need to update");
	return false;
}


#endif

#endif // MTK






#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION < 0x0952)
//============================================================ Phone book interface start

extern mmi_phb_context_struct g_phb_cntx;
extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[MAX_PB_ENTRIES];
extern S8 pbName[(MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH];
extern S8 pbNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];

extern uint16 g_phb_name_index[MAX_PB_ENTRIES];   /* Sorting Order by Name Field. */
extern uint16 PhoneBookEntryCount;

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern S8 pbHomeNumber[(MAX_PB_NUMBER_LENGTH + 1 + 1) * ENCODING_LENGTH];      /* ANRA */
extern S8 pbCompanyName[(MAX_PB_COMPANY_LENGTH + 1) * ENCODING_LENGTH];        /* ANRB */
extern S8 pbEmailAddress[(MAX_PB_EMAIL_LENGTH + 1) * ENCODING_LENGTH];
extern S8 pbOfficeNumber[(MAX_PB_NUMBER_LENGTH + 1 + 1) * ENCODING_LENGTH];
extern S8 pbFaxNumber[(MAX_PB_NUMBER_LENGTH + 1 + 1) * ENCODING_LENGTH];       /* ANRC */
extern PHB_OPTIONAL_FIELDS_STRUCT PhoneBookOptionalFields;
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
extern S8 pbDay[3 * ENCODING_LENGTH];
extern S8 pbMon[3 * ENCODING_LENGTH];
extern S8 pbYear[6 * ENCODING_LENGTH];
#endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 

uint16         g_MexPhbGroupIndex;
Mex_Phs_Phb_CB g_MexPhbCallBack = NULL;
int32          g_MexPhbAppId = 0;
uint16         g_MexPhbCurrOp = 0;

static bool MexPhs_PhbCheckIndex(uint16 uIndex)
{
	// Find uIndex in g_phb_name_index
	uint16 i;
	for (i = 0; i < PhoneBookEntryCount; i++)
	{
		if (g_phb_name_index[i] == uIndex)
		{
			return true; // found
		}
	}
	return false; // not found
}


static uint16 MexPhs_GetPhbEntryGroupIndex(uint16 uIndex)
{
	uint16 groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;
	uint8 optid_record;
	uint8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT *PhbOptIDs = mex_malloc(sizeof(PHB_OPTIONAL_IDS_STRUCT) * OPTIONAL_IDS_RECORD_TOTAL);

	if (PhbOptIDs)
	{
		optid_record = (uIndex / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = uIndex - (optid_record - 1) * OPTIONAL_IDS_RECORD_TOTAL;

		ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
		groupIndex = PhbOptIDs[optid_index].callerGroupID - 1;

		mex_free(PhbOptIDs);
	}

	return groupIndex;
}

static void MexPhs_SetPhbEntryGroupIndex(uint16 uIndex, uint16 groupIndex)
{
	uint8 optid_record;
	uint8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT *PhbOptIDs;
	if (groupIndex >= MAX_PB_CALLER_GROUPS)
		groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;

	PhbOptIDs = mex_malloc(sizeof(PHB_OPTIONAL_IDS_STRUCT) * OPTIONAL_IDS_RECORD_TOTAL);
	if (PhbOptIDs)
	{
		optid_record = (uIndex / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = uIndex - (optid_record - 1) * OPTIONAL_IDS_RECORD_TOTAL;

		ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
		PhbOptIDs[optid_index].callerGroupID = groupIndex  + 1;
		WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

		mex_free(PhbOptIDs);
	}

}

// APIs for PhoneBook
E_Mex_Phs_Phb_Result MexPhs_GetPhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	extern uint16 PhoneBookEntryCount;  
	bool bPermission = false;
 	if((psName == NULL) || (psNumber == NULL))
 	{
        mex_log(MEX_LOG_SYSTEM,"Can't get entry from NULL PHB string");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
 	}

	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookRead);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	// Check uIndex    
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	// Init
	mex_memset(psName, 0, sizeof(Mex_Phs_Phb_Name));
	mex_memset(psNumber, 0, sizeof(Mex_Phs_Phb_Number));

	// Get pbName and Number
	{
		uint32 len;
		extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[MAX_PB_ENTRIES];
		extern void mmi_phb_convert_get_ucs2_number(S8 *pString, uint16 index);

		len = PhoneBook[uIndex].alpha_id.name_length;
		if (len > MEX_H_PHS_PHB_NAME_LEN)
			len = MEX_H_PHS_PHB_NAME_LEN;
		mex_memcpy(psName->name, PhoneBook[uIndex].alpha_id.name, len + len);
		psName->nameLen = len;

		len = PhoneBook[uIndex].tel.length;
		if (len > MEX_H_PHS_PHB_NUMBER_LEN)
			len = MEX_H_PHS_PHB_NUMBER_LEN;
		mmi_phb_convert_get_ucs2_number((S8 *)psNumber->number, uIndex);
		psNumber->number[len] = L'\0';
		psNumber->numberLen = len;
	}
	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_GetPhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{
	PHB_OPTIONAL_IDS_STRUCT *opt_ids = NULL;
	extern uint16 PhoneBookEntryCount;
	extern mmi_phb_context_struct g_phb_cntx;
#if defined(MEX_PLATFORM__FOR_D600)||defined(MEX_PLATFORM__FOR_MTK23C)
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	extern PHB_OPTIONAL_FIELDS_STRUCT PhoneBookOptionalFields;
#endif 
#endif /* MEX_PLATFORM__FOR_D600 */

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	// Init
	mex_memset(psEntry, 0, sizeof(Mex_Phs_Phb_Entry));

	// Get Entry Detail Info
	{
		// Name and Number
		E_Mex_Phs_Phb_Result phbRes;
		phbRes = MexPhs_GetPhbEntry(uIndex, &psEntry->name, &psEntry->number);
		if (phbRes != E_MEX_PHB_RESULT_SUCCESS)
			return phbRes;

		psEntry->groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;

		// Record store in SIM
		if (uIndex >= MAX_PB_PHONE_ENTRIES)
		{
#ifdef MEX_DUAL_SIM
			// SIM2
			if (uIndex >= MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES)
			{
				psEntry->location = E_MEX_PHB_LOCATION_SIM2;

#ifdef MEX_DUAL_SIM

#endif /* MEX_DUAL_SIM */    
			}
			// SIM1
			else
#endif /* MEX_DUAL_SIM */
			{
				psEntry->location = E_MEX_PHB_LOCATION_SIM1;
			}

#if defined(MEX_PLATFORM__FOR_D600)||defined(MEX_PLATFORM__FOR_MTK23C)
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)     /* Associate caller group in SIM card entry */
			mmi_phb_build_caller_group_list();
			mmi_phb_read_optional_ids(uIndex);
			opt_ids = (PHB_OPTIONAL_IDS_STRUCT*) g_phb_cntx.optional_ids;
			if (opt_ids)
				psEntry->groupIndex = opt_ids->callerGroupID;            
#endif /* __MMI_PHB_CALLERGROUP_IN_SIM__ */
#endif /* MEX_PLATFORM__FOR_D600 */

		}
		// Record store in Phone
		else
		{
			// typedef struct
			// {
			//  + name
			//  + number;
			//     number_home;
			//     number_company;
			//     number_fax;
			//     name_company;
			//     email;
			//     birthday;
			//  + groupIndex;
			//  + location;
			// } Mex_Phs_Phb_Entry;



			S16 pError;

			psEntry->location = E_MEX_PHB_LOCATION_PHONE;
			// Get optional ids
			psEntry->groupIndex = MexPhs_GetPhbEntryGroupIndex(uIndex);

			// Get Optional fields
#if defined(MEX_PLATFORM__FOR_D600)||defined(MEX_PLATFORM__FOR_MTK23C)
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			ReadRecord(
				NVRAM_EF_PHB_FIELDS_LID,
				(uint16) (uIndex + 1),
				(void*)&PhoneBookOptionalFields,
				OPTIONAL_FIELDS_RECORD_SIZE,
				&pError);
			mmi_asc_to_ucs2((S8 *)psEntry->number_home.number, (PS8) PhoneBookOptionalFields.homeNumber);
			psEntry->number_home.numberLen = mex_wcslen((const wchar_t *) psEntry->number_home.number);
			mmi_ucs2cpy((S8 *)psEntry->name_company.name, (PS8) PhoneBookOptionalFields.companyName);
			psEntry->name_company.nameLen = mex_wcslen((const wchar_t *) psEntry->name_company.name);
			mex_strcpy(psEntry->email.address, (char *)PhoneBookOptionalFields.emailAddress);
			psEntry->email.addressLen = mex_wcslen((const wchar_t *) psEntry->email.address);
			mmi_asc_to_ucs2((S8 *)psEntry->number_company.number, (PS8) PhoneBookOptionalFields.officeNumber);
			psEntry->number_company.numberLen = mex_wcslen(psEntry->number_company.number);
			mmi_asc_to_ucs2((S8 *)psEntry->number_fax.number, (PS8) PhoneBookOptionalFields.faxNumber);
			psEntry->number_fax.numberLen = mex_wcslen(psEntry->number_fax.number);
#endif /* __MMI_PHB_NO_OPTIONAL_FIELD__ */
#endif /* MEX_PLATFORM__FOR_D600 */

			// Get Birthday field
#if defined(MEX_PLATFORM__FOR_D600)||defined(MEX_PLATFORM__FOR_MTK23C)
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			{
				uint16 bday_record, bday_index;
				int16 pError;
				MMI_PHB_BDAY_STRUCT *bday_data = mex_malloc(sizeof(MMI_PHB_BDAY_STRUCT) * NVRAM_EF_PHB_BIRTHDAY_COUNT);

				if (bday_data)
				{
					bday_record = (uIndex / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
					bday_index = uIndex - (bday_record - 1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;

					ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

					/* Year */
					if (bday_data[bday_index].bYear <= MMI_PHB_BDAY_MAX_YEAR_INT)
					{
						psEntry->birthday.year = bday_data[bday_index].bYear;
					}

					/* Month */
					if (bday_data[bday_index].bMonth <= 12)
					{
						psEntry->birthday.month = bday_data[bday_index].bMonth;
					}

					/* Day */
					if (bday_data[bday_index].bDay <= 31)
					{
						psEntry->birthday.day = bday_data[bday_index].bDay;
					}

					mex_free(bday_data);
				}
			}
#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */
#endif /* MEX_PLATFORM__FOR_D600 */
		}

	}
	return E_MEX_PHB_RESULT_SUCCESS;
}

#if defined(MEX_PLATFORM__FOR_D600)

/* flag used to store current input mode */
extern uint8 MMI_current_input_mode;
// 1. INPUT_MODE_MULTITAP_UPPERCASE_ABC 非拼音
// 2. INPUT_MODE_MMI_MULTITAP_PINYIN 拼音

#endif

uint8 MexPhs_SearchPhb_GetInputMode(void)
{
#if (MEX_MTK_CODE_VERSION < 0x0824)
#if defined(MEX_PLATFORM__FOR_D600)
	return MMI_current_input_mode;
#endif
#else
	return mmi_imm_get_curr_input_mode();
#endif
}
void MexPhs_SearchPhb_SetInputMode(uint8 mode)
{
#if (MEX_MTK_CODE_VERSION < 0x0824)
#if defined(MEX_PLATFORM__FOR_D600)
	MMI_current_input_mode = mode;
#endif
#else
	mmi_imm_create_input_method_list(IMM_INPUT_TYPE_SENTENCE, NULL);
	mmi_imm_set_current_input_mode(mode);
#endif

}

void MexPhs_SearchPhb_ChangeInputMode(bool isPinyin)
{

#if (MEX_MTK_CODE_VERSION < 0x0824)
#if defined(MEX_PLATFORM__FOR_D600)
	if (isPinyin)
	{
		MMI_current_input_mode = INPUT_MODE_MMI_MULTITAP_PINYIN;
	}
	else
	{
		MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
	}
#endif
#else
	mmi_imm_create_input_method_list(IMM_INPUT_TYPE_QUICK_SEARCH, NULL);

	if (isPinyin)
	{
		mmi_imm_set_current_input_mode(IMM_INPUT_MODE_QUICK_SEARCH_PINYIN);
	}
	else
	{
		mmi_imm_set_current_input_mode(IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	}
#endif
}

void MexPhs_SearchPhb_MakeCache(void)
{
	extern void MexPhs_SearchPhb_Prepare(void);
	MexPhs_SearchPhb_Prepare();
	#if defined(__MMI_PHB_QUICK_SEARCH__)
	mmi_phb_quick_search_make_cache();
	#endif
}

int32 MexPhs_SearchPhbEntry(wchar_t* szKeyword, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask)
{
	extern uint16 g_phb_name_index[MAX_PB_ENTRIES];
	extern uint16 PhoneBookEntryCount;
	extern void MexPhs_SearchPhb_GetSearchBitmask(uint8** ppuSearchBitmask);
	int32 res_count = 0;

#if defined(__MMI_PHB_QUICK_SEARCH__)
	res_count = mmi_phb_quick_search_find_entry((uint8 *) szKeyword);
#endif
	MexPhs_SearchPhb_GetSearchBitmask(ppuSearchBitmask);

	// Location Filter
	{
		uint16 i, idxFrom, idxTo;
		uint8 *bitmask = *ppuSearchBitmask;

		switch (eLocation)
		{
		case E_MEX_PHB_LOCATION_PHONE:
			idxFrom = 0;
			idxTo = idxFrom + MAX_PB_PHONE_ENTRIES - 1;
			break;
		case E_MEX_PHB_LOCATION_SIM1:
			idxFrom = MAX_PB_PHONE_ENTRIES;
			idxTo = idxFrom + MAX_PB_SIM_ENTRIES - 1;
			break;
#ifdef MEX_DUAL_SIM
		case E_MEX_PHB_LOCATION_SIM2:
			idxFrom = MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES;
			idxTo = idxFrom + MAX_PB_SIM_ENTRIES - 1;
			break;
#endif /* MEX_DUAL_SIM */
#ifdef __MET_MEX_SIM_PLUS__
		case E_MEX_PHB_LOCATION_SIM3:
			g_phb_cntx.selected_storage = MMI_SIM3;
			break;
		
		case E_MEX_PHB_LOCATION_SIM4:
			g_phb_cntx.selected_storage = MMI_SIM4;
			break;
#endif/*__MET_MEX_SIM_PLUS__*/
		default:
			goto search_end;
		}

		for (i = 0; i < PhoneBookEntryCount; i++)
		{
			if(bitmask[i >> 3] & (1 << (i % 8)))
			{
				uint16 idx = g_phb_name_index[i];
				if (idx >= idxFrom && idx <= idxTo)
				{
					// do nothing
				}
				else
				{
					// 过滤掉
					bitmask[i >> 3] &= (uint8) (~(1 << (i % 8)));
					res_count--;
				}
			}
		}
	}

search_end:
	return res_count;
}



static void MexPhs_InitPhbGlobalVars(Mex_Phs_Phb_Entry* sEntry)
{
	mex_wcscpy((wchar_t *) pbName, sEntry->name.name);
	mex_wcscpy((wchar_t *) pbNumber, sEntry->number.number);

	// 初始化可选字段
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	mex_wcscpy((wchar_t *) pbHomeNumber, sEntry->number_home.number);
	mex_wcscpy((wchar_t *) pbCompanyName, sEntry->name_company.name);
	mex_str2nwcs((wchar_t *)pbEmailAddress, MAX_PB_EMAIL_LENGTH << 1, sEntry->email.address);
	mex_wcscpy((wchar_t *) pbOfficeNumber, sEntry->number_company.number);
	mex_wcscpy((wchar_t *) pbFaxNumber, sEntry->number_fax.number);
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
	// 初始化生日
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
	{
		S8 ascii_buff[6];
		sprintf(ascii_buff, "%04d", sEntry->birthday.year);
		mmi_asc_to_ucs2(pbYear, ascii_buff);

		sprintf(ascii_buff, "%02d", sEntry->birthday.month);
		mmi_asc_to_ucs2(pbMon, ascii_buff);

		sprintf(ascii_buff, "%02d", sEntry->birthday.day);
		mmi_asc_to_ucs2(pbDay, ascii_buff);
	}
#endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 

	g_MexPhbGroupIndex = sEntry->groupIndex;
}

static void MexPhs_AddPhbEntryExRsp(void *info)
{
	uint16 store_index;
	MSG_PHB_SET_ENRTY_RSP_STRUCT *localPtr;
	uint8 flag = 0;
	uint8 num_ascii[MAX_PB_NUMBER_LENGTH + 1 + 1];

	// for phb call back
	E_Mex_Phs_Phb_Event cb_event;
	uint16 cb_index;

	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT*) info;
	if (localPtr->result.flag == 0) /* Set Entry Success */
	{
		// 添加条目
		if (g_phb_cntx.current_op == MMI_PHB_OP_ADD)
		{
			//mex_log(MEX_LOG_PHS, "Phs_AddPhbEntryExRsp Success");
#if defined(MEX_DUAL_SIM)&& (MEX_MTK_CODE_VERSION < 0x0824)
			if (g_phb_cntx.selected_storage == MMI_SIM2)
			{
				g_phb_cntx.sim2_used++;
				store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES - 1;
			}
			else
#endif /* MEX_DUAL_SIM */
			{
				store_index = mmi_phb_op_get_store_index_and_increase_counter(localPtr->list[0].storage, localPtr->list[0].record_index);
			}
			cb_event = E_MEX_PHB_EVENT_ADDENTRY_SUCCESS;
			cb_index = store_index;
			// 分三种情况处理[PHONE SIM1 SIM2]
		}
		// 编辑条目
		else if (g_phb_cntx.current_op == MMI_PHB_OP_EDIT)
		{
			//mex_log(MEX_LOG_PHS, "Phs_UpdatePhbEntryExRsp Success");
			store_index = g_phb_cntx.active_index;
			cb_event = E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS;
			cb_index = store_index;
		}
		else
		{
			mex_log(MEX_LOG_PHS, "E:unexpected operation:%d", g_phb_cntx.current_op);
			return;
		}

		// 保存在手机端
		if (localPtr->list[0].storage == MMI_NVRAM)
		{
			// 可选字段
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			if (mmi_ucs2strlen(pbHomeNumber))
			{
				mmi_phb_number_remove_invalid_character(pbHomeNumber);
			}
			if (mmi_ucs2strlen(pbOfficeNumber))
			{
				mmi_phb_number_remove_invalid_character(pbOfficeNumber);
			}
			if (mmi_ucs2strlen(pbFaxNumber))
			{
				mmi_phb_number_remove_invalid_character(pbFaxNumber);
			}
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
			mmi_phb_op_update_optional_field(store_index, MMI_NVRAM, 1);

			/* Populate Optional Field Number */
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			flag = mmi_phb_op_increase_lookup_table(
				store_index,
				NULL,
				(S8*) PhoneBookOptionalFields.faxNumber,
				(S8*) PhoneBookOptionalFields.homeNumber,
				(S8*) PhoneBookOptionalFields.officeNumber);
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */ 

			// Caller group
			MexPhs_SetPhbEntryGroupIndex(store_index, g_MexPhbGroupIndex);
		}

		/* Populate Number Field */
		mmi_ucs2_to_asc((PS8) num_ascii, (PS8) pbNumber);
		flag += mmi_phb_op_increase_lookup_table(store_index, (S8*) num_ascii, NULL, NULL, NULL);

		if (flag)
		{
			mmi_phb_lookup_table_sort();
		}

		mmi_phb_op_set_entry_rsp_name_number(&localPtr->list[0], store_index);

		if (localPtr->list[0].storage == MMI_SIM)
		{
			mmi_phb_op_update_field_flag(
				store_index,
				(S8*) localPtr->list[0].tel.number,
				NULL,
				NULL,
				NULL,
				NULL,
				TRUE);
		}
		else /* In NVRAM */
		{
#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			mmi_phb_op_update_field_flag(
				store_index,
				(S8*) localPtr->list[0].tel.number,
				NULL,
				NULL,
				NULL,
				NULL,
				TRUE);
#else /* defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */ 
			mmi_phb_op_update_field_flag(
				store_index,
				(S8*) localPtr->list[0].tel.number,
				(S8*) PhoneBookOptionalFields.homeNumber,
				(S8*) PhoneBookOptionalFields.officeNumber,
				(S8*) PhoneBookOptionalFields.faxNumber,
				(S8*) PhoneBookOptionalFields.emailAddress,
				TRUE);
#endif /* defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */ 

			/* Save birthday field */
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			mmi_phb_bday_update_field(store_index, (S8*) pbYear, (S8*) pbMon, (S8*) pbDay);
#endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */

		}
		if (PhoneBook[store_index].alpha_id.name_length)
		{
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NAME;
		}
		/* Update Pinyin Cache for sort */
#if defined(__MMI_PHB_PINYIN_SORT__)
		mmi_phb_util_make_pinyin_cache(store_index);
#endif 

		// MMI_PHB_OP_ADD
		if (g_phb_cntx.current_op == MMI_PHB_OP_ADD)
		{
			/* Insert name to name mapping table. */
			mmi_phb_sort_insert_name_index(store_index);
		}
		// MMI_PHB_OP_EDIT
		else
		{
			/* Name sorting mapping table should Re-Sort HERE, and get the new highlight position!!! */
			mmi_phb_op_delete_update_index(store_index);
			g_phb_cntx.new_highlight_entry = mmi_phb_sort_insert_name_index(store_index);
		}

		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;
	}
	else    /* Set Record Fail. */
	{
		if (g_phb_cntx.current_op == MMI_PHB_OP_ADD)
		{
			mex_log(MEX_LOG_PHS, "E:AddPhb Fail");
			cb_event = E_MEX_PHB_EVENT_ADDENTRY_FAIL;
			cb_index = MEX_H_PHS_PHB_ENTRYINDEX_INVALID;
		}
		else
		{
			mex_log(MEX_LOG_PHS, "E:UpdatePhb Fail");
			cb_event = E_MEX_PHB_EVENT_UPDATEENTRY_FAIL;
			cb_index = g_phb_cntx.active_index;
		}
	}

	g_phb_cntx.current_op = MMI_PHB_OP_NONE;
	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(cb_index, cb_event);
		mex_SetCurrentApp(appId);
	}
}


E_Mex_Phs_Phb_Result MexPhs_AddPhbEntryEx(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* sEntry)
{
	extern E_Mex_Phs_Phb_Result mex_Phs_GetPhbMemoryStatus(E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed);

	uint32 total, used;
	bool bPermission = false;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookAdd);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check space
	mex_Phs_GetPhbMemoryStatus(eLocation, &total, &used);
	if (used >= total)
	{
		mex_log(MEX_LOG_PHS, "E:no free sapce at eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_NO_SPACE;
	}
	// Check current operation
	if (g_phb_cntx.current_op != MMI_PHB_OP_NONE || g_MexPhbCurrOp != MMI_PHB_OP_NONE)
	{
		mex_log(MEX_LOG_PHS, "E:current_op:%d", g_phb_cntx.current_op);
		return E_MEX_PHB_RESULT_BUSY;
	}

	switch (eLocation)
	{
	case E_MEX_PHB_LOCATION_PHONE:
		g_phb_cntx.selected_storage = MMI_NVRAM;
		break;
	case E_MEX_PHB_LOCATION_SIM1:
		g_phb_cntx.selected_storage = MMI_SIM;
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_PHB_LOCATION_SIM2:
		g_phb_cntx.selected_storage = MMI_SIM2;
		break;
#endif /* MEX_DUAL_SIM */
	default:
		mex_log(MEX_LOG_PHS, "E:wrong eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_WRONG_LOCATION;
	}

	MexPhs_InitPhbGlobalVars(sEntry);
#if defined(MEX_DUAL_SIM)
#if defined(MEX_PLATFORM__FOR_MTK23C) || defined(MEX_PLATFORM_FOR_MTK6253) || defined(MEX_PLATFORM_FOR_MTK6235)
	if (g_phb_cntx.selected_storage == MMI_SIM2)
	{
		g_MexPhbCurrOp = MMI_PHB_OP_ADD;
		MTPNP_PFAL_ADN_AddEntry();
		return E_MEX_PHB_RESULT_SUCCESS;
	}
#endif /* MEX_PLATFORM__FOR_MTK23C MEX_PLATFORM_FOR_MTK6253 */    
#endif /* MEX_DUAL_SIM */

	// Current Operation
	g_phb_cntx.current_op = MMI_PHB_OP_ADD;
	// Set done flag
	g_phb_cntx.set_done_flag = 0;
	// Send req
	{
		MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
		MYQUEUE Message;

		myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
		myMsgPtr->type = MMI_PHB_PHONEBOOK;
		myMsgPtr->no_data = 1;

		if (g_phb_cntx.selected_storage == MMI_NVRAM)
			myMsgPtr->list.storage = MMI_NVRAM;
		else
			myMsgPtr->list.storage = MMI_SIM;

		myMsgPtr->list.index = 0xFFFF;
		myMsgPtr->list.record_index = 0xFFFF;

		mmi_phb_op_set_entry_prepare_name_number(&myMsgPtr->list, pbName, pbNumber);

		Message.oslSrcId = MOD_MMI;

#ifdef MEX_DUAL_SIM
		if (g_phb_cntx.selected_storage == MMI_SIM2)
			Message.oslDestId = MOD_L4C_2;
		else
#endif /* MEX_DUAL_SIM */
			Message.oslDestId = MOD_L4C;

		Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
		Message.oslDataPtr = (oslParaType*) myMsgPtr;
		Message.oslPeerBuffPtr = NULL;

#ifdef MEX_DUAL_SIM
		if (g_phb_cntx.selected_storage == MMI_SIM2)
			Mex_SetSlaveProtocolEventHandler((void*)MexPhs_AddPhbEntryExRsp, PRT_PHB_SET_ENTRY_RSP);
		else
#endif /* MEX_DUAL_SIM */
			Mex_SetProtocolEventHandler((void*)MexPhs_AddPhbEntryExRsp, PRT_PHB_SET_ENTRY_RSP);
		OslMsgSendExtQueue(&Message);
	}
	return E_MEX_PHB_RESULT_SUCCESS;

}



// mmi_phb_op_add_entry_req
E_Mex_Phs_Phb_Result MexPhs_AddPhbEntry(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->name_company.name, (const wchar_t *)L"");
		strcpy(pEntry->email.address, "");
		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->number_fax.number, (const wchar_t *)L"");
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
		pEntry->groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;
		res = MexPhs_AddPhbEntryEx(eLocation, pEntry);
		mex_free(pEntry);
	}

	return res;
}



E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	uint16 store_index = 0;
	uint8 storage;
	bool bPermission = false;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookUpdate);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}
	// Check current operation
	if (g_phb_cntx.current_op != MMI_PHB_OP_NONE || g_MexPhbCurrOp != MMI_PHB_OP_NONE)
	{
		mex_log(MEX_LOG_PHS, "E:current_op:%d", g_phb_cntx.current_op);
		return E_MEX_PHB_RESULT_BUSY;
	}


	MexPhs_InitPhbGlobalVars(psEntry);
#if defined(MEX_DUAL_SIM) 
#if defined(MEX_PLATFORM__FOR_MTK23C) || defined(MEX_PLATFORM_FOR_MTK6253)||defined(MEX_PLATFORM_FOR_MTK6235)
	if (uIndex >= (MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES))
	{
		g_phb_cntx.active_index = uIndex;
		g_MexPhbCurrOp = MMI_PHB_OP_EDIT;
		MTPNP_PFAL_ADN_EDITEntry((unsigned short)(uIndex + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES));
		return E_MEX_PHB_RESULT_SUCCESS;
	}
#endif /* MEX_PLATFORM__FOR_MTK23C MEX_PLATFORM_FOR_MTK6253 */
#endif /* MEX_DUAL_SIM */

	// Current Operation
	g_phb_cntx.current_op = MMI_PHB_OP_EDIT;
	// Set done flag
	g_phb_cntx.set_done_flag = 0;

	store_index = g_phb_cntx.active_index = uIndex;
	// Send Req
	{
		myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
		storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
		myMsgPtr->type = MMI_PHB_PHONEBOOK;
		myMsgPtr->no_data = 1;
		myMsgPtr->list.storage = storage;
		myMsgPtr->list.index = 0xffff;

#ifdef MEX_DUAL_SIM
		// SIM2
		if (store_index >= MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES)
		{
			myMsgPtr->list.record_index = store_index + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES;
			g_phb_cntx.selected_storage = MMI_SIM2;
		}
		else
#endif /* MEX_DUAL_SIM */
			// SIM1
			if (store_index >= MAX_PB_PHONE_ENTRIES)
			{
				myMsgPtr->list.record_index = store_index + 1 - MAX_PB_PHONE_ENTRIES;
				g_phb_cntx.selected_storage = MMI_SIM;
			}
			// PHONE
			else
			{
				myMsgPtr->list.record_index = store_index + 1;
				g_phb_cntx.selected_storage = MMI_NVRAM;
			}

			mmi_phb_op_set_entry_prepare_name_number(&myMsgPtr->list, pbName, pbNumber);

			Message.oslSrcId = MOD_MMI;

#ifdef MEX_DUAL_SIM
			if (g_phb_cntx.selected_storage == MMI_SIM2)
				Message.oslDestId = MOD_L4C_2;
			else
#endif /* MEX_DUAL_SIM */
				Message.oslDestId = MOD_L4C;

			Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
			Message.oslDataPtr = (oslParaType*) myMsgPtr;
			Message.oslPeerBuffPtr = NULL;

#ifdef MEX_DUAL_SIM
			if (g_phb_cntx.selected_storage == MMI_SIM2)
				Mex_SetSlaveProtocolEventHandler((void*)MexPhs_AddPhbEntryExRsp, PRT_PHB_SET_ENTRY_RSP);
			else
#endif /* MEX_DUAL_SIM */
				Mex_SetProtocolEventHandler((void*)MexPhs_AddPhbEntryExRsp, PRT_PHB_SET_ENTRY_RSP);        
			OslMsgSendExtQueue(&Message);
	}
	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		// 读取旧的信息
		MexPhs_GetPhbEntryEx(uIndex, pEntry);
		// 填充新的姓名和号码
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

		res = MexPhs_UpdatePhbEntryEx(uIndex, pEntry);
		mex_free(pEntry);
	}

	return res;
}


static void MexPhs_RemovePhbEntryRsp(void *info)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT *localPtr;
	uint16 deletedCnt = 0;
	uint16 store_index;
	uint16 record_index;
	uint8 storage;

	E_Mex_Phs_Phb_Event cb_event;
	uint16 cb_index;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT*) info;
	if (localPtr->result.flag == 0 /* OK */ )
	{
		//  mex_log(MEX_LOG_PHS, "Phs_RemovePhbEntryRsp Success");
		cb_event = E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS;
		cb_index = g_phb_cntx.active_index;

		store_index = g_phb_cntx.active_index;
		storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
		record_index = (storage == MMI_SIM) ? (store_index + 1) - MAX_PB_PHONE_ENTRIES : (store_index + 1);

		PhbUpdateSpeedDial(storage, record_index, MMI_PHB_PHONEBOOK, NULL, NULL);

#if defined(MEX_DUAL_SIM)&& (MEX_MTK_CODE_VERSION < 0x0824) 
		if (store_index >= MAX_PB_PHONE_ENTRIES + MAX_SIM_ENTRIES)    /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
		{
			g_phb_cntx.sim2_used--;
		}
		else
#endif /* MEX_DUAL_SIM */
			if (store_index >= MAX_PB_PHONE_ENTRIES)
			{
				g_phb_cntx.sim_used--;
			}
			else
			{
				g_phb_cntx.phone_used--;

				/* Clear birthday field */
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
				mmi_phb_bday_delete_field(store_index);
#endif 

#if defined(__SYNCML_SUPPORT__)&&!defined(WIN32)
				mmi_syncml_phb_data_changed_notification(SYNCML_RECORD_DELETE, store_index);
#endif
			}

			/* Remove lookup table entry */
			deletedCnt = mmi_phb_op_clear_lookup_table_by_index(store_index);

			if (deletedCnt)
			{
				mmi_phb_lookup_table_sort();
				g_phb_cntx.lookup_table_count -= deletedCnt;
			}

			/* clear phonebook entry and remove from index */
#ifdef __SLT_MMI_VCARD_COPYALL
			memset(&PhoneBook[store_index], 0, sizeof(MMI_PHB_ENTRY_BCD_STRUCT)); //zhangyuansheng 20080302
#endif
			mmi_phb_op_delete_update_index(store_index);

			g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;
	}
	else
	{
		mex_log(MEX_LOG_PHS, "E:RemovePhb Fail");
		cb_event = E_MEX_PHB_EVENT_REMOVEENTRY_FAIL;
		cb_index = g_phb_cntx.active_index;
	}

	g_phb_cntx.current_op = 0;
	// phb call back
	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(cb_index, cb_event);
		mex_SetCurrentApp(appId);
	}    
}



E_Mex_Phs_Phb_Result MexPhs_RemovePhbEntry(uint16 uIndex)
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	uint16 store_index;
	uint8 storage;
	bool bPermission = false;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookDelete);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}    
	// Check current operation
	if (g_phb_cntx.current_op != MMI_PHB_OP_NONE || g_MexPhbCurrOp != MMI_PHB_OP_NONE)
	{
		mex_log(MEX_LOG_PHS, "E: current_op:%d", g_phb_cntx.current_op);
		return E_MEX_PHB_RESULT_BUSY;
	}

#if defined(MEX_DUAL_SIM) 
#if defined(MEX_PLATFORM__FOR_MTK23C) || defined(MEX_PLATFORM_FOR_MTK6253)||defined(MEX_PLATFORM_FOR_MTK6235)
	if (uIndex >= (MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES))
	{
		g_MexPhbCurrOp = MMI_PHB_OP_EDIT + 1;
		MTPNP_AD_DELETE_ADN_Record((unsigned short)(uIndex + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES));
		return E_MEX_PHB_RESULT_SUCCESS;
	}
#endif /* MEX_PLATFORM__FOR_MTK23C MEX_PLATFORM_FOR_MTK6253 */
#endif /* MEX_DUAL_SIM */


	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	g_phb_cntx.current_op = MMI_PHB_OP_EDIT + 1;
	store_index = g_phb_cntx.active_index = uIndex;
	myMsgPtr = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));
	storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;

	myMsgPtr->storage = storage;
	myMsgPtr->del_all = MMI_FALSE;
	myMsgPtr->no_data = 1;
	myMsgPtr->type = MMI_PHB_PHONEBOOK;

	/*The index means "record_index" here. This change only for MMI_PHB_PHONEBOOK(ADN) type */
	/* for other type( BDN ,FDN, MSISDN...) This field still need to use index as parameter. */
#ifdef MEX_DUAL_SIM
	// SIM2
	if (store_index >= MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES)
	{
		myMsgPtr->index = store_index + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES;
		g_phb_cntx.selected_storage = MMI_SIM2;
	}
	else
#endif /* MEX_DUAL_SIM */
		// SIM1
		if (store_index >= MAX_PB_PHONE_ENTRIES)
		{
			myMsgPtr->index = store_index + 1 - MAX_PB_PHONE_ENTRIES;
			g_phb_cntx.selected_storage = MMI_SIM;
		}
		// PHONE
		else
		{
			myMsgPtr->index = store_index + 1;
			g_phb_cntx.selected_storage = MMI_NVRAM;
		}

		Message.oslSrcId = MOD_MMI;

#ifdef MEX_DUAL_SIM
		if (g_phb_cntx.selected_storage == MMI_SIM2)
			Message.oslDestId = MOD_L4C_2;
		else
#endif /* MEX_DUAL_SIM */
			Message.oslDestId = MOD_L4C;    

		Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
		Message.oslDataPtr = (oslParaType*) myMsgPtr;
		Message.oslPeerBuffPtr = NULL;

#ifdef MEX_DUAL_SIM
		if (g_phb_cntx.selected_storage == MMI_SIM2)
			Mex_SetSlaveProtocolEventHandler((void*)MexPhs_RemovePhbEntryRsp, PRT_PHB_DEL_ENTRY_RSP);
		else
#endif /* MEX_DUAL_SIM */
			Mex_SetProtocolEventHandler((void*)MexPhs_RemovePhbEntryRsp, PRT_PHB_DEL_ENTRY_RSP);

		OslMsgSendExtQueue(&Message);

		return E_MEX_PHB_RESULT_SUCCESS;
}


E_Mex_Phs_Phb_Result MexPhs_GetPhbGroup(uint16 groupIndex, Mex_Phs_Phb_Group *pGroup)
{
	mex_memset(pGroup, 0, sizeof(Mex_Phs_Phb_Group));   
	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

#ifdef __MMI_PHB_USIM_FIELD__
	if (groupIndex >= MAX_PB_CALLER_GROUPS)
	{
		MMI_PHB_NAME_STRUCT * usim_caller_groups = (MMI_PHB_NAME_STRUCT *) g_phb_cntx.usim_group;
		mex_wcscpy(pGroup->groupName.name, (const wchar_t *) usim_caller_groups[groupIndex - MAX_PB_CALLER_GROUPS].name);
	}
	else
#endif /* __MMI_PHB_USIM_FIELD__ */ 
		if (groupIndex < MAX_PB_CALLER_GROUPS)
		{
			PHB_CALLER_GROUP_STRUCT * callerGroups = (PHB_CALLER_GROUP_STRUCT *) g_phb_cntx.caller_group;
			if (callerGroups)
				mex_wcscpy(pGroup->groupName.name, (const wchar_t *) callerGroups[groupIndex].groupName);        
		}
		else
		{
			return E_MEX_PHB_RESULT_WRONG_INDEX;
		}
		pGroup->groupName.nameLen = mex_wcslen(pGroup->groupName.name);
		pGroup->groupIndex = groupIndex;

		return E_MEX_PHB_RESULT_SUCCESS;
}



void MexPhs_AddPhbEntryExCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_ADDENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_ADDENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	}    
}

void MexPhs_UpdatePhbEntryExCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_UPDATEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	}    
}

void MexPhs_RemovePhbEntryCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_REMOVEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	} 
}

E_Mex_Phs_Phb_Result MexPhs_LookUpPhbNumber(const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName)
{
	uint16 i = 0xffff;
	uint32 num;
	char number_ASCII[MAX_PB_NUMBER_LENGTH + 2] = { 0 };
#ifdef __MMI_PHB_COMPARE_11_DIGIT__
	uint32 number_forside, number_rearward;
#endif

 	if((pName == NULL) || (pNumber == NULL))
 	{
        mex_log(MEX_LOG_SYSTEM,"can't get Numer from NULL PHB string");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
 	}


	// Phb is empty ?
	if (PhoneBookEntryCount == 0)
	{
		mex_log(MEX_LOG_PHS, "E: Phb is empty");
		return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	if (pNumber->numberLen == 0 || pNumber->number[0] == L'\0')
	{
		mex_log(MEX_LOG_PHS, "E: number is NUL");
		return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	}

	mex_wcs2nstr(number_ASCII, MAX_PB_NUMBER_LENGTH + 2, pNumber->number);

#ifdef __MMI_PHB_COMPARE_11_DIGIT__
	num = mmi_phb_util_convert_number_to_int_ext((U8*)number_ASCII, &number_forside, &number_rearward); 
#else

#if defined(MEX_PLATFORM_FOR_MTK6253) || defined(MEX_PLATFORM_FOR_MTK6235)
	num = mmi_phb_util_convert_number_to_int((U8*)number_ASCII);
#else
	num = mmi_phb_util_convert_number_to_int(number_ASCII);
#endif // MEX_PLATFORM_FOR_MTK6253

#endif // __MMI_PHB_COMPARE_11_DIGIT__

#ifdef __MMI_PHB_COMPARE_11_DIGIT__
	if ((num < INVALID_NUMBER ) && (number_forside < INVALID_NUMBER ) && (number_rearward < INVALID_NUMBER ) )
	{
		i = mmi_phb_lookup_table_search_ext(number_forside, number_rearward, 0, (U16) (g_phb_cntx.lookup_table_count - 1), 
			(S8*) number_ASCII, MMI_STORAGE_BOTH);
	}
#else
	if (num < INVALID_NUMBER)
	{
		i = mmi_phb_lookup_table_search(num, 0, (U16) (g_phb_cntx.lookup_table_count - 1), 
			(S8*) number_ASCII, MMI_STORAGE_BOTH);
	}
#endif // __MMI_PHB_COMPARE_11_DIGIT__

	if (i < 0xffff) /* i is storage location in array, begin from 0. */
	{
		mex_wcscpy(pName->name, (const wchar_t *)PhoneBook[i].alpha_id.name);
		pName->nameLen = mex_wcslen(pName->name);
		return E_MEX_PHB_RESULT_SUCCESS;
	}
	return E_MEX_PHB_RESULT_ERROR_UNKNOWN;

}
#else
typedef void (*srv_phb_callback_type)(S32 result, U16 store_index, void* user_data);
typedef enum
{
	MEXPHS_PHB_OP_ADD,
	MEXPHS_PHB_OP_DEL,
	MEXPHS_PHB_OP_UPDATE,
	MEXPHS_PHB_OP_MAX
}MexPhs_phb_op_event;
typedef struct
{
	srv_phb_callback_type cb;
	MexPhs_phb_op_event event;
	void* user_data;
}MexPhs_phb_cntx_struct;

Mex_Phs_Phb_CB g_MexPhbCallBack = NULL;
uint8* g_ppuSearchBitmask = NULL;
int32          g_MexPhbAppId = 0;
#if (MEX_MTK_CODE_VERSION >= 0x1032)
int32          g_MexPhbInputMode = 0;
#endif

static bool MexPhs_PhbCheckIndex(uint16 uIndex)
{
	return srv_phb_check_entry_exist(uIndex); // not found
}

void MexPhs_SearchPhb_SetInputMode(uint8 mode)
{
	mmi_imm_create_input_method_list(IMM_INPUT_TYPE_SENTENCE, NULL);
	mmi_imm_set_current_input_mode(mode);

}
int32 MexPhs_SearchPhbEntry(wchar_t* szKeyword, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask)
{
	srv_phb_filter_struct * filter;
	U8	storage;
	mmi_imm_input_mode_enum input_mode;
	U16 *ppuStoreIndex;
	U16 *contact_array;
	U16 contact_count,i,j;
	uint8 *bitmask = NULL;
	if(g_ppuSearchBitmask == NULL)
		g_ppuSearchBitmask = srv_phb_mem_malloc((MMI_PHB_ENTRIES/8+1)*sizeof(uint8), SRV_PHB_MEMORY_TYPE_ADM);
	*ppuSearchBitmask = g_ppuSearchBitmask;
	bitmask = *ppuSearchBitmask;
	contact_count =0;
	memset(g_ppuSearchBitmask,0,(MMI_PHB_ENTRIES/8+1)*sizeof(uint8));
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	input_mode = g_MexPhbInputMode;
#else
	input_mode =  mmi_imm_get_curr_input_mode();
#endif
	switch (eLocation)
	{
	case E_MEX_PHB_LOCATION_PHONE:
		storage = PHB_STORAGE_NVRAM;
		break;
	case E_MEX_PHB_LOCATION_SIM1:
		storage = PHB_STORAGE_SIM;
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_PHB_LOCATION_SIM2:
		storage = PHB_STORAGE_SIM2;
		break;
#endif /* MEX_DUAL_SIM */
	case E_MEX_PHB_LOCATION_ALL:
		storage = PHB_STORAGE_MAX;
		break;
	default:
		mex_log(MEX_LOG_PHS, "E:wrong eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_WRONG_LOCATION;
	}
	contact_array = (U16*)mex_malloc(MMI_PHB_ENTRIES*sizeof(U16));
#if (MEX_MTK_CODE_VERSION < 0x1112)    
	filter = mmi_phb_list_build_filter(
		storage,
		SRV_PHB_ENTRY_FIELD_NAME|SRV_PHB_ENTRY_FIELD_NUMBER,
		NULL,
		(U16 *) szKeyword,
		(U16) input_mode);
#else
	{
		srv_phb_qsearch_filter_struct *search_filter = NULL;
		filter = (srv_phb_filter_struct *) srv_phb_mem_malloc(sizeof(srv_phb_filter_struct), SRV_PHB_MEMORY_TYPE_CTR);
		if (filter != NULL)
		{
			filter->storage= storage;
			filter->field_filter = SRV_PHB_ENTRY_FIELD_NAME|SRV_PHB_ENTRY_FIELD_NUMBER;
			filter->name_filter = NULL;
			filter->group_filter = NULL;
			search_filter = (srv_phb_qsearch_filter_struct *) srv_phb_mem_malloc(sizeof(srv_phb_qsearch_filter_struct), SRV_PHB_MEMORY_TYPE_CTR);
			if (search_filter != NULL)
			{
				search_filter->key_len = mmi_wcslen(szKeyword);
				search_filter->key_word = szKeyword;
				search_filter->input_mode = input_mode;
				filter->qsearch_filter = search_filter;
			}
		}
	}
#endif

	contact_count = (U16) srv_phb_oplib_build_contact_list(filter, contact_array, MMI_PHB_ENTRIES);
	ppuStoreIndex = (uint16 *)MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_ENTRY_INDEX_PTR);
	for(i=0;i<contact_count;i++)
	{
		for(j=0;j<MMI_PHB_ENTRIES;j++)
		{
			if(ppuStoreIndex[j] == contact_array[i])
			{
				bitmask[j >> 3] |= 1 << (j % 8);
				break;
			}
		}
	}
	mex_free(contact_array);
#if (MEX_MTK_CODE_VERSION < 0x1112)    
	mmi_phb_list_free_filter(filter);
#else
    if (filter)
    {
        if (filter->qsearch_filter)
            srv_phb_mem_free(filter->qsearch_filter);
        srv_phb_mem_free(filter);
    }
#endif
	return contact_count;
}
void MexPhs_SearchPhb_MakeCache(void)
{
	return;
}
void MexPhs_SearchPhb_ChangeInputMode(bool isPinyin)
{
	mmi_imm_create_input_method_list(IMM_INPUT_TYPE_QUICK_SEARCH, NULL);

	if (isPinyin)
	{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		g_MexPhbInputMode = IMM_INPUT_MODE_QUICK_SEARCH_PINYIN;
#endif
		mmi_imm_set_current_input_mode(IMM_INPUT_MODE_QUICK_SEARCH_PINYIN);
	}
	else
	{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		g_MexPhbInputMode = IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC;
#endif
		mmi_imm_set_current_input_mode(IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	}

}
uint8 MexPhs_SearchPhb_GetInputMode(void)
{
	return mmi_imm_get_curr_input_mode();

}

E_Mex_Phs_Phb_Result MexPhs_GetPhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	extern uint16 PhoneBookEntryCount; 
	int8	*number=NULL;
	bool bPermission = false;
 	if((psName == NULL) || (psNumber == NULL))
 	{
        mex_log(MEX_LOG_SYSTEM,"Can't get entry from NULL PHB string");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
 	}

	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookRead);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	// Init
	mex_memset(psName, 0, sizeof(Mex_Phs_Phb_Name));
	mex_memset(psNumber, 0, sizeof(Mex_Phs_Phb_Number));
	srv_phb_datamgr_get_name(uIndex, psName->name, MEX_H_PHS_PHB_NAME_LEN);
	psName->nameLen = mex_wcslen(psName->name);
#if (MEX_MTK_CODE_VERSION < 0x1112)    
	number = (int8 *)srv_phb_datamgr_get_number(uIndex);
#else
    //zhanggs modified here 20110715, interface function has changed
	//number = (int8 *)srv_phb_datamgr_get_number(mmi_phb_contact_id id, U8 save_index, U8* type);
#endif
	if(number[0]!=0)
	{
		psNumber->numberLen = mex_strlen((const char *) number);
		if (psNumber->numberLen > MEX_H_PHS_PHB_NUMBER_LEN)
			psNumber->numberLen = MEX_H_PHS_PHB_NUMBER_LEN;
		mex_str2nwcs(psNumber->number,(psNumber->numberLen+1)*2,(const char *) number);

	}

	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_GetPhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{
	U8	storage;
	bool bPermission = false;
	if(psEntry == NULL)
	{
        mex_log(MEX_LOG_SYSTEM,"input PhbEntry Buffer is NULL");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	}
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookRead);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}
	// Init
	mex_memset(psEntry, 0, sizeof(Mex_Phs_Phb_Entry));
	MexPhs_GetPhbEntry(uIndex,&psEntry->name,&psEntry->number);
#if 0
	{
		mmi_phb_optional_fields_struct optional_fields_data;
		srv_phb_datamgr_read_optional_nvram(uIndex, &optional_fields_data);

		mmi_asc_n_to_wcs(psEntry->number_home.number, (S8*)optional_fields_data.home_number,MEX_H_PHS_PHB_NUMBER_LEN);
		psEntry->number_home.numberLen = mex_wcslen(psEntry->number_home.number);

		mmi_wcscpy(psEntry->name_company.name, optional_fields_data.company_name);
		psEntry->name_company.nameLen = mex_wcslen(psEntry->name_company.name);

		mex_strncpy(psEntry->email.address, (S8*)optional_fields_data.email_address,MEX_H_PHS_PHB_EMAILADD_LEN);
		psEntry->email.addressLen = mex_strlen(psEntry->email.address);

		mmi_asc_n_to_wcs(psEntry->number_company.number, (S8*)optional_fields_data.office_number,MEX_H_PHS_PHB_NUMBER_LEN);
		psEntry->number_company.numberLen = mex_wcslen(psEntry->number_company.number);

		mmi_asc_n_to_wcs(psEntry->number_fax.number, (S8*)optional_fields_data.fax_number,MEX_H_PHS_PHB_NUMBER_LEN);
		psEntry->number_fax.numberLen = mex_wcslen(psEntry->number_fax.number);

	}
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
#ifdef __MMI_BIRTHDAY_REMINDER__
	srv_phb_get_birthday(uIndex,&psEntry->birthday.year,&psEntry->birthday.month,&psEntry->birthday.day);
#endif
#endif
	storage = srv_phb_get_storage(uIndex);
	switch (srv_phb_get_storage(uIndex))
	{
	case PHB_STORAGE_SIM:

		psEntry->location = E_MEX_PHB_LOCATION_SIM1;
		break;
#ifdef MEX_DUAL_SIM
	case PHB_STORAGE_SIM2:
		psEntry->location = E_MEX_PHB_LOCATION_SIM2;
		break;
#endif          
	case PHB_STORAGE_NVRAM:
		psEntry->location = E_MEX_PHB_LOCATION_PHONE;
		break;

	default:
		break;
	}

	psEntry->groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;
	return E_MEX_PHB_RESULT_SUCCESS;
}


void MexPhs_phb_build_contact_data(srv_phb_contact_info_struct* info,Mex_Phs_Phb_Entry* sEntry,U8 storage)
{
	U16 len;
	mex_memset(info,0,sizeof(srv_phb_contact_info_struct));
#if (MEX_MTK_CODE_VERSION < 0x1112)    
	info->field_mask|= SRV_PHB_ENTRY_FIELD_SIM | SRV_PHB_ENTRY_FIELD_OPTIONAL|SRV_PHB_ENTRY_FIELD_BDAY|SRV_PHB_ENTRY_FIELD_GROUP;
#else	
	info->field_mask|= SRV_PHB_ENTRY_FIELD_SIM |SRV_PHB_ENTRY_FIELD_BDAY|SRV_PHB_ENTRY_FIELD_GROUP;
#endif	
	if (sEntry->name.nameLen)
	{
		info->field_data.name = mex_malloc((MMI_PHB_NAME_FIELD_LENGTH + 1) * sizeof(U16));
		mex_memset(info->field_data.name,0,(MMI_PHB_NAME_FIELD_LENGTH + 1) * sizeof(U16));
		len = (sEntry->name.nameLen<MMI_PHB_NAME_FIELD_LENGTH)?sEntry->name.nameLen:MMI_PHB_NAME_FIELD_LENGTH;
		mmi_wcsncpy(info->field_data.name, sEntry->name.name, len);
	}
	info->field_data.number = mex_malloc((MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
	mex_memset(info->field_data.number,0,(MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
	len = (sEntry->number.numberLen<MMI_PHB_NUMBER_PLUS_LENGTH)?sEntry->number.numberLen:MMI_PHB_NUMBER_PLUS_LENGTH;
	mmi_wcsncpy(info->field_data.number, sEntry->number.number, len);
	if(PHB_STORAGE_NVRAM == storage )
	{
#if (!defined(__MMI_PHB_NO_OPTIONAL_FIELD__))
		info->field_data.home_number = mex_malloc((MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
		mex_memset(info->field_data.home_number,0,(MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
		len = (sEntry->number_home.numberLen<MMI_PHB_NUMBER_PLUS_LENGTH)?sEntry->number_home.numberLen:MMI_PHB_NUMBER_PLUS_LENGTH;
		mmi_wcsncpy(info->field_data.home_number, sEntry->number_home.number, len);

		info->field_data.office_number = mex_malloc((MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16)); 	   
		mex_memset(info->field_data.office_number,0,(MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
		len = (sEntry->number_company.numberLen<MMI_PHB_NUMBER_PLUS_LENGTH)?sEntry->number_company.numberLen:MMI_PHB_NUMBER_PLUS_LENGTH;
		mmi_wcsncpy(info->field_data.office_number, sEntry->number_company.number, len);

		info->field_data.company_name = mex_malloc((MMI_PHB_COMPANY_LENGTH + 1) * sizeof(U16));
		mex_memset(info->field_data.company_name,0,(MMI_PHB_COMPANY_LENGTH + 1) * sizeof(U16));
		len = (sEntry->name_company.nameLen<MMI_PHB_COMPANY_LENGTH)?sEntry->name_company.nameLen:MMI_PHB_COMPANY_LENGTH;
		mmi_wcsncpy(info->field_data.company_name, sEntry->name_company.name, len);

		info->field_data.email_address = mex_malloc((MMI_PHB_EMAIL_LENGTH + 1) * sizeof(U16));
		mex_memset(info->field_data.email_address,0,(MMI_PHB_EMAIL_LENGTH + 1) * sizeof(U16));
		len = (sEntry->email.addressLen<MMI_PHB_EMAIL_LENGTH)?sEntry->email.addressLen:MMI_PHB_EMAIL_LENGTH;
		mex_str2nwcs(info->field_data.email_address,(len+1)*2,sEntry->email.address);

		info->field_data.fax_number = mex_malloc((MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
		mex_memset(info->field_data.fax_number,0,(MMI_PHB_NUMBER_PLUS_LENGTH + 1) * sizeof(U16));
		len = (sEntry->number_fax.numberLen<MMI_PHB_NUMBER_PLUS_LENGTH)?sEntry->number_fax.numberLen:MMI_PHB_NUMBER_PLUS_LENGTH;
		mmi_wcsncpy(info->field_data.fax_number , sEntry->number_fax.number , len);
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		info->field_data.b_year = sEntry->birthday.year;
		info->field_data.b_month = sEntry->birthday.month;
		info->field_data.b_day = sEntry->birthday.day;
#endif
	}
}


void MexPhs_phb_free_contact_data(srv_phb_contact_data_struct* field_data)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if (field_data->name)
	{
		mex_free(field_data->name);
		field_data->name = NULL;
	}

	if (field_data->number)
	{
		mex_free(field_data->number);
		field_data->number = NULL;
	}

#if (!defined(__MMI_PHB_NO_OPTIONAL_FIELD__))
	if (field_data->home_number)
	{
		mex_free(field_data->home_number);
		field_data->home_number = NULL;
	}
	if (field_data->office_number)
	{
		mex_free(field_data->office_number);
		field_data->home_number = NULL;
	}
	if (field_data->company_name)
	{
		mex_free(field_data->company_name);
		field_data->home_number = NULL;
	}
	if (field_data->email_address)
	{
		mex_free(field_data->email_address);
		field_data->email_address = NULL;
	}
	if (field_data->fax_number)
	{
		mex_free(field_data->fax_number);
		field_data->fax_number = NULL;
	}
#endif /* (!defined(__MMI_PHB_NO_OPTIONAL_FIELD__)) */
}


void MexPhs_phb_cb(S32 result, U16 store_index, void* user_data)
{
	MexPhs_phb_cntx_struct* cntx = user_data;
	E_Mex_Phs_Phb_Event mex_result;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	int32 appId = mex_GetCurrentApp();

	if (cntx->cb)
	{
		if(result == SRV_PHB_NO_ERROR)
		{
			switch(cntx->event)
			{
			case MEXPHS_PHB_OP_ADD:
				mex_result = E_MEX_PHB_EVENT_ADDENTRY_SUCCESS;
				break;

			case MEXPHS_PHB_OP_DEL:
				mex_result = E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS;
				break;

			case MEXPHS_PHB_OP_UPDATE:
				mex_result = E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS;
				break;

			default:
				mex_result = E_MEX_PHB_EVENT_MAX;
				break;
			}
		}
		else
		{
			switch(cntx->event)
			{
			case MEXPHS_PHB_OP_ADD:
				mex_result = E_MEX_PHB_EVENT_ADDENTRY_FAIL;
				break;

			case MEXPHS_PHB_OP_DEL:
				mex_result = E_MEX_PHB_EVENT_REMOVEENTRY_FAIL;
				break;

			case MEXPHS_PHB_OP_UPDATE:
				mex_result = E_MEX_PHB_EVENT_UPDATEENTRY_FAIL;
				break;

			default:
				mex_result = E_MEX_PHB_EVENT_MAX;
				break;
			}
		}

		if (g_MexPhbCallBack != NULL)
		{
			mex_SetCurrentApp(g_MexPhbAppId);
			(*g_MexPhbCallBack)(store_index, mex_result);
			g_MexPhbCallBack = NULL;
			mex_SetCurrentApp(appId);
		}	 
	}
	srv_phb_mem_free(cntx);


}


E_Mex_Phs_Phb_Result MexPhs_AddPhbEntryEx(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* sEntry)
{
	U8 storage;
	bool bPermission = false;
	MexPhs_phb_cntx_struct* cntx;
	srv_phb_contact_info_struct info;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookAdd);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	switch (eLocation)
	{
	case E_MEX_PHB_LOCATION_PHONE:
		storage = PHB_STORAGE_NVRAM;
		break;
	case E_MEX_PHB_LOCATION_SIM1:
		storage = PHB_STORAGE_SIM;
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_PHB_LOCATION_SIM2:
		storage = PHB_STORAGE_SIM2;
		break;
#endif /* MEX_DUAL_SIM */
	default:
		mex_log(MEX_LOG_PHS, "E:wrong eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_WRONG_LOCATION;
	}

	cntx = srv_phb_mem_malloc(sizeof(MexPhs_phb_cntx_struct), SRV_PHB_MEMORY_TYPE_ADM);
	MexPhs_phb_build_contact_data(&info,sEntry,storage);

	cntx->cb = MexPhs_phb_cb;
	cntx->event = MEXPHS_PHB_OP_ADD;
	cntx->user_data = NULL;
	srv_phb_oplib_add_contact(storage, SRV_PHB_INVALID_INDEX, &info, MexPhs_phb_cb, cntx);

	MexPhs_phb_free_contact_data(&info.field_data);

	return E_MEX_PHB_RESULT_SUCCESS;

}


E_Mex_Phs_Phb_Result MexPhs_AddPhbEntry(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->name_company.name, (const wchar_t *)L"");
		strcpy(pEntry->email.address, "");
		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->number_fax.number, (const wchar_t *)L"");
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
		pEntry->groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;
		res = MexPhs_AddPhbEntryEx(eLocation, pEntry);
		mex_free(pEntry);
	}

	return res;
}


E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{
	U8 storage;
	bool bPermission = false;
	MexPhs_phb_cntx_struct* cntx;

	srv_phb_contact_info_struct info;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookUpdate);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	cntx = srv_phb_mem_malloc(sizeof(MexPhs_phb_cntx_struct), SRV_PHB_MEMORY_TYPE_ADM);
	storage = srv_phb_get_storage(uIndex);
	MexPhs_phb_build_contact_data(&info,psEntry,storage);
	cntx->cb = MexPhs_phb_cb;
	cntx->event = MEXPHS_PHB_OP_UPDATE;
	cntx->user_data = NULL;
	srv_phb_oplib_update_contact(uIndex, &info, MexPhs_phb_cb, cntx);

	MexPhs_phb_free_contact_data(&info.field_data);
	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		// 读取旧的信息
		MexPhs_GetPhbEntryEx(uIndex, pEntry);
		// 填充新的姓名和号码
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

		res = MexPhs_UpdatePhbEntryEx(uIndex, pEntry);
		mex_free(pEntry);
	}

	return res;
}


E_Mex_Phs_Phb_Result MexPhs_RemovePhbEntry(uint16 uIndex)
{

	MexPhs_phb_cntx_struct* cntx;
	MYQUEUE Message;
	bool bPermission = false;
	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookDelete);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}	 

	cntx = srv_phb_mem_malloc(sizeof(MexPhs_phb_cntx_struct), SRV_PHB_MEMORY_TYPE_ADM);
	cntx->cb = MexPhs_phb_cb;
	cntx->event = MEXPHS_PHB_OP_DEL;
	cntx->user_data = NULL;
	srv_phb_oplib_delete_contact(uIndex,  MexPhs_phb_cb, cntx);

	return E_MEX_PHB_RESULT_SUCCESS;
}

bool MexPhs_PhbCheckGroupIndex(uint16 groupIndex)
{
#ifndef __MMI_PHB_NO_GROUP__
	U8 num_of_grp,i;
	U8 group_list[MMI_PHB_GROUP_COUNT];
	num_of_grp = srv_phb_datamgr_get_group_list(group_list, NULL);
	for(i=0;i<num_of_grp;i++)
	{
		if(group_list[i]==groupIndex)
			return TRUE;
	}
#else
    return FALSE;
#endif
    return FALSE;
}

E_Mex_Phs_Phb_Result MexPhs_GetPhbGroup(uint16 groupIndex, Mex_Phs_Phb_Group *pGroup)
{
#ifndef __MMI_PHB_NO_GROUP__
	mex_memset(pGroup, 0, sizeof(Mex_Phs_Phb_Group));	
	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	if(MexPhs_PhbCheckGroupIndex(groupIndex))
	{
		srv_phb_get_group_name(groupIndex,pGroup->groupName.name,MEX_H_PHS_PHB_NAME_LEN);
		pGroup->groupName.nameLen = mex_wcslen(pGroup->groupName.name);
		pGroup->groupIndex = groupIndex;
	}
	else
	{
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}
#else
    return E_MEX_PHB_RESULT_WRONG_INDEX;
#endif

	return E_MEX_PHB_RESULT_SUCCESS;
}


E_Mex_Phs_Phb_Result MexPhs_LookUpPhbNumber(const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName)
{

 	if((pName == NULL) || (pNumber == NULL))
 	{
        mex_log(MEX_LOG_SYSTEM,"can't get Numer from NULL PHB string");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
 	}


	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	if (pNumber->numberLen == 0 || pNumber->number[0] == L'\0')
	{
		mex_log(MEX_LOG_PHS, "E: number is NUL");
		return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	}


	srv_phb_get_name_by_number((U16 *)pNumber->number,pName->name,MEX_H_PHS_PHB_NAME_LEN);
	pName->nameLen = mex_wcslen(pName->name);

	return E_MEX_PHB_RESULT_SUCCESS;
}


#endif // 09B >=0x0952
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

Mex_Phs_Phb_CB g_MexPhbCallBack = NULL;
int32          g_MexPhbAppId = 0;

typedef struct MEX_PHB_EVENT_T_Tag
{
    SIGNAL_VARS
    int      Result;
    uint16   GroupID;
    uint16   EntryID;    
}MEX_PHB_EVENT_T;

static bool MexPhs_PhbCheckIndex(uint16 uIndex)
{
	// Find uIndex in g_phb_name_index
	
	/*uint16 phb_count;
	phb_count = MMIPB_GetEntryCount();

	return (uIndex <= phb_count); // not found*/
	return (uIndex <= (MMINV_MAX_PHONEBOOK_RECORDS * 2 - 1)); //tangtingtao 20111026
}


static uint16 MexPhs_GetPhbEntryGroupIndex(uint16 uIndex)
{
	uint16 groupIndex = MEX_H_PHS_PHB_GROUPINDEX_INVALID;

	return groupIndex;
}

static void MexPhs_SetPhbEntryGroupIndex(uint16 uIndex, uint16 groupIndex)
{

}

// APIs for PhoneBook
E_Mex_Phs_Phb_Result MexPhs_GetPhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{    
	bool bPermission = false;
	uint8   group_id;
	uint16  entry_id;
	uint16  entry_index;
	PHONEBOOK_STORAGE_E storage;

 	if((psName == NULL) || (psNumber == NULL))
 	{
        mex_log(MEX_LOG_SYSTEM,"Can't get entry from NULL PHB string");
        return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
 	}

	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookRead);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		//return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	//将指定的索引分解为group_id和entry_id
	entry_id = uIndex;
	group_id = PB_GROUP_SIM_1;
	entry_index = uIndex;
	storage  = PHONEBOOK_SIM_STORAGE;
	if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL))
	{    
		entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL);
#ifndef MEX_SINGLE_SIM        
		group_id = PB_GROUP_SIM_2;

		if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL))
		{    
			entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			group_id = PB_GROUP_ALL;	
#else
			group_id = PB_GROUP_UNCLASS;
#endif
			storage  = PHONEBOOK_NV_STORAGE;
		}
#else
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			group_id = PB_GROUP_ALL;	
#else
			group_id = PB_GROUP_UNCLASS;
#endif
		storage  = PHONEBOOK_NV_STORAGE;
#endif
	}
	entry_index = entry_id + mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_USED);
	entry_index += mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_USED);

	// Check uIndex
	if (!MexPhs_PhbCheckIndex(entry_index))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	// Init
	mex_memset(psName, 0, sizeof(Mex_Phs_Phb_Name));
	mex_memset(psNumber, 0, sizeof(Mex_Phs_Phb_Number));

	// Get pbName and Number
	{        
		// 记录列表的映射情况
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		MMIPB_CONTACT_T pb_entry = {0};
#else
		MMIPB_PHONEBOOK_ENTRY_T pb_entry = {0};
		MMIPB_NAME_T    pb_search = {0};
#endif
		MMIPB_STRING_T  string_t = {0};
		MMIPB_NAME_T*    pEntry_name = NULL;
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
	pEntry_name = &(pb_entry.entry_name);
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 	
		pEntry_name = &(pb_entry.name);
#else
		pEntry_name = &(pb_entry.union_t.entry_name);
#endif
#endif
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		pb_entry.contact_id = entry_id;
		pb_entry.union_t.group = group_id;
		MMIAPIPB_GetContactInfo(pb_entry.contact_id,storage ,(MMIPB_CONTACT_T *)&pb_entry);
#else
		pb_entry.entry_id = entry_id;
		pb_entry.group_id = group_id;
		MMIPB_ReadPhonebookEntry(
			storage,
			&pb_entry);
#endif
		mex_wcsncpy((wchar_t *) psName->name,
			(const wchar_t *) pEntry_name->wstr,
			pEntry_name->wstr_len);

		psName->nameLen = pEntry_name->wstr_len;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number[0].npi_ton), 
			pb_entry.number[0].number_len, 
			pb_entry.number[0].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
			);
#else
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number_t[0].npi_ton), 
			pb_entry.number_t[0].number_len, 
			pb_entry.number_t[0].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
			);
#endif
		mex_str2nwcs(psNumber->number,
			sizeof(psNumber->number),
			(const char *)string_t.strbuf);

		psNumber->numberLen = string_t.strlen;
	}
	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_GetPhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{        
	uint8   group_id;
	uint16  entry_id;
	uint16  entry_index;
	PHONEBOOK_STORAGE_E storage;

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	//将指定的索引分解为group_id和entry_id
	entry_id = uIndex;
	group_id = PB_GROUP_SIM_1;
	entry_index = uIndex;
	storage  = PHONEBOOK_SIM_STORAGE;
	if(entry_id >= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL)) //'E_MPC_PHONEBOOK_MEM_SIM1_USED' is changed by 'E_MPC_PHONEBOOK_MEM_SIM1_TOTAL' //tangtingtao 20111026
	{    
		entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL); //tangtingtao 20111026
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)   
				group_id = PB_GROUP_SIM_2;		
				if(entry_id >= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL))	
				{	 
					entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
								group_id = PB_GROUP_ALL;	
#else
								group_id = PB_GROUP_UNCLASS;
#endif

#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
					group_id = PB_GROUP_SIM_3;
					if(entry_id >= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM3_TOTAL))		
					{
						entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM3_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
									group_id = PB_GROUP_ALL;	
#else
									group_id = PB_GROUP_UNCLASS;
#endif

#if defined(MEX_QUAD_SIM)
						group_id = PB_GROUP_SIM_4;
						if(entry_id >= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM4_TOTAL))	   
						{
							entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM4_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
			group_id = PB_GROUP_ALL;	
#else
			group_id = PB_GROUP_UNCLASS;
#endif
						}
#endif            
					}
#endif            
				}
#else
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
							group_id = PB_GROUP_ALL;	
#else
							group_id = PB_GROUP_UNCLASS;
#endif

#endif

	}

	if(MMIPB_IsInNvGroup(group_id)) //tangtingtao 20111117
	{
		storage = PHONEBOOK_NV_STORAGE;
	}
	// Init
	mex_memset(psEntry, 0, sizeof(Mex_Phs_Phb_Entry));

	// Get pbName and Number
	{        
		// 记录列表的映射情况
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	MMIPB_CONTACT_T pb_entry = {0};
#else
	MMIPB_PHONEBOOK_ENTRY_T pb_entry = {0};
	MMIPB_NAME_T    pb_search = {0};
#endif
		MMIPB_STRING_T  string_t = {0};
		MMIPB_NAME_T*    pEntry_name = NULL;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	pb_entry.contact_id = entry_id;
	pb_entry.union_t.group = group_id;
	MMIAPIPB_GetContactInfo(pb_entry.contact_id,storage,(MMIPB_CONTACT_T *)&pb_entry);
#else
	pb_entry.entry_id = entry_id + 1;
	pb_entry.group_id = group_id;

	MMIPB_ReadPhonebookEntry(
	storage,
	&pb_entry);
#endif
      
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
	pEntry_name = &(pb_entry.entry_name);
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	pEntry_name = &(pb_entry.name);
#else
	pEntry_name = &(pb_entry.union_t.entry_name);
#endif
#endif

		mex_wcsncpy((wchar_t *) psEntry->name.name,
			(const wchar_t *) pEntry_name->wstr,
			pEntry_name->wstr_len);

		psEntry->name.nameLen = pEntry_name->wstr_len;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number[0].npi_ton), 
			pb_entry.number[0].number_len, 
			pb_entry.number[0].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
		);
#else
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number_t[0].npi_ton), 
			pb_entry.number_t[0].number_len, 
			pb_entry.number_t[0].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
		);
#endif
		if(string_t.strlen > 0)
		{
			mex_str2nwcs(psEntry->number.number,
				sizeof(psEntry->number.number),
				(const char *)string_t.strbuf);

			psEntry->number.numberLen = string_t.strlen;
		}
		//home number
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 		
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number[1].npi_ton), 
			pb_entry.number[1].number_len, 
			pb_entry.number[1].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
		);
#else
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number_t[1].npi_ton), 
			pb_entry.number_t[1].number_len, 
			pb_entry.number_t[1].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
		);
#endif
		if(string_t.strlen > 0)
		{
			mex_str2nwcs(psEntry->number_home.number,
				sizeof(psEntry->number_home.number),
				(const char *)string_t.strbuf);

			psEntry->number_home.numberLen = string_t.strlen;
		}
#ifndef PB_SUPPORT_LOW_MEMORY
		//company number
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 		
		string_t.strlen = MMIAPICOM_GenDispNumber(
			MMIPB_GetNumberTypeFromUint8(pb_entry.number[2].npi_ton), 
			pb_entry.number[2].number_len, 
			pb_entry.number[2].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
			);
#else
string_t.strlen = MMIAPICOM_GenDispNumber(
	MMIPB_GetNumberTypeFromUint8(pb_entry.number_t[2].npi_ton), 
	pb_entry.number_t[2].number_len, 
	pb_entry.number_t[2].number,
	string_t.strbuf,
	MMIPB_MAX_STRING_LEN
	);
#endif
		if(string_t.strlen > 0)
		{
			mex_str2nwcs(psEntry->number_company.number,
				sizeof(psEntry->number_company.number),
				(const char *)string_t.strbuf);

			psEntry->number_company.numberLen = string_t.strlen;
		}

		//fax number
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	string_t.strlen = MMIAPICOM_GenDispNumber(
		MMIPB_GetNumberTypeFromUint8(pb_entry.number[4].npi_ton), 
		pb_entry.number[4].number_len, 
		pb_entry.number[4].number,
		string_t.strbuf,
		MMIPB_MAX_STRING_LEN
	);
#else
	string_t.strlen = MMIAPICOM_GenDispNumber(
		MMIPB_GetNumberTypeFromUint8(pb_entry.number_t[4].npi_ton), 
		pb_entry.number_t[4].number_len, 
		pb_entry.number_t[4].number,
		string_t.strbuf,
		MMIPB_MAX_STRING_LEN
	);

#endif
		if(string_t.strlen > 0)
		{
			mex_str2nwcs(psEntry->number_fax.number,
				sizeof(psEntry->number_fax.number),
				(const char *)string_t.strbuf);

			psEntry->number_fax.numberLen = string_t.strlen;
		}
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
#if 0//pb不支持company，故先关闭
		mex_wcsncpy(
		(wchar_t *)psEntry->name_company.name, 
		(const wchar_t *)pb_entry.company.wstr,
		pb_entry.company.wstr_len);
		psEntry->name_company.nameLen = pb_entry.company.wstr_len;
#endif
#else
		mex_wcsncpy(
			(wchar_t *)psEntry->name_company.name, 
			(const wchar_t *)pb_entry.org.wstr,
			pb_entry.org.wstr_len);
		psEntry->name_company.nameLen = pb_entry.org.wstr_len;
#endif
#endif
#ifdef MMIPB_MAIL_SUPPORT
		mex_wcsncpy(
			(wchar_t *)psEntry->email.address, 
			(const wchar_t *)pb_entry.mail.wstr,
			pb_entry.mail.wstr_len);
		psEntry->email.addressLen = pb_entry.mail.wstr_len;
#endif
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		psEntry->groupIndex = pb_entry.union_t.group;
#else
		psEntry->groupIndex = pb_entry.group_id;
#endif
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		if(MMIPB_IsInNvGroup(pb_entry.union_t.group))
#else
		if(MMIPB_IsInNvGroup(pb_entry.group_id))
#endif
		{        
			psEntry->location = E_MEX_PHB_LOCATION_PHONE;
		}
		else
		{ 
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 	
		if(pb_entry.union_t.group == PB_GROUP_SIM_1)
#else
		if(pb_entry.group_id == PB_GROUP_SIM_1)
#endif
			{
				psEntry->location = E_MEX_PHB_LOCATION_SIM1;
			}
			else
			{
				psEntry->location = E_MEX_PHB_LOCATION_SIM2;

			}
		}
	}

	return E_MEX_PHB_RESULT_SUCCESS;
}



uint8 MexPhs_SearchPhb_GetInputMode(void)
{
	return 0;
}
void MexPhs_SearchPhb_SetInputMode(uint8 mode)
{

}

void MexPhs_SearchPhb_ChangeInputMode(bool isPinyin)
{

}

void MexPhs_SearchPhb_MakeCache(void)
{
}

int32 MexPhs_SearchPhbEntry(wchar_t* szKeyword, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask)
{
	return 0;
}



static void MexPhs_InitPhbGlobalVars(Mex_Phs_Phb_Entry* sEntry)
{

}

void MexPhs_AddPhbEntryExCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_ADDENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_ADDENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	}    

}

void MexPhs_UpdatePhbEntryExCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_UPDATEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	}    
}

void MexPhs_RemovePhbEntryCB(uint16 uIndex, uint8 success)
{
	E_Mex_Phs_Phb_Event event;

	if (success == 1)
		event = E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_REMOVEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(uIndex, event);
		mex_SetCurrentApp(appId);
	} 
}


void MexPhs_AddPhbEntryExRsp(void *info)
{   
	xSignalHeaderRec   *pSig = (xSignalHeaderRec *)info;
	MEX_PHB_EVENT_T *pPhbEvent;
	E_Mex_Phs_Phb_Event event;
	uint16              iEntryId;

	pPhbEvent = (MEX_PHB_EVENT_T *)(pSig);

	if (pPhbEvent->Result)
		event = E_MEX_PHB_EVENT_ADDENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_ADDENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		iEntryId  = pPhbEvent->EntryID;
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(iEntryId, event);
		mex_SetCurrentApp(appId);
	}    
}


E_Mex_Phs_Phb_Result MexPhs_AddPhbEntryEx(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* psEntry)
{
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 

	extern E_Mex_Phs_Phb_Result mex_Phs_GetPhbMemoryStatus(E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed);

	extern MMIPB_SORT_AND_ENTRY_ID_T   g_mmipb_entry_list;      // sim phonebook中排序和entry_id的使用情况

	uint32 total, used;
	bool bPermission = false;
	PHONEBOOK_STORAGE_E  storage;
	uint8                group_id;
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	MMIPB_CONTACT_T pb_entry;
#else
	MMIPB_PHONEBOOK_ENTRY_T pb_entry;
#endif
	MMI_STRING_T             string_t;
	bool                     result;
	char                 number_str[27];
	MMI_PARTY_NUMBER_T   part_number;
	
	MMIPB_NAME_T*	 pEntry_name = NULL;
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
	pEntry_name = &(pb_entry.entry_name);
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
	pEntry_name = &(pb_entry.union_t.entry_name);
#endif

	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookAdd);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		//return E_MEX_PHB_RESULT_NO_PERMISSION; //tangtingtao20111026
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}
	// Check space
	mex_Phs_GetPhbMemoryStatus(eLocation, &total, &used);
	if (used >= total)
	{
		mex_log(MEX_LOG_PHS, "E:no free sapce at eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_NO_SPACE;
	}
	// Check current operation
	if (!MMIAPIPB_IsPbReady(FALSE))
	{
		mex_log(MEX_LOG_PHS, "E:phb is busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	//组建MMI 电话薄记录
	mex_memset((void *)&pb_entry, 0, sizeof(pb_entry));

	switch (eLocation)
	{
	case E_MEX_PHB_LOCATION_PHONE:
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		pb_entry.group_id = PB_GROUP_ALL;		 
#else
		pb_entry.group_id = PB_GROUP_UNCLASS;		 
#endif    
		storage   = PHONEBOOK_NV_STORAGE;
		pb_entry.entry_id = MMIPB_GetSpareId(&(g_mmipb_entry_list.nv_entry_id_t), pb_entry.group_id);
		break;
	case E_MEX_PHB_LOCATION_SIM1:
		storage   = PHONEBOOK_SIM_STORAGE;
		pb_entry.group_id = PB_GROUP_SIM_1;
		pb_entry.entry_id = MMIPB_GetSpareId(&(g_mmipb_entry_list.sim_entry_id_t), PB_GROUP_SIM_1);
		MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ADD_ONE);        
		break;
#ifdef MEX_DUAL_SIM
	case E_MEX_PHB_LOCATION_SIM2:
		storage   = PHONEBOOK_SIM_STORAGE;
		pb_entry.group_id = PB_GROUP_SIM_2;
		pb_entry.entry_id = MMIPB_GetSpareId(&(g_mmipb_entry_list.sim_entry_id_t), PB_GROUP_SIM_2);
		MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ADD_ONE);

		break;
#endif /* MEX_DUAL_SIM */
	default:
		mex_log(MEX_LOG_PHS, "E:wrong eLocation:%d", eLocation);
		return E_MEX_PHB_RESULT_WRONG_LOCATION;
	}

	g_mmipb_entry_t.entry_id = pb_entry.entry_id;
	g_mmipb_entry_t.group_id = pb_entry.group_id; 

	if(psEntry->name.nameLen > 0)
	{
		mex_wcsncpy((wchar_t *) pEntry_name->wstr,
			(wchar_t *) psEntry->name.name,
			psEntry->name.nameLen);

		pEntry_name->wstr_len = psEntry->name.nameLen;
	}

	//personal number
	if(psEntry->number.numberLen > 0)
	{
		string_t.wstr_ptr = psEntry->number.number;            
		string_t.wstr_len = psEntry->number.numberLen;

		if(MMIPB_StringIsValidNumber(string_t))
		{

			mex_wcs2nstr(number_str,
				sizeof(number_str),    
				(const char *)string_t.wstr_ptr);

			MMIAPICOM_GenPartyNumber(number_str,
				string_t.wstr_len,
				&part_number);

			pb_entry.number_t[0].npi_ton    = part_number.number_type;
			pb_entry.number_t[0].number_len = part_number.num_len;
			mex_memcpy(pb_entry.number_t[0].number,part_number.bcd_num,part_number.num_len);
		}

	}

	if(storage   == PHONEBOOK_NV_STORAGE)
	{
		//home number
		if(psEntry->number_home.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_home.number;            
			string_t.wstr_len = psEntry->number_home.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[1].npi_ton    = part_number.number_type;
				pb_entry.number_t[1].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[1].number,part_number.bcd_num,part_number.num_len);
			}
		}    
#ifndef PB_SUPPORT_LOW_MEMORY

		//company number
		if(psEntry->number_company.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_company.number;            
			string_t.wstr_len = psEntry->number_company.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[2].npi_ton    = part_number.number_type;
				pb_entry.number_t[2].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[2].number,part_number.bcd_num,part_number.num_len);
			}

		}        

		//company number
		if(psEntry->number_fax.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_fax.number;            
			string_t.wstr_len = psEntry->number_fax.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[4].npi_ton    = part_number.number_type;
				pb_entry.number_t[4].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[4].number,part_number.bcd_num,part_number.num_len);
			}

		}                
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
		if(psEntry->name_company.nameLen > 0)
		{
			mex_wcsncpy((wchar_t *)pb_entry.org.wstr,                    
				(wchar_t *)psEntry->name_company.name,
				psEntry->name_company.nameLen);

			pb_entry.org.wstr_len = psEntry->name_company.nameLen;
		}
#endif
#ifdef MMIPB_MAIL_SUPPORT
		if(psEntry->email.addressLen > 0)
		{
			mex_str2nwcs((wchar_t *)pb_entry.mail.wstr,
				sizeof(pb_entry.mail.wstr) * 2,
				(wchar_t *)psEntry->email.address);

			pb_entry.mail.wstr_len = psEntry->email.addressLen;
		}
	}
#endif

	result = MMIPB_UpdatePhonebookEntry(
		storage,
		(MMIPB_PHONEBOOK_ENTRY_T *)&pb_entry);


	if (result)
	{	        
		if(PHONEBOOK_NV_STORAGE == storage)
		{
			result = MMIPB_Add(&pb_entry);
		}
	}

	{
		int32              iEntryIndex;
		MEX_PHB_EVENT_T *sendSignal = NULL;

		// create the signal to send requesting task
		MmiCreateSignal(E_MSSC_PHB_ADD_IND, sizeof(MEX_PHB_EVENT_T), (MmiSignalS**)&sendSignal);
		sendSignal->Sender = MexConfig_ConvertSprdDef(E_M2MDC_MET_MEX_SEND);

		sendSignal->Result = result;
		sendSignal->GroupID = pb_entry.group_id;

		if(pb_entry.group_id == PB_GROUP_SIM_1)
		{        
			iEntryIndex = pb_entry.entry_id;
		}
#ifndef MEX_SINGLE_SIM        
		else if(pb_entry.group_id == PB_GROUP_SIM_2)
		{
			iEntryIndex = pb_entry.entry_id;
			iEntryIndex += mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL);
		}
#endif        
		else
		{            
			iEntryIndex = pb_entry.entry_id;
			iEntryIndex += mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL);
#ifndef MEX_SINGLE_SIM        
			iEntryIndex += mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL);
#endif

		}

		sendSignal->EntryID = iEntryIndex;

		MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
	}
#endif
	return E_MEX_PHB_RESULT_SUCCESS;
}



// mmi_phb_op_add_entry_req
E_Mex_Phs_Phb_Result MexPhs_AddPhbEntry(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));       /*lint !e611 */

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->name_company.name, (const wchar_t *)L"");
		strcpy(pEntry->email.address, "");
		mex_wcscpy(pEntry->number_home.number, (const wchar_t *)L"");
		mex_wcscpy(pEntry->number_fax.number, (const wchar_t *)L"");

		res = MexPhs_AddPhbEntryEx(eLocation, pEntry);

		mex_free(pEntry);       /*lint !e611 */
	}

	return res;
}


void MexPhs_UpdatePhbEntryRsp(void *info)
{
	xSignalHeaderRec       *pSig = (xSignalHeaderRec *)info;
	MEX_PHB_EVENT_T *pPhbEvent;
	E_Mex_Phs_Phb_Event event;

	pPhbEvent = (MEX_PHB_EVENT_T *)(pSig);

	if (pPhbEvent->Result)
		event = E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_UPDATEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		uint16 iEntryId  = pPhbEvent->EntryID;
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(iEntryId, event);
		mex_SetCurrentApp(appId);

	}    

}

E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry)
{
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 

	extern E_Mex_Phs_Phb_Result mex_Phs_GetPhbMemoryStatus(E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed);

	bool bPermission = false;
	PHONEBOOK_STORAGE_E  storage;
	uint8                group_id;
	uint16               entry_id;  
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	MMIPB_CONTACT_T pb_entry = {0};
#else
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		MMIPB_CONTACT_T pb_entry;
#else
	MMIPB_PHONEBOOK_ENTRY_T pb_entry;
#endif
#endif
	MMI_STRING_T             string_t;
	bool                     result;
	char                 number_str[27];
	MMI_PARTY_NUMBER_T   part_number;
	int                  index;
	uint16               entry_index;
	
	MMIPB_NAME_T*	 pEntry_name = NULL;
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
		pEntry_name = &(pb_entry.entry_name);
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	pEntry_name = &(pb_entry.name);
#else
	pEntry_name = &(pb_entry.union_t.entry_name);
#endif
#endif


	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookAdd);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		//return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	//将指定的索引分解为group_id和entry_id
	entry_id = uIndex;
	group_id = PB_GROUP_SIM_1;
	entry_index = uIndex;
	storage  = PHONEBOOK_SIM_STORAGE;
	if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL))
	{    
		entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL);
#ifndef MEX_SINGLE_SIM        
        group_id = PB_GROUP_SIM_2;

		if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL))
        {    
			entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
					group_id = PB_GROUP_ALL;	
#else
					group_id = PB_GROUP_UNCLASS;
#endif
            storage  = PHONEBOOK_NV_STORAGE;
        }
#else
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
					group_id = PB_GROUP_ALL;	
#else
					group_id = PB_GROUP_UNCLASS;
#endif
        storage  = PHONEBOOK_NV_STORAGE;
#endif
	}
	entry_index = entry_id + mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_USED);
	entry_index += mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_USED);


	// Check uIndex
	if (!MexPhs_PhbCheckIndex(entry_index))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	// Check current operation
	if (!MMIAPIPB_IsPbReady(FALSE))
	{
		mex_log(MEX_LOG_PHS, "E:phb is busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

	//组建MMI 电话薄记录
	mex_memset((void *)&pb_entry, 0, sizeof(pb_entry));

	pb_entry.entry_id = entry_id;
	pb_entry.group_id = group_id;    
	g_mmipb_entry_t.entry_id = entry_id;
	g_mmipb_entry_t.group_id = group_id; 

	storage = PHONEBOOK_SIM_STORAGE;
	if(MMIPB_IsInNvGroup(pb_entry.group_id))
	{
		storage = PHONEBOOK_NV_STORAGE;   
	}

	if(psEntry->name.nameLen > 0)
	{
		mex_wcsncpy((wchar_t *) pEntry_name->wstr,
			(wchar_t *) psEntry->name.name,
			psEntry->name.nameLen);

		pEntry_name->wstr_len = psEntry->name.nameLen;
	}

	//personal number
	if(psEntry->number.numberLen > 0)
	{
		string_t.wstr_ptr = psEntry->number.number;            
		string_t.wstr_len = psEntry->number.numberLen;

		if(MMIPB_StringIsValidNumber(string_t))
		{

			mex_wcs2nstr(number_str,
				sizeof(number_str),    
				(const char *)string_t.wstr_ptr);

			MMIAPICOM_GenPartyNumber(number_str,
				string_t.wstr_len,
				&part_number);

			pb_entry.number_t[0].npi_ton    = part_number.number_type;
			pb_entry.number_t[0].number_len = part_number.num_len;
			mex_memcpy(pb_entry.number_t[0].number,part_number.bcd_num,part_number.num_len);
		}

	}

	if(storage   == PHONEBOOK_NV_STORAGE)
	{
		//home number
		if(psEntry->number_home.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_home.number;            
			string_t.wstr_len = psEntry->number_home.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[1].npi_ton    = part_number.number_type;
				pb_entry.number_t[1].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[1].number,part_number.bcd_num,part_number.num_len);
			}
		}    
#ifndef PB_SUPPORT_LOW_MEMORY

		//company number
		if(psEntry->number_company.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_company.number;            
			string_t.wstr_len = psEntry->number_company.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[2].npi_ton    = part_number.number_type;
				pb_entry.number_t[2].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[2].number,part_number.bcd_num,part_number.num_len);
			}

		}        

		//company number
		if(psEntry->number_fax.numberLen > 0)
		{
			string_t.wstr_ptr = psEntry->number_fax.number;            
			string_t.wstr_len = psEntry->number_fax.numberLen;

			if(MMIPB_StringIsValidNumber(string_t))
			{        
				mex_wcs2nstr(number_str,
					sizeof(number_str),    
					(const char *)string_t.wstr_ptr);

				MMIAPICOM_GenPartyNumber(number_str,
					string_t.wstr_len,
					&part_number);

				pb_entry.number_t[4].npi_ton    = part_number.number_type;
				pb_entry.number_t[4].number_len = part_number.num_len;
				mex_memcpy(pb_entry.number_t[4].number,part_number.bcd_num,part_number.num_len);
			}

		}                
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
		if(psEntry->name_company.nameLen > 0)
		{
			mex_wcsncpy((wchar_t *)pb_entry.org.wstr,                    
				(wchar_t *)psEntry->name_company.name,
				psEntry->name_company.nameLen);

			pb_entry.org.wstr_len = psEntry->name_company.nameLen;
		}
#endif
#ifdef MMIPB_MAIL_SUPPORT
		if(psEntry->email.addressLen > 0)
		{
			mex_wcsncpy((wchar_t *)pb_entry.mail.wstr,
				(wchar_t *)psEntry->email.address, 
				psEntry->email.addressLen);

			pb_entry.mail.wstr_len = psEntry->email.addressLen;
		}
	}
#endif
	//电话薄搜索操作保护
	g_mmipb_process_t.is_processing = TRUE;

	g_mmipb_entry_t = pb_entry;

	result = MMIPB_UpdatePhonebookEntry(
		storage,
		(MMIPB_PHONEBOOK_ENTRY_T *)&pb_entry);
	//电话薄搜索操作保护
	g_mmipb_process_t.is_processing = FALSE;

	{
		MEX_PHB_EVENT_T *sendSignal = PNULL;

		// create the signal to send requesting task
		MmiCreateSignal(E_MSSC_PHB_UPDATE_IND, sizeof(MEX_PHB_EVENT_T), (MmiSignalS **)&sendSignal);
		sendSignal->Sender = MexConfig_ConvertSprdDef(E_M2MDC_MET_MEX_SEND);

		sendSignal->Result = result;
		sendSignal->GroupID = pb_entry.group_id;
		sendSignal->EntryID = uIndex;

		MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
	}
#endif
	return E_MEX_PHB_RESULT_SUCCESS;
}

E_Mex_Phs_Phb_Result MexPhs_UpdatePhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber)
{
	E_Mex_Phs_Phb_Result res = E_MEX_PHB_RESULT_ERROR_UNKNOWN;
	Mex_Phs_Phb_Entry *pEntry = mex_malloc(sizeof(Mex_Phs_Phb_Entry));       /*lint !e611 */

	if (pEntry)
	{
		mex_memset(pEntry, 0, sizeof(Mex_Phs_Phb_Entry));
		// 读取旧的信息
		MexPhs_GetPhbEntryEx(uIndex, pEntry);
		// 填充新的姓名和号码
		mex_memcpy(&pEntry->name, psName, sizeof(Mex_Phs_Phb_Name));
		mex_memcpy(&pEntry->number, psNumber, sizeof(Mex_Phs_Phb_Number));

		res = MexPhs_UpdatePhbEntryEx(uIndex, pEntry);
		mex_free(pEntry);       /*lint !e611 */
	}

	return res;
}


void MexPhs_RemovePhbEntryRsp(void *info)
{
	xSignalHeaderRec       *pSig = (xSignalHeaderRec *)info;
	MEX_PHB_EVENT_T *pPhbEvent;
	E_Mex_Phs_Phb_Event event;

	pPhbEvent = (MEX_PHB_EVENT_T *)(pSig);

	if (pPhbEvent->Result)
		event = E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS;
	else
		event = E_MEX_PHB_EVENT_REMOVEENTRY_FAIL;

	if (g_MexPhbCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		uint16 iEntryId  = pPhbEvent->EntryID;
		mex_SetCurrentApp(g_MexPhbAppId);
		(*g_MexPhbCallBack)(iEntryId, event);
		mex_SetCurrentApp(appId);
	}    

}



E_Mex_Phs_Phb_Result MexPhs_RemovePhbEntry(uint16 uIndex)
{
#if(MEX_SPRD_CODE_VERSION != 0x10A6530) 
	PHONEBOOK_STORAGE_E storage;
	MN_DUAL_SYS_E       dual_sys;
	uint16              entry_id;
	uint8               group_id;
	bool                result;
	uint16              entry_index;
	int                 i;
	bool bPermission = false;

	bPermission = mex_IsAppPermissiveToDo(mex_GetCurrentApp(), E_MAPT_PhonebookAdd);
	if (!bPermission)
	{
		mex_log(MEX_LOG_PHS, "E:no permission");
		//return E_MEX_PHB_RESULT_NO_PERMISSION;
	}

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY) ||
		(g_MexPhbCallBack != NULL && g_MexPhbAppId != mex_GetCurrentApp()))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

#if 0
	// Check uIndex
	if (!MexPhs_PhbCheckIndex(uIndex))
	{
		mex_log(MEX_LOG_PHS, "E:wrong index");
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}
#endif    

	// Check current operation
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
if (!MMIAPIPB_IsPbReady())
#else
if (!MMIAPIPB_IsPbReady(FALSE))
#endif	
	{
		mex_log(MEX_LOG_PHS, "E:phb is busy");
		return E_MEX_PHB_RESULT_BUSY;
	}


	//将指定的索引分解为group_id和entry_id
	entry_id = uIndex + 1; //tangtingtao 20111026
	group_id = PB_GROUP_SIM_1;
	if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL))
	{    
		entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM1_TOTAL);
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)   
		group_id = PB_GROUP_SIM_2;		
        if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL))   
		{    
            entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM2_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
										group_id = PB_GROUP_ALL;	
#else
										group_id = PB_GROUP_UNCLASS;
#endif
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
            group_id = PB_GROUP_SIM_3;
            if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM3_TOTAL))       
            {
                entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM3_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
											group_id = PB_GROUP_ALL;	
#else
											group_id = PB_GROUP_UNCLASS;
#endif
#if defined(MEX_QUAD_SIM)
                group_id = PB_GROUP_SIM_4;
                if(entry_id > mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM4_TOTAL))      
                {
                    entry_id -= mex_GetPhoneConfig(E_MPC_PHONEBOOK_MEM_SIM4_TOTAL);
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
												group_id = PB_GROUP_ALL;	
#else
												group_id = PB_GROUP_UNCLASS;
#endif
                }
#endif            
            }
#endif            
		}
#else
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
									group_id = PB_GROUP_ALL;	
#else
									group_id = PB_GROUP_UNCLASS;
#endif
#endif
	}

	dual_sys = MN_DUAL_SYS_1;
	storage = PHONEBOOK_SIM_STORAGE;
	if(MMIPB_IsInNvGroup(group_id))
	{
		storage = PHONEBOOK_NV_STORAGE;   
		g_mmipb_entry_t.entry_id = entry_id;
		g_mmipb_entry_t.group_id = group_id; 
	}
	else
	{
		if(PB_GROUP_SIM_1 == group_id)
		{
			//第一张SIM卡
			storage = PHONEBOOK_SIM_STORAGE;
			dual_sys = MN_DUAL_SYS_1;
			MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ERASE_ONE);
			g_mmipb_entry_t.entry_id = entry_id;
			g_mmipb_entry_t.group_id = group_id; 
		}
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		else if(PB_GROUP_SIM_2 == group_id)
		{        
			//第二张SIM卡
			storage = PHONEBOOK_SIM_STORAGE;
			dual_sys = MN_DUAL_SYS_2;
			MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ERASE_ONE);
			g_mmipb_entry_t.entry_id = entry_id;
			g_mmipb_entry_t.group_id = group_id;
		}
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		else if(PB_GROUP_SIM_3 == group_id)
		{        
			//第二张SIM卡
			storage = PHONEBOOK_SIM_STORAGE;
			dual_sys = MN_DUAL_SYS_3;
			MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ERASE_ONE);
			g_mmipb_entry_t.entry_id = entry_id;
			g_mmipb_entry_t.group_id = group_id;
		}
#endif
#if defined(MEX_QUAD_SIM)
		else if(PB_GROUP_SIM_4 == group_id)
		{        
			//第二张SIM卡
			storage = PHONEBOOK_SIM_STORAGE;
			dual_sys = MN_DUAL_SYS_4;
			MMIPB_SetOperateType(MMIPB_OPERATE_TYPE_ERASE_ONE);
			g_mmipb_entry_t.entry_id = entry_id;
			g_mmipb_entry_t.group_id = group_id;
		}
#endif
#endif

	}

	result = MMIPB_DeletePhonebookEntry(
		dual_sys, //dual sys later
		storage,
		entry_id
		);

	//SCI_TraceLow:"MexPhs_RemovePhbEntry result = %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXCONFIG_10376_112_2_18_2_36_29_6,(uint8*)"d",result);

	if (result)
	{   

		if(PHONEBOOK_NV_STORAGE == storage)
		{
			result = MMIPB_Erase(entry_id, group_id);

		}

	}


	{
		MEX_PHB_EVENT_T   *tPhbEvent;

		xSignalHeaderRec *sendSignal = NULL;

		// create the signal to send requesting task
		MmiCreateSignal(E_MSSC_PHB_REMOVE_IND, sizeof(MEX_PHB_EVENT_T), (MmiSignalS **)&sendSignal);
		sendSignal->Sender = MexConfig_ConvertSprdDef(E_M2MDC_MET_MEX_SEND);
		tPhbEvent = (MEX_PHB_EVENT_T *)(sendSignal + 1);

		tPhbEvent->Result = result;
		tPhbEvent->GroupID = group_id;
		tPhbEvent->EntryID = uIndex;

		MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
	}
#endif
	return E_MEX_PHB_RESULT_SUCCESS;
}


E_Mex_Phs_Phb_Result MexPhs_GetPhbGroup(uint16 groupIndex, Mex_Phs_Phb_Group *pGroup)
{
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	uint8   uiGroupCount;
#endif
	bool    bResult;

	mex_memset(pGroup, 0, sizeof(Mex_Phs_Phb_Group));   

	// Phone book is not ready
	if (0 == MexConfig_GetPhoneConfig(E_MPC_PHONEBOOK_READY))
	{
		mex_log(MEX_LOG_PHS, "E:Phb busy");
		return E_MEX_PHB_RESULT_BUSY;
	}

#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	bResult = MMIPB_GetGroupCount(&uiGroupCount);
	if(!bResult)
	{
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	if (groupIndex < uiGroupCount)
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
	if ((MMIPB_IsInSIMGroup(groupIndex)) ||
        (MMIPB_IsInNvGroup(groupIndex)))
#endif        
	{   
		MMIPB_NAME_T  pGroupName = {0};

		bResult = MMIPB_ReadGroupName(&pGroupName, groupIndex);
		if(bResult)
		{
			mex_wcscpy(pGroup->groupName.name, (const wchar_t *)pGroupName.wstr);

		}
		else
		{
			return E_MEX_PHB_RESULT_ERROR_UNKNOWN;
		}
	}
	else
	{
		return E_MEX_PHB_RESULT_WRONG_INDEX;
	}

	pGroup->groupName.nameLen = mex_wcslen(pGroup->groupName.name);
	pGroup->groupIndex = groupIndex;

	return E_MEX_PHB_RESULT_SUCCESS;
}



E_Mex_Phs_Phb_Result MexPhs_LookUpPhbNumber(const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName)
{
	return E_MEX_PHB_RESULT_SUCCESS;
}



void *MexConfig_ConvertStruct(E_MTK2MEX_Struct_Convert type,void **dst,void *src)
{
	extern void MexConfig_ConvertBCD2Ansi(int8 *ansi_str,int32 max_len,int8 *bcd_str,int32 bcd_len, int32 offset);

	switch(type)
	{
	case E_M2MSC_SockAddr_Struct:
		{			
			unsigned long ip_addr = 0;
			struct sci_sockaddr *dst_Struct = NULL;
			Mex_SockAddr_Struct *src_Struct = NULL;
			if((NULL == dst) || (NULL == *dst) || (NULL == src))
			{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
			}
			dst_Struct = (struct sci_sockaddr *)*dst;
			src_Struct = (Mex_SockAddr_Struct *)src;
			memcpy(&ip_addr, src_Struct->addr, 4);

			memset(dst_Struct->sa_data, 0, 8);
			dst_Struct->family  = AF_INET;
			dst_Struct->ip_addr = ip_addr;			
			dst_Struct->port    = src_Struct->port;			

		}
		break;

		//struct convert for hook
	case E_M2MSC_Hk_Sms_Deliver_Msg:        
		{
			Mex_Hk_Sms_Deliver_Msg *dst_Struct;           
			APP_MN_SMS_IND_T *src_Struct;
			uint8          number[41];

			if((NULL == dst) || (NULL == *dst) || (NULL == src))
			{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
			}


			dst_Struct = *dst;
			src_Struct = (APP_MN_SMS_IND_T *)src;

			//SMS Center Address
			mex_memset(&number, 0, sizeof(number));

			MexConfig_ConvertBCD2Ansi(
				(int8*)number,
				sizeof(number),
				(int8*)src_Struct->sms_t.sc_addr_t.party_num,
				src_Struct->sms_t.sc_addr_t.num_len,
				0);

			mex_str2nwcs(dst_Struct->sca_number.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1)<<1, (const char *)number);
			dst_Struct->sca_number.numberLen = mex_wcslen(dst_Struct->sca_number.number);

			//SMS Sender Address                           
			mex_memset(&number, 0, sizeof(number));

			MexConfig_ConvertBCD2Ansi(
				(int8*)number,
				sizeof(number),
				(int8*)src_Struct->sms_t.origin_addr_t.party_num,
				src_Struct->sms_t.origin_addr_t.num_len,
				0);

			mex_str2nwcs(dst_Struct->oa_number.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1)<<1, (const char *)number);
			dst_Struct->oa_number.numberLen = mex_wcslen(dst_Struct->oa_number.number);

			dst_Struct->pid = src_Struct->sms_t.pid_e;
			dst_Struct->dcs = (src_Struct->sms_t.dcs.alphabet_type << 2);

			dst_Struct->index = src_Struct->record_id;
			dst_Struct->storage_type = src_Struct->storage;
			dst_Struct->no_msg_data  = src_Struct->sms_t.user_data_t.length - 1;
			dst_Struct->msg_data = (uint8 *)&src_Struct->sms_t.user_data_t.user_data_arr[1]; //The first byte is the real msg length
			return dst_Struct;
		}
	case E_M2MSC_Hk_Cc_Call_Ring:
		{
			Mex_Hk_Cc_Call_Ring *dst_Struct = {0};            
			APP_MN_SETUP_IND_T *src_Struct;
			uint8          number[41];
			if((NULL == dst) || (NULL == *dst) || (NULL == src))
			{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
			}
			dst_Struct = *dst;
			src_Struct = src;

			if(src_Struct->calling_num_present)
			{                
				mex_memset(&number, 0, sizeof(number));

				MexConfig_ConvertBCD2Ansi(
					(int8*)number,
					sizeof(number),
					(int8*)src_Struct->calling_num.party_num,
					src_Struct->calling_num.num_len,
					0);

				mex_str2nwcs(dst_Struct->num.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1)<<1, (const char *)number);
				dst_Struct->num.numberLen = mex_wcslen(dst_Struct->num.number);
			}
			if(src_Struct->calling_sub_addr_present)
			{                            
				mex_memset(&number, 0, sizeof(number));

				MexConfig_ConvertBCD2Ansi(
					(int8*)number,
					sizeof(number),
					(int8*)src_Struct->calling_sub_addr.sub_num,
					src_Struct->calling_sub_addr.addr_len,
					0);

				mex_str2nwcs(dst_Struct->sub_addr.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1)<<1, (const char *)number);
				dst_Struct->sub_addr.numberLen = mex_wcslen(dst_Struct->sub_addr.number);
			}

			dst_Struct->call_id = src_Struct->call_id;
			return dst_Struct;
		}

	default:
		if((NULL == dst) || (NULL == *dst) || (NULL == src))
		{
            mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
            return NULL;
		}
	}
	return 0;
}
int32 MexConfig_ConvertSprdDef(E_SYS2MEX_Def_Convert param)
{
	switch (param)
	{
	case E_S2MDC_BLM_MEX_MEM_INTERNAL:
		return BLOCK_MEM_POOL_MET_MEX_MEM_INTERNAL;
	case E_S2MDC_BLM_MEX_MEM_EXTERNAL:
		return BLOCK_MEM_POOL_MET_MEX_MEM_EXTERNAL;
	case E_S2MDC_BLM_MEX_MEM_RESERVED:
		return BLOCK_MEM_POOL_MET_MEX_MEM_RESERVED;
	case E_S2MDC_BLM_NOT_USE:
		return NOT_USE;
		//Mdi Begin
	case E_S2MDC_ADR_NO_ERROR:
		return AUDIO_NO_ERROR;
	case E_S2MDC_ADR_AUDIO_STOP:
		return AUDIO_MULTI_STOP;
	case E_S2MDC_ADR_AUDIO_ERROR_IND:
		return AUDIO_ERROR_IND;	
	case E_S2MDC_ADR_AUDIO_PLAYEND_IND:
		return AUDIO_PLAYEND_IND;	
	case E_S2MDC_ADR_AUDIO_MAX_VOL:
		return MMISET_VOL_MAX;
		
		//Mdi End
		//Socket Begin

	case E_S2MDC_PF_INET:
		return AF_INET;
	case E_S2MDC_IPPROTO_IP:
		return 0;
	case E_S2MDC_SOC_SUCCESS:
		return 0;
	case E_S2MDC_SOC_ERROR:
		return -1;
	case E_S2MDC_SOC_WOULDBLOCK:
		return -2;

	case E_S2MDC_SOC_STREAM:
		return SOCK_STREAM;
	case E_S2MDC_SOC_DGRAM:
		return SOCK_DGRAM ;
	case E_S2MDC_SOC_RAW:
		return SOCK_RAW;
	case E_S2MDC_SOC_SMS:
		return -1;
	case E_S2MDC_SOC_DEBUG:
		return SO_DEBUG;
	case E_S2MDC_SOC_ACCEPTCONN:
		return SO_ACCEPTCONN;
	case E_S2MDC_SOC_REUSEADDR:
		return SO_REUSEADDR;
	case E_S2MDC_SOC_KEEPALIVE:
		return SO_KEEPALIVE;
	case E_S2MDC_SOC_OOBINLINE:
		return SO_OOBINLINE;
	case E_S2MDC_SOC_NBIO:
		return SO_NBIO;
	case E_S2MDC_SOC_BIO:
		return SO_BIO;
	case E_S2MDC_SOC_MYADDR:
		return SO_MYADDR;
	case E_S2MDC_SOC_RXDATA:
		return SO_RXDATA;
	case E_S2MDC_SOC_TXDATA:
		return SO_TXDATA;
	case E_S2MDC_SOC_AF_INET:
		return AF_INET;
	case E_S2MDC_SOC_LINGER:
		return -1;
	case E_S2MDC_SOC_ASYNC:
		return 8;

		//value by mtk
	case E_S2MDC_SOC_READ:
		return 0x01;
	case E_S2MDC_SOC_WRITE:
		return 0x02;
	case E_S2MDC_SOC_ACCEPT:
		return 0x04;
	case E_S2MDC_SOC_CONNECT:
		return 0x08;
	case E_S2MDC_SOC_CLOSE:
		return 0x10;


#if (MEX_SDK_VER >= 1018)   //支持三SIM卡
    case E_S2MDC_MN_DUAL_SYS_1:
        return MN_DUAL_SYS_1;
    case E_S2MDC_MN_DUAL_SYS_2:
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return MN_DUAL_SYS_2;
#else
		return 0;		
#endif        
    case E_S2MDC_MN_DUAL_SYS_3:
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return MN_DUAL_SYS_3;
#else
		return 0;		
#endif        
    case E_S2MDC_CC_SIM1_CALL:
        return CC_SIM1_CALL;
    case E_S2MDC_CC_SIM2_CALL:
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return CC_SIM2_CALL;
#else
		return 0;		
#endif        
    case E_S2MDC_CC_SIM3_CALL:
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		return CC_SIM3_CALL;
#else
		return 0;		
#endif
#endif

#if (MEX_SDK_VER >= 1019)   //支持字体转换,移植人员根据项目调整
    case E_S2MDC_SONG_FONT_16:
        return SONG_FONT_16;
    case E_S2MDC_SONG_FONT_12:
        return SONG_FONT_12;
#endif

#if (MEX_SDK_VER >= 1020)   //增加SOCKET状态码
    case E_M2MDC_SOC_SS_NOFDREF:            /* no file table ref any more */
        return SS_NOFDREF;
    case E_M2MDC_SOC_SS_ISCONNECTED:       /* socket connected to a peer */
        return SS_ISCONNECTED;        
    case E_M2MDC_SOC_SS_ISCONNECTING:      /* in process of connecting to peer */
        return SS_ISCONNECTING;        
    case E_M2MDC_SOC_SS_ISDISCONNECTING:   /*  in process  of disconnecting */
        return SS_ISDISCONNECTING;        
    case E_M2MDC_SOC_SS_CANTSENDMORE:      /* can't send more data to peer */
        return SS_CANTSENDMORE;        
    case E_M2MDC_SOC_SS_CANTRCVMORE:       /* can't receive more data from peer */
        return SS_CANTRCVMORE;        
    case E_M2MDC_SOC_SS_RCVATMARK:         /* at mark on input */
        return SS_RCVATMARK;        
    case E_M2MDC_SOC_SS_PRIV:              /* privileged for broadcast, raw... */
        return SS_PRIV;        
    case E_M2MDC_SOC_SS_NBIO:              /* non-blocking ops */
        return SS_NBIO;        
    case E_M2MDC_SOC_SS_ASYNC:             /* async i/o notify */
        return SS_ASYNC;       
    case E_M2MDC_SOC_SS_UPCALLED:          /* zerocopy data has been upcalled (for select) */
        return SS_UPCALLED;        
    case E_M2MDC_SOC_SS_INUPCALL:          /* inside zerocopy upcall (reentry guard) */
        return SS_INUPCALL;        
    case E_M2MDC_SOC_SS_UPCFIN:            /* inside zerocopy upcall (reentry guard) */
        return SS_UPCFIN;        
    case E_M2MDC_SOC_SS_WASCONNECTING:     /* SS_ISCONNECTING w/possible pending error */
        return SS_WASCONNECTING;   
	case E_M2MDC_MET_MEX_SEND:
		return MET_MEX_SEND;
#endif

    case E_S2MDC_MED_PCM_SRC_OK:
        return AUDIO_STREAM_GET_SRC_SUCCESS;
    case E_S2MDC_MED_PCM_SRC_ERROR:
        return AUDIO_STREAM_GET_SRC_ERROR;
    case E_S2MDC_MED_WAV_TYPE_PCM:
        return WAV_PCM;
	default:
        mex_log(MEX_LOG_SYSTEM,"can't convert unkonwn def %d",param);
        return;
	}

	return 0;
}

bool MexPhs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog)
{
	uint8 record_index = 0;
	MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = NULL;
	MMICL_CALL_BCDINFO_T      * pCallInfo =  NULL;
	MMICL_CALL_BCDINFO_T      * pGivenCallInfo =  NULL;
	MMICL_CALL_TYPE_E           eCallLogType;
	MN_DUAL_SYS_E               eSimType;
	bool                        bResult;
	int32                       iIndex = 0; 
	MN_RETURN_RESULT_E          eErrorCode;

	switch (simId)
	{
	case E_MEX_SIM_1:
		eSimType = MN_DUAL_SYS_1;
		break;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	case E_MEX_SIM_2:
		eSimType = MN_DUAL_SYS_2;
		break;
#endif  
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	case E_MEX_SIM_3:
		eSimType = MN_DUAL_SYS_3;
		break;
#endif  

	default:
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return false;
	}


	// pLogCall & nLogCall
	switch (callType)
	{
	case E_MCT_CALLLOG_DIALED:
		eCallLogType = MMICL_CALL_DIALED;
		break;
	case E_MCT_CALLLOG_MISSED:
		eCallLogType = MMICL_CALL_MISSED;
		break;
	case E_MCT_CALLLOG_RECVD:
		eCallLogType = MMICL_CALL_RECEIVED;
		break;
	case E_MCT_CALLLOG_MIXED:
		eCallLogType = MMICL_CALL_TYPE_MAX;
		break;
	default:
		mex_log(MEX_LOG_PHS, "E:wrong callType");
		return false;
	}

	arraycall_info = mex_malloc(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));       /*lint !e611 */
	if(NULL == arraycall_info)
	{
		return false;
	}

	//read logs of calls from NV
	if(eCallLogType != MMICL_CALL_TYPE_MAX)
	{     
		mex_memset(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
		eErrorCode = MMICL_ReadNV(eCallLogType, arraycall_info);
		if(eErrorCode == MN_RETURN_SUCCESS)
		{
			record_index = 0;
			pCallInfo  = arraycall_info->call_bcdinfo;
			for(iIndex = 0; iIndex < arraycall_info->record_num; iIndex++)
			{
				if(pCallInfo->dual_sys == eSimType)
				{               
					if(record_index == index)
					{
						pGivenCallInfo = pCallInfo;
						break;
					}
					record_index += 1;
				}

				pCallInfo++;
			}
		}        
	}
	else
	{
		record_index = 0;
		for(eCallLogType = MMICL_CALL_MISSED; eCallLogType <= MMICL_CALL_DIALED;eCallLogType++)
		{            
			mex_memset(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
			eErrorCode = MMICL_ReadNV(eCallLogType, arraycall_info);
			if(eErrorCode == MN_RETURN_SUCCESS)
			{              
				pCallInfo  = arraycall_info->call_bcdinfo;
				for(iIndex = 0; iIndex < arraycall_info->record_num; iIndex++)
				{
					if(pCallInfo->dual_sys == eSimType)
					{
						if(record_index == index)
						{
							pGivenCallInfo = pCallInfo;
							break;
						}
						record_index += 1;
					}

					pCallInfo++;
				}
			}        

			//判断是否已发现指定的记录
			if(iIndex < arraycall_info->record_num)
			{
				break;
			}

		}   

	}    

	//清楚输出的呼叫记录的内容
	mex_memset((void *)callLog, 0x0, sizeof(Mex_CallLog_Info));
	bResult = false;

	//复制呼叫记录的内容
	if (pGivenCallInfo)
	{
		char   szCallLogNumber[MAX_CALLLOG_NUMBER_LENGTH] = {0};

		extern void MexConfig_ConvertBCD2Ansi(int8 *ansi_str,int32 max_len,int8 *bcd_str,int32 bcd_len, int32 offset);

		callLog->numberLen = 0;
		if(pGivenCallInfo->is_num_present)
		{
			MexConfig_ConvertBCD2Ansi(
				(int8 *)szCallLogNumber,
				(int32) MAX_CALLLOG_NUMBER_LENGTH,
				(int8 *)pGivenCallInfo->number,
				(int32) pGivenCallInfo->number_len,
				(int32) 0);  

			mex_wcscpy((wchar_t*) callLog->number,mex_tstr2wcs((const char *) szCallLogNumber));

			callLog->numberLen = mex_wcslen( callLog->number);
		}

		callLog->pbNameLen = 0;
#ifdef CALL_TIMES_SUPPORT
		mex_UtcSec2DateTime(pGivenCallInfo->call_start_time[0], &callLog->startTime);
		mex_UtcSec2DateTime(pGivenCallInfo->call_duration_time[0], &callLog->duration);
#else
		mex_UtcSec2DateTime(pGivenCallInfo->call_start_time, &callLog->startTime);
		mex_UtcSec2DateTime(pGivenCallInfo->call_duration_time, &callLog->duration);
#endif        

		callLog->nTimes = 1;
		bResult = true;
	}

	mex_free(arraycall_info);       /*lint !e611 */

	return bResult;
}

//获取通话记录模块的状态
//参数1：无
//返回：E_MEX_CALLOG_STATUS 通话记录模块的状态
E_MEX_CALLOG_STATUS MexPhs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	return E_MCS_CALLLOG_READY;
}

//更新通话记录
//参数1：无
//返回：是否成功
bool MexPhs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	return true; // Default return false
}

uint32 MexPhs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType)
{
	uint8 record_num = 0;
	MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = NULL;
	MMICL_CALL_BCDINFO_T      * pCallInfo =  NULL;
	MMICL_CALL_TYPE_E           eCallLogType;
	MN_DUAL_SYS_E               eSimType;
	MN_RETURN_RESULT_E          eErrorCode;
	int32                       iIndex;    

	switch (simId)
	{
	case E_MEX_SIM_1:
		eSimType = MN_DUAL_SYS_1;
		break;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	case E_MEX_SIM_2:
		eSimType = MN_DUAL_SYS_2;
		break;
#endif  
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	case E_MEX_SIM_3:
		eSimType = MN_DUAL_SYS_3;
		break;
#endif  
	default:
		mex_log(MEX_LOG_PHS, "E:wrong simId");
		return 0;
	}


	// pLogCall & nLogCall
	switch (callType)
	{
	case E_MCT_CALLLOG_DIALED:
		eCallLogType = MMICL_CALL_DIALED;
		break;
	case E_MCT_CALLLOG_MISSED:
		eCallLogType = MMICL_CALL_MISSED;
		break;
	case E_MCT_CALLLOG_RECVD:
		eCallLogType = MMICL_CALL_RECEIVED;
		break;
	case E_MCT_CALLLOG_MIXED:
		eCallLogType = MMICL_CALL_TYPE_MAX;
		break;
	default:
		mex_log(MEX_LOG_PHS, "E:wrong callType");
		return 0;
	}

	arraycall_info = mex_malloc(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));       /*lint !e611 */
	if(NULL == arraycall_info)
	{
		return 0;
	}

	//read logs of calls from NV
	if(eCallLogType != MMICL_CALL_TYPE_MAX)
	{     
		mex_memset(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
		eErrorCode = MMICL_ReadNV(eCallLogType, arraycall_info);
		if(eErrorCode == MN_RETURN_SUCCESS)
		{
			record_num = 0;
			pCallInfo  = arraycall_info->call_bcdinfo;
			for(iIndex = 0; iIndex < arraycall_info->record_num; iIndex++)
			{
				if(pCallInfo->dual_sys == eSimType)
				{
					record_num += 1;
				}

				pCallInfo++;
			}
		}        
	}
	else
	{
		record_num = 0;
		for(eCallLogType = MMICL_CALL_MISSED; eCallLogType <= MMICL_CALL_DIALED;eCallLogType++)
		{            
			mex_memset(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
			eErrorCode = MMICL_ReadNV(eCallLogType, arraycall_info);
			if(eErrorCode == MN_RETURN_SUCCESS)
			{              
				pCallInfo  = arraycall_info->call_bcdinfo;
				for(iIndex = 0; iIndex < arraycall_info->record_num; iIndex++)
				{
					if(pCallInfo->dual_sys == eSimType)
					{
						record_num += 1;
					}

					pCallInfo++;
				}
			}        
		}        
	}    

	mex_free(arraycall_info);       /*lint !e611 */

	return record_num;
}

uint32 MexPhs_GetMissedCallNumber(void)
{
	extern uint32 MMIAPICC_GetMissedCallNumber(void);
	return MMIAPICC_GetMissedCallNumber();
}

bool MexPhs_ClearMissedCallNum(void)
{
	extern PUBLIC BOOLEAN MMIAPICC_ClearMissedCallNum(void);
	return MMIAPICC_ClearMissedCallNum();
}

bool MexConfig_CameraDBuffer(void)
{
	return MEX_CAMERA_DBUFFER;
}

//============================================================ Phone book interface end
#endif // MTK


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 

static bool g_Mex_Sms_SendMessage = false;

//判断短信发送是否已经准备就绪
bool MexPhs_IsSendSmsReady(E_MEX_Sim_Card_ID simId)
{       
	return (!MMIAPISMS_IsSendingSMS());
}

bool MexPhs_IsSmsSend()
{
	return g_Mex_Sms_SendMessage;
}

void MexPhs_SmsSendRsp(void* number, module_type mod, uint16 uResult)
{
	E_MEX_PHS_SMS_RESULT res;

	// mex_log(MEX_LOG_PHS, "SmsSendRsp res:%d", uResult);

	res = E_MPSR_RESULT_FAIL;
	if(uResult)
	{
		res = E_MPSR_RESULT_SUCCESS;
	}

	mex_Phs_SmsSendCallBack(res);

	g_Mex_Sms_SendMessage = false;
}


//实际发送短信的处理
bool MexPhs_SmsSend(E_MEX_Sim_Card_ID simId, const char* szDestNumber, const char* szContent, E_MEX_PHS_SMS_ENCODE eEncode)
{
	wchar_t  *szContent_ptr;
	wchar_t  szContent_w[512];
	uint16   uiContentLen;
	MN_DUAL_SYS_E sim_index;
	MMISMS_OPER_ERR_E  error_code;

	if (MexPhs_IsSendSmsReady(simId))
	{
		//初始化发送模块
		#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
		MMIAPISMS_SendInit();
	    #endif

		//设置需要发送的信息内容
		szContent_ptr = (wchar_t *)szContent;
		if(eEncode == E_MPSE_ENCODE_ASCII)
		{
			uiContentLen = mex_strlen(szContent);
			mex_str2nwcs(
				szContent_w,
				1024,
				szContent);
			szContent_ptr = szContent_w;
		}
		else
		{
			uiContentLen = mex_wcslen((const wchar_t*)szContent);
		}
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
	MMISMS_SetGloblalMsgContent(uiContentLen,szContent_ptr);
#else
	MMISMS_SetMessageContent(uiContentLen,szContent_ptr, FALSE, &g_mmisms_global.edit_content);
#endif

		//设置接收方号码
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
	MMISMS_SetDestNum(szDestNumber,mex_strlen(szDestNumber));
#else
	MMISMS_SetAddressToMessage((const uint8*)szDestNumber, mex_strlen(szDestNumber), &g_mmisms_global.edit_content.dest_info.dest_list);//
#endif

		//设置发送SIM卡
		sim_index = MN_DUAL_SYS_1;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		if(simId == E_MEX_SIM_2)
		{
			sim_index = MN_DUAL_SYS_2;
		}
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
		else if(simId == E_MEX_SIM_3)
		{
			sim_index = MN_DUAL_SYS_3;
		}
#if defined(MEX_QUAD_SIM)        
		else if(simId == E_MEX_SIM_4)
		{
			sim_index = MN_DUAL_SYS_4;
		}
#endif        
#endif  
#endif  

        mex_log(MEX_LOG_SYSTEM,"MexPhs_SmsSend sim_index:%d",sim_index);

		MMISMS_SetCurSendDualSys(sim_index);
		MMISMS_SetDestDualSys(sim_index);

		MMISMS_ClearDestId();

	       error_code = MMISMS_WintabSendSMS(false);
		
		mex_log(MEX_LOG_SYSTEM,"MexPhs_SmsSend error_code:%d",error_code);
			
		if(error_code != MMISMS_NO_ERR)
		{

			return false;
		}
		else
		{
			g_Mex_Sms_SendMessage = true;
			return true;
		}
	}

	return false;
}

#endif // Spread

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

//Convert L4C number
void MexConfig_Convert_L4c_Number(Mex_Phs_Phb_Number *dest, l4c_number_struct *source)
{
	char tempNumber[MEX_H_PHS_PHB_NUMBER_LEN + 1] = { 0 };

	if (((source->type & 0x90) == 0x90) && (source->type != 0xFF))
	{
		tempNumber[0] = '+';
		mex_strcpy((char *)(tempNumber + 1), (const char *)source->number);
	}
	else
	{
		mex_strcpy((char *)tempNumber, (const char *)source->number);
	}

	mex_str2nwcs(dest->number, (MEX_H_PHS_PHB_NUMBER_LEN + 1)<<1, (const char *)tempNumber);
	dest->numberLen = mex_wcslen(dest->number);

}

void *MexConfig_ConvertStruct(E_MTK2MEX_Struct_Convert type,void **dst,void *src)
{
	switch(type)
	{
	case E_M2MSC_SockAddr_Struct:
		{
			sockaddr_struct *dst_Struct;
			Mex_SockAddr_Struct *src_Struct;
    		if((NULL == dst) || (NULL == *dst) || (NULL == src))
    		{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
    		}
			dst_Struct = (sockaddr_struct *)*dst;
			src_Struct = (Mex_SockAddr_Struct *)src;

			dst_Struct->sock_type = src_Struct->sock_type;
			dst_Struct->port = src_Struct->port;
			dst_Struct->addr_len = src_Struct->addr_len;
			memcpy(dst_Struct->addr, src_Struct->addr, src_Struct->addr_len);
			return dst_Struct;
		}
		break;

		//struct convert for hook
	case E_M2MSC_Hk_Sms_Deliver_Msg:  
#if (MEX_MTK_CODE_VERSION < 0x0952)
		{
			Mex_Hk_Sms_Deliver_Msg *dst_Struct;
			mmi_frm_sms_deliver_msg_struct *src_Struct;
    		if((NULL == dst) || (NULL == *dst) || (NULL == src))
    		{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
    		}
			dst_Struct = *dst;
			src_Struct = src;

			MexConfig_Convert_L4c_Number(&dst_Struct->sca_number, &src_Struct->sca_number);
			MexConfig_Convert_L4c_Number(&dst_Struct->oa_number, &src_Struct->addr_number);

			dst_Struct->pid = src_Struct->pid;
			dst_Struct->dcs = src_Struct->dcs;
			dst_Struct->ori_dcs = src_Struct->ori_dcs;
			dst_Struct->index = src_Struct->index;
			dst_Struct->dest_port = src_Struct->dest_port;
			dst_Struct->src_port = src_Struct->src_port;
			dst_Struct->mti = src_Struct->mti;
			dst_Struct->display_type = src_Struct->display_type;
#ifdef __MMI_MESSAGES_COPY__
			dst_Struct->storage_type = src_Struct->storageType;
#endif /* __MMI_MESSAGES_COPY__ */
			dst_Struct->concat_info = src_Struct->concat_info;
			dst_Struct->no_msg_data = src_Struct->no_msg_data;
			dst_Struct->msg_data = (uint8 *)src_Struct->concat_info;
			return dst_Struct;
		}
#else
		{
			Mex_Hk_Sms_Deliver_Msg *dst_Struct;
			srv_sms_event_struct* event_data;
			srv_sms_event_new_sms_struct* event_info;
			srv_sms_new_msg_struct *new_msg_data;
			
    		if((NULL == dst) || (NULL == *dst) || (NULL == src))
    		{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
    		}

			event_data = (srv_sms_event_struct *) src;
			event_info = (srv_sms_event_new_sms_struct*)event_data->event_info;
			new_msg_data = (srv_sms_new_msg_struct*)event_info->msg_data;

			dst_Struct = *dst;

			mex_str2nwcs((wchar_t *) dst_Struct->oa_number.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1) * 2, (const char *) new_msg_data->number);
			mex_str2nwcs((wchar_t *) dst_Struct->sca_number.number, (MEX_H_PHS_PHB_NUMBER_LEN + 1) * 2, (const char *) new_msg_data->sc_number);
			dst_Struct->oa_number.numberLen = mex_wcslen(dst_Struct->oa_number.number);
			dst_Struct->sca_number.numberLen = mex_wcslen(dst_Struct->sca_number.number);
			dst_Struct->pid = new_msg_data->pid;
			dst_Struct->dcs = new_msg_data->dcs; 
			dst_Struct->ori_dcs = 0; // unknown
			dst_Struct->index = 0; // unknown
			dst_Struct->dest_port = new_msg_data->dest_port;
			dst_Struct->src_port = new_msg_data->src_port;
			dst_Struct->mti = new_msg_data->mti;
			dst_Struct->display_type = 0; // unknown
			dst_Struct->storage_type = new_msg_data->storage_type;
			dst_Struct->concat_info = 0; // unknown
			dst_Struct->no_msg_data = new_msg_data->message_len;
			dst_Struct->msg_data = (uint8 *)event_info->content;

			// convert ascii to usc2
			if (dst_Struct->dcs == SMSAL_8BIT_DCS || dst_Struct->dcs == SMSAL_DEFAULT_DCS)
			{
				uint8 *asciiData = (uint8 *)MexInterface_Med_Alloc(dst_Struct->no_msg_data);

				if (asciiData != NULL)
				{
					memcpy(asciiData, dst_Struct->msg_data, dst_Struct->no_msg_data);
					MexInterface_AsciiToUSC2((signed char*)dst_Struct->msg_data, (const signed char*)asciiData, (int)dst_Struct->no_msg_data);
					dst_Struct->dcs = SMSAL_UCS2_DCS;
					dst_Struct->no_msg_data = mex_wcslen((const wchar_t *)dst_Struct->msg_data) * 2;
					MexInterface_Med_Free(asciiData);
				}
			}

			return dst_Struct;
		}
#endif
		break;
	case E_M2MSC_Hk_Cc_Call_Ring:
		{
			Mex_Hk_Cc_Call_Ring *dst_Struct;
			mmi_cc_call_ring_ind_struct *src_Struct;
			
    		if((NULL == dst) || (NULL == *dst) || (NULL == src))
    		{
                mex_log(MEX_LOG_SYSTEM,"fail to convert NULL type %d pointer",type);
                return NULL;
    		}
			dst_Struct = *dst;
			src_Struct = src;

			MexConfig_Convert_L4c_Number(&dst_Struct->num, &src_Struct->num);
			MexConfig_Convert_L4c_Number(&dst_Struct->sub_addr, (l4c_number_struct *)&src_Struct->sub_addr);
			MexConfig_Convert_L4c_Number(&dst_Struct->redirect_num, &src_Struct->redirect_num);

			mex_memcpy(dst_Struct->name, src_Struct->name, 30);

			dst_Struct->call_id = src_Struct->call_id;
			dst_Struct->name_present = src_Struct->name_present;
			dst_Struct->cnap_info_exist = src_Struct->cnap_info_exist;
			dst_Struct->tag = src_Struct->tag;
			dst_Struct->dcs = src_Struct->dcs;
			dst_Struct->length = src_Struct->length;
			dst_Struct->auto_answer = src_Struct->auto_answer;
			dst_Struct->call_type = src_Struct->call_type;
			return dst_Struct;
		}
		break;
		//end

	default:
        mex_log(MEX_LOG_SYSTEM,"fail to convert type %d ",type);
        break;
	}
	return 0;
}

//TODO:  needtodo zgs
int8 MexConfig_EntrySystemApp(E_MEX_Entry_System_App app, void *pParam)
{
	switch(app)
	{
	case E_MESA_PHONEBOOK://进入电话薄
		{
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
 			extern MMI_ID vapp_contact_launch(void* args, U32 argSize);
			vapp_contact_launch(NULL, NULL);
#else

			extern void mmi_phb_entry_main_menu(void);
			mmi_phb_entry_main_menu();
#endif
			return 1;
		}
		break;

	case E_MESA_SMS://进入短信菜单
		{
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))  
			extern MMI_ID vapp_msg_launch(void *param, U32 param_size);
			vapp_msg_launch(NULL, NULL);
            return 1;
#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			extern void mmi_um_entry_main_message_menu(void);
			mmi_um_entry_main_message_menu();
			return 1;
#else 
			extern void EntryScrMessagesMenuList(void);
			EntryScrMessagesMenuList();
			return 1;
#endif
		}
		break;
	case E_MESA_NEWSMS: //进入编辑发送短信程序
		{		
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))    
			extern mmi_id vcui_unifiedcomposer_create(mmi_id parent_id, VappUcEntryStruct *data);
			vcui_unifiedcomposer_create(GRP_ID_ROOT,NULL);
#elif (MEX_MTK_CODE_VERSION >= 0x0932)
			extern void mmi_msg_pre_entry_write_new_msg(void);
			mmi_msg_pre_entry_write_new_msg();
#else
			extern void mmi_msg_entry_write_msg(void);
			mmi_msg_entry_write_msg();
#endif
			return 1;
		}
		break;
	case E_MESA_SMSINBOX://收件箱
		{
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))  
			extern MMI_ID vapp_msg_launch(void *param, U32 param_size);
			vapp_msg_launch(NULL, NULL);
			return 1;		
#elif (MEX_MTK_CODE_VERSION >= 0x1032)

#ifdef __MMI_UM_CONVERSATION_BOX__
				extern void mmi_um_entry_conversation(void);

				if (MMI_TRUE == mmi_um_check_conversation_box_setting())
				{
					mmi_um_entry_conversation();
				}
				else
#endif /* __MMI_UM_CONVERSATION_BOX__ */
				{
					mmi_um_entry_inbox();
				}

			return 0;
#elif (MEX_MTK_CODE_VERSION >= 0x0932)
#if (!defined(__MMI_UNIFIED_MESSAGE__) && (MEX_MTK_CODE_VERSION != 0x1012))
			extern void mmi_sms_entry_inbox_list(void);
			mmi_sms_entry_inbox_list();     
			return 1;
#endif
#else
			extern void mmi_msg_pre_entry_inbox_list(void);
			mmi_msg_pre_entry_inbox_list();	
			return 1;
#endif
			return 0;
		}
		break;
	case E_MESA_WAP: //WAP 浏览器
		{
			
#if defined( WAP_SUPPORT) || defined(BROWSER_SUPPORT)
			//MexInterface_ConnectWapHomepage();
			if( MexSystem_IsSysRunning() )
			{
				extern void MexSystem_Shutdown( E_Mex_PlatformShutdown eShutdownMode );
				MexSystem_Shutdown(E_MPS_HotShutdown);
			}
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))&&(defined(OPERA_V10_BROWSER))
			{
				extern void opera_brw_api_start_browser(void);
				opera_brw_api_start_browser();
				return 1;
			}
#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			{
				extern void GoToServicesMenu(void);
				GoToServicesMenu(); 
				return 1;
			}
#else
			goto_mobile_suite_screen();
			return 1;
#endif
#endif
			return 0;

		}
		break;
	case E_MESA_CAMERA://相机
		{
#if defined(__MMI_CAMERA__) || defined(__MMI_CAMCORDER__)
			if( MexSystem_IsSysRunning() )
			{
				MexSystem_Shutdown(E_MPS_HotShutdown);
			}
			
		#if (MEX_MTK_CODE_VERSION < 0x0952)
			{
			extern void mmi_camera_entry_app_screen(void);
			mmi_camera_entry_app_screen();
			}
		#elif (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
			#if defined(__MMI_CAMCORDER__)
			{
				extern mmi_id vapp_camco_launch_app(void);
				vapp_camco_launch_app();
			}
			#endif
		#else
			#if defined(__MMI_CAMCORDER__)
			mmi_camco_launch();
			#else
			mmi_camera_lauch();
			#endif
		#endif
		
			return 1;
#endif
			return 0;
		}
		break;
	case E_MESA_VIDEOREC://视频录像
		{
#ifdef __MMI_VIDEO_RECORDER__
			//extern void mmi_vdorec_entry_app(void);
			//mmi_vdorec_entry_app();
			return 1;
#else
			return 0;
#endif
		}
		break;
	case E_MESA_AUDIOPLAYER://音乐播放器
		{
#if defined(__COSMOS_MUSICPLY__)
			extern  mmi_id vapp_music_player_launch_app(void);
			vapp_music_player_launch_app();
#elif defined( __MMI_AUDIO_PLAYER__)
			extern void mmi_audply_entry_main(void);
			mmi_audply_entry_main();
			return 1;
#elif defined(__MMI_MEDIA_PLAYER__)
            extern void mmi_medply_app_pre_entry(void);
            mmi_medply_app_pre_entry(); 
			return 1;
#else
			return 0;
#endif
		}
		break;
	case E_MESA_VIDEOPLAYER://视频播放器
		{
#ifdef __MMI_VIDEO_PLAYER__
			extern void mmi_vdoply_entry_app(void);
			mmi_vdoply_entry_app();
			return 1;
#else
			return 0;
#endif
		}
		break;
	case E_MESA_PHOTO://相册
		{
#if defined(__MMI_IMAGE_VIEWER__) || defined(__MMI_FILE_MANAGER__) || defined(__MMI_FTO_GALLERY__)
#if (MEX_MTK_CODE_VERSION < 0x0952)
			extern void mmi_imgview_entry_app(void);
			mmi_imgview_entry_app();
			return 1;
#elif (MEX_MTK_CODE_VERSION >= 0x1112) &&(defined(__COSMOS_MMI_PACKAGE__))
			extern 	MMI_ID vapp_gallery_launch(void* param, U32 param_size);
			vapp_gallery_launch(NULL,NULL);
			return 1;
#endif

#else
			return 0;
#endif
		}
		break;
	case E_MESA_FM://FM
		{
#ifdef __MMI_FM_RADIO__
#if (MEX_MTK_CODE_VERSION < 0x1112)
			extern void mmi_fmrdo_entry_main(void);
			mmi_fmrdo_entry_main();
			return 1;
#else
#if (defined(__COSMOS_MMI_PACKAGE__))
			extern MMI_ID vapp_fm_radio_launch_app(void);
			vapp_fm_radio_launch_app();
			return 1;
#else
			return 0;
#endif /*__COSMOS_MMI_PACKAGE__*/
#endif /*MEX_MTK_CODE_VERSION*/
#endif /*__MMI_FM_RADIO__*/
		}
		break;
	case E_MESA_TV://TV
		{
#ifdef __MMI_TV__ 
			//....
			return 1;
#else
			return 0;
#endif
		}
		break;
	case E_MESA_CALCULATOR:
		{
#ifdef MEX_PLATFORM__FOR_MTK23C
			return 0;
#elif (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
#ifdef __MMI_CALCULATOR__
		extern void vapp_calculator_launch(void);
		vapp_calculator_launch();
		return 1;
#endif
#else
			return 0;
#endif
		}
		break;
	case E_MESA_ALRAM:
		{
#if (MEX_MTK_CODE_VERSION < 0x1112)
			extern void EntryAlmMenu(void);
			EntryAlmMenu();
#else
#if (defined(__COSMOS_MMI_PACKAGE__))
		extern mmi_id vapp_alarm_launch(void* param, U32 param_size);
		vapp_alarm_launch(NULL,NULL);
#endif		
#endif
			return 1;
		}
		break;
	case E_MESA_STOPWATCH:
		{
#ifdef __MMI_STOPWATCH__

#if (MEX_MTK_CODE_VERSION >= 0x1032)
			// TODO:
            mex_log(MEX_LOG_SYSTEM,"EntryApp type %d fail",app);
			return 0;
#else
			extern void EntryWtchScrMultipurposeStopwatch(void);
			EntryWtchScrMultipurposeStopwatch();
			return 1;
#endif

#else
			return 0;
#endif
		}
		break;
	case E_MESA_CALENDAR:
		{
#if (defined(__COSMOS_MMI_PACKAGE__))
			extern mmi_id vapp_calendar_launch(void* args, U32 argSize);
			vapp_calendar_launch(NULL,NULL);
			return 1;
#elif (defined(__MMI_CALENDAR__))
			extern void ClndrPreEntryApp(void);
			ClndrPreEntryApp();
			return 1;
#else
			return 0;
#endif
		}
		break;
	case E_MESA_FILEMGR:
		{
#ifdef __MMI_FILE_MANAGER__
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
			{
				extern MMI_ID vapp_fmgr_launch(void);
				vapp_fmgr_launch();
				return 1;
			}
#elif (MEX_MTK_CODE_VERSION >= 0x0824)
			{
				extern void mmi_fmgr_launch(void);
				mmi_fmgr_launch();
				return 1;
			}
#else
			{
				extern void fmgr_launch(void);
				fmgr_launch();
				return 1;
			}
#endif
#else
			return 0;
#endif
		}
		break;
	case E_MESA_DIGITHANDLER:
		{
			sprintf(g_idle_context.DialPadCallBuffer, "");//清除上次已拨号码
#ifdef __FLIGHT_MODE_SUPPORT__
			if (mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
			{
			#if (MEX_MTK_CODE_VERSION >= 0x1112)				
				extern mmi_id vapp_dialer_launch_from_phb(void *args, U32 argSize);
				vapp_dialer_launch_from_phb(NULL,NULL);
			#elif (MEX_MTK_CODE_VERSION >= 0x1032)
				extern void mmi_dialer_launch(void);   // 第一次按键只能激活dialer 界面 不算输入
				mmi_dialer_launch(); 
			#elif ((MEX_MTK_CODE_VERSION >= 0x0932)&&(MEX_MTK_CODE_VERSION <= 0x0952))
			    if((mex_strncmp((const char*)pParam, "&",1) == 0)||(pParam == NULL))//触屏进入时
			    {
			       extern void FlightModeDigitHandler(void);
                   FlightModeDigitHandler();
				}
				else
				{
			       extern  void HandleIdleScreenDigitEntry(void);
				   HandleIdleScreenDigitEntry();
				}	
			#else
				FlightModeDigitHandler();
			#endif
			}
			else
#endif /* __FLIGHT_MODE_SUPPORT__ */ 
			{
				/* end, diamond */
			#if (MEX_MTK_CODE_VERSION >= 0x1112)
#if defined(__MEX_G7_UI_BOOTUP__)
			{
					extern mmi_id vapp_dialer_launch_from_phb(void *args, U32 argSize);
					vapp_dialer_launch_from_phb(NULL,NULL);
#if 0					
					extern Mex_System g_MexSystem;
					//extern Mex_System g_MexSystem;
					bool bIsDialKey = FALSE;
					//通过传进的参数判断调用dialer的方法
					if(NULL != pParam)
					{
						if(mex_strncmp((const char*)pParam, "*",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "#",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "0",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "1",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "2",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "3",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "4",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "5",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "6",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "7",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "8",1) == 0)
							bIsDialKey = TRUE;	
						else if(mex_strncmp((const char*)pParam, "9",1) == 0)
							bIsDialKey = TRUE;			
						else//other key
							bIsDialKey = FALSE;
						//清空键值
						pParam = NULL;
						//情况本次拨号的参数
						mex_memset( g_MexSystem.m_pLaunchSysAppParam, 0, sizeof(g_MexSystem.m_pLaunchSysAppParam) );
					}

					if (bIsDialKey)
					{
						//TODO: needtodo: idle entry dialer by key
						//extern mmi_idle_entry_dialer_by_key(void);
						//mmi_idle_entry_dialer_by_key();	
					} 
					else
					{
						//extern void mmi_dialer_launch(void);	// 第一次按键只能激活dialer界面 不算输入
						//mmi_dialer_launch();
						
					}
#endif
				}

#endif			
			#elif (MEX_MTK_CODE_VERSION >= 0x1032)
#if defined(__MEX_G7_UI_BOOTUP__)
				{
					//通过传进的参数判断调用dialer的方法

					if((mex_strncmp((const char*)pParam, "&",1) == 0)||(pParam == NULL))
					{
						extern void mmi_dialer_launch(void);	// 第一次按键只能激活dialer界面 不算输入
						mmi_dialer_launch();						
					} 
					else
					{
						extern mmi_idle_entry_dialer_by_key(void);
						mmi_idle_entry_dialer_by_key();	
					}
				}
#else
				extern void mmi_dialer_launch(void);	// 第一次按键只能激活dialer界面 不算输入
				mmi_dialer_launch();
#endif
            #elif ((MEX_MTK_CODE_VERSION >= 0x0932)&&(MEX_MTK_CODE_VERSION <= 0x0952))
			    if((mex_strncmp((const char*)pParam, "&",1) == 0)||(pParam == NULL))//触屏进入时
			    {
                   IdleScreenDigitHandler();
				}
				else
				{
			       extern  void HandleIdleScreenDigitEntry(void);
				   HandleIdleScreenDigitEntry();
				}
			#else
				IdleScreenDigitHandler();				
			#endif
			}
			return 1;
		}
		break;
	case E_MESA_MAINMENU:
		{
			#if (defined(MT6235) || defined(MT6235B)) && ((MEX_MTK_CODE_VERSION >= 0x0932)&&(MEX_MTK_CODE_VERSION <= 0x0952)) && defined(__MEX_G7_UI_BOOTUP__)
			{
              #include "mtpnp_ad_master_common_def.h"
  			  extern E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card1Status(void);
  			  extern E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card2Status(void);
			  extern void EntryMainMenuFromIdleScreen(void);
			  extern void EntryScrSimOptionMenu(void);
  			  if( (MTPNP_AD_Get_Card1Status() != MTPNP_AD_SIMCARD_VALID) && (MTPNP_AD_Get_Card2Status() != MTPNP_AD_SIMCARD_VALID))
  			     EntryScrSimOptionMenu();
			  else
			  	 EntryMainMenuFromIdleScreen();
			  
			}
			#elif (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
#if defined(__MEX_G7_UI_BOOTUP__)
				extern MMI_ID vapp_cosmos_mainmenu_pre_launch(void);
				vapp_cosmos_mainmenu_pre_launch();
#else
				extern MMI_ID vapp_cosmos_mainmenu_launch(void* param, U32 param_size);
				vapp_cosmos_mainmenu_launch(NULL,NULL);
#endif				
			#else
				extern void EntryMainMenuFromIdleScreen(void);
				EntryMainMenuFromIdleScreen();
			#endif
			return 1;
		}
		break;
	case E_MESA_WAP_TO_URL:
		{
			extern void MexInterface_ConnectWap(char *url);
			MexInterface_ConnectWap((char*)pParam);
			return 1;
		}
		break;
	case E_MESA_MEX_PLATFORM:
		{
			extern void Mex_Entry_MainScreen(void);
			Mex_Entry_MainScreen();
			return 1;
		}
		break;
	case E_MESA_MEX_SPECIAL_ENTRY:
		{
#ifdef __MEX_QQ_SUPPORT__

			//如果不是独立QQ版本，请修改以下代码
			extern void Mex_Entry_QQ(void);
			Mex_Entry_QQ();
			//如果不是独立QQ版本，请修改以上代码
			return 1;
#endif

		}
		break;
	case E_MESA_IDLE_SCREEN:
		{
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&(defined(__COSMOS_MMI_PACKAGE__))
			extern void mmi_idle_launch(mmi_id base_group_id);
			mmi_idle_launch(NULL);
#else			
			extern void EntryIdleScreen(void);
			EntryIdleScreen();
#endif			
			return 1;
		}
		break;

	case E_MESA_GO_BACK_HISTORY:
		{
			extern void GoBackHistory(void);
			GoBackHistory();
			return 1;
		}
		break;
#if MEX_SDK_VER >= 1010
	case E_MESA_CALLHISTORY:		//通话记录
		{
#if defined(__SLT_UU_MENU__)
			{
				extern void slt_mmi_before_entry_calllogs(void);
				slt_mmi_before_entry_calllogs();
			}
#elif (MEX_MTK_CODE_VERSION >= 0x1112)
			{
#if defined(__MEX_G7_UI_BOOTUP__)
				//TODO:  needtodo: calllog missed launch
				vapp_clog_mclt_launch();
#else
				//TODO:  needtodo: calllog launch	
				vapp_clog_aclt_launch();
#endif
			}

#elif (MEX_MTK_CODE_VERSION >= 0x1032)
			{
#if defined(__MEX_G7_UI_BOOTUP__)
				extern void mmi_clog_launch_missed_calllog(void);
				mmi_clog_launch_missed_calllog();
#else
				extern void mmi_clog_launch(void);
				mmi_clog_launch();
#endif
			}
#elif (MEX_MTK_CODE_VERSION >= 0x0932) && (MEX_MTK_CODE_VERSION <= 0x0952)
            {
                extern void EntryDMCHISTMainMenu(void);
				EntryDMCHISTMainMenu();
            }
#elif (MEX_MTK_CODE_VERSION >= 0x0824) && (MEX_MTK_CODE_VERSION <= 0x08B0)
			{
				extern void EntryCHISTMainMenu(void);
				EntryCHISTMainMenu();
			}
#else
			{
				extern void mmi_chist_entry_general_main_menu();
				mmi_chist_entry_general_main_menu();
			}
#endif			
		}
		break;
#endif

#if MEX_SDK_VER >= 1014
	case E_MESA_BT:
		{
#ifdef __MMI_BT_SUPPORT__
			extern void mmi_bt_entry_main_menu(void);
			mmi_bt_entry_main_menu();
			return 1;
#endif
		}
		break;
	case E_MESA_BT_ENTRY_POWER_SWITCH:  // 蓝牙状态切换
		{
#ifdef __MMI_BT_SUPPORT__
#if (MEX_MTK_CODE_VERSION < 0x1112)
			extern void mmi_bt_entry_power_switch(void);
			mmi_bt_entry_power_switch();
			return 1;
#else
      // TODO:
            mex_log(MEX_LOG_SYSTEM,"EntryApp type %d fail",app);
      return NULL;
#endif        
#endif
		}
		break;
	case E_MESA_BT_ENTRY_CLOSE_ALL_CONNS:
		{
#ifdef __MMI_BT_SUPPORT__
#if (MEX_MTK_CODE_VERSION < 0x1112)
			extern void mmi_bt_entry_release_all_connections(void);
			mmi_bt_entry_release_all_connections();
#else
        // TODO:
            mex_log(MEX_LOG_SYSTEM,"EntryApp type %d fail",app);
#endif
			return 1;
#endif
		}
		break;
#endif
#if MEX_SDK_VER >= 1019
	case E_MESA_DATETIME_SET:
		{
			extern void EntryPhnsetSetDT(void);
			EntryPhnsetSetDT();
            return 1;
		}
		break;
#endif
#if (MEX_MTK_CODE_VERSION >= 0x1112)&&defined(__COSMOS_MMI_PACKAGE__)
case E_MESA_SETTINGS:				//设置
	{
		extern MMI_ID vapp_setting_launch(void *param, U32 param_size);
		vapp_setting_launch(NULL,NULL);
		return 1;
	}
	break;
#endif	
#if (MEX_MTK_CODE_VERSION >= 0x1032) &&(!defined(__COSMOS_MMI_PACKAGE__))
	case E_MESA_FUNANDGAMES:			//游戏和娱乐
		{
			extern void mmi_fng_entry_screen(void);
			mmi_fng_entry_screen();
            return 1;
		}
		break;
	case E_MESA_MULTIMEDIA:				//多媒体
		{
			extern void EntryMainMultimedia(void);
			EntryMainMultimedia();
            return 1;	
		}
		break;	
	case E_MESA_ORGANIZER:				//工具箱
		{
			extern void EntryOrganizer(void);
			EntryOrganizer();
       		return 1;	
		}
		break;
	case E_MESA_SETTINGS:				//设置
		{
			extern void EntryScrSettingMenu(void);
			EntryScrSettingMenu();
       		return 1;	
		}
		break;
	case E_MESA_PROFILE:				//情景模式
		{
			extern void mmi_profiles_entry_main_screen(void);
			mmi_profiles_entry_main_screen();
       		return 1;	
		}
		break;
	case E_MESA_EXTRA:				//附加功能
		{
			extern void EntryMainExtra(void);
			EntryMainExtra();
       		return 1;	
		}
		break;
#endif		
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	case E_MESA_WLAN:				//无线局域网
		{
			#ifdef __MMI_WLAN_FEATURES__
				#if defined(__COSMOS_MMI_PACKAGE__)
					extern MMI_ID vapp_dtcnt_wlan_launch(void* param, U32 param_size);
					vapp_dtcnt_wlan_launch(NULL, NULL);
				#else
					extern void mmi_wlan_entry_wlan_wizard_new(void);
					mmi_wlan_entry_wlan_wizard_new();
				#endif
       		return 1;
			#endif
			
		}
		break;
#endif

	}
	return 0;
}
uint16 MexConfig_GetMMITimerId(void)
{
	return MEX_SYSTEM_TIMER;
}

uint16 MexConfig_GetMEDTimerId(void)
{
	return MEX_SYSTEM_TIMER_MED;
}

int16 MexConfig_GetAppMemId(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	return APPLIB_MEM_AP_ID_MEX;
#else
	return APPLIB_MEM_AP_ID_MEX;
#endif
}

void MexConfig_InternalStopCallback(void)
{
	#ifndef WAP_SUPPORT
	extern void MexSystem_Shutdown( E_Mex_PlatformShutdown eShutdownMode );
    #endif
	MexSystem_Shutdown( E_MPS_ColdShutdown );

	applib_mem_ap_notify_stop_finished(APPLIB_MEM_AP_ID_MEX, KAL_TRUE);
}

void* MexConfig_GetInternalMemPtr(void)
{
	void *pRet = NULL;
	if (MEX_MEM_INTERNAL_SIZE_APP > 0)
	{
		applib_mem_ap_register(APPLIB_MEM_AP_ID_MEX, E_MSI_MENU_STRING, E_MII_MAIN_ICON, MexConfig_InternalStopCallback);
		pRet = applib_mem_ap_alloc(APPLIB_MEM_AP_ID_MEX, MEX_MEM_INTERNAL_SIZE_APP);
	}
	return pRet;
}

void MexConfig_FreeInternalMemPtr( void **pptr )
{
	applib_mem_ap_free( *pptr );
	*pptr = NULL;
}

char* MexConfig_GetMexPatchVer(void)
{
	return MEX_PLATFORM_PATCH_VERSION;
}

int32 MexConfig_GetAppMemSize(void)
{
	return MEX_MEM_INTERNAL_SIZE_APP;
}
#if (MEX_ENABLE_SMALLOC == 3 || MEX_ENABLE_SMALLOC == 5)
// backup dcm area
static bool MexConfig_Dcm_Backup(void *pDcmAddr, uint32 iDcmLen)
{
#if (MEX_ENABLE_BACKUP_DCM == 0)
	mex_log(MEX_LOG_SYSTEM, "E: no dcm bk");
	return false;
#elif (MEX_ENABLE_BACKUP_DCM == 1)
	{
		FS_HANDLE hBackupDcm = 0;
		uint32 iWritten = 0;
		int32 iTime = mex_gettime();
		hBackupDcm = FS_Open( MEX_BACKUP_DCM_FILE, FS_CREATE_ALWAYS | FS_READ_WRITE );
		if (hBackupDcm >= 0)
		{
			FS_Write( hBackupDcm, pDcmAddr, iDcmLen, &iWritten );
			FS_Close( hBackupDcm );
		}
		mex_log(MEX_LOG_SYSTEM, "I: dcm bk %d %d t:%dms", iDcmLen, iWritten, mex_gettime() - iTime );
		return (iDcmLen == iWritten);
	}
#elif (MEX_ENABLE_BACKUP_DCM == 2)
	mex_log(MEX_LOG_SYSTEM, "E: dcm bk 2");
	return false;
#else
	mex_log(MEX_LOG_SYSTEM, "E: unknown dcm bk");
	return false;
#endif
}

// restore dcm area
static bool MexConfig_Dcm_Restore(void *pDcmAddr, uint32 iDcmLen)
{
#if (MEX_ENABLE_BACKUP_DCM == 0)
	mex_log(MEX_LOG_SYSTEM, "E: no dcm rt");
	return false;
#elif (MEX_ENABLE_BACKUP_DCM == 1)
	{
		FS_HANDLE hBackupDcm = 0;
		uint32 iRead= 0;
		int32 iTime = mex_gettime();
		hBackupDcm = FS_Open( MEX_BACKUP_DCM_FILE, FS_READ_ONLY );
		if (hBackupDcm >= 0)
		{
			FS_Read( hBackupDcm, pDcmAddr, iDcmLen, &iRead );
			FS_Close( hBackupDcm );
		}
		mex_log(MEX_LOG_SYSTEM, "I: dcm rt %d %d t:%dms", iDcmLen, iRead, mex_gettime() - iTime );
		return (iDcmLen == iRead);
	}
#elif (MEX_ENABLE_BACKUP_DCM == 2)
	mex_log(MEX_LOG_SYSTEM, "E: dcm rt 2");
	return false;
#else
	mex_log(MEX_LOG_SYSTEM, "E: unkown dcm rt");
	return false;
#endif
}
#endif // (MEX_ENABLE_SMALLOC == 3)

#endif
bool MexConfig_Sram_Init(void)
{
/*=============== 0 ===============*/
#if (MEX_ENABLE_SMALLOC == 0)
	return false;

/*=============== 1,2 ===============*/
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
	return true;

/*=============== 3 ===============*/
#elif (MEX_ENABLE_SMALLOC == 3)
	if (g_mex_sram.m_iPoolId == NULL)
	{
		g_mex_sram.m_pSramAddr = (void *) MexConfig_GetPhoneConfig(E_MPC_DCM_Address);
		g_mex_sram.m_iSramLen = (uint32) MexConfig_GetPhoneConfig(E_MPC_DCM_Length);
		MexConfig_Dcm_Backup(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);
		memset(g_mex_sram.m_pSramAddr, 0, g_mex_sram.m_iSramLen);
		g_mex_sram.m_iPoolId = kal_adm_create(
			g_mex_sram.m_pSramAddr,
			g_mex_sram.m_iSramLen,
			NULL,
			KAL_FALSE);
		if (g_mex_sram.m_iPoolId == NULL)
		{
			MexConfig_Dcm_Restore(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);
			return KAL_FALSE;
		}
		g_mex_sram.m_iAllocCount = 0;
		g_mex_sram.m_iLeftSize = kal_adm_get_total_left_size( (KAL_ADM_ID) g_mex_sram.m_iPoolId );
	}
	mex_log(MEX_LOG_SYSTEM, "I: SRI ok");
	return true;

/*=============== 4 ===============*/
#elif (MEX_ENABLE_SMALLOC == 4)
	extern bool MexConfig_CreateSramPool();
	return MexConfig_CreateSramPool();
	
/*=============== 5 ===============*/
#elif(MEX_ENABLE_SMALLOC == 5)
	g_mex_sram.m_pSramAddr = (void *) MexConfig_GetPhoneConfig(E_MPC_DCM_Address);
	g_mex_sram.m_iSramLen = (uint32) MexConfig_GetPhoneConfig(E_MPC_DCM_Length);
	MexConfig_Dcm_Backup(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);
	g_mex_sram.m_iLeftSize=MEX_MM_SIZE_2+MEX_MM_SIZE_1+MEX_MM_SIZE_0*7;
	memset(g_mex_sram.m_pSramAddr, 0, g_mex_sram.m_iSramLen);
	kal_prompt_trace(MOD_MED,"MexConfig_Sram_Init 5");
	return KAL_TRUE;

/*===============  ===============*/
#else
    mex_log(MEX_LOG_SYSTEM,"unknown smalloc init type");
	return false;
#endif
}

void MexConfig_Sram_Destory(void)
{
/*=============== 0 ===============*/
#if (MEX_ENABLE_SMALLOC == 0)

/*=============== 1,2 ===============*/
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)

/*=============== 3 ===============*/
#elif (MEX_ENABLE_SMALLOC == 3)
	#if MEX_PLATFORM__FOR_F8
	if (g_mex_sram.m_iPoolId != NULL)
	{
        mex_log(MEX_LOG_SYSTEM,"Sram Destroy ID used");
	}
	if((g_mex_sram.m_iAllocCount == 0) && (g_mex_sram.m_iPoolId != NULL))
	{
		bool ret;
		kal_prompt_trace(MOD_MED,"MexConfig_Sram_Destory ");
		ret=kal_adm_delete(g_mex_sram.m_iPoolId);
		g_mex_sram.m_iPoolId=NULL;
        mex_log(MEX_LOG_SYSTEM,"Sram Destroy ret %d",ret);
	}		
	mex_log(MEX_LOG_SYSTEM, "I: SRD ok %d", g_mex_sram.m_iAllocCount);
	MexConfig_Dcm_Restore(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);
	
	#else //MEX_PLATFORM__FOR_F8
	if (g_mex_sram.m_iPoolId != NULL)
	{
		if(g_mex_sram.m_iAllocCount != 0)
    	{
            mex_log(MEX_LOG_SYSTEM,"Sram Destroy count > 0");
    	}
	
		kal_adm_delete( (KAL_ADM_ID) g_mex_sram.m_iPoolId );
		g_mex_sram.m_iPoolId = NULL;
		mex_log(MEX_LOG_SYSTEM, "I: SRD ok %d", g_mex_sram.m_iAllocCount);
		
		MexConfig_Dcm_Restore(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);
	}
	#endif //MEX_PLATFORM__FOR_F8

/*=============== 4 ===============*/
#elif (MEX_ENABLE_SMALLOC == 4)
	extern bool MexConfig_DeleteSramPool();
	MexConfig_DeleteSramPool();
	
/*=============== 5 ===============*/
#elif (MEX_ENABLE_SMALLOC == 5)
	mex_log(MEX_LOG_SYSTEM, "I: SRD ok %d", g_mex_sram.m_iAllocCount);
	g_mex_sram.m_iLeftSize=0;
	MexConfig_Dcm_Restore(g_mex_sram.m_pSramAddr, g_mex_sram.m_iSramLen);

/*===============  ===============*/
#else
    mex_log(MEX_LOG_SYSTEM,"unknown smalloc destroy type");
#endif
}

void* MexConfig_smalloc(size_t size)
{
/*=============== 0 ===============*/
#if (MEX_ENABLE_SMALLOC == 0)
	return NULL;

/*=============== 1 ===============*/
#elif (MEX_ENABLE_SMALLOC == 1)
	return (void *)med_int_smalloc( size );

/*=============== 2 ===============*/
#elif (MEX_ENABLE_SMALLOC == 2)
	return (void *)med_int_smalloc( size, __FILE__, __LINE__ );

/*=============== 3 ===============*/
#elif (MEX_ENABLE_SMALLOC == 3)
	char *ptr = NULL;
	if (size == 0)
		return NULL;
	
	#if MEX_PLATFORM__FOR_F8
	if(g_mex_sram.m_iAllocCount==0)
	{
		memset(g_mex_sram.m_pSramAddr, 0, g_mex_sram.m_iSramLen);
		g_mex_sram.m_iPoolId = kal_adm_create(
			g_mex_sram.m_pSramAddr,
			g_mex_sram.m_iSramLen,
			NULL,
			KAL_TRUE);
	}
	#endif //MEX_PLATFORM__FOR_F8
	
	if (g_mex_sram.m_iPoolId != NULL)
	{
		extern void *kal_adm_internal_alloc(KAL_ADM_ID adm_id, kal_uint32 size, char *filename, kal_uint32 line);
		ptr = kal_adm_internal_alloc( (KAL_ADM_ID)g_mex_sram.m_iPoolId, size, __FILE__, __LINE__);
		if (ptr != NULL)
		{
			g_mex_sram.m_iAllocCount++;
			g_mex_sram.m_iLeftSize = kal_adm_get_total_left_size( (KAL_ADM_ID) g_mex_sram.m_iPoolId );
		}
	}
	return ptr;
	
/*=============== 4 ===============*/
#elif (MEX_ENABLE_SMALLOC == 4)
	void *pRet = NULL;
	if (g_Mex_Sram_Mem_Pool && g_Mex_Sram_Mem_Pool->m_iMemId != 0)
	{
		tx_byte_allocate((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId, &pRet, size, TX_NO_WAIT);
		if(pRet != NULL)
		{
			g_Mex_Sram_Mem_Pool->m_iAllocCount++;
			g_Mex_Sram_Mem_Pool->m_iMemSize = ((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId)->tx_byte_pool_available;
			mex_log(MEX_LOG_CONFIG,"MexConfig_smalloc OK!");
		}
	}
	return pRet;
	
/*=============== 5 ===============*/
#elif(MEX_ENABLE_SMALLOC == 5)
	int8 i;
	if(size < MEX_MM_SIZE_0)
	{
		for(i = 0; i < 7; i++)
		{
			if(!mex_mm_seg_tbl[i].allocated)
			{
				mex_mm_seg_tbl[i].allocated=KAL_TRUE;
				//memset((void *)mex_mm_seg_tbl[i].start_address,0,mex_mm_seg_tbl[i].size_in_bytes);
				g_mex_sram.m_iLeftSize=g_mex_sram.m_iLeftSize-MEX_MM_SIZE_0;
		  		kal_prompt_trace(MOD_MED,"MexConfig_smalloc 5 i=%d size=%d m_iLeftSize=%d",i,size,g_mex_sram.m_iLeftSize);
				return (void *)mex_mm_seg_tbl[i].start_address;
			}
	 	}
	 	return NULL;
	 }
	else if(size < MEX_MM_SIZE_1)
	{
		if(!mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__13K_0].allocated)
		{
			mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__13K_0].allocated=KAL_TRUE;
			//memset((void *)mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__13K_0].start_address,0,mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__13K_0].size_in_bytes);
			g_mex_sram.m_iLeftSize=g_mex_sram.m_iLeftSize-MEX_MM_SIZE_1;
		  	kal_prompt_trace(MOD_MED,"MexConfig_smalloc 13k m_iLeftSize=%d",g_mex_sram.m_iLeftSize);
			return (void *)mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__13K_0].start_address;
		}
	 	return NULL;
	}
	else if(size < MEX_MM_SIZE_2)
	{
		if(!mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__33K_0].allocated)
		{
			mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__33K_0].allocated=KAL_TRUE;
			//memset((void *)mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__33K_0].start_address,0,mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__33K_0].size_in_bytes);
			g_mex_sram.m_iLeftSize=g_mex_sram.m_iLeftSize-MEX_MM_SIZE_2;
		  	kal_prompt_trace(MOD_MED,"MexConfig_smalloc 33k m_iLeftSize=%d",g_mex_sram.m_iLeftSize);
			return (void *)mex_mm_seg_tbl[MEX_MM_INTMEM_SEG__33K_0].start_address;
		}
	 	return NULL;
	}
	return NULL;

/*===============  ===============*/
#else
    mex_log(MEX_LOG_SYSTEM,"unknown smalloc malloc type");
	return NULL;
#endif
}

void MexConfig_sfree(void *ptr)
{
/*=============== 0 ===============*/
#if (MEX_ENABLE_SMALLOC == 0)
	return;
	
/*=============== 1,2 ===============*/
#elif (MEX_ENABLE_SMALLOC == 1) || (MEX_ENABLE_SMALLOC == 2)
	med_int_sfree( ptr );
	
/*=============== 3 ===============*/
#elif (MEX_ENABLE_SMALLOC == 3)
	if (ptr == NULL)
		return;
	
	if (g_mex_sram.m_iPoolId != NULL)
	{
		kal_adm_free( (KAL_ADM_ID) g_mex_sram.m_iPoolId, ptr);
		g_mex_sram.m_iAllocCount--;
		g_mex_sram.m_iLeftSize = kal_adm_get_total_left_size( (KAL_ADM_ID) g_mex_sram.m_iPoolId );
		
	#if MEX_PLATFORM__FOR_F8
		if(g_mex_sram.m_iAllocCount==0)
		{
			bool ret;
			ret=kal_adm_delete(g_mex_sram.m_iPoolId);
			g_mex_sram.m_iPoolId=NULL;
  			mex_log(MEX_LOG_SYSTEM,"sfree %x ret %d",ptr,ret);
		}
	#endif
	
		return;
	}
	mex_log(MEX_LOG_SYSTEM,"sfree ptr %x pool id NULL",ptr);
	
/*=============== 4 ===============*/
#elif (MEX_ENABLE_SMALLOC == 4)
	if(NULL == ptr)
		return;
	if(TX_SUCCESS == tx_byte_release(ptr))
	{
		g_Mex_Sram_Mem_Pool->m_iAllocCount--;
		g_Mex_Sram_Mem_Pool->m_iMemSize = ((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId)->tx_byte_pool_available;
		mex_log(MEX_LOG_CONFIG,"MexConfig_sfree OK!");
	}
	
/*=============== 5 ===============*/
#elif (MEX_ENABLE_SMALLOC == 5)
	int8 i;
	for(i = 0; i < MEX_MM_INTMEM_SEG____COUNT; i++)
	{
		if(ptr == (void *)mex_mm_seg_tbl[i].start_address)
		{
			mex_mm_seg_tbl[i].allocated=KAL_FALSE;
			g_mex_sram.m_iLeftSize=g_mex_sram.m_iLeftSize+mex_mm_seg_tbl[i].size_in_bytes;
	  		kal_prompt_trace(MOD_MED,"MexConfig_sfree 5 i=%d ptr=%x m_iLeftSize=%d",i,ptr,g_mex_sram.m_iLeftSize);
			return;
		}
	}
	mex_log(MEX_LOG_SYSTEM,"sfree ptr %x not found",ptr);
		
/*===============  ===============*/
#else
    mex_log(MEX_LOG_SYSTEM,"unknown smalloc free type");
#endif

}
void* MexConfig_tmalloc(size_t size)
{
#if (MEX_ENABLE_TMALLOC == 0)
	return NULL;
#else
	extern kal_uint8 * alloc_rtp_codec_buffer(kal_uint32 size);
	return (void*)alloc_rtp_codec_buffer(size);
#endif
}

void MexConfig_tfree(void *ptr)
{
#if (MEX_ENABLE_TMALLOC == 0)
	return;
#else
	extern void free_rtp_codec_buffer(kal_uint8 **ppBuf);
	free_rtp_codec_buffer( (uint8**)&ptr );
#endif
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexConfig_AllocKermem(void)
{
#if MEX_ALLOC_KERNAL_MEM_FOLLOW_MMISTARTUP
	{
		extern void MexSystem_InitKernalMem(void);
		MexSystem_InitKernalMem();
	}
#endif
}

void MexConfig_FreeKermem(void)
{
#if MEX_ENABLE_FREE_KERNAL_MEM
	{
		extern void MexSystem_ReleaseKernalMem(void);
		MexSystem_ReleaseKernalMem();
	}
#endif
}

bool MexConfig_GetSTON(void)
{
	return (MEX_ENABLE_STATISTIC == 1);
}

int32 MexConfig_GetSTTimerID(void)
{
	return MEX_ST_TIMER;
}

bool MexConfig_CameraDBuffer(void)
{
	return MEX_CAMERA_DBUFFER;
}

int8 MexConfig_GetDefaultVolume(void)
{
	return E_MSV_LEVEL_3;
}
#endif


bool MexConfig_NeedHideMexFiles(void)
{
	return MEX_HIDE_MEXFILES;
}


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
bool MexConfig_NeedPauseFM(void)
{
	return MEX_PAUSE_BGFM_ON_LAUNCH;
}


bool MexConfig_UnCompressUseSram(void)
{	
	return MEX_SUPPORT_MASSDATE_IN_INTRAM;
}
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
int8 MexConfig_EntrySystemApp(E_MEX_Entry_System_App app, void *pParam)
{
	switch(app)
	{
	case E_MESA_PHONEBOOK://进入电话薄
		{
			MMIAPIPB_OpenListAllGroupWin();//mainmenu_win.c
			return 1;
		}

	case E_MESA_SMS://进入短信菜单
		{
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
			MMI_OpenMessageMainMenuWin();
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
            MMIAPISMS_OpenMessageMainMenuWin();
#endif            
			return 1;
		}
	case E_MESA_NEWSMS: //进入编辑发送短信程序
		{		
			MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, PNULL, 0);//参考EnterWritingNewMessage
			return 1;
		}
	case E_MESA_SMSINBOX://收件箱
		{
			MMIAPISMS_EnterSmsBox( MMISMS_BOX_MT );
			return 1;
		}
	case E_MESA_WAP: //WAP 浏览器
		{
			
			MexSystem_Shutdown(E_MPS_ColdShutdown);
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
			MMICMSBRW_OpenMainMenuWin();
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
#ifdef BROWSER_SUPPORT_NETFRONT
            MMIWWW_OpenWWWBrowserWin(BROWSER_START_NORMAL,PNULL);
#else
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
#ifdef BROWSER_SUPPORT
    MMIBROWSER_OpenWebWin();
#endif
#else
	MMIBROWSER_EnterMainWin();
#endif
#endif
#endif            
			return 1;
		}
	case E_MESA_CAMERA://相机
		{
			MexSystem_Shutdown(E_MPS_ColdShutdown);
			MMIAPIDC_OpenPhotoWin();
			return 1;

		}
	case E_MESA_VIDEOREC://视频录像
		{
			MexSystem_Shutdown(E_MPS_ColdShutdown);
			MMIAPIDC_OpenVideoWin();
			return 1;

		}
	case E_MESA_AUDIOPLAYER://音乐播放器
		{
			MMIAPIMP3_OpenMp3Player();
			return 1;
		}
	case E_MESA_VIDEOPLAYER://视频播放器
		{
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
			MMIAPIMPEG4_MainEntry();
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MainEntry();
#endif
#endif
#endif
			return 1;
		}
	case E_MESA_PHOTO://相册
		{
			MMIAPIPICVIEW_OpenPicViewer();
			return 1;
		}
	case E_MESA_FM://FM
		{
			MMIAPIFM_OpenMainWin();
			return 1;
		}
	case E_MESA_TV://TV
		{
			//MMIAPITVOUT_OpenWin();//有的机子可能会不支持
			return 1;
		}
	case E_MESA_CALCULATOR:
		{
			MMIAPICALC_OpenMainWin();
			return 1;
		}
	case E_MESA_ALRAM:
		{
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	MMIAPIALM_OpenMainWin();
#else
	MMIALM_OpenMainWin();
#endif

			return 1;
		}
		//case E_MESA_STOPWATCH:  //展讯没有秒表
		//	{

		//	}
		//	break;
#ifdef CALENDAR_SUPPORT
	case E_MESA_CALENDAR:
		{
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
	MMIAPICALEND_OpenMainWin();
#else
	MMICALEND_OpenMainWin();
#endif
			return 1;
		}
#endif
	case E_MESA_FILEMGR:
		//if set privacy protect
		#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
		if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_MYDOC))
		{
			//validate privacy password
			MMIAPISET_ValidatePrivacyPwd( MMK_GetFocusWinId(), MMISET_PROTECT_MYDOC_TYPE );
		}
		else
		#endif
		{
			MMIAPIFMM_OpenExplorer();
		}
		return 1;
	case E_MESA_DIGITHANDLER:
		{
			MMI_MESSAGE_ID_E msg_id = MSG_KEYDOWN_0;
			BOOLEAN   bIsDialKey = FALSE;
#ifdef MEX_G7_UI_SUPPORT	
			if(NULL != pParam)
			{
				bIsDialKey = TRUE;//初始化
				if(mex_strncmp((const char*)pParam, "*",1) == 0)
					msg_id = MSG_KEYDOWN_STAR;	
				else if(mex_strncmp((const char*)pParam, "#",1) == 0)
					msg_id = MSG_KEYDOWN_HASH;	
				else if(mex_strncmp((const char*)pParam, "0",1) == 0)
					msg_id = MSG_KEYDOWN_0;
				else if(mex_strncmp((const char*)pParam, "1",1) == 0)
					msg_id = MSG_KEYDOWN_1;
				else if(mex_strncmp((const char*)pParam, "2",1) == 0)
					msg_id = MSG_KEYDOWN_2;
				else if(mex_strncmp((const char*)pParam, "3",1) == 0)
					msg_id = MSG_KEYDOWN_3;
				else if(mex_strncmp((const char*)pParam, "4",1) == 0)
					msg_id = MSG_KEYDOWN_4;
				else if(mex_strncmp((const char*)pParam, "5",1) == 0)
					msg_id = MSG_KEYDOWN_5;
				else if(mex_strncmp((const char*)pParam, "6",1) == 0)
					msg_id = MSG_KEYDOWN_6;
				else if(mex_strncmp((const char*)pParam, "7",1) == 0)
					msg_id = MSG_KEYDOWN_7;
				else if(mex_strncmp((const char*)pParam, "8",1) == 0)
					msg_id = MSG_KEYDOWN_8;
				else if(mex_strncmp((const char*)pParam, "9",1) == 0)
					msg_id = MSG_KEYDOWN_9;			
				else//other key
					bIsDialKey = FALSE;
			}		         
#endif
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
			MAIN_OpenFuncDialingWin(msg_id,bIsDialKey);
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
			MMIAPIIDLE_OpenDialWin(msg_id,bIsDialKey);
#else
            MMIIDLE_OpenDialWin(msg_id,bIsDialKey);
#endif
#endif            
			return 1;
		}
	case E_MESA_MAINMENU: //从Mex到MainMen时，平台已经退出了
		{
			if(MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))//MAINMENU  win已经打开了
			{
				//弹出消息[确认退出Mex平台，并迁移到主菜单]
				Mex_Exit_System(false);
				g_mex_bGotoMainMenu = true;
			}
			else
			{
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
				MMIMENU_CreatMainMenu();
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
				MMIAPIMENU_CreatMainMenu();
#endif
			}

			return 1;
		}
	case E_MESA_WAP_TO_URL:
		{
			if(pParam)
			{
				extern void MexInterface_ConnectWap(char *url);
				MexSystem_Shutdown(E_MPS_ColdShutdown);
        		MexInterface_ConnectWap((char*)pParam);
			}
			return 1;
		}
	case E_MESA_MEX_PLATFORM:
		{
			extern void MetMexOpenWin(void);
			MetMexOpenWin();
			return 1;
		}
	case E_MESA_MEX_SPECIAL_ENTRY:
		{
			////如果不是独立QQ版本，请修改以下代码
			//extern void Mex_Entry_QQ(void);
			//Mex_Entry_QQ();
			////如果不是独立QQ版本，请修改以上代码
			//return 1;
		}
		break;
	case E_MESA_IDLE_SCREEN:
		{
			//extern BOOLEAN g_mex_bIsFauiIdle;//是否将FAUI作为IDLE
			//mex_log(MEX_LOG_SYSTEM,"Laiger E_MESA_IDLE_SCREEN g_mex_bIsFauiIdle1=%d",g_mex_bIsFauiIdle);
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
		MMIAPIIDLE_OpenIdleWin();
#else
		MAIN_OpenIdleWin();
#endif
			MMK_HandleRedKeyProcess();
			
			if(g_mex_bGotoMainMenu)
			{
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
				MMIMENU_CreatMainMenu();
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
				MMIAPIMENU_CreatMainMenu();
#endif
			}
			return 1;
		}
	case E_MESA_GO_BACK_HISTORY:
		{
			MMK_CloseWin(MMK_GetFocusWinId());
			return 1;
		}
#if MEX_SDK_VER >= 1010
	case E_MESA_CALLHISTORY:
		{
			MMIAPICL_OpenMainMenuWindow();
			 return 1;
		}
#endif
#if MEX_SDK_VER >= 1019
    case E_MESA_DATETIME_SET:
    {
         MMK_CreateWin((uint32*)MMISET_SET_TIME_DATE_WIN_TAB, PNULL);
        return 1;
    }
#endif
    default:
        break;
	}
	return 0;
}


char* MexConfig_GetMexPatchVer(void)
{
	return MEX_PLATFORM_PATCH_VERSION;
}

int32 MexConfig_GetAppMemSize(void)
{
	return MEX_MEM_INTERNAL_SIZE_APP;
}
#if (MEX_ENABLE_SMALLOC == 4)
//创建快速内存池
bool MexConfig_CreateSramPool()
{
	uint32 iRes = TX_SUCCESS;
	uint32 uMemAddr = MEX_IRAM_BASE_ADDR;
	int32  iMemLen = MEX_IRAM_END_ADDR - MEX_IRAM_BASE_ADDR;
	if(iMemLen <= 0)
		return false;
	//以前已经创建了Sram,要先删除
	if(NULL != g_Mex_Sram_Mem_Pool)
	{
		extern bool MexConfig_DeleteSramPool();
		MexConfig_DeleteSramPool();
		//SCI_TraceLow:"MEX_CONFIG MexConfig_DeleteSramPool :delete sram before MexConfig_CreateSramPool"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXCONFIG_12814_112_2_18_2_36_34_7,(uint8*)"");
	}
	if(NULL == g_Mex_Sram_Mem_Pool)
	{
		g_Mex_Sram_Mem_Pool = (Mex_Sram_Mem_Pool*)SCI_ALLOC(sizeof(Mex_Sram_Mem_Pool));
		if(NULL == g_Mex_Sram_Mem_Pool)
			return false;
	}
	mex_memset(g_Mex_Sram_Mem_Pool,0,sizeof(Mex_Sram_Mem_Pool));

	g_Mex_Sram_Mem_Pool->m_iMemId = (int32)SCI_ALLOC(sizeof(TX_BYTE_POOL));
	if(0 == g_Mex_Sram_Mem_Pool->m_iMemId )
		return false;

	g_Mex_Sram_Mem_Pool->m_iMemSize =iMemLen;//先得到长度
#if defined(MEX_PLATFORM_SPREAD) 
	g_Mex_Sram_Mem_Pool->m_pMemAddr = uMemAddr;
	//锁住IRAM
	{
		IRAM_AREA myArea = {0};
		ILOCK_HANDLE lock = NULL;
		myArea.iram_addr = g_Mex_Sram_Mem_Pool->m_pMemAddr;
		myArea.iram_length = 10;
		myArea.data = SCI_NULL;
		myArea.attri = IRAM_ATTRIBUTE_ZI;

		//得到可以分配的起始地址
		while((myArea.iram_addr < MEX_IRAM_END_ADDR)
			&&(SCI_NULL == (lock = IRAM_LockArea(&myArea,1,IRAM_NO_WAIT))))//锁失败
		{
			myArea.iram_addr +=MEX_IRAM_LOCK_CHECK_BLOCK; //检测下一个地址
		}
		IRAM_UnlockArea(lock);
		mex_log(MEX_LOG_CONFIG,"Get Base Addr = %x",myArea.iram_addr);

		myArea.iram_length = MEX_IRAM_END_ADDR - myArea.iram_addr;//初始长度

		//得到可以分配的最大地址
		while((myArea.iram_length > 100)
			&&(SCI_NULL == ((g_Mex_Sram_Mem_Pool->lock) = IRAM_LockArea(&myArea,1,IRAM_NO_WAIT))))//锁失败
		{
			myArea.iram_length -=MEX_IRAM_LOCK_CHECK_BLOCK; //检测下一个
		}
		mex_log(MEX_LOG_CONFIG,"Get End Addr = %x",myArea.iram_addr+myArea.iram_length);

		g_Mex_Sram_Mem_Pool->m_pMemAddr = myArea.iram_addr;
		g_Mex_Sram_Mem_Pool->m_iMemSize = myArea.iram_length;

	}
#elif defined(MEX_PLATFORM_SIMULATOR)//模拟器自己申请内存
	{
		char *pSramMem = (char*)SCI_ALLOC(iMemLen);
		if(NULL == pSramMem)
		{
			SCI_FREE(g_Mex_Sram_Mem_Pool->m_iMemId);
			g_Mex_Sram_Mem_Pool->m_iMemId = 0;

			SCI_FREE(g_Mex_Sram_Mem_Pool);
			g_Mex_Sram_Mem_Pool = NULL;
			return false;
		}
		mex_memset(pSramMem,0,iMemLen);
		g_Mex_Sram_Mem_Pool->m_pMemAddr = (uint32)pSramMem; //模拟器地址
	}
#endif

	iRes = tx_byte_pool_create((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId, "Mex SRAM Memory", g_Mex_Sram_Mem_Pool->m_pMemAddr, g_Mex_Sram_Mem_Pool->m_iMemSize);
	if (iRes != TX_SUCCESS)
		return false;
	g_Mex_Sram_Mem_Pool->m_iAllocCount = 0;
	g_Mex_Sram_Mem_Pool->m_iMemSize = ((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId)->tx_byte_pool_available;
	mex_log(MEX_LOG_CONFIG,"MexConfig_CreateSramPool OK!");
	return true;
}
//删除快速内存池
bool MexConfig_DeleteSramPool()
{
	uint32 iRes = TX_SUCCESS;
	if(NULL == g_Mex_Sram_Mem_Pool)
		return false;
	if(!g_Mex_Sram_Mem_Pool->m_iMemId)
		return false;

	iRes =  tx_byte_pool_delete((TX_BYTE_POOL *)g_Mex_Sram_Mem_Pool->m_iMemId);
	if (iRes != TX_SUCCESS)
		return false;
#if defined(MEX_PLATFORM_SPREAD) //解锁
	if(SCI_NULL == IRAM_UnlockArea(g_Mex_Sram_Mem_Pool->lock))//解锁失败
	{
		mex_log(MEX_LOG_CONFIG,"UnLock SRAM AREA Failed!");
		return false;
	}
	g_Mex_Sram_Mem_Pool->lock = SCI_NULL;
	mex_log(MEX_LOG_CONFIG,"UnLock SRAM AREA OK!");
#elif defined(MEX_PLATFORM_SIMULATOR)
	//释放内存
	if(g_Mex_Sram_Mem_Pool->m_pMemAddr)
	{
		SCI_FREE(g_Mex_Sram_Mem_Pool->m_pMemAddr);
		g_Mex_Sram_Mem_Pool->m_pMemAddr = 0;
	}
	else
	{
		mex_log(MEX_LOG_CONFIG,"Simulator free SRAM AREA Failed!");
		return false;
	}
#endif
	if(g_Mex_Sram_Mem_Pool->m_iMemId)
	{
		SCI_FREE((void *)g_Mex_Sram_Mem_Pool->m_iMemId);
		g_Mex_Sram_Mem_Pool->m_iMemId = 0;
	}
	if(g_Mex_Sram_Mem_Pool)
	{
		SCI_FREE(g_Mex_Sram_Mem_Pool);
		g_Mex_Sram_Mem_Pool = NULL;
	}
	mex_log(MEX_LOG_CONFIG,"MexConfig_DeleteSramPool OK!");
	return true;
}

#endif
bool MexConfig_GetSTON(void)
{
	return (MEX_ENABLE_STATISTIC == 1);
}
int8 MexConfig_GetDefaultVolume(void)
{
	return E_MSV_LEVEL_3;
}

void MexConfig_ConvertBCD2Ansi(int8 *ansi_str,int32 max_len,int8 *bcd_str,int32 bcd_len, int32 offset)
{
	int32 index;
	int32 ansi_index;
	int32 bcd_index;
	int8  ch;

	ansi_index = 0;
	bcd_index  = 0;
	for(index = 0; index < bcd_len; index ++)
	{
		ch = bcd_str[index] & 0xF;   
		if(ch == 0xf)
		{
			break;
		}

		if(offset <= bcd_index)
		{
			ansi_str[ansi_index++] = ch + '0';          
			if(ansi_index >= max_len)
			{
				break;
			}          
		}
		bcd_index++;
		ch = (bcd_str[index] & 0xF0) >> 4;   

		if(ch == 0xf)
		{
			break;
		}

		if(offset <= bcd_index)
		{
			ansi_str[ansi_index++] = ch + '0';          
			if(ansi_index >= max_len)
			{
				break;
			}          
		}
		bcd_index++;
	}

}

void MexConfig_ConvertAnsi2BCD(int8 *bcd_str,int32 bcd_len,int8 *ansi_str,int32 ansi_len, int32 offset)
{
	int32 index;
	int32 ansi_index;
	int32 bcd_index;
	int8  ch;
	bool  bIsBCDLow;

	ansi_index = 0;
	bcd_index  = 0;
	bIsBCDLow  = true;
	for(index = 0; index < ansi_len; index ++)
	{
		if(offset > ansi_index)
		{
			ansi_index++;
			if(ansi_index >= ansi_len)
			{
				break;
			}      

			continue;
		}

		ch = ansi_str[ansi_index++] - '0';  
		if(bIsBCDLow)
		{
			bcd_str[bcd_index] = ch;
			bIsBCDLow = false;
		}
		else
		{
			bcd_str[bcd_index] |= (ch << 4);
			bcd_index++;
			bIsBCDLow = true;
		}
	}

	if(bIsBCDLow)
	{
		bcd_str[bcd_index] = 0x0F;
	}
	else
	{
		bcd_str[bcd_index] |= 0xF0;
		bIsBCDLow = true;
	}
}

#endif // MEX_PLATFORM_SPREAD




void MexConfig_SetTinyLoadingPage( bool bShowColorBar,  mex_color cBgColor, mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr )
{
#if	MEX_SHOW_LOADING_PAGE		
	g_mex_tiny_loading_page.m_bShowColorBar = bShowColorBar;
	g_mex_tiny_loading_page.m_cBgColor = cBgColor;
	g_mex_tiny_loading_page.m_cFontColor = cFontColor;
	g_mex_tiny_loading_page.m_uFontPos = uFontPos;
	g_mex_tiny_loading_page.m_szLoadStr = szLoadStr;
	g_bNeedShowLoadingPage = true;
#endif
}




void MexConfig_SetManLoadingPage( bool bShowColorBar, mex_color cBgColor, mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr )
{
#if	MEX_SHOW_LOADING_PAGE
	g_mex_man_loading_page.m_bShowColorBar = bShowColorBar;
	g_mex_man_loading_page.m_cBgColor = cBgColor;
	g_mex_man_loading_page.m_cFontColor = cFontColor;
	g_mex_man_loading_page.m_uFontPos = uFontPos;
	g_mex_man_loading_page.m_szLoadStr = szLoadStr;
	g_bNeedShowLoadingPage = true;
#endif

}

//MTK系统是否支持矢量字体
bool MexConfig_bSupportVectorFont(void)
{

#if defined(__MMI_VECTOR_FONT_SUPPORT__)	
	return true;
#endif
	return false;
}

//矢量字体每个文字多出来的宽度
//解决当个字符宽度叠加小于整个字符总宽度的情况
//问题现象guidemo绘制文字时，在边界处只显示一半而不是切换到下一行...
int32 MexConfig_VectorFont_CharAddPixels(void)
{
#if defined(__MMI_VECTOR_FONT_SUPPORT__)
		return 1;  //该数字应根据实际情况微调  单位像素数   
#else
		return 0;
#endif
}

#if (MEX_SDK_VER >= 1018)
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexConfig_MapSimCard()	
{

}

E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId)
{
    return simId;
}    
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
void MexConfig_MapSimCard()	
{
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)       
    g_mex_simcard_map[E_MEX_SIM_1] = E_MEX_SIM_1;
    g_mex_simcard_map[E_MEX_SIM_2] = E_MEX_SIM_2;
    g_mex_simcard_map[E_MEX_SIM_3] = E_MEX_SIM_3;  
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if((SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)) &&
	(SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3)))
#else
	if((SIM_STATUS_NO_SIM == MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)) &&
	(SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_3)))
#endif
    {
      g_mex_simcard_map[E_MEX_SIM_1] = E_MEX_SIM_3;      
    }
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)     
	else if((SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)) &&
	(SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2)) &&
	(SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3)))
#else
	else if((SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_1)) &&
	(SIM_STATUS_NO_SIM == MMIPHONE_GetSimStatus(MN_DUAL_SYS_2)) &&
	(SIM_STATUS_NO_SIM != MMIPHONE_GetSimStatus(MN_DUAL_SYS_3)))
#endif
    {
      g_mex_simcard_map[E_MEX_SIM_2] = E_MEX_SIM_3;
    }
#endif
}

E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId)
{
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)       
    return (E_MEX_Sim_Card_ID)g_mex_simcard_map[simId];
#else
    return simId;
#endif
}    


#endif
#endif

// 是否需要检测T卡?
// 参数1:
// 返回: bool TRUE:需要检测T卡。FALSE:不需要检测T卡。
bool MexConfig_NeedCheckTCard(void)
{
	char szAppName[200];
	int32 iNameLen;

#if MEX_PATH_ADAPTIVE
	return false;
#endif

	if (MexConfig_GetPhoneConfig(E_MPC_Tcard_State))
		return true;
	
	Mex_GetCurrentEntryAppInfo(szAppName, 200, NULL);
	iNameLen = strlen(szAppName);
	if (iNameLen > 4 && mex_stricmp(szAppName + iNameLen - 4, ".mex") == 0)
	//if (iNameLen > 4 && mex_stricmp(szAppName, "AdExplorer.mex") == 0)
	{
		int32 *hFile = NULL;
		wchar_t szPath[MEX_FILE_NAME_MAX_LENGTH];
		szAppName[iNameLen - 4] = '\0';
		mex_wcscpy(szPath, MEX_APPROOTPATH);
		mex_wcscat(szPath, L"\\");
		mex_wcscat(szPath, mex_tstr2wcs(szAppName));
		szAppName[iNameLen - 4] = '.';

		hFile = mex_fopenw_ext(mex_tstr2wcs(szAppName), L"rb", szPath, NULL, false);
		if (hFile != NULL)
		{
			mex_fclose(hFile);
			return false; // fopen XXXX.mex ok.
		}
	}

	return true; // default return true
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
int32 MexConfig_GetReservedTimer(void)
{
	return MEX_RESERVED_TIMER;
}

int32 MexConfig_MGDMemSize(void)
{
	return 200*1024;
}
#endif

#endif
