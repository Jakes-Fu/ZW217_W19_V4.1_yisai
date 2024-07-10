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
LOCAL   VP_HISTORY_CONTEXT_T    *s_visit_his_que_ptr    = PNULL;  //历史记录列表


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取存放历史记录列表文件的路径
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForRead(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     );
/*****************************************************************************/
//  Description : 获取存放历史记录列表文件的路径
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetHistoryListFileNameForWrite(
                                     wchar          *full_path_name_ptr,    
                                     uint16          *full_path_len_ptr      
                                     );
/*****************************************************************************/
//  Description : 获得存放历史记录列表需要的连续空间大小
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetHistoryListSpaceSize(void);

/*****************************************************************************/
//  Description : 查找历史记录索引
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteItemFromHistorylist(
                                        VP_HISTORY_ITEM_T  history_list_item);

/*****************************************************************************/
//  Description : 释放历史记录资源
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeHistoryListItem(uint32 index);

/*****************************************************************************/
//  Description : 保存历史记录列表信息到默认文件中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_LIST_ERROR_E SaveHistorylistToFile(void);
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取存放历史记录列表文件的路径
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
//  Description : 获取存放历史记录列表文件的路径
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
//  Description : 获得存放历史记录列表需要的连续空间大小
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 依赖s_visit_his_que_ptr
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
//  Description : 释放历史记录资源
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
        //查找该历史记录
        cur_item_ptr = s_visit_his_que_ptr->item_info_ptr;    
        prev_item_ptr = cur_item_ptr;
        for (i = 0; ((i < index) && (PNULL != cur_item_ptr)); i++)
        {
            prev_item_ptr = cur_item_ptr;
            cur_item_ptr = cur_item_ptr->next_item_ptr;
        }
          
        if (i == index && PNULL != cur_item_ptr)
        {
            //删除该节点
            if (i == 0)
            {
                //如果是第一个记录
                s_visit_his_que_ptr->item_info_ptr = cur_item_ptr->next_item_ptr; 
            }
            else
            {
                prev_item_ptr->next_item_ptr = cur_item_ptr->next_item_ptr;
            }
            s_visit_his_que_ptr->item_count --;

            //释放历史记录资源
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
//  Description : 保存历史记录列表信息到默认文件中
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
    
    //获取历史记录列表文件的文件名信息
    if(GetHistoryListFileNameForWrite(abs_default_path,&abs_default_path_length))
    {
        //删除默认文件
        if (MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {
            MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        }
            
        //判断是否存在历史记录
        if(PNULL != s_visit_his_que_ptr && s_visit_his_que_ptr->item_count > 0)
        {
            //重新创建并打开默认文件
            file_handle = MMIAPIFMM_CreateFile(
                abs_default_path, 
                SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                NULL, 
                NULL);
            if(SFS_INVALID_HANDLE != file_handle)
            {
                //获取历史记录列表文件头信息;     将历史记录列表文件头信息写入文件
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
                        //计算整个历史记录列表的大小
                        load_size = GetHistoryListSpaceSize();
                        if (0 != load_size)
                        {
                            //创建连续空间
                            buffer_ptr = (uint8 *)SCI_ALLOCA(load_size);
                            if(PNULL != buffer_ptr)
                            {
                                //将历史记录列表的信息放入连续空间中
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
                                    //获取下一个历史记录
                                    current_history_item_ptr = current_history_item_ptr->next_item_ptr;
                                }
                                if(SFS_ERROR_NONE == file_write_result)
                                {
                                    //将历史记录信息写入默认文件
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
//  Description : 查找历史记录,如果找到直接删除
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
    
    //查找该历史记录
    if (VP_PLAY_FROM_NET == history_list_item.type && PNULL != history_list_item.net_movie_info_ptr)
    {        
        while((PNULL != current_item_ptr)
            && ((PNULL == current_item_ptr->net_movie_info_ptr)
            || (0 != strcmp(current_item_ptr->net_movie_info_ptr->url_ptr,history_list_item.net_movie_info_ptr->url_ptr))))
        {
             //获取下一个历史记录
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
             //获取下一个历史记录
            pre_item_ptr = current_item_ptr;
            current_item_ptr = current_item_ptr->next_item_ptr;
        }
    }    
        
   
    if (PNULL != current_item_ptr)
    {
        //从历史记录列表中删除该节点
        if (PNULL == pre_item_ptr)
        {       
            //删除的是第一个历史记录
            s_visit_his_que_ptr->item_info_ptr = current_item_ptr->next_item_ptr;
        }
        else
        {
            pre_item_ptr->next_item_ptr = current_item_ptr->next_item_ptr;
        }
        //释放节点空间
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
        //历史记录总数-1
        s_visit_his_que_ptr->item_count--;
        result = TRUE;
    }         
    
    return result;
    
}
/*****************************************************************************/
//  Description : 初始化历史记录列表，加载历史记录信息
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
    
    //1.查找判断默认文件是否被破坏
    //获取默认文件全路径
    if(GetHistoryListFileNameForRead(full_path_name,&full_path_len))
    {    
        //打开文件
        file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);   /*lint !e655*/     
        if(SFS_INVALID_HANDLE != file_handle)
        {
            //读取head info,检验版本是否正确，数据是否完整
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle,&list_head,sizeof(VP_HISTORYLIST_HEAD_T),&read_size,NULL)
                && (0 == MMIAPICOM_Stricmp((uint8 *)list_head.version, (uint8 *)VP_HISTORYLIST_VER)))
            { 
                readlist_result = TRUE;                                
            }
        }
    }
    
    //2加载历史记录
    //VP_STRACE_LOW:"[VP] VP_HistoryList_Load readlist_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_462_112_2_18_3_5_19_290,(uint8*)"d",readlist_result);
    if(readlist_result)  //默认文件正常
    {
        //释放历史记录列表
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
                
                //加载存在的历史记录信息
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
    
    //关闭默认文件
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
//  Description : 释放历史记录资源
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
//  Description : 获取历史记录总数
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
//  Description : 获取某一历史记录的信息
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

    //参数检查
    if (PNULL != s_visit_his_que_ptr && historylist_index < s_visit_his_que_ptr->item_count)
    {      
        //查找历史记录
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
//  Description : 删除某一历史播放记录
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
    //释放该历史记录的空间
    if(FreeHistoryListItem(index) && VP_LIST_ERROR_NONE == SaveHistorylistToFile())
    {
        
        result = TRUE;
    }
    //VP_STRACE_LOW:"[VP] VP_HistoryList_DelItem result = %d, s_visit_his_que_ptr->item_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_HISTORYLIST_671_112_2_18_3_5_19_299,(uint8*)"dd",result,s_visit_his_que_ptr->item_count);
    return result;
}

/*****************************************************************************/
//  Description : 删除全部历史记录
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
//  Description : 添加历史记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_AddItem(
                                      VP_HISTORY_ITEM_T  history_list_item
                                      )
{      
    
    VP_HISTORY_ITEM_T  *current_history_item_ptr    = PNULL;   
   
    
    //创建历史记录节点；
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
    //删除已经存在的
    DeleteItemFromHistorylist(history_list_item);    
    
    //查找最后一个节点
    if(PNULL != s_visit_his_que_ptr && s_visit_his_que_ptr->item_count >= VP_MAX_HISTORY)
    {
        //列表已满，删除历史最久的一个记录
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
    
    //增加历史记录
    current_history_item_ptr->next_item_ptr = s_visit_his_que_ptr->item_info_ptr;
    s_visit_his_que_ptr->item_info_ptr = current_history_item_ptr;    
    s_visit_his_que_ptr->item_count++;
    
    
    //将书签列表信息写入默认文件中
    if(VP_LIST_ERROR_NONE != SaveHistorylistToFile())
    {
        VP_HistoryList_Release();
        VP_HistoryList_Load();
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 为正在播放的历史记录设置最后一次停止播放的位置
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
    
    //获取最后一个历史记录
    cur_item_ptr = s_visit_his_que_ptr->item_info_ptr;
    cur_item_ptr->last_play_point = last_play_point;  
    if (VP_LIST_ERROR_NONE == SaveHistorylistToFile())
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : 修改历史记录
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


