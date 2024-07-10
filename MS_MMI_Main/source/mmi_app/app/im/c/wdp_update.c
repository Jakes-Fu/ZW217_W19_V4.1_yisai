#ifndef WDP_UPDATE_C
#define WDP_UPDATE_C
#include "mmi_app_im_trc.h"
#include "mmi_common.h"
#include "mmi_image.h"
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#include "wdp_update.h"
#include "wdp_update_wintab.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"
#include "wdp_encrpty.h"
#include "WD_Def.h"//6_12_2
#include "wdp_SG_NetWork_Def.h"
#include "mn_old_api.h"
#include "mmi_id.h"
#include "mmipub.h"
#include "mmi_image.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmiim_text.h"
#include "mmipdp_export.h"
#include "mmiim_sogou.h"
#include "mmiconnection_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#define SOGOU_TRACE_OUT    SCI_TRACE_LOW

// 屏幕分辨率定义
#ifdef MAINLCD_SIZE_240X320
#define SCREEN_RESOLUTION "240x320"
#elif defined MAINLCD_SIZE_128X160
#define SCREEN_RESOLUTION "128x160"
#elif defined MAINLCD_SIZE_176X220
#define SCREEN_RESOLUTION "176x220"
#elif defined MAINLCD_SIZE_240X400
#define SCREEN_RESOLUTION "240x400"
#elif defined MAINLCD_SIZE_320X480
#define SCREEN_RESOLUTION "320x480"
#else
#define SCREEN_RESOLUTION "240x320"
#endif

// 网络等待定时器时间长度
#define NET_WAITING_TIMER_DURATION    100

// 激活PDP最长等待时间
#define PDP_WAITING_TIMER_DURATION    10000
// 解析服务器最大重试次数
#define GETSERVER_MAX_RETRY_TIMES   10

// 输入法cookie信息
#define SG_IME_BUILD_TIME     "20100531110000"
#define SG_IME_MODE_ID        "000"

// D盘词库文件路径
#define SG_D_WORDLIB_FILENAME   L"D:\\@sogou\\worddata"
// D盘SKINBIN文件路径
#define SG_D_SKINBIN_FILENAME   L"D:\\@sogou\\skin.bin"
// E盘词库文件路径
#define SG_E_WORDLIB_FILENAME   L"E:\\@sogou\\worddata"
// E盘SKINBIN文件路径
#define SG_E_SKINBIN_FILENAME   L"E:\\@sogou\\skin.bin"
// 词库时间戳文件最大长度
#define SG_WORDDATA_MAX_LENGTH  20
// SKINBIN文件最小长度
#define SG_SKINBIN_FILE_MIN_LENGTH 20
extern uint8 g_sg_save_position;//0:D盘,1:E盘,2:非D非E

BOOLEAN g_sg_IsWap=FALSE;//6_12_3

// 是否已经设置网络成功
LOCAL BOOLEAN s_is_set_network = FALSE;
// 是否正在解析网络地址
LOCAL BOOLEAN s_is_resolving_address = FALSE;
// 当前更新网络状态
LOCAL SG_IM_UPD_S_E s_update_status = SG_IM_UPD_S_NONE;
// 心跳timer
LOCAL uint8 s_lib_timer_id = 0;
// 激活PDP timer
LOCAL uint8 s_waiting_pdp_timer_id = 0;
// 域名解析timer
LOCAL uint8 s_getserver_delay_timer_id = 0;
// 域名解析重试次数
LOCAL uint32 s_get_server_retry_time = 0;
// 更新时所用的SIM卡ID
LOCAL MN_DUAL_SYS_E s_sim_select = MN_DUAL_SYS_1;
// 服务器地址
LOCAL uint32 s_server_address = 0;
// 本次更新的词汇
LOCAL uint16 s_display_string[DISPLAY_TO_USER_STRING_LEN+2+2]={0};
// 更新用的链路id
LOCAL uint32 s_sogou_update_net_id = 0;

/*****************************************************************************/
//  Description : 初始化连接
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateInit(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 读取皮肤配置文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN sg_read_skinbinfile(void);
#endif
/*****************************************************************************/
//  Description : PS消息处理函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleSGPsMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : 激活GPRS并获取地址
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_ActiveNetGprsAndGetAddress(void);

/*****************************************************************************/
//  Description : 获取IMSI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_Get_IMSI(MN_DUAL_SYS_E cardtype,char * pIMSI,uint8 * lenofImsi);

/*****************************************************************************/
//  Description : 获取IMEI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_Get_IMEI(MN_DUAL_SYS_E cardtype,char * pIMEI,uint8 * lenofImei);

/*****************************************************************************/
//  Description : 更新词库
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_update_words(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 下载皮肤文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_download_skin_file(void);

/*****************************************************************************/
//  Description : 下载皮肤配置文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_download_skinbin(void);
#endif
/*****************************************************************************/
//  Description : 设置更新网络
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_UpdateSetNetWork(void);

/*****************************************************************************/
//  Description : 初始化更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_InitParamBeforeUpdate(void);

/*****************************************************************************/
//  Description : 获取IMEI 和 IMSI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGGetIMEI_IMSI(char *imei_ptr, char* imsi_ptr, uint8 *imei_len_ptr, uint8 *imsi_len_ptr);

/*****************************************************************************/
//  Description : 创建网络等待定时器
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGSetNetWaitingTimer(void);

/*****************************************************************************/
//  Description : 初始化更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_InitParamBeforeUpdate(void)
{
    char IMEI[PROJECT_IMEI_LEN] = { 0 };
    char IMSI[PROJECT_ISEI_LEN] = { 0 };
    uint8 lenofImei = 0;
    uint8 lenofImsi = 0;
    SG_InitInputStruct sg_initInput = { 0 };
    SCI_DATE_T cur_date = { 0 };
    SCI_TIME_T cur_time = { 0 };
    
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    SGGetIMEI_IMSI(IMEI, IMSI, &lenofImei, &lenofImsi);
    
    sprintf(sg_initInput.current_time, "%04d%02d%02d%02d%02d%02d",      //lint !e64
        cur_date.year, cur_date.mon, cur_date.mday,
        cur_time.hour, cur_time.min, cur_time.sec);
    SCI_TRACE_LOW("[MMIIM] wdp_update.c SG_InitParamBeforeUpdate current_time: %s", sg_initInput.current_time);
    SCI_STRCPY((char *)sg_initInput.mode_id, (char *)SG_IME_MODE_ID);
    SCI_STRCPY((char *)sg_initInput.resolution, (char *)SCREEN_RESOLUTION);
    SCI_STRCPY((char *)sg_initInput.sg_imei, IMEI);
    SCI_STRCPY((char *)sg_initInput.sg_imsi, IMSI);
#ifndef WIN32
    SG_InterNet_Init(&sg_initInput);
#endif
}



#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 读取皮肤配置文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN sg_read_skinbinfile(void)
{
    SFS_HANDLE  file_handle;
    SFS_ERROR_E   sfs_err;
    uint32      file_size = 0;
    uint32      read_len = 0;
	uint16 sg_skinbin_filename[125]={0};
	SCI_MEMSET (sg_skinbin_filename,0,sizeof(sg_skinbin_filename));

	SCI_TraceLow("[MMIIM]sg_read_skinbinfile g_sg_save_position=%d", g_sg_save_position);

	if(0 == g_sg_save_position)
	{
		SCI_MEMCPY (sg_skinbin_filename,SG_D_SKINBIN_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_D_SKINBIN_FILENAME));
	}
	else if(1 == g_sg_save_position)
	{
		SCI_MEMCPY (sg_skinbin_filename,SG_E_SKINBIN_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_E_SKINBIN_FILENAME));
	}
	else
	{
		return FALSE;
	}
	
    file_handle = SFS_CreateFile((uint16*)sg_skinbin_filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
    if(0==file_handle)
    {
    	SCI_TraceLow ("sg_read_skinbinfile SFS_CreateFile");
        return FALSE;
    }
	
    sfs_err =SFS_GetFileSize(file_handle,&file_size);
	//SOGOU_TRACE_OUT:"sg_read_skinbinfile sfs_err=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_194_112_2_18_2_29_32_305,(uint8*)"d",sfs_err);
    if(0==file_size)
    {
        return FALSE;
    }
	
    MMIAPIIM_SGAllocSkinFileList(file_size + 2);
    SFS_ReadFile(file_handle, MMIAPIIM_GetSkinFileList(), file_size, &read_len, PNULL);
    SFS_CloseFile(file_handle);

    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : 初始化连接
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateInit(void)
{
    //SOGOU_TRACE_OUT:"wdp_update.c UpdateInit: begin."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_212_112_2_18_2_29_32_306,(uint8*)"");
    s_sogou_update_net_id = 0;
    s_is_set_network = FALSE;
    s_waiting_pdp_timer_id = 0;
#ifdef SOGOU_USE_PDA_SKIN
    if (MMIAPIIM_SGGetUpdateType() == SG_UPDATE_TYPE_SKIN_DIR)
    {
        MMIAPIIM_SGAllocSkinInf();
    }
#endif
    s_update_status = SG_IM_UPD_S_SETTING_NETWORK;

    s_getserver_delay_timer_id = 0;
    s_get_server_retry_time = 0;
    s_is_resolving_address = FALSE;

    MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_SETING_NETWORK, NULL);
    MMIAPIIM_SGUpdateCallBackFunc();
}

/*****************************************************************************/
//  Description : 设置更新网络
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_UpdateSetNetWork(void)
{
    //SOGOU_TRACE_OUT:"wdp_update.c SG_UpdateSetNetWork s_is_resolving_address = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_236_112_2_18_2_29_32_307,(uint8*)"d", s_is_resolving_address);
    if (s_is_resolving_address == FALSE)
    {
        SG_ActiveNetGprsAndGetAddress();
        s_is_resolving_address = TRUE;
    }
}

/*****************************************************************************/
//  Description : 更新解包状态
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateUnPacking(void)
{
#ifdef SOGOU_USE_PDA_SKIN    
    BOOLEAN read_skinbin_file=FALSE;
#endif
    MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_UNPACKING, NULL);

    switch (MMIAPIIM_SGGetUpdateType())
    {
    case SG_UPDATE_TYPE_SKIN_FILE:
        MMK_PostMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_SUCCEED, PNULL,0);
        break;
#ifdef SOGOU_USE_PDA_SKIN
    case SG_UPDATE_TYPE_SKIN_DIR:
        // 创建皮肤列表
        read_skinbin_file=sg_read_skinbinfile();
        if(FALSE==read_skinbin_file)
        {
            MMIAPIIM_SGFreeSkinFileList();
            MMK_PostMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_RECEIVE_FAILED, PNULL,0);
        }
        else
        {
            uint8 *file_ptr = MMIAPIIM_GetSkinFileList();
            s_update_status = SG_IM_UPD_S_NONE;
            if ((0xFF == *file_ptr) && (0xFE == *(file_ptr + 1)))
            {
                MMIAPIIM_SoGouOpenSkinBrowseWin();
            }
            else
            {
                MMIAPIIM_SGFreeSkinFileList();
                MMK_PostMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_RECEIVE_FAILED, PNULL,0);
            }
        }
        break;
#endif
    case SG_UPDATE_TYPE_VOCABULARY:
        MMK_PostMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_SUCCEED, PNULL,0);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 释放更新
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpdateRelease(void)
{
#ifndef WIN32
    SG_InterNet_Exit();
#endif
    s_update_status = SG_IM_UPD_S_NONE;
    s_sogou_update_net_id = 0;
    s_is_set_network = FALSE;
    if (s_waiting_pdp_timer_id)
    {
        MMK_StopTimer(s_waiting_pdp_timer_id);
        s_waiting_pdp_timer_id = 0;
    }
    MMIAPIPDP_Deactive(MMI_MODULE_IM);
    MMIAPIIM_SGSetUpdateType(SG_UPDATE_TYPE_NONE);
}

/*****************************************************************************/
//  Description : 更新回调函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpdateCallBackFunc(void)
{
#ifdef SOGOU_USE_PDA_SKIN    
    BOOLEAN is_space_enough = FALSE;
    uint32 free_high_word = 0;
    uint32 free_low_word  = 0;
#endif

    //SOGOU_TRACE_OUT:"wdp_update.c MMIAPIIM_SGUpdateCallBackFunc: s_update_status = %d ,s_is_set_network = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_325_112_2_18_2_29_33_308,(uint8*)"dd", s_update_status, s_is_set_network);

    switch (s_update_status)
    {
    case SG_IM_UPD_S_START:
        UpdateInit();
        break;

	case SG_IM_UPD_S_SETTING_NETWORK:
#ifdef MMI_WIFI_SUPPORT
            if (MMIAPIIM_SGGetIsOverWifi())
            {
                if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
                {
#ifdef SOGOU_USE_PDA_SKIN              
                    if (MMIAPIIM_SGGetUpdateType() == SG_UPDATE_TYPE_SKIN_DIR)
                    {
                        MMIAPIIM_SGAllocSkinInf();
                    }
#endif                
                    s_sogou_update_net_id = MMIAPIWIFI_GetNetId();
                    s_is_set_network = TRUE;
                    s_server_address = 0;
                    s_update_status = SG_IM_UPD_S_CREATING_SOCKET;
                    MMIAPIIM_SGUpdateCallBackFunc();
                }
                else
                {
                    MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
                    MMIPUB_OpenAlertWarningWin( TXT_SOGOU_SELECT_WIFI_NET);
                }
    		}
		else
#endif
		{
			SG_UpdateSetNetWork();
		}
		break;

	case SG_IM_UPD_S_CREATING_SOCKET:
        switch (MMIAPIIM_SGGetUpdateType())
        {
#ifdef SOGOU_USE_PDA_SKIN
        case SG_UPDATE_TYPE_SKIN_DIR:
            SG_download_skinbin();
            break;

        case SG_UPDATE_TYPE_SKIN_FILE:
            if (MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                    MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                                           &free_high_word, &free_low_word))
            {
                if (!(0 == free_high_word && SG_MINIMIZE_FREESPACE > free_low_word))
                {
                    is_space_enough = TRUE;
                }
            }
            if (is_space_enough)
            {
                SG_download_skin_file();
            }
            else
            {
                MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
            }
            break;
#endif
        case SG_UPDATE_TYPE_VOCABULARY:
            SG_update_words();
            break;

        default:
            break;
        }
        break;

    case SG_IM_UPD_S_UNPACKING:
        UpdateUnPacking();
        break;
    case SG_IM_UPD_S_END:
        MMIAPIIM_SGUpdateRelease();
        break;
    // PDP active超时    
    case SG_IM_UPD_S_PDPACTIVE_TIMEOUT:
        s_waiting_pdp_timer_id = 0;
        MMIAPIPDP_Deactive(MMI_MODULE_IM);
        if (MMK_IsOpenWin(SG_WIN_ID_UPDATE_WATCHING))
        {
            MMK_SendMsg(SG_WIN_ID_UPDATE_WATCHING, MSG_SG_UPD_WAIT_CONNECTING_TIME_OUT, NULL);
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 获取IMEI 和 IMSI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGGetIMEI_IMSI(char *imei_ptr, char* imsi_ptr, uint8 *imei_len_ptr, uint8 *imsi_len_ptr)
{
	/*lint -save -e506*/
	/*lint -save -e774*/
    if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
    {
        SG_Get_IMEI(MN_DUAL_SYS_1, imei_ptr, imei_len_ptr);
        SG_Get_IMSI(MN_DUAL_SYS_1, imsi_ptr, imsi_len_ptr);
    }
#if defined MULTI_SIM_SYS_DUAL || defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD    
    else if (MN_SYS_NUMBER > MN_DUAL_SYS_2 && MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        SG_Get_IMEI(MN_DUAL_SYS_2, imei_ptr, imei_len_ptr);
        SG_Get_IMSI(MN_DUAL_SYS_2, imsi_ptr, imsi_len_ptr);
    }
#endif
#if defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
    else if (MN_SYS_NUMBER > MN_DUAL_SYS_3 && MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_3))
    {
        SG_Get_IMEI(MN_DUAL_SYS_3, imei_ptr, imei_len_ptr);
        SG_Get_IMSI(MN_DUAL_SYS_3, imsi_ptr, imsi_len_ptr);
    }
#endif
#if defined MULTI_SIM_SYS_QUAD
    else if (MN_SYS_NUMBER > MN_DUAL_SYS_4 && MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_4))
    {
        SG_Get_IMEI(MN_DUAL_SYS_4, imei_ptr, imei_len_ptr);
        SG_Get_IMSI(MN_DUAL_SYS_4, imsi_ptr, imsi_len_ptr);
    }
#endif    
	/*lint -restore*/
	/*lint -restore*/
	
    if (imei_ptr[0] == 0)
	{
        SCI_STRCPY(imei_ptr, (char*)"0");
	}
    if (imsi_ptr[0] == 0)
	{
        SCI_STRCPY(imsi_ptr, (char*)"0");
	}
}

/*****************************************************************************/
//  Description : 创建网络等待定时器
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGSetNetWaitingTimer(void)
{
    if (s_lib_timer_id != 0)
    {
        MMK_StopTimer(s_lib_timer_id);
    }
    s_lib_timer_id = MMK_CreateWinTimer(SG_WIN_ID_UPDATE_WATCHING, NET_WAITING_TIMER_DURATION, TRUE);
}
#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 下载皮肤配置文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_download_skinbin(void)
{
    SG_getDataInputStruct sg_getDataInput = {0};
    SG_getDataOutputStruct sg_getDataOutput = {0};
	uint16 sg_skinbin_filename[125]={0};

	SCI_MEMSET (sg_skinbin_filename,0,sizeof(sg_skinbin_filename));
	SCI_TRACE_LOW("SG_download_skinbin g_sg_save_position = %d", g_sg_save_position);
	if(0 == g_sg_save_position)
	{
		SCI_MEMCPY(sg_skinbin_filename,SG_D_SKINBIN_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_D_SKINBIN_FILENAME));
	}
	else if(1 == g_sg_save_position)
	{
		SCI_MEMCPY(sg_skinbin_filename,SG_E_SKINBIN_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_E_SKINBIN_FILENAME));
	}
	else
	{
		return;
	}

    SG_InitParamBeforeUpdate();
    SCI_MEMSET(&sg_getDataInput,0,sizeof(sg_getDataInput));
    SCI_MEMSET(&sg_getDataOutput,0,sizeof(sg_getDataOutput));
    sg_getDataInput.type = SG_HTTP_GET_SKIN_BIN;
    sg_getDataInput.urldata = (SG_INT8 *)"";
    sg_getDataInput.destfileName = (SG_UINT16 *)sg_skinbin_filename;
#ifndef WIN32
    SG_InterNet_Get_Data(&sg_getDataInput,&sg_getDataOutput);
#endif

    SGSetNetWaitingTimer();
}
#endif
/*****************************************************************************/
//  Description : 更新超时
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpDateTimeout(void)
{
    SG_getDataOutputStruct sg_getDataOutput = {0};
#ifndef WIN32
    SG_getDataInputStruct sg_getDataInput = {0};
    SG_InterNet_Get_Data(&sg_getDataInput,&sg_getDataOutput);
#endif
	//SOGOU_TRACE_OUT:"SG_UPDATE_TIMEROUT sg_getDataOutput.errorCode=%d,sg_getDataOutput.httpState=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_512_112_2_18_2_29_33_309,(uint8*)"dd",sg_getDataOutput.errorCode,sg_getDataOutput.httpState);
    if(sg_getDataOutput.errorCode == SG_HTTP_OK)
    {
		//SOGOU_TRACE_OUT:"SG_UPDATE_TIMEROUT SG_HTTP_OK"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_515_112_2_18_2_29_33_310,(uint8*)"");
        switch (sg_getDataOutput.httpState)
        {
        case SG_HTTP_STATE_IDLE:
			//SOGOU_TRACE_OUT:"SG_UPDATE_TIMEROUT SG_HTTP_STATE_IDLE"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_519_112_2_18_2_29_33_311,(uint8*)"");
            if (s_lib_timer_id != 0)
            {
                MMK_StopTimer(s_lib_timer_id);
                s_lib_timer_id = 0;
            }
#ifndef WIN32
            SG_InterNet_Exit();
#endif
            if (MMIAPIIM_SGGetUpdateType() == SG_UPDATE_TYPE_VOCABULARY)
            {
                SCI_MEMSET(s_display_string, 0, sizeof(s_display_string));
                SCI_MEMCPY(s_display_string, sg_getDataOutput.displayStr, sizeof(sg_getDataOutput.displayStr));
            }
            s_update_status = SG_IM_UPD_S_UNPACKING;
            MMIAPIIM_SGUpdateCallBackFunc();
            break;

        case SG_HTTP_STATE_CONNECT:
            MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_CONNECTING, NULL);
            break;

        case SG_HTTP_STATE_SEND:
            MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_SENDING, NULL);
            break;

        case SG_HTTP_STATE_GET_CONTENT:
            MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_SG_UPD_WAIT_RECEIVING, NULL);
            break;

        default:
            break;
        }
    }
    else
    {
		//SOGOU_TRACE_OUT:"SG_UPDATE_TIMEROUT error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_555_112_2_18_2_29_33_312,(uint8*)"");
        if (s_lib_timer_id != 0)
        {
            MMK_StopTimer(s_lib_timer_id);
            s_lib_timer_id = 0;
        }
#ifndef WIN32
		SG_InterNet_Exit();
#endif
		if (MMK_IsOpenWin(SG_WIN_ID_UPDATE_WATCHING))
        {
        	MMK_SendMsg(SG_WIN_ID_UPDATE_WATCHING, MSG_SG_UPD_WAIT_CONNECTING_TIME_OUT, NULL);
        }
    }
}

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 下载皮肤文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_download_skin_file(void)
{
    SG_getDataInputStruct sg_getDataInput = {0};
    SG_getDataOutputStruct sg_getDataOutput = {0};

    SG_InitParamBeforeUpdate();

    SCI_MEMSET(&sg_getDataInput,  0, sizeof(sg_getDataInput));
    SCI_MEMSET(&sg_getDataOutput, 0, sizeof(sg_getDataOutput));
    sg_getDataInput.type = SG_HTTP_GET_SKIN;
    sg_getDataInput.urldata = (SG_INT8*)MMIAPIIM_SGGetSkinInfPicId();
    sg_getDataInput.destfileName = MMIAPIIM_SGGetDownloadSkinFullPath();
#ifndef WIN32
    SG_InterNet_Get_Data(&sg_getDataInput,&sg_getDataOutput);
#endif

    SGSetNetWaitingTimer();
}
#endif
/*****************************************************************************/
//  Description : 更新词库
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_update_words(void)
{
    SG_getDataInputStruct sg_getDataInput = {0};
    SG_getDataOutputStruct sg_getDataOutput = {0};
	wchar sg_wordlib_filename[125]={0};

    SG_InitParamBeforeUpdate();

    SCI_MEMSET(&sg_getDataInput,0,sizeof(sg_getDataInput));
    SCI_MEMSET(&sg_getDataOutput,0,sizeof(sg_getDataOutput));
	SCI_MEMSET (sg_wordlib_filename,0,sizeof(sg_wordlib_filename));

    sg_getDataInput.type = SG_HTTP_GET_KEY_UPDATE;

	if(0 == g_sg_save_position)
	{
		SCI_MEMCPY(sg_wordlib_filename,SG_D_WORDLIB_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_D_WORDLIB_FILENAME));
	}
	else if (1 == g_sg_save_position)
	{
		SCI_MEMCPY(sg_wordlib_filename,SG_E_WORDLIB_FILENAME,2 * MMIAPICOM_Wstrlen((wchar*)SG_E_WORDLIB_FILENAME));
	}
	else
	{
		return;
	}
	
    if (!MMIAPIFMM_IsFileExist((wchar*)sg_wordlib_filename, MMIAPICOM_Wstrlen((wchar*)sg_wordlib_filename)))
    {
        sg_getDataInput.urldata = (SG_INT8 *)"";//"1271922092000"; //"1285636628000"D:\@sogou\worddata   worddate
    }
    else
    {
        SFS_HANDLE file_handle = NULL;
        SFS_ERROR_E sfs_err = SFS_NO_ERROR;
        uint32 file_size = 0;
        uint32 read_len = 0;
        uint8 buf[SG_WORDDATA_MAX_LENGTH + 1] = { 0 };

        file_handle = SFS_CreateFile((uint16*)sg_wordlib_filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
        sfs_err = SFS_GetFileSize(file_handle,&file_size);
        SCI_MEMSET(buf, 0, sizeof(buf));
        if (SG_WORDDATA_MAX_LENGTH < file_size)
        {
            file_size = SG_WORDDATA_MAX_LENGTH;
        }

        SFS_ReadFile(file_handle, buf, file_size, &read_len, PNULL);
        SFS_CloseFile(file_handle);
        sg_getDataInput.urldata = (SG_INT8*)buf;    /*lint !e733*/
    }

    sg_getDataInput.destfileName = (SG_UINT16 *)L"";

#ifndef WIN32
    SG_InterNet_Get_Data(&sg_getDataInput,&sg_getDataOutput);
#endif

    SGSetNetWaitingTimer();
}

/*****************************************************************************/
//  Description : 获取IMSI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_Get_IMSI(MN_DUAL_SYS_E cardtype,char * pIMSI,uint8 * lenofImsi)
{
    uint8   imsi[MN_MAX_IMSI_ARR_LEN] = {0};
    uint8   imsiStr[(MN_MAX_IMSI_ARR_LEN<<1) + 1] = {0};
    MN_IMSI_T tmpImsi;

#ifdef WIN32
    //tmpImsi = MNSIM_GetImsi();
#else
    tmpImsi = MNSIM_GetImsiEx(cardtype);
#endif

    SCI_MEMCPY(imsi, tmpImsi.imsi_val, MN_MAX_IMSI_ARR_LEN);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi, MN_MAX_IMSI_ARR_LEN << 1, (char*)imsiStr);

    *lenofImsi = SCI_STRLEN((char*)imsiStr);
    SCI_MEMCPY(pIMSI, (char*)imsiStr, *lenofImsi);
}

/*****************************************************************************/
//  Description : 获取IMEI
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_Get_IMEI(MN_DUAL_SYS_E cardtype,char * pIMEI,uint8 * lenofImei)
{
    MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
    uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    uint16 imei_len = 0;

    if (MNNV_GetIMEISVNEx(cardtype, &imeisvn))
    {
        SCI_MEMSET(imei_str,0,sizeof(imei_str));
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);

        imei_len = (uint16)SCI_STRLEN((char *)imei_str) - 1;
        *lenofImei=imei_len;
        SCI_MEMCPY(pIMEI,imei_str+1,imei_len);
    }
    else
    {
        //SOGOU_TRACE_OUT:"SG_Get_IMEI MNNV_GetIMEISVNEx Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_679_112_2_18_2_29_33_313,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : PS消息处理函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleSGPsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	LOCAL uint32 g_sg_gprs_retry_time = 0;

    if(PNULL == msg_ptr)
    {
        return;
    }
	//SOGOU_TRACE_OUT:"HandleSGPsMsg msg_ptr->msg_id=0x%04x, msg_ptr->result=0x%04x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_695_112_2_18_2_29_33_314,(uint8*)"dd",msg_ptr->msg_id,msg_ptr->result);

    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_CNF:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            //SOGOU_TRACE_OUT:"HandleSGPsMsg ProcessGprsActiveCnf succ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_702_112_2_18_2_29_33_315,(uint8*)"");
            s_is_set_network = TRUE;
            if (s_waiting_pdp_timer_id)
            {
                MMK_StopTimer(s_waiting_pdp_timer_id);
                s_waiting_pdp_timer_id = 0;
            }
            if (!MMK_IsOpenWin(SG_WIN_ID_UPDATE_WATCHING))
            {
#ifndef WIN32
				MMIAPIIM_SGUpdateRelease();
#ifdef SOGOU_USE_PDA_SKIN
				MMIAPIIM_SGFreeSkinFileList();
#endif
                s_is_set_network = FALSE;
#endif
            }
            else
            {
                s_sogou_update_net_id = msg_ptr->nsapi;
#ifndef SOGOU_UPDATE_USE_DOMAIN_RESOLVER
                s_is_set_network = TRUE;
                s_server_address = 0;
                s_update_status = SG_IM_UPD_S_CREATING_SOCKET;
                MMIAPIIM_SGUpdateCallBackFunc();
#else
                MMIAPIIM_SGGetSkinServerAdd();
#endif
            }
        }
        else
        {
            //SOGOU_TRACE_OUT:"HandleSGPsMsg g_sg_gprs_retry_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_732_112_2_18_2_29_33_316,(uint8*)"d", g_sg_gprs_retry_time);
            if (g_sg_gprs_retry_time >= 5)//失败重试的次数为 5次
            {
                g_sg_gprs_retry_time = 0;
                MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, MSG_SG_UPD_WAIT_SET_NETWORK_FALED, NULL);
            }
            else
            {
                g_sg_gprs_retry_time++;
                SG_ActiveNetGprsAndGetAddress();
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 获取皮肤服务器地址
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGGetSkinServerAdd(void)
{
    struct sci_hostent* res;
    uint8 addr[28] = {0};
    char server_address[24] = { 0 };

    s_getserver_delay_timer_id = 0;

    res = sci_gethostbyname((char*)SG_SKIN_SERVER_URL);
    // 解析成功
    if (NULL != res)
    {
		//SOGOU_TRACE_OUT:"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: resolve successful"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_767_112_2_18_2_29_33_317,(uint8*)"");
         // 查询得到结果，跳出循环
        //SCI_ASSERT( 4 == res->h_length ); /*assert verified*/
        SCI_MEMSET(addr,0,sizeof(addr));
        SCI_MEMCPY( addr, res->h_addr_list[0], res->h_length );
        SCI_MEMSET(server_address, 0, sizeof(server_address));//0x3D87B506 //"61.135.181.6"
        sprintf(server_address, "%02x%02x%02x%02x", (uint8) * addr,
                (uint8) * (addr + 1), (uint8) * (addr + 2), (uint8) * (addr + 3));
        s_server_address = strtol(server_address, 0, 16);
        s_is_set_network = FALSE;
        s_update_status = SG_IM_UPD_S_CREATING_SOCKET;
        MMIAPIIM_SGUpdateCallBackFunc();
    }
    // 解析超时
    else if (s_get_server_retry_time > GETSERVER_MAX_RETRY_TIMES)
    {
       	//SOGOU_TRACE_OUT:"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: timeout"
       	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_783_112_2_18_2_29_34_318,(uint8*)"");
        s_is_set_network = FALSE;
	    s_get_server_retry_time = 0;
	    s_is_set_network = FALSE;
        MMIAPIPDP_Deactive(MMI_MODULE_IM);
        MMK_SendMsg(SG_WIN_ID_UPDATE_WATCHING, MSG_SG_UPD_WAIT_SET_NETWORK_FALED, NULL);
    }
    // 再次开启定时器等待
    else
    {
        s_get_server_retry_time++;
        s_getserver_delay_timer_id = MMK_CreateWinTimer(SG_WIN_ID_UPDATE_WATCHING, 1000, FALSE);
        //SOGOU_TRACE_OUT:"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: retry times = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_795_112_2_18_2_29_34_319,(uint8*)"d", s_get_server_retry_time);
    }
}

/*****************************************************************************/
//  Description : 激活GPRS并获取地址
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SG_ActiveNetGprsAndGetAddress(void)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    uint16 sim_sys = MN_DUAL_SYS_1;
    MMI_WIN_ID_T    alert_win_id = MMIPHONE_ALERT_SIM_NOT_OK_WIN_ID;
    uint8 setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

    if (MMIAPISET_GetMultiSysSettingNum(&sim_sys, 1) > 0)
    {
        if (MN_DUAL_SYS_MAX == s_sim_select)
        {
            s_sim_select = (MN_DUAL_SYS_E)sim_sys;
        }
		SCI_TRACE_LOW("[MMIIM] SG_ActiveNetGprsAndGetAddress s_sim_select=%d",s_sim_select);
#ifdef BROWSER_SUPPORT		
		setting_index = MMIAPIBROWSER_GetNetSettingIndex(s_sim_select);    
#endif
		SCI_TRACE_LOW("[MMIIM] SG_ActiveNetGprsAndGetAddress setting_index=%d",setting_index);
		setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(s_sim_select,setting_index);
		SCI_TRACE_LOW("[MMIIM] SG_ActiveNetGprsAndGetAddress setting_item_ptr=0x%04x",setting_item_ptr);

        app_info.app_handler = MMI_MODULE_IM;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;//"cmnet";
        app_info.user_name_ptr = (char*)setting_item_ptr->username;//"";
        app_info.psw_ptr = (char*)setting_item_ptr->password;//"";
        app_info.dual_sys = s_sim_select;
        app_info.priority = 3;
        app_info.handle_msg_callback = HandleSGPsMsg;
        app_info.ps_service_type = IM_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
        MMIAPIPDP_Active(&app_info);
        s_waiting_pdp_timer_id = MMK_CreateWinTimer(SG_WIN_ID_UPDATE_WATCHING, PDP_WAITING_TIMER_DURATION, FALSE);
        SCI_TRACE_LOW("[MMIIM] SG_ActiveNetGprsAndGetAddress active pdp context apn = %s",  setting_item_ptr->apn);
		if(0==setting_item_ptr->gateway_len)//6_12_3
			g_sg_IsWap=FALSE;//6_12_3
		else//6_12_3
			g_sg_IsWap=TRUE;//6_12_3
        SCI_TRACE_LOW("[MMIIM] SG_ActiveNetGprsAndGetAddress g_sg_IsWap = %d",g_sg_IsWap);//6_12_3
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_INSERT_SIM_AND_INFO, TXT_NULL, IMAGE_PUBWIN_WARNING,&alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
    }
}

/*****************************************************************************/
//  Description : 设置网络更新状态
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_E status)
{
    s_update_status = status;
}

/*****************************************************************************/
//  Description : 获取TIMER id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_SGGetLibTimerId(void)
{
    return s_lib_timer_id;
}

/*****************************************************************************/
//  Description : 获取PDP激活TIMER id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_SGPdpTimerId(void)
{
    return s_waiting_pdp_timer_id;
}

/*****************************************************************************/
//  Description : 设置当前选择的sim id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetSelectedSIM(MN_DUAL_SYS_E sim_id)
{
	s_sim_select = sim_id;
}

/*****************************************************************************/
//  Description : 获取服务器地址
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIM_SGGetServerAddress(void)
{
	return s_server_address;
}

/*****************************************************************************/
//  Description : 是否已经设置网络成功
//  Global resource dependence : s_is_set_network
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetIsSetNetwork(void)
{
    return s_is_set_network;
}

/*****************************************************************************/
//  Description : 获取服务器延时定时器ID
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_GetServerDelayTimerId(void)
{
    return s_getserver_delay_timer_id;
}

/*****************************************************************************/
//  Description : 获取显示字符串
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16* MMIAPIIM_GetDisplayStr(void)
{
    return &s_display_string[0];
}

/*****************************************************************************/
//  Description : 获取使用的PDP id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIM_SGGetNetId(void)
{
    return s_sogou_update_net_id;
}
#endif // #if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#endif // #ifndef WDP_UPDATE_C
