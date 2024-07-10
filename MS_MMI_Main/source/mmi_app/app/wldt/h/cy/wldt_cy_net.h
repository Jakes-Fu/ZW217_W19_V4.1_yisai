#ifndef __WLDT_CY_NET_H__
#define __WLDT_CY_NET_H__

#include "wldt_common.h"
#include "wldt_cy_api.h"

#define CY_NET_MSG_START 0x01

#define CY_NET_MSG_END 0x0D
#define CY_NET_MSG_END_2 0x0A

typedef enum {
  CY_NET_WLDT_TYPE_NULL,
  CY_NET_WLDT_TYPE_YESNO,
  CY_NET_WLDT_TYPE_SINGLE,
  CY_NET_WLDT_TYPE_MULTI,
  CY_NET_WLDT_TYPE_MAX
} CY_NET_WLDT_TYPE_E;

extern void CY_Net_CloseAllTimer(CY_APP_T *pMe);
extern BOOLEAN CY_Net_IsAllow(CY_APP_T* pMe);
extern BOOLEAN CY_Net_IsOpen(CY_APP_T* pMe);
extern int CY_Net_Open(CY_APP_T* pMe);
extern int CY_Net_Close(CY_APP_T* pMe);
extern int CY_Net_Rereg(CY_APP_T* pMe);
extern int CY_Net_Init(void);
extern BOOLEAN CY_Net_IsInit(void);
extern int CY_Net_Reset(CY_APP_T* pMe);

extern int CY_Net_GetContentLen(uint8 * pData,uint32 Data_len);
extern uint32 CY_Net_TCPSend(uint8 type,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,WLDTTCPRCVHANDLER rcv_handle);
extern uint32 CY_Net_TCPRespond(uint8 type,char * pContent,uint16 ContentLen);
extern uint32 CY_Net_Send_LK(CY_APP_T *pMe,uint8 repeat_num,uint32 timeout);
extern int CY_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm);

extern int CY_API_Net_Reset(void);


#endif
