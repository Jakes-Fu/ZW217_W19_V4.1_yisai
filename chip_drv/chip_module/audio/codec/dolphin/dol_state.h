/******************************************************************************
 ** File Name:      dol_state.h                                               *
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
#ifndef _DOL_STATE_H_
#define _DOL_STATE_H_

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
    DOL_FUN_PLAYBACK        = 0x00000001,
    DOL_FUN_CAPTURE         = 0x00000002,
    DOL_FUN_LINEIN          = 0x00000004,
    DOL_FUN_LINEIN_REC      = 0x00000008,
    DOL_FUN_HP              = 0x00000010,
    DOL_FUN_LOUT            = 0x00000020,
    DOL_FUN_DAC_UNMUTE      = 0x00000040,
};


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void     DOL_SM_Process(void);
PUBLIC void     DOL_ChangeSt(uint32 func_flag,uint32 is_close);
PUBLIC BOOLEAN  DOL_IsReady(void);
PUBLIC BOOLEAN  DOL_IsRuning(void);
PUBLIC BOOLEAN  DOL_IsOnStop(void);
PUBLIC BOOLEAN  DOL_IsPwrOn(void);
PUBLIC void     __dol_create_task(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_DOL_STATE_H_


