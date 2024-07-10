#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "audio_config.h"
#include "aud_gen.h"
#include "adm.h"
#include "dspvb_as.h"
#include "armvb_as.h"
#include "audio_config.h"

#include "sc_reg.h"

#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
#include "admm_II.h"
#endif

#include "layer1_audio.h"

#ifdef DSP_CTRL_PCM
#include "pcm_drvapi.h"
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif
#ifndef __arm
#ifndef __MODULE__
#define __MODULE__ "dspvb_as.c"
#endif
#endif
#include "deepsleep_drvapi.h"
#include "pmic_26M_clock.h"
/*lint -save -e752 */

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to register LAYER1 logical VB device.
//  Author:         Johnson.sun
//  Note:  used for calling condition.
//****************************************************************************/
PUBLIC HAUDIODEV LAYER1_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("LAYER1"), strlen ("LAYER1") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:   used for general record condition.
//****************************************************************************/
PUBLIC HAUDIODEV RECORD_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("RECORD"), strlen ("RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;

}

PUBLIC HAUDIODEV MP4_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("MP4"), strlen ("MP4") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;


    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register MP4RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:  used for mp4 photograph condition.
//****************************************************************************/
PUBLIC HAUDIODEV MP4RECORD_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("MP4RECORD"), strlen ("MP4RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;


    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register LOOPBACK logical VB device.
//  Author:         
//  Note:  used for calling condition in loopback.
//****************************************************************************/
PUBLIC HAUDIODEV LOOPBACK_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;

}

/*****************************************************************************/
//  Description:    This function is used to reg device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV DSPDATA_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("DSPDATA"), strlen ("DSPDATA") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = 0;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = SCI_NULL;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

#ifdef   __cplusplus
}
#endif  // End of dspvb_as.c




