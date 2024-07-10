
#include "zdt_app.h"
#include "mmifmm_export.h"

#ifdef ZDT_PLAT_SHB_SUPPORT
#define SHB_ABS(x) (((x)<0) ? -(x) : (x))
#define MAX_FILE_NAME      256
#define SHB_SERVER_IP_DOMAIN  " http://mob.3gcare.cn/n"

#ifdef SHB_DB_NEED_SETTING
#define SHB_DB_SET_VERSION_NO 10
static BOOLEAN shb_DB_Set_is_init = FALSE;
SHB_DB_SET_Type shb_DB_Set_Rec = {0};
BOOLEAN SHB_DB_SET_Default(void)
{
    SCI_MEMSET(&shb_DB_Set_Rec, 0, sizeof(SHB_DB_SET_Type));
    SCI_MEMCPY(shb_DB_Set_Rec.ip_domain,SHB_SERVER_IP_DOMAIN,strlen(SHB_SERVER_IP_DOMAIN)); //正式服务器
    shb_DB_Set_Rec.version = SHB_DB_SET_VERSION_NO;
    shb_DB_Set_Rec.ip_port = 80;
    shb_DB_Set_Rec.time_interval = 5;
    shb_DB_Set_Rec.net_open = 1;
    shb_DB_Set_Rec.white_type = 1;
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_SET_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_SET_Type);
    
    if(shb_DB_Set_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_SET_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_SET_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("SHB_DB_SET_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_SET_FS_FILE);
        }
        else
        {
            ZDT_LOG("SHB_DB_SET_ListInit OK IP=%s, Port=%d",shb_DB_Set_Rec.ip_domain,shb_DB_Set_Rec.ip_port);
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SHB_DB_SET_Default();
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SET_FS_FILE,(uint8 *)&shb_DB_Set_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SET_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_SET_FS_FILE,(void *)&shb_DB_Set_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SET_ListInit ERR DB Read");
            SHB_DB_SET_Default();
        }
        else
        {
            if(shb_DB_Set_Rec.version != SHB_DB_SET_VERSION_NO)
            {
                SHB_DB_SET_Default();
                ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SET_FS_FILE,(uint8 *)&shb_DB_Set_Rec,buf_size);
            }
        }
    }
    shb_DB_Set_is_init = TRUE;
    return res;
}

BOOLEAN SHB_DB_SET_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SET_Type);
    
    SHB_DB_SET_Default();
    
    if(shb_DB_Set_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SET_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SET_FS_FILE,(uint8 *)&shb_DB_Set_Rec,buf_size);
    return res;
}

BOOLEAN SHB_DB_SET_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SET_Type);
    	
    shb_DB_Set_Rec.db_id = 1;
    
    if(shb_DB_Set_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SET_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SET_FS_FILE,(uint8 *)&shb_DB_Set_Rec,buf_size);
    return res;
}
#endif

#ifdef SHB_DB_NEED_APPSET
#define SHB_DB_APPSET_VERSION_NO 1
static BOOLEAN shb_DB_AppSet_is_init = FALSE;
SHB_DB_APPSET_Type shb_DB_AppSet_Rec = {0};

BOOLEAN SHB_DB_APPSET_Default(void)
{
    SCI_MEMSET(&shb_DB_AppSet_Rec, 0, sizeof(SHB_DB_APPSET_Type));
    shb_DB_AppSet_Rec.version = SHB_DB_APPSET_VERSION_NO;
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_APPSET_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_APPSET_Type);
    
    if(shb_DB_AppSet_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("SHB_DB_APPSET_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE);
        }
        else
        {
            ZDT_LOG("SHB_DB_APPSET_ListInit OK APP=%s, wChat=%s:%d",shb_DB_AppSet_Rec.app_domain,shb_DB_AppSet_Rec.wchat_domain,shb_DB_AppSet_Rec.wchat_port);
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SHB_DB_APPSET_Default();
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE,(uint8 *)&shb_DB_AppSet_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_APPSET_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE,(void *)&shb_DB_AppSet_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_APPSET_ListInit ERR DB Read");
            SHB_DB_APPSET_Default();
        }
        else
        {
            if(shb_DB_AppSet_Rec.version != SHB_DB_APPSET_VERSION_NO)
            {
                SHB_DB_APPSET_Default();
                ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE,(uint8 *)&shb_DB_AppSet_Rec,buf_size);
            }
        }
    }
    shb_DB_AppSet_is_init = TRUE;
    return res;
}

BOOLEAN SHB_DB_APPSET_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_APPSET_Type);
    
    SHB_DB_APPSET_Default();
    
    if(shb_DB_AppSet_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_APPSET_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE,(uint8 *)&shb_DB_AppSet_Rec,buf_size);
    return res;
}

BOOLEAN SHB_DB_APPSET_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_APPSET_Type);
    	
    shb_DB_AppSet_Rec.db_id = 1;
    
    if(shb_DB_AppSet_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_APPSET_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_APPSET_FS_FILE,(uint8 *)&shb_DB_AppSet_Rec,buf_size);
    return res;
}
#endif

#ifdef SHB_DB_NEED_PLATSET
#define SHB_DB_PLATSET_VERSION_NO 1
static BOOLEAN shb_DB_PlatSet_is_init = FALSE;
SHB_DB_PLATSET_Type shb_DB_PlatSet_Rec = {0};

BOOLEAN SHB_DB_PLATSET_Default(void)
{
    SCI_MEMSET(&shb_DB_PlatSet_Rec, 0, sizeof(SHB_DB_PLATSET_Type));
    shb_DB_PlatSet_Rec.version = SHB_DB_PLATSET_VERSION_NO;
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_PLATSET_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_PLATSET_Type);
    
    if(shb_DB_PlatSet_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("SHB_DB_PLATSET_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE);
        }
        else
        {
            ZDT_LOG("SHB_DB_PLATSET_ListInit OK platNum=%s, mobile=%s",shb_DB_PlatSet_Rec.platNum,shb_DB_PlatSet_Rec.mobile);
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SHB_DB_PLATSET_Default();
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE,(uint8 *)&shb_DB_PlatSet_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_PLATSET_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE,(void *)&shb_DB_PlatSet_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_PLATSET_ListInit ERR DB Read");
            SHB_DB_PLATSET_Default();
        }
        else
        {
            if(shb_DB_PlatSet_Rec.version != SHB_DB_PLATSET_VERSION_NO)
            {
                SHB_DB_PLATSET_Default();
                ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE,(uint8 *)&shb_DB_PlatSet_Rec,buf_size);
            }
        }
    }
    shb_DB_PlatSet_is_init = TRUE;
    return res;
}

BOOLEAN SHB_DB_PLATSET_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_PLATSET_Type);
    
    SHB_DB_PLATSET_Default();
    
    if(shb_DB_PlatSet_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_PLATSET_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE,(uint8 *)&shb_DB_PlatSet_Rec,buf_size);
    return res;
}

BOOLEAN SHB_DB_PLATSET_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_PLATSET_Type);
    	
    shb_DB_PlatSet_Rec.db_id = 1;
    
    if(shb_DB_PlatSet_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_PLATSET_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_PLATSET_FS_FILE,(uint8 *)&shb_DB_PlatSet_Rec,buf_size);
    return res;
}
#endif

#ifdef SHB_DB_NEED_SOS
static BOOLEAN shb_DB_Sos_is_init = FALSE;
SHB_DB_SOS_Type shb_DB_Sos_Reclist[SHB_DB_SOS_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_SOS_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SOS_MAX_SUM);
    
    if(shb_DB_Sos_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_SOS_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_SOS_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("SHB_DB_SOS_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_SOS_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(shb_DB_Sos_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(uint8 *)shb_DB_Sos_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SOS_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(void *)shb_DB_Sos_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SOS_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        shb_DB_Sos_is_init = TRUE;
    }
    return res;
}

BOOLEAN SHB_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SOS_MAX_SUM);
    
    if (sos_idx >= SHB_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SOS_ListModify(%d) ERR idx>=%d",sos_idx,SHB_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len > SHB_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_SOS_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_Sos_Reclist[sos_idx].sos_num,0x00,SHB_DB_SOS_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
        
    if(shb_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListModify(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(uint8 *)shb_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListModify(%d) ERR DB Write",sos_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len)
{
    if (sos_idx >= SHB_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SOS_ListModifyBuf(%d) ERR idx>=%d",sos_idx,SHB_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len > SHB_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_SOS_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_Sos_Reclist[sos_idx].sos_num,0x00,SHB_DB_SOS_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
        
    return TRUE;
}

BOOLEAN SHB_DB_SOS_ListDel(uint8  sos_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SOS_MAX_SUM);

    if (sos_idx >= SHB_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SOS_ListDel(%d) ERR idx>=%d",sos_idx,SHB_DB_SOS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Sos_Reclist[sos_idx],0x00,sizeof(SHB_DB_SOS_Type));
    
    if(shb_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListDel(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(uint8 *)shb_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListDel(%d) ERR DB Write",sos_idx);
        return FALSE;
    }		

    return TRUE;
}

BOOLEAN SHB_DB_SOS_ListDelBuf(uint8  sos_idx)
{
    if (sos_idx >= SHB_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SOS_ListDelBuf(%d) ERR idx>=%d",sos_idx,SHB_DB_SOS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Sos_Reclist[sos_idx],0x00,sizeof(SHB_DB_SOS_Type));
    
    return TRUE;
}
BOOLEAN SHB_DB_SOS_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SOS_MAX_SUM);
    
    SCI_MEMSET(shb_DB_Sos_Reclist,0x00,sizeof(SHB_DB_SOS_Type)* SHB_DB_SOS_MAX_SUM);
    
    if(shb_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(uint8 *)shb_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_SOS_ListDelAllBuf(void)
{
    SCI_MEMSET(shb_DB_Sos_Reclist,0x00,sizeof(SHB_DB_SOS_Type)* SHB_DB_SOS_MAX_SUM);
    return TRUE;
}

BOOLEAN SHB_DB_SOS_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SOS_MAX_SUM);
        
    if(shb_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SOS_FS_FILE,(uint8 *)shb_DB_Sos_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SOS_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

int SHB_DB_SOS_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    SHB_DB_SOS_ListInit();
    for(i = 0; i < SHB_DB_SOS_MAX_SUM; i++)
    {
        if(shb_DB_Sos_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(shb_DB_Sos_Reclist[i].sos_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,shb_DB_Sos_Reclist[i].sos_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN SHB_DB_SOS_have_num(void)
{
    uint8 i = 0;	
    uint8 sos_idx = 0;	
	
    for(i = 0; i < SHB_DB_SOS_MAX_SUM; i++)
    {
        if(shb_DB_Sos_Reclist[i].db_id != 0 && SCI_STRLEN(shb_DB_Sos_Reclist[i].sos_num) > 0)
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

#ifdef SHB_DB_NEED_SPD
static BOOLEAN shb_DB_Spd_is_init = FALSE;
SHB_DB_SPD_Type shb_DB_Spd_Reclist[SHB_DB_SPD_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_SPD_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SPD_MAX_SUM);
    
    if(shb_DB_Spd_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_SPD_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_SPD_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("SHB_DB_SPD_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_SPD_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(shb_DB_Spd_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(uint8 *)shb_DB_Spd_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SPD_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(void *)shb_DB_Spd_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_SPD_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        shb_DB_Spd_is_init = TRUE;
    }
    return res;
}

BOOLEAN SHB_DB_SPD_ListModify(uint8 spd_idx,char * spd_num,uint16 num_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SPD_MAX_SUM);
    
    if (spd_idx >= SHB_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SPD_ListModify(%d) ERR idx>=%d",spd_idx,SHB_DB_SPD_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Spd_Reclist[spd_idx].db_id = spd_idx+1;
    if(num_len > SHB_DB_SPD_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_SPD_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_Spd_Reclist[spd_idx].spd_num,0x00,SHB_DB_SPD_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_Spd_Reclist[spd_idx].spd_num,spd_num,num_len);
        
    if(shb_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListModify(%d) ERR NoInit",spd_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(uint8 *)shb_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListModify(%d) ERR DB Write",spd_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_SPD_ListModifyBuf(uint8 spd_idx,char * spd_num,uint16 num_len)
{
    if (spd_idx >= SHB_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SPD_ListModifyBuf(%d) ERR idx>=%d",spd_idx,SHB_DB_SPD_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Spd_Reclist[spd_idx].db_id = spd_idx+1;
    if(num_len > SHB_DB_SPD_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_SPD_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_Spd_Reclist[spd_idx].spd_num,0x00,SHB_DB_SPD_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_Spd_Reclist[spd_idx].spd_num,spd_num,num_len);
        
    return TRUE;
}

BOOLEAN SHB_DB_SPD_ListDel(uint8  spd_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SPD_MAX_SUM);

    if (spd_idx >= SHB_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SPD_ListDel(%d) ERR idx>=%d",spd_idx,SHB_DB_SPD_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Spd_Reclist[spd_idx],0x00,sizeof(SHB_DB_SPD_Type));
    
    if(shb_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListDel(%d) ERR NoInit",spd_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(uint8 *)shb_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListDel(%d) ERR DB Write",spd_idx);
        return FALSE;
    }		

    return TRUE;
}

BOOLEAN SHB_DB_SPD_ListDelBuf(uint8  spd_idx)
{
    if (spd_idx >= SHB_DB_SPD_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_SPD_ListDelBuf(%d) ERR idx>=%d",spd_idx,SHB_DB_SPD_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Spd_Reclist[spd_idx],0x00,sizeof(SHB_DB_SPD_Type));
    
    return TRUE;
}
BOOLEAN SHB_DB_SPD_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SPD_MAX_SUM);
    
    SCI_MEMSET(shb_DB_Spd_Reclist,0x00,sizeof(SHB_DB_SPD_Type)* SHB_DB_SPD_MAX_SUM);
    
    if(shb_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(uint8 *)shb_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_SPD_ListDelAllBuf(void)
{
    SCI_MEMSET(shb_DB_Spd_Reclist,0x00,sizeof(SHB_DB_SPD_Type)* SHB_DB_SPD_MAX_SUM);
    return TRUE;
}

BOOLEAN SHB_DB_SPD_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_SPD_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_SPD_MAX_SUM);
        
    if(shb_DB_Spd_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_SPD_FS_FILE,(uint8 *)shb_DB_Spd_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_SPD_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

int SHB_DB_SPD_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    SHB_DB_SPD_ListInit();
    for(i = 0; i < SHB_DB_SPD_MAX_SUM; i++)
    {
        if(shb_DB_Spd_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(shb_DB_Spd_Reclist[i].spd_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,shb_DB_Spd_Reclist[i].spd_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN SHB_DB_SPD_have_num(void)
{
    uint8 i = 0;	
    uint8 spd_idx = 0;	
	
    for(i = 0; i < SHB_DB_SPD_MAX_SUM; i++)
    {
        if(shb_DB_Spd_Reclist[i].db_id != 0 && SCI_STRLEN(shb_DB_Spd_Reclist[i].spd_num) > 0)
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

#ifdef SHB_DB_NEED_WHITE
static BOOLEAN shb_DB_White_is_init = FALSE;
SHB_DB_WHITE_Type shb_DB_White_Reclist[SHB_DB_WHITE_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_WHITE_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_WHITE_MAX_SUM);
    
    if(shb_DB_White_is_init == TRUE)
    {
        return TRUE;
    }
    
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("SHB_DB_WHITE_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(shb_DB_White_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(uint8 *)shb_DB_White_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_WHITE_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(void *)shb_DB_White_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_WHITE_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        shb_DB_White_is_init = TRUE;
    }
    return res;
}

BOOLEAN SHB_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_WHITE_MAX_SUM);
    
    if (white_idx >= SHB_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_WHITE_ListModify(%d) ERR idx>=%d",white_idx,SHB_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len > SHB_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_WHITE_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_White_Reclist[white_idx].white_num,0x00,SHB_DB_WHITE_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_White_Reclist[white_idx].white_num,white_num,num_len);
        
    if(shb_DB_White_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListModify(%d) ERR NoInit",white_idx);
        return FALSE;
    }
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(uint8 *)shb_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListModify(%d) ERR DB Write",white_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len)
{    
    if (white_idx >= SHB_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_WHITE_ListModifyBuf(%d) ERR idx>=%d",white_idx,SHB_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    //ZDT_LOG("SHB_DB_WHITE_ListModifyBuf(%d) %s,%s",white_idx,white_num,white_name);
    shb_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len > SHB_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = SHB_DB_WHITE_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(shb_DB_White_Reclist[white_idx].white_num,0x00,SHB_DB_WHITE_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(shb_DB_White_Reclist[white_idx].white_num,white_num,num_len);
        
    return TRUE;
}

BOOLEAN SHB_DB_WHITE_ListDel(uint8  white_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_WHITE_MAX_SUM);

    if (white_idx >= SHB_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,SHB_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_White_Reclist[white_idx],0x00,sizeof(SHB_DB_WHITE_Type));
    
    if(shb_DB_White_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDel(%d) ERR NoInit",white_idx);
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(uint8 *)shb_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDel(%d) ERR DB Write",white_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_WHITE_ListDelBuf(uint8  white_idx)
{
    if (white_idx >= SHB_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,SHB_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_White_Reclist[white_idx],0x00,sizeof(SHB_DB_WHITE_Type));
    
    return TRUE;
}
BOOLEAN SHB_DB_WHITE_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_WHITE_MAX_SUM);
    
    SCI_MEMSET(shb_DB_White_Reclist,0x00,sizeof(SHB_DB_WHITE_Type)* SHB_DB_WHITE_MAX_SUM);
    
    if(shb_DB_White_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(uint8 *)shb_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListDelAllERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN SHB_DB_WHITE_ListDelAllBuf(void)
{
    SCI_MEMSET(shb_DB_White_Reclist,0x00,sizeof(SHB_DB_WHITE_Type)* SHB_DB_WHITE_MAX_SUM);
    return TRUE;
}

BOOLEAN SHB_DB_WHITE_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_WHITE_MAX_SUM);
        
    if(shb_DB_White_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_WHITE_FS_FILE,(uint8 *)shb_DB_White_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_WHITE_ListSaveBuf DB Write");
        return FALSE;
    }		
    return TRUE;
}

int SHB_DB_WHITE_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    SHB_DB_WHITE_ListInit();
    for(i = 0; i < SHB_DB_WHITE_MAX_SUM; i++)
    {
        if(shb_DB_White_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(shb_DB_White_Reclist[i].white_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,shb_DB_White_Reclist[i].white_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN SHB_DB_White_have_num(void)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < SHB_DB_WHITE_MAX_SUM; i++)
    {
        if(shb_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(shb_DB_White_Reclist[i].white_num) > 0)
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

#ifdef SHB_DB_NEED_CLOCK

static BOOLEAN shb_DB_Clock_is_init = FALSE;
static SHB_DB_CLOCK_Type shb_DB_Clock_Reclist[SHB_DB_CLOCK_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_CLOCK_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_CLOCK_MAX_SUM);
    
    if(shb_DB_Clock_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("SHB_DB_CLOCK_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(shb_DB_Clock_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(uint8 *)shb_DB_Clock_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_CLOCK_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(void *)shb_DB_Clock_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_CLOCK_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        shb_DB_Clock_is_init = TRUE;
    }
    return TRUE;
}

BOOLEAN SHB_DB_CLOCK_ListModify(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_CLOCK_MAX_SUM);
    
    if (clock_idx >= SHB_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListModify(%d) ERR idx>=%d",clock_idx,SHB_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Clock_Reclist[clock_idx].db_id = clock_idx+1;
    shb_DB_Clock_Reclist[clock_idx].clock_id = clock_id;
    shb_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    shb_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    shb_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    shb_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    if(pConten != PNULL && content_len > 0)
    {
        if(content_len > SHB_DB_CLOCK_MAX_CONTENT_SIZE)
        {
            content_len = SHB_DB_CLOCK_MAX_CONTENT_SIZE;
            SCI_MEMCPY(shb_DB_Clock_Reclist[clock_idx].clock_content,pConten,content_len);
        }
    }
    
    if(shb_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListModify(%d) ERR NoInit",clock_idx);
        return FALSE;
    }

    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(uint8 *)shb_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListModify(%d) ERR DB Write",clock_idx);
    }
    return res;
}

BOOLEAN SHB_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len)
{    
    if (clock_idx >= SHB_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListModifyBuf(%d) ERR idx>=%d",clock_idx,SHB_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Clock_Reclist[clock_idx].db_id = clock_idx+1;
    shb_DB_Clock_Reclist[clock_idx].clock_id = clock_id;
    shb_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    shb_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    shb_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    shb_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    if(pConten != PNULL && content_len > 0)
    {
        if(content_len > SHB_DB_CLOCK_MAX_CONTENT_SIZE)
        {
            content_len = SHB_DB_CLOCK_MAX_CONTENT_SIZE;
            SCI_MEMSET(shb_DB_Clock_Reclist[clock_idx].clock_content,0,SHB_DB_CLOCK_MAX_CONTENT_SIZE+1);
            SCI_MEMCPY(shb_DB_Clock_Reclist[clock_idx].clock_content,pConten,content_len);
        }
    }
    
    return TRUE;
}
BOOLEAN SHB_DB_CLOCK_ListDel(uint8  clock_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_CLOCK_MAX_SUM);

    if (clock_idx >= SHB_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDel(%d) ERR idx>=%d",clock_idx,SHB_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Clock_Reclist[clock_idx],0x00,sizeof(SHB_DB_CLOCK_Type));
    
    if(shb_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDel(%d) ERR NoInit",clock_idx);
        return FALSE;
    }
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(uint8 *)shb_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDel(%d) ERR DB Write",clock_idx);
        return FALSE;
    }
    return TRUE;
}
BOOLEAN SHB_DB_CLOCK_ListDelBuf(uint8  clock_idx)
{
    if (clock_idx >= SHB_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDel(%d) ERR idx>=%d",clock_idx,SHB_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Clock_Reclist[clock_idx],0x00,sizeof(SHB_DB_CLOCK_Type));
    
    return TRUE;
}
BOOLEAN SHB_DB_CLOCK_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_CLOCK_MAX_SUM);
    
    SCI_MEMSET(shb_DB_Clock_Reclist,0x00,sizeof(SHB_DB_CLOCK_Type)* SHB_DB_CLOCK_MAX_SUM);
    
    if(shb_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(uint8 *)shb_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListDelAllERR DB Write");
        return FALSE;
    }
    return TRUE;
}

BOOLEAN SHB_DB_CLOCK_ListDelAllBuf(void)
{    
    SCI_MEMSET(shb_DB_Clock_Reclist,0x00,sizeof(SHB_DB_CLOCK_Type)* SHB_DB_CLOCK_MAX_SUM);
    
    return TRUE;
}

BOOLEAN SHB_DB_CLOCK_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_CLOCK_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_CLOCK_MAX_SUM);
        
    if(shb_DB_Clock_is_init == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_CLOCK_FS_FILE,(uint8 *)shb_DB_Clock_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListSaveBuf DB Write");
        return FALSE;
    }
    return TRUE;
}

#endif

#ifdef SHB_DB_NEED_TIMER
static BOOLEAN shb_DB_Timer_is_init = FALSE;
static SHB_DB_TIMER_Type shb_DB_Timer_Reclist[SHB_DB_TIMER_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN SHB_DB_TIMER_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(SHB_DB_TIMER_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_TIMER_MAX_SUM);
    
    if(shb_DB_Timer_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("SHB_DB_TIMER_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(shb_DB_Timer_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE,(uint8 *)shb_DB_Timer_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_TIMER_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE,(void *)shb_DB_Timer_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("SHB_DB_TIMER_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        shb_DB_Timer_is_init = TRUE;
    }
    return TRUE;
}

BOOLEAN SHB_DB_TIMER_ListModify(uint8 timer_idx,uint8 * time_id_str,uint8 time_set,uint32 time_start,uint32 time_end,uint32 time_interval_min)
{    
    if (timer_idx >= SHB_DB_TIMER_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_TIMER_ListModify(%d) ERR idx>=%d",timer_idx,SHB_DB_TIMER_MAX_SUM);
        return FALSE;
    }
    ZDT_LOG("SHB_DB_TIMER_ListModify(%d) time_start=%d,time_end=%d,time_set=0x%x,interval=%d",timer_idx,time_start,time_end,time_set,time_interval_min);
    shb_DB_Timer_Reclist[timer_idx].db_id = timer_idx+1;
    shb_DB_Timer_Reclist[timer_idx].time_id = timer_idx;
    SCI_MEMSET(shb_DB_Timer_Reclist[timer_idx].time_id_str,0,SHB_DB_TIME_MAX_ID_SIZE+1);
    if(time_id_str != PNULL)
    {
        SCI_MEMCPY(shb_DB_Timer_Reclist[timer_idx].time_id_str,time_id_str,strlen(time_id_str));
    }
    shb_DB_Timer_Reclist[timer_idx].time_set = time_set;
    shb_DB_Timer_Reclist[timer_idx].time_start = time_start;
    shb_DB_Timer_Reclist[timer_idx].time_end = time_end;
    shb_DB_Timer_Reclist[timer_idx].time_interval_min = time_interval_min;
    return TRUE;
}

BOOLEAN SHB_DB_TIMER_ListModifyExt(uint8 timer_idx,uint8 time_set,uint32 time_start,uint32 time_end,uint32 time_interval_min)
{    
    if (timer_idx >= SHB_DB_TIMER_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_TIMER_ListModifyExt(%d) ERR idx>=%d",timer_idx,SHB_DB_TIMER_MAX_SUM);
        return FALSE;
    }
    	
    shb_DB_Timer_Reclist[timer_idx].db_id = timer_idx+1;
    shb_DB_Timer_Reclist[timer_idx].time_set = time_set;
    shb_DB_Timer_Reclist[timer_idx].time_start = time_start;
    shb_DB_Timer_Reclist[timer_idx].time_end = time_end;
    shb_DB_Timer_Reclist[timer_idx].time_interval_min = time_interval_min;
    return TRUE;
}
BOOLEAN SHB_DB_TIMER_ListDel(uint8  timer_idx)
{
    if (timer_idx >= SHB_DB_TIMER_MAX_SUM)
    {
        ZDT_LOG("SHB_DB_TIMER_ListDel(%d) ERR idx>=%d",timer_idx,SHB_DB_TIMER_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&shb_DB_Timer_Reclist[timer_idx],0x00,sizeof(SHB_DB_TIMER_Type));
    return TRUE;
}

BOOLEAN SHB_DB_TIMER_ListDelAll(void)
{    
    SCI_MEMSET(shb_DB_Timer_Reclist,0x00,sizeof(SHB_DB_TIMER_Type)* SHB_DB_TIMER_MAX_SUM);
    return TRUE;
}

BOOLEAN SHB_DB_TIMER_ListSave(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(SHB_DB_TIMER_Type);
    uint32 buf_array_size = (buf_size * SHB_DB_TIMER_MAX_SUM);
    
    res = ZDT_DBFile_Write(SHB_DB_DIR,SHB_DB_TIMER_FS_FILE,(uint8 *)shb_DB_Timer_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("SHB_DB_CLOCK_ListSaveBuf DB Write");
        return FALSE;
    }
    
    return TRUE;
}

static BOOLEAN SHB_DB_TIMER_TimeIsValid(uint32 start,uint32 end,uint32 cur)
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

static BOOLEAN SHB_DB_TIMER_WeekIsValid(uint8 type, uint8 cur)
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
    if(cur == 0)
    {
        cur = 7;
    }    
    if(((0x01<<cur) & type) > 0)
    {
        ret = TRUE;
    }

    return ret;

}

BOOLEAN SHB_DB_TIMER_IsValid(uint16 idx)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = idx;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    if(i < SHB_DB_TIMER_MAX_SUM)
    {
        if(shb_DB_Timer_Reclist[i].db_id != 0)
        {
            if(SHB_DB_TIMER_WeekIsValid(shb_DB_Timer_Reclist[i].time_set,date.wday) 
                && SHB_DB_TIMER_TimeIsValid(shb_DB_Timer_Reclist[i].time_start,shb_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
            }
        }
    }	
    return is_valid;
}

BOOLEAN SHB_DB_TIMER_IsValid_Loc(uint32  * pIntervalMin,uint8 * pTtimeID)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    uint16 j = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = SHB_TIME_TYPE_LOC_1; i <= SHB_TIME_TYPE_LOC_8; i++)
    {
        if(shb_DB_Timer_Reclist[i].db_id != 0)
        {
            j++;
            if(SHB_DB_TIMER_WeekIsValid(shb_DB_Timer_Reclist[i].time_set,date.wday) 
                && SHB_DB_TIMER_TimeIsValid(shb_DB_Timer_Reclist[i].time_start,shb_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                SCI_TRACE_LOW("SHB_DB_TIMER_IsValid_Loc In %d: time_id=%s,interval=%d",i-SHB_TIME_TYPE_LOC_1,shb_DB_Timer_Reclist[i].time_id_str,shb_DB_Timer_Reclist[i].time_interval_min);
                *pIntervalMin = shb_DB_Timer_Reclist[i].time_interval_min;
                SCI_MEMCPY(pTtimeID,shb_DB_Timer_Reclist[i].time_id_str,SHB_DB_TIME_MAX_ID_SIZE+1);
                is_valid = TRUE;
                break;
            }
        }
    }
    return is_valid;
}

BOOLEAN SHB_DB_TIMER_IsValid_Slient(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = SHB_TIME_TYPE_SLIENT_1; i <= SHB_TIME_TYPE_SLIENT_4; i++)
    {
        if(shb_DB_Timer_Reclist[i].db_id != 0)
        {
            ZDT_LOG("SHB_DB_TIMER_IsValid_Slient[%d-%d](%d) time_start=%d,time_end=%d,time_set=0x%x,wday=%d",i,shb_DB_Timer_Reclist[i].time_id,cur_daysecond,
                                    shb_DB_Timer_Reclist[i].time_start,shb_DB_Timer_Reclist[i].time_end,shb_DB_Timer_Reclist[i].time_set,date.wday);	
            if(SHB_DB_TIMER_WeekIsValid(shb_DB_Timer_Reclist[i].time_set,date.wday) 
                && SHB_DB_TIMER_TimeIsValid(shb_DB_Timer_Reclist[i].time_start,shb_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                SCI_TRACE_LOW("SHB_DB_TIMER_IsValid_Slient In %d",i-SHB_TIME_TYPE_SLIENT_1);
                is_valid = TRUE;
                break;
            }
        }
    }	
    return is_valid;
}

BOOLEAN SHB_DB_TIMER_IsValid_CR(uint32  * pIntervalMin,uint8 * pTtimeID)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = SHB_TIME_TYPE_CR_1; i <= SHB_TIME_TYPE_CR_3; i++)
    {
        if(shb_DB_Timer_Reclist[i].db_id != 0)
        {
            if(SHB_DB_TIMER_WeekIsValid(shb_DB_Timer_Reclist[i].time_set,date.wday) 
                && SHB_DB_TIMER_TimeIsValid(shb_DB_Timer_Reclist[i].time_start,shb_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                SCI_TRACE_LOW("SHB_DB_TIMER_IsValid_CR In %d: time_id=%s,interval=%d",i-SHB_TIME_TYPE_CR_1,shb_DB_Timer_Reclist[i].time_id_str,shb_DB_Timer_Reclist[i].time_interval_min);
                *pIntervalMin = shb_DB_Timer_Reclist[i].time_interval_min;
                SCI_MEMCPY(pTtimeID,shb_DB_Timer_Reclist[i].time_id_str,SHB_DB_TIME_MAX_ID_SIZE+1);
                is_valid = TRUE;
                break;
            }
        }
    }	
    return is_valid;
}
#endif

BOOLEAN SHB_DB_Init(SHB_APP_T* pMe)
{
#ifdef SHB_DB_NEED_SOS
    SHB_DB_SOS_ListInit();
#ifdef FEATURE_CTA_SUPPORT
    SHB_DB_SOS_ListModifyBuf(0,"122",3);
    SHB_DB_SOS_ListModifyBuf(1,"112",3);
    SHB_DB_SOS_ListModifyBuf(2,"119",3);
    SHB_DB_SOS_ListModifyBuf(3,"10000",5);
    SHB_DB_SOS_ListSaveBuf();
#endif
#endif
#ifdef SHB_DB_NEED_SPD
    SHB_DB_SPD_ListInit();
#endif
#ifdef SHB_DB_NEED_WHITE
    SHB_DB_WHITE_ListInit();
#endif
#ifdef SHB_DB_NEED_SETTING
    SHB_DB_SET_ListInit();
#endif
#ifdef SHB_DB_NEED_APPSET
    SHB_DB_APPSET_ListInit();
#endif
#ifdef SHB_DB_NEED_PLATSET
    SHB_DB_PLATSET_ListInit();
#endif
#ifdef SHB_DB_NEED_CLOCK
    SHB_DB_CLOCK_ListInit();
#endif
#ifdef SHB_DB_NEED_TIMER
    SHB_DB_TIMER_ListInit();
#endif
    return TRUE;
}

BOOLEAN SHB_DB_DelAll(SHB_APP_T* pMe)
{
#ifdef SHB_DB_NEED_SOS
    SHB_DB_SOS_ListDelAll();
#endif
#ifdef SHB_DB_NEED_SPD
    SHB_DB_SPD_ListDelAll();
#endif
#ifdef SHB_DB_NEED_WHITE
    SHB_DB_WHITE_ListDelAll();
#endif
#ifdef SHB_DB_NEED_SETTING
    SHB_DB_SET_ListDel();
#endif
#ifdef SHB_DB_NEED_APPSET
    SHB_DB_APPSET_ListDel();
#endif
#ifdef SHB_DB_NEED_PLATSET
    SHB_DB_PLATSET_ListDel();
#endif
#ifdef SHB_DB_NEED_CLOCK
    SHB_DB_CLOCK_ListDelAll();
#endif
#ifdef SHB_DB_NEED_TIMER
    SHB_DB_TIMER_ListDelAll();
    SHB_DB_TIMER_ListSave();
    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_HIDDEN,ANNUN_STATE_OFF);
#endif
    return TRUE;
}

BOOLEAN SHB_DB_Free(SHB_APP_T* pMe)
{
	return TRUE;
}
#endif
