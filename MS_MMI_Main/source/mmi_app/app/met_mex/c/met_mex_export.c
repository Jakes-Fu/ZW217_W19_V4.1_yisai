
#ifdef MET_MEX_SUPPORT
#include "mmimp3_export.h"
#include "mmisms_app.h"
#include "mmisms_save.h"
#include "mmicc_export.h"
#include "chg_drvapi.h"
#include "met_mex_aq.h"
#include "mexlib.h"
#include "met_mex_export.h"
#include "met_mex_app_export.h"

#ifdef __cplusplus
extern   "C"
{
#endif

PUBLIC void MMIMEX_RecordStop(void)
{
	mex_voice_record_stop();//关闭Mex录音
}

PUBLIC BOOLEAN MMIMEX_ConflictPromt(void)
{
	return MetMex_ConflictPromt();
}

PUBLIC BOOLEAN MMIMEX_IsOutCalling(void)
{
	return MexPhs_IsOutCalling();
}

PUBLIC BOOLEAN MMIMEX_IsSysRunning(void)
{
	return MexSystem_IsSysRunning();
}

PUBLIC BOOLEAN MMIMEX_IsSysKernalRunning(void)
{
	return MexSystem_IsSysKernalRunning();
}

PUBLIC BOOLEAN MMIMEX_ShowPlatformEngineerMode(void)
{
    return MetMex_ShowPlatformEngineerMode();
}


PUBLIC void MMIMEX_SmsReceivePromt(void)
{
    MetMex_SmsReceivePromt();
}

PUBLIC void MMIMEX_OutCallEndedCB(void *info)
{
    MexPhs_OutCallEndedCB(info);
}

PUBLIC BOOLEAN MMIMEX_DelSmsRsp(int32 mod, int16 result)
{
	uint32 isProcessed = 0;
	MexPhs_DelSmsRsp(&isProcessed, mod, result);
	return isProcessed;
}

PUBLIC void MMIMEX_PhsOutCallConnectedCB(void *info)
{
	if(MexPhs_IsOutCalling())
	{
	   MexPhs_OutCallConnectedCB(info);
	}
}

PUBLIC void MMIMEX_PhsOutCallEventCB(void *info)
{
	if(MexPhs_IsOutCalling())
	{
	   MexPhs_OutCallEventCB(info);
	}
} 

PUBLIC void MMIMEX_PhsOutCallDroped(void *info)
{
    if(MexPhs_IsOutCalling())
    {
        MexPhs_NetworkCallDropped(info);
    }
}
PUBLIC void MMIMEX_PhsSetSmsOperStatus(void)
{
	MMISMS_OPER_STATUS_E uiOperStatus = (MMISMS_OPER_STATUS_E)MexPhs_GetSaveSmsBox();
	if(uiOperStatus != MMISMS_NO_OPER)
	   {
		   MMISMS_SetOperStatus(uiOperStatus);
	   }
}


PUBLIC void MMIMEX_ReadSmsContentCB(void *data, int32 mod, int16 result)
{

  MexPhs_ReadSmsContentCB(data, mod, result);
}

PUBLIC uint32 MMIMEX_GetSaveSmsBox(void)
{
	return MexPhs_GetSaveSmsBox();
}
PUBLIC void MMIMEX_SaveSmsCB(void *data, int32 mod, int16 result)
{
	MexPhs_SaveSmsCB(data,  mod,  result);
}
PUBLIC void MMIMEX_IsEarphonePlugIn(bool bIsPlugIn)
{
	MexMix_IsEarphonePlugIn(bIsPlugIn);
}

PUBLIC BOOLEAN MMIMEX_SendSmsCB(uint16 fail_count)
{
    if(MexPhs_IsSmsSend())
    {
        MexPhs_SmsSendRsp(NULL,0,((fail_count == 0) ? 1 : 0));
        return TRUE;
    }    
    else
    {
        return FALSE;
    }
}

PUBLIC BOOLEAN MMIMEX_PhsSaveSmsCB(APP_MN_WRITE_SMS_CNF_T *sig_ptr,
											MMISMS_SEND_T  *send_info_ptr
											 )
{
	if(MexPhs_GetSaveSmsBox() == MMISMS_SEND_SAVE_SMS)
    { 			   
        MMISMS_SaveInfoToOrderByResult(
                    sig_ptr->dual_sys,
                    sig_ptr->storage, 
                    sig_ptr->record_id,
                    MN_SMS_MO_SR_NOT_REQUEST, 
                    TRUE,
                    MMISMS_FOLDER_NORMAL, 
                    MMISMS_SEND_SAVE_SMS,
                    send_info_ptr, 
                    MMISMS_SENDSTATUS_SENDSUCC
                    );

      MexPhs_SaveSmsCB(NULL,0,sig_ptr->cause);
      return TRUE;
    }
			  else
			{
			return FALSE;
			}
}
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)

PUBLIC void MMIMEX_AQIPPhNum(MMI_PARTY_NUMBER_T party_num,
											CC_CALL_CONTEXT_T *p_content, 
											uint8 copy_len,
											uint8 *tele_num,
											uint8 *p_tele_len)
{			
	uint8 tele_num_out[CC_MAX_TELE_NUM_LEN + 1] = {0};
	
	if(MexInterface_aq_CheckIPPhone(tele_num,tele_num_out,CC_MAX_TELE_NUM_LEN + 1)) // 是否需要加拨
	{
		*p_tele_len = strlen((const char*)tele_num_out);
		if( TRUE == MMIAPICOM_GenPartyNumber(tele_num_out, *p_tele_len, &party_num))	// 号码处理
		{
			strcpy((char*)tele_num,(const char *)tele_num_out);
			p_content->want_mo_call.address.number_type = party_num.number_type ;
			
			SCI_MEMSET((void*)p_content->want_mo_call.address.party_num, 0xFF, sizeof(p_content->want_mo_call.address.party_num));
			
			copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
			MMI_MEMCPY(
				(void*)p_content->want_mo_call.address.party_num,
				sizeof(p_content->want_mo_call.address.party_num),
				(void*)party_num.bcd_num,
				sizeof(party_num.bcd_num),
				copy_len
				);
			p_content->want_mo_call.address.num_len = copy_len ;
		}
		else
		{
			*p_tele_len = strlen((const char*)tele_num);
		}
	}
}

PUBLIC int32 MMIMEX_AQPhoneWall(MN_DUAL_SYS_E dual_sys,MN_CALLING_NUMBER_T *calling_num)
{
    return MexInterface_aq_PhoneWall(dual_sys, calling_num);
}

PUBLIC BOOLEAN MMIMEX_AQCCFindLocalInfo(uint8 *origin_number_arr,	
									    int16  origin_number_len ,	
										MMI_STRING_T* mmi_str,
									    uint16* str
										)
{   
	char	temp_asc[50];
    uint16*  temp_ptr = PNULL;
    uint16  str_length = 0;
    MMI_STRING_T	temp_mmi_str = {0};

	if(met_aq_showLocationStatus())
	{
		strncpy((int8 *)temp_asc,(int8 *)origin_number_arr,origin_number_len);
		met_mex_aq_GetAttachedAreaNameAndReset((char *)(temp_asc));
		  
		if(met_mex_aq_NumInq_App_Flag==1)
		{
			str_length = MMIAPICOM_Wstrlen((const wchar *) met_mex_aq_strCityNameUnicode);

			if(str_length > 0 )
			{
				MMI_GetLabelTextByLang( TXT_GUISHUDI, &temp_mmi_str );
				MMIAPICOM_Wstrncpy( str, temp_mmi_str.wstr_ptr, temp_mmi_str.wstr_len );
				mmi_str->wstr_len = temp_mmi_str.wstr_len;
				{				
					temp_ptr = str;
					temp_ptr += temp_mmi_str.wstr_len;
					//增加“：”
					{
						wchar colon_str[] = {':', 0};
						MMIAPICOM_Wstrncpy( temp_ptr, colon_str, 1 );
						temp_ptr += 1;
						mmi_str->wstr_len += 1;
					}		 
					MMIAPICOM_Wstrncpy( temp_ptr, (const wchar*)met_mex_aq_strCityNameUnicode, str_length );
					mmi_str->wstr_len += str_length;
				}
				mmi_str->wstr_ptr = str;
				return TRUE;
			}
		}
	}
	return FALSE;

}

PUBLIC int32 MMIMEX_CheckSetDivertCall(int32 is_true)
{

	return MexInterface_CheckSetDivertCall(is_true);

}

PUBLIC int32 MMIMEX_CheckCancelDivertCall(int32 is_true)
{
    return MexInterface_CheckCancelDivertCall(is_true);
}

PUBLIC int32 MMIMEX_CheckQueryDivertCall(int32 is_true,int32 status)
{
    return MexInterface_CheckQueryDivertCall(is_true, status);
}

#endif

//============== MET MEX  start ===================
#define MMIAUDIO_TRACKBUFSIZE_PLAYER    (80 * 150)
extern uint32 VBDecodeOuputBuf[];

uint32* MetMex_GetVBDecodeBuf(void)
{
	return VBDecodeOuputBuf;
}

uint32 MetMex_GetVBDecodeBufSize(void)
{
	return MMIAUDIO_TRACKBUFSIZE_PLAYER;
}

//============== MET MEX  end ===================


////------MP3-------------------------------------//
//*********************   移植注意:  ********************************/
//此处为MP3播放支持，请确认其MP3播放是否有所改动!!
//若对方有所改动，则需要对方支持提供相应接口。
//并将对应接口填充到下面的接口中。
//此段代码位置:
//*********************   移植注意********************************/
/*播放按键*/
PUBLIC void Mex_AudPly_play_handle(void)
{
    switch(MMIAPIMP3_GetPlayerState())
	{
	case MMIMP3_PLAYER_STATE_NONE:
	    MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
	    break;
	case MMIMP3_PLAYER_AUDIO_ACTIVE:
	    {
	        switch(MMIMP3_GetAudioState())
		        {
		        case MMIMP3_AUDIO_OPENED:
		            break;

		        case MMIMP3_AUDIO_PLAYING:
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
			MMIMP3_PauseMp3(TRUE);
#else
			MMIAPIMP3_PauseAudioPlayer(TRUE);
#endif
		            break;

		        case MMIMP3_AUDIO_PAUSED:
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
			MMIMP3_ResumeMp3(TRUE);
#else
			MMIAPIMP3_ResumeAudioPlayer(TRUE);
#endif
		            break;
		        
		        case MMIMP3_AUDIO_STOPPED:
		            break;

		        case MMIMP3_AUDIO_CLOSED:
		            break;

		        default:
		            break;
		        }
	    }
	    break;

	case MMIMP3_PLAYER_MUSIC_SWITCH:
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
		MMIMP3_PauseMp3(TRUE);
#else	
		MMIAPIMP3_PauseAudioPlayer(TRUE);
#endif
	    break;

	case MMIMP3_PLAYER_SWITCH_PAUSED:
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
	MMIMP3_ResumeMp3(TRUE);
#else
	MMIAPIMP3_ResumeAudioPlayer(TRUE);
#endif
	    break;

	default:
	    break;
	}
   

}
/*停止按键*/
PUBLIC void Mex_AudPly_stop_handle(void)
{
    MMIAPIMP3_StopAudioPlayer();
}
/*下一曲按键*/
PUBLIC void Mex_AudPly_next_handle(void)
{
    MMIAPIMP3_GoToPreOrNextMp3(FALSE);
}
/*上一曲按键*/
PUBLIC void Mex_AudPly_prev_handle(void)
{
    MMIAPIMP3_GoToPreOrNextMp3(TRUE);
}
/*增大音量按键*/
PUBLIC void Mex_AudPly_inc_volume_handle(void)
{   
    MMIAPIMP3_SetMp3Volume(TRUE);
}
/*减小音量按键*/
PUBLIC void Mex_AudPly_dec_volume_handle(void)
{
	MMIAPIMP3_SetMp3Volume(FALSE);
}
/*当前是否处于停止播放状态*/
PUBLIC BOOLEAN Mex_Func_get_audply_is_stoped(void)
{
   return MMIAPIMP3_IsPlayerStopped();
}

///////////////////////////////////////////////////


PUBLIC int MMIMex_Phs_HookResult(uint16 msgId, void* MsgStruct, int mod_src, void* peerBuff, int msg_from)
{
   return MexPhs_HookResult(msgId, MsgStruct,  mod_src, peerBuff, msg_from);
}

PUBLIC int32 MMIMex_Aq_SmsWall(MN_DUAL_SYS_E dual_sys,MN_CALLED_NUMBER_T* orig_address_ptr)
{
   return MexInterface_aq_SmsWall(dual_sys,orig_address_ptr);
}


PUBLIC uint32 MEX_CHR_CheckBatteryStatus (void)
{
	CHGMNG_STATE_INFO_T*  p_chgmng_info = CHGMNG_GetModuleState();
	/*
	if (!ischgmng_start)
		{ 
			return CHR_BATTERY_NONE_S;  
		}*/
   switch(p_chgmng_info->chgmng_state)
	{
	   case CHGMNG_IDLE:
	        case CHGMNG_STARTING:
	        case CHGMNG_STOPPING:
	            return CHR_BATTERY_NORMAL_S;

	        case CHGMNG_CHARGING:
	        case CHGMNG_PULSECHARGING:
	            return CHR_BATTERY_CHARGING_S;

	        default:  // the state is not defined.
	            SCI_PASSERT (SCI_FALSE, ("CHGMNG: Current state is not defined."));/*assert to do*/
	            return CHR_BATTERY_NORMAL_S; /*lint !e527 comfirmed by xuepeng*/ 

	} 
}



#ifdef   __cplusplus
}
#endif

#endif   /* MET_MEX_SUPPORT */


