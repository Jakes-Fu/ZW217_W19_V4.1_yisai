
#include "zdt_app.h"
#include "zdt_db.h"
#include "mmifmm_export.h"

#ifdef ZDT_DB_NEED_APPCOUNT
#define ZDT_DB_APPCOUNT_VERSION_NO 2
static BOOLEAN zdt_DB_AppCount_is_init = FALSE;
ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Rec = {0};
ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Bak = {0};

BOOLEAN ZDT_DB_APPCOUNT_Default(void)
{
    SCI_MEMSET(&zdt_DB_AppCount_Rec, 0, sizeof(ZDT_DB_APPCOUNT_Type));
    SCI_MEMSET(&zdt_DB_AppCount_Bak, 0, sizeof(ZDT_DB_APPCOUNT_Type));
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN ZDT_DB_APPCOUNT_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(ZDT_DB_APPCOUNT_Type);
    
    if(zdt_DB_AppCount_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = ZDT_DBFile_Exist(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("ZDT_DB_APPCOUNT_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE);
        }
        else
        {
            ZDT_LOG("ZDT_DB_APPCOUNT_ListInit OK");
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        ZDT_DB_APPCOUNT_Default();
        res = ZDT_DBFile_Write(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE,(uint8 *)&zdt_DB_AppCount_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("ZDT_DB_APPCOUNT_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE,(void *)&zdt_DB_AppCount_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("ZDT_DB_APPCOUNT_ListInit ERR DB Read");
            ZDT_DB_APPCOUNT_Default();
        }
        else
        {
            if(zdt_DB_AppCount_Rec.version != ZDT_DB_APPCOUNT_VERSION_NO)
            {
                ZDT_DB_APPCOUNT_Default();
                ZDT_DBFile_Write(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE,(uint8 *)&zdt_DB_AppCount_Rec,buf_size);
            }
        }
    }
    zdt_DB_AppCount_Bak = zdt_DB_AppCount_Rec;
    zdt_DB_AppCount_is_init = TRUE;
    return res;
}

BOOLEAN ZDT_DB_APPCOUNT_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZDT_DB_APPCOUNT_Type);
    
    ZDT_DB_APPCOUNT_Default();
    if(zdt_DB_AppCount_is_init == FALSE)
    {
        ZDT_LOG("ZDT_DB_APPCOUNT_ListDel ERR NoInit");
        return FALSE;
    }

    res = ZDT_DBFile_Write(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE,(uint8 *)&zdt_DB_AppCount_Rec,buf_size);
    return res;
}

BOOLEAN ZDT_DB_APPCOUNT_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(ZDT_DB_APPCOUNT_Type);
    	
    zdt_DB_AppCount_Rec.db_id = 1;
    
    if(zdt_DB_AppCount_is_init == FALSE)
    {
        ZDT_LOG("ZDT_DB_APPCOUNT_ListModify ERR NoInit");
        return FALSE;
    }
    zdt_DB_AppCount_Bak = zdt_DB_AppCount_Rec;
    res = ZDT_DBFile_Write(ZDT_DB_DIR,ZDT_DB_APPCOUNT_FS_FILE,(uint8 *)&zdt_DB_AppCount_Rec,buf_size);
    return res;
}

BOOLEAN ZDT_DB_APPCOUNT_IsChange(void)
{
    BOOLEAN res = FALSE;
    if(zdt_DB_AppCount_Rec.index != zdt_DB_AppCount_Bak.index)
    {
        res = TRUE;
    }
    return res;
}

#endif


#ifdef ZDT_ADD_FIRST_POWER_RECORD
static BOOLEAN DB_Poweron_time_is_init = FALSE;
DB_Poweron_Time_Type DB_Poweron_time={0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN DB_Poweron_Time_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(DB_Poweron_Time_Type);
    
    if(DB_Poweron_time_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("DB_Poweron_Time_Init Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(&DB_Poweron_time, 0, buf_size);
        res = ZDT_DBFile_Write(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE,(uint8 *)&DB_Poweron_time,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("DB_Poweron_Time_Init ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE,(void *)&DB_Poweron_time,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("DB_Poweron_Time_Init ERR DB Open");
        }
    }
    if(res)
    {
        DB_Poweron_time_is_init = TRUE;
    }
    return res;
}

BOOLEAN DB_Poweron_Time_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(DB_Poweron_Time_Type);
    
    SCI_MEMSET(&DB_Poweron_time,0x00,sizeof(DB_Poweron_Time_Type));
    
    if(DB_Poweron_time_is_init == FALSE)
    {
        ZDT_LOG("DB_Poweron_Time_Del ERR NoInit");
        return FALSE;
    }
    res = ZDT_DBFile_Write(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE,(uint8 *)&DB_Poweron_time,buf_size);
    if (res == FALSE)
    {
        ZDT_LOG("DB_Poweron_Time_Del() ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}
BOOLEAN DB_Poweron_Time_ListModifyBuf(uint32 times)
{
    DB_Poweron_time.is_save = 1;
    DB_Poweron_time.poweron_time = times;    
    return TRUE;
}
BOOLEAN DB_Poweron_Time_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(DB_Poweron_Time_Type);
    if(DB_Poweron_time_is_init == FALSE)
    {
        ZDT_LOG("DB_Poweron_Time_SaveBuf ERR NoInit");
        return FALSE;
    }
    res = ZDT_DBFile_Write(ZDT_DB_DIR,DB_POWERON_TIME_FS_FILE,(uint8 *)&DB_Poweron_time,buf_size);
    if (res == FALSE)
    {
        ZDT_LOG("DB_Poweron_Time_SaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}
#endif


BOOLEAN ZDT_DB_Init(void)
{
#ifdef ZDT_DB_NEED_APPCOUNT
    ZDT_DB_APPCOUNT_ListInit();
#endif
#ifdef ZDT_ADD_FIRST_POWER_RECORD
    DB_Poweron_Time_ListInit();
#endif
    return TRUE;
}

BOOLEAN ZDT_DB_DelAll(void)
{
#ifdef ZDT_DB_NEED_APPCOUNT
    ZDT_DB_APPCOUNT_ListDel();
#endif
    return TRUE;
}

BOOLEAN ZDT_DB_Free(void)
{
	return TRUE;
}
