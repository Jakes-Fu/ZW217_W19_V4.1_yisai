/*****************************************************************************
** File Name:         watch_common_list.c                                          *
** Author:           fangfang.yao                                            *
** Date:             02/07/2020                                              *
** Copyright:                                                                *
** Description:    This file is used to define common part for listbox       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/07/2020           fangfang.yao          Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmitheme_list.h"
#include "mmi_modu_main.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define WATCHCOM_MARKER_NUM_STR_LEN_MIN           (2)
#define WATCHCOM_MARKER_NUM_STR_LEN_MAX           (3)
#define WATCHCOM_MARKER_NUM_DISP_MAX              (100)
#define  WATCHCOM_LIST_ITEM_HEIGHT_MID        DP2PX_VALUE(60)
#define  WATCHCOM_LIST_TEXT_LEFT_SPACE         DP2PX_VALUE(0)
#define  WATCHCOM_LIST_RIGHT_BIG                     DP2PX_VALUE(204)
#define  WATCHCOM_LIST_TOP_SPACE                    DP2PX_VALUE(10)
#define  WATCHCOM_LIST_MIDDLE_MARGIN            DP2PX_VALUE(4)
#define  WATCHCOM_LIST_FONT1_SIZE                    DP2PX_FONT(SONG_FONT_20)
#define  WATCHCOM_LIST_FONT2_SIZE                    DP2PX_FONT(SONG_FONT_16)

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 设置king_style List 状态值
//  Parameter: [In] ctrl_id: the list control ID
//             [In] bHandle: List item set left right function
//             [Out] None
//             [Return] None
//  Author:fangfang.yao
// Note:
/*****************************************************************************/
LOCAL void Watch_SetAllListState( MMI_CTRL_ID_T ctrlId )
{
    //不画分割线
    GUILIST_SetListState( ctrlId, GUILIST_STATE_SPLIT_LINE, FALSE );
    //不画高亮条
    GUILIST_SetListState( ctrlId, GUILIST_STATE_NEED_HIGHTBAR, FALSE );
}

/*****************************************************************************/
//  Description : draw list ctrl--three string (height : small)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_Small_3Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_SMALL_3STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str3;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);
}

/*****************************************************************************/
//  Description : draw list ctrl--one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
#if defined SCREEN_SHAPE_SQUARE
    item_t.item_style = GUIITEM_STYLE_1STR;
#elif defined SCREEN_SHAPE_CIRCULAR
    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_1Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
    item_t.item_style = GUIITEM_STYLE_1ICON_1STR;
#elif defined SCREEN_SHAPE_CIRCULAR
    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = string;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);
}

/*****************************************************************************/
//  Description : draw list ctrl--one string ,one icon
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [In] image: the image ID for display image
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_1Icon(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string,
                                    MMI_IMAGE_ID_T image)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
    item_t.item_style = GUIITEM_STYLE_1STR_1ICON;
#elif defined SCREEN_SHAPE_CIRCULAR
    item_t.item_style = GUIITEM_STYLE_1STR_1ICON_1LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id        = image;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string, one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] img1: the first image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] img2: the second image ID for display image
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T img1,
                                    MMI_STRING_T str1,
                                    MMI_IMAGE_ID_T img2,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1ICON_1STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = img1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id        = img2;

    item_data.item_content[3].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer     = str2;
       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] pos: insert item position
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Insert_1Icon_2Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    uint16 pos)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_2STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_InsertItem(listCtrlId, &item_t, pos);

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_2Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_2STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl-- two string, one icon
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] image: the image ID for display image
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_2Str_1Icon(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_IMAGE_ID_T image)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
        item_t.item_style = GUIITEM_STYLE_2STR_1ICON;
#elif defined SCREEN_SHAPE_CIRCULAR
        item_t.item_style = GUIITEM_STYLE_2STR_1ICON_2LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id        = image;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl-- two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_2Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
    item_t.item_style = GUIITEM_STYLE_2STR;
#elif defined SCREEN_SHAPE_CIRCULAR
    item_t.item_style = GUIITEM_STYLE_2STR_2LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl-- three string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_3Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
    item_t.item_style = GUIITEM_STYLE_3STR;
#elif defined SCREEN_SHAPE_CIRCULAR
    item_t.item_style = GUIITEM_STYLE_3STR_2LINE_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str3;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, three string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_3Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_3STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

    item_data.item_content[3].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer     = str3;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl-- three string ,one marker
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] image: the image ID for display image
//             [In] str3: the third text buffer for display string
//             [In] str4: the forth text buffer for display string
//             [In] number: number info
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_2Str_1Icon_2Str(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str3,
                                    MMI_STRING_T str4)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
    MMI_IMAGE_ID_T          image_id        = 0;

    item_t.item_style = GUIITEM_STYLE_2STR_1ICON_2STR;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id        = image;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

    item_data.item_content[3].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer     = str3;

    item_data.item_content[4].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[4].item_data.text_buffer     = str4;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one string (CheckBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_Check(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
        item_t.item_style = GUIITEM_STYLE_1STR_CHECK;
#elif defined SCREEN_SHAPE_CIRCULAR
        item_t.item_style = GUIITEM_STYLE_1STR_1LINE_CHECK_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one string (RadioBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_Radio(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

#if defined SCREEN_SHAPE_SQUARE
        item_t.item_style = GUIITEM_STYLE_1STR_RADIO;
#elif defined SCREEN_SHAPE_CIRCULAR
        item_t.item_style = GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1;
#endif
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one string , one switch
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [In] isMark: current item is or not marked
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_1Switch(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string,
                                    BOOLEAN isMark)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
    MMI_IMAGE_ID_T          image_id        = 0;

    item_t.item_style = GUIITEM_STYLE_1STR_SWITCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    if(isMark == TRUE)
    {
        image_id = IMAGE_DROPDOWNLIST_ARROW_PR;
    }
    else
    {
        image_id = IMAGE_DROPDOWNLIST_ARROW_UN;
    }
    item_data.item_content[1].item_data.image_id        = image_id;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] icon_info: the icon info for display image
//             [In] str1: the text buffer for display string
//             [In] item_index
//             [Out] None
//             [Return] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_1Str_Dyna(
                                    MMI_CTRL_ID_T listCtrlId,
                                    GUIANIM_FILE_INFO_T icon_info,
                                    MMI_STRING_T str1,
                                    uint16 item_index)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_DYNA;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
    item_data.item_content[0].item_data.anim_path_ptr = &icon_info;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = str1.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = str1.wstr_ptr;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

    GUILIST_SetItemData( listCtrlId, &item_data, item_index );

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string, one string, one button, one button
//  Parameter: [In] listCtrlId: the list control ID
//             [In] icon_info: the icon info for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] button_info1: the button info1
//             [In] button_info2: the button info2
//             [In] item_index
//             [Out] None
//             [Return] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Icon_2Str_2Button_Dyna(
                                    MMI_CTRL_ID_T listCtrlId,
                                    GUIANIM_FILE_INFO_T icon_info,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_BUTTON_T button_info1,
                                    MMI_BUTTON_T button_info2,
                                    uint16 item_index)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
    GUIITEM_CONTENT_T       item_content    = {0};
    item_t.item_style = GUIITEM_STYLE_1ICON_2STR_2BUTTON_DYNA;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
    item_data.item_content[0].item_data.anim_path_ptr = &icon_info;

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = str1.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = str1.wstr_ptr;

    item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = str2.wstr_len;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = str2.wstr_ptr;

    item_data.item_content[3].item_data_type    = GUIITEM_DATA_ANIM_PATH;
    item_data.item_content[3].item_data.anim_path_ptr = &button_info1.image_info;

    item_data.item_content[4].item_data_type    = GUIITEM_DATA_ANIM_PATH;
    item_data.item_content[4].item_data.anim_path_ptr = &button_info2.image_info;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_SetItemData( listCtrlId, &item_data, item_index );

    GUILIST_AppendItem(listCtrlId, &item_t);
}
/*****************************************************************************/
/*****************************************************************************/
//  Description : 设置king_sytle layout区域
//  Parameter: [In] left_in: left info of the rect needed
//             [In] top_in:  top info of the rect needed
//             [In] right_in:  right info of the rect needed
//             [In] bottom_in:  bottom info of the rect needed
//             [Out] rect: the rect which need to set
//             [Return] None
//  Author:dandan.cao
// Note:
/*****************************************************************************/
LOCAL void SetList_LayoutRect(GUI_RECT_T *rect_ptr,
                                         uint16 left_in,
                                         uint16 top_in,
                                         uint16 right_in,
                                         uint16 bottom_in)
{
    if(rect_ptr != PNULL)
    {
        rect_ptr->left      = left_in;
        rect_ptr->top       = top_in;
        rect_ptr->right     = right_in;
        rect_ptr->bottom    = bottom_in;
    }
}
/*****************************************************************************/
//  Description : 设置king_style List 状态值
//  Parameter: [In] ctrl_id: the list control ID
//             [In] bHandle: List item set left right function
//             [Out] None
//             [Return] None
//  Author:dandan.cao
// Note:
/*****************************************************************************/
LOCAL void SetListAllListState(MMI_CTRL_ID_T ctrlId, BOOLEAN isHandleLeftRight)
{
    //不画分割线
    GUILIST_SetListState( ctrlId, GUILIST_STATE_SPLIT_LINE, FALSE );
}

/*****************************************************************************/
//  Description : draw list ctrl--two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_2Str_2Lin_ListItem(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T                 item_t      = {0};
    GUILIST_ITEM_DATA_T            item_data   = {0};
    THEMELIST_ITEM_STYLE_T         my_style    = {0};
    GUIITEM_STYLE_E                style_id    = 0;

    //CUSTOM STYLE
    my_style.height                 = WATCHCOM_LIST_ITEM_HEIGHT_MID;
    my_style.height_focus           = WATCHCOM_LIST_ITEM_HEIGHT_MID;

    my_style.content[0].font        = WATCHCOM_LIST_FONT1_SIZE;
    my_style.content[0].font_focus  = WATCHCOM_LIST_FONT1_SIZE;
    my_style.content[1].font        = WATCHCOM_LIST_FONT2_SIZE;
    my_style.content[1].font_focus  = WATCHCOM_LIST_FONT2_SIZE;

    SetList_LayoutRect(&my_style.content[0].rect,
                            WATCHCOM_LIST_TEXT_LEFT_SPACE,
                            WATCHCOM_LIST_TOP_SPACE,
                            WATCHCOM_LIST_RIGHT_BIG,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID/2);
    SetList_LayoutRect(&my_style.content[0].rect_focus,
                            WATCHCOM_LIST_TEXT_LEFT_SPACE,
                            WATCHCOM_LIST_TOP_SPACE,
                            WATCHCOM_LIST_RIGHT_BIG,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID/2);

    SetList_LayoutRect(&my_style.content[1].rect,
                            WATCHCOM_LIST_TEXT_LEFT_SPACE,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID/2 + WATCHCOM_LIST_MIDDLE_MARGIN,
                            WATCHCOM_LIST_RIGHT_BIG,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID - WATCHCOM_LIST_TOP_SPACE);
    SetList_LayoutRect(&my_style.content[1].rect_focus,
                            WATCHCOM_LIST_TEXT_LEFT_SPACE,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID/2 + WATCHCOM_LIST_MIDDLE_MARGIN,
                            WATCHCOM_LIST_RIGHT_BIG,
                            WATCHCOM_LIST_ITEM_HEIGHT_MID - WATCHCOM_LIST_TOP_SPACE);

    style_id = MMITHEME_CustomListItemStyleReg(listCtrlId, &my_style, 0);
    item_t.item_style = style_id;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = str1.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = str1.wstr_ptr;

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = str2.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = str2.wstr_ptr;

    //设置状态值
    SetListAllListState(listCtrlId , TRUE);
    if (FALSE == GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("Append failed!");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl-- three string ,one marker
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [In] number: number info
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_3Str_1Marker(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3,
                                    uint32 number)
{
    uint8           num_str[WATCHCOM_MARKER_NUM_STR_LEN_MAX]  = {0};
    wchar           temp_str[WATCHCOM_MARKER_NUM_STR_LEN_MAX] = {0};
    MMI_IMAGE_ID_T  image_id    = 0;
    MMI_STRING_T    numStr      = {0};

    if(number != 0)
    {
        if(number == 1)
        {
            image_id = common_list_disp_more;//common_list_disp_only_one;
        }
        else if(number <  WATCHCOM_MARKER_NUM_DISP_MAX)
        {
            image_id = common_list_disp_more;//common_list_disp_someinfo;
            MMIAPICOM_Int2Str(number,num_str,WATCHCOM_MARKER_NUM_STR_LEN_MIN);
            MMI_STRNTOWSTR(temp_str,WATCHCOM_MARKER_NUM_STR_LEN_MIN, num_str,WATCHCOM_MARKER_NUM_STR_LEN_MIN, WATCHCOM_MARKER_NUM_STR_LEN_MIN);
            numStr.wstr_ptr = temp_str;
            numStr.wstr_len = WATCHCOM_MARKER_NUM_STR_LEN_MIN;
        }
        else
        {
            image_id = common_list_disp_more;
            temp_str[0] = '9';
            temp_str[1] = '9';
            temp_str[2] = '+';
            numStr.wstr_ptr = temp_str;
            numStr.wstr_len = WATCHCOM_MARKER_NUM_STR_LEN_MAX;
        }
    }
    WatchCOM_ListItem_Draw_2Str_1Icon_2Str( listCtrlId, str1, numStr, image_id, str2, str3);
}


/*****************************************************************************/
//  Description: appand list
//  Parameter: [In] WATCHCOM_LIST_ITEM_DATA_ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_TextList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,  uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id )
{
    uint32          user_data   = 0;
    uint16          item_index  = 0;
    MMI_STRING_T    item_str1   ={0};
    MMI_STRING_T    item_str2   ={0};
    MMI_STRING_T    item_str3   ={0};
#ifdef ADULT_WATCH_SUPPORT
    MMI_STRING_T    item_str4   ={0};
#endif
    uint8           i           = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    for(i=0; i<list_total_num; i++)
    {
        if(TRUE == list_content_ptr[i].item_visible())
        {    
            switch(list_content_ptr[i].item_style)
            {
                case GUIITEM_STYLE_1STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_1STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    WatchCOM_ListItem_Draw_1Str(list_ctrl_id,item_str1);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_2STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_2STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_2STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    WatchCOM_ListItem_Draw_2Str(list_ctrl_id,item_str1,item_str2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }    
                case GUIITEM_STYLE_3STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_3STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_3STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    WatchCOM_ListItem_Draw_3Str(list_ctrl_id,item_str1,item_str2,item_str3);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }    
                case GUIITEM_STYLE_1STR_SWITCH:
                {
                    WATCHCOM_Item_SwitchOnOff _item_SwitchOnOff = NULL;
                    WATCHCOM_LIST_ITEM_STYLE_1STR_SWITCH_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_SWITCH_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    _item_SwitchOnOff = (WATCHCOM_Item_markNum)item_data.switch_onoff;
                    WatchCOM_ListItem_Draw_1Str_1Switch(list_ctrl_id,item_str1,_item_SwitchOnOff());
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_2STR_1ICON:
                {
                    WATCHCOM_LIST_ITEM_STYLE_2STR_1ICON_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_2STR_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    WatchCOM_ListItem_Draw_2Str_1Icon(list_ctrl_id,item_str1,item_str2,item_data.image_id);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }    
                case GUIITEM_STYLE_3STR_1MARKER:
                {
                    WATCHCOM_Item_markNum _item_markNum =NULL;
                    WATCHCOM_LIST_ITEM_STYLE_2STR_1MARKER_1STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_2STR_1MARKER_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    _item_markNum = (WATCHCOM_Item_markNum)item_data.maker_num;
                    WatchCOM_ListItem_Draw_3Str_1Marker(list_ctrl_id,item_str1,item_str2,item_str3,_item_markNum());
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;

                    break;
                }
                case GUIITEM_STYLE_1ICON_1STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    WatchCOM_ListItem_Draw_1Icon_1Str(list_ctrl_id,item_data.image_id,item_str1);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_1STR_1ICON_1STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_1ICON_1STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_1ICON_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str(list_ctrl_id,item_data.image_id1,item_str1,item_data.image_id2,item_str2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_2STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_2STR_1ICON_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_2STR_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    WatchCOM_ListItem_Draw_1Icon_2Str(list_ctrl_id,item_data.image_id,item_str1,item_str2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_3STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_3STR_1ICON_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_3STR_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    WatchCOM_ListItem_Draw_1Icon_3Str(list_ctrl_id,item_data.image_id,item_str1,item_str2,item_str3);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_SMALL_3STR:
                {
                    WATCHCOM_LIST_ITEM_STYLE_3STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_3STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    WatchCOM_ListItem_Draw_Small_3Str(list_ctrl_id,item_str1,item_str2,item_str3);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1STR_1ICON:
                {
                    WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    WatchCOM_ListItem_Draw_1Str_1Icon(list_ctrl_id,item_str1,item_data.image_id);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
#ifdef ADULT_WATCH_SUPPORT
                case GUIITEM_STYLE_3STR_3LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_3STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_3STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    AdultWatchCOM_ListItem_Draw_3Str_3line(list_ctrl_id,item_str1,item_str2,item_str3);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_4STR_3LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_4STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_4STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    MMI_GetLabelTextByLang(item_data.text_id3, &item_str3);
                    MMI_GetLabelTextByLang(item_data.text_id4, &item_str4);
                    AdultWatchCOM_ListItem_Draw_4Str_3line(list_ctrl_id,item_str1,item_str2,item_str3,item_str4);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_2STR_2LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_2STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_2STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    AdultWatchCOM_ListItem_Draw_2Str_2line(list_ctrl_id,item_str1,item_str2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(list_ctrl_id,item_data.image_id,item_str1,item_str2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                    MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                    AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(list_ctrl_id,item_str1,item_str2,item_data.image_id);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1STR_1LINE_LEFTALIGN_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_LEFTALIGN_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_LEFTALIGN_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    AdultWatchCOM_ListItem_Draw_1StrLeftAlign(list_ctrl_id,item_str1);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_LEFTALIGN_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_LEFTALIGN_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    AdultWatchCOM_ListItem_Draw_1StrCenterAlign(list_ctrl_id,item_str1);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1STR_1ICON_1LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(list_ctrl_id,item_str1,item_data.image_id);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    AdultWatchCOM_ListItem_Draw_1Icon_1Str_1line(list_ctrl_id,item_data.image_id,item_str1);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_1LINE_CENTERALIGN_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_ST *)(list_content_ptr[i].data_ptr);
                    AdultWatchCOM_ListItem_Draw_1Icon_1line(list_ctrl_id,item_data.image_id);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
                case GUIITEM_STYLE_1ICON_1STR_1ICON_1LINE_ADULTWATCH:
                {
                    ADULTWATCHCOM_LIST_ITEM_STYLE_2ICON_1STR_ST item_data = *(ADULTWATCHCOM_LIST_ITEM_STYLE_2ICON_1STR_ST *)(list_content_ptr[i].data_ptr);
                    MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                    AdultWatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1line(list_ctrl_id,item_data.image_id,item_str1,item_data.image_id2);
                    user_data = (uint32)(list_content_ptr[i].item_callback);
                    GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                    item_index++;
                    break;
                }
#endif
                default:
                {
                    SCI_TRACE_LOW("enter WATCHCOM_List_Create, unsupport list item style");
                    break;
                }
            }
        }
    }
    return TRUE;

}
/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_RadioList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_total_num,MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    for(i=0; i<list_total_num; i++)
    {
        //SCI_MEMSET(item_str1.wstr_ptr,0x00,item_str1.wstr_len*sizeof(wchar));//for bug1730399
        if(TRUE == list_content_ptr[i].item_visible()&& GUIITEM_STYLE_1STR_RADIO == list_content_ptr[i].item_style)
        {    
            WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST *)(list_content_ptr[i].data_ptr);
            MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
            WatchCOM_ListItem_Draw_1Str_Radio(list_ctrl_id,item_str1);
            user_data = (uint32)(list_content_ptr[i].item_callback);
            GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
            item_index++;
        }
        else
        {
            SCI_TRACE_LOW("enter WATCHCOM_Radio_List_Create,item is not visible or item style is wrong");
            assert(0);

        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_CheckList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    for(i=0; i<list_total_num; i++)
    {
        //SCI_MEMSET(item_str1.wstr_ptr,0x00,item_str1.wstr_len*sizeof(wchar));//for bug1730399
        if(TRUE == list_content_ptr[i].item_visible())
        {  
            if(GUIITEM_STYLE_1STR_CHECK == list_content_ptr[i].item_style)
            {
                WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST *)(list_content_ptr[i].data_ptr);
                MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                WatchCOM_ListItem_Draw_1Str_Check(list_ctrl_id,item_str1);
                user_data = (uint32)(list_content_ptr[i].item_callback);
                GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                item_index++;
            }
            else
            {
                SCI_TRACE_LOW("enter WATCHCOM_Check_List_Create,item is not visible or item style is wrong");
                assert(0);
            }
        }
    }
    GUILIST_SetMaxSelectedItem(list_ctrl_id, item_index);

    return TRUE;
}
/*****************************************************************************/
//  Description: enter the relative window with selected list item
//  Parameter: [In] ctrl_id: the list control ID
//             [Out] None
//             [Return] None
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_List_Item_CallBack(MMI_CTRL_ID_T ctrl_id)
{
    WATCHCOM_Item_CallBack _callback_listItem = NULL;
    uint32 user_data = 0;
    uint16 idx = 0;

    // get current item index
    idx = GUILIST_GetCurItemIndex(ctrl_id);

    // get item user data
    GUILIST_GetItemData(ctrl_id, idx, &user_data);

    // enter relative window
    _callback_listItem = (WATCHCOM_Item_CallBack)(user_data);
    if (PNULL != _callback_listItem)
    {
        _callback_listItem();
    }
    else
    {
        SCI_TRACE_LOW("enter MMIList_Item_EnterWin, The enter_win_api is PNULL!");
    }
}
/*****************************************************************************/
//  Description: get list item visible flag
//  Parameter: [In] None
//             [Out] None
//             [Return] always return TRUE
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_List_Item_Visible_Default(void)
{
    return TRUE;
}


#ifdef ADULT_WATCH_SUPPORT

/*****************************************************************************/
//  Description : draw list ctrl--three string (height : small)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_3Str_3line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_3STR_3LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str3;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_3Str_3line Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--three string (height : small)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] str3: the third text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_4Str_3line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_STRING_T str3,
                                    MMI_STRING_T str4)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_4STR_3LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str3;

    item_data.item_content[3].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer     = str4;
    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_3Str_2line Append Failed");
    }
}


/*****************************************************************************/
//  Description : draw list ctrl-- two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_2Str_2line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_2STR_2LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_2Str_2line Append Failed");
    }

}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line Append Failed");
    }

}
/*****************************************************************************/
//  Description : draw list ctrl--one icon, two string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] pos:  the position of item
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Insert_1Icon_2Str_2line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    uint16 pos)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_InsertItem(listCtrlId, &item_t, pos))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Insert_1Icon_2Str_2line Insert Failed");
    }

}

/*****************************************************************************/
//  Description : draw list ctrl-- two string, one icon
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] str2: the second text buffer for display string
//             [In] image: the image ID for display image
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_STRING_T str2,
                                    MMI_IMAGE_ID_T image)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str2;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id        = image;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line Append Failed");
    }

}

/*****************************************************************************/
//  Description : draw list ctrl--one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1StrLeftAlign(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_LEFTALIGN_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1StrLeftAlign Append Failed");
    }
}
/*****************************************************************************/
//  Description : draw list ctrl--one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1StrCenterAlign(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1StrCenterAlign Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string,
                                    MMI_IMAGE_ID_T image)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1ICON_1LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id        = image;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Icon_1Str_1line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = string;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Icon_1Str_1line Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--one string (RadioBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Str_1Line_Radio(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Str_1Line_Radio Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--one string (RadioBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Str_1Line_Right_Radio(
	MMI_CTRL_ID_T listCtrlId,
	MMI_STRING_T string)
{
	GUILIST_ITEM_T          item_t          = {0};
	GUILIST_ITEM_DATA_T     item_data       = {0};

	item_t.item_style = GUIITEM_STYLE_1STR_1LINE_RADIO_RIGHT_ADULTWATCH;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer     = string;

	//设置状态值
	Watch_SetAllListState(listCtrlId);

	if(!GUILIST_AppendItem(listCtrlId, &item_t))
	{
		SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Str_1Line_Radio Append Failed");
	}
}

/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchCOM_RadioList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_total_num,MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }

    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    for(i=0; i<list_total_num; i++)
    {
        if(TRUE == list_content_ptr[i].item_visible()&& GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH == list_content_ptr[i].item_style)
        {
            WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST *)(list_content_ptr[i].data_ptr);
            MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
            AdultWatchCOM_ListItem_Draw_1Str_1Line_Radio(list_ctrl_id,item_str1);
            user_data = (uint32)(list_content_ptr[i].item_callback);
            GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
            item_index++;
        }
        else
        {
            SCI_TRACE_LOW("enter WATCHCOM_Radio_List_Create,item is not visible or item style is wrong");
            assert(0);

        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   miao.xi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchCOM_RadioList_Right_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_total_num,MMI_CTRL_ID_T list_ctrl_id)
{
	uint32 user_data = 0;
	uint16 item_index = 0;
	MMI_STRING_T item_str1={0};
	uint8 i = 0;
	if(PNULL == list_content_ptr)
	{
		SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
		return FALSE;
	}

	GUILIST_RemoveAllItems(list_ctrl_id);
	GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

	CTRLLIST_ChangeCheckListImagePosition(list_ctrl_id,0);
	for(i=0; i<list_total_num; i++)
	{
		if(TRUE == list_content_ptr[i].item_visible()&& GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH == list_content_ptr[i].item_style)
		{
			WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST *)(list_content_ptr[i].data_ptr);
			MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
			AdultWatchCOM_ListItem_Draw_1Str_1Line_Right_Radio(list_ctrl_id,item_str1);
			user_data = (uint32)(list_content_ptr[i].item_callback);
			GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
			item_index++;
		}
		else
		{
			SCI_TRACE_LOW("enter WATCHCOM_Radio_List_Create,item is not visible or item style is wrong");
			assert(0);

		}
	}
	return TRUE;
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon
//  Parameter: [In] listCtrlId: the list control ID
//             [In] image: the image ID for display image
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Icon_1line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T image)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_1LINE_CENTERALIGN_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = image;

    //设置状态值
    Watch_SetAllListState(listCtrlId);

    if(!GUILIST_AppendItem(listCtrlId, &item_t))
    {
        SCI_TRACE_LOW("watch_common_list.c:AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line Append Failed");
    }
}

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string, one icon
//  Parameter: [In] listCtrlId: the list control ID
//             [In] img1: the first image ID for display image
//             [In] str1: the first text buffer for display string
//             [In] img2: the second image ID for display image
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1line(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_IMAGE_ID_T img1,
                                    MMI_STRING_T str1,
                                    MMI_IMAGE_ID_T img2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1ICON_1LINE_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id        = img1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = str1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id        = img2;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}


/*****************************************************************************/
//  Description : draw list ctrl--one string (CheckBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_ListItem_Draw_2Str_2Line_Right_Check(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string,
                                    MMI_STRING_T string2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_2STR_2LINE_CHECK_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;
    
    item_data.item_content[1].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer     = string2;
       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   miao.xi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchCOM_CheckList_2Str_2Line_Right_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    MMI_STRING_T item_str2={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    CTRLLIST_ChangeCheckListImage(list_ctrl_id,IMAGE_SWITCH_OFF_SELECTED,IMAGE_SWITCH_ON_SELECTED);

    for(i=0; i<list_total_num; i++)
    {
        if(TRUE == list_content_ptr[i].item_visible())
        {  
            if(GUIITEM_STYLE_2STR_2LINE_CHECK_ADULTWATCH == list_content_ptr[i].item_style)
            {
                WATCHCOM_LIST_ITEM_STYLE_2STR_CHECK_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_2STR_CHECK_ST *)(list_content_ptr[i].data_ptr);
                MMI_GetLabelTextByLang(item_data.text_id1, &item_str1);
                MMI_GetLabelTextByLang(item_data.text_id2, &item_str2);
                AdultWatchCOM_ListItem_Draw_2Str_2Line_Right_Check(list_ctrl_id,item_str1,item_str2);
                user_data = (uint32)(list_content_ptr[i].item_callback);
                GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                item_index++;
            }
            else
            {
                SCI_TRACE_LOW("enter WATCHCOM_Check_List_Create,item is not visible or item style is wrong");
                assert(0);
            }
        }
    }
    GUILIST_SetMaxSelectedItem(list_ctrl_id, item_index);

    return TRUE;
}

/*****************************************************************************/
//  Description : draw list ctrl--one string (CheckBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchWatchCOM_ListItem_Draw_1Str_Left_Check(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}


/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchWatchCOM_CheckList_Left_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    CTRLLIST_ChangeCheckListImage(list_ctrl_id,IMAGE_AW_RADIO_UNSELECTED_ICON,IMAGE_AW_RADIO_SELECTED_ICON);
    CTRLLIST_ChangeCheckListImagePosition(list_ctrl_id,TRUE);

    for(i=0; i<list_total_num; i++)
    {

        if(TRUE == list_content_ptr[i].item_visible())
        {  
            if(GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH == list_content_ptr[i].item_style)
            {
                WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST *)(list_content_ptr[i].data_ptr);
                MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                AdultWatchWatchCOM_ListItem_Draw_1Str_Left_Check(list_ctrl_id,item_str1);
                user_data = (uint32)(list_content_ptr[i].item_callback);
                GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                item_index++;
            }
            else
            {
                SCI_TRACE_LOW("enter WATCHCOM_Check_List_Create,item is not visible or item style is wrong");
                assert(0);
            }
        }
    }
    GUILIST_SetMaxSelectedItem(list_ctrl_id, item_index);

    return TRUE;
}
/*****************************************************************************/
//  Description : draw list ctrl--one string (CheckBox)
//  Parameter: [In] listCtrlId: the list control ID
//             [In] string: the text buffer for display string
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchWatchCOM_ListItem_Draw_1Str_Right_Check(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T string)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1LINE_RADIO_RIGHT_ADULTWATCH;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = string;

       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}
/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   miao.xi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchWatchCOM_CheckList_Right_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id)
{
    uint32 user_data = 0;
    uint16 item_index = 0;
    MMI_STRING_T item_str1={0};
    uint8 i = 0;
    if(PNULL == list_content_ptr)
    {
        SCI_TRACE_LOW("enter WATCHCOM_List_Create, list_content_ptr is null");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, list_total_num, FALSE);//must before append list

    CTRLLIST_ChangeCheckListImage(list_ctrl_id,IMAGE_SWITCH_ON_SELECTED,IMAGE_SWITCH_OFF_SELECTED);

    for(i=0; i<list_total_num; i++)
    {
        //SCI_MEMSET(item_str1.wstr_ptr,0x00,item_str1.wstr_len*sizeof(wchar));
        if(TRUE == list_content_ptr[i].item_visible())
        {  
            if(GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH == list_content_ptr[i].item_style)
            {
                WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST item_data = *(WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST *)(list_content_ptr[i].data_ptr);
                MMI_GetLabelTextByLang(item_data.text_id, &item_str1);
                AdultWatchWatchCOM_ListItem_Draw_1Str_Right_Check(list_ctrl_id,item_str1);
                user_data = (uint32)(list_content_ptr[i].item_callback);
                GUILIST_SetItemUserData(list_ctrl_id, item_index, &user_data);
                item_index++;
            }
            else
            {
                SCI_TRACE_LOW("enter WATCHCOM_Check_List_Create,item is not visible or item style is wrong");
                assert(0);
            }
        }
    }
    GUILIST_SetMaxSelectedItem(list_ctrl_id, item_index);

    return TRUE;
}
/*****************************************************************************/
//  Description: set modify item font color
//  Parameter: [In] listCtrlId: the CTRL ID of listbox
//                  [In] item_index: item index
//                  [In] item_font_color: item font color
//                  [In] is_modify: if modify item font color
//                  [Out] None
//                  [Return] None
//  Author:   dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Modify_OneItem_FontColor(
                                    MMI_CTRL_ID_T listCtrlId,
                                    uint32               item_index,
                                    GUI_COLOR_T    item_font_color,
                                    BOOLEAN           is_modify
                                    )
{
    CTRLLIST_SetCurItemIndex(listCtrlId,item_index);
    CTRLLIST_SetTopItemIndex(listCtrlId,item_index);
    CTRLIST_SetOneItemFontColor(listCtrlId,item_index,item_font_color,is_modify);
}
#endif

/*****************************************************************************/
//  Description : draw list ctrl--one string, one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] img1: the first image ID for display image
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_1Icon_1Str_layout1(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_IMAGE_ID_T img1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT1;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id        = img1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;
       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

/*****************************************************************************/
//  Description : draw list ctrl--one string, one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] str1: the first text buffer for display string
//             [In] img1: the first image ID for display image
//             [In] str2: the second text buffer for display string
//             [Out] None
//             [Return] None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_1Icon_1Str_layout2(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_IMAGE_ID_T img1,
                                    MMI_STRING_T str2)
{
    GUILIST_ITEM_T          item_t          = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};

    item_t.item_style = GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT2;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer     = str1;

    item_data.item_content[1].item_data_type            = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id        = img1;

    item_data.item_content[2].item_data_type            = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer     = str2;
       //设置状态值
    Watch_SetAllListState(listCtrlId);

    GUILIST_AppendItem(listCtrlId, &item_t);

}

