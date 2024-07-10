/*****************************************************************************
** File Name:        appstore_common.h                                       *
** Author:           xiuyun.wang                                             *
** Date:             08/13/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define app store common api and data *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/12/2021           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _APPSTORE_COMMON_H_
#define _APPSTORE_COMMON_H_
#include "os_api.h"
#include "sci_types.h"

#ifndef WIN32
#define TRACE_APPSTORE(_format,...) \
    do \
    { \
        SCI_TraceLow("[%s]<%d> "_format, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#else
#define TRACE_APPSTORE(_format,...) \
    do \
    { \
        printf(""_format, ##__VA_ARGS__);\
    }while(0)
#endif

/*define APP Store handle function result enum */
typedef enum
{
    APPSTORE_RES_SUCC,
    APPSTORE_RES_PARAM_ERR,
    APPSTORE_RES_FILE_MISSED,
    APPSTORE_RES_NO_ENOUGH_SPACE,
    APPSTORE_RES_FAIL,
    APPSTORE_RES_MAX
}APPSTORE_RES_E;

/*define APP Node state enum*/
typedef enum
{
    APPSTORE_APP_STATE_UNINSTALL, //未安装
    APPSTORE_APP_STATE_INSTALLED,//已安装
    APPSTORE_APP_STATE_INSTALLING,//安装中
    APPSTORE_APP_STATE_NEED_UPDATE,//待更新
    APPSTORE_APP_STATE_UPDATING,//更新中
    APPSTORE_APP_STATE_OFFLINE,//已下线
    APPSTORE_APP_STATE_MAX
}APPSTORE_APP_STATE_E;

/*define APP Node struct information*/
typedef struct
{
    uint8 *p_app_id;
    uint8 *p_app_name;
    uint16 app_version_code;
    uint8 *p_app_version_name;
    uint8 *p_app_type;
    uint8 *p_app_introduction;
    uint8 *p_app_icon_full_name;
    uint32 app_size;
    APPSTORE_APP_STATE_E app_node_state;
    struct APPSTORE_APP_NODE_T *p_app_node_next;
}APPSTORE_APP_NODE_T;

/*define APP Node struct information*/
typedef struct
{
    uint16 app_id_len;
    uint16 app_name_len;
    uint16 app_version_name_len;
    uint16 app_type_len;
    uint16 app_introduction_len;
    uint16 app_icon_full_name_len;
}APPSTORE_APP_NODE_LEN_T;


typedef struct
{
    uint32 app_num;
    APPSTORE_APP_NODE_T *p_app_node_header;
}APPSTORE_APP_LIST_T;

#endif

