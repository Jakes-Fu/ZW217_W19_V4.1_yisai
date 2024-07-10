
#include "zdt_app.h"
#include "mmifmm_export.h"

#ifdef ZDT_PLAT_YX_SUPPORT
#define YX_ABS(x) (((x)<0) ? -(x) : (x))
#define MAX_FILE_NAME      256

#ifdef YX_DB_NEED_SETTING
#define YX_DB_SET_VERSION_NO 2
static BOOLEAN yx_DB_Set_is_init = FALSE;
YX_DB_SET_Type yx_DB_Set_Rec = {0};
#if defined(ZDT_PLAT_YX_SUPPORT_FZD)
//防走丢后台
#define YX_DB_IP_DOMAIN_URL "api.xcloudtech.com"
#define YX_DB_APP_DOMAIN_URL "api.xcloudtech.com"
#define YX_DB_IP_DOMAIN_PORT 9000
#define YX_DB_APP_DOMAIN_PORT 8080
#elif defined( ZDT_PLAT_YX_SUPPORT_YS)
//易赛后台 安康守护
#define YX_DB_IP_DOMAIN_URL "watch.ecellsz.com"
////////#define YX_DB_IP_DOMAIN_URL "watch.test.ecellsz.com" // del it
#define YX_DB_APP_DOMAIN_URL "4g.watch.ecellsz.com"
#define YX_DB_IP_DOMAIN_PORT 11500
#define YX_DB_APP_DOMAIN_PORT 8080
#elif defined( ZDT_PLAT_YX_SUPPORT_TX)
//童信后台百合
#define YX_DB_IP_DOMAIN_URL "tcpdis.bh.kidxin.com"
#define YX_DB_APP_DOMAIN_URL "http://apidis.bh.kidxin.com/getway/download_bihee"
#define YX_DB_IP_DOMAIN_PORT 11500
#define YX_DB_APP_DOMAIN_PORT 8080
#else
//防走丢后台
#define YX_DB_IP_DOMAIN_URL "api.xcloudtech.com"
#define YX_DB_APP_DOMAIN_URL "api.xcloudtech.com"
#define YX_DB_IP_DOMAIN_PORT 9000
#define YX_DB_APP_DOMAIN_PORT 8080
#endif

BOOLEAN YX_DB_SET_Default(void)
{
    SCI_MEMSET(&yx_DB_Set_Rec, 0, sizeof(YX_DB_SET_Type));
    SCI_MEMCPY(yx_DB_Set_Rec.ip_domain,YX_DB_IP_DOMAIN_URL,strlen(YX_DB_IP_DOMAIN_URL));
    SCI_MEMCPY(yx_DB_Set_Rec.app_domain,YX_DB_APP_DOMAIN_URL,strlen(YX_DB_APP_DOMAIN_URL));
    yx_DB_Set_Rec.ip_port = YX_DB_IP_DOMAIN_PORT;
    yx_DB_Set_Rec.app_port = YX_DB_APP_DOMAIN_PORT;
    
    yx_DB_Set_Rec.version = YX_DB_SET_VERSION_NO;
    
    
    yx_DB_Set_Rec.time_interval = 3600; // 3600
    yx_DB_Set_Rec.net_open = 1;
    yx_DB_Set_Rec.alert_sos_sms = 1;
    yx_DB_Set_Rec.alert_low_bat = 1;
    yx_DB_Set_Rec.alert_jp = 1;
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
#endif

static BOOLEAN DB_schedule_is_init = FALSE;
YX_SCHEDULE_Type  yx_schedule_Rec= {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN DB_Schedule_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_SCHEDULE_Type);
    
    if(DB_schedule_is_init == TRUE)
    {
        return TRUE;
    }
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,DB_SCHEDULE_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,DB_SCHEDULE_FS_FILE);
        if(file_size != buf_size)
        {
            ZDT_LOG("DB_Schedule_Time_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,DB_SCHEDULE_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }
    
    if (is_exsit == FALSE)
    {
        SCI_MEMSET(&yx_schedule_Rec, 0, buf_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,DB_SCHEDULE_FS_FILE,(uint8 *)&yx_schedule_Rec,buf_size);
        if (res == FALSE)
        {
            ZDT_LOG("DB_Schedule_Time_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,DB_SCHEDULE_FS_FILE,(void *)&yx_schedule_Rec,buf_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("DB_Schedule_Time_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        DB_schedule_is_init = TRUE;
    }
    return res;
}

BOOLEAN DB_Schedule_ListDel(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_SCHEDULE_Type);
    
    SCI_MEMSET(&yx_schedule_Rec,0x00,sizeof(YX_SCHEDULE_Type));
    
    if(DB_schedule_is_init == FALSE)
    {
        ZDT_LOG("DB_Schedule_ListDel ERR NoInit");
        return FALSE;
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,DB_SCHEDULE_FS_FILE,(uint8 *)&yx_schedule_Rec,buf_size);
    if (res == FALSE)
    {
        ZDT_LOG("DB_Schedule_ListDel() ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}
BOOLEAN DB_Schedule_ListModifyBuf(uint32 times)
{
  //  DB_Poweron_time.is_save = 1;
  //  DB_Poweron_time.poweron_time = times;    
    return TRUE;
}
BOOLEAN DB_Schedule_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_SCHEDULE_Type);
    if(DB_schedule_is_init == FALSE)
    {
        ZDT_LOG("DB_Schedule_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,DB_SCHEDULE_FS_FILE,(uint8 *)&yx_schedule_Rec,buf_size);
    if (res == FALSE)
    {
        ZDT_LOG("DB_Schedule_ListSaveBuf ERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_is_have_Schedule(uint8 today_index)
{
    uint8 idx = 0;	
    uint8 j = 0;	
    uint8 i = today_index;	
    
    	for(j = 0; j < SCHEDULE_TIME_MUN_MAX; j++)
    	{
			if(SCI_STRLEN(yx_schedule_Rec.class_content_Type[i][j].class_content_str) != 0)
	        {
				return TRUE;
	        }
    	}

	return FALSE;
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

BOOLEAN YX_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
    
    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListModify(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len > YX_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_SOS_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_Sos_Reclist[sos_idx].sos_num,0x00,YX_DB_SOS_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
        
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

BOOLEAN YX_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len)
{
    if (sos_idx >= YX_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("YX_DB_SOS_ListModifyBuf(%d) ERR idx>=%d",sos_idx,YX_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len > YX_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_SOS_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_Sos_Reclist[sos_idx].sos_num,0x00,YX_DB_SOS_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
        
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
    
    SCI_MEMSET(&yx_DB_Sos_Reclist[sos_idx],0x00,sizeof(YX_DB_SOS_Type));
    
    return TRUE;
}
BOOLEAN YX_DB_SOS_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_SOS_Type);
    uint32 buf_array_size = (buf_size * YX_DB_SOS_MAX_SUM);
    
    SCI_MEMSET(yx_DB_Sos_Reclist,0x00,sizeof(YX_DB_SOS_Type)* YX_DB_SOS_MAX_SUM);
    
    if(yx_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_SOS_ListDelAll ERR NoInit");
        return FALSE;
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
    SCI_MEMSET(yx_DB_Sos_Reclist,0x00,sizeof(YX_DB_SOS_Type)* YX_DB_SOS_MAX_SUM);
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
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    YX_DB_SOS_ListInit();
    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        if(yx_DB_Sos_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(yx_DB_Sos_Reclist[i].sos_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,yx_DB_Sos_Reclist[i].sos_num))
                {
                    return i;
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

BOOLEAN YX_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_WHITE_Type);
    uint32 buf_array_size = (buf_size * YX_DB_WHITE_MAX_SUM);
    
    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListModify(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len > YX_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_WHITE_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_White_Reclist[white_idx].white_num,0x00,YX_DB_WHITE_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_White_Reclist[white_idx].white_num,white_num,num_len);
    
    if(name_len > YX_DB_WHITE_MAX_NAME_SIZE)
    {
        name_len = YX_DB_WHITE_MAX_NAME_SIZE;
    }
    SCI_MEMSET(yx_DB_White_Reclist[white_idx].white_name,0x00,YX_DB_WHITE_MAX_NAME_SIZE+1);
    SCI_MEMCPY(yx_DB_White_Reclist[white_idx].white_name,white_name,name_len);
    
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

BOOLEAN YX_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len)
{    
    if (white_idx >= YX_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("YX_DB_WHITE_ListModifyBuf(%d) ERR idx>=%d",white_idx,YX_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    //ZDT_LOG("YX_DB_WHITE_ListModifyBuf(%d) %s,%s",white_idx,white_num,white_name);
    yx_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len > YX_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_WHITE_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_White_Reclist[white_idx].white_num,0x00,YX_DB_WHITE_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_White_Reclist[white_idx].white_num,white_num,num_len);
    
    if(name_len > YX_DB_WHITE_MAX_NAME_SIZE)
    {
        name_len = YX_DB_WHITE_MAX_NAME_SIZE;
    }
    SCI_MEMSET(yx_DB_White_Reclist[white_idx].white_name,0x00,YX_DB_WHITE_MAX_NAME_SIZE+1);
    SCI_MEMCPY(yx_DB_White_Reclist[white_idx].white_name,white_name,name_len);
    
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

int YX_DB_WHITE_IsValidNumber(char * num)
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
    
    YX_DB_WHITE_ListInit();
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(yx_DB_White_Reclist[i].white_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,yx_DB_White_Reclist[i].white_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN YX_DB_WHITE_GetNameByNumber(uint16* num, uint16* name)
{
    char c_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1] = {0};
    uint16 num_len = 0;
    uint16 i = 0;
    uint16 db_num_len = 0;
    uint16 name_len = 0;
    
    if(num == NULL || name == NULL)
    {
        return FALSE;
    }
    MMIAPICOM_WstrToStr(num,c_num);
    num_len = SCI_STRLEN(c_num);
    if(num_len == 0)
    {
        return FALSE;
    }
    YX_DB_WHITE_ListInit();
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0)
        {
            db_num_len = SCI_STRLEN(yx_DB_White_Reclist[i].white_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)c_num,yx_DB_White_Reclist[i].white_num))
                {
                    name_len = SCI_STRLEN(yx_DB_White_Reclist[i].white_name);
                    if( name_len > 0)
                    {
                        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, name_len ,name, 16);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
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
static YX_DB_CLOCK_Type yx_DB_Clock_Reclist[YX_DB_CLOCK_MAX_SUM] = {0};

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

BOOLEAN YX_DB_CLOCK_ListModify(uint8 clock_idx,uint8 clock_onoff,uint8 clock_type,uint8 clock_week,uint32 clock_time)
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
    yx_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    yx_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    yx_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    yx_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    
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

BOOLEAN YX_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint8 clock_onoff,uint8 clock_type,uint8 clock_week,uint32 clock_time)
{    
    if (clock_idx >= YX_DB_CLOCK_MAX_SUM)
    {
        ZDT_LOG("YX_DB_CLOCK_ListModify(%d) ERR idx>=%d",clock_idx,YX_DB_CLOCK_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Clock_Reclist[clock_idx].db_id = clock_idx+1;
    yx_DB_Clock_Reclist[clock_idx].clock_onoff = clock_onoff;
    yx_DB_Clock_Reclist[clock_idx].clock_type = clock_type;
    yx_DB_Clock_Reclist[clock_idx].clock_week = clock_week;
    yx_DB_Clock_Reclist[clock_idx].clock_time = clock_time;
    
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
static YX_DB_TIMER_Type yx_DB_Timer_Reclist[YX_DB_TIMER_MAX_SUM] = {0};

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

BOOLEAN YX_DB_TIMER_IsValid_JP(void)
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
    
    for(i = YX_TIME_TYPE_JP_1; i <= YX_TIME_TYPE_JP_3; i++)
    {
        if(yx_DB_Timer_Reclist[i].db_id != 0)
        {
            j++;
            if(YX_DB_TIMER_WeekIsValid(yx_DB_Timer_Reclist[i].time_set,date.wday) 
                && YX_DB_TIMER_TimeIsValid(yx_DB_Timer_Reclist[i].time_start,yx_DB_Timer_Reclist[i].time_end,cur_daysecond)
                )
            {
                is_valid = TRUE;
                break;
            }
        }
    }
    if(j == 0 && is_valid == FALSE)
    {
        is_valid = TRUE;
    }
    return is_valid;
}

BOOLEAN YX_DB_TIMER_IsValid_FG(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    cur_daysecond = (time.hour * 3600) + (time.min* 60) + time.sec;
    
    for(i = YX_TIME_TYPE_FG_1; i <= YX_TIME_TYPE_FG_2; i++)
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
    
    for(i = YX_TIME_TYPE_SLIENT_1; i <= YX_TIME_TYPE_SLIENT_5; i++)
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

BOOLEAN YX_DB_JP_IsValid(void)
{
    if(yx_DB_Set_Rec.alert_jp && YX_DB_TIMER_IsValid_JP())
    {
        return TRUE;
    }
    return FALSE;
}
#endif

#ifdef YX_DB_NEED_FRIEND
static BOOLEAN yx_DB_Friend_is_init = FALSE;
YX_DB_FRIEND_Type yx_DB_Friend_Reclist[YX_DB_FRIEND_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN YX_DB_FRIEND_ListInit(void)
{
    uint32 read_byte = 0;
    BOOLEAN is_exsit = 0;
    BOOLEAN res = FALSE;
    uint32 file_size = 0;
    uint32 buf_size = sizeof(YX_DB_FRIEND_Type);
    uint32 buf_array_size = (buf_size * YX_DB_FRIEND_MAX_SUM);
    
    if(yx_DB_Friend_is_init == TRUE)
    {
        return TRUE;
    }
    
    is_exsit = ZDT_DBFile_Exist(YX_DB_DIR,YX_DB_FRIEND_FS_FILE);
    if (is_exsit)
    {
        file_size = ZDT_DBFile_GetSize(YX_DB_DIR,YX_DB_FRIEND_FS_FILE);
        if(file_size != buf_array_size)
        {
            ZDT_LOG("YX_DB_FRIEND_ListInit Filesize = 0x%x, BufSize=0x%x",file_size,buf_array_size);
            ZDT_DBFile_RemoveFile(YX_DB_DIR,YX_DB_FRIEND_FS_FILE);
        }
        else
        {
            is_exsit = TRUE;
        }
    }

    if (is_exsit == FALSE)
    {
        SCI_MEMSET(yx_DB_Friend_Reclist, 0, buf_array_size);
        res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(uint8 *)yx_DB_Friend_Reclist,buf_array_size);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_FRIEND_ListInit ERR DB Write");
        }
    }
    else
    {
        res = ZDT_DBFile_Read(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(void *)yx_DB_Friend_Reclist,buf_array_size,&read_byte);
        if (res == FALSE)
        {
            ZDT_LOG("YX_DB_FRIEND_ListInit ERR DB Open");
        }
    }
    if(res)
    {
        yx_DB_Friend_is_init = TRUE;
    }
    return res;
}

BOOLEAN YX_DB_FRIEND_ListModify(uint8 friend_idx,uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_FRIEND_Type);
    uint32 buf_array_size = (buf_size * YX_DB_FRIEND_MAX_SUM);
    
    if (friend_idx >= YX_DB_FRIEND_MAX_SUM)
    {
        ZDT_LOG("YX_DB_FRIEND_ListModify(%d) ERR idx>=%d",friend_idx,YX_DB_FRIEND_MAX_SUM);
        return FALSE;
    }
    	
    yx_DB_Friend_Reclist[friend_idx].db_id = friend_idx+1;
    if(id_len > YX_DB_FRIEND_MAX_ID_SIZE)
    {
        id_len = YX_DB_FRIEND_MAX_ID_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_id,0x00,YX_DB_FRIEND_MAX_ID_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_id,friend_id,id_len);
    
    if(num_len > YX_DB_FRIEND_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_FRIEND_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_num,0x00,YX_DB_FRIEND_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_num,friend_num,num_len);
    
    if(name_len > YX_DB_FRIEND_MAX_NAME_SIZE)
    {
        name_len = YX_DB_FRIEND_MAX_NAME_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_name,0x00,YX_DB_FRIEND_MAX_NAME_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_name,friend_name,name_len);
    
    if(yx_DB_Friend_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListModify(%d) ERR NoInit",friend_idx);
        return FALSE;
    }
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(uint8 *)yx_DB_Friend_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListModify(%d) ERR DB Write",friend_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_FRIEND_ListModifyBuf(uint8 friend_idx,uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len)
{    
    if (friend_idx >= YX_DB_FRIEND_MAX_SUM)
    {
        ZDT_LOG("YX_DB_FRIEND_ListModifyBuf(%d) ERR idx>=%d",friend_idx,YX_DB_FRIEND_MAX_SUM);
        return FALSE;
    }
    //ZDT_LOG("YX_DB_FRIEND_ListModifyBuf(%d) %s,%s",friend_idx,friend_num,friend_name);
    yx_DB_Friend_Reclist[friend_idx].db_id = friend_idx+1;

    if(id_len > YX_DB_FRIEND_MAX_ID_SIZE)
    {
        id_len = YX_DB_FRIEND_MAX_ID_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_id,0x00,YX_DB_FRIEND_MAX_ID_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_id,friend_id,id_len);

    if(num_len > YX_DB_FRIEND_MAX_NUMBER_SIZE)
    {
        num_len = YX_DB_FRIEND_MAX_NUMBER_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_num,0x00,YX_DB_FRIEND_MAX_NUMBER_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_num,friend_num,num_len);
    
    if(name_len > YX_DB_FRIEND_MAX_NAME_SIZE)
    {
        name_len = YX_DB_FRIEND_MAX_NAME_SIZE;
    }
    SCI_MEMSET(yx_DB_Friend_Reclist[friend_idx].friend_name,0x00,YX_DB_FRIEND_MAX_NAME_SIZE+1);
    SCI_MEMCPY(yx_DB_Friend_Reclist[friend_idx].friend_name,friend_name,name_len);
    
    return TRUE;
}

BOOLEAN YX_DB_FRIEND_ListDel(uint8  friend_idx)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_FRIEND_Type);
    uint32 buf_array_size = (buf_size * YX_DB_FRIEND_MAX_SUM);

    if (friend_idx >= YX_DB_FRIEND_MAX_SUM)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDel(%d) ERR idx>=%d",friend_idx,YX_DB_FRIEND_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Friend_Reclist[friend_idx],0x00,sizeof(YX_DB_FRIEND_Type));
    
    if(yx_DB_Friend_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDel(%d) ERR NoInit",friend_idx);
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(uint8 *)yx_DB_Friend_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDel(%d) ERR DB Write",friend_idx);
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_FRIEND_ListDelBuf(uint8  friend_idx)
{
    if (friend_idx >= YX_DB_FRIEND_MAX_SUM)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDel(%d) ERR idx>=%d",friend_idx,YX_DB_FRIEND_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&yx_DB_Friend_Reclist[friend_idx],0x00,sizeof(YX_DB_FRIEND_Type));
    
    return TRUE;
}
BOOLEAN YX_DB_FRIEND_ListDelAll(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_FRIEND_Type);
    uint32 buf_array_size = (buf_size * YX_DB_FRIEND_MAX_SUM);
    
    SCI_MEMSET(yx_DB_Friend_Reclist,0x00,sizeof(YX_DB_FRIEND_Type)* YX_DB_FRIEND_MAX_SUM);
    
    if(yx_DB_Friend_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(uint8 *)yx_DB_Friend_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListDelAllERR DB Write");
        return FALSE;
    }		
    return TRUE;
}

BOOLEAN YX_DB_FRIEND_ListDelAllBuf(void)
{
    SCI_MEMSET(yx_DB_Friend_Reclist,0x00,sizeof(YX_DB_FRIEND_Type)* YX_DB_FRIEND_MAX_SUM);
    return TRUE;
}

BOOLEAN YX_DB_FRIEND_ListSaveBuf(void)
{
    BOOLEAN res = FALSE;
    uint32 buf_size = sizeof(YX_DB_FRIEND_Type);
    uint32 buf_array_size = (buf_size * YX_DB_FRIEND_MAX_SUM);
        
    if(yx_DB_Friend_is_init == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    res = ZDT_DBFile_Write(YX_DB_DIR,YX_DB_FRIEND_FS_FILE,(uint8 *)yx_DB_Friend_Reclist,buf_array_size);
    if (res == FALSE)
    {
        ZDT_LOG("YX_DB_FRIEND_ListSaveBuf DB Write");
        return FALSE;
    }		
    return TRUE;
}

int YX_DB_FRIEND_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    uint8  friend_num[YX_DB_FRIEND_MAX_NUMBER_SIZE+1];
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
    YX_DB_FRIEND_ListInit();
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(friend_num,0,sizeof(friend_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_Friend_Reclist[i].friend_num,SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_num),friend_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(friend_num,r_num,r_len) == 0)
                {
                    return yx_DB_Friend_Reclist[i].db_id;
                }
            }
        }
    }
    return -1;
}

BOOLEAN YX_DB_FRIEND_GetNameByNumber(uint8* num, uint16* name)
{
    uint16 num_len = 0;
    uint16 i = 0;
    uint16 db_num_len = 0;
    uint16 name_len = 0;
    uint8  friend_num[YX_DB_FRIEND_MAX_NUMBER_SIZE+1];
    uint8  r_num[101] = {0};
    uint16 r_len = 0;
    
    if(num == NULL || name == NULL)
    {
        return FALSE;
    }
    num_len = SCI_STRLEN(num);
    if(num_len == 0)
    {
        return FALSE;
    }
    r_len = ZDT_DeleteStrHead(num,num_len,r_num);
    YX_DB_FRIEND_ListInit();
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0)
        {
            SCI_MEMSET(friend_num,0,sizeof(friend_num));
            db_num_len = ZDT_DeleteStrHead(yx_DB_Friend_Reclist[i].friend_num,SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_num),friend_num);
            if(db_num_len > 0 && r_len == db_num_len)
            {
                if(strncmp(friend_num,r_num,r_len) == 0)
                {
                    name_len = SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_name);
                    if( name_len > 0)
                    {
                        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_Friend_Reclist[i].friend_name, name_len ,name, 100);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

BOOLEAN YX_DB_Friend_have_num(void)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0)
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

uint8 YX_DB_Friend_GetSum(void)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0)
        {
            idx++;
        }
    }
    return idx;
}

uint8  YX_DB_Friend_Add(uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id == 0)
        {
            idx = i+1;
            YX_DB_FRIEND_ListModify(i,friend_id,id_len,friend_num,num_len,friend_name,name_len);
            break;
        }
    }
    return idx;
}

uint8 YX_DB_Friend_DeleteByFrinedID(uint8 * friend_id)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < YX_DB_FRIEND_MAX_SUM; i++)
    {
        if(yx_DB_Friend_Reclist[i].db_id != 0 
            && SCI_STRLEN(yx_DB_Friend_Reclist[i].friend_id) > 0
            && strcmp(yx_DB_Friend_Reclist[i].friend_id,friend_id) == 0)
        {
            YX_DB_FRIEND_ListDel(i);
            break;
        }
    }
    return idx;
}
#endif
BOOLEAN YX_DB_Init(YX_APP_T* pMe)
{
#ifdef YX_DB_NEED_SOS
    YX_DB_SOS_ListInit();
#ifdef FEATURE_CTA_SUPPORT
    YX_DB_SOS_ListModifyBuf(0,"122",3);
    YX_DB_SOS_ListModifyBuf(1,"112",3);
    YX_DB_SOS_ListModifyBuf(2,"119",3);
    YX_DB_SOS_ListModifyBuf(3,"10000",5);
    YX_DB_SOS_ListSaveBuf();
#endif
#endif
	DB_Schedule_ListInit();//add_schedule

#ifdef YX_DB_NEED_WHITE
    YX_DB_WHITE_ListInit();
#ifdef WIN32
    YX_DB_WHITE_ListModifyBuf(0,"122",3,"5F204E09",8);
    YX_DB_WHITE_ListModifyBuf(1,"112",3,"72364EB2",8);
    YX_DB_WHITE_ListModifyBuf(2,"119",3,"6BCD4EB2",8);
    YX_DB_WHITE_ListModifyBuf(3,"10000",5,"72387238",8);
    YX_DB_WHITE_ListModifyBuf(4,"18948735889",11,"59885988 ",8);
    YX_DB_WHITE_ListModifyBuf(5,"15994730009",11,"72377237 ",8);
    YX_DB_WHITE_ListModifyBuf(6,"13012345678",11,"5916516C",8);
    YX_DB_WHITE_ListModifyBuf(7,"13012345671",11,"59165A46",8);
    YX_DB_WHITE_ListModifyBuf(8,"13012345672",11,"53D453D4",8);
    YX_DB_WHITE_ListModifyBuf(9,"13012345673",11,"963F59E8",8);
    YX_DB_WHITE_ListSaveBuf();
#endif
#endif

#ifdef YX_DB_NEED_FRIEND
    YX_DB_FRIEND_ListInit();
#ifdef WIN32
    YX_DB_FRIEND_ListModifyBuf(0,"000",3,"122",3,"5F204E09",8);
    YX_DB_FRIEND_ListModifyBuf(1,"111",3,"112",3,"72364EB2",8);
    YX_DB_FRIEND_ListModifyBuf(2,"222",3,"119",3,"6BCD4EB2",8);
    YX_DB_FRIEND_ListModifyBuf(3,"333",3,"10000",5,"72387238",8);
    YX_DB_FRIEND_ListModifyBuf(4,"444",3,"18948735889",11,"59885988 ",8);
    YX_DB_FRIEND_ListModifyBuf(5,"555",3,"15994730009",11,"72377237 ",8);
    YX_DB_FRIEND_ListModifyBuf(6,"666",3,"13012345678",11,"5916516C",8);
    YX_DB_FRIEND_ListModifyBuf(7,"777",3,"13012345671",11,"59165A46",8);
    YX_DB_FRIEND_ListModifyBuf(8,"888",3,"13012345672",11,"53D453D4",8);
    YX_DB_FRIEND_ListModifyBuf(9,"999",3,"13012345673",11,"963F59E8",8);
    YX_DB_FRIEND_ListSaveBuf();
    YX_DB_Voc_Init(pMe);
#endif
    YX_VCHAT_GetAllGroupInfo();
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
#ifdef YX_DB_NEED_VOC
#ifdef WIN32
    YX_DB_Voc_Init(pMe);
#endif
    //YX_DB_VocRcv_ListInit(pMe);
    //if(pMe->m_yx_db_vocrcv_list_length > 0)
    //{
    //    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_VOC_NEW);
    //}
#endif
    return TRUE;
}

BOOLEAN YX_DB_DelAll(YX_APP_T* pMe)
{
#ifdef YX_DB_NEED_SOS
    YX_DB_SOS_ListDelAll();
#endif
	DB_Schedule_ListDel();//add_schedule
#ifdef YX_DB_NEED_WHITE
    YX_DB_WHITE_ListDelAll();
#endif
#ifdef YX_DB_NEED_FRIEND
    YX_DB_FRIEND_ListDelAll();
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
    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_HIDDEN,ANNUN_STATE_OFF);
#endif
    return TRUE;
}

BOOLEAN YX_DB_Free(YX_APP_T* pMe)
{
	return TRUE;
}

PUBLIC BOOLEAN MMICC_GetYxDbRecCallName(uint8 *call_number, wchar *name_wstr_out , uint16 *name_wstr_len)
{
    BOOLEAN ret = FALSE;
   
    int i = 0;
    uint16 m_yx_whitelist_totalnum = 0;
    uint16 tempStr[101] ={0};

    if(call_number == NULL || strlen(call_number) == 0 || name_wstr_out == NULL || name_wstr_len == NULL )
    {
        return FALSE;
    }
    
    if(strlen(call_number) > 0)
    {
        for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
        {
           if(yx_DB_White_Reclist[i].db_id)
           {
               m_yx_whitelist_totalnum++;
           }
        }


        for(i = 0 ;i < m_yx_whitelist_totalnum ; i++) 
        {
            if(strcmp(yx_DB_White_Reclist[i].white_num, call_number) == 0)
            {
                //is_name_exist = TRUE;
                ret = TRUE;
                break;
            }
        }

    }
    //if(is_name_exist)
    if(ret)
    {
        SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
        #if 0
        SCI_MEMSET(s_call_context.call_info[call_index].name, 0,(CC_MAX_PBNAME_LEN + 1)*sizeof(uint16));
        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,tempStr, 100);
        MMI_WSTRNCPY(s_call_context.call_info[call_index].name, 40/*MMIZDT_DIAL_MAX_LEN*/, tempStr, (uint16) MMIAPICOM_Wstrlen (tempStr), (uint16) MMIAPICOM_Wstrlen (tempStr));
        s_call_context.call_info[call_index].name_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
        s_call_context.call_info[call_index].is_name_exist = TRUE;
        if(name_str_ptr != NULL)
        {
            name_str_ptr->wstr_ptr = s_call_context.call_info[call_index].name;
            name_str_ptr->wstr_len = s_call_context.call_info[call_index].name_len;

        }
        #endif
        
        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,tempStr, 100);
        MMI_WSTRNCPY(name_wstr_out, 40/*MMIZDT_DIAL_MAX_LEN*/, tempStr, (uint16) MMIAPICOM_Wstrlen (tempStr), (uint16) MMIAPICOM_Wstrlen (tempStr));
        *name_wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);

    }

    return ret;

}
