/*****************************************************************************
** File Name:      mmisfr_ctcc_http.h
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME                            DESCRIPTION
** 2020/11/24        mark.zhang1                Create
******************************************************************************/

#ifndef _MMISFR_CTCC_HTTP_H_
#define _MMISFR_CTCC_HTTP_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "http_api.h"

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**--------------------------------------------------------------------------*/
typedef struct _SFR_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance;
} SFR_HTTP_CONTEXT;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Entry of http
//  Param : net_id-app use this net_id to access network.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void sfr_reg_by_http(uint32 net_id);

/*****************************************************************************/
//  Description : network disconnected
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void sfr_notify_deactive_ind(void);

/*****************************************************************************/
//  Description :
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E sfr_handle_http_msg(PWND app_ptr, uint16 msg_id, DPARAM param);

#endif

