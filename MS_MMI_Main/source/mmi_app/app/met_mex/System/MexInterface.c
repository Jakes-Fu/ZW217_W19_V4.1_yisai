/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexInterface.c
* 文件标识：
* 内容摘要： // 实现平台部分外部接口，主要用于适配不同版本系统代码的差异
*
************************************************************************/
#include "mmi_app_met_mex_trc.h"
#if defined(MEX_TARGET_SERVER)

#include "MexLib.h"
#if defined(MEX_PLATFORM_MODIS)
#include "windows.h"
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

#include "MMI_include.h"
#include "MMI_features_camera.h"


#include "med_api.h"

#include "mdi_datatype.h"
#include "Mdi_audio.h"
#include "Device.h"
#include "SettingProfile.h"
#include "l1audio.h"
#include "aud_defs.h"
#include "aud_main.h"
#include "med_api.h"
#include "med_global.h"
#include "med_struct.h"
#include "med_main.h"
#include "med_utility.h"
#include "med_context.h"

#if defined(MT6236)
#include "cache_sw.h"
#include "mmu.h"
#endif

#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "QueueGprot.h"
#endif
#include "ProtocolEvents.h"
#include "mmi_msg_struct.h"
#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "FrameworkStruct.h"
#endif
#include "App_ltlcom.h"
#include "mdi_datatype.h"
#include "mdi_camera.h"
#include "CameraApp.h"
#include "msf_cfg.h"

#include "gpioInc.h"


#include "PhoneBookTypes.h"

#include "mtpnp_ad_master_header.h"
#if defined(MEX_PLATFORM__FOR_D450)
#include "slt_gemini.h"
#endif
#include "SmsGuiInterfaceType.h"
#include "soc_api.h"
#include "wap_ps_struct.h"
#include "wapadp.h"
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "SMSApi.h"           // for simulate new sms
#endif
#include "app_datetime.h"     // for datetime
#include "mmi_msg_context.h"  // for simulate new sms
#include "UCMResDef.h"
#include "UCMGProt.h"
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "UCMCallProt.h"
#endif
#include "UCMProt.h"
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "SMSStruct.h"
#endif

#if(MEX_MTK_CODE_VERSION >= 0x0952)
#include "ucmsrvgprot.h"
#include "Smsstoragecoresrv.h"
#include "smssrvgprot.h"
#endif

#include "med_smalloc.h"
#include "Med_utility.h"
#include "mnsms_struct.h"
#include "Med_Main.h"
#include "FileManagerGProt.h"    // for phnset wall paper
#include "mmi_phnset_dispchar.h" // for phnset wall paper
#include "PhoneSetup.h"          // for phnset wall paper

#include "SimDetectionDef.h"
#include "idleappdef.h"

#include "FileManagerDef.h"
#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "Conversions.h"
#endif

#if MEX_MTK_CODE_VERSION >= 0x0952
#include "wgui_status_icon_bar.h"
#else
#include "Wgui_status_icons.h"
#endif

#include "DataAccountDef.h"
#if defined(MEX_PLATFORM_MTK)
#include "Cache_hw.h"
#include "Cache_sw.h"
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "ProfilesAppGprot.h"
#include "DtcntSrvIprot.h"
#endif
#if (MEX_MTK_CODE_VERSION >= 0x1108)
#include "l4c_nw_cmd.h"
#include "browser_api.h"
#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "mmi_rp_app_vdoply_def.h"
#endif
#include "TimerEvents.h"
#endif
#if (MEX_MTK_CODE_VERSION >= 0x1112)
#include "gui_typedef.h"
#include "FileMgrType.h"
#include "AudioPlayerPlayList.h"
#include "smsal_l4c_enum.h"
#include "browser_api.h"
#include "FileMgrGProt.h"
#include "ProfilesAppGprot.h"
#endif
#if defined(MT6236) && (MEX_MTK_CODE_VERSION < 0x1112)
#include "image.h"
#endif 
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#include "MexLib.h"
#include "met_mex_osbridge.h"
#include "mmipb_nv.h"
#include "apm_express.h"
#ifdef MOTION_SENSOR_TYPE
#include "Mmk_gsensor.h"
#endif
#include "met_mex_id.h"

#if (MEX_SPRD_CODE_VERSION < 0x10A1109)
#define MEX_IMAGE_TYPE_BMP    1
#define MEX_IMAGE_TYPE_PNG    2
#define MEX_IMAGE_TYPE_JPG    3
#define MEX_IMAGE_TYPE_GIF    4
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109)
#include "img_dec_interface.h"
#endif

#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
#include "IN_Feature.h"	
#include "mmiwap.h"
#endif
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "sfs.h"
#include "dal_time.h"
#include "guiblock.h"
#include "mn_api.h"

#include "lcd_cfg.h"
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
#include "Ui_layer.h"
#include "mmisms_export.h"  
#include "mmicc_export.h "
#include "Mmiset_export.h"
#include "Mmisms_order.h"
#include "mmisms_app.h"
#else
#include "MmiCC.h"  
#include "Mmiset.h"

#endif
#include "mmisms_app.h"
#include "app_tcp_if.h"
#include "mmk_app.h"

#include "mmk_timer.h"
#include "Dal_Dcamera.h"             
#include "Mmi_Signal_Ext.h"
#include "Sci_Api.h"
#include "sig_code.h"
#include "mmifmm_interface.h"
#include "mmibt_text.h"
#include "mmipub.h"
#include "mmimp3_id.h"

#include "mmifm_id.h"
#include "mmipb_search.h"
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
#ifdef BROWSER_SUPPORT
#include "mmibrowser_setting.h"
#include "mmibrowser_func.h"
#endif
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
#include "mmiset_call_export.h"

#else
#include "mmiset_call.h"
#endif
#else
#include "mmiset_calll.h"
#endif
#include "aud_gen.h"
#ifdef BROWSER_SUPPORT_NETFRONT
#include "Mmiwww_internal.h"
#endif

#if (MEX_SPRD_CODE_VERSION >= 0x09A1109)
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
#include "mmipdp_export.h"
#else
#include "mmiaudio_ctrl.h"
#include "mmipdp_api.h"
#endif
#endif

#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
extern MMISMS_GLOBAL_T g_mmisms_global;      // SMS的全局变量
#endif

#include "app_tcp_if.h"
#define module_type  int32

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


#define MEX_DRAW_STRING_CLIPPER_SUPPORT 1    //是否支持文字显示裁剪

#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
#define VB_FIFO_SIZE		(160)
#endif

#endif

#if MEX_ROMFILE_INTERFACE
//内置文件
#ifdef MEX_G7_UI_SUPPORT
#include "MexSysFinalRom_UI.hi"
#else
#if defined(MET_MEX_TOMCAT_SUPPORT) && defined(MET_MEX_CQZ_SUPPORT)
#if defined(MAINLCD_SIZE_240X320)
#include "MexSysFinalRom240x320.hi"
#elif defined(MAINLCD_SIZE_240X400)
#include "MexSysFinalRom240X400.hi"
#elif defined(MAINLCD_SIZE_320X480)
#include "MexSysFinalRom320X480.hi"
#else
#include "MexSysFinalRom.hi"
#endif
#else
#include "MexSysFinalRom.hi"
#endif
#endif
#endif

#include "MexLib_Def.h"
#include "MexLib_Std.h"
#include "MexLib_Os.h"
#include "MexLib_Phs.h"
#include "MexLib_Soc.h"
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
#include "MexLib_Mdi.h"
#endif
#include "MexInterface.h"
#include "met_mex_aq.h"
#if (0 == MEX_BUILD_FINAL_USER)
#include "MexSystem.h"
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "mmi_rp_app_metmex_def.h"
#include "mex_app_id.h"
#else
#include "Mex_Res.h"
#endif
#endif


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#include "met_mex_text.h"
#include "mmisms_read.h"
#include "mmisms_edit.h"
#include "mmisms_send.h"
#include "mmisms_save.h"
#include "mmisms_delete.h"
#include "mmidc_flow.h"
#include "met_mex_export.h"

#if (MEX_SPRD_CODE_VERSION == 0x09A1025)
#include "guistring.h"
#include "caf.h"
#endif
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
#include "Mmisrvaud_api.h"
#endif
#include "adm.h"
#endif

#if defined(MEX_PLATFORM_SIMULATOR)
#include "met_socket_sim.h"
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

//保存GPRS拨号所需的相关信息
typedef struct _Mex_SetGprsReq_Context  
{
    uint32 uConnect_time;                           //connect time
	//	E_Mex_Net_Mode mode;                                                //network type
	uint32 dual_sys;                                //sim id
    uint32 ps_service_type;                         //
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)	
	uint8 APNcn[16];                                //access point name
#else	
    uint32	   storage;                             //
	MMIPDP_ACTIVE_INFO_T   app_info;                //
#endif
	
} Mex_SetGprsReq_Context;

#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
uint8 *g_mex_old_layer_rawbuf = NULL, *g_mex_old_layer_buf = NULL, *g_mex_select_buf = NULL;
UILAYER_APPEND_BLT_T g_mex_select_layer = {{GUI_MAIN_LCD_ID,UILAYER_NULL_HANDLE},UILAYER_LEVEL_LOW};
UILAYER_INFO_T g_sprd_baselayer_info;
UILAYER_INFO_T g_mex_dblayer_info;
UILAYER_APPEND_BLT_T g_mex_2append_layer = {{GUI_MAIN_LCD_ID,UILAYER_NULL_HANDLE},UILAYER_LEVEL_LOW};
#endif

#endif
int32 MexConfig_ConvertSprdDef(E_SYS2MEX_Def_Convert param);
void MetMex_StartTimer(uint8 timerid, uint32 time_out, MexFuncPtr funcPtr,bool is_period);
uint8 MetMex_GetNetReconnectTimerId(void);
void *MexConfig_ConvertStruct(E_MTK2MEX_Struct_Convert type,void **dst,void *src);
uint8 MetMex_GetVibratorTimerId(void);


//定义展讯网络错误返回值
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if 0 // 已包含DAPS\export\inc\socket_types.h 
#define     ENOBUFS        1
#define     ETIMEDOUT      2
#define     EISCONN        3
#define     EOPNOTSUPP     4
#define     ECONNABORTED   5
#define     EWOULDBLOCK    6
#define     ECONNREFUSED   7
#define     ECONNRESET     8
#define     ENOTCONN       9
#define     EALREADY       10
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
#define     ELOWER         23    /* lower layer (IP) error */
#define     ENOTSOCK	   24    /* Includes sockets which closed while blocked */
#define     EIEIO		   27 /* bad input/output on Old Macdonald's farm :-) */
#endif
#if (defined(MEX_PLATFORM_SIMULATOR))

#define MEX_SOCK_CONNECT(socket,addr,len) \
met_sock_connect(socket,addr,len)

#define MEX_SOCK_RECV(socket,buf,len,flag)\
met_sock_recv(socket,buf,len,flag)
#define MEX_SOCK_SEND(socket,buf,len,flag)\
met_sock_send(socket,buf,len,flag)
#define MEX_SOCK_ERROR(socket)\
met_sock_error(socket)

#define MEX_PARSE_HOST(domain, addr, flag)\
met_parse_host(domain, addr, flag)

#elif (defined(MEX_PLATFORM_SPREAD))

#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
#define MEX_SOCK_SOCKET(family, type, proto)\
sci_sock_socket(family,type,proto)
#else
#define MEX_SOCK_SOCKET(family, type, proto)\
sci_sock_socket(family,type,proto,TCPIP_DEFAULT_NETID_FOR_GPRS)
#endif

#define MEX_SOCK_CONNECT(socket,addr,len)\
sci_sock_connect(socket,addr,len)
#define MEX_SOCK_RECV(socket,buf,len,flag)\
sci_sock_recv(socket,buf,len,flag)
#define MEX_SOCK_SEND(socket,buf,len,flag)\
sci_sock_send(socket,buf,len,flag)
#define MEX_SOCK_ERROR(socket)\
sci_sock_errno(socket)
#define MEX_PARSE_HOST(domain, addr, flag)\
sci_parse_host(domain, addr, flag)

#else
#error The Platform MUST be PLATFORM__SIMULATOR/PLATFORM__FOR_SPRD6600L/PLATFORM__FOR_SPRD6800H.
#endif

#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MAX_PHONEBOOK_SIM_TOTAL 500
uint16 g_phb_store_index_array[MMINV_MAX_PHONEBOOK_RECORDS * 2 + MAX_PHONEBOOK_SIM_TOTAL] = {0};
uint8 g_phb_mask_array[(MMINV_MAX_PHONEBOOK_RECORDS + 7) / 8] = {0};
MN_DUAL_SYS_E g_met_mex_phone_sim = -1;			/*系统选用的sim卡*/
extern bool   g_met_mex_is_active_gprs;			/*体统是否正在开始gprs*/
int 		  g_met_mex_net_mode  = -1;			/*系统选中的网络模式*/
/*如果网络模式为cmwap 则该值为0*/
/*如果网络模式为cmnet  则该值为1*/
/*如果为其他值说明错误*/
#define       MET_GPRS_DUMMY_TASK_ID            (0x7FFF000) /*定义杨讯网络消息处理模拟任务ID*/

Mex_SetGprsReq_Context g_met_mex_GprsReq_Context = {0};

#endif


extern funcptr *MEX_C_LIB_FUNC_NAME_STD;

#define CUSTOM_FOR_DEWAV_GET_SMSCENT	(0)


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
extern msg_context_struct g_msg_cntx;
extern void PhnsetSetWallpaperByFileName(S8 *path);
extern void PhnsetSetScrSvrByFileName(S8 *path);
extern void PhnsetSetPwrOnOffDisplayByFileName(S8 *path);
int g_Mex_DummyInCall = 0; // for dummy incoming call
int g_Mex_WallPaperIsSetting=0; //0:Idle	1:Setting
#endif


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
uint8 g_mex_soc_gprs_timer_id = 0;
#endif
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define OVERRIDE(func) $Sub$$##func
#define REAL(func) $Super$$##func

//参考 resizer.h
#ifdef MEX_PLATFORM_FOR_MTK6253
#define RESZ_INT_STATUS             0x8401000c
#define RESZ_RGB_BUF1               0x84010084
#define RESZ_RGB_BUF2               0x84010088

//RESZ_INT_STATUS
#define RESZ_INT_LCD_BIT            0x0001
#define RESZ_INT_MEMO1_BIT          0x0002
#define RESZ_INT_FSTART_BIT         0x0004
#define RESZ_INT_PXDINT_BIT         0x0008
#define RESZ_INT_MEMO2_BIT          0x0010

#elif defined(MEX_PLATFORM__FOR_D600)
#define RESZ_RGB_BUF1				0x80610084
#define RESZ_RGB_BUF2				0x80610088

#elif defined(MEX_PLATFORM_FOR_MTK6235)
#define RESZ_RGB_BUF1				0x840E0084
#define RESZ_RGB_BUF2				0x840E0088

#else
#define RESZ_RGB_BUF1				0x80610084
#define RESZ_RGB_BUF2				0x80610088
#endif
#define DRV_Reg32(addr)               (*(volatile kal_uint32 *)(addr))


//intctrl_6253.h
#define IRQ_RESZ_CODE               (21) /*spec : RESIZER*/

extern void IRQUnmask(kal_uint8 code);

//static bool bCapReq = false;
//static kal_uint32 buf_addr;

extern void *MexCamera_GetCameraWriteBuffer(void);
extern void *mex_Phs_GetCameraOutPutData(void);
extern void MexPhs_GetPreviewWH(int32 *pWidth, int32 *pHeight);
extern bool Mex_Phs_Is_CameraCapReq(void);
extern void Mex_Phs_Rest_CameraCapReq(void);
extern int32 MexSystem_NotifyLcdRotate(E_Mex_LcdRotateType eLcdRotateType, int32 iNewWidth, int32 iNewHeight );

MexQQStatus_e g_MexMasterQQStatus = MEX_QQ_STATUS_NONE, g_MexSlaveQQStatus = MEX_QQ_STATUS_NONE;
int g_MexCheckIfShow = FALSE;

kal_uint32 Mex_Get_QQStatus(kal_uint32 qqId)
{
	if(MEX_QQ_MASTER == qqId)
	{
		mex_log(MEX_LOG_SYSTEM,"Mex_Get_QQStatus, master QQ,status:%d",g_MexMasterQQStatus);
		return g_MexMasterQQStatus;
	}
	else if(MEX_QQ_SLAVE == qqId)
	{
		mex_log(MEX_LOG_SYSTEM,"Mex_Get_QQStatus, slave QQ,status:%d",g_MexSlaveQQStatus);
		return g_MexSlaveQQStatus;
	}
	else
	{
		mex_log(MEX_LOG_SYSTEM, "Mex_Get_QQStatus, default return MASTER,ERROR QQ ID!!!!");
		return g_MexMasterQQStatus;
	}
}
#endif

#if (MEX_SDK_VER >= 1018)
extern E_MEX_Sim_Card_ID MexConfig_ConvertSimCard(E_MEX_Sim_Card_ID simId);
#endif

//获取系统当前设置的sim卡通道 一般使用在双卡需要切换通道的系统中
E_MEX_Sim_Card_ID Mex_GetPhoneUsableSim(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_DUAL_SIM_MASTER__
	{
#if (MEX_MTK_CODE_VERSION >= 0x1112)
		if (srv_sim_ctrl_is_available(MMI_SIM2))
			return E_MEX_SIM_2;
		else
			return E_MEX_SIM_1;
#elif (MEX_MTK_CODE_VERSION >= 0x1108)
		unsigned char sim_status;
		unsigned char mode_status;
		mmi_idle_get_dual_sim_status(&sim_status, &mode_status);
		if(MTPNP_AD_SIMCARD2_USABLE == sim_status)
		{
			return E_MEX_SIM_2;
		}
		else
		{
			return E_MEX_SIM_1;
		}
#else
		E_MTPNP_AD_SIMCARD_USABLE_SIDE state = MTPNP_AD_Get_UsableSide();
		if(MTPNP_AD_SIMCARD2_USABLE == state)
		{
			return E_MEX_SIM_2;
		}
		else
		{
			return E_MEX_SIM_1;
		}
#endif
	}
#else
	return E_MEX_SIM_1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MMI_DUALSIM_SUPPORT
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	int8 sim_index;
	sim_index = MNSIM_GetCurrentCardIndex();
	MNSIM_SetCurrentCardIndex(1);
	
	sim_index = MNSIM_GetCurrentCardIndex();
	if(sim_index == 1)
	{
		return E_MEX_SIM_1;
	}
	else
	{
		return E_MEX_SIM_2;
	}
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
	return E_MEX_SIM_1;
#endif
#else
	return E_MEX_SIM_1;
#endif
#endif
}
//设置系统sim卡通道只在特定系统中生效
void Mex_SetPhoneADChannel(E_MEX_Sim_Card_ID sim,E_MEX_AD_CHANNEL channel_type)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_DUAL_SIM_MASTER__
	{
		int32 channel = -1;
		switch(channel_type)
		{
		case E_MAC_CALL_CHANNEL:
			{
				channel = MTPNP_AD_CALL_CHANNEL;
			}
			break;
		case E_MAC_CALLLOG_CHANNEL:
			{
#if (MEX_MTK_CODE_VERSION < 0x0824)
				channel = MTPNP_AD_CALLLOG_CHANNEL;
#endif
			}
			break;
		case E_MAC_SMS_CHANNEL:
			{
				channel = MTPNP_AD_SMS_CHANNEL;			
			}
			break;
		case E_MAC_PHB_CHANNEL:
			{
#if (MEX_MTK_CODE_VERSION < 0x0824)
				channel = MTPNP_AD_PHB_CHANNEL;
#endif
			}
			break;
		default:
			break;
		}
		if(channel < 0)
			return;
		if(E_MEX_SIM_1 == sim)
		{
			MTPNP_AD_Free_Channel(channel);
			MTPNP_AD_Set_Channel(channel, MTPNP_AD_CHANNEL_MASTER);
		}
		else if(E_MEX_SIM_2 == sim)
		{
			MTPNP_AD_Free_Channel(channel);
			MTPNP_AD_Set_Channel(channel, MTPNP_AD_CHANNEL_SLAVE);
		}
	}
#elif defined(__SLT_DUAL_SIM_GEMINI__)
	{
		switch(channel_type)
		{
		case E_MAC_CALL_CHANNEL:
			{
				//todo
			}
			break;
		case E_MAC_CALLLOG_CHANNEL:
			{
				//todo
			}
			break;
		case E_MAC_SMS_CHANNEL:
			{
				if(E_MEX_SIM_1 == sim)
				{
					g_msg_cntx.slt_sim2_select = FALSE;
				}
				else if(E_MEX_SIM_2 == sim)
				{
					g_msg_cntx.slt_sim2_select = TRUE;
				}			
			}
			break;
		case E_MAC_PHB_CHANNEL:
			{
				//todo
			}
			break;
		default:
			break;
		}
	}
#else
	return ;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	if(E_MEX_SIM_1 == sim)
	{
		MNSIM_SetCurrentCardIndex((uint8)MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1));
	}
	else if(E_MEX_SIM_2 == sim)
	{
		MNSIM_SetCurrentCardIndex((uint8)MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2));
	}		
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
#endif
	
#endif
}

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
FuncPtr mex_imsi_func1 = NULL;
FuncPtr mex_imsi_func2 = NULL;
FuncPtr mex_imei_func1 = NULL;
FuncPtr mex_imei_func2 = NULL;
FuncPtr mex_notify_func = NULL;
FuncPtr mex_host_func = NULL;

void Mex_ClearProtocolEventHandler_onexit(void)
{
	if(mex_imsi_func1)
		mmi_frm_clear_protocol_event_handler(PRT_GET_IMSI_RSP, (PsIntFuncPtr)mex_imsi_func1);
	if(mex_imsi_func2)
		mmi_frm_clear_protocol_event_handler(PRT_GET_IMSI_RSP, (PsIntFuncPtr)mex_imsi_func2);
	if(mex_imei_func1)
		mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP, (PsIntFuncPtr)mex_imei_func1);
	if(mex_imei_func2)
		mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP, (PsIntFuncPtr)mex_imei_func2);
	if(mex_notify_func)
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)mex_notify_func);
	if(mex_host_func)
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)mex_host_func);
	
}
#endif

void Mex_SetProtocolEventHandler(void *funcPtr,uint16 eventID)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
    if(NULL == funcPtr)
    {
        return;
    }
	switch (eventID)
	{
	case PRT_GET_IMSI_RSP:
		
		if((mex_imsi_func1!=(FuncPtr)funcPtr)&&(mex_imsi_func1!=NULL))
			mex_imsi_func2=(FuncPtr)funcPtr;
		else
			mex_imsi_func1=(FuncPtr)funcPtr;
		
		break;
	case MSG_ID_MMI_NW_GET_IMEI_RSP:
		
		if((mex_imei_func1!=(FuncPtr)funcPtr)&&(mex_imei_func1!=NULL))
			mex_imei_func2=(FuncPtr)funcPtr;
		else
			mex_imei_func1=(FuncPtr)funcPtr;
		
		break;
	case MSG_ID_APP_SOC_NOTIFY_IND:
		mex_notify_func=(FuncPtr)funcPtr;
		break;
	case MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND:
		mex_host_func=(FuncPtr)funcPtr;
		break;
	}
	
	switch (eventID)
	{
	case MSG_ID_MMI_NW_GET_IMEI_RSP:
		mmi_frm_set_protocol_event_handler(eventID, (PsIntFuncPtr)funcPtr, MMI_FALSE);
		break;
	default:
		mmi_frm_set_protocol_event_handler(eventID, (PsIntFuncPtr)funcPtr, MMI_TRUE);
		break;
	}
#else
	SetProtocolEventHandler((PsFuncPtr)funcPtr,eventID);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
}
void Mex_ClearProtocolEventHandler(
								   uint16 eventID
#if (MEX_MTK_CODE_VERSION >= 0x1032)
								   , void *funcPtr
#endif	
								   )
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	if(NULL == funcPtr)
	{
        return;
	}
	mmi_frm_clear_protocol_event_handler(eventID, (PsIntFuncPtr)funcPtr);
#else
	ClearProtocolEventHandler(eventID);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}
void Mex_SetSlaveProtocolEventHandler(void *funcPtr,uint16 eventID)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(__MMI_DUAL_SIM__) || defined(__MMI_DUAL_SIM_MASTER__)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	Mex_SetProtocolEventHandler(funcPtr, eventID);
#else
	SetSlaveProtocolEventHandler((PsFuncPtr)funcPtr,eventID);
#endif
#elif defined(__SLT_DUAL_SIM_GEMINI__)
	slt_SetSim2ProtocolEventHandler((PsFuncPtr)funcPtr,eventID);
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}
void Mex_ClearSlaveProtocolEventHandler(
										uint16 eventID
#if (MEX_MTK_CODE_VERSION >= 0x1032)
										, void *funcPtr
#endif
										)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if defined(__MMI_DUAL_SIM__) || defined(__MMI_DUAL_SIM_MASTER__)
#if (MEX_MTK_CODE_VERSION >= 0x1032)	
	Mex_ClearProtocolEventHandler(eventID, funcPtr);
#else
	ClearSlaveProtocolEventHandler(eventID);
#endif
#elif defined(__SLT_DUAL_SIM_GEMINI__)
	slt_ClearSim2ProtocolEventHandler(eventID);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}


//==========================================================
char *Mex_Get_gprs_data_account_name(void *param)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	// TODO:
	return "";
#else
	mmi_ps_get_gprs_data_account_rsp_struct *localPtr;
	localPtr = (mmi_ps_get_gprs_data_account_rsp_struct *)param;	
	return (char *)localPtr->gprs_account.name;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return "";  
#endif  
}



bool MexInterface_CheckSetGprsReqResult(void *info)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	mmi_ps_set_gprs_data_account_rsp_struct *localPtr;	
	localPtr = (mmi_ps_set_gprs_data_account_rsp_struct *)info;
	return (localPtr != NULL && localPtr->result == KAL_TRUE); 
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    return false;
#endif
}
//===========================================================
#define STB_ICON_NUM 7
#define ICON_ID_SHIFT 0x10000

static STB_ICON_STATUS_T s_stb_icons[STB_ICON_NUM] = {0};

void MexInterface_SetStbIcon(int32 icon_id,STB_KEY_E key,int32 data)
{
    int i = 0;
    for(i = 0; i < STB_ICON_NUM; i++)
    {
        if(icon_id + ICON_ID_SHIFT == s_stb_icons[i].icon_id)
        {
            break;
        }
        else if(0 == s_stb_icons[i].icon_id)
        {
            s_stb_icons[i].icon_id = icon_id + ICON_ID_SHIFT;
            break;
        }
    }
    if(STB_ICON_NUM == i)
    {
        mex_log(MEX_LOG_SYSTEM,"W: please increase STB_ICON_NUM at MexInterface.c");
        return;
    }
    switch(key)
    {
	case STB_IMAGE:
		s_stb_icons[i].image_id = data;
		break;
	case STB_ISSHOW:
		s_stb_icons[i].is_show = data;
		break;
	default:
		break;
    }            
}

int32 MexInterface_GetStbIcon(int32 icon_id,STB_KEY_E key)
{
    int i = 0;
    for(i = 0; i < STB_ICON_NUM; i++)
    {
        if(icon_id + ICON_ID_SHIFT == s_stb_icons[i].icon_id)
        {
            break;
        }
    }
    if(STB_ICON_NUM == i)
    {
        return -1;
    }
    switch(key)
    {
	case STB_IMAGE:
		return s_stb_icons[i].image_id;
	case STB_ISSHOW:
		return s_stb_icons[i].is_show;
	default:
		return -1;
    }            
}

void Mex_ShowStatuIcon(int16 icon_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_MTK_CODE_VERSION >= 0x0952
	g_MexCheckIfShow = TRUE;
	wgui_status_icon_bar_show_icon(icon_id);
    switch(icon_id)
    {
#ifdef __MEX_QQ_SUPPORT__
	case STATUS_ICON_MEX_MASTER_QQ:
		g_MexMasterQQStatus = MEX_QQ_STATUS_OFFLINE;
		
		break;
	case STATUS_ICON_MEX_SLAVE_QQ:
		g_MexSlaveQQStatus = MEX_QQ_STATUS_OFFLINE;
		break;
#endif
#ifdef __MEX_MSN_SUPPORT__
	case STATUS_ICON_MEX_MSN:
		break;
#endif
#ifdef __MEX_FETION_SUPPORT__
	case STATUS_ICON_MEX_FETION:
		break;
#endif
	default:
		break;
    }
	
#else
	ShowStatusIcon(icon_id);
	UpdateStatusIcons();
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    MexInterface_SetStbIcon(icon_id, STB_ISSHOW, true);
	
	GUIWIN_SetStbItemVisible(icon_id, true);
	GUIWIN_UpdateStb();
	
#endif        
}
void Mex_HideStatuIcon(int16 icon_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_MTK_CODE_VERSION >= 0x0952
	g_MexCheckIfShow = FALSE;
	wgui_status_icon_bar_hide_icon(icon_id);
    switch(icon_id)
    {
#ifdef __MEX_QQ_SUPPORT__
	case STATUS_ICON_MEX_MASTER_QQ:
		g_MexMasterQQStatus = MEX_QQ_STATUS_NONE;
		
		break;
	case STATUS_ICON_MEX_SLAVE_QQ:
		g_MexSlaveQQStatus = MEX_QQ_STATUS_NONE;
		break;
#endif
#ifdef __MEX_MSN_SUPPORT__
	case STATUS_ICON_MEX_MSN:
		break;
#endif
#ifdef __MEX_FETION_SUPPORT__
	case STATUS_ICON_MEX_FETION:
		break;
#endif
	default:
		break;
    }
	
#else
	HideStatusIcon(icon_id);
	UpdateStatusIcons();
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    MexInterface_SetStbIcon(icon_id, STB_ISSHOW, false);
	GUIWIN_SetStbItemVisible(icon_id, false);
	GUIWIN_UpdateStb();
	
#endif        
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void Mex_ChangeStatusIconImage(int16 icon_id,uint16 image_ID)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void Mex_ChangeStatusIconImage(int16 icon_id,uint32 image_ID)
#endif

{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_MTK_CODE_VERSION >= 0x0952
	wgui_status_icon_bar_change_icon_image(icon_id,image_ID);
	switch(icon_id)
	{	
		int status_id;
		
#ifdef __MEX_QQ_SUPPORT__
	case STATUS_ICON_MEX_MASTER_QQ:
	case STATUS_ICON_MEX_SLAVE_QQ:
		{
			switch(image_ID)
			{
			case E_MII_MEX_QQ_ONLINE_ICON:
				status_id = MEX_QQ_STATUS_ONLINE;
				break;
			case E_MII_MEX_QQ_OFFLINE_ICON:
				status_id = MEX_QQ_STATUS_OFFLINE;
				break;
			case E_MII_MEX_QQ_MSG_ICON:
				status_id = MEX_QQ_STATUS_MSG;
				break;
			case E_MII_MEX_QQ_HIDE_ICON:
				status_id = MEX_QQ_STATUS_HIDE;
				break;
			case E_MII_MEX_QQ_AWAY_ICON:
				status_id = MEX_QQ_STATUS_AWAY;
				break;
			default:
				status_id = MEX_QQ_STATUS_NONE;
				break;
			}
			if(STATUS_ICON_MEX_MASTER_QQ == icon_id)
				g_MexMasterQQStatus = status_id;
			else
				g_MexSlaveQQStatus  = status_id;
		}
		break;
#endif
#ifdef __MEX_MSN_SUPPORT__
	case STATUS_ICON_MEX_MSN:
		break;
#endif
#ifdef __MEX_FETION_SUPPORT__
	case STATUS_ICON_MEX_FETION:
		break;
#endif
	default:
		break;
	}
#else
	ChangeStatusIconImage(icon_id, image_ID);
	UpdateStatusIcons();
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    MexInterface_SetStbIcon(icon_id, STB_IMAGE, image_ID);
	GUIWIN_SetStbItemIconId(icon_id, image_ID);
	GUIWIN_SetStbItemIconCurFrame(icon_id, 0);
	
	/*if (image_ID != IMAGE_NULL)
	{
	GUIWIN_SetStbItemVisible(icon_id, TRUE);
	}
	else
	{
	GUIWIN_SetStbItemVisible(icon_id, FALSE);
}*/
	GUIWIN_UpdateStb();
#endif    
}

#ifdef __RICH_AUDIO_PROFILE__

int32 MexInterface_audio_start_calc_spectrum(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern kal_int32 media_aud_start_calc_spectrum(module_type src_mod_id);
	return media_aud_start_calc_spectrum(MOD_MMI);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif    
}

int32 MexInterface_audio_stop_calc_spectrum(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern kal_int32 media_aud_stop_calc_spectrum(module_type src_mod_id);
	return media_aud_stop_calc_spectrum(MOD_MMI);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}

int32 MexInterface_audio_get_spectrum(uint8 *top_p,uint8 *val_p)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern kal_int32 media_aud_get_spectrum(module_type src_mod_id, kal_uint8 *top_p, kal_uint8 *val_p);
	return media_aud_get_spectrum(MOD_MMI,top_p,val_p);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}
#endif

void *MexConfig_smalloc(size_t size);
void MexConfig_sfree(void *ptr);

//申请外部内存，自行修改编译不通过的地方，或添加其他申请内存的途径
void* MexInterface_xmalloc(size_t iSize)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern bool g_bDuringXMalloc;
	void *pRet = NULL;
	//med remaining mem
	if (pRet == NULL)
	{
		extern address_t med_ext_smalloc(size_type);
		pRet = (void*)med_ext_smalloc(iSize + 4);
		if (pRet != NULL)
		{
			*(int16*)pRet = 1;
		}
	}
	
	//scrmem
	if (pRet == NULL)
	{
#if (MEX_MTK_CODE_VERSION >= 0x0932)
		extern void *applib_mem_screen_alloc_int(kal_uint32 mem_size, kal_bool is_framebuffer, kal_uint32 line);
#else
		extern void *applib_mem_screen_alloc(kal_uint32 mem_size);
#endif
		g_bDuringXMalloc = true;
#if (MEX_MTK_CODE_VERSION >= 0x0932)
		pRet = applib_mem_screen_alloc_int(iSize + 4, false, __LINE__);
#else
		pRet = applib_mem_screen_alloc(iSize + 4);
#endif
		g_bDuringXMalloc = false;
		if (pRet != NULL)
		{
			*(int16*)pRet = 2;
		}
	}
	//browser
	if (pRet == NULL)
	{
#if defined(OBIGO_Q03C_BROWSER) || defined(OPERA_V10_BROWSER)// for mtk 10a
        // TODO:
        return NULL;
#else
		int16 pool;
		extern void *wapadp_mem_alloc(uint8 poolId, uint8 user, uint32 size);
		for (pool=0;pool<WAP_MEM_POOL_NUM;pool++)
		{
			g_bDuringXMalloc = true;
			pRet = wapadp_mem_alloc(pool, 0, iSize + 4);
			g_bDuringXMalloc = false;
			if (pRet != NULL)
			{
				*(int16*)pRet = 3;
				mex_memcpy((void*)((int32)pRet+2), &pool, 2);
				break;
			}
		}
#endif
	}
	
#if defined(MEX_MEMORY_GUARD)
	if (pRet != NULL)
	{
		extern int32 g_MemGuardNbXMalloc;
		g_MemGuardNbXMalloc++;
	}
#endif
	
	if (pRet == NULL)
		return NULL;
	mex_memset( (void*)((int32)pRet + 4), 0, iSize );
	return (void*)((int32)pRet + 4);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	void *pRet = NULL;
	pRet = MexConfig_smalloc(iSize+4);
	if(pRet != NULL)
	{
		*(int16*)pRet = 1; //快速内存类型
	}
	else
	{
		pRet = mex_malloc(iSize + 4);       /*lint !e611 */
		if(pRet)
			*(int16*)pRet = 2;//普通内存类型
	}
	if(NULL == pRet)
		return NULL;
	return (void*)((int32)pRet + 4);
#endif
}

//释放内存，同上
void MexInterface_xfree(void *ptr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	if (ptr != NULL)
	{
		void *p = (void*)((int32)ptr - 4);
		if (*(int16*)p == 1)
		{
			extern void med_ext_sfree(address_t);
			med_ext_sfree((address_t)p);
		}
		
		else if (*(int16*)p == 2)
		{
#if (MEX_MTK_CODE_VERSION >= 0x0932)
			extern void applib_mem_screen_free_int(void *mem_ptr, kal_uint32 line);
			applib_mem_screen_free_int(p, __LINE__);
#else
			extern void applib_mem_screen_free(void *);
			applib_mem_screen_free(p);
#endif
		}
		
		else if (*(int16*)p == 3)
		{
#if defined(OBIGO_Q03C_BROWSER)|| defined(OPERA_V10_BROWSER) // for mtk 10a
            // TODO:
            return NULL;
#else
			int16 pool;
			extern void wapadp_mem_free(uint8 poolId, uint8 user, void *p);
			mex_memcpy(&pool, (void*)((int32)p + 2), 2);
			wapadp_mem_free(pool, 0, p);
#endif
		}
		
#if defined(MEX_MEMORY_GUARD)
		{
			extern int32 g_MemGuardNbXMalloc;
			g_MemGuardNbXMalloc--;
		}
#endif
	}
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	if(ptr != NULL)
	{
		void *p = (void*)((int32)ptr - 4);
		if (*(int16*)p == 1)
		{
			MexConfig_sfree(p); //快速内存
		}
		else if (*(int16*)p == 2)
		{
			mex_free(p); /*lint !e611 */        //普通内存        
		}
		p=NULL;
	}
#endif    
}

// 获取系统字体信息
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
uint32 MexInterface_GetFontData(U32 Ch, stFontAttribute Font, U8 **ppCharData, U16 *pnWidth, U16 *pnHeight, U16 *pdataWidth, U16 *pdataHeight, PU16 ascent, PU16 descent)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
uint32 MexInterface_GetFontData(uint32 Ch, void * Font, uint8 **ppCharData, uint16 *pnWidth, uint16 *pnHeight, uint16 *pdataWidth, uint16 *pdataHeight, uint16 *ascent, uint16 *descent)
#endif
{ 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if  (MEX_MTK_CODE_VERSION >= 0x1112)
	// TODO:
	return 0;
#elif (MEX_MTK_CODE_VERSION >= 0x08B0)
	{
		
		extern U32 mmi_fe_get_font_data(
			U8 type,
			U16 ch, 
			U8 **ppCharData, 
			U32 *data_attr,
			S32 *pnWidth, 
			S32 *advance_x, 
			U16 *glyph_width, 
			U16 *glyph_height, 
			S32 *bearing_x,
			S32 *ascent, 
			S32 *descent,
			MMI_BOOL *valid);
		MMI_BOOL valid;
		U32 data_attr;
		S32 pWidth;
		S32 advance_x;
		U16 glyph_width;
		U16 glyph_height; 
		S32 bearing_x;
		S32 pAscent;
		S32 pDescent;
		uint32 nBytes = 0;
		nBytes = mmi_fe_get_font_data(MMI_FALSE, (U16)Ch, ppCharData, &data_attr, &pWidth, &advance_x, &glyph_width, &glyph_height, &bearing_x, &pAscent, &pDescent, &valid);
		*pnWidth = pWidth;
		*pnHeight = 0;//??
		*pdataWidth = glyph_width;
		*pdataHeight = glyph_height;
		return nBytes; // Fix Bug #8624
	}	
#elif(MEX_MTK_CODE_VERSION >= 0x0824)
	{
		extern U32 GetFontdata(U32 Ch, stFontAttribute Font, U8 **ppCharData, U16 *pnWidth, U16 *pnDWidth, U16 *pnHeight, U16 *pdataWidth, U16 *pdataHeight, PU16 ascent, PU16 descent);
		U16 pnDWidth;  //???
		return GetFontdata(Ch, Font, ppCharData, pnWidth,&pnDWidth,pnHeight, pdataWidth, pdataHeight, ascent, descent);
	}
#else
	{
		extern U32 GetFontdata(U32 Ch, stFontAttribute Font, U8 **ppCharData, U16 *pnWidth, U16 *pnHeight, U16 *pdataWidth, U16 *pdataHeight, PU16 ascent, PU16 descent);
		return GetFontdata(Ch, Font, ppCharData, pnWidth, pnHeight, pdataWidth, pdataHeight, ascent, descent);
	}
#endif
	return 0;
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 0;
#endif    
	
}

// 获取系统字体尺寸
void MexInterface_GetCharWidthHeight(uint32 Ch, int32 *pWidth, int32 *pHeight)
{ 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern void Get_CharWidthHeight(uint32 Ch, int32 *pWidth, int32 *pHeight);
	extern int32 MexConfig_VectorFont_CharAddPixels(void);
	
	Get_CharWidthHeight(Ch, pWidth, pHeight);
	*pWidth += MexConfig_VectorFont_CharAddPixels();
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif        
}




#if(MEX_MTK_CODE_VERSION >= 0x1032)

bool g_bNeedAddGprsAcct = true;
int32 iProfAccountIndex = 0;
bool g_bResumeGprsProf = false;

srv_dtcnt_store_prof_data_struct g_storeProfDataStruct = { 0 };

srv_dtcnt_prof_gprs_struct g_Mex_data_account_gprs_profile_srv = { 0 };


#endif


//=============================================================================================

/*网络操作相关的函数*/

//获取真正的网络账号
uint32 MexInterface_always_ask_encode_data_account(uint32 acct_id, int32 sim_src, uint8 app_id, bool always_ask, int8 mode)
{ 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	
	int32 iIndex; 
	wchar_t mex_apn[100];
	wchar_t *pApn = NULL;
	srv_dtcnt_sim_type_enum  sim_info;
	
	if(mode == E_MNM_CMWAP)
	{
		pApn = L"cmwap";
	}
	else
	{
		pApn = L"cmnet";
	}
	
	g_bNeedAddGprsAcct = true;
	
	//for mtk wifi
#if (defined(__WIFI_SUPPORT__) && defined(__TCPIP__))
	{
		uint32 account_id = 0;
		//连接上的代码
		if(srv_dtcnt_wlan_status() == SRV_DTCNT_WLAN_STATUS_CONNECTED && CBM_OK ==  cbm_get_valid_account_id(CBM_BEARER_WIFI,&account_id))
		{
			g_bNeedAddGprsAcct = false;
			return cbm_encode_data_account_id(account_id, sim_src, 1, always_ask);
		}
	}
#endif 
	
	for (iIndex = 0; iIndex < SRV_DTCNT_PROF_MAX_ACCOUNT_NUM; iIndex++)
	{
#if (MEX_MTK_CODE_VERSION < 0x1112)
		extern BOOL mmi_dtcnt_get_apn(U32 account_id, S8 *dest, U8 len);
		memset(mex_apn, 0, 20);
		mmi_dtcnt_get_apn((U32)iIndex, (S8 *)mex_apn, 20);	  
#else
		{
			bool bRet = false;
			srv_dtcnt_prof_str_info_qry_struct acc_data;
			acc_data.dest = (S8*)mex_apn;
			acc_data.dest_len = sizeof(mex_apn);
			bRet = srv_dtcnt_get_apn(
				iIndex, 
				&acc_data, 
				SRV_DTCNT_ACCOUNT_PRIMARY);
			if (!bRet)
				mex_apn[0] = L'\0';
		}
		
#endif
		srv_dtcnt_get_sim_info((U32) iIndex, &sim_info);
		if(( mmi_wcsnicmp(mex_apn, pApn, 6) == 0 )&&(sim_info==(sim_src+1)))
		{ 	
			g_bNeedAddGprsAcct = false;
			iProfAccountIndex = iIndex;
			return cbm_encode_data_account_id(iIndex, sim_src, 1, always_ask);
		}
	}
#ifdef MEX_DEBUG
	mex_log(MEX_LOG_SOC,"get account %d",iIndex);
#endif
	return cbm_encode_data_account_id(acct_id,sim_src,1,always_ask);
	
	
#elif (MEX_MTK_CODE_VERSION >= 0x0824)
	return cbm_encode_data_account_id(acct_id,sim_src,1,always_ask);
#else
	return always_ask_encode_data_account_id(acct_id, sim_src, 0, always_ask);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 1;
	
#endif        
}






#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

#if(MEX_MTK_CODE_VERSION >= 0x0924)
static void GPRSStatusDetachGPRSReq(void)
{
#if(MEX_MTK_CODE_VERSION >= 0x1112)
#ifdef __MMI_GPRS_FEATURES__
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	MYQUEUE msgStruct;
	mmi_nw_set_attach_req_struct *local_data;
	
	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = MSG_ID_MMI_NW_SET_ATTACH_REQ;
	local_data = (mmi_nw_set_attach_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_attach_req_struct));
	local_data->is_poweroff = MMI_FALSE;
	local_data->opcode = RAC_DETACH;
	local_data->type = RAC_PS;
	msgStruct.oslDataPtr = (oslParaType*) local_data;
	msgStruct.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&msgStruct);
	return;
#endif /* __MMI_GPRS_FEATURES__ */ 
#else
#ifdef __MMI_GPRS_FEATURES__
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MYQUEUE msgStruct;
	mmi_nw_set_attach_req_struct *local_data;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = MSG_ID_MMI_NW_SET_ATTACH_REQ;
	local_data = (mmi_nw_set_attach_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_attach_req_struct));
	local_data->is_poweroff = MMI_FALSE;
	local_data->opcode = RAC_DETACH;
	local_data->type = RAC_PS;
	msgStruct.oslDataPtr = (oslParaType*) local_data;
	msgStruct.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&msgStruct);
	return;
#endif /* __MMI_GPRS_FEATURES__ */ 
#endif
}
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)




#endif        





//关闭网络账号
void MexInterface_SocCloseNWKAccountById(uint8 mod_id, uint32 account_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if(MEX_MTK_CODE_VERSION >= 0x1032)
	{
		if(g_bResumeGprsProf)
		{
			extern void MexInterface_ResumeGprsAccount(void);
			MexInterface_ResumeGprsAccount();
		}
	}
#endif
	
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	soc_app_deactivate_req_hdlr(mod_id, account_id, 0, KAL_TRUE);
	
#else
	SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_DEACTIVATE_CNF);
	soc_close_nwk_account_by_id(mod_id, account_id);
#endif
	GPRSStatusDetachGPRSReq();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return;
#endif
}




//打开网络账号
int8 MexInterface_SocOpenNWKAccount(uint8 mod_id, uint32 account_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	return 0;
#else
	return soc_open_nwk_account(mod_id, account_id);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 0;
#endif
}






int8 MexInterface_SocOpenBearer(int8 s)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	return 0;
#else
	return soc_open_bearer(s);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 0;
#endif
}




int8 MexInterface_SocCloseBearer(int8 s)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	return 0;
#else
	return soc_close_bearer(s);
#endif	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 0;
#endif
}





//关闭某模块的网络
void MexInterface_SocCloseNWKAccount(uint8 mod_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if(MEX_MTK_CODE_VERSION >= 0x1032)
	{
		if(g_bResumeGprsProf)
		{
			extern void MexInterface_ResumeGprsAccount(void);
			MexInterface_ResumeGprsAccount();
		}
	}
#endif
	
	
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
	//cbm_release_bearer(MOD_MMI, 1);			
	//cbm_deregister_app_id(1); 
	
#else
	SetProtocolEventHandler (NULL, MSG_ID_APP_SOC_DEACTIVATE_CNF);
	soc_close_nwk_account(mod_id);
#endif	
	GPRSStatusDetachGPRSReq();
	mex_log(MEX_LOG_SOC,"CLOSE ACC");
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	mex_log(MEX_LOG_SOC, "CLOSE GPRS");
#if (MEX_SPRD_CODE_VERSION >= 0x09A1109)
	MMIAPIPDP_Deactive(MMI_MODULE_MET_MEX);
#else
	{
		//断开GPRS连接
		MNGPRS_ResetAndDeactivePdpContextEx(g_met_mex_phone_sim);	
	}
#endif
#endif
	
}




#define MEX_TELECA_WAP

//打开wap浏览器 并访问指定的url
void MexInterface_ConnectWap(char *url)
{
#ifdef BROWSER_SUPPORT
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x1112)
	Mex_Pause_System(); // mexsystem pause
	wap_start_browser(WAP_BROWSER_GOTO_URL, (const uint8 *) mex_tstr2wcs(url));
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
	GoBackHistory(); // mexsystem pause
	wap_start_browser(WAP_BROWSER_GOTO_URL, (const uint8 *) mex_tstr2wcs(url));
#else
	
#ifdef MMI_ON_HARDWARE_P
#ifndef MEX_POLLEX_WAP
	ilm_struct *ilm_ptr = NULL;
	wap_browser_startup_req_struct *param_ptr;
#endif
	GoBackHistory(); // mexsystem pause
	
#ifdef MEX_POLLEX_WAP
	EntryOfPlxBrowserView(url);
#endif
	
#ifdef  MEX_TELECA_WAP
	param_ptr = (wap_browser_startup_req_struct*)construct_local_para(sizeof(wap_browser_startup_req_struct), TD_CTRL);
	param_ptr->type = WAP_BROWSER_GOTO_URL;
	strcpy ((char*)param_ptr->url, (char*)url);
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	
	ilm_ptr = allocate_ilm(MOD_MMI);
	ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
	ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
	ilm_ptr->peer_buff_ptr  = NULL;
	
	SEND_ILM(MOD_MMI, MOD_WAP, WAP_MMI_SAP, ilm_ptr);
#endif
	
#ifdef MEX_JATAYU_WAP
	if (!wap_is_ready())
	{
		wap_entry_no_ready_screen(WAP_SERVICE_BROWSER);
		return;
	}
	
	/* Disable key to prevent sending duplicated requests */
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	
	param_ptr = (wap_browser_startup_req_struct*)construct_local_para(sizeof(wap_browser_startup_req_struct), TD_CTRL);
	param_ptr->type = 2;
	strcpy ((char*)param_ptr->url, (char*)url);
	ilm_ptr = allocate_ilm(MOD_MMI);
	ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
	ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
	ilm_ptr->peer_buff_ptr  = NULL;
	ilm_ptr->src_mod_id  = MOD_MMI;
	ilm_ptr->dest_mod_id = MOD_WAP;
	ilm_ptr->sap_id = WAP_MMI_SAP;
	msg_send_ext_queue(ilm_ptr);
#endif // MEX_JATAYU_WAP
#endif // MMI_ON_HARDWARE_P
#endif 
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
	MMICMSBRW_AccessUrl(url, MMIBRW_GetActiveSim());
#else
#ifdef BROWSER_SUPPORT_NETFRONT
    MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL,url);
#else
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
	MMIBROWSER_Init();
#endif
	MMIBROWSER_UpdateUrl(url);	
	MMIBROWSER_SetSim(g_met_mex_phone_sim);
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
	MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)(url), PNULL);
	MMIBROWSER_FreeEntryUrl();
#else
	MMIBROWSER_EnterMainWin();
#endif
#endif    
#endif            
	
	
	return;
#endif
#endif
}

//==============================================================================

//simulate incoming call
bool MexPhs_IncomingCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	ilm_struct *ilm_ptr;
	mmi_cc_call_ring_ind_struct *p;
	module_type mod_id;
	
	// Check the number of incoming group (Fix Bug #5431)
	{
#if (MEX_MTK_CODE_VERSION < 0x0952)
		int32 nGroup; // number of incoming group
		nGroup = mmi_ucm_get_incoming_group(MMI_UCM_CALL_TYPE_ALL, g_ucm_p->call_misc.index_list);
		if (nGroup > 0)
		{
			mex_log(MEX_LOG_PHS, "InCall E: nGrp:%d", nGroup);
			return false;
		}
		
#else
		if (MetMex_GCall_IsBusy())
		{
			mex_log(MEX_LOG_PHS, "InCall E: gcall busy");
			return false;
		}
#endif
		
	}
	
	switch (eSimId)
	{
	case E_MEX_SIM_1:
		if (MexConfig_GetPhoneConfig(E_MPC_Master_DummyInCall) == 0)
		{
			mex_log(MEX_LOG_PHS, "InCall E: sim1 no-sup");
			return false;
		}
		mod_id = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C);
		break;
	case E_MEX_SIM_2:
		if (MexConfig_GetPhoneConfig(E_MPC_Slave_DummyInCall) == 0)
		{
			mex_log(MEX_LOG_PHS, "InCall E: sim2 no-sup");
			return false;
		}
		mod_id = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_2);
		break;
#ifdef __MET_MEX_SIM_PLUS__
	case E_MEX_SIM_3:
		if (MexConfig_GetPhoneConfig(E_MPC_Slave_DummyInCall) == 0)
		{
			mex_log(MEX_LOG_PHS, "InCall E: sim2 no-sup");
			return false;
		}
		mod_id = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_3);
		break;
		
	case E_MEX_SIM_4:
		if (MexConfig_GetPhoneConfig(E_MPC_Slave_DummyInCall) == 0)
		{
			mex_log(MEX_LOG_PHS, "InCall E: sim2 no-sup");
			return false;
		}
		mod_id = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_4);
		break;
#endif/*__MET_MEX_SIM_PLUS__*/
	default:
		mex_log(MEX_LOG_PHS, "InCall E: sim:%d", eSimId);
		return false;
	}
	
	ilm_ptr = allocate_ilm(mod_id);
	p = (mmi_cc_call_ring_ind_struct*)construct_local_para(sizeof(mmi_cc_call_ring_ind_struct), 0);
	memset(p, 0, sizeof(mmi_cc_call_ring_ind_struct));
	p->ref_count = 1;
	p->msg_len = sizeof(mmi_cc_call_ring_ind_struct);
	p->call_id = 1;
	p->num.type = 129; // default type
	mex_wcs2nstr((char *)p->num.number, MAX_CC_ADDR_LEN, wszNumber);
	p->num.length = mex_strlen((S8*)p->num.number);
	mex_log(MEX_LOG_PHS, "InCall num:%s len:%d", p->num.number, p->num.length);
	
	ilm_ptr->msg_id = (uint16) MexConfig_ConvertMtkDef(E_M2MDC_MSG_MMI_CALL_RING_IND)/*MSG_ID_MMI_CC_CALL_RING_IND*//*2312U*/;/* Set the message id */
	ilm_ptr->peer_buff_ptr = NULL;/* there are no peer message */
	ilm_ptr->local_para_ptr = (local_para_struct *)p;
	
	//SEND_ILM(MOD_L4C,MOD_MMI,MMI_L4C_SAP,ilm_ptr);
	SEND_ILM(mod_id,
		MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI),
		MexConfig_ConvertMtkDef(E_M2MDC_MMI_L4C_SAP),
		ilm_ptr);
	g_Mex_DummyInCall = mod_id;
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	extern void MexConfig_ConvertAnsi2BCD(int8 *bcd_str,int32 bcd_len,int8 *ansi_str,int32 ansi_len, int32 offset);
	
	MN_DUAL_SYS_E eSimType;
	
	
	typedef struct
	{    
		uint16 SignalCode;
		uint16 SignalSize;
		void *  Pre;
		void *  Suc;
		uint32  Sender;
		
		uint8                       call_id;
		BOOLEAN                     calling_num_present;        /*!< indicate the calling_num is present or not */
		MN_CALLING_NUMBER_T         calling_num;                /*!< the remote user's address */
		BOOLEAN                     calling_sub_addr_present;
		MN_SUBADDR_T                calling_sub_addr;           /*!< the remote user's subaddress */
		MN_DUAL_SYS_E               dual_sys;
#ifdef MMI_DUALMODE_ENABLE
		MN_CALL_TYPE_E      call_type;
#endif
	} Mex_App_MN_Setup_IND_T;
	
	typedef struct
	{    
		uint16 SignalCode;
		uint16 SignalSize;
		void *  Pre;
		void *  Suc;
		uint32  Sender;
		
		uint8                       call_id;  
		MN_CALL_ALERT_TYPE_E        alerting_type;      /*!< indicate the alerting type i.e(inband tone or remote alert) */
		MN_DUAL_SYS_E               dual_sys;
		
	} Mex_App_MN_Alerting_Ind_T;
	
	Mex_App_MN_Setup_IND_T    *pSig_Setup = NULL;
	Mex_App_MN_Alerting_Ind_T *pSig_Alerting = NULL;
	
#if (MEX_SDK_VER >= 1018)
	eSimId = MexConfig_ConvertSimCard(eSimId);
#endif
	
	switch (eSimId)
	{
	case E_MEX_SIM_1:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
		break;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)        
	case E_MEX_SIM_2:
		
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
		break;
#endif        
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)    //支持三SIM卡
	case E_MEX_SIM_3:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
		break;
#endif //MEX_THIRD_SIM
		
	default:
		mex_log(MEX_LOG_PHS, "InCall E: sim:%d", eSimId);
		return false;
	}
	
	
	
	// check sc & number & content
	SCI_CREATE_SIGNAL(pSig_Setup,APP_MN_SETUP_IND,sizeof(Mex_App_MN_Setup_IND_T),SCI_IdentifyThread());	/*lint !e668*/
	if (pSig_Setup == NULL)     /*lint !e774*/
	{
		mex_log(MEX_LOG_PHS, "NewSms E: no memory");
		return false;
	}
	SCI_CREATE_SIGNAL(pSig_Alerting,APP_MN_ALERTING_IND,sizeof(Mex_App_MN_Alerting_Ind_T),SCI_IdentifyThread());/*lint !e668*/
	if (pSig_Alerting == NULL)  /*lint !e774*/
	{
		SCI_FREE_SIGNAL(pSig_Setup)
			mex_log(MEX_LOG_PHS, "NewSms E: no memory");
		return false;
	}
	
	
	{
		MexConfig_ConvertAnsi2BCD(
			(int8 *)pSig_Setup->calling_num.party_num, 
			sizeof(pSig_Setup->calling_num.party_num),
			mex_twcs2str(wszNumber), 
			mex_wcslen(wszNumber),
			0);
		pSig_Setup->calling_num.num_len = mex_wcslen(wszNumber);
		pSig_Setup->calling_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
		pSig_Setup->calling_num_present = true;        
		pSig_Setup->dual_sys = eSimType;
		
		pSig_Alerting->alerting_type = MN_CALL_LOCAL_USER_ALERTING;       
		pSig_Alerting->dual_sys = eSimType;
	}
	
	// send message
	{            
		SCI_SEND_SIGNAL((xSignalHeaderRec*)pSig_Setup,P_APP);
		SCI_SEND_SIGNAL((xSignalHeaderRec*)pSig_Alerting,P_APP);
		
		return true;
	}
#endif
	
}
//===================================================================== Sms Read Start

typedef struct
{
	bool                        processing;
	E_MEX_PHS_SMS_MSGBOX        msgBox;
	uint16                      index;
	Mex_Phs_Save_Sms_CB         callback;
	int32                       appId;
	
} MexPhs_Save_Sms_Context;

typedef struct
{
	bool                        processing;
	E_MEX_PHS_SMS_MSGBOX        msgBox;
	uint16                      index;
	Mex_Phs_Read_Sms_CB         callback;
	int32                       appId;
	
} MexPhs_Read_Sms_Content;

typedef struct
{
	bool                m_bProcessing;
	Mex_Phs_Del_Sms_CB  m_pCallBack;
	int32               m_iAppId;
	E_MEX_PHS_SMS_MSGBOX msgBox;
	uint16               index;
} MexPhs_DelSms_Context;

static MexPhs_Save_Sms_Context g_mex_savesms_context = { 0 };

static MexPhs_Read_Sms_Content g_mex_readsms_context = { 0 };

static MexPhs_DelSms_Context g_mex_delsms_context = { 0 };


//========================================================== Simulate Sms Begin

// 1. 13800138000
// 2. +8613800138000
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexPhs_ConvertStringToSmsAddr(const char* pSrc, sms_addr_struct *pDest)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void MexPhs_ConvertStringToSmsAddr(const char* pSrc, void *pDest)
#endif
{
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	uint16 i = 0, j = 0;
	int iNumLen = strlen(pSrc);
	const char *pNum = pSrc;
	
	memset(pDest, 0, sizeof(sms_addr_struct));
	// number type
	if (pNum[i] == '+')
	{
		pDest->addr_bcd[j] = 0x91; // international type
		i++;
		j++;
	}
	else
	{
		pDest->addr_bcd[j] = 0x81; // default type
		j++;
	}
	pDest->addr_length++;
	
	while (i < iNumLen)
	{
		uint8 c1 = pNum[i];
		uint8 c2 = pNum[i+1];
		i += 2;
		
		if (c1 >= '0' && c1 <= '9')
		{
			pDest->addr_bcd[j] = (c1 - '0');
		}
		if (c2 >= '0' && c2 <= '9')
		{
			pDest->addr_bcd[j] |= (c2 - '0') << 4;
		}
		else if (c2 == '\0')
		{
			pDest->addr_bcd[j] |= 0xF0;
		}
		j++;
		pDest->addr_length++;
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	uint16 i = 0, j = 0;
	int iNumLen = strlen(pSrc);
	const char *pNum = pSrc;
	int iNumberLength = 0;
	uint8  *pDestNumber = &((uint8 *)pDest)[2];
	
	memset(pDest, 0, 14);
	// number type
	if (pNum[i] == '+')
	{
		pDestNumber[j] = 0x91; // international type
		i++;
		j++;
	}
	else
	{
		pDestNumber[j] = 0x81; // default type
		j++;
	}
	
	iNumberLength++;
	
	while (i < iNumLen)
	{
		uint8 c1 = pNum[i];
		uint8 c2 = pNum[i+1];
		i += 2;
		
		if (c1 >= '0' && c1 <= '9')
		{
			pDestNumber[j] = (c1 - '0');
		}
		if (c2 >= '0' && c2 <= '9')
		{
			pDestNumber[j] |= (c2 - '0') << 4;
		}
		else if (c2 == '\0')
		{
			pDestNumber[j] |= 0xF0;
		}
		j++;
		iNumberLength++;
	}
	
	*(uint16 *)pDest = iNumberLength;
#endif    
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_mytime
* DESCRIPTION
*  convert MYTIME to scts
* PARAMETERS
*  scts        [?]     Scts
*  time        [?]     
*  b  timestamp      timestamp(?)
* RETURNS
*  void
*****************************************************************************/
void MexPhs_ConvertTime2SmsTime(uint8 *scts, Mex_DateTime *pDt)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	uint8 year = 0;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (pDt->year > 2000)
	{
		year = (uint8)(pDt->year - 2000);
	}
	else
	{
		year = (uint8)(pDt->year - 1900);
	}
	
	scts[0] = ((year % 10) << 4) | (year / 10);
	scts[1] = ((pDt->month % 10) << 4) | (pDt->month / 10);
	scts[2] = ((pDt->day % 10) << 4) | (pDt->day / 10);
	scts[3] = ((pDt->hour % 10) << 4) | (pDt->hour / 10);
	scts[4] = ((pDt->minute % 10) << 4) | (pDt->minute / 10);
	scts[5] = ((pDt->second % 10) << 4) | (pDt->second / 10);
}


bool MexPhs_NewSms(E_MEX_Sim_Card_ID eSimId, const char* pSmsCenter, const char* pSmsNumber, const wchar_t *pSmsContent, const Mex_DateTime *pDt)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	module_type mod_src;
	module_type mod_dest;
	uint8 iSmsContentLen;
	sms_deliver_struct *pLocalPara = NULL;
	peer_buff_struct *pPeerBuffer = NULL;
	if((NULL == pSmsCenter)||(NULL == pSmsNumber)||(NULL == pSmsContent)||(NULL == pDt))
	{
        mex_log(MEX_LOG_PHS,"NewSms E: NULL parameter");
        return false;
	}
	
	switch (eSimId)
	{
	case E_MEX_SIM_1:
		mod_src = MOD_SMS;
		mod_dest = MOD_SMSAL;
		break;
	case E_MEX_SIM_2:
#ifdef __MMI_DUAL_SIM_MASTER__
		mod_src = MOD_SMS_2;
		mod_dest = MOD_SMSAL_2;
		break;
#else
		return false;
#endif	
	default:
		mex_log(MEX_LOG_PHS, "NewSms E: sim:%d", eSimId);
		return false;
	}
	
	// check sc & number & content
	iSmsContentLen = mex_wcslen(pSmsContent);
	if (strlen(pSmsCenter) > 20)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_sc > 20");
		return false;
	}
	if (strlen(pSmsNumber) > 20)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_number > 20");
		return false;
	}
	if (iSmsContentLen > 70)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_content > 70");
		return false;
	}
	
	pLocalPara = construct_local_para(sizeof(sms_deliver_struct), 0);
	if (pLocalPara == NULL)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: no memory");
		return false;
	}
	memset(pLocalPara, 0, sizeof(sms_deliver_struct));
	pLocalPara->ref_count = 1;
	pLocalPara->msg_len = sizeof(sms_deliver_struct);
	pLocalPara->smi = 0;
	MexPhs_ConvertStringToSmsAddr(pSmsCenter, &pLocalPara->sc_addr); // sms center number
	
	// sms pdu
	// iPeerBufferLen = iPeerBufHdrSize(10bytes) + 1(iTruckSize) + 2(01 01) + iSmsCenterSize(9) + 2(big-endian iPduDataSize) + PDU_DATA
	{
		int iPeerBufferLen;
		uint8 *pPeer;
		uint16 iPeer;
		uint16 pdu_len;
		uint16 num_len;
		
		sms_addr_struct sms_addr;
		MexPhs_ConvertStringToSmsAddr(pSmsNumber, &sms_addr);
		num_len = strlen(pSmsNumber);
		if (pSmsNumber[0] == '+')
			num_len--;
		
		pdu_len = 2 + sms_addr.addr_length + 2 + 7 + 1 + iSmsContentLen * 2;
		iPeerBufferLen = 10 + 1 + 2 + 1 + pLocalPara->sc_addr.addr_length + 2 + pdu_len;
		
		pPeerBuffer = construct_peer_buff(iPeerBufferLen, 0, 0, 0);
		memset(pPeerBuffer, 0, iPeerBufferLen);
		{
			uint8 header[10] = {0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x09, 0x01};
			memcpy(pPeerBuffer, header, 10);
		}
		pPeerBuffer->pdu_len = pdu_len; // 2 bytes 0~1
		
		pPeer = (uint8 *) pPeerBuffer;
		// 10 iTruckSize
		pPeer[10] = iPeerBufferLen - 11;
		// 11~12 Unknown
		pPeer[11] = pPeer[12] = 0x01;
		
		// sms center
		iPeer = 13;
		memcpy(&pPeer[iPeer], &pLocalPara->sc_addr, pLocalPara->sc_addr.addr_length + 1);
		iPeer += pLocalPara->sc_addr.addr_length + 1;
		
		// pdu_len big_endian 2 bytes
		{
			uint16 *pPduLen = (uint16 *)&pPeer[iPeer];
			*pPduLen = mex_soc_htons(pdu_len);
		}
		iPeer += 2;
		
		// PDU DATA
		pPeer[iPeer++] = 0x24;
		pPeer[iPeer++] = num_len;
		memcpy(&pPeer[iPeer], sms_addr.addr_bcd, sms_addr.addr_length);
		iPeer += sms_addr.addr_length;
		
		// protocol type
		pPeer[iPeer++] = 0x00;
		
		// sms dcs
		pPeer[iPeer++] = 0x08;
		
		// time stamp
		{
			MYTIME dt;
			dt.nYear = pDt->year;
			dt.nMonth = pDt->month;
			dt.nDay = pDt->day;
			dt.nHour = pDt->hour;
			dt.nMin = pDt->minute;
			dt.nSec = pDt->second;
#if(MEX_MTK_CODE_VERSION >= 0x0952)
			srv_sms_mytime_to_scts((U8 *)&pPeer[iPeer], &dt);
#else
			mmi_frm_sms_convert_mytime((U8 *)&pPeer[iPeer], &dt);
#endif
			
		}
		iPeer += 7;
		
		// sms content len
		pPeer[iPeer] = iSmsContentLen * 2;
		iPeer += 1;
		
		// sms content
		{
			uint8 i = 0, j = 0;
			wchar_t *pDest = (wchar_t *)&pPeer[iPeer];
			
			for (i = 0; i < iSmsContentLen; i++)
				pDest[i] = mex_soc_htons(pSmsContent[j++]);
		}
	}
	
	// send message
	{
		MYQUEUE Message;
		
		Message.oslSrcId = mod_src;
		Message.oslDestId = mod_dest;
		Message.oslMsgId = MSG_ID_SMS_DELIVER;
		Message.oslSapId = MNSMS_SAP;
		Message.oslDataPtr = (void *)pLocalPara;
		Message.oslPeerBuffPtr = (void *)pPeerBuffer;
		
		OslMsgSendExtQueue(&Message);
		
		return true;
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	module_type mod_dest;
	uint8 iSmsContentLen; 
	
#ifdef MEX_DUAL_SIM
#if (MEX_SPRD_CODE_VERSION <= 0x09A1109)
    extern BLOCK_ID g_P_MN_2_ID;
#else    //10A
    extern BLOCK_ID g_P_MN_ID[2];
#endif
#endif
	
#ifdef MEX_THIRD_SIM
    extern BLOCK_ID g_P_MN_ID[3];
#endif
	
#ifdef MEX_QUAD_SIM
    extern BLOCK_ID g_P_MN_ID[4];
#endif
	
#define MEX_SM_RL_DATA_IND    (( P_MN << 8 ) | 1 )
	
	typedef struct MEX_PD_TI_REF_T_s {
		short  pd;
		short  ti;
	} MEX_PD_TI_REF_T;
	
	typedef struct MEX_SMS_ADDR_T_s {
		uint16  length;
		uint8  addr_arr_t[12];
	} MEX_SMS_ADDR_T;
	
	
	typedef struct MEX_L3_MSG_UNIT_T_s {
		uint16  length;
		uint8   l3_msg[251];
	} MEX_L3_MSG_UNIT_T;
	
	typedef struct {        
		uint16 SignalCode;
		uint16 SignalSize;
		void *  Pre;
		void *  Suc;
		uint32  Sender;
		
		MEX_PD_TI_REF_T  Param1;
		MEX_SMS_ADDR_T  Param2;
		MEX_L3_MSG_UNIT_T  Param3;
	} MEX_SM_RL_DATA_IND_T;
	
	MEX_SM_RL_DATA_IND_T *pSig = NULL;
	MEX_SMS_ADDR_T     arrSenderNumber;
	
	if((NULL == pSmsCenter)||(NULL == pSmsNumber)||(NULL == pSmsContent)||(NULL == pDt))
	{
        mex_log(MEX_LOG_PHS,"NewSms E: NULL parameter");
        return false;
	}
	
	switch (eSimId)
	{
	case E_MEX_SIM_1:
		mod_dest = P_MN;
		break;
#if defined(MEX_DUAL_SIM)
	case E_MEX_SIM_2:
#if (MEX_SPRD_CODE_VERSION <= 0x09A1109)
		mod_dest = g_P_MN_2_ID;
#else
        mod_dest = g_P_MN_ID[1];
#endif
		break;        
#endif        
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
    case E_MEX_SIM_2:
        mod_dest = g_P_MN_ID[1];
        break;
		
	case E_MEX_SIM_3:
        mod_dest = g_P_MN_ID[2];
		break;
#endif //MEX_THIRD_SIM
		
	default:
		mex_log(MEX_LOG_PHS, "NewSms E: sim:%d", eSimId);
		return false;
	}
	
	
	
	// check sc & number & content
	iSmsContentLen = mex_wcslen(pSmsContent);
	if (strlen(pSmsCenter) > 20)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_sc > 20");
		return false;
	}
	if (strlen(pSmsNumber) > 20)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_number > 20");
		return false;
	}
	if (iSmsContentLen > 70)
	{
		mex_log(MEX_LOG_PHS, "NewSms E: len_content > 70");
		return false;
	}
	
	SCI_CREATE_SIGNAL(pSig,MEX_SM_RL_DATA_IND,sizeof(MEX_SM_RL_DATA_IND_T),SCI_IdentifyThread());    /*lint !e668*/
	if (pSig == NULL)  /*lint !e774*/
	{
		mex_log(MEX_LOG_PHS, "NewSms E: no memory");
		return false;
	}
	
	
	{
		int32  iL3MsgOffset;
		int32  num_len;
		//send start up signal to start up the sim task
		
		num_len = strlen(pSmsNumber);
		if (pSmsNumber[0] == '+')
			num_len--;
		
		pSig->Param1.pd = 9;       /*lint !e413 */
		pSig->Param1.ti = 8;       /*lint !e413 */
		
		MexPhs_ConvertStringToSmsAddr(pSmsCenter, &pSig->Param2);        /*lint !e413 */// sms center number
		
		MexPhs_ConvertStringToSmsAddr(pSmsNumber, &arrSenderNumber); // sms center number
		
		pSig->Param3.l3_msg[0] = 0x4;       /*lint !e413 */
		pSig->Param3.l3_msg[1] = num_len;       /*lint !e413 */
		
		mex_memcpy(&pSig->Param3.l3_msg[2],arrSenderNumber.addr_arr_t,arrSenderNumber.length);       /*lint !e413 */
		iL3MsgOffset = arrSenderNumber.length + 2;
		
		// protocol type
		pSig->Param3.l3_msg[iL3MsgOffset++] = 0;       /*lint !e413 */
		
		// sms dcs
		pSig->Param3.l3_msg[iL3MsgOffset++] = 0x08;       /*lint !e413 */
		
		
		MexPhs_ConvertTime2SmsTime(&(pSig->Param3.l3_msg[iL3MsgOffset]),(Mex_DateTime *)pDt);       /*lint !e413 */
		iL3MsgOffset += 7;
		
		pSig->Param3.l3_msg[iL3MsgOffset++] = iSmsContentLen * 2;       /*lint !e413 */
		
		// sms content
		{
			uint8 i = 0, j = 0;
			wchar_t *pDest = (wchar_t *)&pSig->Param3.l3_msg[iL3MsgOffset];       /*lint !e413 */
			
			for (i = 0; i < iSmsContentLen; i++)
				pDest[i] = mex_soc_htons(pSmsContent[j++]);
		}   
		
		pSig->Param3.length = iL3MsgOffset + (iSmsContentLen * 2);       /*lint !e413 */
	}
	
	// send message
	{            
		SCI_SEND_SIGNAL((xSignalHeaderRec*)pSig,mod_dest);
		
		return true;
	}
#endif    
}

//========================================================== Simulate Sms End
bool MexPhs_SmsReady(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if(MEX_MTK_CODE_VERSION >= 0x1112)
    if((srv_sms_is_sms_ready() == MMI_FALSE) || (srv_sms_check_send_action_pending(SRV_SMS_BG_SEND)))
    {
        return false;
    }
    else
        return true;
#elif(MEX_MTK_CODE_VERSION >= 0x0952)
	if((srv_sms_is_sms_ready() == MMI_FALSE)|| srv_sms_is_send_action_busy())
		return false;
	else
		return true;
#else
	
	extern U16 mmi_frm_sms_get_sms_list_size(U16 type);
	
	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX) == MMI_FRM_SMS_INVALID_INDEX ||
		mmi_frm_sms_check_action_pending())
		return false;
	else
		return true;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	bool  bResult = false;
	
	bResult = MMISMS_IsSMSReady(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1));
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	bResult = bResult | MMISMS_IsSMSReady(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2));
#if defined(MEX_THIRD_SIM)  || defined(MEX_QUAD_SIM)     //支持三SIM卡
	bResult = bResult | MMISMS_IsSMSReady(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3));
#endif //MEX_THIRD_SIM
#endif
	
	
	return bResult;
#endif    
}

uint16 MexPhs_SmsConvertMsgBox(E_MEX_PHS_SMS_MSGBOX msgBox)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	uint16 type;
	
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	
	switch (msgBox)
	{
	case E_MPSM_INBOX:
		type = SRV_SMS_BOX_INBOX;
		break;
	case E_MPSM_OUTBOX:
		type = SRV_SMS_BOX_OUTBOX;
		break;
	case E_MPSM_DRAFTS:
		type = SRV_SMS_BOX_DRAFTS;
		break;
	case E_MPSM_UNSENT:
		
#ifdef __SRV_SMS_UNSENT_LIST__
		
		type = SRV_SMS_BOX_UNSENT;
#else
		mex_log(MEX_LOG_PHS, "PSCMB E: no-sup box:%d", msgBox);
		return SRV_SMS_BOX_RESERVED;
#endif /* __MMI_UNIFIED_MESSAGE__ */
		break;
		
	default:
		mex_log(MEX_LOG_PHS, "PSCMB E: box:%d", msgBox);
		return SRV_SMS_BOX_RESERVED;
	}
	
#else
	switch (msgBox)
	{
	case E_MPSM_INBOX:
		type = MMI_FRM_SMS_INBOX;
		break;
	case E_MPSM_OUTBOX:
		type = MMI_FRM_SMS_OUTBOX;
		break;
	case E_MPSM_DRAFTS:
		type = MMI_FRM_SMS_DRAFTS;
		break;
	case E_MPSM_UNSENT:
		
#ifdef __MMI_UNIFIED_MESSAGE__
		type = MMI_FRM_SMS_UNSENT;
#else
		mex_log(MEX_LOG_PHS, "PSCMB E: no-sup box:%d", msgBox);
		return MMI_FRM_SMS_MSGBOX_END;
#endif /* __MMI_UNIFIED_MESSAGE__ */
		break;
		
	default:
		mex_log(MEX_LOG_PHS, "PSCMB E: box:%d", msgBox);
		return MMI_FRM_SMS_MSGBOX_END;
	}
#endif
	return type;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	uint16 type;
	switch (msgBox)
	{
	case E_MPSM_INBOX:
		type = MMISMS_BOX_MT;
		break;
	case E_MPSM_OUTBOX:
		type = MMISMS_BOX_SENDSUCC;
		break;
	case E_MPSM_DRAFTS:
		type = MMISMS_BOX_NOSEND;
		break;
	case E_MPSM_UNSENT:
		type = MMISMS_BOX_SENDFAIL;
		break;
		
	default:
		mex_log(MEX_LOG_PHS, "PSCMB E: box:%d", msgBox);
		return MMISMS_BOX_NONE;
	}
	return type;
#endif
}

bool MexPhs_SmsCheckIndex(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index)
{
	uint16 type;
	uint16 size;
	type = MexPhs_SmsConvertMsgBox(msgBox);
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	if (type == SRV_SMS_BOX_RESERVED)
#else
		if (type == MMI_FRM_SMS_MSGBOX_END)
#endif
		{
			mex_log(MEX_LOG_PHS, "PSCI E: no-sup box:%d", msgBox);
			return false;
		}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
		if (type == MMISMS_BOX_NONE)
		{
			mex_log(MEX_LOG_PHS, "PSCI E: no-sup box:%d", msgBox);
			return false;
		}
#endif
		size = MexPhs_GetNumOfSms(msgBox);
		if (size == MEX_PHS_SMS_INDEX_INVALID ||
			index >= size)
		{
			mex_log(MEX_LOG_PHS, "PGSA E: idx:%d size:%d", index, size); 
			return false;
		}
		
		return true;
		
}

bool MexPhs_SmsIsUnread(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index)
{
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	if (msgBox == E_MPSM_INBOX)
	{
#if(MEX_MTK_CODE_VERSION >= 0x0952)
		U16 msg_id; 
		srv_sms_box_enum  msg_type;
		msg_type = MexPhs_SmsConvertMsgBox(msgBox);
		msg_id = srv_sms_get_msg_id(msg_type, index);
		
		if (srv_sms_get_msg_status(msg_id) & SRV_SMS_STATUS_UNREAD)
		{
			return true;
		}
#else
		uint16 status = mmi_frm_sms_get_sms_status(MMI_FRM_SMS_INBOX, index);
		
		if ((status & MMI_FRM_SMS_UNREAD) == MMI_FRM_SMS_UNREAD)
		{
			return true;
		}
#endif
	}
	
	return false;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	if (msgBox == E_MPSM_INBOX)
	{       
		MMISMS_ORDER_ID_T  pFirstUnreadSms;
		MMISMS_ORDER_ID_T  pGivenSms;
		uint16             uiUnreadSmsCount;
		
		//获取未读短信总数
		uiUnreadSmsCount = MMISMS_GetTobeReadMsgCount(
			MN_DUAL_SYS_MAX,
			MMISMS_TYPE_SMS);
		
		if(uiUnreadSmsCount == 0)
		{
			return false;
		}
		
		//获取指定短信的信息
		pGivenSms = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT ,index); 
		if(pGivenSms == NULL)
		{
			return false;
		}
		
		//判断当前读取的短信的类型是否为SMS
		if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
		{       
			return false;
		}    
		
		//获取第一条未读信息
		pFirstUnreadSms = MMISMS_GetFirstUnreadSmsId();
		while(pFirstUnreadSms != NULL)       
		{
			if(pFirstUnreadSms == pGivenSms)
			{
				return true;           
			}
			
			pFirstUnreadSms = pFirstUnreadSms->next_ptr;
		}
		return false;
	}
	
	return false;
#endif
}

#if defined(MOTION_SENSOR_SUPPORT) || defined(MOTION_SENSOR_TYPE)
#ifndef ABS
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif

#define MOTIONSENSOR_TIMES 100
typedef struct  
{
    int32							m_iAppId;
    MexLib_MotionSensor_CallBack	m_callback;
	E_Mex_MotionSensor_Dir			dir_result;
	int16                           x_centerpoint;
	int16                           y_centerpoint;
	int16                           MotionSensitivity_value;
}sMotionSensor_App_Info;

static int32 g_SetTimer = 0;
static int8 g_MotionSensor_Dir = E_MMSD_IDLE;
static sMotionSensor_App_Info sMotionSensorAppInfo={0};
uint16 x_adc = 0;
uint16 y_adc = 0; 
uint16 z_adc = 0;

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
static uint32 g_GSensor_win_id = 0;

void mex_GSensor_CallBack(void)
{
	MMK_GSENSOR_STATUS_E status;
	MMK_GSENSOR_VECTOR_T dir = {0};
	//int32 sensitivity_value = sMotionSensorAppInfo.MotionSensitivity_value;	//pclint//s = a*t*t
	int temp_x = 0;
	int temp_y = 0;
	int temp_z = 0;
	
	status = MMK_GetGsensorStatus();
	
	if( status == MMK_GSENSOR_STATUS_SHAKE || status == MMK_GSENSOR_STATUS_NONE )
	{
		//SCI_TRACE_LOW:"crazy Gsesor CB status none"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_2738_112_2_18_2_36_41_8,(uint8*)"");
		g_MotionSensor_Dir = E_MMSD_IDLE;
		sMotionSensorAppInfo.m_callback((E_Mex_MotionSensor_Dir)g_MotionSensor_Dir);
		return;		
	}
	MMK_GetGsensorVector(&dir);
	
	temp_x = ABS(dir.x) / 100;
	temp_y = ABS(dir.y) / 100;
	temp_z = ABS(dir.z) / 100;
	
	if ( temp_x < 2 && temp_y < 2 && temp_z > 9 )	//水平
		g_MotionSensor_Dir = E_MMSD_IDLE;
	else
	{
		if ( ABS(dir.x) > ABS(dir.y) )	//x轴的移动，left or right
		{
			if ( dir.x > 0 )
				g_MotionSensor_Dir = E_MMSD_RIGHT;
			else
				g_MotionSensor_Dir = E_MMSD_LEFT;
		}
		else  //up or down
		{
			if ( dir.y > 0 )
				g_MotionSensor_Dir = E_MMSD_UP;
			else 
				g_MotionSensor_Dir = E_MMSD_DOWN;
		}
	}
	//SCI_TRACE_LOW:"crazy Gsesor ax = %d,ay = %d, az = %d,r = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_2768_112_2_18_2_36_41_9,(uint8*)"dddd",temp_x, temp_y, temp_z, g_MotionSensor_Dir);
	sMotionSensorAppInfo.m_callback((E_Mex_MotionSensor_Dir)g_MotionSensor_Dir);
	return;		
}

#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MotionSensor_Action(int16 *x_adc, int16 *y_adc, int16 *z_adc)
{
	int16 x_temp = 0;
	int16 y_temp = 0;
	int16 x_standard = sMotionSensorAppInfo.x_centerpoint;
	int16 y_standard= sMotionSensorAppInfo.y_centerpoint;
	int16 iMotionSensitivity = sMotionSensorAppInfo.MotionSensitivity_value;
	x_temp = (int16)*x_adc;
	y_temp = (int16)*y_adc;
	mex_log(MEX_LOG_PHS,"x_temp==%d, y_temp==%d",x_temp,y_temp);
	if (ABS(x_temp - x_standard) < iMotionSensitivity)
	{
		if (ABS(y_temp - y_standard) < iMotionSensitivity)
		{
			return;
		}	
	}
	if (ABS(x_temp - x_standard) > ABS(y_temp - y_standard))
	{
		if (x_temp > x_standard)
		{
			g_MotionSensor_Dir = E_MMSD_RIGHT;
			mex_log(MEX_LOG_PHS,"go right!!!");
		}
		else
		{
			g_MotionSensor_Dir = E_MMSD_LEFT;
			mex_log(MEX_LOG_PHS,"go left!!!");
		}		
	}
	else if (ABS(x_temp - x_standard) < ABS(y_temp - y_standard))
	{
		if (y_temp > y_standard)
		{
			g_MotionSensor_Dir = E_MMSD_DOWN;
			mex_log(MEX_LOG_PHS,"go down!!!");
		}
		else
		{
			g_MotionSensor_Dir = E_MMSD_UP;
			mex_log(MEX_LOG_PHS,"go up!!!!!");
		}
	}
	else
	{
		g_MotionSensor_Dir = E_MMSD_IDLE;
	}
	sMotionSensorAppInfo.m_callback(g_MotionSensor_Dir);
}
#endif
void mex_MotionSensor_CallBack(void)
{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	mex_GSensor_CallBack();
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	mma_sensor_get_data(&x_adc, &y_adc, &z_adc);//取得坐标
	MotionSensor_Action(&x_adc, &y_adc, &z_adc);
#endif
}
#endif

void MexPhs_MotionSensor_Start(MexLib_MotionSensor_CallBack callback,int16 x_ponit,int16 y_point,int16 motionSensitivity)
{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if	defined(MOTION_SENSOR_TYPE)
	MMI_HANDLE_T win_handle = 0;
	bool ret;
    win_handle = MMK_GetFocusWinHandle();
	//SCI_TRACE_LOW:"MexPhs_MotionSensor_Start Gsensor win_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_2842_112_2_18_2_36_41_10,(uint8*)"d",win_handle);
	g_GSensor_win_id = win_handle;
	ret = MMK_OpenGsensor(g_GSensor_win_id);
	//SCI_TRACE_LOW:"MexPhs_MotionSensor_Start GSensor ret = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_2845_112_2_18_2_36_41_11,(uint8*)"d",ret);
	sMotionSensorAppInfo.m_callback = callback;
	sMotionSensorAppInfo.x_centerpoint = x_ponit;
	sMotionSensorAppInfo.y_centerpoint = y_point;
	sMotionSensorAppInfo.MotionSensitivity_value = motionSensitivity;
	g_SetTimer = mex_StartSystemTimer(30, mex_MotionSensor_CallBack);
#endif
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(MOTION_SENSOR_SUPPORT)
	mma_sensor_init(); //重力感应初始化
	
	sMotionSensorAppInfo.m_callback = callback;
	sMotionSensorAppInfo.x_centerpoint = x_ponit;
	sMotionSensorAppInfo.y_centerpoint = y_point;
	sMotionSensorAppInfo.MotionSensitivity_value = motionSensitivity;
	g_SetTimer = mex_StartSystemTimer(MOTIONSENSOR_TIMES, mex_MotionSensor_CallBack);
#endif
#endif
}
void MexPhs_MotionSensor_Stop(void)
{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifdef MOTION_SENSOR_TYPE
	MMK_CloseGsensor(g_GSensor_win_id);
	mex_KillSystemTimer(g_SetTimer);
#endif
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if defined(MOTION_SENSOR_SUPPORT)
	mex_KillSystemTimer(g_SetTimer);
#endif
#endif
}

uint16 MexPhs_GetNumOfSms(E_MEX_PHS_SMS_MSGBOX msgBox)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x0952)
	U16 type;
	U16 size;
	
	type = MexPhs_SmsConvertMsgBox(msgBox);
	if (type == SRV_SMS_BOX_RESERVED)
		return MEX_PHS_SMS_INDEX_INVALID;
	
	size = srv_sms_get_list_size(type);
#if (MEX_MTK_CODE_VERSION < 0x0952)
	if (size == MMI_FRM_SMS_INVALID_INDEX)
		return MEX_PHS_SMS_INDEX_INVALID;
	else
#endif
		return size;
#else
	extern U16 mmi_frm_sms_get_sms_list_size(U16 type);
	U16 type;
	U16 size;
	
	type = MexPhs_SmsConvertMsgBox(msgBox);
	if (type == MMI_FRM_SMS_MSGBOX_END)
		return MEX_PHS_SMS_INDEX_INVALID;
	
	size = mmi_frm_sms_get_sms_list_size(type);
	if (size == MMI_FRM_SMS_INVALID_INDEX)
		return MEX_PHS_SMS_INDEX_INVALID;
	else
		return size;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_BOX_TYPE_E  sms_box_type;     
	uint16 size;
	
	sms_box_type = (MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox);
	if (sms_box_type == MMISMS_BOX_NONE)
		return MEX_PHS_SMS_INDEX_INVALID;
	
	size = MMISMS_GetBoxTotalNumberFromOrder(sms_box_type);
	return size;
#endif 
	
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
static void MexPhs_ReadSmsContentCB(srv_sms_callback_struct* callback_data)
{
	srv_sms_read_msg_cb_struct *read_msg_cb_data;
	srv_sms_msg_data_struct *msg_data;
	int32 appId = mex_GetCurrentApp();
	
	read_msg_cb_data = (srv_sms_read_msg_cb_struct*)callback_data->action_data;
	msg_data = read_msg_cb_data->msg_data;
	
	if (g_mex_readsms_context.callback == NULL)
	{
		g_mex_readsms_context.processing = false;
		return;
	}
	
	
	if (callback_data->result == MMI_FALSE)
	{
		mex_SetCurrentApp(g_mex_readsms_context.appId);
		g_mex_readsms_context.callback(
			g_mex_readsms_context.msgBox, 
			g_mex_readsms_context.index,
			NULL,
			0,
			false);
		mex_SetCurrentApp(appId);
		g_mex_readsms_context.processing = false;
		g_mex_readsms_context.callback = NULL;
		return;
	}
	else
	{
		EMSData* pEms =(EMSData*)(msg_data->content_ems);
		
		mex_log(MEX_LOG_PHS, "PRSCB len:%d", pEms->textLength);
		
		mex_SetCurrentApp(g_mex_readsms_context.appId);
		g_mex_readsms_context.callback(
			g_mex_readsms_context.msgBox, 
			g_mex_readsms_context.index,
			(const wchar_t *)pEms->textBuffer,
			pEms->textLength,
			true);
		mex_SetCurrentApp(appId);
		g_mex_readsms_context.processing = false;
		g_mex_readsms_context.callback = NULL;
	}
#if(MEX_MTK_CODE_VERSION >= 0x1112)
	srv_sms_free_ems_data(msg_data->content_ems);
#endif
	OslMfree(msg_data); // Fix Bug #8591 
}

#else
void MexPhs_ReadSmsContentCB(void *data, module_type mod, U16 result)
{
	U16 type = *(U16*) data;
	EMSData *pEms;
	MMI_BOOL is_compress = MMI_FALSE;
	U8 i = 0;
	int32 appId = mex_GetCurrentApp();
	
	if (g_mex_readsms_context.callback == NULL)
	{
		g_mex_readsms_context.processing = false;
		return;
	}
	
	mex_log(MEX_LOG_PHS, "PRSCB data:%d", data);
	mex_log(MEX_LOG_PHS, "PRSCB mod:%d", mod);
	mex_log(MEX_LOG_PHS, "PRSCB res:%d", result);
	
#if (MEX_MTK_CODE_VERSION < 0x1112)
	GetEMSDataForView(&pEms, 0);
#else
	{
		if (g_sms_cntx.view_ems_data == NULL)
		{
			g_sms_cntx.view_ems_data = srv_sms_malloc_ems_data();
		}
		if (&pEms)
		{
			pEms = g_sms_cntx.view_ems_data;
		}
	}
#endif
	/* Compress dcs SMS is not supported. */
	for (i = 0; i < PendingSaveSendData.totalSegments; i++)
	{
		ems_decode_dcs(PendingSaveSendData.ori_dcs[i], &is_compress);
		if (is_compress == TRUE || result != MMI_FRM_SMS_OK)
		{
			mex_SetCurrentApp(g_mex_readsms_context.appId);
			g_mex_readsms_context.callback(
				g_mex_readsms_context.msgBox, 
				g_mex_readsms_context.index,
				NULL,
				0,
				false);
			mex_SetCurrentApp(appId);
			g_mex_readsms_context.processing = false;
			g_mex_readsms_context.callback = NULL;
			return;
		}
	}    
	
	mex_log(MEX_LOG_PHS, "PRSCB len:%d", pEms->textLength);
	if (pEms->textLength > 0)
	{
		mex_log(MEX_LOG_PHS, "PRSCB content:%s", mex_twcs2str((const wchar_t *) pEms->textBuffer));
	}
	
	mex_SetCurrentApp(g_mex_readsms_context.appId);
	g_mex_readsms_context.callback(
		g_mex_readsms_context.msgBox, 
		g_mex_readsms_context.index,
		(const wchar_t *)pEms->textBuffer,
		pEms->textLength,
		true);
	mex_SetCurrentApp(appId);
	g_mex_readsms_context.processing = false;
	g_mex_readsms_context.callback = NULL;   
}
#endif
#endif
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void MexPhs_ReadSmsContentCB(void *data, module_type mod, int16 result)
{
	MMISMS_CONTENT_T *pSmsContent = 
		(MMISMS_CONTENT_T *)data;
    uint16 *msgdata_ptr = PNULL;
	int32 appId = mex_GetCurrentApp();
	if (g_mex_readsms_context.callback == NULL)
	{
		g_mex_readsms_context.processing = false;
		return;
	}

        msgdata_ptr = SCI_ALLOCAZ(((MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN)>>1)*sizeof(wchar));

        if (PNULL == msgdata_ptr)
        {
            return;
        }
	if(result)
	{            
		mex_log(MEX_LOG_PHS, "PRSCB data:%d", data);
		mex_log(MEX_LOG_PHS, "PRSCB res:%d", result);
		
		mex_log(MEX_LOG_PHS, "PRSCB len:%d", pSmsContent->length);
		if (pSmsContent->length > 0)
		{
			mex_memcpy((char*)msgdata_ptr,(char*)pSmsContent->data,MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN);
			if(pSmsContent->alphabet == MN_SMS_UCS2_ALPHABET)
			{
				mex_log(MEX_LOG_PHS, "PRSCB content:%s", mex_twcs2str((const wchar_t *)msgdata_ptr));
			}
			else
			{
				mex_log(MEX_LOG_PHS, "PRSCB content:%s", (char*)msgdata_ptr);
			}
		}
	}
	
	mex_SetCurrentApp(g_mex_readsms_context.appId);
	
	if(result)
	{
		if(pSmsContent->alphabet == MN_SMS_UCS2_ALPHABET)
		{
			g_mex_readsms_context.callback(
				g_mex_readsms_context.msgBox, 
				g_mex_readsms_context.index,
				(const wchar_t *)msgdata_ptr,
				pSmsContent->length,
				true);
		}
		else
		{
			
			wchar_t   *pContentUcs;
			
			pContentUcs = mex_malloc((pSmsContent->length + 1) * 2);       /*lint !e611 */
			if(pContentUcs == NULL)
			{
				g_mex_readsms_context.callback(
					g_mex_readsms_context.msgBox, 
					g_mex_readsms_context.index,
					NULL,
					0,
					false);
			}
			else
			{
				mex_str2nwcs(
					pContentUcs,
					((pSmsContent->length + 1) * 2),
					(char*)msgdata_ptr);
				
				g_mex_readsms_context.callback(
					g_mex_readsms_context.msgBox, 
					g_mex_readsms_context.index,
					pContentUcs,
					(pSmsContent->length * 2),
					true);
				
				mex_free(pContentUcs);       /*lint !e611 */
			}
		}
	}
	else
	{
		g_mex_readsms_context.callback(
			g_mex_readsms_context.msgBox, 
			g_mex_readsms_context.index,
			NULL,
			0,
			false);
	}
	
	mex_SetCurrentApp(appId);
	g_mex_readsms_context.processing = false;
	g_mex_readsms_context.callback = NULL;
	
	pSmsContent->length = 0xFEDC;  //通知系统是mex 平台发送的读请求

    SCI_FREE(msgdata_ptr);
}
#endif


bool MexPhs_ReadSmsContent(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Read_Sms_CB callback)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
#if(MEX_MTK_CODE_VERSION < 0x1112)
	extern EMSData *GetEMSDataForView(EMSData **p, U8 force);
#endif
	U16 msg_id;
	srv_sms_msg_data_struct* msg_data;
	
#else
	extern void mmi_frm_sms_read_sms(PsFuncPtrU16 callback,module_type mod_src,U16 type,U16 index,MMI_BOOL change_status);
#endif
	if(NULL == callback)
	{
        mex_log(MEX_LOG_PHS, "PRS E: callback can't be NULL");
        return false;
	}
	
	if (!MexPhs_SmsReady() || g_mex_readsms_context.processing)
	{
		mex_log(MEX_LOG_PHS, "PRS E: busy");
		return false;
	}
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	g_mex_readsms_context.processing = true;
	g_mex_readsms_context.appId = mex_GetCurrentApp();
	g_mex_readsms_context.msgBox = msgBox;
	g_mex_readsms_context.index = index;
	g_mex_readsms_context.callback = callback;
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	
	msg_id =  srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
	
	if(msg_id == 0xffff)
		return false;
	
	msg_data = (srv_sms_msg_data_struct*)OslMalloc(sizeof(srv_sms_msg_data_struct));
	msg_data->para_flag = SRV_SMS_PARA_CONTENT_EMS;
#if (MEX_MTK_CODE_VERSION < 0x1112)    
	msg_data->content_ems = (EMSData*)GetEMSDataForView(NULL, 1);
#else
	if (msg_data->content_ems == NULL)
	{
		msg_data->content_ems = srv_sms_malloc_ems_data();
	}
	
	srv_sms_reset_ems_data((void*)msg_data->content_ems);
	
#endif
#ifdef __MMI_MESSAGES_SMS_EMAIL__
	msg_data->para_flag |= SRV_SMS_PARA_PID;
#endif /* __MMI_MESSAGES_SMS_EMAIL__ */
	srv_sms_read_msg(
		msg_id,
		MMI_TRUE,
		msg_data,
		MexPhs_ReadSmsContentCB,
		(void*)0);
	
#else
	mmi_frm_sms_read_sms(MexPhs_ReadSmsContentCB, MOD_MMI, MexPhs_SmsConvertMsgBox(msgBox), index, MMI_TRUE);
#endif
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_ORDER_ID_T pGivenSms;
	MMISMS_BOX_TYPE_E      msgbox_type;
	
	//assert(callback != NULL);
	
	if (!MexPhs_SmsReady() || g_mex_readsms_context.processing)
	{
		mex_log(MEX_LOG_PHS, "PRS E: busy");
		return false;
	}
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	g_mex_readsms_context.processing = true;
	g_mex_readsms_context.appId = mex_GetCurrentApp();
	g_mex_readsms_context.msgBox = msgBox;
	g_mex_readsms_context.index = index;
	g_mex_readsms_context.callback = callback;
	
	
	
	//获取指定短信的信息
	msgbox_type = (MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox);
	
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList(msgbox_type ,index); 
	
	if(pGivenSms == NULL)
	{       
		return false;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	
	//准备全局变量
	MMISMS_ClearOperInfo();
	MMISMS_ReadyReadSms();
	MMISMS_SetCurOperationOrderId(msgbox_type, index); 
	MMISMS_SetCurType(msgbox_type);
	
	//开始读取
	MMISMS_AppReadSms(pGivenSms);        
	
	return true;
#endif    
}

void MexConfig_ConvertBCD2Ansi(int8 *ansi_str,int32 max_len,int8 *bcd_str,int32 bcd_len, int32 offset);

char *MexPhs_GetSmsAddr(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	U16 msg_id;
	srv_sms_msg_node_struct *pNode;
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return NULL;
	
	msg_id =  srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
	if(msg_id == 0xffff)
		return NULL;
	pNode = srv_sms_get_msg_node(msg_id);
	
	return pNode->number;
#else
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return NULL;
	return (char *)mmi_frm_sms_get_sms_address(MexPhs_SmsConvertMsgBox(msgBox), index);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_ORDER_ID_T pGivenSms;
	static char              number[21];
	
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return NULL;
	
	//获取指定短信的信息
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList((MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox) ,index); 
	
	if(pGivenSms == NULL)
	{       
		return NULL;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	
	MexConfig_ConvertBCD2Ansi(number,21,
		(int8*)pGivenSms->number.bcd_number.number,
		pGivenSms->number.bcd_number.number_len,
		0);
	
	//获取指定短信的信息
	return (char *)number;  /*lint !e604*/
#endif    
}

bool MexPhs_GetSmsDateTime(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_DateTime *pDt)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	U16 msg_id;
	MYTIME time_stamp;
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	msg_id =  srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
	if(msg_id == 0xffff)
		return false;
	
	srv_sms_get_msg_timestamp(msg_id, &time_stamp);
	pDt->year = time_stamp.nYear;
	pDt->month = time_stamp.nMonth;
	pDt->day = time_stamp.nDay;
	pDt->hour = time_stamp.nHour;
	pDt->minute = time_stamp.nMin;
	pDt->second = time_stamp.nSec;
	
	if (pDt->month == 0) // 0 -> invalid month
		memset(pDt, 0, sizeof(Mex_DateTime));
	return true;
	
#else
	U8 *pTimeStamp = NULL;
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	pTimeStamp = (U8 *)mmi_frm_sms_get_sms_timestamp(MexPhs_SmsConvertMsgBox(msgBox), index);
	
	pDt->year = pTimeStamp[0] + 2000;
	if (pDt->year > 2090)
		pDt->year -= 100;
	pDt->month = pTimeStamp[1];
	pDt->day = pTimeStamp[2];
	pDt->hour = pTimeStamp[3];
	pDt->minute = pTimeStamp[4];
	pDt->second = pTimeStamp[5];
	
	if (pDt->month == 0) // 0 -> invalid month
		memset(pDt, 0, sizeof(Mex_DateTime));
	return true;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_ORDER_ID_T pGivenSms;
	MMI_TM_T mmi_time_stamp = {0};
	
	if (!MexPhs_SmsReady())
	{
		mex_log(MEX_LOG_PHS, "PRS E: busy");
		return false;
	}
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	//获取指定短信的信息
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList((MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox) ,index); 
	
	if(pGivenSms == NULL)
	{       
		return false;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	mmi_time_stamp = MMIAPICOM_Second2Tm(pGivenSms->time);
	pDt->year = mmi_time_stamp.tm_year;
	if (pDt->year > 2090)
		pDt->year -= 100;
	pDt->month	= mmi_time_stamp.tm_mon;
	pDt->day	= mmi_time_stamp.tm_mday;
	pDt->hour	= mmi_time_stamp.tm_hour;
	pDt->minute = mmi_time_stamp.tm_min;
	pDt->second = mmi_time_stamp.tm_sec;
	if (pDt->month == 0) // 0 -> invalid month
	{
		memset(pDt, 0, sizeof(Mex_DateTime));
		return false;
	}
	
	return true;
#endif        
}

bool MexPhs_GetSmsCache(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, wchar_t *pContent, uint16 iContentMemLen)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	if(NULL == pContent)
	{
        return false;
	}
	
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	{
#ifdef __SRV_SMS_MSG_INFO_CONTENT__
		U16 msg_id;
		srv_sms_msg_node_struct *pNode;
		if (!MexPhs_SmsCheckIndex(msgBox, index))
			return false;
		
		msg_id =  srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
		if(msg_id == 0xffff)
			return false;
		pNode = srv_sms_get_msg_node(msg_id);
		
		if(mex_wcslen(pNode->content) < iContentMemLen/2)
			iContentMemLen = mex_wcslen(pNode->content) * 2;
		
		memcpy(pContent, pNode->content, iContentMemLen);
		
		return true;
#else
		pContent[0] = L'\0';
		return false;
#endif
	}
#else
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
#ifdef __MMI_UNIFIED_MESSAGE__
#ifndef __MMI_MSG_UM_LOW_MEMORY_COST__
	extern kal_bool mmi_frm_sms_get_content(
		kal_uint16 msg_box_type,
		kal_uint16 msg_list_index,
		kal_wchar *content,
		kal_uint8 max_subject_len);
	return mmi_frm_sms_get_content(MexPhs_SmsConvertMsgBox(msgBox), index, pContent, iContentMemLen / 2);
#endif /* __MMI_MSG_UM_LOW_MEMORY_COST__ */
#endif /* __MMI_UNIFIED_MESSAGE__ */ 
	
	pContent[0] = L'\0';
	return false;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_ORDER_ID_T pGivenSms;
	if(NULL == pContent)
	{
        return false;
	}
	
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList((MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox) ,index); 
	
	if(pGivenSms == NULL)
	{       
		return false;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	
	
	mex_memset(pContent, 0, iContentMemLen);
	
	mex_wcsncpy(
		(wchar_t *) pContent,
		(const wchar_t *)pGivenSms->display_content.content,
		(iContentMemLen/2 - 1));
	
	return false;
#endif  
}

E_MEX_Sim_Card_ID MexPhs_GetSmsSimId(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return E_MEX_SIM_NONE;
	
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	
#ifdef __MMI_DUAL_SIM_MASTER__
	{
		U16 msg_id;
		srv_sms_msg_node_struct *pNode;
		if (!MexPhs_SmsCheckIndex(msgBox, index))
			return E_MEX_SIM_NONE;
		
		msg_id =  srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
		if(msg_id == 0xffff)
			return E_MEX_SIM_NONE;
		pNode = srv_sms_get_msg_node(msg_id);
		
		if(SRV_SMS_SIM_1 == pNode->sim_id)
			return E_MEX_SIM_1;
		else if(SRV_SMS_SIM_2 == pNode->sim_id)
			return E_MEX_SIM_2;
#ifdef __MET_MEX_SIM_PLUS__
#if (MMI_MAX_SIM_NUM >= 3)
		else if(SRV_SMS_SIM_3 == pNode->sim_id)
			return E_MEX_SIM_3;
#endif
#if (MMI_MAX_SIM_NUM >= 4)
		else if(SRV_SMS_SIM_4 == pNode->sim_id)
			return E_MEX_SIM_4;
#endif
#endif/*__MET_MEX_SIM_PLUS__*/
		else
			return E_MEX_SIM_NONE;
	}
#else
	return E_MEX_SIM_1;
#endif /* __MMI_DUAL_SIM_MASTER__ */
	
#else
	
#ifdef __MMI_DUAL_SIM_MASTER__
	if (mmi_frm_sms_record_is_slave_sms(MexPhs_SmsConvertMsgBox(msgBox), index))
		return E_MEX_SIM_2;
	else
#endif /* __MMI_DUAL_SIM_MASTER__ */        
		return E_MEX_SIM_1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMISMS_ORDER_ID_T pGivenSms;
	
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return E_MEX_SIM_NONE;
	
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList((MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox) ,index); 
	if(pGivenSms == NULL)
	{       
		return false;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	
	if(pGivenSms->flag.dual_sys == (uint32)MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1))
	{
		
		return E_MEX_SIM_1;
	}
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)    
	else if(pGivenSms->flag.dual_sys == (uint32)MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2))
	{
		return E_MEX_SIM_2;
	}
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
	else if(pGivenSms->flag.dual_sys == (uint32)MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3))
	{
		return E_MEX_SIM_3;
	}
#endif //MEX_THIRD_SIM
#endif    
	
	
	return E_MEX_SIM_NONE;
#endif    
}
void MexPhs_CancelSmsCB(void)
{
	int32 appId = mex_GetCurrentApp();
	
	if (appId == g_mex_delsms_context.m_iAppId)
	{
		g_mex_delsms_context.m_bProcessing = false;
		g_mex_delsms_context.m_pCallBack = NULL;
	}
	if (appId == g_mex_readsms_context.appId)
	{
		g_mex_readsms_context.processing = false;
		g_mex_readsms_context.callback = NULL;
	}
	if (appId == g_mex_savesms_context.appId)
	{
		g_mex_savesms_context.processing = false;
		g_mex_savesms_context.callback = NULL;
	}
}

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)

/*****************************************************************************
* FUNCTION
*  MexPhs_SaveSmsCB
* DESCRIPTION
*  Save SMS msg response
* PARAMETERS
*  status      [?]         
*  mod         [IN]        
*  result      [IN]        
* RETURNS
*  void
*****************************************************************************/
void MexPhs_SaveSmsCB(srv_sms_callback_struct* callback_data)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	MMI_BOOL result = callback_data->result;
	bool success;
	int32 appId = mex_GetCurrentApp();
	
	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if (g_mex_savesms_context.callback == NULL)
	{
		g_mex_savesms_context.processing = false;
		return;
	}
	
	mex_log(MEX_LOG_PHS, "PSSCB res:%d", result);
	
	if (result == MMI_TRUE)
		success = true;
	else
		success = false;
	
	mex_SetCurrentApp(g_mex_savesms_context.appId);
	g_mex_savesms_context.callback(
		g_mex_savesms_context.msgBox, 
		g_mex_savesms_context.index, 
		success);
	mex_SetCurrentApp(appId);
	
	g_mex_savesms_context.processing = false;
	g_mex_savesms_context.callback = NULL;
	
}

#else
void MexPhs_SaveSmsCB(void *data, module_type mod, U16 result)
{
	bool success;
	int32 appId = mex_GetCurrentApp();
	if (g_mex_savesms_context.callback == NULL)
	{
		g_mex_savesms_context.processing = false;
		return;
	}
	
	mex_log(MEX_LOG_PHS, "PSSCB data:%d", data);
	mex_log(MEX_LOG_PHS, "PSSCB mod:%d", mod);
	mex_log(MEX_LOG_PHS, "PSSCB res:%d", result);
	
	if (result == MMI_FRM_SMS_OK)
		success = true;
	else
		success = false;
	
	mex_SetCurrentApp(g_mex_savesms_context.appId);
	g_mex_savesms_context.callback(
		g_mex_savesms_context.msgBox, 
		g_mex_savesms_context.index, 
		success);
	mex_SetCurrentApp(appId);
	
	g_mex_savesms_context.processing = false;
	g_mex_savesms_context.callback = NULL;
}
#endif
#endif
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void MexPhs_SaveSmsCB(void *data, module_type mod, int16 result)
{
	bool success;
	int32 appId;
	
	if (g_mex_savesms_context.callback == NULL)
	{
		g_mex_savesms_context.processing = false;
		return;
	}
	
	mex_log(MEX_LOG_PHS, "PSSCB data:%d", data);
	mex_log(MEX_LOG_PHS, "PSSCB mod:%d", mod);
	mex_log(MEX_LOG_PHS, "PSSCB res:%d", result);
	
	if (MN_SMS_OPERATE_SUCCESS == result)
		success = true;
	else
		success = false;
	
	appId = mex_GetCurrentApp();
	mex_SetCurrentApp(g_mex_savesms_context.appId);
	g_mex_savesms_context.callback(
		g_mex_savesms_context.msgBox, 
		g_mex_savesms_context.index, 
		success);
	mex_SetCurrentApp(appId);
	
	g_mex_savesms_context.processing = false;
	g_mex_savesms_context.callback = NULL;    
	
}
#endif
bool MexPhs_SaveSms(E_MEX_PHS_SMS_MSGBOX msgBox, E_MEX_Sim_Card_ID simId, const wchar_t *pContent, const char *pAddr, Mex_Phs_Save_Sms_CB callback)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	SMS_HANDLE save_handle;
	uint8 status;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	
	if(NULL == callback)
	{
		mex_log(MEX_LOG_PHS, "PSS E: NULL callback");
        return false;
	}
	
	if (!MexPhs_SmsReady() || g_mex_savesms_context.processing)
	{
		mex_log(MEX_LOG_PHS, "PSS E: busy");
		return false;
	}
	
	// check msg box type <= mmi_frm_sms_move_sms_to_otherbox
	switch (msgBox)
	{
	case E_MPSM_OUTBOX:
		status = SRV_SMS_STATUS_SENT;
		break;
	case E_MPSM_DRAFTS:
		
#ifdef __MMI_MESSAGES_DRAFT_BOX__
		status = SRV_SMS_STATUS_DRAFT;
		break;
#endif /* __MMI_MESSAGES_DRAFT_BOX__ */
		
	case E_MPSM_UNSENT:
		
#ifdef __SRV_SMS_UNSENT_LIST__
		status = SRV_SMS_STATUS_UNSENT;
		break;
#endif /* __SRV_SMS_UNSENT_LIST__ */
		
	case E_MPSM_INBOX:
	default:
		mex_log(MEX_LOG_PHS, "PSS E: no-sup box:%d", msgBox);
		return false;
	}
	
	save_handle = srv_sms_get_save_handle();
	
#ifdef __MMI_DUAL_SIM_MASTER__
	if(simId == E_MEX_SIM_1)
	{
		srv_sms_set_sim_id(save_handle, SRV_SMS_SIM_1);
	}
#ifdef __MET_MEX_SIM_PLUS__
#if (MMI_MAX_SIM_NUM >= 3)
	else if(simId == E_MEX_SIM_3)
	{
		srv_sms_set_sim_id(save_handle, SRV_SMS_SIM_3);
	}
#endif 
#if (MMI_MAX_SIM_NUM >= 4)
	else if(simId == E_MEX_SIM_4)
	{
		srv_sms_set_sim_id(save_handle, SRV_SMS_SIM_4);
	}
#endif 
#endif/*__MET_MEX_SIM_PLUS__*/
	else
	{
		srv_sms_set_sim_id(save_handle, SRV_SMS_SIM_2);
	}
#endif /* __MMI_DUAL_SIM_MASTER__ */
	
	if(pAddr)
	{
		S8 addr[(SRV_SMS_MAX_ADDR_LEN + 1) *ENCODING_LENGTH];
		U16 addr_len;
		
		addr_len = mmi_asc_n_to_ucs2((S8*)addr, (S8*)pAddr, SRV_SMS_MAX_ADDR_LEN);
		addr[addr_len] = '\0';
		addr[addr_len + 1] = '\0';
		
		srv_sms_set_address(save_handle, (S8*)addr);
	}
	
	// init savesms context
	g_mex_savesms_context.processing = true;
	g_mex_savesms_context.msgBox = msgBox;
	g_mex_savesms_context.index = MEX_PHS_SMS_INDEX_INVALID;
	g_mex_savesms_context.callback = callback;
	g_mex_savesms_context.appId = mex_GetCurrentApp();
	
	srv_sms_set_status(save_handle, status);
	
	srv_sms_set_content(save_handle, (S8*)pContent,  mex_wcslen(pContent) * ENCODING_LENGTH);
	
	srv_sms_set_content_dcs(save_handle, SRV_SMS_DCS_UCS2);
	
	srv_sms_save_msg(save_handle, MexPhs_SaveSmsCB, NULL);
	
	return true;
	
#else
	extern void mmi_frm_sms_save_sms(PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct *sendData);
	uint8 status;
	mmi_frm_sms_send_struct *sendData = NULL;
	if(NULL == callback)
	{
		mex_log(MEX_LOG_PHS, "PSS E: NULL callback");
        return false;
	}
	
	if (!MexPhs_SmsReady() || g_mex_savesms_context.processing)
	{
		mex_log(MEX_LOG_PHS, "PSS E: busy");
		return false;
	}
	
	// check msg box type <= mmi_frm_sms_move_sms_to_otherbox
	switch (msgBox)
	{
	case E_MPSM_OUTBOX:
		status = SMSAL_STO_SENT;
		break;
	case E_MPSM_DRAFTS:
		
#ifdef __MMI_MESSAGES_DRAFT_BOX__
		status = SMSAL_STO_DRAFT;
		break;
#endif /* __MMI_MESSAGES_DRAFT_BOX__ */
		
	case E_MPSM_UNSENT:
		
#ifdef __MMI_UNIFIED_MESSAGE__
		status = SMSAL_STO_UNSENT;
		break;
#endif /* __MMI_UNIFIED_MESSAGE__ */
		
	case E_MPSM_INBOX:
	default:
		mex_log(MEX_LOG_PHS, "PSS E: no-sup box:%d", msgBox);
		return false;
	}
	
	// init savesms context
	g_mex_savesms_context.processing = true;
	g_mex_savesms_context.msgBox = msgBox;
	g_mex_savesms_context.index = MEX_PHS_SMS_INDEX_INVALID;
	g_mex_savesms_context.callback = callback;
	g_mex_savesms_context.appId = mex_GetCurrentApp();
	
	// set ad channel
	Mex_SetPhoneADChannel(simId, E_MAC_SMS_CHANNEL);
	
	sendData = OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset(sendData, 0, sizeof(mmi_frm_sms_send_struct));
	// status
	sendData->status = status;
	// content
	sendData->stringlength = mex_wcslen(pContent);
	sendData->string = OslMalloc(sendData->stringlength * 2 + 2);
	mex_wcsncpy((wchar_t *)sendData->string, pContent, sendData->stringlength);
	// dcs
	sendData->dcs = SMSAL_UCS2_DCS;
	// number
	strncpy((char *)sendData->number, pAddr, MAX_DIGITS_SMS);
#ifndef __UNIFIED_MESSAGE_SIMBOX_SUPPORT__
#ifndef __SLT_MMI_POWER_SMS
	if (sendData->status == SMSAL_STO_DRAFT)
		sendData->number[0] = '\0'; // mmi_frm_sms_convert_mo_to_entry, sms will be added into drafts box when number is empty
#endif // __SLT_MMI_POWER_SMS , 在D600 上可以保存短信号码到草稿箱中
#endif /* __UNIFIED_MESSAGE_SIMBOX_SUPPORT__ */
	mmi_frm_sms_save_sms(MexPhs_SaveSmsCB, MOD_MMI, sendData);
	//OslMfree(sendData->string); this memory block will be freed auto
	OslMfree(sendData);
	return true;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	int32          content_len;
	MN_DUAL_SYS_E  sim_index;
	
#if (MEX_SDK_VER >= 1018)
	simId = MexConfig_ConvertSimCard(simId);
#endif
	
	content_len = mex_wcslen((const wchar_t *) pContent);
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
	MMISMS_SetGloblalMsgContent(content_len,pContent);
#else
	MMISMS_SetMessageContent(content_len,(wchar*)pContent, FALSE, &g_mmisms_global.edit_content);
	
#endif
	sim_index = MN_DUAL_SYS_MAX;
	if(simId == E_MEX_SIM_1)
	{
		sim_index = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
	}    
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	else if(simId == E_MEX_SIM_2)
	{
		sim_index = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
	}
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
	else if(simId == E_MEX_SIM_3)
	{
		sim_index = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
	}
#endif //MEX_THIRD_SIM
#endif    
	
	// init savesms context
	g_mex_savesms_context.processing = true;
	g_mex_savesms_context.msgBox = msgBox;
	g_mex_savesms_context.index = MEX_PHS_SMS_INDEX_INVALID;
	g_mex_savesms_context.callback = callback;
	g_mex_savesms_context.appId = mex_GetCurrentApp();
	
	
	MMISMS_SetDestDualSys(sim_index);
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
	MMISMS_SetDestNum(pAddr,mex_strlen(pAddr));
#else
	MMISMS_SetAddressToMessage((const uint8*)pAddr,mex_strlen(pAddr), &g_mmisms_global.edit_content.dest_info.dest_list);//
#endif
	
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
	MMISMS_SetNewMsgTOSend();
#endif
	
	MMISMS_HaveEnoughSpace(sim_index, FALSE, 0);
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)	
	MMISMS_AppSaveSms(sim_index);
#else
    MMISMS_AppSaveSms(sim_index,FALSE);
#endif
	
	return true;
#endif
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if(MEX_MTK_CODE_VERSION >= 0x0952)
static void MexPhs_DelSmsRsp(srv_sms_callback_struct* callback_data)
{
	bool success;
	if (callback_data->result == MMI_TRUE)
	{
		mex_log(MEX_LOG_PHS, "DelSms ok");
		success = true;
	}
	else
	{
		mex_log(MEX_LOG_PHS, "DelSms fail res:%d", callback_data->result);
		success = false;
	}
	// CallBack
	if (g_mex_delsms_context.m_pCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_mex_delsms_context.m_iAppId);
		g_mex_delsms_context.m_pCallBack(g_mex_delsms_context.msgBox, g_mex_delsms_context.index, success);
		mex_SetCurrentApp(appId);
	}
	
	g_mex_delsms_context.m_bProcessing = false;
	g_mex_delsms_context.m_pCallBack = NULL;
	
}
#else
static void MexPhs_DelSmsRsp(void *dummy, module_type mod, U16 result)
{
	bool success;
	if (result == MMI_FRM_SMS_OK)
	{
		mex_log(MEX_LOG_PHS, "DelSms ok");
		success = true;
	}
	else
	{
		mex_log(MEX_LOG_PHS, "DelSms fail res:%d", result);
		success = false;
	}
	
	// CallBack
	if (g_mex_delsms_context.m_pCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_mex_delsms_context.m_iAppId);
		g_mex_delsms_context.m_pCallBack(g_mex_delsms_context.msgBox, g_mex_delsms_context.index, success);
		mex_SetCurrentApp(appId);
	}
	
	g_mex_delsms_context.m_bProcessing = false;
	g_mex_delsms_context.m_pCallBack = NULL;
}
#endif
#endif
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void MexPhs_DelSmsRsp(void *dummy, int32 mod, int16 result)
{
	extern uint16 MexPhs_SmsConvertMsgBox(E_MEX_PHS_SMS_MSGBOX msgBox);
	
	bool success = false;
	
	if(g_mex_delsms_context.m_bProcessing)
	{
		*(uint32 *)dummy = g_mex_delsms_context.m_bProcessing;
	}
	else
	{
		*(uint32 *)dummy = 0;
		return;
	}
	
	
	if (MN_SMS_OPERATE_SUCCESS == result)
	{    
		uint16 msg_type;
		MMISMS_OPER_ERR_E  error_code;
		
		// delete the message from order list
		MMISMS_DelSmsInfoInOrder(MMISMS_GetCurOperationOrderId());
		
		error_code = MMISMS_AppDeleteMsgBox(
			(MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(g_mex_delsms_context.msgBox),
			&msg_type
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
            ,MMISMS_FOLDER_NORMAL
#endif
			);
		
		if(MMISMS_FINISH_OPER == error_code)
		{                
			mex_log(MEX_LOG_PHS, "DelSms ok");
			success = true;            
		}
		else
		{
			return;       
		}
	}
	else
	{
		mex_log(MEX_LOG_PHS, "DelSms fail res:%d", result);
		success = false;
	}
	
	// CallBack
	if (g_mex_delsms_context.m_pCallBack != NULL)
	{
		int32 appId = mex_GetCurrentApp();
		mex_SetCurrentApp(g_mex_delsms_context.m_iAppId);
		g_mex_delsms_context.m_pCallBack(g_mex_delsms_context.msgBox, g_mex_delsms_context.index, success);
		mex_SetCurrentApp(appId);
	}
	
	g_mex_delsms_context.m_bProcessing = false;
	g_mex_delsms_context.m_pCallBack = NULL;
}
#endif

//根据短信索引值删除短信
//参数1：删除短信的回调函数指针
//参数2：模块ID
//参数3：类型ID
//参数4：短信索引值
//返回：是否成功
bool MexPhs_DelSms(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Del_Sms_CB callback)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern bool MexPhs_SmsReady(void);
	extern bool MexPhs_SmsCheckIndex(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);
	extern uint16 MexPhs_SmsConvertMsgBox(E_MEX_PHS_SMS_MSGBOX msgBox);
	
	if (!MexPhs_SmsReady())
		return false;
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	// 正在处理中
	if (g_mex_delsms_context.m_bProcessing)
	{
		return false;
	}
	else
	{
#if(MEX_MTK_CODE_VERSION >= 0x0952)
		
		U16 msg_id;
		
		msg_id = srv_sms_get_msg_id(MexPhs_SmsConvertMsgBox(msgBox), index);
		
		if(msg_id == 0xffff)
			return false;
		
		g_mex_delsms_context.m_bProcessing = true;
		g_mex_delsms_context.m_pCallBack = callback;
		g_mex_delsms_context.m_iAppId = mex_GetCurrentApp();
		g_mex_delsms_context.msgBox = msgBox;
		g_mex_delsms_context.index = index;
		srv_sms_delete_msg(msg_id, MexPhs_DelSmsRsp,0);
		
#else
		g_mex_delsms_context.m_bProcessing = true;
		g_mex_delsms_context.m_pCallBack = callback;
		g_mex_delsms_context.m_iAppId = mex_GetCurrentApp();
		g_mex_delsms_context.msgBox = msgBox;
		g_mex_delsms_context.index = index;
		mmi_frm_sms_delete_sms(MexPhs_DelSmsRsp, MOD_MMI, MexPhs_SmsConvertMsgBox(msgBox), index);
#endif
		return true;
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	extern bool MexPhs_SmsReady(void);
	extern bool MexPhs_SmsCheckIndex(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);
	extern uint16 MexPhs_SmsConvertMsgBox(E_MEX_PHS_SMS_MSGBOX msgBox);
	
	MMISMS_ORDER_ID_T pGivenSms;
	uint16            msg_type;
	
	if (!MexPhs_SmsReady())
		return false;
	if (!MexPhs_SmsCheckIndex(msgBox, index))
		return false;
	
	// 正在处理中
	if (g_mex_delsms_context.m_bProcessing)
	{
		return false;
	}
	else
	{
		g_mex_delsms_context.m_bProcessing = true;
		g_mex_delsms_context.m_pCallBack = callback;
		g_mex_delsms_context.m_iAppId = mex_GetCurrentApp();
		g_mex_delsms_context.msgBox = msgBox;
		g_mex_delsms_context.index = index;
	}
	
	
	//获取指定短信的信息
	pGivenSms = MMISMS_GetLinkedArrayPtrIdInList((MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox) ,index); 
	
	if(pGivenSms == NULL)
	{       
		return false;
	}
	
	//判断当前读取的短信的类型是否为SMS
	if(pGivenSms->flag.msg_type != MMISMS_TYPE_SMS)
	{       
		return false;
	}    
	
	while(pGivenSms != NULL)
	{
		pGivenSms->flag.is_marked = 1;
		pGivenSms = pGivenSms->next_long_sms_ptr;
	}
	
	if(
        MMISMS_NO_ERR == MMISMS_AppDeleteMsgBox(
        (MMISMS_BOX_TYPE_E)MexPhs_SmsConvertMsgBox(msgBox),
        &msg_type
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
        ,MMISMS_FOLDER_NORMAL
#endif
        )
    )
	{
		return true;
	}
	
	return false;
	
#endif 
}

bool MexPhs_SmsMsgBoxExist(E_MEX_PHS_SMS_MSGBOX msgBox)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	switch (msgBox)
	{
	case E_MPSM_INBOX:
		return true;
	case E_MPSM_OUTBOX:
#ifdef __MMI_MESSAGES_DRAFT_BOX__       
		return true;
#endif /* __MMI_MESSAGES_DRAFT_BOX__ */
	case E_MPSM_DRAFTS:
		return true;
	case E_MPSM_UNSENT:
#if(MEX_MTK_CODE_VERSION >= 0x0952)
#ifdef __SRV_SMS_UNSENT_LIST__
		return true;
#endif
#else
#ifdef __MMI_UNIFIED_MESSAGE__
		return true;
#endif /* __MMI_UNIFIED_MESSAGE__ */
#endif
	default:
		return false;
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	switch (msgBox)
	{
	case E_MPSM_INBOX:
	case E_MPSM_OUTBOX:
	case E_MPSM_DRAFTS:
	case E_MPSM_UNSENT:
		return true;
	default:
		return false;
	}
#endif
}

#ifdef MEX_USE_ASSERT

int32 MexPhs_GetNbSmsCallBack(int32 iAppId)
{
	int32 nCallback = 0;
	
	if (g_mex_delsms_context.m_iAppId == iAppId &&
		g_mex_delsms_context.m_pCallBack != NULL)
	{
		mex_log(MEX_LOG_PHS, "Detected Delete sms callback:%d", g_mex_delsms_context.m_pCallBack);
		nCallback++;
	}
	if (g_mex_readsms_context.appId == iAppId &&
		g_mex_readsms_context.callback != NULL)
	{
		mex_log(MEX_LOG_PHS, "Detected Read sms callback:%d", g_mex_readsms_context.callback);
		nCallback++;
	}
	if (g_mex_savesms_context.appId == iAppId &&
		g_mex_savesms_context.callback != NULL)
	{
		mex_log(MEX_LOG_PHS, "Detected Save sms callback:%d", g_mex_savesms_context.callback);
		nCallback++;
	}
	
	return nCallback;
}

#endif // MEX_USE_ASSERT

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//===================================== Sms Hook start
#if (MEX_MTK_CODE_VERSION >= 0x0952)
int MetMex_RecvieMsg(srv_sms_event_struct* event_data)
{
	extern int MexPhs_HookResult(U16 msgId, void* MsgStruct, int mod_src, void* peerBuff, int msg_from);
	int iHookRes;
	int mod_src = MOD_L4C;
	
	srv_sms_event_new_sms_struct* event_info;
	srv_sms_new_msg_struct *new_msg_data;
	
	event_info = (srv_sms_event_new_sms_struct*)event_data->event_info;
	new_msg_data = (srv_sms_new_msg_struct*)event_info->msg_data;
	
	if(event_info->msg_id == SRV_SMS_INVALID_MSG_ID)
		return 0;
	if (new_msg_data->sim_id == SRV_SMS_SIM_1)
		mod_src = MOD_L4C;
	else if (new_msg_data->sim_id == SRV_SMS_SIM_2)
		mod_src = MOD_L4C_2;
#ifdef __MET_MEX_SIM_PLUS__
#if (GEMINI_PLUS >= 3)
	else if (new_msg_data->sim_id == SRV_SMS_SIM_3)
		mod_src = MOD_L4C_3;
#if (GEMINI_PLUS >= 4)
	else if (new_msg_data->sim_id == SRV_SMS_SIM_4)
		mod_src = MOD_L4C_4;
#endif /* GEMINI_PLUS >= 4 */
#endif /* GEMINI_PLUS >= 3 */
#endif/*__MET_MEX_SIM_PLUS__*/
	else
		mex_log(MEX_LOG_PHS, "E: hk sms sim:%d", new_msg_data->sim_id);
	
#ifdef MEX_USE_ASSERT
	mex_log(MEX_LOG_PHS, "I: hk sms dcs:%d", new_msg_data->dcs);
	mex_log(MEX_LOG_PHS, "I: hk sms dat:0x%x 0x%x ... 0x%x 0x%x", event_info->content[0], event_info->content[1], event_info->content[new_msg_data->message_len - 2], event_info->content[new_msg_data->message_len - 1]);
	mex_log(MEX_LOG_PHS, "I: hk sms len:%d", new_msg_data->message_len);
#endif
	
	if(new_msg_data->message_len <= 0)
	{
		//空短信不作处理
		return 0;
	}
	
	iHookRes = MexPhs_HookResult(MSG_ID_MMI_SMS_DELIVER_MSG_IND, event_data, mod_src, NULL, 1);
	
	
#if MEX_SDK_VER >= 1009
	if (iHookRes == MEX_H_PHS_HK_RES_COMMON_HIJACK ||
		iHookRes == MEX_H_PHS_HK_RES_SMS_HIJACK_AND_HIDE)
#else
		if (iHookRes != 0)
#endif
		{
			// 短信被删除
			srv_sms_delete_msg(event_info->msg_id, NULL, (void*)NULL); 
			return 1;
		}
		
#if MEX_SDK_VER >= 1009
		if (iHookRes == MEX_H_PHS_HK_RES_SMS_HIDE_MSG_IND)
		{
			// 短信保存在收件箱，但不弹出提示框
			return 1;
		}
#endif
		
		// 短信未处理
		return 0;
}
#endif
//===================================== Sms Hook end



//===================================== Sms Send start

extern void mex_Phs_SmsSendCallBack(E_MEX_PHS_SMS_RESULT res);
#if (MEX_MTK_CODE_VERSION >= 0x08B0)
U8 Mex_DeliveryRepyStates[2] = {0};
#else
extern U8 DeliveryRepyStates[2];
#endif
static bool g_mex_IsCloseSmsReport = false;
U8 g_Mex_Sms_SendMessage = 0;


//判断短信发送是否已经准备就绪
#if(MEX_MTK_CODE_VERSION >= 0x0952)
bool MexPhs_IsSendSmsReady(E_MEX_Sim_Card_ID simId)
{
	bool  result;
	uint8 ServiceType;
	
	if(E_MEX_SIM_1 == simId)
		ServiceType = MexConfig_GetPhoneConfig(E_MPC_Master_Service_Type) /*g_pwr_context.CurrentServiceType*/;
	else
		ServiceType = MexConfig_GetPhoneConfig(E_MPC_Slave_Service_Type);
	
	if(ServiceType == 0)	//NO_SERVICE)
	{
		result = false;
	}
	else if(MexPhs_SmsReady() == false)
	{
		result = false;
	}
	else
		result = true;
	
	return result;
}
#else
bool MexPhs_IsSendSmsReady(E_MEX_Sim_Card_ID simId)
{
	bool  result;
	uint8 ServiceType;
	
	if(E_MEX_SIM_1 == simId)
		ServiceType = MexConfig_GetPhoneConfig(E_MPC_Master_Service_Type) /*g_pwr_context.CurrentServiceType*/;
	else
		ServiceType = MexConfig_GetPhoneConfig(E_MPC_Slave_Service_Type);
	
	if(ServiceType == NO_SERVICE)
	{
		result = false;
	}
	else if(mmi_frm_sms_get_sms_list_size(0x00)==MMI_FRM_SMS_INVALID_INDEX)   //not ready
	{
		result = false;
	}
	else
	{
		if(mmi_frm_sms_check_action_pending())         //other message processing
			result = false;
		else
			result = true;
	}
	
	return result;
}
#endif


#if(MEX_MTK_CODE_VERSION >= 0x0952)
void MexPhs_SmsSendRsp(srv_sms_callback_struct* callback_data)
{
	E_MEX_PHS_SMS_RESULT res;
	
	if (callback_data->result == MMI_TRUE)
	{
		res = E_MPSR_RESULT_SUCCESS;
	}
	else
	{
		res = E_MPSR_RESULT_FAIL;
	}
	
	mex_Phs_SmsSendCallBack(res);
}

#else
//短信发送回调
static void MexPhs_SmsSendRsp(void* number, module_type mod, uint16 uResult)
{
	E_MEX_PHS_SMS_RESULT res;
	
	// 短信提示恢复
	g_Mex_Sms_SendMessage = 0;
	
	// mex_log(MEX_LOG_PHS, "SmsSendRsp res:%d", uResult);
	
	switch(uResult)
	{
	case MMI_FRM_SMS_OK:
		res = E_MPSR_RESULT_SUCCESS;
		break;
	case MMI_FRM_SMS_ABORT:
	case MMI_FRM_SMS_NOTREADY:
	case MMI_FRM_SMS_RETURN:
	case MMI_FRM_SMS_END:
		res = E_MPSR_RESULT_FAIL;
		break;
	default:
		res = E_MPSR_RESULT_FAIL;
		break;
	}
	mex_Phs_SmsSendCallBack(res);
	
	// 短信报告恢复
	if(g_mex_IsCloseSmsReport)
	{
		g_mex_IsCloseSmsReport = false;
#if (MEX_MTK_CODE_VERSION < 0x08B0)
		DeliveryRepyStates[0] = 1;
		mmi_frm_sms_set_common_settings(NULL, MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI), DeliveryRepyStates);
#else
		Mex_DeliveryRepyStates[0] = 1;
		mmi_frm_sms_set_common_settings(NULL, MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI), Mex_DeliveryRepyStates);
#endif
	}  
#if !defined(MEX_PLATFORM_FOR_MTK6253)&&!defined(MEX_PLATFORM_FOR_MTK6235)
	memset(g_msg_cntx.smsPhoneNumber, 0, sizeof(g_msg_cntx.smsPhoneNumber));  //06101419
#endif
}

#endif

//实际发送短信的处理
bool MexPhs_SmsSend(E_MEX_Sim_Card_ID simId, const char* szDestNumber, const char* szContent, E_MEX_PHS_SMS_ENCODE eEncode)
{
#if(MEX_MTK_CODE_VERSION >= 0x0952)
	if (MexPhs_IsSendSmsReady(simId))
	{
		srv_sms_sim_enum sim_id;
		
		CHAR usc2_number[(SRV_SMS_MAX_ADDR_LEN + 1) * ENCODING_LENGTH];
		switch (simId)
		{
		case E_MEX_SIM_1:
			sim_id = SRV_SMS_SIM_1;
			break;
		case E_MEX_SIM_2:
			sim_id = SRV_SMS_SIM_2;
			break;
#ifdef __MET_MEX_SIM_PLUS__
#if (MMI_MAX_SIM_NUM >= 3)
		case E_MEX_SIM_3:
			sim_id = SRV_SMS_SIM_3;
			break;
#endif
#if (MMI_MAX_SIM_NUM >= 4)
		case E_MEX_SIM_4:
			sim_id = SRV_SMS_SIM_4;
			break;
#endif
#endif/*__MET_MEX_SIM_PLUS__*/
		default:
			sim_id = SRV_SMS_SIM_1;
            break;
		}
		memset(usc2_number, 0, sizeof(usc2_number));
		mmi_asc_to_ucs2((CHAR*)usc2_number, (CHAR*)szDestNumber);
		
		if(eEncode == E_MPSE_ENCODE_UNICODE)
		{
			srv_sms_send_ucs2_text_msg((S8*)szContent, (S8*)usc2_number, sim_id, MexPhs_SmsSendRsp, NULL);
		}
		else
		{
			int8 * textbufUCS2 = NULL;
			uint32 len;
			
			len = mex_strlen(szContent);
			textbufUCS2 = (int8 *)mex_malloc((len+1)<<1);
			if(textbufUCS2 == NULL)
				return false;
			mex_str2nwcs((wchar_t*)textbufUCS2, (len+1)<<1, szContent);
			srv_sms_send_ucs2_text_msg((S8*)textbufUCS2, (S8*)usc2_number, sim_id, MexPhs_SmsSendRsp, NULL);
			mex_free(textbufUCS2);
		}
		return true;
		
	}
#else
	
	if (MexPhs_IsSendSmsReady(simId))
	{
		int32 numlen = 0;
		//int32 iMaxDigitsSms = MexConfig_ConvertMtkDef(E_M2MDC_SMSAL_MAX_MSG_LEN);
		//int8* pNumber = (int8*)wszDestNumber;
		//int8* pContent = (int8*)wszContent;
		int8 * textbufUCS2 = NULL;
		//int8 smsPhoneNumber [(MAX_DIGITS_SMS+1)*ENCODING_LENGTH] = {0};   //由于MAX_DIGITS_SMS和ENCODING_LENGTH在不同的平台上可能不一样 所以建议使用动态分配内存
		EMSData	* pEMS = NULL;
		uint8  result;
		uint32 len,i;
		extern EMSData *GetEMSDataForEdit(EMSData **p, uint8 force);
		
		if(eEncode == E_MPSE_ENCODE_UNICODE)
			len = mex_wcslen((wchar_t*)szContent);
		else
			len = mex_strlen(szContent);
		
		mex_log(MEX_LOG_PHS, "I:szContent len:%d", len);
		
		ReleaseEMSEditBuffer();
		pEMS = GetEMSDataForEdit (0, 1);
		
		if(pEMS == NULL)
			return false;
#if (MEX_MTK_CODE_VERSION < 0x0924)
		g_msg_cntx.sendSaveStatus = MexConfig_ConvertMtkDef(E_M2MDC_SMS_SEND_SAVE_NIL)/*SEND_SAVE_NIL*/;
		mex_memset (g_msg_cntx.smsPhoneNumber, 0, sizeof(g_msg_cntx.smsPhoneNumber)); 
		
		numlen = mex_strlen(szDestNumber);// > MAX_DIGITS_SMS?MAX_DIGITS_SMS:mex_wcslen(wszDestNumber);
		numlen = numlen > MAX_DIGITS_SMS ? MAX_DIGITS_SMS : numlen;
		
		mex_log(MEX_LOG_PHS, "I:szDestNumber len:%d", numlen);
		
		mex_str2nwcs((wchar_t*)g_msg_cntx.smsPhoneNumber, (numlen+1)<<1, szDestNumber);
		//MET_PLAT_AnsiiNToUnicodeString((S8 *) smsPhoneNumber,( S8 *) pNumber,numlen);
		//UI_strcpy((UI_string_type)g_msg_cntx.smsPhoneNumber,(UI_string_type)wszDestNumber);
#endif
		PendingSaveSendData.totalSegments=1;
		PendingSaveSendData.mti = MexConfig_ConvertMtkDef(E_M2MDC_SMSAL_MTI_SUBMIT)/*SMSAL_MTI_SUBMIT*/;//MS->SC
		mex_memset(PendingSaveSendData.TPUD, 0,sizeof(PendingSaveSendData.TPUD));
		
		for (i=1; i<mmi_msg_get_seg_num(); i++)
		{
			PendingSaveSendData.TPUDLen[i]=0;
			PendingSaveSendData.TPUD_p[i]=NULL;
			PendingSaveSendData.l4index[i]=MexConfig_ConvertMtkDef(E_M2MDC_SMS_INVALID_INDEX);
			PendingSaveSendData.TPUD_udhi[i]= false;
		}
#if (MEX_MTK_CODE_VERSION < 0x0924)
		g_msg_cntx.sendMessageCase = MexConfig_ConvertMtkDef(E_M2MDC_SEND_CASE_SEND_ONLY)/*SEND_CASE_SEND_ONLY*/;
#endif
		textbufUCS2 = (int8 *)mex_malloc((len+1)<<1);
		
		if(textbufUCS2 == NULL)
			return false;
		
		mex_memset(textbufUCS2,0,(len+1)<<1);
		
		if(eEncode == E_MPSE_ENCODE_UNICODE)
		{
			mex_wcscpy((wchar_t*)textbufUCS2, (wchar_t*)szContent);
			//ConvertString((char *)textbufUCS2);
			result = AppendEMSString (MexConfig_ConvertMtkDef(E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE)/*INPUT_TYPE_ALPHANUMERIC_SENTENCECASE*/,
				pEMS, (uint8*)textbufUCS2, MexConfig_ConvertMtkDef(E_M2MDC_SMSAL_UCS2_DCS)/*SMSAL_UCS2_DCS*/, NULL);  
		}
		else
		{
			//textbufUCS2 = (int8 *)mex_malloc((len+1)<<1);
			
			//if(textbufUCS2 == NULL)
			//	return E_MEX_SMS_STATE_IGNORE;
			
			//mex_memset(textbufUCS2,0,(len+1)<<1);
			mex_str2nwcs((wchar_t*)textbufUCS2, (len+1)<<1, szContent);
			//MET_PLAT_AnsiiToUnicodeString(textbufUCS2,(S8 *)pContent);
			result = AppendEMSString (MexConfig_ConvertMtkDef(E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE),
				pEMS, (uint8*)textbufUCS2, MexConfig_ConvertMtkDef(E_M2MDC_SMSAL_DEFAULT_DCS), NULL);  
		}
		
		mex_free(textbufUCS2);
		
		if(result)
		{
			mmi_frm_sms_send_struct * sendData=mex_malloc(sizeof(mmi_frm_sms_send_struct));
			
			if(sendData == NULL)
				return false;
			
			mex_memset((int8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
			mex_memset(sendData->number, 0, MAX_DIGITS_SMS);	
			
			mex_strcpy((char*)sendData->number, szDestNumber);
			
			
			// 短信提示隐藏
			g_Mex_Sms_SendMessage = 1;
			
			// 短信报告隐藏
#if (MEX_MTK_CODE_VERSION < 0x08B0)
			if(1 == DeliveryRepyStates[0])
			{
				g_mex_IsCloseSmsReport = true;
				DeliveryRepyStates[0] = 0;
				mmi_frm_sms_set_common_settings(NULL,  MOD_MMI,  DeliveryRepyStates);
			}
#else
			{
				Mex_DeliveryRepyStates[0] = 0;
				mmi_frm_sms_set_common_settings(NULL, MOD_MMI, Mex_DeliveryRepyStates);	
			}
#endif
			
			
			
			switch (simId)
			{
			case E_MEX_SIM_1:
#if defined(__SLT_DUAL_SIM_GEMINI__)
				sendData->slt_selectedsim = SLT_MMI_SIM1;
#elif (MEX_MTK_CODE_VERSION >= 0x0924)
#if defined(__MMI_DUAL_SIM_MASTER__)
				sendData->sendrequire = MMI_FRM_SMS_SEND_BY_SIM1;	
#endif
#endif
				break;
			case E_MEX_SIM_2:
#if defined(__SLT_DUAL_SIM_GEMINI__)
				sendData->slt_selectedsim = SLT_MMI_SIM2;
#elif (MEX_MTK_CODE_VERSION >= 0x0924)
#if defined(__MMI_DUAL_SIM_MASTER__)
				sendData->sendrequire = MMI_FRM_SMS_SEND_BY_SIM2;	
#endif
#endif
				break;
			default:
				break;
			}
#if defined(__SLT_DUAL_SIM_GEMINI__)	        
			sendData->sendcheck |= MMI_FRM_SMS_SIM_SELECT;
#endif /* __SLT_DUAL_SIM_GEMINI__ */
			
			Mex_SetPhoneADChannel(simId, E_MAC_SMS_CHANNEL);
			mmi_frm_sms_send_sms(MexPhs_SmsSendRsp, MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI), sendData);
			
			mex_free(sendData);
			
			return true;  
		}//begin send message,but not sure send successfully
		else 
			return false;
	}
	
#endif
	return false;
}
#endif
//===================================== Sms Send End



//显示输入框屏幕的函数
//-------------------------------
//始于版本1.0.0
//上次修改09.05.04
void mex_InputBox_Entry(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	uint8 * gui_buffer = NULL;
	extern uint8 mex_InputBox_DelScrnCallback(void* ptr);
	extern void mex_InputBox_Firm(void);	
	extern Mex_Input_Context g_mex_input_context;
	extern bool MexSystem_IsSysRunning(void);
	
	if(!MexSystem_IsSysRunning())
	{
		return;
	}
	
	
	//进入新的屏幕
	EntryNewScreen(E_MSI_INPUT_BOX , NULL , mex_InputBox_Entry , NULL);
	gui_buffer = GetCurrGuiBuffer(E_MSI_INPUT_BOX);
	
	SetDelScrnIDCallbackHandler( E_MSI_INPUT_BOX, (HistoryDelCBPtr)mex_InputBox_DelScrnCallback );
	
#if (MEX_MTK_CODE_VERSION >= 0x0824)
#if(MEX_MTK_CODE_VERSION >= 0x1112)
	{
		extern void mmi_imc_set_is_disable_candidate_box(MMI_BOOL is_disabled);
		if((g_mex_input_context.m_iInputType == MexConfig_ConvertMtkDef(E_M2MDC_INPUT_SM_MULTITAP_PINYIN))||(g_mex_input_context.m_iInputType == MexConfig_ConvertMtkDef(E_M2MDC_INPUT_SM_PINYIN)))
			mmi_imc_set_is_disable_candidate_box(FALSE);	
	}
#endif
	//显示输入框
	ShowCategory5Screen(0 , 0 , STR_GLOBAL_OK , IMG_GLOBAL_OK , STR_GLOBAL_BACK ,
		IMG_GLOBAL_BACK , (uint32)g_mex_input_context.m_iInputType, (uint8 *) g_mex_input_context.m_pInputBuffer,
		g_mex_input_context.m_iInputMaxNum, gui_buffer);
#else
	//显示输入框
	ShowCategory5Screen(0 , 0 , STR_GLOBAL_OK , IMG_GLOBAL_OK , STR_GLOBAL_BACK ,
		IMG_GLOBAL_BACK , (int16)g_mex_input_context.m_iInputType, (uint8 *) g_mex_input_context.m_pInputBuffer,
		g_mex_input_context.m_iInputMaxNum, gui_buffer);
#endif
	
	if( g_mex_input_context.m_pInputTitle != NULL )
		MMI_title_string = (UI_string_type)g_mex_input_context.m_pInputTitle;
	else
		MMI_title_string = (UI_string_type)L"Input";
	draw_title();
	
	//退出窗口时的处理方法
	SetLeftSoftkeyFunction( mex_InputBox_Firm, KEY_EVENT_UP );
	SetCategory5RightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	if((bool) MexConfig_GetPhoneConfig(E_MPC_Usb_MassStorage))
		GoBackHistory();
	//如果不可改输入模式，抢注#事件
	if( !g_mex_input_context.m_bCanChange )
	{
		SetKeyHandler( NULL,	KEY_POUND,	KEY_EVENT_DOWN);
		SetKeyHandler( NULL,	KEY_POUND,	KEY_EVENT_UP);
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif    
}

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
typedef void (* mmi_pen_hdlr)(int16 x, int16 y);
#endif
void MexInterface_Pen_Reg_Up_Handler(mmi_pen_hdlr pen_fp)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_up_handler(pen_fp);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}

void MexInterface_Pen_Reg_Down_Handler(mmi_pen_hdlr pen_fp)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(pen_fp);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif       
}

void MexInterface_Pen_Reg_Move_Handler(mmi_pen_hdlr pen_fp)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_move_handler(pen_fp);
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif        
}
static void dummy()
{
	
}



//获取基站信息的请求
void MexInterface_GetCellInfo_Req(E_MEX_Sim_Card_ID sim, void *cb)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	MYQUEUE Message;
#if (MEX_MTK_CODE_VERSION >= 0x0852)
	{
		int i=0;
		mmi_em_update_req_struct *em_start_req;
		if(sim == E_MEX_SIM_1)
		{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
			Mex_SetProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
			mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_EM_UPDATE_RSP, (PsIntFuncPtr)NULL);
#else
			Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_UPDATE_REQ);
			Mex_SetProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
			SetProtocolEventHandler(NULL, MSG_ID_MMI_EM_UPDATE_RSP);
#endif
		}
		else if ((sim == E_MEX_SIM_2)
			
#ifdef __MET_MEX_SIM_PLUS__
			||(sim == E_MEX_SIM_3)||(sim == E_MEX_SIM_4)
#endif
			)
		{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
			Mex_SetSlaveProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
			mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_EM_UPDATE_RSP, (PsIntFuncPtr)NULL);
#else
			Mex_ClearSlaveProtocolEventHandler(MSG_ID_MMI_EM_UPDATE_REQ);
			Mex_SetSlaveProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
			Mex_SetSlaveProtocolEventHandler(NULL, MSG_ID_MMI_EM_UPDATE_RSP);
#endif
		}
        else
        {
            // TODO: Get cell info from other sim card...
            break;
        }
		
		Message.oslMsgId = MSG_ID_MMI_EM_UPDATE_REQ;
		em_start_req = OslConstructDataPtr(sizeof(mmi_em_update_req_struct));
		for(i = 0;i<EM_INFO_REQ_NUM ;i++)
		{
			em_start_req->info_request[i] = EM_NC;
		}
		em_start_req->info_request[RR_EM_LAI_INFO] = EM_ON;
		Message.oslDataPtr = (oslParaType *)em_start_req;
		Message.oslPeerBuffPtr = NULL;
		Message.oslSrcId = MOD_MMI;
#ifdef __MMI_DUAL_SIM_MASTER__
		if(sim == E_MEX_SIM_1)
		{
			Message.oslDestId = MOD_L4C;
		}
#ifdef __MET_MEX_SIM_PLUS__
#if (GEMINI_PLUS >= 3)
		else if(sim == E_MEX_SIM_3)
			
		{
			Message.oslDestId = MOD_L4C_3;
		}
#endif	
#if (GEMINI_PLUS >= 4)
		
		else if(sim == E_MEX_SIM_4)
			
		{
			Message.oslDestId = MOD_L4C_4;
		}
#endif
#endif/*__MET_MEX_SIM_PLUS__*/
		else
		{
			Message.oslDestId = MOD_L4C_2;
		}
#else
		Message.oslDestId = MOD_L4C;
#endif
		OslMsgSendExtQueue(&Message);
	}
#else
	{
		mmi_em_start_req_struct *em_start_req;	
		if(sim == E_MEX_SIM_1)
		{
			Mex_ClearProtocolEventHandler(MSG_ID_EM_START_RSP);
			Mex_SetProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
		}
		else if (sim == E_MEX_SIM_2)
		{
			Mex_ClearSlaveProtocolEventHandler(MSG_ID_EM_START_RSP);
			Mex_SetSlaveProtocolEventHandler((funcptr)cb, MSG_ID_MMI_EM_STATUS_IND);
		}
        else
        {
            // TODO: Get cell info from other sim card...
            break;
        }
		Mex_SetPhoneADChannel(sim, E_MAC_SMS_CHANNEL);
		
		Message.oslMsgId = MSG_ID_MMI_EM_START_REQ;
		em_start_req = OslConstructDataPtr(sizeof(mmi_em_start_req_struct));
		em_start_req->mod_id = MOD_AS;
		
		em_start_req->info_request = RR_EM_LAI_INFO;
		
		Message.oslDataPtr = (oslParaType*) em_start_req;
		Message.oslPeerBuffPtr = NULL;
		Message.oslSrcId = MOD_MMI;
		if(sim == E_MEX_SIM_1)
			Message.oslDestId = MOD_L4C;
#ifdef __MET_MEX_SIM_PLUS__
		else if(sim == E_MEX_SIM_3)
			Message.oslDestId = MOD_L4C_3;		
		else if(sim == E_MEX_SIM_4)
			Message.oslDestId = MOD_L4C_4;		
#endif/*__MET_MEX_SIM_PLUS__*/
		else
			Message.oslDestId = MOD_L4C_2;		
		
		OslMsgSendExtQueue(&Message);
	}
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif        
}




//结束基站信息的请求
void MexInterface_EndCellInfo_Req(E_MEX_Sim_Card_ID sim ,void *funcPtr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	MYQUEUE Message;
	if(sim == E_MEX_SIM_1)
	{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND,funcPtr);
#else
		Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND);
#endif
	}
	else
	{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		Mex_ClearSlaveProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND,funcPtr);
#else
		Mex_ClearSlaveProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND);
#endif
	}
#if (MEX_MTK_CODE_VERSION >= 0x0852)
	{
		int i=0;
		mmi_em_update_req_struct *em_stop_req;
		
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_EM_UPDATE_RSP, (PsIntFuncPtr)NULL);
#else
		if(sim == E_MEX_SIM_1)
		{
			Mex_SetProtocolEventHandler(NULL, MSG_ID_MMI_EM_UPDATE_RSP);
		}
		else
		{
			Mex_SetSlaveProtocolEventHandler(NULL, MSG_ID_MMI_EM_UPDATE_RSP);
		}
#endif
		
		Message.oslMsgId = MSG_ID_MMI_EM_UPDATE_REQ;
		
		em_stop_req = OslConstructDataPtr(sizeof(mmi_em_update_req_struct));
		
		for(i = 0;i<EM_INFO_REQ_NUM ;i++)
		{
			em_stop_req->info_request[i] = EM_NC;
		}
		
		em_stop_req->info_request[RR_EM_LAI_INFO] = EM_OFF;
		
		Message.oslDataPtr = (oslParaType *)em_stop_req;
		Message.oslPeerBuffPtr = NULL;
		Message.oslSrcId = MOD_MMI;
#ifdef __MMI_DUAL_SIM_MASTER__
		if(sim == E_MEX_SIM_1)
			Message.oslDestId = MOD_L4C;
#ifdef __MET_MEX_SIM_PLUS__
#if (GEMINI_PLUS >= 3)
		
		else if(sim == E_MEX_SIM_3)
			Message.oslDestId = MOD_L4C_3;
#endif
#if (GEMINI_PLUS >= 4)
		
		else if(sim == E_MEX_SIM_4)
			Message.oslDestId = MOD_L4C_4;
#endif
#endif/*__MET_MEX_SIM_PLUS__*/
		else
			Message.oslDestId = MOD_L4C_2;
#else
		Message.oslDestId = MOD_L4C;
#endif
		OslMsgSendExtQueue(&Message);
	}
#else
	{
		mmi_em_stop_req_struct *em_stop_req;
		module_type mod_id ;
		request_info_type info_id;   
		mod_id =MOD_AS;
		info_id = RR_EM_CELL_SELECT_PARA_INFO|RR_EM_CHANNEL_DESCR_INFO|
			RR_EM_CTRL_CHANNEL_DESCR_INFO|RR_EM_RACH_CTRL_PARA_INFO|
			RR_EM_LAI_INFO|RR_EM_RADIO_LINK_COUNTER_INFO|
			RR_EM_MEASUREMENT_REPORT_INFO|RR_EM_CONTROL_MSG_INFO|
			RR_EM_TBF_INFO|RR_EM_GPRS_GENERAL_INFO;
		
		Message.oslMsgId = MSG_ID_MMI_EM_STOP_REQ;
		em_stop_req = OslConstructDataPtr(sizeof(mmi_em_stop_req_struct));
		em_stop_req->mod_id = mod_id;
		em_stop_req->info_request = info_id;
		
		Message.oslDataPtr = (oslParaType *)em_stop_req;
		Message.oslPeerBuffPtr = NULL;
		Message.oslSrcId = MOD_MMI;
		if(sim == E_MEX_SIM_1)
			Message.oslDestId = MOD_L4C;
#ifdef __MET_MEX_SIM_PLUS__
		else if(sim == E_MEX_SIM_3)
			Message.oslDestId = MOD_L4C_3;
		else if(sim == E_MEX_SIM_4)
			Message.oslDestId = MOD_L4C_4;
#endif/*__MET_MEX_SIM_PLUS__*/
		else
			Message.oslDestId = MOD_L4C_2;
		
		Message.oslDestId = MOD_L4C;
		OslMsgSendExtQueue(&Message);
	}
#endif	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#endif
}


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#define MET_MEX_NET_RECONNECT_INTERVAL (200)   //网络重连时钟间隔
#define MET_MEX_NET_RECONNECT_MAX_TIME (5)    //网络重连最大次数 

static void MexInterface_NetReconnect(void)
{
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
    //重置网络连接状态
    MNGPRS_ResetAndDeactivePdpContextEx(g_met_mex_GprsReq_Context.dual_sys);
	
    //激活GPRS连接
    MNGPRS_SetAndActivePdpContextEx(
        g_met_mex_GprsReq_Context.dual_sys,
        g_met_mex_GprsReq_Context.APNcn
#ifdef PRODUCT_DM         
        ,g_met_mex_GprsReq_Context.ps_service_type
#endif         
		); 
	
    g_met_mex_GprsReq_Context.uConnect_time ++;
    mex_log(MEX_LOG_SYSTEM,"MexInterface_NetReconnect time: %d",g_met_mex_GprsReq_Context.uConnect_time);  
#else
    //重置网络连接状态
    MMIAPIPDP_ReleaseAll(NULL);
	
    //激活GPRS连接
    MNGPRS_ReadStorageExtReqQosEx(g_met_mex_GprsReq_Context.dual_sys,
		g_met_mex_GprsReq_Context.storage,
		g_met_mex_GprsReq_Context.ps_service_type,
		&g_met_mex_GprsReq_Context.app_info.qos_info);
	
    MMIAPIPDP_Active(&g_met_mex_GprsReq_Context.app_info);
    g_met_mex_GprsReq_Context.uConnect_time ++;
    mex_log(MEX_LOG_SYSTEM,"MexInterface_NetReconnect time: %d",g_met_mex_GprsReq_Context.uConnect_time);  
#endif
}

#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
MMI_RESULT_E MexInterface_HandlePsAndRefMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    int           ret;
    extern void MexSoc_InitNw_Cb(void *result);
    APP_MN_GPRS_T  *signal_ptr = NULL;
	
	//SCI_TraceLow:"MexInterface_HandlePsAndRefMsg Msg id=%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5086_112_2_18_2_36_46_12,(uint8*)"d",msg_id);
	
    if(g_met_mex_is_active_gprs == false)
	{	    
		//SCI_TraceLow:"MexInterface_HandlePsAndRefMsg is not need process"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5090_112_2_18_2_36_46_13,(uint8*)"");
        //通知MMI内核消息未被处理
		return MMI_RESULT_FALSE;
	}
    
    switch(msg_id)
    {
	case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
		signal_ptr = (APP_MN_GPRS_T*)param;
		
		//SCI_TraceLow:"MexInterface_HandlePsAndRefMsg activate result=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5100_112_2_18_2_36_46_14,(uint8*)"d",signal_ptr->result);
		mex_log(MEX_LOG_SYSTEM,"[MEX]:pdp active cnf result=%d",signal_ptr->result);
		if(signal_ptr->result == MN_GPRS_ERR_SUCCESS)
		{
			//PDP激活成功
			MexSoc_InitNw_Cb((void *)1); 
		}
		else if(signal_ptr->result == MN_GPRS_ERR_SAME_PDP_CONTEXT)
		{
			//PDP已经激活，获取当前的PDP信息              
			MexSoc_InitNw_Cb((void *)1);
		}
		else
		{
			if(g_met_mex_GprsReq_Context.uConnect_time <= MET_MEX_NET_RECONNECT_MAX_TIME)//重连5次
			{
				MetMex_StartTimer(
					MetMex_GetNetReconnectTimerId(),
					MET_MEX_NET_RECONNECT_INTERVAL,
					MexInterface_NetReconnect,
					false
					);
				mex_log(MEX_LOG_SYSTEM,"[MEX]:pdp active failed reconnect result=%d",signal_ptr->result); 
			}
			else
			{
				MexSoc_InitNw_Cb((void *)0);
				g_met_mex_is_active_gprs = false;
			}
		}
		break;
	case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
		signal_ptr = (APP_MN_GPRS_T*)param;
		ret = MNPHONE_GprsDetachEx(g_met_mex_phone_sim,MN_PHONE_GPRS_DETACH);
		//SCI_TraceLow:"MexInterface_HandlePsAndRefMsg deactive cnf,result=%d detach=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5134_112_2_18_2_36_46_15,(uint8*)"dd",signal_ptr->result,ret);
		mex_log(MEX_LOG_SYSTEM,"MexInterface_HandlePsAndRefMsg deactive cnf,result=%d detach=%d",signal_ptr->result,ret);
		MexSoc_InitNw_Cb((void *)0);
		g_met_mex_is_active_gprs = false;
		
		break;
	case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
		signal_ptr = (APP_MN_GPRS_T*)param;
		
		ret = MNPHONE_GprsDetachEx(g_met_mex_phone_sim,MN_PHONE_GPRS_DETACH);
		//SCI_TraceLow:"Met deactive ind,result=%d detach=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5144_112_2_18_2_36_46_16,(uint8*)"dd",signal_ptr->result,ret);
		mex_log(MEX_LOG_SYSTEM,"Met deactive ind,result=%d detach=%d",signal_ptr->result,ret);            
		MexSoc_InitNw_Cb((void *)0);
		g_met_mex_is_active_gprs = false;
		
		break;
    }
	
    //通知MMI内核消息已被处理
    return MMI_RESULT_FALSE;
}
#else
PUBLIC void MexInterface_HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{	
	extern void MexSoc_InitNw_Cb(void *result);
	
	if(PNULL == msg_ptr)
	{
		return;
	}
	
	switch(msg_ptr->msg_id) 
	{
	case MMIPDP_ACTIVE_CNF:
		//SCI_TraceLow:"[MEX]:MexInterface_HandlePDPMsg MMIPDP_ACTIVE_CNF result=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5168_112_2_18_2_36_46_17,(uint8*)"d",msg_ptr->result);
        mex_log(MEX_LOG_SYSTEM,"[MEX]:pdp active cnf result=%d",msg_ptr->result);
		if(MMIPDP_RESULT_SUCC == msg_ptr->result)
		{
			//SCI_TraceLow:"[MEX]:pdp active success!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5172_112_2_18_2_36_46_18,(uint8*)"");
			MexSoc_InitNw_Cb((void*)1);
		}
		else if( (MMIPDP_RESULT_FAIL == msg_ptr->result) ||
			(MMIPDP_RESULT_TIMEOUT == msg_ptr->result) ||
			(MMIPDP_RESULT_DIFFER_DUALSYS == msg_ptr->result) )
		{
			if(g_met_mex_GprsReq_Context.uConnect_time <= MET_MEX_NET_RECONNECT_MAX_TIME)//重连3次
			{
				MetMex_StartTimer(
					MetMex_GetNetReconnectTimerId(),
					MET_MEX_NET_RECONNECT_INTERVAL,
					MexInterface_NetReconnect,
					false
					);
				
				mex_log(MEX_LOG_SYSTEM,"[MEX]:pdp active failed reconnect result=%d",msg_ptr->result); 
			}
			else
			{
				//SCI_TraceLow:"[MEX]:pdp active failed!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5192_112_2_18_2_36_46_19,(uint8*)"");
				MexSoc_InitNw_Cb((void*)0);
			}
		}
#ifndef GPRSMPDP_SUPPORT
        else if (MMIPDP_RESULT_AT_IS_ON == msg_ptr->result)
#else
			else
#endif
        {
			//SCI_TraceLow:"[MEX]:pdp active failed in else logic"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5202_112_2_18_2_36_46_20,(uint8*)"");
			MexSoc_InitNw_Cb((void*)0);
        }
		break;
		
	case MMIPDP_DEACTIVE_CNF:
        g_met_mex_is_active_gprs = false;
		break;
		
	case MMIPDP_DEACTIVE_IND:
		//SCI_TraceLow:"[MEX]:MexInterface_HandlePDPMsg: MMIPDP_DEACTIVE_IND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5212_112_2_18_2_36_46_21,(uint8*)"");
        g_met_mex_is_active_gprs = false;
		break;
		
	default:
		break;
	}
	//SCI_FREE(msg_ptr);
}
#endif
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1032)

typedef struct 
{
	bool result;
	bool bNeedUpdateAcct;
	uint32 realAcct;
}
SetGprsResult;

SetGprsResult g_GprsResult = { 0 };


void MexInterface_DelayCallBackGprsResult(void)
{
	extern void MexSoc_RegGprsCallBack(uint32 realAcct,bool result,bool bNeedUpdateAcct);
	MexSoc_RegGprsCallBack(g_GprsResult.realAcct,g_GprsResult.result,g_GprsResult.bNeedUpdateAcct);
}

void MexInterface_SetDelayCallBackGprsResult(bool result,bool bNeedUpdateAcct,uint32 realAcct)
{
	g_GprsResult.result = result;
	g_GprsResult.bNeedUpdateAcct = bNeedUpdateAcct;
	g_GprsResult.realAcct = realAcct;
	
	gui_start_timer( 1, MexInterface_DelayCallBackGprsResult);
}


bool MexInterface_AddGprsAccount(int8 mode,int8 sim_info,uint32 *account)
{
	srv_dtcnt_store_prof_data_struct prof_info;
	//U32 acct_id;
	srv_dtcnt_result_enum result;
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	srv_dtcnt_prof_gprs_struct g_data_account_gprs_profile_srv;
#else
	extern srv_dtcnt_prof_gprs_struct g_data_account_gprs_profile_srv;
#endif
	
#ifdef WIN32
	mex_log(1, "sizeof(srv_dtcnt_prof_gprs_struct):%d", sizeof(srv_dtcnt_prof_gprs_struct));
#endif
	memset(&g_data_account_gprs_profile_srv, 0, sizeof(g_data_account_gprs_profile_srv));
	prof_info.prof_type = SRV_DTCNT_BEARER_GPRS;
	prof_info.prof_data = &g_data_account_gprs_profile_srv;
	prof_info.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
	
	if(mode == E_MNM_CMWAP)
	{
		g_data_account_gprs_profile_srv.prof_common_header.AccountName = (const U8 *)L"MEX_WAP";
		g_data_account_gprs_profile_srv.APN = (const U8 *)"cmwap";
	}
	else 
	{
		g_data_account_gprs_profile_srv.prof_common_header.AccountName = (const U8 *)L"MEX_NET";
		g_data_account_gprs_profile_srv.APN = (const U8 *)"cmnet";
	}
	
	g_data_account_gprs_profile_srv.prof_common_header.acct_type = SRV_DTCNT_PROF_TYPE_USER_CONF; 
	g_data_account_gprs_profile_srv.prof_common_header.acc_id = *account;
	
	if(sim_info == E_MEX_SIM_2)
	{
		g_data_account_gprs_profile_srv.prof_common_header.sim_info = SRV_DTCNT_SIM_TYPE_2;
	}
	else
	{
		g_data_account_gprs_profile_srv.prof_common_header.sim_info = SRV_DTCNT_SIM_TYPE_1;
	}
	
	result = srv_dtcnt_store_add_prof(&prof_info, account);
	
	if(result == SRV_DTCNT_RESULT_STORE_FULL)
	{
		int32 iIndex = SRV_DTCNT_PROF_MAX_ACCOUNT_NUM;
		srv_dtcnt_result_enum get_result;
		memset(&g_storeProfDataStruct, 0, sizeof(srv_dtcnt_store_prof_data_struct));
		g_storeProfDataStruct.prof_type = SRV_DTCNT_BEARER_GPRS;
		g_storeProfDataStruct.prof_data = &g_Mex_data_account_gprs_profile_srv;
		g_storeProfDataStruct.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
		
		mex_log(MEX_LOG_SOC,"add prof store full");
		
		while(iIndex--)
		{
			if(srv_dtcnt_acct_is_read_only(iIndex))
			{
				continue;
			}
			memset(&g_Mex_data_account_gprs_profile_srv, 0, sizeof(srv_dtcnt_prof_gprs_struct));
			get_result =  srv_dtcnt_store_qry_prof(iIndex, &g_storeProfDataStruct);
			if(get_result == SRV_DTCNT_RESULT_SUCCESS)
			{
				mex_log(MEX_LOG_SOC,"get last prof success");
				prof_info.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
				result = srv_dtcnt_store_update_prof(iIndex, &prof_info);
				if(result == SRV_DTCNT_RESULT_SUCCESS)
				{
					mex_log(MEX_LOG_SOC,"update last prof success");
					iProfAccountIndex = iIndex;
					g_bResumeGprsProf = true;
					*account = iIndex;
					return true;
				}
			}
		}
	}
	
	if (result == SRV_DTCNT_RESULT_SUCCESS)
	{
#if (MEX_MTK_CODE_VERSION >= 0x1032)&&((MEX_MTK_CODE_VERSION < 0x1112))//网络配置表里刷新,否则重启
		mmi_dtcnt_add_disp_list(*account, 
			g_data_account_gprs_profile_srv.prof_common_header.AccountName,
			g_data_account_gprs_profile_srv.APN,
			DATA_ACCOUNT_BEARER_GPRS,
			0,	//DTCNT_USER_CONFIGURED_ACCT,
			1,
			g_data_account_gprs_profile_srv.prof_common_header.sim_info,
			g_data_account_gprs_profile_srv.prof_common_header.read_only);	
#endif
		return true;
	}
	
	return false;
}

void MexInterface_ResumeGprsAccount(void)
{
	if(NULL == g_bResumeGprsProf)
	{
        return;
	}
	
	g_storeProfDataStruct.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
	srv_dtcnt_store_update_prof(iProfAccountIndex, &g_storeProfDataStruct);
	
	g_bResumeGprsProf = false;
	iProfAccountIndex = 0;
}

#endif


int8 MexInterface_SetGprsReq(uint32 uAcctid, E_MEX_Sim_Card_ID sim_id, E_Mex_Net_Mode mode, funcptr pfCB)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	if(!g_bNeedAddGprsAcct)
	{
		MexInterface_SetDelayCallBackGprsResult(true,false,0);
		return E_MOR_SUCCESS;
	}
	else
	{
		
		if( MexInterface_AddGprsAccount(mode,sim_id,&uAcctid) )
		{
            int32 always_ask_sim_info = 0;
			iProfAccountIndex = uAcctid;
            if(sim_id == E_MEX_SIM_1)
			{
				always_ask_sim_info = MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_1);
				
			}
			else if(sim_id == E_MEX_SIM_2)
			{
				
				always_ask_sim_info = MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_2);
			}
            //TODO: for 3 sim card or 4 sim card...
			uAcctid = cbm_encode_data_account_id(uAcctid, always_ask_sim_info, 1, false);
			MexInterface_SetDelayCallBackGprsResult(true,true,uAcctid);
			return E_MOR_SUCCESS;
		}
	}
	
	return E_MOR_FAILED;
#else
	
	mmi_ps_set_gprs_data_account_req_struct *myMsgPtr;
	uint8	ref_count;
	uint16	msg_len;
	MYQUEUE Message;
	int8 i;
	/*create msg*/
	myMsgPtr = (mmi_ps_set_gprs_data_account_req_struct*) OslConstructDataPtr(sizeof(mmi_ps_set_gprs_data_account_req_struct));
	
	if(myMsgPtr == NULL)
	{
		return E_MOR_MEM_ERROR;//申请内存失败
	}
	
	ref_count = myMsgPtr->ref_count;
	msg_len = myMsgPtr->msg_len;
	memset(myMsgPtr,0,sizeof(mmi_ps_set_gprs_data_account_req_struct));
	myMsgPtr->ref_count = ref_count;
	myMsgPtr->msg_len = msg_len;
	
	myMsgPtr->gprs_account.context_id = uAcctid - custom_get_csd_profile_num() + 1; //10 use the last profile(of gprs) item
	myMsgPtr->gprs_account.authentication_type = 0;//PAP_AUTH;
	
	myMsgPtr->gprs_account.name_dcs = 0;//DATA_ACCOUNT_ASCII;
	
#if (MEX_MTK_CODE_VERSION >= 0x0936)
	
	myMsgPtr->gprs_account.dcomp_algo = 0x02;
	myMsgPtr->gprs_account.hcomp_algo = 0x02;
	myMsgPtr->gprs_account.pdp_type = 0x21;
	myMsgPtr->gprs_account.pdp_addr_len = 0x01; 
	myMsgPtr->profile_type = DATA_ACCT_GPRS_PROF;	
	
#endif
	
	if(mode == E_MNM_CMWAP)
	{
		myMsgPtr->gprs_account.name_length = strlen(GPRS_CMWAP_ACCOUNT_NAME);
		strcpy ((char*)myMsgPtr->gprs_account.name, GPRS_CMWAP_ACCOUNT_NAME);
		strcpy ((char*)myMsgPtr->gprs_account.apn, "cmwap");   
		myMsgPtr->gprs_account.apn_length = strlen("cmwap");
	}
	else
	{
		myMsgPtr->gprs_account.name_length = strlen(GPRS_CMNET_ACCOUNT_NAME);
		strcpy ((char*)myMsgPtr->gprs_account.name, GPRS_CMNET_ACCOUNT_NAME);
		strcpy ((char*)myMsgPtr->gprs_account.apn, "cmnet");   
		myMsgPtr->gprs_account.apn_length = strlen("cmnet");
	}
	
	strcpy ((char*)myMsgPtr->gprs_account.user_name, "");
	strcpy ((char*)myMsgPtr->gprs_account.password, "");
	
	for ( i = 0; i < 4; i++) {
		myMsgPtr->gprs_account.dns[i] = 0;
	}
	
	Message.oslMsgId = PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Mex_SetProtocolEventHandler(pfCB,PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP);
	OslMsgSendExtQueue(&Message);
	
	return E_MOR_SUCCESS;
#endif
#elif  defined(MEX_PLATFORM_SPREAD)
    uint8 apn[16] = {0};
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
    int   ret;
#endif
#if (MEX_SDK_VER >= 1018)
	sim_id = MexConfig_ConvertSimCard(sim_id);
#endif
	
	if(sim_id == E_MEX_SIM_1)
	{
		g_met_mex_phone_sim = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
	}
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)    
	else if(sim_id == E_MEX_SIM_2)
	{
		g_met_mex_phone_sim = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
	}
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
	else if(sim_id == E_MEX_SIM_3)
	{
		g_met_mex_phone_sim = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
	}
#endif //MEX_THIRD_SIM
#endif    
	
	else
	{
		g_met_mex_phone_sim = -1;
	}
	
	if(mode == E_MNM_CMWAP)
	{
		g_met_mex_net_mode = 0;
        strcpy((char*)apn, "cmwap");
	}
	else if(mode == E_MNM_CMNET)
	{
		g_met_mex_net_mode = 1;
        strcpy((char*)apn, "cmnet");
	}
	else
	{
		g_met_mex_net_mode = -1;
	}
	
	g_met_mex_is_active_gprs = true;
	mex_log(MEX_LOG_SOC, "I: PS=%d NM=%d", g_met_mex_phone_sim, 
		g_met_mex_net_mode);
	
	//保存激活GPRS 的相关信息
	g_met_mex_GprsReq_Context.uConnect_time = 1;
    g_met_mex_GprsReq_Context.dual_sys = g_met_mex_phone_sim;
	
#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
    //激活GPRS连接
    ret = MNGPRS_SetAndActivePdpContextEx(
		g_met_mex_phone_sim,
		apn
#ifdef PRODUCT_DM         
		,STREAMING_E
#endif         
		);
#ifdef PRODUCT_DM 	
	g_met_mex_GprsReq_Context.ps_service_type = STREAMING_E;
#endif 
    strcpy(g_met_mex_GprsReq_Context.APNcn,apn);
	
    //SCI_TraceLow:"MexInterface_SetGprsReq Activate err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5535_112_2_18_2_36_47_22,(uint8*)"d",ret);
    
    if(ret !=  ERR_MNGPRS_NO_ERR)
	{
		ret = MNGPRS_ResetAndDeactivePdpContextEx(g_met_mex_phone_sim);
		//SCI_TraceLow:"MexInterface_SetGprsReq Reset PDP err=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_5540_112_2_18_2_36_47_23,(uint8*)"d",ret);
		
		//激活GPRS连接
		ret = MNGPRS_SetAndActivePdpContextEx(
			g_met_mex_phone_sim,
			apn
#ifdef PRODUCT_DM         
			,STREAMING_E
#endif         
			);
		
	}
	
	
#else
	{
		BOOLEAN return_val = FALSE;
		MMIPDP_ACTIVE_INFO_T app_info = {0};
		
		app_info.app_handler = MMI_MODULE_MET_MEX;
		if(mode == E_MNM_CMWAP)
		{
			app_info.apn_ptr = (char*)"cmwap";
		}
		else if(mode == E_MNM_CMNET)
		{
			app_info.apn_ptr = (char*)"cmnet";
		}
		
		app_info.dual_sys = g_met_mex_phone_sim;
		app_info.priority = 3;
		app_info.handle_msg_callback = MexInterface_HandlePDPMsg;
		
		g_met_mex_GprsReq_Context.storage = MN_GPRS_STORAGE_ALL;
		g_met_mex_GprsReq_Context.ps_service_type = BROWSER_E;
		g_met_mex_GprsReq_Context.app_info = app_info;
		
		MNGPRS_ReadStorageExtReqQosEx(g_met_mex_phone_sim,MN_GPRS_STORAGE_ALL,BROWSER_E,&app_info.qos_info);
		return_val = MMIAPIPDP_Active(&app_info);
	}
#endif
	
	return E_MOR_SUCCESS;
	
	//如果是在模拟器上则直接回调成功
	//因为MNGPRS_SetAndActivePdpContextEx在模拟
	//器上运行会导致死机
#elif defined(MEX_PLATFORM_SIMULATOR)
	extern void MexSoc_InitNw_Cb(void *result);
	MexSoc_InitNw_Cb((void*)1);
	return E_MOR_SUCCESS;
	
#endif
}





int8 MexInterface_GetGprsReq(uint32 uAcctid ,funcptr pfCB)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	// TODO:
	return E_MOR_FAILED;
#else
	mmi_ps_get_gprs_data_account_req_struct *myMsgPtr;
	MYQUEUE Message;
	
	myMsgPtr = (mmi_ps_get_gprs_data_account_req_struct*) OslConstructDataPtr(sizeof(mmi_ps_get_gprs_data_account_req_struct));
	
	myMsgPtr->profile_id = (uint8)(uAcctid - custom_get_csd_profile_num() + 1);//19 use the last profile(of gprs) item
	
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr = NULL;
	
	Mex_SetProtocolEventHandler(pfCB,PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP);
	OslMsgSendExtQueue(&Message);
	
	return E_MOR_SUCCESS;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return E_MOR_SUCCESS;
#endif        
}




int8 MexInterface_Soc_Gethostbyname(bool is_blocking, uint8 mod_id, int32 request_id, const char *domain_name, uint8 *addr, uint8 *addr_len,
									uint8 access_id, uint32 nwk_account_id)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return soc_gethostbyname(is_blocking,mod_id,request_id,domain_name,addr,addr_len,access_id,nwk_account_id);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	unsigned long ip = 1;
    int           ret;
    int           parse_count = 0;
	
#define MAX_DOMAIN_NAME_PARSE 5
	
	while(parse_count < MAX_DOMAIN_NAME_PARSE)
    {   
        ret = MEX_PARSE_HOST((char *)domain_name,&ip,0);/* Third param - 1 for block,0 for not block */	
		mex_log(MEX_LOG_SOC, "I: SGH: %s ip= %x ret=%d", domain_name,ip,ret);
		if(MEX_SOC_SUCCESS == ret)  
		{
			//parse success
			memcpy(addr, &ip, sizeof(unsigned long));
			*addr_len = 4;
			mex_log(MEX_LOG_SOC, "I: domain_name: %s addr = [%d.%d.%d.%d]", domain_name, addr[0], addr[1], addr[2], addr[3]);
            return MEX_SOC_SUCCESS;
		}
		
        ++parse_count;
	}
    return MEX_SOC_WOULDBLOCK;
	
#endif        
	
}



#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
extern int mex_get_socket_form_array(int8 index);
#endif



int8 MexInterface_Soc_Connect(int8 s, Mex_SockAddr_Struct *addr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	sockaddr_struct convert_sruct;
	sockaddr_struct *str_ptr = &convert_sruct;
	MexConfig_ConvertStruct(E_M2MSC_SockAddr_Struct,&str_ptr,addr);
	return soc_connect(s, &convert_sruct);
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	int8 connect_ret;
	int socket_id = -1;
	struct sci_sockaddr convert_struct;
	struct sci_sockaddr *str_ptr = &convert_struct;
	MexConfig_ConvertStruct(E_M2MSC_SockAddr_Struct, &str_ptr, addr);
	
	socket_id = mex_get_socket_form_array(s);
#if defined(MEX_PLATFORM_SPREAD)	
	//展讯小端模式，真机上仅需要转换网络端口，即可下载20120202
#ifndef MEX_BIGEND
	str_ptr->port = htons(str_ptr->port);
#endif
	connect_ret = MEX_SOCK_CONNECT((long)socket_id, str_ptr, sizeof(struct sci_sockaddr));
    if(connect_ret < 0)
	{
		if(EWOULDBLOCK == MEX_SOCK_ERROR((long)socket_id) ||
			EINPROGRESS == MEX_SOCK_ERROR((long)socket_id))
		{
			mex_log(MEX_LOG_SOC, "I: connect would block");
			connect_ret =  MEX_SOC_WOULDBLOCK;
		}
		else
		{
			mex_log(MEX_LOG_SOC, "I: connect failed");
			connect_ret =  MEX_SOC_ERROR;
		}
	}
#elif defined(MEX_PLATFORM_SIMULATOR)
    connect_ret = MEX_SOCK_CONNECT((long)socket_id, str_ptr->ip_addr, str_ptr->port);
#endif
	
	mex_log(MEX_LOG_SOC, "soc_id = %d f = %d  ip = %x p = %d e = %d r = %d", s, str_ptr->family,str_ptr->ip_addr,str_ptr->port, sci_sock_errno((long)socket_id), connect_ret);
	return connect_ret;
	
#endif            
}





int32 MexInterface_Soc_Send(int8 s, uint8 *buf, int32 len, uint8 flags)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return soc_send(s, buf, len, flags);
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	int ret;
	int socket_id = -1;
	socket_id = mex_get_socket_form_array(s);
	if((ret = MEX_SOCK_SEND((long)socket_id, (char*)buf, len, (uint8)flags)) < 0)
	{
		int err = MEX_SOCK_ERROR((long)socket_id);
		
		if(EWOULDBLOCK == err || EINPROGRESS == err)
		{
			mex_log(MEX_LOG_SOC, "soc %d send wouldblock", s);
			return MEX_SOC_WOULDBLOCK;
		}
		else
		{
			mex_log(MEX_LOG_SOC, "soc %d send errno = %d", s, err);
			return MEX_SOC_ERROR;
		}
	}
	else if(ret > 0)
	{
		mex_log(MEX_LOG_SOC, "soc %d send bytes = %d success", s, ret);
		return ret;
	}
	else
	{
		return MEX_SOC_WOULDBLOCK;
	}
	
#endif        
}




int32 MexInteface_Soc_Recv(int8 s, uint8 *buf, int32 len, uint8 flags)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return soc_recv(s, buf, len, flags);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	int ret = -1;
	int socket_id = -1;
	socket_id = mex_get_socket_form_array(s);
	if((ret = MEX_SOCK_RECV((long)socket_id, (char*)buf, len, (uint8)flags)) < 0)
	{
		int err = MEX_SOCK_ERROR((long)socket_id);
		if(EWOULDBLOCK == err || EINPROGRESS == err)
		{
			mex_log(MEX_LOG_SOC, "soc %d recv wouldblock", s);
			return MEX_SOC_WOULDBLOCK;
		}
		else
		{
			mex_log(MEX_LOG_SOC, "soc %d recv errno = %d", s, err);
			return MEX_SOC_ERROR;
		}
	}
	else if(ret > 0)
	{
		mex_log(MEX_LOG_SOC, "soc %d recv bytes = %d", s, ret);
		return ret;
	}
	else
	{
		Mex_Soc_Stat *soc_stat = NULL;
		xSignalHeaderRec *sendSignal = NULL;
		
		MmiCreateSignal(E_MSSC_SOC_EVENT, sizeof(Mex_Soc_Stat) + sizeof(xSignalHeaderRec), (MmiSignalS **)&sendSignal);
		sendSignal->Sender = MexConfig_ConvertSprdDef(E_M2MDC_MET_MEX_SEND);
		soc_stat = (Mex_Soc_Stat*)(sendSignal+1);
		
		soc_stat->socketId = s;
		soc_stat->event      = MEX_SOC_CLOSE;
		
		MmiSendSignal(P_APP, (MmiSignalS *)sendSignal);
		
        mex_log(MEX_LOG_SOC, "Mex_Soc Send Msg s_id = %d event=%d",soc_stat->socketId,
			soc_stat->event);  
		return MEX_SOC_ERROR;
	}
	
#endif            
}

//=================================== Start

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

//处理IMSI请求的回调函数
//参数1：mmi_smu_get_imsi_rsp_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
static uint8 MexInterface_Get_Master_Imsi_Rsp(void *MsgStruct, int mod_src, void *data)
#else
static void MexInterface_Get_Master_Imsi_Rsp(void *MsgStruct)
#endif
{
    extern void MexPhs_Set_Master_Imsi(bool bRes, void *pImsi, int iLen);
	mmi_smu_get_imsi_rsp_struct* msg = (mmi_smu_get_imsi_rsp_struct*)MsgStruct;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C))
		return 0;
#endif
    
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp");
	if(msg->result == 1)
	{
		MexPhs_Set_Master_Imsi(true, msg->imsi + 1, 16);
	}
	else
	{
		MexPhs_Set_Master_Imsi(false, NULL, 0);		
	}
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp end");
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif
	
}

//处理IMSI请求的回调函数
//参数1：mmi_smu_get_imsi_rsp_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
static uint8 MexInterface_Get_Slave_Imsi_Rsp(void *MsgStruct, int mod_src, void *data)
#else
static void MexInterface_Get_Slave_Imsi_Rsp(void *MsgStruct)
#endif
{
    extern void MexPhs_Set_Slave_Imsi(bool bRes, void *pImsi, int iLen);
	mmi_smu_get_imsi_rsp_struct* msg = (mmi_smu_get_imsi_rsp_struct*)MsgStruct;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_2))
		return 0;
#endif
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Slave_Imsi_Rsp");
	
	if(msg->result == 1)
	{
		MexPhs_Set_Slave_Imsi(true, msg->imsi + 1, 16);
	}
	else
	{
		MexPhs_Set_Slave_Imsi(false, NULL, 0);
	}
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Slave_Imsi_Rsp end");
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif
	
}

//发送获取Master IMSI 请求
//参数1：无
//返回：无
void MexInterface_Get_Master_Imsi_Req(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MYQUEUE Message;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req");
	Message.oslSrcId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI);
	Message.oslDestId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C);
	Message.oslMsgId = MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_REQ)/*PRT_GET_IMSI_REQ*/;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr = NULL;
	MexInterface_OslMsgSendQueue(&Message);
	Mex_SetProtocolEventHandler((funcptr)MexInterface_Get_Master_Imsi_Rsp, MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_RSP));
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req end");
}


//发送获取Slave IMSI 请求
//参数1：无
//返回：无
void MexInterface_Get_Slave_Imsi_Req(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MYQUEUE Message;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Slave_Imsi_Req");
	Message.oslSrcId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI);
	Message.oslDestId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_2);
	Message.oslMsgId = MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_REQ)/*PRT_GET_IMSI_REQ*/;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr = NULL;
	MexInterface_OslMsgSendQueue(&Message);
	Mex_SetSlaveProtocolEventHandler((funcptr)MexInterface_Get_Slave_Imsi_Rsp, MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_RSP)/*PRT_GET_IMSI_RSP*/);
	
}
#if (MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__)   //???SIM?
#if (MEX_MTK_CODE_VERSION >= 1024)
static uint8 MexInterface_Get_Third_Imsi_Rsp(void *MsgStruct, int mod_src, void *data)
#else
static void MexInterface_Get_Third_Imsi_Rsp(void *MsgStruct)
#endif
{
    extern void MexPhs_Set_Third_Imsi(bool bRes, void *pImsi, int iLen);
	mmi_smu_get_imsi_rsp_struct* msg = (mmi_smu_get_imsi_rsp_struct*)MsgStruct;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_3))
		return 0;
#endif
    
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp");
	if(msg->result == 1)
	{
		MexPhs_Set_Third_Imsi(true, msg->imsi + 1, 16);
	}
	else
	{
		MexPhs_Set_Third_Imsi(false, NULL, 0);		
	}
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp end");
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif
	
}


//发送获取Master IMSI 请求
//参数1：无
//返回：无
void MexInterface_Get_Third_Imsi_Req(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MYQUEUE Message;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req");
	Message.oslSrcId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI);
	Message.oslDestId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_3);
	Message.oslMsgId = MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_REQ)/*PRT_GET_IMSI_REQ*/;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr = NULL;
	MexInterface_OslMsgSendQueue(&Message);
	Mex_SetProtocolEventHandler((funcptr)MexInterface_Get_Third_Imsi_Rsp, MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_RSP));
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req end");
}
#endif/*(MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__)   */
#ifdef __MET_MEX_SIM_PLUS__
#if (MEX_MTK_CODE_VERSION >= 1024)
static uint8 MexInterface_Get_Fouth_Imsi_Rsp(void *MsgStruct, int mod_src, void *data)
#else
static void MexInterface_Get_Fouth_Imsi_Rsp(void *MsgStruct)
#endif
{
    extern void MexPhs_Set_Fouth_Imsi(bool bRes, void *pImsi, int iLen);
	mmi_smu_get_imsi_rsp_struct* msg = (mmi_smu_get_imsi_rsp_struct*)MsgStruct;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_4))
		return 0;
#endif
    
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp");
	if(msg->result == 1)
	{
		MexPhs_Set_Fouth_Imsi(true, msg->imsi + 1, 16);
	}
	else
	{
		MexPhs_Set_Fouth_Imsi(false, NULL, 0);		
	}
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Rsp end");
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif
	
}


//发送获取Master IMSI 请求
//参数1：无
//返回：无
void MexInterface_Get_Fouth_Imsi_Req(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MYQUEUE Message;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req");
	Message.oslSrcId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_MMI);
	Message.oslDestId = MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_4);
	Message.oslMsgId = MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_REQ)/*PRT_GET_IMSI_REQ*/;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr = NULL;
	MexInterface_OslMsgSendQueue(&Message);
	Mex_SetProtocolEventHandler((funcptr)MexInterface_Get_Fouth_Imsi_Rsp, MexConfig_ConvertMtkDef(E_M2MDC_PRT_GET_IMSI_RSP));
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Imsi_Req end");
}
#endif/*__MET_MEX_SIM_PLUS__*/
#endif

//=================================== End


//处理Master CellInfo 的回调函数
//参数1：mmi_em_status_ind_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
uint8 MexInterface_Get_Master_Cell_Info_Ind(void* inMsg, int mod_src, void *data)
#else
void MexInterface_Get_Master_Cell_Info_Ind(void* inMsg)
#endif	
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)	
    extern void MexPhs_Set_Master_Cell_Info(bool bRes, Mex_Cell_Info* pCellInfo);
	mmi_em_status_ind_struct *msg = (mmi_em_status_ind_struct*) inMsg;
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C))
		return 0;
#endif	
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind result %d",(msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO)));
	if (msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO))	//获取一组基站等信息
	{
        Mex_Cell_Info Mex_Master_Cell_Info = { 0 };
		rr_em_lai_info_struct *data_ptr;
		kal_uint16 mm_pdu_len;
        
		data_ptr = (rr_em_lai_info_struct*) get_pdu_ptr(msg->info, &mm_pdu_len);
		
		Mex_Master_Cell_Info.cell_id= data_ptr->cell_id;
		Mex_Master_Cell_Info.lac = ((data_ptr->lac[0])<<8)+(data_ptr->lac[1]);
		Mex_Master_Cell_Info.mcc[0] = data_ptr->mcc[0]+0x30;
		Mex_Master_Cell_Info.mcc[1] = data_ptr->mcc[1]+0x30;
		Mex_Master_Cell_Info.mcc[2] = data_ptr->mcc[2]+0x30;
		Mex_Master_Cell_Info.mnc[0] = data_ptr->mnc[0]+0x30;
		Mex_Master_Cell_Info.mnc[1] = data_ptr->mnc[1]+0x30;
		Mex_Master_Cell_Info.time_stamp = mex_gettime();
        
		MexInterface_EndCellInfo_Req(E_MEX_SIM_1, (void*)MexInterface_Get_Master_Cell_Info_Ind); //内存增长bug
		MexPhs_Set_Master_Cell_Info(true, &Mex_Master_Cell_Info);
	}
	else
	{
		MexPhs_Set_Master_Cell_Info(false, NULL);
	}
	free_peer_buff(msg->info);
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind end");
	
#if (MEX_MTK_CODE_VERSION >= 0x1024)
	Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND, (funcptr)MexInterface_Get_Master_Cell_Info_Ind);
#else
    Mex_SetProtocolEventHandler((funcptr)NULL, MSG_ID_MMI_EM_STATUS_IND);
#endif
	
    
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
}

//处理Slave CellInfo 的回调函数
//参数1：mmi_em_status_ind_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
uint8 MexInterface_Get_Slave_Cell_Info_Ind(void* inMsg, int mod_src, void *data)
#else
void MexInterface_Get_Slave_Cell_Info_Ind(void* inMsg)
#endif
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)	
    extern void MexPhs_Set_Slave_Cell_Info(bool bRes, Mex_Cell_Info* pCellInfo);
	mmi_em_status_ind_struct *msg = (mmi_em_status_ind_struct*) inMsg;
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_2))
		return 0;
#endif
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Slave_Cell_Info_Ind result %d",(msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO)));
	if (msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO))	//获取一组基站等信息
	{
		Mex_Cell_Info Mex_Slave_Cell_Info = { 0 };
		rr_em_lai_info_struct *data_ptr;
		kal_uint16 mm_pdu_len;
		
		data_ptr = (rr_em_lai_info_struct*) get_pdu_ptr(msg->info, &mm_pdu_len);
		
		Mex_Slave_Cell_Info.cell_id= data_ptr->cell_id;
		Mex_Slave_Cell_Info.lac = ((data_ptr->lac[0])<<8)+(data_ptr->lac[1]);
		Mex_Slave_Cell_Info.mcc[0] = data_ptr->mcc[0]+0x30;
		Mex_Slave_Cell_Info.mcc[1] = data_ptr->mcc[1]+0x30;
		Mex_Slave_Cell_Info.mcc[2] = data_ptr->mcc[2]+0x30;
		Mex_Slave_Cell_Info.mnc[0] = data_ptr->mnc[0]+0x30;
		Mex_Slave_Cell_Info.mnc[1] = data_ptr->mnc[1]+0x30;
		Mex_Slave_Cell_Info.time_stamp = mex_gettime();
        
		MexInterface_EndCellInfo_Req(E_MEX_SIM_2, (void*)MexInterface_Get_Master_Cell_Info_Ind); //内存增长bug
		MexPhs_Set_Slave_Cell_Info(true, &Mex_Slave_Cell_Info);
	}
	else
	{
		MexPhs_Set_Slave_Cell_Info(false, NULL);
	}
	free_peer_buff(msg->info);
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Slave_Cell_Info_Ind result end");
	
#if (MEX_MTK_CODE_VERSION >= 0x1024)
	Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND, (funcptr)MexInterface_Get_Slave_Cell_Info_Ind);
#else
    Mex_SetSlaveProtocolEventHandler((funcptr)NULL, MSG_ID_MMI_EM_STATUS_IND);
#endif
	
    
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    
#endif
}

#if (MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__)   //???SIM?
//处理Third CellInfo 的回调函数
//参数1：mmi_em_status_ind_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
uint8 MexInterface_Get_Third_Cell_Info_Ind(void* inMsg, int mod_src, void *data)
#else
void MexInterface_Get_Third_Cell_Info_Ind(void* inMsg)
#endif	
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)	
    extern void MexPhs_Set_Third_Cell_Info(bool bRes, Mex_Cell_Info* pCellInfo);
	mmi_em_status_ind_struct *msg = (mmi_em_status_ind_struct*) inMsg;
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_3))
		return 0;
#endif	
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind result %d",(msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO)));
	if (msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO))	//获取一组基站等信息
	{
        Mex_Cell_Info Mex_Third_Cell_Info = { 0 };
		rr_em_lai_info_struct *data_ptr;
		kal_uint16 mm_pdu_len;
        
		data_ptr = (rr_em_lai_info_struct*) get_pdu_ptr(msg->info, &mm_pdu_len);
		
		Mex_Third_Cell_Info.cell_id= data_ptr->cell_id;
		Mex_Third_Cell_Info.lac = ((data_ptr->lac[0])<<8)+(data_ptr->lac[1]);
		Mex_Third_Cell_Info.mcc[0] = data_ptr->mcc[0]+0x30;
		Mex_Third_Cell_Info.mcc[1] = data_ptr->mcc[1]+0x30;
		Mex_Third_Cell_Info.mcc[2] = data_ptr->mcc[2]+0x30;
		Mex_Third_Cell_Info.mnc[0] = data_ptr->mnc[0]+0x30;
		Mex_Third_Cell_Info.mnc[1] = data_ptr->mnc[1]+0x30;
		Mex_Third_Cell_Info.time_stamp = mex_gettime();
        
		MexInterface_EndCellInfo_Req(E_MEX_SIM_3, (void*)MexInterface_Get_Master_Cell_Info_Ind); //内存增长bug
		MexPhs_Set_Third_Cell_Info(true, &Mex_Third_Cell_Info);
	}
	else
	{
		MexPhs_Set_Third_Cell_Info(false, NULL);
	}
	free_peer_buff(msg->info);
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind end");
	
#if (MEX_MTK_CODE_VERSION >= 0x1024)
	Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND, (funcptr)MexInterface_Get_Third_Cell_Info_Ind);
#else
    Mex_SetProtocolEventHandler((funcptr)NULL, MSG_ID_MMI_EM_STATUS_IND);
#endif
	
    
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
}
#endif/*(MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__) */
#ifdef __MET_MEX_SIM_PLUS__

//处理Fouth CellInfo 的回调函数
//参数1：mmi_em_status_ind_struct
//返回：无
#if (MEX_MTK_CODE_VERSION >= 1024)
uint8 MexInterface_Get_Fouth_Cell_Info_Ind(void* inMsg, int mod_src, void *data)
#else
void MexInterface_Get_Fouth_Cell_Info_Ind(void* inMsg)
#endif	
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)	
    extern void MexPhs_Set_Fouth_Cell_Info(bool bRes, Mex_Cell_Info* pCellInfo);
	mmi_em_status_ind_struct *msg = (mmi_em_status_ind_struct*) inMsg;
#if (MEX_MTK_CODE_VERSION >= 1024)
	if (mod_src != MexConfig_ConvertMtkDef(E_M2MDC_MOD_L4C_4))
		return 0;
#endif	
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind result %d",(msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO)));
	if (msg->em_info == MexConfig_ConvertMtkDef(E_M2MDC_RR_EM_LAI_INFO))	//获取一组基站等信息
	{
        Mex_Cell_Info Mex_Fouth_Cell_Info = { 0 };
		rr_em_lai_info_struct *data_ptr;
		kal_uint16 mm_pdu_len;
        
		data_ptr = (rr_em_lai_info_struct*) get_pdu_ptr(msg->info, &mm_pdu_len);
		
		Mex_Fouth_Cell_Info.cell_id= data_ptr->cell_id;
		Mex_Fouth_Cell_Info.lac = ((data_ptr->lac[0])<<8)+(data_ptr->lac[1]);
		Mex_Fouth_Cell_Info.mcc[0] = data_ptr->mcc[0]+0x30;
		Mex_Fouth_Cell_Info.mcc[1] = data_ptr->mcc[1]+0x30;
		Mex_Fouth_Cell_Info.mcc[2] = data_ptr->mcc[2]+0x30;
		Mex_Fouth_Cell_Info.mnc[0] = data_ptr->mnc[0]+0x30;
		Mex_Fouth_Cell_Info.mnc[1] = data_ptr->mnc[1]+0x30;
		Mex_Fouth_Cell_Info.time_stamp = mex_gettime();
        
		MexInterface_EndCellInfo_Req(E_MEX_SIM_4, (void*)MexInterface_Get_Master_Cell_Info_Ind); //内存增长bug
		MexPhs_Set_Fouth_Cell_Info(true, &Mex_Fouth_Cell_Info);
	}
	else
	{
		MexPhs_Set_Fouth_Cell_Info(false, NULL);
	}
	free_peer_buff(msg->info);
	//mex_log( MEX_LOG_PHS, "MexPhs_Get_Master_Cell_Info_Ind end");
	
#if (MEX_MTK_CODE_VERSION >= 0x1024)
	Mex_ClearProtocolEventHandler(MSG_ID_MMI_EM_STATUS_IND, (funcptr)MexInterface_Get_Fouth_Cell_Info_Ind);
#else
    Mex_SetProtocolEventHandler((funcptr)NULL, MSG_ID_MMI_EM_STATUS_IND);
#endif
	
    
#if (MEX_MTK_CODE_VERSION >= 1024)
	return 1;
#endif	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
}

#endif/*__MET_MEX_SIM_PLUS__*/
//===================================================================== Outgoing Call Start

typedef enum
{
	E_MOS_IDLE       = 0,        // 空闲
		E_MOS_DIALING    = 1,        // 正在拔号
		E_MOS_ACTIVE     = 2,        // 活动
		E_MOS_CONNECTED  = 3,        // 通话中
		E_MOS_DISCONNECT = 4,        // 连接断开
		E_MOS_STATE_MAX
} E_MEX_OUTCALL_STATE;


typedef struct
{
	E_MEX_OUTCALL_STATE        m_eState;       // Outgoing call state
	E_MEX_Sim_Card_ID          m_eSimId;       // SimId
	int32                      m_iCallId;      // call index
	bool                       m_bDtmfStart;   // Dtmf sending?
	
	int32                      m_iAppId;       // app id
	MexOutCallCB               m_pCallBack;    // callback
} MexPhs_OutCall_Context;


MexPhs_OutCall_Context *g_mex_outcall_handle = NULL;

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
bool MexPhs_IsOutCalling(void)
{
	if(g_mex_outcall_handle != NULL)
	{
		mex_log(MEX_LOG_PHS, "MexPhs_IsOutCalling::g_mex_outcall_handle != NULL");
		return (g_mex_outcall_handle->m_eState != E_MOS_IDLE);   
	}
	mex_log(MEX_LOG_PHS, "MexPhs_IsOutCalling::g_mex_outcall_handle == NULL");
	
	return false;
}


void MexPhs_GetCellInfo(Mex_Cell_Info *cell_info, E_MEX_Phs_State *state, E_MEX_Sim_Card_ID sim)
{
	
	//展讯平台获取基站信息的接口是同步模式
	//在这里可以直接获取
	//平台定义的基站信息总共有四个部分组成
	//分别是cellid, lac, mcc, mnc, time_stamp
	//其中cell_id和lac可从MNPHONE_GetRSSIEx获取
	//mcc和mnc可以从imsi中进行提取
	//time_stamp可以从系统时间中获得
	char imsi_str[(MN_MAX_IMEI_LENGTH<<1)+2] = {0};
	
	MN_IMSI_T			  imsi = {0};
	MN_PHONE_SCELL_MEAS_T cell = {0};
	
#if (MEX_SDK_VER >= 1018)
	sim = MexConfig_ConvertSimCard(sim);
#endif
	
	if(E_MEX_SIM_1 == sim)
	{		
		*state = E_MEX_PHS_PROCESSING;
		
		imsi = MNSIM_GetImsiEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1));
		cell = MNPHONE_GetRSSIEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1));
		MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imsi.imsi_val,MN_MAX_IMEI_LENGTH<<1,(char*)imsi_str);
		
	}
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)    
	else if(E_MEX_SIM_2 == sim)
	{
		*state = E_MEX_PHS_PROCESSING;
		
		imsi = MNSIM_GetImsiEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2));
		cell = MNPHONE_GetRSSIEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2));
		MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imsi.imsi_val,MN_MAX_IMEI_LENGTH<<1,(char*)imsi_str);
	}
#endif
#if (MEX_SDK_VER >= 1018)||defined (__MET_MEX_SIM_PLUS__)   //支持三SIM卡
	else if(E_MEX_SIM_3 == sim)
	{
		*state = E_MEX_PHS_PROCESSING;
		
		imsi = MNSIM_GetImsiEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3));
		cell = MNPHONE_GetRSSIEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3));
		MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imsi.imsi_val,MN_MAX_IMEI_LENGTH<<1,(char*)imsi_str);
	}
#endif //MEX_THIRD_SIM
#ifdef __MET_MEX_SIM_PLUS__
	else if(E_MEX_SIM_4 == sim)
	{
		*state = E_MEX_PHS_PROCESSING;
		
		imsi = MNSIM_GetImsiEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_4));
		cell = MNPHONE_GetRSSIEx(MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_4));
		MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imsi.imsi_val,MN_MAX_IMEI_LENGTH<<1,(char*)imsi_str);
	}
#endif/*__MET_MEX_SIM_PLUS__*/
	memset(cell_info,0,sizeof(Mex_Cell_Info));
	memcpy(cell_info->mcc, imsi_str+1, 3);
	memcpy(cell_info->mnc, imsi_str+4, 2);
	cell_info->lac     		    = cell.lac;
	cell_info->cell_id          = cell.cell_id;
	cell_info->time_stamp       = mex_gettime();
	*state = E_MEX_PHS_COMPLETED;
	
}


#endif

static void MexPhs_OutCallNotifyApp(E_MEX_OUTCALL_EVENT event)
{
	int32 appId = mex_GetCurrentApp();
	if((NULL == g_mex_outcall_handle)||(NULL == g_mex_outcall_handle->m_pCallBack))
	{
        return;
	}
	
	mex_SetCurrentApp(g_mex_outcall_handle->m_iAppId);
	(*g_mex_outcall_handle->m_pCallBack)(g_mex_outcall_handle, event);
	mex_SetCurrentApp(appId);
}


static void MexPhs_OutCallCB(void *info)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	mmi_ss_parsing_string_rsp_struct *rsp;
	rsp = (mmi_ss_parsing_string_rsp_struct *)info;
	
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCCB hdl NUL");
		return;
	}
	
	mex_log(MEX_LOG_PHS, "OCCB res flag:%d", rsp->result.flag);
	mex_log(MEX_LOG_PHS, "OCCB res cause:%d", rsp->result.cause);
	
	mex_log(MEX_LOG_PHS, "OCCB inf num:%s len:%d type:%d", rsp->info.addr.number, rsp->info.addr.length, rsp->info.addr.type);
	mex_log(MEX_LOG_PHS, "OCCB inf type:%d", rsp->info.call_type);
	mex_log(MEX_LOG_PHS, "OCCB inf clir_f:%d", rsp->info.clir_flag);
	mex_log(MEX_LOG_PHS, "OCCB inf cug_f:%d", rsp->info.cug_flag);
	mex_log(MEX_LOG_PHS, "OCCB inf op:%d", rsp->info.opcode);
	
	if (rsp->result.flag == 1)
	{
		// SS_PARSING_STRING success
		MexPhs_OutCallNotifyApp(E_MOE_SS_PARSING_STRING_SUCCESS);
	}
	else
	{
		// SS_PARSING_STRING fail
		MexPhs_OutCallNotifyApp(E_MOE_SS_PARSING_STRING_FAIL);
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCCB hdl NUL");
		return;
	}
	
	if (info != NULL)
	{
		// SS_PARSING_STRING success
		MexPhs_OutCallNotifyApp(E_MOE_SS_PARSING_STRING_SUCCESS);
	}
	else
	{
		// SS_PARSING_STRING fail
		MexPhs_OutCallNotifyApp(E_MOE_SS_PARSING_STRING_FAIL);
	}
#endif    
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
static void MexPhs_OutCallEventCB(void *info)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)  
void MexPhs_OutCallEventCB(void *info)
#endif
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	mmi_cc_dial_ind_struct *rsp;
	rsp = (mmi_cc_dial_ind_struct *)info;
	
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCECB hdl NUL");
		return;
	}
	
	mex_log(MEX_LOG_PHS, "OCECB res f:%d", rsp->result.flag);
	mex_log(MEX_LOG_PHS, "OCECB res c:%d", rsp->result.cause);
	mex_log(MEX_LOG_PHS, "OCECB cid:%d", rsp->call_id);
	
	if (rsp->result.flag == L4C_OK)
	{
		g_mex_outcall_handle->m_iCallId = rsp->call_id;
		g_mex_outcall_handle->m_eState = E_MOS_ACTIVE;
		
		//电话已拔通
		MexPhs_OutCallNotifyApp(E_MOE_ACTIVE_CALL_SUCCESS);
	}
	else
	{
		//电话未拔通
		MexPhs_OutCallNotifyApp(E_MOE_ACTIVE_CALL_FAIL);
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	APP_MN_SYNC_IND_T *rsp = (APP_MN_SYNC_IND_T*)info;
	
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCECB hdl NUL");
		return;
	}
	
	if (rsp != NULL)
	{
		g_mex_outcall_handle->m_iCallId = 0;
		g_mex_outcall_handle->m_eState = E_MOS_ACTIVE;
		
		//电话已拔通
		MexPhs_OutCallNotifyApp(E_MOE_ACTIVE_CALL_SUCCESS);
	}
	else
	{
		//电话未拔通
		MexPhs_OutCallNotifyApp(E_MOE_ACTIVE_CALL_FAIL);
	}
#endif
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
static void MexPhs_OutCallConnectedCB(void *info)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
void MexPhs_OutCallConnectedCB(void *info)
#endif

{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	mmi_cc_call_connect_ind_struct *rsp;
	rsp = (mmi_cc_call_connect_ind_struct *)info;
	
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCCCB hdl NUL");
		return;
	}
	
	mex_log(MEX_LOG_PHS, "OCCCB cid:%d", rsp->call_id);
	mex_log(MEX_LOG_PHS, "OCCCB type:%d", rsp->call_type);
	mex_log(MEX_LOG_PHS, "OCCCB num:%s len:%d type:%d", rsp->num.number, rsp->num.length, rsp->num.type);
	mex_log(MEX_LOG_PHS, "OCCCB sub num:%s len:%d type:%d ", rsp->sub_addr.number, rsp->sub_addr.length, rsp->sub_addr.type);
	
	g_mex_outcall_handle->m_iCallId = rsp->call_id;
	g_mex_outcall_handle->m_eState = E_MOS_CONNECTED;
	
	//电话已连接
	MexPhs_OutCallNotifyApp(E_MOE_CONNECTED);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	APP_MN_SETUP_COMPLETE_CNF_T *rsp = (APP_MN_SETUP_COMPLETE_CNF_T *)info;
	
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCCCB hdl NUL");
		return;
	}
	
	mex_log(MEX_LOG_PHS, "OCCCB cid:%d", rsp->call_id);
	
	g_mex_outcall_handle->m_iCallId = rsp->call_id;
	g_mex_outcall_handle->m_eState = E_MOS_CONNECTED;
	
	//电话已连接
	MexPhs_OutCallNotifyApp(E_MOE_CONNECTED);
#endif    
}

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
static void MexPhs_NetworkCallDropped(void *info)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
void MexPhs_NetworkCallDropped(void *info)
#endif
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	mmi_cc_call_release_ind_struct *rsp = (mmi_cc_call_release_ind_struct *)info;
	
	mex_log(MEX_LOG_PHS, "NCD cid:%d", rsp->call_id);
	mex_log(MEX_LOG_PHS, "NCD c:%d", rsp->cause);
	if (g_mex_outcall_handle != NULL)
	{
		g_mex_outcall_handle->m_eState = E_MOS_IDLE;
#if MEX_SDK_VER >= 1011
		MexPhs_OutCallNotifyApp(E_MOE_CALL_RELEASED);
#endif
	}
	// It is very important to re-init CM
	mex_log(MEX_LOG_PHS, "NCD ReinitCM begin");
#if (MEX_MTK_CODE_VERSION < 0x0932)
	{
		ReinitalizeCallManagement();
	}
#elif (MEX_MTK_CODE_VERSION >= 0x0952)
	{
		extern void srv_gcall_phdlr_dial_ind(void *info, int mod_src);
		extern void srv_gcall_phdlr_call_connect_ind(void *info, int mod_src);
		extern void srv_gcall_phdlr_call_release_ind(void *info, int mod_src);
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_DIAL_IND, (PsIntFuncPtr)srv_gcall_phdlr_dial_ind, MMI_FALSE);
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_CALL_CONNECT_IND, (PsIntFuncPtr)srv_gcall_phdlr_call_connect_ind, MMI_FALSE);
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_CALL_RELEASE_IND, (PsIntFuncPtr)srv_gcall_phdlr_call_release_ind, MMI_FALSE);
#else
		Mex_SetProtocolEventHandler((void *)srv_gcall_phdlr_dial_ind, MSG_ID_MMI_CC_DIAL_IND);
		Mex_SetProtocolEventHandler((void *)srv_gcall_phdlr_call_connect_ind, PRT_OUTGOINGCALL_CONNECTED);
		Mex_SetProtocolEventHandler((void *)srv_gcall_phdlr_call_release_ind, MSG_ID_MMI_CC_CALL_RELEASE_IND);
#ifdef __MMI_DUAL_SIM_MASTER__
		Mex_SetSlaveProtocolEventHandler((void *)srv_gcall_phdlr_dial_ind, MSG_ID_MMI_CC_DIAL_IND);
		Mex_SetSlaveProtocolEventHandler((void *)srv_gcall_phdlr_call_connect_ind, PRT_OUTGOINGCALL_CONNECTED);
		Mex_SetSlaveProtocolEventHandler((void *)srv_gcall_phdlr_call_release_ind, MSG_ID_MMI_CC_CALL_RELEASE_IND);
#endif /* __MMI_DUAL_SIM_MASTER__ */ 
#endif /* MEX_MTK_CODE_VERSION */
	}
#else
	// TODO: 
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if MEX_SDK_VER >= 1011
	mex_log(MEX_LOG_PHS, "MexPhs_NetworkCallDropped::E_MOE_CALL_RELEASED");
	g_mex_outcall_handle->m_eState = E_MOS_DISCONNECT;
	MexPhs_OutCallNotifyApp(E_MOE_CALL_RELEASED);
#endif
#endif    
	mex_log(MEX_LOG_PHS, "NCD ReinitCM end");
	
}


void MexPhs_OutCallEndedCB(void *info)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	mex_log(MEX_LOG_PHS, "OCECB");
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	int32 appId = mex_GetCurrentApp();
	if((NULL == g_mex_outcall_handle)||(NULL == g_mex_outcall_handle->m_pCallBack))
	{
        return;
	}
	
	mex_SetCurrentApp(g_mex_outcall_handle->m_iAppId);
	g_mex_outcall_handle->m_eState = E_MOS_IDLE;
	mex_free(g_mex_outcall_handle);       /*lint !e611 */
	g_mex_outcall_handle = NULL;
	mex_SetCurrentApp(appId);    
#endif   
}


static void MexPhs_OutCallHangUpCB(void *info)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	mmi_cc_call_release_ind_struct *rsp;
	rsp = (mmi_cc_call_release_ind_struct *)info;
	
	mex_log(MEX_LOG_PHS, "OCHCB cid:%d", rsp->call_id);
	mex_log(MEX_LOG_PHS, "OCHCB c:%d", rsp->cause);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	
#endif    
}


void MexPhs_CheckFailureChld(void *info)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	int16 result;
	mmi_cc_chld_rsp_struct *rsp = (mmi_cc_chld_rsp_struct*) info;
	
	mex_log(MEX_LOG_PHS, "OCCFC res:%d", rsp->result);
	mex_log(MEX_LOG_PHS, "OCCFC c:%d", rsp->cause);
	mex_log(MEX_LOG_PHS, "OCCFC op:%d", rsp->opcode);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
#endif    
}



MEX_OUTCALL_HANDLE MexPhs_OutCallStart(E_MEX_Sim_Card_ID eSimId, const char* number, MexOutCallCB callback)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	module_type mod_id;
	
	// ucm is busy?
#if (MEX_MTK_CODE_VERSION < 0x0952)
	if (mmi_ucm_is_busy())
	{
		mex_log(MEX_LOG_PHS, "OCS E: busy");
		return NULL;
	}
#else
	if (MetMex_GCall_IsBusy())
	{
		mex_log(MEX_LOG_PHS, "OCS E: busy");
		return NULL;
	}
#endif
	
	
	// check fight mode
	
	// check ucm pending action
	
	// check simId
	switch (eSimId)
	{
	case E_MEX_SIM_1:
		mod_id = MOD_L4C;
		break;
	case E_MEX_SIM_2:
#if defined(__MMI_DUAL_SIM_MASTER__)
		mod_id = MOD_L4C_2;
		break;
#else
		return NULL;
#endif
#ifdef __MET_MEX_SIM_PLUS__
#if (GEMINI_PLUS >= 3)
	case E_MEX_SIM_3:
		mod_id = MOD_L4C_3;
		break;
#endif
#if (GEMINI_PLUS >= 4)
		
	case E_MEX_SIM_4:
		mod_id = MOD_L4C_4;
		break;
#endif
#endif/*__MET_MEX_SIM_PLUS__*/
	default:
		mex_log(MEX_LOG_PHS, "OCS E: sim:%d", eSimId);
		return NULL;
	}
	
	// check mex outcall busy?
	if (g_mex_outcall_handle != NULL)
	{
		mex_log(MEX_LOG_PHS, "OCS E: busy2 state:%d", g_mex_outcall_handle->m_eState);
		return NULL;
	}
	
	// create mex outcall handle
	g_mex_outcall_handle = mex_malloc(sizeof(MexPhs_OutCall_Context));
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCS E: no mem");
		return NULL;
	}
	
	// init mex outcall handle
	if(NULL == callback)
	{
		mex_log(MEX_LOG_PHS, "OCS E: NULL callback");
        return NULL;
	}
	g_mex_outcall_handle->m_eState = E_MOS_DIALING;
	g_mex_outcall_handle->m_eSimId = eSimId;
#if (MEX_MTK_CODE_VERSION >= 0x0952)
	g_mex_outcall_handle->m_iCallId = SRV_UCM_INVALID_ID;
#else
	g_mex_outcall_handle->m_iCallId = MMI_UCM_INVALID_ID;
#endif
	g_mex_outcall_handle->m_bDtmfStart = false;
	g_mex_outcall_handle->m_iAppId = mex_GetCurrentApp();
	g_mex_outcall_handle->m_pCallBack = callback;
	
	//Send message to MOD_L4C or MOD_L4C_2
	{
		MYQUEUE Message;
		mmi_ss_parsing_string_req_struct *strParse;
		
		/* PARSE_SS_REQ */
		Message.oslSrcId = MOD_MMI;
		Message.oslMsgId = PRT_USSDVSS_REQ;
		Message.oslPeerBuffPtr = NULL;
		
		strParse = (mmi_ss_parsing_string_req_struct*) OslConstructDataPtr(sizeof(mmi_ss_parsing_string_req_struct));
		strParse->length = (U8) strlen(number);
		memcpy(strParse->input, number, strParse->length);
		
		Message.oslDestId = mod_id;
		Message.oslDataPtr = (oslParaType*) strParse;
		
#if (MEX_MTK_CODE_VERSION >= 0x1032)
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_SS_PARSING_STRING_RSP, (PsIntFuncPtr)MexPhs_OutCallCB, MMI_FALSE);
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_DIAL_IND, (PsIntFuncPtr)MexPhs_OutCallEventCB, MMI_FALSE);
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_CALL_CONNECT_IND, (PsIntFuncPtr)MexPhs_OutCallConnectedCB, MMI_FALSE);
		mmi_frm_set_protocol_event_handler(MSG_ID_MMI_CC_CALL_RELEASE_IND, (PsIntFuncPtr)MexPhs_NetworkCallDropped, MMI_FALSE);
#else
		
		if (eSimId == E_MEX_SIM_1)
		{
			Mex_SetProtocolEventHandler((void *)MexPhs_OutCallCB, PRT_USSDVSS_RSP);
			Mex_SetProtocolEventHandler((void *)MexPhs_OutCallEventCB, PRT_OUTGOINGCALL_EVENT_RSP);
			Mex_SetProtocolEventHandler((void *)MexPhs_OutCallConnectedCB, PRT_OUTGOINGCALL_CONNECTED);
			Mex_SetProtocolEventHandler((void *) MexPhs_NetworkCallDropped, PRT_NWRK_CALL_RELEASE);
		}
		else
		{
			Mex_SetSlaveProtocolEventHandler((void *)MexPhs_OutCallCB, PRT_USSDVSS_RSP);
			Mex_SetSlaveProtocolEventHandler((void *)MexPhs_OutCallEventCB, PRT_OUTGOINGCALL_EVENT_RSP);
			Mex_SetSlaveProtocolEventHandler((void *)MexPhs_OutCallConnectedCB, PRT_OUTGOINGCALL_CONNECTED);
			Mex_SetSlaveProtocolEventHandler((void *) MexPhs_NetworkCallDropped, PRT_NWRK_CALL_RELEASE);
		}
#endif
		
		OslMsgSendExtQueue(&Message);
		return g_mex_outcall_handle;
	}
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	CC_RESULT_E   eErrorCode;
	MN_DUAL_SYS_E eSimType;
	CC_CALL_SIM_TYPE_E  eCallSimType;    
	// ucm is busy?
	
#if (MEX_SDK_VER >= 1018)
	eSimId = MexConfig_ConvertSimCard(eSimId);
#endif
	
	if ( MMIAPICC_IsInState(CC_IN_CALL_STATE)
		||MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
		||MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) 
		||MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))        
	{
		mex_log(MEX_LOG_PHS, "OCS E: busy");
		return NULL;
	}
	
	// check fight mode
	
	// check ucm pending action
	
	// check simId
	switch (eSimId)
	{    
	case E_MEX_SIM_1:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
		eCallSimType = MexConfig_ConvertSprdDef(E_S2MDC_CC_SIM1_CALL);
		break;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)        
	case E_MEX_SIM_2:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);            
		eCallSimType = MexConfig_ConvertSprdDef(E_S2MDC_CC_SIM2_CALL);
		break;
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
	case E_MEX_SIM_3:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
		eCallSimType = MexConfig_ConvertSprdDef(E_S2MDC_CC_SIM3_CALL);
		break;
#endif //MEX_THIRD_SIM
#endif        
		
	default:
		mex_log(MEX_LOG_PHS, "OCS E: sim:%d", eSimId);
		return NULL;
	}
	
	// check mex outcall busy?
	if (g_mex_outcall_handle != NULL)
	{
		mex_log(MEX_LOG_PHS, "OCS E: busy2 state:%d", g_mex_outcall_handle->m_eState);
		return NULL;
	}
	
	// create mex outcall handle
	g_mex_outcall_handle = mex_malloc(sizeof(MexPhs_OutCall_Context));       /*lint !e611 */
	if (g_mex_outcall_handle == NULL)
	{
		mex_log(MEX_LOG_PHS, "OCS E: no mem");
		return NULL;
	}
	
	// init mex outcall handle
	if(NULL == callback)
	{
		mex_log(MEX_LOG_PHS, "OCS E: NULL callback");
        return NULL;
	}
	g_mex_outcall_handle->m_eState = E_MOS_DIALING;
	g_mex_outcall_handle->m_eSimId = eSimId;
	g_mex_outcall_handle->m_iCallId = -1;
	g_mex_outcall_handle->m_bDtmfStart = false;
	g_mex_outcall_handle->m_iAppId = mex_GetCurrentApp();
	g_mex_outcall_handle->m_pCallBack = callback;
	
	eErrorCode = MMIAPICC_MakeCall(
		eSimType,
		(uint8*)number,
		mex_strlen(number),
		NULL,
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)
        NULL,
#endif
		eCallSimType,
		CC_CALL_NORMAL
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
		,NULL
#endif
		);
	
	if(eErrorCode == CC_RESULT_SUCCESS)
	{   
		
		MexPhs_OutCallCB((void *)g_mex_outcall_handle);
		return g_mex_outcall_handle;
	}
	
	MexPhs_OutCallCB(NULL);
	mex_free(g_mex_outcall_handle);       /*lint !e611 */
	g_mex_outcall_handle = NULL;
	
	return NULL;
	
#endif        
}



static void MexPhs_MakePsAthRequest(void *callBack)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MYQUEUE Message;
	mmi_cc_ath_req_struct *athReq;
	
	
	/* ATH_REQ */
	Message.oslSrcId = MOD_MMI;
	Message.oslMsgId = PRT_ATH_REQ;
	Message.oslPeerBuffPtr = NULL;
	athReq = (mmi_cc_ath_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_ath_req_struct));
	Message.oslDataPtr = (oslParaType*) athReq;
	Message.oslDestId = MOD_L4C;
#if defined(__MMI_DUAL_SIM_MASTER__)
	if(g_mex_outcall_handle->m_eSimId == E_MEX_SIM_2)
	{
		Message.oslDestId = MOD_L4C_2;
	}
#endif
	
	athReq->op_code = L4C_DISCONNECT_MO;
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	athReq->source_id = 0;
#else
	athReq->source_id = LMMI_SRC;
#endif
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	mmi_frm_set_protocol_event_handler(PRT_ATH_REQ_SUCCESS, (PsIntFuncPtr)callBack, MMI_FALSE);
#else
#if defined(__MMI_DUAL_SIM_MASTER__)
	if(g_mex_outcall_handle->m_eSimId == E_MEX_SIM_2)
	{
		Mex_SetSlaveProtocolEventHandler((void * )callBack, PRT_ATH_REQ_SUCCESS);
	}
	else
#endif
	{
		Mex_SetProtocolEventHandler((void *) callBack, PRT_ATH_REQ_SUCCESS);
	}	
#endif
	
	OslMsgSendExtQueue(&Message);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
#endif    
}




static void MexPhs_MakePsEndSelectiveCall(void *callBack, int handle)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MYQUEUE Message;
	mmi_cc_chld_req_struct *hangupReq;
	
	Message.oslSrcId = MOD_MMI;
	Message.oslMsgId = PRT_CALLENDSPECIFIC_EVENT;
	
	hangupReq = (mmi_cc_chld_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_chld_req_struct));
	hangupReq->opcode = CSMCC_REL_SPECIFIC_CALL;
	hangupReq->call_id = (U8) handle;
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	hangupReq->source_id = 0;
#else
	hangupReq->source_id = LMMI_SRC;
#endif
	Message.oslDataPtr = (oslParaType*) hangupReq;
	Message.oslPeerBuffPtr = NULL;
	
	Message.oslDestId = MOD_L4C;
#if defined(__MMI_DUAL_SIM_MASTER__)
	if(g_mex_outcall_handle->m_eSimId == E_MEX_SIM_2)
	{
		Message.oslDestId = MOD_L4C_2;
	}
#endif
	
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	mmi_frm_set_protocol_event_handler(PRT_CALLENDSPECIFIC_SUCCESS, (PsIntFuncPtr)callBack, MMI_FALSE);
	mmi_frm_set_protocol_event_handler(PRT_END_CHLD_RSP, (PsIntFuncPtr)MexPhs_CheckFailureChld, MMI_FALSE);
#else
#if defined(__MMI_DUAL_SIM_MASTER__)
	if(g_mex_outcall_handle->m_eSimId == E_MEX_SIM_2)
	{
		Mex_SetSlaveProtocolEventHandler((void *) callBack, PRT_CALLENDSPECIFIC_SUCCESS);
		Mex_SetSlaveProtocolEventHandler((void *) MexPhs_CheckFailureChld, PRT_END_CHLD_RSP);
	}
	else
#endif
	{
		Mex_SetProtocolEventHandler((void *) callBack, PRT_CALLENDSPECIFIC_SUCCESS);
		Mex_SetProtocolEventHandler((void *) MexPhs_CheckFailureChld, PRT_END_CHLD_RSP);
	}	
#endif
	
	OslMsgSendExtQueue(&Message);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
#endif
}


bool MexPhs_OutCallHangUp(MEX_OUTCALL_HANDLE handle)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	
	switch (app_handle->m_eState)
	{
	case E_MOS_DIALING:
#if defined(MEX_PLATFORM_MTK) || (MEX_MTK_CODE_VERSION < 0x1032)
		MexPhs_MakePsAthRequest((void *) MexPhs_OutCallEndedCB);
#endif
		break;
	case E_MOS_ACTIVE:
	case E_MOS_CONNECTED:
		MexPhs_MakePsEndSelectiveCall((void *) MexPhs_OutCallHangUpCB, app_handle->m_iCallId);
		break;
	default:
		mex_log(MEX_LOG_PHS, "OCH E: state:%d", app_handle->m_eState);
		return false;
	}
	
	app_handle->m_eState = E_MOS_IDLE;
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	mmi_frm_set_protocol_event_handler(PRT_NWRK_CALL_RELEASE, (PsIntFuncPtr)MexPhs_NetworkCallDropped, MMI_FALSE);	
#else
	if (app_handle->m_eSimId == E_MEX_SIM_1)
		Mex_SetProtocolEventHandler((void *) MexPhs_NetworkCallDropped, PRT_NWRK_CALL_RELEASE);
	else
		Mex_SetSlaveProtocolEventHandler((void *) MexPhs_NetworkCallDropped, PRT_NWRK_CALL_RELEASE);
#endif
	
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	CC_RESULT_E eErrorCode;
	
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	eErrorCode = MMIAPICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
	eErrorCode = MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);     
#endif
	if(eErrorCode == CC_RESULT_SUCCESS)
	{
		return true;
	}
	
	return false;
#endif
}


bool MexPhs_OutCallFinish(MEX_OUTCALL_HANDLE handle)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}
	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCF E: hdl err");
		return false;
	}
	
	// check app id
	if (app_handle->m_iAppId != mex_GetCurrentApp())
	{
		mex_log(MEX_LOG_PHS, "OCF E: AppId err");
		return false;
	}
	
	// hang up if needed
	MexPhs_OutCallHangUp(handle);
	
	mex_free(handle);
	g_mex_outcall_handle = NULL;
	
	// re-init outgoing call
	mex_log(MEX_LOG_PHS, "OCF ReinitOC begin");
#if (MEX_MTK_CODE_VERSION < 0x1032)
	ReinitializeOutgoingCall();
#endif
#if (MEX_MTK_CODE_VERSION >= 0x0812) && (MEX_MTK_CODE_VERSION < 0x0952)
#ifdef __MMI_DUAL_SIM_SINGLE_CALL_SLAVE__
	MTPNP_PFAL_initalizeCC(); // re-init sim2
#endif
#endif /* MEX_PLATFORM__FOR_MTK23C MEX_PLATFORM_FOR_MTK6253 */
	mex_log(MEX_LOG_PHS, "OCF ReinitOC end");
	
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}
	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCF E: hdl err");
		return false;
	}
	
	// check app id
	if (app_handle->m_iAppId != mex_GetCurrentApp())
	{
		mex_log(MEX_LOG_PHS, "OCF E: AppId err");
		return false;
	}
	
	if (g_mex_outcall_handle->m_eState == E_MOS_DISCONNECT)
	{
		mex_log(MEX_LOG_PHS, "OCF E: IDLE");
		return false;
	}
	// hang up if needed
	MexPhs_OutCallHangUp(handle);
	
	return true;
#endif
}


bool MexPhs_OutCallDtmfBegin(MEX_OUTCALL_HANDLE handle, char keycode)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}

	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCDB E: hdl err");
		return false;
	}
	
	// check Dtmf state
	if (app_handle->m_bDtmfStart)
	{
		mex_log(MEX_LOG_PHS, "OCDB E: already begin");
		return false;
	}
	
	// check keycode
	if ((keycode >= '0' && keycode <= '9') || keycode == '*' || keycode == '#')
	{
		// do nothing
	}
	else
	{
		mex_log(MEX_LOG_PHS, "OCDB E: key:%c", keycode);
		return false;
	}
	app_handle->m_bDtmfStart = true;
	
	// send dtmf start req
	{
		MYQUEUE Message;
		mmi_cc_start_dtmf_req_struct *dtmf;
		
		Message.oslSrcId = MOD_MMI;
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(app_handle->m_eSimId == E_MEX_SIM_2)
		{
			Message.oslDestId = MOD_L4C_2;
		}
		else
#endif
		{
			Message.oslDestId = MOD_L4C;
		}
		Message.oslMsgId = PRT_DTMF_START_REQ;
		
		dtmf = (mmi_cc_start_dtmf_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_start_dtmf_req_struct));
		dtmf->digit = keycode;    /* the actuall digit */
		
		Message.oslDataPtr = (oslParaType*) dtmf;
		Message.oslPeerBuffPtr = NULL;
		OslMsgSendExtQueue(&Message);
	}
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
	MN_DUAL_SYS_E eSimType;
	
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;    
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}
	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCDB E: hdl err");
		return false;
	}
	
	// check Dtmf state
	if (app_handle->m_bDtmfStart)
	{
		mex_log(MEX_LOG_PHS, "OCDB E: already begin");
		return false;
	}
	
	// check keycode
	if ((keycode >= '0' && keycode <= '9') || keycode == '*' || keycode == '#')
	{
		// do nothing
	}
	else
	{
		mex_log(MEX_LOG_PHS, "OCDB E: key:%c", keycode);
		return false;
	}
	app_handle->m_bDtmfStart = true;
	
	// check simId
#if (MEX_SDK_VER >= 1018)    
	switch ((int)MexConfig_ConvertSimCard(app_handle->m_eSimId))
#else    
		switch (app_handle->m_eSimId)
#endif
	{    
	case E_MEX_SIM_1:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
		break;
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)        
	case E_MEX_SIM_2:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);            
		break;
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
	case E_MEX_SIM_3:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
		break;
#endif //MEX_THIRD_SIM
#endif        
	default:
		eSimType = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
		break;
		
	}
	
	MMIAPICC_SendDtmfByChar(eSimType,keycode);    
	
	return true;
#endif
}

bool MexPhs_OutCallDtmfEnd(MEX_OUTCALL_HANDLE handle)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}

	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCDE E: hdl err");
		return false;
	}
	
	// check Dtmf state
	if (!app_handle->m_bDtmfStart)
	{
		mex_log(MEX_LOG_PHS, "OCDE E: already end");
		return false;
	}
	app_handle->m_bDtmfStart = false;
	
	// send dtmf stop req
	{
		MYQUEUE Message;
		
		Message.oslSrcId = MOD_MMI;
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(app_handle->m_eSimId == E_MEX_SIM_2)
		{
			Message.oslDestId = MOD_L4C_2;
		}
		else
#endif
		{
			Message.oslDestId = MOD_L4C;
		}
		Message.oslMsgId = PRT_DTMF_STOP_REQ;
		
		Message.oslDataPtr = NULL;
		Message.oslPeerBuffPtr = NULL;
		OslMsgSendExtQueue(&Message);
	}
	return true;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MexPhs_OutCall_Context *app_handle = (MexPhs_OutCall_Context *)handle;
	if(NULL == handle)
	{
        mex_log(MEX_LOG_SYSTEM,"OCF E: NULL handle");
        return false;
	}
	
	// check handle
	if (app_handle != g_mex_outcall_handle)
	{
		mex_log(MEX_LOG_PHS, "OCDE E: hdl err");
		return false;
	}
	
	// check Dtmf state
	if (!app_handle->m_bDtmfStart)
	{
		mex_log(MEX_LOG_PHS, "OCDE E: already end");
		return false;
	}
	app_handle->m_bDtmfStart = false;
	
	return true;
#endif
}

//===================================================================== Outgoing Call End

//===================================================================== Sms Read Start
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
uint32 MexPhs_GetSaveSmsBox()
{
	if (g_mex_savesms_context.callback == NULL)
	{
		g_mex_savesms_context.processing = false;
		return MMISMS_NO_OPER;
	}
	
	//判断是否为系统在询问保存到哪个信箱
	switch(g_mex_savesms_context.msgBox)
	{                  
	case E_MPSM_OUTBOX:
		return MMISMS_SEND_SAVE_SMS; 
	case E_MPSM_UNSENT:
		return MMISMS_SAVE_SENDFAIL_SMS;
	case E_MPSM_DRAFTS:     
		return MMISMS_SAVE_SMS;
	case E_MPSM_INBOX:     
		return MMISMS_SAVE_SMS;
	default:
		return MMISMS_NO_OPER;
	}
}
#endif
#ifdef MEX_USE_ASSERT


#endif // MEX_USE_ASSERT


//===================================================================== Sms Read End
//摄像头操作
#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
bool g_bMexDbgCameraDataReq = false;
static gdi_handle g_hPreviewLayer = 0;
#endif    

//摄像头加电操作
bool MexInterface_Camera_PowerOn(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if (MEX_MTK_CODE_VERSION < 0x1112)
	if(mdi_camera_power_on() == MDI_RES_CAMERA_SUCCEED)
		return true;
#else
	if(mdi_camera_power_on(ASM_ANONYMOUS_ID) == MDI_RES_CAMERA_SUCCEED)
		return true;
#endif
	return false;
#elif defined(MEX_PLATFORM_SPREAD) 
	DCAMERA_RETURN_VALUE_E eErrorCode = DCAMERA_OP_MAX;
	
	if(MMIDC_IsDCOpen())
		return false;
	
#if ((MEX_SPRD_CODE_VERSION >= 0x09A0937) && (MEX_SPRD_CODE_VERSION <= 0x09AFFFF))
	eErrorCode = DCAMERA_ExtOpenSensor(SENSOR_MODE_COMMON_INIT);
#elif (MEX_SPRD_CODE_VERSION == 0x1101)
	eErrorCode = DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MAIN);
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109) && (MEX_SPRD_CODE_VERSION != 0x10A1124) && (MEX_SPRD_CODE_VERSION != 0x10A6530) 
	eErrorCode = DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MAIN,DCAMERA_NORMAL_MODE);
#endif    
	if(eErrorCode == DCAMERA_OP_SUCCESS)    /*lint !e774*/
	{
		return true;
	}
	
	return false;
#elif defined(MEX_PLATFORM_SIMULATOR)    
	return true;
#endif
}
//开启预览
bool MexInterface_Camera_PreviewStart(void *setting_data,void *preview_data)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)    
	int32 mdi_res;
#endif
	
#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
	extern void Mex_Cam_CapProc_CallBack(void* args);
	mdi_camera_preview_struct cam_preview = { 0 };
	mdi_camera_setting_struct cam_setting = { 0 };
	int32 iWidth, iHeight;
	gdi_handle hPreviewHandle;
	
	MexPhs_GetPreviewWH(&iWidth, &iHeight);
	mdi_camera_load_default_setting(&cam_setting);
	cam_setting.preview_width = iWidth;
	cam_setting.preview_height = iHeight;
	cam_setting.lcm = 0;
#if	MEX_MTK_CODE_VERSION < 0x1112
	cam_setting.preview_rotate = 0;
	cam_setting.lcm_rotate = 0;
#endif
	gdi_layer_create_using_outside_memory(
		0,0,iWidth,iHeight,
		&g_hPreviewLayer,
		MexCamera_GetCameraWriteBuffer(),
		iWidth*iHeight*2);
	//arvinzhang modify:delete assert
	//ASSERT(g_hPreviewLayer != NULL);
	if(g_hPreviewLayer == NULL)
	{
		mex_log(MEX_LOG_SYSTEM, "I: gdi_layer_create_using_outside_memory fail!!!");
		return FALSE;
	}
	
	cam_preview.preview_layer_handle = g_hPreviewLayer;
#if	MEX_MTK_CODE_VERSION < 0x1112
	cam_preview.preview_wnd_offset_x = 0;
	cam_preview.preview_wnd_offset_y = 0;
	cam_preview.preview_wnd_width = iWidth;
	cam_preview.preview_wnd_height = iHeight;
#endif
	
	
	cam_preview.is_lcd_update = false;
	cam_preview.preview_layer_flag = MexConfig_ConvertMtkDef(E_M2MDC_GDI_LAYER_ENABLE_LAYER_0);
	cam_preview.src_key_color = MexConfig_ConvertMtkDef(E_M2MDC_GDI_COLOR_TRANSPARENT);;
	cam_preview.is_tvout = false;
	setting_data = &cam_setting;
	preview_data = &cam_preview;
	g_bMexDbgCameraDataReq = true;
#endif
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION < 0x1112)
	mdi_res = mdi_camera_preview_start(preview_data, setting_data);
#else
	mdi_res = mdi_camera_preview_start(preview_data, setting_data, NULL);
#endif
	mex_log(1, "cam preview start res:%d", mdi_res);
	if(mdi_res == MDI_RES_CAMERA_SUCCEED)
	{
#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
		//Init
		void *pWriteBuffer = mex_Phs_GetCameraOutPutData();
		mex_memset16(pWriteBuffer, iWidth * iHeight, 0x0000);
#endif
		return true;
	}
#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
	g_bMexDbgCameraDataReq = false;
#endif
	return false;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return true;    
#endif    
}

bool MexInterface_Camera_PreviewStop(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
/*
while(bCapReq)
{
kal_sleep_task(1);
}
	*/
	Mex_Phs_Rest_CameraCapReq();
	
	if(mdi_camera_preview_stop() == MDI_RES_CAMERA_SUCCEED)
	{
#if defined(DRV_IDP_MT6236_SERIES)
		if (g_hPreviewLayer != NULL)
		{
			gdi_layer_free(g_hPreviewLayer);
			g_hPreviewLayer  = NULL;
		}
#endif
		return true;
	}
	return false;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return true;
#endif  
}
bool MexInterface_Camera_PowerOff(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	if(mdi_camera_power_off() == MDI_RES_CAMERA_SUCCEED)
		return true;
	return false;
#elif defined(MEX_PLATFORM_SPREAD) 
#if (MEX_SPRD_CODE_VERSION >= 0x09A0937) && (MEX_SPRD_CODE_VERSION != 0x10A1124) && (MEX_SPRD_CODE_VERSION != 0x10A6530) 
	DCAMERA_RETURN_VALUE_E eErrorCode = DCAMERA_OP_MAX;
    
	eErrorCode = DCAMERA_ExtCloseSensor();
	if(eErrorCode == DCAMERA_OP_SUCCESS)
	{
		return true;
	}
	return false;
#else
    return false;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR)    
	return true;    
#endif   
}


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexInterface_Camera_Capture_Req(Mex_Camera_Cap_Req *req)
{
#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
	extern void MexYuv420ToRGB565(char *yuvBuf, char *rgbBuf, int width, int height);
	extern void mex_idp_imgdam_wdma0_interrupt_enable(void);
	
	//ASSERT(req != NULL);
	if(req == NULL) return;
	
	if (!g_bMexDbgCameraDataReq)
	{
		uint8 *pYuv = MexCamera_GetCameraWriteBuffer();
		uint8 *pRgb = mex_Phs_GetCameraOutPutData();
		int32 iWidth, iHeight;
		
		MexPhs_GetPreviewWH(&iWidth, &iHeight);
#if defined(DRV_IDP_MT6236_SERIES) 
		MexYuv420ToRGB565((char *)pYuv, (char *)pRgb, iWidth, iHeight);
#else	
		memcpy(pRgb,pYuv,iWidth*iHeight*2);
#endif
		g_bMexDbgCameraDataReq = true;
		mex_idp_imgdam_wdma0_interrupt_enable(); 
	}
	req->data_cb();
	Mex_Phs_Rest_CameraCapReq();
	mex_log(MEX_LOG_SYSTEM, "I: cam mt6236 capreq done");
#endif
	
}
#endif

void MexInterface_Camera_Load_Setting(void)
{
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)
	mmi_camera_load_setting();
#endif
}


bool MexInterface_Camera_CaptureToMem(uint8** pBuffer,uint32 *pSize)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	extern BOOL mdi_camera_is_ready_to_capture();
	
	if(mdi_camera_is_ready_to_capture())
	{		
#if (MEX_MTK_CODE_VERSION<0x0824)
		MDI_RESULT result = mdi_camera_capture_to_memory((U8**)pBuffer,pSize);
#else
		MDI_RESULT result = mdi_camera_capture_to_memory((U8**)pBuffer,pSize,NULL);		
#endif
		
		if(result == MexConfig_ConvertMtkDef(E_M2MDC_MDI_RES_CAMERA_SUCCEED))
		{
			return true;
		}
	}
	return false;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return false;
#endif       
}

//截取当前活动图层
void MexInterface_SnapShotScreen(wchar_t *fileName)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	gdi_handle handle_ptr;
	gdi_layer_get_active(&handle_ptr);
	mdi_camera_encode_layer_to_jpeg(handle_ptr,(S8 *)fileName);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif       
}

void MexInterface_Camera_ZoomIn()
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if !defined(MEX_PLATFORM__FOR_MTK23C)
	uint16 zoom_factor;
	
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)
	extern camera_context_struct g_camera_cntx;	
	if (g_camera_cntx.setting.zoom_idx < MexConfig_GetPhoneConfig(E_MPC_CMAERA_ZOOM_LIMIT) - 1)
	{
		if (g_camera_cntx.zoom_limit >  g_camera_feature_zoom[g_camera_cntx.setting.zoom_idx])
		{
			g_camera_cntx.setting.zoom_idx += 1;
			zoom_factor =  g_camera_feature_zoom[g_camera_cntx.setting.zoom_idx];;
			mdi_camera_update_para_zoom(zoom_factor);
		}
	}
#else
    // TODO:
    return;
#endif
	
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif   
}
void MexInterface_Camera_ZoomOut()
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if !defined(MEX_PLATFORM__FOR_MTK23C)
	uint16 zoom_factor;
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)
	extern camera_context_struct g_camera_cntx;
	
	if (g_camera_cntx.setting.zoom_idx > 0)
	{
		g_camera_cntx.setting.zoom_idx -= 1;
		zoom_factor =  g_camera_feature_zoom[g_camera_cntx.setting.zoom_idx];			
		mdi_camera_update_para_zoom(zoom_factor);
	}
#endif
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif   
}

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)    
bool  MexInterface_Camera_OutputData(
									 int32    offset_x,
									 int32    offset_y,
									 int32    iImgWidth,
									 int32    iImgHeight,
									 int8     outputformat,
									 uint32  *buffer,
									 int32    buffer_len,         
									 uint32  *puiImgWidth,
									 uint32  *puiImgHeight,
									 uint32 **puiImgBuff)
{
#if defined(MEX_PLATFORM_SIMULATOR)    
	return false;
#elif defined(MEX_PLATFORM_SPREAD)
#if (MEX_SPRD_CODE_VERSION >= 0x09A0937) && (MEX_SPRD_CODE_VERSION != 0x10A1124) && (MEX_SPRD_CODE_VERSION != 0x10A6530) 
	DCAMERA_EXT_GET_DATA_T get_data={0x00};
	DCAMERA_EXT_GET_DATA_T_PTR get_data_ptr=&get_data;
	DCAMERA_YUV_CHN_ADDR_T rtn_image_addr={0x00};
	DCAMERA_SIZE_T rtn_size={0x00};
	
	mex_log(MEX_LOG_SYSTEM, "C: Camera Output %d %d %d",outputformat, iImgWidth, iImgHeight); // create new layer %x
	
	
	if(outputformat == E_MPCOF_YUV420)
	{
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)    
		get_data_ptr->data_format=DCAMERA_DATA_TYPE_YUV;
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109)        						
		get_data_ptr->data_format=DCAMERA_DATA_TYPE_YUV420;        
#endif        
	}
	else if(outputformat == E_MPCOF_JPEG)
	{
		get_data_ptr->data_format=DCAMERA_DATA_TYPE_JPEG;    
	}
	get_data_ptr->img_size.w=iImgWidth;
	get_data_ptr->img_size.h=iImgHeight;
	
#if 0   
	get_data_ptr->is_display=true;
	get_data_ptr->display_rect.x=0;
	get_data_ptr->display_rect.y=80;
	get_data_ptr->display_rect.w=160;
	get_data_ptr->display_rect.h=120;
	get_data_ptr->lcd_id=MAIN_LCD_ID;
#endif
#if (MEX_SPRD_CODE_VERSION >= 0x09AFFFF)
	get_data_ptr->target_rect.x=0;
	get_data_ptr->target_rect.y=0;
	get_data_ptr->target_rect.w=iImgWidth;
	get_data_ptr->target_rect.h=iImgHeight;
#endif
	
	get_data_ptr->buf_addr = (uint32)buffer;
	get_data_ptr->buf_len = (uint32)buffer_len;
	
	//调用获取摄像头数据接口
	if(DCAMERA_OP_SUCCESS != DCAMERA_ExtGetData(get_data_ptr,&rtn_image_addr,&rtn_size))
	{
		mex_log(MEX_LOG_SYSTEM, "C: Camera Output Failed"); // create new layer %x
		
		return false;
	}
	
	*puiImgWidth  = rtn_size.w;
	*puiImgHeight = rtn_size.h;    
	*puiImgBuff   = (uint32*)rtn_image_addr.y_chn_addr;
    mex_memmove((void*)(rtn_image_addr.y_chn_addr + rtn_size.w * rtn_size.h),(const void*)rtn_image_addr.u_chn_addr,rtn_size.w * rtn_size.h);
	
	mex_log(MEX_LOG_SYSTEM, "C: Camera Output Succeed %d %d %x %x",
		rtn_size.w,rtn_size.h,rtn_image_addr.y_chn_addr,rtn_image_addr.u_chn_addr); // create new layer %x
	
	return true;
#else
    return false;
#endif
#endif
	
}
#endif


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x0952)
#define MMI_FRM_SMS_GET_PROFILE_NUM_RSP_STRUCT     mmi_sms_get_profile_num_rsp_struct
#define MMI_FRM_SMS_SET_PROFILE_PARAMS_REQ_STRUCT  mmi_sms_set_profile_params_req_struct
#define MMI_FRM_SMS_SET_PROFILE_PARAMS_RSP_STRUCT  mmi_sms_set_profile_params_rsp_struct
#define MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT  mmi_sms_get_profile_params_req_struct
#define MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT  mmi_sms_get_profile_params_rsp_struct
#define MMI_FRM_SMS_SET_COMMON_PARAMS_REQ_STRUCT   mmi_sms_set_common_params_req_struct
#define MMI_FRM_SMS_SET_COMMON_PARAMS_RSP_STRUCT   mmi_sms_set_common_params_rsp_struct
#define MMI_FRM_SMS_GET_COMMON_PARAMS_RSP_STRUCT   mmi_sms_get_common_params_rsp_struct
#define MMI_FRM_SMS_GET_MSG_NUM_RSP_STRUCT         mmi_sms_get_msg_num_rsp_struct
#define MMI_FRM_SMS_SET_PREFERRED_STORAGE_REQ_STRUCT  mmi_sms_set_preferred_storage_req_struct
#define MMI_FRM_SMS_SET_PREFERRED_STORAGE_RSP_STRUCT  mmi_sms_set_preferred_storage_rsp_struct
#define MMI_FRM_SMS_GET_PREFERRED_STORAGE_RSP_STRUCT  mmi_sms_get_preferred_storage_rsp_struct
#define MMI_FRM_SMS_SET_MAILBOX_ADDRESS_REQ_STRUCT mmi_sms_set_mailbox_address_req_struct
#define MMI_FRM_SMS_SET_MAILBOX_ADDRESS_RSP_STRUCT mmi_sms_set_mailbox_address_rsp_struct
#define MMI_FRM_SMS_GET_MAILBOX_ADDRESS_RSP_STRUCT mmi_sms_get_mailbox_address_rsp_struct
#if 0 //(MEX_MTK_CODE_VERSION >= 0x0952)
									 void mmi_frm_sms_send_message(
										 module_type MOD_SRC,
										 module_type MOD_DEST,
										 U8 MSG_SAP,
										 const U16 MSG_ID,
										 void *LOCAL,
										 void *PEER)
									 {
										 /*----------------------------------------------------------------*/
										 /* Local Variables                                                */
										 /*----------------------------------------------------------------*/
										 MYQUEUE Message;
										 
										 /*----------------------------------------------------------------*/
										 /* Code Body                                                      */
										 /*----------------------------------------------------------------*/
										 Message.oslSrcId = MOD_SRC;
										 Message.oslDestId = MOD_DEST;
										 Message.oslMsgId = (msg_type) MSG_ID;
										 Message.oslDataPtr = LOCAL;
										 Message.oslPeerBuffPtr = PEER;
										 OslMsgSendExtQueue(&Message);
									 }
#endif
									 
#endif
									 
#if (MEX_MTK_CODE_VERSION >= 0x0952)
									 // get sms sc callback
									 void MexInterface_GetSmsSc_Callback(srv_sms_callback_struct* callback_data)
									 {
										 void *pSmsScUCS2 = NULL;
										 MexPsFuncPtr pMexCB = (MexPsFuncPtr) callback_data->user_data;
										 
										 if (callback_data->result == true)
										 {
											 pSmsScUCS2 = callback_data->action_data;
										 }
										 
										 if (pMexCB != NULL)
										 {
											 pMexCB( pSmsScUCS2 );
										 }
									 }
									 
#endif // MEX_MTK_CODE_VERSION
									 
#endif
									 
									 
#if CUSTOM_FOR_DEWAV_GET_SMSCENT
									 funcptr *sim1_cb = NULL;
									 void mex_get_profile_sim1_rsp(void *profiledetail, module_type mod, U16 result)
									 {
										 if(sim1_cb == NULL)
											 return;
										 if (result == MMI_FRM_SMS_OK)
										 {
											 ((PsFuncPtr)sim1_cb)( profiledetail );
											 return ;
										 }
										 ((PsFuncPtr)sim1_cb)( (void *)0 );
									 }
									 
#ifdef __MMI_DUAL_SIM_MASTER__
									 
									 funcptr *sim2_cb = NULL;
									 
									 void mex_get_profile_sim2_rsp(void *profiledetail, module_type mod, U16 result)
									 {
										 if(sim2_cb == NULL)
											 return;
										 if (result == MMI_FRM_SMS_OK)
										 {
											 ((PsFuncPtr)sim2_cb)((void *)profiledetail);
											 return;
										 }
										 ((PsFuncPtr)sim2_cb)((void *)0);
									 }
									 
#endif
									 
#endif
									 void MexInterface_GetScAddr_Req(funcptr *fpCB,E_MEX_Sim_Card_ID eSimId)
									 {
#if CUSTOM_FOR_DEWAV_GET_SMSCENT
										 ClearInputEventHandler(MMI_DEVICE_ALL);
										 
										 if(eSimId == E_MEX_SIM_1)
										 {
#ifdef __MMI_DUAL_SIM_MASTER__	
											 MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
											 MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);	
#endif	
											 sim1_cb = fpCB;
											 mmi_frm_sms_get_profile_detail(mex_get_profile_sim1_rsp, MOD_MMI, (U8) 0);
										 }
										 else if(eSimId == E_MEX_SIM_2)
										 {
#ifdef __MMI_DUAL_SIM_MASTER__
											 MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
											 MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
											 sim2_cb = fpCB;
											 mmi_frm_sms_get_profile_detail(mex_get_profile_sim2_rsp, MOD_MMI, (U8) 0);
#endif
										 }
										 
#else
										 
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x0952)
										 srv_sms_sim_enum sim_id = SRV_SMS_SIM_1;
										 switch (eSimId)
										 {
										 case E_MEX_SIM_1:
											 sim_id = SRV_SMS_SIM_1;
											 break;
										 case E_MEX_SIM_2:
#ifdef __MMI_DUAL_SIM_MASTER__
											 sim_id = SRV_SMS_SIM_2;
											 break;
#else
											 return ;
#endif
#ifdef __MET_MEX_SIM_PLUS__
#if (MMI_MAX_SIM_NUM >= 3)
										 case E_MEX_SIM_3:
											 sim_id = SRV_SMS_SIM_3;
											 break;
#endif
#if (MMI_MAX_SIM_NUM >= 4)
										 case E_MEX_SIM_4:
											 sim_id = SRV_SMS_SIM_4;
											 break;
#endif
											 
#endif/*__MET_MEX_SIM_PLUS__*/
										 default:
											 break;
										 }
										 
										 srv_sms_get_setting_para(SRV_SMS_ACTIVE_SC_ADDR, NULL, sim_id, MexInterface_GetSmsSc_Callback, fpCB);
#else
										 
										 MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT *msgReq;
										 
										 msgReq = (MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*) OslConstructDataPtr(sizeof(*msgReq));
										 if(!msgReq)
										 {
											 ((PsFuncPtr)fpCB)((void *)0);
											 return;
										 }
										 msgReq->profile_no = 0;
										 
										 if(eSimId == E_MEX_SIM_1)
										 {
											 SetProtocolEventHandler((PsFuncPtr)fpCB,PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
											 mmi_frm_sms_send_message(
												 MOD_MMI,
												 MOD_L4C,
												 0,
												 PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ,
												 (oslParaType*) msgReq,
												 NULL);
										 }
										 
										 else if(eSimId == E_MEX_SIM_2)
										 {
#ifdef __MMI_DUAL_SIM_MASTER__
											 MTPNP_PFAL_Register_Event_Handler(MTPNP_PFAL_MSG_SMS_GET_PROFILE_PARAMS_RSP,(MTPNP_EVENTHANDLER_FuncPtr)fpCB);
											 MTPNP_AD_SMS_Get_Profile_Params_Req(0);
#else
											 /*
											 
											   Mex_SetSlaveProtocolEventHandler(fpCB, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
											   mmi_frm_sms_send_message(
											   MOD_MMI,
											   MOD_L4C_2,
											   0,
											   PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ,
											   (oslParaType*) msgReq,
											   NULL);
											 */
#endif
										 }
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
										 MN_CALLED_NUMBER_T  scaddr;
										 ERR_MNSMS_CODE_E    error_code;
										 MN_DUAL_SYS_E       sim_type;
										 int8                sms_center_addr[27];
										 
#if (MEX_SDK_VER >= 1018)
										 eSimId = MexConfig_ConvertSimCard(eSimId);
#endif
										 
										 sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
										 if(eSimId == E_MEX_SIM_2)
										 {
											 sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
										 }    
#if defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)     //支持三SIM卡
										 else if(eSimId == E_MEX_SIM_3)
										 {
											 sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
										 }    
#endif //MEX_THIRD_SIM
#endif
										 
										 memset(sms_center_addr,0,27);   
										 error_code =  MNSMS_GetScAddrEx(sim_type,&scaddr);
										 if(error_code == ERR_MNSMS_NONE)
										 {        
											 //转化BCD格式的短消息中心号码为字符串模式
											 MexConfig_ConvertBCD2Ansi(
												 sms_center_addr, 
												 27,
												 (int8*)scaddr.party_num, 
												 scaddr.num_len,
												 0);
											 
											 if(fpCB)
											 {
												 ((PsFuncPtr)fpCB)((void *)sms_center_addr);       /*lint !e611 */
											 }
										 }
										 else
										 {
											 if(fpCB)
											 {
												 ((PsFuncPtr)fpCB)((void *)NULL);       /*lint !e611 */
											 }
										 }
										 
#endif	
										 
#endif
}


bool MexInterface_GetProfleDetailRsp(void *inMsg,E_MEX_Sim_Card_ID eSimId,int8 *pSmsCent)
{
	
#if CUSTOM_FOR_DEWAV_GET_SMSCENT
	msg_profile_edit_struct * pd = NULL;
	
	if(inMsg == NULL)
		return false;
	
	
	pd=(msg_profile_edit_struct*) inMsg;
	
	mmi_ucs2_to_asc((S8 *)pSmsCent,(S8 *)pd->scAddress);
	return true;
	
#else
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x0952)
	bool bRes = false;
	if (inMsg != NULL)
	{
		bRes = true;
		mex_wcs2nstr((char *) pSmsCent, 27, (const wchar_t *) inMsg);
	}
	mex_log(MEX_LOG_PHS, "I: sim:%d sc:%s res:%d", eSimId, pSmsCent, bRes);
	return bRes;
#else
	
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	if(eSimId == E_MEX_SIM_1)
	{
		ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	}
	
	else if(eSimId == E_MEX_SIM_2)
	{
#ifdef __MMI_DUAL_SIM_MASTER__
		{
			ST_MTPNP_AD_SMS_GET_PROFILE_PARAMS_RSP *msgRsp = (ST_MTPNP_AD_SMS_GET_PROFILE_PARAMS_RSP *)inMsg;
			MTPNP_PFAL_Remove_Event_Handler(MTPNP_PFAL_MSG_SMS_GET_PROFILE_PARAMS_RSP);
			if (msgRsp->result == TRUE)
			{
				int8 iCpyLen = 27;
				iCpyLen = (msgRsp->sc_addr.length > iCpyLen ?iCpyLen:msgRsp->sc_addr.length);
				strncpy((char*)pSmsCent,(char*)msgRsp->sc_addr.number,iCpyLen);
				return true;
			}
			return false;
		}
#else
		/*
		Mex_ClearSlaveProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);	
		if (msgRsp->result == TRUE)
		{
		int8 iCpyLen = 27;
		iCpyLen = (msgRsp->sc_addr.length > iCpyLen ?iCpyLen:msgRsp->sc_addr.length);
		strncpy((char*)pSmsCent,(char*)msgRsp->sc_addr.number,iCpyLen);
		return true;
		}
		return false;
		*/
#endif
		
	}
	if (msgRsp->result == TRUE)
	{
		int8 iCpyLen = 27;
		iCpyLen = (msgRsp->sc_addr.length > iCpyLen ?iCpyLen:msgRsp->sc_addr.length);
		strncpy((char*)pSmsCent,(char*)msgRsp->sc_addr.number,iCpyLen);
		return true;
	}
	return false;
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return false;
#endif 
#endif
}
//当前是否可以播放声音
bool MexInterface_CanPlaySound(void)
{
#ifdef __MMI_VIDEO_PLAYER__
#include "VdoPlyResDef.h"
	//视频播放界面 返回false
	if(SCR_ID_VDOPLY_PLAYER == GetActiveScreenId())
	{
		return false;
	}
#endif
	return true;
}

//轨迹球 飞梭等输入设备 事件回调

//顺时针方向
void MexInterface_CircleDeviceCallBack_ClockWise(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern void MexSystem_Listener_CircleDevice_Clockwise(void);
	MexSystem_Listener_CircleDevice_Clockwise();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif    
}
//逆时针方向
void MexInterface_CircleDeviceCallBack_AntiClockWise(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern void MexSystem_Listener_CircleDevice_AntiClockwise(void);
	MexSystem_Listener_CircleDevice_AntiClockwise();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif           
}

//重力感应器接口
//todo


//patch001 解决0956代码连接错误问题

void MexInterface_OslMsgSendQueue(void *Message)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	OslMsgSendExtQueue(Message);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif
}

//参考mdi_camera_encode_layer_to_jpeg
bool MexInterface_EncodeBufferToJpeg(uint8 *image_ptr, int32 image_width,int32 image_height,int8 jpg_qty,wchar_t *file_name)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if defined(JPG_ENCODE) && defined(__MTK_TARGET__)
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	img_encode_struct encode_data = { 0 };
	uint32 file_size;
	int32 ret;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	encode_data.image_width = image_width;
	encode_data.image_height = image_height;
	if(jpg_qty == E_MEX_JPG_QTY_LOW)
	{
		encode_data.image_quality = CAM_JPG_QTY_LOW;
	}
	else if(E_MEX_JPG_QTY_HIGH)
	{
		encode_data.image_quality = CAM_JPG_QTY_HIGH;
	}
	else
	{
		encode_data.image_quality = CAM_JPG_QTY_NORMAL;
	}
	encode_data.image_buffer_p = (void*)image_ptr;
	encode_data.image_buffer_size = (image_width * image_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;
	
	encode_data.target_width = image_width;
	encode_data.target_height = image_height;
	encode_data.media_mode = MED_MODE_FILE;
	encode_data.media_type = MED_TYPE_JPG;
	
	encode_data.data = (void*)file_name;
	encode_data.file_buffer_len = (kal_uint32) 0;   /* not used */
	encode_data.file_size_p = (kal_uint32*) & file_size;
	encode_data.seq_num = (kal_uint16) 0;           /* not used */
	encode_data.blocking = (kal_bool) TRUE;         /* blocking */
	encode_data.gray_mode = 0;
#if defined(MT6236) && (MEX_MTK_CODE_VERSION < 0x1112)
	encode_data.input_type = IMAGE_FORMAT_RGB565;
#endif
	
#if (MDI_CAMERA_MT6228_SERIES)
	encode_data.jpeg_yuv_mode = JPEG_FORMAT_YUV420;
	encode_data.thumbnail_mode = FALSE;
	encode_data.overlay_frame_mode = FALSE;
#endif /* MDI_CAMERA_MT6228_SERIES */ 
	kal_prompt_trace(MOD_NIL,"jpg encode start time %d",mex_gettime());
	MexSystem_NotifyLockSRam();
	ret = media_img_encode(stack_int_get_active_module_id(), &encode_data);
	MexSystem_NotifyUnlockSRam();
	kal_prompt_trace(MOD_NIL,"jpg encode end time %d",mex_gettime());
	if (ret == MED_RES_OK)
	{
		return true;
	}
	else if (ret == MED_RES_DISC_FULL)
	{
		//return MET_PLAT_SAVE_JPG_ERR_DISK_FULL;
	}
	else if (ret == MED_RES_WRITE_PROTECTION)
	{
		//return MET_PLAT_SAVE_JPG_ERR_WRITE_PROTECTION;
	}
	else if (ret == MED_RES_NO_DISC)
	{
		//return MET_PLAT_SAVE_JPG_ERR_NO_DISK;
	}
	else if (ret == MED_RES_ROOT_DIR_FULL)
	{
		//return MET_PLAT_SAVE_JPG_ERR_ROOT_DIR_FULL;
	}
	else
	{
		// return MET_PLAT_SAVE_JPG_ERR_FAILED;
	}
	return false;
	
#else /* defined(JPG_ENCODE) && defined(__MTK_TARGET__) */ 
	return false;
#endif /* defined(JPG_ENCODE) && defined(__MTK_TARGET__) */ 
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return false;
#endif           
	
}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//参考mdi_camera_encode_layer_to_jpeg
bool MexInterface_EncodeBufferToJpeg_Ext(uint8 *image_ptr, int32 image_width,int32 image_height,int8 jpg_qty,void *buffer,uint32 buffer_size,uint32 *jpg_size)
{
#if defined(JPG_ENCODE) && defined(__MTK_TARGET__)
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	img_encode_struct encode_data;
	//U32 file_size;
	S32 ret;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	encode_data.image_width = image_width;
	encode_data.image_height = image_height;
	if(jpg_qty == E_MEX_JPG_QTY_LOW)
	{
		encode_data.image_quality = CAM_JPG_QTY_LOW;
	}
	else if(E_MEX_JPG_QTY_HIGH)
	{
		encode_data.image_quality = CAM_JPG_QTY_HIGH;
	}
	else
	{
		encode_data.image_quality = CAM_JPG_QTY_NORMAL;
	}
	encode_data.image_buffer_p = (void*)image_ptr;
	encode_data.image_buffer_size = (image_width * image_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;
	
	encode_data.target_width = image_width;
	encode_data.target_height = image_height;
	encode_data.media_mode = MED_MODE_ARRAY;
	encode_data.media_type = MED_TYPE_JPG;
	
	encode_data.data = (void*)buffer;
	encode_data.file_size_p = (kal_uint32*) jpg_size;
	encode_data.blocking = (kal_bool) TRUE;         /* blocking */
	encode_data.seq_num = (kal_uint16) 0;           /* not used */
	encode_data.file_buffer_len = (kal_uint32) buffer_size;   /* not used */
	encode_data.gray_mode = 0;
	
#if (MDI_CAMERA_MT6228_SERIES)
	encode_data.jpeg_yuv_mode = JPEG_FORMAT_YUV420;
	encode_data.thumbnail_mode = FALSE;
	encode_data.overlay_frame_mode = FALSE;
#endif /* MDI_CAMERA_MT6228_SERIES */ 
	kal_prompt_trace(MOD_NIL,"jpg encode start time %d",mex_gettime());
	MexSystem_NotifyLockSRam();
	ret = media_img_encode(stack_int_get_active_module_id(), &encode_data);
	MexSystem_NotifyUnlockSRam();
	kal_prompt_trace(MOD_NIL,"jpg encode end time %d",mex_gettime());
	if (ret == MED_RES_OK)
	{
		return true;
	}
	else if (ret == MED_RES_DISC_FULL)
	{
		//return MET_PLAT_SAVE_JPG_ERR_DISK_FULL;
		kal_prompt_trace(MOD_NIL,"MED_RES_DISC_FULL");
	}
	else if (ret == MED_RES_WRITE_PROTECTION)
	{
		//return MET_PLAT_SAVE_JPG_ERR_WRITE_PROTECTION;
		kal_prompt_trace(MOD_NIL,"MED_RES_WRITE_PROTECTION");
	}
	else if (ret == MED_RES_NO_DISC)
	{
		//return MET_PLAT_SAVE_JPG_ERR_NO_DISK;
		kal_prompt_trace(MOD_NIL,"jMED_RES_NO_DISC");
	}
	else if (ret == MED_RES_ROOT_DIR_FULL)
	{
		//return MET_PLAT_SAVE_JPG_ERR_ROOT_DIR_FULL;
		kal_prompt_trace(MOD_NIL,"MED_RES_ROOT_DIR_FULL");
	}
	else
	{
		// return MET_PLAT_SAVE_JPG_ERR_FAILED;
		kal_prompt_trace(MOD_NIL,"encode jpg unknown error");
	}
	return false;
	
#else /* defined(JPG_ENCODE) && defined(__MTK_TARGET__) */ 
	return false;
#endif /* defined(JPG_ENCODE) && defined(__MTK_TARGET__) */ 
	
}
#endif


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
int MexInterface_mdi_audio_play_file_ext(void *file_name, int play_style, void *cache_p, void* handler, int volume, int path)
{
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	return mdi_audio_play_file_with_vol_path(
		(void*) file_name,
		(U8) play_style,
		(void*) cache_p,
		(mdi_ext_callback) handler,
		NULL,
		(U8) volume,
		(U8) path);
#else
	return mdi_audio_play_file_with_vol_path(
		(void*) file_name,
		(U8) play_style,
		(void*) cache_p,
		(mdi_callback) handler,
		(U8) volume,
		(U8) path);
#endif
	
}

int MexInterface_mdi_audio_restore_file_ext(void *file_name, int play_style, void *cache_p, void* handler, int volume, int path)
{
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	return mdi_audio_restore_file_with_vol_path(
		(void *)file_name,
		(U8) play_style,
		(void *)cache_p,
		(mdi_ext_callback) handler,
		NULL,
		(U8) volume,
		(U8) path);
#else
	return mdi_audio_restore_file_with_vol_path(
		(void *)file_name,
		(U8) play_style,
		(void *)cache_p,
		(mdi_callback) handler,
		(U8) volume,
		(U8) path);
#endif
}


#endif


bool MexPhs_PhnSetWallPaper(E_MEX_WallPaper_Type eType, const wchar_t *pFileName)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern PHNSET_CNTX *g_phnset_cntx_p;
	extern S8 *phnset_dispchar_strp;
	uint16 typeMask = MexConfig_GetPhoneConfig(E_MPC_PhnSetWallPaperTypeMask);
	
	if ((typeMask & eType) == 0)
	{
		mex_log(MEX_LOG_PHS, "SetWP E: t:%d", eType);
		return false;
	}
	
	if(g_Mex_WallPaperIsSetting==1)
	{
		mex_log(MEX_LOG_PHS, "SetWP E: Busy");
		return false;
	}
	
	switch (eType)
	{
	case E_MWPT_WallPaper:
		{
			bool isValidFile = false;
			
			// 一. 检测文件是否可以设为壁纸
#if (MEX_MTK_CODE_VERSION < 0x1112)
			{
				PS8 error_str_ptr;
				
				gdi_layer_lock_frame_buffer();
				gdi_layer_push_clip();
				gdi_layer_set_clip(0,0,0,0);
				if(mmi_fmgr_util_file_limit_check(FMGR_LIMIT_WALLPAPER, (PS8) pFileName, &error_str_ptr))
				{
					isValidFile = true;
				}
				gdi_layer_pop_clip();
				gdi_layer_unlock_frame_buffer();
			}
#endif
			
			// 二. 设置壁纸
			if (isValidFile)
			{
				phnset_wpss_type_enum wpss_type_bak;
				wpss_type_bak = g_phnset_cntx_p->wpss_type;
				g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
#if (MEX_MTK_CODE_VERSION >= 0x1112)
                // TODO:		
                return false;
#elif (MEX_MTK_CODE_VERSION >= 0x0936)
				PhnsetWpUsrDefFreeBuf();
				phnset_dispchar_strp=(S8 *)pFileName;
				g_Mex_WallPaperIsSetting=1;
				mmi_phnset_copy_wpss_check_device();
				phnset_dispchar_strp=NULL;
#else
				PhnsetSetWallpaperByFileName((S8*) pFileName);
				g_phnset_cntx_p->wpss_type = wpss_type_bak;
#endif/* (MEX_MTK_CODE_VERSION >= 0x0936) */
				return true;
			}
		}
		break;
	case E_MWPT_SubWallPaper:
		{
			
		}
		break;
	case E_MWPT_ScreenSaver:
		{
			bool isValidFile = false;
			
			// 一. 检测文件是否可以设为屏保
			{
				PS8 error_str_ptr;
#if (MEX_MTK_CODE_VERSION < 0x1112)
				if (mmi_fmgr_util_file_limit_check(FMGR_LIMIT_SCREEN_SAVER, (PS8) pFileName, &error_str_ptr))
				{
					isValidFile = true;
				}
#endif
				// 二. 设置屏保
				if(isValidFile){
					phnset_wpss_type_enum wpss_type_bak;			
					wpss_type_bak = g_phnset_cntx_p->wpss_type;
					g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SS;
#if  (MEX_MTK_CODE_VERSION >= 0x1112)
					// TODO:
					return false;
#elif (MEX_MTK_CODE_VERSION >= 0x0936)
					PhnsetWpUsrDefFreeBuf();
					phnset_dispchar_strp=(S8 *)pFileName;
					g_Mex_WallPaperIsSetting=1;
					mmi_phnset_copy_wpss_check_device();
					phnset_dispchar_strp=NULL;
#else
					PhnsetSetScrSvrByFileName((S8*) pFileName);
					g_phnset_cntx_p->wpss_type = wpss_type_bak;
#endif/* (MEX_MTK_CODE_VERSION >= 0x0936) */
					return true;
				}
			}
			
		}
		break;
		
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
	case E_MWPT_PowerOn:
		{
			bool isValidFile = false;
			
			// 一. 检测文件是否可以设为开机动画
			{
				PS8 error_str_ptr;
#if (MEX_MTK_CODE_VERSION < 0x1112)
				if (mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, (PS8) pFileName, &error_str_ptr))
				{
					isValidFile = true;
				}
#endif
			}
			
			// 二. 设置开机动画
			if (isValidFile){
				phnset_wpss_type_enum wpss_type_bak;
				wpss_type_bak = g_phnset_cntx_p->wpss_type;
				g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWRON;
#if (MEX_MTK_CODE_VERSION < 0x1112)
				
				PhnsetSetPwrOnOffDisplayByFileName((S8*)pFileName);
#else
				// TODO
				return false;
#endif
				g_phnset_cntx_p->wpss_type = wpss_type_bak;
				return true;
			}
			
			
		}
		break;
	case E_MWPT_PowerOff:
		{
			bool isValidFile = false;
			
			// 一. 检测文件是否可以设为关机动画
			{
				PS8 error_str_ptr;
#if (MEX_MTK_CODE_VERSION < 0x1112)
				if (mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, (PS8) pFileName, &error_str_ptr))
				{
					isValidFile = true;
				}
#endif
			}
			
			// 二. 设置关机动画
			if (isValidFile){
				phnset_wpss_type_enum wpss_type_bak;
				wpss_type_bak = g_phnset_cntx_p->wpss_type;
				g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWROFF;
#if (MEX_MTK_CODE_VERSION < 0x1112)
				PhnsetSetPwrOnOffDisplayByFileName((S8*)pFileName);
#else
				// TODO:
				return false;
#endif
				g_phnset_cntx_p->wpss_type = wpss_type_bak;
				return true;
			}
			
			
		}
		break;
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */		
	}
	
	return false; // default return false
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	return false;
#endif      
}

void MexInterface_VibratorOn(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	VibratorOn();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMIAPISET_SetVibrator(true,MMISET_RING_TYPE_OTHER);
#endif      
}
void MexInterface_VibratorOff(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	VibratorOff();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MMIAPISET_SetVibrator(false,MMISET_RING_TYPE_OTHER);
#endif
}

void MexInterface_PlayVibratorOnce(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	PlayVibratorOnce();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
	MetMex_StartTimer( MetMex_GetVibratorTimerId(), 500, MexInterface_VibratorOff, false );//创建震动停止Timer
	MMIAPISET_SetVibrator(true,MMISET_RING_TYPE_OTHER);   
#endif
}


void MexInterface_SetNightMode(bool bNightMode)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if defined(__MMI_CAMERA__) && !defined(__MMI_CAMCORDER__)
	extern void mdi_camera_update_para_night(U16 cam_para);
	if(bNightMode)
	{
		mdi_camera_update_para_night(CAMERA_SETTING_NIGHT_ON);
	}
	else
	{
		mdi_camera_update_para_night(CAMERA_SETTING_NIGHT_OFF);
	}
#else
    // TODO:
    return;
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
#endif    
}


//系统启动后会调用该函数
void MexInterface_SystemStartUpHandle(void)
{
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	//对FM的处理	
	if(MexConfig_NeedPauseFM())
	{
#ifdef __MMI_FM_RADIO__
		extern BOOL mmi_fmrdo_is_power_on(void);
		extern void mmi_fmrdo_power_on(BOOL turn_on);
		if (mmi_fmrdo_is_power_on())
		{
			mmi_fmrdo_power_on(FALSE);
		}
#endif
	}
#endif
	
}
//系统退出时调用该函数
void MexInterface_SystemShutDownHandle(E_Mex_PlatformShutdown eShutdownMode)
{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)    
    if(UILAYER_NULL_HANDLE != g_mex_select_layer.lcd_dev_info.block_id)
    {
		UILAYER_SetLayerBufferPtr(&(g_mex_select_layer.lcd_dev_info),
                    g_mex_old_layer_buf,
                    g_mex_old_layer_rawbuf,
                    NULL,
                    NULL);
        UILAYER_ReleaseLayer(&(g_mex_select_layer.lcd_dev_info));
        g_mex_select_layer.lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
    } 
#endif    
	switch(eShutdownMode)
	{
	case E_MPS_Pause:		//被暂停及被其他界面中断
		{
			//todo
		}
		break;
	case E_MPS_HotShutdown:	//部分退出 此时QQ挂机程序不退出
		{
			//todo
			MexInterface_Conflict_Resume();
		}
		break;
	case E_MPS_ColdShutdown://全部退出
		{
			//todo
			MexInterface_Conflict_Resume();
		}
		break;
	default:		//其他模式 可以忽略
		{
			//todo
		}
	}
}


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//文件浏览
#if MEX_SDK_VER >= 1009

typedef struct
{
	uint32 sel_type;
	FMGR_FILTER filter;
	wchar_t base_path[MEX_FILE_NAME_MAX_LENGTH];
	Fmgr_CallBack pfCB;
	int32 appid;
}Mex_Fmgr_Struct;

Mex_Fmgr_Struct *pFmgrStruct = NULL;
bool g_bFmgrRunning = false;

void Mex_Fmgr_exit(void);
int8 MexInterface_FmgrInit(uint32 sel_type, wchar_t* base_path,Fmgr_CallBack pfCB)
{
	int32 iOldApp;
	if(NULL == base_path)
	{
        return E_MFR_MAX;
	}
	if(pFmgrStruct != NULL)
	{
		return E_MFR_BUSY;
	}
	iOldApp = MexSystem_GetCurrentApp();
	MexSystem_SetCurrentApp( /*MEX_APPID_INTERNAL*/ 0 );
	pFmgrStruct = (Mex_Fmgr_Struct *)MexSystem_Alloc( sizeof(Mex_Fmgr_Struct) );
	MexSystem_SetCurrentApp(iOldApp);
	if(pFmgrStruct == NULL)
	{
		return E_MFR_MEMERROR;
	}
	
	mex_wcscpy(pFmgrStruct->base_path, base_path);
	pFmgrStruct->sel_type = 0;
	
#if (MEX_MTK_CODE_VERSION < 0x1112)
	if(E_MFST_SELECT_FILE & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_FILE;
	}
	if(E_MFST_SELECT_FOLDER & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_FOLDER;
	}
	if(E_MFST_SELECT_FILE_OR_FOLDER & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_FILE_OR_FOLDER;
	}
	if(E_MFST_SELECT_BROWSE & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_BROWSE;
	}
	if(E_MFST_SELECT_REPEAT & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_REPEAT;
	}
	if(E_MFST_SELECT_FILE_SET & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_FILE_SET;
	}
	if(E_MFST_SELECT_ARCHIVE & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_ARCHIVE;
	}
	if(E_MFST_SELECT_ARCHIVE_WITH_SINGLE_OBJECT & sel_type)
	{
		pFmgrStruct->sel_type |= FMGR_SELECT_ARCHIVE_WITH_SINGLE_OBJECT;
	}
#else
	// TODO:
	return E_MFR_MAX;
#endif
	
	FMGR_FILTER_INIT(&pFmgrStruct->filter);
	
	pFmgrStruct->pfCB = pfCB;
	pFmgrStruct->appid = MexSystem_GetCurrentApp();
	
	return E_MFR_SUCCESS;
	
}

void MexInterface_FmgrSetFilter(E_Mex_Fmgr_Filtertype fliter)
{
	if(pFmgrStruct != NULL)
	{
		extern int32 MexConfig_ConvertMtkFileType(E_Mex_Fmgr_Filtertype param);
		int32 fmgr_type = MexConfig_ConvertMtkFileType(fliter);
		FMGR_FILTER_SET(&pFmgrStruct->filter, fmgr_type);
	}
}

void Mex_Fmgr_select_result_callback(void *filepath, int is_short)
{
	if(pFmgrStruct != NULL && pFmgrStruct->pfCB != NULL)
	{
		int32 oldAppid = MexSystem_GetCurrentApp();
		MexSystem_SetCurrentApp(pFmgrStruct->appid);
		pFmgrStruct->pfCB((wchar_t *)filepath,is_short);
		MexSystem_SetCurrentApp(oldAppid);
	}
	Mex_Fmgr_exit();
	
}

void Mex_Fmgr_exit(void)
{
	if(pFmgrStruct != NULL)
	{
		int32 iOldApp;
		iOldApp = MexSystem_GetCurrentApp();
		MexSystem_SetCurrentApp( /*MEX_APPID_INTERNAL*/0 );
		MexSystem_Free(pFmgrStruct);
		MexSystem_SetCurrentApp( iOldApp );
		pFmgrStruct = NULL;
		g_bFmgrRunning = false;
	}
	if(IsScreenPresent(E_MSI_FMGR_SCREEN))
	{
		DeleteUptoScrID(E_MSI_FMGR_SCREEN);
		DeleteNHistory(1);
		GoBackHistory();
	}
	else if(GetActiveScreenId() == E_MSI_FMGR_SCREEN)
	{
		GoBackHistory();
	}
}

void Mex_Entry_Fmgr_Screen(void)
{
	
	if(pFmgrStruct != NULL)
	{
		g_bFmgrRunning = true;
		EntryNewScreen(E_MSI_FMGR_SCREEN, NULL, Mex_Entry_Fmgr_Screen, NULL);
		
		mmi_fmgr_select_path_and_enter(
			//#ifdef WIN32
			E_MSI_FMGR_SCREEN,
			//#else
			//MET_MEX, 
			//#endif
			pFmgrStruct->sel_type,  
			pFmgrStruct->filter, 
			(PS8)pFmgrStruct->base_path,
			(PsExtFuncPtr)Mex_Fmgr_select_result_callback
			);
	}
}
//设置当前情景模式
void MexInterface_SetProfile(int32 profileId)
{
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	if (profileId >= MMI_PROF_PROF_ID_BEGIN && profileId <= MMI_PROF_PROF_ID_END)
	{
		srv_prof_activate_profile(profileId);
	}
#elif (MEX_MTK_CODE_VERSION >= 0x1032)
	extern MMI_BOOL mmi_prof_activate_profile(mmi_prof_id_enum profile_id, MMI_BOOL need_popup);
	mmi_prof_activate_profile( profileId, MMI_FALSE);
#else
	extern U8 gactivatedprofile;
	extern U8 gprofileId;
	//extern S32 WriteValue(U16 nDataItemId, void *pBuffer, U8 nDataType, S16 *pError);
	S16 error;
	gactivatedprofile = gprofileId = profileId;
	WriteValue(NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE, &error);
	ActivateProfile();
#endif	
}
#endif // MEX_SDK_VER >= 1009
#endif // defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)


#define MEM_FROM_SRAM		1
#define MEM_FROM_MED		2
#define MEM_FROM_XMALLOC	3

//zlib解压过程中使用的内存控制函数
void *MexInterface_zip_uncompress_alloc(int32 size)
{
	void *pReal = NULL;
	void *pRet = NULL;
	extern void* mex_smalloc(size_t iSize);
	extern void* mex_xmalloc(size_t iSize);
	extern void *Mex_Mem_MedAlloc(int32 size);
	extern void mex_log( int32 iFilter, const char *pText, ... );
	
	size += 4;
	
	//try alloc sram
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	if(MexConfig_UnCompressUseSram())
#endif
	{
		pReal = mex_smalloc( size );
		if(pReal != NULL)
		{
			*(int*)pReal = MEM_FROM_SRAM;
			pRet =(void *)( (int)pReal + 4);
			return pRet;
		}
	}
	
	//try alloc med 
	pReal = Mex_Mem_MedAlloc( size );
	if(pReal != NULL)
	{
		*(int*)pReal = MEM_FROM_MED;
		pRet =(void *)( (int)pReal + 4);
		return pRet;
	}
	//try x malloc
	pReal = mex_xmalloc( size );
	if(pReal != NULL)
	{
		*(int*)pReal = MEM_FROM_XMALLOC;
		pRet =(void *)( (int)pReal + 4 );
		mex_log( MEX_LOG_OSLIB, "W: Unc x" );
		return pRet;
	}
	mex_log( MEX_LOG_OSLIB, "E:zipm nul" );
	return pRet;
	
}

void MexInterface_zip_uncompress_free(void *ptr)
{
	extern bool Mex_Mem_MedFree(void **pointer);
	extern void mex_sfree(void *);
	extern void mex_xfree(void *);
	extern void mex_log( int32 iFilter, const char *pText, ... );
	void *pReal = (void *)( (int)ptr - 4 );
	int iFlag = *(int*)pReal;
	if(MEM_FROM_SRAM ==  iFlag)
	{
		mex_sfree( pReal );
	}
	else if(MEM_FROM_MED == iFlag)
	{
		Mex_Mem_MedFree( &pReal );
	}
	else if(MEM_FROM_XMALLOC == iFlag)
	{
		mex_xfree( pReal );
	}
	else
	{
		mex_log( MEX_LOG_OSLIB, "E:zipf err" );
	}
}

void MexInteface_Zlib_Register(MexZlib_alloc_func alloc_hdlr, MexZlib_free_func free_hdlr)
{
#if MEX_MTK_HAS_ZLIB
	appzlib_register(alloc_hdlr, free_hdlr);
#else
    extern void mex_appzlib_register(MexZlib_alloc_func alloc_hdlr, MexZlib_free_func free_hdlr);
	mex_appzlib_register(alloc_hdlr, free_hdlr);
#endif
}

int MexInteface_Zlib_Uncopress (uint8 *dest, uint32 *destLen, const uint8 *source, uint32 sourceLen)
{
#if MEX_MTK_HAS_ZLIB
	return uncompress( dest, destLen, source, sourceLen );
#else
    int mex_uncompress (
        uint8 *dest,
        uint32 *destLen,
        const uint8 *source,
        uint32 sourceLen);

	return mex_uncompress( dest, destLen, source, sourceLen );
#endif
}

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)

//系统锁屏回调(不含调用mex锁屏接口的情况)
//使用于各种g_keylock_context.gKeyPadLockFlag = 1的场合调用
void MexInterface_Keypad_LockHandler(void)
{
	extern void MexSystem_Listener_KeyPadLock( E_Mex_KeyPad_Lock_Event event );
	MexSystem_Listener_KeyPadLock( E_MKLE_KeyPad_Lock );
}

//系统解锁回调(不含调用mex解锁接口的情况)
//使用于各种g_keylock_context.gKeyPadLockFlag = 0的场合调用
void MexInterface_Keypad_UnLockHandler(void)
{
	extern void MexSystem_Listener_KeyPadLock( E_Mex_KeyPad_Lock_Event event );
	MexSystem_Listener_KeyPadLock( E_MKLE_KeyPad_UnLock );
}
extern idle_keylock_struct g_keylock_context;
//系统锁屏
void MexInterface_Lock_Keypad(void)
{
	if(g_keylock_context.gKeyPadLockFlag == 0)
	{
		g_keylock_context.gKeyPadLockFlag = 1;
#ifdef __MMI_TOUCH_SCREEN__
#if MEX_MTK_CODE_VERSION >= 0x1112
		wgui_status_icon_bar_change_icon_image(STATUS_ICON_KEYPAD_LOCK,IMG_SI_KEYPAD_LOCK);
#else
		ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
		UpdateStatusIcons();    
#endif  
#else 
#if MEX_MTK_CODE_VERSION >= 0x1108
		wgui_status_icon_bar_show_icon(STATUS_ICON_KEYPAD_LOCK);
#else
		IdleResetStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif
#endif 
#if (MEX_MTK_CODE_VERSION < 0x1112)
		StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
#else
		StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
#endif
	}
	
}
//系统解锁
void MexInterface_UnLock_Keypad(void)
{
	if(g_keylock_context.gKeyPadLockFlag == 1)
	{
#if (MEX_MTK_CODE_VERSION < 0x1112)
		StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
#else
		StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
#endif
		g_keylock_context.gKeyPadLockFlag = 0;
		
#ifdef __MMI_TOUCH_SCREEN__
#if MEX_MTK_CODE_VERSION >= 0x1112
		wgui_status_icon_bar_change_icon_image(STATUS_ICON_KEYPAD_LOCK,IMG_SI_KEYPAD_UNLOCK);
#else
		ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
		UpdateStatusIcons();    
#endif  
#else 
#if MEX_MTK_CODE_VERSION >= 0x1108
		wgui_status_icon_bar_hide_icon(STATUS_ICON_KEYPAD_LOCK);
#else
		IdleResetStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif
#endif 
		TurnOnBacklight(1);
	}
}

void MexInterface_PenBlock(void)
{
#ifdef __MMI_TOUCH_SCREEN__
	extern void mmi_pen_block();
	mmi_pen_block();
#endif
}

void MexInterface_PenUnBlock(void)
{
#ifdef __MMI_TOUCH_SCREEN__
	extern void mmi_pen_unblock();
	mmi_pen_unblock();
#endif
}

static void MexInterface_AutoKeyPadLock(void)
{
	g_keylock_context.gKeyPadLockFlag = 1;
#if (MEX_MTK_CODE_VERSION < 0x1112)
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
#else
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
#endif
	MexInterface_Keypad_LockHandler();
}

void MexInterface_ReStartKeyPadLockTimer(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	extern void mmi_scr_locker_start_timer(void);
	mmi_scr_locker_start_timer();
#else
	//参考系统函数mmi_idle_start_keypad_lock
	extern void mmi_idle_start_keypad_lock(void);
	mmi_idle_start_keypad_lock();
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
	if(g_idle_context.AutoLockTimeOut > 0)
	{
		StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, MexInterface_AutoKeyPadLock);
	}
#endif
}

bool MexInterface_GetApn(int32 account_id,char *dest,int8 len)
{
	bool ret;
	S8 apn[(MAX_GPRS_MMI_APN_LEN + 1) *ENCODING_LENGTH];
#if (MEX_MTK_CODE_VERSION < 0x1112)
	extern BOOL mmi_dtcnt_get_apn(U32 account_id, S8 *dest, U8 len);
	
	if(NULL == dest)
	{
        return false;
	}
	
	ret = mmi_dtcnt_get_apn(account_id,apn,MAX_GPRS_MMI_APN_LEN);
#else
	srv_dtcnt_prof_str_info_qry_struct out_apn;
	extern MMI_BOOL srv_dtcnt_get_apn(U32 acc_id, srv_dtcnt_prof_str_info_qry_struct *out_apn, srv_dtcnt_account_enum acc_idx);
	out_apn.dest = apn;
	srv_dtcnt_get_apn(account_id,&out_apn,SRV_DTCNT_ACCOUNT_PRIMARY);
#endif
	if(ret)
	{
		mex_wcs2nstr(dest,len,(wchar_t *)apn);
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
void MexInterface_GetSprdMainLcdInfo(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight)
{
	extern uint16 *MexInterface_GUIBLOCK_GetBlockBuffer(void);
	
	if (ppBuffer != NULL)        
		*ppBuffer = (uint16 *)MexInterface_GUIBLOCK_GetBlockBuffer();
	if (puWidth != NULL)
		*puWidth = LCD_WIDTH;
	if (puHeight != NULL)
		*puHeight = LCD_HEIGHT;
}

void MexInterface_UpdateSprdMainLcd(void)
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUI_RECT_T rect = {0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1};
	
	MMITHEME_InitMainLcdStortUpdateRect();
	MMITHEME_StoreUpdateRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect);
#if (MEX_SPRD_CODE_VERSION < 0x10A1137)      
	MMITHEME_UpdateRect();
#endif
}


bool MexInterface_GetIMEI(E_MEX_Sim_Card_ID sim, int8 *imei,int32 imei_len)
{
	bool bResult;
	MN_DUAL_SYS_E  sim_type;
	MN_IMEI_T   imei_t;
	
#if (MEX_SDK_VER >= 1018)
	sim = MexConfig_ConvertSimCard(sim);
#endif
	
	sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);    
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	if(sim == E_MEX_SIM_2)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
	}
#endif
	
#if (MEX_SDK_VER >= 1018) && (defined(MEX_THIRD_SIM)||defined(MEX_QUAD_SIM)||defined (__MET_MEX_SIM_PLUS__))   //支持三SIM卡
	else if(sim == E_MEX_SIM_3)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
	}    
#endif //MEX_THIRD_SIM
#ifdef __MET_MEX_SIM_PLUS__
	else if(sim == E_MEX_SIM_4)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_4);
	}    
#endif/*__MET_MEX_SIM_PLUS__*/
	bResult = MNNV_GetIMEIEx(sim_type, imei_t);
	
	if(bResult == true)
	{
		//转化BCD格式的短消息中心号码为字符串模式
		MexConfig_ConvertBCD2Ansi(
			imei, 
			imei_len,
			(int8*)imei_t, 
			MN_MAX_IMEI_LENGTH,
			1);
	}
	
	return bResult;
}

void MexInterface_GetImsi_Req(funcptr *fpCB,E_MEX_Sim_Card_ID eSimId)
{
	MN_IMSI_T           imsi_t;
	MN_DUAL_SYS_E       sim_type;
	int8                imsi[17];
	
#if (MEX_SDK_VER >= 1018)
	eSimId = MexConfig_ConvertSimCard(eSimId);
#endif
	
	sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_1);
	
#if defined(MEX_DUAL_SIM) || defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM)
	if(eSimId == E_MEX_SIM_2)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_2);
	}
#endif
	
#if (MEX_SDK_VER >= 1018) && (defined(MEX_THIRD_SIM) || defined(MEX_QUAD_SIM) ||defined (__MET_MEX_SIM_PLUS__))   //支持三SIM卡
	else if(eSimId == E_MEX_SIM_3)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_3);
	}    
#endif //MEX_THIRD_SIM
#ifdef __MET_MEX_SIM_PLUS__
	else if(eSimId == E_MEX_SIM_4)
	{
		sim_type = MexConfig_ConvertSprdDef(E_S2MDC_MN_DUAL_SYS_4);
	}    
#endif/*__MET_MEX_SIM_PLUS__*/
	
	memset(imsi,0,sizeof(imsi));   
	imsi_t =  MNSIM_GetImsiEx(sim_type);
	if(MN_MAX_IMSI_ARR_LEN >= imsi_t.imsi_len)
	{        
		//转化BCD格式的短消息中心号码为字符串模式
		MexConfig_ConvertBCD2Ansi(
			imsi, 
			sizeof(imsi),
			(int8*)imsi_t.imsi_val, 
			imsi_t.imsi_len,
			1);
		
		if(fpCB)
		{
			((PsFuncPtr)fpCB)((void *)imsi);       /*lint !e611 */
		}
	}
	else
	{
		if(fpCB)
		{
			((PsFuncPtr)fpCB)((void *)NULL);       /*lint !e611 */
		}
	}
	
}

#endif    


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

//调用资源管理接口时传入的变量。在回调返回的时候必须置空。只能通过全局变量返回。
typedef struct 
{
	int32 sel_type;
	wchar_t path[FILEARRAY_FILENAME_MAX_LEN+1];
	MMIFMM_FILTER_T filter;
	void*  ret_array ;
	Fmgr_CallBack FmgrCB;
	
}Mex_Fmgr;

static Mex_Fmgr mex_fmgr = {0};

void Mex_Fmgr_CallBack( BOOLEAN  is_success, FILEARRAY   file_array)
{
	uint32  file_num = 0;
	FILEARRAY_DATA_T file_array_data = {0};
	
	//SCI_TraceLow:"MMIBT_OpenFileCallBack, Enter,is_success = %d!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_9233_112_2_18_2_36_54_24,(uint8*)"d",is_success);
	if(is_success)
	{
		file_num = MMIFILEARRAY_GetArraySize(file_array);
		//SCI_TraceLow:"MMIBT_OpenFileCallBack, file_num = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_9237_112_2_18_2_36_54_25,(uint8*)"d", file_num);
		if(file_num > 0)
		{
			//read user selected dir
			if(!MMIFILEARRAY_Read(file_array, 0, &file_array_data))
			{
				MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MET_MEX_FOLDER_NOT_EXIST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			}
			else
			{
				MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);  
			}
			
		}
	}
	mex_fmgr.FmgrCB(file_array_data.filename,0);
	//释放列表
	MMIFILEARRAY_Destroy(&file_array);
	file_array = PNULL;
}

void Mex_Entry_Fmgr_Screen(void)
{    
	if (0 == mex_strlen((const char*)&mex_fmgr.filter))
	{
		MMIFMM_OpenFileWinByLayer(mex_fmgr.path, MMIAPICOM_Wstrlen(mex_fmgr.path), (MMIFMM_FILTER_T*)"*", mex_fmgr.sel_type, TRUE, NULL, mex_fmgr.ret_array, Mex_Fmgr_CallBack,FALSE);
	} 
	else
	{
		MMIFMM_OpenFileWinByLayer(mex_fmgr.path, MMIAPICOM_Wstrlen(mex_fmgr.path), &mex_fmgr.filter, mex_fmgr.sel_type, TRUE, NULL, mex_fmgr.ret_array, Mex_Fmgr_CallBack,FALSE);
	}
	
}

void MexInterface_FmgrSetFilter(E_Mex_Fmgr_Filtertype filter)
{
	char *tfilter = NULL;
	int32 len = mex_strlen((const char*)&mex_fmgr.filter);
	
	switch(filter)
	{
	case E_MFF_TYPE_ALL:      /* this type represent all files (included not supported types, like *.doc */
	case E_MFF_TYPE_UNKNOWN:
	case E_MFF_TYPE_FOLDER:
	case E_MFF_TYPE_FOLDER_DOT:
    case E_MFF_TYPE_COUNT:
		tfilter = "*";
		break;
		// Image type
	case E_MFF_TYPE_BMP:
		tfilter = "*.bmp";
		break;
	case E_MFF_TYPE_JPG:
		tfilter = "*.jpg";
		break;
	case E_MFF_TYPE_JPEG:   
		tfilter = "*.jpeg";
		break;
	case E_MFF_TYPE_GIF:
		tfilter = "*.gif";
		break;
	case E_MFF_TYPE_PNG:
		tfilter = "*.png";
		break;
	case E_MFF_TYPE_WBMP:
		tfilter = "*.wbmp";
		break;
	case E_MFF_TYPE_WBM:
		tfilter = "*.bmp";
		break;
	case E_MFF_TYPE_M3D:
		tfilter = "*.m3d";
		break;
	case E_MFF_TYPE_EMS:
		tfilter = "*.ems";
		break;
	case E_MFF_TYPE_ANM:
		tfilter = "*.anm";
		break;
	case E_MFF_TYPE_SVG:
		tfilter = "*.svg";
		break;
		// Audio type
	case E_MFF_TYPE_IMY:
		tfilter = "*.imy";
		break;
	case E_MFF_TYPE_MID:
		tfilter = "*.mid";
		break;
	case E_MFF_TYPE_MIDI:
		tfilter = "*.midi";
		break;
	case E_MFF_TYPE_WAV:
		tfilter = "*.wav";
		break;
	case E_MFF_TYPE_AMR:
		tfilter = "*.amr";
		break;
	case E_MFF_TYPE_AAC:
		tfilter = "*.aac";
		break;
	case E_MFF_TYPE_MP3:
		tfilter = "*.mp3";
		break;
	case E_MFF_TYPE_MP2:
		tfilter = "*.mp2";
		break;
	case E_MFF_TYPE_VM:
		tfilter = "*.vm";
		break;
	case E_MFF_TYPE_AWB:
		tfilter = "*.awb";
		break;
	case E_MFF_TYPE_AIF:
		tfilter = "*.aif";
		break;
	case E_MFF_TYPE_AIFF:
		tfilter = "*.aiff";
		break;
	case E_MFF_TYPE_AIFC:
		tfilter = "*.aifc";
		break;
	case E_MFF_TYPE_AU:
		tfilter = "*.au";
		break;
	case E_MFF_TYPE_SND:
		tfilter = "*.snd";
		break;
	case E_MFF_TYPE_M4A:
		tfilter = "*.m4a";
		break;
	case E_MFF_TYPE_MMF:
		tfilter = "*.mmf";
		break;
	case E_MFF_TYPE_WMA:
		tfilter = "*.wma";
		break;
		
		// Video type
	case E_MFF_TYPE_3PG:
		tfilter = "*.3pg";
		break;
	case E_MFF_TYPE_MP4:
		tfilter = "*.mp4";
		break;
	case E_MFF_TYPE_MPG:
		tfilter = "*.mpg";
		break;
	case E_MFF_TYPE_AVI:
		tfilter = "*.avi";
		break;
	case E_MFF_TYPE_3G2:
		tfilter = "*.3g2";
		break;
	case E_MFF_TYPE_SDP:
		tfilter = "*.sdp";
		break;
	case E_MFF_TYPE_RAM:
		tfilter = "*.ram";
		break;
		
	case E_MFF_TYPE_WML:
		tfilter = "*.wml";
		break;
	case E_MFF_TYPE_HTML:
		tfilter = "*.html";
		break;
	case E_MFF_TYPE_HTM:
		tfilter = "*.htm";
		break;
	case E_MFF_TYPE_XHTML:
		tfilter = "*.xhtml";
		break;
		
	case E_MFF_TYPE_JAD:
		tfilter = "*.jad";
		break;
	case E_MFF_TYPE_JAR:
		tfilter = "*.jar";
		break;
		
	case E_MFF_TYPE_LRC:
		tfilter = "*.lrc";
		break;
	case E_MFF_TYPE_THEME:
		tfilter = "*.theme";
		break;
	case E_MFF_TYPE_EBOOK_TXT:
		tfilter = "*.txt";
		break;
	case E_MFF_TYPE_EBOOK_PDB:
		tfilter = "*.pdb";
		break;
	case E_MFF_TYPE_FOTA_FULL:
		tfilter = "*.full";
		break;
	case E_MFF_TYPE_URL:
		tfilter = "*.url";
		break;
	}
	
	if (0 == len)
	{
		mex_strcpy((char*)&mex_fmgr.filter,tfilter);
	} 
	else if((0<len) && (len< FMM_FILTER_STRING_MAXLEN-5))
	{
		mex_strcat((char*)&mex_fmgr.filter," ");
		mex_strcat((char*)&mex_fmgr.filter,tfilter);
	}
	else
	{
		return ;
	}
}
int8 MexInterface_FmgrInit(uint32 sel_type, wchar_t* base_path,Fmgr_CallBack pfCB)
{
	mex_fmgr.ret_array = NULL;
	
	if (E_MFST_SELECT_FILE == sel_type)
	{
		mex_fmgr.sel_type = FUNC_FIND_FILE;  //搜索文件
	}
	else if (E_MFST_SELECT_FOLDER == sel_type)
	{
		mex_fmgr.sel_type = FUNC_FIND_FOLDER; //搜索文件夹
	}
	else if (E_MFST_SELECT_FILE_OR_FOLDER == sel_type)
	{ 
		mex_fmgr.sel_type = FUNC_FIND_FILE_AND_FOLDER ;  //搜索文件或文件夹
	}
	else 
	{
		mex_fmgr.sel_type = FUNC_FIND_FILE_AND_ALLFOLER;  //搜索文件和所有文件夹
	}
	
	if (base_path)
	{
        uint32 base_path_len = MMIAPICOM_Wstrlen(base_path);
		memset(mex_fmgr.path, 0, sizeof(mex_fmgr.path));
		MMIAPICOM_Wstrncpy(mex_fmgr.path, base_path, MIN(base_path_len, MMIFILE_FULL_PATH_MAX_LEN ));
	}
	
	if (pfCB)
	{ 
		mex_fmgr.FmgrCB = pfCB;
	} 
	else
	{
		return E_MFR_MEMERROR;
	}
	mex_fmgr.ret_array = MMIFILEARRAY_Create();
	if (!mex_fmgr.ret_array)
	{
		return E_MFR_MEMERROR;
	}
	return E_MFR_SUCCESS;
}

void Mex_Fmgr_exit(void)
{
	MMIFILEARRAY_Destroy(&mex_fmgr.ret_array);
	mex_fmgr.ret_array = PNULL;
}
//获取新短信的index
static uint8 MexPhs_SmsGetIndex()
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	uint16 i = 0;
	while (mmi_frm_sms_index_list[i].previndex != MMI_FRM_SMS_INVALID_INDEX)
		i++;
	return i;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 0;
#endif
	
}

#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

// gdi for spread
static Mex_GdiLayer g_SprdMex_GdiBaseLayer = { 0 };
Mex_GdiLayer *gdi_act_layer = NULL;
unsigned char g_bMexUpdatingSprdLcd = 0;     /*lint !e552*/

#endif

bool MexInterface_gdi_layer_is_multi_layer_enable(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return gdi_layer_is_multi_layer_enable();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return true;
#endif
}

int MexInterface_gdi_layer_multi_layer_enable(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return gdi_layer_multi_layer_enable();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 1;
#endif
	
}

int MexInterface_gdi_layer_multi_layer_disable(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return gdi_layer_multi_layer_disable();
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return 1;
#endif
}


int MexInterface_gdi_layer_get_base_handle(gdi_handle *handle_ptr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return gdi_layer_get_base_handle(handle_ptr);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
    GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };    
    UILAYER_GetLayerInfo(&lcd_dev_info,&g_sprd_baselayer_info);
    g_SprdMex_GdiBaseLayer.width = g_sprd_baselayer_info.layer_width;
    g_SprdMex_GdiBaseLayer.height = g_sprd_baselayer_info.layer_height;
    g_SprdMex_GdiBaseLayer.buf_ptr = UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev());
#else
	MexInterface_GetSprdMainLcdInfo(&g_SprdMex_GdiBaseLayer.buf_ptr, &g_SprdMex_GdiBaseLayer.width, &g_SprdMex_GdiBaseLayer.height);
#endif
	g_SprdMex_GdiBaseLayer.clipx1 = 0;
	g_SprdMex_GdiBaseLayer.clipy1 = 0;
	g_SprdMex_GdiBaseLayer.clipx2 = g_SprdMex_GdiBaseLayer.width - 1;
	g_SprdMex_GdiBaseLayer.clipy2 = g_SprdMex_GdiBaseLayer.height - 1;
        
	*handle_ptr = (gdi_handle) &g_SprdMex_GdiBaseLayer;
	return 1;
#endif
}

int MexInterface_gdi_layer_get_buffer_ptr(void **buf_ptr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return 0;
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	if ( gdi_act_layer == NULL)
		MexInterface_gdi_layer_get_base_handle( (gdi_handle*)&gdi_act_layer );
	if((NULL ==  gdi_act_layer)|| ( NULL == gdi_act_layer->buf_ptr))
	{
        mex_log(MEX_LOG_SYSTEM,"get layer buffer error");
        return 0;
	}
	*buf_ptr = gdi_act_layer->buf_ptr;
	return 1;
#endif
}



int MexInterface_gdi_layer_create(int offset_x, int offset_y, int width, int height, int *handle_ptr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	return gdi_layer_create(offset_x, offset_y, width, height, (gdi_handle *)handle_ptr);
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	Mex_GdiLayer *pNewLayer = (Mex_GdiLayer *)SCI_ALLOC(sizeof(Mex_GdiLayer));
	
	if (pNewLayer != NULL)
	{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
        UILAYER_CREATE_T    create_info = {0};
		g_mex_2append_layer.layer_level = UILAYER_LEVEL_HIGH;
        if (UILAYER_IsMultiLayerEnable())
        {
            //creat layer
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = MET_MEX_MAIN_WIN_ID;
            create_info.offset_x = offset_x;
            create_info.offset_y = offset_y;
            create_info.width = width;
            create_info.height = height;
            create_info.is_bg_layer = TRUE;
            create_info.is_static_layer = TRUE; 
            UILAYER_CreateLayer(
                &create_info,
                &(g_mex_2append_layer.lcd_dev_info));
			
            //set color key
            UILAYER_SetLayerColorKey(&(g_mex_2append_layer.lcd_dev_info),TRUE,UILAYER_TRANSPARENT_COLOR);
        }        
        UILAYER_GetLayerInfo(&(g_mex_2append_layer.lcd_dev_info),&g_mex_dblayer_info);
        pNewLayer->buf_ptr = UILAYER_GetLayerBufferPtr(&(g_mex_2append_layer.lcd_dev_info));
		if (pNewLayer->buf_ptr != NULL)
		{
			pNewLayer->width = g_mex_dblayer_info.layer_width;
			pNewLayer->height = g_mex_dblayer_info.layer_height;
			pNewLayer->clipx1 = 0;
			pNewLayer->clipy1 = 0;
			pNewLayer->clipx2 = pNewLayer->width - 1;
			pNewLayer->clipy2 = pNewLayer->height - 1;
			mex_log(MEX_LOG_SYSTEM, "I: CNL w:%d h:%d ptr:%x", width, height, pNewLayer->buf_ptr); // create new layer ptr:%x
		}
		else
		{
			UILAYER_ReleaseLayer(&(g_mex_2append_layer.lcd_dev_info));
			pNewLayer = NULL;
		}
#else
		pNewLayer->buf_ptr = SCI_ALLOC( width * height * 2 + 8 );
		if (pNewLayer->buf_ptr != NULL)
		{
			pNewLayer->width = width;
			pNewLayer->height = height;
			pNewLayer->clipx1 = 0;
			pNewLayer->clipy1 = 0;
			pNewLayer->clipx2 = pNewLayer->width - 1;
			pNewLayer->clipy2 = pNewLayer->height - 1;
			mex_log(MEX_LOG_SYSTEM, "I: CNL w:%d h:%d ptr:%x", width, height, pNewLayer->buf_ptr); // create new layer ptr:%x
		}
		else
		{
			SCI_FREE( pNewLayer );
			pNewLayer = NULL;
		}
#endif
	}
	
	*handle_ptr = (gdi_handle) pNewLayer;
	mex_log(MEX_LOG_SYSTEM, "I: CNL %x", pNewLayer); // create new layer %x
	return 1;
#endif
}

int MexInterface_SetBltLayer(uint32 h1, uint32 h2, uint32 h3, uint32 h4)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if (MEX_MTK_CODE_VERSION >= 0x1108)
	return gdi_layer_set_blt_layer_ext(h1 , h2, h3, h4, 0, 0);
#else
	
#ifdef GDI_6_LAYERS
	return gdi_layer_set_blt_layer_ext(h1 , h2, h3, h4, 0, 0);
#else
	return gdi_layer_set_blt_layer_ext(h1 , h2, h3, h4);
#endif
	
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
    Mex_GdiLayer *handle = (Mex_GdiLayer*)h1;
	if(handle->buf_ptr == UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev()))
	{
		UILAYER_RemoveBltLayer(&(g_mex_2append_layer.lcd_dev_info));
	}
	else
	{
		UILAYER_AppendBltLayer(&g_mex_2append_layer);
	}
#endif   
		return 1;
#endif
}


GUI_LCD_DEV_INFO *MexInterface_Get_Active_Layer(void)
{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
    if(gdi_act_layer->buf_ptr == UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev()))
	{
		return (GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
	}
    else if(gdi_act_layer->buf_ptr == g_mex_select_buf && NULL != g_mex_select_buf)
    {
        return &(g_mex_select_layer.lcd_dev_info);
    }
	else
	{
		return &(g_mex_2append_layer.lcd_dev_info);
	}
#else
	return MMITHEME_GetDefaultLcdDev();
#endif
}

int MexInterface_gdi_layer_free(gdi_handle handle)
{
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return 	gdi_layer_free(handle);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	Mex_GdiLayer *pLayer = (Mex_GdiLayer *) handle;
	
	if (pLayer == NULL)
		return 0;
	
	if (pLayer->buf_ptr != NULL)
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
		UILAYER_ReleaseLayer(&(g_mex_2append_layer.lcd_dev_info));
#else
	SCI_FREE( pLayer->buf_ptr );
#endif
	SCI_FREE( pLayer );
	return 1;
	
#endif
	
	
}

void MexInterface_gdi_layer_set_active(gdi_handle handle)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	gdi_layer_set_active(handle);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	if (handle != NULL)
		gdi_act_layer = (Mex_GdiLayer *) handle;
#else
	
	return;
#endif
}


void MexInterface_gui_BLT_double_buffer(int x1, int y1, int x2, int y2)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	gui_BLT_double_buffer(x1, y1, x2,y2);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	g_bMexUpdatingSprdLcd = true;
	MexInterface_UpdateSprdMainLcd();
	g_bMexUpdatingSprdLcd = false;
#endif
}

void MexInterface_SelectGuiBuffer(Mex_GdiLayer *gdi_layer)
{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
        UILAYER_CREATE_T    create_info = {0};
		g_mex_select_layer.layer_level = UILAYER_LEVEL_HIGH;
        if (UILAYER_IsMultiLayerEnable())
        {        
            if(UILAYER_NULL_HANDLE != g_mex_select_layer.lcd_dev_info.block_id)
            {
    			UILAYER_SetLayerBufferPtr(&(g_mex_select_layer.lcd_dev_info),
                            g_mex_old_layer_buf,
                            g_mex_old_layer_rawbuf,
                            NULL,
                            NULL);
                UILAYER_ReleaseLayer(&(g_mex_select_layer.lcd_dev_info));
                g_mex_select_layer.lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
            }
            if(gdi_layer->buf_ptr == UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev()))
            {
                return;
            }
            //creat layer
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = MET_MEX_MAX_WIN_ID;
            create_info.offset_x = gdi_layer->clipx1;
            create_info.offset_y = gdi_layer->clipy1;
            create_info.width = gdi_layer->width;
            create_info.height = gdi_layer->height;
            create_info.func_ptr = NULL;
            create_info.is_bg_layer = TRUE;
            create_info.is_static_layer = TRUE; 
            UILAYER_CreateLayer(
                &create_info,
                &(g_mex_select_layer.lcd_dev_info));
            
            if(UILAYER_NULL_HANDLE != g_mex_select_layer.lcd_dev_info.block_id)
            {
    			UILAYER_SetLayerBufferPtr(&(g_mex_select_layer.lcd_dev_info),
                            gdi_layer->buf_ptr,
                            gdi_layer->buf_ptr,
                            &g_mex_old_layer_buf,
                            &g_mex_old_layer_rawbuf);
                g_mex_select_buf = gdi_layer->buf_ptr;
            }
        }        
#endif
}


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexInterface_Resize_Handle(void)
{
#if MEX_SDK_VER >= 1006	
	{	
		void *ptr;
		bool bCameraCapReq;
#if MEX_SDK_VER >= 1013
		extern Mex_Camera_Cap_Req req_data;
#endif
		bCameraCapReq = Mex_Phs_Is_CameraCapReq();
		kal_prompt_trace(MOD_NIL,"MexInterface_Resize_Handle");
		if( bCameraCapReq )
		{	
			kal_prompt_trace(MOD_NIL,"MexInterface_Resize_Handle start req");
			//buf_addr = DRV_Reg32(RESZ_RGB_BUF2);	 
			//切换buffer
			ptr = MexCamera_GetCameraWriteBuffer();	
			DRV_WriteReg32(RESZ_RGB_BUF1,ptr);
			DRV_WriteReg32(RESZ_RGB_BUF2,ptr);
			
			Mex_Phs_Rest_CameraCapReq();
			
#if MEX_SDK_VER >= 1013
			req_data.data_cb(); 
#endif	
			
			/*
			else
			{
			bCapReq = false;
			kal_prompt_trace(MOD_NIL,"MexInterface_Resize_Handle end req");
			Mex_Phs_Rest_CameraCapReq();	
			DRV_WriteReg32(RESZ_RGB_BUF1,buf_addr);
			DRV_WriteReg32(RESZ_RGB_BUF2,buf_addr);
			#if MEX_SDK_VER >= 1013
			req_data.data_cb();	
			#endif
			}
			*/
		}
	}
#endif
}

#ifdef MEX_PLATFORM_FOR_MTK6253

#if defined(DRV_IDP_MT6236_SERIES) ||(MEX_MTK_CODE_VERSION >= 0x1112)
void RESZ_HISR(void)
{
}
#endif

void OVERRIDE(RESZ_HISR)(void)
{
#ifndef WIN32
#if MEX_SDK_VER >= 1006
	
	kal_uint32 reg;
	extern bool MexCamera_IsPreview(void);
	if(!MexCamera_IsPreview())
	{
		
		kal_prompt_trace(MOD_NIL,"enter real resz_hisr");
		REAL(RESZ_HISR)();
		return;
	}
	
	reg = DRV_Reg32(RESZ_INT_STATUS);
	
	
	kal_prompt_trace(MOD_NIL,"enter %s %08x", __func__, reg);
	
	if (reg & RESZ_INT_LCD_BIT)
	{           
		kal_prompt_trace(MOD_NIL,"enter %s RESZ_INT_LCD_BIT", __func__);
	}
	if (reg & RESZ_INT_MEMO1_BIT)
	{             
		kal_prompt_trace(MOD_NIL,"enter %s RESZ_INT_MEMO1_BIT", __func__);
		MexInterface_Resize_Handle();	
	}
	
	if (reg & RESZ_INT_FSTART_BIT)
	{
		kal_prompt_trace(MOD_NIL,"enter %s RESZ_INT_FSTART_BIT", __func__);        
	}
	
	if (reg & RESZ_INT_PXDINT_BIT)
	{
		kal_prompt_trace(MOD_NIL,"enter %s RESZ_INT_PXDINT_BIT", __func__);    
	}
	
	if (reg & RESZ_INT_MEMO2_BIT)
	{
		kal_prompt_trace(MOD_NIL,"enter %s RESZ_INT_MEMO2_BIT", __func__);
	}
	
	IRQUnmask(IRQ_RESZ_CODE);
	
	return;
#endif
#endif
}
#endif
#if MEX_SDK_VER >= 1013
int8 MexInterface_GetCurrPorfile_AlertType(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	srv_prof_alert_type_enum alert_type;
	srv_prof_get_current_profile_value(SRV_PROF_SETTINGS_ALERT_TYPE ,(void*)&alert_type);
	switch (alert_type)
	{
	case SRV_PROF_ALERT_TYPE_RING:	
		return E_MPAT_RING;
	case SRV_PROF_ALERT_TYPE_VIB_ONLY:
		return E_MPAT_VIBRATION_ONLY;
		
	case SRV_PROF_ALERT_TYPE_VIB_AND_RING:	 
		return E_MPAT_VIBRATION_AND_RING;
		
	case SRV_PROF_ALERT_TYPE_VIB_THEN_RING:
		return E_MPAT_VIBRATION_THEN_RING;
		
	case SRV_PROF_ALERT_TYPE_SILENT:
		return E_MPAT_SILENT;
	default:
		return E_MPAT_ALERT_NONE;
	}
#else
	
#if (MEX_MTK_CODE_VERSION > 0x0936)
	extern mmi_profiles_cntx_struct gcurrentprofile;
#else
	extern PROFILE gcurrentprofile;
#endif
	switch(gcurrentprofile.mtCallAlertTypeEnum)
	{
	case MMI_ALERT_NONE:
		return E_MPAT_ALERT_NONE;
	case MMI_RING:
		return E_MPAT_RING;
	case MMI_VIBRATION_ONLY:
		return E_MPAT_VIBRATION_ONLY;
	case MMI_VIBRATION_AND_RING:
		return E_MPAT_VIBRATION_AND_RING;
	case MMI_VIBRATION_THEN_RING:
		return E_MPAT_VIBRATION_THEN_RING;
	case MMI_SILENT:
		return E_MPAT_SILENT;
	}
	
	return E_MPAT_ALERT_NONE;
#endif
}
#endif

void MexInterface_NetWorkPause(void)
{
	extern void MexSoc_PauseNetWork(void);
	extern bool MexSystem_IsSysKernalRunning(void);
	if(MexSystem_IsSysKernalRunning())
	{
		MexSoc_PauseNetWork();
	}
	
}

void MexInterface_NetWorkResume(void)
{
	extern void MexSoc_ResumeNetWork(void);
	extern bool MexSystem_IsSysKernalRunning(void);
	if(MexSystem_IsSysKernalRunning())
	{
		MexSoc_ResumeNetWork();
	}
}
#endif

E_Mex_LcdRotateType g_mex_lcd_rotate_type = E_MLRT_Rotate_0;

E_Mex_LcdRotateType MexInterface_GetLcdRotateType(void)
{
	return g_mex_lcd_rotate_type;
}

bool MexInterface_SetLcdRotateType(E_Mex_LcdRotateType eRotateType )
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern void mmi_frm_screen_rotate(mmi_frm_screen_rotate_enum rotation);
	uint8 rotate_type = 0;
	bool ret = false;
	switch( eRotateType )
	{
	case E_MLRT_Rotate_0:
		rotate_type = MMI_FRM_SCREEN_ROTATE_0;
		break;
		
	case E_MLRT_Rotate_90:
		rotate_type = MMI_FRM_SCREEN_ROTATE_90;
		break;
		
	case E_MLRT_Rotate_180:
		rotate_type = MMI_FRM_SCREEN_ROTATE_180;
		break;
		
	case E_MLRT_Rotate_270:
		rotate_type = MMI_FRM_SCREEN_ROTATE_270;
		break;
		
	default:
		return false;
	}
	
#ifdef __MMI_SCREEN_ROTATE__
	mmi_frm_screen_rotate(rotate_type);
	g_mex_lcd_rotate_type = eRotateType;
	ret = true;
#endif
	
	return ret;
	
#else
	//todo: 展讯lcd旋转接口
	uint8 rotate_type = 0;
	bool ret = false;
	switch( eRotateType )
	{
	case E_MLRT_Rotate_0:
		rotate_type = LCD_ANGLE_0;
		break;
		
	case E_MLRT_Rotate_90:
		rotate_type = LCD_ANGLE_90;
		break;
		
	case E_MLRT_Rotate_180:
		rotate_type = LCD_ANGLE_180;
		break;
		
	case E_MLRT_Rotate_270:
		rotate_type = LCD_ANGLE_270;
		break;
		
	default:
		return false;
	}
#if (MEX_SPRD_CODE_VERSION > 0x09AFFFF)
	GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,(LCD_ANGLE_E)rotate_type);
#endif
	g_mex_lcd_rotate_type = eRotateType;
	ret = true;
	
	return ret;
#endif
}

bool MexInterface_EnableLcdRotate(bool flag)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	// TODO: 是否允许旋转LCD
	return true;
#else
#if (MEX_SPRD_CODE_VERSION > 0x09AFFFF)
	MMI_HANDLE_T win_handle = MMK_GetFocusWinHandle();
	if(flag)
	{
		MMK_SetWinSupportAngle(win_handle,WIN_SUPPORT_ANGLE_ALL);
	}
	else
	{
		MMK_SetWinSupportAngle(win_handle,WIN_SUPPORT_ANGLE_0);
		MMK_SetWinAngle(win_handle,LCD_ANGLE_0,TRUE);
	}
	return true;
#else
	return true;
#endif
#endif
}

bool MexInterface_CheckLcdRotate( uint8 rotate_type )
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	extern bool MexSystem_IsSysRunning(void);
	extern int32 mex_SendMessage(int32 iSrcAppId,int32 iDstAppId,E_Mex_MessageType eMessage,int32 iMsgData0,int32 iMsgData1,int32 iMsgData2,int32 iMsgData3);
	int32 ret;
	
	int32 iNewWidth = GDI_LCD_WIDTH;
	int32 iNewHeight = GDI_LCD_HEIGHT;
	
	switch( rotate_type )
	{
	case MMI_FRM_SCREEN_ROTATE_0:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_0;
		break;
		
	case MMI_FRM_SCREEN_ROTATE_90:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_90;
		iNewWidth = GDI_LCD_HEIGHT;
		iNewHeight = GDI_LCD_WIDTH;
		break;
		
	case MMI_FRM_SCREEN_ROTATE_180:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_180;
		break;
		
	case MMI_FRM_SCREEN_ROTATE_270:
		iNewWidth = GDI_LCD_HEIGHT;
		iNewHeight = GDI_LCD_WIDTH;
		g_mex_lcd_rotate_type = E_MLRT_Rotate_270;
		break;
		
	default:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_0;
		break;
	}
	
	if( !MexSystem_IsSysRunning() )
	{
		return true;
	}
	
	ret = MexSystem_NotifyLcdRotate( g_mex_lcd_rotate_type, iNewWidth, iNewHeight );
	
	if(ret)
	{
		//有的版本不做旋转，这里强制旋转一下
#ifdef __MMI_SCREEN_ROTATE__
		extern void mmi_frm_screen_rotate(mmi_frm_screen_rotate_enum rotation);
		mmi_frm_screen_rotate(rotate_type);
#endif
	}
	
	return (bool)ret;
	
#else
	//todo: 展讯lcd旋转接口
	extern bool MexSystem_IsSysRunning(void);
	extern int32 MexSystem_NotifyLcdRotate(E_Mex_LcdRotateType eLcdRotateType, int32 iNewWidth, int32 iNewHeight );

	int32 ret;
	
	int32 iNewWidth = LCD_WIDTH;
	int32 iNewHeight = LCD_HEIGHT;
	
	switch( rotate_type )
	{
	case LCD_ANGLE_0:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_0;
		break;
		
	case LCD_ANGLE_90:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_90;
		iNewWidth = LCD_HEIGHT;
		iNewHeight = LCD_WIDTH;
		break;
		
	case LCD_ANGLE_180:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_180;
		break;
		
	case LCD_ANGLE_270:
		iNewWidth = LCD_HEIGHT;
		iNewHeight = LCD_WIDTH;
		g_mex_lcd_rotate_type = E_MLRT_Rotate_270;
		break;
		
	default:
		g_mex_lcd_rotate_type = E_MLRT_Rotate_0;
		break;
	}
	
	if( !MexSystem_IsSysRunning() )
	{
		return true;
	}
	
	ret = MexSystem_NotifyLcdRotate( g_mex_lcd_rotate_type, iNewWidth, iNewHeight );
	
	return (bool)ret;
	
#endif
	
}

//切换摄像头
//参数是否为后置摄像头
void MexInterface_SwitchCamera(bool iSBack)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if 0//defined(DUAL_CAMERA_SUPPORT)
	extern kal_bool IsBackSensorSelect(void);
	if(iSBack == IsBackSensorSelect())
	{
		return ;
	}
	else
	{
		SelectSensor( iSBack );//0 front sensor    1  back sensor
	}
#endif
#endif    
}


// Adm funcs
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

void* mex_adm_create(void* pMemAddr, uint32 iMemSize, uint32* subpool_size, bool islogging)
{
    extern int32  mex_mem_adm_create(void* pMemAddr, uint32 uMemSize);
	return (void*)mex_mem_adm_create(pMemAddr,iMemSize);
}

int32 mex_adm_delete(void* adm_id)
{
    extern void mex_mem_adm_delete(void* pAdm);
	if (adm_id == NULL)
		return 1;
	mex_mem_adm_delete(adm_id);
	return 0;
}

void* mex_adm_alloc(void* adm_id, uint32 size, char* file, uint32 line)
{
    extern void* mex_mem_adm_logalloc(void* pAmd, uint32 size, uint16 line_num, char* file_name);
	void* pRet = NULL;
	if (adm_id == NULL || size == 0)
		return NULL;
	pRet = (void*)mex_mem_adm_logalloc(adm_id,size,(uint16)line,file);
	return pRet;
}

void mex_adm_free(void* adm_id, void *mem_addr)
{
    extern void mex_mem_adm_free(void* pAmd,void* ptr);
	if (adm_id == NULL || mem_addr == NULL)
		return;
	mex_mem_adm_free(adm_id,mem_addr);
}

uint32 mex_adm_get_total_left_size(void* adm_id)
{
    extern uint32 mex_mem_adm_getLeftSize(void* pAdm);
	if (adm_id == NULL)
		return 0;
	return mex_mem_adm_getLeftSize(adm_id);
}

#endif

void* mex_adm_internal_alloc(void* adm_id, uint32 size, char* filename, uint32 line)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	extern void *kal_adm_internal_alloc(KAL_ADM_ID adm_id, kal_uint32 size, char *filename, kal_uint32 line);
	return kal_adm_internal_alloc(adm_id, size, filename, line);
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
    void* mex_adm_alloc(void* adm_id, uint32 size, char* file, uint32 line);
	return mex_adm_alloc(adm_id, size, filename, line);
#endif
}

#if defined(MT6225) || defined(MT6223) || defined(MT6253) || defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifndef MAX_MPU_CHANNEL_LEN
#define MAX_MPU_CHANNEL_LEN     (0x1000000)
#define MIN_MPU_CHANNEL_LEN     (0x400)
#endif


#define ADDR_ALIGN_ERR -1
#define SIZE_ERR -2
#define GET_CHNL_ERR -3

int32 Mex_SetRORegion(uint32 start_addr, uint32 *total_size)
{
	
#ifdef WIN32
	return 0;
#else
#if defined(MEX_PLATFORM_MTK) && (defined(MT6225) || defined(MT6253))&& defined(__MTK_L1CACHEABLE__)
	
	MPUMenuType    MPUsetting = {0};
	U32     end_addr = start_addr + *total_size;
	U32     size;
	kal_bool       retval;
	
	/* specify the attribute of MPU setting */
	MPUsetting.m_attribute = READABLE_BUT_NON_WRITEABLE;   
	
	/* specify enable flag of MPU setting */
	MPUsetting.m_enable = 1;   
	
	if((start_addr & (MIN_MPU_CHANNEL_LEN-1)) != 0x0 )
		return ADDR_ALIGN_ERR;
	
	if(start_addr + *total_size > end_addr )
		return SIZE_ERR;
	
	/* try to find a max-fit size */
	for (size = MIN_MPU_CHANNEL_LEN; size <= MAX_MPU_CHANNEL_LEN; size <<= 1)
	{
		/* check if the start address is aligned to the size */
		if (start_addr & (size-1)) {
			/* not aligned */
			/* revert to the previous size */
			size >>= 1;
			break;
		}
		
		/* check if size best hit */
		if (size == *total_size) {
			break;
		}
		
		/* Now determine if coverage premitted or not */
		if (size > *total_size) {
			/* check if the configuration of this size will overwrite the next region */
			if ((start_addr + size) > end_addr) {
				/* revert to the previous size */
				size >>= 1;
			}
			/* find the fit size and exit */
			break;
		}
		
		/* exit when reach max size */
		if (size == MAX_MPU_CHANNEL_LEN) {
			break;
		}
	} /* for ( size = MIN_MPU_CHANNEL_LEN ... MAX_MPU_CHANNEL_LEN ) */
	
	/* specify the start address of MPU setting */
	MPUsetting.m_addr = start_addr;
	
	/* specify the range of MPU setting */
	MPUsetting.m_range = size;
	*total_size = size;
	
	/* get a MPU cahnnel */
	retval = MPUGetChannel(&MPUsetting.m_channel);
	if(retval != 1)
	{
		return GET_CHNL_ERR;
	}
	
	/* configure the MPU */
	MPUSetting(&MPUsetting);
	
	return MPUsetting.m_channel;
#else
	uint32     end_addr = start_addr + *total_size;
	uint32     size;
    
	
	if((start_addr & (MIN_MPU_CHANNEL_LEN-1)) != 0x0 )
		return ADDR_ALIGN_ERR;
	
	if(start_addr + *total_size > end_addr )
		return SIZE_ERR;
	
	/* try to find a max-fit size */
	for (size = MIN_MPU_CHANNEL_LEN; size <= MAX_MPU_CHANNEL_LEN; size <<= 1)
	{
		/* check if the start address is aligned to the size */
		if (start_addr & (size-1)) {
			/* not aligned */
			/* revert to the previous size */
			size >>= 1;
			break;
		}
		
		/* check if size best hit */
		if (size == *total_size) {
			break;
		}
		
		/* Now determine if coverage premitted or not */
		if (size > *total_size) {
			/* check if the configuration of this size will overwrite the next region */
			if ((start_addr + size) > end_addr) {
				/* revert to the previous size */
				size >>= 1;
			}
			/* find the fit size and exit */
			break;
		}
		
		/* exit when reach max size */
		if (size == MAX_MPU_CHANNEL_LEN) {
			break;
		}
	} /* for ( size = MIN_MPU_CHANNEL_LEN ... MAX_MPU_CHANNEL_LEN ) */
	
	*total_size = size;
	
	return 1;
#endif
#endif
}

int32 Mex_SetROCacheable(uint32 start_addr, uint32 total_size)
{
#ifdef WIN32
	return 0;
#else
#if defined(MEX_PLATFORM_MTK) && (defined(MT6225) || defined(MT6253)) && defined(__MTK_L1CACHEABLE__)
	
	CacheMenuType  CACHEsetting;
	U32     size;
	kal_bool       retval;
	
	/* specify enable flag of CACHE setting */
	CACHEsetting.c_enable = 1;      
	
	/* specify the start address of CACHE setting */
	CACHEsetting.c_addr = start_addr;
	
	/* specify the range of CACHE setting */
	CACHEsetting.c_range = total_size;
	
	/* get a CACHE cahnnel */
	retval = CodeCacheGetChannel(&CACHEsetting.c_channel);
	if(retval != 1)
	{
		return GET_CHNL_ERR;
	}
	
	/* configure the CACHE */
	CodeCacheSetting(&CACHEsetting);
	return CACHEsetting.c_channel;
#else
	return 1;
#endif
#endif
}


void Mex_ClearRORegion(int32 mpu_chnl)
{
#ifdef WIN32
	
#else
#if defined(MEX_PLATFORM_MTK) && (defined(MT6225) || defined(MT6253))&& defined(__MTK_L1CACHEABLE__)
	U32 interruptMask;
	U16 nChannel = mpu_chnl;
	
	interruptMask = SaveAndSetIRQMask();
	*(MPU_PORT0 + nChannel) = 0xc0;
	RestoreIRQMask(interruptMask);
	
	MPUFreeChannel(&nChannel);
#else
	// do nothing
#endif
#endif
}

void Mex_ClearROCacheable(int32 cache_chnl)
{
#ifdef WIN32
#else
#if defined(MEX_PLATFORM_MTK) && (defined(MT6225) || defined(MT6253)) && defined(__MTK_L1CACHEABLE__)
	U32  interruptMask;
	U16 nChannel = cache_chnl;
	
	interruptMask = SaveAndSetIRQMask();
	*(MPU_CACHE0 + nChannel) = 0;
	RestoreIRQMask(interruptMask);
	
	CodeCacheFreeChannel(&nChannel);
#else
	// do nothing
#endif
#endif
	
}

typedef struct
{
	int32   mpu_chnl;     // MPU Channel
	int32   cache_chnl;   // Code Cache Channel
	uint32  cache_size;
} Mex_CodeCache;

int32 MexInterface_SetROCache(uint32 start_addr, uint32 total_size, uint32 *cache_size)
{
	int32 mpu_chnl = -1;
	int32 cache_chnl = -1;
	Mex_CodeCache* cache_id = (Mex_CodeCache*)mex_malloc(sizeof(Mex_CodeCache));       /*lint !e611 */
	
	if (cache_id == NULL)
		goto SetROCache_Error;
	mpu_chnl = Mex_SetRORegion(start_addr, &total_size);
	if (mpu_chnl < 0)
		goto SetROCache_Error;
	cache_chnl = Mex_SetROCacheable(start_addr, total_size);
	if (mpu_chnl < 0)
		goto SetROCache_Error;
	*cache_size = total_size;
	cache_id->mpu_chnl = mpu_chnl;
	cache_id->cache_chnl = cache_chnl;
	cache_id->cache_size = total_size;
	mex_log(MEX_LOG_SYSTEM, "I: SROC %d %d %d %x ok", mpu_chnl, cache_chnl, total_size, cache_id);
	return (int32)cache_id;
	
SetROCache_Error:
	mex_log(MEX_LOG_SYSTEM, "E: SROC %d %d %d %x", mpu_chnl, cache_chnl, total_size, cache_id);
	if (mpu_chnl >= 0)
		Mex_ClearRORegion( mpu_chnl );
	if (cache_chnl >= 0)
		Mex_ClearROCacheable( cache_chnl );
	if (cache_id != NULL)
		mex_free( cache_id );       /*lint !e611 */
	*cache_size = 0;
	return (int32)NULL;
}

bool MexInterface_ClearROCache(int32 rocache_id)
{
	Mex_CodeCache* cache_id = (Mex_CodeCache*)rocache_id;
	mex_log(MEX_LOG_SYSTEM, "I: CROC %x", cache_id);
	if(NULL == cache_id || cache_id->mpu_chnl < 0 || cache_id->cache_chnl < 0)
	{
        mex_log(MEX_LOG_SYSTEM, "I: CROC invalid param");
        return false;
	}
	if (cache_id != NULL)
	{
		mex_log(MEX_LOG_SYSTEM, "I: CROC %d %d %d", cache_id->mpu_chnl, cache_id->cache_chnl, cache_id->cache_size);
		Mex_ClearROCacheable( cache_id->cache_chnl );
		Mex_ClearRORegion( cache_id->mpu_chnl );
		mex_free( cache_id );       /*lint !e611 */
		mex_log(MEX_LOG_SYSTEM, "I: CROC ok");
		return true;
	}
	return false;
}
#else
int32 MexInterface_SetROCache(uint32 start_addr, uint32 total_size, uint32 *cache_size)
{
	return (int32)NULL;
}

bool MexInterface_ClearROCache(int32 rocache_id)
{
	return false;
}
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION >= 0x1032) || defined(__MMI_VECTOR_FONT_SUPPORT__)
#include "LangModuleGProt.h"

typedef struct
{
	mmi_lm_cluster_info cluster;
	mmi_lm_cluster_info prev_cluster;
	mmi_lm_cluster_info prev2_cluster;
} Mex_CharClusterInfo;
#endif

void* MexInterface_InitCharClusterInfo(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032) || defined(__MMI_VECTOR_FONT_SUPPORT__)
	Mex_CharClusterInfo* pRet = mex_malloc(sizeof(Mex_CharClusterInfo));
	if (pRet != NULL)
	{
        mmi_lm_cluster_init(&pRet->prev_cluster);
        mmi_lm_cluster_init(&pRet->prev2_cluster);
        mmi_lm_cluster_init(&pRet->cluster);   
	}
	return pRet;
#else
	//assert(0);
	return NULL;
#endif
}

void MexInterface_ReleaseCharClusterInfo(void* pCharClusterInfo)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032) || defined(__MMI_VECTOR_FONT_SUPPORT__)
	if (pCharClusterInfo != NULL)
	{
		mex_free(pCharClusterInfo);
	}
#else
	//assert(0);
#endif
}

bool MexInterface_GetCharClusterInfo(void* CharClusterInfo, void* szStart, void* szEnd, int* pAdvX)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032) || defined(__MMI_VECTOR_FONT_SUPPORT__)
	Mex_CharClusterInfo* pCharClusterInfo = (Mex_CharClusterInfo*) CharClusterInfo;
	if(NULL == pCharClusterInfo || NULL == pAdvX)
	{
        return false;
	}
    memcpy(&pCharClusterInfo->prev2_cluster, &pCharClusterInfo->prev_cluster, sizeof(mmi_lm_cluster_info));
    memcpy(&pCharClusterInfo->prev_cluster, &pCharClusterInfo->cluster, sizeof(mmi_lm_cluster_info));            
    mmi_lm_cluster_init(&pCharClusterInfo->cluster);   
	
	pCharClusterInfo->cluster.data_ptr = szStart;
    mmi_lm_get_current_cluster(pCharClusterInfo->cluster.data_ptr, szEnd, &pCharClusterInfo->prev2_cluster, &pCharClusterInfo->prev_cluster, &pCharClusterInfo->cluster);
	*pAdvX = pCharClusterInfo->cluster.adv_x;
	return true;
#else
	return false;
#endif
}

void MexInterface_remove_dialer_screen_exit_handler(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
    // TODO:
    return;
#else
    mmi_idle_remove_dialer_screen_exit_handler();
#endif
	
}

void MexInterface_AsciiToUSC2(signed char* out_usc2, const signed char* in_ascii, int ascci_len)
{
	int ascii_index = 0;
	int usc2_index = 0;
	
	while(ascii_index < ascci_len)
	{
		if (in_ascii[ascii_index] != '\0')
		{
			out_usc2[usc2_index] = in_ascii[ascii_index];
			out_usc2[usc2_index + 1] = 0;
			
			usc2_index += 2; 
		}
		ascii_index++;
	}
	
	out_usc2[usc2_index] = 0;
	out_usc2[usc2_index + 1] = 0;
}

#endif // MEX_PLATFORM_MODIS || MEX_PLATFORM_MTK

//绘制指定个数的一行文字
int32 MexInterface_ShowStringN(uint32 x, uint32 y, const wchar_t* string, int32 len)
{
#if (MEX_MTK_CODE_VERSION >= 0x08B0)
	{
		extern U32 mmi_fe_show_string_n(U32 x_unsigned,U32 y_unsigned,U8 *String,S32 Len);
		return mmi_fe_show_string_n(x,y,(U8 *)string,len);
	}
#else
	{
		//extern U32 ShowString_n(U32 x_unsigned,U32 y_unsigned,stFontAttribute Font,U8 BackGround,U8 *String,int Len,U32 LineHeight);
		//ShowString_n(x,y,0,);
        return x;
	}	
#endif
}

#define MEX_EXTRACT_RGB565TOGRB888(clr, r, g, b)	    { r = ((clr) & 0xf800) >> 8; g = ((clr) & 0x07E0) >> 3; b = (clr & 0x001F) << 3; }


void MexInterface_SetFontColor(mex_color color)
{
#if (MEX_MTK_CODE_VERSION >= 0x0932)
	{
		extern void mmi_fe_set_text_color(mmi_fe_color c);
		mmi_fe_color c;
		c.a = 100;
		
		MEX_EXTRACT_RGB565TOGRB888(color,c.r,c.g,c.b);
		
		mmi_fe_set_text_color(c);
		
	}
#endif
}

uint32 MexInterface_GetStringWidthHeight_variant(wchar_t *string,int32 n,int32 *pWidth,int32 *pHeight,int32 max_width)
{
#if (MEX_MTK_CODE_VERSION >= 0x0932)
	{
		extern U32 Get_StringWidthHeight_variant(U8 *string, S32 w, S32 n, S32 *pWidth, S32 *pHeight, S32 max_width, U8 checkLineBreak, U8 checkCompleteWord);
		return Get_StringWidthHeight_variant((U8*)string, 0, n, pWidth, pHeight, max_width, 0, 0);
	}
#endif
	return 0;
}


int MexInterface_Rgb565ToYuv420( unsigned int *srcYUV, 
								unsigned char *dstY, unsigned char *dstU, unsigned char *dstV, 
								int width, int height )
{
#if  (MEX_MTK_CODE_VERSION >= 0x0932)
	extern int iul_rgb565_to_yuv420_non_interleave(
		unsigned int *srcYUV, 
		unsigned char *dstY, unsigned char *dstU, unsigned char *dstV, 
		int width, int height);
	return iul_rgb565_to_yuv420_non_interleave(
		srcYUV, 
		dstY, dstU, dstV, 
		width, height);
	
#else
	return 0;
#endif
}

bool MexInterface_SetDateTime(Mex_DateTime DateTime)
{
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if  (MEX_MTK_CODE_VERSION < 0x1112)
	extern MMI_BOOL PhnsetSetDTFromAT(MYTIME mytime);
	MYTIME mytime = {0};
	
	mytime.nYear = DateTime.year;
	mytime.nMonth = DateTime.month;
	mytime.nDay = DateTime.day;
	mytime.nHour = DateTime.hour;
	mytime.nMin = DateTime.minute;
	mytime.nSec = DateTime.second;
	
	return PhnsetSetDTFromAT( mytime );
#else
	extern kal_uint8 applib_dt_last_day_of_mon(kal_uint8 month, kal_uint16 year);
	extern void mmi_dt_set_rtc_dt(MYTIME * mytime);
	static MYTIME mytime = {0};
	
	mytime.nYear = DateTime.year;
	mytime.nMonth = DateTime.month;
	mytime.nDay = DateTime.day;
	mytime.nHour = DateTime.hour;
	mytime.nMin = DateTime.minute;
	mytime.nSec = DateTime.second;
	if (mytime.nYear < 1970 || mytime.nYear > 2030
		|| mytime.nMonth > 12 || (mytime.nMonth == 0) || (mytime.nDay == 0) ||
		mytime.nDay > applib_dt_last_day_of_mon(mytime.nMonth, mytime.nYear) || mytime.nHour > 23 || mytime.nMin > 59)
	{
		return false;
	}
	mmi_dt_set_rtc_dt(&mytime);
	
	return true;
#endif
	
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
	SCI_DATE_T      sys_date = {0};
	SCI_TIME_T      sys_time = {0};
	
	sys_date.year = DateTime.year;
	sys_date.mon  = DateTime.month;
	sys_date.mday = DateTime.day;
	sys_date.wday = DateTime.dayIndex;
	
	sys_time.hour = DateTime.hour;
	sys_time.min  = DateTime.minute;
	sys_time.sec  = DateTime.second;
	
	TM_SetSysDate(sys_date);
	TM_SetSysTime(sys_time);
    return true;
#else
#error MexInterface_SetDateTime TODO...
#endif
	
	
}


void MexInterface_Invalidate_Caches(void* mem_buf, uint32 iMemSize)
{
#if defined(MEX_PLATFORM_MTK)
	
#if defined(MT6236)
	invalidate_arm9_icache((kal_uint32)mem_buf, (kal_uint32)iMemSize);
	clean_and_invalidate_arm9_dcache((kal_uint32)mem_buf, (kal_uint32)iMemSize);
#endif
	
#endif
	
}


void MexInterface_med_Switch_CacheType(void* mem_ptr, uint32 buffer_size, bool b_cacheable)
{
#if defined(MEX_PLATFORM_MTK)
	
#if defined(MT6236)
	if (KAL_TRUE == b_cacheable)
	{
		if(KAL_TRUE == INT_QueryIsNonCachedRAM((kal_uint32 *)mem_ptr, buffer_size))
		{
			dynamic_switch_cacheable_region(&mem_ptr, buffer_size, 1);
		}
	}
	else
	{
		if(KAL_TRUE == INT_QueryIsCachedRAM((kal_uint32 *)mem_ptr, buffer_size))
		{
			dynamic_switch_cacheable_region(&mem_ptr, buffer_size, 0);
		}
	}
#endif
	
#endif
}

void *MexInterface_Med_Alloc_Cacheable(uint32 iSize)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
#if defined(MT6236)
	return med_alloc_ext_mem_cacheable(iSize);
#else
	return med_alloc_ext_mem(iSize);
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return SCI_ALLOC(iSize);
#endif
}



void *MexInterface_Med_Alloc(uint32 iSize)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	return med_alloc_ext_mem(iSize);
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	return SCI_ALLOC(iSize);
#endif
	
}

void MexInterface_Med_Free(void *ptr)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	med_free_ext_mem(&ptr);
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	if(ptr != SCI_NULL)
		SCI_FREE(ptr);
#endif
}

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)

#if (MEX_SPRD_CODE_VERSION == 0x09A1025)
PUBLIC void IM_SetCurInputMode(uint32 cur_input_mode)
{
	extern uint32  s_cur_input_mode;             //当前输入法类型
	
	s_cur_input_mode = cur_input_mode;
}
#endif

void MexInterface_GetFontInfo(
							  uint8 **code_ptr,        //out，字库指针
							  uint8 *fontwidth_ptr,    //out，字宽
							  uint8 *fontheight_ptr,   //out，字高
							  GUI_FONT_T font,         //in，字体
							  uint16 char_unicode         //in, uincode
							  )
{
	*fontwidth_ptr = GUI_GetFontWidth(font,char_unicode);
	*fontheight_ptr = GUI_GetFontHeight(font,char_unicode);    
	
	*code_ptr = NULL;
}


int32 MexInterface_DrawString(
							  const wchar_t *string,
							  int32 start,
							  int32 len,
							  int32 x, 
							  int32 y,
							  mex_color clr,
							  uint8 fontAttr,
							  uint8 fontSize,
							  int16 charSpace)
{
	
	wchar_t    tmp_string[2] = {0};
	uint8      tmp_fontSize = 0;
	GUI_RECT_T draw_rect = {0}; 
	GUI_RECT_T clip_rect = {0};        
	uint16    *puOringnalDisplayBuff = NULL;
	uint16     iDisplayHeight;
	
	// 声明临时保存字符编码用变量
	register uint16 unicode;
	
	int32           tmp_x;
	
	GUI_LCD_DEV_INFO    *p_lcd_dev_info = MexInterface_Get_Active_Layer();
	
	//获取裁剪区域
	mex_GetBufferClipper(&clip_rect.left, &clip_rect.top, &clip_rect.right, &clip_rect.bottom);
	
	
	iDisplayHeight = GUI_GetStringHeight(
		MexConfig_ConvertSprdDef(E_S2MDC_SONG_FONT_16),
		string,
		mex_wcslen(string));
	
	if(((clip_rect.top - y) >= iDisplayHeight) || (y >= clip_rect.bottom))
	{
		return x;
	}
#if (MEX_DRAW_STRING_CLIPPER_SUPPORT)
	{
		extern unsigned char MexSystem_IsSysRunning(void);
		if ( MexSystem_IsSysRunning() )
		{	
			//保存文字显示区域原来的类容(单行显示)
			puOringnalDisplayBuff = mex_malloc(sizeof(uint16) * iDisplayHeight * gdi_act_layer->width);       /*lint !e611 */
			if(puOringnalDisplayBuff != NULL)
			{
				mex_memcpy(puOringnalDisplayBuff, 
					((uint16 *)gdi_act_layer->buf_ptr + y * gdi_act_layer->width),
					(sizeof(uint16) * iDisplayHeight * gdi_act_layer->width)); 
			}
		}
	}
#endif
	
	//设置显示区域
	draw_rect.left = x;
	draw_rect.top = y;
	draw_rect.right  = gdi_act_layer->width - 1;
	draw_rect.bottom = gdi_act_layer->height - 1;
	
	tmp_x = x;
	
	//循环显示
	while(*string)
	{
		unicode = *string;
		
		switch(fontSize)
		{
		case MEX_FONTSIZE_SMALL:
			if(unicode & 0xFF80)
				tmp_fontSize = MexConfig_ConvertSprdDef(E_S2MDC_SONG_FONT_16);
			else
				tmp_fontSize = MexConfig_ConvertSprdDef(E_S2MDC_SONG_FONT_12);
			break;
		case MEX_FONTSIZE_MEDIUM:
			tmp_fontSize = MexConfig_ConvertSprdDef(E_S2MDC_SONG_FONT_16);
			break;
		case MEX_FONTSIZE_LARGE:
		case MEX_FONTSIZE_SUBLCD:
		case MEX_FONTSIZE_DIALER:
		case MEX_FONTSIZE_VIRTUAL_KEYBOARD:
        default:    
			return x;
		}
		
		tmp_string[0] = unicode;
		
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
		LCD_DisplayStringInRect(
			p_lcd_dev_info,
			draw_rect,
			tmp_string,
			1,
			charSpace,
			clr,
			tmp_fontSize,
			0
			);
#elif (defined(MEX_PLATFORM__FOR_SPRD6800H))
		{
			GUISTR_STYLE_T  tFontStyle = {0};
			MMI_STRING_T    tString = {0};
			
			tFontStyle.font_color = clr;
			tFontStyle.char_space = charSpace;
			tFontStyle.font = (GUI_FONT_T)tmp_fontSize;
			
			tString.wstr_ptr = tmp_string;
			tString.wstr_len = 1;
			
			GUISTR_DrawTextToLCDInRect(
				p_lcd_dev_info,
				&draw_rect,
				&draw_rect,
				&tString,
				&tFontStyle,
				0,
				0);
		}        
#endif
		tmp_x += (GUI_GetFontWidth((GUI_FONT_T)tmp_fontSize,unicode) + charSpace);
		
		++string;
		
		if(tmp_x > gdi_act_layer->clipx2)
		{
			tmp_x = gdi_act_layer->clipx2;
			break;
		}
		
		if(--len == 0)
			break;
		
		draw_rect.left = tmp_x;
	}
	
#if (MEX_DRAW_STRING_CLIPPER_SUPPORT)
	{
		int i;
		uint16 *dst_ptr, *src_ptr;
		int16   iClipY;
		
		//将裁剪区域的内容复制到保存的显示区域内容缓冲
		if(puOringnalDisplayBuff != NULL)
		{
			if(y < clip_rect.top)
			{
				iClipY = clip_rect.top - y;
				src_ptr = (uint16 *)gdi_act_layer->buf_ptr + (clip_rect.top * gdi_act_layer->width) + clip_rect.left;
				if(iClipY > iDisplayHeight)
				{
					iClipY = iClipY % iDisplayHeight;
				}
			}
			else
			{
				iClipY  = 0;
				src_ptr = (uint16 *)gdi_act_layer->buf_ptr + (y * gdi_act_layer->width) + clip_rect.left;      
			}
			
			dst_ptr = puOringnalDisplayBuff + (iClipY * gdi_act_layer->width) + clip_rect.left;
			
			for(i = 0; i < iDisplayHeight - iClipY; i++)
			{
                int16 t_width =  clip_rect.right - clip_rect.left;
                if(t_width > 0)
                {
					mex_memcpy(dst_ptr, 
						src_ptr,
						(sizeof(uint16) * (uint16)t_width)); 
                }
                else
                {
                    break;
                }
				
				dst_ptr += gdi_act_layer->width;
				src_ptr += gdi_act_layer->width;
			}
			
			//将叠加后的内容复制到显示缓存
			mex_memcpy(((uint16 *)gdi_act_layer->buf_ptr + y * gdi_act_layer->width),
				puOringnalDisplayBuff,
				(sizeof(uint16) * iDisplayHeight * gdi_act_layer->width)); 
			
			mex_free(puOringnalDisplayBuff);       /*lint !e611 */
		}
	}
	
#endif
	
	draw_rect.left = x;
	MMITHEME_StoreUpdateRect(MexInterface_Get_Active_Layer(), draw_rect);
	
	return tmp_x;
}

uint16 *MexInterface_GUIBLOCK_GetBlockBuffer()
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };        
#if defined(MEX_PLATFORM__FOR_SPRD6600L)    
	return (uint16 *)GUIBLOCK_GetBlockBuffer(&lcd_dev_info);
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
	return (uint16 *)GUIBLOCK_GetBuf(&lcd_dev_info);
#endif
}

int32 MexInterface_SFS_DeleteDirectory(	wchar_t *pathName,wchar_t *fileName)
{
#if (MEX_SPRD_CODE_VERSION == 0x09A0937)
	return SFS_DeleteTree(pathName,fileName,NULL,NULL);
#elif (MEX_SPRD_CODE_VERSION >= 0x09A1025)
	return SFS_DeleteDirectory(pathName);
#endif
}

bool MexInterface_GetEnableKeyRing()
{
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
	return MMIDEFAULT_IsEnableKeyRing();
#else
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)
	return MMIDEFAULT_GetEnableKeyRing();
#else
	return MMIAUDIO_IsEnableKeyRing();
#endif  
#endif
}

bool MexInterface_GetEnableTpRing()
{
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
	return MMIDEFAULT_IsEnableTpRing();
#else
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)
	return MMIDEFAULT_GetEnableTpRing();
#else
	return MMIAUDIO_IsEnableTpRing();
#endif
#endif
}

void MexInterface_EnableKeyRing(bool bIsEnable)
{
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
	
#else
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)
	MMIDEFAULT_EnableKeyRing(bIsEnable);
#else
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_RING,MMIBGPLAY_MODULE_NONE,bIsEnable);
#endif
#endif
}

void MexInterface_EnableTpRing(bool bIsEnable)
{
#if (MEX_SPRD_CODE_VERSION == 0x10A6530)
	
#else
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)
	MMIDEFAULT_EnableTpRing(bIsEnable);
#else
	MMIAUDIO_EnableKeyRing(MMIAUDIO_TP_RING,MMIBGPLAY_MODULE_NONE,bIsEnable);
#endif
#endif
}

#if (MEX_SCREEN_ESPECIAL_VERSION)
void MexInterface_gdi_get_lcd_size(uint16 *puiWidth,uint16 *puiHeight)
{
    extern uint16 g_now_screen_width;
    extern uint16 g_now_screen_height;
	
	if(puiWidth != NULL)
        *puiWidth = g_now_screen_width;
	if(puiHeight != NULL)
        *puiHeight = g_now_screen_height;    
}
#endif

void MexInterface_ResumeDefaultAudioDeviceMode(void)
{
#if 0
#if defined(MEX_PLATFORM_SPREAD)
	if (MexConfig_GetPhoneConfig(E_MPC_EarphonePlugged))//查询耳机是否插入
		mex_set_soundOutputPath(E_MSOP_OUTPUT_SPEAKER2); 
	else
#if (MEX_SPRD_CODE_VERSION==0x09A0937)
		mex_set_soundOutputPath(E_MSOP_OUTPUT_LOUDSPEAKER);
#else
	mex_set_soundOutputPath(E_MSOP_OUTPUT_SPEAKER);
#endif
#endif
#endif
	
}
//如果展讯调用sound_play 播放声音与系统其他一些声音播放冲突时
//请在下面实现冲突处理
bool MexInterface_IsPermitPlaySound(void)
{
	if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID)
#ifdef MMI_RECORD_SUPPORT
		||MMIAPIRECORD_IsOpened()
#endif
		||MMK_IsFocusWin(FM_MAIN_WIN_ID))//mp3 fm or record is playing
		return false;
    else
		return true;
}

int32 MexInterface_SetFileAttributes(const wchar_t * FileName, uint16 Attributes)
{
	BOOLEAN result_hidden = FALSE;
	BOOLEAN result_read_only = FALSE;
	BOOLEAN result_system = FALSE;
	BOOLEAN result_archive = FALSE;
	wchar *file_name_ptr = PNULL;
	wchar *path_name_ptr = PNULL;
    wchar attr[20] = {0};
	uint16 str_len = MMIAPICOM_Wstrlen(FileName);
	uint16 file_name_len = 0;
	file_name_len = str_len;
	
	if ( (Attributes & MEX_FS_ATTR_HIDDEN) == MEX_FS_ATTR_HIDDEN )	//隐藏
		result_hidden = TRUE;
	
	if ( (Attributes & MEX_FS_ATTR_READ_ONLY) == MEX_FS_ATTR_READ_ONLY )	//只读
		result_read_only = TRUE;
	
	if ( (Attributes & MEX_FS_ATTR_SYSTEM) == MEX_FS_ATTR_SYSTEM )		//系统
		result_system = TRUE;
	
	if ( (Attributes & MEX_FS_ATTR_ARCHIVE) == MEX_FS_ATTR_ARCHIVE )		//存档
		result_archive = TRUE;
	
    if(NULL == FileName || 0 == str_len)
    {
        return -1;
    }
	
    if(result_hidden || result_read_only || result_system || result_archive)
    {
		path_name_ptr = SCI_ALLOC((str_len + 1)* sizeof(wchar));
		if(path_name_ptr == PNULL)
		{
			//SCI_TraceLow:"[MMIFILE] SetAttr NO memory !"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_11197_112_2_18_2_36_59_26,(uint8*)"");
			return -1;
		}
		memset(path_name_ptr, 0, (str_len + 1)* sizeof(wchar));
		MMIAPICOM_GetFilePath(FileName, str_len, path_name_ptr, &file_name_len);
		if(file_name_len + 1 < str_len )
		{
			file_name_ptr = (wchar*)&(FileName[file_name_len + 1]);
		}
        MMIAPICOM_Wstrcpy(attr,L"/SA:");
        if(result_hidden)
        {
            MMIAPICOM_Wstrcat(attr,L"+H");
        }
        if(result_read_only)
        {
            MMIAPICOM_Wstrcat(attr,L"+R");
        }
        if(result_system)
        {
            MMIAPICOM_Wstrcat(attr,L"+S");
        }
        if(result_archive)
        {
            MMIAPICOM_Wstrcat(attr,L"+A");
        }
        SFS_SetAttrAsyn(path_name_ptr,file_name_ptr,attr,0);
		SCI_FREE(path_name_ptr);
    }
	
    return 1;
}

#endif


//与其他应用程序的冲突
//比如艺术字,语音王,和其他im应用

bool b_MexConfictSuspendFlag = false;

//进入mex界面之前需要中断的应用
void MexInterface_Conflict_Suspend(void)
{
	
	if(b_MexConfictSuspendFlag)
	{
		return ;
	}
	
	b_MexConfictSuspendFlag = true;
	
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	//	酷宇  艺术字体 
#ifdef __COOTYPE_SUPPORT__
	CT_Done_Engine_Ex();//退出艺术字体 
#endif
	
	//	斯凯艺术字体
#ifdef __SKY_FONT_FONT__
	sky_font_enable(0); //停止工作 加在入口
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
	
	//其他请根据实际情况添加
}

//退出mex界面后,需要恢复的应用
void MexInterface_Conflict_Resume(void)
{
	
	if(b_MexConfictSuspendFlag)
	{
		b_MexConfictSuspendFlag = false;
	}
	else
	{
		return ;
	}
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	
	//	酷宇  艺术字体
#ifdef __COOTYPE_SUPPORT__
	CT_Init(); //初始化艺术字体
#endif
	
	
	//	斯凯艺术字体
#ifdef __SKY_FONT_FONT__
	sky_font_enable(1); //开始工作 加在退出的地方
#endif
	
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
	
#endif
}

#if defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)    
int32 MexInterface_InPhoneBook(
							   MN_NUMBER_PLAN_E    number_plan,
							   MN_NUMBER_TYPE_E    number_type,
							   uint16              num_len,
							   uint8              *party_num)
{
    uint8				group_id = 0;  
    int32               result = 0;
    uint16              entry_id = 0;
    MMIPB_BCD_NUMBER_T  bcd_num = {0};
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	uint16  storage_id = 0;
#endif
    if(NULL == party_num || 0 == num_len)
    {
        result = -1;
        return result;
    }
    bcd_num.number_len = (num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : num_len;
    bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, number_plan);
    MMI_MEMCPY(&bcd_num.number,MMIPB_BCD_NUMBER_MAX_LEN,party_num,MN_MAX_ADDR_BCD_LEN,bcd_num.number_len);  
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if(MMIPB_SearchPhoneNumber(&bcd_num,&entry_id,&storage_id,&group_id))
#else
		if(MMIPB_SearchPhoneNumber(&bcd_num,&entry_id,&group_id))
#endif
		{
			result = 1;
		}
		return result;
}

int32 MexInterface_aq_PhoneWall(MN_DUAL_SYS_E dual_sys,MN_CALLING_NUMBER_T* call_num_ptr)
{
    int result = 0;
#ifdef MET_MEX_AQ_SUPPORT    

    if(PNULL == call_num_ptr)
    {
        return result;
    }
    if(met_aq_PhoneWall(call_num_ptr->number_plan,call_num_ptr->number_type,call_num_ptr->num_len,(uint8*)call_num_ptr->party_num))
    {
        result = 1;
    }
#endif    
    return result;
}


int32 MexInterface_aq_SmsWall(MN_DUAL_SYS_E dual_sys,MN_CALLED_NUMBER_T* orig_address_ptr)
{
    int result = 0;
#ifdef MET_MEX_AQ_SUPPORT  
    if(PNULL == orig_address_ptr)
    {
        return result;
    }
    if(met_aq_SmsWall(orig_address_ptr->number_plan,orig_address_ptr->number_type,orig_address_ptr->num_len,orig_address_ptr->party_num))
    {
        result = 1;
    }
#endif    
    return result;
}

int32 MexInterface_aq_CheckIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len)
{
    int32 result = 0;
#ifdef MET_MEX_AQ_SUPPORT 
    if(NULL == tele_in || NULL == tele_out || 0 == tele_out_len)
    {
        return result;
    }  
    if(met_aq_GetIPPhone(tele_in, tele_out, tele_out_len))
    {
        result = 1;
    }
#endif
    return result;
}
#endif    

#if MEX_SDK_VER >= 1022
#if defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)   
int32 MexInterface_aq_getCity(char * pNumber, wchar_t *CityName)
{
#ifdef MET_MEX_AQ_SUPPORT  
    uint16  str_length = 0;
	
    met_mex_aq_GetAttachedAreaNameAndReset(pNumber);          
    if(met_mex_aq_NumInq_App_Flag==1)
    {
		
        str_length = MMIAPICOM_Wstrlen((const wchar *) met_mex_aq_strCityNameUnicode);       /*lint !e611 */
		
        if(str_length> 0 )
        {
            MMIAPICOM_Wstrncpy( CityName, (const wchar *)met_mex_aq_strCityNameUnicode, str_length );
            CityName[str_length] = 0;
        }
        return 1;
    }
    return 0;    
#else
    return 0;
#endif
}
int32 MexInterface_aq_CancelDivert(AQ_CallBack callback);
int32 MexInterface_aq_QueryDivert(AQ_CallBack callback);
int32 MexInterface_aq_DivertCall(char *pNumber, AQ_CallBack callback);

AQ_CallBack aq_divert_call_callback = NULL;
AQ_CallBack aq_cancel_divert_callback = NULL;
AQ_CallBack aq_query_divert_callback = NULL;

int32 MexInterface_CheckSetDivertCall(int32 is_true)
{
#ifdef MET_MEX_AQ_SUPPORT    
    if(NULL != aq_divert_call_callback)
    {
        aq_divert_call_callback(is_true);
        aq_divert_call_callback = NULL;
        return TRUE;
    }
#endif
    return FALSE;
}

int32 MexInterface_CheckCancelDivertCall(int32 is_true)
{
#ifdef MET_MEX_AQ_SUPPORT    
    if(NULL != aq_cancel_divert_callback)
    {
        aq_cancel_divert_callback(is_true);
        aq_cancel_divert_callback = NULL;
        return TRUE;
    }
#endif
    return FALSE;
}

int32 MexInterface_CheckQueryDivertCall(int32 is_true,int32 status)
{
#ifdef MET_MEX_AQ_SUPPORT  
    if(NULL != aq_query_divert_callback)
    {
        if(is_true)
        {
            aq_query_divert_callback(status);
        }
        else
        {
            aq_query_divert_callback(-1);
        }
        aq_query_divert_callback = NULL;
        return TRUE;
    }
#endif    
    return FALSE;
}

int32 MexInterface_aq_DivertCall(char *pNumber, AQ_CallBack callback)
{
#ifdef MET_MEX_AQ_SUPPORT    
    extern MMISET_SS_STATUS_T g_mmiset_ss_status;
    MMI_PARTY_NUMBER_T          party_num = {0};
    MN_SS_REGISTER_PARAM_U register_param = {0};
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MN_DUAL_SYS_E e_dual_sys = MN_DUAL_SYS_1;
    uint8 line_status = 0;
    if(NULL == aq_divert_call_callback)
    {
        aq_divert_call_callback = callback;
    }
    else
    {
        return 0;
    }
    e_dual_sys = MMIAPISET_GetActiveSim();
    line_status = MMIAPISET_GetCallLineInfo(e_dual_sys);
    if(0 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    }
    else if ( 1 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATA_TELESERVICES;
    }
    MMIAPICOM_GenPartyNumber( (uint8*)pNumber,strlen(pNumber),&party_num);
    
    //set register_param value
    register_param.forward_num.number_type = party_num.number_type;
    register_param.forward_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
    register_param.forward_num.num_len = party_num.num_len;
    MMI_MEMCPY(register_param.forward_num.party_num,MN_MAX_ADDR_BCD_LEN,party_num.bcd_num,MN_MAX_ADDR_BCD_LEN,party_num.num_len);
	
    ss_string.ton = register_param.forward_num.number_type;
    ss_string.npi = register_param.forward_num.number_plan;
    ss_string.len = register_param.forward_num.num_len;
    MMI_MEMCPY(ss_string.ss_ptr, MN_MAX_SIMAT_DATA_OBJECT_LEN, party_num.bcd_num,MN_MAX_ADDR_BCD_LEN, ss_string.len);       /*lint !e685 */
    
    MNSS_SendSSEx(e_dual_sys, MN_SS_REGISTERSS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
    
    return 1;
#else
    return 0;
#endif
}

int32 MexInterface_aq_QueryDivert(AQ_CallBack callback)
{
#ifdef MET_MEX_AQ_SUPPORT    
    extern MMISET_SS_STATUS_T g_mmiset_ss_status;
    MN_SS_REGISTER_PARAM_U register_param = {0};
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MN_DUAL_SYS_E e_dual_sys = MN_DUAL_SYS_1;
    uint8 line_status = 0;
    if(NULL == aq_query_divert_callback)
    {
        aq_query_divert_callback = callback;
    }
    else
    {
        return 0;
    }
    e_dual_sys = MMIAPISET_GetActiveSim();
    line_status = MMIAPISET_GetCallLineInfo(e_dual_sys);
    if(0 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    }
    else if ( 1 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATA_TELESERVICES;
    }
    g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
    
    MNSS_SendSSEx(e_dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
    
    return 1;
#else
    return 0;
#endif
}

int32 MexInterface_aq_CancelDivert(AQ_CallBack callback)
{
#ifdef MET_MEX_AQ_SUPPORT    
    extern MMISET_SS_STATUS_T g_mmiset_ss_status;
    MN_SS_REGISTER_PARAM_U register_param = {0};
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MN_DUAL_SYS_E e_dual_sys = MN_DUAL_SYS_1;
    uint8 line_status = 0;
    if(NULL == aq_cancel_divert_callback)
    {
        aq_cancel_divert_callback = callback;
    }
    else
    {
        return 0;
    }        
    e_dual_sys = MMIAPISET_GetActiveSim();
    line_status = MMIAPISET_GetCallLineInfo(e_dual_sys);
    if(0 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    }
    else if ( 1 == line_status)
    {
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATA_TELESERVICES;
    }
    
    MNSS_SendSSEx(e_dual_sys, MN_SS_ERASESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
    
    return 1;
#else
    return 0;
#endif
}
#endif
#endif


#if MEX_ROMFILE_INTERFACE
//*****************************yjz start*********************************
void * MexInterface_GetRomFileFirstNameAddr()
{
	return (void *) &g_Mex_Rom_Files[0].szFileName;
}
int MexInterface_GetRomFileCount()
{
    return sizeof(g_Mex_Rom_Files) / sizeof(g_Mex_Rom_Files[0]);
}
int MexInterface_GetRomFilePerSize()
{
    return sizeof(g_Mex_Rom_Files[0]);
}

int MexInterface_GetRomFileLen(int i)
{
	return g_Mex_Rom_Files[i].iFileLen;
}

void * MexInterface_GetRomFileDataAddr(int i, int j)
{
	return (void *) &( g_Mex_Rom_Files[i].pData[j] );
}
//***************************yjz end***************************************
#endif

/* mex record */
#if defined(MEX_PLATFORM_SPREAD)|| defined(MEX_PLATFORM_SIMULATOR)

//#if defined(MEX_PLATFORM_SPREAD)
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109)
#include "dsp_codec_adp.h"
extern  HAUDIO mex_voice_record_getDspMp4Record(void);
// Audio error value.
typedef enum
{
	ERR_AUD_NONE,
		ERR_AUD_PARAM,               // Input parameters is error.
		ERR_AUD_CONFLICT,            // It's playing ring or midi now, don't handle tone.
		ERR_AUD_MAX                  // Reserved, user can't use it.
} ERR_AUD_E;
ERR_AUD_E Mex_AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,
									// else return error value
									BOOLEAN is_enable,               	// SCI_TRUE: Enable,   SCI_FALSE: Disable
									AUD_REC_FORMAT_E recorder_format,
									RECORD_CALLBACK callback_fun    
									)
{
	
	DSP_CODEC_EXT_OPE_START_RECORD_T ptPara = {0}; 
	uint32 temp; 
	AUDIO_RESULT_E status;
	ptPara.recorder_format = recorder_format;
	ptPara.callback_fun = callback_fun;
	ptPara.hAudioHandle = mex_voice_record_getDspMp4Record();
	
	if (is_enable)
	{        
		status = AUDIO_ExeCodecExtOpe(ptPara.hAudioHandle, 
			ANSI2UINT16("EXT_START_RECORD"), 
			DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT, 
			&ptPara,
			&temp);  
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
		if(!VB_GetLineinStatus())
		{
			AUD_EnableAmplifier(SCI_FALSE);           
		}  
#endif
	}
	else
	{
		status = AUDIO_ExeCodecExtOpe(ptPara.hAudioHandle, 
			ANSI2UINT16("EXT_STOP_RECORD"), 
			1, 
			(void *)ptPara.hAudioHandle,
			&temp); 
	}
	
	if(AUDIO_NO_ERROR == status)
	{
		return ERR_AUD_NONE;  //merlin ?      
	}
	else
	{
		return ERR_AUD_MAX;//merlin ?
	}          
}

//for 6800 amr record
void Mex_AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{
	DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
	uint32 temp;
	ptPara.source_buf_ptr = destination_buf_ptr;
	ptPara.data_length = data_length;
	
	AUDIO_ExeCodecExtOpe(mex_voice_record_getDspMp4Record(), 
		ANSI2UINT16("READ_VOICE_DATA"), 
		DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, 
		&ptPara,
		&temp); 
	
}
//pro 1109 do nothing
void Mex_AUD_WriteVoiceData (uint16 * source_buf_ptr,uint16 data_length)
{
	
}
#endif
//amr record
HAUDIO mex_enc_amr_CreateHandle(
								HAUDIOCODEC hAudioCodec, 
								HAUDIODEV hDevice, 
								AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
								)
{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109) 
	return AUDIO_CreateNormalHandle(hAudioCodec,     SCI_NULL, hDevice,     SCI_NULL, pNotifyCallbackFunc);
#else
    return 0;
#endif
}
//是否支持wav或pcm录音
bool mex_record_wav_support(void)
{
#if (MEX_SPRD_CODE_VERSION >= 0x10A1109) //目前6800以上不支持插件录音
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        return false;
    }
	return true;
#else
	return true;
#endif
}
//#endif //MEX_PLATFORM_SPREAD
//声音播放

extern AUDIO_STREAM_GET_SRC_RESULT_E mex_audio_getsrcdata_CB(HAUDIO hAudio, uint8 ** const ppucDataAddr, uint32 *puiDataLength);
#if (MEX_SPRD_CODE_VERSION < 0x10A1109)
AUDIO_STREAM_GET_SRC_RESULT_E mex_audio_getsrcdata_callback(HAUDIO hAudio, uint8 ** const ppucDataAddr, uint32 *puiDataLength, int32 offset)
{
	return mex_audio_getsrcdata_CB(hAudio,ppucDataAddr,puiDataLength);
}
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109)
AUDIO_STREAM_GET_SRC_RESULT_E mex_audio_getsrcdata_callback(
															HAUDIO hAudio, 
															AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
															void *pvOtherInfo
															)
{
	return mex_audio_getsrcdata_CB(hAudio,&(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr),&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));
}
#endif
AUDIO_GETSRCDATA_CALLBACK_PFUNC g_mex_audio_getsrcdata_Func = mex_audio_getsrcdata_callback;
bool  mex_amr_stream_play(HAUDIO* pBuffHandle,int32* pResult)
{
#if (MEX_SPRD_CODE_VERSION < 0x10A1109) 
	extern int32 g_iAmrPlayTimer;
	extern void mex_Play_AMR_Timer_CB(void);
	extern BOOLEAN mex_Play_AMR_Stream_CB(void);
	
	if (0 == g_iAmrPlayTimer)
	{
        MetMex_StartTimer(MetMex_GetPlayAmrTimerId(),50,mex_Play_AMR_Timer_CB,false);
		g_iAmrPlayTimer = 1;
	}
	if(g_iAmrPlayTimer > 0 )
	{
		if(MexConfig_GetPhoneConfig(E_MPC_EarphonePlugged))//耳机插入
			//AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
			MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_EARPHONE);
#else
		MMISRVAUD_SetRoute(MetMex_GetHandle(),MMISRVAUD_ROUTE_EARPHONE);
#endif
		else
			//AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);	
#if (MEX_SPRD_CODE_VERSION != 0x10A6530)
			MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_HANDFREE);
#else
        MMISRVAUD_SetRoute(MetMex_GetHandle(),MMISRVAUD_ROUTE_SPEAKER);
#endif
#if defined(MEX_PLATFORM_SPREAD)
		AUD_StartPlay(AUD_REC_AMR,
			mex_Play_AMR_Stream_CB,
			SCI_TRUE,
			SCI_FALSE);
#endif
	}
	else
		return false;	
	if(pResult != NULL)
		*pResult = MexConfig_ConvertSprdDef(E_S2MDC_ADR_NO_ERROR);//默认成功
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109) 
	extern void mex_audio_CreateStreamFileHandleCallback(HAUDIO hAudio, uint32 notify_info, uint32 affix_info);
	*pBuffHandle = AUDIO_CreateStreamBufferHandle(
		hAMRCodec,
		PNULL,
		hARMVB,
		PNULL,
		(uint8 *)PNULL,
		0,
		MetMex_GetVBDecodeBuf(),
		MetMex_GetVBDecodeBufSize(),
		mex_audio_CreateStreamFileHandleCallback,
		mex_audio_getsrcdata_callback
		);
	if(!(*pBuffHandle))
	{
		//SCI_TraceLow:"arm play, AUDIO_CreateStreamBufferHandle, create handle fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_11800_112_2_18_2_37_0_27,(uint8*)"");
		return false;
	}
	if(pResult != NULL)
		*pResult = AUDIO_Play(*pBuffHandle,0);	
#endif
	return true;
}
bool mex_amr_stream_stop(HAUDIO handle,int32* pResult)
{
#if (MEX_SPRD_CODE_VERSION < 0x10A1109) 
    extern int32 g_iAmrPlayTimer;
    g_iAmrPlayTimer = 0;
#if defined(MEX_PLATFORM_SPREAD)
	//if(E_MSS_SOUND_PLAYING == g_AudioStream.eState)
	AUD_StopPlay();
#else
	return false;
#endif
	if(pResult != NULL)
		*pResult = MexConfig_ConvertSprdDef(E_S2MDC_ADR_NO_ERROR);
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109) 
	if(handle)
	{
		if(pResult != NULL)
			*pResult = AUDIO_Stop(handle );
		AUDIO_CloseHandle(handle);
	}
#endif
	return true;
}

void MexInterface_ReadInputMicData(AUDIO_DEVICE_HANDLE_T* ptr,uint16 *pReadBuff,uint32 uiReadCount,uint32 *pReadLength)
{
#ifndef WIN32    
	uint16 pReadBuffR[VB_FIFO_SIZE] = {0};
	extern void Mex_Record_ReadData (uint16 *puiDestData, uint16 *puiDestDataR, uint32 uiCount, uint32 *puiDataLen);

	Mex_Record_ReadData(pReadBuff, pReadBuffR, uiReadCount, pReadLength);
#endif    

}

uint32 MexInterface_SetADSamplerate(AUDIO_DEVICE_HANDLE_T* ptr,uint32 uiSamplerate)
{
#if defined(MEX_PLATFORM_SPREAD)
#if ((MEX_SPRD_CODE_VERSION < 0x09AFFFF)|| (MEX_SPRD_CODE_VERSION == 0x10A6530))
    typedef uint32 (*SetSamplerateFunc)(uint32 uiSamplerate);
    if(NULL == ptr)
    {
        mex_log(MEX_LOG_MDI,"set ad rate device error");
        return;
    }
    ((SetSamplerateFunc)ptr->tDeviceInfo.tAudioDevOpe.pSetSamplerate)(uiSamplerate);
#elif ((MEX_SPRD_CODE_VERSION >= 0x10A1137)&&(MEX_SPRD_CODE_VERSION != 0x10A6530))      
    extern void __dol_set_ad_samplerate(uint32 uiSamplerate);
    __dol_set_ad_samplerate(uiSamplerate);
#else     
    extern void _DOL_SetADSamplerate(uint32 uiSamplerate);
    _DOL_SetADSamplerate(uiSamplerate);
#endif
#endif
    return AUDIO_NO_ERROR;
}
uint32 MexInterface_SetSoundInputPath(AUDIO_DEVICE_HANDLE_T* ptr,int bIsEarphone)
{
#if defined(MEX_PLATFORM_SPREAD)
    typedef uint32 (*Mex_Audio_SetDevModeFunc)(uint32 uiDevMode);
    if(NULL == ptr)
    {
        mex_log(MEX_LOG_MDI,"set sound input path device error");
        return;
    }
    if(bIsEarphone)
    {
		((Mex_Audio_SetDevModeFunc)ptr->tDeviceInfo.tAudioDevOpe.pSetDevMode)(AUDIO_DEVICE_MODE_EARPHONE);
    }
    else
    {
		((Mex_Audio_SetDevModeFunc)ptr->tDeviceInfo.tAudioDevOpe.pSetDevMode)(AUDIO_DEVICE_MODE_HANDFREE);    
    }
	
#endif
    return AUDIO_NO_ERROR;
}
uint32 MexInterface_RegMicInputCallback(AUDIO_DEVICE_HANDLE_T* ptr,void *pMicInputCallbackFunc)
{
#if defined(MEX_PLATFORM_SPREAD)
    typedef uint32 (*Mex_Audio_MicInputCallbackRegFunc)(void *pMicInputCallbackFunc);
    return (uint32)((Mex_Audio_MicInputCallbackRegFunc)ptr->tDeviceInfo.tAudioDevOpe.pRegRecCallback)(pMicInputCallbackFunc);
#endif
}

void MexInterface_UnRegMicInputCallback(AUDIO_DEVICE_HANDLE_T* ptr,uint32 iMicInputCallbackFunc)
{
#if defined(MEX_PLATFORM_SPREAD)
    typedef void (*Mex_Audio_MicInputCallbackUnRegFunc)(uint32 iMicInputCallbackFunc);
    ((Mex_Audio_MicInputCallbackUnRegFunc)ptr->tDeviceInfo.tAudioDevOpe.pUnRegRecCallback)((uint32)iMicInputCallbackFunc);
#endif
}


/*****************************************************************************/
//  Description: 检测录音硬件是否开始正常工作，消除正常工作之前的噪音波形
//  Author:  LeoFeng       
//  Note: 
/*****************************************************************************/
bool MexInterface_RecordDataFilter(uint16* sample_buffer, int32 sample_len)
{
#define SAMPLE_THREHOLD  (0)
#define SAMPLE_HIGH_THREHOLD  (15000)
#define SAMPLE_LOW_THREHOLD  (-15000)
	
	
    int16  *sample_ptr;
    int16   sample_pos = 0;
    int16   sample_hi,sample_low,sample_temp,invalid_sample_count = 0;
    sample_ptr = (int16*)sample_buffer;
    sample_hi = 0;
    sample_low = 0;    
    for(sample_pos = 0; sample_pos < sample_len; sample_pos++)
    {
        if(*sample_ptr >= SAMPLE_THREHOLD)
        {
			sample_hi++;   
        }
        else
        {
			sample_low++;
        }
		
        if((*sample_ptr >= SAMPLE_HIGH_THREHOLD) || (*sample_ptr <= SAMPLE_LOW_THREHOLD))
        {
            invalid_sample_count++;
            return false;
        }
		
        sample_ptr++;
    }
	
    if((sample_hi == 0) || (sample_low == 0))
    {
        return false;
    }
    
    if(sample_hi < sample_low)
    {
        sample_temp = sample_hi;
        sample_hi = sample_low;
        sample_low = sample_temp;
    }
	
    if((sample_hi/sample_low) > 2)
    {
        return false;
    }
	
    return true;
}

#endif//MEX_PLATFORM_SPREAD || MEX_PLATFORM_SIMULATOR

#if defined(MEX_PLATFORM_SPREAD)
int32 MexInterface_sci_sock_socket(int family,int type, int proto)
{
    return MEX_SOCK_SOCKET(family,type,proto);
}
#endif

#if (defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD))
int32 MexInterface_GetImageDimensionFromFileInternal(const char *image_name, int32 *width, int32 *height)
{
	int32   isSuccess = false;
#if (MEX_SPRD_CODE_VERSION < 0x10A1109) 
	{
#define MEX_IMG_FILE_HEADER_LEN  (1024)
		uint8   *img_file_header;
		int32  *fpImg;
		int32   readBytes;
		wchar_t szImageName[100] = {0};
		mex_str2nwcs(szImageName,100<<1,image_name);
		fpImg = mex_fopenw_org(szImageName,L"rb");
		if(fpImg != NULL)
		{
			isSuccess = false;
			img_file_header = mex_malloc(MEX_IMG_FILE_HEADER_LEN);
			if(img_file_header != NULL)
			{
				readBytes = mex_fread(img_file_header, 1,MEX_IMG_FILE_HEADER_LEN,fpImg);
				if(readBytes > 0)
				{   
					//JPEG特殊处理
					if((readBytes > 10)&& (img_file_header[0] == 0xFF) 
						&& (img_file_header[1] == 0xD8)
						&& (img_file_header[2] == 0xFF)
						&& (img_file_header[3] >= 0xE0)
						&& (img_file_header[3] <= 0xEF)
						)
					{
						uint32 uReadOffset = readBytes;//read 偏移量
						// uint32 uCurOffset = 2;//跳过文件头+APP0标记头  (FFD8 FFE0)
						//APP0标记
						uint16 uAppNAddr = 2;//第一个AppN的位置,标志头  (N取值 1-15)
						//遇到一个APP(N)
						while((img_file_header[uAppNAddr]==0xFF)
							&&(img_file_header[uAppNAddr+1]>=0xE0)&&(img_file_header[uAppNAddr+1]<=0xEF)
							)
						{
							uint16 uAppNLen  = (uint16)((img_file_header[uAppNAddr+2] << 8) | img_file_header[uAppNAddr+3]);//得到AppN的长度
							uAppNAddr += 2;//skip : FF EN
							if(uAppNLen > (readBytes - uAppNAddr))//剩余的数据不够了，继续读
							{
								uAppNLen -= (readBytes - uAppNAddr);
								do
								{
									readBytes = mex_fread(img_file_header, 1,MEX_IMG_FILE_HEADER_LEN,fpImg);
									if((readBytes>0)&&(uAppNLen > readBytes))
										uAppNLen -= readBytes;
									else
										break;
								}while(uAppNLen > 0);
								//APP (n)段读完了
								uAppNAddr = uAppNLen;//下一个APPN的地址 
							}
							else
							{
								uAppNAddr += uAppNLen;//下一个APPN的地址
							}
						}
						//已经把所有的APPN跳过了,搜索SOFO标志
						{
							int32 i = uAppNAddr;
							while(i < (readBytes - 8))
							{
								if((img_file_header[i] == 0xFF) && (img_file_header[i + 1] == 0xC0))  //SOFO标记
								{
									*height = (uint16)((img_file_header[i + 5] << 8) | img_file_header[i + 6]);
									*width = (uint16)((img_file_header[i + 7] << 8) | img_file_header[i + 8]);                   
									isSuccess = true;
									break;                    
								}
								i++;
							}
							//失败了，再读一次
							if(!isSuccess)
							{
								
								readBytes = mex_fread(img_file_header, 1,MEX_IMG_FILE_HEADER_LEN,fpImg);
								i=0;
								while((readBytes > 0)&&(i < (readBytes - 8)))
								{
									if((img_file_header[i] == 0xFF) && (img_file_header[i + 1] == 0xC0))  //SOFO标记
									{
										*height = (uint16)((img_file_header[i + 5] << 8) | img_file_header[i + 6]);
										*width = (uint16)((img_file_header[i + 7] << 8) | img_file_header[i + 8]);                   
										isSuccess = true;
										break;                    
									}
									i++;
								}
							}
						}
						
					}
					else
					{
						isSuccess = mex_GetImageDimensionFromMemInternal(0,img_file_header,readBytes,width,height);
					}
				}
				
				mex_free(img_file_header);
			}
			mex_fclose(fpImg);
		}   
		return 0;
	}
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109)
	{
		//6800可以通过接口获取，而且获取jpg尺寸时也更准确些
		IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
		IMG_DEC_SRC_T       dec_src = {0};
		IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
		wchar_t szImageName[100] = {0};
		mex_str2nwcs(szImageName,100<<1,image_name);
		dec_src.file_name_ptr = szImageName;
		get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
		if(IMG_DEC_RET_SUCCESS == get_result)
		{
			switch (dec_info.img_type)
			{
			case IMG_DEC_TYPE_BMP:
				*width  = dec_info.img_detail_info.bmp_info.img_width;
				*height = dec_info.img_detail_info.bmp_info.img_height;
				break;
			case IMG_DEC_TYPE_WBMP:
				*width = dec_info.img_detail_info.wbmp_info.img_width;
				*height = dec_info.img_detail_info.wbmp_info.img_height;
				break;
			case IMG_DEC_TYPE_JPEG:
				*width  = dec_info.img_detail_info.jpeg_info.img_width;
				*height = dec_info.img_detail_info.jpeg_info.img_heigth;
				break;
			case IMG_DEC_TYPE_PNG:
				*width  = dec_info.img_detail_info.png_info.img_width;
				*height = dec_info.img_detail_info.png_info.img_height;
				break;
			case IMG_DEC_TYPE_GIF:
				*width  = dec_info.img_detail_info.gif_info.img_width;
				*height = dec_info.img_detail_info.gif_info.img_height;
				break;
			case IMG_DEC_TYPE_UNKNOWN:
				break;
			default:
				mex_log(MEX_LOG_OSLIB,"GUIANIM_GetInfo:img_type %d is error!",dec_info.img_type);
				break;
			}
			isSuccess =  true;
		}
		return isSuccess;
	}
#endif
}
int32 MexInterface_GetImageDimensionFromMemInternal(uint8 *img_ptr,int32 img_size,int32 *width, int32 *height)
{
	bool isSuccess =false;
#if (MEX_SPRD_CODE_VERSION < 0x10A1109)
	uint16 img_width=0, img_height=0;
	uint8  image_type=0;
	
	if((img_ptr[0] == 'B') && (img_ptr[1] == 'M'))
		image_type = MEX_IMAGE_TYPE_BMP;
	else if	((img_ptr[1] == 'P') && (img_ptr[2] == 'N') && (img_ptr[3] == 'G'))
		image_type = MEX_IMAGE_TYPE_PNG;
	else if	(     (img_ptr[0] == 0xFF) 
		&& (img_ptr[1] == 0xD8) 
		&& (img_ptr[2] == 0xFF)
		&& (img_ptr[3] >= 0xE0)
		&& (img_ptr[3] <= 0xEF)
		&& (img_ptr[img_size-2] == 0xFF) 
		&& (img_ptr[img_size-1] == 0xD9))//有的jpg文件头标示 是EXIF 所以不能通过JFIF来辨别JPG文件
		image_type = MEX_IMAGE_TYPE_JPG;
	else if	((img_ptr[0] == 'G') && (img_ptr[1] == 'I') && (img_ptr[2] == 'F'))
		image_type = MEX_IMAGE_TYPE_GIF;
	
	
	switch(image_type)
	{
	case MEX_IMAGE_TYPE_BMP:
		
		//SCI_TraceLow:"GetImageDimension BMP"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12143_112_2_18_2_37_1_28,(uint8*)"");
		
		img_width  = (uint16)((img_ptr[0x12]) | (img_ptr[0x13] << 8) | (img_ptr[0x14] << 16) | (img_ptr[0x15] << 24));
		img_height = (uint16)((img_ptr[0x16]) | (img_ptr[0x17] << 8) | (img_ptr[0x18] << 16) | (img_ptr[0x19] << 24));
		
		//SCI_TraceLow:"GetImageDimension width;%d height:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12149_112_2_18_2_37_1_29,(uint8*)"dd",img_width, img_height);
		
		isSuccess = true;
		break;
	case MEX_IMAGE_TYPE_PNG:        
		//SCI_TraceLow:"GetImageDimension PNG"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12154_112_2_18_2_37_1_30,(uint8*)"");
		
		img_width  = (uint16)((img_ptr[0x10] << 24) | (img_ptr[0x11] << 16) | (img_ptr[0x12] << 8) | (img_ptr[0x13]));
		img_height = (uint16)((img_ptr[0x14] << 24) | (img_ptr[0x15] << 16) | (img_ptr[0x16] << 8) | (img_ptr[0x17]));
		
		//SCI_TraceLow:"GetImageDimension width;%d height:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12160_112_2_18_2_37_1_31,(uint8*)"dd",img_width, img_height);
		
		isSuccess = true;
		break;
	case MEX_IMAGE_TYPE_GIF:        
		//SCI_TraceLow:"GetImageDimension GIF"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12165_112_2_18_2_37_1_32,(uint8*)"");
		
		img_width  = (uint16)((img_ptr[0x07] << 8) | (img_ptr[0x6]));
		img_height = (uint16)((img_ptr[0x09] << 24) | (img_ptr[0x08]));
		
		//SCI_TraceLow:"GetImageDimension width;%d height:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MEXINTERFACE_12171_112_2_18_2_37_1_33,(uint8*)"dd",img_width, img_height);
		
		isSuccess = true;
		break;
	case MEX_IMAGE_TYPE_JPG:
		{
			uint32 uCurOffset = 2;//跳过文件头  (FFD8)
			//APP0标记
			uint16 uAppNAddr = uCurOffset;
			//跳过所有的APP
			while((img_ptr[uAppNAddr]==0xFF)
				&&(img_ptr[uAppNAddr+1]>=0xE0)&&(img_ptr[uAppNAddr+1]<=0xEF)//遇到一个APP(N)
				)
			{
				uint16 uAppNLen  = (uint16)((img_ptr[uAppNAddr+2] << 8) | img_ptr[uAppNAddr+3]);//得到AppN的长度
				uCurOffset += uAppNLen+2;//skip : FF EN
				uAppNAddr = uCurOffset;
			}
			//已经把所有的APPN跳过了,搜索SOFO标志
			{
				int32 i = uAppNAddr;
				while(i < (img_size - 8))
				{
					if((img_ptr[i] == 0xFF) && (img_ptr[i + 1] == 0xC0))  //SOFO标记
					{
						img_height = (uint16)((img_ptr[i + 5] << 8) | img_ptr[i + 6]);
						img_width = (uint16)((img_ptr[i + 7] << 8) | img_ptr[i + 8]);                   
						isSuccess = true;
						break;                    
					}
					i++;
				}
			}
		}
		break;
	default:
		img_width = 0;
		img_height = 0;
		isSuccess = false;
		break;
	}
	
	mex_log(MEX_LOG_GUI, "E: GUIRES W&H W:%d, H:%d isSuccess:%d", img_width, img_height,isSuccess);
	
	*width = img_width, *height = img_height;
#elif (MEX_SPRD_CODE_VERSION >= 0x10A1109)
	{
		//6800可以通过接口获取，而且获取jpg尺寸时也更准确些
		IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
		IMG_DEC_SRC_T       dec_src = {0};
		IMG_DEC_INFO_T      dec_info = {0}; 
		if(PNULL == img_ptr)
		{
            mex_log(MEX_LOG_OSLIB,"NULL img ptr get info Error");
            return false;
		}
		dec_src.src_ptr       = img_ptr;  
		dec_src.src_file_size = img_size; 
		get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
		if(IMG_DEC_RET_SUCCESS == get_result)
		{
			switch (dec_info.img_type)
			{
			case IMG_DEC_TYPE_BMP:
				*width  = dec_info.img_detail_info.bmp_info.img_width;
				*height = dec_info.img_detail_info.bmp_info.img_height;
				break;
			case IMG_DEC_TYPE_WBMP:
				*width = dec_info.img_detail_info.wbmp_info.img_width;
				*height = dec_info.img_detail_info.wbmp_info.img_height;
				break;
			case IMG_DEC_TYPE_JPEG:
				*width  = dec_info.img_detail_info.jpeg_info.img_width;
				*height = dec_info.img_detail_info.jpeg_info.img_heigth;
				break;
			case IMG_DEC_TYPE_PNG:
				*width  = dec_info.img_detail_info.png_info.img_width;
				*height = dec_info.img_detail_info.png_info.img_height;
				break;
			case IMG_DEC_TYPE_GIF:
				*width  = dec_info.img_detail_info.gif_info.img_width;
				*height = dec_info.img_detail_info.gif_info.img_height;
				break;
			case IMG_DEC_TYPE_UNKNOWN:
				break;
			default:
				mex_log(MEX_LOG_OSLIB,"GUIANIM_GetInfo:img_type %d is error!",dec_info.img_type);
				break;
			}
			isSuccess =  true;
		}
	}
#endif
	return isSuccess;
}

#if defined(MEX_PLATFORM__FOR_SPRD6800H)
bool MexInterface_Img_Decode(
                             uint32 iWidth,
                             uint32 iHeight,
                             uint32 *iActualWidth,
                             uint32 *iActualHeight,
                             char   *img_ptr,
                             int     img_size,
                             uint8  *dst_buf_ptr,
                             int     dst_buf_size,
                             wchar  *psz_img_name)
{
    
	GUIIMG_SRC_T        img_src = {0};
	GUIIMG_DISPLAY_T    img_display = {0};
	GUIIMG_DEC_OUT_T    img_output = {0};
	bool isSuccess = FALSE;
	
	
    if(img_ptr != NULL)
    {        
        img_src.src_buf_ptr = (uint8*)img_ptr;
        img_src.src_data_size = (uint32)img_size;
    }
    else if(psz_img_name!= NULL)
    {
        img_src.is_file = true;//是文件
        img_src.full_path_ptr = psz_img_name;
    }
    else
    {
        return false;
    }
	
    
	//set image dest width and height
	img_display.dest_width = iWidth;
	img_display.dest_height = iHeight;
	
	//set image decode output data buffer
	img_output.decode_data_ptr = dst_buf_ptr;
	img_output.decode_data_size = dst_buf_size;
	
    //decode 
    isSuccess = GUIIMG_Decode(&img_src,&img_display,&img_output);
	
    if(isSuccess)
    {
        *iActualWidth = img_output.actual_width;
        *iActualHeight = img_output.actual_height;     
    }    
	
    return isSuccess;
}
#endif

#endif

#if MEX_SDK_VER >= 1027
static int32 g_IsCanPowerOff =1;

int32 Mex_Is_Can_PowerOff(void)
{
	return g_IsCanPowerOff;
}

void MexInterface_Set_Can_PowerOff(int32 IsCanPowerOff)
{
	g_IsCanPowerOff = IsCanPowerOff;
}
#endif


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void MexInterface_Set_Aud_State(Mex_Audio_Media_State state)
{
	int32 s;
	extern aud_context_struct *aud_context_p;
	switch(state)
	{
	case E_MEX_AUD_MEDIA_IDLE: 
		s = AUD_MEDIA_IDLE;
		break;
	case E_MEX_AUD_MEDIA_PLAY:
		s = AUD_MEDIA_PLAY;
		break;
	case E_MEX_AUD_MEDIA_PLAY_PAUSED:
		s = AUD_MEDIA_PLAY_PAUSED;
		break;
	case E_MEX_AUD_MEDIA_RECORD:
		s = AUD_MEDIA_RECORD;
		break;
	case E_MEX_AUD_MEDIA_RECORD_PAUSED:
		s = AUD_MEDIA_RECORD_PAUSED;
		break;
	case E_MEX_AUD_MEDIA_SEEK_PAUSED:
		s = AUD_MEDIA_SEEK_PAUSED;
		break;
	case E_MEX_AUD_MEDIA_UNDERFLOW:
		s = AUD_MEDIA_UNDERFLOW;
		break;
	case E_MEX_AUD_MEDIA_WAIT_RECORD:
		s = AUD_MEDIA_WAIT_RECORD;
		break;
	default:
		s = AUD_MEDIA_IDLE;
	}
	
	aud_context_p->state = s;
	
	//AUD_ENTER_STATE(s);
}
#if (MEX_MTK_CODE_VERSION >= 0x1112)
mmi_ret mex_Phs_New_SMS_Ind(mmi_event_struct *evt)
{
    extern int MetMex_RecvieMsg(srv_sms_event_struct* event_data);
    MetMex_RecvieMsg((srv_sms_event_struct*)evt);
	
	return MMI_RET_OK;
    
}
#endif	/*MEX_MTK_CODE_VERSION >= 0x1112*/

#endif

//MEX平台的断言函数，调用系统的断言接口
void MexInterface_Assert(void *szFile, int iMod,int iLineNo)
{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)     
	char curAppFileName[160];
	
#if (0 == MEX_BUILD_FINAL_USER)
	Mex_App  *ptCurrApp;
	
	curAppFileName[0] = 0;
	
	//获取当前应用的名称
	ptCurrApp = (Mex_App *)mex_GetCurrentApp();
	if(ptCurrApp != NULL)
	{
		
		mex_wcs2nstr(curAppFileName,
			160,
			ptCurrApp->m_ProgData.m_szFileName);
		
		mex_log(iMod, "Assert:%s %d Curr App name:%s ro:0x%.8X ro_sram:0x%.8X rw:0x%.8X",
			szFile,
			iLineNo,
			curAppFileName,
			ptCurrApp->m_ProgData.m_ptrRo,
			ptCurrApp->m_ProgData.m_ptrRoSRam,
			ptCurrApp->m_ProgData.m_ptrRwzi);
	}
#else
    wchar_t curAppFileNameW[160];
    mex_GetCurAppFileName(curAppFileNameW);
	
    mex_wcs2nstr(curAppFileName,160,curAppFileNameW);
    mex_log(iMod,"Assert: %s at %d Cur App name:%s",szFile,iLineNo,curAppFileName);
#endif
	
	SCI_Sleep(100);   //等候log文件写入完成
	SCI_Assert(curAppFileName,szFile,iLineNo);/*assert verified*/
#endif
}




#if defined(MEX_G7_UI_BOOT)
static const wchar_t g_szFauiFile[] = L"Faui.cfg";
void Mex_Set_UI_Status(U8 s)
{
	U8 data = s;
	wchar_t *path = (wchar_t*)MexConfig_GetPhoneConfig(E_MPC_MexDir_In_Phone);
	int32 *file = mex_fopenw_ext(g_szFauiFile, L"wb", path, NULL, false);
	if(file)
	{
		mex_fwrite(&data, sizeof(data), 1, file);
		mex_fclose(file);
	}
}

U8 Mex_Get_UI_Status(void)
{
	U8 data = E_MEX_UI_STYLE_DEFAULT;
	wchar_t *path = (wchar_t*)MexConfig_GetPhoneConfig(E_MPC_MexDir_In_Phone);
	int32 *file = mex_fopenw_ext(g_szFauiFile, L"rb", path, NULL, false);
	if(file)
	{
		mex_fread(&data, sizeof(data), 1, file);
		mex_fclose(file);
	}
	return data;
}

static void Mex_phnset_ui_style_list(void)
{
#ifdef __PLUTO_MMI_PACKAGE__
	/*----------------------------------------------------------------*/
	/* Local Variables 											   */
	/*----------------------------------------------------------------*/
	phnset_radio_button_menu_data_struct radio_button_menu;
	U16 nStrItemList[E_MEX_UI_STYLE_MAX];
	
	/*----------------------------------------------------------------*/
	/* Code Body													   */
	/*----------------------------------------------------------------*/
	nStrItemList[E_MEX_UI_STYLE_DEFAULT] = E_MSI_UI_STYLE_DEFAULT;
	nStrItemList[E_MEX_UI_STYLE_FAUI] = E_MSI_UI_STYLE_FAUI;
	
	radio_button_menu.group_id = MET_MEX_UI_STYLE_SETTING_GRP_ID;
	radio_button_menu.menu_title_str_id = E_MSI_UI_STYLE_SETTING;
	radio_button_menu.menu_icon_id = MAIN_MENU_TITLE_SETTINGS_ICON;
	radio_button_menu.menu_item_num = E_MEX_UI_STYLE_MAX;
	radio_button_menu.menu_item_str_list = NULL;
	radio_button_menu.menu_item_str_id_list = nStrItemList;
	radio_button_menu.highlighted_item_idx = Mex_Get_UI_Status();
	
	mmi_phnset_init_radio_button_menu(&radio_button_menu);  
#endif
}

static void Mex_Phnset_Setting_UI_Style(U8 highlighted_item)
{
	S16 error;
	U8 style = highlighted_item;
	Mex_Set_UI_Status(style);
	
	mmi_popup_display_simple((WCHAR*) GetString(STR_GLOBAL_DONE), MMI_EVENT_SUCCESS, GRP_ID_ROOT, NULL);
	DeleteNScrId( IDLE_SCREEN_ID );
	
	if(style == E_MEX_UI_STYLE_DEFAULT)
	{
		if(MexSystem_IsSysKernalRunning())
		{
			Mex_ShutDownAllSystem();
		}
	}
}
mmi_ret Mex_phnset_ui_style_proc(mmi_event_struct *param)
{
	/*----------------------------------------------------------------*/
	/* Local Variables 											   */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body													   */
	/*----------------------------------------------------------------*/
	switch (param->evt_id)
	{
	case EVT_ID_CUI_MENU_LIST_ENTRY:
		Mex_phnset_ui_style_list();
		break;
		
	case EVT_ID_CUI_MENU_ITEM_SELECT:
	case EVT_ID_CUI_MENU_ITEM_TAP:
		g_phnset_cntx_p->curHightlightItem= (U8)((cui_menu_event_struct*)param)->highlighted_menu_id;		  
		Mex_Phnset_Setting_UI_Style((U8)((cui_menu_event_struct*)param)->highlighted_menu_id);
		break;
		
	case EVT_ID_CUI_MENU_CLOSE_REQUEST:
		cui_menu_close(mmi_frm_group_get_active_id());
		break;
		
	case EVT_ID_GROUP_DEINIT:
		if (param->user_data)
		{
			OslMfree(param->user_data);
		}
		break;
		
	default:
		break;
	}
	return MMI_RET_OK;
}

void MexInterface_PhnsetEntryUIStyle(MMI_ID parent_gid)
{
	/*----------------------------------------------------------------*/
	/* Local Variables 											   */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body													   */
	/*----------------------------------------------------------------*/
#if (MEX_MTK_CODE_VERSION < 0x1112)
	mmi_phnset_appcreate_menucui_create(
		parent_gid,
		MET_MEX_UI_STYLE_SETTING_GRP_ID,
		MET_MEX_UI_STYLE_SETTING_MENUID,
		Mex_phnset_ui_style_proc);
#endif
}


#endif
void MexSystem_SendKeyMessage(int32 keyCode, int32 keyEvent);

void Mex_Key_Listener_Key_0_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_0, E_MKE_Up );
}

void Mex_Key_Listener_Key_0_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_0, E_MKE_Down);
}
void Mex_Key_Listener_Key_1_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_1, E_MKE_Up );
}
void Mex_Key_Listener_Key_1_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_1, E_MKE_Down);
}

void Mex_Key_Listener_Key_2_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_2, E_MKE_Up );
}

void Mex_Key_Listener_Key_2_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_2, E_MKE_Down);
}
void Mex_Key_Listener_Key_3_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_3, E_MKE_Up );
}

void Mex_Key_Listener_Key_3_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_3, E_MKE_Down);
}

void Mex_Key_Listener_Key_4_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_4, E_MKE_Up );
}

void Mex_Key_Listener_Key_4_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_4, E_MKE_Down);
}

void Mex_Key_Listener_Key_5_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_5, E_MKE_Up );
}

void Mex_Key_Listener_Key_5_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_5, E_MKE_Down);
}

void Mex_Key_Listener_Key_6_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_6, E_MKE_Up );
}

void Mex_Key_Listener_Key_6_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_6, E_MKE_Down);
}

void Mex_Key_Listener_Key_7_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_7, E_MKE_Up );
}

void Mex_Key_Listener_Key_7_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_7, E_MKE_Down);
}

void Mex_Key_Listener_Key_8_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_8, E_MKE_Up );
}

void Mex_Key_Listener_Key_8_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_8, E_MKE_Down);
}

void Mex_Key_Listener_Key_9_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_9, E_MKE_Up );
}

void Mex_Key_Listener_Key_9_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_9, E_MKE_Down);
}


void Mex_Key_Listener_Key_SoftLeft_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SOFTLEFT, E_MKE_Up );
}

void Mex_Key_Listener_Key_SoftLeft_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SOFTLEFT, E_MKE_Down);
}

void Mex_Key_Listener_Key_SoftRight_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SOFTRIGHT, E_MKE_Up );
}

void Mex_Key_Listener_Key_SoftRight_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SOFTRIGHT, E_MKE_Down);
}

void Mex_Key_Listener_Key_VolUp_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_UP, E_MKE_Up );
}

void Mex_Key_Listener_Key_VolUp_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_UP, E_MKE_Down);
}

void Mex_Key_Listener_Key_VolDown_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_DOWN, E_MKE_Up );
}

void Mex_Key_Listener_Key_VolDown_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_DOWN, E_MKE_Down);
}

void Mex_Key_Listener_Key_Enter_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_ENTER, E_MKE_Up );
}

void Mex_Key_Listener_Key_Enter_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_ENTER, E_MKE_Down);
}


void Mex_Key_Listener_Key_UpArrow_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_UP, E_MKE_Up );
}

void Mex_Key_Listener_Key_UpArrow_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_UP, E_MKE_Down);
}

void Mex_Key_Listener_Key_DownArrow_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_DOWN, E_MKE_Up );
}

void Mex_Key_Listener_Key_DownArrow_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_DOWN, E_MKE_Down);
}


void Mex_Key_Listener_Key_LeftArrow_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_LEFT, E_MKE_Up );
}

void Mex_Key_Listener_Key_LeftArrow_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_LEFT, E_MKE_Down);
}


void Mex_Key_Listener_Key_RightArrow_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_RIGHT, E_MKE_Up );
}

void Mex_Key_Listener_Key_RightArrow_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_RIGHT, E_MKE_Down);
}

void Mex_Key_Listener_Key_Send_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SEND, E_MKE_Up );
}

void Mex_Key_Listener_Key_Send_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_SEND, E_MKE_Down);
}

void Mex_Key_Listener_Key_End_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_END, E_MKE_Up );
}

void Mex_Key_Listener_Key_End_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_END, E_MKE_Down);
}


void Mex_Key_Listener_Key_Clear_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_CLEAR, E_MKE_Up );
}

void Mex_Key_Listener_Key_Clear_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_CLEAR, E_MKE_Down);
}


void Mex_Key_Listener_Key_Star_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_STAR, E_MKE_Up );
}

void Mex_Key_Listener_Key_Star_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_STAR, E_MKE_Down);
}

void Mex_Key_Listener_Key_Pound_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_POUND, E_MKE_Up );
}

void Mex_Key_Listener_Key_Pound_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_POUND, E_MKE_Down);
}

void Mex_Key_Listener_Key_VolumeUp_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_UP, E_MKE_Up );
}

void Mex_Key_Listener_Key_VolumeUp_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_UP, E_MKE_Down);
}

void Mex_Key_Listener_Key_VolumeDown_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_DOWN, E_MKE_Up );
}

void Mex_Key_Listener_Key_VolumeDown_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_VOLUME_DOWN, E_MKE_Down);
}

#if defined(MEX_SUPPORT_KEY_EXTRA_1)
void Mex_Key_Listener_Key_Extra_1_Up(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_EXTRA_1, E_MKE_Up );
}

void Mex_Key_Listener_Key_Extra_1_Down(void)
{
	MexSystem_SendKeyMessage( E_MKC_KEY_EXTRA_1, E_MKE_Down);
}
#endif


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
void Mex_Key_Listener_Key_Handle(void)
{
	uint16 uiKeyCode, uiKeyEvent;
	uint16 uiMexKeyCode, uiMexKeyEvent;
	
	GetkeyInfo(&uiKeyCode,&uiKeyEvent);
	
	switch(uiKeyCode)
    {
    case KEY_0:
        uiMexKeyCode = E_MKC_KEY_0;
        break;
    case KEY_1:
        uiMexKeyCode = E_MKC_KEY_1;
        break;
    case KEY_2:
        uiMexKeyCode = E_MKC_KEY_2;
        break;
    case KEY_3:
        uiMexKeyCode = E_MKC_KEY_3;
        break;
    case KEY_4:
        uiMexKeyCode = E_MKC_KEY_4;
        break;
    case KEY_5:
        uiMexKeyCode = E_MKC_KEY_5;
        break;
    case KEY_6:
        uiMexKeyCode = E_MKC_KEY_6;
        break;
    case KEY_7:
        uiMexKeyCode = E_MKC_KEY_7;
        break;
    case KEY_8:
        uiMexKeyCode = E_MKC_KEY_8;
        break;
    case KEY_9:
        uiMexKeyCode = E_MKC_KEY_9;
        break;
    case KEY_UP_ARROW:
        uiMexKeyCode = E_MKC_KEY_UP;
        break;
    case KEY_DOWN_ARROW:
        uiMexKeyCode = E_MKC_KEY_DOWN;
        break;
    case KEY_LEFT_ARROW:
        uiMexKeyCode = E_MKC_KEY_LEFT;
        break;
    case KEY_RIGHT_ARROW:
        uiMexKeyCode = E_MKC_KEY_RIGHT;
        break;
    case KEY_ENTER:
        uiMexKeyCode = E_MKC_KEY_ENTER;
        break;
#if defined(__MEX_G7_UI_BOOTUP__) || defined(MEX_G7_UI_BOOT)        
    case KEY_END:
        uiMexKeyCode = E_MKC_KEY_END;
        break;
#endif        
    case KEY_SEND:
        uiMexKeyCode = E_MKC_KEY_SEND;
        break;
    case KEY_STAR:
        uiMexKeyCode = E_MKC_KEY_STAR;
        break;
    case KEY_POUND:
        uiMexKeyCode = E_MKC_KEY_POUND;
        break;
    case KEY_LSK:
        uiMexKeyCode = E_MKC_KEY_SOFTLEFT;
        break;
    case KEY_RSK:
        uiMexKeyCode = E_MKC_KEY_SOFTRIGHT;
        break;
    case KEY_VOL_UP:
        uiMexKeyCode = E_MKC_KEY_VOLUME_UP;
        break;
    case KEY_VOL_DOWN:
        uiMexKeyCode = E_MKC_KEY_VOLUME_DOWN;
        break;
#if MEX_SDK_VER >= 1012
		// 清除键        
    case KEY_CLEAR:
        uiMexKeyCode = E_MKC_KEY_CLEAR;
        break;
#endif
		
#if defined(MEX_SUPPORT_KEY_EXTRA_1)        
    case KEY_EXTRA_1:
        uiMexKeyCode = E_MKC_KEY_EXTRA_1;
        break;
#endif        
		
#if defined(__MMI_QWERTY_KEYPAD_SUPPORT__)    
    case KEY_A:
        uiMexKeyCode = E_MKC_KEY_A;
        break;  
    case KEY_B:
        uiMexKeyCode = E_MKC_KEY_B;
        break;  
    case KEY_C:
        uiMexKeyCode = E_MKC_KEY_C;
        break;  
    case KEY_D:
        uiMexKeyCode = E_MKC_KEY_D;
        break;  
    case KEY_E:
        uiMexKeyCode = E_MKC_KEY_E;
        break;  
    case KEY_F:
        uiMexKeyCode = E_MKC_KEY_F;
        break;  
    case KEY_G:
        uiMexKeyCode = E_MKC_KEY_G;
        break;  
    case KEY_H:
        uiMexKeyCode = E_MKC_KEY_H;
        break;  
    case KEY_I:
        uiMexKeyCode = E_MKC_KEY_I;
        break;  
    case KEY_J:
        uiMexKeyCode = E_MKC_KEY_J;
        break;  
    case KEY_K:
        uiMexKeyCode = E_MKC_KEY_K;
        break;  
    case KEY_L:
        uiMexKeyCode = E_MKC_KEY_L;
        break;  
    case KEY_M:
        uiMexKeyCode = E_MKC_KEY_M;
        break;  
    case KEY_N:
        uiMexKeyCode = E_MKC_KEY_N;
        break;  
    case KEY_O:
        uiMexKeyCode = E_MKC_KEY_O;
        break;  
    case KEY_P:
        uiMexKeyCode = E_MKC_KEY_P;
        break;  
    case KEY_Q:
        uiMexKeyCode = E_MKC_KEY_Q;
        break;  
    case KEY_R:
        uiMexKeyCode = E_MKC_KEY_R;
        break;  
    case KEY_S:
        uiMexKeyCode = E_MKC_KEY_S;
        break;  
    case KEY_T:
        uiMexKeyCode = E_MKC_KEY_T;
        break;  
    case KEY_U:
        uiMexKeyCode = E_MKC_KEY_U;
        break;  
    case KEY_V:
        uiMexKeyCode = E_MKC_KEY_V;
        break;  
    case KEY_W:
        uiMexKeyCode = E_MKC_KEY_W;
        break;  
    case KEY_X:
        uiMexKeyCode = E_MKC_KEY_X;
        break;  
    case KEY_Y:
        uiMexKeyCode = E_MKC_KEY_Y;
        break;  
    case KEY_Z:
        uiMexKeyCode = E_MKC_KEY_Z;
        break;  
    case KEY_SPACE:
        uiMexKeyCode = E_MKC_KEY_SPACE;
        break;  
    case KEY_TAB:
        uiMexKeyCode = E_MKC_KEY_TAB;
        break;  
    case KEY_DEL:
        uiMexKeyCode = E_MKC_KEY_DEL;
        break;  
    case KEY_ALT:
        uiMexKeyCode = E_MKC_KEY_ALT;
        break;  
    case KEY_CTRL:
        uiMexKeyCode = E_MKC_KEY_CTRL;
        break;  
    case KEY_WIN:
        uiMexKeyCode = E_MKC_KEY_WIN;
        break;  
    case KEY_SHIFT:
        uiMexKeyCode = E_MKC_KEY_SHIFT;
        break;  
    case KEY_QUESTION:
        uiMexKeyCode = E_MKC_KEY_QUESTION;
        break;  
    case KEY_PERIOD:
        uiMexKeyCode = E_MKC_KEY_PERIOD;
        break;  
    case KEY_COMMA:
        uiMexKeyCode = E_MKC_KEY_COMMA;
        break;  
    case KEY_EXCLAMATION:
        uiMexKeyCode = E_MKC_KEY_EXCLAMATION;
        break;  
    case KEY_APOSTROPHE:
        uiMexKeyCode = E_MKC_KEY_APOSTROPHE;
        break;  
    case KEY_AT:
        uiMexKeyCode = E_MKC_KEY_AT;
        break;  
    case KEY_BACKSPACE:
        uiMexKeyCode = E_MKC_KEY_BACKSPACE;
        break;  
    case KEY_QWERTY_ENTER:
        uiMexKeyCode = E_MKC_KEY_QWERTY_ENTER;
        break;  
    case KEY_FN:
        uiMexKeyCode = E_MKC_KEY_FN;
        break;  
    case KEY_SYMBOL:
        uiMexKeyCode = E_MKC_KEY_SYMBOL;
        break;  
    case KEY_NUM_LOCK:
        uiMexKeyCode = E_MKC_KEY_NUM_LOCK;
        break;  
    case KEY_QWERTY_MENU:
        uiMexKeyCode = E_MKC_KEY_QWERTY_MENU;
        break;  
    case KEY_OK:
        uiMexKeyCode = E_MKC_KEY_OK;
        break;  
#endif        
    default:
        return;
    }
	
	if(uiKeyEvent == MexConfig_ConvertMtkDef(E_M2MDC_KEY_EVENT_DOWN))
    {
		uiMexKeyEvent = E_MKE_Down;
    }
	else if(uiKeyEvent == MexConfig_ConvertMtkDef(E_M2MDC_KEY_EVENT_UP))
    {
		uiMexKeyEvent = E_MKE_Up;
    }
	
    //发送键盘消息给MEX
    MexSystem_SendKeyMessage(uiMexKeyCode, uiMexKeyEvent);
    
}

//mex system key event
void MexSystem_Handler_Register_KeyMouseEvent(void)
{
	
	extern void MexSystem_Handler_Pen_Down(mmi_pen_point_struct point);
	extern void MexSystem_Handler_Pen_Move(mmi_pen_point_struct point);
	extern void MexSystem_Handler_Pen_Up(mmi_pen_point_struct point);
	
    uint16   uiMexKeyGroup[] = 
    {
        KEY_0,
			KEY_1,    
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,    
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,        
			KEY_UP_ARROW,
			KEY_DOWN_ARROW,
			KEY_LEFT_ARROW,
			KEY_RIGHT_ARROW,
			KEY_ENTER,
#if defined(__MEX_G7_UI_BOOTUP__) || defined(MEX_G7_UI_BOOT)        
			KEY_END,
#endif        
			KEY_SEND,
			KEY_STAR,
			KEY_POUND,
			KEY_LSK,
			KEY_RSK,
			KEY_VOL_UP,
			KEY_VOL_DOWN,
#if MEX_SDK_VER >= 1012
			// 清除键        
			KEY_CLEAR,
#endif
			
#if defined(MEX_SUPPORT_KEY_EXTRA_1)        
			KEY_EXTRA_1,
#endif        
			
#if defined(__MMI_QWERTY_KEYPAD_SUPPORT__)    
			KEY_A,
			KEY_B,
			KEY_C,
			KEY_D,
			KEY_E,
			KEY_F,
			KEY_G,
			KEY_H,
			KEY_I,
			KEY_J,
			KEY_K,
			KEY_L,
			KEY_M,
			KEY_N,
			KEY_O,
			KEY_P,
			KEY_Q,
			KEY_R,
			KEY_S,
			KEY_T,
			KEY_U,
			KEY_V,
			KEY_W,
			KEY_X,
			KEY_Y,
			KEY_Z,
			KEY_SPACE,
			KEY_TAB,
			KEY_DEL,
			KEY_ALT,
			KEY_CTRL,
			KEY_WIN,
			KEY_SHIFT,
			KEY_QUESTION,
			KEY_PERIOD,
			KEY_COMMA,
			KEY_EXCLAMATION,
			KEY_APOSTROPHE,
			KEY_AT,
			KEY_BACKSPACE,
			KEY_QWERTY_ENTER,
			KEY_FN,
			KEY_SYMBOL,
			KEY_NUM_LOCK,
			KEY_QWERTY_MENU,
			KEY_OK
#endif        
	};
	
	ClearAllKeyHandler();
	
    SetGroupKeyHandler(Mex_Key_Listener_Key_Handle,
		uiMexKeyGroup,
		(uint8)(sizeof(uiMexKeyGroup) / sizeof(uint16)),
		MexConfig_ConvertMtkDef(E_M2MDC_KEY_EVENT_DOWN));
	
    SetGroupKeyHandler(Mex_Key_Listener_Key_Handle,
		uiMexKeyGroup,
		(uint8)(sizeof(uiMexKeyGroup) / sizeof(uint16)),
		MexConfig_ConvertMtkDef(E_M2MDC_KEY_EVENT_UP));
	//mouse event
	MexInterface_Pen_Reg_Down_Handler( MexSystem_Handler_Pen_Down );
	MexInterface_Pen_Reg_Move_Handler( MexSystem_Handler_Pen_Move );
	MexInterface_Pen_Reg_Up_Handler( MexSystem_Handler_Pen_Up );
	
	mex_log(MEX_LOG_SYSTEM,"Reg key and pen");
	
}
#elif defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)
void Mex_Key_Listener_Key_Handle(uint16 uiKeyCode, uint16 uiKeyEvent)
{
	uint16 uiMexKeyCode, uiMexKeyEvent;
	
	switch(uiKeyCode)
    {
    case KEY_0:
        uiMexKeyCode = E_MKC_KEY_0;
        break;
    case KEY_1:
        uiMexKeyCode = E_MKC_KEY_1;
        break;
    case KEY_2:
        uiMexKeyCode = E_MKC_KEY_2;
        break;
    case KEY_3:
        uiMexKeyCode = E_MKC_KEY_3;
        break;
    case KEY_4:
        uiMexKeyCode = E_MKC_KEY_4;
        break;
    case KEY_5:
        uiMexKeyCode = E_MKC_KEY_5;
        break;
    case KEY_6:
        uiMexKeyCode = E_MKC_KEY_6;
        break;
    case KEY_7:
        uiMexKeyCode = E_MKC_KEY_7;
        break;
    case KEY_8:
        uiMexKeyCode = E_MKC_KEY_8;
        break;
    case KEY_9:
        uiMexKeyCode = E_MKC_KEY_9;
        break;
    case KEY_UP:
        uiMexKeyCode = E_MKC_KEY_UP;
        break;
    case KEY_DOWN:
        uiMexKeyCode = E_MKC_KEY_DOWN;
        break;
    case KEY_LEFT:
        uiMexKeyCode = E_MKC_KEY_LEFT;
        break;
    case KEY_RIGHT:
        uiMexKeyCode = E_MKC_KEY_RIGHT;
        break;
    case KEY_WEB:
        uiMexKeyCode = E_MKC_KEY_ENTER;
        break;
    case KEY_RED:
        uiMexKeyCode = E_MKC_KEY_END;
        break;
    case KEY_STAR:
        uiMexKeyCode = E_MKC_KEY_STAR;
        break;
    case KEY_HASH:
        uiMexKeyCode = E_MKC_KEY_POUND;
        break;
    case KEY_OK:
        uiMexKeyCode = E_MKC_KEY_SOFTLEFT;
        break;
    case KEY_CANCEL:
        uiMexKeyCode = E_MKC_KEY_SOFTRIGHT;
        break;
    case KEY_UPSIDEKEY:
        uiMexKeyCode = E_MKC_KEY_VOLUME_UP;
        break;
    case KEY_DOWNSIDEKEY:
        uiMexKeyCode = E_MKC_KEY_VOLUME_DOWN;
        break;
        
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case KEY_A:
        uiMexKeyCode = E_MKC_KEY_A;
        break;  
    case KEY_B:
        uiMexKeyCode = E_MKC_KEY_B;
        break;  
    case KEY_C:
        uiMexKeyCode = E_MKC_KEY_C;
        break;  
    case KEY_D:
        uiMexKeyCode = E_MKC_KEY_D;
        break;  
    case KEY_E:
        uiMexKeyCode = E_MKC_KEY_E;
        break;  
    case KEY_F:
        uiMexKeyCode = E_MKC_KEY_F;
        break;  
    case KEY_G:
        uiMexKeyCode = E_MKC_KEY_G;
        break;  
    case KEY_H:
        uiMexKeyCode = E_MKC_KEY_H;
        break;  
    case KEY_I:
        uiMexKeyCode = E_MKC_KEY_I;
        break;  
    case KEY_J:
        uiMexKeyCode = E_MKC_KEY_J;
        break;  
    case KEY_K:
        uiMexKeyCode = E_MKC_KEY_K;
        break;  
    case KEY_L:
        uiMexKeyCode = E_MKC_KEY_L;
        break;  
    case KEY_M:
        uiMexKeyCode = E_MKC_KEY_M;
        break;  
    case KEY_N:
        uiMexKeyCode = E_MKC_KEY_N;
        break;  
    case KEY_O:
        uiMexKeyCode = E_MKC_KEY_O;
        break;  
    case KEY_P:
        uiMexKeyCode = E_MKC_KEY_P;
        break;  
    case KEY_Q:
        uiMexKeyCode = E_MKC_KEY_Q;
        break;  
    case KEY_R:
        uiMexKeyCode = E_MKC_KEY_R;
        break;  
    case KEY_S:
        uiMexKeyCode = E_MKC_KEY_S;
        break;  
    case KEY_T:
        uiMexKeyCode = E_MKC_KEY_T;
        break;  
    case KEY_U:
        uiMexKeyCode = E_MKC_KEY_U;
        break;  
    case KEY_V:
        uiMexKeyCode = E_MKC_KEY_V;
        break;  
    case KEY_W:
        uiMexKeyCode = E_MKC_KEY_W;
        break;  
    case KEY_X:
        uiMexKeyCode = E_MKC_KEY_X;
        break;  
    case KEY_Y:
        uiMexKeyCode = E_MKC_KEY_Y;
        break;  
    case KEY_Z:
        uiMexKeyCode = E_MKC_KEY_Z;
        break;  
    case KEY_SPACE:
        uiMexKeyCode = E_MKC_KEY_SPACE;
        break;  
    case KEY_DEL:
        uiMexKeyCode = E_MKC_KEY_DEL;
        break;  
    case KEY_CTRL:
        uiMexKeyCode = E_MKC_KEY_CTRL;
        break;  
    case KEY_SHIFT:
        uiMexKeyCode = E_MKC_KEY_SHIFT;
        break;  
    case KEY_QUESTION:
        uiMexKeyCode = E_MKC_KEY_QUESTION;
        break;  
    case KEY_PERIOD:
        uiMexKeyCode = E_MKC_KEY_PERIOD;
        break;  
    case KEY_COMMA:
        uiMexKeyCode = E_MKC_KEY_COMMA;
        break;  
    case KEY_EXCLAMATION:
        uiMexKeyCode = E_MKC_KEY_EXCLAMATION;
        break;  
    case KEY_AT:
        uiMexKeyCode = E_MKC_KEY_AT;
        break;    
    case KEY_ENTER:
        uiMexKeyCode = E_MKC_KEY_QWERTY_ENTER;
        break;  
    case KEY_FN:
        uiMexKeyCode = E_MKC_KEY_FN;
        break;  
#endif        
    default:
        return;
    }
	
    if(uiKeyEvent == KEY_PRESSED)
    {
        uiMexKeyEvent = E_MKE_Down;
    }
    else if(uiKeyEvent == KEY_RELEASED)
    {
        uiMexKeyEvent = E_MKE_Up;
    }
    else
    {
        // FIXME: 未处理的特殊情况
        return;
    }
	
    //发送键盘消息给MEX
    MexSystem_SendKeyMessage(uiMexKeyCode, uiMexKeyEvent);
    
}
#endif

void Mex_ClearProtocolEventHandler_conflict(void)
{
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
	PsFuncPtr func =NULL;
	mex_mmi_frm_get_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP,&func);
	if(func !=NULL)
	{
		mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP, (PsIntFuncPtr)func);
		func =NULL;
	}
	mex_mmi_frm_get_protocol_event_handler(PRT_GET_IMSI_RSP,&func);
	if(func !=NULL)
	{
		mmi_frm_clear_protocol_event_handler(PRT_GET_IMSI_RSP, (PsIntFuncPtr)func);
		func =NULL;
	} 
	mex_mmi_frm_get_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,&func);
	if(func!=NULL)
	{
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)func);
		func =NULL;
	} 
	mex_mmi_frm_get_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND,&func);
	if(func!=NULL)
	{
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)func);
		func =NULL;
	}
#endif
}

/***********************号码卫士*********************************/
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
int32 MexInterface_InPhoneBook(
							   U16              num_len,
							   U8              *party_num)
{
    U8               result = 0;
	int				i = 0;
	U16				*pName = NULL;
	U16				temp_tel_num[MEX_H_PHS_PHB_NAME_LEN] = {0};
    if(NULL == party_num || 0 == num_len)
    {
        result = -1;
        return result;
    }
	for (; i < num_len; i++)
	{
		temp_tel_num[i] = *(party_num++);
	}
	if (srv_phb_check_number_exist(temp_tel_num))
    {
        result = 1;
    }
    return result;
}



int32 MexInterface_aq_PhoneWall(srv_ucm_remote_info_struct *remote_info_ptr)
{
    int result = 0;
	uint16 num_len = 0;
    if(PNULL == remote_info_ptr)
    {
        return result;
    }
	num_len = strlen((const char *)remote_info_ptr->num_uri);
    if(met_aq_PhoneWall(num_len,remote_info_ptr->num_uri))
    {
        result = 1;
    }    
    return result;
}


int32 MexInterface_aq_SmsWall(uint8* number)
{
    int result = 0;
	uint16 num_len = 0;
    if(PNULL == number)
    {
        return result;
    }
	num_len = strlen((const char *)number);
    if(met_aq_SmsWall(num_len,number))
    {
        result = 1;
    }   
    return result;
}


int32 MexInterface_aq_CheckIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len)
{
    int32 result = 0;
    if(NULL == tele_in || NULL == tele_out || 0 == tele_out_len)
    {
        return result;
    }  
	
    if(met_aq_GetIPPhone(tele_in, tele_out, tele_out_len))
    {
        result = 1;
    }
    return result;
}

int32 MexInterface_aq_getCity(char * pNumber, wchar_t *CityName)
{
    uint16  str_length = 0;
    extern void met_mex_aq_GetAttachedAreaNameAndReset(char* tempBuf);
	
    extern int32 met_mex_aq_NumInq_App_Flag;
    extern uint8 met_mex_aq_strCityNameUnicode[];
	
    met_mex_aq_GetAttachedAreaNameAndReset(pNumber);          
    if(met_mex_aq_NumInq_App_Flag==1)
    {
		
        str_length = mex_strlen((const char *)met_mex_aq_strCityNameUnicode);
		
        if(str_length> 0 )
        {
            mex_strncpy( (char *)CityName, (const char *)met_mex_aq_strCityNameUnicode, str_length );
            CityName[str_length] = 0;
        }
        return 1;
    }
    return 0;    
}

//设置号码卫士提醒功能
bool MexInterface_aq_remindUpdate()
{
	if (met_mex_aq_remind_update())
		return true;
	return false;
}


#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void MexInterface_SetExpressPlugger(AUDIO_EXPRESS_T *ptr)
{
#if ( (MEX_SPRD_CODE_VERSION >= 0x09A1109) && (MEX_SPRD_CODE_VERSION <= 0x09A1116 ) )
	ptr->bExpWorkOn = SCI_TRUE;
#endif
}
#endif
#if (defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK))
bool MexSoc_getDnsResult(void *inMsg)
{
    app_soc_get_host_by_name_ind_struct *dns_ind = (app_soc_get_host_by_name_ind_struct *)inMsg;
    return dns_ind->result;
}
uint8* MexSoc_getDnsAddr(void *inMsg)
{
    app_soc_get_host_by_name_ind_struct *dns_ind = (app_soc_get_host_by_name_ind_struct *)inMsg;
    return dns_ind->addr;
}
#endif
#if (defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK))
//1		表示 use wifi
//0		表示 can't use wifi
int MexInterface_isWifeOk()
{
#if (defined(__WIFI_SUPPORT__) && defined(__TCPIP__))
	{
		uint32 account_id = 0;
		//连接上的代码
		if(srv_dtcnt_wlan_status() == SRV_DTCNT_WLAN_STATUS_CONNECTED && CBM_OK ==  cbm_get_valid_account_id(CBM_BEARER_WIFI,&account_id))
		{
			mex_log(MEX_LOG_SOC, "cody_wifi_is_ok");
			return 1;
		}
	}
#endif
	return 0;
}
#endif

#if defined(MEX_PLATFORM_SPREAD) && !defined(SCI_TRACE_MODE) && !defined(TRACE_INFO_SUPPORT)
#undef SCI_TraceLow
uint32 SCI_TraceLow(
					const char *x_format, ...)
{
	return 1;
}
#endif

#elif defined (MEX_TARGET_CLIENT)
#endif


int32 MexInterface_invoke_post_event()//启动慢的问题
{
#if (defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK))
	return mmi_frm_invoke_post_event();
#else
    return 0;
#endif
}


