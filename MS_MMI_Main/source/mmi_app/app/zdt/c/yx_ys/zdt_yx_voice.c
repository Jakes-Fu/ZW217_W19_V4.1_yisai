/*
云芯平台
*/
#include "zdt_app.h"
#include "mmisrvrecord_export.h"

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#define YX_VOICE_RECORD_TIME_OUT 15

static BOOLEAN s_voc_is_play_all = FALSE;
static uint8 s_voc_play_err_num = 0;
static BOOLEAN           s_yx_voc_is_playing = FALSE;
static BOOLEAN           s_yx_voc_is_recording = FALSE;
static uint16 s_yx_voc_play_vol = 9;
static uint16 s_yx_voc_rcv_idx = 0;

BOOLEAN YX_Voice_Play_Valid(void);
#if 1
YX_GROUP_INFO_DATA_T m_vchat_all_group_info_arr[YX_DB_FRIEND_MAX_SUM+1] = {0};
YX_GROUP_INFO_DATA_T * m_pCurGroupInfo = &m_vchat_all_group_info_arr[0];
YX_GROUP_INFO_DATA_T * m_pCurRcvGroupInfo = &m_vchat_all_group_info_arr[0];
uint16 m_vchat_all_group_info_index[YX_DB_FRIEND_MAX_SUM+1] = {0};
uint16 m_vchat_all_group_sum = 0;

uint32 YX_VCHAT_UTF8_To_GB(uint8 * utf8_str,uint8 * gb_str)
{
    uint16 * ucs2_buf = PNULL;
    uint32 utf8_len = 0;
    uint32 ucs2_len = 0;
    uint32 gbk_len = 0;
    if(utf8_str == PNULL || gb_str == PNULL)
    {
        return 0;
    }
    utf8_len = strlen(utf8_str);
    if(utf8_len > 0)
    {
        ucs2_buf = (uint16 *)SCI_ALLOC_APPZ(utf8_len*2);
        if(ucs2_buf != PNULL)
        {
    #ifdef WIN32
            SCI_MEMCPY(gb_str,utf8_str,utf8_len);
            gbk_len = utf8_len;
            //gbk_len = ucs2str_to_gb2312str((uint8 *)ucs2_buf, ucs2_len*2,gb_str);
    #else
            ucs2_len = GUI_UTF8ToWstr(ucs2_buf, utf8_len, utf8_str, utf8_len);
            gbk_len = ucs2str_to_gb2312strExt((uint8 *)ucs2_buf, ucs2_len*2,gb_str,0);
    #endif
            SCI_FREE(ucs2_buf);
        }
    }
    return gbk_len;
}

static uint8 * YX_VCHAT_GetGroup_Patch(uint8 * utf8_group_id)
{
    uint32 utf8_len = 0;
    uint32 pach_len = 0;
    uint32 fullname_len = 0;
    uint8 * gb_buf = PNULL;
    uint32 gb_len = 0;
    uint8 * gb_patch_buf = PNULL;
    if(utf8_group_id == PNULL)
    {
        return PNULL;
    }
    pach_len = SCI_STRLEN(YX_VCHAT_DIR);
    utf8_len = SCI_STRLEN(utf8_group_id);
    gb_buf = (uint8 *)SCI_ALLOC_APPZ(utf8_len+2);
    if(gb_buf == PNULL)
    {
        return PNULL;
    }        
    gb_len=YX_VCHAT_UTF8_To_GB(utf8_group_id,gb_buf);
    fullname_len = pach_len+gb_len;
    gb_patch_buf = (uint8 *)SCI_ALLOC_APPZ(fullname_len+1);
    if(gb_patch_buf != PNULL)
    {
        SCI_MEMCPY(gb_patch_buf,YX_VCHAT_DIR,pach_len);
        SCI_MEMCPY(gb_patch_buf+pach_len,gb_buf,gb_len);
    }
    SCI_FREE(gb_buf);
    return gb_patch_buf;
}

static uint8 * YX_VCHAT_GetGroup_FullFileName(uint8 * utf8_group_id)
{
    uint32 utf8_len = 0;
    uint32 pach_len = 0;
    uint32 file_len = 0;
    uint32 fullname_len = 0;
    uint8 * gb_buf = PNULL;
    uint32 gb_len = 0;
    uint8 * gb_patch_buf = PNULL;
    if(utf8_group_id == PNULL)
    {
        return PNULL;
    }
    
    pach_len = SCI_STRLEN(YX_VCHAT_DIR);
    file_len = SCI_STRLEN(YX_VCHAT_FILENAME);
    utf8_len = SCI_STRLEN(utf8_group_id);
    gb_buf = (uint8 *)SCI_ALLOC_APPZ(utf8_len+2);
    if(gb_buf == PNULL)
    {
        return PNULL;
    }        
    gb_len=YX_VCHAT_UTF8_To_GB(utf8_group_id,gb_buf);
    fullname_len = pach_len+gb_len+file_len;
    gb_patch_buf = (uint8 *)SCI_ALLOC_APPZ(fullname_len+2);
    if(gb_patch_buf != PNULL)
    {
        SCI_MEMCPY(gb_patch_buf,YX_VCHAT_DIR,pach_len);
        SCI_MEMCPY(gb_patch_buf+pach_len,gb_buf,gb_len);
        gb_patch_buf[pach_len+gb_len] = '\\';
        SCI_MEMCPY(gb_patch_buf+pach_len+gb_len+1,YX_VCHAT_FILENAME,file_len);
    }
    SCI_FREE(gb_buf);
    return gb_patch_buf;
}

static uint8 * YX_VCHAT_GetGroup_FullFileStatus(uint8 * utf8_group_id)
{
    uint32 utf8_len = 0;
    uint32 pach_len = 0;
    uint32 file_len = 0;
    uint32 fullname_len = 0;
    uint8 * gb_buf = PNULL;
    uint32 gb_len = 0;
    uint8 * gb_patch_buf = PNULL;
    if(utf8_group_id == PNULL)
    {
        return PNULL;
    }
    
    pach_len = SCI_STRLEN(YX_VCHAT_DIR);
    file_len = SCI_STRLEN(YX_VCHAT_FILESTATUS);
    utf8_len = SCI_STRLEN(utf8_group_id);
    gb_buf = (uint8 *)SCI_ALLOC_APPZ(utf8_len+2);
    if(gb_buf == PNULL)
    {
        return PNULL;
    }        
    gb_len=YX_VCHAT_UTF8_To_GB(utf8_group_id,gb_buf);
    fullname_len = pach_len+gb_len+file_len;
    gb_patch_buf = (uint8 *)SCI_ALLOC_APPZ(fullname_len+2);
    if(gb_patch_buf != PNULL)
    {
        SCI_MEMCPY(gb_patch_buf,YX_VCHAT_DIR,pach_len);
        SCI_MEMCPY(gb_patch_buf+pach_len,gb_buf,gb_len);
        gb_patch_buf[pach_len+gb_len] = '\\';
        SCI_MEMCPY(gb_patch_buf+pach_len+gb_len+1,YX_VCHAT_FILESTATUS,file_len);
    }
    SCI_FREE(gb_buf);
    return gb_patch_buf;
}

PUBLIC void YX_VCHAT_GetCurGroupInfo_File(uint8 * file_fullname,YX_GROUP_INFO_DATA_T * p_group_info)
{
    uint32 read_len = 0;
    uint8 buffer[MAX_YX_VOC_SAVE_SIZE*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint16 line_index = 0;
    uint16 i = 0, j = 0 ;
    uint16 k = 0;
    uint16 cr_index = 0;
    if(file_fullname == PNULL || p_group_info == PNULL)
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_File  ERR NULL ");
        return;
    }
    SCI_MEMSET(p_group_info->filename.fullname,0,MAX_YX_VOC_GROUP_FULL_PATH_SIZE+1);
    SCI_MEMCPY(p_group_info->filename.fullname,file_fullname,SCI_STRLEN(file_fullname));
    p_group_info->file_num = 0;
    SCI_MEMSET(p_group_info->file_arr,0,sizeof(YX_GROUP_FILENAME_DATA_T)*MAX_YX_VOC_SAVE_SIZE);
    if(ZDT_File_Read((const uint8*)file_fullname , buffer, MAX_YX_VOC_SAVE_SIZE*MAX_YX_VOC_GROUP_FULL_PATH_SIZE , &read_len))
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_File success read_len=%d",read_len);
    }
    else
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_File ERR ZDT_File_Read");
        return;
    }
    
    if(buffer && SCI_STRLEN(buffer) > 0)
    {
        for(i = 0 ; i < SCI_STRLEN(buffer) ; i++)
        {
            if(buffer[i] != '\n')
            {
                p_group_info->file_arr[line_index].fullname[j++] = buffer[i];
            }
            else
            {
                p_group_info->file_arr[line_index].fullname[j] = '\0';
                j = 0;
                cr_index = i;
                if(ZDT_File_Exsit(p_group_info->file_arr[line_index].fullname))
                {
                    line_index++;
                }
            }
        }
        p_group_info->file_num = line_index;
    }
}

PUBLIC void YX_VCHAT_GetCurGroupInfo_Status(uint8 * status_fullname,YX_GROUP_INFO_DATA_T * p_group_info)
{
    uint32 read_len = 0;

    if(status_fullname == PNULL || p_group_info == PNULL)
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_Status  ERR NULL ");
        return;
    }
    SCI_MEMSET(p_group_info->status_name.fullname,0,MAX_YX_VOC_GROUP_FULL_PATH_SIZE+1);
    SCI_MEMCPY(p_group_info->status_name.fullname,status_fullname,SCI_STRLEN(status_fullname));
    SCI_MEMSET(p_group_info->status_arr,0,MAX_YX_VOC_SAVE_SIZE+1);
    if(ZDT_File_Read((const uint8*)status_fullname , p_group_info->status_arr, MAX_YX_VOC_SAVE_SIZE+1 , &read_len))
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_Status success read_len=%d",read_len);
    }
    else
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_Status ERR ZDT_File_Read");
        return;
    }
}

BOOLEAN YX_VCHAT_GetCurGroupInfo(uint8 * utf8_group_id,YX_GROUP_INFO_DATA_T * group_info)
{
    BOOLEAN res = FALSE;
    uint8 * patchname = PNULL;
    uint8 * filename = PNULL;
    uint8 * statusname = PNULL;
    
    if(utf8_group_id == PNULL || group_info == PNULL)
    {
        return FALSE;
    }
    SCI_MEMSET(group_info->group_id,0,YX_DB_FRIEND_MAX_ID_SIZE+1);
    SCI_MEMCPY(group_info->group_id,utf8_group_id,SCI_STRLEN(utf8_group_id));
    patchname = YX_VCHAT_GetGroup_Patch(utf8_group_id);
    if(patchname != PNULL)
    {
        SCI_MEMSET(group_info->patchname.fullname,0,MAX_YX_VOC_GROUP_FULL_PATH_SIZE+1);
        SCI_MEMCPY(group_info->patchname.fullname,patchname,SCI_STRLEN(patchname));
        filename = YX_VCHAT_GetGroup_FullFileName(utf8_group_id);
        if(filename != PNULL)
        {
            res = TRUE;
            YX_VCHAT_GetCurGroupInfo_File(filename,group_info);
            statusname = YX_VCHAT_GetGroup_FullFileStatus(utf8_group_id);
            if(statusname != PNULL)
            {
                YX_VCHAT_GetCurGroupInfo_Status(statusname,group_info);
                SCI_FREE(statusname);
            }
            SCI_FREE(filename);
        }
        SCI_FREE(patchname);
    }
    return res;
}

BOOLEAN YX_VCHAT_GetAllGroupInfo(void)
{
    uint16 i = 0;
    uint16 j = 0;
    SCI_MEMSET(m_vchat_all_group_info_arr,0,sizeof(YX_GROUP_INFO_DATA_T)*(YX_DB_FRIEND_MAX_SUM+1));
    YX_VCHAT_GetCurGroupInfo(YX_VCHAT_DEFAULT_GROUP_ID,&m_vchat_all_group_info_arr[j]);
    SCI_MEMCPY(m_vchat_all_group_info_arr[j].group_name,"5BB65EAD7FA4804A",SCI_STRLEN("5BB65EAD7FA4804A"));
    j++;
    for ( i = 0; i < YX_DB_FRIEND_MAX_SUM; i++ )
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0)
        {
            YX_VCHAT_GetCurGroupInfo(yx_DB_Friend_Reclist[i].friend_id,&m_vchat_all_group_info_arr[j]);
            SCI_MEMCPY(m_vchat_all_group_info_arr[j].group_name,yx_DB_Friend_Reclist[i].friend_name,SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_name));
            SCI_MEMCPY(m_vchat_all_group_info_arr[j].group_num,yx_DB_Friend_Reclist[i].friend_num,SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_num));
            m_vchat_all_group_info_index[j] = i+1;
            j++;
        }
    }
    m_vchat_all_group_sum = j;
    //m_pCurGroupInfo = &m_vchat_all_group_info_arr[0];
    return TRUE;
}

YX_GROUP_INFO_DATA_T * YX_VCHAT_GetGroupInfoByFileName(uint8 * full_name)
{
    uint32 len = 0;
    char* p;
    char* p1;
    char* p2;
    uint16 i = 0;
    uint8 group_id[YX_DB_FRIEND_MAX_ID_SIZE+1] = {0};
    if(full_name != PNULL)
    {
        p = strchr(full_name, '\\');
        if(p == NULL)
        {
            return PNULL;
        }
        p = p+1;
        if(p != PNULL)
        {
            p1 = strchr(p, '\\');
            if(p1 != PNULL)
            {
                p2 = strrchr(full_name, '\\');
                if(p2 != PNULL && p2 > p1)
                {
                    len = p2-p1-1;
                    strncpy(group_id,p1+1,len);
                }
            }
        }        
        for(i = 0; i < m_vchat_all_group_sum; i++)
        {
            if(strcmp(m_vchat_all_group_info_arr[i].group_id,group_id) == 0)
            {
                return &m_vchat_all_group_info_arr[i];
            }
        }
    }
    return PNULL;
}

PUBLIC void YX_VCHAT_UpdateGroupInfoByFileBuf(YX_GROUP_INFO_DATA_T * p_group_info,uint8 * buffer,uint32 buf_len)
{
    uint16 line_index = 0;
    uint16 i = 0, j = 0 ;
    uint16 k = 0;
    uint16 cr_index = 0;
    if(p_group_info == PNULL || buffer == PNULL)
    {
        ZDT_LOG("YX_VCHAT_GetCurGroupInfo_File  ERR NULL ");
        return;
    }
    p_group_info->file_num = 0;
    SCI_MEMSET(p_group_info->file_arr,0,sizeof(YX_GROUP_FILENAME_DATA_T)*MAX_YX_VOC_SAVE_SIZE);
    
    if(buffer && buf_len > 0)
    {
        for(i = 0 ; i < buf_len ; i++)
        {
            if(buffer[i] != '\n')
            {
                p_group_info->file_arr[line_index].fullname[j++] = buffer[i];
            }
            else
            {
                p_group_info->file_arr[line_index].fullname[j] = '\0';
                j = 0;
                cr_index = i;
                if(ZDT_File_Exsit(p_group_info->file_arr[line_index].fullname))
                {
                    line_index++;
                }
            }
        }
        p_group_info->file_num = line_index;
    }
}

BOOLEAN YX_VCHAT_UpdateGroupInfoByFileName(uint8 * full_name,uint8 * buffer,uint32 buf_len)
{
    YX_GROUP_INFO_DATA_T * pInfo =  YX_VCHAT_GetGroupInfoByFileName(full_name);
    if(pInfo != PNULL)
    {
        YX_VCHAT_UpdateGroupInfoByFileBuf(pInfo,buffer,buf_len);
    }
    return TRUE;
}

uint32 YX_VCHAT_GetGroupUnread(YX_GROUP_INFO_DATA_T * group_info)
{
    uint32 i = 0, j = 0 ;
    uint32 unread_sum = 0;
    if(group_info == PNULL)
    {
        return 0;
    }
    if(group_info->file_num == 0)
    {
        return 0;
    }
    for(i = 0; i < group_info->file_num; i ++)
    {
        if(group_info->status_arr[i] != 0x31)
        {
            unread_sum++;
        }
    }
    return unread_sum;
}    

static BOOLEAN YX_VCHAT_Send_GetFileName(YX_APP_T * pMe)
{
    uint32 time_ms = ZDT_Get_TimeSecond();
    
    SCI_MEMSET(pMe->m_yx_pFileSendVocName, 0, MAX_YX_VOC_FILENAME_SIZE+1);
    sprintf( pMe->m_yx_pFileSendVocName,
             "%s\\S%d_%ld.amr",
             m_pCurGroupInfo->patchname.fullname,pMe->m_yx_voc_data_idx+1,time_ms);
    ZDT_LOG("YX_Voice_Send_BuildFileName %s",pMe->m_yx_pFileSendVocName);
    SCI_MEMSET(pMe->m_yx_pFileSendVocFullName, 0, (MAX_YX_VOC_FILENAME_SIZE+1)*2);
    pMe->m_yx_pFileSendVocFullName_len = GUI_GBToWstr(pMe->m_yx_pFileSendVocFullName, (const uint8*)pMe->m_yx_pFileSendVocName, SCI_STRLEN(pMe->m_yx_pFileSendVocName));

    ZDT_File_Remove(pMe->m_yx_pFileSendVocName);
    return TRUE;
}

static BOOLEAN YX_VCHAT_RCV_GetFileName(YX_APP_T * pMe,uint16 voc_idx,uint8 * utf8_group_id)
{
    uint32 time_ms = ZDT_Get_TimeSecond();
    uint8 * rcv_group_patch = YX_VCHAT_GetGroup_Patch(utf8_group_id);

    if(rcv_group_patch != PNULL)
    {
        ZDT_LOG("YX_VCHAT_RCV_GetFileName voc_idx=%d,time=%ld,%s",voc_idx,time_ms,rcv_group_patch);
        SCI_MEMSET(pMe->m_yx_pFileRcvVocName, 0, MAX_YX_VOC_FILENAME_SIZE+1);

        sprintf( pMe->m_yx_pFileRcvVocName,
                 "%s\\R%d_%ld.amr",
                 rcv_group_patch,voc_idx,time_ms);
        return TRUE;
    }
    return FALSE;
}

uint32 YX_VCHAT_GetGroupIDByFileName(uint8 * full_name, uint8 * group_id)
{
    uint32 len = 0;
    char* p;
    char* p1;
    char* p2;
    if(full_name != PNULL)
    {
        p = strchr(full_name, '\\');
        if(p == NULL)
        {
            return FALSE;
        }
        p = p+1;
        if(p != PNULL)
        {
            p1 = strchr(p, '\\');
            if(p1 != PNULL)
            {
                p2 = strrchr(full_name, '\\');
                if(p2 != PNULL && p2 > p1)
                {
                    len = p2-p1-1;
                    strncpy(group_id,p1+1,len);
                }
            }
        }        
    }
    return len;
}

BOOLEAN YX_VCHAT_DeleteGroupFile(uint8 * utf8_group_id)
{
    BOOLEAN res = FALSE;
    uint8 * patchname = PNULL;
    
    if(utf8_group_id == PNULL)
    {
        return FALSE;
    }
    patchname = YX_VCHAT_GetGroup_Patch(utf8_group_id);
    if(patchname != PNULL)
    {
        ZDT_File_RemoveDirFile(patchname);
        ZDT_Dir_Remove(patchname);
        SCI_FREE(patchname);
        res = TRUE;
    }
    return res;
}

// 检查并删除不存在的用户
BOOLEAN YX_VCHAT_CheckOneGroupUser(uint8 * utf8_group_id)
{
    BOOLEAN res = FALSE;
    uint16 i = 0;
    if(utf8_group_id == PNULL)
    {
        return FALSE;
    }
    for ( i = 0; i < YX_DB_FRIEND_MAX_SUM; i++ )
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0)
        {
            if(strcmp(yx_DB_Friend_Reclist[i].friend_id,utf8_group_id) == 0)
            {
                res = TRUE;
                break;
            }
        }
    }
    if(res == FALSE)
    {
        YX_VCHAT_DeleteGroupFile(utf8_group_id);
    }
    return res;
}

BOOLEAN YX_VCHAT_CheckAllGroupUser(void)
{
    uint16 i = 0;
    for ( i = 1; i < m_vchat_all_group_sum; i++ )
    {
        if(SCI_STRLEN(m_vchat_all_group_info_arr[i].group_id) > 0)
        {
            YX_VCHAT_CheckOneGroupUser(m_vchat_all_group_info_arr[i].group_id);
        }
    }
    return TRUE;
}


#endif

static uint8 YX_Voice_GetMemStatus(YX_APP_T * pMe)
{
    BOOLEAN is_full = ZDT_File_MemFull(FALSE);
    if(is_full)
    {
        return 0;
    }
    return 1;
}

BOOLEAN YX_Voice_Allow_Receive(YX_APP_T * pMe)
{
    BOOLEAN is_full = ZDT_File_MemFull(FALSE);
    if(is_full)
    {
        return FALSE;
    }
    return TRUE;
}

LOCAL void YX_VocFilePathSaveFullBufToTxt(uint8 * full_filename,char* full_data, uint32 data_len);
PUBLIC void YX_VocFilePathTxtDel()
{
    ZDT_File_Remove((const char *)m_pCurGroupInfo->filename.fullname);
    ZDT_File_Remove((const char *)m_pCurGroupInfo->status_name.fullname);
}

PUBLIC void YX_VocFileCheckAndRemoveOverItem(uint8 * full_filename)
{
    uint32 read_len = 0;
    uint8 buffer[(MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint8 buffer2[(MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    char line_data[(MAX_YX_VOC_SAVE_SIZE + 10)][MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint8 valid_line[(MAX_YX_VOC_SAVE_SIZE + 10)]= {0};
    uint16 valid_line_count = 0;
    uint16 line_index = 0;
    uint32 i = 0, j = 0 ;
    uint16 cr_index = 0;
    uint16 del_item_count = 0;
    char file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_len = 0;
    char* p;

    if(full_filename == PNULL)
    {
        ZDT_LOG("YX_VocFileCheckAndRemoveOverItem Err group Name");
        return;
    }
    ZDT_LOG("YX_VocFileCheckAndRemoveOverItem %s",full_filename);
    p = strrchr(full_filename,'\\');
    if(p == PNULL)
    {
        ZDT_LOG("YX_VocFileCheckAndRemoveOverItem Err group Name");
        return;
    }
    file_len = p-full_filename+1;
    strncpy(file_name,full_filename,file_len);
    strcat(file_name,YX_VCHAT_FILENAME);

    if(ZDT_File_Read((const uint8*)file_name , buffer, (MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE, &read_len))
    {
        ZDT_LOG("YX_VocFileCheckAndRemoveOverItem success buffer=%s", buffer);
    }
    
    if(buffer && SCI_STRLEN(buffer) > 0)
    {
        for(i = 0 ; i < SCI_STRLEN(buffer) ; i++)
        {
            if(buffer[i] != '\n')
            {
                line_data[line_index][j++] = buffer[i];
            }
            else
            {
                line_data[line_index][j] = '\0';
                j = 0;
                line_index++;
                cr_index = i;
                
            }
            
        }
        
        //SCI_MEMCPY(ui_line_data , line_data, 20*50*sizeof(char));
        j = 0;
        for(i = 0 ; i < (MAX_YX_VOC_SAVE_SIZE + 10) ; i++) // total will only save 20 lines, over 20 will delete
        {
            if(line_data[i] && SCI_STRLEN(line_data[i]) > 0 && ZDT_File_Exsit(line_data[i]))
            {
                valid_line_count++;
                valid_line[j++] = i;

            }
            
        }

        if(valid_line_count > MAX_YX_VOC_SAVE_SIZE)
        {
            del_item_count = valid_line_count - MAX_YX_VOC_SAVE_SIZE;
            for( i = 0; i < del_item_count ; i++)
            {
                ZDT_File_Remove(line_data[valid_line[i]]);
            }
            j = 0;
            for(i = del_item_count; i < valid_line_count ; i++)
            {
                 SCI_MEMCPY(buffer2 + j, line_data[valid_line[i]], SCI_STRLEN(line_data[valid_line[i]]));
                 j += SCI_STRLEN(line_data[valid_line[i]]);
                 buffer2[j] = '\n'; //next line
                 j += 1;
            }
            YX_VocFilePathSaveFullBufToTxt(full_filename,buffer2, j);
            YX_VCHAT_UpdateGroupInfoByFileName(full_filename,buffer2,j);
        }
    }
}

PUBLIC void YX_VocFileRemoveOneItem(uint16 idx)
{
    uint32 read_len = 0;
    uint8 buffer[(MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint8 buffer2[(MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    char line_data[(MAX_YX_VOC_SAVE_SIZE + 10)][MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint16 line_index = 0;
    uint32 i = 0, j = 0 ;
    uint16 cr_index = 0;
    uint16 len = 0;
    
    if(ZDT_File_Read((const uint8*)m_pCurGroupInfo->filename.fullname, buffer, (MAX_YX_VOC_SAVE_SIZE + 10)*MAX_YX_VOC_GROUP_FULL_PATH_SIZE, &read_len))
    {
        ZDT_LOG("YX_VocFileRemoveOneItem success buffer=%s", buffer);
    }

    
    if(buffer && SCI_STRLEN(buffer) > 0)
    {
        for(i = 0 ; i < SCI_STRLEN(buffer) ; i++)
        {
            if(buffer[i] != '\n')
            {
                line_data[line_index][j++] = buffer[i];
            }
            else
            {
                line_data[line_index][j] = '\0';
                j = 0;
                if(ZDT_File_Exsit(line_data[line_index]))
                {
                    line_index++;
                }
                cr_index = i;
                
            }
            
        }
        if(idx < line_index)
        {
            j = 0;
            ZDT_File_Remove(line_data[idx]);
            for( i = 0; i < idx ; i++)
            {   
                len = SCI_STRLEN(line_data[i]);
                SCI_MEMCPY(buffer2 + j, line_data[i], len);
                j += len;
                buffer2[j++] = '\n';
            }
            for(i = (idx+1); i < line_index ; i++)
            {
                len = SCI_STRLEN(line_data[i]);
                SCI_MEMCPY(buffer2 + j, line_data[i], len);
                j += len;
                buffer2[j++] = '\n';
            }
            if(j == 0)
            {
                ZDT_File_Remove((const char *)m_pCurGroupInfo->filename.fullname);
                m_pCurGroupInfo->file_num = 0;
                SCI_MEMSET(m_pCurGroupInfo->file_arr,0,sizeof(YX_GROUP_FILENAME_DATA_T)*MAX_YX_VOC_SAVE_SIZE);
                SCI_MEMSET(m_pCurGroupInfo->status_arr,0,MAX_YX_VOC_SAVE_SIZE+1);
            }
            else
            {
                uint8  status_arr[MAX_YX_VOC_SAVE_SIZE+1] = {0};
                if(idx != 0)
                {
                    SCI_MEMCPY(status_arr,m_pCurGroupInfo->status_arr,idx);
                }
                SCI_MEMCPY(status_arr+idx,m_pCurGroupInfo->status_arr+idx+1,MAX_YX_VOC_SAVE_SIZE-idx);
                YX_VocFilePathSaveFullBufToTxt(m_pCurGroupInfo->filename.fullname,buffer2, j);
                //YX_VocFileStatusWrite(status_arr);
                SCI_MEMSET(m_pCurGroupInfo->status_arr,0,MAX_YX_VOC_SAVE_SIZE+1);
                SCI_MEMCPY(m_pCurGroupInfo->status_arr,status_arr,MAX_YX_VOC_SAVE_SIZE+1);
                YX_VCHAT_UpdateGroupInfoByFileBuf(m_pCurGroupInfo,buffer2,j);
            }
            
        }
        
        
    }

}
LOCAL void YX_VocFilePathSaveFullBufToTxt(uint8 * full_filename,char* full_data, uint32 data_len)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16  tmp_len = 0;
    BOOLEAN ret = FALSE;
    MMIFILE_HANDLE file_handle;
    //uint8 data_save[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    char file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_len = 0;
    char* p;
    
    if(full_data == NULL || data_len == 0)
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt error");
        return;
    }

    if(full_filename == PNULL)
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt Err group Name");
        return;
    }
    p = strrchr(full_filename,'\\');
    if(p == PNULL)
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt Err group Name");
        return;
    }
    file_len = p-full_filename+1;
    strncpy(file_name,full_filename,file_len);
    strcat(file_name,YX_VCHAT_FILENAME);

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
       ZDT_File_Remove((const uint8*)file_name); 
    }  
    file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);

    //file_handle = ZDT_File_Open(full_path, FALSE);
    if(file_handle == SFS_INVALID_HANDLE)
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt create file failed");
        return ;
    }

    if(ZDT_File_Write(file_handle, full_data, data_len)==0)
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt file handle error AEE_ENOMEMORY");
        ZDT_File_Close(file_handle);
        return ;
    }
    else
    {
        ZDT_LOG("YX_VocFilePathSaveFullBufToTxt success");
        //ZDT_File_Write(file_handle, "\n", SCI_STRLEN("\n"));// next line

    }
    ZDT_File_Close(file_handle);
}

LOCAL void YX_PlayRcvVoiceMsgTone()
{
    if(!MMIZDT_IsInTinyChatWin())
    {
        MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE, 0, 1, MMISET_RING_TYPE_MSG, PNULL);
    }
}


LOCAL void YX_VocFilePathSaveToTxt(char* data_full_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16  tmp_len = 0;
    char file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_len = 0;
    char* p;
    BOOLEAN ret = FALSE;
    MMIFILE_HANDLE file_handle;
    YX_GROUP_INFO_DATA_T * pInfo =  PNULL;

    //uint8 data_save[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    
    if(data_full_name == NULL || SCI_STRLEN(data_full_name) == 0)
    {
        ZDT_LOG("YX_VocFilePathSaveToTxt error");
        return;
    }
    ZDT_LOG("YX_VocFilePathSaveToTxt %s",data_full_name);
    p = strrchr(data_full_name,'\\');
    if(p == PNULL)
    {
        ZDT_LOG("YX_VocFilePathSaveToTxt Err group Name");
        return;
    }
    tmp_len = p-data_full_name+1;
    strncpy(file_name,data_full_name,tmp_len);
    strcat(file_name,YX_VCHAT_FILENAME);
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
    if(!MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);
    }
    

    if(file_handle == SFS_INVALID_HANDLE)
    {
        ZDT_LOG("YX_VocFilePathSaveToTxt create file failed");
        return ;
    }

    if(ZDT_File_Write(file_handle, data_full_name, SCI_STRLEN(data_full_name))==0)
    {
        ZDT_LOG("YX_VocFilePathSaveToTxt file handle error AEE_ENOMEMORY");
        ZDT_File_Close(file_handle);
        return ;
    }
    else
    {
        ZDT_LOG("YX_VocFilePathSaveToTxt success");
        ZDT_File_Write(file_handle, "\n", SCI_STRLEN("\n"));// next line

    }
    ZDT_File_Close(file_handle);
    
    pInfo =  YX_VCHAT_GetGroupInfoByFileName(data_full_name);
    if(pInfo != PNULL && pInfo->file_num < MAX_YX_VOC_SAVE_SIZE)
    {
        SCI_MEMSET(pInfo->file_arr[pInfo->file_num].fullname,0,MAX_YX_VOC_GROUP_FULL_PATH_SIZE+1);
        SCI_MEMCPY(pInfo->file_arr[pInfo->file_num].fullname,data_full_name,SCI_STRLEN(data_full_name));
        if(YX_VOC_IsRcvFile(data_full_name))
        {
            pInfo->status_arr[pInfo->file_num] = 0;
        }
        else
        {
            pInfo->status_arr[pInfo->file_num] = 0x31; //0x31 49 ascii 字符1 啥玩意 直接=1不行？
        }
        pInfo->file_num++;
    }
    YX_VocFileCheckAndRemoveOverItem(data_full_name);
    MMIZDT_TinyChatRefreshWin();

}
#ifdef WIN32
BOOLEAN YX_DB_Voc_Init(YX_APP_T* pMe)
{
    uint32 time_ms = ZDT_Get_TimeSecond();
    char full_path_name[100] = {0};

    sprintf( full_path_name,"D:\\vChat\\222\\R1_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"111",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
    
    sprintf( full_path_name,"D:\\vChat\\common\\R2_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"222",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
    
    sprintf( full_path_name,"D:\\vChat\\common\\S1_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"111",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
    
    sprintf( full_path_name,"D:\\vChat\\common\\S2_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"222",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
     sprintf( full_path_name,"D:\\vChat\\222\\S3_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"333",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
    sprintf( full_path_name,"D:\\vChat\\common\\S4_%ld.amr",time_ms);
    ZDT_File_Remove(full_path_name);
    ZDT_File_Save(full_path_name,"444",3,FALSE);
    YX_VocFilePathSaveToTxt(full_path_name);
    time_ms++;
  return TRUE;
}
#endif

extern void YX_VocFileStatusWrite(char *ui_line_status)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16  tmp_len = 0;

    MMIFILE_HANDLE file_handle=0;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)m_pCurGroupInfo->status_name.fullname, SCI_STRLEN(m_pCurGroupInfo->status_name.fullname));
    full_path_len = tmp_len;    
    
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
       ZDT_File_Remove((const uint8*)full_path); 
    }  
    file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);

    if(file_handle == SFS_INVALID_HANDLE)
    {
        ZDT_LOG("YX_VocFileStatusWrite create file failed");
        return ;
    }
    if(ZDT_File_Write(file_handle, ui_line_status, MAX_YX_VOC_SAVE_SIZE+1))
    {
        ZDT_LOG("YX_VocFileStatusWrite success ui_line_status=%s",ui_line_status);
    }
    else
    {
        ZDT_LOG("YX_VocFileStatusWrite ERR");
    }
    ZDT_File_Close(file_handle);
}

PUBLIC uint32 YX_VocReadStatusCheck(uint8 * utf8_group_id)
{
    uint32 read_len = 0;
    uint32 unread_sum = 0;
    uint8 buffer[MAX_YX_VOC_SAVE_SIZE*MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    char line_data[MAX_YX_VOC_SAVE_SIZE][MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint16 line_index = 0;
    uint16 i = 0, j = 0 ;
    uint16 cr_index = 0;
    uint8 * cur_filegroup = YX_VCHAT_GetGroup_FullFileName(utf8_group_id);
    uint8 * cur_filestatus = PNULL;
    char   cur_line_status[MAX_YX_VOC_SAVE_SIZE+1] = {0};
    
    if(cur_filegroup == PNULL)
    {
        return 0;
    }
    if(FALSE == ZDT_File_Read((const uint8*)cur_filegroup , buffer, MAX_YX_VOC_SAVE_SIZE*MAX_YX_VOC_GROUP_FULL_PATH_SIZE , &read_len))
    {
        SCI_FREE(cur_filegroup);
        return 0;
    }

    
    if(buffer && SCI_STRLEN(buffer) > 0)
    {
        for(i = 0 ; i < SCI_STRLEN(buffer) ; i++)
        {
            if(buffer[i] != '\n')
            {
                line_data[line_index][j++] = buffer[i];
            }
            else
            {
                line_data[line_index][j] = '\0';
                j = 0;
                cr_index = i;
                if(ZDT_File_Exsit(line_data[line_index]))
                {
                    line_index++;
                }
            }
        }
        if(line_index > 0)
        {
            cur_filestatus = YX_VCHAT_GetGroup_FullFileStatus(utf8_group_id);
            if(cur_filestatus == PNULL)
            {
                SCI_FREE(cur_filegroup);
                return 0;
            }
            if(ZDT_File_Read((const uint8*)cur_filestatus , cur_line_status, MAX_YX_VOC_SAVE_SIZE+1 , &read_len))
            {
                for(i = 0 ; i < line_index ; i++)
                {
                    if(cur_line_status[i] != 0x31)
                    {
                        unread_sum++;
                    }
                    
                }
            }
            SCI_FREE(cur_filestatus);
        }
    }
    SCI_FREE(cur_filestatus);
    return unread_sum;
}

PUBLIC BOOLEAN YX_VOC_IsSendFile(char* full_name)
{
    BOOLEAN res = FALSE;
    char* p;
    char szFileName[100] = {0};

    if(full_name != PNULL)
    {
        p = strrchr(full_name, '\\');
        if(p == NULL)
        {
            ZDT_LOG("YX_VOC_IsSendFile Err 1");
            return FALSE;
        }
        strcpy(szFileName,p+1);
        if(szFileName[0] == 'S')
        {
            res = TRUE;
        }
    }
    return res;
}

PUBLIC BOOLEAN YX_VOC_IsRcvFile(char* full_name)
{
    BOOLEAN res = FALSE;
    char* p;
    char szFileName[100] = {0};

    if(full_name != PNULL)
    {
        p = strrchr(full_name, '\\');
        if(p == NULL)
        {
            ZDT_LOG("YX_VOC_IsRcvFile Err 1");
            return FALSE;
        }
        strcpy(szFileName,p+1);
        if(szFileName[0] == 'R')
        {
            res = TRUE;
        }
    }
    return res;
}

BOOLEAN YX_Voice_GroupRemoveAll(YX_APP_T * pMe,uint8 * utf8_group_id)
{
    if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_STOP
        && pMe->m_yx_voc_is_sending == FALSE
        )
    {
        uint8 * cur_group_patch = YX_VCHAT_GetGroup_Patch(utf8_group_id);
        if(cur_group_patch != PNULL)
        {
            ZDT_File_RemoveDirFile(cur_group_patch);
            ZDT_Dir_Remove(cur_group_patch);
            SCI_FREE(cur_group_patch);
        }
    }
    return TRUE;
}

static BOOLEAN YX_VOC_Send_Link_Add(YX_APP_T *pMe, uint8 * filename, uint32 name_len)
{
    YX_VOC_SEND_NODE_T  * p1 = NULL;
    YX_VOC_SEND_NODE_T  * p2 = NULL;
    uint32 len = name_len;

    if(len == 0)
    {
        return FALSE;
    }
    
    p1=(YX_VOC_SEND_NODE_T *)SCI_ALLOC_APPZ(sizeof(YX_VOC_SEND_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    p1->data.pFilename = (uint8 *)SCI_ALLOC_APPZ(len+1);
    if(p1->data.pFilename == NULL)
    {
        SCI_FREE(p1);
        return FALSE;
    }
    
    SCI_MEMCPY(p1->data.pFilename,filename,name_len);
    p1->data.pFilename[len] = 0;

    p1->data.len = len;
    p1->next_ptr = NULL;

    if(pMe->m_yx_voc_send_link_head == NULL)
    {
        pMe->m_yx_voc_send_link_head = p1;
    }
    else
    {
        p2 = pMe->m_yx_voc_send_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  YX_VOC_Send_Link_Get(YX_APP_T *pMe,YX_VOC_SEND_DATA_T * pVocSendData)
{
    YX_VOC_SEND_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_yx_voc_send_link_head;
    
    if(p1 != NULL)
    {
      *pVocSendData = p1->data;
       return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  YX_VOC_Send_Link_DelOne(YX_APP_T *pMe)
{
    YX_VOC_SEND_NODE_T  * p1 = NULL;
    YX_VOC_SEND_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_voc_send_link_head;
    
    while(p1 != NULL)
    {
        if(p1 == pMe->m_yx_voc_send_link_head)
        {
            pMe->m_yx_voc_send_link_head = p1->next_ptr;
        }
        else
        {
            p2->next_ptr = p1->next_ptr;
        }
        res = TRUE;
        if(p1->data.pFilename != NULL)
        {
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
        break;
    }
        
    return res;
}

BOOLEAN  YX_VOC_Send_Link_DelAll(YX_APP_T *pMe)
{
    YX_VOC_SEND_NODE_T  * p1 = NULL;
    ZDT_LOG("YX_VOC_SendFile_Link_DelAll");
    while(pMe->m_yx_voc_send_link_head != NULL)
    {
        p1 = pMe->m_yx_voc_send_link_head;
        pMe->m_yx_voc_send_link_head = p1->next_ptr;
        if(p1->data.pFilename!= 0)
        {
            ZDT_File_Remove((char *)p1->data.pFilename);
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static BOOLEAN YX_VOC_RcvFile_Link_Add(YX_APP_T *pMe, YX_VOC_RCV_FILE_DATA_T * pRcvData)
{
    YX_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    YX_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    
    p1=(YX_VOC_RCV_FILE_NODE_T *)SCI_ALLOC_APPZ(sizeof(YX_VOC_RCV_FILE_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(YX_VOC_RCV_FILE_NODE_T));

    p1->data = *pRcvData;
    p1->next_ptr = NULL;
    if(pMe->m_yx_voc_rcvfile_link_head == NULL)
    {
        pMe->m_yx_voc_rcvfile_link_head = p1;
    }
    else
    {
        p2 = pMe->m_yx_voc_rcvfile_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  YX_VOC_RcvFile_Link_GetByIdx(YX_APP_T *pMe,YX_VOC_RCV_FILE_DATA_T * pData,uint16 voc_idx,uint8 pack_idx)
{
    YX_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    YX_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    if(pData == NULL)
    {
        return FALSE;
    }
    
    p2 = p1 = pMe->m_yx_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.voc_idx == voc_idx && p1->data.pack_idx == pack_idx)
        {     
            * pData = p1->data;
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN  YX_VOC_RcvFile_Link_DelByVocIdx(YX_APP_T *pMe,uint16 voc_idx)
{
    YX_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    YX_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    YX_VOC_RCV_FILE_NODE_T  * p3 = NULL;
    BOOLEAN res = FALSE;
    
    p3 = p2 = p1 = pMe->m_yx_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        p2 = p1;
        p1 = p2->next_ptr;
        if(p2->data.voc_idx == voc_idx)
        {     
            if(p2 == pMe->m_yx_voc_rcvfile_link_head)
            {
                pMe->m_yx_voc_rcvfile_link_head = p1;
                p3 = p1;
            }
            else
            {
                p3->next_ptr = p1;
            }
            
            if(p2->data.voc_data != NULL)
            {
                SCI_FREE(p2->data.voc_data);
            }
            SCI_FREE(p2);
            res = TRUE;
        }
        else
        {
            p3 = p2;
        }
    }
        
    return res;
}

BOOLEAN  YX_VOC_RcvFile_Link_DelAll(YX_APP_T *pMe)
{
    YX_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    ZDT_LOG("YX_VOC_RcvFile_Link_DelAll");
    while(pMe->m_yx_voc_rcvfile_link_head != NULL)
    {
        p1 = pMe->m_yx_voc_rcvfile_link_head;
        pMe->m_yx_voc_rcvfile_link_head = p1->next_ptr;
        if(p1->data.voc_data != 0)
        {
            SCI_FREE(p1->data.voc_data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint8  YX_VOC_RcvFile_Link_isValid(YX_APP_T *pMe,uint16 voc_idx)
{
    YX_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    YX_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    uint8 valid_pack_idx[3];
    uint8 pack_sum = 0;
    
    p2 = p1 = pMe->m_yx_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.voc_idx == voc_idx)
        {     
            pack_sum = p1->data.pack_sum;
            if(p1->data.pack_idx == 1)
            {
                valid_pack_idx[0] = 1;
            }
            else if(p1->data.pack_idx == 2)
            {
                valid_pack_idx[1] = 1;
            }

            if(p1->data.pack_sum == p1->data.pack_idx)
            {
                valid_pack_idx[2] = 1;
                if(p1->data.pack_sum == 1)
                {
                    valid_pack_idx[1] = 1;
                }
            }
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    if(pack_sum > 0 && valid_pack_idx[0] == 1 && valid_pack_idx[1] == 1 && valid_pack_idx[2] == 1)
    {
        return pack_sum;
    }
    else
    {
        return 0;
    }
}

int  YX_VOC_RcvFile_Save(YX_APP_T *pMe,YX_VOC_RCV_FILE_DATA_T * pRcvData,uint8 * friend_id)
{
    int res = AEE_SUCCESS;
    MMIFILE_HANDLE file_handle;
    uint8 pack_sum = 0;
    uint16 i = 0;
    
    YX_VOC_RCV_FILE_DATA_T FileData = {0};
    
    pack_sum = YX_VOC_RcvFile_Link_isValid(pMe,pRcvData->voc_idx);
    if(pack_sum > 0)
    {
        if(friend_id == PNULL)
        {
            YX_VCHAT_RCV_GetFileName(pMe,pRcvData->voc_idx,YX_VCHAT_DEFAULT_GROUP_ID);
        }
        else
        {
            YX_VCHAT_RCV_GetFileName(pMe,pRcvData->voc_idx,friend_id);
        }
        ZDT_File_Remove(pMe->m_yx_pFileRcvVocName);
        file_handle = ZDT_File_Open(pMe->m_yx_pFileRcvVocName,FALSE);
        if(file_handle == SFS_INVALID_HANDLE)
        {
            ZDT_LOG("YX Save VocRcvFile ERR Open voc_idx=%d",pRcvData->voc_idx);
            return AEE_EUNABLETOLOAD;
        }
        for(i = 0; i < pack_sum; i++)
        {
            if(YX_VOC_RcvFile_Link_GetByIdx(pMe,&FileData,pRcvData->voc_idx,i+1))
            {
                if(ZDT_File_Write(file_handle, FileData.voc_data, FileData.voc_data_len)==0)
                {
                    ZDT_LOG("YX Save VocRcvFile Err voc_idx=%d,pack_idx=%d",pRcvData->voc_idx,i+1);
                    ZDT_File_Close(file_handle);
                    return AEE_ENOMEMORY;
                }
            }
        }
        
        ZDT_File_Close(file_handle);
    }
    else
    {
        res = AEE_EFAILED;
    }
    return res;
}

BOOLEAN YX_Voice_RecordStop_CallBack(YX_APP_T * pMe,YXVocRcdStopErrStatus err_id)
{        
    return TRUE;
}

BOOLEAN YX_Voice_Record_CallBack(YX_APP_T * pMe,YXVocRecordErrStatus err_code)
{
    if(YX_VOCRECORD_SUCCESS != err_code)
    {
        MMIZDT_TinyChatSendCallBack(FALSE);
    }
    return TRUE;
}

BOOLEAN YX_Voice_Send_CallBack(YX_APP_T * pMe,YXVocSendErrStatus err_code)
{
    if(YX_VOCSEND_SUCCESS == err_code)
    {
        YX_VocFilePathSaveToTxt(pMe->m_yx_pFileSendVocName);
    }
    else
    {
        MMIZDT_TinyChatSendCallBack(FALSE);
    }
    return TRUE;
}

void YX_Voice_RecordStop_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_TIMEOUT,NULL,0);
}

void YX_Voice_RecordStop_TimerStart(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_record_stop_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_record_stop_time_id);
        pMe->m_yx_voc_record_stop_time_id = 0;
    }
    
    pMe->m_yx_voc_record_stop_time_id = MMK_CreateTimerCallback(10000, 
                                                                        YX_Voice_RecordStop_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void YX_Voice_RecordStop_TimerStop(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_record_stop_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_record_stop_time_id);
        pMe->m_yx_voc_record_stop_time_id = 0;
    }
}

static BOOLEAN YX_Voice_File_Check(const char * file_name)
{
    uint32 buf_len = 0;
    
    if(file_name == NULL)
    {
        return FALSE;
    }

    buf_len = ZDT_File_GetSize(file_name);
    
    if(buf_len < 19)
    {
        return FALSE;
    }
    
    return TRUE;
}
PUBLIC BOOLEAN YX_Voice_Record_PlayCur(void)
{
    BOOLEAN ret = FALSE;
    MMIRECORD_SRV_RESULT_E srv_result = MMIRECORD_SRV_RESULT_SUCCESS;

    if(!MMIRECORDSRV_IsRecordFileExist(g_yx_app.m_yx_pRecordMedia))
    {
        ret = FALSE;
    }
    else
    {
        srv_result = MMIRECORDSRV_Play(g_yx_app.m_yx_pRecordMedia, 9);

        if(MMIRECORD_SRV_RESULT_SUCCESS == srv_result)
        {
            ret = TRUE;
        }
    }
    return ret;
}

PUBLIC void YX_Voice_Record_HandleEnd(BOOLEAN is_succeed)
{
    YX_APP_T *   pMe = (YX_APP_T *)&g_yx_app;
    
    if(is_succeed)
    {
        ZDT_LOG("YX_Voice_Record_HandleEnd Success");
        YX_Voice_RecordStop_TimerStop(pMe);
        #ifdef WIN32
            s_yx_voc_is_recording = FALSE;
            pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
            YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_ERR_LOWTIME);
            return;
        #endif
        if (PNULL != pMe->m_yx_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_yx_pRecordMedia);
            pMe->m_yx_pRecordMedia = 0;
        }
        s_yx_voc_is_recording = FALSE;
        pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
        if(YX_Voice_File_Check(pMe->m_yx_pFileSendVocName))
        { 
            if(YX_VOC_Send_Link_Add(pMe,(uint8 *)pMe->m_yx_pFileSendVocName,MAX_YX_VOC_FILENAME_SIZE))
            {
                YX_API_Record_Send();
            }
            YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_SUCCESS);
        }
        else
        {
            ZDT_File_Remove(pMe->m_yx_pFileSendVocName);
            YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_ERR_LOWTIME);
        }
    }
    else
    {
        ZDT_LOG("YX_Voice_Record_HandleEnd FAIL");
        YX_Voice_RecordStop_TimerStop(pMe);
        if (PNULL != pMe->m_yx_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_yx_pRecordMedia);
            pMe->m_yx_pRecordMedia = 0;
        }
        pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
        s_yx_voc_is_recording = FALSE;
        ZDT_File_Remove(pMe->m_yx_pFileSendVocName);
        YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_ERR);
    }
    return;
}

static void YX_Voice_RecordNotify(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result)
{

	if (record_srv_handle == g_yx_app.m_yx_pRecordMedia
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{			
            ZDT_LOG("YX_Voice_RecordNotify RECORD result=%d",result);
            if(MMIRECORD_SRV_RESULT_SUCCESS == result)
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_SUCCESS,NULL,0);
            }
            else
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_FAIL,NULL,0);
            }
	}
    
    	if (record_srv_handle == g_yx_app.m_yx_pRecordMedia
		&& MMIRECORD_SRV_SUPPORT_PLAY == support_type)
	{			
            ZDT_LOG("YX_Voice_RecordNotify PLAY result=%d",result);
	}

}

void YX_Voice_Play_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_APP_T *   pMe = (YX_APP_T *)param;
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP,NULL,0);
    if(YX_Voice_Play_Valid())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_FILEPLAY,NULL,0);
    }
}

void YX_Voice_Play_TimerStart(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_play_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_play_time_id);
        pMe->m_yx_voc_play_time_id = 0;
    }
    
    pMe->m_yx_voc_play_time_id = MMK_CreateTimerCallback(1000, 
                                                                        YX_Voice_Play_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void YX_Voice_Play_TimerStop(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_play_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_play_time_id);
        pMe->m_yx_voc_play_time_id = 0;
    }
}
static void YX_Voice_Play_HandleEnd(BOOLEAN is_end)
{
    YX_APP_T *   pMe = (YX_APP_T *)&g_yx_app;

    if (is_end )
    {
        s_voc_play_err_num = 0;
        ZDT_LOG("YX_Voice_PlayNotify Play DONE");
        if(pMe->m_yx_pPlayMedia)
        {
            MMISRVAUD_Stop(pMe->m_yx_pPlayMedia);
            MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
            pMe->m_yx_pPlayMedia = NULL;
        }
        pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
        pMe->m_yx_voc_is_playing = FALSE;
        if(s_yx_voc_is_playing)
        {               
            if(s_voc_is_play_all)
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP,NULL,0);
                if(YX_Voice_Play_Valid())
                {
                    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_FILEPLAY,NULL,0);
                }
            }
            else
            {
                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP,NULL,0);
            }
        }
    }
    else
    {
        ZDT_LOG("YX_Voice_PlayNotify Play ABORT err_num=%d",s_voc_play_err_num);
        if(pMe->m_yx_pPlayMedia)
        {
            MMISRVAUD_Stop(pMe->m_yx_pPlayMedia);
            MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
            pMe->m_yx_pPlayMedia = NULL;
        }
        pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
        pMe->m_yx_voc_is_playing = FALSE;
        
        if(s_yx_voc_is_playing)
        {
            s_voc_play_err_num++;
            if(s_voc_play_err_num > 2)
            {
                s_voc_play_err_num = 0;

                MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP,NULL,0);
                if(s_voc_is_play_all)
                {
                    if(YX_Voice_Play_Valid())
                    {
                        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_FILEPLAY,NULL,0);
                    }
                }
            }
            else
            {
                YX_Voice_Play_TimerStart(pMe);
            }
        }
    }
}

static void YX_Voice_PlayNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    YX_APP_T *   pMe = (YX_APP_T *)&g_yx_app;
    BOOLEAN       result = TRUE;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pMe->m_yx_pPlayMedia)
        {
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:                
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    pMe->m_yx_pPlayMedia = 0;

                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        YX_Voice_Play_HandleEnd(result);
                    }

                break;

            default:
                break;
            }
        }
    }
}

int YX_Net_TK2_GetContentLen_RCV(YX_APP_T *pMe,uint8 * pData,uint32 Data_len,uint8 ** ppContent)
{
    uint8 is_md5_head = 0;
    uint16 MsgLen = 0;
    uint16 head_len = g_yx_imei_len + 10;
    
    if(pData == NULL || Data_len < (head_len+1))
    {
        ZDT_LOG("ZDT__LOG YX_Net_GetContentLen_RCV Err NULL");
        return -1;
    }
    
    if(pData[0] != YX_NET_MSG_START 
        //|| pData[1] != YX_NET_MSG_RCV_CS_1 
        //|| pData[2] != YX_NET_MSG_RCV_CS_2 
        || pData[3] != '*'
        || pData[head_len-6] != '*'
        || pData[head_len-1] != '*'
        )
    {
        head_len += 32;
        if(pData[0] != YX_NET_MSG_START 
            || pData[head_len-6] != '*'
            || pData[head_len-1] != '*'
            )
        {
            return -1;
        }
    }
    MsgLen = YX_Func_HexCharToInt(pData[head_len-5]);
    MsgLen = MsgLen<<4;
    MsgLen += YX_Func_HexCharToInt(pData[head_len-4]);
    MsgLen = MsgLen<<4;
    MsgLen += YX_Func_HexCharToInt(pData[head_len-3]);
    MsgLen = MsgLen<<4;
    MsgLen += YX_Func_HexCharToInt(pData[head_len-2]);
    
    if((MsgLen+head_len+1) > Data_len)
    {
        ZDT_LOG("ZDT__LOG YX_Net_GetContentLen_RCV Err Len %d>%d",(MsgLen+head_len+1),Data_len);
        return -1;
   }
    *ppContent = pData+head_len;
    return (int)MsgLen;
}
static int YX_Net_Send_TK_VocFile_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char buf[21] = {0};
    char val_buf[11] = {0};
    int ret = 0;
    int ret_val = 0;
    uint16 rsp_val = 0;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    cont_len = YX_Net_TK2_GetContentLen_RCV(pMe,pRcv,Rcv_len,&pContent);
    if(cont_len > 0)
    {
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,20);
        if(ret > 0)
        {
            YX_Func_strupr(buf);
        }
        if(ret == 2 &&  strncmp( (char *)buf, "TK", ret ) == 0)
        {
            ret_val = YX_Func_GetNextPara(&pContent, &Content_len,val_buf,10);
            if(ret_val > 0)
            {
                rsp_val = atoi(val_buf);
            }
            if(rsp_val == 1)
            {
                ZDT_LOG("YX_Net_Send_TK_VocFile_CB Success");
                return AEE_SUCCESS;
            }
            else
            {
                ZDT_LOG("YX_Net_Send_TK_VocFile_CB Fail");
                return AEE_EFAILED;
            }
        }
        else if(ret == 3 &&  strncmp( (char *)buf, "TK2", ret ) == 0)
        {
            ret_val = YX_Func_GetNextPara(&pContent, &Content_len,buf,20);
            ret_val = YX_Func_GetNextPara(&pContent, &Content_len,val_buf,10);
            if(ret_val > 0)
            {
                rsp_val = atoi(val_buf);
            }
            if(rsp_val == 1)
            {
                ZDT_LOG("YX_Net_Send_TK2_VocFile_CB Success");
                return AEE_SUCCESS;
            }
            else
            {
                ZDT_LOG("YX_Net_Send_TK2_VocFile_CB Fail");
                return AEE_EFAILED;
            }
        }
    }

    ZDT_LOG("YX_Net_Send_TK_VocFile_CB  ERR cont_len=%d",cont_len);
    return AEE_EFAILED;
}

/*
语音包格式是MM_AMR_FORMAT_FIXED_4_75_AMR
AMR 头 6 个字节,每一帧13个字节发送时需要对齐
*/
int32 YX_Net_Send_TK_VocFile_Start(YX_APP_T *pMe)
{

    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint32 len = 0;
    uint32 msg_len = 0;
    uint32 i = 0;
    uint32 j = 0;
    uint32 content_len = 0;
    BOOLEAN bResult = TRUE;
    uint32 buf_len = 0;
    uint32 file_len = 0;
    MMIFILE_HANDLE file_handle;
    uint32 cur_len = 0;
    char * file_name = NULL;
    char len_str[5] = {0};
    char id_str[40] = {0};
    uint32 id_len = 0;
    uint8 * tmp_buf = NULL;
    YX_VOC_SEND_DATA_T cur_filedata = {0};
#if FEATURE_YX_SEND_MD5
    char md5_str[32] = {0};
#endif
    BOOLEAN is_common_send = FALSE;

    if(pMe->m_yx_voc_is_sending)
    {
        ZDT_LOG("Voc is Sending");
        return 0;
    }
    
    if(FALSE == YX_VOC_Send_Link_Get(pMe,&cur_filedata))
    {
        return 0;
    }

    if(pMe->m_yx_is_land == FALSE)
    {
        return -10;
    }
    
    if(g_yx_imei_len == 0)
    {
        return -1;
    }
    #if 0
    if(pMe->m_zdt_is_in_call)
    {
    	ZDT_LOG("Voc is InCall");
    	return 0;
    }
    #endif
    
    file_name = (char *)cur_filedata.pFilename;
    if(file_name == NULL)
    {
        ZDT_LOG("YX_Send_FileStart: ERR Name");
        return -1;
    }
    id_len = YX_VCHAT_GetGroupIDByFileName(file_name,id_str);
    if(id_len == SCI_STRLEN(YX_VCHAT_DEFAULT_GROUP_ID) && strncmp(id_str,YX_VCHAT_DEFAULT_GROUP_ID,id_len) == 0)
    {
        is_common_send = TRUE;
    }
    file_len = ZDT_File_GetSize(file_name);
    if(file_len < 19)
    {
        ZDT_LOG("YX_Send_FileStart: ERR File Len=%d",file_len);
        return -5;
    }
    
    tmp_buf = (uint8 * )SCI_ALLOC_APPZ(file_len+10);
    if(tmp_buf == NULL)
    {
        ZDT_LOG("YX_Send_FileStart: Malloc File Len=%d",file_len+10);
        return -6;
    }

    ZDT_File_Read(file_name, tmp_buf, file_len,&cur_len);
    if(cur_len >  0)
    {
        ZDT_LOG("YX_Send_FileStart: IFILE_Read cur_len=0x%x,file_len=0x%x",cur_len,file_len);
        pMe->m_yx_voc_file_seek = cur_len;
        content_len = cur_len;
        for(i = 0; i < cur_len; i++)
        {
            if(tmp_buf[i] == 0x7D)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x5B)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x5D)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x2C)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x2A)
            {
                content_len++; 
            }
        }
    }
    else
    {
        SCI_FREE(tmp_buf);
        return -7;
    }
        
    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("YX_Send_FileStart: ERR Malloc 2");
        SCI_FREE(tmp_buf);
        return -8;
    }
    if(is_common_send)
    {
        content_len = content_len + 3;
    }
    else
    {
        content_len = content_len + id_len+5;
    }
#if FEATURE_YX_SEND_MD5
    buf_len = content_len+g_yx_imei_len+43;
#else
    buf_len = content_len+g_yx_imei_len+11;
#endif
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(buf_len);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("YX_Send_FileStart: ERR Malloc buf_len=%d",buf_len);
        SCI_FREE(tmp_buf);
        SCI_FREE(cur_data);
        return -9;
    }
    
#if FEATURE_YX_SEND_MD5
    //[XY*
    cur_data->str[0] = YX_NET_MSG_START;
    cur_data->str[33] = YX_NET_MSG_SEND_CS_1;
    cur_data->str[34] = YX_NET_MSG_SEND_CS_2;
    cur_data->str[35] = '*';
    len = 36;
 #else
    //[XY*
    cur_data->str[0] = YX_NET_MSG_START;
    cur_data->str[1] = YX_NET_MSG_SEND_CS_1;
    cur_data->str[2] = YX_NET_MSG_SEND_CS_2;
    cur_data->str[3] = '*';
    len = 4;
#endif
    //MEID
    SCI_MEMCPY(cur_data->str+len,g_zdt_phone_imei,g_yx_imei_len);
    len += g_yx_imei_len;
    cur_data->str[len++] = '*';
    if(is_common_send)
    {
        j = len+8;
    }
    else
    {
        j = len+10+id_len;
    }
    for(i = 0; i < cur_len; i++)
    {
        if(tmp_buf[i] == 0x7D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x01; 
        }
        else if(tmp_buf[i] == 0x5B)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x02; 
        }
        else if(tmp_buf[i] == 0x5D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x03; 
        }
        else if(tmp_buf[i] == 0x2C)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x04; 
        }
        else if(tmp_buf[i] == 0x2A)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x05; 
        }
        else
        {
            cur_data->str[j++] = tmp_buf[i]; 
        }
    }
    
    SCI_FREE(tmp_buf);
    
    //包长度
    msg_len = j-len-5;
    sprintf(len_str,"%04X",msg_len);
    SCI_MEMCPY(cur_data->str+len,len_str,4);
    len += 4;
    cur_data->str[len++] = '*';
    cur_data->str[len++] = 'T';
    cur_data->str[len++] = 'K';
    if(is_common_send)
    {
        cur_data->str[len++] = ',';
    }
    else
    {
        cur_data->str[len++] = '2';
        cur_data->str[len++] = ',';
        SCI_MEMCPY(cur_data->str+len,id_str,id_len);
        len += id_len;
        cur_data->str[len++] = ',';
    }
    
    //结束]
    len = j;
    
#if FEATURE_YX_SEND_MD5
    YX_Net_MD5(cur_data->str+33,len-33,md5_str);
    SCI_MEMCPY(cur_data->str+1,md5_str,32);
#endif

    cur_data->str[len++] = YX_NET_MSG_END;

    pMe->m_yx_voc_pack_num = 1;
    pMe->m_yx_voc_pack_idx = 1;
    pMe->m_yx_voc_data_idx++;

    cur_data->len = len;
    cur_data->times = 1;
    cur_data->timeout = 0;
    cur_data->priority = 150;
    cur_data->rcv_handle = YX_Net_Send_TK_VocFile_CB;
    ZDT_API_TCP_SendDataNow(pMe->m_yx_tcp_reg_interface,cur_data);
    pMe->m_yx_voc_is_sending = TRUE;
    ZDT_LOG("YX_Send_FileStart Start num=%d/%d,data_idx=%d,seek=%d,file_len=%d",\
        pMe->m_yx_voc_pack_idx,pMe->m_yx_voc_pack_num,pMe->m_yx_voc_data_idx,pMe->m_yx_voc_file_seek,file_len);
    return cur_len;
}

void YX_Net_Send_TK_VocFile_End(YX_APP_T *pMe,uint8 err_id)
{
    YX_VOC_SEND_DATA_T cur_filedata = {0};
    
    if(YX_VOC_Send_Link_Get(pMe,&cur_filedata))
    {
        //ZDT_File_Remove((char *)cur_filedata.pFilename);
        YX_VOC_Send_Link_DelOne(pMe);
    }
    
    pMe->m_yx_voc_file_seek = 0;
    pMe->m_yx_voc_is_sending = FALSE;
    
    if(YX_VOCSEND_SUCCESS == err_id)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_SEND,NULL,0);
    }
    else
    {        
        if(err_id != YX_VOCSEND_ERR_FILE)
        {
            YX_VOC_Send_Link_DelAll(pMe);
        }
        else
        {
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_SEND,NULL,0);
        }
    }
    YX_Voice_Send_CallBack(pMe,err_id);
    ZDT_LOG("YX_Net_Send_TK_VocFile_End err_id=%d",err_id);
    return;
}

BOOLEAN YX_Voice_Send_Start(YX_APP_T * pMe)
{
    if(0 > YX_Net_Send_TK_VocFile_Start(pMe))
    {
        YX_Net_Send_TK_VocFile_End(pMe,YX_VOCSEND_ERR);
    }
}

void YX_Voice_File_Rcv_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_APP_T *   pMe = (YX_APP_T *)param;
    YX_VOC_RcvFile_Link_DelAll(pMe);
}

void YX_Voice_File_Rcv_TimerStart(YX_APP_T * pMe)
{
    
    if(0 != pMe->m_yx_voc_rcv_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_rcv_time_id);
        pMe->m_yx_voc_rcv_time_id = 0;
    }
    
    pMe->m_yx_voc_rcv_time_id = MMK_CreateTimerCallback(30000, 
                                                                        YX_Voice_File_Rcv_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void YX_Voice_File_Rcv_TimerStop(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_rcv_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_rcv_time_id);
        pMe->m_yx_voc_rcv_time_id = 0;
    }
}

int YX_Net_Receive_TK_VocFile(YX_APP_T *pMe,uint8 * pFiledata,uint16 DataLen)
{
    int result = 0;
    
    if(pFiledata != NULL && DataLen > 18)
    {
        uint8 pack_num;
        uint8 pack_idx;
        uint16 voc_idx;
        uint8 mem_status = 0;
        BOOLEAN is_full = FALSE;
        int res = 0;
        uint16 voc_len = 0;
        uint16 i = 0;
        uint16 j = 0;
        
        YX_VOC_RCV_FILE_DATA_T voc_rcv_data = {0};
        s_yx_voc_rcv_idx ++;
        if(s_yx_voc_rcv_idx > 0xFFF0)
        {
            s_yx_voc_rcv_idx = 1;
        }
        pack_num = 1;
        voc_idx = s_yx_voc_rcv_idx;
        pack_idx = 1;
        
        ZDT_LOG("YX_Net_Receive_TK_VocFile s_yx_voc_rcv_idx=%d",s_yx_voc_rcv_idx);
        
        voc_rcv_data.voc_idx = voc_idx;
        voc_rcv_data.pack_idx = pack_idx;
        voc_rcv_data.pack_sum= pack_num;
        voc_rcv_data.voc_data = (uint8 *)SCI_ALLOC_APPZ(DataLen);
        if(voc_rcv_data.voc_data == NULL)
        {
            ZDT_LOG("YX_Net_Receive_TK_VocFile Err SCI_ALLOC_APPZ Len=%d",DataLen);
            return 0;
        }
        
        for(i = 0; i < DataLen; i++)
        {
            if(pFiledata[i] == 0x7D && i != (DataLen-1))
            {
                if(pFiledata[i+1] == 0x01)
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x02)
                {
                    voc_rcv_data.voc_data[j] = 0x5B;
                    i++;
                }
                else if(pFiledata[i+1] == 0x03)
                {
                    voc_rcv_data.voc_data[j] = 0x5D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x04)
                {
                    voc_rcv_data.voc_data[j] = 0x2C;
                    i++;
                }
                else if(pFiledata[i+1] == 0x05)
                {
                    voc_rcv_data.voc_data[j] = 0x2A;
                    i++;
                }
                else
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                }
            }
            else
            {
                voc_rcv_data.voc_data[j] = pFiledata[i];
            }
            j++;
        }
        voc_rcv_data.voc_data_len = j;

        if(FALSE == YX_VOC_RcvFile_Link_Add(pMe,&voc_rcv_data))
        {
            ZDT_LOG("YX_Net_Receive_TK_VocFile ERR LinkAdd");
            return 0;
        }
                
        if(pack_idx == pack_num)
        {            
            res = YX_VOC_RcvFile_Save(pMe,&voc_rcv_data,YX_VCHAT_DEFAULT_GROUP_ID);
            
            if(res == AEE_SUCCESS)
            {
                result = 1;
                YX_VocFilePathSaveToTxt(pMe->m_yx_pFileRcvVocName);//yangyu add
                m_pCurRcvGroupInfo =  YX_VCHAT_GetGroupInfoByFileName(pMe->m_yx_pFileRcvVocName);
                MMIVoice_ShowNewMsgPrompt(TRUE);
            }
            else
            {
                if(res == AEE_ENOMEMORY || res == AEE_EUNABLETOLOAD)
                {
                    if(is_full)
                    {
                    }
                    else
                    {
                    }
                }
                ZDT_LOG("YX_Net_Receive_TK_VocFile ERR FileSave res=%d,voc_idx=%d",res,voc_idx);
            }
            YX_VOC_RcvFile_Link_DelByVocIdx(pMe,voc_idx);
        }
        YX_Voice_File_Rcv_TimerStart(pMe);
    }
    ZDT_LOG("YX_Net_Receive_TK_VocFile result=%d",result);
    return result;
}

int YX_Net_Receive_TK2_VocFile(YX_APP_T *pMe,uint8 * pFiledata,uint16 DataLen,uint8 * friend_id)
{
    int result = 0;
    
    if(pFiledata != NULL && DataLen > 18)
    {
        uint8 pack_num;
        uint8 pack_idx;
        uint16 voc_idx;
        uint8 mem_status = 0;
        BOOLEAN is_full = FALSE;
        int res = 0;
        uint16 voc_len = 0;
        uint16 i = 0;
        uint16 j = 0;
        
        YX_VOC_RCV_FILE_DATA_T voc_rcv_data = {0};
        s_yx_voc_rcv_idx ++;
        if(s_yx_voc_rcv_idx > 0xFFF0)
        {
            s_yx_voc_rcv_idx = 1;
        }
        pack_num = 1;
        voc_idx = s_yx_voc_rcv_idx;
        pack_idx = 1;
        
        ZDT_LOG("YX_Net_Receive_TK2_VocFile s_yx_voc_rcv_idx=%d",s_yx_voc_rcv_idx);
        
        voc_rcv_data.voc_idx = voc_idx;
        voc_rcv_data.pack_idx = pack_idx;
        voc_rcv_data.pack_sum= pack_num;
        voc_rcv_data.voc_data = (uint8 *)SCI_ALLOC_APPZ(DataLen);
        if(voc_rcv_data.voc_data == NULL)
        {
            ZDT_LOG("YX_Net_Receive_TK2_VocFile Err SCI_ALLOC_APPZ Len=%d",DataLen);
            return 0;
        }
        
        for(i = 0; i < DataLen; i++)
        {
            if(pFiledata[i] == 0x7D && i != (DataLen-1))
            {
                if(pFiledata[i+1] == 0x01)
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x02)
                {
                    voc_rcv_data.voc_data[j] = 0x5B;
                    i++;
                }
                else if(pFiledata[i+1] == 0x03)
                {
                    voc_rcv_data.voc_data[j] = 0x5D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x04)
                {
                    voc_rcv_data.voc_data[j] = 0x2C;
                    i++;
                }
                else if(pFiledata[i+1] == 0x05)
                {
                    voc_rcv_data.voc_data[j] = 0x2A;
                    i++;
                }
                else
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                }
            }
            else
            {
                voc_rcv_data.voc_data[j] = pFiledata[i];
            }
            j++;
        }
        voc_rcv_data.voc_data_len = j;

        if(FALSE == YX_VOC_RcvFile_Link_Add(pMe,&voc_rcv_data))
        {
            ZDT_LOG("YX_Net_Receive_TK2_VocFile ERR LinkAdd");
            return 0;
        }
                
        if(pack_idx == pack_num)
        {            
            res = YX_VOC_RcvFile_Save(pMe,&voc_rcv_data,friend_id);
            
            if(res == AEE_SUCCESS)
            {
                result = 1;
                YX_VocFilePathSaveToTxt(pMe->m_yx_pFileRcvVocName);
                m_pCurRcvGroupInfo =  YX_VCHAT_GetGroupInfoByFileName(pMe->m_yx_pFileRcvVocName);
                MMIVoice_ShowNewMsgPrompt(TRUE);
            }
            else
            {
                if(res == AEE_ENOMEMORY || res == AEE_EUNABLETOLOAD)
                {
                    if(is_full)
                    {
                    }
                    else
                    {
                    }
                }
                ZDT_LOG("YX_Net_Receive_TK2_VocFile ERR FileSave res=%d,voc_idx=%d",res,voc_idx);
            }
            YX_VOC_RcvFile_Link_DelByVocIdx(pMe,voc_idx);
        }
        YX_Voice_File_Rcv_TimerStart(pMe);
    }
    ZDT_LOG("YX_Net_Receive_TK2_VocFile result=%d",result);
    return result;
}
void YX_Voice_Record_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    YX_APP_T *   pMe = (YX_APP_T *)param;
    if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_RECORD)
    {
        if(pMe->m_yx_voc_send_times > 0)
        {
            pMe->m_yx_voc_send_times--;
        }
        if(pMe->m_yx_voc_send_times == 0)
        {
            if(0 != pMe->m_yx_voc_record_time_id)
            {
                MMK_StopTimer(pMe->m_yx_voc_record_time_id);
                pMe->m_yx_voc_record_time_id = 0;
            }
            YX_API_Record_Stop();
        }
        else
        {
            if(0 != pMe->m_yx_voc_record_time_id)
            {
                MMK_StartTimerCallback(pMe->m_yx_voc_record_time_id, 1000, YX_Voice_Record_TimeOut, (uint32)pMe, FALSE);
            }
        }
    }
}

void YX_Voice_Record_TimerStart(YX_APP_T * pMe)
{
    
    if(0 != pMe->m_yx_voc_record_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_record_time_id);
        pMe->m_yx_voc_record_time_id = 0;
    }
    
    pMe->m_yx_voc_record_time_id = MMK_CreateTimerCallback(1000, 
                                                                        YX_Voice_Record_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void YX_Voice_Record_TimerStop(YX_APP_T * pMe)
{
    if(0 != pMe->m_yx_voc_record_time_id)
    {
        MMK_StopTimer(pMe->m_yx_voc_record_time_id);
        pMe->m_yx_voc_record_time_id = 0;
    }
}

BOOLEAN YX_Voice_Record(YX_APP_T * pMe)
{
    BOOLEAN Ret = FALSE;
    MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
    MMIRECORD_SRV_RESULT_E  record_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMIFILE_DEVICE_E record_dev = MMI_DEVICE_UDISK;
    BOOLEAN build_file = FALSE;
    MMI_STRING_T record_name = {0};
    ZDT_LOG("YX_Voice_Record");
    if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_RECORD)
    {
        return TRUE;
    }
    #ifdef WIN32
        YX_Voice_Record_TimerStart(pMe);
        s_yx_voc_is_recording = TRUE;
        pMe->m_yx_eRecorderStatus = YX_RECOREDER_RECORD;
        return TRUE;
    #endif

    build_file = YX_VCHAT_Send_GetFileName(pMe);
    if(FALSE == build_file)
    {
        ZDT_LOG("YX_Voice_Record ERR FileName");
        return FALSE;
    }
    
    if (PNULL != pMe->m_yx_pRecordMedia)
    {
        MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
        MMIRECORDSRV_FreeRecordHandle(pMe->m_yx_pRecordMedia);
        pMe->m_yx_pRecordMedia = 0;
    }

    pMe->m_yx_pRecordMedia = MMIRECORDSRV_RequestRecordHandle(YX_Voice_RecordNotify);
    if (PNULL == pMe->m_yx_pRecordMedia)
    {
        ZDT_LOG("YX_Voice_Record ERR Handle");
        return FALSE;
    }
    
    record_name.wstr_ptr = pMe->m_yx_pFileSendVocFullName;
    record_name.wstr_len = pMe->m_yx_pFileSendVocFullName_len;
    
    record_param.is_create_file_name = TRUE;
    record_param.prefix_ptr = PNULL;
    record_param.record_dev_ptr = &record_dev;
    record_param.record_file_id_ptr = PNULL;
    record_param.record_name_ptr = &record_name;
    record_param.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
    record_param.fmt = MMISRVAUD_RECORD_FMT_AMR;
    record_param.record_param_ptr = PNULL;
    record_param.volume = 9; //MMIAPISET_GetMultimVolume();
    record_result = MMIRECORDSRV_StartRecordFullFile(pMe->m_yx_pRecordMedia, record_param);

    if (record_result == MMIRECORD_SRV_RESULT_SUCCESS)
    {
        Ret = TRUE;
        YX_Voice_Record_TimerStart(pMe);
        s_yx_voc_is_recording = TRUE;
        pMe->m_yx_eRecorderStatus = YX_RECOREDER_RECORD;
        ZDT_LOG("Voice_Record OK");
    }
    return Ret;
}

BOOLEAN YX_Voice_Record_Start(YX_APP_T * pMe)
{
    BOOLEAN Ret = TRUE;
    ZDT_LOG("ZDTYX_Voice_RecordStart");
    #ifdef WIN32
    if(YX_Voice_Record(pMe))
    {
        pMe->m_yx_voc_send_times = YX_VOICE_RECORD_TIME_OUT;
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_SUCCESS);
    }
    else
    {
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR);
    }
    return TRUE;
    #endif
    if(ZDT_SIM_Exsit() == FALSE)
    {
        ZDT_LOG("RECORD_START Err NoCard");
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_NO_CARD);
        return FALSE;
    }
#ifdef YX_DB_NEED_TIMER
    else if(YX_DB_TIMER_IsValid_Slient())
    {
        ZDT_LOG("RECORD_START Err HIDDEN");
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_INHIDDEN);
        return FALSE;
    }
#endif
#if 0
    else if(CallUtil_Get_Calldraw_Status() != CALLDRAW_STATUS_NONE)
    {
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_INCALL);
        return FALSE;
    }
#endif
#if 0
    else if( FALSE == CDMA_Service_Status_Isok())
    {
        ZDT_LOG("RECORD_START Err NoServer");
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_NO_SRV);
        return FALSE;
    }
#endif
    else if(pMe->m_yx_is_land == FALSE)
    {
        ZDT_LOG("RECORD_START Err No Land");
        YX_Net_Rereg(pMe);
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_NET);
        return FALSE;
    }
    else if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_RECORD)
    {
        ZDT_LOG("RECORD_START Err IsRecording");
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_RECORDING);
        return FALSE;
    }
    
    if(YX_Voice_GetMemStatus(pMe) == 0)
    {
            ZDT_LOG("RECORD_START Err MemFull");
            //YX_Voice_FileSendAllRemove(pMe);
            YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR_SPACE);
            return FALSE;
    }

    if(s_yx_voc_is_playing)
    {
        YX_Voice_FilePlay_Stop(pMe);
    }
    
    //RingSet_PlayStop();
    if(YX_Voice_Record(pMe))
    {
        pMe->m_yx_voc_send_times = YX_VOICE_RECORD_TIME_OUT;
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_SUCCESS);
    }
    else
    {
        YX_Voice_Record_CallBack(pMe,YX_VOCRECORD_ERR);
        return FALSE;
    }
    return TRUE;
}

BOOLEAN YX_Voice_Record_Stop(YX_APP_T * pMe)
{
    ZDT_LOG("YX_Voice_Record_Stop");
    if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_RECORD)
    {
        YX_Voice_Record_TimerStop(pMe);
        #ifdef WIN32
            MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_SUCCESS,NULL,0);
            return TRUE;
        #endif
        if(pMe->m_yx_pRecordMedia)
        {
            YX_Voice_RecordStop_TimerStart(pMe);
            MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
        }
        else
        {
            s_yx_voc_is_recording = FALSE;
            pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
            //ZDT_File_Remove(pMe->m_yx_pFileSendVocName);
            YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_ERR_SPACE);
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN YX_Voice_Record_StopTimout(YX_APP_T * pMe)
{
    ZDT_LOG("YX_Voice_Record_StopTimout");
    if(pMe->m_yx_eRecorderStatus == YX_RECOREDER_RECORD)
    {
        if (PNULL != pMe->m_yx_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_yx_pRecordMedia);
            pMe->m_yx_pRecordMedia = 0;
        }
        s_yx_voc_is_recording = FALSE;
        pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
        //ZDT_File_Remove(pMe->m_yx_pFileSendVocName);
        YX_Voice_RecordStop_CallBack(pMe,YX_VOCRCDSTOP_ERR_TIMEOUT);
        return TRUE;
    }
    return FALSE;
}

BOOLEAN YX_Voice_Play_CallBack(YX_APP_T * pMe,YXFilePlayErrStatus err_code)
{
    if(YX_FILEPLAY_SUCCESS == err_code)
    {
        //MMIZDT_UpdateTalkTPWin();
    }
    else
    {
        char err_str[100] = {0};
        sprintf(err_str,"Err=%d",err_code);
        MMIZDT_UpdateTalkTPWinErr(0,err_str);
    }
    return TRUE;
}

LOCAL BOOLEAN YX_Voice_Play_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  volume;

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISRVAUD_VOLUME_LEVEL_MAX)
        {
            audio_srv.volume = MMISRVAUD_VOLUME_LEVEL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int YX_Voice_FilePlay(YX_APP_T * pMe, const char * file_name)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16		full_path[MAX_YX_VOC_FILENAME_SIZE+1] = {0};
    uint16		full_path_len = 0;
    int Ret = 0;
    if(file_name == PNULL)
    {
        return 1;
    }
    
    if(pMe->m_yx_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_yx_pPlayMedia);
        MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
        pMe->m_yx_pPlayMedia = NULL;
    }

    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    
    if(YX_Voice_Play_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, YX_Voice_PlayNotify,s_yx_voc_play_vol))
    {
        pMe->m_yx_pPlayMedia = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {			          
            MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
		pMe->m_yx_pPlayMedia = 0;		
            Ret = 2;
        }
    }
    else
    {        
        Ret = 3;
    }
	  
    if (Ret != 0)
    {
        pMe->m_yx_pPlayMedia = NULL;
        pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
        ZDT_LOG("Voice_RecordPlay Err Play Ret=%d",Ret);
        return Ret;
    }
    else
    {
        ZDT_LOG("Voice_RecordPlay OK vol=%d",s_yx_voc_play_vol);
        pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYING;        
    }
    return Ret;
}

BOOLEAN YX_Voice_FilePlay_Start(YX_APP_T * pMe)
{
    BOOLEAN Ret = FALSE;
    uint8 need_play = 0;
    
    ZDT_LOG("YX_Voice_FilePlay_Start is_playing = %d,%d",pMe->m_yx_voc_is_playing,s_yx_voc_is_playing);

    if(pMe->m_yx_voc_is_playing)
    {
        YX_Voice_Play_CallBack(pMe,YX_FILEPLAY_ERR_PLAYING);
        return FALSE;
    }

#if 0
    if(CallUtil_Get_Calldraw_Status() != CALLDRAW_STATUS_NONE)
    {
        YX_Voice_Play_CallBack(pMe,YX_FILEPLAY_ERR_INCALL);
        return FALSE;
    }
#endif
    if(s_yx_voc_is_recording)
    {
        YX_Voice_Play_CallBack(pMe,YX_FILEPLAY_ERR_RECORRDING);
        return FALSE;
    }
    
#ifdef YX_DB_NEED_TIMER
    if(YX_DB_TIMER_IsValid_Slient())
    {
        YX_Voice_Play_CallBack(pMe,YX_FILEPLAY_ERR_HIDDEN);
        return FALSE;
    }
#endif
    YX_Voice_Play_TimerStop(pMe);
    YX_Voice_FilePlay_Stop(pMe);
    return Ret;

}

BOOLEAN YX_Voice_FilePlay_Stop(YX_APP_T * pMe)
{
#ifdef TOUCH_PANEL_SUPPORT
    BOOLEAN is_play = s_yx_voc_is_playing;
#endif
    BOOLEAN               result = FALSE;
    ZDT_LOG("YX_Voice_FilePlay_Stop");
    YX_Voice_Play_TimerStop(pMe);

    if(pMe->m_yx_pPlayMedia)
    {
        result = MMISRVAUD_Stop(pMe->m_yx_pPlayMedia);
        if (result)
        {
            MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
            pMe->m_yx_pPlayMedia = NULL;
        }
    }
    pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
    pMe->m_yx_voc_is_playing = FALSE;
    s_yx_voc_is_playing = FALSE;
    return TRUE;
}

BOOLEAN YX_Voice_FilePlay_SetVolume(YX_APP_T * pMe,uint16 vol)
{
    ZDT_LOG("YX_Voice_FilePlay_SetVolume vol = %d,is_playing=%d",vol,s_yx_voc_is_playing);
    if(s_yx_voc_is_playing)
    {
        if(pMe->m_yx_pPlayMedia)
        {
            //IMEDIA_SetVolume(pMe->m_yx_pPlayMedia, (uint16)MIN(vol * 25 , AEE_MAX_VOLUME));
        }
    }
    return TRUE;
}

BOOLEAN YX_Voice_Record_Valid(void)
{
    BOOLEAN res = FALSE;
    if(yx_DB_Set_Rec.net_open)
    {
        res = TRUE;
    }        
    ZDT_LOG("YX_Voice_Record_Valid res=%d",res);
    return res;
}

BOOLEAN YX_Voice_Play_Valid(void)
{
    BOOLEAN res = FALSE;
    if(yx_DB_Set_Rec.net_open)
    {
        res = TRUE;
    }        
    ZDT_LOG("YX_Voice_Play_Valid res=%d",res);
    return res;
}

int YX_Voice_HandleStop(YX_APP_T * pMe)
{
    YX_Voice_Record_TimerStop(pMe);
    if (PNULL != pMe->m_yx_pRecordMedia)
    {
        MMIRECORDSRV_StopRecord(pMe->m_yx_pRecordMedia);
        MMIRECORDSRV_FreeRecordHandle(pMe->m_yx_pRecordMedia);
        pMe->m_yx_pRecordMedia = 0;
    }
    s_yx_voc_is_recording = FALSE;
    pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;

    YX_Voice_Play_TimerStop(pMe);

    if(pMe->m_yx_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_yx_pPlayMedia);
        MMISRVMGR_Free(pMe->m_yx_pPlayMedia);
        pMe->m_yx_pPlayMedia = NULL;
    }
    pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
    pMe->m_yx_voc_is_playing = FALSE;
    s_yx_voc_is_playing = FALSE;
    return AEE_SUCCESS;
}

int YX_API_Record_Start(void)
{
    #ifdef WIN32
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_START,NULL,0);
        return AEE_SUCCESS;
    #else
    if(YX_Voice_Record_Valid())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_START,NULL,0);
        return AEE_SUCCESS;
    }
    #endif
    return AEE_EFAILED;
}

#ifdef TOUCH_PANEL_SUPPORT
int YX_API_Record_Start_TP(void)
{
    //ISHELL_StartAppletArgs(AEE_GetShell(), AEECLSID_ZDT, "O");
    return AEE_SUCCESS;
}
#endif

int YX_API_Record_Stop(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_STOP,NULL,0);
    return AEE_SUCCESS;
}

int YX_API_Record_Send(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_RECORD_SEND,NULL,0);
    return AEE_SUCCESS;
}

BOOLEAN YX_API_Record_IsFilePlay(void)
{
    return s_yx_voc_is_playing;
}

BOOLEAN YX_API_Record_IsRecording(void)
{
    return s_yx_voc_is_recording;
}

int YX_API_Record_FilePlay(void)
{
    if(YX_Voice_Play_Valid())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_FILEPLAY,NULL,0);
    }
    return AEE_SUCCESS;
}

int YX_API_Record_PlayStop(void)
{
    if(s_yx_voc_is_playing)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP,NULL,0);
    }
    
    return AEE_SUCCESS;
}

int YX_API_SetVocPlayVolume(uint16 vol)
{
    s_yx_voc_play_vol = vol;
    if(s_yx_voc_is_playing)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_VOLUME,NULL,vol);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}

int YX_API_SetVocPlayVolumeUp(void)
{
    if(s_yx_voc_is_playing)
    {
        if(s_yx_voc_play_vol < 9)
        {
            s_yx_voc_play_vol++;
        }
        else
        {
            s_yx_voc_play_vol = 9;
        }
        //htNVMgr_SetYXVocVolume(s_yx_voc_play_vol);
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_VOLUME,NULL,s_yx_voc_play_vol);
        //ISHELL_SendEvent(AEE_GetShell(), AEECLSID_ZDT,ZDT_APP_SIGNAL_YX_VOC_VOLUME,s_yx_voc_play_vol,0);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}

int YX_API_SetVocPlayVolumeDown(void)
{
    if(s_yx_voc_is_playing)
    {
        if(s_yx_voc_play_vol > 1)
        {
            s_yx_voc_play_vol--;
        }
        else
        {
            s_yx_voc_play_vol = 1;
        }
        //htNVMgr_SetYXVocVolume(s_yx_voc_play_vol);
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_VOC_VOLUME,NULL,s_yx_voc_play_vol);
        //ISHELL_SendEvent(AEE_GetShell(), AEECLSID_ZDT,ZDT_APP_SIGNAL_YX_VOC_VOLUME,s_yx_voc_play_vol,0);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}


void YX_API_Record_SetPlayAll(BOOLEAN is_play_all)
{
    s_voc_is_play_all = is_play_all;
    return;
}

BOOLEAN YX_API_Record_FileDelAll(YX_APP_T * pMe)
{
    uint8 i = 0;	
    YX_Voice_GroupRemoveAll(pMe,YX_VCHAT_DEFAULT_GROUP_ID);
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0)
        {
            YX_Voice_GroupRemoveAll(pMe,yx_DB_Friend_Reclist[i].friend_id);
        }
    }
    return TRUE;
}

#endif

