/****************************************************************************
** File Name:      mmicdma_app.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 tlt, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef  _MMI_ZDT_APP_C_
#define _MMI_ZDT_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "zdt_app.h"
#include "mmicc_export.h"

MMI_APPLICATION_T   		g_zdt_app;

static BOOLEAN s_zdt_app_init = FALSE;
BOOLEAN is_zdt_jt_mode = FALSE;
static uint8 g_zdt_check_timer = 0;
static uint16 s_zdt_app_cur_day = 0;
static uint8 s_zdt_app_cur_hour = 0xff;
static uint8  g_zdt_flymode_reset_timer = 0;

#ifdef ZDT_WIFI_SUPPORT

LOCAL uint8 g_wifi_app_loc_once_result = 0;
LOCAL uint8 g_wifi_pwon_loc_once_timer = 0;
LOCAL BOOLEAN g_wifi_app_loc_once_start = FALSE;
BOOLEAN g_wifi_loc_test_start = FALSE;
MMIZDTWIFI_LINK_NODE_T * mmi_zdt_wifi_link_head = NULL;

extern void ZDT_YXTrace(const char *pszFmt, ...);
extern void ZDT_Trace(const char *pszFmt, ...);

#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API, 20231204
BOOLEAN zdt_is_get_wifi_data_by_net(void);
#endif

static BOOLEAN MMIZDTWIFI_Link_AddData(MMIZDTWIFI_LINK_DATA_T * pData)
{
    MMIZDTWIFI_LINK_NODE_T  * p1 = NULL;
    MMIZDTWIFI_LINK_NODE_T  * p2 = NULL;
    if(pData ==  NULL)
    {
        return FALSE;
    }
    
    p1=(MMIZDTWIFI_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(MMIZDTWIFI_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(MMIZDTWIFI_LINK_NODE_T));

    p1->data = *pData;
    p1->next_ptr = NULL;
    
    if(mmi_zdt_wifi_link_head == NULL)
    {
        mmi_zdt_wifi_link_head = p1;
    }
    else
    {
        p2 = mmi_zdt_wifi_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  MMIZDTWIFI_Link_Get(MMIZDTWIFI_LINK_DATA_T * pUDPData)
{
    MMIZDTWIFI_LINK_NODE_T  * p1 = NULL;
    
    p1 = mmi_zdt_wifi_link_head;
    
    if(p1 != NULL)
    {
      *pUDPData = p1->data;
        mmi_zdt_wifi_link_head = p1->next_ptr;
        SCI_FREE(p1);
        return TRUE;
    }
    return FALSE;
}

static BOOLEAN  MMIZDTWIFI_Link_DelAll(void)
{
    MMIZDTWIFI_LINK_NODE_T  * p1 = NULL;
    
    while(mmi_zdt_wifi_link_head != NULL)
    {
        p1 = mmi_zdt_wifi_link_head;
        mmi_zdt_wifi_link_head = p1->next_ptr;
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint16  MMIZDTWIFI_Link_Count(void)
{
    MMIZDTWIFI_LINK_NODE_T  * p1 = NULL;
    uint16 num = 0;
    
    p1 = mmi_zdt_wifi_link_head;
    
    while(p1 != NULL)
    {
        num++;
        p1 = p1->next_ptr;
    }
    
    return num;
}
LOCAL void MMIZDTWIFI_HandlePwOnLocOnceTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(timer_id == g_wifi_pwon_loc_once_timer && 0 != g_wifi_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_wifi_pwon_loc_once_timer);
        g_wifi_pwon_loc_once_timer = 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
}

LOCAL void MMIZDTWIFI_StartPwOnLocOnceTimer(uint32 time_ms)
{
    if(0 != g_wifi_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_wifi_pwon_loc_once_timer);
        g_wifi_pwon_loc_once_timer = 0;
    }
    
    g_wifi_pwon_loc_once_timer = MMK_CreateTimerCallback(time_ms, 
                                                                        MMIZDTWIFI_HandlePwOnLocOnceTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

PUBLIC void MMIZDTWIFI_ClosePwOnLocOnceTimer(void)
{
    if(0 != g_wifi_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_wifi_pwon_loc_once_timer);
        g_wifi_pwon_loc_once_timer = 0;
    }
}

LOCAL int MMIZDTWIFI_API_LocCallback(void)
{
    uint8 is_ok = 0;
    MMIZDTWIFI_LINK_DATA_T cur_data = {0};
    
#ifdef ZDT_PLAT_YX_SUPPORT_CY
    if(ZDT_WIFI_Get_DataNum() >= 3)
#else
    if(ZDT_WIFI_Get_DataNum() > 1)
#endif
    {
        is_ok = is_ok | 0x02;
    }

    ZDT_LOG("MMIZDTWIFI_API_LocCallback is_ok=%d,wifi_num=%d",is_ok,ZDT_WIFI_Get_DataNum());
    while(MMIZDTWIFI_Link_Get(&cur_data))
    {
        if(cur_data.rcv_handle != NULL)
        {
            cur_data.rcv_handle(is_ok,PNULL);
        }
    }
    return 0;
}

static void  MMIWIFI_HanldeScanOK(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    ZDT_LOG("MMIWIFI_HanldeScanOK");
    MMIZDTWIFI_ClosePwOnLocOnceTimer();
    MMIZDT_WIFI_OnOff(0);
    MMIZDTWIFI_API_LocCallback();
    if(p_getdata != PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
    g_wifi_app_loc_once_result = 1;
    if(g_wifi_app_loc_once_start)
    {
        g_wifi_app_loc_once_start = FALSE;
    }
    if(g_wifi_loc_test_start)
    {
        ZDT_LOG("MMIWIFI_HanldeScan %d",ZDT_WIFI_Get_DataNum());
        g_wifi_loc_test_start = FALSE;
    }
    
}

static void  MMIWIFI_HanldeScanFail(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    
    ZDT_LOG("MMIWIFI_HanldeScanFail");

    #if defined(ZDT_W217_FACTORY_WIFI)// wuxx_20231224  wifi
    mmi_re_get_wifi_list_delay_StopTimer(); // wuxx 20231224
    #endif
    
    MMIZDTWIFI_ClosePwOnLocOnceTimer();
    MMIZDT_WIFI_OnOff(0);
    MMIZDTWIFI_API_LocCallback();
    if(g_wifi_app_loc_once_start)
    {
        g_wifi_app_loc_once_start = FALSE;
    }
    if(g_wifi_loc_test_start)
    {
        g_wifi_loc_test_start = FALSE;
    }
    if(p_getdata != PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
    
}

PUBLIC BOOLEAN MMIZDTWIFI_PowerOn_LocOnce(void)
{  
    SCI_TraceLow("wuxx, MMIZDTWIFI_PowerOn_LocOnce, 01");
    if(g_wifi_app_loc_once_start == FALSE)
    {
#if defined(ZDT_W217_FACTORY_GPS)// wuxx_20231219  gps
        //CR 启动前,清空先前的GPS 数据
        MMIZDTGPS_ClearGpsData();
#endif
        SCI_TraceLow("wuxx, MMIZDTWIFI_PowerOn_LocOnce, 02");
        MMIZDT_WIFI_OnOff(1);
        g_wifi_app_loc_once_result = 0;
        g_wifi_app_loc_once_start = TRUE;
        MMIZDTWIFI_StartPwOnLocOnceTimer(25000);
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN MMIZDTWIFI_PowerOff_LocOnce(void)
{  
    SCI_TraceLow("wuxx, MMIZDTWIFI_PowerOff_LocOnce, 01");
    if(g_wifi_app_loc_once_start)
    {
        SCI_TraceLow("wuxx, MMIZDTWIFI_PowerOff_LocOnce, 02");
        MMIZDTWIFI_ClosePwOnLocOnceTimer();
        g_wifi_app_loc_once_start = FALSE;
        MMIZDT_WIFI_OnOff(0);
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN MMIZDTWIFI_IsLocOnceStart(void)
{
    return g_wifi_app_loc_once_start;
}

BOOLEAN MMIZDTWIFI_API_LocStart(uint32 time_s,MMIZDTWIFIHANDLER loc_callback)
{
    MMIZDTWIFI_LINK_DATA_T cur_data = {0};
    uint32 time_ms = time_s*1000;
    ZDT_LOG("MMIZDTWIFI_API_LocStart time_s=%ld",time_s);
    cur_data.timeout = time_s;
    cur_data.rcv_handle = loc_callback;
    SCI_TraceLow("wuxx, MMIZDTWIFI_API_LocStart, 01");
    if(MMIZDTWIFI_Link_AddData(&cur_data))
    {
        #ifdef WIFI_SUPPORT
            #ifdef BLUETOOTH_SUPPORT
            if(BT_GetState())
            {
                MMIZDTWIFI_API_LocCallback();
            }
            else
            {
                MMIZDTWIFI_PowerOn_LocOnce();
            }
            #else
            MMIZDTWIFI_PowerOn_LocOnce();
            #endif
        #else
            MMIZDTWIFI_API_LocCallback();
        #endif
        return TRUE;
    }
  
    return FALSE;
}

BOOLEAN MMIZDTWIFI_API_LocStop(void)
{
    if(MMIZDTWIFI_PowerOff_LocOnce())
    {
        //MMIZDTWIFI_API_LocCallback();
    }
    MMIZDTWIFI_Link_DelAll();
    return TRUE;
}
#endif

#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
extern BLOCK_ID ZDTGPS_TaskGetID(void);
LOCAL void MMIZDTGPS_HandleInterval(void);
LOCAL int MMIZDTGPS_API_LocCallback(uint8 is_ok,GPS_DATA_T * pos_ptr );
PUBLIC void MMIZDTGPS_ClosePwOnLocOnceTimer(void);

LOCAL BOOLEAN g_gps_app_loc_need = FALSE;

LOCAL uint8 g_gps_interval_timer = 0;
LOCAL uint8 g_gps_pwon_loc_once_timer = 0;
LOCAL BOOLEAN g_gps_app_is_interval_on = FALSE;
LOCAL uint16 g_gps_app_interval_time_min = 0;
LOCAL BOOLEAN g_gps_app_loc_once_start = FALSE;
LOCAL uint8 g_gps_app_loc_once_result = 0;

MMIZDTGPS_LINK_NODE_T * mmi_zdt_gps_link_head = NULL;

#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
///////////////////////////////////////////////////////////////////////////////////////////
uint32 g_gps_hw_loc_ok_time = 0;
uint8        g_gps_data_valid_cnt = 0;


GPS_WORK_PARA_T gps_work_para = {0};
static GPS_DATA_T s_gps_last_valid_data = {0};
static GPS_DATA_T s_gps_cur_data = {0};
static GPS_GGA_DATA_T s_gga_data = {0};
static GPS_GSA_DATA_T s_gsa_data = {0};
static GPS_GSV_DATA_T s_gpgsv_data = {0};
static GPS_GSV_DATA_T s_bdgsv_data = {0};
static GPS_GSV_DATA_T s_bdgsv_cur_data = {0};
static GPS_GSV_DATA_T s_gpgsv_cur_data = {0};
static GPS_RMC_DATA_T s_rmc_data = {0};
static GPS_DATA_T          s_gps_data[GPS_DATA_VALID_SUM] = {0};
static   uint8        s_gps_data_cnt = 0;
#ifdef GPS_SET_SYSTEM_TIME
static   BOOLEAN s_gps_is_systime_set = FALSE;
#endif
static   BOOLEAN s_gps_is_in_room = FALSE;

static int s_gpgsv_max_snr = 0;
static int s_gpgsv_min_snr = 0;
static uint8 s_gpgsv_satellite_dbh_pos_num = 0;
static uint8 s_gpgsv_satellite_dbh_valid_num = 0;

static int s_bdgsv_max_snr = 0;
static int s_bdgsv_min_snr = 0;
static uint8 s_bdgsv_satellite_dbh_pos_num = 0;
static uint8 s_bdgsv_satellite_dbh_valid_num = 0;

///////////////////////////////////////////////////////////////////////////////////////////
#endif

#if 1//def WIN32 // wuxx open for COMPILE ZDT_W217_FACTORY_GPS
BOOLEAN g_gps_eph_is_set = FALSE; // add for compile

void ZDT_GPSTrace(const char *pszFmt, ...)
{
}

BLOCK_ID ZDTGPS_TaskGetID(void)
{
    return P_ATC;
}
int ZDT_GPS_Init(void)
{    
    return 0;
}
uint8 ZDT_GPS_Get_RunMode(void)
{    
    return 0;
}
BOOLEAN ZDT_GPS_Set_RunMode(uint8 mode)
{    
    return TRUE;
}
int ZDT_GPS_PowerOn(void)
{
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOn_for_w217();
#endif
    return 0;
}
int ZDT_GPS_PowerOff(void)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
        MMIZDTGPS_PowerOff_for_w217();
#endif
    return 0;
}
int ZDT_GPS_CheckHW_Start(void)
{    
    return 0;
}
int ZDT_GPS_CheckHW_End(void)
{    
    return 0;
}
BOOLEAN ZDT_GPS_Get_PowerNoOff(void)
{
    return FALSE;
}

int GPS_API_GetLastPos(GPS_DATA_T * last_pos)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    *last_pos = s_gps_last_valid_data;
#endif
    return 1;
}

int GPS_API_ClearLastPos()
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    SCI_MEMSET(&s_gps_last_valid_data, 0, sizeof(GPS_DATA_T));
#endif
    return 1;
}

int GPS_API_GetCurPos(GPS_DATA_T * last_pos)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    *last_pos = s_gps_cur_data;
#endif
    return 1;
}

BOOLEAN GPS_API_IsFixed(void)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    if(s_gps_last_valid_data.is_valid)
    {
        return TRUE;
    }
#endif
    return FALSE;
}

int GPS_API_GetCurGSV(GPS_GSV_DATA_T * last_gsv)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    *last_gsv = s_gpgsv_cur_data;
#endif
    return 1;
}

int GPS_API_GetCurGPGSV(GPS_GSV_DATA_T * last_gsv)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    *last_gsv = s_gpgsv_cur_data;
#endif

    return 1;
}

int GPS_API_GetCurBDGSV(GPS_GSV_DATA_T * last_gsv)
{    
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    *last_gsv = s_bdgsv_cur_data;
#endif
    return 1;
}

BOOLEAN gps_hw_is_ok(void) 
{
    return TRUE;
}

BOOLEAN gps_hwic_type_is_td(void) 
{
    return FALSE;
}

BOOLEAN gps_hwic_type_is_ubx(void) 
{
    return FALSE;
}

BOOLEAN gps_hwic_type_is_casic(void) 
{
    return TRUE;
}

BOOLEAN ZDT_GPS_Hw_IsOn(void)
{
    return TRUE;
}

void ZDT_GPS_Set_PowerNoOff(BOOLEAN no_off)
{
    return;
}
#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231129 GPS_NEW_STYLE
uint16 GPS_Data_Get_StrLong(uint8 dir,float Longitude,char * buf)
{
    uint32 d = 0;
    uint32 m = 0;
    char cur_buf[20] = {0};
    uint16 len = 0;
    
    d = Longitude/180000;
    m = (Longitude - d*180000)/1.8;
    
    if(dir == 'E' )
    {
        //东
    	cur_buf[0] = 'E';			
    }
    else
    {
        //西
    	cur_buf[0] = 'W';			
    }
    ////cur_buf[0] = 'E';
    
    //sprintf(&cur_buf[1],"%ld.%05ld",d,m);
    sprintf(&cur_buf[1],"%.6f",Longitude); // wuxx modify
    len = strlen(cur_buf);
    SCI_MEMCPY(buf,cur_buf,len);
    return len;
}

uint16 GPS_Data_Get_StrLat(uint8 dir,float Latitude,char * buf)
{
    uint32 d = 0;
    uint32 m = 0;
    char cur_buf[20] = {0};
    uint16 len = 0;
    d = Latitude/180000;
    m = (Latitude - d*180000)/1.8;
    
    if(dir == 'N' )
    {
        //北
    	cur_buf[0] = 'N';			
    }
    else
    {
        //南 
    	cur_buf[0] = 'S';			
    }
    ////cur_buf[0] = 'N';
    
    //sprintf(&cur_buf[1],"%ld.%05ld",d,m);
    sprintf(&cur_buf[1],"%.6f",Latitude); // wuxx modify
    len = strlen(cur_buf);
    SCI_MEMCPY(buf,cur_buf,len);
    return len;
}
#else
uint16 GPS_Data_Get_StrLong(uint8 dir,uint32 Longitude,char * buf)
{
    uint32 d = 0;
    uint32 m = 0;
    char cur_buf[20] = {0};
    uint16 len = 0;
    
    d = Longitude/180000;
    m = (Longitude - d*180000)/1.8;
    
    if(dir == 'E' )
    {
        //东
    	cur_buf[0] = 'E';			
    }
    else
    {
        //西
    	cur_buf[0] = 'W';			
    }
    
    sprintf(&cur_buf[1],"%ld.%05ld",d,m);
    len = strlen(cur_buf);
    SCI_MEMCPY(buf,cur_buf,len);
    return len;
}

uint16 GPS_Data_Get_StrLat(uint8 dir,uint32 Latitude,char * buf)
{
    uint32 d = 0;
    uint32 m = 0;
    char cur_buf[20] = {0};
    uint16 len = 0;
    d = Latitude/180000;
    m = (Latitude - d*180000)/1.8;
    
    if(dir == 'N' )
    {
        //北
    	cur_buf[0] = 'N';			
    }
    else
    {
        //南 
    	cur_buf[0] = 'S';			
    }
    sprintf(&cur_buf[1],"%ld.%05ld",d,m);
    len = strlen(cur_buf);
    SCI_MEMCPY(buf,cur_buf,len);
    return len;
}
#endif

BOOLEAN gps_hwic_type_is_allystar(void) 
{
	return TRUE;
}

#endif
static BOOLEAN MMIZDTGPS_Link_AddData(MMIZDTGPS_LINK_DATA_T * pData)
{
    MMIZDTGPS_LINK_NODE_T  * p1 = NULL;
    MMIZDTGPS_LINK_NODE_T  * p2 = NULL;
    if(pData ==  NULL)
    {
        return FALSE;
    }
    
    p1=(MMIZDTGPS_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(MMIZDTGPS_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(MMIZDTGPS_LINK_NODE_T));

    p1->data = *pData;
    p1->next_ptr = NULL;
    
    if(mmi_zdt_gps_link_head == NULL)
    {
        mmi_zdt_gps_link_head = p1;
    }
    else
    {
        p2 = mmi_zdt_gps_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  MMIZDTGPS_Link_Get(MMIZDTGPS_LINK_DATA_T * pUDPData)
{
    MMIZDTGPS_LINK_NODE_T  * p1 = NULL;
    
    p1 = mmi_zdt_gps_link_head;
    
    if(p1 != NULL)
    {
      *pUDPData = p1->data;
        mmi_zdt_gps_link_head = p1->next_ptr;
        SCI_FREE(p1);
        return TRUE;
    }
    return FALSE;
}

static BOOLEAN  MMIZDTGPS_Link_DelAll(void)
{
    MMIZDTGPS_LINK_NODE_T  * p1 = NULL;
    
    while(mmi_zdt_gps_link_head != NULL)
    {
        p1 = mmi_zdt_gps_link_head;
        mmi_zdt_gps_link_head = p1->next_ptr;
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint16  MMIZDTGPS_Link_Count(void)
{
    MMIZDTGPS_LINK_NODE_T  * p1 = NULL;
    uint16 num = 0;
    
    p1 = mmi_zdt_gps_link_head;
    
    while(p1 != NULL)
    {
        num++;
        p1 = p1->next_ptr;
    }
    
    return num;
}

LOCAL void MMIZDTGPS_HandleIntervalTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(timer_id == g_gps_interval_timer && 0 != g_gps_interval_timer)
    {
        MMK_StopTimer(g_gps_interval_timer);
        g_gps_interval_timer = 0;
    }
    SCI_TraceLow("wuxx, MMIZDTGPS_HandleIntervalTimer,");
    MMIZDTGPS_HandleInterval();
}

LOCAL void MMIZDTGPS_StartIntervalTimer(uint32 time_ms)
{
    if(0 != g_gps_interval_timer)
    {
        MMK_StopTimer(g_gps_interval_timer);
        g_gps_interval_timer = 0;
    }
    g_gps_interval_timer = MMK_CreateTimerCallback(time_ms, 
                                                                        MMIZDTGPS_HandleIntervalTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

PUBLIC void MMIZDTGPS_CloseIntervalTimer(void)
{
    if(0 != g_gps_interval_timer)
    {
        MMK_StopTimer(g_gps_interval_timer);
        g_gps_interval_timer = 0;
    }
}

LOCAL void MMIZDTGPS_LocOnceCallback(uint8 is_ok,GPS_DATA_T * pos_ptr )
{
    ZDT_LOG("MMIZDTGPS_LocOnceCallback is_ok=%d",is_ok);
    g_gps_app_loc_need = FALSE;
    g_gps_app_loc_once_start = FALSE;
#if 0 //ndef ZDT_PLAT_SHB_SUPPORT  // GPS定位不需要处理WIFI
#ifdef ZDT_WIFI_SUPPORT
#ifdef ZDT_PLAT_YX_SUPPORT_CY
    if(ZDT_WIFI_Get_DataNum() >= 3)
#else
    #if defined(ZDT_W217_FACTORY_GPS)// wuxx_20231224  gps
    // 修改逻辑, 先启动WIFI 定位之后,is_ok 不判断是否搜索到WIFI
    // 避免在没有WIFI 的地方, 定位周期变短的问题
    #else
    if(ZDT_WIFI_Get_DataNum() > 1)
    #endif
#endif
    {
        is_ok = is_ok | 0x02;
    }
#endif
#endif
    MMIZDTGPS_API_LocCallback(is_ok,pos_ptr);
#ifdef ZDT_SMS_SUPPORT
    //MMIZDT_GPS_SMS_Send_Loc(is_ok);
#endif
#ifdef ZDT_UDP_SUPPORT
    MMIZDT_GPS_UDP_Send_Loc(is_ok);
#endif

    if(g_gps_app_is_interval_on)
    {
        uint32 time_ms = 0;
        if(g_gps_app_interval_time_min > 1)
        {
            time_ms = g_gps_app_interval_time_min * 60000;
            SCI_TraceLow("wuxx, MMIZDTGPS_LocOnceCallback, g_gps_app_interval_time_min=%d", g_gps_app_interval_time_min);
            SCI_TraceLow("wuxx, MMIZDTGPS_LocOnceCallback, g_gps_app_is_interval_on=%d", g_gps_app_is_interval_on);
            ZDT_GPS_PowerOff();
        }
        else
        {
            time_ms = 60000;
        }
        if(is_ok )
        {
            time_ms -= 10000;
        }
        else
        {
            time_ms -= 30000;
        }
        SCI_TraceLow("wuxx, MMIZDTGPS_LocOnceCallback, time_ms=%d", time_ms);
        MMIZDTGPS_StartIntervalTimer(time_ms);
    }
    else
    {
        SCI_TraceLow("wuxx, MMIZDTGPS_LocOnceCallback, g_gps_app_is_interval_on=%d", g_gps_app_is_interval_on);
        ZDT_GPS_PowerOff();
    }
    return;
}

LOCAL void MMIZDTGPS_HandlePwOnLocOnceTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(timer_id == g_gps_pwon_loc_once_timer && 0 != g_gps_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_gps_pwon_loc_once_timer);
        g_gps_pwon_loc_once_timer = 0;
    }
#ifdef ZDT_PLAT_YX_SUPPORT_CY
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_GPS_POS_OVER,PNULL,0);
#else
    SCI_TraceLow("wuxx, MMIZDTGPS_HandlePwOnLocOnceTimer, ");
    MMIZDTGPS_LocOnceCallback(0,PNULL);
#endif
}

LOCAL void MMIZDTGPS_StartPwOnLocOnceTimer(uint32 time_ms)
{
    if(0 != g_gps_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_gps_pwon_loc_once_timer);
        g_gps_pwon_loc_once_timer = 0;
    }
    SCI_TraceLow("wuxx, MMIZDTGPS_StartPwOnLocOnceTimer, time_ms=%d", time_ms);
    g_gps_pwon_loc_once_timer = MMK_CreateTimerCallback(time_ms, 
                                                                        MMIZDTGPS_HandlePwOnLocOnceTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

PUBLIC void MMIZDTGPS_ClosePwOnLocOnceTimer(void)
{
    if(0 != g_gps_pwon_loc_once_timer)
    {
        MMK_StopTimer(g_gps_pwon_loc_once_timer);
        g_gps_pwon_loc_once_timer = 0;
    }
}

#if defined(ZDT_W217_FACTORY_GPS)// WUXXX_20231213 gps // GPS 定位时,先WIFI , 再GPS, 不能同时启动

//定时器用
uint32 g_get_gps_loc_once_duaration = 0;


#if 1//先获取WIFI，再停止WIFI, 再 使用定时器获取GPS 
static uint8 s_get_gps_data_delay_timer_id = 0;

void mmi_get_gps_data_delay(void)
{
    SCI_TRACE_LOW("TIMER CHECK,mmi_get_gps_data_delay.");

    //停止WIFI 
    ZDT_W217_WIFI_ON_OFF_for_Net(FALSE);

    //启动GPS
    ZDT_GPS_PowerOn(); 
    MMIZDTGPS_StartPwOnLocOnceTimer(g_get_gps_loc_once_duaration);
    
}

BOOLEAN mmi_get_gps_data_delay_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_get_gps_data_delay_timer_id)
    {
        MMK_StopTimer(s_get_gps_data_delay_timer_id);
        s_get_gps_data_delay_timer_id = 0;
    }

    s_get_gps_data_delay_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        mmi_get_gps_data_delay, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN mmi_get_gps_data_delay_StopTimer(void)
{
    if (s_get_gps_data_delay_timer_id != 0)
    {
        MMK_StopTimer(s_get_gps_data_delay_timer_id);
        s_get_gps_data_delay_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif


PUBLIC BOOLEAN MMIZDTGPS_PowerOn_LocOnce(uint32 once_time)
{  
    ////ZDT_GPS_PowerOn(); //先开了WIFI，使用定时器开启GPS //  WUXXX_20231213
#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOn_for_w217();
#endif
    g_gps_app_loc_need = TRUE;
    g_gps_app_loc_once_start = TRUE;
    g_gps_app_loc_once_result = 0;
    SCI_TraceLow("wuxx, MMIZDTGPS_PowerOn_LocOnce, 01, once_time=%d", once_time);
#if 0 //ndef ZDT_PLAT_SHB_SUPPORT  //deleted by bao GPS定位只在WIFI定位失败后开启，不需要重新开启WIFI
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_PowerOn_LocOnce();
#endif
#endif

    //先开了WIFI，使用定时器开启GPS
    g_get_gps_loc_once_duaration = once_time;

    mmi_get_gps_data_delay_StartTimer(500); //15000 关闭WIFI后，GPS可以快速启动 modified by bao

    ////MMIZDTGPS_StartPwOnLocOnceTimer(once_time);    
    return TRUE;
}

PUBLIC BOOLEAN MMIZDTGPS_PowerOff_LocOnce(void)
{  
    mmi_get_gps_data_delay_StopTimer();// WUXXX_20231213

    SCI_TraceLow("wuxx, MMIZDTGPS_PowerOff_LocOnce, 01");

    MMIZDTGPS_ClosePwOnLocOnceTimer();
    ZDT_GPS_PowerOff();
#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOff_for_w217();
#endif
    g_gps_app_loc_need = FALSE;
    g_gps_app_loc_once_start = FALSE;
#ifndef ZDT_PLAT_SHB_SUPPORT
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_PowerOff_LocOnce();
#endif
#endif
    return TRUE;
}

#else
PUBLIC BOOLEAN MMIZDTGPS_PowerOn_LocOnce(uint32 once_time)
{  
    ZDT_GPS_PowerOn();
#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOn_for_w217();
#endif
    g_gps_app_loc_need = TRUE;
    g_gps_app_loc_once_start = TRUE;
    g_gps_app_loc_once_result = 0;
#ifndef ZDT_PLAT_SHB_SUPPORT
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_PowerOn_LocOnce();
#endif
#endif
    MMIZDTGPS_StartPwOnLocOnceTimer(once_time);    
    return TRUE;
}

PUBLIC BOOLEAN MMIZDTGPS_PowerOff_LocOnce(void)
{  
    MMIZDTGPS_ClosePwOnLocOnceTimer();
    ZDT_GPS_PowerOff();
#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOff_for_w217();
#endif
    g_gps_app_loc_need = FALSE;
    g_gps_app_loc_once_start = FALSE;
#ifndef ZDT_PLAT_SHB_SUPPORT
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_PowerOff_LocOnce();
#endif
#endif
    return TRUE;
}
#endif

LOCAL void MMIZDTGPS_HandleInterval(void)
{
    SCI_TraceLow("wuxx, MMIZDTGPS_HandleInterval");
    MMIZDTGPS_PowerOn_LocOnce(60000);
    return;
}

PUBLIC void MMIZDTGPS_PowerOn_Interval(uint16 time_min)
{  
    ZDT_LOG("MMIZDTGPS_PowerOn_Interval time_min=%d",time_min);
    SCI_TraceLow("wuxx, MMIZDTGPS_PowerOn_Interval, time_min=%d", time_min);
    g_gps_app_is_interval_on = TRUE;
    MMIZDTGPS_CloseIntervalTimer();
    MMIZDTGPS_PowerOn_LocOnce(60000);
    g_gps_app_interval_time_min = time_min;
    return;
}

PUBLIC void MMIZDTGPS_PowerOff_Interval(void)
{
    ZDT_LOG("MMIZDTGPS_PowerOff_Interval");
    SCI_TraceLow("wuxx, MMIZDTGPS_PowerOff_Interval");
    MMIZDTGPS_CloseIntervalTimer();
    MMIZDTGPS_PowerOff_LocOnce();
    g_gps_app_is_interval_on = FALSE;
    return;
}

PUBLIC BOOLEAN MMIZDTGPS_IsLocOnceStart(void)
{
    return g_gps_app_loc_once_start;
}

PUBLIC BOOLEAN MMIZDTGPS_IsNeedLoc(void)
{
    return g_gps_app_loc_need;
}

void MMIZDTGPS_PowerOn(void)
{
    ZDT_GPS_PowerOn();
}

void MMIZDTGPS_PowerOff(void)
{
    ZDT_GPS_PowerOff();
}

LOCAL int MMIZDTGPS_API_LocCallback(uint8 is_ok,GPS_DATA_T * pos_ptr )
{
    MMIZDTGPS_LINK_DATA_T cur_data = {0};
    GPS_DATA_T last_gps = {0};
    ZDT_LOG("MMIZDTGPS_API_LocCallback is_ok=%d",is_ok);
    SCI_TraceLow("wuxx, MMIZDTGPS_API_LocCallback");
    if(is_ok && pos_ptr != NULL)
    {
        last_gps = *pos_ptr;
    }
    else
    {
        GPS_API_GetLastPos(&last_gps);
    }
    while(MMIZDTGPS_Link_Get(&cur_data))
    {
        if(cur_data.rcv_handle != NULL)
        {
            SCI_TraceLow("wuxx, MMIZDTGPS_API_LocCallback 02");
            cur_data.rcv_handle(is_ok,&last_gps);
            SCI_TraceLow("wuxx, MMIZDTGPS_API_LocCallback 03");
        }
    }
    return 0;
}

BOOLEAN MMIZDTGPS_API_LocStart(uint32 time_s,MMIZDTGPSHANDLER loc_callback)
{
    MMIZDTGPS_LINK_DATA_T cur_data = {0};
    uint32 time_ms = time_s*1000;
    ZDT_LOG("MMIZDTGPS_API_LocStart time_s=%ld",time_s);
    SCI_TraceLow("wuxx, MMIZDTGPS_API_LocStart, time_s=%ld", time_s);
    cur_data.timeout = time_s;
    cur_data.rcv_handle = loc_callback;
    if(MMIZDTGPS_Link_AddData(&cur_data))
    {
        if(gps_hw_is_ok())
        {
            if(g_gps_app_loc_need == FALSE)
            {
               SCI_TraceLow("wuxx, MMIZDTGPS_API_LocStart, time_ms=%d", time_ms);
                MMIZDTGPS_PowerOn_LocOnce(time_ms);
            }
        }
        else
        {
            SCI_TraceLow("wuxx, MMIZDTGPS_API_LocStart, 02");
            MMIZDTGPS_API_LocCallback(0,PNULL);
        }
        return TRUE;
    }
  
    return FALSE;
}

BOOLEAN MMIZDTGPS_API_LocEnd(void)
{    
    ZDT_LOG("MMIZDTGPS_API_LocEnd");
    SCI_TraceLow("wuxx, MMIZDTGPS_API_LocEnd, 01");
    MMIZDTGPS_ClosePwOnLocOnceTimer();
    MMIZDTGPS_LocOnceCallback(0,PNULL);
    return TRUE;
}


BOOLEAN  ZDTGPS_SendTo_APP(ZDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_ZDT_SIG_T * psig = PNULL;
   SCI_TraceLow("wuxx, ZDTGPS_SendTo_APP, 01");
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDTGPS_SendTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    SCI_TraceLow("wuxx, ZDTGPS_SendTo_APP, 02");
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZDT_SIG_T),ZDTGPS_TaskGetID());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}
static void  MMIGPS_HanldePosOK(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    GPS_DATA_T pos_data = {0};
    #ifndef WIN32
        GPS_API_GetLastPos(&pos_data);
        ZDT_LOG("GPS OK is_valid=%d,sate_num=%d,speed=%ld,cog=%d,hdop=%d",\
            pos_data.is_valid,pos_data.sate_num,pos_data.speed,pos_data.cog,pos_data.hdop);
        ZDT_LOG("GPS OK date=0x%x,time=0x%x,Lat=%ld-%05ld,%c,long=%ld-%05d,%c",\
            pos_data.date,pos_data.time, \
            GPS_Data_Get_D(pos_data.Latitude),GPS_Data_Get_M(pos_data.Latitude),pos_data.Lat_Dir, \
            GPS_Data_Get_D(pos_data.Longitude),GPS_Data_Get_M(pos_data.Longitude),pos_data.Long_Dir);
    #endif
    g_gps_app_loc_once_result = 1;
	
	#ifndef ZDT_PLAT_YX_SUPPORT_CY
    if(g_gps_app_loc_once_start)
    {
        MMIZDTGPS_ClosePwOnLocOnceTimer();
        if(pos_data.is_valid)
        {
            MMIZDTGPS_LocOnceCallback(1,&pos_data);
        }
        else
        {
            MMIZDTGPS_LocOnceCallback(0,&pos_data);
        }
    }
    #endif
    
    if(p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

static void  MMIGPS_HanldePosFail(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    GPS_DATA_T pos_data = {0};
    
    ZDT_LOG("MMIGPS_HanldePosFail");
    
    if(p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}

static void  MMIGPS_HanldePosOver(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    GPS_DATA_T pos_data = {0};
    uint8 is_ok = 0;
    SCI_TraceLow("wuxx, MMIGPS_HanldePosOver, 01");
    if(g_gps_app_loc_once_result)
    {
        is_ok = is_ok | 0x01;
    }
#if 0 //def ZDT_WIFI_SUPPORT  // GPS定位不需要处理WIFI
    if(g_wifi_app_loc_once_result)
    {
        if(ZDT_WIFI_Get_DataNum() > 1)
        {
            is_ok = is_ok | 0x02;
        }
    }
#endif
   SCI_TraceLow("wuxx, MMIGPS_HanldePosOver, 02");
    if(g_gps_app_loc_once_start)
    {
       SCI_TraceLow("wuxx, MMIGPS_HanldePosOver, 03");
        GPS_API_GetLastPos(&pos_data);
        MMIZDTGPS_ClosePwOnLocOnceTimer();
        MMIZDTGPS_LocOnceCallback(is_ok,&pos_data);
    }
    
    if(p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
}
#endif


#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127 // W217 GPS

#ifdef GPS_SUPPORT
#include "gps_drv.h"
//#include "mmigps_api.h"
#endif
#ifdef UAL_GNSS_SUPPORT
#include "ual_gnss.h"
#endif


#ifdef GPS_SUPPORT
LOCAL uint32 g_gnss_handle_for_w217 = 0;
#endif

LOCAL uint8  g_gps_readInfo_timer_id_for_w217   = 0;

#ifdef GPS_SUPPORT
typedef enum
{
    GNSS_SATELLITE_TYPE_GPS,
    GNSS_SATELLITE_TYPE_BEIDOU,
    GNSS_SATELLITE_TYPE_GALIEO,
    GNSS_SATELLITE_TYPE_GLONASS,
    GNSS_SATELLITE_TYPE_MAX
}ENG_GNSS_SATELLITE_TYPE_E;
#endif

#define GNSS_TEST_GPS_SATELLITE_PRN_MIN   1
#define GNSS_TEST_GPS_SATELLITE_PRN_MAX   32
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN   201
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX   264
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MIN   95
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MAX   130
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MIN   65
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MAX   92


///////////////////////////////////////////////////////////////////////////////////////////
static BOOLEAN GPS_Data_OneCycle_End(void)
{
    uint16 i = 0;
    s_gps_cur_data.is_valid = s_rmc_data.is_valid;
    s_gps_cur_data.Latitude = s_rmc_data.Latitude;
    s_gps_cur_data.Lat_m = s_rmc_data.Lat_m;
    s_gps_cur_data.Lat_Dir = s_rmc_data.Lat_Dir;
    s_gps_cur_data.Longitude = s_rmc_data.Longitude;
    s_gps_cur_data.Long_Dir = s_rmc_data.Long_Dir;
    s_gps_cur_data.date = s_rmc_data.date;
    s_gps_cur_data.time = s_rmc_data.time;
    s_gps_cur_data.speed = s_rmc_data.speed;
    s_gps_cur_data.cog = s_rmc_data.cog;
    s_gps_cur_data.hdop = s_gga_data.hdop;
    s_gps_cur_data.sate_num = s_gga_data.sate_num;
    s_gps_cur_data.altitude = s_gga_data.altitude;
    s_gps_cur_data.alt_Dir = s_gga_data.alt_Dir;
    s_gpgsv_cur_data = s_gpgsv_data;
    s_bdgsv_cur_data = s_bdgsv_data;
    if(s_rmc_data.is_valid)
    {
        if(s_gps_is_in_room)
        {
            s_gps_is_in_room = FALSE;
        }

        #if 0
        if(GPS_Data_IsValid(&s_gps_cur_data))
        {
            g_gps_data_valid_cnt++;
            g_gps_hw_loc_ok_time = SCI_GetCurrentTime();
            if(g_gps_mutexPtr != NULL)
            {
                SCI_GetMutex(g_gps_mutexPtr, SCI_WAIT_FOREVER);
            }
            s_gps_last_valid_data = s_gps_cur_data;
            if(g_gps_mutexPtr != NULL)
            {
                SCI_PutMutex(g_gps_mutexPtr);
            }
            s_gps_data[s_gps_data_cnt] = s_gps_cur_data;
            s_gps_data_cnt++;
            if(s_gps_data_cnt >= GPS_DATA_VALID_SUM)
            {
                s_gps_data_cnt = 0;
            }
            if(g_gps_data_valid_cnt >= 3)
            {
                GPS_Data_Valid();
                g_gps_data_valid_cnt = 0;
            }
        }
        #endif
        
        //s_gps_last_valid_data = s_gps_cur_data;
        memcpy(&s_gps_last_valid_data, &s_gps_cur_data, sizeof(GPS_DATA_T));
        
        return TRUE;
    }
    else
    {
        if(s_gps_is_in_room == FALSE)
        {
            s_gps_is_in_room = TRUE;
        }
        
        #if 0 // how to check inroom
        if(GPS_Data_IsValid_InRoom())
        {
            if(s_gps_is_in_room == FALSE)
            {
                s_gps_is_in_room = TRUE;
            }
        }
        else
        {
            if(s_gps_is_in_room)
            {
                s_gps_is_in_room = FALSE;
            }
        }
        #endif
    }
    
    return FALSE;
}







#if 0
///////////////////////////////////////////////////////////////////////////////////////////
uint32 g_gps_hw_loc_ok_time = 0;
uint8        g_gps_data_valid_cnt = 0;


GPS_WORK_PARA_T gps_work_para = {0};
static GPS_DATA_T s_gps_last_valid_data = {0};
static GPS_DATA_T s_gps_cur_data = {0};
static GPS_GGA_DATA_T s_gga_data = {0};
static GPS_GSA_DATA_T s_gsa_data = {0};
static GPS_GSV_DATA_T s_gpgsv_data = {0};
static GPS_GSV_DATA_T s_bdgsv_data = {0};
static GPS_GSV_DATA_T s_bdgsv_cur_data = {0};
static GPS_GSV_DATA_T s_gpgsv_cur_data = {0};
static GPS_RMC_DATA_T s_rmc_data = {0};
static GPS_DATA_T          s_gps_data[GPS_DATA_VALID_SUM] = {0};
static   uint8        s_gps_data_cnt = 0;
#ifdef GPS_SET_SYSTEM_TIME
static   BOOLEAN s_gps_is_systime_set = FALSE;
#endif
static   BOOLEAN s_gps_is_in_room = FALSE;

static int s_gpgsv_max_snr = 0;
static int s_gpgsv_min_snr = 0;
static uint8 s_gpgsv_satellite_dbh_pos_num = 0;
static uint8 s_gpgsv_satellite_dbh_valid_num = 0;

static int s_bdgsv_max_snr = 0;
static int s_bdgsv_min_snr = 0;
static uint8 s_bdgsv_satellite_dbh_pos_num = 0;
static uint8 s_bdgsv_satellite_dbh_valid_num = 0;

///////////////////////////////////////////////////////////////////////////////////////////
#endif

#if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231130 GPS_NEW_STYLE_20231204
// W217 首次定位成功, 再置为 s_yx_loction_is_first_on_for_gps = FALSE
extern BOOLEAN s_yx_loction_is_first_on_for_gps;
#endif


#ifdef UAL_GNSS_SUPPORT
LOCAL void UITestGPS_AppendMutilLINEItemContent_for_w217(MMI_CTRL_ID_T ctrl_id,ual_gnss_read_info_cnf_t*gpsSatelliteInfo)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};

    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    uint8 i = 0;
    uint8 GPS_Num = 0;
    uint8 BD_Num = 0;
    uint8 GALIEO_Num = 0;
    uint8 GLONASS_Num = 0;
    wchar temp_gps[4] = {'G','P','S',0};
    wchar temp_bd[7] = {'B','e','i','d','o','u',0};
    wchar temp_galieo[7] = {'G','a','l','i','e','o',0};
    wchar temp_glonass[8] = {'G','l','o','n','a','s','s',0};
    ENG_GNSS_SATELLITE_TYPE_E  satellite_type = GNSS_SATELLITE_TYPE_MAX;
    wchar wstr_gps_num[50] = {0};
    wchar wstr_bd_num[50] = {0};
    wchar wstr_galieo_num[50] = {0};
    wchar wstr_glonass_num[50] = {0};
    MMI_STRING_T sat_num_str = {0};
    MMI_STRING_T position_ok = {0};
    MMI_STRING_T position_fail = {0};
    uint8 str_position_ok[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_position_fail[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_gps_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_bd_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_galieo_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_glonass_num[GUILIST_STRING_MAX_NUM] = {0};
    wchar wstr_position_result[50] = {0};
    uint8 i_gps=0;
    uint8 i_bd=0;
    uint8 latitude_str_tmp[20] = {0};
    uint8 longitude_str_tmp[20] = {0};
    uint8 latitude_str_ok[20] = {0};
    uint8 longitude_str_ok[20] = {0};
    uint8 latitude_temp_str_test[20] = {0};
    uint8 longitude_temp_str_test[20] = {0};
    char *char_find = PNULL;
    uint8 i_find=0;
    uint8 str_len = 0;
    uint8 latitude_temp_str_check[20] = {0}; // wuxx add for check GPS DATA
    uint8 longitude_temp_str_check[20] = {0};// wuxx add for check GPS DATA
    uint8 trace_float_str_tmp[32] = {0};
    uint8 change_float_str_tmp[32] = {0};
    

    if(PNULL == gpsSatelliteInfo)
    {
        SCI_TRACE_LOW("[W217] [GPS]: AppendSatelliteListItem, gpsSatelliteInfo is null!!!\n");
        return;
    }
    SCI_TRACE_LOW("[W217] [GPS]: AppendSatelliteListItem\n");
    ////////GUILIST_RemoveAllItems(MMIENG_LISTBOX_CTRL_ID);
    ////////GUILIST_SetListState(MMIENG_LISTBOX_CTRL_ID,GUILIST_STATE_FOCUS,FALSE);
    SCI_TRACE_LOW("[W217] [GPS]: AppendSatelliteListItem longitude %f \n",gpsSatelliteInfo->gps_info.longitude);
    SCI_TRACE_LOW("[W217] [GPS]: AppendSatelliteListItem satellite num %d\n",gpsSatelliteInfo->satellite_list_info.satellite_num);

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_info.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    //未定位/已经定位
    if(0 == gpsSatelliteInfo->gps_info.ttff_time)
    {
        SCI_TRACE_LOW("[W217][GPS]no position");
        ////////////////MMI_GetLabelTextByLang(TXT_ENG_NO_POSITION,&position_fail);
        sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
        MMI_STRNTOWSTR(tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        MMIAPICOM_Wstrncat(wstr_position_result,position_fail.wstr_ptr,position_fail.wstr_len);
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;

        ///////////////////////////////////////////////////////////////////////////////////////////////
        //wuxx add. gps data fill.
        s_rmc_data.is_valid = 0;

        SCI_TraceLow("[W217] [GPS]:NO, s_rmc_data.is_valid=%d", s_rmc_data.is_valid);
        ///////////////////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        SCI_TRACE_LOW("[W217][GPS]have positioned");
        ////////////////MMI_GetLabelTextByLang(TXT_ENG_HAVE_POSITIONED,&position_ok);
        sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
        MMI_STRNTOWSTR(tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        MMIAPICOM_Wstrncat(wstr_position_result,position_ok.wstr_ptr,position_ok.wstr_len);
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;

        
        ///////////////////////////////////////////////////////////////////////////////////////////////
        //wuxx add. gps data fill.

        //规避出纬度出现0.0 值的问题, 此处要抓日志提CQ
        sprintf(latitude_temp_str_check, "%.6f", gpsSatelliteInfo->gps_info.latitude);
        sprintf(longitude_temp_str_check, "%.6f", gpsSatelliteInfo->gps_info.longitude);

        //if ((strcmp(latitude_temp_str_check, "0.000000")==0)||(strcmp(longitude_temp_str_check, "0.000000")==0))
        if ((strncmp(latitude_temp_str_check, "0.000000", strlen("0.000000"))==0)||(strncmp(longitude_temp_str_check, "0.000000", strlen("0.000000"))==0))
        {
             // ERR GPS DATA
             SCI_TraceLow("[W217] [GPS]:ERR data,latitude_temp_str_check=%s", latitude_temp_str_check);
             SCI_TraceLow("[W217] [GPS]:ERR data,longitude_temp_str_check=%s", longitude_temp_str_check);
        }
        else
        {

            ////if (gpsSatelliteInfo->gps_info.fix_flag == 1) // gps fixed flag. no need.
            {
                s_rmc_data.is_valid = 1; // 已定位OK

                #if defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231130 GPS_NEW_STYLE_20231204
                // W217 首次定位成功, 再置为 s_yx_loction_is_first_on_for_gps = FALSE
                s_yx_loction_is_first_on_for_gps = FALSE;
                #endif
            }

        }

        SCI_TraceLow("[W217] [GPS]:OK, s_rmc_data.is_valid=%d", s_rmc_data.is_valid);
        SCI_TraceLow("[W217] [GPS]:OK, TTFF, gpsSatelliteInfo->gps_info.ttff_time=%d", gpsSatelliteInfo->gps_info.ttff_time);
        ///////////////////////////////////////////////////////////////////////////////////////////////
    }
    ////////GUILIST_AppendItem(ctrl_id,&item_info);

    //纬度 经度
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    sprintf(tmp_item, "%s:%f  %s:%f", "N", gpsSatelliteInfo->gps_info.latitude,"E", gpsSatelliteInfo->gps_info.longitude);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    ////////GUILIST_AppendItem(ctrl_id,&item_info);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //wuxx add. gps data fill.
    if (s_rmc_data.is_valid == 1)// 已定位OK
    {
         //FLOAT TO UINT32 // wuxx add for OLD GPS DATA  STYLE
         sprintf(latitude_str_tmp, "%.6f", gpsSatelliteInfo->gps_info.latitude);
         sprintf(longitude_str_tmp, "%.6f", gpsSatelliteInfo->gps_info.longitude);

        // longitude表示经度，负数表示“西”，正数表示“东”
        // latitude表示纬度，负数表示“南”，正数表示“北”
         ////////////////////////////
         //怎么判断是N, E
         if (latitude_str_tmp[0]=='-')
         {
             s_rmc_data.Lat_Dir = 'S';
         }
         else
        {
             s_rmc_data.Lat_Dir = 'N';
         }

           if (longitude_str_tmp[0]=='-')
           {
               s_rmc_data.Long_Dir = 'W';
           }
           else
          {
               s_rmc_data.Long_Dir = 'E';
           }
           ///////////////////////////

           //wuxx change it for 

          ///////////////////////////////////////////////////////////
          #if 0
          char_find = strstr(latitude_str_tmp, ".");
          strncpy(latitude_temp_str_test, latitude_str_tmp, (char_find-latitude_str_tmp));
          strcat(latitude_temp_str_test, char_find+1);
          s_rmc_data.Latitude = atoi(latitude_temp_str_test);//gpsSatelliteInfo->gps_info.latitude; //纬度

          char_find = strstr(longitude_str_tmp, ".");
          strncpy(longitude_temp_str_test, longitude_str_tmp, (char_find-longitude_str_tmp));
          strcat(longitude_temp_str_test, char_find+1);
          s_rmc_data.Longitude = atoi(longitude_temp_str_test);//gpsSatelliteInfo->gps_info.latitude; //纬度
          #endif
          ///////////////////////////////////////////////////////////
          s_rmc_data.Latitude = gpsSatelliteInfo->gps_info.latitude; //纬度
          s_rmc_data.Longitude = gpsSatelliteInfo->gps_info.longitude; //经度
          ///////////////////////////////////////////////////////////

          
          ////s_rmc_data.Lat_Dir = 'N';
          ////s_rmc_data.Long_Dir = 'E';

          memset(change_float_str_tmp, 0, sizeof(change_float_str_tmp));
          sprintf(change_float_str_tmp, "%f", gpsSatelliteInfo->gps_info.speed);
          s_rmc_data.speed = atoi(change_float_str_tmp);
          SCI_TraceLow("[W217] [GPS]: change, s_rmc_data.speed=%d", s_rmc_data.speed);

          memset(change_float_str_tmp, 0, sizeof(change_float_str_tmp));
          sprintf(change_float_str_tmp, "%f", gpsSatelliteInfo->gps_info.course);
          s_rmc_data.cog = atoi(change_float_str_tmp);// or  course ? Direction
          SCI_TraceLow("[W217] [GPS]: change, s_rmc_data.cog=%d", s_rmc_data.cog);
          //s_rmc_data.Lat_m = Lat_m;

          ////s_rmc_data.time = gpsSatelliteInfo->gps_info.utc_time.


          memset(change_float_str_tmp, 0, sizeof(change_float_str_tmp));
          sprintf(change_float_str_tmp, "%f", gpsSatelliteInfo->gps_info.HDOP);
          s_gga_data.hdop = atoi(change_float_str_tmp);
          SCI_TraceLow("[W217] [GPS]: change, s_rmc_data.hdop=%d", s_gga_data.hdop);

         s_gga_data.valid= TRUE;

          // SCI_TraceLow 不能打印FLOAT 数据, 日志输出以及速度等值需要重新修改 !!!!!!!!!!
          #if 0
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.longitude=%f", gpsSatelliteInfo->gps_info.longitude);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.latitude=%f", gpsSatelliteInfo->gps_info.latitude);

          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.altitude=%f", gpsSatelliteInfo->gps_info.altitude);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.utc_time=%04d-%02d-%02d,%d:%d:%d", gpsSatelliteInfo->gps_info.utc_time.year,gpsSatelliteInfo->gps_info.utc_time.month,gpsSatelliteInfo->gps_info.utc_time.day,gpsSatelliteInfo->gps_info.utc_time.hour,gpsSatelliteInfo->gps_info.utc_time.minute,gpsSatelliteInfo->gps_info.utc_time.second);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.ttff_time=%d", gpsSatelliteInfo->gps_info.ttff_time);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.satellite_num=%d", gpsSatelliteInfo->gps_info.satellite_num);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.speed=%f", gpsSatelliteInfo->gps_info.speed);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.course=%f", gpsSatelliteInfo->gps_info.course);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.clockdrift=%d", gpsSatelliteInfo->gps_info.clockdrift);
          
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.SystemTimeStamp.second=%d", gpsSatelliteInfo->gps_info.SystemTimeStamp.second);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.SystemTimeStamp.milliseconds=%d", gpsSatelliteInfo->gps_info.SystemTimeStamp.milliseconds);
          
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.m_nUTCTime=%d", gpsSatelliteInfo->gps_info.m_nUTCTime);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.uncertaintySemiMajor=%d", gpsSatelliteInfo->gps_info.uncertaintySemiMajor);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.uncertaintySemiMinor=%d", gpsSatelliteInfo->gps_info.uncertaintySemiMinor);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.Bearing=%f", gpsSatelliteInfo->gps_info.Bearing);

          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.Direction=%d", gpsSatelliteInfo->gps_info.Direction);

          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HorizontalVelocity=%f", gpsSatelliteInfo->gps_info.HorizontalVelocity);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.VerticalVelocity=%f", gpsSatelliteInfo->gps_info.VerticalVelocity);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.fix_flag=%d", gpsSatelliteInfo->gps_info.fix_flag);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HorizontalAccuracy=%f", gpsSatelliteInfo->gps_info.HorizontalAccuracy);
          
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.PDOP=%f", gpsSatelliteInfo->gps_info.PDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HDOP=%f", gpsSatelliteInfo->gps_info.HDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.VDOP=%f", gpsSatelliteInfo->gps_info.VDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.speed=%f", gpsSatelliteInfo->gps_info.speed);



          
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.Latitude=%d", s_rmc_data.Latitude);
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.Longitude=%d", s_rmc_data.Longitude);

          SCI_TraceLow("[W217] [GPS]:s_rmc_data.speed=%d", s_rmc_data.speed);
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.cog=%d", s_rmc_data.cog);

          SCI_TraceLow("[W217] [GPS]:s_gga_data.hdop=%d", s_gga_data.hdop);
          SCI_TraceLow("[W217] [GPS]:s_gga_data.valid=%d", s_gga_data.valid);
          #endif
          // SCI_TraceLow 不能打印FLOAT 数据, 日志输出以及速度等值需要重新修改 !!!!!!!!!!

          
          #if 1// float, str atoi
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.longitude);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.longitude=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.latitude);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.latitude=%s", trace_float_str_tmp);

          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.altitude);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.altitude=%s", trace_float_str_tmp);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.utc_time=%04d-%02d-%02d,%d:%d:%d", gpsSatelliteInfo->gps_info.utc_time.year,gpsSatelliteInfo->gps_info.utc_time.month,gpsSatelliteInfo->gps_info.utc_time.day,gpsSatelliteInfo->gps_info.utc_time.hour,gpsSatelliteInfo->gps_info.utc_time.minute,gpsSatelliteInfo->gps_info.utc_time.second);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.ttff_time=%d", gpsSatelliteInfo->gps_info.ttff_time);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.satellite_num=%d", gpsSatelliteInfo->gps_info.satellite_num);

          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.speed);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.speed=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.course);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.course=%s", trace_float_str_tmp);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.clockdrift=%d", gpsSatelliteInfo->gps_info.clockdrift);
          
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.SystemTimeStamp.second=%d", gpsSatelliteInfo->gps_info.SystemTimeStamp.second);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.SystemTimeStamp.milliseconds=%d", gpsSatelliteInfo->gps_info.SystemTimeStamp.milliseconds);
          
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.m_nUTCTime=%d", gpsSatelliteInfo->gps_info.m_nUTCTime);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.uncertaintySemiMajor=%d", gpsSatelliteInfo->gps_info.uncertaintySemiMajor);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.uncertaintySemiMinor=%d", gpsSatelliteInfo->gps_info.uncertaintySemiMinor);

          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.Bearing);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.Bearing=%s", trace_float_str_tmp);

          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.Direction=%d", gpsSatelliteInfo->gps_info.Direction);

          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.HorizontalVelocity);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HorizontalVelocity=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.VerticalVelocity);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.VerticalVelocity=%s", trace_float_str_tmp);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.fix_flag=%d", gpsSatelliteInfo->gps_info.fix_flag); // fix_flag .
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.HorizontalAccuracy);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HorizontalAccuracy=%s", trace_float_str_tmp);

          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.PDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.PDOP=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.HDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.HDOP=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.VDOP);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.VDOP=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%f",gpsSatelliteInfo->gps_info.speed);
          SCI_TraceLow("[W217] [GPS]:gpsSatelliteInfo->gps_info.speed=%s", trace_float_str_tmp);



          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%.6f",s_rmc_data.Latitude);
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.Latitude=%s", trace_float_str_tmp);
          memset(trace_float_str_tmp, 0, sizeof(trace_float_str_tmp));
          sprintf(trace_float_str_tmp, "%.6f",s_rmc_data.Longitude);
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.Longitude=%s", trace_float_str_tmp);


          SCI_TraceLow("[W217] [GPS]:s_rmc_data.speed=%d", s_rmc_data.speed);
          SCI_TraceLow("[W217] [GPS]:s_rmc_data.cog=%d", s_rmc_data.cog);

          SCI_TraceLow("[W217] [GPS]:s_gga_data.hdop=%d", s_gga_data.hdop);
          SCI_TraceLow("[W217] [GPS]:s_gga_data.valid=%d", s_gga_data.valid);
          #endif


          
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    SCI_MEMSET(&s_gpgsv_data,0,sizeof(GPS_GSV_DATA_T));
    SCI_MEMSET(&s_bdgsv_data,0,sizeof(GPS_GSV_DATA_T));


    //append satellite info
    for(i=0;i<gpsSatelliteInfo->satellite_list_info.satellite_num;i++)
    {
        //GPS星座
        if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GPS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GPS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GPS;
            GPS_Num += 1;
        }
        //北斗星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_BEIDOU;
            BD_Num += 1;
        }
        //Galieo星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GALIEO_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GALIEO_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GALIEO;
            GALIEO_Num += 1;
        }
        //GLONASS星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GLONASS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GLONASS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GLONASS;
            GLONASS_Num += 1;
        }
        // Satellite Identifier  and CN0
        SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
        SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
        switch(satellite_type)
        {
            case GNSS_SATELLITE_TYPE_GPS:
                sprintf(tmp_item, "%s [%d]:%d", "GPS", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_BEIDOU:
                sprintf(tmp_item, "%s [%d]:%d", "Beidou", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GALIEO:
                sprintf(tmp_item, "%s [%d]:%d", "Galieo", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GLONASS:
                sprintf(tmp_item, "%s [%d]:%d", "Glonass", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            default:
                SCI_TRACE_LOW("[W217] [GPS]:other satellite type");
                break;
        }
        MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
        ////////GUILIST_InsertItem(ctrl_id, &item_info,i+6);
        SCI_TRACE_LOW("[W217] [GPS]: AppendSatelliteListItem %s\n",tmp_item);

        
        ///////////////////////////////////////////////////////////////////////////////////////////////
        //wuxx add. gps data fill.
        //if (s_rmc_data.is_valid == 1)// 已定位OK
        {
            ////s_gpgsv_data.satellite_dbh_pos_num = gpsSatelliteInfo->satellite_list_info.satellite_num;

            ////s_gpgsv_data.satellite_dbh_valid_num = gpsSatelliteInfo->satellite_list_info.satellite_num;//gpsSatelliteInfo.satellite_list_info.satellite_info// valid same

           if (satellite_type == GNSS_SATELLITE_TYPE_GPS)
           {
                s_gpgsv_data.rsv[i_gps].is_valid = gpsSatelliteInfo->satellite_list_info.satellite_info[i].is_used;//TRUE;//gpsSatelliteInfo.satellite_list_info.satellite_info[i].

                s_gpgsv_data.rsv[i_gps].sate_id = gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier;

                s_gpgsv_data.rsv[i_gps].elevation = gpsSatelliteInfo->satellite_list_info.satellite_info[i].elevation;

                s_gpgsv_data.rsv[i_gps].azimuth = gpsSatelliteInfo->satellite_list_info.satellite_info[i].azimuth;
                
                s_gpgsv_data.rsv[i_gps].snr = gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0; //

                SCI_TraceLow("[W217] [GPS]: other satellite_type");
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.satellite_dbh_pos_num=%d", s_gpgsv_data.satellite_dbh_pos_num);
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.satellite_dbh_valid_num=%d", s_gpgsv_data.satellite_dbh_valid_num);
                
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.rsv[%d].sate_id=%d", i_gps,s_gpgsv_data.rsv[i_gps].sate_id);
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.rsv[%d].elevation=%d", i_gps,s_gpgsv_data.rsv[i_gps].elevation);
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.rsv[%d].azimuth=%d", i_gps,s_gpgsv_data.rsv[i_gps].azimuth);
                SCI_TraceLow("[W217] [GPS]:s_gpgsv_data.rsv[%d].snr=%d", i_gps,s_gpgsv_data.rsv[i_gps].snr);

                i_gps++;
           }
           else if (satellite_type == GNSS_SATELLITE_TYPE_BEIDOU)
           {
               s_bdgsv_data.rsv[i_bd].is_valid =  gpsSatelliteInfo->satellite_list_info.satellite_info[i].is_used;//TRUE;//gpsSatelliteInfo.satellite_list_info.satellite_info[i].
               
               s_bdgsv_data.rsv[i_bd].sate_id = gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier;
               
               s_bdgsv_data.rsv[i_bd].elevation = gpsSatelliteInfo->satellite_list_info.satellite_info[i].elevation;
               
               s_bdgsv_data.rsv[i_bd].azimuth = gpsSatelliteInfo->satellite_list_info.satellite_info[i].azimuth;
               
               s_bdgsv_data.rsv[i_bd].snr = gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0; //
               
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.satellite_dbh_pos_num=%d", s_bdgsv_data.satellite_dbh_pos_num);
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.satellite_dbh_valid_num=%d", s_bdgsv_data.satellite_dbh_valid_num);
               
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.rsv[%d].sate_id=%d", i_bd,s_bdgsv_data.rsv[i_bd].sate_id);
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.rsv[%d].elevation=%d", i_bd,s_bdgsv_data.rsv[i_bd].elevation);
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.rsv[%d].azimuth=%d", i_bd,s_bdgsv_data.rsv[i_bd].azimuth);
               SCI_TraceLow("[W217] [GPS]:s_bdgsv_data.rsv[%d].snr=%d", i_bd,s_bdgsv_data.rsv[i_bd].snr);

               i_bd++;
           }
           else
           {
               SCI_TraceLow("[W217] [GPS]: other satellite_type");
           }

           
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////
    }

    //memset(&s_bdgsv_data, 0, sizeof(GPS_GSV_DATA_T));

    //memcpy(&s_bdgsv_data, &s_gpgsv_data, sizeof(GPS_GSV_DATA_T));// COPY s_gpgsv_data TO s_bdgsv_data // W217 NO s_bdgsv_data info.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    s_gpgsv_data.satellite_dbh_pos_num = i_gps;

    for(i=0; i<s_gpgsv_data.satellite_dbh_pos_num; i++)
    {
        if (s_gpgsv_data.rsv[i].is_valid != 0)
        {
            s_gpgsv_data.satellite_dbh_valid_num++;
        }
    }
    s_gpgsv_data.sates_in_view = s_gpgsv_data.satellite_dbh_pos_num;

    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////
    s_bdgsv_data.satellite_dbh_pos_num = i_bd;

    for(i=0; i<s_bdgsv_data.satellite_dbh_pos_num; i++)
    {
        if (s_bdgsv_data.rsv[i].is_valid != 0)
        {
            s_bdgsv_data.satellite_dbh_valid_num++;
        }
    }
    s_bdgsv_data.sates_in_view = s_bdgsv_data.satellite_dbh_pos_num;

    
    ///////////////////////////////////////////////////////////////////////////////////////////////



    //GPS卫星数量
    SCI_TRACE_LOW("[W217] [GPS]: GPS satellite num:%d\n",GPS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,temp_gps,MMIAPICOM_Wstrlen(temp_gps));
    ////////////////MMI_GetLabelTextByLang(TXT_ENG_SATELLITE_NUM,&sat_num_str);
    MMIAPICOM_Wstrncat(wstr_gps_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GPS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_gps_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_gps_num;
    ////////GUILIST_InsertItem(ctrl_id, &item_info,2);

    //北斗卫星数量
    SCI_TRACE_LOW("[W217] [GPS]: Beidou satellite num:%d\n",BD_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,temp_bd,MMIAPICOM_Wstrlen(temp_bd));
    MMIAPICOM_Wstrncat(wstr_bd_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", BD_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_bd_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_bd_num;
    ////////GUILIST_InsertItem(ctrl_id, &item_info,3);

    //Galieo卫星数量
    SCI_TRACE_LOW("[W217] [GPS]: Galieo satellite num:%d\n",GALIEO_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,temp_galieo,MMIAPICOM_Wstrlen(temp_galieo));
    MMIAPICOM_Wstrncat(wstr_galieo_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GALIEO_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_galieo_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_galieo_num;
    ////////GUILIST_InsertItem(ctrl_id, &item_info,4);

    //GLONASS卫星数量
    SCI_TRACE_LOW("[W217] [GPS]: Glonass satellite num:%d\n",GLONASS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,temp_glonass,MMIAPICOM_Wstrlen(temp_glonass));
    MMIAPICOM_Wstrncat(wstr_glonass_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GLONASS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_glonass_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_glonass_num;
    ////////GUILIST_InsertItem(ctrl_id, &item_info,5);


    ////////GUIWIN_SetSoftkeyTextId(MMIENG_UITEST_GPS_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
    ////////g_isResponsePassKey = TRUE;
    ////////MMK_PostMsg(MMIENG_UITEST_GPS_WIN_ID,MSG_FULL_PAINT,PNULL,0);

    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //wuxx add. gps data fill.  satellite data
    //if (s_rmc_data.is_valid == 1)// 已定位OK
    {
    
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //wuxx add. gps data fill.

    //定位数据保存在全局
    GPS_Data_OneCycle_End();
    ///////////////////////////////////////////////////////////////////////////////////////////////
}
#endif


LOCAL void Gps_HandleReadInfoReqTimer_for_w217(uint8 timer_id,uint32 param)
{
    //MNGPS_ReadInfoReq(0);
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_read_info();
#endif
    SCI_TRACE_LOW("[W217] [GPS]:readInfoReq in timer\n");
}

LOCAL void Gps_StartReadInfoReqTimer_for_w217(void)
{
    if (0 != g_gps_readInfo_timer_id_for_w217)
    {
        MMK_StopTimer(g_gps_readInfo_timer_id_for_w217);
        g_gps_readInfo_timer_id_for_w217 = 0;
    }
    SCI_TRACE_LOW("[W217] [GPS]:Gps_StartReadInfoReqTimer_for_w217\n");
    g_gps_readInfo_timer_id_for_w217 = MMK_CreateTimerCallback(1000, Gps_HandleReadInfoReqTimer_for_w217, 0,TRUE);
}

LOCAL void Gps_StopReadInfoReqTimer_for_w217(void)
{
    if (0 != g_gps_readInfo_timer_id_for_w217)
    {
        MMK_StopTimer(g_gps_readInfo_timer_id_for_w217);
        g_gps_readInfo_timer_id_for_w217 = 0;
    }

}

#ifdef UAL_GNSS_SUPPORT
LOCAL boolean gnss_callback_for_w217(ual_cms_msg_t param)
{
    boolean  ret    = TRUE;
    uint32   msg_id = 0;
    SCI_TRACE_LOW("[W217 GPS]:gnss_callback_for_w217,Enter\n");
    msg_id = param.msg_id;
    SCI_TRACE_LOW("[W217 GPS]:gnss_callback_for_w217, msg_id:%d\n",msg_id);
    switch(msg_id)
    {
        case MSG_UAL_GNSS_START_CNF:
        {
            //CALL report req
            ual_gnss_start_cnf_t *pStartInfo = (ual_gnss_start_cnf_t *)(param.p_body);
            if (NULL == pStartInfo)
            {
                SCI_TRACE_LOW("[W217] [GPS]gnss_callback_for_w217 APP_MN_GPS_START_CNF,pStartInfo is null \n");
                return FALSE;
            }
            if(TRUE == pStartInfo->is_ok)
            {
                ual_gnss_read_info();   //request gps and location info
                Gps_StartReadInfoReqTimer_for_w217();
                SCI_TRACE_LOW("[W217] [GPS]gnss_callback_for_w217 APP_MN_GPS_START_CNF is OK\n");

            }
            else
            {
                SCI_TRACE_LOW("[W217] [GPS]gnss_callback_for_w217 APP_MN_GPS_START_CNF is NOT OK\n");
                ////UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_START_FAIL);
            }
            break;
        }
        case MSG_UAL_GNSS_READ_INFO_CNF:
        {
            ual_gnss_read_info_cnf_t*  param_ptr = (ual_gnss_read_info_cnf_t*)(param.p_body);
            ual_gnss_read_info_cnf_t   read_gpsinfo = {0};
            int                        i=0;
            char                       test_str[256] = {0};
            SCI_TRACE_LOW("[W217][GPS]:gnss_callback_for_w217 MSG_UAL_GNSS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                SCI_TRACE_LOW("[W217][GPS]:gnss_callback_for_w217 MSG_UAL_GNSS_READ_INFO_CNF  gps_info is null\n");
                ret = FALSE;
                break;
            }
            read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
            read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
            read_gpsinfo.gps_info.latitude = param_ptr->gps_info.latitude;
            read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;
            SCI_TRACE_LOW("[W217][GPS]gnss_callback_for_w217,ttff:%d,longtitude:%d,latitude:%d,altitude:%d",
                read_gpsinfo.gps_info.ttff_time,(int)(read_gpsinfo.gps_info.longitude),
                (int)(read_gpsinfo.gps_info.latitude),(int)(read_gpsinfo.gps_info.altitude));
            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info,
                param_ptr->satellite_list_info.satellite_info,
                sizeof(param_ptr->satellite_list_info.satellite_info));
            SCI_TRACE_LOW("[W217][GPS]gnss_callback_for_w217,read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                SCI_TRACE_LOW("[W217] [GPS]gnss_callback_for_w217,MSG_UAL_GNSS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                        read_gpsinfo.satellite_list_info.satellite_info[i].satellite_identifier,
                        read_gpsinfo.satellite_list_info.satellite_info[i].cn0,
                        read_gpsinfo.satellite_list_info.satellite_info[i].elevation,  param_ptr->satellite_list_info.satellite_info[i].azimuth,
                        read_gpsinfo.satellite_list_info.satellite_info[i].is_used);
            }
            ////UITestGPS_AppendMutilLINEItemContent(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo);
            UITestGPS_AppendMutilLINEItemContent_for_w217(NULL, &read_gpsinfo);
            break;
        }
        case MSG_UAL_GNSS_STOP_CNF:
        {
            ual_gnss_stop_cnf_t* pStopcnf = (ual_gnss_stop_cnf_t*)(param.p_body);
            if(PNULL == pStopcnf)
            {
                SCI_TRACE_LOW("[W217][GPS]:gnss_callback_for_w217, MSG_UAL_GNSS_STOP_CNF  gps_info is null\n");
                ret = FALSE;
                break;
            }
            SCI_TRACE_LOW("[W217][GPS]:gnss_callback_for_w217,MSG_UAL_GNSS_STOP_CNF \n");
            // Gps_StopReadInfoReqTimer_for_w217
            if (0 != g_gps_readInfo_timer_id_for_w217)
            {
                MMK_StopTimer(g_gps_readInfo_timer_id_for_w217);
                g_gps_readInfo_timer_id_for_w217 = 0;
            }

            //power off gps
            //MMIGPS_Close();
            //ual_gnss_poweroff();
            ual_gnss_unregister(g_gnss_handle_for_w217);
            break;
        }
        case MSG_UAL_GNSS_FIX_IND:
        {
            ual_gnss_fix_ind_t* pFixInd = (ual_gnss_fix_ind_t*)(param.p_body);
            if(PNULL == pFixInd)
            {
                SCI_TRACE_LOW("[W217][GPS]:gnss_callback_for_w217, MSG_UAL_GNSS_FIX_IND info is null\n");
                ret = FALSE;
                break;
            }
            //UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_STAT_SUC,pFixInd);
            break;
        }
        default:
            SCI_TRACE_LOW("[W217] [GPS]:gnss_callback_for_w217, Unknow msg id!\n");
            ret = FALSE;
            break;
    }
    SCI_TRACE_LOW("[W217][GPS]:Exit,ret:%d\n",ret);
    return ret;
}
#endif


LOCAL void UAL_GPSStart_for_w217( void )
{
    //MMIGPS_RES_E gps_res = MMIGPS_RES_STATE_WRONG;
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_start_param_t   start_param = {0};
#endif
    SCI_TRACE_LOW("[GPS]UAL_GPSStart_for_w217");
    //init gps first
    //MMIGPS_RegisterCallBack(UITestGPSCallback);
    //gps_res = MMIGPS_Open();
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_set_gnss_mode(UAL_GNSS_MODE_GPS_BDS_GALILEO); //added by bao 默认开启4模定位
    ual_gnss_register(gnss_callback_for_w217,&g_gnss_handle_for_w217);
    start_param.start_mode = 0;
    ual_gnss_start(&start_param);
#endif
    //SCI_TRACE_LOW("[ENG_UITEST] gps init res:%d", gps_res);
}

LOCAL void UAL_GPSStop_for_w217( void )
{
    SCI_TRACE_LOW("[GPS]UAL_GPSStop_for_w217");

    //ZDT_GPS_PowerOff();
    ual_gnss_unregister(g_gnss_handle_for_w217);
    
    #ifdef UAL_GNSS_SUPPORT
    ual_gnss_stop();
    #endif

}

void MMIZDTGPS_PowerOn_for_w217(void)
{
   //第二次启动前,上次数据清空
    GPS_API_ClearLastPos();

   //s_rmc_data 
   memset(&s_rmc_data, 0, sizeof(GPS_RMC_DATA_T));

    //START
    UAL_GPSStart_for_w217();
}

void MMIZDTGPS_PowerOff_for_w217(void)
{
    // STOP
    UAL_GPSStop_for_w217();

    // STOP TIMER
    Gps_StopReadInfoReqTimer_for_w217();
}

// wuxx_20231219  gps
void MMIZDTGPS_ClearGpsData(void)
{
    //第二次启动前,上次数据清空
     GPS_API_ClearLastPos();
    
    //s_rmc_data 
    memset(&s_rmc_data, 0, sizeof(GPS_RMC_DATA_T));

}



#endif// W217 GPS



#ifdef ZDT_GSENSOR_SUPPORT
extern BLOCK_ID ZDTGSensor_TaskGetID(void);
#ifdef WIN32
BLOCK_ID zdtgsensor_taskid;
BLOCK_ID ZDTGSensor_TaskGetID(void)
{
    return zdtgsensor_taskid;
}
int ZDT_GSensor_Init(void)
{    
    return 0;
}
int ZDT_GSensor_Open(void)
{
    return 0;
}
int ZDT_GSensor_Close(void)
{    
    return 0;
}

uint32 ZDT_GSensor_GetStep(void)
{    
    return 123;
}

uint32 ZDT_GSensor_GetStepOneDay(void) 
{
    return 2800;
}

BOOLEAN ZDT_GSensor_GetStill(void)
{    
    return FALSE;
}

BOOLEAN ZDT_GSensor_HW_IsOK(void) 
{
    return TRUE;
}

void ZDT_GSensor_SetStepOneDay(uint32 val) 
{
    return;
}

void ZDT_GSensor_RestoreStepOneDay(uint32 val) 
{
    return;
}

ZDT_GSENSOR_IC_TYPE_E ZDT_GSensor_GetType(void)
{
    return GSENSOR_TYPE_QMA7981;
}
#endif
BOOLEAN  ZDTGSensor_SendTo_APP(ZDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_ZDT_SIG_T * psig = PNULL;
        
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDTGSensor_SendTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZDT_SIG_T),ZDTGSensor_TaskGetID());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}
#endif

#ifdef ZDT_PCBA_ZW31_SUPPORT
extern BOOLEAN  MMIZDT_ZW31_HanldeHallInt(uint32 len);
#endif

LOCAL void MMIZDT_CheckTimerOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(g_zdt_check_timer == timer_id)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_CHECK_TIME,NULL,0);
    }
    return ;
}

LOCAL void MMIZDT_CheckTimerStart(void)
{
    if(0 != g_zdt_check_timer)
    {
        MMK_StopTimer(g_zdt_check_timer);
        g_zdt_check_timer = 0;
    }
    
    g_zdt_check_timer = MMK_CreateTimerCallback(60000, 
                                                                        MMIZDT_CheckTimerOut, 
                                                                        PNULL, 
                                                                        TRUE);
}

PUBLIC void MMIZDT_CheckTimerStop(void)
{
    if(0 != g_zdt_check_timer)
    {
        MMK_StopTimer(g_zdt_check_timer);
        g_zdt_check_timer = 0;
    }
}

PUBLIC BOOLEAN MMIZDT_TimerCheckCalib(void)
{
    uint32 cal_info = 0;
    BOOLEAN is_calibration = FALSE;
#if 0
    is_calibration = ZDT_CheckChipCalibrationed(&cal_info);
    if(is_calibration == FALSE)
    {
#ifdef ZDT_LCD_NOT_SUPPORT
#ifdef ZDT_PLAY_RING_CUSTOM
        MMIAPISET_PlayCustomRing(MMISET_CUST_NO_CALIB,1);
#endif
#endif
    }
#endif
    return is_calibration;
}

LOCAL void MMIZDT_FlyResetCheckTimerOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
#ifdef ZDT_NET_SUPPORT
#ifdef ZDT_PLAT_YX_SUPPORT
    if(FALSE == YX_Net_Is_Land())
    {
        //飞行模式下直接复位网络
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_NET_RESET,PNULL,0);
        //MMIZDT_Net_Reset(TRUE);
    }
#endif
#endif
    return;
}

LOCAL void MMIZDT_FlyResetTimerOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(0 != g_zdt_flymode_reset_timer)
    {
        MMK_StopTimer(g_zdt_flymode_reset_timer);
        g_zdt_flymode_reset_timer = 0;
    }
    g_zdt_flymode_reset_timer = MMK_CreateTimerCallback(30000, 
                                                                        MMIZDT_FlyResetCheckTimerOut, 
                                                                        PNULL, 
                                                                        FALSE);
    return;
}

LOCAL void MMIZDT_FlyResetTimerStart(void)
{
    if(0 != g_zdt_flymode_reset_timer)
    {
        MMK_StopTimer(g_zdt_flymode_reset_timer);
        g_zdt_flymode_reset_timer = 0;
    }
    g_zdt_flymode_reset_timer = MMK_CreateTimerCallback(3000, 
                                                                        MMIZDT_FlyResetTimerOut, 
                                                                        PNULL, 
                                                                        FALSE);
}

PUBLIC void MMIZDT_FlyResetTimerStop(void)
{
    if(0 != g_zdt_flymode_reset_timer)
    {
        MMK_StopTimer(g_zdt_flymode_reset_timer);
        g_zdt_flymode_reset_timer = 0;
    }
}

PUBLIC void MMIZDT_FlyReset_Start(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_FLYMODE_RESET,NULL,0);
}

static BOOLEAN MMIZDT_HandleCheckTimer(void)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    //SCI_TRACE_LOW("[MEM] %d KB-- %d KB U=%d KB",SCI_GetSystemSpaceTotalAvalidMem()/1024, SCI_GetHeapTotalSpace(DYNAMIC_HEAP_APP_MEMORY)/1024,ZDT_File_GetMemFree(FALSE));
    if(s_zdt_app_cur_day == 0)
    {
        s_zdt_app_cur_day = date.mday;
    }
    if(s_zdt_app_cur_day != date.mday)
    {
    #ifdef FILE_LOG_SUPPORT
        //UserFile_RemoveDirFile("TEST");
    #endif
#ifdef ZDT_PLAT_YX_SUPPORT
        YX_API_WT_Reset();
#endif
#ifdef ZDT_GSENSOR_SUPPORT
        //SCI_TRACE_LOW("MMIZDT_HandleCheckTimer :ZDT_GSensor_SetStepOneDay(0);");
        ZDT_GSensor_SetStepOneDay(0);
#endif
        s_zdt_app_cur_day = date.mday;
    }
    
    if(s_zdt_app_cur_hour != time.hour)
    {
#ifdef FOTA_SUPPORT_ABUP
#if defined(ZDT_LCD_NOT_SUPPORT)||defined(ZDT_BH_W9_FEATRUE)
        //MMIAPISET_FotaCheck();
#endif
#endif
        s_zdt_app_cur_hour = time.hour;
    }

		Is_time_silent();
        
#ifdef ZDT_PLAT_YX_SUPPORT
        YX_Net_TimerCheck(date,time);
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
        SHB_Net_TimerCheck(date,time);
#endif

#ifdef ZDT_NET_SUPPORT
        //MMIZDT_Net_Reset_Check();
#endif
    return TRUE;
}


#ifdef ZDT_PLAY_RING_CUSTOM
int MMIZDT_HandleCustRingPlay(DPARAM param)
{
    MMIAPISET_HandleCustomRingStart();
    return 0;
}
#endif


static MMI_RESULT_E  HandleZDTAppMsg (PWND app_ptr, 
								 uint16 msg_id, 
								 DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    #ifdef ZDT_SMS_SUPPORT
    if(MMIZDT_SMS_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }
    #endif
    
    #ifdef ZDT_NET_SUPPORT
    if(MMI_RESULT_TRUE == MMIZDT_Net_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }
    #endif
#ifdef ZDT_PLAT_SHB_SUPPORT
    if(MMI_RESULT_TRUE == SHB_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }
#endif
    switch(msg_id)
    {                       
        case ZDT_APP_SIGNAL_FLYMODE_RESET:
                ZDT_LOG("ZDT_APP_SIGNAL_FLYMODE_RESET");
                MMIZDT_FlyResetTimerStart();
            break;
            
        case ZDT_APP_SIGNAL_CHECK_TIME:
                MMIZDT_HandleCheckTimer();
            break;
			
#ifdef ZDT_PLAY_RING_CUSTOM
        case ZDT_APP_SIGNAL_CUSTRING_PLAY:
                MMIZDT_HandleCustRingPlay(param);
            break;
#endif

#ifdef ZDT_PCBA_ZW31_SUPPORT
        case ZDT_APP_SIGNAL_HALL_INT:
            {
                MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
                MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
                ZDT_LOG("ZDT_APP_SIGNAL_HALL_INT");
                if(p_getdata != PNULL)
                {
                    ZDT_LOG("ZDT_APP_SIGNAL_HALL_INT %d",p_getdata->len);
                    MMIZDT_ZW31_HanldeHallInt(p_getdata->len);
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }
            break;
#endif
#ifdef ZDT_GPS_SUPPORT
        case ZDT_APP_SIGNAL_GPS_POS_OK:
                ZDT_LOG("GPS_APP_SIGNAL_POS_OK");
                MMIGPS_HanldePosOK(param);
            break;
            
        case ZDT_APP_SIGNAL_GPS_POS_FAIL:
                ZDT_LOG("GPS_APP_SIGNAL_POS_FAIL");
                MMIGPS_HanldePosFail(param);
            break;
            
        case ZDT_APP_SIGNAL_GPS_POS_OVER:
                ZDT_LOG("ZDT_APP_SIGNAL_GPS_POS_OVER");
                MMIGPS_HanldePosOver(param);
            break;
#endif

#ifdef ZDT_WIFI_SUPPORT
        case ZDT_APP_SIGNAL_WIFI_SCAN_OK:
                ZDT_LOG("ZDT_APP_SIGNAL_WIFI_SCAN_OK");
                MMIWIFI_HanldeScanOK(param);
            break;
            
        case ZDT_APP_SIGNAL_WIFI_SCAN_FAIL:
                ZDT_LOG("ZDT_APP_SIGNAL_WIFI_SCAN_FAIL");
                MMIWIFI_HanldeScanFail(param);
            break;
#endif

#ifdef ZDT_GSENSOR_SUPPORT
         case ZDT_APP_SIGNAL_GSENSOR_MOVE:
                ZDT_LOG("GSENSOR MOVE");
            break;
            
         case ZDT_APP_SIGNAL_GSENSOR_STILLNESS:
                ZDT_LOG("GSENSOR STILLNESS");
            break;
            
         case ZDT_APP_SIGNAL_GSENSOR_FAIL:
                ZDT_LOG("GSENSOR FAIL");
            break;
            
         case ZDT_APP_SIGNAL_GSENSOR_STEP:
            {
                MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
                MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != PNULL)
                {
                    ZDT_LOG("GSENSOR Step %d",p_getdata->len);
                }
                
                if(p_getdata->len > 0 && p_getdata->str != PNULL)
                {
                   SCI_FREE(p_getdata->str);
                }
            }
                
            break;
#endif            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

extern void MMIZDT_Gsensor_Init(void)
{
#ifdef ZDT_GSENSOR_SUPPORT
    ZDT_GSensor_Init();
    ZDT_GSensor_Open();
#endif
}


extern void MMIZDT_Hsensor_Init(void)
{
#ifdef ZDT_HSENSOR_SUPPORT
    ZDT_HSensor_Init();
#endif
    return;
}

#ifdef ZDT_WIFI_SUPPORT
#define 	ZDT_WIFI_TASK_SIG_START 0xFE01

typedef struct
{
    SIGNAL_VARS
    uint8 *        str;
    uint32         len; 
} ZDT_WIFI_SIG_T;

uint8 s_zdt_wifi_is_open = 0;
uint8 s_zdt_wifi_is_on = 0;
uint8 s_zdt_wifi_status  = 0;
BLOCK_ID m_zdt_wifi_task_id = SCI_INVALID_BLOCK_ID;
uint8 g_zdt_wifi_is_hw_on = 0;


#if 1
ZDT_WIFI_DATA_T g_zdt_wifi_data[ZDT_WIFI_DATA_VALID_SUM] = {0};
ZDT_WIFI_DATA_T g_zdt_wifi_cur_data = {0};
uint8        g_zdt_wifi_data_cnt = 0;
BOOLEAN        g_zdt_wifi_hw_is_ok = FALSE;

static void  ZDT_WIFI_ReBuild_Data(uint8 cnt)
{
    uint8 i;
    uint8 j;
    ZDT_WIFI_DATA_T tmp;
    
    if(cnt == 0)
    {
        return;
    }
    
    for(i=0;i<cnt-1;i++) 
    {
        for(j=i+1;j<cnt;j++)
        {
            if(g_zdt_wifi_data[i].rssi < g_zdt_wifi_data[j].rssi)
            { 
                tmp=g_zdt_wifi_data[i];
                g_zdt_wifi_data[i]=g_zdt_wifi_data[j]; 
                g_zdt_wifi_data[j]=tmp;
            }
        }
    }
    return;
}

static BOOLEAN  ZDT_WIFI_Add_Data(ZDT_WIFI_DATA_T * pData)
{
    if(g_zdt_wifi_data_cnt >= ZDT_WIFI_DATA_VALID_SUM)
    {
        uint8 i; 
        uint8 min_idx = 0;
        int32 min_rssi = g_zdt_wifi_data[0].rssi;
        g_zdt_wifi_data_cnt = ZDT_WIFI_DATA_VALID_SUM;

        #if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API
        // W217 WIFI LIST RSSI API 出来就是排序的,不需要再排序
        #else
        for(i = 1; i < ZDT_WIFI_DATA_VALID_SUM; i++)
        {
            if(g_zdt_wifi_data[i].rssi < min_rssi)
            {
                min_idx=i;
                min_rssi = g_zdt_wifi_data[i].rssi;
            }
        }
        
        if(pData->rssi > min_rssi)
        {
            g_zdt_wifi_data[min_idx] = *pData;
        }
        #endif
    }
    else
    {
        g_zdt_wifi_data[g_zdt_wifi_data_cnt++] = *pData;
    }
    return TRUE;
}

static BOOLEAN  ZDT_WIFI_Clean_Data(void)
{
    g_zdt_wifi_data_cnt = 0; 
    SCI_MEMSET(g_zdt_wifi_data,0,sizeof(ZDT_WIFI_DATA_T)*ZDT_WIFI_DATA_VALID_SUM);
    return TRUE;
}

BOOLEAN  ZDT_WIFI_Get_Data(uint8 idx,ZDT_WIFI_DATA_T *pData)
{
    if(idx >= ZDT_WIFI_DATA_VALID_SUM)
    {
        return FALSE;
    }
    if(g_zdt_wifi_data[idx].idx > 0)
    {
        *pData = g_zdt_wifi_data[idx];
        return TRUE;
    }
    return FALSE;
}
uint8  ZDT_WIFI_Get_DataNum(void)
{
    return g_zdt_wifi_data_cnt;
}

BOOLEAN  ZDT_WIFI_HW_IsOK(void)
{
    return g_zdt_wifi_hw_is_ok;
}
#endif

BOOLEAN  ZDT_APP_SendSigTo_WIFI(uint16 sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_WIFI_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=m_zdt_wifi_task_id;
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCA(data_len+1);//free it in wifi task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDT_APP_SendSigTo_WIFI Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_WIFI_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

void ZDT_WIFI_TaskEntry(uint32 argc, void *argv)
{
    xSignalHeaderRec*   sig_ptr = 0; 
    int ret = 0;
    m_zdt_wifi_task_id = SCI_IdentifyThread();
    WIFISUPP_OnReq(m_zdt_wifi_task_id);
    
    while(1)
    {
        sig_ptr = SCI_GetSignal(m_zdt_wifi_task_id); 
        switch(sig_ptr->SignalCode)
        {
            case ZDT_WIFI_TASK_SIG_START:
                ZDT_LOG("ZDT_WIFI_TASK_SIG_START");
                WIFISUPP_OnReq(m_zdt_wifi_task_id);
                break;
                
            case WIFISUPP_SIG_ON_CNF:
                {
                    WIFISUPP_ON_CNF_T* wifi_on_cnf_ptr= (WIFISUPP_ON_CNF_T*) sig_ptr;
                    WIFISUPP_SCAN_REQ_INFO_T scan_req_info = {0};
                    ZDT_LOG("ZDT WIFISUPP_SIG_ON_CNF");
                    g_zdt_wifi_is_hw_on = TRUE;
                    if(PNULL != wifi_on_cnf_ptr)
                    {
                        //save wifi on cnf param to context
                        if(WIFISUPP_RESULT_SUCC  == wifi_on_cnf_ptr->on_result.wifi_on_result)
                        {
                            ZDT_LOG("ZDT WIFISUPP_SIG_ON_CNF SUCCESS");
                            s_zdt_wifi_status = 1;
                            WIFISUPP_ScanReq(&scan_req_info);
                            if(g_zdt_wifi_hw_is_ok == FALSE)
                            {
                                g_zdt_wifi_hw_is_ok = TRUE;
                            }
                            ZDT_WIFI_Clean_Data();
                        }
                        else
                        {
                            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
                        }
                    }
                    else
                    {
                        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
                    }
                }
                break;
                
            case WIFISUPP_SIG_OFF_CNF:
                    ZDT_LOG("ZDT WIFISUPP_SIG_OFF_CNF");
                    s_zdt_wifi_status = 0;
                    g_zdt_wifi_is_hw_on = FALSE;
                    ZDT_WIFI_ReBuild_Data(g_zdt_wifi_data_cnt);
                    
                    if(g_zdt_wifi_data_cnt > 0)
                    {
                        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_OK,PNULL,0);
                    }
                    else
                    {
                        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
                    }
                break;

            case WIFISUPP_SIG_SCAN_IND:
                {
                    WIFISUPP_SCAN_IND_T* wifi_scan_ind_ptr = (WIFISUPP_SCAN_IND_T*)sig_ptr;
                    
                    ZDT_LOG("ZDT WIFISUPP_SIG_SCAN_IND");
                    if (PNULL == wifi_scan_ind_ptr)
                    {
                        break;
                    }

                    g_zdt_wifi_cur_data.idx = 1;
                    g_zdt_wifi_cur_data.channel = wifi_scan_ind_ptr->ap_info.channel;
                    g_zdt_wifi_cur_data.ssid_len = wifi_scan_ind_ptr->ap_info.ssid.ssid_len;
                    g_zdt_wifi_cur_data.rssi = wifi_scan_ind_ptr->ap_info.signal_qua;
                    g_zdt_wifi_cur_data.security_enabled = wifi_scan_ind_ptr->ap_info.encryp_protocol;
                    g_zdt_wifi_cur_data.bss_type = wifi_scan_ind_ptr->ap_info.network_mode;
                    SCI_MEMCPY(g_zdt_wifi_cur_data.bssid,wifi_scan_ind_ptr->ap_info.bssid_info.bssid,WIFISUPP_BSSID_LEN);
                    SCI_MEMCPY(g_zdt_wifi_cur_data.ssid,wifi_scan_ind_ptr->ap_info.ssid.ssid,WIFISUPP_SSID_MAX_LEN);
                    ZDT_WIFI_Add_Data(&g_zdt_wifi_cur_data);
                    
                    ZDT_LOG("ZDT WIFISUPP_SIG_SCAN_IND idx=%ld,channel=%d,rssi=%d,%d,%02X:%02X:%02X:%02X:%02X:%02X",g_zdt_wifi_cur_data.idx,g_zdt_wifi_cur_data.channel,g_zdt_wifi_cur_data.rssi ,wifi_scan_ind_ptr->ap_info.signal_qua,\
                        wifi_scan_ind_ptr->ap_info.bssid_info.bssid[0],wifi_scan_ind_ptr->ap_info.bssid_info.bssid[1],wifi_scan_ind_ptr->ap_info.bssid_info.bssid[2],
                       wifi_scan_ind_ptr->ap_info.bssid_info.bssid[3],wifi_scan_ind_ptr->ap_info.bssid_info.bssid[4],wifi_scan_ind_ptr->ap_info.bssid_info.bssid[5]);
                    ZDT_LOG("ZDT WIFISUPP_SIG_SCAN_IND ssid_len=%d,%s",wifi_scan_ind_ptr->ap_info.ssid.ssid_len,wifi_scan_ind_ptr->ap_info.ssid.ssid);

                }

                break;

            case WIFISUPP_SIG_SCAN_CNF:
                    ZDT_LOG("ZDT WIFISUPP_SIG_SCAN_CNF Count=%d",g_zdt_wifi_data_cnt);
                    WIFISUPP_OffReq();
                break;

            case WIFISUPP_SIG_CONNECT_CNF:
                    ZDT_LOG("ZDT WIFISUPP_SIG_CONNECT_CNF");
                break;

            case WIFISUPP_SIG_DISCONNECT_CNF:
                    ZDT_LOG("ZDT WIFISUPP_SIG_DISCONNECT_CNF");
                break;

            case WIFISUPP_SIG_DISCONNECT_IND:
                    ZDT_LOG("ZDT WIFISUPP_SIG_DISCONNECT_IND");
                break;

            case WIFISUPP_SIG_BSSID_CHANGED_IND:
                    ZDT_LOG("ZDT WIFISUPP_SIG_BSSID_CHANGED_IND");
                break;

            case WIFISUPP_SIG_RSSI_CHANGED_IND:
                    ZDT_LOG("ZDT WIFISUPP_SIG_RSSI_CHANGED_IND");
                break;
            default:
                break;
        }
        SCI_FREE(sig_ptr);
    }
    
}

static void ZDT_WIFI_TaskClose(void)
{
    ZDT_LOG("ZDT_WIFI_TaskClose, m_zdt_wifi_task_id=0x%x", m_zdt_wifi_task_id);
    #if 0
    if(SCI_INVALID_BLOCK_ID != m_zdt_wifi_task_id)
    {   
        SCI_TerminateThread(m_zdt_wifi_task_id);
        SCI_DeleteThread(m_zdt_wifi_task_id);
        m_zdt_wifi_task_id = SCI_INVALID_BLOCK_ID;
    }
    #endif
}

static BLOCK_ID ZDT_WIFI_TaskCreate(void)
{
    ZDT_LOG("ZDT_WIFI_TaskCreate, taskid=0x%x", m_zdt_wifi_task_id);
    if(m_zdt_wifi_task_id == SCI_INVALID_BLOCK_ID)
    {
        m_zdt_wifi_task_id = SCI_CreateThread("ZDT_WIFI_THREAD_NAME",
                                                                "ZDT_WIFI_QUEUE_NAME",
                                                                ZDT_WIFI_TaskEntry,
                                                                NULL,
                                                                PNULL,
                                                                1024*10,
                                                                50,
                                                                TX_SYSTEM_NORMALE,
                                                                SCI_PREEMPT,
                                                                SCI_AUTO_START
                                                                );
    }
    else
    {
        ZDT_APP_SendSigTo_WIFI(ZDT_WIFI_TASK_SIG_START,PNULL,0);
    }
    return m_zdt_wifi_task_id;
}

LOCAL uint8 MMIZDT_NV_GetWIFIOnOff(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 wifi_onoff = 1;
    //get if is flip answer
    MMINV_READ(MMI_ZDT_NV_WIFI_ONOFF,&wifi_onoff,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        wifi_onoff = 1;
        MMINV_WRITE(MMI_ZDT_NV_WIFI_ONOFF,&wifi_onoff);
    }
    s_zdt_wifi_is_open = wifi_onoff;
    return s_zdt_wifi_is_open;
}

LOCAL BOOLEAN  MMIZDT_NV_SetWIFIOnOff(uint8 val) 
{
    uint8 wifi_onoff = val;
    if(s_zdt_wifi_is_open != wifi_onoff)
    {
        MMINV_WRITE(MMI_ZDT_NV_WIFI_ONOFF,&wifi_onoff);
        s_zdt_wifi_is_open = wifi_onoff;
        return TRUE;
    }
    return FALSE;
}

#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API, 20231017
void ZDT_W217_WIFI_ON_OFF_for_Net(BOOLEAN isOn);
#endif
PUBLIC void MMIZDT_WIFI_OnOff(uint8 val) 
{
    if(s_zdt_wifi_is_on == val)
    {
        return;
    }
    ZDT_LOG("MMIZDT_WIFI_OnOff, val=%d", val);
    if(val)
    {
        #ifdef BLUETOOTH_SUPPORT
        if(BT_GetState())
        {
            return;
        }
        #endif
        
        if(s_zdt_wifi_status == 0)
        {
            g_zdt_wifi_is_hw_on = TRUE;
            
            #if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API, 20231017
            ZDT_W217_WIFI_ON_OFF_for_Net(1);
            #else
            ZDT_WIFI_TaskCreate();
            #endif
        }
    }
    else
    {
        if(s_zdt_wifi_status == 0)
        {
             #if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API, 20231017
            ZDT_W217_WIFI_ON_OFF_for_Net(0); // wuxx,  must 0
            #else
            ZDT_WIFI_TaskClose();
            #endif
        }
    }
    s_zdt_wifi_is_on = val;
    return;
}

PUBLIC uint8 MMIZDT_WIFI_GetOpen(void)
{
    return s_zdt_wifi_is_open;
}

PUBLIC void MMIZDT_WIFI_SetOpen(uint8 val) 
{
    if(MMIZDT_NV_SetWIFIOnOff(val))
    {
        if(val)
        {
            MMIZDT_WIFI_OnOff(1);
        }
        else
        {
            MMIZDT_WIFI_OnOff(0);
        }
    }
    return;
}
#endif


#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API, 20231005
  #ifdef UAL_WIFI_SUPPORT
  #include "Ual_wifi.h"
  #ifdef UAL_RF_AM_SUPPORT
  #include "ual_rf_am.h"
  #endif
  #endif

// w217 WIFI 最大重新搜索次数
#define ZDT_UAL_WIFI_RE_GET_SCANN_TIMES    10


// FAE 参考UiTestWifiWinOpen

//define it in  Mmieng_uitestwin.c
#define UAL_WIFI_AP_NODE_NUMBER 30
#define UAL_WIFI_AP_NODE_BSSID_STR_LEN 25
#define UAL_WIFI_AP_NODE_RSSI_STR_LEN 15

////////////////////////////////////////////////////
static BOOLEAN  ZDT_WIFI_Add_Data(ZDT_WIFI_DATA_T * pData);
static BOOLEAN  ZDT_WIFI_Clean_Data(void);

BOOLEAN  ZDT_WIFI_Clean_Data_Public(void)
{
    ZDT_WIFI_Clean_Data();
}

void ZDT_W217_WIFI_scan_result_for_net(void); // 20231213

uint8 MMIZDTWIFI_Start(void);
void MMIZDTWIFI_Stop(void);


////////////////////////////////////////////////////
//调用 ZdtWatch_Factory_WIFI_ShowData 显示函数, 填充如下数据
// g_zdt_wifi_hw_is_ok
// ZDT_WIFI_Get_DataNum
// ZDT_WIFI_Get_Data

void  ZDT_WIFI_HW_Set_IsOK(BOOLEAN isOK)
{
    g_zdt_wifi_hw_is_ok = isOK;
}

void  ZDT_WIFI_Set_DataNum(uint8 wifi_num)
{
    g_zdt_wifi_data_cnt = wifi_num;
}


//是否网络调用WIFI 接口, TRUE IS NET ,FALSE IS FACTORY TEST.
BOOLEAN g_is_get_wifi_data_by_net = FALSE;
BOOLEAN g_is_get_wifi_data_from_factory = FALSE;

BOOLEAN zdt_is_get_wifi_data_by_net(void)
{
    return g_is_get_wifi_data_by_net;
}

////////////////////////////////////////////////////
LOCAL uint32 g_zdt_ual_wifi_handle = NULL;
LOCAL BOOLEAN g_zdt_is_reopen_wifi_scan = FALSE;

void ZDT_WIFI_Set_Wifi_Reopen_Scan(BOOLEAN is_reopen)
{
    g_zdt_is_reopen_wifi_scan = is_reopen;
}

LOCAL void ZDT_Wifi_StartScan(void)
{
    uint8 i = 0;
    ual_wifi_scan_req_param_t scan_req_param = {0};

    SCI_TRACE_LOW("ZDT_Wifi_StartScan, ENTER");
    scan_req_param.scan_mode = UAL_WIFI_SCAN_BY_CHANNEL;
    scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nChNum = UAL_WIFI_CHANNEL_NUMBER;
    scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nMaxNodeNum = UAL_WIFI_AP_NODE_NUMBER;
    for(i = 0;i < UAL_WIFI_CHANNEL_NUMBER;i++)
    {
        scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nch[i] = i+1;
    }
    ual_wifi_start_scan(&scan_req_param);
    return;
}

LOCAL boolean MMIZDT_HandleUalWifimsg(ual_cms_msg_t param)
{
    ual_wifi_msg_e ual_wifi_msg = (ual_wifi_msg_e)param.msg_id;
    ual_wifi_scan_req_param_t scan_req_param = {0};


    if(PNULL == param.p_body)
    {
        SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg param.pbody is PNULL");
    }
    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg msg id = %d", param.msg_id);
    switch(ual_wifi_msg)
    {
        case MSG_UAL_WIFI_OPEN_CNF :
        {
            ual_wifi_open_cnf_t ret = *(ual_wifi_open_cnf_t*)param.p_body;
            if(TRUE == ret.is_ok)
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg open ok ,start scan");
                ZDT_Wifi_StartScan();
            }
            else
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg WIFI Open fail");
            }
            break;
        }
        case MSG_UAL_WIFI_CLOSE_CNF :
        {
            if(TRUE == g_zdt_is_reopen_wifi_scan)
            {
                g_zdt_is_reopen_wifi_scan = FALSE;
                #ifdef UAL_RF_AM_SUPPORT
                if(RF_AM_STATE_IDLE == ual_rf_am_get_current_status() || RF_AM_STATE_WIFISCAN_WORK == ual_rf_am_get_current_status())
                {
                    ual_wifi_open();
                }
                else
                {
                    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg,wifi status error, can't scan");

                ////////////////////////////////////////////////////////////////////////////
                // 因为W217 平台BUG, 有可能第一次搜索不到WIFI AP LIST, 重新搜索
                //if (TRUE == mmi_is_re_get_wifi_list_flag())
                if ((mmi_is_re_get_wifi_list_flag()>0 && mmi_is_re_get_wifi_list_flag()<ZDT_UAL_WIFI_RE_GET_SCANN_TIMES)&&(0==ZDT_WIFI_Get_DataNum()))// 没搜索到,最多重搜索 10 次
                {
                    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg, can't scan,  re get wifi list also is NULL.");
                    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg, wuxx, can't scan, re get wifi list times=%d", mmi_is_re_get_wifi_list_flag());
                    #if 1// W217 BUG 初次开机进入搜索WIFI 第一次一般搜索不到,重新搜索或者搜索多次才可以
                    ////MMIZDTWIFI_Factory_WIFI_Start();
                    mmi_re_get_wifi_list_delay_StartTimer(5000);
                    #else
                    ZDT_WIFI_HW_Set_IsOK(FALSE);
                    ZDT_WIFI_Set_DataNum(0);
                    
                    // ZDT_WIFI_Clean_Data(); // 进入界面前调用
                    
                    // SHOW WIFI DATA 
                    if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
                    {
                        MMIZDTWIFI_Factory_ShowScanData();
                    }
                   #endif
                }
                ////////////////////////////////////////////////////////////////////////////
                }
                #else
                ual_wifi_open();
                #endif
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg close wifi cnf,reopen wifi");
            }
            else
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg close wifi cnf,close wifi");
            }
            break;
        }
        case MSG_UAL_WIFI_START_SCAN_CNF :
        {
            ual_wifi_start_scan_cnf_t ret = *(ual_wifi_start_scan_cnf_t*)param.p_body;
            if(TRUE == ret.is_ok)
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg start scan ok,wait for ap_list");

                ////////////////////////////////////////////////////////////////////////////
                // 因为W217 平台BUG, 有可能第一次搜索不到WIFI AP LIST, 重新搜索
                //if (TRUE == mmi_is_re_get_wifi_list_flag())
                if ((mmi_is_re_get_wifi_list_flag()>0 && mmi_is_re_get_wifi_list_flag()<ZDT_UAL_WIFI_RE_GET_SCANN_TIMES)&&(0==ZDT_WIFI_Get_DataNum()))// 没搜索到,最多重搜索 10 次
                {
                    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg,  re get wifi list also is NULL.");
                    SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg, wuxx, re get wifi list times=%d", mmi_is_re_get_wifi_list_flag());
                    #if 1// W217 BUG 初次开机进入搜索WIFI 第一次一般搜索不到,重新搜索或者搜索多次才可以
                    ////MMIZDTWIFI_Factory_WIFI_Start();
                    mmi_re_get_wifi_list_delay_StartTimer(5000);
                    #else
                    ZDT_WIFI_HW_Set_IsOK(FALSE);
                    ZDT_WIFI_Set_DataNum(0);
                    
                    // ZDT_WIFI_Clean_Data(); // 进入界面前调用
                    
                    // SHOW WIFI DATA 
					if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
					{
                    	MMIZDTWIFI_Factory_ShowScanData();
					}
                    #endif
                }
                ////////////////////////////////////////////////////////////////////////////
            }
            else
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg  WIFI Start Scan fail.");
                 /////////////////////////////////////////////////////////////////////////////
                 ZDT_WIFI_HW_Set_IsOK(FALSE);
                 ZDT_WIFI_Set_DataNum(0);
                
                 // ZDT_WIFI_Clean_Data(); // 进入界面前调用

                 if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
                {
                     // SHOW WIFI DATA 
                     MMIZDTWIFI_Factory_ShowScanData();
                }

                
                 if (g_is_get_wifi_data_by_net == TRUE)
                {
                     #if 1 // 20231213
                     ZDT_W217_WIFI_scan_result_for_net();
                     #else
                     //MMIWIFI_HanldeScanFail(PNULL);// fail
                     MMIZDT_WIFI_OnOff(0);
                     MMIZDTWIFI_API_LocCallback();
                     
                     g_wifi_app_loc_once_result = 0;
                     g_wifi_app_loc_once_start = FALSE;
                     #endif
                }
                /////////////////////////////////////////////////////////////////////////////
            }
            break;
        }
        case MSG_UAL_WIFI_STOP_SCAN_CNF :
        {
            SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg stop scan cnf");
            break;
        }
        case MSG_UAL_WIFI_SCAN_AP_INFO_IND :
        {
            SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg scan ap_node");
            break;
        }
        case MSG_UAL_WIFI_SCAN_AP_INFO_CNF :
        {
            uint32 node_index = 0;
            uint32 wifi_index = 0;
            ual_wifi_scan_ap_info_t show_ap_list[UAL_WIFI_AP_NODE_NUMBER] = {0};

            if(UAL_WIFI_RES_SUCCESS != ual_wifi_get_scaned_ap_list(show_ap_list,UAL_WIFI_AP_NODE_NUMBER))
            {
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg get ap_list wrong");
                 /////////////////////////////////////////////////////////////////////////////
                 ZDT_WIFI_HW_Set_IsOK(FALSE);
                 ZDT_WIFI_Set_DataNum(0);
                
                 // ZDT_WIFI_Clean_Data(); // 进入界面前调用

                 if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
                {
                     // SHOW WIFI DATA 
                     MMIZDTWIFI_Factory_ShowScanData();
                }

                 
                  if (g_is_get_wifi_data_by_net == TRUE)
                 {
                     #if 1 // 20231213
                     ZDT_W217_WIFI_scan_result_for_net();
                     #else
                      //MMIWIFI_HanldeScanFail(PNULL);// fail
                      MMIZDT_WIFI_OnOff(0);
                      MMIZDTWIFI_API_LocCallback();

                      g_wifi_app_loc_once_result = 0;
                      g_wifi_app_loc_once_start = FALSE;
                      #endif
                 }
                /////////////////////////////////////////////////////////////////////////////
                break;
            }
            for(node_index = 0;node_index < UAL_WIFI_AP_NODE_NUMBER; node_index++)//cid607264
            {
                char ssid_char[UAL_WIFI_SSID_MAX_LEN] = { 0 };
                char bssid_char[UAL_WIFI_AP_NODE_BSSID_STR_LEN] = { 0 };
                char rssi_char[UAL_WIFI_AP_NODE_RSSI_STR_LEN] = { 0 };
                uint8 index=0;
                uint i_tmp=0;

                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg, scan ok,show ap_list node");
                if(0 == show_ap_list[node_index].rssival.rssival)
                {
                    continue;
                }
                //for(index=0; index++; index < show_ap_list[node_index].ssid.ssid_len)
                {
                    sprintf( ssid_char, "ssid:%s",show_ap_list[node_index].ssid.ssid); // wuxx add wifi ap name.
                }
                
                sprintf( bssid_char, "MAC:%02x:%02x:%02x:%02x:%02x:%02x", show_ap_list[node_index].bssid_info.bssid[5], show_ap_list[node_index].bssid_info.bssid[4],
                        show_ap_list[node_index].bssid_info.bssid[3], show_ap_list[node_index].bssid_info.bssid[2],show_ap_list[node_index].bssid_info.bssid[1], show_ap_list[node_index].bssid_info.bssid[0] );
                sprintf( rssi_char, "rssi:%02d db",show_ap_list[node_index].rssival.rssival);

                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg ssid_char[%d]%s", node_index, ssid_char);// wuxx add wifi ap name.
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg bssid_char[%d]%s", node_index, bssid_char);
                SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg rssi_char[%d]%s", node_index, rssi_char);

                ////////////////////////////////////////////////////////////////////////////
                memset(&g_zdt_wifi_cur_data, 0, sizeof(g_zdt_wifi_cur_data));
                
                g_zdt_wifi_cur_data.idx = 1;//node_index;// 看原始数据结构是0 空着,这里写1
                g_zdt_wifi_cur_data.channel = show_ap_list[node_index].channel;
                g_zdt_wifi_cur_data.ssid_len = show_ap_list[node_index].ssid.ssid_len;
                g_zdt_wifi_cur_data.rssi = show_ap_list[node_index].rssival.rssival;
                g_zdt_wifi_cur_data.security_enabled = 1;//wifi_scan_ind_ptr->ap_info.encryp_protocol;
                g_zdt_wifi_cur_data.bss_type = 0;//wifi_scan_ind_ptr->ap_info.network_mode;
                ////SCI_MEMCPY(g_zdt_wifi_cur_data.bssid,bssid_char, UAL_WIFI_AP_NODE_BSSID_STR_LEN);
                ////SCI_MEMCPY(g_zdt_wifi_cur_data.ssid,ssid_char, UAL_WIFI_SSID_MAX_LEN);
                //SCI_MEMCPY(g_zdt_wifi_cur_data.bssid, show_ap_list[node_index].bssid_info.bssid, UAL_WIFI_AP_NODE_BSSID_STR_LEN);
                for (i_tmp = 0; i_tmp < UAL_WIFI_BSSID_MAX_LEN; i_tmp++)// MAC 前后转换,为何不同啊// UAL_WIFI_BSSID_MAX_LEN==6
                {
                 g_zdt_wifi_cur_data.bssid[UAL_WIFI_BSSID_MAX_LEN-1-i_tmp] = show_ap_list[node_index].bssid_info.bssid[i_tmp];
                }
                SCI_MEMCPY(g_zdt_wifi_cur_data.ssid, show_ap_list[node_index].ssid.ssid, UAL_WIFI_SSID_MAX_LEN);
                ZDT_WIFI_Add_Data(&g_zdt_wifi_cur_data);

                ////////////////////////////////////////////////////////////////////////////

            }

            /////////////////////////////////////////////////////////////////////////////
            ZDT_WIFI_HW_Set_IsOK(TRUE);
            ////ZDT_WIFI_Set_DataNum(node_index);


            if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
            {
                // SHOW WIFI DATA 
                MMIZDTWIFI_Factory_ShowScanData();
            }

            
           /////////////////////////////////////////////////////////////////////////////
            
            ual_wifi_stop_scan();
            SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg has scaned all ap_list node");

            
             if (g_is_get_wifi_data_by_net == TRUE)
            {
                 #if 1 // 20231213
                 ZDT_W217_WIFI_scan_result_for_net();
                 #else
                 //MMIWIFI_HanldeScanOK(PNULL); // OK
                 MMIZDT_WIFI_OnOff(0);
                 MMIZDTWIFI_API_LocCallback();

                 g_wifi_app_loc_once_result = 1;
                 g_wifi_app_loc_once_start = FALSE;
                 #endif
            }
            break;
        }
        default:
        {
            SCI_TRACE_LOW("MMIZDT_HandleUalWifimsg msg is wrong");
            return FALSE;
        }
    }
    return TRUE;
}


uint8 MMIZDTWIFI_Start(void)
{
    uint8 res = 0;
    g_wifi_app_loc_once_result = 0;
    g_wifi_app_loc_once_start = FALSE;

    if(NULL != g_zdt_ual_wifi_handle)
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Start unregister wifi cms handle = %d",g_zdt_ual_wifi_handle);
        ////ual_wifi_unregister(g_zdt_ual_wifi_handle);
        ////g_zdt_ual_wifi_handle = NULL;

        //什么情况下异常导致WIFI 不能开启, 先关闭
        SCI_TRACE_LOW("MMIZDTWIFI_Start,  before open , close it  ");
        MMIZDTWIFI_Stop();
    }


    if(NULL == g_zdt_ual_wifi_handle)
    {
        ual_wifi_register(MMIZDT_HandleUalWifimsg,&g_zdt_ual_wifi_handle);
        SCI_TRACE_LOW("MMIZDTWIFI_Start = %d",g_zdt_ual_wifi_handle);
    }
    else
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Start Error:g_zdt_ual_wifi_handle is %d,not NULL",g_zdt_ual_wifi_handle);
    }

    #ifdef UAL_RF_AM_SUPPORT
    if(RF_AM_STATE_IDLE == ual_rf_am_get_current_status() || RF_AM_STATE_WIFISCAN_WORK == ual_rf_am_get_current_status())
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Start, ual_wifi_open");
        ual_wifi_open();
    }
    else
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Start Error:wifi status is not RF_AM_STATE_IDLE or RF_AM_STATE_WIFISCAN_WORK");
        res = 1;
        //有时有异常
        MMIZDTWIFI_Stop();
        
        ////////////////////////////////////////////////////////////////////////////
        // 因为W217 平台BUG, 有可能第一次搜索不到WIFI AP LIST, 重新搜索
        //if (TRUE == mmi_is_re_get_wifi_list_flag())
        if ((mmi_is_re_get_wifi_list_flag()<ZDT_UAL_WIFI_RE_GET_SCANN_TIMES)&&(0==ZDT_WIFI_Get_DataNum()))// 没搜索到,最多重搜索 10 次
        {
            SCI_TRACE_LOW("MMIZDTWIFI_Start,  re get wifi list also is NULL.");
            SCI_TRACE_LOW("MMIZDTWIFI_Start, wuxx, re get wifi list times=%d", mmi_is_re_get_wifi_list_flag());
            #if 1// W217 BUG 初次开机进入搜索WIFI 第一次一般搜索不到,重新搜索或者搜索多次才可以
            ////MMIZDTWIFI_Factory_WIFI_Start();
            mmi_re_get_wifi_list_delay_StartTimer(2500);
            #else
            ZDT_WIFI_HW_Set_IsOK(FALSE);
            ZDT_WIFI_Set_DataNum(0);
            
            // ZDT_WIFI_Clean_Data(); // 进入界面前调用
            
            // SHOW WIFI DATA 
            if (g_is_get_wifi_data_by_net == FALSE && g_is_get_wifi_data_from_factory)
            {
                MMIZDTWIFI_Factory_ShowScanData();
            }
            #endif
            res = 2;
        }
        ////////////////////////////////////////////////////////////////////////////

        // 如APP 发CR 同时,智能机呼叫手表接听,此时搜索不到WIFI, 最大搜索次数到了,发搜索失败消息 MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
        // 不然会导致, 电话挂断后, 再发CR时， YX_LOCTION_Start--(YX_LOCTION_Link_TypeExsit(pMe,type))  存在而不能再搜索WIFI 的问题
        SCI_TRACE_LOW("MMIZDTWIFI_Start mmi_is_re_get_wifi_list_flag()=%d", mmi_is_re_get_wifi_list_flag());
        if (mmi_is_re_get_wifi_list_flag()>=(ZDT_UAL_WIFI_RE_GET_SCANN_TIMES-1)) // wuxx 20240102
        {
            //ual_wifi_stop_scan();
            MMIZDTWIFI_Stop(); 
            SCI_TRACE_LOW("MMIZDTWIFI_Start MMIZDTWIFI_Stop, mmi_is_re_get_wifi_list_flag()>=(ZDT_UAL_WIFI_RE_GET_SCANN_TIMES-1)");
            
             if (g_is_get_wifi_data_by_net == TRUE)
            {
                 SCI_TRACE_LOW("MMIZDTWIFI_Start  mmi_is_re_get_wifi_list_flag()>=(ZDT_UAL_WIFI_RE_GET_SCANN_TIMES-1), 01");
                 #if 1 // 20231213
                 ZDT_W217_WIFI_scan_result_for_net();
                 #else
                 //MMIWIFI_HanldeScanFail(PNULL);// fail
                 MMIZDT_WIFI_OnOff(0);
                 MMIZDTWIFI_API_LocCallback();
                 
                 g_wifi_app_loc_once_result = 0;
                 g_wifi_app_loc_once_start = FALSE;
                #endif
            }
        }
        ////////////////////////////////////////////////////////////////////////////
    }
    #else
    ual_wifi_open();
    #endif

   //WIFI status on
    s_zdt_wifi_status  = 0; // wuxx modify 

    s_zdt_wifi_is_open = 1;

    s_zdt_wifi_is_on = 1;
    return res;
}


void MMIZDTWIFI_Stop(void)
{
    ual_wifi_state_e        ual_wifi_state = 0;

    g_wifi_app_loc_once_result = 0;
    g_wifi_app_loc_once_start = FALSE;

    SCI_TRACE_LOW("MMIZDTWIFI_Stop");
    ual_wifi_close(); // wuxx add 20231213 , 有时有异常, 一直无法开启,可能需要 ual_wifi_close,不判断条件

    ual_wifi_get_current_state(&ual_wifi_state);
    if(UAL_WIFI_STATE_IDLE != ual_wifi_state)
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Stop, ual_wifi_close, UAL_WIFI_STATE_IDLE != ual_wifi_state.");
        ual_wifi_close();
    }

    if(NULL != g_zdt_ual_wifi_handle)
    {
        SCI_TRACE_LOW("MMIZDTWIFI_Stop unregister wifi cms handle = %d",g_zdt_ual_wifi_handle);
        ual_wifi_unregister(g_zdt_ual_wifi_handle);
        g_zdt_ual_wifi_handle = NULL;
    }

    //WIFI status on
     s_zdt_wifi_status  = 0;

    s_zdt_wifi_is_open = 0;

    s_zdt_wifi_is_on = 0;

}

////是否网络调用WIFI 接口, TRUE IS NET ,FALSE IS FACTORY TEST.
////BOOLEAN g_is_get_wifi_data_by_net = FALSE;
void ZDT_W217_WIFI_ON_OFF_for_Net(BOOLEAN isOn)
{
    if (isOn)
    {
        g_is_get_wifi_data_by_net = TRUE;
        MMIZDTWIFI_Factory_WIFI_Start();
    }
    else
    {
        g_is_get_wifi_data_by_net = FALSE;
        mmi_re_get_wifi_list_delay_StopTimer();
        MMIZDTWIFI_Stop();
    }


}

void ZDT_W217_WIFI_scan_result_for_net(void) // 20231213 same as old msg. WIFISUPP_SIG_OFF_CNF
{
    s_zdt_wifi_status = 0;
    g_zdt_wifi_is_hw_on = FALSE;
    ZDT_WIFI_ReBuild_Data(g_zdt_wifi_data_cnt);
    
    if(g_zdt_wifi_data_cnt > 0)
    {
        mmi_re_get_wifi_list_delay_StopTimer();// 搜索OK, 停止重新搜索TIMER
        
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_OK,PNULL,0);
    }
    else
    {
        if (mmi_is_re_get_wifi_list_flag()>=ZDT_UAL_WIFI_RE_GET_SCANN_TIMES)// 达到最大重新搜索次数,停止TIMER
        {
            mmi_re_get_wifi_list_delay_StopTimer();
        }
    
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_WIFI_SCAN_FAIL,PNULL,0);
    }

}



#endif// W217 WIFI END



#if defined(ZDT_HSENSOR_SUPPORT)
#ifdef WIN32
int ZDT_HSensor_Init(void)
{    
    return 0;
}

int ZDT_HSensor_Open(void)
{    
    return 0;
}

int ZDT_HSensor_Close(void)
{    
    return 0;
}

int ZDT_HSensor_Reset(void)
{    
    return 0;
}

int ZDT_HSensor_TimerOut(void)
{    
    return 0;
}

int ZDT_HSensor_HR(void)
{    
    return 0;
}

int ZDT_HSensor_TW(void)
{    
    return 0;
}

uint16 ZDT_HSensor_GetHR(void)
{    
    return 0;
}

uint16 ZDT_HSensor_GetHRPre(void)
{    
    return 0;
}

double ZDT_HSensor_GetTW(void)
{    
    return 0;
}

double ZDT_HSensor_GetRealTW(void)
{    
    return 0;
}


BOOLEAN ZDT_HSensor_HW_IsOK(void) 
{
    return 0;
}

ZDT_HSENSOR_IC_TYPE_E ZDT_HSensor_GetType(void)
{
    return HSENSOR_TYPE_NULL;
}
#endif
#endif

#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
LOCAL void StopHeartrateValueTimer();

LOCAL void StartHeartrateTestTimer();




LOCAL uint8 heartrate_test_timer_id = 0;

LOCAL uint8 hr_get_value_timer_id = 0;
LOCAL uint8 hr_get_value_times = 0;




void HeartrateValueFunc( uint8 timer_id, uint32 param)
{
    uint16 hr_value = ZDT_HSensor_GetHR();
    YX_APP_T * pMe = &g_yx_app;
    hr_get_value_times++;
    if(hr_value > 0)
    {
        StopHeartrateValueTimer();
		#ifndef ZDT_HSENSOR_SUPPORT_TW
	        YX_Net_Send_HEART(pMe, hr_value);
		#endif
        if(!MMIZDT_IsInHRWin())
        {
            
            ZDT_HSensor_Close();      
        }
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
        if(yx_DB_Set_Rec.heart > 2)
#endif
        {
            StartHeartrateTestTimer();
        }

    }
    if(hr_get_value_times > 60)
    {
        hr_get_value_times = 0;
        StopHeartrateValueTimer();
        if(!MMIZDT_IsInHRWin())
        {
            
            ZDT_HSensor_Close();      
        }
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
        if(yx_DB_Set_Rec.heart > 2)
#endif
        {
            StartHeartrateTestTimer();
        }
    }

    
}


LOCAL void StopHeartrateValueTimer()
{
    if(hr_get_value_timer_id != 0)
    {
        MMK_StopTimer(hr_get_value_timer_id);
    }
    hr_get_value_times = 0;
    hr_get_value_timer_id = 0;
    
}


LOCAL void StartHeartrateValueTimer()
{
    if(hr_get_value_timer_id != 0)
    {
        MMK_StopTimer(hr_get_value_timer_id);
    }
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    if(yx_DB_Set_Rec.heart > 0)
#endif
    {
        hr_get_value_timer_id = MMK_CreateTimerCallback(2000, HeartrateValueFunc, 0, TRUE);
    }
}



void HeartrateTestFunc( uint8 timer_id, uint32 param)
{
    ZDT_HSensor_Open();
    StartHeartrateValueTimer();
    
}


LOCAL void StopHeartrateTestTimer()
{
    if(heartrate_test_timer_id != 0)
    {
        MMK_StopTimer(heartrate_test_timer_id);
    }
    heartrate_test_timer_id = 0;
    
}


LOCAL void StartHeartrateTestTimer()
{
    if(heartrate_test_timer_id != 0)
    {
        MMK_StopTimer(heartrate_test_timer_id);
    }
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    if(yx_DB_Set_Rec.heart > 1)
    {
        heartrate_test_timer_id = MMK_CreateTimerCallback(yx_DB_Set_Rec.heart*1000, HeartrateTestFunc, 0, FALSE);
    }
    else if(yx_DB_Set_Rec.heart == 1)
    {
        heartrate_test_timer_id = MMK_CreateTimerCallback(100, HeartrateTestFunc, 0, FALSE);// only test one time
    }
#else
    heartrate_test_timer_id = MMK_CreateTimerCallback(1000, HeartrateTestFunc, 0, FALSE);
#endif
}



PUBLIC void ZDT_StartHeartrate(uint16 frequency)
{
    if(frequency > 0)
    {
        StartHeartrateTestTimer();
    }
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    else if(yx_DB_Set_Rec.heart == 0)
    {
        StopHeartrateTestTimer();
    }
#endif
}

#endif

#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
extern BOOLEAN  g_tw_is_win_testing;
PUBLIC void ZDT_SendTw(double tw);

BOOLEAN g_tw_is_net_testing = FALSE;
LOCAL uint8 tw_test_timer_id = 0;
LOCAL uint32 tw_test_times_sec = 0;

LOCAL void StartTwTestTimer();
LOCAL void StopTwTestTimer();
void TwTestFunc( uint8 timer_id, uint32 param)
{
    double tw = 0;
    double r_tw = 0;
    uint32 need_sec = 0;
#ifdef ZDT_PLAT_YX_SUPPORT
    need_sec = yx_DB_Set_Rec.body_tw_peroid;
#endif
    if(g_tw_is_net_testing)
    {
        tw = MMIZDT_TW_HW_Get(&r_tw);
        if(tw > 35 && tw <= 42.5)
        {
            ZDT_SendTw(tw);
            g_tw_is_net_testing = FALSE;
            if(g_tw_is_win_testing == FALSE)
            {
                MMIZDT_TW_HW_Close();
            }
        }
    }
#ifdef ZDT_PLAT_YX_SUPPORT
    if(yx_DB_Set_Rec.body_tw_peroid <= 2)
    {
        need_sec = 1;
    }
    else
    {
        need_sec = yx_DB_Set_Rec.body_tw_peroid/2;
    }
#endif
    tw_test_times_sec++;
    if(tw_test_times_sec >= need_sec)
    {
        g_tw_is_net_testing = TRUE;
        tw_test_times_sec = 0;
    }
}

LOCAL void StopTwTestTimer()
{
    if(tw_test_timer_id != 0)
    {
        MMK_StopTimer(tw_test_timer_id);
    }
    tw_test_timer_id = 0;
    
}

LOCAL void StartTwTestTimer()
{
    if(tw_test_timer_id != 0)
    {
        MMK_StopTimer(tw_test_timer_id);
    }
    tw_test_timer_id = MMK_CreateTimerCallback(2000, TwTestFunc, 0, TRUE);
}


PUBLIC void ZDT_StartBodyTw(uint16 frequency)
{
    if(frequency > 0)
    {
        
        MMIZDT_TW_HW_Open();
        g_tw_is_net_testing = TRUE;
        StartTwTestTimer();
        tw_test_times_sec = 0;
    }
    else
    {
        StopTwTestTimer();
        if(g_tw_is_win_testing == FALSE)
        {
            MMIZDT_TW_HW_Close();
        }
        g_tw_is_net_testing = FALSE;
    }
}


PUBLIC void ZDT_SendTw(double tw)
{
#ifdef ZDT_PLAT_YX_SUPPORT_LZ
    YX_Net_Send_REPORT_TEMPER_Ex(tw);
#elif defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    YX_APP_T * pMe = &g_yx_app;
    YX_Net_Send_TEMP(pMe, tw);
#endif
}
#endif

#ifdef ZDT_NFC_SUPPORT
#ifdef WIN32 
uint8 g_nfc_card_simulation_init_error = 0;
BOOLEAN ZDT_NFC_Init(void)
{    
    return FALSE;
}

uint16 ZDT_NFC_SIM_UID_IsOK(uint8 * pUID,uint16 uid_max_len)
{
    return 0;
}

void ZDTNFC_TaskClose(void)
{
    return;
}

static uint8_t         g_nfc_uid_buf[50] = {0xB6,0xF0,0x28,0xC4,0};
static uint8_t         g_nfc_uid_len = 4;

static uint8_t nfc_Nibble2HexChar(uint8_t bNibble)
{
    uint8_t  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint16_t nfc_U8_To_Hex(uint8_t *buf,uint16_t len,uint8_t * dest_buf,uint16_t dest_len)
{
    uint8_t low = 0;
    uint8_t high = 0;
    int i = 0;
    uint16_t j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    if(len > 0)
    {
        for(i = len-1; i >= 0; i--)
        {
            high = buf[i]>>4;
            dest_buf[j++] = nfc_Nibble2HexChar(high);
            low = buf[i] & 0x0F;
            dest_buf[j++] = nfc_Nibble2HexChar(low);
        }
    }
    dest_buf[j] = 0;
    return j;
}

uint16_t ZDT_NFC_SIM_UID_IsOK(uint8_t * pUID,uint16_t uid_max_len)
{
    int i = 0;
    uint32_t high = 0;
    uint32_t low = 0;
    uint64 int_val = 0;
    uint64 temp = 0;
    uint64 temp_l = 0;
    uint8 int_str[81] = {0};
    char hex_str[81] = {0};
    char all_str[201] = {0};
    uint16_t int_str_len = 0;
    uint16_t hex_str_len = 0;
    uint16_t all_str_len = 0;
    
    if(g_nfc_uid_len > 0)
    {
        hex_str_len = nfc_U8_To_Hex(g_nfc_uid_buf,g_nfc_uid_len,hex_str,80);
        int_str_len = g_nfc_uid_len;
        if(int_str_len > 8)
        {
            int_str_len = 8;
        }
        for(i = 0; i < int_str_len; i++)
        {
            temp = g_nfc_uid_buf[i];
            temp = temp << (i*8);
			int_val |= temp;
        }
        
        if(int_val > 0)
        {
            temp = int_val/1000000000;
            high = temp;
            temp_l = int_val - (temp*1000000000);
            low = temp_l;
            sprintf(all_str,"%ld%09ld(%s)",high,low,hex_str);
            all_str_len = strlen(all_str);
            if(all_str_len >  uid_max_len)
            {
                all_str_len = uid_max_len;
            }
            SCI_MEMCPY(pUID,all_str,all_str_len);
        }
    }
    return all_str_len;
}
#endif
#endif

#ifdef WIN32 
void ZDT_SaveCurStepsBeforePowerOff()
{
}
#endif

#ifdef ZDT_GSENSOR_SUPPORT
PUBLIC void ZDT_NV_SetTodaySteps(uint32 steps)
{
    STEP_NV_T step_nv = {0};
    SCI_DATE_T          date;
    TM_GetSysDate(&date);
    step_nv.steps = steps;
    step_nv.month = date.mon;
    step_nv.day = date.mday;
    MMINV_WRITE(MMI_ZDT_STEPS, &step_nv);
}

PUBLIC uint32 ZDT_NV_GetTodaySteps()
{
    STEP_NV_T step_nv = {0};
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMI_ZDT_STEPS, &step_nv, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_DATE_T          date;
        TM_GetSysDate(&date);
        step_nv.steps = 0;
        step_nv.month = date.mon;
        step_nv.day = date.mday;
        MMINV_WRITE(MMI_ZDT_STEPS, &step_nv);
    }
    return step_nv.steps;
}

PUBLIC STEP_NV_T ZDT_NV_GetTodayNvStepInfo()
{
    STEP_NV_T step_nv = {0};
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMI_ZDT_STEPS, &step_nv, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_DATE_T          date;
        TM_GetSysDate(&date);
        step_nv.steps = 0;
        step_nv.month = date.mon;
        step_nv.day = date.mday;
        MMINV_WRITE(MMI_ZDT_STEPS, &step_nv);
    }
    return step_nv;
}


PUBLIC void ZDT_NV_ClearSteps( )
{
    ZDT_NV_SetTodaySteps(0);
    ZDT_GSensor_SetStepOneDay(0);
    SCI_Sleep(20);
}

PUBLIC void ZDT_Step_CheckDayChange()
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    if(time.hour == 0 && time.min == 0 )
    {
        ZDT_NV_ClearSteps();
    }
}




void ZDT_Gsensor_StepInit()
{
    STEP_NV_T step_nv = ZDT_NV_GetTodayNvStepInfo();
    SCI_DATE_T date;
    TM_GetSysDate(&date);
    if(date.mday != step_nv.day || date.mon != step_nv.month)
    {
        ZDT_NV_ClearSteps();
    }
    else
    {
        ZDT_GSensor_SetStepOneDay(step_nv.steps);
    }
}
#endif

#ifdef HERO_ENGINE_SUPPORT
#ifdef WIN32
int32 hr_mainInstallRom(uint8 *p, int32 len)
{
    return 0;
}
int hero_app_get_price_by_appid(int appid)
{
    return 0;
}
int hero_app_reset_price_by_appid(int appid)
{
    return 0;
}
int hero_app_is_show_nettips()
{
    return 0;
}
int hero_app_get_toolbar_type()
{
    return 0;
}
int hero_app_get_menu_swtich()
{
    return 0;
}
int hero_app_check_sms(unsigned char *pContent, int nLen, char *pNum)
{
    return 0;
}
#endif
#endif

void MMIZDT_AppInit(void)
{
    g_zdt_app.ProcessMsg     = HandleZDTAppMsg;
#ifdef FILE_LOG_SUPPORT
    TraceUser_Init();
    Trace_Log_Print("MMIZDT_AppInit");
#endif
    MMIZDT_NV_GetFirstPWON();
#ifdef ZDT_WIFI_SUPPORT
    if(MMIZDT_NV_GetWIFIOnOff())
    {
        //MMIZDT_WIFI_OnOff(1);
    }
#endif
    MMIZDT_NVGetRFID();
    MMIZDT_NV_GetNeedWhiteCall();
    ZDT_DB_Init();

#ifdef ZDT_NET_SUPPORT
    MMIZDT_Net_APP_Init();
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
   SHB_AppInit();
#endif

#ifdef ZDT_GSENSOR_SUPPORT
    ZDT_Gsensor_StepInit();
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
    ZDT_GPS_Init();
    ZDT_GPS_CheckHW_Start();
#endif
    MMIZDT_Gsensor_Init();
    MMIZDT_Hsensor_Init();
    MMIZDT_CheckTimerStart();
    s_zdt_app_init = TRUE;
}

BOOLEAN MMIZDT_MoCallByNum(MN_DUAL_SYS_E dual_sys,uint8 * num_str)
{    
    CC_RESULT_E cc_res = CC_RESULT_ERROR;
    uint16 num_len = 0;
    CC_CALL_SIM_TYPE_E call_sim_type = CC_SIM1_CALL;
    if(num_str == PNULL)
    {
        return FALSE;
    }
    
    num_len = SCI_STRLEN(num_str);
    if(num_len > 0)
    {
        if(dual_sys == MN_DUAL_SYS_2)
        {
            call_sim_type = CC_SIM2_CALL;
        }
        cc_res = MMIAPICC_MakeCall(
                                        dual_sys,
                                        num_str,
                                        num_len,
                                        PNULL,
                                        PNULL,
                                        call_sim_type,
                                        CC_CALL_NORMAL_CALL,
                                        PNULL
                                        );
        
        if(CC_RESULT_SUCCESS == cc_res)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOLEAN MMIZDT_AppIsInit(void)
{
    return s_zdt_app_init;
}

void MMIZDT_InitModule(void)
{
    MMIZDT_RegMenuGroup();
    MMIZDT_RegNv();
    MMIZDT_RegWinIdNameArr();
}

BOOLEAN  MMIZDT_SendSigTo_APP(ZDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_ZDT_SIG_T * psig = PNULL;
    if(s_zdt_app_init == FALSE)
    {
        return FALSE;
    }
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIZDT_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZDT_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}

void MMIZDT_ResetFactory(void)
{
    MMIZDT_NV_SetFirstPWON(1);
    MMIZDT_NV_SetRejectUnknownCall(0);
#ifdef SETUP_WIZARD_SUPPORT
    MMIZDT_NV_Set_Setup_Wizard(0);
#endif
#ifdef ZDT_GSENSOR_SUPPORT
    ZDT_NV_SetTodaySteps(0);
#endif
#ifdef ZDT_PLAT_YX_SUPPORT
    YX_RestorFactoryExtCall();
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
    SHB_RestorFactory();
#endif
#if defined(ZDT_W18_RESTORE_SET_DEFAULT)
    setDefaultPanel(); //设置默认表盘
    MMISET_SetWatchMenuStyle_default();// 默认主菜单样式,圆弧
#endif
    return;
}

#ifdef ZDT_PLAY_RING_CUSTOM
BOOLEAN YX_CustRing_Send_Start(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_CUSTRING_PLAY,PNULL,0);
    return TRUE;
}
#endif

static uint8 Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint32 U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

PUBLIC void ZDT_HexTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[U8]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[U8]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HexTraceEPH(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[EPH]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[EPH]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HexTraceCasic(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[CASIC]:%s",dest_buff);
    }
    else
    {
        char *p;
        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[CASIC]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}
PUBLIC void ZDT_EPHRcvTrace(const char *string, int len)
{
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TraceLow("[EPH]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[EPH]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
}

PUBLIC void ZDT_SHBTraceExt(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TraceLow("[SHB]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[SHB]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZDT_SHBTrace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        ZDT_SHBTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}

int  ZDT_Trace_CR_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    int  is_cr = FALSE;
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == 0x0D || *(*pCmd + i) == 0x0A)
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    if( (i+1) < *pLen && bHaveNextComma && *(*pCmd + i) == 0x0D && *(*pCmd + i + 1) == 0x0A)
    {
        is_cr = TRUE;
    }
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)
        {
            wMaxStrLen = 100;
        }
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            if(is_cr)
            {
                *pCmd += (EndIndex + 2);
                *pLen -= (EndIndex + 2);
            }
            else
            {
                *pCmd += (EndIndex + 1);
                *pLen -= (EndIndex + 1);
            }
        }
    }
    return(bRtn);
}

PUBLIC void ZDT_TCPTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[TCP]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[TCP]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_TCPSendTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Buf_Print("[TCP] Send:%s",tmp_str);
#else
        SCI_TRACE_LOW("[TCP] Send:%s",tmp_str);
#endif
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
#ifdef FILE_LOG_SUPPORT
           Trace_Log_Buf_Print("[TCP] Send:%s",buff);
#else
           SCI_TRACE_LOW("[TCP] Send:%s",buff);
#endif
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_TCPSendTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[TCP][U8] Send:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[TCP][U8] Send_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_TCPRcvTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Buf_Print("[TCP] Rcv:%s",tmp_str);
#else
        SCI_TRACE_LOW("[TCP] Rcv:%s",tmp_str);
#endif
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
#ifdef FILE_LOG_SUPPORT
           Trace_Log_Buf_Print("[TCP] Rcv:%s",buff);
#else
           SCI_TRACE_LOW("[TCP] Rcv:%s",buff);
#endif
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_TCPRcvTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[TCP][U8] Rcv:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[TCP][U8] Rcv_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HTTPSendTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[HTTP] Send:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[HTTP] Send:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HTTPSendTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[HTTP][U8] Send:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[HTTP][U8] Send_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HTTPRcvTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[HTTP] Rcv:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[HTTP] Rcv:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HTTPRcvTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[HTTP][U8] Rcv:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[HTTP][U8] Rcv_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_HTTPTraceCR(const char *data_str, int data_len)
{
#ifndef RELEASE_INFO
    int ret = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[128] = {0};
    
    if(data_str == NULL || data_len == 0)
    {
        return;
    }
    
    pTime = (uint8 *)data_str;
    con_len = data_len;
    while(ret != -1)
    {
        ret = ZDT_Trace_CR_GetNextPara(&pTime,&con_len,tmp_str,127);
        if(ret > 0)
        {
            ZDT_HTTPRcvTrace(tmp_str,ret);
        }
    }

    return;
#endif
}

PUBLIC void ZDT_HmacTrace(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[HMAC]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[HMAC]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}
PUBLIC void ZDT_CryptTrace(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[CRYPT]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[CRYPT]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_DecryptTrace(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[DECRYPT]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[DECRYPT]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}
PUBLIC void ZDT_YXTraceExt(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[YX]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[YX]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZDT_YXTrace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        ZDT_YXTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}

PUBLIC void ZDT_YXTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[YX][U8]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TRACE_LOW("[YX][U8]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_YXTraceCR(const char *data_str, int data_len)
{
#ifndef RELEASE_INFO
    int ret = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[128] = {0};
    
    if(data_str == NULL || data_len == 0)
    {
        return;
    }
    
    pTime = (uint8 *)data_str;
    con_len = data_len;
    while(ret != -1)
    {
        ret = ZDT_Trace_CR_GetNextPara(&pTime,&con_len,tmp_str,127);
        if(ret > 0)
        {
            ZDT_YXTraceExt(tmp_str,ret);
        }
    }

    return;
#endif
}

void ZDT_YXTraceUTF8(char * utf8_str,const char *pszFmt, ...)
{
    char buf[1024] = {0};
    uint16 ucs2_buf[1024] = {0};
    uint8 gb_buf[1024] = {0};
    uint32 ucs2_len = 0;
    uint32 gbk_len = 0;
    va_list argptr;
    va_start(argptr, pszFmt);
    
    if(utf8_str != PNULL)
    {
        ZDT_YXTraceU8(utf8_str,strlen(utf8_str));
        ucs2_len = GUI_UTF8ToWstr(ucs2_buf, 1000, utf8_str, strlen(utf8_str));
        #ifdef WIN32
            gbk_len = ucs2str_to_gb2312str((uint8 *)ucs2_buf, ucs2_len*2,gb_buf);
        #else
            gbk_len = ucs2str_to_gb2312strExt((uint8 *)ucs2_buf, ucs2_len*2,gb_buf,0);
        #endif
        ZDT_YXTraceU8((uint8 *)ucs2_buf,ucs2_len*2);
        ZDT_YXTraceU8(gb_buf,gbk_len);
    }

    if (vsprintf(buf,pszFmt, argptr) >= 0) {
            SCI_TRACE_LOW("[YX][GB]:%s=%s",buf,gb_buf);
            
    }
    va_end(argptr);
}

PUBLIC void ZDT_cJsonTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[JSON]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[JSON]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_TraceExt(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Print("[ZDT]:%s",tmp_str);
#else
        SCI_TRACE_LOW("[ZDT]:%s",tmp_str);
#endif
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
#ifdef FILE_LOG_SUPPORT
        Trace_Log_Print("[ZDT]:%s",buff);
#else
        SCI_TRACE_LOW("[ZDT]:%s",buff);
#endif        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZDT_Trace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        ZDT_TraceExt(buf,tmp_len);
    }
    va_end(va_args);
}
#endif
