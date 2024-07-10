/******************************************************************************
 ** File Name:      adc_phy_v5.c                                                 *
 ** Author:         Mingwei.Zhang                                                    *
 ** DATE:           05/28/2010                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 ** 08/28/2011      lichd         Modify for sc6530                           *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "adc_reg_v5.h"
#include "adi_hal_internal.h"
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

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADC_REG_OR   ANA_REG_OR
#define ADC_REG_AND  ANA_REG_AND
#define ADC_REG_GET  ANA_REG_GET
#define ADC_REG_SET  ANA_REG_SET

/**---------------------------------------------------------------------------*
 **                         Local Define                                      *
 **---------------------------------------------------------------------------*/
// ADC device handle
LOCAL DEVICE_HANDLE s_dev_adc = SCI_NULL;

LOCAL void _adc_enable(uint32 is_en)
{
    if( is_en )
    {
        ADC_REG_OR(ADC_CTRL, ADC_CTRL_ADC_EN);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL, ~ADC_CTRL_ADC_EN);        
    }
}

LOCAL void _adc_int_enable(uint32 is_en)
{
    if( is_en )
    {
        ADC_REG_OR(ADC_CTRL, ADC_CTRL_ADC_EN);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL, ~ADC_CTRL_ADC_EN);        
    }
}

/*
    Set adc run , is cleared by HW.
*/
PUBLIC void ADC_Run( )
{
    ADC_REG_OR (ADC_CTRL, ADC_CTRL_SWCH_RUN);
} 

LOCAL void _adc_set_runnum( uint32 run_number )
{
    ADC_REG_AND (ADC_CTRL, ~ADC_CTRL_SWCH_RUN_NUM_MASK);
    ADC_REG_OR  (ADC_CTRL, run_number<<ADC_CTRL_SWCH_RUN_NUM_SHIFT);
}

/******************************************************************************
 *     ISR for adc in analog die
 ******************************************************************************/
PUBLIC void ADC_IsrHandler( uint32 data)
{
    uint32 adc_data=0;
    
	if( !(ADC_REG_GET(ADC_INT_RAW) & ADC_IRQ_MASK) )
		//SCI_TraceLow:"ADC_IsrHandler interrupt ERROR\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_92_112_2_17_23_2_29_11,(uint8*)"");
		
	//Get ADC result
    adc_data = ADC_REG_GET(ADC_DATA) & ADC_DATA_MSK;	
		
	// clear interrupt status
	ADC_REG_OR( ADC_INT_CLR, ADC_IRQ_MASK);;

    //SCI_TraceLow:"[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_100_112_2_17_23_2_29_12,(uint8*)"d", adc_data);
}

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
/******************************************************************************
 * ADC_Set12Bit
 ******************************************************************************/
PUBLIC void ADC_Set12Bit( BOOLEAN is_en )
{
    if(is_en)
    {
        ADC_REG_OR(ADC_CTRL,  BIT_2);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL,  ~BIT_2);
    }
}
#endif
/******************************************************************************
 * ADC_OpenTPC
 ******************************************************************************/
PUBLIC void ADC_OpenTPC (void)
{
    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYD); //
    
    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXR); // 

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYU); // 

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXL); //     

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    ADC_Set12Bit(SCI_FALSE);  //Default:10bit adc
#endif
}

/******************************************************************************
 * ADC_Init
 ******************************************************************************/
PUBLIC uint32 ADC_Init (void)
{
    /* Enable ADC device */	
    SCI_ASSERT (DEVICE_Find ("ADC", &s_dev_adc) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_adc);
       
    // Reset and enable ADC module
    DEVICE_Reset(s_dev_adc);
    
    _adc_enable(SCI_TRUE);
	
    // Set HW_CH_DELAY[7:0] for TPC sampling
    ADC_REG_SET (ADC_HW_CH_DELAY, 0xe0);
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), BIT_6);  //CHN_DLY_EN  
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), BIT_6);  //CHN_DLY_EN     
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), BIT_6);  //CHN_DLY_EN  
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), BIT_6);  //CHN_DLY_EN     
    
    ADC_OpenTPC();

    return SCI_SUCCESS;
}

/******************************************************************************
 * ADC_SetCs
 ******************************************************************************/
PUBLIC void ADC_SetCs (uint32 source)
{
    SCI_PASSERT ( (source < ADC_CH_MAX_NUM), ("error: source < ADC_CH_MAX_NUM"));/*assert verified*/
    ADC_REG_AND (ADC_SW_CH_CFG, ~ADC_SW_CH_ID_MASK);
    ADC_REG_OR  (ADC_SW_CH_CFG, source<<ADC_SW_CH_ID_SHIFT);

    //ADC_LOG_TRACE(("[ADC:ADC_SetCs]=cs%d", source));
}

/******************************************************************************
 * ADC_SetScale
 * 0--little scale,1.2V
 * 1--big scale,   3.0v 
 ******************************************************************************/
PUBLIC void ADC_SetScale (BOOLEAN scale)
{
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC small scalel
        ADC_REG_AND (ADC_SW_CH_CFG, ~ADC_SW_CH_BIG_SCALE_EN);
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC large scalel
        ADC_REG_OR (ADC_SW_CH_CFG, ADC_SW_CH_BIG_SCALE_EN);
    }
    
     //Slow mode
    ADC_REG_OR  (ADC_SW_CH_CFG, BIT_5);   

    //ADC_LOG_TRACE(("[ADC:ADC_SetScale]"));
}

PUBLIC void TPC_SetScale (BOOLEAN scale)
{
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC Little scalel
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~BIT_4);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~BIT_4); 
		
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~BIT_4);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~BIT_4); 
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC Big scalel
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), BIT_4);    
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), BIT_4);            

        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), BIT_4);    
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), BIT_4); 
    }

    //Enable slow mode
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), BIT_5);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), BIT_5);  
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), BIT_5);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), BIT_5);  
	
    //ADC_LOG_TRACE(("[ADC:TPC_SetScale]"));
}

/******************************************************************************
 * ADC_ConfigTPC
 ******************************************************************************/
PUBLIC void ADC_ConfigTPC (uint8 x, uint8 y)
{
    //ADC_LOG_TRACE:"[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_217_112_2_17_23_2_29_13,(uint8*)"");
}

/******************************************************************************
 * ADC_CloseTPC
 ******************************************************************************/
PUBLIC void ADC_CloseTPC (void)
{
    //ADC_LOG_TRACE:"[ADC:ADC_CloseTPC] is null for sc6530."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_224_112_2_17_23_2_29_14,(uint8*)"");
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
    ADC_REG_OR (ADC_INT_CLR, BIT_0);

    //Set ADC cs
    ADC_SetCs (source);

    //Set ADC scale
    ADC_SetScale (scale);

    //Run ADC soft channel
    ADC_Run();

    //Get current tick
    cur_tick = next_tick = SCI_GetTickCount();

    //Wait ADC run over
    while (! (ADC_REG_GET(ADC_INT_RAW) &ADC_IRQ_MASK))
    {
        //Timeout
        //SCI_PASSERT ((next_tick - cur_tick < 6),
        //             ("ADC CHN %d, timeout %dms", source, (next_tick - cur_tick)));
        if ( (next_tick - cur_tick) > 1)
        {
            ADC_Init();
            //ANA_REG_OR (ADC_CTRL, (adc_ctrl&ADC_TPC_CH_ON_BIT));
            SCI_RestoreIRQ();			
            //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly timeout...."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_262_112_2_17_23_2_29_15,(uint8*)"");
            return ADC_GetResultDirectly (source, scale);
        }

        next_tick = SCI_GetTickCount();
    }

    //Get ADC result
    result = ADC_REG_GET(ADC_DATA) & ADC_DATA_MSK;


    //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_273_112_2_17_23_2_29_16,(uint8*)"dd",source, result);
    
    // clear the intsts
    ADC_REG_OR (ADC_INT_CLR, ADC_DATA_MSK);

    //Enable irq
    SCI_RestoreIRQ();

    return result;
}
