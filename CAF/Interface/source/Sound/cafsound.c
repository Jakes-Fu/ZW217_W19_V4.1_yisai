#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafsound.c                                              *
** Author:         Riancy.Zhang                                            *
** Date:           02/23/2009                                              *
** Copyright:      2008-2010 Spreatrum, Incoporated. All Rights Reserved.  *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2009        Riancy.Zhang       Create
**
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafsound.h"
#include "cafmm.h"
#include "mmisrvaud_api.h"
#include "sig_code.h"
#include "mmi_ring.h"
#include "audio_api.h"
#include "mmi_modu_main.h"
#include "asm.h"
#include "sci_types.h"
#include "priority_system.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CAFAUDIO_TRACKBUFSIZE   (80 * 150)
#define CAFTHREAD_NAME			"CAFSOUND"
#define TASK_CAFSOUND_PRI        PRI_CAFSOUND_TASK  //26

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/

//sound class
typedef struct
{
    CAF_VTBL_T(ISOUND_T)  *vtbl_ptr; // �����б�

    CAF_GUID_T       	guid;
    uint32           	ref_num;

	uint32				dw_curvolume;//��ǰ����ֵ
	uint32				*start_dur_ptr;//���ڰ���ģʽ��������¼��ǰ���ڲ���
								 //���������Ѳ��Ŵ���;
								 //���ڰ�ʱ�䲥��ģʽ��������¼��ǰ����
								 //���ŵ��������Ѳ���ʱ��(����).
	BLOCK_ID		tonelistthreadid;//���thread��Ҫ��ΪPlayToneList��
	////
	HAUDIO 				haud;//audio's handle
	int					next_idx;//next tone array-unit's index

	CAFSoundToneData_T* std_ptr;//tone array to malloced
	uint16				std_num;//tone array's units

	CAFMMState_E 		estate;//״̬

	uint32 				*vbdoub_ptr;//VBDecodeOuputBuf

	BOOLEAN			bthreaddone;//׼����

	CAFSoundDeviceMode_E	edevmode;//�豸ģʽ
}CAFSOUND_T;

//ring-ID and ring-index's table-struct.
//here is midi-format ring only.
//because sound play base ring only.
typedef struct
{
	CAFSoundTone_E	st_id;//ring ID
	uint32			st_idx;//ring index
	uint32			adtype;  //audio type

}CAFRRTable_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : query interface
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr	ָ�� ISOUND_T �ӿڶ����ָ�롣
//	guid		��ʶ���ڳ��Բ�ѯ��ʵ�壨�ӿڻ����ݣ���ȫ��Ψһ id��
//	ojbect_pptr   	ָ��Ҫ�����Ľӿڻ����ݵ�ָ�롣 ������ݻ�����ֵΪ NULL��
//				���ʾҪ��ѯ�Ľӿڻ����ݲ����ڡ�
//
//	Return:
//	FALSE  :  ʧ��
//	TRUE   :  �ɹ�
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOUND_QueryInterface(
                                            ISOUND_T *isound_ptr,
                                            CAF_GUID_T guid,
                                            void** ojbect_pptr
                                            );
/*****************************************************************************/
// 	Description : add ref
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	Reference Count ���ӵĶ������ü���
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSOUND_AddRef( ISOUND_T * isound_ptr );
/*****************************************************************************/
// 	Description : release
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	Reference Count ���ٵĶ������ü�����
//	0 (��)����Ѿ��ͷŶ���,�Ҳ�����Ч
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSOUND_Release( ISOUND_T * isound_ptr );
/*****************************************************************************/
// 	Description : get volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume_ptr		 ����ֵ�洢��ַ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetVolume( ISOUND_T* isound_ptr, uint32* dw_volume_ptr);
/*****************************************************************************/
// 	Description : set volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume		�豸/ ������������ֵ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetVolume( ISOUND_T* isound_ptr, uint32 dw_volume);
/*****************************************************************************/
// 	Description : play tone
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume_ptr		 ����ֵ�洢��ַ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_PlayTone( ISOUND_T* isound_ptr, CAFSoundToneData_T tonedata);
/*****************************************************************************/
// 	Description : play tone-list
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	tonedata	CAFSoundToneData_T �ṹ�б�����
//	wdatalen		tonedata_ptr �б��е�������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_PlayToneList( ISOUND_T* isound_ptr, CAFSoundToneData_T* tonedata_ptr,uint16 wdatalen);
/*****************************************************************************/
// 	Description : stop tone
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_StopTone( ISOUND_T* isound_ptr);

/*****************************************************************************/
// 	Description :create new class
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	guid 		�½������Ψһid
//	object_pptr		 �½������ַ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOUND_ClassNew(
                                      CAF_GUID_T guid,
                                      void**     object_pptr
                                      );


/*****************************************************************************/
//  Description : get ring-data by toneid .
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	dw_toneid	tone ��id
//	hac_ptr		��Ƶ�������
//	had_ptr		��Ƶ�豸���
//	ri_ptr		������Ϣ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//  Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetRingDataByToneID(uint32 dw_toneid,HAUDIOCODEC* hac_ptr,HAUDIODEV* had_ptr,MMIAUD_RING_DATA_INFO_T* ri_ptr);
/*****************************************************************************/
//  Description : Sound notify callback function.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	haudio		��Ƶ���
//	notify_info		����֪ͨ��
//	affix_info		֪ͨ������Ϣ
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFSOUND_NotifyCallback(
								   HAUDIO haudio,
								   uint32 notify_info,
								   uint32 affix_info
                );
/*****************************************************************************/
//  Description : thead entry .
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFSOUND_ThreadEntry(uint32 param,void* obj_ptr);
/*****************************************************************************/
//  Description : set params.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	curidx			��������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//  Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetParams(
									   ISOUND_T *isound_ptr,
									   int curidx);
/*****************************************************************************/
// 	Description : set device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//  emode			�豸ģʽö��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E emode);
/*****************************************************************************/
// 	Description : get device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//  emode_ptr			�豸ģʽָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E* emode_ptr);

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//audio's decode buffer to output
//LOCAL uint32 vbdoub_ptr[CAFAUDIO_TRACKBUFSIZE];

// functions table
LOCAL const CAF_VTBL_T(ISOUND_T) s_sound_vtbl =
{
    CAFSOUND_QueryInterface,
    CAFSOUND_AddRef,
    CAFSOUND_Release,

    CAFSOUND_SetVolume,
    CAFSOUND_GetVolume,

    CAFSOUND_PlayTone,
    CAFSOUND_PlayToneList,
    CAFSOUND_StopTone,
    CAFSOUND_SetDevMode,
	CAFSOUND_GetDevMode
};


LOCAL const CAF_STATIC_CLASS_T s_sound_class_list[] =
{
    { SPRD_GUID_SOUND,      CAFSOUND_ClassNew ,0}
};

const CAF_STATIC_CLASS_INFO_T g_sound_static_class_info =
{
    (CAF_STATIC_CLASS_T*)s_sound_class_list, ARR_SIZE(s_sound_class_list)
};

LOCAL const CAFRRTable_T s_sound_rrtable[]=
{
	//������
	{CAF_TONE_0,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_1,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_2,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_3,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_4,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_5,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_6,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_7,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_8,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_9,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_A,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_B,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_C,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_D,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},
	{CAF_TONE_POUND,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},  //#
	{CAF_TONE_STAR,R_KEY_1,MMISRVAUD_RING_FMT_WAVE},	 //*
	//������
	{CAF_SMS_RING,R_SMS_1,MMISRVAUD_RING_FMT_MIDI},
#ifdef MMS_SUPPORT
	//������
	{CAF_MMS_RING,R_MMS_1,MMISRVAUD_RING_FMT_MIDI},
#endif
	//��ѹ��
	{CAF_POWER_RING,R_POWER_1,MMISRVAUD_RING_FMT_MIDI},
	//������
	{CAF_FLIP_RING,R_FLIP_1,MMISRVAUD_RING_FMT_MIDI},
	//ɾ����
	{CAF_DEL_RING,R_CALL_1,MMISRVAUD_RING_FMT_MIDI},
	//ѡ����
	{CAF_SELECTED_RING,R_CALL_1 + 1,MMISRVAUD_RING_FMT_MIDI}
};
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          functions definition                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get audio's codec and device by type.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note:
/*****************************************************************************/
LOCAL void CAFSOUND_GetCodecAndDevice(uint32 dw_type,HAUDIOCODEC* hac_ptr,HAUDIODEV* had_ptr)
{
#if 0
	switch(dw_type)
	{
		case MMISRVAUD_RING_FMT_MIDI:
			*hac_ptr = hMIDICodec;
			*had_ptr = hAUDDEV;
			break;
		case MMISRVAUD_RING_FMT_SMAF:
			*hac_ptr = hMIDICodec;
			*had_ptr = hLAYER1;
			break;
		case MMISRVAUD_RING_FMT_MP3:
			*hac_ptr = hMP3Codec;
			*had_ptr = hAUDDEV;
			break;
		case MMISRVAUD_RING_FMT_WMA:
#ifdef MMI_WMA_ENABLE
			*hac_ptr = hWMACodec;
			*had_ptr = hAUDDEV;
#else
			*hac_ptr = 0;
			*had_ptr = 0;
#endif
			break;
		case MMISRVAUD_RING_FMT_AMR:
			*hac_ptr = hAMRCodec;
			*had_ptr = hLAYER1;
			break;
		case MMISRVAUD_RING_FMT_GENERIC_TONE:
			*hac_ptr = hGenericCodec;
			*had_ptr = hLAYER1;
			break;
		case MMISRVAUD_RING_FMT_CUSTOM_TONE:
			*hac_ptr = hCustomCodec;
			*had_ptr = hLAYER1;
			break;
		case MMISRVAUD_RING_FMT_DTMF_TONE:
			*hac_ptr = hDtmfCodec;
			*had_ptr = hLAYER1;
			break;
		case MMISRVAUD_RING_FMT_WAVE:
			*hac_ptr = hWAVCodec;
			*had_ptr = hAUDDEV;
			break;
		case MMISRVAUD_RING_FMT_AAC:
			*hac_ptr = hAACLCCodec;
			*had_ptr = hAUDDEV;
			break;
		default:
			*hac_ptr = hMIDICodec;
			*had_ptr = hLAYER1;
			break;
	}
#endif
}
/*****************************************************************************/
//  Description : Sound notify callback function.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	haudio		��Ƶ���
//	notify_info		����֪ͨ��
//	affix_info		֪ͨ������Ϣ
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFSOUND_NotifyCallback(
                HAUDIO haudio,
                uint32 notify_info,
                uint32 affix_info
                )
{
	CAFSOUND_T* sound_ptr=NULL;
	//AUDIO_RESULT_E result=affix_info;
	//��Ҫ�����Ž���
    if(AUDIO_PLAYEND_IND != notify_info)
	{
		return;
    }
	//����ֹͣ
	sound_ptr=(CAFSOUND_T*)CAFMM_GetObject((uint32)haudio);
	if(sound_ptr != NULL)
	{
		sound_ptr->estate=CAF_MM_STATE_STOP;
	}
}
/*****************************************************************************/
//  Description : set params.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	curidx			��������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//  Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetParams(
									   ISOUND_T *isound_ptr,
									   int curidx)
{
	CAF_MM_RESULT mmresult=CAF_MM_NOERROR;
    HAUDIOCODEC haudiocode = 0;
    HAUDIODEV hdevice = 0;
	MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};

	CAFSOUND_T* sound_ptr = (CAFSOUND_T*)isound_ptr;

	mmresult = CAFSOUND_GetRingDataByToneID(sound_ptr->std_ptr[curidx].etone,&haudiocode,&hdevice,&ring_data);
	if(CAF_MM_NOERROR != mmresult)
	{
		//��Ч��ID
		return mmresult;
	}


	if(INVALID_HANDLE != sound_ptr->haud )
	{
		AUDIO_Stop(sound_ptr->haud);
		AUDIO_CloseHandle(sound_ptr->haud);
	}

	//ȱʡ������Ƶ�豸
	sound_ptr->haud=AUDIO_CreateBufferHandle(
		haudiocode,PNULL,
		hdevice,PNULL,
		ring_data.data_ptr,
		ring_data.data_len,
		sound_ptr->vbdoub_ptr,CAFAUDIO_TRACKBUFSIZE,
		CAFSOUND_NotifyCallback);

    if(INVALID_HANDLE == sound_ptr->haud)
    {
		mmresult= CAF_MM_INVALHANDLE;
    }
	else
	{
		AUDIO_SetOutputSampleRate(sound_ptr->haud, CAF_MM_OUTPUT_SAMPLERATE);
	}

	return mmresult;
}
/*****************************************************************************/
//  Description : thead entry .
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFSOUND_ThreadEntry(uint32 param,void* obj_ptr)
{
	CAFSOUND_T* sound_ptr = (CAFSOUND_T*)obj_ptr;
	int curidx=0,nextidx=0;
	uint32 curdur=0;
	AUDIO_RESULT_E result=AUDIO_NO_ERROR;
	CAF_MM_RESULT mmresult=CAF_MM_NOERROR;
    //HAUDIOCODEC haudiocode = 0;
    //HAUDIODEV hdevice = 0;

	//MMIAUD_RING_DATA_INFO_T     ring_data = {AUD_MIDI_RING, 0, NULL};

	if(NULL == sound_ptr)
	{
		return;
	}

	while(sound_ptr->bthreaddone)
	{
		//���в��Ž�������
		if(sound_ptr->std_num <sound_ptr->next_idx)
		{
			//
			sound_ptr->bthreaddone=FALSE;
			return;
		}
		if(CAF_MM_STATE_STOP != sound_ptr->estate )
		{
			SCI_Sleep(10);
			continue;
		}
		curidx=(sound_ptr->next_idx >= 1)?(sound_ptr->next_idx-1):0;

		//��Ҫ�Ǵ���ѭ������
		//�����ǰ�ʲôģʽ,ֻҪ sound_ptr->std_ptr[curidx].dw_value == 0
		//�������޲���
		if(0 == sound_ptr->std_ptr[curidx].dw_value)
		{

			//���޴�
			//��������
			mmresult = CAFSOUND_SetParams((ISOUND_T*)sound_ptr,curidx);
			if(CAF_MM_NOERROR != mmresult)
			{
				//����ʧ��
				sound_ptr->bthreaddone=FALSE;
				return;
			}
		}
		else //��������
		{
			//���β���
			if(CSM_MODE_TIMES == sound_ptr->std_ptr[curidx].emode )
			{
				sound_ptr->start_dur_ptr[curidx]++;
				curdur = sound_ptr->start_dur_ptr[curidx];
				nextidx=sound_ptr->next_idx;
			}
			else
			{
				curdur=(SCI_GetCurrentTime()-sound_ptr->start_dur_ptr[curidx])*1000;
				nextidx=curidx;
			}

			if(curdur < sound_ptr->std_ptr[curidx].dw_value)
			{
				//
				mmresult = CAFSOUND_SetParams((ISOUND_T*)sound_ptr,nextidx);
				if(CAF_MM_NOERROR != mmresult)
				{
					//����ʧ��
					sound_ptr->bthreaddone=FALSE;
					return;
				}
			}
			if(curdur >= sound_ptr->std_ptr[curidx].dw_value)
			{
				//duration end
				sound_ptr->next_idx++;
				if(sound_ptr->std_num >= sound_ptr->next_idx)
				{

					//times or duration init
					if(sound_ptr->std_ptr[sound_ptr->next_idx-1].emode == CSM_MODE_TIMES)
						sound_ptr->start_dur_ptr[sound_ptr->next_idx-1]=0;
					else
						sound_ptr->start_dur_ptr[sound_ptr->next_idx-1] = SCI_GetCurrentTime();

				}
				continue;
			}

		}


		//����
		result=AUDIO_Play(sound_ptr->haud, 0);
		if(AUDIO_NO_ERROR == result)
		{
			sound_ptr->estate =CAF_MM_STATE_PLAYING;
		}
	}
}
/*****************************************************************************/
//  Description : get ring-data by toneid .
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	dw_toneid	tone ��id
//	hac_ptr		��Ƶ�������
//	had_ptr		��Ƶ�豸���
//	ri_ptr		������Ϣ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//  Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetRingDataByToneID(uint32 dw_toneid,HAUDIOCODEC* hac_ptr,HAUDIODEV* had_ptr,MMIAUD_RING_DATA_INFO_T* ri_ptr)
{
	int i;
	BOOLEAN bResult;
	uint32 dw_idx=0,dw_type=MMISRVAUD_RING_FMT_WAVE;
	int count= sizeof(s_sound_rrtable)/sizeof(CAFRRTable_T);

	for(i=0;i<count;i++)
	{
		if(dw_toneid == (uint32)s_sound_rrtable[i].st_id)
		{
			dw_idx = s_sound_rrtable[i].st_idx;
			dw_type = s_sound_rrtable[i].adtype;
			break;
		}
	}
	//û���ҵ�
	if(0 == dw_idx)
	{
		return CAF_MM_INVALID;
	}
	//
	CAFSOUND_GetCodecAndDevice(dw_type,hac_ptr,had_ptr);

	bResult=MMI_GetRingInfo(dw_idx,ri_ptr);
	//
	return (bResult?CAF_MM_NOERROR:CAF_MM_READERROR);
}

/*****************************************************************************/
// 	Description : new sound-class
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOUND_ClassNew(
                                      CAF_GUID_T guid,
                                      void**     object_pptr
                                      )
{
    CAFSOUND_T   *sound_ptr = PNULL;
    CAF_MM_RESULT ret=CAF_MM_NOERROR;

    if( SPRD_GUID_SOUND != guid )
	{
		return CAF_RESULT_NOT_SUPPORT;//����sound id
    }

    sound_ptr = SCI_ALLOC_APP(sizeof(CAFSOUND_T));
	if( NULL == sound_ptr)
	{
		return CAF_RESULT_NOT_SUPPORT;//û���ڴ�ɷ���
	}
    SCI_MEMSET(sound_ptr, 0, sizeof(CAFSOUND_T));
    sound_ptr->vtbl_ptr = (CAF_VTBL_T(ISOUND_T)*)&s_sound_vtbl;

	//�������buffer
	sound_ptr->vbdoub_ptr=(uint32*)SCI_ALLOC_APP(CAFAUDIO_TRACKBUFSIZE*sizeof(uint32));
	if(sound_ptr->vbdoub_ptr == NULL)
	{
		SCI_FREE(sound_ptr);
		return CAF_RESULT_NOT_SUPPORT;
	}
	SCI_MEMSET(sound_ptr->vbdoub_ptr, 0, (CAFAUDIO_TRACKBUFSIZE*sizeof(uint32)));
	//init object
	CAFMM_InitObject();

	sound_ptr->estate=CAF_MM_STATE_UNKNOWN;
	sound_ptr->bthreaddone = FALSE;
	////
	sound_ptr->haud = INVALID_HANDLE;
    sound_ptr->guid      = guid;
    sound_ptr->ref_num   = 1;
    sound_ptr->start_dur_ptr= NULL;
    sound_ptr->next_idx	 = 0;
	sound_ptr->dw_curvolume = CAF_MAX_SPEAKER_VOLUME-1;
	sound_ptr->edevmode = SDM_DEVICE_MODE_HANDFREE;
	//save object
	ret=CAFMM_AddObject((uint32)sound_ptr);
	if(CAF_MM_NOERROR != ret)
	{
		SCI_FREE(sound_ptr->vbdoub_ptr);
		SCI_FREE(sound_ptr);
		return CAF_RESULT_NOT_SUPPORT;
	}
	*object_pptr = sound_ptr;


    return CAF_RESULT_SUCCEEDED;
}

/*****************************************************************************/
// 	Description : query interface
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr	ָ�� ISOUND_T �ӿڶ����ָ�롣
//	guid		��ʶ���ڳ��Բ�ѯ��ʵ�壨�ӿڻ����ݣ���ȫ��Ψһ id��
//	ojbect_pptr   	ָ��Ҫ�����Ľӿڻ����ݵ�ָ�롣 ������ݻ�����ֵΪ NULL��
//				���ʾҪ��ѯ�Ľӿڻ����ݲ����ڡ�
//
//	Return:
//	FALSE  :  ʧ��
//	TRUE   :  �ɹ�
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFSOUND_QueryInterface(
                                            ISOUND_T *isound_ptr,
                                            CAF_GUID_T guid,
                                            void** ojbect_pptr
                                            )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAFSOUND_T* sound_ptr = (CAFSOUND_T*)isound_ptr;

	if(NULL == isound_ptr)
	{
		return result;
	}
    if ( IUNKNOWN_ID == guid ||  sound_ptr->guid == guid )
    {
        *ojbect_pptr = isound_ptr;

        IUNKNOWN_AddRef( *ojbect_pptr );
		result = CAF_RESULT_SUCCEEDED;
    }
    else
	{
		return result;//����sound id,Ҳ����IUNKNOWN ID
    }

    return result;
}

/*****************************************************************************/
// 	Description : add ref
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	Reference Count ���ӵĶ������ü���
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSOUND_AddRef(
                              ISOUND_T * isound_ptr
                              )
{
	if(NULL == isound_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
    return ++(((CAFSOUND_T *)isound_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : release
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	Reference Count ���ٵĶ������ü�����
//	0 (��)����Ѿ��ͷŶ���,�Ҳ�����Ч
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFSOUND_Release(
                               ISOUND_T * isound_ptr
                               )
{
    CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;

	if(NULL == isound_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
    if ( --sound_ptr->ref_num )
    {
        return( sound_ptr->ref_num );
    }
    else
    {

		if(CAF_MM_STATE_PLAYING == sound_ptr->estate )
		{
			//��ֹ���ͷ�Thread��Դ
			sound_ptr->bthreaddone=FALSE;
			SCI_TerminateThread(sound_ptr->tonelistthreadid);
			SCI_DeleteThread(sound_ptr->tonelistthreadid);
			//��ֹ��Ƶ
			AUDIO_Stop(sound_ptr->haud);
			//
			AUDIO_CloseHandle(sound_ptr->haud);
		}
		//remove object
		CAFMM_RemoveObject((uint32)sound_ptr);
		//�ͷ�Tone�����б�
		SCI_FREE(sound_ptr->std_ptr);
		SCI_FREE(sound_ptr->start_dur_ptr);
		SCI_FREE(sound_ptr->vbdoub_ptr);
        SCI_FREE( sound_ptr );

        return 0;
    }
}
/*****************************************************************************/
// 	Description : get volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume_ptr		 ����ֵ�洢��ַ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetVolume( ISOUND_T* isound_ptr, uint32* dw_volume_ptr)
{

	AUDIO_RESULT_E result;
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;

	if(NULL == isound_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	//WORD lvolume=0,rvolume=0;
	//�����ǰ��������������ģ���Ϊϵͳ������
	//��ʱ���õ�����������
	result=AUDIO_GetVolume(dw_volume_ptr);
	if(result != AUDIO_NO_ERROR)
	{
		//��ȡ�����������,�����ǰ�豸�ı���
		//����ֵ������
		*dw_volume_ptr=sound_ptr->dw_curvolume;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume		�豸/ ������������ֵ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetVolume( ISOUND_T* isound_ptr, uint32 dw_volume)
{
	AUDIO_RESULT_E result;
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;
	uint16 lvolume=CAF_LOWORD(dw_volume);//rvolume=CAF_HIWORD(dw_volume);

	if(NULL == isound_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(0 == dw_volume && 0 ==sound_ptr->dw_curvolume)
	{
		//�Ѿ��Ǿ���
		return CAF_MM_NOERROR;
	}

	if(0 == dw_volume && 0 != sound_ptr->dw_curvolume)
	{
		//���Ǿ���
		result = AUDIO_SetDownLinkMute(TRUE);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		sound_ptr->dw_curvolume=dw_volume;
	}

	if(0 != dw_volume && 0 == sound_ptr->dw_curvolume)
	{
		//ԭ���Ǿ������ȴ򿪹رվ���
		result = AUDIO_SetDownLinkMute(FALSE);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		//����������
		SCI_Sleep(25);//ԭ��ϵͳ�����������ʱ!��ʱ����
        result = AUDIO_SetVolume((uint32)lvolume);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}

		sound_ptr->dw_curvolume=dw_volume;
	}

	if((0 != dw_volume && 0 != sound_ptr->dw_curvolume)&& \
		(dw_volume != sound_ptr->dw_curvolume))
	{
		//ֱ������
        result = AUDIO_SetVolume((uint32)lvolume);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		sound_ptr->dw_curvolume=dw_volume;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : play tone
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	dwvolume_ptr		 ����ֵ�洢��ַ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_PlayTone( ISOUND_T* isound_ptr, CAFSoundToneData_T tonedata)
{
	return CAFSOUND_PlayToneList(isound_ptr,&tonedata,1);
}
/*****************************************************************************/
// 	Description : play tone-list
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	tonedata	CAFSoundToneData_T �ṹ�б�����
//	wdatalen		tonedata_ptr �б��е�������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_PlayToneList( ISOUND_T* isound_ptr, CAFSoundToneData_T* tonedata_ptr,uint16 wdatalen)
{
	CAF_MM_RESULT mmresult;
    //HAUDIOCODEC haudiocode = 0;
    //HAUDIODEV hdevice = 0;
    AUDIO_RESULT_E result;
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;
	//MMIAUD_RING_DATA_INFO_T     ring_data = {AUD_MIDI_RING, 0, NULL};

	if(0 == wdatalen || NULL == isound_ptr)
	{
		//������Ч
		return CAF_MM_INVALPARAM;
	}
	//�洢tone ����
	sound_ptr->std_ptr=SCI_ALLOC_APP(sizeof(CAFSoundToneData_T) * wdatalen);
	if(NULL == sound_ptr->std_ptr)
	{
		//�ڴ治��
		return CAF_MM_NOMEM;
	}
	//��ʼ������Ԫ
	sound_ptr->start_dur_ptr=SCI_ALLOC_APP(sizeof(DWORD) * wdatalen);
	if(NULL == sound_ptr->start_dur_ptr)
	{
		//�ڴ治��
		SCI_FREE(sound_ptr->std_ptr);
		return CAF_MM_NOMEM;
	}
    sound_ptr->next_idx	 = 0;
	SCI_MEMSET(sound_ptr->std_ptr,0x00,(sizeof(CAFSoundToneData_T)*wdatalen));
	SCI_MEMSET(sound_ptr->start_dur_ptr,0x00,(sizeof(DWORD)*wdatalen));
	//���ݴ洢
	SCI_MEMCPY(sound_ptr->std_ptr,tonedata_ptr,sizeof(CAFSoundToneData_T)*wdatalen);
	/*
	for(int i=0;i<wdatalen;i++)
	{
		sound_ptr->std_ptr[i].emode = tonedata[i].emode;
		sound_ptr->std_ptr[i].etone = tonedata[i].etone;
		sound_ptr->std_ptr[i].dw_value = tonedata[i].dw_value;
	}
	*/
	sound_ptr->std_num = wdatalen;
	//������
	if(0 == sound_ptr->std_ptr[0].dw_value)
	{
		//����ʱ,���޴�
		sound_ptr->next_idx = 0;//��������
	}
	else
	{
		if(sound_ptr->std_num > 1)
		{
			//�ƶ�����һ������
			sound_ptr->next_idx=1;
		}
	}

	if( CSM_MODE_TIMES == sound_ptr->std_ptr[0].emode )
	{
		sound_ptr->start_dur_ptr[0]=0;
	}
	else
	{
		//��ȡ��ǰʱ��
		sound_ptr->start_dur_ptr[0]= SCI_GetCurrentTime();
	}
	//�ҵ���һ��ring id,��ȡ�������ݽ��в���
	//���豸���ò���
	mmresult = CAFSOUND_SetParams((ISOUND_T*)sound_ptr,0);

	if(CAF_MM_NOERROR != mmresult)
	{
		//����ʧ��
		SCI_FREE(sound_ptr->std_ptr);
		SCI_FREE(sound_ptr->start_dur_ptr);
		return CAF_MM_INVALPARAM;
	}
	//��ʼ����
	result=AUDIO_Play(sound_ptr->haud, 0);
	if(AUDIO_NO_ERROR != result)
	{
		SCI_FREE(sound_ptr->std_ptr);
		SCI_FREE(sound_ptr->start_dur_ptr);
		return CAF_MM_UNKNOWNERROR;
	}

	//modify object
	CAFMM_ModifyObject((uint32)sound_ptr,(uint32)sound_ptr->haud);

	sound_ptr->estate = CAF_MM_STATE_PLAYING;
	//����thread,�������Ʋ���,��Ϊ�����д�������ʱ��Ϊ����
	//��ѭ��

	sound_ptr->bthreaddone=TRUE;
	sound_ptr->tonelistthreadid= SCI_CreateAppThread(
										(const char*)CAFTHREAD_NAME,
										(const char*)CAFTHREAD_NAME,
										CAFSOUND_ThreadEntry,
										0,
										(void*)sound_ptr,
										4*1024,
										6,26,SCI_PREEMPT,SCI_AUTO_START);

	if(0 == sound_ptr->tonelistthreadid)
	{

		sound_ptr->bthreaddone=FALSE;
		SCI_FREE(sound_ptr->std_ptr);
		SCI_FREE(sound_ptr->start_dur_ptr);
    	return CAF_MM_INVALHANDLE;

	}
	////
	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : stop tone
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_StopTone( ISOUND_T* isound_ptr)
{
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;
	if(NULL == isound_ptr)
	{
		//������Ч
		return CAF_MM_INVALPARAM;
	}
	if(CAF_MM_STATE_PLAYING == sound_ptr->estate )
	{
		//��ֹthread
		sound_ptr->bthreaddone=FALSE;
		SCI_TerminateThread(sound_ptr->tonelistthreadid);
		SCI_DeleteThread(sound_ptr->tonelistthreadid);
		//ֹͣ����
		AUDIO_Stop(sound_ptr->haud);
		AUDIO_CloseHandle(sound_ptr->haud);
		sound_ptr->haud = INVALID_HANDLE;
	}
	sound_ptr->estate = CAF_MM_STATE_STOP;

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//  emode			�豸ģʽö��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_SetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E emode)
{
	AUDIO_RESULT_E result;
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;
	if(NULL == isound_ptr)
	{
		//������Ч
		return CAF_MM_INVALPARAM;
	}
	if(emode >= SDM_DEVICE_MODE_MAX )
	{
		sound_ptr->edevmode = SDM_DEVICE_MODE_HANDFREE;
	}
	else
	{
		sound_ptr->edevmode = emode;
	}

	result= AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)sound_ptr->edevmode);

	return (AUDIO_NO_ERROR != result)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : get device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		ָ�� ISOUND_T �ӿڶ����ָ��
//  emode_ptr			�豸ģʽָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFSOUND_GetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E* emode_ptr)
{
	AUDIO_RESULT_E result;
	CAFSOUND_T * sound_ptr = (CAFSOUND_T *)isound_ptr;
	if(NULL == isound_ptr)
	{
		//������Ч
		return CAF_MM_INVALPARAM;
	}


	result=AUDIO_GetDevMode((AUDIO_DEVICE_MODE_TYPE_E*)emode_ptr);
	if(result != AUDIO_NO_ERROR)
	{
		//��ȡ�����������,�����ǰ�豸�ı���
		//ģʽֵ������
		*emode_ptr=sound_ptr->edevmode;
	}

	return CAF_MM_NOERROR;

}
#endif