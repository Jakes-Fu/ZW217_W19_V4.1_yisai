/*****************************************************************************
** File Name:      mmiphone_nitz.c                                          *
** Author:                                                                   *
** Date:           01/21/2012                                                   *
** Copyright:      20112Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe update NITZ info        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/20/2012       dave.ruan          Create                                *
******************************************************************************/

#define _MMIPHONE_NITZ_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "mmiphone_export.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_subwintab.h"
#include "mmiidle_export.h"
#include "mmi_id.h"
#include "mmipub.h"
#include "mmi_nv.h"
#include "mmiset_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_UPDATE_TIME_SUPPORT
#include "mmiaut_export.h"
#endif
#include "mmiwclk_export.h"
#include "mmiphone_internal.h"
#include "mmieng_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_NET_TIME_YEAR_BASE      (2000)                         /*网络时间起始年份，这个由网络决定，不允许随意调整*/
#define MMI_TIME_ZONE_SECONDS_IN_QUARTER      (900)                   /*时区小时乘数,以1/4小时计算,60sec * 15mins=900sec*/
#define UINT32_SIZE      (4294967296)                   // 2^32 = 4294967296
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    BOOLEAN  is_net_time_exist;
    BOOLEAN  is_tick_time_exist;
    MN_MM_NET_TIME_INFO_T net_time_info;  
    SCI_TICK_TIME_T    tick_time_info;
}TICK_TIME_INFO_T;

typedef struct 
{
    MN_DUAL_SYS_E  last_exist_nitz_sim;
    MN_DUAL_SYS_E  attaching_nitz_sim;  
    BOOLEAN  is_attach_nitz_time_finish;//is finish,stop attaching gprs
}AUTO_UPDATE_TIME_SIM_T;

typedef struct 
{
    int8  local_time_zone;  //!!include dst value
    BOOLEAN daylight_saving_time_flag;
    uint8   daylight_saving_time;
}TIME_ZONE_INFO_T;

LOCAL MMIPHONE_NV_NETWORK_NAME_T s_network_name_from_nitz[MMI_DUAL_SYS_MAX] = {0};//network name in NV 
LOCAL TICK_TIME_INFO_T s_tick_time = {0};//remember tick time when NITZ comes incase use it
LOCAL AUTO_UPDATE_TIME_SIM_T s_auto_update_time_sim = {0};//remember last SIM index
LOCAL uint8 s_connection_dettach_pdp_timer = 0; //dettach wait pdp timer
LOCAL TIME_ZONE_INFO_T s_tz_dst_info = {0}; //
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_UPDATE_TIME_SUPPORT
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : auto update net work time if support drm
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 手机出厂后一次也未同步过时间且打开DRM则需要同步一次
/*****************************************************************************/
LOCAL void DrmAutoUpdateNetworkTime(void);
#endif
/*****************************************************************************/
// 	Description : 设置时间到系统
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetNetworkInfoTimeToSystem(const MN_MM_NET_TIME_INFO_T *net_time_info_ptr);

/*****************************************************************************/
// 	Description : 网络提供了MM时间后的相关操作
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void ParseSysTimeFromNetworkInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind);

/*****************************************************************************/
// 	Description : 网络提供了MM时间后,记住当前tick和NITZ时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StoreNitzTimeAndTickTimeInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind);

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
// 	Description :Start Get Nitz Time
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartGetNitzTime(uint8  timer_id, uint32 param);
#endif

/*---------------------------------------------------------------------------*/
/*                          RESERVED FOR WIN_TAB DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : auto update net work time if support drm
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 手机出厂后一次也未同步过时间且打开DRM则需要同步一次
/*****************************************************************************/
LOCAL void DrmAutoUpdateNetworkTime(void)
{
    /*因为DRM依赖于自动更新网络时间，但是针对DRM应用来讲，
    只要此手机只要出厂后更新过一次，对于DRM来讲，就够用了，
    以后的更新与否DRM都不做修改及其它逻辑处理*/
    
    MMIDRM_NITZ_TYPE_T nitz_time = {0};
    MMIAPIDRM_GetDrmNitzTime(&nitz_time);

    if (!nitz_time.is_syn_time)
    {
        //如果出厂后，一次也没有更新过，则需要强制自动更新一下
        MMIAPIAUT_SetIsNeedUpdateTimeByNet(TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPHONE] DrmAutoUpdateNetworkTime: The phone has update network time after leaving factory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_93_112_2_18_2_45_23_206,(uint8*)"");
    }
    
    return;
}
#endif

/*****************************************************************************/
// 	Description : 设置时间到系统
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetNetworkInfoTimeToSystem(const MN_MM_NET_TIME_INFO_T *net_time_info_ptr)
{
    SCI_DATE_T   sysdate      = {0};
    SCI_TIME_T   systime      = {0};
    MMI_WIN_ID_T query_win_id = NULL;
    BOOLEAN is_set_ok = FALSE;
    
    if (PNULL == net_time_info_ptr)
    {
        return FALSE;
    }
   
    /*为了避免当RTC有误提示需Reset 日期/时间前需先关闭"自动更新时间"功能,
    与此时正在更新网络时间的冲突。将询问窗口关闭, 退出进一步Reset操作*/
    query_win_id = MMIAPISET_GetCloseAutoUpdateQueryWinID();
    if (MMIAPISET_IsNeedResetDateTime() && MMK_IsOpenWin(query_win_id))
    {
        MMIAPISET_SetNeedResetDateTime(FALSE);
        MMK_CloseWin(query_win_id);
    }

    //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flag =%d daylight_saving_time_flag =%d ,ut_lt_zone.time_zone =%d, local_time_zone= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_128_112_2_18_2_45_23_207,(uint8*)"dddd",net_time_info_ptr->ut_lt_zone_flag,net_time_info_ptr->daylight_saving_time_flag,net_time_info_ptr->ut_lt_zone.time_zone,net_time_info_ptr->local_time_zone);

    if (net_time_info_ptr->ut_lt_zone_flag
        ||net_time_info_ptr->local_time_zone_flag  // only change the time_zone case
    )
    {
        SCI_TIME_T	tmp_time ={0};//MS curr time
        SCI_DATE_T	tmp_date ={0};//MS curr date
        MMISET_DATE_T   mmiset_date ={0};
        MMISET_TIME_T   mmiset_time ={0};
        uint32  net_second = 0;
        int32    temp_second = 0;
        MMI_TM_T mmi_time    = {0};
        
        if(net_time_info_ptr->ut_lt_zone_flag)
        {
            /* set sys time and date */
            sysdate.year = MMI_NET_TIME_YEAR_BASE + (uint16)net_time_info_ptr->ut_lt_zone.year;
            sysdate.mon= net_time_info_ptr->ut_lt_zone.month;
            sysdate.mday = net_time_info_ptr->ut_lt_zone.day;
            systime.hour = net_time_info_ptr->ut_lt_zone.hour;
            systime.min = net_time_info_ptr->ut_lt_zone.minute;
            systime.sec = net_time_info_ptr->ut_lt_zone.second;
            
            net_second = MMIAPICOM_Tm2Second((uint32)systime.sec,
                                                                        (uint32)systime.min,
                                                                        (uint32)systime.hour,
                                                                        (uint32)sysdate.mday,
                                                                        (uint32)sysdate.mon,
                                                                        (uint32)sysdate.year);
            //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_155_112_2_18_2_45_23_208,(uint8*)"d",net_second);
            temp_second = (int32)net_second;

            //add time zone,time zone value is in quarter
            temp_second += (int32)(net_time_info_ptr->ut_lt_zone.time_zone * MMI_TIME_ZONE_SECONDS_IN_QUARTER);
        }
        else if(net_time_info_ptr->local_time_zone_flag)
        {
            net_second = MMIAPICOM_GetCurTime();
            temp_second = (int32)net_second;
            temp_second += (int32)((net_time_info_ptr->local_time_zone - s_tz_dst_info.local_time_zone) *MMI_TIME_ZONE_SECONDS_IN_QUARTER);
        }
        
        //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , temp_second2 =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_169_112_2_18_2_45_24_209,(uint8*)"d",temp_second);

        net_second = (uint32)temp_second;
        //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second2 =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_172_112_2_18_2_45_24_210,(uint8*)"d",net_second);
        //get mmi time
        mmi_time = MMIAPICOM_Second2Tm(net_second);
        
        //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , DT:%04d-%02d-%02d %02d-%02d-%02d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_177_112_2_18_2_45_24_211,(uint8*)"dddddd",mmi_time.tm_year,mmi_time.tm_mon,mmi_time.tm_mday,mmi_time.tm_hour,mmi_time.tm_min,mmi_time.tm_sec);

        mmiset_date.date_day   =  mmi_time.tm_mday;
        mmiset_date.date_month = mmi_time.tm_mon;
        mmiset_date.date_year  =  (uint16)mmi_time.tm_year;
        mmiset_time.time_hour   =  mmi_time.tm_hour;
        mmiset_time.time_minute = mmi_time.tm_min;
        
        TM_GetSysDate(&tmp_date);
        TM_GetSysTime(&tmp_time);	
        if((mmiset_time.time_minute==tmp_time.min)  //minutes first
            &&(mmiset_time.time_hour==tmp_time.hour)
            &&(mmiset_date.date_day ==tmp_date.mday)
            &&(mmiset_date.date_month==tmp_date.mon)
            &&(mmiset_date.date_year ==tmp_date.year)
            &&(!MMIAPIAUT_IsManualUpdateTimeRunning())//if manual update,the same time need pub success message!!!if not, will cause assert
            )
        {
            //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , date time is the same"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_195_112_2_18_2_45_24_212,(uint8*)"");
            return FALSE;
        }

        sysdate.year   = mmiset_date.date_year;
        sysdate.mon   = mmiset_date.date_month;
        sysdate.mday  = mmiset_date.date_day;
        systime.hour    = mmiset_time.time_hour;
        systime.min    = mmiset_time.time_minute;
        systime.sec    = mmi_time.tm_sec;
        //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , set time:%04d-%02d-%02d %02d-%02d-%02d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_206_112_2_18_2_45_24_213,(uint8*)"dddddd",sysdate.year,sysdate.mon,sysdate.mday,systime.hour,systime.min,systime.sec);
        
#ifndef WIN32    
        if(MMIAPICOM_CheckDateIsValid(mmiset_date))
        {
            is_set_ok = MMIAPICOM_SetSysData(sysdate);
            if(TRUE  == is_set_ok)
            {
                if(MMIAPICOM_CheckTimeIsValid(mmiset_time))
                {
                    is_set_ok = MMIAPICOM_SetSysTime(systime);
                }
                else
                {
                    return  FALSE;
                }                
            }
            
            if(TRUE == is_set_ok)
            {
            #ifdef DRM_SUPPORT
                MMIAPIDRM_ResetNetworkTime();
            #endif
                
                /* 以下的函数调用,都是参考HandleSetTimeWindow设置完时间后的一些更新操作*/
                MMIAPISET_UpdateAlarmSet();/*更新闹钟的设置*/
#ifndef SUBLCD_SIZE_NONE                
                //刷新小屏的显示，刷新时间
                MMISUB_UpdateDisplaySubLcd(); 
#endif               
                return TRUE;
            }
        }
        else
#endif
        {
            //SCI_TRACE_LOW:"[MMIPHONE] :SetNetworkInfoTimeToSystem, SetDate:year is %d,month is %d and day is %d is invalid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_251_112_2_18_2_45_24_216,(uint8*)"ddd",mmiset_date.date_year, mmiset_date.date_month, mmiset_date.date_day);
            return  FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flagis_network_time_exist %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_257_112_2_18_2_45_24_217,(uint8*)"d",net_time_info_ptr->ut_lt_zone_flag);
        return FALSE;
    }
    return is_set_ok;
}

/*****************************************************************************/
// 	Description : 网络提供了MM时间后,记住当前tick和NITZ时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StoreNitzTimeAndTickTimeInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind)
{
    if (PNULL == network_info_ind)
    {
        return;
    }

    SCI_TRACE_LOW("[MMIPHONE]: SetSysTimeToTickTimeInfo ut_lt_zone_flag = %d",network_info_ind->time_and_timezone.ut_lt_zone_flag);

    //store network time info and current tick count
    if(network_info_ind->time_and_timezone.ut_lt_zone_flag)
    {
        SCI_TICK_TIME_T tick_time_info = {0};
        
        SCI_MEMSET(&s_tick_time, 0x00, sizeof(TICK_TIME_INFO_T));
       
        s_tick_time.is_net_time_exist = TRUE;
        s_tick_time.net_time_info.daylight_saving_time_flag= network_info_ind->time_and_timezone.daylight_saving_time_flag;
        s_tick_time.net_time_info.daylight_saving_time = network_info_ind->time_and_timezone.daylight_saving_time;
        s_tick_time.net_time_info.local_time_zone_flag = network_info_ind->time_and_timezone.local_time_zone_flag;
        s_tick_time.net_time_info.local_time_zone = network_info_ind->time_and_timezone.local_time_zone;
        SCI_MEMCPY(&s_tick_time.net_time_info.ut_lt_zone, &network_info_ind->time_and_timezone.ut_lt_zone, sizeof(MN_UT_LT_ZONE_T));

        SCI_GetTickTime(&tick_time_info);
        s_tick_time.tick_time_info.milliseconds = tick_time_info.milliseconds;
        s_tick_time.tick_time_info.second = tick_time_info.second;
        s_tick_time.is_tick_time_exist = TRUE;
        SCI_TRACE_LOW("[MMIPHONE]: SCI_GetTickTime tick_time_info.second = %d",tick_time_info.second);
    }

}

/*****************************************************************************/
// 	Description : 利用当前tick和NITZ时间设置系统时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSysTimeWithNitzAndTickTime(void)
{
    BOOLEAN result = FALSE;
    
    SCI_TRACE_LOW("[MMIPHONE]: MMIAPIPHONE_SetSysTimeWithNitzAndTickTime is_net_time_exist= %d, is_tick_time_exist = %d",
                                    s_tick_time.is_net_time_exist, s_tick_time.is_tick_time_exist);

    //store network time info and current tick count
    if(s_tick_time.is_net_time_exist && s_tick_time.is_tick_time_exist)
    {
        SCI_TICK_TIME_T tick_time_info = {0};
        uint32 time_second = 0;
        uint32 time_zone_second = 0;
        uint32 diff_time_second = 0;
        MMI_TM_T mmi_time    = {0};
        
        mmi_time.tm_year = s_tick_time.net_time_info.ut_lt_zone.year + MMI_NET_TIME_YEAR_BASE;
        mmi_time.tm_mon = s_tick_time.net_time_info.ut_lt_zone.month;
        mmi_time.tm_mday = s_tick_time.net_time_info.ut_lt_zone.day;
        mmi_time.tm_hour = s_tick_time.net_time_info.ut_lt_zone.hour;
        mmi_time.tm_min = s_tick_time.net_time_info.ut_lt_zone.minute;
        mmi_time.tm_sec = s_tick_time.net_time_info.ut_lt_zone.second;
        
        SCI_TRACE_LOW("[MMIPHONE]: SetSysTimeWithNitzAndTickTime, set time:%04d-%02d-%02d %02d-%02d-%02d",
                mmi_time.tm_year, mmi_time.tm_mon, mmi_time.tm_mday, mmi_time.tm_hour, mmi_time.tm_min, mmi_time.tm_sec);

        time_second = MMIAPICOM_Tm2Second((uint32)mmi_time.tm_sec, (uint32)mmi_time.tm_min, (uint32)mmi_time.tm_hour, 
                                                    (uint32)mmi_time.tm_mday, (uint32)mmi_time.tm_mon, (uint32)mmi_time.tm_year);

        SCI_TRACE_LOW("[MMIPHONE]: MMIAPICOM_Tm2Second time_second = %d",time_second);
        
        time_zone_second = s_tick_time.net_time_info.ut_lt_zone.time_zone * MMI_TIME_ZONE_SECONDS_IN_QUARTER;
        
        SCI_TRACE_LOW("[MMIPHONE]:  time_zone_second = %d",time_zone_second);

        time_second = time_second + time_zone_second;
        
        SCI_TRACE_LOW("[MMIPHONE]:  add time zone time_second = %d",time_second);
        
        SCI_GetTickTime(&tick_time_info);
        
        SCI_TRACE_LOW("[MMIPHONE]: s_tick_time.tick_time_info.second = %d, tick_time_info.second = %d",s_tick_time.tick_time_info.second, tick_time_info.second);
        
        if(s_tick_time.tick_time_info.second <= tick_time_info.second)
        {
            diff_time_second = tick_time_info.second -s_tick_time.tick_time_info.second;
        }
        else
        {
            diff_time_second = 0xFFFF -(s_tick_time.tick_time_info.second - tick_time_info.second);//if UINT32_SIZE, its wrong
        }
        
        SCI_TRACE_LOW("[MMIPHONE]: diff_time_second = %d",diff_time_second);
        
        time_second = time_second + diff_time_second;
        
        if(MMIAPIAUT_SetSysTime(time_second))
        {
            uint16 current_year = MMI_NET_TIME_YEAR_BASE + s_tick_time.net_time_info.ut_lt_zone.year;
            int8 real_time_zone = 0;
            int8 net_time_zone = 0;
            MN_PLMN_T curr_plmn = {0};
            
            //get network time zone
            if(s_tick_time.net_time_info.ut_lt_zone_flag)
            {
                net_time_zone = s_tick_time.net_time_info.ut_lt_zone.time_zone;
            }
            else if(s_tick_time.net_time_info.local_time_zone_flag)
            {
                net_time_zone = s_tick_time.net_time_info.local_time_zone;
            }
            else
            {
                //use default 0
            }
            
            MMIAPIPHONE_GetCurPLMN(&curr_plmn, MMIAPIPHONE_GetExistNitzSIM());
            //for cetecom test
            if(s_tick_time.net_time_info.daylight_saving_time_flag)
            {
                real_time_zone = net_time_zone -s_tick_time.net_time_info.daylight_saving_time * 4;
            }
            
            if(0 == real_time_zone)
            {
                MMIAPIAUT_GetTimezoneByMcc(curr_plmn.mcc,&real_time_zone);
                SCI_TRACE_LOW("[mmiphone_nitz]:MMIAPIAUT_GetTimezoneByMcc mcc = %d, net_time_zone =%d", curr_plmn.mcc, net_time_zone);
            }

#ifdef WORLD_CLOCK_SUPPORT
            MMIAPIWCLK_SetTimeZone(curr_plmn.mcc, real_time_zone, current_year, s_tick_time.net_time_info.daylight_saving_time);
#endif 
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 网络提供了MM时间后的相关操作
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void ParseSysTimeFromNetworkInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind)
{
    int8 net_time_zone = 0;

    if (PNULL == network_info_ind)
    {
        return;
    }

    //SCI_TRACE_LOW:"[MMIPHONE]: ParseSysTimeFromNetworkInfo success , net_time_zone [%d] daylight_saving_time[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_277_112_2_18_2_45_24_218,(uint8*)"dd",network_info_ind->time_and_timezone.ut_lt_zone.time_zone, network_info_ind->time_and_timezone.daylight_saving_time);
    //get network time zone
    if(network_info_ind->time_and_timezone.ut_lt_zone_flag)
    {
        net_time_zone = network_info_ind->time_and_timezone.ut_lt_zone.time_zone;
    }
    else if(network_info_ind->time_and_timezone.local_time_zone_flag)
    {
        net_time_zone = network_info_ind->time_and_timezone.local_time_zone;
    }
    else
    {
        //use default 0
    }
    
#ifdef DRM_SUPPORT
    DrmAutoUpdateNetworkTime();
#endif
    
    /*时区发生变化时，需要更新系统时间*/
    if (MMIAPIAUT_GetIsNeedUpdateTimeByNet()
        || net_time_zone != s_tz_dst_info.local_time_zone
        || network_info_ind->time_and_timezone.daylight_saving_time_flag != s_tz_dst_info.daylight_saving_time_flag
        )
    {
        
        /*设置网络时间到系统，并提示用户*/
        if (SetNetworkInfoTimeToSystem(&network_info_ind->time_and_timezone))
        {
            uint16 current_year = MMI_NET_TIME_YEAR_BASE + network_info_ind->time_and_timezone.ut_lt_zone.year;
            int8 real_time_zone = 0;
            
            //for cetecom test
            if(network_info_ind->time_and_timezone.daylight_saving_time_flag)
            {
                real_time_zone = net_time_zone -network_info_ind->time_and_timezone.daylight_saving_time * 4;
            }
            
            if(0 == real_time_zone)
            {
                MMIAPIAUT_GetTimezoneByMcc(network_info_ind->cur_plmn.mcc,&real_time_zone);
                SCI_TRACE_LOW("[mmiphone]:MMIAPIAUT_GetTimezoneByMcc mcc = %d, net_time_zone =%d", network_info_ind->cur_plmn.mcc, net_time_zone);
            }

#ifdef WORLD_CLOCK_SUPPORT
            MMIAPIWCLK_SetTimeZone(network_info_ind->cur_plmn.mcc, real_time_zone, current_year, network_info_ind->time_and_timezone.daylight_saving_time);
#endif 
            
            //maybe auto update time; maybe manual update time; so set manual update flag false
            MMIAPISET_ShowUpdateTimeMsgWin(TRUE, FALSE);

            /*重置update_time flag,防止频繁更新时间*/
            MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE); 
            
        }
        else
        {
            /*do nothing*/
            //SCI_TRACE_LOW:"[MMIPHONE]: ParseSysTimeFromNetworkInfo , local_time_zone_flag [%d] ut_lt_zone_flag[%d],daylight_saving_time_flag [%d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_319_112_2_18_2_45_24_219,(uint8*)"ddd",network_info_ind->time_and_timezone.local_time_zone_flag,network_info_ind->time_and_timezone.ut_lt_zone_flag,network_info_ind->time_and_timezone.daylight_saving_time_flag);
        }
        
        //remember tz_dst value
        s_tz_dst_info.local_time_zone = net_time_zone;
        s_tz_dst_info.daylight_saving_time_flag = network_info_ind->time_and_timezone.daylight_saving_time_flag;
        s_tz_dst_info.daylight_saving_time = network_info_ind->time_and_timezone.daylight_saving_time;
    }
    else
    {
        /*do nothing*/
        //SCI_TRACE_LOW:"[MMIPHONE]: ParseSysTimeFromNetworkInfo , net_time_zone=%d, MMIAPIACC_GetLocalTimeZone() =%f"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_326_112_2_18_2_45_24_220,(uint8*)"df",net_time_zone,MMIAPIACC_GetLocalTimeZone());
    }
}

/*****************************************************************************/
// 	Description : Handle Network Time Info Ind
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:获取MM infor 后的处理，只有自动处理，不会手动更新
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetworkTimeInfoInd(const APP_MN_NETWORK_INFO_IND_T *network_info_ind)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if (PNULL == network_info_ind)
    {
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"[MMIPHONE]: AutoUpdateTimeHandleNetworkInfoInd SIM [%d] ut_lt_zone_flag[%d] "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_343_112_2_18_2_45_24_221,(uint8*)"dd", network_info_ind->dual_sys,network_info_ind->time_and_timezone.ut_lt_zone_flag);
   
    if (MMIAPIPHONE_IsSimAvailable(network_info_ind->dual_sys)) /*网络是否注册成功*/
    {
        if (MMIAPIAUT_GetNeedUpdateTimeEnable())
        {
            //time/time_zone/DST any one changes, it should update time
            if (network_info_ind->time_and_timezone.ut_lt_zone_flag
            ||network_info_ind->time_and_timezone.local_time_zone_flag
            )
            {
                /*网络提供了本地时区*/
                ParseSysTimeFromNetworkInfo(network_info_ind);
            } 
            else
            {
                /*do nothing*/
                //SCI_TRACE_LOW:"[MMIPHONE]:  net no support time and timezone"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_NITZ_357_112_2_18_2_45_24_222,(uint8*)"");
            }
        }
    }
    
    if (network_info_ind->time_and_timezone.ut_lt_zone_flag)
    {
        /*网络提供了本地时区*/
        StoreNitzTimeAndTickTimeInfo(network_info_ind);
#ifdef MMI_UPDATE_TIME_WHEN_STARTUP		
        MMIAPIPHONE_SetExistNitzSIM(network_info_ind->dual_sys);
#endif		
    } 

    return result;
}

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
// 	Description : start timer to get NITZ time manual
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_StartTimerToGetNitzTime(void)
{
    if(MMIAPIAUT_GetIsNeedUpdateTimeByNet())
    {
        if (0 != s_connection_dettach_pdp_timer)
        {
            MMK_StopTimer(s_connection_dettach_pdp_timer);
            s_connection_dettach_pdp_timer = 0;
        }

        s_connection_dettach_pdp_timer = MMK_CreateTimerCallback(MMIAUT_5SECOND, StartGetNitzTime, PNULL, FALSE);

        SCI_TRACE_LOW("[MMIPHONE]:: StartTimerToGetNitzTime for connection s_connection_dettach_pdp_timer = %d",s_connection_dettach_pdp_timer);
    }
}

/*****************************************************************************/
// 	Description :Start Get Nitz Time
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartGetNitzTime(uint8  timer_id, uint32 param)
{
    if (0 != s_connection_dettach_pdp_timer)
    {
        MMIAPIPHONE_StartGetNitzTime();
        MMK_StopTimer(s_connection_dettach_pdp_timer);
        s_connection_dettach_pdp_timer = 0;
    }
}

/*****************************************************************************/
// 	Description : start get NITZ time manual
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_StartGetNitzTime(void)
{
    BOOLEAN sim_available_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);

    SCI_TRACE_LOW("[MMIPHONE]: StartGetNitzTime sim_available_num = %d, isGPRSSwitchedOff = %d", sim_available_num, MMIAPICONNECTION_isGPRSSwitchedOff());

    if(!MMIAPIPHONE_IsAttachTimeFinished()
        && MMIAPIPHONE_IsAllSIMSelectNetSucc()
        && MMIAPIAUT_GetIsNeedUpdateTimeByNet()
        && MMIAPISET_GetIsAutoUpdateTime()
        && (!MMIAPICONNECTION_isGPRSSwitchedOff()))
        //&& (MMIAPIPHONE_GetAttachingNitzSIM() == MMI_DUAL_SYS_MAX))//for bug1655309
    {
        //attach gprs after all SIM select network OK
        //attach gprs for NITZ, after active_cnf comes 5sec, then deactive it
        SCI_TRACE_LOW("MMIAPIPHONE_GetNitzTime");
        MMIAPIPHONE_GetNitzTime(MMI_DUAL_SYS_MAX);
    }
}
/*****************************************************************************/
// 	Description : get NITZ time manual
//	Global resource dependence : none
//    Author: dave.ruan
//	Note: attaching_sim 不是我们要attach的卡，attaching_sim+1才是
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_GetNitzTime(MN_DUAL_SYS_E attaching_sim)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    MN_DUAL_SYS_E first_attach_sim = MMI_DUAL_SYS_MAX;
    MN_DUAL_SYS_E exist_nitz_sim = MMIAPIPHONE_GetExistNitzSIM();
    PHONE_SERVICE_STATUS_T* cur_service_status = MMIAPIPHONE_GetServiceStatus();
    
    SCI_TRACE_LOW("[mmiphone]:GetNitzTime exist_nitz_sim= %d, attaching_sim =%d",exist_nitz_sim, attaching_sim);

    if(MMIAPIPHONE_SetSysTimeWithNitzAndTickTime())
    {
        //popup success notification even if set_value is not true
        MMIAPISET_ShowUpdateTimeMsgWin(TRUE,FALSE);
        
        /*重置update_time flag,防止频繁更新时间*/
        MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE); 
    }
    else
    {
        //find a SIM for attach GPRS 
        if(exist_nitz_sim >= MMI_DUAL_SYS_MAX)
        {
            if(attaching_sim + 1 <  MMI_DUAL_SYS_MAX||MMI_DUAL_SYS_MAX == attaching_sim )
            {
                if(MMI_DUAL_SYS_MAX == attaching_sim)
                {
                    first_attach_sim = MN_DUAL_SYS_1;
                }
                else
                {
                    first_attach_sim = attaching_sim+1;//attach next sim
                }
                
                for(dual_sys = first_attach_sim; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    if((MMIAPIPHONE_IsSimAvailable(dual_sys)) &&
                       ((PLMN_NORMAL_GSM_ONLY == cur_service_status[dual_sys].plmn_status) ||
                        (PLMN_NORMAL_GSM_GPRS_BOTH == cur_service_status[dual_sys].plmn_status))
                    /*&& (cur_service_status[dual_sys].gprs_support)*/)//network search ok fast, and may gprs_support not come
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            dual_sys = exist_nitz_sim;
        }

#ifdef MMI_DATA_QUERY_WHEN_BOOTUP
        if((TRUE == MMIAPIPHONE_IsTestSIM(dual_sys)) && (FALSE == MMISET_IsUserUpdateTime()))
        {
            SCI_TRACE_LOW("[mmiphone]:GetNitzTime dual_sys is test sim");
            return;
        }
#endif
        SCI_TRACE_LOW("[mmiphone]:GetNitzTime dual_sys = %d ",dual_sys);

        if(dual_sys < MMI_DUAL_SYS_MAX && MMIAPIAUT_GetIsNeedUpdateTimeByNet())
        {
            SCI_TRACE_LOW("[mmiphone]:GetNitzTime need update by SNTP");
            MMIAPIPHONE_SetAttachingNitzSIM(dual_sys);
            MMIAPIAUT_UpdateNitzTime(dual_sys);
        }
    }
}

/*****************************************************************************/
// 	Description : get last sim card id which exist nitz
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMIAPIPHONE_GetExistNitzSIM(void)
{
    SCI_TRACE_LOW("[mmiphone]: GetExistNitzSIM s_last_exist_nitz_sim = %d",s_auto_update_time_sim.last_exist_nitz_sim);
    return s_auto_update_time_sim.last_exist_nitz_sim;
}

/*****************************************************************************/
// 	Description : get last sim card id which exist nitz
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetExistNitzSIM(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("[mmiphone]:SetExistNitzSIM s_last_exist_nitz_sim = %d",dual_sys);
    s_auto_update_time_sim.last_exist_nitz_sim = dual_sys;
}

/*****************************************************************************/
// 	Description : get open update time flag
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPHONE_IsAttachTimeFinished(void)
{
    SCI_TRACE_LOW("MMIAPIPHONE IsOpenUpdateTime is_attach_nitz_time_finish = %d",s_auto_update_time_sim.is_attach_nitz_time_finish);
    return s_auto_update_time_sim.is_attach_nitz_time_finish;
}

/*****************************************************************************/
// 	Description : set open update time flag
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetAttachNitzTimeFinish(BOOLEAN is_finished)
{
    SCI_TRACE_LOW("[mmiphone]:SetOpenUpdateTime is_attach_nitz_time_finish = %d",is_finished);
    s_auto_update_time_sim.is_attach_nitz_time_finish = is_finished;
}

/*****************************************************************************/
// 	Description : get attaching nitz SIM index
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMIAPIPHONE_GetAttachingNitzSIM(void)
{
    SCI_TRACE_LOW("[mmiphone]: GetAttachingNitzSIM attaching_nitz_sim = %d",s_auto_update_time_sim.attaching_nitz_sim);
    return s_auto_update_time_sim.attaching_nitz_sim;
}

/*****************************************************************************/
// 	Description : set attaching nitz SIM index
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetAttachingNitzSIM(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("[mmiphone]:SetExistNitzSIM attaching_nitz_sim = %d",dual_sys);
    s_auto_update_time_sim.attaching_nitz_sim = dual_sys;
}
#endif/*MMI_UPDATE_TIME_WHEN_STARTUP*/
#endif/*MMI_UPDATE_TIME_SUPPORT*/

/*****************************************************************************/
// 	Description : init nitz info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNITZ(void)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //从NV 读取network name    
    MMINV_READ(MMINV_NETWORK_NAME,&s_network_name_from_nitz,return_value);
    if(MN_RETURN_SUCCESS !=return_value)
    {
        SCI_MEMSET(&s_network_name_from_nitz,0,sizeof(MMIPHONE_NV_NETWORK_NAME_T)*MMI_DUAL_SYS_MAX);
        MMINV_WRITE(MMINV_NETWORK_NAME,s_network_name_from_nitz);
    }

    SCI_MEMSET(&s_tick_time, 0x00, sizeof(TICK_TIME_INFO_T));
	
#ifdef MMI_UPDATE_TIME_WHEN_STARTUP    
    s_auto_update_time_sim.last_exist_nitz_sim = MMI_DUAL_SYS_MAX;//set as no sim
    s_auto_update_time_sim.attaching_nitz_sim = MMI_DUAL_SYS_MAX;//set as no sim
    s_auto_update_time_sim.is_attach_nitz_time_finish  = FALSE;
#endif	
    s_tz_dst_info.daylight_saving_time = 0;
    s_tz_dst_info.daylight_saving_time_flag = FALSE;
    s_tz_dst_info.daylight_saving_time = 0;
}

/*****************************************************************************/
// 	Description : get NV Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NV_NETWORK_NAME_T* MMIPHONE_GetNVNetworkNameInfo(void)
{
    return s_network_name_from_nitz;
}

/*****************************************************************************/
// 	Description : Store Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:only update and store network name, do not Parse name string 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_StoreNetworkNameInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dual_sys = 0;//coverity 12718
    MMIPHONE_NV_NETWORK_NAME_T* nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
#ifdef MMI_NETWORK_NAME_SUPPORT
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
#endif
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMIPHONE_NV_NETWORK_NAME_T temp_network_name[MMI_DUAL_SYS_MAX] = {0};
    //coverity 12718
    if (PNULL == network_info_ind )
    {
        return MMI_RESULT_FALSE;
    }
	dual_sys = network_info_ind->dual_sys;
	if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return MMI_RESULT_FALSE;
    }
    
    SCI_TRACE_LOW("[mmiphone]:StoreNetworkNameInfo dual_sys=%d, full_name =%d,  short_name =%d ",
        dual_sys, network_info_ind->full_name.network_name_exist, network_info_ind->short_name.network_name_exist);

    /*The MS shall be free to choose one of these names depending upon its own characteristics and/or limitations, 
    such as those of its display*/
    if(network_info_ind->full_name.network_name_exist)
    {
        nv_network_name[dual_sys].is_opn_exist = TRUE;
        nv_network_name[dual_sys].opn_dcs = (MN_OPN_DCS_E)network_info_ind->full_name.code_scheme;
        nv_network_name[dual_sys].opn_len  = network_info_ind->full_name.length;
        nv_network_name[dual_sys].is_add_ci = network_info_ind->full_name.add_ci;
        SCI_MEMCPY(nv_network_name[dual_sys].opn, network_info_ind->full_name.name, network_info_ind->full_name.length);
    }
    else if(network_info_ind->short_name.network_name_exist)
    {
        nv_network_name[dual_sys].is_opn_exist = TRUE;
        nv_network_name[dual_sys].opn_dcs = (MN_OPN_DCS_E)network_info_ind->short_name.code_scheme;
        nv_network_name[dual_sys].opn_len  = network_info_ind->short_name.length;
        nv_network_name[dual_sys].is_add_ci = network_info_ind->short_name.add_ci;
        SCI_MEMCPY(nv_network_name[dual_sys].opn, network_info_ind->short_name.name, network_info_ind->short_name.length);
    }
    else
    {
        nv_network_name[dual_sys].is_opn_exist = FALSE;
        nv_network_name[dual_sys].opn_dcs = 0;
        nv_network_name[dual_sys].opn_len  = 0;
        nv_network_name[dual_sys].is_add_ci = 0;
        SCI_MEMSET(nv_network_name[dual_sys].opn, 0x00,MAX_ONSHORTFORM_LEN);
    }

    MMINV_READ(MMINV_NETWORK_NAME, temp_network_name, return_value);
    
    if (MN_RETURN_SUCCESS != return_value 
    ||((0 != memcmp(temp_network_name[dual_sys].opn, nv_network_name[dual_sys].opn, nv_network_name[dual_sys].opn_len))
            ||(temp_network_name[dual_sys].opn_dcs != nv_network_name[dual_sys].opn_dcs)
            ||(temp_network_name[dual_sys].opn_len != nv_network_name[dual_sys].opn_len))
    )
    {
        MMINV_WRITE(MMINV_NETWORK_NAME,nv_network_name);
    }
    
    //set network name, store plmn
#ifdef MMI_NETWORK_NAME_SUPPORT
    network_name[dual_sys].opn_dcs = nv_network_name[dual_sys].opn_dcs;
    network_name[dual_sys].opn_len = nv_network_name[dual_sys].opn_len;
    network_name[dual_sys].is_add_ci = nv_network_name[dual_sys].is_add_ci;
    SCI_MEMCPY(network_name[dual_sys].opn,nv_network_name[dual_sys].opn,MIN(network_name[dual_sys].opn_len,MAX_ONSHORTFORM_LEN));
#endif    

    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_INFO_IND
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:store, update network name,and update mm nitz time，full name和short name
//          ,与opn状态相同,在处理opn时存NV
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetworkInfoInd(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    APP_MN_NETWORK_INFO_IND_T network_info_ind = *((APP_MN_NETWORK_INFO_IND_T *)param);
    
    // 保存网络状态信息,
    MMIPHONE_StoreNetworkNameInfo(&network_info_ind);
    //set idle string
    MAIN_SetIdleNetWorkName();

#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMIPHONE_HandleNetworkTimeInfoInd(&network_info_ind);
#endif /*MMI_UPDATE_TIME_SUPPORT*/

    return (result);
}
/*****************************************************************************/
//  Description : nitz time is updated
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNITZUpdated(void)
{
   SCI_TRACE_LOW("[MMIPHONE]: MMIAPIPHONE_IsNITZUpdated is_net_time_exist = %d, is_tick_time_exist = %d", s_tick_time.is_net_time_exist, s_tick_time.is_tick_time_exist);
   return s_tick_time.is_net_time_exist && s_tick_time.is_tick_time_exist;
}

