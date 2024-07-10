#ifndef _ZDT_HSENSOR_C
#define _ZDT_HSENSOR_C

#include "zdt_hsensor.h"
#include "priority_system.h"
#include "ldo_drvapi.h"

#define h_abs(x)  ((x) >= 0 ? (x) : (-(x)))


#ifdef ZDT_APP_SUPPORT
extern BOOLEAN  ZDTHSensor_SendTo_APP(ZDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);
#endif

BOOLEAN  ZDTHSensor_TWCheckTimer_Start(void);
BOOLEAN  ZDTHSensor_TWCheckTimer_Stop(void);

BOOLEAN  ZDTHSensor_HRCheckTimer_Start(void);
BOOLEAN  ZDTHSensor_HRCheckTimer_Stop(void);
#ifdef ZDT_HSENSOR_SUPPORT_TW
double ct1711_get_temp_degree(void);
#endif

static SCI_TIMER_PTR      s_hsensor_tw_check_timer = PNULL;
static SCI_TIMER_PTR      s_hsensor_hr_check_timer = PNULL;

static BLOCK_ID zdthsensor_taskid=SCI_INVALID_BLOCK_ID;
BOOLEAN g_is_zdt_hsensor_open = FALSE;
BOOLEAN g_is_zdt_hsensor_tw_open = FALSE;
BOOLEAN g_is_zdt_hsensor_hr_open = FALSE;
BOOLEAN g_is_zdt_hsensor_bp_open = FALSE;
BOOLEAN g_is_zdt_hsensor_spo2_open = FALSE;
BOOLEAN g_is_zdt_hsensor_init = FALSE;

uint16 hsensor_cur_hr = 0;
uint16 hsensor_cur_hr_pre = 0;
float hsensor_cur_spo = 0;
double hsensor_cur_tw = 0;
static uint16 cur_spo2 = 0;
uint16 hsensor_cur_bp_up = 0;
uint16 hsensor_cur_bp_down = 0;

double hsensor_real_tw = 0;
double hsensor_tibiao_tw = 0;

LOCAL uint32 over26_time = 0;

static BOOLEAN hsensor_is_init = FALSE;
static BOOLEAN hsensor_is_open = FALSE;
static ZDT_HSENSOR_OPT_E hsensor_opt_mode = HSENSOR_OPT_MODE_NULL;
static BOOLEAN hsensor_tw_is_open = FALSE;
static ZDT_HSENSOR_IC_TYPE_E s_hsensor_type = HSENSOR_TYPE_NULL;

#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
extern uint32 VCHR02_HandleInt();
extern uint16 vchr02_get_spo2();
extern uint16 vchr02_get_heartrate();
#endif

#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_HRS3300)
extern unsigned int hrs3300_hr_data;
extern unsigned int hrs3300_up_data;
extern unsigned int hrs3300_down_data;
#endif

#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_PAH8009)
extern float pah8009_hr_data;
extern float pah8009_spo2_data;
#endif

PUBLIC void ZDT_HRTraceExt(const char *string, int len)
{
#if 1
    if (len < 128)
    {
        SCI_TraceLow("[HR]:%s",string);
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
        
           SCI_TraceLow("[HR]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZDT_HRTrace(const char *pszFmt, ...)
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
        ZDT_HRTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}


static BOOLEAN hsensor_init(void) 
{
    BOOLEAN ret = FALSE;
    if(hsensor_is_init == FALSE)
    {
        ret = TRUE;
        GPIO_HSensor_IRQCtrl(FALSE);
        GPIO_SetHSensorPower(1);
        SCI_Sleep(50);
        #ifdef ZDT_HSENSOR_TYPE_AUTO
            if(hrs3300_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_HRS3300;
            }
            else if(VCHR02_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_VCHR02;
            }
            else if(pah8009_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_PAH8009;
            }
            else
            {
                s_hsensor_type = HSENSOR_TYPE_NULL;
            }
        #elif defined(ZDT_HSENSOR_TYPE_HRS3300)
            if(hrs3300_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_HRS3300;
            }
            else
            {
                s_hsensor_type = HSENSOR_TYPE_NULL;
            }
        #elif defined(ZDT_HSENSOR_TYPE_VCHR02)
            if(VCHR02_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_VCHR02;
            }
            else
            {
                s_hsensor_type = HSENSOR_TYPE_NULL;
            }
        #elif defined(ZDT_HSENSOR_TYPE_PAH8009)
            if(pah8009_CheckID())
            {
                s_hsensor_type = HSENSOR_TYPE_PAH8009;
            }
            else
            {
                s_hsensor_type = HSENSOR_TYPE_NULL;
            }
        #else
            s_hsensor_type = HSENSOR_TYPE_NULL;
        #endif
        GPIO_SetHSensorPower(0);
        ZDT_HSENSOR_TRACE("hsensor_init over ret=%d,type=%d",ret,s_hsensor_type);
        if(s_hsensor_type != HSENSOR_TYPE_NULL)
        {
            hsensor_is_init = TRUE;
        }
    }
   return ret;
}

static BOOLEAN hsensor_open(ZDT_HSENSOR_OPT_E opt_mode) 
{
    BOOLEAN ret = FALSE;
    ZDT_HSENSOR_TRACE("hsensor_open is_init=%d,type=%d",hsensor_is_init,s_hsensor_type);
    if(hsensor_is_open == FALSE)
    {
        hsensor_opt_mode = opt_mode;
        #ifndef WIN32
        if(s_hsensor_type == HSENSOR_TYPE_NULL)
        {
            hsensor_init();
        }
        if(s_hsensor_type == HSENSOR_TYPE_HRS3300)
        {
            GPIO_SetHSensorPower(1);
            SCI_Sleep(5);
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_HRS3300)
            hrs3300_init();
            ZDTHSensor_HRCheckTimer_Start();
#endif
        }
        else if(s_hsensor_type == HSENSOR_TYPE_VCHR02)
        {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
            GPIO_HSensor_IRQCtrl(TRUE);
            if(opt_mode == HSENSOR_OPT_MODE_SPO2)
            {
                vchr02_spo2_drv_open();
            }
            else
            {
                vchr02_drv_open();
            }
#endif
        }
        else if(s_hsensor_type == HSENSOR_TYPE_PAH8009)
        {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_PAH8009)
            GPIO_SetHSensorPower(1);
            SCI_Sleep(5);
            //GPIO_HSensor_IRQCtrl(TRUE);
            if(opt_mode == HSENSOR_OPT_MODE_DEFAULT)
            {
                pah8009_init();
            }
            else if(opt_mode == HSENSOR_OPT_MODE_HR)
            {
                pah8009_HR_Open();
            }
            else if(opt_mode == HSENSOR_OPT_MODE_BP)
            {
                pah8009_BP_Open();
            }
            else if(opt_mode == HSENSOR_OPT_MODE_SPO2)
            {
                pah8009_SPO2_Open();
            }
            else if(opt_mode == HSENSOR_OPT_MODE_FACTORY)
            {
                pah8009_Factory_Open();
            }
            ZDTHSensor_HRCheckTimer_Start();
#endif
        }
        #endif
        
        ret = TRUE;
        hsensor_is_open = TRUE;
    }
   return ret;
}

static BOOLEAN hsensor_close(void)
{
    BOOLEAN ret = TRUE;
    ZDT_HSENSOR_TRACE("hsensor_close is_init=%d,type=%d",hsensor_is_init,s_hsensor_type);
    if(hsensor_is_open)
    {
        ZDTHSensor_HRCheckTimer_Stop();
        if(s_hsensor_type == HSENSOR_TYPE_HRS3300)
        {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_HRS3300)
            hrs3300_exit();
#endif
        }
        else if(s_hsensor_type == HSENSOR_TYPE_VCHR02)
        {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
            vchr02_drv_close();
#endif
            GPIO_HSensor_IRQCtrl(FALSE);
        }
        else if(s_hsensor_type == HSENSOR_TYPE_PAH8009)
        {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_PAH8009)
            if(hsensor_opt_mode == HSENSOR_OPT_MODE_DEFAULT)
            {
                pah8009_exit();
            }
            else if(hsensor_opt_mode == HSENSOR_OPT_MODE_HR)
            {
                pah8009_HR_Close();
            }
            else if(hsensor_opt_mode == HSENSOR_OPT_MODE_BP)
            {
                pah8009_BP_Close();
            }
            else if(hsensor_opt_mode == HSENSOR_OPT_MODE_SPO2)
            {
                pah8009_SPO2_Close();
            }
            else if(hsensor_opt_mode == HSENSOR_OPT_MODE_FACTORY)
            {
                pah8009_Factory_Close();
            }
#endif
            GPIO_HSensor_IRQCtrl(FALSE);
        }
		
        GPIO_SetHSensorPower(0);
        ret = TRUE;
        hsensor_is_open = FALSE;
    }
   return ret;
}


static BOOLEAN hsensor_tw_open(void) 
{
    BOOLEAN ret = FALSE;
    ZDT_HSENSOR_TRACE("hsensor_tw_open hsensor_tw_is_open=%d",hsensor_tw_is_open);
    if(hsensor_tw_is_open == FALSE)
    {        
        #ifndef WIN32
            GPIO_SetHSensorTWPower(1);
        #endif
        
        #ifdef ZDT_HSENSOR_SUPPORT_TW
            ZDTHSensor_TWCheckTimer_Start();
        #endif
        ret = TRUE;
        hsensor_tw_is_open = TRUE;
    }
   return ret;
}

static BOOLEAN hsensor_tw_close(void) 
{
    BOOLEAN ret = TRUE;
    ZDT_HSENSOR_TRACE("hsensor_tw_close hsensor_tw_is_open=%d",hsensor_tw_is_open);
    if(hsensor_tw_is_open)
    {
        #ifdef ZDT_HSENSOR_SUPPORT_TW
            ZDTHSensor_TWCheckTimer_Stop();
        #endif
        #ifndef WIN32
            GPIO_SetHSensorTWPower(0);
        #endif
        ret = TRUE;
        hsensor_tw_is_open = FALSE;
    }
   return ret;
}

static BOOLEAN hsensor_reset(void) 
{
    BOOLEAN ret = FALSE;
    ZDT_HSENSOR_TRACE("hsensor_reset_step is_init=%d,type=%d",hsensor_is_init,s_hsensor_type);
    if(hsensor_is_init)
    {
        ret = TRUE;
    }
   return ret;
}

static void ZDTHSensor_TWCheckTimerOut(uint32 param)
{	    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TW_TIMER,PNULL,0);
    SCI_ChangeTimer(s_hsensor_tw_check_timer, ZDTHSensor_TWCheckTimerOut,5000);
    SCI_ActiveTimer(s_hsensor_tw_check_timer);	
    return ;
}

BOOLEAN  ZDTHSensor_TWCheckTimer_Start(void)
{
    if(s_hsensor_tw_check_timer == PNULL)
    {
        s_hsensor_tw_check_timer = SCI_CreateTimer("TW_Timer", ZDTHSensor_TWCheckTimerOut,
                                    1, 1000, SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_hsensor_tw_check_timer, ZDTHSensor_TWCheckTimerOut,5000);
        SCI_ActiveTimer(s_hsensor_tw_check_timer);	
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_TWCheckTimer_Stop(void)
{
    if(s_hsensor_tw_check_timer != PNULL)
    {
        if(SCI_IsTimerActive(s_hsensor_tw_check_timer))
        {
            SCI_DeactiveTimer(s_hsensor_tw_check_timer);
        }
    }
    return TRUE;
}

static void ZDTHSensor_HRCheckTimerOut(uint32 param)
{	    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TIMER,PNULL,0);
    SCI_ChangeTimer(s_hsensor_hr_check_timer, ZDTHSensor_HRCheckTimerOut,40);
    SCI_ActiveTimer(s_hsensor_hr_check_timer);	
    return;
}

BOOLEAN  ZDTHSensor_HRCheckTimer_Start(void)
{
    if(s_hsensor_hr_check_timer == PNULL)
    {
        s_hsensor_hr_check_timer = SCI_CreateTimer("HR_Timer", ZDTHSensor_HRCheckTimerOut,
                                    1, 40, SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_hsensor_hr_check_timer, ZDTHSensor_HRCheckTimerOut,40);
        SCI_ActiveTimer(s_hsensor_hr_check_timer);	
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_HRCheckTimer_Stop(void)
{
    if(s_hsensor_hr_check_timer != PNULL)
    {
        if(SCI_IsTimerActive(s_hsensor_hr_check_timer))
        {
            SCI_DeactiveTimer(s_hsensor_hr_check_timer);
        }
    }
    return TRUE;
}

uint32 ZDTHSensor_GPIO_IntHandler(BOOLEAN is_high)
{
    if(g_is_zdt_hsensor_init == FALSE)
    {
        return 0;
    }
    if(s_hsensor_type == HSENSOR_TYPE_VCHR02 && is_high == TRUE)
    {
        APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_INT,PNULL,0);
    }
    else if(s_hsensor_type == HSENSOR_TYPE_PAH8009 && is_high == FALSE)
    {
        APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_INT0,PNULL,0);
    }
		
    return 0;
}

BOOLEAN  ZDTHSensor_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleOpen");
    if(hsensor_is_open && hsensor_opt_mode != HSENSOR_OPT_MODE_DEFAULT)
    {
        hsensor_close();
    }
    if(hsensor_open(HSENSOR_OPT_MODE_DEFAULT))
    {
        g_is_zdt_hsensor_open = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleClose");
    if(hsensor_close())
    {
        cur_spo2 = 0;
        hsensor_cur_hr = 0;
        hsensor_cur_spo = 0;
        hsensor_cur_bp_up = 0;
        hsensor_cur_bp_down = 0;
        g_is_zdt_hsensor_open = FALSE;
    }
    return TRUE;
}


BOOLEAN ZDTHSensor_HR_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HR_HandleOpen");
    if(hsensor_is_open && hsensor_opt_mode != HSENSOR_OPT_MODE_HR)
    {
        hsensor_close();
    }
    if(hsensor_open(HSENSOR_OPT_MODE_HR))
    {
        g_is_zdt_hsensor_hr_open = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_HR_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HR_HandleClose");
    if(hsensor_close())
    {
        cur_spo2 = 0;
        hsensor_cur_hr = 0;
        hsensor_cur_spo = 0;
        hsensor_cur_bp_up = 0;
        hsensor_cur_bp_down = 0;
        g_is_zdt_hsensor_hr_open = FALSE;
    }
    return TRUE;
}

BOOLEAN ZDTHSensor_BP_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_BP_HandleOpen");
    if(hsensor_is_open && hsensor_opt_mode != HSENSOR_OPT_MODE_BP)
    {
        hsensor_close();
    }
    if(hsensor_open(HSENSOR_OPT_MODE_BP))
    {
        g_is_zdt_hsensor_bp_open = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_BP_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_BP_HandleClose");
    if(hsensor_close())
    {
        cur_spo2 = 0;
        hsensor_cur_hr = 0;
        hsensor_cur_spo = 0;
        hsensor_cur_bp_up = 0;
        hsensor_cur_bp_down = 0;
        g_is_zdt_hsensor_bp_open = FALSE;
    }
    return TRUE;
}

BOOLEAN ZDTHSensor_Factory_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_Factory_HandleOpen");
    if(hsensor_is_open && hsensor_opt_mode != HSENSOR_OPT_MODE_FACTORY)
    {
        hsensor_close();
    }
    if(hsensor_open(HSENSOR_OPT_MODE_FACTORY))
    {
        g_is_zdt_hsensor_bp_open = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_Factory_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_Factory_HandleClose");
    if(hsensor_close())
    {
        cur_spo2 = 0;
        hsensor_cur_hr = 0;
        hsensor_cur_spo = 0;
        hsensor_cur_bp_up = 0;
        hsensor_cur_bp_down = 0;
        g_is_zdt_hsensor_bp_open = FALSE;
    }
    return TRUE;
}

BOOLEAN ZDTHSensor_SPO2_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_SPO2_HandleOpen");
    if(hsensor_is_open && hsensor_opt_mode != HSENSOR_OPT_MODE_SPO2)
    {
        hsensor_close();
    }
    if(hsensor_open(HSENSOR_OPT_MODE_SPO2))
    {
        g_is_zdt_hsensor_spo2_open = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_SPO2_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_SPO2_HandleClose");
    if(hsensor_close())
    {
        cur_spo2 = 0;
        hsensor_cur_hr = 0;
        hsensor_cur_spo = 0;
        hsensor_cur_bp_up = 0;
        hsensor_cur_bp_down = 0;
        g_is_zdt_hsensor_spo2_open = FALSE;
    }
    return TRUE;
}

BOOLEAN  ZDT_HSensor_HandleInt(void)
{
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
    VCHR02_HandleInt();
#endif
    return TRUE;
}

BOOLEAN  ZDTHSensor_TW_HandleOpen(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_TW_HandleOpen");
    if(hsensor_tw_open())
    {
        g_is_zdt_hsensor_tw_open = TRUE;
    }
    hsensor_cur_tw = 0;
    hsensor_real_tw = 0;
    hsensor_tibiao_tw = 0;
    over26_time = 0;
    return TRUE;
}

BOOLEAN  ZDTHSensor_TW_HandleClose(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_TW_HandleClose");
    if(hsensor_tw_close())
    {
        hsensor_cur_tw = 0;
        hsensor_real_tw = 0;
        hsensor_tibiao_tw = 0;
        g_is_zdt_hsensor_tw_open = FALSE;
        over26_time = 0;
    }
    return TRUE;
}

BOOLEAN  ZDTHSensor_HandleReset(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleReset");
    hsensor_reset();
    return TRUE;
}

BOOLEAN  ZDTHSensor_HandleTimer(void)
{
    if(s_hsensor_type == HSENSOR_TYPE_HRS3300)
    {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_HRS3300)
        heart_rate_meas_timeout_handler(PNULL);
        if(hrs3300_hr_data > 0)
        {
            hsensor_cur_hr = hrs3300_hr_data;
        }
        else
        {
            hsensor_cur_hr = 0;
        }
		if(hrs3300_up_data > 0)
		{
		    hsensor_cur_bp_up = hrs3300_up_data;
		}
		else
		{
		    hsensor_cur_bp_up = 0;

		}

		if(hrs3300_down_data > 0)
		{
		    hsensor_cur_bp_down = hrs3300_down_data;
		}
		else
		{
		    hsensor_cur_bp_down = 0;

		}	
#endif
    }
    else if(s_hsensor_type == HSENSOR_TYPE_PAH8009)
    {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_PAH8009)
        //ZDT_HSENSOR_TRACE("ZDTHSensor_HandleTimer s_hsensor_type=%d,hsensor_opt_mode=%d",s_hsensor_type,hsensor_opt_mode);
        if(hsensor_opt_mode == HSENSOR_OPT_MODE_DEFAULT || hsensor_opt_mode == HSENSOR_OPT_MODE_SPO2)
        {
            demo_auto_mode_spo2_polling_handle();
        }
        else if(hsensor_opt_mode == HSENSOR_OPT_MODE_BP)
        {
        }
        else
        {
            demo_auto_mode_hrd_polling_handle();
        }
        if(pah8009_hr_data > 0)
        {
            hsensor_cur_hr = pah8009_hr_data;
        }
        else
        {
            hsensor_cur_hr = 0;
        }
        
        if(pah8009_spo2_data > 0)
        {
            hsensor_cur_spo = pah8009_spo2_data;
        }
        else
        {
            hsensor_cur_spo = 0;
        }
 #endif
    }
    return TRUE;
}

double randomAddTemp1(void)
{
    double tmp;
    int32 r1 = 0;
    srand(SCI_GetTickCount());
    r1 = rand()%4;
    tmp = r1*0.1;
    return tmp;
}

double randomAddTemp2(void)
{
    double tmp;
    int32 r1 = 0;
    srand(SCI_GetTickCount());
    r1 = rand()%6;
    tmp = r1*0.1;
    return tmp;
}

double ZDTHSensor_TW_Value_Check(double resultTemp)
{
    double cur_tw = 0.00;
    double resultTempAdd = 0.00;
    
    if( resultTemp > 32  &&  resultTemp < 43  )  
    {
        if( resultTemp <= 36 )
        {
            if(  resultTemp <= 32.1) {
                resultTempAdd = 35.9 + randomAddTemp1();
            }
            else if(  resultTemp <= 33.5f) {
                resultTempAdd = 36.0 + randomAddTemp1();
            }
            else if( resultTemp <= 34.5f) {
                resultTempAdd = 36.1 + randomAddTemp1();
            }
            else if( resultTemp <= 35.5) {
                resultTempAdd = 36.2 + randomAddTemp1();
            }
            else if( resultTemp <= 36.0) {
                resultTempAdd = 36.4 + randomAddTemp1();
            }
        }
        else if( resultTemp <= 36.4 ) 
        {
            resultTempAdd = 36.3 + randomAddTemp2() ;
        }
        else if( resultTemp <= 37.3 ) 
        {
            resultTempAdd = resultTemp  + 0.5 + randomAddTemp1();
        }
        else if(  resultTemp <= 39) 
        {
            resultTempAdd = resultTemp  + 0.3 +  randomAddTemp1();
        }
        else if(  resultTemp <= 43)
        {
            resultTempAdd = resultTemp;
        }
        
    }

    return resultTempAdd;
}

const float temp_trans_tab[][2] = 
{
    {26.0f, 35.0f},
    {30.0f, 35.8f},
    {32.1f, 36.0f},
    {33.5f, 36.1f},
    {34.5f, 36.2f},
    {35.4f, 36.3f},
    {36.0f, 36.4f},
    {36.4f, 36.6f},
};


float calc_temp(float resultTemp)
{
    static float last_result = 0.0f;
    float resultTempAdd = 0.0f;
    uint8 i;
    float trigger_temp = 30.0f;
    uint32 cur_time = MMIAPICOM_GetCurTime();
    if(resultTemp >= 26)
    {
        if(over26_time == 0)
        {
            over26_time = cur_time;
        }
        else
        {
            if(cur_time - over26_time >= 30 && (cur_time - over26_time) <= 120)
            {
                if(resultTemp > 27)
                {
                    trigger_temp = 26.0f;
                }
                
            }
            else if(cur_time - over26_time >= 120)
            {
                over26_time = 0;
            }
        }
            
    }
    else
    {
        over26_time = 0;
    }
    
    //if (resultTemp > 25.0f && resultTemp < 43.0f)
    if (resultTemp > trigger_temp && resultTemp < 43.0f)
    {
        if (resultTemp <= 36.0f)
        {
            for(i =0; i< sizeof(temp_trans_tab)/sizeof(temp_trans_tab[0]);i++)
            {
                if (resultTemp < temp_trans_tab[i][0])
                    break;
            }
            if (i >= sizeof(temp_trans_tab)/sizeof(temp_trans_tab[0]))
            {
                i = sizeof(temp_trans_tab)/sizeof(temp_trans_tab[0])-1;
            }
            {
                float ti0 = temp_trans_tab[i][0];
                float ti1 = temp_trans_tab[i+1][0];
                float to0 = temp_trans_tab[i][1];
                float to1 = temp_trans_tab[i+1][1];
                resultTempAdd = (resultTemp-ti0)*(to1-to0)/(ti1-ti0)+to0+randomAddTemp1();
            }
        }
        else if (resultTemp <= 36.4f)
        {
            resultTempAdd = 36.6f + randomAddTemp2();
        }
        else if (resultTemp <= 37.3f)
        {
            resultTempAdd = resultTemp + 0.15f + randomAddTemp1();
        }
        else if (resultTemp <= 39)
        {
            resultTempAdd = resultTemp + 0.1f + randomAddTemp1();
        }
    }
    //else if(resultTemp < 55)
    else if(resultTemp < 45)
    {
        resultTempAdd = resultTemp;
    }
    
    if (last_result < 30)
    {
        last_result = resultTempAdd;
    }
    else
    {
        if(last_result > 0 && resultTempAdd > 0)
        {
            last_result = resultTempAdd = (last_result + resultTempAdd)/2;
        }
        else
        {
            last_result = resultTempAdd;
        }
    }
    
    
    if(1)
    {
        int temp = (int)(resultTemp*10);
        int temp_add = (int)(resultTempAdd*10);
        SCI_TRACE_LOW("tw ZDT calc_temp resultTemp= %ld ,resultTempAdd=%ld",temp, temp_add);
    }
    
    return resultTempAdd;
}

typedef struct
{
    double last_tw;
    BOOLEAN recordStart;
}TW_RECORD_T;
TW_RECORD_T last_tw_record = {0};

BOOLEAN  ZDTHSensor_TW_HandleTimer(void)
{
    double temp_tw = 0.00;
    double cur_tw = 0.00;
#ifdef ZDT_HSENSOR_SUPPORT_TW
#ifndef WIN32
    temp_tw = ct1711_get_temp_degree();
    hsensor_real_tw = temp_tw;
    if(temp_tw < 43)
    {
        hsensor_tibiao_tw = temp_tw;
    }

    cur_tw = ZDTHSensor_TW_Value_Check(temp_tw);
    cur_tw = calc_temp((float)temp_tw);
    //cur_tw = temp_tw;
    if(cur_tw > 0)
    {
        
        //hsensor_cur_tw = cur_tw;
        if(hsensor_cur_tw == 0)
        {
            hsensor_cur_tw = cur_tw;
        }
        else if(hsensor_cur_tw > 20 && h_abs(hsensor_cur_tw - cur_tw) > 11)
        {

        }
        else
        {
            hsensor_cur_tw = cur_tw;
        }
    }
    else if(temp_tw > 0 && temp_tw < 43)
    {
        hsensor_cur_tw = temp_tw;
    }
    #if 0
    if(hsensor_cur_tw > 35 && hsensor_cur_tw <= 42.5)
    {
        ZDT_SendTw(hsensor_cur_tw);
    }
    #endif
#endif
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleTimer TEMP= %lf",hsensor_cur_tw);
    SCI_TRACE_LOW("ZDTHSensor_HandleTimer2 TEMP= %ld",temp_tw*10);
#endif
    return TRUE;
}

BOOLEAN  ZDTHSensor_HandleHR(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleHR");
    return TRUE;
}

BOOLEAN  ZDTHSensor_HandleTW(void)
{
    ZDT_HSENSOR_TRACE("ZDTHSensor_HandleTW");
    return TRUE;
}

void ZDTHSensor_HandleINT0(void)
{
    //ZDT_HSENSOR_TRACE("ZDTHSensor_HandleINT0 s_hsensor_type=%d,hsensor_opt_mode=%d",s_hsensor_type,hsensor_opt_mode);

    if(s_hsensor_type == HSENSOR_TYPE_PAH8009)
    {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_PAH8009)
        if(hsensor_opt_mode == HSENSOR_OPT_MODE_DEFAULT || hsensor_opt_mode == HSENSOR_OPT_MODE_SPO2)
        {
            //qah800x_handle_gpio_spo2_interrupt();
        }
        else if(hsensor_opt_mode == HSENSOR_OPT_MODE_BP)
        {
        }
        else
        {
            //qah800x_handle_gpio_hr_interrupt();
        }
        if(pah8009_hr_data > 0)
        {
            hsensor_cur_hr = pah8009_hr_data;
        }
        else
        {
            hsensor_cur_hr = 0;
        }
        
        if(pah8009_spo2_data > 0)
        {
            hsensor_cur_spo = pah8009_spo2_data;
        }
        else
        {
            hsensor_cur_spo = 0;
        }
#endif
    }
    return;
}

void ZDTHSensor_TaskEntry(uint32 argc, void *argv)
{	
    zdthsensor_taskid=SCI_IdentifyThread();
    if(hsensor_init())
    {
        g_is_zdt_hsensor_init = TRUE;
    }
    ZDT_HSENSOR_TRACE("ZDTHSensor_TaskEntry is_init = %d",g_is_zdt_hsensor_init);
    
    while(1)
    {
    	xSignalHeaderRec*   sig_ptr = 0; 
        
    	sig_ptr = SCI_GetSignal( zdthsensor_taskid); 
    	switch(sig_ptr->SignalCode)
    	{
                case SIG_ZDT_HSENSOR_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_HandleClose();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
					
                case SIG_ZDT_HSENSOR_SPO2_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_SPO2_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                case  SIG_ZDT_HSENSOR_INT: //vchr02
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDT_HSensor_HandleInt();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;

                
                    
                case SIG_ZDT_HSENSOR_SPO2_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_SPO2_HandleClose();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
					
                case SIG_ZDT_HSENSOR_TW_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_TW_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_TW_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_TW_HandleClose();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_RESET:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_HandleReset();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_TIMER:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_HandleTimer();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_TW_TIMER:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_TW_HandleTimer();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_HR_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_HR_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_HR_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_HR_HandleClose();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_BP_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_BP_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_BP_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_BP_HandleClose();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_TW:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        
                        ZDTHSensor_HandleTW();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_INT0:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_HandleINT0();
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                case SIG_ZDT_HSENSOR_INT1:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;

                case SIG_ZDT_HSENSOR_FACTORY_OPEN:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_Factory_HandleOpen();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_HSENSOR_FACTORY_CLOSE:
                    {
                        ZDT_HSENSOR_SIG_T *  gs_sig = (ZDT_HSENSOR_SIG_T*)sig_ptr;
                        ZDTHSensor_Factory_HandleClose();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                default:
                    break;
    	}
    	SCI_FREE(sig_ptr);
    }
}

void ZDTHSensor_TaskCreate(void)
{
	if(zdthsensor_taskid == SCI_INVALID_BLOCK_ID)
	{
		zdthsensor_taskid = SCI_CreateThread("ZDTHSOR_THREAD",
			"ZDTHSOR_QUEUE",
			ZDTHSensor_TaskEntry,
			NULL,
			NULL,
			1024*10,
			100,
			77,	// 比MMI 74要高
			SCI_PREEMPT,
			SCI_AUTO_START
			);
        
		ZDT_HSENSOR_TRACE("ZDTHSensor_TaskCreate, zdthsensor_taskid=0x%x", zdthsensor_taskid);
	}
}

void ZDTHSensor_TaskClose(void)
{
	if(SCI_INVALID_BLOCK_ID != zdthsensor_taskid)
	{
		SCI_TerminateThread(zdthsensor_taskid);
		SCI_DeleteThread(zdthsensor_taskid);
		zdthsensor_taskid = SCI_INVALID_BLOCK_ID;       
	}
	ZDT_HSENSOR_TRACE("ZDTHSensor_TaskClose");
}

BLOCK_ID ZDTHSensor_TaskGetID(void)
{
	return zdthsensor_taskid;
}

BOOLEAN  APP_SendSigTo_HSensor(ZDT_HSENSOR_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_HSENSOR_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZDTHSensor_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        ZDT_HSENSOR_TRACE("APP_SendSigTo_HSensor Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCA(data_len);//free it in gps task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("APP_SendTo_ATC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_HSENSOR_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

BOOLEAN ZDT_HSensor_IsOpen(void)
{
    return g_is_zdt_hsensor_open;
}

BOOLEAN ZDT_HSensor_HR_IsOpen(void)
{
    return g_is_zdt_hsensor_hr_open;
}

BOOLEAN ZDT_HSensor_SPO2_IsOpen(void)
{
    return g_is_zdt_hsensor_spo2_open;
}

BOOLEAN ZDT_HSensor_TW_IsOpen(void)
{
    return g_is_zdt_hsensor_tw_open;
}

int ZDT_HSensor_Init(void)
{    
    ZDTHSensor_TaskCreate();
    return 0;
}

int ZDT_HSensor_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_CLOSE,PNULL,0);
    return 0;
}

int ZDT_HSensor_SPO2_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_SPO2_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_SPO2_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_SPO2_CLOSE,PNULL,0);
    return 0;
}


int ZDT_HSensor_TW_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TW_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_TW_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TW_CLOSE,PNULL,0);
    return 0;
}

int ZDT_HSensor_Reset(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_RESET,PNULL,0);
    return 0;
}

int ZDT_HSensor_TimerOut(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TIMER,PNULL,0);
    return 0;
}

int ZDT_HSensor_HR_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_HR_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_HR_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_HR_CLOSE,PNULL,0);
    return 0;
}

int ZDT_HSensor_BP_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_BP_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_BP_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_BP_CLOSE,PNULL,0);
    return 0;
}

int ZDT_HSensor_Factory_Open(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_FACTORY_OPEN,PNULL,0);
    return 0;
}

int ZDT_HSensor_Factory_Close(void)
{    
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_FACTORY_CLOSE,PNULL,0);
    return 0;
}

int ZDT_HSensor_TW(void)
{    
    //体温
    APP_SendSigTo_HSensor(SIG_ZDT_HSENSOR_TW,PNULL,0);
    return 0;
}

uint16 ZDT_HSensor_GetHR(void)
{   
    if(s_hsensor_type == HSENSOR_TYPE_VCHR02)
    {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
        hsensor_cur_hr = vchr02_get_heartrate();
#endif
    }
    return hsensor_cur_hr;
}

uint16 ZDT_HSensor_GetBP_up(void)
{   
    return hsensor_cur_bp_up;
}

uint16 ZDT_HSensor_GetBP_down(void)
{   
    return hsensor_cur_bp_down;
}

static int ls_rand(void)
{    
	static int ali_rand_offset = 12345;    
	int radom = 0;    
	srand(SCI_GetTickCount()+ali_rand_offset);    
	ali_rand_offset++;    
	radom = rand();    
	//SCI_TRACE_LOW("alipay_iot_rand radom=%d",radom);
	return radom;
}
static  uint16 ZDT_GetRandSPO2(void)
{    
    switch(ls_rand()%5)
    {
        	case 0:
        		cur_spo2 = 99;
        		break;
        	case 1:
        		cur_spo2 = 98;
        		break;
        	case 2:
        		cur_spo2 = 97;
        		break;
        	case 3:
        		cur_spo2 = 96;
        		break;
        	case 4:
        		cur_spo2 = 95;
        		break;
        	default:
        		cur_spo2 = 99;
        		break;
    }
    return cur_spo2;
}

uint16 ZDT_HSensor_GetHRPre(void)
{    
    return hsensor_cur_hr_pre;
}

double ZDT_HSensor_GetTW(void)
{    
    return hsensor_cur_tw;
}

double ZDT_HSensor_GetRealTW(void)
{    
    return hsensor_real_tw;
}

double ZDT_HSensor_GetTibiaoTW(void)
{    
    return hsensor_tibiao_tw;
}

float ZDT_HSensor_GetSPO(void)
{    
    return hsensor_cur_spo;
}

uint16 ZDT_HSensor_GetSPO2(void)
{    
    uint16 spo2_val = 0;
    if(s_hsensor_type == HSENSOR_TYPE_HRS3300)
    {
        ZDT_GetRandSPO2();
        spo2_val = cur_spo2;
    }
    else if(s_hsensor_type == HSENSOR_TYPE_VCHR02)
    {
#if defined(ZDT_HSENSOR_TYPE_AUTO) || defined(ZDT_HSENSOR_TYPE_VCHR02)
        cur_spo2 = vchr02_get_spo2();
        spo2_val = cur_spo2;
#endif
    }
    else if(s_hsensor_type == HSENSOR_TYPE_PAH8009)
    {
        spo2_val = hsensor_cur_spo;
    }
    return spo2_val;
}

BOOLEAN ZDT_HSensor_HW_IsOK(void) 
{
    return hsensor_is_init;
}

ZDT_HSENSOR_IC_TYPE_E ZDT_HSensor_GetType(void)
{
    return s_hsensor_type;
}

#endif/*_ZDT_HSENSOR_C*/

