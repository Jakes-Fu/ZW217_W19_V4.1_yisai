#ifndef __ZDT_SHB_SMS_H__
#define __ZDT_SHB_SMS_H__

#include "zdt_common.h"
#include "zdt_shb_api.h"

typedef struct _SHB_SMS_SOS_DATA_t
{
    uint8          * pNum;
    uint8          * pData;
    uint16         DataLen;
} SHB_SMS_SOS_DATA_T;

typedef enum
{
    SHB_SMS_TYPE_NULL = 0, 
    SHB_SMS_TYPE_DW,  //定位
    SHB_SMS_TYPE_JT,    //监听
    SHB_SMS_TYPE_KQGPS,// 开启GPS
    SHB_SMS_TYPE_GBGPS,//关闭GPS
    SHB_SMS_TYPE_KQWIFI,//开启WIFI
    SHB_SMS_TYPE_GBWIFI,//关闭WIFI
    SHB_SMS_TYPE_MAX
} SHB_SMS_SOS_TYPE_E;

extern BOOLEAN SHB_SMS_IsValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2);

extern int SHB_API_SMS_SendLowBattery(void);
extern int SHB_API_SMS_SendLose(void);
extern int SHB_API_SMS_SendSOS(uint8* pStr, uint32 Len);

#endif
