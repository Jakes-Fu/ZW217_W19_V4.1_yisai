/*****************************************************************************
** File Name:       watch_common_list.h                                            *
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
#ifndef _WATCH_COMMON__LIST_H_
#define _WATCH_COMMON__LIST_H_

#include "mmk_type.h"
#include "mmitheme_list.h"
#include "ctrllist_export.h"

#ifdef ZTE_SUPPORT_240X284

#ifdef ZTE_WATCH
#define WATCHK1LIST_RECT                         DP2PX_RECT(0,20,239,195)//list rect
#define WATCHK1_SETOK_RECT				 DP2PX_RECT(12,214,228,254)//setok rect
#endif
#define WATCHLIST_RECT                         DP2PX_RECT(0,46,239,283)//list rect
#else
#ifdef ZTE_WATCH
#define WATCHK1LIST_RECT                         DP2PX_RECT(0,10,239,195-12)//list rect
#define WATCHK1_SETOK_RECT				 DP2PX_RECT(12,214-20,228,254-20)//setok rect
#endif
#define WATCHLIST_RECT                         DP2PX_RECT(0,46,239,239)//list rect
#endif
#define GUIITEM_STYLE_3STR_1MARKER GUIITEM_STYLE_2STR_1ICON_2STR

typedef BOOLEAN (*WATCHCOM_Item_Visible)(void);
typedef void    (*WATCHCOM_Item_CallBack)(void);
typedef uint32  (*WATCHCOM_Item_markNum)(void);
typedef BOOLEAN (*WATCHCOM_Item_SwitchOnOff)(void);


typedef struct
{
    MMI_TEXT_ID_T                text_id;
}WATCHCOM_LIST_ITEM_STYLE_1STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}WATCHCOM_LIST_ITEM_STYLE_2STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
    MMI_TEXT_ID_T                text_id3;
}WATCHCOM_LIST_ITEM_STYLE_3STR_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id;
    MMI_TEXT_ID_T                text_id;
}WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id;
    WATCHCOM_Item_SwitchOnOff      switch_onoff;
}WATCHCOM_LIST_ITEM_STYLE_1STR_SWITCH_ST;
typedef struct
{
    GUIANIM_FILE_INFO_T    icon_info;
    MMI_STRING_T           str1;
}WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_DYNA_T;
typedef struct
{
    GUIANIM_FILE_INFO_T    icon_info;
    MMI_STRING_T           str1;
    MMI_BUTTON_T           button_info1;
    MMI_BUTTON_T           button_info2;
    MMI_STRING_T           str2;
}WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T;
typedef struct
{   
    MMI_IMAGE_ID_T               image_id;
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}WATCHCOM_LIST_ITEM_STYLE_2STR_1ICON_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
    MMI_TEXT_ID_T                text_id3;
    WATCHCOM_Item_markNum          maker_num;
}WATCHCOM_LIST_ITEM_STYLE_2STR_1MARKER_1STR_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id1;
    MMI_IMAGE_ID_T               image_id2;
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_1ICON_1STR_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id;
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
    MMI_TEXT_ID_T                text_id3;
}WATCHCOM_LIST_ITEM_STYLE_3STR_1ICON_ST;

typedef struct
{
    MMI_TEXT_ID_T                text_id;
}WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST;

typedef struct
{
    MMI_TEXT_ID_T                text_id;
}WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST;

typedef struct
{
    WATCHCOM_Item_Visible         item_visible;
    uint32                      item_style;
    void                        *data_ptr;
    WATCHCOM_Item_CallBack        item_callback;
}WATCHCOM_LIST_ITEM__ST;


#ifdef ADULT_WATCH_SUPPORT
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
    MMI_TEXT_ID_T                text_id3;
}ADULTWATCHCOM_LIST_ITEM_STYLE_3STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
    MMI_TEXT_ID_T                text_id3;
    MMI_TEXT_ID_T                text_id4;
}ADULTWATCHCOM_LIST_ITEM_STYLE_4STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}ADULTWATCHCOM_LIST_ITEM_STYLE_2STR_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id;
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id;
}ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_LEFTALIGN_ST;
typedef struct
{
    MMI_TEXT_ID_T                text_id;
    MMI_IMAGE_ID_T               image_id;
}ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id;
}ADULTWATCHCOM_LIST_ITEM_STYLE_1ICON_ST;
typedef struct
{
    MMI_IMAGE_ID_T               image_id;
    MMI_TEXT_ID_T                text_id;
    MMI_IMAGE_ID_T               image_id2;
}ADULTWATCHCOM_LIST_ITEM_STYLE_2ICON_1STR_ST;

typedef struct
{
    MMI_TEXT_ID_T                text_id1;
    MMI_TEXT_ID_T                text_id2;
}WATCHCOM_LIST_ITEM_STYLE_2STR_CHECK_ST;
#endif


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
                                    MMI_STRING_T str3);

/*****************************************************************************/
//  Description : draw list ctrl--one icon, one string
//  Parameter: [In] listCtrlId: the list control ID
//             [In] icon_info: the icon info for display image
//             [In] str1 :the text buffer for display string
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
                                    uint16 item_index);

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
                                    uint16 item_index);

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
                                    MMI_STRING_T string);

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
                                    MMI_STRING_T string);

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
                                    MMI_IMAGE_ID_T image);

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
                                    MMI_STRING_T str2);

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
                                    uint16 pos);

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
                                    MMI_STRING_T str2);

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
                                    MMI_IMAGE_ID_T image);

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
                                    MMI_STRING_T str2);

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
                                    MMI_STRING_T str3);

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
                                    MMI_STRING_T str3);

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
                                    MMI_STRING_T str4);

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
                                    MMI_STRING_T string);

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
                                    MMI_STRING_T string);

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
                                    BOOLEAN isMark);

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
                                    uint32 number);


/*****************************************************************************/
//  Description: enter the relative window with selected list item
//  Parameter: [In] ctrl_id: the list control ID
//             [Out] None
//             [Return] None
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_List_Item_CallBack(MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
//  Description: appand list
//  Parameter: [In] MMI_LIST_ITEM_DATA_ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_TextList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_num, MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description: get list item visible flag
//  Parameter: [In] None
//             [Out] None
//             [Return] always return TRUE
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_List_Item_Visible_Default(void);

/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_RadioList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_num,MMI_CTRL_ID_T list_ctrl_id);
/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchCOM_CheckList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_num,MMI_CTRL_ID_T list_ctrl_id);
#endif
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
                                    MMI_STRING_T str3);

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
                                    MMI_STRING_T str4);

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
                                    MMI_STRING_T str2);

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
                                    MMI_STRING_T str2);

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
                                    uint16 pos);

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
                                    MMI_IMAGE_ID_T image);

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
                                    MMI_STRING_T string);

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
                                    MMI_STRING_T string);

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
                                    MMI_IMAGE_ID_T image);

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
                                    MMI_STRING_T string);
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
                                    MMI_STRING_T string);

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
	MMI_STRING_T string);
/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchCOM_RadioList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_total_num,MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Description: appand radio list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   miao.xi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchCOM_RadioList_Right_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr, uint32 list_total_num,MMI_CTRL_ID_T list_ctrl_id);

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
                                    MMI_IMAGE_ID_T image);
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
                                    MMI_IMAGE_ID_T img2);
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
                                    MMI_STRING_T string);
/*****************************************************************************/
//  Description: appand check list
//  Parameter: [In] WATCHCOM_LIST_ITEM__ST: the content of list
//             [In]MMI_CTRL_ID_T: the CTRL ID of listbox
//             [Return]append list suc or failed
//  Author:   miao.xi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchWatchCOM_CheckList_Right_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,uint32 list_total_num, MMI_CTRL_ID_T list_ctrl_id);
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
                                    MMI_STRING_T str2);
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
                                    uint32 item_index,
                                    GUI_COLOR_T item_font_color,
                                    BOOLEAN is_modify
                                    );

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
                                    MMI_STRING_T str2);

/*****************************************************************************/
//	Description : draw list ctrl--one string, one icon, one string
//	Parameter: [In] listCtrlId: the list control ID
//			   [In] str1: the first text buffer for display string
//			   [In] img1: the first image ID for display image
//			   [In] str2: the second text buffer for display string
//			   [Out] None
//			   [Return] None
//	Author: yifan.yang
//	Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ListItem_Draw_1Str_1Icon_1Str_layout12(
                                    MMI_CTRL_ID_T listCtrlId,
                                    MMI_STRING_T str1,
                                    MMI_IMAGE_ID_T img1,
                                    MMI_STRING_T str2);
