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

typedef struct _YX_JOB_LINK_DATA_t
{
    uint8 answer_type; //答题模式: 0=单次答题，1=多次答题
    uint8 job_type; //题目类型: 31=判断题 32=单选题 33=多选题
} YX_JOB_LINK_DATA_T;

typedef struct _YX_JOB_LINK_NODE_t
{
       YX_JOB_LINK_DATA_T  data;
       struct _YX_JOB_LINK_NODE_t * next_ptr;
       
} YX_JOB_LINK_NODE_T;


#define HTTP_HEALTH_MSG_MAX_LEN      64
#define HTTP_HEALTH_QRCODE_MAX_LEN      512
#define HTTP_HEALTH_VERCODE_MSG_MAX_NUM      5
#define HTTP_HEALTH_NAME_MAX_LEN      64
#define HTTP_HEALTH_URL_PIC_MAX_LEN      200

#define TRAVEL_CITY_MAX_NUM      20

typedef struct _YX_HEALTH_MSG_Type
{
    uint8 personName[HTTP_HEALTH_NAME_MAX_LEN+1]; //姓名
    uint8 code_result;  // 1代表解析成功，0代表解析失败。
    uint16 code_status;  //健康码状态 1绿码  2 黄码 3红码 4 未申领 5 已过期 6 资料不全 7 灰码
    					 //97 不支持 98 未授权99 暂未开通  其他值 未知状态                                  
    uint8 code_addr[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //健康码省份来源
    uint8 code_base4[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //二维码base4
    uint8 code_content[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //二维码内容
    uint8  code_time[HTTP_HEALTH_MSG_MAX_LEN+1]; //亮码精确时间
    uint16 travel_status;   //行程卡状态 1.正常 2去过中高风险区 3无记录 96受政策影响,暂停使用
    					    //97暂不支持 98未授权 99暂未开通
    uint32 travel_city_content_len[TRAVEL_CITY_MAX_NUM];
    uint8  travel_city_content[TRAVEL_CITY_MAX_NUM][HTTP_HEALTH_MSG_MAX_LEN+1]; 
    uint16 nucleic_status;  //核算状态0 未检测 1已检测 96 受政策影响 停用 
    					    //97 不支持 98未授权 99 暂未开通
    uint8 nucleic_feminine;   // 1 阴性  2 阳性0:其它未查询到
    uint32 nucleic_report_len;
    uint16 nucleicHour;
    uint8  nucleicOrgan[HTTP_HEALTH_MSG_MAX_LEN+1]; //检测机构
    uint8  nucleic_report[HTTP_HEALTH_MSG_MAX_LEN+1]; //检测结果
    uint8  detect_time[HTTP_HEALTH_MSG_MAX_LEN+1]; //检测时间
    uint16  effective_time;//有效时间
    
    uint16 vacc_num; // 疫苗次数
    uint16 vacc_status; //0:未查询到接种信息,1:不适宜接种,2:未完成基础免疫,3:完成基础免疫,4:完成加强免疫
    uint8  vacc_conut[2+1]; //接种次数
    uint8 vacc_last_time[HTTP_HEALTH_MSG_MAX_LEN+1];//最近的接种时间
    int16 vacc_newest_time[15];//最近的接种时间
    
    uint8 url_health[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//健康码
    uint8 url_nucleic[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//核酸码
    uint8 url_acc[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//行程码
    uint8 url_type1_health[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//健康码
    uint8 url_type1_nucleic[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//核酸码
    uint8 url_type1_acc[HTTP_HEALTH_URL_PIC_MAX_LEN+1];//行程码
    uint32  rcv_time;//下载时间
    
    uint8 type2_status;
    uint8 type2_type; // 1表示展示base64图片；2表示展示二维码字符串和并且显示颜色；3表示展示二维码字符串并且颜色为空；4表示未上传或者上传图片已过期。
    uint8 type2_qrcode[HTTP_HEALTH_QRCODE_MAX_LEN+1];
    uint8 type2_username[HTTP_HEALTH_NAME_MAX_LEN+1];
    uint8 type2_color; // 1 GREEN，2 YELLOW，3 RED
    uint32 type2_validityDate;
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
extern uint32 YX_Net_Send_SYSTEM_SMS(YX_APP_T *pMe, uint8* pNum,uint8 * utf8_ptr,uint16 utf8_len);
extern uint32 YX_Net_Send_REPORT_TEMPER(YX_APP_T *pMe, uint16 alert_type);
extern uint32 YX_Net_Send_REPORT_SMS_READ(YX_APP_T *pMe, uint16 sms_idx);

extern int YX_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm);

extern int YX_API_Net_Reset(void);
extern int YX_API_CZSB_Stop(void);
extern int YX_API_SeNetOpen(void);
extern int YX_API_SeNetClose(void);
extern void YX_CheckClassSilentOnOff();
extern BOOLEAN  MMIRFID_HTTP_Send(void);
extern BOOLEAN  MMIRFID_CheckUpdate(void);

#endif
