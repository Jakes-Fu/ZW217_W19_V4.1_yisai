#ifndef __ZDT_YX_API_H__
#define __ZDT_YX_API_H__

#include "zdt_common.h"
#include "zdt_tcp.h"

typedef struct YX_APP_T         YX_APP_T;

#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
typedef enum {
  YX_LOCTION_TYPE_OFF,
  YX_LOCTION_TYPE_CR,
  YX_LOCTION_TYPE_NET,
  YX_LOCTION_TYPE_SMS,
  YX_LOCTION_TYPE_ALERT,
  YX_LOCTION_TYPE_FRIEND,
  YX_LOCTION_TYPE_MAX
} YX_LOCTION_TYPE_E;

typedef struct _YX_LOCTION_LINK_DATA_t
{
    uint16 type;
    uint8 * pParam;
    uint16 len;
} YX_LOCTION_LINK_DATA_T;

typedef struct _YX_LOCTION_LINK_NODE_t
{
       YX_LOCTION_LINK_DATA_T  data;
       struct _YX_LOCTION_LINK_NODE_t * next_ptr;
       
} YX_LOCTION_LINK_NODE_T;

extern BOOLEAN  YX_LOCTION_Link_DelAll(YX_APP_T *pMe);
extern BOOLEAN YX_LOCTION_Start(YX_APP_T *pMe,uint16 type,uint8 * pParam,uint16 ParamLen);
extern int YX_LOCTION_EndHandle(YX_APP_T *pMe,DPARAM param);
extern int YX_API_LOCTION_End(uint16 loc_ok,uint16 type);
#endif

extern int YX_API_Lose_Post(void);
extern int YX_API_BatStatus_Post(uint8 level,uint8 percent);
extern int YX_API_Move_Post(void);
extern int YX_API_Still_Post(void);
extern int YX_API_JP_Change_Post(void);
extern int YX_API_LBS_Change_Post(void);
extern int YX_API_FallDown_Post(void);
#endif
