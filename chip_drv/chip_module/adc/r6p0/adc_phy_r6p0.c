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
#include "adc_reg_r6p0.h"
#include "adi_hal_internal.h"
#include "adc_drvapi.h"
#include "chip_internal.h"
#include "efuse_phy.h"

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
#define ADC_REG_MASK_OR  ANA_REG_MSK_OR

#define ADC_USE_DEFAUT_EFUSE 0
#define false 0
#define true 1
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

PUBLIC void ADC_Set_Cal()
{
    ADC_REG_OR (ADC_CTRL, ADC_CTRL_OFFSET_CAL);
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

/******************************************************************************
 * ADC_Set12Bit
 ******************************************************************************/
PUBLIC void ADC_Set12Bit( BOOLEAN is_en )
{
    if(is_en)
    {
        ADC_REG_OR(ADC_CTRL,  ADC_CTRL_MODE_12B);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL,  ~ADC_CTRL_MODE_12B);
    }
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
/******************************************************************************
 * ADC_OpenTPC
 ******************************************************************************/
PUBLIC void ADC_OpenTPC (void)
{
    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYD | ADC_HW_CH_CAPACITY_MODE); //
    
    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXR | ADC_HW_CH_CAPACITY_MODE); // 

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYU | ADC_HW_CH_CAPACITY_MODE); // 

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXL | ADC_HW_CH_CAPACITY_MODE); //     

    ADC_Set12Bit(SCI_TRUE);  //Default:10bit adc
}
#endif

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
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN  
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN     
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN  
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN     
    
    ADC_OpenTPC();
#endif
    drvEfusePmicOpen();
#if (ADC_USE_DEFAUT_EFUSE == 0)
    if (true == prvAdcEffuseValid())
    {
        prvAdcScale00EffuseGet();      //adc1,0.1-1V,0.1 efuse(160-500),1v efuse(3000-3740),E-block18,F-block19,
        prvAdcScale01EffuseGet();      //batsense,3.6-4.2,3.6V efuse(2600-3320),4.2V efuse(3000-3720),A-block20,B-block21
        prvAdcScale02EffuseGet();      //0.3-3V,C-block22,D-block23
        prvAdcScale03EffuseGet();      //0.5-4V,J-block24,H-block25
        prvAdcScale03_bat_EffuseGet(); //bat_det 0.2-1.4V,0.2v efuse(300-700),1.4V efuse(3120-3920),G-block16,H block17,only use for battery
    }
#endif
    drvEfusePmicClose();
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

 ******************************************************************************/
PUBLIC void ADC_SetScale (ADC_SCALE_TYPE_E scale)
{
#if 0
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
#else
    if ( scale < ADC_SCALE_MAX )
    {
        ADC_REG_MASK_OR (ADC_SW_CH_CFG, (scale<<ADC_SW_CH_SCALE_SHIFT), ADC_SW_CH_SCALE_MASK);
    }
#endif
    
     //Slow mode
    ADC_REG_OR  (ADC_SW_CH_CFG, ADC_SW_CH_SLOW_MODE_EN);   

    //ADC_LOG_TRACE(("[ADC:ADC_SetScale]"));
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
PUBLIC void TPC_SetScale (BOOLEAN scale)
{
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC Little scalel
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN); 
        
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN); 
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC Big scalel
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);    
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);            

        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);    
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN); 
    }

    //Enable slow mode
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);  
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);  
    
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
#endif
/******************************************************************************
 * ADC_GetResultDirectly
 ******************************************************************************/
PUBLIC uint32 ADC_GetResultDirectly (ADC_E source, ADC_SCALE_TYPE_E scale)
{
    uint32 cur_tick, next_tick, result;
    uint32          ana_ctrl_reg32;
    uint32          rf_ana_26M_ctrl;
    uint32          status;
    uint32          start_tick;

#if defined(FPGA_VERIFICATION) 
    return 0;
#endif

    //Disable irq
    SCI_DisableIRQ();

    start_tick = SLEEP_GetCur32kNum();

ADC_CHANNEL_RUN:
    
    // prepare
    CHIP_ADC_Sample_Prepare_Enter();

    //Clear int flag
    ADC_REG_OR (ADC_INT_CLR, BIT_0);

    //Set ADC Offset Calibration
    ADC_Set_Cal();
	
if(source == ADIN_3)  //enable current mode
{
	ADC_REG_OR(0x100 + 0xB0, BIT_0);
	ADC_REG_OR(ANA_AUXADC_CTRL, BIT_4);
}

    //Set ADC cs
    ADC_SetCs (source);

    //Set ADC scale
    ADC_SetScale (scale);

    //Run ADC soft channel
    ADC_Run();

    //Get current tick
//    cur_tick = next_tick = SCI_GetTickCount();
    cur_tick = next_tick = SLEEP_GetCur32kNum();
    
    //Wait ADC run over
    while (! (ADC_REG_GET(ADC_INT_RAW) & ADC_IRQ_MASK))
    {
        //Timeout
        //SCI_PASSERT ((next_tick - cur_tick < 6),
        //             ("ADC CHN %d, timeout %dms", source, (next_tick - cur_tick)));

//        if (next_tick - cur_tick > 6)
        if ((next_tick - start_tick) > 64)   // 64/32.768 ~2ms
        {
            //PMIC_26M_get_status(&ana_ctrl_reg32, &rf_ana_26M_ctrl, &status);
            //SCI_PASSERT (0,("ADC CHN %d, ana_ctrl_reg32=%x, rf_ana_26M_ctrl=%x, status=%d",source, ana_ctrl_reg32, rf_ana_26M_ctrl, status));
        }
        
//        if ( (next_tick - cur_tick) > 1)
       if ( (next_tick - cur_tick) > 16)
        {
            ADC_Init();
            CHIP_ADC_Sample_Prepare_Exit();
            //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly timeout...."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_262_112_2_17_23_2_29_15,(uint8*)"");            
            goto ADC_CHANNEL_RUN;
            //return ADC_GetResultDirectly (source, scale);
        }

//        next_tick = SCI_GetTickCount();
        next_tick = SLEEP_GetCur32kNum();
    }

    //Get ADC result
    result = ADC_REG_GET(ADC_DATA) & ADC_DATA_MSK;

    //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_273_112_2_17_23_2_29_16,(uint8*)"dd",source, result);
    
    // clear the intsts
    ADC_REG_OR (ADC_INT_CLR, ADC_DATA_MSK);

    //prepare restore
    CHIP_ADC_Sample_Prepare_Exit();

    //Enable irq
    SCI_RestoreIRQ();

    return result;
}
#if 1 //for µ¥Èí¶àÓ²
/*for sc2731/sc2721/2720 if set 1.25uA, parameter isen is 125uA */
LOCAL uint32 _adc_isen_set(uint32 isen)
{
	uint32 isen_set = 0;
	switch (isen) {
	case 125:
		isen_set = 0x0;
		break;
	case 250:
		isen_set = 0x1;
		break;
	case 500:
		isen_set = 0x2;
		break;
	case 1000:
		isen_set = 0x3;
		break;
	case 2000:
		isen_set = 0x4;
		break;
	case 4000:
		isen_set = 0x5;
		break;
	case 8000:
		isen_set = 0x6;
		break;
	case 16000:
		isen_set = 0x7;
		break;
	default:
		isen_set = 0x0;
		break;
	}
	return isen_set;
}
#define BITS_BATDET_CUR_I(x)                    (((x) & 0x7) << 8)
LOCAL void udelay(unsigned int us)
{
    unsigned int start;
	unsigned int ticks;
	start = hwp_timer2->hwtimer_curval_l;
    ticks = us*2;  //timer freq = 2000000HZ  0.5us + 1
    while(!(((unsigned)((hwp_timer2->hwtimer_curval_l) - start)) > ticks))
    {
	}
}


LOCAL void _adc_set_isen(uint8 enable, uint32 isen)//xun zhao ti dai
{
	uint32 isen_bit;
	if (enable) {
		isen_bit = _adc_isen_set(isen);
		ADC_REG_AND(ANA_LED_CTRL, ~BIT_7);
		ADC_REG_OR(ANA_LED_CTRL,BIT_11);
		ADC_REG_AND(ANA_LED_CTRL,~(BITS_BATDET_CUR_I(0xF)));
		ADC_REG_OR(ANA_LED_CTRL,BITS_BATDET_CUR_I(isen_bit));
		udelay(150);
	} else {
		ADC_REG_AND(ANA_LED_CTRL,~(BITS_BATDET_CUR_I(0xF)));
		ADC_REG_AND(ANA_LED_CTRL,~BIT_11);
	}
}

LOCAL void _adc_set_current(uint8 enable, uint32 isen)
{
	if (enable) {
		_adc_set_isen(enable, isen);
		ADC_REG_OR(ANA_AUXADC_CTRL, BIT_4);

	} else {
		ADC_REG_AND(ANA_AUXADC_CTRL, ~BIT_4);
		_adc_set_isen(enable, isen);
	}
}
PUBLIC uint32 ADC_GetCalVol_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale)
{
	uint32 adc_value;

	/* get adc original data */
	adc_value = ADC_GetResultDirectly(channel_id, scale);

	//vol =(adc_value * 1250/*1000 * 1.25*/) / 4096;
	return adc_value;
}

PUBLIC uint32 ADC_GetVoltageByCurrent_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale,
	uint32 isen,
	uint32 cnt)
{
	uint32 i = 0, value= 0;
	cnt = (cnt<50)?cnt:50;
	for (i=0; i<cnt; i++) {
		_adc_set_current(1, isen);
		value += ADC_GetCalVol_Extend(channel_id, scale);
	}
	value = (uint32)(value/cnt);

	_adc_set_current(0, 0);
	return value;
}
#endif

