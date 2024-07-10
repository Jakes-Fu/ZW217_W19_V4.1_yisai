/******************************************************************************
 ** File Name:      audio_cfg.h                                               *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the CUSTOMER CONFIG of AUDIO            *
 **					about product                                             *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang         Create.                                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#ifndef _AUDIO_CFG_H_
#define _AUDIO_CFG_H_


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif



/*****************************************************************************/
//  Description:    This function is used to control extern pa.
//  Author:         ken.kuang
//  Param:          en: enable/disable
//                  id: some customer use dual spk,then id=1 is the other PA.
//  Note:
/*****************************************************************************/
PUBLIC void codec_pa_ctl(uint32 en,uint32 id);

/*****************************************************************************/
//  Description:    This function is used to control extern hp pa.
//  Author:         ken.kuang
//  Param:          en: enable/disable
//  Note:
/*****************************************************************************/
PUBLIC void codec_hp_pa_ctl(uint32 en);

/*****************************************************************************/
//  Description:    This function is used to get extern hp pa params.
//  Author:         ken.kuang
//  Param:          param: [OUT] delay time from hp enable to extern hp pa on.
//  Note: if use external hp pa,return 1,else MUST return 0.
/*****************************************************************************/
PUBLIC uint32 codec_hp_pa_test_and_get_param(void *param);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif

#endif // _AUDIO_CFG_H_

