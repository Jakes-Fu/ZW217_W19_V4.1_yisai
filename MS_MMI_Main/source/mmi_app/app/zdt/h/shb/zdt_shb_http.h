#ifndef __ZDT_SHB_HTTP_H__
#define __ZDT_SHB_HTTP_H__

#include "zdt_common.h"
#include "zdt_shb_api.h"
#include "zdthttp_app.h"


#define ZDT_HTTP_SHB_RET_OK 0
#define ZDT_HTTP_SHB_RET_ERR 1

typedef enum
{
    SHB_HTTP_LOC_TYPE_NULL = 0, 
    SHB_HTTP_LOC_TYPE_DW,  //定位
    SHB_HTTP_LOC_TYPE_SOS,    //SOS
    SHB_HTTP_LOC_TYPE_LONG,// 连续定位
    SHB_HTTP_LOC_TYPE_WL,//围栏定位
    SHB_HTTP_LOC_TYPE_SOS_OK,    //SOS 带位置信息
    SHB_HTTP_LOC_TYPE_MAX
} SHB_HTTP_LOC_TYPE_E;

extern void SHB_Net_CloseAllTimer(SHB_APP_T *pMe);
extern BOOLEAN SHB_Net_IsAllow(void);
extern BOOLEAN SHB_Net_IsOpen(SHB_APP_T* pMe);
extern BOOLEAN SHB_Net_IsInit(void);
extern int SHB_Net_Open(SHB_APP_T* pMe);
extern int SHB_Net_Close(SHB_APP_T* pMe);
extern int SHB_Net_Init(void);
extern int SHB_Net_Reset(SHB_APP_T* pMe);

extern int SHB_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm);

extern int SHB_API_Net_Reset(void);
extern int SHB_API_SeNetOpen(void);
extern int SHB_API_SeNetClose(void);
extern char * SHB_Get_CurIMEI(void);


extern BOOLEAN  SHB_HTTP_ZTE2100_Send(HTTPRCVHANDLER rcv_handle);
extern BOOLEAN  SHB_HTTP_ZTE2200_Send(void);
extern BOOLEAN  SHB_HTTP_ZTE2200_Start(void);
extern BOOLEAN  SHB_HTTP_ZTE1000_Send(void);
extern BOOLEAN  SHB_HTTP_ZTE1001_Send(uint8 type);
extern BOOLEAN  SHB_HTTP_ZTE1003_Send(uint8 is_loc_ok);
extern BOOLEAN  SHB_HTTP_ZTE1006_Send(uint8 is_loc_ok);
extern BOOLEAN  SHB_HTTP_ZTE1008_Send(uint8 is_loc_ok);
extern BOOLEAN  SHB_HTTP_ZTE1010_Send(uint8 is_loc_ok);
extern BOOLEAN  SHB_HTTP_ZTE1211_Send(char* num, uint8 result);
extern BOOLEAN  SHB_HTTP_SOS_Send(void);
extern BOOLEAN  SHB_HTTP_RegCheck(void);

#endif
