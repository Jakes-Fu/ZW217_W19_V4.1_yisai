 /****************************************************************************
** File Name:      ejTTSPlayer_api.c                                                 *
** Author:                                                         *
** Date:           08/06/2011                                             *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/06/2011  tonny.chen         Create                                  *
**                                                                         *
****************************************************************************/
#include "mmi_app_tts_trc.h"
#ifdef TTS_SUPPORT_EJ
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"      
#include "mmitts_export.h"
#include "mmicc_export.h"
#include "ejTTSPlayer_audio.h"
#include "ejTTSPlayer_api.h"
#ifdef MMI_READMENU_ENABLE
#include "mmienvset_export.h"
#include "mmieng_export.h"
#endif
#include "mmisms_id.h"
#include "mmipub.h"
#include "mmiudisk_export.h"

#ifdef TTS_SUPPORT_EJ_ROM
//#include "ejres_Standard_LE_EN_16k_Cameal.h"
//#include "ejres_Standard_LE_CN_16k_ZhangNan.h"
//#include "ejres_Basic_LE_EN_8K_ZhangNan.h"
//#include "ejres_Basic_LE_CN_8k_ZhangNan_GBK.h"
#include "ejres_Basic_LE_CN_8k_ZhangNan_Unicode.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
	
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define EJ_ENG_SUPPORT
#ifdef TTS_PLAYER_DEBUG
#define  JTTS_LOG SCI_TRACE_LOW						//打Log的宏
#else
#define  JTTS_LOG SCI_TRACE_LOW
#endif

#define TTS_PLAYER_MAX_TEXTLEN	(1024 + 2)
#define TTS_DELAY_TIME              30

#define TTS_DEFAULT_DIR               L"Tts"
#define TTS_DEFAULT_DIR_LEN           3

#define TTS_RESOURCE_FILENAME         L"CNPackage.dat"  //存放JT TTS中文库文件名
#define TTS_RESOURCE_FILENAME_LEN     13

#define TTS_EN_RESOURCE_FILENAME         L"ENPackage.dat"  //存放JT TTS英文库文件名
#define TTS_EN_RESOURCE_FILENAME_LEN     13

#define TTS_DM_RESOURCE_FILENAME         L"DMPackage.dat"  //存放JT TTS定制库文件名
#define TTS_DM_RESOURCE_FILENAME_LEN     13
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint16                  tts_vol;
	MMISRVAUD_VOLUME_TYPE_E volume_type;
    TTS_ALL_RING_TYPE_E     ring_type;
    char                    szTextIn[TTS_PLAYER_MAX_TEXTLEN];
    uint8                   timer_id;
    BOOLEAN                 is_prompt_when_err;
    MMI_HANDLE_T			win_handle;
	MMISRVAUD_ROUTE_T       tts_route;
	MMITTS_DIGIT_MODE_TYPE_E  digit_mode;
	MMITTS_PUNC_MODE_TYPE_E   punc_mode;	
} TTSINFO_EXT_T;


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL TTSINFO_EXT_T s_ttsinfo = {0};
						
//LOCAL const wchar CN_Package[] = L"E:\\CNPackage.dat";//中文音库
//#ifdef EJ_ENG_SUPPORT
//LOCAL const wchar EN_Package[] = L"E:\\ENPackage.dat";//英文音库
// #endif


LOCAL BOOLEAN				s_is_manual_stop = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  HandleAudioEnv
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleAudioEnv(BOOLEAN is_begin);

/*****************************************************************************/
//  Description :  JTTS_PlayCallback
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL jtErrCode JTTS_PlayCallback(
                                  long uMessage,                // [in] Message
                                  long lParam,                  // [in] parameter 
                                  unsigned long wParam);         // [in] parameter 


/*****************************************************************************/
//  Description : HandlePlayTextTimer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayTextTimer(uint8 timer_id, uint32 param);


/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSENResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSDMResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      );

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/	
PUBLIC BOOLEAN TTSAPI_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr)
{    
    uint16 str_len = 0, copy_times = 0;
    char* pszTextIn = s_ttsinfo.szTextIn;

	if (PNULL == play_param_ptr)
	{
		return FALSE;
	}

	//JTTS_LOG:"TTSAPI_PlayText enter txt_len_in_byte = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_140_112_2_18_3_3_38_6,(uint8*)"d",play_param_ptr->txt_len_in_byte);
	
	TTS_SetTTSRingType(play_param_ptr->ring_type);	
    if(play_param_ptr->txt_addr==PNULL || play_param_ptr->txt_len_in_byte==0)   //如果数据无效,直接返回
    {        
		if (TTSAPI_IsPromptWhenErr())
        {
            MMIPUB_OpenAlertWarningWin(TXT_TTS_NO_SPEAK_CONTENT);
        }
        return FALSE;
    }  
    else if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {        
        if (TTSAPI_IsPromptWhenErr())
        {
            MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        }
        return FALSE;            
    }
#ifdef TTS_FILE_RES
    else if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        if (TTSAPI_IsPromptWhenErr())    
        {            
            MMIPUB_OpenAlertWarningWin(TXT_TTS_UDISK_USING);
        }
        return FALSE;
    }
    else if(!TTSAPI_checkResFile())
    {       
        if (TTSAPI_IsPromptWhenErr())
        {
            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
        }
        return FALSE;
    }
#endif
    SCI_MEMSET(pszTextIn, 0, TTS_PLAYER_MAX_TEXTLEN);
	SCI_MEMSET(&s_ttsinfo,0,sizeof( TTSINFO_EXT_T));
    if (!play_param_ptr->is_ucs2)
    {
        play_param_ptr->txt_len_in_byte *= 2;
    }
    play_param_ptr->txt_len_in_byte = MIN(play_param_ptr->txt_len_in_byte, TTS_PLAYER_MAX_TEXTLEN - 2);

    do
    {
		
        if (play_param_ptr->is_ucs2)
        {
            MMI_MEMCPY(pszTextIn + str_len, TTS_PLAYER_MAX_TEXTLEN - 2 - str_len, 
                play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, 
                play_param_ptr->txt_len_in_byte);
        }
        else
        {
            int32 i = 0;
            for (i = 0; i < play_param_ptr->txt_len_in_byte; i ++)
            {
                if (1 == (i % 2))
                    *(pszTextIn + str_len + i) = play_param_ptr->txt_addr[i / 2];
            }
        }
        str_len += MIN(play_param_ptr->txt_len_in_byte, TTS_PLAYER_MAX_TEXTLEN - 2 - str_len);
        copy_times ++;
        if (play_param_ptr->play_times == copy_times) break;
    } while (TTS_PLAYER_MAX_TEXTLEN - 2 - str_len >= play_param_ptr->txt_len_in_byte);
    s_ttsinfo.win_handle = play_param_ptr->win_handle;

	//JTTS_LOG:"TTSAPI_PlayText ring_type = %d, tts_vol = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_209_112_2_18_3_3_38_8,(uint8*)"dd",play_param_ptr->ring_type,play_param_ptr->tts_vol);
	TTS_SetTTSRingType(play_param_ptr->ring_type);
	TTS_SetTTSVolume(play_param_ptr->tts_vol);    
	s_ttsinfo.tts_route = play_param_ptr->tts_route;
	s_ttsinfo.digit_mode = play_param_ptr->digit_mode;
	s_ttsinfo.punc_mode = play_param_ptr->punc_mode;
	s_ttsinfo.volume_type = play_param_ptr->volume_type;
	if (s_ttsinfo.timer_id > 0)
    {
        MMK_StopTimer(s_ttsinfo.timer_id);
        s_ttsinfo.timer_id = 0;
    }
	//JTTS_LOG:"TTSAPI_PlayText enter stop jttsplayer"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_217_112_2_18_3_3_38_9,(uint8*)"");
	TTS_SetManualStop(TRUE);
	ejTTSPlayer_Stop(TTS_PLAYER_STOP_ASYNC);
    s_ttsinfo.timer_id = MMK_CreateTimerCallback(  TTS_DELAY_TIME, HandlePlayTextTimer, NULL, FALSE);
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      )
#ifdef TTS_SUPPORT_EJ_ROM
{    
    return TRUE; 
}
#else
{
    BOOLEAN         result      =   FALSE;    
    
    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)    
    {    
        result = MMIAPIFMM_GetSysFileFullPathForRead(
            TRUE,
            TTS_DEFAULT_DIR,
            TTS_DEFAULT_DIR_LEN,
            TTS_RESOURCE_FILENAME,
            TTS_RESOURCE_FILENAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }
    //JTTS_LOG:" GetTTSResouceFileFullName result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_246_112_2_18_3_3_38_10,(uint8*)"d",result);
    return result; 
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSENResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      )
#ifdef TTS_SUPPORT_EJ_ROM
{    
    return TRUE; 
}
#else
{
    BOOLEAN         result      =   FALSE;    
    
    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)    
    {    
        result = MMIAPIFMM_GetSysFileFullPathForRead(
            TRUE,
            TTS_DEFAULT_DIR,
            TTS_DEFAULT_DIR_LEN,
            TTS_EN_RESOURCE_FILENAME,
            TTS_EN_RESOURCE_FILENAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }

    //JTTS_LOG:" GetTTSENResouceFileFullName result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_275_112_2_18_3_3_38_11,(uint8*)"d",result);
    return result; 
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTTSDMResouceFileFullName(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      )
{
    BOOLEAN         result      =   FALSE;    
    
    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)    
    {    
        result = MMIAPIFMM_GetSysFileFullPathForRead(
            TRUE,
            TTS_DEFAULT_DIR,
            TTS_DEFAULT_DIR_LEN,
            TTS_DM_RESOURCE_FILENAME,
            TTS_DM_RESOURCE_FILENAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }

    SCI_TRACE_LOW("GetTTSDMResouceFileFullName result = %d",result);
    
    return result; 
}

/*****************************************************************************/
//  Description : HandlePlayTextTimer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayTextTimer(uint8 timer_id, uint32 param)
{
    
	int32 status = 0;
	wchar                       abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#ifdef EJ_ENG_SUPPORT
	wchar                       en_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      en_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#endif
	wchar                       dm_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      dm_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;

	s_ttsinfo.timer_id = 0;
    //JTTS_LOG:"MMITTS_HandlePlayTextTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_294_112_2_18_3_3_38_12,(uint8*)"");
    if (TTS_IsCommandStatusNone())
    {
        
        ejTTSPlayer_GetStatus((int*)(&status));
        //JTTS_LOG:"MMITTS_HandlePlayTextTimer | ejTTSPlayer_GetStatus = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_299_112_2_18_3_3_38_13,(uint8*)"d", status);
        if (TTS_PLAYER_NOT_INIT == status)
        {
			if(!GetTTSResouceFileFullName(abs_default_path,&abs_default_path_length)
#ifdef EJ_ENG_SUPPORT
				|| !GetTTSENResouceFileFullName(en_abs_default_path,&en_abs_default_path_length)
#endif
#ifdef TTS_SUPPORT_EJ_ROM
				|| (TTS_PLAYER_ERR_NONE != ejTTSPlayer_Init((const char*)g_cn_package, 
	#ifdef EJ_ENG_SUPPORT
					NULL,//(const char*)g_en_package, 
	#else
					NULL,
	#endif
					NULL)
#else
                || !GetTTSDMResouceFileFullName(dm_abs_default_path,&dm_abs_default_path_length)
				|| (TTS_PLAYER_ERR_NONE != ejTTSPlayer_Init((const char*)abs_default_path, 
	#ifdef EJ_ENG_SUPPORT
					NULL,//(const char*)en_abs_default_path, 
	#else
					NULL,
	#endif
					(const char*)dm_abs_default_path)
#endif
					)
				)
			{		
				ejTTSPlayer_End();
				TTSAPI_NotifyTTSOver();
				if (TTSAPI_IsPromptWhenErr())
				{
					MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
				}
				return;							
			}		
            
        }
        ejTTSPlayer_Play(TTS_PLAYER_PLAY_ASYNC, s_ttsinfo.szTextIn, JTTS_PlayCallback, 0);
    }
    else
    {
		//JTTS_LOG:"HandlePlayTextTimer stop jtts palyer"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_329_112_2_18_3_3_38_14,(uint8*)"");
		TTS_SetManualStop(TRUE);
        ejTTSPlayer_Stop(TTS_PLAYER_STOP_ASYNC);
        s_ttsinfo.timer_id = MMK_CreateTimerCallback(  TTS_DELAY_TIME, HandlePlayTextTimer, NULL, FALSE);
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_StopPlayText(void)
{

	 //JTTS_LOG:"MMITTS_StopPlayText"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_341_112_2_18_3_3_38_15,(uint8*)"");
    if (s_ttsinfo.timer_id > 0)
    {
        MMK_StopTimer(s_ttsinfo.timer_id);
        s_ttsinfo.timer_id = 0;
    }

	TTS_SetManualStop(TRUE);	
	
    while(TTSAPI_GetPlayStatus())
    {
        ejTTSPlayer_Stop(TTS_PLAYER_STOP_ASYNC);
        SCI_Sleep(30);
    }

    ejTTSPlayer_End();    
      
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_GetPlayStatus(void)
{
    BOOLEAN result = FALSE;

    if (!TTS_IsCommandStatusNone() || 
        0 != s_ttsinfo.timer_id)
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("**TTSAPI_GetPlayStatus, TTSAPI_GetPlayStatus=%d,",result);
    return result;
}


/*****************************************************************************/
//  Description : 获得audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
    //JTTS_LOG:"TTSAPI_GetAudioIsOpen | audio_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_383_112_2_18_3_3_38_16,(uint8*)"d", audio_handle);
    if(audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }  
}

/*****************************************************************************/
//  Description : TTSAPI_SynthTextByTask
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  TTSAPI_SynthTextByTask(void )
{
	int status = 0;
	wchar                       abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#ifdef EJ_ENG_SUPPORT
	wchar                       en_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      en_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#endif
	wchar                       dm_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      dm_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;

	//JTTS_LOG:"TTSAPI_SynthTextByTask enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_408_112_2_18_3_3_38_17,(uint8*)"");
	if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {        
        if (TTSAPI_IsPromptWhenErr())
        {
            MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        }
        return FALSE;            
    }

#ifdef TTS_FILE_RES
    else if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        if (TTSAPI_IsPromptWhenErr())    
        {            
            MMIPUB_OpenAlertWarningWin(TXT_TTS_UDISK_USING);
        }
        return FALSE;
    }
    else if(!TTSAPI_checkResFile())
    {       
        if (TTSAPI_IsPromptWhenErr())
        {
            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
        }
        return FALSE;
    }
#endif


    if (TTSAPI_GetPlayStatus())
    {
		//JTTS_LOG:"TTSAPI_SynthTextByTask stop jtts"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_440_112_2_18_3_3_38_18,(uint8*)"");
		ejTTSPlayer_Stop(TTS_PLAYER_STOP_ASYNC);		
	}
        
    ejTTSPlayer_GetStatus(&status);
    //JTTS_LOG:"TTSAPI_SynthTextByTask | ejTTSPlayer_GetStatus = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_445_112_2_18_3_3_38_19,(uint8*)"d", status);
    if (TTS_PLAYER_NOT_INIT == status)
    {
		if(!GetTTSResouceFileFullName(abs_default_path,&abs_default_path_length)
#ifdef EJ_ENG_SUPPORT
		 || !GetTTSENResouceFileFullName(en_abs_default_path,&en_abs_default_path_length)
#endif	
#ifdef TTS_SUPPORT_EJ_ROM
		 || (TTS_PLAYER_ERR_NONE != ejTTSPlayer_Init((const char*)g_cn_package, 
#ifdef EJ_ENG_SUPPORT
            NULL,//(const char*)g_en_package,
#else
            NULL,
#endif
            NULL)
#else
        || !GetTTSDMResouceFileFullName(dm_abs_default_path,&dm_abs_default_path_length)
		 || (TTS_PLAYER_ERR_NONE != ejTTSPlayer_Init((const char*)abs_default_path, 
#ifdef EJ_ENG_SUPPORT
            NULL,//(const char*)en_abs_default_path,
#else
            NULL,
#endif
            (const char*)dm_abs_default_path)
#endif
                )
			)
		{
			ejTTSPlayer_End();		
			if (TTSAPI_IsPromptWhenErr())
			{
				MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
			}
			return FALSE;
		}
    }
    ejTTSPlayer_Play(TTS_PLAYER_PLAY_SYNC, s_ttsinfo.szTextIn, JTTS_PlayCallback, 0);    
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_NotifyTTSOver(void)
{
	//JTTS_LOG:"TTSAPI_NotifyTTSOver enter, s_ttsinfo.win_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_479_112_2_18_3_3_38_20,(uint8*)"d",s_ttsinfo.win_handle);
	if (TTS_RING_TYPE_EBOOK == s_ttsinfo.ring_type)
	{
		MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_ERROR_RESOURCE, PNULL ,0);
	}    
 	MMK_PostMsg(s_ttsinfo.win_handle, MSG_TTS_OVER, PNULL, 0 );
}

/*****************************************************************************/
//  Description : stop tts by type
//  Global resource dependence :
//  Author: aoke.hu
//  Note: 返回值 true:tts未播放或调用了停止播放;  false:tts仍旧在播放
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type)
{
	BOOLEAN result = TRUE;
    
    //SCI_TRACE_LOW:"TTSAPI_StopPlayTextByType ring_type=%d,  s_ttsinfo.ring_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_492_112_2_18_3_3_38_21,(uint8*)"dd",ring_type,s_ttsinfo.ring_type);
   // if(TTSAPI_GetAudioIsOpen())
    //{
        if(s_ttsinfo.ring_type == ring_type)
        {
            TTSAPI_StopPlayText();
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
   
    
    return result;
}

/*****************************************************************************/
//  Description :  同步停tts
//  Global resource dependence : none
//  Author: jun hu 2010/10 修改了此函数....
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void TTSAPI_StopSynthTextByTask(void)
{	
    TTSAPI_StopPlayText();  

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void TTSAPI_StopASynchTextByTask(void)
{
	TTSAPI_StopPlayText();  

}

/*****************************************************************************/
//  Description : 释放内存资源
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTS_MemoryRelease(void)
{

}
/*****************************************************************************/
//  Description : 判断是否在播放
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = FALSE;

    if (s_ttsinfo.ring_type == ring_type && TTSAPI_GetPlayStatus())
    {
        result = TRUE;
    }
    SCI_TRACE_LOW("**TTSAPI_IsPlayRing, ring_type=%d,result = %d,",ring_type,result);
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_Init(void)
{
	s_ttsinfo.is_prompt_when_err = TRUE;
}

/*****************************************************************************/
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    )
{
     // 如果TTS正在播放....
    if(TTSAPI_GetPlayStatus())
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, STXT_STOP, rightsoft_id, is_need_update);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, TXT_SET_TTS_PLAY, rightsoft_id, is_need_update);
    }
}

/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_checkResFile(void)
#ifdef TTS_SUPPORT_EJ_ROM
{    
    return TRUE; 
}
#else
{	
	wchar                       abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#ifdef EJ_ENG_SUPPORT
	wchar                       en_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      en_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;
#endif
	wchar                       dm_abs_default_path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      dm_abs_default_path_length             = MMIFMM_FILE_FILE_NAME_MAX_LEN;

	if(!GetTTSResouceFileFullName(abs_default_path,&abs_default_path_length)
#ifdef EJ_ENG_SUPPORT
		|| !GetTTSENResouceFileFullName(en_abs_default_path,&en_abs_default_path_length)
#endif
        || !GetTTSDMResouceFileFullName(dm_abs_default_path,&dm_abs_default_path_length)
	   )
	{
		return FALSE;
	}

    if (
        !MMIFILE_IsFileExist((const wchar*)abs_default_path, abs_default_path_length) 
#ifdef EJ_ENG_SUPPORT
        ||
        !MMIFILE_IsFileExist((const wchar*)en_abs_default_path, en_abs_default_path_length)
#endif
        || !MMIFILE_IsFileExist((const wchar*)dm_abs_default_path, dm_abs_default_path_length)
		)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
    
}
#endif

/*****************************************************************************/
//  Description : gui 调用此接口朗读菜单 
//  Global resource dependence : none
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_PlayTextForControl(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
									  MMISRVAUD_ROUTE_T tts_route
                                      )
{
#ifdef MMI_READMENU_ENABLE
    if(PNULL == str_ptr || 0 == num)
    {
        //SCI_TRACE_LOW:"[MMITTS]:TTSAPI_PlayTextForControl ERROR!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_636_112_2_18_3_3_38_22,(uint8*)"");
        return;
    }

    
    //JTTS_LOG:"TTSAPI_PlayTextForControl enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_641_112_2_18_3_3_39_23,(uint8*)"");
    // 0 为开关打开状态
    if (0 == MMIAPIENVSET_GetActiveModeOptValue(0, ENVSET_MENU_READ_SWITCH) &&
        !MMIAPIENG_GetIQModeStatus())
    {        
            uint32 i = 0, len = 0;
            wchar* total_str_ptr = NULL;
            uint8 vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_READMENU);
			MMITTS_PLAY_PARAM_T play_param = {0};

// 根据需求，开关此功能
#if 1
            // 考虑到隐私问题，不自动读出收件箱，发件箱，草稿箱，已发信箱在list中部分内容
            MMI_WIN_ID_T focus_win_id = MMK_GetFocusWinId();
            if (MMISMS_MSGBOX_MAIN_WIN_ID == focus_win_id)
            {
                if (3 == num) num = 2;
            }
#endif

            // 不知道所有字符串的总长，需要累加得出
            for (; i < num; i ++)
            {
                len += str_ptr[i].wstr_len + 1;
            }
            total_str_ptr = (wchar*)SCI_ALLOC((len + 1) * 2);
            SCI_MEMSET(total_str_ptr, 0, (len + 1) * 2);
            
            // 将所有字符串复制到一个buffer中，便于朗读
            for (i = 0, len = 0; i < num; i ++)
            {
                if (str_ptr[i].wstr_len > 0)
                {
                    // 复制
                    SCI_MEMCPY(total_str_ptr + len, str_ptr[i].wstr_ptr, str_ptr[i].wstr_len * 2);
                    len += str_ptr[i].wstr_len;
                    // 在字符串与字符串之间加空格符，这样可以产生停顿效果
                    *(total_str_ptr + len) = 0x20;
                    len ++;
                }
            }

            // 此时不应该弹出出错提示（比如缺少TTS资源文件等），因为菜单朗读太频繁，提示会给用户带来困扰
            s_ttsinfo.is_prompt_when_err = FALSE;
			play_param.txt_addr = (uint8*)total_str_ptr;
			play_param.txt_len_in_byte = len * 2;
			play_param.is_ucs2 = TRUE;
			play_param.play_times = 1;
			play_param.tts_vol = vol;
			play_param.ring_type = TTS_RING_TYPE_MENU;
			play_param.win_handle = 0;
			play_param.tts_route = tts_route;
			play_param.digit_mode = TTS_DIGIT_NUMBER;
			play_param.punc_mode = TTS_PUNC_OFF;
			play_param.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;        
            TTSAPI_PlayText(&play_param);
            // 恢复弹出出错提示标志量
            s_ttsinfo.is_prompt_when_err = TRUE;

            SCI_FREE(total_str_ptr);
        
    }

#endif
}



/*****************************************************************************/
//  Description :  设置TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRingType(TTS_ALL_RING_TYPE_E ring_type)
{
	s_ttsinfo.ring_type = ring_type;

}
/*****************************************************************************/
//  Description :  获取TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void)
{
	return s_ttsinfo.ring_type;
}

/*****************************************************************************/
//  Description :  获取TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolume(void)
{

	return s_ttsinfo.tts_vol;
}

/*****************************************************************************/
//  Description :  设置TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSVolume(uint16 set_vol)
{
	MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();

	s_ttsinfo.tts_vol = set_vol;

	if (0 != audio_handle)
	{		
		MMISRVAUD_SetVolume(audio_handle, set_vol);
	}
	
}

/*****************************************************************************/
//  Description :  获取TTS volume type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolumeType(void)
{

	return s_ttsinfo.volume_type;
}

/*****************************************************************************/
//  Description :  TTSAPI_IsPromptWhenErr
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPromptWhenErr(void)
{

	if (TTS_RING_TYPE_MENU != s_ttsinfo.ring_type)
	{
		return TRUE;
	}
	return FALSE;
}


/*****************************************************************************/
//  Description :  HandleAudioEnv
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleAudioEnv(BOOLEAN is_begin)
{
	MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();

	//JTTS_LOG:"HandleAudioEnv is_begin = %d audio_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_753_112_2_18_3_3_39_24,(uint8*)"dd",is_begin,audio_handle);

    if (is_begin)
    {        

        if (0 == audio_handle)  
		{
    		//SCI_TRACE_LOW:"HandleAudioEnv() Handle error !!!!"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_760_112_2_18_3_3_39_25,(uint8*)"");
			return;
		}

        if(TTS_GetTTSVolume() != MMISRVAUD_GetVolume(audio_handle))
		{
			MMISRVAUD_SetVolume(audio_handle, TTS_GetTTSVolume());
		}        
        
    }
   
}

/*****************************************************************************/
//  Description :  JTTS_PlayCallback
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL jtErrCode JTTS_PlayCallback(
                                  long uMessage,                // [in] Message
                                  long lParam,                  // [in] parameter 
                                  unsigned long wParam)         // [in] parameter 
{
   // uint32 result = 0;

	//SCI_TRACE_LOW:"JTTS_PlayCallback() uMessage = %d s_is_manual_stop = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_784_112_2_18_3_3_39_26,(uint8*)"dd",uMessage , s_is_manual_stop);
    if (TTS_PLAYER_NOTIFY_BEGIN == uMessage)
    {
        HandleAudioEnv(TRUE);
    }
    else if (TTS_PLAYER_NOTIFY_END == uMessage)
    {
        HandleAudioEnv(FALSE);

		if (!s_is_manual_stop)
        {
			MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_OVER, PNULL, 0 );
		}
		//SCI_TRACE_LOW:"JTTS_PlayCallback() post MSG_TTS_OVER"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_797_112_2_18_3_3_39_27,(uint8*)"");        
        MMI_TriggerMMITask();
        
    }
    else if (TTS_PLAYER_NOTIFY_ERROR == uMessage)
    {
        if (TTS_PLAYER_ERR_AUDIO_DEVICE == lParam)
        {
            HandleAudioEnv(FALSE);
			if (TTS_RING_TYPE_EBOOK == s_ttsinfo.ring_type)
			{
				MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_ERROR_RESOURCE, PNULL ,0);
			}
 			MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL, 0 );
            MMI_TriggerMMITask();
        }
    }
    
    return jtTTS_ERR_NONE;
}


/*****************************************************************************/
// 	Description : TTS_GetManualStop
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetManualStop(void)
{
	//JTTS_LOG:"TTS_GetManualStop s_is_manual_stop = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,EJTTSPLAYER_API_823_112_2_18_3_3_39_28,(uint8*)"d",s_is_manual_stop);
	return s_is_manual_stop;
}

/*****************************************************************************/
// 	Description : TTS_SetManualStop
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTS_SetManualStop(BOOLEAN is_manul_stop)
{
	 s_is_manual_stop = is_manul_stop;
}

/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id)
{

}



/*****************************************************************************/
//  Description : TTS_GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16  TTS_GetPuncMode(void)
{
	uint16 tts_punc_mode = jtTTS_PUNC_OFF;

	SCI_TRACE_LOW("[TTS] TTS_GetPuncMode s_ttsinfo.punc_mode = %d",s_ttsinfo.punc_mode);
	switch(s_ttsinfo.punc_mode) 
	{
	case TTS_PUNC_OFF:
		tts_punc_mode = jtTTS_PUNC_OFF;
		break;

	case TTS_PUNC_ON:
		tts_punc_mode = jtTTS_PUNC_ON;
		break;

	default:
		break;
	}

	SCI_TRACE_LOW("[TTS] TTS_GetPuncMode tts_punc_mode = %d",tts_punc_mode);
	return tts_punc_mode;
}

/*****************************************************************************/
//  Description : TTS_GetDigitMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 TTS_GetDigitMode(void)
{
	uint16 tts_digit_mode = jtTTS_DIGIT_NUMBER_AUTO;
	
	switch(s_ttsinfo.digit_mode) 
	{
	case TTS_DIGIT_AUTO:
		tts_digit_mode = jtTTS_DIGIT_NUMBER_AUTO;
		break;			
    case TTS_DIGIT_NUMBER:
		tts_digit_mode = jtTTS_DIGIT_TELEGRAM_ONLY ;
		break;
    case TTS_DIGIT_VALUE:
		tts_digit_mode = jtTTS_DIGIT_NUMBER_ONLY;
		break;

	default:
		break;
	}
	SCI_TRACE_LOW("[TTS] TTS_GetDigitMode tts_digit_mode = %d",tts_digit_mode);
	return tts_digit_mode;
}

/*****************************************************************************/
//  Description : TTS_GetRoute
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T TTS_GetRoute(void)
{
	SCI_TRACE_LOW("[TTS] TTS_GetRoute tts_route = %d",s_ttsinfo.tts_route);
	return s_ttsinfo.tts_route;
}

PUBLIC uint32 jt_Printf(
    const char *x_format, ...)
{
	SCI_TRACE_LOW(x_format);
	return 1;
		
}
#ifdef   __cplusplus
    }
#endif

#endif 

