/*****************************************************************************
** File Name:       ctrlwfdigitaldate.c                                      *
** Author:          qi.zhang                                                 *
** Date:            11/1/2021                                                *
** Copyright:                                                                *
** Description:    This file is used to define digital date control          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DIGITALDATE          NAME                  DESCRIPTION                    *
** 11/1/2021            qi.zhang              Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "img_dec_interface.h"
#include "mmi_string.h"
#include "ctrlwfdigitaldate.h"
#include "ctrlwf_export.h"
#include "ctrlwfcommon.h"
#include "mmk_timer.h"
#include "dal_time.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define DIGITAL_DATE_TIMER_OUT          (60000) //date的刷新时间

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create watch face digital date data
//  Parameter: [In] win_id
//             [In] p_image_list[]: 数字图片资源路径
//             [In] p_month_image:汉字“月”或者连接符图片资源路径
//             [In] scale:图片缩放比例
//             [In] type:digital date传入type
//             [Out] None
//             [Return] WATCHFACE_DIGITAL_DATE_OBJ_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DIGITAL_DATE_OBJ_T* CreateDigitalDateData(
                                        MMI_WIN_ID_T                   win_id,
                                        char*                          p_image_list[],
                                        char*                          p_image_connector,
                                        float                          scale,
                                        WATCHFACE_ALL_TYPE_E           type
                                        );

/*****************************************************************************/
//  Description : creat digital date ctrl id
//  Parameter: [In] p_date_info
//             [In] ctrl_id
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalDateItemsCtrlId(WATCHFACE_DIGITAL_DATE_OBJ_T* p_date_info, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : set digital date position
//  Parameter: [In] p_date_info
//             [In] center_position
//             [Out] p_layer
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalDateItemsPosAndLayerSize(WATCHFACE_DIGITAL_DATE_OBJ_T* p_date_info, GUI_POINT_T center_point, GUIIMG_INFO_T* p_layer);
/*****************************************************************************/
//  Description : display month and day
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonthDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display 年月日
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYearMonthDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display year
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYear(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display month
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonth(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display day
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display 年月
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYearMonth(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[]);
/*****************************************************************************/
//  Description : display digitaldate
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalDate(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create);
/*****************************************************************************/
//  Description : update date
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDigitalDate(uint8 timer_id, uint32 param);
/*****************************************************************************/
//  Description : create watch face digital date data
//  Parameter: [In] win_id
//             [In] p_image_list[]: 数字图片资源路径
//             [In] p_month_image:汉字“月”或者连接符图片资源路径
//             [In] scale:图片缩放比例
//             [In] type:digital date传入type
//             [Out] None
//             [Return] WATCHFACE_DIGITAL_DATE_OBJ_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DIGITAL_DATE_OBJ_T* CreateDigitalDateData(
                                        MMI_WIN_ID_T                   win_id,
                                        char*                          p_image_list[],
                                        char*                          p_image_connector,
                                        float                          scale,
                                        WATCHFACE_ALL_TYPE_E           type
                                        )
{
    WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info = PNULL;
    wchar                  num_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    wchar                  connector_wster[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint16                 wstr_len = 0;
    uint8                  i = 0;

    if((PNULL == p_image_list)||(PNULL == p_image_connector))
    {
        CTRL_WF_TRACE("full_path_ptr = pnull!");
        return PNULL;
    }
    //alloc space
    p_date_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_DIGITAL_DATE_OBJ_T));
    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("The p_date_info is PNULL!");
        return PNULL;
    }
    SCI_MEMSET(p_date_info, 0, sizeof(WATCHFACE_DIGITAL_DATE_OBJ_T));

    //store base info
    p_date_info->win_id = win_id;
    p_date_info->scale = scale;
    p_date_info->type = type;

    MMIAPICOM_StrToWstr((uint8 *)p_image_connector, connector_wster);
    wstr_len = MMIAPICOM_Wstrlen(connector_wster);
    MMI_WSTRNCPY(p_date_info->img_connector, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, (wchar*)connector_wster, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], num_wstr);
            wstr_len = MMIAPICOM_Wstrlen(num_wstr);
            MMI_WSTRNCPY(p_date_info->img_number[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, num_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
        }
        else
        {
            CTRL_WF_TRACE("p_image_list[%d] is PNULL!", i);
        }
    }
    return p_date_info;
}
/*****************************************************************************/
//  Description : creat digital date ctrl id
//  Parameter: [In] p_date_info
//             [In] ctrl_id
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalDateItemsCtrlId(WATCHFACE_DIGITAL_DATE_OBJ_T* p_date_info, MMI_CTRL_ID_T ctrl_id)
{
    uint8   i = 0;
    if(PNULL == p_date_info)
    {
        CTRL_WF_TRACE("SetDigitalDateItemsCtrlId error.");
        return;
    }

    CTRL_WF_TRACE("type = %d", p_date_info->type);
    switch (p_date_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_DATE_MD:
        {
           for(i = DIGITAL_DATE_MONTH_HIGH; i < DIGITAL_DATE_ITEM_MAX; i ++)
            {
                p_date_info->self_info[i].ctrl_id = ctrl_id;
                ctrl_id ++;
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YMD:
        {
            for(i = DIGITAL_DATE_YEAR_1; i < DIGITAL_DATE_ITEM_MAX; i ++)
            {
                p_date_info->self_info[i].ctrl_id = ctrl_id;
                ctrl_id ++;
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_MONTH:
        {
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].ctrl_id = ctrl_id;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].ctrl_id = ctrl_id + 1;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YEAR:
        {
            for(i = DIGITAL_DATE_YEAR_1; i < DIGITAL_DATE_YEAR_IMAGE; i ++)
            {
                p_date_info->self_info[i].ctrl_id = ctrl_id;
                ctrl_id ++;
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YM:
        {
            for(i = DIGITAL_DATE_YEAR_1; i < DIGITAL_DATE_MONTH_IMAGE; i ++)
            {
                p_date_info->self_info[i].ctrl_id = ctrl_id;
                ctrl_id ++;
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_DAY:
        {
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].ctrl_id = ctrl_id;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].ctrl_id = ctrl_id + 1;
            break;
        }
        default:
        {
            CTRL_WF_TRACE("type is error");
            break;
        }
    }
}

/*****************************************************************************/
//  Description : creat digital date position
//  Parameter: [In] p_date_info
//             [In] center_position
//             [Out] p_layer
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalDateItemsPosAndLayerSize(WATCHFACE_DIGITAL_DATE_OBJ_T* p_date_info, GUI_POINT_T center_point, GUIIMG_INFO_T* p_layer)
{
    GUIIMG_INFO_T       num_img_info = {0};
    GUIIMG_INFO_T       connector_img_info = {0};
    uint16              num_width = 0;
    uint16              num_height = 0;
    uint16              connector_width = 0;
    uint8               i = 0;

    if(PNULL == p_date_info)
    {
        CTRL_WF_TRACE("p_date_info = pnull.");
        return;
    }

    if(PNULL == p_layer)
    {
        CTRL_WF_TRACE("p_layer = pnull.");
        return;
    }

    //get image width and height
    CTRLWF_Common_GetImgInfoByPath(p_date_info->img_number[0], &num_img_info);
    CTRLWF_Common_GetImgInfoByPath(p_date_info->img_connector, &connector_img_info);
    CTRL_WF_TRACE("scale = %f", p_date_info->scale);
    num_width = (uint16)(num_img_info.image_width * p_date_info->scale);
    num_height = (uint16)(num_img_info.image_height * p_date_info->scale);
    connector_width = (uint16)(connector_img_info.image_width * p_date_info->scale);

    CTRL_WF_TRACE("num_width = %d, connector_width = %d, center_point = [%d, %d]", num_width, connector_width, center_point.x, center_point.y);

    CTRL_WF_TRACE("type = %d", p_date_info->type);
    switch (p_date_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_DATE_MD:
        {
            for(i = DIGITAL_DATE_MONTH_HIGH; i < DIGITAL_DATE_ITEM_MAX; i ++)
            {
                p_date_info->self_info[i].center_point.y = center_point.y;
            }
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x = center_point.x - connector_width/2 - num_width - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.left =   p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.right =  p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.top =    center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x = center_point.x - connector_width/2 - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.left =    p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.right =  p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x = center_point.x;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x - connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x + connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.top = center_point.y - (uint16)(connector_img_info.image_height * p_date_info->scale)/2; //need to check what it is.
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.bottom = center_point.y + (uint16)(connector_img_info.image_height * p_date_info->scale)/2;

            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x = center_point.x + connector_width/2 + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x = center_point.x + connector_width/2 + num_width + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.bottom = center_point.y + num_height/2;

            p_layer->image_width = num_width * 4 + connector_width + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YMD:
        {
            for(i = DIGITAL_DATE_YEAR_1; i < DIGITAL_DATE_ITEM_MAX; i ++)
            {
                p_date_info->self_info[i].center_point.y = center_point.y;
            }
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x = center_point.x - connector_width - num_width/2*7;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x = center_point.x - connector_width - num_width/2*5;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x = center_point.x - connector_width - num_width/2*3;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x = center_point.x - connector_width - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x = center_point.x - connector_width/2;// is this cord correct?
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x - connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x + connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.top = center_point.y - (uint16)(connector_img_info.image_height * p_date_info->scale)/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.bottom = center_point.y + (uint16)(connector_img_info.image_height * p_date_info->scale)/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x = center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x = center_point.x + num_width/2*3;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x = center_point.x + num_width*2 + connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x - connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].center_point.x + connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.top = center_point.y - (uint16)(connector_img_info.image_height * p_date_info->scale)/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect.bottom = center_point.y + (uint16)(connector_img_info.image_height * p_date_info->scale)/2;

            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x = center_point.x + connector_width + num_width/2*5;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x = center_point.x + connector_width + num_width/2*7;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.bottom = center_point.y + num_height/2;

            p_layer->image_width = num_width * 8 + connector_width * 2 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_MONTH:
        {
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x = center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.y = center_point.y;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x = center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.y = center_point.y;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.bottom = center_point.y + num_height/2;

            p_layer->image_width = num_width * 2 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YEAR:
        {
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x = center_point.x - num_width/2*3;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.y = center_point.y;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.bottom = center_point.y + num_height/2;

            for(i = DIGITAL_DATE_YEAR_2; i < DIGITAL_DATE_YEAR_IMAGE; i ++)
            {
                p_date_info->self_info[i].center_point.x = p_date_info->self_info[i - 1].center_point.x + num_width;
                p_date_info->self_info[i].center_point.y = center_point.y;
                p_date_info->self_info[i].rect.left = p_date_info->self_info[i].center_point.x - num_width/2;
                p_date_info->self_info[i].rect.right = p_date_info->self_info[i].center_point.x + num_width/2;
                p_date_info->self_info[i].rect.top = center_point.y - num_height/2;
                p_date_info->self_info[i].rect.bottom = center_point.y + num_height/2;

            }

            p_layer->image_width = num_width * 4 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YM:
        {
            for(i = DIGITAL_DATE_YEAR_1; i < DIGITAL_DATE_MONTH_IMAGE; i ++)
            {
                p_date_info->self_info[i].center_point.y = center_point.y;
            }
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x = center_point.x - num_width*3;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_1].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_1].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x = center_point.x - num_width*2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_2].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_2].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x = center_point.x - num_width;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_3].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_3].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x = center_point.x;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_4].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_4].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x = center_point.x + connector_width/2 + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.left = p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x - connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.right = p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].center_point.x + connector_width/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.top = center_point.y - (uint16)(connector_img_info.image_height * p_date_info->scale)/2;
            p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect.bottom = center_point.y + (uint16)(connector_img_info.image_height * p_date_info->scale)/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x = center_point.x + num_width + connector_width;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x = center_point.x + num_width*2 + connector_width;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_MONTH_LOW].rect.bottom = center_point.y + num_height/2;

            p_layer->image_width = num_width * 6 + connector_width + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_DAY:
        {
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x = center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.y = center_point.y;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_HIGH].rect.bottom = center_point.y + num_height/2;

            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x = center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.y = center_point.y;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.left = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x - num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.right = p_date_info->self_info[DIGITAL_DATE_DAY_LOW].center_point.x + num_width/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.top = center_point.y - num_height/2;
            p_date_info->self_info[DIGITAL_DATE_DAY_LOW].rect.bottom = center_point.y + num_height/2;

            p_layer->image_width = num_width * 2 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        default:
        {
            CTRL_WF_TRACE("type is error");
            break;
        }
    }
}
/*****************************************************************************/
//  Description : display month and day
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonthDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayMonthDay is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        CTRLWF_Common_CreateImg(p_date_info->win_id,
                                p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].ctrl_id,
                                &p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect,
                                p_date_info->img_connector,
                                p_date_info->scale,
                                &(p_date_info->layer)
                                );

        for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
        {
            if(index != DIGITAL_DATE_MONTH_IMAGE)
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //create date item
                CTRLWF_Common_CreateImg(p_date_info->win_id,
                                        p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale,
                                        &(p_date_info->layer)
                                        );
            }
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            CTRLWF_Common_UpdateImg(p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].ctrl_id,
                                    &p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect,
                                    p_date_info->img_connector,
                                    p_date_info->scale, &(p_date_info->layer));
            for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
            {
                if (index != DIGITAL_DATE_MONTH_IMAGE)
                {
                    //record current date value
                    p_date_info->self_info[index].time_value = item_value[index];
                    //update date item
                    CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                            &p_date_info->self_info[index].rect,
                                            p_img_number[p_date_info->self_info[index].time_value],
                                            p_date_info->scale, &(p_date_info->layer)
                                            );
                }
            }
        }
    }
}
/*****************************************************************************/
//  Description : display 年月日
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYearMonthDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayYearMonthDay is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_ITEM_MAX; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        CTRLWF_Common_CreateImg(p_date_info->win_id,
                                p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].ctrl_id,
                                &p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect,
                                p_date_info->img_connector,
                                p_date_info->scale,
                                &(p_date_info->layer)
                                );
        CTRLWF_Common_CreateImg(p_date_info->win_id,
                                p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].ctrl_id,
                                &p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect,
                                p_date_info->img_connector,
                                p_date_info->scale,
                                &(p_date_info->layer)
                                );
        for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_ITEM_MAX; index ++)
        {
            if((index != DIGITAL_DATE_YEAR_IMAGE)&&(index != DIGITAL_DATE_MONTH_IMAGE))
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //create date item
                CTRLWF_Common_CreateImg(p_date_info->win_id,
                                        p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale,
                                        &(p_date_info->layer)
                                        );
            }
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            CTRLWF_Common_UpdateImg(p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].ctrl_id,
                                    &p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect,
                                    p_date_info->img_connector,
                                    p_date_info->scale, &(p_date_info->layer));
            CTRLWF_Common_UpdateImg(p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].ctrl_id,
                                    &p_date_info->self_info[DIGITAL_DATE_MONTH_IMAGE].rect,
                                    p_date_info->img_connector,
                                    p_date_info->scale, &(p_date_info->layer));

            for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_ITEM_MAX; index ++)
            {
                if((index != DIGITAL_DATE_YEAR_IMAGE)&&(index != DIGITAL_DATE_MONTH_IMAGE))
                {
                    //record current date value
                    p_date_info->self_info[index].time_value = item_value[index];
                    //update date item
                    CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                            &p_date_info->self_info[index].rect,
                                            p_img_number[p_date_info->self_info[index].time_value],
                                            p_date_info->scale, &(p_date_info->layer)
                                            );
                }
            }
        }
    }
}
/*****************************************************************************/
//  Description : display year
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYear(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayYear is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_YEAR_IMAGE; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_YEAR_IMAGE; index ++)
        {
            p_date_info->self_info[index].time_value = item_value[index];
            //create date item
            CTRLWF_Common_CreateImg(p_date_info->win_id,
                                    p_date_info->self_info[index].ctrl_id,
                                    &p_date_info->self_info[index].rect,
                                    p_img_number[p_date_info->self_info[index].time_value],
                                    p_date_info->scale,
                                    &(p_date_info->layer)
                                    );
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_YEAR_IMAGE; index ++)
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //update date item
                CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale, &(p_date_info->layer)
                                        );
            }
        }
    }
}
/*****************************************************************************/
//  Description : display month
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonth(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayMonth is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
        {
            //record current date value
            p_date_info->self_info[index].time_value = item_value[index];
            //create date item
            CTRLWF_Common_CreateImg(p_date_info->win_id,
                                    p_date_info->self_info[index].ctrl_id,
                                    &p_date_info->self_info[index].rect,
                                    p_img_number[p_date_info->self_info[index].time_value],
                                    p_date_info->scale,
                                    &(p_date_info->layer)
                                    );
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //update date item
                CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale, &(p_date_info->layer)
                                        );
            }
        }
    }
 }
/*****************************************************************************/
//  Description : display day
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDay(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayDay is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_DAY_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        for(index = DIGITAL_DATE_DAY_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
        {
            //record current date value
            p_date_info->self_info[index].time_value = item_value[index];
            //create date item
            CTRLWF_Common_CreateImg(p_date_info->win_id,
                                    p_date_info->self_info[index].ctrl_id,
                                    &p_date_info->self_info[index].rect,
                                    p_img_number[p_date_info->self_info[index].time_value],
                                    p_date_info->scale,
                                    &(p_date_info->layer)
                                    );
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            for(index = DIGITAL_DATE_DAY_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //update date item
                CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale, &(p_date_info->layer)
                                        );
            }
        }
    }
}
/*****************************************************************************/
//  Description : display 年月
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [In] item_value:每张图片对应的的数字
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayYearMonth(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create, uint8  item_value[])
{
    wchar*  p_img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM] = {PNULL};
    uint8   index = 0;
    uint8   i = 0;
    BOOLEAN is_need_update = FALSE;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("DisplayYearMonth is error");
        return;
    }

    for(i = 0; i < WATCHFACE_DIGITAL_DATE_LIST_NUM; i++)
    {
        p_img_number[i] = p_date_info->img_number[i];
    }

    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
    {
        if (item_value[index] != p_date_info->self_info[index].time_value)
        {
            is_need_update = TRUE;
            break;
        }
    }

    if(TRUE == is_create)
    {
        CTRLWF_Common_CreateImg(p_date_info->win_id,
                                p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].ctrl_id,
                                &p_date_info->self_info[DIGITAL_DATE_YEAR_IMAGE].rect,
                                p_date_info->img_connector,
                                p_date_info->scale,
                                &(p_date_info->layer)
                                );
        for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
        {
            if(index != DIGITAL_DATE_YEAR_IMAGE)
            {
                //record current date value
                p_date_info->self_info[index].time_value = item_value[index];
                //create date item
                CTRLWF_Common_CreateImg(p_date_info->win_id,
                                        p_date_info->self_info[index].ctrl_id,
                                        &p_date_info->self_info[index].rect,
                                        p_img_number[p_date_info->self_info[index].time_value],
                                        p_date_info->scale,
                                        &(p_date_info->layer)
                                        );
            }
        }
    }
    else
    {
        if (TRUE == is_need_update)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
#endif
            for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
            {
                if (index != DIGITAL_DATE_YEAR_IMAGE)
                {
                    //record current date value
                    p_date_info->self_info[index].time_value = item_value[index];
                    //update date item
                    CTRLWF_Common_UpdateImg(p_date_info->self_info[index].ctrl_id,
                                            &p_date_info->self_info[index].rect,
                                            p_img_number[p_date_info->self_info[index].time_value],
                                            p_date_info->scale, &(p_date_info->layer)
                                            );
                }
            }
        }
    }
}
/*****************************************************************************/
//  Description : display digitaldate
//  Parameter: [In] win_id
//             [In] p_date_info:有关date的所有信息
//             [In] is_create:是否第一次create
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalDate(WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info, BOOLEAN  is_create)
{
    SCI_DATE_T              sys_time = {0};
    uint8                   item_value[DIGITAL_DATE_ITEM_MAX] = {0};
    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("p_date_info = pnull!");
        return;
    }
    TM_GetSysDate(&sys_time);
    item_value[DIGITAL_DATE_YEAR_1] = sys_time.year / 1000;
    item_value[DIGITAL_DATE_YEAR_2] = sys_time.year % 1000/ 100;
    item_value[DIGITAL_DATE_YEAR_3] = sys_time.year % 1000 % 100 / 10;
    item_value[DIGITAL_DATE_YEAR_4] = sys_time.year % 1000 % 100 % 10;
    item_value[DIGITAL_DATE_MONTH_HIGH] = sys_time.mon / 10;
    item_value[DIGITAL_DATE_MONTH_LOW] = sys_time.mon % 10;
    item_value[DIGITAL_DATE_DAY_HIGH] = sys_time.mday / 10;
    item_value[DIGITAL_DATE_DAY_LOW] = sys_time.mday % 10;
    CTRL_WF_TRACE("display digitaldate type = %d", p_date_info->type);
    switch (p_date_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_DATE_MD:
        {
            DisplayMonthDay(p_date_info, is_create, item_value);
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YEAR:
        {
            DisplayYear(p_date_info, is_create, item_value);
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_MONTH:
        {
            DisplayMonth(p_date_info, is_create, item_value);
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_DAY:
        {
            DisplayDay(p_date_info, is_create, item_value);
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YMD:
        {
            DisplayYearMonthDay(p_date_info, is_create, item_value);
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_DATE_YM:
        {
            DisplayYearMonth(p_date_info, is_create, item_value);
            break;
        }
        default:
        {
            CTRL_WF_TRACE("type is error");
            break;
        }
    }

}
/*****************************************************************************/
//  Description : update date
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDigitalDate(uint8 timer_id, uint32 param)
{
    WATCHFACE_DIGITAL_DATE_OBJ_T*   p_date_info = (WATCHFACE_DIGITAL_DATE_OBJ_T*) param;

    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("param = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_date_info->win_id))
    {
        if (0 != p_date_info->timer_id)
        {
            if(timer_id == p_date_info->timer_id)
            {
                //update time
                DisplayDigitalDate(p_date_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_date_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif

            }
        }
    }
}
/*****************************************************************************/
//  Description : create watch face digital date ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 数字图片资源路径
//             [In] p_month_image:汉字“月”或者连接符图片资源路径
//             [In] scale:图片缩放比例
//             [In] type:digital date传入type
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/

PUBLIC MMI_HANDLE_T CTRLWF_DigitalDate_Create (
                                        MMI_WIN_ID_T                   win_id,
                                        MMI_CTRL_ID_T                  ctrl_id,
                                        GUI_POINT_T                    center_point,
                                        char*                          p_image_list[],
                                        char*                          p_image_connector,
                                        float                          scale,
                                        WATCHFACE_ALL_TYPE_E           type
                                        )
{
    MMI_HANDLE_T        handle = 0;
    WATCHFACE_DIGITAL_DATE_OBJ_T*  p_date_info = PNULL;
    GUIIMG_INFO_T       layer_size = {0};

    CTRL_WF_TRACE("CTRLWF_DigitalDate_Create");
    if((PNULL == p_image_list)||(PNULL == p_image_connector))
    {
        CTRL_WF_TRACE("full_path_ptr = pnull!");
        return handle;
    }

    //create data
    p_date_info = CreateDigitalDateData(win_id, p_image_list, p_image_connector, scale, type);
    if (PNULL == p_date_info)
    {
        CTRL_WF_TRACE("p_date_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_date_info;
        p_date_info->handle = handle;
    }

    //set items ctrl id
    SetDigitalDateItemsCtrlId(p_date_info, ctrl_id);

    //set display rect
    SetDigitalDateItemsPosAndLayerSize(p_date_info, center_point, &layer_size);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_date_info->layer), win_id, layer_size.image_width, layer_size.image_height);
    if (p_date_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_date_info, 0, sizeof(WATCHFACE_DIGITAL_DATE_OBJ_T));
        SCI_FREE(p_date_info);
        p_date_info = PNULL;
        return 0;
    }
    UILAYER_SetLayerPosition(&(p_date_info->layer),
                            (center_point.x - (layer_size.image_width) / 2),
                            (center_point.y - (layer_size.image_height) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_date_info->layer));
#endif
    //display digital date
    DisplayDigitalDate(p_date_info, TRUE);

    if (0 != p_date_info->timer_id)
    {
        MMK_StopTimer(p_date_info->timer_id);
        p_date_info->timer_id = 0;
    }
    p_date_info->timer_id = MMK_CreateTimerCallback(DIGITAL_DATE_TIMER_OUT, UpdateDigitalDate, (WATCHFACE_DIGITAL_DATE_OBJ_T*)p_date_info, TRUE);

    return handle;
}
/*****************************************************************************/
//  Description : destory watch face digital date ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalDate_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN   result = FALSE;
    WATCHFACE_DIGITAL_DATE_OBJ_T*   p_date_info = PNULL;
    uint8     index = 0;

    CTRL_WF_TRACE("destory enter, handle = %d", handle);
    if (0 != handle)
    {
        p_date_info = (WATCHFACE_DIGITAL_DATE_OBJ_T*)handle;
        if(PNULL == p_date_info)
        {
            CTRL_WF_TRACE("p_date_info = pnull!");
            return result;
        }
        if (handle == p_date_info->handle)
        {
            CTRL_WF_TRACE("type = %d", p_date_info->type);
            //stop timer
            MMK_StopTimer(p_date_info->timer_id);
            p_date_info->timer_id = 0;
            //destory ctrl
            switch (p_date_info->type)
            {
                case WATCHFACE_TYPE_DIGITAL_DATE_MD:
                {
                    for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }
                case WATCHFACE_TYPE_DIGITAL_DATE_YMD:
                {
                    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_ITEM_MAX; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }
                case WATCHFACE_TYPE_DIGITAL_DATE_MONTH:
                {
                    for(index = DIGITAL_DATE_MONTH_HIGH; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }case WATCHFACE_TYPE_DIGITAL_DATE_YEAR:
                {
                    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_YEAR_IMAGE; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }
                case WATCHFACE_TYPE_DIGITAL_DATE_YM:
                {
                    for(index = DIGITAL_DATE_YEAR_1; index < DIGITAL_DATE_MONTH_IMAGE; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }
                case WATCHFACE_TYPE_DIGITAL_DATE_DAY:
                {
                    for(index = DIGITAL_DATE_DAY_HIGH; index < DIGITAL_DATE_ITEM_MAX; index ++)
                    {
                        MMK_DestroyControl(p_date_info->self_info[index].ctrl_id);
                    }
                    break;
                }
                default:
                {
                    CTRL_WF_TRACE("type is error");
                    break;
                }
            }
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_date_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_date_info->layer));
#endif
            //free space
            SCI_MEMSET(p_date_info, 0, sizeof(WATCHFACE_DIGITAL_DATE_OBJ_T));
            SCI_FREE(p_date_info);
            p_date_info = PNULL;
            result = TRUE;
        }
    }

    CTRL_WF_TRACE("destory exit, result = %d", result);
    return result;
}
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalDate_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_DIGITAL_DATE_OBJ_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DIGITAL_DATE_OBJ_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return FALSE;
        }
        if (handle == p_info->handle)
        {
            if(0 != p_info->timer_id)
            {
                MMK_StopTimer(p_info->timer_id);
                p_info->timer_id = 0;
                result = TRUE;
            }
        }
    }
    CTRL_WF_TRACE("stop timer exit, result = %d", result);
    return result;

}
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_DigitalDate_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_DIGITAL_DATE_OBJ_T*   p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DIGITAL_DATE_OBJ_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }

        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                DisplayDigitalDate(p_info, FALSE);
                if (0 == p_info->timer_id)
                {
                    p_info->timer_id = MMK_CreateTimerCallback(DIGITAL_DATE_TIMER_OUT, UpdateDigitalDate, (WATCHFACE_DIGITAL_DATE_OBJ_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

