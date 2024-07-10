/****************************************************************************
** File Name:      cafsound.h                                              *
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

#ifndef  _CAF_SOUND_H_    
#define  _CAF_SOUND_H_  

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

	
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

// tone ID to define
typedef enum {
   CAF_RING_FIRST,
   //按键音
   CAF_TONE_0,
   CAF_TONE_1,
   CAF_TONE_2,
   CAF_TONE_3,
   CAF_TONE_4,
   CAF_TONE_5,
   CAF_TONE_6,
   CAF_TONE_7,
   CAF_TONE_8,
   CAF_TONE_9,
   CAF_TONE_A,
   CAF_TONE_B,
   CAF_TONE_C,
   CAF_TONE_D,
   CAF_TONE_POUND,	//#
   CAF_TONE_STAR,	//*
   //短信音
   CAF_SMS_RING,
   //彩信音
   CAF_MMS_RING,
   //电压音
   CAF_POWER_RING,
   //滑盖音
   CAF_FLIP_RING,
   //删除音
   CAF_DEL_RING,
   //选中音
   CAF_SELECTED_RING,

   CAF_RING_LAST
   
} CAFSoundTone_E;

// Sound mode to play
typedef enum {
	
   CSM_MODE_TIMES = 1,//按次播放
   CSM_MODE_DURATION  //按时间播放(毫秒)
	
} CAFSoundMode_E;

//brief Audio device mode type list.
typedef enum
{
	SDM_DEVICE_MODE_HANDHOLD,
	SDM_DEVICE_MODE_HANDFREE,
	SDM_DEVICE_MODE_EARPHONE,
	SDM_DEVICE_MODE_EARFREE,
	SDM_DEVICE_MODE_TVOUT,
	SDM_DEVICE_MODE_MAX
	
}CAFSoundDeviceMode_E;


//Tone data
typedef struct
{
   CAFSoundTone_E etone;
   CAFSoundMode_E  emode; 
   uint32 dw_value;	//the value of mode

} CAFSoundToneData_T ;

//typedef struct ISOUND_T ;
typedef struct ISOUND_T ISOUND_T;


// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(ISOUND_T)
{
    INHERIT_IUNKNOWN(ISOUND_T);

    CAF_MM_RESULT (*CAFSOUND_SetVolume)( ISOUND_T* isound_ptr, uint32 dw_volume);
    CAF_MM_RESULT	(*CAFSOUND_GetVolume)( ISOUND_T* isound_ptr, uint32* dw_volume_ptr);

	CAF_MM_RESULT (*CAFSOUND_PlayTone)( ISOUND_T* isound_ptr, CAFSoundToneData_T tonedata);
	CAF_MM_RESULT (*CAFSOUND_PlayToneList)( ISOUND_T* isound_ptr, CAFSoundToneData_T* tonedata_ptr,uint16 wdatalen);
	
	CAF_MM_RESULT (*CAFSOUND_StopTone)( ISOUND_T* isound_ptr);

	CAF_MM_RESULT (*CAFSOUND_SetDevMode)(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E emode);
	CAF_MM_RESULT (*CAFSOUND_GetDevMode)(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E *emode_ptr);
};

// Define pointer to poit the virtual table of the sound class
struct ISOUND_T
{
    CAF_VTBL_T(ISOUND_T)  *vtbl_ptr;
	
};



/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : query interface
//	Global resource dependence : 
//	Author:Riancy.Zhang
//	Param:
//	isound_ptr	指向 ISOUND_T 接口对象的指针。
//	guid		标识正在尝试查询的实体（接口或数据）的全局唯一 id。 
//	ojbect_pptr 	指向要检索的接口或数据的指针。 如果传递回来的值为 NULL，
//				则表示要查询的接口或数据不存在。
//
//	Return:
//	FALSE  :  失败
//	TRUE   :  成功
//	Note:
/*****************************************************************************/
static __inline CAF_RESULT_E ISOUND_QueryInterface( ISOUND_T *isound_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
	CAF_ASSERT( PNULL != isound_ptr );
	return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->QueryInterface( isound_ptr, guid, object_pptr );
}
/*****************************************************************************/
// 	Description : add ref
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	Return:
//	Reference Count 增加的对象引用计数
//	Note:
/*****************************************************************************/
static __inline uint32 ISOUND_AddRef( ISOUND_T * isound_ptr ) 
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->AddRef( isound_ptr );
}
/*****************************************************************************/
// 	Description : release
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	Return:
//	Reference Count 减少的对象引用计数。
//	0 (零)如果已经释放对象,且不再有效
//	Note:
/*****************************************************************************/
static __inline uint32 ISOUND_Release( ISOUND_T * isound_ptr ) 
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->Release( isound_ptr );
}
/*****************************************************************************/
// 	Description : get volume
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	dwvolume_ptr		 音量值存储地址
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_GetVolume( ISOUND_T* isound_ptr, uint32* dw_volume_ptr) 
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_GetVolume( isound_ptr, dw_volume_ptr);
}
/*****************************************************************************/
// 	Description : set volume
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	dwvolume		设备/ 方法的新音量值
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_SetVolume( ISOUND_T* isound_ptr, uint32 dw_volume)
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_SetVolume( isound_ptr, dw_volume);
}
/*****************************************************************************/
// 	Description : play tone
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	dwvolume_ptr		 音量值存储地址
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_PlayTone( ISOUND_T* isound_ptr, CAFSoundToneData_T tonedata)
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_PlayTone( isound_ptr, tonedata);
}
/*****************************************************************************/
// 	Description : play tone-list
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	tonedata	CAFSoundToneData_T 结构列表数组
//	wdatalen		tonedata_ptr 列表中的音质数
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_PlayToneList( ISOUND_T* isound_ptr, CAFSoundToneData_T* tonedata_ptr,uint16 wdatalen)
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_PlayToneList( isound_ptr, tonedata_ptr,wdatalen);
}
/*****************************************************************************/
// 	Description : stop tone
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_StopTone( ISOUND_T* isound_ptr) 
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_StopTone( isound_ptr);
}
/*****************************************************************************/
// 	Description : set device mode
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//  emode			设备模式枚举
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_SetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E emode)
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_SetDevMode( isound_ptr,emode);
}
/*****************************************************************************/
// 	Description : get device mode
//	Global resource dependence : 
//  Author:Riancy.Zhang
//	Param:
//	isound_ptr 		指向 ISOUND_T 接口对象的指针
//  emode			设备模式指针
//	Return:
//	>0  :  错误返回码
//	0   :  没有错误
//	Note:
/*****************************************************************************/
static __inline CAF_MM_RESULT ISOUND_GetDevMode(ISOUND_T* isound_ptr,CAFSoundDeviceMode_E *emode_ptr)
{
    CAF_ASSERT( PNULL != isound_ptr );
    return CAF_GET_VTBLPTR( isound_ptr, ISOUND_T )->CAFSOUND_GetDevMode( isound_ptr,emode_ptr);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
