/*****************************************************************************
** File Name:      mmivp_locallist.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#ifndef _MMIVP_LOCALLIST_H_
#define _MMIVP_LOCALLIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "mmifilearray_export.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_LOCALLIST_FILENAME           L"movie_list.dat"  //���ز����б���Ĭ���ļ���
#define VP_LOCALLIST_FILENAME_LEN       14                 //���ز����б��ŵ�Ĭ���ļ�������
#define VP_LOCALLIST1_FILENAME           L"movie_list1.dat"  //���ز����б���Ĭ���ļ���
#define VP_LOCALLIST1_FILENAME_LEN       15                 //���ز����б��ŵ�Ĭ���ļ�������
#define VP_FILE_SUFFIX_MAX_LEN          7                   //�ļ���׺����󳤶�

#define VP_LOCALLIST_MIN_SAPCE     (1024<<5)  //add by sandy for cr246623, the same as MMIMP3_LIST_MIN_MEM;

#define VP_LOCALLIST_UDISK_UNICODE   'D'  //add by sandy for cr246623
#define VP_LOCALLIST_SDCARD_UNICODE   'E'  //add by sandy for cr246623
#define VP_LOCALLIST_SDCARD1_UNICODE   'F'  //add by sandy for cr246623
#define VP_LOCALLIST_MAX_DEV_UNICODE   'G'  //add by sandy for cr246623

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef enum
{
    VP_LOCALLIST_DEFAULT, //ϵͳĿ¼�б�
    VP_LOCALLIST_USER, //�û��Զ����б�
    VP_LOCALLIST_MAX
}VP_LOCALLIST_TYPE_E;  //���ز����б�����

//������Ƶ�ļ���֧������Լ������Ϣ
typedef struct 
{
    uint8   suffix[VP_FILE_SUFFIX_MAX_LEN]; //��Ƶ�ļ���׺
    uint32  type;                           //��Ƶ�ļ�����
    BOOLEAN is_support;                     //�Ƿ�֧��
}VP_SUFFIX_T;

//���ڲ����б�����Ϣ
typedef struct  
{
    int32     index;        //���ز����б�����
    BOOLEAN    is_ok;       //��Ƶ�ļ��Ƿ������������
}VP_PLAYINGLIST_ITEM_T;

//���ڲ����б�
typedef struct
{
    int32      start_index;                                 //��ʼ������Ƶ�����ڲ����б��е�����
    int32      active_index;                                //��ǰ���ŵ���Ƶ�����ڲ����б��е�����
    VP_PLAYINGLIST_ITEM_T     *playing_list_ptr;            //���ڲ����б�����Ϣ
} VP_PLAYINGLIST_T;

typedef struct  
{
    VP_HANDLE           vp_handle;
    int32               locallist_index;   //ĳһ�����ز����б��е�����
    VP_LOCALLIST_TYPE_E locallist_type;  //���ز����б�����
}VP_LOCALLIST_INFO_T; //���ز����б���Ϣ

typedef enum
{
    VP_OP_LIST_SUCCESS,
    VP_OP_LIST_SAVE_ERROR,
    VP_OP_LIST_NO_SPACE,
    VP_OP_LIST_ERROR,
    VP_OP_LIST_MAX
}VP_OP_LIST_RESULT_E;

typedef void (*VP_OPLISTCALLBACK)( VP_OP_LIST_RESULT_E    result, VP_LOCALLIST_TYPE_E locallist_type);  //�б�����Ļص�����
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ����������������б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_ResetCurPlayList(BOOLEAN   is_random_play);

/*****************************************************************************/
//  Description : ����ģʽ��ȡ���ز����б��е���һ����Ƶ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetPreByMode(wchar *full_path_ptr,
                                            uint16 *full_path_len_ptr,
                                            VP_REPEAT_PLAY_MODE_E mode,
                                            BOOLEAN   is_random_play
                                           );

/*****************************************************************************/
//  Description : ����ģʽ��ȡ���ز����б��е���һ����Ƶ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetNextByMode(wchar *full_path_ptr,
                                            uint16 *full_path_len_ptr,
                                            VP_REPEAT_PLAY_MODE_E mode,
                                            BOOLEAN   is_random_play
                                           );
/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetIsCurPlayListItemOK(BOOLEAN   is_ok);

/*****************************************************************************/
//  Description : ������������б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_DestroyPlayList(void);
/*****************************************************************************/
//  Description : ������������б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_CreateCurPlayList(BOOLEAN   is_random_play);

/*****************************************************************************/
//  Description : ��ʼ�첽���ر��ز����б������״δ򿪲������Լ������б�Ϊ�գ��Զ�����ָ��Ŀ¼�е���Ƶ�ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note:  Sandy.Wan,2011-03-31�� ����һ��file_dev_type ����������ڼ����Ժ�ƽ̨����洢���������
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_BeginLoad(
                                        MMI_WIN_ID_T        win_id,  
                                        MMI_MESSAGE_ID_E    msg_id,
                                        VP_LOCALLIST_TYPE_E  locallist_type,
                                        MMIFILE_DEVICE_E     file_dev_type);

/*****************************************************************************/
//  Description : �첽���ر��ز����б���ɣ����汾�ز����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndLoad(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : �ͷű��ز����б���Դ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_Release(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ��ȡ���ز����б���ĳһ�����Ϣ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetInfo(
                                    int32               locallist_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr,
                                    VP_LOCALLIST_TYPE_E  locallist_type
                                    );

/*****************************************************************************/
//  Description : ��ȡ���ز����б���б������Ŀ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_LocalList_GetNum( VP_LOCALLIST_TYPE_E  locallist_type );

/*****************************************************************************/
//  Description : ���ļ��������������Ƶ�ļ��������б���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_AddItem(VP_OPLISTCALLBACK callback,
                                 VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ���µ�ǰ���ز����б�����u�̺�sd��������֧�ֵ���Ƶ�ļ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_BeginUpdate(                                     
                                        MMI_WIN_ID_T        win_id,  
                                        MMI_MESSAGE_ID_E    msg_id,
                                        VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : �첽���±��ز����б���ɣ����汾�ز����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndUpdate(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ɾ�����ز����б��е�ĳһ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_DelItemWithouSaveFile(int32        index,
                                                  VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ���汾�ز����б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_Locallist_SaveToFile(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ��ȡ���ز����б�ǰ�����ļ���������
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC int32 VP_LocalList_GetCurrentIndex(VP_LOCALLIST_TYPE_E locallist_type);

/*****************************************************************************/
//  Description : ���õ�ǰ���ŵ���Ƶ�ļ�λ�ڱ��ز����б��е�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurrentIndex(int32 index);

/*****************************************************************************/
//  Description : ��ȡ��ǰ���ز����б����͡�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LOCALLIST_TYPE_E VP_LocalList_GetCurListType(void);

/*****************************************************************************/
//  Description : ���õ�ǰ���ز����б��е�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurListType(VP_LOCALLIST_TYPE_E type);

/*****************************************************************************/
//  Description : ��ȡ���ز����б��е���һ����Ƶ��Ϣ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetNext(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : ��ȡ���ز����б����һ����Ƶ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetPrev(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : ��ʼ��������ŵ�˳���б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_InitDifferentRandomList(void);

/*****************************************************************************/
//  Description : ����Ƿ��к��ʵĴ洢���Ҵ洢���ռ��㹻��
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note: add for cr246623
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_GetSuitableDeviceState(void);

/*****************************************************************************/
//  Description : ɾ�������̷�����������ļ���ͬ��ͬ·���ļ���
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note:  add for cr246623
/*****************************************************************************/
PUBLIC void VP_LocalList_DeleteSameNameFile( uint16     *file_name, int16     file_name_len);
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
/*****************************************************************************/
//  Description : �Ƿ���²����б� 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_IsNeedReSearchLocallist( void );
/*****************************************************************************/
//  Description : �����û��Զ��岥���б� 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN VP_LocalList_LoadUserLocalList(void);
#endif


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

#endif


