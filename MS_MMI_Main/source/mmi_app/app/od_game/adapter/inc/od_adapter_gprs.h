#ifndef __OD_ADAPTER_GPRS_H__
#define __OD_ADAPTER_GPRS_H__
#include "od_type_def.h"
#define OD_APN_CONFIG_MAX (2)

typedef struct
{
    OD_U8              plmn[8];
    OD_U8              name[12];
    OD_U8              auth_type;       //鉴定类型0:PAP，1:CHAP:
    OD_U8              apn[30];
    OD_U8              username[16];
    OD_U8              password[12];
} OD_ACCOUNT_DATA_STRUCT;

typedef struct
{
    OD_U32             plmn;
    OD_U8              *name;
    OD_U8              auth_type;       //鉴定类型0:PAP，1:CHAP:
    OD_U8              *apn;
    OD_U8              *username;
    OD_U8              *password;
} Od_AcountData_t;

OD_BOOL Od_IsGprsEnable(void);
#endif
