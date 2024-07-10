/******************************************************************************
 ** File Name:      sc6531efm_audio_cfg.c                                        *
 ** Author:         huijie.feng                                                 *
 ** DATE:           12/01/2015                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Configurations of audio device                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/01/2015     huijie.feng         Create.                                  *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "sci_api.h"
#include "audio_drvapi.h"
#include "audio_dai.h"
#include "audio_codec.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         GLOBAL defination                                 *
 **---------------------------------------------------------------------------*/
//extern AUD_DAI_OPS_T* AUDIO_DAI_Get_VBC_OPS(uint32 param);
extern AUD_DAI_OPS_T* AUDIO_DAI_Get_CUSTOM_OPS(uint32 param);


#if defined (FPGA_VERIFICATION)
#if defined (AUDIO_CODEC_LM49370)
extern AUD_CODEC_OPS_T* AUDIO_CODEC_Get_LM49370_OPS(uint32 param);
#else
extern AUD_CODEC_OPS_T* AUDIO_CODEC_Get_USC15241_OPS(uint32 param);
extern void __usc15241_run_callback(uint32 is_open);
#endif
#endif
//extern void __sprd_codec_run_callback(uint32 is_open);
//extern AUD_CODEC_OPS_T* AUDIO_CODEC_Get_SPRD_CODEC_OPS(uint32 param);
extern AUD_CODEC_OPS_T* AUDIO_CODEC_Get_CUSTOM_OPS(uint32 param);

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
#if 0
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_OPS(uint32 param)
{
    AUD_DAI_OPS_T* ret_ops;
    if( 0== (ret_ops = AUDIO_DAI_Get_CUSTOM_OPS(param)))
    {
        ret_ops = AUDIO_DAI_Get_VBC_OPS(param);
    }
    return ret_ops;
}


PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_OPS(uint32 param)
{
    AUD_CODEC_OPS_T* ret_ops;
    if( 0== (ret_ops = AUDIO_CODEC_Get_CUSTOM_OPS(param)))
    {
#if 0 // defined (FPGA_VERIFICATION)
#if defined (AUDIO_CODEC_LM49370)
        ret_ops = AUDIO_CODEC_Get_LM49370_OPS(param);
#else
        ret_ops = AUDIO_CODEC_Get_USC15241_OPS(param);
#endif
#else
        ret_ops = AUDIO_CODEC_Get_SPRD_CODEC_OPS(param);
#endif
    }
    return ret_ops;
}

PUBLIC void __run_callback(uint32 is_open)
{
#if 0 // defined (FPGA_VERIFICATION)
#if defined (AUDIO_CODEC_LM49370)
#else
    __usc15241_run_callback(is_open);
#endif
#else
    __sprd_codec_run_callback(is_open);
#endif
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
