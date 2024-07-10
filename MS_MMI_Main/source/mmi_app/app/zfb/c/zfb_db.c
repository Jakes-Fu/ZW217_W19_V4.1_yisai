
#include "zfb_app.h"
#include "zfb_db.h"
#include "mmifmm_export.h"

#ifdef ZFB_DB_NEED_RSVD
static BOOLEAN zfb_DB_Rsvd_is_init = FALSE;
ZFB_DB_RSVD_Type zfb_DB_Rsvd_Rec = {0};
BOOLEAN ZFB_DB_RSVD_Default(void)
{
    SCI_MEMSET(&zfb_DB_Rsvd_Rec, 0, sizeof(ZFB_DB_RSVD_Type));
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN ZFB_DB_RSVD_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(ZFB_DB_RSVD_Type);
    
    if(zfb_DB_Rsvd_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZFB_DBFile_Exist(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE);
    if (is_exsit)
    {
        file_size = ZFB_DBFile_GetSize(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE);
        if(file_size != buf_size)
        {
            ZFBAPP_TRACE("ZFB_DB_RSVD_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZFB_DBFile_RemoveFile(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE);
        }
        else
        {
            ZFBAPP_TRACE("ZFB_DB_RSVD_ListInit OK");
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        ZFB_DB_RSVD_Default();
        res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE,(uint8 *)&zfb_DB_Rsvd_Rec,buf_size);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_RSVD_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZFB_DBFile_Read(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE,(void *)&zfb_DB_Rsvd_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_RSVD_ListInit ERR DB Read");
            ZFB_DB_RSVD_Default();
        }
    }
    zfb_DB_Rsvd_is_init = TRUE;
    return res;
}

BOOLEAN ZFB_DB_RSVD_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZFB_DB_RSVD_Type);
    
    ZFB_DB_RSVD_Default();
    if(zfb_DB_Rsvd_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_RSVD_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE,(uint8 *)&zfb_DB_Rsvd_Rec,buf_size);
    return res;
}

BOOLEAN ZFB_DB_RSVD_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZFB_DB_RSVD_Type);
    	    
    if(zfb_DB_Rsvd_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_RSVD_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_RSVD_FS_FILE,(uint8 *)&zfb_DB_Rsvd_Rec,buf_size);
    return res;
}
#endif

#ifdef ZFB_DB_NEED_PROV
static BOOLEAN zfb_DB_Prov_is_init = FALSE;
ZFB_DB_PROV_Type zfb_DB_Prov_Rec = {0};
BOOLEAN ZFB_DB_PROV_Default(void)
{
    SCI_MEMSET(&zfb_DB_Prov_Rec, 0, sizeof(ZFB_DB_PROV_Type));
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN ZFB_DB_PROV_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(ZFB_DB_PROV_Type);
    
    if(zfb_DB_Prov_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZFB_DBFile_Exist(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE);
    if (is_exsit)
    {
        file_size = ZFB_DBFile_GetSize(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE);
        if(file_size != buf_size)
        {
            ZFBAPP_TRACE("ZFB_DB_PROV_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZFB_DBFile_RemoveFile(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE);
        }
        else
        {
            ZFBAPP_TRACE("ZFB_DB_PROV_ListInit OK");
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        ZFB_DB_PROV_Default();
        res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE,(uint8 *)&zfb_DB_Prov_Rec,buf_size);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_PROV_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZFB_DBFile_Read(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE,(void *)&zfb_DB_Prov_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_PROV_ListInit ERR DB Read");
            ZFB_DB_PROV_Default();
        }
    }
    zfb_DB_Prov_is_init = TRUE;
    return res;
}

BOOLEAN ZFB_DB_PROV_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZFB_DB_PROV_Type);
    
    ZFB_DB_PROV_Default();
    if(zfb_DB_Prov_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_PROV_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE,(uint8 *)&zfb_DB_Prov_Rec,buf_size);
    return res;
}

BOOLEAN ZFB_DB_PROV_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZFB_DB_PROV_Type);
    	    
    if(zfb_DB_Prov_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_PROV_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_PROV_FS_FILE,(uint8 *)&zfb_DB_Prov_Rec,buf_size);
    return res;
}

char * ZFB_DB_PROV_Get(void)
{
   return zfb_DB_Prov_Rec.prov_data;
}

void ZFB_DB_PROV_Set(char *provdata,uint16 prov_len)
{
    if(prov_len > MAX_ZFB_PROV_SIZE)
    {
        ZFBAPP_TRACE("ZFB_DB_PROV_Set ERR LEN > %d",MAX_ZFB_PROV_SIZE);
        prov_len = MAX_ZFB_PROV_SIZE;
    }
    SCI_MEMSET(&zfb_DB_Prov_Rec, 0, sizeof(ZFB_DB_PROV_Type));
    SCI_MEMCPY(&zfb_DB_Prov_Rec.prov_data, provdata, prov_len);

    ZFB_DB_PROV_ListModify();
}

uint16 ZFB_DB_PROV_GetLen(void)
{
    uint16 prov_len = 0;
    prov_len = SCI_STRLEN(zfb_DB_Prov_Rec.prov_data);
   return prov_len;
}

#endif

#ifdef ZFB_DB_NEED_KV
static BOOLEAN zfb_DB_Kv_is_init = FALSE;
sst_kv_t g_kv_list[ZFB_DB_KV_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN ZFB_DB_KV_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(sst_kv_t);
    uint32 buf_array_size = (buf_size * ZFB_DB_KV_MAX_SUM);
    
    if(zfb_DB_Kv_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZFB_DBFile_Exist(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE);
    if (is_exsit)
    {
        file_size = ZFB_DBFile_GetSize(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZFBAPP_TRACE("ZFB_DB_KV_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZFB_DBFile_RemoveFile(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(g_kv_list, 0, buf_array_size);
        res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE,(uint8 *)g_kv_list,buf_array_size);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_KV_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZFB_DBFile_Read(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE,(void *)g_kv_list,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZFBAPP_TRACE("ZFB_DB_KV_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        zfb_DB_Kv_is_init = TRUE;
    }
    return res;
}

BOOLEAN ZFB_DB_KV_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(sst_kv_t);
    uint32 buf_array_size = (buf_size * ZFB_DB_KV_MAX_SUM);
            
    if(zfb_DB_Kv_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_KV_ListModify ERR NoInit");
        return FALSE;
    }

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE,(uint8 *)g_kv_list,buf_array_size);
    if (res == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_KV_ListModify ERR DB Write");
        return FALSE;
    }
    
    return TRUE;
}

BOOLEAN ZFB_DB_KV_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(sst_kv_t);
    uint32 buf_array_size = (buf_size * ZFB_DB_KV_MAX_SUM);
    
    SCI_MEMSET(g_kv_list,0x00,sizeof(sst_kv_t)* ZFB_DB_KV_MAX_SUM);
    
    if(zfb_DB_Kv_is_init == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_KV_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_KV_FS_FILE,(uint8 *)g_kv_list,buf_array_size);
    if (res == FALSE)
    {
        ZFBAPP_TRACE("ZFB_DB_KV_ListDelAll ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

#endif

BOOLEAN ZFB_DB_INIT_OK_Set(void)
{
    BOOLEAN res = FALSE;
    uint8 buf[3] = {'O','K',0};
    uint32 buf_size = 2;

    res = ZFB_DBFile_Write(ZFB_DB_DIR,ZFB_DB_INIT_OK_FS_FILE,(uint8 *)buf,buf_size);
    return res;
}

BOOLEAN ZFB_DB_INIT_OK_Get(void)
{
    BOOLEAN res = FALSE;
    	    
    res = ZFB_DBFile_Exist(ZFB_DB_DIR,ZFB_DB_INIT_OK_FS_FILE);
    return res;
}

BOOLEAN ZFB_DB_INIT_OK_Delelte(void)
{
    ZFB_DBFile_RemoveFile(ZFB_DB_DIR,ZFB_DB_INIT_OK_FS_FILE);
    return TRUE;
}

BOOLEAN ZFB_DB_INIT_OK_Delete_1(void)
{
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_INIT_OK_Delelte();
    return TRUE;
}

BOOLEAN ZFB_DB_INIT_OK_Delete_2(void)
{
    ZFB_DB_PROV_ListDel();
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_INIT_OK_Delelte();
    return TRUE;
}

BOOLEAN ZFB_DB_INIT_OK_Delete_All(void)
{
    ZFB_DB_RSVD_ListDel();
    ZFB_DB_PROV_ListDel();
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_INIT_OK_Delelte();
    return TRUE;
}

BOOLEAN ZFB_DB_INIT_OK_Reset_1(void)
{
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_INIT_OK_Delelte();
    MMIAPIPHONE_PowerReset();
    return TRUE;
}
BOOLEAN ZFB_DB_INIT_OK_Reset_2(void)
{
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_PROV_ListDel();
    ZFB_DB_INIT_OK_Delelte();
    MMIAPIPHONE_PowerReset();
    return TRUE;
}

BOOLEAN ZFB_DB_INIT_OK_Reset_All(void)
{
    ZFB_DB_RSVD_ListDel();
    ZFB_DB_PROV_ListDel();
    ZFB_DB_KV_ListDelAll();
    ZFB_DB_INIT_OK_Delelte();
    MMIAPIPHONE_PowerReset();
    return TRUE;
}

BOOLEAN ZFB_DB_Init(void)
{
#ifdef ZFB_DB_NEED_RSVD
    ZFB_DB_RSVD_ListInit();
#endif
#ifdef ZFB_DB_NEED_PROV
    ZFB_DB_PROV_ListInit();
#endif
#ifdef ZFB_DB_NEED_KV
    ZFB_DB_KV_ListInit();
#endif
    return TRUE;
}

BOOLEAN ZFB_DB_DelAll(void)
{
    return TRUE;
}

BOOLEAN ZFB_DB_Free(void)
{
	return TRUE;
}
