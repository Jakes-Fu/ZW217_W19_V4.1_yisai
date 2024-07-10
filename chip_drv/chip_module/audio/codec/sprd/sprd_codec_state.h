/******************************************************************************
 ** File Name:      sprd_codec_state.h                                        *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/
#ifndef _SPRD_CODEC_STATE_H_
#define _SPRD_CODEC_STATE_H_

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
enum{
    SPRD_CODEC_FUN_PLAYBACK        = 0x00000001,
    SPRD_CODEC_FUN_CAPTURE         = 0x00000002,
    SPRD_CODEC_FUN_LINEIN          = 0x00000004,
    SPRD_CODEC_FUN_LINEIN_REC      = 0x00000008,
    SPRD_CODEC_FUN_HP              = 0x00000010,
    SPRD_CODEC_FUN_LOUT            = 0x00000020,
    SPRD_CODEC_FUN_EP              = 0x00000040,
    SPRD_CODEC_FUN_DAC_UNMUTE      = 0x00000080,
    SPRD_CODEC_FUN_LININ_DIGTAL    = 0x00000100, 
};


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void     SPRD_CODEC_SM_Process(void);
PUBLIC void     SPRD_CODEC_ChangeSt(uint32 func_flag,uint32 is_close);
PUBLIC BOOLEAN  SPRD_CODEC_IsReady(void);
PUBLIC BOOLEAN  SPRD_CODEC_IsRuning(void);
PUBLIC BOOLEAN  SPRD_CODEC_IsOnStop(void);
PUBLIC BOOLEAN  SPRD_CODEC_IsPwrOn(void);
PUBLIC void     SPRD_CODEC_Change_Chan(uint32 fun_flg,uint32 is_close);
PUBLIC void     __sprd_codec_create_task(void);
PUBLIC uint32   AUDIO_PA_Ctl(uint32 id,uint32 is_open);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_SPRD_CODEC_STATE_H_


