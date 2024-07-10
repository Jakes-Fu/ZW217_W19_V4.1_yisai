/*******************************************************************/
/*  Copyright (C) 2001 SPREADTRUM COMMUNICATION¡¡CO.               */
/*  All rights reserved.                                           */
/*                                                                 */
/*  This document contains proprietary information belonging to    */
/*  SPREADTRUM. Passing on and copying of this                     */
/*  document, use and communication of its contents is not         */
/*  permitted without prior written authorisation.                 */
/*******************************************************************/
/* Revision information:                                           */
/* File      [ env_atc_signal.h                       ]             */
/* Version   [ 0.1  Draft  07/30/2002   ivan ren  ]             */
/* Description                                                     */
/*      this header file define atc received signals                */
/*******************************************************************/

#ifndef _ENV_LAYER1_SIGNAL_H
#define _ENV_LAYER1_SIGNAL_H


#ifndef _EDGE_DEV_
#define ENV_LAYER1_SIGNAL_LIST	\
        DSP_IS_READY_REQ = END_LAYER1_IN_PS_SIGNAL, \
        DSP_RX_BLOCK_IS_READY_REQ, \
        DSP_BLOCK_SEND_REQ, \
        DSP_MEAS_REPORT_REQ, \
        DSP_ASK_MORE_REQ, \
        DSP_TX_SACCH_REQ, \
        DSP_TX_FACCH_REQ, \
        DSP_TX_SDCCH_REQ, \
        DSP_SEND_RACH_CNF, \
        DSP_CHANNEL_CHANGE_CNF, \
        DSP_POWERMONQUEUE_REQ, \
        DSP_BEPARFCN_REPORT_REQ, \
        DSP_OVER_RUN_MCU_REQ, \
        DSP_GPRS_MEAS_IS_READY, \
        DSP_SCH_DECODE_FAIL, \
        DSP_WAKEUP, \
        MCU_ADJUST_DELTA_FN, \
        DSP_GPS_CIPHER_REPORT_READY,\
        DSP_ECHO_TESTING_REPORT_CNF, \
        DSP_READW_MEM_CNF, \
        WAIT_REQ , \
        ERROR_SIGNAL_CODE,	\
        UPDATE_RXLEV_REQ, \
        UPDATE_BSIC_REQ, \
        FRAME_QUEUE_EMPTY,\
        APP_START_VOICE_RECORD_REQ,\
        APP_PH_SWITCH_SOUND_DEVICE_REQ , \
        APP_PH_ENABLE_CODEC_REQ , \
        APP_PH_ENABLE_AUDIO_REQ , \
        APP_PH_SPECIAL_TONE_REQ , \
        APP_PH_SET_VOLUME_REQ , \
        APP_PH_MUTE_SWITCH_REQ , \
        APP_PH_STOP_TONE_REQ , \
        APP_PH_GENERIC_TONE_REQ , \
        APP_PH_SWITCH_TEST_MODE_REQ , \
        APP_PH_ENABLE_GPS_REQ , \
        APP_PH_AUDIO_ENGINEERING_PARAMETER_REQ , \
        APP_PH_ENABLE_VOICE_LOOP_REQ ,\
        APP_PH_DOWNLINK_MUTE_ENABLE_REQ ,\
        APP_PH_ENABLE_ECHO_TEST_REQ ,\
        APP_START_VOICE_DOWNLOAD_REQ ,\
        APP_PH_SET_DEV_MODE_REQ ,\
        MPH_BCCH_BUFREPORT_TIMER_RESET_REQ, \
        APP_PH_MP3_A2DP_REQ , \
        L1_BCCH_MEAS_REPORT_TIMER_REQ,\
        APP_PH_AUDIO_REQ,\
        APP_START_MAGIC_VOICE_REQ,\
        APP_START_NXP_REQ,\
        APP_RXADC_ONOFF_REQ,\
        END_LAYER1_NORMAL_SIGNAL,\
        DIAG_L1_WR_CALI_PARAM_REQ = END_LAYER1_NORMAL_SIGNAL,\
        DIAG_L1_RD_CALI_PARAM_REQ,\
        CALI_ACTIVE_REQ,\
        CALI_DEACTIVE_REQ,\
        CALI_BSIC_DECODE_REQ,\
        CALI_BCCH_DECODE_REQ,\
        CALI_BCCH_STOP_REQ,\
        CALI_RXPOWER_SWEEP_REQ,\
        CALI_RX_REQ,\
        CALI_STOP_RX_REQ,\
        CALI_TX_REQ,\
        CALI_STOP_TX_REQ,\
        CALI_RACH_REQ,\
        CALI_STOP_RACH_REQ,\
        CALI_RF_CONTROL_REQ,\
        CALI_CLASSMARK_REQ,  \
        CALI_PH_ENABLE_CODEC_REQ,\
        CALI_NOSIGAL_RXTX_LOOP_REQ,\
        CALI_NOSIGAL_RXTX_LOOP_CNF,\
        CALI_FASTCALI_REQ,\
        CALI_FASTCALI_CNF,\
        CALI_DSP_FCCH_FREQ_OFFSET_REQ,\
        END_LAYER1_SIGNAL, 
#else
#define ENV_LAYER1_SIGNAL_LIST	\
        DSP_IS_READY_REQ = END_LAYER1_IN_PS_SIGNAL, \
        DSP_RX_BLOCK_IS_READY_REQ, \
        DSP_BLOCK_SEND_REQ, \
        DSP_MEAS_REPORT_REQ, \
        DSP_ASK_MORE_REQ, \
        DSP_TX_SACCH_REQ, \
        DSP_TX_FACCH_REQ, \
        DSP_TX_SDCCH_REQ, \
        DSP_SEND_RACH_CNF, \
        DSP_CHANNEL_CHANGE_CNF, \
        DSP_POWERMONQUEUE_REQ, \
        DSP_OVER_RUN_MCU_REQ, \
        DSP_GPRS_MEAS_IS_READY, \
        DSP_SCH_DECODE_FAIL, \
        DSP_WAKEUP, \
        MCU_ADJUST_DELTA_FN, \
        DSP_GPS_CIPHER_REPORT_READY,\
        DSP_ECHO_TESTING_REPORT_CNF, \
        DSP_READW_MEM_CNF, \
        WAIT_REQ , \
        ERROR_SIGNAL_CODE,	\
        UPDATE_RXLEV_REQ, \
        UPDATE_BSIC_REQ, \
        FRAME_QUEUE_EMPTY,\
        APP_START_VOICE_RECORD_REQ,\
        APP_PH_SWITCH_SOUND_DEVICE_REQ , \
        APP_PH_ENABLE_CODEC_REQ , \
        APP_PH_ENABLE_AUDIO_REQ , \
        APP_PH_SPECIAL_TONE_REQ , \
        APP_PH_SET_VOLUME_REQ , \
        APP_PH_MUTE_SWITCH_REQ , \
        APP_PH_STOP_TONE_REQ , \
        APP_PH_GENERIC_TONE_REQ , \
        APP_PH_SWITCH_TEST_MODE_REQ , \
        APP_PH_ENABLE_GPS_REQ , \
        APP_PH_AUDIO_ENGINEERING_PARAMETER_REQ , \
        APP_PH_ENABLE_VOICE_LOOP_REQ ,\
        APP_PH_DOWNLINK_MUTE_ENABLE_REQ ,\
        APP_PH_ENABLE_ECHO_TEST_REQ ,\
        APP_START_VOICE_DOWNLOAD_REQ ,\
        APP_PH_SET_DEV_MODE_REQ ,\
        MPH_BCCH_VICECAMP_REQ, \
        APP_PH_MP3_A2DP_REQ , \
        L1_BCCH_MEAS_REPORT_TIMER_REQ,\
        APP_PH_AUDIO_REQ,\
        APP_START_MAGIC_VOICE_REQ,\
        APP_START_NXP_REQ,\
        END_LAYER1_NORMAL_SIGNAL,\
        DIAG_L1_WR_CALI_PARAM_REQ = END_LAYER1_NORMAL_SIGNAL,\
        DIAG_L1_RD_CALI_PARAM_REQ,\
        CALI_ACTIVE_REQ,\
        CALI_DEACTIVE_REQ,\
        CALI_BSIC_DECODE_REQ,\
        CALI_BCCH_DECODE_REQ,\
        CALI_BCCH_STOP_REQ,\
        CALI_RXPOWER_SWEEP_REQ,\
        CALI_RX_REQ,\
        CALI_STOP_RX_REQ,\
        CALI_TX_REQ,\
        CALI_STOP_TX_REQ,\
        CALI_EDGE_TX_REQ,\
        CALI_STOP_EDGE_TX_REQ,\
        CALI_RACH_REQ,\
        CALI_STOP_RACH_REQ,\
        CALI_RF_CONTROL_REQ,\
        CALI_CLASSMARK_REQ,  \
        CALI_PH_ENABLE_CODEC_REQ,\
        CALI_NOSIGAL_RXTX_LOOP_REQ,\
        CALI_NOSIGAL_RXTX_LOOP_CNF,\
        CALI_FASTCALI_REQ,\
        CALI_FASTCALI_CNF,\
        CALI_DSP_FCCH_FREQ_OFFSET_REQ,\
        END_LAYER1_SIGNAL, 
#endif


#endif
