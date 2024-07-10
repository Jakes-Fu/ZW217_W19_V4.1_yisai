/******************************************************************************
 ** File Name:      fm_cfg.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           04/19/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of FM*
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
#include "os_api.h"
#include "sci_types.h"
#include "i2c_api.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
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

/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/
#ifdef MOTION_SENSOR_TYPE_ST_LIS35DE
extern MSENSOR_CFG_T g_lis35DE_msensor_info;
#endif

#ifdef MOTION_SENSOR_TYPE_QST_QMA7981
extern MSENSOR_CFG_T g_qmax981_msensor_info;
#endif

#ifdef MOTION_SENSOR_TYPE_MIRAMEMS_DA215S
extern MSENSOR_CFG_T g_d217_msensor_info;
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 // Customer need to add FM probe into below table
LOCAL const MSENSOR_CFG_T_PRT s_msensor_cfg_tab[]=
{
#ifdef MOTION_SENSOR_TYPE_ST_LIS35DE
	&g_lis35DE_msensor_info,
#endif

#ifdef MOTION_SENSOR_TYPE_QST_QMA7981
	&g_qmax981_msensor_info,
#endif

#ifdef MOTION_SENSOR_TYPE_MIRAMEMS_DA215S
	&g_d217_msensor_info,
#endif
	PNULL					// END
};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get motion sensor information table
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC MSENSOR_CFG_T_PRT * MSensor_GetCfgTab(void)
{
	return (MSENSOR_CFG_T_PRT *)s_msensor_cfg_tab;
}

/*****************************************************************************/
//  Description:    This function is used to get motion sensor information table length
//  Author:         Tim.Zhu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MSensor_GetCfgTabLength(void)
{
	return ((sizeof(s_msensor_cfg_tab)/sizeof(MSENSOR_CFG_T_PRT*))-0x01);
}
/*****************************************************************************/
//  Description:  get useful configuration
//  Note:      msensor_ptr: in;    msensor_misc_cfg_ptr: out
/*****************************************************************************/
PUBLIC void MSensor_GetMiscCfg(
    MSENSOR_CFG_T_PRT       msensor_ptr,
    MSENSOR_MISC_CFG_T_PRT  msensor_misc_cfg_ptr)
{
    SCI_ASSERT(msensor_ptr != SCI_NULL);/*assert verified*/
    SCI_ASSERT(msensor_misc_cfg_ptr != SCI_NULL);/*assert verified*/

    switch(msensor_ptr->type)
    {
        case MSENSOR_TYPE_ACCELEROMETER:

          //#ifdef MOTION_SENSOR_TYPE_ST_LIS35DE
            if (strnicmp(msensor_ptr->name, "st lis35de", strlen("st lis35de")) == 0)
            {
                // sp6801h use acc: lis53de as follow
                msensor_misc_cfg_ptr->i2c_cfg.id             = 1;
                msensor_misc_cfg_ptr->i2c_cfg.freq           = 100000; // 100K
                msensor_misc_cfg_ptr->i2c_cfg.bus            = 0;
                msensor_misc_cfg_ptr->i2c_cfg.slave_addr     = 0x38;   // 8-bit write addr
                msensor_misc_cfg_ptr->i2c_cfg.reg_addr_num   = 1;
                msensor_misc_cfg_ptr->i2c_cfg.check_ack      = 1;
                msensor_misc_cfg_ptr->i2c_cfg.no_stop        = 1;

                msensor_misc_cfg_ptr->lcd_face   = MSENSOR_LCD_FACE_REVERSE;
                msensor_misc_cfg_ptr->pin1       = MSENSOR_PIN1_LEFT_DONW;
            }
          //#endif
          //#ifdef MOTION_SENSOR_TYPE_QST_QMA7981
            else if (strnicmp(msensor_ptr->name, "QMA7981", strlen("QMA7981")) == 0)
            {
                // w217 use i2c2: qma7981 as follow
                msensor_misc_cfg_ptr->i2c_cfg.id             = 3;      //I2C-M2
                msensor_misc_cfg_ptr->i2c_cfg.freq           = 100000; //100K
                msensor_misc_cfg_ptr->i2c_cfg.bus            = 0;
                msensor_misc_cfg_ptr->i2c_cfg.slave_addr     = 0x24;   //8-bit write addr(7-bit addr 0010010)
                msensor_misc_cfg_ptr->i2c_cfg.reg_addr_num   = 1;
                msensor_misc_cfg_ptr->i2c_cfg.check_ack      = 1;
                msensor_misc_cfg_ptr->i2c_cfg.no_stop        = 1;

                msensor_misc_cfg_ptr->lcd_face   = MSENSOR_LCD_FACE_SAME;
                msensor_misc_cfg_ptr->pin1       = MSENSOR_PIN1_LEFT_DONW;
            }
          //#endif
          //#ifdef MOTION_SENSOR_TYPE_MIRAMEMS_DA215S
            else if (strnicmp(msensor_ptr->name, "DA215S", strlen("DA215S")) == 0)
            {
                // w217 use i2c2: DA215S as follow
                msensor_misc_cfg_ptr->i2c_cfg.id             = 3;      //I2C-M2
                msensor_misc_cfg_ptr->i2c_cfg.freq           = 100000; //100K
                msensor_misc_cfg_ptr->i2c_cfg.bus            = 0;
                msensor_misc_cfg_ptr->i2c_cfg.slave_addr     = 0x4c;   //8-bit write addr(7-bit addr 0100110)
                msensor_misc_cfg_ptr->i2c_cfg.reg_addr_num   = 1;
                msensor_misc_cfg_ptr->i2c_cfg.check_ack      = 1;
                msensor_misc_cfg_ptr->i2c_cfg.no_stop        = 1;

                msensor_misc_cfg_ptr->lcd_face   = MSENSOR_LCD_FACE_SAME;
                msensor_misc_cfg_ptr->pin1       = MSENSOR_PIN1_LEFT_DONW;
            }
          //#endif
            break;

        case MSENSOR_TYPE_MAGNETIC:
            break;

        case MSENSOR_TYPE_OPTICAL:
            break;

        default:
            break;
    }
    return ;
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of sensor_drv.c
