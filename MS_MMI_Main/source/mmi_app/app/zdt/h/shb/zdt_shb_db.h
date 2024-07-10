
#ifndef _ZDT_SHB_DB_H_
#define _ZDT_SHB_DB_H_

#include "zdt_common.h"

#define SHB_DB_NEED_SETTING
#define SHB_DB_NEED_APPSET
#define SHB_DB_NEED_PLATSET
#define SHB_DB_NEED_SOS
#define SHB_DB_NEED_SPD
#define SHB_DB_NEED_WHITE
//#define SHB_DB_NEED_CLOCK
#define SHB_DB_NEED_TIMER
#define SHB_DB_NEED_CLOCK

#define SHB_DB_DIR "SHBDB"

#define MAX_SHB_APP_DOMAIN_SIZE 100
#define MAX_SHB_APP_NUMBER_SIZE 20
#define MAX_SHB_APP_ICCID_SIZE 20

#ifdef SHB_DB_NEED_SETTING
#define SHB_DB_SET_FS_FILE "shb_setting_DBFile.db"

#define MAX_SHB_IP_DOMAIN_SIZE 100
#define MAX_SHB_SMS_NUMBER_SIZE 40

typedef struct _SHB_DB_SET_Type
{
    uint8 version;
    uint8 db_id;
    uint8 net_open;
    uint8 is_reg;
    uint8 is_land;
    uint16 white_type;
    uint16 ip_port;
    uint16 time_interval;
    uint8  ip_domain[MAX_SHB_IP_DOMAIN_SIZE+1];
    uint8  sms_ct[MAX_SHB_SMS_NUMBER_SIZE+1];  //电信号
    uint8  sms_cmcc[MAX_SHB_SMS_NUMBER_SIZE+1]; //移动号
    uint8  sms_cu[MAX_SHB_SMS_NUMBER_SIZE+1];   //联通号
}SHB_DB_SET_Type;

extern SHB_DB_SET_Type shb_DB_Set_Rec;
extern BOOLEAN SHB_DB_SET_ListInit(void);
extern BOOLEAN SHB_DB_SET_ListModify(void);
extern BOOLEAN SHB_DB_SET_ListDel(void);
#endif

#ifdef SHB_DB_NEED_APPSET
#define SHB_DB_APPSET_FS_FILE "shb_appset_DBFile.db"


typedef struct _SHB_DB_APPSET_Type
{
    uint8 version;
    uint8 db_id;
    uint16 wchat_port;
    uint8 iccid[MAX_SHB_APP_ICCID_SIZE+1];  //ICCID
    uint8 slef_num[MAX_SHB_APP_NUMBER_SIZE+1];  //终端号码
    uint8 shb_num[MAX_SHB_APP_NUMBER_SIZE+1];  //平台接入号
    uint8 app_domain[MAX_SHB_APP_DOMAIN_SIZE+1];  //守护宝协议服务地址
    uint8 wchat_domain[MAX_SHB_APP_DOMAIN_SIZE+1];  //微聊服务地址
    uint8 reserv1[MAX_SHB_APP_DOMAIN_SIZE+1];  //备用地址1
    uint8 reserv2[MAX_SHB_APP_DOMAIN_SIZE+1];  //备用地址2
    uint8 reserv3[MAX_SHB_APP_DOMAIN_SIZE+1];  //备用地址3
}SHB_DB_APPSET_Type;

extern SHB_DB_APPSET_Type shb_DB_AppSet_Rec;
extern BOOLEAN SHB_DB_APPSET_ListInit(void);
extern BOOLEAN SHB_DB_APPSET_ListModify(void);
extern BOOLEAN SHB_DB_APPSET_ListDel(void);
#endif

#ifdef SHB_DB_NEED_PLATSET
#define SHB_DB_PLATSET_FS_FILE "shb_platset_DBFile.db"

typedef struct _SHB_DB_PLATSET_Type
{
    uint8 version;
    uint8 db_id;
    //FUNC0
    uint8 platNum[MAX_SHB_APP_NUMBER_SIZE+1];  //平台接入号
    uint8 mobile[MAX_SHB_APP_NUMBER_SIZE+1];  //终端手机号码
}SHB_DB_PLATSET_Type;

extern SHB_DB_PLATSET_Type shb_DB_PlatSet_Rec;
extern BOOLEAN SHB_DB_PLATSET_ListInit(void);
extern BOOLEAN SHB_DB_PLATSET_ListModify(void);
extern BOOLEAN SHB_DB_PLATSET_ListDel(void);
#endif

#ifdef SHB_DB_NEED_SOS
#define SHB_DB_SOS_MAX_SUM                     5
#define SHB_DB_SOS_MAX_NUMBER_SIZE             20

#define SHB_DB_SOS_FS_FILE "shbsos_DBFile.db"

typedef struct _SHB_DB_SOS_Type
{
    uint8 db_id;
    char  sos_num[SHB_DB_SOS_MAX_NUMBER_SIZE+1];
}SHB_DB_SOS_Type;

extern SHB_DB_SOS_Type shb_DB_Sos_Reclist[SHB_DB_SOS_MAX_SUM];

extern BOOLEAN SHB_DB_SOS_ListInit(void);
extern BOOLEAN SHB_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len);
extern BOOLEAN SHB_DB_SOS_ListDel(uint8  sos_idx);
extern BOOLEAN SHB_DB_SOS_ListDelAll(void);
extern BOOLEAN SHB_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len);
extern BOOLEAN SHB_DB_SOS_ListDelBuf(uint8  sos_idx);
extern BOOLEAN SHB_DB_SOS_ListDelAllBuf(void);
extern BOOLEAN SHB_DB_SOS_ListSaveBuf(void);
extern int SHB_DB_SOS_IsValidNumber(char * num);
#endif

#ifdef SHB_DB_NEED_SPD
#define SHB_DB_SPD_MAX_SUM                     9
#define SHB_DB_SPD_MAX_NUMBER_SIZE             20

#define SHB_DB_SPD_FS_FILE "shbspd_DBFile.db"

typedef struct _SHB_DB_SPD_Type
{
    uint8 db_id;
    char  spd_num[SHB_DB_SPD_MAX_NUMBER_SIZE+1];
}SHB_DB_SPD_Type;

extern SHB_DB_SPD_Type shb_DB_Spd_Reclist[SHB_DB_SPD_MAX_SUM];

extern BOOLEAN SHB_DB_SPD_ListInit(void);
extern BOOLEAN SHB_DB_SPD_ListModify(uint8 spd_idx,char * spd_num,uint16 num_len);
extern BOOLEAN SHB_DB_SPD_ListDel(uint8  spd_idx);
extern BOOLEAN SHB_DB_SPD_ListDelAll(void);
extern BOOLEAN SHB_DB_SPD_ListModifyBuf(uint8 spd_idx,char * spd_num,uint16 num_len);
extern BOOLEAN SHB_DB_SPD_ListDelBuf(uint8  spd_idx);
extern BOOLEAN SHB_DB_SPD_ListDelAllBuf(void);
extern BOOLEAN SHB_DB_SPD_ListSaveBuf(void);
extern int SHB_DB_SPD_IsValidNumber(char * num);
#endif

#ifdef SHB_DB_NEED_WHITE
#define SHB_DB_WHITE_MAX_SUM                     20
#define SHB_DB_WHITE_MAX_NUMBER_SIZE             20
#define SHB_DB_WHITE_MAX_NAME_SIZE             40

#define SHB_DB_WHITE_FS_FILE "shbwhite_DBFile.db"

typedef struct _SHB_DB_WHITE_Type
{
    uint8 db_id;
    char  white_num[SHB_DB_WHITE_MAX_NUMBER_SIZE+1];
}SHB_DB_WHITE_Type;

extern SHB_DB_WHITE_Type shb_DB_White_Reclist[SHB_DB_WHITE_MAX_SUM];

extern BOOLEAN SHB_DB_WHITE_ListInit(void);
extern BOOLEAN SHB_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len);
extern BOOLEAN SHB_DB_WHITE_ListDel(uint8  white_idx);
extern BOOLEAN SHB_DB_WHITE_ListDelAll(void);
extern BOOLEAN SHB_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len);
extern BOOLEAN SHB_DB_WHITE_ListDelBuf(uint8  white_idx);
extern BOOLEAN SHB_DB_WHITE_ListDelAllBuf(void);
extern BOOLEAN SHB_DB_WHITE_ListSaveBuf(void);
extern int SHB_DB_WHITE_IsValidNumber(char * num);
#endif

#ifdef SHB_DB_NEED_CLOCK
#define SHB_DB_CLOCK_MAX_SUM                     5
#define SHB_DB_CLOCK_MAX_CONTENT_SIZE             120

#define SHB_DB_CLOCK_FS_FILE "shbclock_DBFile.db"

typedef struct _SHB_DB_CLOCK_Type
{
    uint8 db_id;
    uint8 clock_onoff;
    uint8 clock_week;
    uint8  clock_content[SHB_DB_CLOCK_MAX_CONTENT_SIZE+1];
    uint32 clock_id;
    uint32 clock_time;
    uint32 clock_type;
}SHB_DB_CLOCK_Type;

extern BOOLEAN SHB_DB_CLOCK_ListInit(void);
extern BOOLEAN SHB_DB_CLOCK_ListModify(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len);
extern BOOLEAN SHB_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len);
extern BOOLEAN SHB_DB_CLOCK_ListDel(uint8  clock_idx);
extern BOOLEAN SHB_DB_CLOCK_ListDelBuf(uint8  clock_idx);
extern BOOLEAN SHB_DB_CLOCK_ListDelAll(void);
extern BOOLEAN SHB_DB_CLOCK_ListDelAllBuf(void);
extern BOOLEAN SHB_DB_CLOCK_ListSaveBuf(void);

#endif

#ifdef SHB_DB_NEED_TIMER
typedef  enum
{
    SHB_TIME_TYPE_LOC_1,
    SHB_TIME_TYPE_LOC_2,
    SHB_TIME_TYPE_LOC_3,
    SHB_TIME_TYPE_LOC_4,
    SHB_TIME_TYPE_LOC_5,
    SHB_TIME_TYPE_LOC_6,
    SHB_TIME_TYPE_LOC_7,
    SHB_TIME_TYPE_LOC_8,
    SHB_TIME_TYPE_SLIENT_1,
    SHB_TIME_TYPE_SLIENT_2,
    SHB_TIME_TYPE_SLIENT_3,
    SHB_TIME_TYPE_SLIENT_4,
    SHB_TIME_TYPE_CR_1,
    SHB_TIME_TYPE_CR_2,
    SHB_TIME_TYPE_CR_3,
    SHB_TIME_TYPE_MAX
}SHB_TIMER_TYPE_E;


#define SHB_DB_TIMER_MAX_SUM                     16

#define SHB_DB_TIMER_FS_FILE "shbtimer_DBFile.db"
#define SHB_DB_TIME_MAX_ID_SIZE             40
typedef struct _SHB_DB_TIMER_Type
{
    uint8 db_id;
    uint8 time_set;
    uint8  time_id_str[SHB_DB_TIME_MAX_ID_SIZE+1];
    uint32 time_id;
    uint32 time_start;
    uint32 time_end;
    uint32 time_interval_min;
}SHB_DB_TIMER_Type;

extern BOOLEAN SHB_DB_TIMER_ListInit(void);
extern BOOLEAN SHB_DB_TIMER_ListModify(uint8 timer_idx,uint8 * time_id_str,uint8 time_set,uint32 time_start,uint32 time_end,uint32 time_interval_min);
extern BOOLEAN SHB_DB_TIMER_ListDel(uint8  timer_idx);
extern BOOLEAN SHB_DB_TIMER_ListDelAll(void);
extern BOOLEAN SHB_DB_TIMER_ListSave(void);
BOOLEAN SHB_DB_TIMER_IsValid(uint16 idx);
BOOLEAN SHB_DB_TIMER_IsValid_Loc(uint32  * pIntervalMin,uint8 * pTtimeID);
BOOLEAN SHB_DB_TIMER_IsValid_Slient(void);
BOOLEAN SHB_DB_TIMER_IsValid_CR(uint32  * pIntervalMin,uint8 * pTtimeID);
#endif

BOOLEAN SHB_DB_Init(SHB_APP_T* pMe);
BOOLEAN SHB_DB_DelAll(SHB_APP_T* pMe);
BOOLEAN SHB_DB_Free(SHB_APP_T* pMe);


#endif /*_ZDT_SHB_DB_H_*/
