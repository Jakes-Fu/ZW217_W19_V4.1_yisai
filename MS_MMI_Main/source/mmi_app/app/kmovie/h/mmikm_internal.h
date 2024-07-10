/***************************************************************************
** File Name:      mmikm_internal.h                                        *
** Author:                                                                 *
** Date:           14/07/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011          tonny.chen          Create                              *
**																		   *
****************************************************************************/
#ifndef _MMIKM_INTERNAL_H_
#define _MMIKM_INTERNAL_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "mmifmm_export.h"

#include "mmikm_playlist.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#ifndef  MMI_OK
#define  MMI_OK (0x00)
#endif

#ifndef  MMI_ERROR
#define  MMI_ERROR  (0xFF)
#endif

#define KM_FILE_INFO_THUMBNAIL_MAX_H           81
#define KM_FILE_INFO_THUMBNAIL_MAX_W           81
#define KM_FILE_INFO_THUMBNAIL_ENLARGE_SIZE    40

#define KM_FILE_INFO_NAME_LEN_MAX              MMIFMM_FILE_FILE_NAME_MAX_LEN //���Ƶ���󳤶�
#define KM_FILE_INFO_TIME_LEN_MAX              16//8*2
#define KM_FILE_INFO_YEAR_LEN_MAX              16//8*2
#define KM_FILE_INFO_ACTOR_LEN_MAX             150//50*3
#define KM_FILE_INFO_SYNOPSIS_LEN_MAX          600//200*3
#define KM_FILE_INFO_FULL_NAME_LEN_MAX         MMIFMM_FILE_FILE_NAME_MAX_LEN    //���Ƶ���󳤶�

#ifdef MMI_KINGMOVIE_50_MOVIE_SUPPORT //6530_MINI
#define KM_FILE_INFO_NUM_MAX                   50//MINI�汾��ʡ�ڴ��ʡ�ڴ�
#else
#define KM_FILE_INFO_NUM_MAX                   100//100//50 
#endif  

#define KM_FILE_SUFFIX_STR                     L".kmv"
#define KM_THUMBNAIL_SUFFIX_STR                L".bmp"
#define SMV_FILE_SUFFIX_STR                    L".smv"//smv_support


#define HMV_FILE_SUFFIX_STR                    L".hmv"//hmv_support


#define KM_LOG_SUFFIX_STR                      L".dat"
#ifdef WIN32
#define KM_DEBUG_SWITCH_ENABLE   //for win32 debug swith 
#endif
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
 
//.kmv file context type 
typedef enum
{
    MMIKM_TYPE_ACTION,
    MMIKM_TYPE_ADVENTURE,          
    MMIKM_TYPE_ANIMATION,
    MMIKM_TYPE_COMEDY, 
    MMIKM_TYPE_DOCUMENTARY,
    MMIKM_TYPE_DRAMA, //5
    MMIKM_TYPE_FAMILY, 
    MMIKM_TYPE_FANTASY,
    MMIKM_TYPE_HORROR,
    MMIKM_TYPE_MUSIC, 
    MMIKM_TYPE_MYSTERY, //10
    MMIKM_TYPE_PERFORMING, 
    MMIKM_TYPE_ROMANCE,
    MMIKM_TYPE_SCIFI, 
    MMIKM_TYPE_SPORTS, 
    MMIKM_TYPE_SUSPENSE,//15
    MMIKM_TYPE_WAR, 
    MMIKM_TYPE_WESTERN,
    MMIKM_TYPE_OTHER, //18
    MMIKM_TYPE_MAX, //19
}MMIKM_TYPE_E;
                           
typedef struct MMIKM_FILE_ITEM_INFO_T
{
    BOOLEAN                     is_valid;
    MMIKM_TYPE_E                file_contx_type;
    
    uint16                      cur_indx;
    uint16                      file_full_path_len;
    uint16                      file_info_name_len;
    wchar                       file_full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1];
    wchar                       file_info_name[KM_FILE_INFO_NAME_LEN_MAX+1];
    wchar                       file_info_time[KM_FILE_INFO_TIME_LEN_MAX+1];
    wchar                       file_info_year[KM_FILE_INFO_YEAR_LEN_MAX+1];
} MMIKM_FILE_ITEM_INFO_T;

typedef struct MMIKM_PLAY_LIST_INFO_T
{
    MMIKM_FILE_ITEM_INFO_T      km_file_item_info[KM_FILE_INFO_NUM_MAX];
    
} MMIKM_PLAY_LIST_INFO_T;

typedef struct MMIKM_PLAY_INFO_T
{
    uint16                      total_num;
    uint16                      cur_file_full_path_len;
    uint32                      played_time;
   
    wchar                       cur_file_full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1];

    uint16                      type_num[MMIKM_TYPE_MAX + 1];
   
} MMIKM_PLAY_INFO_T;

typedef struct MMIKM_LOAD_SAVE_INFO_T
{
    uint32                     cur_list_index;
    uint32                     list_num;
    KM_PLAYLIST_TYPE_E  list_type;
} MMIKM_LOAD_SAVE_INFO_T;

//km file context type 
typedef enum
{
    MMIKM_FILE_TYPE_PLAY_LIST,   
    MMIKM_FILE_TYPE_PLAY_LIST_INFOR,    
    MMIKM_FILE_TYPE_PLAY_INFOR, 
    MMIKM_FILE_TYPE_MAX,
}MMIKM_LIST_FILE_TYPE_E;
/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  ���ر��ز����б�ȴ����ڴ�����
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePlaylistSearchWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
/*****************************************************************************/
//  Description : create main window
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MMIKM_CreateMainWin(void);

/*****************************************************************************/
//  Description : check km files 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_CheckFile(void);
#ifndef MMI_KM_NEED_SEARCH_FIRST_TIME
/*****************************************************************************/
//  Description : check play_info file 
//  Global resource dependence :  none
//  Author: Leiming.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_CheckPlayInfoFile(void);
/*****************************************************************************/
//  Description : save play_info file 
//  Global resource dependence :  none
//  Author: Leiming.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_LoadPlayInfoFromFile(MMIKM_PLAY_INFO_T *km_play_info_ptr);
#endif //MMI_KM_NEED_SEARCH_FIRST_TIME

/*****************************************************************************/
//  Description : create play list file 
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_CreateInforFile(void);

/*****************************************************************************/
//  Description : delete km play infor files 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_DeleteFile(void);
/*****************************************************************************/
//  Description : read km files 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_ReadFile(void);
#if 0
/*****************************************************************************/
//  Description : write km playlist file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIKM_WritePlayListFile(MMIKM_PLAY_LIST_INFO_T *play_list_info_ptr);
#endif 
/*****************************************************************************/
//  Description : ��ʼ�첽���ر��ز����б������״δ򿪲������Լ������б�Ϊ
//                �գ��Զ�����ָ��Ŀ¼�е���Ƶ�ļ���
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayList_BeginLoad(
                                      MMI_WIN_ID_T        win_id,  
                                      MMI_MESSAGE_ID_E    msg_id,
                                      KM_PLAYLIST_TYPE_E  locallist_type,
                                      MMIFILE_DEVICE_E dev
                                      );

/*****************************************************************************/
//  Description : �첽���ر��ز����б���ɣ����汾�ز����б�
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIKM_PlayList_EndLoad(KM_PLAYLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ��ȡÿ��playlist item�ļ�����Ϣ
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayList_AllItem_Load(void);

/*****************************************************************************/
//  Description : �ͷ�playlist infor�������Դ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_ReleaseMemoForPlaylistInfo(void);

/*****************************************************************************/
//  Description : playlist infor�������Դ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_AlloclMemoForPlaylistInfo(void);

/*****************************************************************************/
//  Description : ��������playlist infor�������Դ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_MemSetForPlaylistInfo(void);

/*****************************************************************************/
//  Description : �Ƿ����ϴβ�����Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_IsLastPlayInfor(void);

/*****************************************************************************/
//  Description : ��ȡÿ��ӰƬ���͵���Ŀ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIKM_GetMovieTypeNum(MMIKM_TYPE_E type_enum);

/*****************************************************************************/
//  Description : ��ȡ�ϴβ����ļ���Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetLastPlayInfor(MMIKM_PLAY_INFO_T *km_play_info_ptr);

/*****************************************************************************/
//  Description : �����ϴβ����ļ���Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have last play infor; FALSE: no have last play infor 
/*****************************************************************************/
PUBLIC void MMIKM_SetLastPlayInfor(wchar          *file_full_path_name_ptr,    
                                          uint16          file_full_path_len ,
                                          uint32          played_time);

/*****************************************************************************/
//  Description : ��ȡָ��ӰƬ����ָ��index��ӰƬ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: have get movie type infor; FALSE: no movie type infor 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetMovieTpyeInfor( uint16       index,  //in:type num����
                                                 MMIKM_TYPE_E type_enum,
                                                 MMIKM_FILE_ITEM_INFO_T *km_file_item_info_ptr
                                               );
                                        
/*****************************************************************************/
//  Description : create searching file wait win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MMIKM_CreateSearchWaitWin(void);

/*****************************************************************************/
//  Description : ��ȫ·���л�ȡ������׺���ļ���
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: file_name_ptr[d:\com\test\test.exe],�������file_path_ptr[test]
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetFileNameNoSuffix(      
                        const uint16 *src_name_ptr, // [IN]
                        uint16 src_name_len,  // [IN]
                        uint16 *des_path_ptr, // [OUT]
                        uint16 *des_path_len  // [OUT]
                         );

/*****************************************************************************/
//  Description : ��ȫ·���л�ȡ����׺������ַ�
//                
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: //  Note: file_name_ptr[d:\com\test\test.exe],
//            �������file_path_ptr[d:\com\test\test]
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetFileFullPathNoSuffix(      
                        const uint16 *src_name_ptr, // [IN]
                        uint16 src_name_len,  // [IN]
                        uint16 *des_path_ptr, // [OUT]
                        uint16 *des_path_len  // [OUT]
                         );

/*****************************************************************************/
//  Description : ɾ��ָ��ӰƬ����ָ��index��ӰƬ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: TRUE: delete movie type infor success; FALSE: delete fail 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_DeltMovieTpyeInfor(
                                        uint16*       index_ptr,  //in:type num����
                                        uint16        delete_num, //in
                                        MMIKM_TYPE_E type_enum //in 
                                       );

/*****************************************************************************/
//  Description : get kmv file content 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetKmvFileContent(wchar *file_full_path_ptr,
                                                KMV_INFORMATION_T *kmv_infor_ptr
                                                );
//smv_support
/*****************************************************************************/
//  Description : get kmv file content 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_GetSmvFileContent(wchar *file_full_path_ptr,
                                                KMV_INFORMATION_T *kmv_infor_ptr
                                                );

/*****************************************************************************/
//  Description : get play list device information
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIKM_GetPlayListDevice(void);

/*****************************************************************************/
//  Description : �����������е�������Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_ResetAllInfo(void);

/*****************************************************************************/
//  Description : �Ƿ񲥷��б�Ϊ��
//  Global resource dependence :  none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_IsPlayListEmpty(void);

/*****************************************************************************/
//  Description : �ϲ�˫t���ļ�����
//  Global resource dependence :  none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKM_DoubleCardFileArrayCombine(void);

/*****************************************************************************/
//  Description : �첽��ȡ���ز����б���Ƶ�ļ�����Ϣ��
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKM_PlayListGetItem(
                                    KM_PLAYLIST_TYPE_E  list_type, 
                                    MMIKM_LOAD_SAVE_INFO_T* save_info_ptr,
                                    uint32* file_size_ptr);//andy.he_cr122288

/*****************************************************************************/
//  Description :   ͨ���ļ�fullpath����ȡ ͼƬ fullpath ��Ϣ
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN GetThumbnailImageByFileName(wchar * file_info_fullpath,
                                                                                            wchar *file_info_name,
                                                                                            wchar *thumbnail_file_full_name,
                                                                                            uint16 *thumbnail_file_full_name_len);

/*****************************************************************************/
//  Description : ɾ��FileArray
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_FileArrayDestroy(void);

#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT
/*****************************************************************************/
//  Description : �ϲ�Udisk��T���ļ�����
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_UdiskTcardFileArrayCombine(void);
#endif
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif


#endif//_MMIKM_INTERNAL_H_

