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
    uint16 city_name[101]; //��ַ
    uint16 content[101];    //������������
    uint16 date_str[21];    //����2020-04-08
    uint16 time_str[21];    //ʱ��11:52:32
    uint16 num; //�������1������  2������   3������  4����ѩ
    int32 cur_degree;       //��ǰ�¶�
    int32 min_degree;      //�����
    int32 max_degree;   ////�����
} YX_WEATHER_INFO_T;

#define HTTP_HEALTH_MSG_MAX_LEN      64
#define HTTP_HEALTH_QRCODE_MAX_LEN      512
#define HTTP_HEALTH_VERCODE_MSG_MAX_NUM      5

#define TRAVEL_CITY_MAX_NUM      20

typedef struct _YX_HEALTH_MSG_Type
{
    uint16 code_status;  //������״̬ 1����  2 ���� 3���� 4 δ���� 5 �ѹ��� 6 ���ϲ�ȫ 7 ����
    					 //97 ��֧�� 98 δ��Ȩ99 ��δ��ͨ  ����ֵ δ֪״̬                                  
    uint8 code_addr[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //��ά���ַ
    uint8 code_base4[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //��ά��base4
    uint8 code_content[HTTP_HEALTH_QRCODE_MAX_LEN+1]; //��ά������
    uint16 travel_status;   //�г̿�״̬ 1.���� 2ȥ���и߷����� 3�޼�¼ 96������Ӱ��,��ͣʹ��
    					    //97�ݲ�֧�� 98δ��Ȩ 99��δ��ͨ
    uint32 travel_city_content_len[TRAVEL_CITY_MAX_NUM];
    uint8  travel_city_content[TRAVEL_CITY_MAX_NUM][HTTP_HEALTH_MSG_MAX_LEN+1]; 
    uint16 nucleic_status;  //����״̬0 δ��� 1�Ѽ�� 96 ������Ӱ�� ͣ�� 
    					    //97 ��֧�� 98δ��Ȩ 99 ��δ��ͨ
    uint8 nucleic_feminine;   // 1 ����  0 ����
    uint32 nucleic_report_len;
    uint8  nucleic_report[HTTP_HEALTH_MSG_MAX_LEN+1]; //�����
    uint8  detect_time[HTTP_HEALTH_MSG_MAX_LEN+1]; //���ʱ��
    uint16  effective_time;//��Чʱ��

    uint16 vacc_status; // 1�޼�¼ 2 �ѻ�ȡ 96������Ӱ�� 97��֧�� 98 δ��Ȩ 99 δ��ͨ
    uint8  vacc_conut[2+1]; //���ִ���
    int16 vacc_newest_time[15];//����Ľ���ʱ��
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
