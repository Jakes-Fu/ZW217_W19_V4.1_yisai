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

// ý��¼��\��������
typedef enum 
{
	
	MMD_FILE_NAME,//���ļ���ʽ
		MMD_BUFFER,//��buffer��ʽ
		MMD_STREAM //������ʽ
	  
} CAFMediaCls_E;

//ý��seekģʽ
typedef enum {
	
   	MMS_MODE_POS = 1,//������λ��(�ֽ�)
	MMS_MODE_TIME  //��ʱ��(����)
	   
} CAFMediaSeekMode_E;

//ý�幤��ģʽ
typedef enum {
	
   	MMW_MODE_PLAY = 0,//����
	MMW_MODE_RECORD,  //¼��
	MMW_MODE_BOTH  //¼��
	
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

//ý��¼��\����,����Ƶ���õĸ�ʽ
typedef enum {

	//��Ƶ
    MM_A_MIDI = 1,
	MM_A_WAVE,
	MM_A_MP3,
	MM_A_WMA,
	MM_A_AMR,
	MM_A_AAC,
	MM_A_M4A,
	
	//��Ƶ
	MM_V_3GP = 1<<16,
	MM_V_MP4 = 2<<16,
	MM_V_MJPEG = 3<<16

} CAFMediaFormat_E;

//ý��seek��ʽ
typedef enum {
	
      MM_SEEK_START = 0,//��ý�忪ͷ����
      MM_SEEK_END,		//��ý���β����
      MM_SEEK_CURRENT	//��ý�嵱ǰλ������
      
} CAFMediaSeek_E;

//ý����Ϣ�ṹ
typedef struct
{
    char 	name   [CAF_NAME_MAXLEN +1];//Ƭ��
    char 	artist [CAF_ARTIST_MAXLEN +1];//����\����\�ݳ���
    char 	album  [CAF_ALBUM_MAXLEN +1];//������
    char 	year   [CAF_YEAR_MAXLEN+1];//�������
	char 	comment[CAF_COMMENT_MAXLEN +1];//������Ϣ
	uint32  	ntimes;//����ʱ��(��)

	int   	nframes;//֡��
	int   	nsamplerate;//������
	//������Ƶ
	int  	nwidth;//ͼ����
	int  	nheight;//ͼ��߶�
	
} CAFMediaInfo_T;

//ý������
typedef struct
{
	CAFMediaCls_E   clsdata;//ý����������
	void *          data_ptr;//���ݵ�ַ
	uint32          dw_size;//���ݴ�С(���ڲ���,ϵͳ���Զ��������ݴ�С)
	void *			user_ptr;//ͨ�Ŷ���(ͨ���Ǵ��ھ��,�Ա���û�ͨ��)

	uint32			dw_format;//ý���ʽ(���ڲ��ţ�û�и�16λ�͵�16λ֮��,
							//ͨ���Ѹ�ʽֵ���ڵ�16λ����;�������ʱ������,
							//����0ֵ,˵����ϵͳ�Զ�����;����¼��,�и�16
							//λ�͵�16λ֮��,��16λͨ������Ƶ��ʽ,��16λ
							//ͨ������Ƶ��ʽ;���¼��ʱ��������,������0ֵ,
							//˵����ϵͳʹ��ȱʡֵ)
	CAFMediaWorkMode_E	eworkmode; //����ģʽ
     
 }CAFMediaData_T;
/*
��ʵ�������г���Ԥ����ý���������͵��������������ݵ���ϸ��Ϣ["��/д"����˼��"����/¼��"]�� 
����             ģʽ     ����        ��С               ��ʽ
----------------------------------------------------------------------------------
MMD_FILE_NAME   ��/д    �ļ���         0                MAKELONG(MM_A_AAC,MM_V_3GP)

MMD_BUFFER	  	��/д    ������ָ��     ���ݴ�С�� 0     MM_V_3GP

MMD_STREAM	   ֻ��      ������ָ��     ���ݴ�С�� 0     0  

(1) ���ڲ��ţ��ɽ� cls ����Ϊ�κ�CLSID�� 
(2) ���ڼ�¼��ֻ����ʹ�� MMD_FILE_NAME/MMD_BUFFER ���͡� �����ļ����ڴ滺�����������ǡ�
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	guid			 ��ʶ���ڳ��Բ�ѯ��ʵ�壨�ӿڻ����ݣ���ȫ��Ψһ id�� 
//	ojbect_pptr 	ָ��Ҫ�����Ľӿڻ����ݵ�ָ�롣 ������ݻ�����ֵΪ NULL�����ʾҪ��ѯ�Ľӿڻ����ݲ�����
//	Return:
//	FALSE  :  ʧ��
//	TRUE   :  �ɹ�
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	���ü��������ӵĶ������ü���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	���ü��������ӵĶ������ü���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	medidadata_ptr 		ý������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	medidadata_ptr	 ý������
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	dw_volume_ptr		 ָ������ֵ��ָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	dw_volume		������ֵ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	rimage_ptr		 ͼ�����ָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	rimage_ptr		ͼ�����ָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	dwstate_ptr		״ֵ̬
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	mediainfo_ptr			 ý����Ϣ
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	eseek			 ��λ���� 
//	emode			 ��λģʽ
//	lvalue			 ƫ����
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	data_ptr 		 ��д���������
//	dwsize_ptt 	��д������������ݴ�С����ʵ��д������ݴ�С
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr		 ָ�� IMedia �ӿڶ����ָ�롣
//	data_ptr		 ��Ŵ�ȡ�������ݿռ�
//	dwsize_ptr		��Ŵ�ȡ�������ݿռ��С����ʵ�ʻ�ȡ�����ݴ�С��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr 		ָ�� IMEDIA_T �ӿڶ����ָ��
//  emode			�豸ģʽö��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
//	imedia_ptr 		ָ�� IMEDIA_T �ӿڶ����ָ��
//  emode_ptr			�豸ģʽָ��
//	Return:
//	>0  :  ���󷵻���
//	0   :  û�д���
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
