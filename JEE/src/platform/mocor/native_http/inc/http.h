/*****************************************************************************
** File Name:        qq_http.h                                               *
** Author:           xiaoju.cheng                                            *
** Date:             11/30/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define qq http                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/31/2021           xiaoju.cheng          Create                         *
******************************************************************************/
#ifndef _QQ_HTTP_H_
#define _QQ_HTTP_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "ual_cms_export.h"
/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

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

/*****************************************************************************/
//  Description : qq http request
//  Parameter: [In] request_id:different app instance
//             [In] p_url:url to be download
//             [In] p_content:download data
//             [In] http_callback:callback
//             [Return] None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC http_request_result_e http_start_request(uint32 request_id, char* p_url, char* p_content, http_request_type_e request_type, _ual_cms_client_register_callback http_callback);

/*****************************************************************************/
//  Description : http_request_init
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void http_request_init(void);

/*****************************************************************************/
//  Description : qq http module init
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void  http_init_module(void);
/*****************************************************************************/
//  Description : set post request type
//  Parameter: None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void  js_http_set_content_type(char* p_content_type);

#endif

