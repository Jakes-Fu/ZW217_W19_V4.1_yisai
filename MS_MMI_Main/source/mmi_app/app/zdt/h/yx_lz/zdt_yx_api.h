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
  YX_LOCTION_TYPE_WL,
  YX_LOCTION_TYPE_SOS,
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

#if 1 //ëÁµã²¹´«
#define YX_MDBC_MAX_SUM 100
typedef enum {
  YX_MDBC_TYPE_NULL,
  YX_MDBC_TYPE_LOCTION,
  YX_MDBC_TYPE_ALERT,
  YX_MDBC_TYPE_MAX
} YX_MDBC_TYPE_E;

typedef struct _YX_MDBC_LINK_DATA_t
{
    uint16 type;
    uint8 * pParam;
    uint16 len;
} YX_MDBC_LINK_DATA_T;

typedef struct _YX_MDBC_LINK_NODE_t
{
       YX_MDBC_LINK_DATA_T  data;
       struct _YX_MDBC_LINK_NODE_t * next_ptr;
       
} YX_MDBC_LINK_NODE_T;

extern BOOLEAN YX_MDBC_Link_AddData(YX_APP_T *pMe,uint16 type,uint8 * pParam,uint16 ParamLen);
extern BOOLEAN  YX_MDBC_Link_Get(YX_APP_T *pMe,YX_MDBC_LINK_DATA_T * pData);
extern BOOLEAN  YX_MDBC_Link_TypeExsit(YX_APP_T *pMe,uint16 type);
extern BOOLEAN  YX_MDBC_Link_GetByType(YX_APP_T *pMe,YX_MDBC_LINK_DATA_T * pData,uint16 type);
extern BOOLEAN  YX_MDBC_Link_DelAll(YX_APP_T *pMe);
extern BOOLEAN  YX_MDBC_Send(YX_APP_T *pMe);
extern uint16  YX_MDBC_GetLength(void);
#endif

extern int YX_API_Lose_Post(void);
extern int YX_API_BatStatus_Post(uint8 level,uint8 percent);
extern int YX_API_Move_Post(void);
extern int YX_API_Still_Post(void);
extern int YX_API_JP_Change_Post(void);
extern int YX_API_LBS_Change_Post(void);
extern int YX_API_FallDown_Post(void);
#endif
