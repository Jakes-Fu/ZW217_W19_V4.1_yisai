/******************************************************************************
 ** File Name:      audio_cfg.c                                              *
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


#include "os_api.h"
#include "audio_drvapi.h"
#include "audio_dai.h"
#include "audio_codec.h"

#include "audio_cfg.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **        Static Data
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                            *
 **---------------------------------------------------------------------------*/

extern PUBLIC void GPIO_EnableAmplifier (BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function is used to control extern pa.
//  Author:         ken.kuang
//  Param:          en: enable/disable
//                  id: some customer use dual spk,then id=1 is the other PA.
//  Note:
/*****************************************************************************/
PUBLIC void codec_pa_ctl(uint32 en,uint32 id)
{
#ifndef PLATFORM_UWS6121E
    if( 0==id )
    {
        if( en )
        {
            GPIO_EnableAmplifier(SCI_TRUE);
        }
        else
        {
            GPIO_EnableAmplifier(SCI_FALSE);
        }
    }
    else
    {
    }
#endif
}


/*****************************************************************************/
//  Description:    This function is used to control extern hp pa.
//  Author:         ken.kuang
//  Param:          en: enable/disable
//  Note:
/*****************************************************************************/
PUBLIC void codec_hp_pa_ctl(uint32 en)
{
}

/*****************************************************************************/
//  Description:    This function is used to get extern hp pa params.
//  Author:         ken.kuang
//  Param:          param: [OUT] delay time from hp enable to extern hp pa on.
//  Note: if use external hp pa,return 1,else MUST return 0.
/*****************************************************************************/
PUBLIC uint32 codec_hp_pa_test_and_get_param(void *param)
{
    if( param )
    {
        *((uint32*)param) = 0; //ms
    }
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to config custom dai ops.
//  Author:         ken.kuang
//  Param:          
//  Note:
/*****************************************************************************/
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_CUSTOM_OPS(uint32 param)
{
    return (AUD_DAI_OPS_T*)(0);
}

/*****************************************************************************/
//  Description:    This function is used to config custom codec ops.
//  Author:         ken.kuang
//  Param:          
//  Note:
/*****************************************************************************/
PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_CUSTOM_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T*)(0);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  

// End of i2s_cfg.c

