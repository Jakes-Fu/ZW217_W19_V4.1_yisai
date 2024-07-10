/******************************************************************************
 ** File Name:      audio_app.c                                               *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines audio general application interface.    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/30/2010     shujing.dong     Create.                                   *
 ******************************************************************************/

 /*! \file audio_app.c
  *  \author shujing.dong
  *  \date june 30, 2010
  *  \brief This file defines audio general application interface.
  */

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "audio_api.h"
#include "audio_app.h"
#include "audio_config.h"
#include "aud_gen.h"
#include "layer1_audio.h"
#include "ms_ref_aud_config_trc.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_AUDIO_APP

#ifdef DEBUG_AUDIO_APP
    #ifndef AUDIO_PRINT_APP
    #define AUDIO_PRINT_APP( _format_string )   SCI_TRACE_LOW _format_string
    #endif
#else
    #ifndef AUDIO_PRINT_APP
    #define AUDIO_PRINT_APP( _format_string )
    #endif
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define AUDIO_APP_PCM_BUF_SIZE 640
#define DSPDATACODEC_ADP_EXT_OPE_PLAY_PARA_COUNT  4
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
BOOLEAN s_tx_clarity_enable = FALSE;
HAUDIO  s_volte_audiohandle = INVALID_HANDLE;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Definitions                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: Only one dummy function.
// Author:
// Note:
/*****************************************************************************/
static void audio_app_Dummy_Notify(
    HAUDIO hAudio,
    uint32 notify_info,
    uint32 affix_info
)
{

}
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_APP_EnableDataFetch(void)
//! \param  is_enable
//! \return AUDIO_RESULT_E
//! \brief  Description: This function is used to set data_fetch enable/disable.
//! \author Author: shujing.dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDataFetch(  // If succeed, return AUDIO_NO_ERROR,
                                        // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
    )
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    static HAUDIO h_data_play=INVALID_HANDLE, h_data_record=INVALID_HANDLE;
    static uint32 *ui_pcm_buffer=PNULL;
    AUDIO_PRINT_APP(("AUDIO_EnablePcm: %s", is_enable?"enable":"disable"));

    if(is_enable)
    {
        if(PNULL!=ui_pcm_buffer)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm] be enable buf:0x%x."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_89_112_2_18_1_1_59_7,(uint8*)"d", ui_pcm_buffer);
            return AUDIO_ERROR;
        }
        if((INVALID_HANDLE!=h_data_play) || (INVALID_HANDLE!=h_data_record))
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm] be enable handle:0x%x, 0x%x."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_94_112_2_18_1_1_59_8,(uint8*)"dd", h_data_play, h_data_record);
            return AUDIO_ERROR;
        }
        ui_pcm_buffer = (uint32 *)SCI_ALLOC_APP( AUDIO_APP_PCM_BUF_SIZE*4);
        if(PNULL == ui_pcm_buffer )
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]ui_pcm_buffer alloc failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_100_112_2_18_1_1_59_9,(uint8*)"");
            return AUDIO_ERROR;
        }
#ifndef WIN32
        h_data_play = AUDIO_CreateBufferHandle(hDownlinkCodec, SCI_NULL, hPCMDev, SCI_NULL,
            SCI_NULL, 0, ui_pcm_buffer, AUDIO_APP_PCM_BUF_SIZE, audio_app_Dummy_Notify);
#endif
        if(INVALID_HANDLE==h_data_play)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play create failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_109_112_2_18_1_2_0_10,(uint8*)"");
            return AUDIO_ERROR;
        }
#ifndef WIN32
        h_data_record = AUDIO_CreateNormalHandle(hUplinkCodec, SCI_NULL, hPCMDev, SCI_NULL,
             audio_app_Dummy_Notify);
#endif
        if(INVALID_HANDLE==h_data_record)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record create failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_118_112_2_18_1_2_0_11,(uint8*)"");
            AUDIO_CloseHandle(h_data_play);
            return AUDIO_ERROR;
        }

        /* start audio */
        result = AUDIO_Play(h_data_play, 0);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play play failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_127_112_2_18_1_2_0_12,(uint8*)"d", result);
          AUDIO_CloseHandle(h_data_play);
          AUDIO_CloseHandle(h_data_record);
          return result;
        }

        result = AUDIO_Play(h_data_record, 0);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record play failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_136_112_2_18_1_2_0_13,(uint8*)"d", result);
          AUDIO_Stop(h_data_play);
          AUDIO_CloseHandle(h_data_play);
          AUDIO_CloseHandle(h_data_record);
          return result;
        }
    }
    else
    {
        result = AUDIO_Stop(h_data_play);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play stop failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_148_112_2_18_1_2_0_14,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_Stop(h_data_record);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record stop failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_154_112_2_18_1_2_0_15,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_CloseHandle(h_data_play);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play close failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_160_112_2_18_1_2_0_16,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_CloseHandle(h_data_record);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record close failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_166_112_2_18_1_2_0_17,(uint8*)"d", result);
          return result;
        }
        h_data_play = INVALID_HANDLE;
        h_data_record = INVALID_HANDLE;

        if(PNULL != ui_pcm_buffer )
        {
            SCI_FREE(ui_pcm_buffer);
        }
    }

    return result;
}

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
//!    BOOLEAN bOn,             //start/stop
//!    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
//!    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
//!    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
//!    )
//! \param bOn              start/stop
//! \param bwrite_to_flash  TRUE:change NV; FALSE: not change nv, active untill reopen phone.
//! \param bdump_to_log     TRUE:DUMP_TO_ARM_LOG  FALSE:DUMP_TO_FILE
//! \param uiposflag        uint32:32 posisions.bit_0:pos_0;bit_1:pos_1....bit_31:pos_31;
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is dynamic open/close dump pcm in audio.
//! \author Author: yaye.jiang
//! \note   Note: None.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
    BOOLEAN bOn,             //start/stop
    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
    )
{
    AUDMOD_RESULT_E result = AUDIO_NO_ERROR;
    AUDIO_NV_ARM_MODE_INFO_T    *ptmpMode = PNULL;
    char *mode_name = NULL;
    uint16 usNvConfigToFiles=0;

    ptmpMode = (AUDIO_NV_ARM_MODE_INFO_T *)SCI_ALLOCA(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptmpMode)
    {
        SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump, alloc fail, size: %d", sizeof(AUDIO_NV_ARM_MODE_INFO_T));

        return AUDIO_NO_ENOUGH_MEMORY;
    }
    mode_name = AUDIONVARM_GetAudioModeNameById(NV_AUDIO_ARM_HANDSFREE_E);
    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptmpMode)))
    {
        SCI_FREE(ptmpMode);
        SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump failed to get mode param.");
        return AUDIO_ERROR;
    }
SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump bOn=%d,uidump_save=%d,uidump_path=%d,uiposflag=0x%x",
    bOn, uidump_save, uidump_path, uiposflag);

SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump reserve[63]=0x%x,reserve[61]=0x%x",
    ptmpMode->tAudioNvArmModeStruct.reserve[63], ptmpMode->tAudioNvArmModeStruct.reserve[61]);

    usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[63];
    if(bOn)
    {
        //如果nv中pos对应bit有数据，则说明已经开启dump，此时输出方式不能修改
        if((usNvConfigToFiles&0x0FFF)||(ptmpMode->tAudioNvArmModeStruct.reserve[61]&0xFFFF))
        {
            AUD_DUMP_TO_FLAG_E dump_path = (usNvConfigToFiles&0x3000)>>12;

            if(uidump_path != dump_path)
            {
                SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump fail!! It already dumping to %d.", dump_path);
                SCI_FREE(ptmpMode);
                return AUDIO_PARAM_ERROR;
            }
        }
        else
        {
            if(DATA_DUMP_TO_ARM_LOG == uidump_path)
            {
                usNvConfigToFiles |= 0x1000;
            }
            else if(DATA_DUMP_TO_FILE == uidump_path)
            {
                usNvConfigToFiles &= 0xCFFF;
            }
        }
        usNvConfigToFiles |= (uiposflag&0x0FFF);

        ptmpMode->tAudioNvArmModeStruct.reserve[63] = usNvConfigToFiles;
        if(uiposflag>>12)
        {
            usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[61];
            usNvConfigToFiles |= (uiposflag>>12);
            ptmpMode->tAudioNvArmModeStruct.reserve[61] = usNvConfigToFiles;
        }
        AUDIONVARM_SetModeParam(mode_name, ptmpMode);

        Datadump_Enable(bOn, uiposflag);
    }
    else
    {
        Datadump_Enable(bOn, uiposflag);
        //if(bwrite_to_flash)
        {
            usNvConfigToFiles &= ~(uiposflag&0x0FFF);
            ptmpMode->tAudioNvArmModeStruct.reserve[63] = usNvConfigToFiles;
            if(uiposflag>>12)
            {
                usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[61];
                usNvConfigToFiles &= ~(uiposflag>>12);
                ptmpMode->tAudioNvArmModeStruct.reserve[61] = usNvConfigToFiles;
            }
            AUDIONVARM_SetModeParam(mode_name, ptmpMode);
        }
    }


SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump reserve[63]=0x%x,reserve[61]=0x%x",
    ptmpMode->tAudioNvArmModeStruct.reserve[63], ptmpMode->tAudioNvArmModeStruct.reserve[61]);
    if(DUMP_SAVE_TO_FLASH == uidump_save)
    {
        result = AUDIONVARM_WriteModeParamToFlash(mode_name, ptmpMode);
    }

    SCI_FREE(ptmpMode);
    return result;
}


PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_EnableVoiceCodec(
	AUDIO_VOLTE_COMMAND_T *ptCommand)
{
	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
#if 0
	static DSPDATA_EXT_OPER_START_PARA_T tPara = {0};
	static SCI_MUTEX_PTR volte_mutex=NULL;
	static HAUDIO hVoiceCall = NULL;
	static uint32 uiEnableFlag = 0;
	static BOOLEAN bVoiceProcess = FALSE;
	static DSPDATA_EXT_OPER_PARA_SET_T tToParaSet = {0};

	if (!ptCommand) {
		SCI_TRACE_LOW(("AUDIO_VOLTE_EnableVoiceCodec ptCommand error"));
		return AUDIO_PARAM_ERROR;
	}
	if (SCI_NULL == volte_mutex) {
		volte_mutex = SCI_CreateMutex( "volte_mutex",SCI_INHERIT);
		if (SCI_NULL == volte_mutex) {
		SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec error to create mutex");
		}
	}

	if (volte_mutex) {
		SCI_GetMutex(volte_mutex, SCI_WAIT_FOREVER);
	}

	switch(ptCommand->eCommand) {
	case AUDIO_VOLTE_INIT:
	{
		AUDIO_VOLTE_COMMAND_PARA_INIT_T *ptInit =
			(AUDIO_VOLTE_COMMAND_PARA_INIT_T *)&(ptCommand->uPara.tInit);
		if (INVALID_HANDLE == s_volte_audiohandle) {
			SCI_ASSERT(ptInit!=NULL);
			SCI_MEMSET((void *)&tPara, 0, sizeof(DSPDATA_EXT_OPER_START_PARA_T));
#ifndef WIN32
			s_volte_audiohandle=AUDIO_CreateNormalHandle(hDspDataCodec,
				SCI_NULL, hAUDDEVDUAL, SCI_NULL, audio_app_Dummy_Notify);
#endif
			tPara.hAudioHandle = s_volte_audiohandle;
			tPara.para0 = 3;
			tPara.readdataClkFunc = ptInit->readdataClkFunc;
			tPara.writedataClkFunc = ptInit->writedataClkFunc;
			SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec:init: 0x%x",
				s_volte_audiohandle);
		} else {
			SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: 0x%x, inited.",
				s_volte_audiohandle);
		}

		if (NULL==hVoiceCall) {
			hVoiceCall = AUDIO_CreateNormalHandle(hVoiceCodec, SCI_NULL, hAUDDEVDUAL,
				SCI_NULL, audio_app_Dummy_Notify);
			SCI_ASSERT(NULL != hVoiceCall);/*assert verified*/
	}
		}
		break;
	case AUDIO_VOLTE_START:
	{
		BOOLEAN bNormalCallNeedEnable = FALSE;
		BOOLEAN bVolteCallNeedEnable = FALSE;
		AUDIO_VOLTE_COMMAND_PARA_ENABLE_T *ptEnable =
			&(ptCommand->uPara.tEnableFlag);
		AUDIO_VOLTE_COMMAND_PARA_PARASET_T tParaSet = ptCommand->uPara.tParaSet;

		tToParaSet.uiformat = tParaSet.uiformat;
		tToParaSet.uiFrameTypeIndex = tParaSet.uiFrameTypeIndex;
		tToParaSet.dtxFlag = tParaSet.dtxFlag;
		tToParaSet.hAudioHandle = s_volte_audiohandle;
		tToParaSet.para0 = 3;
		tToParaSet.evsMode = tParaSet.evsMode;
		tToParaSet.evsSidInteval = tParaSet.evsSidInteval;
		tToParaSet.sampleRate = tParaSet.sampleRate;
		tPara.other = &tToParaSet;

		if ((ptEnable->eEnableFlag & NORMAL_CALL_ENABLE) &&
			(!(uiEnableFlag&NORMAL_CALL_ENABLE))) {
			SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: start 1.");
			bNormalCallNeedEnable = TRUE;
		}
		if ((ptEnable->eEnableFlag & VOLTE_CALL_ENABLE) &&
			(!(uiEnableFlag&VOLTE_CALL_ENABLE))) {
			SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: start 2.");
			bVolteCallNeedEnable = TRUE;
		}

		SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: start 3 %d, %d, %d, 0x%x,"
			"0x%x.", bNormalCallNeedEnable, bVolteCallNeedEnable, bVoiceProcess,
			uiEnableFlag, ptEnable->eEnableFlag);

		if (bNormalCallNeedEnable || bVolteCallNeedEnable) {
			if (!bVoiceProcess) {
				AUD_EnableVoiceProcessConfig(TRUE, FALSE, NULL);
				bVoiceProcess = TRUE;
			}
			if (bNormalCallNeedEnable && (hVoiceCall != INVALID_HANDLE)) {
				AUDIO_Play (hVoiceCall, 0);
				uiEnableFlag |= NORMAL_CALL_ENABLE;
			}
			if (bVolteCallNeedEnable && (s_volte_audiohandle != INVALID_HANDLE)) {
				tPara.para1 = 1;
				AUDIO_ExeCodecExtOpe(
					s_volte_audiohandle,
					ANSI2UINT16("DSPDATA_EXT_VOLTE_START"),
					DSPDATACODEC_ADP_EXT_OPE_PLAY_PARA_COUNT,
					&tPara,
					&result);
				uiEnableFlag |= VOLTE_CALL_ENABLE;
			}
		}
	}
		break;
	case AUDIO_VOLTE_STOP:
	{
		BOOLEAN bNormalCallNeedDisable = FALSE;
		BOOLEAN bVolteCallNeedDisable = FALSE;
		DSPDATA_EXT_OPER_STOP_PARA_T tStopPara = {0};
		AUDIO_VOLTE_COMMAND_PARA_ENABLE_T *ptEnable =
			&(ptCommand->uPara.tEnableFlag);
		SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: stop 1 %d, 0x%x, 0x%x.",
			bVoiceProcess, uiEnableFlag, ptEnable->eEnableFlag);
		if ((ptEnable->eEnableFlag & NORMAL_CALL_ENABLE) &&
			(uiEnableFlag&NORMAL_CALL_ENABLE)) {
			bNormalCallNeedDisable = TRUE;
		}
		if ((ptEnable->eEnableFlag & VOLTE_CALL_ENABLE) &&
			(uiEnableFlag&VOLTE_CALL_ENABLE)) {
			bVolteCallNeedDisable = TRUE;
		}

		SCI_TRACE_LOW("AUDIO_VOLTE_EnableVoiceCodec: stop 2 %d, %d, 0x%x.",
			bNormalCallNeedDisable, bVolteCallNeedDisable, hVoiceCall);

		if (bNormalCallNeedDisable && (hVoiceCall != INVALID_HANDLE)) {
			AUDIO_Stop (hVoiceCall);
			uiEnableFlag &= (~NORMAL_CALL_ENABLE);
		}

		if (bVolteCallNeedDisable && (s_volte_audiohandle != INVALID_HANDLE)) {
			tStopPara.hAudioHandle = s_volte_audiohandle;
			tStopPara.para0 = 3;
			tStopPara.para1 = 0;

			AUDIO_ExeCodecExtOpe(s_volte_audiohandle,
				ANSI2UINT16("DSPDATA_EXT_VOLTE_STOP"),
				0,
				&tStopPara,
				&result);
			uiEnableFlag &= (~VOLTE_CALL_ENABLE);
		}

		if ((!uiEnableFlag) && bVoiceProcess) {
			AUD_EnableVoiceProcessConfig(FALSE, FALSE, NULL);
			bVoiceProcess = FALSE;
		}
	}
		break;
	case AUDIO_VOLTE_DEINIT:
	{
		if (INVALID_HANDLE != s_volte_audiohandle) {
			AUDIO_CloseHandle(s_volte_audiohandle);
			s_volte_audiohandle = INVALID_HANDLE;
		}
	}
		break;
	case AUDIO_VOLTE_PARASET:
	{
		if (INVALID_HANDLE != s_volte_audiohandle) {
			AUDIO_VOLTE_COMMAND_PARA_PARASET_T tParaSet =
				ptCommand->uPara.tParaSet;
			tToParaSet.uiformat = tParaSet.uiformat;
			tToParaSet.uiFrameTypeIndex = tParaSet.uiFrameTypeIndex;
			tToParaSet.dtxFlag = tParaSet.dtxFlag;
			tToParaSet.hAudioHandle = s_volte_audiohandle;
			tToParaSet.para0 = 3;
			tToParaSet.evsMode = tParaSet.evsMode;
			tToParaSet.evsSidInteval = tParaSet.evsSidInteval;
			tToParaSet.sampleRate = tParaSet.sampleRate;

			AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec PARASET para: "
				"index:%d, format:%d!", tParaSet.uiFrameTypeIndex, tParaSet.uiformat));

			AUDIO_ExeCodecExtOpe(s_volte_audiohandle,
				ANSI2UINT16("DSPDATA_EXT_VOLTE_PARASET"),
				0,
				&tToParaSet,
				&result
			);
		} else {
			AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec "
				"PARASET handle is invalid!"));
		}
	}
		break;
	case AUDIO_VOLTE_MIX:
	{
		if (INVALID_HANDLE != s_volte_audiohandle) {
			AUDIO_VOLTE_COMMAND_PARA_MIX_T tParaMix = ptCommand->uPara.tParaMix;
			DSPDATA_EXT_OPER_PARA_MIX_T tToParaMix = {0};
			uint32 i =0;

		for (i = 0; i<MAX_ADSP_AUDIO_PATH_NUM; i++) {
			tToParaMix.mix_ctrl[i] = tParaMix.mixCtrl[i];
			tToParaMix.dtxFlag[i] = tParaMix.para[i].dtxFlag;
			tToParaMix.rate_reg[i] = tParaMix.para[i].uiFrameTypeIndex;
			tToParaMix.coder_type[i] = tParaMix.para[i].uiformat;
			AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec mix para[%d]: "
				"mix_ctrl:%d, dtxFlag:%d, rate_reg:%d, coder_type:%d",
				i, tToParaMix.mix_ctrl[i], tToParaMix.dtxFlag[i], tToParaMix.rate_reg[i],
				tToParaMix.coder_type[i]));
		}

			tToParaMix.hAudioHandle = s_volte_audiohandle;
			tToParaMix.para0 = 3;
			tToParaMix.audioPathNum = tParaMix.audioPathNum;

			AUDIO_ExeCodecExtOpe(s_volte_audiohandle,
				ANSI2UINT16("DSPDATA_EXT_VOLTE_MIX"),
				0,
				&tToParaMix,
				&result);
		}else{
		AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec  PARASET handle is invalid!"));
		}
	}
		break;
	default:
		AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec  command is invalid:%d.",
			ptCommand->eCommand));
	break;
	}

	AUDIO_PRINT_APP(("AUDIO_VOLTE_EnableVoiceCodec command:%d",
		ptCommand->eCommand));

	if (volte_mutex) {
		SCI_PutMutex(volte_mutex);
	}
	return result;
}

PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_Write_Data(uint16 *buff, uint32 buff_size)
{
	DSPDATA_EXT_OPER_COMMON_T tCommon;
	AUDIO_RESULT_E result = AUDIO_NO_ERROR;

	if ( s_volte_audiohandle == 0 || buff == SCI_NULL || buff_size ==0) {
		return AUDIO_PARAM_ERROR;
	}

	tCommon.hAudioHandle = s_volte_audiohandle;
	tCommon.para0 = (uint32)buff;
	tCommon.para1 = buff_size;
	AUDIO_ExeCodecExtOpe(s_volte_audiohandle,
		ANSI2UINT16("DSPDATA_EXT_VOLTE_WRITE"),
		0,
		&tCommon,
		&result);
#endif
	return result;
}

PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_Read_Data(uint16 *buff, uint32 *buff_size)
{

	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    #if 0
	DSPDATA_EXT_OPER_COMMON_T tCommon;

	if ( s_volte_audiohandle == 0 || buff == SCI_NULL || buff_size ==0) {
		return AUDIO_PARAM_ERROR;
	}

	tCommon.hAudioHandle = s_volte_audiohandle;
	tCommon.para0 = (uint32)buff;
	tCommon.para1 = (uint32)buff_size;
	AUDIO_ExeCodecExtOpe(s_volte_audiohandle,
		ANSI2UINT16("DSPDATA_EXT_VOLTE_READ"),
		0,
		&tCommon,
		&result);
    #endif
	return result;
}

PUBLIC void AUDIO_EnableTxClarity(BOOLEAN bEnable)
{
    s_tx_clarity_enable = bEnable;
}

PUBLIC BOOLEAN AUDIO_GetTxClarityEnable(void)
{
    return s_tx_clarity_enable;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of audio_api.c

