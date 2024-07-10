
#ifndef _WLDT_CY_DB_H_
#define _WLDT_CY_DB_H_

#include "wldt_common.h"

#define CY_DB_NEED_SETTING

#define CY_DB_DIR "CYDB"

#ifdef CY_DB_NEED_SETTING
#define CY_DB_SET_FS_FILE "cy_setting_DBFile.db"

#define MAX_CY_IP_DOMAIN_SIZE 100
#define MAX_CY_CENTER_NUMBER_SIZE 40
#define MAX_CY_PASSWORD_SIZE 40

typedef struct _CY_DB_SET_Type
{
    uint8 version;
    uint8 db_id;
    uint8 language;
    uint8 alert_sos_sms;
    uint8 alert_low_bat;
    uint8 alert_loser;
    uint8 alert_loser_sms;
    uint8 alert_jp;
    uint8 profile;
    uint8 falldown;
    uint8 zone;
    uint8 net_open;
    uint16 heart;
    uint16 ip_port;
    uint16 app_port;
    uint16 flower;
    uint16 time_interval;
    uint8  center_number[MAX_CY_CENTER_NUMBER_SIZE+1];
    uint8  password[MAX_CY_PASSWORD_SIZE+1];
    uint8  ip_domain[MAX_CY_IP_DOMAIN_SIZE+1];
    uint8  app_domain[MAX_CY_IP_DOMAIN_SIZE+1];
}CY_DB_SET_Type;

extern CY_DB_SET_Type cy_DB_Set_Rec;
extern BOOLEAN CY_DB_SET_ListInit(void);
extern BOOLEAN CY_DB_SET_ListModify(void);
extern BOOLEAN CY_DB_SET_ListDel(void);
#endif

BOOLEAN CY_DB_Init(CY_APP_T* pMe);
BOOLEAN CY_DB_DelAll(CY_APP_T* pMe);
BOOLEAN CY_DB_Free(CY_APP_T* pMe);


#endif /*_WLDT_CY_DB_H_*/
