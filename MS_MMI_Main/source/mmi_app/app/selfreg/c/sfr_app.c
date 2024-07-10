#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "mmipub.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "mmialarm_service.h"


#include "sfr_carrier_manager.h"
#include "sfr_header.h"
#include "sfr_nv.h"

#define START_RETRY_MAX         (5)
#define REGISTER_RETRY_MAX      (10)


LOCAL BOOLEAN b_sfr_running = FALSE;
LOCAL uint8 s_sfr_starter = 0;
LOCAL uint8 s_sfr_pdp_retry = 0;
LOCAL uint8 s_sfr_register_times = 0;

PUBLIC void MMISFR_SelfRegisterByHttp(uint8 timer_id, uint32 param);
PUBLIC void MMISFR_ResetEvent(int loop_days);
PUBLIC BOOLEAN MMISFR_CheckEvent(void);

PUBLIC BOOLEAN MMISFR_IsChinaTelecom(MN_DUAL_SYS_E sim)
{
    // is the sim China Telecom ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((03 == service_status.plmn.mnc ) || (05 == service_status.plmn.mnc ) || (11 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d (CTCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

PUBLIC BOOLEAN MMISFR_IsChinaMobile(MN_DUAL_SYS_E sim)
{
    // is the sim China Telecom ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((00 == service_status.plmn.mnc ) || (02 == service_status.plmn.mnc ) || (07 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d (CMCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

PUBLIC BOOLEAN MMISFR_IsChinaUnicom(MN_DUAL_SYS_E sim)
{
    // is the sim China Unicom ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((1 == service_status.plmn.mnc ) || (6 == service_status.plmn.mnc ) || (9 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d (CUCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

PUBLIC void MMISFR_Http_Run(void);

PUBLIC void MMISFR_Run(void)
{
    uint8 i_sim;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i_sim))
        {
            if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(i_sim))
            {
                SCI_TraceLow("[Self Reg] Sim [%d] Card Not Ready.", i_sim);
                return;
            }
#ifdef SFR_SUPPORT_CTCC_SMS
            if (MMISFR_IsChinaTelecom(i_sim) )
            {
                if(MMISFR_IsImsiChanged(i_sim))
                {
                    MMISFR_SMS_Run(i_sim);
                }
            }
            //else // Not CTCC Sim
            //{
            //    if(MMISFR_IsImsiChanged(i_sim))
            //    {
            //        MMISFR_NVSetIMSI(i_sim);
            //    }
            //}
#endif
        }
    }
#if 0
    if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL)
    {
        MMISFR_Http_Run();
    }
// CUCC reg when boot, so don't check event
#ifndef SFR_SUPPORT_CUCC
    else
    {
        MMISFR_CheckEvent();
    }
#endif
#endif
}

PUBLIC void MMISFR_End(void)
{
    if (g_sfr_linker.carrier != NULL)
    {
        SCI_TraceLow("[Self Reg] Save the Carrier NV");
        g_sfr_linker.carrier->carrier_saver(g_sfr_linker.carrier->carr_context);
        if (g_sfr_linker.carrier->carr_reg_loop != NULL)
        {
            int loop_min = g_sfr_linker.carrier->carr_reg_loop();
            MMISFR_ResetEvent(loop_min);
        }
        g_sfr_linker.carrier->carier_free(g_sfr_linker.carrier->carr_context);
    }

    g_sfr_linker.carrier = SFRM_GetNextRegCarrier();

    if (g_sfr_linker.carrier != NULL)
    {
        g_sfr_linker.http_return= FALSE;
        SCI_TraceLow("[Self Reg] Start Next Reg Carrier.");
        SlfReg_PostPackageRequest();
    }
    else
    {
        SCI_TraceLow("[Self Reg] No Next Reg Carrier. ---- END");
        SlfReg_CloseRequest();
        MMIAPIPDP_Deactive(MMI_MODULE_SFR);
        b_sfr_running = FALSE;
#ifdef SFR_SUPPORT_CUCC
        MMISFR_CUCC_CleanHistory();
        MMISFR_CUCC_RunNextRegType();
#endif
    }
}

PUBLIC void MMISFR_ReTry(int seconds)
{
    if (g_sfr_linker.carrier != NULL)
    {
        g_sfr_linker.carrier->carier_free(g_sfr_linker.carrier->carr_context);
    }
    SCI_TraceLow("[Self Reg] Retry.");

    SlfReg_CloseRequest();
    MMIAPIPDP_Deactive(MMI_MODULE_SFR);
    b_sfr_running = FALSE;
    MMK_StopTimer(s_sfr_starter);

    if (++s_sfr_register_times < REGISTER_RETRY_MAX)
    {
        b_sfr_running = TRUE;
        s_sfr_starter = MMK_CreateTimerCallback(seconds*1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
    }
}

PUBLIC void MMISFR_Http_Run(void)
{
    if (b_sfr_running)
    {
        SCI_TraceLow("[Self Reg] Running.");
        return;
    }

    MMK_StopTimer(s_sfr_starter);

    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
#ifdef SFR_SUPPORT_CUCC
        if (MMISFR_GetCollectInfoMode())
        {
            MMISFR_CUCC_SaveToHistory();
        }
#endif
        SCI_TraceLow("[Self Reg] GPRS if off !!! --- [can't send register package SFR-STOP].");
        return;
    }
    b_sfr_running = TRUE;
    SCI_TraceLow("[Self Reg] Self Register by http.");
    s_sfr_pdp_retry = 0;
    s_sfr_register_times = 0;
    s_sfr_starter = MMK_CreateTimerCallback(10*1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
}

PUBLIC void MMISFR_SelfRegisterByHttp(uint8 timer_id, uint32 param)
{
    MMK_StopTimer(s_sfr_starter);
    s_sfr_starter = 0;

#ifdef SFR_SUPPORT_CUCC
    if (MMISFR_GetCollectInfoMode() && !MMISFR_ConnectPDP())
#else
    if (!MMISFR_ConnectPDP())
#endif
    {
        if (s_sfr_pdp_retry++ < START_RETRY_MAX)
        {
            SCI_TraceLow("[Self Reg] Timer -- Retry to Start PDP [retry : %d times]", s_sfr_pdp_retry);
            s_sfr_starter = MMK_CreateTimerCallback(10 * 1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
        }
        else
        {
            SCI_TraceLow("[Self Reg] Timer -- Start PDP [final fail !!!]");
        }
    }
}

LOCAL BOOLEAN MMISFR_Event_Callback(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    uint8 isim = 0;

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    SCI_TraceLow("[Self Reg] MMISFR_Event_Callback -- svc_id = %d", svc_id);
    SCI_TraceLow("[Self Reg] MMISFR_Event_Callback Time : %d-%d-%d %d:%d", sys_date.year,sys_date.mon,sys_date.mday,sys_time.hour,sys_time.min);

#ifdef SFR_SUPPORT_CUCC

    if (MMISFR_GetCUCCType() == MMISFR_CUCC_TYPE_OFF)
    {
        SCI_TraceLow("[Self Reg CUCC] start <circle>");
        MMISFR_SetCUCCType(MMISFR_CUCC_TYPE_CIRCLE);
        if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL && MMISFR_GetCollectInfoMode())
        {
            MMISFR_Http_Run();
        }
    }
#else

    // 1. clean the saved meid ....
    memset (sim_iccid, '0', SFR_ICCID_LENGTH);
    for (isim = 0 ; isim < MMI_DUAL_SYS_MAX; isim++)
    {
        MMISFR_NVSetICCID(isim, sim_iccid);
    }

    // 2. start next self reg ...
    if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL)
    {
        MMISFR_Http_Run();
    }

#endif
    return TRUE;
}

LOCAL  void IncreaseDateByOne(SCI_DATE_T  *pdate_ptr)
{
    const uint8 month_info[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const uint8 month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (pdate_ptr->mon<1 ||pdate_ptr->mon>12)
    {
        SCI_TRACE_LOW("IncreaseDateByOne error! mon=%d", pdate_ptr->mon);
        return;
    }
    if(MMIAPICOM_IsLeapYear((pdate_ptr->year)))
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++;
        }
        else if(pdate_ptr->mday == month_info_leap[pdate_ptr->mon-1])
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
        else if(pdate_ptr->mday == month_info[pdate_ptr->mon-1])
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

LOCAL  void IncreaseTimeByMin( SCI_DATE_T *pdate, SCI_TIME_T *ptime, int min)
{
    int i = 0, h=0;

    i = ptime->min + min;
    if (i < 60)
    {
        ptime->min = i;
    }
    else
    {
        ptime->min = i%60;
        h = ptime->hour + i/60;

        for (i = 0; i < h/24; i++)
        {
            IncreaseDateByOne(pdate);
        }
        ptime->hour = h%24;
    }
}

PUBLIC void MMISFR_ResetEvent(int loop_mins)
{
    int32 sfr_event_id = 0;
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};

    if (loop_mins <= 0)
        return;

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    //get event id from nv ...
    sfr_event_id = MMISFR_GetEventID();

    if (sfr_event_id > 0)
    {
        MMIALMSVC_UnRegService(sfr_event_id, TRUE);
    }

    SCI_TraceLow("[Self Reg] Current Time : %d-%d-%d %d:%d",
        sys_date.year,sys_date.mon,sys_date.mday,
        sys_time.hour,sys_time.min);

    IncreaseTimeByMin(&sys_date, &sys_time, loop_mins);

    SCI_MEMSET(&content_event_info, 0x00, sizeof(content_event_info));

    content_event_info.event_type = ALMSVC_EVENT_OTHER;
    content_event_info.event_time.year = sys_date.year;
    content_event_info.event_time.mon = sys_date.mon;
    content_event_info.event_time.mday = sys_date.mday;
    content_event_info.event_time.hour = sys_time.hour;
    content_event_info.event_time.min = sys_time.min;
    content_event_info.event_time.sec = 0;
    content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMISFR_Event_Callback;

    SCI_TraceLow("[Self Reg] Add a New Event %d-%d-%d %d:%d",
        content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,
        content_event_info.event_time.hour,content_event_info.event_time.min);

    sfr_event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);

    SCI_TraceLow("[Self Reg] Add a New Event event_id = %d", sfr_event_id);

    MMIALMSVC_SetAlmService();

    //write event id to nv ...
    MMISFR_SetEventID(sfr_event_id);
}

PUBLIC BOOLEAN MMISFR_CheckEvent(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    SCI_ALARM_T cur_time = {0};
    int32 sfr_event_id = 0;

    sfr_event_id = MMISFR_GetEventID();

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    if ((sys_time.hour == 0 && sys_time.min == 0 && sys_time.sec == 0)
            || !MMIALMSVC_IsValidSvc(sfr_event_id))
    {
        SCI_TraceLow("[Self Reg] Event Valid Svc");
        return FALSE;
    }

    cur_time.year = sys_date.year;
    cur_time.mon = sys_date.mon;
    cur_time.mday = sys_date.mday;
    cur_time.hour = sys_time.hour;
    cur_time.min = sys_time.min;
    cur_time.sec = 0;

    if (MMIALMSVC_IsValidTimedSvc(sfr_event_id, cur_time))
    {
        SCI_TraceLow("[Self Reg] Event Valid (cur time :  %d-%d-%d %d:%d)",
            cur_time.year, cur_time.mon, cur_time.mday, cur_time.hour, cur_time.min);
        return FALSE;
    }

    MMISFR_Event_Callback(sfr_event_id, 0);

   return TRUE;
}

PUBLIC void SFR_LongTrace(const char *string, int len)
{
    if (len < 128)
    {
        SCI_TraceLow("[Self Reg] :%s ",string);
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

           SCI_TraceLow("[Self Reg] :%s ",buff);

          p += l;
          len -= l;
        }
    }
}

PUBLIC void SFR_BinTrace(const char *src, int len)
{
    int i;

    for (i = 0; i + 15 < len; i+= 16)
    {
       SCI_TraceLow("[Self Reg] :0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x",
        src[i+0], src[i+1], src[i+2], src[i+3], src[i+4], src[i+5], src[i+6], src[i+7], 
        src[i+8], src[i+9], src[i+10], src[i+11], src[i+12], src[i+13], src[i+14], src[i+15]);
    }
    if ( i < len)
    {
        char buff[128];
        for( ; i < len; i++)
        {
            char tmp[8];
            sprintf (tmp, "0x%.2x, ", src[i]);
            strcat (buff, tmp);
        }
        SCI_TraceLow("[Self Reg] :%s", buff);
    }
}

