
#include "zdt_app.h"

#ifdef ZDT_PLAT_HYHL_SUPPORT
#include "zdt_hyhl.h"
#define HYHL_ABS(x) (((x)<0) ? -(x) : (x))

#ifdef HYHL_DB_NEED_SOS
static BOOLEAN hyhl_DB_Sos_is_init = FALSE;
HYHL_DB_SOS_Type hyhl_DB_Sos_Reclist[HYHL_DB_SOS_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN HYHL_DB_SOS_ListInit(void)
{
    int i = 0;
    uint32 handletmp;
    int offset;
    offset = sizeof(HYHL_DB_SOS_Type);
    
    if(hyhl_DB_Sos_is_init == TRUE)
    {
        return TRUE;
    }

    hyhl_DB_Sos_is_init = TRUE;
    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len,char * sos_name,uint16 name_len)
{
    uint32 handletmp;
    
    if (sos_idx >= HYHL_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_SOS_ListModify(%d) ERR idx>=%d",sos_idx,HYHL_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    hyhl_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len >= HYHL_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = HYHL_DB_SOS_MAX_NUMBER_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_Sos_Reclist[sos_idx].sos_num,0x00,HYHL_DB_SOS_MAX_NUMBER_SIZE);
    SCI_MEMCPY(hyhl_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
    
    if(name_len >= HYHL_DB_SOS_MAX_NAME_SIZE)
    {
        name_len = HYHL_DB_SOS_MAX_NAME_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_Sos_Reclist[sos_idx].sos_name,0x00,HYHL_DB_SOS_MAX_NAME_SIZE);
    SCI_MEMCPY(hyhl_DB_Sos_Reclist[sos_idx].sos_name,sos_name,name_len);
    
    if(hyhl_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_SOS_ListModify(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len,char * sos_name,uint16 name_len)
{
    if (sos_idx >= HYHL_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_SOS_ListModifyBuf(%d) ERR idx>=%d",sos_idx,HYHL_DB_SOS_MAX_SUM);
        return FALSE;
    }
    	
    hyhl_DB_Sos_Reclist[sos_idx].db_id = sos_idx+1;
    if(num_len >= HYHL_DB_SOS_MAX_NUMBER_SIZE)
    {
        num_len = HYHL_DB_SOS_MAX_NUMBER_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_Sos_Reclist[sos_idx].sos_num,0x00,HYHL_DB_SOS_MAX_NUMBER_SIZE);
    SCI_MEMCPY(hyhl_DB_Sos_Reclist[sos_idx].sos_num,sos_num,num_len);
    
    if(name_len >= HYHL_DB_SOS_MAX_NAME_SIZE)
    {
        name_len = HYHL_DB_SOS_MAX_NAME_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_Sos_Reclist[sos_idx].sos_name,0x00,HYHL_DB_SOS_MAX_NAME_SIZE);
    SCI_MEMCPY(hyhl_DB_Sos_Reclist[sos_idx].sos_name,sos_name,name_len);
    
    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListDel(uint8  sos_idx)
{
    uint32 handletmp;

    if (sos_idx >= HYHL_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_SOS_ListDel(%d) ERR idx>=%d",sos_idx,HYHL_DB_SOS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&hyhl_DB_Sos_Reclist[sos_idx],0x00,sizeof(HYHL_DB_SOS_Type));
    
    if(hyhl_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_SOS_ListDel(%d) ERR NoInit",sos_idx);
        return FALSE;
    }

    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListDelBuf(uint8  sos_idx)
{
    if (sos_idx >= HYHL_DB_SOS_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_SOS_ListDelBuf(%d) ERR idx>=%d",sos_idx,HYHL_DB_SOS_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&hyhl_DB_Sos_Reclist[sos_idx],0x00,sizeof(HYHL_DB_SOS_Type));
    
    return TRUE;
}
BOOLEAN HYHL_DB_SOS_ListDelAll(void)
{
    uint32 handletmp;
    
    SCI_MEMSET(hyhl_DB_Sos_Reclist,0x00,sizeof(HYHL_DB_SOS_Type)* HYHL_DB_SOS_MAX_SUM);
    
    if(hyhl_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_SOS_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListDelAllBuf(void)
{
    SCI_MEMSET(hyhl_DB_Sos_Reclist,0x00,sizeof(HYHL_DB_SOS_Type)* HYHL_DB_SOS_MAX_SUM);
    return TRUE;
}

BOOLEAN HYHL_DB_SOS_ListSaveBuf(void)
{
    uint32 handletmp;
        
    if(hyhl_DB_Sos_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_SOS_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    return TRUE;
}

int HYHL_DB_SOS_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = strlen(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    HYHL_DB_SOS_ListInit();
    for(i = 0; i < HYHL_DB_SOS_MAX_SUM; i++)
    {
        if(hyhl_DB_Sos_Reclist[i].db_id != 0)
        {
            db_num_len = strlen(hyhl_DB_Sos_Reclist[i].sos_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,hyhl_DB_Sos_Reclist[i].sos_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN HYHL_DB_SOS_GetNameByNumber(uint16* num, uint16* name)
{
    char c_num[HYHL_DB_SOS_MAX_NUMBER_SIZE+1] = {0};
    uint16 num_len = 0;
    uint16 i = 0;
    uint16 db_num_len = 0;
    uint16 name_len = 0;
    
    if(num == NULL || name == NULL)
    {
        return FALSE;
    }
    for(i = 0; i < HYHL_DB_SOS_MAX_NUMBER_SIZE; i++)
    {
        if(num[i] == 0)
        {
            break;
        }
        c_num[i] = num[i];
    }
    num_len = strlen(c_num);
    if(num_len == 0)
    {
        return FALSE;
    }
    HYHL_DB_SOS_ListInit();
    for(i = 0; i < HYHL_DB_SOS_MAX_SUM; i++)
    {
        if(hyhl_DB_Sos_Reclist[i].db_id != 0)
        {
            db_num_len = strlen(hyhl_DB_Sos_Reclist[i].sos_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)c_num,hyhl_DB_Sos_Reclist[i].sos_num))
                {
                    name_len = strlen(hyhl_DB_Sos_Reclist[i].sos_name);
                    if( name_len > 0)
                    {
                        GUI_UTF8ToWstr(name, 32,(uint8 *)hyhl_DB_Sos_Reclist[i].sos_name, name_len);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}
BOOLEAN HYHL_DB_SOS_have_num(void)
{
    uint8 i = 0;	
    uint8 sos_idx = 0;	
	
    for(i = 0; i < HYHL_DB_SOS_MAX_SUM; i++)
    {
        if(hyhl_DB_Sos_Reclist[i].db_id != 0 && strlen(hyhl_DB_Sos_Reclist[i].sos_num) > 0)
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

#ifdef HYHL_DB_NEED_WHITE
static BOOLEAN hyhl_DB_White_is_init = FALSE;
HYHL_DB_WHITE_Type hyhl_DB_White_Reclist[HYHL_DB_WHITE_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN HYHL_DB_WHITE_ListInit(void)
{
    int i = 0;
    uint32 handletmp;
    int offset;
    offset = sizeof(HYHL_DB_WHITE_Type);
    
    if(hyhl_DB_White_is_init == TRUE)
    {
        return TRUE;
    }

    hyhl_DB_White_is_init = TRUE;
    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len)
{
    uint32 handletmp;
    
    if (white_idx >= HYHL_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListModify(%d) ERR idx>=%d",white_idx,HYHL_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    	
    hyhl_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len >= HYHL_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = HYHL_DB_WHITE_MAX_NUMBER_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_White_Reclist[white_idx].white_num,0x00,HYHL_DB_WHITE_MAX_NUMBER_SIZE);
    SCI_MEMCPY(hyhl_DB_White_Reclist[white_idx].white_num,white_num,num_len);
    
    if(name_len >= HYHL_DB_WHITE_MAX_NAME_SIZE)
    {
        name_len = HYHL_DB_WHITE_MAX_NAME_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_White_Reclist[white_idx].white_name,0x00,HYHL_DB_WHITE_MAX_NAME_SIZE);
    SCI_MEMCPY(hyhl_DB_White_Reclist[white_idx].white_name,white_name,name_len);
    
    if(hyhl_DB_White_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListModify(%d) ERR NoInit",white_idx);
        return FALSE;
    }

    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len)
{    
    if (white_idx >= HYHL_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListModifyBuf(%d) ERR idx>=%d",white_idx,HYHL_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    	
    hyhl_DB_White_Reclist[white_idx].db_id = white_idx+1;
    if(num_len >= HYHL_DB_WHITE_MAX_NUMBER_SIZE)
    {
        num_len = HYHL_DB_WHITE_MAX_NUMBER_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_White_Reclist[white_idx].white_num,0x00,HYHL_DB_WHITE_MAX_NUMBER_SIZE);
    SCI_MEMCPY(hyhl_DB_White_Reclist[white_idx].white_num,white_num,num_len);
    
    if(name_len >= HYHL_DB_WHITE_MAX_NAME_SIZE)
    {
        name_len = HYHL_DB_WHITE_MAX_NAME_SIZE-1;
    }
    SCI_MEMSET(hyhl_DB_White_Reclist[white_idx].white_name,0x00,HYHL_DB_WHITE_MAX_NAME_SIZE);
    SCI_MEMCPY(hyhl_DB_White_Reclist[white_idx].white_name,white_name,name_len);
    
    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListDel(uint8  white_idx)
{
    uint32 handletmp;

    if (white_idx >= HYHL_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,HYHL_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&hyhl_DB_White_Reclist[white_idx],0x00,sizeof(HYHL_DB_WHITE_Type));
    
    if(hyhl_DB_White_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListDel(%d) ERR NoInit",white_idx);
        return FALSE;
    }

    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListDelBuf(uint8  white_idx)
{
    if (white_idx >= HYHL_DB_WHITE_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListDel(%d) ERR idx>=%d",white_idx,HYHL_DB_WHITE_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&hyhl_DB_White_Reclist[white_idx],0x00,sizeof(HYHL_DB_WHITE_Type));
    
    return TRUE;
}
BOOLEAN HYHL_DB_WHITE_ListDelAll(void)
{
    uint32 handletmp;
    
    SCI_MEMSET(hyhl_DB_White_Reclist,0x00,sizeof(HYHL_DB_WHITE_Type)* HYHL_DB_WHITE_MAX_SUM);
    
    if(hyhl_DB_White_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListDelAll ERR NoInit");
        return FALSE;
    }
    
    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListDelAllBuf(void)
{
    SCI_MEMSET(hyhl_DB_White_Reclist,0x00,sizeof(HYHL_DB_WHITE_Type)* HYHL_DB_WHITE_MAX_SUM);
    return TRUE;
}

BOOLEAN HYHL_DB_WHITE_ListSaveBuf(void)
{
    uint32 handletmp;
        
    if(hyhl_DB_White_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_WHITE_ListSaveBuf ERR NoInit");
        return FALSE;
    }
    
    return TRUE;
}

int HYHL_DB_WHITE_IsValidNumber(char * num)
{
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    
    if(num == NULL)
    {
        return -1;
    }
    
    num_len = strlen(num);
    if(num_len == 0)
    {
        return -1;
    }
    
    HYHL_DB_WHITE_ListInit();
    for(i = 0; i < HYHL_DB_WHITE_MAX_SUM; i++)
    {
        if(hyhl_DB_White_Reclist[i].db_id != 0)
        {
            db_num_len = strlen(hyhl_DB_White_Reclist[i].white_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)num,hyhl_DB_White_Reclist[i].white_num))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

BOOLEAN HYHL_DB_WHITE_GetNameByNumber(uint16* num, uint16* name)
{
    char c_num[HYHL_DB_WHITE_MAX_NUMBER_SIZE+1] = {0};
    uint16 num_len = 0;
    uint16 i = 0;
    uint16 db_num_len = 0;
    uint16 name_len = 0;
    
    if(num == NULL || name == NULL)
    {
        return FALSE;
    }
    for(i = 0; i < HYHL_DB_WHITE_MAX_NUMBER_SIZE; i++)
    {
        if(num[i] == 0)
        {
            break;
        }
        c_num[i] = num[i];
    }
    num_len = strlen(c_num);
    if(num_len == 0)
    {
        return FALSE;
    }
    HYHL_DB_WHITE_ListInit();
    for(i = 0; i < HYHL_DB_WHITE_MAX_SUM; i++)
    {
        if(hyhl_DB_White_Reclist[i].db_id != 0)
        {
            db_num_len = strlen(hyhl_DB_White_Reclist[i].white_num);
            if(num_len >= db_num_len)
            {
                if(strstr((char *)c_num,hyhl_DB_White_Reclist[i].white_num))
                {
                    name_len = strlen(hyhl_DB_White_Reclist[i].white_name);
                    if( name_len > 0)
                    {
                        GUI_UTF8ToWstr(name, 32,(uint8 *)hyhl_DB_White_Reclist[i].white_name, name_len);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}
BOOLEAN HYHL_DB_White_have_num(void)
{
    uint8 i = 0;	
    uint8 idx = 0;	
	
    for(i = 0; i < HYHL_DB_WHITE_MAX_SUM; i++)
    {
        if(hyhl_DB_White_Reclist[i].db_id != 0 && strlen(hyhl_DB_White_Reclist[i].white_num) > 0)
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

#ifdef HYHL_DB_NEED_TIMESET

static BOOLEAN hyhl_DB_TimeSet_is_init = FALSE;
static HYHL_DB_TIMESET_Type hyhl_DB_TimeSet_Rec = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN HYHL_DB_TIMESET_ListInit(void)
{
    uint32 handletmp;
    int offset;
    offset = sizeof(HYHL_DB_TIMESET_Type);
    
    if(hyhl_DB_TimeSet_is_init == TRUE)
    {
        return TRUE;
    }

    hyhl_DB_TimeSet_is_init = TRUE;
    return TRUE;
}

BOOLEAN HYHL_DB_TIMESET_ListModify(uint32 time_start,uint32 time_end)
{
    uint32 handletmp;
    	
    hyhl_DB_TimeSet_Rec.db_id = 1;
    hyhl_DB_TimeSet_Rec.time_start = time_start;
    hyhl_DB_TimeSet_Rec.time_end = time_end;
    
    if(hyhl_DB_TimeSet_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_TIMESET_ListModify ERR NoInit");
        return FALSE;
    }

    return TRUE;
}

BOOLEAN HYHL_DB_TIMESET_ListDel(void)
{
    uint32 handletmp;
    
    SCI_MEMSET(&hyhl_DB_TimeSet_Rec,0x00,sizeof(HYHL_DB_TIMESET_Type));
    
    if(hyhl_DB_TimeSet_is_init == FALSE)
    {
        ZDT_LOG("HYHL_DB_TIMESET_ListDel ERR NoInit");
        return FALSE;
    }

    return TRUE;
}

static BOOLEAN HYHL_DB_TIMESET_IsValid(uint32 start,uint32 end,uint32 cur)
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

BOOLEAN HYHL_DB_TIMESET_CurTimeIsValid(void)
{
    BOOLEAN is_valid = FALSE;
    if(hyhl_DB_TimeSet_Rec.db_id != 0)
    {
        uint32 cur_daysecond = ZDT_Get_DayTimeSecond();
        is_valid = HYHL_DB_TIMESET_IsValid(hyhl_DB_TimeSet_Rec.time_start,hyhl_DB_TimeSet_Rec.time_end,cur_daysecond);
    }
    return is_valid;
}
#endif

#ifdef HYHL_DB_NEED_HIDDEN

static BOOLEAN hyhl_DB_Hidden_is_init = FALSE;
static HYHL_DB_HIDDEN_Type hyhl_DB_Hidden_Reclist[HYHL_DB_HIDDEN_MAX_SUM] = {0};

/*****************************************链表数据操作*****************************************************************/
BOOLEAN HYHL_DB_HIDDEN_ListInit(void)
{
    int i = 0;
    uint32 handletmp;
    int offset;
    offset = sizeof(HYHL_DB_HIDDEN_Type);
    
    if(hyhl_DB_Hidden_is_init == TRUE)
    {
        return TRUE;
    }

    hyhl_DB_Hidden_is_init = TRUE;
    return TRUE;
}

BOOLEAN HYHL_DB_HIDDEN_ListModify(uint8 hidden_idx,uint8 time_set,uint32 time_start,uint32 time_end)
{    
    if (hidden_idx >= HYHL_DB_HIDDEN_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_HIDDEN_ListModify(%d) ERR idx>=%d",hidden_idx,HYHL_DB_HIDDEN_MAX_SUM);
        return FALSE;
    }
    	
    hyhl_DB_Hidden_Reclist[hidden_idx].db_id = hidden_idx+1;
    hyhl_DB_Hidden_Reclist[hidden_idx].time_set = time_set;
    hyhl_DB_Hidden_Reclist[hidden_idx].time_start = time_start;
    hyhl_DB_Hidden_Reclist[hidden_idx].time_end = time_end;
    return TRUE;
}

BOOLEAN HYHL_DB_HIDDEN_ListDel(uint8  hidden_idx)
{
    if (hidden_idx >= HYHL_DB_HIDDEN_MAX_SUM)
    {
        ZDT_LOG("HYHL_DB_HIDDEN_ListDel(%d) ERR idx>=%d",hidden_idx,HYHL_DB_HIDDEN_MAX_SUM);
        return FALSE;
    }
    
    SCI_MEMSET(&hyhl_DB_Hidden_Reclist[hidden_idx],0x00,sizeof(HYHL_DB_HIDDEN_Type));
    return TRUE;
}

BOOLEAN HYHL_DB_HIDDEN_ListDelAll(void)
{    
    SCI_MEMSET(hyhl_DB_Hidden_Reclist,0x00,sizeof(HYHL_DB_HIDDEN_Type)* HYHL_DB_HIDDEN_MAX_SUM);
    return TRUE;
}

BOOLEAN HYHL_DB_HIDDEN_ListSave(void)
{     
    return TRUE;
}

static BOOLEAN HYHL_DB_HIDDEN_TimeIsValid(uint32 start,uint32 end,uint32 cur)
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
static BOOLEAN HYHL_DB_HIDDEN_WeekIsValid(uint8 type, uint8 cur)
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
    cur += 1;
    if(((0x01<<cur) & type) > 0)
    {
        ret = TRUE;
    }

    return ret;

}

BOOLEAN HYHL_DB_HIDDEN_IsValid(void)
{
    uint32 cur_daysecond = 0;
    BOOLEAN is_valid = FALSE;
    uint16 i = 0;
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    cur_daysecond = (time.hour* 3600) + (time.min * 60) + time.sec;
    
    for(i = 0; i < HYHL_DB_HIDDEN_MAX_SUM; i++)
    {
        if(hyhl_DB_Hidden_Reclist[i].db_id != 0)
        {
            if(HYHL_DB_HIDDEN_WeekIsValid(hyhl_DB_Hidden_Reclist[i].time_set,date.wday) 
                && HYHL_DB_HIDDEN_TimeIsValid(hyhl_DB_Hidden_Reclist[i].time_start,hyhl_DB_Hidden_Reclist[i].time_end,cur_daysecond)
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
BOOLEAN HYHL_DB_Init(HYHL_APP_T* pMe)
{
#ifdef HYHL_DB_NEED_SOS
    HYHL_DB_SOS_ListInit();
#if 1
    HYHL_DB_SOS_ListModify(0,"15994730009",11,"15994730009",11);
    HYHL_DB_SOS_ListModify(1,"112",3,"112",3);
    HYHL_DB_SOS_ListModify(2,"119",3,"119",3);
    HYHL_DB_SOS_ListModify(3,"10000",5,"10000",5);
#endif
#endif
#ifdef HYHL_DB_NEED_WHITE
    HYHL_DB_WHITE_ListInit();
#if 1
    HYHL_DB_WHITE_ListModify(0,"122",3,"122",3);
    HYHL_DB_WHITE_ListModify(1,"112",3,"112",3);
    HYHL_DB_WHITE_ListModify(2,"119",3,"119",3);
    HYHL_DB_WHITE_ListModify(3,"10000",5,"10000",5);
#endif
#endif
#ifdef HYHL_DB_NEED_TIMESET
    HYHL_DB_TIMESET_ListInit();
#endif
#ifdef HYHL_DB_NEED_HIDDEN
    HYHL_DB_HIDDEN_ListInit();
#endif
    return TRUE;
}

BOOLEAN HYHL_DB_DelAll(HYHL_APP_T* pMe)
{
#ifdef HYHL_DB_NEED_SOS
    HYHL_DB_SOS_ListDelAll();
#endif
#ifdef HYHL_DB_NEED_WHITE
    HYHL_DB_WHITE_ListDelAll();
#endif
#ifdef HYHL_DB_NEED_TIMESET
    HYHL_DB_TIMESET_ListDel();
#endif
#ifdef HYHL_DB_NEED_HIDDEN
    HYHL_DB_HIDDEN_ListDelAll();
    HYHL_DB_HIDDEN_ListSave();
#endif
    return TRUE;
}

BOOLEAN HYHL_DB_Free(HYHL_APP_T* pMe)
{
	return TRUE;
}
#endif
