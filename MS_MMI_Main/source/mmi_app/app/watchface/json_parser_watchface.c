/****************************************************************************
** File Name:      json_parser_watchface                                    *
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
#include "json_parser_watchface.h"
#include "mmicom_trace.h"
#include "sfs.h"

/*---------------------------------------------------------------------------*/
/*                        Macro Declaration                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Free object space
//  Parameter: [In]  None
//             [Out] p_object_list: the object will free
//             [Return] free space result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_FreeObjectList( WATCHFACE_OBJECT_LIST_T** p_object_list)
{
    WATCHFACE_OBJECT_LIST_T* p_temp_list = PNULL;
    uint8 item_num = 0;
    uint32 i = 0;//bug 1958580 CID 589713
    uint8 j = 0;
    uint8 image_num = 0;
    TRACE_JSON_PARSER("begin Free space");
    if( PNULL == p_object_list)
    {
        TRACE_JSON_PARSER("the param is error");
        return JSON_PARSER_ERROR;
    }
    if(PNULL == *p_object_list)
    {
        TRACE_JSON_PARSER("the param is error");
        return JSON_PARSER_ERROR;
    }
    p_temp_list = *p_object_list;

    for(i = 0; i < p_temp_list->item_num; i++)
    {
        for(j=0; PNULL != p_temp_list->p_object_info[i].p_image_list[j]; j++)
        {
            SCI_FREE(p_temp_list->p_object_info[i].p_image_list[j]);
            p_temp_list->p_object_info[i].p_image_list[j] = PNULL;
        }
    }
    if(PNULL != p_temp_list->p_object_info)
    {
        SCI_FREE(p_temp_list->p_object_info);
        p_temp_list->p_object_info = PNULL;
    }
    if(PNULL != p_temp_list)
    {
        SCI_FREE(p_temp_list);
        p_temp_list = PNULL;
    }
    *p_object_list = PNULL;
    TRACE_JSON_PARSER("finish Free space");
    return JSON_PARSER_SUCCESS;//bug 1958580 CID 589411
}

/*****************************************************************************/
//  Description : Create object space
//  Parameter: [In]  None
//             [Out] p_app_node: the app node
//             [Return] create space result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E CreateObjectList(char* p_object_name, uint32 item_num, WATCHFACE_OBJECT_LIST_T** p_object_list)
{
    WATCHFACE_OBJECT_LIST_T* p_temp_list = PNULL;
    uint8 i = 0;

    if(0 == item_num || PNULL == p_object_list || PNULL == p_object_name)
    {
        TRACE_JSON_PARSER("the param is error");
        return JSON_PARSER_ERROR;
    }
    if(PNULL != *p_object_list)
    {
        TRACE_JSON_PARSER("the param is error");
        return JSON_PARSER_ERROR;
    }
    TRACE_JSON_PARSER("begin create object space, item_num:%d,object_name:%s",item_num, p_object_name);
    //malloc list
    p_temp_list = (WATCHFACE_OBJECT_LIST_T *)SCI_ALLOC_APPZ(sizeof(WATCHFACE_OBJECT_LIST_T));
    if(PNULL == p_temp_list)
    {
        TRACE_JSON_PARSER("malloc space fail");
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_temp_list, 0x00, sizeof(WATCHFACE_OBJECT_LIST_T));
    p_temp_list->item_num = item_num;
    SCI_MEMCPY(p_temp_list->object_name, p_object_name, SCI_STRLEN(p_object_name));

    //malloc
    p_temp_list->p_object_info = (WATCHFACE_OBJECT_COMMON_T *)SCI_ALLOC_APPZ(item_num * sizeof(WATCHFACE_OBJECT_COMMON_T)+1);
    if(PNULL == p_temp_list->p_object_info)
    {
        SCI_FREE(p_temp_list);
        p_temp_list = PNULL;//bug 1958580 CID 589454
        TRACE_JSON_PARSER("malloc space fail");
        return JSON_PARSER_ERROR;
    }
    SCI_MEMSET(p_temp_list->p_object_info, 0x00, item_num * sizeof(WATCHFACE_OBJECT_COMMON_T));
    *p_object_list = p_temp_list;

    TRACE_JSON_PARSER("malloc space successful");
    return JSON_PARSER_SUCCESS;
}

LOCAL JSONPARSER_RESULT_E GetPosition (
                                            cJSON*                          p_root,
                                            WATCHFACE_RATIO_POSITION_T*     p_position
                                            )
{
    cJSON* p_json_object = PNULL;
    cJSON* p_json_pintx = PNULL;
    cJSON* p_json_pinty = PNULL;

    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("p_root is NULL");
       return JSON_PARSER_ERROR;
    }
    if (PNULL == p_position)
    {
       TRACE_JSON_PARSER("p_position is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_json_object = cJSON_GetObjectItem(p_root, "center_position");
    if (PNULL == p_json_object)
    {
       TRACE_JSON_PARSER("p_json_object is NULL");
       return JSON_PARSER_ERROR;
    }

    p_position->ratio_x = (float)cJSON_GetObjectDouble(cJSON_GetObjectItem(p_json_object, "X"));
    TRACE_JSON_PARSER("pointx is %lf",p_position->ratio_x);

    p_position->ratio_y = (float)cJSON_GetObjectDouble(cJSON_GetObjectItem(p_json_object, "Y"));
    TRACE_JSON_PARSER("pointy is %lf",p_position->ratio_y);

    return JSON_PARSER_SUCCESS;
}
LOCAL JSONPARSER_RESULT_E GetRange (
                                            cJSON*                 p_root,
                                            WATCHFACE_RANGE_T*     p_range
                                            )
{
    cJSON* p_json_object = PNULL;
    cJSON* p_json_pintx = PNULL;
    cJSON* p_json_pinty = PNULL;

    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("p_root is NULL");
       return JSON_PARSER_ERROR;
    }
    if (PNULL == p_range)
    {
       TRACE_JSON_PARSER("p_position is NULL");
       return JSON_OUTDATA_NULL;
    }

    p_json_object = cJSON_GetObjectItem(p_root, "range");
    if (PNULL == p_json_object)
    {
       TRACE_JSON_PARSER("p_json_object is NULL");
       return JSON_PARSER_ERROR;
    }

    p_range->min_value = (float)cJSON_GetObjectDouble(cJSON_GetObjectItem(p_json_object, "min_value"));
    TRACE_JSON_PARSER("min_value is %lf",p_range->min_value);

    p_range->max_value = (float)cJSON_GetObjectDouble(cJSON_GetObjectItem(p_json_object, "max_value"));
    TRACE_JSON_PARSER("max_value is %lf",p_range->max_value);

    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : parse common watchface object:解析表盘某个控件的信息
//  Parameter: [In] p_json_string:表盘某个控件的json数据
//             [Out]p_object_info:解析之后的表盘控件的信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL JSONPARSER_RESULT_E  ParseWatchfaceObjectInfo (
                                                    char*                         p_object_name,
                                                    char*                         p_json_string,
                                                    WATCHFACE_OBJECT_LIST_T**     p_object_list
                                                    )
{
    cJSON* p_root = PNULL;
    cJSON* p_json_item_ptr = PNULL;
    cJSON* p_json_object = PNULL;
    WATCHFACE_RATIO_POSITION_T  position = {0};
    WATCHFACE_RANGE_T range = {0};
    cJSON* p_json_array_ptr = PNULL;
    uint16   array_count = 0;
    uint16   i = 0;
    uint16   j = 0;
    uint16   size = 0;
    uint8*   p_type = PNULL;
    uint8*   p_image_name = PNULL;
    uint8*   p_direction = PNULL;
    JSONPARSER_RESULT_E parse_res = JSON_PARSER_MAX;
    WATCHFACE_OBJECT_COMMON_T* p_temp_object = PNULL;
    WATCHFACE_OBJECT_LIST_T *p_temp_list = PNULL;

    if (PNULL == p_json_string ||PNULL == p_object_name)
    {
       TRACE_JSON_PARSER("p_json_string or p_object_name is NULL");
       return JSON_STRING_NULL;
    }
    if (PNULL == p_object_list)
    {
        TRACE_JSON_PARSER("p_object_info is NULL");
        return JSON_OUTDATA_NULL;
    }

    if (PNULL != *p_object_list)
    {
       TRACE_JSON_PARSER("p_object_info is not NULL,free first");
       return JSON_OUTDATA_NULL;
    }

    p_root = cJSON_Parse(p_json_string);

    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("p_root is NULL");
       return JSON_PARSER_ERROR;
    }

    size = (uint16)cJSON_GetArraySize(p_root);
    parse_res = CreateObjectList(p_object_name, size, &p_temp_list);
    if(parse_res != JSON_PARSER_SUCCESS)
    {
        TRACE_JSON_PARSER("CreateObjectList false");
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    p_temp_object = p_temp_list->p_object_info;
    for (j = 0; j < size; j++)
    {
        p_json_item_ptr = cJSON_GetArrayItem(p_root, j);
        if(PNULL == p_json_item_ptr)
        {
            TRACE_JSON_PARSER("p_json_item_ptr is NULL");
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }

        /*center_position*/
        parse_res = GetPosition(p_json_item_ptr,&position);
        if(JSON_PARSER_SUCCESS != parse_res)
        {
            TRACE_JSON_PARSER("Json parser get position fail");
            cJSON_Delete(p_root);
            return JSON_PARSER_ERROR;
        }
        p_temp_object[j].center_position.ratio_x = position.ratio_x;
        p_temp_object[j].center_position.ratio_y = position.ratio_y;

        /*update_interval*/
        p_temp_object[j].update_interval = (uint32)cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "update_interval"));

        /*image_list*/
        p_json_array_ptr = cJSON_GetObjectItem(p_json_item_ptr, "image_list");
        if(PNULL == p_json_array_ptr)
        {
            TRACE_JSON_PARSER("p_json_array_ptr is NULL");
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
        array_count = (uint16)cJSON_GetArraySize(p_json_array_ptr);
        if(MAX_IMAGE_COUNT < array_count)
        {
            TRACE_JSON_PARSER("array_count is out of range!!!! array_count:%d", array_count);
            array_count = MAX_IMAGE_COUNT;
        }
        for(i=0;i<array_count;i++)
        {
            //malloc
            p_temp_object[j].p_image_list[i] = (uint8 *)SCI_ALLOC_APPZ((MAX_NAME_LEN+1) * sizeof(uint8));
            if(PNULL == p_temp_object[j].p_image_list[i])
            {
                TRACE_JSON_PARSER("malloc space fail");
                cJSON_Delete(p_root);
                return JSON_PARSER_ERROR;
            }
            SCI_MEMSET(p_temp_object[j].p_image_list[i], 0x00, (sizeof(uint8) * (MAX_NAME_LEN+1)));
            p_image_name = cJSON_GetObjectString(cJSON_GetArrayItem(p_json_array_ptr,i));
            if(PNULL != p_image_name)
            {
                SCI_MEMSET(p_temp_object[j].p_image_list[i], 0, sizeof(uint8) * (MAX_NAME_LEN+1));
                SCI_MEMCPY(p_temp_object[j].p_image_list[i], p_image_name, MIN(sizeof(uint8) *MAX_NAME_LEN,SCI_STRLEN(p_image_name)));
                TRACE_JSON_PARSER("image index:%d, image_name is %s",i, p_image_name);
            }
        }

        /*image_none*/
        p_image_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "image_none"));
        if(PNULL != p_image_name)
        {
            SCI_MEMSET(p_temp_object[j].image_none, 0, (MAX_NAME_LEN+1));
            SCI_MEMCPY(p_temp_object[j].image_none, p_image_name, MIN(MAX_NAME_LEN,SCI_STRLEN(p_image_name)));
            TRACE_JSON_PARSER("image_none is %s",p_image_name);
        }

        /*image_connector*/
        p_image_name = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "image_connector"));
        if(PNULL != p_image_name)
        {
            SCI_MEMSET(p_temp_object[j].image_connector, 0, (MAX_NAME_LEN+1));
            SCI_MEMCPY(p_temp_object[j].image_connector, p_image_name, MIN(MAX_NAME_LEN,SCI_STRLEN(p_image_name)));
            TRACE_JSON_PARSER("image_connector is %s",p_image_name);
        }

        /*type*/
        p_type = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "type"));
        if(PNULL != p_type)
        {
            SCI_MEMSET(p_temp_object[j].type, 0, (MAX_NAME_LEN+1));
            SCI_MEMCPY(p_temp_object[j].type, p_type, MIN(MAX_NAME_LEN,SCI_STRLEN(p_type)));
            TRACE_JSON_PARSER("type is %s",p_type);
        }

        /*digit_capacity*/
        p_temp_object[j].digit_capacity = (uint32)cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "digit_capacity"));

        /*range*/
        parse_res = GetRange(p_json_item_ptr, &range);
        if(JSON_PARSER_SUCCESS == parse_res)
        {
            TRACE_JSON_PARSER("Json parser get range,range.max_value:%lf, range.min_value:%lf",range.max_value,range.min_value);//bug 1958580 CID 589302
            p_temp_object[j].range.max_value = range.max_value;
            p_temp_object[j].range.min_value = range.min_value;
        }

        /*big_radius*/
        p_temp_object[j].big_radius = (uint32)cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "big_radius"));

        /*little_radius*/
        p_temp_object[j].little_radius = (uint32)cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "little_radius"));

        /*color_rgb*/
        p_temp_object[j].color_rgb = (uint32)cJSON_GetObjectInt(cJSON_GetObjectItem(p_json_item_ptr, "color_rgb"));

        /*direction*/
        p_direction = cJSON_GetObjectString(cJSON_GetObjectItem(p_json_item_ptr, "direction"));
        if(PNULL != p_direction)
        {
            SCI_MEMSET(p_temp_object[j].direction, 0, (MAX_NAME_LEN+1));
            SCI_MEMCPY(p_temp_object[j].direction, p_direction, MIN(MAX_NAME_LEN,SCI_STRLEN(p_direction)));
            TRACE_JSON_PARSER("direction is %s",p_direction);
        }
    }

    cJSON_Delete(p_root);
    *p_object_list= p_temp_list;
    return JSON_PARSER_SUCCESS;

}

/*****************************************************************************/
//  Description : get watchface all key info:解析表盘layout数据，获取所有控件信息
//  Parameter: [In] p_json_string:表盘layout文件json数据
//             [In] p_scale: the image scale get from layout file
//             [Out] p_object_name[]:指针数组，获取所有的表盘控件的名称
//             [Out] p_json_string[]:指针数组，获取对应的所有表盘控件的信息
//             [Return] JSON_PARSER_RESULT_E
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceAllObjectInfo (
                                                    char*                       p_json_string,
                                                    uint8*                      p_object_num,
                                                    double*                     p_scale,
                                                    WATCHFACE_OBJECT_LIST_T*    p_object_list[]
                                                    )
{
    cJSON*  p_root = PNULL;
    cJSON*  child = PNULL;
    uint8   obj_num = 0;
    cJSON*  p_json_object = PNULL;
    char*   p_string = PNULL;
    uint8   i = 0;
    JSONPARSER_RESULT_E res = JSON_PARSER_MAX;

    if (PNULL == p_json_string)
    {
       TRACE_JSON_PARSER("p_json_string is NULL");
       return JSON_STRING_NULL;
    }
    if (PNULL == p_object_num || PNULL == p_scale)
    {
       TRACE_JSON_PARSER("object_num or p_scale is NULL");
       return JSON_STRING_NULL;
    }
    if (PNULL == p_object_list)
    {
        TRACE_JSON_PARSER("p_object_list is not NULL");
        return JSON_OUTDATA_NULL;
    }
    p_root = cJSON_Parse(p_json_string);
    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("p_root is NULL");
       return JSON_PARSER_ERROR;
    }
    child = p_root->child;
    while(PNULL != child)
    {
        if(PNULL != child->string)
        {
            if(0 == strcmp(child->string,"Scale"))
            {
                *p_scale = child->valuedouble;
                TRACE_JSON_PARSER("p_scale is %lf",child->valuedouble);
                child = child->next;
            }
            else
            {
                TRACE_JSON_PARSER("p_object_name[%d] is %s",obj_num,child->string);
                p_string = cJSON_Print(child);
                res = ParseWatchfaceObjectInfo(child->string,p_string,&p_object_list[obj_num]);
                if(PNULL == p_object_list[obj_num])
                {
                    TRACE_JSON_PARSER("p_object_list[%d] is pnull",obj_num);
                    for(i=0;i<=obj_num;i++)
                    {
                        JSONPARSER_FreeObjectList(&p_object_list[i]);
                        p_object_list[i] = PNULL;
                    }
                    SCI_FREE(p_string);
                    cJSON_Delete(p_root);
                    return JSON_PARSER_ERROR;
                }
                if(JSON_PARSER_SUCCESS != res)
                {
                    TRACE_JSON_PARSER("json parser fail");
                    for(i=0;i<=obj_num;i++)
                    {
                        JSONPARSER_FreeObjectList(&p_object_list[i]);
                        p_object_list[i] = PNULL;
                    }
                    SCI_FREE(p_string);
                    cJSON_Delete(p_root);
                    return JSON_PARSER_ERROR;
                }
                if(PNULL == p_object_list[obj_num]->p_object_info)
                {
                    TRACE_JSON_PARSER("object info is null");
                    for(i=0;i<=obj_num;i++)
                    {
                        JSONPARSER_FreeObjectList(&p_object_list[i]);
                        p_object_list[i] = PNULL;
                    }
                    SCI_FREE(p_string);
                    cJSON_Delete(p_root);
                    return JSON_PARSER_ERROR;
                }
                obj_num++;
                child = child->next;
                SCI_FREE(p_string);
            }
        }
        else
        {
            TRACE_JSON_PARSER("p_object_name[%d] is NULL",obj_num);
        }
    }
    *p_object_num = obj_num;
    cJSON_Delete(p_root);
    TRACE_JSON_PARSER("object_num=%d",obj_num);
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : Get Watchface Object Size:解析表盘某个控件的信息
//  Parameter: [In] p_json_string:表盘某个控件的json数据
//                  p_title:      标签，可为PNULL
//             [Out]size:         类型个数
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceObjectSize(char* p_json_string, char* p_title, uint8* p_size)
{
    cJSON*  p_root = PNULL;
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

    if (PNULL != p_title)
    {
        p_json_item_ptr = cJSON_GetArrayItem(p_root, 0);
        if(PNULL == p_json_item_ptr)
        {
            TRACE_JSON_PARSER("p_json_item_ptr is NULL");
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }
        *p_size = (uint8)cJSON_GetArraySize(cJSON_GetObjectItem(p_json_item_ptr, p_title));
    }
    else
    {
        *p_size = (uint8)cJSON_GetArraySize(p_root);
    }
    cJSON_Delete(p_root);

    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : Get Watchface Multinum Info:解析表盘可编辑multinum的信息
//  Parameter: [In] index:表盘索引值
//             [Out]p_multinum_info:   信息结构体
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:JSONPARSER_GetWatchfaceMultinumInfo
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_GetWatchfaceMultinumInfo(uint16 index, WATCHFACE_EDIT_MULTINUM_T* p_multinum_info)
{
    uint8                 i= 0;
    SFS_HANDLE  sfs_handle = 0;
    uint32       file_size = 0;
    uint32    bytes_readed = 0;
    char*              buf = PNULL;
    char      layout_path[MAX_PATH_LEN] = {0};
    wchar  wsrc_file_name[MAX_PATH_LEN] = {0};
    cJSON*        p_root = PNULL;
    cJSON*    p_multinum = PNULL;
    cJSON*        p_item = PNULL;
    char*      data_type = PNULL;
    uint8  multinum_count = 0;
    WATCHFACE_RATIO_POSITION_T  p_position = {0};

    TRACE_JSON_PARSER("enter");

    if (PNULL == p_multinum_info)
    {
        TRACE_JSON_PARSER("p_multinum_info is null");
        return JSON_OUTDATA_NULL;
    }

    WATCHFACE_Get_WatchfaceFile_Path(index, layout_path);
    MMIAPICOM_StrToWstr(layout_path, wsrc_file_name);

    sfs_handle = SFS_CreateFile(wsrc_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read
    if(0 == sfs_handle)
    {
        TRACE_JSON_PARSER("sfs_handle is NULL");
        return JSON_PARSER_ERROR;
    }

    SFS_GetFileSize(sfs_handle, &file_size);
    TRACE_WATCHFACE("sfs_handle = %d, file_size = %d", sfs_handle, file_size);

    buf = SCI_ALLOC_APPZ(file_size);
    if(PNULL == buf)
    {
        TRACE_JSON_PARSER("buf is NULL");
        return JSON_PARSER_ERROR;
    }

    SFS_ReadFile(sfs_handle, buf, file_size,&bytes_readed, PNULL);

    p_root = cJSON_Parse(buf);
    if(0 != sfs_handle)
    {
        SFS_CloseFile(sfs_handle);
        sfs_handle = 0;
    }
    if(PNULL != buf)
    {
        SCI_FREE(buf);
    }
    if (PNULL == p_root)
    {
       TRACE_JSON_PARSER("p_root is NULL");
       return JSON_PARSER_ERROR;
    }

    p_multinum = cJSON_GetObjectItem(p_root, "MultiNum");
    if (PNULL == p_multinum)
    {
        TRACE_JSON_PARSER("MultiNum doesn't exist");
        cJSON_Delete(p_root);
        return JSON_PARSER_EMPTY;
    }

    multinum_count = (uint8)cJSON_GetArraySize(p_multinum);
    p_multinum_info->multinum_count = multinum_count;

    for (i = 0; i < multinum_count; i++)
    {
        p_item = cJSON_GetArrayItem(p_multinum, i);
        if(PNULL == p_item)
        {
            TRACE_JSON_PARSER("p_item is NULL");
            cJSON_Delete(p_root);
            return JSON_PARSER_EMPTY;
        }

        GetPosition(p_item,&p_position);
        p_multinum_info->multinum_info[i].center_position.ratio_x = p_position.ratio_x;
        p_multinum_info->multinum_info[i].center_position.ratio_y = p_position.ratio_y;

        data_type = cJSON_GetObjectString(cJSON_GetObjectItem(p_item, "type"));
        if(PNULL != data_type)
        {
            SCI_MEMSET(p_multinum_info->multinum_info[i].data_type, 0x00, MAX_NAME_LEN);
            SCI_MEMCPY(p_multinum_info->multinum_info[i].data_type, data_type, MAX_NAME_LEN);
            TRACE_JSON_PARSER("data_type is %s",data_type);
        }
    }
    cJSON_Delete(p_root);

    TRACE_JSON_PARSER("exist");
    return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : Set Watchface Multinum Info:设置表盘可编辑multinum的信息
//  Parameter: [In] index:表盘索引值
//              p_multinum_info:   信息结构体
//             [Return] JSON_PARSER_RESULT_E
//  Author: yifan.yang
//  Note:JSONPARSER_SetWatchfaceMultinumInfo
/*****************************************************************************/
PUBLIC JSONPARSER_RESULT_E JSONPARSER_SetWatchfaceMultinumInfo(uint16 index, WATCHFACE_EDIT_MULTINUM_T* p_multinum_info)
{
    uint8                 i= 0;
    SFS_HANDLE  sfs_handle = 0;
    uint32       file_size = 0;
    uint32    bytes_readed = 0;
    uint32    bytes_writen = 0;
    char*            p_buf = PNULL;
    char*         p_string = PNULL;
    cJSON*          p_root = PNULL;
    cJSON*         p_child = PNULL;
    cJSON*          p_item = PNULL;
    cJSON*          p_type = PNULL;
    BOOLEAN        del_ret = FALSE;
    char      layout_path[MAX_PATH_LEN] = {0};
    wchar  wsrc_file_name[MAX_PATH_LEN] = {0};

    TRACE_JSON_PARSER("enter");

    if (PNULL == p_multinum_info)
    {
        TRACE_JSON_PARSER("p_multinum_info is null");
        return JSON_OUTDATA_NULL;
    }

    WATCHFACE_Get_WatchfaceFile_Path(index, layout_path);
    TRACE_JSON_PARSER("layout path is %s", layout_path);
    MMIAPICOM_StrToWstr(layout_path, wsrc_file_name);

    sfs_handle = SFS_CreateFile(wsrc_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read
    if(0 == sfs_handle)
    {
        TRACE_JSON_PARSER("sfs_handle is NULL");
        return JSON_PARSER_ERROR;
    }

    SFS_GetFileSize(sfs_handle, &file_size);
    TRACE_WATCHFACE("sfs_handle = %d, file_size = %d", sfs_handle, file_size);

    p_buf = SCI_ALLOC_APPZ(file_size);
    if(PNULL == p_buf)
    {
        TRACE_JSON_PARSER("buf is NULL");
        return JSON_PARSER_ERROR;
    }

    SFS_ReadFile(sfs_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(sfs_handle);
    sfs_handle = 0;

    p_root = cJSON_Parse(p_buf);
    if (PNULL != p_buf)
    {
        SCI_FREE(p_buf);
    }
    if (PNULL == p_root)
    {
        TRACE_JSON_PARSER("p_root is null");
        return JSON_PARSER_ERROR;
    }
    p_child = cJSON_GetObjectItem(p_root, "MultiNum");
    if (PNULL == p_child)
    {
        TRACE_JSON_PARSER("p_child is null");
        cJSON_Delete(p_root);
        return JSON_PARSER_ERROR;
    }
    for (i = 0; i < p_multinum_info->multinum_count; i++)
    {
        p_item = cJSON_GetArrayItem(p_child, i);
        if (PNULL == p_item)
        {
            TRACE_JSON_PARSER("p_item is null");
            cJSON_Delete(p_root);
            return JSON_PARSER_ERROR;
        }
        p_type = cJSON_GetObjectItem(p_item, "type");
        if (PNULL == p_type)
        {
            TRACE_JSON_PARSER("p_type is null");
            cJSON_Delete(p_root);
            return JSON_PARSER_ERROR;
        }
        else if (PNULL != p_type->valuestring)
        {
            SCI_FREE(p_type->valuestring);
            p_type->valuestring = SCI_ALLOC_APPZ(MAX_NAME_LEN);
            SCI_MEMSET(p_type->valuestring, 0x00, MAX_NAME_LEN);
            SCI_MEMCPY(p_type->valuestring, p_multinum_info->multinum_info[i].data_type, MAX_NAME_LEN);
        }
    }
    p_string = cJSON_Print(p_root);

    del_ret = MMIAPIFMM_DeleteFileSyn(wsrc_file_name,MMIAPICOM_Wstrlen(wsrc_file_name));
    if (!del_ret)
    {
        TRACE_JSON_PARSER("[adapter]delete config failed");
        if (PNULL != p_root)
        {
            SCI_FREE(p_string);
            cJSON_Delete(p_root);
        }
        return JSON_OUTDATA_NULL;
    }

    sfs_handle = SFS_CreateFile (wsrc_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_JSON_PARSER("[adapter]handle = %d",sfs_handle);
    if (0 == sfs_handle)
    {
        TRACE_JSON_PARSER("[adapter]handle = 0");
        if (PNULL != p_root)
        {
            SCI_FREE(p_string);
            cJSON_Delete(p_root);
        }
        return JSON_OUTDATA_NULL;
    }

    SFS_WriteFile(sfs_handle,p_string,SCI_STRLEN(p_string),&bytes_writen,PNULL);
    SFS_CloseFile(sfs_handle);
    sfs_handle = 0;
    SCI_FREE(p_string);
    if (PNULL != p_root)
    {
        cJSON_Delete(p_root);
    }
    TRACE_JSON_PARSER("exist");
    return JSON_PARSER_SUCCESS;
}
