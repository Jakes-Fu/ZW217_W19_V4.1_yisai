/*****************************************************************************
** File Name:                    mivp_bookmarklist.c                                  *
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

#define MMIVP_BOOKMARKLIST_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_bookmarklist.h"
#include "mmivp_export.h"
#include "mmifmm_export.h"
#include "mmivp_historylist.h"
#include "mmivp_text.h"
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL VP_BOOKMARK_CONTEXT_T  *s_visit_bookmark_que_ptr = PNULL;   //��ǩ�б�
#ifdef WIN32
#define VP_CMCC_URL "rtsp://172.16.14.60/sprd_1.3gp"
#define VP_CMCC_URL_LEN 30
#else
#define VP_CMCC_URL "http://211.136.165.53/mobiletv.jsp"//andy.he_crNEWMS00112388
#define VP_CMCC_URL_LEN 35
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ������ǩ�б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_LIST_ERROR_E SaveBookmarklistToFile(void);

/*****************************************************************************/
//  Description : ��ȡ�����ǩ�б��ļ���·��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetBookmarkListFileNameForRead(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      );
/*****************************************************************************/
//  Description : ��ȡ�����ǩ�б��ļ���·���Թ�д
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetBookmarkListFileNameForWrite(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      );

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ������ǩ�б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_LIST_ERROR_E SaveBookmarklistToFile(void)
{
    VP_LIST_ERROR_E             save_result                         = VP_LIST_ERROR_OTHER;
    MMIFILE_HANDLE              file_handle                         = SFS_INVALID_HANDLE;
    wchar                       abs_default_path[VP_MAX_NAME_LEN+1] = {0};
    uint16                      abs_default_path_length             = VP_MAX_NAME_LEN;
    VP_BOOKMARKLIST_HEAD_T      list_head                           = {0};
    uint32                      write_size                          = 0;
    uint32                      load_size                           = 0;
    MMIFILE_ERROR_E             file_write_result                   = SFS_ERROR_NONE;

    
    //��ȡ��ǩ�б��ļ����ļ�����Ϣ
    if(GetBookmarkListFileNameForWrite(abs_default_path,&abs_default_path_length))
    {
        //ɾ����ǩĬ���ļ�
        if (MMIAPIFMM_IsFileExist(abs_default_path,abs_default_path_length))
        {
            MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
        }  
        //�ж��Ƿ������ǩ
        if(PNULL != s_visit_bookmark_que_ptr && s_visit_bookmark_que_ptr->item_count > 0)
        {             
            //��������ǩĬ���ļ�
            file_handle = MMIAPIFMM_CreateFile(
                abs_default_path, 
                SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                NULL, 
                NULL);
            if(SFS_INVALID_HANDLE != file_handle)
            {
                //��ȡ��ǩ�б��ļ�ͷ��Ϣ;     ����ǩ�б��ļ�ͷ��Ϣд���ļ�
                SCI_MEMSET(list_head.version,0,(VP_BOOKMARKLIST_VER_LEN+1));
                SCI_MEMCPY(list_head.version, VP_BOOKMARKLIST_VER, VP_BOOKMARKLIST_VER_LEN); 
              
                file_write_result = MMIAPIFMM_WriteFile(file_handle, &list_head, sizeof(VP_BOOKMARKLIST_HEAD_T),&write_size, PNULL);
          
                if(SFS_ERROR_NONE == file_write_result)
                {                
                    if (write_size != sizeof(VP_BOOKMARKLIST_HEAD_T))
                    {
                        file_write_result = SFS_ERROR_NO_SPACE;
                    }
                    else
                    {
                        load_size = sizeof(VP_BOOKMARK_CONTEXT_T);    
                        //����ǩ�б�д��Ĭ���ļ���
                        file_write_result = MMIAPIFMM_WriteFile(file_handle, s_visit_bookmark_que_ptr, load_size, &write_size, PNULL);
                        if(SFS_ERROR_NONE ==  file_write_result)
                        {
                            if (load_size == write_size)
                            {                            
                                //VP_STRACE_LOW:"[VP] SaveBookmarklistToFile write bookmark successfully"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_116_112_2_18_3_5_1_1,(uint8*)"");
                                save_result = VP_LIST_ERROR_NONE;
                            }
                            else
                            {
                                file_write_result = SFS_ERROR_NO_SPACE;
                            }
                        }  
                    }
                }
                            //�ر�Ĭ���ļ�
                MMIAPIFMM_CloseFile(file_handle);
                if(SFS_ERROR_NO_SPACE == file_write_result)
                {
                    save_result = VP_LIST_ERROR_NO_SPACE;
                    MMIAPIFMM_DeleteFile(abs_default_path,PNULL);
					VP_BookmarkList_Release();
                }

            }
        }
        else
        {
            //VP_STRACE_LOW:"[VP] SaveBookmarklistToFile bookmarklist is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_139_112_2_18_3_5_1_2,(uint8*)"");
            save_result = VP_LIST_ERROR_NONE;
        }
        
    }
    //VP_STRACE_LOW:"[VP] SaveBookmarklistToFile save_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_144_112_2_18_3_5_1_3,(uint8*)"d",save_result);
    return save_result;        
}

/*****************************************************************************/
//  Description : ��ȡ�����ǩ�б��ļ���·���Թ���ȡ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetBookmarkListFileNameForRead(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      )
{
    BOOLEAN         result      =   FALSE;    
    
    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)    
    {    
        result = MMIAPIFMM_GetSysFileFullPathForRead(
            TRUE,
            VP_DEFAULT_DIR,
            VP_DEFAULT_DIR_LEN,
            VP_BOOKMARKLIST_FILENAME,
            VP_BOOKMARKLIST_FILENAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }
    //VP_STRACE_LOW:"[VP] GetBookmarkListFileNameForRead result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_171_112_2_18_3_5_1_4,(uint8*)"d",result);
    return result; 
}

/*****************************************************************************/
//  Description : ��ȡ�����ǩ�б��ļ���·���Թ�д
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetBookmarkListFileNameForWrite(
                                      wchar          *full_path_name_ptr,    
                                      uint16          *full_path_len_ptr      
                                      )
{
    BOOLEAN         result          = FALSE;    
    uint32          min_free_size   = 0;

    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)
    {        
        min_free_size = sizeof(VP_BOOKMARK_CONTEXT_T)+sizeof(VP_BOOKMARKLIST_HEAD_T);
        result = MMIAPIFMM_GetSysFileFullPathForWrite(
            TRUE,
            VP_DEFAULT_DIR,
            VP_DEFAULT_DIR_LEN,
            VP_BOOKMARKLIST_FILENAME,
            VP_BOOKMARKLIST_FILENAME_LEN,
            min_free_size,
            full_path_name_ptr,
            full_path_len_ptr);
    }
    //VP_STRACE_LOW:"[VP] GetBookmarkListFileNameForWrite result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_200_112_2_18_3_5_1_5,(uint8*)"d",result);
    return result; 
}

/*****************************************************************************/
//  Description : ��ʼ����ǩ�б�������ǩ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_Load( void )
{
    
    BOOLEAN                     readlist_result                     = FALSE;
    wchar                       full_path_name[VP_MAX_NAME_LEN+1]   = {0};
    uint16                      full_path_len                       = VP_MAX_NAME_LEN;
    MMIFILE_HANDLE              file_handle                         = 0;
    VP_BOOKMARKLIST_HEAD_T      list_head                           = {0};
    uint32                      read_size                           = 0;
    uint32                      load_size                           = 0;
 
    //1.���Ĭ���ļ��Ƿ���ڣ��Ƿ��ƻ�
    //��ȡ��ǩ�б��Ĭ���ļ�ȫ·��
    if(GetBookmarkListFileNameForRead(full_path_name,&full_path_len))
    {    
        //���ļ�
        file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        if(SFS_INVALID_HANDLE != file_handle)
        {
            //��ȡhead info������汾�Ƿ���ȷ�������Ƿ�����
            SCI_MEMSET(&list_head,0,sizeof(VP_BOOKMARKLIST_HEAD_T));
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle,&list_head,sizeof(VP_BOOKMARKLIST_HEAD_T), &read_size,NULL)
                && 0 == MMIAPICOM_Stricmp((uint8 *)list_head.version, (uint8 *)VP_BOOKMARKLIST_VER))
            {  
                readlist_result = TRUE;
            }            
        }
    }

    //VP_STRACE_LOW:"[VP] VP_BookmarkList_Load readlist_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_237_112_2_18_3_5_1_6,(uint8*)"d",readlist_result);
    if(readlist_result)  //Ĭ���ļ�����
    {
        //2.��ȡ��ǩ��Ϣ
        //�ͷ���ǩ�б�
        if (PNULL != s_visit_bookmark_que_ptr)
        {
            VP_BookmarkList_Release();
        }
        s_visit_bookmark_que_ptr = ( VP_BOOKMARK_CONTEXT_T*)SCI_ALLOCA(sizeof(VP_BOOKMARK_CONTEXT_T));
        //VP_STRACE_LOW:"[VP] VP_BookmarkList_Load s_visit_bookmark_que_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_247_112_2_18_3_5_1_7,(uint8*)"d",s_visit_bookmark_que_ptr);
        if (PNULL != s_visit_bookmark_que_ptr)
        {
            SCI_MEMSET(s_visit_bookmark_que_ptr,0,sizeof(VP_BOOKMARK_CONTEXT_T));
            load_size = sizeof(VP_BOOKMARK_CONTEXT_T);
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, s_visit_bookmark_que_ptr, load_size, &read_size, NULL)
				|| load_size != read_size)
            {
                SCI_FREE(s_visit_bookmark_que_ptr);
                s_visit_bookmark_que_ptr = PNULL;
                readlist_result = FALSE;                
            }
            
        }
        else
        {
            readlist_result = FALSE;
            s_visit_bookmark_que_ptr = PNULL;
        }
        
    }
    else
    {
        readlist_result = TRUE;
        s_visit_bookmark_que_ptr = PNULL;
    }

    //�ر��ļ�
    if(PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }

    if (PNULL == s_visit_bookmark_que_ptr)
    {
        MMIAPIFMM_DeleteFile(full_path_name,PNULL);
    }
    
    return readlist_result;     
}

/*****************************************************************************/
//  Description : �ͷ���ǩ�б���Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_BookmarkList_Release( void )
{
    if (PNULL != s_visit_bookmark_que_ptr)
    {        
        SCI_FREE(s_visit_bookmark_que_ptr);
        s_visit_bookmark_que_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : ��ȡ��ǩ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_BookmarkList_GetNum( void )
{
    uint32  total_bookmark_num = 0;

    if (PNULL != s_visit_bookmark_que_ptr)
    { 
         total_bookmark_num = s_visit_bookmark_que_ptr->item_count;
    }

    //VP_STRACE_LOW:"[VP] VP_BookmarkList_GetNum total_bookmark_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_314_112_2_18_3_5_1_8,(uint8*)"d",total_bookmark_num);
    return total_bookmark_num;
}

/*****************************************************************************/
//  Description :��ȡĳһ��ǩ����Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_NET_MOVIE_INFO_T * VP_BookmarkList_GetInfo(uint32       bookmark_list_index)
{

    //VP_STRACE_LOW:"[VP] VP_BookmarkList_GetInfo list_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_325_112_2_18_3_5_1_9,(uint8*)"d",bookmark_list_index);

    if (bookmark_list_index >= VP_BOOKMARK_MAX_ITEM || PNULL == s_visit_bookmark_que_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_BookmarkList_GetInfo param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_329_112_2_18_3_5_1_10,(uint8*)"");
        return PNULL;
    }
    
    return &(s_visit_bookmark_que_ptr->item_info[bookmark_list_index]);   
}

/*****************************************************************************/
//  Description :ɾ��ĳһ��ǩ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelItem( uint32 index )
{
    BOOLEAN         result      = FALSE;
    uint32          i           = 0;
    
    //VP_STRACE_LOW:"[VP] VP_BookmarkList_DelItem index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_345_112_2_18_3_5_1_11,(uint8*)"d",index);

    if(PNULL != s_visit_bookmark_que_ptr && index < s_visit_bookmark_que_ptr->item_count)
    {        
        //����������ǩ��¼ǰ��
        for (i = index; i < s_visit_bookmark_que_ptr->item_count-1; i++)
        {
            SCI_MEMCPY(&(s_visit_bookmark_que_ptr->item_info[i]),&(s_visit_bookmark_que_ptr->item_info[i+1]),sizeof(VP_NET_MOVIE_INFO_T));
        } 
        //�ͷ����һ����ǩ
        SCI_MEMSET(&(s_visit_bookmark_que_ptr->item_info[i]),0,sizeof(VP_NET_MOVIE_INFO_T));
        //��ǩ��Ŀ-1
        s_visit_bookmark_que_ptr->item_count--;
        //������ǩ�б��ļ���
        if (VP_LIST_ERROR_NONE == SaveBookmarklistToFile())
        {
            result = TRUE;
        }
        
        //VP_STRACE_LOW:"[VP] VP_BookmarkList_DelItem result = %d, s_visit_bookmark_que_ptr->item_count = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_364_112_2_18_3_5_1_12,(uint8*)"dd",result,s_visit_bookmark_que_ptr->item_count);
    }
    
    return result;
}

/*****************************************************************************/
//  Description :ɾ��ȫ����ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelAllItem(void )
{
    BOOLEAN del_result = TRUE;

    VP_BookmarkList_Release();
    if (VP_LIST_ERROR_NONE == SaveBookmarklistToFile())
    {
        del_result = TRUE;
    }             

    return del_result;
}

/*****************************************************************************/
//  Description :�����ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_AddItem(VP_NET_MOVIE_INFO_T     bookmark_data)
{   
    uint32              last_bookmark_index     = 0;      
    VP_LIST_ERROR_E     add_result              = VP_LIST_ERROR_NONE;

    //�ж���ӵļ�¼�Ƿ񳬳�������,û�г������Ƶģ���ò����������
    if(PNULL != s_visit_bookmark_que_ptr && s_visit_bookmark_que_ptr->item_count >= VP_BOOKMARK_MAX_ITEM)
    {
        //VP_STRACE_LOW:"[VP] VP_BookmarkList_AddItem list too much bookmark "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_399_112_2_18_3_5_2_13,(uint8*)"");
        add_result = VP_LIST_ERROR_TOO_MUCH;
    }   
    else if (PNULL == s_visit_bookmark_que_ptr)
    {
        s_visit_bookmark_que_ptr = ( VP_BOOKMARK_CONTEXT_T*)SCI_ALLOCA(sizeof(VP_BOOKMARK_CONTEXT_T));
        if (PNULL != s_visit_bookmark_que_ptr)
        {
            SCI_MEMSET(s_visit_bookmark_que_ptr,0,sizeof(VP_BOOKMARK_CONTEXT_T));
            last_bookmark_index = 0;
        }
        else
        {
            add_result = VP_LIST_ERROR_NO_SPACE;
        }
    }
    else
    {
        last_bookmark_index = s_visit_bookmark_que_ptr->item_count;
    }
         
    //VP_STRACE_LOW:"[VP] VP_BookmarkList_AddItem index = %d, count = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_420_112_2_18_3_5_2_14,(uint8*)"dd", last_bookmark_index, s_visit_bookmark_que_ptr->item_count);
    if (VP_LIST_ERROR_NONE == add_result)
    {    
        //���ü�¼��ӵ���ǩ�б��� 
        SCI_MEMCPY(&(s_visit_bookmark_que_ptr->item_info[last_bookmark_index]),&bookmark_data,sizeof(VP_NET_MOVIE_INFO_T));
        s_visit_bookmark_que_ptr->item_count++;
        //����ǩ�б���Ϣд��Ĭ���ļ���
        add_result = SaveBookmarklistToFile();
        if(0 != add_result)
        {
            s_visit_bookmark_que_ptr->item_count--;
            SCI_MEMSET(&(s_visit_bookmark_que_ptr->item_info[last_bookmark_index]),0,sizeof(VP_NET_MOVIE_INFO_T));
        }
    }
    //VP_STRACE_LOW:"[VP] VP_BookmarkList_AddItem add_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_429_112_2_18_3_5_2_15,(uint8*)"d",add_result);
    return add_result;
}

/*****************************************************************************/
//  Description :�޸���ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_ModifyItem( 
                                                  uint16                  bookmark_list_index,
                                                  VP_NET_MOVIE_INFO_T     bookmark_data
                                               )
{   
   VP_LIST_ERROR_E     modify_result = VP_LIST_ERROR_OTHER;

   //VP_STRACE_LOW:"[VP] VP_BookmarkList_ModifyItem  index is worng list_index = %d "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_444_112_2_18_3_5_2_16,(uint8*)"d",bookmark_list_index);

    //�������
    if(PNULL != s_visit_bookmark_que_ptr && s_visit_bookmark_que_ptr->item_count > bookmark_list_index)
    {  
        //���ü�¼��ӵ���ǩ�б���  
         SCI_MEMCPY(&(s_visit_bookmark_que_ptr->item_info[bookmark_list_index]),&(bookmark_data),sizeof(VP_NET_MOVIE_INFO_T));
        //����ǩ�б���Ϣд��Ĭ���ļ���
         modify_result = SaveBookmarklistToFile();              
    }

    //VP_STRACE_LOW:"[VP] VP_BookmarkList_ModifyItem modify_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_BOOKMARKLIST_455_112_2_18_3_5_2_17,(uint8*)"d",modify_result);
    return modify_result;
}

/*****************************************************************************/
//  Description :��ȡԤ��CMCC��ý���Ż���ַ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NET_MOVIE_INFO_T VP_GetCMCCAddressInfo(void)
{
    VP_NET_MOVIE_INFO_T  net_movie_info = {0};
    MMI_STRING_T         string = {0};
    MMI_GetLabelTextByLang(TXT_VP_CMMC_URL_INFO,&string);//@andy.he  cr241051 20110627
    SCI_MEMSET(net_movie_info.title_arr,0,(VP_MAX_NAME_LEN+1));
    net_movie_info.title_len = MIN(string.wstr_len,VP_MAX_NAME_LEN);
    MMIAPICOM_Wstrncpy(net_movie_info.title_arr,string.wstr_ptr,net_movie_info.title_len);
    SCI_MEMSET(net_movie_info.url_ptr,0,(VP_MAX_URL_LEN+1));
    SCI_MEMCPY(net_movie_info.url_ptr,VP_CMCC_URL,MIN(VP_CMCC_URL_LEN,VP_MAX_URL_LEN));

    return net_movie_info;
}
#endif


