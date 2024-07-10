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
//  Description : get config date:��ȡ������Ϣ�е�dateֵ��
//  Parameter: [In] p_json_string: config�ļ���json���ݡ�
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
//  Description : set config date:����������Ϣ��dateֵ
//  Parameter: [In/Out] p_json_string: config�ļ���json���ݡ�
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
//  Description : get current watch face:��ȡ������Ϣ�е�currentfaceֵ��
//  Parameter: [In] p_json_string:config�ļ���json���ݡ�
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
//  Description : set current watch face:����������Ϣ��currentfaceֵ
//  Parameter: [In] p_json_string:config�ļ���json���ݡ�
//             [In] pValue
//             [Out] p_json_string:����֮���config�ļ���json���ݡ�
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetConfigCurrentWatchface (
                                                    char*            p_json_string,
                                                    uint16           value
                                                    );

/*****************************************************************************/
//  Description : set config array count:��ȡ������Ϣ������Ӧ�õ�������
//  Parameter: [In] p_json_string:config�ļ���json���ݡ�
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
//  Description : get config info:��ȡ�����ļ���ĳһ��Ӧ�õ�����
//  Parameter: [In] p_json_string:config�ļ���json����
//             [In] index:��Ҫ��ȡ��Ӧ�õ�index
//             [Out] p_object_info:�����configӦ����Ϣ
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
//  Description : add a new config info:������Ϣ����һ��Ӧ��
//  Parameter: [In/Out] p_json_string:config�ļ���json����
//             [In] p_object_info:��Ҫ���ӵ�Ӧ������
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_AddConfigObject (
                                                    char*                           p_json_string,
                                                    COMMON_CONFIG_OBJECT_T*         p_object_info
                                                    );
/*****************************************************************************/
//  Description : delete a config info:ɾ�������ļ���ĳһ��Ӧ��
//  Parameter: [In/Out] p_json_string:config�ļ���json����
//             [In] index:Ҫɾ����Ӧ�õ�index
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_DeleteConfigObject (
                                                    char*                       p_json_string,
                                                    uint16                      index
                                                    );
/*****************************************************************************/
//  Description : update a config info:�޸�������Ϣ�е�ĳһ��Ӧ��
//  Parameter: [In/Out] p_json_string:config�ļ���json����
//             [In] index:��Ҫ�޸ĵ�Ӧ�õ�index
//             [In] object_info:��Ҫ�����޸ĵ�Ӧ������
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
//  Description : ��ȡDescription.json�е���Ϣ��
//  Parameter: [In] p_json_string: Description�ļ���json���ݡ�
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
//  Description : ��ȡapp.json�е���Ϣ��
//  Parameter: [In] p_json_string: Description�ļ���json���ݡ�
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

