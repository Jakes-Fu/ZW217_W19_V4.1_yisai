/*****************************************************************************
** File Name:        http.h                                                  *
** Author:           qi.zhang                                                *
** Date:             08/10/2022                                              *
** Copyright:                                                                *
** Description:    This file is used to define js http                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/31/2021           qi.zhang              Create                         *
******************************************************************************/
#ifdef JS_COMPENT_HTTP //for 217 ram cutdown

#ifndef _HTTP_H_
#define _HTTP_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "std.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
typedef unsigned char        boolean;

typedef enum
{
    HTTP_REQUEST_SUCESS = 0,
    HTTP_REQUEST_ERROR,
    HTTP_REQUEST_PARAM_ERROR,
    HTTP_REQUEST_RESULT_MAX,
}http_request_result_e;
typedef enum
{
    HTTP_GET_REQUEST,
    HTTP_POST_REQUEST,
    HTTP_OPTIONS_REQUEST,
    HTTP_REQUEST_MAX,
}http_request_type_e;
typedef struct
{
    uint8       result;
    uint32      handle;
    uint32      body_size;
    void*       p_body;
} http_callback_param_t;

typedef boolean(* http_register_callback)(http_callback_param_t param);

/*****************************************************************************/
//  Description : http request
//  Parameter: [In] request_id:different app instance
//             [In] p_url:url to be download
//             [In] p_content:download data
//             [In] request_type:post/get/options request
//             [In] http_callback:callback
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC http_request_result_e http_start_request(uint32 request_id, char* p_url, char* p_content, http_request_type_e request_type, http_register_callback http_callback);

/*****************************************************************************/
//  Description : http module init
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void  http_init_module(void);
/*****************************************************************************/
//  Description : http set content type
//  Parameter: p_content_type:content type
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void http_set_content_type(char* p_content_type);

#endif
#endif
