/****************************************************************************
** File Name:      cafmedia.h                                              *
** Author:         Riancy.Zhang                                            *
** Date:           03/04/2009                                              *
** Copyright:      2008-2010 Spreatrum, Incoporated. All Rights Reserved.  *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2009        Riancy.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_MEDIA_H_    
#define  _CAF_MEDIA_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "caf.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define    CAF_NAME_MAXLEN			255
#define    CAF_ARTIST_MAXLEN		30
#define    CAF_ALBUM_MAXLEN			30
#define    CAF_COMMENT_MAXLEN		64
#define    CAF_YEAR_MAXLEN			4

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

// 媒体录制\播放类型
typedef enum 
{
	
	MMD_FILE_NAME,//以文件方式
		MMD_BUFFER,//以buffer方式
		MMD_STREAM //以流方式
	  
} CAFMediaCls_E;

//媒体seek模式
typedef enum {
	
   	MMS_MODE_POS = 1,//按具体位置(字节)
	MMS_MODE_TIME  //按时间(毫秒)
	   
} CAFMediaSeekMode_E;

//媒体工作模式
typedef enum {
	
   	MMW_MODE_PLAY = 0,//播放
	MMW_MODE_RECORD,  //录制
	MMW_MODE_BOTH  //录播
	
} CAFMediaWorkMode_E;

//brief Audio device mode type list.
typedef enum
{
	MDM_DEVICE_MODE_HANDHOLD,
	MDM_DEVICE_MODE_HANDFREE,
	MDM_DEVICE_MODE_EARPHONE,
	MDM_DEVICE_MODE_EARFREE,
	MDM_DEVICE_MODE_TVOUT,
	MDM_DEVICE_MODE_MAX
	
}CAFMediaDeviceMode_E;

//媒体录制\播放,音视频采用的格式
typedef enum {

	//音频
    MM_A_MIDI = 1,
	MM_A_WAVE,
	MM_A_MP3,
	MM_A_WMA,
	MM_A_AMR,
	MM_A_AAC,
	MM_A_M4A,
	
	//视频
	MM_V_3GP = 1<<16,
	MM_V_MP4 = 2<<16,
	MM_V_MJPEG = 3<<16

} CAFMediaFormat_E;

//媒体seek方式
typedef enum {
	
      MM_SEEK_START = 0,//从媒体开头搜索
      MM_SEEK_END,		//从媒体结尾搜索
      MM_SEEK_CURRENT	//从媒体当前位置搜索
      
} CAFMediaSeek_E;

//媒体信息结构
typedef struct
{
    char 	name   [CAF_NAME_MAXLEN +1];//片名
    char 	artist [CAF_ARTIST_MAXLEN +1];//主演\作曲\演唱者
    char 	album  [CAF_ALBUM_MAXLEN +1];//出版商
    char 	year   [CAF_YEAR_MAXLEN+1];//发行年代
	char 	comment[CAF_COMMENT_MAXLEN +1];//附加信息
	uint32  	ntimes;//播放时长(秒)

	int   	nframes;//帧数
	int   	nsamplerate;//采样率
	//用于视频
	int  	nwidth;//图像宽度
	int  	nheight;//图像高度
	
} CAFMediaInfo_T;

//媒体数据
typedef struct
{
	CAFMediaCls_E   clsdata;//媒体数据类型
	void *          data_ptr;//数据地址
	uint32          dw_size;//数据大小(对于播放,系统会自动附上数据大小)
	void *			user_ptr;//通信对象(通常是窗口句柄,以便给用户通信)

	uint32			dw_format;//媒体格式(对于播放，没有高16位和低16位之分,
							//通常把格式值放在低16位即可;如果播放时不设置,
							//即给0值,说明让系统自动区分;对于录制,有高16
							//位和低16位之分,高16位通常是视频格式,低16位
							//通常是音频格式;如果录制时都不设置,即都给0值,
							//说明让系统使用缺省值)
	CAFMediaWorkMode_E	eworkmode; //工作模式
     
 }CAFMediaData_T;
/*
下实例表中列出了预定义媒体数据类型的上下文敏感数据的详细信息["读/写"的意思是"播放/录制"]。 
类型             模式     数据        大小               格式
----------------------------------------------------------------------------------
MMD_FILE_NAME   读/写    文件名         0                MAKELONG(MM_A_AAC,MM_V_3GP)

MMD_BUFFER	  	读/写    缓冲区指针     数据大小或 0     MM_V_3GP

MMD_STREAM	   只读      缓冲区指针     数据大小或 0     0  

(1) 对于播放，可将 cls 设置为任何CLSID。 
(2) 对于记录，只允许使用 MMD_FILE_NAME/MMD_BUFFER 类型。 现有文件或内存缓冲区将被覆盖。
*/

typedef struct IMEDIA_T IMEDIA_T;


// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IMEDIA_T)
{
    INHERIT_IUNKNOWN(IMEDIA_T);

	
	CAF_MM_RESULT (*CAFMEDIA_SetMediaData)(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr);
    CAF_MM_RESULT	(*CAFMEDIA_GetMediaData)(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr );

	CAF_MM_RESULT (*CAFMEDIA_SetVolume)(IMEDIA_T* imedia_ptr,uint32 dw_volume);
	CAF_MM_RESULT (*CAFMEDIA_GetVolume)(IMEDIA_T* imedia_ptr,uint32* dw_volume_ptr);
	
	CAF_MM_RESULT (*CAFMEDIA_SetRect)(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr);
	CAF_MM_RESULT (*CAFMEDIA_GetRect)(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr);
	
	CAF_MM_RESULT (*CAFMEDIA_GetState)(IMEDIA_T* imedia_ptr,uint32* dw_state_ptr);
    CAF_MM_RESULT (*CAFMEDIA_GetMediaInfo)(IMEDIA_T* imedia_ptr,CAFMediaInfo_T* mediainfo_ptr);
	
	CAF_MM_RESULT (*CAFMEDIA_Play)(IMEDIA_T* imedia_ptr);
	CAF_MM_RESULT (*CAFMEDIA_Record)(IMEDIA_T* imedia_ptr);
	CAF_MM_RESULT (*CAFMEDIA_Pause)(IMEDIA_T* imedia_ptr);
	CAF_MM_RESULT (*CAFMEDIA_Resume)(IMEDIA_T* imedia_ptr);	
	CAF_MM_RESULT (*CAFMEDIA_Stop)(IMEDIA_T* imedia_ptr);
	CAF_MM_RESULT (*CAFMEDIA_Seek)(IMEDIA_T* imedia_ptr,CAFMediaSeek_E eseek,CAFMediaSeekMode_E emode,int lvalue);


	CAF_MM_RESULT (*CAFMEDIA_WriteStreamData)(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr);
	CAF_MM_RESULT (*CAFMEDIA_ReadStreamData)(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr);

	CAF_MM_RESULT (*CAFMEDIA_SetDevMode)(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E emode);
	CAF_MM_RESULT (*CAFMEDIA_GetDevMode)(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E *emode_ptr);
};

// Define pointer to poit the virtual table of the media class
struct IMEDIA_T
{
    CAF_VTBL_T(IMEDIA_T)  *vtbl_ptr;
	
};



/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : query interface
//	Global resource dependence : 
//	Author:Riancy.Zhang
//	Param:
//	imedia_ptr		 指向 IMedia 接口对象的指针。
//	guid			 标识正在尝试查询的实体（接口或数据）的全局唯一 id。 
//	ojbect_pptr 	指向要检索的接口或数据的指针。 如果传递回来的值为 NULL，则表示要查询的接口或数据不存在
//	Return:
//	FALSE  :  失败
//	TRUE   :  成功
//	Note:
/*****************************************************************************/
static __inline CAF_RESULT_E IMEDIA_QueryInterface( IMEDIA_T *imedia_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
	CAF_ASSERT( PNULL != imedia_ptr );
	return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->QueryInterface( imedia_ptr, guid, object_pptr );
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
static __inline uint32 IMEDIA_AddRef( IMEDIA_T * imedia_ptr ) 
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->AddRef( imedia_ptr );
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
static __inline uint32 IMEDIA_Release( IMEDIA_T * imedia_ptr ) 
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->Release( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_SetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_SetMediaData( imedia_ptr,medidadata_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_GetMediaData(IMEDIA_T* imedia_ptr,CAFMediaData_T* medidadata_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetMediaData( imedia_ptr,medidadata_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_GetVolume( IMEDIA_T* imedia_ptr, uint32* dw_volume_ptr) 
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetVolume(imedia_ptr, dw_volume_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_SetVolume( IMEDIA_T* imedia_ptr, uint32 dw_volume)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_SetVolume( imedia_ptr, dw_volume);
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
static __inline CAF_MM_RESULT IMEDIA_SetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_SetRect( imedia_ptr,rimage_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_GetRect(IMEDIA_T* imedia_ptr,CAF_RECT_T* rimage_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetRect( imedia_ptr,rimage_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_GetState(IMEDIA_T* imedia_ptr,uint32* dwstate_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetState( imedia_ptr,dwstate_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_GetMediaInfo(IMEDIA_T* imedia_ptr,CAFMediaInfo_T* mediainfo_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetMediaInfo( imedia_ptr,mediainfo_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_Play(IMEDIA_T* imedia_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Play( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_Record(IMEDIA_T* imedia_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Record( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_Pause(IMEDIA_T* imedia_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Pause( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_Resume(IMEDIA_T* imedia_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Resume( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_Stop(IMEDIA_T* imedia_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Stop( imedia_ptr );
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
static __inline CAF_MM_RESULT IMEDIA_Seek(IMEDIA_T* imedia_ptr,CAFMediaSeek_E eseek,CAFMediaSeekMode_E emode,int lvalue)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_Seek( imedia_ptr,eseek,emode,lvalue);
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
static __inline CAF_MM_RESULT IMEDIA_WriteStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_WriteStreamData( imedia_ptr,data_ptr,dw_size_ptr);
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
static __inline CAF_MM_RESULT IMEDIA_ReadStreamData(IMEDIA_T* imedia_ptr,uint8* data_ptr,uint32* dw_size_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_ReadStreamData( imedia_ptr,data_ptr,dw_size_ptr);	
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
static __inline CAF_MM_RESULT IMEDIA_SetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E emode)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_SetDevMode( imedia_ptr,emode);
}
/*****************************************************************************/
// 	Description : set device mode
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
static __inline CAF_MM_RESULT IMEDIA_GetDevMode(IMEDIA_T* imedia_ptr,CAFMediaDeviceMode_E *emode_ptr)
{
    CAF_ASSERT( PNULL != imedia_ptr );
    return CAF_GET_VTBLPTR( imedia_ptr, IMEDIA_T )->CAFMEDIA_GetDevMode( imedia_ptr,emode_ptr);
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
