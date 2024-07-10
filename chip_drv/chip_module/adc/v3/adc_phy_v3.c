/******************************************************************************
 ** File Name:      adc_phy_v3.c                                                 *
 ** Author:         Mingwei.Zhang                                                    *
 ** DATE:           05/28/2010                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 ** 05/28/2010      Mingwei.Zhang       Create                     *
 ******************************************************************************/
//common head file
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adc_reg_v3.h"
//#include "../../analog/v3/analog_reg_v3.h"
//#include "adi_hal_internal.h"
#include "adc_drvapi.h"

#define _ADC_DEBUG
#ifdef _ADC_DEBUG
#include "sci_log.h"

#define ADC_ASSERT(_x_)                SCI_ASSERT(_x_)
#define ADC_VERIFY(_x_)                SCI_ASSERT(_x_)
#define ADC_ERR_TRACE(_x_)            SCI_TraceLow _x_
#define ADC_LOG_TRACE(_x_)            SCI_TraceLow _x_
#define ADC_INF_TRACE(_x_)            SCI_TraceLow _x_
#else
#define ADC_ASSERT(_x_)                ((void)0)
#define ADC_VERIFY(_x_)                ((void)(_x_))
#define ADC_ERR_TRACE(_x_)            ((void)0)
#define ADC_LOG_TRACE(_x_)            ((void)0)
#define ADC_INF_TRACE(_x_)            ((void)0)
#endif /* _ADC_DEBUG */

/******************************************************************************
 * ADC_Init
 ******************************************************************************/
PUBLIC uint32 ADC_Init (void)
{
    ANA_REG_OR (ANA_AGEN, AGEN_ADC_EN);
    ANA_REG_OR (ANA_CLK_CTL, ACLK_CTL_AUXAD_EN | ACLK_CTL_AUXADC_EN);

    ANA_REG_OR (ANA_MODULE_ARM_RST, BIT_4);///adc soft reset
    {
        ///wait reset done
        uint32 i;

        for (i = 0; i < 100; i++)
        {
        }
    }
    ANA_REG_AND (ANA_MODULE_ARM_RST, (~BIT_4));///adc soft reset

    ANA_REG_OR (ADC_CTRL, ADC_EN_BIT);

    //ADC_LOG_TRACE:"[ADC:ADC_Init]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V3_54_112_2_17_23_2_28_6,(uint8*)"");
    return SCI_SUCCESS;
}

/******************************************************************************
 * ADC_SetCs
 ******************************************************************************/
PUBLIC void ADC_SetCs (uint32 source)
{
    SCI_PASSERT ( (source < ADC_CH_MAX_NUM), ("error: source < ADC_CH_MAX_NUM"));/*assert verified*/
    ANA_REG_MSK_OR (ADC_CS, source, ADC_CS_BIT_MSK);
    //ADC_LOG_TRACE(("[ADC:ADC_SetCs]"));
}

/******************************************************************************
 * ADC_SetScale
 ******************************************************************************/
PUBLIC void ADC_SetScale (BOOLEAN scale)
{
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC small scalel
        ANA_REG_AND (ADC_CS, ~ADC_SCALE_BIT);
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC large scalel
        ANA_REG_OR (ADC_CS, ADC_SCALE_BIT);
    }

    //ADC_LOG_TRACE(("[ADC:ADC_SetScale]"));
}

/******************************************************************************
 * ADC_ConfigTPC
 ******************************************************************************/
PUBLIC void ADC_ConfigTPC (uint8 x, uint8 y)
{
    SCI_PASSERT ( (x < ADC_CH_MAX_NUM) && (y < ADC_CH_MAX_NUM),/*assert verified*/
                  ("error: (x < ADC_CH_MAX_NUM)&&(y < ADC_CH_MAX_NUM)"));

    ANA_REG_MSK_OR (ADC_TPC_CH_CTRL,
                    x| (y << ADC_TPC_Y_CH_OFFSET),
                    (ADC_TPC_X_CH_MSK|ADC_TPC_Y_CH_MSK));

    //ADC_LOG_TRACE:"[ADC:ADC_ConfigTPC]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V3_96_112_2_17_23_2_28_7,(uint8*)"");
}

/******************************************************************************
 * ADC_OpenTPC
 ******************************************************************************/
PUBLIC void ADC_OpenTPC (void)
{
    //Open TPC channel
    ANA_REG_OR (ADC_CTRL, ADC_TPC_CH_ON_BIT);

    //ADC_LOG_TRACE:"[ADC:ADC_OpenTPC]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V3_106_112_2_17_23_2_29_8,(uint8*)"");
}

/******************************************************************************
 * ADC_CloseTPC
 ******************************************************************************/
PUBLIC void ADC_CloseTPC (void)
{
    //Close TPC channel
    ANA_REG_AND (ADC_CTRL, ~ADC_TPC_CH_ON_BIT);

    //ADC_LOG_TRACE:"[ADC:ADC_CloseTPC]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V3_116_112_2_17_23_2_29_9,(uint8*)"");
}

/******************************************************************************
 * ADC_GetResultDirectly
 ******************************************************************************/
PUBLIC uint32 ADC_GetResultDirectly (ADC_E source, BOOLEAN scale)
{
    uint32 cur_tick, next_tick, result;

    //Disable irq
    SCI_DisableIRQ();

    //Clear int flag
    ANA_REG_OR (ADC_INT_CLR, ADC_IRQ_CLR_BIT);

    //Set ADC cs
    ADC_SetCs (source);

    //Set ADC scale
    ADC_SetScale (scale);

    //Run ADC soft channel
    ANA_REG_OR (ADC_CTRL, SW_CH_ON_BIT);

    //Get current tick
    cur_tick = next_tick = SCI_GetTickCount();

    //Wait ADC run over
    while (! (ANA_REG_GET (ADC_INT_SRC) &ADC_IRQ_RAW_BIT))
    {
        //Timeout
        //SCI_PASSERT ((next_tick - cur_tick < 6),
        //             ("ADC CHN %d, timeout %dms", source, (next_tick - cur_tick)));
        if ( (next_tick - cur_tick) > 1)
        {
            uint16 adc_ctrl = ANA_REG_GET (ADC_CTRL);
            ADC_Init();
            ANA_REG_OR (ADC_CTRL, (adc_ctrl&ADC_TPC_CH_ON_BIT));
            SCI_RestoreIRQ();
            //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly timeout...."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V3_155_112_2_17_23_2_29_10,(uint8*)"");
            return ADC_GetResultDirectly (source, scale);
        }

        next_tick = SCI_GetTickCount();
    }

    //Get ADC result
    result = ANA_REG_GET (ADC_DAT) & ADC_DATA_MSK;
    //Stop ADC soft channel
    ANA_REG_AND (ADC_CTRL, ~SW_CH_ON_BIT);

    ADC_SetScale (ADC_SCALE_3V);   ///for tpc scale
    //Cs must be set to ADIN_2 on idle for TPC
    ADC_SetCs (TPC_CHANNEL_X);

    //ADC_LOG_TRACE(("[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)",
    //source, result));
    ANA_REG_OR (ADC_INT_CLR, ADC_IRQ_CLR_BIT);

    //Enable irq
    SCI_RestoreIRQ();

    return result;
}
