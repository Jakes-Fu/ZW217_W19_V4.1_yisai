/******************************************************************************
 ** File Name:      sensor_drv.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           04/19/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of sensor*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2006     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#define DEBUG_SENSOR_DRV

#include "ms_ref_dc_trc.h"
#ifdef DEBUG_SENSOR_DRV
#define SENSOR_PRINT   SCI_TRACE_LOW
#else
#define SENSOR_PRINT( _format_string )
#endif


#include "sensor_drv.h"
#include "sensor_cfg.h"
#include "sc_reg.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "ref_outport.h"
#include "ldo_drv.h"
#include "gpio_prod_api.h"
#include "clock_drvapi.h"
#include "power_cfg.h"
#include "ap_apb.h"
#include "ap_clk.h"

#include "../../image_proc/src/isp/inc/uws6121e_reg_isp.h"
/*lint -save -e765 */
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define     SENSOR_ONE_I2C                     1
#define     SENSOR_ZERO_I2C                    0
#define     SENSOR_16_BITS_I2C               2
#define     SENSOR_I2C_FREQ                   (400*1000) //(400*1000)//(100*1000)
#define     SENSOR_I2C_PORT_0                 0
#define     SENSOR_I2C_ACK_TRUE             1
#define     SENSOR_I2C_ACK_FALSE            0
#define     SENSOR_I2C_STOP                    1
#define     SENSOR_I2C_NOSTOP                0
#define     SENSOR_I2C_NULL_HANDLE     -1
#define     SENSOR_ADDR_BITS_8              1
#define     SENSOR_ADDR_BITS_16             2
#define     SENSOR_CMD_BITS_8                1
#define     SENSOR_CMD_BITS_16              2
#define     SENSOR_LOW_SEVEN_BIT          0x7f
#define     SENSOR_LOW_EIGHT_BIT          0xff
#define     SENSOR_HIGN_SIXTEEN_BIT      0xffff0000
#define     SENSOR_LOW_SIXTEEN_BIT       0xffff
#define     SENSOR_I2C_OP_TRY_NUM        4
#define     SENSOR_RECOVER_TIME            10
#define     NUMBER_MAX                           0xFFFFFFFF
#define     SENSOR_ABS(a)                                      ((a) > 0 ? (a) : -(a))

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#ifndef CHIP_VER_6531EFM
#define     SENSOR_MCLK_DIV_MAX           4
#define     SENSOR_MCLK_SRC_NUM           4
#define SENSOR_MAX_MCLK                         89      // MHZ
const SN_MCLK sensor_mclk_tab[SENSOR_MCLK_SRC_NUM] =
{
    {26, 0},
    {78, 1},
    {89, 2},
    {48, 3}
};
#else
#define     SENSOR_MCLK_DIV_MAX           8
#define     SENSOR_MCLK_SRC_NUM           5
#define SENSOR_MAX_MCLK                         83      // MHZ
const SN_MCLK sensor_mclk_tab[SENSOR_MCLK_SRC_NUM] =
{
    {26, 0},
    {26, 1},
    {48, 2},
    {64, 3},
    {83, 4}
};
#endif
/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL SENSOR_INFO_T* s_sensor_list_ptr[SENSOR_ID_MAX]={0x00};
LOCAL SENSOR_INFO_T* s_sensor_info_ptr=PNULL;
LOCAL SENSOR_EXP_INFO_T s_sensor_exp_info={0x00};
LOCAL uint32 s_sensor_mclk=0;
LOCAL uint8 s_sensor_probe_index=0;
LOCAL BOOLEAN s_sensor_identified = SCI_FALSE;
LOCAL BOOLEAN s_sensor_init=SCI_FALSE;
LOCAL BOOLEAN s_sensor_open=SCI_FALSE;
LOCAL BOOLEAN s_atv_init=SCI_FALSE;
LOCAL BOOLEAN s_atv_open=SCI_FALSE;
LOCAL SENSOR_TYPE_E s_sensor_type=SENSOR_TYPE_IMG_SENSOR;
LOCAL SENSOR_MODE_E s_sensor_mode[SENSOR_ID_MAX]={SENSOR_MODE_MAX,SENSOR_MODE_MAX,SENSOR_MODE_MAX};
LOCAL uint8    s_sensor_index[SENSOR_ID_MAX] = {0xFF, 0xFF, 0xFF};
LOCAL SCI_MUTEX_PTR s_imgsensor_mutex_ptr=PNULL;
LOCAL SENSOR_REGISTER_INFO_T s_sensor_register_info={0x00};
LOCAL SENSOR_REGISTER_INFO_T_PTR s_sensor_register_info_ptr=&s_sensor_register_info;
LOCAL int s_i2c_dev_handler=SENSOR_I2C_NULL_HANDLE;
LOCAL DEVICE_HANDLE s_dev_ccir = SCI_NULL;

LOCAL BOOLEAN _Sensor_PowerUpHandle(SENSOR_ID_E sensor_id);
LOCAL BOOLEAN _select_sensor_mclk(uint8 clk_set, uint8* clk_src,uint8* clk_div);
LOCAL SENSOR_OPEN_FUNC_PTR g_OpenCamController = NULL;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: Create Mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          Mutex
/*****************************************************************************/
PUBLIC void ImgSensor_CreateMutex(void)
{

    s_imgsensor_mutex_ptr = SCI_CreateMutex("IMG SENSOR SYNC MUTEX", SCI_INHERIT);
    SCI_PASSERT((s_imgsensor_mutex_ptr!=PNULL),("IMG SENSOR Great MUTEX fail!"));/*assert verified*/

}

/*****************************************************************************/
//  Description: Delete Mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  Mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_DeleteMutex(void)
{
    uint32 ret;

    if(NULL==s_imgsensor_mutex_ptr)
    {
        return ;
    }

    ret=SCI_DeleteMutex(s_imgsensor_mutex_ptr);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
    s_imgsensor_mutex_ptr=NULL;

}

/*****************************************************************************/
//  Description: Get Mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_GetMutex(void)
{
    uint32 ret;
    if (!SCI_InThreadContext())
    {
        return;
    }


    if(PNULL==s_imgsensor_mutex_ptr)
    {
        ImgSensor_CreateMutex();
    }

    ret = SCI_GetMutex(s_imgsensor_mutex_ptr, SCI_WAIT_FOREVER);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}
/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_PutMutex(void)
{
    uint32 ret;

    if (!SCI_InThreadContext())
    {
        return;
    }


    if(NULL==s_imgsensor_mutex_ptr)
    {
        return;
    }

    ret = SCI_PutMutex(s_imgsensor_mutex_ptr);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}

/*****************************************************************************/
//  Description:    This function is used to get sensor type
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void _Sensor_IicHandlerRelease(void)
{
    SCI_TRACE_LOW("Sensor IicRelease");
    if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
    {
        //SENSOR_PRINT:"SENSOR: I2C handle abnormal"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_229_112_2_18_1_24_32_575,(uint8*)"");
        return;
    }

    if(SENSOR_ZERO_I2C!=I2C_HAL_Close(s_i2c_dev_handler))
    {
        //SENSOR_PRINT:"SENSOR: I2C_no_close"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_235_112_2_18_1_24_32_576,(uint8*)"");
    }
    else
    {
        s_i2c_dev_handler=SENSOR_I2C_NULL_HANDLE;
        //SENSOR_PRINT:"SENSOR: I2C_close s_i2c_dev_handler=%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_240_112_2_18_1_24_32_577,(uint8*)"d",s_i2c_dev_handler);
    }
}

/*****************************************************************************/
//  Description:    This function is used to get sensor type
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC int32 _Sensor_IicHandlerInit(SENSOR_ID_E sensor_id)
{
    int32 dev_handler=0;
    I2C_DEV  dev={0x00};

    SCI_TRACE_LOW("Sensor:IicInit id = %d", sensor_id);

    if(!(SENSOR_I2C_NULL_HANDLE==s_i2c_dev_handler))
    {
        //SCI_TRACE_LOW("Sensor:IicRelease");
        _Sensor_IicHandlerRelease();
    }

    dev.id = (uint32)Sensor_GetDefaultI2cPort(sensor_id);

    if(SENSOR_I2C_FREQ_20==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_FREQ_MASK))
    {
        dev.freq=(20*1000);
    }
    else if(SENSOR_I2C_FREQ_50==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_FREQ_MASK))
    {
        dev.freq=(50*1000);
    }
    else if(SENSOR_I2C_FREQ_200==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_FREQ_MASK))
    {
        dev.freq=(400*1000);
    }
    else if(SENSOR_I2C_FREQ_400==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_FREQ_MASK))
    {
        dev.freq=(400*1000);
    }
    else
    {
        dev.freq=SENSOR_I2C_FREQ;
    }

    dev.slave_addr=s_sensor_info_ptr->salve_i2c_addr_w;

    if(SENSOR_I2C_CUSTOM==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_CUSTOM))
    {
        dev.reg_addr_num=SENSOR_ZERO_I2C;
    }
    else if(SENSOR_I2C_REG_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_REG_16BIT))
    {
        dev.reg_addr_num=SENSOR_ADDR_BITS_16;
    }
    else
    {
        dev.reg_addr_num=SENSOR_ADDR_BITS_8;
    }

    if(SENSOR_I2C_NOACK_BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_NOACK_BIT))
    {
        dev.check_ack=SENSOR_I2C_ACK_FALSE;
    }
    else
    {
        dev.check_ack=SENSOR_I2C_ACK_TRUE;
    }

    if(SENSOR_I2C_STOP_BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_STOP_BIT))
    {
        dev.no_stop=SENSOR_I2C_STOP;
    }
    else
    {
        dev.no_stop=SENSOR_I2C_NOSTOP;
    }

    //SENSOR_PRINT:"_Sensor_IicHandlerInit, id %d freq %d bus %d slave_addr 0x%x reg_addr_num %d,ack %d no_stop %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_314_112_2_18_1_24_32_578,(uint8*)"ddddddd",dev.id,dev.freq,dev.bus,dev.slave_addr,dev.reg_addr_num,dev.check_ack,dev.no_stop);

    dev_handler=I2C_HAL_Open(&dev);

    if(dev_handler==SENSOR_I2C_NULL_HANDLE)
    {
        //SENSOR_PRINT:"SENSOR_handler creat err first"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_320_112_2_18_1_24_32_579,(uint8*)"");
    }

    s_i2c_dev_handler=dev_handler;

    return dev_handler;
}

/*****************************************************************************/
//  Description:    This function is used to get sensor type
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC SENSOR_TYPE_E _Sensor_GetSensorType(void)
{
    return s_sensor_type;
}

/*****************************************************************************/
//  Description:    This function is used to reset sensor
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void Sensor_Reset(void)
{
    BOOLEAN                 reset_pulse_level;
    uint32                  reset_pulse_width;
    SENSOR_IOCTL_FUNC_PTR   reset_func;

    reset_pulse_level = (BOOLEAN)s_sensor_info_ptr->reset_pulse_level;
    reset_pulse_width = s_sensor_info_ptr->reset_pulse_width;
    reset_func        = s_sensor_info_ptr->ioctl_func_tab_ptr->reset;

    //SENSOR_PRINT:"SENSOR: Sensor_Reset -> reset_pulse_level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_349_112_2_18_1_24_32_580,(uint8*)"d", reset_pulse_level);
    // HW Reset sensor
    if(PNULL != reset_func)
    {
        reset_func(0);
    }
    else
    {
        if(NULL == reset_pulse_width)
        {
            reset_pulse_width = SENSOR_RESET_PULSE_WIDTH_DEFAULT;
        }
        else if(SENSOR_RESET_PULSE_WIDTH_MAX < reset_pulse_width)
        {
            reset_pulse_width = SENSOR_RESET_PULSE_WIDTH_MAX;
        }

        GPIO_ResetSensor(reset_pulse_level, reset_pulse_width);
    }

}

LOCAL BOOLEAN _select_sensor_mclk(uint8 clk_set, uint8* clk_src,uint8* clk_div)
{
    //return TRUE;


    uint8   i,j,mark_src = 0,mark_div = 0,mark_src_tmp = 0;
    uint32  clk_tmp,src_delta,src_delta_min = NUMBER_MAX;
    uint32  div_delta_min = NUMBER_MAX;             //div_delta

    if(clk_set > SENSOR_MAX_MCLK || !clk_src || !clk_div)
    {
        //SENSOR_PRINT:"SENSOR_DRV: set mclk parameter error, the MCLK is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_380_112_2_18_1_24_32_581,(uint8*)"d", clk_set);
        return FALSE;
    }

    for(i = 0; i < SENSOR_MCLK_DIV_MAX; i++ )
    {
        clk_tmp = (uint32)(clk_set * (i + 1));
        src_delta_min = NUMBER_MAX;
        for(j = 0; j < SENSOR_MCLK_SRC_NUM; j++ )
        {
            src_delta = SENSOR_ABS((int32)(sensor_mclk_tab[j].clock) - (int32)clk_tmp);
            if(src_delta < src_delta_min)
            {
                src_delta_min = src_delta;
                mark_src_tmp = j;
            }
        }

        if(src_delta_min <  div_delta_min)
        {
            div_delta_min = src_delta_min;
            mark_src = mark_src_tmp;
            mark_div = i;
        }
    }

    *clk_src = mark_src;
    *clk_div = mark_div;
    return TRUE;
}

void Sensor_SetMCLK_6121e()
{
    hwp_apClk->cgm_ap_a5_sel_cfg = 0x5;         //clock src APLL 500m
    hwp_apClk->cgm_ap_bus_div_cfg = 0x1;        //ap bus clock 250m

    hwp_apClk->cgm_camera_ref_sel_cfg = 0x5;   //clock src APLL 500m
    hwp_apClk->cgm_camera_ref_div_cfg = 0x14;       //camera mclk 24M
    hwp_apApb->misc_cfg |= (1<<9);   //clk camera out enable
#if 0//bringup_tmp
    uint32 ctrlReg = 0;
    uint32 newClkScale = 0;

    hwp_sysCtrl->REG_DBG = 0x00A50001; // unlock
    hwp_sysCtrl->CLK_AP_APB_ENABLE  |= SYS_CTRL_ENABLE_AP_APB_CLK_ID_MOD_CAMERA;
    hwp_sysCtrl->CLK_AP_APB_ENABLE  |= SYS_CTRL_ENABLE_AP_APB_CLK_ID_CAMERA;
    hwp_sysCtrl->CLK_OTHERS_ENABLE  |= SYS_CTRL_ENABLE_OC_ID_PIX;
    hwp_sysCtrl->CLK_OTHERS_ENABLE  |= SYS_CTRL_ENABLE_OC_ID_ISP;
    hwp_sysCtrl->CLK_OTHERS_ENABLE  |= (SYS_CTRL_ENABLE_OC_ID_SPICAM |  \
                                        SYS_CTRL_ENABLE_OC_ID_CAM);

    //select PLL fast clock
    hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SYS_SEL_FAST_SLOW;
    OS_TickDelay(1);

    hwp_sysCtrl->Cfg_Clk_Camera_Out |= SYS_CTRL_CLK_CAMERA_OUT_EN_ENABLE;
    ctrlReg = hwp_sysCtrl->Cfg_Clk_Camera_Out & SYS_CTRL_CLK_CAMERA_OUT_EN_ENABLE;

    //set dividers 24Mhz 756/32 = 24M
    newClkScale = 42;
    ctrlReg |=  /*SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_156_MHZ*/ (1<<1) |
        SYS_CTRL_CLK_CAMERA_OUT_DIV(newClkScale);
    OS_TickDelay(1);
    hwp_sysCtrl->Cfg_Clk_Camera_Out = ctrlReg;
    hwp_sysCtrl->Cfg_Clk_Camera_Out |= SYS_CTRL_CLK_CAMERA_DIV_UPDATE;
#endif
}

void Sensor_disableMCLK_6121e()
{
    hwp_apApb->misc_cfg &=(~(1<9));   //clk camera out disable
#if 0//bringup_tmp
    hwp_sysCtrl->REG_DBG = 0x00A50001;
    //hwp_sysCtrl->CLK_OTHERS_DISABLE = SYS_CTRL_DISABLE_OC_ID_CAM;

    hwp_sysCtrl->CLK_AP_APB_DISABLE   = SYS_CTRL_DISABLE_AP_APB_CLK_ID_MOD_CAMERA;
    hwp_sysCtrl->CLK_OTHERS_DISABLE   = SYS_CTRL_DISABLE_OC_ID_PIX;
    hwp_sysCtrl->CLK_OTHERS_DISABLE   = SYS_CTRL_DISABLE_OC_ID_ISP;
    hwp_sysCtrl->CLK_OTHERS_DISABLE   = (SYS_CTRL_DISABLE_OC_ID_SPICAM |  \
                                         SYS_CTRL_DISABLE_OC_ID_CAM);

    hwp_sysCtrl->Cfg_Clk_Camera_Out = SYS_CTRL_CLK_CAMERA_OUT_EN_DISABLE;
#endif
}


/*****************************************************************************/
//  Description:    This function is used to power on sensor and select xclk
//  Author:         Liangwen.Zhen
//  Note:           1.Unit: MHz 2. if mclk equal 0, close main clock to sensor
/*****************************************************************************/
/*lint -e{538,529}*/
PUBLIC void Sensor_SetMCLK(uint32 mclk)
{

    uint32 divd = 0;

    //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK -> s_sensor_mclk = %dMHz, clk = %dMHz"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_419_112_2_18_1_24_32_582,(uint8*)"dd", s_sensor_mclk, mclk);

    return ;
#if 0
#if 0
    if((0 != mclk)&&(s_sensor_mclk != mclk))
    {
        if(mclk > SENSOR_MAX_MCLK)
        {
            mclk = SENSOR_MAX_MCLK;
        }

        divd=SENSOR_MAX_MCLK/mclk-1;
        DEVICE_SetClockDiv(s_dev_ccir, divd);
        DEVICE_Open(s_dev_ccir);

        s_sensor_mclk = mclk;
        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK divd = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_434_112_2_18_1_24_32_583,(uint8*)"d", divd);
    }
    else if(0 == mclk)
    {//close the m_clk
        DEVICE_Close(s_dev_ccir);
        s_sensor_mclk = 0;
        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK -> Disable MCLK !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_440_112_2_18_1_24_32_584,(uint8*)"");
    }
    else
    {
        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK -> Do nothing !! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_444_112_2_18_1_24_32_585,(uint8*)"");
    }
#else

    if((0 != mclk)&&(s_sensor_mclk != mclk))
    {
        uint32 clk_src = 0;
        uint32 clk_div = 0;

        if(mclk > SENSOR_MAX_MCLK)
        {
            mclk = SENSOR_MAX_MCLK;
        }

        _select_sensor_mclk((uint8)mclk, (uint8 *)&clk_src, (uint8 *)&clk_div);

        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK src = 0x%x,  divd = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_461_112_2_18_1_24_32_586,(uint8*)"dd", clk_src, clk_div);

        Sensor_MclkConfig(clk_src,clk_div);
#ifdef CHIP_DSP_6500
        /*Only for SC6500, Open the SPI Sensor MCLOK*/
        *(volatile uint32*)APB_EB0_SET = BIT_17;
#endif
        s_sensor_mclk = mclk;
    }
    else if(0 == mclk)
    {//close the m_clk
#ifdef CHIP_DSP_6500
        /*Only for SC6500, Close the SPI Sensor MCLOK*/
        *(volatile uint32*)APB_EB0_CLR = BIT_17;
#endif
        *(volatile uint32*) APB_CLK_CFG0 &= ~BIT_12;

        s_sensor_mclk = 0;
        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK -> Disable MCLK !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_481_112_2_18_1_24_32_587,(uint8*)"");
    }
    else
    {
        //SENSOR_PRINT:"SENSOR: Sensor_SetMCLK -> Do nothing !! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_485_112_2_18_1_24_32_588,(uint8*)"");
    }
#endif
#endif
}

/******************************************************************************
// Purpose: convert SENSOR_AVDD_VAL_E type to POWER_SENSOR_VDD_VAL_E type
// Author:  Eddy.Wei
// Input:   SENSOR_AVDD_VAL_E type
// Output:  None
// Return:  POWER_SENSOR_VDD_VAL_E type
// Note:
******************************************************************************/
LOCAL POWER_SENSOR_VDD_VAL_E Sensor_VoltageConvertor(SENSOR_AVDD_VAL_E format)
{
    POWER_SENSOR_VDD_VAL_E  voltage_level=POWER_VDD_CLOSED;

    switch(format)
    {
        case SENSOR_AVDD_3300MV:
            voltage_level=POWER_VDD_3300MV;
        break;

        case SENSOR_AVDD_3000MV:
            voltage_level=POWER_VDD_3000MV;
        break;

        case SENSOR_AVDD_2800MV:
            voltage_level=POWER_VDD_2800MV;
        break;

        case SENSOR_AVDD_2500MV:
            voltage_level=POWER_VDD_2500MV;
        break;

        case SENSOR_AVDD_1800MV:
            voltage_level=POWER_VDD_1800MV;
        break;

        case SENSOR_AVDD_1500MV:
            voltage_level=POWER_VDD_1500MV;
        break;

        case SENSOR_AVDD_1300MV:
            voltage_level=POWER_VDD_1300MV;
        break;

        case SENSOR_AVDD_1200MV:
            voltage_level=POWER_VDD_1200MV;
        break;

        case SENSOR_AVDD_CLOSED:
            voltage_level=POWER_VDD_CLOSED;
        break;

        case SENSOR_AVDD_UNUSED:
            voltage_level=POWER_VDD_UNUSED;
        break;


        default:
            voltage_level=POWER_VDD_CLOSED;
        break;

    }
    return voltage_level;
}

/******************************************************************************
// Purpose: convert SENSOR_ID_E type to POWER_SENSOR_ID_E type
// Author:  Eddy.Wei
// Input:   SENSOR_ID_E type
// Output:  None
// Return:  POWER_SENSOR_ID_E type
// Note:
******************************************************************************/
LOCAL POWER_SENSOR_ID_E Sensor_IDConvertor(SENSOR_ID_E sensor_id)
{
    POWER_SENSOR_ID_E   power_sensorid=POWER_SENSOR_ID_MAX;

    switch(sensor_id)
    {
        case SENSOR_MAIN:
            power_sensorid=POWER_SENSOR_MAIN;
        break;

        case SENSOR_SUB:
            power_sensorid=POWER_SENSOR_SUB;
        break;

        case SENSOR_ATV:
            power_sensorid=POWER_SENSOR_ATV;
        break;

        default:
            power_sensorid=POWER_SENSOR_ID_MAX;
        break;

    }
    return power_sensorid;
}


/*****************************************************************************/
//  Description:    This function is used to set AVDD
//  Author:         Liangwen.Zhen
//  Note:           Open AVDD on one special voltage or Close it
/*****************************************************************************/
PUBLIC void Sensor_SetVoltage(SENSOR_AVDD_VAL_E dvdd_val, SENSOR_AVDD_VAL_E avdd_val, SENSOR_AVDD_VAL_E iodd_val)
{
    static int32    power_on_times   = 0;
    BOOLEAN         is_turn_sensor_off = FALSE;     //the bit is use for handle the turn on/of situation

    SCI_TRACE_LOW("Sensor:dvdd:%d, avdd:%d, iovdd:%d",dvdd_val, avdd_val, iodd_val);

    is_turn_sensor_off = ((SENSOR_AVDD_CLOSED == dvdd_val)&&(SENSOR_AVDD_CLOSED == avdd_val)&&(SENSOR_AVDD_CLOSED == iodd_val));

    if(is_turn_sensor_off)
    {
        //if turn off, must make the LDO turn off times = turn on times
        while(0 < power_on_times)
        {
            Power_SetSensorVoltage(Sensor_IDConvertor(Sensor_GetCurId()),Sensor_VoltageConvertor(dvdd_val),Sensor_VoltageConvertor(avdd_val),Sensor_VoltageConvertor(iodd_val));
            power_on_times --;
        }
    }
    else
    {
        Power_SetSensorVoltage(Sensor_IDConvertor(Sensor_GetCurId()),Sensor_VoltageConvertor(dvdd_val),Sensor_VoltageConvertor(avdd_val),Sensor_VoltageConvertor(iodd_val));
        power_on_times ++;
    }
}

/*****************************************************************************/
//  Description:    This function is used to power on/off sensor
//  Author:         Liangwen.Zhen
//  Note:           SCI_TRUE: POWER ON; SCI_FALSE: POWER OFF
/*****************************************************************************/
LOCAL void Sensor_PowerOn(BOOLEAN power_on)
{
    BOOLEAN                 power_down      = (BOOLEAN)s_sensor_info_ptr->power_down_level;
    SENSOR_AVDD_VAL_E       dvdd_val        = s_sensor_info_ptr->dvdd_val;
    SENSOR_AVDD_VAL_E       avdd_val        = s_sensor_info_ptr->avdd_val;
    SENSOR_AVDD_VAL_E       iovdd_val       = s_sensor_info_ptr->iovdd_val;
    SENSOR_IOCTL_FUNC_PTR   power_func      = s_sensor_info_ptr->ioctl_func_tab_ptr->power;

    //SENSOR_PRINT:"SENSOR: Sensor_PowerOn -> power_on = %d, power_down_level = %d, avdd_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_623_112_2_18_1_24_33_589,(uint8*)"ddd",power_on,power_down,avdd_val);

    // call user func
    #if 0
    if(PNULL != power_func)
    {
        power_func(power_on);
    }
    else
    #endif
    {
        if(power_on)
        {
            #if 1
            // open power
            Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
            OS_TickDelay(1);   //delay 1ms,make sure voltage output really

            //mclk 12Mhz
            Sensor_SetMCLK_6121e();
            hwp_apApb->clk_ap_mode1_set |= AP_APB_CLK_MODE_CAMERA;
            hwp_apApb->clk_ap_en1_set |= AP_APB_CLK_EN_CAMERA;
            //pwdn pull high
            hwp_camera->CMD_SET |= CAMERA_PWDN;
            OS_TickDelay(1);

            //pwdn pull low
            hwp_camera->CMD_CLR |= CAMERA_PWDN;

            #else
            //sensor power
            Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
            if(g_OpenCamController)
            {
                //open camera controller. set MCLK, RESET, PWDN
                g_OpenCamController(1);
            }
            #endif

        }
        else
        {
            #if 1
            //pwdn pull high
            hwp_camera->CMD_SET |= CAMERA_PWDN;
            OS_TickDelay(1);

            // Close Mclk
            Sensor_disableMCLK_6121e();

            Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);

            //pwdn pull low
            OS_TickDelay(2);
            hwp_camera->CMD_CLR |= CAMERA_PWDN;
            hwp_apApb->clk_ap_en1_clr |= AP_APB_CLK_EN_CAMERA;
            #else
            if(g_OpenCamController)
            {
                //Close camera controller.
                g_OpenCamController(0);
            }
            Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
            #endif
        }
    }
}

/*****************************************************************************/
//  Description:    This function is used to power down sensor
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_PowerDown(BOOLEAN power_down)
{
    SENSOR_INFO_T  *sensor_info_ptr=NULL;
    //uint32 param = 0x00;

    switch(Sensor_GetCurId())
    {
        case SENSOR_MAIN:
        {
            //SENSOR_PRINT:"SENSOR: Sensor_PowerDown -> main, power_down %d, time %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_668_112_2_18_1_24_33_590,(uint8*)"dd",power_down, SCI_GetTickCount());
            sensor_info_ptr=s_sensor_list_ptr[SENSOR_MAIN];
            if((NULL!=sensor_info_ptr)
                &&(NULL!=sensor_info_ptr->ioctl_func_tab_ptr)
                &&(NULL!=sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep))
            {
                sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep(power_down);
            }
            else
            {
                GPIO_SetSensorPwdn(power_down);
                SCI_Sleep(SENSOR_RECOVER_TIME);

            }

            break;
        }
        #if 0
        case SENSOR_SUB:
        {
            //SENSOR_PRINT:"SENSOR: Sensor_PowerDown -> sub, power_down %d, time %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_687_112_2_18_1_24_33_591,(uint8*)"dd",power_down,SCI_GetTickCount());
            sensor_info_ptr=s_sensor_list_ptr[SENSOR_SUB];
            if((NULL!=sensor_info_ptr)
                &&(NULL!=sensor_info_ptr->ioctl_func_tab_ptr)
                &&(NULL!=sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep))
            {
                sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep(power_down);
            }
            else
            {
                GPIO_SetFrontSensorPwdn(power_down);
                SCI_Sleep(SENSOR_RECOVER_TIME);
            }

            break;
        }
        case SENSOR_ATV:
        {
            //SENSOR_PRINT:"SENSOR: Sensor_PowerDown -> atv %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_705_112_2_18_1_24_33_592,(uint8*)"d",power_down);
            sensor_info_ptr = s_sensor_list_ptr[SENSOR_ATV];

            if(SCI_TRUE != s_sensor_register_info_ptr->is_register[SENSOR_ATV])
            {
                //SENSOR_PRINT:"SENSOR: Sensor_PowerDown -> atv not found"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_710_112_2_18_1_24_33_593,(uint8*)"");
                break;
            }

            if(NULL == sensor_info_ptr)
            {
                //SENSOR_PRINT:"SENSOR: Sensor_PowerDown -> atv not found"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_716_112_2_18_1_24_33_594,(uint8*)"");
                break;
            }

            if((NULL!=sensor_info_ptr->ioctl_func_tab_ptr)
                &&(NULL!=sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep))
            {
                sensor_info_ptr->ioctl_func_tab_ptr->enter_sleep(power_down);
            }
            else
            {
                if(sensor_info_ptr->power_down_level == (uint32)power_down) //sleep
                {
                    param=(((ATV_CMD_CHIP_SLEEP<<BIT_4)&SENSOR_HIGN_SIXTEEN_BIT)|1);
                    s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1(param);
                }
                else if(sensor_info_ptr->power_down_level ==(uint32) (!power_down))  //else is wake up
                {
                    // call user func
                    if((PNULL != sensor_info_ptr->ioctl_func_tab_ptr)&&(PNULL != sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1))
                    {
                        param=(((ATV_CMD_CHIP_SLEEP<<BIT_4)&SENSOR_HIGN_SIXTEEN_BIT));
                        sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1(param);
                    }
                    else
                    {

                        // Open power
                        Sensor_SetVoltage(sensor_info_ptr->dvdd_val,
                                            sensor_info_ptr->avdd_val,
                                            sensor_info_ptr->iovdd_val);
                        // Open Mclk in default frequency
                        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);
                        // Reset sensor
                        Sensor_Reset();
                    }
                }
            }

            break;
        }
        #endif

        default :
            break;
    }

    Sensor_SetMode(SENSOR_MODE_MAX);

    return SCI_SUCCESS;

}
/*****************************************************************************/
//  Description:    This function is used to reset img sensor
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_SetResetLevel(BOOLEAN plus_level)
{
    if(SENSOR_TYPE_IMG_SENSOR==_Sensor_GetSensorType())
    {
        GPIO_SetSensorResetLevel(plus_level);
    }
    else if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
    {
        GPIO_SetAnalogTVResetLevel(plus_level);
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to check sensor parameter
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Sensor_CheckSensorInfo(SENSOR_INFO_T * info_ptr)
{
    if(info_ptr->name)
    {
        //SENSOR_PRINT:"SENSOR: Sensor_CheckSensorInfo -> sensor name = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_791_112_2_18_1_24_33_595,(uint8*)"s", info_ptr->name);
    }

    return SCI_TRUE;
}

/*****************************************************************************/
//  Description:    This function is used to power on/off sensor
//  Author:         Liangwen.Zhen
//  Note:           SCI_TRUE: POWER ON; SCI_FALSE: POWER OFF
/*****************************************************************************/
LOCAL void Sensor_SetExportInfo(SENSOR_EXP_INFO_T * exp_info_ptr)
{
    SENSOR_REG_TAB_INFO_T* resolution_info_ptr = NULL;
    SENSOR_TRIM_T_PTR resolution_trim_ptr = NULL;
    SENSOR_INFO_T* sensor_info_ptr = s_sensor_info_ptr;
    uint32 i = 0;

    //SENSOR_PRINT:"SENSOR: Sensor_SetExportInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_807_112_2_18_1_24_33_596,(uint8*)"");

    SCI_MEMSET(exp_info_ptr, 0x00, sizeof(SENSOR_EXP_INFO_T)); /*lint !e718 !e516 !e628  -e516*/

    exp_info_ptr->output_mode = sensor_info_ptr->output_mode;
    exp_info_ptr->output_endian = sensor_info_ptr->output_endian;
    exp_info_ptr->image_format = sensor_info_ptr->image_format;
    exp_info_ptr->image_pattern = sensor_info_ptr->image_pattern;

    exp_info_ptr->pclk_polarity = (sensor_info_ptr->hw_signal_polarity & 0x01) ;  //the high 3bit will be the phase(delay sel)
    exp_info_ptr->vsync_polarity = ((sensor_info_ptr->hw_signal_polarity >> 2) & 0x1);
    exp_info_ptr->hsync_polarity = ((sensor_info_ptr->hw_signal_polarity >> 4) & 0x1);
    exp_info_ptr->pclk_delay = ((sensor_info_ptr->hw_signal_polarity >> 5) & 0x07);

    exp_info_ptr->source_width_max = sensor_info_ptr->source_width_max;
    exp_info_ptr->source_height_max = sensor_info_ptr->source_height_max;

    exp_info_ptr->environment_mode = sensor_info_ptr->environment_mode;
    exp_info_ptr->image_effect = sensor_info_ptr->image_effect;
    exp_info_ptr->wb_mode = sensor_info_ptr->wb_mode;
    exp_info_ptr->step_count = sensor_info_ptr->step_count;

    exp_info_ptr->ext_info_ptr = sensor_info_ptr->ext_info_ptr;

    exp_info_ptr->preview_skip_num = sensor_info_ptr->preview_skip_num;
    exp_info_ptr->capture_skip_num = sensor_info_ptr->capture_skip_num;
    exp_info_ptr->preview_deci_num = sensor_info_ptr->preview_deci_num;
    exp_info_ptr->video_preview_deci_num = sensor_info_ptr->video_preview_deci_num;

    exp_info_ptr->threshold_eb = sensor_info_ptr->threshold_eb;
    exp_info_ptr->threshold_mode = sensor_info_ptr->threshold_mode;
    exp_info_ptr->threshold_start = sensor_info_ptr->threshold_start;
    exp_info_ptr->threshold_end = sensor_info_ptr->threshold_end;

    exp_info_ptr->ioctl_func_ptr=sensor_info_ptr->ioctl_func_tab_ptr;

    if(PNULL!=sensor_info_ptr->ioctl_func_tab_ptr->get_trim)
    {
        resolution_trim_ptr=(SENSOR_TRIM_T_PTR)sensor_info_ptr->ioctl_func_tab_ptr->get_trim(0x00);
    }

    for(i=SENSOR_MODE_COMMON_INIT; i<SENSOR_MODE_MAX; i++)
    {
        resolution_info_ptr = &(sensor_info_ptr->resolution_tab_info_ptr[i]);
        if((PNULL!= resolution_info_ptr->sensor_reg_tab_ptr)||((0x00!=resolution_info_ptr->width)&&(0x00!=resolution_info_ptr->width)))
        {
            exp_info_ptr->sensor_mode_info[i].mode=i;
            exp_info_ptr->sensor_mode_info[i].width=resolution_info_ptr->width;
            exp_info_ptr->sensor_mode_info[i].height=resolution_info_ptr->height;

            if((NULL!=resolution_trim_ptr)
                &&(0x00!=resolution_trim_ptr[i].trim_width)
                &&(0x00!=resolution_trim_ptr[i].trim_height))
            {
                exp_info_ptr->sensor_mode_info[i].trim_start_x=resolution_trim_ptr[i].trim_start_x;
                exp_info_ptr->sensor_mode_info[i].trim_start_y=resolution_trim_ptr[i].trim_start_y;
                exp_info_ptr->sensor_mode_info[i].trim_width=resolution_trim_ptr[i].trim_width;
                exp_info_ptr->sensor_mode_info[i].trim_height=resolution_trim_ptr[i].trim_height;
            }
            else
            {
                exp_info_ptr->sensor_mode_info[i].trim_start_x=0x00;
                exp_info_ptr->sensor_mode_info[i].trim_start_y=0x00;
                exp_info_ptr->sensor_mode_info[i].trim_width=resolution_info_ptr->width;
                exp_info_ptr->sensor_mode_info[i].trim_height=resolution_info_ptr->height;
            }

            if(NULL!=resolution_trim_ptr)
            {
                exp_info_ptr->sensor_mode_info[i].line_time=resolution_trim_ptr[i].line_time;
                exp_info_ptr->sensor_mode_info[i].pclk=resolution_trim_ptr[i].pclk;
            }
            else
            {
                exp_info_ptr->sensor_mode_info[i].line_time=0x00;
                exp_info_ptr->sensor_mode_info[i].pclk=0x00;
            }

            if(SENSOR_IMAGE_FORMAT_MAX != sensor_info_ptr->image_format)
            {
                exp_info_ptr->sensor_mode_info[i].image_format = sensor_info_ptr->image_format;
            }
            else
            {
                exp_info_ptr->sensor_mode_info[i].image_format = resolution_info_ptr->image_format;
            }

            //SENSOR_PRINT:"SENSOR: SENSOR mode Info > mode = %d, width = %d, height = %d, format = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_895_112_2_18_1_24_34_597,(uint8*)"dddd",i, resolution_info_ptr->width, resolution_info_ptr->height, exp_info_ptr->sensor_mode_info[i].image_format);

        }
        else
        {
            exp_info_ptr->sensor_mode_info[i].mode = SENSOR_MODE_MAX;
        }
    }
}

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

//------ To Sensor Module

/*****************************************************************************/
//  Description:    This function is used to write value to sensor register
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void Sensor_WriteReg( uint16  subaddr, uint16 data )
{
    uint8  cmd[4] = {0};
    uint32 index=0, i=0;
    uint32 cmd_num = 0;
    uint32 wr_num = 0;
    SENSOR_IOCTL_FUNC_PTR   write_reg_func;

//    SCI_TRACE_LOW("david Sensor_WriteReg addr 0x%x data 0x%x", subaddr, data);
    write_reg_func = s_sensor_info_ptr->ioctl_func_tab_ptr->write_reg;

    if(PNULL != write_reg_func)
    {
        if(SENSOR_OP_SUCCESS != write_reg_func((subaddr << BIT_4) + data))
        {
            //SENSOR_PRINT:"SENSOR: IIC write : reg:0x%04x, val:0x%04x error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_927_112_2_18_1_24_34_598,(uint8*)"dd", subaddr, data);
        }
    }
    else
    {
        if(SENSOR_I2C_REG_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_REG_16BIT) )
        {
            cmd[cmd_num++] = (uint8)((subaddr >> BIT_3)&SENSOR_LOW_EIGHT_BIT);
            index++;
            cmd[cmd_num++] = (uint8)(subaddr & SENSOR_LOW_EIGHT_BIT);
            index++;
        }
        else
        {
            cmd[cmd_num++] = (uint8)subaddr;
            index++;
        }

        if(SENSOR_I2C_VAL_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_VAL_16BIT))
        {
            cmd[cmd_num++] = (uint8)((data >> BIT_3)&SENSOR_LOW_EIGHT_BIT);
            cmd[cmd_num++] = (uint8)(data & SENSOR_LOW_EIGHT_BIT);
        }
        else
        {
            cmd[cmd_num++] = (uint8)data;
        }

        if(SENSOR_WRITE_DELAY != subaddr)
        {
            if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
            {
                //SENSOR_PRINT:"SENSOR: s_i2c_dev_handler 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_959_112_2_18_1_24_34_599,(uint8*)"d", s_i2c_dev_handler);
            }
            else
            {

                for(i = 0; i < SENSOR_I2C_OP_TRY_NUM; i++)
                {
                    wr_num = I2C_HAL_Write(s_i2c_dev_handler, cmd, &cmd[index], cmd_num-index);
                    if(wr_num != cmd_num-index)
                    {
                        //SENSOR_PRINT:"SENSOR: IIC write reg Error! 0x%04x, val:0x%04x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_969_112_2_18_1_24_34_600,(uint8*)"dd", subaddr, data);
                        OS_TickDelay(2);
                    }
                    else
                    {
                        //SENSOR_PRINT:"SENSOR: IIC write reg OK! 0x%04x, val:0x%04x "
                        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_974_112_2_18_1_24_34_601,(uint8*)"dd", subaddr, data);

                        break;
                    }
                }
            }
        }
        else
        {
            if(data > 0x80)
            {
                SCI_Sleep(data);
            }
            else
            {
                OS_TickDelay(data);
            }
            //SENSOR_PRINT:"SENSOR: IIC write Delay %d ms"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_990_112_2_18_1_24_34_602,(uint8*)"d", data);
        }
    }

    return ;

}

PUBLIC void Sensor_WriteRegtest( uint16  subaddr, uint16 data )
{
    uint8  cmd[4] = {0};
    uint32 index=0, i=0;
    uint32 cmd_num = 0;
    uint32 wr_num = 0;
    SENSOR_IOCTL_FUNC_PTR   write_reg_func;

//    SCI_TRACE_LOW("david Sensor_WriteReg addr 0x%x data 0x%x", subaddr, data);
#if 0
write_reg_func = s_sensor_info_ptr->ioctl_func_tab_ptr->write_reg;

    if(PNULL != write_reg_func)
    {
        if(SENSOR_OP_SUCCESS != write_reg_func((subaddr << BIT_4) + data))
        {
            //SENSOR_PRINT:"SENSOR: IIC write : reg:0x%04x, val:0x%04x error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_927_112_2_18_1_24_34_598,(uint8*)"dd", subaddr, data);
        }
    }
    else
    #endif
    {
        #if 0
        if(SENSOR_I2C_REG_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_REG_16BIT) )
        {
            cmd[cmd_num++] = (uint8)((subaddr >> BIT_3)&SENSOR_LOW_EIGHT_BIT);
            index++;
            cmd[cmd_num++] = (uint8)(subaddr & SENSOR_LOW_EIGHT_BIT);
            index++;
        }
        else
        #endif
        {
            cmd[cmd_num++] = (uint8)subaddr;
            index++;
        }

        #if 0
        if(SENSOR_I2C_VAL_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_VAL_16BIT))
        {
            cmd[cmd_num++] = (uint8)((data >> BIT_3)&SENSOR_LOW_EIGHT_BIT);
            cmd[cmd_num++] = (uint8)(data & SENSOR_LOW_EIGHT_BIT);
        }
        else
        #endif
        {
            cmd[cmd_num++] = (uint8)data;
        }

        if(SENSOR_WRITE_DELAY != subaddr)
        {
            if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
            {
                //SENSOR_PRINT:"SENSOR: s_i2c_dev_handler 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_959_112_2_18_1_24_34_599,(uint8*)"d", s_i2c_dev_handler);
            }
            else
            {

                for(i = 0; i < SENSOR_I2C_OP_TRY_NUM; i++)
                {
                    wr_num = I2C_HAL_Write(s_i2c_dev_handler, cmd, &cmd[index], cmd_num-index);
                    if(wr_num != cmd_num-index)
                    {
                        //SENSOR_PRINT:"SENSOR: IIC write reg Error! 0x%04x, val:0x%04x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_969_112_2_18_1_24_34_600,(uint8*)"dd", subaddr, data);
                        SCI_Sleep(20);
                    }
                    else
                    {
                        //SENSOR_PRINT:"SENSOR: IIC write reg OK! 0x%04x, val:0x%04x "
                        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_974_112_2_18_1_24_34_601,(uint8*)"dd", subaddr, data);

                        break;
                    }
                }
            }
        }
        else
        {
            if(data > 0x80)
            {
                SCI_Sleep(data);
            }
            else
            {
                OS_TickDelay(data);
            }
            //SENSOR_PRINT:"SENSOR: IIC write Delay %d ms"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_990_112_2_18_1_24_34_602,(uint8*)"d", data);
        }
    }

    return ;

}

/*****************************************************************************/
//  Description:    This function is used to read value from sensor register
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint16 Sensor_ReadReg(uint16 subaddr)
{
    uint32 i=0;
    I2C_DEV dev;
    uint8  cmd[2] = {0};
    uint16 ret_val;
    uint16 w_cmd_num = 0;
    uint16 r_cmd_num = 0;
    uint16 ret_num = 0;
    SENSOR_IOCTL_FUNC_PTR   read_reg_func;

    //SCI_TRACE_LOW("david Sensor_ReadReg addr 0x%x", subaddr);

    #if 0
    read_reg_func = s_sensor_info_ptr->ioctl_func_tab_ptr->read_reg;

    if(PNULL != read_reg_func)
    {
        //SCI_TRACE_LOW("david Sensor_ReadReg 1");
        ret_val = (uint16)read_reg_func((uint32)(subaddr & SENSOR_LOW_SIXTEEN_BIT));
    }
    else
    #endif
    {
        //SCI_TRACE_LOW("david Sensor_ReadReg 22");
        if(SENSOR_I2C_REG_16BIT==(s_sensor_info_ptr->reg_addr_value_bits&SENSOR_I2C_REG_16BIT))
        {
            cmd[w_cmd_num++] = (uint8)((subaddr >>BIT_3)&SENSOR_LOW_EIGHT_BIT);
            cmd[w_cmd_num++] = (uint8)(subaddr & SENSOR_LOW_EIGHT_BIT);
        }
        else
        {
            cmd[w_cmd_num++] = (uint8)subaddr;
        }

        dev.reg_addr_num = w_cmd_num;

        if(SENSOR_I2C_VAL_16BIT==(s_sensor_info_ptr->reg_addr_value_bits & SENSOR_I2C_VAL_16BIT) )
        {
            r_cmd_num = SENSOR_CMD_BITS_16;
        }
        else
        {
            r_cmd_num = SENSOR_CMD_BITS_8;
        }

        if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
        {
            //SENSOR_PRINT:"SENSOR: s_i2c_dev_handler 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1043_112_2_18_1_24_34_603,(uint8*)"d", s_i2c_dev_handler);
            ret_val = 0xFFFF;
        }
        else
        {
            //SCI_TRACE_LOW("david Sensor_ReadReg 33");
            for(i = 0; i < SENSOR_I2C_OP_TRY_NUM; i++)
            {
                //SCI_TRACE_LOW("david Sensor_ReadReg 44");
                ret_num = I2C_HAL_Read(s_i2c_dev_handler, cmd, &cmd[0], r_cmd_num);
                if(ret_num != r_cmd_num)
                {
                    //SCI_TRACE_LOW:"SENSOR: IIC read reg, ERROR! %d"
                   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1054_112_2_18_1_24_34_604,(uint8*)"d", ret_num);
                    OS_TickDelay(2);
                    ret_val = 0xFFFF;
                }
                else
                {
                    ret_val = (r_cmd_num == 1)?(uint16)cmd[0]:(uint16)((cmd[0] << 8) + cmd[1]);
                    //SCI_TRACE_LOW:"SENSOR: IIC read reg OK:0x%04x, val:0x%04x"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1061_112_2_18_1_24_34_605,(uint8*)"dd", subaddr, ret_val);
                    break;
                }
            }
        }
    }

    return  ret_val;
}


PUBLIC uint16 Sensor_ReadRegtest(uint16 subaddr)
{
    uint32 i=0;
    I2C_DEV dev;
    uint8  cmd[2] = {0};
    uint16 ret_val;
    uint16 w_cmd_num = 0;
    uint16 r_cmd_num = 0;
    uint16 ret_num = 0;
    SENSOR_IOCTL_FUNC_PTR   read_reg_func;

    //SCI_TRACE_LOW("david Sensor_ReadReg addr 0x%x", subaddr);
    read_reg_func = s_sensor_info_ptr->ioctl_func_tab_ptr->read_reg;

#if 0
    if(PNULL != read_reg_func)
    {
        //SCI_TRACE_LOW("david Sensor_ReadReg 1");
        ret_val = (uint16)read_reg_func((uint32)(subaddr & SENSOR_LOW_SIXTEEN_BIT));
    }
    else
#endif

    {
        //SCI_TRACE_LOW("david Sensor_ReadReg 22");
        #if 0
        if(SENSOR_I2C_REG_16BIT==(0&SENSOR_I2C_REG_16BIT))
        {
            cmd[w_cmd_num++] = (uint8)((subaddr >>BIT_3)&SENSOR_LOW_EIGHT_BIT);
            cmd[w_cmd_num++] = (uint8)(subaddr & SENSOR_LOW_EIGHT_BIT);
        }
        else
        #endif
        {
            cmd[w_cmd_num++] = (uint8)subaddr;
        }

        dev.reg_addr_num = w_cmd_num;


        if(SENSOR_I2C_VAL_16BIT==(0& SENSOR_I2C_VAL_16BIT) )
        {
            r_cmd_num = SENSOR_CMD_BITS_16;
        }
        else
        {
            r_cmd_num = SENSOR_CMD_BITS_8;
        }

        if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
        {
            //SENSOR_PRINT:"SENSOR: s_i2c_dev_handler 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1043_112_2_18_1_24_34_603,(uint8*)"d", s_i2c_dev_handler);
            ret_val = 0xFFFF;
        }
        else
        {
            //SCI_TRACE_LOW("david Sensor_ReadReg 33");
            for(i = 0; i < SENSOR_I2C_OP_TRY_NUM; i++)
            {
                //SCI_TRACE_LOW("david Sensor_ReadReg 44");
                ret_num = I2C_HAL_Read(s_i2c_dev_handler, cmd, &cmd[0], r_cmd_num);
                if(ret_num != r_cmd_num)
                {
                    //SCI_TRACE_LOW:"SENSOR: IIC read reg, ERROR! %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1054_112_2_18_1_24_34_604,(uint8*)"d", ret_num);
                    SCI_Sleep(20);
                    ret_val = 0xFFFF;
                }
                else
                {
                    ret_val = (r_cmd_num == 1)?(uint16)cmd[0]:(uint16)((cmd[0] << 8) + cmd[1]);
                    //SCI_TRACE_LOW:"SENSOR: IIC read reg OK:0x%04x, val:0x%04x"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1061_112_2_18_1_24_34_605,(uint8*)"dd", subaddr, ret_val);
                    break;
                }
            }
        }
    }

    return  ret_val;
}
/*****************************************************************************/
//  Description:    This function is used to send a table of register to sensor
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_SendRegTabToSensor(SENSOR_REG_TAB_INFO_T * sensor_reg_tab_info_ptr   )
{
    uint32 i;
    uint8 value;
    uint32 times = 0;
    uint32 stat[12] = {1};
    uint32 err = 0;
    uint32 total = 0;

    //SENSOR_PRINT:"SENSOR: Sensor_SendRegValueToSensor -> reg_count = %d start time = %d"

    if((NULL==sensor_reg_tab_info_ptr)
        ||(NULL==sensor_reg_tab_info_ptr->sensor_reg_tab_ptr))
    {
        //SENSOR_PRINT:"SENSOR: Sensor_SendRegTabToSensor tab is null error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1085_112_2_18_1_24_34_607,(uint8*)"");
        return SENSOR_OP_SUCCESS;
    }
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1080_112_2_18_1_24_34_606,(uint8*)"dd",sensor_reg_tab_info_ptr->reg_count,SCI_GetTickCount());

    times = 3;
    memset(stat, 0xFF, sizeof(uint32)*12);
    err   = sensor_reg_tab_info_ptr->reg_count;
    total = sensor_reg_tab_info_ptr->reg_count;

    for( i = 0; i < total; i++)
    {
        OS_TickDelay(1);
        Sensor_WriteReg(sensor_reg_tab_info_ptr->sensor_reg_tab_ptr[i].reg_addr, \
                             sensor_reg_tab_info_ptr->sensor_reg_tab_ptr[i].reg_value);

        #if 0
        //delay 2 ms
        OS_TickDelay(1);
        value = Sensor_ReadReg(sensor_reg_tab_info_ptr->sensor_reg_tab_ptr[i].reg_addr);
        if(value != sensor_reg_tab_info_ptr->sensor_reg_tab_ptr[i].reg_value)
        {
            stat[i/32] |= 0x1 << (i%32);
            //SCI_TRACE_LOW(" sensor write error: addr 0x%x value 0x%x",
            //            GC032A_YUV_320X240_setting[i].reg_addr, value);

        }
        else
        {
            err--;
            stat[i/32] &= ~(0x1 << (i%32));
            break;
        }
        #endif
    }
    OS_TickDelay(2);
    //SCI_TRACE_LOW("david send setting over 11: stat0-3 0x%x 0x%x 0x%x 0x%x",stat[0], stat[1], stat[2], stat[3]);
    //SCI_TRACE_LOW("david send setting over 22: stat4-7 0x%x 0x%x 0x%x 0x%x",stat[4], stat[5], stat[6], stat[7]);
    //SCI_TRACE_LOW("david send setting over 33: stat8-9 0x%x 0x%x, err %d",stat[8], stat[9], err);

    //SENSOR_PRINT:"SENSOR: Sensor_SendRegValueToSensor -> end time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1097_112_2_18_1_24_34_608,(uint8*)"d", SCI_GetTickCount());

    return SENSOR_OP_SUCCESS;
}

//------ To Digital Camera Module

/*****************************************************************************/
//  Description:    This function is used to reset sensor
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
LOCAL void _Sensor_CleanInformation(void)
{
    uint32 i;
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=s_sensor_register_info_ptr;

    s_sensor_info_ptr=PNULL;

    s_sensor_init=SCI_FALSE;
    s_sensor_open=SCI_FALSE;

    s_atv_init=SCI_FALSE;
    s_atv_open=SCI_FALSE;

    for(i=0;i<SENSOR_ID_MAX;i++)
    {
        s_sensor_list_ptr[i]=PNULL;
    }
    SCI_MEMSET(&s_sensor_exp_info, 0x00, sizeof(s_sensor_exp_info));
    SCI_MEMSET(sensor_register_info_ptr, 0x00, sizeof(SENSOR_REGISTER_INFO_T));
    sensor_register_info_ptr->cur_id=SENSOR_ID_MAX;

    return ;
}

/*****************************************************************************/
//  Description:    This function is used to set currect sensor id
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
LOCAL void _Sensor_SetId(SENSOR_ID_E sensor_id)
{
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=s_sensor_register_info_ptr;

    sensor_register_info_ptr->cur_id=sensor_id;

    return ;
}

/*****************************************************************************/
//  Description:    This function is used to get currect sensor id
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC SENSOR_ID_E Sensor_GetCurId(void)
{
    return 0;

    //SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=s_sensor_register_info_ptr;
    //return (SENSOR_ID_E)sensor_register_info_ptr->cur_id;
}

/*****************************************************************************/
//  Description:    This function is used to set all sensor power down
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
LOCAL uint32 _Sensor_SetAllPowerDown(void)
{
    //return SCI_SUCCESS;

    uint8 cur_sensor_id=0x00;
    uint8 sensor_id=0x00;

    cur_sensor_id=Sensor_GetCurId();

    _Sensor_IicHandlerRelease();
    for(sensor_id=0x00; sensor_id<SENSOR_ID_MAX; sensor_id++)
    {
        if(NULL == s_sensor_list_ptr[sensor_id])
        {
            continue;
        }
        _Sensor_SetId((SENSOR_ID_E)sensor_id);
        s_sensor_info_ptr=s_sensor_list_ptr[sensor_id];
        Sensor_SetExportInfo(&s_sensor_exp_info);
         _Sensor_IicHandlerInit((SENSOR_ID_E)sensor_id);
        Sensor_PowerDown((BOOLEAN)s_sensor_info_ptr->power_down_level);
        _Sensor_IicHandlerRelease();

    }

    _Sensor_SetId((SENSOR_ID_E)cur_sensor_id);

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to set currect sensor id and set sensor
//                  information
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_SetCurId(SENSOR_ID_E sensor_id)
{
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=s_sensor_register_info_ptr;

    //SCI_TRACE_LOW:"Sensor_SetCurId : %d, start time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1192_112_2_18_1_24_34_609,(uint8*)"dd", sensor_id, SCI_GetTickCount());

    if(sensor_id >= SENSOR_ID_MAX)
    {
        //SCI_TRACE_LOW:"Sensor_SetCurId : sensor_id %d unsupported"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1196_112_2_18_1_24_34_610,(uint8*)"d", sensor_id);
        return SENSOR_OP_PARAM_ERR;
    }

    if(SCI_TRUE != sensor_register_info_ptr->is_register[sensor_id])
    {
        //SENSOR_PRINT:"Sensor_SetCurId: No such sensor! %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1202_112_2_18_1_24_34_611,(uint8*)"d", sensor_id);
        return SENSOR_OP_PARAM_ERR;
    }

    if(sensor_register_info_ptr->cur_id != sensor_id)
    {
        if(Sensor_IsOpen())
        {
            Sensor_PowerOn(SCI_FALSE);
            if(!_Sensor_PowerUpHandle(sensor_id))
            {
                return SCI_ERROR;
            }
        }
        else
        {
            s_sensor_info_ptr=s_sensor_list_ptr[sensor_id];
            Sensor_SetExportInfo(&s_sensor_exp_info);
            _Sensor_SetId(sensor_id);
        }

    }
    //SENSOR_PRINT:"SENSOR:Sensor_SetCurId id:%d, num:%d, ptr: 0x%x, ptr: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1228_112_2_18_1_24_34_612,(uint8*)"dddd", sensor_id, sensor_register_info_ptr->img_sensor_num,s_sensor_list_ptr, s_sensor_info_ptr);

    //SCI_TRACE_LOW:"Sensor_SetCurId : %d, end time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1230_112_2_18_1_24_34_613,(uint8*)"dd", sensor_id, SCI_GetTickCount());
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get info of register sensor
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC SENSOR_REGISTER_INFO_T_PTR Sensor_GetRegisterInfo(void)
{
    return s_sensor_register_info_ptr;
}


/*****************************************************************************
//  Description:    This function is handle the power up issue of appointed sensor
//  Author:         xi.zhang
//  Global;         SENSOR_INFO_T* s_sensor_info_ptr
                    SENSOR_INFO_T* s_sensor_list_ptr
                    SENSOR_REGISTER_INFO_T_PTR s_sensor_register_info_ptr
//  Note:
*****************************************************************************/
LOCAL void _Sensor_Identify(SENSOR_ID_E sensor_id)
{
    uint sensor_index = 0;
    SENSOR_INFO_T** sensor_info_tab_ptr = PNULL;
    uint32 valid_tab_index_max = 0x00;
    SENSOR_INFO_T* sensor_info_ptr = PNULL;

    //SENSOR_PRINT:"SENSOR: sensor identifing %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1257_112_2_18_1_24_34_614,(uint8*)"d", sensor_id);

    //if already identified
    if(SCI_TRUE == s_sensor_register_info_ptr->is_register[sensor_id])
    {
        //SENSOR_PRINT:"SENSOR: sensor identified"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1262_112_2_18_1_24_34_615,(uint8*)"");
        return;
    }

    _Sensor_SetId(sensor_id);
    sensor_info_tab_ptr=(SENSOR_INFO_T**)Sensor_GetInforTab(sensor_id);
    valid_tab_index_max=Sensor_GetInforTabLenght(sensor_id)-SENSOR_ONE_I2C;

    //if sensor had been identified, then directly return the sensor info
    if(s_sensor_identified)
    {
        if(0xFF != s_sensor_index[sensor_id])
        {
            //get the sensor info from the table
            sensor_info_ptr =  sensor_info_tab_ptr[s_sensor_index[sensor_id]];
            if(NULL==sensor_info_ptr)
            {
                //SENSOR_PRINT:"SENSOR: %d info of Sensor_Init table %d is null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1277_112_2_18_1_24_34_616,(uint8*)"dd", sensor_index, (uint)sensor_id);
                return ;
            }
            s_sensor_info_ptr = sensor_info_ptr;

            //check if sensor works OK
            _Sensor_IicHandlerInit(sensor_id);
            Sensor_PowerOn(SCI_TRUE);
            if(PNULL!=sensor_info_ptr->ioctl_func_tab_ptr->identify)
            {
                ImgSensor_GetMutex();
                if(SCI_SUCCESS==sensor_info_ptr->ioctl_func_tab_ptr->identify(SENSOR_ZERO_I2C))
                {
                    s_sensor_list_ptr[sensor_id] = sensor_info_ptr;
                    s_sensor_register_info_ptr->is_register[sensor_id]=SCI_TRUE;
                    s_sensor_register_info_ptr->img_sensor_num++;
                }
                else
                {
                    s_sensor_list_ptr[sensor_id] = PNULL;
                    s_sensor_register_info_ptr->is_register[sensor_id]=SCI_FALSE;
                    s_sensor_register_info_ptr->img_sensor_num--;
                }

                ImgSensor_PutMutex();
                _Sensor_IicHandlerRelease();
                Sensor_PowerOn(SCI_FALSE);
            }
        }

        //if sensor work OK, directly return the result
#if  defined(_DC_WRONG_SENSOR_COMPATIBLE_)
        if(SCI_TRUE == s_sensor_register_info_ptr->is_register[sensor_id])
#endif
        {
            //SENSOR_PRINT("SENSOR TYPE of %d indentified result %d", sensor_id, s_sensor_register_info_ptr->is_register[sensor_id]);
            return;
        }
    }

    //search the sensor in the table
    s_sensor_register_info_ptr->is_register[sensor_id] = SCI_FALSE;

    for(sensor_index=0x00; sensor_index<valid_tab_index_max;sensor_index++)
    {
        sensor_info_ptr = sensor_info_tab_ptr[sensor_index];

        if(NULL==sensor_info_ptr)
        {
            //SENSOR_PRINT:"SENSOR: %d info of Sensor_Init table %d is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1277_112_2_18_1_24_34_616,(uint8*)"dd", sensor_index, (uint)sensor_id);
            continue ;
        }
        s_sensor_info_ptr = sensor_info_ptr;

        _Sensor_IicHandlerInit((SENSOR_ID_E)sensor_index);

        Sensor_PowerOn(SCI_TRUE);

        if(PNULL!=sensor_info_ptr->ioctl_func_tab_ptr->identify)
        {
            ImgSensor_GetMutex();
            if(SCI_SUCCESS==sensor_info_ptr->ioctl_func_tab_ptr->identify(SENSOR_ZERO_I2C))
            {
                SCI_TRACE_LOW("Sensor:identify ok!");
                s_sensor_list_ptr[sensor_id]=sensor_info_ptr;
                s_sensor_register_info_ptr->is_register[sensor_id]=SCI_TRUE;
                s_sensor_register_info_ptr->img_sensor_num++;
                s_sensor_index[sensor_id] = sensor_index;
                ImgSensor_PutMutex();
                Sensor_PowerOn(SCI_FALSE);
                _Sensor_IicHandlerRelease();
                break;
            }
            ImgSensor_PutMutex();
        }
        //SCI_TRACE_LOW("david identify over!");
        _Sensor_IicHandlerRelease();

        Sensor_PowerOn(SCI_FALSE);
    }

    //print identify results
    if(SCI_TRUE == s_sensor_register_info_ptr->is_register[sensor_id])
    {
        //SENSOR_PRINT:"SENSOR TYPE of %d indentify OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1310_112_2_18_1_24_35_617,(uint8*)"d",(uint)sensor_id);
    }
    else
    {
        s_sensor_index[sensor_id] = 0xFF;                       //set the sensor index to none again
        //SENSOR_PRINT:"SENSOR TYPE of %d indentify FAILURE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1314_112_2_18_1_24_35_618,(uint8*)"d",(uint)sensor_id);
    }
}

/*****************************************************************************/
//  Description:    This function is used to initialize Sensor function
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_Init(void)
{
    BOOLEAN ret_val=SCI_FALSE;
    uint32 i = 0;

    //SENSOR_PRINT:"SENSOR: Sensor_Init start %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1326_112_2_18_1_24_35_619,(uint8*)"d", SCI_GetTickCount());

    if(Sensor_IsInit())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Init is done\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1330_112_2_18_1_24_35_620,(uint8*)"");
        for(i=0; i<SENSOR_ID_MAX; i++)
        {
           SCI_TRACE_LOW("Sensor:Init the index %d is %d", i, s_sensor_index[i] );
        }
        return SCI_TRUE;
    }

    //SCI_ASSERT(DEVICE_Find("CCIR", &s_dev_ccir)==CLOCK_SUCCESS);/*assert verified*/

    //Clean the information of img sensor
    //sensor_SetCurId(SENSOR_ID_MAX);
    _Sensor_CleanInformation();

    //if ATV then pass
    if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
    {
        s_atv_init=SCI_TRUE;
        s_atv_open=SCI_FALSE;
        ret_val = SCI_TRUE;
        return ret_val;
    }

    //for the issue of ATV leakage, the ATV sensor should be power up and sleep at first
    //and also for this leak issue, we cannot do the identify issue of ATV in initial
    if(PNULL != Sensor_GetInforTab(SENSOR_ATV)[0])
    {
        s_sensor_info_ptr = (SENSOR_INFO_T*)Sensor_GetInforTab(SENSOR_ATV)[0];

       _Sensor_SetId(SENSOR_ATV);
        _Sensor_IicHandlerInit(SENSOR_ATV);
        Sensor_PowerOn(SCI_TRUE);

        //give the atv sensor pointer directly to the info pointer
        s_sensor_list_ptr[SENSOR_ATV]=s_sensor_info_ptr;
        s_sensor_register_info_ptr->is_register[SENSOR_ATV]=SCI_TRUE;
        Sensor_SetExportInfo(&s_sensor_exp_info);
        s_sensor_index[SENSOR_ATV] = 0;

        Sensor_PowerDown((BOOLEAN)s_sensor_info_ptr->power_down_level);
        _Sensor_IicHandlerRelease();
    }

    //SCI_TRACE_LOW("before sensor identify!");

    // main img sensor
    _Sensor_Identify(SENSOR_MAIN);

    //SCI_TRACE_LOW("after sensor identify!");

    // sub img sensor
    //_Sensor_Identify(SENSOR_SUB);

    s_sensor_identified = SCI_TRUE;

    //init flag mark
    if(PNULL!=s_sensor_list_ptr[SENSOR_MAIN])
    {
        s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_MAIN];
        Sensor_SetExportInfo(&s_sensor_exp_info);
        _Sensor_SetId(SENSOR_MAIN);

        s_sensor_init = SCI_TRUE;
        ret_val=SCI_TRUE;
        //SENSOR_PRINT:"SENSOR: Sensor_Init Main Success \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1383_112_2_18_1_24_35_621,(uint8*)"");
    }
    else if(PNULL!=s_sensor_list_ptr[SENSOR_SUB])
    {
        s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_SUB];
        Sensor_SetExportInfo(&s_sensor_exp_info);
        _Sensor_SetId(SENSOR_SUB);

        s_sensor_init = SCI_TRUE;
        ret_val=SCI_TRUE;
        //SENSOR_PRINT:"SENSOR: Sensor_Init Sub Success \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1393_112_2_18_1_24_35_622,(uint8*)"");
    }
    else
    {
        _Sensor_SetId(SENSOR_ID_MAX);
        s_sensor_init = SCI_FALSE;
        ret_val=SCI_FALSE;
        //SENSOR_PRINT:"SENSOR: Sensor_Init Fail No Sensor err \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1400_112_2_18_1_24_35_623,(uint8*)"");
    }

    //SENSOR_PRINT:"SENSOR: Sensor_Init: end: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1403_112_2_18_1_24_35_624,(uint8*)"d", SCI_GetTickCount());
    return ret_val;
}

/*****************************************************************************
//  Description:    restore the sensor state from intial operation
//  Author:         xi.zhang
//  Note:
*****************************************************************************/
PUBLIC void Sensor_RestoreStateFromInit(void)
{
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=Sensor_GetRegisterInfo();
    SENSOR_ID_E cur_sensor_id=Sensor_GetCurId();

    //confirm the ATV LDO be shutted down
    if(SCI_TRUE==sensor_register_info_ptr->is_register[SENSOR_ATV])
    {
        s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_ATV];
        _Sensor_SetId(SENSOR_ATV);
        Sensor_PowerOn(SCI_FALSE);
    }

    _Sensor_SetId(cur_sensor_id);

    //set the global sensor variable to close state
    s_sensor_mode[SENSOR_MAIN]=SENSOR_MODE_MAX;
    s_sensor_mode[SENSOR_SUB]=SENSOR_MODE_MAX;
    s_sensor_mode[SENSOR_ATV]=SENSOR_MODE_MAX;
}

/*****************************************************************************/
//  Description:    This function is used to check if sensor has been init
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_IsInit(void)
{
    if(SENSOR_TYPE_IMG_SENSOR==_Sensor_GetSensorType())
    {
        return s_sensor_init;
    }
    else if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
    {
        return s_atv_init;
    }

    return SCI_FALSE;
}

/*****************************************************************************
//  Description:    This function is to set sensor index value
//  Author:         xi.zhang
//  Note:
*****************************************************************************/
PUBLIC void Sensor_SetIdentifiedState(BOOLEAN if_identified, uint8 *sensor_index)
{
    int i = 0;
    uint8 * index_ptr = sensor_index;

    s_sensor_identified = if_identified;
    for(i = 0; i < SENSOR_ID_MAX; i++)
    {
        s_sensor_index[i] = *index_ptr++;
        SCI_TRACE_LOW("Sensor:SetState the index of %d is %d", i, s_sensor_index[i]);
    }
}

/*****************************************************************************
//  Description:    This function is to get sensor index value
//  Author:         xi.zhang
//  Note:
*****************************************************************************/
PUBLIC void Sensor_GetIdentifiedState(SENSOR_ID_E sensor_id, uint8 *p_sensor_index)
{
    *p_sensor_index = s_sensor_index[sensor_id];
    SCI_TRACE_LOW("Sensor:GetState the index of %d is %d", sensor_id, s_sensor_index[sensor_id]);
    return;
}

/*****************************************************************************
//  Description:    Check if there exist camera
//  Author:         xi.zhang
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN Sensor_CheckIfHaveCamera(void)
{
    int i=0;
    for(i = SENSOR_MAIN; i <= SENSOR_SUB; i++)
    {
        if(0xFF != s_sensor_index[i])
        {
            return SCI_TRUE;
        }
    }

    return SCI_FALSE;
}

int sensor_powerup()
{
    uint iTmpID = 0;
    int sensor_id = 0;
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=Sensor_GetRegisterInfo();
    uint32 param = 0x00;

    //SENSOR_PRINT:"SENSOR: Sensor_powerUpHandle start %d"
   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1437_112_2_18_1_24_35_625,(uint8*)"d", SCI_GetTickCount());
    s_sensor_info_ptr=s_sensor_list_ptr[sensor_id];
    Sensor_SetExportInfo(&s_sensor_exp_info);
    _Sensor_SetId(sensor_id);
    //_Sensor_IicHandlerInit(sensor_id);
    // Sensor_PowerOn(SCI_TRUE);
    //SCI_TRACE_LOW("davidtest send regs!");
    Sensor_SetMode(SENSOR_MODE_COMMON_INIT);

    SCI_TRACE_LOW("Sensor:powerup over!");
    return 0; // CID37010
}

/*****************************************************************************
//  Description:    This function is handle the power up issue of appointed sensor
//  Author:         xi.zhang
//  Global;         SENSOR_INFO_T* s_sensor_info_ptr
                    SENSOR_INFO_T* s_sensor_list_ptr

//  Note:
*****************************************************************************/
LOCAL BOOLEAN _Sensor_PowerUpHandle(SENSOR_ID_E sensor_id)
{
    uint iTmpID = 0;
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=Sensor_GetRegisterInfo();
    uint32 param = 0x00;

    //SENSOR_PRINT:"SENSOR: Sensor_powerUpHandle start %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1437_112_2_18_1_24_35_625,(uint8*)"d", SCI_GetTickCount());

    //handle current sensor
    if(PNULL == s_sensor_list_ptr[sensor_id])
    {
        //SENSOR_PRINT:"SENSOR: Sensor_powerUpHandle error for sensor info of %d is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1442_112_2_18_1_24_35_626,(uint8*)"d", sensor_id);
        return SCI_FALSE;
    }
    s_sensor_info_ptr=s_sensor_list_ptr[sensor_id];
    Sensor_SetExportInfo(&s_sensor_exp_info);
    _Sensor_SetId(sensor_id);
    _Sensor_IicHandlerInit(sensor_id);
    Sensor_PowerOn(SCI_TRUE);

    #if 0
    /* First, put the sensor into power down, to avoid any disturber from current sensor while shutdown the other sensor */
    Sensor_PowerDown((BOOLEAN)s_sensor_info_ptr->power_down_level);
    _Sensor_IicHandlerRelease();

    //power up and sleep another sensor
    for(iTmpID = 0; iTmpID <SENSOR_ID_MAX; iTmpID ++)
    {
        //current sensor need not handle
        if(sensor_id == iTmpID)
        {
            continue;
        }

        if(SCI_TRUE==sensor_register_info_ptr->is_register[iTmpID])
        {
            _Sensor_SetId(iTmpID);
            s_sensor_info_ptr=s_sensor_list_ptr[iTmpID];
            Sensor_SetExportInfo(&s_sensor_exp_info);
            _Sensor_IicHandlerInit((SENSOR_ID_E)iTmpID);
            Sensor_PowerOn(SCI_TRUE);
            Sensor_PowerDown((BOOLEAN)s_sensor_info_ptr->power_down_level);
            //SENSOR_PRINT:"SENSOR: Sensor_sleep of id %d, time %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1473_112_2_18_1_24_35_627,(uint8*)"dd",iTmpID,SCI_GetTickCount());
            _Sensor_IicHandlerRelease();
        }
    }
    #endif

    /*the end ,recover the current sensor */
    s_sensor_info_ptr=s_sensor_list_ptr[sensor_id];
    Sensor_SetExportInfo(&s_sensor_exp_info);
    _Sensor_SetId(sensor_id);

    //_Sensor_IicHandlerInit(sensor_id);
    //powerup and initial appoint sensor
    //Sensor_PowerDown((BOOLEAN)(!((BOOLEAN)s_sensor_info_ptr->power_down_level)));

    //SCI_TRACE_LOW("david before identify");
    if(PNULL!=s_sensor_info_ptr->ioctl_func_tab_ptr->identify)      //sensor identify
    {
        if(SCI_SUCCESS!=s_sensor_info_ptr->ioctl_func_tab_ptr->identify(0x00))
        {
            Sensor_PowerOn(SCI_FALSE);
            _Sensor_IicHandlerRelease();
            //SENSOR_PRINT:"SENSOR: Sensor_Open the sensor identify fail %d error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1493_112_2_18_1_24_35_628,(uint8*)"d",__LINE__);

            return FALSE;
        }
    }

    if(SENSOR_ATV == sensor_id)
    {
        if(NULL!=s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1)
        {

            param=(ATV_CMD_CHIP_INIT<<BIT_4)&SENSOR_HIGN_SIXTEEN_BIT;
            if(SCI_SUCCESS!=s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1(param))
            {
                s_atv_open=SCI_FALSE;
                Sensor_PowerOn(SCI_FALSE);
                _Sensor_IicHandlerRelease();

                //SENSOR_PRINT:"SENSOR: Sensor_Open the atv init fail %d error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1511_112_2_18_1_24_35_629,(uint8*)"d",__LINE__);
                return FALSE;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW("david after identify");
    //    send init regs to sensor
        Sensor_SetMode(SENSOR_MODE_COMMON_INIT);
    }
    SCI_TRACE_LOW("Sensor power up end");

    return TRUE;

}

/*****************************************************************************/
//  Description:    This function is used to Open sensor function
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_Open(void)
{
    SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=Sensor_GetRegisterInfo();
    SENSOR_ID_E cur_sensor_id=SENSOR_ID_MAX;
    SENSOR_EXT_FUN_T ext_cmd={0x00};
    uint32 param = 0;

    //SENSOR_PRINT:"SENSOR: Sensor_Open sensor type: %d id: %d, time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1535_112_2_18_1_24_35_630,(uint8*)"ddd", _Sensor_GetSensorType(), Sensor_GetCurId(), SCI_GetTickCount());

    if(!Sensor_IsInit())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Open -> sensor has not init"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1539_112_2_18_1_24_35_631,(uint8*)"");
        s_atv_open=SCI_FALSE;
        s_sensor_open=SCI_FALSE;
        s_sensor_mode[SENSOR_MAIN]=SENSOR_MODE_MAX;
        s_sensor_mode[SENSOR_SUB]=SENSOR_MODE_MAX;
        return SENSOR_OP_STATUS_ERR;
    }

    //SENSOR_PRINT:"SENSOR: Sensor_Open: start: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1547_112_2_18_1_24_35_632,(uint8*)"d", SCI_GetTickCount());
    if(Sensor_IsOpen())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Open -> sensor has open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1550_112_2_18_1_24_35_633,(uint8*)"");
    }
    else
    {
        if(SENSOR_TYPE_IMG_SENSOR==_Sensor_GetSensorType())
        {
            cur_sensor_id=Sensor_GetCurId();

            //to handle the situation when sensorID haven't been changed by MMI app, but exactly the sensor is changed @^@
            if(SENSOR_ATV == cur_sensor_id)
            {
                //SENSOR_PRINT:"SENSOR: Sensor_Open -> sensor main manual set"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1561_112_2_18_1_24_35_634,(uint8*)"");
                cur_sensor_id = SENSOR_MAIN;
            }

            _Sensor_SetId(cur_sensor_id);

            if(!_Sensor_PowerUpHandle(cur_sensor_id))
            {
                s_sensor_open=SCI_FALSE;
                return SENSOR_OP_ERR;
            }

            //function intial
            if((PNULL!=s_sensor_info_ptr->ioctl_func_tab_ptr)
                &&(PNULL!=s_sensor_info_ptr->ioctl_func_tab_ptr->ext_func))
            {
                ext_cmd.cmd=SENSOR_EXT_FUNC_INIT;
                if(SCI_SUCCESS!=s_sensor_info_ptr->ioctl_func_tab_ptr->ext_func((uint32)&ext_cmd))
                {
                    //SENSOR_PRINT:"SENSOR: ext func init %d error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1580_112_2_18_1_24_35_635,(uint8*)"d", __LINE__);
                }
            }

            s_sensor_open=SCI_TRUE;
        }

        #if 0
        else if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
        {

            if(SCI_TRUE != sensor_register_info_ptr->is_register[SENSOR_ATV])
            {
                //handle when not initialize
                s_sensor_info_ptr = (SENSOR_INFO_T*)Sensor_GetInforTab(SENSOR_ATV)[0];
                s_sensor_list_ptr[SENSOR_ATV]=s_sensor_info_ptr;
                sensor_register_info_ptr->is_register[SENSOR_ATV]=SCI_TRUE;
            }
            else
            {
                //initialized handle
                s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_ATV];
            }

            //set the atv parameters
            Sensor_SetExportInfo(&s_sensor_exp_info);
            _Sensor_SetId(SENSOR_ATV);
            _Sensor_IicHandlerInit(SENSOR_ATV);
            Sensor_PowerOn(SCI_TRUE);

            //open atv
            if(NULL!=s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1)
            {

                param=(ATV_CMD_CHIP_INIT<<BIT_4)&SENSOR_HIGN_SIXTEEN_BIT;
                if(SCI_SUCCESS!=s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1(param))
                {
                    s_atv_open=SCI_FALSE;
                    Sensor_PowerOn(SCI_FALSE);
                    _Sensor_IicHandlerRelease();

                    //SENSOR_PRINT:"SENSOR: Sensor_Open the atv init fail %d error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1619_112_2_18_1_24_35_636,(uint8*)"d",__LINE__);
                    return SENSOR_OP_ERR;
                }
            }

            s_atv_open=SCI_TRUE;

        }
        #endif

    }

    //SENSOR_PRINT:"SENSOR: Sensor_Open end %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1629_112_2_18_1_24_35_637,(uint8*)"d", SCI_GetTickCount());

    return SENSOR_OP_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to set sensor work-mode
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_SetMode(SENSOR_MODE_E mode)
{
    uint32 mclk = 0;

    //if(SENSOR_TYPE_IMG_SENSOR==_Sensor_GetSensorType())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_SetMode -> mode = %d, time %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1643_112_2_18_1_24_35_638,(uint8*)"dd", mode, SCI_GetTickCount());

        if(s_sensor_mode[Sensor_GetCurId()] == mode)
        {
            //SENSOR_PRINT:"SENSOR: The sensor mode as before"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1647_112_2_18_1_24_35_639,(uint8*)"");
            return SENSOR_OP_SUCCESS;
        }

        if(!Sensor_IsInit())
        {
            //SENSOR_PRINT:"SENSOR: Sensor_SetResolution -> sensor has not init"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1653_112_2_18_1_24_35_640,(uint8*)"");
            return SENSOR_OP_STATUS_ERR;
        }

        if(PNULL == s_sensor_info_ptr)
        {
            //SENSOR_PRINT:"SENSOR: current sensor info is not available!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1659_112_2_18_1_24_35_641,(uint8*)"");
            return SENSOR_OP_STATUS_ERR;
        }

        if(PNULL != s_sensor_info_ptr->resolution_tab_info_ptr[mode].sensor_reg_tab_ptr)
        {
            // set mclk
            //mclk = s_sensor_info_ptr->resolution_tab_info_ptr[mode].xclk_to_sensor;
            //Sensor_SetMCLK(mclk);

            // set image format
            if(SENSOR_MODE_MAX > mode)
            {
                s_sensor_exp_info.image_format = s_sensor_exp_info.sensor_mode_info[mode].image_format;
            }
            // send register value to sensor
            Sensor_SendRegTabToSensor(&s_sensor_info_ptr->resolution_tab_info_ptr[mode]);
        }
        else
        {
            //SENSOR_PRINT:"SENSOR: Sensor_SetResolution -> No this resolution information !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1680_112_2_18_1_24_35_642,(uint8*)"");
        }
        s_sensor_mode[Sensor_GetCurId()]=mode;
    }

    return SENSOR_OP_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to control sensor
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_Ioctl(uint32 cmd, uint32 arg)
{
    SENSOR_IOCTL_FUNC_PTR func_ptr;
    SENSOR_IOCTL_FUNC_TAB_T* func_tab_ptr;
    uint32 temp;
    uint32 ret_value = SENSOR_OP_SUCCESS;

    //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> cmd = %d, arg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1699_112_2_18_1_24_35_643,(uint8*)"dd", cmd, arg);

    if(SENSOR_IOCTL_MAX<=cmd)
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> cmd %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1703_112_2_18_1_24_35_644,(uint8*)"d",cmd);
        return SENSOR_OP_ERR;
    }

    if(!Sensor_IsInit())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> sensor has not init error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1709_112_2_18_1_24_35_645,(uint8*)"");
        return SENSOR_OP_STATUS_ERR;
    }

    if(!Sensor_IsOpen())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> sensor has not open error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1715_112_2_18_1_24_35_646,(uint8*)"");
        return SENSOR_OP_STATUS_ERR;
    }

    if(SENSOR_I2C_NULL_HANDLE == s_i2c_dev_handler)
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> sensor has not initial I2C handle"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1721_112_2_18_1_24_35_647,(uint8*)"");
        return SENSOR_OP_STATUS_ERR;
    }

    if(SENSOR_IOCTL_ATV > cmd)
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl - > can't access internal command error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1727_112_2_18_1_24_35_648,(uint8*)"");
        return SENSOR_OP_SUCCESS;
    }

    func_tab_ptr = s_sensor_info_ptr->ioctl_func_tab_ptr;

    temp = *(uint32*)((uint32)func_tab_ptr + cmd * BIT_2);

    func_ptr = (SENSOR_IOCTL_FUNC_PTR)temp;

    if(PNULL!= func_ptr)
    {
        ImgSensor_GetMutex();
        ret_value = func_ptr(arg);
        ImgSensor_PutMutex();
    }
    else
    {
        //SENSOR_PRINT:"SENSOR: Sensor_Ioctl -> the ioctl function has not register err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1745_112_2_18_1_24_35_649,(uint8*)"");
    }

    return ret_value;

}

/*****************************************************************************/
//  Description:    This function is used to Get sensor information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC SENSOR_EXP_INFO_T* Sensor_GetInfo( void )
{
    if(!Sensor_IsInit())
    {
        //SENSOR_PRINT:"SENSOR: Sensor_GetInfo -> sensor has not init"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1759_112_2_18_1_24_35_650,(uint8*)"");
        return PNULL;
    }

    return &s_sensor_exp_info;
}

/*****************************************************************************/
//  Description:    This function is used to Close sensor function
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_Close(void)
{
    //SENSOR_PRINT:"SENSOR: Sensor_close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1771_112_2_18_1_24_35_651,(uint8*)"");

    if(Sensor_IsInit())
    {
        if(Sensor_IsOpen())
        {
            if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
            {
                uint32 param=0x00;
                param=(ATV_CMD_CLOSE<<BIT_4)&SENSOR_HIGN_SIXTEEN_BIT;
                s_sensor_info_ptr->ioctl_func_tab_ptr->cus_func_1(param);
                Sensor_PowerOn(SCI_FALSE);
                _Sensor_IicHandlerRelease();

            }
            else
            {
                SENSOR_REGISTER_INFO_T_PTR sensor_register_info_ptr=Sensor_GetRegisterInfo();
                SENSOR_ID_E cur_sensor_id=Sensor_GetCurId();

                Sensor_PowerOn(SCI_FALSE);

                if(SENSOR_MAIN==cur_sensor_id)
                {
                    if(SCI_TRUE==sensor_register_info_ptr->is_register[SENSOR_SUB])
                    {
                        _Sensor_IicHandlerRelease();
                        s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_SUB];
                        Sensor_SetExportInfo(&s_sensor_exp_info);
                        _Sensor_SetId(SENSOR_SUB);
                        _Sensor_IicHandlerInit(SENSOR_SUB);
                        Sensor_PowerOn(SCI_FALSE);
                        _Sensor_IicHandlerRelease();
                    }
                    _Sensor_SetId(SENSOR_MAIN);
                }
                #if 0
                else
                {
                    if(SCI_TRUE==sensor_register_info_ptr->is_register[SENSOR_MAIN])
                    {
                        _Sensor_IicHandlerRelease();
                        s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_MAIN];
                        Sensor_SetExportInfo(&s_sensor_exp_info);
                        _Sensor_SetId(SENSOR_MAIN);
                        _Sensor_IicHandlerInit(SENSOR_MAIN);
                        Sensor_PowerOn(SCI_FALSE);
                        _Sensor_IicHandlerRelease();
                    }
                    _Sensor_SetId(SENSOR_SUB);
                }

                //confirm the ATV LDO be shutted down
                if(SCI_TRUE==sensor_register_info_ptr->is_register[SENSOR_ATV])
                {
                    s_sensor_info_ptr=s_sensor_list_ptr[SENSOR_ATV];
                    _Sensor_SetId(SENSOR_ATV);
                    Sensor_PowerOn(SCI_FALSE);
                }
                #endif
            }

        }
    }

    s_atv_open=SCI_FALSE;
    s_sensor_open=SCI_FALSE;
    s_sensor_mode[SENSOR_MAIN]=SENSOR_MODE_MAX;
    s_sensor_mode[SENSOR_SUB]=SENSOR_MODE_MAX;
    s_sensor_mode[SENSOR_ATV]=SENSOR_MODE_MAX;
    return SENSOR_OP_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to Close sensor function
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Sensor_IsOpen(void)
{
    if(SENSOR_TYPE_IMG_SENSOR==_Sensor_GetSensorType())
    {
        return s_sensor_open;
    }
    else if(SENSOR_TYPE_ATV==_Sensor_GetSensorType())
    {
        return s_atv_open;
    }

    return SCI_FALSE;

}

/*****************************************************************************/
//  Description:    This function is used to set sensor type (img sensor or atv)
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_SetSensorType(SENSOR_TYPE_E sensor_type)
{
    s_sensor_type=sensor_type;

    return SENSOR_OP_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get iic handler
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_GetI2cHnadler(void)
{
    return (uint32)s_i2c_dev_handler;
}

/*****************************************************************************/
//  Description:    This function is used to set sensor info
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_SetInfo(void)
{
    Sensor_SetExportInfo(&s_sensor_exp_info);
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get sensor exif info
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 Sensor_SetSensorExifInfo(SENSOR_EXIF_CTRL_E cmd ,uint32 param)
{
    SENSOR_EXP_INFO_T_PTR sensor_info_ptr=Sensor_GetInfo();
    EXIF_SPEC_PIC_TAKING_COND_T* sensor_exif_info_ptr=PNULL;

    if(PNULL!=sensor_info_ptr->ioctl_func_ptr->get_exif)/*lint !e613*/
    {
        sensor_exif_info_ptr=(EXIF_SPEC_PIC_TAKING_COND_T*)sensor_info_ptr->ioctl_func_ptr->get_exif(0x00);/*lint !e613*/
    }
    else
    {
        //SENSOR_PRINT:"SENSOR: Sensor_SetSensorExifInfo the get_exif fun is null %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_1901_112_2_18_1_24_36_652,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    switch(cmd)
    {
        case SENSOR_EXIF_CTRL_EXPOSURETIME:
        {
            SENSOR_MODE_E img_sensor_mode=s_sensor_mode[Sensor_GetCurId()];
            uint32 exposureline_time=sensor_info_ptr->sensor_mode_info[img_sensor_mode].line_time;/*lint !e613*/
            uint32 exposureline_num=param;
            uint32 exposure_time=0x00;

            exposure_time=exposureline_time*exposureline_num;

            sensor_exif_info_ptr->valid.ExposureTime=1;

            if(0x00==exposure_time)
            {
                sensor_exif_info_ptr->valid.ExposureTime=0;
            }
            else if(1000000>=exposure_time)
            {
                sensor_exif_info_ptr->ExposureTime.numerator=0x01;
                sensor_exif_info_ptr->ExposureTime.denominator=1000000/exposure_time;
            }
            else
            {
                uint32 second=0x00;
                do
                {
                    second++;
                    exposure_time-=1000000;
                    if(1000000>=exposure_time)
                    {
                        break;
                    }
                }while(1);/*lint !e506*/

                sensor_exif_info_ptr->ExposureTime.denominator=1000000/exposure_time;
                sensor_exif_info_ptr->ExposureTime.numerator=sensor_exif_info_ptr->ExposureTime.denominator*second;
            }
            break;
        }
        case SENSOR_EXIF_CTRL_FNUMBER:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_EXPOSUREPROGRAM:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SPECTRALSENSITIVITY:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_ISOSPEEDRATINGS:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_OECF:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SHUTTERSPEEDVALUE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_APERTUREVALUE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_BRIGHTNESSVALUE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_EXPOSUREBIASVALUE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_MAXAPERTUREVALUE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SUBJECTDISTANCE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_METERINGMODE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_LIGHTSOURCE:
        {
            sensor_exif_info_ptr->valid.LightSource=1;
            switch(param)
            {
                case 0:
                {
                    sensor_exif_info_ptr->LightSource=0x00;
                    break;
                }
                case 1:
                {
                    sensor_exif_info_ptr->LightSource=0x03;
                    break;
                }
                case 2:
                {
                    sensor_exif_info_ptr->LightSource=0x0f;
                    break;
                }
                case 3:
                {
                    sensor_exif_info_ptr->LightSource=0x0e;
                    break;
                }
                case 4:
                {
                    sensor_exif_info_ptr->LightSource=0x03;
                    break;
                }
                case 5:
                {
                    sensor_exif_info_ptr->LightSource=0x01;
                    break;
                }
                case 6:
                {
                    sensor_exif_info_ptr->LightSource=0x0a;
                    break;
                }
                default :
                {
                    sensor_exif_info_ptr->LightSource=0xff;
                    break;
                }
            }
            break;
        }
        case SENSOR_EXIF_CTRL_FLASH:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FOCALLENGTH:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SUBJECTAREA:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FLASHENERGY:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SPATIALFREQUENCYRESPONSE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FOCALPLANEXRESOLUTION:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FOCALPLANEYRESOLUTION:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FOCALPLANERESOLUTIONUNIT:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SUBJECTLOCATION:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_EXPOSUREINDEX:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SENSINGMETHOD:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FILESOURCE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SCENETYPE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_CFAPATTERN:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_CUSTOMRENDERED:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_EXPOSUREMODE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_WHITEBALANCE:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_DIGITALZOOMRATIO:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_FOCALLENGTHIN35MMFILM:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SCENECAPTURETYPE:
        {
            sensor_exif_info_ptr->valid.SceneCaptureType=1;
            switch(param)
            {
                case 0:
                {
                    sensor_exif_info_ptr->SceneCaptureType=0x00;
                    break;
                }
                case 1:
                {
                    sensor_exif_info_ptr->SceneCaptureType=0x03;
                    break;
                }
                default :
                {
                    sensor_exif_info_ptr->LightSource=0xff;
                    break;
                }
            }
            break;
        }
        case SENSOR_EXIF_CTRL_GAINCONTROL:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_CONTRAST:
        {
            sensor_exif_info_ptr->valid.Contrast=1;
            switch(param)
            {
                case 0:
                case 1:
                case 2:
                {
                    sensor_exif_info_ptr->Contrast=0x01;
                    break;
                }
                case 3:
                {
                    sensor_exif_info_ptr->Contrast=0x00;
                    break;
                }
                case 4:
                case 5:
                case 6:
                {
                    sensor_exif_info_ptr->Contrast=0x02;
                    break;
                }
                default :
                {
                    sensor_exif_info_ptr->Contrast=0xff;
                    break;
                }
            }
            break;
        }
        case SENSOR_EXIF_CTRL_SATURATION:
        {
            sensor_exif_info_ptr->valid.Saturation=1;
            switch(param)
            {
                case 0:
                case 1:
                case 2:
                {
                    sensor_exif_info_ptr->Saturation=0x01;
                    break;
                }
                case 3:
                {
                    sensor_exif_info_ptr->Saturation=0x00;
                    break;
                }
                case 4:
                case 5:
                case 6:
                {
                    sensor_exif_info_ptr->Saturation=0x02;
                    break;
                }
                default :
                {
                    sensor_exif_info_ptr->Saturation=0xff;
                    break;
                }
            }
            break;
        }
        case SENSOR_EXIF_CTRL_SHARPNESS:
        {
            sensor_exif_info_ptr->valid.Sharpness=1;
            switch(param)
            {
                case 0:
                case 1:
                case 2:
                {
                    sensor_exif_info_ptr->Sharpness=0x01;
                    break;
                }
                case 3:
                {
                    sensor_exif_info_ptr->Sharpness=0x00;
                    break;
                }
                case 4:
                case 5:
                case 6:
                {
                    sensor_exif_info_ptr->Sharpness=0x02;
                    break;
                }
                default :
                {
                    sensor_exif_info_ptr->Sharpness=0xff;
                    break;
                }
            }
            break;
        }
        case SENSOR_EXIF_CTRL_DEVICESETTINGDESCRIPTION:
        {
            break;
        }
        case SENSOR_EXIF_CTRL_SUBJECTDISTANCERANGE:
        {
            break;
        }
        default :
            break;
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get sensor exif info
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC EXIF_SPEC_PIC_TAKING_COND_T* Sensor_GetSensorExifInfo(void)
{
    SENSOR_EXP_INFO_T_PTR sensor_info_ptr=Sensor_GetInfo();
    EXIF_SPEC_PIC_TAKING_COND_T* sensor_exif_info_ptr=PNULL;

    if(PNULL!=sensor_info_ptr->ioctl_func_ptr->get_exif)/*lint !e613*/
    {
        sensor_exif_info_ptr=(EXIF_SPEC_PIC_TAKING_COND_T*)sensor_info_ptr->ioctl_func_ptr->get_exif(0x00);/*lint !e613*/
    }

    return sensor_exif_info_ptr;
}
/*****************************************************************************/
//  Description:    This function is used to get sensor info
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC void Sensor_GetTable(SENSOR_ID_E sensor_id,SENSOR_INFO_T **table_ptr)
{
    if(sensor_id > SENSOR_SUB)
    {
        *table_ptr = PNULL;
    }
    else
    {
        *table_ptr = s_sensor_list_ptr[sensor_id];
    }
}

PUBLIC void Sensor_SetCallback(SENSOR_OPEN_FUNC_PTR pFunc)
{
    if(NULL != pFunc)
    {
        g_OpenCamController = pFunc;
    }
}


/*****************************************************************************/
//  Description:    This function is used for ccir pclk source
//  Author:         Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirPclkSrc(SENSOR_ID_E sensor_id)
{
    return Sensor_Get_Ccir_Pclk_Src(sensor_id);
}

/*****************************************************************************/
//  Description:    This function is used for ccir D0
//  Author:         Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirD0Src(SENSOR_ID_E sensor_id)
{
    return Sensor_Get_Ccir_D0_Src(sensor_id);
}

/*****************************************************************************/
//  Description:    This function is used for ccir vsync
//  Author:         Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirVsyncSrc(SENSOR_ID_E sensor_id)
{
    return Sensor_Get_Ccir_Vsync_Src(sensor_id);
}

/*****************************************************************************/
//  Description:    This function is used for ccir vsync
//  Author:         Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirD1Src(SENSOR_ID_E sensor_id)
{
    return Sensor_Get_Ccir_D1_Src(sensor_id);
}

PUBLIC int32 Sensor_I2cInit()
{
    int32 dev_handler=0;
    I2C_DEV  dev={0x00};

    SCI_TRACE_LOW("_Sensor_IicHandlerInit");

    if(!(SENSOR_I2C_NULL_HANDLE==s_i2c_dev_handler))
    {
        SCI_TRACE_LOW("_Sensor_IicHandlerRelease");
        _Sensor_IicHandlerRelease();
    }

    dev.id = 0;//(uint32)Sensor_GetDefaultI2cPort(0);
    dev.freq=SENSOR_I2C_FREQ;
    dev.slave_addr= 0x42;//s_sensor_info_ptr->salve_i2c_addr_w;
    dev.reg_addr_num = 1;
    dev.check_ack=1;
     dev.no_stop = 0;

    //SENSOR_PRINT:"_Sensor_IicHandlerInit, id %d freq %d bus %d slave_addr 0x%x reg_addr_num %d,ack %d no_stop %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_314_112_2_18_1_24_32_578,(uint8*)"ddddddd",dev.id,dev.freq,dev.bus,dev.slave_addr,dev.reg_addr_num,dev.check_ack,dev.no_stop);

    dev_handler=I2C_HAL_Open(&dev);

    if(dev_handler==SENSOR_I2C_NULL_HANDLE)
    {
        //SENSOR_PRINT:"SENSOR_handler creat err first"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_DRV_320_112_2_18_1_24_32_579,(uint8*)"");
    }

    s_i2c_dev_handler=dev_handler;

    return dev_handler;

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of sensor_drv.c
