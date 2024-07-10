/******************************************************************************
 ** File Name:      audio_dai.h                                               *
 ** Author:         rui.huang                                                 *
 ** DATE:           06/28/2022                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2022     rui.huang        Create.                                   *
 ******************************************************************************/

#ifndef _AUDIO_DAI_H_
#define _AUDIO_DAI_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
    
#include "sci_types.h"
#include "audio_drvapi.h"


/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct
{
    uint32 ptsinM;   //0x8c
    uint32 ptsinL;   //0x90
    uint32 ptcosM;   //0x94
    uint32 ptcosL;   //0x98
    uint32 ptgl;     //0x9c
    uint32 ptgr;     //0xa0
    uint32 gd;       //0xa4
    uint32 gu;       //0xa8
    uint32 holdtime; //0xac
} NothPara;

typedef struct
{
    int32 (*dai_init)               (uint32);
    int32 (*dai_open)               (uint32 ,uint32);
    int32 (*dai_close)              (uint32);
    int32 (*dai_ioctl)              (uint32 ,uint32);
    int32 (*dai_dac_set_buf_info)   (void *,void *,uint32);
    int32 (*dai_dac_set_write_point)(uint32);
    int32 (*dai_dac_get_read_point) (uint32*);
    int32 (*dai_dac_trigger)        (void);
    int32 (*dai_adc_set_buf_info)   (void *,void *,uint32);
    int32 (*dai_adc_set_read_point) (uint32);
    int32 (*dai_adc_get_write_point)(uint32*);
    int32 (*dai_adc_trigger)        (void);
    int32 (*dai_register_playback) (AUDIO_HAL_CALLBACK_FUN);
    int32 (*dai_register_capture) (AUDIO_HAL_CALLBACK_FUN);
} AUD_DAI_OPS_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_DAI_OPS_T *AUDIO_DAI_Get_OPS(uint32 param);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_DAI_H_
// End
