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
#ifndef  _MMI_ZDT_LED_C_
#define _MMI_ZDT_LED_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "zdt_app.h"
#include "zdt_led.h"

#define ZDTLED_TRACE            SCI_TRACE_LOW

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

uint8       s_zdt_led_charge_is_on   = 1;
uint8       s_zdt_led_sms_is_on   = 1;
uint8       s_zdt_led_call_is_on   = 1;
uint8       s_zdt_led_power_is_on   = 1;
uint8       s_zdt_led_flip_is_on   = 1;

extern void ZDT_LedRed_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void ZDT_LedBlue_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void ZDT_LedYellow_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void ZDT_LedAll_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                );
extern void LED_Call_Switch(BOOLEAN onoff);
extern void LED_SMS_Switch(BOOLEAN onoff);
extern void LED_Charge_Switch(BOOLEAN onoff);
uint8 ZDT_Led_Start_SMS_Normal(void);
uint8 ZDT_Led_Start_Charge_Normal(void);
uint8 ZDT_Led_Start_Call_Normal(void);
uint8 ZDT_LedAll_Start_Normal(void);
extern uint8 ZDT_LedRed_Stop(void);
extern uint8 ZDT_LedBlue_Stop(void);
extern uint8 ZDT_LedYellow_Stop(void);

BOOLEAN ZDT_LedRed_StartTimer(uint32 duaration)
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
                                                                        ZDT_LedRed_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN ZDT_LedRed_StopTimer(void)
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

BOOLEAN ZDT_LedRed_IsOn(void)
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

uint8 ZDT_LedRed_OnOff(BOOLEAN is_on)
{
    if(s_led_red_is_on == is_on)
    {
        return 0;
    }
    //ZDTLED_TRACE("ZDT_LedRed_OnOff is_on=%d",is_on);
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

uint8 ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    ZDTLED_TRACE("LED_LED: ZDT_LedRed_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_red_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    ZDT_LedRed_StopTimer();
                    s_cur_red_led_open_times = 0;
                    s_cur_red_led_close_times = 0;
                    s_cur_red_led_num = 0;
                    s_cur_red_led_status = 0;
                    ZDT_LedRed_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                ZDT_LedRed_StopTimer();
                s_cur_red_led_open_times = 0;
                s_cur_red_led_close_times = 0;
                s_cur_red_led_num = 0;
                s_cur_red_led_status = 1;
                ZDT_LedRed_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_red_led_open_times = on_time;
                s_cur_red_led_close_times = off_time;
                s_cur_red_led_num = num;
                ZDT_LedRed_StopTimer();
                s_cur_red_led_status = 1;
                if(s_led_red_is_on)
                {
                    ZDT_LedRed_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                ZDT_LedRed_OnOff(TRUE);
                ZDT_LedRed_StartTimer(s_cur_red_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void ZDT_LedRed_TimeOut(
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
        ZDT_LedRed_OnOff(!s_led_red_is_on);
        ZDT_LedRed_StartTimer(cur_times);
    }
    else
    {
        s_cur_red_led_status = 0;
        ZDT_LedRed_OnOff(FALSE);
    }
}

uint8 ZDT_LedRed_Start_Normal(void)
{
    ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 ZDT_LedRed_On(void)
{
    ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 ZDT_LedRed_Stop(void)
{
    ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN ZDT_LedBlue_StartTimer(uint32 duaration)
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
                                                                        ZDT_LedBlue_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);

    return TRUE;
}

BOOLEAN ZDT_LedBlue_StopTimer(void)
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

BOOLEAN ZDT_LedBlue_IsOn(void)
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

uint8 ZDT_LedBlue_OnOff(BOOLEAN is_on)
{
    if(s_led_blue_is_on == is_on)
    {
        return 0;
    }
    //ZDTLED_TRACE("ZDT_LedBlue_OnOff is_on=%d",is_on);
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

uint8 ZDT_LedBlue_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    ZDTLED_TRACE("LED_LED: ZDT_LedBlue_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_blue_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    ZDT_LedBlue_StopTimer();
                    s_cur_blue_led_open_times = 0;
                    s_cur_blue_led_close_times = 0;
                    s_cur_blue_led_num = 0;
                    s_cur_blue_led_status = 0;
                    ZDT_LedBlue_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                ZDT_LedBlue_StopTimer();
                s_cur_blue_led_open_times = 0;
                s_cur_blue_led_close_times = 0;
                s_cur_blue_led_num = 0;
                s_cur_blue_led_status = 1;
                ZDT_LedBlue_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_blue_led_open_times = on_time;
                s_cur_blue_led_close_times = off_time;
                s_cur_blue_led_num = num;
                ZDT_LedBlue_StopTimer();
                s_cur_blue_led_status = 1;
                if(s_led_blue_is_on)
                {
                    ZDT_LedBlue_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                ZDT_LedBlue_OnOff(TRUE);
                ZDT_LedBlue_StartTimer(s_cur_blue_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void ZDT_LedBlue_TimeOut(
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
        ZDT_LedBlue_OnOff(!s_led_blue_is_on);
        ZDT_LedBlue_StartTimer(cur_times);
    }
    else
    {
        s_cur_blue_led_status = 0;
        ZDT_LedBlue_OnOff(FALSE);
    }
}

uint8 ZDT_LedBlue_Start_Normal(void)
{
    ZDT_LedBlue_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 ZDT_LedBlue_On(void)
{
    ZDT_LedBlue_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 ZDT_LedBlue_Stop(void)
{
    ZDT_LedBlue_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN ZDT_LedYellow_StartTimer(uint32 duaration)
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
                                                                        ZDT_LedYellow_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);

    return TRUE;
}

BOOLEAN ZDT_LedYellow_StopTimer(void)
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

BOOLEAN ZDT_LedYellow_IsOn(void)
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

uint8 ZDT_LedYellow_OnOff(BOOLEAN is_on)
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

uint8 ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    ZDTLED_TRACE("LED_LED: ZDT_LedYellow_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_yellow_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    ZDT_LedYellow_StopTimer();
                    s_cur_yellow_led_open_times = 0;
                    s_cur_yellow_led_close_times = 0;
                    s_cur_yellow_led_num = 0;
                    s_cur_yellow_led_status = 0;
                    ZDT_LedYellow_OnOff(FALSE);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                ZDT_LedYellow_StopTimer();
                s_cur_yellow_led_open_times = 0;
                s_cur_yellow_led_close_times = 0;
                s_cur_yellow_led_num = 0;
                s_cur_yellow_led_status = 1;
                ZDT_LedYellow_OnOff(TRUE);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_yellow_led_open_times = on_time;
                s_cur_yellow_led_close_times = off_time;
                s_cur_yellow_led_num = num;
                ZDT_LedYellow_StopTimer();
                s_cur_yellow_led_status = 1;
                if(s_led_yellow_is_on)
                {
                    ZDT_LedYellow_OnOff(FALSE);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                ZDT_LedYellow_OnOff(TRUE);
                ZDT_LedYellow_StartTimer(s_cur_yellow_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void ZDT_LedYellow_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    uint32 cur_times = s_cur_yellow_led_close_times;
    if(s_led_yellow_is_on == FALSE)
    {
        if(s_cur_yellow_led_num > 0)
        {
            if(s_cur_yellow_led_num != 0xFFFF)
            {
                s_cur_yellow_led_num--;
            }
        }
        cur_times = s_cur_yellow_led_open_times;
    }
    if(s_cur_yellow_led_num > 0)
    {
        ZDT_LedYellow_OnOff(!s_led_yellow_is_on);
        ZDT_LedYellow_StartTimer(cur_times);
    }
    else
    {
        s_cur_yellow_led_status = 0;
        ZDT_LedYellow_OnOff(FALSE);
    }

}

uint8 ZDT_LedYellow_Start_Normal(void)
{
    ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_TIMES,500,10000,0xFFFF);
    return 0;
}

uint8 ZDT_LedYellow_On(void)
{
    ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 ZDT_LedYellow_Stop(void)
{
    ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

BOOLEAN ZDT_LedAll_StartTimer(uint32 duaration)
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
                                                                        ZDT_LedAll_TimeOut, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN ZDT_LedAll_StopTimer(void)
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

BOOLEAN ZDT_LedAll_IsOn(void)
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

uint8 ZDT_LedAll_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num)
{
    ZDTLED_TRACE("LED_LED: ZDT_LedAll_Open %d type=%d,on_time=%d,off_time=%d,num=%d", s_led_all_is_on,type,on_time,off_time,num);
    switch(type)
    {
        case LED_LIGHT_OPEN_TYPE_OFF:
                    ZDT_LedAll_StopTimer();
                    s_cur_all_led_open_times = 0;
                    s_cur_all_led_close_times = 0;
                    s_cur_all_led_num = 0;
                    s_cur_all_led_status = 0;
                    s_cur_all_led_open_num = 0;
                    s_led_all_is_on = FALSE;
                    LED_Call_Switch(0);
                    LED_SMS_Switch(0);
                    LED_Charge_Switch(0);
                    LED_Func_Switch(0);
                    return 0;
                break;

        case LED_LIGHT_OPEN_TYPE_ON:
                ZDT_LedAll_StopTimer();
                s_cur_all_led_open_times = 0;
                s_cur_all_led_close_times = 0;
                s_cur_all_led_num = 0;
                s_cur_all_led_status = 1;
                s_cur_all_led_open_num = 0;
                s_led_all_is_on = TRUE;
                LED_Call_Switch(1);
                LED_SMS_Switch(1);
                LED_Charge_Switch(1);
                LED_Func_Switch(1);
                return 0;
            break;
            
         case LED_LIGHT_OPEN_TYPE_TIMES:
                s_cur_all_led_open_times = on_time;
                s_cur_all_led_close_times = off_time;
                s_cur_all_led_num = num;
                ZDT_LedAll_StopTimer();
                s_cur_all_led_status = 1;
                if(s_led_all_is_on)
                {
                    LED_Call_Switch(0);
                    LED_SMS_Switch(0);
                    LED_Charge_Switch(0);
                    LED_Func_Switch(0);
                    #ifndef WIN32
                    SCI_SLEEP(10);
                    #endif
                }
                s_cur_all_led_open_num = 0;
                s_led_all_is_on = TRUE;
                LED_Call_Switch(1);
                LED_SMS_Switch(0);
                LED_Charge_Switch(1);
                LED_Func_Switch(1);
                ZDT_LedAll_StartTimer(s_cur_all_led_open_times);
            break;
          
                                   
        default:
            return 1;
    }
    
    return 0;
}

void ZDT_LedAll_TimeOut(
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
        LED_SMS_Switch(1);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 2)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(0);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 3)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(1);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 4)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(0);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 5)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(1);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 6)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(0);
        LED_Charge_Switch(1);
    }
    else if(s_cur_all_led_open_num == 7)
    {
        s_led_all_is_on = TRUE;
        cur_times = s_cur_all_led_open_times;
        LED_SMS_Switch(1);
        LED_Charge_Switch(0);
    }
    else if(s_cur_all_led_open_num == 8)
    {
        s_cur_all_led_open_num = 0;
        cur_times = s_cur_all_led_open_times;
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
                LED_SMS_Switch(0);
                LED_Charge_Switch(1);
            }
        }
    }
    if(s_cur_all_led_num > 0)
    {
        ZDT_LedAll_StartTimer(cur_times);
    }
    else
    {
        s_led_all_is_on = FALSE;
        s_cur_all_led_status = 0;
        ZDT_LedAll_Start_Normal();
    }
}

uint8 ZDT_LedAll_Start_Flash(uint32 num)
{
#ifndef WIN32
    //SCI_LED_EnableDeepSleep(0);
#endif
    ZDT_LedRed_Stop();
    ZDT_LedBlue_Stop();
    ZDT_LedYellow_Stop();
    ZDT_LedAll_Open(LED_LIGHT_OPEN_TYPE_TIMES,200,200,num);
    return 0;
}

uint8 ZDT_LedAll_Start_Normal(void)
{

    ZDT_Led_Start_SMS_Normal();
    ZDT_Led_Start_Charge_Normal();
    ZDT_Led_Start_Call_Normal();
#ifndef WIN32
    //SCI_LED_EnableDeepSleep(1);
#endif
    return 0;
}

uint8 ZDT_LedAll_On(void)
{
    ZDT_LedRed_Stop();
    ZDT_LedBlue_Stop();
    ZDT_LedYellow_Stop();
    ZDT_LedAll_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 ZDT_LedAll_Stop(void)
{
    ZDT_LedRed_Stop();
    ZDT_LedBlue_Stop();
    ZDT_LedYellow_Stop();
    ZDT_LedAll_Open(LED_LIGHT_OPEN_TYPE_OFF,0,0,0);
    return 0;
}

uint8 ZDT_Led_Start_PowerOn(void)
{
    //if(MMIZDT_LED_GetPowerSwitch())
    {
        ZDT_LedAll_Start_Flash(0xFFFF);
    }
    return 0;
}
uint8 ZDT_Led_Start_PowerOnOver(void)
{
    ZDT_LedAll_Stop();
    ZDT_LedAll_Start_Normal();
    return 0;
}

uint8 ZDT_Led_Start_PowerOff(void)
{
    s_led_all_is_on = TRUE;
    ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_ON,0,0,0);
    return 0;
}

uint8 ZDT_Led_Start_PowerOffOver(void)
{
    ZDT_LedAll_Stop();
    ZDT_LedAll_Start_Normal();
    return 0;
}

uint8 ZDT_Led_Start_ChargeLow(void)
{
    return 0;
}

uint8 ZDT_Led_Start_Charge(void)
{
    if(s_led_is_in_call_incomming)
    {
        return 1;
    }
    //if(MMIZDT_LED_GetChargeSwitch())
    if(1)
    {
        if(FALSE == ZDT_LedAll_IsOn())
        {
            ZDT_Led_Start_SMS_UnRead_Over();
            ZDT_LedYellow_Stop();
            ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_TIMES,300,1000,0xFFFF);
        }
    }
    return 0;
}

uint8 ZDT_Led_Start_ChargeOver(void)
{
    if(s_led_is_in_call_incomming)
    {
        return 1;
    }
    if(FALSE == ZDT_LedAll_IsOn())
    {
        ZDT_LedYellow_Stop();
        ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_TIMES,300,2000,0xFFFF);
        //ZDT_Led_Start_SMS_UnRead();
    }
    return 0;
}

uint8 ZDT_Led_Start_ChargeFull(void)
{
    //if(MMIZDT_LED_GetChargeSwitch())
    if(s_led_is_in_call_incomming)
    {
        return 1;
    }
    if(1)
    {
        if(FALSE == ZDT_LedAll_IsOn())
        {
            ZDT_LedYellow_On();
        }
    }
    return 0;
}

uint8 ZDT_Led_Start_Charge_Normal(void)
{
    //if(MMIZDT_LED_GetChargeSwitch())
    if(1)
    {
        if(ZDT_GetIsCharge())
        {
            if(ZDT_GetIsBatFull())
            {
                ZDT_Led_Start_ChargeFull();
            }
            else
            {
                ZDT_Led_Start_Charge();
            }
        }
        else
        {
            ZDT_Led_Start_ChargeOver();
        }
    }
    else
    {
        ZDT_Led_Start_ChargeOver();
    }
    return 0;
}

uint8 ZDT_Led_Start_Call_Incoming(void)
{
    //if(MMIZDT_LED_GetCallSwitch())
    {
        s_led_is_in_call_incomming = TRUE;
        //if(FALSE == ZDT_LedAll_IsOn())
        if(1)
        {
            ZDT_LedYellow_Stop();
            ZDT_LedYellow_Open(LED_LIGHT_OPEN_TYPE_TIMES,200,200,0xFFFF);
        }
        else
        {
            ZDT_LedAll_Stop();
            ZDT_Led_Start_SMS_Normal();
            ZDT_Led_Start_Charge_Normal();
            ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,200,200,0xFFFF);
        }
    }
    return 0;
}

uint8 ZDT_Led_Start_Call_Incoming_Over(void)
{
    if(s_led_is_in_call_incomming)
    {
        if(FALSE == ZDT_LedAll_IsOn())
        {
            if(g_need_misscall_led)
            {
                ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
            }
            else
            {
                ZDT_LedRed_Stop();
            }
        }
        s_led_is_in_call_incomming = FALSE;
        ZDT_Led_Start_Charge_Normal();
    }
    return 0;
}

uint8 ZDT_Led_Start_Call_Miss(void)
{
    if(MMIZDT_LED_GetCallSwitch())
    {
        g_need_misscall_led = TRUE;
        if(FALSE == ZDT_LedAll_IsOn())
        {
            if(s_led_is_in_call_incomming == FALSE)
            {
                ZDT_LedRed_Open(LED_LIGHT_OPEN_TYPE_TIMES,1000,3000,0xFFFF);
            }
        }
    }
    return 0;
}

uint8 ZDT_Led_Start_Call_Miss_Over(void)
{
    if(g_need_misscall_led)
    {
        if(FALSE == ZDT_LedAll_IsOn() && s_led_is_in_call_incomming == FALSE)
        {
            ZDT_LedRed_Stop();
        }
        g_need_misscall_led = FALSE;
    }
    return 0;
}

uint8 ZDT_Led_Start_Call_Normal(void)
{
    if(MMIZDT_LED_GetCallSwitch())
    {
        if(s_led_is_in_call_incomming)
        {
            ZDT_Led_Start_Call_Incoming();
        }
        else if(g_need_misscall_led)
        {
            ZDT_Led_Start_Call_Miss();
        }
        else
        {
            ZDT_LedRed_Stop();
        }
    }
    else
    {
        ZDT_LedRed_Stop();
    }
    return 0;
}

uint8 ZDT_Led_Start_SMS_UnRead(void)
{
    s_need_sms_led = TRUE;
    //if(MMIZDT_LED_GetSmsSwitch())
    if(1)
    {
        if(FALSE == ZDT_LedAll_IsOn())
        {
            ZDT_LedBlue_On();
        }
    }
    return 0;
}

uint8 ZDT_Led_Start_SMS_UnRead_OverExt(void)
{
    if(FALSE == ZDT_LedAll_IsOn())
    {
        ZDT_LedBlue_Stop();
    }
    return 0;
}


uint8 ZDT_Led_Start_SMS_UnRead_Over(void)
{
    s_need_sms_led = FALSE;
    ZDT_Led_Start_SMS_UnRead_OverExt();
    return 0;
}


uint8 ZDT_Led_Start_SMS_Normal(void)
{
    //if(MMIZDT_LED_GetSmsSwitch())
    if(1)
    {
        if(s_need_sms_led)
        {
            ZDT_LedBlue_On();
        }
        else
        {
            ZDT_LedBlue_Stop();
        }
    }
    else
    {
        ZDT_LedBlue_Stop();
    }
    
    return 0;
}

uint8 ZDT_Led_Start_Flip(void)
{
    if(MMIZDT_LED_GetFlipSwitch())
    {
        ZDT_LedAll_Start_Flash(2);
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
      GPIO_OpenLed_1(onoff);
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
	GPIO_OpenLed_2(onoff);
#endif
}

void LED_Func_Switch(BOOLEAN onoff)
{
      static uint8 s_is_led_func_on = 0xff;
      if(s_is_led_func_on == onoff)
      {
            return;
      }
      else
      {
            s_is_led_func_on = onoff;
      }
}

void LED_Test_Switch(void)
{
      static BOOLEAN s_is_led_test_on = FALSE;
      s_is_led_test_on = !s_is_led_test_on;
      //GPIO_OpenLed_2(s_is_led_test_on);
}

PUBLIC uint8 MMIZDT_LED_NV_GetChargeSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_ZDT_NV_LED_CHARGE,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_ZDT_NV_LED_CHARGE,&is_on);
    }
    s_zdt_led_charge_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIZDT_LED_NV_SetChargeSwitch(uint8 is_on) 
{
    if(s_zdt_led_charge_is_on != is_on)
    {
        MMINV_WRITE(MMI_ZDT_NV_LED_CHARGE,&is_on);
        s_zdt_led_charge_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_LED_GetChargeSwitch(void)
{
    return s_zdt_led_charge_is_on;
}


PUBLIC uint8 MMIZDT_LED_NV_GetSmsSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_ZDT_NV_LED_SMS,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_ZDT_NV_LED_SMS,&is_on);
    }
    s_zdt_led_sms_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIZDT_LED_NV_SetSmsSwitch(uint8 is_on) 
{
    if(s_zdt_led_sms_is_on != is_on)
    {
        MMINV_WRITE(MMI_ZDT_NV_LED_SMS,&is_on);
        s_zdt_led_sms_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_LED_GetSmsSwitch(void)
{
    return s_zdt_led_sms_is_on;
}

PUBLIC uint8 MMIZDT_LED_NV_GetCallSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_ZDT_NV_LED_CALL,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_ZDT_NV_LED_CALL,&is_on);
    }
    s_zdt_led_call_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIZDT_LED_NV_SetCallSwitch(uint8 is_on) 
{
    if(s_zdt_led_call_is_on != is_on)
    {
        MMINV_WRITE(MMI_ZDT_NV_LED_CALL,&is_on);
        s_zdt_led_call_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_LED_GetCallSwitch(void)
{
    return s_zdt_led_call_is_on;
}

PUBLIC uint8 MMIZDT_LED_NV_GetPowerSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_ZDT_NV_LED_POWER,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_ZDT_NV_LED_POWER,&is_on);
    }
    s_zdt_led_power_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIZDT_LED_NV_SetPowerSwitch(uint8 is_on) 
{
    if(s_zdt_led_power_is_on != is_on)
    {
        MMINV_WRITE(MMI_ZDT_NV_LED_POWER,&is_on);
        s_zdt_led_power_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_LED_GetPowerSwitch(void)
{
    return s_zdt_led_power_is_on;
}


PUBLIC uint8 MMIZDT_LED_NV_GetFlipSwitch(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 is_on = 1;
    MMINV_READ(MMI_ZDT_NV_LED_FLIP,&is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = 1;
        MMINV_WRITE(MMI_ZDT_NV_LED_FLIP,&is_on);
    }
    s_zdt_led_flip_is_on = is_on;
    return is_on;
}

PUBLIC BOOLEAN  MMIZDT_LED_NV_SetFlipSwitch(uint8 is_on) 
{
    if(s_zdt_led_flip_is_on != is_on)
    {
        MMINV_WRITE(MMI_ZDT_NV_LED_FLIP,&is_on);
        s_zdt_led_flip_is_on = is_on;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_LED_GetFlipSwitch(void)
{
    return s_zdt_led_flip_is_on;
}

#endif
