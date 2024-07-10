/*****************************************************************************
** File Name:       ctrlwf_digitaltime.c                                     *
** Author:          yuming.yang                                              *
** Date:            08/13/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/13/2021       yuming.yang               Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "img_dec_interface.h"
#include "mmi_string.h"
#include "ctrlwfdigitaltime.h"
#include "ctrlwf_export.h"
#include "ctrlwfcommon.h"
#include "mmk_timer.h"
#include "dal_time.h"



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/





/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set ctrl id for items
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalItemsCtrlId(WATCHFACE_DIGITAL_TIME_OBJ_T* p_info, MMI_CTRL_ID_T ctrl_id, GUIIMG_INFO_T  colon_info);

/*****************************************************************************/
//  Description : calculate every items display position
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalItemsPosAndLayerSize(WATCHFACE_DIGITAL_TIME_OBJ_T*   p_info,
                                                    GUI_POINT_T                     center_point,
                                                    GUIIMG_INFO_T                   num_info,
                                                    GUIIMG_INFO_T                   colon_info,
                                                    GUIIMG_INFO_T*                  p_layer
                                                    );

/*****************************************************************************/
//  Description : display digital clock
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalTime(MMI_WIN_ID_T                    win_id,
                                    wchar*                          p_num_path[],
                                    wchar*                          p_conn_path,
                                    WATCHFACE_DIGITAL_TIME_OBJ_T*   p_info,
                                    BOOLEAN                         is_create
                                    );

/*****************************************************************************/
//  Description : update by timer
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDigitalTime(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DIGITAL_TIME_OBJ_T* CreateDigitalTimeData(
                                                    MMI_WIN_ID_T                    win_id,
                                                    uint32                          update_interval,
                                                    char*                           p_image_list[],
                                                    char*                           p_image_connector,
                                                    float                           scale,
                                                    WATCHFACE_ALL_TYPE_E            type
                                                    );




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : set ctrl id for items
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalItemsCtrlId(WATCHFACE_DIGITAL_TIME_OBJ_T* p_info, MMI_CTRL_ID_T ctrl_id, GUIIMG_INFO_T  colon_info)
{
    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("SetDigitalItemsCtrlId error.");
        return;
    }

    CTRL_WF_TRACE("type = %d", p_info->type);
    switch (p_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN:
        {
            if(0 != colon_info.image_width)
            {
                p_info->self_info[DIGITAL_TIME_COLON].ctrl_id = ctrl_id;
                p_info->self_info[DIGITAL_TIME_HOUR_HIGH].ctrl_id = ctrl_id + 1;
                p_info->self_info[DIGITAL_TIME_HOUR_LOW].ctrl_id = ctrl_id + 2;
                p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].ctrl_id = ctrl_id + 3;
                p_info->self_info[DIGITAL_TIME_MINUTE_LOW].ctrl_id = ctrl_id + 4;
            }
            else
            {
                p_info->self_info[DIGITAL_TIME_HOUR_HIGH].ctrl_id = ctrl_id;
                p_info->self_info[DIGITAL_TIME_HOUR_LOW].ctrl_id = ctrl_id + 1;
                p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].ctrl_id = ctrl_id + 2;
                p_info->self_info[DIGITAL_TIME_MINUTE_LOW].ctrl_id = ctrl_id + 3;
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_HOUR:
        {
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].ctrl_id = ctrl_id;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].ctrl_id = ctrl_id + 1;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_MIN:
        {
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].ctrl_id = ctrl_id ;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].ctrl_id = ctrl_id + 1;
            break;
        }
        default:
            break;
        }
}


/*****************************************************************************/
//  Description : calculate every items display position
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void SetDigitalItemsPosAndLayerSize(WATCHFACE_DIGITAL_TIME_OBJ_T*   p_info,
                                                    GUI_POINT_T                     center_point,
                                                    GUIIMG_INFO_T                   num_info,
                                                    GUIIMG_INFO_T                   colon_info,
                                                    GUIIMG_INFO_T*                  p_layer
                                                    )
{
    uint16          num_width = 0;
    uint16          num_height = 0;
    uint16          colon_width = 0;

    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull.");
        return;
    }

    if(PNULL == p_layer)
    {
        CTRL_WF_TRACE("p_layer = pnull.");
        return;
    }

    num_width = (uint16)(num_info.image_width * p_info->scale);
    num_height = (uint16)(num_info.image_height * p_info->scale);
    colon_width = (uint16)(colon_info.image_width * p_info->scale);

    CTRL_WF_TRACE("num_width = %d, colon_width = %d, center_point = [%d, %d]", num_width, colon_width, center_point.x, center_point.y);

    CTRL_WF_TRACE("type = %d", p_info->type);
    switch (p_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN:
        {
            p_info->self_info[DIGITAL_TIME_COLON].center_point.x = center_point.x;
            p_info->self_info[DIGITAL_TIME_COLON].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_COLON].rect.left = p_info->self_info[DIGITAL_TIME_COLON].center_point.x - colon_width/2;
            p_info->self_info[DIGITAL_TIME_COLON].rect.right = p_info->self_info[DIGITAL_TIME_COLON].center_point.x + colon_width/2;
            p_info->self_info[DIGITAL_TIME_COLON].rect.top = p_info->self_info[DIGITAL_TIME_COLON].center_point.y - (uint16)(colon_info.image_height* p_info->scale)/2;
            p_info->self_info[DIGITAL_TIME_COLON].rect.bottom = p_info->self_info[DIGITAL_TIME_COLON].center_point.y + (uint16)(colon_info.image_height* p_info->scale)/2;

            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x = center_point.x - colon_width/2 - num_width/2 - num_width;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.left =   p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.right =  p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.top =    p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.bottom=  p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y + num_height/2;

            p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x = center_point.x - colon_width/2 - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.left =   p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.right =  p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.top =    p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.bottom=  p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y + num_height/2;

            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x = center_point.x + colon_width/2 + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.left =   p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.right =  p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.top =    p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.bottom=  p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y + num_height/2;

            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x = center_point.x + colon_width/2 + num_width/2 + num_width;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.left =   p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.right =  p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.top =    p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.bottom=  p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y + num_height/2;

            p_layer->image_width = num_width * 4 + colon_width + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_HOUR:
        {
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x = center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.left =   p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.right =  p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.top =    p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect.bottom=  p_info->self_info[DIGITAL_TIME_HOUR_HIGH].center_point.y + num_height/2;

            p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x = center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.left =   p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.right =  p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.top =    p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect.bottom=  p_info->self_info[DIGITAL_TIME_HOUR_LOW].center_point.y + num_height/2;

            p_layer->image_width = num_width * 2 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_MIN:
        {
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x = center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.left =   p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.right =  p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.top =    p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect.bottom=  p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].center_point.y + num_height/2;

            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x = center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y = center_point.y;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.left =   p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x - num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.right =  p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.x + num_width/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.top =    p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y - num_height/2;
            p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect.bottom=  p_info->self_info[DIGITAL_TIME_MINUTE_LOW].center_point.y + num_height/2;

            p_layer->image_width = num_width * 2 + WATCHFACE_CREATE_LAYER_MARGIN;
            p_layer->image_height = num_height + WATCHFACE_CREATE_LAYER_MARGIN;
            break;
        }
        default:
            break;
    }
}


/*****************************************************************************/
//  Description : display digital clock
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalTime(MMI_WIN_ID_T                    win_id,
                                    wchar*                          p_num_path[],
                                    wchar*                          p_conn_path,
                                    WATCHFACE_DIGITAL_TIME_OBJ_T*   p_info,
                                    BOOLEAN                         is_create
                                    )
{
    SCI_TIME_T  sys_time= {0};
    uint8       item_value[DIGITAL_TIME_ITEM_MAX] = {0};
    uint8       index = 0;
    BOOLEAN     is_need_update = FALSE;

    //get sys time
    TM_GetSysTime(&sys_time);
#ifdef WIN32
    do
    {
        LOCAL uint8 localsec = 0;
        localsec++;
        localsec %= 60;
        sys_time.sec= localsec;
    } while (0);
#endif

    //calculate numbers for each position
    item_value[DIGITAL_TIME_HOUR_HIGH] = sys_time.hour / 10;
    item_value[DIGITAL_TIME_HOUR_LOW] = sys_time.hour % 10;

    item_value[DIGITAL_TIME_MINUTE_HIGH] = sys_time.min / 10;
    item_value[DIGITAL_TIME_MINUTE_LOW] = sys_time.min % 10;

    //create digital time
    CTRL_WF_TRACE("type = %d", p_info->type);
    switch (p_info->type)
    {
        case WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN:
        {
            for(index = DIGITAL_TIME_HOUR_HIGH; index < DIGITAL_TIME_ITEM_MAX; index ++)
            {
                if (item_value[index] != p_info->self_info[index].time_value)
                {
                    is_need_update = TRUE;
                    break;
                }
            }

            if(TRUE == is_create)
            {
                //create colon
                CTRLWF_Common_CreateImg(win_id,
                                        p_info->self_info[DIGITAL_TIME_COLON].ctrl_id,
                                        &p_info->self_info[DIGITAL_TIME_COLON].rect,
                                        p_conn_path,
                                        p_info->scale,
                                        &(p_info->layer)
                                        );
                for(index = DIGITAL_TIME_HOUR_HIGH; index < DIGITAL_TIME_ITEM_MAX; index ++)
                {
                    if(index != DIGITAL_TIME_COLON)
                    {
                        //record current time value
                        p_info->self_info[index].time_value = item_value[index];
                        //create time item
                        CTRLWF_Common_CreateImg(win_id,
                                                p_info->self_info[index].ctrl_id,
                                                &p_info->self_info[index].rect,
                                                p_num_path[p_info->self_info[index].time_value],
                                                p_info->scale,
                                                &(p_info->layer)
                                                );
                    }
                }
            }
            //update digital time
            else
            {
                if (TRUE == is_need_update)
                {
#ifdef UI_MULTILAYER_SUPPORT
                    //clear layer
                    UILAYER_Clear(&(p_info->layer));
#endif
                    CTRLWF_Common_UpdateImg(p_info->self_info[DIGITAL_TIME_COLON].ctrl_id,
                                            &p_info->self_info[DIGITAL_TIME_COLON].rect,
                                            p_conn_path,
                                            p_info->scale, &(p_info->layer));
                    for(index = DIGITAL_TIME_HOUR_HIGH; index < DIGITAL_TIME_ITEM_MAX; index ++)
                    {
                        if((index != DIGITAL_TIME_COLON))
                        {
                            //record current time value
                            p_info->self_info[index].time_value = item_value[index];
                            //update time item
                            CTRLWF_Common_UpdateImg(p_info->self_info[index].ctrl_id,
                                                    &p_info->self_info[index].rect,
                                                    p_num_path[p_info->self_info[index].time_value],
                                                    p_info->scale, &(p_info->layer)
                                                    );
                        }
                    }
                }
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_HOUR:
        {
            if ((item_value[DIGITAL_TIME_HOUR_HIGH] != p_info->self_info[DIGITAL_TIME_HOUR_HIGH].time_value)
                || (item_value[DIGITAL_TIME_HOUR_LOW] != p_info->self_info[DIGITAL_TIME_HOUR_LOW].time_value))
            {
                is_need_update = TRUE;
            }

            if(TRUE == is_create)
            {
                //record current time value
                p_info->self_info[DIGITAL_TIME_HOUR_HIGH].time_value = item_value[DIGITAL_TIME_HOUR_HIGH];
                p_info->self_info[DIGITAL_TIME_HOUR_LOW].time_value = item_value[DIGITAL_TIME_HOUR_LOW];
                //create time item
                CTRLWF_Common_CreateImg(win_id,
                                        p_info->self_info[DIGITAL_TIME_HOUR_HIGH].ctrl_id,
                                        &p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect,
                                        p_num_path[p_info->self_info[DIGITAL_TIME_HOUR_HIGH].time_value],
                                        p_info->scale,
                                        &(p_info->layer)
                                        );
                CTRLWF_Common_CreateImg(win_id,
                                        p_info->self_info[DIGITAL_TIME_HOUR_LOW].ctrl_id,
                                        &p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect,
                                        p_num_path[p_info->self_info[DIGITAL_TIME_HOUR_LOW].time_value],
                                        p_info->scale,
                                        &(p_info->layer)
                                        );
            }
            //update digital time
            else
            {
                if (TRUE == is_need_update)
                {
#ifdef UI_MULTILAYER_SUPPORT
                    //clear layer
                    UILAYER_Clear(&(p_info->layer));
#endif
                    //record current time value
                    p_info->self_info[DIGITAL_TIME_HOUR_HIGH].time_value = item_value[DIGITAL_TIME_HOUR_HIGH];
                    p_info->self_info[DIGITAL_TIME_HOUR_LOW].time_value = item_value[DIGITAL_TIME_HOUR_LOW];
                    //update time item
                    CTRLWF_Common_UpdateImg(p_info->self_info[DIGITAL_TIME_HOUR_HIGH].ctrl_id,
                                            &p_info->self_info[DIGITAL_TIME_HOUR_HIGH].rect,
                                            p_num_path[p_info->self_info[DIGITAL_TIME_HOUR_HIGH].time_value],
                                            p_info->scale, &(p_info->layer)
                                            );
                    CTRLWF_Common_UpdateImg(p_info->self_info[DIGITAL_TIME_HOUR_LOW].ctrl_id,
                                            &p_info->self_info[DIGITAL_TIME_HOUR_LOW].rect,
                                            p_num_path[p_info->self_info[DIGITAL_TIME_HOUR_LOW].time_value],
                                            p_info->scale, &(p_info->layer)
                                            );
                }
            }
            break;
        }
        case WATCHFACE_TYPE_DIGITAL_TIME_MIN:
        {
            if ((item_value[DIGITAL_TIME_MINUTE_HIGH] != p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].time_value)
                || (item_value[DIGITAL_TIME_MINUTE_LOW] != p_info->self_info[DIGITAL_TIME_MINUTE_LOW].time_value))
            {
                is_need_update = TRUE;
            }

            if(TRUE == is_create)
            {
                //record current time value
                p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].time_value = item_value[DIGITAL_TIME_MINUTE_HIGH];
                p_info->self_info[DIGITAL_TIME_MINUTE_LOW].time_value = item_value[DIGITAL_TIME_MINUTE_LOW];
                //create time item
                CTRLWF_Common_CreateImg(win_id,
                                        p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].ctrl_id,
                                        &p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect,
                                        p_num_path[p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].time_value],
                                        p_info->scale,
                                        &(p_info->layer)
                                        );
                CTRLWF_Common_CreateImg(win_id,
                                        p_info->self_info[DIGITAL_TIME_MINUTE_LOW].ctrl_id,
                                        &p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect,
                                        p_num_path[p_info->self_info[DIGITAL_TIME_MINUTE_LOW].time_value],
                                        p_info->scale,
                                        &(p_info->layer)
                                        );
            }
            //update digital time
            else
            {
                if (TRUE == is_need_update)
                {
#ifdef UI_MULTILAYER_SUPPORT
                    //clear layer
                    UILAYER_Clear(&(p_info->layer));
#endif
                    //record current time value
                    p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].time_value = item_value[DIGITAL_TIME_MINUTE_HIGH];
                    p_info->self_info[DIGITAL_TIME_MINUTE_LOW].time_value = item_value[DIGITAL_TIME_MINUTE_LOW];
                    //update time item
                    CTRLWF_Common_UpdateImg(p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].ctrl_id,
                                            &p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].rect,
                                            p_num_path[p_info->self_info[DIGITAL_TIME_MINUTE_HIGH].time_value],
                                            p_info->scale, &(p_info->layer)
                                            );
                    CTRLWF_Common_UpdateImg(p_info->self_info[DIGITAL_TIME_MINUTE_LOW].ctrl_id,
                                            &p_info->self_info[DIGITAL_TIME_MINUTE_LOW].rect,
                                            p_num_path[p_info->self_info[DIGITAL_TIME_MINUTE_LOW].time_value],
                                            p_info->scale, &(p_info->layer)
                                            );
                }
            }
            break;
        }
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : update by timer
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDigitalTime(uint8 timer_id, uint32 param)
{
    WATCHFACE_DIGITAL_TIME_OBJ_T* p_info = (WATCHFACE_DIGITAL_TIME_OBJ_T*) param;
    wchar*              p_img_number[WATCHFACE_DIGITAL_LIST_NUM] = {PNULL};
    uint8               i = 0;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("param = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_info->win_id))
    {
        if (0 != p_info->timer_id)
        {
            if(timer_id == p_info->timer_id)
            {
                //update time
                for(i = 0; i < WATCHFACE_DIGITAL_LIST_NUM; i++)
                {
                    p_img_number[i] = p_info->img_number[i];
                }
                DisplayDigitalTime(p_info->win_id, p_img_number, p_info->img_colon, p_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif
            }
        }
    }
}


/*****************************************************************************/
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DIGITAL_TIME_OBJ_T* CreateDigitalTimeData(
                                                    MMI_WIN_ID_T                    win_id,
                                                    uint32                          update_interval,
                                                    char*                           p_image_list[],
                                                    char*                           p_image_connector,
                                                    float                           scale,
                                                    WATCHFACE_ALL_TYPE_E            type
                                                    )
{
    WATCHFACE_DIGITAL_TIME_OBJ_T*  p_info = PNULL;
    wchar               num_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    wchar               conn_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint8               wstr_len = 0;
    uint8               i = 0;

    //alloc space
    p_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_DIGITAL_TIME_OBJ_T));
    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("The pInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_DIGITAL_TIME_OBJ_T));

    //store base info
    p_info->win_id = win_id;
    p_info->time_out = update_interval;
    p_info->scale = scale;
    p_info->type = type;
    CTRL_WF_TRACE("type = %d\n", type);
    MMIAPICOM_StrToWstr((uint8 *)p_image_connector, conn_wstr);
    wstr_len = MMIAPICOM_Wstrlen(conn_wstr);
    MMI_WSTRNCPY(p_info->img_colon, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)conn_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);

    for(i = 0; i < WATCHFACE_DIGITAL_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], num_wstr);
            wstr_len = MMIAPICOM_Wstrlen(num_wstr);
            MMI_WSTRNCPY(p_info->img_number[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)num_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
        }
    }
    return p_info;
}


/*****************************************************************************/
//  Description : create watch face digital time ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] update_interval: 刷新间隔/ms
//             [In] p_image_list[]: 数字资源路径
//             [In] p_image_connector: 连接符资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_DigitalTime_Create (
                                                MMI_WIN_ID_T                    win_id,
                                                MMI_CTRL_ID_T                   ctrl_id,
                                                GUI_POINT_T                     center_point,
                                                uint32                          update_interval,
                                                char*                           p_image_list[],
                                                char*                           p_image_connector,
                                                float                           scale,
                                                WATCHFACE_ALL_TYPE_E            type
                                                )
{
    MMI_HANDLE_T        handle = 0;
    GUIIMG_INFO_T       num_img_info = {0};
    GUIIMG_INFO_T       conn_img_info = {0};
    WATCHFACE_DIGITAL_TIME_OBJ_T*  p_info = PNULL;
    wchar*              p_img_number[WATCHFACE_DIGITAL_LIST_NUM] = {PNULL};
    GUIIMG_INFO_T       layer_size = {0};
    uint8               i = 0;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("full_path_ptr = pnull!");
        return handle;
    }

    //create data
    p_info = CreateDigitalTimeData(win_id, update_interval, p_image_list, p_image_connector, scale, type);
    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_info;
        p_info->handle = handle;
    }

    //get image width and height
    CTRLWF_Common_GetImgInfoByPath(p_info->img_number[0], &num_img_info);
    CTRLWF_Common_GetImgInfoByPath(p_info->img_colon, &conn_img_info);

    //set items ctrl id
    SetDigitalItemsCtrlId(p_info, ctrl_id, conn_img_info);

    //set display rect
    SetDigitalItemsPosAndLayerSize(p_info, center_point, num_img_info, conn_img_info, &layer_size);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_info->layer), win_id, layer_size.image_width, layer_size.image_height);
    if (p_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_DIGITAL_TIME_OBJ_T));
        SCI_FREE(p_info);
        p_info = PNULL;
        return 0;
    }
    UILAYER_SetLayerPosition(&(p_info->layer),
                            (center_point.x - (layer_size.image_width) / 2),
                            (center_point.y - (layer_size.image_height) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_info->layer));
#endif
    //display clock
    for(i = 0; i < WATCHFACE_DIGITAL_LIST_NUM; i++)
    {
        p_img_number[i] = p_info->img_number[i];
    }
    DisplayDigitalTime(win_id, p_img_number, p_info->img_colon, p_info, TRUE);

    if (0 != p_info->timer_id)
    {
        MMK_StopTimer(p_info->timer_id);
        p_info->timer_id = 0;
    }
    p_info->timer_id = MMK_CreateTimerCallback(update_interval, UpdateDigitalTime, (WATCHFACE_DIGITAL_TIME_OBJ_T*)p_info, TRUE);

    return handle;
}


/*****************************************************************************/
//  Description : destory watch face digital time ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalTime_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_DIGITAL_TIME_OBJ_T*  p_info = PNULL;
    uint8       index = 0;

    CTRL_WF_TRACE("destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DIGITAL_TIME_OBJ_T*)handle;

        if (handle == p_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_info->timer_id);
            p_info->timer_id = 0;
            //destory ctrl
            for(index = DIGITAL_TIME_HOUR_HIGH; index < DIGITAL_TIME_ITEM_MAX; index ++)
            {
                MMK_DestroyControl(p_info->self_info[index].ctrl_id);
            }
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_info->layer));
#endif
            //free space
            SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_DIGITAL_TIME_OBJ_T));
            SCI_FREE(p_info);
            p_info = PNULL;
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
PUBLIC BOOLEAN CTRLWF_DigitalTime_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_DIGITAL_TIME_OBJ_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DIGITAL_TIME_OBJ_T*)handle;
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
PUBLIC void CTRLWF_DigitalTime_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_DIGITAL_TIME_OBJ_T*   p_info = PNULL;
    wchar*                          p_img_number[WATCHFACE_DIGITAL_LIST_NUM] = {PNULL};
    uint8                           i = 0;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DIGITAL_TIME_OBJ_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                for(i = 0; i < WATCHFACE_DIGITAL_LIST_NUM; i++)
                {
                    p_img_number[i] = p_info->img_number[i];
                }
                DisplayDigitalTime(p_info->win_id, p_img_number, p_info->img_colon, p_info, FALSE);
                if (0 == p_info->timer_id)
                {
                   p_info->timer_id = MMK_CreateTimerCallback(p_info->time_out, UpdateDigitalTime, (WATCHFACE_DIGITAL_TIME_OBJ_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

