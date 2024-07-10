/****************************************************************************
** File Name:      mmisrvmgr.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of                 
**                     MMI service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/

#ifndef _MMISRVMGR_H_
#define _MMISRVMGR_H_

#include "mmisrv.h"

#define MMISRVMGR_SERVICE_NAME_LEN  20

typedef enum
{
    MMISRVMGR_NOTIFY_NONE,
    MMISRVMGR_NOTIFY_REQ,
    MMISRVMGR_NOTIFY_SUSPEND,
    MMISRVMGR_NOTIFY_RESUME,
    MMISRVMGR_NOTIFY_FREE,
    MMISRVMGR_NOTIFY_EXT,
    MMISRVMGR_NOTIFY_EVENT_MAX
}MMISRVMGR_NOTIFY_EVENT_E;

typedef struct
{
    MMISRVMGR_NOTIFY_EVENT_E event;
    uint32 ind_data;
    uint32 data;
} MMISRVMGR_NOTIFY_PARAM_T;


typedef enum
{
    MMISRVMGR_SERVICE_CMD_NONE,
    MMISRVMGR_SERVICE_CMD_REQ,
    MMISRVMGR_SERVICE_CMD_SUSPEND,
    MMISRVMGR_SERVICE_CMD_RESUME,
    MMISRVMGR_SERVICE_CMD_FREE,
    MMISRVMGR_SERVICE_CMD_GET_ENTITY,
    MMISRVMGR_SERVICE_CMD_EXT,
    MMISRVMGR_SERVICE_CMD_MAX
}MMISRVMGR_SERVICE_CMD_E;

typedef struct
{
    MMISRVMGR_SERVICE_CMD_E cmd;
    MMISRV_HANDLE_T handle;
    uint32 data;
    uint32 ext_data;
}MMISRVMGR_SERVICE_CMD_PARAM_T;

typedef BOOLEAN (*MMISRVMGR_NOTIFY_FUNC)(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);
//typedef BOOLEAN (*MMISRVMGR_SERVICE_CMD_FUNC)(MMISRVMGR_SERVICE_CMD_PARAM_T *param);
typedef BOOLEAN (*MMISRVMGR_SERVICE_CMD_FUNC)(MMISRVMGR_SERVICE_CMD_PARAM_T *param, uint32 *res_data);


typedef enum
{
    MMISRVMGR_SERVICE_RPT_NONE,
    MMISRVMGR_SERVICE_RPT_REQ,
    MMISRVMGR_SERVICE_RPT_SUSPEND,
    MMISRVMGR_SERVICE_RPT_RESUME,
    MMISRVMGR_SERVICE_RPT_FREE,
    MMISRVMGR_SERVICE_RPT_MAX
}MMISRVMGR_SERVICE_REPORT_E;

typedef struct
{
MMISRVMGR_SERVICE_CMD_E cmd;
MMISRV_HANDLE_T handle;
//void *data;
}MMISRVMGR_SERVICE_REPORT_PARAM_T;

typedef struct
{
    /* 'id' and 'name' is used to distinguish the different service, but we use 'name' now and give up 'id' */
    //uint32 id;
    uint8 name[MMISRVMGR_SERVICE_NAME_LEN];
    /* the num of supported pri */
    uint16 pri_num;
    /* the num of the max service that one service module can provide at the same time */
    uint16 max_num;
    /* the cmd function called by service manager to communicate with service entity */
    MMISRVMGR_SERVICE_CMD_FUNC cmd_func;
}MMISRVMGR_SERVICE_CREATE_T;

typedef struct
{
    uint16 pri;
    BOOLEAN is_async;
    BOOLEAN is_auto_resume_off; /* If TRUE, Don't resume the suspended service(s) when it is freed */
    BOOLEAN is_auto_free;   /* If TRUE, the service will be freed automatically when it is finished. */
    uint32 ind_data;
    char *client_name;
    MMISRVMGR_NOTIFY_FUNC notify;
}MMISRVMGR_SERVICE_REQ_T;

typedef enum
{
    MMISRVMGR_SERVICE_STATE_ACTIVE = 0,     /* the service is avtive state, and can provide service */
    MMISRVMGR_SERVICE_STATE_SUSPEND,        /* The service is suspended manually*/
    MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED,   /* The service is suspended by other service */
    MMISRVMGR_SERVICE_STATE_READY,           /* The service is waiting for being actived */
    MMISRVMGR_SERVICE_STATE_STANDING,      /* The sevice is resident service, it can provide service anytime */
    MMISRVMGR_SERVICE_STATE_MAX
}MMISRVMGR_SERVICE_STATE_E;













#define MMISRV_VTBL_T( _iname_ )  _iname_##_VTBL

#define MMISRV_INTERFACE( _iname_ ) \
    typedef struct MMISRV_VTBL_T( _iname_ )  MMISRV_VTBL_T( _iname_ ); \
    struct MMISRV_VTBL_T( _iname_ )

typedef struct IMMISRV_UNKNOWN_T  IMMISRV_UNKNOWN_T;

#define IMMISRV_INHERIT_BASE  \
    BOOLEAN (*Resume)(IMMISRV_UNKNOWN_T *ptr); \
    BOOLEAN (*Suspend)(IMMISRV_UNKNOWN_T *ptr); \
    BOOLEAN (*Free)(IMMISRV_UNKNOWN_T *ptr); \
    BOOLEAN (*SetPri)(IMMISRV_UNKNOWN_T *ptr, uint16 pri); \
    BOOLEAN (*SetNotify)(IMMISRV_UNKNOWN_T *ptr, MMISRVMGR_NOTIFY_FUNC notify); \
    BOOLEAN (*IoCtrl)(IMMISRV_UNKNOWN_T *ptr, uint32 data, void *res);

MMISRV_INTERFACE(IMMISRV_UNKNOWN_T)
{
    IMMISRV_INHERIT_BASE
};

struct IMMISRV_UNKNOWN_T
{
   MMISRV_VTBL_T(IMMISRV_UNKNOWN_T) *vbtl_ptr;
};


/*-------------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Request a new service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVMGR_Request(char *srv_name, MMISRVMGR_SERVICE_REQ_T *req_data, void *ext_data);


/*****************************************************************************/
//  Description : Free a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Free(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : Suspend a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Suspend(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : Resume a service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Resume(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : Add a new service priority.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetPri(MMISRV_HANDLE_T handle, uint16 pri);

/*****************************************************************************/
//  Description : Set service notify function.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_FUNC notify);

/*****************************************************************************/
//  Description : Communicate between client and service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_IoCtrl(MMISRV_HANDLE_T handle, uint32 data, void *res);

/*****************************************************************************/
//  Description : Add a new service to service db.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRVMGR_AddService(MMISRVMGR_SERVICE_CREATE_T *srv_data, void *ext_data, uint32 ext_data_size);

/*****************************************************************************/
//  Description : Init service mananement.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_Init(void);

/*****************************************************************************/
//  Description : Send command to a appointed service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SendCmd(char *srv_name, uint32 cmd_data, uint32 *res_data);

#endif


