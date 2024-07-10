#ifndef __AUDIO_CP_H
#define __AUDIO_CP_H
#include "rpc_aud.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

PUBLIC AUDIO_RESULT_E AUDIO_CP_STOP(AUDIO_DEV_TYPE_E dev_type,SND_ITF_T itf, AUD_AP2CP_PARA_T *zsp_para);
PUBLIC AUDIO_RESULT_E AUDIO_CP_START(
    AUDIO_DEV_TYPE_E dev_type,
    SND_ITF_T itf, 
    HAL_AIF_STREAM_T *stream, 
    AUD_LEVEL_T *cfg,
    VOIS_AUDIO_CFG_T *Vois_cfg,
    AUD_DEVICE_CFG_EXT_T devicecfg, 
    SND_BT_WORK_MODE_T btworkmode,
    AUD_AP2CP_PARA_T *zsp_para
    );
PUBLIC AUD_ERR_T AUDIO_CP_RecordStart(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg);
PUBLIC AUD_ERR_T AUDIO_CP_Setup(SND_ITF_T itf, AUD_LEVEL_T *cfg);
PUBLIC VOIS_ERR_T AUDIO_CP_VoisSetCfg(AUD_ITF_T itf, VOIS_AUDIO_CFG_T *cfg, AUD_DEVICE_CFG_EXT_T devicecfg, SND_BT_WORK_MODE_T btworkmode);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of audio_cp.h

#endif //__AUDIO_CP_H
