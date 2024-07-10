/*
 * <nv_dev_adp.c> - <nv adapter layer.>
 *
 * Copyright (c) 2019 Unisoc Communications Inc.
 * History
 *      <2011/01/16> <shujing.dong> <creating>
 *      <2019/11/11> <elon li> <refactoring>
 *      This file defines corresponding relations between nv and logic device..
 */
#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
#include "admm_II.h"
#endif
#include "audio_config.h"
#ifdef CVS_ENABLE
#include "cvs_ctrl_param_nv.h"
#endif
#ifdef NXP_LVVE_SUPPORT
#include "lvve_ctrl_param_nv.h"
#endif
#include "ms_ref_aud_dev_trc.h"
#include "nv_dev_adp.h"
#include "os_api.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __arm
#ifndef __MODULE__
#define __MODULE__ "dspvb_as.c"
#endif
#endif
#ifdef DSP_NV
extern uint32 s_pcm_on_count;
extern unsigned char BT_NREC_connected;
extern int voicecall_network_status;

LOCAL NV_AUDIO_E _BASE_GetDspAudioNvId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
	NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		nv_audio_e = NV_AUDIO_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		nv_audio_e = NV_AUDIO_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		/*if (s_pcm_on_count > 0) {
			nv_audio_e = NV_AUDIO_BTHS_E;
		} else*/
		nv_audio_e = NV_AUDIO_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		nv_audio_e = NV_AUDIO_HEADFREE_E;
		break;
	case AUDIO_DEVICE_MODE_TVOUT:
		nv_audio_e = NV_AUDIO_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		nv_audio_e = NV_AUDIO_BTHS_E;
		break;
	default:
		nv_audio_e = NV_AUDIO_HANDSET_E;
		SCI_TRACE_LOW ("[_BASE_GetDspAudioNvId]: error mode:%d", eDev_mode);
		break;
	}
	SCI_TRACE_LOW ("[_BASE_GetDspAudioNvId]: mode:%d, nv_audio_e:%d", eDev_mode,
		nv_audio_e);
	return nv_audio_e;
}

LOCAL NV_AUDIO_E _LAYER1_GetDspAudioNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode, NV_DSP_TYPE_E flag)
{
	NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
	int *bt_headset_type = NULL;
	NV_DSP_TYPE_E net_flag = NV_DSP_TYPE_GSM;

	/*if (AUDIONVARM_GetTDEnable() || (NV_DSP_TYPE_TD != flag))
		net_flag = flag;*/

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_audio_e = (NV_AUDIO_E)eDev_mode;
#else
	switch(eDev_mode)
	{
	case AUDIO_DEVICE_MODE_HANDHOLD:
		if (NV_DSP_TYPE_GSM == net_flag)
			nv_audio_e = NV_AUDIO_HANDSET_E;
		else if (NV_DSP_TYPE_TD == net_flag)
			nv_audio_e = NV_AUDIO_HANDSET_TD_E;
		else
			nv_audio_e = NV_AUDIO_HANDSET_WB_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		if (NV_DSP_TYPE_GSM == net_flag)
			nv_audio_e = NV_AUDIO_HANDSFREE_E;
		else if (NV_DSP_TYPE_TD == net_flag)
			nv_audio_e = NV_AUDIO_HANDSFREE_TD_E;
		else
			nv_audio_e = NV_AUDIO_HANDSFREE_WB_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		if (s_pcm_on_count > 0) {
			AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
			if (BT_NREC_connected){
				if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
					nv_audio_e = NV_AUDIO_BTHSNREC_DEV16K_E;
				else
					nv_audio_e = NV_AUDIO_BTHSNREC_E;
			} else {
				if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
					nv_audio_e = NV_AUDIO_BTHS_DEV16K_E;
				else
					nv_audio_e = NV_AUDIO_BTHS_E;
			}
		} else {
			if(NV_DSP_TYPE_GSM == net_flag)
				nv_audio_e = NV_AUDIO_HEADSET_E;
			else if(NV_DSP_TYPE_TD == net_flag)
				nv_audio_e = NV_AUDIO_HEADSET_TD_E;
			else
				nv_audio_e = NV_AUDIO_HEADSET_WB_E;
		}
		break;
	case AUDIO_DEVICE_MODE_TVOUT:
		if(NV_DSP_TYPE_GSM == net_flag)
			nv_audio_e = NV_AUDIO_HEADSET_TVOUT_E;
		else if(NV_DSP_TYPE_TD == net_flag)
			nv_audio_e = NV_AUDIO_HEADSET_TVOUT_TD_E;
		else
			nv_audio_e = NV_AUDIO_HEADSET_TVOUT_WB_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		SCI_TRACE_LOW("_LAYER1_GetDspAudioNvId, case AUDIO_DEVICE_MODE_BLUEPHONE, BT_NREC_connected=%d", BT_NREC_connected);
		AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
		if (BT_NREC_connected) {
			if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
				nv_audio_e = NV_AUDIO_BTHSNREC_DEV16K_E;
			else
				nv_audio_e = NV_AUDIO_BTHSNREC_E;
		} else {
			if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
				nv_audio_e = NV_AUDIO_BTHS_DEV16K_E;
			else
				nv_audio_e = NV_AUDIO_BTHS_E;
		}
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
		AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
		if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
			nv_audio_e = NV_AUDIO_BTHSNREC_DEV16K_E;
		else
			nv_audio_e = NV_AUDIO_BTHSNREC_E;
		break;
	default:
		if(NV_DSP_TYPE_GSM == net_flag)
			nv_audio_e = NV_AUDIO_HANDSET_E;
		else if(NV_DSP_TYPE_TD == net_flag)
			nv_audio_e = NV_AUDIO_HANDSET_TD_E;
		else
			nv_audio_e = NV_AUDIO_HANDSET_WB_E;
		SCI_TRACE_LOW ("[_LAYER1_GetDspAudioNvId]: error mode:%d, set default:%d ", eDev_mode,nv_audio_e);
		break;
	}
#endif
	SCI_TRACE_LOW ("[_LAYER1_GetDspAudioNvId]: mode:%d, id:%d", eDev_mode, nv_audio_e);
	return nv_audio_e;
}

LOCAL NV_AUDIO_DSP_CODEC_E _LAYER1_GetDspCodecNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode, NV_DSP_TYPE_E flag)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    int *bt_headset_type = NULL;
    NV_DSP_TYPE_E net_flag = NV_DSP_TYPE_GSM;

    /*if(AUDIONVARM_GetTDEnable() || (NV_DSP_TYPE_TD != flag))
    {
        net_flag = flag;
    }*/
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
    dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
         if(NV_DSP_TYPE_TD == net_flag)
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_TD_E;
        }
        else
        {
            if(voicecall_network_status)
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_WB_E;
            else
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
         if(NV_DSP_TYPE_TD == net_flag)
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_TD_E;
        }
        else
        {
            if(voicecall_network_status)
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_WB_E;
            else
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        if (s_pcm_on_count > 0)
        {
            AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
            if(BT_NREC_connected)
            {
                if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_D16K_E;
                else
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_E;
            }
            else
            {
                if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_D16K_E;
                else
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
            }
        }
        else
        {
             if(NV_DSP_TYPE_TD == net_flag)
            {
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_TD_E;
            }
            else
            {
                if(voicecall_network_status)
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_WB_E;
                else
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
            }
        }
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        {
             if(NV_DSP_TYPE_TD == net_flag)
            {
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_TVOUT_TD_E;
            }
            else
            {
                if(voicecall_network_status)
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_TVOUT_WB_E;
                else
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_TVOUT_E;
            }
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
		SCI_TRACE_LOW("_LAYER1_GetDspCodecNvId, case AUDIO_DEVICE_MODE_BLUEPHONE, BT_NREC_connected=%d", BT_NREC_connected);
        if(BT_NREC_connected)
        {
            AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
            if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_D16K_E;
            else
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_E;

        }
        else
        {
            AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
            if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_D16K_E;
            else
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        }
        break;

        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
            if ((NULL != bt_headset_type) && (0x40 == *bt_headset_type))
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_D16K_E;
            else
                dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHSNREC_E;
        }
        break;
        default:
            if(NV_DSP_TYPE_TD == net_flag)
            {
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_TD_E;
            }
            else
            {
                if(voicecall_network_status)
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_WB_E;
                else
                    dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
            }
       SCI_TRACE_LOW ("[_LAYER1_GetDspCodecNvId]: error mode:%d, set default:%d ", eDev_mode,dsp_codec_id);
        break;
    }
#endif

    SCI_TRACE_LOW ("[_LAYER1_GetDspCodecNvId]: mode:%d, dsp_codec_id:%d", eDev_mode, dsp_codec_id);
    return dsp_codec_id;
}

LOCAL NV_AUDIO_E _RECORD_GetDspAudioNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_audio_e = (NV_AUDIO_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            nv_audio_e = NV_AUDIO_RECORD_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            nv_audio_e = NV_AUDIO_RECORD_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        {
            if(BT_NREC_connected)
            {
                nv_audio_e = NV_AUDIO_BTHSNREC_E;
            }
            else
            {
                nv_audio_e = NV_AUDIO_BTHS_E;
            }
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            nv_audio_e = NV_AUDIO_BTHSNREC_E;
        }
        break;
        default:
        {
            nv_audio_e = NV_AUDIO_RECORD_HANDSFREE_E;
        }
        SCI_TRACE_LOW ("[_RECORD_GetDspAudioNvId]: error mode:%d, set default:%d ", eDev_mode,nv_audio_e);
        break;
    }
#endif
    SCI_TRACE_LOW ("[_RECORD_GetDspAudioNvId]: mode:%d, nv_audio_e:%d", eDev_mode, nv_audio_e);
    return nv_audio_e;
}

LOCAL NV_AUDIO_DSP_CODEC_E _RECORD_GetDspCodecNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
    dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        }
        break;
        default:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_HANDSFREE_E;
        }
        SCI_TRACE_LOW ("[_RECORD_GetDspCodecNvId]: error mode:%d, set default:%d ", eDev_mode,dsp_codec_id);
        break;
    }
#endif

    SCI_TRACE_LOW ("[_RECORD_GetDspCodecNvId]: mode:%d, dsp_codec_id:%d", eDev_mode, dsp_codec_id);
    return dsp_codec_id;
}

LOCAL NV_AUDIO_E _LOOPBACK_GetDspAudioNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_audio_e = (NV_AUDIO_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            nv_audio_e = NV_AUDIO_LOOPBACK_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            nv_audio_e = NV_AUDIO_LOOPBACK_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            nv_audio_e = NV_AUDIO_LOOPBACK_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        {
            nv_audio_e = NV_AUDIO_LOOPBACK_TVOUT_E;
        }
        break;
        default:
        {
            nv_audio_e = NV_AUDIO_LOOPBACK_HANDSFREE_E;
        }
        SCI_TRACE_LOW ("[_LOOPBACK_GetDspAudioNvId]: error mode:%d, set default:%d ", eDev_mode,nv_audio_e);
        break;
    }
#endif
	SCI_TRACE_LOW ("[_LOOPBACK_GetDspAudioNvId]: mode:%d, nv_audio_e:%d", eDev_mode, nv_audio_e);
    return nv_audio_e;
}

LOCAL NV_AUDIO_DSP_CODEC_E _LOOPBACK_GetDspCodecNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
    dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHEADSET_TV_E;
        }
        break;
        default:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDFREE_E;
        }
        SCI_TRACE_LOW ("[_LOOPBACK_GetDspCodecNvId]: error mode:%d, set default:%d ", eDev_mode,dsp_codec_id);
        break;
    }
#endif

    SCI_TRACE_LOW ("[_LOOPBACK_GetDspCodecNvId]: mode:%d, dsp_codec_id:%d", eDev_mode, dsp_codec_id);
    return dsp_codec_id;
}

LOCAL NV_AUDIO_E _MP4RECORD_GetDspAudioNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_audio_e = (NV_AUDIO_E)eDev_mode;
#else
    switch(eDev_mode)
    {
    	 case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            nv_audio_e = NV_AUDIO_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            nv_audio_e = NV_AUDIO_MP4HFTR_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            nv_audio_e = NV_AUDIO_MP4HFTR_HEADSET_E;
        }
        break;
        default:
        {
            nv_audio_e = NV_AUDIO_MP4HFTR_HANDSFREE_E;
        }
        SCI_TRACE_LOW ("[_MP4RECORD_GetDspAudioNvId]: error mode:%d, set default:%d ", eDev_mode,nv_audio_e);
        break;
    }
#endif
    SCI_TRACE_LOW ("[_MP4RECORD_GetDspAudioNvId]: mode:%d, nv_audio_e:%d", eDev_mode, nv_audio_e);
    return nv_audio_e;
}

LOCAL NV_AUDIO_DSP_CODEC_E _MP4RECORD_GetDspCodecNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
    dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        }
        break;
        default:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_HANDSFREE_E;
        }
        SCI_TRACE_LOW ("[_MP4RECORD_GetDspCodecNvId]: error mode:%d, set default:%d ", eDev_mode,dsp_codec_id);
        break;
    }
#endif

    SCI_TRACE_LOW ("[_MP4RECORD_GetDspCodecNvId]: mode:%d, dsp_codec_id:%d", eDev_mode, dsp_codec_id);
    return dsp_codec_id;
}

PUBLIC NV_AUDIO_DSP_CODEC_E NV_DEV_LAYER1_GetDspCodecId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
	NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		/*if (s_pcm_on_count > 0) {
		dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
		} else*/
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
		break;
	default:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		SCI_TRACE_LOW ("[NV_DEV_LAYER1_GetDspCodecId]: mode:%d", eDev_mode);
	break;
	}
	return dsp_codec_id;
}

PUBLIC NV_AUDIO_DSP_CODEC_E NV_DEV_RECORD_GetDspCodecId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
	NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
		break;
	default:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		SCI_TRACE_LOW ("[NV_DEV_RECORD_GetDspCodecId]: mode:%d", eDev_mode);
		break;
	}
	return dsp_codec_id;
}

PUBLIC NV_AUDIO_DSP_CODEC_E NV_DEV_MP4RECORD_GetDspCodecId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
	NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
		break;
	default:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
		SCI_TRACE_LOW ("[NV_DEV_MP4RECORD_GetDspCodecId]: mode:%d", eDev_mode);
		break;
	}
	return dsp_codec_id;
}

PUBLIC NV_AUDIO_DSP_CODEC_E NV_DEV_LOOPBACK_GetDspCodecId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
	NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
		break;
	default:
		dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDSET_E;
		SCI_TRACE_LOW ("[NV_DEV_LOOPBACK_GetDspCodecId]: mode:%d", eDev_mode);
		break;
	}
	return dsp_codec_id;
}

#if 0
PUBLIC AUDIO_NV_DSP_RESULT_E NV_DEV_LOOPBACK_GetDspCodecModeParam(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode,
	AUDIO_NV_DSP_MODE_INFO_T* ptAudMode
)
{
	NV_AUDIO_DSP_CODEC_E id = NV_DEV_LOOPBACK_GetDspCodecId(eDev_mode);
	char *name = AUDIONVDSP_GetAudioModeName(id);
	AUDIO_NV_DSP_RESULT_E result = AUDIONVDSP_GetModeParam( (const uint8 *) name, ptAudMode);

	if (AUDIO_NV_DSP_NO_ERROR!=result) {
		id = NV_DEV_LAYER1_GetDspCodecId(eDev_mode);
		name = AUDIONVDSP_GetAudioModeName(id);
		result = AUDIONVDSP_GetModeParam( (const uint8 *) name, ptAudMode);
		if (AUDIO_NV_DSP_NO_ERROR!=result) {
			SCI_TRACE_LOW("dspvb_as.c NV_DEV_LOOPBACK_GetDspCodecModeParam "
				"mode:%d, result:%d.", eDev_mode, result) ;
		}
	}
	return result;
}
#endif
LOCAL NV_AUDIO_E _DEFAULT_GetDspAudioNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_audio_e = (NV_AUDIO_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            nv_audio_e = NV_AUDIO_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            nv_audio_e = NV_AUDIO_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        if (s_pcm_on_count > 0)
        {
             nv_audio_e = NV_AUDIO_BTHS_E;
        }
        else
        {
            nv_audio_e = NV_AUDIO_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARFREE:
        {
            nv_audio_e = NV_AUDIO_HEADFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        {
            nv_audio_e = NV_AUDIO_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        {
            nv_audio_e = NV_AUDIO_BTHS_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            nv_audio_e = NV_AUDIO_BTHSNREC_E;
        }
        break;
        default:
        {
            nv_audio_e = NV_AUDIO_HANDSET_E;
        }
        SCI_TRACE_LOW ("[_DEFAULT_GetDspAudioNvId]: error mode:%d, set default:%d ", eDev_mode,nv_audio_e);
        break;
    }
#endif
    SCI_TRACE_LOW ("[_DEFAULT_GetDspAudioNvId]: mode:%d, nv_audio_e:%d", eDev_mode, nv_audio_e);
    return nv_audio_e;
}

LOCAL NV_AUDIO_DSP_CODEC_E _DEFAULT_GetDspCodecNvId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1) && defined (AUDIO_MODE_CUSTOM_CONFIG)
    dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)eDev_mode;
#else
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        if (s_pcm_on_count > 0)
        {
             dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        }
        else
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_EARFREE:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        }
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        }
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        case AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        }
        break;
        default:
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        }
        SCI_TRACE_LOW ("[_DEFAULT_GetDspCodecNvId]: error mode:%d, set default:%d ", eDev_mode,dsp_codec_id);
        break;
    }
#endif

    SCI_TRACE_LOW ("[_DEFAULT_GetDspCodecNvId]: mode:%d, dsp_codec_id:%d", eDev_mode, dsp_codec_id);
    return dsp_codec_id;
}

#ifdef NXP_LVVE_SUPPORT
LOCAL NV_AUDIO_NXP_E _LVVE_GetLvveNvId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode,
	NV_NXP_APP_TYPE_E flag1,
	NV_DSP_TYPE_E flag2)
{
	NV_AUDIO_NXP_E nv_nxp_e = (NV_AUDIO_NXP_E)0;
	int *bt_headset_type = NULL;
	//NV_DSP_TYPE_E net_flag = NV_DSP_TYPE_GSM;
	uint16 is_wb_process = VoiceProcess_GetWbMode();

	/*if(AUDIONVARM_GetTDEnable() || (NV_DSP_TYPE_TD != flag2)) {
		net_flag = flag2;
	}*/
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1)
	&& defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_nxp_e = (NV_AUDIO_NXP_E)eDev_mode;
#else
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		if (is_wb_process) {
			if (NV_NXP_APP_TYPE_VOIP == flag1) {
				//nv_nxp_e = _VOIP_GetLvveNvId(eDev_mode, eModeAppType);
			} else {
				nv_nxp_e = NV_AUDIO_NXP_HANDSET_WB_E;
			}
		} else {
			if (NV_NXP_APP_TYPE_LOOPBACK == flag1) {//loopback
				//nv_nxp_e = NV_AUDIO_NXP_LOOPBHANDSET_E;
			} else {
				/*if (NV_DSP_TYPE_TD == net_flag) {
					nv_nxp_e = NV_AUDIO_NXP_HANDSET_TD_E;
				} else {*/
					nv_nxp_e = NV_AUDIO_NXP_HANDSET_E;
				//}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		if (is_wb_process) {
			if (NV_NXP_APP_TYPE_VOIP == flag1) {
				//nv_nxp_e = _VOIP_GetLvveNvId(eDev_mode, eModeAppType);
			} else {
				nv_nxp_e = NV_AUDIO_NXP_HANDSFREE_WB_E;
			}
		} else {
			if (NV_NXP_APP_TYPE_LOOPBACK == flag1) { //loopback
				//nv_nxp_e = NV_AUDIO_NXP_LOOPBHANDSFREE_E;
			} else {
				/*if(NV_DSP_TYPE_TD == net_flag) {
					nv_nxp_e = NV_AUDIO_NXP_HANDSFREE_TD_E;
				} else {*/
					nv_nxp_e = NV_AUDIO_NXP_HANDSFREE_E;
				//}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		if (s_pcm_on_count > 0) {
			if (NV_NXP_APP_TYPE_VOIP == flag1) {
				//nv_nxp_e = _VOIP_GetLvveNvId(eDev_mode, eModeAppType);
			} else {
				//AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
				if (is_wb_process) {
					nv_nxp_e = NV_AUDIO_NXP_BTHS_WB_E;
				} else {
					nv_nxp_e = NV_AUDIO_NXP_BTHS_E;
				}
			}
		} else {
			if (is_wb_process) {
				if (NV_NXP_APP_TYPE_VOIP == flag1) {
					//nv_nxp_e = _VOIP_GetLvveNvId(eDev_mode, eModeAppType);
				} else {
					nv_nxp_e = NV_AUDIO_NXP_HEADSET_WB_E;
				}
			} else {
				if (NV_NXP_APP_TYPE_LOOPBACK == flag1) {//loopback
					//nv_nxp_e = NV_AUDIO_NXP_LOOPBHEADSET_E;
				} else {
					/*if (NV_DSP_TYPE_TD == net_flag) {
						nv_nxp_e = NV_AUDIO_NXP_HEADSET_TD_E;
					} else {*/
						nv_nxp_e = NV_AUDIO_NXP_HEADSET_E;
					//}
				}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		if (NV_NXP_APP_TYPE_VOIP == flag1) {
			//nv_nxp_e = _VOIP_GetLvveNvId(eDev_mode, eModeAppType);
		} else {
			//AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
			/*if (BT_NREC_connected) {
				if(is_wb_process) {
					nv_nxp_e = NV_AUDIO_NXP_BTHSNREC_D16K_E;
				} else {
					nv_nxp_e = NV_AUDIO_NXP_BTHSNREC_E;
				}
			} else {*/
				if (is_wb_process) {
					nv_nxp_e = NV_AUDIO_NXP_BTHS_WB_E;
				} else {
					nv_nxp_e = NV_AUDIO_NXP_BTHS_E;
				}
			//}
		}
		break;
	default:
		/*if (NV_DSP_TYPE_TD == net_flag) {
			nv_nxp_e = NV_AUDIO_NXP_HANDSET_TD_E;
		} else {*/
			if (is_wb_process) {
				nv_nxp_e = NV_AUDIO_NXP_HANDSET_WB_E;
			} else {
				nv_nxp_e = NV_AUDIO_NXP_HANDSET_E;
			}
		//}
		SCI_TRACE_LOW ("[_LVVE_GetLvveNvId]: error mode:%d, set default:%d ",
			eDev_mode,nv_nxp_e);
		break;
	}
#endif
	SCI_TRACE_LOW ("[_LVVE_GetLvveNvId]: mode:%d, nv_nxp_e:%d", eDev_mode,
		nv_nxp_e);
	return nv_nxp_e;
}
#endif
#ifdef CVS_ENABLE
LOCAL NV_AUDIO_CVS_E _CVS_GetCvsNvId(
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode,
	AUDIO_MODE_APP_TYPE_E eModeAppType,
	NV_CVS_APP_TYPE_E flag1,
	NV_DSP_TYPE_E flag2)
{
	NV_AUDIO_CVS_E nv_cvs_e = (NV_AUDIO_CVS_E)0;
	int *bt_headset_type = NULL;
	uint16 is_wb_process = VoiceProcess_GetWbMode();

	/*if (AUDIONVARM_GetTDEnable() || (NV_DSP_TYPE_TD != flag2)) {
		net_flag = flag2;
	}*/
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1)
	&& defined (AUDIO_MODE_CUSTOM_CONFIG)
	nv_cvs_e = (NV_AUDIO_CVS_E)eDev_mode;
#else
	switch (eDev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		if (is_wb_process) {
			if (NV_CVS_APP_TYPE_VOIP == flag1) {
				//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
			} else {
				nv_cvs_e = NV_AUDIO_CVS_HANDSET_WB_E;
			}
		} else {//voice-call
			if (NV_CVS_APP_TYPE_LOOPBACK == flag1) {//loopback
				nv_cvs_e = NV_AUDIO_CVS_LOOPBHANDSET_E;
			} else {
				/*if(NV_DSP_TYPE_TD == net_flag) {
					nv_cvs_e = NV_AUDIO_CVS_HANDSET_TD_E;
				} else {*/
					nv_cvs_e = NV_AUDIO_CVS_HANDSET_E;
				//}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		if (is_wb_process) {
			if (NV_CVS_APP_TYPE_VOIP == flag1){
				//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
			} else {
				nv_cvs_e = NV_AUDIO_CVS_HANDSFREE_WB_E;
			}
		} else {//voice-call
			if (NV_CVS_APP_TYPE_LOOPBACK == flag1) {//loopback
				nv_cvs_e = NV_AUDIO_CVS_LOOPBHANDSFREE_E;
			} else {
				/*if (NV_DSP_TYPE_TD == net_flag) {
					nv_cvs_e = NV_AUDIO_CVS_HANDSFREE_TD_E;
				} else {*/
					nv_cvs_e = NV_AUDIO_CVS_HANDSFREE_E;
				//}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		if (s_pcm_on_count > 0) {
			if (NV_CVS_APP_TYPE_VOIP == flag1) {
				//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
			} else {
				//AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
				/*if(BT_NREC_connected) {
					if (is_wb_process) {
						nv_cvs_e = NV_AUDIO_CVS_BTHSNREC_D16K_E;
					} else {
						nv_cvs_e = NV_AUDIO_CVS_BTHSNREC_E;
					}
				} else {*/
					if (is_wb_process) {
						nv_cvs_e = NV_AUDIO_CVS_BTHS_D16K_E;
					} else {
						nv_cvs_e = NV_AUDIO_CVS_BTHS_E;
					}
				//}
			}
		} else {
			if (is_wb_process) {
				if (NV_CVS_APP_TYPE_VOIP == flag1) {
					//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
				} else {
					nv_cvs_e = NV_AUDIO_CVS_HEADSET_WB_E;
				}
			} else {
				if (NV_CVS_APP_TYPE_LOOPBACK == flag1) {//loopback
					nv_cvs_e = NV_AUDIO_CVS_LOOPBHEADSET_E;
				} else {
					/*if (NV_DSP_TYPE_TD == net_flag) {
						nv_cvs_e = NV_AUDIO_CVS_HEADSET_TD_E;
					} else {*/
						nv_cvs_e = NV_AUDIO_CVS_HEADSET_E;
					//}
				}
			}
		}
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		if (NV_CVS_APP_TYPE_VOIP == flag1) {
			//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
		} else {
			//AUD_GetExtParameter("bt_headset_samplerate_type", (void *)&bt_headset_type);
			/*if(BT_NREC_connected) {
				if (is_wb_process) {
					nv_cvs_e = NV_AUDIO_CVS_BTHSNREC_D16K_E;
				} else {
					nv_cvs_e = NV_AUDIO_CVS_BTHSNREC_E;
				}
			} else {*/
				if(is_wb_process) {
					nv_cvs_e = NV_AUDIO_CVS_BTHS_D16K_E;
				} else {
					nv_cvs_e = NV_AUDIO_CVS_BTHS_E;
				}
			//}
		}
		break;
	case AUDIO_DEVICE_MODE_TVOUT:
		if (is_wb_process) {
			if (NV_CVS_APP_TYPE_VOIP == flag1) {
				//nv_cvs_e = _VOIP_GetCvsNvId(eDev_mode, eModeAppType);
			} else {
				nv_cvs_e = NV_AUDIO_CVS_HEADSET_TVOUT_WB_E;
			}
		} else {
			if (NV_CVS_APP_TYPE_LOOPBACK == flag1) {//loopback
				nv_cvs_e = NV_AUDIO_CVS_LOOPBHEADSET_TVOUT_E;
			} else {
				/*if (NV_DSP_TYPE_TD == net_flag) {
					nv_cvs_e = NV_AUDIO_CVS_HEADSET_TVOUT_TD_E;
				} else {*/
					nv_cvs_e = NV_AUDIO_CVS_HEADSET_TVOUT_E;
				//}
			}
		}
		break;
	default:
		/*if (NV_DSP_TYPE_TD == net_flag) {
			nv_cvs_e = NV_AUDIO_CVS_HANDSET_TD_E;
		} else {*/
			if (is_wb_process) {
				nv_cvs_e = NV_AUDIO_CVS_HANDSET_WB_E;
			} else {
				nv_cvs_e = NV_AUDIO_CVS_HANDSET_E;
			}
		//}
		SCI_TRACE_LOW ("[_CVS_GetCvsNvId]: error mode:%d, set default:%d ",
			eDev_mode, nv_cvs_e);
		break;
	}
#endif
	SCI_TRACE_LOW ("[_CVS_GetCvsNvId]: mode:%d, nv_nxp_e:%d", eDev_mode, nv_cvs_e);
	return nv_cvs_e;
}
#endif

PUBLIC char * NV_DEV_GetDspAudioNv_Name(
    NV_DEV_TYPE_E eNvDevType,
    AUDIO_DEVICE_MODE_TYPE_E  eDev_mode,
    AUDIO_MODE_APP_TYPE_E eModeAppType,
    NV_DSP_TYPE_E flag  //0: gsm, 1: td, 2:wb
    )
{
    NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
    char * name = NULL;
    AUDMOD_DEVMOD_T *ptDspAudioNv  = NULL;
    switch(eNvDevType)
    {
        case  NV_DEV_ADP_LAYER1:
        {
            nv_audio_e = _LAYER1_GetDspAudioNvId(eDev_mode, flag);
            break;
        }
        case  NV_DEV_ADP_RECORD:
        {
            nv_audio_e = _RECORD_GetDspAudioNvId(eDev_mode);
            break;
        }
        case  NV_DEV_ADP_MP4RECORD:
        {
            nv_audio_e = _MP4RECORD_GetDspAudioNvId(eDev_mode);
            break;
        }
        case  NV_DEV_ADP_LOOPBACK:
        {
            nv_audio_e = _LOOPBACK_GetDspAudioNvId(eDev_mode);
            break;
        }
        default:
        {
            SCI_TRACE_LOW ("[NV_DEV_GetDspAudioNv_Name]: error eNvDevType:%d, go to default type", eNvDevType);
            nv_audio_e = _DEFAULT_GetDspAudioNvId(eDev_mode);
            break;
        }
    }

    name = AUDMOD_GetAudioDeviceModeNameById(nv_audio_e);

#ifndef PSIT_ARM_L1SIM
    ptDspAudioNv = AUDMOD_GetDevMode((const uint8*)name);
    if(NULL==ptDspAudioNv)
    {
        SCI_TRACE_LOW ("[NV_DEV_GetDspAudioNv_Name]: first get fail, reget: eNvDevType:%d, mode:%d, flag:%d, name:%s",
                eNvDevType, eDev_mode, flag, name);

        nv_audio_e = _DEFAULT_GetDspAudioNvId(eDev_mode);
        name = AUDMOD_GetAudioDeviceModeNameById(nv_audio_e);
        ptDspAudioNv = AUDMOD_GetDevMode((const uint8*)name);
        if(NULL==ptDspAudioNv)
        {
            SCI_PASSERT(FALSE, ("[NV_DEV_GetDspAudioNv_Name]: can't get valid name, eNvDevType:%d, mode:%d, flag:%d",
                eNvDevType, eDev_mode, flag));
            SCI_TRACE_LOW ("[NV_DEV_GetDspAudioNv_Name]: can't get valid name, eNvDevType:%d, mode:%d, flag:%d",
                eNvDevType, eDev_mode, flag);
            return NULL;
        }
    }

    SCI_TRACE_LOW ("[NV_DEV_GetDspAudioNv_Name]: suceess, eNvDevType:%d, mode:%d, eModeAppType:%d, flag:%d, nv_name:%s",
                eNvDevType, eDev_mode, eModeAppType, flag, name);
#endif

    return name;
}
#if 0
PUBLIC char * NV_DEV_GetDspCodecNv_Name(
    NV_DEV_TYPE_E eNvDevType,
    AUDIO_DEVICE_MODE_TYPE_E  eDev_mode,
    AUDIO_MODE_APP_TYPE_E eModeAppType,
    NV_DSP_TYPE_E flag  //0: gsm, 1: td.
    )
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    char * name = NULL;
    AUDIO_NV_DSP_MODE_INFO_T *ptDspCodecNv  = NULL;
    switch(eNvDevType)
    {
        case  NV_DEV_ADP_LAYER1:
        {
            dsp_codec_id = _LAYER1_GetDspCodecNvId(eDev_mode, flag);
            break;
        }
        case  NV_DEV_ADP_RECORD:
        {
            dsp_codec_id = _RECORD_GetDspCodecNvId(eDev_mode);
            break;
        }
        case  NV_DEV_ADP_MP4RECORD:
        {
            dsp_codec_id = _MP4RECORD_GetDspCodecNvId(eDev_mode);
            break;
        }
        case  NV_DEV_ADP_LOOPBACK:
        {
            dsp_codec_id = _LOOPBACK_GetDspCodecNvId(eDev_mode);
            break;
        }
        default:
        {
            SCI_TRACE_LOW ("[NV_DEV_GetDspCodecNv_Name]: error eNvDevType:%d, go to default type", eNvDevType);
            dsp_codec_id = _DEFAULT_GetDspCodecNvId(eDev_mode);
            break;
        }
    }

    name = AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    ptDspCodecNv = _AUDIONVDSP_GetModeInfo(name);
    if(NULL==ptDspCodecNv)
    {
        SCI_TRACE_LOW ("[NV_DEV_GetDspCodecNv_Name]: first get fail, reget: eNvDevType:%d, mode:%d, flag:%d, name:%s",
                eNvDevType, eDev_mode, flag, name);

        dsp_codec_id = _DEFAULT_GetDspAudioNvId(eDev_mode);
        name = AUDIONVDSP_GetAudioModeName(dsp_codec_id);
        ptDspCodecNv = _AUDIONVDSP_GetModeInfo(name);
        if(NULL==ptDspCodecNv)
        {
            SCI_PASSERT(FALSE, ("[NV_DEV_GetDspCodecNv_Name]: can't get valid name, eNvDevType:%d, mode:%d, flag:%d",
                eNvDevType, eDev_mode, flag));
            SCI_TRACE_LOW ("[NV_DEV_GetDspCodecNv_Name]: can't get valid name, eNvDevType:%d, mode:%d, flag:%d",
                eNvDevType, eDev_mode, flag);
            return NULL;
        }
    }

    SCI_TRACE_LOW ("[NV_DEV_GetDspCodecNv_Name]: suceess, eNvDevType:%d, mode:%d, eModeAppType:%d, flag:%d, nv_name:%s",
                eNvDevType, eDev_mode, eModeAppType, flag, name);

    return name;
}
#endif

PUBLIC char * NV_DEV_GetValidDspAudioNvName(
	NV_DEV_ADP_E eNvDevAdp,
	AUDIO_DEVICE_MODE_TYPE_E  eDev_mode,
	uint16 flag) //flag:0: gsm, 1: td.
{
	NV_AUDIO_E nv_audio_e = (NV_AUDIO_E)0;
	char * name = NULL;
	AUDMOD_DEVMOD_T *ptDspAudioNv = NULL;
	switch (eNvDevAdp) {
	case NV_DEV_ADP_LAYER1:
		nv_audio_e = _LAYER1_GetDspAudioNvId(eDev_mode, flag);
		break;
	case NV_DEV_ADP_RECORD:
		nv_audio_e = _RECORD_GetDspAudioNvId(eDev_mode);
		break;
	case NV_DEV_ADP_MP4RECORD:
		nv_audio_e = _MP4RECORD_GetDspAudioNvId(eDev_mode);
		break;
	case NV_DEV_ADP_LOOPBACK:
		nv_audio_e = _LOOPBACK_GetDspAudioNvId(eDev_mode);
		break;
	default:
		SCI_TRACE_LOW ("[NV_DEV_GetValidDspAudioNvName]: error eNvDevAdp:%d",
			eNvDevAdp);
		nv_audio_e = _BASE_GetDspAudioNvId(eDev_mode);
		break;
	}

	name = AUDMOD_GetAudioDeviceModeNameById(nv_audio_e);
	ptDspAudioNv = AUDMOD_GetDevMode(name);
	if (NULL==ptDspAudioNv) {
		SCI_TRACE_LOW ("[NV_DEV_GetValidDspAudioNvName]: first get fail, "
			"reget: eNvDevAdp:%d, mode:%d, flag:%d, name:%s", eNvDevAdp,
			eDev_mode, flag, name);
		nv_audio_e = _BASE_GetDspAudioNvId(eDev_mode);
		name = AUDMOD_GetAudioDeviceModeNameById(nv_audio_e);
		ptDspAudioNv = AUDMOD_GetDevMode(name);
		if (NULL==ptDspAudioNv) {
			SCI_PASSERT(FALSE, ("[NV_DEV_GetValidDspAudioNvName]: "
				"can't get valid name, eNvDevAdp:%d, mode:%d, flag:%d", eNvDevAdp,
				eDev_mode, flag));
			SCI_TRACE_LOW ("[NV_DEV_GetValidDspAudioNvName]:"
				"can't get valid name, eNvDevAdp:%d, mode:%d, flag:%d", eNvDevAdp,
				eDev_mode, flag);
			return NULL;
		}
	}
	SCI_TRACE_LOW ("[NV_DEV_GetValidDspAudioNvName]: suceess, eNvDevAdp:%d,"
		"mode:%d, flag:%d, name:%s", eNvDevAdp, eDev_mode, flag, name);
	return name;
}
#endif
PUBLIC char * NV_DEV_GetArmAudioNv_Name(
	AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
	NV_AUDIO_ARM_E arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
	char * name = NULL;
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_CUSTOM1)
	&& defined (AUDIO_MODE_CUSTOM_CONFIG)
	arm_nv_id = (NV_AUDIO_ARM_E)aud_dev_mode;
#else
	switch (aud_dev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
		break;
	case AUDIO_DEVICE_MODE_TVOUT:
		arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		arm_nv_id = NV_AUDIO_ARM_BTHS_E;
		break;
	/*case  AUDIO_DEVICE_MODE_BLUEPHONE_NREC:
		arm_nv_id = NV_AUDIO_ARM_BTHS_E;
		break;*/
	default:
		arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
		break;
	}
#endif
	name =  AUDIONVARM_GetAudioModeNameById(arm_nv_id);
	SCI_TRACE_LOW("[NV_DEV_GetArmAudioNv_Name]: suceess, mode:%d, arm_nv_id:%d,"
		"nv_name:%s", aud_dev_mode, arm_nv_id, name);
	return name;
}


#ifdef __cplusplus
}
#endif // End of dspvb_as.c
