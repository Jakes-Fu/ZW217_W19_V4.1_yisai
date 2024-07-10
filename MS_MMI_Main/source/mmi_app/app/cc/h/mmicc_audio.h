/****************************************************************************
** File Name:      mmicc_audio.h                                            *
** Author:                                                                  *
** Date:           24/11/2008                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2008        samboo.shen      Create
** 
****************************************************************************/
#ifndef  _MMICC_AUDIO_H_
#define  _MMICC_AUDIO_H_

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ��õ�ǰ������ģʽ
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMICC_AUDIO_DEV_E MMICC_GetCurrentDev ( void );

/*****************************************************************************/
//  Description : construct 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructAudioHandle (void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyAudioHandle (void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAudioHandleEnable (void);

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructVoiceCodec (BOOLEAN is_vt_codec);

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyVoiceCodec (void);

/*****************************************************************************/
//  Description : ��/�ر�voice codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_EnableVoiceCodec ( BOOLEAN is_enable );

/*****************************************************************************/
//  Description : voice codec�Ƿ��Ѿ���
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVoiceCodecEnable ( void );


/*****************************************************************************/
//  Description : ��/�ر� ���˾���
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableMute ( BOOLEAN is_mute );

/*****************************************************************************/
//  Description : �жϱ��˾����Ƿ��
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN    MMICC_IsMute ( void );

/*****************************************************************************/
//  Description : ��/�ر� Զ�˾���
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableRemoteMute ( BOOLEAN is_enable );

/*****************************************************************************/
//  Description : �ж�Զ�˾����Ƿ��
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN    MMICC_IsRemoteMute ( void );

/*****************************************************************************/
//  Description : enable /disable remote mute from net, but the local tone still to be played
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_EnableNetRemoteMute ( BOOLEAN is_enable );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleVibrateTimer(uint8 timer_id, uint32 param);

/*******************************************************************************/
//  Description : ͨ��ǰֹͣ���е���������
//  Global resource dependence : 
//  Author:jun.hu
//  Note: �� MMICC_StopRingOrVibrate�������������ͬ���Ƕ�TTS�Ĵ�������첽���õķ�ʽ
/********************************************************************************/
PUBLIC void MMICC_StopRingOrVibrateBeforeCCing(void);

/*****************************************************************************/
//  Description :set mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC void MMICC_SetMtPresetCodecRoute ( MMISRVAUD_ROUTE_T  route );

/*****************************************************************************/
//  Description :get mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMICC_GetMtPresetCodecRoute ( void );

#endif


