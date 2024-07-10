/******************************************************************************
 ** File Name:      dualbat_hal.c                                             *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           21/06/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic dual battery operation process.*
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                  *
 ** 21/06/2011          Mingwei.Zhang            Create.                      *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "dal_power.h"
#include "chg_drvapi.h"
#include "dualbat_drvapi.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_DBAT

#ifdef DEBUG_DBAT
#define DBAT_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define DBAT_PRINT( _format_string )
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/

#define DBAT_STACK_SIZE     (1024*2)
#define DBAT_QUEUE_NUM      (24)
#define DBAT_PRIORITY      PRI_DBAT_TASK  // (SCI_PRIORITY_NORMAL + 1)

#define ADC_SAMPLING_NUM     5       //ADC sampling number
#define ADC_SAMPLING_DELAY   10      //time delay between AD convert

#define VOL_DIV_P1			268			///voltage divider 0.268,268/1000
#define VOL_DIV_P2			1000		///voltage divider 0.268,268/1000 
    
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL BLOCK_ID                  dbat_task_id;
LOCAL CHGMNG_ADC_CAL_T adc_cal[DBAT_MAX_BAT] = {0};

LOCAL DBAT_BAT_INFO_T           bat_info[DBAT_MAX_BAT];
LOCAL DBAT_STATE_INFO_T         dbat_state;
LOCAL DBAT_STATE_INFO_T         dbat_state_to_app;
LOCAL DBAT_DISCHARGE_PARAM_T*   p_dischg_param = NULL;
LOCAL DBAT_CHARGE_PARAM_T*      p_chg_param = NULL;
LOCAL DBAT_OVP_PARAM_T*         p_ovp_param = NULL;
LOCAL DBAT_OTP_PARAM_T*         p_otp_param = NULL;
LOCAL DBAT_BATDETECT_PARAM_T*   p_bat_detect = NULL;
LOCAL DBAT_HW_PARAM_T*          p_hw_param = NULL;
LOCAL DBAT_PROD_FUN_T*          p_prod_fun  = NULL;
LOCAL BOOLEAN                   charging_mutual_req = SCI_FALSE;


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void _DBAT_SrvThread (uint32 argc, void *argv);
LOCAL void _DBAT_BatMonitorHandler(void);
LOCAL BOOLEAN _DBAT_MutualMng(DBAT_BAT_E need_chg_bat,BOOLEAN on);
LOCAL void _DBAT_ChgFinishHandler(void);
LOCAL BOOLEAN _DBAT_CheckSupportMutualChg(DBAT_BAT_E aim_bat);
LOCAL void _DBAT_SwBatReqHandler(void);
LOCAL void _DBAT_ShutDownHandler(void);
LOCAL void _DBAT_WarningHandler(void);
LOCAL void _DBAT_ChgStartHandler(void);
LOCAL void _DBAT_ChgFaultHandler(void);
LOCAL void _DBAT_AdpPlugOutHandler(void);
LOCAL void _DBAT_BatPlugInHandler(DBAT_BAT_E plugin_bat);
LOCAL void _DBAT_BatPlugOutHandler(DBAT_BAT_E plugout_bat);
LOCAL void _DBAT_MutualChgRoutineInCharging(void);
LOCAL DBAT_BAT_E _DBAT_CheckWorkPriorityChg(void);
LOCAL DBAT_BAT_E _DBAT_CheckWorkPriorityDischg(void);
LOCAL BOOLEAN _DBAT_SwitchBatHandler(DBAT_BAT_E aim_bat);
LOCAL void _DBAT_ResetChgmngCfg(DBAT_BAT_E aim_bat);
LOCAL DBAT_BAT_E _DBAT_GetIdleBat(void);
LOCAL void _DBAT_BatStatistic(uint32 m_bat_vol,uint32 a_bat_vol);
LOCAL BOOLEAN _DBAT_ADCDetectBatPresent(uint32 mbat_vol,uint32 abat_vol);
LOCAL void _DBAT_CHGMNGCallBack(uint32 taskID, uint32 argc, void *argv);
LOCAL uint32 _DBAT_GetVBatAdc(DBAT_BAT_E bat);
LOCAL int32 _DBAT_AdcCvtToVol(DBAT_BAT_E bat,int32 adc);
LOCAL uint32 _DBAT_GetBatVol(DBAT_BAT_E bat);
LOCAL void _DBAT_CheckPowerOnCondition(void);
LOCAL BOOLEAN _DBAT_AdcCheckBatPlugIn(uint32 bat_vol);
LOCAL void _DBAT_BatInfoInit(DBAT_BAT_E bat);
LOCAL uint32 _DBAT_StateInit(void);
LOCAL void _DBAT_SendMessagetoClient (DBAT_MSG_SERVICE_E  msg, uint32 param);
LOCAL void _DBATSVR_default (BLOCK_ID id, uint32 argc, void *argv);

/**---------------------------------------------------------------------------*
 **                     LOCAL Function Definitions                            *
 **---------------------------------------------------------------------------*/

LOCAL void _DBAT_PrintTestLog(void)
{
    //DBAT_PRINT:"_DBAT_PrintTestLog start!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_103_112_2_17_23_3_29_432,(uint8*)"");
    //DBAT_PRINT:"DBAT:module state: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_104_112_2_17_23_3_29_433,(uint8*)"d", dbat_state.module_state);
    //DBAT_PRINT:"DBAT:current bat: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_105_112_2_17_23_3_29_434,(uint8*)"d", dbat_state.cur_bat);
    //DBAT_PRINT:"DBAT:current vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_106_112_2_17_23_3_29_435,(uint8*)"d", dbat_state.chgmng_info.bat_cur_vol);
    //DBAT_PRINT:"DBAT:statistic_vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_107_112_2_17_23_3_29_436,(uint8*)"d", dbat_state.chgmng_info.bat_statistic_vol);
    //DBAT_PRINT:"DBAT:bat_remain_cap: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_108_112_2_17_23_3_29_437,(uint8*)"d", dbat_state.chgmng_info.bat_remain_cap);
    //DBAT_PRINT:"DBAT:Idle bat: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_109_112_2_17_23_3_29_438,(uint8*)"d", _DBAT_GetIdleBat());
    //DBAT_PRINT:"DBAT:main bat info vol: %d,cap: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_110_112_2_17_23_3_29_439,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_MAIN_BAT].remain_cap);
    //DBAT_PRINT:"DBAT:aux bat info vol: %d,cap: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_111_112_2_17_23_3_29_440,(uint8*)"dd", bat_info[DBAT_AUX_BAT].vol,bat_info[DBAT_AUX_BAT].remain_cap);
    //DBAT_PRINT:"_DBAT_PrintTestLog end!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_112_112_2_17_23_3_29_441,(uint8*)"");
}

/*****************************************************************************/
//  Description:    dual battery module task entry.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_SrvThread (uint32 argc, void *argv)
{
    xSignalHeader	signal;

    _DBAT_StateInit();
    
    SCI_RegisterMsg( CHR_SERVICE, 
                (uint8)((CHR_CAP_IND) & 0xff), 
                (uint8)((CHR_MSG_MAX_NUM - 1) & 0xff), 
                _DBAT_CHGMNGCallBack);
    
    CHGMNG_SendMsgToChgTask (CHGMNG_MODULE_RESET_MSG,dbat_state.chgmng_info.bat_statistic_vol);

    for(;;)
    {
        signal = SCI_GetSignal (dbat_task_id);
        
        switch(signal->SignalCode)
        {
            case DBAT_BAT_MONITOR_MSG:
                _DBAT_BatMonitorHandler();
            break;
            
            case DBAT_CHR_CHARGE_START_IND:
                _DBAT_ChgStartHandler();
            break;
            
            case DBAT_CHR_WARNING_IND:
                _DBAT_WarningHandler();
            break;
            
            case DBAT_CHR_SHUTDOWN_IND:
                _DBAT_ShutDownHandler();
            break;

            case DBAT_CHR_CHARGE_FINISH:
                _DBAT_ChgFinishHandler();
            break;

            case DBAT_CHR_CHARGE_FAULT:
                _DBAT_ChgFaultHandler();
            break;
        
            case DBAT_CHARGER_PLUGOUT_MSG:
                _DBAT_AdpPlugOutHandler();
            break;
            
            case DBAT_M_BAT_PLUGOUT_MSG:
                _DBAT_BatPlugOutHandler(DBAT_MAIN_BAT);                                          
            break;
            
            case DBAT_A_BAT_PLUGOUT_MSG:
                _DBAT_BatPlugOutHandler(DBAT_AUX_BAT);   
            break;
            
            case DBAT_M_BAT_PLUGIN_MSG:
                _DBAT_BatPlugInHandler(DBAT_MAIN_BAT);
            break;
            case DBAT_A_BAT_PLUGIN_MSG:
                _DBAT_BatPlugInHandler(DBAT_AUX_BAT);
            break;
            
            case DBAT_SWITCH_BAT_REQ_MSG:
                _DBAT_SwBatReqHandler();
            break;             
            default:
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            break;      
        }
        
        SCI_FREE(signal);
        signal = NULL;
    }  
}

/*****************************************************************************/
//  Description:    battery monitor message handler .
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_BatMonitorHandler(void)
{
    uint32 a_bat_vol,m_bat_vol;
    
    if(/*(p_bat_detect->detect_mode == DBAT_BAT_DETECT_ADC)&&*/
        (p_hw_param->mutual_chg_mode != DBAT_NOT_SUPPORT_MUTUAL)) 
    {
       if(_DBAT_MutualMng(DBAT_MAX_BAT,SCI_FALSE))
       {
            //DBAT_PRINT:"DBAT:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!SCI_SLEEP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_205_112_2_17_23_3_29_442,(uint8*)"");
            
            SCI_SLEEP(DETECT_WAIT_VOL_DROP_TIME);  
       }
    }

    m_bat_vol = _DBAT_GetBatVol(DBAT_MAIN_BAT);
    a_bat_vol = _DBAT_GetBatVol(DBAT_AUX_BAT);

    _DBAT_BatStatistic(m_bat_vol,a_bat_vol);
    
    _DBAT_PrintTestLog();
    
    if(_DBAT_ADCDetectBatPresent(m_bat_vol,a_bat_vol))
    {
        return;
    }
#if 1
    if(dbat_state.module_state == DBAT_CONNECT_CHARGING)
    {
        if(_DBAT_GetIdleBat() == DBAT_MAX_BAT)
        {
            if(dbat_state.cur_bat == DBAT_MAIN_BAT)
            {
                _DBAT_MutualMng(DBAT_AUX_BAT,SCI_TRUE);
            }
            else
            {
                _DBAT_MutualMng(DBAT_MAIN_BAT,SCI_TRUE);
            }
        }
    }
#endif                     
    if(charging_mutual_req)
    {
        _DBAT_MutualChgRoutineInCharging();
    }
    
    _DBAT_SendMessagetoClient (DBAT_UPDATE_STATE_IND, 0);
}

/*****************************************************************************/
//  Description:    Mutual charge management .
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _DBAT_MutualMng(DBAT_BAT_E need_chg_bat,BOOLEAN on)
{
    static BOOLEAN record = SCI_FALSE;  //if aux Mutual and main differ,must modify here
    
    if(on == record)
    {
        //DBAT_PRINT:"DBAT:!!!!!!!!on == record!!!!!!!!!!!record %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_255_112_2_17_23_3_29_443,(uint8*)"d", record);
        return SCI_FALSE;
    }
    else
    {
        record = on;
        //DBAT_PRINT:"DBAT:-------on != record---------record %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_261_112_2_17_23_3_29_444,(uint8*)"d", record);
        p_prod_fun->MutualCharge(need_chg_bat,on);
        return SCI_TRUE;
    }
}

/*****************************************************************************/
//  Description:    Handle charge finish message.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_ChgFinishHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();
    CHGMNG_CHARGE_PARAM_T chg = CHGMNG_GetChgParam ();

    //DBAT_PRINT:"DBAT:ChgFinish m_vol: %d,a_vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_275_112_2_17_23_3_29_445,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
    
    if(idle_bat == DBAT_MAX_BAT) //only one battery
    {
        //DBAT_PRINT:"DBAT_CHARGE_FINISH0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_279_112_2_17_23_3_29_446,(uint8*)"");
        chg.rechg_vol = p_chg_param->rechg_vol;
        CHGMNG_SetChgParam (&chg);
        _DBAT_SendMessagetoClient (DBAT_CHARGE_FINISH, 0);
        return;
    }
    
    if(bat_info[idle_bat].vol > p_chg_param->idle_bat_full_vol)//dual battery is full
    {
        //DBAT_PRINT:"DBAT_CHARGE_FINISH1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_288_112_2_17_23_3_29_447,(uint8*)"");
        chg.rechg_vol = p_chg_param->rechg_vol;
        CHGMNG_SetChgParam (&chg);
        _DBAT_SendMessagetoClient (DBAT_CHARGE_FINISH, 0);
        return;
    }

    if(bat_info[idle_bat].vol > p_chg_param->chg_switch_vol)
    {
        charging_mutual_req  = SCI_FALSE;
        _DBAT_SwitchBatHandler(idle_bat);
        chg.rechg_vol = p_chg_param->chg_end_vol;
        CHGMNG_SetChgParam (&chg);
    }
    else
    {
        if(!_DBAT_CheckSupportMutualChg(idle_bat))
        {
            //DBAT_PRINT:"DBAT_CHARGE_FINISH2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_306_112_2_17_23_3_29_448,(uint8*)"");
            chg.rechg_vol = p_chg_param->rechg_vol;
            CHGMNG_SetChgParam (&chg);
            _DBAT_SendMessagetoClient (DBAT_CHARGE_FINISH, 0);
            return;  
        }
        charging_mutual_req = SCI_TRUE;
        chg.rechg_vol = p_chg_param->chg_end_vol;
        CHGMNG_SetChgParam (&chg);
    }
}

/*****************************************************************************/
//  Description:    query mutual charge mode.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _DBAT_CheckSupportMutualChg(DBAT_BAT_E aim_bat)
{
    BOOLEAN re = SCI_TRUE ;
    
    switch(p_hw_param->mutual_chg_mode)
    {
        case DBAT_NOT_SUPPORT_MUTUAL:
            re = SCI_FALSE;
        break;
        case DBAT_UNIDIRECTIONAL_M_TO_A:
            if(aim_bat != DBAT_AUX_BAT)
            {
                re = SCI_FALSE;
            }
        break;
        case DBAT_UNIDIRECTIONAL_A_TO_M:
            if(aim_bat != DBAT_MAIN_BAT)
            {
                re = SCI_FALSE;
            }
        break;
        default:
        break;
            
    }
    return re;
}

/*****************************************************************************/
//  Description:    Handle battery switch request message.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_SwBatReqHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();
    
    if(idle_bat != DBAT_MAX_BAT)
    {
        _DBAT_SwitchBatHandler(idle_bat);
    }
}

/*****************************************************************************/
//  Description:    Handle shutDown message from CHGMNG.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_ShutDownHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();

    if(idle_bat == DBAT_MAX_BAT)
    {
        //DBAT_PRINT:"DBAT:Shutdown3 m_vol: %d,a_vol: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_371_112_2_17_23_3_29_449,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
        _DBAT_SendMessagetoClient (DBAT_SHUTDOWN_IND, 0);
        return;
    }
    if((bat_info[idle_bat].vol < p_dischg_param->shutdown_vol) 
        ||(bat_info[idle_bat].vol < (dbat_state.chgmng_info.bat_statistic_vol + p_dischg_param->dischg_switch_gate))
        ||(bat_info[idle_bat].vol < p_dischg_param->dischg_switch_vol))
    {
        //DBAT_PRINT:"DBAT:Shutdown1 m_vol: %d,a_vol: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_379_112_2_17_23_3_29_450,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
        _DBAT_SendMessagetoClient (DBAT_SHUTDOWN_IND, 0);
        return;
    }

    //DBAT_PRINT:"DBAT:Shutdown2 m_vol: %d,a_vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_384_112_2_17_23_3_29_451,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
    
    _DBAT_SwitchBatHandler(idle_bat);    
    
    return;
}

/*****************************************************************************/
//  Description:    Handle warning message from CHGMNG.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_WarningHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();

    //DBAT_PRINT:"DBAT:Warning1 m_vol: %d,a_vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_398_112_2_17_23_3_29_452,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);

    if(idle_bat == DBAT_MAX_BAT)
    {
        _DBAT_SendMessagetoClient (DBAT_WARNING_IND, 0);
        return;
    }

    if((bat_info[idle_bat].vol < p_dischg_param->warning_vol))
    {
        _DBAT_SendMessagetoClient (DBAT_WARNING_IND, 0);
    }
        
    if(dbat_state.chgmng_info.bat_statistic_vol <= p_dischg_param->dischg_switch_vol)
    {
        if(bat_info[idle_bat].vol > (p_dischg_param->dischg_switch_vol + p_dischg_param->dischg_switch_gate))
        {
            //DBAT_PRINT:"DBAT:Warning2 m_vol: %d,a_vol: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_415_112_2_17_23_3_29_453,(uint8*)"dd", bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
            _DBAT_SwitchBatHandler(idle_bat);
        }
    }
    
    return;
}

/*****************************************************************************/
//  Description:    Handle starting charge message from CHGMNG.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_ChgStartHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();
    
    //DBAT_PRINT:"_DBAT_ChgStartHandler!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_430_112_2_17_23_3_29_454,(uint8*)"");
    
    dbat_state.module_state = DBAT_CONNECT_CHARGING;
    
    _DBAT_SendMessagetoClient (DBAT_CHARGE_START_IND, 0);
    
    charging_mutual_req = SCI_TRUE;

    if(p_prod_fun->ChargeCallBack != NULL)
    {
        p_prod_fun->ChargeCallBack(SCI_TRUE);
    }
    
    if(idle_bat == DBAT_MAX_BAT)    //only one battery
    {
        return;
    }
    
    if(_DBAT_CheckWorkPriorityChg() == idle_bat)
    {
        _DBAT_SwitchBatHandler(idle_bat);
    }
    
    return;
}

/*****************************************************************************/
//  Description:    Handle fault message when charge from CHGMNG.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_ChgFaultHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();

    //DBAT_PRINT:"_DBAT_ChgFaultHandler!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_463_112_2_17_23_3_29_455,(uint8*)"");
    
    _DBAT_SendMessagetoClient (DBAT_CHARGE_FAULT, 0);
    dbat_state.module_state = DBAT_EXCEPTION;

    if(p_prod_fun->ChargeCallBack != NULL)
    {
        p_prod_fun->ChargeCallBack(SCI_FALSE);
    }
    
    if(idle_bat == DBAT_MAX_BAT)    //only one battery
    {
        return;
    }

    if(_DBAT_CheckWorkPriorityDischg() == idle_bat)
    {
        _DBAT_SwitchBatHandler(idle_bat);
    }
    return;
}

/*****************************************************************************/
//  Description:    Handle adapter plugout message.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_AdpPlugOutHandler(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();
    
    //DBAT_PRINT:"DBAT:_DBAT_AdpPlugOutHandler!!!!!!!!!!!!!!!!!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_492_112_2_17_23_3_29_456,(uint8*)"");

    _DBAT_SendMessagetoClient (DBAT_CHARGE_DISCONNECT, 0);
    dbat_state.module_state = DBAT_DISCONNECT;

    if(p_prod_fun->ChargeCallBack != NULL)
    {
        p_prod_fun->ChargeCallBack(SCI_FALSE);
    }
    
    if(idle_bat == DBAT_MAX_BAT)    //only one battery
    {
        return;
    }

    if(_DBAT_CheckWorkPriorityDischg() == idle_bat)
    {
        _DBAT_SwitchBatHandler(idle_bat);
    }
    return;
}

/*****************************************************************************/
//  Description:    Handle battery plugin message.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_BatPlugInHandler(DBAT_BAT_E plugin_bat)
{
    if(dbat_state.p_bat[plugin_bat]!= NULL)
    {
        //DBAT_PRINT:"DBAT:Battery PLUGIN Again!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_521_112_2_17_23_3_29_457,(uint8*)"");
        return;
    }
    
    if(p_bat_detect->detect_mode == DBAT_BAT_DETECT_INT)
    {
        _DBAT_MutualMng(DBAT_MAX_BAT,SCI_FALSE);

        SCI_SLEEP(DETECT_WAIT_VOL_DROP_TIME);  
        
        bat_info[plugin_bat].vol = _DBAT_GetBatVol(plugin_bat);
    } 
    
    bat_info[plugin_bat].remain_cap = CHGMNG_VoltagetoPercentumDBat (plugin_bat,bat_info[plugin_bat].vol,SCI_TRUE);
    
    dbat_state.p_bat[plugin_bat] = &bat_info[plugin_bat];

    //DBAT_PRINT:"DBAT:Battery plugin:%d m_vol: %d,a_vol: %d......................"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_538_112_2_17_23_3_30_458,(uint8*)"ddd", plugin_bat,bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
     
    _DBAT_SendMessagetoClient (DBAT_BAT_PLUGIN_IND, plugin_bat);
    
    if(dbat_state.module_state == DBAT_DISCONNECT || dbat_state.module_state == DBAT_EXCEPTION)
    {
        if(_DBAT_CheckWorkPriorityDischg() == plugin_bat)
        {
            _DBAT_SwitchBatHandler(plugin_bat);
        }
    }
    else        ///charging
    {
        CHGMNG_CHARGE_PARAM_T chg = CHGMNG_GetChgParam();
        chg.rechg_vol = p_chg_param->chg_end_vol;
        CHGMNG_SetChgParam(&chg);
        charging_mutual_req = SCI_TRUE;
        if(_DBAT_CheckWorkPriorityChg() == plugin_bat)
        {
            _DBAT_SwitchBatHandler(plugin_bat);
        }
    }
    
    if(p_prod_fun->BatPlugCallBack != NULL)
    {
        p_prod_fun->BatPlugCallBack(plugin_bat,SCI_TRUE);
    }
}

/*****************************************************************************/
//  Description:    Handle battery plugout message.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_BatPlugOutHandler(DBAT_BAT_E plugout_bat)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();

    if(dbat_state.p_bat[plugout_bat] == NULL)
    {
        //DBAT_PRINT:"DBAT:Battery PLUGOUT Again!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_576_112_2_17_23_3_30_459,(uint8*)"");
        return;
    }

    if(p_bat_detect->detect_mode == DBAT_BAT_DETECT_INT)
    {
        //DBAT_PRINT:"DBAT:Battery PLUGOUT INT DETECT!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_582_112_2_17_23_3_30_460,(uint8*)"");
        
        _DBAT_MutualMng(DBAT_MAX_BAT,SCI_FALSE);
        
        SCI_SLEEP(DETECT_WAIT_VOL_DROP_TIME);  
        
        if(_DBAT_AdcCheckBatPlugIn(_DBAT_GetBatVol(plugout_bat)))
        {
            //DBAT_PRINT:"DBAT:Battery PLUGOUT INT ERR!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_590_112_2_17_23_3_30_461,(uint8*)"");
            return;
        }   
    }
        
    dbat_state.p_bat[plugout_bat] = NULL;
    
    _DBAT_SendMessagetoClient (DBAT_BAT_PLUGOUT_IND, plugout_bat);

    //DBAT_PRINT:"DBAT:Battery plugout:%d m_vol: %d,a_vol: %d................................."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_599_112_2_17_23_3_30_462,(uint8*)"ddd", plugout_bat,bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
    
    if(plugout_bat == dbat_state.cur_bat)
    {
        _DBAT_SwitchBatHandler(idle_bat);
    }

    if(p_prod_fun->BatPlugCallBack != NULL)
    {
        p_prod_fun->BatPlugCallBack(plugout_bat,SCI_FALSE);
    }
}

/*****************************************************************************/
//  Description:    mutual charge routine when charging.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_MutualChgRoutineInCharging(void)
{
    DBAT_BAT_E idle_bat = _DBAT_GetIdleBat();
    
    if((dbat_state.module_state == DBAT_DISCONNECT) 
        || (dbat_state.module_state == DBAT_EXCEPTION)
        || (idle_bat == DBAT_MAX_BAT)) ///only one battery
    {
        charging_mutual_req  = SCI_FALSE;
        return;
    }

    if(dbat_state.chgmng_info.bat_statistic_vol > (p_dischg_param->warning_vol + p_dischg_param->dischg_switch_gate))
    {
        //DBAT_PRINT:"_DBAT_MutualChgRoutineInCharging: >>>>>>>>"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_629_112_2_17_23_3_30_463,(uint8*)"");
        if(bat_info[idle_bat].vol < (p_chg_param->chg_switch_vol + p_chg_param->chg_switch_gate))
        {
            _DBAT_MutualMng(idle_bat,SCI_TRUE);
        }
        else
        {
           charging_mutual_req  = SCI_FALSE;
           _DBAT_MutualMng(idle_bat,SCI_FALSE);
           if(idle_bat == _DBAT_CheckWorkPriorityChg())
           {
                //DBAT_PRINT:"DBAT:_DBAT_MutualChgRoutineInCharging!!!!!!!_DBAT_SwitchBatHandler!!!!!!!!!!!bat:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_640_112_2_17_23_3_30_464,(uint8*)"d",idle_bat);
                _DBAT_SwitchBatHandler(idle_bat); 
           }
        }
    }
}

/*****************************************************************************/
//  Description:    query work mode when charging.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL DBAT_BAT_E _DBAT_CheckWorkPriorityChg(void)
{
    if(dbat_state.p_bat[DBAT_MAIN_BAT] == NULL)
    {
        return DBAT_AUX_BAT;
    }
    if(dbat_state.p_bat[DBAT_AUX_BAT] == NULL)
    {
        return DBAT_MAIN_BAT;
    }
    
    switch(p_hw_param->work_mode)
    {
        case DBAT_DISCHG_M_CHG_A:
            if(dbat_state.cur_bat != DBAT_AUX_BAT)
            {
                if(dbat_state.p_bat[DBAT_AUX_BAT]->vol > p_chg_param->chg_switch_vol)
                {
                    return DBAT_AUX_BAT;
                }
            }
        break;
        case DBAT_DISCHG_M_CHG_M:
        case DBAT_DISCHG_N_CHG_M:
            if(dbat_state.cur_bat != DBAT_MAIN_BAT)
            {
                if(dbat_state.p_bat[DBAT_MAIN_BAT]->vol > p_chg_param->chg_switch_vol)
                {
                    return DBAT_MAIN_BAT;
                }
            }
        break;
        default:
        break;
    }

    return DBAT_MAX_BAT;    
}

/*****************************************************************************/
//  Description:    query work mode when discharge.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL DBAT_BAT_E _DBAT_CheckWorkPriorityDischg(void)
{
    if(dbat_state.p_bat[DBAT_MAIN_BAT] == NULL)
    {
        return DBAT_AUX_BAT;
    }
    if(dbat_state.p_bat[DBAT_AUX_BAT] == NULL)
    {
        return DBAT_MAIN_BAT;
    }
    
    switch(p_hw_param->work_mode)
    {
        case DBAT_DISCHG_M_CHG_M:
        case DBAT_DISCHG_M_CHG_A:
        case DBAT_DISCHG_M_CHG_N:
            if(dbat_state.cur_bat != DBAT_MAIN_BAT)
            {
                if(dbat_state.p_bat[DBAT_MAIN_BAT]->vol > p_dischg_param->dischg_switch_vol)
                {
                    return DBAT_MAIN_BAT;
                }
            }
        break;
        case DBAT_DISCHG_N_CHG_M:
        case DBAT_DISCHG_N_CHG_N:
        break;
        default:
        break;
    }
    
    return DBAT_MAX_BAT;
}

/*****************************************************************************/
//  Description:    battery switch handler.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _DBAT_SwitchBatHandler(DBAT_BAT_E aim_bat)
{
    if(dbat_state.cur_bat == aim_bat || aim_bat == DBAT_MAX_BAT)
    {
        return SCI_TRUE;
    }
    
    //DBAT_PRINT:"DBAT:Sw1 aim_bat:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_736_112_2_17_23_3_30_465,(uint8*)"d", aim_bat);
    
    _DBAT_ResetChgmngCfg(aim_bat);

    if(!p_prod_fun->SelectBat(aim_bat))
    {
        _DBAT_ResetChgmngCfg(dbat_state.cur_bat);
        return SCI_FALSE;
    }

    //DBAT_PRINT:"DBAT:Sw2 aim_bat:%d m_vol: %d,a_vol: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_746_112_2_17_23_3_30_466,(uint8*)"ddd", aim_bat,bat_info[DBAT_MAIN_BAT].vol,bat_info[DBAT_AUX_BAT].vol);
    
    dbat_state.chgmng_info.bat_statistic_vol = bat_info[aim_bat].vol;
    dbat_state.chgmng_info.bat_cur_vol   =   bat_info[aim_bat].vol;
    dbat_state.chgmng_info.bat_remain_cap   = bat_info[aim_bat].remain_cap;
    dbat_state.cur_bat = aim_bat;

    //DBAT_PRINT:"DBAT:CHGMNG_SendMsgToChgTask CHGMNG_MODULE_RESET_MSG!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_753_112_2_17_23_3_30_467,(uint8*)"");
    CHGMNG_SendMsgToChgTask (CHGMNG_MODULE_RESET_MSG,dbat_state.chgmng_info.bat_statistic_vol);
    
    _DBAT_SendMessagetoClient (DBAT_SWITCH_BAT_IND, aim_bat);
        
    return SCI_TRUE;
    
}

/*****************************************************************************/
//  Description:    reset CHGMNG module config data.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_ResetChgmngCfg(DBAT_BAT_E aim_bat)
{
    CHGMNG_CHARGE_PARAM_T chg =  CHGMNG_GetChgParam ();
    chg.standard_chg_current    = p_chg_param->standard_chg_current[aim_bat];
    chg.usb_chg_current         = p_chg_param->usb_chg_current[aim_bat];
    chg.nonstandard_chg_current = p_chg_param->nonstandard_chg_current[aim_bat];
    CHGMNG_SetChgParam ((CHGMNG_CHARGE_PARAM_T*) &chg);
}

/*****************************************************************************/
//  Description:    Get idle battery number,
//  if don't have idle battery,return DBAT_MAX_BAT.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL DBAT_BAT_E _DBAT_GetIdleBat(void)
{
    if(dbat_state.p_bat[DBAT_MAIN_BAT] == NULL && dbat_state.p_bat[DBAT_AUX_BAT] == NULL)
    {
        SCI_ASSERT(0);  /*assert verified*/      
    }
    
    if((dbat_state.p_bat[DBAT_MAIN_BAT] == NULL) || (dbat_state.p_bat[DBAT_AUX_BAT] == NULL))
    {
        return DBAT_MAX_BAT;
    }

    if(dbat_state.cur_bat == DBAT_AUX_BAT)
    {
        return DBAT_MAIN_BAT;
    }
    else
    {
        return DBAT_AUX_BAT;
    }
}

/*****************************************************************************/
//  Description:    record battery statistic information,
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _DBAT_BatStatistic(uint32 m_bat_vol,uint32 a_bat_vol)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    
    dbat_state.chgmng_info = *p_chgmng_info;

    bat_info[DBAT_MAIN_BAT].vol = m_bat_vol;
    bat_info[DBAT_MAIN_BAT].remain_cap = CHGMNG_VoltagetoPercentumDBat (DBAT_MAIN_BAT,bat_info[DBAT_MAIN_BAT].vol,SCI_FALSE);
    bat_info[DBAT_AUX_BAT].vol = a_bat_vol;
    bat_info[DBAT_AUX_BAT].remain_cap = CHGMNG_VoltagetoPercentumDBat (DBAT_AUX_BAT,bat_info[DBAT_AUX_BAT].vol,SCI_FALSE);
}

/*****************************************************************************/
//  Description:    polling battery plug state through battery voltage,
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _DBAT_ADCDetectBatPresent(uint32 mbat_vol,uint32 abat_vol)
{
    BOOLEAN a_plug,m_plug;
    BOOLEAN result = SCI_FALSE;
    
    if(p_bat_detect->detect_mode != DBAT_BAT_DETECT_ADC)
    {
        return result;
    }
    
    a_plug = _DBAT_AdcCheckBatPlugIn(abat_vol);
    m_plug = _DBAT_AdcCheckBatPlugIn(mbat_vol);

    if(a_plug)  //plugin detection must be polled earlier than plug out detection;
    {
        if(dbat_state.p_bat[DBAT_AUX_BAT] == NULL)
        {
            result = SCI_TRUE;
            //DBAT_PRINT:"DBAT: ADCDetect DBAT_A_BAT_PLUGIN_MSG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_836_112_2_17_23_3_30_468,(uint8*)"");
            DBAT_SendMsgToDBatTask (DBAT_A_BAT_PLUGIN_MSG);
        }
    }
    
    if(m_plug)
    {
        if(dbat_state.p_bat[DBAT_MAIN_BAT] == NULL)
        {
            result = SCI_TRUE;
            //DBAT_PRINT:"DBAT: ADCDetect DBAT_M_BAT_PLUGIN_MSG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_846_112_2_17_23_3_30_469,(uint8*)"");
            DBAT_SendMsgToDBatTask (DBAT_M_BAT_PLUGIN_MSG);
        }
    }

    if(!a_plug)
    {
        if(dbat_state.p_bat[DBAT_AUX_BAT] != NULL)
        {
            result = SCI_TRUE;
            //DBAT_PRINT:"DBAT: ADCDetect DBAT_A_BAT_PLUGOUT_MSG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_856_112_2_17_23_3_30_470,(uint8*)"");
            DBAT_SendMsgToDBatTask (DBAT_A_BAT_PLUGOUT_MSG);
        }
        /*
        if(dbat_state.module_state == DBAT_CONNECT_CHARGING)
        {
            //DBAT_PRINT:"DBAT: not present A bat"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_862_112_2_17_23_3_30_471,(uint8*)"");
            _DBAT_MutualMng(DBAT_AUX_BAT,SCI_TRUE);
        }
        */
    }

    if(!m_plug)
    {
        if(dbat_state.p_bat[DBAT_MAIN_BAT] != NULL)
        {
            result = SCI_TRUE;
            //DBAT_PRINT:"DBAT: ADCDetect DBAT_M_BAT_PLUGOUT_MSG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_873_112_2_17_23_3_30_472,(uint8*)"");
            DBAT_SendMsgToDBatTask (DBAT_M_BAT_PLUGOUT_MSG);
        }
        /*
        if(dbat_state.module_state == DBAT_CONNECT_CHARGING)
        {
            //DBAT_PRINT:"DBAT: not present M bat"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_879_112_2_17_23_3_30_473,(uint8*)"");
            _DBAT_MutualMng(DBAT_MAIN_BAT,SCI_TRUE);
        }
        */
    }

    return result;

}

/*****************************************************************************/
// 	Description : This function is the callback of CHR
//	Global resource dependence :
//  Author:       Mingwei.Zhang
//	Note:
/*****************************************************************************/
LOCAL void _DBAT_CHGMNGCallBack(uint32 taskID, uint32 argc, void *argv)
{
    DBAT_MSG_E msg;

	switch (argc)
	{
	case CHR_CAP_IND:
        msg = DBAT_BAT_MONITOR_MSG;
		break;
	case CHR_CHARGE_START_IND:
        msg = DBAT_CHR_CHARGE_START_IND;
		break;
	case CHR_WARNING_IND:
		msg = DBAT_CHR_WARNING_IND;
		break;
	case CHR_SHUTDOWN_IND:
		msg = DBAT_CHR_SHUTDOWN_IND;
		break;
	case CHR_CHARGE_FINISH:
		msg = DBAT_CHR_CHARGE_FINISH;
		break;
	case CHR_CHARGE_DISCONNECT:
        msg = DBAT_CHARGER_PLUGOUT_MSG;
		break;
	case CHR_CHARGE_FAULT:
        msg = DBAT_CHR_CHARGE_FAULT;
		break;

	default:
		//DBAT_PRINT:"DBAT:_DBAT_CHGMNGCallBack err msg!!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_922_112_2_17_23_3_30_474,(uint8*)"");
		return;
	}
    
    DBAT_SendMsgToDBatTask (msg);
    
}

/*****************************************************************************/
//  Description:    Get vbat ADC value.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 _DBAT_GetVBatAdc(DBAT_BAT_E bat)
{
    int i,j, temp;
    uint32 vbat_result[ADC_SAMPLING_NUM];

    SCI_ASSERT(bat < DBAT_MAX_BAT); /*assert verified*/
    
    for (i = 0; i < ADC_SAMPLING_NUM; i++)
    {
        vbat_result[i] = p_prod_fun->GetBatADC(bat);

        //loop for delay
        for (j = ADC_SAMPLING_DELAY - 1; j >= 0; j--)
        {
            ;
        }
    }

    for (j=1; j<=ADC_SAMPLING_NUM-1; j++)
    {
        for (i=0; i<ADC_SAMPLING_NUM -j; i++)
        {
            if (vbat_result[i] > vbat_result[i+1])
            {
                temp = vbat_result[i];
                vbat_result[i] = vbat_result[i+1];
                vbat_result[i+1] = temp;
            }
        }
    }
    //DBAT_PRINT:"DBAT:_DBAT_GetVBatAdc:%d,bat:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_963_112_2_17_23_3_30_475,(uint8*)"dd",bat,vbat_result[ADC_SAMPLING_NUM/2]);
    return vbat_result[ADC_SAMPLING_NUM/2];
}

/*****************************************************************************/
//  Description:    ADC value convert to voltage.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/   
LOCAL int32 _DBAT_AdcCvtToVol(DBAT_BAT_E bat,int32 adc)
{
    int32 result;

    SCI_ASSERT(bat < DBAT_MAX_BAT); /*assert verified*/
    if(adc_cal[bat].slope != 0)
    {   
        //DBAT_PRINT:"DBAT:_DBAT_AdcCvtToVol slope:%d,zoom_in:%d,x0_adc:%d,y0_vol:%d,adc:%d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_978_112_2_17_23_3_30_476,(uint8*)"ddddd",adc_cal[bat].slope,adc_cal[bat].zoom_in,adc_cal[bat].x0_adc,adc_cal[bat].y0_vol,adc);
        
        result = ((adc - adc_cal[bat].x0_adc)*adc_cal[bat].slope)/adc_cal[bat].zoom_in + adc_cal[bat].y0_vol;
        
        if(result < 0)
        {
            result = 0;
        }
    }
    else    ///dual battery adc don't calibration,so we use ADC_CHANNEL_VBAT to calculate the voltage 
    {       ///v1 = vbat_vol*0.268 = vol_bat_m * r2 /(r1+r2)
        uint32 vbat_vol = CHGMNG_AdcvalueToVoltage (adc);
        uint32 m,n;
        
        //DBAT_PRINT:"DBAT:_DBAT_AdcCvtToVol use vbat!vbat_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_992_112_2_17_23_3_30_477,(uint8*)"d",vbat_vol);
        
        SCI_ASSERT((p_hw_param->adc_resistor_gnd + p_hw_param->adc_resistor_vbat) < 1500);  /*assert verified*/
        
        n = VOL_DIV_P2*p_hw_param->adc_resistor_gnd;
        m = vbat_vol*VOL_DIV_P1*(p_hw_param->adc_resistor_gnd + p_hw_param->adc_resistor_vbat);
        result = (m + n/2)/n;
    }
    return result;
}

/*****************************************************************************/
//  Description:    Get battery voltage.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/   
LOCAL uint32 _DBAT_GetBatVol(DBAT_BAT_E bat)
{
    return _DBAT_AdcCvtToVol(bat,_DBAT_GetVBatAdc(bat));
}

/*****************************************************************************/
//  Description:    Check PowerOn Condition through battery voltage.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/  
LOCAL void _DBAT_CheckPowerOnCondition(void)
{
    uint32 m_bat_vol,a_bat_vol;
    uint32 poweron_condition;

    SCI_ASSERT((p_dischg_param != NULL));   /*assert to do*/

    if (CHGMNG_IsChargeConnect())
    {
        return;
    }
    
    m_bat_vol = _DBAT_GetBatVol(DBAT_MAIN_BAT);
    a_bat_vol = _DBAT_GetBatVol(DBAT_AUX_BAT);
    
    poweron_condition = p_dischg_param->shutdown_vol + p_dischg_param->dischg_switch_gate;
    
    if((m_bat_vol < poweron_condition) && (a_bat_vol < poweron_condition))
    {
        POWER_PowerOff();
    }
    
    return;
}

/*****************************************************************************/
//  Description:    Check battery plugin through ADC value.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/  
LOCAL BOOLEAN _DBAT_AdcCheckBatPlugIn(uint32 bat_vol)
{
    if(bat_vol > (p_bat_detect->detect_vol + p_bat_detect->detect_vol_gate))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    Init battery information.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/ 
LOCAL void _DBAT_BatInfoInit(DBAT_BAT_E bat)
{
    bat_info[bat].vol= _DBAT_GetBatVol(bat);
    bat_info[bat].remain_cap = CHGMNG_VoltagetoPercentumDBat(DBAT_AUX_BAT,bat_info[bat].vol,SCI_TRUE);
    //DBAT_PRINT:"DBAT:_DBAT_BatInfoInit,bat:%d vol:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1061_112_2_17_23_3_31_478,(uint8*)"dd",bat,bat_info[bat].vol);
    if(_DBAT_AdcCheckBatPlugIn(bat_info[bat].vol))
    {
        dbat_state.p_bat[bat] = &bat_info[bat];
        p_prod_fun->BatPlugCallBack(bat,SCI_TRUE);
    }
    else
    {
        dbat_state.p_bat[bat] = NULL;
        p_prod_fun->BatPlugCallBack(bat,SCI_FALSE);
    }
}

/*****************************************************************************/
//  Description:    Init dual battery module information.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/ 
LOCAL uint32 _DBAT_StateInit(void)
{
    //DBAT_PRINT:"DBAT:state init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1079_112_2_17_23_3_31_479,(uint8*)"");
    
    dbat_state.module_state = DBAT_DISCONNECT;
    
    dbat_state.work_mode = p_hw_param->work_mode;
    dbat_state.prod_type = p_hw_param->prod_type;

    if((dbat_state.p_bat[DBAT_MAIN_BAT] != NULL)&&(dbat_state.p_bat[DBAT_AUX_BAT] != NULL))
    {
        if((dbat_state.p_bat[DBAT_MAIN_BAT]->vol > p_dischg_param->dischg_switch_vol)
            ||(dbat_state.p_bat[DBAT_AUX_BAT]->vol < dbat_state.p_bat[DBAT_MAIN_BAT]->vol + p_dischg_param->dischg_switch_gate))
        {
            //DBAT_PRINT:"DBAT:1init SelectBat(DBAT_MAIN_BAT)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1091_112_2_17_23_3_31_480,(uint8*)"");
            if(p_prod_fun->SelectBat(DBAT_MAIN_BAT))
            {
                dbat_state.cur_bat = DBAT_MAIN_BAT;
            }
            else
            {
                dbat_state.cur_bat = DBAT_AUX_BAT;
            }
        }
        else
        {
            //DBAT_PRINT:"DBAT:2init SelectBat(DBAT_AUX_BAT)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1103_112_2_17_23_3_31_481,(uint8*)"");
            if(p_prod_fun->SelectBat(DBAT_AUX_BAT))
            {
                dbat_state.cur_bat = DBAT_AUX_BAT;
            }
            else
            {
                dbat_state.cur_bat = DBAT_MAIN_BAT;
            }
        }
    }
    else if(dbat_state.p_bat[DBAT_AUX_BAT] == NULL)
    {
        if(!p_prod_fun->SelectBat(DBAT_MAIN_BAT))
        {
            //DBAT_PRINT:"DBAT:err: _DBAT_StateInit DBAT_MAIN_BAT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1118_112_2_17_23_3_31_482,(uint8*)"");
        }
        dbat_state.cur_bat = DBAT_MAIN_BAT;
    }
    else if(dbat_state.p_bat[DBAT_MAIN_BAT] == NULL)
    {
        
        if(!p_prod_fun->SelectBat(DBAT_AUX_BAT))
        {
            //DBAT_PRINT:"DBAT:err: _DBAT_StateInit DBAT_AUX_BAT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1127_112_2_17_23_3_31_483,(uint8*)"");
        }
        dbat_state.cur_bat = DBAT_AUX_BAT;
    }
    else
    {
        SCI_ASSERT(0);  /*assert to do*/
    }

    if(dbat_state.cur_bat == DBAT_AUX_BAT)
    {
        dbat_state.chgmng_info.bat_cur_vol =  bat_info[DBAT_AUX_BAT].vol;
        dbat_state.chgmng_info.bat_statistic_vol = bat_info[DBAT_AUX_BAT].vol;
        dbat_state.chgmng_info.bat_remain_cap = bat_info[DBAT_AUX_BAT].remain_cap;
    }
    else
    {
        dbat_state.chgmng_info.bat_cur_vol =  bat_info[DBAT_MAIN_BAT].vol;
        dbat_state.chgmng_info.bat_statistic_vol = bat_info[DBAT_MAIN_BAT].vol;
        dbat_state.chgmng_info.bat_remain_cap = bat_info[DBAT_MAIN_BAT].remain_cap;

    }

    {
        CHGMNG_CHARGE_PARAM_T chg;
        CHGMNG_DISCHARGE_PARAM_T dischg;

        chg.rechg_vol               = p_chg_param->rechg_vol;
        chg.chg_end_vol             = p_chg_param->chg_end_vol;
        chg.bat_safety_vol          = p_chg_param->bat_safety_vol;
        chg.standard_chg_current    = p_chg_param->standard_chg_current[dbat_state.cur_bat];
        chg.usb_chg_current         = p_chg_param->usb_chg_current[dbat_state.cur_bat];
        chg.nonstandard_chg_current = p_chg_param->nonstandard_chg_current[dbat_state.cur_bat];
        chg.chg_timeout             = p_chg_param->chg_timeout;
        CHGMNG_SetChgParam ((CHGMNG_CHARGE_PARAM_T*) &chg);

        dischg.warning_vol          = p_dischg_param->warning_vol;
        dischg.shutdown_vol         = p_dischg_param->shutdown_vol;
        dischg.deadline_vol         = p_dischg_param->deadline_vol;
        dischg.warning_count        = p_dischg_param->warning_count;
        CHGMNG_SetDischgParam ((CHGMNG_DISCHARGE_PARAM_T*)&dischg);
            
        CHGMNG_SetOtpParam((CHGMNG_OTP_PARAM_T*) p_otp_param);
        CHGMNG_SetOvpParam ((CHGMNG_OVP_PARAM_T*) p_ovp_param);
    }

    return 0;
}

/*****************************************************************************/
//  Description:    Send message to client.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/ 
#define MESSAGE_BUFFER_LEN 6
LOCAL void _DBAT_SendMessagetoClient (DBAT_MSG_SERVICE_E  msg, uint32 param)
{
    static uint32 msg_count = 1;
    static uint32 recent_message[MESSAGE_BUFFER_LEN][2] = {0};
    
    if (DBAT_MSG_MAX_NUM == recent_message[0][0])
    {
        int32 i = 1;

        while (i < MESSAGE_BUFFER_LEN)
        {
            if ( (recent_message[i][0] != 0))
            {
                if (SCI_SUCCESS != SCI_SendEventToClient (DBAT_SERVICE, recent_message[i][0], (void *)recent_message[i][1]))
                {
                    if ( (msg != DBAT_UPDATE_STATE_IND) && (msg != DBAT_WARNING_IND))
                    {
                        recent_message[msg_count][0] = msg;
                        recent_message[msg_count][1] = param;
                        msg_count++;

                        if (MESSAGE_BUFFER_LEN == msg_count)
                        {
                            SCI_PASSERT (SCI_FALSE, ("DBAT:System init time is too long!"));/*assert verified*/
                        }
                    }

                    return;
                }
                else
                {
                    //DBAT_PRINT:"DBAT3:%d has been send!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1211_112_2_17_23_3_31_484,(uint8*)"d", recent_message[i][0]);
                    recent_message[i][0] = 0;
                }
            }

            i++;
        }

        msg_count = 1;
        recent_message[0][0] = 0;
    }

    if (SCI_SUCCESS != SCI_SendEventToClient (DBAT_SERVICE, (uint32) msg, (void *) param))
    {
        //DBAT_PRINT:"DBAT1:%d has been saved!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1225_112_2_17_23_3_31_485,(uint8*)"d", msg);

        if ( (msg != DBAT_UPDATE_STATE_IND) && (msg != DBAT_WARNING_IND))
        {
            recent_message[0][0] = DBAT_MSG_MAX_NUM;        //This is used to indicate that we have had messages to handle.
            recent_message[msg_count][0] = msg;
            recent_message[msg_count][1] = param;
            msg_count++;

            if (MESSAGE_BUFFER_LEN == msg_count)
            {
                SCI_PASSERT (SCI_FALSE, ("DBAT System init time is too long"));/*assert verified*/
            }
        }
    }
}

/*****************************************************************************/
//  Description:    Default callback function. Called if client has no callback function..
//                      This is a dummy function.
//  Author:         Mingwei.Zang
//  Note:
/*****************************************************************************/
LOCAL void _DBATSVR_default (BLOCK_ID id, uint32 argc, void *argv)
{
}


/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Definitions                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    dual battery module Init function,be called when system setup.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_Init(DBAT_PROD_FUN_T* p_fun, DBAT_PROD_CFG_T* p_param)
{

    SCI_PASSERT ((p_fun != NULL),("prod function table error!!!"));   /*assert verified*/
    SCI_PASSERT ((p_param != NULL),("prod cfg table error!!!"));   /*assert verified*/

    //DBAT_PRINT:"DBAT:DBAT_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUALBAT_HAL_1263_112_2_17_23_3_31_486,(uint8*)"");
        
    if((p_fun->Init == NULL) || (p_fun->PowerOff == NULL)
        || (p_fun->MutualCharge == NULL) || (p_fun->GetBatADC == NULL)||(p_fun->SelectBat == NULL) 
        || (p_fun->GetBatPresent == NULL) || (p_fun->GetCurBat == NULL) ||(p_fun->BatPlugCallBack == NULL)
        || (p_fun->ChargeCallBack == NULL) || (p_fun->CalibrationModeInit == NULL))
    {
        SCI_PASSERT (0,("prod function element error!!!"));   /*assert verified*/
    }

    //init dual battery parameter
    p_dischg_param = &(p_param->dischg_param);
    p_chg_param = &(p_param->chg_param);
    p_ovp_param = &(p_param->ovp_param);
    p_otp_param = &(p_param->otp_param);
    p_bat_detect = &(p_param->detct_param);
    p_hw_param = &(p_param->hw_param);    
    p_prod_fun = p_fun;

    //dual battery IC init
    if(p_prod_fun->Init != NULL)
    {
        (p_prod_fun->Init)();
    }

    p_prod_fun->MutualCharge(DBAT_MAX_BAT,SCI_FALSE);
    
    _DBAT_CheckPowerOnCondition();

    _DBAT_BatInfoInit(DBAT_MAIN_BAT);
    _DBAT_BatInfoInit(DBAT_AUX_BAT);

    //_DBAT_StateInit();

    dbat_task_id  = SCI_CreateThread("DBAT_SRV",
    							"Q_DBAT_SRV",
    							_DBAT_SrvThread ,
    							0,
    							0,
    							DBAT_STACK_SIZE ,
    							DBAT_QUEUE_NUM,
    							DBAT_PRIORITY ,
    							SCI_PREEMPT,
    							SCI_AUTO_START);
    
    SCI_CreateClientList (DBAT_SERVICE, (DBAT_MSG_MAX_NUM & 0x0ff),
                          (REG_CALLBACK) _DBATSVR_default);
}

/*****************************************************************************/
//  Description:    Send message to dual battery task.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_SendMsgToDBatTask (DBAT_MSG_E msg)
{
    xSignalHeader   signal;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());
    
    SCI_SEND_SIGNAL(signal,dbat_task_id);
}

/*****************************************************************************/
//  Description:    Set battery calibration data.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_SetBatAdcCal(DBAT_BAT_E bat, CHGMNG_ADC_CAL_T *cal_info)
{
    if((cal_info != NULL) && (bat < DBAT_MAX_BAT))
    {
        adc_cal[bat] = *cal_info;
    }
}

/*****************************************************************************/
//  Description:    Get dual battery module information.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_STATE_INFO_T* DBAT_GetState(void)
{
    SCI_DisableIRQ(); 
    dbat_state_to_app = dbat_state;
    SCI_RestoreIRQ();
    
    return &dbat_state_to_app;
}

