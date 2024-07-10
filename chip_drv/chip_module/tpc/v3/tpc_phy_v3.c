/******************************************************************************
 ** File Name:        Tpc_phy_v3.c                                               *
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
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "task_monitor.h"
#include "tpc_reg_v3.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "tpc_drvapi.h"
#include "adc_internal.h"
#include "Tp_cfg.h"

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

#define TPC_BUF_LENGTH                  4


#define TPC_INT_DOWN            1
#define TPC_INT_UP              2
#define TPC_INT_DONE            4

INPUT_BUFFER_INIT (int32,64)


#define CLEAR_TPC_INT(msk) \
    do{ \
        uint32 cnt = 0;\
        SCI_DisableIRQ();\
        SCI_DisableFIQ();\
        ANA_REG_SET(TPC_INT_CLR, (msk));\
        while((ANA_REG_GET(TPC_INT_RAW)&(msk))&&(cnt < 10)){cnt++;}\
        if(cnt >= 10){TM_SendTestPointRequest(0x15170005,cnt);}\
        SCI_RestoreFIQ();\
        SCI_RestoreIRQ();\
    }while(0)


/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef union
{
    struct
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        uint16 data_x;
        uint16 data_y;
#else
        uint16 data_y;
        uint16 data_x;
#endif

    } data;
    uint32 dwValue;
} TPC_DATA_U;

/******************************************************************************
 * LOCAL fucntion declaration
 ******************************************************************************/
LOCAL TP_CFG_INFO_PTR s_tp_cfginfo_ptr;
LOCAL BOOLEAN is_first_down = SCI_TRUE;

LOCAL ISR_EXE_T _IrqHandler (void);
LOCAL void _Run (void);
LOCAL void _Stop (void);
LOCAL BOOLEAN _FetchData (TPC_DATA_U *data);
LOCAL void _TPCHisrFunc (uint32 i, void *d);
LOCAL void _HandleTPCHisr (int int_type);



/******************************************************************************
 * TPC_Init
 ******************************************************************************/
PUBLIC void TPDRV_Init (void)
{

    ANA_REG_OR (ANA_AGEN, AGEN_TPC_EN | AGEN_RTC_TPC_EN);
    s_tp_cfginfo_ptr = TP_GetCfgInfo();

    //Enable TPC module
    ANA_REG_OR (TPC_CTRL, TPC_EN_BIT);
    //Config pen request polarity
    ANA_REG_AND (TPC_CTRL, ~TPC_PEN_REQ_POL_BIT);

    //Config tpc mode
    ANA_REG_AND (TPC_CTRL, ~TPC_TPC_MODE_BIT);

    //default set: 12ms
    ANA_REG_MSK_OR (TPC_CTRL, ((s_tp_cfginfo_ptr->data_period)<< TPC_PRESCALE_OFFSET), TPC_PRESCALE_MSK);

    //Enable TPC interrupt
    //ANA_REG_OR(TPC_INT_EN, (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    //Config ADC channel
    ADC_SetScale (ADC_SCALE_3V);

    //Config TPC sample properity
	//Without pclk prescale, the data below is regard as 1ms period sending data to server.
    ANA_REG_SET (TPC_SAMPLE_CTRL0,
                 (0x14 << TPC_SAMPLE_INTERVAL_OFFSET) | 
				 (0x26 <<TPC_POINT_INTERVAL_OFFSET));
    ANA_REG_SET (TPC_SAMPLE_CTRL1,
                 (0x400 << TPC_DATA_INTERVAL_OFFSET) | 
				 (4 <<TPC_SAMPLE_NUM_OFFSET));

    //Config TPC filter properity
    //Config TPC debounce properity
    ANA_REG_SET (TPC_BOUNCE_CTRL,
                 TPC_DEBOUNCE_EN_BIT | (10 << TPC_DEBOUNCE_NUM_OFFSET));
    //Config TPC buffer length
    ANA_REG_MSK_OR (TPC_BUF_CTRL, (TPC_BUF_LENGTH << TPC_BUF_LENGTH_OFFSET), TPC_BUF_LENGTH_MSK);
    //Clear TPC interrupt
    CLEAR_TPC_INT ( (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    //Register TPC ISR handler
    ISR_RegHandler_Ex (TB_TPC_INT, (TB_ISR) _IrqHandler, _TPCHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    ANA_REG_OR (TPC_INT_EN, (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    if (ANA_REG_GET (ANA_ADIE_CHIP_ID) == ANA_V2_CHIP_ID)
    {
        CHIPDRV_EnableIRQINT (TB_TPC_INT);
    }

}

/******************************************************************************
 * TPDRV_Enable
 ******************************************************************************/
PUBLIC void TPDRV_Enable (void)
{
    ANA_REG_OR (TPC_INT_EN, (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    if (ANA_REG_GET (ANA_ADIE_CHIP_ID) == ANA_V2_CHIP_ID)
    {
        CHIPDRV_EnableIRQINT (TB_TPC_INT);
    }

}

/******************************************************************************
 * TPDRV_Enable
 ******************************************************************************/
PUBLIC void TPDRV_Disable (void)
{
    ANA_REG_AND (TPC_INT_EN, ~ (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    if (ANA_REG_GET (ANA_ADIE_CHIP_ID) == ANA_V2_CHIP_ID)
    {
        CHIPDRV_DisableIRQINT (TB_TPC_INT);
    }
}

/******************************************************************************
 * TPC_EnableCoord
 ******************************************************************************/
PUBLIC void TPDRV_EnableCoord (void)
{
    ANA_REG_OR (TPC_CALC_CTRL, TPC_CALC_EN_BIT);
    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_EnableCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_177_112_2_17_23_7_26_1332,(uint8*)"");
}

/******************************************************************************
 * TPC_DisableCoord
 ******************************************************************************/
PUBLIC void TPDRV_DisableCoord (void)
{
    ANA_REG_AND (TPC_CALC_CTRL, ~TPC_CALC_EN_BIT);

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_DisableCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_186_112_2_17_23_7_26_1333,(uint8*)"");
}

/******************************************************************************
 * TP_EnterCalibrate
 ******************************************************************************/
PUBLIC void TPDRV_EnterCalibration ( )
{
    //Disable coordinate convertion
    TPDRV_DisableCoord();
}

/******************************************************************************
 * TP_EnterMulti
 ******************************************************************************/
PUBLIC void TPDRV_EnterMulti ( )
{
    //Enable coordinate convertion
    TPDRV_EnableCoord();
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

    if(xy_exchange == SCI_FALSE)
    {
        //Config x coordinate coef
        ANA_REG_MSK_OR (TPC_CALC_X_COEF_A, x_slop, TPC_X_COEF_A_MSK);
        ANA_REG_MSK_OR (TPC_CALC_X_COEF_B, x_offset, TPC_X_COEF_B_MSK);
        //Config y coordinate coef
        ANA_REG_MSK_OR (TPC_CALC_Y_COEF_A, y_slop, TPC_Y_COEF_A_MSK);
        ANA_REG_MSK_OR (TPC_CALC_Y_COEF_B, y_offset, TPC_Y_COEF_B_MSK);
    }
	else
	{
    	// X/Y analog input switch.So x for y,y for x.
    	ANA_REG_OR (TPC_CTRL, TPC_XY_INPUT_SWITCH);
    	//Config x coordinate coef
        ANA_REG_MSK_OR (TPC_CALC_X_COEF_A, y_slop, TPC_X_COEF_A_MSK);
        ANA_REG_MSK_OR (TPC_CALC_X_COEF_B, y_offset, TPC_X_COEF_B_MSK);
        //Config y coordinate coef
        ANA_REG_MSK_OR (TPC_CALC_Y_COEF_A, x_slop, TPC_Y_COEF_A_MSK);
        ANA_REG_MSK_OR (TPC_CALC_Y_COEF_B, x_offset, TPC_Y_COEF_B_MSK);
	}
    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_ConfigCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_235_112_2_17_23_7_26_1334,(uint8*)"");
}

/******************************************************************************
 * TPC_IrqHandler
 ******************************************************************************/
LOCAL ISR_EXE_T _IrqHandler (void)
{
    uint16 int_status;

    int_status = ANA_REG_GET (TPC_INT_STS);

    //Pen Down interrtup
    if (int_status&TPC_DOWN_IRQ_MSK_BIT)
    {
        IsrWriteBuffer (TPC_INT_DOWN);
        //Clear down interrupt*/
        CLEAR_TPC_INT (TPC_DOWN_IRQ_MSK_BIT);
    }
    //Pen Up interrupt
    else if (int_status&TPC_UP_IRQ_MSK_BIT)
    {
        IsrWriteBuffer (TPC_INT_UP);
        //disable interrupt
        ANA_REG_AND (TPC_INT_EN, ~TPC_DONE_IRQ_MSK_BIT);
        //Clear up interrupt*/
        CLEAR_TPC_INT ( (TPC_UP_IRQ_MSK_BIT|TPC_DONE_IRQ_MSK_BIT));
    }
    //Sample Done interrupt
    else if (int_status&TPC_DONE_IRQ_MSK_BIT)
    {
        IsrWriteBuffer (TPC_INT_DONE);
        //Clear done interrupt*/
        CLEAR_TPC_INT (TPC_DONE_IRQ_MSK_BIT);
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
LOCAL void _HandleTPCHisr (int int_type)
{
    TPC_DATA_U tpc_data;
    TB_MSG msg_ptr;

    //Pen Down interrtup
    if (int_type == TPC_INT_DOWN)
    {
        is_first_down = SCI_TRUE;
        //Run TPC
        _Run();
        //enable done interrupt
        ANA_REG_OR (TPC_INT_EN, TPC_DONE_IRQ_MSK_BIT);
        return ;
    }
    //Pen Up interrupt
    else if (int_type == TPC_INT_UP)
    {
        //Stop TPC
        _Stop();
        CLEAR_TPC_INT (TPC_DONE_IRQ_MSK_BIT);
        //TP sever need deal with pen up coordinate
        msg_ptr.wparam = 0;
        msg_ptr.message = TB_TP_UP;
    }
    //Sample Done interrupt
    else if (int_type == TPC_INT_DONE)
    {
        //Get data from TP buffer
        if (_FetchData (&tpc_data))
        {
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
LOCAL void _Run (void)
{
    //Disable APB sleep
    SCI_TPC_EnableDeepSleep (DISABLE_APB_SLEEP);

    //Enable TPC module
    ANA_REG_OR (TPC_CTRL, TPC_RUN_BIT);

    //ANA_REG_GET(TPC_CTRL);    //dummy
    CLEAR_TPC_INT (TPC_DONE_IRQ_MSK_BIT);

    //Set ADC cs to TPC_CHANNEL_X
    ADC_SetCs (TPC_CHANNEL_X);

    //Set ADC scale
    ADC_SetScale (ADC_SCALE_3V);

    //Open ADC channel
    ADC_OpenTPC();

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Run ]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_376_112_2_17_23_7_26_1335,(uint8*)"");
}

/******************************************************************************
 * _Stop
 ******************************************************************************/
LOCAL void _Stop (void)
{
    //Enable APB sleep
    SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);

    //Disable TPC module
    ANA_REG_OR (TPC_CTRL, TPC_STOP_BIT);
    //Close ADC channel
    ADC_CloseTPC();

    //ANA_REG_GET(TPC_Y_DATA);  //fifo empty
    //ANA_REG_GET(TPC_Y_DATA);
    //ANA_REG_GET(TPC_Y_DATA);
    //ANA_REG_GET(TPC_Y_DATA);
    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Stop]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_395_112_2_17_23_7_26_1336,(uint8*)"");
}

/******************************************************************************
 * _FetchData
 ******************************************************************************/
LOCAL BOOLEAN _FetchData (TPC_DATA_U *data)
{
    uint8 i;
    int delta_x, delta_y;
    BOOLEAN result = SCI_TRUE;
    TPC_DATA_U cur_data, pre_data;
    uint16 buf_status = 0;

    buf_status = ANA_REG_GET (TPC_BUF_CTRL);

    //TPC data buffer is empty
    if (! (buf_status & TPC_BUF_FULL))
    {
        //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData] buffer not full 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_413_112_2_17_23_7_26_1337,(uint8*)"d",buf_status);
        return SCI_FALSE;
    }

    pre_data.dwValue = 0;

    //Fecth data from TPC data buffer
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        cur_data.data.data_x  = ANA_REG_GET (TPC_X_DATA);
        cur_data.data.data_y  = ANA_REG_GET (TPC_Y_DATA);

        if (pre_data.dwValue != 0)
        {
            delta_x = pre_data.data.data_x - cur_data.data.data_x;
            delta_x = (delta_x > 0) ? delta_x : (- delta_x);
            delta_y = pre_data.data.data_y - cur_data.data.data_y;
            delta_y = (delta_y > 0) ? delta_y : (- delta_y);

            if ( (delta_x + delta_y) >= s_tp_cfginfo_ptr->delta_data_limit)
            {
                //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_FetchData] delta_x = %d delta_y = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V3_435_112_2_17_23_7_26_1338,(uint8*)"dd", delta_x, delta_y);
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

