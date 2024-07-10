#ifndef __ZDT_HYHL_NET_H__
#define __ZDT_HYHL_NET_H__

#include "zdt_common.h"

#define HYHL_NET_MSG_START_1 0x55
#define HYHL_NET_MSG_START_2 0x55

#define HYHL_NET_MSG_END_1 0x0D
#define HYHL_NET_MSG_END_2 0x0A

#define HYHL_NET_SOS_MAX_NAME_SIZE 40
#define HYHL_NET_SOS_MAX_NUM_SIZE 12
#define HYHL_NET_WHITE_MAX_NAME_SIZE 40
#define HYHL_NET_WHITE_MAX_NUM_SIZE 12
#define HYHL_NET_LINKMAN_MAX_NAME_SIZE 40
#define HYHL_NET_LINKMAN_MAX_NUM_SIZE 12
#define HYHL_NET_CLOCK_MAX_NAME_SIZE 40

#ifdef ZDT_PLAT_HYHL_SUPPORT_AGPS
typedef struct _HYHL_AGPS_FILE_DATA_t
{
    uint8 * agps_data;
    uint16 agps_data_len;
    uint8 pack_idx;
    uint8 pack_sum;
}HYHL_AGPS_FILE_DATA_T;

typedef struct _HYHL_AGPS_FILE_NODE_t
{
       HYHL_AGPS_FILE_DATA_T  data;
       struct _HYHL_AGPS_FILE_NODE_t * next_ptr;
}HYHL_AGPS_FILE_NODE_T;
#endif

extern uint16 g_hyhl_net_msg_index;
extern void HYHL_Net_Rereg_TimeStop(HYHL_APP_T* pMe);
extern void HYHL_Net_CloseAllTimer(HYHL_APP_T *pMe);
extern BOOLEAN HYHL_Net_IsAllow(HYHL_APP_T* pMe);
extern BOOLEAN HYHL_Net_IsOpen(HYHL_APP_T* pMe);
extern int HYHL_Net_Open(HYHL_APP_T* pMe);
extern int HYHL_Net_Close(HYHL_APP_T* pMe);
extern int HYHL_API_Net_Reset(void);
extern int HYHL_Net_Init(void);
extern int HYHL_Net_Reset(HYHL_APP_T* pMe);
extern BOOLEAN HYHL_Net_IsInit(void);

extern int HYHL_Net_GetContentLen(uint8 * pData,uint32 Data_len);
extern int HYHL_Net_GetProtocol(uint8 * pData,uint32 Data_len);

extern uint32 HYHL_Net_TCPSendNow(ZDT_TCP_INTERFACE_T *pMe,uint8 protocol_id,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle);
extern uint32 HYHL_Net_TCPSend(ZDT_TCP_INTERFACE_T *pMe,uint8 protocol_id,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,TCPRCVHANDLER rcv_handle);

extern uint32 HYHL_Net_1A_Send(HYHL_APP_T *pMe,uint8 repeat_num,uint32 timeout);
extern uint32 HYHL_Net_A0_Send(HYHL_APP_T *pMe,uint8 repeat_num,uint32 timeout);
extern uint32 HYHL_Net_A3_Send(HYHL_APP_T* pMe,uint8 loc_ok,uint16 type);
extern uint32 HYHL_Net_A4_Send(HYHL_APP_T* pMe);
extern uint32 HYHL_Net_A6_Send(HYHL_APP_T *pMe,uint8 * number);
extern uint32 HYHL_Net_A8_Send(HYHL_APP_T* pMe,uint8 * number,uint8 loc_ok);
extern uint32 HYHL_Net_A9_Respond(HYHL_APP_T *pMe,uint8 * ide,uint8 loc_ok);
extern uint32 HYHL_Net_AA_Respond(HYHL_APP_T *pMe,uint8 pack_num,uint8 pack_idx,uint16 voc_idx);
extern uint32 HYHL_Net_AC_Respond(HYHL_APP_T *pMe,uint8 *mark,char * rsp_cont,uint16 cont_len);

#endif
