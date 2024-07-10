/*****************************************************************************
** File Name:       json_parser_watchface.h                                  *
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
#ifndef _JSON_PARSER_WATCHFACE_H_
#define _JSON_PARSER_WATCHFACE_H_

#include "mmk_type.h"
#include "json_parser_common.h"
#include "watchface_ctrl_adapter.h"

#define MAX_MULTINUM_COUNT (5)

typedef struct
{
    float  min_value;  //�������λ��
    float  max_value;  //�������λ��
}WATCHFACE_RANGE_T;

typedef struct
{
    WATCHFACE_RATIO_POSITION_T  center_position;
    uint32                      update_interval;
    uint8*                      p_image_list[MAX_IMAGE_COUNT+1];/*AnalogTime�е�image,����image_hour��image_minute��image_second����ڴ�list��*/
    uint8                       image_none[MAX_NAME_LEN+1];
    uint8                       image_connector [MAX_NAME_LEN+1];
    uint8                       type[MAX_NAME_LEN+1];
    uint32                      digit_capacity;
    WATCHFACE_RANGE_T           range;
    uint32                      big_radius;
    uint32                      little_radius;
    uint32                      color_rgb; //ʮ��������
    uint8                       direction[MAX_NAME_LEN+1];
} WATCHFACE_OBJECT_COMMON_T;

typedef struct
{
    uint8                      object_name[MAX_NAME_LEN+1];
    uint32                     item_num;
    WATCHFACE_OBJECT_COMMON_T *p_object_info;
} WATCHFACE_OBJECT_LIST_T;

typedef struct
{
	uint8                       data_type[MAX_NAME_LEN];//multinum����
	WATCHFACE_RATIO_POSITION_T  center_position;
} EDIT_MULTINUM_INFO_T;

typedef struct
{
    uint8                       multinum_count;//multinum����
    EDIT_MULTINUM_INFO_T        multinum_info[MAX_MULTINUM_COUNT];
} WATCHFACE_EDIT_MULTINUM_T;

/*****************************************************************************/
//  Description : get watchface all key info:��������layout���ݣ���ȡ���пؼ���Ϣ
//  Parameter: [In] p_json_string:����layout�ļ�json����
//             [Out] p_object_num: ����layout��object������
//             [Out] p_scale: the image scale get from layout file
//             [Out] p_object_string[]:ָ�����飬��ȡ��Ӧ�����б��̿ؼ�����Ϣ
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceAllObjectInfo (
                                                    char*                       p_json_string,
                                                    uint8*                      p_object_num,
                                                    double*                     p_scale,
                                                    WATCHFACE_OBJECT_LIST_T*    p_object_list[]
                                                    );
/*****************************************************************************/
//  Description : Get Watchface Object Size:��������ĳ���ؼ�����Ϣ
//  Parameter: [In] p_json_string:����ĳ���ؼ���json����
//                  p_title:      ��ǩ����ΪPNULL
//             [Out]size:         ���͸���
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceObjectSize(char* p_json_string, char* p_title, uint8* p_size);

/*****************************************************************************/
//  Description : Get Watchface Multinum Info:�������̿ɱ༭multinum����Ϣ
//  Parameter: [In] index:��������ֵ
//             [Out]p_multinum_info:   ��Ϣ�ṹ��
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:JSONPARSER_GetWatchfaceMultinumInfo
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceMultinumInfo(uint16 index, WATCHFACE_EDIT_MULTINUM_T* p_multinum_info);

/*****************************************************************************/
//  Description : Set Watchface Multinum Info:���ñ��̿ɱ༭multinum����Ϣ
//  Parameter: [In] index:��������ֵ
//              p_multinum_info:   ��Ϣ�ṹ��
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:JSONPARSER_SetWatchfaceMultinumInfo
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetWatchfaceMultinumInfo(uint16 index, WATCHFACE_EDIT_MULTINUM_T* p_multinum_info);
/*****************************************************************************/
//  Description : Free object space
//  Parameter: [In]  None
//             [Out] p_object_list: the object will free
//             [Return] free space result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_FreeObjectList( WATCHFACE_OBJECT_LIST_T** p_object_list);

#endif
