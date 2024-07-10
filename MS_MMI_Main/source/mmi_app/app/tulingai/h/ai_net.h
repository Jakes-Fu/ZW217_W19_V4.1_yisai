#ifndef __AI_AI_NET_H__
#define __AI_AI_NET_H__

#include "ai_common.h"
#include "ai_api.h"
#define AI_NET_MSG_START_1 0x55
#define AI_NET_MSG_START_2 0x55

#define AI_NET_MSG_END_1 0x0D
#define AI_NET_MSG_END_2 0x0A



extern uint16 g_ai_net_msg_index;

extern BOOLEAN AI_Net_IsAllow(AI_APP_T* pMe);
extern BOOLEAN AI_Net_IsOpen(AI_APP_T* pMe);
extern int AI_Net_Open(AI_APP_T* pMe);
extern int AI_Net_Close(AI_APP_T* pMe);
extern int AI_API_Net_Reset(void);
int AI_Net_Init(void);
extern int AI_Net_Reset(AI_APP_T* pMe);
extern BOOLEAN AI_Net_IsInit(void);

extern uint32 AI_Net_TCPSend(uint8* fileData,char *host,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,AITCPRCVHANDLER rcv_handle);


extern void  AI_TCPSendAIRecordFile(void);

extern void AI_InitNextUrlRequest();



//================
/*


#ifdef AI_UDP_SUPPORT
#include "zdt_udp.h"
#endif
#ifdef AI_TCP_SUPPORT
#include "zdt_tcp.h"
#endif


#define  AI_N_STATION_NUM		5
#define  AI_IMEI_LEN 15
#define  AI_IMSI_LEN 15
#define  AI_ICCID_LEN 20

typedef struct MMIAI_S_STATION_DATA_t
{
    uint8                                      is_valid;
    uint8                                      rssiLevel;
    uint16						mcc;
    uint16						mnc;
    uint16						mnc_digit_num;
    uint16						lac;    //位置区码
    uint32						cell_id; //基站ID
    MN_DUAL_SYS_E                    dual_sys;
} MMIAI_S_STATION_DATA_T;

typedef struct MMIAI_N_STATION_DATA_t
{
    MN_DUAL_SYS_E                    dual_sys;
    uint8                                      is_valid;
    uint8                                      rssiLevel;
    uint32						cell_id; //基站ID
} MMIAI_N_STATION_DATA_T;

typedef struct MMIAI_STATION_DATA_t
{
    MMIAI_S_STATION_DATA_T   s_cell;
    MMIAI_N_STATION_DATA_T   n_cell[AI_N_STATION_NUM];
} MMIAI_STATION_DATA_T;

#ifdef MEID_WRITE_SUPPORT
extern char         g_ai_phone_meid[AI_IMEI_LEN+1];
#endif
extern char         g_ai_phone_imei[AI_IMEI_LEN+1];
extern char         g_ai_phone_imei_2[AI_IMEI_LEN+1];
extern char         g_ai_sim_imsi[AI_IMSI_LEN+1];
extern char         g_ai_sim_iccid[AI_ICCID_LEN+1];
extern uint32         g_ai_net_cell_id;

extern MMIAI_STATION_DATA_T g_ai_station_data;
extern MMIAI_S_STATION_DATA_T g_ai_s_station_data;

PUBLIC uint32 MMIAI_Net_GetNetID(void);
PUBLIC BOOLEAN MMIAI_Net_PdpStateGet(void);
PUBLIC BOOLEAN MMIAI_Net_IsActiving(void);
PUBLIC BOOLEAN MMIAI_Net_IsClosing(void);
PUBLIC BOOLEAN MMIAI_Net_IsInit(void);
PUBLIC BOOLEAN MMIAI_Net_Init(void);
PUBLIC BOOLEAN MMIAI_Net_Reset(void);
PUBLIC BOOLEAN MMIAI_Net_Open(void);
PUBLIC BOOLEAN MMIAI_Net_Close(void);

PUBLIC MN_DUAL_SYS_E MMIAI_Net_GetActiveSys(void);
PUBLIC void MMIAI_Net_SetActiveSys(MN_DUAL_SYS_E set_sys);

PUBLIC MMI_RESULT_E MMIAI_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC BOOLEAN MMIAI_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr);
PUBLIC void MMIAI_Net_RssiInd(MN_DUAL_SYS_E dual_sys,uint8 rssi);
PUBLIC void MMIAI_Station_Rssi_Update(uint8 rssi);
PUBLIC BOOLEAN MMIAI_Net_APP_Init(void);
PUBLIC BOOLEAN MMIAI_Net_GPRS_IsOK(MN_DUAL_SYS_E dual_sys);

*/

//========================
#define  AI_N_STATION_NUM		5
#define  AI_IMEI_LEN 15
#define  AI_IMSI_LEN 15
#define  AI_ICCID_LEN 20



#endif
