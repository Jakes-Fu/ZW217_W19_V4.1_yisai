
#ifndef _ZDT_ZDT_DB_H_
#define _ZDT_ZDT_DB_H_

#include "zdt_common.h"

#define ZDT_DB_DIR "ZDTDB"

#define ZDT_DB_NEED_APPCOUNT

#ifdef ZDT_DB_NEED_APPCOUNT

#define ZDT_DB_APPCOUNT_FS_FILE "zdt_appcount_DBFile.db"

typedef struct _ZDT_DB_APPCOUNT_Type
{
    uint8 version;
    uint8 db_id;
    uint32 index;
    uint32 PEDOMETER; //计步器使用，字符串型
    uint32 ALIPAY; //支付宝使用，字符串型
    uint32 CLOCK; //闹钟使用，字符串型
    uint32 CALENDAR; //日历使用，字符串型
    uint32 MESSAGE; //短信使用，字符串型
    uint32 CALL_RECORD; //通话记录使用，字符串型
    uint32 HOMEWORK; //作业使用，字符串型
    uint32 ANSWERCARD; //答题卡使用，字符串型
    uint32 ADDRESSBOOK; //通讯录使用，字符串型
    uint32 HEALTHCODE; //健康码使用，字符串型
    uint32 IDCODE; //身份码使用，字符串型
}ZDT_DB_APPCOUNT_Type;

extern ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Rec;
extern ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Bak;
extern BOOLEAN ZDT_DB_APPCOUNT_ListInit(void);
extern BOOLEAN ZDT_DB_APPCOUNT_ListModify(void);
extern BOOLEAN ZDT_DB_APPCOUNT_ListDel(void);
extern BOOLEAN ZDT_DB_APPCOUNT_IsChange(void);
#endif

#ifdef ZDT_ADD_FIRST_POWER_RECORD
#define DB_POWERON_TIME_FS_FILE "poweron_DBFile.db"
typedef struct _DB_Poweron_Time_Type
{
    uint8 is_save;
    uint32  poweron_time;
}DB_Poweron_Time_Type;
extern DB_Poweron_Time_Type DB_Poweron_time;
extern BOOLEAN DB_Poweron_Time_ListSaveBuf(void);
extern BOOLEAN DB_Poweron_Time_ListModifyBuf(uint32 times);
extern BOOLEAN DB_Poweron_Time_ListDel(void);
#endif

BOOLEAN ZDT_DB_Init(void);
BOOLEAN ZDT_DB_DelAll(void);
BOOLEAN ZDT_DB_Free(void);


#endif /*_ZDT_YX_DB_H_*/
