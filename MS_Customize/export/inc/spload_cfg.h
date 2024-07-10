/******************************************************************************
 ** File Name:      spload_cfg.h                                                    *
 ** Author:           tao.feng                                               *
 ** DATE:            09/15/2010                                              *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                            *
 ** 09/15/2010     tao.feng        Create.                                   *
 ******************************************************************************/
#ifndef _LCM_COMMON_CFG_H_
#define _LCM_COMMON_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "lcm_cfg.h"
#include "sensor_drv.h"

/**---------------------------------------------------------------------------*
 **                         Macro definition                                   *
 **---------------------------------------------------------------------------*/
/*define lcm bin load base addr*/
/*Important Notice: please don't change macro name.perl script will use this name to auto generate spload_scatter.scf,
for same reason, don't change the macro value:(example:0xe08000) to macro define,you can only change it to number expression*/
#define  LCM_SENSOR_LOAD_BASE_ADDR			0x00FE0000


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct
{
     LCM_CFG_INFO_T_PTR (*_lcm_getcfginfotable)(uint32 lcd_id, uint32* count);
     void (*_lcm_sensor_copyrw)(void);
     void (*_lcm_sensor_initzi)(void);
     void (*_lcm_sensor_init)(void);
     void (*_sensor_selectsensorformat)(SENSOR_IMAGE_FORMAT image_format);
     SENSOR_IMAGE_FORMAT (*_sensor_getsensorformat)(void);
     SENSOR_INFO_T **(*sensor_getinfortab)(SENSOR_ID_E sensor_id); 
     uint32 (*_sensor_getInfortablenght)(SENSOR_ID_E sensor_id);
}LCM_SENSOR_EXT_FUN_TABLE_S;

typedef struct
{
    int32 (*lcd_sendcmd)(uint32 cmd,uint32 lcd_id);
    int32 (*lcd_senddata)(uint32 lcd_data,uint32 lcd_id);
    int32 (*lcd_sendcmdData)(uint32 cmd,uint32 lcd_data,uint32 lcd_id);
    void  (*lcd_reset)(void);
    void  (*lcd_delayms) (uint32 ms);
    void  (*lcd_wait)(uint32 ms);
    uint32 (*lcd_read)(uint32 lcd_id,uint32 cd_value);

    void  (*trace)(const char *format, ...);
    void  (*assert)(void*exp, void    *file, uint32  line);
    void  (*Assert)(int exp);
    void  (*sci_sleep)(uint32 millisecs);

    uint32 (*i2c_halwrite)(uint32 handle, uint8 *reg_addr, uint8 *buffer, uint32 bytes);
    uint32 (*i2c_halread)(uint32 handle, uint8 *reg_addr, uint8 *buffer, uint32 bytes);

    uint16 (*sensor_readReg)(uint16 subaddr);
    void (*sensor_writereg)( uint16  subaddr, uint16 data );
    void  (*sensor_setmclk)(uint32 mclk);
    void  (*sensor_setvoltage)(SENSOR_AVDD_VAL_E dvdd_val, SENSOR_AVDD_VAL_E avdd_val, SENSOR_AVDD_VAL_E iodd_val);
    BOOLEAN (*sensor_setresetlevel)(BOOLEAN plus_level);
    uint32 (*sensor_setSensorExifInfo)(SENSOR_EXIF_CTRL_E cmd ,uint32 param);
    uint32 (*sensor_setCurId)(SENSOR_ID_E sensor_id);
    SENSOR_ID_E (*sensor_getCurId)(void);
    ERR_SENSOR_E (*sensor_setMode)(SENSOR_MODE_E mode);
    uint32 (*sensor_geti2chandler)(void);
    uint32 (*sensor_setInfo)(void);

    void (*gpio_setsensorpwdn) (BOOLEAN b_on);
    void (*gpio_setfrontsensorpwdn) (BOOLEAN b_on);

    uint32 (*chip_detectmemtype)(void);
}LCD_SENSOR_EXT_FUN_TABLE_S;


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif // _LCD_H
