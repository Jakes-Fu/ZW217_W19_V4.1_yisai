/******************************************************************************
 ** File Name:      Adc_phy_v0.c                                                 *
 ** Author:         Yi.Qiu                                                    *
 ** DATE:           09/16/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 ** 06/10/2008      Tao.Feng      Create                                      *
 ** 09/16/2009      Yi.Qiu        Reconstruct ADC driver                      *
 ******************************************************************************/
//common head file
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adc_reg_v0.h"
#include "adc_drvapi.h"
#include "clock_drvapi.h"

#define _ADC_DEBUG
#ifdef _ADC_DEBUG
#include "sci_log.h"
#define ADC_ASSERT(_x_)             SCI_ASSERT(_x_)
#define ADC_VERIFY(_x_)             SCI_ASSERT(_x_)
#define ADC_ERR_TRACE(_x_)          SCI_TraceLow _x_
#define ADC_LOG_TRACE(_x_)          SCI_TraceLow _x_


#define ADC_INF_TRACE(_x_)          SCI_TraceLow _x_
#else
#define ADC_ASSERT(_x_)             ((void)0)
#define ADC_VERIFY(_x_)             ((void)(_x_))
#define ADC_ERR_TRACE(_x_)          ((void)0)
#define ADC_LOG_TRACE(_x_)          ((void)0)


#define ADC_INF_TRACE(_x_)          ((void)0)
#endif /* _TPC_DEBUG */

LOCAL DEVICE_HANDLE s_dev_adc = SCI_NULL;

/******************************************************************************
 * ADC_Init
 ******************************************************************************/
PUBLIC uint32 ADC_Init (void)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    SCI_ASSERT (DEVICE_Find ("ADC", &s_dev_adc) == CLOCK_SUCCESS); /*assert verified*/

    //Enable ADC module
    ctl_adc->ctrl.mBits.adc_en = SCI_TRUE;

    //ADC_LOG_TRACE:"[ADC:ADC_Init]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_48_112_2_17_23_2_27_0,(uint8*)"");
    return SCI_SUCCESS;
}

/******************************************************************************
 * ADC_SetScale
 ******************************************************************************/
PUBLIC void ADC_SetCs (uint32 source)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    ctl_adc->cs.mBits.adc_cs = source;

    //ADC_LOG_TRACE(("[ADC:ADC_SetChannelCs]"));
}

/******************************************************************************
 * ADC_SetScale
 ******************************************************************************/
PUBLIC void ADC_SetScale (BOOLEAN scale)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    if (scale == ADC_SCALE_1V2)
    {
        //small measure scale
        ctl_adc->cs.mBits.adc_scale = SCI_FALSE;
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC large measure scalel
        ctl_adc->cs.mBits.adc_scale = SCI_TRUE;
    }

    //ADC_LOG_TRACE(("[ADC:ADC_SetMeasureScale]"));
}

/******************************************************************************
 * ADC_ConfigTPC
 ******************************************************************************/
PUBLIC void ADC_ConfigTPC (uint8 x, uint8 y)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    //Config TPC channel
    ctl_adc->ch_ctrl.mBits.tpc_x_ch = x;
    ctl_adc->ch_ctrl.mBits.tpc_y_ch = y;

    //ADC_LOG_TRACE:"[ADC:ADC_ConfigTpcChannel]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_93_112_2_17_23_2_27_1,(uint8*)"");
}

/******************************************************************************
 * ADC_OpenTPC
 ******************************************************************************/
PUBLIC void ADC_OpenTPC (void)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    //Open TPC channel
    ctl_adc->ctrl.mBits.tpc_ch_on = SCI_TRUE;

    //ADC_LOG_TRACE:"[ADC:ADC_OpenTpcChannel]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_105_112_2_17_23_2_27_2,(uint8*)"");
}

/******************************************************************************
 * ADC_CloseTPC
 ******************************************************************************/
PUBLIC void ADC_CloseTPC (void)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;

    //Close TPC channel
    ctl_adc->ctrl.mBits.tpc_ch_on = SCI_FALSE;

    //ADC_LOG_TRACE:"[ADC:ADC_CloseTpcChannel]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_117_112_2_17_23_2_27_3,(uint8*)"");
}

/******************************************************************************
 * ADC_GetResultDirectly
 ******************************************************************************/
PUBLIC uint32 ADC_GetResultDirectly (ADC_E source, BOOLEAN scale)
{
    ADC_REG_T *ctl_adc = (ADC_REG_T *) ADC_REG_BASE ;
    uint32 i, cur_tick, next_tick, result;

    //Disable irq
    SCI_DisableIRQ();

    //Get current tick
    cur_tick = next_tick = SCI_GetTickCount();

    //Clear int flag
    ctl_adc->int_clr.mBits.adc_irq_clr = SCI_TRUE;

    //Set ADC cs
    ctl_adc->cs.mBits.adc_cs = source;

    //Set ADC scale
    ctl_adc->cs.mBits.adc_scale = ~scale;

    //Run ADC soft channel
    ctl_adc->ctrl.mBits.sw_ch_on = SCI_TRUE;

    //Wait ADC run over
    while (ctl_adc->ctrl.mBits.adc_status != SCI_TRUE)
    {
        //Timeout
        if (next_tick - cur_tick >= 5)
        {
            //SCI_TRACE_LOW:"0x%x 0x%x 0x%x 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_152_112_2_17_23_2_27_4,(uint8*)"dddd",ctl_adc->ctrl.dwValue, ctl_adc->cs.dwValue, next_tick, cur_tick);
            //SCI_TRACE_LOW:"--%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V0_153_112_2_17_23_2_27_5,(uint8*)"d", * (volatile uint32 *) 0x87003000);

            /* Reset ADC controller */
            DEVICE_Reset (s_dev_adc);

            //Enable ADC module
            ctl_adc->ctrl.mBits.adc_en = SCI_TRUE;

            return ADC_GetResultDirectly (source, scale);
        }

        next_tick = SCI_GetTickCount();
    }

    //Get ADC result
    result = ctl_adc->data.mBits.adc_data;

    //Stop ADC soft channel
    ctl_adc->ctrl.mBits.sw_ch_on = SCI_FALSE;

    for (i=0; i<30; i++) {};

    //Cs must be set to TPC_CHANNEL_X on idle for TPC running
    ctl_adc->cs.mBits.adc_cs = TPC_CHANNEL_X;

    //Clear int flag
    ctl_adc->int_clr.mBits.adc_irq_clr = SCI_TRUE;

    //ADC_LOG_TRACE(("[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)",
    //source, result));

    //Enable irq
    SCI_RestoreIRQ();

    return result;
}

