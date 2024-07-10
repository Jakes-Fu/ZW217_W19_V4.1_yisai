/******************************************************************************
 ** File Name:      es8374.h                                           *
 ** Author:         jen.li                                                    *
 ** DATE:           6/15/2022                                                 *
 ** Copyright:      Copyright (c) 2022 Unisoc Communications Inc.             *
 ** Description:    Intelligent Flow Controller                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME           DESCRIPTION                                 *
 ** 6/24/2022      rui.huang      Create.                                     *
 *****************************************************************************/
#ifndef _AUD_ES83xx_H_
#define _AUD_ES83xx_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "sci_types.h"
#include "audio_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
// direction flag

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 __es83xx_support_device(void);
PUBLIC void __es83xx_modify_device(uint32 *device, uint32 direct);
PUBLIC uint32 __es83xx_fs_check(uint32 sampleRate);

PUBLIC int32 __es83xx_i2c_init();
PUBLIC void __es83xx_i2c_deinit();
PUBLIC uint32 __es83xx_cfg_init(uint32 sampleRate, uint32 mode, uint8 isSlave);
PUBLIC void __es83xx_cfg_fs(uint32 sampleRate);

PUBLIC void __es83xx_cfg_swicth(uint32 mode, uint8 isSlave);
PUBLIC void __es83xx_cfg_close();
PUBLIC void __es83xx_cfg_output(uint16 hpFlag, uint16 dac, uint16 mixgain);
PUBLIC void __es83xx_cfg_input(uint16 hpFlag, uint16 adc, uint16 pga);
PUBLIC void __es83xx_cfg_output_mute(uint16 muteFlag);
PUBLIC void __es83xx_cfg_input_mute(uint16 muteFlag);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_AUD_IFC_PHY_H_
