#include "caf_trc.h"
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

#include "cafmedia.h"
#include "cafmm.h"

#include "caffilesys.h"

#include "cafhelp.h"
#include "sig_code.h"
#include "dal_player.h"
#include "dal_recorder.h"

#include "mmidc_setting.h"
#include "mmidc_save.h"
#include "mmidc_display.h"
#include "mmidc_export.h"

#include "aud_gen.h"
#include "audio_api.h"
#include "audio_prod.h"
#include "mmi_modu_main.h"

#include "asm.h"
#include "mmk_app.h"
#include "guifont.h"
#include "mmk_shell.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CAFMEDIA_TRACKBUFSIZE   (80 * 150)

#define CAF_AUDIO_BIT_RATE						4750
#define CAF_AUDIO_CHANNEL						1
#define	CAF_AUDIO_SAMPLE_RATE					8000

#define CAF_VIDEO_128X96_HEIGHT					96
#define CAF_VIDEO_128X96_WIDTH					128
#define CAF_VIDEO_160X128_HEIGHT				128
#define CAF_VIDEO_160X128_WIDTH					160
#define CAF_VIDEO_176X144_HEIGHT				144
#define CAF_VIDEO_176X144_WIDTH					176
#define CAF_VIDEO_240X176_HEIGHT				176
#define CAF_VIDEO_240X176_WIDTH					240
#define CAF_VIDEO_320X240_HEIGHT				240
#define CAF_VIDEO_320X240_WIDTH					320
#define CAF_VIDEO_352X288_HEIGHT				288
#define CAF_VIDEO_352X288_WIDTH					352

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
/*
typedef enum
{
	VIDEO_SIZE_352X288=0,
	VIDEO_SIZE_320X240,
	VIDEO_SIZE_240X176,
	VIDEO_SIZE_176X144,
	VIDEO_SIZE_160X128,
	VIDEO_SIZE_128X96,
	VIDEO_SIZE_MAX,
}VIDEO_SIZE_E;


typedef enum
{
	CAMERA_MODE_DC = 0,
	CAMERA_MODE_DV,
	CAMERA_MODE_MAX
}CAMERA_MODE_E;


typedef enum
{
    VIDEO_FILE_TYPE_MP4 = 0,
    VIDEO_FILE_TYPE_MJPEG,
    VIDEO_FILE_TYPE_3GP,
    VIDEO_FILE_TYPE_MAX,
} VIDEO_FILE_TYPE_E;

typedef enum
{
	AUDIO_SETTING_ON = 0,
	AUDIO_SETTING_OFF,
	AUDIO_SETTING_MAX
} AUDIO_SETTING_E;
typedef enum
{
SCREEN_MODE_HORIIZONTAL = 0,
SCREEN_MODE_VERTICAL,
SCREEN_MODE_MAX
}SCREEN_MODE_E;
*/
typedef enum
{
	MMIENG_DV_FRAME_FRE_MIN,
	MMIENG_DV_FRAME_FRE_7HZ = 7,
	MMIENG_DV_FRAME_FRE_10HZ = 10,
	MMIENG_DV_FRAME_FRE_15HZ = 15,
	MMIENG_DV_FRAME_FRE_20HZ = 20,
	MMIENG_DV_FRAME_FRE_25HZ = 25,
	MMIENG_DV_FRAME_FRE_30HZ = 30,
	MMIENG_DV_FRAME_FRE_MAX
} MMIENG_DV_FRAME_FRE_E;

typedef enum
{
	MMIENG_SENSOR_MODE_DEFAULT = 0,
		MMIENG_SENSOR_MODE_RGB,
		MMIENG_SENSOR_MODE_YUV,
		MMIENG_SENSOR_MODE_MAX
} MMIENG_SENSOR_MODE_E;


//media type struct

typedef struct
{
	char	*mt_ptr; //类型串
	CAFMediaFormat_E ntype;//


}CAFMediaTypeTable_T;

//stream buffer's struct
typedef struct {

    uint8 *buffer;    // the buffer holding the data
    uint32 size;    // the size of the allocated buffer
    uint32 in;    // data is added at offset (in % size)
    uint32 out;    // data is extracted from off. (out % size)

}CAFFifo_T;

//media class
typedef struct
{
    CAF_VTBL_T(IMEDIA_T)  *vtbl_ptr; // 函数列表

    CAF_GUID_T       	guid;
    uint32           	ref_num;


	uint32				dw_curvolume;//当前音量值
	BOOLEAN				isvideo;//是视频媒体

	DPLAYER_HANDLE		hvideo;//av handle for mp4 etc.
	HAUDIO 				haud;//audio's handle for only audio
	CAFMediaData_T		mediadata;//媒体数据
	CAFMediaInfo_T		mediainfo;//媒体信息
	CAFFifo_T			fifo;//stream buffer-struct
	CAF_RECT_T			imgrt;//image display-rect

	CAFMMState_E 		estate;//state

	uint32 				*vbdoub_ptr;//VBDecodeOuputBuf;
	uint32 				*vbeoub_ptr;//VBEncodeOuputBuf;

	CAFMediaDeviceMode_E edevmode;//设备模式
}CAFMEDIA_T;


/*****************************************************************************/
// 	Description : set media's data by file play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByFileMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : set media's data by buffer play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByBuffMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : set media's data by stream play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByStreamMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : set media's data by file record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByFileMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : set media's data by buffer record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByBuffMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : set media's data by stream record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByStreamMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);

/*****************************************************************************/
// 	Description : query interface
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	guid			 标识正在尝试查询的实体（接口或数据）的全局唯一 id。
//	ojbect_pptr   	指向要检索的接口或数据的指针。 如果传递回来的值为 NULL，则表示要查询的接口或数据不存在
//	Return:
//	FALSE  :  失败
//	TRUE   :  成功
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFMEDIA_QueryInterface(
                                            IMEDIA_T* imedia_ptr,
                                            CAF_GUID_T guid,
                                            void** ojbect_pptr
                                            );
/*****************************************************************************/
// 	Description : add ref
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	引用计数，增加的对象引用计数
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFMEDIA_AddRef( IMEDIA_T* imedia_ptr );
/*****************************************************************************/
// 	Description : release
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	引用计数，增加的对象引用计数
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFMEDIA_Release( IMEDIA_T* imedia_ptr );
/*****************************************************************************/
// 	Description : set media's data
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	medidadata_ptr 		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
/*****************************************************************************/
// 	Description : get media's data
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	medidadata_ptr	 媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr );

/*****************************************************************************/
// 	Description : set media's volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dw_volume		新音量值
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetVolume(IMEDIA_T* imedia_ptr,uint32 dw_volume);
/*****************************************************************************/
// 	Description : get media's volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dw_volume_ptr		 指向音量值的指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetVolume(IMEDIA_T* imedia_ptr,uint32* dw_volume_ptr);
/*****************************************************************************/
// 	Description : set media's displayed rect
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	rimage_ptr		 图像矩形指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr);
/*****************************************************************************/
// 	Description : get media's displayed rect
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	rimage_ptr		图像矩形指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr);
/*****************************************************************************/
// 	Description : get media's state
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dwstate_ptr		状态值
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetState(IMEDIA_T* imedia_ptr,uint32* dw_state_ptr);
/*****************************************************************************/
// 	Description : get media's information
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	mediainfo_ptr			 媒体信息
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetMediaInfo(IMEDIA_T* imedia_ptr,CAFMediaInfo_T* mediainfo_ptr);
/*****************************************************************************/
// 	Description : play media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Play(IMEDIA_T* imedia_ptr);
/*****************************************************************************/
// 	Description : record media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Record(IMEDIA_T* imedia_ptr);
/*****************************************************************************/
// 	Description : pause a playing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Pause(IMEDIA_T* imedia_ptr);
/*****************************************************************************/
// 	Description : resume a pausing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Resume(IMEDIA_T* imedia_ptr);
/*****************************************************************************/
// 	Description : stop a playing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Stop(IMEDIA_T* imedia_ptr);
/*****************************************************************************/
// 	Description : seek the position in a media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	eseek			 定位引用
//	emode			 定位模式
//	lvalue			 偏移量
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Seek(IMEDIA_T* imedia_ptr,CAFMediaSeek_E eseek,CAFMediaSeekMode_E emode,int lvalue);
/*****************************************************************************/
// 	Description : write a stream data to a buffer
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	data_ptr 		 待写入的流数据
//	dwsize_ptt 	待写入的流数据数据大小，及实际写入的数据大小
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_WriteStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr);
/*****************************************************************************/
// 	Description : read a stream data from buffer
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	data_ptr		 存放待取的流数据空间
//	dwsize_ptr		存放待取的流数据空间大小，及实际获取的数据大小。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_ReadStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr);
/*****************************************************************************/
// 	Description :create new class
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	guid 		新建对象的唯一id
//	object_pptr		 新建对象地址
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFMEDIA_ClassNew(
                                      CAF_GUID_T guid,
                                      void**     object_pptr
                                      );
/*****************************************************************************/
//  Description : get device and codec by media type.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	dw_mtype		 媒体类型
//	hacodec			音频编解码句柄
//	hadev			音频设备句柄
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_GetCodecAndDevByMediaType(
											  uint32 dw_mtype,
											  HAUDIOCODEC *hacodec,
											  HAUDIODEV *hadev
											  );
/*****************************************************************************/
//  Description : get device and codec by filename ext.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	fn_ptr			文件名
//	hacodec			音频编解码句柄
//	hadev			音频设备句柄
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFMEDIA_GetCodecAndDevByFileName(
											   wchar* fn_ptr,
											   HAUDIOCODEC *hacodec,
											   HAUDIODEV *hadev
                );
/*****************************************************************************/
// 	Description : callback when record complete
//	Global resource dependence : none
//  Author: ryan.xu
//	Param:
//	i_type		 通知类型。
//	Return:
//
//	Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_RecordEndNotifyCallBack(int32 i_type);
/*****************************************************************************/
//  Description : Sound notify callback function.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	haudio		音频句柄
//	notify_info		播放通知码
//	affix_info		通知附加信息
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_NotifyCallback(
								   HAUDIO haudio,
								   uint32 notify_info,
								   uint32 affix_info
                );
/*****************************************************************************/
//  Description : get audio source-data
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	hAudio		 音频句柄
//	ppucDataAddr	数据存放地址
//	puiDataLength	数据存放长度
//	offset			数据读取偏移
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E CAFMEDIA_GetSrcData(
	HAUDIO hAudio,
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo
	);
/*****************************************************************************/
// 	Description : set device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 		指向 IMEDIA_T 接口对象的指针
//  emode			设备模式枚举
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E emode);
/*****************************************************************************/
// 	Description : get device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 		指向 IMEDIA_T 接口对象的指针
//  emode_ptr			设备模式指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E *emode_ptr);
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//audio's decode buffer to output
//LOCAL uint32 vbdoub_ptr[CAFMEDIA_TRACKBUFSIZE];
//LOCAL uint32 vbeoub_ptr[CAFMEDIA_TRACKBUFSIZE];


// functions table
LOCAL const CAF_VTBL_T(IMEDIA_T) s_media_vtbl =
{
    CAFMEDIA_QueryInterface,
    CAFMEDIA_AddRef,
    CAFMEDIA_Release,

	CAFMEDIA_SetMediaData,
    CAFMEDIA_GetMediaData,

	CAFMEDIA_SetVolume,
	CAFMEDIA_GetVolume,

	CAFMEDIA_SetRect,
	CAFMEDIA_GetRect,

	CAFMEDIA_GetState,
    CAFMEDIA_GetMediaInfo,

	CAFMEDIA_Play,
	CAFMEDIA_Record,
	CAFMEDIA_Pause,
	CAFMEDIA_Resume,
	CAFMEDIA_Stop,
	CAFMEDIA_Seek,


	CAFMEDIA_WriteStreamData,
	CAFMEDIA_ReadStreamData,

	CAFMEDIA_SetDevMode,
	CAFMEDIA_GetDevMode
};


LOCAL const CAF_STATIC_CLASS_T s_media_class_list[] =
{
    { SPRD_GUID_MEDIA,      CAFMEDIA_ClassNew,0 }

};

const CAF_STATIC_CLASS_INFO_T g_media_static_class_info =
{
    (CAF_STATIC_CLASS_T*)s_media_class_list, ARR_SIZE(s_media_class_list)
};
//
LOCAL const CAFMediaTypeTable_T s_media_type_table[] =
{
	//audio
	{ "mp3",      MM_A_MP3},
	{ "mid",      MM_A_MIDI},
	{ "midi",     MM_A_MIDI},
	{ "asf",      MM_A_WMA},
	{ "wav",      MM_A_WAVE},
	//av
	{ "mpg",      MM_V_MJPEG},
	{ "3gp",      MM_V_3GP},
	{ "mp4",      MM_V_MP4},
	{ "mpeg",     MM_V_MJPEG}
};

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/
extern MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void);

#ifndef CAMERA_SUPPORT
#else
extern MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void);
#endif

/*---------------------------------------------------------------------------*/
/*                          functions definition                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get device and codec by media type.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	dw_mtype		 媒体类型
//	hacodec			音频编解码句柄
//	hadev			音频设备句柄
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_GetCodecAndDevByMediaType(
                uint32 dw_mtype,
				HAUDIOCODEC *hacodec,
				HAUDIODEV *hadev
                )
{
	switch(dw_mtype)
	{
		case MM_A_MP3:
			if(hacodec != NULL)
			{
				*hacodec = hMP3Codec;
			}
			if(hadev != NULL)
			{
				*hadev = hAUDDEV;
			}

			break;
		case MM_A_MIDI:
		case MM_A_WMA:
			if(hacodec != NULL)
			{
				*hacodec = hMIDICodec;
			}
			if(hadev != NULL)
			{
				*hadev = hAUDDEV;
			}
			break;
		case MM_A_WAVE:
			if(hacodec != NULL)
			{
				*hacodec = hWAVCodec;
			}
			if(hadev != NULL)
			{
				*hadev = hAUDDEV;
			}
			break;
		default:
			if(hacodec != NULL)
			{
				*hacodec = 0;
			}
			if(hadev != NULL)
			{
				*hadev = 0;
			}
			break;
	}

}

/*****************************************************************************/
//  Description : get device and codec by filename ext.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	fn_ptr			文件名
//	hacodec			音频编解码句柄
//	hadev			音频设备句柄
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFMEDIA_GetCodecAndDevByFileName(
                wchar* fn_ptr,
				HAUDIOCODEC *hacodec,
				HAUDIODEV *hadev
                )
{
	char *      ext_ptr=NULL;
	char fpath[256]={0x00};
	size_t  count=0,i=0,len=0;

	len=MMIAPICOM_Wstrlen(fn_ptr);
	GUI_WstrToGB((uint8*)fpath,fn_ptr,(uint16)len);
	count = sizeof(s_media_type_table)/sizeof(s_media_type_table[0]);
	ext_ptr = strrchr(fpath, '.');
	if( NULL != ext_ptr) ext_ptr++;
	//SCI_TRACE_LOW:">>>>>>>>>>%s,%s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFMEDIA_737_112_2_17_23_0_51_0,(uint8*)"ss",fpath,ext_ptr);
	for(i=0;i<count;i++)
	{
		/*lint -e668*/
		if(strcmp(ext_ptr,s_media_type_table[i].mt_ptr) == 0)
		{
			CAFMEDIA_GetCodecAndDevByMediaType((uint32)s_media_type_table[i].ntype,hacodec,hadev);
			return s_media_type_table[i].ntype;
		}
	}

	*hacodec=INVALID_HANDLE;
	*hadev=INVALID_HANDLE;

	return CAF_MM_INVALIDVALUE;
}
/*****************************************************************************/
// 	Description : callback when record complete
//	Global resource dependence : none
//  Author: ryan.xu
//	Param:
//	i_type		 通知类型。
//	Return:
//
//	Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_RecordEndNotifyCallBack(int32 i_type)
{
	CAFMEDIA_T   *media_ptr=NULL;
	//录制停止
	media_ptr=(CAFMEDIA_T*)CAFMM_GetObject((uint32)CAF_DEFAULT_HRECORD);
	if(NULL != media_ptr)
	{
		media_ptr->estate=CAF_MM_STATE_STOP;
	}

}

/*****************************************************************************/
//  Description : Sound notify callback function.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	haudio		音频句柄
//	notify_info		播放通知码
//	affix_info		通知附加信息
//	Return:
//
//  Note:
/*****************************************************************************/
LOCAL void CAFMEDIA_NotifyCallback(
                HAUDIO haudio,
				uint32 notify_info,
				uint32 affix_info
                )
{
	CAFMEDIA_T   *media_ptr=NULL;
	//AUDIO_RESULT_E result=affix_info;
	//主要处理播放结束
    if(AUDIO_PLAYEND_IND != notify_info)
	{
		return;
	}
	//播放停止
	media_ptr=(CAFMEDIA_T*)CAFMM_GetObject((uint32)haudio);
	if(NULL != media_ptr)
	{
		media_ptr->estate=CAF_MM_STATE_STOP;
	}
}
/*****************************************************************************/
//  Description : get audio source-data
//  Global resource dependence : none
//  Author: Riancy.Zhang
//	Param:
//	hAudio		 音频句柄
//	ppucDataAddr	数据存放地址
//	puiDataLength	数据存放长度
//	offset			数据读取偏移
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//  Note:
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E CAFMEDIA_GetSrcData(
    HAUDIO hAudio,
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo
	)
{
	CAF_MM_RESULT ret = CAF_MM_NOERROR;
	CAFMEDIA_T* media_ptr=(CAFMEDIA_T*)CAFMM_GetObject((uint32)hAudio);

	if(NULL == media_ptr)
	{
		return AUDIO_STREAM_GET_SRC_ERROR;
	}

	ret=CAFMEDIA_ReadStreamData(
	    (IMEDIA_T *)media_ptr,
	    (uint8 *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr),
	    (uint32 *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));

	return (CAF_MM_NOERROR == ret)?AUDIO_STREAM_GET_SRC_SUCCESS:AUDIO_STREAM_GET_SRC_ERROR;
}
/*****************************************************************************/
// 	Description :create new class
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	guid 		新建对象的唯一id
//	object_pptr		 新建对象地址
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFMEDIA_ClassNew(
                                      CAF_GUID_T guid,
                                      void**     object_pptr
                                      )
{
    CAFMEDIA_T   *media_ptr = PNULL;
    CAF_MM_RESULT ret =CAF_MM_NOERROR;
    if( SPRD_GUID_MEDIA != guid )
	{
		return CAF_RESULT_NOT_SUPPORT;//不是media id
	}

    media_ptr = SCI_ALLOC_APP(sizeof(CAFMEDIA_T));
	if( NULL == media_ptr)
	{
		return CAF_RESULT_NOT_SUPPORT;//没有内存可分配
	}
	SCI_MEMSET(media_ptr, 0, sizeof(CAFMEDIA_T));

    media_ptr->vtbl_ptr = (CAF_VTBL_T(IMEDIA_T)*)&s_media_vtbl;


	//分配解码buffer
	media_ptr->vbdoub_ptr=(uint32*)SCI_ALLOC_APP(CAFMEDIA_TRACKBUFSIZE*sizeof(uint32));
	if(NULL == media_ptr->vbdoub_ptr)
	{
		SCI_FREE(media_ptr);
		return CAF_RESULT_NOT_SUPPORT;
	}
	SCI_MEMSET(media_ptr->vbdoub_ptr, 0, (CAFMEDIA_TRACKBUFSIZE*sizeof(uint32)));
	media_ptr->vbeoub_ptr=(uint32*)SCI_ALLOC_APP(CAFMEDIA_TRACKBUFSIZE*sizeof(uint32));
	if(NULL == media_ptr->vbeoub_ptr)
	{
		SCI_FREE( media_ptr->vbdoub_ptr);
		SCI_FREE(media_ptr);

		return CAF_RESULT_NOT_SUPPORT;
	}
	SCI_MEMSET(media_ptr->vbeoub_ptr, 0, (CAFMEDIA_TRACKBUFSIZE*sizeof(uint32)));


	//该初始化用来为3gp,mp4做准备
	if(DPLAYER_SUCCESS != DPLAYER_Init())
	{
    	//设备句柄创建失败
    	SCI_FREE( media_ptr->vbdoub_ptr);
		SCI_FREE( media_ptr->vbeoub_ptr);
    	SCI_FREE( media_ptr);
    	return CAF_RESULT_NOT_SUPPORT;
	}

	media_ptr->estate = CAF_MM_STATE_UNKNOWN;
	//init object
	CAFMM_InitObject();

	////
    media_ptr->guid      = guid;
    media_ptr->ref_num   = 1;
	media_ptr->haud   = INVALID_HANDLE;
	media_ptr->hvideo = INVALID_HANDLE;
	media_ptr->dw_curvolume = 0;
	media_ptr->isvideo=FALSE;

	//save object
	ret=CAFMM_AddObject((uint32)media_ptr);
	if(CAF_MM_NOERROR != ret)
	{
		SCI_FREE( media_ptr->vbdoub_ptr);
		SCI_FREE( media_ptr->vbeoub_ptr);
		SCI_FREE(media_ptr);
		return CAF_RESULT_NOT_SUPPORT;
	}


    *object_pptr = media_ptr;


    return CAF_RESULT_SUCCEEDED;
}
/*****************************************************************************/
// 	Description : query interface
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	guid			 标识正在尝试查询的实体（接口或数据）的全局唯一 id。
//	ojbect_pptr   	指向要检索的接口或数据的指针。 如果传递回来的值为 NULL，则表示要查询的接口或数据不存在
//	Return:
//	FALSE  :  失败
//	TRUE   :  成功
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFMEDIA_QueryInterface(
                                            IMEDIA_T *imedia_ptr,
                                            CAF_GUID_T guid,
                                            void** ojbect_pptr
                                            )
{

    CAFMEDIA_T* media_ptr = (CAFMEDIA_T*)imedia_ptr;
    CAF_RESULT_E result = CAF_RESULT_FAILED;
	if(NULL == imedia_ptr)
	{
		return result;
	}

    if ( IUNKNOWN_ID == guid ||  media_ptr->guid == guid )
    {
        *ojbect_pptr = imedia_ptr;

        IUNKNOWN_AddRef( *ojbect_pptr );
		result = CAF_RESULT_SUCCEEDED;
    }
    else
	{
		return result;//不是media id,也不是IUNKNOWN ID
    }

    return result;
}

/*****************************************************************************/
// 	Description : add ref
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	引用计数，增加的对象引用计数
//	Note:
/*****************************************************************************/
LOCAL DWORD CAFMEDIA_AddRef(
                              IMEDIA_T * imedia_ptr
                              )
{
	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

    return ++(((CAFMEDIA_T *)imedia_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : release
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	引用计数，增加的对象引用计数
//	Note:
/*****************************************************************************/
LOCAL uint32 CAFMEDIA_Release(
                               IMEDIA_T * imedia_ptr
                               )
{
    CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
    }

    if ( --media_ptr->ref_num )
    {
        return( media_ptr->ref_num );
    }
    else
    {

		//终止音频
		if(0 != media_ptr->haud)
		{
			AUDIO_Stop(media_ptr->haud);

			AUDIO_CloseHandle(media_ptr->haud);
		}
		//end av
		if(0 != media_ptr->hvideo)
		{
			if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
			{
				DPLAYER_Stop(media_ptr->hvideo);
				DPLAYER_Close(media_ptr->hvideo);
			}
			else if( MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
			{
				DRECORDER_StopRecord();
				DRECORDER_Close();
			}


		}

		CAFMM_RemoveObject((uint32)media_ptr);

    	SCI_FREE( media_ptr->vbdoub_ptr);
		SCI_FREE( media_ptr->vbeoub_ptr);

        SCI_FREE( media_ptr );

        return 0;
    }
}

/*****************************************************************************/
// 	Description : set media's data by file play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByFileMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	HAUDIOCODEC haudcodec=0;
	HAUDIODEV hauddev=0;
	wchar fpath[256]={0x0000};
	uint32 dwmt=0;
	void    *argv[3] = {0};

	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//file
	TransferFileNameFromCAFToSys(fpath, medidadata_ptr->data_ptr);
	dwmt=CAFMEDIA_GetCodecAndDevByFileName((wchar*)fpath,&haudcodec,&hauddev);
	if(CAF_MM_INVALIDVALUE != dwmt && 0 != medidadata_ptr->dw_format)
	{
		dwmt=(dwvideo)?dwvideo:wauido;//av优先
	}
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio
		media_ptr->haud=AUDIO_CreateFileHandle(
												haudcodec,PNULL,
												hauddev,PNULL,
												(const int16*)fpath,
												media_ptr->vbdoub_ptr,CAFMEDIA_TRACKBUFSIZE,
												CAFMEDIA_NotifyCallback);
		media_ptr->isvideo = FALSE;

	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
		//av
		argv[0] = (void*)fpath;
		media_ptr->hvideo=DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 1, argv);
		media_ptr->isvideo = TRUE;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's data by buffer play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByBuffMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	HAUDIOCODEC haudcodec=0;
	HAUDIODEV hauddev=0;
	uint32 dwmt=0;
	void    *argv[3] = {0};

	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//buffer
	dwmt=(dwvideo)?dwvideo:wauido;//av优先
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio
		CAFMEDIA_GetCodecAndDevByMediaType(dwmt,&haudcodec,&hauddev);
		media_ptr->haud=AUDIO_CreateBufferHandle(
												haudcodec,PNULL,
												hauddev,PNULL,
												(const uint8*)medidadata_ptr->data_ptr,medidadata_ptr->dw_size,
												media_ptr->vbdoub_ptr,CAFMEDIA_TRACKBUFSIZE,
												CAFMEDIA_NotifyCallback);
		media_ptr->isvideo = FALSE;
	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
		//av
		argv[2] = (void*)(dwmt%MM_V_3GP);
		argv[0] = (void*)(medidadata_ptr->data_ptr);
		argv[1] = (void*)medidadata_ptr->dw_size;
		media_ptr->hvideo=DPLAYER_Open(DPLAYER_OPEN_FROM_MEM, 3, argv);
		media_ptr->isvideo = TRUE;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's data by stream play
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetPlayByStreamMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	HAUDIOCODEC haudcodec=0;
	HAUDIODEV hauddev=0;
	uint32 dwmt=0;

	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//stream
	dwmt=(dwvideo)?dwvideo:wauido;//av优先
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio
		CAFMEDIA_GetCodecAndDevByMediaType(dwmt,&haudcodec,&hauddev);
		media_ptr->haud=AUDIO_CreateStreamBufferHandle(
												haudcodec,PNULL,
												hauddev,PNULL,
												(const uint8*)medidadata_ptr->data_ptr,medidadata_ptr->dw_size,
												media_ptr->vbdoub_ptr,CAFMEDIA_TRACKBUFSIZE,
												CAFMEDIA_NotifyCallback,
												CAFMEDIA_GetSrcData
												);
		media_ptr->isvideo = FALSE;
	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
		//av
		/*
		argv[2] = (void*)(dwmt%MM_V_3GP);
		argv[0] = (void*)(medidadata_ptr->data_ptr);
		argv[1] = (void*)medidadata_ptr->dwsize;
		media_ptr->hvideo=DPLAYER_Open(DPLAYER_OPEN_FROM_NET, 3, argv);
		media_ptr->isvideo = TRUE;
		*/
		return CAF_MM_NOTSUPPORTED;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's data by file record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByFileMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
#if 0
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	HAUDIOCODEC haudcodec=0;
	HAUDIODEV hauddev=0;
	wchar fpath[256]={0x0000};
	uint32 dwmt=0;
	uint32          pram_result = 0;
	//RECORD_PARAM_T record_param = {AUD_REC_ADPCM, RECORD_OUTPUT_SAVE_TO_FILE, 0, ADPCM_FRAME_LENGTH, 0,0,0};

	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//file
	TransferFileNameFromCAFToSys(fpath, medidadata_ptr->data_ptr);
	dwmt=CAFMEDIA_GetCodecAndDevByFileName((wchar*)fpath,NULL,NULL);
	if(CAF_MM_INVALIDVALUE != dwmt && 0 != medidadata_ptr->dw_format)
	{
		dwmt=(dwvideo)?dwvideo:wauido;//av优先
	}
	haudcodec=hRecordAdpcmCodec;
	hauddev=hRECORDVB;
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio
		media_ptr->haud=AUDIO_CreateRecordFileHandle(
												haudcodec,PNULL,
												hauddev,PNULL,
												(const int16*)fpath,
												media_ptr->vbeoub_ptr,CAFMEDIA_TRACKBUFSIZE,
												CAFMEDIA_NotifyCallback);
		if(INVALID_HANDLE == media_ptr->haud)
		{
			//设备句柄创建失败
			return CAF_MM_INVALHANDLE;
		}
		media_ptr->isvideo = FALSE;
		AUDIO_ExeCodecExtOpe(media_ptr->haud,
			ANSI2UINT16("SET_RECORDPARAM"),//扩展操作的标志
			RECORD_PARAM_SET_COUNT,
			&record_param,
			&pram_result);
	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
#ifdef  CAMERA_SUPPORT
		MMIDC_SetLcdForRotate(TRUE);
		MMIAPIDC_AllocSettingMemory();
		//MMIAPIDC_InitOSDMenuIcon();
		MMIDC_SetCameraMode(CAMERA_MODE_DV);
		MMIDC_Setting_InitDefaultValue();
		//av
		if(DRECORDER_ERROR == DRECORDER_Open())
		{
			return CAF_MM_NOTENABLED;
		}
		media_ptr->hvideo=CAF_DEFAULT_HRECORD;
		media_ptr->isvideo = TRUE;
#else
            return CAF_MM_NOTENABLED;
#endif
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}
#endif
	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's data by buffer record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByBuffMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	//HAUDIODEV hauddev=0;
	uint32 dwmt=0;

	//CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//buffer
	dwmt=(dwvideo)?dwvideo:wauido;//av优先
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio
		return CAF_MM_NOTSUPPORTED;
		/*
		haudcodec=hRecordAdpcmCodec;
		hauddev=hRECORDVB;
		media_ptr->haud=AUDIO_CreateRecordBufferHandle(
		haudcodec,PNULL,
		hauddev,PNULL,
		(const uint8*)medidadata_ptr->data_ptr,medidadata_ptr->dwsize,
		media_ptr->vbeoub_ptr,CAFMEDIA_TRACKBUFSIZE,
		CAFMEDIA_NotifyCallback);
		media_ptr->isvideo = FALSE;
		*/
	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
		//av
		//media_ptr->isvideo = TRUE;
		return CAF_MM_NOTSUPPORTED;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	//return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's data by stream record
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 	 指向 IMedia 接口对象的指针
//	medidadata_ptr		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRecordByStreamMode(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	//HAUDIOCODEC haudcodec=0;
	//HAUDIODEV hauddev=0;
	uint32 dwmt=0;

	//CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	wvideo = CAF_HIWORD(medidadata_ptr->dw_format);
	wauido = CAF_LOWORD(medidadata_ptr->dw_format);
	dwvideo=(wvideo<<16);

	//stream
	dwmt=(dwvideo)?dwvideo:wauido;//av优先
	if(dwmt < MM_V_3GP && dwmt >= MM_A_MIDI)
	{
		//audio

		//media_ptr->isvideo = FALSE;
		return CAF_MM_NOTSUPPORTED;
	}
	else if(dwmt >= MM_V_3GP && dwmt <= MM_V_MJPEG)
	{
		//av
		//media_ptr->isvideo = TRUE;
		return CAF_MM_NOTSUPPORTED;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	//return CAF_MM_NOERROR;
}

/*****************************************************************************/
// 	Description : set media's data
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	medidadata_ptr 		媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	AUDIO_PLAYINFO_T apinfo={0,0,0,0,NULL};
	CAF_MM_RESULT ret = CAF_MM_NOERROR;

	if(NULL == imedia_ptr || NULL == medidadata_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	//end audio
	if(0 != media_ptr->haud)
	{
		AUDIO_Stop(media_ptr->haud);

		AUDIO_CloseHandle(media_ptr->haud);
	}
	//end av
	if(0 != media_ptr->hvideo)
	{
		DPLAYER_Stop(media_ptr->hvideo);

		DPLAYER_Close(media_ptr->hvideo);

	}
	//////////////////////////////////////
	switch(medidadata_ptr->clsdata)
	{
	case MMD_FILE_NAME:
		{
			if(MMW_MODE_PLAY == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetPlayByFileMode(imedia_ptr,medidadata_ptr);
			}
			else if( MMW_MODE_RECORD == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetRecordByFileMode(imedia_ptr,medidadata_ptr);
			}
		}
		break;
	case MMD_BUFFER:
		{
			if(MMW_MODE_PLAY == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetPlayByBuffMode(imedia_ptr,medidadata_ptr);
			}
			else if( MMW_MODE_RECORD == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetRecordByBuffMode(imedia_ptr,medidadata_ptr);
			}
		}
		break;
	case MMD_STREAM:
		{
			if(MMW_MODE_PLAY == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetPlayByStreamMode(imedia_ptr,medidadata_ptr);
			}
			else if( MMW_MODE_RECORD == medidadata_ptr->eworkmode)
			{
				ret=CAFMEDIA_SetRecordByStreamMode(imedia_ptr,medidadata_ptr);
			}
		}
		break;
	default:
		break;
	}

	if(CAF_MM_NOERROR != ret)
	{
		goto SET_END_FLAG;
	}
	//check dev
	if(!media_ptr->isvideo)
	{
		if(INVALID_HANDLE == media_ptr->haud)
		{
			//设备句柄创建失败
			//SCI_TRACE_LOW:">>>>>>>>>>>>>>>haud fail"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFMEDIA_1493_112_2_17_23_0_53_1,(uint8*)"");
			ret= CAF_MM_INVALHANDLE;
			goto SET_END_FLAG;
		}

		AUDIO_GetPlayingInfo(media_ptr->haud,&apinfo);

	}
	else
	{

		if(INVALID_HANDLE == media_ptr->hvideo)
		{
			//设备句柄创建失败
			ret= CAF_MM_INVALHANDLE;
			goto SET_END_FLAG;
		}
	}

	////
	media_ptr->mediadata.clsdata = medidadata_ptr->clsdata;
	media_ptr->mediadata.data_ptr= medidadata_ptr->data_ptr;
	media_ptr->mediadata.dw_format= medidadata_ptr->dw_format;
	media_ptr->mediadata.dw_size= medidadata_ptr->dw_size;
	media_ptr->mediadata.eworkmode= medidadata_ptr->eworkmode;
	////
	media_ptr->fifo.buffer= media_ptr->mediadata.data_ptr;
	media_ptr->fifo.size= media_ptr->mediadata.dw_size;
	media_ptr->fifo.in = media_ptr->fifo.out =0;

SET_END_FLAG:
	return ret;

}
/*****************************************************************************/
// 	Description : get media's data
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	medidadata_ptr	 媒体数据
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr )
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr || NULL == medidadata_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	medidadata_ptr->clsdata=media_ptr->mediadata.clsdata;
	medidadata_ptr->data_ptr=media_ptr->mediadata.data_ptr;
	medidadata_ptr->dw_format=media_ptr->mediadata.dw_format;
	medidadata_ptr->dw_size=media_ptr->mediadata.dw_size;
	medidadata_ptr->user_ptr=media_ptr->mediadata.user_ptr;
	medidadata_ptr->eworkmode = media_ptr->mediadata.eworkmode;
	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : get media's volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dw_volume_ptr		 指向音量值的指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetVolume( IMEDIA_T* imedia_ptr, uint32* dwvolume_ptr)
{

	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	//WORD lvolume=0,rvolume=0;
	//本来是按照左、右声道来的，因为系统的问题
	//暂时采用单声道来处理
	result=AUDIO_GetVolume(dwvolume_ptr);
	if(result != AUDIO_NO_ERROR)
	{
		//获取不到的情况下,如果当前设备的备份
		//音量值来返回
		*dwvolume_ptr=media_ptr->dw_curvolume;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's volume
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dw_volume		新音量值
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetVolume( IMEDIA_T* imedia_ptr, uint32 dwvolume)
{
	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	WORD lvolume=CAF_LOWORD(dwvolume);

	if(NULL == imedia_ptr)
		return CAF_MM_INVALPARAM;

	if(0 == dwvolume && 0 == media_ptr->dw_curvolume)
	{
		//已经是静音
		return CAF_MM_NOERROR;
	}

	if(0 == dwvolume && 0 != media_ptr->dw_curvolume)
	{
		//不是静音
		result = AUDIO_SetDownLinkMute(TRUE);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		media_ptr->dw_curvolume=dwvolume;
	}

	if(0 != dwvolume && 0 == media_ptr->dw_curvolume)
	{
		//原来是静音，先打开关闭静音
		result = AUDIO_SetDownLinkMute(FALSE);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		//在设置音量
		SCI_Sleep(25);//原来系统设置有这个延时!暂时保留
        result = AUDIO_SetVolume((uint32)lvolume);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		media_ptr->dw_curvolume=dwvolume;
	}

	if((0 != dwvolume && 0 != media_ptr->dw_curvolume)&& \
		(dwvolume != media_ptr->dw_curvolume))
	{
		//直接设置
        result = AUDIO_SetVolume((uint32)lvolume);
		if(AUDIO_NO_ERROR != result)
		{
			return CAF_MM_UNKNOWNERROR;
		}
		media_ptr->dw_curvolume=dwvolume;
	}

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set media's displayed rect
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	rimage_ptr		 图像矩形指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	CAF_MM_RESULT ret = CAF_MM_NOERROR;
	if(NULL == imedia_ptr || NULL == rimage_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	media_ptr->imgrt.left = rimage_ptr->left;
	media_ptr->imgrt.top = rimage_ptr->top;
	media_ptr->imgrt.right= rimage_ptr->right;
	media_ptr->imgrt.bottom= rimage_ptr->bottom;

	if(media_ptr->isvideo)
	{
		if( MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
		{
			//play
			DPLAYER_DISPLAY_PARAM_T ddp;
			SCI_MEMSET(&ddp,0x00,sizeof(DPLAYER_DISPLAY_PARAM_T));

			ddp.RotateAngle = 0;
			ddp.LcdId = 0;
			ddp.LcdBlkId     = 0;
    		ddp.videoSupport = 1;
            ddp.disp_mode = DPLAYER_DISP_CUSTOMIZE;
			ddp.audioSupport = 1;
            ddp.disp_rect.x = media_ptr->imgrt.left;
			ddp.disp_rect.y = media_ptr->imgrt.top;
			ddp.disp_rect.dx = (media_ptr->imgrt.right - media_ptr->imgrt.left);
			ddp.disp_rect.dy = (media_ptr->imgrt.bottom- media_ptr->imgrt.top);
            ddp.target_rect.x = ddp.disp_rect.x;
            ddp.target_rect.y = ddp.disp_rect.y;
            ddp.target_rect.dx = ddp.disp_rect.dx;
            ddp.target_rect.dy = ddp.disp_rect.dy;
			if(INVALID_HANDLE != media_ptr->hvideo)
			{
				DPLAYER_SetDisplayParam(media_ptr->hvideo,(DPLAYER_DISPLAY_PARAM_T const *)&ddp);
			}
		}
		else if( MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
		{
			//record
			/*
			if(media_ptr->hvideo == CAF_DEFAULT_HRECORD)
			{
				//
			}
			*/
			ret = CAF_MM_NOTSUPPORTED;
		}
		else
		{
			ret = CAF_MM_NOTSUPPORTED;
		}
	}

	return ret;

}
/*****************************************************************************/
// 	Description : get media's displayed rect
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	rimage_ptr		图像矩形指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr || NULL == rimage_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	rimage_ptr->left=media_ptr->imgrt.left;
	rimage_ptr->top=media_ptr->imgrt.top;
	rimage_ptr->right=media_ptr->imgrt.right;
	rimage_ptr->bottom=media_ptr->imgrt.bottom;

	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : get media's state
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	dwstate_ptr		状态值
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetState(IMEDIA_T* imedia_ptr,DWORD* dwstate_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr || NULL == dwstate_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	*dwstate_ptr = media_ptr->estate;

	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : get media's information
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	mediainfo_ptr			 媒体信息
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetMediaInfo(IMEDIA_T* imedia_ptr,CAFMediaInfo_T* mediainfo_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;


	if(NULL == imedia_ptr || NULL == mediainfo_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{
		//play
		if(!media_ptr->isvideo)
		{
			//is audio
			AUDIO_FORMATINFO_T fi;
			SCI_MEMSET(&fi,0x00,sizeof(AUDIO_FORMATINFO_T));
			if(AUDIO_ERROR != AUDIO_IdentifyAudioFormat(media_ptr->haud,&fi))
			{
				mediainfo_ptr->nsamplerate = fi.uiSamplerate;
				mediainfo_ptr->ntimes = fi.uiTotalTime;
			}
		}
		else
		{
			DPLAYER_MEDIA_INFO_T mf;
			SCI_MEMSET(&mf,0x00,sizeof(DPLAYER_MEDIA_INFO_T));
			if(DPLAYER_ERROR != DPLAYER_GetMovieInfo(media_ptr->hvideo,&mf))
			{
				mediainfo_ptr->nwidth = mf.video_info.width;
				mediainfo_ptr->nheight = mf.video_info.height;
				mediainfo_ptr->nsamplerate = mf.video_info.frame_rate;
				mediainfo_ptr->ntimes = DPLAYER_GetLength(media_ptr->hvideo);
			}
		}
	}
	else if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
	{
		//record
		return CAF_MM_NOTSUPPORTED;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : play media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Play(IMEDIA_T* imedia_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{	//play
		if(media_ptr->isvideo)
		{	//3gp,mp4 etc.
			if( DPLAYER_SUCCESS != DPLAYER_Play(media_ptr->hvideo))
			{
				//play fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
		else
		{
			//mp3,midi,wav etc.
			if(AUDIO_NO_ERROR != AUDIO_Play(media_ptr->haud, 0))
			{
				//play fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
	}
	else if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
	{
		//record
		return CAF_MM_NOTSUPPORTED;
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	media_ptr->estate = CAF_MM_STATE_PLAYING;


	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : init AV info struct
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_InitAVInfo(uint32 dwmt,MEDIA_AUDIO_INFO_T* audioinfo_ptr,MEDIA_VIDEO_INFO_T* videoinfo_ptr)
{

	if(NULL == audioinfo_ptr || NULL == videoinfo_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	/* set audio info */
	audioinfo_ptr->audio_support    = (AUDIO_SETTING_ON == MMIDC_GetVideoAudioSetting());
	audioinfo_ptr->channels	   = CAF_AUDIO_CHANNEL;
	audioinfo_ptr->sample_rate	   = CAF_AUDIO_SAMPLE_RATE;
	audioinfo_ptr->audio_type	   = AUDIO_TYPE_AMR;
    /* set video info */
	/////
    if(MM_V_3GP == dwmt)
    {
        videoinfo_ptr->video_type	= VIDEO_TYPE_H263;
    }
    else if(MM_V_MP4 == dwmt)
    {
        videoinfo_ptr->video_type	= VIDEO_TYPE_MP4V;
    }
    else if(MM_V_MJPEG == dwmt)
    {
        videoinfo_ptr->video_type	= VIDEO_TYPE_MJPEG;
    }
    else
    {
        return CAF_MM_NOTSUPPORTED;
    }


	/////
    videoinfo_ptr->frame_rate	= (uint32)MMIAPIENG_GetDVFrameFre();
    if(VIDEO_SIZE_320X240 == MMIDC_GetVideoSize())
    {
        videoinfo_ptr->height		= CAF_VIDEO_320X240_HEIGHT;
        videoinfo_ptr->width		= CAF_VIDEO_320X240_WIDTH;
    }
    else if(VIDEO_SIZE_240X176 == MMIDC_GetVideoSize())
    {
        videoinfo_ptr->height		= CAF_VIDEO_240X176_HEIGHT;
        videoinfo_ptr->width		= CAF_VIDEO_240X176_WIDTH;
    }
    else if(VIDEO_SIZE_176X144 == MMIDC_GetVideoSize())
    {
        videoinfo_ptr->height		= CAF_VIDEO_176X144_HEIGHT;
        videoinfo_ptr->width		= CAF_VIDEO_176X144_WIDTH;
    }
    else if(VIDEO_SIZE_160X128 == MMIDC_GetVideoSize())
    {
        videoinfo_ptr->height		= CAF_VIDEO_160X128_HEIGHT;
        videoinfo_ptr->width		= CAF_VIDEO_160X128_WIDTH;
    }
    else if(VIDEO_SIZE_128X96 == MMIDC_GetVideoSize())
    {
        videoinfo_ptr->height		= CAF_VIDEO_128X96_HEIGHT;
        videoinfo_ptr->width		= CAF_VIDEO_128X96_WIDTH;
    }
    else
    {
        return CAF_MM_NOTSUPPORTED;
    }
	////
    videoinfo_ptr->video_support = TRUE;

	return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : record media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Record(IMEDIA_T* imedia_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	uint16 wvideo=0,wauido=0;
	uint32 dwvideo=0;
	uint32 dwmt=0;
	wchar fpath[256]={0x0000};
#ifdef CAMERA_SUPPORT
    DRECORDER_PREVIEW_PARAM_T record_param = {0};
    DRECORDER_START_RECORD_T recorder_start_t = {0};
    MEDIA_AUDIO_INFO_T	audio_info = {0};
    MEDIA_VIDEO_INFO_T	video_info = {0};
    DRECORDER_RETURN_E  ret = DRECORDER_SUCCESS;
	CAF_MM_RESULT       cafret = CAF_MM_NOERROR;
#endif

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{
		return CAF_MM_INVALFLAG;
	}

	if(!media_ptr->isvideo)
	{
		//audio record
		if(AUDIO_NO_ERROR != AUDIO_Play(media_ptr->haud, 0))
		{
			//record fail
			return CAF_MM_UNKNOWNERROR;
		}


		media_ptr->estate = CAF_MM_STATE_RECORDING;

		return CAF_MM_NOERROR;
	}

	////
	wvideo = CAF_HIWORD(media_ptr->mediadata.dw_format);
	wauido = CAF_LOWORD(media_ptr->mediadata.dw_format);
	dwvideo=(wvideo<<16);
	TransferFileNameFromCAFToSys(fpath, media_ptr->mediadata.data_ptr);
	dwmt=CAFMEDIA_GetCodecAndDevByFileName((wchar*)fpath,NULL,NULL);
	if(CAF_MM_INVALIDVALUE == dwmt)
	{
		dwmt=(dwvideo)?dwvideo:wauido;//av优先
	}

#ifndef CAMERA_SUPPORT
    return CAF_MM_NOTSUPPORTED;
#else
	cafret=CAFMEDIA_InitAVInfo(dwmt,&audio_info,&video_info);
	if(CAF_MM_NOERROR != cafret)
    {
        return CAF_MM_NOTSUPPORTED;
    }
	/////
    if(MM_V_3GP == dwmt)
    {
		recorder_start_t.file_type	= DRECORDER_FILETYPE_3GP;
		MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_3GP);
    }
    else if(MM_V_MP4 == dwmt)
    {
		recorder_start_t.file_type	= DRECORDER_FILETYPE_MP4;
		MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_MP4);
		DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, DRECORDER_FILETYPE_MP4, PNULL);
    }
    else if(MM_V_MJPEG == dwmt)
    {
		recorder_start_t.file_type	= DRECORDER_FILETYPE_AVI;
		MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_AVI);
		DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, DRECORDER_FILETYPE_AVI, PNULL);
    }

	record_param.LcdId = 0;//GUI_MAIN_LCD_ID;
	if(SCREEN_MODE_VERTICAL == MMIDC_GetScreenMode())
	{
		record_param.disp_rect.dx = MMIDC_GetPreviewWidth();
		record_param.disp_rect.dy = MMIDC_GetPreviewHeight();
		record_param.DisplayRotateAngle = 0;
	}
	else if(SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
	{
		record_param.disp_rect.dx = MMIDC_GetPreviewHeight();
		record_param.disp_rect.dy = MMIDC_GetPreviewWidth();
		record_param.DisplayRotateAngle = 3;
	}

	record_param.target_rect = record_param.disp_rect;

	record_param.display_mode = DRECORDER_DISP_FULLVIEW;
	record_param.LcdBlkId = 0;
	record_param.image_format = MMIAPIENG_GetSensorMode();
	ret = DRECORDER_StartPreview(&record_param);

	if(DRECORDER_SUCCESS != ret)
	{
		return CAF_MM_NOTENABLED;
	}

	////
    recorder_start_t.audio_info_ptr		= &audio_info;
    recorder_start_t.video_info_ptr		= &video_info;
    recorder_start_t.frame_callback		= PNULL;
    recorder_start_t.end_callback		= (DRECORDER_END_CALLBACK)CAFMEDIA_RecordEndNotifyCallBack;


   recorder_start_t.file_name_ptr		= (uint8*)fpath;
   recorder_start_t.file_name_byte_len = MMIAPICOM_Wstrlen(fpath) * 2;

    if(!MMIDC_CreateVideoName(&recorder_start_t))
    {
        return CAF_MM_FILEOPENERR;
    }
    recorder_start_t.file_limit_size = 0;
    ret = DRECORDER_StartRecord(&recorder_start_t);
	if(DRECORDER_SUCCESS != ret)
	{
		return CAF_MM_UNKNOWNERROR;
	}
	media_ptr->estate = CAF_MM_STATE_RECORDING;

	return CAF_MM_NOERROR;
#endif  //CAMERA_SUPPORT

}
/*****************************************************************************/
// 	Description : pause a playing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Pause(IMEDIA_T* imedia_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{	//play
		if(media_ptr->isvideo)
		{	//3gp,mp4 etc.
			if( DPLAYER_SUCCESS != DPLAYER_Pause(media_ptr->hvideo))
			{
				//pause fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
		else
		{
			//mp3,midi,wav etc.
			if(AUDIO_NO_ERROR != AUDIO_Pause(media_ptr->haud))
			{
				//pause fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
	}
	else if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
	{
		//record
		if(media_ptr->isvideo)
		{
			if(DRECORDER_ERROR == DRECORDER_Pause())
			{
				//pause fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	media_ptr->estate = CAF_MM_STATE_PAUSE;


	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : resume a pausing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Resume(IMEDIA_T* imedia_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{	//play
		if(media_ptr->isvideo)
		{	//3gp,mp4 etc.
			if( DPLAYER_SUCCESS != DPLAYER_Resume(media_ptr->hvideo))
			{
				//resume fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
		else
		{
			//mp3,midi,wav etc.
			if(AUDIO_NO_ERROR != AUDIO_Resume(media_ptr->haud))
			{
				//resume fail
				return CAF_MM_UNKNOWNERROR;
			}
		}
	}
	else if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
	{
		if(media_ptr->isvideo)
		{
			if(DRECORDER_ERROR == DRECORDER_Resume())
			{
				//pause resume
				return CAF_MM_UNKNOWNERROR;
			}
		}
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}


	media_ptr->estate = CAF_MM_STATE_PLAYING;


	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : stop a playing media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Stop(IMEDIA_T* imedia_ptr)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;

	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

	if(MMW_MODE_PLAY == media_ptr->mediadata.eworkmode)
	{	//play
		if(media_ptr->isvideo)
		{	//3gp,mp4 etc.
			if( DPLAYER_SUCCESS != DPLAYER_Stop(media_ptr->hvideo))
			{
				//stop fail
				return CAF_MM_UNKNOWNERROR;
			}
			DPLAYER_Close(media_ptr->hvideo);
			media_ptr->hvideo=INVALID_HANDLE;
		}
		else
		{
			//mp3,midi,wav etc.
			if(AUDIO_NO_ERROR != AUDIO_Stop(media_ptr->haud))
			{
				//stop fail
				return CAF_MM_UNKNOWNERROR;
			}
			AUDIO_CloseHandle(media_ptr->haud);
			media_ptr->haud=INVALID_HANDLE;
		}
	}
	else if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode)
	{
		//record
		if(media_ptr->isvideo)
		{
			if(DRECORDER_ERROR == DRECORDER_StopRecord())
			{
				//stop fail
				return CAF_MM_UNKNOWNERROR;
			}

			DRECORDER_Close();
		}
	}
	else
	{
		return CAF_MM_NOTSUPPORTED;
	}

	media_ptr->estate = CAF_MM_STATE_STOP;

	return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : seek the position in a media
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	eseek			 定位引用
//	emode			 定位模式
//	lvalue			 偏移量
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_Seek(IMEDIA_T* imedia_ptr,CAFMediaSeek_E eseek,CAFMediaSeekMode_E emode,int lvalue)
{
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	uint32 dwvalue=lvalue,dwtotal=0,dwcur=0;
	AUDIO_PLAYINFO_T	pi;
	CAF_MM_RESULT ret = CAF_MM_NOERROR;
	DPLAYER_RETURN_E vret=DPLAYER_SUCCESS;
	AUDIO_RESULT_E   aret=AUDIO_NO_ERROR;
	if(NULL == imedia_ptr)
	{
		return CAF_MM_INVALPARAM;
	}
	/////////////////////////////////////////////
	if(MMW_MODE_PLAY != media_ptr->mediadata.eworkmode || (media_ptr->isvideo && MMS_MODE_TIME != emode))
	{
		//目前只支持播放seek
		//视频只支持按时间seek
		return CAF_MM_NOTSUPPORTED;
	}

	SCI_MEMSET(&pi,0x00,sizeof(AUDIO_PLAYINFO_T));
	switch(eseek)
	{
		case MM_SEEK_START:
			//time
			if(lvalue < 0) break;
			if(MMS_MODE_TIME == emode)
			{
				if(media_ptr->isvideo)
				{
					dwtotal=DPLAYER_GetLength(media_ptr->hvideo);
					if((uint32)lvalue > dwtotal) dwvalue=dwtotal;
					//set time
					vret=DPLAYER_SetTime(media_ptr->hvideo,dwvalue);
					ret=(DPLAYER_SUCCESS != vret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
				else //is audio
				{
					if(AUDIO_ERROR == AUDIO_GetPlayingInfo(media_ptr->haud,&pi))
					{
						break;
					}
					dwtotal=pi.uiTotalTime*1000;
					if((uint32)lvalue > dwtotal) dwvalue=dwtotal;
					//change times to bytes
					//dwcur=(dwvalue*pi.uiTotalDataLength)/(pi.uiTotalTime*1000);
					dwcur=(dwvalue/1000)*(pi.uiTotalDataLength/pi.uiTotalTime);
					//set offset
					aret=AUDIO_Seek(media_ptr->haud,dwcur);
					ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
			}
			else
			{
				//byte
				dwtotal=pi.uiTotalDataLength;
				if((uint32)lvalue > dwtotal) dwvalue=dwtotal;
				//set offset
				aret=AUDIO_Seek(media_ptr->haud,dwvalue);
				ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
			}
			break;
		case MM_SEEK_CURRENT:
			//time
			if(lvalue == 0) break;
			if(MMS_MODE_TIME == emode)
			{
				if(media_ptr->isvideo)
				{
					dwcur=DPLAYER_GetTime(media_ptr->hvideo);
					if(lvalue < 0)
					{
						//prev move
						if(dwcur <= (DWORD)(-lvalue)) dwvalue = 0;
						else
							dwvalue = dwcur + (DWORD)lvalue;
					}
					else
					{
						//back move
						dwtotal=DPLAYER_GetLength(media_ptr->hvideo);
						if(dwtotal <= (dwcur + (DWORD)lvalue)) dwvalue = dwtotal;
						else
							dwvalue = dwcur + (DWORD)lvalue;
					}
					//set time
					vret=DPLAYER_SetTime(media_ptr->hvideo,dwvalue);
					ret=(DPLAYER_SUCCESS != vret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
				else
				{
					dwcur=pi.uiCurrentTime*1000;
					if(lvalue < 0)
					{
						//prev move
						if(dwcur <= (uint32)(-lvalue)) dwvalue = 0;
						else
							dwvalue = dwcur + (uint32)lvalue;

						//dwcur=(dwvalue*pi.uiTotalDataLength)/(pi.uiTotalTime*1000);
						dwcur=(dwvalue/1000)*(pi.uiTotalDataLength/pi.uiTotalTime);
					}
					else
					{
						//back move
						dwtotal=pi.uiTotalTime*1000;
						if(dwtotal <= (dwcur + (uint32)lvalue)) dwvalue = dwtotal;
						else
							dwvalue = dwcur + (uint32)lvalue;
						//change times to bytes
						//dwcur=(dwvalue*pi.uiTotalDataLength)/(pi.uiTotalTime*1000);
						dwcur=(dwvalue/1000)*(pi.uiTotalDataLength/pi.uiTotalTime);
					}
					//set offset
					aret=AUDIO_Seek(media_ptr->haud,dwcur);
					ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
			}
			else
			{
				//byte
				dwcur=pi.uiCurrentDataOffset;
				if(lvalue < 0)
				{
					//prev move
					if(dwcur <= (uint32)(-lvalue)) dwvalue = 0;
					else
						dwvalue = dwcur + (uint32)lvalue;
				}
				else
				{
					//back move
					dwtotal=pi.uiTotalDataLength;
					if(dwtotal <= (dwcur + (uint32)lvalue)) dwvalue = dwtotal;
					else
						dwvalue = dwcur + (uint32)lvalue;
				}
				//set offset
				aret=AUDIO_Seek(media_ptr->haud,dwvalue);
				ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
			}
			break;
		case MM_SEEK_END:
			//time
			if(lvalue > 0) break;
			if(MMS_MODE_TIME == emode)
			{
				if(media_ptr->isvideo)
				{
					dwtotal=DPLAYER_GetLength(media_ptr->hvideo);
					if( (DWORD)(-lvalue) > dwtotal) dwvalue=0;
					else
						dwvalue = dwtotal+(DWORD)lvalue;
					//set time
					vret=DPLAYER_SetTime(media_ptr->hvideo,dwvalue);
					ret=(DPLAYER_SUCCESS != vret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
				else
				{
					dwtotal=pi.uiTotalTime*1000;
					if( (uint32)(-lvalue) > dwtotal) dwvalue=0;
					else
						dwvalue = dwtotal+(uint32)lvalue;
					//change times to bytes
					//dwcur=(dwvalue*pi.uiTotalDataLength)/(pi.uiTotalTime*1000);
					dwcur=(dwvalue/1000)*(pi.uiTotalDataLength/pi.uiTotalTime);
					//set offset
					aret=AUDIO_Seek(media_ptr->haud,dwcur);
					ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
				}
			}
			else
			{
				//byte
				//byte
				if(lvalue > 0) return CAF_MM_NOERROR;
				dwtotal=pi.uiTotalDataLength;
				if( (uint32)(-lvalue) > dwtotal) dwvalue=0;
				else
					dwvalue = dwtotal+(uint32)lvalue;
				//set offset
				aret=AUDIO_Seek(media_ptr->haud,dwvalue);
				ret=(AUDIO_NO_ERROR != aret)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
			}
			break;
		default:
			{
				ret = CAF_MM_NOTSUPPORTED;
			}
	}


	return ret;

}
/*****************************************************************************/
// 	Description : write a stream data to a buffer
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	data_ptr 		 待写入的流数据
//	dwsize_ptt 	待写入的流数据数据大小，及实际写入的数据大小
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_WriteStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr)
{
    unsigned int l=0,len=*dw_size_ptr;

	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;


	if(NULL == media_ptr || NULL == data_ptr || NULL == dw_size_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

    len = MIN(len, media_ptr->fifo.size - media_ptr->fifo.in + media_ptr->fifo.out);

    /* first put the data starting from fifo->in to buffer end */
    l = MIN(len, media_ptr->fifo.size - (media_ptr->fifo.in & (media_ptr->fifo.size - 1)));
    SCI_MEMCPY(media_ptr->fifo.buffer + (media_ptr->fifo.in & (media_ptr->fifo.size - 1)), data_ptr, l);

    /* then put the rest (if any) at the beginning of the buffer */
    SCI_MEMCPY(media_ptr->fifo.buffer, data_ptr + l, len - l);

    media_ptr->fifo.in += len;
	*dw_size_ptr=len;

	//录制时通知用户取数据
	if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode &&  MMD_STREAM == media_ptr->mediadata.clsdata)
	{
		/*lint -e718*/
		ISHELL_PostMessage((MMI_HANDLE_T)media_ptr->mediadata.user_ptr,MSG_DATA_WRITED,(void*)&len,sizeof(int));
        /*lint +e718*/
	}

    return CAF_MM_NOERROR;

}
/*****************************************************************************/
// 	Description : read a stream data from buffer
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	data_ptr		 存放待取的流数据空间
//	dwsize_ptr		存放待取的流数据空间大小，及实际获取的数据大小。
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_ReadStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr)
{
    unsigned int l=0,len=*dw_size_ptr;
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;


	if(NULL == imedia_ptr || NULL == data_ptr || NULL == dw_size_ptr)
	{
		return CAF_MM_INVALPARAM;
	}

    len = MIN(len, media_ptr->fifo.in - media_ptr->fifo.out);

    /* first get the data from fifo->out until the end of the buffer */
    l = MIN(len, media_ptr->fifo.size - (media_ptr->fifo.out & (media_ptr->fifo.size - 1)));
    SCI_MEMCPY(data_ptr, media_ptr->fifo.buffer + (media_ptr->fifo.out & (media_ptr->fifo.size - 1)), l);

    /* then get the rest (if any) from the beginning of the buffer */
    SCI_MEMCPY(data_ptr + l, media_ptr->fifo.buffer, len - l);

    media_ptr->fifo.out += len;
	*dw_size_ptr=len;


	//播放时通知用户放数据
	if(MMW_MODE_RECORD == media_ptr->mediadata.eworkmode &&  MMD_STREAM == media_ptr->mediadata.clsdata)
	{
        /*lint -e718*/
		ISHELL_PostMessage((MMI_HANDLE_T)media_ptr->mediadata.user_ptr,MSG_DATA_READ,(void*)&len,sizeof(int));
        /*lint +e718*/
	}

    return CAF_MM_NOERROR;
}
/*****************************************************************************/
// 	Description : set device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 		指向 IMEDIA_T 接口对象的指针
//  emode			设备模式枚举
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_SetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E emode)
{
	AUDIO_RESULT_E result=AUDIO_NO_ERROR;
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	if(NULL == imedia_ptr)
	{
		//参数无效
		return CAF_MM_INVALPARAM;
	}
	if(emode >= MDM_DEVICE_MODE_MAX )
	{
		media_ptr->edevmode = MDM_DEVICE_MODE_HANDFREE;
	}
	else
	{
		media_ptr->edevmode = emode;
	}

	result= AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)media_ptr->edevmode);

	return (AUDIO_NO_ERROR != result)?CAF_MM_UNKNOWNERROR:CAF_MM_NOERROR;
}

/*****************************************************************************/
// 	Description : get device mode
//	Global resource dependence :
//  Author:Riancy.Zhang
//	Param:
//	imedia_ptr 		指向 IMEDIA_T 接口对象的指针
//  emode_ptr			设备模式指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
LOCAL CAF_MM_RESULT CAFMEDIA_GetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E *emode_ptr)
{
	AUDIO_RESULT_E result;
	CAFMEDIA_T * media_ptr = (CAFMEDIA_T *)imedia_ptr;
	if(NULL == imedia_ptr)
	{
		//参数无效
		return CAF_MM_INVALPARAM;
	}

	result= AUDIO_GetDevMode((AUDIO_DEVICE_MODE_TYPE_E*)emode_ptr);
	if(result != AUDIO_NO_ERROR)
	{
		//获取不到的情况下,如果当前设备的备份
		//模式值来返回
		*emode_ptr=media_ptr->edevmode;
	}

	return CAF_MM_NOERROR;
}
#endif