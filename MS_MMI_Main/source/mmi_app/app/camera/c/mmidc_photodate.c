/*****************************************************************************
** File Name:      mmidc_photodate.c                                         *
** Author:                                                                   *
** Date:           06/11/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dc application.             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011       robert.wang         Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifdef CAMERA_SUPPORT
#ifdef MMI_CAMERA_F_PHOTO_DATE
#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "sci_api.h"
#include "sfs.h"
#include "mmifmm_export.h"
#include "mmi_text.h"

#include "mmidisplay_data.h"
#include "mmiset_display.h"

//#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_display.h"
#include "mmidc_setting.h"
#include "mmidc_photodate.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define EXPAND_PIXEL 0
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/
LOCAL MMIDC_PHOTO_DATE_INFO_T *s_photo_date_info_ptr = PNULL;

/*---------------------------------------------------------------------------*
**                          TYPE AND CONSTANT                                *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                        LOCAL FUNCTION DECLARE                             *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : init photo date parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL DATE_OSD_LAYOUT_E GetPhotoDateOSDLayout(void)
{
    return OSD_LAYOUT_ONE_LINE;
}

/*****************************************************************************/
//Description : get photo date layer id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL OSD_LAYER_ID_E GetPhotoDateLayerID(void) 
{
    OSD_LAYER_ID_E id = 0;

    if (PNULL != s_photo_date_info_ptr)
    {
        id = s_photo_date_info_ptr->cur_layer_id;
    }
    
    return id;
}

/*****************************************************************************/
//Description : set photo date layer id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPhotoDateLayerID(OSD_LAYER_ID_E id
                                    ) 
{

    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->cur_layer_id = id;
    }

    //SCI_TRACE_LOW:"[MMIDC] SetPhotoDateLayerID id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_96_112_2_18_2_9_37_506,(uint8*)"d", id);
}

/*****************************************************************************/
//Description : get photo date photo size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_PHOTO_SIZE_E GetPhotoDatePhotoSize(void)
{
    OSD_LAYER_ID_E id = 0;
    MMIDC_PHOTO_SIZE_E photo_size = 0;

    id = GetPhotoDateLayerID();

    if (OSD_LAYER_PHOTO == id)
    {
        photo_size = MMIDC_GetPhotoSize();
    }
    else
    {
        photo_size = MMIDC_GetThumbnailPhotoSize();
    }

    return photo_size;
}
/*****************************************************************************/
//Description : get date osd init attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
//    if x and y are 0,then calculate by hor_offset and ver_offset
/*****************************************************************************/
LOCAL DATE_OSD_ATTRIBUTE_T GetDateOSDInitAttribute(MMIDC_PHOTO_SIZE_E photo_size
                                                        )
{
    DATE_OSD_ATTRIBUTE_T attr_array[]=
    {
        {PHOTO_SIZE_128X160, 0, 0, 6, 6, SONG_FONT_12},
		{PHOTO_SIZE_160X128, 0, 0, 6, 6, SONG_FONT_12},
        {PHOTO_SIZE_176X220, 0, 0, 6, 6, SONG_FONT_12}, 
        {PHOTO_SIZE_220X176, 0, 0, 6, 6, SONG_FONT_12}, 
        {PHOTO_SIZE_240X320,  0, 0, 10, 10, SONG_FONT_14},
        {PHOTO_SIZE_240X400,  0, 0, 10, 10, SONG_FONT_14},
        {PHOTO_SIZE_400X240,  0, 0, 10, 10, SONG_FONT_14},
        {PHOTO_SIZE_320X480,  0, 0, 20, 20, SONG_FONT_20},
        {PHOTO_SIZE_480X320,  0, 0, 20, 20, SONG_FONT_20},
        {PHOTO_SIZE_480X640,  0, 0, 20, 20, SONG_FONT_20},
        {PHOTO_SIZE_320X240,  0, 0, 10, 10, SONG_FONT_14},
        {PHOTO_SIZE_640X480,  0, 0, 20, 20, SONG_FONT_20},
        {PHOTO_SIZE_960X1280, 0, 0, 20, 20, SONG_FONT_32},        
        {PHOTO_SIZE_1280X960, 0, 0, 20, 20, SONG_FONT_32},
        {PHOTO_SIZE_1600X1200,0, 0, 20, 20, SONG_FONT_32},
        {PHOTO_SIZE_2048X1536,0, 0, 20, 20, SONG_FONT_48},
        {PHOTO_SIZE_2592X1944,0, 0, 20, 20, SONG_FONT_48},
        {PHOTO_SIZE_3264X2448,0, 0, 20, 20, SONG_FONT_62}
    };
    
    DATE_OSD_ATTRIBUTE_T osd_attribute = {0};
    int16 total_items = 0;
    int16 i = 0;

    total_items = ARR_SIZE(attr_array);   

    //SCI_TRACE_LOW:"[MMIDC] GetDateOSDInitAttribute photo_size=%d, total_items=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_155_112_2_18_2_9_37_507,(uint8*)"dd",photo_size, total_items);
               
    for (i = 0; i < total_items; i++)
    {
        if (photo_size == attr_array[i].photo_size)
        {
            osd_attribute = attr_array[i];
            break;
        }
    }
    
    return osd_attribute;
}
/*****************************************************************************/
//Description : init photo date parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN InitPhotoDateParameter(void)
{
    BOOLEAN ret = TRUE;

    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->font_color = MMIDC_TEXT_COLOR;   
    }
    
    return ret;
}

/*****************************************************************************/
// 	Description : get time string
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetTimeStr(MMI_STRING_T *str_ptr //out
                         )
{
    SCI_TIME_T time = {0};
    char time_char[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    uint16 time_len = 0;
    
    TM_GetSysTime(&time);
    time_len = sizeof(time_char);
    MMIAPISET_FormatTimeStrByTime(time.hour, time.min, (uint8*)time_char, time_len);

    time_len = strlen(time_char);
    str_ptr->wstr_len = time_len;
    MMI_STRNTOWSTR(str_ptr->wstr_ptr, str_ptr->wstr_len,
                   (const uint8*)time_char, time_len,
                   time_len
                   );
}

/*****************************************************************************/
// 	Description : get date string
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetDateStr(MMI_STRING_T *str_ptr //out
                         )
{
    char date_char[MMISET_DATE_STR_LEN + 1] = {0};
    uint16 date_str_len = 0;
    char symbol = '/';
    
    date_str_len = MMISET_DATE_STR_LEN;
    MMIAPISET_GetCurrentDateStr((uint8*)date_char, symbol, date_str_len);

    date_str_len = strlen(date_char);
    str_ptr->wstr_len = date_str_len;
    MMI_STRNTOWSTR(str_ptr->wstr_ptr, str_ptr->wstr_len,
                   (const uint8*)date_char, date_str_len,
                   date_str_len
                   );      
}

/*****************************************************************************/
// 	Description : get interval string
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetIntervalStr(MMI_STRING_T *str_ptr //out
                         )
{
    wchar space_wchar[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    int32 space_len = OSD_INTERVAL_STRING_NUMBER;

    SCI_MEM16SET(space_wchar, 0x20,space_len * 2);
    
    space_len = MIN(space_len, str_ptr->wstr_len);    
    MMIAPICOM_Wstrncpy(str_ptr->wstr_ptr, space_wchar, space_len);
    str_ptr->wstr_len = space_len;
}

/*****************************************************************************/
//Description : get photo date OSD font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetPhotoDateOSDFont(void) 
{
    GUI_FONT_T font = 0;

    if (PNULL != s_photo_date_info_ptr)
    {
        font = s_photo_date_info_ptr->osd_attribute.font;
    }
    
    return font;
}

/*****************************************************************************/
//Description : get photo date OSD font color
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIDC_GetPhotoDateOSDFontColor(void) 
{
    GUI_COLOR_T color = 0;

    if (PNULL != s_photo_date_info_ptr)
    {
        color = s_photo_date_info_ptr->font_color;
    }
    
    return color;
}

/*****************************************************************************/
//Description : get photo date attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL DATE_OSD_ATTRIBUTE_T GetPhotoDateOSDAttribute(void) 
{
    DATE_OSD_ATTRIBUTE_T  attr = {0};

    if (PNULL != s_photo_date_info_ptr)
    {
        attr = s_photo_date_info_ptr->osd_attribute;
    }
    
    return attr;
}

/*****************************************************************************/
//Description : set photo date attribute
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPhotoDateOSDAttribute(DATE_OSD_ATTRIBUTE_T  attr
                                           ) 
{
    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->osd_attribute = attr;
    }
}

/*****************************************************************************/
// 	Description : get photo date string rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetPhotoDateStringRect (void)
{
    GUI_RECT_T rect = {0};
    GUI_FONT_T font = GetPhotoDateOSDFont();
    wchar date_wchar[MMISET_DATE_STR_LEN + 1] = {0};
    wchar time_wchar[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar interval_wchar[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};

    MMI_STRING_T date_string = {0};
    MMI_STRING_T time_string = {0};
    MMI_STRING_T interval_string = {0};

    int16 interval_text_width = 0;
    int16 interval_text_height = 0;
    int16 date_text_width = 0;
    int16 date_text_height = 0;
    int16 time_text_width = 0;
    int16 time_text_height = 0;
    int16 text_width = 0;
    int16 text_height = 0;

    DATE_OSD_LAYOUT_E layout = 0;
    

    date_string.wstr_len = MMISET_DATE_STR_LEN;
    date_string.wstr_ptr = date_wchar;
    GetDateStr(&date_string);
    
    time_string.wstr_len = MMISET_TIME_STR_12HOURS_LEN;
    time_string.wstr_ptr = time_wchar;    
    GetTimeStr(&time_string);

    interval_string.wstr_len = MMIFILE_FILE_NAME_MAX_LEN;
    interval_string.wstr_ptr = interval_wchar;
    GetIntervalStr(&interval_string);
    
    date_text_width = MMIDC_GetStringWidthByFont(font, &date_string);
    date_text_height = MMIDC_GetStringHeightByFont(font, &date_string);
    
    time_text_width = MMIDC_GetStringWidthByFont(font, &time_string);
    time_text_height = MMIDC_GetStringHeightByFont(font, &time_string);

    interval_text_width = MMIDC_GetStringWidthByFont(font, &interval_string);
    interval_text_height = MMIDC_GetStringHeightByFont(font, &interval_string);

    layout = GetPhotoDateOSDLayout();
    if (OSD_LAYOUT_ONE_LINE == layout)
    {
        text_height = MAX(date_text_height, time_text_height);
        text_width = date_text_width + time_text_width + interval_text_width + PHOTO_DATE_OSD_SPACE;
    }
    else
    {
        text_width = MAX(date_text_width, time_text_width);
        text_height = date_text_height + time_text_height + PHOTO_DATE_OSD_SPACE;
    }

    rect.right = rect.left + text_width - 1;
    rect.bottom = rect.top + text_height - 1;

    return rect;
}

/*****************************************************************************/
// 	Description : display date string 
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void DisplayPhotoDateString(int16 *start_x_ptr, //out
                                        int16 *start_y_ptr  //out
                                        )
{
    GUI_RECT_T rect = {0};
    GUI_FONT_T font = GetPhotoDateOSDFont();
    GUI_COLOR_T bg_color = MMIDC_GetTransparentColor();
    wchar date_wchar[MMISET_DATE_STR_LEN + 1] = {0};
    wchar time_wchar[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar interval_wchar[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};


    MMI_STRING_T date_string = {0};
    MMI_STRING_T time_string = {0};
    MMI_STRING_T interval_string = {0};

    int16 interval_text_width = 0;
    int16 interval_text_height = 0;    
    int16 date_text_width = 0;
    int16 date_text_height = 0;
    int16 time_text_width = 0;
    int16 time_text_height = 0;
    int16 text_width_max = 0;
    int16 text_height_max = 0;
    
    DATE_OSD_LAYOUT_E layout = 0;
    int16 x = 0;
    int16 y = 0;
    int16 x_offset = 0;
    int16 y_offset = 0;

    int16 start_x = 0;
    int16 start_y = 0;

    MMIDC_PHOTO_SIZE_E photo_size = 0;
    MMIDC_SIZE_ATTRIBUTE_T size_val = {0};
    DATE_OSD_ATTRIBUTE_T osd_attribute = {0};
    

    date_string.wstr_len = MMISET_DATE_STR_LEN;
    date_string.wstr_ptr = date_wchar;
    GetDateStr(&date_string);
    
    time_string.wstr_len = MMISET_TIME_STR_12HOURS_LEN;
    time_string.wstr_ptr = time_wchar;    
    GetTimeStr(&time_string);

    interval_string.wstr_len = MMIFILE_FILE_NAME_MAX_LEN;
    interval_string.wstr_ptr = interval_wchar;
    GetIntervalStr(&interval_string);
    
    date_text_width = MMIDC_GetStringWidthByFont(font, &date_string);
    date_text_height = MMIDC_GetStringHeightByFont(font, &date_string);
    
    time_text_width = MMIDC_GetStringWidthByFont(font, &time_string);
    time_text_height = MMIDC_GetStringHeightByFont(font, &time_string);

    interval_text_width = MMIDC_GetStringWidthByFont(font, &interval_string);
    interval_text_height = MMIDC_GetStringHeightByFont(font, &interval_string);

    text_width_max = MAX(date_text_width, time_text_width);
    text_width_max = MAX(text_width_max, interval_text_width);

    text_height_max = MAX(date_text_height, time_text_height);
    text_height_max = MAX(text_height_max, interval_text_height);

    //SCI_TRACE_LOW:"[MMIDC] DisplayPhotoDateString date_width=%d,time_width=%d,interval_width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_445_112_2_18_2_9_38_508,(uint8*)"ddd",date_text_width, time_text_width, interval_text_width);


    //SCI_TRACE_LOW:"[MMIDC] DisplayPhotoDateString text_width_max=%d,text_height_max=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_449_112_2_18_2_9_38_509,(uint8*)"dd",text_width_max, text_height_max);
    
    layout = GetPhotoDateOSDLayout();
    photo_size = GetPhotoDatePhotoSize();
    osd_attribute = GetPhotoDateOSDAttribute();
    start_x = osd_attribute.x;
    start_y = osd_attribute.y;
    x_offset = osd_attribute.x_offset;
    y_offset = osd_attribute.y_offset;
    
    if ( (0 == start_x)
        && (0 == start_y)
       )
    {
        MMIDC_GetPhotoSizeWidthAndHeight(photo_size, &size_val);
       if (OSD_LAYOUT_ONE_LINE == layout)
       {
            x = size_val.size_width - x_offset - date_text_width\
                - interval_text_width - time_text_width;
            y = size_val.size_height - y_offset - text_height_max;
       }
       else
       {
            x = size_val.size_width - x_offset - text_width_max;
            y = size_val.size_height - y_offset - date_text_height \
                - time_text_height - PHOTO_DATE_OSD_SPACE;
       }
    }
    else
    {
        x = start_x;
        y = start_y;
    }

    rect.left = 0;
    rect.top = 0;
    if (OSD_LAYOUT_ONE_LINE == layout)
    {
        //display date
        rect.bottom = rect.top + text_height_max + PHOTO_DATE_OSD_SPACE - 1;
        rect.right = rect.left + date_text_width - 1;
        MMIDC_DisplayDateOSDStringByFont(font, rect, &date_string, bg_color, TRUE);

        //display time
        rect.left = rect.right + interval_text_width + 1;
        rect.right = rect.left + time_text_width - 1;

        rect.right = MIN(rect.right, size_val.size_width - 1);
        MMIDC_DisplayDateOSDStringByFont(font, rect, &time_string, bg_color, TRUE);
    }
    else
    {
        rect.right = rect.left + text_width_max - 1;
        rect.bottom = rect.top + date_text_height - 1;
        MMIDC_DisplayDateOSDStringByFont(font, rect, &date_string, bg_color, TRUE);

        rect.top = rect.bottom + PHOTO_DATE_OSD_SPACE;
        rect.bottom = rect.top + time_text_height - 1;
        
        rect.bottom = MIN(rect.bottom, size_val.size_height - 1);
        MMIDC_DisplayDateOSDStringByFont(font, rect, &time_string, bg_color, TRUE);
    }

    (*start_x_ptr) = x;
    (*start_y_ptr) = y;

    //SCI_TRACE_LOW:"[MMIDC] DisplayPhotoDateString photo_size=%d,x=%d, y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_516_112_2_18_2_9_38_510,(uint8*)"ddd",photo_size, x, y);
}

/*****************************************************************************/
// 	Description : get photo date string rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetPhotoDateOSDRectWidthAndHeight (int16 *width_ptr, //out
                                                              int16 *height_ptr //ount
                                                              )
{
   GUI_RECT_T rect = {0};

   rect = GetPhotoDateStringRect();

   (*width_ptr) = (int16)(rect.right - rect.left + 1 + 2 * PHOTO_DATE_OSD_SPACE);
   (*height_ptr) = (int16)(rect.bottom - rect.top + 1 + 2 * PHOTO_DATE_OSD_SPACE);

   //SCI_TRACE_LOW:"[MMIDC] GetPhotoDateOSDRectWidthAndHeight width=%d, height=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_535_112_2_18_2_9_38_511,(uint8*)"dd",(*width_ptr), (*height_ptr));
}

/*****************************************************************************/
//Description : get photo date dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIDC_GetPhotoDateDevInfo(void)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    OSD_LAYER_ID_E id = 0;
    
    if (PNULL != s_photo_date_info_ptr)
    {
        id = GetPhotoDateLayerID();
        dev_info = s_photo_date_info_ptr->layer[id].dev_info;
    }
    
    return dev_info;
}

/*****************************************************************************/
//Description : get photo date layer dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetPhotoDateLayerDevInfo(OSD_LAYER_ID_E id
                                                       )
{
    GUI_LCD_DEV_INFO dev_info = {0};
    
    if (PNULL != s_photo_date_info_ptr)
    {
        dev_info = s_photo_date_info_ptr->layer[id].dev_info;
    }
    
    return dev_info;
}

/*****************************************************************************/
//Description : set photo date layer dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPhotoDateLayerDevInfo(GUI_LCD_DEV_INFO *dev_info_ptr, //in
                                           OSD_LAYER_ID_E id
                                           )
{
    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->layer[id].dev_info = *dev_info_ptr;
    }
}

/*****************************************************************************/
//Description : get photo date layer info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPhotoDateLayerInfo(PHOTO_DATE_LAYER_INFO_T *layer_ptr, //out
                                         OSD_LAYER_ID_E id
                                         )
{
    BOOLEAN ret = FALSE;
    
    if (PNULL != s_photo_date_info_ptr)
    {
        ret = s_photo_date_info_ptr->layer[id].is_enable;
        (*layer_ptr) = s_photo_date_info_ptr->layer[id];
    }

    //SCI_TRACE_LOW:"[MMIDC] GetPhotoDateLayerInfo ret=%d,id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_604_112_2_18_2_9_38_512,(uint8*)"dd",ret, id);
    
    return ret;
}

/*****************************************************************************/
//Description : set photo date layer info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPhotoDateLayerInfo(PHOTO_DATE_LAYER_INFO_T *layer_ptr, //in
                                       OSD_LAYER_ID_E id
                                      )
{
    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->layer[id] = (*layer_ptr);
    }
}

/*****************************************************************************/
//Description : set photo date display layer info 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPhotoDateDisplayLayerInfo(PHOTO_DATE_LAYER_INFO_T *layer_ptr, //in
                                               OSD_LAYER_ID_E id
                                              )
{
    if (PNULL != s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr->display_layer[id] = (*layer_ptr);
    }
}

/*****************************************************************************/
//Description : get photo date display layer info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC PHOTO_DATE_LAYER_INFO_T MMIDC_GetPhotoDateDisplayLayerInfo(void)
{
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};
    OSD_LAYER_ID_E id = 0;
    
    if (PNULL != s_photo_date_info_ptr)
    {
        id = GetPhotoDateLayerID();
        layer_info = s_photo_date_info_ptr->display_layer[id];
    }
    
    return layer_info;
}

/*****************************************************************************/
// 	Description : free photo date osd buffer
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestoryPhotoDateOSDLayer(void)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    OSD_LAYER_ID_E id = 0;
    
    for (id = 0; id < OSD_LAYER_MAX; id++)
    {
        dev_info = GetPhotoDateLayerDevInfo(id);

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_DestoryPhotoDateOSDLayer id=%d,block_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_669_112_2_18_2_9_39_513,(uint8*)"dd",id, dev_info.block_id);
        
        if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
        {
            UILAYER_RELEASELAYER(&dev_info);    /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&dev_info);            
//            dev_info.block_id = UILAYER_NULL_HANDLE;
            SetPhotoDateLayerDevInfo(&dev_info, id);        
        }
    }
}

/*****************************************************************************/
// 	Description : alloc Photo date osd layer
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateOSDLayer(PHOTO_DATE_LAYER_INFO_T *layer_ptr, //in
                                  OSD_LAYER_ID_E id
                                  )
{
    BOOLEAN result = FALSE;
    int16   text_width = 0;
    int16   text_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO dev_info = {0};
    UILAYER_RESULT_E ret = 0;
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};

                                        
    dev_info = GetPhotoDateLayerDevInfo(id);
    
    //SCI_TRACE_LOW:"[MMIDC] CreateOSDLayer id=%d,block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_701_112_2_18_2_9_39_514,(uint8*)"dd",id, dev_info.block_id);

    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&dev_info))
    {
        text_width = layer_ptr->layer_width;
        text_height= layer_ptr->layer_height;
        
         //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = MMIDC_MAIN_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = text_width;
        create_info.height = text_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        ret = UILAYER_CreateLayer(
            &create_info,
            &dev_info
            );

        //SCI_TRACE_LOW:"[MMIDC] CreateOSDLayer ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_723_112_2_18_2_9_39_515,(uint8*)"d", ret);

        if (UILAYER_RESULT_SUCCESS == ret)
        {
            layer_info.layer_width = text_width;
            layer_info.layer_height = text_height;
            layer_info.dev_info = dev_info;
            layer_info.buffer_ptr = UILAYER_GetLayerBufferPtr(&dev_info);
            if (PNULL != layer_info.buffer_ptr)
            {
                result = TRUE;
                layer_info.is_enable = TRUE;
                
                SetPhotoDateLayerInfo(&layer_info, id);
                SetPhotoDateDisplayLayerInfo(&layer_info, id);
            }

        }
        
        //SCI_TRACE_LOW:"[MMIDC] CreateOSDLayer block_id=%d, buf_ptr=0x%X"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_745_112_2_18_2_9_39_516,(uint8*)"dd",layer_info.dev_info.block_id, layer_info.buffer_ptr );
                 
        //SCI_TRACE_LOW:"[MMIDC] CreateOSDLayer width=%d, height=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_750_112_2_18_2_9_39_517,(uint8*)"dd",layer_info.layer_width, layer_info.layer_height );
    }
                  

   //SCI_TRACE_LOW:"[MMIDC] CreateOSDLayer result=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_756_112_2_18_2_9_39_518,(uint8*)"d",result);
             
   return result;
}

/*****************************************************************************/
//Description : create osd layer info
//Global resource dependence : none
//Author: robert.wang
//Note: create max photo size and thumbnail size OSD layer.
//      It do not destory them, until close DC
/*****************************************************************************/
PUBLIC BOOLEAN CreateOSDLayerInfo(OSD_LAYER_ID_E id
                                       )
{
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};
    int16 layer_width = 0;
    int16 layer_height = 0;
    DATE_OSD_ATTRIBUTE_T attr = {0};
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    BOOLEAN ret = FALSE;

    //step 1
    SetPhotoDateLayerID(id);
    
    InitPhotoDateParameter();

    //step 2
    switch(id)
    {
    case OSD_LAYER_PHOTO:
        photo_size = MMIDC_GetSupportSizeArray(PNULL);
        break;
    case OSD_LAYER_THUMBNAIL:
        photo_size = MMIDC_GetThumbnailPhotoSize();
        break;
    default:        
        break;
    }
    attr = GetDateOSDInitAttribute(photo_size);

    SetPhotoDateOSDAttribute(attr);
    
    //step 3
    GetPhotoDateOSDRectWidthAndHeight(&layer_width,\
                                      &layer_height\
                                      );
    layer_width += EXPAND_PIXEL;//expand memory
    layer_height += EXPAND_PIXEL;
    layer_info.layer_width = layer_width;
    layer_info.layer_height = layer_height;
    ret = CreateOSDLayer(&layer_info, id);
    
    //SCI_TRACE_LOW:"[MMIDC] CreatePhotoDateLayers ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_808_112_2_18_2_9_39_519,(uint8*)"d",ret);
                   
    return ret;
}

/*****************************************************************************/
//Description : alloc Photo date osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreatePhotoDateLayers(void)
{
    BOOLEAN ret = FALSE;

    ret = CreateOSDLayerInfo(OSD_LAYER_PHOTO);
    if (ret)
    {
        ret = CreateOSDLayerInfo(OSD_LAYER_THUMBNAIL);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreatePhotoDateLayers ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_828_112_2_18_2_9_39_520,(uint8*)"d",ret);
                   
    return ret;
}

/*****************************************************************************/
//Description : get display osd layer info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayOSDLayerInfo(OSD_LAYER_ID_E id,
                                          PHOTO_DATE_LAYER_INFO_T *layer_ptr, //out
                                          int16 *x_ptr, //out
                                          int16 *y_ptr  //out
                                         )
{
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};
    int16 layer_width = 0;
    int16 layer_height = 0;
    DATE_OSD_ATTRIBUTE_T attr = {0};
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    GUI_LCD_DEV_INFO dev_info = {0};
    BOOLEAN ret = FALSE;
    
    //step 1
    SetPhotoDateLayerID(id);
    
    InitPhotoDateParameter();

    //step 2
    photo_size = GetPhotoDatePhotoSize();

    attr = GetDateOSDInitAttribute(photo_size);

    SetPhotoDateOSDAttribute(attr);
    
    //step 3 
    ret = GetPhotoDateLayerInfo(&layer_info, id);
    if (ret)
    {
        (*layer_ptr) = layer_info;

        dev_info = layer_info.dev_info;
        UILAYER_Clear(&dev_info);

        //get actual rect
        GetPhotoDateOSDRectWidthAndHeight(&layer_width,\
                                          &layer_height\
                                          );   
                                      
        layer_ptr->layer_width = layer_width;
        layer_ptr->layer_height = layer_height;
        
        //change layer width and height
        SetPhotoDateDisplayLayerInfo(layer_ptr, id);

        //step 4
        DisplayPhotoDateString(x_ptr, y_ptr);        
    }    
    
    //SCI_TRACE_LOW:"[MMIDC] GetDisplayOSDLayerInfo ret=%d, width=%d, height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_888_112_2_18_2_9_39_521,(uint8*)"ddd",ret, layer_width, layer_height);

    return ret;
}

/*****************************************************************************/
// 	Description : callback for update photo date
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL uint32 UpdatePhotoDateCallback (void * param_ptr)
{
    //SCI_TRACE_LOW:"[MMIDC] UpdatePhotoDateCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_899_112_2_18_2_9_39_522,(uint8*)"");

    if (PNULL != param_ptr)
    {
        
    }

    return 0;
}

/*****************************************************************************/
//Description : create photo date data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreatePhotoDateData(void)
{
    BOOLEAN ret = FALSE;
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreatePhotoDateData s_photo_date_info_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_918_112_2_18_2_9_39_523,(uint8*)"d",s_photo_date_info_ptr);
    
    if (PNULL == s_photo_date_info_ptr)
    {
        s_photo_date_info_ptr = (MMIDC_PHOTO_DATE_INFO_T *)SCI_ALLOCA(sizeof(MMIDC_PHOTO_DATE_INFO_T));
        
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreatePhotoDateData ALLOC s_photo_date_info_ptr=0x%lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_925_112_2_18_2_9_39_524,(uint8*)"d",s_photo_date_info_ptr);
        
        if (PNULL != s_photo_date_info_ptr)
        {
            SCI_MEMSET(s_photo_date_info_ptr, 0x00, sizeof(*s_photo_date_info_ptr));
            
            MMIDC_CreatePhotoDateLayers();
        }
    }
    
    if (PNULL != s_photo_date_info_ptr)
    {
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreatePhotoDateData 2 s_photo_date_info_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_941_112_2_18_2_9_39_525,(uint8*)"d",s_photo_date_info_ptr);
    
    return ret;
}

/*****************************************************************************/
//Description : release photo date data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ReleasePhotoDateData(void)
{
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ReleasePhotoDateData s_photo_date_info_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_953_112_2_18_2_9_39_526,(uint8*)"d",s_photo_date_info_ptr);
    
    if (PNULL != s_photo_date_info_ptr)
    {    
        MMIDC_DestoryPhotoDateOSDLayer();

        SCI_FREE(s_photo_date_info_ptr);
    }

    return 0;
}

/*****************************************************************************/
// 	Description : get photo date parameter
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhotoDateParameter(DCAMERA_CAPTURE_DATE_T *capture_date_ptr //out
                              )
{
    BOOLEAN is_enable = FALSE;
    uint16 layer_width = 0;
    uint16 layer_height = 0;    
    int16 start_x = 0;
    int16 start_y = 0;

    BOOLEAN ret = 0;
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};
    
    if (PHOTO_DATE_ON == MMIDC_GetNVPhotoDate())
    {
        is_enable = TRUE;
    }

     if (is_enable)
     {
        //step 1
        ret = GetDisplayOSDLayerInfo(OSD_LAYER_PHOTO,
                                    &layer_info,
                                    &start_x,
                                    &start_y
                                    );
        if (ret)
        {
            layer_width = layer_info.layer_width;
            layer_height = layer_info.layer_height;
            
            capture_date_ptr->osd_addr.y_addr = (uint32)layer_info.buffer_ptr;

            capture_date_ptr->overlay_rect.x = start_x;
            capture_date_ptr->overlay_rect.y = start_y;
            capture_date_ptr->overlay_rect.w = layer_width;
            capture_date_ptr->overlay_rect.h = layer_height;

            capture_date_ptr->date_update = UpdatePhotoDateCallback;        


            //step 2
            ret = GetDisplayOSDLayerInfo(OSD_LAYER_THUMBNAIL,
                                        &layer_info,
                                        &start_x,
                                        &start_y
                                        );
            if (ret)
            {
                layer_width = layer_info.layer_width;
                layer_height = layer_info.layer_height;
                
                capture_date_ptr->thumb_osd_addr.y_addr = (uint32)layer_info.buffer_ptr;

                capture_date_ptr->thumb_overlay_rect.x = start_x;
                capture_date_ptr->thumb_overlay_rect.y = start_y;
                capture_date_ptr->thumb_overlay_rect.w = layer_width;
                capture_date_ptr->thumb_overlay_rect.h = layer_height;

                capture_date_ptr->thumb_date_update = UpdatePhotoDateCallback;        
            }
        }

        //if fail,then disable photo date 
        if (!ret)
        {
            is_enable = FALSE;
        }
     }

    capture_date_ptr->eb = is_enable;
    //SCI_TRACE_LOW:"[MMIDC] GetPhotoDateParameter is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_PHOTODATE_1039_112_2_18_2_9_39_527,(uint8*)"d", is_enable);

}
#endif //#if defined(MMI_CAMERA_F_PHOTO_DATE)
#endif  //#ifdef  CAMERA_SUPPORT

