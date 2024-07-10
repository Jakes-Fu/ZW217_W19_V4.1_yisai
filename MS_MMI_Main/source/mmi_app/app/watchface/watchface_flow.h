/*****************************************************************************
** File Name:        watchface_flow.h                                        *
** Author:           xiaoju.cheng                                            *
** Date:             08/11/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define json parser                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/11/2021           xiaoju.cheng          Create                         *
******************************************************************************/
#ifndef _WATCHFACE_FLOW_H_
#define _WATCHFACE_FLOW_H_

#include "mmk_type.h"
#include "ctrlwf_export.h"
#include "watchface_ctrl_adapter.h"
#include "json_parser_common.h"

#define MAX_NAME_LEN (50)

typedef BOOLEAN    (*WATCHFACE_Destroy_CallBack)(MMI_HANDLE_T handle);
typedef MMI_HANDLE_T   (*WATCHFACE_Create_CallBack)(MMI_WIN_ID_T   win_id,
                                                WATCHFACE_CTRL_PARAM_T*   p_param);
typedef void    (*WATCHFACE_Restart_Timer_CallBack)(MMI_HANDLE_T handle);
typedef BOOLEAN    (*WATCHFACE_Stop_Timer_CallBack)(MMI_HANDLE_T handle);



typedef struct
{
    uint8                               object_name[MAX_NAME_LEN];
    WATCHFACE_Create_CallBack           object_callback;
    WATCHFACE_Stop_Timer_CallBack       stop_timer_callback;
    WATCHFACE_Restart_Timer_CallBack    restart_timer_callback;
    WATCHFACE_Destroy_CallBack          destroy_callback;
}WATCHFACE_OBJECT_ITEM_ST;

typedef struct
{
    MMI_HANDLE_T                    handle;
    WATCHFACE_Destroy_CallBack      destroy_callback;
}WATCHFACE_CTRL_DESTROY_INFO_T;


typedef struct
{
    WATCHFACE_Create_CallBack       object_callback;
    WATCHFACE_CTRL_PARAM_T*         p_ctrl_param;
}WATCHFACE_CTRL_CREATE_INFO_T;
typedef struct
{
    uint8                          item_number;
    WATCHFACE_CTRL_CREATE_INFO_T  *p_ctrl_create_info;
}WATCHFACE_CTRL_CREATE_LIST_T;


typedef struct
{
    WATCHFACE_ALL_TYPE_E  type;
    char                  type_str[MAX_NAME_LEN];
}WATCHFACE_ALL_TYPE_T;

/*****************************************************************************/
//  Description : watchface fullpaint
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_FullPaint(void);

/*****************************************************************************/
//  Description : append watchface layer
//  Parameter: None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_AppendLayer(void);

/*****************************************************************************/
//  Description : remove watchface layer
//  Parameter: None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_RemoveLayer(void);
/************************************************************************************************/
//  Description : WATCHFACE_Construct_Layout_Data:解析表盘layout数据，并存到全局变量s_p_display_info
//  Parameter: [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/************************************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Construct_Layout_Data(void);

/*****************************************************************************/
//  Description : stop watchface timer
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_StopTimer(void);

#endif

