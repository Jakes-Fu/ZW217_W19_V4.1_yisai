
#include "zdt_app.h"
#include "mmifmm_export.h"

#ifdef ZDT_PLAT_YX_SUPPORT
#define YX_ABS(x) (((x)<0) ? -(x) : (x))
#define MAX_FILE_NAME      256

#ifdef YX_DB_NEED_SETTING
#define YX_DB_SET_VERSION_NO 2
static BOOLEAN yx_DB_Set_is_init = FALSE;
YX_DB_SET_Type yx_DB_Set_Rec = {0};
BOOLEAN YX_DB_SET_Default(void)
{
    SCI_MEMSET(&yx_DB_Set_Rec, 0, sizeof(YX_DB_SET_Type));
    SCI_MEMCPY(yx_DB_Set_Rec.reg_ip_domain,"zd.ctxy.cn",strlen("zd.ctxy.cn")); //zd.ctxy.cn:9141
    yx_DB_Set_Rec.reg_ip_port = 9141;
    //SCI_MEMCPY(yx_DB_Set_Rec.ip_domain,"zd.ctxy.cn",strlen("zd.ctxy.cn")); //zd.ctxy.cn:9002
    //yx_DB_Set_Rec.ip_port = 9002;
    SCI_MEMCPY(yx_DB_Set_Rec.app_domain,"zd.ctxy.cn",strlen("zd.ctxy.cn"));
    yx_DB_Set_Rec.app_port = 8080;
    
    yx_DB_Set_Rec.version = YX_DB_SET_VERSION_NO;
    
    yx_DB_Set_Rec.time_interval = 600;
    yx_DB_Set_Rec.heart_interval = 600;
    yx_DB_Set_Rec.net_open = 1;
    yx_DB_Set_Rec.alert_sos_sms = 1;
    yx_DB_Set_Rec.alert_low_bat = 1;
    yx_DB_Set_Rec.in_call_mode = 2;
    yx_DB_Set_Rec.env_mute = 0;
    yx_DB_Set_Rec.env_ring = 1;
    yx_DB_Set_Rec.mo_call_limit = 0;
    yx_DB_Set_Rec.mt_call_limit = 0;
    yx_DB_Set_Rec.sos_mo = 1;
    yx_DB_Set_Rec.sos_mt = 1;
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_SET_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_SET_Type);
    
    if(yx_DB_Set_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_SET_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_SET_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("YX_DB_SET_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_SET_FS_FILE);
        }
        else
        {
            ZDT_LOG("YX_DB_SET_ListInit OK IP=%s, Port=%d",yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        YX_DB_SET_Default();
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SET_FS_FILE,(uint8 *)&yx_DB_Set_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SET_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_SET_FS_FILE,(void *)&yx_DB_Set_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SET_ListInit ERR DB Read");
            YX_DB_SET_Default();
        }
        else
        {
            if(yx_DB_Set_Rec.version != YX_DB_SET_VERSION_NO)
            {
                YX_DB_SET_Default();
                ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SET_FS_FILE,(uint8 *)&yx_DB_Set_Rec,buf_size);
            }
        }
    }
    yx_DB_Set_is_init = TRUE;
    return res;
}

BOOLEAN YX_DB_SET_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SET_Type);
    
    YX_DB_SET_Default();
    if(yx_DB_Set_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SET_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SET_FS_FILE,(uint8 *)&yx_DB_Set_Rec,buf_size);
    return res;
}

BOOLEAN YX_DB_SET_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SET_Type);
    	
    yx_DB_Set_Rec.db_id = 1;
    
    if(yx_DB_Set_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SET_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SET_FS_FILE,(uint8 *)&yx_DB_Set_Rec,buf_size);
    return res;
}

BOOLEAN YX_DB_SET_HaveIP(void)
{
    BOOLEAN res = FALSE;
    
    if(yx_DB_Set_is_init == FALSE)
    {
        YX_DB_SET_ListInit();
    }
    if(strlen(yx_DB_Set_Rec.ip_domain) > 0)
    {
        res = TRUE;
    }
    return res;
}

BOOLEAN YX_DB_SET_ClearIP(void)
{
    BOOLEAN res = TRUE;
    
    if(yx_DB_Set_is_init == FALSE)
    {
        YX_DB_SET_ListInit();
    }
    SCI_MEMSET(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
    YX_DB_SET_ListModify();
    return res;
}
#endif


#ifdef YX_DB_NEED_VOC
static uint32 s_yx_DB_vocrcv_list_length = 0;
static BOOLEAN YX_DB_VocRcv_ListUpdateID(YX_APP_T *pMe,yx_db_vocrcv_index_type *head)
{
	uint16 i = 0;
	yx_db_vocrcv_index_type *temp_index = head;

	if(temp_index == NULL)
	{
		return FALSE;
	}

	for(i = 0; i < pMe->m_yx_db_vocrcv_list_length; i++)
	{
		temp_index->list_id = i;
		temp_index = temp_index->db_index_next;
	}
		
	return TRUE;
}

/**********链表数据操作************************/
BOOLEAN YX_DB_VocRcv_ListInit(YX_APP_T *pMe)
{
    BOOLEAN ret = FALSE;
    char * path = YX_VOICE_RCV_DIR;
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    SFS_ERROR_E		result              = SFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint8 full_char_path[MMIFILE_FULL_PATH_MAX_LEN*3+4] = {0};
    
    pMe->m_yx_db_vocrcv_list_head = NULL;
    pMe->m_yx_db_vocrcv_list_length = 0;
    s_yx_DB_vocrcv_list_length = 0;

   ZDT_LOG("YX_DB_VocRcv_ListInit Start %s",path);
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;
    

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        ZDT_LOG("YX_DB_VocRcv_ListInit NULL");
        return TRUE;
    }
    
    do
    {
        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            uint32 uNameLen = 0;
            uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
            SCI_MEMSET(full_char_path,0,sizeof(full_char_path));
            strcpy(full_char_path,path);
            strcat(full_char_path,"\\");
            full_path_len = strlen(full_char_path);
            tmp_len = GUI_WstrToGB(&full_char_path[full_path_len],sfsfind.name,uNameLen);
            full_path_len += tmp_len;
            i++;
            ZDT_LOG("YX_DB_VocRcv_ListInit %d = %s",i,full_char_path);
            YX_DB_VocRcv_ListInsertByName(pMe,full_char_path);
        }

        result = SFS_FindNextFile(hFind, &sfsfind);
        ZDT_LOG("SFS_FindNextFile result = %d",result);
    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);
    return TRUE;
}

BOOLEAN YX_DB_VocRcv_ListFree(YX_APP_T *pMe)
{
    yx_db_vocrcv_index_type  * curr;
    yx_db_vocrcv_index_type  * prev;
    if(pMe->m_yx_db_vocrcv_list_head == NULL)
    {
        pMe->m_yx_db_vocrcv_list_length = 0;
        s_yx_DB_vocrcv_list_length = 0;
        return TRUE;
    }
    ZDT_LOG("YX_DB_VocRcv_ListFree");
    curr = pMe->m_yx_db_vocrcv_list_head->db_index_prev;
    while(curr != NULL)
    {
        prev = curr->db_index_prev;
        if(curr == pMe->m_yx_db_vocrcv_list_head)
        {
            SCI_FREE(curr);
            break; 
        }
        else
        {
            SCI_FREE(curr);
        }
        curr = prev;
    }
    
    pMe->m_yx_db_vocrcv_list_head = NULL;
    pMe->m_yx_db_vocrcv_list_length = 0;
    s_yx_DB_vocrcv_list_length = 0;
    return TRUE;
}

uint16 YX_DB_VocRcv_ListNumber(YX_APP_T *pMe)
{
    return pMe->m_yx_db_vocrcv_list_length;
}

yx_db_vocrcv_index_type  *YX_DB_VocRcv_ListHead(YX_APP_T *pMe)
{
    return pMe->m_yx_db_vocrcv_list_head;
}

BOOLEAN YX_DB_VocRcv_ListDeleteAll(YX_APP_T *pMe)
{
    ZDT_LOG("DB VocRcv DelAll %ld",pMe->m_yx_db_vocrcv_list_length);
    YX_DB_VocRcv_ListFree(pMe);
    pMe->m_yx_db_vocrcv_list_head = NULL;
    pMe->m_yx_db_vocrcv_list_length = 0;
    s_yx_DB_vocrcv_list_length = 0;
    return TRUE;
}

BOOLEAN YX_DB_VocRcv_ListDeleteOne(YX_APP_T *pMe,uint16 list_id)
{
    yx_db_vocrcv_index_type  *list_temp_curr = NULL;
    yx_db_vocrcv_index_type  *list_temp_prev = NULL;
       	
    if(pMe->m_yx_db_vocrcv_list_head == NULL)
    {
        pMe->m_yx_db_vocrcv_list_length = 0;
        return TRUE;
    }

    if(pMe->m_yx_db_vocrcv_list_length == 1)
    {
        YX_DB_VocRcv_ListDeleteAll(pMe);
        return TRUE;
    }
    
    if(list_id >= pMe->m_yx_db_vocrcv_list_length)/*非法参数*/
    {
        return FALSE;
    }

    ZDT_LOG("DB VocRcv DelOne(%d)",list_id);
	
    /*查找删除接点,并记录删除点的上一接点指针*/
    list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
    while((list_id != list_temp_curr->list_id)
            && (list_temp_curr != NULL))
    {
        list_temp_prev = list_temp_curr;/*删除点的上一点*/
        list_temp_curr = list_temp_curr->db_index_next;
        if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
        {
            break; 
        }
    }

    if(list_id != list_temp_curr->list_id)
    {
        return FALSE;
    }

    /*找到删除点*/
    if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)/*删除点为头接点*/
    {
        list_temp_prev = pMe->m_yx_db_vocrcv_list_head;/*记录原先头结点*/
        pMe->m_yx_db_vocrcv_list_head = pMe->m_yx_db_vocrcv_list_head->db_index_next;
        pMe->m_yx_db_vocrcv_list_head->db_index_prev = list_temp_prev->db_index_prev;
        list_temp_prev->db_index_prev->db_index_next = pMe->m_yx_db_vocrcv_list_head;
    }
    else if(list_temp_curr->db_index_next == pMe->m_yx_db_vocrcv_list_head)/*重新记录当前操作结点为删除点的前一点*/
    {
        list_temp_prev->db_index_next = pMe->m_yx_db_vocrcv_list_head;/*尾结点的下一结点为头接点*/
        pMe->m_yx_db_vocrcv_list_head->db_index_prev = list_temp_prev;
    }
    else
    {
        list_temp_prev->db_index_next = list_temp_curr->db_index_next;
        list_temp_curr->db_index_next->db_index_prev = list_temp_prev;
    }
	
    pMe->m_yx_db_vocrcv_list_length--;
    s_yx_DB_vocrcv_list_length--;
    SCI_FREE(list_temp_curr);/*释放删除的接点*/
    YX_DB_VocRcv_ListUpdateID(pMe,pMe->m_yx_db_vocrcv_list_head);
    return TRUE;
}

	
/*=================================================================================
链表的插入
注意:
1   前提条件在最插入操作前链表已经按时间排序
==================================================================================*/
BOOLEAN YX_DB_VocRcv_ListInsertOne(YX_APP_T *pMe,YX_DB_VOCRCV_Type  *pInBuffer)
{
    uint32  sum = 0;
    yx_db_vocrcv_index_type  *new_list_index = NULL;

    if(pInBuffer == NULL)
    {
        return FALSE;
    }

    new_list_index = (yx_db_vocrcv_index_type *)SCI_ALLOC_APPZ(sizeof(yx_db_vocrcv_index_type));
    if(new_list_index == NULL)
    {
        return FALSE;
    }

    SCI_MEMSET(new_list_index,0x00,sizeof(yx_db_vocrcv_index_type));
    
    SCI_STRCPY((char*)new_list_index->r_name, (char*)pInBuffer->r_name);
    new_list_index->r_time = pInBuffer->r_time;
    new_list_index->voc_idx = pInBuffer->voc_idx;
    if(pMe->m_yx_db_vocrcv_list_head == NULL)/*新消息来前链表为空*/
    {
        new_list_index->list_id = 0;
        pMe->m_yx_db_vocrcv_list_head = new_list_index;
        new_list_index->db_index_next = pMe->m_yx_db_vocrcv_list_head;
        new_list_index->db_index_prev= pMe->m_yx_db_vocrcv_list_head;
        pMe->m_yx_db_vocrcv_list_length = 1;
        s_yx_DB_vocrcv_list_length = 1;
    }
    else
    {
        new_list_index->list_id = pMe->m_yx_db_vocrcv_list_length;
        //添加到表尾
        new_list_index->db_index_prev = pMe->m_yx_db_vocrcv_list_head->db_index_prev;
        new_list_index->db_index_next = pMe->m_yx_db_vocrcv_list_head;

        pMe->m_yx_db_vocrcv_list_head->db_index_prev->db_index_next = new_list_index;
        pMe->m_yx_db_vocrcv_list_head->db_index_prev = new_list_index;

        pMe->m_yx_db_vocrcv_list_length++;
        s_yx_DB_vocrcv_list_length++;
    }
    ZDT_LOG("DB VocRcv Add(%d) 0x%x,%ld,%s",new_list_index->list_id,new_list_index->r_time,new_list_index->voc_idx,new_list_index->r_name);

    YX_DB_VocRcv_ListUpdateID(pMe,pMe->m_yx_db_vocrcv_list_head);
    return TRUE;
}

BOOLEAN YX_DB_VocRcv_ListModifyOne(YX_APP_T *pMe,uint16 list_id,YX_DB_VOCRCV_Type  *pInBuffer)
{
    yx_db_vocrcv_index_type  *list_temp_curr = NULL;
    yx_db_vocrcv_index_type  *list_temp_prev = NULL;
    
    if(pMe->m_yx_db_vocrcv_list_head == NULL)
    {
        pMe->m_yx_db_vocrcv_list_length = 0;
        s_yx_DB_vocrcv_list_length = 0;
        return FALSE;
    }
    
    if(list_id >= pMe->m_yx_db_vocrcv_list_length)/*非法参数*/
    {
        return FALSE;
    }

    ZDT_LOG("DB VocRcv Modify(%d) 0x%x,%ld %s",list_id,pInBuffer->r_time,pInBuffer->voc_idx,pInBuffer->r_time);
	
    /*查找删除接点,并记录删除点的上一接点指针*/
    list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
    while((list_temp_curr != NULL) && list_id != list_temp_curr->list_id)
    {
        list_temp_prev = list_temp_curr;
        list_temp_curr = list_temp_curr->db_index_next;
        if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
        {
            break; 
        }
    }

    if(list_id != list_temp_curr->list_id)
    {
        return FALSE;
    }

    SCI_MEMSET(list_temp_curr->r_name,0x00,MAX_YX_VOC_FILENAME_SIZE);
    SCI_STRCPY((char*)list_temp_curr->r_name, (char*)pInBuffer->r_name);
    list_temp_curr->r_time = pInBuffer->r_time;
    list_temp_curr->voc_idx = pInBuffer->voc_idx;
    return TRUE;
}

BOOLEAN YX_DB_VocRcv_ListInsertByName(YX_APP_T *pMe,char *pInBuffer)
{
    uint16 len = 0;
    char szFileName[MAX_FILE_NAME] = {0};
    char* p;
    YX_DB_VOCRCV_Type cur_data = {0};
    uint32 time_ms = 0;
    uint32 voc_idx = 0;
    
    if(pInBuffer == NULL)
    {
        ZDT_LOG("DB VocRcv Add Err 1");
        return FALSE;
    }
    
    len = SCI_STRLEN(pInBuffer);
    if(len == 0)
    {
        ZDT_LOG("DB VocRcv Add Err 2");
        return FALSE;
    }
    
    
    p = strrchr(pInBuffer, '\\');
    if(p == NULL)
    {
        ZDT_LOG("DB VocRcv Add Err 3");
        return FALSE;
    }
    SCI_STRCPY(szFileName,p+1);
    
    p = strrchr(szFileName, '.');
    if(p == NULL)
    {
        ZDT_LOG("DB VocRcv Add Err 4");
        return FALSE;
    }
   
    if(0 != strncmp(p+1,"amr",3))
    {
        ZDT_LOG("DB VocRcv Add Err 5");
        return FALSE;
    }
    p[0] = 0;
    
    if(szFileName[0] != 'R')
    {
        ZDT_LOG("DB VocRcv Add Err 6");
        return FALSE;
    }
    
    p = strrchr(szFileName, '_');
    if(p == NULL)
    {
        ZDT_LOG("DB VocRcv Add Err 7");
        return FALSE;
    }
    time_ms = (uint32)atof(p+1);

    p[0] = 0;
    voc_idx = (uint32)atof(szFileName+1);
    cur_data.r_time = time_ms;
    cur_data.voc_idx = voc_idx;
    SCI_STRCPY(cur_data.r_name,pInBuffer);
    ZDT_LOG("DB VocRcv Add voc_idx=%ld,time_ms=%ld,FullName=%s",voc_idx,time_ms,pInBuffer);
    YX_DB_VocRcv_ListInsertOne(pMe,&cur_data);
    return TRUE;
}

BOOLEAN YX_DB_VocRcv_ListGetCurList(YX_APP_T *pMe,uint16 list_id, yx_db_vocrcv_index_type  *list)
{
	yx_db_vocrcv_index_type  *list_temp_curr = NULL;

	if(0 == pMe->m_yx_db_vocrcv_list_length)
	{
             ZDT_LOG("DB VocRcv GetCur ERR Len=0");
		SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
		return FALSE;
	}

	if(list_id >= pMe->m_yx_db_vocrcv_list_length)/*非法参数*/
	{
            ZDT_LOG("DB VocRcv GetCur ERR list_id=%d,%d",list_id,pMe->m_yx_db_vocrcv_list_length);
            return FALSE;
	}

	list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
	while((list_id != list_temp_curr->list_id)
		&& (list_temp_curr != NULL))
	{
		list_temp_curr = list_temp_curr->db_index_next;
             if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
             {
                    break; 
             }
	}	

	if(list_id == list_temp_curr->list_id)
	{
		SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
		SCI_MEMCPY((void *)list, list_temp_curr, sizeof(yx_db_vocrcv_index_type));
		return TRUE;
	}
	else
	{
            ZDT_LOG("DB VocRcv GetCur ERR No list_id=%d,%d",list_id,pMe->m_yx_db_vocrcv_list_length);
		return FALSE;
	}
}

BOOLEAN YX_DB_VocRcv_ListGetFirstByTime(YX_APP_T *pMe,yx_db_vocrcv_index_type  *list)
{
    yx_db_vocrcv_index_type  *list_temp_curr = NULL;
    yx_db_vocrcv_index_type  *list_temp_min = NULL;
    uint32 min_time = 0;
    uint32 min_voc_idx = 0;
    uint16 min_list_id = 0;
    uint8 check_err = 0;
    
    if(0 == pMe->m_yx_db_vocrcv_list_length)
    {
        ZDT_LOG("DB VocRcv GetFirst ERR Len=0");
        SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
        return FALSE;
    }

    list_temp_min = list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
    if(list_temp_curr == NULL)
    {
        ZDT_LOG("DB VocRcv GetFirst ERR Head=NULL");
        SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
        return FALSE;
    }

    //获取最早时间的数据
    min_list_id = list_temp_curr->list_id;
    min_time = list_temp_curr->r_time;
    while(list_temp_curr != NULL)
    {
        if(list_temp_curr->r_time < min_time)
        {
            min_time = list_temp_curr->r_time;
            list_temp_min = list_temp_curr;
        }
         list_temp_curr = list_temp_curr->db_index_next;
         if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
         {
                break; 
         }
    }
    
    min_list_id = list_temp_min->list_id;

    //判断是否接收顺序出错，15秒内的数据索引判断
    min_voc_idx = list_temp_min->voc_idx;
    list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
    while(list_temp_curr != NULL)
    {
        //是否索引值比当前小的
        if(list_temp_curr->voc_idx< min_voc_idx)
        {
            int time_offset = list_temp_curr->r_time-min_time;
            time_offset = YX_ABS(time_offset);
            if(time_offset < 15000)
            {
                check_err = 1;
                min_voc_idx = list_temp_curr->voc_idx;
                list_temp_min = list_temp_curr;
            }
        }
         list_temp_curr = list_temp_curr->db_index_next;
         if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
         {
                break; 
         }
    }
    
    if(check_err)
    {
        ZDT_LOG("DB VocRcv GetFirst check_err min_list_id=%d",min_list_id);
        min_list_id = list_temp_min->list_id;
    }
    
    if(YX_DB_VocRcv_ListGetCurList(pMe,min_list_id,list))
    {	
        ZDT_LOG("DB VocRcv GetFirst(%d)Time=%ld,voc_idx=%ld,check_err=%d,%s",min_list_id,list->r_time,list->voc_idx,check_err,list->r_name);
        return TRUE;
    }
    else
    {
        ZDT_LOG("DB VocRcv GetFirst ERR min_list_id=%d",min_list_id);
        return FALSE;
    }	
}

BOOLEAN YX_DB_VocRcv_ListGetLastByTime(YX_APP_T *pMe,yx_db_vocrcv_index_type  *list)
{
    yx_db_vocrcv_index_type  *list_temp_curr = NULL;
    uint32 max_time = 0;
    uint16 max_list_id = 0;
    if(0 == pMe->m_yx_db_vocrcv_list_length)
    {
        SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
        return FALSE;
    }

    list_temp_curr = pMe->m_yx_db_vocrcv_list_head;
    if(list_temp_curr == NULL)
    {
        SCI_MEMSET((void *)list, 0x00, sizeof(yx_db_vocrcv_index_type));
        return FALSE;
    }
    max_time = list_temp_curr->r_time;
    max_list_id = list_temp_curr->list_id;
    while(list_temp_curr != NULL)
    {
        if(list_temp_curr->r_time > max_time)
        {
            max_list_id = list_temp_curr->list_id;
            max_time = list_temp_curr->r_time;
        }
         list_temp_curr = list_temp_curr->db_index_next;
         if(list_temp_curr == pMe->m_yx_db_vocrcv_list_head)
         {
                break; 
         }
    }	

    if(YX_DB_VocRcv_ListGetCurList(pMe,max_list_id,list))
    {	
        return TRUE;
    }
    else
    {
        return FALSE;
    }	
}

BOOLEAN YX_DB_VocRcvAdd(YX_APP_T *pMe,char * file_name)
{
    return YX_DB_VocRcv_ListInsertByName(pMe,file_name);
}

BOOLEAN YX_DB_VocRcvDel(YX_APP_T *pMe,uint16  List_Id)
{
    YX_DB_VocRcv_ListDeleteOne(pMe,List_Id);
    return TRUE;
}

uint32 YX_DB_GetVocRcvNum(void)
{
    return s_yx_DB_vocrcv_list_length;
}
#endif

#ifdef YX_DB_NEED_SOS
static BOOLEAN yx_DB_Sos_is_init = FALSE;
YX_DB_SOS_Type yx_DB_Sos_Reclist[YX_DB_SOS_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_SOS_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
    
    if(yx_DB_Sos_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_SOS_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_SOS_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_SOS_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_SOS_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Sos_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SOS_FS_FILE,(uint8 *)yx_DB_Sos_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SOS_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_SOS_FS_FILE,(void *)yx_DB_Sos_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SOS_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Sos_is_init = TRUE;
    }
    return res;
}

BOOLEAN YX_DB_SOS_ListModify(uint8 sos_idx,YX_DB_SOS_Type * pData)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
    
    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListModify(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Sos_Reclist[sos_idx] = *pData;
    yx_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;    
        
    if(yx_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListModify(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SOS_FS_FILE,(uint8 *)yx_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListModify(%d) ERR DB Write",sos_idx);
        return FALSE;
    }		
    return TRUE;
}

static BOOLEAN YX_DB_Change_SOS_NV(
                                                             uint8 index,
                                                             uint8* number_ptr
                                                            )
{
    #if 0
    if(index == 0)
    {
        MMIAPISet_SosCall_Num1(number_ptr);
    }
    else if(index == 1)
    {
        MMIAPISet_SosCall_Num2(number_ptr);
    }
    else if(index == 2)
    {
        MMIAPISet_SosCall_Num3(number_ptr);
    }
    else if(index == 3)
    {
        MMIAPISet_SosCall_Num4(number_ptr);
    }
    else if(index == 4)
    {
        MMIAPISet_SosCall_Num5(number_ptr);
    }
    #endif
    return TRUE;
}

BOOLEAN YX_DB_SOS_ListModifyBuf(uint8 sos_idx,YX_DB_SOS_Type * pData)
{
    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListModifyBuf(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    YX_DB_Change_SOS_NV(sos_idx,pData->sos_num);	
    yx_DB_Sos_Reclist[sos_idx] = *pData;
    yx_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;    
    return TRUE;
}

BOOLEAN YX_DB_SOS_ListDel(uint8  sos_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);

    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListDel(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Sos_Reclist[sos_idx],0x00,sizeof(YX_DB_SOS_Type));
    
    if(yx_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListDel(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SOS_FS_FILE,(uint8 *)yx_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListDel(%d) ERR DB Write",sos_idx);
        return FALSE;
    }		

    return TRUE;
}

BOOLEAN YX_DB_SOS_ListDelBuf(uint8  sos_idx)
{
    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListDelBuf(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    YX_DB_Change_SOS_NV(sos_idx,"");	
    SCI_MEMSET(&yx_DB_Sos_Reclist[sos_idx],0x00,sizeof(YX_DB_SOS_Type));
    
    return TRUE;
}
BOOLEAN YX_DB_SOS_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
    uint8 i = 0; 
    
    SCI_MEMSET(yx_DB_Sos_Reclist,0x00,sizeof(YX_DB_SOS_Type)* YX_DB_SOS_MAX_SUM);
    
    if(yx_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        YX_DB_Change_SOS_NV(i,"");	
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SOS_FS_FILE,(uint8 *)yx_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_SOS_ListDelAllBuf(void)
{
    uint8 i = 0; 
    SCI_MEMSET(yx_DB_Sos_Reclist,0x00,sizeof(YX_DB_SOS_Type)* YX_DB_SOS_MAX_SUM);
    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        YX_DB_Change_SOS_NV(i,"");	
    }
    return TRUE;
}

BOOLEAN YX_DB_SOS_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
        
    if(yx_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SOS_FS_FILE,(uint8 *)yx_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

int YX_DB_SOS_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint8  sos_num[YX_DB_SOS_MAX_NUMBER_SIZE+1] = {0};
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);

    YX_DB_SOS_ListInit();
    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        if(yx_DB_Sos_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(sos_num,0,sizeof(sos_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_Sos_Reclist[i].sos_num,SCI_STRLEN(yx_DB_Sos_Reclist[i].sos_num),sos_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(sos_num,r_num,r_len) == 0)
                {
                    return yx_DB_Sos_Reclist[i].db_id;
                }
            }
        }
    }
    return -1;
}

BOOLEAN YX_DB_SOS_have_num(void)
{
    uint8 i = 0;	
    uint8 sos_idx = 0;	
	
    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        if(yx_DB_Sos_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Sos_Reclist[i].sos_num) > 0)
        {
            sos_idx = i+1;
            break;
        }
    }
    if (sos_idx == 0)
	return FALSE;
    else
	return TRUE;
}


#endif

#ifdef YX_DB_NEED_WHITE
static BOOLEAN yx_DB_White_is_init = FALSE;
YX_DB_WHITE_Type yx_DB_White_Reclist[YX_DB_WHITE_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_WHITE_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);
    
    if(yx_DB_White_is_init == TRUE)
    {
        return TRUE;
    }
    
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_WHITE_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_WHITE_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_WHITE_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_WHITE_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_White_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(uint8 *)yx_DB_White_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_WHITE_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(void *)yx_DB_White_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_WHITE_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_White_is_init = TRUE;
    }
    return res;
}

BOOLEAN YX_DB_WHITE_ListModify(uint8 white_idx,YX_DB_WHITE_Type * pData)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);
    
    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListModify(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    yx_DB_White_Reclist[white_idx] = *pData;
    yx_DB_White_Reclist[white_idx].db_id = white_idx+1;
    
    if(yx_DB_White_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListModify(%d) ERR NoInit",white_idx);
        return FALSE;
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(uint8 *)yx_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListModify(%d) ERR DB Write",white_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_WHITE_ListModifyBuf(uint8 white_idx,YX_DB_WHITE_Type * pData)
{    
    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListModifyBuf(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    yx_DB_White_Reclist[white_idx] = *pData;
    yx_DB_White_Reclist[white_idx].db_id = white_idx+1;    
    return TRUE;
}

BOOLEAN YX_DB_WHITE_ListDel(uint8  white_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);

    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_White_Reclist[white_idx],0x00,sizeof(YX_DB_WHITE_Type));
    
    if(yx_DB_White_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListDel(%d) ERR NoInit",white_idx);
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(uint8 *)yx_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListDel(%d) ERR DB Write",white_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_WHITE_ListDelBuf(uint8  white_idx)
{
    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_White_Reclist[white_idx],0x00,sizeof(YX_DB_WHITE_Type));
    
    return TRUE;
}
BOOLEAN YX_DB_WHITE_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);
    
    SCI_MEMSET(yx_DB_White_Reclist,0x00,sizeof(YX_DB_WHITE_Type)* YX_DB_WHITE_MAX_SUM);
    
    if(yx_DB_White_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(uint8 *)yx_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListDelAllERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_WHITE_ListDelAllBuf(void)
{
    SCI_MEMSET(yx_DB_White_Reclist,0x00,sizeof(YX_DB_WHITE_Type)* YX_DB_WHITE_MAX_SUM);
    return TRUE;
}

BOOLEAN YX_DB_WHITE_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);
        
    if(yx_DB_White_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_WHITE_FS_FILE,(uint8 *)yx_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_WHITE_ListSaveBuf DB Write");
        return FALSE;
    }		
    return TRUE;
}

static BOOLEAN YX_DB_WHITE_TimeIsValid(uint32 start,uint32 end,uint32 cur)
{
    BOOLEAN ret = FALSE;
    
    if(start > end)
    {
        if(cur > end && cur < start)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        if(cur > start && cur < end)
        {
            return TRUE;
        }
    }
    return ret;

}

static BOOLEAN YX_DB_WHITE_WeekIsValid(uint8 type, uint8 cur)
{
    BOOLEAN ret = FALSE;
    
    if(cur > 6)
    {
        return FALSE;
    }
    
    if(((0x01<<cur) & type) > 0)
    {
        ret = TRUE;
    }

    return ret;

}

int YX_DB_WHITE_IsValidNumberSMS(char * num)
{
    int ret = -1;
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint32 cur_daysecond = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    YX_DB_WHITE_ListInit();
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(white_num,0,sizeof(white_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_White_Reclist[i].white_num,SCI_STRLEN(yx_DB_White_Reclist[i].white_num),white_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(white_num,r_num,r_len) == 0)
                {
                    if(YX_DB_WHITE_WeekIsValid(yx_DB_Set_Rec.in_call_cycle,date.wday) 
                        && (YX_DB_WHITE_TimeIsValid(yx_DB_White_Reclist[i].time1_start,yx_DB_White_Reclist[i].time1_end,cur_daysecond)
                            || YX_DB_WHITE_TimeIsValid(yx_DB_White_Reclist[i].time2_start,yx_DB_White_Reclist[i].time2_end,cur_daysecond))
                        )
                    {
                        ret = yx_DB_White_Reclist[i].db_id;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}

int YX_DB_WHITE_IsValidNumber(char * num)
{
    int ret = -1;
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    
    YX_DB_WHITE_ListInit();
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(white_num,0,sizeof(white_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_White_Reclist[i].white_num,SCI_STRLEN(yx_DB_White_Reclist[i].white_num),white_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(white_num,r_num,r_len) == 0)
                {
                    ret = yx_DB_White_Reclist[i].db_id;
                    break;
                }
            }
        }
    }
    return ret;
}

int YX_DB_WHITE_IsValidNumberExt(char * num)
{
    int ret = -1;
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint32 cur_daysecond = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(yx_DB_Set_Rec.in_call_mode == 1)
    {
        return 1;
    }
    else if(yx_DB_Set_Rec.in_call_mode == 3)
    {
        return -1;
    }
    if(YX_DB_White_have_num() == FALSE)
    {
        return -1;
    }
    
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    if(YX_DB_SPD_IsValidNumber(num) > 0)
    {
        return 1;
    }
    
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    YX_DB_WHITE_ListInit();
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(white_num,0,sizeof(white_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_White_Reclist[i].white_num,SCI_STRLEN(yx_DB_White_Reclist[i].white_num),white_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(white_num,r_num,r_len) == 0)
                {
                    if(YX_DB_WHITE_WeekIsValid(yx_DB_Set_Rec.in_call_cycle,date.wday) 
                        && (YX_DB_WHITE_TimeIsValid(yx_DB_White_Reclist[i].time1_start,yx_DB_White_Reclist[i].time1_end,cur_daysecond)
                            || YX_DB_WHITE_TimeIsValid(yx_DB_White_Reclist[i].time2_start,yx_DB_White_Reclist[i].time2_end,cur_daysecond))
                        )
                    {
                        ret = yx_DB_White_Reclist[i].db_id;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}

BOOLEAN YX_DB_WHITE_DeleteNumber(char * num)
{
    BOOLEAN res = FALSE;
    uint16 num_len = 0;
    uint16 i = 0;
    uint16 db_num_len = 0;
    uint16 name_len = 0;
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL)
    {
        return FALSE;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return FALSE;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    
    YX_DB_WHITE_ListInit();
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(white_num,0,sizeof(white_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_White_Reclist[i].white_num,SCI_STRLEN(yx_DB_White_Reclist[i].white_num),white_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(white_num,r_num,r_len) == 0)
                {
                    SCI_MEMSET(&yx_DB_White_Reclist[i],0,sizeof(YX_DB_WHITE_Type));
                    res = TRUE;
                }
            }
        }
    }
    return res;
}

int YX_DB_WHITE_GetNumberIndex(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    
    YX_DB_WHITE_ListInit();
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(white_num,0,sizeof(white_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_White_Reclist[i].white_num,SCI_STRLEN(yx_DB_White_Reclist[i].white_num),white_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(white_num,r_num,r_len) == 0)
                {
                    return i+1;
                }
            }
        }
    }
    
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id == 0)
        {
            return i+1;
        }
    }
    return -1;
}

int YX_DB_WHITE_AddNumber(char * num,YX_DB_WHITE_Type * pData)
{
    int ret = -1;
    ret = YX_DB_WHITE_GetNumberIndex(num);
    if(ret > 0)
    {
        YX_DB_WHITE_ListModifyBuf(ret-1,pData);
    }
    return ret;
}

BOOLEAN YX_DB_White_have_num(void)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_White_Reclist[i].white_num) > 0)
        {
            idx = i+1;
            break;
        }
    }
    if (idx == 0)
	return FALSE;
    else
	return TRUE;
}
#endif

#ifdef YX_DB_NEED_CLOCK

static BOOLEAN yx_DB_Clock_is_init = FALSE;
YX_DB_CLOCK_Type yx_DB_Clock_Reclist[YX_DB_CLOCK_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_CLOCK_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * YX_DB_CLOCK_MAX_SUM);
    
    if(yx_DB_Clock_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_CLOCK_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_CLOCK_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_CLOCK_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_CLOCK_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Clock_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(uint8 *)yx_DB_Clock_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_CLOCK_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(void *)yx_DB_Clock_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_CLOCK_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Clock_is_init = TRUE;
    }
    return TRUE;
}

BOOLEAN YX_DB_CLOCK_ListModify(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * YX_DB_CLOCK_MAX_SUM);
    
    if (clock_idx >= YX_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("YX_DB_CLOCK_ListModify(%d) ERR idx>=%d",clock_idx,YX_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Clock_Reclist[clock_idx].db_id = clock_idx+1;
    yx_DB_Clock_Reclist[clock_idx].clock_id = clock_id;
    yx_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    yx_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    yx_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    yx_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    if(pConten != PNULL && content_len > 0)
    {
        if(content_len > YX_DB_CLOCK_MAX_CONTENT_SIZE)
        {
            content_len = YX_DB_CLOCK_MAX_CONTENT_SIZE;
            SCI_MEMCPY(yx_DB_Clock_Reclist[clock_idx].clock_content,pConten,content_len);
        }
    }
    
    if(yx_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListModify(%d) ERR NoInit",clock_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(uint8 *)yx_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListModify(%d) ERR DB Write",clock_idx);
    }
    return res;
}

BOOLEAN YX_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len)
{    
    if (clock_idx >= YX_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("YX_DB_CLOCK_ListModifyBuf(%d) ERR idx>=%d",clock_idx,YX_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Clock_Reclist[clock_idx].db_id = clock_idx+1;
    yx_DB_Clock_Reclist[clock_idx].clock_id = clock_id;
    yx_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    yx_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    yx_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    yx_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    if(pConten != PNULL && content_len > 0)
    {
        if(content_len > YX_DB_CLOCK_MAX_CONTENT_SIZE)
        {
            content_len = YX_DB_CLOCK_MAX_CONTENT_SIZE;
            SCI_MEMSET(yx_DB_Clock_Reclist[clock_idx].clock_content,0,YX_DB_CLOCK_MAX_CONTENT_SIZE+1);
            SCI_MEMCPY(yx_DB_Clock_Reclist[clock_idx].clock_content,pConten,content_len);
        }
    }
    
    return TRUE;
}
BOOLEAN YX_DB_CLOCK_ListDel(uint8  clock_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * YX_DB_CLOCK_MAX_SUM);

    if (clock_idx >= YX_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDel(%d) ERR idx>=%d",clock_idx,YX_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Clock_Reclist[clock_idx],0x00,sizeof(YX_DB_CLOCK_Type));
    
    if(yx_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDel(%d) ERR NoInit",clock_idx);
        return FALSE;
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(uint8 *)yx_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDel(%d) ERR DB Write",clock_idx);
        return FALSE;
    }
    return TRUE;
}

BOOLEAN YX_DB_CLOCK_ListDelBuf(uint8  clock_idx)
{
    if (clock_idx >= YX_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDel(%d) ERR idx>=%d",clock_idx,YX_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Clock_Reclist[clock_idx],0x00,sizeof(YX_DB_CLOCK_Type));
    
    return TRUE;
}

BOOLEAN YX_DB_CLOCK_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * YX_DB_CLOCK_MAX_SUM);
    
    SCI_MEMSET(yx_DB_Clock_Reclist,0x00,sizeof(YX_DB_CLOCK_Type)* YX_DB_CLOCK_MAX_SUM);
    
    if(yx_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(uint8 *)yx_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListDelAllERR DB Write");
        return FALSE;
    }
    return TRUE;
}

BOOLEAN YX_DB_CLOCK_ListDelAllBuf(void)
{    
    SCI_MEMSET(yx_DB_Clock_Reclist,0x00,sizeof(YX_DB_CLOCK_Type)* YX_DB_CLOCK_MAX_SUM);
    
    return TRUE;
}

BOOLEAN YX_DB_CLOCK_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * YX_DB_CLOCK_MAX_SUM);
        
    if(yx_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_CLOCK_FS_FILE,(uint8 *)yx_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListSaveBuf DB Write");
        return FALSE;
    }
    return TRUE;
}

#endif

#ifdef YX_DB_NEED_TIMER
static BOOLEAN yx_DB_Timer_is_init = FALSE;
YX_DB_TIMER_Type yx_DB_Timer_Reclist[YX_DB_TIMER_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_TIMER_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_TIMER_Type);
    uint32 buf_array_size = (buf_size * YX_DB_TIMER_MAX_SUM);
    
    if(yx_DB_Timer_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_TIMER_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_TIMER_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_TIMER_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_TIMER_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Timer_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_TIMER_FS_FILE,(uint8 *)yx_DB_Timer_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_TIMER_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_TIMER_FS_FILE,(void *)yx_DB_Timer_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_TIMER_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Timer_is_init = TRUE;
    }
    return TRUE;
}

BOOLEAN YX_DB_TIMER_ListModify(uint8 timer_idx,uint8 time_set,uint32 time_start,uint32 time_end)
{    
    if (timer_idx >= YX_DB_TIMER_MAX_SUM)
    {
        ZDT_LOG("YX_DB_TIMER_ListModify(%d) ERR idx>=%d",timer_idx,YX_DB_TIMER_MAX_SUM);
        return FALSE;
    }
    ZDT_LOG("YX_DB_TIMER_ListModify timer_idx=%d, time_set=0x%02x, time_start=%d, time_end=%d",timer_idx,time_set,time_start,time_end);	
    yx_DB_Timer_Reclist[timer_idx].db_id = timer_idx+1;
    yx_DB_Timer_Reclist[timer_idx].time_set = time_set;
    yx_DB_Timer_Reclist[timer_idx].time_start = time_start;
    yx_DB_Timer_Reclist[timer_idx].time_end = time_end;
    return TRUE;
}

BOOLEAN YX_DB_TIMER_ListDel(uint8  timer_idx)
{
    if (timer_idx >= YX_DB_TIMER_MAX_SUM)
    {
        ZDT_LOG("YX_DB_TIMER_ListDel(%d) ERR idx>=%d",timer_idx,YX_DB_TIMER_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Timer_Reclist[timer_idx],0x00,sizeof(YX_DB_TIMER_Type));
    return TRUE;
}

BOOLEAN YX_DB_TIMER_ListDelAll(void)
{    
    SCI_MEMSET(yx_DB_Timer_Reclist,0x00,sizeof(YX_DB_TIMER_Type)* YX_DB_TIMER_MAX_SUM);
    return TRUE;
}

BOOLEAN YX_DB_TIMER_ListSave(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_TIMER_Type);
    uint32 buf_array_size = (buf_size * YX_DB_TIMER_MAX_SUM);
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_TIMER_FS_FILE,(uint8 *)yx_DB_Timer_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListSaveBuf DB Write");
        return FALSE;
    }
    
    return TRUE;
}

static BOOLEAN YX_DB_TIMER_TimeIsValid(uint32 start,uint32 end,uint32 cur)
{
    BOOLEAN ret = FALSE;
    
    if(start > end)
    {
        if(cur > end && cur < start)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        if(cur > start && cur < end)
        {
            return TRUE;
        }
    }
    return ret;

}

static BOOLEAN YX_DB_TIMER_WeekIsValid(uint8 type, uint8 cur)
{
    BOOLEAN ret = FALSE;
    if((type&0x01) == 0)
    {
        return FALSE;
    }
    
    if(cur > 6)
    {
        return FALSE;
    }

    cur++;
    
    if(((0x01<<cur) & type) > 0)
    {
        ret = TRUE;
    }

    return ret;

}

BOOLEAN YX_DB_TIMER_IsValid(uint16 idx)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = idx;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    if(i < YX_DB_TIMER_MAX_SUM)
    {
        if(yx_DB_Timer_Reclist[i].db_id != 0)
        {
            if(YX_DB_TIMER_WeekIsValid(yx_DB_Timer_Reclist[i].time_set,date.wday) 
                && YX_DB_TIMER_TimeIsValid(yx_DB_Timer_Reclist[i].time_start,yx_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
            }
        }
    }	
    return is_valid;
}

BOOLEAN YX_DB_TIMER_IsValid_Slient(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = YX_TIME_TYPE_SLIENT_1; i <  YX_TIME_TYPE_SLIENT_MAX; i++)
    {
        if(yx_DB_Timer_Reclist[i].db_id != 0)
        {
            if(YX_DB_TIMER_WeekIsValid(yx_DB_Timer_Reclist[i].time_set,date.wday) 
                && YX_DB_TIMER_TimeIsValid(yx_DB_Timer_Reclist[i].time_start,yx_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
                break;
            }
        }
    }	
    return is_valid;
}

BOOLEAN YX_DB_TIMER_IsValid_Class(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = YX_TIME_TYPE_CLASS_1; i <  YX_TIME_TYPE_CLASS_MAX; i++)
    {
        if(yx_DB_Timer_Reclist[i].db_id != 0)
        {
            if(YX_DB_TIMER_WeekIsValid(yx_DB_Timer_Reclist[i].time_set,date.wday) 
                && YX_DB_TIMER_TimeIsValid(yx_DB_Timer_Reclist[i].time_start,yx_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
                break;
            }
        }
    }	
    return is_valid;
}

BOOLEAN YX_DB_TIMER_IsValid_Sleep(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = YX_TIME_TYPE_SLEEP_1; i <  YX_TIME_TYPE_SLEEP_MAX; i++)
    {
        if(yx_DB_Timer_Reclist[i].db_id != 0)
        {
            if(YX_DB_TIMER_WeekIsValid(yx_DB_Timer_Reclist[i].time_set,date.wday) 
                && YX_DB_TIMER_TimeIsValid(yx_DB_Timer_Reclist[i].time_start,yx_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
                break;
            }
        }
    }	
    return is_valid;
}
#endif

#ifdef YX_DB_NEED_SPD
static BOOLEAN yx_DB_Spd_is_init = FALSE;
YX_DB_SPD_Type yx_DB_Spd_Reclist[YX_DB_SPD_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_SPD_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SPD_MAX_SUM);
    
    if(yx_DB_Spd_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_SPD_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_SPD_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_SPD_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_SPD_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Spd_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SPD_FS_FILE,(uint8 *)yx_DB_Spd_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SPD_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_SPD_FS_FILE,(void *)yx_DB_Spd_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_SPD_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Spd_is_init = TRUE;
    }
    return res;
}

BOOLEAN YX_DB_SPD_ListModify(uint8 spd_idx,YX_DB_SPD_Type * pData)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SPD_MAX_SUM);
    
    if (spd_idx >= YX_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SPD_ListModify(%d) ERR idx>=%d",spd_idx,YX_DB_SPD_MAX_SUM);
        return FALSE;
    }
    yx_DB_Spd_Reclist[spd_idx] = *pData;
    yx_DB_Spd_Reclist[spd_idx].db_id = spd_idx+1;
        
    if(yx_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListModify(%d) ERR NoInit",spd_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SPD_FS_FILE,(uint8 *)yx_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListModify(%d) ERR DB Write",spd_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_SPD_ListModifyBuf(uint8 spd_idx,YX_DB_SPD_Type * pData)
{
    if (spd_idx >= YX_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SPD_ListModifyBuf(%d) ERR idx>=%d",spd_idx,YX_DB_SPD_MAX_SUM);
        return FALSE;
    }
    YX_DB_Change_SOS_NV(spd_idx+1,pData->spd_num);	
    yx_DB_Spd_Reclist[spd_idx] = *pData;
    yx_DB_Spd_Reclist[spd_idx].db_id = spd_idx+1;
        
    return TRUE;
}

BOOLEAN YX_DB_SPD_ListDel(uint8  spd_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SPD_MAX_SUM);

    if (spd_idx >= YX_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SPD_ListDel(%d) ERR idx>=%d",spd_idx,YX_DB_SPD_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Spd_Reclist[spd_idx],0x00,sizeof(YX_DB_SPD_Type));
    
    if(yx_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListDel(%d) ERR NoInit",spd_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SPD_FS_FILE,(uint8 *)yx_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListDel(%d) ERR DB Write",spd_idx);
        return FALSE;
    }		

    return TRUE;
}

BOOLEAN YX_DB_SPD_ListDelBuf(uint8  spd_idx)
{
    if (spd_idx >= YX_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SPD_ListDelBuf(%d) ERR idx>=%d",spd_idx,YX_DB_SPD_MAX_SUM);
        return FALSE;
    }
    YX_DB_Change_SOS_NV(spd_idx+1,"");
    SCI_MEMSET(&yx_DB_Spd_Reclist[spd_idx],0x00,sizeof(YX_DB_SPD_Type));
    
    return TRUE;
}

BOOLEAN YX_DB_SPD_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SPD_MAX_SUM);
    
    SCI_MEMSET(yx_DB_Spd_Reclist,0x00,sizeof(YX_DB_SPD_Type)* YX_DB_SPD_MAX_SUM);
    
    if(yx_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SPD_FS_FILE,(uint8 *)yx_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_SPD_ListDelAllBuf(void)
{
    SCI_MEMSET(yx_DB_Spd_Reclist,0x00,sizeof(YX_DB_SPD_Type)* YX_DB_SPD_MAX_SUM);
    return TRUE;
}

BOOLEAN YX_DB_SPD_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SPD_MAX_SUM);
        
    if(yx_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_SPD_FS_FILE,(uint8 *)yx_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_SPD_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

int YX_DB_SPD_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint8  spd_num[YX_DB_SPD_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    
    YX_DB_SPD_ListInit();
    for(i = 0; i < YX_DB_SPD_MAX_SUM; i++)
    {
        if(yx_DB_Spd_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(spd_num,0,sizeof(spd_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_Spd_Reclist[i].spd_num,SCI_STRLEN(yx_DB_Spd_Reclist[i].spd_num),spd_num);
            if(db_num_len > 0 &&  r_len == db_num_len)
            {
                if(strncmp(spd_num,r_num,r_len) == 0)
                {
                    return yx_DB_Spd_Reclist[i].db_id;
                }
            }
        }
    }
    return -1;
}

BOOLEAN YX_DB_SPD_have_num(void)
{
    uint8 i = 0;	
    uint8 spd_idx = 0;	
	
    for(i = 0; i < YX_DB_SPD_MAX_SUM; i++)
    {
        if(yx_DB_Spd_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Spd_Reclist[i].spd_num) > 0)
        {
            spd_idx = i+1;
            break;
        }
    }
    if (spd_idx == 0)
	return FALSE;
    else
	return TRUE;
}
#endif

#ifdef ZDT_BH_W9_FEATRUE
YX_PB_LIST_Type yx_PbList_Reclist[15];
BOOLEAN num_is_sos(char* num)
{
	uint8 i=0;
	for(i=0;i<YX_DB_SOS_MAX_SUM;i++)
	{
		if(yx_DB_Sos_Reclist[i].db_id != 0 && 0==strcmp((char *)num,yx_DB_Sos_Reclist[i].sos_num))
		{
			return TRUE;
		}
	}
	return FALSE;
}
uint16 Get_Pb_List(void)
{
	uint8 i=0;
	uint8 j=0;
	uint16 count=0;
	SCI_MEMSET(&yx_PbList_Reclist, 0, sizeof(yx_PbList_Reclist));
	for(i=0;i<YX_DB_SOS_MAX_SUM;i++)
	{
		if(yx_DB_Sos_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Sos_Reclist[i].sos_num) > 0)
		{
			SCI_MEMCPY(&yx_PbList_Reclist[count],&yx_DB_Sos_Reclist[i],sizeof(yx_DB_Sos_Reclist[i]));
			count++;
		}
	}
	for(i=0;i<YX_DB_SPD_MAX_SUM;i++)
	{
		if(yx_DB_Spd_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Spd_Reclist[i].spd_num) > 0)
		{
			if(!num_is_sos(yx_DB_Spd_Reclist[i].spd_num))
			{
                           SCI_MEMCPY(&yx_PbList_Reclist[count],&yx_DB_Spd_Reclist[i],sizeof(yx_DB_Spd_Reclist[i]));
                           count++;
			}
		}
	}
	return count;
}
#endif
#ifdef YX_DB_NEED_NETSMS
static BOOLEAN yx_DB_NetSms_is_init = FALSE;
YX_DB_NETSMS_Type yx_DB_NetSms_Reclist[YX_DB_NETSMS_MAX_SUM] = {0};
uint8 yx_DB_NetSms_list_index[YX_DB_NETSMS_MAX_SUM] = {0};
extern uint8 g_cur_mesage_index;

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_NETSMS_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);
    
    if(yx_DB_NetSms_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_NETSMS_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_NETSMS_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_NETSMS_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_NETSMS_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_NetSms_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_NETSMS_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(void *)yx_DB_NetSms_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_NETSMS_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_NetSms_is_init = TRUE;
    }
    YX_DB_NETSMS_ReOrderIndex();
    return res;
}

BOOLEAN YX_DB_NETSMS_ListModify(uint8 netsms_idx,YX_DB_NETSMS_Type * netsms_ptr)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);
    
    if (netsms_idx >= YX_DB_NETSMS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR idx>=%d",netsms_idx,YX_DB_NETSMS_MAX_SUM);
        return FALSE;
    }
    if(netsms_ptr == PNULL)
    {
        return FALSE;
    }
    	
    SCI_MEMSET(&yx_DB_NetSms_Reclist[netsms_idx],0x00,sizeof(YX_DB_NETSMS_Type));
    SCI_MEMCPY(&yx_DB_NetSms_Reclist[netsms_idx],netsms_ptr,sizeof(YX_DB_NETSMS_Type));
    yx_DB_NetSms_Reclist[netsms_idx].db_id = netsms_idx+1;
    
    if(yx_DB_NetSms_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR NoInit",netsms_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR DB Write",netsms_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_NETSMS_ListModifyBuf(uint8 netsms_idx,YX_DB_NETSMS_Type * netsms_ptr)
{
    if (netsms_idx >= YX_DB_NETSMS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModifyBuf(%d) ERR idx>=%d",netsms_idx,YX_DB_NETSMS_MAX_SUM);
        return FALSE;
    }
    	
    if(netsms_ptr == PNULL)
    {
        return FALSE;
    }
    	
    SCI_MEMSET(&yx_DB_NetSms_Reclist[netsms_idx],0x00,sizeof(YX_DB_NETSMS_Type));
    SCI_MEMCPY(&yx_DB_NetSms_Reclist[netsms_idx],netsms_ptr,sizeof(YX_DB_NETSMS_Type));
    yx_DB_NetSms_Reclist[netsms_idx].db_id = netsms_idx+1;
        
    return TRUE;
}

BOOLEAN YX_DB_NETSMS_ListDel(uint8  netsms_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);

    if (netsms_idx >= YX_DB_NETSMS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDel(%d) ERR idx>=%d",netsms_idx,YX_DB_NETSMS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_NetSms_Reclist[netsms_idx],0x00,sizeof(YX_DB_NETSMS_Type));
    YX_DB_NETSMS_ReOrderIndex();
    if(yx_DB_NetSms_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDel(%d) ERR NoInit",netsms_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDel(%d) ERR DB Write",netsms_idx);
        return FALSE;
    }		

    return TRUE;
}

BOOLEAN YX_DB_NETSMS_ListDelBuf(uint8  netsms_idx)
{
    if (netsms_idx >= YX_DB_NETSMS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDelBuf(%d) ERR idx>=%d",netsms_idx,YX_DB_NETSMS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_NetSms_Reclist[netsms_idx],0x00,sizeof(YX_DB_NETSMS_Type));
    YX_DB_NETSMS_ReOrderIndex();
    return TRUE;
}
BOOLEAN YX_DB_NETSMS_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);
    
    SCI_MEMSET(yx_DB_NetSms_Reclist,0x00,sizeof(YX_DB_NETSMS_Type)* YX_DB_NETSMS_MAX_SUM);
    
    if(yx_DB_NetSms_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDelAll ERR NoInit");
        return FALSE;
    }
    YX_DB_NETSMS_ReOrderIndex();
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_NETSMS_ListDelAllBuf(void)
{
    SCI_MEMSET(yx_DB_NetSms_Reclist,0x00,sizeof(YX_DB_NETSMS_Type)* YX_DB_NETSMS_MAX_SUM);
    YX_DB_NETSMS_ReOrderIndex();
    return TRUE;
}

BOOLEAN YX_DB_NETSMS_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);
        
    if(yx_DB_NetSms_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_Have_NETSMS_NoRead(void)
{
    BOOLEAN res = FALSE;
    uint8 i;
    for(i=0;i<YX_DB_NETSMS_MAX_SUM;i++)
    {
    	  if(yx_DB_NetSms_Reclist[i].db_id != 0&&0==yx_DB_NetSms_Reclist[i].is_read)
    	  {
    	  	res = TRUE;
		break;
    	  }
    }
    SCI_TRACE_LOW("W9P_LOG:____MSG res =%d", res);
    return res;
}


BOOLEAN YX_DB_NETSMS_ChangStatusToRead(uint8 netsms_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_NETSMS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_NETSMS_MAX_SUM);
    
    if (netsms_idx >= YX_DB_NETSMS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR idx>=%d",netsms_idx,YX_DB_NETSMS_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_NetSms_Reclist[netsms_idx].is_read = 1;
    YX_DB_NETSMS_ReOrderIndex();
    if(yx_DB_NetSms_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR NoInit",netsms_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_NETSMS_FS_FILE,(uint8 *)yx_DB_NetSms_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_NETSMS_ListModify(%d) ERR DB Write",netsms_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_NETSMS_GetNewNum(uint16 * pIdx)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 netsms_idx = 0;
    BOOLEAN have_netsms = FALSE;
    uint32 last_rcv_time = 0;
	
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0 && yx_DB_NetSms_Reclist[i].netsms_rcv_time >  last_rcv_time)
        {
            last_rcv_time = yx_DB_NetSms_Reclist[i].netsms_rcv_time;
            netsms_idx = i;
            have_netsms = TRUE;
        }
    }
    *pIdx = netsms_idx;
    
    if (have_netsms)
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN YX_DB_NETSMS_GetNewUnReadNum(uint16 * pIdx)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 netsms_idx = 0;
    BOOLEAN have_netsms = FALSE;
    uint32 last_rcv_time = 0;
	
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0 && yx_DB_NetSms_Reclist[i].is_read == 0 && yx_DB_NetSms_Reclist[i].netsms_rcv_time >  last_rcv_time)
        {
            last_rcv_time = yx_DB_NetSms_Reclist[i].netsms_rcv_time;
            netsms_idx = i;
            have_netsms = TRUE;
        }
    }
    
    *pIdx = netsms_idx;
    
    if(have_netsms == FALSE)
    {
        have_netsms = YX_DB_NETSMS_GetNewNum(pIdx);
    }
    return have_netsms;
}

BOOLEAN YX_DB_NETSMS_GetOldNum(uint16 * pIdx)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 netsms_idx = 0;
    BOOLEAN have_netsms = FALSE;
    uint32 last_rcv_time = 0xFFFFFFFF;
	
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0 
            && yx_DB_NetSms_Reclist[i].is_read
            && yx_DB_NetSms_Reclist[i].netsms_rcv_time <  last_rcv_time)
        {
            last_rcv_time = yx_DB_NetSms_Reclist[i].netsms_rcv_time;
            netsms_idx = i;
            have_netsms = TRUE;
        }
    }
    if(have_netsms == FALSE)
    {
        for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
        {
            if(yx_DB_NetSms_Reclist[i].db_id != 0 
                && yx_DB_NetSms_Reclist[i].is_read == 0
                && yx_DB_NetSms_Reclist[i].netsms_rcv_time <  last_rcv_time)
            {
                last_rcv_time = yx_DB_NetSms_Reclist[i].netsms_rcv_time;
                netsms_idx = i;
                have_netsms = TRUE;
            }
        }
    }
    
    *pIdx = netsms_idx;
    
    if (have_netsms)
    {
        return TRUE;
    }
    return FALSE;
}

uint16 YX_DB_NetSms_Add( YX_DB_NETSMS_Type * netsms_ptr)
{
    uint16 i = 0;
    uint16 j = 0;
    
    if(netsms_ptr == NULL)
    {
        return 0;
    }
    YX_DB_NETSMS_ListInit();
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id == 0)
        {
            YX_DB_NETSMS_ListModify(i,netsms_ptr);
            j = i+1;
            break;
        }
    }
    if(j == 0)
    {
        if(YX_DB_NETSMS_GetOldNum(&i))
        {
            YX_DB_NETSMS_ListModify(i,netsms_ptr);
            j = i+1;
        }
    }
    YX_DB_NETSMS_ReOrderIndex();
    return j;
}

uint16 YX_DB_NETSMS_HaveNum(void)
{
    uint16 i = 0;
    uint16 netsms_sum = 0;
	
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0)
        {
            netsms_sum++;
        }
    }
    
    return netsms_sum;
}

BOOLEAN YX_DB_NETSMS_ReOrderIndex(void)
{
    uint8 i;
    uint8 j;
    uint8 unread_cnt = 0;
    uint8 read_cnt = 0;
    uint8 cnt = 0;
    uint8 tmp;
    
    SCI_MEMSET(yx_DB_NetSms_list_index,0,sizeof(yx_DB_NetSms_list_index));
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0 && yx_DB_NetSms_Reclist[i].is_read == 0)
        {
            yx_DB_NetSms_list_index[unread_cnt++] = i;
        }
    }
    for(i = 0; i < YX_DB_NETSMS_MAX_SUM; i++)
    {
        if(yx_DB_NetSms_Reclist[i].db_id != 0 && yx_DB_NetSms_Reclist[i].is_read)
        {
            yx_DB_NetSms_list_index[unread_cnt+read_cnt] = i;
            read_cnt++;
        }
    }
    
    cnt = unread_cnt+read_cnt;
    if(cnt == 0)
    {
        return FALSE;
    }

    if(unread_cnt > 0)
    {
        for(i=0;i<unread_cnt-1;i++) 
        {
            for(j=i+1;j<unread_cnt;j++)
            {
                if(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].netsms_rcv_time < yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[j]].netsms_rcv_time)
                { 
                    tmp=yx_DB_NetSms_list_index[i];
                    yx_DB_NetSms_list_index[i]=yx_DB_NetSms_list_index[j]; 
                    yx_DB_NetSms_list_index[j]=tmp;
                }
            }
        }
    }

    if(read_cnt > 0)
    {
        for(i=unread_cnt;i<cnt-1;i++) 
        {
            for(j=i+1;j<cnt;j++)
            {
                if(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].netsms_rcv_time < yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[j]].netsms_rcv_time)
                { 
                    tmp=yx_DB_NetSms_list_index[i];
                    yx_DB_NetSms_list_index[i]=yx_DB_NetSms_list_index[j]; 
                    yx_DB_NetSms_list_index[j]=tmp;
                }
            }
        }
    }
    return TRUE;
}
#endif

#ifdef YX_DB_NEED_REGIONAL
static BOOLEAN yx_DB_Regional_is_init = FALSE;
YX_DB_REGIONAL_Type yx_DB_Regional_Reclist[YX_DB_REGIONAL_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_REGIONAL_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_REGIONAL_Type);
    uint32 buf_array_size = (buf_size * YX_DB_REGIONAL_MAX_SUM);
    
    if(yx_DB_Regional_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_REGIONAL_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Regional_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE,(uint8 *)yx_DB_Regional_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_REGIONAL_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE,(void *)yx_DB_Regional_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_REGIONAL_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Regional_is_init = TRUE;
    }
    return TRUE;
}

BOOLEAN YX_DB_REGIONAL_ListModify(uint8 regional_idx,YX_DB_REGIONAL_Type * cur_data)
{    
    if (regional_idx >= YX_DB_REGIONAL_MAX_SUM)
    {
        ZDT_LOG("YX_DB_REGIONAL_ListModify(%d) ERR idx>=%d",regional_idx,YX_DB_REGIONAL_MAX_SUM);
        return FALSE;
    }
    yx_DB_Regional_Reclist[regional_idx] = *cur_data;
    yx_DB_Regional_Reclist[regional_idx].db_id = regional_idx+1;
    return TRUE;
}

BOOLEAN YX_DB_REGIONAL_ListDel(uint8  regional_idx)
{
    if (regional_idx >= YX_DB_REGIONAL_MAX_SUM)
    {
        ZDT_LOG("YX_DB_REGIONAL_ListDel(%d) ERR idx>=%d",regional_idx,YX_DB_REGIONAL_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Regional_Reclist[regional_idx],0x00,sizeof(YX_DB_REGIONAL_Type));
    return TRUE;
}

BOOLEAN YX_DB_REGIONAL_ListDelAll(void)
{    
    SCI_MEMSET(yx_DB_Regional_Reclist,0x00,sizeof(YX_DB_REGIONAL_Type)* YX_DB_REGIONAL_MAX_SUM);
    return TRUE;
}

BOOLEAN YX_DB_REGIONAL_ListSave(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_REGIONAL_Type);
    uint32 buf_array_size = (buf_size * YX_DB_REGIONAL_MAX_SUM);
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_REGIONAL_FS_FILE,(uint8 *)yx_DB_Regional_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_CLOCK_ListSaveBuf DB Write");
        return FALSE;
    }    
    return TRUE;
}

static BOOLEAN YX_DB_REGIONAL_TimeIsValid(uint32 start,uint32 end,uint32 cur)
{
    BOOLEAN ret = FALSE;
    
    if(start > end)
    {
        if(cur > end && cur < start)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        if(cur > start && cur < end)
        {
            return TRUE;
        }
    }
    return ret;

}

static BOOLEAN YX_DB_REGIONAL_WeekIsValid(uint8 type, uint8 cur)
{
    BOOLEAN ret = FALSE;
    
    if(cur > 6)
    {
        return FALSE;
    }

    cur++;
    
    if(((0x01<<cur) & type) > 0)
    {
        ret = TRUE;
    }

    return ret;

}

BOOLEAN YX_DB_REGIONAL_IsTimerValid(uint16 idx)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = idx;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    if(i < YX_DB_REGIONAL_MAX_SUM)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0)
        {
            if(YX_DB_REGIONAL_WeekIsValid(yx_DB_Regional_Reclist[i].time_week,date.wday) 
                && (YX_DB_REGIONAL_TimeIsValid(yx_DB_Regional_Reclist[i].time1_start,yx_DB_Regional_Reclist[i].time1_end,cur_daysecond)
                        || YX_DB_REGIONAL_TimeIsValid(yx_DB_Regional_Reclist[i].time2_start,yx_DB_Regional_Reclist[i].time2_end,cur_daysecond))
                )
            {
                is_valid = TRUE;
            }
        }
    }	
    return is_valid;
}

uint16 YX_DB_REGIONAL_IsTimerValid_All(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    uint16 reg_index = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = 0; i <  YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0)
        {
            if(YX_DB_REGIONAL_WeekIsValid(yx_DB_Regional_Reclist[i].time_week,date.wday) 
                && (YX_DB_REGIONAL_TimeIsValid(yx_DB_Regional_Reclist[i].time1_start,yx_DB_Regional_Reclist[i].time1_end,cur_daysecond)
                        || YX_DB_REGIONAL_TimeIsValid(yx_DB_Regional_Reclist[i].time2_start,yx_DB_Regional_Reclist[i].time2_end,cur_daysecond))
                )
            {
                is_valid = TRUE;
                reg_index = i+1;
                break;
            }
        }
    }	
    return reg_index;
}

double g_yx_wl_Latitude = 0;
double g_yx_wl_Longitude = 0;
BOOLEAN YX_DB_REGIONAL_IsInFence(uint16 idx,double latitude,double longitude)
{
    BOOLEAN is_in_fence = FALSE;
    double src_longitude = 0;
    double src_latitude = 0;
    uint16 radius = 0;
    double new_longitude = 0;
    double new_latitude = 0;
    uint32 d = 0;
    int i = 0;

    g_yx_wl_Longitude = longitude;
    g_yx_wl_Latitude = latitude;
    
    if(YX_DB_REGIONAL_IsTimerValid(idx))
    {
        new_longitude = longitude;
        new_latitude = latitude;
        if(yx_DB_Regional_Reclist[idx].zone_shape == 1)
        {
            src_longitude = yx_DB_Regional_Reclist[idx].regional_gps[0].Longitude;
            src_latitude = yx_DB_Regional_Reclist[idx].regional_gps[0].Latitude;
            is_in_fence = YX_Func_Regional_IsInRound(src_longitude,src_latitude,yx_DB_Regional_Reclist[idx].gps_radius,new_longitude,new_latitude);
        }
        else if(yx_DB_Regional_Reclist[idx].zone_shape == 2)
        {
            double long_arry[MAX_YX_REGIONAL_GPS_NUM] = {0};
            double lat_arry[MAX_YX_REGIONAL_GPS_NUM] = {0};
            for(i = 0; i < yx_DB_Regional_Reclist[idx].gps_sum; i++)
            {
                long_arry[i] = yx_DB_Regional_Reclist[idx].regional_gps[i].Longitude;
                lat_arry[i] = yx_DB_Regional_Reclist[idx].regional_gps[i].Latitude;
            }
            is_in_fence = YX_Func_Regional_IsInPoly(new_longitude,new_latitude,yx_DB_Regional_Reclist[idx].gps_sum,long_arry,lat_arry);
        }
        else if(yx_DB_Regional_Reclist[idx].zone_shape == 3)
        {
            double long_arry[MAX_YX_REGIONAL_GPS_NUM] = {0};
            double lat_arry[MAX_YX_REGIONAL_GPS_NUM] = {0};
            for(i = 0; i < yx_DB_Regional_Reclist[idx].gps_sum; i++)
            {
                long_arry[i] = yx_DB_Regional_Reclist[idx].regional_gps[i].Longitude;
                lat_arry[i] = yx_DB_Regional_Reclist[idx].regional_gps[i].Latitude;
            }
            is_in_fence = YX_Func_Regional_IsInPoly(new_longitude,new_latitude,yx_DB_Regional_Reclist[idx].gps_sum,long_arry,lat_arry);
        }
    }
    return is_in_fence;
}

BOOLEAN YX_DB_REGIONAL_Fence_CheckExt(double latitude,double longitude)
{
    BOOLEAN is_infence = FALSE;
    uint16 i = 0;
    BOOLEAN is_change = FALSE;
    for(i = 0; i <  YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0)
        {
            is_infence = YX_DB_REGIONAL_IsInFence(i,latitude,longitude);
            if(yx_DB_Regional_Reclist[i].is_in != is_infence)
            {
                if(ZDT_Factory_IsTesting_WL())
                {
                    if(is_infence)
                    {
                        YX_API_PlayWL_IN();
                    }
                    else
                    {
                        YX_API_PlayWL_Out();
                    }
                }
                if(YX_Net_Send_REPORT_CROSS_BORDER(&g_yx_app,1,is_infence,yx_DB_Regional_Reclist[i].request_status,yx_DB_Regional_Reclist[i].zone_id,latitude,longitude))
                {
                    yx_DB_Regional_Reclist[i].is_in = is_infence;
                    is_change = TRUE;
                }
            }
        }
    }	
    if(is_change)
    {
        YX_DB_REGIONAL_ListSave();
    }
    return is_change;
}

BOOLEAN YX_DB_REGIONAL_Fence_Check(GPS_DATA_T * gps_info)
{
    BOOLEAN res = FALSE;
    double new_longitude = 0;
    double new_latitude = 0;
    double gcjLat = 0;
    double gcjLon = 0;
    uint32 d = 0;
    if(gps_info == PNULL)
    {
        return FALSE;
    }
    
    new_longitude = gps_info->Longitude;
    new_longitude = new_longitude/180000;
    if(gps_info->Long_Dir != 'E')
    {
        new_longitude = 0 - new_longitude;
    }
    d = gps_info->Latitude/180000;
    new_latitude = gps_info->Lat_m;
    new_latitude = new_latitude/600000;
    new_latitude = new_latitude + d;
    if(gps_info->Lat_Dir != 'N')
    {
        new_latitude = 0 - new_latitude;
    }
    WGS84toGCJ02(new_latitude, new_longitude, &gcjLat, &gcjLon);
    res = YX_DB_REGIONAL_Fence_CheckExt(gcjLat,gcjLon);
    return res;
}

uint16 YX_DB_REGIONAL_Add( YX_DB_REGIONAL_Type * regional_ptr)
{
    uint16 i = 0;
    uint16 j = 0;
    
    if(regional_ptr == NULL)
    {
        return 0;
    }
    YX_DB_REGIONAL_ListInit();
    for(i = 0; i < YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0 && yx_DB_Regional_Reclist[i].zone_id == regional_ptr->zone_id)
        {
            YX_DB_REGIONAL_ListDel(i);
            YX_DB_REGIONAL_ListModify(i,regional_ptr);
            j = i+1;
            break;
        }
    }
    if(j == 0)
    {
        for(i = 0; i < YX_DB_REGIONAL_MAX_SUM; i++)
        {
            if(yx_DB_Regional_Reclist[i].db_id == 0)
            {
                YX_DB_REGIONAL_ListDel(i);
                YX_DB_REGIONAL_ListModify(i,regional_ptr);
                j = i+1;
                break;
            }
        }
    }
    if(j == 0)
    {
        j++;
        YX_DB_REGIONAL_ListModify(0,regional_ptr);
    }
    return j;
}

uint16 YX_DB_REGIONAL_Change( YX_DB_REGIONAL_Type * regional_ptr)
{
    return YX_DB_REGIONAL_Add(regional_ptr);
}

uint16 YX_DB_REGIONAL_Delete( YX_DB_REGIONAL_Type * regional_ptr)
{
    uint16 i = 0;
    uint16 j = 0;
    
    if(regional_ptr == NULL)
    {
        return 0;
    }
    YX_DB_REGIONAL_ListInit();
    for(i = 0; i < YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0 && yx_DB_Regional_Reclist[i].zone_id == regional_ptr->zone_id)
        {
            YX_DB_REGIONAL_ListDel(i);
            j = i+1;
            break;
        }
    }
    return j;
}

uint16 YX_DB_REGIONAL_HaveNum(void)
{
    uint16 i = 0;
    uint16 regional_sum = 0;
	
    for(i = 0; i < YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(yx_DB_Regional_Reclist[i].db_id != 0)
        {
            regional_sum++;
        }
    }
    
    return regional_sum;
}
#endif

#ifdef WIN32
BOOLEAN YX_DB_Default_Init(void)
{
    YX_DB_SOS_Type sos_data = {0};
    YX_DB_SPD_Type spd_data = {0};
    YX_DB_WHITE_Type white_data = {0};
    SCI_MEMCPY(sos_data.sos_name,"SOS1",4);
    SCI_MEMCPY(sos_data.sos_num,"18952560001",11);
    YX_DB_SOS_ListModifyBuf(0,&sos_data);
    SCI_MEMCPY(sos_data.sos_name,"SOS2",4);
    SCI_MEMCPY(sos_data.sos_num,"18952560002",11);
    YX_DB_SOS_ListModifyBuf(1,&sos_data);

    SCI_MEMCPY(spd_data.spd_name,"SPD1",4);
    SCI_MEMCPY(spd_data.spd_num,"18952560003",11);
    YX_DB_SPD_ListModifyBuf(0,&spd_data);
    SCI_MEMCPY(spd_data.spd_name,"SPD2",4);
    SCI_MEMCPY(spd_data.spd_num,"18952560004",11);
    YX_DB_SPD_ListModifyBuf(1,&spd_data);
    
    white_data.time1_start = 0;
    white_data.time1_end = (23 * 3600) + (59 * 60) + 59;
    SCI_MEMCPY(white_data.white_name,"WHITE1",6);
    SCI_MEMCPY(white_data.white_num,"18952560005",11);
    YX_DB_WHITE_ListModifyBuf(0,&white_data);
    SCI_MEMCPY(white_data.white_name,"WHITE2",6);
    SCI_MEMCPY(white_data.white_num,"18952560006",11);
    YX_DB_WHITE_ListModifyBuf(1,&white_data);
    return TRUE;
}
#endif

BOOLEAN YX_DB_Init(YX_APP_T* pMe)
{
#ifdef YX_DB_NEED_SOS
    YX_DB_SOS_ListInit();
#endif
#ifdef YX_DB_NEED_WHITE
    YX_DB_WHITE_ListInit();
#endif
#ifdef YX_DB_NEED_SPD
    YX_DB_SPD_ListInit();
#endif
#ifdef YX_DB_NEED_NETSMS
    YX_DB_NETSMS_ListInit();
#endif
#ifdef YX_DB_NEED_SETTING
    YX_DB_SET_ListInit();
#endif
#ifdef YX_DB_NEED_CLOCK
    YX_DB_CLOCK_ListInit();
#endif
#ifdef YX_DB_NEED_TIMER
    YX_DB_TIMER_ListInit();
#endif
#ifdef YX_DB_NEED_REGIONAL
    YX_DB_REGIONAL_ListInit();
#endif
#ifdef YX_DB_NEED_VOC
#ifdef WIN32
    YX_DB_Voc_Init(pMe);
#endif
    YX_DB_VocRcv_ListInit(pMe);
    if(pMe->m_yx_db_vocrcv_list_length > 0)
    {
        //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_VOC_NEW);
    }
#endif
#ifdef WIN32
    YX_DB_Default_Init();
#endif
    return TRUE;
}
BOOLEAN YX_DB_DelAll(YX_APP_T* pMe)
{
#ifdef YX_DB_NEED_SOS
    YX_DB_SOS_ListDelAll();
#endif
#ifdef YX_DB_NEED_WHITE
    YX_DB_WHITE_ListDelAll();
#endif
#ifdef YX_DB_NEED_SPD
    YX_DB_SPD_ListDelAll();
#endif
#ifdef YX_DB_NEED_NETSMS
    YX_DB_NETSMS_ListDelAll();
#endif
#ifdef YX_DB_NEED_SETTING
    YX_DB_SET_ListDel();
#endif
#ifdef YX_DB_NEED_CLOCK
    YX_DB_CLOCK_ListDelAll();
#endif
#ifdef YX_DB_NEED_TIMER
    YX_DB_TIMER_ListDelAll();
    YX_DB_TIMER_ListSave();
#endif
#ifdef YX_DB_NEED_REGIONAL
    YX_DB_REGIONAL_ListDelAll();
    YX_DB_REGIONAL_ListSave();
#endif
#ifdef YX_DB_NEED_VOC
    YX_DB_VocRcv_ListDeleteAll(pMe);
#endif
#ifdef WIN32
    YX_DB_Default_Init();
#endif
    return TRUE;
}

BOOLEAN YX_DB_Free(YX_APP_T* pMe)
{
#ifdef YX_DB_NEED_VOC
      YX_DB_VocRcv_ListFree(pMe);
#endif
	return TRUE;
}

PUBLIC BOOLEAN MMICC_GetYxDbRecCallName(uint8 *call_number, wchar *name_wstr_out , uint16 *name_wstr_len)
{
    BOOLEAN ret = FALSE;
   
    int i = 0;
    uint16 m_yx_totalnum = 0;
    uint16 tempStr[101] ={0};

    if(call_number == NULL || strlen(call_number) == 0 || name_wstr_out == NULL || name_wstr_len == NULL )
    {
        return FALSE;
    }
    
    if(strlen(call_number) > 0)
    {
        i = YX_DB_SOS_IsValidNumber(call_number);
        if(i > 0)
        {
            SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));        
            GUI_UTF8ToWstr(tempStr,100,yx_DB_Sos_Reclist[i-1].sos_name,strlen(yx_DB_Sos_Reclist[i-1].sos_name));	
            MMI_WSTRNCPY(name_wstr_out, YX_DB_SPD_MAX_NAME_SIZE/*MMIZDT_DIAL_MAX_LEN*/, tempStr, (uint16) MMIAPICOM_Wstrlen (tempStr), (uint16) MMIAPICOM_Wstrlen (tempStr));
            *name_wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
            ret = TRUE;
        }
        if(ret == FALSE)
        {
            i = YX_DB_SPD_IsValidNumber(call_number);
            if(i > 0)
            {
                SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));        
                GUI_UTF8ToWstr(tempStr,100,yx_DB_Spd_Reclist[i-1].spd_name,strlen(yx_DB_Spd_Reclist[i-1].spd_name));	
                MMI_WSTRNCPY(name_wstr_out, YX_DB_SPD_MAX_NAME_SIZE/*MMIZDT_DIAL_MAX_LEN*/, tempStr, (uint16) MMIAPICOM_Wstrlen (tempStr), (uint16) MMIAPICOM_Wstrlen (tempStr));
                *name_wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
                ret = TRUE;
            }
        }
        
        if(ret == FALSE)
        {
            i = YX_DB_WHITE_IsValidNumber(call_number);
            if(i > 0)
            {
                SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));        
                GUI_UTF8ToWstr(tempStr,100,yx_DB_White_Reclist[i-1].white_name,strlen(yx_DB_White_Reclist[i-1].white_name));	
                MMI_WSTRNCPY(name_wstr_out, YX_DB_SPD_MAX_NAME_SIZE/*MMIZDT_DIAL_MAX_LEN*/, tempStr, (uint16) MMIAPICOM_Wstrlen (tempStr), (uint16) MMIAPICOM_Wstrlen (tempStr));
                *name_wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
                ret = TRUE;
            }
        }
    }
    return ret;
}
#endif
