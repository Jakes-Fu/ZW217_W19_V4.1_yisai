
#include "wldt_app.h"
#include "mmifmm_export.h"

#ifdef WLDT_PLAT_CY_SUPPORT
#define CY_ABS(x) (((x)<0) ? -(x) : (x))
#define MAX_FILE_NAME      256

#ifdef CY_DB_NEED_SETTING
#define CY_DB_SET_VERSION_NO 2

#define CY_DB_IP_DOMAIN_URL "cardlbs.hejiaoyu.com.cn"
#define CY_DB_APP_DOMAIN_URL "cardlbs.hejiaoyu.com.cn"

#define CY_DB_IP_DOMAIN_PORT 9142
#define CY_DB_APP_DOMAIN_PORT 80


static BOOLEAN cy_DB_Set_is_init = FALSE;
CY_DB_SET_Type cy_DB_Set_Rec = {0};
BOOLEAN CY_DB_SET_Default(void)
{
    SCI_MEMSET(&cy_DB_Set_Rec, 0, sizeof(CY_DB_SET_Type));
    SCI_MEMCPY(cy_DB_Set_Rec.ip_domain,CY_DB_IP_DOMAIN_URL,strlen(CY_DB_IP_DOMAIN_URL));
    SCI_MEMCPY(cy_DB_Set_Rec.app_domain,CY_DB_APP_DOMAIN_URL,strlen(CY_DB_APP_DOMAIN_URL));
    cy_DB_Set_Rec.ip_port = CY_DB_IP_DOMAIN_PORT;
    cy_DB_Set_Rec.app_port = CY_DB_APP_DOMAIN_URL;
    cy_DB_Set_Rec.version = CY_DB_SET_VERSION_NO;
    
    cy_DB_Set_Rec.time_interval = 600;
    cy_DB_Set_Rec.net_open = 0;
    cy_DB_Set_Rec.alert_sos_sms = 1;
    cy_DB_Set_Rec.alert_low_bat = 1;
    return TRUE;
}

/*****************************************链表数据操作*****************************************************************/
BOOLEAN CY_DB_SET_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(CY_DB_SET_Type);
    
    if(cy_DB_Set_is_init == TRUE)
    {
        return TRUE;
    }
   
    is_exsit = WLDT_DBFile_Exist(CY_DB_DIR,CY_DB_SET_FS_FILE);
    if (is_exsit)
    {
        file_size = WLDT_DBFile_GetSize(CY_DB_DIR,CY_DB_SET_FS_FILE);
        if(file_size != buf_size)
        {
            WLDT_LOG("CY_DB_SET_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            WLDT_DBFile_RemoveFile(CY_DB_DIR,CY_DB_SET_FS_FILE);
        }
        else
        {
            WLDT_LOG("CY_DB_SET_ListInit OK IP=%s, Port=%d",cy_DB_Set_Rec.ip_domain,cy_DB_Set_Rec.ip_port);
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        CY_DB_SET_Default();
        res = WLDT_DBFile_Write(CY_DB_DIR,CY_DB_SET_FS_FILE,(uint8 *)&cy_DB_Set_Rec,buf_size);
        if (res == FALSE)
        {
            WLDT_LOG("CY_DB_SET_ListInit ERR DB Write");
        }
    }
    else
    {
        res = WLDT_DBFile_Read(CY_DB_DIR,CY_DB_SET_FS_FILE,(void *)&cy_DB_Set_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            WLDT_LOG("CY_DB_SET_ListInit ERR DB Read");
            CY_DB_SET_Default();
        }
        else
        {
            if(cy_DB_Set_Rec.version != CY_DB_SET_VERSION_NO)
            {
                CY_DB_SET_Default();
                WLDT_DBFile_Write(CY_DB_DIR,CY_DB_SET_FS_FILE,(uint8 *)&cy_DB_Set_Rec,buf_size);
            }
        }
    }
    cy_DB_Set_is_init = TRUE;
    return res;
}

BOOLEAN CY_DB_SET_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(CY_DB_SET_Type);
    
    SCI_MEMSET(&cy_DB_Set_Rec,0x00,sizeof(CY_DB_SET_Type));
#ifdef FEATURE_CY_AKQ
        SCI_MEMCPY(cy_DB_Set_Rec.ip_domain,"114.55.16.137",13);
        cy_DB_Set_Rec.ip_port = 4002;
        cy_DB_Set_Rec.time_interval = 1800;
#else
    SCI_MEMCPY(cy_DB_Set_Rec.ip_domain,"api.xcloudtech.com",18);
    cy_DB_Set_Rec.ip_port = 9000;
    cy_DB_Set_Rec.time_interval = 1800;
#endif
    if(cy_DB_Set_is_init == FALSE)
    {
        WLDT_LOG("CY_DB_SET_ListDel ERR NoInit");
        return FALSE;
    }

    res = WLDT_DBFile_Write(CY_DB_DIR,CY_DB_SET_FS_FILE,(uint8 *)&cy_DB_Set_Rec,buf_size);
    return res;
}

BOOLEAN CY_DB_SET_ListModify(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(CY_DB_SET_Type);
    	
    cy_DB_Set_Rec.db_id = 1;
    
    if(cy_DB_Set_is_init == FALSE)
    {
        WLDT_LOG("CY_DB_SET_ListModify ERR NoInit");
        return FALSE;
    }

    res = WLDT_DBFile_Write(CY_DB_DIR,CY_DB_SET_FS_FILE,(uint8 *)&cy_DB_Set_Rec,buf_size);
    return res;
}
#endif

BOOLEAN CY_DB_Init(CY_APP_T* pMe)
{
#ifdef CY_DB_NEED_SETTING
    CY_DB_SET_ListInit();
#endif
    return TRUE;
}

BOOLEAN CY_DB_DelAll(CY_APP_T* pMe)
{
#ifdef CY_DB_NEED_SETTING
    CY_DB_SET_ListDel();
#endif
    return TRUE;
}

BOOLEAN CY_DB_Free(CY_APP_T* pMe)
{
	return TRUE;
}
#endif
