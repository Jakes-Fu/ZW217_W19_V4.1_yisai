#ifndef __ZDT_YX_NET_H__
#define __ZDT_YX_NET_H__

#include "zdt_common.h"
#include "zdt_yx_api.h"

#define YX_NET_SW_VERSION 500
#define YX_NET_MSG_HEAD_LEN 41

//[
#define YX_NET_MSG_START 0x5B

//]
#define YX_NET_MSG_END 0x5D

#define YX_NET_CMD_END 0x0D
#define YX_NET_CMD_END_2 0x0A

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

#define HTTP_HEALTH_MSG_MAX_LEN      64
#define HTTP_HEALTH_QRCODE_MAX_LEN      512
#define HTTP_HEALTH_VERCODE_MSG_MAX_NUM      5

#define TRAVEL_CITY_MAX_NUM      20

typedef struct _YX_HEALTH_MSG_Type
{
    uint16 code_status;  //健康码状态 1绿码  2 黄码 3红码 4 未申领 5 已过期 6 资料不全 7 灰码
    					 //97 不支持 98 未授权99 暂未开通  其他值 未知状态                                  
    uint8 code_addr[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //二维码地址
    uint8 code_base4[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //二维码base4
    uint8 code_content[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //二维码内容
    uint16 travel_status;   //行程卡状态 1.正常 2去过中高风险区 3无记录 96受政策影响,暂停使用
    					    //97暂不支持 98未授权 99暂未开通
    uint32 travel_city_content_len[TRAVEL_CITY_MAX_NUM];
    uint8  travel_city_content[TRAVEL_CITY_MAX_NUM][HTTP_HEALTH_MSG_MAX_LEN+1]; 
    uint16 nucleic_status;  //核算状态0 未检测 1已检测 96 受政策影响 停用 
    					    //97 不支持 98未授权 99 暂未开通
    uint8 nucleic_feminine;   // 1 阴性  0 阳性
    uint32 nucleic_report_len;
    uint8  nucleic_report[HTTP_HEALTH_MSG_MAX_LEN+1]; //检测结果
    uint8  detect_time[HTTP_HEALTH_MSG_MAX_LEN+1]; //检测时间
    uint16  effective_time;//有效时间

    uint16 vacc_status; // 1无记录 2 已获取 96受政策影响 97不支持 98 未授权 99 未开通
    uint8  vacc_conut[2+1]; //接种次数
    int16 vacc_newest_time[15];//最近的接种时间
}YX_HEALTH_MSG_Type;
extern YX_HEALTH_MSG_Type yx_health_msg_Rec;

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
extern uint32 YX_Net_TCPSend(char type,char * mark_str,uint8 * pContent,uint32 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle);
extern uint32 YX_Net_TCPRespond(char type,char * water_str,char * mark_str,char * pContent,uint32 ContentLen);

extern uint32 YX_Net_Send_GET_SERVER_INFO(YX_APP_T *pMe);
extern uint32 YX_Net_Send_DEVICE_LOGIN(YX_APP_T *pMe);
extern uint32 YX_Net_Send_REPORT_HEARTBEAT(YX_APP_T *pMe);
extern uint32 YX_Net_Send_ALARM_POWER(YX_APP_T *pMe, uint16 alert_type);
extern uint32 YX_Net_Send_REPORT_LOCATION_INFO(YX_APP_T *pMe, uint8 loc_ok);
extern uint32 YX_Net_Send_REPORT_SOS(void);
extern uint32 YX_Net_Respond_LOCATION_INFO_GET(YX_APP_T *pMe, uint8 loc_ok,char * water_str);

extern int YX_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm);

extern int YX_API_Net_Reset(void);
extern int YX_API_CZSB_Stop(void);
extern int YX_API_SeNetOpen(void);
extern int YX_API_SeNetClose(void);


#endif
