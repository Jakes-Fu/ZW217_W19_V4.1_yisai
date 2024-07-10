
#ifndef _ZDT_YX_DB_H_
#define _ZDT_YX_DB_H_

#include "zdt_common.h"
#include "zdt_yx_voice.h"

#define YX_DB_NEED_SETTING
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#define YX_DB_NEED_VOC
#endif
#define YX_DB_NEED_SOS
#define YX_DB_NEED_WHITE
#define YX_DB_NEED_CLOCK
#define YX_DB_NEED_TIMER
#define YX_DB_NEED_SPD
#define YX_DB_NEED_NETSMS
#define YX_DB_NEED_REGIONAL
#define YX_DB_DIR "YSDB"

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
    uint8  in_call_mode; // 1、无限制2、限制白名单以外的号码呼入3、限制所有号码呼入
    uint8  in_call_cycle;
    uint16 heart;
    uint16 body_tw_peroid;
    uint16 reg_ip_port;
    uint16 ip_port;
    uint16 app_port;
    uint16 flower;
    uint16 time_interval;
    uint16 heart_interval;
    uint8 env_ring;
    uint8 env_mute;
    uint8 mo_call_limit;//呼出
    uint8 mt_call_limit;//呼入
    uint8 sos_mt;
    uint8 sos_mo;
    uint8  center_number[MAX_YX_CENTER_NUMBER_SIZE+1];
    uint8  password[MAX_YX_PASSWORD_SIZE+1];
    uint8  reg_ip_domain[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  ip_domain[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  app_domain[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  first_time[MAX_YX_IP_DOMAIN_SIZE+1];
    uint8  imei[16];
}YX_DB_SET_Type;

extern YX_DB_SET_Type yx_DB_Set_Rec;
extern BOOLEAN YX_DB_SET_ListInit(void);
extern BOOLEAN YX_DB_SET_ListModify(void);
extern BOOLEAN YX_DB_SET_ListDel(void);
extern BOOLEAN YX_DB_SET_HaveIP(void);
extern BOOLEAN YX_DB_SET_ClearIP(void);
#endif

#ifdef YX_DB_NEED_VOC
typedef struct _YX_DB_VOCRCV_Type
{
    uint32 r_time;
    uint32 voc_idx;
    char  r_name[MAX_YX_VOC_FILENAME_SIZE];
}YX_DB_VOCRCV_Type;

typedef struct yx_db_vocrcv_index_type_s
{
    uint16                            list_id;
    uint32 r_time;
    uint32 voc_idx;
    char  r_name[MAX_YX_VOC_FILENAME_SIZE];
   struct yx_db_vocrcv_index_type_s     *db_index_next;/*下一接点指针,最后一个指针为空*/  
   struct yx_db_vocrcv_index_type_s     *db_index_prev;/*上一接点指针*/   
}yx_db_vocrcv_index_type;


BOOLEAN YX_DB_VocRcv_ListInit(YX_APP_T *pMe);
BOOLEAN YX_DB_VocRcv_ListFree(YX_APP_T *pMe);
uint16 YX_DB_VocRcv_ListNumber(YX_APP_T *pMe);
yx_db_vocrcv_index_type  *YX_DB_VocRcv_ListHead(YX_APP_T *pMe);
BOOLEAN YX_DB_VocRcv_ListDeleteAll(YX_APP_T *pMe);
BOOLEAN YX_DB_VocRcv_ListDeleteOne(YX_APP_T *pMe,uint16 list_id);
BOOLEAN YX_DB_VocRcv_ListInsertOne(YX_APP_T *pMe,YX_DB_VOCRCV_Type  *pInBuffer);
BOOLEAN YX_DB_VocRcv_ListModifyOne(YX_APP_T *pMe,uint16 list_id,YX_DB_VOCRCV_Type  *pInBuffer);
BOOLEAN YX_DB_VocRcv_ListInsertByName(YX_APP_T *pMe,char *pInBuffer);
BOOLEAN YX_DB_VocRcv_ListGetCurList(YX_APP_T *pMe,uint16 list_id, yx_db_vocrcv_index_type  *list);
BOOLEAN YX_DB_VocRcv_ListGetFirstByTime(YX_APP_T *pMe,yx_db_vocrcv_index_type  *list);
BOOLEAN YX_DB_VocRcv_ListGetLastByTime(YX_APP_T *pMe,yx_db_vocrcv_index_type  *list);
BOOLEAN YX_DB_VocRcvAdd(YX_APP_T *pMe,char * file_name);
BOOLEAN YX_DB_VocRcvDel(YX_APP_T *pMe,uint16  List_Id);
uint32 YX_DB_GetVocRcvNum(void);
#endif

#ifdef YX_DB_NEED_SOS
#define YX_DB_SOS_MAX_SUM                     5
#define YX_DB_SOS_MAX_NUMBER_SIZE             20
#define YX_DB_SOS_MAX_NAME_SIZE             40

#ifdef FEATURE_YX_AKQ
#define YX_DB_SOS_FS_FILE "akqsos_DBFile.db"
#else
#define YX_DB_SOS_FS_FILE "yxsos_DBFile.db"
#endif

typedef struct _YX_DB_SOS_Type
{
    uint8 db_id;
    uint8  sos_name[YX_DB_SOS_MAX_NAME_SIZE+1];
    uint8  sos_num[YX_DB_SOS_MAX_NUMBER_SIZE+1];
}YX_DB_SOS_Type;

extern YX_DB_SOS_Type yx_DB_Sos_Reclist[YX_DB_SOS_MAX_SUM];

extern BOOLEAN YX_DB_SOS_ListInit(void);
extern BOOLEAN YX_DB_SOS_ListModify(uint8 sos_idx,YX_DB_SOS_Type * pData);
extern BOOLEAN YX_DB_SOS_ListDel(uint8  sos_idx);
extern BOOLEAN YX_DB_SOS_ListDelAll(void);
extern BOOLEAN YX_DB_SOS_ListModifyBuf(uint8 sos_idx,YX_DB_SOS_Type * pData);
extern BOOLEAN YX_DB_SOS_ListDelBuf(uint8  sos_idx);
extern BOOLEAN YX_DB_SOS_ListDelAllBuf(void);
extern BOOLEAN YX_DB_SOS_ListSaveBuf(void);
extern int YX_DB_SOS_IsValidNumber(char * num);
#endif

#ifdef YX_DB_NEED_WHITE
#define YX_DB_WHITE_MAX_SUM                     20
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
    uint32 time1_start;
    uint32 time1_end;
    uint32 time2_start;
    uint32 time2_end;
    uint8  white_name[YX_DB_WHITE_MAX_NAME_SIZE+1];
    uint8  white_num[YX_DB_WHITE_MAX_NUMBER_SIZE+1];
}YX_DB_WHITE_Type;

extern YX_DB_WHITE_Type yx_DB_White_Reclist[YX_DB_WHITE_MAX_SUM];

extern BOOLEAN YX_DB_WHITE_ListInit(void);
extern BOOLEAN YX_DB_WHITE_ListModify(uint8 white_idx,YX_DB_WHITE_Type * pData);
extern BOOLEAN YX_DB_WHITE_ListDel(uint8  white_idx);
extern BOOLEAN YX_DB_WHITE_ListDelAll(void);
extern BOOLEAN YX_DB_WHITE_ListModifyBuf(uint8 white_idx,YX_DB_WHITE_Type * pData);
extern BOOLEAN YX_DB_WHITE_ListDelBuf(uint8  white_idx);
extern BOOLEAN YX_DB_WHITE_ListDelAllBuf(void);
extern BOOLEAN YX_DB_WHITE_ListSaveBuf(void);
extern int YX_DB_WHITE_IsValidNumber(char * num);
extern int YX_DB_WHITE_IsValidNumberExt(char * num);
extern int YX_DB_WHITE_IsValidNumberSMS(char * num);
extern BOOLEAN YX_DB_WHITE_DeleteNumber(char * num);
extern int YX_DB_WHITE_AddNumber(char * num,YX_DB_WHITE_Type * pData);
extern BOOLEAN YX_DB_White_have_num(void);

#endif

#ifdef YX_DB_NEED_CLOCK
#define YX_DB_CLOCK_MAX_SUM                     10
#define YX_DB_CLOCK_MAX_CONTENT_SIZE             60

#define YX_DB_CLOCK_FS_FILE "yxclock_DBFile.db"

typedef struct _YX_DB_CLOCK_Type
{
    uint8 db_id;
    uint8 clock_onoff;
    uint8 clock_week;
    uint8  clock_content[YX_DB_CLOCK_MAX_CONTENT_SIZE+1];
    uint32 clock_id;
    uint32 clock_time;
    uint32 clock_type;
}YX_DB_CLOCK_Type;

extern YX_DB_CLOCK_Type yx_DB_Clock_Reclist[YX_DB_CLOCK_MAX_SUM];

extern BOOLEAN YX_DB_CLOCK_ListInit(void);
extern BOOLEAN YX_DB_CLOCK_ListModify(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len);
extern BOOLEAN YX_DB_CLOCK_ListModifyBuf(uint8 clock_idx,uint32 clock_id,uint8 clock_onoff,uint32 clock_type,uint8 clock_week,uint32 clock_time,uint8 * pConten, uint16 content_len);
extern BOOLEAN YX_DB_CLOCK_ListDel(uint8  clock_idx);
extern BOOLEAN YX_DB_CLOCK_ListDelBuf(uint8  clock_idx);
extern BOOLEAN YX_DB_CLOCK_ListDelAll(void);
extern BOOLEAN YX_DB_CLOCK_ListDelAllBuf(void);
extern BOOLEAN YX_DB_CLOCK_ListSaveBuf(void);

#endif

#ifdef YX_DB_NEED_TIMER
#define YX_DB_TIME_SLIENT_MAX_SUM  5
#define YX_DB_TIME_CLASS_MAX_SUM  20
#define YX_DB_TIME_SLEEP_MAX_SUM  10

typedef  enum
{
    YX_TIME_TYPE_START,
    YX_TIME_TYPE_SLIENT_1,
    YX_TIME_TYPE_SLIENT_MAX=(YX_TIME_TYPE_SLIENT_1+YX_DB_TIME_SLIENT_MAX_SUM),
    YX_TIME_TYPE_CLASS_1,
    YX_TIME_TYPE_CLASS_MAX=(YX_TIME_TYPE_CLASS_1+YX_DB_TIME_CLASS_MAX_SUM),
    YX_TIME_TYPE_SLEEP_1,
    YX_TIME_TYPE_SLEEP_MAX=(YX_TIME_TYPE_SLEEP_1+YX_DB_TIME_SLEEP_MAX_SUM),
    YX_TIME_TYPE_MAX
}YX_TIMER_TYPE_E;

#define YX_DB_TIMER_MAX_SUM                     YX_TIME_TYPE_MAX

#define YX_DB_TIMER_FS_FILE "yxtimer_DBFile.db"

typedef struct _YX_DB_TIMER_Type
{
    uint8 db_id;
    uint8 time_set;
    uint32 time_start;
    uint32 time_end;
}YX_DB_TIMER_Type;

extern YX_DB_TIMER_Type yx_DB_Timer_Reclist[YX_DB_TIMER_MAX_SUM];

extern BOOLEAN YX_DB_TIMER_ListInit(void);
extern BOOLEAN YX_DB_TIMER_ListModify(uint8 timer_idx,uint8 time_set,uint32 time_start,uint32 time_end);
extern BOOLEAN YX_DB_TIMER_ListDel(uint8  timer_idx);
extern BOOLEAN YX_DB_TIMER_ListDelAll(void);
extern BOOLEAN YX_DB_TIMER_ListSave(void);
BOOLEAN YX_DB_TIMER_IsValid(uint16 idx);
BOOLEAN YX_DB_TIMER_IsValid_Slient(void);
BOOLEAN YX_DB_TIMER_IsValid_Class(void);
BOOLEAN YX_DB_TIMER_IsValid_Sleep(void);
#endif


#ifdef YX_DB_NEED_SPD
#define YX_DB_SPD_MAX_SUM                     9
#define YX_DB_SPD_MAX_NUMBER_SIZE             20
#define YX_DB_SPD_MAX_NAME_SIZE             40

#define YX_DB_SPD_FS_FILE "yxspd_DBFile.db"

typedef struct _YX_DB_SPD_Type
{
    uint8 db_id;
    uint8  spd_name[YX_DB_SPD_MAX_NAME_SIZE+1];
    uint8  spd_num[YX_DB_SPD_MAX_NUMBER_SIZE+1];
}YX_DB_SPD_Type;

extern YX_DB_SPD_Type yx_DB_Spd_Reclist[YX_DB_SPD_MAX_SUM];

extern BOOLEAN YX_DB_SPD_ListInit(void);
extern BOOLEAN YX_DB_SPD_ListModify(uint8 spd_idx,YX_DB_SPD_Type * pData);
extern BOOLEAN YX_DB_SPD_ListDel(uint8  spd_idx);
extern BOOLEAN YX_DB_SPD_ListDelAll(void);
extern BOOLEAN YX_DB_SPD_ListModifyBuf(uint8 spd_idx,YX_DB_SPD_Type * pData);
extern BOOLEAN YX_DB_SPD_ListDelBuf(uint8  spd_idx);
extern BOOLEAN YX_DB_SPD_ListDelAllBuf(void);
extern BOOLEAN YX_DB_SPD_ListSaveBuf(void);
extern int YX_DB_SPD_IsValidNumber(char * num);
#endif

#ifdef YX_DB_NEED_NETSMS
#define YX_DB_NETSMS_MAX_SUM                     20
#define YX_DB_NETSMS_MAX_NAME_SIZE             1530
#define YX_DB_NETSMS_MAX_UUID_SIZE             100

#define YX_DB_NETSMS_FS_FILE "yxnetsms_DBFile.db"

typedef struct _YX_DB_NETSMS_Type
{
    uint8 db_id;
    uint8 is_read;
    uint8 msg_id[YX_DB_NETSMS_MAX_UUID_SIZE+1];
    uint8 netsms_type; // 短信类型:0 紧急消息 1通知 2作业3考勤4消费5普通消息6短信息
    uint8 netsms_mode; //播报还是屏显:0语音播报1屏幕显示2语音播报并 屏幕显示
    uint8 netsms_playtimes;  //播报报次数 播报时默认1次，不播报则0，上限三 次
    uint8 netsms_isflash;  //是否闪烁屏幕或LED
    uint8 netsms_isvib;  //是否震动
    uint8 netsms_isring;  //是否响铃
    uint8  netsms_encoding; // 0: assic 1;ucs 2:uft8
    uint32 netsms_rcv_time;
    uint32 netsms_content_len;
    uint8  netsms_content[YX_DB_NETSMS_MAX_NAME_SIZE+1];
}YX_DB_NETSMS_Type;

extern YX_DB_NETSMS_Type yx_DB_NetSms_Reclist[YX_DB_NETSMS_MAX_SUM];
extern uint8 yx_DB_NetSms_list_index[YX_DB_NETSMS_MAX_SUM];

extern BOOLEAN YX_DB_NETSMS_ListInit(void);
extern BOOLEAN YX_DB_NETSMS_ListModify(uint8 netsms_idx,YX_DB_NETSMS_Type * netsms_ptr);
extern BOOLEAN YX_DB_NETSMS_ListDel(uint8  netsms_idx);
extern BOOLEAN YX_DB_NETSMS_ListDelAll(void);
extern BOOLEAN YX_DB_NETSMS_ListModifyBuf(uint8 netsms_idx,YX_DB_NETSMS_Type * netsms_ptr);
extern BOOLEAN YX_DB_NETSMS_ListDelBuf(uint8  netsms_idx);
extern BOOLEAN YX_DB_NETSMS_ListDelAllBuf(void);
extern BOOLEAN YX_DB_NETSMS_ListSaveBuf(void);
extern uint16 YX_DB_NetSms_Add( YX_DB_NETSMS_Type * netsms_ptr);
extern BOOLEAN YX_DB_NETSMS_GetOldNum(uint16 * pIdx);
extern BOOLEAN YX_DB_NETSMS_GetNewNum(uint16 * pIdx);
extern BOOLEAN YX_DB_NETSMS_GetNewUnReadNum(uint16 * pIdx);
extern BOOLEAN YX_DB_NETSMS_ChangStatusToRead(uint8 netsms_idx);
extern BOOLEAN YX_DB_Have_NETSMS_NoRead(void);
extern BOOLEAN YX_DB_NETSMS_ReOrderIndex(void);
#endif

#ifdef YX_DB_NEED_REGIONAL
#define YX_DB_REGIONAL_MAX_SUM                     10
#define YX_DB_REGIONAL_FS_FILE "yx_regional_DBFile.db"

#define MAX_YX_REGIONAL_GPS_NUM 8

typedef struct _REGIONAL_GPS_INFO_t
{
    double Latitude;
    double Longitude;
} REGIONAL_GPS_INFO_T;

typedef struct _YX_DB_REGIONAL_Type
{
    uint8 is_in;
    uint8 db_id;
    uint8 opt_type; // 1 表示新增区域 2 表示修改区域 3 表示删除区域(报文体仅上传操作代码和区域代码，删除时报文格式不变，其他参数补 0)
    uint8 request_status;    //请求状态：1 表示父亲卡 2 表示母亲卡
    uint8 time_week;
    uint8 zone_shape; // 形状1.Round（注：代表圆）2.Rectangle(注 ： 代 表 矩 形)  3.Polygon（注：代表多边形,多边形最多 8 个点）
    uint16 zone_id;  //区域编号
    uint16 gps_sum;
    uint16 gps_radius;
    uint32 time1_start;
    uint32 time1_end;
    uint32 time2_start;
    uint32 time2_end;
    REGIONAL_GPS_INFO_T regional_gps[MAX_YX_REGIONAL_GPS_NUM];
}YX_DB_REGIONAL_Type;

extern YX_DB_REGIONAL_Type yx_DB_Regional_Reclist[YX_DB_REGIONAL_MAX_SUM];

extern BOOLEAN YX_DB_REGIONAL_ListInit(void);
extern BOOLEAN YX_DB_REGIONAL_ListModify(uint8 regional_idx,YX_DB_REGIONAL_Type * cur_data);
extern BOOLEAN YX_DB_REGIONAL_ListDel(uint8  regional_idx);
extern BOOLEAN YX_DB_REGIONAL_ListDelAll(void);
extern BOOLEAN YX_DB_REGIONAL_ListSave(void);
BOOLEAN YX_DB_REGIONAL_IsTimerValid(uint16 idx);
BOOLEAN YX_DB_REGIONAL_IsValid_Slient(void);
BOOLEAN YX_DB_REGIONAL_IsValid_Class(void);
BOOLEAN YX_DB_REGIONAL_IsValid_Sleep(void);
#endif

BOOLEAN YX_DB_Init(YX_APP_T* pMe);
BOOLEAN YX_DB_DelAll(YX_APP_T* pMe);
BOOLEAN YX_DB_Free(YX_APP_T* pMe);

#ifdef ZDT_BH_W9_FEATRUE
typedef struct _YX_PB_LIST_Type
{
    uint8 db_id;
    uint8  pblist_name[YX_DB_SPD_MAX_NAME_SIZE+1];
    uint8  pblist_num[YX_DB_SPD_MAX_NUMBER_SIZE+1];
}YX_PB_LIST_Type;
extern YX_PB_LIST_Type yx_PbList_Reclist[15];
#endif

#endif /*_ZDT_YX_DB_H_*/
