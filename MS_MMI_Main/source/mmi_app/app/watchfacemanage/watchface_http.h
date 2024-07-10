/*****************************************************************************
** File Name:        watchface_http.h                                        *
** Author:           xiaoju.cheng                                            *
** Date:             08/31/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define watch face http               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/31/2021           xiaoju.cheng          Create                         *
******************************************************************************/
#ifndef _WATCHFACE_HTTP_H_
#define _WATCHFACE_HTTP_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : watchface http start
//  Parameter: [In] p_url:url to be download
//             [In] p_id:download file name
//             [Return] BOOLEAN:TRUE,success,FALSE,fail
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_HttpStart(char* p_url, char* p_id);

/*****************************************************************************/
//  Description : watchface http module init
//  Parameter: None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_HTTP_InitModule(void);

#endif

