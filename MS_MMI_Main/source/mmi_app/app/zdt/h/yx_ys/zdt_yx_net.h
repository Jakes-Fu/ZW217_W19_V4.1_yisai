#ifndef __ZDT_YX_NET_H__
#define __ZDT_YX_NET_H__

#include "zdt_common.h"
#include "zdt_yx_api.h"

#if defined(ZDT_PLAT_YX_SUPPORT_FZD)
//防走丢后台
#define FEATURE_YX_SEND_MD5 1

#if 1
//TL  K20
#define YX_NET_MD5_KEY "tJcZar4toZrCOfT9VDguc6h23Q4BGcaa"   //K20
//[
#define YX_NET_MSG_START 0x5B
// TL
#define YX_NET_MSG_SEND_CS_1 0x54
#define YX_NET_MSG_SEND_CS_2 0x4C
#else
#if 0
//BK Q3L
#define YX_NET_MD5_KEY "fJcZar4toZrCOfT9VDguc6h23Q4Bznbk"   //Q3L 
//[
#define YX_NET_MSG_START 0x5B
// BK
#define YX_NET_MSG_SEND_CS_1 0x42
#define YX_NET_MSG_SEND_CS_2 0x4B

#else
//RL  RY570
#define YX_NET_MD5_KEY "fJcZar4toZrCOfT9VDguc6h23Q4Bznbk"   // RY570
//[
#define YX_NET_MSG_START 0x5B
// RL
#define YX_NET_MSG_SEND_CS_1 0x52
#define YX_NET_MSG_SEND_CS_2 0x4C
#endif
#endif


#elif defined( ZDT_PLAT_YX_SUPPORT_YS)
//易赛后台 安康守护
#define FEATURE_YX_SEND_MD5 1
#define YX_NET_MD5_KEY "g3oCnND6NzhfA6dmNfSnBWZuKyLdeYLd"
//[
#define YX_NET_MSG_START 0x5B
// AK
#define YX_NET_MSG_SEND_CS_1 0x59
#define YX_NET_MSG_SEND_CS_2 0x53

#elif defined( ZDT_PLAT_YX_SUPPORT_TX)
//童信后台百合
#define FEATURE_YX_SEND_MD5 0
#define YX_NET_MD5_KEY "vZKhHwPMWffbur11V3Y9NwhM1DjfbG2P"
//[
#define YX_NET_MSG_START 0x5B
// XT
#define YX_NET_MSG_SEND_CS_1 0x58
#define YX_NET_MSG_SEND_CS_2 0x54
#else
//防走丢后台
#define FEATURE_YX_SEND_MD5 1
#define YX_NET_MD5_KEY "tJcZar4toZrCOfT9VDguc6h23Q4BGcaa"
//[
#define YX_NET_MSG_START 0x5B
// TL
#define YX_NET_MSG_SEND_CS_1 0x54
#define YX_NET_MSG_SEND_CS_2 0x4C

#endif

//SG
#define YX_NET_MSG_RCV_CS_1 0x4E
#define YX_NET_MSG_RCV_CS_2 0x4E

//]
#define YX_NET_MSG_END 0x5D
#define YX_NET_MSG_END_2 0x0D

typedef enum {
  YX_NET_ALERT_TYPE_NULL,
  YX_NET_ALERT_TYPE_SOS,
  YX_NET_ALERT_TYPE_LOWBAT,
  YX_NET_ALERT_TYPE_OUT,
  YX_NET_ALERT_TYPE_IN,
  YX_NET_ALERT_TYPE_LOSE,
  YX_NET_ALERT_TYPE_FALLDOWN,
  YX_NET_ALERT_TYPE_MAX
} YX_NET_ALERT_TYPE_E;


typedef struct _YX_WEATHER_INFO_t
{
    uint16 city_name[101]; //地址
    uint16 content[101];    //天气描述多云
    uint16 date_str[21];    //日期2020-04-08
    uint16 time_str[21];    //时间11:52:32
    uint16 num; //天气编号1——晴  2——阴   3——雨  4——雪
    int32 cur_degree;       //当前温度
    int32 min_degree;      //最低温
    int32 max_degree;   ////最高温
} YX_WEATHER_INFO_T;

extern void YX_Net_CloseAllTimer(YX_APP_T *pMe);
extern BOOLEAN YX_Net_IsAllow(YX_APP_T* pMe);
extern BOOLEAN YX_Net_IsOpen(YX_APP_T* pMe);
extern int YX_Net_Open(YX_APP_T* pMe);
extern int YX_Net_Close(YX_APP_T* pMe);
extern int YX_Net_Rereg(YX_APP_T* pMe);
extern int YX_Net_Init(void);
extern BOOLEAN YX_Net_IsInit(void);
extern int YX_Net_Reset(YX_APP_T* pMe);

extern int YX_Net_GetContentLen(uint8 * pData,uint32 Data_len);
#if FEATURE_YX_SEND_MD5
extern void YX_Net_MD5(uint8 * pDate, uint16 data_len, char out_str[32]);
#endif
extern uint32 YX_Net_TCPSend(char * meid,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle);
extern uint32 YX_Net_TCPRespond(char * meid,char * pContent,uint16 ContentLen);
extern uint32 YX_Net_Send_DLT(YX_APP_T *pMe,uint8 repeat_num,uint32 timeout);
extern uint32 YX_Net_Send_LK(YX_APP_T *pMe,uint8 repeat_num,uint32 timeout);
extern uint32 YX_Net_Send_TIME(YX_APP_T *pMe);
extern uint32 YX_Net_Send_LGZONE(YX_APP_T *pMe);
extern uint32 YX_Net_Send_UD(YX_APP_T* pMe,uint8 loc_ok);
extern uint32 YX_Net_Send_UD2(YX_APP_T* pMe,uint8 loc_ok);
extern uint32 YX_Net_Send_AL(YX_APP_T* pMe,uint16 alert_type,uint8 loc_ok);
extern uint32 YX_Net_Send_WT(YX_APP_T* pMe, uint8 loc_ok);
extern uint32 YX_Net_Send_TKQ(YX_APP_T* pMe);
extern uint32 YX_Net_Send_TKQ2(YX_APP_T* pMe);
extern uint32 YX_Net_Send_PHLQ(YX_APP_T* pMe);
extern uint32 YX_Net_Send_SOSQ(YX_APP_T* pMe);
extern uint32 YX_Net_Send_REMINDQ(YX_APP_T* pMe);
extern uint32 YX_Net_Send_HEART(YX_APP_T* pMe,uint16 cur_hr);
extern uint32 YX_Net_Send_TEMP(YX_APP_T* pMe,double tw);
extern uint32 YX_Net_Send_BLOOD(YX_APP_T* pMe,uint16 b_h, uint16 b_l);

extern int YX_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm);

extern int YX_API_Net_Reset(void);
extern int YX_API_CZSB_Stop(void);
extern int YX_API_SeNetOpen(void);
extern int YX_API_SeNetClose(void);
extern BOOLEAN MMIYX_UnBindDevice(void);


#endif
