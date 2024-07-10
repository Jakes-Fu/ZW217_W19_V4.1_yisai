/******************************************************************************
 ** File Name:      sensor_cfg.c                                                  *
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
#include "ms_customize_trc.h"
#include "sensor_drv.h"
#include "sensor_cfg.h"
#include "sc_reg.h"

#ifdef SPLOAD_ENABLE
#include "spload_cfg.h"
#include "spload_api.h"
#endif
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
#if defined(AUTOTEST_DEVICE_SUPPORT)

  extern SENSOR_INFO_T g_at_spi_sensor_yuv_info;

  extern SENSOR_INFO_T g_at_parallel_sensor_yuv_info;

#endif



#define SENSOR_IMAGE_FORMAT_DEFAULT			SENSOR_IMAGE_FORMAT_MAX

/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/
//extern const SENSOR_INFO_T g_OV7670_yuv_info;
//extern const SENSOR_INFO_T g_GC0307_yuv_info;

//extern SENSOR_INFO_T g_OV9655_yuv_info;
//extern SENSOR_INFO_T g_OV2640_yuv_info;
//extern SENSOR_INFO_T g_OV2655_yuv_info;
//extern SENSOR_INFO_T g_GC0306_yuv_info;
//extern SENSOR_INFO_T g_SIV100A_yuv_info;
//extern SENSOR_INFO_T g_SIV100B_yuv_info;
//extern SENSOR_INFO_T g_OV3640_yuv_info;
//extern SENSOR_INFO_T g_mt9m112_yuv_info;
//extern SENSOR_INFO_T g_OV7690_yuv_info;
//extern SENSOR_INFO_T g_OV9660_yuv_info;
//extern const SENSOR_INFO_T g_OV7676_yuv_info;

//extern const SENSOR_INFO_T g_BF3903_yuv_info;
////////extern const SENSOR_INFO_T g_bf30a2_yuv_info;
//extern const SENSOR_INFO_T g_GC6113_yuv_info;
//extern const SENSOR_INFO_T g_GC2015_yuv_info;
//extern const SENSOR_INFO_T g_OV2659_yuv_info;
//extern const SENSOR_INFO_T g_GC0310_yuv_info;
////////extern const SENSOR_INFO_T g_GC032A_yuv_info;
extern const SENSOR_INFO_T g_gc6133_yuv_info;
//extern const SENSOR_INFO_T g_GC2145_yuv_info;
//extern const SENSOR_INFO_T g_GC0309_yuv_info;


/**---------------------------------------------------------------------------*
 **                         analog tv                                         *
 **---------------------------------------------------------------------------*/
//extern SENSOR_INFO_T g_tlg1120_yuv_info;
//extern SENSOR_INFO_T  g_qn8610_yuv_info;
#if defined(ATV_SUPPORT_NMI601)	
    extern SENSOR_INFO_T g_nmi600_yuv_info;
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL SENSOR_IMAGE_FORMAT s_sensor_image_format = SENSOR_IMAGE_FORMAT_DEFAULT;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL const SENSOR_INFO_T* main_sensor_infor_tab[]=
{  

#if defined(AUTOTEST_DEVICE_SUPPORT)

//  &g_at_parallel_sensor_yuv_info,
//  &g_at_spi_sensor_yuv_info,
#endif
    //&g_OV2640_yuv_info,
    //&g_OV2655_yuv_info,
    //&g_OV9655_yuv_info,
    //&g_OV7670_yuv_info,
    //&g_GC0307_yuv_info,
    //&g_SIV100A_yuv_info,
    //&g_SIV100B_yuv_info,
    //&g_OV3640_yuv_info,
    //&g_mt9m112_yuv_info,
    //&g_GC0306_yuv_info,
    //&g_OV9660_yuv_info,
    //&g_BF3903_yuv_info,
    ////&g_bf30a2_yuv_info,
    //&g_GC0310_yuv_info,
    ////&g_GC032A_yuv_info,
    &g_gc6133_yuv_info,
    //&g_GC2145_yuv_info,
    //&g_OV7676_yuv_info,
    //&g_OV2659_yuv_info,

    PNULL
};

LOCAL const SENSOR_INFO_T* sub_sensor_infor_tab[]=
{
#if defined(AUTOTEST_DEVICE_SUPPORT)

  &g_at_parallel_sensor_yuv_info,
  &g_at_spi_sensor_yuv_info,

#endif
    //&g_OV7670_yuv_info,
    //&g_SIV100A_yuv_info,
    //&g_SIV100B_yuv_info,
    //&g_GC0306_yuv_info,
    //&g_GC0310_yuv_info,
    //&g_GC6113_yuv_info,
    PNULL
};

LOCAL const SENSOR_INFO_T* atv_infor_tab[]=
{
#if defined(ATV_SUPPORT_NMI601)	
    //&g_nmi600_yuv_info,
    //&g_qn8610_yuv_info,
    //&g_tlg1120_yuv_info,
#endif
    PNULL
};

LOCAL const CCIR_SRC_CFG_T s_ccir_src_cfg_info=
{
    CCIR_PCLK_SOURCE_CCIR,
    CCIR_PCLK_SOURCE_CCIR,
    CCIR_PCLK_SOURCE_ATV,
    
    CCIR_D0_SOURCE_CCIRD0,
    CCIR_D0_SOURCE_CCIRD0,
    CCIR_D0_SOURCE_CCIRD0,

    CCIR_VSYNC_SOURCE_CCIRVS,
    CCIR_VSYNC_SOURCE_CCIRVS,
    CCIR_VSYNC_SOURCE_CCIRVS,

    CCIR_D1_SOURCE_SDI1_1,
    CCIR_D1_SOURCE_SDI1_1,
    CCIR_D1_SOURCE_SDI1_1
};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to select sensor data format    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT image_format)
{
    SCI_ASSERT(image_format <= SENSOR_IMAGE_FORMAT_MAX);/*assert verified*/

#if defined(SPLOAD_ENABLE)
     s_lcm_ext_func_table->_sensor_selectsensorformat(image_format);
#else


    if( SENSOR_IMAGE_FORMAT_MAX != image_format)
    {
        s_sensor_image_format = image_format;   
    }
    else
    {
        s_sensor_image_format = SENSOR_IMAGE_FORMAT_DEFAULT;
    }
#endif
    
    //SCI_TRACE_LOW:"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_CFG_144_112_2_18_0_26_36_92,(uint8*)"dd", image_format, s_sensor_image_format);
}

/*****************************************************************************/
//  Description:    This function is used to get sensor sensor data format    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_IMAGE_FORMAT Sensor_GetSensorFormat(void)
{
#if defined(SPLOAD_ENABLE)
    return  s_lcm_ext_func_table->_sensor_getsensorformat();
#else
    return s_sensor_image_format;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_INFO_T ** Sensor_GetInforTab(SENSOR_ID_E sensor_id)
{
    SENSOR_INFO_T * sensor_infor_tab_ptr=NULL;
#ifdef SPLOAD_ENABLE
    /*because atc driver is much bigger, so keep place with binary*/
    if(SENSOR_ATV == sensor_id )
    {
        sensor_infor_tab_ptr=(SENSOR_INFO_T*)&atv_infor_tab;
    }
    else
    {
        sensor_infor_tab_ptr = s_lcm_ext_func_table->sensor_getinfortab(sensor_id);
    }
#else
    switch(sensor_id)
    {
        case SENSOR_MAIN:
        {
            sensor_infor_tab_ptr=(SENSOR_INFO_T*)&main_sensor_infor_tab;
            break;
        }
        case SENSOR_SUB:
        {
            sensor_infor_tab_ptr=(SENSOR_INFO_T*)&sub_sensor_infor_tab;
            break;
        }
        case SENSOR_ATV:
        {
            sensor_infor_tab_ptr=(SENSOR_INFO_T*)&atv_infor_tab;
            break;
        }
        default:
            break;
    }
#endif    
    return (SENSOR_INFO_T **)sensor_infor_tab_ptr;
}

/*****************************************************************************/
//  Description:    This function is used to get sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_GetInforTabLenght(SENSOR_ID_E sensor_id)
{
    uint32 tab_lenght = 0;
#ifdef SPLOAD_ENABLE
    /*because atc driver is much bigger, so keep place with binary*/
    if(SENSOR_ATV == sensor_id )
    {
        tab_lenght=(sizeof(atv_infor_tab)/sizeof(SENSOR_INFO_T*));
    }
    else
    {
        tab_lenght = s_lcm_ext_func_table->_sensor_getInfortablenght(sensor_id);
    }
#else
    switch(sensor_id)
    {
        case SENSOR_MAIN:
        {
            tab_lenght=(sizeof(main_sensor_infor_tab)/sizeof(SENSOR_INFO_T*));
            break;
        }
        case SENSOR_SUB:
        {
            tab_lenght=(sizeof(sub_sensor_infor_tab)/sizeof(SENSOR_INFO_T*));
            break;
        }
        case SENSOR_ATV:
        {
            tab_lenght=(sizeof(atv_infor_tab)/sizeof(SENSOR_INFO_T*));
            break;
        }
        default:
            break;
    }
#endif    
    return tab_lenght;
}

/*****************************************************************************/
//  Description:    This function is used for ccir pclk source   
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_Pclk_Src(SENSOR_ID_E sensor_id)
{
    CCIR_PCLK_SOURCE_E  ccir_pclk_src = CCIR_PCLK_SOURCE_CCIR;
    
    switch(sensor_id)
    {
        case SENSOR_MAIN:
             ccir_pclk_src = s_ccir_src_cfg_info.main_sensor_pclk_src;
             break;
             
        case SENSOR_SUB:
             ccir_pclk_src = s_ccir_src_cfg_info.sub_sensor_pclk_src;
             break;
             
        case SENSOR_ATV:
             ccir_pclk_src = s_ccir_src_cfg_info.atv_pclk_src;
             break;
             
        default:
             break;
    }

    return (uint8)ccir_pclk_src;
}

/*****************************************************************************/
//  Description:    This function is used for ccir d0  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_D0_Src(SENSOR_ID_E sensor_id)
{
    CCIR_D0_SOURCE_E  ccir_d0_src = CCIR_D0_SOURCE_CCIRD0;
    
    switch(sensor_id)
    {
        case SENSOR_MAIN:
             ccir_d0_src = s_ccir_src_cfg_info.main_sensor_d0_src;
             break;
             
        case SENSOR_SUB:
             ccir_d0_src = s_ccir_src_cfg_info.sub_sensor_d0_src;
             break;
             
        case SENSOR_ATV:
             ccir_d0_src = s_ccir_src_cfg_info.atv_d0_src;
             break;
             
        default:
             break;
    }

    return (uint8)ccir_d0_src;
}

/*****************************************************************************/
//  Description:    This function is used for ccir Vsync  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_Vsync_Src(SENSOR_ID_E sensor_id)
{
    CCIR_VSYNC_SOURCE_E  ccir_vsync_src = CCIR_VSYNC_SOURCE_CCIRVS;
    
    switch(sensor_id)
    {
        case SENSOR_MAIN:
             ccir_vsync_src = s_ccir_src_cfg_info.main_sensor_vsync_src;
             break;
             
        case SENSOR_SUB:
             ccir_vsync_src = s_ccir_src_cfg_info.sub_sensor_vsync_src;
             break;
             
        case SENSOR_ATV:
             ccir_vsync_src = s_ccir_src_cfg_info.atv_vsync_src;
             break;
             
        default:
             break;
    }

    return (uint8)ccir_vsync_src;
}

/*****************************************************************************/
//  Description:    This function is used for ccir D1  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_D1_Src(SENSOR_ID_E sensor_id)
{
    CCIR_D1_SOURCE_E  ccir_d1_src = CCIR_D1_SOURCE_CCIRD1;
    
    switch(sensor_id)
    {
        case SENSOR_MAIN:
             ccir_d1_src = s_ccir_src_cfg_info.main_sensor_d1_src;
             break;
             
        case SENSOR_SUB:
             ccir_d1_src = s_ccir_src_cfg_info.sub_sensor_d1_src;
             break;
             
        case SENSOR_ATV:
             ccir_d1_src = s_ccir_src_cfg_info.atv_d1_src;
             break;
             
        default:
             break;
    }

    return (uint8)ccir_d1_src;
}

/*****************************************************************************/
//  Description:    This function is used to get default i2c port  
//  Author:         X.Z
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_I2C_PORT_E Sensor_GetDefaultI2cPort(SENSOR_ID_E sensor_id)
{
    SENSOR_I2C_PORT_E rtn_port = SENSOR_I2C_PORT_ZERO;

    switch(sensor_id)
    {
        case SENSOR_MAIN:
            rtn_port = SENSOR_I2C_PORT_ZERO;
        break;
        
        case SENSOR_SUB:
            rtn_port = SENSOR_I2C_PORT_ZERO;
        break;

        case SENSOR_ATV:
            rtn_port = SENSOR_I2C_PORT_ZERO;
        break;

        default:
        break;
    }        

    return rtn_port;
}

/*****************************************************************************/
//  Description:    This function is used to set mclk  
//  Author:         Eddy.Wei
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_MclkConfig(uint32 clk_src, uint32 clk_div)
{
	return;
#if 0	
    clk_src = (clk_src & 3);
    CHIP_REG_AND(CGM_SENSOR_CFG, ~CGM_SENSOR_SEL_MSK);
    CHIP_REG_OR(CGM_SENSOR_CFG, clk_src<<CGM_SENSOR_SEL_SHIFT);

    clk_div = (clk_div & 3);
    CHIP_REG_AND(CGM_SENSOR_CFG, ~CGM_SENSOR_DIV_MSK);
    CHIP_REG_OR(CGM_SENSOR_CFG, clk_div<<CGM_SENSOR_DIV_SHIFT);
	
    *(volatile uint32*) APB_CLK_CFG0 |= BIT_12; //enable MCLK
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of sensor_drv.c


