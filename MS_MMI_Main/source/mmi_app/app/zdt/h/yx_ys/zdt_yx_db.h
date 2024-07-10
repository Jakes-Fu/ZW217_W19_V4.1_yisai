
#ifndef _ZDT_YX_DB_H_
#define _ZDT_YX_DB_H_

#include "zdt_common.h"
#include "zdt_yx_api.h"

#define YX_DB_NEED_SETTING
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#define YX_DB_NEED_VOC
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_SOS
#define YX_DB_NEED_SOS
#endif
#define YX_DB_NEED_WHITE
#define YX_DB_NEED_CLOCK
#define YX_DB_NEED_TIMER

#define YX_DB_DIR "YSDB"
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
#define YX_DB_NEED_FRIEND
#endif

#ifdef YX_DB_NEED_SETTING
#ifdef FEATURE_YX_AKQ
#define YX_DB_SET_FS_FILE "akq_setting_DBFile.db"
#else
#define YX_DB_SET_FS_FILE "yx_setting_DBFile.db"
#endif

#define MAX_YX_IP_DOMAIN_SIZE 100
#define MAX_YX_CENTER_NUMBER_SIZE 40
#define MAX_YX_PASSWORD_SIZE 40

typedef struct _YX_DB_SET_Type
{
    uint8 version;
    uint8 db_id;
    uint8 language;
    uint8 alert_sos_sms;
    uint8 alert_low_bat;
    uint8 alert_loser;
    uint8 alert_loser_sms;
    uint8 alert_jp;
    uint8 profile;
    uint8 falldown;
    uint8 zone;
    uint8 net_open;
    uint16 heart;
    uint16 body_tw_peroid;//tw update peroid
    uint16 ip_port;
    uint16 app_port;
    uint16 flower;
    uint16 time_interval;
    uint32 validity;
    uint8  center_number[MAX_YX_CENTER_NUMBER_SIZE+1];
    uint8  password[MAX_YX_PASSWORD_SIZE+1];
    uint8  ip_domain[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  app_domain[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  imei[16];
}YX_DB_SET_Type;

extern YX_DB_SET_Type yx_DB_Set_Rec;
extern BOOLEAN YX_DB_SET_ListInit(void);
extern BOOLEAN YX_DB_SET_ListModify(void);
extern BOOLEAN YX_DB_SET_ListDel(void);
#endif


#if 1//add_schedule
#define SCHEDULE_DAY_MAX      7
#define SCHEDULE_TIME_MUN_MAX      10
#define SCHEDULE_TIME_MAX_LEN       24//64
#define SCHEDULE_CLASS_MAX_LEN      24//64

#define DB_SCHEDULE_FS_FILE "schedule_DBFile.db"

typedef struct _class_content_Type
{
	//wchar  class_content_wstr[64+1];
	char  class_content_str[64+1];
	uint16 class_content_len;
}class_content_Type;

typedef struct _YX_SCHEDULE_Type
{
   // uint8 schedule_time_end[SCHEDULE_TIME_MUN_MAX+1]; //课程最后时间 [小时]
    class_content_Type class_content_Type[SCHEDULE_DAY_MAX+1][SCHEDULE_CLASS_MAX_LEN+1]; //课程 [星期][课程]
    uint8 schedule_time[SCHEDULE_TIME_MUN_MAX+1][12+1];
}YX_SCHEDULE_Type;

extern YX_SCHEDULE_Type  yx_schedule_Rec;
extern BOOLEAN DB_Schedule_ListSaveBuf(void);
extern BOOLEAN DB_Schedule_ListDel(void);

#endif

#ifdef YX_DB_NEED_SOS
#define YX_DB_SOS_MAX_SUM                     3
#define YX_DB_SOS_MAX_NUMBER_SIZE             40

#ifdef FEATURE_YX_AKQ
#define YX_DB_SOS_FS_FILE "akqsos_DBFile.db"
#else
#define YX_DB_SOS_FS_FILE "yxsos_DBFile.db"
#endif

typedef struct _YX_DB_SOS_Type
{
    uint8 db_id;
    char  sos_num[YX_DB_SOS_MAX_NUMBER_SIZE+1];
}YX_DB_SOS_Type;

extern YX_DB_SOS_Type yx_DB_Sos_Reclist[YX_DB_SOS_MAX_SUM];

extern BOOLEAN YX_DB_SOS_ListInit(void);
extern BOOLEAN YX_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len);
extern BOOLEAN YX_DB_SOS_ListDel(uint8  sos_idx);
extern BOOLEAN YX_DB_SOS_ListDelAll(void);
extern BOOLEAN YX_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len);
extern BOOLEAN YX_DB_SOS_ListDelBuf(uint8  sos_idx);
extern BOOLEAN YX_DB_SOS_ListDelAllBuf(void);
extern BOOLEAN YX_DB_SOS_ListSaveBuf(void);
extern int YX_DB_SOS_IsValidNumber(char * num);
#endif

#ifdef YX_DB_NEED_WHITE
#define YX_DB_WHITE_MAX_SUM                     50
#define YX_DB_WHITE_MAX_NUMBER_SIZE             20
#define YX_DB_WHITE_MAX_NAME_SIZE             40

#ifdef FEATURE_YX_AKQ
#define YX_DB_WHITE_FS_FILE "akqwhite_DBFile.db"
#else
#define YX_DB_WHITE_FS_FILE "yxwhite_DBFile.db"
#endif

typedef struct _YX_DB_WHITE_Type
{
    uint8 db_id;
    char  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
    char  white_name[YX_DB_WHITE_MAX_NAME_SIZE+1];
}YX_DB_WHITE_Type;

extern YX_DB_WHITE_Type yx_DB_White_Reclist[YX_DB_WHITE_MAX_SUM];

extern BOOLEAN YX_DB_WHITE_ListInit(void);
extern BOOLEAN YX_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len);
extern BOOLEAN YX_DB_WHITE_ListDel(uint8  white_idx);
extern BOOLEAN YX_DB_WHITE_ListDelAll(void);
extern BOOLEAN YX_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len);
extern BOOLEAN YX_DB_WHITE_ListDelBuf(uint8  white_idx);
extern BOOLEAN YX_DB_WHITE_ListDelAllBuf(void);
extern BOOLEAN YX_DB_WHITE_ListSaveBuf(void);
extern int YX_DB_WHITE_IsValidNumber(char * num);
#endif

#ifdef YX_DB_NEED_CLOCK
//#define YX_DB_CLOCK_MAX_SUM                     (ALARM_CLOCK_NUMBER - ALARM_CLOCK_DISP_NUMBER)
#define YX_DB_CLOCK_MAX_SUM                     5

#ifdef FEATURE_YX_AKQ
#define YX_DB_CLOCK_FS_FILE "akqclock_DBFile.db"
#else
#define YX_DB_CLOCK_FS_FILE "yxclock_DBFile.db"
#endif

typedef struct _YX_DB_CLOCK_Type
{
    uint8 db_id;
    uint8 clock_onoff;
    uint8 clock_type;
    uint8 clock_week;
    uint32 clock_time;
}YX_DB_CLOCK_Type;

extern BOOLEAN YX_DB_CLOCK_ListInit(void);
extern BOOLEAN YX_DB_CLOCK_ListModify(uint8 clock_idx,uint8 clock_onoff,uint8 clock_type,uint8 clock_week,uint32 clock_time);
extern BOOLEAN YX_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint8 clock_onoff,uint8 clock_type,uint8 clock_week,uint32 clock_time);
extern BOOLEAN YX_DB_CLOCK_ListDel(uint8  clock_idx);
extern BOOLEAN YX_DB_CLOCK_ListDelBuf(uint8  clock_idx);
extern BOOLEAN YX_DB_CLOCK_ListDelAll(void);
extern BOOLEAN YX_DB_CLOCK_ListDelAllBuf(void);
extern BOOLEAN YX_DB_CLOCK_ListSaveBuf(void);

#endif

#ifdef YX_DB_NEED_TIMER
typedef  enum
{
    YX_TIME_TYPE_JP_1,
    YX_TIME_TYPE_JP_2,
    YX_TIME_TYPE_JP_3,
    YX_TIME_TYPE_FG_1,
    YX_TIME_TYPE_FG_2,
    YX_TIME_TYPE_SLIENT_1,
    YX_TIME_TYPE_SLIENT_2,
    YX_TIME_TYPE_SLIENT_3,
    YX_TIME_TYPE_SLIENT_4,
    YX_TIME_TYPE_SLIENT_5,
    YX_TIME_TYPE_MAX
}YX_TIMER_TYPE_E;


#define YX_DB_TIMER_MAX_SUM                     20

#ifdef FEATURE_YX_AKQ
#define YX_DB_TIMER_FS_FILE "akqtimer_DBFile.db"
#else
#define YX_DB_TIMER_FS_FILE "yxtimer_DBFile.db"
#endif

typedef struct _YX_DB_TIMER_Type
{
    uint8 db_id;
    uint8 time_set;
    uint32 time_start;
    uint32 time_end;
}YX_DB_TIMER_Type;

extern BOOLEAN YX_DB_TIMER_ListInit(void);
extern BOOLEAN YX_DB_TIMER_ListModify(uint8 timer_idx,uint8 time_set,uint32 time_start,uint32 time_end);
extern BOOLEAN YX_DB_TIMER_ListDel(uint8  timer_idx);
extern BOOLEAN YX_DB_TIMER_ListDelAll(void);
extern BOOLEAN YX_DB_TIMER_ListSave(void);
BOOLEAN YX_DB_TIMER_IsValid(uint16 idx);
BOOLEAN YX_DB_TIMER_IsValid_JP(void);
BOOLEAN YX_DB_TIMER_IsValid_FG(void);
BOOLEAN YX_DB_TIMER_IsValid_Slient(void);
BOOLEAN YX_DB_JP_IsValid(void);
#endif

#ifdef YX_DB_NEED_FRIEND
#define YX_DB_FRIEND_MAX_SUM                     20
#define YX_DB_FRIEND_MAX_ID_SIZE             20
#define YX_DB_FRIEND_MAX_NUMBER_SIZE             20
#define YX_DB_FRIEND_MAX_NAME_SIZE             40

#define YX_DB_FRIEND_FS_FILE "yxfriend_DBFile.db"

typedef struct _YX_DB_FRIEND_Type
{
    uint8 db_id;
    uint8  friend_id[YX_DB_FRIEND_MAX_ID_SIZE+1];
    uint8  friend_num[YX_DB_FRIEND_MAX_NUMBER_SIZE+1];
    uint8  friend_name[YX_DB_FRIEND_MAX_NAME_SIZE+1];
}YX_DB_FRIEND_Type;

extern YX_DB_FRIEND_Type yx_DB_Friend_Reclist[YX_DB_FRIEND_MAX_SUM];

extern BOOLEAN YX_DB_FRIEND_ListInit(void);
extern BOOLEAN YX_DB_FRIEND_ListModify(uint8 friend_idx,uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len);
extern BOOLEAN YX_DB_FRIEND_ListDel(uint8  friend_idx);
extern BOOLEAN YX_DB_FRIEND_ListDelAll(void);
extern BOOLEAN YX_DB_FRIEND_ListModifyBuf(uint8 friend_idx,uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len);
extern BOOLEAN YX_DB_FRIEND_ListDelBuf(uint8  friend_idx);
extern BOOLEAN YX_DB_FRIEND_ListDelAllBuf(void);
extern BOOLEAN YX_DB_FRIEND_ListSaveBuf(void);
extern int YX_DB_FRIEND_IsValidNumber(char * num);
extern uint8  YX_DB_Friend_Add(uint8 * friend_id,uint16 id_len,uint8 * friend_num,uint16 num_len,uint8 * friend_name,uint16 name_len);
extern uint8 YX_DB_Friend_GetSum(void);

#endif

BOOLEAN YX_DB_Init(YX_APP_T* pMe);
BOOLEAN YX_DB_DelAll(YX_APP_T* pMe);
BOOLEAN YX_DB_Free(YX_APP_T* pMe);


#endif /*_ZDT_YX_DB_H_*/
