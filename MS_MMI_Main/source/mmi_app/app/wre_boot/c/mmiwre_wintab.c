/****************************************************************************
** File Name:      mmiWre_download.c                            
** Author:                                                                  
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define game module outside API .
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/02/2012     wei.zhou          Create
** 
****************************************************************************/  
 
#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmiwre_id.h"
#include "tb_dal.h"
#include "mmiwre.h"
#include "mmi_textfun.h"
#include "mmiwre_text.h"
#include "mmiwre_menutable.h"
#include "http_api.h"
#include "in_message.h"
#include "mmi_signal_ext.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#include "mmipub.h"
#include "mmiconnection_export.h"
#include "mmiwre_other.h"
#include "mmiphone_export.h"
#include "mmiwre_export.h"
#include "mmiwre_tapi.h"
#include "mmiwre_file.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_other.h"
#include "mmi_mainmenu_synchronize.h"
#include "mmiidle_export.h"
#include "wre_boot_mdu_def.h"
#include "mmiwre_anim.h"    //default wre app icon
#include "mmiidle_statusbar.h"	// changed by yifei
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "guires.h"

#ifdef WIN32
#define   ONEKEY_DOWNLOAD_TIMER_VALUE   (20*1000)
#else
#define   ONEKEY_DOWNLOAD_TIMER_VALUE   (30*1000)
#endif
#define   ONEKEY_DOWNLOAD_TIMER_VALUE2  (120*1000)
#define   ONKEY_GPRS_DIAL_TIMER_VALUE   (30*1000)
#define   ONKEY_HANDLE_MSG_TIMER_VALUE  (50)


LOCAL BOOLEAN WREHTTPInit(void);

LOCAL void WRE_GPRSConnect(void);
LOCAL void NetOnekeyDisconnect(void);
LOCAL void WREHTTPClose(void);

LOCAL BOOLEAN WRE_GetDefaultDialParam(uint32* pSimIdx,BOOLEAN * pIs_Wifi_Sel);
LOCAL void CreateAndSendGet(xSignalHeaderRec* recv_sig_ptr,uint32 context_id ,uint32 app_instance,WRE_HTTP_T * phttp_info,uint32 step);
LOCAL BOOLEAN MMIWRE_SelectNetworkType(void);
LOCAL void FreeUrlBuffer(void);
LOCAL void GetOnekeyStatus(uint32 msg_id,MMI_STRING_T * prompt_str,MMI_STRING_T * prompt_str2);
LOCAL void MMIWRE_RefreshSimParam(uint32 selsim,BOOLEAN bWifiSel);
LOCAL BOOLEAN WREOneKeyDownloadTimeout(void);
LOCAL BOOLEAN IsWREUpdateDiskEnough(void);

extern uint32 MMIWRE_GetFileSize(uint32 step);
extern uint8 WRE_GetAvailableLinkIndex(uint32 dual_sys,  DIAL_TYPE dialType);
//extern BOOLEAN WRE_GetDefaultDialParam(uint32* pSimIdx,BOOLEAN * pIs_Wifi_Sel);	// changed by yifei
extern BOOL WRE_IsChinaUnicom(uint32 dual_sys);
extern int MiniInstaller_Is_WRE_OK(void);	// changed by yifei
extern BOOLEAN WRE_InstallKernelOrStore(wchar * sourcefile,uint32 step);	// changed by yifei
extern BOOLEAN MMIPDP_PsIsActivedPdpLinkExist(MN_DUAL_SYS_E dual_sys); // changed by yifei
extern BOOLEAN MMIAPIUDISK_UdiskIsRun(void);    // changed by yifei
extern wchar *wre_wcsstr(const wchar *s,const wchar *b);    // changed by yifei
extern BOOL OneKeyDownloading;

LOCAL void OneKeyHandleMsgCallBack(uint32 msg);	
#define GPRS_FLAGS  0X65789043

#define STATUS_ONEKEY_DL_INIT             (MSG_USER_START+99)
#define STATUS_PDP_ACTIVE_OK              (MSG_USER_START+100)  //拨号成功
#define STATUS_PDP_ACTIVE_FAIL            (MSG_USER_START+101)  //拨号失败
#define STATUS_PDP_DEACTIVE_IND           (MSG_USER_START+102)  //拨号断开
#define STATUS_HTTP_DOWNLOAD_PROC_IND     (MSG_USER_START+103)  //下载进行中
#define STATUS_HTTP_DOWNLOAD_FINISH       (MSG_USER_START+104)  //下载完成
#define STATUS_HTTP_DOWNLOAD_FAIL         (MSG_USER_START+105)  //下载失败
#define STATUS_HTTP_TASK_END              (MSG_USER_START+106)  //下载结束
#define STATUS_HTTP_DOWNLOAD_CONNECTING   (MSG_USER_START+107)  //下载服务器连接中
#define STATUS_HTTP_DOWNLOAD_CONNECTED    (MSG_USER_START+108)  //服务器连接成功
#define STATUS_HTTP_WRE_INSTALLING        (MSG_USER_START+109)  //WRE安装中
#define STATUS_HTTP_WRE_INSTALLED         (MSG_USER_START+110)  //WRE安装完成

//http 命令状态
#define HTTP_STATE_INIT        0
#define HTTP_STATE_INIT_CNF    1
#define HTTP_STATE_REQUEST     2
#define HTTP_STATE_REQUEST_CNF 3
#define HTTP_STATE_CANCEL      4
#define HTTP_STATE_CLOSE       5
#define HTTP_STATE_CLOSE_CNF   6

#define  MAX_ONEKEY_TRY_TIMES   30
#define  MAX_ONEKEY_FIRST_STEP  3

typedef struct _tagWRE_BK_HTTP_T
{
    uint32         http_state;   //HTTP 命令状态
    uint8          is_task_on;   //task 是否运行
    uint32         http_task_id; //task id  
    
    uint32         old_file_size;  //old file size for range.
    uint32         reload_num;     //reload times.
    uint32         httpinit_tick;    //readed bytes.
    uint32         refresh_tick;   //readed bytes refresh time.
    uint8          reloading;      //reloading for fail.
    SCI_TIMER_PTR  timer_handle;

}WRE_BK_HTTP_T,*PWRE_BK_HTTP_T;

/*GPRS 网络拨号参数*/
typedef struct _tagGPRS_DIAL_INFO_T
{

	uint32         s_validflags;

	BOOLEAN        s_wre_is_use_wifi;

	uint32          s_wre_sim_select;

	uint8          s_link_idx;

	int32          s_valid_sim_num;

	int32          s_valid_sim_idx[MMI_DUAL_SYS_MAX];

	BOOLEAN        s_dial_ok;

    uint32        s_net_type;

	uint32         s_net_id;

	HTTP_PROXY_T   s_wre_http_proxy;
}GPRS_DIAL_INFO_T,*PGPRS_DIAL_INFO_T;

/*一建下载统计数据*/
typedef struct _tagONEKEY_DOWNLOAD_INFO_T
{
	BOOLEAN  s_download_on;

	BOOLEAN  s_normaldownload;

	uint8    s_download_timer_id;

	uint8    s_download_times;

	uint8    s_gprs_dial_timer_id;

	uint8    s_gprs_hangup_timer_id;

	uint8    s_onekey_msg_timer_id;

	uint32   s_download_status;

	uint32   s_download_step;

	uint32   s_onekey_msg;

	GPRS_DIAL_INFO_T  s_gprs_info;

	WRE_HTTP_T     s_bk_http_info;

	uint32   s_download_win_id;
	
	uint8  * s_part2_buf;

}ONEKEY_DOWNLOAD_INFO_T,*PONEKEY_DOWNLOAD_INFO_T;

uint8 g_pic_id=0;
uint8 g_timer_pic_text_id=0;
uint8 g_probox_step=0;


#define DOWNLOAD_TIMER_PERIOD_VALUE  6000

#define  DOWNLOAD_ID_MAX   4
#ifndef MAINLCD_SIZE_240X400
uint32 WRE_DOWNLOAD_TEXT_ID[DOWNLOAD_ID_MAX]={
	//TXT_WRE_DOWNLOAD_WAIT1,
	//TXT_WRE_DOWNLOAD_WAIT2,
	TXT_WRE_DOWNLOAD_WAIT3,
	TXT_WRE_DOWNLOAD_WAIT4,
	//TXT_WRE_DOWNLOAD_WAIT5,
	TXT_WRE_DOWNLOAD_WAIT6,
	//TXT_WRE_DOWNLOAD_WAIT7,
	TXT_WRE_DOWNLOAD_WAIT8,
	//TXT_WRE_DOWNLOAD_WAIT9,
	//TXT_WRE_DOWNLOAD_WAIT10
};
#else
uint32 WRE_DOWNLOAD_TEXT_ID[DOWNLOAD_ID_MAX]={
	//TXT_WRE_DOWNLOAD_WAIT1,
	//TXT_WRE_DOWNLOAD_WAIT2,
	TXT_WRE_DOWNLOAD_WAIT3,
	TXT_WRE_DOWNLOAD_WAIT4,
	TXT_WRE_DOWNLOAD_WAIT5,
	TXT_WRE_DOWNLOAD_WAIT6,
	//TXT_WRE_DOWNLOAD_WAIT7,
	//TXT_WRE_DOWNLOAD_WAIT8,
	//TXT_WRE_DOWNLOAD_WAIT9,
	//TXT_WRE_DOWNLOAD_WAIT10
};
#endif
/*one key download information*/
LOCAL ONEKEY_DOWNLOAD_INFO_T  s_onekey_download_info = {0};
//LOCAL char md5string[MD5_STRING_LEN] = {0};

static  uint32     s_old_sim_idx = (MMI_DUAL_SYS_MAX-1);
static  BOOLEAN    s_old_wifi_sel = FALSE;
static  BOOLEAN    s_old_sel_is_ok = TRUE;


uint16 *s_update_kernel_path = NULL;

/*Set Status*/
LOCAL void SetOneKeyDownloadStatus(uint32 status)
{
	s_onekey_download_info.s_download_status = status;
	SCI_TRACE_LOW("[wre]s_onekey_download_info.s_download_status %d",s_onekey_download_info.s_download_status);
}

LOCAL void NetOnekeyDisconnect(void)
{
    if(s_onekey_download_info.s_gprs_info.s_validflags)
    {
		SCI_ASSERT(s_onekey_download_info.s_gprs_info.s_validflags == GPRS_FLAGS);/*assert verified*/
		
        s_onekey_download_info.s_gprs_info.s_validflags= 0;
        MMIAPIPDP_Deactive((uint32)(&s_onekey_download_info.s_gprs_info));        
    }
}	// changed by yifei

LOCAL void DeinitOneKeyDownload(void)
{
	FreeUrlBuffer();
	
	if(s_onekey_download_info.s_gprs_dial_timer_id)
	{
		MMK_StopTimer(s_onekey_download_info.s_gprs_dial_timer_id);
		s_onekey_download_info.s_gprs_dial_timer_id = 0;
	}
	SetOneKeyDownloadStatus(0);

	NetOnekeyDisconnect();
}
/*
LOCAL BOOLEAN CheckIfExistActivePdp(uint32 selsim)
{
	uint32 ret = FALSE;

	uint32 i;
	for(i=0;i<MMI_DUAL_SYS_MAX;i++)
	{
		if(MMIPDP_PsIsActivedPdpLinkExist(i))
		{
			break;
		}
	}

	if(i != selsim && i!= MMI_DUAL_SYS_MAX)
	{
		ret = TRUE;
	}
	
	
	return ret;
}
*/
/*One key download callback*/
LOCAL void OneKeyDownLoadCallback(
                       uint8  timer_id,
                       uint32 param
                       )
{

	SCI_TRACE_LOW("[WRE] onekey (%d)",SCI_GetTickCount());
	
	if(s_onekey_download_info.s_download_times > MAX_ONEKEY_TRY_TIMES)
	{
		MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
		s_onekey_download_info.s_download_timer_id = 0;
		SCI_TRACE_LOW("[wre] times is out");
		return;
	}
#ifndef WIN32
	else if(s_onekey_download_info.s_download_times == MAX_ONEKEY_FIRST_STEP)

	{
		MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
		s_onekey_download_info.s_download_timer_id = MMK_CreateTimerCallback(ONEKEY_DOWNLOAD_TIMER_VALUE2,
												OneKeyDownLoadCallback, 0, TRUE);
		s_onekey_download_info.s_download_times++;
		
	}
#endif

	//downloading is time
	if(WREOneKeyDownloadTimeout() && TRUE == s_onekey_download_info.s_download_on)
	{
		WREHTTPClose();
		return;
	}

	//后台下载模块是否启动
	if(s_onekey_download_info.s_download_on 
		|| (s_onekey_download_info.s_normaldownload && timer_id != 0)
		|| s_onekey_download_info.s_gprs_dial_timer_id > 0 )
	{
		SCI_TRACE_LOW("[wre] download is already on!!!");
		return;
	}
	//udisk in use!!!
	if(TRUE == MMIAPIUDISK_UdiskIsRun())
	{
		SCI_TRACE_LOW("[wre] udisk is on");
		return;
	}
	//WRE runing
	if(WBOOT_IsWRERunning())
	{
		//TIMER 可以释放了。
		MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
		s_onekey_download_info.s_download_timer_id = 0;
		SCI_TRACE_LOW("[wre] wre is ok");
		return ;
	}

	//获得更新路径
    s_update_kernel_path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
    if (s_update_kernel_path == PNULL)
    {
		//无法获得更新路径，主要原因是磁盘空间不够
		SCI_TRACE_LOW("[wre] disk not enough");
		goto error;

    }
	//检查当前WRE kernel是否OK
#ifdef WIN32 
	if(0)
#else
	if(TRUE == MiniInstaller_Is_WRE_OK())
#endif

	{
		uint8 ret = 0;
		FreeUrlBuffer();
		s_onekey_download_info.s_part2_buf = SCI_ALLOC(2*1024);

#ifdef WIN32
#else
		WRE_Set_SYS_Dir(WRE_SYS_PATH_KERNEL,WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE));

		ret = MiniIntaller_Is_Store_OK((char *)s_onekey_download_info.s_part2_buf);	// changed by yifei
#endif
		if(0 == ret)
		{
			FreeUrlBuffer();		
			//TIMER 可以释放了。
			MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
			s_onekey_download_info.s_download_timer_id = 0;
			
			/*一键下载功能关闭*/
			WRE_SetOnekeyDownLoadState(FALSE);
			
			goto error;

		}
		else if(2==ret)
		{
			s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_PART2;
		}
		else
		{
			goto error;
		}
	}
	else
	{
		s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_PART1;
	}

	if(FALSE == IsWREUpdateDiskEnough())
	{
		goto error;
	}

	//check if other pdp is active.
	//if(TRUE == CheckIfExistActivePdp(s_onekey_download_info.s_gprs_info.s_wre_sim_select))
	//{
	//	goto error;
	//}

	//get a valid dial param.
	//background runing,need to get default sim index and link index
	if(timer_id == 0)
	{
		s_onekey_download_info.s_gprs_info.s_wre_sim_select = s_old_sim_idx;
		s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi= s_old_wifi_sel;
	}
	else
	{
		if(FALSE == WRE_GetDefaultDialParam(&s_onekey_download_info.s_gprs_info.s_wre_sim_select,
			&s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi))
		{
			SCI_TRACE_LOW("[WRE] find no valid sim");
			goto error;
		}
	}

	

	//启动后台拨号程序
	WRE_GPRSConnect();

	return;
error:

	if(s_onekey_download_info.s_download_win_id)
	{			
		MMK_PostMsg(s_onekey_download_info.s_download_win_id,STATUS_HTTP_TASK_END,0,0);
	}
	return;

}

/*This function is a timer callback*/
LOCAL void HandleOneMsg(
                       uint8  timer_id,
                       uint32 param
                       )
{
	if(param == STATUS_HTTP_TASK_END)
	{
		DeinitOneKeyDownload();
		MMK_StopTimer(timer_id);
		s_onekey_download_info.s_gprs_hangup_timer_id = 0;
	}
	else
	{
		if(timer_id == s_onekey_download_info.s_onekey_msg_timer_id)
		{
			switch(s_onekey_download_info.s_download_status)
			{
			case STATUS_PDP_ACTIVE_OK:
				//启动后台线程进行下载
				break;
				
			case STATUS_PDP_ACTIVE_FAIL:
			case STATUS_PDP_DEACTIVE_IND:
				//关闭后台线程，退出下载
				WREHTTPClose();
				break;
			default:
				break;	// changed by yifei
			}
			
			
			MMK_StopTimer(timer_id);
			s_onekey_download_info.s_onekey_msg_timer_id = 0;
		}
		else
		{
			MMK_StopTimer(timer_id);
		}
	}
}

#if 1
/*消息处理*/
LOCAL void OneKeyHandleMsgCallBack(uint32 msg)
{
	if(s_onekey_download_info.s_onekey_msg_timer_id)
		MMK_StopTimer(s_onekey_download_info.s_onekey_msg_timer_id);

	if(msg == STATUS_HTTP_TASK_END)
	{
		if(s_onekey_download_info.s_gprs_hangup_timer_id)
		{
			MMK_StopTimer(s_onekey_download_info.s_gprs_hangup_timer_id);
		}
		s_onekey_download_info.s_gprs_hangup_timer_id = MMK_CreateTimerCallback(ONKEY_HANDLE_MSG_TIMER_VALUE,
												HandleOneMsg, msg, FALSE);   
	}
	else
	{
		s_onekey_download_info.s_onekey_msg_timer_id = MMK_CreateTimerCallback(ONKEY_HANDLE_MSG_TIMER_VALUE,
												HandleOneMsg, 0, FALSE);
	}
}
#endif
LOCAL void StopDialTimer(void)
{
	
	if(s_onekey_download_info.s_gprs_dial_timer_id)
	{
		//GPRS 拨号超时
		MMK_StopTimer(s_onekey_download_info.s_gprs_dial_timer_id);
		
	}
	s_onekey_download_info.s_download_status = 0;
	s_onekey_download_info.s_gprs_dial_timer_id = 0;
}
/*gprs 拨号超时处理*/
LOCAL void OneKeyGprsDialCallback(
                       uint8  timer_id,
                       uint32 param)
{
	//停止拨号timer.
	StopDialTimer();

	//关闭拨号连接
	NetOnekeyDisconnect();
}


/*gprs download */
LOCAL void WRE_Notify_PostMsg(uint32  msg,uint32 wParam,uint32 lParam)
{
	SetOneKeyDownloadStatus(msg);

	if(STATUS_PDP_ACTIVE_OK == msg)
	{
		//停止拨号timer
		StopDialTimer();

		//进行http初始化
		WREHTTPInit();		
		
	}
	else if(STATUS_PDP_ACTIVE_FAIL == msg || STATUS_PDP_DEACTIVE_IND == msg)
	{
		StopDialTimer();
	}
	
	

	//一些消息需要进行通过mmi回调函数处理
	if(msg == STATUS_PDP_ACTIVE_OK 
		|| msg == STATUS_PDP_ACTIVE_FAIL
		|| msg == STATUS_PDP_DEACTIVE_IND
		|| msg == STATUS_HTTP_TASK_END)
	{
		OneKeyHandleMsgCallBack(msg);
	}
	
	if(s_onekey_download_info.s_download_win_id)
	{
		
		MMK_PostMsg(s_onekey_download_info.s_download_win_id,msg,(DPARAM)wParam,lParam);	// changed by yifei
	}

}



/*gprs 拨号消息处理函数*/
LOCAL void HandleOneKeyPdpPsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    GPRS_DIAL_INFO_T  *ptr = (GPRS_DIAL_INFO_T *)msg_ptr->app_handler;
    SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/

    if(ptr->s_validflags != GPRS_FLAGS)
    {
        return;
    }

    switch(msg_ptr->msg_id)
    {

    case MMIPDP_ACTIVE_CNF:
		
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            //通知进行HTTP数据发送
            ptr->s_net_id = msg_ptr->nsapi;
			MMIWRE_RefreshSimParam(s_onekey_download_info.s_gprs_info.s_wre_sim_select,
									s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi);

            WRE_Notify_PostMsg(STATUS_PDP_ACTIVE_OK,0,0);            
			s_old_sel_is_ok = TRUE;
        }
        else
        {
            //拨号失败
            WRE_Notify_PostMsg(STATUS_PDP_ACTIVE_FAIL,0,0);
			s_old_sel_is_ok = FALSE;
        }
        break;

    case MMIPDP_DEACTIVE_CNF:
    case MMIPDP_DEACTIVE_IND:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            //拨号断开
            WRE_Notify_PostMsg(STATUS_PDP_DEACTIVE_IND,0,0);
        }
        break;
    case MMIPDP_ACTIVE_TIMOUT_IND:
        {
            //拨号超时失败
            WRE_Notify_PostMsg(STATUS_PDP_ACTIVE_FAIL,0,0);

        }
        break;
    default:
        break;
    }
}

//拨号
LOCAL BOOLEAN WREOneKeyNetConnect(void*         dialParam,
							uint32        simNo,
                            uint32        isWifi,
                            char        * apn_ptr,
                            char        * user_ptr,
                            char        * psw_ptr)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};

    //记录当前拨号方式
    app_info.app_handler = (uint32)dialParam;

    app_info.apn_ptr = (char*)apn_ptr;
    app_info.user_name_ptr = user_ptr;
    app_info.psw_ptr = psw_ptr;
    app_info.dual_sys = simNo;
    app_info.priority = 3;
    app_info.handle_msg_callback = HandleOneKeyPdpPsMsg;

    if(isWifi)
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    else
        app_info.ps_interface = MMIPDP_INTERFACE_GPRS;

    return MMIAPIPDP_Active(&app_info);

}

BOOLEAN WRE_OneKey_Try2ConnectNet(DIAL_TYPE dialType)
{
    MMICONNECTION_LINKSETTING_DETAIL_T   * link_ptr;

    uint32 linkidx;
	
	//wap if the pri for china unicom.
	if(TRUE == WRE_IsChinaUnicom(s_onekey_download_info.s_gprs_info.s_wre_sim_select))
	{
		dialType = DIAL_WAP;
	}
	
	linkidx = WRE_GetAvailableLinkIndex(s_onekey_download_info.s_gprs_info.s_wre_sim_select, 
												dialType);

    memset(&s_onekey_download_info.s_gprs_info.s_wre_http_proxy, 0, sizeof(HTTP_PROXY_T));

    //MMIPDP_GetLinkInfo(linkidx,link_ptr);
    link_ptr = MMICONNECTION_GetLinkSettingItemByIndex(s_onekey_download_info.s_gprs_info.s_wre_sim_select,
						linkidx);
	if(NULL == link_ptr)
		return FALSE;
    
	
    if (0 == strcmp((const char *)(link_ptr->gateway), GATEWAY_HTTP_PROXY_NONE))
    {
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.is_use_proxy = FALSE;
		s_onekey_download_info.s_gprs_info.s_net_type = DIAL_WAP;

    }
    else
    {
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.is_use_proxy = TRUE;
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.http_port = link_ptr->port;
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.password_ptr = (char *)(link_ptr->password);
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.proxy_host_ptr = (char *)(link_ptr->gateway);
        s_onekey_download_info.s_gprs_info.s_wre_http_proxy.user_name_ptr = (char *)(link_ptr->username);
		s_onekey_download_info.s_gprs_info.s_net_type = DIAL_WAP;

    }
    SCI_TRACE_LOW("[WRE]s_wre_http_proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s", \
		s_onekey_download_info.s_gprs_info.s_wre_http_proxy.is_use_proxy, 
		s_onekey_download_info.s_gprs_info.s_wre_http_proxy.http_port, 
		s_onekey_download_info.s_gprs_info.s_wre_http_proxy.proxy_host_ptr);

    return WREOneKeyNetConnect(&s_onekey_download_info.s_gprs_info,
						s_onekey_download_info.s_gprs_info.s_wre_sim_select,
						s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi,
						(char *)(link_ptr->apn),PNULL,PNULL);
}

/*尽量在timer中实现，如果没有找到一个比较好的地方*/
LOCAL void WRE_GPRSConnect(void)
{
	BOOLEAN ret;
	//GPRS 拨号
	s_onekey_download_info.s_gprs_info.s_validflags = GPRS_FLAGS;

	if(s_onekey_download_info.s_gprs_dial_timer_id)
		MMK_StopTimer(s_onekey_download_info.s_gprs_dial_timer_id);
	
	if(s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi)
	{
		ret = WREOneKeyNetConnect(&s_onekey_download_info.s_gprs_info,
							s_onekey_download_info.s_gprs_info.s_wre_sim_select,
							s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi,
							NULL,0,0);
	}
	else
	{
		s_onekey_download_info.s_gprs_dial_timer_id = MMK_CreateTimerCallback(ONKEY_GPRS_DIAL_TIMER_VALUE,
												OneKeyGprsDialCallback, 0, FALSE);

		ret = WRE_OneKey_Try2ConnectNet(DIAL_NET);
	}
	if(!ret)
	{
		s_onekey_download_info.s_gprs_info.s_validflags = 0;
		WRE_Notify_PostMsg(STATUS_PDP_ACTIVE_FAIL,0,0);

		MMK_StopTimer(s_onekey_download_info.s_gprs_dial_timer_id);
		s_onekey_download_info.s_gprs_dial_timer_id = 0;
	}
}

PUBLIC void MMIWRE_EnableOneKeyDownload(BOOLEAN  state)
{
	SCI_TRACE_LOW("MMIWRE_EnableOneKeyDownload %d",state);
	if(state)
		s_onekey_download_info.s_normaldownload = FALSE;
	else
		s_onekey_download_info.s_normaldownload = TRUE;
}



LOCAL void MMIWRE_RefreshSimParam(uint32 selsim,BOOLEAN bWifiSel)
{
	s_old_sim_idx   = selsim;
	s_old_wifi_sel  = bWifiSel;
	s_old_sel_is_ok = TRUE;
}

PUBLIC void MMIWRE_InitDownLoad(BOOLEAN bFirstInit)
{
	BOOLEAN   bStartup = TRUE;
	//to do : 增加WRE版本完整性检查

	#ifdef WRE_WREKERNEL_NORFLASH
		return;
	#endif
	
	if(bFirstInit)  /*lint !e527*/
	{
#ifndef WIN32
		bStartup = WRE_GetOnekeyDownLoadState();
#endif
		s_old_sel_is_ok = FALSE;
	}
	else
	{

		WRE_SetOnekeyDownLoadState(TRUE);
	}
	
	if(bStartup)
	{
		uint32  period_value;
		SCI_TRACE_LOW("MMIWRE_InitDownLoad %d",bStartup);
		
		if(bFirstInit)
		{
			period_value = ONEKEY_DOWNLOAD_TIMER_VALUE2;
		}
		else
		{
			period_value = ONEKEY_DOWNLOAD_TIMER_VALUE;
		}
		if(s_onekey_download_info.s_download_timer_id)
		{
			MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
		}
		s_onekey_download_info.s_download_timer_id = MMK_CreateTimerCallback(ONEKEY_DOWNLOAD_TIMER_VALUE,
			OneKeyDownLoadCallback, 0, TRUE);
		s_onekey_download_info.s_download_times = 0;
		
	}

}

//==============================================HTTP 处理部分===========================================
#define WRE_DOWNLOAD_TASK_STACK_SIZE      (1024*8)
#define WRE_DOWNLOAD_TASK_QUEUE_NUM       100
#define WRE_DOWNLOAD_TASK_PRIORITY        SCI_SYSTEM_PRI(30)

#ifdef WIN32
#define WRE_HTTP_URI_STRING                 "http://172.16.131.54/wre202.zip"
#define WRE_ACCEPT_CHARSET_STRING           "UTF-8,gb2312,gbk,ISO-8859-1,USA-ASCII,ISO-10646-UCS-2;q=0.600"
#ifdef WRE_STORE_OVERSEAS
#define WRE_REFERER_STRING                  "http://hk.51wre.com/wre/download?rc=wre202.zip:80"
#define WRE_UA_PROFILE_STRING               "http://hk.51wre.com/wre/download?rc=wre202.zip:80"
#else
#define WRE_REFERER_STRING                  "http://51wre.com/wre/download?rc=wre202.zip:80"
#define WRE_UA_PROFILE_STRING               "http://51wre.com/wre/download?rc=wre202.zip:80"
#endif
#else
#define WRE_HTTP_TEST_URI_STRING            "http://musicplayer.3g.qq.com/mu388"
#ifdef WRE_STORE_OVERSEAS
#define WRE_HTTP_URI_STRING                 "http://hk.51wre.com:80/wre/download?rc=wrepart1.wpk"
#else
#define WRE_HTTP_URI_STRING                 "http://51wre.com:80/wre/download?rc=wrepart1.wpk"
#endif
#define WRE_ACCEPT_CHARSET_STRING           "UTF-8,gb2312,gbk,ISO-8859-1,USA-ASCII,ISO-10646-UCS-2;q=0.600"
#endif

#define MAX_RELOAD_TIME        3 //最多三次下载机会

#define HTTP_RELOAD_WREKERNEL  0xD101 //重新下载处理

LOCAL void RefreshDloadIn(uint32 tick)
{
    s_onekey_download_info.s_bk_http_info.refresh_tick = tick;//readed bytes refresh time.
}
LOCAL BOOLEAN WREOneKeyDownloadTimeout(void)
{
	BOOLEAN ret = FALSE;
	uint32 tick = SCI_GetTickCount();

	if( s_onekey_download_info.s_download_on  == TRUE
		&& (tick - s_onekey_download_info.s_bk_http_info.refresh_tick)>ONEKEY_DOWNLOAD_TIMER_VALUE2)
	{	
		ret = TRUE;
	}
	return ret;
}

LOCAL void WREOneKeyDownLoadTask(uint32 argc, void * argv)
{
    wchar  http_content_file[100];
    BOOLEAN runtest = TRUE;
    HTTP_INIT_PARAM_T* init_param_ptr;
    xSignalHeaderRec* recv_sig_ptr;
    uint32 request_id;
    HTTP_HEADER_IND_SIG_T* header_ptr;
    HTTP_DATA_IND_SIG_T* data_ptr;
    HTTP_NET_PROG_IND_SIG_T* prog_ptr;
    uint32 len;
    uint32 context_id;
    uint32 app_instance;
    uint32 bSuc;

    uint32 msg;
    uint32 total_num;
    uint32 download_num;
	uint32 filesize;

    BOOLEAN   bErrExist = FALSE;
    //INIT:
    init_param_ptr = SCI_ALLOC(sizeof(HTTP_INIT_PARAM_T));
    memset(init_param_ptr,0,sizeof(HTTP_INIT_PARAM_T));

    //根据当前拨号方式,来决定采用何种方式


#if  0
    //  if(1)//模拟器仅仅支持非代理方式
#else
    if(s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi)
#endif
    {

    }
    else
    {
        init_param_ptr->proxy.is_use_proxy = s_onekey_download_info.s_gprs_info.s_wre_http_proxy.is_use_proxy;
        if(init_param_ptr->proxy.is_use_proxy)
        {
            init_param_ptr->proxy.http_port = s_onekey_download_info.s_gprs_info.s_wre_http_proxy.http_port;
            len = strlen(s_onekey_download_info.s_gprs_info.s_wre_http_proxy.proxy_host_ptr);
            init_param_ptr->proxy.proxy_host_ptr = SCI_ALLOC(len + 1);
            memset(init_param_ptr->proxy.proxy_host_ptr,0,len + 1);
            memcpy(init_param_ptr->proxy.proxy_host_ptr,s_onekey_download_info.s_gprs_info.s_wre_http_proxy.proxy_host_ptr,len);
        }
    }
    //SCI_TRACE_LOW:"[WRE]init_param_ptr->proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2049_112_2_18_3_11_22_490,(uint8*)"dds", init_param_ptr->proxy.is_use_proxy, init_param_ptr->proxy.http_port, init_param_ptr->proxy.proxy_host_ptr);
    init_param_ptr->is_cookie_enable = FALSE;
    //memset(init_param_ptr->cookie_file_name,0,sizeof(init_param_ptr->cookie_file_name));
    init_param_ptr->is_cache_enable = FALSE;
    init_param_ptr->net_id = s_onekey_download_info.s_gprs_info.s_net_id;
    
	s_onekey_download_info.s_download_on = TRUE;
	RefreshDloadIn(SCI_GetTickCount());

onekey_down_again:

	//reset all various.
    runtest = TRUE;
    request_id = 0;
    header_ptr = PNULL;
    data_ptr = PNULL;
    prog_ptr = PNULL;
    len = 0;
    context_id = 0;
    app_instance = 0x65742367;
    bSuc = FALSE;
    msg = 0;
    total_num     = 0;
    download_num  = 0;
    bErrExist = FALSE;

    HTTP_InitRequest((HTTP_APP_MODULE_ID_T)init_param_ptr,1,init_param_ptr);
    
    s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_INIT;

	//memset(md5string, 0, MD5_STRING_LEN);

    while (runtest)
    {
           // Get signal.
        recv_sig_ptr = (xSignalHeaderRec*)SCI_GetSignal(SCI_IdentifyThread());
#ifdef WIN32
		Sleep(400);
#endif
        switch (recv_sig_ptr->SignalCode)
        {
        case HTTP_SIG_INIT_CNF:
            context_id = ((HTTP_INIT_CNF_SIG_T* )recv_sig_ptr)->context_id;
            app_instance = 0;
			SCI_ASSERT(s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART1 ||s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART2);/*assert verified*/
	
			CreateAndSendGet(recv_sig_ptr,context_id,app_instance,
					&s_onekey_download_info.s_bk_http_info,
					s_onekey_download_info.s_download_step);
			

            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST;

            break;
        case HTTP_SIG_NEED_AUTH_IND:
            //CreateAndSendAuthRsp(recv_sig_ptr,request_id);
            //SCI_TRACE_LOW("\n http test auth request id = %d\n",request_id);
            //HTTP_CancelRequest(((HTTP_REQUEST_ID_IND_SIG_T*)recv_sig_ptr)->context_id,((HTTP_REQUEST_ID_IND_SIG_T*)recv_sig_ptr)->app_instance,request_id);
            break;

        case HTTP_SIG_REQUEST_ID_IND:
            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST;

            request_id = ((HTTP_REQUEST_ID_IND_SIG_T*)recv_sig_ptr)->request_id;
            //SCI_TRACE_LOW:"[WRE] http test request id = %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2087_112_2_18_3_11_23_491,(uint8*)"d",request_id);
            break;

        case HTTP_SIG_GET_CNF:
            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST_CNF;
            //SCI_TRACE_LOW:"[WRE] http test recv HTTP_SIG_GET_CNF\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2092_112_2_18_3_11_23_492,(uint8*)"");
            HTTP_CloseRequest(((HTTP_GET_CNF_SIG_T*)recv_sig_ptr)->context_id);
                   
			WRE_Notify_PostMsg(STATUS_HTTP_DOWNLOAD_FINISH,0,0);
		
            //bSuc = TRUE;
            break;

        case HTTP_SIG_HEAD_CNF:
            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST_CNF;

            //SCI_TRACE_LOW:"[WRE] http test recv HTTP_SIG_HEAD_CNF\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2108_112_2_18_3_11_23_493,(uint8*)"");
            HTTP_CloseRequest(((HTTP_HEAD_CNF_SIG_T*)recv_sig_ptr)->context_id);
            break;

        case HTTP_SIG_DATA_IND:
            data_ptr = (HTTP_DATA_IND_SIG_T*)recv_sig_ptr;
            //SCI_TRACE_LOW:"[WRE] http test data ind rsp code=%d,request_id =%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2114_112_2_18_3_11_23_494,(uint8*)"dd",data_ptr->response_code,data_ptr->request_id);
            break;

        case HTTP_SIG_HEADER_IND:
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2118_112_2_18_3_11_23_495,(uint8*)"");
            header_ptr = (HTTP_HEADER_IND_SIG_T*)recv_sig_ptr;

			SCI_TRACE_LOW("[WRE] http test recv HTTP_SIG_HEADER_IND headercode (%d)\n",header_ptr->rsp_header_info.response_code);

            if((header_ptr->rsp_header_info.response_code == 200) || (header_ptr->rsp_header_info.response_code == 206))
            {
#ifndef WIN32
                /* get the md5 string...  ??? */
                char    *p = PNULL;
                char    newmd5string[MD5_STRING_LEN+1] = {0};
				char    oldmd5string[MD5_STRING_LEN+1] = {0};

                p = strstr(header_ptr->header_ptr, http_hdr_wre_md5);
				SCI_TRACE_LOW("[WRE]:hdr_wre_md5 %s",p?p:"");
                if(!p)
                {
                    HTTP_CloseRequest(header_ptr->context_id);
                    break;
                }
                //p += 5;
				memcpy(newmd5string, p+5, MD5_STRING_LEN);

				if(FALSE == WRE_GetMd5(s_onekey_download_info.s_download_step,oldmd5string))
				{
                    HTTP_CloseRequest(header_ptr->context_id);
                    break;
				}

                SCI_TRACE_LOW("old md5 str is %s",oldmd5string);
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2134_112_2_18_3_11_23_496,(uint8*)"s", newmd5string);
                if(oldmd5string[0] == 0)
                {
					WRE_SaveMd5(s_onekey_download_info.s_download_step,newmd5string);

                    //memcpy(md5string, newmd5string, MD5_STRING_LEN);
                }
                else
                {
                    if(memcmp(oldmd5string, newmd5string,MD5_STRING_LEN) != 0)
                    {
						WRE_SaveMd5(s_onekey_download_info.s_download_step,"");//clear old md5 string

						MMIWRE_DeleteInitFile(s_onekey_download_info.s_download_step);    
                        /* compare md5 error, close reconnect...  */
                        HTTP_CloseRequest(header_ptr->context_id);
                        break;
                    }
                }
#endif

                //MMIAPICOM_StrToWstr(HTTP_CONTENT_FILE_NAME,http_content_file);
                MMIAPICOM_Wstrcpy(http_content_file, s_update_kernel_path);
                MMIAPICOM_Wstrcat(http_content_file, L"\\");
				if(s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART1)
				{
					MMIAPICOM_Wstrcat(http_content_file, WRE_HTTP_CONTENT_FILE_NAME);
				}
				else
				{
					MMIAPICOM_Wstrcat(http_content_file, WRE_PART2_HTTP_CONTENT_FILE_NAME);
				}

                HTTP_HeaderResponse(header_ptr->context_id,header_ptr->app_instance,header_ptr->request_id,HTTP_DATA_TRANS_STYLE_BUFFER,(uint16*)http_content_file,MMIAPICOM_Wstrlen(http_content_file));
            }
            else if(header_ptr->rsp_header_info.response_code == 404)
            {
                //SCI_TRACE_LOW:"no find file, or parameter error.."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2158_112_2_18_3_11_23_497,(uint8*)"");
				HTTP_CloseRequest(header_ptr->context_id);
				runtest = FALSE;

            }
            else if(header_ptr->rsp_header_info.response_code == 510)
            {

				WRE_SaveMd5(s_onekey_download_info.s_download_step,"");//clear old md5 string
				MMIWRE_DeleteInitFile(s_onekey_download_info.s_download_step);    

                //SCI_TRACE_LOW:"error code, context_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2162_112_2_18_3_11_23_498,(uint8*)"d", header_ptr->context_id);
                /* renew download...  */
                HTTP_CloseRequest(header_ptr->context_id);
            }
            else
            {
                
            }
            break;

        case HTTP_SIG_ERROR_IND:
            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST_CNF;

            //SCI_TRACE_LOW:"[WRE] http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2187_112_2_18_3_11_23_499,(uint8*)"d",((HTTP_ERROR_IND_SIG_T*)recv_sig_ptr)->result);
            HTTP_CloseRequest(((HTTP_ERROR_IND_SIG_T*)recv_sig_ptr)->context_id);


            bErrExist  = TRUE;
            bSuc       = FALSE;
            s_onekey_download_info.s_bk_http_info.reload_num++;
            if(s_onekey_download_info.s_bk_http_info.reload_num >= MAX_RELOAD_TIME)
            {
                s_onekey_download_info.s_bk_http_info.reloading = FALSE;
            }
           
            download_num = s_onekey_download_info.s_bk_http_info.old_file_size;

			WRE_Notify_PostMsg(STATUS_HTTP_DOWNLOAD_FAIL,(uint32)&(((HTTP_ERROR_IND_SIG_T*)recv_sig_ptr)->result), 4);	// changed by yifei
            break;

        case HTTP_SIG_CLOSE_CNF:
            if(s_onekey_download_info.s_bk_http_info.reloading)
            {
                HTTP_InitRequest((HTTP_APP_MODULE_ID_T)init_param_ptr,1,init_param_ptr);
                s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_INIT;
            }
            else
            {
                //SCI_TRACE_LOW:"[WRE] http test recv HTTP_SIG_CLOSE_CNF\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2215_112_2_18_3_11_23_500,(uint8*)"");
                if(FALSE == bSuc && !bErrExist && total_num)
                {
					/*
					uint32  filesize;
					if(ONEKEY_DOWNLOAD_STEP_PART1 == s_onekey_download_info.s_download_step)
					{
						filesize = MMIWRE_GetFileSize(s_onekey_download_info.s_download_step);
					}
					else
					{

					}

                    if( filesize == total_num)
                    {
                        bSuc = TRUE;
                    }*/
					
                }
                runtest = FALSE;

                SCI_Sleep(100);  //等待http将文件关闭
            }
            // goto INIT;
            break;
        case HTTP_SIG_CANCEL_CNF:
            HTTP_CloseRequest(((HTTP_CANCEL_CNF_SIG_T*)recv_sig_ptr)->context_id);
            break;


        case HTTP_SIG_NET_PROG_IND:
            prog_ptr = (HTTP_NET_PROG_IND_SIG_T*)recv_sig_ptr;

            {

                switch(prog_ptr->net_prog_op_code)
                {
                case NET_PROGRESS_CONNECTING:
					s_old_sel_is_ok = FALSE;
                    msg = STATUS_HTTP_DOWNLOAD_CONNECTING;

                    break;

                case NET_PROGRESS_CONNECTED:
					s_old_sel_is_ok = TRUE;
                    msg = STATUS_HTTP_DOWNLOAD_CONNECTED;
                    break;

                case NET_PROGRESS_RECEIVING:
                    if(bErrExist == FALSE)
                    {

                        if( s_onekey_download_info.s_bk_http_info.reloading == TRUE)
                            s_onekey_download_info.s_bk_http_info.reloading = FALSE;

                        //MMIWRE_StartupDownloadTimeout(NULL);

                        if(0 == total_num)
                        {
                            total_num     = prog_ptr->content_len;

                            //it is a exception, download will be fail
                            if( total_num == 0 )
                            {
                                //find no match file,download fail
                                uint32  retcode = HTTP_ERROR_NET_DATA_ERROR;

								WRE_Notify_PostMsg(STATUS_HTTP_DOWNLOAD_FAIL,(uint32)(&retcode), 4);
							
                                break;
                            }
                        }
                        else
                        {
                            if(total_num != prog_ptr->content_len)
                            {
                                //SCI_TRACE_LOW:"[WRE] WRE download total(%d) does match num %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2281_112_2_18_3_11_23_501,(uint8*)"dd",total_num,prog_ptr->content_len);
                            }
                        }
                        download_num += prog_ptr->current_len;

                        RefreshDloadIn(SCI_GetTickCount());

                        if(download_num == total_num)
                        {
                            bSuc = TRUE;
                            //g_wre_progress_step = STEP_WRE_DOWNLOADED;
							//s_onekey_download_info.s_download_status = 
                        }

                        s_onekey_download_info.s_bk_http_info.proc_ind.totalnum = total_num;
            
                        s_onekey_download_info.s_bk_http_info.proc_ind.recvnum  = s_onekey_download_info.s_bk_http_info.old_file_size+download_num;

						
                        //SCI_TRACE_LOW:"[WRE] %d %d %d %d"
                        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2298_112_2_18_3_11_23_502,(uint8*)"dddd",prog_ptr->current_len,prog_ptr->content_len,download_num,total_num);
                        msg = STATUS_HTTP_DOWNLOAD_PROC_IND;

						WRE_Notify_PostMsg(msg,(uint32)&s_onekey_download_info.s_bk_http_info.proc_ind,sizeof(WRE_HTTP_PRO_IND_T));	// changed by yifei
                    }
                    msg = 0;
                    break;

                    
                case NET_PROGRESS_SENDING:

                case NET_PROGRESS_OTHER:
                default:
                    msg = 0;
                    break;
                }
                if(msg != 0 && s_onekey_download_info.s_bk_http_info.reloading == FALSE)
                {
					WRE_Notify_PostMsg(msg, 0, 0);

                    if(STATUS_HTTP_DOWNLOAD_CONNECTED == msg)
                        SCI_Sleep(500);
                    
                }
            }
            //SCI_TRACE_LOW:"[WRE] http test net prog ind,%d,%d,%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2330_112_2_18_3_11_23_503,(uint8*)"ddd",prog_ptr->net_prog_op_code,prog_ptr->content_len,prog_ptr->current_len);
            break;
        case HTTP_SIG_REDIRECT_IND:
            //SCI_TRACE_LOW:"[WRE] http test redirect ind\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2333_112_2_18_3_11_23_504,(uint8*)"");
            break;
        case HTTP_SIG_POST_CNF:
            s_onekey_download_info.s_bk_http_info.http_state = HTTP_STATE_REQUEST_CNF;
            //SCI_TRACE_LOW:"[WRE] http test recv HTTP_SIG_POST_CNF\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2337_112_2_18_3_11_23_505,(uint8*)"");
            HTTP_CloseRequest(((HTTP_POST_CNF_SIG_T*)recv_sig_ptr)->context_id);
            break;

        case HTTP_SIGNAL_END://cancel
            if(s_onekey_download_info.s_bk_http_info.http_state >= HTTP_STATE_REQUEST && s_onekey_download_info.s_bk_http_info.http_state <= HTTP_STATE_REQUEST_CNF)
            {
                HTTP_CancelRequest(context_id,app_instance,request_id);
            }
            else
            {
                HTTP_CloseRequest(context_id);
            }
            s_onekey_download_info.s_bk_http_info.reloading = FALSE;
            break;
        case HTTP_RELOAD_WREKERNEL:
            {
                //SCI_TRACE_LOW:"[WRE] BEGIN RELOAD!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2354_112_2_18_3_11_23_506,(uint8*)"");

                //MMIWRE_StopTimerCheck();
            
                s_onekey_download_info.s_bk_http_info.reload_num++;
                if(s_onekey_download_info.s_bk_http_info.reload_num >= MAX_RELOAD_TIME)
                {
                    s_onekey_download_info.s_bk_http_info.reloading = FALSE;
                    bErrExist = TRUE;
                }
                
                if(s_onekey_download_info.s_bk_http_info.http_state >= HTTP_STATE_REQUEST && s_onekey_download_info.s_bk_http_info.http_state <= HTTP_STATE_REQUEST_CNF)
                {
                    HTTP_CloseRequest(context_id);
                }
                
        

            }
            break;

        default:
            break;
        }
        // Free signal.
        SCI_FREE(recv_sig_ptr);
    }

	//判断是否下载成功
	if(!bErrExist && !bSuc && total_num)
	{
		filesize = MMIWRE_GetFileSize(s_onekey_download_info.s_download_step);
		if(filesize == (total_num + s_onekey_download_info.s_bk_http_info.old_file_size))
		{
			bSuc = TRUE;
		}
	}

    {

		SCI_TRACE_LOW("[WRE]: download part%d,bSuc = %d",(s_onekey_download_info.s_download_step+1),bSuc);
        if( bSuc )
        {
            BOOLEAN bInstallKernel = FALSE;
            int iIndex = WRE_GetAvailableLinkIndex(s_onekey_download_info.s_gprs_info.s_wre_sim_select, s_onekey_download_info.s_gprs_info.s_net_type);


			WRE_Notify_PostMsg(STATUS_HTTP_WRE_INSTALLING,0,0);
            
            //MMIAPICOM_StrToWstr(HTTP_CONTENT_FILE_NAME,http_content_file);
            MMIAPICOM_Wstrcpy(http_content_file, s_update_kernel_path);
            MMIAPICOM_Wstrcat(http_content_file, L"\\");
			if(ONEKEY_DOWNLOAD_STEP_PART1 == s_onekey_download_info.s_download_step)
			{
				MMIAPICOM_Wstrcat(http_content_file, WRE_HTTP_CONTENT_FILE_NAME);
			}
			else
			{
				MMIAPICOM_Wstrcat(http_content_file, WRE_PART2_HTTP_CONTENT_FILE_NAME);
			}

#ifdef WIN32
			bInstallKernel = TRUE;
#else


			bInstallKernel = WRE_InstallKernelOrStore(http_content_file,s_onekey_download_info.s_download_step );
#endif

            if (!bInstallKernel)
            {
                wchar syspath[100] = {0};

		uint32  error = HTTP_ERROR_FILE_WRITE;

				WRE_Notify_PostMsg(STATUS_HTTP_DOWNLOAD_FAIL,(uint32)(&error),0);
                SCI_TRACE_LOW("[WRE] WRE_InstallKernel return FALSE");                
                MMIAPICOM_Wstrcpy(syspath, s_update_kernel_path);
                MMIAPICOM_Wstrcat(syspath, L"\\");

				if(ONEKEY_DOWNLOAD_STEP_PART1 == s_onekey_download_info.s_download_step)
				{
					MMIAPICOM_Wstrcat(syspath,WRE_SYS_FILE_NAME_KERNELL_SZIP);
				}
				else
				{
					MMIAPICOM_Wstrcat(syspath,WRE_SYS_FILE_NAME_STORE_SZIP);
				}

                
                SFS_DeleteFile(syspath, NULL);
                //SFS_DeleteFile(syspath, NULL);

            }
			else
			{
				 MMIWRE_DeleteInitFile(s_onekey_download_info.s_download_step);    

				if(ONEKEY_DOWNLOAD_STEP_PART1 == s_onekey_download_info.s_download_step)
				{
					uint8 ret = 0;
					//SCI_TRACE_LOW:"[WRE] WRE_InstallKernel return TRUE"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2452_112_2_18_3_11_23_509,(uint8*)"");
					
					WRE_Set_SYS_Dir(WRE_SYS_PATH_STORE, s_update_kernel_path);
					
					
					//check appstore is ok
					FreeUrlBuffer();
					s_onekey_download_info.s_part2_buf = SCI_ALLOC(2*1024);
#ifdef WIN32
					ret = 2;
#else
					ret = MiniIntaller_Is_Store_OK((char *)s_onekey_download_info.s_part2_buf);	// changed by yifei
#endif
					if(0 == ret)
					{	
						FreeUrlBuffer();
						
						s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_FINISH;
						
						//delete wrong wpk 						
					}
					else if(2 == ret)
					{
						s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_PART2;
						MMIWRE_DeleteInitFile(s_onekey_download_info.s_download_step);    
						goto onekey_down_again;
					}
					else //can not get the url
					{
						s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_FINISH;
					}					
				}
				else
				{
					s_onekey_download_info.s_download_step = ONEKEY_DOWNLOAD_STEP_FINISH;
					#ifdef WRE_AUTO_STORE_SREEN_FIX
					{
						uint16 *p = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL);
						SCI_TRACE_LOW("[wre]download finish, check is T card,path = %s",p);
						if(p != NULL && p[0] == 'e') /*lint !e774*/
						{//wre目录在T卡	
							wre_load_store_ver_dat(p);	
						}
					}
					#endif
				}
			}
            

			WRE_Notify_PostMsg(STATUS_HTTP_WRE_INSTALLED,0,0);

        }
		else
		{
			s_old_sel_is_ok = FALSE;
		}

    }

	FreeUrlBuffer();
	if(init_param_ptr->proxy.proxy_host_ptr)
    {
        SCI_FREE(init_param_ptr->proxy.proxy_host_ptr);
    }
    
    SCI_FREE(init_param_ptr);
	
	s_update_kernel_path = NULL; 
	
    HideWreDirectroy(TRUE);


	WRE_Notify_PostMsg(STATUS_HTTP_TASK_END,0,0);
	s_onekey_download_info.s_bk_http_info.http_state = 0;
    s_onekey_download_info.s_bk_http_info.http_task_id = 0;

	RefreshDloadIn(0);

	s_onekey_download_info.s_download_status = 0;
	s_onekey_download_info.s_download_on = FALSE;
	OneKeyDownloading = FALSE;
	s_onekey_download_info.s_download_times++;

	SCI_ThreadExit();
}

LOCAL BOOLEAN WREHTTPInit(void)
{

	if(s_onekey_download_info.s_bk_http_info.http_task_id == 0)
	{
		s_onekey_download_info.s_bk_http_info.http_task_id = SCI_CreateAppThread("BK_TASK_NAME",
        "BK_TASK_NAME",
        WREOneKeyDownLoadTask,
        (uint32)(&s_onekey_download_info),
        NULL,
        WRE_DOWNLOAD_TASK_STACK_SIZE,
        WRE_DOWNLOAD_TASK_QUEUE_NUM,
        WRE_DOWNLOAD_TASK_PRIORITY,
        SCI_PREEMPT,
        SCI_AUTO_START );	// changed by yifei
	}

    return TRUE;
}

LOCAL void WREHTTPClose(void)
{
	if(0 == s_onekey_download_info.s_bk_http_info.http_task_id)
	{
		return;
	}

    if(s_onekey_download_info.s_bk_http_info.http_task_id)
    {
        MmiSignalS *sendSignal = PNULL;

        MmiCreateSignal(HTTP_SIGNAL_END, sizeof(MmiSignalS), (MmiSignalS*)&sendSignal);

        sendSignal->Sender = P_APP;

        SCI_SendSignal( (void*)sendSignal,s_onekey_download_info.s_bk_http_info.http_task_id);
    }
}

#define WRE_HTTP_PROXY_HOST                 "10.0.0.172"
#define WRE_ACCEPT_LANGUAGE_STRING          "en,zh-cn"
#define WRE_USER_AGENT_STRING               "MOCOR/1.0 WRE/1.0"
/*
#ifdef WIN32
#define WRE_HTTP_URI_STRING                 "http://172.16.131.54/wre202.zip"
#define WRE_ACCEPT_CHARSET_STRING           "UTF-8,gb2312,gbk,ISO-8859-1,USA-ASCII,ISO-10646-UCS-2;q=0.600"
#define WRE_REFERER_STRING                  "http://51wre.com/wre/download?rc=wre202.zip:80"
#define WRE_UA_PROFILE_STRING               "http://51wre.com/wre/download?rc=wre202.zip:80"
#else
#define WRE_HTTP_TEST_URI_STRING            "http://musicplayer.3g.qq.com/mu388"
#define WRE_HTTP_URI_STRING                 "http://51wre.com:80/wre/download?rc=wrepart1.wpk"
#define WRE_ACCEPT_CHARSET_STRING           "UTF-8,gb2312,gbk,ISO-8859-1,USA-ASCII,ISO-10646-UCS-2;q=0.600"
#endif
*/


#define HTTP_HER_EXTERN_BEGIN   0
#define HTTP_HER_BEGIN          0

char *http_hdr_wre[] =
{
     http_hdr_wre_imsi,
     http_hdr_wre_imei, 
     http_hdr_wre_cellid,
     //http_hdr_wre_wreversion, 
     http_hdr_wre_bootversion,
     http_hdr_wre_chipversion,
     http_hdr_wre_idhid, 
     http_hdr_wre_scrensize,
     //http_hdr_wre_deviceid
     http_hdr_wre_range,
     http_hdr_wre_md5
};

#if 1
//==================================  ui manager ==================================
void MMIPUB_RefreshLabel(MMI_WIN_ID_T    alert_win_id,MMI_STRING_T *promt_str,MMI_STRING_T * promt_alert_str)
{

    MMIPUB_SetAlertWinTextByPtr(alert_win_id, promt_str, promt_alert_str, TRUE);
    //MMK_PostMsg(alert_win_id, MSG_FULL_PAINT, PNULL,0);
}

LOCAL MMI_RESULT_E HandleWREDownLoadWaitingWin(
                                                  MMI_WIN_ID_T       win_id,
                                                  MMI_MESSAGE_ID_E   msg_id,
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T alert_str = {0};
    MMI_WIN_ID_T    progress_win_id = MMISET_WRE_KERNEL_CHECK_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		MMIDEFAULT_AllowOpenKeylocWin(FALSE);
		s_onekey_download_info.s_download_win_id = win_id;
        /* init md5 strings... */
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		/* get current status */
		g_timer_pic_text_id = MMK_CreateWinTimer(win_id, DOWNLOAD_TIMER_PERIOD_VALUE, TRUE);
		result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
		MMIWRE_EnableOneKeyDownload(FALSE);
		OneKeyDownLoadCallback(0,0);
		
		MMIWRE_InitDownLoad(FALSE);//startup one key 
		MMI_GetLabelTextByLang(WRE_DOWNLOAD_TEXT_ID[g_pic_id], &prompt_str);
		MMIPUB_RefreshLabel(win_id,&prompt_str,&alert_str);		
		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
       	break;

/*
	case MSG_FULL_PAINT:
		MMIPUB_SetWinSoftkey(win_id, STXT_OK, STXT_CANCEL, FALSE);
		return MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);		
*/
    case MSG_TIMER:
    			if(g_timer_pic_text_id == *((uint8 *) param))
			{
				if(g_pic_id<DOWNLOAD_ID_MAX-1)
				{
					++g_pic_id;
				}
				else
				{
					g_pic_id=0;
				}
				MMI_GetLabelTextByLang(WRE_DOWNLOAD_TEXT_ID[g_pic_id], &prompt_str);
				MMIPUB_RefreshLabel(win_id,&prompt_str,&alert_str);		
				MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
			}
    			break;
    case STATUS_HTTP_TASK_END:   
		MMK_CloseWin(win_id);
		/*if(ONEKEY_DOWNLOAD_STEP_FINISH == s_onekey_download_info.s_download_step)
		{
			//wre_init_global();
			WRE_EntryApp();
		}
        else
        {*/
            MMK_CloseApplet(SPRD_WRE_APPLET_ID);
      //  }
		break;


        //show the process of downloading.
    case STATUS_HTTP_DOWNLOAD_PROC_IND:
      {
  	WRE_HTTP_PRO_IND_T  * proc_ind = &s_onekey_download_info.s_bk_http_info.proc_ind;
	  uint32                total = proc_ind->totalnum/1024;
	  uint32                recv  = proc_ind->recvnum/1024;
	  
	  if(total == 0)
		  return result;
	  if(s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART1)
	  total = (recv*100)/total/2;
	  else
	  total = (recv*100)/total/2 + 50;
	  if(total  >  100)
	  {
		  total = 100;
	  }
	  g_probox_step = total;

		MMIPUB_UpdateProgressBarEx(&progress_win_id, g_probox_step, TRUE);
      		 result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);

        }

        break;

        //connecting the server
    case STATUS_HTTP_DOWNLOAD_CONNECTING:
        //the server is connected
    case STATUS_HTTP_DOWNLOAD_CONNECTED:
        //download ok
    case STATUS_HTTP_DOWNLOAD_FINISH:
    case STATUS_HTTP_WRE_INSTALLING:
    case STATUS_HTTP_WRE_INSTALLED:
    	 g_probox_step = 0;
	result = MMIPUB_HandleProgressWinMsg( win_id,msg_id,param);//MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case STATUS_HTTP_DOWNLOAD_FAIL:

        switch(*((DWORD*)param))
        {
        case HTTP_ERROR_FAILED:
        case HTTP_ERROR_INVALID_INPUT:
        case HTTP_ERROR_NO_MEMORY:
        case HTTP_ERROR_FILE_OPEN:
        case HTTP_ERROR_FILE_WRITE:
        case HTTP_ERROR_FILE_NOT_FOUND:
        case HTTP_ERROR_FILE_DELETE:
        case HTTP_ERROR_FILE_GET_SIZE:
        case HTTP_ERROR_FILE_READ:
        case HTTP_ERROR_FILE_NO_SPACE:
            MMI_GetLabelTextByLang(TXT_WRE_SYS_ERROR, &alert_str);
            break;


        case HTTP_ERROR_NET_DATA_ERROR:
        case HTTP_ERROR_PROXY_CONNECT:
        case HTTP_ERROR_TIMEOUT:
        case HTTP_ERROR_SERVER_CLOSE:
        case HTTP_ERROR_UNKNOWN:
        case HTTP_ERROR_HANDSHAKE:
        case HTTP_ERROR_PROXY_HANDSHAKE:
        case HTTP_ERROR_RECV:
        case HTTP_ERROR_SEND:
            MMI_GetLabelTextByLang(TXT_WRE_NETWORK_ERROR, &alert_str);
            break;

        case HTTP_ERROR_CONNECT:
            MMI_GetLabelTextByLang(TXT_WRE_SERVER_FAIL, &alert_str);
            break;

        default:
            MMI_GetLabelTextByLang(TXT_WRE_SYS_ERROR, &alert_str);
            break;

        }
       // g_probox_step = 0;
       s_onekey_download_info.s_bk_http_info.proc_ind.totalnum = 0;
        MMIPUB_RefreshLabel(win_id,&prompt_str,&alert_str);
	result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);//MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    		MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
        break;

        case STATUS_PDP_DEACTIVE_IND:
        case STATUS_PDP_ACTIVE_FAIL:
            MMIPUB_RefreshLabel(win_id,&prompt_str,&alert_str);
			SCI_Sleep(1000);
			MMK_CloseWin(win_id);	
            MMK_CloseApplet(SPRD_WRE_APPLET_ID);
            break;

            //拨号成功
        case STATUS_PDP_ACTIVE_OK:
        //    MMIPUB_RefreshLabel(win_id,&prompt_str,&alert_str);
 	 result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);//MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
           break;

		case MSG_KEYDOWN_OK:
			MMK_CloseWin(win_id);
			WREHTTPClose();
			SCI_Sleep(100);
			//WRE_SetOnekeyDownLoadState(FALSE);
			MMK_StopTimer(s_onekey_download_info.s_download_timer_id);
			s_onekey_download_info.s_download_timer_id = 0;
			break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            MMK_CloseApplet(SPRD_WRE_APPLET_ID);
            break;
	case MSG_KEYUP_POWER: // zuobin modify
	     MMK_CloseWin(win_id);
	     MMK_CloseApplet(SPRD_WRE_APPLET_ID);
            return MMI_RESULT_FALSE;
            
        case MSG_CLOSE_WINDOW:
			MMIDEFAULT_AllowOpenKeylocWin(TRUE);

			s_onekey_download_info.s_download_win_id = 0;

            MMIDEFAULT_AllowTurnOffBackLight(TRUE);

			MMIWRE_EnableOneKeyDownload(TRUE);           
            break;
            
	case MSG_LOSE_FOCUS:
		MMK_CloseWin(win_id);
		MMK_CloseApplet(SPRD_WRE_APPLET_ID);
	     break;
        default:
            result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);//MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return result;
}

LOCAL void GetOnekeyProcString(MMI_STRING_T * prompt_str2)
{
  if( s_onekey_download_info.s_download_status == STATUS_HTTP_DOWNLOAD_PROC_IND)
  {
	  WRE_HTTP_PRO_IND_T  * proc_ind = &s_onekey_download_info.s_bk_http_info.proc_ind;
	  uint32                total = proc_ind->totalnum/1024;
	  uint32                recv  = proc_ind->recvnum/1024;
	  char                  temp[10];
	  wchar                 wstr_temp[20];
	  wchar                 *temp_ptr;
	  int                   len;
	  
	  if(total == 0)
		  return;
	  
	  total = (recv*100)/total;
	  
	  if(total  > 100)
	  {
		  total = 100;
	  }
	  
	  sprintf(temp," %d",total);
	  
	  len = strlen(temp);
	  temp[len] = '%';
	  temp[len+1] = 0;
	  len ++;
	  
	  MMIAPICOM_StrToWstr((const uint8 *)temp,wstr_temp);
	  
	  temp_ptr = SCI_ALLOC(100);
	  temp_ptr[0]  = 0;
	  
	  
	  
	  MMI_GetLabelTextByLang(TXT_WRE_INSTALLING, prompt_str2);
	  MMIAPICOM_Wstrncat(temp_ptr,prompt_str2->wstr_ptr,prompt_str2->wstr_len);
	  MMIAPICOM_Wstrncat(temp_ptr,wstr_temp,len);
	  
	  //change the wstr pointer
	  prompt_str2->wstr_ptr = temp_ptr;
	  prompt_str2->wstr_len = MMIAPICOM_Wstrlen(temp_ptr);   
	}

}
LOCAL void GetOnekeyStatus(uint32 msg, MMI_STRING_T * prompt_str,MMI_STRING_T * prompt_str2)
{	
	if(s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART1)
		MMI_GetLabelTextByLang(TXT_WRE_ENTERTAINMENT_PART1, prompt_str);
	else if(s_onekey_download_info.s_download_step == ONEKEY_DOWNLOAD_STEP_PART2)
		MMI_GetLabelTextByLang(TXT_WRE_ENTERTAINMENT_PART2, prompt_str);
	else
	{
		MMI_GetLabelTextByLang(TXT_WRE_ENTERTAINMENT, prompt_str);
	}
	
	switch(msg)
	{
	case STATUS_HTTP_DOWNLOAD_PROC_IND:
		GetOnekeyProcString(prompt_str2);
		break;
        //connecting the server
    case STATUS_HTTP_DOWNLOAD_CONNECTING:
		MMI_GetLabelTextByLang(TXT_WRE_DOWNLOAD_CONNECTING, prompt_str2);
        break;
		
        //the server is connected
    case STATUS_HTTP_DOWNLOAD_CONNECTED:
        MMI_GetLabelTextByLang(TXT_WRE_DOWNLOAD_CONNECTED, prompt_str2);        
        break;
		
        //download ok
    case STATUS_HTTP_DOWNLOAD_FINISH:
        MMI_GetLabelTextByLang(TXT_WRE_DOWNLOAD_DONE, prompt_str2);
        break;
		
    case STATUS_HTTP_WRE_INSTALLING:
        MMI_GetLabelTextByLang(TXT_WRE_INSTALL, prompt_str2);
        break;
		
    case STATUS_HTTP_WRE_INSTALLED:
        MMI_GetLabelTextByLang(TXT_WRE_INSTALLED, prompt_str2);
        break;		
		
    case STATUS_PDP_DEACTIVE_IND:
    case STATUS_PDP_ACTIVE_FAIL:
        MMI_GetLabelTextByLang(TXT_WRE_NETWORK_CON_FAIL, prompt_str2);
        break;
		
        //拨号成功
	case STATUS_PDP_ACTIVE_OK:
		MMI_GetLabelTextByLang(TXT_WRE_DOWNLOAD, prompt_str2);
		break;
	default:
		MMI_GetLabelTextByLang(TXT_WRE_NETWORK_CONNECTING, prompt_str2);
		break;
	}
}

PUBLIC void ShowWRELoading(void)
{
		MMI_STRING_T prompt_str = {0};
	    	MMI_WIN_ID_T    progress_win_id = MMISET_WRE_KERNEL_CHECK_WIN_ID;
   
		SCI_TRACE_LOW("******enter ShowWRELoading");
		MMI_GetLabelTextByLang(TXT_WRE_ENTERTAINMENT, &prompt_str);
	       MMIPUB_OpenProgressWinByTextId(TXT_NULL, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleWREDownLoadWaitingWin);
		MMIPUB_SetWinTitleText(progress_win_id,&prompt_str,FALSE);
              MMIPUB_SetProgressTotalSize(&progress_win_id, 100);
}
#endif
    

//judge One key downloading status
PUBLIC BOOLEAN  WRE_IsBkDownloadOn(void)
{
	if(s_onekey_download_info.s_download_on || s_onekey_download_info.s_gprs_dial_timer_id > 0)
	{
#if 0
		uint32  num = 5;
		MMIPUB_OpenAlertWaitingWinEx(SPRD_WRE_APPLET_ID, TXT_WRE_INSTALLING);
		return TRUE;
#else
		//show downloading interface
		ShowWRELoading();
		return TRUE;
		//disconnect network untill the onekeydown thread exits.
#endif
	}
	return FALSE;
}

PUBLIC uint8 MiniIntaller_Is_Store_OK(char * urlbuf)
{
	WRE_APP_PARAM_T  param = {0};

	char * url = NULL;
	uint8 ret = 0;
	#ifdef WIN32
            return 0;
	#endif

	if(urlbuf)
		url = urlbuf;
	else
		url = SCI_ALLOC(2*1024);

	param.argc = (int)0xeaebece2;
	param.cmdline = (uint8 *)url;
	url[0] = 0;
	SCI_TRACE_LOW("enter MMIWRE_Get_Part2_ulr");
	wre_startup(&param);
	SCI_TRACE_LOW("[WRE] ret %d ",url[0]);
	ret = url[0];

	if(NULL == urlbuf)
		SCI_FREE(url);

	if(ret == 0)//ok
		return 0;
	else if(ret == 1)
		return 1; //can not check 
	else if(ret == '\h')    /*lint !e606*/
		return 2;//need
	else
		return 1;
}

PUBLIC char * WRE_GetPart2Url(void)
{

	return (s_onekey_download_info.s_part2_buf?(char *)s_onekey_download_info.s_part2_buf:"");	// changed by yifei
}

LOCAL void FreeUrlBuffer(void)
{
	if(s_onekey_download_info.s_part2_buf){
		SCI_FREE(s_onekey_download_info.s_part2_buf);
		s_onekey_download_info.s_part2_buf = NULL;
	}
}

PUBLIC BOOLEAN WRE_SetOnekeyDownLoadState(BOOLEAN bStartup)
{
	SFS_HANDLE   wre_md5_file;
	uint16       filename[200] = {0};
	uint16       *path = NULL;
	WRE_MD5_LIST md5List = {0};
	SFS_ERROR_E  err;
	uint32       nReaded;

	memset(&md5List,0,sizeof(WRE_MD5_LIST));

	path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
	MMIAPICOM_Wstrcpy(filename, path);
	MMIAPICOM_Wstrcat(filename, WRE_UPDATE_MD5_FILE);

	wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
	if(wre_md5_file == PNULL)
	{
		wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, 0, 0);/*lint !e655 */		
		if(wre_md5_file == PNULL)
			return FALSE;
	}
	else
	{
		err = SFS_ReadFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
		if(err != SFS_NO_ERROR)
		{
			SFS_CloseFile(wre_md5_file);
			return FALSE;
		}
	}

	if(bStartup)
		md5List.onekeyflag = WRE_ONE_KEY_FLAG;
	else
		md5List.onekeyflag = 0;

	SFS_SetFilePointer(wre_md5_file, (__int64)0, SFS_SEEK_BEGIN);
	
	SFS_WriteFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
	
	SFS_CloseFile(wre_md5_file);

	return TRUE;
}

PUBLIC BOOLEAN WRE_GetOnekeyDownLoadState(void)
{
	SFS_HANDLE   wre_md5_file;
	uint16       filename[200] = {0};
	uint16       *path = NULL;
	WRE_MD5_LIST md5List = {0};
	SFS_ERROR_E  err;
	uint32       nReaded;

	memset(&md5List,0,sizeof(WRE_MD5_LIST));

	path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
	MMIAPICOM_Wstrcpy(filename, path);
	MMIAPICOM_Wstrcat(filename, WRE_UPDATE_MD5_FILE);

	SCI_TRACE_LOW("WRE_GetOnekeyDownLoadState");
	wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
	if(wre_md5_file == PNULL)
	{
		wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, 0, 0);/*lint !e655 */		
		if(wre_md5_file == PNULL)
			return FALSE;

		SFS_SetFilePointer(wre_md5_file, (__int64)0, SFS_SEEK_BEGIN);
		
		SFS_WriteFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
		
	}
	else
	{
		err = SFS_ReadFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
	}
	
	
	SFS_CloseFile(wre_md5_file);

	SCI_TRACE_LOW("[WRE] onekey state %d",(md5List.onekeyflag == WRE_ONE_KEY_FLAG));

	return (md5List.onekeyflag==WRE_ONE_KEY_FLAG)?TRUE:FALSE;
}

PUBLIC BOOLEAN WRE_GetMd5(uint32 step,char *md5string)
{
	SFS_HANDLE   wre_md5_file;
	uint16       filename[200] = {0};
	uint16       *path = NULL;
	WRE_MD5_LIST md5List;
	SFS_ERROR_E  err;
	uint32       nReaded;

	memset(&md5List,0,sizeof(WRE_MD5_LIST));

	path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
	MMIAPICOM_Wstrcpy(filename, path);
	MMIAPICOM_Wstrcat(filename, WRE_UPDATE_MD5_FILE);

	wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
	if(wre_md5_file == PNULL)
	{
		md5string[0] = 0;
		return TRUE;
	}
	else
	{
		err = SFS_ReadFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
		SCI_TRACE_LOW("[wre]:onekey flags= %d,%s",md5List.onekeyflag,md5List.part1md5);
		SCI_TRACE_LOW("[wre]:onekey flags= %d,%s",md5List.onekeyflag,md5List.part2md5);
		if(err != SFS_NO_ERROR)
		{
			SFS_CloseFile(wre_md5_file);
			return FALSE;
		}

		if(step == 0)
		{
			memcpy(md5string,md5List.part1md5,MD5_STRING_LEN);
		}
		else 
		{
			memcpy(md5string,md5List.part2md5,MD5_STRING_LEN);
		}
		SFS_CloseFile(wre_md5_file);

	}

	return TRUE;

}
PUBLIC BOOLEAN WRE_SaveMd5(uint32 step,char * md5string)
{
	SFS_HANDLE   wre_md5_file;
	uint16       filename[200] = {0};
	uint16       *path = NULL;
	WRE_MD5_LIST md5List = {0};
	SFS_ERROR_E  err;
	uint32       nReaded;

	memset(&md5List,0,sizeof(WRE_MD5_LIST));

	path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
	MMIAPICOM_Wstrcpy(filename, path);
	MMIAPICOM_Wstrcat(filename, WRE_UPDATE_MD5_FILE);

	wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
	if(wre_md5_file == PNULL)
	{
		wre_md5_file = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, 0, 0);/*lint !e655 */
		if(wre_md5_file == PNULL)
			return FALSE;
	}
	else
	{
		err = SFS_ReadFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);
		SCI_TRACE_LOW("[wre]:onekey flags= %d",md5List.onekeyflag);

		if(err != SFS_NO_ERROR)
		{
			SFS_CloseFile(wre_md5_file);
			return FALSE;
		}
	}

	if(step == 0)
	{
		memcpy(md5List.part1md5,md5string,MD5_STRING_LEN);  /*lint !e420 !e831*/
	}
	else if(step == 1)
	{
		memcpy(md5List.part2md5,md5string,MD5_STRING_LEN);  /*lint !e420 !e831*/
	}

	SFS_SetFilePointer(wre_md5_file, (__int64)0, SFS_SEEK_BEGIN);

	SFS_WriteFile(wre_md5_file,&md5List,sizeof(WRE_MD5_LIST),&nReaded,NULL);

	SFS_CloseFile(wre_md5_file);

	return TRUE;
	
}

PUBLIC void WRE_DeleteOnekeydownloadFile(void)
{
	SFS_HANDLE   wre_md5_file;
	uint16       filename[200] = {0};
	uint16       *path = NULL;
	
	path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
	if(path){
		MMIAPICOM_Wstrcpy(filename, path);
		MMIAPICOM_Wstrcat(filename, WRE_UPDATE_MD5_FILE);
		
		SFS_DeleteFile(filename,NULL);
	}
}


/*get the default dial param for onekey*/
LOCAL BOOLEAN WRE_GetDefaultDialParam(uint32* pSimIdx,BOOLEAN * pIs_Wifi_Sel)
{
	
	uint32  i,j;
	BOOLEAN is_need_sel;//记录是否需要进行网络选择
	BOOLEAN bValidate;
	BOOLEAN simList[MMI_DUAL_SYS_MAX] = {0};



#ifdef MMI_WIFI_SUPPORT
	//if wifi exists ,dial by wifi.
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
	{
		*pIs_Wifi_Sel = TRUE;
		*pSimIdx = 0;	// changed by yifei
		return TRUE;
	}
#endif
	
	//find the first sim with valid network
	for(i=0;i<MMI_DUAL_SYS_MAX;i++)
	{
		is_need_sel = MMIAPIPHONE_GetSimExistedStatus(i);
		bValidate = MMIAPIPHONE_IsSimOk(i);
		
		if(is_need_sel&&bValidate)
		{
			simList[i] = TRUE;
		}
		else
		{
			simList[i] = FALSE;
		}
		
		//SCI_TRACE_LOW("simList[%d] = %d",i,simList[i]);
	}
	SCI_TRACE_LOW("[WRE] s_old_sel_is_ok %d(%d)",s_old_sel_is_ok,s_old_sim_idx);
	if(FALSE == s_old_sel_is_ok)
	{
		//寻找一个合适的sim卡
		if(s_old_sim_idx == (MMI_DUAL_SYS_MAX-1))
		{
			j = 0;
		}
		else
		{
			j = s_old_sim_idx+1;
		}
		for(;j!=s_old_sim_idx;)
		{
			if(simList[j])
			{
				//got
				break;
			}
			else
			{
				j++;
				if(j == MMI_DUAL_SYS_MAX)
					j=0;
				continue;
			}
		}

		if(j >= MMI_DUAL_SYS_MAX)   /*lint !e831*/
		{
			s_old_sel_is_ok = FALSE;
			//SCI_TRACE_LOW("[wre] get no valid sim 2");
			return FALSE;
		}
		else	// changed by yifei /*lint !e661 !e831*/
		{
			if(simList[j])  /*lint !e661 !e831*/
			{
				*pSimIdx      = j;
				*pIs_Wifi_Sel = FALSE;
				s_old_wifi_sel = *pIs_Wifi_Sel;
				s_old_sim_idx  = j;
				return TRUE;
			}
			else
			{
				s_old_sel_is_ok = FALSE;
				//SCI_TRACE_LOW("[wre] get no valid sim 3");
				
				return FALSE;
			}
		}
	}
	else
	{
		bValidate = MMIAPIPHONE_IsSimOk(s_old_sim_idx);
		if(bValidate)
		{
			*pSimIdx      = s_old_sim_idx;
			*pIs_Wifi_Sel = s_old_wifi_sel;
			return TRUE;
		}
		else
		{
			s_old_sel_is_ok = FALSE;
		}
	}
	//SCI_TRACE_LOW("[wre] get no valid sim 3");

	return FALSE;
}
static BOOL IsDevEnoughSpace(MMIFILE_DEVICE_E dev)
{
    uint32 free_space_low = 0;
    uint32 free_space_high = 0;

    MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(dev),
                               MMIAPIFMM_GetDevicePathLen(dev),
        &free_space_high,
        &free_space_low);

    if (0 == free_space_high && WRE_GetKernelMinSpace(WRE_SYS_PATH_KERNEL) > free_space_low)
    {
        //SCI_TRACE_LOW:"[WRE download].IsDevEnoughSpace: free_spaced_high = %d, free_space_low = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_1412_112_2_18_3_11_21_482,(uint8*)"dd", free_space_high, free_space_low);
        return FALSE;
    }

    return TRUE;

}

MMIFILE_DEVICE_E WreDeviceMapFileDevice(WRE_FILE_DEVICE_E wreDevice)
{
    return (MMIFILE_DEVICE_E)wreDevice;
}


BOOLEAN ShowLoading(BOOLEAN bStartup)
{
	OneKeyDownloading = TRUE;
	if(FALSE == IsWREUpdateDiskEnough())
    {
        MMIPUB_OpenAlertFailWin(TXT_WRE_NOFREESPACE);         
        return FALSE;
    }
    
    return MMIWRE_SelectNetworkType();
}


PUBLIC void MMIWRE_DeleteInitFile(uint32 step)
{
    uint16       filepath[156];
    
    MMIAPICOM_Wstrcpy(filepath, (const wchar*)WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    MMIAPICOM_Wstrcat(filepath, L"\\");
	if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
	{
		MMIAPICOM_Wstrcat(filepath, WRE_HTTP_CONTENT_FILE_NAME);
	}
	else
	{
		MMIAPICOM_Wstrcat(filepath, WRE_PART2_HTTP_CONTENT_FILE_NAME);
	}

    SFS_DeleteFile(filepath,NULL);
}

void SetHeader(HTTP_OTHER_HEADER_T *pHttpOtherHeader, char *pName, char *pValue)
{
    uint32 len = 0;
    if (!pHttpOtherHeader || !pName || !pValue)
    {
        return ;
    }

    len = strlen(pName);
    pHttpOtherHeader->header_name_ptr = SCI_ALLOC(len+1);
    memset(pHttpOtherHeader->header_name_ptr,0,len + 1);
    strcpy(pHttpOtherHeader->header_name_ptr,pName);
    len = strlen(pValue);
    pHttpOtherHeader->header_value_ptr = SCI_ALLOC(len+1);
    memset(pHttpOtherHeader->header_value_ptr,0,len + 1);
    strcpy(pHttpOtherHeader->header_value_ptr,pValue);
}

void FreeHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader)
{
     uint32 i = 0;
    if (pHttpExternHeader == NULL)
        return ;

    for (; i < sizeof(http_hdr_wre)/sizeof(char *); i++)
    {
        if (pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_name_ptr != NULL)
        {
            SCI_FREE(pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_name_ptr);
            pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_name_ptr = NULL;
        }
        if (pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_value_ptr != NULL)
        {
                  SCI_FREE(pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_value_ptr);
                  pHttpExternHeader->other_header_ptr[HTTP_HER_EXTERN_BEGIN+i].header_value_ptr = NULL;
        }
    }
}

void SetWreParamToHttpHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader,WRE_HTTP_T * phttp_info,uint32 step)
{
     VERSION_T ver={0}; 
     int iWide = 0;
     int iHeight = 0;
     uint32 filesize = 0;
#define BUFF_SIZE (64)   
     char buff[BUFF_SIZE]={0};
    //OSVERSIONINFOW versionInfo = {0};
    //SYSTEM_INFO Systeminfo = {0};
    WRE_PHONE_PLMN_T  * plmninfo = NULL;
    int iBeginEx = HTTP_HER_EXTERN_BEGIN;
    int iBegin = HTTP_HER_BEGIN;
	char md5string[MD5_STRING_LEN+1] = {0};

	if(FALSE == WRE_GetMd5(step,md5string))
	{
		return;
	}

    if (pHttpExternHeader == NULL)
        return ;

	if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
	{
		// IMEI
		memset(buff, 0, BUFF_SIZE);
		Adapt_GetImsi(0,buff);
		SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    // IMSI
    memset(buff, 0, BUFF_SIZE);
    Adapt_GetImei(0,buff);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    //net info
    memset(buff, 0, BUFF_SIZE);
    plmninfo = Adapt_GetNetWorkInfo(0);
    sprintf(buff,"%d",plmninfo->cell_id);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    //version info
    //GetVersionEx(&versionInfo);
    //memset(buff,0,BUFF_SIZE);
    //sprintf(buff,"%d.%d.%d",versionInfo.dwMajorVersion, versionInfo.dwMinorVersion, versionInfo.dwBuildNumber );
    //SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    //boot version
    WRE_GetBOOTVersion(&ver);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], ver.version);

    //System Info
    memset(buff,0,BUFF_SIZE);
    sprintf(buff,"%d",WRE_GetCHIPVersion());
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    //SytemParametersInfo
    memset(&ver,0,sizeof(ver));
    WRE_GetPRJName(&ver);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], ver.version);

    //ScreenSize
    memset(buff, 0, BUFF_SIZE);
    WRE_GetScreenSize(&iWide,&iHeight);        
    sprintf(buff,"%d*%d",iWide, iHeight);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);
	}
	else
	{
		iBegin += 7;
	}
    memset(buff, 0, BUFF_SIZE);

	SCI_TRACE_LOW("[WRE] md5string %s",md5string);
	if(md5string[0] == 0)
	{
		//download again
		MMIWRE_DeleteInitFile(step);               

	}

    filesize = MMIWRE_GetFileSize(step);           
    sprintf(buff,"bytes=%d-",filesize);
    phttp_info->old_file_size = filesize;

    //SCI_TRACE_LOW:"[WRE] %s %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_1746_112_2_18_3_11_22_484,(uint8*)"ss",http_hdr_wre[iBegin],buff);
    SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);

    //Md5
    memset(buff, 0, BUFF_SIZE);
    if(md5string[0] != 0)
    {
        memcpy(buff, md5string, MD5_STRING_LEN);
    }
    //SCI_TRACE_LOW:"[WRE1] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_1755_112_2_18_3_11_22_485,(uint8*)"s",http_hdr_wre[iBegin]);
    if(buff[0] != 0)
    {
        //SCI_TRACE_LOW:"md5 is %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_1758_112_2_18_3_11_22_486,(uint8*)"s", buff);
        SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);
    }
    else
    {
        SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], NULL);
    }

    //ComputerName
    //memset(buff, 0, BUFF_SIZE);
    //SetHeader(&(pHttpExternHeader->other_header_ptr[iBeginEx++]), http_hdr_wre[iBegin++], buff);
}

LOCAL void CreateAndSendGet(xSignalHeaderRec* recv_sig_ptr,uint32 context_id ,uint32 app_instance,WRE_HTTP_T * phttp_info,uint32 step)
{
    HTTP_GET_PARAM_T* get_param_ptr;
    uint32 len = 0;
    HTTP_INIT_CNF_SIG_T* sig_ptr = (HTTP_INIT_CNF_SIG_T*)recv_sig_ptr;
    
    if((get_param_ptr = SCI_ALLOC(sizeof (HTTP_GET_PARAM_T))) ==NULL)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_1779_112_2_18_3_11_22_487,(uint8*)"");
        return;
    }

    memset(get_param_ptr,0x00,sizeof(HTTP_GET_PARAM_T));
    get_param_ptr->connection = HTTP_CONNECTION_DEFAULT;
    get_param_ptr->need_net_prog_ind = TRUE;
    len = strlen(WRE_ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept.accept_ptr,0,len+1);
    strcpy(get_param_ptr->accept.accept_ptr,WRE_ACCEPT_STRING);
    len = strlen(WRE_ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->accept_language.accept_language_ptr,0,len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,WRE_ACCEPT_LANGUAGE_STRING);
    len = strlen(WRE_USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->user_agent.user_agent_ptr,0,len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,WRE_USER_AGENT_STRING);
#ifdef WIN32
	if(1)
#else 
	if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
#endif		
	{
		len = strlen(WRE_HTTP_URI_STRING);
		get_param_ptr->uri.uri_ptr = SCI_ALLOC(len+1+32);
		memset(get_param_ptr->uri.uri_ptr,0,len+1+32);
		strcpy(get_param_ptr->uri.uri_ptr,WRE_HTTP_URI_STRING);
	}
	else
	{
		char * p = WRE_GetPart2Url();
		len = strlen(p);
		
		get_param_ptr->uri.uri_ptr = SCI_ALLOC(len+1+32);
		memset(get_param_ptr->uri.uri_ptr,0,len+1+32);
		strcpy(get_param_ptr->uri.uri_ptr,p);
	}

    len = strlen(WRE_ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC(len + 1);
    memset(get_param_ptr->accept_charset.accept_charset_ptr,0,len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,WRE_ACCEPT_CHARSET_STRING);

    /*
    len = strlen(UA_PROFILE_STRING);
    get_param_ptr->ua_profile.ua_profile_ptr = SCI_ALLOC(len + 1);
    memset(get_param_ptr->ua_profile.ua_profile_ptr,0,len + 1);
    strcpy(get_param_ptr->ua_profile.ua_profile_ptr,UA_PROFILE_STRING);
    */
    /*
    len = strlen(REFERER_STRING);
    get_param_ptr->referer.referer_ptr = SCI_ALLOC(len + 1);
    memset(get_param_ptr->referer.referer_ptr,0,len + 1);
    strcpy(get_param_ptr->referer.referer_ptr,REFERER_STRING);
    */
    get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;

    get_param_ptr->extern_header.header_num = sizeof(http_hdr_wre)/sizeof(char *);
    get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*(get_param_ptr->extern_header.header_num));
    memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*(get_param_ptr->extern_header.header_num));
#if 0
    len = strlen("accept");
    get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len + 1);
    strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,"accept");
    len = strlen(ACCEPT_STRING);
    get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len + 1);
    strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,ACCEPT_STRING);

    len = strlen("accept");
    get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len + 1);
    strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,"accept");
    len = strlen(ACCEPT_STRING);
    get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len + 1);
    strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,ACCEPT_STRING);
#endif
    SetWreParamToHttpHeader(&(get_param_ptr->extern_header),phttp_info,step);

    get_param_ptr->get_data.is_header_use_file = FALSE;

    //MMI_STRNTOWSTR(get_param_ptr->get_data.header_file_name,24,
    //          g_http_head_name,
    //          24,strlen(g_http_head_name));

    //MMIAPICOM_StrToWstr(HTTP_HEADER_FILE_NAME,get_param_ptr->get_data.header_file_name);
    MMIAPICOM_Wstrcpy(get_param_ptr->get_data.header_file_name, s_update_kernel_path);
    MMIAPICOM_Wstrcat(get_param_ptr->get_data.header_file_name, L"\\");

	if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
    {
		MMIAPICOM_Wstrcat(get_param_ptr->get_data.header_file_name, WRE_HTTP_HEADER_FILE_NAME);
	}
	else
	{
		MMIAPICOM_Wstrcat(get_param_ptr->get_data.header_file_name, WRE_PART2_HTTP_HEADER_FILE_NAME);
	}

    SFS_DeleteFile(get_param_ptr->get_data.header_file_name,NULL);
    //memcpy(get_param_ptr->get_data.header_file_name,HTTP_HEADER_FILE_NAME,24);

    //MMIAPICOM_StrToWstr(get_param_ptr->get_data.content_file_name,24,
    //          g_http_content_name,24,strlen(g_http_content_name));

    //MMIAPICOM_StrToWstr(HTTP_CONTENT_FILE_NAME,get_param_ptr->get_data.content_file_name);
    MMIAPICOM_Wstrcpy(get_param_ptr->get_data.content_file_name, s_update_kernel_path);
    MMIAPICOM_Wstrcat(get_param_ptr->get_data.content_file_name, L"\\");

    if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
	{
		MMIAPICOM_Wstrcat(get_param_ptr->get_data.content_file_name, WRE_HTTP_CONTENT_FILE_NAME);
	}
	else
	{
		MMIAPICOM_Wstrcat(get_param_ptr->get_data.content_file_name, WRE_PART2_HTTP_CONTENT_FILE_NAME);
	}
    
    
    //memcpy(get_param_ptr->get_data.content_file_name,HTTP_CONTENT_FILE_NAME,24);
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_FILE;

    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;

    get_param_ptr->need_net_prog_ind = TRUE;
    get_param_ptr->is_use_relative_url = TRUE;
    HTTP_GetRequest(context_id,get_param_ptr,app_instance);

    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->referer.referer_ptr);
    SCI_FREE(get_param_ptr->ua_profile.ua_profile_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(get_param_ptr->uri.uri_ptr);
    //SCI_FREE(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr);
    //SCI_FREE(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr);
    //SCI_FREE(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr);
    //SCI_FREE(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr);
    FreeHeader(&(get_param_ptr->extern_header));

    SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
    get_param_ptr->extern_header.other_header_ptr = NULL;
    
    SCI_FREE(get_param_ptr);
}
PUBLIC uint32 MMIWRE_GetFileSize(uint32 step)
{
    SFS_HANDLE   hFile;
    uint32       filesize;
    uint16       filepath[156];

    MMIAPICOM_Wstrcpy(filepath, (const wchar*)WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    MMIAPICOM_Wstrcat(filepath, L"\\");
	if(ONEKEY_DOWNLOAD_STEP_PART1 == step)
	{
	    MMIAPICOM_Wstrcat(filepath, WRE_HTTP_CONTENT_FILE_NAME);
	}
	else
	{
		MMIAPICOM_Wstrcat(filepath, WRE_PART2_HTTP_CONTENT_FILE_NAME);
	}

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(NULL == hFile)
    {
        return 0;
    }

    SFS_GetFileSize(hFile,&filesize);

    SFS_CloseFile(hFile);

    return filesize;
}

/************* 菜单态加载 ***********************/
LOCAL void StartupWRE(void* param1,void *param2)
{
    uint32 appid = (*(uint32 *)param1);
    uint16 *appname = (uint16 *)param2;
    MMIWRE_DynStartup(appid, appname);
}
BOOLEAN DeleteApp(void* param1, void* param2)
{
    uint32 appid = (*(uint32 *)param1);
    char guid[WRE_GUID_NUM] = {0};
    //SCI_TRACE_LOW:"[WRE] DeleteApp  begin:"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2532_112_2_18_3_11_23_510,(uint8*)"");
    if (!Find_App_GUID(appid, (uint8 *)guid, WRE_GUID_NUM))
    {
        //SCI_TRACE_LOW:"[WRE] DeleteApp : FindAppGuid Fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2535_112_2_18_3_11_23_511,(uint8*)"");
        return TRUE;
    }
    if (!WriteGUIDToDeleteIni((uint8 *)guid, WRE_GUID_NUM))
    {
        //SCI_TRACE_LOW:"[WRE]DeleteApp : WriteGUIDToDeleteIni Fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2540_112_2_18_3_11_23_512,(uint8*)"");
        return TRUE;
    }

    //SCI_TRACE_LOW:"[WRE]DeleteApp : return TRUE!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2544_112_2_18_3_11_23_513,(uint8*)"");
    return TRUE;
}
BOOLEAN WRE_UninstallApp(uint32 appid, uint16 *appname)
{
    BOOLEAN   ret = TRUE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    GUIMAINMENU_DYNAMIC_INFO_T dynamic_item_info = {0};
/*
    dynamic_item_info.dynamic_menu_data_type = GUIMENU_MAINMENU_DATA_TYPE_DY_JAVA;
    dynamic_item_info.dynamic_menu_info.dynamic_java_menu.java_suiteId = appid;//只需要传入id即可
    ret = MMISLIDEMENU_DeleteDynamicMenu(&dynamic_item_info);
*/

    int nAppnameLen = MMIAPICOM_Wstrlen(appname);
    nAppnameLen = nAppnameLen>GUIMENU_DYMAINMENU_STR_MAX_NUM?GUIMENU_DYMAINMENU_STR_MAX_NUM:nAppnameLen;
    MMIAPICOM_Wstrncpy(dynamic_item_info.dynamic_menu_info.text, appname, nAppnameLen);
    dynamic_item_info.dynamic_menu_info.link_function_ptr = StartupWRE;
    ret = MMIMENU_DyDeleteItem(&dynamic_item_info);
    //SCI_TRACE_LOW:"[WRE] WRE_UninstallApp: appid = %d, appname = %s, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2561_112_2_18_3_11_24_514,(uint8*)"dsd", appid, appname, ret);
#endif
    return ret;
}

LOCAL GUIMENU_FILE_DEVICE_E WreDeviceMapGUIMenuDevice(WRE_FILE_DEVICE_E wreDevice)
{
    return (GUIMENU_FILE_DEVICE_E)wreDevice;
}
    
BOOLEAN WRE_InstalleApp(uint32 appid,
                        uint16 *appname,
                        uint   *iconbuf,
                        uint32  iconbuflen,
                        uint16 *apppath)
{
    BOOLEAN   ret = TRUE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMENU_SYN_VALUE_E ret_syn_value = MMIMENU_SYN_VALUE_SUC;
    GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr = PNULL;
    GUIMENU_FILE_DEVICE_E store_idx = WreDeviceMapGUIMenuDevice(WRE_GetInstallDevice(apppath));

    //MMIMENU_DyDeleteItemByFunc(StartupWRE);
    WRE_UninstallApp(appid, appname);

    dynamic_item_ptr = SCI_ALLOC_APP(sizeof(GUIMAINMENU_DYNAMIC_INFO_T));
    if (dynamic_item_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[WRE]WRE_InstalleApp:  alloc dynamic_item_ptr return PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2588_112_2_18_3_11_24_515,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(dynamic_item_ptr,0,sizeof(GUIMAINMENU_DYNAMIC_INFO_T));
/*
    //此处开始给新增菜单数据赋值，以供主菜单添加、显示
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.new_item_pic_ptr  = iconbuf;//图片数据
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.icon_datasize     = iconbuflen;//图片大小
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.is_icon_info = 1;//有图片

    dynamic_item_ptr->dynamic_menu_data_type = GUIMENU_MAINMENU_DATA_TYPE_DY_JAVA;//必须为此类型
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.java_suiteId  = appid;
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.link_function_ptr = StartupWRE;//此为开启应用的函数指针
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.param1 = appid;//启动函数参数1
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.param2 = 0;// 启动函数参数2

    MMIAPICOM_Wstrcpy(dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.new_item_text,appname);
    dynamic_item_ptr->dynamic_menu_info.dynamic_java_menu.new_item_text_len = MMIAPICOM_Wstrlen(appname);

    ret = MMISLIDEMENU_AddDynamicMenu(dynamic_item_ptr);//添加菜单接口
*/
    dynamic_item_ptr->dynamic_menu_info.has_icon = TRUE;
    dynamic_item_ptr->dynamic_menu_info.icon_datasize = iconbuflen;
    dynamic_item_ptr->dynamic_menu_info.icon_ptr = (uint8 *)iconbuf;
    dynamic_item_ptr->dynamic_menu_info.icon_height= 48;
    dynamic_item_ptr->dynamic_menu_info.icon_width= 48;
    dynamic_item_ptr->dynamic_menu_info.select_icon_id= IMAGE_MAINMENU_ICON_SELECT;

    MMIAPICOM_Wstrcpy(dynamic_item_ptr->dynamic_menu_info.text, appname);
    dynamic_item_ptr->dynamic_menu_info.text_len = MMIAPICOM_Wstrlen(appname);

    dynamic_item_ptr->dynamic_menu_info.link_function_ptr = StartupWRE;
    dynamic_item_ptr->dynamic_menu_info.delete_function_ptr = DeleteApp;
    dynamic_item_ptr->dynamic_menu_info.param1ptr = &appid;
    dynamic_item_ptr->dynamic_menu_info.param2ptr = appname;
    dynamic_item_ptr->dynamic_menu_info.parm1_size = 4;
    dynamic_item_ptr->dynamic_menu_info.parm2_size = (MMIAPICOM_Wstrlen(appname)+1)*sizeof(uint16);

    ret_syn_value = MMIMENU_DyAppendItem(dynamic_item_ptr, store_idx);

    if (MMIMENU_SYN_VALUE_SUC == ret_syn_value)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    //SCI_TRACE_LOW:"[WRE] WRE_InstalleApp: appid = %d, appname = %s, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_WINTAB_2627_112_2_18_3_11_24_516,(uint8*)"dsd", appid, appname, ret);

    SCI_FREE(dynamic_item_ptr);
#endif
    return ret;
}

//get guid and store id from fixed app list table.
BOOLEAN MMIWRE_GetFixAppGuid(uint16 * guid,
                             uint16 * store_app_id,
                             uint32  fixed_app_id)
{
#ifdef WRE_PREINSTALL_APP_SUPPORT
    uint32  i = 0;
    FIXED_APP_INFO_T *pFixAppList = WRE_GetFixAppList();
    for(i=0;i<MAX_WRE_FIXED_APP_NUM;i++)
    {
        if(fixed_app_id == pFixAppList[i].fixed_id)
        {
            MMIAPICOM_Wstrcpy(guid,pFixAppList[i].guid);
            MMIAPICOM_Wstrcpy(store_app_id,pFixAppList[i].store_app_id);
            return TRUE;
        }
    }
#endif
    return FALSE;
}

//get guid and store id from fixed app list table.
BOOLEAN MMIWRE_IsFixApp(uint16 * guid)
{
#ifdef WRE_PREINSTALL_APP_SUPPORT
    uint32  i = 0;
    FIXED_APP_INFO_T *pFixAppList = WRE_GetFixAppList();
    for(i=0;i<MAX_WRE_FIXED_APP_NUM;i++)
    {
        if(MMIAPICOM_Wstrcmp(guid,pFixAppList[i].guid)==0)
        {
            return TRUE;
        }
    }
#endif
    return FALSE;
}


void BOOT_MessageBox(int code,void* param){
	switch(code)
	{
	case 0x01:
		//TXT_JAVA_AMS_MIDLETSUITELDR_QUIT_OUT_OF_MEMORY
		MMIPUB_OpenAlertFailWin(TXT_WRE_QUIT_OUT_OF_MEMORY); 
		
		break;
	default:
		break;	// changed by yifei
	}

}


uint8 WRE_GetAvailableLinkIndex(uint32 dual_sys,  DIAL_TYPE dialType)
{
	uint8 i ;
	uint8 j =0;
    MMICONNECTION_LINKSETTING_DETAIL_T   * link_ptr = NULL;

RETRY_ANOTHER:
    for (i = 0; i < MMICONNECTION_GetLinkSettingNum(dual_sys); i++)
    {
        link_ptr = MMICONNECTION_GetLinkSettingItemByIndex(dual_sys, i);    
        if (0 == strcmp((const char *)(link_ptr->gateway), GATEWAY_HTTP_PROXY_NONE))
        {
            if (dialType == DIAL_NET)
            {
                SCI_TRACE_LOW("[WRE]WRE_GetAvailableLinkIndex: DIAL_NET return %d", i);
                return i;
            }
        }
        else
        {
            if (dialType == DIAL_WAP)
            {
                SCI_TRACE_LOW("[WRE]WRE_GetAvailableLinkIndex: DIAL_WAP return %d", i);
                return i;
            }
        }
    }

	if(j == 0)
	{
		j ++;
		if(dialType == DIAL_WAP)
		{
			dialType = DIAL_NET;
		}
		else
		{
			dialType = DIAL_WAP;
		}
		goto RETRY_ANOTHER;
	}

    SCI_TRACE_LOW("[WRE]WRE_GetAvailableLinkIndex: ERROR return 0");
    
	return 0;
}

BOOL WRE_IsChinaUnicom(uint32 dual_sys)
{
    PHONE_PLMN_NAME_E plmn = MMIPHONE_GetNetworkName((MN_DUAL_SYS_E)dual_sys);
    if (plmn == PLMN_UNICOM)
    {
        //SCI_TRACE_LOW("[WRE]WRE_IsChinaArea: return 1");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW("[WRE]WRE_IsChinaArea: return 0");
        return FALSE;
    }
}


BOOL IsNetworkOk()
{
    BOOL ret = FALSE;
    int  i;

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        if( ((MMIAPIPHONE_GetPlmnStatus(i)  >   PLMN_NO_SERVICE ) && (MMIAPIPHONE_GetPlmnStatus(i)  <=  PLMN_NORMAL_GSM_GPRS_BOTH )) )
        {
            ret = TRUE;
            break;
        }

    }

    return ret;

}

LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*选卡需求*/
//当wifi模块开启时,该窗口为网络选择(sim卡和wifi网络)
WINDOW_TABLE(MMIWRE_SELECT_SIM_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSIMSelectWinMsg),
    WIN_ID(MMIWRE_WIN_ID_SELECT_SIM),
    WIN_TITLE(TXT_SIM_SEL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,  MMIWRE_CTRL_ID_SELECT_SIM_ID),
    END_WIN
};


PUBLIC void MMIWRE_SetSimSelect(MN_DUAL_SYS_E sim_select)
{
    if (sim_select >= MMI_DUAL_SYS_MAX)/*做个保护*/
    {
        sim_select = MN_DUAL_SYS_1;
    }
    s_onekey_download_info.s_gprs_info.s_wre_sim_select = sim_select;
}

LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              cur_selection;
    MMI_CTRL_ID_T       ctrl_id = MMIWRE_CTRL_ID_SELECT_SIM_ID;
    MN_DUAL_SYS_E		dual_sys;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_WIFI_SUPPORT
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_SIM_SEL,FALSE);
#else
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
		s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi = FALSE;
        //GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        if(cur_selection == s_onekey_download_info.s_gprs_info.s_valid_sim_num)
        {
#ifdef MMI_WIFI_SUPPORT
            if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
            {
				MMIWRE_RefreshSimParam(0,TRUE);
            }
            else
            {
				MMIWRE_RefreshSimParam(0,TRUE);
            }
#else
			dual_sys = s_onekey_download_info.s_gprs_info.s_valid_sim_idx[cur_selection];

			MMIWRE_RefreshSimParam(dual_sys,FALSE);
#endif
        }
        else
        {
            //dual_sys = s_onekey_download_info.s_gprs_info.s_valid_sim_idx[cur_selection];
            //MMIWRE_SetSimSelect((MN_DUAL_SYS_E)dual_sys);
			MMIWRE_RefreshSimParam(s_onekey_download_info.s_gprs_info.s_valid_sim_idx[cur_selection],FALSE);
        }
		
        MMK_CloseWin(win_id);
        ShowWRELoading();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
		MMK_CloseWin(win_id);//返回上一级
		
		MMK_CloseApplet(SPRD_WRE_APPLET_ID);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}
LOCAL void WREAppendSelectListItem(MMI_CTRL_ID_T ctrl_id, MN_DUAL_SYS_E dual_sys)
{
    int32               i = 0,sim_idx;
    BOOLEAN             bExisted, bValidate;
    MMISET_SIM_NAME_T   sim_str[MMI_DUAL_SYS_MAX]     = {0};
    uint16              max_num;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
#ifdef MMI_WIFI_SUPPORT
    MMI_STRING_T   item_str[MMI_DUAL_SYS_MAX + 1] = {0};
    BOOLEAN        bWifiInUse = FALSE;
    if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        bWifiInUse = TRUE;
    }
#else
    MMI_STRING_T   item_str[MMI_DUAL_SYS_MAX] = {0};
#endif

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    s_onekey_download_info.s_gprs_info.s_valid_sim_num = 0;
#if 1
    for(i=0;i<MMI_DUAL_SYS_MAX;i++)
    {
        bExisted = MMIAPIPHONE_GetSimExistedStatus(i);
        bValidate = MMIAPIPHONE_IsSimOk(i);
        if(bExisted&&bValidate)
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            sim_img_id[s_onekey_download_info.s_gprs_info.s_valid_sim_num] = IMAGE_IDLE_PROMPT_SIM;
#else
            sim_img_id[s_onekey_download_info.s_gprs_info.s_valid_sim_num] = IMAGE_IDLE_PROMPT_SIM1 + (i - MN_DUAL_SYS_1);
#endif
            s_onekey_download_info.s_gprs_info.s_valid_sim_idx[s_onekey_download_info.s_gprs_info.s_valid_sim_num] = i;
            s_onekey_download_info.s_gprs_info.s_valid_sim_num ++;
        }
    }
#endif

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_RADIO;

    max_num = s_onekey_download_info.s_gprs_info.s_valid_sim_num;

#ifdef MMI_WIFI_SUPPORT
    if (bWifiInUse)
    {
    max_num += 1;
    }    
#endif

    GUILIST_SetMaxItem(ctrl_id, max_num, FALSE );

    for (i = 0; i < s_onekey_download_info.s_gprs_info.s_valid_sim_num;i++)
    {
        wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
        MMI_STRING_T network_str = {0};

        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
        sim_idx = s_onekey_download_info.s_gprs_info.s_valid_sim_idx[i];

        item_str[i].wstr_len = 0;
        item_str[i].wstr_ptr = NULL;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        dual_sys = (MN_DUAL_SYS_E)sim_idx;        
        sim_str[i] = MMIAPISET_GetSimName(dual_sys);
        item_str[i].wstr_len = sim_str[i].wstr_len;
        item_str[i].wstr_ptr = sim_str[i].wstr_arr;
#endif
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = sim_img_id[i];

        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = item_str[i].wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_str[i].wstr_ptr;

        SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
        network_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));

        MMIAPISET_GetSimServiceString(sim_idx, &network_str);

        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = network_str.wstr_len;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

        GUILIST_AppendItem(ctrl_id, &item_t);
    }

#ifdef MMI_WIFI_SUPPORT
    {
		item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
		MMI_GetLabelTextByLang(TXT_WIFI,&item_str[s_onekey_download_info.s_gprs_info.s_valid_sim_num]);
		
		SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
		
		item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
		item_data.item_content[0].item_data.image_id = MAIN_GetIdleWifiIcon();
		
		item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[1].item_data.text_buffer.wstr_len = item_str[s_onekey_download_info.s_gprs_info.s_valid_sim_num].wstr_len;
		item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_str[s_onekey_download_info.s_gprs_info.s_valid_sim_num].wstr_ptr;
		
		GUILIST_AppendItem(ctrl_id, &item_t);
    }
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
}

PUBLIC void MMIWRE_EntrySelectNetworkWin(MN_DUAL_SYS_E dual_sys)
{
    MMK_CreateWinByApplet(SPRD_WRE_APPLET_ID, (uint32*)MMIWRE_SELECT_SIM_WIN_TAB, (ADD_DATA)dual_sys);

    WREAppendSelectListItem(MMIWRE_CTRL_ID_SELECT_SIM_ID, dual_sys);
}

//wre network selection
LOCAL BOOLEAN MMIWRE_SelectNetworkType(void)
{
    BOOLEAN is_sim1_ok;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    BOOLEAN is_need_sel = FALSE;//记录是否需要进行网络选择
    int     i;
    int     valid_sim_num = 0;
    BOOLEAN bValidate;
    BOOLEAN bWifiInUse = FALSE;

#ifdef MMI_WIFI_SUPPORT   
	if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        bWifiInUse = TRUE;
    }
#endif

    is_sim1_ok = TRUE;

    for(i=0;i<MMI_DUAL_SYS_MAX;i++)
    {
        is_need_sel = MMIAPIPHONE_GetSimExistedStatus(i);
        bValidate = MMIAPIPHONE_IsSimOk(i);
        if(is_need_sel&&bValidate)
        {
            s_onekey_download_info.s_gprs_info.s_valid_sim_idx[valid_sim_num] = i;
            ++valid_sim_num;
            dual_sys = i;
        }
    }

#ifndef MMI_WIFI_SUPPORT
    if(valid_sim_num>1)
        is_need_sel = TRUE;
    else
    {
        is_need_sel =  FALSE;
        if(valid_sim_num == 0)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();//无sim卡            
            return FALSE;
        }
    }
#else//WIFI_SUPPORT
    s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi = FALSE;//reset

    if((bWifiInUse && valid_sim_num > 0) 
    	|| (valid_sim_num > 1))
        is_need_sel =  TRUE;//有wifi时,默认进行网络选择
    else
        is_need_sel =  FALSE;//有wifi时,默认进行网络选择

#endif
    //进入网络选择(or sim卡选择)
    if (is_need_sel)/*lint !e774*/
    {
        MMIWRE_EntrySelectNetworkWin(dual_sys);
        return TRUE;
    }
    else
    {
		if(bWifiInUse)
			s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi = TRUE;
		else
			s_onekey_download_info.s_gprs_info.s_wre_is_use_wifi = FALSE;

     //   MMIWRE_SetSimSelect(s_onekey_download_info.s_gprs_info.s_valid_sim_idx[0]);
        MMIWRE_RefreshSimParam(s_onekey_download_info.s_gprs_info.s_valid_sim_idx[0],bWifiInUse);
        ShowWRELoading();
    }

    return TRUE;
}

LOCAL BOOLEAN IsWREUpdateDiskEnough(void)
{
    MMIFILE_DEVICE_E store_idx = MMI_DEVICE_SYSTEM;
	uint16 * path;

    path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
    if (path == PNULL)
    {
        return FALSE;    
    }
    
    store_idx = WreDeviceMapFileDevice(WRE_GetInstallDevice(path));
    if (!IsDevEnoughSpace(store_idx) && store_idx != MMI_DEVICE_SYSTEM)
    {
        return FALSE;         
    }

	return TRUE;
}

//add begin 20120521  --- 解决分辨率不相配
#ifdef WRE_AUTO_STORE_SREEN_FIX

int WRE_ReadIniValue(uint16 *lpPath, uint8 *name,	uint8 *out, int size)
{
	uint8 *p = NULL, *find = NULL, *lpHead = NULL;
    int len = 0, lenRead = 0, i = 0;
    SFS_ERROR_E err = SFS_NO_ERROR;
    SFS_HANDLE hFile = SFS_CreateFile(lpPath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
	if(!hFile) {
		SCI_TRACE_LOW("[WRE]WRE_ReadIniValue: not find file(StoreVer.dat)");
		return 0;
	}
    
    SFS_GetFileSize(hFile, (uint32 *)&len); // changed by yifei
	p = (uint8*)SCI_ALLOCA(len+2);
	if (!p)
	{
		SFS_CloseFile(hFile);
		return 0;
	}
    memset(p, 0, len+2);
	
    err = SFS_ReadFile(hFile, p, len, (uint32 *)&lenRead, NULL);    // changed by yifei
    SFS_CloseFile(hFile);
    if (SFS_NO_ERROR != err)
    {
        SCI_FREE(p);
        return 0;
    }
	
	//SCI_TRACE_LOW("[WRE]in StoreVer.dat,p[0]=%08x,p[1]=%08x",p[0],p[1]);
    SCI_TRACE_LOW("[WRE]in StoreVer.dat,value is %s",p);
	SCI_TRACE_LOW("[WRE]find str = %s",name);
	
	lpHead = p;
    find = strstr(lpHead, name);
	if(!find) 
    {
		SCI_TRACE_LOW("[WRE]in StoreVer.dat,not find name");
		SCI_FREE(p);
		return 0;
	}
    
    {
        uint8 *lpBuff = find;
        while((*(lpBuff++) != 0) && (*(lpBuff++) != L'='))
            ;
		find = lpBuff;
	}
    
	if(*find == 0)   /*lint !e774*/
	{
		SCI_TRACE_LOW("[WRE]in StoreVer.dat,not find =");
		SCI_FREE(p);
		return 0;
	}
	
	i = 0;
	while(*find != 0 && *find != L'\r' && i < size)
	{
		out[i++] = *find++;
	}
	
	SCI_TRACE_LOW("[WRE]store size = %s",out);
	
	SCI_FREE(p);
	return 1;
}

#define STORE_VERSION_FILE           L"\\StoreVer.dat"
#define STORE_VERSION_KEY_LCDSIZE    "STORE_LCDSIZE"

PUBLIC int WRE_IsStoreScreenFix(void)
{
    uint16 srcFilename[128] = {0};
    uint8 *lpKey = STORE_VERSION_KEY_LCDSIZE;
    uint8 value[64+1] = {0};
    int ret = STORE_SCREEN_IS_FIXED;

    MMIAPICOM_Wstrcpy(srcFilename, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    MMIAPICOM_Wstrcat(srcFilename, STORE_VERSION_FILE);
    
    if (WRE_ReadIniValue(srcFilename, lpKey, value, sizeof(value)-1))
    {
#ifdef MAINLCD_SIZE_240X320
		if(strcmp(value, "240X320") != 0)
	 	{
	 		ret = STORE_SCREEN_NOT_FIXED;
	 	}
#elif defined MAINLCD_SIZE_320X480
		if(strcmp(value, "320X480") != 0)
	 	{
	 		ret = STORE_SCREEN_NOT_FIXED;
	 	}
#elif defined MAINLCD_SIZE_240X400
		if(strcmp(value, "240X400") != 0)
	 	{
	 		ret = STORE_SCREEN_NOT_FIXED;
	 	}
#else
        	ret = STORE_SCREEN_NOT_SURE;
#endif
    }
    else
    {
		ret = STORE_SCREEN_NOT_SURE;
    }

    SCI_TRACE_LOW("[WRE]WRE_IsStoreScreenFix return %d", ret);
    
    return ret;  
}

static void MMIWRE_DeleteWRE(void)
{
    uint16       filepath[156];
	//int len;
    
	SCI_TRACE_LOW("[WRE]MMIWRE_DeleteWRE");
	filepath[0] = 0;
    MMIAPICOM_Wstrcpy(filepath, (const wchar*)WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
	#if 0
	len = MMIAPICOM_Wstrlen(filepath);
    if (len > 0 && len < 156)
    {
		int i;
		for (i=len-1;i>=0;i--)
		{
			if (filepath[i] == '\\')
			{
				break;
			}
		}
		if (i > 0)
		{
			filepath[i] = 0;
			MMIFILE_DeleteTreeEx(filepath,filepath+i+1,TRUE);
		}
    }
	#endif
	MMIAPICOM_DelDirAll(filepath,SFS_MAX_PATH);
}

extern int hot_reset_tag;

LOCAL MMI_RESULT_E HandleUpdateQueryWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)                
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMK_CloseWin(win_id);

            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_703_112_2_18_3_10_31_11,(uint8*)"");
			MMIWRE_DeleteWRE();
            
            hot_reset_tag = 0;            
            SetDownLoadFlag();
            DownloadAndInstallWRE(TRUE);            
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
            
            WRE_EntryApp();
        }
        break;
    
    case MSG_CLOSE_WINDOW:
        break;   
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}

PUBLIC void OpenWREQueryWin(int  format)
{
	int str_id;
	if(format == 0)
	{
		str_id= TXT_WRE_VERSION_ERR;
	}
	else
	{
		str_id= TXT_WRE_VERSION_NOT_SURE;
	}
	MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, str_id, IMAGE_PUBWIN_QUERY,\
        	NULL, HandleUpdateQueryWinMsg, NULL);
}

BOOLEAN wre_load_store_ver_dat(uint16 *dir)
{
	uint16       lpPath[156];	
	SFS_HANDLE hFile;
	int len,err,lenRead;
	char    store_ver[50+1] = {0};
	//int w,h;
	BOOLEAN ret = TRUE;

	if(dir == NULL || dir[0] == 0)
	{
		return FALSE;
	}
	len = MMIAPICOM_Wstrlen(dir);
	if(len + MMIAPICOM_Wstrlen(STORE_VERSION_FILE) >= 156)
	{
		return FALSE;
	}
	MMIAPICOM_Wstrcpy(lpPath,dir);
	MMIAPICOM_Wstrcat(lpPath,STORE_VERSION_FILE);
	hFile = SFS_CreateFile(lpPath, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, 0, 0);
	if(hFile == 0)
	{
		return FALSE;
	}
	err = SFS_ReadFile(hFile, store_ver, sizeof(store_ver)-1, (uint32 *)&lenRead, NULL);    // changed by yifei
	if (SFS_NO_ERROR != err)
	{
		SFS_CloseFile(hFile);
       	return FALSE;
	}
	if (strstr(store_ver,"STORE_LCDSIZE"))
	{
		SCI_TRACE_LOW("store_ver exist,store_ver = %s",store_ver);
	}
	else
	{
		memset(store_ver,0,sizeof(store_ver));
#ifdef MAINLCD_SIZE_240X320
		strcpy(store_ver,"STORE_LCDSIZE=240X320");
#elif defined MAINLCD_SIZE_320X480        	
		strcpy(store_ver,"STORE_LCDSIZE=320X480");
#elif defined MAINLCD_SIZE_240X400        	
		strcpy(store_ver,"STORE_LCDSIZE=240X400");
#else
#endif
		SCI_TRACE_LOW("store_ver not exist,store_ver info: %s",store_ver);
		err = SFS_WriteFile(hFile,store_ver,sizeof(store_ver)-1,&lenRead,NULL);
		if (err != SFS_NO_ERROR)ret = FALSE;
	}
	SFS_CloseFile(hFile);
	return ret;
}
#endif
//add end 20120521
#endif // WRE_SUPPORT

/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
