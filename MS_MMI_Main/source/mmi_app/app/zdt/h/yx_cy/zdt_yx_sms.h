#ifndef __ZDT_YX_SMS_H__
#define __ZDT_YX_SMS_H__

#include "zdt_common.h"
#include "zdt_yx_api.h"

typedef struct _YX_SMS_SOS_DATA_t
{
    uint8          * pNum;
    uint8          * pData;
    uint16         DataLen;
} YX_SMS_SOS_DATA_T;

typedef enum
{
    YX_SMS_TYPE_NULL = 0, 
    YX_SMS_TYPE_DW,  //定位
    YX_SMS_TYPE_JT,    //监听
    YX_SMS_TYPE_KQGPS,// 开启GPS
    YX_SMS_TYPE_GBGPS,//关闭GPS
    YX_SMS_TYPE_KQWIFI,//开启WIFI
    YX_SMS_TYPE_GBWIFI,//关闭WIFI
    YX_SMS_TYPE_MAX
} YX_SMS_SOS_TYPE_E;

extern BOOLEAN YX_SMS_IsValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2);

extern int YX_API_SMS_SendLowBattery(void);
extern int YX_API_SMS_SendLose(void);
extern int YX_API_SMS_SendSOS(void);

#endif
