/*****************************************************************************
** File Name:                    mivp_locallist.c                                  *
** Author:                                                                   *
** Date:           2010/3/11                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe vp state machine            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/11      apple.zhang      Create
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#define MMIVP_LOCALLIST_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mn_type.h"
#include "mmivp_export.h"
#include "mmivp_locallist.h"
#include "mmifmm_export.h"
#include "mmivp_historylist.h"
#include "mmivp_internal.h"

#ifdef MMI_KING_MOVIE_SUPPORT
#include "mmikm_export.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    VP_LOCALLIST_TYPE_E locallist_type;
    VP_OPLISTCALLBACK callbackfunc;
}VP_LOCALLIST_CALLBACK_INFO_T;

#define MMIVP_VIDEO_SUFFIX_TYPE_NUM  (sizeof(s_vp_all_suffix)/sizeof(VP_SUFFIX_T))

//#define VP_LOCALLIST_MIN_SAPCE 1024   //masked by sandy for cr246623
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL   FILEARRAY                       s_locallist_array_ptr[VP_LOCALLIST_MAX]   = {PNULL,PNULL};          //���ز����б�
LOCAL   VP_LOCALLIST_CALLBACK_INFO_T    s_oplist_info    = {0};           //���ز����б�ִ��ĳЩ�����Ļص���Ϣ
const   VP_SUFFIX_T                     s_vp_all_suffix[] =     //��Ƶ��֧�����
{
    {"3gp",MMICOM_VIDEO_TYPE_3GP,TRUE},
    {"mp4",MMICOM_VIDEO_TYPE_MP4,TRUE},
    {"avi",MMICOM_VIDEO_TYPE_AVI,VP_IS_SUPPORT_AVI},
    {"flv",MMICOM_VIDEO_TYPE_FLV,VP_IS_FLV_SUPPORT},
    {"rm",MMICOM_VIDEO_TYPE_RMVB,VP_IS_RMVB_SUPPORT},
    {"rv",MMICOM_VIDEO_TYPE_RMVB,VP_IS_RMVB_SUPPORT},
    {"rmvb",MMICOM_VIDEO_TYPE_RMVB,VP_IS_RMVB_SUPPORT},
#ifdef DRM_SUPPORT
    {"dm",MMICOM_VIDEO_TYPE_DRM,TRUE},
    {"dcf",MMICOM_VIDEO_TYPE_DCF,TRUE},
#endif
#ifdef MMI_KING_MOVIE_SUPPORT   
    {"kmv",MMICOM_VIDEO_TYPE_KMV,TRUE},
    {"smv",MMICOM_VIDEO_TYPE_SMV,TRUE},
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    {"hmv",MMICOM_VIDEO_TYPE_HMV,TRUE},
#endif
#endif
};

LOCAL   VP_PLAYINGLIST_T   s_playing_list           = {0};                //���ڲ����б�
LOCAL   VP_LOCALLIST_INFO_T     s_current_list_info  = {0}; //��¼��ǰ���������õı��ز����б���Ϣ


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ����ļ������ز����б�Ļص�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToLocalListCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      );

/*****************************************************************************/
//  Description : ��ȡ��ű��ز����б��ļ���·����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetLocalListFileNameForRead(
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr,     
                                   VP_LOCALLIST_TYPE_E  locallist_type
                                   );

/*****************************************************************************/
//  Description : ��ȡ��ű��ز����б��ļ���·����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetLocalListFileNameForWrite(
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr,
                                   VP_LOCALLIST_TYPE_E  locallist_type
                                   );
/*****************************************************************************/
//  Description : ���汾�ز����б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SaveLocallistToFile(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : ��ȡָ����Χ�ڵĲ��ظ����������.
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetDifferentRandom(void);

/*****************************************************************************/
//  Description : ��ȡ��ͬdevice type�� Ĭ�ϴ����Ƶ�ļ����ļ���·��(video folder)
//  Global resource dependence :  none
//  Author: Sandy.Wan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIVP_GetDefaultMoviePath(
                                   wchar            *full_path_name_ptr,
                                   uint16           *full_path_len_ptr,
                                   MMIFILE_DEVICE_E file_dev_type);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ȡɸѡ��Ƶ�ļ���ɸѡ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetFilerInfo(MMIFMM_FILTER_T * filter)
{
    uint32           i           = 0;
    uint16          str_len     = 0; 
    
    if (PNULL == filter)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_GetFilerInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_120_112_2_18_3_5_20_307,(uint8*)"");
        return;
    }
    //������֧�ֵ���Ƶ��ʽ������"*.3gp *.mp4"�Ĵ�
    for (i = 0; i < MMIVP_VIDEO_SUFFIX_TYPE_NUM; i++)
    {
        
        if (s_vp_all_suffix[i].is_support)
        {
            str_len = (uint16)strlen((char*)filter->filter_str);
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                "*.", 2, 2);
            str_len += 2;
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                s_vp_all_suffix[i].suffix, 
                ((uint16)strlen((char*)s_vp_all_suffix[i].suffix)), 
                ((uint16)strlen((char*)s_vp_all_suffix[i].suffix))); /*lint !e666*/
            str_len = (uint16)strlen((char*)filter->filter_str);
            MMI_MEMCPY(
                (filter->filter_str + str_len), 
                (FMM_FILTER_STRING_MAXLEN - str_len),
                " ", 1, 1);
        }
    }
}

/*****************************************************************************/
//  Description : ����ļ������ز����б�Ļص�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToLocalListCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      )
{
    uint32      file_num    =   0;
    VP_OP_LIST_RESULT_E  result = VP_OP_LIST_SUCCESS;
  

    //��ȡ�����Ƶ�ļ�����Ŀ
    file_num = MMIAPIFILEARRAY_GetArraySize(file_array);

    //VP_STRACE_LOW:"[VP] AddFileToLocalListCallBack is_success = %d, file_num = %d, file_array = %x, s_oplist_info.locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_166_112_2_18_3_5_20_308,(uint8*)"dddd",is_success,file_num,file_array,s_oplist_info.locallist_type);
    if( is_success && (0 < file_num) && VP_LOCALLIST_MAX > s_oplist_info.locallist_type)
    {        
        if (PNULL == s_locallist_array_ptr[s_oplist_info.locallist_type])
        {
            s_locallist_array_ptr[s_oplist_info.locallist_type] = MMIAPIFILEARRAY_Create();
        }
        //��ӵ����ز����б���
        //VP_STRACE_LOW:"[VP] AddFileToLocalListCallBack total_num 1 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_174_112_2_18_3_5_20_309,(uint8*)"d",MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr[s_oplist_info.locallist_type]));
        MMIAPIFILEARRAY_Combine(s_locallist_array_ptr[s_oplist_info.locallist_type],file_array); 
        //VP_STRACE_LOW:"[VP] AddFileToLocalListCallBack total_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_176_112_2_18_3_5_20_310,(uint8*)"d",MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr[s_oplist_info.locallist_type]));
        //���汾�ز����б�Ĭ���ļ���
        if(SFS_ERROR_NONE != SaveLocallistToFile(s_oplist_info.locallist_type))
        {
            result = VP_OP_LIST_SAVE_ERROR;
        }       
        
    }       

    //�ͷ���Դ
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);   
        file_array = PNULL;
    }
    
  
    //VP_STRACE_LOW:"[VP] AddFileToLocalListCallBack is_success = %d, s_oplist_info.locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_193_112_2_18_3_5_20_311,(uint8*)"dd",is_success,s_oplist_info.locallist_type);
    //֪ͨui��ӽ��
    if (PNULL != s_oplist_info.callbackfunc)
    {
        s_oplist_info.callbackfunc(result,s_oplist_info.locallist_type);
        s_oplist_info.callbackfunc = PNULL;
    }
    
}

/*****************************************************************************/
//  Description : ��ȡ��ű��ز����б��ļ���·����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetLocalListFileNameForRead(
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr,
                                   VP_LOCALLIST_TYPE_E  locallist_type
                                   )
{
    BOOLEAN         result      =   FALSE;
    
    if (PNULL != full_path_name_ptr && PNULL != full_path_len_ptr)
    {        
    
        if(VP_LOCALLIST_DEFAULT == locallist_type)
        {       
            result = MMIAPIFMM_GetSysFileFullPathForRead(
                FALSE,
                VP_DEFAULT_DIR,
                VP_DEFAULT_DIR_LEN,
                VP_LOCALLIST_FILENAME,
                VP_LOCALLIST_FILENAME_LEN,
                full_path_name_ptr,
                full_path_len_ptr
                );
        }
        else
        {
            result = MMIAPIFMM_GetSysFileFullPathForRead(
                FALSE,
                VP_DEFAULT_DIR,
                VP_DEFAULT_DIR_LEN,
                VP_LOCALLIST1_FILENAME,
                VP_LOCALLIST1_FILENAME_LEN,
                full_path_name_ptr,
                full_path_len_ptr
                );

        }
    }
    
    //VP_STRACE_LOW:"[VP] GetLocalListFileNameForRead locallist_type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_245_112_2_18_3_5_20_312,(uint8*)"dd",locallist_type,result);
    return result; 
    
}

/*****************************************************************************/
//  Description : ��ȡ��ű��ز����б��ļ���·����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetLocalListFileNameForWrite(
                                   wchar          *full_path_name_ptr,    
                                   uint16          *full_path_len_ptr ,
                                   VP_LOCALLIST_TYPE_E  locallist_type
                                   )
{
    BOOLEAN         result      =   FALSE;
    
    //VP_STRACE_LOW:"[VP] GetLocalListFileNameForWrite locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_262_112_2_18_3_5_20_313,(uint8*)"d",locallist_type);

    if (PNULL != full_path_name_ptr && PNULL != full_path_len_ptr)
    { 
    
        if (VP_LOCALLIST_DEFAULT == locallist_type)
        {        
            result = MMIAPIFMM_GetSysFileFullPathForWrite(
                FALSE,   //changed by sandy for cr246623, it is not fixed store
                VP_DEFAULT_DIR,
                VP_DEFAULT_DIR_LEN,
                VP_LOCALLIST_FILENAME,
                VP_LOCALLIST_FILENAME_LEN,
                VP_LOCALLIST_MIN_SAPCE,  //changed by sandy for cr246623, it is not fixed store
                full_path_name_ptr,
                full_path_len_ptr
                );
        }
        else
        {
            result = MMIAPIFMM_GetSysFileFullPathForWrite(
                FALSE,  //changed by sandy for cr246623, it is not fixed store
                VP_DEFAULT_DIR,
                VP_DEFAULT_DIR_LEN,
                VP_LOCALLIST1_FILENAME,
                VP_LOCALLIST1_FILENAME_LEN,
                VP_LOCALLIST_MIN_SAPCE,   //changed by sandy for cr246623, it is not fixed store
                full_path_name_ptr,
                full_path_len_ptr
                );
        }
    }
    
    //VP_STRACE_LOW:"[VP] GetLocalListFileNameForWrite result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_295_112_2_18_3_5_20_314,(uint8*)"d",result);
    return result; 
    
}
/*****************************************************************************/
//  Description : ���汾�ز����б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SaveLocallistToFile(VP_LOCALLIST_TYPE_E  locallist_type)
{
    MMIFILE_ERROR_E             save_result                     =   SFS_NO_ERROR;
    wchar               abs_default_path[VP_MAX_NAME_LEN+1]     =   {0};
    uint16              abs_default_path_length                 =   VP_MAX_NAME_LEN;
    MMIFILE_HANDLE      file_handle                             =   0;    
   
    //VP_STRACE_LOW:"[VP] SaveLocallistToFile locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_310_112_2_18_3_5_20_315,(uint8*)"d",locallist_type);
    //��ȡĬ�ϱ��ز����б��ļ�ȫ·��
    if(GetLocalListFileNameForWrite(abs_default_path,&abs_default_path_length,locallist_type))
    {
        // fmm��д�ļ�ʱ,����洢���ռ��㹻д�����������д���ļ�Ȼ���ٽ���д������
        //������ֿռ䲻�㣬��д���ļ������Ǳ���ԭ�����ļ�����ֱ�ӷ��أ�
        //������д������Ƶ�б���ļ�֮ǰ��app�㲻��ɾ��ԭ�е���Ƶ�б��ļ���
#if 0
        //ɾ����ǩĬ���ļ�
        if(MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {        
            MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        }
#endif
        
        if (VP_LOCALLIST_MAX > locallist_type && PNULL != s_locallist_array_ptr[locallist_type])
        {  
            if (!MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
            {
                file_handle = MMIAPIFMM_CreateFile(abs_default_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
                MMIAPIFMM_CloseFile(file_handle);
            }
            //���汾�ز����б�Ĭ���ļ���
            save_result = MMIAPIFILEARRAY_SaveArrayToFileEx( 
                s_locallist_array_ptr[locallist_type],
                abs_default_path,
                abs_default_path_length );
            //�������ʹ�����ڲ����б����������ɲ����б�
            if (PNULL != s_playing_list.playing_list_ptr)  //�������
            {
                VP_LocalList_ResetCurPlayList(VP_Setting_GetIsRandomPlayMode());
            }
        }
        else
        {
            save_result = SFS_ERROR_NO_ENOUGH_RESOURCE;
        }
        
    }
    else
    {
        save_result = SFS_ERROR_NO_SPACE;
    }
    //VP_STRACE_LOW:"[VP] SaveLocallistToFile save_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_353_112_2_18_3_5_20_316,(uint8*)"d",save_result);
    
    //��������쳣����ɾ��ԭ������Ƶ�б�ṹ��    
    #if 0
    if (VP_LOCALLIST_MAX > locallist_type && SFS_ERROR_NONE != save_result)
    {
        MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr[locallist_type]);
        s_locallist_array_ptr[locallist_type] = PNULL;
        VP_LocalList_DestroyPlayList();
    }
    #else
    //��������쳣������洢���ռ䲻��ʱ��fmm�㲻�ܼ���д������Ƶ�б���ļ������Ǳ���ԭ�����ļ������Ա�֤��ɾ��ԭ�е���Ƶ�ļ��б�ṹ��
    //�������쳣ʱ����ʱ�������ڴ����ļ��е����ݿ��ܲ�һ���ˣ����Ա������ͷŸ��ڴ棬Ȼ�����¿����ڴ��ԭ�е���Ƶ�б��ļ������¼������ݣ�
    if (VP_LOCALLIST_MAX > locallist_type && SFS_ERROR_NONE != save_result)
    {
        uint32 file_size = 0;

        MMIAPIFMM_GetFileInfo(abs_default_path,abs_default_path_length,&file_size,PNULL,PNULL);
        
        //file_size >0 ��ʾ�ø��ļ��б�������������ɹ�����
        if (file_size > 0)  //add by sandy for cr246623
        {
            if (PNULL != s_locallist_array_ptr[locallist_type])
            {
                MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr[locallist_type]);
                s_locallist_array_ptr[locallist_type] = PNULL;
            }

        s_locallist_array_ptr[locallist_type] = MMIAPIFILEARRAY_Create();  

            if(PNULL != s_locallist_array_ptr[locallist_type]) 
            {
                //��ԭ������Ƶ�б��ļ������¼��ر��ز����б���Ϣ
                MMIAPIFILEARRAY_LoadArrayFromFile(
                    s_locallist_array_ptr[locallist_type],
                    abs_default_path,
                    abs_default_path_length
                    );   
            }
        }
        else //add by sandy for cr246623
        {
        //if file_size = 0,  ��ʾ������һ��loading ��Ƶ�б�ʱ���������洢���ռ䲻�㣬����û������Ϊ�ļ���
        //��ʱ���뱣�ָ�����ռ�����ݲ����Ա����ʹ�ã�
        }
		
    }
    else  //add by sandy for crcr246623.
    {
        //�����еĴ洢������list����ɹ��������£�ֻ�ܴ������һ���������list�ļ��������洢���е�ͬ��list�ļ�����ɾ������
        VP_LocalList_DeleteSameNameFile(abs_default_path,abs_default_path_length);
    }
    #endif
    
    return save_result;              
}

/*****************************************************************************/
//  Description : ��ȡĬ�ϴ����Ƶ�ļ����ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_GetDefaultFolderPath(
                                   wchar          *full_path_name_ptr,
                                   uint16          *full_path_len_ptr
                                   )
{
    BOOLEAN         result  =   FALSE;
    MMIFILE_DEVICE_E    file_dev = 0;
    wchar*     dev_path_ptr = PNULL;
    uint16     dev_len = 0;
    const wchar     *dir_ptr = PNULL;
    uint16           dir_len = 0;
	
#ifdef MMI_KING_MOVIE_SUPPORT
	if(MMIAPIKM_IsKMmainWinOpen())
	{
		dir_ptr = KM_DEFAULT_DIR;
		dir_len = KM_DEFAULT_DIR_LEN;
	}
	else
#endif
	{
		dir_ptr = VP_DEFAULT_DIR;
		dir_len = VP_DEFAULT_DIR_LEN;
	}
	
    file_dev = MMIAPIFMM_GetFirstValidDevice();
    if (MMI_DEVICE_NUM !=file_dev)
    {
        dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
        dev_path_ptr =  MMIAPIFMM_GetDevicePath(file_dev);


        //if(MMIAPIFMM_GetDeviceStatus(dev_path_ptr, dev_len))
        {
            result = MMIAPIFMM_CombineFullPath(
                dev_path_ptr, 
                dev_len,
                dir_ptr, 
                dir_len,
                PNULL, 
                PNULL,
                full_path_name_ptr,
                full_path_len_ptr
                );
        }
    }
    
    return result; 
}

/*****************************************************************************/
//  Description : ��ȡ��ͬdevice type�� Ĭ�ϴ����Ƶ�ļ����ļ���·��(video folder)
//  Global resource dependence :  none
//  Author: Sandy.Wan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIVP_GetDefaultMoviePath(
                                   wchar          *full_path_name_ptr,
                                   uint16          *full_path_len_ptr,
                                   MMIFILE_DEVICE_E file_dev_type
                                   )
{
    BOOLEAN     result  =   FALSE;
    
	if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(file_dev_type), MMIAPIFMM_GetDevicePathLen(file_dev_type)))
	{
		result = MMIAPIFMM_CombineFullPath(
			MMIAPIFMM_GetDevicePath(file_dev_type), 
			MMIAPIFMM_GetDevicePathLen(file_dev_type),
			VP_DEFAULT_DIR, 
			VP_DEFAULT_DIR_LEN,
			PNULL, 
			PNULL,
			full_path_name_ptr,
			full_path_len_ptr
			);
	}
    else
	{                  
		//VP_STRACE_LOW:"[VP] MMIVP_GetDefaultMoviePath, file_dev_type = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_477_112_2_18_3_5_21_317,(uint8*)"d",file_dev_type);
		result  =   FALSE;
	}

         
    return result; 
}
/*****************************************************************************/
//  Description : ��ȡָ����Χ�ڵĲ��ظ����������.
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetDifferentRandom(void)
{
    BOOLEAN *random_arr_ptr = PNULL; 
    uint32      i , j   ,count     = 0;
    uint16      random          = 0;    
    VP_LOCALLIST_TYPE_E  locallist_type = VP_LocalList_GetCurListType();
    uint32      total_num       = VP_LocalList_GetNum(locallist_type);
    int32       locallist_index = VP_LocalList_GetCurrentIndex(locallist_type);
    
    //VP_STRACE_LOW:"[VP] GetDifferentRandom locallist_type = %d, locallist_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_497_112_2_18_3_5_21_318,(uint8*)"dd",locallist_type,locallist_index);
    if (VP_LOCALLIST_MAX <= locallist_type || 0 > locallist_index)
    {
        return;
    }

    if (PNULL == s_playing_list.playing_list_ptr)
    {
        s_playing_list.playing_list_ptr = (VP_PLAYINGLIST_ITEM_T*)SCI_ALLOC_APP((sizeof(VP_PLAYINGLIST_ITEM_T)*total_num));
        SCI_MEMSET(s_playing_list.playing_list_ptr,0,(sizeof(VP_PLAYINGLIST_ITEM_T)*total_num));
    }
    
    
    //��������Ҫ�Ľ�������ظ�������Ԥ�ȷ���һ��������random_arr_ptr����Ϊrand������������������ظ������Բ�ֱ��ʹ�ã����Ƕ�Ӧ���ݵ��±�
    random_arr_ptr = (BOOLEAN*)SCI_ALLOC_APP((sizeof(BOOLEAN)*total_num));
    SCI_MEMSET(random_arr_ptr,0,(sizeof(BOOLEAN)*total_num));
    for (i = 0; i < total_num; i++)
    {
        random_arr_ptr[i] = FALSE;
    }
    
    srand(SCI_GetTickCount());
    for (i=0; i<total_num; i++)
    {
        count =0;
        random = (uint16)((uint16)(rand())%(total_num-i));//����0��total-1-i֮�������

        //�ҵ�boolean������û��ûѡ�еĵ�random+1�������±�
        for(j = 0;j<total_num;j++)
        {
            if(FALSE == random_arr_ptr[j])
            {
               count++;
            }
            if(count == random+1)
                break;
        }
        //�������������±���Ϊ��i�������
        s_playing_list.playing_list_ptr[i].index = j;
        //���ñ�־�����±�j�Ѿ���ռ��
        random_arr_ptr[j] = TRUE;

        if(s_playing_list.playing_list_ptr[i].index == locallist_index)
        {
            s_playing_list.active_index = i;
            s_playing_list.start_index = i;
        }
    }
    
    //free memory
    SCI_FREE(random_arr_ptr);
    random_arr_ptr = PNULL;
}


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
                                      MMIFILE_DEVICE_E     file_dev_type
                                      )
{                   
    wchar                   abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                  abs_default_path_length = VP_MAX_NAME_LEN;
    BOOLEAN                 load_result             = FALSE;
    MMIFMM_FILTER_T         filter                  = {0};

    //Ϊ���ز����б����ռ�
    //VP_STRACE_LOW:"[VP] VP_LocalList_BeginLoad locallist_type = %d,file_dev_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_570_112_2_18_3_5_21_319,(uint8*)"dd",locallist_type,file_dev_type);
    if (VP_LOCALLIST_MAX <= locallist_type)
    {
        return FALSE;
    }
    
    if (PNULL == s_locallist_array_ptr[locallist_type])
    {
        s_locallist_array_ptr[locallist_type] = MMIAPIFILEARRAY_Create();  
    }
 
    if(PNULL == s_locallist_array_ptr[locallist_type])
    {
        return load_result;
    }
    
    if (VP_LOCALLIST_DEFAULT == locallist_type)
    {

                    //��ȡ��Ƶ�ļ�������Ϣ
            MMIAPIVP_GetFilerInfo(&filter);

            //��ȡĬ���ļ���
            if(!MMIVP_GetDefaultMoviePath(abs_default_path,&abs_default_path_length,file_dev_type)) 
                {
                //VP_STRACE_LOW:"[VP] MMIVP_GetDefaultMoviePath is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_594_112_2_18_3_5_21_320,(uint8*)"");
                return load_result;
                }

            //Ĭ���ļ���������֧�ֵ���Ƶ�ļ�
            MMIAPIFMM_SearchFileInPath(
                abs_default_path, 
                abs_default_path_length,
                &filter,
                TRUE,
                FUNC_FIND_FILE,
                s_locallist_array_ptr[locallist_type],
                win_id,
                msg_id
                ); 
    }
    else
    {    
        if(GetLocalListFileNameForRead (abs_default_path,&abs_default_path_length,locallist_type)) 
        {        
            //��Ĭ���ļ��м��ر��ز����б���Ϣ
            load_result = MMIAPIFILEARRAY_LoadArrayFromFile(
                s_locallist_array_ptr[locallist_type],
                abs_default_path,
                abs_default_path_length
                );       
        
        }
        MMK_PostMsg(win_id,msg_id,PNULL,PNULL);    

    }
    //VP_STRACE_LOW:"[VP] VP_LocalList_BeginLoad load_result = %d, locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_625_112_2_18_3_5_21_321,(uint8*)"dd",load_result,locallist_type);
    return TRUE;    
}


/*****************************************************************************/
//  Description : �첽���ر��ز����б���ɣ����汾�ز����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndLoad(VP_LOCALLIST_TYPE_E  locallist_type)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_EndLoad locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_636_112_2_18_3_5_21_322,(uint8*)"d",locallist_type);

    //�ȼ���Ƿ��к��ʵĴ洢���Ҵ洢���ռ��㹻��add by sandy for cr246623.
    if(SFS_ERROR_NO_SPACE == VP_LocalList_GetSuitableDeviceState()) 
    {
        return SFS_ERROR_NO_SPACE;
    }
    return SaveLocallistToFile(locallist_type);
}

/*****************************************************************************/
//  Description : �ͷű��ز����б���Դ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_Release(VP_LOCALLIST_TYPE_E  locallist_type)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_Release locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_652_112_2_18_3_5_21_323,(uint8*)"d",locallist_type);
    if(VP_LOCALLIST_MAX > locallist_type && PNULL != s_locallist_array_ptr[locallist_type])
    {
        MMIAPIFMM_SearchFileStop();
        MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr[locallist_type]);
        s_locallist_array_ptr[locallist_type] = PNULL;
    }
}

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
                                    )
{
    BOOLEAN    result = FALSE;

    //VP_STRACE_LOW:"[VP] VP_LocalList_GetInfo locallist_type = %d, locallist_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_672_112_2_18_3_5_21_324,(uint8*)"dd",locallist_type,locallist_index);
    if (PNULL != f_array_data_ptr && VP_LOCALLIST_MAX > locallist_type && PNULL != s_locallist_array_ptr[locallist_type] && locallist_index >= 0)   
    {
        result = MMIAPIFILEARRAY_Read(s_locallist_array_ptr[locallist_type], locallist_index, f_array_data_ptr);
    }
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetInfo locallist_type = %d, result = %d, locallist_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_677_112_2_18_3_5_21_325,(uint8*)"ddd",locallist_type, result,locallist_index);
    return result;
}

/*****************************************************************************/
//  Description : ��ȡ���ز����б���б������Ŀ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_LocalList_GetNum( VP_LOCALLIST_TYPE_E  locallist_type )
{
    uint32  locallist_num = 0;
    
    
    
    if(VP_LOCALLIST_MAX > locallist_type &&  PNULL != s_locallist_array_ptr[locallist_type])
    {
        locallist_num = MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr[locallist_type]);
    }
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetNum locallist_type = %d, locallist_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_695_112_2_18_3_5_21_326,(uint8*)"dd",locallist_type,locallist_num);
    
    return locallist_num;
}
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
/*****************************************************************************/
//  Description : �Ƿ���²����б� 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_IsNeedReSearchLocallist( void )
{    

    wchar                   abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                  abs_default_path_length = VP_MAX_NAME_LEN;
    BOOLEAN                 load_result             = FALSE;
    BOOLEAN                 need_load            = FALSE;
    
    if (PNULL == s_locallist_array_ptr[VP_LOCALLIST_DEFAULT])
    {
        s_locallist_array_ptr[VP_LOCALLIST_DEFAULT] = MMIAPIFILEARRAY_Create();  
    }
 
    if(PNULL == s_locallist_array_ptr[VP_LOCALLIST_DEFAULT])
    {
        return need_load;
    }


        if(GetLocalListFileNameForRead (abs_default_path,&abs_default_path_length,VP_LOCALLIST_DEFAULT)) 
        {        
           
            if(MMIFILE_IsFileExist(abs_default_path, abs_default_path_length))
            {
                VP_STRACE_LOW("[VP] VP_LocalList_BeginLoad locallist MMIAPIFILEARRAY_LoadArrayFromFile VP_LOCALLIST_DEFAULT");
                            //��Ĭ���ļ��м��ر��ز����б���Ϣ
                load_result = MMIAPIFILEARRAY_LoadArrayFromFile(
                s_locallist_array_ptr[VP_LOCALLIST_DEFAULT],
                abs_default_path,
                abs_default_path_length
                );   
				if(!load_result)
	            {        
	                SCI_TRACE_LOW("[VP] VP_LocalList_IsNeedReSearchLocallist,load_result =%d",load_result);
									need_load = TRUE;            
									MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
									MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr[VP_LOCALLIST_DEFAULT]);
									s_locallist_array_ptr[VP_LOCALLIST_DEFAULT] = PNULL;

	            }   
				else if(MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr[VP_LOCALLIST_DEFAULT]) == 0)
	            {
	                need_load = TRUE;
	            }
            }
            else
            {
                need_load = TRUE;
            }
        
        }

     return need_load;       

}
/*****************************************************************************/
//  Description : �����û��Զ��岥���б� 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  VP_LocalList_LoadUserLocalList(void)
{
    wchar                   abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                  abs_default_path_length = VP_MAX_NAME_LEN;
    BOOLEAN                 load_result             = FALSE;
    if (PNULL == s_locallist_array_ptr[VP_LOCALLIST_USER])
    {
        s_locallist_array_ptr[VP_LOCALLIST_USER] = MMIAPIFILEARRAY_Create();  
    }
 
    if(PNULL == s_locallist_array_ptr[VP_LOCALLIST_USER])
    {
        return load_result;
    }
    
    if(GetLocalListFileNameForRead (abs_default_path,&abs_default_path_length,VP_LOCALLIST_USER)) 
    {        
        //��Ĭ���ļ��м��ر��ز����б���Ϣ
        load_result = MMIAPIFILEARRAY_LoadArrayFromFile(
            s_locallist_array_ptr[VP_LOCALLIST_USER],
            abs_default_path,
            abs_default_path_length
            );       
    
    }
    return load_result;
}
#endif

/*****************************************************************************/
//  Description : ���ļ��������������Ƶ�ļ��������б���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void  VP_LocalList_AddItem(VP_OPLISTCALLBACK callback,
                                 VP_LOCALLIST_TYPE_E  locallist_type)
{
    FILEARRAY               ret_array   = NULL;
    MMIFMM_FILTER_T         filter      = {0};
    wchar                   abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                  abs_default_path_length = VP_MAX_NAME_LEN;

    //VP_STRACE_LOW:"[VP] VP_LocalList_AddItem locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_712_112_2_18_3_5_21_327,(uint8*)"d",locallist_type);

    if (VP_LOCALLIST_MAX <= locallist_type)
    {
        if (PNULL != callback)
        {
            callback(VP_OP_LIST_ERROR,locallist_type);            
        } 
        return;
    }

    //�ȼ���Ƿ��к��ʵĴ洢���Ҵ洢���ռ��㹻��changed  by sandy for cr246623.
    if(SFS_ERROR_NO_SPACE == VP_LocalList_GetSuitableDeviceState())  
    {
        if (PNULL != callback)
        {
            callback(VP_OP_LIST_NO_SPACE,locallist_type);            
        }    
        return;
    }
    ret_array = MMIAPIFILEARRAY_Create();
    
    //������ӽ�����֪ͨui��callback����
    s_oplist_info.callbackfunc = callback;
    s_oplist_info.locallist_type = locallist_type;

    //�����Ƶ�ļ�������Ϣ
    MMIAPIVP_GetFilerInfo(&filter);

    //����Դ������
    MMIAPIVP_GetDefaultFolderPath (abs_default_path,&abs_default_path_length);
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(
        abs_default_path,
        abs_default_path_length,
        &filter,
        FUNC_FIND_FILE_AND_ALLFOLER,
        TRUE,
        PNULL,
        ret_array,
        AddFileToLocalListCallBack,
        FALSE
        ))
    {
        if (PNULL != ret_array)
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);   
            ret_array = PNULL;
        }
        if (PNULL != callback)
        {
            callback(VP_OP_LIST_ERROR,locallist_type);            
        }         
    }
}

/*****************************************************************************/
//  Description : ���µ�ǰ���ز����б�����u�̺�sd��������֧�ֵ���Ƶ�ļ�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_BeginUpdate(                                     
                                     MMI_WIN_ID_T        win_id,  
                                     MMI_MESSAGE_ID_E    msg_id,
                                     VP_LOCALLIST_TYPE_E  locallist_type)
{
    MMIFMM_FILTER_T         filter = {0};
    BOOLEAN                 result = FALSE;
    //VP_STRACE_LOW:"[VP] VP_LocalList_BeginUpdate locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_778_112_2_18_3_5_21_328,(uint8*)"d",locallist_type);
    //�ͷ����еı��ز����б�
    if (VP_LOCALLIST_MAX <= locallist_type)
    {
        return FALSE;
    }
    
    if(PNULL != s_locallist_array_ptr[locallist_type])
    {
        MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr[locallist_type]);
        s_locallist_array_ptr[locallist_type] = PNULL;
    }

    //Ϊ���ز����б�����ռ�
    s_locallist_array_ptr[locallist_type] = MMIAPIFILEARRAY_Create();
    if (PNULL != s_locallist_array_ptr[locallist_type])
    {    
        //��ȡ��Ƶ�ļ��Ĺ�����Ϣ
        MMIAPIVP_GetFilerInfo(&filter);

        //����ȫ�����Ϲ�����������Ƶ�ļ�
        MMIAPIFMM_SearchFileInPath(
            PNULL, 
            0,
            &filter,
            TRUE,
            FUNC_FIND_FILE,
            s_locallist_array_ptr[locallist_type],
            win_id,
            msg_id
            );
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : �첽���±��ز����б���ɣ����汾�ز����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndUpdate(VP_LOCALLIST_TYPE_E  locallist_type)
{    
    //VP_STRACE_LOW:"[VP] VP_LocalList_EndUpdate locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_821_112_2_18_3_5_21_329,(uint8*)"d",locallist_type);
    return SaveLocallistToFile(locallist_type);
}

/*****************************************************************************/
//  Description : ɾ�����ز����б��е�ĳһ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_DelItemWithouSaveFile( 
                                                  int32        index,
                                                  VP_LOCALLIST_TYPE_E  locallist_type
                                                  )
{
    BOOLEAN    result       =   FALSE;
    //VP_STRACE_LOW:"[VP] VP_LocalList_DelItemWithouSaveFile locallist_type= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_835_112_2_18_3_5_21_330,(uint8*)"d",locallist_type);
    if (VP_LOCALLIST_MAX > locallist_type && PNULL != s_locallist_array_ptr[locallist_type])
    {
        result = MMIAPIFILEARRAY_Delete(s_locallist_array_ptr[locallist_type], index);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : ���汾�ز����б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_Locallist_SaveToFile(VP_LOCALLIST_TYPE_E  locallist_type)
{
    //VP_STRACE_LOW:"[VP] VP_Locallist_SaveToFile locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_850_112_2_18_3_5_22_331,(uint8*)"d",locallist_type);
    return SaveLocallistToFile(locallist_type);
}

/*****************************************************************************/
//  Description : ��ȡ���ز����б��е���һ����Ƶ��Ϣ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetNext(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr)
{    
    VP_LOCALLIST_TYPE_E locallist_type           = VP_LocalList_GetCurListType();
    int32               cur_locallist_index       = VP_LocalList_GetCurrentIndex(locallist_type);
    FILEARRAY_DATA_T    array_data      = {0};
    BOOLEAN             result          = FALSE;
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetNext cur_index = %d, locallist_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_866_112_2_18_3_5_22_332,(uint8*)"dd",cur_locallist_index,locallist_type);
    if (VP_LOCALLIST_MAX <= locallist_type || cur_locallist_index < 0)
    {
        return FALSE;
    }
    
    if(PNULL != full_path_len_ptr && PNULL != full_path_ptr && VP_LocalList_GetNum(locallist_type) > 0) 
    { 

        //��ȡ���ز����б�����һ����Ƶ�ļ�������
        if((uint32)cur_locallist_index >= (VP_LocalList_GetNum(locallist_type)-1))
        { 
            cur_locallist_index = 0;
        }
        else
        {
            cur_locallist_index++;
        }

        //��ȡ�ļ���Ϣ
        if(VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type))
        {            

            MMI_WSTRNCPY(
                full_path_ptr,
                *full_path_len_ptr,
                array_data.filename,
                array_data.name_len,
                array_data.name_len);
            *full_path_len_ptr = array_data.name_len;

            //���õ�ǰ���ŵ��ļ��ı��ز����б�����
            VP_LocalList_SetCurrentIndex(cur_locallist_index);
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : ��ȡ���ز����б����һ����Ƶ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetPrev(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr
                                    )
{    
    FILEARRAY_DATA_T    array_data      = {0};
    BOOLEAN             result          = FALSE;    
    VP_LOCALLIST_TYPE_E  locallist_type = VP_LocalList_GetCurListType();
    int32               cur_locallist_index       = VP_LocalList_GetCurrentIndex(locallist_type);
    uint32              total_num       = VP_LocalList_GetNum(locallist_type);
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetPrev cur_index = %d, total_num = %d, locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_919_112_2_18_3_5_22_333,(uint8*)"ddd",cur_locallist_index, total_num,locallist_type);

    if (VP_LOCALLIST_MAX <= locallist_type || 0 == total_num || 0 > cur_locallist_index)
    {
        return FALSE;
    }

    if(PNULL != full_path_ptr && PNULL != full_path_len_ptr) 
    { 
        //��ȡ���ز����б�����һ����Ƶ�ļ�������
        if(cur_locallist_index == 0)
        { 
            cur_locallist_index = total_num-1;
        }
        else
        {
            cur_locallist_index--;
        }

        //��ȡ�ļ���Ϣ
        if(VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type))
        {        
            MMI_WSTRNCPY(
                full_path_ptr,
                *full_path_len_ptr,
                array_data.filename,
                array_data.name_len,
                array_data.name_len);
            *full_path_len_ptr = array_data.name_len;        

            //���õ�ǰ���ŵ��ļ��ı��ز����б�����
            VP_LocalList_SetCurrentIndex(cur_locallist_index);
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : �������ڲ����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_CreateCurPlayList(BOOLEAN   is_random_play)
{
    if (PNULL == s_playing_list.playing_list_ptr)
    {
        VP_LocalList_ResetCurPlayList(is_random_play);
    }
}

/*****************************************************************************/
//  Description : �����������ڲ����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_ResetCurPlayList(BOOLEAN   is_random_play)
{   
    uint32      i           = 0;
    VP_LOCALLIST_TYPE_E  locallist_type = VP_LocalList_GetCurListType();
    uint32      total_num   = VP_LocalList_GetNum(locallist_type);
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_ResetCurPlayList is_random_play = %d, total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_979_112_2_18_3_5_22_334,(uint8*)"dd",is_random_play,total_num);
    
    
    //�жϱ��ز����б��Ƿ�Ϊ��
    if (0 < VP_LocalList_GetNum(locallist_type))
    {
        //�ͷ����ڲ����б�
        if (PNULL != s_playing_list.playing_list_ptr)
        {
            SCI_FREE(s_playing_list.playing_list_ptr);
            s_playing_list.playing_list_ptr = PNULL;
        }
    
        //Ϊ���ڲ����б�����ռ�
        s_playing_list.playing_list_ptr = (VP_PLAYINGLIST_ITEM_T* )SCI_ALLOC_APP((sizeof(VP_PLAYINGLIST_ITEM_T)*total_num));
        SCI_MEMSET(s_playing_list.playing_list_ptr,0,(sizeof(VP_PLAYINGLIST_ITEM_T)*total_num));

        if(total_num <= VP_LocalList_GetCurrentIndex(locallist_type))
        {
            VP_LocalList_SetCurrentIndex(0);
        }
        
        if (is_random_play)
        {
            //������������б�
            GetDifferentRandom();
        }
        else
        {

            //����˳�򲥷��б�
            for (i = 0; i < total_num; i++)
            {
                s_playing_list.playing_list_ptr[i].index = i;
                s_playing_list.playing_list_ptr[i].is_ok = FALSE;

                //���õ�ǰ���ڲ����ļ������ڲ����б��е�����
                if ((int32)i == VP_LocalList_GetCurrentIndex(locallist_type))
                {
                    s_playing_list.active_index = i;
                    s_playing_list.start_index = i;
                }
            }
            
            
        }
    }
}

/*****************************************************************************/
//  Description : ���õ�ǰ���ŵ��ļ��Ƿ�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetIsCurPlayListItemOK(BOOLEAN   is_ok)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_SetIsCurPlayListItemOK is_ok = %d , s_playing_list.active_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1029_112_2_18_3_5_22_335,(uint8*)"dd",is_ok,s_playing_list.active_index);

    if (PNULL != s_playing_list.playing_list_ptr)
    {
        s_playing_list.playing_list_ptr[s_playing_list.active_index].is_ok = is_ok;
    }
    
}

/*****************************************************************************/
//  Description : ���ٲ����б�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_DestroyPlayList(void)
{
    if (PNULL != (s_playing_list.playing_list_ptr))
    {
        SCI_FREE(s_playing_list.playing_list_ptr);
        s_playing_list.playing_list_ptr = PNULL;
    }
    s_playing_list.active_index = 0;
    s_playing_list.playing_list_ptr = PNULL;
    s_playing_list.start_index = 0;   
}
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
                                           )
{
    
    BOOLEAN             result          = FALSE;
    int32               next_index      = 0;    
    uint32              i               = 0;
    FILEARRAY_DATA_T    array_data      = {0};
    VP_LOCALLIST_TYPE_E  locallist_type = VP_LocalList_GetCurListType();
    uint32              total_num       = 0;
    BOOLEAN              is_new_create_play_list = FALSE;
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1072_112_2_18_3_5_22_336,(uint8*)"dddd",mode,is_random_play,total_num,locallist_type);
   
    if (VP_LOCALLIST_MAX <= locallist_type)
    {
        return FALSE;
    }

    total_num = VP_LocalList_GetNum(locallist_type);

    if (PNULL == full_path_len_ptr || PNULL == full_path_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1083_112_2_18_3_5_22_337,(uint8*)"");
        return FALSE;
        
    }
    
    //����ģʽ�����仯����Ҫ�����������ٲ����б�
    if (is_random_play
        || VP_REPEAT_PLAY_MODE_ALL == mode
        || (!is_random_play && VP_REPEAT_PLAY_MODE_CLOSE == mode)
        || VP_REPEAT_PLAY_MODE_ONE == mode) //lint !e774
    {    
        if (s_playing_list.playing_list_ptr == PNULL)
        {
            VP_LocalList_CreateCurPlayList(is_random_play);
			if (PNULL == s_playing_list.playing_list_ptr)
			{
				return FALSE;
			}
            is_new_create_play_list = TRUE;
        }
    }
    else
    {
        VP_LocalList_DestroyPlayList();  
		return FALSE;
    }

    //���ݲ���ģʽ��ȡ��һ����Ҫ���ŵ��ļ�
    switch(mode) 
    {
    case VP_REPEAT_PLAY_MODE_CLOSE:
    case VP_REPEAT_PLAY_MODE_ONE: 
      //  if (is_random_play)
        {
            if (0 == s_playing_list.active_index)
            {
                s_playing_list.active_index = total_num-1;
            }
            else
            {
                s_playing_list.active_index --;               
            }
            
            //�ж��Ƿ��Ѿ�ѭ����һ��
            if (s_playing_list.start_index == s_playing_list.active_index)
            {
                //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode one circle"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1123_112_2_18_3_5_22_338,(uint8*)"");
                result = FALSE;
                break;
            }
            
            //��ȡ��һ�����ŵ��ļ��ڱ��ز����б��е�����
            next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
            result = TRUE;            
        }        
        break;
        
    case VP_REPEAT_PLAY_MODE_ALL: //ȫ��ѭ��

        //��ȡ��һ�����ŵ����ڲ����б������
            if (0 == s_playing_list.active_index)
            {
                s_playing_list.active_index = total_num-1;
            }
            else
            {
                s_playing_list.active_index --;               
            }
        
        ////�ж��Ƿ��Ѿ�ѭ����һ��
        if (!is_new_create_play_list &&
            s_playing_list.start_index == s_playing_list.active_index)
        {
            //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode one circle"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1150_112_2_18_3_5_22_339,(uint8*)"");
            //�ж��Ƿ������ļ���������������
            for (i = 0; i < total_num; i++)
            {
                //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode s_playing_list.playing_list_ptr[i].is_ok = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1154_112_2_18_3_5_22_340,(uint8*)"d",s_playing_list.playing_list_ptr[i].is_ok);
                if (s_playing_list.playing_list_ptr[i].is_ok)
                {
                    //������һ��ѭ����ʼ�ı��ز����б�����
                    VP_LocalList_SetCurrentIndex(s_playing_list.playing_list_ptr[s_playing_list.active_index].index);
                    //�����������ڲ����б�
                    VP_LocalList_ResetCurPlayList(is_random_play);                    
                    
                    next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
                    result = TRUE;
                    break;
                }
            }           
            
            break;                
        }
        //��ȡ��һ�����ŵ��ļ��ڱ��ز����б��е�����
        next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
        result = TRUE;         
        break;
     /* 
    case VP_REPEAT_PLAY_MODE_ONE: //���ײ���
        //��õ�ǰ���ڲ����ļ��ڱ��ز����б��е�����
        next_index = VP_LocalList_GetCurrentIndex(locallist_type);
        result = TRUE;    
        break;
	*/
        
    default:

        break;
    }
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode result = %d,next_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1187_112_2_18_3_5_22_341,(uint8*)"dd",result,next_index);
    if (result)
    {
        
        VP_LocalList_SetCurrentIndex(next_index);        
        //����ļ���Ϣ
        if(!VP_LocalList_GetInfo(next_index,&array_data,locallist_type))
        {  
            return FALSE;            
        }
        MMI_WSTRNCPY(
            full_path_ptr,
            *full_path_len_ptr,
            array_data.filename,
            array_data.name_len,
            array_data.name_len);
        *full_path_len_ptr = array_data.name_len;
        
    }
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetPreByMode result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1206_112_2_18_3_5_22_342,(uint8*)"d",result);
    return result;
}


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
                                          )
{
    
    BOOLEAN             result          = FALSE;
    int32               next_index      = 0;    
    uint32              i               = 0;
    FILEARRAY_DATA_T    array_data      = {0};
    VP_LOCALLIST_TYPE_E  locallist_type = VP_LocalList_GetCurListType();
    uint32              total_num       = 0;
    BOOLEAN              is_new_create_play_list = FALSE;
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1230_112_2_18_3_5_22_343,(uint8*)"dddd",mode,is_random_play,total_num,locallist_type);
   
    if (VP_LOCALLIST_MAX <= locallist_type)
    {
        return FALSE;
    }

    total_num = VP_LocalList_GetNum(locallist_type);

    if (PNULL == full_path_len_ptr || PNULL == full_path_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1241_112_2_18_3_5_22_344,(uint8*)"");
        return FALSE;
        
    }
    
    //����ģʽ�����仯����Ҫ�����������ٲ����б�
    if (is_random_play
        || VP_REPEAT_PLAY_MODE_ALL == mode
        || (!is_random_play && VP_REPEAT_PLAY_MODE_CLOSE == mode)
        || VP_REPEAT_PLAY_MODE_ONE == mode) //lint !e774
    {            
        if (s_playing_list.playing_list_ptr == PNULL)
        {
            VP_LocalList_CreateCurPlayList(is_random_play);
			if (PNULL == s_playing_list.playing_list_ptr)
			{
				return FALSE;
			}
            is_new_create_play_list = TRUE;
        }
    }
    else
    {
        VP_LocalList_DestroyPlayList();  
	    return FALSE;
    }

    //���ݲ���ģʽ��ȡ��һ����Ҫ���ŵ��ļ�
    switch(mode) 
    {
    case VP_REPEAT_PLAY_MODE_CLOSE:
    case VP_REPEAT_PLAY_MODE_ONE:
      //  if (is_random_play)
        {
            //�������
            if (s_playing_list.active_index < (int32)(total_num-1))
            {
                s_playing_list.active_index++;
            }
            else
            {
                s_playing_list.active_index = 0;               
            }
            
            //�ж��Ƿ��Ѿ�ѭ����һ��
            if (s_playing_list.start_index == s_playing_list.active_index)
            {
                //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode one circle"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1282_112_2_18_3_5_22_345,(uint8*)"");
                result = FALSE;
                break;
            }
            
            //��ȡ��һ�����ŵ��ļ��ڱ��ز����б��е�����
            next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
            result = TRUE;            
        }        
        break;
        
    case VP_REPEAT_PLAY_MODE_ALL: //ȫ��ѭ��

        //��ȡ��һ�����ŵ����ڲ����б������
        if (s_playing_list.active_index < (int32)(total_num-1))
        {
            s_playing_list.active_index++;
        }
        else
        {
            s_playing_list.active_index = 0;
        }
        
        ////�ж��Ƿ��Ѿ�ѭ����һ��
        if (!is_new_create_play_list &&
            s_playing_list.start_index == s_playing_list.active_index)
        {
            //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode one circle"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1309_112_2_18_3_5_22_346,(uint8*)"");
            //�ж��Ƿ������ļ���������������
            for (i = 0; i < total_num; i++)
            {
                //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode s_playing_list.playing_list_ptr[i].is_ok = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1313_112_2_18_3_5_22_347,(uint8*)"d",s_playing_list.playing_list_ptr[i].is_ok);
                if (s_playing_list.playing_list_ptr[i].is_ok)
                {
                    //������һ��ѭ����ʼ�ı��ز����б�����
                    VP_LocalList_SetCurrentIndex(s_playing_list.playing_list_ptr[s_playing_list.active_index].index);
                    //�����������ڲ����б�
                    VP_LocalList_ResetCurPlayList(is_random_play);                    
                    
                    next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
                    result = TRUE;
                    break;
                }
            }           
            
            break;                
        }
        //��ȡ��һ�����ŵ��ļ��ڱ��ز����б��е�����
        next_index = s_playing_list.playing_list_ptr[s_playing_list.active_index].index;
        result = TRUE;         
        break;
    /*    
    case VP_REPEAT_PLAY_MODE_ONE: //���ײ���
        //��õ�ǰ���ڲ����ļ��ڱ��ز����б��е�����
        next_index = VP_LocalList_GetCurrentIndex(locallist_type);
        result = TRUE;    
        break;
	*/
        
    default:

        break;
    }
    
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode result = %d,next_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1346_112_2_18_3_5_23_348,(uint8*)"dd",result,next_index);
    if (result)
    {
        
        VP_LocalList_SetCurrentIndex(next_index);        
        //����ļ���Ϣ
        if(!VP_LocalList_GetInfo(next_index,&array_data,locallist_type))
        {  
            return FALSE;            
        }
        MMI_WSTRNCPY(
            full_path_ptr,
            *full_path_len_ptr,
            array_data.filename,
            array_data.name_len,
            array_data.name_len);
        *full_path_len_ptr = array_data.name_len;
        
    }
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetNextByMode result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1365_112_2_18_3_5_23_349,(uint8*)"d",result);
    return result;
}


/*****************************************************************************/
//  Description : ��ȡĳһ�����ز����б�ǰ�����ļ���������
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC int32 VP_LocalList_GetCurrentIndex(VP_LOCALLIST_TYPE_E locallist_type)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_GetCurrentIndex locallist_type = %d, s_current_list_info.locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1377_112_2_18_3_5_23_350,(uint8*)"dd",locallist_type,s_current_list_info.locallist_type);
    if (locallist_type == s_current_list_info.locallist_type)
    {
        return s_current_list_info.locallist_index;
    }
    return -1;
}

/*****************************************************************************/
//  Description : ���õ�ǰ���ŵ���Ƶ�ļ�λ�ڱ��ز����б��е�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurrentIndex(int32 index)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_SetCurrentIndex index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1391_112_2_18_3_5_23_351,(uint8*)"d",index);
   s_current_list_info.locallist_index = index; 

}

/*****************************************************************************/
//  Description : ��ȡ��ǰ���ز����б����͡�
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LOCALLIST_TYPE_E VP_LocalList_GetCurListType(void)
{
    return s_current_list_info.locallist_type;
}

/*****************************************************************************/
//  Description : ���õ�ǰ���ز����б��е�����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurListType(VP_LOCALLIST_TYPE_E type)
{
    //VP_STRACE_LOW:"[VP] VP_LocalList_SetCurListType type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_LOCALLIST_1411_112_2_18_3_5_23_352,(uint8*)"d",type);
   s_current_list_info.locallist_type = type;
}


/*****************************************************************************/
//  Description : ����Ƿ��к��ʵĴ洢���Ҵ洢���ռ��㹻��
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note:  add for cr246623
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_GetSuitableDeviceState(void)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    MMIFILE_ERROR_E result = SFS_NO_ERROR;

    //�ȼ���Ƿ��к��ʵĴ洢���Ҵ洢���ռ��㹻��
    result = MMIAPIFMM_GetSuitableFileDev(MMI_DEVICE_SDCARD, VP_LOCALLIST_MIN_SAPCE, &dev);
    if (SFS_NO_ERROR != result)
    {
        if(SFS_ERROR_NO_SPACE == result)
        {
            return SFS_ERROR_NO_SPACE;
        }
        else 
        {
            return SFS_ERROR_DEVICE;
        }
    }  

    return result;
}


/*****************************************************************************/
//  Description : ɾ�������̷�����������ļ���ͬ��ͬ·���ļ���
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note:  add for cr246623
/*****************************************************************************/
PUBLIC void VP_LocalList_DeleteSameNameFile( uint16     *file_name, int16     file_name_len)
{
    wchar               delete_name[VP_MAX_NAME_LEN+1]     =   {0};
    uint16              delete_name_length                 =   VP_MAX_NAME_LEN;

    MMIFILE_DEVICE_E  dev_type = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E  keep_dev_type = MMI_DEVICE_NUM; //�����ļ����������device type��
         
    if(PNULL == file_name
        || 0 == file_name_len)
    {
        return ;
    }

    MMI_WSTRNCPY(delete_name, delete_name_length, file_name, VP_MAX_NAME_LEN, file_name_len);

    keep_dev_type = MMIAPIFMM_GetDeviceTypeByPath(&file_name[0], MMIFILE_DEVICE_LEN);

    //���������ļ�����U�̣���ɾ�������洢������ͬ·���µ�ͬ���ļ���
    for(dev_type = MMI_DEVICE_UDISK; dev_type < MMI_DEVICE_NUM; dev_type++)
    {
        if(!MMIAPIFMM_GetDeviceTypeStatus(dev_type) || (dev_type == keep_dev_type))
        {
            continue;
        }

        switch (dev_type)
        {
            case MMI_DEVICE_UDISK:
            {
                delete_name[0] = VP_LOCALLIST_UDISK_UNICODE;  
            }
            break;
            
            case MMI_DEVICE_SDCARD:
            {
                delete_name[0] = VP_LOCALLIST_SDCARD_UNICODE;  
            }
            break;
            
            case MMI_DEVICE_SDCARD_1:
            {
                delete_name[0] = VP_LOCALLIST_SDCARD1_UNICODE;  
            }
            break;

            default:
            {
                delete_name[0] = VP_LOCALLIST_MAX_DEV_UNICODE;  
            }
            break;
        }

        if(MMIAPIFMM_IsFileExist(delete_name,file_name_len))
        {        
            MMIAPIFMM_DeleteFile(delete_name,PNULL);
        } 
    }       
}
#endif

