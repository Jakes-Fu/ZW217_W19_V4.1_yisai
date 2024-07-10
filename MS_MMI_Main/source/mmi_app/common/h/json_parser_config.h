/*****************************************************************************
** File Name:       json_parser_config.h                                     *
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
#ifndef _JSON_PARSER_CONFIG_H_
#define _JSON_PARSER_CONFIG_H_

#include "mmk_type.h"
#include "json_parser_common.h"

typedef struct
{
    uint8       id[MAX_NAME_LEN+1];
    uint8       app_name[MAX_NAME_LEN+1];
    uint16      version_code;
    uint8       version_name[MAX_NAME_LEN+1];
    uint8       type[MAX_NAME_LEN+1];
    uint8       introduction[MAX_NAME_LEN+1];
    uint8       icon[MAX_NAME_LEN+1];
    uint8       url[MAX_PATH_LEN+1];
    uint32      size;
    BOOLEAN     is_edit;
} COMMON_CONFIG_OBJECT_T;

/*****************************************************************************/
//  Description : get config date:获取配置信息中的date值。
//  Parameter: [In] p_json_string: config文件的json数据。
//             [Out] p_date
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetConfigDate(
                                                    char*            p_json_string,
                                                    char*            p_date
                                                    );
/*****************************************************************************/
//  Description : set config date:设置配置信息的date值
//  Parameter: [In/Out] p_json_string: config文件的json数据。
//             [In] p_date
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetConfigDate(
                                                    char*            p_json_string,
                                                    char*            p_date
                                                    );
/*****************************************************************************/
//  Description : get current watch face:获取配置信息中的currentface值。
//  Parameter: [In] p_json_string:config文件的json数据。
//             [Out] p_value
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetConfigCurrentWatchface(
                                                    char*            p_json_string,
                                                    uint16*          p_value
                                                    );
/*****************************************************************************/
//  Description : set current watch face:设置配置信息的currentface值
//  Parameter: [In] p_json_string:config文件的json数据。
//             [In] pValue
//             [Out] p_json_string:更新之后的config文件的json数据。
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetConfigCurrentWatchface (
                                                    char*            p_json_string,
                                                    uint16           value
                                                    );

/*****************************************************************************/
//  Description : set config array count:获取配置信息中所有应用的数量。
//  Parameter: [In] p_json_string:config文件的json数据。
//             [Out] p_count
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetConfigArrayCount (
                                                    char            *p_json_string,
                                                    uint16          *p_count
                                                    );
/*****************************************************************************/
//  Description : get config info:获取配置文件中某一个应用的数据
//  Parameter: [In] p_json_string:config文件的json数据
//             [In] index:需要获取的应用的index
//             [Out] p_object_info:输出的config应用信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetConfigObjectInfo (
                                                    char                        *p_json_string,
                                                    uint16                       index,
                                                    COMMON_CONFIG_OBJECT_T      *p_object_info
                                                    );
/*****************************************************************************/
//  Description : add a new config info:配置信息增加一个应用
//  Parameter: [In/Out] p_json_string:config文件的json数据
//             [In] p_object_info:需要增加的应用数据
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_AddConfigObject (
                                                    char*                           p_json_string,
                                                    COMMON_CONFIG_OBJECT_T*         p_object_info
                                                    );
/*****************************************************************************/
//  Description : delete a config info:删除配置文件的某一个应用
//  Parameter: [In/Out] p_json_string:config文件的json数据
//             [In] index:要删除的应用的index
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_DeleteConfigObject (
                                                    char*                       p_json_string,
                                                    uint16                      index
                                                    );
/*****************************************************************************/
//  Description : update a config info:修改配置信息中的某一个应用
//  Parameter: [In/Out] p_json_string:config文件的json数据
//             [In] index:需要修改的应用的index
//             [In] object_info:需要进行修改的应用数据
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_UpdateConfigObject (
                                                    char*                           p_json_string, 
                                                    uint16                          index,
                                                    COMMON_CONFIG_OBJECT_T          object_info
                                                    );
/*****************************************************************************/
//  Description : 获取Description.json中的信息。
//  Parameter: [In] p_json_string: Description文件的json数据。
//             [Out] p_object_info
//             [Return] JSON_PARSER_RESULT_E
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetDescriptionInfo(
                                                    char                        *p_json_string,
                                                    COMMON_CONFIG_OBJECT_T      *p_object_info
                                                    );
/*****************************************************************************/
//  Description : 获取app.json中的信息。
//  Parameter: [In] p_json_string: Description文件的json数据。
//             [Out] p_object_info
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetAppJsonInfo(
                                                    char                        *p_json_string,
                                                    COMMON_CONFIG_OBJECT_T      *p_object_info
                                                    );

#endif

