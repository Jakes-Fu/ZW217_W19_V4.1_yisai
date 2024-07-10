/*****************************************************************************
** File Name:                    mmivp_historylist.c                                  *
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

#define MMIVP_HISTORYLIST_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
//#include "sci_types.h"
#include "mn_type.h"
#include "mmivp_historylist.h"
#include "mmivp_export.h"
#include "mmifmm_export.h"
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL   VP_HISTORY_CONTEXT_T    *s_visit_his_que_ptr    = PNULL;  //��ʷ��¼�б�


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ȡ�����ʷ��¼�б��ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForRead(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     );
/*****************************************************************************/
//  Description : ��ȡ�����ʷ��¼�б��ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForWrite(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     );
/*****************************************************************************/
//  Description : ��ô����ʷ��¼�б���Ҫ�������ռ��С
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetHistoryListSpaceSize(void);

/*****************************************************************************/
//  Description : ������ʷ��¼����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteItemFromHistorylist(
                                        VP_HISTORY_ITEM_T  history_list_item);

/*****************************************************************************/
//  Description : �ͷ���ʷ��¼��Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeHistoryListItem(uint32 index);

/*****************************************************************************/
//  Description : ������ʷ��¼�б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_LIST_ERROR_E SaveHistorylistToFile(void);
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ȡ�����ʷ��¼�б��ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForRead(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     )
{
    BOOLEAN         result      =   FALSE;    

    if (PNULL != full_path_name_ptr && PNULL != full_path_len_ptr)
    {
        result = MMIAPIFMM_GetSysFileFullPathForRead(
            TRUE,
            VP_DEFAULT_DIR,
            VP_DEFAULT_DIR_LEN,
            VP_HISTORYLIST_FILENAME,
            VP_HISTORYLIST_FILENAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }
    
    return result; 
}

/*****************************************************************************/
//  Description : ��ȡ�����ʷ��¼�б��ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForWrite(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     )
{
    BOOLEAN         result          = FALSE;    
    uint32          min_free_size   = 0;

    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)
    {       
        min_free_size = GetHistoryListSpaceSize();
        //VP_STRACE_LOW:"[VP] GetHistoryListFileNameForWrite min_free_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_114_112_2_18_3_5_18_283,(uint8*)"d",min_free_size);
    
        result = MMIAPIFMM_GetSysFileFullPathForWrite(
            TRUE,
            VP_DEFAULT_DIR,
            VP_DEFAULT_DIR_LEN,
            VP_HISTORYLIST_FILENAME,
            VP_HISTORYLIST_FILENAME_LEN,
            min_free_size,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }
    
    return result; 
}
/*****************************************************************************/
//  Description : ��ô����ʷ��¼�б���Ҫ�������ռ��С
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: ����s_visit_his_que_ptr
/*****************************************************************************/
LOCAL uint32 GetHistoryListSpaceSize(void)
{    
    uint32                      load_size                   = 0;
    VP_HISTORY_ITEM_T           *current_history_item_ptr   = PNULL;
    
    if (PNULL != s_visit_his_que_ptr)
    { 
        load_size = sizeof(VP_HISTORY_CONTEXT_T);    
        current_history_item_ptr = s_visit_his_que_ptr->item_info_ptr;    
        while (PNULL != current_history_item_ptr)
        {
            load_size += sizeof(VP_HISTORY_ITEM_T);
            if (current_history_item_ptr->type == VP_PLAY_FROM_NET)
            {
                load_size += sizeof(VP_NET_MOVIE_INFO_T);
            }
            else
            {            
                load_size += sizeof(VP_LOCAL_MOVIE_FILE_INFO_T);        
            }
            current_history_item_ptr = current_history_item_ptr->next_item_ptr;
        }
    }
    
    //VP_STRACE_LOW:"[VP] GetHistoryListSpaceSize load_size = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_158_112_2_18_3_5_18_284,(uint8*)"d",load_size);
    return load_size;
}

/*****************************************************************************/
//  Description : �ͷ���ʷ��¼��Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeHistoryListItem(uint32 index)
{
    VP_HISTORY_ITEM_T   *cur_item_ptr   = PNULL;
    VP_HISTORY_ITEM_T   *prev_item_ptr  = PNULL;
    uint32              i               = 0;
    BOOLEAN             result          = FALSE;
    
    //VP_STRACE_LOW:"[VP] FreeHistoryListItem index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_173_112_2_18_3_5_18_285,(uint8*)"d",index);
    
    if(s_visit_his_que_ptr != PNULL && index < s_visit_his_que_ptr->item_count)
    {
        //���Ҹ���ʷ��¼
        cur_item_ptr = s_visit_his_que_ptr->item_info_ptr;    
        prev_item_ptr = cur_item_ptr;
        for (i = 0; ((i < index) && (PNULL != cur_item_ptr)); i++)
        {
            prev_item_ptr = cur_item_ptr;
            cur_item_ptr = cur_item_ptr->next_item_ptr;
        }
          
        if (i == index && PNULL != cur_item_ptr)
        {
            //ɾ���ýڵ�
            if (i == 0)
            {
                //����ǵ�һ����¼
                s_visit_his_que_ptr->item_info_ptr = cur_item_ptr->next_item_ptr; 
            }
            else
            {
                prev_item_ptr->next_item_ptr = cur_item_ptr->next_item_ptr;
            }
            s_visit_his_que_ptr->item_count --;

            //�ͷ���ʷ��¼��Դ
            if (PNULL != cur_item_ptr->local_movie_file_info_ptr)
            {
                SCI_FREE(cur_item_ptr->local_movie_file_info_ptr);
                cur_item_ptr->local_movie_file_info_ptr = PNULL;
            }
            if (PNULL != cur_item_ptr->net_movie_info_ptr)
            {
                SCI_FREE(cur_item_ptr->net_movie_info_ptr);
                cur_item_ptr->net_movie_info_ptr = PNULL;
            }
            SCI_FREE(cur_item_ptr);
            cur_item_ptr = PNULL;
            result = TRUE;
        }
    }
    //VP_STRACE_LOW:"[VP] FreeHistoryListItem  result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_216_112_2_18_3_5_18_286,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : ������ʷ��¼�б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_LIST_ERROR_E SaveHistorylistToFile(void)
{
    VP_LIST_ERROR_E             save_result                 = VP_LIST_ERROR_OTHER;
    MMIFILE_HANDLE              file_handle                 = SFS_INVALID_HANDLE;
    wchar                       abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                      abs_default_path_length     = VP_MAX_NAME_LEN;
    VP_HISTORYLIST_HEAD_T       list_head                   = {0};
    uint32                      write_size                  = 0;
    uint32                      load_size                   = 0;
    uint8                       *buffer_ptr                 = PNULL;
    uint8                       *current_curser             = PNULL;
    VP_HISTORY_ITEM_T           *current_history_item_ptr   = PNULL;
    MMIFILE_ERROR_E             file_write_result           = SFS_ERROR_NONE;
    
    //��ȡ��ʷ��¼�б��ļ����ļ�����Ϣ
    if(GetHistoryListFileNameForWrite(abs_default_path,&abs_default_path_length))
    {
        //ɾ��Ĭ���ļ�
        if (MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {
            MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        }
            
        //�ж��Ƿ������ʷ��¼
        if(PNULL != s_visit_his_que_ptr && s_visit_his_que_ptr->item_count > 0)
        {
            //���´�������Ĭ���ļ�
            file_handle = MMIAPIFMM_CreateFile(
                abs_default_path, 
                SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                NULL, 
                NULL);
            if(SFS_INVALID_HANDLE != file_handle)
            {
                //��ȡ��ʷ��¼�б��ļ�ͷ��Ϣ;     ����ʷ��¼�б��ļ�ͷ��Ϣд���ļ�
                SCI_MEMSET(list_head.version,0,VP_HISTORYLIST_VER_LEN+1);
                SCI_MEMCPY(list_head.version, VP_HISTORYLIST_VER, VP_HISTORYLIST_VER_LEN);         
                file_write_result = MMIAPIFMM_WriteFile(file_handle, 
                                        &list_head, 
                                        sizeof(VP_HISTORYLIST_HEAD_T), 
                                        &write_size, 
                                        PNULL);

                if(SFS_ERROR_NONE == file_write_result)
                {                
                    if (write_size != sizeof(VP_HISTORYLIST_HEAD_T))
                    {
                        file_write_result = SFS_ERROR_NO_SPACE;
                    }
                    else
                    { 
                        //����������ʷ��¼�б�Ĵ�С
                        load_size = GetHistoryListSpaceSize();
                        if (0 != load_size)
                        {
                            //���������ռ�
                            buffer_ptr = (uint8 *)SCI_ALLOCA(load_size);
                            if(PNULL != buffer_ptr)
                            {
                                //����ʷ��¼�б����Ϣ���������ռ���
                                SCI_MEMSET(buffer_ptr,0,load_size);
                                //VP_STRACE_LOW:"[VP] SaveHistorylistToFile load_size = %x, buffer_ptr = %x"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_285_112_2_18_3_5_18_287,(uint8*)"dd",load_size,buffer_ptr);
                                current_curser= buffer_ptr;
                                current_history_item_ptr = s_visit_his_que_ptr->item_info_ptr;
                                SCI_MEMCPY(current_curser,s_visit_his_que_ptr,sizeof(VP_HISTORY_CONTEXT_T));
                                current_curser += sizeof(VP_HISTORY_CONTEXT_T);
                                while (PNULL != current_history_item_ptr)                  
                                {
                                    SCI_MEMCPY(current_curser,current_history_item_ptr,sizeof(VP_HISTORY_ITEM_T));
                                    current_curser += sizeof(VP_HISTORY_ITEM_T);   
                                   // VP_STRACE_LOW("[VP] SaveHistorylistToFile current_curser = %x",current_curser);
                                    if (current_history_item_ptr->type == VP_PLAY_FROM_NET)
                                    {
                                        SCI_MEMCPY(current_curser,current_history_item_ptr->net_movie_info_ptr,sizeof(VP_NET_MOVIE_INFO_T));
                                        current_curser += sizeof(VP_NET_MOVIE_INFO_T);
                                    } 
                                    else
                                    {
                                        SCI_MEMCPY(current_curser,current_history_item_ptr->local_movie_file_info_ptr,sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
                                        current_curser += sizeof(VP_LOCAL_MOVIE_FILE_INFO_T);
                                        if(0 == current_history_item_ptr->local_movie_file_info_ptr->full_path_len)
                                        {
                                            //SCI_TRACE_LOW:"[VP]SaveHistorylistToFile pathlen==0  error!!!!!!!"
                                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_306_112_2_18_3_5_18_288,(uint8*)"");
                                            file_write_result = SFS_ERROR_SYSTEM;
                                        }
                                    }
                                  //  VP_STRACE_LOW("[VP] SaveHistorylistToFile current_curser = %x",current_curser);
                                    //��ȡ��һ����ʷ��¼
                                    current_history_item_ptr = current_history_item_ptr->next_item_ptr;
                                }
                                if(SFS_ERROR_NONE == file_write_result)
                                {
                                    //����ʷ��¼��Ϣд��Ĭ���ļ�
                                    file_write_result = MMIAPIFMM_WriteFile(file_handle, buffer_ptr, load_size, &write_size, PNULL);
                                    if(SFS_ERROR_NONE ==  file_write_result)
                                    {
                                        if (load_size == write_size)
                                        {                                    
                                            //VP_STRACE_LOW:"[VP] SaveHistorylistToFile write_size = %d"
                                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_322_112_2_18_3_5_18_289,(uint8*)"d",write_size);
                                            save_result = VP_LIST_ERROR_NONE;
                                        }
                                        else
                                        {
                                            file_write_result = SFS_ERROR_NO_SPACE;
                                        }
                                    } 
                                }
                                SCI_FREE(buffer_ptr);
                                buffer_ptr = PNULL;
                            }
                        }
                    }
                }
                if (SFS_ERROR_NO_SPACE == file_write_result)
                {
                    save_result = VP_LIST_ERROR_NO_SPACE;
                }
                MMIAPIFMM_CloseFile(file_handle);            
            }            
        
        }
        else
        {
            save_result = VP_LIST_ERROR_NONE;
        }
       
    }
    return save_result;        
}

/*****************************************************************************/
//  Description : ������ʷ��¼,����ҵ�ֱ��ɾ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteItemFromHistorylist(VP_HISTORY_ITEM_T  history_list_item)
{   
    VP_HISTORY_ITEM_T  *current_item_ptr    = PNULL;
    VP_HISTORY_ITEM_T  *pre_item_ptr        = PNULL;
    BOOLEAN             result              = FALSE;
    

    if (PNULL != s_visit_his_que_ptr)
    {
        current_item_ptr = s_visit_his_que_ptr->item_info_ptr;    
    } 
    
    //���Ҹ���ʷ��¼
    if (VP_PLAY_FROM_NET == history_list_item.type && PNULL != history_list_item.net_movie_info_ptr)
    {        
        while((PNULL != current_item_ptr)
            && ((PNULL == current_item_ptr->net_movie_info_ptr)
            || (0 != strcmp(current_item_ptr->net_movie_info_ptr->url_ptr,history_list_item.net_movie_info_ptr->url_ptr))))
        {
             //��ȡ��һ����ʷ��¼
            pre_item_ptr = current_item_ptr;
            current_item_ptr = current_item_ptr->next_item_ptr;
        }
    }  
    else if(PNULL != history_list_item.local_movie_file_info_ptr)            
    {
        while((PNULL != current_item_ptr) 
            &&((PNULL == current_item_ptr->local_movie_file_info_ptr)
            || (0 != MMIAPICOM_Wstrcmp (current_item_ptr->local_movie_file_info_ptr->full_path_ptr,history_list_item.local_movie_file_info_ptr->full_path_ptr))))
        {
             //��ȡ��һ����ʷ��¼
            pre_item_ptr = current_item_ptr;
            current_item_ptr = current_item_ptr->next_item_ptr;
        }
    }    
        
   
    if (PNULL != current_item_ptr)
    {
        //����ʷ��¼�б���ɾ���ýڵ�
        if (PNULL == pre_item_ptr)
        {       
            //ɾ�����ǵ�һ����ʷ��¼
            s_visit_his_que_ptr->item_info_ptr = current_item_ptr->next_item_ptr;
        }
        else
        {
            pre_item_ptr->next_item_ptr = current_item_ptr->next_item_ptr;
        }
        //�ͷŽڵ�ռ�
        if (PNULL != current_item_ptr->net_movie_info_ptr)
        {
            SCI_FREE(current_item_ptr->net_movie_info_ptr);
        }
        if (PNULL != current_item_ptr->local_movie_file_info_ptr)
        {
            SCI_FREE(current_item_ptr->local_movie_file_info_ptr);
        }        
        SCI_FREE(current_item_ptr);
        current_item_ptr = PNULL;
        //��ʷ��¼����-1
        s_visit_his_que_ptr->item_count--;
        result = TRUE;
    }         
    
    return result;
    
}
/*****************************************************************************/
//  Description : ��ʼ����ʷ��¼�б�������ʷ��¼��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_Load( void )
{
    
    BOOLEAN                     readlist_result             = FALSE;
    wchar                      full_path_name[VP_MAX_NAME_LEN+1] = {0};
    uint16                      full_path_len               = VP_MAX_NAME_LEN;
    MMIFILE_HANDLE              file_handle                 = 0;
    VP_HISTORYLIST_HEAD_T       list_head                   = {0};
    uint32                      read_size                   = 0;
    uint32                      load_size                   = 0;
    VP_HISTORY_ITEM_T           * cur_item_info_ptr         = PNULL;
    uint8                       * temp_item_info_ptr        = PNULL;
    uint32                      i                           = 0;
    
    //1.�����ж�Ĭ���ļ��Ƿ��ƻ�
    //��ȡĬ���ļ�ȫ·��
    if(GetHistoryListFileNameForRead(full_path_name,&full_path_len))
    {    
        //���ļ�
        file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);   /*lint !e655*/     
        if(SFS_INVALID_HANDLE != file_handle)
        {
            //��ȡhead info,����汾�Ƿ���ȷ�������Ƿ�����
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle,&list_head,sizeof(VP_HISTORYLIST_HEAD_T),&read_size,NULL)
                && (0 == MMIAPICOM_Stricmp((uint8 *)list_head.version, (uint8 *)VP_HISTORYLIST_VER)))
            { 
                readlist_result = TRUE;                                
            }
        }
    }
    
    //2������ʷ��¼
    //VP_STRACE_LOW:"[VP] VP_HistoryList_Load readlist_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_462_112_2_18_3_5_19_290,(uint8*)"d",readlist_result);
    if(readlist_result)  //Ĭ���ļ�����
    {
        //�ͷ���ʷ��¼�б�
        if (PNULL != s_visit_his_que_ptr)
        {
            VP_HistoryList_Release();
        }
        s_visit_his_que_ptr = ( VP_HISTORY_CONTEXT_T* )SCI_ALLOCA(sizeof(VP_HISTORY_CONTEXT_T));        
        //VP_STRACE_LOW:"[VP] VP_HistoryList_Load s_visit_his_que_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_471_112_2_18_3_5_19_291,(uint8*)"d",s_visit_his_que_ptr);
        if (PNULL != s_visit_his_que_ptr)
        {
            SCI_MEMSET(s_visit_his_que_ptr,0,sizeof(VP_HISTORY_CONTEXT_T));
            load_size = sizeof(VP_HISTORY_CONTEXT_T);
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, s_visit_his_que_ptr, load_size, &read_size, NULL)
                && 0 < s_visit_his_que_ptr->item_count)
            {  
                //VP_STRACE_LOW:"[VP] VP_HistoryList_Load s_visit_his_que_ptr->item_count = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_479_112_2_18_3_5_19_292,(uint8*)"d",s_visit_his_que_ptr->item_count);
                load_size = sizeof(VP_HISTORY_ITEM_T);
                s_visit_his_que_ptr->item_info_ptr = (VP_HISTORY_ITEM_T*)SCI_ALLOCA(sizeof(VP_HISTORY_ITEM_T));
                cur_item_info_ptr = s_visit_his_que_ptr->item_info_ptr;   
                //VP_STRACE_LOW:"[VP] VP_HistoryList_Load cur_item_info_ptr = %x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_483_112_2_18_3_5_19_293,(uint8*)"d",cur_item_info_ptr);
                
                //���ش��ڵ���ʷ��¼��Ϣ
                for (i = 0; ((i < s_visit_his_que_ptr->item_count)&&(PNULL != cur_item_info_ptr)); i++)
                {                 
                    
                   // VP_STRACE_LOW("[VP] VP_HistoryList_Load load_size = %d, s_visit_his_que_ptr->item_count = %d",load_size,s_visit_his_que_ptr->item_count);
                    SCI_MEMSET(cur_item_info_ptr,0,load_size);
                    if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, cur_item_info_ptr, load_size, &read_size, NULL))
                    {                        
                        cur_item_info_ptr->local_movie_file_info_ptr = PNULL;
                        cur_item_info_ptr->net_movie_info_ptr = PNULL;
                        cur_item_info_ptr->next_item_ptr=PNULL;
                        if(VP_PLAY_FROM_NET == cur_item_info_ptr->type)
                        {
                            cur_item_info_ptr->net_movie_info_ptr = (VP_NET_MOVIE_INFO_T*)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                            load_size = sizeof(VP_NET_MOVIE_INFO_T);
                            temp_item_info_ptr = (uint8 *)cur_item_info_ptr->net_movie_info_ptr;
                        }
                        else
                        {
                            cur_item_info_ptr->local_movie_file_info_ptr = (VP_LOCAL_MOVIE_FILE_INFO_T*)SCI_ALLOCA(sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
                            load_size = sizeof(VP_LOCAL_MOVIE_FILE_INFO_T);
                            temp_item_info_ptr = (uint8 *)cur_item_info_ptr->local_movie_file_info_ptr;
                        }
                      //  VP_STRACE_LOW("[VP] temp_item_info_ptr = %x, VP_HistoryList_Load load_size = %d,cur_item_info_ptr->type = %d",temp_item_info_ptr,load_size,cur_item_info_ptr->type);    
                        if (PNULL != temp_item_info_ptr)
                        {
                            SCI_MEMSET(temp_item_info_ptr,0,load_size);
                            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, temp_item_info_ptr, load_size, &read_size, NULL))
                            {           
                                //VP_STRACE_LOW:"[VP] VP_HistoryList_Load read_size = %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_513_112_2_18_3_5_19_294,(uint8*)"d",read_size);
                                break;
                            }
                        }
                        else
                        {   
                            break;
                        }    
                        load_size = sizeof(VP_HISTORY_ITEM_T);                       
                        if (i < s_visit_his_que_ptr->item_count-1)
                        {
                            cur_item_info_ptr->next_item_ptr = (struct VP_HISTORY_ITEM_tag*)SCI_ALLOCA(load_size);                            
                        }
                        cur_item_info_ptr = cur_item_info_ptr->next_item_ptr;  
                    }   
                    else
                    {
                        break;
                    }
                
                }
                if (i != s_visit_his_que_ptr->item_count)
                {
                    readlist_result = FALSE;                    
                }
                
            }
            else
            {
                readlist_result = FALSE;                
            }
            
        }    
        else
        {
            readlist_result = FALSE;
        }
        
    }
    else
    {   
        readlist_result = TRUE;
        s_visit_his_que_ptr = PNULL;
    }    
    
    //�ر�Ĭ���ļ�
    if(PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
    
    if (!readlist_result)
    {
        MMIAPIFMM_DeleteFile(full_path_name,PNULL);
        VP_HistoryList_Release();
    }

    return readlist_result;     
}

/*****************************************************************************/
//  Description : �ͷ���ʷ��¼��Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_HistoryList_Release( void )
{
    
    VP_HISTORY_ITEM_T  *cur_history_item_ptr    = PNULL;
    
    if (PNULL != s_visit_his_que_ptr)
    { 
        cur_history_item_ptr = s_visit_his_que_ptr->item_info_ptr;
        while (PNULL != cur_history_item_ptr)
        {
            s_visit_his_que_ptr->item_info_ptr = cur_history_item_ptr->next_item_ptr;
            if (PNULL != cur_history_item_ptr->local_movie_file_info_ptr)
            {
                SCI_FREE(cur_history_item_ptr->local_movie_file_info_ptr);
                cur_history_item_ptr->local_movie_file_info_ptr = PNULL;
            }
            if (PNULL != cur_history_item_ptr->net_movie_info_ptr)
            {
                SCI_FREE(cur_history_item_ptr->net_movie_info_ptr);
                cur_history_item_ptr->net_movie_info_ptr = PNULL;
            }
            SCI_FREE(cur_history_item_ptr);
            cur_history_item_ptr = s_visit_his_que_ptr->item_info_ptr;
        }
    
        SCI_FREE(s_visit_his_que_ptr);
        s_visit_his_que_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : ��ȡ��ʷ��¼����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_HistoryList_GetNum( void )
{
    uint32    total_history_item = 0;
    if (PNULL != s_visit_his_que_ptr)
    {
        total_history_item = s_visit_his_que_ptr-> item_count;
    }
    return total_history_item;
}

/*****************************************************************************/
//  Description : ��ȡĳһ��ʷ��¼����Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_HISTORY_ITEM_T * VP_HistoryList_GetInfo(
                                                uint32              historylist_index                                      
                                                )
{
    uint32              i                       = 0;
    VP_HISTORY_ITEM_T    *cur_history_item_ptr = PNULL;

    //VP_STRACE_LOW:"[VP] VP_HistoryList_GetInfo list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_632_112_2_18_3_5_19_295,(uint8*)"d",historylist_index);

    //�������
    if (PNULL != s_visit_his_que_ptr && historylist_index < s_visit_his_que_ptr->item_count)
    {      
        //������ʷ��¼
        cur_history_item_ptr = s_visit_his_que_ptr->item_info_ptr;
        while (PNULL != cur_history_item_ptr && i < historylist_index)
        {            
            i++;
            cur_history_item_ptr = cur_history_item_ptr->next_item_ptr;
        }    
    }
    //VP_STRACE_LOW:"[VP] VP_HistoryList_GetInfo i=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_645_112_2_18_3_5_19_296,(uint8*)"d",i);
    return cur_history_item_ptr;
}


/*****************************************************************************/
//  Description : ɾ��ĳһ��ʷ���ż�¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelItem( uint32 index )
{
    BOOLEAN     result      = FALSE;
    
    
    //VP_STRACE_LOW:"[VP] VP_HistoryList_DelItem index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_659_112_2_18_3_5_19_297,(uint8*)"d",index);
    if(s_visit_his_que_ptr == PNULL || index >= s_visit_his_que_ptr->item_count)
    {
        //VP_STRACE_LOW:"[VP] VP_HistoryList_DelItem error param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_662_112_2_18_3_5_19_298,(uint8*)"");
        return TRUE;
    }
    //�ͷŸ���ʷ��¼�Ŀռ�
    if(FreeHistoryListItem(index) && VP_LIST_ERROR_NONE == SaveHistorylistToFile())
    {
        
        result = TRUE;
    }
    //VP_STRACE_LOW:"[VP] VP_HistoryList_DelItem result = %d, s_visit_his_que_ptr->item_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_671_112_2_18_3_5_19_299,(uint8*)"dd",result,s_visit_his_que_ptr->item_count);
    return result;
}

/*****************************************************************************/
//  Description : ɾ��ȫ����ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelAllItem(void )
{
    BOOLEAN result = FALSE;
    VP_HistoryList_Release();
    if (VP_LIST_ERROR_NONE == SaveHistorylistToFile())
    {
        result = TRUE;
    }       
    return result;
}


/*****************************************************************************/
//  Description : �����ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_AddItem(
                                      VP_HISTORY_ITEM_T  history_list_item
                                      )
{      
    
    VP_HISTORY_ITEM_T  *current_history_item_ptr    = PNULL;   
   
    
    //������ʷ��¼�ڵ㣻
    current_history_item_ptr = (VP_HISTORY_ITEM_T*)SCI_ALLOCA(sizeof(VP_HISTORY_ITEM_T));
    if (PNULL == current_history_item_ptr)
    {   
        //VP_STRACE_LOW:"[VP] VP_HistoryList_AddItem there is no enough space"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_707_112_2_18_3_5_19_300,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(current_history_item_ptr,0,sizeof(VP_HISTORY_ITEM_T));
    current_history_item_ptr->type = history_list_item.type;
    if (current_history_item_ptr->type == VP_PLAY_FROM_NET)
    {
        current_history_item_ptr->net_movie_info_ptr = (VP_NET_MOVIE_INFO_T*)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
        if (PNULL != current_history_item_ptr->net_movie_info_ptr)
        {
            SCI_MEMSET(current_history_item_ptr->net_movie_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
            SCI_MEMCPY(current_history_item_ptr->net_movie_info_ptr,history_list_item.net_movie_info_ptr,sizeof(VP_NET_MOVIE_INFO_T));
            current_history_item_ptr->local_movie_file_info_ptr = PNULL;
        }
        else
        {
            SCI_FREE(current_history_item_ptr);
            current_history_item_ptr = PNULL;
            return FALSE;
        }
        
    }
    else
    {
        current_history_item_ptr->local_movie_file_info_ptr = (VP_LOCAL_MOVIE_FILE_INFO_T*)SCI_ALLOCA(sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
        if (PNULL != current_history_item_ptr->local_movie_file_info_ptr)
        {
            SCI_MEMSET(current_history_item_ptr->local_movie_file_info_ptr,0,sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
            SCI_MEMCPY(current_history_item_ptr->local_movie_file_info_ptr,history_list_item.local_movie_file_info_ptr,sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
            current_history_item_ptr->net_movie_info_ptr = PNULL;
        }
        else
        {
            SCI_FREE(current_history_item_ptr);
            current_history_item_ptr = PNULL;
            return FALSE;
        }
        
    }
    current_history_item_ptr->last_play_point = history_list_item.last_play_point;
    current_history_item_ptr->next_item_ptr = PNULL;
    current_history_item_ptr->locallist_index = history_list_item.locallist_index;
    current_history_item_ptr->locallist_type = history_list_item.locallist_type;
    //VP_STRACE_LOW:"[VP] VP_HistoryList_AddItem current_history_item_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_750_112_2_18_3_5_19_301,(uint8*)"d",current_history_item_ptr);
    //ɾ���Ѿ����ڵ�
    DeleteItemFromHistorylist(history_list_item);    
    
    //�������һ���ڵ�
    if(PNULL != s_visit_his_que_ptr && s_visit_his_que_ptr->item_count >= VP_MAX_HISTORY)
    {
        //�б�������ɾ����ʷ��õ�һ����¼
        FreeHistoryListItem(s_visit_his_que_ptr->item_count - 1);
        
    }
    else if (PNULL == s_visit_his_que_ptr)
    {
        s_visit_his_que_ptr = (VP_HISTORY_CONTEXT_T* )SCI_ALLOCA(sizeof( VP_HISTORY_CONTEXT_T));
        if (PNULL != s_visit_his_que_ptr)
        {
            //VP_STRACE_LOW:"[VP] VP_HistoryList_AddItem s_visit_his_que_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_766_112_2_18_3_5_19_302,(uint8*)"d",s_visit_his_que_ptr);
            s_visit_his_que_ptr->item_count = 0;
            s_visit_his_que_ptr->item_info_ptr = PNULL;        
        }
        else
        {
            //VP_STRACE_LOW:"[VP] VP_HistoryList_AddItem there is no enough space"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_772_112_2_18_3_5_19_303,(uint8*)"");
            if (PNULL != current_history_item_ptr->local_movie_file_info_ptr)
            {
                SCI_FREE(current_history_item_ptr->local_movie_file_info_ptr);
                current_history_item_ptr->local_movie_file_info_ptr = PNULL;
            }
            if (PNULL != current_history_item_ptr->net_movie_info_ptr)
            {
                SCI_FREE(current_history_item_ptr->net_movie_info_ptr);
                current_history_item_ptr->net_movie_info_ptr = PNULL;
            }
            SCI_FREE(current_history_item_ptr);
            current_history_item_ptr = PNULL;
            return FALSE;
        }
        
    }
    
    //������ʷ��¼
    current_history_item_ptr->next_item_ptr = s_visit_his_que_ptr->item_info_ptr;
    s_visit_his_que_ptr->item_info_ptr = current_history_item_ptr;    
    s_visit_his_que_ptr->item_count++;
    
    
    //����ǩ�б���Ϣд��Ĭ���ļ���
    if(VP_LIST_ERROR_NONE != SaveHistorylistToFile())
    {
        VP_HistoryList_Release();
        VP_HistoryList_Load();
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Ϊ���ڲ��ŵ���ʷ��¼�������һ��ֹͣ���ŵ�λ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_SetLastPlayPoint(uint32        last_play_point )
{
    VP_HISTORY_ITEM_T   *cur_item_ptr = PNULL;
    
    //VP_STRACE_LOW:"[VP] VP_HistoryList_SetLastPlayPoint last_play_point = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_814_112_2_18_3_5_19_304,(uint8*)"d",last_play_point);

    if(PNULL == s_visit_his_que_ptr || PNULL == s_visit_his_que_ptr->item_info_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_HistoryList_SetLastPlayPoint list error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_818_112_2_18_3_5_19_305,(uint8*)"");
        return FALSE;
    }
    
    //��ȡ���һ����ʷ��¼
    cur_item_ptr = s_visit_his_que_ptr->item_info_ptr;
    cur_item_ptr->last_play_point = last_play_point;  
    if (VP_LIST_ERROR_NONE == SaveHistorylistToFile())
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : �޸���ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_ModifyItem(                                         
                                         uint32 last_play_point,
                                         uint32 total_time,
                                         uint32 video_height,
                                         uint32 video_width,
                                         uint32 media_length,
                                         uint16 title_len,
                                         wchar  *title_ptr)
{   
    BOOLEAN             result = FALSE;

   
    //VP_STRACE_LOW:"[VP] VP_HistoryList_ModifyItem  last_play_point = %d, video_height = %d, video_width = %d, media_length = %d, title_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_849_112_2_18_3_5_19_306,(uint8*)"ddddd",last_play_point,video_height,video_width,media_length, title_len );
    if (PNULL != s_visit_his_que_ptr && PNULL != s_visit_his_que_ptr->item_info_ptr)
    {   
        s_visit_his_que_ptr->item_info_ptr->last_play_point = last_play_point;
        s_visit_his_que_ptr->item_info_ptr->total_time = total_time;       
        if (PNULL != title_ptr  && PNULL != s_visit_his_que_ptr->item_info_ptr->net_movie_info_ptr)
        {
            s_visit_his_que_ptr->item_info_ptr->net_movie_info_ptr->title_len = MIN(title_len, VP_MAX_NAME_LEN);
            SCI_MEMSET(s_visit_his_que_ptr->item_info_ptr->net_movie_info_ptr->title_arr,0,((VP_MAX_NAME_LEN+1)*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(s_visit_his_que_ptr->item_info_ptr->net_movie_info_ptr->title_arr,
                title_ptr,
                s_visit_his_que_ptr->item_info_ptr->net_movie_info_ptr->title_len);
        }
        else if(PNULL != s_visit_his_que_ptr->item_info_ptr->local_movie_file_info_ptr)
        {
            s_visit_his_que_ptr->item_info_ptr->local_movie_file_info_ptr->height = video_height;
            s_visit_his_que_ptr->item_info_ptr->local_movie_file_info_ptr->width = video_width;
            s_visit_his_que_ptr->item_info_ptr->local_movie_file_info_ptr->media_length = media_length;
        }

        if (VP_LIST_ERROR_NONE == SaveHistorylistToFile())
        {
            result = TRUE;
        }
        
    }
    return result;
}

#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT


