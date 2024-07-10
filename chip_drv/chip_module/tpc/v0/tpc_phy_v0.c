/******************************************************************************
 ** File Name:        Tpc_phy_v0.c                                               *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             09/16/2009                                              *
 ** Copyright:        2009 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 06/10/2008      Tao.Feng        Create                                    *
 ** 09/16/2009      Yi.Qiu          Reconstruct TPC driver                    *


 ******************************************************************************/

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
//common head file
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "tpc_reg_v0.h"
#include "adc_drvapi.h"
#include "tpc_drvapi.h"
#include "isr_drvapi.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "adc_internal.h"
#include "Tp_cfg.h"

#define TOUCHPANEL_SW_HW_TRUE

#define _TPDRV_DEBUG
#ifdef _TPDRV_DEBUG
#include "sci_log.h"

#define TPDRV_ASSERT(_x_)                SCI_ASSERT(_x_)
#define TPDRV_VERIFY(_x_)                SCI_ASSERT(_x_)
#define TPDRV_ERR_TRACE(_x_)            SCI_TraceLow _x_
#define TPDRV_LOG_TRACE(_x_)            SCI_TraceLow _x_


#define TPDRV_INF_TRACE(_x_)            SCI_TraceLow _x_
#else
#define TPDRV_ASSERT(_x_)                ((void)0)
#define TPDRV_VERIFY(_x_)                ((void)(_x_))
#define TPDRV_ERR_TRACE(_x_)            ((void)0)
#define TPDRV_LOG_TRACE(_x_)            ((void)0)


#define TPDRV_INF_TRACE(_x_)            ((void)0)
#endif /* _TPC_DEBUG */

#if defined(TOUCHPANEL_SW_HW_TRUE)
#include "gpio_drvapi.h"

#define TPC_BUF_LENGTH                  8

#define TP_GPIO_XL              90
#define TP_GPIO_XR              91
#define TP_GPIO_YU              92
#define TP_GPIO_YD              93

#else
#define TPC_BUF_LENGTH                  4
#endif //endof defined(TOUCHPANEL_SW_HW_TRUE)

#define TPC_INT_DOWN            1
#define TPC_INT_UP              2
#define TPC_INT_DONE            4

INPUT_BUFFER_INIT (int32,64)


/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef union
{
    struct
    {
        uint16 data_y;
        uint16 data_x;
    } data;
    uint32 dwValue;
} TPC_DATA_U;

typedef enum
{
    TP_STATE_SAMPLING_X,
    TP_STATE_SAMPLING_Y
}TP_SAMPLING_STATE_E;
/******************************************************************************
 * LOCAL fucntion declaration
 ******************************************************************************/
LOCAL TP_CFG_INFO_PTR s_tp_cfginfo_ptr;
LOCAL BOOLEAN is_first_down = SCI_FALSE;
LOCAL TP_SAMPLING_STATE_E s = TP_STATE_SAMPLING_X;
LOCAL TPC_DATA_U tpc_data;

LOCAL ISR_EXE_T _IrqHandler (void);
LOCAL BOOLEAN _Run (void);
LOCAL void _Stop (void);
LOCAL BOOLEAN _FetchData (TPC_DATA_U *data);

LOCAL void _TPCHisrFunc (uint32 i, void *d);
LOCAL void _HandleTPCHisr(int int_type);


/******************************************************************************
 * TPC_Init
 ******************************************************************************/
PUBLIC void TPDRV_Init (void)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;
    s_tp_cfginfo_ptr = TP_GetCfgInfo();

    //Enable TPC module
    ctl_tpc->ctrl.mBits.tpc_en = SCI_TRUE;

    //Config pen request polarity
    ctl_tpc->ctrl.mBits.pen_req_polarity = 0;

    //Config tpc mode
    ctl_tpc->ctrl.mBits.tpc_mode = 0;
    ctl_tpc->ctrl.mBits.pclk_prescale = s_tp_cfginfo_ptr->data_period;/*default set: 12ms*/

    //Enable TPC interrupt
    ctl_tpc->int_en.mBits.down_irq_en = SCI_TRUE;
#if defined(TOUCHPANEL_SW_HW_TRUE)	
    ctl_tpc->int_en.mBits.up_irq_en = SCI_FALSE;
#else
    ctl_tpc->int_en.mBits.up_irq_en = SCI_TRUE;
#endif	
    ctl_tpc->int_en.mBits.done_irq_en = SCI_TRUE;

    //Config ADC channel
    ADC_SetScale (ADC_SCALE_3V);
    ADC_ConfigTPC (TPC_CHANNEL_X, TPC_CHANNEL_Y);

    //Config TPC sample properity. 
    //Without pclk prescale, the data below is regard as 1ms period sending data to server.
    ctl_tpc->sample_ctrl.mBits.sample_interval = 0x14;
    ctl_tpc->sample_ctrl.mBits.sample_num      = 2;
    ctl_tpc->sample_ctrl.mBits.point_interval  = 0x13;
    ctl_tpc->sample_ctrl.mBits.data_interval = 0x100;//0x400;

    //Config TPC filter properity
    ctl_tpc->filter_ctrl.mBits.filter_en = SCI_TRUE;
    ctl_tpc->filter_ctrl.mBits.filter_mode = 1;
    ctl_tpc->filter_ctrl.mBits.filter_tap = TPC_BUF_LENGTH;

    //Config TPC debounce properity
    ctl_tpc->bounce_ctrl.mBits.debounce_en = SCI_TRUE;
    ctl_tpc->bounce_ctrl.mBits.debounce_num = 10;

    //Config TPC buffer length
    ctl_tpc->buf_ctrl.mBits.buf_len = TPC_BUF_LENGTH;

    //Clear TPC interrupt
    ctl_tpc->int_clr.mBits.down_irq_clr = SCI_TRUE;
    ctl_tpc->int_clr.mBits.up_irq_clr = SCI_TRUE;
    ctl_tpc->int_clr.mBits.done_irq_clr = SCI_TRUE;

    //Register TPC ISR handler
    ISR_RegHandler_Ex (TB_TPC_INT, (TB_ISR) _IrqHandler, _TPCHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);
}
/******************************************************************************
 * TPDRV_Enable
 ******************************************************************************/
PUBLIC void TPDRV_Enable (void)
{
    CHIPDRV_EnableIRQINT (TB_TPC_INT);
}

/******************************************************************************
 * TPDRV_Enable
 ******************************************************************************/
PUBLIC void TPDRV_Disable (void)
{
    CHIPDRV_DisableIRQINT (TB_TPC_INT);
}

/******************************************************************************
 * TPC_EnableCoord
 ******************************************************************************/
PUBLIC void TPDRV_EnableCoord (void)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    //Enable coordinate calculation
    ctl_tpc->calc_ctrl.mBits.calc_en = SCI_TRUE;

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_EnableCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_179_112_2_17_23_7_23_1319,(uint8*)"");
}

/******************************************************************************
 * TPC_DisableCoord
 ******************************************************************************/
PUBLIC void TPDRV_DisableCoord (void)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    //Disable coordinate calculation
    ctl_tpc->calc_ctrl.mBits.calc_en = SCI_FALSE;

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_DisableCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_191_112_2_17_23_7_23_1320,(uint8*)"");
}

/******************************************************************************
 * TPC_ConfigCoord
 ******************************************************************************/
PUBLIC void TPDRV_ConfigCoord (
    int x_slop,
    int x_offset,
    int y_slop,
    int y_offset,
    BOOLEAN xy_exchange)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;


    if(xy_exchange == SCI_FALSE)
    {
        //Config x coordinate coef
        ctl_tpc->x_coef_ctrl.mBits.x_coef_a = x_slop;
        ctl_tpc->x_coef_ctrl.mBits.x_coef_b = x_offset;

        //Config y coordinate coef
        ctl_tpc->y_coef_ctrl.mBits.y_coef_a = y_slop;
        ctl_tpc->y_coef_ctrl.mBits.y_coef_b = y_offset;
    }
    else
    {
        ctl_tpc->ctrl.mBits.xy_input_switch = SCI_TRUE;			
        //Config x coordinate coef
        ctl_tpc->x_coef_ctrl.mBits.x_coef_a = y_slop;
        ctl_tpc->x_coef_ctrl.mBits.x_coef_b = y_offset;

        //Config y coordinate coef
        ctl_tpc->y_coef_ctrl.mBits.y_coef_a = x_slop;
        ctl_tpc->y_coef_ctrl.mBits.y_coef_b = x_offset;	
    }

    //TPDRV_LOG_TRACE(("[TPDRV:TPDRV_ConfigCoord]"));
}

/******************************************************************************
 * TPC_IrqHandler
 ******************************************************************************/
LOCAL ISR_EXE_T _IrqHandler (void)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    //TPDRV_LOG_TRACE ( ("[_IrqHandler]status:0x%x-0x%x",
    //ctl_tpc->int_status.dwValue, OS_GetTickCount()));

    //Pen Down interrtup
    if (ctl_tpc->int_status.mBits.down_irq)
    {
        IsrWriteBuffer (TPC_INT_DOWN);
        //Clear down interrupt*/
        ctl_tpc->int_clr.mBits.up_irq_clr = SCI_TRUE;
        ctl_tpc->int_clr.mBits.down_irq_clr = SCI_TRUE;
    }
    //Pen Up interrupt
    else if (ctl_tpc->int_status.mBits.up_irq)
    {
        IsrWriteBuffer (TPC_INT_UP);

        //disable done interrupt
        ctl_tpc->int_en.mBits.done_irq_en = SCI_FALSE;
        //Clear up interrupt*/
        ctl_tpc->int_clr.mBits.done_irq_clr = SCI_TRUE;
        ctl_tpc->int_clr.mBits.up_irq_clr = SCI_TRUE;
    }
    //Sample Done interrupt
    else if (ctl_tpc->int_status.mBits.done_irq)
    {
        IsrWriteBuffer (TPC_INT_DONE);
        //Clear done interrupt*/
        ctl_tpc->int_clr.mBits.done_irq_clr = SCI_TRUE;
    }
    //Error occured
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     ("[TPDRV:TPDRV_IrqHandler] Undefined irq"));
    }

    return CALL_HISR;
}

/******************************************************************************
 * _TPCHisrFunc
 ******************************************************************************/
LOCAL void _TPCHisrFunc (uint32 i, void *d)
{
    int32  int_type = 0;

    while (!threadReadBuffer (&int_type))
    {
        _HandleTPCHisr (int_type);
    }
}

/******************************************************************************
 * _HandleTPCHisr
 ******************************************************************************/
LOCAL void _HandleTPCHisr(int int_type)
{
    TB_MSG msg_ptr;
    BOOLEAN result;
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;
    
    //Pen Down interrtup
    if (int_type == TPC_INT_DOWN)
    {
#if defined(TOUCHPANEL_SW_HW_TRUE)	
        //TPDRV_LOG_TRACE:"[TPDRV:Down at %d] "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_301_112_2_17_23_7_23_1321,(uint8*)"d",SCI_GetTickCount());

        GPIO_Enable (TP_GPIO_XL);
        GPIO_Enable (TP_GPIO_XR);
        GPIO_Enable (TP_GPIO_YU);
        GPIO_Enable (TP_GPIO_YD);
     
#endif
        
        //Run TPC
        result = _Run();
        
        if (SCI_TRUE == result)
        {
            is_first_down = SCI_TRUE;
        }
        else
        {    
#if defined(TOUCHPANEL_SW_HW_TRUE)	            
            GPIO_SetDirection (TP_GPIO_YD, SCI_TRUE);
            GPIO_SetValue (TP_GPIO_YD, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_XL, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_XR, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
            CHIP_REG_SET(PIN_TP_XL_REG, PIN_I_EN | PIN_SPU_EN  | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_3 );
            CHIP_REG_SET(PIN_TP_XR_REG, PIN_Z_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3 );
            CHIP_REG_SET(PIN_TP_YU_REG, PIN_Z_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3);
            CHIP_REG_SET(PIN_TP_YD_REG, PIN_O_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3 );            
#endif                   
            return;
        }
        
        //enable done interrupt
        ctl_tpc->int_en.mBits.done_irq_en = SCI_TRUE;

        return ;
    }
    //Pen Up interrupt
    else if (int_type == TPC_INT_UP)
    {
        //Stop TPC
        _Stop();

        ctl_tpc->int_clr.mBits.done_irq_clr = SCI_TRUE;
        //TP sever need deal with pen up coordinate
        msg_ptr.wparam = 0;
        msg_ptr.message = TB_TP_UP;
    }
    //Sample Done interrupt
    else if (int_type == TPC_INT_DONE)
    {
#if defined(TOUCHPANEL_SW_HW_TRUE)	
        //TPDRV_LOG_TRACE:"[TPDRV:Done at %d] "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_354_112_2_17_23_7_23_1322,(uint8*)"d",SCI_GetTickCount());

        GPIO_SetDirection (TP_GPIO_XL, SCI_FALSE);
        GPIO_SetDirection (TP_GPIO_XR, SCI_FALSE);
        GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
        GPIO_SetDirection (TP_GPIO_YD, SCI_TRUE);
        GPIO_SetValue (TP_GPIO_YD, SCI_FALSE);

        {
            int i;
            for(i=0; i<4000; i++){;}
        }
        
        if(GPIO_GetValue (TP_GPIO_XL))
        {
            _Stop();
            ctl_tpc->int_clr.mBits.done_irq_clr = SCI_TRUE;
            msg_ptr.wparam = 0;
            msg_ptr.message = TB_TP_UP;
            ISR_Callback (TB_TPC_INT,  &msg_ptr);
            GPIO_SetDirection (TP_GPIO_YD, SCI_TRUE);
            GPIO_SetValue (TP_GPIO_YD, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_XL, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_XR, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
            CHIP_REG_SET(PIN_TP_XL_REG, PIN_I_EN | PIN_SPU_EN  | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_3 );
            CHIP_REG_SET(PIN_TP_XR_REG, PIN_Z_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3 );
            CHIP_REG_SET(PIN_TP_YU_REG, PIN_Z_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3);
            CHIP_REG_SET(PIN_TP_YD_REG, PIN_O_EN | PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_3 );
            s = TP_STATE_SAMPLING_X;

            return ;            
        }
        if(s == TP_STATE_SAMPLING_X)
        {
            /* stop TPC */


            ctl_tpc->ctrl.mBits.tpc_stop = SCI_TRUE;

            result = _FetchData (&tpc_data);
            if(SCI_TRUE == result)
            {
                s = TP_STATE_SAMPLING_Y;
                GPIO_SetDirection (TP_GPIO_XL, SCI_FALSE);
                GPIO_SetDirection (TP_GPIO_YD, SCI_TRUE);
                GPIO_SetValue (TP_GPIO_YD, SCI_FALSE);
                GPIO_SetDirection (TP_GPIO_XR, SCI_FALSE);
                GPIO_SetDirection (TP_GPIO_YU, SCI_TRUE);
                GPIO_SetValue (TP_GPIO_YU, SCI_TRUE);
            }
            else
            {
                GPIO_SetDirection (TP_GPIO_XL, SCI_TRUE);
                GPIO_SetDirection (TP_GPIO_XR, SCI_TRUE);
                GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
                GPIO_SetDirection (TP_GPIO_YD, SCI_FALSE);
                GPIO_SetValue (TP_GPIO_XL, SCI_TRUE);
                GPIO_SetValue (TP_GPIO_XR, SCI_FALSE);                     
            }

            {
                int i;
                for(i=0; i<500; i++){;}
            }
                    
            /* run TPC */


            ctl_tpc->ctrl.mBits.tpc_run = SCI_TRUE;

            return;
        } 

        /* stop TPC */


        ctl_tpc->ctrl.mBits.tpc_stop = SCI_TRUE;        
#endif
		  
        //Get data from TP buffer
        result = _FetchData (&tpc_data);

        if(SCI_TRUE == result )
        {
#if defined(TOUCHPANEL_SW_HW_TRUE)		
            s = TP_STATE_SAMPLING_X;

            GPIO_SetDirection (TP_GPIO_XL, SCI_TRUE);
            GPIO_SetDirection (TP_GPIO_XR, SCI_TRUE);
            GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_YD, SCI_FALSE);
            GPIO_SetValue (TP_GPIO_XL, SCI_TRUE);
            GPIO_SetValue (TP_GPIO_XR, SCI_FALSE);     
            
            /* run TPC */


            ctl_tpc->ctrl.mBits.tpc_run = SCI_TRUE;
#endif
			
            msg_ptr.wparam = tpc_data.dwValue;

            if (is_first_down == SCI_TRUE)
            {
                msg_ptr.message = TB_TP_DOWN;
                is_first_down = SCI_FALSE;
            }
            else
            {
                msg_ptr.message = TB_TP_MOVE;
            }      
        }
        else
        {
#if defined(TOUCHPANEL_SW_HW_TRUE)		
            GPIO_SetDirection (TP_GPIO_XL, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_YD, SCI_TRUE);
            GPIO_SetValue (TP_GPIO_YD, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_XR, SCI_FALSE);
            GPIO_SetDirection (TP_GPIO_YU, SCI_TRUE);
            GPIO_SetValue (TP_GPIO_YU, SCI_TRUE);        

            {
                int i;
                for(i=0; i<500; i++){;}
            }
            
            /* run TPC */


            ctl_tpc->ctrl.mBits.tpc_run = SCI_TRUE;
#endif
            return ;
        }
    }
    //Error occured
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     ("[TPDRV:TPDRV_IrqHandler] Undefined irq"));
    }

    //Send message to TP service layer
    ISR_Callback (TB_TPC_INT,  &msg_ptr);
    return ;
}


/******************************************************************************
 * _Run
 ******************************************************************************/
LOCAL BOOLEAN _Run (void)
{
    uint32 temp = 0, i = 0;    
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    //Disable APB sleep
    SCI_TPC_EnableDeepSleep (DISABLE_APB_SLEEP);

    while(1)/*lint !e716*/
    {
        for(i = 0; i<4; i++)
        {
            temp += ADC_GetResultDirectly (TPC_CHANNEL_Y, SCI_TRUE);    
            SCI_Sleep(1);
        }

        temp = temp >> 2;
            
        if (temp > s_tp_cfginfo_ptr->pressure_factor_limit && temp < 800)    
        {        
            //Enable APB sleep        
            //SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);        
            //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Run %d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_512_112_2_17_23_7_23_1323,(uint8*)"d", temp);
            //return SCI_FALSE;    
            SCI_Sleep(5);
        }
        else if(temp > 800)
        {
            SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);        
            //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Run %d, temp > 800]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_519_112_2_17_23_7_23_1324,(uint8*)"d", temp);
            return SCI_FALSE;    
        }
        else
        {
            break;
        }
        
        temp = 0;
        i = 0;
    }
    
    //Set ADC cs to TPC_CHANNEL_X
    ADC_SetCs (TPC_CHANNEL_X);

    //Set ADC scale
    ADC_SetScale (ADC_SCALE_3V);

    //Open ADC channel
    ADC_OpenTPC();

#if defined(TOUCHPANEL_SW_HW_TRUE)
        CHIP_REG_OR(PIN_TP_XL_REG, PIN_FUNC_3);
        CHIP_REG_OR(PIN_TP_XR_REG, PIN_FUNC_3);
        CHIP_REG_OR(PIN_TP_YU_REG, PIN_FUNC_3);
        CHIP_REG_OR(PIN_TP_YD_REG, PIN_FUNC_3);

        GPIO_SetDirection (TP_GPIO_XL, SCI_TRUE);
        GPIO_SetDirection (TP_GPIO_XR, SCI_TRUE);
        GPIO_SetDirection (TP_GPIO_YU, SCI_FALSE);
        GPIO_SetDirection (TP_GPIO_YD, SCI_FALSE);
        GPIO_SetValue (TP_GPIO_XL, SCI_TRUE);
        GPIO_SetValue (TP_GPIO_XR, SCI_FALSE);       
#endif

    //Enable TPC module
    ctl_tpc->ctrl.mBits.tpc_run = SCI_TRUE;

    return SCI_TRUE;
}

/******************************************************************************
 * _Stop
 ******************************************************************************/
LOCAL void _Stop (void)
{
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    //Enable APB sleep
    SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);

    //Disable TPC module
    ctl_tpc->ctrl.mBits.tpc_stop = SCI_TRUE;

    //Close ADC channel
    ADC_CloseTPC();

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Stop]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_575_112_2_17_23_7_24_1325,(uint8*)"");
}

/******************************************************************************
 * _FetchData
 ******************************************************************************/
LOCAL BOOLEAN _FetchData (TPC_DATA_U *data)
#if defined(TOUCHPANEL_SW_HW_TRUE)
{
    uint8 i;
    int delta_x, delta_y;
    BOOLEAN result = SCI_TRUE;
    TPC_DATA_U first_data, cur_data, pre_data;
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    pre_data.data.data_x = 0xFFFF;
    pre_data.data.data_y = 0xFFFF;
    
    //TPC data buffer is empty
    if (ctl_tpc->buf_ctrl.dwValue & TPC_BUF_EMPTY_FLAG)
    {
        //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData] buffer empty"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_595_112_2_17_23_7_24_1326,(uint8*)"");
        return SCI_FALSE;
    }

    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        if(s == TP_STATE_SAMPLING_X)

        //Fecth data from TPC data buffer
        {
            //Get data from buffer
            cur_data.dwValue = ctl_tpc->buf_data.dwValue;

            if (pre_data.data.data_x != 0xFFFF)
            {
                delta_x = pre_data.data.data_x - cur_data.data.data_x;
                delta_x = (delta_x > 0) ? delta_x : (- delta_x);

                if ( delta_x >= s_tp_cfginfo_ptr->delta_data_limit)
                {
                    TPDRV_LOG_TRACE (
                    ("[TPDRV:TPDRV_FetchData] delta_x = %d",
                     delta_x));
                    result  = SCI_FALSE;
                }      
            }
            else
            {
                first_data.data.data_x= cur_data.data.data_x;
            }
            pre_data.data.data_x = cur_data.data.data_x;
            //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData x %x] "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_626_112_2_17_23_7_24_1327,(uint8*)"d",cur_data.data.data_x);
        }
        else
        {
            cur_data.dwValue = ctl_tpc->buf_data.dwValue;

            if (pre_data.data.data_y != 0xFFFF)
            {
                delta_y = pre_data.data.data_y - cur_data.data.data_y;
                delta_y = (delta_y > 0) ? delta_y : (- delta_y);

                if ( delta_y >= s_tp_cfginfo_ptr->delta_data_limit)
                {
                    TPDRV_LOG_TRACE (
                    ("[TPDRV:TPDRV_FetchData] delta_y = %d",
                     delta_y));
                    result = SCI_FALSE;
                }
            }
            else
            {
                first_data.data.data_y = cur_data.data.data_y;
            }
            pre_data.data.data_y = cur_data.data.data_y;

            //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData y %x] "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_651_112_2_17_23_7_24_1328,(uint8*)"d",cur_data.data.data_y);
        }
    }

    if(s == TP_STATE_SAMPLING_X)
    {       

        data->data.data_x = cur_data.data.data_x;
    }
    else
    {

        data->data.data_y = cur_data.data.data_y;
    }
    return result;
}

#else
{
    uint8 i;
    int delta_x, delta_y;
    uint32 temp;
    BOOLEAN result = SCI_TRUE;
    TPC_DATA_U cur_data, pre_data;
    TPC_REG_T *ctl_tpc = (TPC_REG_T *) TPC_REG_BASE ;

    pre_data.dwValue = 0;

    //TPC data buffer is empty
    if (ctl_tpc->buf_ctrl.dwValue & TPC_BUF_EMPTY_FLAG)
    {
        //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData] buffer empty"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_682_112_2_17_23_7_24_1329,(uint8*)"");
        return SCI_FALSE;
    }
    /*
    *  To avoid flying point, @vine.yuan 2011.8.12
    */
    temp = ADC_GetResultDirectly (TPC_CHANNEL_Y, SCI_TRUE);
    ADC_SetScale (ADC_SCALE_3V);
    
    if (temp > s_tp_cfginfo_ptr->pressure_factor_limit)
    {
        //Fecth data from TPC data buffer
        for (i=0; i<TPC_BUF_LENGTH; i++)
        {
            cur_data.dwValue = ctl_tpc->buf_data.dwValue;
        }
        //TPDRV_LOG_TRACE:"[TPDRV:_FetchData]current pressure factor:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V0_698_112_2_17_23_7_24_1330,(uint8*)"d", temp);
        return SCI_FALSE;
    }

    //Fecth data from TPC data buffer
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        cur_data.dwValue = ctl_tpc->buf_data.dwValue;

        if (pre_data.dwValue != 0)
        {
            delta_x = pre_data.data.data_x - cur_data.data.data_x;
            delta_x = (delta_x > 0) ? delta_x : (- delta_x);
            delta_y = pre_data.data.data_y - cur_data.data.data_y;
            delta_y = (delta_y > 0) ? delta_y : (- delta_y);

            if ( (delta_x + delta_y) >= s_tp_cfginfo_ptr->delta_data_limit)
            {
                TPDRV_LOG_TRACE (
                    ("[TPDRV:TPDRV_FetchData] delta_x = %d delta_y = %d",
                     delta_x, delta_y));
                result  = SCI_FALSE;
            }
        }

        pre_data.dwValue = cur_data.dwValue;

        //TPDRV_LOG_TRACE ( ("[TPDRV:TPDRV_FetchData %x, %x] ",
        //cur_data.data.data_x, cur_data.data.data_y));
    }

    //Get the last data
    data->dwValue = cur_data.dwValue;

    return result;
}
#endif

