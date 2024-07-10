/****************************************************************************
** File Name:      json_parser_config                                       *
** Author:                                                                  *
** Date:           2021.8.20                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to do provide network access api       *
*****************************************************************************
**                         Important Edit History                           *
** --------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                  *
** 2021.8.20      xiaoju.cheng                 Create
**
****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "os_param.h"
#include "cJSON.h"
#include "json_parser_config.h"
#include "mmicom_trace.h"
/*---------------------------------------------------------------------------*/
/*                        Macro Declaration                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/
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
                                                    )
{
    cJSON*  p_root = PNULL;
    char*   p_string = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_date)
    {
       TRACE_JSON_PARSER("p_date is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);
    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }
    p_string = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "date"));

    if(PNULL != p_string)
    {
        SCI_MEMCPY(p_date,p_string,MAX_NAME_LEN);
        TRACE_JSON_PARSER("date is %s",p_date);
    }

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : set config date:����������Ϣ��dateֵ
//  Parameter: [In] p_json_string: config�ļ���json���ݡ�
//             [In] p_date
//             [Out] p_json_string:����֮���config�ļ���json���ݡ�
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetConfigDate(
                                                    char*            p_json_string,
                                                    char*            p_date
                                                    )
{
    cJSON*  p_root = PNULL;
    char*   p_string = PNULL;
    cJSON*  p_new_json_ptr = PNULL;
    cJSON*  p_json_item_ptr = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_date)
    {
       TRACE_JSON_PARSER("p_date is NULL");
       return JSON_STRING_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }
    p_json_item_ptr = cJSON_GetObjectItem(p_root, "date");
    if (PNULL == p_json_item_ptr)
    {
        TRACE_JSON_PARSER("p_json_item_ptr is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    if(PNULL != p_json_item_ptr->valuestring)
    {
        SCI_FREE(p_json_item_ptr->valuestring);
        p_json_item_ptr->valuestring = PNULL;
        p_json_item_ptr->valuestring = SCI_ALLOC_APP(strlen(p_date)+1);
        if(PNULL != p_json_item_ptr->valuestring)
        {
            SCI_MEMSET(p_json_item_ptr->valuestring,0x00,strlen(p_json_item_ptr->valuestring));
            SCI_MEMCPY(p_json_item_ptr->valuestring,p_date,strlen(p_date)+1);
            TRACE_JSON_PARSER("image_minute is %s",p_date);
        }
    }

    TRACE_JSON_PARSER("date is %s",p_date);

    p_string = cJSON_Print(p_root);
    if(PNULL == p_string)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_json_string,0x00,strlen(p_json_string));
    SCI_MEMCPY(p_json_string,p_string,strlen(p_string));

    SCI_FREE(p_string);
    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

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
                                                    )
{
    cJSON*  p_root = PNULL;
    uint16  current_watchface = 0;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_value)
    {
       TRACE_JSON_PARSER("p_value is NULL");
       return JSON_STRING_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    current_watchface = cJSON_GetObjectInt(cJSON_GetObjectItem(p_root, "current_watchface"));
    *p_value = current_watchface;
    TRACE_JSON_PARSER("current_watchface is %d",current_watchface);

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : set current watch face:����������Ϣ��currentfaceֵ
//  Parameter: [In] p_json_string:config�ļ���json���ݡ�
//             [In] p_value
//             [Out] p_json_string:����֮���config�ļ���json���ݡ�
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetConfigCurrentWatchface (
                                                    char*            p_json_string,
                                                    uint16           value
                                                    )
{
    cJSON*  p_root = PNULL;
    char*   p_string = PNULL;
    cJSON*  p_json_item_ptr = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_item_ptr = cJSON_GetObjectItem(p_root, "current_watchface");//������json����

    if(PNULL == p_json_item_ptr)
    {
        TRACE_JSON_PARSER("p_json_item_ptr is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }
#ifdef WIN32
    p_json_item_ptr->valuedouble = value;
#else
    p_json_item_ptr->valueint = value;
#endif

    TRACE_JSON_PARSER("current_watchface is %d",value);

    p_string = cJSON_Print(p_root);
    if(PNULL == p_string)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_json_string,0x00,strlen(p_json_string));
    SCI_MEMCPY(p_json_string,p_string,strlen(p_string));
    SCI_FREE(p_string);
    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

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
                                                    )
{
    cJSON*  p_root = PNULL;
    cJSON*  p_json_array = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }
    if (PNULL == p_count)
    {
       TRACE_JSON_PARSER("p_count is NULL");
       return JSON_STRING_NULL;
    }
    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_array = cJSON_GetObjectItem(p_root, "app_info");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    *p_count = (uint8)cJSON_GetArraySize(p_json_array);
    TRACE_JSON_PARSER("array count is %d",*p_count);

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

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
                                                    )
{
    char*    id = PNULL;
    char*    app_name = PNULL;
    char*    version_name = PNULL;
    char*    type = PNULL;
    char*    introduction = PNULL;
    char*    icon = PNULL;
    char*    url = PNULL;
    cJSON*   p_root = PNULL;
    cJSON*   p_json_array = PNULL;
    cJSON*   p_json_item_ptr = 0;

    uint16  is_edit = 0;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_object_info)
    {
       TRACE_JSON_PARSER("p_object_info is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);
    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_array = cJSON_GetObjectItem(p_root, "app_info");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    p_json_item_ptr = cJSON_GetArrayItem(p_json_array, index);

    id = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "id"));
    if(PNULL != id)
    {
        SCI_MEMCPY(p_object_info->id,id,MAX_NAME_LEN);
        TRACE_JSON_PARSER("id is %s",id);
    }

    app_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "app_name"));
    if(PNULL != app_name)
    {
        SCI_MEMCPY(p_object_info->app_name,app_name,MAX_NAME_LEN);
        TRACE_JSON_PARSER("app_name is %s",app_name);
    }

    p_object_info->version_code = cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "version_code"));

    version_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "version_name"));
    if(PNULL != version_name)
    {
        SCI_MEMCPY(p_object_info->version_name,version_name,MAX_NAME_LEN);
        TRACE_JSON_PARSER("version_name is %s",version_name);
    }

    type = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "type"));
    if(PNULL != type)
    {
        SCI_MEMCPY(p_object_info->type,type,MAX_NAME_LEN);
        TRACE_JSON_PARSER("type is %s",type);
    }

    introduction = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "introduction"));
    if(PNULL != introduction)
    {
        SCI_MEMCPY(p_object_info->introduction,introduction,MAX_NAME_LEN);
        TRACE_JSON_PARSER("introduction is %s",introduction);
    }

    icon = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "icon"));
    if(PNULL != icon)
    {
        SCI_MEMCPY(p_object_info->icon,icon,MAX_NAME_LEN);
        TRACE_JSON_PARSER("icon is %s",icon);
    }

    url = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "url"));
    if(PNULL != url)
    {
        SCI_MEMCPY(p_object_info->url,url,MAX_PATH_LEN);
        TRACE_JSON_PARSER("url is %s",url);
    }

    p_object_info->size = cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "size"));

    is_edit = cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "edit"));

    TRACE_JSON_PARSER("is_edit is %d",is_edit);
    if(is_edit)
    {
        p_object_info->is_edit = TRUE;
    }
    else
    {
        p_object_info->is_edit = FALSE;
    }

    p_object_info->is_edit = is_edit;

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

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
                                                    )
{
    cJSON*  p_root = PNULL;
    cJSON*  p_json_object = PNULL;
    cJSON*  p_json_array = 0;
    char*   p_string = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_object_info)
    {
        TRACE_JSON_PARSER("p_object_info is NULL");
        return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_array = cJSON_GetObjectItem(p_root, "app_info");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    p_json_object = cJSON_CreateObject();

    cJSON_AddStringToObject(p_json_object,"id",p_object_info->id);
    cJSON_AddStringToObject(p_json_object,"app_name",p_object_info->app_name);
    cJSON_AddNumberToObject(p_json_object,"version_code",p_object_info->version_code);
    cJSON_AddStringToObject(p_json_object,"version_name",p_object_info->version_name);
    cJSON_AddStringToObject(p_json_object,"type",p_object_info->type);
    cJSON_AddStringToObject(p_json_object,"introduction",p_object_info->introduction);
    cJSON_AddStringToObject(p_json_object,"icon",p_object_info->icon);
    cJSON_AddStringToObject(p_json_object,"url",p_object_info->url);
    cJSON_AddNumberToObject(p_json_object,"size",p_object_info->size);

    cJSON_AddItemToArray(p_json_array, p_json_object);

    p_string = cJSON_Print(p_root);
    if(PNULL == p_string)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_json_string,0x00,strlen(p_json_string));
    SCI_MEMCPY(p_json_string,p_string,strlen(p_string));
    SCI_FREE(p_string);

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

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
                                                    )
{
    cJSON*   p_root = PNULL;
    cJSON*   p_json_object = PNULL;
    cJSON*   p_json_array = PNULL;
    uint16   json_array_size = 0;
    char*    p_string = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_array = cJSON_GetObjectItem(p_root, "app_info");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }
    json_array_size = (uint16)cJSON_GetArraySize(p_json_array);

    TRACE_JSON_PARSER("json_array_size=%d,index=%d",json_array_size,index);
    if(json_array_size >= index)
    {
        cJSON_DeleteItemFromArray(p_json_array,index);
    }
    else
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_OUTOF_RANGE;
    }

    p_string = cJSON_Print(p_root);
    if(PNULL == p_string)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_json_string,0x00,strlen(p_json_string));
    SCI_MEMCPY(p_json_string,p_string,strlen(p_string));
    SCI_FREE(p_string);

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;

}

/*****************************************************************************/
//  Description : up_date a config info:�޸�������Ϣ�е�ĳһ��Ӧ��
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
                                                    )
{
    cJSON*   p_root = PNULL;
    cJSON*   p_json_object = PNULL;
    cJSON*   p_json_array = PNULL;
    uint16   json_array_size = 0;
    char*    p_string = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    p_json_array = cJSON_GetObjectItem(p_root, "app_info");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    json_array_size = (uint16)cJSON_GetArraySize(p_json_array);

    TRACE_JSON_PARSER("json_array_size=%d,index=%d",json_array_size,index);
    if(json_array_size < index)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_OUTOF_RANGE;
    }

    p_json_object = cJSON_CreateObject();

    cJSON_AddStringToObject(p_json_object,"id",object_info.id);
    cJSON_AddStringToObject(p_json_object,"app_name",object_info.app_name);
    cJSON_AddNumberToObject(p_json_object,"version_code",object_info.version_code);
    cJSON_AddStringToObject(p_json_object,"version_name",object_info.version_name);
    cJSON_AddStringToObject(p_json_object,"type",object_info.type);
    cJSON_AddStringToObject(p_json_object,"introduction",object_info.introduction);
    cJSON_AddStringToObject(p_json_object,"icon",object_info.icon);
    cJSON_AddStringToObject(p_json_object,"url",object_info.url);
    cJSON_AddNumberToObject(p_json_object,"size",object_info.size);
    cJSON_AddNumberToObject(p_json_object,"edit",object_info.is_edit);

    cJSON_ReplaceItemInArray(p_json_array,index,p_json_object);
    p_string = cJSON_Print(p_root);
    if(PNULL == p_string)
    {
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_json_string,0x00,strlen(p_json_string));
    SCI_MEMCPY(p_json_string,p_string,strlen(p_string));
    SCI_FREE(p_string);

    //cJSON_Delete(p_json_object);
    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}
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
                                                    )
{
    char*    id = PNULL;
    char*    app_name = PNULL;
    char*    version_name = PNULL;
    char*    type = PNULL;
    char*    introduction = PNULL;
    char*    icon = PNULL;
    cJSON*   p_root = PNULL;

    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_object_info)
    {
       TRACE_JSON_PARSER("p_object_info is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);
    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }

    id = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "id"));
    if(PNULL != id)
    {
        SCI_MEMCPY(p_object_info->id,id,MAX_NAME_LEN);
        TRACE_JSON_PARSER("id is %s",id);
    }

    app_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "app_name"));
    if(PNULL != app_name)
    {
        SCI_MEMCPY(p_object_info->app_name,app_name,MAX_NAME_LEN);
        TRACE_JSON_PARSER("app_name is %s",app_name);
    }

    p_object_info->version_code = cJSON_GetObjectInt(cJSON_GetObjectItem(p_root, "version_code"));
    
    version_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "version_name"));
    if(PNULL != version_name)
    {
        SCI_MEMCPY(p_object_info->version_name,version_name,MAX_NAME_LEN);
        TRACE_JSON_PARSER("version_name is %s",version_name);
    }

    type = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "type"));
    if(PNULL != type)
    {
        SCI_MEMCPY(p_object_info->type,type,MAX_NAME_LEN);
        TRACE_JSON_PARSER("type is %s",type);
    }

    introduction = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "introduction"));
    if(PNULL != introduction)
    {
        SCI_MEMCPY(p_object_info->introduction,introduction,MAX_NAME_LEN);
        TRACE_JSON_PARSER("introduction is %s",introduction);
    }

    icon = cJSON_GetObjectString(cJSON_GetObjectItem(p_root, "icon"));
    if(PNULL != icon)
    {
        SCI_MEMCPY(p_object_info->icon,icon,MAX_NAME_LEN);
        TRACE_JSON_PARSER("icon is %s",icon);
    }
    p_object_info->size = cJSON_GetObjectInt(cJSON_GetObjectItem(p_root, "size"));

    p_object_info->is_edit = cJSON_GetObjectInt(cJSON_GetObjectItem(p_root, "edit"));

    TRACE_JSON_PARSER("is_edit is %d",p_object_info->is_edit);

    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}
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
                                                    )
{
    char*    id = PNULL;
    char*    app_name = PNULL;
    cJSON*   p_json_array = PNULL;
    char*    icon = PNULL;
    cJSON*   p_root = PNULL;
    if (PNULL == p_json_string)
    {
        TRACE_JSON_PARSER("p_json_string is NULL");
        return JSON_STRING_NULL;
    }

    if (PNULL == p_object_info)
    {
       TRACE_JSON_PARSER("p_object_info is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);
    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is NULL");
        return JSON_PARSER_ERROR;
    }
    p_json_array = cJSON_GetObjectItem(p_root, "app");//������json����

    if(PNULL == p_json_array)
    {
        TRACE_JSON_PARSER("p_json_array is NULL");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }
    id = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_array, "id"));
    if(PNULL != id)
    {
        SCI_MEMCPY(p_object_info->id,id,MAX_NAME_LEN);
        TRACE_JSON_PARSER("id is %s",id);
    }

    app_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_array, "name"));
    if(PNULL != app_name)
    {
        SCI_MEMCPY(p_object_info->app_name,app_name,MAX_NAME_LEN);
        TRACE_JSON_PARSER("app_name is %s",app_name);
    }
    icon = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_array, "icon"));
    if(PNULL != icon)
    {
        SCI_MEMCPY(p_object_info->icon,icon,MAX_NAME_LEN);
        TRACE_JSON_PARSER("icon is %s",icon);
    }
    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

