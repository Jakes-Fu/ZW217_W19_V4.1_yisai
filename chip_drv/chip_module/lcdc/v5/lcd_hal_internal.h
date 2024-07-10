/******************************************************************************
 ** File Name:       lcd.h                                                    *
 ** Author:          Jim zhang                                                *
 ** DATE:            08/21/2003                                               *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ** 08/21/2003     Jim zhang        Create.                                   *
 ******************************************************************************/


#ifndef _LCD_IF_HAL_H
#define _LCD_IF_HAL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "arm_reg.h"
#include "dal_lcd.h"
#include "lcm_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structure                                    *
 **---------------------------------------------------------------------------*/


//-- the abstract structure of lcm_controller
typedef struct _lcm_controller
{
    void (*_init) (uint32 lcd_id);
    void (*_reset)(uint32 delay_ms,uint8 rst_level);
    //uint16 ERR_LCD_E BOOLEAN
    ERR_LCD_E (*_setparam) (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk,uint16 lcd_id);
    void      (*_setaddr ) (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);
    ERR_LCD_E (*_senddata) (uint32 data,uint32 lcd_id);
    ERR_LCD_E (*_sendcmd ) (uint32 cmd,uint32 lcd_id);

#if 0
    uint16   (*_dmawrite) (
          						uint32  lcd_id,
          						LCD_BUS_WIDTH_E bus_width,
                                uint32 src_addr,
                                uint16 data_width,
                                uint16 data_height,
                                uint16 buf_width,
                                LCD_DIRECT_E dir_type,                                
                                LCD_BRUSH_MODE_E brush_mode
                                );
#else
    uint32   (*_dmawrite) (
                                LCD_BUS_WIDTH_E bus_width,
                                uint32 src_addr,
                                uint32 des_addr,
                                uint32 data_width,
                                uint32 data_height,
                                uint32 buf_width,
                                
                                LCD_DIRECT_E dir_type,                                
                                LCD_BRUSH_MODE_E brush_mode
                                );
#endif
    uint32    (*_readid) (uint32 lcd_id, uint32 cd_level);
}LCM_CONTROLLER_T, *LCM_CONTROLLER_PTR;

/**---------------------------------------------------------------------------*
 **                         Varibale Declear                                  *
 **---------------------------------------------------------------------------*/

#define CS_NUMBER 2
extern LCM_ADDR_T  g_lcm_addr[CS_NUMBER];
extern LCMCallBackFunc s_lcm_callbackfunc ;

extern const PUBLIC LCM_CONTROLLER_T  s_if_lcm ;
extern const PUBLIC LCM_CONTROLLER_T  s_if_spi ;
extern const PUBLIC LCM_CONTROLLER_T  s_if_emc ;  
/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _LCD_IF_HAL_H
