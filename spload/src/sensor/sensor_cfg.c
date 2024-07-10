//##!(important notice: do not remove this line)

/******************************************************************************
 ** File Name:      sensor_cfg.c                                              *
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
#include "sensor_drv.h"
#include "spload_main.h"
#include "sensor_cfg.h"

#undef __MODULE__ 
#define __MODULE__ "sensor_cfg.c"
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
#define SENSOR_IMAGE_FORMAT_DEFAULT			SENSOR_IMAGE_FORMAT_MAX

/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/
extern const SENSOR_INFO_T g_OV7670_yuv_info;
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
//extern SENSOR_INFO_T g_OV7675_yuv_info;


/**---------------------------------------------------------------------------*
 **                         analog tv                                         *
 **---------------------------------------------------------------------------*/
#ifdef ATV_SUPPORT	
extern SENSOR_INFO_T g_tlg1120_yuv_info;
//extern SENSOR_INFO_T  g_qn8610_yuv_info;
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
    //&g_OV2640_yuv_info,
    //&g_OV2655_yuv_info,
    //&g_OV9655_yuv_info,
    &g_OV7670_yuv_info,
    //&g_SIV100A_yuv_info,
    //&g_SIV100B_yuv_info,
    //&g_OV3640_yuv_info,
    //&g_mt9m112_yuv_info,
    //&g_GC0306_yuv_info,
    //&g_OV9660_yuv_info,
    PNULL
};

LOCAL const SENSOR_INFO_T* sub_sensor_infor_tab[]=
{
    //&g_OV7670_yuv_info,
    //&g_SIV100A_yuv_info,
    //&g_SIV100B_yuv_info,
    //&g_GC0306_yuv_info,
    PNULL
};

LOCAL const SENSOR_INFO_T* atv_infor_tab[]=
{
#ifdef ATV_SUPPORT	
//    &g_tlg1120_yuv_info,
#endif    
    PNULL
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
    s_lcm_func_table->Assert(image_format <= SENSOR_IMAGE_FORMAT_MAX);
    if( SENSOR_IMAGE_FORMAT_MAX != image_format)
    {
        s_sensor_image_format = image_format;   
    }
    else
    {
        s_sensor_image_format = SENSOR_IMAGE_FORMAT_DEFAULT;
    }
    
    s_lcm_func_table->trace("Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d", image_format, s_sensor_image_format);
}

/*****************************************************************************/
//  Description:    This function is used to get sensor sensor data format    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_IMAGE_FORMAT Sensor_GetSensorFormat(void)
{
    return s_sensor_image_format;
}

/*****************************************************************************/
//  Description:    This function is used to get sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_INFO_T ** Sensor_GetInforTab(SENSOR_ID_E sensor_id)
{
    SENSOR_INFO_T * sensor_infor_tab_ptr=NULL;

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

    return tab_lenght;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of sensor_cfg.c

