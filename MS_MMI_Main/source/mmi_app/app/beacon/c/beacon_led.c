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
#ifndef  _MMI_BEACON_LED_C_
#define _MMI_BEACON_LED_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "beacon_app.h"

#define BEACONLED_TRACE            SCI_TRACE_LOW

static uint8                       s_led_red_timer_id       = 0;
static BOOLEAN s_led_red_is_on = FALSE;
static uint32 s_cur_red_led_close_times = 0;
static uint32 s_cur_red_led_open_times = 0;
static uint32 s_cur_red_led_num = 0;
static uint8 s_cur_red_led_status = 0;

static uint8                       s_led_blue_timer_id       = 0;
static BOOLEAN s_led_blue_is_on = FALSE;
static uint32 s_cur_blue_led_close_times = 0;
static uint32 s_cur_blue_led_open_times = 0;
static uint32 s_cur_blue_led_num = 0;
static uint8 s_cur_blue_led_status = 0;

static uint8                       s_led_yellow_timer_id       = 0;
static BOOLEAN s_led_yellow_is_on = FALSE;
static uint32 s_cur_yellow_led_close_times = 0;
static uint32 s_cur_yellow_led_open_times = 0;
static uint32 s_cur_yellow_led_num = 0;
static uint8 s_cur_yellow_led_status = 0;

static uint8                       s_led_all_timer_id       = 0;
static BOOLEAN s_led_all_is_on = FALSE;
static uint32 s_cur_all_led_close_times = 0;
static uint32 s_cur_all_led_open_times = 0;
static uint32 s_cur_all_led_num = 0;
static uint8 s_cur_all_led_status = 0;
static uint32 s_cur_all_led_open_num = 0;


static BOOLEAN s_need_sms_led = FALSE;
static BOOLEAN s_led_is_in_call_incomming = FALSE;

BOOLEAN g_need_misscall_led = FALSE;

uint8       s_beacon_led_charge_is_on   = 1;
uint8       s_beacon_led_sms_is_on   = 1;
uint8       s_beacon_led_call_is_on   = 1;
uint8       s_beacon_led_power_is_on   = 1;
uint8       s_beacon_led_flip_is_on   = 0;

extern void BEACON_LedRed_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void BEACON_LedBlue_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void BEACON_LedYellow_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void BEACON_LedAll_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void LED_Call_Switch(BOOLEAN onoff);
extern void LED_SMS_Switch(BOOLEAN onoff);
extern void LED_Charge_Switch(BOOLEAN onoff);
uint8 BEACON_Led_Start_SMS_Normal(void);
uint8 BEACON_Led_Start_Charge_Normal(void);
uint8 BEACON_Led_Start_Call_Normal(void);
uint8 BEACON_LedAll_Start_Normal(void);
extern uint8 BEACON_LedRed_Stop(void);
extern uint8 BEACON_LedBlue_Stop(void);
extern uint8 BEACON_LedYellow_Stop(void);

BOOLEAN BEACON_LedRed_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_led_red_timer_id)
    {
        MMK_StopTimer(s_led_red_timer_id);
        s_led_red_timer_id = 0;
    }

    s_led_red_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        BEACON_LedRed_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN BEACON_LedRed_StopTimer(void)
{
    if (s_led_red_timer_id != 0)
    {
        MMK_StopTimer(s_led_red_timer_id);
        s_led_red_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOLEAN BEACON_LedRed_IsOn(void)
{
    BOOLEAN res = FALSE;
    if(s_led_red_is_on)
    {
        res = TRUE;
    }
    else
    {
        if(s_cur_red_led_status > 0)
        {
            res = TRUE;
        }
    }
    return res;
}

uint8 BEACON_LedRed_OnOff(BOOLEAN is_on)
{
    if(s_led_red_is_on == is_on)
    {
        return 0;
    }
    //BEACONLED_TRACE("BEACON_LedRed_OnOff is_on=%d",is_on);
    s_led_red_is_on = is_on;
    #ifndef WIN32
        if(is_on)
        {
            LED_Call_Switch(1);
        }
        else
        {
            LED_Call_Switch(0);
        }
    #endif
   return 1;
}

uint8 BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    BEACONLED_TRACE("LED_LED: BEACON_LedRed_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_red_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    BEACON_LedRed_StopTimer();
                    s_cur_red_led_open_times = 0;
                    s_cur_red_led_close_times = 0;
                    s_cur_red_led_num = 0;
                    s_cur_red_led_status = 0;
                    BEACON_LedRed_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                BEACON_LedRed_StopTimer();
                s_cur_red_led_open_times = 0;
                s_cur_red_led_close_times = 0;
                s_cur_red_led_num = 0;
                s_cur_red_led_status = 1;
                BEACON_LedRed_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_red_led_open_times = on_time;
                s_cur_red_led_close_times = off_time;
                s_cur_red_led_num = num;
                BEACON_LedRed_StopTimer();
                s_cur_red_led_status = 1;
                if(s_led_red_is_on)
                {
                    BEACON_LedRed_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                BEACON_LedRed_OnOff(TRUE);
                BEACON_LedRed_StartTimer(s_cur_red_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void BEACON_LedRed_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    uint32 cur_times = s_cur_red_led_close_times;
    if(s_led_red_is_on == FALSE)
    {
        if(s_cur_red_led_num > 0)
        {
            if(s_cur_red_led_num != 0xFFFF)
            {
                s_cur_red_led_num--;
            }
        }
        cur_times = s_cur_red_led_open_times;
    }
    if(s_cur_red_led_num > 0)
    {
        BEACON_LedRed_OnOff(!s_led_red_is_on);
        BEACON_LedRed_StartTimer(cur_times);
    }
    else
    {
        s_cur_red_led_status = 0;
        BEACON_LedRed_OnOff(FALSE);
    }
}

uint8 BEACON_LedRed_Start_Normal(void)
{
    BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 BEACON_LedRed_On(void)
{
    BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 BEACON_LedRed_Stop(void)
{
    BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN BEACON_LedBlue_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_led_blue_timer_id)
    {
        MMK_StopTimer(s_led_blue_timer_id);
        s_led_blue_timer_id = 0;
    }

    s_led_blue_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        BEACON_LedBlue_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);

    return TRUE;
}

BOOLEAN BEACON_LedBlue_StopTimer(void)
{
    if (s_led_blue_timer_id != 0)
    {
        MMK_StopTimer(s_led_blue_timer_id);
        s_led_blue_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOLEAN BEACON_LedBlue_IsOn(void)
{
    BOOLEAN res = FALSE;
    if(s_led_blue_is_on)
    {
        res = TRUE;
    }
    else
    {
        if(s_cur_blue_led_status > 0)
        {
            res = TRUE;
        }
    }
    return res;
}

uint8 BEACON_LedBlue_OnOff(BOOLEAN is_on)
{
    if(s_led_blue_is_on == is_on)
    {
        return 0;
    }
    //BEACONLED_TRACE("BEACON_LedBlue_OnOff is_on=%d",is_on);
    s_led_blue_is_on = is_on;
    #ifndef WIN32
        if(is_on)
        {
            LED_SMS_Switch(1);
        }
        else
        {
            LED_SMS_Switch(0);
        }
    #endif
   return 1;
}

uint8 BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    BEACONLED_TRACE("LED_LED: BEACON_LedBlue_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_blue_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    BEACON_LedBlue_StopTimer();
                    s_cur_blue_led_open_times = 0;
                    s_cur_blue_led_close_times = 0;
                    s_cur_blue_led_num = 0;
                    s_cur_blue_led_status = 0;
                    BEACON_LedBlue_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                BEACON_LedBlue_StopTimer();
                s_cur_blue_led_open_times = 0;
                s_cur_blue_led_close_times = 0;
                s_cur_blue_led_num = 0;
                s_cur_blue_led_status = 1;
                BEACON_LedBlue_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_blue_led_open_times = on_time;
                s_cur_blue_led_close_times = off_time;
                s_cur_blue_led_num = num;
                BEACON_LedBlue_StopTimer();
                s_cur_blue_led_status = 1;
                if(s_led_blue_is_on)
                {
                    BEACON_LedBlue_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                BEACON_LedBlue_OnOff(TRUE);
                BEACON_LedBlue_StartTimer(s_cur_blue_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void BEACON_LedBlue_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    uint32 cur_times = s_cur_blue_led_close_times;
    if(s_led_blue_is_on == FALSE)
    {
        if(s_cur_blue_led_num > 0)
        {
            if(s_cur_blue_led_num != 0xFFFF)
            {
                s_cur_blue_led_num--;
            }
        }
        cur_times = s_cur_blue_led_open_times;
    }
    if(s_cur_blue_led_num > 0)
    {
        BEACON_LedBlue_OnOff(!s_led_blue_is_on);
        BEACON_LedBlue_StartTimer(cur_times);
    }
    else
    {
        s_cur_blue_led_status = 0;
        BEACON_LedBlue_OnOff(FALSE);
    }
}

uint8 BEACON_LedBlue_Start_Normal(void)
{
    BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 BEACON_LedBlue_On(void)
{
    BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 BEACON_LedBlue_Stop(void)
{
    BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN BEACON_LedYellow_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_led_yellow_timer_id)
    {
        MMK_StopTimer(s_led_yellow_timer_id);
        s_led_yellow_timer_id = 0;
    }

    s_led_yellow_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        BEACON_LedYellow_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);

    return TRUE;
}

BOOLEAN BEACON_LedYellow_StopTimer(void)
{
    if (s_led_yellow_timer_id != 0)
    {
        MMK_StopTimer(s_led_yellow_timer_id);
        s_led_yellow_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOLEAN BEACON_LedYellow_IsOn(void)
{
    BOOLEAN res = FALSE;
    if(s_led_yellow_is_on)
    {
        res = TRUE;
    }
    else
    {
        if(s_cur_yellow_led_status > 0)
        {
            res = TRUE;
        }
    }
    return res;
}

uint8 BEACON_LedYellow_OnOff(BOOLEAN is_on)
{
    if(s_led_yellow_is_on == is_on)
    {
        return 0;
    }
    s_led_yellow_is_on = is_on;
    #ifndef WIN32
        if(is_on)
        {
            LED_Charge_Switch(1);
        }
        else
        {
            LED_Charge_Switch(0);
        }
    #endif
   return 1;
}

uint8 BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    BEACONLED_TRACE("LED_LED: BEACON_LedYellow_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_yellow_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    BEACON_LedYellow_StopTimer();
                    s_cur_yellow_led_open_times = 0;
                    s_cur_yellow_led_close_times = 0;
                    s_cur_yellow_led_num = 0;
                    s_cur_yellow_led_status = 0;
                    BEACON_LedYellow_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                BEACON_LedYellow_StopTimer();
                s_cur_yellow_led_open_times = 0;
                s_cur_yellow_led_close_times = 0;
                s_cur_yellow_led_num = 0;
                s_cur_yellow_led_status = 1;
                BEACON_LedYellow_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_yellow_led_open_times = on_time;
                s_cur_yellow_led_close_times = off_time;
                s_cur_yellow_led_num = num;
                BEACON_LedYellow_StopTimer();
                s_cur_yellow_led_status = 1;
                if(s_led_yellow_is_on)
                {
                    BEACON_LedYellow_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                BEACON_LedYellow_OnOff(TRUE);
                BEACON_LedYellow_StartTimer(s_cur_yellow_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void BEACON_LedYellow_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    uint32 cur_times = s_cur_yellow_led_close_times;
    if(s_led_yellow_is_on == FALSE)
    {
        if(s_cur_yellow_led_num > 0)
        {
            s_cur_yellow_led_num--;
        }
        cur_times = s_cur_yellow_led_open_times;
    }
    if(s_cur_yellow_led_num > 0)
    {
        BEACON_LedYellow_OnOff(!s_led_yellow_is_on);
        BEACON_LedYellow_StartTimer(cur_times);
    }
    else
    {
        s_cur_yellow_led_status = 0;
        BEACON_LedYellow_OnOff(FALSE);
    }

}

uint8 BEACON_LedYellow_Start_Normal(void)
{
    BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 BEACON_LedYellow_On(void)
{
    BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 BEACON_LedYellow_Stop(void)
{
    BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN BEACON_LedAll_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_led_all_timer_id)
    {
        MMK_StopTimer(s_led_all_timer_id);
        s_led_all_timer_id = 0;
    }

    s_led_all_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        BEACON_LedAll_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN BEACON_LedAll_StopTimer(void)
{
    if (s_led_all_timer_id != 0)
    {
        MMK_StopTimer(s_led_all_timer_id);
        s_led_all_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOLEAN BEACON_LedAll_IsOn(void)
{
    BOOLEAN res = FALSE;
    if(s_led_all_is_on)
    {
        res = TRUE;
    }
    else
    {
        if(s_cur_all_led_status > 0)
        {
            res = TRUE;
        }
    }
    return res;
}

uint8 BEACON_LedAll_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    BEACONLED_TRACE("LED_LED: BEACON_LedAll_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_all_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    BEACON_LedAll_StopTimer();
                    s_cur_all_led_open_times = 0;
                    s_cur_all_led_close_times = 0;
                    s_cur_all_led_num = 0;
                    s_cur_all_led_status = 0;
                    s_cur_all_led_open_num = 0;
                    s_led_all_is_on = FALSE;
                    LED_Call_Switch(0);
                    LED_SMS_Switch(0);
                    LED_Charge_Switch(0);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                BEACON_LedAll_StopTimer();
                s_cur_all_led_open_times = 0;
                s_cur_all_led_close_times = 0;
                s_cur_all_led_num = 0;
                s_cur_all_led_status = 1;
                s_cur_all_led_open_num = 0;
                s_led_all_is_on = TRUE;
                LED_Call_Switch(1);
                LED_SMS_Switch(1);
                LED_Charge_Switch(1);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_all_led_open_times = on_time;
                s_cur_all_led_close_times = off_time;
                s_cur_all_led_num = num;
                BEACON_LedAll_StopTimer();
                s_cur_all_led_status = 1;
                if(s_led_all_is_on)
                {
                    LED_Call_Switch(0);
                    LED_SMS_Switch(0);
                    LED_Charge_Switch(0);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                s_cur_all_led_open_num = 0;
                s_led_all_is_on = TRUE;
                LED_Call_Switch(1);
                LED_SMS_Switch(1);
                LED_Charge_Switch(1);
                BEACON_LedAll_StartTimer(s_cur_all_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void BEACON_LedAll_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    uint32 cur_times = s_cur_all_led_close_times;    
    s_cur_all_led_open_num++;
    if(s_cur_all_led_open_num == 1)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_Call_Switch(0);
        LED_SMS_Switch(0);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 2)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_Call_Switch(1);
        LED_SMS_Switch(1);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 3)
    {
        s_led_all_is_on = FALSE;
        LED_Call_Switch(0);
        LED_SMS_Switch(0);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 4)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_Call_Switch(1);
        LED_SMS_Switch(1);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 5)
    {
        s_led_all_is_on = FALSE;
        LED_Call_Switch(0);
        LED_SMS_Switch(0);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 6)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_Call_Switch(1);
        LED_SMS_Switch(1);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 7)
    {
        s_led_all_is_on = FALSE;
        LED_Call_Switch(0);
        LED_SMS_Switch(0);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 8)
    {
        s_cur_all_led_open_num = 0;
        if(s_cur_all_led_num > 0)
        {
            if(s_cur_all_led_num != 0xFFFF)
            {
                s_cur_all_led_num--;
            }
            if(s_cur_all_led_num > 0)
            {
                s_led_all_is_on = TRUE;
                cur_times = s_cur_all_led_open_times;
                LED_Call_Switch(1);
                LED_SMS_Switch(1);
                LED_Charge_Switch(1);
            }
        }
    }
    if(s_cur_all_led_num > 0)
    {
        BEACON_LedAll_StartTimer(cur_times);
    }
    else
    {
        s_led_all_is_on = FALSE;
        s_cur_all_led_status = 0;
        BEACON_LedAll_Start_Normal();
    }
}

uint8 BEACON_LedAll_Start_Flash(uint32 num)
{
#ifndef WIN32
    //SCI_LED_EnableDeepSleep(0);
#endif
    BEACON_LedRed_Stop();
    BEACON_LedBlue_Stop();
    BEACON_LedYellow_Stop();
    BEACON_LedAll_Open(LED_LIGHT_OPEN_TYPE_TIMES,120,120,num);
    return 0;
}

uint8 BEACON_LedAll_Start_Normal(void)
{

    BEACON_Led_Start_SMS_Normal();
    BEACON_Led_Start_Charge_Normal();
    BEACON_Led_Start_Call_Normal();
#ifndef WIN32
    //SCI_LED_EnableDeepSleep(1);
#endif
    return 0;
}

uint8 BEACON_LedAll_On(void)
{
    BEACON_LedRed_Stop();
    BEACON_LedBlue_Stop();
    BEACON_LedYellow_Stop();
    BEACON_LedAll_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 BEACON_LedAll_Stop(void)
{
    BEACON_LedRed_Stop();
    BEACON_LedBlue_Stop();
    BEACON_LedYellow_Stop();
    BEACON_LedAll_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

uint8 BEACON_Led_Start_PowerOn(void)
{
    if(MMIBEACON_LED_GetPowerSwitch())
    {
        BEACON_LedAll_Start_Flash(0xFFFF);
    }
    return 0;
}
uint8 BEACON_Led_Start_PowerOnOver(void)
{
    BEACON_LedAll_Stop();
    BEACON_LedAll_Start_Normal();
    return 0;
}

uint8 BEACON_Led_Start_PowerOff(void)
{
    if(MMIBEACON_LED_GetPowerSwitch())
    {
        BEACON_LedAll_Start_Flash(0xFFFF);
    }
    return 0;
}

uint8 BEACON_Led_Start_PowerOffOver(void)
{
    BEACON_LedAll_Stop();
#ifndef WIN32
    //SCI_LED_EnableDeepSleep(1);
#endif
    //BEACON_LedAll_Start_Normal();
    return 0;
}

uint8 BEACON_Led_Start_ChargeLow(void)
{
    return 0;
}

uint8 BEACON_Led_Start_Charge(void)
{
    if(MMIBEACON_LED_GetChargeSwitch())
    {
        if(FALSE == BEACON_LedAll_IsOn())
        {
            BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_ON,1000,3000,0xFFFF);
        }
    }
    return 0;
}

uint8 BEACON_Led_Start_ChargeOver(void)
{
    if(FALSE == BEACON_LedAll_IsOn())
    {
        BEACON_LedYellow_Stop();
    }
    return 0;
}

uint8 BEACON_Led_Start_ChargeFull(void)
{
    if(MMIBEACON_LED_GetChargeSwitch())
    {
        if(FALSE == BEACON_LedAll_IsOn())
        {
            //BEACON_LedYellow_On();
            BEACON_LedYellow_Stop();
        }
    }
    return 0;
}

uint8 BEACON_Led_Start_Charge_Normal(void)
{
    if(MMIBEACON_LED_GetChargeSwitch())
    {
        if(BEACON_GetIsCharge())
        {
            if(BEACON_GetIsBatFull())
            {
                BEACON_Led_Start_ChargeFull();
            }
            else
            {
                BEACON_Led_Start_Charge();
            }
        }
        else
        {
            //if(BEACON_GetIsBatLow())
            if(0)
            {
                BEACON_Led_Start_ChargeLow();
            }
            else
            {
                BEACON_Led_Start_ChargeOver();
            }
        }
    }
    else
    {
        BEACON_Led_Start_ChargeOver();
    }
    return 0;
}

uint8 BEACON_Led_Start_Call_Incoming(void)
{
    if(MMIBEACON_LED_GetCallSwitch())
    {
        s_led_is_in_call_incomming = TRUE;
        if(FALSE == BEACON_LedAll_IsOn())
        {
            BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,200,200,0xFFFF);
        }
        else
        {
            BEACON_LedAll_Stop();
            BEACON_Led_Start_SMS_Normal();
            BEACON_Led_Start_Charge_Normal();
            BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,200,200,0xFFFF);
        }
    }
    return 0;
}

uint8 BEACON_Led_Start_Call_Incoming_Over(void)
{
    if(s_led_is_in_call_incomming)
    {
        if(FALSE == BEACON_LedAll_IsOn())
        {
            if(g_need_misscall_led)
            {
                BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
            }
            else
            {
                BEACON_LedRed_Stop();
            }
        }
        s_led_is_in_call_incomming = FALSE;
    }
    return 0;
}

uint8 BEACON_Led_Start_Call_Miss(void)
{
    if(MMIBEACON_LED_GetCallSwitch())
    {
        g_need_misscall_led = TRUE;
        if(FALSE == BEACON_LedAll_IsOn())
        {
            if(s_led_is_in_call_incomming == FALSE)
            {
                BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
            }
        }
    }
    return 0;
}

uint8 BEACON_Led_Start_Call_Miss_Over(void)
{
    if(g_need_misscall_led)
    {
        if(FALSE == BEACON_LedAll_IsOn() && s_led_is_in_call_incomming == FALSE)
        {
            BEACON_LedRed_Stop();
        }
        g_need_misscall_led = FALSE;
    }
    return 0;
}

uint8 BEACON_Led_Start_Call_Normal(void)
{
    if(MMIBEACON_LED_GetCallSwitch())
    {
        if(s_led_is_in_call_incomming)
        {
            BEACON_Led_Start_Call_Incoming();
        }
        else if(g_need_misscall_led)
        {
            BEACON_Led_Start_Call_Miss();
        }
        else
        {
            BEACON_LedRed_Stop();
        }
    }
    else
    {
        BEACON_LedRed_Stop();
    }
    return 0;
}

uint8 BEACON_Led_Start_SMS_UnRead(void)
{
    s_need_sms_led = TRUE;
    if(MMIBEACON_LED_GetSmsSwitch())
    {
        if(FALSE == BEACON_LedAll_IsOn())
        {
            BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
        }
    }
    return 0;
}

uint8 BEACON_Led_Start_SMS_UnRead_OverExt(void)
{
    if(FALSE == BEACON_LedAll_IsOn())
    {
        BEACON_LedBlue_Stop();
    }
    return 0;
}


uint8 BEACON_Led_Start_SMS_UnRead_Over(void)
{
    s_need_sms_led = FALSE;
    BEACON_Led_Start_SMS_UnRead_OverExt();
    return 0;
}


uint8 BEACON_Led_Start_SMS_Normal(void)
{
    if(MMIBEACON_LED_GetSmsSwitch())
    {
        if(s_need_sms_led)
        {
            BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
        }
        else
        {
            BEACON_LedBlue_Stop();
        }
    }
    else
    {
        BEACON_LedBlue_Stop();
    }
    
    return 0;
}

uint8 BEACON_Led_Start_Flip(void)
{
    if(MMIBEACON_LED_GetFlipSwitch())
    {
        BEACON_LedAll_Start_Flash(2);
    }
    return 0;
}

void LED_Call_Switch(BOOLEAN onoff)
{
      static uint8 s_is_led_call_on = 0xff;
      if(s_is_led_call_on == onoff)
      {
            return;
      }
      else
      {
            s_is_led_call_on = onoff;
      }
      #ifndef WIN32
	GPIO_SetLed_2(onoff);
      #endif
}

void LED_SMS_Switch(BOOLEAN onoff)
{
      static uint8 s_is_led_sms_on = 0xff;
      if(s_is_led_sms_on == onoff)
      {
            return;
      }
      else
      {
            s_is_led_sms_on = onoff;
      }

      #ifndef WIN32
      GPIO_SetLed_3(onoff);
      #endif
}

void LED_Charge_Switch(BOOLEAN onoff)
{
      static uint8 s_is_led_charge_on = 0xff;
      if(s_is_led_charge_on == onoff)
      {
            return;
      }
      else
      {
            s_is_led_charge_on = onoff;
      }
      #ifndef WIN32
	GPIO_SetLed_1(onoff);
      #endif
}

PUBLIC uint8 MMIBEACON_LED_NV_GetChargeSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_BEACON_NV_LED_CHARGE,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_BEACON_NV_LED_CHARGE,&is_on);
    }
    s_beacon_led_charge_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIBEACON_LED_NV_SetChargeSwitch(uint8 is_on) 
{
    if(s_beacon_led_charge_is_on != is_on)
    {
        MMINV_WRITE(MMI_BEACON_NV_LED_CHARGE,&is_on);
        s_beacon_led_charge_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIBEACON_LED_GetChargeSwitch(void)
{
    return s_beacon_led_charge_is_on;
}


PUBLIC uint8 MMIBEACON_LED_NV_GetSmsSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_BEACON_NV_LED_SMS,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_BEACON_NV_LED_SMS,&is_on);
    }
    s_beacon_led_sms_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIBEACON_LED_NV_SetSmsSwitch(uint8 is_on) 
{
    if(s_beacon_led_sms_is_on != is_on)
    {
        MMINV_WRITE(MMI_BEACON_NV_LED_SMS,&is_on);
        s_beacon_led_sms_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIBEACON_LED_GetSmsSwitch(void)
{
    return s_beacon_led_sms_is_on;
}

PUBLIC uint8 MMIBEACON_LED_NV_GetCallSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_BEACON_NV_LED_CALL,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_BEACON_NV_LED_CALL,&is_on);
    }
    s_beacon_led_call_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIBEACON_LED_NV_SetCallSwitch(uint8 is_on) 
{
    if(s_beacon_led_call_is_on != is_on)
    {
        MMINV_WRITE(MMI_BEACON_NV_LED_CALL,&is_on);
        s_beacon_led_call_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIBEACON_LED_GetCallSwitch(void)
{
    return s_beacon_led_call_is_on;
}

PUBLIC uint8 MMIBEACON_LED_NV_GetPowerSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_BEACON_NV_LED_POWER,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_BEACON_NV_LED_POWER,&is_on);
    }
    s_beacon_led_power_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIBEACON_LED_NV_SetPowerSwitch(uint8 is_on) 
{
    if(s_beacon_led_power_is_on != is_on)
    {
        MMINV_WRITE(MMI_BEACON_NV_LED_POWER,&is_on);
        s_beacon_led_power_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIBEACON_LED_GetPowerSwitch(void)
{
    return s_beacon_led_power_is_on;
}


PUBLIC uint8 MMIBEACON_LED_NV_GetFlipSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 0;
    MMINV_READ(MMI_BEACON_NV_LED_FLIP,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 0;
        MMINV_WRITE(MMI_BEACON_NV_LED_FLIP,&is_on);
    }
    s_beacon_led_flip_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIBEACON_LED_NV_SetFlipSwitch(uint8 is_on) 
{
    if(s_beacon_led_flip_is_on != is_on)
    {
        MMINV_WRITE(MMI_BEACON_NV_LED_FLIP,&is_on);
        s_beacon_led_flip_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIBEACON_LED_GetFlipSwitch(void)
{
    return s_beacon_led_flip_is_on;
}

#endif
